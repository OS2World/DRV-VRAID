/*
 * $Source: r:/source/driver/sleep/RCS/iorb.c,v $
 * $Revision: 1.17 $
 * $Date: 2002/01/03 05:52:00 $
 * $Locker:  $
 *
 *	Request Time processing	- Timer processing
 *
 *	DDK says we have to saved SI,DI,DS,ES but all code I've seen
 *	only uses 'loadds' restoring only SI,DI,DS (and remember: CorelSCSI
 *	even changed DS!).  Nevertheless I will do a '_saveregs' now and
 *	save/restore all registers when calling other drivers...
 *
 * History: see bottom of file
 * ------------------------------------------------
 * This code is Copyright Vitus Jensen 1996-99,2001
 */
static char const id[]="$Id: iorb.c,v 1.17 2002/01/03 05:52:00 vitus Exp $";

#define INCL_NOBASEAPI
#define INCL_NOPMAPI
#define INCL_INITRP_ONLY
#include "dsksleep.h"
#include "apmcalls.h"

#include "dskslpub.h"
#include "proto.h"
#include "extern.h"





/* **********************************************************************
 * **** Debugging Data **************************************************
 * ******************************************************************* */
#if defined(DEBUG)
char _inconst dszQueueIorb[]=		"\r\n[QueueIorb] entry";
char _inconst dszYankQueue[]=		"\r\n[YankQueue] entry";
char _inconst dszStartComplete[]=	"\r\n[StartComplete] entry";
char _inconst dszStartUnitComplete[]=	"\r\n[StartComplete] START UNIT completed";
char _inconst dszStartUnitFailed[]=	"\r\nSTART UNIT failed, error %w";
char _inconst dszTestUnitComplete[]=	"\r\n[StartComplete] TEST UNIT READY completed";
char _inconst dszTestUnitFailed[]=	"\r\nTEST UNIT READY failed, error %w";
char _inconst dszStartExit[]=		"\r\n[StartComplete] exit(none)";
char _inconst dszIssueStart[]=		"\r\n[IssueStart] entry";
char _inconst dszIssueStartMem[]=	"\r\n[IssueStart] no memory";
char _inconst dszIssueStartExit[]=	"\r\n[IssueStart] exit(0)";
char _inconst dszUnknPwrstate[]=	"\r\nUnknown pwrstate %w";
char _inconst dszFilterFunction[]=	"\r\n[FilterFunction] entry";
char _inconst dszFF_Suspended[]=	"\r\n[FilterFunction] suspended! (ignored)";
char _inconst dszCC_Config[]=		"\r\nCC Configuration";
char _inconst dszCM_DevTab[]=		"\r\nCM Device Table";
char _inconst dszC[]=			"C";
char _inconst dszR[]=			"R";
char _inconst dszStopComplete[]=	"\r\n[StopComplete] entry";
char _inconst dszIssueStop[]=		"\r\n[IssueStop] entry";
char _inconst dszEventDump[]=		"\r\nAPMEvent: %z";
char _inconst dszAPMSuspend[]=		"\r\n[APMSuspend] entry";
char _inconst dszAPMResume[]=		"\r\n[APMResume] entry";
#endif




/* **********************************************************************
 * **** Allocation of per device IORB ***********************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * AllocateIorb(device)
 *
 * PARAMETER
 *	device		IORB for this device
 * RETURNS
 *	/0		allocated IORB
 *	0		no IORB available
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Simple, as there is only a single IORB to allocate from.
 * REMARKS
 */
#if defined(_MSC_VER)
# pragma optimize("lge",off)
#endif
PRIVATE NPVOID NEAR
AllocateIorb(NPDEVICE const device)
{
    NPVOID np = 0;

    pushf();
    cli();
    if( device->iorb_busy == 0 )
    {
	device->iorb_busy = 1;
	np = &device->iorb;
    }
    popf();
    return np;
}
#if defined(_MSC_VER)
# pragma optimize("",on)
#endif




