/*
 * $Source: r:/source/driver/mini/RCS/str1.c,v $
 * $Revision: 1.2 $
 * $Date: 2001/06/26 02:02:05 $
 * $Locker:  $
 *
 * 	Strategy Level 1 code.
 *
 * $Log: str1.c,v $
 * Revision 1.2  2001/06/26 02:02:05  vitus
 * - inserted GenIOCtl() from ioctl.c
 *
 * Revision 1.1  2000/06/12 01:44:00  vitus
 * Initial revision
 *
 * ----------------------------------------
 * Author: Vitus Jensen, 2:2474/424
 */
static char vcid[]="$Id: str1.c,v 1.2 2001/06/26 02:02:05 vitus Exp $";

/*#define INCL_NOBASEAPI*/
#define INCL_DOSERRORS
#define INCL_NOPMAPI
#include <os2.h>

#include <devcmd.h>

#include <strat2.h>
#include <reqpkt.h>

#if defined(__WATCOMC__)
# include <devhelp.h>
typedef USHORT NEAR *	NPUSHORT;
typedef VOID NEAR *	NPVOID;
#else
# include <dhcalls.h>
#endif

#include "mini.h"






/*# ----------------------------------------------------------------------
 * GenIOCtl(rp)
 *
 * PARAMETER
 *	rp		request packet
 *
 * RETURNS
 *	driver status word
 *
 * DESPRIPTION
 *	Calls approbiate subfunction.
 *
 * REMARKS
 */
PUBLIC USHORT
GenIOCtl(PRP_GENIOCTL rp)
{
    return STATUS_ERR_UNKCMD;
}





/*# ----------------------------------------------------------------------
 * Shutdown(rph)
 *
 * PARAMETER
 *	rph		request header
 *
 * RETURNS
 *	STATUS_DONE
 *
 * DESPRIPTION
 *	The system starts or ends shutdown state.  There is currently
 *	nothing special we would like to do.
 *
 * REMARKS
 * ------------------------------------------------------------------- #*/
PRIVATE USHORT NEAR
Shutdown(PRPH const rph)
{
    return STATUS_DONE;
}




/*# ----------------------------------------------------------------------
 * Open(rp)
 *
 * PARAMETER
 *	rp		request
 *
 * RETURNS
 *	STATUS_DONE
 *
 * DESPRIPTION
 *
 * REMARKS
 * ------------------------------------------------------------------- #*/
PRIVATE USHORT NEAR
Open(PRP_OPENCLOSE const rp)
{
    return STATUS_DONE;
}




/*# ----------------------------------------------------------------------
 * Close(rp)
 *
 * PARAMETER
 *	rp		request
 *
 * RETURNS
 *	STATUS_DONE
 *
 * DESPRIPTION
 *
 * REMARKS
 * ------------------------------------------------------------------- #*/
PRIVATE USHORT NEAR
Close(PRP_OPENCLOSE const rp)
{
    return STATUS_DONE;
}




/*# ----------------------------------------------------------------------
 * Read(rph)
 *
 * PARAMETER
 *	rph		request header
 *
 * RETURNS
 *	STATUS_ERR_NOTREADY
 *
 * DESPRIPTION
 *
 * REMARKS
 * ------------------------------------------------------------------- #*/
PRIVATE USHORT NEAR
Read(PRP_RWV const rp)
{
    return STATUS_ERR_NOTREADY;
}




/*# ----------------------------------------------------------------------
 * Write(rph)
 *
 * PARAMETER
 *	rph		request header
 *
 * RETURNS
 *	STATUS_ERR_NOTREADY
 *
 * DESPRIPTION
 *
 * REMARKS
 * ------------------------------------------------------------------- #*/
PRIVATE USHORT NEAR
Write(PRP_RWV const rp)
{
    return STATUS_ERR_NOTREADY;
}






/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */


/*# ----------------------------------------------------------------------
 * Strategy(rph)
 *
 * PARAMETER
 *	rph		request header
 *
 * RETURNS
 *	(nothing)
 *
 * DESPRIPTION
 *	Entry point for strategy level 1 requests.  Only
 *	some commands are supported.
 *
 * REMARKS
 *	Called by assembler stub.
 * ------------------------------------------------------------------- #*/
PUBLIC void NEAR _Cdecl
Strategy(PRPH rph)
{
    switch( rph->Cmd )
    {
      case CMDInit:
	if( (fDriverFlags & DF_INITDONE) )
	    rph->Status = STDON | STERR | ERROR_I24_BAD_COMMAND;
	else
	    rph->Status = Init((PRPINITIN)rph);
	break;

      case CMDShutdown:
	rph->Status = Shutdown(rph);
	break;

      case CMDOpen:
	rph->Status = Open((PRP_OPENCLOSE)rph);
	break;

      case CMDClose:
	rph->Status = Close((PRP_OPENCLOSE)rph);
	break;

      case CMDINPUT:
	rph->Status = Read((PRP_RWV)rph);
	break;

      case CMDOUTPUT:
	rph->Status = Write((PRP_RWV)rph);
	break;

      case CMDGenIOCTL:
	rph->Status = GenIOCtl((PRP_GENIOCTL)rph);
	break;

      default:
	rph->Status = STDON | STERR | ERROR_I24_BAD_COMMAND;
	break;
    }
    return;
}
