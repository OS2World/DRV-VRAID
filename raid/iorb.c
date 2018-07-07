/*
 * $Source: r:/source/driver/raid/RCS/iorb.c,v $
 * $Revision: 3.1 $
 * $Date: 2005/06/17 05:57:45 $
 * $Locker:  $
 *
 *      Request Time processing - Timer processing
 *
 *      DDK says we have to saved SI,DI,DS,ES but all code I've seen
 *      only uses 'loadds' restoring only SI,DI,DS (and remember: CorelSCSI
 *      even changed DS!).  Nevertheless I will do a '_saveregs' now and
 *      save/restore all registers when calling other drivers...
 *
 * History:
 * $Log: iorb.c,v $
 * Revision 3.1  2005/06/17 05:57:45  vitus
 * - GetDeviceGeometry: instead of providing no head/sector mapping for large
 *   arrays tell OS/2 to use 255/63.  When there is no mapping supplied
 *   OS2DASD.DMD uses 64/32 :-(
 * - added New BSD license statement
 * - removed tabs
 *
 * Revision 2.4  2003/11/14 00:58:24  vitus
 * - GetDeviceGeometry: when cylinder count passes 0xFFFF no hd/sec mapping
 *   is supplied.  OS/2 can't use more than 16bit cylinder numbers.
 *
 * Revision 2.3  2001/07/31 23:57:32  vitus
 * - Timer: start build process after 45 seconds (there seems to be a critical
 *   period during boot where build I/Os may fail)
 *
 * Revision 2.2  2001/07/22 00:29:19  vitus
 * - Timer: instead of calling UpdateBuilds() directly arm a context
 *   hook to call it.  May be done more seldom as ther BuildNotify()
 *   does it, too.
 *
 * Revision 2.1  1999/09/23 23:13:28  vitus
 * - defined all DBGMSG() strings as located in CONST segment
 * - changed IorbEntry() to ArrayHandler()
 * - readded IorbEntry() to be a router between ArrayHandler() and FilterHandler().
 *   All IORB processing routines now get a pointer to the device they are working
 *   for.  IorbEntry() translates IORB.UnitHandle to this pointer.
 * - GetDeviceTable() now returns host drives *and* BASEDISKs (those which
 *   are processed by FilterHandler() in filter.c)
 *
 * Revision 1.10  1999/06/20 17:35:14  vitus
 * - beeping may now be disabled via fDriverFlags bit
 *
 * Revision 1.9  1999/06/02 02:22:09  vitus
 * - Timer: beautified beeper frequency and duration
 *
 * Revision 1.8  1999/05/12 01:45:32  vitus
 * - Timer: runs more often and calls routines at two different intervals
 *
 * Revision 1.7  1999/04/17 00:16:56  vitus
 * - Timer: calls UpdateBuilds()
 * - Timer: call DevHelp_Beep() if cBeep != 0
 *
 * Revision 1.6  1998/12/13 22:29:09  vitus
 * - updates HOSTDRIVE element 'ReqCnt'
 * - changes for removed elements from HOSTDRIVE (flags, vrd, size)
 *
 * Revision 1.5  1998/09/20 14:28:35  vitus
 * - returns minimum of all MaxCDBTransferLength
 * - seperate code for IOCM_GET_LOCK_STATUS (returns error)
 * - Timer: InsertVrio(vrio) instead of QueueVrio(vrio)
 *
 * Revision 1.4  1998/06/26 22:39:33  vitus
 * - switched to DDK macros (cli, sti, pushf)
 * - uses CallADD()
 *
 * Revision 1.3  1998/05/29 01:45:43  vitus
 * - fine tuning of DEBMSG macro calls
 *
 * Revision 1.2  1998/04/08 01:11:08  vitus
 * - added PUBLIC HostdriveNotify()
 *
 * Revision 1.1  1998/02/18 00:00:53  vitus
 * Initial revision
 * --------------------------------------------------
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
static char const id[]="$Id: iorb.c,v 3.1 2005/06/17 05:57:45 vitus Exp $";
#if defined(__WATCOMC__)
# pragma on (unreferenced)
#endif

#include <string.h>

#define INCL_NOBASEAPI
#define INCL_NOPMAPI
#define INCL_INITRP_ONLY
#include "os2intf.h"

#include "vraid.h"
#include "proto.h"
#include "extern.h"




#if defined(DEBUG)
CONSTMSG dszArrayHandler[]=     "\r\n[ArrayHandler] drive %p, iorb %p";
CONSTMSG dszHostNotify[]=       "\r\n[HostdriveNotify] io %p, iorb %p, error %w";
CONSTMSG dszQueueIorb[]=        "\r\n[QueueIorbChain] entry";
CONSTMSG dszQueueIo[]=          "\r\n[QueueDeviceIO] entry";
CONSTMSG dszQueueIoAlloc[]=     "\r\n[QueueDeviceIO] HD not allocated";
CONSTMSG dszSetAdapter[]=       "\r\n[SetAdapterInfo] entry";
CONSTMSG dszSetUnit[]=          "\r\n[SetUnitInfo] entry";
CONSTMSG dszSetUnitCopy[]=      "\r\n[SetUnitInfo] copying supplied new UnitInfo";
CONSTMSG dszSetUnitColl[]=      "\r\n[SetUnitInfo] collecting default UnitInfo";
CONSTMSG dszSetUnitDef[]=       "\r\n[SetUnitInfo] setting defect flag";
CONSTMSG dszGetDevTab[]=        "\r\n[GetDeviceTable] entry";
CONSTMSG dszAllocUnit[]=        "\r\n[AllocateUnit] entry";
CONSTMSG dszAllocUnitAlloc[]=   "\r\n[AllocateUnit] HD already allocated";
CONSTMSG dszDeallocUnit[]=      "\r\n[DeAllocateUnit] entry";
CONSTMSG dszDeallocUnitAlloc[]= "\r\n[DeAllocateUnit] HD not allocated";
CONSTMSG dszChangeUnit[]=       "\r\n[ChangeUnitInfo] entry";
CONSTMSG dszGetDevGeo[]=        "\r\n[GetDeviceGeometry] entry";
CONSTMSG dszGetDevGeoSmall[]=   "\r\n[GetDeviceGeometry] structure too small";
CONSTMSG dszGetDevGeoAlloc[]=   "\r\n[GetDeviceGeometry] HD not allocated";
CONSTMSG dszGetDevGeoSize[]=    "\r\n[GetDeviceGeometry] size = %lx sectors";
CONSTMSG dszUnitStatus[]=       "\r\n[GetUnitStatus] entry";
CONSTMSG dszNotify[]=           "\r\n[NotifyIorb] entry (%p, %w)";
CONSTMSG dszNotifyError[]=      "\r\nVUJRAID$ returns %w to OS/2";
CONSTMSG dszIorbUnknown[]=      "\r\nCommand %w, Modifier %w not supported";
CONSTMSG dszIorbNoGetLock[]=    "\r\nIOCM_GET_LOCK_STATUS not supported";
CONSTMSG dszIorbUnknCmd[]=      "\r\nReceived unknown command code %w";
CONSTMSG dszBE_ACH[]=           "\r\n[Timer] ArmCtxHook - rc %w";
#endif





/* **********************************************************************
 * **** Callback Routine ************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * HostdriveNotify(io)
 *
 * PARAMETER
 *      io              I/O just completed
 *
 * RETURNS
 *      (nothing)
 *
 * DESCRIPTION
 *      The VRIO 'io' to first layer below hostdrive has just completed.
 *      Figure out which hostdrive and IORB is affected and complete the
 *      IORB, too.
 *
 * REMARKS
 */
