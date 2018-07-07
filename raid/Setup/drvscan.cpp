/*
 * $Source: r:/source/driver/raid/setup/RCS/drvscan.cpp,v $
 * $Revision: 3.2 $
 * $Date: 2006/01/16 21:18:45 $
 * $Locker:  $
 *
 *
 *	Seperate thread to scan OS/2 devices and existing RAID drives.
 *
 *
 * $Log: drvscan.cpp,v $
 * Revision 3.2  2006/01/16 21:18:45  vitus
 * - define NORAID5: generate code for current VRAID.FLT, not the one
 *   that might follow and support RAID5 (if it ever will)
 *
 * Revision 3.1  2003/09/24 10:43:06  vitus
 * - added RAID 5 code (new major because of "stopped" development)
 *
 * Revision 2.13  2001/01/30 00:37:15  vitus
 * - AnalyseVRDev2: now correctly passes valid/invalid to addChild()
 * - WUM_INVCHILD: removed ioStartRebuild(), handled by filter
 *
 * Revision 2.12  2001/01/15 01:50:39  vitus
 * - implemented WUM_INVCHILD, sets the child to invalid and posts the tree back
 *
 * Revision 2.11  2000/11/07 01:31:24  vitus
 * - Verbose() calls use modname[] string
 *
 * Revision 2.10  2000/08/21 08:14:36  vitus
 * - modifications for changed addChild() prototype
 * - uses ioStartRebuild method of array instead of calling
 *   DriverArrayRebuild() directly
 *
 * Revision 2.9  2000/08/05 02:57:16  vitus
 * - added detection of rebuild state
 * - added processing of WUM_REBUILD_DRV
 *
 * Revision 2.8  2000/06/15 01:18:23  vitus
 * - method 'setFltHd' is called for hostdrives.  Should be used for all
 *   arrays but this needs another IOCtl on vraid.flt
 *
 * Revision 2.7  2000/05/28 17:42:56  vitus
 * - adjusted verbosity levels
 * - moved partition analysis/creation to pcreate.cpp
 *
 * Revision 2.6  2000/05/13 16:43:59  vitus
 * - clarified SayArea() messages
 * - FindFreeExtSpace: removed 'no free entry' abort because there are never
 *   more than 2 used entries and even if: the included extended partitions have
 *   to be analysed.
 *
 * Revision 2.5  2000/04/15 23:50:12  vitus
 * - IsProvidedByFlt: detection of OS/2 disk/filter device now in seperate function
 *
 * Revision 2.4  2000/04/10 01:09:00  vitus
 * - converted calls to WinMessageBox() to MyMessageBox()
 *
 * Revision 2.3  2000/03/17 02:11:33  vitus
 * - create extended partions arouch 7C partitions via addOwn()
 *
 * Revision 2.2  2000/03/09 23:40:16  vitus
 * - scans and creates extended partitions correctly.
 *   Well, at least it creates a PHYSDEV in the middle between two logical
 *   drives.  To create a "logical" vraid partition out of a clear disk it's a
 *   long, long way...
 *
 * Revision 2.1  2000/02/18 00:15:33  vitus
 * - major changes to find VRAID partitions inside extended partitions
 *   and to create those partitions.
 * - uses PartTable class to handle partition sectors.
 *
 * Revision 1.24  1999/12/14 00:28:52  vitus
 * - more error logging
 *
 * Revision 1.23  1999/10/01 01:29:37  vitus
 * - AllocateOs2Devices: added code to detect OS/2 devices which are accessable
 *   through VRAID.FLT as PHYSDEVICEs
 *
 * Revision 1.22  1999/07/05 00:54:24  vitus
 * - calls addChild() with "config sector valid" flag
 * - remove DriverEndSetup() call, this is done (or not done) in DriverClose()
 *
 * Revision 1.21  1999/06/21 01:29:42  vitus
 * - corrected english spelling
 *
 * Revision 1.20  1999/06/20 17:16:27  vitus
 * - AnalyseVRDev/AnalyseVRDev2: don't remove childs from global device table
 *   before it's clear that the parent will be created.  A "creation delayed"
 *   did lead to forgotten disks!
 *
 * Revision 1.19  1999/06/19 22:01:52  vitus
 * - minor corrections to keep compiler happy
 * - Scanning tests fSetupMode instead of calling DriverVersion()
 * - OS/2 devices are opened here and a VOS2Disk constructed with the handle.
 *   This way errors may be detected and reported.
 *
 * Revision 1.18  1999/06/02 02:06:57  vitus
 * - AnalyseVRDev2: validates that all childs contain the same SEC_VRDEV2 contents
 *   and updates status variables if not.  ValidateVSec2() implemented.
 * - BuildDevTree: apDskInfo[i] is now cleared by Analyse*() itself.  Only do
 *   it here if no Analyse*() seems to be correct
 * - WUM_DELDRV: adds all childs to display before deleting the parent
 *
 * Revision 1.17  1999/05/24 19:56:45  vitus
 * - completed WUM_REMCHILD: may now add the deleted device itself to container
 *   if it's an PYHSDEVICE via vraid.flt.  In this case the partition can't be
 *   deleted and VOS2Disk is an impossible solution.
 *
 * Revision 1.16  1999/05/13 23:16:49  vitus
 * - implemented WUM_REMCHILD
 *
 * Revision 1.15  1999/05/12 01:29:30  vitus
 * - WUM_DELPDEV: should detect VORdev vs. VFRDev and create the appropiate
 *   disk object
 * - RDFLAG* defines now unified (same as dsklayout.h)
 * - AnalyseVRDev2: new sector layout
 *
 * Revision 1.14  1999/04/30 03:15:35  vitus
 * - checks minimum size of VRAID partition (10MB)
 * - when scanning free disk areas: start of n. areas (n>1) was undefined)
 * - calls DriverStartSetup() and DriverEndSetup()
 *
 * Revision 1.13  1999/03/24 23:08:46  vitus
 * - VMissing always added as "invalid"
 * - WinPostMsg() instead of WinSendMsg()
 *
 * Revision 1.12  1999/03/11 03:24:19  vitus
 * - include seperate class interface headers
 * - corrected partition creation code
 * - VMirror stuff enabled
 *
 * Revision 1.11  1999/03/01 02:27:44  vitus
 * - include dskinfo.hpp for drvbuild.h
 * - removed many routine just because of C++ design
 *
 * Revision 1.10  1999/02/09 03:50:23  vitus
 * - switched to C++
 * - moved analyse of SEC_VRDEV* to AnalyseVRDev resp AnalyseVRDev2
 *
 * Revision 1.9  1998/12/13 23:28:41  vitus
 * - changed as global headers moved to /source/Lib
 * - AllocateOs2Devices: detects which OS/2 devices are VRAID drives
 *   and skips those devices
 * - BuildDevTree: detects which array drives are used by OS/2 (see
 *   AllocateOs2Devices) and forwards OS/2 device's flags to array drive
 * - removed -*- beta -*- message, no longer true
 *
 * Revision 1.8  1998/11/21 20:05:16  vitus
 * - BUGFIX: reenable AllocateOs2Devices() if BETA_PKG
 * - creation of VRAID partition -> PHYSDEV = 0MB, fixed?
 *
 * Revision 1.7  1998/11/05 00:29:03  vitus
 * - corrected 'VRAID.FLT found' message: it is possible to modify existent
 *   PHYSDEVICEs (and higher), but in this beta it isn't possible to create
 *   new PHYSDEVICEs (that makes a difference!)
 *
 * Revision 1.6  1998/11/03 03:07:57  vitus
 * - modified for DSKINFO change: 'avail' element changed position
 * - BuildDevTree: create new devices as 'avail = 1' and change this if
 *   any child is 'avail = 0'
 *
 * Revision 1.5  1998/09/27 01:50:10  vitus
 * - added -+- beta -+- message box explaining current restrictions
 * - implemented AllocateFltDevices: asks raid.flt
 * - added DSKACC_FLT to all the places
 *
 * Revision 1.4  1998/06/09 01:17:05  vitus
 * - added existence of DSKTYPE_SINGLE VRAID sectors
 * - corrected sector no/offset when reading higher VRAID sectors
 * - BuildDevTree: drive size calculation now based on VRAID sector contents
 *   (not found childs)
 * - added WUM_DELPDEV: DeletePhysdev(), RemovePartition()
 * - UpdateDsklayout: corrected saved drive sizes
 * - added WUM_DELDRV: RemoveDrive()
 * - DoPartitionIo() handles missing childs
 *
 * Revision 1.3  1998/05/27 01:55:06  vitus
 * - DoAbsIo(), DoPartitionIo()
 * - checks MBR signature
 * - CreatePartition(), UpdatePhys(), CreatePhysdev(), UpdateDsklayout()
 *
 * Revision 1.2  1998/05/15 01:07:22  vitus
 * - create each DSKINFO structure as own memory object
 *
 * Revision 1.1  1998/05/11 00:32:25  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2001
 */
