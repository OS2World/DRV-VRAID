/*
 * $Source: R:/Source/driver/raid/Setup/RCS/VSetup.cpp,v $
 * $Revision: 1.18 $
 * $Date: 2000/11/10 02:13:49 $
 * $Locker:  $
 *
 *	Main Module to vsetup.exe
 *
 * $Log: VSetup.cpp,v $
 * Revision 1.18  2000/11/10 02:13:49  vitus
 * - Version 0.84
 * - Logfile (and MLE) gets \r\n
 *
 * Revision 1.17  2000/06/13 03:37:33  vitus
 * - changes to use QWL_USER constant as window word
 *
 * Revision 1.16  2000/05/28 17:44:33  vitus
 * - adjusted verbosity levels
 *
 * Revision 1.15  2000/05/15 22:58:35  vitus
 * - added calls to PPSubclassWindow(), saves presentation parameters
 *
 * Revision 1.14  2000/04/15 23:47:43  vitus
 * - version 0.82, this should be the next release (vraid22)
 *
 * Revision 1.14  2000/04/15 23:45:47  vitus
 * - version 0.81, this should be the next release (vraid22)
 *
 * Revision 1.13  2000/03/09 23:43:31  vitus
 * - accepts and handles option "-l": log messages to file
 *
 * Revision 1.12  1999/11/27 23:02:07  vitus
 * - version 0.81
 *
 * Revision 1.11  1999/06/20 17:25:40  vitus
 * - version 0.80
 * - menuentry to enable/disable driver beeping
 * - this module sets SM_FLTAVAIL in fSetupMode, others have to test
 *
 * Revision 1.10  1999/04/30 18:49:28  vitus
 * - version 0.71
 *
 * Revision 1.9  1999/03/24 23:48:11  vitus
 * - version 0.70
 *
 * Revision 1.8  1999/02/09 02:53:16  vitus
 * - switched to C++
 *
 * Revision 1.7  1998/12/22 00:29:08  vitus
 * - added PUBLIC 'fSetupMode' and 'szProgName'.  Data every module may read
 * - changed as global headers moved to /source/Lib
 * - version 0.60
 *
 * Revision 1.6  1998/11/21 20:05:55  vitus
 * - versin 0.52
 *
 * Revision 1.5  1998/11/05 00:24:54  vitus
 * - switched to version 0.51
 *
 * Revision 1.4  1998/09/28 00:55:32  vitus
 * - disables File menu
 * - disables Driver menu if no driver is installed
 *
 * Revision 1.3  1998/09/27 00:14:31  vitus
 * - added 'driver' to menu bar
 * - added code to display driver version and startup messages
 *
 * Revision 1.2  1998/06/09 01:37:56  vitus
 * - WM_CLOSE, WM_DESTROY
 * - corrected position of MLE inside client window
 * - reenables "drive setup" menue entry
 * - added command line options
 *
 * Revision 1.1  1998/04/30 01:28:54  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2000
 */
static char const vcid[]="$Id: VSetup.cpp,v 1.18 2000/11/10 02:13:49 vitus Exp $";

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <process.h>

#define __IBMC__	0
#define INCL_DOSDEVIOCTL
#define INCL_WIN
#include <os2.h>

extern "C" {
#include "../../../Lib/defines.h"
#include "../../../Lib/verbose.h"
#include "../../../Lib/profile.h"		/* profile handling */
#include "../../../Lib/pphnd.h"			/* presparam handling */
}
#include "resources.h"
#include "vsetup.h"
#include "DrvSetup.h"
#include "drvif.h"


/*
 * Moved these dialog procedures to seperate files.
 */
extern MRESULT EXPENTRY AboutDialogProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2);
extern MRESULT EXPENTRY InilocDialogProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2);
extern MRESULT EXPENTRY DumpDialogProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2);



#define VER		0
#define SUBVER		84

#define WC_MAIN		"MainWindow"



/*
 * Pointer to this structure saved in window words of main window
 */
typedef struct {
    HAB		hab;
    HWND	hwndMle;
} MAIN_WINDOW_DATA, * PMAIN_WINDOW_DATA;






