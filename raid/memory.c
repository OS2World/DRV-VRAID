/*
 * $Source: R:/source/driver/raid/RCS\\memory.c,v $
 * $Revision: 1.13 $
 * $Date: 2001/08/03 02:18:05 $
 * $Locker:  $
 *
 *	Memory allocation routines.
 *	They use a vector inside an additional data segment which
 *	is locked by our driver (IOPL plus DevHelper).
 *	OBS: use memory from this pool only for short-time allocations!
 *
 * $Log: memory.c,v $
 * Revision 1.13  2001/08/03 02:18:05  vitus
 * - AllocateFixed: only allocate memory below 16 if needed
 *
 * Revision 1.12  2001/07/03 07:52:24  vitus
 * - AllocateFixed: removed VMDHA_CONTIG flag, sc/ga lists are OK
 *
 * Revision 1.11  2001/06/24 22:59:42  vitus
 * - implemented AllocateFixed(), FreeFixed()
 *
 * Revision 1.10  1999/09/19 23:40:15  vitus
 * - defined all DBGMSG() strings as located in CONST segment
 *
 * Revision 1.9  1999/06/22 00:03:58  vitus
 * - renamed InitWorkmem to BuildWorkmem
 * - implemented ReleaseWorkmem, to unlock the memory
 *
 * Revision 1.8  1999/06/19 00:25:20  vitus
 * - ein paar statistische Daten mitfÅhren...
 *
 * Revision 1.7  1999/05/11 23:52:59  vitus
 * - added CRC code from init1.c
 *
 * Revision 1.6  1998/12/20 01:55:31  vitus
 * - lock memory as "any type"
 *
 * Revision 1.5  1998/09/07 23:15:01  vitus
 * - memory routines contain more debugging (mem overwrites)
 * - AllocateIorb(USHORT const sgcnt)
 *
 * Revision 1.4  1998/06/26 22:56:48  vitus
 * - added missing locking (did cause hangups!)
 *
 * Revision 1.3  1998/04/08 01:14:16  vitus
 * - uses static buffer as memory pool (simpler but change later)
 *
 * Revision 1.2  1998/03/10 02:40:33  vitus
 * - made lowest memory allocation routines public
 *
 * Revision 1.1  1998/02/17 22:31:17  vitus
 * Initial revision
 * -------------------------------------------
 * This code is Copyright Vitus Jensen 1998-99
 */
#if defined(__WATCOMC__)
# pragma off (unreferenced)
#endif
static char const id[]="$Id: memory.c,v 1.13 2001/08/03 02:18:05 vitus Exp $";
#if defined(__WATCOMC__)
# pragma on (unreferenced)
#endif

#include <string.h>

#define INCL_INITRP_ONLY
#include "os2intf.h"

#include "vraid.h"
#include "proto.h"
#include "extern.h"




#if defined(DEBUG)
CONSTMSG dszAllocIorb[]=	"\r\n[AllocateIorb] %p";
CONSTMSG dszFreeIorb[]=		"\r\n[FreeIorb] %p";
CONSTMSG dszAllocVrio[]=	"\r\n[AllocateVrio] %p";
CONSTMSG dszFreeVrio[]=		"\r\n[FreeVrio] %p";
CONSTMSG dszBuildMem[]=		"\r\n[BuildWorkmem] entry";
CONSTMSG dszBuildMemLock[]=	"\r\n[BuildWorkmem] Lock() failed - rc %w";
CONSTMSG dszReleaseMem[]=	"\r\n[ReleaseWorkmem] entry";
CONSTMSG dszReleaseMemNothing[]="\r\n[ReleaseWorkmem] nothing to do!";
CONSTMSG dszReleaseMemUnlock[]=	"\r\n[ReleaseWorkmem] UnLock() failed - rc %w";

