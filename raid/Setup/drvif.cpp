/*
 * $Source: R:/Source/driver/raid/Setup/RCS/drvif.cpp,v $
 * $Revision: 1.14 $
 * $Date: 2001/01/23 01:41:16 $
 * $Locker:  $
 *
 *	Interface to vraid.flt and other drivers (OS2DASD.DMD comes to mind).
 *
 * $Log: drvif.cpp,v $
 * Revision 1.14  2001/01/23 01:41:16  vitus
 * - DriverArrayChild() korrigiert, Zeiger und Zeiger auf Zeiger...
 *
 * Revision 1.13  2000/08/17 02:02:13  vitus
 * - implemented DriverArrayChild()
 *
 * Revision 1.12  2000/08/07 01:27:02  vitus
 * - implemented DriverArrayRebuild()
 *
 * Revision 1.11  2000/06/12 20:29:38  vitus
 * - implemented DriverArrayVerify()
 *
 * Revision 1.10  1999/10/01 01:06:42  vitus
 * - renamed DriverPhysinfo() to DriverPhyssize()
 * - new DriverPhysinfo() returning the complete VRAID_PHYSINFO_DATA
 *
 * Revision 1.9  1999/06/19 22:03:14  vitus
 * - implemented DriverSetBeeper()
 * - changed DriverGetVersion() to return flags, too.
 *
 * Revision 1.8  1999/06/02 01:47:34  vitus
 * - DriverClose: tells user that build processes have stopped for good, too.
 *
 * Revision 1.7  1999/05/13 22:46:32  vitus
 * - DriverClose: message box telling user to reboot when configuration
 *   was changed.
 *
 * Revision 1.6  1999/05/12 01:04:23  vitus
 * - DriverOpen: calls DriverStartSetup()
 * - DriverClose: calls DriverEndSetup()
 *
 * Revision 1.5  1999/04/14 23:42:27  vitus
 * - implemented DriverStartSetup(), DriverEndSetup()
 *
 * Revision 1.4  1999/02/09 02:48:17  vitus
 * - Umstellung auf C++
 *
 * Revision 1.3  1998/12/13 22:35:53  vitus
 * - implemented DriverArraylist(), DriverArrayinfo()
 *
 * Revision 1.2  1998/09/27 00:57:40  vitus
 * - commented
 * - corrected DriverPhysIO: data address
 *
 * Revision 1.1  1998/09/24 02:01:41  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2000
 */
static char const vcid[]="$Id: drvif.cpp,v 1.14 2001/01/23 01:41:16 vitus Exp $";

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <process.h>

#define __IBMC__	0
#define INCL_DOSDEVICES
#define INCL_DOSDEVIOCTL
#define INCL_DOSERRORS
#define INCL_WIN
#include <os2.h>

extern "C" {
#include "../../../lib/defines.h"
#include "../../../lib/verbose.h"
}
#include "drvif.h"




#define DEVICENAME	"VUJRAID$"



PRIVATE HFILE	hdDriver = -1ul;		/* handle of open VRaid.flt */




/*# ----------------------------------------------------------------------
 * DriverOpen(VOID)
 *
 * PARAMETER
 *	(none)
 * RETURNS
 *	APIRET
 * GLOBAL
 *	hdDriver
 * DESCRIPTION
 *	Opens vraid.flt driver and stores handle in global variable.
 *
 * REMARKS
 */
PUBLIC APIRET
DriverOpen(VOID)
{
    ULONG	action_taken;
    APIRET	rc;

    rc = DosOpen(DEVICENAME, &hdDriver, &action_taken, 0, 0,
		 OPEN_ACTION_FAIL_IF_NEW|OPEN_ACTION_OPEN_IF_EXISTS,
		 OPEN_SHARE_DENYNONE|OPEN_ACCESS_READWRITE,
		 NULL);
    DriverStartSetup();
    return rc;
}




/*# ----------------------------------------------------------------------
 * DriverClose(VOID)
 *
 * PARAMETER
 *	(none)
 * RETURNS
 *	APIRET
 * GLOBAL
 *	hdDriver
 * DESCRIPTION
 *	Ends driver access by closing 'hdDriver'.
 *	If no writing occured to any physdevice DriverEndSetup() is
 *	called restarting any suspended build process.
 *
 * REMARKS
 */
