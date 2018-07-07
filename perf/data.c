/*
 * $Source: e:/source/driver/perf/RCS/data.c,v $
 * $Revision: 1.4 $
 * $Date: 1997/12/16 02:19:46 $
 * $Locker:  $
 *
 * Declares all data used in this driver.
 *
 * $Log: data.c,v $
 * Revision 1.4  1997/12/16 02:19:46  vitus
 * - removed szDriverName (driver uses definition from startup.asm)
 *
 * Revision 1.3  1997/12/11 01:45:52  vitus
 * - initialies ppDiskDDHeader - moved from BBS to DATA segment
 *
 * Revision 1.2  1997/12/06 00:45:03  vitus
 * - enabled support for WatCom C
 *
 * Revision 1.1  1997/10/17 00:07:09  vitus
 * Initial revision
 * --------------------------------------------
 * Author: Vitus Jensen, 2:2474/424, 1997
 */
#pragma off (unreferenced)
static char vcid[]="$Id: data.c,v 1.4 1997/12/16 02:19:46 vitus Exp $";
#pragma on (unreferenced)


#define INCL_NOBASEAPI
#define INCL_NOPMAPI
#include <os2.h>

#include <devcmd.h>

#define INCL_INITRP_ONLY
#include <reqpkt.h>

#include <scb.h>
#include <abios.h>

#include <iorb.h>
#include <addcalls.h>

#if defined(__WATCOMC__)
# include <devhelp.h>
typedef USHORT NEAR *	NPUSHORT;
typedef VOID NEAR *	NPVOID;
#else
# include <dhcalls.h>
#endif

#include "scsiopt.h"
#include "proto.h"
#include "extern.h"



/*
 * Static Data
 */

ULONG		ppDiskDDHeader = 0;
PFN		Device_Help =	0;
struct InfoSegGDT FAR	*pGlobalInfoSeg = 0;

USHORT		fDriverFlags =	0;
USHORT		hdThisDriver =	0;



/*
 * Initialization Data
 */

UCHAR		MemPool[30*1024] =	{0};
NPUCHAR		npInitData =		MemPool;
BYTE    	DeviceTable[MAX_DEVICETABLE_SIZE] = {0};
SCATGATENTRY	ScGaEntry = {0};


/*
 * Message generated by this driver.  May
 * be discarded after INIT.
 */
#define MSG_REPLACEMENT_STRING	1178		/* %1 %2 %3 ... */

UCHAR	szMsgBuffer[1024]		= {0};

MSGTABLE msgBuffer = {
    MSG_REPLACEMENT_STRING, 1, (PBYTE)szMsgBuffer 
};