CONSTMSG dszAllocFixed1[]=	"\r\n[AllocateFixed] result placed at %p (= lin %lx)";
CONSTMSG dszAllocFixed2[]=	"\r\n[AllocateFixed] got memory from phys %lx = lin %lx";
CONSTMSG dszFreeFixed1[]=	"\r\n[FreeFixed] freeing %lx";
CONSTMSG dszFreeFixed2[]=	"\r\n[FreeFixed] FreePhys - error %w";
#endif






/* Define memory pool.  It should reside in a seperate, non-swappable
 * segment.  This is acomplished by referencing a segment 'WORKMEM'
 * defined in our linker .def file (see Makefile). */

#define _inmem		_based(_segname("WORKMEM"))
#define MAGIX1		0x1089
#define MAGIX2		0xDA04

PRIVATE UCHAR _inmem	abWorkMem[WORKMEM_SIZE]; 	/* 1st element!!! */

typedef struct _HEADER {
    struct _HEADER FAR * ptr;
    USHORT		magix;			/* MAGIX1 */
    USHORT		size;			/* [HEADER] */
} HEADER, FAR * PHEADER;

typedef struct _FOOTER {
    USHORT	magix[2];			/* MAGIX2, ~MAGIX2 */
    PHEADER	header;
} FOOTER, FAR * PFOOTER;			/* same size as HEADER! */



PRIVATE HEADER _inmem	base = {0};		/* leere Liste zur Initial. */
PRIVATE PHEADER	_inmem	allocp = NULL;		/* letzter vergebener Block */
PRIVATE ULONG _inmem	hlMem = 0;		/* lock handle for above */
PRIVATE ULONG _inmem	hdMemLock = 0;		/* from DevHelp_Lock() */

PRIVATE USHORT		CRCtable[256] = {0};




/*# ----------------------------------------------------------------------
 * CALL
 *	MemFree(ap)
 * PARAMETER
 *	ap	allocated memory region
 * RETURNS
 *	(nothing)
 * GLOBAL
 *	allocp
 * DESCRIPTION
 *	Frees a previously allocated memory region.
 *
 * REMARKS
 *	From K&R pg 192
 */
PUBLIC VOID NEAR _Cdecl
MemFree(PVOID const ap)
{
    PHEADER const	p = (PHEADER)ap - 1;	/* auf Beschreibung zeigen */
    PFOOTER const	f = (PFOOTER)(p + p->size - 1);
    register PHEADER	q;

    ASSERT( p->magix == MAGIX1 );
    ASSERT( f->magix[0] == MAGIX2 );
    ASSERT( f->magix[1] == ~MAGIX2 );
    ASSERT( f->header == p );
#if defined(DEBUG)
    _fmemset(ap, 0xCC, p->size+sizeof(FOOTER));
#endif

    LOCK(hlMem);
    for( q = allocp; !(p > q  &&  p < q->ptr); q = q->ptr )
	if( q >= q->ptr  &&  (p > q  ||  p < q->ptr) )
	    break;				/* am einen oder anderen Ende */

    if( p + p->size == q->ptr )
    {
	/* Mit Nachfolger kombinieren */

	p->size += q->ptr->size;
	p->ptr = q->ptr->ptr;
    }
    else
	p->ptr = q->ptr;
    if( q + q->size == p )
    {
	/* Mit VorgÑnger kombinieren */

	q->size += p->size;
	q->ptr = p->ptr;
    }
    else
	q->ptr = p;
    allocp = q;
    UNLOCK(hlMem);

    return;
}




/*# ----------------------------------------------------------------------
 * CALL
 *	MemAlloc(nbytes)
 * PARAMETER
 *	nbytes			size of object to allocate
 * RETURNS
 *	pointer to allocated memory or NULL
 * GLOBAL
 *	allocp
 * DESCRIPTION
 *	Allocates region of memory.
 *
 * REMARKS
 *	From K&R pg 189
 */