static char const vcid[]="$Id: drvscan.cpp,v 3.2 2006/01/16 21:18:45 vitus Exp $";
static char const modname[]="DrvScan";

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define __IBMC__	0
#define INCL_DOS
#define INCL_DOSDEVIOCTL
#define INCL_ERRORS
#define INCL_WIN
#define INCL_GPI
#include <os2.h>

extern "C" {
#include "../../../Lib/defines.h"
#include "../../../Lib/verbose.h"
#include "../../../Lib/physdsk.h"
}

#include "VSetup.h"
#include "resources.h"
#include "drvif.h"
#include "dskinfo.hpp"
#include "drvbuild.h"
#include "pcreate.h"

#include "PartTable.hpp"
#include "Single.hpp"
#include "Chain.hpp"
#include "Stripe.hpp"
#include "Mirror.hpp"
#if !defined(NORAID5)
# include "Raid5.hpp"
#endif
#include "Missing.hpp"




#define WC_DRVSCAN	"DrvscanWindow"
#define MAXDRIVES	(MAXCHILDREN*2)





typedef struct _WORK {
    PTHR_DRVSCAN self;
    HAB		hab;				/* to retrieve errors */

    VOS2Disk **	apOs2Dsk;			/* list of OS/2 accessable disks */
    ULONG	cOs2Dsk;
    VRDev     **apDskInfo;			/* list of devices with VRAID struc*/
    ULONG	cDskInfo;
    VRDev     **apEndPts;			/* analysis complete */
    ULONG	cEndPts;
} WORK, * PWORK;


PUBLIC	USHORT	usHostId = 0;

PRIVATE struct _OS2DEVID {
    ULONG	os2idx;
    UCHAR	fl;				/* 0: table entry free */
    UCHAR	avail;
    DEVID	id;
    USHORT	hd;
} ahsOs2Devid[MAXDRIVES];






/* **********************************************************************
 * **** Private Routines ************************************************
 * ******************************************************************* */


/*# ----------------------------------------------------------------------
 * SetArrayInfo(array_devid,array_flthd,os2idx,avail)
 *
 * PARAMETER
 *	array_devid	device ID of array drive
 *	array_flthd	vraid.flt handle of array drive
 *	os2idx		index of OS/2 device residing here (PDsk*)
 *			or -1u if no host drive
 *	avail		0: locked by OS/2
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Stores some information gotten while scanning OS/2 devices on to
 *	devices available via VRAID.FLT.
 *
 * REMARKS
 */
PRIVATE VOID
SetArrayInfo(DEVID const array_devid, USHORT const array_flthd,
	     ULONG const os2idx, UCHAR const avail)
{
    unsigned	i;

    for( i = 0; i < MAXDRIVES; ++i )
	if( ahsOs2Devid[i].fl == 0 )
	{
	    ahsOs2Devid[i].fl = 1;
	    ahsOs2Devid[i].os2idx = os2idx;
	    memcpy(ahsOs2Devid[i].id, array_devid, sizeof(array_devid));
	    ahsOs2Devid[i].hd = array_flthd;
	    ahsOs2Devid[i].avail = avail;
	    break;
	}

    return;
}




/*# ----------------------------------------------------------------------
 * QueryArrayInfo(array_devid)
 *
 * PARAMETER
 *	array_devid	device ID of array drive
 *	os2idx		index of OS/2 device residing here (PDsk*)
 *	avail		0: locked by OS/2
 *
 * RETURNS
 *	struct _OS2DEVID *
 *
 * DESCRIPTION
 *	Retrieves information stored by SetArrayInfo().
 *
 * REMARKS
 */
PRIVATE struct _OS2DEVID *
QueryArrayInfo(DEVID const * const array_devid)
{
    unsigned	i;

    for( i = 0; i < MAXDRIVES; ++i )
	if( ahsOs2Devid[i].fl != 0
	    &&  memcmp(ahsOs2Devid[i].id, array_devid, sizeof(array_devid)) == 0 )
	{
	    return &ahsOs2Devid[i];
	}

    return NULL;
}