PUBLIC void NEAR _Cdecl
HostdriveNotify(PVRIO io)
{
    PIORB_EXECUTEIO const       iorb = io->iorb;

    DEBMSG3(DBG_PATH,dszHostNotify,(PVOID)io,(PVOID)iorb,io->oserr);
    ASSERT( io != NULL );
    ASSERT( iorb != NULL );

    NotifyIorb(&iorb->iorbh, io->oserr);
    return;
}






/* **********************************************************************
 * **** Private Routines ************************************************
 * ******************************************************************* */


/*# ----------------------------------------------------------------------
 * QueueIorbChain(pDrive,pIorb)
 *
 * PARAMETER
 *      pDrive          host drive
 *      pIorb           OS/2 request
 * RETURNS
 *      (nothing)
 *
 * DESCRIPTION
 *      Adds a chain of IORBs to pDrive's IORB queue.
 *
 * REMARKS
 */
PRIVATE void NEAR
QueueIorbChain(PHOSTDRIVE const pDrive,PIORBH const pIorb)
{
    PIORBH      pLastIorb   = pIorb;
    USHORT      ChainLength = 0;

    DEBMSG(DBG_LEVEL4,dszQueueIorb);

    /* Find Last IORB if chained set of IORBs were received
     * from device manager.
     * By doing so we can also initialize the ADDWorkSpace. */

    for(;;)
    {
        /*_fmemset(pLastIorb->ADDWorkSpace, 0, ADD_WORKSPACE_SIZE);*/
        ++ChainLength;

        if( !(pLastIorb->RequestControl & IORB_CHAIN) )
            break;
        else
            pLastIorb   = pLastIorb->pNxtIORB;
    }
    pLastIorb->pNxtIORB = 0;

    LOCK(pDrive->hlQueue);
    ++pDrive->ReqCnt;
    if( pDrive->pQueueHead == NULL )            /* empty queue? */
        pDrive->pQueueHead = pIorb;             /* set as first */
    else
        pDrive->pQueueFoot->pNxtIORB = pIorb;   /* set behind last */
    pDrive->pQueueFoot = pLastIorb;             /* new last IORB */
    cIorbInQueue += ChainLength;                /* global count */
    UNLOCK(pDrive->hlQueue);                    /* somebody may interrupt */

    return;
}




