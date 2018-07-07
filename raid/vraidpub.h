/*
 * $Source: R:/source/driver/raid/RCS\\vraidpub.h,v $
 * $Revision: 1.10 $
 * $Date: 2000/08/22 00:22:31 $
 * $Locker:  $
 *
 *	Describes IOCtl interface to vraid.flt
 *
 * $Log: vraidpub.h,v $
 * Revision 1.10  2000/08/22 00:22:31  vitus
 * - defined VRAID_SET_CHILD_STATE, VRAID_START_REBUILD
 *
 * Revision 1.9  2000/06/15 01:09:10  vitus
 * - defined VRAID_VERIFY_ARRAY
 * - expanded VRAID_ARRAYINFO_DATA to include flag and children count
 *
 * Revision 1.8  1999/10/01 00:49:23  vitus
 * - expanded VRAID_PHYSINFO_DATA to include request/busy counts
 *
 * Revision 1.7  1999/06/20 17:32:36  vitus
 * - defined VRAID_SET_BEEPER
 * - expanded VRAID_VER_DATA to include flags
 *
 * Revision 1.6  1999/05/26 00:50:36  vitus
 * - VRAID_VER_DATA allows 'configuration sectors modified' detection
 *
 * Revision 1.5  1999/04/30 03:19:50  vitus
 * - defines VRAID_START_SETUP and VRAID_END_SETUP
 *
 * Revision 1.4  1998/12/13 22:31:13  vitus
 * - defined VRAID_QUERY_ARRAYLIST and VRAID_QUERY_ARRAYINFO
 *
 * Revision 1.3  1998/09/27 16:13:07  vitus
 * - added write/read flag to VRAID_IO_PARM
 *
 * Revision 1.2  1998/09/20 15:15:20  vitus
 * - added VRAID_QUERY_PHYSLIST (return list of phys. devices)
 * - added VRAID_QUERY_PHYSINFO (return info about above)
 * - added VRAID_ADMINSEC_IO (read/write administrative sectors)
 *
 * Revision 1.1  1998/03/10 02:42:47  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1997-2000
 */
#if !defined(_VRAIDPUB_H)
#define _VRAIDPUB_H
#pragma pack(1)					/* all structures packed */


/*
 * IOCtl category and function codes
 */
#define IOCTL_VRAID_CATEGORY	0xC0		/* a user-defined category */

#define VRAID_CLEAR_MSGS	0x43		/* forget all messages */

#define VRAID_ADMINSEC_IO	0x44		/* read/write administrative
						   sectors to PHYS device */
#define VRAID_SET_CHILD_STATE	0x48
#define VRAID_END_SETUP		0x49		/* start build, etc. */
#define VRAID_SET_BEEPER	0x4A		/* enable/disable beeper (USHORT) */
#define VRAID_START_REBUILD	0x4B		/* force array to build */

#define VRAID_QUERY_VERSION	0x60		/* returns version and stati */
#define VRAID_READ_MSGS		0x63		/* read startup messages */
#define VRAID_QUERY_PHYSLIST	0x65		/* return list of devices (PHYS) */
#define VRAID_QUERY_PHYSINFO	0x66		/* return info about above */

#define VRAID_QUERY_ARRAYLIST	0x67		/* return list of arrays */
#define VRAID_QUERY_ARRAYINFO	0x68		/* return info about above */
#define VRAID_START_SETUP	0x69		/* end build, etc. */

#define VRAID_VERIFY_ARRAY	0x6B		/* verifies that reduncy informations
						   are still correct */




/*
 * Describes installed VRAID.flt
 */
typedef struct _VRAID_VER_DATA {
    USHORT	version;			/* as in PC-DOS */
    USHORT	flags;				/* Bit0:3 registration level
						   Bit 4  configuration modified
						   Bit 5  beeper disabled */
    USHORT	builds;				/* active build processes */
} VRAID_VER_DATA, NEAR * NPVRAID_VER_DATA, FAR * PVRAID_VER_DATA;



/* Returned by VRAID_READ_MSGS
 * 'msg' will contain a buffer filled with ASCIIZ strings.  Strings
 * usually don't contain newlines.
 * Some time in the future this buffer might even contain messages
 * added after system startup.
 * To clear the buffer just issue VRAID_CLEAR_MSGS w/o parameter and
 * data buffer.
 */
typedef struct _VRAID_MSGS_DATA {
    USHORT	cb;				/* byte count driver wants
						   to return */
    char	msg[1];				/* some compiler don't
						   support '[]' */
} VRAID_MSGS_DATA, NEAR * NPVRAID_MSGS_DATA, FAR * PVRAID_MSGS_DATA;




/* VRAID_QUERY_PHYSLIST
 *
 * Return list of hd values of all PHYSDEV structures VRAID knows of.
 * Param: (none)
 * Data: VRAID_PHYSLIST_DATA
 */
typedef struct _VRAID_PHYSLIST_DATA {
    USHORT	cb;				/* byte count driver wants
						   to return */
    USHORT	hd[1];				/* some compiler don't
						   support '[]' */
} VRAID_PHYSLIST_DATA, NEAR * NPVRAID_PHYSLIST_DATA, FAR * PVRAID_PHYSLIST_DATA;




/* VRAID_QUERY_PHYSINFO
 *
 * Return information about a single PHYSDEV.
 * Param: USHORT handle (from call to VRAID_QUERY_PHYSLIST)
 * Data: VRAID_PHYSINFO_DATA
 */
