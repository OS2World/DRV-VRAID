/*
 * $Source: r:/source/driver/raid/setup/RCS/dskinfo.hpp,v $
 * $Revision: 2.1 $
 * $Date: 2005/06/17 06:10:30 $
 * $Locker:  $
 *
 *      Define classes to represent RAID structure at runtime.
 *
 *      Reading block 0 means reading the sector containg configuration
 *      data about the device just read.
 *
 * $Log: dskinfo.hpp,v $
 * Revision 2.1  2005/06/17 06:10:30  vitus
 * - new setSize() method
 * - added New BSD license statement
 * - removed tabs
 *
 * Revision 1.16  2001/07/08 18:31:09  vitus
 * - enum ArrayState commented
 *
 * Revision 1.15  2001/01/15 01:25:47  vitus
 * - VDriveWithRedundancy: added method queryState()
 *
 * Revision 1.14  2000/08/17 02:05:21  vitus
 * - class VRDrive: addChild() parameter 'datav' changed to contain first invalid
 *   sector no.  So this class holds the same amount of information as a
 *   configuration sector.
 * class VDriveWithRedundancy: added ioInvalidChild(), ioStartRebuild()
 *
 * Revision 1.13  2000/08/07 00:50:10  vitus
 * - VDriveWithRedundancy::setBuild: added redo argument to distinguish
 *   between build and rebuild
 *
 * Revision 1.12  2000/06/15 01:21:31  vitus
 * - added queryFltHd and setFltHd methods
 *
 * Revision 1.11  1999/07/05 00:48:30  vitus
 * - VRDrive::addChild() gets additional flag "config sector valid"
 *
 * Revision 1.10  1999/06/21 01:32:56  vitus
 * - corrected english spelling
 *
 * Revision 1.9  1999/06/19 17:20:03  vitus
 * - added VOS2Disk::VOS2Disk(ULONG idx,ULONG handle) constructor
 *
 * Revision 1.8  1999/06/02 01:56:41  vitus
 * - VRDev: defines method queryWhere(), virtual base returns NULL
 * - VFRDev/VORDev: contains member inv_icon
 * - VRDrive: isValidChild(), two parameter sets
 *
 * Revision 1.7  1999/05/13 23:17:44  vitus
 * - VDriveWithRedundancy. added method ioRemoveChild()
 *
 * Revision 1.6  1999/04/29 01:31:48  vitus
 * - define MAX_CHILDS = 32, should be enough for everybody
 *
 * Revision 1.5  1999/03/15 03:25:33  vitus
 * - VRDev: added virtual destructure. Important, see Stroustroup
 *
 * Revision 1.4  1999/03/06 20:19:02  vitus
 * - added queryIcon() method
 *
 * Revision 1.3  1999/03/04 02:05:36  vitus
 * - removed any but abstract classes
 *
 * Revision 1.2  1999/03/01 00:56:58  vitus
 * - many more methods to get things working again
 *
 * Revision 1.1  1999/02/24 00:20:33  vitus
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
#define _DSKINFO_HPP    "$Id: dskinfo.hpp,v 2.1 2005/06/17 06:10:30 vitus Exp $"


typedef enum { false = 0, False = 0, true  = 1, True  = 1 } Boolean;

typedef enum { Ready = 0,                       /* all well */
               Build,                           /* at least one child is invalid */
               Rebuild,                         /* just rewrite all information */
               Fail,                            /* working w/o redundancy */
               Error                            /* data [partly] unavailable */
} ArrayState;


#define MAX_CHILDREN    32                      /* should be enough for everybody */

#define VERROR_NO_CONTENTS      0xFF00          /* application defined errorcode */






class VRDrive;                                  /* forward declaration */






/* ======================================================================
 * A disk of any kind.  It has the (pure virtual) methods to read and
 * write and has a size.
 * Anything more? */

class VDisk {

  public:
    virtual Boolean     isWritable() =0;
    virtual ULONG       querySize() =0;
    virtual APIRET      read(ULONG block,ULONG count,PVOID buffer) =0;
    virtual APIRET      write(ULONG block,ULONG count,PVOID buffer) =0;
};


class VOS2Disk : public VDisk {
    ULONG               pdskidx;
    ULONG               pdskhd;                 /* PDskOpen() handle */
    DEVICEPARAMETERBLOCK devparam;
    Boolean             writable;

    APIRET              create(ULONG idx,ULONG handle);


  public:
    VOS2Disk(ULONG idx);
    VOS2Disk(ULONG idx,ULONG handle);
    ~VOS2Disk();

    DEVICEPARAMETERBLOCK &queryDPB();
    ULONG               queryIndex();

    Boolean             isWritable() { return writable; }
    ULONG               querySize();
    APIRET              read(ULONG block,ULONG count,PVOID buffer);
    APIRET              write(ULONG block,ULONG count,PVOID buffer);
};






/* ======================================================================
 * VRDev
 * A special VDisk to contain all operations possible on all
 * VRAID entities (PHYSDEV, VRAID, HOST).  All linking is done
 * with objects of this type.
 */
class VRDev : public VDisk {
  protected:
    USHORT      flt_hd;                         /* accessable through vraid.flt? */
    VRDrive *   parent;
    DEVID       id;

  public:
    VRDev() { flt_hd = -1u; }
    virtual     ~VRDev() {}                     /* see Stroustroup pg216!!! */

    void                setParent(VRDrive * newparent) { parent = newparent; }
    VRDrive *           getParent() { return parent; }

