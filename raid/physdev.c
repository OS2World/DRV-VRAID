/*
 * $Source: R:/source/driver/raid/RCS\\physdev.c,v $
 * $Revision: 1.15 $
 * $Date: 2000/10/24 23:59:37 $
 * $Locker:  $
 *
 *	How to do I/O to a PHYSDEVICE, the lowest level structure.
 *
 *	Remember that all retries are up to ADDs.  Any error
 *	is fatal (strictly spoken).
 *
 * $Log: physdev.c,v $
 * Revision 1.15  2000/10/24 23:59:37  vitus
 * - more debugging output
 *
 * Revision 1.14  2000/08/29 01:05:55  vitus
 * - very minor DEBMSGx() change
 *
 * Revision 1.13  1999/10/01 00:51:35  vitus
 * - updates new iocnt and busycnt fields in VRDEVHDR
 *
 * Revision 1.12  1999/09/23 02:44:38  vitus
 * - defined all DBGMSG() strings as located in CONST segment
 *
 * Revision 1.11  1999/08/09 23:21:06  vitus
 * - PhysdevCreate: initialize new VRDPHYS structure with 0
 *
 * Revision 1.10  1999/07/08 01:53:09  vitus
 * - IorbNotification: workaround for WatCom C bug
 *
 * Revision 1.9  1999/05/26 00:49:13  vitus
 * - PhysdevCreate: return value now 'int', but as it is always 0...
 *
 * Revision 1.8  1999/05/12 01:48:02  vitus
 * - dummy PhysdevUpdate() implemented
 *
 * Revision 1.7  1999/04/28 01:43:10  vitus
 * - merged PhysdevAdmin() code with Worker, IOCMD_A* commands
 * - PhysdevNotify() uses CompleteVrio()
 *
 * Revision 1.6  1999/04/21 00:31:53  vitus
 * - PhysdevPartIO() removed: replaced by admin I/Os
 *
 * Revision 1.5  1999/02/07 17:59:01  vitus
 * - implemented PhysdevAdmin() method
 * - changed for iorb -> vrio transition
 *
 * Revision 1.4  1998/11/05 22:35:23  vitus
 * - DevHelp_ProcBlock() now uses WAIT_IS_INTERRUPTABLE
 *
 * Revision 1.3  1998/09/20 14:06:20  vitus
 * - modified for AllocateIorb(sgcnt)
 * - added casts to correct 32bit calculation
 * - BuildSGList() returns error code if S/G list can't be built
 * - added debugging code to detect above calculation error
 *
 * Revision 1.2  1998/06/26 22:37:06  vitus
 * - switched to DDK macros (cli, sti, pushf)
 * - worker,notifyy get const pointers
 *
 * Revision 1.1  1998/06/15 02:06:43  vitus
 * Initial revision
 * -------------------------------------------
 * This code is Copyright Vitus Jensen 1998-99
 */
#if defined(__WATCOMC__)
# pragma off (unreferenced)
#endif
static char const id[]="$Id: physdev.c,v 1.15 2000/10/24 23:59:37 vitus Exp $";
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




/*
 * Our object: the single drive.
 */
typedef struct _VRDPHYS {

    VRDEVHDR	header;				/* ALWAYS FIRST! */
    PPHYSDEVICE	phys;

    USHORT	offset;				/* offset in PHYSDEVICE */
} VRDPHYS, FAR * PVRDPHYS;




/*
 * On commands to lower level drivers we have to link back to out VRIOs.  As
 * those commands don't have a DMD the workspace is free.
 */
typedef union _VRAID_LOWER_DMDSPACE {
    struct {
	char	dummy[DM_WORKSPACE_SIZE-1];
	UCHAR	chk;
    };
    struct {
	PPHYSDEVICE	self;
	PVRIO		vrio;


	PIORB	pOS2Request;			/* generated on behalf of
						   this OS/2 IORB */
    };
} VRAID_LOWER_DMDSPACE, FAR * PVRAID_LOWER_DMDSPACE;




