/*
 * $Source: R:/Source/driver/raid/Setup/RCS/VSetup.h,v $
 * $Revision: 1.8 $
 * $Date: 2001/01/15 01:36:46 $
 * $Locker:  $
 *
 *	General defines and types.
 *
 * $Log: VSetup.h,v $
 * Revision 1.8  2001/01/15 01:36:46  vitus
 * - added WUM_INVCHILD
 *
 * Revision 1.7  2000/08/21 08:05:47  vitus
 * - defines WUM_REBUILD_DRV, WUM_UPDATE, WUM_COMPLETE
 *
 * Revision 1.6  2000/04/10 01:13:40  vitus
 * - added prototypes from proto.h
 *
 * Revision 1.5  1999/06/02 01:57:38  vitus
 * - defines WUM_REMCHILD
 *
 * Revision 1.4  1999/03/24 23:47:48  vitus
 * - increased stack size to 1MB
 *
 * Revision 1.3  1998/12/22 00:28:04  vitus
 * - added 'fSetupMode' and 'szProgName'.  Data every module may read
 *
 * Revision 1.2  1998/06/09 01:41:13  vitus
 * - larger stack
 * - unused definitions removed
 *
 * Revision 1.1  1998/05/15 22:49:16  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2001
 */

#define STACK_SIZE	(1024*1024)	/* sparse stack! */




#define WUM_ADDLOG	(WM_USER+1)	/* send this to main window */
#define WUM_START	(WM_USER+2)	/* window configured, starts
					   regular processing */
#define WUM_THREADENDED	(WM_USER+3)	/* Other thread ended:
					   mp1: thread structure
					   mp2: return value */

#define WUM_DEVTREE	(WM_USER+10)	/* passes a drive structure to display
					   window or a newly created tree to
					   scanner thread.
					   mp1: PDSKINFO */

#define WUM_DELPDEV	(WM_USER+11)	/* del PHYSDEVICE */
#define WUM_DELDRV	(WM_USER+12)	/* del VRAIDDEVICE */
#define WUM_MKPDEV	(WM_USER+13)	/* md PHYSDEVICE */
#define WUM_REMCHILD	(WM_USER+14)	/* remove child from array */
#define WUM_INVCHILD	(WM_USER+15)	/* invalidate child (rebuild starts) */

#define WUM_CLOSEDEV	(WM_USER+16)	/* close all devices of tree */
#define WUM_REBUILD_DRV	(WM_USER+17)	/* let vraid.flt do the work */


#define WUM_UPDATE	(WM_USER+20)	/* DrvVerify: update dialog entries */
#define WUM_COMPLETE	(WM_USER+21)	/* DrvVerify: verified all sectors */



#define INI_APPNAME	"VSetup"	/* app. key for OS2.INI */




extern char	szPrgName[];
extern USHORT	fSetupMode;

#define SM_FLTAVAIL	0x01			/* VRAID.FLT found */




extern void	ChangeWindowTitle(HWND const hwndFrame,char const * const title);
extern void	MyMessageBox(int const level,HWND const owner,char *fmt,...);
extern int	CheckValidDir(HWND const hwnd,char const * const dir);

extern APIRET	QueryHostId(PUSHORT const id);

extern void	CrcInit(USHORT polynom);
extern USHORT	Crc16(PVOID buffer,USHORT cb);
