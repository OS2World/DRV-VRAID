/*
 * $Source: R:/Source/driver/raid/Setup/RCS/Chain.cpp,v $
 * $Revision: 1.12 $
 * $Date: 2000/08/17 02:14:41 $
 * $Locker:  $
 *
 *	Implements VChain class.
 *
 * $Log: Chain.cpp,v $
 * Revision 1.12  2000/08/17 02:14:41  vitus
 * - method addChild() parameter 'datav' changed to sector no.  Ignored.
 *
 * Revision 1.11  2000/04/10 00:56:56  vitus
 * - include VSetup.h instead of proto.h
 * - converted C++ to C comments
 *
 * Revision 1.10  1999/12/14 00:26:13  vitus
 * - more error logging
 *
 * Revision 1.9  1999/07/05 00:45:51  vitus
 * - addChild() gets additional flag "config sector valid"
 * - modifications to keep this flag in child table and honour it when reading
 *   from admin space
 *
 * Revision 1.8  1999/06/21 01:37:41  vitus
 * - corrected english spelling
 *
 * Revision 1.7  1999/06/19 18:26:00  vitus
 * - minor corrections to keep compiler happy
 *
 * Revision 1.6  1999/06/02 01:40:17  vitus
 * - queryIcon() returns ICO_INVCHILDARRAY if parent says that this
 *   child is invalid
 *
 * Revision 1.5  1999/05/13 22:41:50  vitus
 * - ioRemoveParent: clear parent's admin sector
 * - ioDeleteYourself: no disk i/o
 *
 * Revision 1.4  1999/04/13 01:25:16  vitus
 * - child[] now static member of object
 *
 * Revision 1.3  1999/03/24 23:02:06  vitus
 * - allocateDescription() adds drive ID to description
 *
 * Revision 1.2  1999/03/22 02:07:13  vitus
 * - implemented queryIcon() method
 * - ioDeleteYourself: first clear own sector and afterwards call childs
 * - read method reads all childs and compares.  Currently differences are
 *   only logged.  Better ideas?
 *
 * Revision 1.1  1999/03/01 01:07:06  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1999-2000
 */
static char const vcid[]="$Id: Chain.cpp,v 1.12 2000/08/17 02:14:41 vitus Exp $";

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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
}
#include "VSetup.h"
#include "resources.h"
#include "drvif.h"

#include "dskinfo.hpp"
#include "Chain.hpp"





/*# ----------------------------------------------------------------------
 * VChain::VChain(drive_id,nchd)
 *
 * PARAMETER
 *	drive_id	ID of this chain
 *	nchd		child count
 *
 * RETURNS
 *	(nothing, C++)
 *
 * DESCRIPTION
 *	Creates a new VRAID object in memory.
 *
 * REMARKS
 */
VChain::VChain(DEVID drive_id,int nchd)
{
    parent = NULL;
    children = 0;
    size = 0;
    hostdrive = False;
    writable = True;
    memcpy(id, drive_id, sizeof(DEVID));

    inv_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_INVCHILDARRAY);
    rw_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_RAIDDRIVE);
    ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDARRAY);
}




/*# ----------------------------------------------------------------------
 * VChain::~VChain()
 *
 * PARAMETER
 *	(none, C++)
 * RETURNS
 *	(nothing, C++)
 * DESCRIPTION
 *	Destroys VRAID memory object.
 *
 * REMARKS
 */
VChain::~VChain()
{
    WinDestroyPointer(rw_icon);
    WinDestroyPointer(ro_icon);
}




/*# ----------------------------------------------------------------------
 * VChain::queryChildren()
 *
 * PARAMETER
 *	(none)
 * RETURNS
 *	count
 * DESCRIPTION
 *	Returns count of children.
 *
 * REMARKS
 */
int
VChain::queryChildren()
{
    return children;
}




/*# ----------------------------------------------------------------------
 * VChain::getChild(index)
 *
 * PARAMETER
 *	index		which child?
 * RETURNS
 *	pointer to child object
 * DESCRIPTION
 *	Returns pointer to a child, used for display purposes.
 *
 * REMARKS
 */
VRDev *
VChain::getChild(int index)
{
    if( index >= children )
	return 0;
    return child[index].rdev;
}




/*# ----------------------------------------------------------------------
 * VChain::allocateDescription()
 *
 * PARAMETER
 *	(none)
 * RETURNS
 *	pointer to description.
 *
 * DESCRIPTION
 *	Allocates and fill buffer containing description of our object.
 *
 * REMARKS
 */
char *
VChain::allocateDescription()
{
    char * descr = new char[80];
    if( hostdrive )
	sprintf(descr, "Chaining (OS/2 Drive %lu)\n"
		"ID %02X%02X%02X%02X%02X%02X    %lu MByte",
		os2access,
		id[0],id[1],id[2],id[3],id[4],id[5],
		size / 2 / 1024);
    else
	sprintf(descr, "Chaining\n"
		"ID %02X%02X%02X%02X%02X%02X    %lu MByte",
		id[0],id[1],id[2],id[3],id[4],id[5],
		size / 2 / 1024);
    return descr;
}




