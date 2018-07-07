/*
 * $Source: R:/Source/driver/raid/Setup/RCS/VRDev.cpp,v $
 * $Revision: 1.16 $
 * $Date: 2000/08/07 01:33:53 $
 * $Locker:  $
 *
 *	Implements classes VRDev and direct childs.
 *
 * $Log: VRDev.cpp,v $
 * Revision 1.16  2000/08/07 01:33:53  vitus
 * - VORDev: changed class member 'hd' to 'flt_hd'
 *
 * Revision 1.15  2000/05/28 17:38:40  vitus
 * - adjusted verbosity levels
 *
 * Revision 1.14  2000/04/10 01:10:30  vitus
 * - include VSetup.h instead of proto.h
 * - converted C++ to C comments
 *
 * Revision 1.13  1999/12/14 00:28:26  vitus
 * - more error logging
 *
 * Revision 1.12  1999/11/16 22:39:18  vitus
 * - VOS2Disk: create() tries PDskLockIO() but ignores any errors
 *
 * Revision 1.11  1999/10/01 01:28:13  vitus
 * - VFRDev constructor: DriverPhysinfo() changed to DriverPhyssize()
 *
 * Revision 1.10  1999/06/20 17:04:49  vitus
 * - corrected VOS2Disk::create, didn't return a value
 *
 * Revision 1.9  1999/06/19 17:19:13  vitus
 * - added VOS2Disk::VOS2Disk(ULONG idx,ULONG handle) constructor
 *
 * Revision 1.8  1999/06/02 01:38:59  vitus
 * - VFRDev/VORDev: queryIcon() returns ICO_INVCHILDPDEV if parent says that this
 *   child is invalid
 *
 * Revision 1.7  1999/05/13 22:47:18  vitus
 * - ioRemoveParent: clear parent's admin sector
 * - ioDeleteYourself: no disk i/o
 *
 * Revision 1.6  1999/03/24 23:04:11  vitus
 * - allocateDescription() adds drive ID to description
 *
 * Revision 1.5  1999/03/15 03:27:37  vitus
 * - VORDev: create new ID when PYSDEV sector is invalid
 * - VORDev: doesn't delete theDisk if ioDeleteYourself() is called first
 *
 * Revision 1.4  1999/03/08 03:02:47  vitus
 * - VORDev deletes it's disk when destructed
 * - may now construct VORDev w/o SEC_PHYSDEV
 *
 * Revision 1.3  1999/03/06 19:55:45  vitus
 * - implemented queryIcon() methods
 *
 * Revision 1.2  1999/03/01 02:17:30  vitus
 * - many more methods to get things working again
 *
 * Revision 1.1  1999/02/24 00:18:55  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1999-2000
 */
static char const vcid[]="$Id: VRDev.cpp,v 1.16 2000/08/07 01:33:53 vitus Exp $";

#include <assert.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define __IBMC__	0
#define INCL_WINPOINTERS
#define INCL_DOS
#define INCL_DOSDEVIOCTL
#define INCL_ERRORS
#include <os2.h>

extern "C" {
#include "../../../Lib/defines.h"
#include "../../../Lib/verbose.h"
#include "../../../Lib/physdsk.h"
}
#include "VSetup.h"
#include "drvif.h"
#include "resources.h"

#include "dskinfo.hpp"

extern USHORT	usHostId;






/* **********************************************************************
 * **** VORDev::* *******************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * VORDev::VORDev(where,pstart,psize)
 *
 * PARAMETER
 *	where		disk where this thing resides
 *	pstart,psize	describes VRAID partition
 * RETURNS
 *	(nothing, C++)
 * DESCRIPTION
 *	Constructur for VORDev object.
 *
 * REMARKS
 */
