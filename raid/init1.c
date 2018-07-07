/*
 * $Source: r:/source/driver/raid/RCS/init1.c,v $
 * $Revision: 4.1 $
 * $Date: 2005/06/17 05:54:29 $
 * $Locker:  $
 *
 *      First step of initialization, executed at ring 0 in protected
 *      mode as the only running process.
 *
 * History:
 * $Log: init1.c,v $
 * Revision 4.1  2005/06/17 05:54:29  vitus
 * - added New BSD license statement
 * - removed tabs
 *
 * Revision 3.8  2003/11/14 00:35:27  vitus
 * - NORAID5: prohibits calls to Raid5Create().  To build old versions...
 *
 * Revision 3.7  2001/09/03 02:56:33  vitus
 * - corrected setting of AF_NOT16M flag
 * - calls Raid5Create()
 *
 * Revision 3.6  2001/08/03 02:19:03  vitus
 * - update fADDFlags with ADD specific restrictions
 *
 * Revision 3.5  2001/07/22 00:32:01  vitus
 * - allocates another context hook which executes UpdateBuilds()
 *
 * Revision 3.4  2001/06/25 00:28:38  vitus
 * - allocates context hook which executes MemoryHook()
 *
 * Revision 3.3  2000/04/09 12:24:22  vitus
 * - handles VRAID partitions inside extended partitions
 * - handles /[!]SHARE and doesn't install router if not requested
 *
 * Revision 3.2  1999/12/14 00:35:49  vitus
 * - added ValidateVSec2() from mirror.c
 *
 * Revision 3.1  1999/09/23 23:06:47  vitus
 * - defined all DBGMSG() strings as located in CONST segment
 * - implemented InstallRouter(), installs a BASEDISK for every PHYSDEVICE found
 * - all shortterm memory is now allocated via routines from memory.c.  We
 *   had a memory-overwrite problem (in inter-release versions)
 *
 * Revision 2.5  1999/06/22 00:06:01  vitus
 * - calls ReleaseWorkmem() when initialization failed (hangs the machine w/o)
 *
 * Revision 2.4  1999/06/15 22:43:29  vitus
 * - BuildDevTree: changed the way device pointers are temporaly saved.  It was
 *   impossible to create a parent from child just created in the same loop.
 *   Switching from two to a single table made it possible.
 *
 * Revision 2.3  1999/06/06 23:34:42  vitus
 * - changed key calculation to use XOR and include previous characters
 * - checks and display demo keys
 *
 * Revision 2.2  1999/05/26 00:40:22  vitus
 * - BuildDevTee: AnalyseVRDev[2] and *Create() return/handle 0 (OK), >0 (creation
 *   deferred) and <0 (error) values.
 *
 * Revision 2.1  1999/05/24 19:23:05  vitus
 * - *Create() may return 0 (OK), >0 (quiet fail) or <0 (load fail)
 *
 * Revision 1.12  1999/05/24 00:46:29  vitus
 * - ignore any disks which contain a VRAID partition but no PHYSDEVICE sector.  User
 *   may try to destroy any VRAID traces of those drives.
 *
 * Revision 1.11  1999/05/11 23:48:19  vitus
 * - moved CRC code to memory.c
 *
 * Revision 1.10  1999/04/21 00:29:16  vitus
 * - switched from PhysdevPartIO() and offset calculation
 *   to ProcessAdminIO() and offset 1 (fix!)
 *
 * Revision 1.9  1999/01/22 02:53:49  vitus
 * - added support if VRAIDEVICE2 sectors (preliminary)
 *
 * Revision 1.8  1998/12/13 23:02:24  vitus
 * - CheckReqistration: now sets global variable 'uRegLevel' (1-3)
 * - changes for removed elements from HOSTDRIVE (flags, vrd, size)
 *
 * Revision 1.7  1998/11/05 22:40:49  vitus
 * - addred registration code
 * - LocalSendIORB: check ProcBlock return code
 *
 * Revision 1.6  1998/09/20 15:18:29  vitus
 * - modified for AllocateVrio(working_dev,mother)
 * - added timebombed code
 * - collects minimum of MaxHWSGList and MaxCDBTransferLength
 *
 * Revision 1.5  1998/06/26 22:33:14  vitus
 * - switched to DDK macros (cli, sti, pushf)
 * - first scanning now build PHYSDEVICE devices (VerifyDiskData checks
 *   SEC_PHYSDEV) and BuildDevTree uses PhysdevPartIO() on that devices.
 *
 * Revision 1.4  1998/05/29 01:42:58  vitus
 * - changed all INFMSG to DEBMSG(DBG_LEVEL4,...)
 *
 * Revision 1.3  1998/04/08 01:09:03  vitus
 * - added CRC16 calculation
 * - only excepts UIB_TYPE_DISK
 * - corrected IORB handling when using created PHYSDEVICEs
 * - corrected adapter index calculation
 * - moved initialization of memory routines
 * - test successfull (single, chain)
 *
 * Revision 1.2  1998/03/10 02:27:06  vitus
 * - BuildDevTree: calls PUBLIC *Create() functions per device type
 *
 * Revision 1.1  1998/03/06 01:25:12  vitus
 * Initial revision
 * ---------------------------------------------
 *      Copyright (c) 1998-2001,2005, Vitus Jensen
 *      All rights reserved.
 *
 *      Redistribution and use in source and binary forms, with or without
 *      modification, are permitted provided that the following conditions are
 *      met:
 *
 *          * Redistributions of source code must retain the above copyright
 *            notice, this list of conditions and the following disclaimer.
 *          * Redistributions in binary form must reproduce the above copyright
 *            notice, this list of conditions and the following disclaimer in
 *            the documentation and/or other materials provided with the
 *            distribution.
 *          * Neither the name of Vitus Jensen nor the names of its
 *            contributors may be used to endorse or promote products derived
 *            from this software without specific prior written permission.
 *
 *      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *      "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *      LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *      A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *      OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *      SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *      LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *      DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *      THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *      (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *      OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#if defined(__WATCOMC__)
# pragma off (unreferenced)
#endif
static char const id[]="$Id: init1.c,v 4.1 2005/06/17 05:54:29 vitus Exp $";
#if defined(__WATCOMC__)
# pragma on (unreferenced)
#endif

#include <string.h>

#define INCL_DOSERRORS
#define INCL_INITRP_ONLY
#include "os2intf.h"
#include "cmdphdr.h"

#include "vraid.h"
#include "proto.h"
#include "extern.h"

extern PASCAL NEAR MemoryHook();
extern PASCAL NEAR BuildHook();


/* locally used error codes */

#define ERR_NO_MEM              ((USHORT)-1)
#define ERR_NO_VRAIDPART        ((USHORT)-2)
#define ERR_BAD_PHYSDEV         ((USHORT)-3)
#define ERR_XLATION_FAIL        ((USHORT)-4)
#define ERR_BAD_REGLEVEL        ((USHORT)-5)
#define ERR_UNKNOWN_RTYPE       ((USHORT)-6)





/*
 * Macro to allocate *permanent* data from memory pool.
 * OBS: to free this memory decrement 'npInitData' by the amount
 * allocated.  Be sure to undo it inverse.
 */
#define GETINITRAM(p,type,size) (p=(type)npInitData,npInitData+=(size))




/*
 * Messages used during initialization
 *
 * 1. error messages start with driver name as they are
 * displayed even if '/V' isn't used.
 */
INITMSG szErrorVAddr[]=         "%s: error converting virtual to physical address";
INITMSG szNoMem[]=              "%s: error getting permanent memory";
INITMSG szNoDriverTab[]=        "%s: error retrieving driver table";
INITMSG szNoDevTab[]=           "%s: error retrieving device table from ADD %u (0x%x)";
INITMSG szAllocFailed[]=        "%s: adapter %u, unit %u allocation failed (0x%x)";
INITMSG szDeallocFailed[]=      "%s: adapter %u, unit %u, deallocation failed";

INITMSG szReadFailed[]=         "%s: adapter %u, unit %u READ failed (%04x)";
INITMSG szReadFailed2[]=        "%s: phys %u, admin. sector %lu READ failed (%04x)";
INITMSG szAdminDestroyed[]=     "%s: phys %u, admin. sector %lu destroyed";
INITMSG szAdminUnknown[]=       "%s: phys %u, unknown format of admin. sector";

INITMSG szRegDevClassFailed[]=  "%s: error registering device class";
INITMSG szNoTimer[]=            "%s: error registrating timer routine";
INITMSG szBadPhysdevSector[]=   "%s: adapter %u, unit %u, PHYSDEV definition destroyed, device ignored";
INITMSG szCreateFail[]=         "%s: creation of VRAID device failed";

INITMSG szUnknownRType[]=       "%s: unknown RDTYPE %u, drive ignored";
INITMSG szMirrorNotReg[]=       "%s: MIRROR defined but driver isn't registrated for MIRROR support"
"\r\n-- MIRROR parts will appear as seperate drives --";
INITMSG szRaidNotReg[]=         "%s: RAID4,5 defined but driver isn't registrated for it's support"
"\r\n-- array parts will appear as seperate drives --";