/*# ----------------------------------------------------------------------
 * QueueDeviceIO(pDrive,pIorb)
 *
 * PARAMETER
 *      pDrive
 *      pIorb           IORB chain (EXECUTEIO)
 *
 * RETURNS
 *      (nothing)
 *
 * DESCRIPTION
 *      Adds an IORB chain to our internal queues and call
 *      'StartMayIorb' to get as many I/Os started as possible.
 *
 * REMARKS
 */
PRIVATE void NEAR
QueueDeviceIO(PHOSTDRIVE const pDrive,PIORB const pIorb)
{
    DEBMSG(DBG_LEVEL4,dszQueueIo);
    if ( !(pDrive->OsFlags & OF_ALLOCATED) )    /* only I/O if allocated */
    {
        DEBMSG(DBG_ALL,dszQueueIoAlloc);
        NotifyIorb(pIorb, IOERR_UNIT_NOT_ALLOCATED);
        return;
    }

    /* Append this IORB chain to already received IORB. */

    QueueIorbChain(pDrive, pIorb);

    /* Start as many IORBs as possible. */

    StartManyIorb(pDrive);

    /* Ok, notification will complete IORBs. */

    return;
}




/*# ----------------------------------------------------------------------
 * SetAdapterInfo(pAdapterInfo)
 *
 * PARAMETER
 *      pAdapterInfo
 *
 * RETURNS
 *      (nothing)
 *
 * DESCRIPTION
 *      This routines inserts a wonnabe adapter description
 *      into the device table.
 *
 * REMARKS
 */
PRIVATE void NEAR
SetAdapterInfo(PADAPTERINFO const pAdapterInfo)
{
    DEBMSG(DBG_LEVEL2,dszSetAdapter);

    pAdapterInfo->AdapterUnits          = 0;
    pAdapterInfo->AdapterIOAccess       = AI_IOACCESS_OTHER;
    pAdapterInfo->AdapterFlags          = 0;

    _fstrncpy(pAdapterInfo->AdapterName, "Virtual RAID Adp", 17);

    /* Just set any reasonable value. */

    pAdapterInfo->AdapterHostBus = AI_HOSTBUS_UNKNOWN;
    pAdapterInfo->AdapterDevBus = AI_DEVBUS_OTHER;
    pAdapterInfo->AdapterSCSITargetID = 0;      /* non-SCSI */
    pAdapterInfo->AdapterSCSILUN = 0;

    /* Without buffering, the worst physical device determines
     * our restrictions. */

    if( (fADDFlags & AF_NOT16M) == 0 )
        pAdapterInfo->AdapterFlags |= AF_16M;   /* hurray! */

    if( (fADDFlags & AF_HWSCATGAT) )
        pAdapterInfo->AdapterFlags |= AF_HW_SCATGAT;

    pAdapterInfo->MaxHWSGList = uADDMaxSGList;
    pAdapterInfo->MaxCDBTransferLength = ulADDMaxTransfer;
    return;
}




/*# ----------------------------------------------------------------------
 * SetUnitInfo(pUnitInfo,devp)
 *
 * PARAMETER
 *      pUnitInfo       from OS/2
 *      devp            device structure
 *
 * RETURNS
 *      (nothing)
 *      *pUnitInfo filled
 *
 * DESCRIPTION
 *      Fills unitinfo for an array.
 *
 * REMARKS
 */
PRIVATE void NEAR
SetUnitInfo(PUNITINFO const pUnitInfo,PHOSTDRIVE const devp)
{
    DEBMSG(DBG_LEVEL2,dszSetUnit);
    if( devp->modinfo )
    {
        DEBMSG(DBG_LEVEL1,dszSetUnitCopy);
        _fmemcpy(pUnitInfo, devp->modinfo, sizeof(UNITINFO));
    }
    else
    {
        DEBMSG(DBG_LEVEL3,dszSetUnitColl);
        pUnitInfo->QueuingCount = uMaxQueueCount;

        pUnitInfo->UnitFlags = 0;
        pUnitInfo->Reserved = 0;
        pUnitInfo->FilterADDHandle = 0;

        if( (devp->child->flags & RDFLAG_ERROR) )
        {
            pUnitInfo->UnitFlags |= UF_DEFECTIVE;
            DEBMSG(DBG_LEVEL1,dszSetUnitDef);
        }

        pUnitInfo->UnitType = UIB_TYPE_DISK;
        pUnitInfo->UnitSCSITargetID = 0;        /* non-SCSI */
        pUnitInfo->UnitSCSILUN  = 0;
    }
    return;
}




/*# ----------------------------------------------------------------------
 * GetDeviceTable(pIorb)
 *
 * PARAMETER
 *      pIorb                   PIORB_CONFIGURATION
 *
 * RETURNS
 *      (nothing)
 *      pIorb                   notified
 *          pIorb->pDeviceTable filled
 *
 * DESCRIPTION
 *      Returns a device table containing a two adapters and
 *      many devices.
 *      adapter 0               all BASEDISK devices
 *      adapter 1               all HOSTDRIVEs (arrays)
 *
 * REMARKS
 *      It could be performance relative to combine units with certain
 *      restrictions (part of RAID is located on adapter w/o >16MB
 *      support, etc.).  But this is the [possible] future.
 */
