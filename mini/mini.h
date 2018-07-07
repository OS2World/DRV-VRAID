/*
 * $Source: r:/source/driver/mini/RCS/mini.h,v $
 * $Revision: 1.2 $
 * $Date: 2000/06/12 02:10:24 $
 * $Locker:  $
 *
 *	General Header for Mini.sys
 *
 * $Log: mini.h,v $
 * Revision 1.2  2000/06/12 02:10:24  vitus
 * - removed variables not used in sample
 * - added dprintf() prototype
 *
 * Revision 1.1  2000/06/12 01:28:44  vitus
 * Initial revision
 * ----------------------------------------
 * Author: Vitus Jensen, 2:2474/424
 */


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

#define _Cdecl		cdecl

#elif defined(__WATCOMC__)
# include <include.h>				/* DDK: WatCom support */
#endif




/* **********************************************************************
 * **** External Data ***************************************************
 * ******************************************************************* */

extern DDHDR		DiskDDHeader;		/* device header, located
						   at start of first segment*/
extern PFN		Device_Help;		/* entry to header routines */

extern USHORT		fDriverFlags;
#define DF_VERBOSE	0x0001			/* logo, some messages */
#define DF_DEBUG	0x0002			/* many messages */
#define DF_KEEP		0x0004			/* keep filter in memory */
#define DF_INITDONE	0x0100			/* INIT stage 1 done, no more
						   screen messages possible */


extern char	szDriverName[];			/* my name ("Mini$") */



/* Initialization Data */

extern UCHAR	szMsgBuffer[];			/* to build ASCII messages */




/* **********************************************************************
 * **** External Functions **********************************************
 * ******************************************************************* */

extern USHORT	NEAR Init(PRPINITIN pRPI);
extern void	NEAR sprintk(char _far *outs,const char _far *fmt,...);

#if defined(DEBUG)
extern void NEAR _Cdecl	dprintf(char FAR * msg,...);
#endif




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
