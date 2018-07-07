/*
 * $Source: e:/source/driver/perf/RCS/extern.h,v $
 * $Revision: 1.2 $
 * $Date: 1997/12/08 23:04:14 $
 * $Locker:  $
 *
 * Data extern declarations
 * (keep in sync with data.c)
 *
 * $Log: extern.h,v $
 * Revision 1.2  1997/12/08 23:04:14  vitus
 * - commented
 *
 * Revision 1.1  1997/10/28 02:09:09  vitus
 * Initial revision
 * ----------------------------------------
 * Author: Vitus Jensen, 2:2474/424
 */

extern DDHDR		DiskDDHeader;		/* device header, located
						   at start of first segment*/
extern ULONG		ppDiskDDHeader;		/* physical address of above */

extern PFN		Device_Help;		/* entry to header routines */
extern struct InfoSegGDT FAR	*pGlobalInfoSeg; /* nice to know */

extern USHORT		hdThisDriver;		/* if filter: our handle */

extern USHORT		fDriverFlags;
#define DF_VERBOSE	0x0001			/* logo, some messages */
#define DF_DEBUG	0x0002			/* many messages */
#define DF_KEEP		0x0004			/* keep filter in memory */
#define DF_INITDONE	0x0100			/* INIT stage 1 done, no more
						   screen messages possible */


extern char	szDriverName[];			/* my name "DPERF$" */



/* Initialization Data */

extern NPUCHAR	npInitData;			/* allocate memory here */

extern UCHAR	DeviceTable[MAX_DEVICETABLE_SIZE]; /* if filter: */
extern SCATGATENTRY	ScGaEntry;		/* needed to transfer data */


extern UCHAR	szMsgBuffer[];		/* put ASCII messages in this buffer */
extern MSGTABLE msgBuffer;		/* and use this to display */

