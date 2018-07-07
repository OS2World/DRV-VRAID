/*
 * $Source: R:/source/driver/sleep/RCS/init1.c,v $
 * $Revision: 1.24 $
 * $Date: 2001/02/27 02:24:47 $
 * $Locker:  $
 *
 *	First step of initialisation.
 *
 * History: see bottom of file
 * ------------------------------------------------
 * This code is Copyright Vitus Jensen 1996-99,2001
 */
static char const id[]="$Id: init1.c,v 1.24 2001/02/27 02:24:47 vitus Exp $";

#define INCL_DOSERRORS
#define INCL_NOPMAPI
#define INCL_INITRP_ONLY
#include "dsksleep.h"
#include "cmdphdr.h"
#include "dskslpub.h"
#include "proto.h"
#include "extern.h"



/*
 * Macro to allocate data from memory pool
 */
#define GETINITRAM(p,type,size) (p=(type)npInitData,npInitData+=(size))

#define INITMSG		static char _ininit




/* **********************************************************************
 * **** Debugging Data **************************************************
 * ******************************************************************* */
#if defined(DEBUG)
char _inconst	dszRegister[]=		"\r\nRegistering device class";
char _inconst	dszRemoveFilter[]=	"\r\nRemoveFilter";
char _inconst	dszInstFilter[]=	"\r\nInstallFilter %w";
char _inconst	dszInstFilterEnd[]=	"\r\nInstallFilter ends";
char _inconst	dszInstallTimer[]=	"\r\nInstalling timer";
char _inconst	dszInitBase[]=		"\r\nInitBase";
char _inconst	dszScanDevices[]=	"\r\nScanDevices";
char _inconst	dszAddScan[]=		"\r\nLooking for ADDs";
char _inconst	dszAdpScan[]=		"\r\nLooking for Adapters";
char _inconst	dszUnitScan[]=		"\r\nLooking for Units";
char _inconst	dszOldFilter[]=		"\r\nFound other filter for unit";
char _inconst	dszInstalled[]=		"\r\nDriver installed";
char _inconst	dszNotInstalled[]=	"\r\nDriver *NOT* installed";
char _inconst	dszUnitAlloc[]=		"\r\nallocated unit structure: %w";
#endif /* DEBUG */




/* **********************************************************************
 * **** Messages used during initialization *****************************
 * ******************************************************************* */
INITMSG	szNoDriverTab[]="%s: error retrieving driver table";
INITMSG	szNoDevTab[]=	"%s: error retrieving device table from ADD %u";
INITMSG	szNoSCSI[]=	"%s: adapter %u has no SCSI bus (%04x)";
INITMSG	szNo16M[]=	"%s: adapter %u doesn't support memory above 16MB";
INITMSG	szNoScGa[]=	"%s: adapter %u doesn't support HW scatter/gather";
INITMSG	szNoDiskType[]= "%s: adapter %u, unit %u is no disk device";
INITMSG	szRemovable[]=	"%s: adapter %u, unit %u is removable";
INITMSG	szDiskette[]=	"%s: adapter %u, unit %u is diskette drive";
INITMSG	szDefective[]=	"%s: adapter %u, unit %u is defect";
INITMSG	szAllocFailed[]="%s: adapter %u, unit %u allocation failed";
INITMSG	szDeallocFailed[]="%s: adapter %u, unit %u, deallocation failed";
INITMSG	szChangeUInfoFailed[]="%s: adapter %u, unit %u, change unit info failed";
INITMSG	szNoTimer[]=	"%s: couldn't install timer handler";
INITMSG	szRegDevClassFailed[]=	"%s: error registering device class";
INITMSG	szNoDevices[]=	"%s: no devices found";

/* Related to device test */

INITMSG	szTestStart[]=	"%s: adapter %u, unit %u, device test starts...";
INITMSG	szTestComplete[]= "%s: adapter %u, unit %u, device test ended successfully";
INITMSG	szStopFailed[]= "%s: adapter %u, unit %u STOP UNIT failed (%04x)";
INITMSG	szNoStart[]=	"%s: adapter %u, unit %u, cannot issue START UNIT command";
INITMSG	szStartFailed[]="%s: adapter %u, unit %u, START UNIT failed (%04x)";
INITMSG	szStatusblock[]="%s: StatusBlock, Flags %02x, Adapter Error %04x, Target Status %02x";
INITMSG	szAdapterDiag[]="%s: adapter specific info [%s]";
INITMSG	szSensedata[]=	"%s: sense data [%s]";
INITMSG	szHex[]=	"%02x ";

/* Related to command line parsing */

INITMSG	szCmdLineSyntax[]= "%s: syntax error in command line";
INITMSG	szMissingOption[]= "%s: required option missing in command line";
INITMSG	szUnknownOption[]= "%s: unknown command line option";
INITMSG	szCmdLineError[]="%s: command line option error %d";

/* Other */

INITMSG	szFoundDevice[]="Adapter %u, Unit %u installed (%u min)";



/*
 * Very important :-)
 */
INITMSG szProduct[]="SCSI Disk Sleeper";
INITMSG szLogo[]=	"\n\r%s                                            Version %u.%02u\n\r" \
			"Copyright 1996-97 by Vitus Jensen (2:2474/424)               " __DATE__ ;
INITMSG szNoReg[]= "Not registered for commercial use";
INITMSG szReg[]=	"Registered to: %s";

#define REG_MAGIC	0x53A1B7F9
#define REG_KEYLEN	40
#pragma pack(1)
typedef struct {
    ULONG	ulMagic;
    UCHAR	szUser[REG_KEYLEN];
    UCHAR	szRegCode[REG_KEYLEN];
    ULONG	ulSize;
} REGSTRUCT;
#pragma pack()
REGSTRUCT _ininit RegData = {
    REG_MAGIC,
    "\000T3456789012345678901234567890123456789",
    "nfjhfjhfkjshffdsfsfsfjshfkjshfkjsfhsjkf",
    sizeof(REGSTRUCT) };



#pragma pack(1)
typedef struct {
    UCHAR	adpno;				/* MSB set if valid entry */
    UCHAR	unitno;
    USHORT	to;
} TO_OPTION;
#pragma pack(1)
TO_OPTION _ininit	ToOption[MAX_UNITS] = {0};

PRIVATE USHORT _ininit	usSleepSeconds = SLEEP_TIME; /* timeout without /[!]UT */





/*
 * Last element in code segments which remains resident
 */
PRIVATE void NEAR
Code_End(void) {}




