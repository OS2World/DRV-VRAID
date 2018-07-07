/*
 * $Source: r:/source/driver/perf/RCS/init1.c,v $
 * $Revision: 1.10 $
 * $Date: 2000/06/12 02:34:00 $
 * $Locker:  $
 *
 * First step of initialisation, executed at ring 0 in protected
 * mode as the only running process.
 *
 * History: see bottom of file
 * --------------------------------------------
 * Author: Vitus Jensen, 2:2474/424, 1997-98
 */
#pragma off (unreferenced)
static char const id[]="$Id: init1.c,v 1.10 2000/06/12 02:34:00 vitus Exp $";
#pragma on (unreferenced)


#include <string.h>

#define INCL_DOSERRORS
#define INCL_NOPMAPI
#include <os2.h>

#include <devcmd.h>
#include <devclass.h>
#include <dskinit.h>
#include <infoseg.h>

#define INCL_INITRP_ONLY
#include <reqpkt.h>

#include <scsi.h>
#include <iorb.h>
#include <addcalls.h>
#if defined(__WATCOMC__)
# include <devhelp.h>
typedef USHORT NEAR *	NPUSHORT;
typedef VOID NEAR *	NPVOID;
#else
# include <dhcalls.h>
#endif
#include "cmdphdr.h"

#include "scsiopt.h"
#include "proto.h"
#include "extern.h"

#include "modepages.h"



/*
 * Macro to allocate data from memory pool.
 * OBS: to free this memory decrement 'npInitData' by the amount
 * allocated.  Be sure to undo it inverse.
 */
#define GETINITRAM(p,type,size) (p=(type)npInitData,npInitData+=(size))



#define INITMSG		static char _ininit


/*
 * Messages used during initialization
 *
 * 1. error messages start with driver name as they are
 * displayed even if '/V' isn't used.
 */
INITMSG	szErrorVAddr[]= "%s: error converting virtual to physical address";
INITMSG	szNoDriverTab[]="%s: error retrieving driver table";
INITMSG	szNoDevTab[]=	"%s: error retrieving device table from ADD %u";
INITMSG	szAllocFailed[]="%s: adapter %u, unit %u allocation failed";
INITMSG	szDeallocFailed[]="%s: adapter %u, unit %u, deallocation failed";

INITMSG	szSenseFailed[]= "%s: adapter %u, unit %u MODE SENSE(6) failed (%04x)";
INITMSG	szSelectFailed[]= "%s: adapter %u, unit %u MODE SELECT(6) failed (%04x)";
INITMSG	szUnknFormat[]=	"%s: adapter %u, unit %u, unknown format of page %u";

INITMSG	szStatusblock[]= "%s: StatusBlock, Flags %02x, Adapter Error %04x, Target Status %02x";
INITMSG	szAdapterDiag[]= "%s: adapter specific info [%s]";
INITMSG	szSensedata[]=	"%s: SenseData [%s]";
INITMSG	szHex[]=	"%02x ";


/*
 * 2. Screen messages only displayed if '/V' or '/DEBUG' are used.
 */

INITMSG	szNoDevices[]=	"No devices found";
INITMSG	szNoSCSI[]=	"Adapter %u: no SCSI bus";
INITMSG	szNo16M[]=	"Adapter %u: hardware doesn't support memory above 16MB";
INITMSG	szNoScGa[]=	"Adapter %u: hardware doesn't support scatter/gather";
INITMSG	szDiskette[]=	"Adapter %u, Unit %u: diskette drive";
INITMSG	szDefective[]=	"Adapter %u, Unit %u: defect";

/* Related to device modifications */

INITMSG	szModifyStart[]=  "Adapter %u, Unit %u: modifications start...";
INITMSG	szModifyOK[]=	  "Adapter %u, Unit %u: modifications ended w/o errors";

INITMSG	szRCachePage[]=	  "Adapter %u, Unit %u: reading cache page 8";
INITMSG	szRControlPage[]= "Adapter %u, Unit %u: reading control page 10";
INITMSG	szRConnectPage[]= "Adapter %u, Unit %u: reading connect/disconnect page 2";
INITMSG	szRCDPage[]=	"Adapter %u, Unit %u: reading CD-ROM page 13";
INITMSG	szWCachePage[]=	  "Adapter %u, Unit %u: writing cache page 9";
INITMSG	szWControlPage[]= "Adapter %u, Unit %u: writing control page 10";
INITMSG	szWConnectPage[]= "Adapter %u, Unit %u: writing connect/disconnect page 2";
INITMSG	szWCDPage[]=	"Adapter %u, Unit %u: writing CD-ROM page 13";

/* Display current settings */

INITMSG	szReadCache[]=	"Adapter %u, Unit %u: read cache %s";
INITMSG	szWriteCache[]=	"Adapter %u, Unit %u: write cache %s";
INITMSG	szPrefetchPrio[]= "Adapter %u, Unit %u: prefetch priority %s";
INITMSG	szPrefetch[]=	"Adapter %u, Unit %u: prefetch %s";

INITMSG	szTaggedQueue[]="Adapter %u, Unit %u: tagged queuing %s";
INITMSG	szReorder[]=	"Adapter %u, Unit %u: unrestricted reordering %s";

INITMSG	szFullReselect[]="Adapter %u, Unit %u: buffer fill disconnect/reconnect %s";
INITMSG	szInactiveTO[]="Adapter %u, Unit %u: inactive bus disconnect %s";
INITMSG	szDisconnectTO[]="Adapter %u, Unit %u: disconnect to reselect timeout %s";
INITMSG	szConnectTO[]="Adapter %u, Unit %u: connection timeout %s";

INITMSG	szCDTimeout[]=	"Adapter %u, Unit %u: CD-ROM inactivity timeout %s";


/* Related to command line parsing */

INITMSG	szCmdLineSyntax[]= "%s: syntax error in command line";
INITMSG	szMissingOption[]= "%s: required option missing in command line";
INITMSG	szUnknownOption[]= "%s: unknown command line option";
INITMSG	szCmdLineError[]=  "%s: command line option error %d";


INITMSG	szFoundDevice[]=   "Adapter %u, Unit %u: found";




/*
 * Very important :-)
 */
INITMSG szProduct[]=	"SCSI Optimizer                ";
INITMSG szLogo[]=	"\r\n%s                               Version %u.%02u"
"\r\nMade by Vitus Jensen (2:2474/424)                            " __DATE__ ;




#pragma pack(1)
typedef struct _UNIT_OPTION {
    UCHAR	adpno;			/* MSB set if valid entry */
    UCHAR	unitno;
    struct {
	UINT	access : 2;		/* forces reading of  this page */
	UINT	buffer : 2;		/* empty/full ration */
	UINT	inactive : 2;		/* enable inactive bus timeout */
	UINT	disconnect : 2;		/* enable disconnect timeout */
	UINT	connect : 2;		/* enable connect timeout */
    } pg2;
    struct {
	UINT	access : 2;		/* forces reading of  this page */
	UINT	wce : 2;		/* write cache enable */
	UINT	rce : 2;		/* read cache enable */
	UINT	pre : 2;		/* pre-fetch enable */
	UINT	prio : 2;		/* higher pre-fetch priority */
    } pg8;
    struct {
	UINT	access : 2;		/* forces reading of this page */
	UINT	queue : 2;		/* enable tagged queuing */
	UINT	reorder : 2;		/* unrestricted reordering */
    } pg10;
    struct {
	UINT	access : 2;		/* forces reading of this page */
	UINT	seconds : 12;		/* seconds until CD-ROM stop */
    } pg13;
} UNIT_OPTION, FAR * PUNIT_OPTION;
#pragma pack(1)
UNIT_OPTION _ininit	UnitOption[MAX_UNITS] = {0};






/*
 * Last element in code segments which remains resident
 */
PRIVATE void
Code_End(void) {}




/*#
 * CALL
 *	AddUTable(aidx,uidx,pgno,patch)
 * PARAMETER
 *	aidx		adapter index
 *	uidx		unit index
 *	pgno		mode page to change
 *	enable		enable feature?  or disable?
 *	patch		bitfield telling which patches to apply
 * RETURNS
 *	(nothing)
 * GLOBAL
 *	UnitTable
 * DESPRIPTION
 *	Adds entry to global table to save unit specific settings
 *	or updates already allocated entry.
 *
 * REMARKS
 *	Entries are allocated from low to higher indices and never freed.
 */
