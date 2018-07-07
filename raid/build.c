/*
 * $Source: R:/source/driver/raid/RCS\\build.c,v $
 * $Revision: 1.13 $
 * $Date: 2001/07/22 00:26:01 $
 * $Locker:  $
 *
 *	BUILD Process.  Triggered by Timer() in iorb.c
 *
 * $Log: build.c,v $
 * Revision 1.13  2001/07/22 00:26:01  vitus
 * - BuildNotify: arms context hook on WRITE completion to start the next I/O
 *
 * Revision 1.12  2001/06/25 00:27:56  vitus
 * - builds and rebuilds now allocate a big (well, 64KB) buffer and
 *   are therefore faster than before (4KB).  The bigger buffer requires
 *   a real scatter/gather list and a context hook to free it when
 *   to process completed (see BuildEnd)
 *
 * Revision 1.11  2001/01/15 01:17:47  vitus
 * - UpdateCfg: new 'forced' parameter which allows writing of
 *   configuration data even if vsetup is running.
 *
 * Revision 1.10  2000/10/24 23:56:16  vitus
 * - BuildCreate: parameter 'mode' to distinguish build and rebuild
 * - BuildNotify: clears BUILD *and* REBUILD flags when complete
 *
 * Revision 1.9  1999/10/01 00:56:36  vitus
 * - UpdateCfg: return whether update was really started or not.
 *
 * Revision 1.8  1999/09/20 00:21:52  vitus
 * - defined all DBGMSG() strings as located in CONST segment
 *
 * Revision 1.7  1999/07/06 02:04:18  vitus
 * - corrected calling convention of BuildNotify(), UpdateNotify(): _Cdecl
 * - BuildNotify(), UpdateNotify(): protect via cli()
 *
 * Revision 1.6  1999/07/05 01:50:44  vitus
 * - UpdateCfg: just do nothing if not completely initialized or setup still active
 *
 * Revision 1.5  1999/06/21 02:36:19  vitus
 * - corrected english spelling
 *
 * Revision 1.4  1999/05/26 00:31:09  vitus
 * - BuildNotify: clears RDFLAG_BUILDING when build completed.  This flag
 *   is managed by build.c itself.
 *
 * Revision 1.3  1999/05/12 01:32:35  vitus
 * - UpdateCfg() and related routines: implemented and tested
 *
 * Revision 1.2  1999/04/23 01:55:37  vitus
 * - BuildNotify: cleanup
 * - SaveArrayInfos: dummy
 *
 * Revision 1.1  1999/04/17 00:11:10  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2001
 */
#if defined(__WATCOMC__)
# pragma off (unreferenced)
#endif
static char const id[]="$Id: build.c,v 1.13 2001/07/22 00:26:01 vitus Exp $";
#if defined(__WATCOMC__)
# pragma on (unreferenced)
#endif

#include <string.h>

#define INCL_NOBASEAPI
#define INCL_NOPMAPI
#define INCL_INITRP_ONLY
#include "os2intf.h"

#include "vraid.h"
#include "proto.h"
#include "extern.h"




CONSTMSG szBuildStart[]=	"Starting %sbuild process (at %u%%)";
CONSTMSG szBuildMemFail[]=	"Out of memory while starting [re]build process";

#if defined(DEBUG)
CONSTMSG dszUpdateNotify[]=	"\r\n[UpdateNotify] io %p, error %w";
CONSTMSG dszUpdateKeep[]=	"\r\n[UpdateNotify] reusing request";
CONSTMSG dszBuildNotify[]=	"\r\n[BuildNotify] io %p, error %w";
CONSTMSG dszBuildKeep[]=	"\r\n[BuildNotify] reusing request";
CONSTMSG dszBuildWorker[]=	"\r\n[BuildWorker] entry";
CONSTMSG dszBW_LTPL[]=		"\r\n[BuildWorker] LinToPageList - rc %w";
CONSTMSG dszBE_ACH[]=		"\r\n[BuildEnd] ArmCtxHook - rc %w";
CONSTMSG dszBN_ACH[]=		"\r\n[BuildNotify] ArmCtxHook - rc %w";
CONSTMSG dszUpdateCfg[]=	"\r\n[UpdateCfg] entry";
CONSTMSG dszUpdateSetup[]=	"\r\n[UpdateCfg] SETUPACTIVE|!INIT2DONE, update delayed!";
CONSTMSG dszStr[]=		"\r\n%s";
CONSTMSG dszSaveArrayInfos[]=	"\r\nSaveArrayInfos()";
#endif




