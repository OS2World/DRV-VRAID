/*
 * $Source: R:/source/driver/raid/RCS\\single.c,v $
 * $Revision: 2.3 $
 * $Date: 2001/07/12 02:20:35 $
 * $Locker:  $
 *
 *
 *	How to do I/O to a RDTYPE_SINGLE.
 *
 *	Remember that all retries are up to ADDs.  Any error
 *	is fatal (strictly spoken).
 *
 *
 * $Log: single.c,v $
 * Revision 2.3  2001/07/12 02:20:35  vitus
 * - SingleCreate: more debugging output for created array
 *
 * Revision 2.2  2001/02/11 01:17:26  vitus
 * - changed SingleCtrl() to return DRVSTATUS
 *
 * Revision 2.1  2000/06/13 01:43:47  vitus
 * - implemented ioctl method (VRDIOCTL_QUERY_CHILDREN, VRDIOCTL_INFO)
 *
 * Revision 1.15  1999/09/26 00:21:22  vitus
 * - defined all DBGMSG() strings as located in CONST segment
 *
 * Revision 1.14  1999/08/09 23:18:07  vitus
 * - SingleCreate: initialize new VRDSINGLE structure with 0
 *
 * Revision 1.13  1999/06/21 02:26:43  vitus
 * - corrected english spelling
 *
 * Revision 1.12  1999/05/26 00:47:47  vitus
 * - SingleCreate: return value now 'int', but as it is always 0...
 *
 * Revision 1.11  1999/05/12 01:48:25  vitus
 * - dummy SingleUpdate() implemented
 *
 * Revision 1.10  1999/04/28 01:46:56  vitus
 * - merged SingleAdmin() code with Worker/Notify, IOCMD_A* commands
 * - SingleNotify() uses CompleteVrio()
 *
 * Revision 1.9  1999/02/07 17:53:08  vitus
 * - implemented SingleAdmin() method
 * - changed for iorb -> vrio transition
 *
 * Revision 1.8  1998/09/20 14:24:37  vitus
 * - modified for AllocateVrio(working_dev,mother)
 *
 * Revision 1.7  1998/06/26 23:03:24  vitus
 * - switched to DDK macros (cli, sti, pushf)
 * - worker,notify get const pointers and become _Cdecl
 *
 * Revision 1.6  1998/06/15 02:08:18  vitus
 * - moved all PHYSDEVICE handling to physdev.c.  Now single.c only handles
 *   the minimum VRAID device: RDTYPE_SINGLE
 *
 * Revision 1.5  1998/05/29 01:56:02  vitus
 * - fine tuning of DEBMSG macro calls
 * - corrected (again) block offset things
 *
 * Revision 1.4  1998/04/08 00:59:51  vitus
 * - corrected VRIO handling
 * - corrected some block offset things
 * - test (single, chain drive) successfull
 *
 * Revision 1.3  1998/03/10 02:06:47  vitus
 * - changed to PUBLIC *Create() function and PRIVATE workers
 *
 * Revision 1.2  1998/03/06 01:21:37  vitus
 * - entries now public (needed?)
 *
 * Revision 1.1  1998/02/12 00:58:41  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2001
 */
#if defined(__WATCOMC__)
# pragma off (unreferenced)
#endif
static char const vcid[]="$Id: single.c,v 2.3 2001/07/12 02:20:35 vitus Exp $";
#if defined(__WATCOMC__)
# pragma on (unreferenced)
#endif

#include <string.h>

#define INCL_NOBASEAPI
#define INCL_INITRP_ONLY
#include "os2intf.h"

#include "vraid.h"
#include "proto.h"
#include "extern.h"
#include "vraidpub.h"




/*
 * Our object: the single drive.
 */
typedef struct _VRDSINGLE {

    VRDEVHDR	header;				/* ALWAYS FIRST! */
    PVRDEVHDR	child;

} VRDSINGLE, FAR * PVRDSINGLE;




PRIVATE USHORT NEAR _Cdecl	SingleWorker(PVRDEVHDR const self,PVRIO const myio);
PRIVATE USHORT NEAR _Cdecl	SingleUpdate(PVRDEVHDR const self,PVOID const sec);
PRIVATE DRVSTATUS NEAR _Cdecl	SingleCtrl(PVRDEVHDR const self, USHORT const cmd,
					   PVOID const param, PVOID const data);