PUBLIC APIRET
DriverClose(VOID)
{
    VRAID_VER_DATA	data;
    ULONG		datasize = sizeof(data);
    APIRET		rc;

    if( hdDriver == -1ul )
	return ERROR_INVALID_HANDLE;
    rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_VERSION,
		     NULL, 0, NULL, &data, datasize, &datasize);
    if( rc == 0 )
    {
	if( (data.flags & 0x10) == 0 )
	    DriverEndSetup();			// no writing -> restart builds
	else
	    WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
			  "You have modified disk configuration, please reboot"
			  " as soon as possible to let VRAID.FLT read any changes"
			  " you did and reconfigure itself.\n"
			  "Build processes will be paused until reboot.",
			  "Reminder", -1ul, MB_ENTER|MB_INFORMATION|MB_MOVEABLE);
    }

    rc = DosClose(hdDriver);
    hdDriver = -1ul;
    return rc;
}




/*# ----------------------------------------------------------------------
 * DriverGetVersion(*ver)
 *
 * PARAMETER
 *	*ver			room to return version
 * RETURNS
 *	APIRET
 * GLOBAL
 *	hdDriver
 * DESCRIPTION
 *	Queries vraid.flt driver for it's version and returns it in '*ver'.
 *
 * REMARKS
 */
PUBLIC APIRET
DriverGetVersion(PUSHORT ver,PUSHORT flags)
{
    VRAID_VER_DATA	data;
    ULONG		datasize = sizeof(data);
    APIRET		rc;

    if( hdDriver == -1ul )
	return ERROR_INVALID_HANDLE;
    rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_VERSION,
		     NULL, 0, NULL, &data, datasize, &datasize);
    if( rc )
	return rc;
    *ver = data.version;
    *flags = data.flags;
    return 0;
}




/*# ----------------------------------------------------------------------
 * DriverReadMessages(**retbuf)
 *
 * PARAMETER
 *	**retbuf		returns pointer to buffer
 * RETURNS
 *	APIRET
 * GLOBAL
 *	hdDriver
 * DESCRIPTION
 *	Queries vraid.flt driver for stored messages, allocates
 *	a buffer large enough to hold them all and lets vraid.flt
 *	fill this buffer.  A pointer to the buffer is returned in
 *	'**retbuf'.
 *
 * REMARKS
 *	It's the responsibility of the caller to free the buffer.
 */
PUBLIC APIRET
DriverReadMessages(PUCHAR * retbuf)
{
    APIRET		rc;
    USHORT		cb = 0;			/* keep compiler happy */
    ULONG		datasize;
    char		* cp = NULL;
    PVRAID_MSGS_DATA	data;

    do
    {
	/* First: read size of stored messages */

	datasize = sizeof(cb);
	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_READ_MSGS,
			 NULL, 0, NULL, &cb, datasize, &datasize);
	if( rc )
	    break;


	/* Second: allocate buffer big enough to hold
	   those messages (but no single byte more) */

	datasize = FIELDOFFSET(VRAID_MSGS_DATA,msg) + (ULONG)cb;
	cp = new char[datasize];
	data = (PVRAID_MSGS_DATA)cp;
	*retbuf = new UCHAR[cb];		/* only messages */


	/* Third: read all messages and copy to caller
	 * buffer (newly allocated) */

	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_READ_MSGS,
			 NULL, 0, NULL, data, datasize, &datasize);
	if( rc )
	{
	    delete[] cp;
	    delete[] *retbuf;
	    *retbuf = NULL;
	    break;
	}

	memcpy(*retbuf,
	       data->msg,
	       (size_t)(data->cb-FIELDOFFSET(VRAID_MSGS_DATA,msg)));
	(*retbuf)[data->cb-FIELDOFFSET(VRAID_MSGS_DATA,msg)] = '\0'; /* drv err! */

	delete[] cp;
    }
    while(0);

    return rc;
}




