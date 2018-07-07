/*
 * $Source: R:/Source/driver/raid/Setup/RCS/Single.cpp,v $
 * $Revision: 1.12 $
 * $Date: 2000/08/15 00:12:29 $
 * $Locker:  $
 *
 *	Implements VSingle class.
 *
 * $Log: Single.cpp,v $
 * Revision 1.12  2000/08/15 00:12:29  vitus
 * - changed addChild() prototype
 * - removed limits.h, not needed
 *
 * Revision 1.11  2000/04/10 00:50:18  vitus
 * - include VSetup.h instead of proto.h
 *
 * Revision 1.10  1999/12/14 00:27:02  vitus
 * - more error logging
 *
 * Revision 1.9  1999/07/05 00:41:10  vitus
 * - addChild() gets additional flag "config sector valid"
 *
 * Revision 1.8  1999/06/21 01:36:54  vitus
 * - corrected english spelling
 *
 * Revision 1.7  1999/06/19 18:27:33  vitus
 * - minor corrections to keep compiler happy
 *
 * Revision 1.6  1999/06/02 01:39:48  vitus
 * - queryIcon() returns ICO_INVCHILDPDEV if parent says that this
 *   child is invalid
 *
 * Revision 1.5  1999/05/13 22:43:15  vitus
 * - ioRemoveParent: clear parent's admin sector
 * - ioDeleteYourself: no disk i/o
 *
 * Revision 1.4  1999/04/13 00:59:28  vitus
 * - ioDeleteYourself: first clear own sector and afterwards call childs
 *
 * Revision 1.3  1999/03/24 23:03:13  vitus
 * - allocateDescription() adds drive ID to description
 *
 * Revision 1.2  1999/03/06 20:10:32  vitus
 * - implemented queryIcon() method
 *
 * Revision 1.1  1999/03/01 01:11:16  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1999-2000
 */
static char const vcid[]="$Id: Single.cpp,v 1.12 2000/08/15 00:12:29 vitus Exp $";

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
#include "Single.hpp"






/*# ----------------------------------------------------------------------
 * VSingle::VSingle(drive_id,nchd)
 *
 * PARAMETER
 *	drive_id	ID of this single
 *
 * RETURNS
 *	(nothing, C++)
 *
 * DESCRIPTION
 *	Creates a new VRAID object in memory.
 *
 * REMARKS
 */
VSingle::VSingle(DEVID drive_id)
{
    parent = NULL;
    child = 0;
    size = 0;
    hostdrive = False;
    writable = True;
    memcpy(id, drive_id, sizeof(DEVID));

    inv_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_INVCHILDARRAY);
    rw_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_RAIDDRIVE);
    ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDARRAY);
}




/*# ----------------------------------------------------------------------
 * VSingle::~VSingle()
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
VSingle::~VSingle()
{
    delete[] child;
    WinDestroyPointer(rw_icon);
    WinDestroyPointer(ro_icon);
}




/*# ----------------------------------------------------------------------
 * VSingle::queryChildren()
 *
 * PARAMETER
 *	(none)
 * RETURNS
 *	count
 *
 * DESCRIPTION
 *	Returns count of children.
 *
 * REMARKS
 */
int
VSingle::queryChildren()
{
    return 1;
}




/*# ----------------------------------------------------------------------
 * VSingle::getChild(index)
 *
 * PARAMETER
 *	index		which child?  (should be 0)
 * RETURNS
 *	pointer
 *
 * DESCRIPTION
 *	Returns pointer to a child.
 *
 * REMARKS
 */
VRDev *
VSingle::getChild(int index)
{
    if( index >= 1 )
	return 0;
    return child;
}




/*# ----------------------------------------------------------------------
 * VSingle::allocateDescription()
 *
 * PARAMETER
 *	(none)
 * RETURNS
 *	ASCIIZ
 *
 * DESCRIPTION
 *	Allocates buffer and fills it with description of
 *	this object.  Caller has to free.
 *
 * REMARKS
 */
char *
VSingle::allocateDescription()
{
    char * descr = new char[80];
    if( hostdrive == True  &&  os2access != -1UL )
	sprintf(descr, "Single (OS/2 Drive %lu)\n"
		"ID %02X%02X%02X%02X%02X%02X    %lu MByte",
		os2access,
		id[0],id[1],id[2],id[3],id[4],id[5],
		size / 2 / 1024);
    else
	sprintf(descr, "Single\n"
		"ID %02X%02X%02X%02X%02X%02X    %lu MByte",
		id[0],id[1],id[2],id[3],id[4],id[5],
		size / 2 / 1024);
    return descr;
}




/*# ----------------------------------------------------------------------
 * VSingle::queryIcon()
 *
 * PARAMETER
 *	(none)
 * RETURNS
 *	HPOINTER to display
 * DESCRIPTION
 *		Returns icon to reflect the logical state of this drive.
 *
 * REMARKS
 */
HPOINTER
VSingle::queryIcon()
{
    if( parent != 0  &&  parent->isValidChild(this) == False )
	return inv_icon;
    else if( writable == True )
	return rw_icon;
    else
	return ro_icon;
}




/*# ----------------------------------------------------------------------
 * VSingle::setHostdrive(os2idx)
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
VSingle::setHostdrive(ULONG os2idx)
{
    hostdrive = True;
    os2access = os2idx;
}




/*# ----------------------------------------------------------------------
 * VSingle::addChild(newchild, cfgv, datav)
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
 *	OBS: as this is a SINGLE there is only a single child!  And
 *	it's always valid.
 *
 * REMARKS
 */