PRIVATE void
AddUTable(USHORT aidx,USHORT uidx,USHORT pgno,int enable,USHORT patch)
{
    int i;

    DEBMSG4("\r\nAddUTable: %w %w %w = %w",aidx,uidx,pgno,patch);
    if( aidx > 0x007F  ||  uidx > 0x00FF )
	return;					/* can't allocate that */

    for( i = 0; i < MAX_UNITS; ++i )
    {
	if( (UnitOption[i].adpno & 0x80) == 0
	    ||  ((UCHAR)(UnitOption[i].adpno & ~0x80) == aidx
		 &&  UnitOption[i].unitno == uidx) )
	{
	    UnitOption[i].adpno = (UCHAR)(aidx | 0x80);
	    UnitOption[i].unitno = (UCHAR)uidx;
	    switch( pgno )
	    {
	      case 2:
		UnitOption[i].pg2.access |= (patch == 0 ? 1 : 2);
		if( (patch & 0x0001) )
		    UnitOption[i].pg2.buffer = (enable ? 2 : 1);
		if( (patch & 0x0002) )
		    UnitOption[i].pg2.inactive = (enable ? 2 : 1);
		if( (patch & 0x0004) )
		    UnitOption[i].pg2.disconnect = (enable ? 2 : 1);
		if( (patch & 0x0008) )
		    UnitOption[i].pg2.connect = (enable ? 2 : 1);
		break;

	      case 8:
		UnitOption[i].pg8.access |= (patch == 0 ? 1 : 2);
		if( (patch & 0x0001) )
		    UnitOption[i].pg8.wce = (enable ? 2 : 1);
		if( (patch & 0x0002) )
		    UnitOption[i].pg8.rce = (enable ? 2 : 1);
		if( (patch & 0x0004) )
		    UnitOption[i].pg8.pre = (enable ? 2 : 1);
		if( (patch & 0x0008) )
		    UnitOption[i].pg8.prio = (enable ? 2 : 1);
		break;

	      case 10:
		UnitOption[i].pg10.access |= (patch == 0 ? 1 : 2);
		if( (patch & 0x0001) )
		    UnitOption[i].pg10.queue = (enable ? 2 : 1);
		if( (patch & 0x0002) )
		    UnitOption[i].pg10.reorder = (enable ? 2 : 1);
		break;

	      case 13:
		UnitOption[i].pg13.access |= (patch == 0 ? 1 : 2);
		UnitOption[i].pg13.seconds = (patch > 0x0FFF ? 0x0FFF : patch);
		break;

	      default:
		break;			/* no error, empty entry */
	    }

	    DEBMSG2("\r\nAddUTable: entry\r\n%z",(PVOID)&UnitOption[i],sizeof(UnitOption[i]));
	    return;
	} /* end[if found of empty entry] */
    } /* end[for every entry] */

    return;
}




/*#
 * CALL
 *	QueryUTable(aidx,uidx)
 * PARAMETER
 *	aidx		adapter index
 *	uidx		unit index
 * RETURNS
 *	&UnitTable[]
 * GLOBAL
 *	UnitTable
 * DESPRIPTION
 *	Retrieve special settings for this device, if available.
 *
 * REMARKS
 */
PRIVATE UNIT_OPTION FAR *
QueryUTable(USHORT aidx,USHORT uidx)
{
    int i;

    if( aidx > 0x007F  ||  uidx > 0xFF )
	return 0;				/* quiet fail */

    for( i = 0; i < MAX_UNITS; ++i )
    {
	if( (UnitOption[i].adpno & 0x80)
	   &&  (UCHAR)(UnitOption[i].adpno & ~0x80) == aidx
	   &&  UnitOption[i].unitno == uidx )
	{
	    return &UnitOption[i];
	}
    }

    return 0;					/* let compiler expand... */
}




/*#
 * CALL
 *	DumpStatusblock(statusblock)
 * PARAMETER
 *	statusblock	SCSI_STATUS_BLOCK, see DDK
 * RETURNS
 *	(nothing)
 * GLOBAL
 *	(none)
 * DESPRIPTION
 *	Displays error values and dumps sense data to screen
 *
 * REMARKS
 */
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




/*#
 * CALL
 *	SendIORBNotify(iorb)
 * PARAMETER
 *	iorb		completed request
 * RETURNS
 *	(nothing)
 * GLOBAL
 *	(none)
 * DESPRIPTION
 *	Awakens any process blocking on the passed IORB.  Used
 *	by 'SendIORB'.
 *
 * REMARKS
 */
PRIVATE void FAR _loadds _Cdecl
SendIORBNotify(PIORB iorb)
{
    USHORT awoken;

    DevHelp_ProcRun((ULONG)iorb, &awoken);
    return;
}




/*#
 * CALL
 *	SendIORB(iorb,addentry)
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
 *
 * REMARKS
 *	Need local variable to prevent restoring sp from bp
 *	after ADD call.
 */
#pragma optimize("lge",off)
PRIVATE USHORT NEAR
SendIORB(PIORB iorb,void (FAR _Cdecl * addentry)(PIORB))
{
    USHORT	rc;

    iorb->NotifyAddress = (PVOID)SendIORBNotify; /* kind of hack */
    iorb->RequestControl |= IORB_ASYNC_POST;
    iorb->ErrorCode =	0;
    iorb->Status =	0;

    SAVE_REGS();
    (addentry)(iorb);
    RESTORE_REGS();

    DISABLE();
    while( !(iorb->Status & IORB_DONE) )
    {
	rc = DevHelp_ProcBlock( (ULONG)iorb, -1L, WAIT_IS_INTERRUPTABLE );
	if( rc != 0 )
	    break;				/* was interrupted: abort! */
	DISABLE();
    }
    ENABLE();

    return (rc != 0
	    ? IOERR_CMD_ABORTED : (iorb->Status & IORB_ERROR ? iorb->ErrorCode : 0));
}
#pragma optimize("",on)




/*#
 * CALL
 *	CheckAdapterInfo(iAdapter,npAdapterInfo)
 * PARAMETER
 *	iAdapter		index of adapter in system
 *,	npAdapterInfo		from GET_DEVICE_TABLE
 * RETURNS
 *	0			OK to process units from this adapter
 *	/0			no SCSI, defect, ...
 * GLOBAL
 *	fDriverFlags
 * DESPRIPTION
 *	Checks adapter flags, additional service: displays some
 *	performance related capabilities of adapter.
 *
 * REMARKS
 */
PRIVATE int
CheckAdapterInfo(USHORT const iAdapter,NPADAPTERINFO const npAdapterInfo)
{
    if( !(npAdapterInfo->AdapterFlags & AF_16M) )
    {
	sprintk(szMsgBuffer, szNo16M, iAdapter);
	DEBMSG1("\r\n%s",(PSZ)szMsgBuffer);
	if( (fDriverFlags & DF_DEBUG) )
	    SaveMessage();
    }
    if( !(npAdapterInfo->AdapterFlags & AF_HW_SCATGAT) )
    {
	sprintk(szMsgBuffer, szNoScGa, iAdapter);
	DEBMSG1("\r\n%s",(PSZ)szMsgBuffer);
	if( (fDriverFlags & DF_DEBUG) )
	    SaveMessage();
    }

    if( !((npAdapterInfo->AdapterDevBus & AI_DEVBUS_SCSI_1)
	  ||  (npAdapterInfo->AdapterDevBus & AI_DEVBUS_SCSI_2)
	  ||  (npAdapterInfo->AdapterDevBus & AI_DEVBUS_SCSI_3)) )
    {
	/* No SCSI devices to expect on non-SCSI bus,
	 * ignore adapter. */

	sprintk(szMsgBuffer, szNoSCSI, iAdapter);
	DEBMSG1("\r\n%s",(PSZ)szMsgBuffer);
	if( (fDriverFlags & DF_DEBUG) )
	    SaveMessage();
	return 1;
    }

    return 0;
}




/*#
 * CALL
 *	CheckUnitInfo(iAdapter,npUnitInfo)
 * PARAMETER
 *	iAdapter		index of adapter in system
 *	npUnitInfo		from GET_DEVICE_TABLE
 * RETURNS
 *	0	OK, try/use this unit
 *	/0	bad type, defective, etc.
 * GLOBAL
 *	fDriverFlags
 * DESPRIPTION
 *	If this unit is defect then skip the unit.
 *	Dito if it's diskette A or B.
 *
 * REMARKS
 */