typedef struct _VRAID_PHYSINFO_DATA {
    USHORT	cb;				/* byte count driver wants
						   to return */
    UCHAR	adapter, target, lun;		/* LUN only on SCSI devices */
    ULONG	partsize, totalsize;		/* [sectors] */

    USHORT	reqcnt, busycnt;		/* free running counters */

} VRAID_PHYSINFO_DATA, NEAR * NPVRAID_PHYSINFO_DATA, FAR * PVRAID_PHYSINFO_DATA;




/* Structures to execute VRAID_ADMINSEC_IO.
 *
 * Pass a filled VRAID_IO_PARM structure to the IOCtl in the parameter packet
 * and let the data packet contain the buffer to read to/write from.
 * I/O is done in units of 512 bytes (one sector), only inside the
 * administrative sector range.  This is not intended as regular I/O interface!
 * Param: VRAID_IO_PARM
 * Data: data to read/write.
 */
typedef struct _VRAID_IO_PARM {
    USHORT	hd;				/* handle (VRAID_QUERY_PHYSLIST) */
    UCHAR	flags;
    UCHAR	reserved;
    ULONG	partsecno;			/* 0 - sizeof(admin. sectors)-1 */

} VRAID_IO_PARM, NEAR * NPVRAID_IO_PARM, FAR * PVRAID_IO_PARM;

#define VIOF_WRITE	0x01




/* VRAID_QUERY_ARRAYLIST
 *
 * Return list of hd values of all ARRAY DRIVEs VRAID knows of.
 * Param: (none)
 * Data: VRAID_ARRAYLIST_DATA
 */
typedef struct _VRAID_ARRAYLIST_DATA {
    USHORT	cb;				/* byte count driver wants
						   to return */
    USHORT	hd[1];				/* some compiler don't
						   support '[]' */
} VRAID_ARRAYLIST_DATA, NEAR * NPVRAID_ARRAYLIST_DATA, FAR * PVRAID_ARRAYLIST_DATA;




/* VRAID_QUERY_ARRAYINFO
 *
 * Return information about a single ARRAY DRIVE.
 *
 * Param: USHORT handle (from call to VRAID_QUERY_ARRAYLIST)
 * Data: VRAID_ARRAYINFO_DATA
 */
typedef struct _VRAID_ARRAYINFO_DATA {
    USHORT	cb;				/* byte count driver wants
						   to return */
    DEVID	id;				/* marks any device */
    USHORT	reqcnt;				/* free running */
    ULONG	size;				/* [sectors] */

    /* ... starting with vraid.flt v0.83 ... */

    USHORT	children;			/* count of children */
    UCHAR	flags;				/* RDFLAG_* */
    UCHAR	reserved;			/* 0 */

} VRAID_ARRAYINFO_DATA, NEAR * NPVRAID_ARRAYINFO_DATA, FAR * PVRAID_ARRAYINFO_DATA;




/* VRAID_SET_CHILD_STATE
 *
 * Modify the idea an array has about one of it's children.
 *
 * Param: VRAID_CHILD_PARAM
 * Data: (nothing)
 */
typedef struct _VRAID_CHILD_PARAM {
    USHORT	handle;

    DEVID	id;				/* ID of child to modify */
    UCHAR	flags;				/* bit 0: completely valid */
    ULONG	secno;				/* if not valid: from where */

} VRAID_CHILD_PARAM, NEAR * NPVRAID_CHILD_PARAM, FAR * PVRAID_CHILD_PARAM;




/* VRAID_VERIFY_ARRAY
 *
 * Instructs vraid.flt to read from all children for the indicated user
 * sector range.  vraid.flt will check whether any redundancy informations
 * are still correct and update the status marks.
 * As the supplied buffer is used for all data transfers and this buffer is
 * returned to the caller any programm may do special analysis.
 *
 * Param: VRAID_VERIFY_PARM
 * Data: VRAID_ARRAYINFO_DATA following a VRAID_VERIFY_DATA for every child.
 */
typedef struct _VRAID_VERIFY_PARM {
    USHORT	handle;

    ULONG	secno;
    USHORT	seccnt;
} VRAID_VERIFY_PARM, NEAR * NPVRAID_VERIFY_PARM, FAR * PVRAID_VERIFY_PARM;

typedef struct _VRAID_VERIFY_DATA {
    DEVID	id;				/* DEVID of child */
    USHORT	bcnt;				/* byte count in data[1] */
    UCHAR	flags;
    UCHAR	reserved;
    ULONG	valid;				/* set to -1ul if the whole child
						   is valid, set to a sector no
						   if only that part is valid */
    UCHAR	data[1];			/* data as read from child */

} VRAID_VERIFY_DATA, NEAR * NPVRAID_VERIFY_DATA, FAR * PVRAID_VERIFY_DATA;

#define VVDF_CONFIRMED	0x01




/* VRAID_START_REBUILD
 *
 * Instructs vraid.flt start a rebuild process for the passed array handle.
 *
 * Param: VRAID_REBUILD_PARM
 * Data: VRAID_ARRAYINFO_DATA
 */
typedef struct _VRAID_REBUILD_PARM {
    USHORT	handle;

    ULONG	secno;
} VRAID_REBUILD_PARM, NEAR * NPVRAID_REBUILD_PARM, FAR * PVRAID_REBUILD_PARM;





#pragma pack()
#endif /* _VRAIDPUB_H */