VORDev::VORDev(VOS2Disk * where,ULONG pstart,ULONG psize)
{
    PSEC_PHYSDEV sec = new SEC_PHYSDEV;
    APIRET	rc;

    delete_disk = True;
    theDisk = where;
    partstart = pstart;
    partsize = psize;
    parent = NULL;
    size = 0;
    writable = theDisk->isWritable();

    do
    {
	USHORT		crc;

	rc = read(0, 1, sec);
	if( rc != 0 )
	{
	    Verbose(1, "VORDev::VORDev", "can't read PHYSDEV - rc %lu", rc);
	    break;
	}

	crc = Crc16(sec, SECTOR_SIZE-2);
	if( memcmp(sec->sectype, "PHYSDEVICE      ", 16) != 0 )
	{
	    Verbose(1, "VORDev", "missing PHYSDEV mark");
	    rc = -1ul;				/* anything != 0 */
	    break;
	}
	if( crc != sec->crc )
	{
	    Verbose(1, "VORDev", "CRC error in PHYSDEV sector (%#x, %#x)",
		    crc,sec->crc);
	    rc = -2ul;				/* anything != 0 */
	    break;
	}
	memcpy(id, sec->u.s.id, sizeof(DEVID));
	size = sec->u.s.size;
    }
    while(0);
    delete sec;

    if( rc != 0 )
    {
	/* Data couldn't be read from the disk because we are creating a
	 * new VRAID partition and a new PHYSDEV sector.  Next call will
	 * probably be ioSync() so prepare operation by inventing an ID
	 * and a size... */

	memcpy(&id[0], &usHostId, sizeof(usHostId));
	*(PULONG)&id[2] = time(NULL);

	size = psize - VRAID_ADMINSECTORS;
    }

    inv_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_INVCHILDPDEV);
    rw_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_PDEVDRIVE);
    ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDPDEV);
}




/*# ----------------------------------------------------------------------
 * VORDev::~VORDev()
 *
 * PARAMETER
 *	(none, C++)
 * RETURNS
 *	(none, C++)
 *
 * DESCRIPTION
 *	Destructur for VORDev object.
 *
 * REMARKS
 */
VORDev::~VORDev()
{
    if( delete_disk == True )
	delete theDisk;				/* no need for that disk object */

    WinDestroyPointer(rw_icon);
    WinDestroyPointer(ro_icon);
}




/*# ----------------------------------------------------------------------
 * VORDev::allocateDescription()
 *
 * PARAMETER
 *	(none)
 * RETURNS
 *	ASCIIZ
 * DESCRIPTION
 *	Allocates buffer and fill it with description of this object.
 *
 * REMARKS
 */
char *
VORDev::allocateDescription()
{
    char * descr = new char[80];
    sprintf(descr, "PHYSDEV (OS/2 Device %lu)\n"
	    "ID %02X%02X%02X%02X%02X%02X    %lu MByte",
	    theDisk->queryIndex(),
	    id[0],id[1],id[2],id[3],id[4],id[5],
	    size / 2 / 1024u);
    return descr;
}




/*# ----------------------------------------------------------------------
 * VORDev::queryIcon()
 *
 * PARAMETER
 *	(none)
 * RETURNS
 *	handle of pointer/icon
 *
 * DESCRIPTION
 *	Returns handle of special icnon for this class of devices.
 *
 * REMARKS
 */
HPOINTER
VORDev::queryIcon()
{
    if( parent != 0  &&  parent->isValidChild(this) == False )
	return inv_icon;
    else if( writable == True )
	return rw_icon;
    else
	return ro_icon;
}




/*# ----------------------------------------------------------------------
 * VORDev::ioChangeID(newid)
 *
 * PARAMETER
 *	newid		new DEVID
 * RETURNS
 *	count of errors
 *
 * DESCRIPTION
 *	Changes our own DEVID.
 *
 * REMARKS
 */
int
VORDev::ioChangeID(DEVID newid)
{
    PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
    int			errors = 0;

    memcpy(id, newid, sizeof(DEVID));		/* update object */

    do
    {
	APIRET	rc;

	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
	if( rc != 0 )
	{
	    Verbose(1,"VORDev::ioChangeID","can't read own SEC_PHYSDEV - rc %lu",rc);
	    ++errors;
	    break;
	}

	memcpy(sec->u.s.id, newid, sizeof(DEVID));
	sec->timestamp = time(NULL);		/* UTC of change */
	sec->crc = Crc16(sec, SECTOR_SIZE-2);	/* !!! */

	rc = write(0, 1, sec);
	if( rc != 0 )
	{
	    Verbose(1, "VORDev::ioChangeID", "can't write own SEC_PHYSDEV - rc %lu", rc);
	    ++errors;
	    break;
	}
    }
    while(0);

    delete sec;
    return errors;
}