PRIVATE int
CheckUnitInfo(PDEVICE const device,NPUNITINFO const npUnitInfo)
{
    USHORT const uflags = npUnitInfo->UnitFlags;

    DEBMSG2("\r\nCheckUnitInfo: %w %w",device->iAdapter,device->iUnit);
    if( (uflags & (UF_A_DRIVE | UF_B_DRIVE)) )
    {
	sprintk(szMsgBuffer, szDiskette, device->iAdapter, device->iUnit);
	DEBMSG1("\r\n%s",(PSZ)szMsgBuffer);
	if( (fDriverFlags & DF_DEBUG) )
	    SaveMessage();
	return 3;
    }

    if( (uflags & UF_DEFECTIVE) )
    {
	sprintk(szMsgBuffer, szDefective, device->iAdapter, device->iUnit);
	DEBMSG1("\r\n%s",(PSZ)szMsgBuffer);
	if( (fDriverFlags & DF_VERBOSE) )
	    SaveMessage();
	return 4;
    }

    return 0;
}




/*#
 * CALL
 *	ModeSense(pADDEntry,npUnit,pgno,buf,bufsiz)
 * PARAMETER
 *	pADDEntry	call ADD
 *	npUnit		ADD's UNITINFO structure
 *	pgno		which page?
 *	buf
 *	bufsiz
 * RETURNS
 *	0		OK
 *	/0		error code from ADD
 * GLOBAL
 * DESPRIPTION
 * REMARKS
 */
PRIVATE USHORT
ModeSense(void (FAR _Cdecl * pADDEntry)(),NPUNITINFO const npUnit,
	  UCHAR pgno,PUCHAR buf,USHORT bufsiz)
{
    NPIORB_ADAPTER_PASSTHRU	iothru;
    NPSCSI_STATUS_BLOCK		statusblock;
    NPSCSI_REQSENSE_DATA	sensedata;

    BYTE		cdb[6];
    USHORT		error;

    GETINITRAM(iothru, NPIORB_ADAPTER_PASSTHRU, sizeof(IORB_ADAPTER_PASSTHRU));
    GETINITRAM(statusblock, NPSCSI_STATUS_BLOCK, sizeof(SCSI_STATUS_BLOCK));
    GETINITRAM(sensedata, NPSCSI_REQSENSE_DATA, sizeof(SCSI_REQSENSE_DATA));

    cdb[0] = SCSI_MODE_SENSE;
    cdb[1] = (UCHAR)((npUnit->UnitSCSILUN << 5) | 0);
    cdb[2] = pgno;
    cdb[3] = 0;
    cdb[4] = (UCHAR)bufsiz;
    cdb[5] = 0;

    memset(iothru, 0, sizeof(IORB_ADAPTER_PASSTHRU));
    memset(statusblock, 0, sizeof(SCSI_STATUS_BLOCK));
    memset(sensedata, 0, sizeof(SCSI_REQSENSE_DATA));
    statusblock->ReqSenseLen = sizeof(SCSI_REQSENSE_DATA);
    statusblock->SenseData = sensedata;

    iothru->iorbh.Length =	sizeof(IORB_ADAPTER_PASSTHRU);
    iothru->iorbh.UnitHandle =	npUnit->UnitHandle;
    iothru->iorbh.CommandCode =	IOCC_ADAPTER_PASSTHRU;
    iothru->iorbh.CommandModifier =	IOCM_EXECUTE_CDB;

    iothru->iorbh.RequestControl =	IORB_REQ_STATUSBLOCK;
    iothru->iorbh.StatusBlockLen =	sizeof(SCSI_STATUS_BLOCK);
    iothru->iorbh.pStatusBlock =	(NPBYTE)statusblock;

    iothru->pControllerCmd =	cdb;
    iothru->ControllerCmdLen =	6;

    /* Data buffer: have to supply a scatter/gather list
     * (1 entry) and a direction. */

    ScGaEntry.ppXferBuf = (ULONG)((USHORT)buf + ppDiskDDHeader);
    ScGaEntry.XferBufLen = bufsiz;
    iothru->Flags = PT_DIRECTION_IN;
    iothru->cSGList = 1;
    iothru->pSGList = &ScGaEntry;
    iothru->ppSGLIST = (ULONG)((USHORT)&ScGaEntry + ppDiskDDHeader);

    DEBMSG2("\r\nMODE SENSE(6) - iorb cdb sc/ga\r\n%z",(PIORB)iothru,iothru->iorbh.Length);
    DEBMSG2("\r\n%z",(PVOID)cdb,6);
    DEBMSG2("\r\n%z",(PVOID)&ScGaEntry,sizeof(ScGaEntry));

    if( (error=SendIORB((PIORB)iothru, pADDEntry)) )
    {
	sprintk(szMsgBuffer, szSenseFailed, (PSZ)szDriverName,
		npUnit->AdapterIndex, npUnit->UnitIndex, error);
	SaveMessage();
	DumpStatusblock(statusblock);
    }

    npInitData -= sizeof(SCSI_REQSENSE_DATA);
    npInitData -= sizeof(SCSI_STATUS_BLOCK);
    npInitData -= sizeof(IORB_ADAPTER_PASSTHRU);

    return error;
}




/*#
 * CALL
 *	ModeSelect(pADDEntry,npUnit,pgno,buf,bufsiz)
 * PARAMETER
 *	pADDEntry	call ADD
 *	npUnit		ADD's UNITINFO structure
 *	pgno		which page?
 *	buf
 *	bufsiz
 * RETURNS
 *	0		OK
 *	/0		error code from ADD
 * GLOBAL
 * DESPRIPTION
 * REMARKS
 */
PRIVATE USHORT
ModeSelect(void (FAR _Cdecl * pADDEntry)(),NPUNITINFO const npUnit,
	   UCHAR pgno,PUCHAR buf,USHORT bufsiz)
{
    NPIORB_ADAPTER_PASSTHRU	iothru;
    NPSCSI_STATUS_BLOCK		statusblock;
    NPSCSI_REQSENSE_DATA	sensedata;

    BYTE		cdb[6];
    USHORT		error;


    GETINITRAM(iothru, NPIORB_ADAPTER_PASSTHRU, sizeof(IORB_ADAPTER_PASSTHRU));
    GETINITRAM(statusblock, NPSCSI_STATUS_BLOCK, sizeof(SCSI_STATUS_BLOCK));
    GETINITRAM(sensedata, NPSCSI_REQSENSE_DATA, sizeof(SCSI_REQSENSE_DATA));

    cdb[0] = SCSI_MODE_SELECT;
    cdb[1] = (UCHAR)((npUnit->UnitSCSILUN << 5) | 0x10 | 0x00);
    cdb[2] = 0x00;
    cdb[3] = 0;
    cdb[4] = (UCHAR)bufsiz;
    cdb[5] = 0;

    memset(iothru, 0, sizeof(IORB_ADAPTER_PASSTHRU));
    memset(statusblock, 0, sizeof(SCSI_STATUS_BLOCK));
    memset(sensedata, 0, sizeof(SCSI_REQSENSE_DATA));
    statusblock->ReqSenseLen = sizeof(SCSI_REQSENSE_DATA);
    statusblock->SenseData = sensedata;

    iothru->iorbh.Length =		sizeof(IORB_ADAPTER_PASSTHRU);
    iothru->iorbh.UnitHandle =		npUnit->UnitHandle;
    iothru->iorbh.CommandCode =		IOCC_ADAPTER_PASSTHRU;
    iothru->iorbh.CommandModifier =	IOCM_EXECUTE_CDB;

    iothru->iorbh.RequestControl =	IORB_REQ_STATUSBLOCK;
    iothru->iorbh.StatusBlockLen =	sizeof(SCSI_STATUS_BLOCK);
    iothru->iorbh.pStatusBlock =	(NPBYTE)statusblock;

    iothru->pControllerCmd =	cdb;
    iothru->ControllerCmdLen =	6;

    /* Data buffer: have to supply a scatter/gather list
     * (1 entry) and a direction. */

    ScGaEntry.ppXferBuf = (ULONG)((USHORT)buf + ppDiskDDHeader);
    ScGaEntry.XferBufLen = MAX_MODE_DATA;
    iothru->Flags = 0;
    iothru->cSGList = 1;
    iothru->pSGList = &ScGaEntry;
    iothru->ppSGLIST = (ULONG)((USHORT)&ScGaEntry + ppDiskDDHeader);

    DEBMSG2("\r\nMODE SELECT(6) - iorb cdb sc/ga\r\n%z",(PIORB)iothru,iothru->iorbh.Length);
    DEBMSG2("\r\n%z",(PVOID)cdb,6);
    DEBMSG2("\r\n%z",(PVOID)&ScGaEntry,sizeof(ScGaEntry));

    if( (error=SendIORB((PIORB)iothru, pADDEntry)) )
    {
	sprintk(szMsgBuffer, szSelectFailed, (PSZ)szDriverName,
		npUnit->AdapterIndex, npUnit->UnitIndex, error);
	SaveMessage();
	DumpStatusblock(statusblock);
    }

    npInitData -= sizeof(SCSI_REQSENSE_DATA);
    npInitData -= sizeof(SCSI_STATUS_BLOCK);
    npInitData -= sizeof(IORB_ADAPTER_PASSTHRU);

    return error;
}