#if defined(DEBUG)
CONSTMSG dszSingleNotify[]=	"\r\n[SingleNotify] io %p, myio %p, error %w";
CONSTMSG dszSingleWorker[]=	"\r\n[SingleWorker] %p";
CONSTMSG dszSWorkerAlloc[]=	"\r\n[SingleWorker] couldn't allocate VRIO";
CONSTMSG dszSCreateAdminData[]=	"\r\n[SingleCreate] admin data\r\n%z";
CONSTMSG dszSingleCreate[]=	"\r\n[SingleCreate] at %p"
				"\r\n               ID %z"
				"\r\n               Size 0x%lx";
#endif






/*# ----------------------------------------------------------------------
 * SingleNotify(vrio)
 *
 * PARAMETER
 *	io		VRIO chich completed
 *
 * RETURNS
 *	(nothing)
 *
 * DESPRIPTION
 *	Used by a child to report that it's I/O has completed.
 *	Ends own VRIO as all children have processed.
 *
 * REMARKS
 */
PRIVATE void NEAR _Cdecl
SingleNotify(PVRIO const io)
{
    PVRIO const		myio = io->mother;
    /*PVRDSINGLE const	single = (PVRDSINGLE)myio->vrd;*/

    DEBMSG3(DBG_PATH,dszSingleNotify,io,myio,io->oserr);

    ASSERT( io->bcnt != 0 );
    ASSERT( io->link == NULL );
    ASSERT( myio->link == NULL );
    ASSERT( myio->bcnt != 0 );
    ASSERT( myio->vrd->size != 0 );
    ASSERT( myio->vrd->notify == SingleNotify );
    ASSERT( myio->vrd->worker == SingleWorker );

    pushf();
    cli();

    /* Process errors!  (Nothing to do?) */

    myio->oserr = io->oserr;			/* propagate error */

    CompleteVrio(myio);

    popf();
    return;
}




/*# ----------------------------------------------------------------------
 * SingleWorker(self,io)
 *
 * PARAMETER
 *	self		our single device
 *	io		VRIO describing I/O
 *
 * RETURNS
 *	0		OK, I/O started
 *	/0		please queue I/O for later processing
 *
 * DESPRIPTION
 *	Pass iorb to underlying physical device.  Nothing more.
 *
 * REMARKS
 */
PRIVATE USHORT NEAR _Cdecl
SingleWorker(PVRDEVHDR const self,PVRIO const myio)
{
    PVRDSINGLE const	single = (PVRDSINGLE)self;
    PVRIO const		newio = AllocateVrio(single->child, myio);
    USHORT		rc;

    DEBMSG1(DBG_PATH,dszSingleWorker,(PVOID)myio);

    ASSERT( myio->vrd == self );
    ASSERT( myio->link == NULL );
    ASSERT( myio->bcnt != 0 );
    ASSERT( single->header.size != 0 );
    ASSERT( single->header.notify == SingleNotify );
    ASSERT( single->header.worker == SingleWorker );
    ASSERT( single->header.update == SingleUpdate );
    ASSERT( single->header.ioctl == SingleCtrl );

    ++self->iocnt;
    if( newio == 0 )
    {
	DEBMSG(DBG_ALL,dszSWorkerAlloc);
	++self->busycnt;
	return !0;
    }

    newio->iorb = myio->iorb;
    newio->iocmd = myio->iocmd;
    newio->cSGList = myio->cSGList;
    newio->pSGList = myio->pSGList;
    newio->ppSGList = myio->ppSGList;

    switch( myio->iocmd )
    {
      case IOCMD_READ:
      case IOCMD_WRITE:
	newio->offset = myio->offset;
	newio->blk = myio->blk;
	newio->bcnt = myio->bcnt;

      case IOCMD_AREAD:
      case IOCMD_AWRITE:
	newio->offset = myio->offset;
	newio->blk = myio->blk + 1;		/* our configuration sector lies
						   behind our child's sector */
	newio->bcnt = myio->bcnt;
	break;
    }


    newio->oserr = 0xFFFF;
    newio->link = NULL;
    rc = newio->vrd->worker(newio->vrd, newio);
    if( rc )
	QueueVrio(newio);

    return 0;
}




/*# ----------------------------------------------------------------------
 * SingleUpdate(self,sec)
 *
 * PARAMETER
 *	mirror		a VRD which forms an array
 *	sec		sector contains configuration
 *
 * RETURNS
 *	0		don't write back, nothing changed
 *
 * DESCRIPTION
 *	Do nothing.
 *
 * REMARKS
 */