/* 2. Screen messages only displayed if '/V' or '/DEBUG' are used. */

INITMSG szSearchPhysDevices[] = "Looking for physical devices...";
INITMSG szFoundPhysDevices[] =  "Found %u PHYSDEVICE's";
INITMSG szNoPhysDevices[]=      "No PHYSDEVICE's found";
INITMSG szFoundPhysDevice[]=    "Adapter %u, Unit %u: found PHYSDEVICE";
INITMSG szNo16M[]=      "Adapter %u: hardware doesn't support memory above 16MB";
INITMSG szNoScGa[]=     "Adapter %u: hardware doesn't support scatter/gather";
INITMSG szDiskette[]=           "Adapter %u, Unit %u: diskette drive";
INITMSG szDefective[]=          "Adapter %u, Unit %u: defect";
INITMSG szNoDiskType[]=         "Adapter %u, unit %u: no disk device";

INITMSG szBuildDevTree[]=       "Building device tree...";
INITMSG szHostDriveCreate[] =   "Creating array drive";
INITMSG szHostDriveCount[] =    "Found %u array drive(s)";
INITMSG szNoHostDrive[] =       "No array drives found!";
INITMSG szSingleCreate[]=       "Creating SINGLE...";
INITMSG szSpanCreate[]=         "Creating SPAN...";
INITMSG szStripeCreate[]=       "Creating STRIPE...";
INITMSG szMirrorCreate[]=       "Creating MIRROR...";
INITMSG szRaid5Create[]=        "Creating RAID 5...";


/* Related to command line parsing */

INITMSG szCmdLineSyntax[]= "%s: syntax error in command line";
INITMSG szMissingOption[]= "%s: required option missing in command line";
INITMSG szUnknownOption[]= "%s: unknown command line option";
INITMSG szCmdLineError[]=  "%s: command line option error %d";


#if defined(DEBUG)
INITMSG dszValidateVSet2[]=     "\r\n[ValidateVSec2] checking %p";
INITMSG dszVal2Newer[]=         "\r\n[ValidateVSec2] %p contains newer admin data";
INITMSG dszVal2AdminData[]=     "\r\n[ValidateVSec2] admin data\r\n%z";
#endif




/*
 * Very important :-)
 */
INITMSG szProduct[]=    "Visual RAID Filter";
INITMSG szLogo[]=
"\r\n%s                                            Version %u.%02u"
"\r\nCopyright 1998-2001,2005 by Vitus Jensen (2:2474/424)         " __DATE__ ;

INITMSG szNoReg[]=      "Unregistered version, please register after a trial period";
INITMSG szDemo[]=       "Demonstration key, valid until %u/%u";
INITMSG szReg[]=        "Registered to: %s (level %u)";

#define REG_MAGIC       0x53A1B7F9
#define REG_KEYLEN      40
#pragma pack(1)
typedef struct {
    ULONG       ulMagic;
    UCHAR       szUser[REG_KEYLEN];
    UCHAR       szRegCode[REG_KEYLEN];
    ULONG       ulSize;
} REGSTRUCT;
#pragma pack()
REGSTRUCT _ininit RegData = {
    REG_MAGIC,
    "\000T3456789012345678901234567890123456789",
    "nfjhfjhfkjshffdsfsfsfjshfkjshfkjsfhsjkf",
    sizeof(REGSTRUCT) };

INITMSG szTimeExpired[]= "Timebomb expired, get new key or register!";






/*
 * Last element in code segments which remains resident
 */
PRIVATE void
Code_End(void) {}





/* **********************************************************************
 * **** Init-time I/O related *******************************************
 * ******************************************************************* */


/*# ----------------------------------------------------------------------
 * CALL
 *      SendIORBNotify(iorb)
 * PARAMETER
 *      iorb            completed request
 * RETURNS
 *      (nothing)
 * GLOBAL
 *      (nothing)
 * DESPRIPTION
 *      Awakens any process blocking on the passed IORB.  Used
 *      by 'SendIORB'.
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




/*# ----------------------------------------------------------------------
 * LocalSendIORB(iorb,addentry)
 *
 * PARAMETER
 *      iorb            request to execute
 *      addentry        entry point of ADD to call
 * RETURNS
 *      0               OK
 *      /0              error code returned from ADD
 * GLOBAL
 *      (nothing)
 * DESPRIPTION
 *      Uses 'SendIORBNotify' as notification routine.
 * REMARKS
 *      Need local variable to prevent restoring sp from bp
 *      after ADD call.
 */
#pragma optimize("lge",off)
PRIVATE USHORT NEAR
LocalSendIORB(PIORB iorb,void (FAR _Cdecl * addentry)(PIORB))
{
    USHORT      rc;                             /* define here (see cli) */

    iorb->NotifyAddress = (PVOID)SendIORBNotify; /* kind of hack */
    iorb->RequestControl |= IORB_ASYNC_POST;
    iorb->ErrorCode =   0;
    iorb->Status =      0;

    CallADD(addentry, iorb);

    cli();
    while( !(iorb->Status & IORB_DONE) )
    {
        rc = DevHelp_ProcBlock((ULONG)iorb, (ULONG)-1L, WAIT_IS_INTERRUPTABLE);
        if( rc != 0 )
        {
            DEBMSG1(DBG_INIT1,"\r\n[LocalSendIORB] ProcBlock - rc %w, ignored",rc);
#if 0
            iorb->Status = IORB_DONE | IORB_ERROR;
            iorb->ErrorCode = IOERR_CMD_ABORTED;
#endif
        }
        cli();
    }
    sti();

    if( (iorb->Status & IORB_ERROR) )
    {
        DEBMSG1(DBG_INIT1,"\r\n[LocalSendIORB] IORB ERROR %w",iorb->ErrorCode);
        DEBMSG2(DBG_INIT1,"\r\nIORB dump\r\n%z",(PVOID)iorb,iorb->Length);
        return iorb->ErrorCode;
    }
    else
        return 0;
}
#pragma optimize("",on)






/* **********************************************************************
 * **** Physical Device Searching (ADDs and UNITs) **********************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * CALL
 *      CheckAdapterInfo(iAdapter,npAdapterInfo)
 * PARAMETER
 *      iAdapter                index of adapter in system
 *,     npAdapterInfo           from GET_DEVICE_TABLE
 *
 * RETURNS
 *      0                       OK to process units from this adapter
 *      /0                      no SCSI, defect, ...
 *
 *      fDriverFlags
 * DESPRIPTION
 *      Checks adapter flags, additional service: displays some
 *      performance related capabilities of adapter.
 *
 * REMARKS
 *      There is really no reason to reject a certain adapter.
 *      We don't have to propagate a AF_16M flag because OS/2 will
 *      detect it anyway (getting infos from the ADD itself).
 */
PRIVATE int
CheckAdapterInfo(USHORT const iAdapter,NPADAPTERINFO const npAdapterInfo)
{
    if( !(npAdapterInfo->AdapterFlags & AF_16M) )
    {
        sprintk(szMsgBuffer, szNo16M, iAdapter);
        DEBMSG1(DBG_INIT2,"\r\n[CheckAdapterInfo] %s",(PSZ)szMsgBuffer);
        if( (fDriverFlags & DF_DEBUG) )
            SaveMessage();
    }
    if( !(npAdapterInfo->AdapterFlags & AF_HW_SCATGAT) )
    {
        sprintk(szMsgBuffer, szNoScGa, iAdapter);
        DEBMSG1(DBG_INIT2,"\r\n[CheckAdapterInfo] %s",(PSZ)szMsgBuffer);
        if( (fDriverFlags & DF_DEBUG) )
            SaveMessage();
    }

    return 0;
}




/*# ----------------------------------------------------------------------
 * CALL
 *      CheckUnitInfo(iAdapter,npUnitInfo)
 * PARAMETER
 *      iAdapter                index of adapter in system
 *      npUnitInfo              from GET_DEVICE_TABLE
 *
 * RETURNS
 *      0       OK, try/use this unit
 *      /0      bad type, defective, etc.
 *
 *
 * DESPRIPTION
 *      If this unit is defect then skip the unit.
 *      Dito if it's diskette A or B.
 *
 * REMARKS
 *      Should we skip removable media?
 *      Uses global 'fDriverFlags'.
 */
