/*
 * $Source: R:/Source/driver/raid/Setup/RCS/Missing.cpp,v $
 * $Revision: 1.4 $
 * $Date: 2000/04/10 00:48:36 $
 * $Locker:  $
 *
 *	Implements VMissing class.
 *
 * $Log: Missing.cpp,v $
 * Revision 1.4  2000/04/10 00:48:36  vitus
 * - include VSetup.h instead of proto.h
 *
 * Revision 1.3  1999/05/13 22:44:36  vitus
 * - allocateDescription: real drive ID to description
 *
 * Revision 1.2  1999/03/24 23:01:52  vitus
 * - allocateDescription() adds drive ID to description (none)
 *
 * Revision 1.1  1999/03/06 20:12:26  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1999-2000
 */
static char const vcid[]="$Id: Missing.cpp,v 1.4 2000/04/10 00:48:36 vitus Exp $";

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
#include "resources.h"
#include "drvif.h"

#include "dskinfo.hpp"
#include "Missing.hpp"




/*# ----------------------------------------------------------------------
 * VMissing::VMissing(drive_id,mysize)
 *
 * PARAMETER
 *	drive_id	ID of this dummy
 *	mysize		dito, size
 *
 * RETURNS
 *	(nothing, C++)
 *
 * DESCRIPTION
 *	Creates a dummy VRAID object in memory.
 *
 * REMARKS
 */
VMissing::VMissing(DEVID drive_id,ULONG mysize)
{
    size = mysize;
    memcpy(id, drive_id, sizeof(DEVID));
    icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_MISSINGDRIVE);
}




/*# ----------------------------------------------------------------------
 * VMissing::~VMissing()
 *
 * PARAMETER
 *	(none, C++)
 * RETURNS
 *	(nothing, C++)
 *
 * DESCRIPTION
 *	Destroys dummy VRAID memory object, nearly nothing to do.
 *
 * REMARKS
 */
VMissing::~VMissing()
{
    WinDestroyPointer(icon);
}




/*# ----------------------------------------------------------------------
 * VMissing::allocateDescription()
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
VMissing::allocateDescription()
{
    char * descr = new char[80];
    sprintf(descr, "Missing device (unknown)\n"
	    "ID %02X%02X%02X%02X%02X%02X    %lu MByte",
	    id[0],id[1],id[2],id[3],id[4],id[5],
	    size / 2 / 1024);
    return descr;
}




/*# ----------------------------------------------------------------------
 * VMissing::queryIcon()
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
VMissing::queryIcon()
{
    return icon;
}




/*# ----------------------------------------------------------------------
 * VMissing::ioRemoveParent()
 *
 * PARAMETER
 *	(none, C++)
 * RETURNS
 *	0		OK
 *
 * DESCRIPTION
 *	Removes any traces of parent.  As we only live to support our
 *	parent: delete yourself.
 *
 * REMARKS
 */
int
VMissing::ioRemoveParent()
{
    delete this;
    return 0;					/* no errors */
}




/*# ----------------------------------------------------------------------
 * VMissing::ioChangeID(newid)
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
VMissing::ioChangeID(DEVID newid)
{
    memcpy(id, newid, sizeof(DEVID));		/* update object */
    return 0;
}




/*# ----------------------------------------------------------------------
 * VMissing::ioSync()
 *
 * PARAMETER
 *	(none, C++)
 * RETURNS
 *	count of errors
 *
 * DESCRIPTION
 *	'this' has been changed by the user -> update physical storage
 *	but there is no physical storage...
 *
 * REMARKS
 */
int
VMissing::ioSync()
{
    return 0;
}




/*# ----------------------------------------------------------------------
 * ViMssing::querySize()
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
VMissing::querySize()
{
    return size;
}




/*# ----------------------------------------------------------------------
 * VMissing::read(offset,count,buffer)
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
 *	Should read all childs and compare. xxx
 *	Should test 'valid'. xxx
 */
APIRET
VMissing::read(ULONG offset,ULONG count,PVOID buffer)
{
    return ERROR_INVALID_HANDLE;
}




/*# ----------------------------------------------------------------------
 * VMissing::write(offset,count,buffer)
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
VMissing::write(ULONG offset,ULONG count,PVOID buffer)
{
    APIRET	rc = 0;

    return ERROR_INVALID_HANDLE;
}
