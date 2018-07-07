/*
 * $Source: R:/source/driver/raid/RCS\\init2.c,v $
 * $Revision: 1.7 $
 * $Date: 2000/07/03 01:53:15 $
 * $Locker:  $
 *
 *	Second step of initialisation, executed at ring 0 in protected
 *	mode when the system has loaded all drivers and inter-driver
 *	communication may be initialized.
 *
 * History:
 * $Log: init2.c,v $
 * Revision 1.7  2000/07/03 01:53:15  vitus
 * - InitLevel2: don't do SaveArrayInfos(), it was seen to hang the system.
 *   But living w/o this call isn't a problem as OS/2 will read at least the
 *   MPR from *every* disk and this will trigger the update.
 *
 * Revision 1.6  1999/09/23 23:03:32  vitus
 * - defined all DBGMSG() strings as located in CONST segment
 *
 * Revision 1.5  1999/07/06 02:03:06  vitus
 * - moved "fDriverFlags |= DF_INIT2DONE" to InitLevel2: build code checks flag
 *
 * Revision 1.4  1999/06/19 18:53:21  vitus
 * - InitLevel2: call SaveArrayInfos() to keep configuration sectors up-to-date
 *   Important for VSetup and detection of removed and later readded disks (!)
 *
 * Revision 1.3  1999/05/24 17:13:44  vitus
 * - surrounded WatCom pragma's with #ifdef.  MS-C complains.
 * -------------------------------------------
 * This code is Copyright Vitus Jensen 1998-99
 */
#if defined(__WATCOMC__)
# pragma off (unreferenced)
#endif
static char const id[]="$Id: init2.c,v 1.7 2000/07/03 01:53:15 vitus Exp $";
#if defined(__WATCOMC__)
# pragma on (unreferenced)
#endif

#include <string.h>

#define INCL_DOSERRORS
#define INCL_INITRP_ONLY
#include "os2intf.h"

#include "vraid.h"
#include "proto.h"
#include "extern.h"




#if defined(DEBUG)
CONSTMSG dszInitLevel2[]=	"\r\nInitLevel2()";
CONSTMSG dszInit2wo1[]=		"\r\nFATAL: InitLevel2() w/o InitLevel1()!";
CONSTMSG dszInit2done[]=	"\r\nInitLevel2(): already done";
CONSTMSG dszCompleteInit[]=	"\r\nCompleteInit()";
#endif






/*# ----------------------------------------------------------------------
 * CALL
 *	InitLevel2(void)
 * PARAMETER
 *	(none)
 * RETURNS
 *	(nothing)
 * GLOBAL
 *	(nothing)
 * DESCRIPTION
 *	Called after OS switches the last time to protected mode.
 *	Does nothing.
 *
 * REMARKS
 */
PRIVATE void NEAR
InitLevel2(void)
{
    DEBMSG(DBG_LEVEL1,dszInitLevel2);
    if( !(fDriverFlags & DF_INIT1DONE) )	/* 0: nothing, 3: ready to run */
    {
	DEBMSG(DBG_ALL,dszInit2wo1);
	DBSTOP();
	return;
    }
    if( (fDriverFlags & DF_INIT2DONE) )		/* 0: nothing, 3: ready to run */
    {
	DEBMSG(DBG_ALL,dszInit2done);
	return;
    }
    fDriverFlags |= DF_INIT2DONE;		/* 1 -> 3 */

#if 0 /* might hang the machine? */
    SaveArrayInfos();				/* keep up-to-date
						   (VSetup will analyse!)*/
#endif

    return;
}




/*# ----------------------------------------------------------------------
 * CALL
 *	CompleteInit(pIorb)
 * PARAMETER
 *	pIorb		IORB
 * RETURNS
 *	(nothing)
 * GLOBAL
 *	(nothing)
 * DESCRIPTION
 *	Called by REQPKT code 0x1B, driver may initiate IPC.
 *
 * REMARKS
 */
PUBLIC void NEAR
CompleteInit(PIORB_CONFIGURATION const pIorb)
{
    DEBMSG(DBG_LEVEL2,dszCompleteInit);

    InitLevel2();

    /* All done, ready to do I/O's. */

    NotifyIorb(&pIorb->iorbh, 0);		/* Say: OK */
    return;
}
