/*
 * $Source: r:/source/driver/raid/setup/RCS/missing.hpp,v $
 * $Revision: 2.1 $
 * $Date: 2005/06/17 06:10:02 $
 * $Locker:  $
 *
 *      Declaration of class VMissing.
 *      Simplifies handling of missing devices when scanning
 *      the existing configuration.  Holds information (most
 *      notible DEVID) between scan and rewrite of configuration
 *      sectors but may even disappear while running VSetup: when
 *      it's parent is removed.
 *
 *      Implement this class with the richest possible interface.
 *
 * $Log: missing.hpp,v $
 * Revision 2.1  2005/06/17 06:10:02  vitus
 * - new setSize() method
 * - added New BSD license statement
 * - removed tabs
 *
 * Revision 1.9  2001/01/15 01:30:04  vitus
 * - VMissing: implemented queryState()
 *
 * Revision 1.8  2000/08/15 00:13:27  vitus
 * - changed addChild() prototype
 * - added some aasert's on methods which can never be called
 *
 * Revision 1.7  2000/08/07 01:30:00  vitus
 * - adding support for rebuild state
 *
 * Revision 1.6  1999/07/05 00:46:13  vitus
 * - addChild() gets additional flag "config sector valid"
 *
 * Revision 1.5  1999/06/21 01:39:14  vitus
 * - corrected english spelling
 *
 * Revision 1.4  1999/06/19 18:28:42  vitus
 * - minor corrections to keep compiler happy
 *
 * Revision 1.3  1999/05/13 22:45:25  vitus
 * - isArray() returns 0xFF to detect this class
 *
 * Revision 1.2  1999/03/08 05:07:42  vitus
 * - added queryIcon() method
 *
 * Revision 1.1  1999/03/04 02:05:00  vitus
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
#define _MISSING_HPP    "$Id: missing.hpp,v 2.1 2005/06/17 06:10:02 vitus Exp $"




class VMissing : public VDriveWithRedundancy {
  private:
    ULONG       size;
    HPOINTER    icon;

  public:
    VMissing(DEVID drive_id,ULONG mysize);
    ~VMissing();

    void        setSize(ULONG newsize) { assert(0); }
    void        setBuild(ULONG nextsec, Boolean redo = False) { assert(0); }
    int         ioRemoveChild(VRDev * child) { assert(0); return 0; }
    int         ioInvalidChild(DEVID const id_of_invalid_child,
                               ULONG from_this_sector_on) { assert(0); return 0; }
    int         ioStartRebuild(ULONG start_sector) { assert(0); return 0; }

    int         ioDeleteYourself() { return 0; }
    int         ioRemoveParent();
    int         ioChangeID(DEVID);
    int         ioSync();

    UCHAR       isArray() { return 0xFF; }
    Boolean     isHostdrive() { return False; }
    char *      allocateDescription();
    HPOINTER    queryIcon();
    ArrayState  queryState() { assert(0); return Error; }
    int         queryChildren() { return 0; }
    VRDev *     getChild(int which) { return NULL; }

    void        setHostdrive(ULONG os2idx = -1UL) { }
    void        addChild(VRDev * newchild, Boolean cfgv,
                         ULONG datav = -1ul) { assert(0); }

    void        forceUnwritable() { }
    Boolean     isWritable() { return True; }
    ULONG       querySize();
    APIRET      read(ULONG offset,ULONG count,PVOID buffer);
    APIRET      write(ULONG offset,ULONG count,PVOID buffer);
};