/*# ----------------------------------------------------------------------
 * DriverPhyslist(cnt,retbuf)
 *
 * PARAMETER
 *	cnt		return count here
 *	retbuf		returns pointer to allocated list
 * RETURNS
 *	APIRET
 * GLOBAL
 *	hdDriver
 * DESCRIPTION
 *	Queries vraid.flt for the list of physical devices hidden from
 *	OS/2.
 *
 * REMARKS
 */
PUBLIC APIRET
DriverPhyslist(PUSHORT count,PUSHORT * retbuf)
{
    APIRET	rc;
    USHORT	cnt = 0;

    do
    {
	USHORT			cb;
	ULONG			datasize = sizeof(cb);
	char			* cp = NULL;
	PVRAID_PHYSLIST_DATA	data;

	/* First: read size of stored data */

	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_PHYSLIST,
			 NULL, 0, NULL, &cb, datasize, &datasize);
	if( rc )
	    break;


	/* Second: allocate buffer big enough to hold
	 * those handles (but no single byte more) */

	datasize = cb;
	cp = new char[datasize];
	data = (PVRAID_PHYSLIST_DATA)cp;
	*retbuf = new USHORT[(cb - sizeof(data->cb))/2]; /* only handles */


	/* Third: read all handles and copy to caller
	 * buffer (newly allocated) */

	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_PHYSLIST,
			 NULL, 0, NULL, data, datasize, &datasize);
	if( rc )
	{
	    delete[] cp;
	    delete[] *retbuf;
	    *retbuf = NULL;
	    break;
	}

	cnt = (USHORT)((data->cb - FIELDOFFSET(VRAID_PHYSLIST_DATA,hd))
		       / sizeof(data->hd[0]));
	for( cb = 0; cb < cnt; ++cb )
	    (*retbuf)[cb] = data->hd[cb];
	delete[] cp;
    }
    while(0);

    *count = cnt;
    return rc;
} /* end[DriverPhyslist] */




/*# ----------------------------------------------------------------------
 * DriverPhysinfo(hd,*info)
 *
 * PARAMETER
 *	hd		handle of driver device
 *	*info		room to return information
 *	hdDriver
 *
 * RETURNS
 *	APIRET

 * DESCRIPTION
 *	Queries vraid.flt for parameters of 'hd'.
 *
 * REMARKS
 */
PUBLIC APIRET
DriverPhysinfo(USHORT hd, PVRAID_PHYSINFO_DATA info)
{
    APIRET		rc;
    ULONG		parmsize;
    ULONG		datasize;

    datasize = sizeof(*info);
    parmsize = sizeof(hd);
    rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_PHYSINFO,
		     &hd, parmsize, &parmsize, info, datasize, &datasize);
    return rc;
} /* end[DriverPhysinfo] */




/*# ----------------------------------------------------------------------
 * DriverPhyssize(hd,*partsize)
 *
 * PARAMETER
 *	hd		handle of driver device
 *	*partsize	room to write partition size
 *	hdDriver
 *
 * RETURNS
 *	APIRET
 *
 * DESCRIPTION
 *	Queries vraid.flt for size of 'hd'.
 *
 * REMARKS
 */
PUBLIC APIRET
DriverPhyssize(USHORT hd,PULONG partsize)
{
    APIRET		rc;
    VRAID_PHYSINFO_DATA	data;

    rc = DriverPhysinfo(hd, &data);
    if( rc )
	*partsize = 0;
    else
	*partsize = data.partsize;

    return rc;
} /* end[DriverPhyssize] */




/*# ----------------------------------------------------------------------
 * DriverPhysIO(hd,write,offset,buffer)
 *
 * PARAMETER
 *	hd		handle of driver device
 *	write		write flag
 *	offset		offset in administration space
 *	buffer		place data here
 * RETURNS
 *	APIRET
 * GLOBAL
 *	hdDriver
 * DESCRIPTION
 *	Reads a single sector from a vraid.flt device and place
 *	it's contents in 'buffer'.
 *
 * REMARKS
 *	Should only be used to read sectors inside administrative
 *	sector range.
 */
