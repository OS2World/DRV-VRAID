/*
 * $Source: e:/source/driver/perf/RCS/proto.h,v $
 * $Revision: 1.3 $
 * $Date: 1997/12/11 01:53:38 $
 * $Locker:  $
 *
 * Function prototyping
 *
 * $Log: proto.h,v $
 * Revision 1.3  1997/12/11 01:53:38  vitus
 * - added prototype of SaveMessage()
 *
 * Revision 1.2  1997/12/06 00:42:49  vitus
 * - enabled support for WatCom C
 *
 * Revision 1.1  1997/10/15 00:54:42  vitus
 * Initial revision
 * --------------------------------------------
 * Author: Vitus Jensen, 2:2474/424
 */


#if defined(_MSC_VER)
/* MS-C inline functions (these take only near pointer!)
 * OBS: don't forget to compile w/ inlining (at least -Oi) */

void	 _disable(void);
void	 _enable(void);
#endif


/* Module: str1.C */

extern void	NEAR _Cdecl Strategy();


/* Module: ioctl.c */

extern void	NEAR SaveMessage(void);


/* Module: iorb.c.  No real filter, no code. */


/* Module: init.c */

extern USHORT	NEAR InitBase(PRPINITIN pRPI);


/* Module: printk.c */

extern void	NEAR sprintk(char _far *outs,const char _far *fmt,...);

/* Module: printk.c */

extern void	NEAR sprintk(char _far *outs,const char _far *fmt,...);

#if defined(DEBUG)
/* Module: dprintf.asm */

extern void NEAR _Cdecl	dprintf(char FAR * msg,...);
#endif
