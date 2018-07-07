/*
 * $Source: r:/source/driver/sleep/RCS/dsksleep.h,v $
 * $Revision: 1.15 $
 * $Date: 2001/06/23 00:18:33 $
 * $Locker:  $
 *
 *	Types and Constants
 *
 * History: see bottom of file
 * ------------------------------------------------
 * This code is Copyright Vitus Jensen 1996-99,2001
 */

#include <string.h>
#include <stdlib.h>

#include <os2.h>				/* INCL_ ist set by caller */

#include <devcmd.h>
#include <devclass.h>
#include <dskinit.h>
#include <infoseg.h>
#include <strat2.h>
#include <reqpkt.h>

/*
#include <scb.h>
#include <abios.h>
*/

#if defined(__WATCOMC__)
# include <devhelp.h>
typedef USHORT NEAR *NPUSHORT;
typedef VOID   NEAR *NPVOID;
# include <include.h>				/* DDK: WatCom support */
#else
# define _Cdecl		cdecl
# include <dhcalls.h>
#endif

#include <scsi.h>
#include <iorb.h>




#define VERSION		0
#define SUBVERSION	99



/* Tunable constants */

#define MAX_UNITS		24
#define MAX_DEVICETABLE_SIZE	(1*1024)
#define DISPLAYBUFFER_SIZE	(3*1024)

#define START_TIMEOUT		30		/* if defined: force ADD
						   to wait up to n seconds
						   for START UNIT */

#define DRIVERFLAGS		0
#define DRIVERCLASS_ADD		1


#if !defined(DEBUG)
# define PRIVATE	static
#else
# define PRIVATE
#endif
#define PUBLIC

#define _intext 	_based(_segname("_CODE"))
#define _ininit 	_based(_segname("INITDATA"))
#define _inconst 	_based(_segname("_CONST"))
#define	_inldata	_based(_segname("LIBDATA"))


#define MK_FP(sel,off)	MAKEP(sel,off)
#define FP_SEL(fp)	SELECTOROF(fp)
#define FP_OFF(fp)	OFFSETOF(fp)


#if defined(_MSC_VER)
void	 _disable(void);
void	 _enable(void);

#define pushf()		_asm{pushf}
#define popf()		_asm{popf}
#define cli()		_disable()
#define sti()		_enable()


#elif defined(__WATCOMC__)

#endif



/*
 *	Timer definitions
 *	The driver contains a ticker (ulTimer) which is
 *	allocated and handled through standard OS/2 API.
 *	This 'ulTimer' is referenced for timeout detection.
 *	OBS: all definitions refer to an intervall of 1s!
 */
#define TIMER_TMS	(1*10000U)	/* call intervall [tenths of ms] */
#define SLEEP_TIME	(30*60U)	/* will result device stop [s] */



/* Defined in DDK but for some reason not included */
typedef UCHAR NEAR *NPUCHAR;

/* Missing in DDK: */
#undef STATUS_ERR_UNKCMD
#define STDEV			0x4000		/* driver defined error code */
#define STATUS_ERR_UNKUNIT	(STERR | STDON | 0x01)
#define STATUS_ERR_NOTREADY	(STERR | STDON | 0x02)
#define STATUS_ERR_UNKCMD	(STERR | STDON | 0x03)
#define STATUS_ERR_CRC		(STERR | STDON | 0x04)
#define STATUS_ERR_REQLEN	(STERR | STDON | 0x05)
#define STATUS_ERR_SEEK		(STERR | STDON | 0x06)
#define STATUS_ERR_UNKMEDIA	(STERR | STDON | 0x07)
#define STATUS_ERR_NOTFOUND	(STERR | STDON | 0x08)
#define STATUS_ERR_PAPEROUT	(STERR | STDON | 0x09)
#define STATUS_ERR_WRFAULT	(STERR | STDON | 0x0A)
#define STATUS_ERR_RDFAULT	(STERR | STDON | 0x0B)
#define STATUS_ERR_GENERR	(STERR | STDON | 0x0C)
#define STATUS_ERR_CHGDISK	(STERR | STDON | 0x0D)
#define STATUS_ERR_UNCERTAIN	(STERR | STDON | 0x10)
#define STATUS_ERR_INTERRUPTED	(STERR | STDON | 0x11)
#define STATUS_ERR_NOMONITOR	(STERR | STDON | 0x12)
#define STATUS_ERR_INVPARAM	(STERR | STDON | 0x13)
#define STATUS_ERR_DEVINUSE	(STERR | STDON | 0x14)
#define STATUS_ERR_INITFAILED	(STERR | STDON | 0x15)