PUBLIC APIRET
DriverPhysIO(USHORT hd,BOOL write,ULONG offset,PVOID buffer)
{
    APIRET		rc;
    ULONG		parmsize;
    ULONG		datasize;
    VRAID_IO_PARM	parm;

    parm.hd = hd;
    parm.flags = 0;
    if( write )
	parm.flags |= VIOF_WRITE;
    parm.reserved = 0;
    parm.partsecno = offset;

    datasize = 512;
    parmsize = sizeof(parm);
    rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_ADMINSEC_IO,
		     &parm, parmsize, &parmsize, buffer, datasize, &datasize);
    return rc;

} /* end[DriverPhysIO] */




/*# ----------------------------------------------------------------------
 * DriverArraylist(cnt,retbuf)
 *
 * PARAMETER
 *	cnt		return count here
 *	retbuf		returns pointer to allocated list
 * RETURNS
 *	APIRET
 * DESCRIPTION
 *	Queries vraid.flt for the list of array drives reported
 *	to OS/2.
 *
 * REMARKS
 *	Used global 'hdDriver'.
 */
PUBLIC APIRET
DriverArraylist(PUSHORT count,PUSHORT * retbuf)
{
    APIRET	rc;
    USHORT	cnt = 0;

    do
    {
	USHORT			cb;
	ULONG			datasize = sizeof(cb);
	char			* cp = NULL;
	PVRAID_ARRAYLIST_DATA	data;

	/* First: read size of stored data */

	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_ARRAYLIST,
			 NULL, 0, NULL, &cb, datasize, &datasize);
	if( rc )
	    break;


	/* Second: allocate buffer big enough to hold
	 * those handles (but not a single byte more) */

	datasize = cb;
	cp = new char[datasize];
	data = (PVRAID_ARRAYLIST_DATA)cp;
	*retbuf = new USHORT[(cb - sizeof(data->cb))/2]; /* only handles */


	/* Third: read all handles and copy to caller
	 * buffer (newly allocated) */

	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_ARRAYLIST,
			 NULL, 0, NULL, data, datasize, &datasize);
	if( rc )
	{
	    delete[] cp;
	    delete[] *retbuf;
	    *retbuf = NULL;
	    break;
	}

	cnt = (USHORT)((data->cb - FIELDOFFSET(VRAID_ARRAYLIST_DATA,hd))
		       / sizeof(data->hd[0]));
	for( cb = 0; cb < cnt; ++cb )
	    (*retbuf)[cb] = data->hd[cb];
	delete[] cp;
    }
    while(0);

    *count = cnt;
    return rc;
} /* end[DriverArraylist] */




/*# ----------------------------------------------------------------------
 * DriverArrayinfo(hd,*info)
 *
 * PARAMETER
 *	hd		handle of driver device
 *	*info		room to return information
 * RETURNS
 *	APIRET
 * DESCRIPTION
 *	Queries vraid.flt for parameters of 'hd'.
 *
 * REMARKS
 *	Uses global 'hdDriver'.
 */
PUBLIC APIRET
DriverArrayinfo(USHORT hd, PVRAID_ARRAYINFO_DATA info)
{
    APIRET	rc;
    ULONG	parmsize;
    ULONG	datasize;


    parmsize = sizeof(hd);
    datasize = sizeof(*info);
    rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_ARRAYINFO,
		     &hd, parmsize, &parmsize, info, datasize, &datasize);

    return rc;
} /* end[DriverArrayinfo] */




/*# ----------------------------------------------------------------------
 * DriverArrayVerify(p,buf,bufsiz)
 *
 * PARAMETER
 *	p		handle of driver device
 *	buf		room to return information
 *	bufsiz		bytes available at '*buf'
 *
 * RETURNS
 *	APIRET
 *
 * DESCRIPTION
 *	Forces vraid.flt to verify a sector range.
 *
 * REMARKS
 *	Uses global 'hdDriver'.
 */
