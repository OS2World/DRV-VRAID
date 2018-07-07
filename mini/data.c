/*
 * $Source: r:/source/driver/mini/RCS/data.c,v $
 * $Revision: 1.2 $
 * $Date: 2000/06/12 02:13:51 $
 * $Locker:  $
 *
 * 	Declares all data used in this driver.
 *
 * $Log: data.c,v $
 * Revision 1.2  2000/06/12 02:13:51  vitus
 * - removed variables not used in sample
 *
 * Revision 1.1  2000/06/12 01:41:40  vitus
 * Initial revision
 * --------------------------------------------
 * Author: Vitus Jensen, 2:2474/424, 1997
 */
static char vcid[]="$Id: data.c,v 1.2 2000/06/12 02:13:51 vitus Exp $";

#define INCL_NOBASEAPI
#define INCL_NOPMAPI
#include <os2.h>

#define INCL_INITRP_ONLY
#include <reqpkt.h>

#if defined(__WATCOMC__)
# include <devhelp.h>
typedef USHORT NEAR *	NPUSHORT;
typedef VOID NEAR *	NPVOID;
#else
# include <dhcalls.h>
#endif

#include "mini.h"



/* **********************************************************************
 *	Static Data
 * ******************************************************************* */

#if defined(DEBUG)
USHORT		uDebugLevel = 0x8031;
#endif

PFN		Device_Help =	0;

USHORT		fDriverFlags =	0;



/* **********************************************************************
 *	Initialization Data
 * ******************************************************************* */

UCHAR		MemPool[30*1024] =	{0};
NPUCHAR		npInitData =		MemPool;

UCHAR	szMsgBuffer[1024]		= {0};