char _ininit	achDump[200];
PRIVATE void NEAR
DumpStatusblock(NPSCSI_STATUS_BLOCK statusblock)
{
    /* Status block */

    sprintk(szMsgBuffer, szStatusblock, (PSZ)szDriverName,
	    statusblock->Flags, statusblock->AdapterErrorCode,
	    statusblock->TargetStatus);
    SaveMessage();

    /* Adapter specific info, only if valid */

    if( (statusblock->Flags & STATUS_DIAGINFO_VALID) )
    {
	USHORT	i;

	achDump[0] = '\0';
	for( i = 0; i < 8; ++i )
	    sprintk(&achDump[_fstrlen(achDump)],
		    szHex, statusblock->AdapterDiagInfo[i]);
	sprintk(szMsgBuffer, szAdapterDiag, (PSZ)szDriverName, (PSZ)achDump);
	SaveMessage();
    }

    /* Sense data, only if valid */

    if( (statusblock->Flags & STATUS_SENSEDATA_VALID) )
    {
	USHORT	i;
	PUCHAR	sensedata = (PUCHAR)statusblock->SenseData;

	achDump[0] = '\0';
	for( i = 0; i < statusblock->ReqSenseLen; ++i )
	    sprintk(&achDump[_fstrlen(achDump)], szHex, sensedata[i]);
	sprintk(szMsgBuffer, szSensedata, (PSZ)szDriverName, (PSZ)achDump);
	SaveMessage();
    }
    return;
}




/*# ----------------------------------------------------------------------
 * SendIORBNotify(iorb)
 *
 * PARAMETER
 *	iorb		completed request
 * RETURNS
 *	nothing
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Awakens any process blocking on the passed IORB.  Used
 *	by 'SendIORB'.
 * REMARKS
 */
PRIVATE void FAR _saveregs _loadds _Cdecl
SendIORBNotify(PIORB iorb)
{
    USHORT awoken;

    DevHelp_ProcRun((ULONG)iorb, &awoken);
    return;
}




/*# ----------------------------------------------------------------------
 * SendIORB(iorb,addentry)
 *
 * PARAMETER
 *	iorb		request to execute
 *	addentry	entry point of ADD to call
 * RETURNS
 *	0		OK
 *	/0		error code returned from ADD
 * GLOBAL
 *	(none)
 * DESPRIPTION
 *	Uses 'SendIORBNotify' as notification routine.
 * REMARKS
 *	Need local variable to prevent restoring sp from bp
 *	after ADD call.
 */
#if defined(_MSC_VER)
# pragma optimize("lge",off)
#endif
PRIVATE USHORT NEAR
SendIORB(PIORB iorb,void (FAR _Cdecl * addentry)(PIORB))
{
    USHORT	rc = 0;

    iorb->NotifyAddress = (PVOID)SendIORBNotify; /* quick hack! */
    iorb->RequestControl |= IORB_ASYNC_POST;
    iorb->ErrorCode =	0;
    iorb->Status =	0;

    CallADD(addentry, iorb);

    cli();
    while( !(iorb->Status & IORB_DONE) )
    {
	rc = DevHelp_ProcBlock((ULONG)iorb, (ULONG)-1l, WAIT_IS_INTERRUPTABLE);
	if( rc != 0 )
	    break;				/* was interrupted: abort! */
	cli();
    }
    sti();

    return (rc != 0
	    ? IOERR_CMD_ABORTED : (iorb->Status & IORB_ERROR ? iorb->ErrorCode : 0));
}
#if defined(_MSC_VER)
# pragma optimize("",on)
#endif




/*# ----------------------------------------------------------------------
 * TestOperation(device)
 *
 * PARAMETER
 *	device		device to test
 * RETURNS
 *	0		OK, test passed
 *	/0		fatal error
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Special setting allows to start/stop unit during startup.
 *	Slow but safe.
 * REMARKS
 *	Last step should be a real READ (ADAPTER_PASSTHRU or EXECUTE_IO),
 *	to skip the generation of a scatter/gather list (not impossible!)
 *	we do a READ_VERIFY.  Hopefully this affects the medium, too.
 */
PRIVATE int NEAR
TestOperation(NPDEVICE const device)
{
    int	flag;

    sprintk(szMsgBuffer, szTestStart, (PSZ)szDriverName,
	    device->iAdapter, device->iUnit);
    SaveMessage();


    /* First: stop device */

    IssueStop(device);


    /* Wait for STOP complete */

    while( device->blocked )
	DevHelp_Beep(25000, 3000);		/* wait some time */

    if( !device->stopped )			/* not successfull? */
    {
	sprintk(szMsgBuffer, szStopFailed, (PSZ)szDriverName,
		device->iAdapter, device->iUnit, device->lasterror);
	SaveMessage();
	DumpStatusblock(&device->statusblock);
	return 2;
    }


    /* Some devices may have problems restarting immediately
     * after a stop.  Wait some time? */

    DevHelp_Beep(25000, 1000);			/* wait some time */


    /* Second: device should also be restartable... */

    flag = IssueStart(device);
    if( flag )					/* Errors? */
    {
	sprintk(szMsgBuffer, szNoStart, (PSZ)szDriverName,
		device->iAdapter, device->iUnit);
	SaveMessage();
	return 3;
    }

    /* Wait for START complete */

    while( device->blocked )
	DevHelp_Beep(25000, 3000);		/* wait some time */

    if( device->stopped )			/* not successfull? */
    {
	sprintk(szMsgBuffer, szStartFailed, (PSZ)szDriverName,
		device->iAdapter, device->iUnit, device->lasterror);
	SaveMessage();
	DumpStatusblock(&device->statusblock);
	return 4;
    }


    /* Only reason to execute the code below: device test
     * completed successfully. */

    sprintk(szMsgBuffer, szTestComplete, (PSZ)szDriverName,
	    device->iAdapter, device->iUnit);
    SaveMessage();
    return 0;
} /* end[TestOperation] */




/*# ----------------------------------------------------------------------
 * CheckAdapterInfo(iAdapter,npAdapterInfo)
 *
 * PARAMETER
 *	iAdapter		global index of adapter
 *	npAdapterInfo		from GET_DEVICE_TABLE
 * RETURNS
 *	0		OK to process units here
 *	/0		no SCSI, defect, ...
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Checks various flags in adapter information.  Displays
 *	some nice-to-know thing when DEBUG:1
 * REMARKS
 */