/*# ----------------------------------------------------------------------
 * FreeIorb(device)
 *
 * PARAMETER
 *	device		device to stop
 * RETURNS
 *	nothing
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Frees previously allocated IORB.
 * REMARKS
 */
PRIVATE void NEAR
FreeIorb(NPDEVICE device)
{
    device->iorb_busy = 0;
    return;
}




/*# ----------------------------------------------------------------------
 * NotifyIorb(pIorbh,ErrorCode)
 *
 * PARAMETER
 *	pIorbh			I/O request buffer
 *	ErrorCode		see DDK
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Return IORB to OS, setting errorcode and flags. 'pIorbh' may
 *	be head of linked list or single request.
 *
 * REMARKS
 */
PRIVATE VOID NEAR _Cdecl
NotifyIorb(PIORBH const pIorbh, USHORT ErrorCode)
{
    PIORB	p;

    /* Set IORB_ERROR in pIorb->Status if there is a non-zero
     * error code AND the RECOVERED ERROR bit is not set in
     * the IORB status field. */

    for( p = pIorbh; p != NULL;
	 p = (p->RequestControl & IORB_CHAIN ? p->pNxtIORB : NULL) )
    {
	p->ErrorCode = ErrorCode;
	p->Status |= (IORB_DONE | ((ErrorCode && !(p->Status & IORB_RECOV_ERROR))
				   ? IORB_ERROR : 0));
	if( (p->RequestControl & IORB_ASYNC_POST) )
	    CallADD(p->NotifyAddress, p);
    }
    return;
}






/* **********************************************************************
 * **** Code to queue/dequeue IORBs *************************************
 * ******************************************************************* */

typedef union {
    PIORB	next;
    UCHAR	dummy[ADD_WORKSPACE_SIZE];
} ADDWS, FAR * PADDWS;



/*# ----------------------------------------------------------------------
 * QueueIorb(device, iorb)
 *
 * PARAMETER
 *	device		stopped device
 *	iorb		IORB or IORB chain from DMD
 * RETURNS
 *	(nothing)
 * GLOBAL
 *	(none)
 * PURPOSE
 *	Adds 'iorb' to per device queue of 'to-be-restarted'
 *	IORB (chains).
 * REMARKS
 *	Don't use 'iorb->pNxtIORB' (designed to let ADD build
 *	IORB chains) as we would have to seperate IORB types
 *	when yanking the queue to the ADD (see DDK inf).
 */
#if defined(_MSC_VER)
# pragma optimize("lge",off)
#endif
PRIVATE void NEAR
QueueIorb(NPDEVICE const device,PIORB const iorb)
{
    DEBMSG(dszQueueIorb);
    _fmemset(iorb->ADDWorkSpace, 0, ADD_WORKSPACE_SIZE);

    pushf();
    cli();					/* do not disturb */
    if( device->pQueueHead == NULL )		/* empty queue? */
	device->pQueueHead = iorb;
    else
    {
	PADDWS const ws = (PADDWS)device->pQueueFoot->ADDWorkSpace;
	ws->next = iorb;
    }
    device->pQueueFoot = iorb;
    popf();
    return;
}
#if defined(_MSC_VER)
# pragma optimize("",on)
#endif




/*# ----------------------------------------------------------------------
 * YankQueue(device)
 *
 * PARAMETER
 *	device		our device structure
 * RETURNS
 *	(nothing)
 * GLOBAL
 *	(none)
 * PURPOSE
 *	Passes all saved IORBs for this device to it's ADD.
 * REMARKS
 *	Only pass a single IORB at a time if not IOCC_EXECUTE_IO
 *	CommandCode (see DDK inf).
 *	For safety: clear used bytes in ADDWorkSpace.
 */