/* **********************************************************************
 * **** Public Variables ************************************************
 * ******************************************************************* */

PUBLIC USHORT		fSetupMode = 0;
PUBLIC char		szPrgName[_MAX_FNAME];	/* basename of executable */






/* **********************************************************************
 * **** Private Data ****************************************************
 * ******************************************************************* */

PRIVATE char		szPrgDir[_MAX_PATH];	/* location of executable  */
PRIVATE HWND		hwndClient = 0;		/* MLE */
PRIVATE unsigned	uLogLevel = 0;		/* only fatal errors */
PRIVATE HFILE		hdLogFile = 0;		/* no logfile... */
PRIVATE char const	szMleFirst[] = "Startup";






/* **********************************************************************
 * **** Exported routines ***********************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * Verbose(level,module,fmt,...)
 *
 * PARAMETER
 *	level		needed verbosity level
 *	module
 *	fmt		format string (see printf)
 * RETURNS
 *	nothing
 * GLOBAL
 *	hwndClient	handle of client window of main window
 * DESPRIPTION
 *	Post complete string to hwndClient if level is below or 
 *	equal to current verbosity level.
 *	Allocates room for string on the heap, window routine
 *	has to free it.
 * REMARKS
 *	exported routine
 */
PUBLIC VOID
Verbose(unsigned level,char const * module,char const * fmt,...)
{
    if( level <= uLogLevel )
    {
	char * const	str = new char[4000];
	va_list		argp;
	int		i;

	i = sprintf(str, "\r\n%s - ", module);	/* \n OK?	xxx */
	va_start(argp,fmt);
	vsprintf(&str[i], fmt, argp);
	va_end(argp);

	if( hdLogFile != 0 )
	{
	    ULONG	ul;

	    DosWrite(hdLogFile, str, strlen(str), &ul);
	}
	WinPostMsg(hwndClient, WUM_ADDLOG, MPFROMP(str), 0);
    }
}




/*# ----------------------------------------------------------------------
 * SetLoglevel(new)
 *
 * PARAMETER
 *	new		new log level (0-5)
 * RETURNS
 *	old log level
 * GLOBAL
 *	uLogLevel (modul global)
 * DESPRIPTION
 *	Sets global variable and returns old value.
 * REMARKS
 *	exported routine
 */
PUBLIC unsigned
SetLoglevel(unsigned newlevel)
{
    unsigned old = uLogLevel;

    uLogLevel = newlevel;
    return old;
}




/*# ----------------------------------------------------------------------
 * SetLogfile(newfile)
 *
 * PARAMETER
 *	new		new log file handle
 *
 * RETURNS
 *	old log file handle
 *
 * DESPRIPTION
 *	Sets global variable and returns old value.
 *
 * REMARKS
 */
PUBLIC HFILE
SetLogfile(HFILE newfile)
{
    HFILE old = hdLogFile;
    hdLogFile = newfile;
    return old;
}






/* **********************************************************************
 * **** Local routines **************************************************
 * ******************************************************************* */

PRIVATE VOID
DisplayDriverVersion(VOID)
{
    USHORT	ver, us;
    APIRET	rc;

    if( (rc=DriverGetVersion(&ver, &us)) != 0 )
	Verbose(0, "VSETUP", "Can't read driver version - error %lu",rc);
    else
	Verbose(0, "VSETUP", "Driver Version %u.%02u installed",
		HIBYTE(ver), LOBYTE(ver));
    return;
}




PRIVATE VOID
DisplayDriverMessages(VOID)
{
    char	* buffer;
    APIRET	rc;

    if( (rc=DriverReadMessages((PUCHAR *)&buffer)) != 0 )
	Verbose(0, "VSETUP", "Can't read driver messages - error %lu", rc);
    else
    {
	unsigned i, l;

	for( l = i = 0; buffer[i] != '\0'; i += strlen(&buffer[i])+1, ++l )
	    Verbose(0, "VRAID", "Line %u: %s", l, &buffer[i]);
	delete[] buffer;
    }
    return;
}