PRIVATE int
CheckUnitInfo(PPHYSDEVICE const phys,NPUNITINFO const npUnitInfo)
{
    USHORT const uflags = npUnitInfo->UnitFlags;
    USHORT const utype = npUnitInfo->UnitType;

    DEBMSG2(DBG_INIT3,"\r\n[CheckUnitInfo] %w %w",phys->iAdapter,phys->iUnit);
    if( (uflags & (UF_A_DRIVE | UF_B_DRIVE)) )
    {
        sprintk(szMsgBuffer, szDiskette, phys->iAdapter, phys->iUnit);
        DEBMSG1(DBG_INIT2,"\r\n[CheckUnitInfo] %s",(PSZ)szMsgBuffer);
        if( (fDriverFlags & DF_DEBUG) )
            SaveMessage();
        return 3;
    }

    if( (utype != UIB_TYPE_DISK) )
    {
        sprintk(szMsgBuffer, szNoDiskType, phys->iAdapter, phys->iUnit);
        DEBMSG1(DBG_INIT2,"\r\n[CheckUnitInfo] %s",(PSZ)szMsgBuffer);
        if( (fDriverFlags & DF_DEBUG) )
            SaveMessage();
        return 4;
    }

    if( (uflags & UF_DEFECTIVE) )
    {
        sprintk(szMsgBuffer, szDefective, phys->iAdapter, phys->iUnit);
        DEBMSG1(DBG_INIT1,"\r\n[CheckUnitInfo] %s",(PSZ)szMsgBuffer);
        if( (fDriverFlags & DF_VERBOSE) )
            SaveMessage();
        return 4;
    }

    return 0;
}




/*# ----------------------------------------------------------------------
 * LocalAllocateUnit(phys)
 *
 * PARAMETER
 *      phys            local structure
 *
 * RETURNS
 *      0               OK
 *      /0              something failed, skip this device
 *
 * DESPRIPTION
 *      Allocate unit.
 *
 * REMARKS
 */
PRIVATE int NEAR
LocalAllocateUnit(PPHYSDEVICE const phys)
{
    PIORB_UNIT_CONTROL  pIOUC = (PVOID)AllocateIorb(1);
    int                 rc = 0;
    USHORT              failed;


    DEBMSG1(DBG_INIT2,"\r\n[LocalAllocateUnit] PPHYSDEVICE %p",phys);

    /* Send request to ALLOCATE UNIT.  May be done
     * without registering our driver. */

    _fmemset(pIOUC, 0, MAX_IORB_SIZE);
    pIOUC->iorbh.Length =               sizeof(IORB_UNIT_CONTROL);
    pIOUC->iorbh.UnitHandle =           phys->hdADDUnit;
    pIOUC->iorbh.CommandCode =          IOCC_UNIT_CONTROL;
    pIOUC->iorbh.CommandModifier =      IOCM_ALLOCATE_UNIT;

    failed = LocalSendIORB((PIORB)pIOUC, phys->pADDEntry);

    if( failed )
    {
        sprintk(szMsgBuffer, szAllocFailed, (PSZ)szDriverName,
                phys->iAdapter, phys->iUnit, failed);
        SaveMessage();
        rc = 1;
    }

    FreeIorb((PVOID)pIOUC);
    DEBMSG(DBG_INIT3,"\r\n[LocalAllocateUnit] end");
    return rc;
} /* end[LocalAllocateUnit] */




/*# ----------------------------------------------------------------------
 * LocalDeallocateUnit(phys)
 *
 * PARAMETER
 *      phys            device structure
 *
 * RETURNS
 *      0               OK
 *      /0              error, not deallocated
 *
 * DESPRIPTION
 *      Deallocates unit.
 *
 * REMARKS
 */
PRIVATE int NEAR
LocalDeallocateUnit(PPHYSDEVICE const phys)
{
    PIORB_UNIT_CONTROL  pIOUC = (PVOID)AllocateIorb(1);
    int                 rc = 0;
    USHORT              failed;


    DEBMSG1(DBG_INIT2,"\r\n[LocalDeallocateUnit] PPHYSDEVICE %p",(PVOID)phys);

    /* Send request to DEALLOCATE UNIT */

    _fmemset(pIOUC, 0, MAX_IORB_SIZE);
    pIOUC->iorbh.Length =               sizeof(IORB_UNIT_CONTROL);
    pIOUC->iorbh.UnitHandle =           phys->hdADDUnit;
    pIOUC->iorbh.CommandCode =          IOCC_UNIT_CONTROL;
    pIOUC->iorbh.CommandModifier =      IOCM_DEALLOCATE_UNIT;

    failed = LocalSendIORB((PIORB)pIOUC, phys->pADDEntry);

    if( failed )
    {
        /* Couldn't deallocte unit.
         * Bad, display and ignore */

        sprintk(szMsgBuffer, szDeallocFailed,
                (PSZ)szDriverName, phys->iAdapter, phys->iUnit);
        SaveMessage();
    }

    FreeIorb((PVOID)pIOUC);
    DEBMSG(DBG_INIT3,"\r\n[LocalDeallocateUnit] end");
    return rc;
} /* end[LocalDeallocateUnit] */




/*# ----------------------------------------------------------------------
 * LocalReadSector(phys,secno,physaddr)
 *
 * PARAMETER
 *      phys            PHYSDEVICE structure
 *      secno           PSN
 *      physaddr        physical memory address to place data
 *
 * RETURNS
 *      0               OK
 *      /0              ADD error code
 *
 * DESCRIPTION
 *      Read a sector from disk.
 *
 * REMARKS
 * ------------------------------------------------------------------- #*/
PRIVATE USHORT
LocalReadSector(PPHYSDEVICE const phys,ULONG const secno,ULONG const physaddr)
{
    PIORB_EXECUTEIO const iorb = AllocateIorb(1);
    USHORT              error;


    if( iorb == NULL )
    {
        DEBMSG(DBG_INIT1,"\r\n[LocalReadSector] memory allocation failed");
        return ERR_NO_MEM;
    }

    iorb->cSGList = 1;
    iorb->pSGList->ppXferBuf = physaddr;
    iorb->pSGList->XferBufLen = SECTOR_SIZE;

    iorb->iorbh.Length =        sizeof(IORB_EXECUTEIO);
    iorb->iorbh.UnitHandle =    phys->hdADDUnit;
    iorb->iorbh.CommandCode =   IOCC_EXECUTE_IO;
    iorb->iorbh.CommandModifier = IOCM_READ;

    iorb->iorbh.RequestControl =        0;
    iorb->iorbh.StatusBlockLen =        0;
    iorb->iorbh.pStatusBlock =          0;

    iorb->RBA = secno;
    iorb->BlockCount = 1;                       /* only this single sector */
    iorb->BlockSize = SECTOR_SIZE;

    DEBMSG2(DBG_INIT4,"\r\n[LocalReadSector] IOCM_READ - iorb sc/ga\r\n%z",(PVOID)iorb,iorb->iorbh.Length);
    DEBMSG2(DBG_LEVEL4,"\r\n%z",(PVOID)iorb->pSGList,sizeof(*iorb->pSGList));

    if( (error=LocalSendIORB((PIORB)iorb, phys->pADDEntry)) )
    {
        sprintk(szMsgBuffer, szReadFailed, (PSZ)szDriverName,
                phys->iAdapter, phys->iUnit, error);
        SaveMessage();
    }

    FreeIorb(iorb);
    return error;
}




/*# ----------------------------------------------------------------------
 * FindVRaidPartition(phys,secno,extstart,pstart,psize)
 *
 * PARAMETER
 *      phys            
 *      secno           secto no of partition sector
 *      extstart        start of first extended partition, offsets are
 *                      calculated from here
 *      pstart          returns VRAID start (absolute)
 *      psize           returns VRAID size
 *
 * RETURNS
 *      APIRET
 *
 * DESCRIPTION
 *      Used to search recursive after a VRAID partition.
 *
 * REMARKS
 * ------------------------------------------------------------------- #*/
PRIVATE USHORT NEAR
FindVRaidPartition(PPHYSDEVICE const phys,ULONG const secno,ULONG extstart,
                   PULONG const pstart,PULONG const psize)
{
    PMBR const  mbr = MemAlloc(SECTOR_SIZE);
    USHORT      rc;


    DEBMSG1(DBG_INIT1,"\r\n[FindVRaidPartition] %p", phys);
    if( mbr == 0 )
    {
        DEBMSG(DBG_INIT1,"\r\n[FindVRaidPartition] memory allocation failed");
        return ERR_NO_MEM;
    }

    do
    {
        int     j;

        rc = LocalReadSector(phys, secno, FP_OFF(mbr) + ppWorkMem);
        if( rc != 0 )
            break;

        if( mbr->Signature != 0xAA55 )
            break;


        for( j = 0; j < 4  &&  *psize == 0; ++j )
        {
            switch( mbr->PartitionTable[j].SysIndicator )
            {
              case VRAID_PARTTYPE:
                *pstart = secno + mbr->PartitionTable[j].RelativeSectors;
                *psize = mbr->PartitionTable[j].NumSectors;
                DEBMSG1(DBG_INIT1,"\r\n[FindVRaidPartition] found starting at %lx",(PVOID)pstart);
                break;

              case EXTENDED_PARTTYPE:
                if( extstart == 0 )
                {
                    /* First EXT: calculate from there */

                    extstart = mbr->PartitionTable[j].RelativeSectors;
                    rc = FindVRaidPartition(phys,
                                            mbr->PartitionTable[j].RelativeSectors,
                                            extstart, pstart, psize);
                }
                else
                {
                    rc = FindVRaidPartition(phys,
                                            extstart
                                            + mbr->PartitionTable[j].RelativeSectors,
                                            extstart, pstart, psize);
                }
                break;

              default:
                break;                          /* ignore empty or other types */
            }
        }
    }
    while( 0 );

    MemFree(mbr);
    return rc;
}