PRIVATE USHORT NEAR _Cdecl	PhysdevWorker(PVRDEVHDR const self,PVRIO const myio);
PRIVATE USHORT NEAR _Cdecl	PhysdevUpdate(PVRDEVHDR const self,PVOID const sec);




#if defined(DEBUG)
CONSTMSG dszIorbNotification[]=	"\r\n[IorbNotification] iorb %p";
CONSTMSG dszINoteError[]=	"\r\nthis iorb %p, error %w from adapter %w/unit %w";
CONSTMSG dszIorbDump[]=		"\r\niorb dump\r\n%z";
CONSTMSG dszNoSGList[]=		"\r\n0 Sc/Ga 0 elements (%p)";
CONSTMSG dszSGDump[]=		"\r\nSc/Ga dump\r\n%z";
CONSTMSG dszVrioDump[]=		"\r\nVRIO dump\r\n%z";
CONSTMSG dszMasterIorb[]=	"\r\nmaster iorb %p, error %w";
CONSTMSG dszMasterDump[]=	"\r\nmaster dump\r\n%z";
CONSTMSG dszINoteWakeup[]=	"\r\n[IorbNotification] wakeup %p, error %w";
CONSTMSG dszPhysSendIORB[]=	"\r\n[PhysSendIORB] pdev %p, vrio %p, iorb %p";
CONSTMSG dszBSGL[]=		"\r\n[BSGL] input %p, start %lx";
CONSTMSG dszBSGLDump[]=		"\r\n[BSGL] input Sc/Ga dump\r\n%z";
CONSTMSG dszBSGLSkip[]=		"\r\n[BSGL] skipping %lx";
CONSTMSG dszBSGLIndex[]=	"\r\n[BSGL] !index %w >= size %w!";
CONSTMSG dszBSGLInput[]=	"\r\n[BSGL] input %p, start %lx, cb %lx";
CONSTMSG dszBSGLSGList[]=	"\r\n[BSGL] out->cSGList %w";
CONSTMSG dszBSGLCurrent[]=	"\r\n[BSGL] current 0x%lx, copied 0x%lx";
CONSTMSG dszBSGLAdd[]=		"\r\n[BSGL] added %lx from %lx";
CONSTMSG dszPhysdevNotify[]=	"\r\n[PhysdevNotify] myio %p";
CONSTMSG dszPhysdevWorker[]=	"\r\n[PhysdevWorker] vrio %p, iorb %p";
CONSTMSG dszPWorkerNoAlloc[]=	"\r\n[PhysdevWorker] couldn't allocate IORB";
CONSTMSG dszPWorkerInvalid[]=	"\r\n[PhysdevWorker] invalid iocmd %w";
CONSTMSG dszPWorkerBlocks[]=	"\r\n[PhysdevWorker] %w blocks from/to %lx";
CONSTMSG dszPhysdevCreate[]=	"\r\n[PhysdevCreate] at %p for phys %p"
                                "\r\n                ID %z";
CONSTMSG dszPCreateDump[]=	"\r\n[PhysdevCreate] SEC_PHYSDEV\r\n%z";
#endif






#if defined(DEBUG)
PRIVATE UCHAR
Chksum(PVOID const buf,USHORT l)
{
    PUCHAR	p = buf;
    UCHAR	c = 0;

    while( l-- )
	c += *(p++);
    return c;
}
#endif /* ?DEBUG */




/*# ----------------------------------------------------------------------
 * IorbNotification(iorb)
 *
 * PARAMETER
 *	iorb
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Notification routine to be called from ADD to signal that
 *	an IORB has completed.
 *	Converts information to VRIO format, frees IORB and calls
 *	VRIO notification routine.
 *
 * REMARKS
 *	This functions should do any special error handling, that's
 *	up to VRDEVICE devices.  PHYSDEVICEs should always be
 *	responsive to callers, no matter how badly broken the hardware is.
 */