PRIVATE int NEAR
CheckAdapterInfo(USHORT const iAdapter,NPADAPTERINFO const npAdapterInfo)
{
    USHORT	devbus = (npAdapterInfo->AdapterDevBus & 0x000F);

    if( (fDriverFlags & DF_DEBUG)
       &&  !(npAdapterInfo->AdapterFlags & AF_16M) )
    {
	sprintk(szMsgBuffer, szNo16M, (PSZ)szDriverName, iAdapter);
	SaveMessage();
    }
    if( (fDriverFlags & DF_DEBUG)
       &&  !(npAdapterInfo->AdapterFlags & AF_HW_SCATGAT) )
    {
	sprintk(szMsgBuffer, szNoScGa, (PSZ)szDriverName, iAdapter);
	SaveMessage();
    }

    if( !(devbus == AI_DEVBUS_SCSI_1
	  ||  devbus == AI_DEVBUS_SCSI_2  ||  devbus == AI_DEVBUS_SCSI_3) )
    {
	/* No SCSI devices to expect on non-SCSI bus,
	 * ignore adapter. */

	if( (fDriverFlags & DF_DEBUG) )
	{
	    sprintk(szMsgBuffer, szNoSCSI, (PSZ)szDriverName,
		    iAdapter, npAdapterInfo->AdapterDevBus);
	    SaveMessage();
	}
	return 1;
    }

    return 0;
} /* end[CheckAdapterInfo] */




/*# ----------------------------------------------------------------------
 * CheckUnitInfo(iAdapter,npUnitInfo)
 *
 * PARAMETER
 *	iAdapter	global index of adapter
 *	npUnitInfo	from GET_DEVICE_TABLE
 * RETURNS
 *	0		OK, try/use this unit
 *	/0		bad type, defective, etc.
 * GLOBAL
 *	none
 * DESPRIPTION
 *	If this unit isn't a disk or a disk but a diskette, skip this
 *	unit.  Could be expanded to allow more device types in the
 *	future.
 *	Removable media mostly stops by itself and is no so easy to
 *	support as it could be removed anytime.
 *	If this unit is defect or the ADD command line indicates
 *	NO DASD/SCSI support then skip the unit, too.
 * REMARKS
 */
PRIVATE int NEAR
CheckUnitInfo(USHORT const iAdapter, NPUNITINFO const npUnitInfo)
{
    USHORT const utype = npUnitInfo->UnitType;
    USHORT const uflags = npUnitInfo->UnitFlags;


    if( utype != UIB_TYPE_DISK )
    {
	if( (fDriverFlags & DF_DEBUG) )
	{
	    sprintk(szMsgBuffer, szNoDiskType, (PSZ)szDriverName,
		    iAdapter, npUnitInfo->UnitIndex);
	    SaveMessage();
	}
	return 1;
    }

    if( (uflags & UF_REMOVABLE) )
    {
	if( (fDriverFlags & DF_DEBUG) )
	{
	    sprintk(szMsgBuffer, szRemovable, (PSZ)szDriverName,
		    iAdapter, npUnitInfo->UnitIndex);
	    SaveMessage();
	}
	return 2;
    }

    if( (uflags & (UF_A_DRIVE | UF_B_DRIVE)) )
    {
	if( (fDriverFlags & DF_DEBUG) )
	{
	    sprintk(szMsgBuffer, szDiskette, (PSZ)szDriverName,
		    iAdapter, npUnitInfo->UnitIndex);
	    SaveMessage();
	}
	return 3;
    }

    if( (uflags & UF_DEFECTIVE) )
    {
	if( (fDriverFlags & DF_VERBOSE) )
	{
	    sprintk(szMsgBuffer, szDefective, (PSZ)szDriverName,
		    iAdapter, npUnitInfo->UnitIndex);
	    SaveMessage();
	}
	return 4;
    }

    return 0;
} /* end[CheckUnitInfo] */




/*# ----------------------------------------------------------------------
 * InstallFilter(npDevice)
 *
 * PARAMETER
 *	npDevice	local structure
 *
 * RETURNS
 *	0		OK
 *	/0		something failed, skip this device
 *
 * GLOBAL
 *	hdThisDriver
 *
 * DESPRIPTION
 *	To install our filter we have to change the unit
 *	information of this device.  To change the unit info
 *	of a device we have to allocate it (but don't forget
 *	to deallocate afterwards).
 *
 * REMARKS
 *	Call DevHelp_RegisterDevClass() as late as possible.
 *	A registered driver returning 'don't install' will
 *	panic the kernel. :-(
 */
PRIVATE int NEAR
InstallFilter(NPDEVICE const npDevice)
{
    int rc = 0;

    DEBMSG1(dszInstFilter,npDevice);

    /* Send request to ALLOCATE UNIT.  May be done without registering
     * our driver. */

    {
	NPIORB_UNIT_CONTROL pIOUC = (NPVOID)npInitData;
	USHORT		failed;

	memset(pIOUC, 0, MAX_IORB_SIZE);
	pIOUC->iorbh.Length =		sizeof(IORB_UNIT_CONTROL);
	pIOUC->iorbh.UnitHandle =	npDevice->hdADDUnit;
	pIOUC->iorbh.CommandCode =	IOCC_UNIT_CONTROL;
	pIOUC->iorbh.CommandModifier =	IOCM_ALLOCATE_UNIT;

	failed = SendIORB((PIORB)pIOUC, npDevice->pADDEntry);

	npDevice->iotype = DSKIO_ALLOC;
	npDevice->laststatus = pIOUC->iorbh.Status;
	npDevice->lasterror = pIOUC->iorbh.ErrorCode;
	if( failed )
	{
	    sprintk(szMsgBuffer, szAllocFailed, (PSZ)szDriverName,
		    npDevice->iAdapter, npDevice->iUnit);
	    SaveMessage();
	    return 1;
	}
    }


    /* If requested, test START STOP UNIT.  Even this can be done
     * without a registered driver.
     * Skip device testing if this device has stopping [currently]
     * disabled. */

    if( (fDriverFlags & DF_TESTOP)  &&  npDevice->usSleep != 0 )
    {
	if( TestOperation(npDevice) )
	    rc = 2;
    }


    /* *Now* it's time to register this filter driver.
     * But --- naturally --- only if the above code succeeded
     * and we haven't done it yet. */

    if( rc == 0  &&  (fDriverFlags & DF_REGDEVCLASS) == 0 )
    {
	BOOL	b;

	DEBMSG(dszRegister);
	b = DevHelp_RegisterDeviceClass((NPSZ)szDriverName,
					(PFN)FilterFunction,
					(USHORT)DRIVERFLAGS,
					(USHORT)DRIVERCLASS_ADD,
					(PUSHORT)&hdThisDriver);
	if( b )
	{
	    sprintk(szMsgBuffer, szRegDevClassFailed, (PSZ)szDriverName);
	    SaveMessage();
	    rc = 3;
	}
	else
	    fDriverFlags |= DF_REGDEVCLASS;	/* done! */
    }


    /* If all previous actions completed successfully, modify
     * the device structure by sending a CHANGE UNITINFO */

    if( rc == 0 )
    {
	NPIORB_UNIT_CONTROL pIOUC = (NPVOID)npInitData;
	USHORT		failed;


	/* Insert late-breaking-news in device structure */

	npDevice->modinfo.FilterADDHandle = hdThisDriver;


	memset(pIOUC, 0, MAX_IORB_SIZE);
	pIOUC->iorbh.Length =		sizeof(IORB_UNIT_CONTROL);
	pIOUC->iorbh.UnitHandle =	npDevice->hdADDUnit;
	pIOUC->iorbh.CommandCode =	IOCC_UNIT_CONTROL;
	pIOUC->iorbh.CommandModifier =	IOCM_CHANGE_UNITINFO;

	pIOUC->pUnitInfo =		&npDevice->modinfo;
	pIOUC->UnitInfoLen =		sizeof(UNITINFO);

	failed = SendIORB((PIORB)pIOUC, npDevice->pADDEntry);

	npDevice->iotype = DSKIO_CHANGEUI;
	npDevice->laststatus = pIOUC->iorbh.Status;
	npDevice->lasterror = pIOUC->iorbh.ErrorCode;
	if( failed )
	{
	    sprintk(szMsgBuffer, szChangeUInfoFailed,
		    (PSZ)szDriverName, npDevice->iAdapter, npDevice->iUnit);
	    SaveMessage();

	    /* No return!  We have to deallocate!! */

	    rc = 4;
	}
    }

    {
	NPIORB_UNIT_CONTROL pIOUC = (NPVOID)npInitData;
	USHORT		failed;

	/* Send request to DEALLOCATE UNIT */

	memset(pIOUC, 0, MAX_IORB_SIZE);
	pIOUC->iorbh.Length =		sizeof(IORB_UNIT_CONTROL);
	pIOUC->iorbh.UnitHandle =	npDevice->hdADDUnit;
	pIOUC->iorbh.CommandCode =	IOCC_UNIT_CONTROL;
	pIOUC->iorbh.CommandModifier =	IOCM_DEALLOCATE_UNIT;

	failed = SendIORB((PIORB)pIOUC, npDevice->pADDEntry);

	npDevice->iotype = DSKIO_DEALLOC;
	npDevice->laststatus = pIOUC->iorbh.Status;
	npDevice->lasterror = pIOUC->iorbh.ErrorCode;
	if( failed )
	{
	    /* Couldn't deallocte unit.
	     * Bad, display and ignore */

	    sprintk(szMsgBuffer, szDeallocFailed,
		    (PSZ)szDriverName, npDevice->iAdapter, npDevice->iUnit);
	    SaveMessage();
	}
    }

    DEBMSG(dszInstFilterEnd);
    return rc;
} /* end[InstallFilter] */