/*# ----------------------------------------------------------------------
 * VerifyDiskData(phys)
 *
 * PARAMETER
 *      phys            describes device
 *
 * RETURNS
 *      0               OK
 *      /0              really bad things happend.
 *                      Don't touch this device.
 *
 * DESCRIPTION
 *      Do a first check whether this unit has been configured
 *      for VRaid.flt
 *      Builds VRDEVICE PHYSDEV if successfull.
 *
 * REMARKS
 *      Updates global 'apPDev', 'npInitData'.
 * ------------------------------------------------------------------- #*/
PRIVATE USHORT NEAR
VerifyDiskData(PPHYSDEVICE const phys)
{
    USHORT      error;
    PSEC_PHYSDEV sec = NULL;


    DEBMSG1(DBG_INIT1,"\r\n[VerifyDiskData] %p", phys);
    do
    {
        USHORT          crc;


        /* Search VRAID partition.  This works recursive starting from
         * sector 0 (the 'Master Boot Record'). */

        error = FindVRaidPartition(phys, 0, 0, &phys->ulVRStart, &phys->ulVRSize);
        if( error != 0 )
            break;
        if( phys->ulVRStart == 0 )
        {
            DEBMSG(DBG_INIT2,"\r\n[VerifyDiskData] no VRAID partition");
            error = ERR_NO_VRAIDPART;
            break;
        }


        /* Allocate resources (after FindVRaidPartition()). */

        sec = MemAlloc(SECTOR_SIZE);
        if( sec == NULL )
        {
            DEBMSG(DBG_INIT1,"\r\n[VerifyDiskData] memory allocation failed");
            error = ERR_NO_MEM;
            break;
        }


        /* -------- Read 'PHYSDEVICE' sector -------- */

        error = LocalReadSector(phys, phys->ulVRStart, FP_OFF(sec)+ppWorkMem);
        if( error != 0 )
            break;


        /* Analyse PHYSDEV sector.  That's really no big deal
         * as it contains nearly nothing. */

        DEBMSG2(DBG_INIT4,"\r\n[VerifyDiskData] SEC_PHYSDEV\r\n%z",(PVOID)sec,512u);
        crc = Crc16(sec, SECTOR_SIZE-2);
        if( _fmemcmp(sec->sectype,"PHYSDEVICE      ",16) != 0  ||  crc != sec->crc )
        {
            sprintk(szMsgBuffer, szBadPhysdevSector,
                    (PSZ)szDriverName, phys->iAdapter, phys->iUnit);
            SaveMessage();
            error = ERR_BAD_PHYSDEV;            /* bad layout (?) */
            DBSTOP();
            break;
        }


        /* Create a PHYSDEVICE.
         * afterwards it's getting simpler because we can rely on PHYSDEVICE
         * to handle all that IORB stuff. */

        sprintk(szMsgBuffer, szFoundPhysDevice, phys->iAdapter, phys->iUnit);
        DEBMSG1(DBG_INIT1,"\r\n[VerifyDiskData] %s",(PSZ)szMsgBuffer);
        if( (fDriverFlags & DF_VERBOSE) )
            SaveMessage();
        {
            USHORT      size;

            PhysdevCreate((PVRDEVHDR)npInitData, phys, sec, &size);

            apPDev[cPDev] = (PVOID)npInitData;
            npInitData += size;
            ++cPDev;                            /* one more */
        }
    }
    while( 0 );

    if( sec != NULL )   MemFree(sec);
    return error;
}




/*# ----------------------------------------------------------------------
 * InstallRouter(phys,npUnitInfo)
 *
 * PARAMETER
 *      phys
 *      npUnitInfo
 *
 * RETURNS
 *      (nothing)
 *
 * DESCRIPTION
 *      Create an entry in 'apBDisk[]' for this PHYSDEVICE.  The
 *      entry will contain any data for allocation/deallocation and
 *      device_table.
 *
 * REMARKS
 */
PRIVATE void NEAR
InstallRouter(PPHYSDEVICE const phys,NPUNITINFO const npUnitInfo)
{
    PBASEDISK           disk;


    GETINITRAM(disk,PBASEDISK,sizeof(BASEDISK));
    _fmemset(disk, 0, sizeof(BASEDISK));
    DEBMSG1(DBG_INIT2,"\r\n[InstallRouter] new BASEDISK: %p",disk);

    /* Link BASEDISK to PHYSDEVICE. */

    disk->phys = phys;

    /* Build unit info for later IOCC_DEVICE_TABLE calls.
     * Some data is from the original ADD but most flags/indices
     * are new. */

    _fmemcpy(&disk->info, npUnitInfo, sizeof(disk->info));
    disk->info.UnitHandle = (USHORT)&apBDisk[cBDisk];
    disk->info.FilterADDHandle = 0;             /* none so far */
    disk->info.UnitSCSITargetID = 0;            /* remove SCSI infos */
    disk->info.UnitSCSILUN = 0;

    /* Last step: add to table of BASEDISK and we are done. */

    apBDisk[cBDisk] = disk;
    ++cBDisk;

    return;
}




/*# ----------------------------------------------------------------------
 * CALL
 *      SearchPhysDevices(void)
 *
 * PARAMETER
 *      (none)
 *
 * RETURNS
 *      0               OK, install driver
 *      <0              fatal error, loud abort
 *      >0              no device found, be quiet
 *
 * DESPRIPTION
 *      Scans all adapters on all ADDs for DASD and inserts
 *      our DASD (OK, VRAID partition) into apPDev.
 *
 * REMARKS
 *      Updates global 'apPdev', 'npInitData' and 'fDriverFlags'.
 */
