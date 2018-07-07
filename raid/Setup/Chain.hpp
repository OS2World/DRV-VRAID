/*
 * $Source: R:/Source/driver/raid/Setup/RCS/Chain.hpp,v $
 * $Revision: 1.8 $
 * $Date: 2000/08/15 00:16:22 $
 * $Locker:  $
 *
 *	Declaration of class VChain.
 *
 * $Log: Chain.hpp,v $
 * Revision 1.8  2000/08/15 00:16:22  vitus
 * - changed addChild() prototype
 *
 * Revision 1.7  1999/07/05 00:49:14  vitus
 * - addChild() gets additional flag "config sector valid", saved in child array
 *
 * Revision 1.6  1999/06/21 01:34:01  vitus
 * - corrected english spelling
 *
 * Revision 1.5  1999/06/19 18:27:58  vitus
 * - minor corrections to keep compiler happy
 *
 * Revision 1.4  1999/06/02 01:45:12  vitus
 * - added inv_icon member (icon if child is invalid in respect to parent)
 *
 * Revision 1.3  1999/04/13 01:26:48  vitus
 * - child[] now static member of object
 *
 * Revision 1.2  1999/03/08 05:08:16  vitus
 * - added queryIcon() method
 *
 * Revision 1.1  1999/03/04 00:22:02  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1999-2000
 */
#define _CHAIN_HPP	"$Id: Chain.hpp,v 1.8 2000/08/15 00:16:22 vitus Exp $"




class VChain : public VRDrive {
  private:
    Boolean	hostdrive;
    ULONG	os2access;

    int		children;
    struct _ChildInfo {
	VRDev *	rdev;
	Boolean cfgok;
    } child[MAX_CHILDREN];

    ULONG	size;
    Boolean	writable;
    HPOINTER	inv_icon, rw_icon, ro_icon;

  public:
    VChain(DEVID drive_id,int nchd);
    ~VChain();

    void	forceUnwritable() { writable = False; }

    int		ioDeleteYourself();
    int		ioRemoveParent();
    int		ioChangeID(DEVID);
    int		ioSync();

    UCHAR	isArray() { return RDTYPE_CHAIN; }
    Boolean	isHostdrive() { return hostdrive; }
    char *	allocateDescription();
    HPOINTER	queryIcon();
    int		queryChildren();
    VRDev *	getChild(int which);

    void	setHostdrive(ULONG os2idx = -1UL);
    void	addChild(VRDev * newchild, Boolean cfgv, ULONG datav = -1ul);

    Boolean	isWritable() { return writable; }
    ULONG	querySize();
    APIRET	read(ULONG offset,ULONG count,PVOID buffer);
    APIRET	write(ULONG offset,ULONG count,PVOID buffer);
};