/*#
 * CALL
 *	ModifyCachePage(device,npUnit)
 * PARAMETER
 *	device		our own device structure
 *	npUnit		from GET_DEVICE_TABLE
 * RETURNS
 *	0		OK
 * GLOBAL
 *	fDriverFlags
 * DESPRIPTION
 *	Reads caching mode page (MODE SENSE), modifies requested
 *	settings and writes new page (MODE SELECT).
 *	If device options exists for this page nothing is done at all.
 *	If no change is requested MODE SELECT isn't executed.
 *
 * REMARKS
 *	The device isn't requested to save these parameters permanent.  I
 *	think it's safer this way: reset your machine and you get the default
 *	state.
 */
PRIVATE int
ModifyCachePage(PDEVICE const device,NPUNITINFO const npUnit)
{
    NPBYTE	data;
    int		rc = 0;				/* error? */
    PUNIT_OPTION pOption = QueryUTable(device->iAdapter, device->iUnit);

    if( pOption == NULL  ||  pOption->pg8.access == 0 )
    {
	DEBMSG2("\r\nModifyCachePage: nothing requested for %w,%w", device->iAdapter,device->iUnit);
	return 0;				/* no change -> no error */
    }

    GETINITRAM(data, NPBYTE, MAX_MODE_DATA);

    sprintk(szMsgBuffer, szRCachePage, device->iAdapter, device->iUnit);
    DEBMSG1("\r\n%s",(PSZ)szMsgBuffer);
    if( (fDriverFlags & DF_VERBOSE) )
	SaveMessage();

    do
    {
	UCHAR			pgno = 0x08;
	NPSCSI_MODEPAGE_8	page;
	UCHAR			mdlen;
	int			modified = 0;	/* how many modifications? */

	/* First: read cache mode page via MODE SENSE(6),
	 * read current values. */

	if( ModeSense(device->pADDEntry, npUnit, pgno, data, MAX_MODE_DATA) )
	{
	    rc = 1;
	    break;
	}
	DEBMSG3("\r\nOld page %w\r\n%z",pgno,(PVOID)data,sizeof(SCSI_MODEPAGE_HDR)
		+sizeof(SCSI_MODEPAGE_DESCR)+sizeof(SCSI_MODEPAGE_8));

	/* Make some checks to detect non-standard page layout. */

	mdlen = (UCHAR)(data[0] + 1u);		/* 'does not include itself' */
	page = (NPVOID)&data[sizeof(SCSI_MODEPAGE_HDR)
			    +sizeof(SCSI_MODEPAGE_DESCR)];

	if( (UCHAR)(page->pcode & 0x7F) != pgno ) /* correct page read? */
	{
	    sprintk(szMsgBuffer, szUnknFormat, (PSZ)szDriverName,
		    device->iAdapter, device->iUnit, pgno);
	    SaveMessage();
	    DEBMSG2("\r\nDumping complete buffer\r\n%z",(PVOID)data, mdlen);
	    rc = 2;
	    break;				/* skip this page */
	}


	/* If no change is requested: display current settings */

	if( pOption->pg8.access == 1  &&  (fDriverFlags & DF_VERBOSE) )
	{
	    sprintk(szMsgBuffer, szReadCache,
		    device->iAdapter, device->iUnit,
		    (PSZ)((page->cache & 0x01) != 0 ? "disabled" : "enabled"));
	    SaveMessage();

	    sprintk(szMsgBuffer, szWriteCache,
		    device->iAdapter, device->iUnit,
		    (PSZ)((page->cache & 0x04) == 0 ? "disabled" : "enabled"));
	    SaveMessage();

	    sprintk(szMsgBuffer, szPrefetchPrio,
		    device->iAdapter, device->iUnit,
		    (PSZ)((page->priority == 0) ? "disabled" : "enabled"));
	    SaveMessage();

	    sprintk(szMsgBuffer, szPrefetch,
		    device->iAdapter, device->iUnit,
		    (PSZ)((page->disprefetch[0] | page->disprefetch[1]) == 0
			  ? "disabled" : "enabled"));
	    SaveMessage();

	    break;				/* nothing more to do, exit */
	}


	/* Modify page according to 'pOption'. */

	if( pOption->pg8.rce == 1  &&  (page->cache & 0x01) == 0 )
	{
	    page->cache |= 0x01;		/* disable read cache */
	    ++modified;
	}
	else if( pOption->pg8.rce == 2  &&  (page->cache & 0x01) != 0 )
	{
	    page->cache &= ~0x01;		/* don't disable read cache */
	    ++modified;
	}

	if( pOption->pg8.wce == 1  &&  (page->cache & 0x04) != 0 )
	{
	    page->cache &= ~0x04;		/* don't enable write cache */
	    ++modified;
	}
	else if( pOption->pg8.wce == 2  &&  (page->cache & 0x04) == 0 )
	{
	    page->cache |= 0x04;		/* enable write cache */
	    ++modified;
	}

	if( pOption->pg8.prio == 1  &&  page->priority != 0 )
	{
	    page->priority = 0;			/* don't distinguish */
	    ++modified;
	}
	else if( pOption->pg8.prio == 2  &&  page->priority != 0x11 )
	{
	    page->priority = 0x11;		/* keep prefetched data longer */
	    ++modified;
	}

	if( pOption->pg8.pre == 1
	    &&  (page->disprefetch[0] | page->disprefetch[1]) != 0 )
	{
	    /* Disable all pre-fetches */

	    page->disprefetch[0] = 0;
	    page->disprefetch[1] = 0;		/* LSB */
	    page->minprefetch[0] = 0;
	    page->minprefetch[1] = 0;		/* LSB */
	    page->maxprefetch[0] = 0;
	    page->maxprefetch[1] = 0;		/* LSB */
	    page->prefetchceiling[0] = 0;
	    page->prefetchceiling[1] = 0;	/* LSB */
	    ++modified;
	}
	else if( pOption->pg8.pre == 2
		 &&  (page->disprefetch[0] | page->disprefetch[1]) == 0 )
	{
	    /* Enable pre-fetch but disable it
	     * on reads with more than 8KB data. */

	    page->cache &= ~0x02;		/* no multipliers, real counts */
	    page->disprefetch[0] = 0;
	    page->disprefetch[1] = 32;		/* LSB */
	    page->minprefetch[0] = 0;
	    page->minprefetch[1] = 1;		/* LSB */
	    page->maxprefetch[0] = 0;
	    page->maxprefetch[1] = 16;		/* LSB */
	    page->prefetchceiling[0] = 0;
	    page->prefetchceiling[1] = 16;	/* LSB */
	    ++modified;
	}

	if( modified > 0 )
	{
	    sprintk(szMsgBuffer, szWCachePage, device->iAdapter, device->iUnit);
	    DEBMSG1("\r\n%s",(PSZ)szMsgBuffer);
	    if( (fDriverFlags & DF_VERBOSE) )
		SaveMessage();

	    /* Write this control mode page via MODE SELECT(6)
	     * but don't let target save this permanently. */

	    page->pcode &= 0x3f;		/* mask reserved bits */
	    data[0] = 0;			/* reserved in MODE SELECT */
	    DEBMSG3("\r\nNew data for page %w\r\n%z",pgno,(PVOID)data,mdlen);
	    if( ModeSelect(device->pADDEntry, npUnit, pgno, data, mdlen) )
		rc = 4;
	}
    }
    while( 0 );


    /* Release all memory in inverted order. */

    npInitData -= MAX_MODE_DATA;

    return 0;
}