PRIVATE void NEAR	BuildEnd(PBUILDPROCESS const proc);




/* **********************************************************************
 * **** Private Routines ************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * UpdateNotify(io)
 *
 * PARAMETER
 *	io		returned from device to build
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Called as notification routine whenever a VRDEVICE wants
 *	to update it's own configuration sector.
 *
 * REMARKS
 */
PUBLIC void NEAR _Cdecl
UpdateNotify(PVRIO const io)
{
    BOOL	keep_data = FALSE;


    DEBMSG2(DBG_LEVEL1,dszUpdateNotify,io,io->oserr);
    ASSERT( io->mother == &vrioUpdate );

    pushf();
    cli();

    if( io->oserr == 0 )
    {
	switch( io->iocmd )
	{
	  case IOCMD_AREAD:
	  {
	    PVRIO	writeio;

	    /* We have successfully read the configuration sector of an
	     * array.  Now tell that array to update the information */

	    if( io->vrd->update(io->vrd,
				MK_FP(SELECTOROF(io->pSGList),
				      io->pSGList->ppXferBuf-ppWorkMem)) == 0 )
		break;				/* kein Update n”tig */


	    /* Write the sector back to medium. */

	    writeio = AllocateVrio(io->vrd, &vrioUpdate);
	    if( writeio == NULL )
		break;				/* free data and retry later */

	    writeio->iocmd = IOCMD_AWRITE;

	    writeio->cSGList = io->cSGList;
	    writeio->pSGList = io->pSGList;
	    writeio->ppSGList = io->ppSGList;

	    writeio->offset = io->offset;
	    writeio->blk = io->blk;
	    writeio->bcnt = io->bcnt;
	    QueueVrio(writeio);
	    keep_data = TRUE;
	  }
	  break;

	  case IOCMD_AWRITE:
	    break;

	  default:
	    ASSERT(0);
	    break;
	}
    }


    popf();


    if( keep_data == TRUE )
    {
	DEBMSG(DBG_MEMORY,dszUpdateKeep);
    }
    else
    {
	MemFree(MK_FP(SELECTOROF(io->pSGList),io->pSGList->ppXferBuf-ppWorkMem));
	MemFree(io->pSGList);
    }
    return;					/* Aufrufer gebe 'io' frei */
}




/*# ----------------------------------------------------------------------
 * BuildNotify(io)
 *
 * PARAMETER
 *	io		returned from device to build
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Called as notification routine for all I/O relating to
 *	building/rebuilding data area.
 *
 *	- IOCMD_READ
 *	- IOCMD_WRITE		the process of copying all user data
 *				from valid to invalid children converting the
 *				invalid to valid children.
 *
 * REMARKS
 */
