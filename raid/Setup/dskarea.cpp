/*
 * $Source: R:/Source/driver/raid/Setup/RCS/dskarea.cpp,v $
 * $Revision: 2.5 $
 * $Date: 2000/06/12 20:31:00 $
 * $Locker:  $
 *
 *	Process dialog to select area on disk.
 *
 * $Log: dskarea.cpp,v $
 * Revision 2.5  2000/06/12 20:31:00  vitus
 * - changes to use QWL_USER constant as window word
 *
 * Revision 2.4  2000/05/28 17:36:17  vitus
 * - uses pcreate.h
 * - added calls to PPSubClassWindow()
 *
 * Revision 2.3  2000/04/10 01:40:51  vitus
 * - Verbose() uses key 'DskArea'
 * - hides 'Help' button, no functionality yet
 * - updates text area in dialog with currently selected area size
 * - prohibits area sizes below 1MB
 * - list box contains 'MB' instead of 'MByte'
 *
 * Revision 2.2  2000/03/17 02:09:48  vitus
 * - corrected size calculation
 *
 * Revision 2.1  2000/01/24 02:25:53  vitus
 * - QueryDlgItems: space calculation uses floating point.  Better results!
 *
 * Revision 1.9  2000/01/07 00:07:14  vitus
 * - QueryDlgItems: adjusted selected space calculation so that
 *   no 32bit overflow occurs.  See mail to Knud.
 *
 * Revision 1.8  1999/06/20 17:03:13  vitus
 * - minor corrections to keep compiler happy
 *
 * Revision 1.7  1999/03/24 23:44:10  vitus
 * - expanded debugging messages
 *
 * Revision 1.6  1999/03/01 02:19:36  vitus
 * - include dskinfo.hpp for drvbuild.h
 *
 * Revision 1.5  1999/02/09 02:50:44  vitus
 * - switched to C++
 *
 * Revision 1.4  1998/12/13 23:32:23  vitus
 * - changed as global headers moved to /source/Lib
 *
 * Revision 1.3  1998/11/03 03:00:30  vitus
 * - QueryDlgItems: return /0 and display message box if user forgot to
 *   select a disk area.  dialog will remain on screen.
 *
 * Revision 1.2  1998/06/09 01:30:36  vitus
 * - corrected percent calculation
 *
 * Revision 1.1  1998/05/27 01:51:33  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2000
 */
static char const vcid[]="$Id: dskarea.cpp,v 2.5 2000/06/12 20:31:00 vitus Exp $";
static char const modname[]="DskArea";

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define __IBMC__	0
#define INCL_DOSDEVIOCTL
#define INCL_WIN
#define INCL_GPI
#include <os2.h>

extern "C" {
#include "../../../Lib/defines.h"
#include "../../../Lib/verbose.h"
#include "../../../Lib/pphnd.h"			/* presparam handling */
}
#include "vsetup.h"
#include "resources.h"

#include "../dsklayout.h"
#include "dskinfo.hpp"
#include "drvbuild.h"
#include "pcreate.h"






PRIVATE ULONG
CalcArea(HWND hwndDlg, DSKAREA const * const p, SHORT idx)
{
    assert( idx <= p->cnt );

    ULONG ul = (ULONG)WinSendDlgItemMsg(hwndDlg, SL_AREAPERCENT,
					SLM_QUERYSLIDERINFO,
					MPFROM2SHORT(SMA_SLIDERARMPOSITION,
						     SMA_RANGEVALUE), 0);
    ULONG const		freerange = p->area[idx].size;
    USHORT const 	total =  HIUSHORT(ul);
    USHORT const 	selected = (USHORT)(LOUSHORT(ul)+1);
    double const 	percent = 100.0 * selected / total;

    Verbose(2, modname, "%u of %u gives %.2lf percent",
	    selected, total, percent);
    ULONG const 	size = freerange * percent / 100.0;
    Verbose(2, modname, "gives %lu sectors (of %lu)", size, freerange);
    return size;
}




/*# ----------------------------------------------------------------------
 * InitDlgItems(hwndDlg,p)
 *
 * PARAMETER
 *	hwndDlg		window handle of dialoge
 *	p		pointer to dialog parameters
 *
 * RETURNS
 *	0		OK
 *	/0		couldn't configure dialog
 *
 * DESPRIPTION
 *	Configures some controls which can't from dialog template.
 *
 * REMARKS
 *	Uses global 'apcszRaidType'.
 * ------------------------------------------------------------------- #*/
