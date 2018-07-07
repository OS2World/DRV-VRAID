/*
 * $Source: R:/Source/driver/raid/Setup/RCS/drverify.cpp,v $
 * $Revision: 1.7 $
 * $Date: 2001/07/08 18:30:31 $
 * $Locker:  $
 *
 *	Verify redundance information.
 *
 *	- Thread doing the verification
 *	- Thread handling a diaglog for user interaction (progress bar,
 *	  pause/restart/abort, etc.)
 *
 * $Log: drverify.cpp,v $
 * Revision 1.7  2001/07/08 18:30:31  vitus
 * - the process to rewrite an invalid device is now called build not rebuild
 *
 * Revision 1.6  2001/01/30 00:35:06  vitus
 * - removed ioStartRebuild, this is now automatically done by filter
 *
 * Revision 1.5  2000/11/07 01:27:13  vitus
 * - add text to tick marks
 * - slider is now totally filled on 100%
 *
 * Revision 1.4  2000/08/17 02:12:16  vitus
 * - actually implemented the analysis of the verify operation.  On errors
 *   the user gets the possibility to ignore the error, abort the verify
 *   or start a rebuild (and abort).
 *   The rebuild starts on the very sector which failed the verify.
 * - we do call queryFltHd() instead of our caller
 *
 * Revision 1.3  2000/08/05 02:54:49  vitus
 * - corrected slider movements
 * - when verify is complete the dialog now remains visible (Pause disabled and
 *   Abort changed to Close)
 *
 * Revision 1.2  2000/06/15 01:15:09  vitus
 * - verify works good enough to use it as test frontend for driver operation.
 *   There is no update to the slider bar und the dialog needs some optical
 *   enhancements but it works...
 *
 * Revision 1.1  2000/06/06 01:06:10  vitus
 * Initial revision
 * -------------------------------------------
 * This code is Copyright Vitus Jensen 2000-01
 */
static char const vcid[]="$Id: drverify.cpp,v 1.7 2001/07/08 18:30:31 vitus Exp $";
static char const modname[]="DrvVerify";

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define INCL_DOS
#define INCL_DOSDEVIOCTL
#define INCL_ERRORS
#define INCL_WIN
#define INCL_WINSTDDRAG
#define INCL_GPI
#include <os2.h>

extern "C" {
#include "../../../Lib/defines.h"
#include "../../../Lib/verbose.h"
#include "../../../Lib/profile.h"		/* profile handling */
#include "../../../Lib/pphnd.h"			/* presparam handling */
}
#include "VSetup.h"
#include "resources.h"
#include "drvif.h"
#include "dskinfo.hpp"
#include "drverify.h"


#define VERIFY_SECTORS	10





/* **********************************************************************
 * **** Working class coordinating proceesing and display ***************
 * ******************************************************************* */


class Work {
  private:
    PTHR_VERIFY thread_dta;

    HWND	hwndProgressDlg;
    HEV		hevPause;			/* event sem for pause/resume */
    BOOL	fPaused;
    BOOL	fVerifyEnded;			/* set on abort or ended and OK  */

    ULONG	size;				/* size of array as returned by
						   vraid.flt [sectors] */
    ULONG	done;				/* already processed sectors */

  public:
    Work(PTHR_VERIFY d);
    ~Work();

    void	setDlg(HWND hwnd);

    BOOL	isPaused();
    void	pause();
    void	resume();
    void	stop();
    void	waitOK();

    BOOL	update(ULONG s, ULONG c);
    ULONG	queryPercentDone();
    void	abortDlg();
};




Work::Work(PTHR_VERIFY d)
{
    thread_dta = d;
    fPaused = FALSE;
    fVerifyEnded = FALSE;
    hwndProgressDlg = 0;

    APIRET	rc;
    rc = DosCreateEventSem(NULL, &hevPause, 0, TRUE);
    if( rc != 0 )
	throw(rc);

    size = d->drv->querySize();
}


Work::~Work()
{
    abortDlg();

    DosCloseEventSem(hevPause);
    DosSleep(2000);				/* wait time to let dialog close */
}


void Work::setDlg(HWND hwnd)
{
    assert( hwndProgressDlg == 0 );
    hwndProgressDlg = hwnd;

    /* If this method is called after somebody already has aborted
     * processing, post a message to ourself.  But this is a really
     * unusual case. */

    if( fVerifyEnded == TRUE )
	WinPostMsg(hwndProgressDlg, WM_CLOSE, 0, 0);
}


void Work::abortDlg()
{
    fVerifyEnded = TRUE;			/* nobody should access 'this' */

    if( hwndProgressDlg != 0 )			/* if still running */
	WinPostMsg(hwndProgressDlg, WM_CLOSE, 0, 0);
}

