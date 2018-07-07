/*
 * $Source: R:/Source/driver/raid/Setup/RCS/VOS2Drive.cpp,v $
 * $Revision: 1.5 $
 * $Date: 2000/04/10 00:57:54 $
 * $Locker:  $
 *
 *	Implements classes VRDrive and higher.
 *
 * $Log: VOS2Drive.cpp,v $
 * Revision 1.5  2000/04/10 00:57:54  vitus
 * - include VSetup.h instead of proto.h
 * - converted C++ to C comments
 *
 * Revision 1.4  1999/03/24 23:48:39  vitus
 * - added assert()'s
 *
 * Revision 1.3  1999/03/08 05:04:47  vitus
 * - implemented queryIcon()
 * - destructor deletes theDisk if ioDeleteYourself() hasn't been called
 *
 * Revision 1.2  1999/03/01 01:00:12  vitus
 * - removed all class implementation except VOS2Drive's
 * - renamed to VOS2Drive.cpp
 *
 * Revision 1.1  1999/02/24 00:18:34  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1999-2000
 */
static char const vcid[]="$Id: VOS2Drive.cpp,v 1.5 2000/04/10 00:57:54 vitus Exp $";

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
#include "drvif.h"
#include "resources.h"

#include "dskinfo.hpp"






/* **********************************************************************
 * **** VOS2Drive::* ****************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * CALL
 * PARAMETER
 * RETURNS
 * DESCRIPTION
 *	Create VOS2Drive (an OS/2 accessable disk without any VRAID
 *	partition from VOS2Disk object).
 *
 * REMARKS
 */
VOS2Drive::VOS2Drive(VOS2Disk * disk)
{
    assert( disk != NULL );
    assert( disk->querySize() != 0 );

    delete_disk = True;
    theDisk = disk;
    parent = NULL;
    writable = theDisk->isWritable();
    memset(&id, 0, sizeof(DEVID));

    rw_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_FREEDRIVE);
    ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_LOCKEDDRIVE);

}




VOS2Drive::~VOS2Drive()
{
    if( delete_disk == True )
        delete theDisk;				/* no need for that disk object */

    WinDestroyPointer(rw_icon);
    WinDestroyPointer(ro_icon);
}




char *
VOS2Drive::allocateDescription()
{
    char * descr = new char[80];
    sprintf(descr, "OS/2 Device %lu\n%lu MByte",
	    theDisk->queryIndex(), querySize() / 2 / 1024u);
    return descr;
}




HPOINTER
VOS2Drive::queryIcon()
{
    if( writable == True )
	return rw_icon;
    else
	return ro_icon;
}




/*# ----------------------------------------------------------------------
 * CALL
 *      VOS2Drive::ioDeleteYourself()
 * PARAMETER
 *      ((none)
 * RETURNS
 *      0               OK
 * DESCRIPTION
 *      As OS/2 Drives aren't deletable from this application I have
 *      "reused" this method to modify the operation of the destructor:
 *      normally it would would delete 'theDisk' but when this method
 *      has been called it will only delete itself.  Used when we
 *      switch from VOS2Drive to VORDev by crating a VRAID partition.
 *
 * REMARKS
 */
int
VOS2Drive::ioDeleteYourself()
{
    delete_disk = False;
    return 0;
}




int
VOS2Drive::ioRemoveParent()
{
    assert( 0 );				/* we have no parent */
    return 1;					/* "1" error :-/ */
}




int
VOS2Drive::ioChangeID(DEVID)
{
    assert( 0 );				/* gibt's nich */
    return 1;					/* "1" error :-/ */
}




int
VOS2Drive::ioSync()
{
    assert( 0 );				/* nothing changable, create other! */
    return 1;
}




ULONG
VOS2Drive::querySize()
{
    return theDisk->querySize();
}




APIRET
VOS2Drive::read(ULONG block,ULONG count,PVOID buffer)
{
    return theDisk->read(block,count,buffer);
}




APIRET
VOS2Drive::write(ULONG block,ULONG count,PVOID buffer)
{
    return theDisk->write(block,count,buffer);
}