/*# ----------------------------------------------------------------------
 * VORDev::ioRemoveParent()
 *
 * PARAMETER
 *	(none, C++)
 * RETURNS
 *	count of errors
 *
 * DESCRIPTION
 *	No parent means: all room in partition usable as user space.
 *
 * REMARKS
 */
int
VORDev::ioRemoveParent()
{
    PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
    int		errors = 0;
    APIRET	rc;


    /* 1st: clear administraive sector of parent. */

    memset(sec, 0, SECTOR_SIZE);
    rc = write(1, 1, sec);
    if( rc != 0 )
    {
	Verbose(1, "VORDev::ioRemoveParent", "can't clear parent SEC_VRDEV - rc %lu", rc);
	++errors;
    }


    /* 2nd: update size (no RDFLAG_HOSTDRIVE) */

    do
    {
	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
	if( rc != 0 )
	{
	    Verbose(1, "VORDev::ioRemoveParent", "can't read own SEC_PHYSDEV - rc %lu", rc);
	    ++errors;
	    break;
	}

	sec->u.s.size = size = partsize - sec->u.s.adminspace;
	sec->timestamp = time(NULL);
	sec->crc = Crc16(sec, SECTOR_SIZE-2);	/* !!! */

	rc = write(0, 1, sec);
	if( rc != 0 )
	{
	    Verbose(1, "VORDev::ioRemoveParent", "can't write own SEC_PHYSDEV - rc %lu", rc);
	    ++errors;
	    break;
	}

	parent = NULL;				/* none */
    }
    while(0);

    delete sec;
    return errors;
}




/*# ----------------------------------------------------------------------
 * VORDev::ioDeleteYourself()
 *
 * PARAMETER
 *	(none, C++)
 * RETURNS
 *	count of errors
 *
 * DESCRIPTION
 *	Destroys itself by clearing configuration sector.
 *
 * REMARKS
 */
int
VORDev::ioDeleteYourself()
{
    PUCHAR	buffer = new UCHAR[SECTOR_SIZE];
    PMBR const	mbr = (PMBR)buffer;
    APIRET	rc;
    int		errors = 0;

    delete_disk = False;			/* destructor: don't delete theDisk */

    do
    {
	memset(buffer, 0, SECTOR_SIZE);
	rc = write(0, 1, buffer);		/* clear PHYSDEV sector */
	if( rc != 0 )
	{
	    Verbose(1, "VORDev::ioDeleteYourself", "can't clear SEC_PHYSDEV - rc %lu", rc);
	    ++errors;				/* ignore errors */
	}

	int	i;
	int	found = -1;			/* indices */

	rc = theDisk->read(0, 1, buffer);
	if( rc != 0 )
	{
	    Verbose(1, "VORDev::ioDeleteYourself", "can't read MBR - rc %lu", rc);
	    ++errors;
	    break;
	}

	if( mbr->Signature != 0xAA55 )
	{
	    Verbose(1, "VORDev", "invalid MBR, done");
	    break;
	}

	for( i = 0; i < 4; ++i )
	{
	    if( mbr->PartitionTable[i].SysIndicator == VRAID_PARTTYPE )
	    {
		memset(&mbr->PartitionTable[i], 0, sizeof(mbr->PartitionTable[i]));
		found = i;
		break;
	    }
	}

	if( found == -1 )
	{
	    Verbose(0, "VORDev", "no VRAID partition, done");
	    break;				/* rc still 0 */
	}

	rc = theDisk->write(0, 1, buffer);
	if( rc != 0 )
	{
	    Verbose(1, "VORDev::ioDeleteYourself", "can't write MBR - rc %lu", rc);
	    ++errors;
	    break;
	}
    }
    while(0);

    return errors;
}




/*# ----------------------------------------------------------------------
 * VORDev::ioSync()
 *
 * PARAMETER
 *	(none, C++)
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	'this' has been changed -> update physical storage.
 *
 * REMARKS
 */