PRIVATE USHORT NEAR _Cdecl
SingleUpdate(PVRDEVHDR const self,PVOID const sec)
{
    PVRDSINGLE const	single = (PVRDSINGLE)self;

    ASSERT( single->header.size != 0 );
    ASSERT( single->header.notify == SingleNotify );
    ASSERT( single->header.worker == SingleWorker );
    ASSERT( single->header.update == SingleUpdate );
    ASSERT( single->header.ioctl == SingleCtrl );

    return 0;
}




/*# ----------------------------------------------------------------------
 * SingleCtrl(self,cmd.data)
 *
 * PARAMETER
 *	self		a VRD which forms a single device
 *	cmd		I/O Control Code
 *	data		data buffer
 *
 * RETURNS
 *	0		OK
 *	/0		error (driver status code)
 *
 * DESCRIPTION
 *	Special function interface
 *
 * REMARKS
 */
PRIVATE DRVSTATUS NEAR _Cdecl
SingleCtrl(PVRDEVHDR const self, USHORT const cmd,
	   PVOID const param, PVOID const data)
{
    PVRDSINGLE const	single = (PVRDSINGLE)self;
    DRVSTATUS		result = STATUS_ERR_UNKCMD;


    ASSERT( single->header.size != 0 );
    ASSERT( single->header.notify == SingleNotify );
    ASSERT( single->header.worker == SingleWorker );
    ASSERT( single->header.update == SingleUpdate );
    ASSERT( single->header.ioctl == SingleCtrl );

    switch( cmd )
    {
      case VRDIOCTL_QUERY_CHILDREN:
	*(PUSHORT)data = 1;
	result = 0;
	break;

      case VRDIOCTL_INFO:
	DEBMSG(DBG_PATH, "VRDIOCTL_INFO");
	{
	    PVRAID_ARRAYINFO_DATA	d = data;

	    _fmemcpy(d->id, single->header.id, sizeof(d->id));
	    d->reqcnt = single->header.iocnt;
	    d->size = single->header.size;

	    d->children = 1;
	    d->flags = single->header.flags;
	    d->reserved = 0;
	}
	result = 0;
	break;

      default:
	break;
    }

    return result;
} /* end[SingleCtrl] */






/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * SingleCreate(pVrh,pPhys,sec,pSize)
 *
 * PARAMETER
 *	pVrh		room to create device structure
 *	pPhys		physical device to create for
 *	sec		contents of configuration sector
 *	pSize		return: size used at '*pVrh'
 * RETURNS
 *	0		OK
 *	/0		couldn't create
 * GLOBAL
 * DESCRIPTION
 * REMARKS
 */
PUBLIC int NEAR _Cdecl
SingleCreate(PVRDEVHDR pVrh,PVRDEVHDR FAR * tab,PSEC_VRDEV sec,PUSHORT pSize)
{
    PVRDSINGLE const	single = (PVRDSINGLE)pVrh;
    USHORT		j;


    DEBMSG2(DBG_INIT3,dszSCreateAdminData,(PVOID)sec,SECTOR_SIZE);
    _fmemset(single, 0, sizeof(*single));


    /* Initialize header elements */

    single->header.worker = SingleWorker;	/* required */
    single->header.notify = SingleNotify;
    single->header.update = SingleUpdate;
    single->header.ioctl = SingleCtrl;
    single->header.size = 0;			/* initialize, wil be modified */
    single->header.flags = 0;
    _fmemcpy(single->header.id, sec->u.s.id, sizeof(single->header.id));

    /* Build linking by looking up all parts in 'tab'. */

    single->child = NULL;
    for( j = 0; j < MAX_PDEV; ++j )
    {
	if( tab[j] == NULL )
	    continue;				/* ignore empty entries */

	if( _fmemcmp(tab[j]->id, sec->u.s.child[0].id, sizeof(tab[j]->id)) == 0 )
	{
	    single->header.size = sec->u.s.child[0].size;
	    single->child = tab[j];
	    tab[j] = NULL;
	    break;				/* stop scan, no next child */
	}
    }

    *pSize = sizeof(*single);

    DEBMSG4(DBG_INIT1,dszSingleCreate,(PVOID)pVrh,(PVOID)sec->u.s.id,6,(PVOID)&single->header.size);
    return 0;
} /* end[SingleCreate] */