PRIVATE PIORB FAR _loadds _Cdecl
IorbNotification(PIORB iorb)
{
    PVRAID_LOWER_DMDSPACE const ws = (PVRAID_LOWER_DMDSPACE)iorb->DMWorkSpace;
    PVRIO const			vrio = ws->vrio;


    DEBMSG1(DBG_PATH,dszIorbNotification,iorb);

    ASSERT( ws->chk == Chksum(ws, DM_WORKSPACE_SIZE-1) );
    ASSERT( ws->self != NULL );
    ASSERT( (iorb->Status & IORB_DONE) != 0 );	/* HAS to be completed! */

#if defined(DEBUG)
    if( (iorb->Status & IORB_ERROR)  &&  vrio != 0 )
    {
	PIORB_EXECUTEIO const	servant = (PVOID)iorb;

	DEBMSG4(DBG_LEVEL1,dszINoteError,(PVOID)iorb,iorb->ErrorCode,ws->self->iAdapter,ws->self->iUnit);
	DEBMSG2(DBG_LEVEL1,dszIorbDump,(PVOID)iorb,iorb->Length);
	if( servant->cSGList == 0 )
	    DEBMSG1(DBG_LEVEL1,dszNoSGList,servant->pSGList);
	else
	    DEBMSG2(DBG_LEVEL1,dszSGDump,
		    servant->pSGList,servant->cSGList*sizeof(SCATGATENTRY));

	DEBMSG2(DBG_LEVEL1,dszVrioDump,(PVOID)vrio,sizeof(*vrio));

	if( vrio->iorb != NULL )
	{
	    PIORB_EXECUTEIO const	master = vrio->iorb;

	    DEBMSG2(DBG_LEVEL1,dszMasterIorb,(PVOID)master,master->iorbh.ErrorCode);
	    DEBMSG2(DBG_LEVEL1,dszMasterDump,(PVOID)master,master->iorbh.Length);
	    DEBMSG2(DBG_LEVEL1,dszSGDump,
		    (PVOID)master->pSGList,master->cSGList*sizeof(SCATGATENTRY));
	}
	if( servant->cSGList == 0 )
	    DBSTOP();
    }
#endif

    if( vrio == NULL )
    {
	USHORT	cnt = 0;

	DEBMSG2(DBG_ALL,dszINoteWakeup,(PVOID)iorb,
		(iorb->Status & IORB_ERROR ? iorb->ErrorCode : 0));
	DevHelp_ProcRun((ULONG)iorb, &cnt);	/* just wakeup task-time thread */
    }
    else
    {
	PVRDEVHDR const	vrd = vrio->vrd;

	ASSERT( vrd != NULL );
	if( (iorb->Status & IORB_ERROR) )
	    vrio->oserr = iorb->ErrorCode;
	else
	    vrio->oserr = 0;

	/* Don't need the IORB anymore: free it's memory. */
#if 1
	{
	    PVOID volatile p = iorb;
	    FreeIorb(p);
	}
#else
	/* WatCom only pushes offset and adjusts for 4 bytes afterwards. */
	FreeIorb((PVOID)iorb);
#endif
	DO( iorb = NULL );

	vrd->notify(vrio);
    }
    return NULL;
}




/*# ----------------------------------------------------------------------
 * PhysSendIORB(pdev,io,iorb)
 *
 * PARAMETER
 *	pdev		PPHYSDEVICE
 *	vrio		PVRIO (or 0: internal I/O)
 *	iorb		IORB to send to ADD
 *
 * RETURNS
 *	0		OK
 *
 * DESCRIPTION
 *	Initializes DMWorkSpace and other administrative fileds
 *	in IORB and calls ADD.
 *
 * REMARKS
 *	Only PUBLIC as we want to use this routine from initialization
 *	code (do not create any special code there).
 */
