/*
 * $Source: R:/source/driver/raid/RCS\\str1.c,v $
 * $Revision: 1.6 $
 * $Date: 1999/10/01 00:54:18 $
 * $Locker:  $
 *
 *	Strategy Level 1 code.
 *
 * History:
 * $Log: str1.c,v $
 * Revision 1.6  1999/10/01 00:54:18  vitus
 * - defined all DBGMSG() strings as located in CONST segment
 *
 * Revision 1.5  1999/05/12 01:51:07  vitus
 * - calls SaveArrayInfos() on shutdown
 *
 * Revision 1.4  1998/09/27 16:11:27  vitus
 * - MS-C/C++ 7.0: _fstrncpy()
 * - Shutdown() detects start vs end of shutdown
 *
 * Revision 1.3  1998/06/26 23:05:13  vitus
 * - implemented _fstr*, _fmem* routines for MS-C
 *
 * Revision 1.2  1998/05/29 01:58:08  vitus
 * - added copyright to source
 *
 * Revision 1.1  1998/02/18 00:05:02  vitus
 * Initial revision
 * -------------------------------------------
 * This code is Copyright Vitus Jensen 1998-99
 */
#if defined(__WATCOMC__)
# pragma off (unreferenced)
#endif
static char const id[]="$Id: str1.c,v 1.6 1999/10/01 00:54:18 vitus Exp $";
#if defined(__WATCOMC__)
# pragma on (unreferenced)
#endif

#include <string.h>

#define INCL_DOSERRORS
#define INCL_NOPMAPI
#define INCL_INITRP_ONLY
#include "os2intf.h"

#include "vraid.h"
#include "proto.h"
#include "extern.h"

/* To skip including all request packets... */
extern USHORT NEAR	GenIOCtl(PRPH pRPH);




#if defined(DEBUG)
CONSTMSG dszShutdownEnter[]=	"\r\nSystem enters SHUTDOWN state";
CONSTMSG dszShutdownLeave[]=	"\r\nSystem completed SHUTDOWN";
#endif






/*# ----------------------------------------------------------------------
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
    PUCHAR	parameter = (PVOID)(rph + 1);

    /* What to do? */

    if( *parameter == 0 )			/* 0 means start of shutdown */
    {
	DEBMSG(DBG_LEVEL1,dszShutdownEnter);
	fDriverFlags |= DF_SHUTDOWN1;

	SaveArrayInfos();
    }
    else
    {
	DEBMSG(DBG_LEVEL1,dszShutdownLeave);
	fDriverFlags |= DF_SHUTDOWN2;
    }

    return STATUS_DONE;
}






/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */

#if defined(__WATCOMC__)

/* Nothing to do: _fmem*() are intrinsics! */

#elif (defined(_MSC_VER) && _MSC_VER >= 700)
char _far * _far _cdecl
_fstrncpy(char _far * d,const char _far * s,size_t size)
{
    char _far * dst = d;
    char const _far *src = s;

    while( (*dst++ = *src++) != '\0'   &&  size-- )
	;
    for(; size != 0; --size )
	*dst++ = '\0';
    return d;
}


#else
void _far * _far _cdecl
_fmemset(void _far *data,int val,size_t size)
{
    PUCHAR	d = data;

    while( size-- )
	*(d++) = (UCHAR)val;
    return data;
}


void _far * _far _cdecl
_fmemcpy(void _far * dest,const void _far * src,size_t size)
{
    PUCHAR	d = dest;
    UCHAR const FAR * s = src;

    while( size-- )
	*(d++) = *(s++);
    return dest;
}

int _far _cdecl
_fmemcmp(const void _far *s1,const void _far * s2,size_t size)
{
    char const _far * dest = s1;
    char const _far * src = s2;

    while( size-- )
	if( *(dest++) != *(src++) )
	    return 1;
    return 0;					/* memory regions identical */
}

size_t _far _cdecl
_fstrlen(const char _far * s)
{
    size_t	len = 0;

    for(; *s != '\0'; ++s )
	++len;
    return len;
}

char _far * _far _cdecl
_fstrcpy(char _far * d,const char _far * s)
{
    char _far * dst = d;
    char const _far *src = s;

    while( (*dst++ = *src++) != '\0' )
	;
    return d;
}

char _far * _far _cdecl
_fstrncpy(char _far * d,const char _far * s,size_t size)
{
    char _far * dst = d;
    char const _far *src = s;

    while( (*dst++ = *src++) != '\0'   &&  size-- )
	;
    for(; size != 0; --size )
	*dst++ = '\0';
    return d;
}
#endif




/*# ----------------------------------------------------------------------
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
	if( (fDriverFlags & DF_INIT1DONE) )
	    rph->Status = STDON | STERR | ERROR_I24_BAD_COMMAND;
	else
	    rph->Status = InitBase((PRPINITIN)rph);
	break;

      case CMDShutdown:
	rph->Status = Shutdown(rph);
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