PUBLIC void NEAR _Cdecl
BuildNotify(PVRIO const io)
{
    PBUILDPROCESS	proc = NULL;
    BOOL		keep_data = FALSE;
    USHORT	i;


    DEBMSG2(DBG_PATH,dszBuildNotify,io,io->oserr);
    ASSERT( io->mother == &vrioBuild );

    pushf();
    cli();

    /* Look for the matching entry in aBuildProcess[]. */

    for( i = 0; i < sizeof(aBuildProcess)/sizeof(aBuildProcess[0]); ++i )
    {
	if( aBuildProcess[i].vrd == io->vrd )
	{
	    proc = &aBuildProcess[i];
	    break;
	}
    }
    ASSERT( proc != NULL  &&  proc->vrd == io->vrd );


    if( io->oserr == 0 )
    {
	switch( io->iocmd )
	{
	  case IOCMD_READ:
	  {
	    PVRIO	writeio;

	    /* We have successfully read some sectors from the device to
	     * build.  Now write the data back to the very same sectors.  This
	     * is done by changing the iocmd from IOCMD_READ to IOCMD_WRITE.
	     * OBS: as 'io' will soon be freed by our caller we need a
	     * new VRIO.  Allocate one and fill it with the same values. */

	    writeio = AllocateVrio(proc->vrd, &vrioBuild);
	    if( writeio == NULL )
		break;				/* free data and retry later */

	    writeio->iocmd = IOCMD_WRITE;

	    writeio->cSGList = io->cSGList;
	    writeio->pSGList = io->pSGList;
	    writeio->ppSGList = io->ppSGList;

	    writeio->offset = io->offset;
	    writeio->blk = io->blk;
	    writeio->bcnt = io->bcnt;
	    QueueVrio(writeio);
	    keep_data = TRUE;
	  }
	  break;

	  case IOCMD_WRITE:
	    proc->next = io->blk + io->bcnt;

	    if( proc->next == proc->vrd->size )
	    {
		/* All of the driver has been read/written.  Remove this build
		 * process as it has done it's duty.
		 * The fail-safe device itself will rewrite it's configuration
		 * sector as it is the only one to know:
		 * 1. it's layout
		 * 2. whether this read/write process completed without
		 *	another failure on the to-be-rebuild child */

		BuildEnd(proc);
	    }
	    break;

	  default:
	    ASSERT(0);
	    break;
	}
    }


    popf();


    if( keep_data == TRUE )
    {
	DEBMSG(DBG_MEMORY,dszBuildKeep);
    }
    else
    {
	MemFree(io->pSGList);
	proc->busy = FALSE;			/* => retry later */

	i = DevHelp_ArmCtxHook(0, hdBuildHook);
	if( i != 0 )
	{
	    DEBMSG1(DBG_LEVEL1,dszBN_ACH,i);
	}
    }
    return;					/* Aufrufer gebe 'io' frei */
}




/*# ----------------------------------------------------------------------
 * BuildWorker(proc)
 *
 * PARAMETER
 *	proc		description of build proces
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Starts the next I/O to complete BUILD process.
 *
 * REMARKS
 */
PRIVATE void NEAR
BuildWorker(PBUILDPROCESS const proc)
{
    PSCATGATENTRY	scga;
    PVRIO		vrio;

    DEBMSG(DBG_PATH,dszBuildWorker);
    ASSERT( !proc->busy );

    /* There is a BUILD to keep alive and it isn't busy right
     * now: allocate Sc/Ga list and VRIO, fill them and
     * add I/O to queue. */

    do
    {
	USHORT const	this_bcnt = (proc->vrd->size - proc->next > BUILD_IOSIZE
				     ? BUILD_IOSIZE
				     : (USHORT)(proc->vrd->size - proc->next));
	USHORT const	sgl_size = (sizeof(SCATGATENTRY)
				    * (BUILD_IOSIZE * (ULONG)SECTOR_SIZE
				       / PAGESIZE + 1));
	LIN		lin_of_scga;
	USHORT		error;
	ULONG		ul;


	scga = MemAlloc(sgl_size);
	if( scga == NULL )
	    break;				/* do it next time */

	vrio = AllocateVrio(proc->vrd, &vrioBuild);
	if( vrio == 0 )
	{
	    MemFree(scga);
	    break;
	}


	/* VRIO: only change things != 0.
	 * Setup scatter/gather list */

	error = DevHelp_VirtToLin(SELECTOROF(scga), OFFSETOF(scga),
				  &lin_of_scga);
	if( error == 0 )
	    error = DevHelp_LinToPageList(proc->lin_buffer,
					  this_bcnt * (ULONG)SECTOR_SIZE,
					  lin_of_scga, &ul);
	if( error != 0 )
	{
	    DEBMSG1(DBG_LEVEL1,dszBW_LTPL,error);
	    FreeVrio(vrio);
	    MemFree(scga);
	    break;				/* perhaps next time */
	}

	vrio->pSGList = scga;
	vrio->ppSGList = ppWorkMem + FP_OFF(scga);
	vrio->cSGList = (USHORT)ul;

	vrio->iocmd = IOCMD_READ;
	vrio->offset = 0;
	vrio->blk = proc->next;
	vrio->bcnt = this_bcnt;

	proc->busy = TRUE;
	QueueVrio(vrio);			/* append, will be started soon */
    }
    while(0);

    return;
}