PRIVATE USHORT NEAR _Cdecl
PhysSendIORB(PPHYSDEVICE const pdev,PVRIO const vrio,PIORB const iorb)
{
    DEBMSG3(DBG_PATH,dszPhysSendIORB,pdev,vrio,iorb);
    iorb->NotifyAddress = IorbNotification;
    iorb->RequestControl |= IORB_ASYNC_POST;
    iorb->ErrorCode =	0;
    iorb->Status =	0;

    /* Initialize WorkSpace. */
    {
	((PVRAID_LOWER_DMDSPACE)iorb->DMWorkSpace)->self
	    = pdev;				/* link back to device */
	((PVRAID_LOWER_DMDSPACE)iorb->DMWorkSpace)->vrio
	    = vrio;				/* link to active VRIO */
#if defined(DEBUG)
	((PVRAID_LOWER_DMDSPACE)iorb->DMWorkSpace)->chk
	    = Chksum(iorb->DMWorkSpace, DM_WORKSPACE_SIZE-1);
#endif
    }

    CallADD(pdev->pADDEntry, iorb);

    return 0;
}




/*# ----------------------------------------------------------------------
 * BuildSGList(out,in,offset)
 *
 * PARAMETER
 *	out		output S/G list
 *	in		input S/G list
 *	offset		byte offset in 'in' to start at
 *
 * RETURNS
 *	0		OK
 *	/0		IOERR_CMD_SGLIST_BAD
 *
 * DESCRIPTION
 *	Take S/G list from 'in' and build a new list in 'out' starting
 *	'offset' * 512 bytes into 'in'.
 *	Length of I/O is out->BlockCount.
 *
 * REMARKS
 */
PRIVATE USHORT NEAR
BuildSGList(PIORB_EXECUTEIO const out,
	    USHORT const clist,PSCATGATENTRY const plist,USHORT const offset)
{
    /* start		byte offset in 'in' I/O 
     * cb		bytes to copy
     * copied		bytes in S/G already copied
     * current		current byte offset in I/O */

    ULONG const	start = (ULONG)offset * SECTOR_SIZE;
    ULONG const	cb = (ULONG)out->BlockCount * SECTOR_SIZE;
    ULONG	copied = 0;
    ULONG	current = 0;
    USHORT	i, rc = 0;

    DEBMSG2(DBG_LEVEL2,dszBSGL,(PVOID)plist,(PVOID)&start);
    DEBMSG2(DBG_LEVEL4,dszBSGLDump,(PVOID)plist,clist*sizeof(*plist));
    out->cSGList = 0;				/* no entry so far */

    /* Scan S/G list for first entry containing 'start' offset. */

    for( i = 0; i < clist; ++i )
    {
	ULONG const	ul = plist[i].XferBufLen;

	if( current+ul > start )		/* >= is wrong!  s.b. */
	    break;
	DEBMSG1(DBG_LEVEL3,dszBSGLSkip,(PVOID)&ul);
	current += ul;
    }

    /* Now copy entries from S/G list until we got enough bytes
     * to satisfy our 'out' list. */

    for(; copied != cb; ++i )
    {
#if defined(DEBUG)
	if( i >= clist )
	{

	    DEBMSG2(DBG_ALL,dszBSGLIndex,i,clist);
	    DEBMSG3(DBG_ALL,dszBSGLInput,(PVOID)plist,(PVOID)&start,(PVOID)&cb);
	    DEBMSG1(DBG_ALL,dszBSGLSGList,out->cSGList);
	    DEBMSG2(DBG_ALL,dszBSGLCurrent,(PVOID)&current,(PVOID)&copied);
	    DEBMSG2(DBG_ALL,dszBSGLDump,(PVOID)plist,clist*sizeof(*plist));
	    DBSTOP();
	    rc = IOERR_CMD_SGLIST_BAD;
	    break;
	}
#endif

	if( current < start )
	{
	    /* Only 1st loop if ever: begin of element may be skipped */

	    ASSERT( out->cSGList == 0 );
	    out->pSGList[out->cSGList].ppXferBuf
		= plist[i].ppXferBuf + start - current;
	}
	else
	{
	    ASSERT( out->cSGList == 0 ? current == start : 1 );
	    out->pSGList[out->cSGList].ppXferBuf = plist[i].ppXferBuf;
	}

	if( plist[i].XferBufLen + copied > out->BlockCount * (ULONG)SECTOR_SIZE )
	{
	    out->pSGList[out->cSGList].XferBufLen
		= out->BlockCount * (ULONG)SECTOR_SIZE - copied;
	}
	else
	{
	    out->pSGList[out->cSGList].XferBufLen = plist[i].XferBufLen;
	}
	DEBMSG2(DBG_LEVEL3,dszBSGLAdd,(PVOID)&out->pSGList[out->cSGList].XferBufLen,
		(PVOID)&out->pSGList[out->cSGList].ppXferBuf);

	copied += out->pSGList[out->cSGList].XferBufLen;
	current += plist[i].XferBufLen;
	++out->cSGList;
    }

    ASSERT( out->cSGList != 0 );
    return rc;
}