/*#
 * CALL
 *	ModifyControlPage(device,npUnit)
 * PARAMETER
 *	device		our own device structure
 *	npUnit		from GET_DEVICE_TABLE
 * RETURNS
 *	0		OK
 * GLOBAL
 *	fDriverFlags
 * DESPRIPTION
 *	Modifies Control Page (10).  Similar to ModifyCachePage().
 *
 * REMARKS
 */
PRIVATE int
ModifyControlPage(PDEVICE const device,NPUNITINFO const npUnit)
{
    NPBYTE	data;
    int		rc = 0;				/* error? */
    PUNIT_OPTION pOption = QueryUTable(device->iAdapter, device->iUnit);

    if( pOption == NULL  ||  pOption->pg10.access == 0 )
    {
	DEBMSG2("\r\nModifyControlPage: nothing requested for %w,%w", device->iAdapter,device->iUnit);
	return 0;				/* no change -> no error */
    }

    GETINITRAM(data, NPBYTE, MAX_MODE_DATA);

    sprintk(szMsgBuffer, szRControlPage, device->iAdapter, device->iUnit);
    DEBMSG1("\r\n%s",(PSZ)szMsgBuffer);
    if( (fDriverFlags & DF_VERBOSE) )
	SaveMessage();

    do
    {
	UCHAR			pgno = 0x0A;
	NPSCSI_MODEPAGE_A	page;
	UCHAR			mdlen;
	int			modified = 0;

	/* First: read mode page via MODE SENSE(6),
	 * read current values. */

	if( ModeSense(device->pADDEntry, npUnit, pgno, data, MAX_MODE_DATA) )
	{
	    rc = 1;
	    break;
	}
	DEBMSG3("\r\nOld page %w\r\n%z",pgno,(PVOID)data,sizeof(SCSI_MODEPAGE_HDR)
		+sizeof(SCSI_MODEPAGE_DESCR)+sizeof(SCSI_MODEPAGE_A));


	mdlen = (UCHAR)(data[0] + 1u);		/* 'does not include itself' */
	page = (NPVOID)&data[sizeof(SCSI_MODEPAGE_HDR)
			    +sizeof(SCSI_MODEPAGE_DESCR)];

	if( (page->pcode & 0x7F) != pgno )
	{
	    sprintk(szMsgBuffer, szUnknFormat, (PSZ)szDriverName,
		    device->iAdapter, device->iUnit, pgno);
	    SaveMessage();
	    DEBMSG2("\r\nDumping complete buffer\r\n%z",(PVOID)data, mdlen);
	    rc = 2;
	    break;				/* skip this page */
	}


	/* If no change is requested: display current settings */

	if( pOption->pg10.access == 1  &&  (fDriverFlags & DF_VERBOSE) )
	{
	    sprintk(szMsgBuffer, szTaggedQueue,
		    device->iAdapter, device->iUnit,
		    (PSZ)((page->queuing & 0x01) != 0
			  ? "disabled" : "enabled"));
	    SaveMessage();

	    sprintk(szMsgBuffer, szReorder,
		    device->iAdapter, device->iUnit,
		    (PSZ)((page->queuing & 0xF0) != 0x10
			  ? "disabled" : "enabled"));
	    SaveMessage();

	    break;				/* nothing more to do, exit */
	}


	/* Modify page according to 'pOption'. */

	if( pOption->pg10.queue == 1  &&  (page->queuing & 0x01) == 0 )
	{
	    page->queuing |= 0x01;		/* disable tagged queuing */
	    ++modified;
	}
	else if( pOption->pg10.queue == 2  &&  (page->queuing & 0x01) != 0 )
	{
	    page->queuing &= ~0x01;		/* don't disable TQ */
	    ++modified;
	}

	if( pOption->pg10.reorder == 1  &&  (page->queuing & 0xF0) == 0x10 )
	{
	    page->queuing &= 0x0F;		/* 'restricted reordering */
	    ++modified;
	}
	else if(pOption->pg10.reorder == 2 && (page->queuing & 0xF0) != 0x10)
	{
	    page->queuing &= 0x0F;
	    page->queuing |= 0x10;		/* 'unrestricted reordering' */
	    ++modified;
	}


	if( modified > 0 )
	{
	    sprintk(szMsgBuffer,szWControlPage, device->iAdapter, device->iUnit);
	    DEBMSG1("\r\n%s",(PSZ)szMsgBuffer);
	    if( (fDriverFlags & DF_VERBOSE) )
		SaveMessage();

	    /* Write this control mode page via MODE SELECT(6)
	     * but don't let target save this permanently. */

	    page->pcode &= 0x3f;		/* mask reserved bits */
	    data[0] = 0;			/* reserved in MODE SELECT */
	    DEBMSG3("\r\nNew page %w\r\n%z",pgno,(PVOID)data,mdlen);
	    if( ModeSelect(device->pADDEntry, npUnit, pgno, data, mdlen) )
		rc = 4;
	}
    }
    while( 0 );


    /* Release all memory in inverted order. */

    npInitData -= MAX_MODE_DATA;

    return 0;
}




/*#
 * CALL
 *	ModifyConnectPage(device,npUnit)
 * PARAMETER
 *	device		our own device structure
 *	npUnit		from GET_DEVICE_TABLE
 * RETURNS
 *	0		OK
 * GLOBAL
 *	fDriverFlags
 * DESPRIPTION
 *	See ModifiyCachePage()
 *
 * REMARKS
 */
