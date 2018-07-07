/*
 * $Source: r:/source/driver/raid/setup/RCS/mirror.cpp,v $
 * $Revision: 2.1 $
 * $Date: 2005/06/17 06:09:03 $
 * $Locker:  $
 *
 *      Implements VMirror class.
 *
 * $Log: mirror.cpp,v $
 * Revision 2.1  2005/06/17 06:09:03  vitus
 * - new setSize() method
 * - added New BSD license statement
 * - removed tabs
 *
 * Revision 1.19  2001/07/08 18:33:25  vitus
 * - ioInvalidChild: next state is Build now Rebuild
 *
 * Revision 1.18  2001/01/30 00:34:01  vitus
 * - code correctly distinguishes between build and rebuild
 *
 * Revision 1.17  2001/01/23 01:40:33  vitus
 * - VMirror::ioStartRebuild: lokale Variable flt_hd umbenannt
 *
 * Revision 1.16  2001/01/15 01:35:27  vitus
 * - VMirror::ioInvalidChild: modifies child[i].valid
 *
 * Revision 1.15  2000/08/17 02:25:42  vitus
 * - method addChild() parameter 'datav' changed to sector no.
 * - method querySize() removed (now inline)
 * - implemented ioInvalidChild(), ioStartRebuild()
 *
 * Revision 1.14  2000/08/07 01:24:57  vitus
 * - adding support for rebuild state
 *
 * Revision 1.13  2000/04/10 00:54:49  vitus
 * - include VSetup.h instead of proto.h
 * - converted C++ to C comments
 *
 * Revision 1.12  1999/12/14 00:26:40  vitus
 * - more error logging
 *
 * Revision 1.11  1999/07/05 00:45:18  vitus
 * - addChild() gets additional flag "config sector valid"
 * - modifications to keep this flag in child table and honour it when reading
 *   from admin space
 * - ioSync: updates current config sector if it's still the same array.
 *   Recreating it did change the size of the array.
 *
 * Revision 1.10  1999/06/21 01:33:40  vitus
 * - corrected english spelling
 *
 * Revision 1.9  1999/06/19 22:06:40  vitus
 * - minor corrections to keep compiler happy
 * - detected states include now ready, fail and build
 *
 * Revision 1.8  1999/06/02 01:43:11  vitus
 * - queryIcon() returns ICO_INVCHILDARRAY if parent says that this
 *   child is invalid
 * - isValidChild(int/VRDev *) now located here
 * - allocateDescription: adds "building" to text
 * - addChild: set building flag if child is invalid
 *
 * Revision 1.7  1999/05/13 22:40:27  vitus
 * - array description includes "building" or "ready"
 * - adding an invalid child causes building state
 * - ioRemoveParent: clear parent's admin sector
 * - ioDeleteYourself: no disk i/o
 * - ioRemoveChild() implemented
 *
 * Revision 1.6  1999/05/12 01:30:43  vitus
 * - ioSync: new sector layout
 *
 * Revision 1.5  1999/04/13 01:25:03  vitus
 * - child[] now static member of object
 *
 * Revision 1.4  1999/03/24 23:01:06  vitus
 * - allocateDescription() displays drive ID
 *
 * Revision 1.3  1999/03/23 00:50:14  vitus
 * - read method reads all childs and compares.  Currently differences are
 *   only logged.  Better ideas?
 *
 * Revision 1.2  1999/03/22 01:59:33  vitus
 * - ioDeleteYourself: first clear own sector and afterwards call childs
 *
 * Revision 1.1  1999/03/06 20:09:47  vitus
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
static char const vcid[]="$Id: mirror.cpp,v 2.1 2005/06/17 06:09:03 vitus Exp $";

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define __IBMC__        0
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
#include "Mirror.hpp"





/*# ----------------------------------------------------------------------
 * VMirror::VMirror(drive_id,nchd)
 *
 * PARAMETER
 *      drive_id        ID of this chain
 *      nchd            child count
 *
 * RETURNS
 *      (nothing, C++)
 *
 * DESCRIPTION
 *      Creates a new VRAID object in memory.
 *
 * REMARKS
 */