static int
InitDlgItems(HWND const hwndDlg,PDSKAREA const p)
{
    BOOL	bool;
    HWND	hwndItem;
    LONG	l;


    Verbose(3, modname, "InitDlg (parameter %#x)", p);

    /* Store caller pointer */

    if( (bool = WinSetWindowPtr(hwndDlg, QWL_USER, (PVOID)p)) == FALSE )
    {
	Verbose(0, modname, "WinSetWindowPtr failed");
	return 1;				/* can't continue */
    }

    /* Store area sizes in list-box. */

    hwndItem = WinWindowFromID(hwndDlg, LB_AREASIZE);
    Verbose(4, modname, "hwnd(LB_AREASIZE): %#x", hwndItem);
    PPSubclassWindow(hwndItem, modname);
    for( l = 0; l < p->cnt; ++l )
    {
	char	s[50];

	sprintf(s, "%lu MB", p->area[l].size / 2 / 1024);
	WinSendMsg(hwndItem, LM_INSERTITEM, MPFROMLONG(l), s);
    }

    /* Disable slider (enabled on area select) */

    hwndItem = WinWindowFromID(hwndDlg, SL_AREAPERCENT);
    Verbose(4, modname, "hwnd(SL_AREAPERCENT): %#x", hwndItem);
    PPSubclassWindow(hwndItem, modname);

    WinSendMsg(hwndItem, SLM_SETTICKSIZE, MPFROM2SHORT(SMA_SETALLTICKS,6), NULL);
    WinEnableWindow(hwndItem, FALSE);

    /* Hide static text saying how much is selected. */

    hwndItem = WinWindowFromID(hwndDlg, ST_AREASIZE);
    Verbose(4, modname, "hwnd(ST_AREASIZE): %#x", hwndItem);
    PPSubclassWindow(hwndItem, modname);
    WinShowWindow(hwndItem, FALSE);

    hwndItem = WinWindowFromID(hwndDlg, DID_HELP);
    PPSubclassWindow(hwndItem, modname);
    WinShowWindow(hwndItem, FALSE);

    return 0;
}




/*# ----------------------------------------------------------------------
 * QueryDlgItems(hwndDlg,p)
 *
 * PARAMETER
 *	hwndDlg		window handle of dialog
 *	p		insert values here
 *
 * RETURNS
 *	0		OK, do start test
 *	/0		some value missing, don't start
 *
 * DESPRIPTION
 *	Asks all controls inside the dialog box for current values
 *	and fills them into structure 'p' points to.
 *
 * REMARKS
 *	Sets focus to control with invalid data to allow user
 *	to complete input.
 * ------------------------------------------------------------------- #*/
static int
QueryDlgItems(HWND hwndDlg,PDSKAREA const p)
{
    MRESULT	mresult;
    SHORT	idx;

    Verbose(4, modname, "QueryDlgItems(parameter %#x)", p);

    /* Query drive type selected by user. */

    mresult = WinSendDlgItemMsg(hwndDlg, LB_AREASIZE, LM_QUERYSELECTION,
				MPFROMLONG(LIT_FIRST), 0);
    idx = SHORT1FROMMR(mresult);
    if( LIT_NONE == idx )
    {
	Verbose(1, modname, "No disk area selected!");
	MyMessageBox(1, hwndDlg, "You forgot to select an area...");
	return 1;
    }

    Verbose(2, modname, "selected area %d", idx);
    ULONG	size = CalcArea(hwndDlg, p, idx);
    if( size < 2048 )
    {
	Verbose(1, modname, "Area too small!");
	MyMessageBox(1, hwndDlg, "Please select a reasonable size...");
	return 2;
    }

    p->cnt = idx;				/* move selected area to 'cnt' */
    p->area[idx].size = size;

    return 0;
}




/*# ----------------------------------------------------------------------
 * DrvtypeDlgProc(hwnd,msg,mp1,mp2)
 *
 * PARAMETER
 *	hwnd,msg,mp1,mp2	see PM reference
 *
 * RETURNS
 *	MRESULT			see PM reference
 *
 * DESCRIPTION
 *	Dialog window procedure for partition generation.
 *
 * REMARKS
 * ------------------------------------------------------------------- #*/