/*# ----------------------------------------------------------------------
 * IsProvidedByFlt(newdev, acnt, ahd[], pcnt, phd[])
 *
 * PARAMETER
 *	newdev			OS/2 disk to test
 *	acnt, ahd[]		array handles
 *	pcnt, phd[]		PHYSDEV handles
 *
 * RETURNS
 *	TRUE			newdev is accessable via VRAID.FLT
 *	FALSE			normal disk
 *
 * DESCRIPTION
 *	Try to figure out whether 'newdev' is an array or PHYSDEV or just
 *	any other disk.
 *
 * REMARKS
 */
PRIVATE BOOL
IsProvidedByFlt(VOS2Disk * const newdev,
		USHORT acnt, USHORT ahd[], USHORT pcnt, USHORT phd[])
{
    PVRAID_ARRAYINFO_DATA * ainfo;
    PVRAID_PHYSINFO_DATA * pinfo;
    USHORT	j;
    BOOL	result;
    BOOL	isarray = FALSE;
    USHORT	accessed_array;
    BOOL	isphys = FALSE;
    APIRET	rc;


    /* Allocate and initialize tables to hold array and physdev informations. */

    ainfo = new PVRAID_ARRAYINFO_DATA[acnt];
    for( j = 0; j < acnt; ++j )
    {
	ainfo[j] = new VRAID_ARRAYINFO_DATA;
	memset(ainfo[j], 0, sizeof(VRAID_ARRAYINFO_DATA));
    }

    pinfo = new PVRAID_PHYSINFO_DATA[pcnt];
    for( j = 0; j < pcnt; ++j )
    {
	pinfo[j] = new VRAID_PHYSINFO_DATA;
	memset(pinfo[j], 0, sizeof(VRAID_PHYSINFO_DATA));
    }


    if( acnt != 0 )				/* if vraid.flt knows an array... */
    {
	unsigned	retry;
	unsigned	flt_changes = 0;
	PUCHAR		dummybuffer = new UCHAR[SECTOR_SIZE];

	for( retry = 0; retry < 3; ++retry )
	{
	    VRAID_ARRAYINFO_DATA	this_info;
	    USHORT			j;

	    for( j = 0; j < acnt; ++j )
		rc = DriverArrayinfo(ahd[j], ainfo[j]);

	    rc = newdev->read(0, 1, dummybuffer);

	    for( j = 0; j < acnt; ++j )
	    {
		Verbose(3,modname, "Testing array %u", j);
		rc = DriverArrayinfo(ahd[j], &this_info);
		if( rc != 0 )
		    Verbose(1,modname, "Array Info %u - rc %lu", j, rc);
		else if( this_info.reqcnt > ainfo[j]->reqcnt )
		{
		    Verbose(2,modname,
			    "Array %u accessed when reading OS/2 Drive %lu",
			    j, newdev->queryIndex());
		    ++flt_changes;
		    accessed_array = j;
		}
	    }
	    if( flt_changes == 1 )
	    {
		isarray = TRUE;
		assert( accessed_array <= acnt );
		break;
	    }
	    else if( flt_changes == 0 )
		break;			/* definitely no array */

	    DosSleep(1000);		/* wait for peace :-) */
	}
	delete[] dummybuffer;
    }

    if( !isarray )
    {
	unsigned	retry;
	unsigned	flt_changes = 0;
	PUCHAR		dummybuffer = new UCHAR[SECTOR_SIZE];

	for( retry = 0; retry < 3; ++retry )
	{
	    VRAID_PHYSINFO_DATA	this_info;
	    USHORT		j;

	    for( j = 0; j < pcnt; ++j )
		rc = DriverPhysinfo(phd[j], pinfo[j]);

	    rc = newdev->read(0, 1, dummybuffer);

	    for( j = 0; j < pcnt; ++j )
	    {
		Verbose(3,modname, "Testing Physdev %u", j);
		rc = DriverPhysinfo(phd[j], &this_info);
		if( rc != 0 )
		    Verbose(1,modname, "Physdev Info %u - rc %lu", j, rc);
		else if( this_info.reqcnt > pinfo[j]->reqcnt )
		{
		    Verbose(2,modname,
			    "PHYSDEV %u accessed when reading OS/2 disk %lu",
			    j, newdev->queryIndex());
		    ++flt_changes;
		    /*accessed_phys = j;*/
		}
	    }
	    if( flt_changes == 1 )
	    {
		isphys = TRUE;
		break;
	    }
	    else if( flt_changes == 0 )
		break;			/* definitely no array */

	    DosSleep(1000);		/* wait for peace :-) */
	}
	delete[] dummybuffer;
    }

    if( isarray  ||  isphys )
    {
	if( isarray )
	    SetArrayInfo(ainfo[accessed_array]->id, ahd[accessed_array],
			 newdev->queryIndex(), newdev->isWritable());
	result = TRUE;				/* accessable through VRAID.FLT! */
    }
    else
    {
	result = FALSE;
    }


    /* Remove any memory allocated in LIFO manner. */

    for( j = 0; j < acnt; ++j )
	delete ainfo[j];
    delete[] ainfo;
    for( j = 0; j < pcnt; ++j )
	delete pinfo[j];
    delete[] pinfo;

    return result;
}




/*# ----------------------------------------------------------------------
 * AllocateFltDevices(work)
 *
 * PARAMETER
 *	work		thread data
 *
 * RETURNS
 *	count of found devices
 *
 * DESCRIPTION
 *	Retrieve information about all devices accessible via
 *	VRAID.FLT.
 *
 * REMARKS
 */
PRIVATE ULONG
AllocateFltDevices(PWORK const work)
{
    APIRET	rc;
    ULONG	i, idx;
    USHORT	cnt;
    PUSHORT	list;

    Verbose(3,modname, "AllocateFltDevices");

    idx = work->cDskInfo;			/* new index: behind already found
						   devices (none) */
    do
    {
	rc = DriverPhyslist(&cnt, &list);
	if( rc )
	{
	    Verbose(1,modname, "DriverPhyslist - rc %lu", rc);
	    cnt = 0;
	    break;
	}

	for( i = 0; i < cnt; ++i )
	{
	    work->apDskInfo[idx] = new VFRDev(list[i]);

	    ++work->cDskInfo;			/* keep this device! */

	    Verbose(2,modname, "FLT drive capacity: %lu MBytes",
		    work->apDskInfo[idx]->querySize() / 2ul / 1024);

	    ++idx;
	} /* end[while(every device)] */
    }
    while(0);					/* end[OS/2 devices] */

    return idx;
}




/*# ----------------------------------------------------------------------
 * AllocateOs2Devices(work)
 *
 * PARAMETER
 *	work		thread data
 *
 * RETURNS
 *	count of found OS/2 devices
 *
 * DESCRIPTION
 *	Retrieve information about all devices accessible via
 *	the OS/2 API.
 *
 * REMARKS
 */
