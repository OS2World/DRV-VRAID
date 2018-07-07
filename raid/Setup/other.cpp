/*
 * $Source: R:/Source/driver/raid/Setup/RCS/other.cpp,v $
 * $Revision: 1.5 $
 * $Date: 2000/04/10 01:05:08 $
 * $Locker:  $
 *
 *	Small helper routines.
 *
 * $Log: other.cpp,v $
 * Revision 1.5  2000/04/10 01:05:08  vitus
 * - implemented GUI _assert() routine: replaces VAC++ code
 *
 * Revision 1.4  1999/06/20 17:05:26  vitus
 * - minor corrections to keep compiler happy
 *
 * Revision 1.3  1999/02/09 02:52:30  vitus
 * - switched to C++
 *
 * Revision 1.2  1998/12/13 22:37:39  vitus
 * - changed as global headers moved to /source/Lib
 *
 * Revision 1.1  1998/05/15 22:47:15  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2000
 */
static char const vcid[]="$Id: other.cpp,v 1.5 2000/04/10 01:05:08 vitus Exp $";

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <process.h>

#define __IBMC__	0
#define INCL_WIN
#define INCL_DOSERRORS
#include <os2.h>

extern "C" {
#include "../../../Lib/defines.h"
#include "../../../Lib/verbose.h"
#include "resources.h"
}
#include "vsetup.h"




/*# ----------------------------------------------------------------------
 * ChangeWindowTitle(hwndFrame,title)
 *
 * PARAMETER
 *	hwndFrame	handle of window
 *	title		new title
 *
 * RETURNS
 *	(nothing)
 *
 * DESPRIPTION
 *	Changes window title in window list and on window frame.
 *
 * REMARKS
 */
PUBLIC void
ChangeWindowTitle(HWND const hwndFrame,char const * const title)
{
    HSWITCH hswitch;
    HWND hwndWindowHandle;

    if( (hswitch=WinQuerySwitchHandle(NULLHANDLE, getpid())) != NULLHANDLE )
    {
	SWCNTRL swctl;

        swctl.szSwtitle[0] = '\0';

        WinQuerySwitchEntry(hswitch, &swctl);
        hwndWindowHandle = swctl.hwnd;

	strcpy(swctl.szSwtitle, title);

        WinChangeSwitchEntry(hswitch, &swctl);
        WinSetWindowText(hwndWindowHandle, (PSZ)title);
    }
    UNREFERENCED(hwndFrame);
}




/*# ----------------------------------------------------------------------
 * MyMessageBox(level,owner,fmt,...)
 *
 * PARAMETER
 *	level		error level
 *	owner		owner window
 *	fmt		like printf
 *
 * RETURNS
 *	(nothing)
 *
 * DESPRIPTION
 *	Displays a message box with styles according to 'level'.
 *
 * REMARKS
 */
PUBLIC void
MyMessageBox(int const level,HWND const owner,char *fmt,...)
{
    static char const * const pszTitle[] = { "FATAL", "Error",
						 "Warning", "Other", ""};
    static ULONG flStyle[] = { MB_ENTER|MB_ERROR,
			       MB_ENTER|MB_ERROR,
			       MB_ENTER|MB_WARNING,
			       MB_ENTER|MB_INFORMATION,
			       MB_ENTER };
    char   str[1000];
    va_list argp;

    va_start(argp, fmt);
    vsprintf(str, fmt, argp);
    va_end(argp);

    WinMessageBox(HWND_DESKTOP, owner, str, (PSZ)pszTitle[level],
		  0xFFFF, flStyle[level]);
}




/*# ----------------------------------------------------------------------
 * CheckValidDir(hwnd,dir)
 *
 * PARAMETER
 *	hwnd		use as owner of any message box
 *	dir		is this a directory?
 *
 * RETURNS
 *	0		OK, dir is a directory (may be newly created)
 *	/0		no directory, don't start test
 *
 * DESPRIPTION
 *	Check whether 'suspect_dir' describes an existent directory
 *	or a directory which may be valid if created.
 *	In second case ask user to create that directory.
 *
 * REMARKS
 */
PUBLIC int
CheckValidDir(HWND const hwnd,char const * const suspect_dir)
{
    APIRET	rc;
    FILESTATUS3 buf;
    char	path[_MAX_PATH];

    if( strlen(suspect_dir) == 0 )
    {
	MyMessageBox(1, hwnd, "missing directory");
	return 1;
    }
    strcpy(path, suspect_dir);			/* further processing w/ path */
    if( strlen(path) == 1 )
	;
    else if( strlen(path) == 3  &&  path[strlen(path)-2] == ':' )
	;
    else if( path[strlen(path)-1] == '\\'  ||  path[strlen(path)-1] == '/' )
	path[strlen(path)-1] = '\0';

    rc = DosQueryPathInfo(path, FIL_STANDARD, (PVOID)&buf, sizeof(buf));
    if( rc == ERROR_PATH_NOT_FOUND  || rc == ERROR_FILE_NOT_FOUND )
    {
	char	str[_MAX_PATH];
	USHORT  resp;

	sprintf(str, "Directory %s not existent! Do you want to create it?",
		path );
	resp = (USHORT)WinMessageBox(HWND_DESKTOP, hwnd,
				     str, "Directory", 0, MB_YESNO);
	if( resp == MBID_NO )
	    return 1;
	rc = DosCreateDir( path, NULL );
	if( rc )
	{
	    MyMessageBox(1, hwnd, "Error %#x creating %s", rc, path);
	    return 1;
	}
	return 0;
    }
    else if( rc )
    {
	Verbose(1, "CheckValidDir", "DosQueryPathInfo(%s) - error %#x", path, rc);
	MyMessageBox(1, hwnd, "Error %#x checking %s", rc, path);
	return 1;
    }
    else if( !(buf.attrFile & FILE_DIRECTORY) )
    {
	Verbose(2, "CheckValidDir", "DosQueryPathInfo(%s) - attribute %#x",
		path, buf.attrFile);
	MyMessageBox(1, hwnd, "%s is not a directory", path);
	return 1;
    }
    Verbose(2, "CheckValidDir", "%s _is_ a directory", path);
    return 0;
}




/*# ----------------------------------------------------------------------
 * _assert(expr,file,line)
 *
 * PARAMETER
 *	expr,file,line	see assert.h
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	This is a highly compiler-dependend function. It replaces the
 *	function which is called by the assert() macro dupplied with
 *	IBM Visual Age for C++.
 *
 * REMARKS
 */
extern "C" void
_LNK_CONV _assert(const char * expr, const char * file, unsigned int line)
{
    USHORT 	resp;
    char	str[1000];

    sprintf(str,
	    "Condition: %s\n"
	    "File: %s\n"
	    "Line: %u\n"
	    "Abort program?", expr, file, line);

    resp = WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, str, "Assertion failed!",
			 -1ul, MB_ERROR|MB_YESNO);
    if( resp == MBID_YES )
	abort();
}

