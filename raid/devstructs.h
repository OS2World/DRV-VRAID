/*
 * $Source: R:/source/driver/raid/RCS\\devstructs.h,v $
 * $Revision: 2.4 $
 * $Date: 2001/06/25 00:30:11 $
 * $Locker:  $
 *
 *
 *	Layout of device structures used at runtime.
 *
 *
 * $Log: devstructs.h,v $
 * Revision 2.4  2001/06/25 00:30:11  vitus
 * - BUILDPROCESS: added lin_buffer, builds now use a seperate buffer
 *
 * Revision 2.3  2001/02/11 01:18:45  vitus
 * - VRDEVHDR: changed ioctl() method to return DRVSTATUS
 *
 * Revision 2.2  2000/08/29 01:08:08  vitus
 * - defined more VRDIOCTL_* codes
 *
 * Revision 2.1  2000/06/15 01:11:25  vitus
 * - added 'ioctl' method to VRDEVHDR and defined some codes
 *
 * Revision 1.11  1999/10/01 00:52:16  vitus
 * - VRDEVHDR includes iocnt and busycnt fields
 *
 * Revision 1.10  1999/09/18 14:33:27  vitus
 * - added BASEDISK definition (router to PHYSDEVICEs)
 * - added macro UH2PPTR() to get device pointer from iorb.UnitHandle
 *
 * Revision 1.9  1999/06/21 02:34:43  vitus
 * - corrected english spelling
 *
 * Revision 1.8  1999/05/12 01:35:50  vitus
 * - removed admin() element of VRDEVHDR, now handled via IOCMD_A*
 * - removed RDFLAGS_*, see dsklayout.h
 * - BUILDPROCESS don't needs VRIO, VRDEVHDR.  Removed.
 *
 * Revision 1.7  1999/02/06 00:45:52  vitus
 * - added 'admin' method to VRDEVHDR
 * - modified VRIO to include all relevant parts of IORB (which isn't
 *   used any more for commands, addresses, etc.)
 *
 * Revision 1.6  1999/01/22 02:52:08  vitus
 * - added definition of BUILDPROCESS: keeps data from processes which rebuild arrays
 * - added RDFLAGS_BUILD, related
 *
 * Revision 1.5  1998/12/13 22:27:01  vitus
 * - removed hdr, size, flags from HOSTDRIVE: we don't need them as a
 *   HOSTDRIVE is used only for a single internal device structure so
 *   we can query that device.
 *
 * Revision 1.4  1998/06/26 22:10:31  vitus
 * - corrected worker,notify prototypes (MS-C wonders)
 *
 * Revision 1.3  1998/04/08 01:02:03  vitus
 * - added VRDEVHDR to HOSTDRIVE structure
 * - renamed father I/O to mother
 *
 * Revision 1.2  1998/03/10 02:30:00  vitus
 * - changed VRDEVICE to VRDEVHDR structure (modules define their structure)
 *
 * Revision 1.1  1998/03/06 01:27:21  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2001
 */

/* Forward declarations */

typedef struct _VRIO FAR *	PVRIO;
typedef struct _VRDEVHDR FAR *	PVRDEVHDR;



/*
 * Runtime configuration is done with list/tables/parent-child of
 * structures.  An I/O enters at HOSTDRIVE which has a pointer to
 * the first VRDEVICE which has pointers to child which build it.  And
 * those children have pointers, etc.
 *
 * Phys,mirror,spanning,stripe-VRDEVICES are all different
 * but start with this header.
 *
 * The difference between 'worker' and 'admin' is:
 * - 'admin' allows access to the configuration sectors, in fact it should
 *   only be used for this purpose.  Reading sector 0 gives the configuration
 *   data for the device you are reading from (SEC_PHYSDEV from PYHSDEVICE).
 * - 'worker' only reads sectors from the "data range" (where OS/2 writes
 *   it's data to).
 *
 */
typedef struct _VRDEVHDR {
    DEVID	id;
    UCHAR	flags;				/* RDFLAG_* */
    USHORT	iocnt;				/* counts VRIOs */
    USHORT	busycnt;			/* counts VRIOs not started */
    ULONG	size;				/* [sectors] */

    /* self:	VR device which will be notified ('notify' is method
     *		of 'self'.
     * io:	VR I/O to child of 'self' which completed. */

    void	(NEAR _Cdecl *notify)(PVRIO const io);

    /* self:	VR device which has to process 'io' ('worker' is method
     *		of 'self').
     * io:	VR I/O to 'self' */

    USHORT	(NEAR _Cdecl *worker)(PVRDEVHDR const self,PVRIO const io);

    /* self:	VR device which has to modify 'sector' ('update' is method
     *		of 'self').
     * sector:	'self's definition sector */

    USHORT	(NEAR _Cdecl *update)(PVRDEVHDR const self, PVOID const sector);

    /* Special function ('ioctl') interface.  This method is only called
     * at task time and may block the current thread!
     *
     * self:	VR device which is to query/modify ('ioctl' is method
     *		of 'self').
     * fct:	function code to execute (VRDIOCTL_*)
     * data:	dependend on 'cmd'
     *
     * returns 0 or driver status code (incl. STDON). */

    DRVSTATUS	(NEAR _Cdecl *ioctl)(PVRDEVHDR const self, USHORT const cmd,
				     PVOID const parameter, PVOID const data);


} VRDEVHDR, FAR * PVRDEVHDR;