VMirror::VMirror(DEVID drive_id, int nchd)
{
    parent = NULL;
    children = 0;
    size = -1ul;
    hostdrive = False;
    writable = True;
    state = Ready;
    memcpy(id, drive_id, sizeof(DEVID));

    inv_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_INVCHILDARRAY);
    rw_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_RAIDDRIVE);
    ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDARRAY);
}




/*# ----------------------------------------------------------------------
 * VMirror::~VMirror()
 *
 * PARAMETER
 *      (none, C++)
 * RETURNS
 *      (nothing, C++)
 *
 * DESCRIPTION
 *      Destroys VRAID memory object.
 *
 * REMARKS
 */
VMirror::~VMirror()
{
    WinDestroyPointer(rw_icon);
    WinDestroyPointer(ro_icon);
}




/*# ----------------------------------------------------------------------
 * VMirror::setBuild(nextsec,[rebuild])
 *
 * PARAMETER
 *      nextsec         where to restart the buildimg process
 *                      (0: complete restart)
 * RETURNS
 *      (nothing)
 * DESCRIPTION
 *      Configures the BUILD process for this array.  Either when reading
 *      the configuration sector or when adding devices etc.
 *
 * REMARKS
 */
void
VMirror::setBuild(ULONG nextsec, Boolean redo)
{
    if( redo )
        state = Rebuild;
    else
        state = Build;
}




/*# ----------------------------------------------------------------------
 * VMirror::queryChildren()
 *
 * PARAMETER
 *      (none)
 * RETURNS
 *      count
 * DESCRIPTION
 *      Returns count of children.
 *
 * REMARKS
 */
int
VMirror::queryChildren()
{
    return children;
}




/*# ----------------------------------------------------------------------
 * VMirror::getChild(index)
 *
 * PARAMETER
 *      index           which child
 * RETURNS
 *      pointer
 *
 * DESCRIPTION
 *      Returns pointer to a child.
 *
 * REMARKS
 */
VRDev *
VMirror::getChild(int index)
{
    if( index >= children )
        return 0;
    return child[index].rdev;
}




Boolean
VMirror::isValidChild(int which)
{
    assert( which < children );
    return (Boolean)(child[which].valid == -1ul);
}




Boolean
VMirror::isValidChild(VRDev * which)
{
    for( int i = 0; i < children; ++i )
        if( which == child[i].rdev )
            return (Boolean)(child[i].valid == -1ul);
    assert( 0 );
    return False;                               /* xxx? */
}




/*# ----------------------------------------------------------------------
 * VMirror::allocateDescription()
 *
 * PARAMETER
 *      (none)
 * RETURNS
 *      ASCIIZ
 *
 * DESCRIPTION
 *      Allocates buffer and fills it with description of this object.
 *
 * REMARKS
 */
char *
VMirror::allocateDescription()
{
    static char * State2Str[] = { "ready", "build", "rebuild", "fail", "error" };
    char *      descr = new char[80];
    int         found = 0;

    for( int i = 0; i < children; ++i )
        if( child[i].rdev != 0  &&  child[i].rdev->isArray() != 0xFF )
            ++found;
    if( found < children )
        state = Fail;                           /* always! */

    sprintf(descr, "RAID 1 (Mirroring, %s)", State2Str[state]);

    if( hostdrive == True  &&  os2access != -1UL )
        sprintf(&descr[strlen(descr)], " (OS/2 Drive %lu)", os2access);

    sprintf(&descr[strlen(descr)], "\nID %02X%02X%02X%02X%02X%02X    %lu MByte",
            id[0],id[1],id[2],id[3],id[4],id[5], size / 2 / 1024);
    return descr;
}




/*# ----------------------------------------------------------------------
 * VMirror::queryIcon()
 *
 * PARAMETER
 *      (none)
 * RETURNS
 *      HPOINTER to display
 * DESCRIPTION
 *      Returns icon to reflect the logical state of this drive.
 *
 * REMARKS
 */
HPOINTER
VMirror::queryIcon()
{
    if( parent != 0  &&  parent->isValidChild(this) == False )
        return inv_icon;
    else if( writable == True )
        return rw_icon;
    else
        return ro_icon;
}