#if defined(_MSC_VER)
# pragma optimize("lge",off)
#endif
PRIVATE void NEAR
YankQueue(NPDEVICE const device)
{
    PIORB iorb;

    DEBMSG(dszYankQueue);
    pushf();
    cli();
    while( (iorb=device->pQueueHead) != NULL )
    {
	PADDWS const	ws = (PADDWS)iorb->ADDWorkSpace;
	PIORB		p;

	/* Set 'pQueueHead' to next IORB to remove an IORB from
	 * our queue and clear anything left in ADDWorkSpace. */

	if( (device->pQueueHead=ws->next) == NULL )
	    device->pQueueFoot = NULL;
	ws->next = NULL;

	/* Make sure the correct unit handle is set (in all IORB,
	 * may be a linked list) and call ADD. */

	for( p = iorb; p != NULL;
	     p = (p->RequestControl & IORB_CHAIN ? p->pNxtIORB : NULL) )
	    p->UnitHandle = device->hdADDUnit;
	device->counter = 0;			/* reset counter */

	CallADD(device->pADDEntry, iorb);

	cli();				/* ADD may have changed */
    }
    popf();

    return;
}
#if defined(_MSC_VER)
# pragma optimize("",on)
#endif






/* **********************************************************************
 * **** Task Time Processing (filter) ***********************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * APMSuspend(void)
 *
 * PARAMETER
 *	(none)
 *
 * RETURNS
 *	0		always
 *
 * DESCRIPTION
 *	Called whenever the system enters suspend state.  It checks
 *	all disks and stops any left in running state.
 *	And sets 'fAPMSuspended'.
 *
 * REMARKS
 */
PRIVATE USHORT
APMSuspend(void)
{
    DEBMSG(dszAPMSuspend);
    fAPMSuspended = 1;				/* when suspended, refuse any IORBs*/
#if 0
    {
	USHORT	i;

	for( i = 0; i < cUnit; ++i )
	{
	    if( anpUnit[i] != 0  &&  !anpUnit[i]->stopped )
	    {
		IssueStop(anpUnit[i]);
	    }
	}
    }
#endif

    return 0;
}




/*# ----------------------------------------------------------------------
 * APMResume(void)
 *
 * PARAMETER
 *	(none)
 *
 * RETURNS
 *	0		always
 *
 * DESCRIPTION
 *	Clears 'fAPMSuspended'.
 *
 * REMARKS
 *	We could do a lot more (restart all disks) but why do it
 *	now?  If OS/2 accesses a disk it will be restarted anyway
 *	and if it does not the disk doesn't have to do a start/stop
 *	cycle.
 *
 */
PRIVATE USHORT
APMResume(void)
{
    DEBMSG(dszAPMResume);

    fAPMSuspended = 0;                  /* OK, process IORB's */
    return 0;
}




/*# ----------------------------------------------------------------------
 * StartComplete(iorb)
 *
 * PARAMETER
 *	iorb		request completed
 * RETURNS
 *	(nothing)
 * GLOBAL
 *	(none)
 *
 * DESPRIPTION
 *	Called when the ADD completed a START UNIT or TEST UNIT READY
 *	command. Updates flags and unblocks waiting threads.
 *	Any unblocking/clearing of flags will be done *after* TEST UNIT
 *	READY verified a correct working drive!
 *
 * REMARKS
 *	Do retries make any sense?
 */