/*# ----------------------------------------------------------------------
 * VChain::queryIcon()
 *
 * PARAMETER
 *	(none)
 * RETURNS
 *	HPOINTER to display
 * DESCRIPTION
 *	Returns icon to reflect the logical state of this drive.
 *
 * REMARKS
 */
HPOINTER
VChain::queryIcon()
{
    if( parent != 0  &&  parent->isValidChild(this) == False )
	return inv_icon;
    else if( writable == True )
	return rw_icon;
    else
	return ro_icon;
}




/*# ----------------------------------------------------------------------
 * VChain::setHostdrive(os2idx)
 *
 * PARAMETER
 *	os2idx		accessable through this PDsk*() index
 *			or -1 if it isn't accessable.
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Marks this VRAID drives as a host drive with or w/o
 *	OS/2 access.
 *
 * REMARKS
 */
void
VChain::setHostdrive(ULONG os2idx)
{
    hostdrive = True;
    os2access = os2idx;
}




/*# ----------------------------------------------------------------------
 * VChain::addChild(newchild,cfgv,datav)
 *
 * PARAMETER
 *	newchild	pointer to child to add
 *	cfgv		configuration already written?
 *	datav		whether data of this child is valid (uptodate)
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Adds a new child to our VRAID object.
 *
 * REMARKS
 */
void
VChain::addChild(VRDev * newchild, Boolean cfgv, ULONG datav)
{
    assert( newchild != NULL );
    assert( datav == -1ul );

    /* Set double link between child and parent. */

    child[children].rdev = newchild;
    child[children].cfgok = cfgv;
    newchild->setParent(this);

    /* Update our object with information from child. */

    size += newchild->querySize();
    if( newchild->isWritable() == False )
	writable = False;			/* oups, it isn't 'changable' */

    ++children;
}




/*# ----------------------------------------------------------------------
 * VChain::ioDeleteYourself()
 *
 * PARAMETER
 *	this		to be removed
 *
 * RETURNS
 *	count of errors
 *
 * DESCRIPTION
 *	Remove any traces of ourself from physical storage.
 *	This is done by telling all children that we are about
 *	to be killed (they might set the "HOSTDRIVE" in their
 *	configuration sectors) and clearing our own sector.
 *
 * REMARKS
 */
int
VChain::ioDeleteYourself()
{
    int		i;
    int		errors = 0;


    /* Update all children (parent removed -> marked in sectors) */

    for( i = 0; i < children; ++i )
    {
	if( child[i].rdev != NULL )
	{
	    errors += child[i].rdev->ioRemoveParent();
	    child[i].rdev = NULL;		/* we don't need pointer anymore */
	}
    }

    return errors;
}




/*# ----------------------------------------------------------------------
 * VChain::ioRemoveParent()
 *
 * PARAMETER
 *	(none, C++)
 * RETURNS
 *	0		OK
 *
 * DESCRIPTION
 *	Removes any traces of parent from configuration sectors.  Means
 *	this array becomes hostdrive!
 *
 * REMARKS
 */