PUBLIC MRESULT EXPENTRY
AreaDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
    switch( msg )
    {
	/* WM_INITDLG:
	 *	Save caller parameter in window words and
	 *	initialize all control inside the dialog. */
      case WM_INITDLG:
        {
	    PDSKAREA	p = (PDSKAREA)PVOIDFROMMP(mp2);

	    if( InitDlgItems(hwnd, p) )
	    {
		WinDismissDlg(hwnd, DID_ERROR); /* abort dialog */
		return FALSE;
	    }
	}
        break;

	/* ============================================================
	 * WM_COMMAND:
	 *	Button control sends.
	 *	Should return 0 (reserved value). */
      case WM_COMMAND:
	switch( SHORT1FROMMP(mp1) )
	{
	    /* DID_OK:
	     *	User pressed OK, query all controls about
	     *	actual values, check these, fill them in caller 
	     *	structure and return OK to caller. */
	  case DID_OK:
	    {
		PDSKAREA p = (PDSKAREA)WinQueryWindowPtr(hwnd, QWL_USER);

		if( 0 != QueryDlgItems(hwnd, p) )
		    return 0;			/* restart from beginning */

		WinDismissDlg(hwnd, DID_OK);
	    }
	    return 0;

	  case DID_CANCEL:
	    WinDismissDlg(hwnd, DID_CANCEL);
	    return 0;
	}
	break;


	/* ============================================================
	 * WM_CONTROL:
	 *	Notifications from the elements of our dialog.
	 */
      case WM_CONTROL:
	Verbose(5, modname, "WM_CONTROL");
	{
	    USHORT const	id = SHORT1FROMMP(mp1);
	    USHORT const	ncode = SHORT2FROMMP(mp1);
	    HWND const	hwndSlider = WinWindowFromID(hwnd, SL_AREAPERCENT);
	    HWND const	hwndLB = WinWindowFromID(hwnd, LB_AREASIZE);
	    HWND const	hwndText = WinWindowFromID(hwnd, ST_AREASIZE);
	    char	str[20];

	    Verbose(4, modname,"WM_CONTROL, id %#x, code %#x", id, ncode);
	    switch( id )
	    {
		MRESULT		mresult;
		ULONG		ul;
		SHORT		idx;

		/* LB_AREASIZE
		 *	Things the list-box wants to tell us. */
	      case LB_AREASIZE:
		switch( ncode )
		{

		  case LN_SELECT:
		    mresult = WinSendMsg(hwndLB, LM_QUERYSELECTION,
					 MPFROMLONG(LIT_FIRST), 0);
		    idx = SHORT1FROMMR(mresult);
		    if( LIT_NONE != idx )
		    {
			PDSKAREA p = (PDSKAREA)WinQueryWindowPtr(hwnd, QWL_USER);

			Verbose(3, modname, "LN_SELECT area %d", idx);
			WinEnableWindow(hwndSlider, TRUE);
			WinSendMsg(hwndSlider, SLM_SETSLIDERINFO,
				   MPFROM2SHORT(SMA_SLIDERARMPOSITION,
						SMA_INCREMENTVALUE),
				   MPFROMLONG(4));

			sprintf(str, "Size:\t%lu MByte",
				p->area[idx].size / 2ul / 1024);
			WinSetWindowText(hwndText, str);
			WinShowWindow(hwndText, TRUE);
		    }
		    return 0;

		  default:
		    break;
		}
		break;

		/* SL_AREAPERCENT:
		 *	Things the slider wants to tell us. */
	      case SL_AREAPERCENT:
		switch( ncode )
		{
		    /* SLN_CHANGE
		     *	The slider was moved to a new position.  Update
		     * 	static text with new size of selected part. */
		  case SLN_CHANGE:
		    mresult = WinSendMsg(hwndSlider, LM_QUERYSELECTION,
					 MPFROMLONG(LIT_FIRST), 0);
		    ul = CalcArea(hwnd,
				  (PDSKAREA)WinQueryWindowPtr(hwnd, QWL_USER),
				  SHORT1FROMMR(mresult));
		    sprintf(str, "Size:\t%lu MByte", ul / 2ul / 1024);
		    WinSetWindowText(hwndText, str);
		    break;

		  default:
		    break;
		}
		break;
	    }
	} /* WM_CONTROL */
	break;

	/* default: let WinDefDlgProc() handle all others. */
      default:
	break;
    }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}