void
VSingle::addChild(VRDev * newchild, Boolean cfgv, ULONG datav)
{
    assert( newchild != NULL );
    assert( child == 0 );			/* how do I do exceptions? */
    assert( datav == -1ul );

    /* Set double link between child and parent. */

    child = newchild;
    newchild->setParent(this);

    /* Update own object with child's information. */

    size += newchild->querySize();
    if( newchild->isWritable() == False )
	writable = False;			/* oups, it isn't 'changable' */
}




/*# ----------------------------------------------------------------------
 * ioDeleteYourself()
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
VSingle::ioDeleteYourself()
{
    int		errors = 0;


    /* Update all children (parent removed -> marked in sector) */

    if( child != NULL )
    {
	errors += child->ioRemoveParent();
	child = NULL;
    }

    return errors;
}




/*# ----------------------------------------------------------------------
 * VSingle::ioRemoveParent()
 *
 * PARAMETER
 *	(none, C++)
 * RETURNS
 *	count of errors
 *
 * DESCRIPTION
 *	Removes any traces of parent from configuration sectors.  Means
 *	this array becomes hostdrive!
 *
 * REMARKS
 */
int
VSingle::ioRemoveParent()
{
    PSEC_VRDEV	sec = new SEC_VRDEV;
    int		errors = 0;
    APIRET	rc;


    /* 1st: clear administrative sector of parent. */

    memset(sec, 0, SECTOR_SIZE);
    rc = write(1, 1, sec);
    if( rc != 0 )
    {
	Verbose(1, "VSingle::ioRemoveParent", "can't clear parent SEC_VRDEV - rc %lu", rc);
	++errors;
    }


    /* 2nd: update RDFLAG_HOSTDRIVE */

    do
    {
	rc = read(0, 1, sec);			// remember: 0 defines ourself
	if( rc != 0 )
	{
	    Verbose(1, "VSingle::ioRemoveParent", "can't read own SEC_VRDEV - rc %lu", rc);
	    ++errors;
	    break;
	}

	sec->u.s.flags |= RDFLAG_HOSTDRIVE;	// no parent means hostdrive
	sec->timestamp = time(NULL);		// UTC of change
	sec->crc = Crc16(sec, SECTOR_SIZE-2);	// !!!

	rc = write(0, 1, sec);
	if( rc != 0 )
	{
	    Verbose(1, "VSingle::ioRemoveParent", "can't write own SEC_VRDEV - rc %lu", rc);
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
 * VSingle::ioChangeID(newid)
 *
 * PARAMETER
 *	newid		new ID
 * RETURNS
 *	count of errors
 *
 * DESCRIPTION
 *	Changes our own DEVID.
 *
 * REMARKS
 */
int
VSingle::ioChangeID(DEVID newid)
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
	    Verbose(1,"VSingle::ioChangeID","can't read own SEC_VRDEV - rc %lu", rc);
	    ++errors;
	    break;
	}

	memcpy(sec->u.s.id, newid, sizeof(DEVID));
	sec->timestamp = time(NULL);		// UTC of change
	sec->crc = Crc16(sec, SECTOR_SIZE-2);	// !!!

	rc = write(0, 1, sec);
	if( rc != 0 )
	{
	    Verbose(1, "VSingle::ioChangeID", "can't write own SEC_VRDEV - rc %lu", rc);
	    ++errors;
	    break;
	}
    }
    while(0);

    delete sec;
    return errors;
}




/*# ----------------------------------------------------------------------
 * VSingle::ioSync()
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
VSingle::ioSync()
{
    PSEC_VRDEV	sec = new SEC_VRDEV;
    int		errors = 0;


    /* First: fill configuration sector of current level. */

    memset(sec, 0, sizeof(*sec));
    memcpy(sec->sectype, "VRAIDDEVICE     ", 16);
    sec->timestamp = time(NULL);

    memcpy(sec->u.s.id, id, sizeof(DEVID));
    sec->u.s.type = RDTYPE_SINGLE;
    sec->u.s.flags = (UCHAR)(parent != 0 ? 0 : 0x80);

    sec->u.s.children = 1;


    /* 2nd: recalculate drive size, correct size of children.
     * The current values were only wild guesses. */

    sec->u.s.child[0].size = size = child->querySize();


    /* 3rd: update all children and record their IDs. */

    child->ioSync();
    memcpy(sec->u.s.child[0].id, child->queryID(), sizeof(DEVID));


    /* Last: write administrative sector of current level. */

    sec->crc = Crc16(sec, SECTOR_SIZE-2);
    APIRET rc = write(0, 1, sec);
    if( rc != 0 )
    {
	Verbose(1, "VSingle::ioSync", "write(0,1,...) - rc %lu, not updated", rc);
	++errors;
    }

    delete sec;
    return errors;
}




/*# ----------------------------------------------------------------------
 * VSingle::querySize()
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
VSingle::querySize()
{
    return size;
}




/*# ----------------------------------------------------------------------
 * VSingle::read(offset,count,buffer)
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
 *	Reads data from configuration sectors of this VRAID drive.  No
 *	redundancy as this is a single.
 *
 * REMARKS
 */
APIRET
VSingle::read(ULONG offset,ULONG count,PVOID buffer)
{
    return child->read(offset+1, count, buffer);
}




/*# ----------------------------------------------------------------------
 * VSingle::write(offset,count,buffer)
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
VSingle::write(ULONG offset,ULONG count,PVOID buffer)
{
    return child->write(offset+1, count, buffer);
}
