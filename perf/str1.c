/*
 * $Source: e:/source/driver/perf/RCS/str1.c,v $
 * $Revision: 1.3 $
 * $Date: 1997/12/16 02:22:14 $
 * $Author: vitus $
 *
 * Strategy Level 1 code.
 *
 * $Log: str1.c,v $
 * Revision 1.3  1997/12/16 02:22:14  vitus
 * - renamed source files
 *
 * Revision 1.2  1997/12/06 00:42:23  vitus
 * - enabled support for WatCom C
 *
 * Revision 1.1  1997/06/04 23:44:55  vitus
 * Initial revision
 * ----------------------------------------
 * Author: Vitus Jensen, 2:2474/424, 1997
 */
#pragma off (unreferenced)
static char vcid[]="$Id: str1.c,v 1.3 1997/12/16 02:22:14 vitus Exp $";
#pragma on (unreferenced)


/*#define INCL_NOBASEAPI*/
#define INCL_DOSERRORS
#define INCL_NOPMAPI
#include <os2.h>

#include <devcmd.h>

#define INCL_INITRP_ONLY
#include <reqpkt.h>

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

/* To skip including all request packets... */
extern USHORT NEAR	GenIOCtl(PRPH pRPH);




/*
 * NAME
 *	Shutdown
 * CALL
 *	Shutdown(rph)
 * PARAMETER
 *	rph		request header
 * RETURNS
 *	STATUS_DONE
 * GLOBAL
 *	none
 * DESPRIPTION
 *	The system starts or ends shutdown state.  There is currently
 *	nothing special we would like to do.
 *
 * REMARKS
 */
PRIVATE USHORT NEAR
Shutdown(PRPH const rph)
{
    /* What to do? */

    return STATUS_DONE;
}






/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */


/*
 * NAME
 *	Strategy
 * CALL
 *	Strategy(rph)
 * PARAMETER
 *	rph		request header
 * RETURNS
 *	nothing
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Entry point for strategy level 1 requests.  Only
 *	some commands are supported.
 * REMARKS
 *	Called by assembler stub.
 */
PUBLIC void NEAR _Cdecl
Strategy(PRPH rph)
{
    switch( rph->Cmd )
    {
      case CMDInitBase:
	if( (fDriverFlags & DF_INITDONE) )
	    rph->Status = STDON | STERR | ERROR_I24_BAD_COMMAND;
	else
	    rph->Status = InitBase( (PRPINITIN)rph );
	break;

      case CMDShutdown:
	rph->Status = Shutdown( rph );
	break;

      case CMDOpen:
      case CMDClose:
	rph->Status = STATUS_DONE;
	break;

      case CMDGenIOCTL:
	rph->Status = GenIOCtl( rph );
	break;

      default:
	rph->Status = STDON | STERR | ERROR_I24_BAD_COMMAND;
	break;
    }
    return;
}