PRIVATE int
ModifyConnectPage(PDEVICE const device,NPUNITINFO const npUnit)
{
    NPBYTE	data;
    int		rc = 0;				/* error? */
    PUNIT_OPTION pOption = QueryUTable(device->iAdapter, device->iUnit);

    if( pOption == NULL  ||  pOption->pg2.access == 0 )
    {
	DEBMSG2("\r\nModifyConnectPage: nothing requested for %w,%w", device->iAdapter,device->iUnit);
	return 0;				/* no change -> no error */
    }

    GETINITRAM(data, NPBYTE, MAX_MODE_DATA);

    sprintk(szMsgBuffer, szRConnectPage, device->iAdapter, device->iUnit);
    DEBMSG1("\r\n%s",(PSZ)szMsgBuffer);
    if( (fDriverFlags & DF_VERBOSE) )
	SaveMessage();

    do
    {
	UCHAR			pgno = 0x02;
	NPSCSI_MODEPAGE_2	page;
	UCHAR			mdlen;
	int			modified = 0;

	/* First: read mode page via MODE SENSE(6),
	 * read current values. */

	if( ModeSense(device->pADDEntry, npUnit, pgno, data, MAX_MODE_DATA) )
	{
	    rc = 1;
	    break;
	}
	DEBMSG3("\r\nOld page %w\r\n%z",pgno,(PVOID)data,sizeof(SCSI_MODEPAGE_HDR)
		+sizeof(SCSI_MODEPAGE_DESCR)+sizeof(SCSI_MODEPAGE_2));


	mdlen = (UCHAR)(data[0] + 1u);		/* 'does not include itself' */
	page = (NPVOID)&data[sizeof(SCSI_MODEPAGE_HDR)
			    +sizeof(SCSI_MODEPAGE_DESCR)];

	if( (page->pcode & 0x7F) != pgno )
	{
	    sprintk(szMsgBuffer, szUnknFormat, (PSZ)szDriverName,
		    device->iAdapter, device->iUnit, pgno);
	    SaveMessage();
	    DEBMSG2("\r\nDumping complete buffer\r\n%z",(PVOID)data, mdlen);
	    rc = 2;
	    break;				/* skip this page */
	}


	/* If no change is requested: display current settings */

	if( pOption->pg2.access == 1  &&  (fDriverFlags & DF_VERBOSE) )
	{
	    sprintk(szMsgBuffer, szFullReselect,
		    device->iAdapter, device->iUnit,
		    (PSZ)((page->bfull | page->bempty) == 0
			  ? "disabled" : "enabled"));
	    SaveMessage();

	    sprintk(szMsgBuffer, szInactiveTO,
		    device->iAdapter, device->iUnit,
		    (PSZ)((page->businactive[0] |  page->businactive[1]) == 0
			  ? "disabled" : "enabled"));
	    SaveMessage();

	    sprintk(szMsgBuffer, szDisconnectTO,
		    device->iAdapter, device->iUnit,
		    (PSZ)((page->disconnect[0] |  page->disconnect[1]) == 0
			  ? "disabled" : "enabled"));
	    SaveMessage();

	    sprintk(szMsgBuffer, szConnectTO,
		    device->iAdapter, device->iUnit,
		    (PSZ)((page->connect[0] |  page->connect[1]) == 0
			  ? "disabled" : "enabled"));
	    SaveMessage();

	    break;				/* nothing more to do, exit */
	}


	/* Modify page according to 'pOption'. */

	if( pOption->pg2.buffer == 1
	    &&  (page->bfull | page->bempty) != 0 )
	{
	    /* no disconnect because of full/empty buffers */

	    page->bfull = 0;
	    page->bempty = 0;
	    ++modified;
	}
	else if( pOption->pg2.buffer == 2
		 &&  (page->bfull | page->bempty) == 0 )
	{
	    /* disconnect because of full/empty buffers */

	    page->bfull = 40;			/* ~15% */
	    page->bempty = 40;
	    ++modified;
	}


	if( pOption->pg2.inactive == 1
	    &&  (page->businactive[0] |  page->businactive[1]) != 0 )
	{
	    /* no limit on bus busy time w/o connection */

	    page->businactive[0] = page->businactive[1] = 0;
	    ++modified;
	}
	else if(pOption->pg2.inactive == 2
		&&  (page->businactive[0] |  page->businactive[1]) == 0 )
	{
	    /* don't keep bus busy w/o connection */

	    page->businactive[0] = 0;
	    page->businactive[1] = 200;		/* LSB */
	    ++modified;
	}

	if( pOption->pg2.disconnect == 1
	    &&  (page->disconnect[0] |  page->disconnect[1]) != 0 )
	{
	    /* no limit on bus disconnect state */

	    page->disconnect[0] = page->disconnect[1] = 0;
	    ++modified;
	}
	else if( pOption->pg2.disconnect == 2
		 &&  (page->disconnect[0] |  page->disconnect[1]) == 0 )
	{
	    /* don't keep disconnect state forever */

	    page->disconnect[0] = 0;
	    page->disconnect[1] = 200;		/* LSB */
	    ++modified;
	}

	if( pOption->pg2.connect == 1
	    &&  (page->connect[0] |  page->connect[1]) != 0 )
	{
	    /* no limit on bus connect state */

	    page->connect[0] = page->connect[1] = 0;
	    ++modified;
	}
	else if( pOption->pg2.connect == 2
	    &&  (page->connect[0] |  page->connect[1]) == 0 )
	{
	    /* don't keep connect state forever */

	    page->connect[0] = 0;
	    page->connect[1] = 200;		/* LSB */
	    ++modified;
	}


	if( modified > 0 )
	{
	    sprintk(szMsgBuffer,szWConnectPage, device->iAdapter, device->iUnit);
	    DEBMSG1("\r\n%s",(PSZ)szMsgBuffer);
	    if( (fDriverFlags & DF_VERBOSE) )
		SaveMessage();

	    /* Write this control mode page via MODE SELECT(6)
	     * but don't let target save this permanently. */

	    page->pcode &= 0x3f;		/* mask reserved bits */
	    data[0] = 0;			/* reserved in MODE SELECT */
	    DEBMSG3("\r\nNew page %w\r\n%z",pgno,(PVOID)data,mdlen);
	    if( ModeSelect(device->pADDEntry, npUnit, pgno, data, mdlen) )
		rc = 4;
	}
    }
    while( 0 );


    /* Release all memory in inverted order. */

    npInitData -= MAX_MODE_DATA;

    return 0;
}




/*#
 * CALL
 *	ModifyCdromPage(device,npUnit)
 * PARAMETER
 *	device		our own device structure
 *	npUnit		from GET_DEVICE_TABLE
 * RETURNS
 *	0		OK
 * GLOBAL
 *	fDriverFlags
 * DESPRIPTION
 *	Modifies CDROM Control Page (13).
 *
 * REMARKS
 */
PRIVATE int
ModifyCdromPage(PDEVICE const device,NPUNITINFO const npUnit)
{
    NPBYTE	data;
    int		rc = 0;				/* error? */
    PUNIT_OPTION pOption = QueryUTable(device->iAdapter, device->iUnit);

    if( pOption == NULL  ||  pOption->pg13.access == 0 )
    {
	DEBMSG2("\r\nModifyCdromPage: nothing requested for %w,%w", device->iAdapter,device->iUnit);
	return 0;				/* no change -> no error */
    }

    GETINITRAM(data, NPBYTE, MAX_MODE_DATA);

    sprintk(szMsgBuffer, szRCDPage, device->iAdapter, device->iUnit);
    DEBMSG1("\r\n%s",(PSZ)szMsgBuffer);
    if( (fDriverFlags & DF_VERBOSE) )
	SaveMessage();

    do
    {
	UCHAR			pgno = 0x0D;
	NPSCSI_MODEPAGE_D	page;
	UCHAR			mdlen;
	int			modified = 0;

	/* First: read mode page via MODE SENSE(6),
	 * read current values. */

	if( ModeSense(device->pADDEntry, npUnit, pgno, data, MAX_MODE_DATA) )
	{
	    rc = 1;
	    break;
	}
	DEBMSG3("\r\nOld page %w\r\n%z",pgno,(PVOID)data,sizeof(SCSI_MODEPAGE_HDR)
		+sizeof(SCSI_MODEPAGE_DESCR)+sizeof(SCSI_MODEPAGE_D));


	mdlen = (UCHAR)(data[0] + 1u);		/* 'does not include itself' */
	page = (NPVOID)&data[sizeof(SCSI_MODEPAGE_HDR)
			    +sizeof(SCSI_MODEPAGE_DESCR)];

	if( (page->pcode & 0x7F) != pgno )
	{
	    sprintk(szMsgBuffer, szUnknFormat, (PSZ)szDriverName,
		    device->iAdapter, device->iUnit, pgno);
	    SaveMessage();
	    DEBMSG2("\r\nDumping complete buffer\r\n%z",(PVOID)data, mdlen);
	    rc = 2;
	    break;				/* skip this page */
	}


	/* If no change is requested: display current settings */

	if( pOption->pg13.access == 1  &&  (fDriverFlags & DF_VERBOSE) )
	{
	    char * cp;

	    switch( page->multiplier )
	    {
	      case 0x01:
		cp = "125 ms";
		break;
	      case 0x02:
		cp = "250 ms";
		break;
	      case 0x03:
		cp = "500 ms";
		break;
	      case 0x04:
		cp = "1 s";
		break;
	      case 0x05:
		cp = "2 s";
		break;
	      case 0x06:
		cp = "4 s";
		break;
	      case 0x07:
		cp = "8 s";
		break;
	      case 0x08:
		cp = "16 s";
		break;
	      case 0x09:
		cp = "32 s";
		break;
	      case 0x0A:
		cp = "60 s";
		break;
	      case 0x0B:
		cp = "120 s";
		break;
	      case 0x0C:
		cp = "240 s";
		break;
	      case 0x0D:
		cp = "480 s";
		break;
	      case 0x0E:
		cp = "960 s";
		break;
	      case 0x0F:
		cp = "1920 s";
		break;

	      case 0:
	      default:
		cp = "unknown";
		break;
	    }

	    sprintk(szMsgBuffer, szCDTimeout, device->iAdapter, device->iUnit, cp);
	    SaveMessage();
	    break;				/* nothing more to do, exit */
	}


	/* Modify page according to 'pOption'. */

	page->multiplier &= 0xF0;		/* clear low nibble */
	++modified;				/* always modified if reached */
	if( pOption->pg13.seconds <= 1 )
	    page->multiplier |= 0x04;
	else if( pOption->pg13.seconds <= 2 )
	    page->multiplier |= 0x05;
	else if( pOption->pg13.seconds <= 4 )
	    page->multiplier |= 0x06;
	else if( pOption->pg13.seconds <= 8 )
	    page->multiplier |= 0x07;
	else if( pOption->pg13.seconds <= 16 )
	    page->multiplier |= 0x08;
	else if( pOption->pg13.seconds <= 32 )
	    page->multiplier |= 0x09;
	else if( pOption->pg13.seconds <= 60 )
	    page->multiplier |= 0x0A;
	else if( pOption->pg13.seconds <= 120 )
	    page->multiplier |= 0x0B;
	else if( pOption->pg13.seconds <= 240 )
	    page->multiplier |= 0x0C;
	else if( pOption->pg13.seconds <= 480 )
	    page->multiplier |= 0x0D;
	else if( pOption->pg13.seconds <= 960 )
	    page->multiplier |= 0x0E;
	else
	    page->multiplier |= 0x0F;		/* else maximum */

	if( modified > 0 )
	{
	    sprintk(szMsgBuffer,szWCDPage, device->iAdapter, device->iUnit);
	    DEBMSG1("\r\n%s",(PSZ)szMsgBuffer);
	    if( (fDriverFlags & DF_VERBOSE) )
		SaveMessage();

	    /* Write this mode page via MODE SELECT(6)
	     * but don't let target save this permanently. */

	    page->pcode &= 0x3f;		/* mask reserved bits */
	    data[0] = 0;			/* reserved in MODE SELECT */
	    DEBMSG3("\r\nNew page %w\r\n%z",pgno,(PVOID)data,mdlen);
	    if( ModeSelect(device->pADDEntry, npUnit, pgno, data, mdlen) )
		rc = 4;
	}
    }
    while( 0 );


    /* Release all memory in inverted order. */

    npInitData -= MAX_MODE_DATA;

    return 0;
}