int
VORDev::ioSync()
{
    PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
    int			modified = 0;
    APIRET		rc;
    int			errors = 0;


    do
    {
	rc = read(0, 1, sec);
	if( rc )
	{
	    Verbose(1, "VORDev::ioSync", "can't read own SEC_PHYSDEV - rc %lu", rc);
	    ++errors;
	    break;
	}

	if( memcmp(sec->sectype, "PHYSDEVICE      ", 16) != 0
	    ||  Crc16(sec, SECTOR_SIZE-2) != sec->crc )
	{
	    Verbose(1, "VORDev", "new PHYSDEV sector");
	    ++modified;
	    memset(sec, 0, sizeof(*sec));

	    memcpy(sec->sectype, "PHYSDEVICE      ", 16);
	    sec->u.s.adminspace = VRAID_ADMINSECTORS;
	    size = sec->u.s.size = (partsize - sec->u.s.adminspace);

	    memcpy(sec->u.s.id, id, sizeof(DEVID));
	}

	if( size != sec->u.s.size )
	{
	    ++modified;
	    sec->u.s.size = size;
	}

	if( modified )
	{
	    sec->timestamp = time(NULL);
	    sec->crc = Crc16(sec, SECTOR_SIZE-2); /* !!! */
	    rc = write(0, 1, sec);
	    if( rc != 0 )
	    {
		Verbose(1, "VORDev::ioSync", "write(0,1,...) - rc %lu, not updated", rc);
		++errors;
		break;
	    }
	}
    }
    while(0);

    return errors;
}




/*# ----------------------------------------------------------------------
 * VORDev::querySize()
 *
 * PARAMETER
 *	(none, C++)
 * RETURNS
 *	size of device
 *
 * DESCRIPTION
 *	Guess!
 *
 * REMARKS
 */
ULONG
VORDev::querySize()
{
    return (size == 0 ? partsize : size);
}




/*# ----------------------------------------------------------------------
 * VORDev::read(block,count,buffer)
 *
 * PARAMETER
 *	block		block number on disk
 *	count		how many blocks?
 *	buffer		data from here
 * RETURNS
 *	0		OK
 *	/0		APIRET
 *
 * DESCRIPTION
 *	Read one or more sectors from configuration space of disk.  Done
 *	when analysing disk contents.
 *
 * REMARKS
 */
APIRET
VORDev::read(ULONG block,ULONG count,PVOID buffer)
{
    return theDisk->read(partstart+block, count, buffer);
}




/*# ----------------------------------------------------------------------
 * VORDev::write(block,count,buffer)
 *
 * PARAMETER
 *	block		block number on disk
 *	count		how many blocks?
 *	buffer		data from here
 * RETURNS
 *	0		OK
 *	/0		APIRET
 *
 * DESCRIPTION
 *	Write one or more sectors to configuration space of disk. Done
 *	when creating something new.
 *
 * REMARKS
 */
APIRET
VORDev::write(ULONG block,ULONG count,PVOID buffer)
{
    return theDisk->write(partstart+block, count, buffer);
}






/* **********************************************************************
 * **** VFRDev::* *******************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * VFRDev::VFRDev(flthd)
 *
 * PARAMETER
 *	flthd		how to access via VRAID.FLT
 * RETURNS
 *	(nothing, C++)
 * DESCRIPTION
 *	Constructur for VFRDev object.
 *
 * REMARKS
 */
VFRDev::VFRDev(USHORT flthd)
{
    APIRET	rc;
    PSEC_PHYSDEV sec = new SEC_PHYSDEV;

    flt_hd = flthd;
    parent = NULL;

    /* Assume this device may be changed by VSETUP.  Only 'VORDev',
     * 'VOS2Disk' and top-level 'VRDrive' objects should get an
     * 'avail = 0'. */

    writable = True;

    do
    {
	USHORT		crc;

	rc = DriverPhyssize(flt_hd, &partsize);
	if( rc != 0 )
	{
	    Verbose(1, "VFRDev::VFRDev", "DriverPhyssize - rc %lu", rc);
	    break;
	}

	rc = read(0, 1, sec);
	if( rc != 0 )
	{
	    Verbose(1, "VFRDev::VFRDev", "read(0,1,...) - rc %lu", rc);
	    break;
	}

	if( memcmp(sec->sectype, "PHYSDEVICE      ", 16) != 0 )
	{
	    Verbose(1, "VFRDev", "missing PHYSDEV mark");
	    rc = -1ul;				/* anything != 0 */
	    break;
	}

	crc = Crc16(sec, SECTOR_SIZE-2);
	if( crc != sec->crc )
	{
	    Verbose(1, "VFRDev", "CRC error in PHYSDEV sector (%#x, %#x)",
		    crc,sec->crc);
	    rc = -2ul;				/* anything != 0 */
	    break;
	}
	memcpy(id, sec->u.s.id, sizeof(DEVID));
	size = sec->u.s.size;
    }
    while(0);
    delete sec;

    inv_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_INVCHILDPDEV);
    rw_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_PDEVDRIVE);
    ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDPDEV);
}




