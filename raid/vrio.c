/*
 * $Source: R:/source/driver/raid/RCS\\vrio.c,v $
 * $Revision: 1.15 $
 * $Date: 2001/07/03 06:52:28 $
 * $Locker:  $
 *
 *	Queue handling for VRIOs and IORBs.
 *
 * $Log: vrio.c,v $
 * Revision 1.15  2001/07/03 06:52:28  vitus
 * - StartManyIorb: complete test of command modifiers
 *
 * Revision 1.14  2001/02/11 01:38:01  vitus
 * - ProcessAdminIO, ProcessUserIO: have to return DRVSTATUS instead of
 *   IORB error. Implemented XlateIOERR() to do the translation.
 *
 * Revision 1.13  2000/08/22 00:21:57  vitus
 * - added debug code to alert when a ProcBlock has to be repeated (no impact
 *   on non-debug versions)
 *
 * Revision 1.12  2000/06/15 01:07:02  vitus
 * - implemented ProcessUserIO()
 *
 * Revision 1.11  1999/09/26 00:23:51  vitus
 * - defined all DBGMSG() strings as located in CONST segment
 *
 * Revision 1.10  1999/09/19 23:37:15  vitus
 * - changes to keep MS-C/C++ 7.0 happy
 *
 * Revision 1.9  1999/06/04 01:06:55  vitus
 * - changed DBG_INFO to DBG_LEVEL3
 *
 * Revision 1.8  1999/04/28 01:49:00  vitus
 * - implemented CompleteVrio()
 * - uses IOCMD_A* commands and Worker
 *
 * Revision 1.7  1999/02/09 00:11:07  vitus
 * - ProcessAdminIO() implemented
 *
 * Revision 1.6  1999/02/07 17:56:37  vitus
 * - changed for iorb -> vrio transition
 *
 * Revision 1.5  1998/09/20 14:21:44  vitus
 * - modified for AllocateVrio(working_dev,mother)
 *
 * Revision 1.4  1998/06/26 23:18:40  vitus
 * - public routines now _Cdecl
 * - StartManyIorb: don't allocate VRIO w/o IORB
 *
 * Revision 1.3  1998/05/29 02:03:27  vitus
 * - calls to DEBMSG macro
 *
 * Revision 1.2  1998/04/08 01:30:55  vitus
 * - StartManyIorb() handles some commands internally (dummies)
 *
 * Revision 1.1  1998/02/17 22:32:28  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2001
 */
#if defined(__WATCOMC__)
# pragma off (unreferenced)
#endif
static char const id[]="$Id: vrio.c,v 1.15 2001/07/03 06:52:28 vitus Exp $";
#if defined(__WATCOMC__)
# pragma on (unreferenced)
#endif

#include <string.h>

#define INCL_INITRP_ONLY
#include "os2intf.h"

#include "vraid.h"
#include "proto.h"
#include "extern.h"


/* Linked list of queued VRIOs. */

PRIVATE PVRIO	pVrioQueueHead = NULL;
PRIVATE PVRIO	pVrioQueueFoot = NULL;
PRIVATE ULONG	hlVrioQueue = 0;		/* lock handle for above */

#if defined(DEBUG)
CONSTMSG dszCompleteVrioParent[]=	"\r\n[CompleteVrio] calling parent %p, mother I/O %p";
CONSTMSG dszCompleteVrioHDrive[]=	"\r\n[CompleteVrio] iorb != 0 -> Host Drive";
CONSTMSG dszCompleteVrioIoctl[]=	"\r\n[CompleteVrio] nothing -> IOCtl";
CONSTMSG dszQueueVrio[]=	"\r\n[QueueVrio] %p";
CONSTMSG dszDequeueVrio[]=	"\r\n[DequeueVrio] %p";
CONSTMSG dszInsertVrio[]=	"\r\n[InsertVrio] %p";
CONSTMSG dszGetNextIorb[]=	"\r\n[GetNextIorb] %p";
CONSTMSG dszInsertIorb[]=	"\r\n[InsertIorb] %p";
CONSTMSG dszStartManyIorb[]=	"\r\n[StartManyIorb] entry";
CONSTMSG dszSMIorbAlloc[]=	"\r\n[StartManyIorb] couldn't allocate VRIO";
CONSTMSG dszSMIorbStart[]=	"\r\n[StartManyIorb] starting iorb %p";
CONSTMSG dszDump[]=		"\r\n%z";
CONSTMSG dszSMIorbVerify[]=	"\r\n[StartManyIorb] READ_VERIFY %w blocks from %lx";
CONSTMSG dszSMIorbPrefetch[]=	"\r\n[StartManyIorb] READ_PREFETCH %w blocks from %lx";
CONSTMSG dszSMIorbRead[]=	"\r\n[StartManyIorb] READ %w blocks from %lx";
CONSTMSG dszSMIorbWrite[]=	"\r\n[StartManyIorb] WRITE %w blocks to %lx";
CONSTMSG dszProcessAdminIo[]=	"\r\n[ProcessAdminIO] dev %p, sector %lx";
CONSTMSG dszProcessUserIo[]=	"\r\n[ProcessUserIO] dev %p, sector %lx";
CONSTMSG dszPAIoBlock[]=	"\r\n[ProcessAdminIO] ProcBlock - rc %w";
CONSTMSG dszPAIoBlkCnt[]=	"\r\n[ProcessAdmin/UserIO] used %w ProcBlock's";
#endif






