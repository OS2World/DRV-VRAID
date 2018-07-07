/*
 * $Source: R:/Source/driver/raid/Setup/RCS/drvbuild.h,v $
 * $Revision: 1.12 $
 * $Date: 2000/05/28 17:39:32 $
 * $Locker:  $
 *
 *	Interface to drive scanning/creation/etc.
 *
 * $Log: drvbuild.h,v $
 * Revision 1.12  2000/05/28 17:39:32  vitus
 * - moved DSKAREA structure to pcreate.h
 *
 * Revision 1.11  2000/03/09 23:17:19  vitus
 * - struct _DSKAREA_ENTRY contains extstart (start of first extended partition).
 *   This is important to know!
 *
 * Revision 1.10  1999/06/21 01:30:08  vitus
 * - corrected english spelling
 *
 * Revision 1.9  1999/04/16 01:03:50  vitus
 * - added LookupDisk(), searches record for specific VRDev
 *
 * Revision 1.8  1999/03/01 02:21:50  vitus
 * - removed DSKINFO structure
 * - any pointer converted to VRDev *
 *
 * Revision 1.7  1999/02/09 02:54:45  vitus
 * - switched to C++
 *
 * Revision 1.6  1998/12/13 22:34:48  vitus
 * - DSKINFO: added access.none substructure
 *
 * Revision 1.5  1998/11/03 03:04:10  vitus
 * - DSKINFO change: 'avail' element changed position
 *
 * Revision 1.4  1998/09/27 00:11:31  vitus
 * - added access.flt and access.none to DSKINFO
 *
 * Revision 1.3  1998/06/09 01:28:39  vitus
 * - switched CHAIN and STRIPE contants
 *
 * Revision 1.2  1998/05/27 01:55:46  vitus
 * - AreaDlgProc() and data structure
 *
 * Revision 1.1  1998/05/18 00:46:15  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2000
 */


#define MAXCHILDREN	32



typedef struct {
    HWND	parent;				/* post devices here */
} THR_DRVSCAN, * PTHR_DRVSCAN;			/* parameter to DrvscanThread*/




typedef struct _MYRECORD {
    MINIRECORDCORE	record;
    VRDev *		dsk;			/* see drvscan.cpp */
} MYRECORD, * PMYRECORD;




/* Passed to DrvtypeDlgProc() */
typedef struct _DRVBUILD {
    ULONG	type;				/* DSKTYPE_*, see drvscan.h */
    ULONG	cnt;
    PMYRECORD	child[MAXCHILDREN];
} DRVBUILD, * PDRVBUILD;




/* ----------------------------------------------------------------------
 * From drvscan.c
 * -------------------------------------------------------------------- */
extern void _Optlink	DrvscanThread(void *dummy);


/* ----------------------------------------------------------------------
 * From drvtype.c
 * -------------------------------------------------------------------- */
extern MRESULT EXPENTRY	DrvtypeDlgProc(HWND hwnd,ULONG msg,
				       MPARAM mp1,MPARAM mp2);


/* ----------------------------------------------------------------------
 * From areaselect.c
 * -------------------------------------------------------------------- */
extern MRESULT EXPENTRY	AreaDlgProc(HWND hwnd,ULONG msg,
				    MPARAM mp1,MPARAM mp2);


/* ----------------------------------------------------------------------
 * From container.c
 * -------------------------------------------------------------------- */

/*
 * PFNSRCH is used by SearchCnr().  The parameters are:
 * - container window handle,
 * - record pointer,
 * - two user-data pointer.
 */
typedef BOOL	(*PFNSRCH)(HWND,PVOID,PVOID,PULONG);

extern BOOL EmphSelect(HWND hwnd,PMYRECORD myrecord,PBOOL pbSelect,PULONG cnt);
extern BOOL SearchSource(HWND hwnd,PMYRECORD myrecord,PBOOL pbSource,PULONG cnt);
extern BOOL EmphInuse2Source(HWND hwnd,PMYRECORD myrecord,PBOOL pbSource,PULONG cnt);
extern BOOL EmphInuse(HWND hwnd,PMYRECORD myrecord,PBOOL pbSource,PULONG cnt);
extern BOOL EmphSource(HWND hwnd,PMYRECORD myrecord,PBOOL pbSource,PULONG cnt);
extern BOOL QueryInuse(HWND hwnd,PMYRECORD myrecord,PULONG cnt,PMYRECORD record_array[]);
extern BOOL QueryAny(HWND hwnd,PMYRECORD myrecord,PULONG cnt,PMYRECORD record_array[]);
extern BOOL LookupDisk(HWND hwnd,PMYRECORD myrecord,VRDev * disk,PMYRECORD * record_returned);

extern PVOID SearchCnr(HWND hwndCnr,PFNSRCH pfnSearch,PVOID pvUser1,PVOID pvUser2,PVOID pvRsrvd);