PRIVATE int NEAR
SearchPhysDevices(void)
{
    USHORT      j, k;
    USHORT      add_count;
    USHORT      add_index;
    USHORT      adapter_index;

    void        (FAR _Cdecl * pADDBaseEP)(PIORB);

    struct DevClassTableEntry FAR *pDCE;
    struct DevClassTableStruc FAR *pDCT;


    sprintk(szMsgBuffer, szSearchPhysDevices);
    DEBMSG1(DBG_INIT2,"\r\n[SearchPhysDevices] %s",(PSZ)szMsgBuffer);
    if( (fDriverFlags & DF_DEBUG) )
        SaveMessage();


    /* Get address of Registered ADD Table */

    if( DevHelp_GetDOSVar(DHGETDOSV_DEVICECLASSTABLE,DRIVERCLASS_ADD,(PPVOID)&pDCT) )
    {
        /* Should never happen.  What can we do? */

        sprintk(szMsgBuffer, szNoDriverTab, (PSZ)szDriverName);
        SaveMessage();
        return -1;
    }


    /* For each ADD registered  */

    DEBMSG(DBG_INIT3,"\r\n[SearchPhysDevices] Looking for ADDs");
    pDCE = pDCT->DCTableEntries;
    add_count = pDCT->DCCount;

    for( add_index = 0, adapter_index = 0;
         add_index < add_count; ++add_index, ++pDCE )
    {
        {
            PIORB_CONFIGURATION pIOCF = (PVOID)AllocateIorb(1);
            USHORT              rc;

            /* Get ADD's Device Table */

            _fmemset(pIOCF, 0, sizeof(IORB_CONFIGURATION));
            pIOCF->iorbh.Length =               sizeof(IORB_CONFIGURATION);
            pIOCF->iorbh.UnitHandle =           0;
            pIOCF->iorbh.CommandCode =          IOCC_CONFIGURATION;
            pIOCF->iorbh.CommandModifier =      IOCM_GET_DEVICE_TABLE;

            pIOCF->pDeviceTable =       (PDEVICETABLE)DeviceTable;
            pIOCF->DeviceTableLen =     sizeof(DeviceTable);

            pADDBaseEP = (PVOID)MAKEP(pDCE->DCSelector, pDCE->DCOffset);

            if( (rc=LocalSendIORB((PIORB)pIOCF, pADDBaseEP)) != 0 )
            {
                /* Error (?), could not get device table of installed
                 * ADD.  Just ignore that ADD (which probably hasn't
                 * any adapters/devices, either). */

                sprintk(szMsgBuffer, szNoDevTab, (PSZ)szDriverName, add_index, rc);
                SaveMessage();
                FreeIorb((PVOID)pIOCF);
                continue;
            }
            FreeIorb((PVOID)pIOCF);
        }


        /* -------- For each Adapter in ADD's Device Table -------- */

        for( j = 0;
             j < ((PDEVICETABLE)DeviceTable)->TotalAdapters;
             ++j, ++adapter_index )
        {
            NPADAPTERINFO const npAdapterInfo
                = ((PDEVICETABLE)DeviceTable)->pAdapter[j];
            NPUNITINFO          npUnitInfo;


            if( CheckAdapterInfo(adapter_index, npAdapterInfo) )
                continue;

            /* -------- Access UNITINFO for each Device on adapter -------- */

            DEBMSG1(DBG_INIT2,"\r\n[SearchPhysDevices] looking for units on adapter %w",adapter_index);
            npUnitInfo = npAdapterInfo->UnitInfo;

            for( k = 0; k < npAdapterInfo->AdapterUnits; ++k, ++npUnitInfo )
            {
                PPHYSDEVICE     phys;

                GETINITRAM(phys,PPHYSDEVICE,sizeof(PHYSDEVICE));
                _fmemset(phys, 0, sizeof(PHYSDEVICE));
                DEBMSG1(DBG_INIT2,"\r\n[SearchPhysDevices] new PHYSDEVICE: %p",phys);

                /* If a Filter ADD has registered for this unit
                 * then send subsequent requests to the Filter ADD
                 * and not to the ADD. */

                if( (phys->hdFilter=npUnitInfo->FilterADDHandle) != 0 )
                {
                    struct DevClassTableEntry FAR *pDCEFilter;

                    DEBMSG(DBG_INIT3,"\r\n[SearchPhysDevices] found other filter for unit");
                    pDCEFilter = &pDCT->DCTableEntries[phys->hdFilter-1];
                    phys->pADDEntry = (PVOID)MAKEP(pDCEFilter->DCSelector,
                                                   pDCEFilter->DCOffset);
                }
                else
                {
                    phys->pADDEntry = pADDBaseEP;
                }
                phys->iAdapter = adapter_index;
                phys->iUnit = npUnitInfo->UnitIndex;
                phys->hdADDUnit = npUnitInfo->UnitHandle;

                if( CheckUnitInfo(phys, npUnitInfo) )
                {
                    npInitData -= sizeof(PHYSDEVICE); /* release memory */
                    continue;
                }

                if( LocalAllocateUnit(phys) )
                {
                    /* Couldn't allocate unit.  Release any data
                     * allocated and continue with next device. */

                    npInitData -= sizeof(PHYSDEVICE); /* free memory */
                    continue;
                }

                /* Now read partition table to verify that this
                 * is an unit managed by our filter. */

                if( VerifyDiskData(phys) != 0 )
                {
                    LocalDeallocateUnit(phys);
                    npInitData -= sizeof(PHYSDEVICE); /* free memory */
                    continue;
                }

                /* Save some restrictions in global variables. */

                if( npAdapterInfo->MaxHWSGList != 0
                    &&  npAdapterInfo->MaxHWSGList < uADDMaxSGList )
                    uADDMaxSGList = npAdapterInfo->MaxHWSGList;
                if( npAdapterInfo->MaxCDBTransferLength != 0
                    &&  npAdapterInfo->MaxCDBTransferLength < ulADDMaxTransfer )
                    ulADDMaxTransfer = npAdapterInfo->MaxCDBTransferLength;
                if( (npAdapterInfo->AdapterFlags & AF_16M) == 0 )
                    fADDFlags |= AF_NOT16M;
                if( (npAdapterInfo->AdapterFlags & AF_HW_SCATGAT) )
                    fADDFlags |= AF_HWSCATGAT;

                if( (fDriverFlags & DF_NOROUTER) == 0 )
                    InstallRouter(phys, npUnitInfo);

            } /* for each unit */
        } /* for each adapter */
    } /* for each ADD */

    if( cPDev == 0 )
    {
        sprintk(szMsgBuffer, szNoPhysDevices);
        DEBMSG1(DBG_INIT1,"\r\n[SearchPhysDevices] %s",(PSZ)szMsgBuffer);
        if( (fDriverFlags & DF_VERBOSE) )
            SaveMessage();
        return 1;                               /* non-fatal error */
    }

    sprintk(szMsgBuffer, szFoundPhysDevices, cPDev);
    DEBMSG1(DBG_INIT1,"\r\n[InitBase] %s",(PSZ)szMsgBuffer);
    if( (fDriverFlags & DF_VERBOSE) )
        SaveMessage();

    return 0;
} /* end[SearchPhysDevices] */






/* **********************************************************************
 * **** Analysis of configuration sectors *******************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * CALL
 *      AddHostDrive(PVRDEVHDR vrd)
 * PARAMETER
 *      vrd             VRDEVICE to build HOSTDRIVE for
 * RETURNS
 *      (nothing)
 * GLOBAL
 *      apHDrv
 * DESCRIPTION
 *      Builds an entry in 'apHDrv' for a VRDEVICE.  Called if a VRDEVICE
 *      is top of structure (configuration sector marks it as HOSTDRIVE).
 *
 * REMARKS
 */
PRIVATE VOID NEAR
AddHostDrive(PVRDEVHDR vrd)
{
    GETINITRAM(apHDrv[cHDrv],PHOSTDRIVE,sizeof(HOSTDRIVE));

    DEBMSG2(DBG_INIT1,"\r\n[AddHostDrive] creating at %p for %p",(PVOID)apHDrv[cHDrv],(PVOID)vrd);
    sprintk(szMsgBuffer, szHostDriveCreate);
    DEBMSG1(DBG_INIT2,"\r\n[AddHostDrive] %s",(PSZ)szMsgBuffer);
    if( (fDriverFlags & DF_DEBUG) )
        SaveMessage();
    _fmemset(apHDrv[cHDrv], 0, sizeof(HOSTDRIVE));
    apHDrv[cHDrv]->child = vrd;
    ++cHDrv;
}




/*# ----------------------------------------------------------------------
 * CompressTab(tab)
 *
 * PARAMETER
 *      tab     table of FAR pointer
 *
 * RETURNS
 *      count of entries inside table
 *
 * DESCRIPTION
 *      Compresses entries in 'tab' so that no holes (NULL pointers) appear.
 *
 * REMARKS
 */
PRIVATE int NEAR
CompressTab(PVRDEVHDR FAR * tab,int const cnt)
{
    int j, i;

    for( j = i = 0; i < cnt; ++i )
    {
        if( tab[i] != 0 )
            tab[j++] = tab[i];
    }
    for( i = j; i < cnt; ++i )
        tab[i] = 0;

    return j;
}




/*# ----------------------------------------------------------------------
 *      AnalyseVRDev(sec,save_here,tab2[])
 *
 * PARAMETER
 *      sec             sector to analyse
 *      save_here       saves pointer to new VRDEVHDR here
 *      tab2[]          drives to build from
 *
 * RETURNS
 *      0       OK, device created
 *      /0      no device created, don't try again
 *
 * DESCRIPTION
 *      Analyse VRDEVICE-description sector.
 *
 * REMARKS
 */
PRIVATE int NEAR
AnalyseVRDev(PSEC_VRDEV const sec,PVRDEVHDR FAR * save_here,PVRDEVHDR tab2[])
{
    USHORT      size;
    int         rc = 0;
    int         i;


    *save_here = NULL;
    switch( sec->u.s.type )
    {
      case RDTYPE_SINGLE:
        sprintk(szMsgBuffer, szSingleCreate);
        DEBMSG1(DBG_INIT1,"\r\n[AnalyseVRDev] %s",(PSZ)szMsgBuffer);
        if( (fDriverFlags & DF_VERBOSE) )
            SaveMessage();
        i = SingleCreate((PVRDEVHDR)npInitData, tab2, sec, &size);
        if( i  == 0 )
        {
            *save_here = (PVOID)npInitData;
            npInitData += size;
        }
        else
        {
            if( i < 0 )
            {
                sprintk(szMsgBuffer, szCreateFail, (PSZ)szDriverName);
                SaveMessage();
            }
            rc = i;
        }
        break;

      case RDTYPE_STRIPE:
        sprintk(szMsgBuffer, szStripeCreate);
        DEBMSG1(DBG_INIT1,"\r\n[AnalyseVRDev] %s",(PSZ)szMsgBuffer);
        if( (fDriverFlags & DF_VERBOSE) )
            SaveMessage();
        i = StripeCreate((PVRDEVHDR)npInitData, tab2, sec, &size);
        if( i == 0 )
        {
            *save_here = (PVOID)npInitData;
            npInitData += size;
        }
        else
        {
            if( i < 0 )
            {
                sprintk(szMsgBuffer, szCreateFail, (PSZ)szDriverName);
                SaveMessage();
            }
            rc = i;
        }
        break;

      case RDTYPE_CHAIN:
        sprintk(szMsgBuffer, szSpanCreate);
        DEBMSG1(DBG_INIT1,"\r\n[AnalyseVRDev] %s",(PSZ)szMsgBuffer);
        if( (fDriverFlags & DF_VERBOSE) )
            SaveMessage();
        i = SpanCreate((PVRDEVHDR)npInitData, tab2, sec, &size);
        if( i == 0 )
        {
            *save_here = (PVOID)npInitData;
            npInitData += size;
        }
        else
        {
            if( i < 0 )
            {
                sprintk(szMsgBuffer, szCreateFail, (PSZ)szDriverName);
                SaveMessage();
            }
            rc = i;
        }
        break;

      default:
        sprintk(szMsgBuffer, szUnknownRType, (PSZ)szDriverName, sec->u.s.type);
        SaveMessage();
        rc = ERR_UNKNOWN_RTYPE;
        break;
    }

    if( *save_here != NULL )
    {
        if( (sec->u.s.flags & 0x80) )
        {
            AddHostDrive(*save_here);           /*End of VRDEVICE: add as HostDrive*/
            *save_here = NULL;                  /* and remove from list */
        }
    }
    return rc;
}