/*# ----------------------------------------------------------------------
 * VFRDev::~VFRDev()
 *
 * PARAMETER
 *	(none, C++)
 * RETURNS
 *	(none, C++)
 *
 * DESCRIPTION
 *	Destructur for VFRDev object.
 *
 * REMARKS
 */
VFRDev::~VFRDev()
{
    WinDestroyPointer(rw_icon);
    WinDestroyPointer(ro_icon);
}




/*# ----------------------------------------------------------------------
 * VFRDev::allocateDescription()
 *
 * PARAMETER
 *	(none)
 * RETURNS
 *	ASCIIZ
 * DESCRIPTION
 *	Allocates buffer and fill it with description of this object.
 *
 * REMARKS
 */
char *
VFRDev::allocateDescription()
{
    char * descr = new char[80];
    sprintf(descr, "PHYSDEV (filter device handle %#x)\n"
	    "ID %02X%02X%02X%02X%02X%02X    %lu MByte partition",
	    flt_hd,
	    id[0],id[1],id[2],id[3],id[4],id[5],
	    partsize / 2 / 1024);
    return descr;
}




/*# ----------------------------------------------------------------------
 * VFRDev::queryIcon()
 *
 * PARAMETER
 *	(none)
 * RETURNS
 *	handle of pointer/icon
 *
 * DESCRIPTION
 *	Returns handle of special icnon for this class of devices.
 *
 * REMARKS
 */
HPOINTER
VFRDev::queryIcon()
{
    if( parent != 0  &&  parent->isValidChild(this) == False )
	return inv_icon;
    else if( writable == True )
	return rw_icon;
    else
	return ro_icon;
}




/*# ----------------------------------------------------------------------
 * VFRDev::ioRemoveParent()
 *
 * PARAMETER
 *	(none, C++)
 * RETURNS
 *	count of errors
 *
 * DESCRIPTION
 *	No parent means: all room in partition usable as user space.
 *
 * REMARKS
 */
int
VFRDev::ioRemoveParent()
{
    PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
    int		errors = 0;
    APIRET	rc;


    /* 1st: clear administrative sector of parent. */

    memset(sec, 0, SECTOR_SIZE);
    rc = write(1, 1, sec);
    if( rc != 0 )
    {
	Verbose(1, "VFRDev::ioRemoveParent",
		"can't clear parent SEC_VRDEV - rc %lu", rc);
	++errors;
    }


    /* 2nd: update size (no RDFLAG_HOSTDRIVE) */

    do
    {
	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
	if( rc != 0 )
	{
	    Verbose(1, "VFRDev::ioRemoveParent",
		    "can't read own SEC_PHYSDEV - rc %lu", rc);
	    ++errors;
	    break;
	}

	sec->u.s.size = size = partsize - sec->u.s.adminspace;
	sec->timestamp = time(NULL);
	sec->crc = Crc16(sec, SECTOR_SIZE-2);	/* !!! */

	rc = write(0, 1, sec);
	if( rc != 0 )
	{
	    Verbose(1, "VFRDev::ioRemoveParent", "can't write own SEC_PHYSDEV - rc %lu", rc);
	    ++errors;
	    break;
	}

	parent = NULL;				/* none */
    }
    while(0);

    delete sec;
    return errors;
}




/*# ----------------------------------------------------------------------
 * VFRDev::ioChangeID(myid)
 *
 * PARAMETER
 *	myid		new DEVID
 * RETURNS
 *	count of errors
 *
 * DESCRIPTION
 *	Changes our own DEVID.
 *
 * REMARKS
 */