PRIVATE void NEAR
GetDeviceTable(PIORB_CONFIGURATION const pIorb)
{
    PDEVICETABLE    pDeviceTable;               /* pointers to tell OS/2 */
    PADAPTERINFO    pAdapterInfo;
    PUNITINFO       pUnitInfo;

    USHORT      idxUnit;                        /* counts units per adapter */
    USHORT      i;


    DEBMSG(DBG_LEVEL2,dszGetDevTab);
    pDeviceTable = pIorb->pDeviceTable;         /* passed by caller */

    /* General information */

    pDeviceTable->ADDLevelMajor = ADD_LEVEL_MAJOR; /* defined by DDK-level */
    pDeviceTable->ADDLevelMinor = ADD_LEVEL_MINOR;
    pDeviceTable->ADDHandle = hdThisDriver;     /* told by RegisterDriver() */
    pDeviceTable->TotalAdapters = 2;            /* total count found */


    /* Set information of adapter 0 (containing BASEDISKs). */

    pAdapterInfo =  (PADAPTERINFO)(pDeviceTable->pAdapter);
    (PBYTE)pAdapterInfo += 2 * sizeof(NPADAPTERINFO);

    pDeviceTable->pAdapter[0] = (NPADAPTERINFO)FP_OFF(pAdapterInfo);
    SetAdapterInfo(pAdapterInfo);
    pUnitInfo = pAdapterInfo->UnitInfo;         /* Room for UNIT INFO */


    /* Build entries for all BASEDISKs (for every disk allocated
     * by our driver but reachable by OS/2).
     * We pass most of the original UNIT INFO to caller:
     * - Indices are ours */

    for( idxUnit = i = 0; i < MAX_PDEV; ++i )
    {
        if( apBDisk[i] == NULL )
            continue;

        _fmemcpy(pUnitInfo, &apBDisk[i]->info, sizeof(*pUnitInfo));

        pUnitInfo->AdapterIndex = 0;            /* index of first adapter */
        pUnitInfo->UnitIndex    = idxUnit;      /* index of unit on this adapter */

        ++pUnitInfo;
        ++pAdapterInfo->AdapterUnits;
        ++idxUnit;
    } /*end[for(i)]*/


    /* Set information of adapter 0 (containing BASEDISKs). */

    pAdapterInfo = (PADAPTERINFO)pUnitInfo;
    pDeviceTable->pAdapter[1] = (NPADAPTERINFO)FP_OFF(pAdapterInfo);
    SetAdapterInfo(pAdapterInfo);
    pUnitInfo = pAdapterInfo->UnitInfo;         /* room for UNIT INFO */


    /* For each device in 'apHDrv' (a host drive) build an entry
     * in supplied device table.  */

    for( idxUnit = i = 0; i < MAX_HDRV ; ++i )
    {
        if( apHDrv[i] == NULL )
            continue;

        pUnitInfo->AdapterIndex = 1;            /* index of second adapter */
        pUnitInfo->UnitIndex    = idxUnit;      /* index of unit on this adapter */
        pUnitInfo->UnitHandle   = (USHORT)&apHDrv[i];
        SetUnitInfo(pUnitInfo, apHDrv[i]);

        ++pUnitInfo;
        ++pAdapterInfo->AdapterUnits;
        ++idxUnit;
    } /*end[for(i)]*/

    NotifyIorb(&pIorb->iorbh, 0);
    return;
}




/*# ----------------------------------------------------------------------
 * AllocateUnit(pDrive,pIorb)
 *
 * PARAMETER
 *      pDrive
 *      pIorb        pointer to requested unit
 *
 * RETURNS
 *      (nothing)
 *      IORB completed
 *
 * DESCRIPTION
 *      Called by DMD to garanty exclusive access of an unit.  Sets
 *      'allocated' flag in device structure.
 *
 * REMARKS
 */
PRIVATE void NEAR
AllocateUnit(PHOSTDRIVE const pDrive,PIORB_UNIT_CONTROL const pIorb)
{
    DEBMSG(DBG_LEVEL2,dszAllocUnit);
    if( (pDrive->OsFlags & OF_ALLOCATED) )
    {
        DEBMSG(DBG_ALL,dszAllocUnitAlloc);
        NotifyIorb(&pIorb->iorbh, IOERR_UNIT_ALLOCATED);
        return;
    }

    pDrive->OsFlags |= OF_ALLOCATED;
    ++pDrive->ReqCnt;                           /* no locking needed */
    NotifyIorb(&pIorb->iorbh, 0);
    return;
}