/*# ----------------------------------------------------------------------
 *      AnalyseVRDev2(sec,save_here,tab2[])
 *
 * PARAMETER
 *      sec             sector to analyse
 *      save_here       saves pointer to new VRDEVHDR here
 *      tab2[]          drives to build from
 * RETURNS
 *      0       OK, device created
 *      /0      no device created, don't try again
 *
 * DESCRIPTION
 *      Analyse VRDEVICE-description sector.
 *
 * REMARKS
 */
PRIVATE int NEAR
AnalyseVRDev2(PSEC_VRDEV2 const sec,PVRDEVHDR FAR * save_here,PVRDEVHDR tab2[])
{
    USHORT      size;
    int         rc = 0;
    int         i;

    *save_here = NULL;
    switch( sec->u.s.type )
    {
      case RDTYPE_MIRROR:
        if( uRegLevel < 2 )
        {
            sprintk(szMsgBuffer, szMirrorNotReg, (PSZ)szDriverName);
            SaveMessage();
            rc = ERR_BAD_REGLEVEL;
            break;
        }

        sprintk(szMsgBuffer, szMirrorCreate);
        DEBMSG1(DBG_INIT1,"\r\n[AnalyseVRDev2] %s",(PSZ)szMsgBuffer);
        if( (fDriverFlags & DF_VERBOSE) )
            SaveMessage();
        i = MirrorCreate((PVOID)npInitData, tab2, sec, &size);
        if( i == 0 )
        {
            *save_here = (PVOID)npInitData;
            npInitData += size;
        }
        else
        {
            if( i < 0 )
            {
                sprintk(szMsgBuffer, szCreateFail, (PSZ)szDriverName);
                SaveMessage();
            }
            rc = i;
        }
        break;


#if !defined(NORAID5)
      case RDTYPE_RAID4:
      case RDTYPE_RAID5:
        if( uRegLevel < 3 )
        {
            sprintk(szMsgBuffer, szRaidNotReg, (PSZ)szDriverName);
            SaveMessage();
            rc = ERR_BAD_REGLEVEL;
            break;
        }

        sprintk(szMsgBuffer, szRaid5Create);
        DEBMSG1(DBG_INIT1,"\r\n[AnalyseVRDev2] %s",(PSZ)szMsgBuffer);
        if( (fDriverFlags & DF_VERBOSE) )
            SaveMessage();
        i = Raid5Create((PVOID)npInitData, tab2, sec, &size);
        if( i == 0 )
        {
            *save_here = (PVOID)npInitData;
            npInitData += size;
        }
        else
        {
            if( i < 0 )
            {
                sprintk(szMsgBuffer, szCreateFail, (PSZ)szDriverName);
                SaveMessage();
            }
            rc = i;
        }
        break;
#endif


      default:
        sprintk(szMsgBuffer, szUnknownRType, (PSZ)szDriverName, sec->u.s.type);
        SaveMessage();
        rc = ERR_UNKNOWN_RTYPE;
        break;
    }

    if( *save_here != NULL )
    {
        if( (sec->u.s.flags & 0x80) )
        {
            AddHostDrive(*save_here);           /*End of VRDEVICE:add as HostDrive*/
            *save_here = NULL;                  /* and remove from list */
        }
    }
    return rc;
}




/*# ----------------------------------------------------------------------
 * CALL
 *      BuildDevTree(void)
 * PARAMETER
 *      (none)
 *      aPDev, cPDev    previously found PHYSDEVICEs
 *
 * RETURNS
 *      0               OK
 *      /0              fatal error, don't install driver
 *
 * DESCRIPTION
 *      Scans all previously found physical devices and does:
 *      - add their ID to structure
 *      - read next adminsector and build VRDEVICE
 *      - ...
 *
 * REMARKS
 */
PRIVATE int NEAR
BuildDevTree(void)
{
    /* Temporary table to hold pointers (all PHYSDEVs plus
     * all parents (may all be Singles!)) */

    static PVRDEVHDR            tab1[2*MAX_PDEV];

    PVOID const         secbuf = MemAlloc(SECTOR_SIZE); /* allocated in MemPool*/
    USHORT              rc = 0;
    ULONG               adsecno;
    int                 i;

    sprintk(szMsgBuffer, szBuildDevTree);
    DEBMSG1(DBG_INIT1,"\r\n[BuildDevTree] %s",(PSZ)szMsgBuffer);
    if( (fDriverFlags & DF_DEBUG) )
        SaveMessage();
    DEBMSG1(DBG_INIT3,"\r\n[BuildDevTree] sector buffer %p",secbuf);

    if( secbuf == NULL )
    {
        sprintk(szMsgBuffer, szMallocFailed, (PSZ)szDriverName);
        SaveMessage();
        return ERR_NO_MEM;
    }

    _fmemset(tab1, 0, sizeof(tab1));
    _fmemcpy(tab1, apPDev, cPDev*sizeof(apPDev[0]));


    /* ------------------------------------------------------------ */
    /* We already read the first administrative sector and installed
     * all physical devices (all PHYSDEVICE sectors read).
     * All devices still in tab1[] build complicated structures. */

    for( adsecno = 1; adsecno < VRAID_ADMINSECTORS; ++adsecno )
    {
        int const found = CompressTab(tab1, sizeof(tab1)/sizeof(tab1[0]));
        int     j = found;

        /* 'found' contains the count of non-empty entries in 'tab1',
         * starting from index 0 to 'found-1' (compressed).
         * 'j' is the index of the first free entry in 'tab1', new
         * device will be entered here and in later entries */

        if( found == 0 )
            break;                              /* nothing left in table = done */


        /* Now read administrative sector 'adsecno' of every VRDEVICE
         * in 'tab1' and build entry in 'tab1' at index 'j'. */

        for( i = 0; i < found; ++i )
        {
            if( tab1[i] == NULL )
            {
                DEBMSG1(DBG_INIT3,"\r\n[BuildDevTree] ignored empty index %w",i);
                continue;
            }

            DEBMSG1(DBG_INIT2,"\r\n[BuildDevTree] reading index %w",i);
            rc = ProcessAdminIO(tab1[i], FALSE, 1, 1, FP_OFF(secbuf) + ppWorkMem);
            if( rc != 0 )
            {
                sprintk(szMsgBuffer,szReadFailed2, (PSZ)szDriverName, i,adsecno,rc);
                SaveMessage();
                DBSTOP();
                tab1[i] = NULL;
                rc = 0;                         /* no reason to unload driver... */
                continue;                       /* skip this device */
            }


            /* Verify VRDEVICE-description sector. */

            DEBMSG2(DBG_INIT4,"\r\n[BuildDevTree] SEC_???\r\n%z",(PVOID)secbuf,512u);
            if( _fmemcmp(secbuf, "VRAID", 5) != 0  &&  adsecno == 1 )
            {
                DEBMSG1(DBG_INIT1,"\r\n[BuildDevTree] PHYSDEVICE-only (index %w)",i);
                tab1[i] = NULL;                 /* remove and ... */
                continue;                       /* skip this device */
            }

            if( Crc16(secbuf, SECTOR_SIZE-2)
                != ((PUSHORT)secbuf)[SECTOR_SIZE/2-1] )
            {
                sprintk(szMsgBuffer,szAdminDestroyed, (PSZ)szDriverName, i, adsecno);
                SaveMessage();
                DEBMSG2(DBG_ALL,"\r\n[BuildDevTree] contents:\r\n%z",secbuf,SECTOR_SIZE);
                DBSTOP();
                tab1[i] = NULL;
                continue;                       /* skip this device */
            }


            /* Analyse sector data and build array structure. */

            if( _fmemcmp(secbuf, "VRAIDDEVICE     ", 16) == 0 )
            {
                if( AnalyseVRDev(secbuf, &tab1[j++], tab1) < 0 )
                    tab1[i] = NULL;             /* remove even this */
            }
            else if( _fmemcmp(secbuf, "VRAIDDEVICE2    ", 16) == 0 )
            {
                if( AnalyseVRDev2(secbuf, &tab1[j++], tab1) < 0 )
                    tab1[i] = NULL;             /* remove even this */
            }
            else
            {
                sprintk(szMsgBuffer,szAdminUnknown, (PSZ)szDriverName, i, adsecno);
                SaveMessage();
                DEBMSG2(DBG_ALL,"\r\n[BuildDevTree] contents:\r\n%z",secbuf,SECTOR_SIZE);
                DBSTOP();
                tab1[i] = NULL;
                continue;                       /* skip this device */
            }
        } /* end[for every found device] */
    } /* end[for(adsecno)] */

    MemFree(secbuf);
    if( cHDrv != 0 )
    {
        sprintk(szMsgBuffer, szHostDriveCount, cHDrv);
        DEBMSG1(DBG_INIT2,"\r\n[BuildDevTree] %s",(PSZ)szMsgBuffer);
        if( (fDriverFlags & DF_VERBOSE) )
            SaveMessage();
    }
    else
    {
        sprintk(szMsgBuffer, szNoHostDrive);
        DEBMSG1(DBG_INIT2,"\r\n[BuildDevTree] %s",(PSZ)szMsgBuffer);
        if( (fDriverFlags & DF_VERBOSE) )
            SaveMessage();
    }

    return rc;
}