PRIVATE ULONG
AllocateOs2Devices(PWORK const work)
{
    APIRET		rc;
    ULONG		i, os2cnt;
    USHORT		j;
    USHORT		arraycnt = 0;
    PUSHORT		arrayhd = NULL;
    USHORT		physcnt = 0;
    PUSHORT		physhd = NULL;


    Verbose(3,modname, "AllocateOs2Devices");

    /* For consistent display we need to now which array drive
     * and which PHYSDEV is which OS/2 device.  What we need for this
     * purpose:
     * - a handle for every array drive and every PHYSDEV
     * - room to keep at least a counter for each (we keep the whole info) */

    if( (fSetupMode & SM_FLTAVAIL) )
    {
	do
	{
	    rc = DriverArraylist(&arraycnt, &arrayhd);
	    if( rc )
	    {
		Verbose(1,modname, "DriverArraylist - rc %lu", rc);
		arraycnt = 0;
		break;
	    }

	}
	while( 0 );
	do
	{
	    rc = DriverPhyslist(&physcnt, &physhd);
	    if( rc )
	    {
		Verbose(1,modname, "DriverPhyslist - rc %lu", rc);
		physcnt = 0;
		break;
	    }

	}
	while( 0 );
    }


    rc = PDskEnum(&os2cnt);
    if( rc )
    {
	Verbose(1,modname, "PDskEnum - rc %lu", rc);
	os2cnt = 0;
    }
    for( i = 0; i < os2cnt; ++i )
    {
	/* Open physical device, this may fail! */

	ULONG	handle;
	rc = PDskOpen(i, &handle);
	if( rc != 0 )
	{
	    Verbose(1,modname, "PDskOpen(%lu) - rc %lu", i, rc);
	    MyMessageBox(2, HWND_DESKTOP,
			 "An OS2 disk cannot be accessed.  There may be an"
			 " instance of FDISK running somewhere.\n"
			 "Please don't change drive configuration but end"
			 " VSetup and any disk management software like FDISK"
			 " and restart VSetup!");
	    continue;
	}

	VOS2Disk * newdev = new VOS2Disk(i, handle);


	/* Try to determine whether this device is provided by
	 * VRAID.FLT.  Do this by reading some sectors and comparing
	 * VRAID's request counters before and after the reading.
	 * OBS: as we live in the multitasking world some other
	 * process may by writing to a disk while we are don¡ng
	 * this stuff.  It's ugly but can't be detected for sure.
	 * Just do some consistency checks and retry if they fail. */

	if( (fSetupMode & SM_FLTAVAIL) )
	    if( IsProvidedByFlt(newdev, arraycnt, arrayhd, physcnt, physhd) )
	    {
		delete newdev;
		continue;
	    }



	/* Keep this device: enter in table and increase counter. */

	work->apOs2Dsk[work->cOs2Dsk] = newdev;
	++work->cOs2Dsk;

    } /* end[for(every device)] */


    return os2cnt;
}






/* **********************************************************************
 * **** Routines to analyse VRAID own sectors ***************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * AnalyseVRDev(work,dsk,sec)
 *
 * PARAMETER
 *	work		contains disk data (among other things)
 *	dsk		which device was read?
 *	sec		sector contents to analyse
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *
 * REMARKS
 */
PRIVATE void
AnalyseVRDev(PWORK const work,VRDev * dsk,PSEC_VRDEV const sec)
{
    USHORT	crc;

    crc = Crc16(sec, SECTOR_SIZE-2);
    if( crc != sec->crc )
    {
	Verbose(1,modname, "CRC error in VRAIDDEVICE sector (%#x, %#x)",
		crc,sec->crc);
	work->apEndPts[work->cEndPts++] = dsk;
	return;
    }


    /* This is a valid VRAID sector.  It describes a new level
     * of administration and will therefore result in a new DSKINFO. */

    USHORT	j, k;
    VRDrive	*newdrv;

    switch( sec->u.s.type )
    {
      case RDTYPE_CHAIN:
	newdrv = new VChain(sec->u.s.id, sec->u.s.children);
	break;

      case RDTYPE_STRIPE:
	newdrv = new VStripe(sec->u.s.id, sec->u.s.children);
	break;

      case RDTYPE_SINGLE:
	newdrv = new VSingle(sec->u.s.id);
	break;

      default:
	Verbose(1,modname, "Unknown VRAID type %u", sec->u.s.type);
	work->apEndPts[work->cEndPts++] = dsk;
	return;
    }


    /* Link children to parent. */

    for( j = 0; j < sec->u.s.children; ++j )
    {
	Boolean	found = False;

	for( k = 0; k < work->cDskInfo; ++k )
	{
	    if( work->apDskInfo[k] == NULL )
		continue;			/* skip empty entry */

	    if( memcmp(work->apDskInfo[k]->queryID(),
		       sec->u.s.child[j].id, sizeof(DEVID)) == 0 )
	    {
		found = True;
		newdrv->addChild(work->apDskInfo[k], True);

		break;
	    }
	}
	if( found == False )
	{
	    VRDev *	dummy = new VMissing(sec->u.s.child[j].id,
					     sec->u.s.child[j].size);
	    newdrv->addChild(dummy, False, 0);
	}
    }


    /* Remove children from search table as they may not be children
     * of two parents. */

    for( j = 0; j < newdrv->queryChildren(); ++j )
    {
	VRDev *	child = newdrv->getChild(j);

	for( k = 0; k < work->cDskInfo; ++k )
	{
	    if( work->apDskInfo[k] == child )
	    {
		work->apDskInfo[k] = NULL;
		break;
	    }
	}
    }



    struct _OS2DEVID * const info = QueryArrayInfo(newdrv->queryID());
    if( info != 0 )
    {
	if( info->avail == 0 )
	    newdrv->forceUnwritable();
	newdrv->setFltHd(info->hd);
    }
    if( (sec->u.s.flags & RDFLAG_HOSTDRIVE) )
    {
	if( info != 0 )
	    newdrv->setHostdrive(info->os2idx);
	else
	    newdrv->setHostdrive();
	work->apEndPts[work->cEndPts++] = newdrv; 	/* reached host drive! */
    }
    else
	work->apDskInfo[work->cDskInfo++] = newdrv;

    return;
}