#if defined(_MSC_VER)
# pragma optimize("lge",off)
#endif
PRIVATE VOID FAR /*_saveregs*/ _loadds _Cdecl
StartComplete(PIORB iorb)
{
    NPDEVICE const		device = *(NPDEVICE FAR *)iorb->DMWorkSpace;
    PIORB_ADAPTER_PASSTHRU const ioadp = (PIORB_ADAPTER_PASSTHRU)iorb;


    DEBMSG(dszStartComplete);
    if( ioadp->pControllerCmd[0] == 0x1B )	/* START UNIT ? */
    {
	DEBMSG(dszStartUnitComplete);

	device->iotype = DSKIO_START;
	device->laststatus = ioadp->iorbh.Status;
	device->lasterror = ioadp->iorbh.ErrorCode;

	if( (ioadp->iorbh.Status & IORB_ERROR) )
	{
	    DEBMSG1(dszStartUnitFailed,ioadp->iorbh.ErrorCode);
	}

	/* Well, target may be in a UNIT ATTENTION state.  Issue
	 * TEST UNIT READY (plus REQUEST SENSE) to clear that state. */

	memset(&device->sensedata, 0, sizeof(SCSI_REQSENSE_DATA));
	memset(&device->statusblock, 0, sizeof(SCSI_STATUS_BLOCK));
	device->statusblock.ReqSenseLen = sizeof(SCSI_REQSENSE_DATA);
	device->statusblock.SenseData = &device->sensedata;

	_fmemset(ioadp, 0, MAX_IORB_SIZE);
	ioadp->iorbh.Length =		sizeof(IORB_ADAPTER_PASSTHRU);
	ioadp->iorbh.UnitHandle =	device->hdADDUnit;
	ioadp->iorbh.CommandCode =	IOCC_ADAPTER_PASSTHRU;
	ioadp->iorbh.CommandModifier =	IOCM_EXECUTE_CDB;
	ioadp->iorbh.RequestControl =	IORB_ASYNC_POST | IORB_REQ_STATUSBLOCK;
	ioadp->iorbh.NotifyAddress =	(PVOID)StartComplete;

	ioadp->iorbh.StatusBlockLen =	sizeof(SCSI_STATUS_BLOCK);
	ioadp->iorbh.pStatusBlock =	(NPBYTE)&device->statusblock;

	ioadp->pControllerCmd =		device->cdbTestReady;
	ioadp->ControllerCmdLen =	6;

	*(NPDEVICE FAR *)ioadp->iorbh.DMWorkSpace = device;

	{
	    PVOID volatile	p = &ioadp->iorbh; /* WatCom error: force far ptr */

	    CallADD(device->pADDEntry, p);
	}
	/* Will return to this routine after
	 * TEST UNIT READY (else case) */

    } /* end[START STOP UNIT] */
    else					/* TEST UNIT READY */
    {
	DEBMSG(dszTestUnitComplete);

	device->iotype = DSKIO_TESTREADY;
	device->laststatus = ioadp->iorbh.Status;
	device->lasterror = ioadp->iorbh.ErrorCode;

	if( (ioadp->iorbh.Status & IORB_ERROR) )
	{
	    DEBMSG1(dszTestUnitFailed,ioadp->iorbh.ErrorCode);

	    /* Bad!  What to do?  HELP!!!
	     * We could at least notify the user if he's there... */

	    DevHelp_Beep(440, 500);
	    DevHelp_Beep(300, 1000);

	    FreeIorb(device);			/* So IssueStart may allocate */
	    if( (fDriverFlags & DF_INITDONE) )	/* no retries during /DEBUG */
	    {
		/* There is something more we can do: retry.
		 * If this isn't possible, clear 'blocked' flag
		 * and let next I/O call 'IssueStart'. */

		if( IssueStart(device) )
		    device->blocked = 0;
	    }
	    else
		device->blocked = 0;		/* keep on running */
	}
	else
	{
	    /* TEST UNIT READY completed without errors,
	     * clear 'stopped' flag and reset 'counter' to zero.
	     * Clear flags after 'FreeIorb()', a timer interrupt
	     * may occure during that function. */

	    device->counter = 0;
	    device->stopped = 0;
	    FreeIorb(device);
	    device->blocked = 0;
	    YankQueue(device);			/* restart any queued I/Os */
	}
    } /* end[TEST UNIT READY] */

    DEBMSG(dszStartExit);
    return;
}
#if defined(_MSC_VER)
# pragma optimize("",on)
#endif




/*# ----------------------------------------------------------------------
 * IssueStart(device)
 *
 * PARAMETER
 *	device		device to start
 * RETURNS
 *	0		START UNIT issued
 *	/0		error, cmd not started
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Builds START STOP UNIT cdb with start bit set
 *	in newly allocated IORB and passes this IORB
 *	to ADD for 'device'.
 *	Set 'blocked' even if already set (init1.c).
 *
 * REMARKS
 *	Uses 'StartComplete' as notification routine.
 */