/*# ----------------------------------------------------------------------
 * RemoveFilter(npDevice)
 *
 * PARAMETER
 *	npDevice	device structure
 * RETURNS
 *	0		ok, removed
 *	/0		error, not removed
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Removes filter of 'npDevice' from ADD's structures
 *	(unit info).
 * REMARKS
 */
PRIVATE int NEAR
RemoveFilter(NPDEVICE const npDevice)
{
    int rc = 0;

    DEBMSG(dszRemoveFilter);
    {
	NPIORB_UNIT_CONTROL pIOUC = (NPVOID)npInitData;
	USHORT		failed;

	/* Send request to ALLOCATE UNIT */

	memset(pIOUC, 0, MAX_IORB_SIZE);
	pIOUC->iorbh.Length =		sizeof(IORB_UNIT_CONTROL);
	pIOUC->iorbh.UnitHandle =	npDevice->hdADDUnit;
	pIOUC->iorbh.CommandCode =	IOCC_UNIT_CONTROL;
	pIOUC->iorbh.CommandModifier =	IOCM_ALLOCATE_UNIT;

	failed = SendIORB((PIORB)pIOUC, npDevice->pADDEntry);

	npDevice->iotype = DSKIO_ALLOC;
	npDevice->laststatus = pIOUC->iorbh.Status;
	npDevice->lasterror = pIOUC->iorbh.ErrorCode;
	if( failed )
	{
	    sprintk(szMsgBuffer, szAllocFailed,
		    (PSZ)szDriverName, npDevice->iAdapter, npDevice->iUnit);
	    SaveMessage();
	    return 1;
	}
    }

    /* Restore previous unit info */

    npDevice->modinfo.FilterADDHandle = npDevice->hdFilter;
    npDevice->modinfo.UnitHandle = npDevice->hdADDUnit;

    {
	NPIORB_UNIT_CONTROL pIOUC = (NPVOID)npInitData;
	USHORT		failed;

	/* Send request to CHANGE UNITINFO */

	memset(pIOUC, 0, MAX_IORB_SIZE);
	pIOUC->iorbh.Length =		sizeof(IORB_UNIT_CONTROL);
	pIOUC->iorbh.UnitHandle =	npDevice->hdADDUnit;
	pIOUC->iorbh.CommandCode =	IOCC_UNIT_CONTROL;
	pIOUC->iorbh.CommandModifier =	IOCM_CHANGE_UNITINFO;

	pIOUC->pUnitInfo =		&npDevice->modinfo;
	pIOUC->UnitInfoLen =		sizeof(UNITINFO);

	failed = SendIORB((PIORB)pIOUC, npDevice->pADDEntry);

	npDevice->iotype = DSKIO_CHANGEUI;
	npDevice->laststatus = pIOUC->iorbh.Status;
	npDevice->lasterror = pIOUC->iorbh.ErrorCode;
	if( failed )
	{
	    sprintk(szMsgBuffer, szChangeUInfoFailed,
		    (PSZ)szDriverName, npDevice->iAdapter, npDevice->iUnit);
	    SaveMessage();

	    /* No return!  We have to deallocate!! */

	    rc = 2;
	}
    }

    {
	NPIORB_UNIT_CONTROL pIOUC = (NPVOID)npInitData;
	USHORT		failed;

	/* Send request to DEALLOCATE UNIT */

	memset(pIOUC, 0, MAX_IORB_SIZE);
	pIOUC->iorbh.Length =		sizeof(IORB_UNIT_CONTROL);
	pIOUC->iorbh.UnitHandle =	npDevice->hdADDUnit;
	pIOUC->iorbh.CommandCode =	IOCC_UNIT_CONTROL;
	pIOUC->iorbh.CommandModifier =	IOCM_DEALLOCATE_UNIT;

	failed = SendIORB((PIORB)pIOUC, npDevice->pADDEntry);

	npDevice->iotype = DSKIO_DEALLOC;
	npDevice->laststatus = pIOUC->iorbh.Status;
	npDevice->lasterror = pIOUC->iorbh.ErrorCode;
	if( failed )
	{
	    /* Couldn't deallocte unit.
	     * Bad, display and ignore */

	    sprintk(szMsgBuffer, szDeallocFailed,
		    (PSZ)szDriverName, npDevice->iAdapter, npDevice->iUnit);
	    SaveMessage();
	}
    }

    return rc;
} /* end[RemoveFilter] */