/*# ----------------------------------------------------------------------
 * CALL
 *	WindowProc(hwnd,msg,mp1,mp2)
 * PARAMETER
 *	hwnd,msg,mp1,mp2	see presentation manager reference
 * RETURNS
 *	MRESULT			see presentation manager reference
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Window procedure
 * REMARKS
 */
PRIVATE MRESULT EXPENTRY
WindowProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
    MAIN_WINDOW_DATA * const windata = (PMAIN_WINDOW_DATA)WinQueryWindowPtr(hwnd,
									   QWL_USER);
    static int	regular = FALSE;		/* in message loop? */
    BOOL	bool;

    switch( msg )
    {
	/* Window created but not yet visible */
      case WM_CREATE:
	Verbose(4, "VSETUP", "WM_CREATE");
	break;


	/* WM_CLOSE	-	this window closed
	 *	Stop all further processing, cleanup.	
	 */
      case WM_CLOSE:
	Verbose(4, "VSETUP", "WM_CLOSE");
	break;					/* continue... */


	/* WM_DESTROY	-	this window destroyed
	 *	This message may be posted after WM_CLOSE or without
	 *	any WM_CLOSE (if user close per windowlist).
	 *	So do here the same as in WM_CLOSE (if possible).
	 */
      case WM_DESTROY:
	Verbose(4, "VSETUP", "WM_DESTROY");
	break;


	/* WUM_START:
	 *	window configuration ended, application enters
	 *	message loop. All further messages are caused
	 *	by the user.
	 */
      case WUM_START:
	Verbose(3, "VSETUP", "WUM_START");
	regular = TRUE;
	return 0;


	/* WM_SAVEAPPLICATION
	 *	Save current window position, presentation parameters,
	 *	etc. for next run.
	 */
      case WM_SAVEAPPLICATION:
	Verbose(3, "VSETUP", "WM_SAVEAPPLICATION");
	ProfileClose(windata->hab);		/* there will be no more */
	break;					/* run default processing (see dox)*/

 
	/* WM_SIZE
	 *	Window is resized.
	 *	If MLE already created -> resize MLE
	 */
      case WM_SIZE:
	Verbose(4, "VSETUP", "WM_SIZE");
	{
	    SWP		size;

	    if( windata != NULL )
	    {
		if( regular == TRUE
		    &&  WinQueryWindowPos(WinQueryWindow(hwnd, QW_PARENT),
					  &size) == TRUE )
		    ProfileWrite(WC_MAIN, "Size", sizeof(size), &size);

		if( WinQueryWindowPos(hwnd, &size) == TRUE )
		    WinSetWindowPos(windata->hwndMle, HWND_TOP,
				    0, 0, size.cx, size.cy, SWP_SIZE|SWP_MOVE);
	    }
	}
	break;


	/* MW_MOVE:
	 *	Parent window has moved, record new position in profile.
	 */
      case WM_MOVE:
	Verbose(4, "VSETUP", "WM_MOVE");
	{
	    SWP	swp;

	    if( regular == TRUE
	       &&  WinQueryWindowPos(WinQueryWindow(hwnd, QW_PARENT),
				     &swp) == TRUE )
	    {
		ProfileWrite(WC_MAIN, "Position", sizeof(swp), &swp);
	    }
	}
	break;					/* continue... */


	/* WUM_ADDLOG	-	add string to log (MLE)
	 *
	 * There is a problem: I have to import all characters
	 * including '\0' to the MLE to get '\n' generate a newline.
	 * Unfortunately this '\0' will be displayed (as blank). To fix
	 * this the 'lOffset' value is decremented after importing; now
	 * all '\0's are collected at end of text...
	 */
      case WUM_ADDLOG:
	{
	    char * str = (char *)PVOIDFROMMP(mp1);
	    static IPT lOffset = sizeof(szMleFirst);	/* updated by MLE */

	    ULONG const cbBytes = strlen(str);
	    ULONG	cb;

	    if( windata == NULL )
		fprintf(stderr, str);		/* no MLE created */
	    else
	    {
		bool = (BOOL)WinSendMsg(windata->hwndMle, MLM_SETIMPORTEXPORT,
					mp1, MPFROMSHORT(cbBytes));
		if( bool == FALSE )
		    fprintf(stderr, str);
		else
		{
		    cb = (ULONG)WinSendMsg(windata->hwndMle, MLM_IMPORT,
					   MPFROMP(&lOffset),
					   MPFROMSHORT(cbBytes));
		    if( cb != cbBytes )
			fprintf(stderr,"mismatch: %lu of %lu imported\n",
				cb, cbBytes);
		}
	    }
	    delete[] str;				/* dynamically allocated! */
	}
	return 0;


	/* WUM_THREADENDED:
	 *	Some other thread ended.
	 */
      case WUM_THREADENDED:
	if( 1 )					/* only one thread started! */
	{
	    HWND const hwndFrame = WinQueryWindow(hwnd, QW_PARENT);
	    HWND const hwndMenu = WinWindowFromID(hwndFrame, FID_MENU);

	    Verbose(4, "VSETUP", "hwndFrame %#x", hwndFrame);
	    Verbose(4, "VSETUP", "hwndMenu %#x", hwndMenu);
	    bool = WinEnableMenuItem(hwndMenu, IDM_DRVSETUP, TRUE);
	    if( bool == FALSE )
		Verbose(1, "VSetup",
			"WinEnableMenuItem failed (last error %#x)",
			WinGetLastError(windata->hab));
	}
	return 0;


	/* WM_COMMMAND
	 *	Commands selected by menu bar.
	 */
      case WM_COMMAND:
	{
	    USHORT	cmd = SHORT1FROMMP(mp1);

	    Verbose(3, "VSETUP", "WM_COMMAND: %#x",cmd);
	    switch( cmd )
	    {
	      case IDM_VERSION:
		DisplayDriverVersion();
		return 0;

	      case IDM_MSGDUMP:
		DisplayDriverMessages();
		return 0;

	      case IDM_NOBEEP:
	      {
		  HWND const	hwndFrame = WinQueryWindow(hwnd, QW_PARENT);
		  HWND const	hwndMenu = WinWindowFromID(hwndFrame, FID_MENU);
		  BOOL		checked;

		  checked = WinIsMenuItemChecked(hwndMenu, IDM_NOBEEP);
		  DriverSetBeeper((USHORT)checked);
		  checked = !checked;
		  WinCheckMenuItem(hwndMenu,IDM_NOBEEP, checked);
	      }
	      return 0;

	      case IDM_DRVSETUP:
		{
		    int		tid;
		    PTHR_DRVSETUP	dta = new THR_DRVSETUP;

		    dta->parent = hwnd;
		    tid = _beginthread(DrvsetupThread, NULL, STACK_SIZE, dta);
		    if( tid == -1 )
			Verbose(1, "VSETUP",
				"Couldn't create DrvsetupThread - error %d", errno);
		    else
		    {
			HWND const hwndFrame = WinQueryWindow(hwnd, QW_PARENT);
			HWND const hwndMenu = WinWindowFromID(hwndFrame, FID_MENU);

			Verbose(3, "VSETUP", "hwndFrame %#x", hwndFrame);
			Verbose(3, "VSETUP", "hwndMenu %#x", hwndMenu);
			bool = WinEnableMenuItem(hwndMenu, IDM_DRVSETUP, FALSE);
			if( bool == FALSE )
			    Verbose(1, "VSetup",
				    "WinEnableMenuItem failed (last error %#x)",
				    WinGetLastError(windata->hab));
			Verbose(2, "VSETUP", "created thread id %d", tid);
		    }
		}
		return 0;

	      case IDM_DRVDUMP:
	      {
		  ULONG result;

		  result = WinDlgBox(HWND_DESKTOP, hwnd, DumpDialogProc,
				     NULLHANDLE, IDW_DRVDUMP, (PVOID)hwnd);
		  Verbose(2, "VSETUP", "WinDlgBox(IDW_DRVDUMP) returned %lu "
			  "- last error %#x",
			  result, WinGetLastError(windata->hab));
	      }
	      return 0;

	      case IDM_SAVE:
		return 0;

	      case IDM_CLEAR:
		{
		    IPT	textlen;

		    textlen = (IPT)WinSendMsg(windata->hwndMle,
					      MLM_QUERYTEXTLENGTH, 0, 0);
		    WinSendMsg(windata->hwndMle, MLM_DELETE,
			       0, MPFROMLONG(textlen));
		}
		return 0;

	      case IDM_COPY:
		WinSendMsg(windata->hwndMle, MLM_COPY, 0, 0);
		return 0;

	      case IDM_ABOUT:
		{
		    USHORT version = (SUBVER << 8) | VER;
		    ULONG result;

		    result = WinDlgBox(HWND_DESKTOP, hwnd, AboutDialogProc,
				       NULLHANDLE, IDW_ABOUT, &version);
		    Verbose(2, "VSETUP", "WinDlgBox(IDW_ABOUT) returned %lu "
			    "- last error %#x",
			    result, WinGetLastError(windata->hab));
		}
		return 0;

	      default:
		break;
	    }
	}
	break;					/* continue with default? */


      case WM_ERASEBACKGROUND:
	return (MRESULT)TRUE;


      default:
	break;					/* continue... */
    }

    return WinDefWindowProc(hwnd, msg, mp1, mp2);
}




