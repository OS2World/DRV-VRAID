/*
 * $Source: R:/source/driver/sleep/RCS/extern.h,v $
 * $Revision: 1.8 $
 * $Date: 2001/02/27 02:25:33 $
 * $Author: vitus $
 *
 * Data extern declarations
 * (keep in sync with data.c)
 *
 * $Log: extern.h,v $
 * Revision 1.8  2001/02/27 02:25:33  vitus
 * - added DF_NOAPM flag
 * - added fAPMSuspended
 *
 * Revision 1.7  2001/02/16 02:11:03  vitus
 * - DF_INITCOMPLETE to detect second INIT_COMPLETE IORB
 *
 * Revision 1.6  1997/09/03 01:08:17  vitus
 * Added DF_SHUTDOWN
 *
 * Revision 1.5  1997/06/03 23:30:15  vitus
 * Removed syntax error
 *
 * Revision 1.4  1997/05/07 23:49:27  vitus
 * Added fDriverFlags 'register driver done'
 *
 * Revision 1.3  1996/11/04 00:11:11  vitus
 * Removed global timeout value
 *
 * Revision 1.2  1996/10/23 23:44:42  vitus
 * Added/changed bit definitions of fDriverFlags
 *
 * Revision 1.1  1996/09/27 03:36:20  vitus
 * Initial revision
 * ------------------------------------------------
 * This code is Copyright Vitus Jensen 1996-97,2001
 */


extern PFN		Device_Help;
extern struct InfoSegGDT FAR	*pGlobalInfoSeg;

extern ULONG		ulTimer;

extern USHORT		hdThisDriver;

extern USHORT		cUnit;
extern NPDEVICE		anpUnit[];

extern USHORT		fDriverFlags;
#define DF_VERBOSE	0x0001
#define DF_DEBUG	0x0002
#define DF_TESTOP	0x0004			/* start/stop during install */
#define DF_SHUTDOWN	0x0008			/* don't stop after shutdown */
#define DF_NOAPM	0x0010			/* don't register via APM */
#define DF_REGISTERED	0x0080
#define DF_INITDONE	0x0100
#define DF_INITCOMPLETE	0x0200			/* IOCM_COMPLETE_INIT */
#define DF_REGDEVCLASS	0x0400			/* RegisterDeviceClass() done */

extern UCHAR		fAPMSuspended;
extern char		szDriverName[];



/* Initialization Data */

extern NPUCHAR	npInitData;			/* allocate memory here */


extern UCHAR	DeviceTable[MAX_DEVICETABLE_SIZE];

extern UCHAR	szMsgBuffer[];		/* put ASCII messages in this buffer */
extern MSGTABLE msgBuffer;		/* and use this to display */