/*# ----------------------------------------------------------------------
 * Add2TOTable(aidx,uidx,to)
 *
 * PARAMETER
 *	aidx,uindx	device coords
 *	to		timeout value
 * RETURNS
 *	nothing
 * GLOBAL
 *	ToOption[]
 * DESPRIPTION
 *	Adds entry to global table to set unit specific timeout.
 * REMARKS
 */
PRIVATE void NEAR
Add2TOTable(USHORT const aidx,USHORT const uidx,USHORT const to)
{
    int i;

    if( aidx > 0x007F  ||  uidx > 0xFF )
	return;					/* quiet fail */

    for( i = 0; i < MAX_UNITS; ++i )
    {
	if( (ToOption[i].adpno & 0x80) == 0 )
	{
	    ToOption[i].adpno = (UCHAR)(aidx | 0x80);
	    ToOption[i].unitno = (UCHAR)uidx;
	    ToOption[i].to = to;
	    break;
	}
	else if( (UCHAR)(ToOption[i].adpno & ~0x80) == aidx
		&&  ToOption[i].unitno == uidx )
	{
	    ToOption[i].to = to;
	    break;
	}
    }

    return;
}




/*# ----------------------------------------------------------------------
 * QueryTOTable(aidx,uidx)
 *
 * PARAMETER
 *	aidx,uidx	device coords
 * RETURNS
 *	pointer to ToOption[] entry or NULL
 * GLOBAL
 *	ToOption[]
 * DESPRIPTION
 *	Retrieve special timeout settings for this
 *	device, if available.
 * REMARKS
 */
PRIVATE TO_OPTION FAR * NEAR
QueryTOTable(USHORT const aidx,USHORT const uidx)
{
    int i;

    if( aidx > 0x007F  ||  uidx > 0xFF )
	return 0;				/* quiet fail */

    for( i = 0; i < MAX_UNITS; ++i )
    {
	if( (ToOption[i].adpno & 0x80)
	   &&  (UCHAR)(ToOption[i].adpno & ~0x80) == aidx
	   &&  ToOption[i].unitno == uidx )
	{
	    return &ToOption[i];
	}
    }

    return 0;					/* let compiler expand... */
}




/*# ----------------------------------------------------------------------
 * ScanDevices(void)
 *
 * PARAMETER
 *	none
 * RETURNS
 *	0		OK, install driver
 *	<0		fatal error, loud abort
 *	>0		no device found, be quiet
 * GLOBAL
 *	anpUnit
 *	npInitData
 *	fDriverFlags
 * DESPRIPTION
 *	Scans all adapters on all ADDs for a SCSI disk device
 *	which isn't removable.
 *	Adds found devices to global list.  Handles already
 *	installed filter drivers.
 * REMARKS
 */
PRIVATE int NEAR
ScanDevices(void)
{
    USHORT	j, k;
    int		fatal = 0;

    USHORT	add_count, add_index, adapter_index;

    void	(FAR _Cdecl * pADDBaseEP)(PIORB);

    struct DevClassTableEntry FAR *pDCE;
    struct DevClassTableStruc FAR *pDCT;


    DEBMSG(dszScanDevices);

    /* Get address of Registered ADD Table */

    if( DevHelp_GetDOSVar(DHGETDOSV_DEVICECLASSTABLE,DRIVERCLASS_ADD,(PPVOID)&pDCT) )
    {
	/* Should never happen.	 What can we do? */

	sprintk(szMsgBuffer, szNoDriverTab, (PSZ)szDriverName);
	SaveMessage();
	return -1;
    }

    add_count = pDCT->DCCount;


    /* For each ADD registered	*/

    DEBMSG(dszAddScan);
    for( pDCE = pDCT->DCTableEntries, add_index = 0, adapter_index = 0;
	add_index < add_count;
	++add_index, ++pDCE )
    {
	{
	    NPIORB_CONFIGURATION pIOCF = (NPVOID)npInitData;

	    /* Get ADD's Device Table */

	    memset(pIOCF, 0, MAX_IORB_SIZE);
	    pIOCF->iorbh.Length =		sizeof(IORB_CONFIGURATION);
	    pIOCF->iorbh.UnitHandle =		0;
	    pIOCF->iorbh.CommandCode =		IOCC_CONFIGURATION;
	    pIOCF->iorbh.CommandModifier =	IOCM_GET_DEVICE_TABLE;

	    pIOCF->pDeviceTable =	(PDEVICETABLE)DeviceTable;
	    pIOCF->DeviceTableLen =	sizeof(DeviceTable);

	    pADDBaseEP = (PVOID)MAKEP(pDCE->DCSelector, pDCE->DCOffset);

	    if( SendIORB((PIORB)pIOCF, pADDBaseEP) )
	    {
		/* Error (?), could not get device table of installed
		 * ADD.	 Just ignore that ADD (which probably hasn't
		 * any adapters/devices, either). */

		sprintk(szMsgBuffer, szNoDevTab, (PSZ)szDriverName, add_index);
		SaveMessage();
		continue;
	    }
	}


	/* For each Adapter in ADD's Device Table */

	DEBMSG(dszAdpScan);
	for( j = 0;
	    j < ((PDEVICETABLE)DeviceTable)->TotalAdapters;
	    ++j, ++adapter_index )
	{
	    NPADAPTERINFO const npAdapterInfo
		= ((PDEVICETABLE)DeviceTable)->pAdapter[j];
	    NPUNITINFO		npUnitInfo;


	    if( CheckAdapterInfo(adapter_index, npAdapterInfo) )
		continue;

	    /* Access UNITINFO for each Device on adapter */

	    DEBMSG(dszUnitScan);
	    for( npUnitInfo = npAdapterInfo->UnitInfo, k = 0;
		k < npAdapterInfo->AdapterUnits;
		++k, ++npUnitInfo )
	    {
		void	(FAR _Cdecl * pADDEntry)();
		USHORT	FilterADDHandle;

		/* If a Filter ADD has been registered for this unit
		 * then send subsequent requests to the Filter ADD
		 * and not to the ADD. */

		if( (FilterADDHandle = npUnitInfo->FilterADDHandle) != 0 )
		{
		    struct DevClassTableEntry FAR *pDCEFilter;

		    DEBMSG(dszOldFilter);
		    pDCEFilter = &pDCT->DCTableEntries[FilterADDHandle-1];

		    pADDEntry = (PVOID)MAKEP(pDCEFilter->DCSelector,
					     pDCEFilter->DCOffset);
		}
		else
		{
		    pADDEntry = pADDBaseEP;
		}

		if( CheckUnitInfo(adapter_index, npUnitInfo) )
		    continue;


		/* We handle this unit, allocate some memory
		 * in global table to hold neccessary data.
		 * Needed at least:
		 * - entry point of ADD (or filter)
		 * - modified unit info */

		GETINITRAM(anpUnit[cUnit],NPDEVICE,sizeof(DEVICE));
		memset(anpUnit[cUnit], 0, sizeof(DEVICE));
		DEBMSG1(dszUnitAlloc,anpUnit[cUnit]);

		anpUnit[cUnit]->pADDEntry = (PVOID)pADDEntry;
		anpUnit[cUnit]->hdADDUnit = npUnitInfo->UnitHandle;
		anpUnit[cUnit]->hdFilter = FilterADDHandle;

#if defined(_MSC_VER)
		_asm { cld }			/* seems to be a bug in MS-C */
#endif
		anpUnit[cUnit]->modinfo = *npUnitInfo;

		anpUnit[cUnit]->modinfo.UnitHandle = (USHORT)anpUnit[cUnit];
		anpUnit[cUnit]->iAdapter = adapter_index;
		anpUnit[cUnit]->iUnit = npUnitInfo->UnitIndex;

		anpUnit[cUnit]->cdbStart[0] = 0x1B;
		anpUnit[cUnit]->cdbStart[1] = (UCHAR)(npUnitInfo->UnitSCSILUN << 5);
		anpUnit[cUnit]->cdbStart[4] = 0x01;

		anpUnit[cUnit]->cdbStop[0] = 0x1B;
		anpUnit[cUnit]->cdbStop[1] = (UCHAR)(npUnitInfo->UnitSCSILUN << 5);
		anpUnit[cUnit]->cdbStop[4] = 0;

		anpUnit[cUnit]->cdbTestReady[0] = 0;
		anpUnit[cUnit]->cdbTestReady[1] = (UCHAR)(npUnitInfo->UnitSCSILUN << 5);

		{
		    TO_OPTION FAR * p = QueryTOTable(anpUnit[cUnit]->iAdapter,
						     anpUnit[cUnit]->iUnit);

		    if( p != 0 )
			anpUnit[cUnit]->usSleep = p->to;
		    else
			anpUnit[cUnit]->usSleep = usSleepSeconds;
		}

		if( InstallFilter(anpUnit[cUnit]) )
		{
		    /* Couldn't allocate unit.	Release
		     * any data allocated and continue with
		     * next device. */

		    anpUnit[cUnit] = 0;
		    npInitData -= sizeof(DEVICE);	/* free memory */
		    ++fatal;				/* that may be fatal */
		    continue;
		}

		if( (fDriverFlags & DF_VERBOSE) )
		{
		    sprintk(szMsgBuffer, szFoundDevice,
			    anpUnit[cUnit]->iAdapter, anpUnit[cUnit]->iUnit,
			    anpUnit[cUnit]->usSleep/60U);
		    SaveMessage();
		}

		++cUnit;			/* one more for us */
	    } /* for each unit */
	} /* for each adapter */
    } /* for each ADD */


    if( fatal )					/* errors occurred? */
    {
	USHORT i;

	/* A fatal error occurred during scanning, we cannot
	 * install this driver.	 But there may be devices where
	 * our driver is referenced, remove that pointer.
	 * Ignore any errors, don't release memory (won't
	 * install at all). */

	for( i = 0; i < cUnit; ++i )
	    RemoveFilter(anpUnit[i]);
	return -1;				/* than display and wait */
    }
    if( cUnit == 0 )
    {
	if( (fDriverFlags & DF_VERBOSE) )
	{
	    sprintk(szMsgBuffer, szNoDevices, (PSZ)szDriverName);
	    SaveMessage();
	}
	return 1;				/* non-fatal error */
    }
    return 0;
}