/*# ----------------------------------------------------------------------
 * VMirror::setHostdrive(os2idx)
 *
 * PARAMETER
 *      os2idx          accessable through this PDsk*() index
 *                      or -1 if it isn't accessable.
 *
 * RETURNS
 *      (nothing)
 *
 * DESCRIPTION
 *      Marks this array as a host drive with or w/o
 *      OS/2 access.
 *
 * REMARKS
 */
void
VMirror::setHostdrive(ULONG os2idx)
{
    hostdrive = True;
    os2access = os2idx;
}




/*# ----------------------------------------------------------------------
 * VMirror::setSize(newsize)
 *
 * PARAMETER
 *      newsize         new size of array
 *
 * RETURNS
 *      (nothing)
 *
 * DESCRIPTION
 *      RAID 1 devices may be smaller than any part of it (but not larger)
 *
 * REMARKS
 *      This method could be avoided if the constructor would analyze
 *      the "VRAIDDEVICE2" itself.  Redesign the device scanner?        xxx
 */
void
VMirror::setSize(ULONG newsize)
{
    if( newsize < size )
        size = newsize;                         /* smaller is possible */
    return;
}




/*# ----------------------------------------------------------------------
 * VMirror::addChild(newchild, cfgv, datav)
 *
 * PARAMETER
 *      newchild        pointer to child to add
 *      cfgv            configuration already written?
 *      datav           whether data of this child is valid (uptodate)
 *
 * RETURNS
 *      (nothing)
 *
 * DESCRIPTION
 *      Adds a new child to our VRAID object.
 *
 * REMARKS
 */
void
VMirror::addChild(VRDev * newchild, Boolean cfgv, ULONG datav)
{
    assert( newchild != NULL );

    /* Set double link between child and parent. */

    child[children].rdev = newchild;
    child[children].cfgok = cfgv;
    child[children].valid = datav;
    newchild->setParent(this);

    /* Update our object with child's information. */

    ULONG       childsize = newchild->querySize();
    if( size > childsize )
        size = childsize;
    if( newchild->isWritable() == False )
        writable = False;                       /* oups, it isn't 'changable' */

    if( state == Ready )
    {
        if( !cfgv )
            state = Build;
        else if( datav != -1ul )
            state = Fail;
    }
    ++children;
}




/*# ----------------------------------------------------------------------
 * VMirror::ioRemoveChild(rdev)
 *
 * PARAMETER
 *      rdev            child, to be removed
 *
 * RETURNS
 *      count of errors
 *
 * DESCRIPTION
 *      Remove any traces of ourself from physical storage.
 *      This is done by telling all children that we are about
 *      to be killed (they might set the "HOSTDRIVE" in their
 *      configuration sectors) and clearing our own sector.
 *
 * REMARKS
 */
int
VMirror::ioRemoveChild(VRDev * rdev)
{
    int         i;
    int         errors = 0;
    Boolean     found = False;


    do
    {
        /* Search this child in internal tables. */

        for( i = 0; i < children; ++i )
        {
            if( child[i].rdev == rdev )
            {
                int     j, k;

                errors += child[i].rdev->ioRemoveParent();
                for( j = i, k = i+1; k < children; ++j, ++k )
                {
                    child[j].rdev = child[k].rdev;
                    child[j].valid = child[k].valid;
                }
                --children;
                found = True;
                break;
            }
        }
        if( !found )
        {
            Verbose(1, "VMirror", "can't find child to remove!");
            ++errors;
            break;
        }


        /* Now update configuration sector. */

        errors += ioSync();
    }
    while(0);

    return errors;
}




/*# ----------------------------------------------------------------------
 * VMirror::ioDeleteYourself()
 *
 * PARAMETER
 *      this            to be removed
 *
 * RETURNS
 *      count of errors
 *
 * DESCRIPTION
 *      Remove any traces of ourself from physical storage.
 *      This is done by telling all children that we are about
 *      to be killed (they might set the "HOSTDRIVE" in their
 *      configuration sectors) and clearing our own sector.
 *
 * REMARKS
 */