/*# ----------------------------------------------------------------------
 * ValidateVSec2(child,sec)
 *
 * PARAMETER
 *	child		child to validate
 *	sec		configuration space of sibling
 *
 * RETURNS
 *	0		OK, use this child
 *	/0		don't use it
 *
 * DESCRIPTION
 *	This routine checks whether a VRDEV is a valid child as configured
 *	in 'sec'.
 *	Normally that means that 'child' contains the identical admin.
 *	sector.  But whenever 'child' had problems during operation and
 *	had to be deactivated those sectors may differ.  Certain combinations
 *	are allowed, other not.  See below for details.
 *
 * REMARKS
 *	OBS: there is a goto!
 */
PRIVATE int
ValidateVSec2(VRDev * const child,PSEC_VRDEV2 const sec)
{
    int		result = 0;
    APIRET	rc;
    USHORT	i;
    PSEC_VRDEV2 const	childsec = new SEC_VRDEV2;


    Verbose(3,modname, "ValidateVSec2(child %p)", child);

    do
    {
	++result;
	if( childsec == NULL )
	{
	    Verbose(0,modname, "memory problem, can't validate child");
	    break;
	}

	++result;
	rc = child->read(1, 1, childsec);
	if( rc != 0 )
	{
	    Verbose(0,modname, "read error, can't validate child");
	    delete childsec;
	    break;
	}

	++result;
	if( memcmp(sec, childsec, SECTOR_SIZE) != 0 )
	{
	    if( memcmp(childsec->sectype, "VRAIDDEVICE2    ", 16) != 0 )
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

		result = -1;
		break;
	    }

	    if( memcmp(childsec->u.s.id, sec->u.s.id, sizeof(DEVID)) != 0 )
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
		if( memcmp(childsec->u.s.child[i].id, sec->u.s.child[i].id, sizeof(DEVID)) != 0 )
		    goto XABORT;
		if( childsec->u.s.child[i].size != sec->u.s.child[i].size )
		    goto XABORT;
	    }
	}

	/* When we finally reached this point 'child' may be used
	 * to build a RAID1 array. */

	result = 0;
    }
    while( 0 );

  XABORT:
    delete childsec;
    return result;
} /* end[ValidateVSec2] */




/*# ----------------------------------------------------------------------
 * AnalyseVRDev2(work,sec)
 *
 * PARAMETER
 *	work
 *	dskidx		device from which 'sec' was read
 *	sec		sector contents to analyse
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Analyses the newstyle VRAIDEVICE2 sector contents.
 * 
 * REMARKS
 */
PRIVATE void
AnalyseVRDev2(PWORK const work,VRDev * dsk,PSEC_VRDEV2 const sec)
{
    USHORT	crc;

    crc = Crc16(sec, SECTOR_SIZE-2);
    if( crc != sec->crc )
    {
	Verbose(1,modname, "CRC error in VRAIDDEVICE2 sector (%#x, %#x)",
		crc, sec->crc);
	work->apEndPts[work->cEndPts++] = dsk;
	return;
    }


    /* This is a valid VRAID sector.  It describes a new level
     * of administration and will therefore result in a new object. */

    USHORT			j, k;
    VDriveWithRedundancy *	newdrv;

    switch( sec->u.s.type )
    {
      case RDTYPE_MIRROR:
	newdrv = new VMirror(sec->u.s.id, sec->u.s.children);
#if 0
	for( j = 0; j < sec->u.s.children; ++j )
	{
	    newdrv->setSize(sec->u.s.child[j].size); /* not bigger than stored! */
	}
#endif
	break;

#if !defined(NORAID5)
      case RDTYPE_RAID5:
	newdrv = new VRaid5(sec->u.s.id, sec->u.s.children);
	break;
#endif

      default:
	Verbose(1,modname, "Unknown VRAID type %u", sec->u.s.type);
	work->apEndPts[work->cEndPts++] = dsk;
	return;
    }


    /* Link children to parent. */

    for( j = 0; j < sec->u.s.children; ++j )
    {
	Boolean	found = False;

	for( k = 0; k < work->cDskInfo; ++k )
	{
	    if( work->apDskInfo[k] == NULL )
		continue;			/* skip empty entry */

	    if( memcmp(work->apDskInfo[k]->queryID(),
		       sec->u.s.child[j].id, sizeof(DEVID)) == 0 )
	    {
		int	result;
		result = ValidateVSec2(work->apDskInfo[k], sec);
		if( result < 0 )
		{
		    Verbose(2,modname, "array creation delayed");
		    delete newdrv;		/* don't create mirror */
		    goto XABORT;
		}
		else if( result != 0 )
		    continue;			/* don't use this child */


		found = True;
		newdrv->addChild(work->apDskInfo[k], True,
				 (sec->u.s.child[j].flags & 0x01)
				 ? -1ul : sec->u.s.child[j].valid);
		break;
	    }
	}
	if( found == False )
	{
	    VRDev * dummy = new VMissing(sec->u.s.child[j].id,
					 sec->u.s.child[j].size);
	    newdrv->addChild(dummy, False, 0);
	}
    }


    /* Remove children from search table as they may not be children
     * of two parents. */

    for( j = 0; j < newdrv->queryChildren(); ++j )
    {
	VRDev *	child = newdrv->getChild(j);

	for( k = 0; k < work->cDskInfo; ++k )
	{
	    if( work->apDskInfo[k] == child )
	    {
		work->apDskInfo[k] = NULL;
		break;
	    }
	}
    }


    if( (sec->u.s.flags & RDFLAG_BUILDING) )
    {
	ULONG	bdone = (ULONG)-1;

	for( j = 0; j < sec->u.s.children; ++j )
	    if( !(sec->u.s.child[j].flags & 0x01) )
		bdone = min(bdone,sec->u.s.child[j].valid);
	newdrv->setBuild(bdone);		/* build in progress... */
    }
    else if( (sec->u.s.flags & RDFLAG_REBUILD) )
    {
	ULONG	bdone = sec->rebuild;

	newdrv->setBuild(bdone, True);		/* rebuild in progress... */
    }


    {
	struct _OS2DEVID * const info = QueryArrayInfo(newdrv->queryID());
	if( info != 0 )
	{
	    if( info->avail == 0 )
		newdrv->forceUnwritable();
	    newdrv->setFltHd(info->hd);
	}
	if( (sec->u.s.flags & RDFLAG_HOSTDRIVE) )
	{
	    if( info != 0 )
		newdrv->setHostdrive(info->os2idx);
	    else
		newdrv->setHostdrive();
	    work->apEndPts[work->cEndPts++] = newdrv; /* reached host drive! */
	}
	else
	    work->apDskInfo[work->cDskInfo++] = newdrv;
    }

  XABORT:
    return;
}