int
VFRDev::ioChangeID(DEVID newid)
{
    PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
    int		errors = 0;

    memcpy(id, newid, sizeof(DEVID));		/* update object */

    do
    {
	APIRET	rc;

	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
	if( rc != 0 )
	{
	    Verbose(1,"VFRDev::ioChangeID","can't read own SEC_PHYSDEV - rc %lu",rc);
	    ++errors;
	    break;
	}

	memcpy(sec->u.s.id, newid, sizeof(DEVID));
	sec->timestamp = time(NULL);		/* UTC of change */
	sec->crc = Crc16(sec, SECTOR_SIZE-2);	/* !!! */

	rc = write(0, 1, sec);
	if( rc != 0 )
	{
	    Verbose(1, "VFRDev::ioChangeID", "can't write own SEC_PHYSDEV - rc %lu", rc);
	    ++errors;
	    break;
	}
    }
    while(0);

    delete sec;
    return errors;
}




/*# ----------------------------------------------------------------------
 * VFRDev::ioDeleteYourself()
 *
 * PARAMETER
 *	(none, C++)
 * RETURNS
 *	count of errors
 *
 * DESCRIPTION
 *	Destroys itself by clearing configuration sector.
 *
 * REMARKS
 */
int
VFRDev::ioDeleteYourself()
{
    PUCHAR	buffer = new UCHAR[SECTOR_SIZE];
    APIRET	rc;
    int		errors = 0;

    memset(buffer, 0, SECTOR_SIZE);
    rc = write(0, 1, buffer);			/* clear PHYSDEV sector */
    if( rc != 0 )
    {
	Verbose(1, "VFRDev::ioDeleteYourself", "can't clear own SEC_PHYSDEV - rc %lu", rc);
	++errors;				/* ignore errors */
    }

    delete[] buffer;
    return errors;
}




/*# ----------------------------------------------------------------------
 * VFRDev::ioSync()
 *
 * PARAMETER
 *	(none, C++)
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	'this' has been changed -> update physical storage.
 *
 * REMARKS
 *	Uses global 'usHostId' to build a device ID.
 */
int
VFRDev::ioSync()
{
    PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
    int			modified = 0;
    APIRET		rc;
    int			errors = 0;


    do
    {
	rc = read(0, 1, sec);
	if( rc )
	{
	    Verbose(1, "VFRDev::ioSync", "read(0,1,...) - rc %lu", rc);
	    ++errors;
	    break;
	}

	if( memcmp(sec->sectype, "PHYSDEVICE      ", 16) != 0
	    ||  Crc16(sec, SECTOR_SIZE-2) != sec->crc )
	{
	    Verbose(1, "VFRDev", "new PHYSDEV sector");
	    ++modified;
	    memset(sec, 0, sizeof(*sec));

	    memcpy(sec->sectype, "PHYSDEVICE      ", 16);
	    sec->u.s.adminspace = VRAID_ADMINSECTORS;
	    size = sec->u.s.size = (partsize - sec->u.s.adminspace);

	    memcpy(sec->u.s.id, id, sizeof(DEVID));
	}

	if( size != sec->u.s.size )
	{
	    ++modified;
	    sec->u.s.size = size;
	}

	if( modified )
	{
	    sec->timestamp = time(NULL);
	    sec->crc = Crc16(sec, SECTOR_SIZE-2);

	    rc = write(0, 1, sec);
	    if( rc != 0 )
	    {
		Verbose(1, "VFRDev::ioSync", "write(0,1,...) - rc %lu, not updated", rc);
		++errors;
		break;
	    }
	}
    }
    while(0);

    return errors;
}




/*# ----------------------------------------------------------------------
 * VFRDev::querySize()
 *
 * PARAMETER
 *	(none, C++)
 * RETURNS
 *	size of device
 *
 * DESCRIPTION
 *	Guess!
 *
 * REMARKS
 */
ULONG
VFRDev::querySize()
{
    return (size == 0 ? partsize : size);
}




