/*
 * $Source: r:/source/driver/raid/setup/RCS/drvtype.cpp,v $
 * $Revision: 2.1 $
 * $Date: 2003/09/24 10:46:04 $
 * $Locker:  $
 *
 *	Process dialog to select type of RAID device.
 *
 * $Log: drvtype.cpp,v $
 * Revision 2.1  2003/09/24 10:46:04  vitus
 * - added RAID 5 code (new major because of "stopped" development)
 *
 * Revision 1.11  2000/06/12 20:30:28  vitus
 * - changes to use QWL_USER constant as window word
 *
 * Revision 1.10  2000/05/28 17:40:02  vitus
 * - adjusted verbosity levels
 *
 * Revision 1.9  2000/04/10 01:00:01  vitus
 * - include VSetup.h instead of proto.h
 * - converted calls to WinMessageBox() to MyMessageBox()
 *
 * Revision 1.8  1999/06/20 17:03:47  vitus
 * - minor corrections to keep compiler happy
 *
 * Revision 1.7  1999/03/01 02:20:53  vitus
 * - include dskinfo.hpp for drvbuild.h
 * - changed DSKTYPE_* to RDTYPE_* constants
 *
 * Revision 1.6  1999/02/09 02:50:16  vitus
 * - switched to C++
 *
 * Revision 1.5  1998/12/17 02:22:37  vitus
 * - changed for moved global headers
 *
 * Revision 1.4  1998/11/03 02:50:38  vitus
 * - QueryDlgItems: return /0 and display message box if user forgot to
 *   select a drive type.  dialog will remain on screen.
 *
 * Revision 1.3  1998/06/09 01:23:24  vitus
 * - corrected drive size calculation
 *
 * Revision 1.2  1998/05/27 02:23:51  vitus
 * - correctly fill listbox for one or more childs
 *
 * Revision 1.1  1998/05/18 01:53:45  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2001
 */
static char const vcid[]="$Id: drvtype.cpp,v 2.1 2003/09/24 10:46:04 vitus Exp $";

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define __IBMC__	0
#define INCL_DOS
#define INCL_DOSDEVIOCTL
#define INCL_ERRORS
#define INCL_WIN
#define INCL_GPI
#include <os2.h>

extern "C" {
#include "../../../Lib/defines.h"
#include "../../../Lib/verbose.h"
#include "../../../Lib/profile.h"		/* profile handling */
}
#include "VSetup.h"
#include "resources.h"

#include "../dsklayout.h"
#include "dskinfo.hpp"
#include "drvbuild.h"


PSZ const	apcszRaidType[] = {"Single",
				   "Chain",
				   "Stripe (RAID 0)",
				   "Mirror (RAID 1)",
				   "RAID 4",
				   "RAID 5"};






/*# ----------------------------------------------------------------------
 * InitDlgItems(hwndDlg,p)
 *
 * PARAMETER
 *	hwndDlg		window handle of dialoge
 *	p		pointer to dialog parameters
 * RETURNS
 *	0		OK
 *	/0		couldn't configure dialog
 * GLOBAL
 *	apcszRaidType
 * DESPRIPTION
 *	Configures some controls which can't from dialog template.
 * REMARKS
 */
PRIVATE int
InitDlgItems(HWND const hwndDlg,PDRVBUILD const p)
{
    BOOL	bool;
    HWND	hwndItem;
    LONG	l, idx;

    Verbose(3, "DrvSetup", "InitDlg (parameter %#x)", p);

    /* Store caller pointer */

    if( (bool = WinSetWindowPtr(hwndDlg, QWL_USER, (PVOID)p)) == FALSE )
    {
	Verbose(1, "DrvSetup", "WinSetWindowPtr failed");
	return 1;				/* can't continue */
    }

    /* Store possible RAID levels in combo-box (list-box control). */

    hwndItem = WinWindowFromID(hwndDlg, CB_DRVTYPE);
    Verbose(4, "DrvSetup", "hwnd(CB_DRVTYPE): %#x", hwndItem);
    for( idx = l = 0; l < sizeof(apcszRaidType)/sizeof(apcszRaidType[0]); ++l )
    {
	if( 1 == p->cnt  &&  0 != l )
	    continue;				/* only "single" possible */
	else if( 1 != p->cnt  &&  0 == l )
	    continue;				/* "single" not possible */

	WinSendMsg(hwndItem, LM_INSERTITEM, MPFROMLONG(idx), apcszRaidType[l]);
	++idx;
    }

    hwndItem = WinWindowFromID(hwndDlg, SC_DRVSIZE);
    Verbose(4, "DrvSetup", "hwnd(SC_DRVSIZE): %#x", hwndItem);
    WinSetWindowText(hwndItem, "(still unknown)");

    return 0;
}




/*# ----------------------------------------------------------------------
 * QueryDlgItems(hwndDlg,p)
 *
 * PARAMETER
 *	hwndDlg		window handle of dialog
 *	p		insert values here
 * RETURNS
 *	0		OK, do start test
 *	/0		some value missing, don't start
 * GLOBAL
 *	aulIoSize[]
 * DESPRIPTION
 *	Asks all controls inside the dialog box for current values
 *	and fills them into structure 'p' points to.
 * REMARKS
 *	Sets focus to control with invalid data to allow user
 *	to complete input.
 */