#if defined(_MSC_VER)
# pragma optimize("lge",off)
#endif
PUBLIC int NEAR
IssueStart(NPDEVICE const device)
{
    NPIORB_ADAPTER_PASSTHRU ioadp = AllocateIorb(device);

    DEBMSG(dszIssueStart);
    if( ioadp == NULL )
    {
	DEBMSG(dszIssueStartMem);
	return -1;				/* not possible */
    }

    device->blocked = 1;			/* please, no I/Os */

    memset(&device->sensedata, 0, sizeof(SCSI_REQSENSE_DATA));
    memset(&device->statusblock, 0, sizeof(SCSI_STATUS_BLOCK));
    device->statusblock.ReqSenseLen = sizeof(SCSI_REQSENSE_DATA);
    device->statusblock.SenseData = &device->sensedata;

    memset(ioadp, 0, MAX_IORB_SIZE);
    ioadp->iorbh.Length =		sizeof(IORB_ADAPTER_PASSTHRU);
    ioadp->iorbh.UnitHandle =		device->hdADDUnit;
    ioadp->iorbh.CommandCode =		IOCC_ADAPTER_PASSTHRU;
    ioadp->iorbh.CommandModifier =	IOCM_EXECUTE_CDB;
    ioadp->iorbh.Timeout =		START_TIMEOUT;
    ioadp->iorbh.RequestControl =	IORB_ASYNC_POST | IORB_REQ_STATUSBLOCK;
    ioadp->iorbh.NotifyAddress =	(PVOID)StartComplete;

    ioadp->iorbh.StatusBlockLen =	sizeof(SCSI_STATUS_BLOCK);
    ioadp->iorbh.pStatusBlock =		(NPBYTE)&device->statusblock;

    ioadp->pControllerCmd =		device->cdbStart;
    ioadp->ControllerCmdLen =		6;

    /* To reconstruct which unit this start command has
     * been issued for save pointer in request.  The DM
     * workspace will never be used by an ADD and is
     * therefore free (there is no DMD either...) */

    *(NPDEVICE FAR *)ioadp->iorbh.DMWorkSpace = device;

    CallADD(device->pADDEntry, &ioadp->iorbh);

    DEBMSG(dszIssueStartExit);
    return 0;
}
#if defined(_MSC_VER)
# pragma optimize("",on)
#endif




/*# ----------------------------------------------------------------------
 * APMEventHandler(event)
 *
 * PARAMETER
 *	event
 *
 * RETURNS
 *	0
 *
 * DESCRIPTION
 * REMARKS
 */
PUBLIC USHORT FAR _saveregs _Cdecl
APMEventHandler(PAPMEVENT event)
{
    USHORT const SubId = LOUSHORT(event->ulParm1);


    DEBMSG2(dszEventDump,, (PVOID)event, sizeof(*event));
    if( SubId == APM_SETPWRSTATEEVENT )
    {
	USHORT const pwrstate = HIUSHORT(event->ulParm2);

	switch( pwrstate )
	{
	  case APM_PWRSTATESUSPEND:
	    return APMSuspend();

	  case APM_PWRSTATEREADY:
	    return APMResume();

	  default:
	    DEBMSG1(dszUnknPwrstate,pwrstate);
	    break;
	}
    }
    else if( SubId == APM_NORMRESUMEEVENT
	     ||  SubId == APM_CRITRESUMEEVENT
	     /*||  SubId == APM_STANDBYRESUMEEVENT*/ )
    {
        return APMResume();
    }
    return 0;
}




/*# ----------------------------------------------------------------------
 * FilterFunction(iorb)
 *
 * PARAMETER
 *	iorb		request to anylyse
 * RETURNS
 *	nothing
 * GLOBAL
 *	anpUnit
 *
 * DESPRIPTION
 *	Besides returning our own device table (there is none) this
 *	routine checks whether the addressed device is still running
 *	and forwards the request to ADD if it is.
 *	A stopped device is passed to 'IssueStart' and forwarding waits
 *	until the START completed.  To prevent I/Os during START or
 *	STOP the routine checks 'blocked' flag first.
 *
 * REMARKS
 */