#define VRDIOCTL_QUERY_CHILDREN	0
#define VRDIOCTL_INFO		1
#define VRDIOCTL_VERIFY		10
#define VRDIOCTL_START_REBUILD	11
#define VRDIOCTL_CHILD_STATE	12



#if 0
/* command codes above 0x8000 are used for internal purposes: */

#define VRDIOCTL_SET_BUILDPOS	0x8000		/* parameter contains pointer to
						   current position of [re]build
						   process (which may be detected
						   by other means from the array
						   itself, it's only a hint). */
#endif




/*
 * A physical device is kind of special.  We need data to access it
 * via ADD and may --- in the future --- save some hardware related
 * settings here.
 */
typedef struct _PHYSDEVICE {
    PVRDEVHDR	parent;

    /* Used to address the ADD: */

    void	(FAR  _Cdecl * pADDEntry)(PIORB);
    USHORT	hdADDUnit;			/* ADD handle of this unit */
    USHORT	hdFilter;			/* previous filter (if any) */

    USHORT	iAdapter;			/* global index */
    USHORT	iUnit;				/* per adapter index */

    /* From MBR and VRAID disk sectors: */

    ULONG	ulVRStart;			/* start of partition */
    ULONG	ulVRSize;			/* size of partition */

} PHYSDEVICE, FAR * PPHYSDEVICE;




/*
 * All internal I/O handling uses VRIO structures.
 */
typedef struct _VRIO {

    PVRDEVHDR	vrd;				/* who processes this I/O? */
    PVRIO	link;				/* temporary pointer
						   and in QueueVrio() */

    /* Data for notification */

    PVOID	iorb;				/* OS/2 request to VRAID.FLT */

    PVRIO	mother;				/* created on behalf of this I/O */
    USHORT	children;			/* how many child I/Os? */
    USHORT	oserr;				/* like in IORB */


    /* Data to generate command to underlying
     * ADD or VRDEVICE from. */

    USHORT	iocmd;

    USHORT	cSGList;			/* count of S/G list elements */
    PSCATGATENTRY pSGList;			/* far ptr to S/G list */
    ULONG	ppSGList;			/* physical address of S/G list */

    USHORT	offset;				/* in memory relative
						   to S/G list start [sector] */
    USHORT	bcnt;				/* how many sectors */
    ULONG	blk;				/* block no on device [sector]
						   worker: relative to data area
						   admin: relative to partition */
} VRIO;


#define IOCMD_READ	0x01			/* read from data area */
#define IOCMD_WRITE	0x02			/* write to data area */

#define IOCMD_AREAD	0x11			/* read from admin sectors */
#define IOCMD_AWRITE	0x12			/* write to admin sectors */




/*
 * This structure contains all information the OS is willing
 * to know or change.
 * I.e. keep modified unitinfo, etc. here.
 */
typedef struct _HOSTDRIVE {

    USHORT	OsFlags;			/* OF_* */
    USHORT	ReqCnt;				/* counts IORBs, IORB chains count
						   as single request */

    PVRDEVHDR	child;

    PUNITINFO	modinfo;			/* modified unit info */

    /* We might need an IORB queue */

    PIORB	pQueueHead;
    PIORB	pQueueFoot;			/* IORB queueing */
    ULONG	hlQueue;			/* 0B: lock handle for above */

} HOSTDRIVE, FAR * PHOSTDRIVE;



typedef struct _BASEDISK {

    USHORT	OsFlags;			/* OF_* */
    UNITINFO	info;
    PPHYSDEVICE	phys;				/* to access ADD etc */

} BASEDISK, FAR * PBASEDISK;



#define OF_ALLOCATED	0x0001			/* OS/2 related (OsFlags) */




/* Translate the UnitHandle passed to/from OS/2 to a pointer.  Pointer
 * points to PHOSTDRIVE of PBASEDISK. */

#define UH2PPTR(uhandle)		((ULONG _based(_segname("_DATA")) *)(uhandle))






typedef struct _BUILDPROCESS {
    PVRDEVHDR		vrd;			/* which to build */
    ULONG		next;			/* next sector to update */
    UCHAR		busy;			/* nonzero if I/O running */
    ULONG		lin_buffer;		/* linear address of buffer
						   allocated for I/Os */
} BUILDPROCESS, FAR *PBUILDPROCESS;
