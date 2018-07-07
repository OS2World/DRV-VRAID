/*
 * $Source: r:/source/driver/raid/setup/RCS/mirror.hpp,v $
 * $Revision: 2.1 $
 * $Date: 2005/06/17 06:09:37 $
 * $Locker:  $
 *
 *      Declaration of class VMirror.
 *
 * $Log: mirror.hpp,v $
 * Revision 2.1  2005/06/17 06:09:37  vitus
 * - new setSize() method
 * - added New BSD license statement
 * - removed tabs
 *
 * Revision 1.13  2001/01/15 01:28:47  vitus
 * - VMirror: implemented queryState()
 *
 * Revision 1.12  2000/08/17 02:16:32  vitus
 * - method addChild() parameter 'datav' changed to sector no.
 * - method querySize() now inline.
 * - added ioInvalidChild(), ioStartRebuild()
 *
 * Revision 1.11  2000/08/07 01:25:37  vitus
 * - adding support for rebuild state
 *
 * Revision 1.10  1999/07/05 00:48:49  vitus
 * - addChild() gets additional flag "config sector valid", saved in child array
 *
 * Revision 1.9  1999/06/21 01:30:52  vitus
 * - corrected english spelling
 *
 * Revision 1.8  1999/06/19 22:05:23  vitus
 * - added 'state' member to hold fail/error/ready/build status
 *
 * Revision 1.7  1999/06/19 17:22:31  vitus
 * - minor corrections (setHostdrive)
 *
 * Revision 1.6  1999/06/02 01:46:18  vitus
 * - added inv_icon member (icon if child is invalid in respect to parent)
 * - isValidChild() now non-inline and with two incarnations
 *
 * Revision 1.5  1999/05/26 01:33:42  vitus
 * - added isValidChild(int which)
 *
 * Revision 1.4  1999/05/13 22:41:21  vitus
 * - added ioRemoveChild()
 * - removed bdone, handled per child
 *
 * Revision 1.3  1999/04/13 01:26:18  vitus
 * - child[] now static member of object
 *
 * Revision 1.2  1999/03/08 05:08:02  vitus
 * - added queryIcon() method
 *
 * Revision 1.1  1999/03/04 02:06:28  vitus
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
#define _MIRROR_HPP     "$Id: mirror.hpp,v 2.1 2005/06/17 06:09:37 vitus Exp $"





class VMirror : public VDriveWithRedundancy {
  private:
    Boolean     hostdrive;
    ULONG       os2access;

    int         children;
    struct _ChildInfo {
        VRDev * rdev;
        ULONG   valid;
        Boolean cfgok;
    } child[MAX_CHILDREN];

    ULONG       size;
    Boolean     writable;
    HPOINTER    inv_icon, rw_icon, ro_icon;
    ArrayState  state;


  public:
    VMirror(DEVID drive_id,int nchd);
    ~VMirror();

    void        setSize(ULONG newsize);
    void        setBuild(ULONG nextsec, Boolean redo = False);
    int         ioRemoveChild(VRDev * child);
    int         ioInvalidChild(DEVID const id_of_invalid_child,
                               ULONG from_this_sector_on);
    int         ioStartRebuild(ULONG start_sector);

    int         ioDeleteYourself();
    int         ioRemoveParent();
    int         ioChangeID(DEVID);
    int         ioSync();

    UCHAR       isArray() { return RDTYPE_MIRROR; }
    Boolean     isHostdrive() { return hostdrive; }
    char *      allocateDescription();
    HPOINTER    queryIcon();
    ArrayState  queryState() { return state; }
    int         queryChildren();
    VRDev *     getChild(int which);
    Boolean     isValidChild(int which);
    Boolean     isValidChild(VRDev * which);

    void        setHostdrive(ULONG os2idx = -1UL);
    void        addChild(VRDev * newchild, Boolean cfgv, ULONG datav = -1ul);

    void        forceUnwritable() { writable = False; }
    Boolean     isWritable() { return writable; }
    ULONG       querySize() { return size; }
    APIRET      read(ULONG offset,ULONG count,PVOID buffer);
    APIRET      write(ULONG offset,ULONG count,PVOID buffer);
};

