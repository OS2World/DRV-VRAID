/*
 * $Source: r:/source/driver/sleep/RCS/str1.c,v $
 * $Revision: 1.10 $
 * $Date: 2001/02/27 02:24:09 $
 * $Locker:  $
 *
 *	Strategy Level 1 code.
 *
 * History:
 * $Log: str1.c,v $
 * Revision 1.10  2001/02/27 02:24:09  vitus
 * - APM registration may be disabled via DF_NOAPM flag
 *
 * Revision 1.9  2001/02/21 23:22:15  vitus
 * - handles CMDInitComplete and registers APM event handler
 *
 * Revision 1.8  1999/11/29 00:37:15  vitus
 * - moved all system includes to dsksleep.h
 * - minor changes for M$ C 7.0 (_fmem* routines excluded as they are inlined)
 *
 * Revision 1.7  1998/06/28 23:56:16  vitus
 * - renamed _far_*() to _f*() (for MS-C, WatCom supplies)
 * - switched to DDK support macros (cli, pushf, etc.)
 *
 * Revision 1.6  1997/12/05 01:44:13  vitus
 * - added WatCom specific code and header
 * - moved history to bottom of file
 * - added _Cdecl to all exported functions
 *
 * Revision 1.5  1997/09/03 01:07:38  vitus
 * Restarts units/cancels sleeping if DF_SHUTDOWN
 *
 * Revision 1.4  1997/03/03 01:28:25  vitus
 * Added _far_strlen, _far_strcpy
 *
 * Revision 1.3  1997/02/05 01:53:52  vitus
 * Added call to GenIOCtl()
 *
 * Revision 1.2  1996/10/23 23:42:54  vitus
 * Added _far_mem*() functions
 *
 * Revision 1.1  1996/09/27 02:33:30  vitus
 * Initial revision
 * -------------------------------------------
 * This code is Copyright Vitus Jensen 1996-99
 */
static char id[]="$Id: str1.c,v 1.10 2001/02/27 02:24:09 vitus Exp $";

#define INCL_DOSERRORS
#define INCL_NOPMAPI
#define INCL_INITRP_ONLY
#include "dsksleep.h"
#include "apmcalls.h"

#include "proto.h"
#include "extern.h"

/* To skip including all request packets... */
extern USHORT NEAR	GenIOCtl(PRPH pRPH);


#if defined(DEBUG)
char _inconst dszShutdownStart[]=	"\r\nSystem enters SHUTDOWN state";
char _inconst dszShutdownEnd[]=		"\r\nSystem completed SHUTDOWN";
char _inconst dszCMDInit2[]=		"\r\nCMDInitComplete";
char _inconst dszAPMAttached[]=		"\r\nAttached to APM.SYS!";
char _inconst dszAPMRegister[]=		"\r\nAPMRegister() returned %w";
char _inconst dszUnknownStr[]=		"\r\nUnknown strategy coded %w";
#endif





#if defined(_MSC_VER) && _MSC_VER < 700
PUBLIC void NEAR
_fmemset(void _far *data,int val,size_t size)
{
    PUCHAR	d = data;

    while( size-- )
	*(d++) = val;
}


PUBLIC void NEAR
_fmemcpy(void _far * dest,const void _far * src,size_t size)
{
    PUCHAR	d = dest;
    UCHAR const FAR * s = src;

    while( size-- )
	*(d++) = *(s++);
}

PUBLIC int NEAR
_fmemcmp(const void _far *s1,const void _far * s2,size_t size)
{
    UCHAR const * dest = s1, src = s2;

    while( size-- )
	if( *(dest++) != *(src++) )
	    return 1;
    return 0;					/* memory regions identical */
}

PUBLIC size_t NEAR
_fstrlen(const char _far * s)
{
    size_t	len = 0;

    for(; *s != '\0'; ++s )
	++len;
    return len;
}

PUBLIC _far * NEAR
_fstrcpy(char _far * d,const char _far * s)
{
    UCHAR	_far * dst = d, const * _far *src = s;

    while( (*dst++ = *src++) != '\0' )
	;
    return d;
}
#endif




/*# ----------------------------------------------------------------------
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
 *	Disables device stopping when shutdown starts.
 *	Could start all stopped devices to avoid errors when
 *	BIOS starts.
 *	BUT --- OS/2 flushes all cache buffers before shutting
 *	down, so all disks are rotating anyway now.
 *
 * REMARKS
 *	Is this even neccessary?  Newer AHA* have an option
 *	"send start" in BIOS setup...
 */
PRIVATE USHORT NEAR
Shutdown(PRPH const rph)
{
    USHORT	u;

    if( (fDriverFlags & DF_SHUTDOWN) )
    {
	if( *(PUCHAR)(rph+1) == 0 )		/* 0 means start of shutdown */
	{
	    /* For every device in our table disable
	     * device stopping. */

	    DEBMSG(dszShutdownStart);
	    for( u = 0; u < cUnit; ++u )
	    {
		cli();
		anpUnit[u]->usSleep = 0;
		anpUnit[u]->counter = 0;
		sti();
	    }
	}
	else
	{
	    DEBMSG(dszShutdownEnd);

	    /* For every device in our table check status
	     * and start rotation if stopped. */

	    for( u = 0; u < cUnit; ++u )
	    {
		if( anpUnit[u]->stopped )
		    IssueStart(anpUnit[u]);	/* restart */
	    }
	}
    }

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
PUBLIC void _Cdecl NEAR
Strategy(PRPH rph)
{
    switch( rph->Cmd )
    {
      case CMDInitBase:
	if( (fDriverFlags & DF_INITDONE) )
	    rph->Status = STDON | STERR | ERROR_I24_BAD_COMMAND;
	else
	    rph->Status = InitBase((PRPINITIN)rph);
	break;

      case CMDInitComplete:
	DEBMSG(dszCMDInit2);
	if( !(fDriverFlags & DF_INITCOMPLETE) )
	{
	    if( !(fDriverFlags & DF_NOAPM)  &&  APMAttach() == 0 )
	    {
		USHORT	rc;

		DEBMSG(dszAPMAttached);

		/* If attached, register for suspend */

		rc = APMRegister((APMHANDLER)APMEventHandler,
				 APM_NOTIFYSETPWR
				 | APM_NOTIFYNORMRESUME
				 | APM_NOTIFYCRITRESUME,
				 0);		/* 0x02FF = all disks */

		DEBMSG1(dszAPMRegister, rc);
	    }
	    fDriverFlags |= DF_INITCOMPLETE;
	}
	rph->Status = STATUS_DONE;
	break;

      case CMDShutdown:
	rph->Status = Shutdown(rph);
	break;

      case CMDOpen:
      case CMDClose:
	rph->Status = STATUS_DONE;
	break;

      case CMDGenIOCTL:
	rph->Status = GenIOCtl(rph);
	break;

      default:
	DEBMSG1(dszUnknownStr, rph->Cmd);
	rph->Status = STDON | STERR | ERROR_I24_BAD_COMMAND;
	break;
    }
    return;
}

