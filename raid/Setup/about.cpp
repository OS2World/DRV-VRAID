/*
 * $Source: R:/Source/driver/raid/Setup/RCS/about.cpp,v $
 * $Revision: 1.4 $
 * $Date: 1999/02/09 02:54:12 $
 * $Locker:  $
 *
 *	About dialog proc. VIW = Very Important Window!
 *
 * $Log: about.cpp,v $
 * Revision 1.4  1999/02/09 02:54:12  vitus
 * - switched to C++
 *
 * Revision 1.3  1998/12/13 23:07:18  vitus
 * - changed as global headers moved to /source/Lib
 *
 * Revision 1.2  1998/09/28 00:57:16  vitus
 * - VRAID instead SoftRAID
 *
 * Revision 1.1  1998/04/28 22:31:26  vitus
 * Initial revision
 * -------------------------------------------
 * This code is Copyright Vitus Jensen 1998-99
 */
static char vcid[]="$Id: about.cpp,v 1.4 1999/02/09 02:54:12 vitus Exp $";

#include <stdio.h>

#define __IBMC__	0
#define INCL_WIN
#include <os2.h>

extern "C" {
#include "../../../Lib/defines.h"
#include "../../../Lib/verbose.h"
}
#include "resources.h"
#include "vsetup.h"






/*# ----------------------------------------------------------------------
 * CALL
 *	AboutDialogProc(hwnd,msg,mp1,mp2)
 * PARAMETER
 *	hwnd,msg,mp1,mp2	standard parameter
 *	WM_INITDLG		mp2 contains pointer to version
 *
 * RETURNS
 *	MRESULT			standard result code
 *	dialog handling returns id of control which ended dialog
 *
 * DESPRIPTION
 *	Nothing special, selfcontained dialog procedure.
 *
 * REMARKS
 */
PUBLIC MRESULT EXPENTRY
AboutDialogProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
    switch( msg )
    {
	/*
	 * WM_INITDLG:
	 *	mp2	PUSHORT	- program version, format like MS-DOG
	 */
      case WM_INITDLG:
	{
	    USHORT	version = *(PUSHORT)PVOIDFROMMP(mp2);
	    char	str[300];

	    sprintf(str,
		    "VSetup - Version %u.%02u\r\n"
		    "A simple program to visual setup your VRAID devices.",
		    LOBYTE(version), HIBYTE(version) );
	    WinSetWindowText(WinWindowFromID(hwnd, ST_DESCRIPTION), str);
	}
	break;

      default:
	break;
    }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

