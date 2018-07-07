/*
 * $Source: r:/source/driver/mini/RCS/init1.c,v $
 * $Revision: 1.2.1.2 $
 * $Date: 2001/06/26 02:06:42 $
 * $Locker:  $
 *
 * 	Driver initialisation, executed at ring 3 with IOPL.
 *
 * $Log: init1.c,v $
 * Revision 1.2.1.2  2001/06/26 02:06:42  vitus
 * - removed code to create file on load
 *
 * Revision 1.2.1.1  2001/06/26 01:59:16  vitus
 * - removed unused variables
 *
 * Revision 1.2  2000/06/12 02:14:54  vitus
 * - added extern npInitData
 *
 * Revision 1.1  2000/06/12 02:08:20  vitus
 * Initial revision
 * --------------------------------------------
 * Author: Vitus Jensen, 2:2474/424
 */
static char const id[]="$Id: init1.c,v 1.2.1.2 2001/06/26 02:06:42 vitus Exp $";

#include <string.h>

#define INCL_NOPMAPI
#define INCL_DOSERRORS
#define INCL_DOSDEVICES
#define INCL_DOSDEVIOCTL
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

extern NPUCHAR		npInitData;


#define INITMSG		static char _ininit


/*
 * Messages used during initialization
 *
 * 1. error messages start with driver name as they are
 * displayed even if '/V' isn't used.
 */
INITMSG	szAPIError[]=	"%s: error %u from Dos* API";






/*
 * Last element in code segments which remains resident
 */
PRIVATE void
Code_End(void) {}




/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */


/*# ----------------------------------------------------------------------
 * InitBase(pRPI)
 *
 * PARAMETER
 *	pRPI		init request
 *
 * RETURNS
 *	error code as expected from the OS/2 device system
 *
 * DESPRIPTION
 *	Process Initialization Request Packet
 *
 * REMARKS
 * ------------------------------------------------------------------- #*/
PUBLIC USHORT NEAR
Init(PRPINITIN pRPI)
{
    PRPINITOUT const	pRPO = (PRPINITOUT)pRPI;
    int			rc = 0;


    DEBMSG(DBG_ALL,"\r\nInitBase");
    DBSTOP();

    /* Contains entry point to kernel functions */

    Device_Help = pRPI->DevHlpEP;

#if 0
    do
    {
	HFILE	hf;
	UCHAR	uch;
	USHORT	us;

	rc = DosOpen("C:/Mini.out", &hf, &us, 0, 0, 0x0010, 0x2091, 0);
	if( rc != 0 )
	{
	    sprintk(szMsgBuffer, szAPIError, (PSZ)szDriverName, rc);
	    DosWrite(2, szMsgBuffer, strlen(szMsgBuffer), &us);
	    rc = -1;
	    break;
	}

	rc = DosWrite(hf, "Dummy\n", 6, &us);
	if( rc != 0 )
	{
	    sprintk(szMsgBuffer, szAPIError, (PSZ)szDriverName, rc);
	    DosWrite(2, szMsgBuffer, strlen(szMsgBuffer), &us);
	    rc = -1;
	    break;
	}

	uch = 0;
	us = 0;
	rc = DosDevIOCtl(&us, &uch, DEV_FLUSHOUTPUT, IOCTL_GENERAL, hf);
	if( rc != 0 )
	{
	    sprintk(szMsgBuffer, szAPIError, (PSZ)szDriverName, rc);
	    DosWrite(2, szMsgBuffer, strlen(szMsgBuffer), &us);
	    rc = -1;
	    break;
	}

	DosClose(hf);
    }
    while( 0 );
#endif


    /* Processing complete.  Analyse situation, should we install,
     * quiet fail or even loudly stop Config.Sys processing? */

    if( rc == 0 )
    {
	/* We do not need to register any units, since this
	 * filter does not generate any units on his own. */

	pRPO->Unit    = 0;
	pRPO->CodeEnd = ((USHORT)Code_End & ~0x0F) + 0x10;
	pRPO->DataEnd = ((USHORT)npInitData & ~0x0F) + 0x10;

	DEBMSG(DBG_ALL,"\r\nDriver installed");
	fDriverFlags |= DF_INITDONE;
	return STATUS_DONE;
    }
    else
    {
	pRPO->Unit    = 0;
	pRPO->CodeEnd = 0;
	pRPO->DataEnd = 0;

	if( rc < 0 )
	    return STERR | STDON | ERROR_I24_GEN_FAILURE;
	else
	    return STERR | STDON | ERROR_I24_QUIET_INIT_FAIL;
    }
}
