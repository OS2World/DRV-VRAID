/*
 * $Source: E:/source/driver/perf/RCS/scsiopt.h,v $
 * $Revision: 1.3 $
 * $Date: 1997/12/11 01:49:12 $
 * $Locker:  $
 *
 * Types and Constants
 *
 * $Log: scsiopt.h,v $
 * Revision 1.3  1997/12/11 01:49:12  vitus
 * - added DISPLAYBUFFER_SIZE (swappable memory size)
 *
 * Revision 1.2  1997/12/06 00:41:43  vitus
 * - enabled support for WatCom C
 *
 * Revision 1.1  1997/10/28 02:08:27  vitus
 * Initial revision
 * ----------------------------------------
 * Author: Vitus Jensen, 2:2474/424, 1997
 */


#define MAX_UNITS		32
#define MAX_DEVICETABLE_SIZE	(1*1024)
#define MAX_MODE_DATA		250		/* max. requested from device
						   via MODE SENSE(6) */
#define DISPLAYBUFFER_SIZE	(8*1024)

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
# define SAVE_IF()	_asm{pushf}
# define RESTORE_IF()	_asm{popf}
# define DISABLE()	_disable()
# define ENABLE()	_enable()
# define SAVE_REGS()	_asm{_asm pusha _asm push ds _asm push es}
# define RESTORE_REGS()	_asm{_asm pop es _asm pop ds _asm popa}

#define _Cdecl		cdecl

#elif defined(__WATCOMC__)
void	SAVE_IF(void);
void	RESTORE_IF(void);
void	DISABLE(void);
void	ENABLE(void);
void	SAVE_REGS(void);
void	RESTORE_REGS(void);

#pragma aux SAVE_IF = "pushf";
#pragma aux RESTORE_IF = "popf";
#pragma aux DISABLE = "cli";
#pragma aux ENABLE = "sti";
#pragma aux SAVE_REGS =	\
	"pusha",	\
	"push ds",	\
	"push es"
#pragma aux RESTORE_REGS =	\
	"pop es",	\
	"pop ds",	\
	"popa"
#endif



/* Defined in DDK but for some reason not included */
typedef UCHAR NEAR *NPUCHAR;

/*
 * Missing in DDK (not in newest, but not defined
 * in this way)
 */
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




/*
 * All information regarding a unit are collected
 * into a single structure.
 * If we are a real filter (like DSKSleep) we would have
 * a global table of this kind.
 */
typedef struct _DEVICE {

    /* How to access this device... */

    void	(FAR _Cdecl * pADDEntry)(PIORB);
    USHORT	hdADDUnit;			/* ADD handle of this unit */
    USHORT	hdFilter;			/* previous filter (if any) */

    USHORT	iAdapter;			/* global index */
    USHORT	iUnit;				/* per adapter index */

    /* more... */

} DEVICE, NEAR * NPDEVICE, FAR * PDEVICE;




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
# define ERRMSG4(msg,p1,p2,p3,p4) dprintf((char _far *)msg,p1,p2,p3,p4); _asm {int 3}

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
# define ERRMSG4(msg,p1,p2,p3,p4)
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
void DBSTOP(void);
#  pragma aux DBSTOP = "int 3";
# endif
#else
# define DBSTOP()
#endif


#if defined(DEBUG)
# define DO(cmd)		cmd
#else
# define DO(cmd)
#endif