int
VMirror::ioDeleteYourself()
{
    int         i;
    int         errors = 0;

    /* Update all children (parent removed -> marked in sectors). */

    for( i = 0; i < children; ++i )
    {
        errors += child[i].rdev->ioRemoveParent();
        child[i].rdev = NULL;
    }

    return errors;
}




/*# ----------------------------------------------------------------------
 * VMirror::ioRemoveParent()
 *
 * PARAMETER
 *      (none, C++)
 * RETURNS
 *      0               OK
 *
 * DESCRIPTION
 *      Removes any traces of parent from configuration sectors.  Means
 *      this array becomes hostdrive!
 *
 * REMARKS
 */
int
VMirror::ioRemoveParent()
{
    PSEC_VRDEV2 sec = new SEC_VRDEV2;
    int         errors = 0;
    APIRET      rc;


    /* 1st: clear administrative sector of old parent. */

    memset(sec, 0, SECTOR_SIZE);
    rc = write(1, 1, sec);
    if( rc != 0 )
    {
        Verbose(1, "VMirror::ioRemoveParent", "can't clear parent SEC_VRDEV - rc %lu", rc);
        ++errors;
    }


    /* 2nd: update FDFLAG_HOSTDRIVE */

    do
    {
        rc = read(0, 1, sec);                   /* remember: 0 defines ourself */
        if( rc != 0 )
        {
            Verbose(1, "VMirror::ioRemoveParent", "can't read own SEC_VRDEV2 - rc %lu", rc);
            ++errors;
            break;
        }

        sec->u.s.flags |= RDFLAG_HOSTDRIVE;     /* no parent means hostdrive */
        sec->timestamp = time(NULL);            /* UTC of change */
        sec->crc = Crc16(sec, SECTOR_SIZE-2);   /* !!! */

        rc = write(0, 1, sec);
        if( rc != 0 )
        {
            Verbose(1, "VMirror::ioRemoveParent", "can't write own SEC_VRDEV2 - rc %lu", rc);
            ++errors;
            break;
        }

        parent = NULL;                          /* none */
    }
    while(0);

    delete sec;
    return errors;
}




/*# ----------------------------------------------------------------------
 * VMirror::ioChangeID(newid)
 *
 * PARAMETER
 *      newid           new ID
 * RETURNS
 *      count of errors
 *
 * DESCRIPTION
 *      Changes it's own DEVID.
 *
 * REMARKS
 */
int
VMirror::ioChangeID(DEVID newid)
{
    PSEC_VRDEV2 sec = new SEC_VRDEV2;
    int         errors = 0;

    memcpy(id, newid, sizeof(DEVID));           /* update object */

    do
    {
        APIRET  rc;

        rc = read(0, 1, sec);                   /* remember: 0 defines ourself */
        if( rc != 0 )
        {
            Verbose(1, "VMirror::ioChangeID", "can't read own SEC_VRDEV2 - rc %lu", rc);
            ++errors;
            break;
        }

        memcpy(sec->u.s.id, newid, sizeof(DEVID));
        sec->timestamp = time(NULL);            /* UTC of change */
        sec->crc = Crc16(sec, SECTOR_SIZE-2);   /* !!! */

        rc = write(0, 1, sec);
        if( rc != 0 )
        {
            Verbose(1, "VMirror::ioChangeID", "can't write own SEC_VRDEV2 - rc %lu", rc);
            ++errors;
            break;
        }
    }
    while(0);

    delete sec;
    return errors;
}




/*# ----------------------------------------------------------------------
 * VMirror::ioInvalidChild(id_of_invalid_child, from_this_sector_on)
 *
 * PARAMETER
 *      id_of_invalid_child     DEVID
 *      from_this_sector_on     first sector which needs update
 *
 * RETURNS
 *      0               OK
 *      /0              something went wrong (I/O or internal error)
 *
 * DESCRIPTION
 *      Someone detected that a child of ours has an invalid sector
 *      and tells us via this method.
 *      If VRAID.FLT is running and this array is known to it just
 *      tell it and the FLT will modify it's structures and start a
 *      build.
 *      If VRAID.FLT is not running just modify the configuration
 *      sector.
 *
 * REMARKS
 */