/*# ----------------------------------------------------------------------
 * FindVRaidPartition(dsk,secno,pstart,psize)
 *
 * PARAMETER
 *	dsk		OS/2 device
 *	secno		sector no of partition sector
 *	extstart	sector of first extended partition table
 *	pstart		returns VRAID start (absolute)
 *	psize		returns VRAID size
 *
 * RETURNS
 *	APIRET
 *
 * DESCRIPTION
 *	Used to search recursive after a VRAID partition.
 *
 * REMARKS
 */
PRIVATE APIRET
FindVRaidPartition(VOS2Disk * const dsk, ULONG const secno, ULONG extstart,
		   PULONG const pstart, PULONG const psize)
{
    APIRET	rc = 0;
    PartTable	pt(dsk, secno, true);


    if( !pt.isValid() )
	return rc;

    if( secno != 0  &&  extstart == 0 )
	extstart = secno;			/* first table is located here */

    for( int j = 0; j < 4  &&  *psize == 0; ++j )
    {
	switch( pt.entry[j].SysIndicator )
	{
	  case VRAID_PARTTYPE:
	    *pstart = secno + pt.entry[j].RelativeSectors;
	    *psize = pt.entry[j].NumSectors;
	    break;

	  case EXTENDED_PARTTYPE:
	    rc = FindVRaidPartition(dsk, extstart + pt.entry[j].RelativeSectors,
				    extstart,
				    pstart, psize);
	    break;

	  default:
	    break;				/* ignore empty or other types */
	}
    }
    return rc;
}




/*# ----------------------------------------------------------------------
 * BuildDevTree(work)
 *
 * PARAMETER
 *	work
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Read all structures on all devices found so far and try
 *	to create higher valued devices.
 *
 * REMARKS
 */
PRIVATE VOID
BuildDevTree(PWORK const work)
{
    APIRET	rc;
    ULONG	i;


    /* Read partition table and PHYSDEV sector of every OS/2 device
     * and create VORDev's or VOS2Drive from them. */

    for( i = 0; i < work->cOs2Dsk; ++i )
    {
	VOS2Disk * const dsk = work->apOs2Dsk[i];
	ULONG		partstart, partsize = 0;

	if( dsk == NULL )
	    continue;				/* skip empty entry */

	rc = FindVRaidPartition(dsk, 0, 0, &partstart, &partsize);

	if( partsize == 0 )
	{
	    Verbose(3,modname, "no VRAID partition on drive %u", i);

	    VOS2Drive * drv = new VOS2Drive(dsk);
	    work->apEndPts[work->cEndPts++] = drv;
	    work->apOs2Dsk[i] = NULL;
	}
	else
	{
	    Verbose(2,modname, "found VRAID partition on drive %u, size %lu", i, partsize);
	    VORDev * dev = new VORDev(dsk, partstart, partsize);
	    work->apDskInfo[work->cDskInfo++] = dev;
	}
    } /* end[for every cOs2Dsk] */



    /* Read VRAID sectors until no more devices remain (only HOSTDRIVES left).
     * As we create higher levels of complexity in every loop, "offset = 1" is
     * always the sector to read for the next step.  Difficult to explain but
     * trust me. <g>*/

    PUCHAR	buffer = new UCHAR[SECTOR_SIZE];
    for(;;)
    {
	int		found = 0;
	ULONG const	cnt = work->cDskInfo;

	for( i = 0; i < cnt; ++i )
	    if( work->apDskInfo[i] != NULL )
	    {
		++found;
		break;
	    }
	if( found == 0 )			/* nothing left? */
	    break;				/* end outer loop */

	for( i = 0; i < cnt; ++i )
	{
	    VRDev * const	dev = work->apDskInfo[i];

	    if( dev == NULL )
		continue;			/* skip empty entry */

	    rc = dev->read(1, 1, buffer);	/* config sector of next level */
	    if( rc != 0 )
	    {
		Verbose(1,modname, "read error %lu, device ignored", rc);
		delete dev;
		work->apDskInfo[i] = NULL;
		continue;
	    }

	    if( memcmp(buffer, "VRAIDDEVICE     ", 16) == 0 )
		AnalyseVRDev(work, dev, (PSEC_VRDEV)buffer);
	    else if( memcmp(buffer, "VRAIDDEVICE2    ", 16) == 0 )
		AnalyseVRDev2(work, dev, (PSEC_VRDEV2)buffer);
	    else
	    {
		work->apEndPts[work->cEndPts++] = dev;
		work->apDskInfo[i] = NULL;
	    }
	}
    } /* for(every admin sector) */
    delete[] buffer;

    return;
} /* end[BuildDevTree] */




/*# ----------------------------------------------------------------------
 * CreatePhysdev(hwnd,disk,buffer)
 *
 * PARAMETER
 *	hwnd		used for dialogs
 *	disk		create on this VOS2Disk
 *	buffer		512 byte buffer
 *
 * RETURNS
 *	APIRET
 *
 * DESCRIPTION
 *	Analyse disk contents and ask user about where to create a
 *	VRAID partition.  Update bootsector and 'disk' object.
 *
 * REMARKS
 *	It's a little bit difficult:
 *	- perhaps nothing to do (already modified)
 *	- perhaps no partition at all
 *	- perhaps many partitions and search for largest hole
 */
PRIVATE APIRET
CreatePhysdev(HWND hwnd,VOS2Disk * const disk,VORDev ** dev)
{
    int		found = -1;			/* indices */
    DSKAREA	info = {0};
    int		i;
    ULONG	ul;
    APIRET	rc;

    do
    {
	rc = FindFreeSpace(disk, &info, &found);
	if( rc != 0 )
	    break;

	if( found != -1 )
	{
	    Verbose(0,modname, "VRAID partition already there, stopping");
	    *dev = new VORDev(disk, info.area[found].start, info.area[found].size);
	    break;				/* rc still 0 */
	}

	if( info.cnt == 0 )
	{
	    Verbose(1,modname, "No room on disk, aborting");
	    MyMessageBox(3, HWND_DESKTOP,
			 "There is no room on requested disk to create a"
			 " VRAID partition.");
	    rc = ERROR_HANDLE_DISK_FULL;
	    break;
	}


	ul = WinDlgBox(HWND_DESKTOP, hwnd, AreaDlgProc,
		       NULLHANDLE, IDW_DSKAREA, &info);
	if( DID_OK != ul )
	{
	    Verbose(3,modname, "CreatePhysdev: user aborted");
	    rc = ERROR_HANDLE_DISK_FULL;
	    break;
	}

	struct _DSKAREA_ENTRY * vpart = &info.area[info.cnt];
	vpart->type = VRAID_PARTTYPE;
	vpart->bootable = 0;
	if( vpart->ptsec == 0 )
	    rc = CreatePriPartition(disk, vpart->ptsec, vpart);
	else
	{
	    vpart->bootable = 0x80;
	    rc = CreateLogPartition(disk, vpart->ptsec, vpart);
	}
	if( rc != 0 )
	    break;


	/* Last step: create VORDev object on that disk.  Start new:
	 * clear SEC_PHYSDEV and first SEC_VRAID.  Immediately rewrite
	 * configuration to create new ones. */

	PUCHAR	p = new UCHAR[2*SECTOR_SIZE];
	memset(p, 0, 2*SECTOR_SIZE);
	rc = disk->write(vpart->start, 2, p);
	if( rc != 0 )
	    Verbose(1,modname, "CreatePhysdev: can't clear SEC_PHYSDEV- rc %lu", rc);
	delete[] p;

	*dev = new VORDev(disk, vpart->start, vpart->size);
	(*dev)->ioSync();
    }
    while(0);

    return rc;
}






