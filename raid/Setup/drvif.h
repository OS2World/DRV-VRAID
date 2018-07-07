/*
 * $Source: R:/Source/driver/raid/Setup/RCS/drvif.h,v $
 * $Revision: 1.8 $
 * $Date: 2000/08/17 02:01:52 $
 * $Locker:  $
 *
 *	Interface to drvif.c
 *
 * $Log: drvif.h,v $
 * Revision 1.8  2000/08/17 02:01:52  vitus
 * - added DriverArrayChild()
 *
 * Revision 1.7  2000/08/07 01:26:10  vitus
 * - added DriverArrayRebuild()
 *
 * Revision 1.6  2000/06/12 20:27:56  vitus
 * - added DriverArrayVerify()
 *
 * Revision 1.5  1999/09/28 00:58:54  vitus
 * - changed DriverPhysinfo() to DriverPhyssize()
 * - added new DriverPhysinfo() to return VRAID_PHYSINFO_DATA
 *
 * Revision 1.4  1999/06/19 22:02:51  vitus
 * - added DriverSetBeeper()
 * - changed DriverGetVersion() to return flags, too.
 *
 * Revision 1.3  1999/04/14 23:42:18  vitus
 * - added DriverStartSetup(), DriverEndSetup()
 *
 * Revision 1.2  1998/12/13 22:35:24  vitus
 * - added DriverArraylist(), DriverArrayinfo()
 *
 * Revision 1.1  1998/09/24 01:53:37  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2000
 */

#include "../dsklayout.h"
#include "../vraidpub.h"


extern APIRET	DriverOpen(VOID);
extern APIRET	DriverClose(VOID);
extern APIRET	DriverGetVersion(PUSHORT ver,PUSHORT flags);
extern APIRET	DriverReadMessages(PUCHAR * retbuf);

extern APIRET	DriverPhyslist(PUSHORT cnt,PUSHORT * list);
extern APIRET	DriverPhysinfo(USHORT hd,PVRAID_PHYSINFO_DATA info);
extern APIRET	DriverPhyssize(USHORT hd,PULONG partsize);
extern APIRET	DriverPhysIO(USHORT hd,BOOL write,ULONG offset,PVOID buffer);

extern APIRET	DriverArraylist(PUSHORT cnt,PUSHORT * list);
extern APIRET	DriverArrayinfo(USHORT hd,PVRAID_ARRAYINFO_DATA info);
extern APIRET	DriverArrayVerify(PVRAID_VERIFY_PARM p, PVOID buf, ULONG bufsiz);
extern APIRET	DriverArrayRebuild(USHORT handle, ULONG start_sector);
extern APIRET	DriverArrayChild(PVRAID_CHILD_PARAM param);

extern APIRET	DriverStartSetup(VOID);
extern APIRET	DriverEndSetup(VOID);

extern APIRET	DriverSetBeeper(USHORT onoff);
