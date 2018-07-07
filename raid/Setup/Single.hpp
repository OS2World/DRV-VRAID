/*
 * $Source: R:/Source/driver/raid/Setup/RCS/Single.hpp,v $
 * $Revision: 1.7 $
 * $Date: 2000/08/15 00:15:40 $
 * $Locker:  $
 *
 *	Declaration of class VSingle.
 *
 * $Log: Single.hpp,v $
 * Revision 1.7  2000/08/15 00:15:40  vitus
 * - changed addChild() prototype
 *
 * Revision 1.6  1999/07/05 00:49:32  vitus
 * - addChild() gets additional flag "config sector valid"
 *
 * Revision 1.5  1999/06/21 01:38:32  vitus
 * - corrected english spelling
 *
 * Revision 1.4  1999/06/19 18:28:27  vitus
 * - minor corrections to keep compiler happy
 *
 * Revision 1.3  1999/06/02 01:44:54  vitus
 * - added inv_icon member (icon if child is invalid in respect to parent)
 *
 * Revision 1.2  1999/03/08 05:07:30  vitus
 * - added queryIcon() method
 *
 * Revision 1.1  1999/03/04 00:22:17  vitus
 * Initial revision
 * ----------------------------------------
 * This code is Copyright Vitus Jensen 1999
 */
#define _SINGLE_HPP	"$Id: Single.hpp,v 1.7 2000/08/15 00:15:40 vitus Exp $"



class VSingle : public VRDrive {
  private:
    Boolean	hostdrive;
    ULONG	os2access;
    VRDev      *child;
    ULONG	size;
    Boolean	writable;
    HPOINTER	inv_icon, rw_icon, ro_icon;

  public:
    VSingle(DEVID drive_id);
    ~VSingle();

    void	forceUnwritable() { writable = False; }

    int		ioDeleteYourself();
    int		ioRemoveParent();
    int		ioChangeID(DEVID newid);
    int		ioSync();

    UCHAR	isArray() { return RDTYPE_SINGLE; }
    Boolean	isHostdrive() { return hostdrive; }
    char *	allocateDescription();
    HPOINTER	queryIcon();
    int		queryChildren();
    VRDev *	getChild(int which);

    void	setHostdrive(ULONG os2idx = -1UL);
    void	addChild(VRDev * newchild,Boolean cfgv, ULONG datav = -1ul);

    Boolean	isWritable() { return writable; }
    ULONG	querySize();
    APIRET	read(ULONG offset,ULONG count,PVOID buffer);
    APIRET	write(ULONG offset,ULONG count,PVOID buffer);
};