/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * UpdateCfg(vrd)
 *
 * PARAMETER
 *	vrd		array information
 *	forced		write even if setup still active
 *
 * RETURNS
 *	TRUE		Update triggered
 *	FALSE		busy, not allowed now, ...
 *
 * DESCRIPTION
 *	We need to rewrite the configuration sector for some reason (build
 *	completed, child failed, vsetup requested it, ...).
 *	Start this as last step from a notifications routine to allow
 *	resources from the I/O, we just got notified for, to be
 *	freed (CompleteVrio).
 *
 *	This rewrite isn't executed if the disk may have changed without driver
 *	notice (via VSetup.exe).
 *
 * REMARKS
 */
PUBLIC BOOL NEAR
UpdateCfg(PVRDEVHDR const vrd, BOOL forced)
{
    PVOID		buffer;
    PSCATGATENTRY	scga;
    PVRIO		vrio;
    BOOL		started = FALSE;


    DEBMSG(DBG_LEVEL1,dszUpdateCfg);

    if( !forced  &&  !((fDriverFlags & DF_INIT2DONE) != 0
		       &&  (fDriverFlags & DF_SETUPACTIVE) == 0) )
    {
	DEBMSG(DBG_LEVEL1,dszUpdateSetup);
	return FALSE;				/* don't write to disk now */
    }

    do
    {
	buffer = MemAlloc(SECTOR_SIZE);
	if( buffer == NULL )
	    break;

	scga = MemAlloc(sizeof(*scga));
	if( scga == NULL )
	{
	    MemFree(buffer);
	    break;
	}

	vrio = AllocateVrio(vrd, &vrioUpdate);
	if( vrio == NULL )
	{
	    MemFree(buffer);
	    MemFree(scga);
	    break;
	}

	/* VRIO: only change things != 0. */

	vrio->iocmd = IOCMD_AREAD;

	vrio->pSGList = scga;
	vrio->ppSGList = ppWorkMem + FP_OFF(scga);
	vrio->cSGList = 1;
	vrio->pSGList->ppXferBuf =  ppWorkMem + FP_OFF(buffer);
	vrio->pSGList->XferBufLen = 1 * (ULONG)SECTOR_SIZE;

	vrio->offset = 0;
	vrio->bcnt = 1;
	vrio->blk = 0;

	vrio->oserr = 0xFFFF;

	QueueVrio(vrio);
	started = TRUE;
    }
    while( 0 );

    return started;
}




/*# ----------------------------------------------------------------------
 * UpdateBuilds(void)
 *
 * PARAMETER
 *	(none)
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Called from build context hook which is armed by timer and
 *	any completed build I/O.  Starts a read operation for
 *	every BUILDPROCESS whenever the previous read/write cycle has
 *	completed.
 *	Operation is started by inserting a READ into the matching
 *	VRDEVHDR queue.  Timer() will execute and the READ notification
 *	will start the WRITE.
 *
 * REMARKS
 *	The write notification isn't used to start the next READ because
 *	- we don't want to flood the stack (no read callbacks
 *	  in silly coded ADD)
 *	- it's easier to handle I/O errors.  Just stop the notification
 *	  actions and UpdateBuild() will restart the last.
 */
PUBLIC void NEAR
UpdateBuilds(void)
{
    USHORT	i;

    for( i = 0; i < sizeof(aBuildProcess)/sizeof(aBuildProcess[0]); ++i )
    {
	if( aBuildProcess[i].vrd != 0  &&  aBuildProcess[i].busy == 0 )
	    BuildWorker(&aBuildProcess[i]);
    }

    return;
}




/*# ----------------------------------------------------------------------
 * BuildEnd(proc)
 *
 * PARAMETER
 * RETURNS
 * DESCRIPTION
 *	Ends build process
 *
 * REMARKS
 */
