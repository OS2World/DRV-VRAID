/*
 * $Source: R:/source/driver/raid/RCS\\dsklayout.h,v $
 * $Revision: 1.11 $
 * $Date: 2001/07/20 01:14:21 $
 * $Locker:  $
 *
 *	Describes layout of administrative disk sectors.
 *
 * $Log: dsklayout.h,v $
 * Revision 1.11  2001/07/20 01:14:21  vitus
 * - added RDFLAG_STATEMASK, mask all state bits
 *
 * Revision 1.10  2000/07/24 23:54:18  vitus
 * - struct _SEC_VRDEV2 now supports rebuild state
 *
 * Revision 1.9  2000/03/17 02:07:23  vitus
 * - added EXTENDED_PARTTYPE
 *
 * Revision 1.8  1999/06/21 02:34:17  vitus
 * - corrected my english (spelling)
 *
 * Revision 1.7  1999/05/12 01:38:06  vitus
 * - SEC_VRDEV2: new layout
 * - moved RDFLAGS_* from driver specific header
 *
 * Revision 1.6  1999/01/22 02:54:08  vitus
 * - added VRAIDEVICE2 sectors (preliminary)
 *
 * Revision 1.5  1998/12/22 00:03:10  vitus
 * - added missing "#pragma pack()".  Silly bug.
 *
 * Revision 1.4  1998/06/26 22:11:22  vitus
 * - removed flags from SEC_PHYSDEV
 *
 * Revision 1.3  1998/05/29 01:39:53  vitus
 * - changed definition of STRIPE,CHAIN
 * - SINGLE is now valid type of VRAIDDEVICE sector
 *
 * Revision 1.2  1998/04/08 01:05:01  vitus
 * - changed unnamed to named structures (IBM Compiler)
 * - added size to child definition
 * - removed bitfield usage (probs with different compilers)
 *
 * Revision 1.1  1998/03/10 02:25:09  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1997-2001
 */

#ifndef _DSKSLAYOUT_H
#define _DSKSLAYOUT_H
#pragma pack(1)

#define SECTOR_SIZE		512


/*
 * Absolute sector 0 contains a standard partition table.
 * A single partition is defined starting at absolute sector 1
 * and including the rest of physical disk space.
 */
#define VRAID_PARTTYPE		0x7C
#if !defined(EXTENDED_PARTTYPE)
# define EXTENDED_PARTTYPE	0x05
#endif


/*
 * Relative sectors 1 to n in partition are used for administrative data.
 * May become variable in non-alpha versions...
 */
#define VRAID_ADMINSECTORS	32


/*
 * Layout of master boot record sector,
 * imported from OS2DASD header file.
 */
typedef struct _PARTITIONENTRY {
    UCHAR	BootIndicator;
    UCHAR	BegHead;
    UCHAR	BegSector;			/* plus 2 bits BegCylinder */
    UCHAR	BegCylinder;
    UCHAR	SysIndicator;
    UCHAR	EndHead;
    UCHAR	EndSector;			/* plus 2 bits EndCylinder */
    UCHAR	EndCylinder;
    ULONG	RelativeSectors;
    ULONG	NumSectors;
} PARTITIONENTRY, FAR * PPARTITIONENTRY;


typedef struct _MBR {
    UCHAR	Pad[0x1BE];
    PARTITIONENTRY PartitionTable[4];
    USHORT	Signature;
} MBR, FAR * PMBR;




/*
 * All devices are marked with an unique ID.
 * This could be build from two byte host ID (sum of BIOS image) and
 * a time stamp (UNIX time = seconds since 1.1.1970) at creation of device.
 */
typedef UCHAR		DEVID[6];
typedef DEVID FAR *	PDEVID;


/*
 * First configuration sector defines this physical device.  Currently
 * only an ID is defined.
 * Sector type: "PHYSDEVICE      "
 */
typedef struct _SEC_PHYSDEV {
    UCHAR	sectype[16];			/* R: type of sector */
    ULONG	timestamp;			/* R: when written */
    union {
	char		dummy[490];
	struct {
	    DEVID	id;
	    USHORT	adminspace;		/* will be VRAID_ADMINSECTORS */
	    ULONG	size;			/* user size [sectors]} */
	} s;
    } u;
    USHORT		crc;			/* CRC16 */
} SEC_PHYSDEV, FAR * PSEC_PHYSDEV;




/*
 * Second and all further configuration sectors define
 * the VRDEVICES build upon this disk.
 * Sector type: "VRAIDDEVICE     ":
 */
typedef struct _SEC_VRDEV {
    UCHAR	sectype[16];			/* R: type of sector */
    ULONG	timestamp;			/* R: when written */
    union {
	char		dummy[490];
	struct {
	    DEVID	id;
	    UCHAR	type;			/* RDTYPE_* */
	    UCHAR	flags;			/* RDFLAG_* */
	    USHORT	children;		/* entries in following table */
	    struct {
		DEVID	id;
		ULONG	size;			/* [sectors] */
	    } child[1];
	} s;
    } u;
    USHORT		crc;			/* CRC16 */
} SEC_VRDEV, FAR * PSEC_VRDEV;


/* Sector type: "VRAIDDEVICE2    " */
typedef struct _SEC_VRDEV2 {
    UCHAR	sectype[16];			/* R: type of sector */
    ULONG	timestamp;			/* R: when written */
    union {
	char		dummy[486];
	struct {
	    DEVID	id;
	    UCHAR	type;			/* RDTYPE_* */
	    UCHAR	flags;			/* RDFLAG_* */
	    USHORT	children;		/* entries in following table */
	    struct {
		DEVID	id;
		UCHAR	flags;			/* bit 0: completely valid child */
		UCHAR	reserved;		/* to be defined */
		ULONG	valid;			/* valid sector range
						   (flags & 0x01) != 0 -> Build runs
						   (flags & 0x01) == 0 -> Rebuild */
		ULONG	size;			/* [sectors] */
	    } child[1];
	} s;
    } u;

    ULONG	rebuild;			/* restart rebuild here [sector] */
    USHORT	crc;				/* CRC16 */
} SEC_VRDEV2, FAR * PSEC_VRDEV2;

#define RDTYPE_SINGLE	1
#define RDTYPE_CHAIN	2
#define RDTYPE_STRIPE	3
#define RDTYPE_MIRROR	4
#define RDTYPE_RAID4	5
#define RDTYPE_RAID5	6


#define RDFLAG_STATEMASK	0x0F		/* reserved for array states */

#define RDFLAG_BUILDING		0x01		/* build running */
#define RDFLAG_ERROR		0x02		/* array in ERROR state,
						   no more I/Os possible */
#define RDFLAG_FAIL		0x04		/* redundancy lost */
#define RDFLAG_REBUILD		0x08		/* rebuild running */

#define RDFLAG_HOSTDRIVE	0x80		/* a hostdrive, stop scanning */


#pragma pack()
#endif /* _DSKSLAYOUT_H */