BOOL Work::isPaused()
{
    return fPaused;
}

void Work::pause()
{
    ULONG	ul;
    APIRET	rc;

    fPaused = TRUE;
    WinSetDlgItemText(hwndProgressDlg, PB_PAUSERESUME, "~Resume");
    rc = DosResetEventSem(hevPause, &ul);
}


void Work::resume()
{
    APIRET	rc;

    fPaused = FALSE;
    WinSetDlgItemText(hwndProgressDlg, PB_PAUSERESUME, "~Pause");
    rc = DosPostEventSem(hevPause);
}


void Work::stop()
{
    fVerifyEnded = TRUE;			/* somebody requested STOP */
    resume();
}


void Work::waitOK()
{
    if( fVerifyEnded )
	return;					/* already aborted */
    if( hwndProgressDlg != 0 )
    {
	BOOL	bool = WinPostMsg(hwndProgressDlg, WUM_COMPLETE, 0, 0);
	assert( bool == TRUE );

	/* Now wait until 'fVerifyEnded' is set (user pressed Abort). */

	while( !fVerifyEnded )
	    DosSleep(2000);
    }
}


BOOL Work::update(ULONG s, ULONG c)
{
    done = s + c;

    if( hwndProgressDlg != 0 )
    {
	BOOL	bool;

	bool = WinPostMsg(hwndProgressDlg, WUM_UPDATE,
			  MPFROMLONG(s), MPFROMLONG(c));
	assert( bool == TRUE );
    }

    APIRET	rc;
    rc = DosWaitEventSem(hevPause, SEM_INDEFINITE_WAIT);
    assert( rc == 0  ||  rc == ERROR_INVALID_HANDLE );

    if( fVerifyEnded )
	return FALSE;				/* abort update loop */
    return TRUE;
}


ULONG Work::queryPercentDone()
{
    return done * 100ul / size;
}






/* **********************************************************************
 * **** Dialog including progress bar and pause/stop ********************
 * ******************************************************************* */

PRIVATE int
InitDlgItems(HWND const hwndDlg, Work * const w)
{
    BOOL	bool;
    HWND	hwnd;

    if( FALSE == (bool = WinSetWindowPtr(hwndDlg, QWL_USER, (PVOID)w)) )
    {
	Verbose(0,modname, "WinSetWindowPtr() - error %#x",
		WinGetLastError(WinQueryAnchorBlock(hwndDlg)));
	return 1;				/* can't continue */
    }

    hwnd = WinWindowFromID(hwndDlg, SL_PROGPERCENT);
    WinSendMsg(hwnd, SLM_SETTICKSIZE, MPFROM2SHORT(SMA_SETALLTICKS,6), NULL);
    for( unsigned i = 0; i <= 5; ++i )
    {
	char	str[8];

	sprintf(str, "%u", i * 20);
	if( strlen(str) > 2 )
	    strcat(str, "   ");
	bool = (BOOL)WinSendMsg(hwnd, SLM_SETSCALETEXT,
				MPFROMSHORT(i), MPFROMP(str));
	if( FALSE == bool )
	    Verbose(0,modname, "WinSendMsg(SLM_SETSCALETEXT,) - error %#x",
		    WinGetLastError(WinQueryAnchorBlock(hwndDlg)));
    }

    w->setDlg(hwndDlg);
    return 0;
}




/*# ----------------------------------------------------------------------
 * ProgressDlgProc(hwnd,msg,mp1,mp2)
 *
 * PARAMETER
 *	hwnd,msg,mp1,mp2	as usual
 *
 * RETURNS
 *	MRESULT
 *
 * DESCRIPTION
 *	Percent Bar dialog
 *
 * REMARKS
 */