/* **********************************************************************
 * **** Private Routines ************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * XlateIOERR(ioerr)
 *
 * PARAMETER
 *	ioerr		ADD error code
 *
 * RETURNS
 *	driver status word
 *
 * DESCRIPTION
 *	This translation is neccessary when we cross the bridge between
 *	IORB processing and strategy code (i.e. init or ioctl).
 *
 * REMARKS
 */
PRIVATE DRVSTATUS NEAR
XlateIOERR(USHORT const ioerr)
{
    DRVSTATUS result;

    if( ioerr == 0 )
	return 0;				/* fastest way back */
    switch( (ioerr & 0xFF00) )
    {
      case IOERR_CMD:
	result = STATUS_ERR_UNKCMD;
	break;

      case IOERR_UNIT:
	result = STATUS_ERR_NOTREADY;
	break;

      case IOERR_RBA:
	if( ioerr == IOERR_RBA_CRC_ERROR )
	    result = STATUS_ERR_CRC;
	else
	    result = STATUS_ERR_NOTFOUND;
	break;

      case IOERR_MEDIA:
	if( ioerr == IOERR_MEDIA_NOT_FORMATTED
	    ||  ioerr == IOERR_MEDIA_NOT_SUPPORTED )
	    result = STATUS_ERR_UNKMEDIA;
	else if( ioerr == IOERR_MEDIA_WRITE_PROTECT )
	    result = STATUS_ERR_WRFAULT;
	else
	    result = STATUS_ERR_CHGDISK;
	break;

      case IOERR_ADAPTER:
	result = STATUS_ERR_UNCERTAIN;
	break;

      case IOERR_DEVICE:
	result = STATUS_ERR_PAPEROUT;
	break;

      default:
	result = STATUS_ERR_GENERR;
	break;
    }

    return result;
}






/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * CompleteVrio(io)
 *
 * PARAMETER
 *	io		PVRIO
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 * REMARKS
 */
PUBLIC void NEAR _Cdecl
CompleteVrio(PVRIO const io)
{
    if( io->mother != NULL )
    {
	PVRDEVHDR const	parent = io->mother->vrd;

	DEBMSG2(DBG_LEVEL3,dszCompleteVrioParent,parent,io->mother);
	parent->notify(io);
	FreeVrio(io);				/* don't need it anymore */
    }
    else if( io->iorb != NULL )
    {
	DEBMSG(DBG_LEVEL3,dszCompleteVrioHDrive);
	HostdriveNotify(io);
	FreeVrio(io);				/* don't need it anymore */
    }
    else
    {
	USHORT	awoken;

	DEBMSG(DBG_LEVEL2,dszCompleteVrioIoctl);
	DevHelp_ProcRun((ULONG)io, &awoken);	/* *nicht* freigeben! */
    }
    return;
}





/* **********************************************************************
 * **** VRIO queuing/dequeuing ******************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * CALL
 *	QueueVrio(io)
 * PARAMETER
 *	io			PVRIO
 *	pVrioQueueHead, hlVrioQueue
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Adds 'io' to single linked list based at 'pVrioQueueHead'.
 *	Used if a VRDEVICE isn't able to process the VRIO.
 *	A timer should remove VRIOs from foot of this list and
 *	restart them.
 *
 * REMARKS
 *	SMP safe spinlocks are used to protect the queue.
 */
PUBLIC void NEAR _Cdecl
QueueVrio(PVRIO const io)
{
    DEBMSG1(DBG_QUEUE,dszQueueVrio,(PVOID)io);
    LOCK(hlVrioQueue);
    if( pVrioQueueHead == NULL )		/* empty queue? */
	pVrioQueueHead = io;			/* set as first */
    else
	pVrioQueueFoot->link = io;		/* set behind last */
    pVrioQueueFoot = io;			/* new last IORB */
    ++cVrioInQueue;
    UNLOCK(hlVrioQueue);
    return;
}




