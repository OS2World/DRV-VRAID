/*
 * $Source: R:/Source/driver/raid/Setup/RCS/pcreate.h,v $
 * $Revision: 1.1 $
 * $Date: 2000/05/28 17:26:53 $
 * $Locker:  $
 *
 *	Interface to pcreate.cpp
 *
 * $Log: pcreate.h,v $
 * Revision 1.1  2000/05/28 17:26:53  vitus
 * Initial revision
 *
 * ----------------------------------------
 * This code is Copyright Vitus Jensen 2000
 */



/* Passed to AreaDlgProc(), too. */

struct _DSKAREA_ENTRY {
    ULONG	start;				/* start of free area */
    ULONG	size;				/* size of free area */

    UCHAR	type;
    UCHAR	bootable;

    ULONG	ptsec;				/* sector no of partition table
						   where this area is left free.
						   This (and perhaps another) table
						   has to be modified. */
    ULONG	extstart;			/* start of first extended partition,
						   needed for relative addressing. */
};

typedef struct _DSKAREA {
    ULONG		  cnt;
    struct _DSKAREA_ENTRY area[10];
} DSKAREA, * PDSKAREA;




extern APIRET	FindFreeSpace(VOS2Disk * const disk, PDSKAREA const info,
			      int * const found);

extern APIRET	CreatePriPartition(VOS2Disk * const disk, ULONG const ptsec,
				   struct _DSKAREA_ENTRY * const vpart);
extern APIRET	CreateLogPartition(VOS2Disk * const disk, ULONG const ptsec,
				   struct _DSKAREA_ENTRY * const vpart);