/* **********************************************************************
 * **** Other routines **************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * CALL
 *      RegisterDriver(void)
 * PARAMETER
 *      (none)
 * RETURNS
 *      0               OK
 *      /0              registration failed
 * GLOBAL
 *      fDriverFlags
 * DESCRIPTION
 *      Add our filter driver to OS/2 internal tables.
 *
 * REMARKS
 *      Call DevHelp_RegisterDevClass() as late as possible.
 *      A registered driver returning 'don't install' will
 *      panic the kernel. :-(
 */
PRIVATE int NEAR
RegisterDriver(void)
{
    BOOL        b;
    int         rc = 0;                         /* assume OK */

    DEBMSG(DBG_INIT2,"\r\n[RegisterDriver] entry");
    b = DevHelp_RegisterDeviceClass((NPSZ)szDriverName,
                                    (PFN)IorbEntry,
                                    (USHORT)DRIVERFLAGS,
                                    (USHORT)DRIVERCLASS_ADD,
                                    (PUSHORT)&hdThisDriver);
    if( b )
    {
        sprintk(szMsgBuffer, szRegDevClassFailed, (PSZ)szDriverName);
        SaveMessage();
        rc = 3;
    }
    return rc;
}






#define ENTRY_STATE        0
#define MAX_STATES         2

/*
 *                                      opt.state[] initialization definitions
 *
 *                                              ____ entry state
 *                                              |               previous
 *                                              v                 opt   |
 *  ----Command Line Option --------            ----- STATE TABLE ----- |
 *  token id            string  type            0   1   2               |
 *                                                                      |
 *                                              *  /A:  /A:      <-------
 *                                                       plus
 */
#define TOK_ID_EF       100
#define TOK_ID_DF       101

OPT OPT_VERBOSE =
    {TOK_ID_VERBOSE,0,  "/V", TYPE_0,           {0,  E,  E}};

OPT OPT_DEBUG_NO =
    {TOK_ID_DEBUG,0,    "/DEBUG:", TYPE_D,      {0,  E,  E}};

OPT OPT_DEBUG_DEFAULT =
    {TOK_ID_DEBUG,0,    "/DEBUG", TYPE_0,       {0,  E,  E}};

OPT OPT_NSHARE =
    {TOK_ID_NOT_SHARE,0,"/!SHARE", TYPE_0,      {0,  E,  E}};

OPT OPT_SHARE =
    {TOK_ID_SHARE,0,    "/SHARE", TYPE_0,       {0,  E,  E}};

OPT OPT_END =
    {TOK_ID_END,0,      "\0", TYPE_0,           {O,  O,  O}};


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
        &OPT_NSHARE,
        &OPT_SHARE,
        &OPT_END
    }
};




/*# ----------------------------------------------------------------------
 * CALL
 *      AnalyseCmdLine(pszLine)
 * PARAMETER
 *      pszLine         pointer to command line
 * RETURNS
 *      0               OK
 * GLOBAL
 * DESPRIPTION
     Invoke parser and analyse token output.
 * REMARKS
 */
PRIVATE int NEAR
AnalyseCmdLine(PSZ const pszLine)
{
    CC          parse_rc;
    PCHAR       tp;
    UCHAR       TokenId;
    USHORT      adapter = 0xFFFF;


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
         *      General error in command line, no special handling
         */
      case SYNTAX_ERR:
        sprintk(szMsgBuffer, szCmdLineSyntax, (PSZ)szDriverName);
        SaveMessage();
        break;

        /* REQ_OPT_ERR:
         *      Required option is missing in command line (probably /A: )
         */
      case REQ_OPT_ERR:
        sprintk(szMsgBuffer, szMissingOption, (PSZ)szDriverName);
        SaveMessage();
        break;

        /* INVALID_OPT_ERR:
         *      Bad format?
         */
      case INVALID_OPT_ERR:
        sprintk(szMsgBuffer, szUnknownOption, (PSZ)szDriverName, (PBYTE)DeviceTable);
        SaveMessage();
        break;

      case UNDEFINED_TYPE_ERR:
      case UNDEFINED_STATE_ERR:
      case BUF_TOO_SMALL_ERR:
      default:
        sprintk(szMsgBuffer, szCmdLineError, (PSZ)szDriverName, parse_rc.ret_code);
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
            DEBMSG(DBG_INIT2,"\r\n/VERBOSE");
            fDriverFlags |= DF_VERBOSE;
            break;

          case TOK_ID_DEBUG:
            DEBMSG(DBG_INIT2,"\r\n/DEBUG");
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

          case TOK_ID_SHARE:
            DEBMSG(DBG_INIT2,"\r\n/SHARE");
            fDriverFlags &= ~DF_NOROUTER;
            break;

          case TOK_ID_NOT_SHARE:
            DEBMSG(DBG_INIT2,"\r\n/!SHARE");
            fDriverFlags |= DF_NOROUTER;
            break;

          case TOK_ID_ADAPTER:
            adapter = *(tp+TOKL_VALUE);
            DEBMSG1(DBG_INIT2,"\r\n/A:%w",adapter);
            break;

          case TOK_ID_END:
            break;

          default:
            break;
        }
    }

    return 0;
} /*end[AnalyseCmdLine]*/




/*# ----------------------------------------------------------------------
 * CheckRegistration(void)
 *
 * PARAMETER
 *      (none)
 *
 * RETURNS
 *      (nothing)
 *
 * DESPRIPTION
 *      Sets global 'uRegLevel' according to registration code and
 *      display message (if requested or not registered).
 *
 * REMARKS
 */
PRIVATE VOID NEAR
CheckRegistration(VOID)
{
    UCHAR       code[REG_KEYLEN];
    UCHAR       c;
    int         i;
    UCHAR _far *p, _far *k;
    USHORT      try;

    for( try = 1; try <= 3; ++try )
    {
        /* In case that some guy just patches the registration level
         * inside the binary file... */

        uRegLevel = 0;


        /* Now calculate registration key for level 'try' and
         * compare it with registration key inside our image. */

        _fmemset(code, 0, REG_KEYLEN);
        c = szProduct[0];                       /* start value */

        for( i = 0, p = RegData.szUser, k = szProduct;
             *p != '\0';
             ++p, ++i, k = (*(k+1) == '\0' ? szProduct : k+1) )
        {
            c += (UCHAR)((*p & 0xf0) >> 2 ^ (*p & 0x0f));
            c *= *k + try;
            c += *k;
            code[i] = c;
        }
        if( i == 0 )
            break;                              /* no user data found */

        if( _fmemcmp(code, RegData.szRegCode, i) == 0 )
        {
            uRegLevel = try;
            DEBMSG1(DBG_INIT1,"\r\n!!!Registration level %w detected!!!",uRegLevel);
            break;                              /* end, only single level possible */
        }
    }


    /* Display registration or non-registration data. */

    if( uRegLevel == 0 )
        fDriverFlags |= DF_VERBOSE;
    if( _fmemcmp(&RegData.szUser[4], "DEMO", 4) == 0 )
    {
        USHORT  year, month;

        /* Demonstration key, timebombed. */

        fDriverFlags |= DF_VERBOSE;             /* always verbose */
        month = (RegData.szUser[0] - '0') * 10  +  (RegData.szUser[1] - '0');
        year = (RegData.szUser[2] - '0') * 10  +  (RegData.szUser[3] - '0');
        year += 2000;

        sprintk(szMsgBuffer, szLogo, (PSZ)szProduct, VERSION, SUBVERSION);
        SaveMessage();
        sprintk(DeviceTable, szDemo, month, year);
        sprintk(szMsgBuffer, szReg, (PSZ)DeviceTable, uRegLevel);
        SaveMessage();

        if( pGlobalInfoSeg->YrsDate > year
            ||  (pGlobalInfoSeg->YrsDate == year
                 &&  pGlobalInfoSeg->MonDate > (UCHAR)month) )
        {
            sprintk(szMsgBuffer, szTimeExpired);
            SaveMessage();
            uRegLevel = 0;
        }
    }
    else if( (fDriverFlags & DF_VERBOSE) )
    {
        sprintk(szMsgBuffer, szLogo, (PSZ)szProduct, VERSION, SUBVERSION);
        SaveMessage();
        if( uRegLevel != 0 )
        {
            sprintk(szMsgBuffer, szReg, (PSZ)RegData.szUser, uRegLevel);
            SaveMessage();
        }
        else
        {
            sprintk(szMsgBuffer, szNoReg);
            SaveMessage();
        }
    }

    return;
}