#if defined(_MSC_VER)
# pragma optimize("lge",off)
#endif
PUBLIC void FAR _saveregs _loadds _Cdecl
FilterFunction(PIORB const iorb)
{
    NPDEVICE	device = 0;


    /*DEBMSG(dszFilterFunction);*/
#if 1
    if( fAPMSuspended )
    {
	DevHelp_Beep(220, 2000);
	DEBMSG(dszFF_Suspended);
# if 0
	NotifyIorb(iorb, IOERR_UNIT_PWR_OFF);
	return;
# endif
    }
#endif

    /* Handle the Get Device Table call.  Since we are just filtering
     * requests, return a table with no unit info. */

    if( iorb->CommandCode == IOCC_CONFIGURATION )
    {
	PVOID	p = iorb;

	DEBMSG(dszCC_Config);
	switch( iorb->CommandModifier )
	{
	  case IOCM_GET_DEVICE_TABLE:
	    DEBMSG(dszCM_DevTab);
	    ((PIORB_CONFIGURATION)iorb)->pDeviceTable->ADDLevelMajor = 1;
	    ((PIORB_CONFIGURATION)iorb)->pDeviceTable->ADDLevelMinor = 0;
	    ((PIORB_CONFIGURATION)iorb)->pDeviceTable->ADDHandle = hdThisDriver;
	    ((PIORB_CONFIGURATION)iorb)->pDeviceTable->TotalAdapters = 0;
	    NotifyIorb(iorb, 0);
	    break;

	  default:
	    NotifyIorb(iorb, IOERR_CMD_NOT_SUPPORTED);
	    break;
	}
	return;
    }


    /* iorb->UnitHandle should be a valid pointer in
     * our global table, save in 'device'.  If not -> tell caller. */

    {
	int	i;

	for( i = 0; i < MAX_UNITS; ++i )
	{
	    if( iorb->UnitHandle == (USHORT)anpUnit[i] )
	    {
		device = (NPDEVICE)iorb->UnitHandle; /* device structure */
		break;
	    }
	    else if( anpUnit[i] == 0 )		/* end of used range */
		break;
	} /* end[for()] */
    }
    if( device == 0 )				/* not found? */
    {
	NotifyIorb(iorb, IOERR_UNIT_NOT_ALLOCATED);
	return;
    }


    /* First, there may be a start or stop pending (setting
     * blocked flag):  add this IORB(s) to internal chain and return. */

    pushf();
    cli();					/* needed? */
    if( device->blocked )
    {
	QueueIorb(device, iorb);		/* StartComplete restarts it */
	popf();
	return;					/* return to caller */
    }


    /* Second, this device may be stopped and has
     * to be started again.
     * Only start device if this is a request accessing the medium. */

    {
	int	restart;
	PIORB	p;

	switch( iorb->CommandCode )
	{
	  case IOCC_UNIT_CONTROL:
	  case IOCC_GEOMETRY:
	    restart = 0;			/* no need to restart */
	    break;

	  default:
	    restart = 1;			/* better restart */
	    break;
	}

	if( restart  &&  device->stopped )	/* interrupts still disabled */
	{
	    device->blocked = 1;		/* no other I/Os */
	    _fmemcpy(device->rst_reason, iorb, iorb->Length);
	    QueueIorb(device, iorb);		/* StartComplete restarts it */
	    IssueStart(device);			/* now START! */
	    popf();
	    return;				/* return to caller */
	}


	/* We are about to call the ADD for this request [chain].
	 * Replace unit handle of filter with unit handle
	 * of ADD (in all IORBs!). */

	DEBMSG(dszC);
	for( p = iorb; p != NULL;
	     p = (p->RequestControl & IORB_CHAIN ? p->pNxtIORB : NULL) )
	    p->UnitHandle = device->hdADDUnit;

	/* Reset spindown timeout, but do this only if it's an important
	 * command (one which would restart the disk. */

	if( restart )
	    device->counter = 0;
    }

    /* Call the ADD. */

    {
	PVOID volatile	p = iorb;		/* s.a. */

	CallADD(device->pADDEntry, p);
    }
    DEBMSG(dszR);
    popf();
    return;
}
#if defined(_MSC_VER)
# pragma optimize("",on)
#endif