/*# ----------------------------------------------------------------------
 * DeAllocateUnit(pDrive,pIorb)
 *
 * PARAMETER
 *      pDrive
 *      pIorb           I/O request buffer
 *
 * RETURNS
 *      (nothing)
 *      IORB completed
 *
 * DESCRIPTION
 *      Updates 'allocated' flag in device structure.
 *
 * REMARKS
 *      Could do a SYNCHRONIZE CACHE on affected physical devices?
 */
PRIVATE void NEAR
DeAllocateUnit(PHOSTDRIVE const pDrive,PIORB_UNIT_CONTROL const pIorb)
{
    DEBMSG(DBG_LEVEL2,dszDeallocUnit);
    if( !(pDrive->OsFlags & OF_ALLOCATED) )
    {
        DEBMSG(DBG_ALL,dszDeallocUnitAlloc);
        NotifyIorb(&pIorb->iorbh, IOERR_UNIT_NOT_ALLOCATED);
        return;
    }

    pDrive->OsFlags &= ~OF_ALLOCATED;
    ++pDrive->ReqCnt;                           /* no locking needed */
    NotifyIorb(&pIorb->iorbh, 0);
    return;
}




/*# ----------------------------------------------------------------------
 * ChangeUnitInfo(pDrive,pIorb)
 *
 * PARAMETER
 *      pDrive
 *      pIorb           I/O request buffer
 *
 * RETURNS
 *      (nothing)
 *      IORB completed
 *
 * DESCRIPTION
 *      Save pointer to new unit info in device structure.  Will be
 *      returned on next GET_DEVICE_TABLE.
 *
 * REMARKS
 */
PRIVATE void NEAR
ChangeUnitInfo(PHOSTDRIVE const pDrive,PIORB_UNIT_CONTROL const pIorb)
{
    DEBMSG(DBG_LEVEL2,dszChangeUnit);
    pDrive->modinfo = pIorb->pUnitInfo;
    ++pDrive->ReqCnt;                           /* no locking needed */
    NotifyIorb(&pIorb->iorbh, 0);
    return;
}




/*# ----------------------------------------------------------------------
 * GetDeviceGeometry(pDrive,pIorb)
 *
 * PARAMETER
 *      pDrive
 *      pIorb           I/O request buffer
 *
 * RETURNS
 *      (nothing)
 *      IORB completed
 *
 * DESCRIPTION
 *      Return previously calculated parameter
 *
 * REMARKS
 */
PRIVATE void NEAR
GetDeviceGeometry(PHOSTDRIVE const pDrive,PIORB_GEOMETRY const pIorb)
{
    DEBMSG(DBG_LEVEL2,dszGetDevGeo);
    if ( pIorb->GeometryLen < sizeof(GEOMETRY) )
    {
        DEBMSG(DBG_ALL,dszGetDevGeoSmall);
        NotifyIorb(&pIorb->iorbh, IOERR_CMD_SYNTAX);
        return;
    }
    if( !(pDrive->OsFlags & OF_ALLOCATED) )
    {
        DEBMSG(DBG_ALL,dszGetDevGeoAlloc);
        NotifyIorb(&pIorb->iorbh, IOERR_UNIT_NOT_ALLOCATED);
        return;
    }

    pIorb->pGeometry->TotalSectors      = pDrive->child->size;
    pIorb->pGeometry->BytesPerSector    = 512;
    pIorb->pGeometry->Reserved          = 0;
    pIorb->pGeometry->NumHeads          = 64;
    pIorb->pGeometry->SectorsPerTrack   = 32;
    pIorb->pGeometry->TotalCylinders    = pDrive->child->size / 64 / 32;
    if( pIorb->pGeometry->TotalCylinders > 0xFFFF )
    {
        /* The OS/2 only passes a 16bit value to LVM, FDISK and other
         * disk management tools, the API uses an USHORT.
         * To make those big arrays usable all geometry data has to
         * be zero (which should have been used from the beginning,
         * now we keep 64 / 32 for compatibility reasons). */

        pIorb->pGeometry->NumHeads = 255;
        pIorb->pGeometry->SectorsPerTrack = 63;
        pIorb->pGeometry->TotalCylinders = pDrive->child->size / 255 / 63;
    }

    DEBMSG1(DBG_LEVEL2,dszGetDevGeoSize,(PVOID)&pDrive->child->size);
    ++pDrive->ReqCnt;                           /* no locking needed */
    NotifyIorb(&pIorb->iorbh, 0);
    return;
}




/*# ----------------------------------------------------------------------
 * GetUnitStatus(pDrive,pIorb)
 *
 * PARAMETER
 *      pDrive
 *      pIorb                   PIORB_UNIT_STATUS
 *
 * RETURNS
 *      (nothing)
 *      IORB completed
 *          pIorb->UnitStatus
 *
 * DESCRIPTION
 *      This functions sets bits in ->UnitFlags to reflect the state
 *      of the requested unit.
 *
 * REMARKS
 */
