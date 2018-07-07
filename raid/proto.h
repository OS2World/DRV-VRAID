/*
 * $Source: R:/source/driver/raid/RCS\\proto.h,v $
 * $Revision: 1.19 $
 * $Date: 2001/09/03 02:58:08 $
 * $Locker:  $
 *
 *	Function prototyping.
 *
 *
 *	OBS: functions marked to return DRVSTATUS will return '0'
 *	if all was OK.  This is not totally correct as the OS
 *	needs the STDON bit to mark the request as complete, but
 *	comparing with '0' is so much easier...
 *	Errors are returned complete with STERR and STDON set, though.
 *
 *
 * History:
 * $Log: proto.h,v $
 * Revision 1.19  2001/09/03 02:58:08  vitus
 * - added Raid5Create()
 *
 * Revision 1.18  2001/06/24 23:00:34  vitus
 * - added AllocateFixed(), FreeFixed()
 * - BuildCreate() now returns error code
 *
 * Revision 1.17  2001/02/11 01:31:24  vitus
 * - InitBase, ProcessAdminIO, ProcessUserIO now return DRVSTATUS
 * - removed PhysdevPartIO, since almost 2 year non existing
 *
 * Revision 1.16  2001/01/15 01:20:31  vitus
 * - UpdateCfg: new 'forced' parameter
 *
 * Revision 1.15  2000/10/24 23:56:39  vitus
 * - BuildCreate: parameter 'mode' to distinguish build and rebuild
 *
 * Revision 1.14  2000/06/15 01:07:31  vitus
 * - added ProcessUserIO()
 *
 * Revision 1.13  1999/12/14 00:36:33  vitus
 * - added ValidateVSec2() (init1.c)
 *
 * Revision 1.12  1999/10/01 00:55:30  vitus
 * - changed UpdateCfg() prototype: returns BOOL saying whether update
 *   was really started or not.
 *
 * Revision 1.11  1999/09/18 14:35:10  vitus
 * - added FilterHandler(), routes IORBs to BASEDISKs
 *
 * Revision 1.10  1999/07/06 01:58:26  vitus
 * - corrected calling convention of BuildNotify(), UpdateNotify(): _Cdecl
 *
 * Revision 1.9  1999/06/22 00:05:06  vitus
 * - renamed InitWorkmem to BuildWorkmem
 * - added ReleaseWorkmem, to unlock the memory
 *
 * Revision 1.8  1999/05/26 00:51:31  vitus
 * - *Create() routines now return 'int'
 *
 * Revision 1.7  1999/05/12 01:50:29  vitus
 * - added CRC routines
 * - more routines from build.c
 * - added CompleteVrio()
 *
 * Revision 1.6  1999/02/09 00:09:23  vitus
 * - ProcessAdminIO() addded (vrio.c)
 * - MirrorCreate() now accepts SEC_VRDEV2 structure
 *
 * Revision 1.5  1998/09/27 16:09:06  vitus
 * - corrected CallADD() prototype: MC-C/C++ 7.0 complains
 *
 * Revision 1.4  1998/09/20 15:16:26  vitus
 * - changed to AllocateIorb(sgcnt)
 * - changed to AllocateVrio(working_dev,mother)
 *
 * Revision 1.3  1998/06/26 23:01:45  vitus
 * - corrected prototypes for MS-C 6.00A
 *
 * Revision 1.2  1998/04/08 01:29:29  vitus
 * - corrected prototypes to *Create() functions
 * - changed AllocateVrio() to accept mother
 *
 * Revision 1.1  1998/03/10 02:30:32  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1997-2001
 */

#if defined(_MSC_VER)

/* MS-C inline functions (these take only near pointer!)
 * OBS: don't forget to compile w/ inlining (at least -Oi) */

void	_disable(void);
void	_enable(void);
#endif


/* Module: startup.asm */

extern void	NEAR _Cdecl AsmTimer(void);	/* really far */
extern void	NEAR _Cdecl CallADD(void (FAR _Cdecl *)(PIORB),PIORB);

extern void	NEAR _Cdecl _lock(PULONG pulHandle);
extern void	NEAR _Cdecl _unlock(PULONG pulHandle);


/* Module: str1.c */

extern void	NEAR _Cdecl Strategy();


/* Module: ioctl.c */

extern void	NEAR SaveMessage(void);


/* Module: build.c */