PRIVATE MRESULT EXPENTRY
ProgressDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    switch( msg )
    {
	/* WM_INITDLG:
	 *	Save caller parameter in window words and
	 *	initialize all control inside the dialog. */
      case WM_INITDLG:
	Verbose(3,modname, "WM_INITDLG, Work * = %#lx", mp2);
        {
	    Work * w = (Work *)PVOIDFROMMP(mp2);
	    if( InitDlgItems(hwnd, w) )
	    {
		w->stop();
		WinDismissDlg(hwnd, DID_ERROR);	/* abort dialog */
		return FALSE;
	    }
	}
        break;


      case WM_COMMAND:
	Verbose(4,modname, "WM_COMMAND");
	{
	    USHORT const cmd = SHORT1FROMMP(mp1);
	    Work * const work = (Work *)WinQueryWindowPtr(hwnd, QWL_USER);


	    Verbose(3,modname, "WM_COMMAND, Work * = %#lx", work);
	    switch( cmd )
	    {
	      case PB_PAUSERESUME:
		Verbose(3,modname, "WM_COMMAND, PB_PAUSERESUME");
		if( work->isPaused() )
		    work->resume();
		else
		    work->pause();
		return 0;			/* handled */

	      case DID_CANCEL:
		Verbose(3,modname, "WM_COMMAND, DID_CANCEL");
		work->stop();
		WinDismissDlg(hwnd, DID_CANCEL);
		return 0;			/* handled */

	      default:
		Verbose(2,modname, "WM_COMMAND %u", cmd);
		break;				/* ignore */
	    }
	}
	break;


      case WUM_COMPLETE:
	Verbose(4,modname, "WUM_COMPLETE");
	WinEnableWindow(WinWindowFromID(hwnd, PB_PAUSERESUME), FALSE);
	WinSetWindowText(WinWindowFromID(hwnd, DID_CANCEL), "~Close");
	WinPostMsg(hwnd, WUM_UPDATE, 0, 0);	/* update slider a last time */
	return 0;


      case WUM_UPDATE:
	Verbose(4,modname, "WUM_UPDATE");
	{
	    Work * const w = (Work *)WinQueryWindowPtr(hwnd, QWL_USER);
	    ULONG const	perc = w->queryPercentDone();


	    ULONG ul = (ULONG)WinSendDlgItemMsg(hwnd, SL_PROGPERCENT,
						SLM_QUERYSLIDERINFO,
						MPFROM2SHORT(SMA_SLIDERARMPOSITION,
							     SMA_RANGEVALUE), 0);
	    ULONG const total =  HIUSHORT(ul);	/* pixel count of entire range */
	    ULONG	pos = total * perc / 100ul;
	    if( 100 == perc )
		pos = total - 1;		/* avoid rounding */

	    if( pos != LOUSHORT(ul) )
	    {
		WinSendDlgItemMsg(hwnd, SL_PROGPERCENT,
				  SLM_SETSLIDERINFO,
				  MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_RANGEVALUE),
				  MPFROMLONG(pos));
	    }
	}
	return 0;

      default:
	break;
    }

    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}




/*# ----------------------------------------------------------------------
 * ProgressThread(dummy)
 *
 * PARAMETER
 *	dummy		Work *
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Percent Bar thread
 *
 * REMARKS
 */
PRIVATE void _Optlink
ProgressThread(PVOID dummy)
{
    HWND	dlg;
    HAB		hab;
    HMQ         hmq;
    ULONG	ul;


    Verbose(2,modname, "ProgressThread: startup (%#x)", dummy);

    hab = WinInitialize(0L);
    hmq = WinCreateMsgQueue(hab, 0L);
    WinCancelShutdown(hmq, TRUE);		/* don't want to handle it */

    dlg = WinDlgBox(HWND_DESKTOP, NULLHANDLE, ProgressDlgProc,
		    NULLHANDLE, IDW_PROGRESS, dummy);

    WinDestroyMsgQueue(hmq);
    WinTerminate(hab);

    Verbose(2,modname, "ProgressThread: ends");
    return;
}











/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * DrvVerifyThread(dummy)
 *
 * PARAMETER
 *	dummy		pointer to information from parent
 * RETURNS
 *	none
 *
 * DESPRIPTION
 *	Runs as a seperate thread.
 *
 * REMARK
 */