/*#
 * InstallTimer(void)
 *
 * PARAMETER
 *	none
 * RETURNS
 *	0		OK
 *	/0		error, no timer installed
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Install a timer handler so we can detect when
 *	to stop a device.
 * REMARKS
 */
PRIVATE int NEAR
InstallTimer(void)
{
    DEBMSG(dszInstallTimer);
    if( DevHelp_TickCount((NPFN)&AsmTimer,
			  TIMER_TMS/pGlobalInfoSeg->SIS_ClkIntrvl) )
    {
	sprintk(szMsgBuffer, szNoTimer, (PSZ)szDriverName);
	SaveMessage();
	return 1;
    }
    return 0;
}




#define ENTRY_STATE	   0
#define MAX_STATES	   2

/*
 *					opt.state[] initialization definitions
 *
 *						____ entry state
 *						|		previous
 *						v		  opt	|
 *  ----Command Line Option --------		----- STATE TABLE ----- |
 *  token id		string	type		0   1	2		|
 *									|
 *						*  /A:	/A:	 <-------
 *							 plus
 */
#define TOK_ID_NOT_APM	100
#define TOK_ID_APM	101

OPT OPT_VERBOSE =
    {TOK_ID_VERBOSE,0,	"/V", TYPE_0,		{0,  E,	 E}};

OPT OPT_NSR =
    {TOK_ID_NOT_SR,0,	"/!SR", TYPE_0,		{0,  E,	 E}};

OPT OPT_SR =
    {TOK_ID_SR,0,	"/SR", TYPE_0,		{0,  E,	 E}};

OPT OPT_DEBUG_DEFAULT =
    {TOK_ID_DEBUG,0,	"/DEBUG", TYPE_0,	{0,  E,	 E}};

OPT OPT_DEBUG_NO =
    {TOK_ID_DEBUG,0,	"/DEBUG:", TYPE_D,	{0,  E,	 E}};

OPT OPT_TIME =
    {TOK_ID_TIME,0,	"/T:", TYPE_DDDD,	{0,  E,	 E}};

OPT OPT_NUT =
    {TOK_ID_NOT_UT,0,	"/!UT:", TYPE_ULIST,	{0,  E,	 E}};

OPT OPT_UT =
    {TOK_ID_UT,0,	"/UT:", TYPE_ULIST,	{0,  E,	 E}};

OPT OPT_NAPM =
    {TOK_ID_NOT_APM,0,	"/!APM", TYPE_0,	{0,  E,	 E}};

OPT OPT_APM =
    {TOK_ID_APM,0,	"/APM", TYPE_0,		{0,  E,	 E}};

OPT OPT_END =
    {TOK_ID_END,0,	"\0", TYPE_0,		{O,  O,	 O}};


/*
 *
 *   The following is a generic OPTIONTABLE for ADDs which support SCSI
 *   devices.
 *
 *   Please note the importance of the ordering of entries in poption[]
 *   For example: the pointer to the option "/DM:" must come before the
 *   option "/DM"
 */
