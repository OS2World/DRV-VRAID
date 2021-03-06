/*
 * $Source: R:/source/driver/sleep/RCS/Data.c,v $
 * $Revision: 1.8 $
 * $Date: 2001/02/27 02:27:56 $
 * $Locker:  $
 *
 * Declares all data used in this driver.  This should be
 * done in a seperate source file to have greater control
 * on segment/data allocation.
 *
 * History:
 * $Log: Data.c,v $
 * Revision 1.8  2001/02/27 02:27:56  vitus
 * - added fAPMSuspended, non-zero when the machine is suspended
 *
 * Revision 1.7  1999/11/29 00:40:33  vitus
 * - moved all system includes to dsksleep.h
 *
 * Revision 1.6  1998/06/28 23:54:41  vitus
 * - removed addcalls.h (not needed, WatCom probs)
 *
 * Revision 1.5  1997/12/05 01:45:48  vitus
 * - added WatCom specific code and header
 * - moved history to bottom of file
 *
 * Revision 1.4  1997/09/03 01:16:44  vitus
 * Included scsi.h (structures now contain SCSI status)
 *
 * Revision 1.3  1997/03/03 01:17:09  vitus
 * Moved message buffer data to resident area
 *
 * Revision 1.2  1996/11/04 00:04:17  vitus
 * Removed global timeout value
 *
 * Revision 1.1  1996/09/26 01:37:41  vitus
 * Initial revision
 * ------------------------------------------------
 * This code is Copyright Vitus Jensen 1996-99,2001
 */
static char id[]="$Id: Data.c,v 1.8 2001/02/27 02:27:56 vitus Exp $";

#define INCL_NOBASEAPI
#define INCL_NOPMAPI
#define INCL_INITRP_ONLY
#include "dsksleep.h"
#include "proto.h"
#include "extern.h"



/*
 * Static Data
 */

PFN		Device_Help =	0;
struct InfoSegGDT FAR	*pGlobalInfoSeg = 0;

USHORT		fDriverFlags =	0;
UCHAR		fAPMSuspended = 0;
char		szDriverName[]= "DSLEEPS$";
USHORT		hdThisDriver =	0;

ULONG		ulTimer =	0;		/* free running timer */


/*
 * Configuration data.  A structure
 * for every filtered device.
 */

NPDEVICE	anpUnit[MAX_UNITS] = {0};
USHORT		cUnit =		0;		/* entries in above table */


/*
 * Message generated by this driver.  May
 * be discarded after INIT.
 */
#define MSG_REPLACEMENT_STRING	1178		/* %1 %2 %3 ... */

UCHAR	szMsgBuffer[1024]		= {0};

MSGTABLE msgBuffer = {
    MSG_REPLACEMENT_STRING, 1, (PBYTE)szMsgBuffer 
};


/*
 * Initialization Data
 */

UCHAR	MemPool[30*1024] =	{0};
NPUCHAR	npInitData =		MemPool;
BYTE    DeviceTable[MAX_DEVICETABLE_SIZE] = {0};