/*# ----------------------------------------------------------------------
 * PhysdevNotify(vrio)
 *
 * PARAMETER
 *	dummy		there is no child!
 *	io		from DMWorkSpace
 *
 * RETURNS
 *	(nothing)
 *
 * DESPRIPTION
 *	A device notifies it's PHYSDEV VRDEVICE of completion
 *	of an I/O.
 *	Process errors (?) and call parent.
 *
 * REMARKS
 *	This code is different from higher layers as it get passed it's
 *	own VRIO.  Don't cut-n-paste!!!
 */
PRIVATE VOID NEAR _Cdecl
PhysdevNotify(PVRIO const myio)
{
    DEBMSG1(DBG_PATH,dszPhysdevNotify,(PVOID)myio);

    ASSERT( myio->link == NULL );
    ASSERT( myio->bcnt != 0 );
    ASSERT( myio->vrd->size != 0 );
    ASSERT( myio->vrd->notify == PhysdevNotify );
    ASSERT( myio->vrd->worker == PhysdevWorker );


    /* Process errors!  (Nothing to do?) */

    CompleteVrio(myio);
    return;
}




/*# ----------------------------------------------------------------------
 * PhysdevWorker(self,myio)
 *
 * PARAMETER
 *	self		our single device
 *	myio		VRIO describing I/O
 *
 * RETURNS
 *	0		OK, I/O started
 *	/0		please queue I/O for later processing
 *
 * DESPRIPTION
 *	Build iorb from myio and pass it to the underlying device
 *	via PhysSendIORB().  Easy?
 *
 * REMARKS
 */
