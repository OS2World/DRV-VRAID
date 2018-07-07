/*
 * $Source: R:/source/driver/sleep/RCS/proto.h,v $
 * $Revision: 1.10 $
 * $Date: 2001/02/27 02:31:08 $
 * $Locker:  $
 *
 *	Function prototyping
 *
 * History:
 * $Log: proto.h,v $
 * Revision 1.10  2001/02/27 02:31:08  vitus
 * - added APMEventHandler()
 *
 * Revision 1.9  1999/11/29 00:42:31  vitus
 * - minor changes for M$ C 7.0 (_fmem* routines excluded as they are inlined)
 *
 * Revision 1.8  1998/06/29 00:00:40  vitus
 * - renamed _far_*() to _f*() (for MS-C, WatCom supplies)
 * - removed disable()/enable(), see dsksleep.h
 *
 * Revision 1.7  1997/12/05 01:42:18  vitus
 * - added WatCom specific code and header
 * - moved history to bottom of file
 * - added _Cdecl to all exported functions
 *
 * Revision 1.6  1997/07/21 01:32:26  vitus
 * Added IssueStart,IssueStop from iorb.c
 *
 * Revision 1.5  1997/06/18 00:51:15  vitus
 * - changed FilterFunction to _saveregs
 * - added prototypes for MS-C inline functions (_disable,_enable)
 *
 * Revision 1.4  1997/05/07 23:47:21  vitus
 * - added dprintf prototype (debugging)
 *
 * Revision 1.3  1997/03/03 01:26:26  vitus
 * Added _far_strlen, SaveMessage
 *
 * Revision 1.2  1996/10/23 23:42:15  vitus
 * Added _far_mem*() functions
 *
 * Revision 1.1  1996/09/27 03:39:41  vitus
 * Initial revision
 * ------------------------------------------------
 * This code is Copyright Vitus Jensen 1996-99,2001
 */


/* Module: segments.asm */

extern void _Cdecl	NEAR AsmTimer(void);	/* really far */
extern void _Cdecl	NEAR CallADD(PVOID,PIORB);

/* Module: str1.c */

extern void _Cdecl	NEAR Strategy();

#if defined(_MSC_VER) &&  _MSC_VER < 700
extern void	NEAR _fmemset(void _far *,UCHAR,USHORT);
extern void	NEAR _fmemcpy(void _far * dest,const void _far * src,size_t size);
extern int	NEAR _fmemcmp(const void _far *s1,const void _far * s2,size_t size);

extern size_t	NEAR _fstrlen(const char _far * s);
extern void	NEAR _fstrcpy(char _far * dst,const char _far * src);
#endif


/* Module: ioctl.c */

extern void	NEAR SaveMessage(void);


/* Module: iorb.c */

extern int NEAR				IssueStart(NPDEVICE const device);
extern void NEAR			IssueStop(NPDEVICE const device);
extern USHORT FAR _saveregs _Cdecl	APMEventHandler();
extern void  FAR _saveregs _loadds _Cdecl FilterFunction(PIORB const iorb);


/* Module: init.c */

extern USHORT	NEAR InitBase(PRPINITIN pRPI);


/* Module: printk.c */

extern void	NEAR sprintk(char FAR *outs,const char FAR *fmt,...);

#if defined(DEBUG)
/* Module: dprintf.asm */

extern void _Cdecl	NEAR dprintf(char FAR * msg,...);
#endif