PUBLIC PVOID NEAR _Cdecl
MemAlloc(USHORT const nbytes)
{
    register PHEADER	p;
    register PHEADER	q;
    USHORT const	nunits = (1
				  + (nbytes + sizeof(HEADER) - 1) / sizeof(HEADER)
				  + 1);		/* includes FOOTER! */
    PVOID		result = NULL;		/* default: no room */

    LOCK(hlMem);
    q = allocp;
    for( p = q->ptr; ; q = p, p = p->ptr )
    {
	if( p->size >= nunits )			/* gro· genug? */
	{
	    if( p->size == nunits )		/* passt genau? */
		q->ptr = p->ptr;
	    else
	    {
		p->size -= nunits;
		p += p->size;
		p->size = nunits;
	    }
	    allocp = q;
	    result = (PVOID)(p+1);
	    p->magix = MAGIX1;
	    {
		PFOOTER f = (PFOOTER)(p + p->size - 1);

		f->magix[0] = MAGIX2;
		f->magix[1] = ~MAGIX2;
		f->header = p;
	    }
	    break;
	}
	if( p == allocp )			/* einmal rum um freie Liste? */
	    break;				/* kein Platz mehr frei! */
    }
    UNLOCK(hlMem);

    return result;
}






/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * CALL
 *	AllocateIorb(sgcnt)
 * PARAMETER
 *	sgcnt		size of S/G list
 * RETURNS
 *	/0		pointer to IORB
 *	0		no memory available
 * GLOBAL
 *	(none)
 * DESCRIPTION
 *	Allocates buffer containing IORB of maximum size and (directly
 *	following) a scatter/gather list of 'sgcnt' elements.
 *	Pointer to scatter/gather list is set up and memory is
 *	initialized to 0.
 *
 * REMARKS
 */
PUBLIC PIORB_EXECUTEIO NEAR _Cdecl
AllocateIorb(USHORT const sgcnt)
{
    USHORT const	cb = MAX_IORB_SIZE + sgcnt * sizeof(SCATGATENTRY);
    PIORB_EXECUTEIO const iorb = MemAlloc(cb);

    if( iorb != NULL )
    {
	_fmemset(iorb, 0, cb);			/* !!! */
	iorb->pSGList = (PVOID)((PUCHAR)iorb + MAX_IORB_SIZE);
	iorb->ppSGList = ppWorkMem + FP_OFF(iorb->pSGList);

	++cIorbAllocated;
    }

    DEBMSG1(DBG_MEMORY,dszAllocIorb,(PVOID)iorb);
    return iorb;
}




/*# ----------------------------------------------------------------------
 * CALL
 *	FreeIorb(iorb)
 * PARAMETER
 *	iorb		allocated IORB
 * RETURNS
 *	(nothing)
 * GLOBAL
 *	(none)
 * DESCRIPTION
 *	Frees previously allocated IORB (plus S/G list).
 *
 * REMARKS
 */
PUBLIC VOID NEAR _Cdecl
FreeIorb(PIORB_EXECUTEIO iorb)
{
    DEBMSG1(DBG_MEMORY,dszFreeIorb,(PVOID)iorb);

    ASSERT( cIorbAllocated != 0 );
    --cIorbAllocated;

    MemFree(iorb);
}




/*# ----------------------------------------------------------------------
 * CALL
 *	AllocateVrio(working_dev,mother)
 * PARAMETER
 *	working_dev
 *	mother		mother of this I/O
 * RETURNS
 *	PVRIO		allocated structure
 * GLOBAL
 *	(none)
 * DESCRIPTION
 *	Allocates and initializes (zeroes) VRIO structure.
 *
 * REMARKS
 */
PUBLIC PVRIO NEAR _Cdecl
AllocateVrio(PVRDEVHDR working_dev,PVRIO mother)
{
    PVRIO const io = MemAlloc(sizeof(VRIO));

    if( io != NULL )
    {
	_fmemset(io, 0, sizeof(VRIO));
	io->mother = mother;
	io->vrd = working_dev;

	++cVrioAllocated;
    }
    DEBMSG1(DBG_MEMORY,dszAllocVrio,(PVOID)io);
    return io;
}




