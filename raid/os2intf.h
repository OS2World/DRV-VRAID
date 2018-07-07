/*
 * $Source: R:/source/driver/raid/RCS\\os2intf.h,v $
 * $Revision: 1.2 $
 * $Date: 1998/06/26 23:00:17 $
 * $Author: vitus $
 *
 * Defines OS/2 interface (Includes, defines)
 *
 * $Log: os2intf.h,v $
 * Revision 1.2  1998/06/26 23:00:17  vitus
 * - added support of MS-C 6.00A
 *
 * Revision 1.1  1998/02/17 23:11:19  vitus
 * Initial revision
 * ----------------------------------------
 * This code is Copyright Vitus Jensen 1998
 */

#define INCL_NOPMAPI
#include <os2.h>

#include <devcmd.h>
#include <strat2.h>
#include <reqpkt.h>

#include <devclass.h>
#include <dskinit.h>
#include <infoseg.h>

#if defined(_MSC_VER)
# define _Cdecl		cdecl
# include <dhcalls.h>
#elif defined(__WATCOMC__)
# include <devhelp.h>
typedef USHORT NEAR *NPUSHORT;
typedef VOID   NEAR *NPVOID;
#endif
#include <iorb.h>
