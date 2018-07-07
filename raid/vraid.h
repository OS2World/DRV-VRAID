/*
 * $Source: R:/source/driver/raid/RCS\\vraid.h,v $
 * $Revision: 1.15 $
 * $Date: 2001/06/25 00:34:03 $
 * $Locker:  $
 *
 *	Constants, some types, macros; mainly all that stuff 
 *	that isn't special to VRAID but to all OS/2 drivers.
 *
 * $Log: vraid.h,v $
 * Revision 1.15  2001/06/25 00:34:03  vitus
 * - increased BUILD_IOSIZE to 128, 64KB is largest no of sectors possible
 * - added more defines which should be included in DDK headers
 * - version 0.85, interim version for faster builds
 *
 * Revision 1.14  2001/02/11 01:34:13  vitus
 * - defines DRVSTATUS
 *
 * Revision 1.13  2001/01/30 00:50:01  vitus
 * - version 0.84, this should be the next release (vraid23)
 *
 * Revision 1.12  2000/04/15 22:24:18  vitus
 * - version 0.82, this should be the next release (vraid22)
 *
 * Revision 1.11  1999/10/05 01:05:37  vitus
 * - Version 0.80
 * - increased Workmem size to 40000 bytes
 * - CONSTMSG macro (all messages remaining after INIT)
 * - DBG_INIT4 level
 *
 * Revision 1.10  1999/06/06 23:37:03  vitus
 * - version 0.70 (next release)
 *
 * Revision 1.9  1999/06/02 02:25:32  vitus
 * - Version 0.66
 * - added INITMSG from init1.c
 * - define DBG_INIT1-3 display level
 *
 * Revision 1.8  1999/05/12 01:53:01  vitus
 * - TIMER_TMS decreased
 * - defined BUILD_IOSIZE, to 8
 *
 * Revision 1.7  1998/12/22 00:04:29  vitus
 * - VERSION/SUBVERSION 0.60 (rc)
 *
 * Revision 1.6  1998/11/05 01:06:13  vitus
 * - removed TIMEBOMB definitions (do users accept?)
 * - corrected (?) assert()
 *
 * Revision 1.5  1998/10/07 23:31:10  vitus
 * - added VERSION/SUBVERSION from Makefile
 * - timebomb explodes 31.3.1999
 *
 * Revision 1.4  1998/09/20 15:24:58  vitus
 * - added TIMEBOMB definitions (enabled)
 * - DBG_MEMORY, DBG_PATH
 *
 * Revision 1.3  1998/06/26 23:15:39  vitus
 * - implemented DDK macros (cli, sti, pushf) for MS-C
 *
 * Revision 1.2  1998/05/29 02:02:39  vitus
 * - removed INFMSG macro
 * - added debug level to DEBMSG macro
 *
 * Revision 1.1  1998/03/10 02:42:24  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2001
 */

/* To avoid mixing routines which return IORB and REQPKT error codes
 * we define a special type to contain the REQPKT codes.  This won't
 * stop the compiler from assigning IORB error codes to it but it
 * is a hint for the coder. */

typedef USHORT		DRVSTATUS;		/* STATUS_* */



#include "dsklayout.h"
#include "devstructs.h"


#define VERSION		0
#define SUBVERSION	85



/* Tunable constants */

#define MAX_PDEV		24		/* any reasonable value */
#define MAX_HDRV		16		/* --- "" --- */
#define MAX_ACTIVE_IORB		8		/* per hostdrive */
#define BUILD_IOSIZE		128		/* max=128 [sectors] */
#define WORKMEM_SIZE		40000		/* below 64KB !!! */
#define TIMER_TMS		1000		/* timer intervall [1/10 ms] */
#define VRIO_PER_TIMER		5		/* try to start that much */

#define MAX_DEVICETABLE_SIZE	(1*1024)
#define DISPLAYBUFFER_SIZE	(3*1024)

#define DRIVERFLAGS		0
#define DRIVERCLASS_ADD		1


/* Fixed values.  May be modified until first release. */