PUBLIC void _Optlink
DrvVerifyThread(PVOID dummy)
{
    PTHR_VERIFY const	dta = (PTHR_VERIFY)dummy;
    Work		work(dta);
    int			tidProgress = -1;
    USHORT	flthd;
    APIRET	rc;


    Verbose(2,modname, "DrvVerifyThread: startup");

    flthd = dta->drv->queryFltHd();
    if( flthd == 0xFFFF )
    {
	MyMessageBox(0, HWND_DESKTOP,
		     "The array you selected isn't known to"
		     " vraid.flt.  Therefore it can't be verified"
		     " by the driver.  Please reboot to let"
		     " vraid.flt analyze the new situation and"
		     " retry the operation.");
	goto RETURN;				/* !!!	sorry */
    }

    do
    {

	VRAID_ARRAYINFO_DATA	arrayinfo;
	rc = DriverArrayinfo(flthd, &arrayinfo);
	if( rc != 0 )
	{
	    Verbose(1,modname, "DriverArrayinfo() failed - rc %lu", rc);
	    break;
	}
	if( arrayinfo.cb < FIELDOFFSET(VRAID_ARRAYINFO_DATA,flags) )
	{
	    MyMessageBox(1, HWND_DESKTOP,
			 "You are using an older version of vraid.flt.  Please"
			 " update to the one released together with this"
			 " version of vsetup.exe (or newer).");
	    break;
	}
	if( (arrayinfo.flags & 0x0F) != 0 )
	{
	    MyMessageBox(1, HWND_DESKTOP,
			 "The array you selected is not in a state to be verified."
			 "\nPlease wait until any build or rebuild processes have"
			 " ended.");
	    break;
	}



	Verbose(2,modname, "starting ProgressThread with %#lx", &work);
	tidProgress = _beginthread(ProgressThread, NULL, STACK_SIZE, &work);
	if( tidProgress == -1 )
	{
	    Verbose(1,modname,
		    "_beginthread(ProgressThread) failed - errno %u", errno);
	    break;
	}

	ULONG	sec = 0;
	ULONG	cnt;
	ULONG const	vd_cb = (sizeof(VRAID_VERIFY_DATA) - 1
				 + VERIFY_SECTORS * SECTOR_SIZE);
	ULONG const	cb = (sizeof(VRAID_ARRAYINFO_DATA)
			      + arrayinfo.children * vd_cb);
	PUCHAR		buffer = new UCHAR[cb];
	PVRAID_VERIFY_DATA * pvd = new PVRAID_VERIFY_DATA[arrayinfo.children];

	for( USHORT i = 0; i < arrayinfo.children; ++i )
	    pvd[i] = (PVRAID_VERIFY_DATA)&buffer[sizeof(VRAID_ARRAYINFO_DATA)
						+ i * vd_cb];

	do
	{
	    if( sec + VERIFY_SECTORS >= arrayinfo.size )
		cnt = arrayinfo.size - sec;
	    else
		cnt = VERIFY_SECTORS;

	    VRAID_VERIFY_PARM	param;
	    param.handle = flthd;
	    param.secno = sec;
	    param.seccnt = cnt;
	    rc = DriverArrayVerify(&param, buffer, cb);
	    if( rc != 0 )
	    {
		Verbose(1,modname, "DriverArrayVerify() failed - rc %lu", rc);
		break;
	    }


	    /* Analyse returned data.
	     * xxx	Always set child as invalid even if user doesn't want
	     * to rebuild?  Looks a bit dangorous to me... */

	    PDEVID	bad_id = new DEVID[arrayinfo.children];
	    USHORT	bad_cnt = 0;
	    for( i = 0; i < arrayinfo.children; ++i )
	    {
		if( (pvd[i]->flags & VVDF_CONFIRMED) == 0 )
		{
		    Verbose(0,modname, "Child %u is not confirmed", i);
		    memcpy(&bad_id[bad_cnt], pvd[i]->id, sizeof(DEVID));
		    ++bad_cnt;
		}
	    }
	    if( bad_cnt != 0 )
	    {
		PSZ str = new char[200 + bad_cnt * 18];

		sprintf(str,
			"The verify of sectors %lu to %lu failed on %u part(s)"
			" of the array!\nDo you want to declare the following"
			" devices as invalid and start a build or cancel the"
			" verify (or answer 'NO' to continue the verify)?",
			param.secno, param.secno + param.seccnt - 1, bad_cnt);
		for( i = 0; i < bad_cnt; ++i )
		    sprintf(&str[strlen(str)], "\n\tID %02X%02X%02X%02X%02X%02X",
			    pvd[i]->id[0],pvd[i]->id[1],pvd[i]->id[2],
			    pvd[i]->id[3],pvd[i]->id[4],pvd[i]->id[5]);

		ULONG res = WinMessageBox(HWND_DESKTOP, NULLHANDLE,
					  str, "Verify failed...",
					  0xFFFF, MB_WARNING|MB_YESNOCANCEL);
		switch( res )
		{
		  case MBID_YES:
		    for( i = 0; i < bad_cnt; ++i )
		    {
			dta->drv->ioInvalidChild(pvd[i]->id, param.secno);
		    }

		    /* fall through... */

		  case MBID_CANCEL:
		    goto ABORT_VERIFY;		/* sorry, no better way */


		  case MBID_NO:
		    break;			/* do nothing and continue */

		  default:
		    assert( res == -1 );	/* will fail! */
		}
		delete[] str;
	    }
	    delete[] bad_id;


	    /* Next sector range. */

	    BOOL bool = work.update(sec, cnt);
	    sec += cnt;
	    if( bool == FALSE )
		break;				/* user abort */
	}
	while( sec < arrayinfo.size );
      ABORT_VERIFY:
	delete[] pvd;
	delete[] buffer;

	work.waitOK();
    }
    while(0);


    work.abortDlg();				/* if still displayed... */
  RETURN:
    Verbose(2,modname, "DrvVerifyThread: ends");
    WinPostMsg(dta->parent, WUM_THREADENDED, dta, 0);
    return;
}