int
VMirror::ioInvalidChild(DEVID const id_of_invalid, ULONG from_this_sector_on)
{
    int child_idx = -1;

    for( int i = 0; i < children; ++i )
    {
        if( memcmp(child[i].rdev->queryID(), id_of_invalid, sizeof(DEVID)) == 0 )
        {
            child_idx = i;
            break;
        }
    }
    assert( child_idx != -1 );

    if( child[child_idx].valid != -1ul
        &&  from_this_sector_on >= child[child_idx].valid )
    {
        /* This child is already invalid, we have nothing to do. */

        return 0;
    }
    child[child_idx].valid = from_this_sector_on;
    if( state == Ready )
        state = Build;


    if( queryFltHd() != -1u )
    {
        VRAID_CHILD_PARAM       param;

        /* Tell VRAID.FLT that our array has an invalid child. */

        param.handle = queryFltHd();
        memcpy(param.id, id_of_invalid, sizeof(DEVID));
        param.flags = 0;                        /* not valid */
        param.secno = from_this_sector_on;

        return (DriverArrayChild(&param) != 0);
    }
    else
    {
        return ioSync();
    }
}




/*# ----------------------------------------------------------------------
 * VMirror::ioStartRebuild(start_sector)
 *
 * PARAMETER
 *      start_sector            sector number on array
 *
 * RETURNS
 *      0               OK
 *      1               vraid.flt not loaded
 *      2               vraid.flt dos not know this array
 *
 * DESCRIPTION
 *      Starts a rebuild process on an array w/o invalid children.
 *
 * REMARKS
 */
int
VMirror::ioStartRebuild(ULONG start_sector)
{
    USHORT const hd = queryFltHd();

    if( hd == 0xFFFF )
    {
        MyMessageBox(0, HWND_DESKTOP,
                     "The array you selected isn't known to"
                     " vraid.flt.  Therefore it can't be rebuild"
                     " by the driver.  Please reboot to let"
                     " vraid.flt analyze the new situation and"
                     " retry the operation.");
        return 1;
    }

    APIRET      rc;
    rc = DriverArrayRebuild(hd, start_sector);
    if( rc != 0 )
    {
        MyMessageBox(0, HWND_DESKTOP,
                     "The array you selected can't be rebuild"
                     " by vraid.flt.  You need to reboot"
                     " to let vraid.flt analyze a new situation.");
        return 2;
    }
    setBuild(start_sector, True);               /* _re_build */
    return 0;
}




/*# ----------------------------------------------------------------------
 * VMirror::ioSync()
 *
 * PARAMETER
 *      (none, C++)
 * RETURNS
 *      count of errors
 *
 * DESCRIPTION
 *      'this' has been changed by the user -> update physical storage.
 *
 * REMARKS
 */