/* Missing in apmcalls.h, but not in apmioctl.h (though
 * they use different naming there) */

#define APM_SETPWRSTATEEVENT	APM_SETPWRSTATE	/* APMEVENT_SetPowerState */
#define APM_STANDBYRESUMEEVENT	0x0A
#define APM_QUERYPWRSTATEEVENT	0x10		/* APMEVENT_QueryPowerState */

#define APM_NOTIFYSTANDBYRESUME	(1<<APM_STANDBYRESUMEEVENT)
#define APM_NOTIFYQUERYPWRSTATE	(1<<APM_QUERYPWRSTATEEVENT)

/* APM_SETPWRSTATE Event, power states: */
#define APM_PWRSTATESTANDBY	0x01
#define APM_PWRSTATESUSPEND	0x02		/* APMSTATE_Suspend */
#define APM_PWRSTATEOFF		0x03






typedef struct _DEVICE {

    struct {
	int	iorb_busy : 1;
	int	stopped : 1;			/* motor stopped */
	int	blocked : 1;			/* no I/Os, please */
    };

    USHORT	counter;			/* set to 0 on access,
						   incremented by timer */

    /* Reason for last device start: */

    UCHAR	rst_reason[MAX_IORB_SIZE];	/* complete IORB which started */

    /* How to access this device... */

    void	(FAR  _Cdecl * pADDEntry)(PIORB);
    USHORT	hdADDUnit;			/* ADD handle of this unit */
    USHORT	hdFilter;			/* previous filter (if any) */

    UNITINFO	modinfo;			/* modified unit info */
    USHORT	iAdapter;			/* global index */
    USHORT	iUnit;				/* per adapter index */

    /* Things to do I/Os with */

    USHORT	usSleep;			/* seconds until sleep */
    UCHAR	cdbStop[6];			/* for each unit because
						   LUN is included */
    UCHAR	cdbStart[6];
    UCHAR	cdbTestReady[6];

    /* Information about last I/O done by this driver. */

    USHORT		iotype;			/* really UCHAR, see pub.h */
    USHORT		laststatus;
    USHORT		lasterror;
    SCSI_STATUS_BLOCK	statusblock;		/* same segment as IORB! */
    SCSI_REQSENSE_DATA	sensedata;

    UCHAR	iorb[MAX_IORB_SIZE];		/* IORB to do start/stop */

    /* As we cannot block we need an IORB queue */

    PIORB	pQueueHead;
    PIORB	pQueueFoot;			/* IORB queueing */

} DEVICE, NEAR * NPDEVICE;




/*
 *	Debugging macros
 */

#if defined(DEBUG)
# define DEBMSG(msg)		dprintf((char _far *)msg)
# define DEBMSG1(msg,p1)	dprintf((char _far *)msg,p1)
# define DEBMSG2(msg,p1,p2)	dprintf((char _far *)msg,p1,p2)
# define DEBMSG3(msg,p1,p2,p3)	dprintf((char _far *)msg,p1,p2,p3)
# define DEBMSG4(msg,p1,p2,p3,p4) dprintf((char _far *)msg,p1,p2,p3,p4)
# define ERRMSG(msg)		dprintf((char _far *)msg); _asm {int 3}
# define ERRMSG1(msg,p1)	dprintf((char _far *)msg,p1); _asm {int 3}
# define ERRMSG2(msg,p1,p2)	dprintf((char _far *)msg,p1,p2); _asm {int 3}
# define ERRMSG3(msg,p1,p2,p3)	dprintf((char _far *)msg,p1,p2,p3); _asm {int 3}