OPTIONTABLE  opttable = {
    ENTRY_STATE, MAX_STATES,
    {
	&OPT_VERBOSE,
	&OPT_NSR,
	&OPT_SR,
	&OPT_DEBUG_NO,
	&OPT_DEBUG_DEFAULT,
	&OPT_TIME,
	&OPT_NUT,
	&OPT_UT,
	&OPT_NAPM,
	&OPT_APM,
	&OPT_END
    }
};



/*# ----------------------------------------------------------------------
 * AnalyseCmdLine(pszLine)
 *
 * PARAMETER
 *	pszLine		pointer to command line
 * RETURNS
 *	0		OK
 * GLOBAL
 * DESPRIPTION
     Invoke parser and analyse token output.
 * REMARKS
 */
PRIVATE int NEAR
AnalyseCmdLine(PSZ pszLine)
{
    CC		parse_rc;
    PUCHAR	cp;
    int		i;
    UCHAR	TokenId;


    /* Call IBM supplied parser with the above defined option
     * table. */

    parse_rc = Command_Parser(pszLine, &opttable,
			      DeviceTable, MAX_DEVICETABLE_SIZE);

    /* Process errors. Although errors were detected the
     * command line and 'DeviceTable' may contain valid tokens
     * which have to be processed later, so only use 'break'
     * in 'case' clauses. */

    switch( parse_rc.ret_code )
    {
	/* SYNTAX_ERR:
	 *	General error in command line, no special handling
	 */
      case SYNTAX_ERR:
	sprintk(szMsgBuffer, szCmdLineSyntax, (PSZ)szDriverName);
	SaveMessage();
	break;

	/* REQ_OPT_ERR:
	 *	Required option is missing in command line (probably /A: )
	 */
      case REQ_OPT_ERR:
	sprintk(szMsgBuffer, szMissingOption, (PSZ)szDriverName);
	SaveMessage();
	break;

	/* INVALID_OPT_ERR:
	 *	Bad format?
	 */
      case INVALID_OPT_ERR:
	sprintk(szMsgBuffer, szUnknownOption,
		(PSZ)szDriverName, (PBYTE)DeviceTable);
	SaveMessage();
	break;

      case UNDEFINED_TYPE_ERR:
      case UNDEFINED_STATE_ERR:
      case BUF_TOO_SMALL_ERR:
      default:
	sprintk(szMsgBuffer, szCmdLineError,
		(PSZ)szDriverName, parse_rc.ret_code);
	SaveMessage();
      case NO_OPTIONS_FND_ERR:
      case NO_ERR:
	break;
    }

    for( cp = DeviceTable;
	 (TokenId=*(cp+TOKL_ID)) != TOK_ID_END;
	 cp += *(cp+TOKL_LEN) )
    {
	switch( TokenId )
	{
	  case TOK_ID_VERBOSE:
	    fDriverFlags |= DF_VERBOSE;
	    break;

	  case TOK_ID_APM:
	    fDriverFlags &= ~DF_NOAPM;
	    break;

	  case TOK_ID_NOT_APM:
	    fDriverFlags |= DF_NOAPM;
	    break;

	  case TOK_ID_SR:
	    fDriverFlags |= DF_SHUTDOWN;
	    break;

	  case TOK_ID_NOT_SR:
	    fDriverFlags &= ~DF_SHUTDOWN;
	    break;

	  case TOK_ID_DEBUG:
	    fDriverFlags |= DF_VERBOSE;
	    fDriverFlags |= DF_DEBUG;

	    if( *(cp+TOKL_LEN) == TOK_MIN_LENGTH+1 )
	    {
		UCHAR code = *(cp+TOKL_VALUE);

		switch( code )
		{
		  case 1:
		    fDriverFlags |= DF_TESTOP;
		    break;

		  default:
		    sprintk(szMsgBuffer, szUnknownOption,
			    (PSZ)szDriverName, (PBYTE)DeviceTable);
		    SaveMessage();
		    break;
		}
	    }
	    break;

	  case TOK_ID_UT:
	    for( i = 0;
		i < *(cp+TOKL_LEN)-2;
		i += 6 )			/* for every coordinate */
	    {
		USHORT time = usSleepSeconds;	/* current active setting */

		if( *(PUSHORT)(cp+TOKL_VALUE+i+4) != 0 )
		    time = *(PUSHORT)(cp+TOKL_VALUE+i+4) * 60U;

		Add2TOTable(*(PUSHORT)(cp+TOKL_VALUE+i),
			    *(PUSHORT)(cp+TOKL_VALUE+i+2), time);
	    }
	    break;

	  case TOK_ID_NOT_UT:
	    for( i = 0;
		i < *(cp+TOKL_LEN)-2;
		i += 6 )			/* for every coordinate */
	    {
		if( *(PUSHORT)(cp+TOKL_VALUE+i+4) != 0 )
		{
		    sprintk(szMsgBuffer, szCmdLineSyntax, (PSZ)szDriverName);
		    SaveMessage();
		}
		else
		    Add2TOTable(*(PUSHORT)(cp+TOKL_VALUE+i),
				*(PUSHORT)(cp+TOKL_VALUE+i+2),
				0);
	    }
	    break;

	  case TOK_ID_TIME:
	    {
		USHORT time = *(PUSHORT)(cp+TOKL_VALUE);

		usSleepSeconds = time * 60U;
	    }
	    break;

	  case TOK_ID_END:
	    break;

	  default:
	    DBSTOP();
	    break;
	}
    }

    return 0;
} /*end[AnalyseCmdLine]*/




/*
 * CALL
 *	CheckRegistration(void)
 * PARAMETER
 *	none
 * RETURNS
 *	nothing
 * GLOBAL
 *	fDriverFlags updated
 * DESPRIPTION
 *	Sets DF_REGISTERED in fDriverFlags depending on
 *	registration code.
 * REMARKS
 */
PRIVATE void NEAR
CheckRegistration(void)
{
    UCHAR code[REG_KEYLEN];
    UCHAR c;
    int i;
    UCHAR _far *p, _far *k;

    _fmemset(code, 0, REG_KEYLEN);
    for( c = 0, i = 0, p = RegData.szUser, k = szProduct;
	*p != '\0';
	++p, ++i, k = (*(k+1) == '\0' ? szProduct : k+1) )
    {
	c = (UCHAR)((*p & 0xf0) >> 4 | (*p & 0x0f));
	c *= *k;
	c += *k;
	code[i] = c;
    }
    if( i != 0	&&  _fmemcmp(code, RegData.szRegCode, i) == 0 )
	fDriverFlags |= DF_REGISTERED;
    return;
}






/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */


/*#
 * InitBase(pRPI)
 *
 * PARAMETER
 *	pRPI		init request
 * RETURNS
 *	error code as expected from the OS/2 device system
 * GLOBAL
 *	Device_Help
 *	pGlobalInfoSeg
 *	fDriverFlags
 *	szDriverName, error messages
 * DESPRIPTION
 *	Process Base Initialization Request Packet
 * REMARKS
 *	Uses IBM supplied command line parser.
 */
