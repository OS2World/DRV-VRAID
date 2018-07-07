/*
 * $Source: R:/Source/driver/raid/Setup/RCS/Stripe.cpp,v $
 * $Revision: 1.15 $
 * $Date: 2001/02/11 01:03:43 $
 * $Locker:  $
 *
 *	Implements VStripe class.
 *
 * $Log: Stripe.cpp,v $
 * Revision 1.15  2001/02/11 01:03:43  vitus
 * - addChild: removed datav assert, when a child is missing
 *   then there is no valid data
 *
 * Revision 1.14  2000/08/15 00:14:59  vitus
 * - changed addChild() prototype
 *
 * Revision 1.13  2000/04/10 00:51:51  vitus
 * - include VSetup.h instead of proto.h
 *
 * Revision 1.12  1999/12/14 00:27:22  vitus
 * - more error logging
 *
 * Revision 1.11  1999/07/05 00:40:45  vitus
 * - addChild() gets additional flag "config sector valid"
 * - modifications to keep this flag in child table
 *
 * Revision 1.10  1999/06/21 01:35:36  vitus
 * - corrected english spelling
 *
 * Revision 1.9  1999/06/19 18:25:46  vitus
 * - minor corrections to keep compiler happy
 *
 * Revision 1.8  1999/06/02 01:39:25  vitus
 * - queryIcon() returns ICO_INVCHILDPDEV if parent says that this
 *   child is invalid
 *
 * Revision 1.7  1999/05/13 22:42:57  vitus
 * - ioRemoveParent: clear parent's admin sector
 * - ioDeleteYourself: no disk i/o
 *
 * Revision 1.6  1999/04/13 01:24:48  vitus
 * - child now static member of object
 *
 * Revision 1.5  1999/03/24 23:02:57  vitus
 * - allocateDescription() adds drive ID to description
 * - addChild() array size calculation corrected
 *
 * Revision 1.4  1999/03/23 00:50:29  vitus
 * - read method reads all childs and compares.  Currently differences are
 *   only logged.  Better ideas?
 *
 * Revision 1.3  1999/03/22 01:57:50  vitus
 * - ioDeleteYourself: first clear own sector and afterwards call childs
 *
 * Revision 1.2  1999/03/06 19:56:18  vitus
 * - implemented queryIcon() method
 *
 * Revision 1.1  1999/03/01 01:17:40  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1999-2000
 */
static char const vcid[]="$Id: Stripe.cpp,v 1.15 2001/02/11 01:03:43 vitus Exp $";

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

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
#include "resources.h"				// icon resources
#include "drvif.h"

#include "dskinfo.hpp"
#include "Stripe.hpp"





/*# ----------------------------------------------------------------------
 * VStripe::VStripe(drive_id,nchd)
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
VStripe::VStripe(DEVID drive_id,int nchd)
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
 * VStripe::~VStripe()
 *
 * PARAMETER
 *	(none, C++)
 * RETURNS
 *	(nothing, C++)
 *
 * DESCRIPTION
 *	Destroys VRAID memory object.
 *
 * REMARKS
 */
VStripe::~VStripe()
{
    WinDestroyPointer(rw_icon);
    WinDestroyPointer(ro_icon);
}




/*# ----------------------------------------------------------------------
 * VStripe::queryChildren()
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
VStripe::queryChildren()
{
    return children;
}




/*# ----------------------------------------------------------------------
 * VStripe::getChild(index)
 *
 * PARAMETER
 *	index		which child
 * RETURNS
 *	pointer
 *
 * DESCRIPTION
 *	Returns pointer to a child.
 *
 * REMARKS
 */
VRDev *
VStripe::getChild(int index)
{
    if( index >= children )
	return 0;
    return child[index].rdev;
}




/*# ----------------------------------------------------------------------
 * VStripe::allocateDescription()
 *
 * PARAMETER
 *	(none)
 * RETURNS
 *	ASCIIZ
 *
 * DESCRIPTION
 *	Allocates buffer and fills it with description of this object.
 *
 * REMARKS
 */