PUBLIC APIRET
DriverArrayVerify(PVRAID_VERIFY_PARM p, PVOID buf, ULONG bufsiz)
{
    APIRET	rc;
    ULONG	parmsize;
    ULONG	datasize;


    parmsize = sizeof(*p);
    datasize = bufsiz;
    rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_VERIFY_ARRAY,
		     p, parmsize, &parmsize, buf, datasize, &datasize);

    return rc;
} /* end[DriverArrayVerify] */




/*# ----------------------------------------------------------------------
 * DriverArrayRebuild(hd,start_sector)
 *
 * PARAMETER
 *	hd		handle of driver device
 *	start_sector	where to start
 *
 * RETURNS
 *	APIRET
 *
 * DESCRIPTION
 *	Forces vraid.flt to rebuild an array.
 *
 * REMARKS
 *	Uses global 'hdDriver'.
 */
PUBLIC APIRET
DriverArrayRebuild(USHORT hd, ULONG start_sector)
{
    VRAID_REBUILD_PARM		param;
    VRAID_ARRAYINFO_DATA	data;
    ULONG	parmsize;
    ULONG	datasize;
    APIRET	rc;


    param.handle = hd;
    param.secno = start_sector;
    parmsize = sizeof(param);

    datasize = sizeof(data);;
    rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_START_REBUILD,
		     &param, parmsize, &parmsize, &data, datasize, &datasize);

    return rc;
}




/*# ----------------------------------------------------------------------
 * DriverArrayChild(child_info)
 *
 * PARAMETER
 *	child_info	new child info
 *
 * RETURNS
 *	APIRET
 *
 * DESCRIPTION
 *	Forces vraid.flt to change it's view of an array's child.
 *
 * REMARKS
 *	Uses global 'hdDriver'.
 */
PUBLIC APIRET
DriverArrayChild(PVRAID_CHILD_PARAM child_info)
{
    ULONG	parmsize;
    ULONG	datasize;
    APIRET	rc;


    parmsize = sizeof(child_info);
    datasize = 0;
    rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_SET_CHILD_STATE,
		     child_info, parmsize, &parmsize, NULL, datasize, &datasize);

    return rc;
}




/*# ----------------------------------------------------------------------
 * DriverStartSetup()
 *
 * PARAMETER
 *	(none)
 * RETURNS
 *	APIRET
 * DESCRIPTION
 *	Tells VRAID
 *
 * REMARKS
 *	Uses global 'hdDriver'.
 */
PUBLIC APIRET
DriverStartSetup(void)
{
    APIRET		rc;
    ULONG		parmsize;
    ULONG		datasize;

    datasize = 0;
    parmsize = 0;
    rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_START_SETUP,
		     NULL, parmsize, &parmsize, NULL, datasize, &datasize);

    return rc;
} /* end[DriverStartSetup] */




/*# ----------------------------------------------------------------------
 * DriverEndSetup(void)
 *
 * PARAMETER
 *	(none)
 * RETURNS
 *	APIRET
 *
 * DESCRIPTION
 *	Tells VRAID
 *
 * REMARKS
 *	Uses global 'hdDriver'.
 */
PUBLIC APIRET
DriverEndSetup(void)
{
    APIRET		rc;
    ULONG		parmsize;
    ULONG		datasize;

    datasize = 0;
    parmsize = 0;
    rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_END_SETUP,
		     NULL, parmsize, &parmsize, NULL, datasize, &datasize);

    return rc;
} /* end[DriverEndSetup] */




/*# ----------------------------------------------------------------------
 * DriverSetBeeper(onoff)
 *
 * PARAMETER
 *	onoff		0 : switch beeper off
 *			/0 : beeper on
 * RETURNS
 *	APIRET
 *
 * DESCRIPTION
 *	Tells VRAID to disable beeping in case of errors.
 *
 * REMARKS
 *	Uses global 'hdDriver'.
 */
PUBLIC APIRET
DriverSetBeeper(USHORT onoff)
{
    APIRET		rc;
    ULONG		parmsize;
    ULONG		datasize;

    datasize = 0;
    parmsize = sizeof(onoff);
    rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_SET_BEEPER,
		     &onoff, parmsize, &parmsize, NULL, datasize, &datasize);

    return rc;
}