    DEVID const *       queryID() { return &id; }
    virtual void        forceUnwritable() =0;

    /* If configuration changes ... */
    virtual int         ioDeleteYourself() =0;
    virtual int         ioRemoveParent() =0;
    virtual int         ioChangeID(DEVID) =0;
    virtual int         ioSync() =0;

    USHORT const        queryFltHd() { return flt_hd; }
    virtual VOS2Disk *  queryWhere() { return NULL; }

    virtual UCHAR       isArray() =0;           /* returns type or array or 0 */
    virtual Boolean     isHostdrive() =0;
    virtual char *      allocateDescription() =0;
    virtual HPOINTER    queryIcon() =0;
};






/* ======================================================================
 * V{F|O}RDev
 * The complete VRAID partition.  Either accessed via filter driver
 * or directly through OS/2.
 */
class VFRDev : public VRDev {
  private:
    ULONG       partsize;
    ULONG       size;                           /* may be less than partition! */
    Boolean     writable;
    HPOINTER    inv_icon, rw_icon, ro_icon;

  public:
    /* Create from filter handle... */
    VFRDev(USHORT flthd);
    ~VFRDev();

    int         ioDeleteYourself();
    int         ioRemoveParent();
    int         ioChangeID(DEVID);              // doesn't do I/O...
    int         ioSync();

    UCHAR       isArray() { return 0; }
    Boolean     isHostdrive() { return False; }
    char *      allocateDescription();
    HPOINTER    queryIcon();

    void        forceUnwritable() { writable = False; }
    Boolean     isWritable() { return writable; }
    ULONG       querySize();
    APIRET      read(ULONG offset,ULONG count,PVOID buffer);
    APIRET      write(ULONG offset,ULONG count,PVOID buffer);
};




class VORDev : public VRDev {
  private:
    VOS2Disk *  theDisk;
    ULONG       partstart, partsize;
    ULONG       size;
    Boolean     writable;
    Boolean     delete_disk;
    HPOINTER    inv_icon, rw_icon, ro_icon;

  public:
    VORDev(VOS2Disk * where,ULONG pstart,ULONG psize); /* create from PDsk*() index*/
    ~VORDev();

    int         ioDeleteYourself();
    int         ioRemoveParent();
    int         ioChangeID(DEVID);              // doesn't do I/O...
    int         ioSync();

    VOS2Disk *  queryWhere() { return theDisk; }

    UCHAR       isArray() { return 0; }
    Boolean     isHostdrive() { return False; }
    char *      allocateDescription();
    HPOINTER    queryIcon();

    void        forceUnwritable() { writable = False; }
    USHORT const queryFltHd() { return -1u; }
    Boolean     isWritable() { return writable; }
    ULONG       querySize();
    APIRET      read(ULONG offset,ULONG count,PVOID buffer);
    APIRET      write(ULONG offset,ULONG count,PVOID buffer);
};






/* ======================================================================
 * VOS2Drive
 * Dummy added to conform to C++ syntax.
 * This is a OS/2 drive *not* containing any VRAID structures but directly
 * representing the total disk.
 * It's directly accessable by OS/2 (thus 'hostdrive') but no array.
 */
class VOS2Drive : public VRDev {
  private:
    VOS2Disk *  theDisk;
    Boolean     writable;
    Boolean     delete_disk;
    HPOINTER    rw_icon, ro_icon;

  public:
    VOS2Drive(VOS2Disk *);
    ~VOS2Drive();

    VOS2Disk *  queryWhere() { return theDisk; }
    int         ioDeleteYourself();             /* impossible.  marks for deletion. */
    int         ioRemoveParent();               // returns error, sorry
    int         ioChangeID(DEVID);              // dito
    int         ioSync();

    UCHAR       isArray() { return 0; }
    Boolean     isHostdrive() { return True; }
    char *      allocateDescription();
    HPOINTER    queryIcon();

    void        forceUnwritable() { writable = False; }
    USHORT const queryFltHd() { return -1u; }
    Boolean     isWritable() { return writable; }
    ULONG       querySize();
    APIRET      read(ULONG block,ULONG count,PVOID buffer);
    APIRET      write(ULONG block,ULONG count,PVOID buffer);
};






/* ======================================================================
 * VRDrive
 * A possible hostdrive, anything above a PHYSDEVICE.
 */
class VRDrive : public VRDev {
  public:
    void                setFltHd(USHORT hd) { flt_hd = hd; }
    virtual void        setHostdrive(ULONG os2idx = -1ul) =0;
    virtual void        addChild(VRDev * newchild, Boolean cfgv,
                                 ULONG datav = -1ul) =0;

    virtual int         queryChildren() =0;
    virtual VRDev *     getChild(int which) =0;
    virtual Boolean     isValidChild(int which) { return True; }
    virtual Boolean     isValidChild(VRDev * child) { return True; }
};






/* ======================================================================
 * VDriveWithRedundancy
 * Fail-safe devices are based on this abstract class.
 */

class VDriveWithRedundancy : public VRDrive {
  public:
    virtual void        setSize(ULONG newsize) =0; // those devices are changable
    virtual void        setBuild(ULONG nextsec, Boolean redo = False) =0;
    virtual ArrayState  queryState() =0;
    virtual int         ioRemoveChild(VRDev * child) =0;

    virtual int         ioInvalidChild(DEVID const id_of_invalid_child,
                                       ULONG from_this_sector_on) =0;

    virtual int         ioStartRebuild(ULONG start_sector) =0;
};