PRIVATE void NEAR
GetUnitStatus(PHOSTDRIVE const pDrive,PIORB_UNIT_STATUS const pIorb)
{
    DEBMSG(DBG_LEVEL2,dszUnitStatus);
    if( (pDrive->child->flags & RDFLAG_ERROR) )
        pIorb->UnitStatus |= US_DEFECTIVE;
    else
        pIorb->UnitStatus |= US_READY | US_POWER;

    ++pDrive->ReqCnt;                           /* no locking needed */
    NotifyIorb(&pIorb->iorbh, 0);               /* translation: always OK */
    return;
}




/*# ----------------------------------------------------------------------
 * ArrayHandler(pDrive,pIorb)
 *
 * PARAMETER
 *      pDrive
 *      pIorb                   I/O request buffer
 *
 * RETURNS
 *      (nothing)
 *
 * DESCRIPTION
 *      IORB processing for hostdrives and the driver itself.
 *
 * REMARKS
 */
PRIVATE void
ArrayHandler(PHOSTDRIVE const pDrive,PIORB const pIorb)
{
    USHORT const CommandCode     = pIorb->CommandCode;
    USHORT const CommandModifier = pIorb->CommandModifier;


    DEBMSG2(DBG_PATH,dszArrayHandler,(PVOID)pDrive,(PVOID)pIorb);

    /* Test for allowed combinations of flags, opcodes, etc. */

    if( (pIorb->RequestControl & IORB_CHAIN) && CommandCode != IOCC_EXECUTE_IO )
    {
        NotifyIorb(pIorb, IOERR_CMD_SYNTAX);
        return;
    }


    /* IORB may be valid, more checking in
     * tillbeh”rig rutin */

    switch( CommandCode )
    {
      case IOCC_CONFIGURATION:                  /* affects driver operation */
        switch( CommandModifier )
        {
            /* **************************************************** 
             **** IOCM_GET_DEVICE_TABLE:                           
             **** Generates a table of all found devices           
             **** along with device manager information.           
             **************************************************** */
          case IOCM_GET_DEVICE_TABLE:
            GetDeviceTable((PIORB_CONFIGURATION)pIorb);
            break;

            /* ****************************************************
             **** IOCM_COMPLETE_INIT:                      
             **** Notifies the driver that BIOS-operation
             **** has stopped and only this driver will be
             **** used to do I/O.                          
             **************************************************** */
          case IOCM_COMPLETE_INIT:
            CompleteInit((PIORB_CONFIGURATION)pIorb);
            break;

          default:
            DEBMSG2(DBG_ALL,dszIorbUnknown,CommandCode,CommandModifier);
            DBSTOP();
            NotifyIorb(pIorb, IOERR_CMD_NOT_SUPPORTED);
            break;
        }
        break;

      case IOCC_UNIT_CONTROL:                   /* all mandatory */
        switch( CommandModifier )
        {
            /* ****************************************************
             **** IOCM_ALLOCATE_UNIT:                      
             **** Used by device manager to get control of   
             **** a single unit; I/O may be done only to        
             **** allocated units and units can only be    
             **** allocated once.                          
             **************************************************** */
          case IOCM_ALLOCATE_UNIT:
            AllocateUnit(pDrive, (PIORB_UNIT_CONTROL)pIorb);
            break;

            /* **************************************************** 
             **** IOCM_DEALLOCATE_UNIT:                   
             **** Used by device manager to release control  
             **** of an unit, so other manager can use it.   
             **** Currently only OS2ASPI.DMD uses this call. 
             **************************************************** */
          case IOCM_DEALLOCATE_UNIT:
            DeAllocateUnit(pDrive, (PIORB_UNIT_CONTROL)pIorb);
            break;

            /* ****************************************************
             **** IOCM_CHANGE_UNITINFO:                    
             **** Used by: ????                            
             **** Usage: ????                              
             **** May by used to set a different unit info   
             **** (FilterADDHandle, ...)                       
             **************************************************** */
          case IOCM_CHANGE_UNITINFO:
            ChangeUnitInfo(pDrive, (PIORB_UNIT_CONTROL)pIorb);
            break;

          default:
            DEBMSG2(DBG_ALL,dszIorbUnknown,CommandCode,CommandModifier);
            DBSTOP();
            NotifyIorb( pIorb, IOERR_CMD_NOT_SUPPORTED );
            break;
        }
        break;

      case IOCC_GEOMETRY:
        switch( CommandModifier )
        {
            /* *****************************************************
             * **** IOCM_GET_MEDIA_GEOMETRY:
             * **** IOCM_GET_DEVICE_GEOMETRY:
             * **** Informs the caller about the format of
             * **** this device. DEVICE and MEDIA formats are
             * **** the same on RAID devices.
             * ************************************************** */
          case IOCM_GET_MEDIA_GEOMETRY:
          case IOCM_GET_DEVICE_GEOMETRY:
            GetDeviceGeometry(pDrive, (PIORB_GEOMETRY)pIorb);
            break;

          default:
            DEBMSG2(DBG_ALL,dszIorbUnknown,CommandCode,CommandModifier);
            DBSTOP();
            NotifyIorb(pIorb, IOERR_CMD_NOT_SUPPORTED);
            break;
        }
        break;

      case IOCC_EXECUTE_IO:                     /* read/write I/Os */
        switch( CommandModifier )
        {
            /* ****************************************************
             **** IOCM_READ:                               
             **** IOCM_WRITE:                              
             **** IOCM_WRITE_VERIFY:                       
             **** WRITE_VERIFY is the same as simple WRITE!  
             **************************************************** */
          case IOCM_READ:
          case IOCM_WRITE:
          case IOCM_WRITE_VERIFY:
            QueueDeviceIO(pDrive, pIorb);
            break;

            /* ****************************************************
             **** IOCM_READ_VERIFY:                        
             **** Verifies whether blocks are readable on a 
             **** devices.                                 
             **** Because this Modifier may send a         
             **** list of blocks to verify, 'QueueDeviceIO()'
             **** is used (instead of simply returning OK)!  
             **************************************************** */
          case IOCM_READ_VERIFY:
            QueueDeviceIO(pDrive, pIorb);
            break;

            /* **************************************************** 
             **** IOCM_READ_PREFETCH:                      
             **** Low priority read w/o actually reading data
             **** into host menory.
             **** Because this Modifier may send a         
             **** list of blocks to prefetch,'QueueDeviceIO()'
             **** is used (instead of simply returning OK)!  
             **************************************************** */
          case IOCM_READ_PREFETCH:
            QueueDeviceIO(pDrive, pIorb);
            break;

            /* **************************************************** 
             **** default:                                 
             **** Handles all other Command Modifier of 
             **** Command Code 'IOCC_EXECUTE_IO'.
             **************************************************** */
          default:
            DEBMSG2(DBG_ALL,dszIorbUnknown,CommandCode,CommandModifier);
            DBSTOP();
            NotifyIorb(pIorb, IOERR_CMD_NOT_SUPPORTED);
            break;
        }
        break;

      case IOCC_ADAPTER_PASSTHRU:               /* SCSI commands */
        switch( CommandModifier )
        {
          case IOCM_EXECUTE_CDB:                /* non-SCSI */
          default:
            DEBMSG2(DBG_ALL,dszIorbUnknown,CommandCode,CommandModifier);
            DBSTOP();
            NotifyIorb(pIorb, IOERR_CMD_NOT_SUPPORTED);
            break;
        }
        break;

      case IOCC_UNIT_STATUS:                    /* set/get state of unit */
        switch( CommandModifier )
        {
            /* **************************************************** 
             * **** IOCM_GET_UNIT_STATUS:                          
             * **** Request the state of some unit flags           
             * **** (UF_READY, UF_DEFECTIVE, etc.)                 
             * **************************************************** */
          case IOCM_GET_UNIT_STATUS:
            GetUnitStatus(pDrive, (PIORB_UNIT_STATUS)pIorb);
            break;

          case IOCM_GET_LOCK_STATUS:            /* never removable! */
            DEBMSG(DBG_ALL,dszIorbNoGetLock);
            NotifyIorb(pIorb, IOERR_CMD_NOT_SUPPORTED);
            break;

          case IOCM_GET_MEDIA_SENSE:            /* optional */
          case IOCM_GET_CHANGELINE_STATE:       /* optional */
          default:
            DEBMSG2(DBG_ALL,dszIorbUnknown,CommandCode,CommandModifier);
            DBSTOP();
            NotifyIorb(pIorb, IOERR_CMD_NOT_SUPPORTED);
            break;
        }
        break;

      case IOCC_DEVICE_CONTROL:                 /* SCSI commands */
        switch( CommandModifier )
        {
          case IOCM_EJECT_MEDIA:                /* optional: not removable */
          case IOCM_LOCK_MEDIA:                 /* optional */
          case IOCM_UNLOCK_MEDIA:               /* optional */
          case IOCM_ABORT:                      /* optional */
          case IOCM_RESET:                      /* optional */
          default:
            DEBMSG2(DBG_ALL,dszIorbUnknown,CommandCode,CommandModifier);
            DBSTOP();
            NotifyIorb(pIorb, IOERR_CMD_NOT_SUPPORTED);
            break;
        }
        break;

      default:                                  /* CommandCode unknown */
        DEBMSG1(DBG_ALL,dszIorbUnknCmd,CommandCode);
        DBSTOP();
        NotifyIorb(pIorb, IOERR_CMD_NOT_SUPPORTED);
        break;
    }

    return;
}