/*# ----------------------------------------------------------------------
 * CALL
 *	DequeueVrio(void)
 * PARAMETER
 *	(none)
 *	pVrioQueueHead, hlVrioQueue
 *
 * RETURNS
 *	PVRIO		VRIO from list
 *
 * DESCRIPTION
 *	Removes a VRIO from start of linked list based
 *	at 'pVrioQueueHead'.
 *	Used in timer routine to restart it.
 *
 * REMARKS
 *	SMP safe spinlocks are used to protect the queue.
 */
PUBLIC PVRIO NEAR _Cdecl
DequeueVrio(void)
{
    PVRIO	io;

    LOCK(hlVrioQueue);
    if( (io = pVrioQueueHead) != NULL )
    {
	if( (pVrioQueueHead = io->link) == NULL )
	    pVrioQueueFoot = NULL;		/* foot == head (now empty) */
	else
	    io->link = NULL;			/* remove link */
	++cVrioInQueue;
    }
    UNLOCK(hlVrioQueue);
    if( io != NULL )
    {
	DEBMSG1(DBG_QUEUE,dszDequeueVrio,(PVOID)io);
    }
    return io;
}




/*# ----------------------------------------------------------------------
 * CALL
 *	InsertVrio(io)
 * PARAMETER
 *	IO			PVRIO
 *	pVrioQueueHead, hlVrioQueue
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Adds a VRIO to the head of internal VRIO queue.
 *
 * REMARKS
 *	SMP safe spinlocks are used to protect the queue.
 *
 *	Should not be neccessary. But may if a VRDEVICE rejects this VRIO
 *	again?
 */
PUBLIC void NEAR _Cdecl
InsertVrio(PVRIO const io)
{
    DEBMSG1(DBG_QUEUE,dszInsertVrio,(PVOID)io);
    LOCK(hlVrioQueue);
    io->link = pVrioQueueHead;
    pVrioQueueHead = io;
    if( pVrioQueueFoot == NULL )		/* queue was empty? */
	pVrioQueueFoot = pVrioQueueHead;	/* Yes, -> last == first */
    ++cVrioInQueue;
    UNLOCK(hlVrioQueue);
    return;
}






/* **********************************************************************
 * **** IORB queuing/dequeuing ******************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * CALL
 *	GetNextIorb(drvp)
 * PARAMETER
 *	drvp		PHOSTDRIVE
 *	cIorbInQueue
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Get a new IORB from the driver queues.
 *	Requests are always removed from the queue head so this routine
 *	implements part of a FIFO. The other part is located in iorb.c
 *	(inside QueueIorbChain).
 *
 * REMARKS
 *	SMP safe spinlocks are used to protect the queue.
 */
PUBLIC PIORB NEAR _Cdecl
GetNextIorb(PHOSTDRIVE const drvp)
{
    PIORB pIorb;

    LOCK(drvp->hlQueue);
    if( (pIorb=drvp->pQueueHead) != NULL )
    {
	if( (drvp->pQueueHead=pIorb->pNxtIORB) == NULL )
	    drvp->pQueueFoot = NULL;		/* Foot == Head (now empty) */
	else
	    pIorb->pNxtIORB = NULL;		/* remove linkage (safety) */
	--cIorbInQueue;
    }
    UNLOCK(drvp->hlQueue);
    if( pIorb != NULL )
    {
	DEBMSG1(DBG_PATH,dszGetNextIorb,(PVOID)pIorb);
    }
    return pIorb;
}




/*# ----------------------------------------------------------------------
 * CALL
 *	InsertIorb(drvp,pIorb)
 * PARAMETER
 *	drvp			PHOSTDRIVE
 *	pIorb			I/O request structure
 *	cIorbInQueue
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Adds a lately retrieved OS request to the head of internal request
 *	queue.
 * REMARKS
 *	SMP safe spinlocks are used to protect the queue.
 *
 *	Should not be neccessary. But may if command not available and
 *	protocol free?
 */
PUBLIC void NEAR _Cdecl
InsertIorb(PHOSTDRIVE const drvp,PIORB const pIorb)
{
    DEBMSG1(DBG_QUEUE,dszInsertIorb,(PVOID)pIorb);
    LOCK(drvp->hlQueue);
    pIorb->pNxtIORB	= drvp->pQueueHead;
    drvp->pQueueHead	= pIorb;
    if( !drvp->pQueueFoot )			/* queue was empty? */
	drvp->pQueueFoot = drvp->pQueueHead;	/* Yes, -> last == first */
    ++cIorbInQueue;
    UNLOCK(drvp->hlQueue);
    return;
}




