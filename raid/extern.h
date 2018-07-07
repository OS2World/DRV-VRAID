/*
 * $Source: R:/source/driver/raid/RCS\\extern.h,v $
 * $Revision: 2.4 $
 * $Date: 2001/07/22 00:30:46 $
 * $Locker:  $
 *
 *	Data extern declarations
 *	(keep in sync with data.c)
 *
 * History:
 * $Log: extern.h,v $
 * Revision 2.4  2001/07/22 00:30:46  vitus
 * - added hdBuildHook, another context hook handle
 *
 * Revision 2.3  2001/06/25 00:29:14  vitus
 * - added hdMemHook, handle of context hook
 *
 * Revision 2.2  2000/04/08 17:36:49  vitus
 * - added DF_NOROUTER, bit of fDriverFlags to disable concurrent access
 *
 * Revision 2.1  1999/09/23 23:08:05  vitus
 * - added apBDisk[], cBDisk, holds BASEDISKs
 *
 * Revision 1.11  1999/06/20 17:36:10  vitus
 * - added DF_BEEPDISABLED, bit of fDriverFlags
 *
 * Revision 1.10  1999/06/19 00:28:26  vitus
 * - ein paar statistische Daten mehr
 *
 * Revision 1.9  1999/05/26 01:15:11  vitus
 * - some initialization messages now located in data.c.  They may be usefull for
 *   all modules!
 *
 * Revision 1.8  1999/05/12 01:41:29  vitus
 * - added UpdateParent and vrioUpdate for rewriting configuration sectors
 * - more fDriverFlags
 *
 * Revision 1.7  1999/04/21 00:27:33  vitus
 * - added BuildParent and vrioBuild for build processes
 *
 * Revision 1.6  1999/01/22 02:52:50  vitus
 * - added aBuildProcess[], cBuildProcess, cBeeps
 *
 * Revision 1.5  1998/12/22 00:03:36  vitus
 * - added 'uRegLevel', used instead of flag inside 'fDriverFlags'
 *
 * Revision 1.4  1998/09/20 15:20:15  vitus
 * - added uMaxQueueCount
 *
 * Revision 1.3  1998/06/26 22:12:16  vitus
 * - corrected apPDev[]
 * - history at top of file
 *
 * Revision 1.2  1998/05/29 01:40:36  vitus
 * - corrected ppMemPool to ppWorkMem
 *
 * Revision 1.1  1998/03/10 02:32:03  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2001
 */

extern char		szDriverName[];
extern DDHDR		DiskDDHeader;		/* device header, located
						   at start of first segment*/
extern ULONG		ppDiskDDHeader;		/* physical address of above */
extern ULONG		ppWorkMem;		/* of Alloc* region */

extern PFN		Device_Help;
extern struct InfoSegGDT FAR	*pGlobalInfoSeg;

extern ULONG		hdMemHook;
extern ULONG		hdBuildHook;
extern USHORT		hdThisDriver;

extern PBASEDISK	apBDisk[];
extern USHORT		cBDisk;
extern PVRDEVHDR	apPDev[];
extern USHORT		cPDev;
extern PHOSTDRIVE	apHDrv[];
extern USHORT		cHDrv;

extern BUILDPROCESS	aBuildProcess[MAX_PDEV/2];
extern USHORT		cBuildProcess;

extern VRDEVHDR		BuildParent;
extern VRIO		vrioBuild;
extern VRDEVHDR		UpdateParent;
extern VRIO		vrioUpdate;

extern ULONG		ulTimer;
extern USHORT		cBeeps;

extern USHORT		fDriverFlags;
#define DF_VERBOSE	0x0001
#define DF_DEBUG	0x0002
#define DF_NOROUTER	0x0004			/* don't share with OS/2 */
#define DF_SETUPACTIVE	0x0010
#define DF_SETUPWRITTEN	0x0020			/* somebody has written via setup */
#define DF_BEEPDISABLED	0x0040			/* disabled via VSetup */
#define DF_INIT1DONE	0x0100
#define DF_INIT2DONE	0x0200
#define DF_SHUTDOWN1	0x0400			/* Shutdown started, end builds */
#define DF_SHUTDOWN2	0x0800			/* Shutdown ended, save state */

extern USHORT		uRegLevel;		/* 0: not registered user,
						   1: up to RAID0
						   2: up to RAID1
						   3: up to RAID4+5 */

extern USHORT		fADDFlags;
#define AF_NOT16M	0x0001			/* don't supports memory >16MB */
#define AF_HWSCATGAT	0x0002			/* all support Sc/Ga in hw */

extern USHORT	uADDMaxSGList;			/* minimum of all ADDs */
extern ULONG	ulADDMaxTransfer;		/* --- "" --- */

extern USHORT	uMaxQueueCount;			/* minimum of all physical d. */


/* Statistical Data: */

extern USHORT	cIorbInQueue;			/* IORBs from OS/2 */
extern USHORT	cVrioInQueue;			/* VRIOs queued internally */
extern USHORT	cIorbAllocated;			/* allocated to do I/O */
extern USHORT	cVrioAllocated;			/* allocated to do I/O */



/* Initialization Data: */

extern NPUCHAR	npInitData;			/* allocate memory here */


extern UCHAR	DeviceTable[MAX_DEVICETABLE_SIZE];

extern UCHAR	szMsgBuffer[];		/* put ASCII messages in this buffer */
extern MSGTABLE msgBuffer;		/* and use this to display */



extern char _ininit	szMallocFailed[];
extern char _ininit	szReadError[];