/* **********************************************************************
 * **** Task Time Processing (filter) ***********************************
 * ******************************************************************* */


/*# ----------------------------------------------------------------------
 * CALL
 *      NotifyIorb(pIorbh,ErrorCode)
 * PARAMETER
 *      pIorbh                  I/O request buffer
 *      ErrorCode               see DDK
 * RETURNS
 *      (nothing)
 *
 * DESCRIPTION
 *      Return IORB to OS, setting errorcode and flags. 'pIorbh' may
 *      be head of linked list or single request.
 *
 * REMARKS
 */
PUBLIC VOID NEAR _Cdecl
NotifyIorb(PIORBH const pIorbh,USHORT ErrorCode)
{
    DEBMSG2(DBG_LEVEL2|DBG_PATH,dszNotify,(PVOID)pIorbh,ErrorCode);
    if( ErrorCode )
    {
        DEBMSG1(DBG_ALL,dszNotifyError,ErrorCode);
    }

    /* Set IORB_ERROR in pIorb->Status if there is a non-zero
     * error code AND the RECOVERED ERROR bit is not set in
     * the IORB status field. */

    pIorbh->ErrorCode = ErrorCode;
    pIorbh->Status |= (IORB_DONE
                       | ((ErrorCode && !(pIorbh->Status & IORB_RECOV_ERROR))
                          ? IORB_ERROR : 0));

    if( (pIorbh->RequestControl & IORB_ASYNC_POST) )
        CallADD((PVOID)(pIorbh->NotifyAddress), pIorbh);

    return;
}