/*#
 * CALL
 *	ModifyDevice(device,npUnit)
 * PARAMETER
 *	device		DEVICE structure
 *	npUnit
 * RETURNS
 *	0		OK
 *	/0		error
 * GLOBAL
 * DESPRIPTION
 *	Modify performance-related mode pages.
 *
 * REMARKS
 */
PRIVATE int NEAR
ModifyDevice(PDEVICE const device,NPUNITINFO const npUnit)
{
    int rc = 0;

    sprintk(szMsgBuffer, szModifyStart, device->iAdapter, device->iUnit);
    DEBMSG1("\r\n%s",(PSZ)szMsgBuffer);
    if( (fDriverFlags & DF_DEBUG) )
	SaveMessage();

    /* We need access to a device before issuing commands
     * to it.  So send a request to ALLOCATE UNIT */

    {
	NPIORB_UNIT_CONTROL pIOUC = (NPVOID)npInitData;

	memset(pIOUC, 0, sizeof(IORB_UNIT_CONTROL));
	pIOUC->iorbh.Length =		sizeof(IORB_UNIT_CONTROL);
	pIOUC->iorbh.UnitHandle =	npUnit->UnitHandle;
	pIOUC->iorbh.CommandCode =	IOCC_UNIT_CONTROL;
	pIOUC->iorbh.CommandModifier =	IOCM_ALLOCATE_UNIT;

	if( SendIORB((PIORB)pIOUC, device->pADDEntry) )
	{
	    sprintk(szMsgBuffer, szAllocFailed, (PSZ)szDriverName,
		    device->iAdapter, device->iUnit);
	    SaveMessage();
	    return 1;
	}
    }

    /* Modify pages... */

    rc = ModifyCachePage(device, npUnit);
    rc |= ModifyControlPage(device, npUnit);
    rc |= ModifyConnectPage(device, npUnit);
    rc |= ModifyCdromPage(device, npUnit);

    /* All done, so release access to this unit. */

    {
	NPIORB_UNIT_CONTROL pIOUC = (NPVOID)npInitData;

	/* Send request to DEALLOCATE UNIT */

	memset( pIOUC, 0, sizeof(IORB_UNIT_CONTROL) );
	pIOUC->iorbh.Length =		sizeof(IORB_UNIT_CONTROL);
	pIOUC->iorbh.UnitHandle =	npUnit->UnitHandle;
	pIOUC->iorbh.CommandCode =	IOCC_UNIT_CONTROL;
	pIOUC->iorbh.CommandModifier =	IOCM_DEALLOCATE_UNIT;

	if( SendIORB((PIORB)pIOUC, device->pADDEntry) )
	{
	    /* Couldn't deallocte unit.
	     * Bad, display and ignore */

	    sprintk(szMsgBuffer, szDeallocFailed, (PSZ)szDriverName,
		    device->iAdapter, device->iUnit);
	    SaveMessage();
	}
    }

    if( rc == 0 )
    {
	sprintk(szMsgBuffer, szModifyOK, device->iAdapter, device->iUnit);
	DEBMSG1("\r\n%s",(PSZ)szMsgBuffer);
	if( (fDriverFlags & DF_DEBUG) )
	    SaveMessage();
    }
    return rc;
}





/*#
 * CALL
 *	ScanDevices(void)
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
 *	Scans all adapters on all ADDs for SCSI devices and
 *	calls ModifyDevice() for each.
 *
 * REMARKS
 */