/*# ----------------------------------------------------------------------
 * StartManyIorb(drvp)
 *
 * PARAMETER
 *	drvp		PHOSTDRIVE
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Retrieve IORBs from 'drvp's queue, convert them to
 *	new allocated VRIOs and pass them to child VRDEVICE.  Do this
 *	until further processing is impossible.
 *
 * REMARKS
 */
PUBLIC void NEAR _Cdecl
StartManyIorb(PHOSTDRIVE const drvp)
{
    DEBMSG(DBG_LEVEL3,dszStartManyIorb);
    for(;;)
    {
	PIORB_EXECUTEIO const	iorb = (PVOID)GetNextIorb(drvp);
	PVRIO			vrio;
	USHORT			rc;

	if( iorb == NULL )
	    break;				/* No more work to do. */
	vrio = AllocateVrio(drvp->child, NULL);
	if( vrio == NULL )
	{
	    DEBMSG(DBG_ALL,dszSMIorbAlloc);
	    InsertIorb(drvp, &iorb->iorbh);	/* insert back to queue */
	    break;
	}

	DEBMSG1(DBG_LEVEL3,dszSMIorbStart,(PVOID)iorb);
	DEBMSG2(DBG_LEVEL4,dszDump,(PIORB)iorb,iorb->iorbh.Length);
	if( iorb->iorbh.CommandModifier == IOCM_READ_VERIFY )
	{
	    DEBMSG2(DBG_LEVEL3,dszSMIorbVerify,iorb->BlockCount,(PVOID)&iorb->RBA);
	    FreeVrio(vrio);
	    DO( vrio = NULL );
	    NotifyIorb(&iorb->iorbh, 0);
	    continue;				/* do next IORB */
	}
	else if( iorb->iorbh.CommandModifier == IOCM_READ_PREFETCH )
	{
	    DEBMSG2(DBG_LEVEL3,dszSMIorbPrefetch,iorb->BlockCount,(PVOID)&iorb->RBA);
	    FreeVrio(vrio);
	    DO( vrio = NULL );
	    NotifyIorb(&iorb->iorbh, 0);
	    continue;				/* do next IORB */
	}
	else if( iorb->iorbh.CommandModifier == IOCM_READ )
	{
	    DEBMSG2(DBG_LEVEL3,dszSMIorbRead,iorb->BlockCount,(PVOID)&iorb->RBA);
	    vrio->iocmd = IOCMD_READ;
	}
	else if( iorb->iorbh.CommandModifier == IOCM_WRITE )
	{
	    DEBMSG2(DBG_LEVEL3,dszSMIorbWrite,iorb->BlockCount,(PVOID)&iorb->RBA);
	    vrio->iocmd = IOCMD_WRITE;
	}
	else
	{
	    DBSTOP();
	    FreeVrio(vrio);
	    DO( vrio = NULL );
	    NotifyIorb(&iorb->iorbh, IOERR_CMD_NOT_SUPPORTED);
	    continue;
	}

	/* Translate IORB to VRIO */

	vrio->iorb = iorb;			/* we need this thing when reporting
						   completion */

	vrio->cSGList = iorb->cSGList;
	vrio->pSGList = iorb->pSGList;
	vrio->ppSGList = iorb->ppSGList;

	vrio->offset = 0;			/* relativ to S/G list */
	vrio->blk = iorb->RBA;
	vrio->bcnt = iorb->BlockCount;
	vrio->oserr = 0xFFFF;			/* invalid error code */

	/* Send VRIO to child. */

	rc = vrio->vrd->worker(vrio->vrd, vrio);
	if( rc )
	{
	    QueueVrio(vrio);
	    break;				/* no reason to continue */
	}
    }

    return;
}




/*# ----------------------------------------------------------------------
 * ProcessAdminIO(target,write,blk,bcnt,physaddr)
 *
 * PARAMETER
 *	target		device to address
 *	write		read (0) or write (/0)
 *	blk		sector no relative to partition start
 *	bcnt		sector count
 *	physaddr	pyhsical addresse to do I/O to,from
 *
 * RETURNS
 *	0		OK
 *
 * DESCRIPTION
 *	Reads/Writes into administrative area of an array.  Context
 *	has to allow blocking!
 *
 * REMARKS
 *	If the ProcBlock was interrupted (process died, etc.) it's better
 *	to keep all memory and waste space as to free it and produce
 *	system traps when the I/O complets.
 */