/*# ----------------------------------------------------------------------
 * CALL
 *	ExamineArgs(argc,argv)
 * PARAMETER
 *	argc,argv	from main()
 * RETURNS
 *	0		OK
 *	/0		stop running
 * GLOBAL
 *	(some)
 * DESPRIPTION
 * REMARKS
 */
PRIVATE int
ExamineArgs(int argc,char *argv[])
{
    char	drv[_MAX_DRIVE], dir[_MAX_DIR];
    char	ext[_MAX_EXT];

    _splitpath(argv[0], drv, dir, szPrgName, ext);
    _makepath(szPrgDir, drv, dir, "", "");

    while( argc > 1  &&  argv[1][0] == '-' )
    {
	switch( argv[1][1] )
	{
	  case 'q':
	    SetLoglevel(VL_FATAL);
	    break;

	  case 'v':
	    if( argv[1][2] == ':' )
	    {
		int newlevel = argv[1][3] - '0';

		Verbose(0, "VSETUP", "New log level %d", newlevel);
		SetLoglevel(newlevel);
	    }
	    else
		SetLoglevel(2);
	    break;

	  case 'l':
	    if( argv[1][2] == ':' )
	    {
		APIRET	rc;
		HFILE	hf;
		ULONG	ul;

		rc = DosOpen(&argv[1][3], &hf, &ul,
			     0, FILE_NORMAL,
			     OPEN_ACTION_CREATE_IF_NEW|OPEN_ACTION_OPEN_IF_EXISTS,
			     OPEN_FLAGS_WRITE_THROUGH|OPEN_FLAGS_FAIL_ON_ERROR
			     |OPEN_FLAGS_SEQUENTIAL|OPEN_SHARE_DENYWRITE
			     |OPEN_ACCESS_WRITEONLY,
			     NULL);
		if( rc == 0 )
		{
		    DosSetFilePtr(hf, 0, FILE_END, &ul);
		    SetLogfile(hf);
		}
	    }
	    break;

	  default:
	    Verbose(0, "VSETUP", "unknown parameter \'%c\'", argv[1][1]);
	    return 1;
	}
	--argc;
	++argv;
    }
    return 0;
}




