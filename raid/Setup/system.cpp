/*
 * $Source: R:/Source/driver/raid/Setup/RCS/system.cpp,v $
 * $Revision: 1.3 $
 * $Date: 1999/02/09 02:53:40 $
 * $Locker:  $
 *
 *	Interface to machine structures (BIOS, etc.)
 *
 * $Log: system.cpp,v $
 * Revision 1.3  1999/02/09 02:53:40  vitus
 * - switched to C++
 *
 * Revision 1.2  1998/12/13 23:04:49  vitus
 * - changed as global headers moved to /source/Lib
 *
 * Revision 1.1  1998/05/18 00:28:19  vitus
 * Initial revision
 * -------------------------------------------
 * This code is Copyright Vitus Jensen 1998-99
 */
static char const vcid[]="$Id: system.cpp,v 1.3 1999/02/09 02:53:40 vitus Exp $";

#include <stdlib.h>

#define __IBMC__	0
#define INCL_DOS
#define INCL_DOSDEVIOCTL
#define INCL_DOSERRORS
#include <os2.h>

extern "C" {
#include "../../../Lib/defines.h"
#include "../../../Lib/verbose.h"
}




#pragma pack(1)
typedef struct _TESTCFG_40PARAM {
    ULONG	Command;
    ULONG	Addr0;
    USHORT	NumBytes;
} TESTCFG_40PARAM;
#pragma pack()



PRIVATE HFILE	hdTestcfg = NULLHANDLE;



PUBLIC APIRET
QueryHostId(PUSHORT const id)
{
    APIRET		rc;
    ULONG		action;
    TESTCFG_40PARAM	param;
    ULONG		paramsize;
    ULONG		datasize = 0x8000;
    PUCHAR const	data = new UCHAR[datasize];
    PUCHAR	p = data;
    ULONG	i;


    rc = DosOpen("TESTCFG$", &hdTestcfg, &action, 0, 0,
		 OPEN_ACTION_FAIL_IF_NEW|OPEN_ACTION_OPEN_IF_EXISTS,
		 OPEN_SHARE_DENYNONE|OPEN_ACCESS_READWRITE, NULL);
    if( rc )
    {
	Verbose(0, "VSETUP", "DosOpen(TESTCFG$) - error %lu", rc);
	return rc;
    }

    param.Command = 0;
    param.Addr0 = 0x00F8000;
    param.NumBytes = 0x7000;
    paramsize = sizeof(param);
    rc = DosDevIOCtl(hdTestcfg,
		     IOCTL_TESTCFG_SYS, TESTCFG_SYS_GETBIOSADAPTER,
		     &param, paramsize, &paramsize,
		     data, datasize,  &datasize );
    if( rc )
    {
	Verbose(0, "VSETUP", "DosDevIOCtl(BIOSADAPTER) - error %lu", rc );
	return rc;
    }

    *id = 0;
    for( i = 0; i < datasize; ++i, ++p )
    {
	*id += *p;
    }

    delete[] data;
    rc = DosClose( hdTestcfg );
    if( rc )
    {
	Verbose(1, "VSETUP", "DosClose(TESTCFG$) - error %lu", rc );
    }
    return rc;
}