/* **********************************************************************
 * **** Timer Context Processing ****************************************
 * ******************************************************************* */


/*#
 * StopComplete(iorb)
 *
 * PARAMETER
 *	iorb		STOP IORB completed
 * RETURNS
 *	nothing
 * GLOBAL
 *	none
 * DESPRIPTION
 *	see 'StartComplete'
 * REMARKS
 */
PRIVATE void FAR _saveregs _loadds _Cdecl
StopComplete(PIORB iorb)
{
    NPDEVICE const device = *(NPDEVICE FAR *)iorb->DMWorkSpace;

    DEBMSG(dszStopComplete);

    device->iotype = DSKIO_STOP;
    device->laststatus = iorb->Status;
    device->lasterror = iorb->ErrorCode;

    FreeIorb(device);				/* so START can use it */
    device->blocked = 0;			/* OK, do want you want */
    return;
}




/*# -----------------------------------------------------------------------
 * IssueStop(device)
 *
 * PARAMETER
 *	device		to stop
 * RETURNS
 *	nothing
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Allocates IORB, fills in START STOP UNIT cdb with
 *	start bit cleared and passes it to ADD.
 * REMARKS
 *	Interrupts disabled!
 */
#if defined(_MSC_VER)
# pragma optimize("lge",off)
#endif
PUBLIC void NEAR
IssueStop(NPDEVICE const device)
{
    NPIORB_ADAPTER_PASSTHRU const ioadp = AllocateIorb(device);


    DEBMSG(dszIssueStop);

    device->counter = device->usSleep;			/* for IOCtls... */
    device->rst_reason[0] = device->rst_reason[1] = 0; 	/* clear length field */


    /* The IORB from unit structure may not be available.
     * STOP while STARTing? */

    if( ioadp != NULL )
    {
	device->stopped = 1;
	device->blocked = 1;			/* please, no I/Os */

	memset(&device->sensedata, 0, sizeof(SCSI_REQSENSE_DATA));
	memset(&device->statusblock, 0, sizeof(SCSI_STATUS_BLOCK));
	device->statusblock.ReqSenseLen = sizeof(SCSI_REQSENSE_DATA);
	device->statusblock.SenseData = &device->sensedata;

	memset(ioadp, 0, MAX_IORB_SIZE);
	ioadp->iorbh.Length =		sizeof(IORB_ADAPTER_PASSTHRU);
	ioadp->iorbh.UnitHandle =	device->hdADDUnit;
	ioadp->iorbh.CommandCode =	IOCC_ADAPTER_PASSTHRU;
	ioadp->iorbh.CommandModifier =	IOCM_EXECUTE_CDB;
	ioadp->iorbh.RequestControl =	IORB_ASYNC_POST | IORB_REQ_STATUSBLOCK;
	ioadp->iorbh.NotifyAddress =	(PVOID)StopComplete;

	ioadp->iorbh.StatusBlockLen =	sizeof(SCSI_STATUS_BLOCK);
	ioadp->iorbh.pStatusBlock =	(NPBYTE)&device->statusblock;

	ioadp->pControllerCmd =		device->cdbStop;
	ioadp->ControllerCmdLen =	6;

	/* To reconstruct which unit this stop command has
	 * been issued for save pointer in request.  The DM
	 * workspace will never be used by an ADD and is
	 * therefore free (there is no DMD either...) */

	*(NPDEVICE FAR *)ioadp->iorbh.DMWorkSpace = device;

	CallADD(device->pADDEntry, &ioadp->iorbh);
    }

    return;
}
#if defined(_MSC_VER)
# pragma optimize("",on)
#endif