#else
# define DEBMSG(msg)
# define DEBMSG1(msg,p1)
# define DEBMSG2(msg,p1,p2)
# define DEBMSG3(msg,p1,p2,p3)
# define DEBMSG4(msg,p1,p2,p3,p4)
# define ERRMSG(msg)
# define ERRMSG1(msg,p1)
# define ERRMSG2(msg,p1,p2)
# define ERRMSG3(msg,p1,p2,p3)
#endif /*DEBUG*/



#if defined(INFORM)
# define INFMSG(msg)		dprintf((char _far *)msg);
# define INFMSG1(msg,p1)	dprintf((char _far *)msg,p1);
# define INFMSG2(msg,p1,p2)	dprintf((char _far *)msg,p1,p2);
# define INFMSG3(msg,p1,p2,p3)	dprintf((char _far *)msg,p1,p2,p3);
# define INFMSG4(msg,p1,p2,p3,p4)	dprintf((char _far *)msg,p1,p2,p3,p4);

#else
# define INFMSG(msg)
# define INFMSG1(msg,p1)
# define INFMSG2(msg,p1,p2)
# define INFMSG3(msg,p1,p2,p3)
# define INFMSG4(msg,p1,p2,p3,p4)
#endif




#if defined(DEBUG)
# if defined(_MSC_VER)
#  define DBSTOP()		_asm {int 3}
# else
#  define DBSTOP()		int3()
# endif
#else
# define DBSTOP()
#endif


#if defined(DEBUG)
# define DO(bef)		bef
#else
# define DO(bef)
#endif



/* History;
 *
 * $Log: dsksleep.h,v $
 * Revision 1.15  2001/06/23 00:18:33  vitus
 * - version 0.99
 * - defined some more APM constants
 *
 * Revision 1.14  2001/02/27 02:34:03  vitus
 * - changed to version 0.98 (first APM support)
 *
 * Revision 1.13  1999/11/29 00:45:02  vitus
 * - includes all system include files
 * - includes includes.h if WatCom C
 * - DEVICE contains restart reason (IORB)
 *
 * Revision 1.12  1998/02/26 00:03:15  vitus
 * - corrected ENABLE macro (WatCom)
 *
 * Revision 1.11  1997/12/05 01:46:54  vitus
 * - added WatCom specific macros and defines
 * - moved history to bottom of file
 *
 * Revision 1.10  1997/09/03 01:20:51  vitus
 * added iotype to device structure (which I/O failed?)
 *
 * Revision 1.9  1997/07/21 01:05:58  vitus
 * Added status buffers to unit structure
 *
 * Revision 1.8  1997/06/18 00:47:37  vitus
 * added [SAVE|RESTORE]_[REGS|IF] macros
 * changed DISABLE,ENABLE to use inline functions
 *
 * Revision 1.7  1997/05/07 23:55:06  vitus
 * added debugging macros and _inconst
 * added IORB queue (head,foot) to device structure
 *
 * Revision 1.6  1997/03/03 01:20:05  vitus
 * added START_TIMEOUT definition
 * added DISPLAYBUFFER_SIZE
 * added TEST UNIT READY cdb and adapter index to device structure
 *
 * Revision 1.5  1997/02/11 23:09:36  vitus
 * Redefine STATUS_ERR_UNKNCMD (incl. STDON)
 *
 * Revision 1.4  1997/02/06 01:04:22  vitus
 * Added STATUS_ERR_* definitions
 * Added FP_* macros
 *
 * Revision 1.3  1996/11/04 00:10:27  vitus
 * Added timeout value to device structure
 *
 * Revision 1.2  1996/10/23 23:35:07  vitus
 * Don't define PRIVATE as static in debug version
 *
 * Revision 1.1  1996/09/27 03:34:16  vitus
 * Initial revision
 */