extern void	NEAR _Cdecl BuildNotify(PVRIO const io);
extern USHORT	NEAR BuildCreate(PVRDEVHDR vrd, ULONG next_sector, USHORT mode);
extern void	NEAR UpdateBuilds(void);
extern void	NEAR _Cdecl UpdateNotify(PVRIO const io);
extern BOOL	NEAR UpdateCfg(PVRDEVHDR const vrd, BOOL forced);
extern void	NEAR SaveArrayInfos(void);


/* Module: iorb.c */

extern void	NEAR _Cdecl HostdriveNotify(PVRIO io);
extern void	NEAR _Cdecl NotifyIorb(PIORBH const pIorbh,USHORT ErrorCode);
extern void	FAR _loadds _Cdecl IorbEntry(PIORB const pIorb);


/* Module: filter.c */

extern void	NEAR _Cdecl FilterHandler(PBASEDISK const disk,PIORB const pIorb);


/* Module: physdev.c, single.c, mirror.c, stripe.c, span.c, ... */

extern int	NEAR _Cdecl PhysdevCreate(PVRDEVHDR pVrh, PPHYSDEVICE pPhys,
					  PSEC_PHYSDEV sec, PUSHORT pSize);
extern int	NEAR _Cdecl SingleCreate(PVRDEVHDR pVrh, PVRDEVHDR FAR * tab,
					 PSEC_VRDEV sec, PUSHORT pSize);
extern int	NEAR _Cdecl SpanCreate(PVRDEVHDR pVrh, PVRDEVHDR FAR * tab,
				       PSEC_VRDEV sec, PUSHORT pSize);
extern int	NEAR _Cdecl StripeCreate(PVRDEVHDR pVrh, PVRDEVHDR FAR * tab,
					 PSEC_VRDEV sec, PUSHORT pSize);
extern int	NEAR _Cdecl MirrorCreate(PVRDEVHDR pVrh, PVRDEVHDR FAR * tab,
					 PSEC_VRDEV2 sec, PUSHORT pSize);
extern int	NEAR _Cdecl Raid5Create(PVRDEVHDR pVrh, PVRDEVHDR FAR * tab,
					PSEC_VRDEV2 sec, PUSHORT pSize);




/* Module: vrio.c */

extern void	NEAR _Cdecl CompleteVrio(PVRIO const io);
extern void	NEAR _Cdecl QueueVrio(PVRIO const io);
extern PVRIO	NEAR _Cdecl DequeueVrio(void);
extern void	NEAR _Cdecl InsertVrio(PVRIO const io);
extern void	NEAR _Cdecl StartManyIorb(PHOSTDRIVE const drvp);
extern DRVSTATUS NEAR _Cdecl ProcessAdminIO(PVRDEVHDR const target,BOOL write,
					    ULONG blk,USHORT bcnt,ULONG physaddr);
extern DRVSTATUS NEAR _Cdecl ProcessUserIO(PVRDEVHDR const target,BOOL write,
					   ULONG blk,USHORT bcnt,ULONG physaddr);



/* Module: memory.c */

extern USHORT	NEAR _Cdecl BuildWorkmem(void);
extern USHORT	NEAR _Cdecl ReleaseWorkmem(void);

extern void	NEAR _Cdecl MemFree(PVOID const ap);
extern PVOID	NEAR _Cdecl MemAlloc(USHORT const nbytes);

extern PIORB_EXECUTEIO	NEAR _Cdecl AllocateIorb(USHORT const sgcnt);
extern void	NEAR _Cdecl FreeIorb(PIORB_EXECUTEIO iorb);
extern PVRIO	NEAR _Cdecl AllocateVrio(PVRDEVHDR working_dev,PVRIO mother);
extern void	NEAR _Cdecl FreeVrio(PVRIO io);

extern USHORT 	NEAR _Cdecl AllocateFixed(ULONG size, PLIN lin);
extern USHORT 	NEAR _Cdecl FreeFixed(LIN lin);

extern void	NEAR _Cdecl CrcInit(USHORT uPOLYNOM);
extern USHORT	NEAR _Cdecl Crc16(PVOID buffer,USHORT cb);



/* Module: init1.c */

extern DRVSTATUS NEAR InitBase(PRPINITIN pRPI);
extern int	NEAR ValidateVSec2(PVRDEVHDR const child,
				   SEC_VRDEV2 const FAR * const sec);



/* Module: init2.c */

extern void	NEAR CompleteInit(PIORB_CONFIGURATION const pIorb);


/* Module: printk.c */

extern void	NEAR sprintk(char FAR *outs,const char FAR *fmt,...);

#if defined(DEBUG)
/* Module: dprintf.asm (really void _saveregs but...) */

extern int	NEAR _Cdecl dprintf(char FAR * msg,...);
#endif