/*# ----------------------------------------------------------------------
 * VFRDev::read(block,count,buffer)
 *
 * PARAMETER
 *	block		block number on disk
 *	count		how many blocks?
 *	buffer		data from here
 * RETURNS
 *	0		OK
 *	/0		APIRET
 *
 * DESCRIPTION
 *	Read one or more sectors from configuration space of disk.  Done
 *	when analysing disk contents.
 *
 * REMARKS
 */
APIRET
VFRDev::read(ULONG block,ULONG count,PVOID buffer)
{
    APIRET	rc = 0;

    for( ; count != 0; --count, ++block, block += SECTOR_SIZE )
    {
	rc = DriverPhysIO(flt_hd, FALSE, block, buffer);
	if( rc != 0 )
	{
	    Verbose(1, "VFRDev", "can't read block %lu - rc %lu", block, rc);
	    break;
	}
    }
    return rc;
}




/*# ----------------------------------------------------------------------
 * VFRDev::write(block,count,buffer)
 *
 * PARAMETER
 *	block		block number on disk
 *	count		how many blocks?
 *	buffer		data from here
 * RETURNS
 *	0		OK
 *	/0		APIRET
 *
 * DESCRIPTION
 *	Write one or more sectors to configuration space of disk. Done
 *	when creating something new.
 *
 * REMARKS
 */
APIRET
VFRDev::write(ULONG block,ULONG count,PVOID buffer)
{
    APIRET	rc = 0;

    for( ; count != 0; --count, ++block, block += SECTOR_SIZE )
    {
	rc = DriverPhysIO(flt_hd, TRUE, block, buffer);
	if( rc != 0 )
	{
	    Verbose(1, "VFRDev", "can't write block %lu - rc %lu", block, rc);
	    break;
	}
    }
    return rc;
}






/* **********************************************************************
 * **** VOS2Disk:: ******************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * VOS2Disk::VOS2Disk(idx)
 *
 * PARAMETER
 *	idx		from DosPhysicalDisk()
 * RETURNS
 *	(nothing, C++)
 * DESCRIPTION
 *	Constructur for new object.  Uses PDsk*() routines to access
 *	the physical disk.
 *
 * REMARKS
 */
VOS2Disk::VOS2Disk(ULONG idx)
{
    ULONG	handle;
    APIRET	rc;

    /* Open physical device, this may fail! */

    rc = PDskOpen(idx, &handle);
    if( rc != 0 )
    {
	Verbose(1, "VOS2Disk", "PDskOpen(%lu) - rc %lu", pdskidx, rc);
	return;
    }

    rc = create(idx, handle);
    return;
}




/*# ----------------------------------------------------------------------
 * VOS2Disk::VOS2Disk(idx,handle)
 *
 * PARAMETER
 *	idx		from DosPhysicalDisk()
 *	handle		frpm PDskOpen()
 *
 * RETURNS
 *	(nothing, C++)
 *
 * DESCRIPTION
 *	Constructur for new object.  Uses PDsk*() routines to access
 *	the physical disk.
 *
 * REMARKS
 */
VOS2Disk::VOS2Disk(ULONG idx,ULONG handle)
{
    create(idx, handle);
}




/*# ----------------------------------------------------------------------
 * VOS2Disk::~VOS2Disk()
 *
 * PARAMETER
 *	(none, C++)
 * RETURNS
 *	(nothing, C++)
 * DESCRIPTION
 *	Destructor for our object.  Releases resources.
 *
 * REMARKS
 */
VOS2Disk::~VOS2Disk()
{
    APIRET	rc;

    if( writable == True )
    {
	rc = PDskUnlockIO(pdskhd);
	if( rc != 0 )
	    Verbose(0, "VOS2Disk", "PDskUnlockIO - rc %lu", rc);
    }
    rc = PDskClose(pdskhd);
}




/*# ----------------------------------------------------------------------
 * VOS2Disk::create(idx,hd)
 *
 * PARAMETER
 *	idx		from DosPhysicalDisk()
 *	handle		from PDskOpen()
 *
 * RETURNS
 * DESCRIPTION
 * REMARKS
 */