/*# ----------------------------------------------------------------------
 * CALL
 *	main(argc,argv)
 * PARAMETER
 *	argc,argv		commandline contents
 * RETURNS
 *	0			OK
 *	/0			fatal error occured
 * GLOBAL
 *	(many)
 * DESCRIPTION
 *	Creates main window, opens INI-file (filename stored in os2.ini
 *	or specified by user), modifies window parameters, ...
 *
 * REMARKS
 */
int
main(int argc,char *argv[])
{
    MAIN_WINDOW_DATA *windata = new MAIN_WINDOW_DATA;
    int		rc = 0;
    BOOL	bool;
    HMQ 	hmq;
    HWND	hwndFrame;
    ULONG	flCreate;
    QMSG	qmsg;

    SetLoglevel(1);
    if( windata == NULL )
    {
	fprintf(stderr, "VSETUP\tmain - memory problem\n");
	return 1;
    }
    if( ExamineArgs(argc, argv) != 0 )
	return 2;

    do
    {
	if( !(windata->hab = WinInitialize(0)) )
	{
	    Verbose(0, "VSETUP", "WinInitialize failed!?!");
	    rc = 1;
	    break;
	}

	if( !(hmq = WinCreateMsgQueue(windata->hab, 0)) )
	{
	    Verbose(0, "VSETUP", "WinCreateMsgQueue - error %#x",
		    WinGetLastError(windata->hab));
	    rc = 2;
	    break;
	}

	if( !WinRegisterClass(windata->hab, WC_MAIN, WindowProc,
			      CS_MOVENOTIFY, sizeof(PVOID)) )
	{
	    Verbose(0, "VSETUP", "WinRegisterClass - error %#x",
		    WinGetLastError(windata->hab));
	    rc = 3;
	    break;
	}

	flCreate = FCF_SIZEBORDER | FCF_TASKLIST | FCF_SYSMENU | FCF_TITLEBAR
	    | FCF_ICON | FCF_MENU | FCF_MINMAX;

	hwndFrame = WinCreateStdWindow(HWND_DESKTOP, WS_ANIMATE, &flCreate,
				       WC_MAIN, "VSetup - Main Window",
				       0, 0,
				       IDW_MAIN, &hwndClient);

	if( !hwndFrame || !hwndClient )
	{
	    Verbose(0, "VSETUP", "WinCreateStdWindow - error %#x",
		    WinGetLastError(windata->hab));
	    rc = 4;				/* couldn't create window for some
						   reason. */
	    break;
	}

	/* Configure element of window or add more elements to window.
	 *
	 * First: create MLE in client area */
	{
	    SWP  size;

	    WinQueryWindowPos(hwndClient, &size);
	    Verbose(3, "VSETUP", "client window %u,%u %u,%u",
		    size.x, size.y, size.cx, size.cy);

	    windata->hwndMle = WinCreateWindow(hwndClient, WC_MLE, szMleFirst,
					       WS_VISIBLE
					       |MLS_HSCROLL
					       |MLS_VSCROLL|MLS_READONLY,
					       size.x, size.y, size.cx, size.cy,
					       hwndClient, HWND_TOP,
					       IDW_LOGBOOK, NULL, NULL);
	    if( windata->hwndMle == NULLHANDLE )
	    {
		MyMessageBox(0, HWND_DESKTOP,
			     "WinCreateWindow(...,WC_MLE,...) - error %#x",
			     WinGetLastError(windata->hab));
		rc = 5;
		break;
	    }

	    bool = WinSetWindowPtr(hwndClient, QWL_USER, windata);
	    if( bool == FALSE )
	    {
		MyMessageBox(0, HWND_DESKTOP,
			     "WinSetWindowPtr failed - error %#",
			     WinGetLastError(windata->hab));
		rc = 6;
		break;
	    }
	    WinPostMsg(windata->hwndMle, MLM_RESETUNDO, NULL, NULL);
	}


	/* Second: update menu items, etc. */
	{
	    HWND hwndMenu = WinWindowFromID(hwndFrame, FID_MENU);

	    WinEnableMenuItem(hwndMenu, IDM_CUT, FALSE);
	    WinEnableMenuItem(hwndMenu, IDM_PASTE, FALSE);

#if defined(_BETA_PKG)
	    WinEnableMenuItem(hwndMenu, IDM_FILE, FALSE);
#endif

	    /* Try to open vraid.flt and modify driver menu
	     * according to result. */

	    if( DriverOpen() != 0 )
	    {
		WinEnableMenuItem(hwndMenu, IDM_DRIVER, FALSE);
	    }
	    else
	    {
		fSetupMode |= SM_FLTAVAIL;

		USHORT	ver, flags;
		DriverGetVersion(&ver, &flags);
		if( (flags & 0x20) )
		    WinCheckMenuItem(hwndMenu,IDM_NOBEEP, 1);
	    }
	}


	/* Retrieve path and name of Vsetup.ini file. Try
	 * OS2.INI first. If this fails ask user. */
	{
	    char	drv[_MAX_DRIVE], dir[_MAX_DIR];
	    char	fname[_MAX_FNAME], ext[_MAX_EXT];
	    char	path[_MAX_PATH];
	    char	inifile[_MAX_PATH];
	    ULONG	length = sizeof(path);

	    length = PrfQueryProfileString(HINI_USERPROFILE,
					   INI_APPNAME, "INI Path",
					   NULL,	/* no default value */
					   path,	/* place here */
					   length);

	    if( length == 0  ||  CheckValidDir(hwndFrame, path) )
	    {
		ULONG result;

		strcpy(path, szPrgDir);		/* default value */
		result = WinDlgBox(HWND_DESKTOP, hwndFrame, InilocDialogProc,
				   NULLHANDLE, IDW_INILOCATION, path);
		Verbose(2, "VSETUP", "WinDlgBox(IDW_INILOCATION) returned %lu "
			"- last error %#x",result, WinGetLastError(windata->hab));
		if( result != DID_OK )
		{
		    Verbose(0, "VSETUP",
			    "Sorry, can't continue (no directory to place ini)");
		    return 4;
		}

		/* Write to OS2.INI, ignore any errors. */

		PrfWriteProfileString(HINI_USERPROFILE,
				      INI_APPNAME, "INI Path", path);
	    }

	    _splitpath(path, drv, dir, fname, ext);
	    strcat(dir, fname ),	strcat( dir, ext);
	    _makepath(inifile, drv, dir, szPrgName, "ini");
	    ProfileOpen(windata->hab, inifile);
	}


	/* Restore previous window state as close as possible. Last call
	 * in this block will also display the now invisible main window. */

	PPSubclassWindow(windata->hwndMle, WC_MAIN);
	PPSubclassWindow(WinWindowFromID(hwndFrame, FID_MENU), WC_MAIN);
	PPSubclassWindow(WinWindowFromID(hwndFrame, FID_TITLEBAR), WC_MAIN);
	PPSubclassWindow(WinWindowFromID(hwndFrame, FID_SYSMENU), WC_MAIN);

	{
	    SWP		swp, tmp;
	    unsigned	cb;

	    WinQueryTaskSizePos(windata->hab, 0, &swp); /* initialize 'swp' */

	    cb = sizeof(tmp);
	    if( ProfileRead(WC_MAIN, "Size", &cb, &tmp) == 0 )
	    {
		Verbose(3, "VSETUP", "Copying old size of main window");
		swp.cx = tmp.cx;
		swp.cy = tmp.cy;
		swp.fl |= SWP_SIZE;
	    }
	    cb = sizeof(tmp);
	    if( ProfileRead(WC_MAIN, "Position", &cb, &tmp) == 0 )
	    {
		Verbose(3, "VSETUP", "Copying old position of main window");
		swp.x = tmp.x;
		swp.y = tmp.y;
		swp.fl |= SWP_MOVE;
	    }
	    WinSetWindowPos(hwndFrame, HWND_TOP, swp.x, swp.y, swp.cx, swp.cy,
			    SWP_SIZE | SWP_MOVE | SWP_SHOW | SWP_ACTIVATE);
	}


	/* All done, enable window (WUM_START) and start processing messages. */

	bool = WinPostMsg(hwndClient, WUM_START, 0, 0);
	while( WinGetMsg(windata->hab, &qmsg, 0L, 0, 0) )
	    WinDispatchMsg(windata->hab, &qmsg);

	/* Message processing ended (WM_QUIT sent), clean up. */

	DriverClose();				/* ignore errors */

	WinDestroyWindow(hwndFrame);
	WinDestroyMsgQueue(hmq);
	WinTerminate(windata->hab);
	delete windata;
    }
    while(0);

    return rc;
}