/*# ----------------------------------------------------------------------
 * CALL
 *	FreeVrio(io)
 * PARAMETER
 *	io		allocated structure
 * RETURNS
 *	(nothing)
 * GLOBAL
 *	(none)
 * DESCRIPTION
 *	Frees previously allocated structure.
 *
 * REMARKS
 */
PUBLIC VOID NEAR _Cdecl
FreeVrio(PVRIO io)
{
    DEBMSG1(DBG_MEMORY,dszFreeVrio,(PVOID)io);

    ASSERT( cVrioAllocated != 0 );
    --cVrioAllocated;

    MemFree(io);
}




/*# ----------------------------------------------------------------------
 * CALL
 *	BuildWorkmem(void)
 * PARAMETER
 *	(none)
 * RETURNS
 *	0		OK
 *	/0		OS/2 kernel error?
 *
 * DESCRIPTION
 *	Initializes memory allocation routines.
 *
 * REMARKS
 */
PUBLIC USHORT NEAR _Cdecl
BuildWorkmem(void)
{
    USHORT	rc;
    PVOID	pv = abWorkMem;

    DEBMSG(DBG_INIT1,dszBuildMem);


    /* Lock whole segment of 'abWorkMem'. */

    rc = DevHelp_Lock(FP_SEL(pv), LOCKTYPE_LONG_ANYMEM, 0, &hdMemLock);
    if( rc != 0 )
    {
	DEBMSG1(DBG_INIT1,dszBuildMemLock,rc);
	return rc;
    }


    /* Calculate physical address of 'abWorkMem'. */

    if( (rc=DevHelp_VirtToPhys(pv, &ppWorkMem)) )
    {
	sprintk(szMsgBuffer, "%s: error converting virtual to physical address",
		(PSZ)szDriverName);
	SaveMessage();
	return rc;
    }


    /* Initialize memory routines. */

    {
	PHEADER	x = (PVOID)abWorkMem;

	/* Noch keine freie Liste */

	base.ptr = allocp = &base;
	base.size = 0;
	x->size = (WORKMEM_SIZE - sizeof(HEADER)) / sizeof(HEADER);
#if defined(DEBUG)
	x->magix = MAGIX1;
	((PFOOTER)(x + x->size - 1))->magix[0] = MAGIX2;
	((PFOOTER)(x + x->size - 1))->magix[1] = ~MAGIX2;
	((PFOOTER)(x + x->size - 1))->header = x;
#endif
	MemFree(x+1);
    }

    return 0;
}




/*# ----------------------------------------------------------------------
 * CALL
 *	ReleaseWorkmem(void)
 * PARAMETER
 *	(none)
 * RETURNS
 *	0		OK
 *	/0		OS/2 kernel error?
 *
 * DESCRIPTION
 *	Releases any resources relating to our memory memagement.
 *
 * REMARKS
 */
PUBLIC USHORT NEAR _Cdecl
ReleaseWorkmem(void)
{
    USHORT	rc;

    DEBMSG(DBG_INIT1,dszReleaseMem);

    if( hdMemLock == 0 )
    {
	DEBMSG(DBG_INIT1,dszReleaseMemNothing);
	return 0;
    }


    rc = DevHelp_UnLock(hdMemLock);
    if( rc != 0 )
    {
	DEBMSG1(DBG_MEMORY,dszReleaseMemUnlock,rc);
    }
    return rc;
}






/* **********************************************************************
 * **** Big Memory Buffers **********************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * AllocateFixed(size, plin)
 *
 * PARAMETER
 *	size		byte count to allocate
 *	plin		linear address of allocated memory
 *
 * RETURNS
 *	0		OK
 *	/0		no memory available
 *
 * DESCRIPTION
 *	Allocates fixed continous memory.
 *
 * REMARKS
 *	Call only at task or kernel time!
 */