APIRET
VOS2Disk::create(ULONG idx,ULONG handle)
{
    APIRET	rc;

    /* Initialize object data. */

    pdskidx = idx;
    pdskhd = handle;

    memset(&devparam, 0, sizeof(devparam));
    writable = False;


    /* And please, don't disturb us.  If it isn't possible
     * to do a PDskLock() just ignore this error.  Sam Detweiler
     * told (ML.OS2PDD) FDISK won't lock at all... */
  
    rc = PDskLockIO(pdskhd);
    if( rc != 0 )
	Verbose(2, "VOS2Disk", "PDskLock(%lu) - rc %lu, ignored", pdskidx, rc);
    writable = True;				/* device is changable! */


    rc = PDskQueryParam(pdskhd,&devparam);
    if( rc != 0 )
    {
	Verbose(1, "VOS2Disk", "PDskQueryParam(%lu) - rc %lu", pdskidx, rc);
	PDskUnlockIO(pdskhd);
	PDskClose(pdskhd);
	return rc;
    }

    Verbose(3, "VOS2Disk", "Index: %lu, Cylinders: %d, Heads: %d, Sectors/Track: %d",
	    pdskidx,
	    devparam.cCylinders, devparam.cHeads, devparam.cSectorsPerTrack);
    return 0;
}








/*# ----------------------------------------------------------------------
 * VOS2Disk::queryDPB()
 *
 * PARAMETER
 *	(none, C++)
 *
 * RETURNS
 *	Reference to DEVICEPARAMETERBLOCK
 *
 * DESCRIPTION
 *	Usefull when creating new partitions, as a partition should start at
 *	head 0 or at least at sector 1.
 *
 * REMARKS
 */
DEVICEPARAMETERBLOCK &
VOS2Disk::queryDPB()
{
    return devparam;
}




/*# ----------------------------------------------------------------------
 * VOS2Disk::queryIndex()
 *
 * PARAMETER
 *	(none)
 * RETURNS
 *	0-based index
 *
 * DESCRIPTION
 *	Returns index used to access this disk.  Use only for display purposes!
 *
 * REMARKS
 */
ULONG
VOS2Disk::queryIndex()
{
    return pdskidx;
}




/*# ----------------------------------------------------------------------
 * VOS2Disk::querySize()
 *
 * PARAMETER
 *	(none, C++)
 * RETURNS
 *	size
 * DESCRIPTION
 *	Returns size of OS/2 disk.
 *
 * REMARKS
 */
ULONG
VOS2Disk::querySize()
{
    ULONG const	size = (ULONG)devparam.cSectorsPerTrack
	* devparam.cHeads * (ULONG)devparam.cCylinders;

    Verbose(4, "VOS2Disk", "disk capacity: %lu MBytes", size / 2 / 1024ul);
    return size;
}




/*# ----------------------------------------------------------------------
 * VOS2Disk::read(block,count,buffer)
 *
 * PARAMETER
 *	block		block number on disk
 *	count		how many blocks?
 *	buffer		data from here
 * RETURNS
 *	0		OK
 *	/0		APIRET
 *
 * DESCRIPTION
 *	Read one or more sectors from configuration space of disk.  Done
 *	when analysing disk contents.
 *
 * REMARKS
 */
APIRET
VOS2Disk::read(ULONG block,ULONG count,PVOID buffer)
{
    assert( count < USHRT_MAX );

    APIRET const rc = PDskRead(pdskhd, block, (USHORT)count, buffer);
    if( rc != 0 )
	Verbose(1, "VOS2Disk", "can't read sector %lu - rc %lu", block, rc);
    return rc;
}




/*# ----------------------------------------------------------------------
 * VOS2Disk::write(block,count,buffer)
 *
 * PARAMETER
 *	block		block number on disk
 *	count		how many blocks?
 *	buffer		data from here
 * RETURNS
 *	0		OK
 *	/0		APIRET
 *
 * DESCRIPTION
 *	Write one or more sectors to configuration space of disk. Done
 *	when creating a new VORDev.
 *
 * REMARKS
 */
APIRET
VOS2Disk::write(ULONG block,ULONG count,PVOID buffer)
{
    assert( count < USHRT_MAX );

    APIRET const rc = PDskWrite(pdskhd, block, (USHORT)count, buffer);
    if( rc != 0 )
	Verbose(1, "VOS2Disk", "can't write sector %lu - rc %lu", block, rc);
    return rc;
}