#define STRIPE_SIZE	16			/* [sectors] */

#define RESERVED_SIZE	16			/* reserved [sectors] at start
						   of physical disk including
						   boot sector */




/* **********************************************************************
 *	Types and Qualifiers
 * ******************************************************************* */

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


#define INITMSG		static char _ininit
#define CONSTMSG	static char _inconst




/* **********************************************************************
 *	Support Macros
 * ******************************************************************* */

#define MIN(a,b)	((a)>(b)?(b):(a))
#define MAX(a,b)	((a)>(b)?(a):(b))

#define MK_FP(sel,off)	MAKEP(sel,off)
#define FP_SEL(fp)	SELECTOROF(fp)
#define FP_OFF(fp)	OFFSETOF(fp)

#define LOCK(h)		_lock(&h)
#define UNLOCK(h)	_unlock(&h)

#if defined(_MSC_VER)
#define pushf()		_asm{pushf}
#define popf()		_asm{popf}
#define cli()		_disable()
#define sti()		_enable()
/*
#define SAVE_REGS()	_asm{_asm pusha _asm push ds _asm push es}
#define RESTORE_REGS()	_asm{_asm pop es _asm pop ds _asm popa}
*/
#elif defined(__WATCOMC__)
# include <include.h>				/* DDK: WatCom support */
#endif




/* **********************************************************************
 *	DDK Extensions
 * ******************************************************************* */

/* Defined in DDK but for some reason not included */
typedef UCHAR NEAR *NPUCHAR;

/* Missing in DDK */
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


#define MEMTYPE_ABOVE_1MB	0
#define MEMTYPE_BELOW_1MB	1


#define PAGESIZE		4096






/* **********************************************************************
 *	Debugging macros
 ********************************************************************* */

#if defined(DEBUG)
/* Debug levels are bit coded: */

#define DBG_ALL		0xFFFF
#define DBG_LEVEL1	0x0001
#define DBG_LEVEL2	0x0002
#define DBG_LEVEL3	0x0004
#define DBG_LEVEL4	0x0008
#define DBG_INIT1	0x0010
#define DBG_INIT2	0x0020
#define DBG_INIT3	0x0040
#define DBG_INIT4	0x0080
#define DBG_QUEUE	0x0100
#define DBG_MEMORY	0x0200
#define DBG_PATH	0x0400

extern USHORT		uDebugLevel;

#define DO(bef)		bef

#if defined(_MSC_VER)
# define DBSTOP()	_asm {int 3}
#else
void DBSTOP(void);
# pragma aux DBSTOP = "int 3";
#endif

#define ASSERT(cond)	{if(!(cond)){dprintf("Assertion failed: %s",(char _far *)#cond);DBSTOP();}}


#define DEBMSG(lvl,msg)		((lvl)&uDebugLevel ? dprintf((char _far *)msg) : 0)
#define DEBMSG1(lvl,msg,p1)	((lvl)&uDebugLevel ? dprintf((char _far *)msg,p1) : 0)
#define DEBMSG2(lvl,msg,p1,p2)	((lvl)&uDebugLevel ? dprintf((char _far *)msg,p1,p2) : 0)
#define DEBMSG3(lvl,msg,p1,p2,p3)	((lvl)&uDebugLevel ? dprintf((char _far *)msg,p1,p2,p3) : 0)
#define DEBMSG4(lvl,msg,p1,p2,p3,p4)	((lvl)&uDebugLevel ? dprintf((char _far *)msg,p1,p2,p3,p4) : 0)

#else
/* Define all debugging to nothing on release code. */
# define DO(bef)
# define DBSTOP()

# define ASSERT(cond)		{}

# define DEBMSG(lvl,msg)
# define DEBMSG1(lvl,msg,p1)
# define DEBMSG2(lvl,msg,p1,p2)
# define DEBMSG3(lvl,msg,p1,p2,p3)
# define DEBMSG4(lvl,msg,p1,p2,p3,p4)
#endif /*DEBUG*/