/*# ----------------------------------------------------------------------
 * IorbEntry(pIorb)
 *
 * PARAMETER
 *      pIorb                   I/O request buffer
 *
 * RETURNS
 *      (nothing)
 *
 * DESCRIPTION
 *      Entry for IORB type of I/O.
 *      Calls FilterHandler() for any IORB to a basedisk and
 *      selects subroutines for all hostdrives.
 *      IOCC_CONFIGURATION is special, as it is for the complete driver.
 *
 * REMARKS
 */
PUBLIC void FAR _loadds _Cdecl
IorbEntry(PIORB const pIorb)
{
    PVOID const dpp = UH2PPTR(pIorb->UnitHandle);

    if( pIorb->CommandCode == IOCC_CONFIGURATION )
    {
        ArrayHandler(NULL, pIorb);
    }
    else if( dpp >= (PVOID)&apBDisk[0]  &&  dpp < (PVOID)&apBDisk[MAX_PDEV] )
    {
        FilterHandler((PVOID)*(PULONG)dpp, pIorb);
    }
    else if( dpp >= (PVOID)&apHDrv[0]  &&  dpp < (PVOID)&apHDrv[MAX_HDRV] )
    {
        ArrayHandler((PVOID)*(PULONG)dpp, pIorb);
    }
    else
    {
        /* He, what do you mean?  There is no such device! */

        NotifyIorb(pIorb, IOERR_CMD_OS_SOFTWARE_FAILURE);
    }
    return;
}






/* **********************************************************************
 * **** Timer Context Processing ****************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * CALL
 *      Timer(void)
 * PARAMETER
 *      (none)
 *      ulTimer
 *      anpUnit
 *
 * RETURNS
 *      (nothing)
 *
 * DESPRIPTION
 *      Increments 'counter' in all unit structures
 *      and sends stop if 'counter' reached limit.
 *
 * REMARKS
 *      Called by assembler stub which saves/restores registers
 *      but doesn't (!) enable interrupts.
 */
#if defined(_MSC_VER)
# pragma optimize("lge",off)
#endif
PUBLIC void NEAR _Cdecl
Timer(void)
{
    static USHORT frac = 10;
    static USHORT beepfrac = 10;
    USHORT      i;


    /* Try to yank all queued VRIOs to their device. */

    for( i = 0; i < VRIO_PER_TIMER; ++i )
    {
        PVRIO const vrio = DequeueVrio();

        if( vrio == NULL )
            break;                              /* no more work */
        if( vrio->vrd->worker(vrio->vrd, vrio) )
        {
            InsertVrio(vrio);                   /* back to *start* of queue */
            break;
        }
    }


    if( --frac == 0 )
    {
        ++ulTimer;                              /* free running counter */
        frac = 10;                              /* restart */

        /* Continue BUILD process on every VRDEVICE requiring it.  Only
         * do it after loading has completed but before shutdown has
         * been initiated. */

        if( (fDriverFlags & DF_INIT2DONE) != 0
            &&  (fDriverFlags & DF_SHUTDOWN1) == 0
            &&  (fDriverFlags & DF_SETUPACTIVE) == 0
            &&  cBuildProcess != 0
            &&  ulTimer > 45 )
        {
            i = DevHelp_ArmCtxHook(0, hdBuildHook);
            if( i != 0 )
            {
                DEBMSG1(DBG_LEVEL1,dszBE_ACH,i);
            }
        }


        if( --beepfrac == 0 )
        {
            /* Check state of all drives and call DevHelp_Beep() if any
             * drive is in state 'ERROR' or 'FAIL'. */

            if( cBeeps != 0  &&  (fDriverFlags & DF_BEEPDISABLED) == 0 )
                DevHelp_Beep(880, 200);
            beepfrac = 10;
        }

        /* Now start some IORBs for every host drive. */

        for( i = 0; i < cHDrv; ++i )
            StartManyIorb(apHDrv[i]);
    }

    return;
}
#if defined(_MSC_VER)
# pragma optimize("",on)
#endif