PRIVATE int
ScanDevices(void)
{
    USHORT	j, k;
    USHORT	changed = 0;
    int		fatal = 0;

    USHORT	add_count;
    USHORT	add_index;
    USHORT	adp = 0;

    void	(FAR _Cdecl * pADDBaseEP)();

    struct DevClassTableEntry FAR *pDCE;
    struct DevClassTableStruc FAR *pDCT;


    DEBMSG("\r\nScanDevices");

    /* Get address of Registered ADD Table */

    if( DevHelp_GetDOSVar(DHGETDOSV_DEVICECLASSTABLE, DRIVERCLASS_ADD, &pDCT) )
    {
	/* Should never happen.  What can we do? */

	sprintk(szMsgBuffer, szNoDriverTab, (PSZ)szDriverName);
	SaveMessage();
	return -1;
    }

    add_count = pDCT->DCCount;


    /* For each ADD registered  */

    DEBMSG("\r\nLooking for ADDs");
    for( pDCE = pDCT->DCTableEntries, add_index = 0;
	 add_index < add_count;
	 ++add_index, ++pDCE )
    {
	{
	    NPIORB_CONFIGURATION pIOCF = (NPVOID)npInitData;

	    /* Get ADD's Device Table */

	    memset( pIOCF, 0, sizeof(IORB_CONFIGURATION) );
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
		 * ADD.  Just ignore that ADD (which probably hasn't
		 * any adapters/devices, either). */

		sprintk(szMsgBuffer, szNoDevTab, (PSZ)szDriverName, add_index);
		SaveMessage();
		continue;
	    }
	}


	/* For each Adapter in ADD's Device Table */

	for( j = 0; j < ((PDEVICETABLE)DeviceTable)->TotalAdapters; ++j, ++adp )
        {
	    NPADAPTERINFO const npAdapterInfo
		= ((PDEVICETABLE)DeviceTable)->pAdapter[j];
	    NPUNITINFO		npUnitInfo;


	    if( CheckAdapterInfo(adp, npAdapterInfo) )
		continue;

	    /* Access UNITINFO for each Device on adapter */

	    DEBMSG("\r\nLooking for Units");
	    for( npUnitInfo = npAdapterInfo->UnitInfo, k = 0;
		 k < npAdapterInfo->AdapterUnits;
		 ++k, ++npUnitInfo )
            {
		DEVICE	dev;			/* local structure, not
						   global table as we aren't
						   a real filter. */

		/* If a Filter ADD has registered for this unit
		 * then send subsequent requests to the Filter ADD
		 * and not to the ADD. */

		if( (dev.hdFilter=npUnitInfo->FilterADDHandle) != 0 )
                {
		    struct DevClassTableEntry FAR *pDCEFilter;

		    DEBMSG("\r\nFound other filter for unit");
		    pDCEFilter = &pDCT->DCTableEntries[dev.hdFilter-1];

		    dev.pADDEntry = (PVOID)MAKEP(pDCEFilter->DCSelector,
						 pDCEFilter->DCOffset);
                }
		else
                {
		    dev.pADDEntry = pADDBaseEP;
                }
		dev.iAdapter = adp;
		dev.iUnit = npUnitInfo->UnitIndex;

		if( CheckUnitInfo(&dev, npUnitInfo) )
		    continue;

		sprintk(szMsgBuffer, szFoundDevice, dev.iAdapter, dev.iUnit);
		DEBMSG1("\r\n%s",(PSZ)szMsgBuffer);
		if( (fDriverFlags & DF_VERBOSE) )
		    SaveMessage();

		if( ModifyDevice(&dev, npUnitInfo) == 0 )
		    ++changed;

	    } /* for each unit */
	} /* for each adapter */
    } /* for each ADD */

    if( fatal )					/* errors occurred? */
    {
	return -1;				/* than display and wait */
    }
    if( changed == 0 )
    {
	sprintk(szMsgBuffer, szNoDevices);
	DEBMSG1("\r\n%s",(PSZ)szMsgBuffer);
	if( (fDriverFlags & DF_VERBOSE) )
	    SaveMessage();
	return 1;				/* non-fatal error,
						   don't install */
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
#define TOK_ID_EF	100
#define TOK_ID_DF	101

OPT OPT_VERBOSE =
    {TOK_ID_VERBOSE,0,	"/V", TYPE_0,		{0,  E,  E}};

OPT OPT_DEBUG_NO =
    {TOK_ID_DEBUG,0,	"/DEBUG:", TYPE_D,	{0,  E,  E}};

OPT OPT_DEBUG_DEFAULT =
    {TOK_ID_DEBUG,0,	"/DEBUG", TYPE_0,	{0,  E,  E}};

OPT OPT_ADAPTER =
    {TOK_ID_ADAPTER,0,	"/A:", TYPE_D,		{1,  E,  1}};

OPT OPT_ENABLE =
    {TOK_ID_EF,0,	"/E:", TYPE_GEOMETRY,	{E,  2,  2}};

OPT OPT_DISABLE =
    {TOK_ID_DF,0,	"/!E:", TYPE_GEOMETRY,	{E,  2,  2}};

OPT OPT_END =
    {TOK_ID_END,0,	"\0", TYPE_0,		{O,  O,  O}};


/*
 *   Please note the importance of the ordering of entries in poption[]
 *   For example: the pointer to the option "/DM:" must come before the
 *   option "/DM"
 */
OPTIONTABLE  opttable = {
    ENTRY_STATE, MAX_STATES,
    {
	&OPT_VERBOSE,
	&OPT_DEBUG_NO,
	&OPT_DEBUG_DEFAULT,
	&OPT_ADAPTER,
	&OPT_ENABLE,
	&OPT_DISABLE,
	&OPT_END
    }
};




/*#
 * CALL
 *	AnalyseCmdLine(pszLine)
 * PARAMETER
 *	pszLine		pointer to command line
 * RETURNS
 *	0		OK
 * GLOBAL
 * DESPRIPTION
 *	Invoke parser and analyse token output.
 *
 * REMARKS
 */
PRIVATE int NEAR
AnalyseCmdLine(PSZ const pszLine)
{
    CC		parse_rc;
    PCHAR	tp;
    CHAR	TokenId;
    int		i;
    USHORT	adapter = 0xFFFF;


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

    /* Let 'tp' jump from token to token. */

    for( tp = DeviceTable; (TokenId=*(tp+TOKL_ID)) != TOK_ID_END;
	 tp += *(tp+TOKL_LEN) )
    {
	switch( TokenId )
	{
	  case TOK_ID_VERBOSE:
	    DEBMSG("\r\n/VERBOSE");
	    fDriverFlags |= DF_VERBOSE;
	    break;

	  case TOK_ID_DEBUG:
	    DEBMSG("\r\n/DEBUG");
	    fDriverFlags |= DF_VERBOSE;
	    fDriverFlags |= DF_DEBUG;

	    if( *(tp+TOKL_LEN) == TOK_MIN_LENGTH+1 )
	    {
		UCHAR code = *(tp+TOKL_VALUE);

		switch( code )
		{
		  default:
		    sprintk(szMsgBuffer, szUnknownOption, 
			    (PSZ)szDriverName, (PBYTE)DeviceTable);
		    SaveMessage();
		    break;
		}
	    }
	    break;

	  case TOK_ID_ADAPTER:
	    adapter = *(tp+TOKL_VALUE);
	    DEBMSG1("\r\n/A:%w",adapter);
	    break;

	  case TOK_ID_EF:
	  case TOK_ID_DF:
	    /* Loop through every coordinate.patch provided */
	    for( i = 0; i < *(tp+TOKL_LEN)-2; i += 6 )
	    {
		PCHAR	cp = tp + TOKL_VALUE + i;

		DEBMSG3("\r\n/[!]E:(%w,%w,%w)",*(PUSHORT)(cp),*(PUSHORT)(cp+2),*(PUSHORT)(cp+4));

		AddUTable(adapter,			/* adapter index */
			  *(PUSHORT)(cp),		/* unit index */
			  *(PUSHORT)(cp+2),		/* page number */
			  (TokenId == TOK_ID_EF ? 1 : 0),
			  *(PUSHORT)(cp+4));		/* mask */
	    }
	    break;

	  case TOK_ID_END:
	    break;

	  default:
	    break;
	}
    }

    return 0;
} /*end[AnalyseCmdLine]*/





/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */


/*#
 * CALL
 *	InitBase(pRPI)
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
 *
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

    DEBMSG("\r\nInitBase");
    DBSTOP();

    /* Contains entry point to kernel functions */

    Device_Help = pRPI->DevHlpEP;
    pCmdLine	= MK_FP(FP_SEL(pInitParms),pInitParms->cmd_line_args);

    if( DevHelp_VirtToPhys(&DiskDDHeader, &ppDiskDDHeader) )
    {
	sprintk(szMsgBuffer, szErrorVAddr, (PSZ)szDriverName);
	SaveMessage();
	rc = -1;
	goto ERROR_EXIT;			/* !!! */
    }

    DevHelp_GetDOSVar(DHGETDOSV_SYSINFOSEG, 0, (PPVOID)&pInfoSegSel);
    pGlobalInfoSeg = MAKEP(*pInfoSegSel, 0);


    /* Parse Command Line Args */

    AnalyseCmdLine(pCmdLine);

    if( (fDriverFlags & DF_VERBOSE) )
    {
	sprintk(szMsgBuffer, szLogo, (PSZ)szProduct, VERSION, SUBVERSION);
	SaveMessage();
    }

    rc = ScanDevices();


    /* Processing complete.  Analyse situation, should we install,
     * quiet fail or even loudly stop Config.Sys processing? */

    if( rc == 0 )
    {
	/* We do not need to register any units, since this
	 * filter does not generate any units on his own. */

	pRPO->Unit    = 0;
	pRPO->CodeEnd = ((USHORT)Code_End & ~0x0F) + 0x10;
	pRPO->DataEnd = ((USHORT)npInitData & ~0x0F) + 0x10;

	DEBMSG("\r\nDriver installed");
	fDriverFlags |= DF_INITDONE;
	return STATUS_DONE;
    }
    else
    {
      ERROR_EXIT:				/* goto destination */
	pRPO->Unit    = 0;
	pRPO->CodeEnd = 0;
	pRPO->DataEnd = 0;

	if( rc < 0 )
	    return STERR | STDON | ERROR_I24_GEN_FAILURE;
	else
	    return STERR | STDON | ERROR_I24_QUIET_INIT_FAIL;
    }
}



/* History:
 *
 * $Log: init1.c,v $
 * Revision 1.10  2000/06/12 02:34:00  vitus
 * - DevHelp_ProcBlock now interruptable
 *
 * Revision 1.9  1998/03/04 02:11:28  vitus
 * - added option and code to modify mode page 13 (inact. timeout)
 *
 * Revision 1.8  1997/12/16 02:48:09  vitus
 * - renamed source files
 * - clarified messages
 *
 * Revision 1.7  1997/12/11 01:43:44  vitus
 * - displays current settings if no change requested
 * - uses external SaveMessages() - keep messages in swappable memory
 *
 * Revision 1.6  1997/12/08 23:10:24  vitus
 * - INITMSG define
 * - expected messages don't display driver name
 * - AddUTable/QueryUTable: adjusted for WatCom's type conversion
 * - added messages before writing the new page contents
 * - new OPT structure layout
 *
 * Revision 1.5  1997/12/06 00:37:13  vitus
 * - enabled support for WatCom C
 * - switched to internal _fxxxxx functions
 *
 * Revision 1.4  1997/11/12 00:40:30  vitus
 * - enable/disable options are saved per page
 *
 * Revision 1.3  1997/11/10 01:15:37  vitus
 * - ModifyXxxxx function does MODE SENSE if device in device table,
 *   but doesn't do a MODE SELECT if no changes requested
 *
 * Revision 1.2  1997/11/05 00:49:57  vitus
 * - seperate functions 'ModifyXxxxxPage(device, npUnit)'
 *
 * Revision 1.1  1997/10/28 02:06:59  vitus
 * Initial revision
 */