int
VMirror::ioSync()
{
    PSEC_VRDEV2 sec = new SEC_VRDEV2;
    USHORT      i;
    ULONG       ul;
    int         errors = 0;
    Boolean     update = False;                 /* True: modify sector */
    APIRET      rc;

    do
    {
        rc = read(0, 1, sec);
        if( rc != 0 )
            break;                      /* read error?  Assume no cfg sectors */
        if( memcmp(sec->sectype, "VRAIDDEVICE2    ", 16) != 0 )
            break;
        if( Crc16(sec, sizeof(*sec)-2) != sec->crc )
            break;
        if( memcmp(sec->u.s.id, id, sizeof(DEVID)) != 0 )
            break;
        if( sec->u.s.type != RDTYPE_MIRROR )
            break;

        update = True;
    }
    while( 0 );


    if( update == True )
    {
        Verbose(1, "VMirror", "updating current configuration sector");

        /* Update configuration sector with new contents.  Keep
         * in mind, that the outside view should be unchanged. */

        sec->timestamp = time(NULL);
        sec->u.s.flags = (UCHAR)(parent != 0 ? 0 : 0x80);
        sec->u.s.children = children;


        /* 2nd: keep drive size, keep size of children.
         * The current values shouldn't be changed! */

        ul = sec->u.s.child[0].size;            /* drive size = size of any child */
        size = ul;
    }
    else
    {
        /* New sector contents. */

        Verbose(1, "VMirror", "creating new configuration sector");


        /* First: fill configuration sector of current level. */

        memset(sec, 0, sizeof(*sec));
        memcpy(sec->sectype, "VRAIDDEVICE2    ", 16);
        sec->timestamp = time(NULL);

        memcpy(sec->u.s.id, id, sizeof(DEVID));
        sec->u.s.type = RDTYPE_MIRROR;
        sec->u.s.flags = (UCHAR)(parent != 0 ? 0 : 0x80);

        sec->u.s.children = children;


        /* 2nd: recalculate drive size, correct size of children.
         * The current values were only wild guesses. */

        ul = (ULONG)ULONG_MAX;
        for( i = 0; i < children; ++i )
            ul = min(ul, child[i].rdev->querySize());
        size = ul;
    }


    /* 3rd: update all children and record their IDs and flags. */

    for( i = 0; i < children; ++i )
    {
        child[i].rdev->ioSync();
        memcpy(sec->u.s.child[i].id, child[i].rdev->queryID(), sizeof(DEVID));
        sec->u.s.child[i].size = size;
        if( (sec->u.s.child[i].valid = child[i].valid) == -1ul )
            sec->u.s.child[i].flags |= 0x01;    /* child is valid */
    }
    if( state == Build )
        sec->u.s.flags |= RDFLAG_BUILDING;
    else if( state == Rebuild )
        sec->u.s.flags |= RDFLAG_REBUILD;


    /* Last: write administrative sector of current level. */

    sec->crc = Crc16(sec, SECTOR_SIZE-2);
    rc = write(0, 1, sec);
    if( rc != 0 )
    {
        Verbose(1, "VMirror::ioSync", "write(0,1,...) - rc %lu, not updated", rc);
        ++errors;
    }

    delete sec;
    return errors;
}




/*# ----------------------------------------------------------------------
 * VMirror::read(offset,count,buffer)
 *
 * PARAMETER
 *      offset          offset in configuratoin space
 *      count           sector count
 *      buffer          sector data
 *
 * RETURNS
 *      0               OK
 *      /0              APIRET
 *
 * DESCRIPTION
 *      Reads data from configuration sectors of this VRAID drive.
 *
 * REMARKS
 *      Should read all children and compare. xxx
 *      Should test 'valid'. xxx
 */
APIRET
VMirror::read(ULONG offset, ULONG count, PVOID buffer)
{
    PUCHAR      copybuf = new UCHAR[count];
    int         goodchildren = 0;
    APIRET      rc = VERROR_NO_CONTENTS;

    for( int i = 0; i < children; ++i )
    {
        if( !child[i].cfgok )
            continue;                           /* nothing on this child */

        rc = child[i].rdev->read(offset+1, count, buffer);
        if( rc != 0 )
            continue;                           /* Verbose() already called */
        if( goodchildren == 0 )
            memcpy(copybuf, buffer, (size_t)count);
        else if( memcmp(copybuf, buffer, (size_t)count) != 0 )
            Verbose(0, "VMirror", "Data error when reading child %d, ignored", i);
        ++goodchildren;
    }

    delete[] copybuf;
    return (goodchildren == 0 ? rc : 0);
}




/*# ----------------------------------------------------------------------
 * VMirror::write(offset,count,buffer)
 *
 * PARAMETER
 *      offset          offset in configuratoin space
 *      count           sector count
 *      buffer          sector data
 *
 * RETURNS
 *      0               OK
 *      /0              APIRET
 *
 * DESCRIPTION
 *      Writes data to configuration sectors of this VRAID drive.
 *
 * REMARKS
 *      Should test 'valid'. xxx
 */
APIRET
VMirror::write(ULONG offset, ULONG count, PVOID buffer)
{
    APIRET      rc = 0;

    for( int i = 0; i < children; ++i )
        rc |= child[i].rdev->write(offset+1, count, buffer);
    return rc;
}