/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * ValidateVSec2(child,sec)
 *
 * PARAMETER
 *      child           child to validate
 *      sec             configuration space of sibling
 *
 * RETURNS
 *      0               OK, use this child
 *      /0              don't use it
 *
 * DESCRIPTION
 *      This routine checks whether a VRDEV is a valid child as configured
 *      in 'sec'.
 *      Normally that means that 'child' contains the identical admin.
 *      sector.  But whenever 'child' had problems during operation and
 *      had to be deactivated those sectors may differ.  Certain combinations
 *      are allowed, other not.  See below for details.
 *
 * REMARKS
 *      OBS: there is a goto!
 */
PUBLIC int
ValidateVSec2(PVRDEVHDR const child,SEC_VRDEV2 const FAR * const sec)
{
    int         result = 0;
    USHORT      rc, i;
    PVOID const secbuf = MemAlloc(SECTOR_SIZE); /* allocated in MemPool*/

    DEBMSG1(DBG_INIT1,dszValidateVSet2,(PVOID)child);

    do
    {
        ++result;
        if( secbuf == NULL )
        {
            sprintk(szMsgBuffer, szMallocFailed, (PSZ)szDriverName);
            SaveMessage();
            break;
        }

        ++result;
        rc = ProcessAdminIO(child, FALSE, 1, 1, FP_OFF(secbuf) + ppWorkMem);
        if( rc != 0 )
        {
            sprintk(szMsgBuffer, szReadError, (PSZ)szDriverName, rc);
            SaveMessage();
            DBSTOP();
            MemFree(secbuf);
            break;
        }

        ++result;
        if( _fmemcmp(sec, secbuf, SECTOR_SIZE) != 0 )
        {
            PSEC_VRDEV2 const   childsec = secbuf;

            if( _fmemcmp(childsec->sectype, "VRAIDDEVICE2    ", 16) != 0 )
                break;

            ++result;
            if( Crc16(childsec, SECTOR_SIZE-2) != childsec->crc )
                break;

            /* 'child' contains a valid admin. sector which *could* be an
             * old instance of 'child'.
             * To be true 'child's sector has to be older and from the same
             * machine.  It has to define the same RAID1 array, too. */

            ++result;
            if( childsec->timestamp > sec->timestamp )
            {
                /* This is impossible to handle the current way: we detected
                 * a sibling which is more uptodate than the one we are creating
                 * an array now.  Better stop the current creation and repeat
                 * this stuff when scanning reached this child. */

                DEBMSG1(DBG_INIT1,dszVal2Newer,(PVOID)child);
                DEBMSG2(DBG_INIT2,dszVal2AdminData,(PVOID)childsec,SECTOR_SIZE);
                result = -1;
                break;
            }

            if( _fmemcmp(childsec->u.s.id, sec->u.s.id, sizeof(DEVID)) != 0 )
                break;

            ++result;
            if( childsec->u.s.type != sec->u.s.type )
                break;

            ++result;
            if( childsec->u.s.children != sec->u.s.children )
                break;

            ++result;
            for( i = 0; i < sec->u.s.children; ++i )
            {
                if( _fmemcmp(childsec->u.s.child[i].id, sec->u.s.child[i].id, sizeof(DEVID)) != 0 )
                    goto XABORT;
                if( childsec->u.s.child[i].size != sec->u.s.child[i].size )
                    goto XABORT;
            }
        }

        /* When we finally reached this point 'child' may be used
         * to build an array. */

        result = 0;
    }
    while( 0 );

  XABORT:
    MemFree(secbuf);
    return result;
} /* end[ValidateVSec2] */




/*# ----------------------------------------------------------------------
 * NAME
 *      InitBase
 * CALL
 *      InitBase(pRPI)
 * PARAMETER
 *      pRPI            init request
 * RETURNS
 *      error code as expected from the OS/2 device system
 * GLOBAL
 *      Device_Help
 *      pGlobalInfoSeg
 *      fDriverFlags
 *      szDriverName, error messages
 * DESPRIPTION
 *      Process Base Initialization Request Packet
 * REMARKS
 *      Uses IBM supplied command line parser.
 */
PUBLIC USHORT NEAR
InitBase(PRPINITIN pRPI)
{
    PULONG              pInfoSegSel;
    int                 rc;
    PRPINITOUT const    pRPO = (PRPINITOUT)pRPI;

    PDDD_PARM_LIST const pInitParms = (PDDD_PARM_LIST)pRPI->InitArgs;
    PSZ                 pCmdLine;

    DEBMSG(DBG_INIT2,"\r\n[InitBase] entry");
    DBSTOP();


    /* Contains entry point to kernel functions */

    Device_Help = pRPI->DevHlpEP;
    pCmdLine    = MK_FP(FP_SEL(pInitParms),pInitParms->cmd_line_args);

    if( DevHelp_VirtToPhys(&DiskDDHeader, &ppDiskDDHeader) )
    {
        sprintk(szMsgBuffer, szErrorVAddr, (PSZ)szDriverName);
        SaveMessage();
        rc = ERR_XLATION_FAIL;
        goto ERROR_EXIT;                        /* !!! */
    }

    DevHelp_GetDOSVar(DHGETDOSV_SYSINFOSEG, 0, (PPVOID)&pInfoSegSel);
    pGlobalInfoSeg = MAKEP(*pInfoSegSel, 0);


    AnalyseCmdLine(pCmdLine);                   /* parse command line args */
    CheckRegistration();                        /* analyse registration info */


    do
    {
        /* Initialize memory routines, we need memory
         * to scan and analyse disks. */

        CrcInit(0xA001);                        /* CRC16 table */
        rc = BuildWorkmem();
        if( rc )
        {
            sprintk(szMsgBuffer, szNoMem, (PSZ)szDriverName);
            SaveMessage();
            break;
        }
        DEBMSG1(DBG_ALL,"\r\nppWorkMem == %lx",(PVOID)&ppWorkMem);

        rc = DevHelp_AllocateCtxHook(MemoryHook, &hdMemHook);
        if( rc != 0 )
            break;
        rc = DevHelp_AllocateCtxHook(BuildHook, &hdBuildHook);
        if( rc != 0 )
            break;

        rc = SearchPhysDevices();
        if( rc )
            break;


        /* We got some physical devices to install for.  So
         * allocate now all drive-independed resources. */

        /* Install a timer handler so we can detect timeouts. */

        DEBMSG(DBG_INIT1,"\r\n[InitBase] allocating timer");
        rc = DevHelp_TickCount((NPFN)&AsmTimer, 
                               TIMER_TMS/pGlobalInfoSeg->SIS_ClkIntrvl);
        if( rc )
        {
            sprintk(szMsgBuffer, szNoTimer, (PSZ)szDriverName);
            SaveMessage();
            break;
        }


        /* Analyse all physical devices found in the first run. */

        rc = BuildDevTree();
        if( rc )
            break;
    }
    while( 0 );


    /* Processing complete.  Analyse situation, should we install,
     * quiet fail or even loudly stop Config.Sys processing? */

    if( rc == 0 )
    {
        rc = RegisterDriver();                  /* very late! */
        if( rc != 0 )
            goto ERROR_EXIT;                    /* really sad */

        /* Register host drives as units. */

        pRPO->Unit    = (UCHAR)(cHDrv + cBDisk);
        pRPO->CodeEnd = ((USHORT)Code_End & ~0x0F) + 0x10;
        pRPO->DataEnd = ((USHORT)npInitData & ~0x0F) + 0x10;

        DEBMSG2(DBG_INIT1,"\r\n[InitBase] driver installed (%w array, %w basedisks)",cHDrv,cBDisk);
        fDriverFlags |= DF_INIT1DONE;
        return STATUS_DONE;
    }
    else
    {
      ERROR_EXIT:                               /* goto destination */
        pRPO->Unit    = 0;
        pRPO->CodeEnd = 0;
        pRPO->DataEnd = 0;

        DEBMSG(DBG_INIT1,"\r\n[InitBase] don't load driver");
        ReleaseWorkmem();                               /* !!! unlock memory !!! */
        if( hdMemHook != 0 )
            DevHelp_FreeCtxHook(hdMemHook);
        if( hdBuildHook != 0 )
            DevHelp_FreeCtxHook(hdBuildHook);

        if( rc < 0 )
            return STERR | STDON | ERROR_I24_GEN_FAILURE;
        else
            return STERR | STDON | ERROR_I24_QUIET_INIT_FAIL;
    }
} /* end[InitBase] */