PUBLIC USHORT NEAR
InitBase(PRPINITIN pRPI)
{
    PULONG		pInfoSegSel;
    int			rc;
    PRPINITOUT const	pRPO = (PRPINITOUT)pRPI;
    PDDD_PARM_LIST const pInitParms = (PDDD_PARM_LIST)pRPI->InitArgs;
    PSZ			pCmdLine;

    DBSTOP();

    /* Contains entry point to kernel functions */

    Device_Help = pRPI->DevHlpEP;
    pCmdLine	= MK_FP(FP_SEL(pInitParms),pInitParms->cmd_line_args);

    DevHelp_GetDOSVar(DHGETDOSV_SYSINFOSEG, 0, (PPVOID)&pInfoSegSel);
    pGlobalInfoSeg = MAKEP(*pInfoSegSel, 0);


    /* Parse Command Line Args */

    AnalyseCmdLine(pCmdLine);

    CheckRegistration();

    if( !(fDriverFlags & DF_REGISTERED) )
	fDriverFlags |= DF_VERBOSE;
    if( (fDriverFlags & DF_VERBOSE) )
    {
	sprintk(szMsgBuffer, szLogo, (PSZ)szProduct, VERSION, SUBVERSION);
	SaveMessage();
	if( (fDriverFlags & DF_REGISTERED) )
	{
	    sprintk(szMsgBuffer, szReg, (PSZ)RegData.szUser);
	    SaveMessage();
	}
	else
	{
	    sprintk(szMsgBuffer, szNoReg);
	    SaveMessage();
	}
    }


    rc = ScanDevices();
    if( rc == 0 )
    {
	rc = InstallTimer();
    }

    /* Processing complete.  Analyse situation, should we install,
     * quiet fail or even loudly stop Config.Sys processing? */

    if( rc == 0 )
    {
	/* We do not need to register any units, since this
	 * filter does not generate any units on his own. */

	pRPO->Unit = 0;
	pRPO->CodeEnd = ((USHORT)Code_End & ~0x0F) + 0x10;
	pRPO->DataEnd = ((USHORT)npInitData & ~0x0F) + 0x10;

	DEBMSG(dszInstalled);
	fDriverFlags |= DF_INITDONE;
	return STATUS_DONE;
    }
    else
    {
	pRPO->Unit = 0;
	pRPO->CodeEnd = 0;
	pRPO->DataEnd = 0;

	DEBMSG(dszNotInstalled);
	if( rc < 0 )
	    return STERR | STDON | ERROR_I24_GEN_FAILURE;
	else
	    return STERR | STDON | ERROR_I24_QUIET_INIT_FAIL;
    }
}



/* History:
 *
 * $Log: init1.c,v $
 * Revision 1.24  2001/02/27 02:24:47  vitus
 * - added /APM and /!APM switches
 *
 * Revision 1.23  2001/02/16 02:12:36  vitus
 * - removed APM releated code, this is now done on INIT_COMPLETE
 *
 * Revision 1.22  1999/11/29 00:35:12  vitus
 * - moved all system includes to dsksleep.h
 * - minor changes for M$ C 7.0
 *
 * Revision 1.21  1999/11/28 23:45:30  vitus
 * - uses WAIT_IS_INTERRUPTABLE
 *
 * Revision 1.20  1998/06/29 00:01:25  vitus
 * - renamed _far_*() to _f*() (for MS-C, WatCom supplies)
 * - switched to DDK support macros (cli, pushf, etc.)
 *
 * Revision 1.19  1998/01/29 22:20:09  vitus
 * - added casts to skip two warnings
 * - uses #define INITMSG to place strings
 *
 * Revision 1.18  1997/12/05 01:33:40  vitus
 * - added WatCom specific code and header
 * - removed things declared in newer DDKs
 * - declared all exported and imported functions as _Cdecl
 * - changes for new OPT structure, removed unneccessary casts
 *
 * Revision 1.17  1997/11/23 22:37:50  vitus
 * - correctly corrected detection of non-SCSI controllers
 *
 * Revision 1.16  1997/11/22 01:07:47  vitus
 * - corrected detection of non-SCSI controllers
 *
 * Revision 1.15  1997/10/26 00:29:18  vitus
 * - longer and higher beep
 * - /!UT device aren't tested
 * - modified UnitHandle is pointer not index (better debugging)
 *
 * Revision 1.14  1997/10/24 23:30:58  vitus
 * - slower beep (lo ger)
 *
 * Revision 1.13  1997/10/11 17:44:11  vitus
 * - does driver registration *after* device test
 * - clear IORB with MAX_IORB_SIZE bytes, as most DMD do
 *
 * Revision 1.12  1997/09/03 01:02:23  vitus
 * - saves status and sense values in device structure (IOCtl retrieves)
 * - accept /!DM or /!SM units
 *
 * Revision 1.11  1997/07/21 01:13:38  vitus
 * Changed TestOperation to use IssueStart,IssueStop
 * Added /SR, /!SR tokens in AnalyseCmdLine
 * Detects APM
 *
 * Revision 1.10  1997/06/18 00:49:45  vitus
 * SendIORB() now saves,restores all registers
 *
 * Revision 1.9  1997/06/03 23:44:11  vitus
 * Removed syntax errors
 *
 * Revision 1.8  1997/05/11 14:44:12  vitus
 * Removed TAB from logo string
 *
 * Revision 1.7  1997/05/07 23:31:37  vitus
 * Added DEBMSG calls and strings
 * Devhelp_Beep now public (see iorb.c)
 * Corrected dump of status block
 * Delayed driver registration (avoids trap)
 * Shows timeout on unit display
 * Resident memory area expanded to next paragraph
 *
 * Revision 1.6	 1997/03/03 01:15:02  vitus
 * Corrected dump of sense bytes
 * Corrected adapter numbering
 * Uses SaveMessage instead of DevHelp_Save_Message
 *
 * Revision 1.5	 1997/02/06 01:08:59  vitus
 * Changed copyright to 1997 and 2:2474/424
 *
 * Revision 1.4	 1996/11/04 00:14:57  vitus
 * Better TestOperation(), corrected error display
 * Displays missing 16MB, HW S/G support (debug)
 * Parses /[!]UT tokens (timeout per device)
 *
 * Revision 1.3	 1996/10/24 00:12:25  vitus
 * Added registration check/display
 * Modularisiert
 * Added /Debug[:1] switch
 * Tests start/stop
 *
 * Revision 1.2	 1996/09/30 00:27:21  vitus
 * Added command line parsing
 *
 * Revision 1.1	 1996/09/27 02:28:09  vitus
 * Initial revision
 */