PRIVATE USHORT NEAR _Cdecl
PhysdevWorker(PVRDEVHDR const self,PVRIO const myio)
{
    PVRDPHYS const	physdev = (PVRDPHYS)self;
    PIORB_EXECUTEIO const iorb = AllocateIorb(myio->cSGList);
    USHORT		rc;


    DEBMSG2(DBG_PATH,dszPhysdevWorker,(PVOID)myio,(PVOID)iorb);

    ASSERT( myio->vrd == self );
    ASSERT( myio->link == NULL );
    ASSERT( myio->bcnt != 0 );
    ASSERT( physdev->header.size != 0 );
    ASSERT( physdev->header.notify == PhysdevNotify );
    ASSERT( physdev->header.worker == PhysdevWorker );
    ASSERT( physdev->header.update == PhysdevUpdate );

    ++self->iocnt;
    if( iorb == 0 )
    {
	DEBMSG(DBG_ALL,dszPWorkerNoAlloc);
	++self->busycnt;
	return !0;
    }


    /* Only things != 0 */

    iorb->iorbh.Length = sizeof(IORB_EXECUTEIO);
    iorb->iorbh.UnitHandle = physdev->phys->hdADDUnit;

    switch( myio->iocmd )
    {
      case IOCMD_READ:
	iorb->iorbh.CommandCode = IOCC_EXECUTE_IO;
	iorb->iorbh.CommandModifier = IOCM_READ;
	iorb->RBA = myio->blk + physdev->offset; /* add size of VRAID sectors */
	iorb->RBA += physdev->phys->ulVRStart;	/* add start of partition */
	break;

      case IOCMD_WRITE:
	iorb->iorbh.CommandCode = IOCC_EXECUTE_IO;
	iorb->iorbh.CommandModifier = IOCM_WRITE;
	iorb->RBA = myio->blk + physdev->offset; /* add size of VRAID sectors */
	iorb->RBA += physdev->phys->ulVRStart;	/* add start of partition */
	break;

      case IOCMD_AREAD:
	iorb->iorbh.CommandCode = IOCC_EXECUTE_IO;
	iorb->iorbh.CommandModifier = IOCM_READ;
	iorb->RBA = myio->blk + physdev->phys->ulVRStart; /* add start of partition*/
	break;

      case IOCMD_AWRITE:
	iorb->iorbh.CommandCode = IOCC_EXECUTE_IO;
	iorb->iorbh.CommandModifier = IOCM_WRITE;
	iorb->RBA = myio->blk + physdev->phys->ulVRStart; /* add start of partition*/
	break;

      default:
	DEBMSG1(DBG_ALL,dszPWorkerInvalid,myio->iocmd);
	DBSTOP();
	break;
    }

    iorb->BlockCount = myio->bcnt;
    iorb->BlockSize = SECTOR_SIZE;

    if( (rc = BuildSGList(iorb, myio->cSGList, myio->pSGList, myio->offset)) != 0 )
    {
	myio->oserr = rc;
	if( myio->mother != NULL )
	    myio->mother->vrd->notify(myio);
	else if( myio->iorb != NULL )
	    HostdriveNotify(myio);
	else
	{
	    USHORT	awoken;

	    DevHelp_ProcRun((ULONG)myio, &awoken);
	}
	return 0;
    }

    DEBMSG2(DBG_LEVEL3,dszPWorkerBlocks,iorb->BlockCount,(PVOID)&iorb->RBA);
    PhysSendIORB(physdev->phys, myio, &iorb->iorbh);
    return 0;
}




/*# ----------------------------------------------------------------------
 * PhysdevUpdate(self,sec)
 *
 * PARAMETER
 *	self		a VRD which is a PHYSDEV
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
PhysdevUpdate(PVRDEVHDR const self,PVOID const sec)
{
    return 0;
}






/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */


/*# ----------------------------------------------------------------------
 * PhysdevCreate(pVrh,pPhys,sec,pSize)
 *
 * PARAMETER
 *	pVrh		room to create device structure
 *	pPhys		physical device to create for
 *	sec		contents of configuration sector
 *	pSize		return: size used at '*pVrh'
 *
 * RETURNS
 *	0		OK
 *	/0		couldn't create
 *
 * DESCRIPTION
 *	Creates a VRAID device at location pointed to by 'pVrh' which
 *	relates to
 *	- the VRAID partition			-> admin
 *	- the data part of that partition	-> worker
 *
 * REMARKS
 *	-> admin should make 'PhysdevPartIO()' unneccessary.
 */
PUBLIC int NEAR _Cdecl
PhysdevCreate(PVRDEVHDR pVrh,PPHYSDEVICE pPhys,PSEC_PHYSDEV sec,PUSHORT pSize)
{
    PVRDPHYS physdev = (PVRDPHYS)pVrh;

    DEBMSG4(DBG_INIT1,dszPhysdevCreate,(PVOID)pVrh,(PVOID)pPhys,(PVOID)sec->u.s.id,6);
    _fmemset(physdev, 0, sizeof(*physdev));


    /* Initialize header elements */

    physdev->header.notify = PhysdevNotify;
    physdev->header.worker = PhysdevWorker;
    physdev->header.update = PhysdevUpdate;
    physdev->header.size = sec->u.s.size;
    physdev->header.flags = 0;
    _fmemcpy(physdev->header.id, sec->u.s.id, sizeof(physdev->header.id));
    DEBMSG2(DBG_INIT3,dszPCreateDump,(PVOID)sec,64);

    physdev->phys = pPhys;			/* linking */
    pPhys->parent = &physdev->header;

    physdev->offset = sec->u.s.adminspace;	/* reserved for VRAID */

    *pSize = sizeof(*physdev);
    return 0;
} /* end[PhysdevCreate] */