/*# ----------------------------------------------------------------------
 * Timer(void)
 *
 * PARAMETER
 *	none
 * RETURNS
 *	nothing
 * GLOBAL
 *	ulTimer
 *	anpUnit
 * DESPRIPTION
 *	Increments 'counter' in all unit structures
 *	and sends stop if 'counter' reached limit.
 * REMARKS
 *	Called by assembler stub which saves/restores registers
 *	but doesn't enable interrupts.
 */
#if defined(_MSC_VER)
# pragma optimize("lge",off)
#endif
PUBLIC void NEAR _Cdecl
Timer(void)
{
    USHORT i;

    ++ulTimer;					/* free running counter */

    /* Detect unused device */

    for( i = 0; i < cUnit; ++i )
    {
	NPDEVICE const dev = anpUnit[i];

	cli();
	if(  !dev->stopped  &&  dev->usSleep != 0
	   &&  ++dev->counter >= dev->usSleep )
	{
	    IssueStop(dev);
	}
	sti();
    }

    return;
}
#if defined(_MSC_VER)
# pragma optimize("",on)
#endif



/* History:
 *
 * $Log: iorb.c,v $
 * Revision 1.17  2002/01/03 05:52:00  vitus
 * - notification seperated in function NotifyIorb()
 * - new DDK, new APM_* constants
 * - more debugging code
 *
 * Revision 1.16  2001/02/27 22:36:36  vitus
 * - coded all debugging messages as _inconst
 * - added APMEventHandler: stopps all disks on suspend
 *
 * Revision 1.15  1999/11/29 00:39:52  vitus
 * - moved all system includes to dsksleep.h
 *
 * Revision 1.14  1999/09/17 08:00:46  vitus
 * - all DEBMSG() calls now use strings located in CONST segment
 * - StartComplete: _saveregs removed (?)
 *
 * Revision 1.13  1998/10/14 00:30:32  vitus
 * - swiched from SAVE_IF,RESTORE_IF, etc. to cli,st,pushf,popf
 * - uses CallADD()
 * - saves reason for restart in DEVICE structure
 *
 * Revision 1.12  1997/12/05 01:37:12  vitus
 * - added WatCom specific code and header
 * - declared all exported and imported functions as _Cdecl
 *
 * Revision 1.11  1997/10/26 00:32:08  vitus
 * - modified for UnitHandle = pointer
 *
 * Revision 1.10  1997/10/11 17:32:41  vitus
 * - FilterFunction,YankQueue: set UnitHandle of *all* IORBs in linked list
 *   to ADDs value
 * - clear IORB with MAX_IORB_SIZE bytes, as most DMD do
 *
 * Revision 1.9  1997/08/23 22:26:24  vitus
 * - saves status and sense values in device structure (IOCtl retrieves)
 * - IssueStart: now used by INIT code, too
 *
 * Revision 1.8  1997/06/18 00:54:29  vitus
 * - preserve registers when calling other drivers
 *   or being called from others
 * - use SAVE_IF,RESTORE_IF to restore interrupt flag
 *
 * Revision 1.7  1997/05/11 01:57:44  vitus
 * - switched from Block/Run to IORB queues (aka HPFS386 problem)
 * - added debugging messages
 *
 * Revision 1.6  1997/04/27 22:30:34  vitus
 * Added retries if START+TEST failed
 * StartComplete: beeps if TEST failed
 * StartComplete: flag updates after FreeIorb
 *
 * Revision 1.5  1997/03/03 23:41:27  vitus
 * StartComplete: issue TEST UNIT READY after START
 * StartComplete: only successfull TEST UNIT READY resets flags
 *
 * Revision 1.4  1996/11/03 23:39:57  vitus
 * Use timeout from device structure instead of global
 *
 * Revision 1.3  1996/10/23 23:41:14  vitus
 * Added pragma to display MS-C warnings
 * Allows retries when stopping a device
 *
 * Revision 1.2  1996/09/30 00:27:56  vitus
 * Removed indirection from pointer calls
 *
 * Revision 1.1  1996/09/27 03:19:52  vitus
 * Initial revision
 */