int
VChain::ioRemoveParent()
{
    PSEC_VRDEV	sec = new SEC_VRDEV;
    int		errors = 0;
    APIRET	rc;


    /* 1st: clear administrative sector of parent. */

    memset(sec, 0, SECTOR_SIZE);
    rc = write(1, 1, sec);
    if( rc != 0 )
    {
	Verbose(1, "VChain::ioRemoveParent", "can't clear parent SEC_VRDEV - rc %lu", rc);
	++errors;
    }


    /* 2nd: set RDFLAG_HOSTDRIVE in own configuration. */

    do
    {
	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
	if( rc != 0 )
	{
	    Verbose(1, "VChain::ioRemoveParent", "can't read own SEC_VRDEV - rc %lu", rc);
	    ++errors;
	    break;
	}

	sec->u.s.flags |= RDFLAG_HOSTDRIVE;	/* no parent means hostdrive */
	sec->timestamp = time(NULL);		/* UTC of change */
	sec->crc = Crc16(sec, SECTOR_SIZE-2);	/* update CRC!!! */

	rc = write(0, 1, sec);
	if( rc != 0 )
	{
	    Verbose(1, "VChain::ioRemoveParent", "can't write own SEC_VRDEV - rc %lu", rc);
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
 * VChain::ioChangeID(newid)
 *
 * PARAMETER
 *	newid		new ID
 * RETURNS
 *	count of errors
 *
 * DESCRIPTION
 *	Changes it's own DEVID.
 *
 * REMARKS
 */
int
VChain::ioChangeID(DEVID newid)
{
    PSEC_VRDEV	sec = new SEC_VRDEV;
    int		errors = 0;

    memcpy(id, newid, sizeof(DEVID));		/* update object */

    do
    {
	APIRET	rc;

	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
	if( rc != 0 )
	{
	    Verbose(1,"VChain::ioChangeID", "can't read own SEC_VRDEV - rc %lu", rc);
	    ++errors;
	    break;
	}

	memcpy(sec->u.s.id, newid, sizeof(DEVID));
	sec->timestamp = time(NULL);		/* UTC of change */
	sec->crc = Crc16(sec, SECTOR_SIZE-2);	/* update CRC!!! */

	rc = write(0, 1, sec);
	if( rc != 0 )
	{
	    Verbose(1,"VChain::ioChangeID","can't write own SEC_VRDEV - rc %lu", rc);
	    ++errors;
	    break;
	}
    }
    while(0);

    delete sec;
    return errors;
}




/*# ----------------------------------------------------------------------
 * VChain::ioSync()
 *
 * PARAMETER
 *	(none, C++)
 * RETURNS
 *	count of errors
 *
 * DESCRIPTION
 *	'this' has been changed by the user -> update physical storage.
 *
 * REMARKS
 */
int
VChain::ioSync()
{
    PSEC_VRDEV	sec = new SEC_VRDEV;
    USHORT	i;
    int		errors = 0;


    /* First: fill configuration sector of current level. */

    memset(sec, 0, sizeof(*sec));
    memcpy(sec->sectype, "VRAIDDEVICE     ", 16);
    sec->timestamp = time(NULL);

    memcpy(sec->u.s.id, id, sizeof(DEVID));
    sec->u.s.type = RDTYPE_CHAIN;
    sec->u.s.flags = (UCHAR)(parent != 0 ? 0 : 0x80);

    sec->u.s.children = children;


    /* 2nd: recalculate drive size, correct size of children.
     * The current values were only wild guesses. */

    size = 0;
    for( i = 0; i < children; ++i )
    {
	sec->u.s.child[i].size = child[i].rdev->querySize();
	size += sec->u.s.child[i].size;
    }


    /* 3rd: update all children and record their IDs. */

    for( i = 0; i < children; ++i )
    {
	child[i].rdev->ioSync();
	memcpy(sec->u.s.child[i].id, child[i].rdev->queryID(), sizeof(DEVID));
    }


    /* Last: write administrative sector of current level. */

    sec->crc = Crc16(sec, SECTOR_SIZE-2);
    APIRET rc = write(0, 1, sec);
    if( rc != 0 )
    {
	Verbose(1, "VChain::ioSync", "write(0,1,...) - rc %lu, not updated", rc);
	++errors;
    }

    delete sec;
    return errors;
}




/*# ----------------------------------------------------------------------
 * VChain::querySize()
 *
 * PARAMETER
 *	(none, C++)
 * RETURNS
 *	size of array
 *
 * DESCRIPTION
 *	Guess!
 *
 * REMARKS
 */
ULONG
VChain::querySize()
{
    return size;
}




/*# ----------------------------------------------------------------------
 * VChain::read(offset,count,buffer)
 *
 * PARAMETER
 *	offset		offset in configuration space
 *	count		sector count
 *	buffer		sector data
 *
 * RETURNS
 *	0		OK
 *	/0		APIRET
 *
 * DESCRIPTION
 *	Reads data from configuration sectors of this VRAID drive.
 *
 * REMARKS
 */
APIRET
VChain::read(ULONG offset,ULONG count,PVOID buffer)
{
    PUCHAR	copybuf = new UCHAR[count];
    int		goodchildren = 0;
    APIRET	rc = VERROR_NO_CONTENTS;

    for( int i = 0; i < children; ++i )
    {
	if( !child[i].cfgok )
	    continue;				/* nothing on this child */

	rc = child[i].rdev->read(offset+1, count, buffer);
	if( rc != 0 )
	    continue;				/* Verbose() already called */
	if( goodchildren == 0 )
	    memcpy(copybuf, buffer, (size_t)count);
	else if( memcmp(copybuf, buffer, (size_t)count) != 0 )
	    Verbose(0, "VChain", "Data error when reading child %d, ignored", i);
	++goodchildren;
    }

    delete[] copybuf;
    return (goodchildren == 0 ? rc : 0);
}




/*# ----------------------------------------------------------------------
 * VChain::write(offset,count,buffer)
 *
 * PARAMETER
 *	offset		offset in configuratoin space
 *	count		sector count
 *	buffer		sector data
 *
 * RETURNS
 *	0		OK
 *	/0		APIRET
 *
 * DESCRIPTION
 *	Writes data to configuration sectors of this VRAID drive.
 *
 * REMARKS
 */
APIRET
VChain::write(ULONG offset,ULONG count,PVOID buffer)
{
    APIRET	rc = 0;

    for( int i = 0; i < children; ++i )
	rc |= child[i].rdev->write(offset+1, count, buffer);
    return rc;
}
