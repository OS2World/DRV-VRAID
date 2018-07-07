/*
 * $Source: R:/Source/driver/raid/Setup/RCS/iniloc.cpp,v $
 * $Revision: 1.3 $
 * $Date: 1999/02/09 02:51:36 $
 * $Locker:  $
 *
 *	Dialog box to select directory for .ini file.
 *
 * $Log: iniloc.cpp,v $
 * Revision 1.3  1999/02/09 02:51:36  vitus
 * - switched to C++
 *
 * Revision 1.2  1998/12/13 22:39:45  vitus
 * - changed as global headers moved to /source/Lib
 *
 * Revision 1.1  1998/04/29 00:44:00  vitus
 * Initial revision
 * -------------------------------------------
 * This code is Copyright Vitus Jensen 1998-99
 */
static char vcid[]="$Id: iniloc.cpp,v 1.3 1999/02/09 02:51:36 vitus Exp $";

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define __IBMC__	0
#define INCL_WIN
#include <os2.h>

#include "../../../Lib/defines.h"
#include "resources.h"
#include "vsetup.h"




/*# ----------------------------------------------------------------------
 * CALL
 *	InidirDialogProc(hwnd,msg,mp1,mp2)
 * PARAMETER
 *	hwnd,msg,mp1,mp2	standard parameter
 *	WM_INITDLG		mp2 contains pointer to defaul dir
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
InilocDialogProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
    static char *pszDir;			/* copied from parameter */

    switch( msg )
    {
      case WM_INITDLG:
	pszDir = (char *)PVOIDFROMMP(mp2);
	WinSetWindowText(WinWindowFromID(hwnd, EF_INILOCATION), pszDir);
	break;

      case WM_COMMAND:
	switch( SHORT1FROMMP(mp1) )
	{
	  case DID_OK:
	    {
		char  chText[_MAX_PATH];
		LONG  cbText;

		cbText = WinQueryWindowText(WinWindowFromID(hwnd, EF_INILOCATION),
					    sizeof(chText), chText);
		chText[cbText] = '\0';
		if( CheckValidDir(hwnd, chText) != 0 )
		{
						/* set input to entry field */
		    return 0;			/* redo from start */
		}
		strcpy(pszDir, chText);		/* copy to caller */
		WinDismissDlg(hwnd, DID_OK);
	    }
	    return 0;

	  case DID_CANCEL:
	    WinDismissDlg(hwnd, DID_CANCEL);
	    return 0;
	}
	break;

      default:
	break;
    }
    return WinDefDlgProc( hwnd, msg, mp1, mp2 );
}