char *
VStripe::allocateDescription()
{
    char * descr = new char[80];
    if( hostdrive == True  &&  os2access != - 1UL )
	sprintf(descr, "RAID 0 (Striping) (OS/2 Drive %lu)\n"
		"ID %02X%02X%02X%02X%02X%02X    %lu MByte",
		os2access,
		id[0],id[1],id[2],id[3],id[4],id[5],
		size / 2 / 1024);
    else
	sprintf(descr, "RAID 0 (Striping)\n"
		"ID %02X%02X%02X%02X%02X%02X    %lu MByte",
		id[0],id[1],id[2],id[3],id[4],id[5],
		size / 2 / 1024);
    return descr;
}




/*# ----------------------------------------------------------------------
 * VStripe::queryIcon()
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
VStripe::queryIcon()
{
    if( parent != 0  &&  parent->isValidChild(this) == False )
	return inv_icon;
    else if( writable == True )
	return rw_icon;
    else
	return ro_icon;
}




/*# ----------------------------------------------------------------------
 * VStripe::setHostdrive(os2idx)
 *
 * PARAMETER
 *	os2idx		accessable through this PDsk*() index
 *			or -1 if it isn't accessable.
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Marks this array as a host drive with or w/o
 *	OS/2 access.
 *
 * REMARKS
 */
void
VStripe::setHostdrive(ULONG os2idx)
{
    hostdrive = True;
    os2access = os2idx;
}




/*# ----------------------------------------------------------------------
 * VStripe::addChild(newchild, cfgv, datav)
 *
 * PARAMETER
 *	newchild	pointer to child to add
 *	cfgv		configuration already written?
 *	data		whether data of this child is valid (ignored)
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
VStripe::addChild(VRDev * newchild, Boolean cfgv, ULONG datav)
{
    assert( newchild != NULL );

    /* Set double link between child and parent. */

    child[children].rdev = newchild;
    child[children].cfgok = cfgv;
    newchild->setParent(this);

    /* Update our object with child's information. */

    ULONG	childsize = newchild->querySize();
    for( int i = 0; i < children; ++i )
    {
	ULONG	ul;

	if( (ul = child[i].rdev->querySize()) < childsize )
	    childsize = ul;
    }

    size = (children + 1) * childsize;
    if( newchild->isWritable() == False )
	writable = False;			/* oups, it isn't 'changable' */

    ++children;
}




/*# ----------------------------------------------------------------------
 * VStripe::ioDeleteYourself()
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
VStripe::ioDeleteYourself()
{
    int		i;
    int		errors = 0;


    /* Update all children (parent removed -> marked in sector) */

    for( i = 0; i < children; ++i )
    {
	if( child[i].rdev != NULL )
	{
	    errors += child[i].rdev->ioRemoveParent();
	    child[i].rdev = NULL;		// forget this child
	}
    }

    return errors;
}




/*# ----------------------------------------------------------------------
 * VStripe::ioRemoveParent()
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
VStripe::ioRemoveParent()
{
    PSEC_VRDEV	sec = new SEC_VRDEV;
    int		errors = 0;
    APIRET	rc;


    /* 1st: clear administrative sector of parent. */

    memset(sec, 0, SECTOR_SIZE);
    rc = write(1, 1, sec);
    if( rc != 0 )
    {
	Verbose(1, "VStripe::ioRemoveParent", "can't clear parent SEC_VRDEV - rc %lu", rc);
	++errors;
    }

    /* 2nd: set RDFLAG_HOSTDRIVE */

    do
    {
	rc = read(0, 1, sec);			// remember: 0 defines ourself
	if( rc != 0 )
	{
	    Verbose(1, "VStripe::ioRemoveParent", "can't read own SEC_VRDEV - rc %lu", rc);
	    ++errors;
	    break;
	}

	sec->u.s.flags |= RDFLAG_HOSTDRIVE;	// no parent means hostdrive
	sec->timestamp = time(NULL);		// UTC of change
	sec->crc = Crc16(sec, SECTOR_SIZE-2);	// !!!

	rc = write(0, 1, sec);
	if( rc != 0 )
	{
	    Verbose(1, "VStripe::ioRemoveParent", "can't write own SEC_VRDEV - rc %lu", rc);
	    ++errors;
	    break;
	}

	parent = NULL;				// none
    }
    while(0);

    delete sec;
    return errors;
}