PUBLIC DRVSTATUS NEAR _Cdecl
ProcessAdminIO(PVRDEVHDR const target,BOOL write,
	       ULONG blk,USHORT bcnt,ULONG physaddr)
{
    PSCATGATENTRY scga = MemAlloc(sizeof(*scga));
    PVRIO const	vrio = AllocateVrio(target, NULL);
    DRVSTATUS	result = 0;
    USHORT	i;


    DEBMSG2(DBG_LEVEL1,dszProcessAdminIo,(PVOID)target,(PVOID)&blk);

    scga->ppXferBuf = physaddr;
    scga->XferBufLen = bcnt * (ULONG)SECTOR_SIZE;

    vrio->iocmd = (write ? IOCMD_AWRITE : IOCMD_AREAD);
    vrio->cSGList = 1;				/* continuous memory */
    vrio->pSGList = scga;
    vrio->ppSGList = FP_OFF(scga) + ppWorkMem;

    vrio->offset = 0;
    vrio->bcnt = bcnt;
    vrio->blk = blk;

    vrio->oserr = 0xFFFF;

    target->worker(target, vrio);

    cli();
    i = 0;
    while( vrio->oserr == 0xFFFF )
    {
	USHORT	us;

	us = DevHelp_ProcBlock((ULONG)vrio, (ULONG)-1L, WAIT_IS_INTERRUPTABLE);
	if( us == WAIT_INTERRUPTED )
	{
	    DEBMSG1(DBG_ALL,dszPAIoBlock,us);
	    result = STATUS_ERR_INTERRUPTED;
	    break;
	}
	cli();
	++i;
    }
    sti();
    if( i > 1 )
    {
	DEBMSG1(DBG_ALL,dszPAIoBlkCnt,i);
    }

    if( result == 0 )
    {
	result = XlateIOERR(vrio->oserr);	/* translate and before FreeVrio */

	FreeVrio(vrio);
	MemFree(scga);
    }

    return result;
}




/*# ----------------------------------------------------------------------
 * ProcessUserIO(target,write,blk,bcnt,physaddr)
 *
 * PARAMETER
 *	target		device to address
 *	write		read (0) or write (/0)
 *	blk		sector no in data area
 *	bcnt		sector count
 *	physaddr	pyhsical addresse to do I/O to,from
 *
 * RETURNS
 *	0		OK
 *	/0		error (driver status word)
 *
 * DESCRIPTION
 *	Reads/Writes into user area of an array.  Context
 *	has to allow blocking!
 *
 * REMARKS
 *	If the ProcBlock was interrupted (process died, etc.) it's better
 *	to keep all memory and waste space as to free it and produce
 *	system traps when the I/O complets.
 */
PUBLIC DRVSTATUS NEAR _Cdecl
ProcessUserIO(PVRDEVHDR const target,BOOL write,
	      ULONG blk,USHORT bcnt,ULONG physaddr)
{
    PSCATGATENTRY scga = MemAlloc(sizeof(*scga));
    PVRIO const	vrio = AllocateVrio(target, NULL);
    DRVSTATUS	result = 0;
    USHORT	i;


    DEBMSG2(DBG_LEVEL2,dszProcessUserIo,(PVOID)target,(PVOID)&blk);

    scga->ppXferBuf = physaddr;
    scga->XferBufLen = bcnt * (ULONG)SECTOR_SIZE;

    vrio->iocmd = (write ? IOCMD_AWRITE : IOCMD_AREAD);
    vrio->cSGList = 1;				/* continuous memory */
    vrio->pSGList = scga;
    vrio->ppSGList = FP_OFF(scga) + ppWorkMem;

    vrio->offset = 0;
    vrio->bcnt = bcnt;
    vrio->blk = VRAID_ADMINSECTORS + blk;

    vrio->oserr = 0xFFFF;

    target->worker(target, vrio);

    cli();
    i = 0;
    while( vrio->oserr == 0xFFFF )
    {
	USHORT	us;

	us = DevHelp_ProcBlock((ULONG)vrio, (ULONG)-1L, WAIT_IS_INTERRUPTABLE);
	if( us == WAIT_INTERRUPTED )
	{
	    DEBMSG1(DBG_ALL,dszPAIoBlock,us);
	    result = STATUS_ERR_INTERRUPTED;
	    break;
	}
	cli();
	++i;
    }
    sti();
    if( i > 1 )
    {
	DEBMSG1(DBG_ALL,dszPAIoBlkCnt,i);
    }

    if( result == 0 )
    {
	result = XlateIOERR(vrio->oserr);

	FreeVrio(vrio);
	MemFree(scga);
    }

    return result;
}