/* **********************************************************************
 * **** Window Management ***********************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * DrvscanWindowProc(hwnd,msg,mp1,mp2)
 *
 * PARAMETER
 *	hwnd,msg,mp1,mp2	see presentation manager reference
 *
 * RETURNS
 *	MRESULT			see presentation manager reference
 *
 * DESPRIPTION
 *	Window procedure (child of HWND_OBJECT) to coordinate drive
 *	access (scan, update).
 *
 * REMARKS
 */
PRIVATE MRESULT EXPENTRY
DrvscanWindowProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
    WORK * const work = (PWORK)WinQueryWindowPtr(hwnd, QWL_USER);
    APIRET	rc;


    switch( msg )
    {
      case WM_CREATE:
	Verbose(4,modname, "WM_CREATE -> default");
	CrcInit(0xA001);			/* CRC16 table */
	if( (rc=QueryHostId(&usHostId)) != 0 )
	{
	    Verbose(0,modname, "couldn't determine host id");
	    return 0;
	}
	break;


	/* ============================================================
	 * WM_CLOSE	-	this window closed
	 */
      case WM_CLOSE:
	Verbose(4,modname, "WM_CLOSE");
	break;


	/* ============================================================
	 * WUM_START	-	start normal processing
	 */
      case WUM_START:
	Verbose(3,modname, "WUM_START");
	{
	    if( (fSetupMode & SM_FLTAVAIL) )
	    {
		AllocateFltDevices(work);
		AllocateOs2Devices(work);
	    }
	    else
		AllocateOs2Devices(work);
	    BuildDevTree(work);

	    ULONG	i;
	    for( i = 0; i < work->cEndPts; ++i )
	    {
		VRDev * const	dev = work->apEndPts[i];

		WinPostMsg(work->self->parent, WUM_DEVTREE, dev, MPFROMHWND(hwnd));
	    }
	}
	return 0;


	/* ============================================================
	 * WUM_MKPDEV	-	create PHYSDEVICE
	 */
      case WUM_MKPDEV:
	Verbose(3,modname, "WUM_MKPDEV");
	{
	    VOS2Drive *	drive = (VOS2Drive *)PVOIDFROMMP(mp1);
	    VOS2Disk *	disk = drive->queryWhere();
	    VORDev *	rdev;

	    rc = CreatePhysdev(hwnd, disk, &rdev);
	    if( rc == 0 )
	    {
                /* That's a special case: we want to delete a VOS2Drive *without*
                 * deleting the VOS2Disk!  The ioDeleteYourself() method will
                 * be used to "mark" the object that it shouldn't delete.
                 * Kind of hack, eh? */

                drive->ioDeleteYourself();
                delete drive;

		rdev->ioSync();
		WinPostMsg(work->self->parent, WUM_DEVTREE, rdev, MPFROMHWND(hwnd));
	    }
	    else
		WinPostMsg(work->self->parent, WUM_DEVTREE, drive, MPFROMHWND(hwnd));
	}
	return 0;


	/* ============================================================
	 * WUM_DELPDEV	-	delete PHYSDEVICE
	 *
	 * Currently the PHYSDEVICE has to be accessable from OS/2, a VORDev,
	 * and VFRDev and their partitions (!) are only destroyable if
	 * booted w/o VRAID.FLT.  But there should be a possibility to
	 * destroy SEC_PHYSDEV from VFRDev and reboot to remove the partition...
	 */
      case WUM_DELPDEV:
	Verbose(3,modname, "WUM_DELPDEV");
	{
	    VRDev *	dev = (VRDev *)PVOIDFROMMP(mp1);
	    VOS2Disk *	disk = dev->queryWhere();


	    dev->ioDeleteYourself();		/* removes SEC_PHYSDEV [+partition]*/

	    if( disk != NULL )
	    {
		delete dev,	dev = NULL;

		VOS2Drive *	drive = new VOS2Drive(disk);
		WinPostMsg(work->self->parent, WUM_DEVTREE, drive, MPFROMHWND(hwnd));
	    }
	    else
	    {
		MyMessageBox(3, HWND_DESKTOP,
			     "Make sure you delete the VRAID partition by hand"
			     " after reboot.\nUse the context menu of the drives"
			     " folder for this purpose.");
		WinPostMsg(work->self->parent, WUM_DEVTREE, dev, MPFROMHWND(hwnd));
	    }
	}
	return 0;


	/* ============================================================
	 * WUM_DEVTREE:
	 *	A new devicetree has to be created (said the user).
	 *	Update disk structures, recalculate all fields of affected
	 *	structures and post this devicetree back to the drvsetup
	 *	thread (for display).
	 * OBS: all parts of the DEVTREE are already PHYSDEVs or more.
	 */
      case WUM_DEVTREE:
	Verbose(3,modname, "WUM_DEVTREE");
	{
	    VRDev *	dev = (VRDev *)PVOIDFROMMP(mp1);

	    dev->ioSync();			/* write to disk[s] */
	    WinPostMsg(work->self->parent, WUM_DEVTREE, dev, MPFROMHWND(hwnd));
	}
	return 0;


	/* ============================================================
	 * WUM_DELDRV:
	 *	Remove a VRAID drive.
	 */
      case WUM_DELDRV:
	Verbose(3,modname, "WUM_DELDRV");
	{
	    VRDrive * const	drv = (VRDrive *)PVOIDFROMMP(mp1);

	    /* Display all children which exist on disk. */

	    for( int i = 0; i < drv->queryChildren(); ++i )
	    {
		VRDev *	const	child = drv->getChild(i);

		if( child->isArray() != 0xFF )
		    WinPostMsg(work->self->parent, WUM_DEVTREE, child, MPFROMHWND(hwnd));
	    }

	    drv->ioDeleteYourself();
	    delete drv;
	}
	return 0;


	/* ============================================================
	 * WUM_REBUILD_DRV:
	 *	let vraid.flt do the work
	 */
      case WUM_REBUILD_DRV:
	Verbose(3,modname, "WUM_REBUILD_DRV");
	{
	    VDriveWithRedundancy* const drv= (VDriveWithRedundancy*)PVOIDFROMMP(mp1);
	    assert( drv->isArray()==RDTYPE_MIRROR || drv->isArray()==RDTYPE_RAID5 );
	    drv->ioStartRebuild(LONGFROMMP(mp2));
	}
	WinPostMsg(work->self->parent, WUM_DEVTREE, mp1, MPFROMHWND(hwnd));
	return 0;


	/* ============================================================
	 * WUM_CLOSEDEV:
	 *	A device isn't needed anymore.
	 */
      case WUM_CLOSEDEV:
	Verbose(3,modname, "WUM_CLOSEDEV");
	{
	    VRDev * dev = (VRDev *)PVOIDFROMMP(mp1);
	    delete dev;
	}
	return 0;


	/* ============================================================
	 * WUM_REMCHILD:
	 *	Remove a child from a disk array.
	 */
      case WUM_REMCHILD:
	Verbose(3,modname, "WUM_REMCHILD");
	{
	    Boolean	postchild = True;
	    VRDev *	child = (VRDev *)PVOIDFROMMP(mp1);
	    VDriveWithRedundancy * parent = (VDriveWithRedundancy *)PVOIDFROMMP(mp2);

	    if( child->isArray() == 0xFF )	/* VMissing!  Don't post. */
		postchild = False;

	    parent->ioRemoveChild(child);

	    VRDev * higher = parent;
	    while( higher->getParent() != 0  )
		higher = higher->getParent();
	    WinPostMsg(work->self->parent, WUM_DEVTREE, higher, MPFROMHWND(hwnd));

	    if( postchild == True )
		WinPostMsg(work->self->parent, WUM_DEVTREE, child, MPFROMHWND(hwnd));
	}
	return 0;


	/* ============================================================
	 * WUM_INVCHILD:
	 *	Invalidate a child of an array.
	 */
      case WUM_INVCHILD:
	Verbose(3,modname, "WUM_INVCHILD");
	{
	    VRDev *	child = (VRDev *)PVOIDFROMMP(mp1);
	    VDriveWithRedundancy * parent = (VDriveWithRedundancy *)PVOIDFROMMP(mp2);
	    DEVID	id;

	    memcpy(id, child->queryID(), sizeof(DEVID));
	    parent->ioInvalidChild(id, 0);	/* from the very beginning */

	    VRDev * higher = parent;
	    while( higher->getParent() != 0  )
		higher = higher->getParent();
	    WinPostMsg(work->self->parent, WUM_DEVTREE, higher, MPFROMHWND(hwnd));
	}
	return 0;


      default:
	break;
    } /* end[switch] */

    return WinDefWindowProc(hwnd, msg, mp1, mp2);
}