/*# ----------------------------------------------------------------------
 * VStripe::ioChangeID(newid)
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
VStripe::ioChangeID(DEVID newid)
{
    PSEC_VRDEV	sec = new SEC_VRDEV;
    int		errors = 0;

    memcpy(id, newid, sizeof(DEVID));		// update object

    do
    {
	APIRET	rc;

	rc = read(0, 1, sec);			// remember: 0 defines ourself
	if( rc != 0 )
	{
	    Verbose(1,"VStripe::ioChangeID","can't read own SEC_VRDEV - rc %lu", rc);
	    ++errors;
	    break;
	}

	memcpy(sec->u.s.id, newid, sizeof(DEVID));
	sec->timestamp = time(NULL);		// UTC of change
	sec->crc = Crc16(sec, SECTOR_SIZE-2);	// !!!

	rc = write(0, 1, sec);
	if( rc != 0 )
	{
	    Verbose(1, "VStripe::ioChangeID", "can't write own SEC_VRDEV - rc %lu", rc);
	    ++errors;
	    break;
	}
    }
    while(0);

    delete sec;
    return errors;
}




/*# ----------------------------------------------------------------------
 * VStripe::ioSync()
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
VStripe::ioSync()
{
    PSEC_VRDEV	sec = new SEC_VRDEV;
    USHORT	i;
    ULONG	ul;
    int		errors = 0;


    /* First: fill configuration sector of current level. */

    memset(sec, 0, sizeof(*sec));
    memcpy(sec->sectype, "VRAIDDEVICE     ", 16);
    sec->timestamp = time(NULL);

    memcpy(sec->u.s.id, id, sizeof(DEVID));
    sec->u.s.type = RDTYPE_STRIPE;
    sec->u.s.flags = (UCHAR)(parent != 0 ? 0 : 0x80);

    sec->u.s.children = children;


    /* 2nd: recalculate drive size, correct size of children.
     * The current values were only wild guesses. */

    size = 0;
    ul = (ULONG)ULONG_MAX;
    for( i = 0; i < children; ++i )
    {
	ul = min(ul, child[i].rdev->querySize());
    }
    for( i = 0; i < children; ++i )
    {
	sec->u.s.child[i].size = ul;
	size += ul;
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
	Verbose(1, "VStripe::ioSync", "write(0,1,...) - rc %lu, not updated", rc);
	++errors;
    }

    delete sec;
    return errors;
}




/*# ----------------------------------------------------------------------
 * VStripe::querySize()
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
VStripe::querySize()
{
    return size;
}




/*# ----------------------------------------------------------------------
 * VStripe::read(offset,count,buffer)
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
 *	Reads data from configuration sectors of this VRAID drive.
 *
 * REMARKS
 *	Should read all children and compare. xxx
 *	Should test 'valid'. xxx
 */
APIRET
VStripe::read(ULONG offset,ULONG count,PVOID buffer)
{
    PUCHAR	copybuf = new UCHAR[count];
    int		goodchildren = 0;
    APIRET	rc = VERROR_NO_CONTENTS;

    for( int i = 0; i < children; ++i )
    {
	if( !child[i].cfgok )
	    continue;				// nothing on this child

	rc = child[i].rdev->read(offset+1, count, buffer);
	if( rc != 0 )
	    continue;				// Verbose() already called
	if( goodchildren == 0 )
	    memcpy(copybuf, buffer, (size_t)count);
	else if( memcmp(copybuf, buffer, (size_t)count) != 0 )
	    Verbose(0, "VStripe", "Data error when reading child %d, ignored", i);
	++goodchildren;
    }

    delete[] copybuf;
    return (goodchildren == 0 ? rc : 0);
}




/*# ----------------------------------------------------------------------
 * VStripe::write(offset,count,buffer)
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
 *	Should test 'valid'. xxx
 */
APIRET
VStripe::write(ULONG offset,ULONG count,PVOID buffer)
{
    APIRET	rc = 0;

    for( int i = 0; i < children; ++i )
	rc |= child[i].rdev->write(offset+1, count, buffer);
    return rc;
}