PUBLIC USHORT NEAR _Cdecl
AllocateFixed(ULONG size, PLIN plin)
{
    ULONG	phys_addr;
    PULONG 	pphys = &phys_addr;
    LIN		lin_of_physvar;			/*lin of 'phys' (VMAlloc wants it)*/
    PVOID	dummy;
    USHORT	error;


    error = DevHelp_VirtToLin(SELECTOROF(pphys), OFFSETOF(pphys),
			      &lin_of_physvar);
    if( error == 0 )
    {
	ULONG	flags = VMDHA_FIXED;

	if( (fADDFlags & AF_NOT16M) )
	    flags |= VMDHA_16M;			/* there is some ISA adapter */

	DEBMSG2(DBG_LEVEL2,dszAllocFixed1,pphys,(PVOID)&lin_of_physvar);
	error = DevHelp_VMAlloc(flags, size, lin_of_physvar, plin, &dummy);
	DEBMSG2(DBG_LEVEL1,dszAllocFixed2,pphys,plin);
    }
    return error;
}




/*# ----------------------------------------------------------------------
 * FreeFixed(lin)
 *
 * PARAMETER
 *	lin		linear address of memory block
 *
 * RETURNS
 *	0		OK
 *	/0		some error
 *
 * DESCRIPTION
 *	Frees memory allocated by AllocateFixed().
 *
 * REMARKS
 *	Call only at task or kernel time!
 */
PUBLIC USHORT NEAR _Cdecl
FreeFixed(LIN lin)
{
    USHORT	error;

    DEBMSG1(DBG_LEVEL1,dszFreeFixed1,(PVOID)&lin);
    error = DevHelp_VMFree(lin);
    if( error != 0 )
    {
	DEBMSG1(DBG_LEVEL1,dszFreeFixed2,error);
    }
    return error;
}






/* **********************************************************************
 * **** Small Helpers (CRC16 calculation, see setup/crc.c) **************
 * ******************************************************************* */

PRIVATE USHORT
CalcTable(USHORT data,USHORT genpoly,USHORT accum)
{
    int i;

    for( i = 0; i < 8; ++i )
    {
	if( (accum^data) & 1 )
	{
	    accum >>=1;
	    accum ^= genpoly;
	}
	else
	    accum >>= 1;

	data >>= 1;
    }
    return accum;
}




PRIVATE USHORT
UpdateCRC(USHORT crc,UCHAR byte)
{
    /* bei MSB first: */
    /* return( (crc << 8) ^ CRCtable[ (crc >> 8) ^ byte ] ); */

    /* bei LSB first: */
    return ((crc >> 8) ^ CRCtable[ (crc ^ byte) & 0xFF ]);
}




/*# ----------------------------------------------------------------------
 * CALL
 *	CrcInit(uPOLYNOM)
 * PARAMETER
 *	uPOLYNOM		generator polynom
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Builds table for CRC16 calculation.  Call once per session.
 *
 * REMARKS
 */
PUBLIC VOID NEAR _Cdecl
CrcInit(USHORT uPOLYNOM)
{
    int i;

    for( i = 0; i < 256; ++i )
	CRCtable[i] = CalcTable(i, uPOLYNOM, 0);
    return;
}




/*# ----------------------------------------------------------------------
 * CALL
 *	Crc16(buffer,cb)
 * PARAMETER
 *	buffer		data buffer
 *	cb		size of above
 *
 * RETURNS
 *	CRC16
 *
 * DESCRIPTION
 *	Calculates CRC16 over buffer contents.
 *
 * REMARKS
 */
PUBLIC USHORT NEAR _Cdecl
Crc16(PVOID buffer,USHORT cb)
{
    USHORT	crc = 0;
    PUCHAR	p = buffer;

    for(; cb != 0; --cb, ++p )
	crc = UpdateCRC(crc, *p);
    return crc;
}