/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */


/*# ----------------------------------------------------------------------
 * DrvscanThread(dummy)
 *
 * PARAMETER
 *	dummy		pointer to information from parent
 *
 * RETURNS
 *	(nothing)
 *
 * DESPRIPTION
 *	Runs as a seperate thread.
 *
 * REMARK
 */
PUBLIC VOID
DrvscanThread(void *dummy)
{
    PTHR_DRVSCAN dta = (PTHR_DRVSCAN)dummy;
    PWORK	work;
    HMQ		hmq;
    HWND	hwnd;
    QMSG	qmsg;
    BOOL	bool;

    do
    {
	memset(ahsOs2Devid, 0, sizeof(ahsOs2Devid));

	work = new WORK;
	memset(work, 0, sizeof(*work));
	work->self = dta;

	work->apOs2Dsk = new VOS2Disk *[MAXDRIVES];
	memset(work->apOs2Dsk, 0, MAXDRIVES * sizeof(VOS2Disk *));

	work->apDskInfo = new VRDev *[MAXDRIVES];
	memset(work->apDskInfo, 0, MAXDRIVES * sizeof(VRDev *));

	work->apEndPts = new VRDev *[MAXDRIVES];

	if( !(work->hab = WinInitialize(0)) )
	{
	    Verbose(0,modname, "WinInitialize failed!?!");
	    break;
	}

	if( !(hmq = WinCreateMsgQueue(work->hab, 0)) )
	{
	    Verbose(0,modname, "WinCreateMsgQueue - error %#x",
		    WinGetLastError(work->hab));
	    break;
	}
	WinCancelShutdown(hmq, TRUE);		/* don't want to handle it */

	if( !WinRegisterClass(work->hab, WC_DRVSCAN, DrvscanWindowProc,
			      0, sizeof(PVOID)) )
	{
	    Verbose(0,modname, "WinRegisterClass - error %#x",
		    WinGetLastError(work->hab));
	    break;
	}

	hwnd = WinCreateWindow(HWND_OBJECT, WC_DRVSCAN, NULL,
			       0,		/* style */
			       0,0,0,0,		/* position,size */
			       NULLHANDLE,	/* owner */
			       HWND_BOTTOM,	/* behind which? */
			       0,		/* id */
			       NULL, NULL);
	if( hwnd == NULL )
	{
	    Verbose(0,modname, "WinCreateWindow - error %#x",
		    WinGetLastError(work->hab));
	    break;
	}

	bool = WinSetWindowPtr(hwnd, QWL_USER, work);
	if( bool == FALSE )
	{
	    MyMessageBox(0, HWND_DESKTOP,
			 "WinSetWindowPtr failed - error %#",
			 WinGetLastError(work->hab));
	    break;
	}

	/* Message processing loop. */

	WinPostMsg(hwnd, WUM_START, 0, 0);
	while( WinGetMsg(work->hab, &qmsg, 0L, 0, 0) )
	    WinDispatchMsg(work->hab, &qmsg);

	/* Message processing ended (WM_QUIT sent), clean up. */

	WinDestroyWindow(hwnd);
	WinDestroyMsgQueue(hmq);
	WinTerminate(work->hab);
	delete work;
    }
    while(0);

    Verbose(2,modname, "DrvScanThread: stopped");
    WinPostMsg(dta->parent, WUM_THREADENDED, dta, 0);
    return;
}
          