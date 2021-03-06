/*
 * $Source$
 * $Revision: 1.13 $
 * $Date: 2001/01/15 01:28:47 $
 * $Locker:  $
 *
 *	Declaration of class VRaid5.
 *
 * $Log$
 * ----------------------------------------
 * This code is Copyright Vitus Jensen 2001
 */
#define _RAID5_HPP	"$Id$"





class VRaid5 : public VDriveWithRedundancy {
  private:
    Boolean	hostdrive;
    ULONG	os2access;

    int		children;
    struct _ChildInfo {
	VRDev *	rdev;
	ULONG	valid;
	Boolean cfgok;
    } child[MAX_CHILDREN];

    ULONG	size;
    Boolean	writable;
    HPOINTER	inv_icon, rw_icon, ro_icon;
    ArrayState	state;


  public:
    VRaid5(DEVID drive_id, int nchd);
    ~VRaid5();

    void	setSize(ULONG newsize);
    void	setBuild(ULONG nextsec, Boolean redo = False);
    int		ioRemoveChild(VRDev * child);
    int		ioInvalidChild(DEVID const id_of_invalid_child,
			       ULONG from_this_sector_on);
    int		ioStartRebuild(ULONG start_sector);

    int		ioDeleteYourself();
    int		ioRemoveParent();
    int		ioChangeID(DEVID);
    int		ioSync();

    UCHAR	isArray() { return RDTYPE_RAID5; }
    Boolean	isHostdrive() { return hostdrive; }
    char *	allocateDescription();
    HPOINTER	queryIcon();
    ArrayState	queryState() { return state; }
    int		queryChildren();
    VRDev *	getChild(int which);
    Boolean	isValidChild(int which);
    Boolean	isValidChild(VRDev * which);

    void	setHostdrive(ULONG os2idx = -1UL);
    void	addChild(VRDev * newchild, Boolean cfgv, ULONG datav = -1ul);

    void	forceUnwritable() { writable = False; }
    Boolean	isWritable() { return writable; }
    ULONG	querySize() { return size; }
    APIRET	read(ULONG offset,ULONG count,PVOID buffer);
    APIRET	write(ULONG offset,ULONG count,PVOID buffer);
};