PRIVATE int
QueryDlgItems(HWND hwndDlg,PDRVBUILD const p)
{
    SHORT	idx;
    MRESULT	mresult;

    Verbose(4, "DrvSetup", "QueryDlgItems(parameter %#x)", p);

    /* Query drive type selected by user. */

    mresult = WinSendDlgItemMsg(hwndDlg, CB_DRVTYPE, LM_QUERYSELECTION,
				MPFROMLONG(LIT_FIRST), 0);
    idx = SHORT1FROMMR(mresult);
    if( LIT_NONE == idx )
    {
	Verbose(1, "DrvSetup", "No drive type selected!");
	MyMessageBox(1, hwndDlg,
		     "You forgot to select a drive type...");
	return 1;
    }
    else if( 1 != p->cnt )
	p->type = idx + 2;			/* first entry not displayed */
    else
    {
	assert(idx == 0);			/* only "single" possible */
	p->type = idx + 1;
    }

    return 0;
}




/*# ----------------------------------------------------------------------
 * DrvtypeDlgProc(hwnd,msg,mp1,mp2)
 *
 * PARAMETER
 * RETURNS
 * GLOBAL
 * DESCRIPTION
 * REMARKS
 */
PUBLIC MRESULT EXPENTRY
DrvtypeDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
    switch( msg )
    {
	/* WM_INITDLG:
	 *	Save caller parameter in window words and
	 *	initialize all control inside the dialog. */
      case WM_INITDLG:
        {
	    PDRVBUILD	p = (PDRVBUILD)PVOIDFROMMP(mp2);

	    if( InitDlgItems(hwnd, p) )
	    {
		WinDismissDlg(hwnd, DID_ERROR); /* abort dialog */
		return FALSE;
	    }
	}
	return FALSE;				/*all done, focus not changed*/

	/* WM_COMMAND:
	 *	Button control sends.
	 *	Should return 0 (reserved value). */
      case WM_COMMAND:
	switch( SHORT1FROMMP(mp1) )
	{
	    /* DID_START:
	     *	User pressed OK, query all controls about
	     *	actual values, check these, fill them in caller 
	     *	structure and return OK to caller. */
	  case DID_OK:
	    {
		PDRVBUILD p = (PDRVBUILD)WinQueryWindowPtr(hwnd, QWL_USER);

		if( QueryDlgItems(hwnd, p) != 0 )
		    return 0;			/* restart from beginning */

		WinDismissDlg(hwnd, DID_OK);
	    }
	    return 0;

	  case DID_CANCEL:
	    WinDismissDlg(hwnd, DID_CANCEL);
	    return 0;
	}
	break;

	/* WM_CONTROL:
	 *	Things the combo-box wants to tell us. */
      case WM_CONTROL:
	Verbose(4, "DrvSetup", "DrvType.WM_CONTROL");
	{
	    PDRVBUILD	p = (PDRVBUILD)WinQueryWindowPtr(hwnd, QWL_USER);
	    USHORT	code = SHORT2FROMMP(mp1);
	    HWND	hwndItem = HWNDFROMMP(mp2);
	    MRESULT	mresult;
	    SHORT	idx;
	    CHAR	str[30];

	    switch( code )
	    {
	      case CBN_ENTER:
		mresult = WinSendMsg(hwndItem, LM_QUERYSELECTION,
					MPFROMLONG(LIT_FIRST), 0);
		idx = SHORT1FROMMR(mresult);
		if( LIT_NONE != idx )
		{
		    ULONG	size = 0;

		    if( 1 != p->cnt )
			idx += 2;		/* first entry not displayed */
		    else
		    {
			assert(idx == 0);	/* only "single" possible */
			idx += 1;
		    }

		    Verbose(2, "DrvSetup", "selected type %d", idx);
		    switch( idx )
		    {
			int	i;

		      case RDTYPE_SINGLE:
			size = p->child[0]->dsk->querySize();
			break;

		      case RDTYPE_CHAIN:	/* chain */
			for( i = 0; i < p->cnt; ++i )
			    size += p->child[i]->dsk->querySize();
			break;

		      case RDTYPE_STRIPE:	/* stripe */
			size = (ULONG)ULONG_MAX;
			for( i = 0; i < p->cnt; ++i )
			    size = min(size, p->child[i]->dsk->querySize());
			size *= p->cnt;
			break;

		      case RDTYPE_MIRROR:
			size = (ULONG)ULONG_MAX;
			for( i = 0; i < p->cnt; ++i )
			    size = min(size, p->child[i]->dsk->querySize());
			break;

		      case RDTYPE_RAID4:
		      case RDTYPE_RAID5:
			size = (ULONG)ULONG_MAX;
			for( i = 0; i < p->cnt; ++i )
			    size = min(size, p->child[i]->dsk->querySize());
			size *= p->cnt - 1;
			break;
		    }
		    sprintf(str, "appr. %lu MBytes", size/2/1024);
		    hwndItem = WinWindowFromID(hwnd, SC_DRVSIZE);
		    Verbose(4, "DrvSetup", "hwnd(SC_DRVSIZE): %#x", hwndItem);
		    WinSetWindowText(hwndItem, str);
		}
		return 0;

	      default:
		break;
	    }
	}
	break;

	/* default: let WinDefDlgProc() handle all others. */
      default:
	break;
    }

    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}