PRIVATE void NEAR
BuildEnd(PBUILDPROCESS const proc)
{
    USHORT rc;

    rc = DevHelp_ArmCtxHook(proc->lin_buffer, hdMemHook);
    if( rc != 0 )
    {
	DEBMSG1(DBG_LEVEL1,dszBE_ACH,rc);
    }

    proc->lin_buffer = 0;
    proc->vrd->flags &= ~(RDFLAG_BUILDING | RDFLAG_REBUILD);
    proc->vrd = NULL;

    /* xxx	Somebody should decrement 'cBeeps' to stop the sound.  But
     * as [re-]starting a build at runtime isn't implemented so far,
     * we may ignore the beeps. */

    return;
}




/*# ----------------------------------------------------------------------
 * BuildCreate(vrd,next_sector,mode)
 *
 * PARAMETER
 *	vrd		device which need [re]build
 *	next_sector	from here on (sector number, 0: total device)
 *	mode		0	build
 *			1	rebuild
 *
 * RETURNS
 *	0		OK
 *	/0		error ocde (no memory available)
 *
 * DESCRIPTION
 *	Adds a device to the list of active builds.  Doesn't do any
 *	I/Os, this is done at interrupt time whenever build is
 *	allowed.
 *
 * REMARKS
 *	This routine is *always* run at task time and may therefore block.  But
 *	as it's not documented that way we should use that feature.  Let's just
 *	say "there is a lot of time, it doesn't matter how mcuh you take".
 */
PUBLIC USHORT NEAR
BuildCreate(PVRDEVHDR vrd, ULONG next_sector, USHORT mode)
{
    USHORT	i;
    USHORT	error = STATUS_ERR_PAPEROUT;	/* assume: no free entry */

    for( i = 0; i < sizeof(aBuildProcess)/sizeof(aBuildProcess[0]); ++i )
    {
	if( aBuildProcess[i].vrd == 0 )
	{
	    /* Fill structure and allocate a buffer that is locked
	     * down in memory and is addressable from all process
	     * contextes as we don't know when the notification will
	     * be called from the ADD. */

	    aBuildProcess[i].vrd = vrd;
	    aBuildProcess[i].next = next_sector;
	    aBuildProcess[i].busy = 0;

	    error = AllocateFixed(BUILD_IOSIZE * (ULONG)SECTOR_SIZE,
				  &aBuildProcess[i].lin_buffer);
	    if( error != 0 )
	    {
		aBuildProcess[i].vrd = 0;	/* free entry */

		if( (fDriverFlags & DF_INIT1DONE) == 0 )
		{
		    sprintk(szMsgBuffer, szBuildMemFail);
		    DEBMSG1(DBG_LEVEL1,dszStr,(PSZ)szMsgBuffer);
		    if( (fDriverFlags & DF_VERBOSE) )
			SaveMessage();
		}

		aBuildProcess[i].vrd = 0;	/* no build possible */
		break;
	    }

	    if( (fDriverFlags & DF_INIT1DONE) == 0 )
	    {
		sprintk(szMsgBuffer, szBuildStart,
			(mode ? (PVOID)"re" : (PVOID)""),
			(next_sector*100)/vrd->size);
		DEBMSG1(DBG_LEVEL1,dszStr,(PSZ)szMsgBuffer);
		if( (fDriverFlags & DF_VERBOSE) )
		    SaveMessage();
	    }


	    ++cBuildProcess;			/* one more */
	    break;
	}
    }

    return error;
}




/*# ----------------------------------------------------------------------
 * SaveArrayInfos(void)
 *
 * PARAMETER
 *	(none)
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Calls UpdateCfg() for every VRDEVICE with active build process.
 *
 * REMARKS
 */
PUBLIC void NEAR
SaveArrayInfos(void)
{
    USHORT	i;

    DEBMSG(DBG_LEVEL1,dszSaveArrayInfos);
    for( i = 0; i < sizeof(aBuildProcess)/sizeof(aBuildProcess[0]); ++i )
    {
	if( aBuildProcess[i].vrd != 0 )
	{
	    UpdateCfg(aBuildProcess[i].vrd, FALSE);
	}
    }
    return;
}
