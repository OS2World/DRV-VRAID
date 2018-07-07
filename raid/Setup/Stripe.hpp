/*
 * $Source: R:/Source/driver/raid/Setup/RCS/Stripe.hpp,v $
 * $Revision: 1.8 $
 * $Date: 2000/08/15 00:16:00 $
 * $Locker:  $
 *
 *	Declaration of class VStripe.
 *
 * $Log: Stripe.hpp,v $
 * Revision 1.8  2000/08/15 00:16:00  vitus
 * - changed addChild() prototype
 *
 * Revision 1.7  1999/07/05 00:47:14  vitus
 * - addChild() gets additional flag "config sector valid", saved in child array
 *
 * Revision 1.6  1999/06/21 01:31:51  vitus
 * - corrected english spelling
 *
 * Revision 1.5  1999/06/19 18:28:16  vitus
 * - minor corrections to keep compiler happy
 *
 * Revision 1.4  1999/06/02 01:44:34  vitus
 * - added inv_icon member (icon if child is invalid in respect to parent)
 *
 * Revision 1.3  1999/04/13 01:25:44  vitus
 * - child[] now static member of object
 *
 * Revision 1.2  1999/03/08 05:07:16  vitus
 * - added queryIcon() method
 *
 * Revision 1.1  1999/03/04 00:22:29  vitus
 * Initial revision
 * ----------------------------------------
 * This code is Copyright Vitus Jensen 1999
 */
#define _STRIPE_HPP	"$Id: Stripe.hpp,v 1.8 2000/08/15 00:16:00 vitus Exp $"




class VStripe : public VRDrive {
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
    VStripe(DEVID drive_id,int nchd);
    ~VStripe();

    void	forceUnwritable() { writable = False; }

    int		ioDeleteYourself();
    int		ioRemoveParent();
    int		ioChangeID(DEVID);
    int		ioSync();

    UCHAR	isArray() { return RDTYPE_STRIPE; }
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
