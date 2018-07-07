/*
 * $Source: R:/source/driver/raid/RCS\\mirror.c,v $
 * $Revision: 3.10 $
 * $Date: 2001/07/25 01:56:52 $
 * $Locker:  $
 *
 *
 *	How to do RAID 1 I/O.
 *
 *	Children to a RAID 1 device build a table.
 *	Read I/Os will do round robbing on reads.
 *
 *
 * $Log: mirror.c,v $
 * Revision 3.10  2001/07/25 01:56:52  vitus
 * - MirrorCreate: entries in tab[] got lost when the creation was postponed.  Fixed
 *
 * Revision 3.9  2001/07/08 18:36:53  vitus
 * - implemented NextValidChild() to select next device to read.  When there
 *   is no valid device the array state is changed to ERROR
 *
 * Revision 3.8  2001/06/25 00:31:08  vitus
 * - BuildCreate() now returns an error code.  Check and pass to caller
 *
 * Revision 3.7  2001/02/11 01:15:18  vitus
 * - changed MirrorCtrl() to return DRVSTATUS
 *
 * Revision 3.6  2001/01/30 00:31:47  vitus
 * - BUILD and REBUILD are now mostly the same except statt REBUILD is
 *   possible w/o an invalid child.  In this case rebuild_sec is used.
 *
 * Revision 3.5  2001/01/15 01:14:59  vitus
 * - VRDIOCTL_START_REBUILD: call UpdateCfg() immediately, we need that data
 *   on disk and can't wait to the next shutdown.  Someone my restart vsetup
 *   and wants to see the correct state.
 *
 * Revision 3.4  2000/10/24 23:58:39  vitus
 * - changes for new BuildCreate() parameter 'mode'
 * - more debugging output
 *
 * Revision 3.3  2000/08/22 01:59:40  vitus
 * - implemented VRAID_SET_CHILD_STATE
 *
 * Revision 3.2  2000/07/23 00:19:01  vitus
 * - code distinguishes between BUILD and REBUILD
 * - implemented ioctl method VRDIOCTL_START_REBUILD
 *
 * Revision 3.1  2000/06/15 01:01:22  vitus
 * - implemented ioctl method (VRDIOCTL_QUERY_CHILDREN, VRDIOCTL_INFO, VRDIOCTL_VERIFY)
 *
 * Revision 2.8  1999/12/14 00:30:44  vitus
 * - moved ValidateVSec2() to init1.c
 *
 * Revision 2.7  1999/10/01 01:04:26  vitus
 * - updates new iocnt and busycnt fields in VRDEVHDR
 * - MirrorCreate: detect when RDFLAG_BUILDING still exist but building has
 *   already completed.  Build is *not* started but update of configuration
 *   is triggered.
 * - MirrorNotify: only clear need_updatecfg if UpdateCfg() says it has
 *   really started the update...
 *
 * Revision 2.6  1999/09/20 00:24:44  vitus
 * - defined all DBGMSG() strings as located in CONST segment
 *
 * Revision 2.5  1999/08/09 23:18:33  vitus
 * - MirrorCreate: initialize new VRDMIRROR structure with 0
 *
 * Revision 2.4  1999/06/21 02:33:46  vitus
 * - corrected english spelling
 * - MirrorNotify: did switch to same child on I/O errors, fixed
 *
 * Revision 2.3  1999/06/20 17:34:15  vitus
 * - more code to avoid doing I/O to a missing child.  Old revision could access
 *   NULL pointers when the first child was missing
 *
 * Revision 2.2  1999/05/26 01:14:22  vitus
 * - lock a failed child from further access (perhaps this lock could be lifted
 *   by an IOCtl?)
 *
 * Revision 2.1  1999/05/24 19:18:13  vitus
 * - detects and handles situations were an out-of-data child is
 *   found
 *
 * Revision 1.11  1999/05/24 00:39:24  vitus
 * - MirrorUpdate() complete
 * - MirrorNotify: triggers update of configuration sectors whenever an state
 *   change occured.
 *
 * Revision 1.10  1999/05/12 01:47:33  vitus
 * - new SEC_VRDEV2 layout
 * - handles valid/non-valid per child
 *
 * Revision 1.9  1999/04/28 01:45:19  vitus
 * - merged MirrorAdmin() code with Worker/Notify, IOCMD_A* commands
 * - MirrorNotify() uses CompleteVrio()
 * - dummy MirrorUpdate() code
 *
 * Revision 1.8  1999/04/14 01:22:48  vitus
 * - implemented MirrorAdmin() method
 * - changed for iorb -> vrio transition
 * - changes for valid/not valid childs and build process, still untested
 *
 * Revision 1.7  1999/01/11 00:59:29  vitus
 * - MirrorNotify: corrected, did free wrong VRIO, call wrong notification, etc.
 *
 * Revision 1.6  1998/09/20 14:26:21  vitus
 * - modified for AllocateVrio(working_dev,mother)
 * - clarified pointer naming (father -> myio)
 *
 * Revision 1.5  1998/06/26 22:58:24  vitus
 * - switched to DDK macros (cli, sti, pushf)
 * - worker,notify get const pointers and become _Cdecl
 *
 * Revision 1.4  1998/05/29 01:53:38  vitus
 * - chnages for new prototype of AllocateVrio()
 * - PUBLICs are now _Cdecl
 *
 * Revision 1.3  1998/03/10 02:08:53  vitus
 * - changed to PUBLIC *Create() function and PRIVATE workers
 *
 * Revision 1.2  1998/03/06 01:23:25  vitus
 * - Worker/Notify entries Need public?)
 *
 * Revision 1.1  1998/02/12 01:31:46  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2001
 */
#if defined(__WATCOMC__)
# pragma off (unreferenced)
#endif
static char const id[]="$Id: mirror.c,v 3.10 2001/07/25 01:56:52 vitus Exp $";
#if defined(__WATCOMC__)
# pragma on (unreferenced)
#endif

#include <string.h>

#define INCL_NOBASEAPI
#define INCL_INITRP_ONLY
#include "os2intf.h"

#include "vraid.h"
#include "proto.h"
#include "extern.h"
#include "vraidpub.h"



/*
 * Our object: the mirror drive.
 */
typedef struct _VRDMIRROR {
    VRDEVHDR	header;				/* ALWAYS FIRST! */

    USHORT	readchild;			/* read from this child */
    BOOL	need_updatecfg;

    ULONG	rebuild_sec;			/* secno reached in rebuild */

    USHORT	children;			/* entries in table below */
    struct {
	PVRDEVHDR	vrd;
	ULONG		valid;			/* 0 - vrd->size	needs update
						   0xFFFFFFFF		valid */
	struct {
	    int		missing : 1;
	};
    } child[];

} VRDMIRROR, FAR * PVRDMIRROR;



PRIVATE USHORT NEAR _Cdecl	MirrorWorker(PVRDEVHDR const self,PVRIO const myio);
PRIVATE USHORT NEAR _Cdecl	MirrorUpdate(PVRDEVHDR const self,PVOID const sec);
PRIVATE DRVSTATUS NEAR _Cdecl	MirrorCtrl(PVRDEVHDR const self, USHORT const cmd,
					   PVOID const param, PVOID const data);




INITMSG	szMissingChild[]=	"MIRROR, fail state: %u child(s) missing";
INITMSG	szCreationDelayed[]=	"MIRROR creation delayed";

#if defined(DEBUG)
CONSTMSG dszMirrorNotify[]=	"\r\n[MirrorNotify] io %p, myio %p, err %w";
CONSTMSG dszMNotifyReStop[]=	"\r\n[MirrorNotify] rebuild now complete";
CONSTMSG dszMNotifyValid[]=	"\r\n[MirrorNotify] child %w now valid";
CONSTMSG dszMNotifyUStart[]=	"\r\n[MirrorNotify] starting UpdateCfg()";
CONSTMSG dszMNotifyUDefer[]=	"\r\n[MirrorNotify] deferring UpdateCfg()";
CONSTMSG dszMirrorWorker[]=	"\r\n[MirrorWorker] %p";
CONSTMSG dszMWorkerRelease[]=	"\r\n[MirrorWorker] releasing VRIOs";
CONSTMSG dszMWorkerBusy[]=	"\r\n[MirrorWorker] child busy, queuing VRIO";
CONSTMSG dszMirrorCreate[]=	"\r\n[MirrorCreate] at %p"
				"\r\n               ID %z"
				"\r\n               Size 0x%lx";
CONSTMSG dszMCreateAdminData[]=	"\r\n[MirrorCreate] admin data\r\n%z";
CONSTMSG dszStr[]=		"\r\n%s";
CONSTMSG dszMCreateNoBuild[]=	"\r\n[MirrorCreate] sector says [RE]BUILD, all children valid -> clear [RE]BUILD";
CONSTMSG dszMCreateBuild[]=	"\r\n[MirrorCreate] BUILD, a child is invalid";
CONSTMSG dszMCreateRebuild[]=	"\r\n[MirrorCreate] flags say REBUILD, at 0x%lx";
CONSTMSG dszMUpdateRebuild[]=	"\r\n[MirrorUpdate] storing REBUILD flag, at 0x%lx";
#endif






/* **********************************************************************
 * **** Private Routines ************************************************
 * ******************************************************************* */

PRIVATE USHORT NEAR
Vrd2Child(PVRDMIRROR const mirror,PVRDEVHDR const child)
{
    USHORT	i;

    for( i = 0; i < mirror->children; ++i )
    {
	if( child == mirror->child[i].vrd )
	    return i;
    }
    return (USHORT)-1;
}




/*# ----------------------------------------------------------------------
 * NextValidChild(mirror, previous)
 *
 * PARAMETER
 *	mirror		a VRD which forms a mirror device
 *	previous	the previously used child
 *
 * RETURNS
 *	child index or (USHORT)-1
 *
 * DESCRIPTION
 *	Selects another valid child.
 *
 * REMARKS
 */
PRIVATE USHORT
NextValidChild(PVRDMIRROR const mirror, USHORT const previous)
{
    USHORT	next = previous;
    USHORT	tries = mirror->children;
    BOOL	found = FALSE;

    do
    {
	next = (next + 1 == mirror->children ? 0 : next + 1);
	if( mirror->child[next].vrd != NULL
	    &&  !mirror->child[next].missing
	    &&  mirror->child[next].valid == (ULONG)-1 )
	{
	    found = TRUE;
	    break;
	}
    }
    while( tries-- );
    if( !found )
    {
	mirror->header.flags &= ~RDFLAG_STATEMASK;
	mirror->header.flags |= RDFLAG_ERROR;	/* ERROR state */
	mirror->need_updatecfg = TRUE;
	return (USHORT)-1;
    }
    return next;
}




/*# ----------------------------------------------------------------------
 * MirrorNotify(io)
 *
 * PARAMETER
 *	io			I/O chich completed
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Notified by child when it has completed an I/O.
 *	At task time only a single child is called to execute
 *	a specific I/O, when this I/O has completed it is time to check
 *	whether it was successfull.  If not: choose another child!
 *
 * REMARKS
 */
PRIVATE VOID NEAR _Cdecl
MirrorNotify(PVRIO const io)
{
    PVRIO const		myio = io->mother;		/* I/O to us */
    PVRDMIRROR const	mirror = (PVRDMIRROR)myio->vrd; /* it's me */
    USHORT const	childidx = Vrd2Child(mirror, io->vrd);


    DEBMSG3(DBG_PATH,dszMirrorNotify,io,myio,io->oserr);

    ASSERT( io->bcnt != 0 );
    ASSERT( io->link == NULL );
    ASSERT( myio->link == NULL );
    ASSERT( myio->bcnt != 0 );
    ASSERT( myio->vrd->size != 0 );
    ASSERT( myio->vrd->notify == MirrorNotify );
    ASSERT( myio->vrd->worker == MirrorWorker );
    ASSERT( myio->vrd->update == MirrorUpdate );
    ASSERT( myio->vrd->ioctl == MirrorCtrl );

    pushf();
    cli();


    /* Error handling, keep OKs
     * Force update if anything failed and this hasn't already be marked
     * on disk. */

    if( io->oserr != 0 )
    {
	if( mirror->child[childidx].valid != 0 )
	{
	    mirror->child[childidx].valid = 0;
	    mirror->child[childidx].missing = 1;
	    mirror->need_updatecfg = TRUE;	/* we *need* an update */
	}
	if( !(mirror->header.flags & RDFLAG_FAIL)	&&
	    !(mirror->header.flags & RDFLAG_ERROR) )
	{
	    mirror->header.flags &= ~RDFLAG_STATEMASK;
	    mirror->header.flags |= RDFLAG_FAIL;
	    ++cBeeps;
	    mirror->need_updatecfg = TRUE;	/* we *need* an update */
	}
    }
    if( myio->oserr == 0xFFFF )			/* still not set? */
	myio->oserr = io->oserr;		/* propagate error */
    else if( myio->oserr != 0 )			/* any error until now? */
	myio->oserr = io->oserr;		/* keep this (OK?) */


    switch( io->iocmd )
    {
	/* **** WRITE data area **** */
      case IOCMD_WRITE:
	--myio->children;
	if( io->oserr == 0 )
	{
	    if( (mirror->header.flags & RDFLAG_BUILDING)
		||  (mirror->header.flags & RDFLAG_REBUILD) )
	    {
		/* Update [re]build variables when build is in effect and
		 * this child isn't valid already. */

		if( io->blk <= mirror->child[childidx].valid
		    &&  io->blk + io->bcnt > mirror->child[childidx].valid )
		{
		    mirror->child[childidx].valid = io->blk + io->bcnt;
		    if( mirror->child[childidx].valid == mirror->header.size )
		    {
			/* Build process is now complete.  Mark configuration
			 * sectors.
			 * The process itself should close down automatically. */

			DEBMSG1(DBG_ALL,dszMNotifyValid,childidx);
			mirror->child[childidx].valid = (ULONG)-1;
			mirror->need_updatecfg = TRUE;
		    }
		}
		if( (mirror->header.flags & RDFLAG_REBUILD) )
		{
		    if( io->blk <= mirror->rebuild_sec
			&&  io->blk + io->bcnt > mirror->rebuild_sec )
		    {
			mirror->rebuild_sec = io->blk + io->bcnt;
			if( mirror->rebuild_sec == mirror->header.size )
			{
			    DEBMSG(DBG_ALL,dszMNotifyReStop);
			    mirror->need_updatecfg = TRUE;
			    mirror->header.flags &= ~RDFLAG_REBUILD;
			    mirror->rebuild_sec = 0; /*OK? Do we need to set this?*/
			}
		    }
		}
	    }
	}
	break;

      case IOCMD_AWRITE:
	/* **** AWRITE ****
	 * If last I/O, call parent notification. */

	--myio->children;
	break;

      case IOCMD_READ:
      case IOCMD_AREAD:
	/* **** READ ****
	 * **** AREAD ****
	 * Try next device if error.  Else call parent notification. */

	if( io->oserr == 0 )
	{
	    myio->children = 0;			/* no more I/Os, see below */
	}
	else
	{
	    PVRDEVHDR	child = io->vrd;	/* child which completed */

	    --myio->children;
	    if( myio->children != 0 )
	    {
		PVRIO	newio;
		USHORT	rc;
		USHORT	i, found = 0;

		/* Select next child. */

		for( i = 0; i < mirror->children; ++i )
		    if( child == mirror->child[i].vrd )
		    {
			i = (i + 1 == mirror->children ? 0 : i+1);
			child = mirror->child[i].vrd;
			found = 1;
			break;
		    }
		ASSERT( found == 1 );

		/* Allocate VRIO for this new child and start it.  We
		 * can't reuse 'io' as it will be free when we return
		 * from MirrorNotify! */

		newio = AllocateVrio(child, myio);
		if( newio == NULL )
		    myio->children = 0;		/* will end 'myio' with last error */
		else
		{
		    newio->iorb =	io->iorb;
		    newio->iocmd =	io->iocmd;
		    newio->cSGList =	io->cSGList;
		    newio->pSGList =	io->pSGList;
		    newio->ppSGList =	io->ppSGList;
		    newio->offset =	io->offset;
		    newio->bcnt =	io->bcnt;
		    newio->blk =	io->blk;

		    rc = newio->vrd->worker(newio->vrd, newio);
		    if( rc )
			QueueVrio(newio);
		}
	    } /* end[if(children!=0)] */
	}
	break;
    } /* end[switch] */


    if( myio->children == 0 )
    {
	/* Our task is now completed, all transfer has been done.
	 * Complete 'myio' by calling it's notification routine. */

	CompleteVrio(myio);

	/* If neccessary rewrite configuration sector.  This is
	 * delayed until all I/O regarding the data sector causing this
	 * update have been completed. */

	if( mirror->need_updatecfg == TRUE )
	{
	    DEBMSG(DBG_ALL,dszMNotifyUStart);
	    if( UpdateCfg(&mirror->header, FALSE) == TRUE )
		mirror->need_updatecfg = FALSE;
	}
    }
    else if( mirror->need_updatecfg == TRUE )
    {
	DEBMSG(DBG_ALL,dszMNotifyUDefer);
    }

    popf();
    return;
} /* end[MirrorNotify] */




/*# ----------------------------------------------------------------------
 * MirrorWorker(self,myio)
 *
 * PARAMETER
 *	mirror		a VRD which forms a mirror device
 *	myio		I/O to myself, distribute
 *
 * RETURNS
 *	0		OK, I/O started
 *	/0		not started
 *
 * DESCRIPTION
 *	Pass VRIO to I/O routines of every child on write or to
 *	a single child if read.
 *
 * REMARKS
 */
PRIVATE USHORT NEAR _Cdecl
MirrorWorker(PVRDEVHDR const self, PVRIO const myio)
{
    PVRDMIRROR const	mirror = (PVRDMIRROR)self;
    PVRIO	io;
    PVRIO	newio;
    PVRIO	firstio = NULL;

    DEBMSG1(DBG_PATH,dszMirrorWorker,(PVOID)myio);

    ASSERT( myio->vrd == self );
    ASSERT( myio->link == NULL );
    ASSERT( myio->bcnt != 0 );
    ASSERT( mirror->header.size != 0 );
    ASSERT( mirror->header.notify == MirrorNotify );
    ASSERT( mirror->header.worker == MirrorWorker );
    ASSERT( mirror->header.update == MirrorUpdate );
    ASSERT( mirror->header.ioctl == MirrorCtrl );


    if( (mirror->header.flags & RDFLAG_ERROR) )
    {
	myio->oserr = IOERR_DEVICE_DIAGFAIL;
	CompleteVrio(myio);
	return 0;
    }


    ++self->iocnt;
    switch( myio->iocmd )
    {
	USHORT	i;


      case IOCMD_WRITE:
	/* Write: create VRIO to all children. */

	myio->children = 0;

	for( i = 0; i < mirror->children; ++i )
	{
	    if( mirror->child[i].vrd == NULL )
		continue;			/* skip non-existing child */
	    if( mirror->child[i].missing )
		continue;			/* skip removed child */


	    /* Allocate VRIOs and update links. */

	    newio = AllocateVrio(mirror->child[i].vrd, myio);
	    if( newio == NULL )			/* memory exhausted? */
		break;

	    if( firstio == NULL )
		firstio = newio;		/* save this as first */
	    else
		io->link = newio;
	    io = newio;
	    io->link = NULL;
	    ++myio->children;

	    /* Children will receive exactly the same parameters
	     * as this is a mirror device. */

	    io->iorb = myio->iorb;
	    io->iocmd = myio->iocmd;
	    io->cSGList = myio->cSGList;
	    io->pSGList = myio->pSGList;
	    io->ppSGList = myio->ppSGList;

	    io->offset = myio->offset;
	    io->blk = myio->blk;
	    io->bcnt = myio->bcnt;
	    io->oserr = 0xFFFF;			/* invalid error code */
	}
	break;


      case IOCMD_READ:
	/* Read: create for 'readchild' child and
	 * do another in Notify if this first failed!
	 * Don't forget to increment 'readchild'. */

	myio->children = mirror->children;	/* to detect 'all children tried' */

	/* Allocate VRIOs and update links. */

	do
	{
	    ASSERT( mirror->readchild != (USHORT)-1 );
	    newio = AllocateVrio(mirror->child[mirror->readchild].vrd, myio);
	    if( newio == NULL )			/* memory exhausted? */
		break;

	    if( firstio == NULL )
		firstio = newio;		/* save this as first */
	    else
		io->link = newio;
	    io = newio;
	    io->link = NULL;

	    /* Child will receive exactly the same parameters
	     * as parent (mirror device). */

	    io->iorb = myio->iorb;
	    io->iocmd = myio->iocmd;
	    io->cSGList = myio->cSGList;
	    io->pSGList = myio->pSGList;
	    io->ppSGList = myio->ppSGList;

	    io->offset = myio->offset;
	    io->blk = myio->blk;
	    io->bcnt = myio->bcnt;
	    io->oserr = 0xFFFF;			/* invalid error code */

	    pushf();
	    cli();
	    mirror->readchild = NextValidChild(mirror, mirror->readchild);
	    popf();
	}
	while(0);
	break;


      case IOCMD_AWRITE:

	/* Write: create VRIO to *all* children. */

	myio->children = 0;

	for( i = 0; i < mirror->children; ++i )
	{
	    if( mirror->child[i].vrd == NULL )
		continue;			/* skip non-existing child */
	    if( mirror->child[i].missing != 0 )
		continue;			/* skip removed child */


	    /* Allocate VRIOs and update links. */

	    newio = AllocateVrio(mirror->child[i].vrd, myio);
	    if( newio == NULL )			/* memory exhausted? */
		break;

	    if( firstio == NULL )
		firstio = newio;		/* save this as first */
	    else
		io->link = newio;
	    io = newio;
	    io->link = NULL;
	    ++myio->children;

	    /* Children will receive exactly the same parameters as the
	     * configuration sectors are identical on all parts of a
	     * VRAID device. */

	    io->iorb = myio->iorb;
	    io->iocmd = myio->iocmd;
	    io->cSGList = myio->cSGList;
	    io->pSGList = myio->pSGList;
	    io->ppSGList = myio->ppSGList;

	    io->offset = myio->offset;
	    io->blk = myio->blk + 1;		/* as it is a configuraion sector */
	    io->bcnt = myio->bcnt;
	    io->oserr = 0xFFFF;			/* invalid error code */
	}
	break;


      case IOCMD_AREAD:
	/* Read: create for 'readchild' child and do another in Notify()
	 * if this first failed!
	 * Don't forget to increment 'readchild'. */

	myio->children = mirror->children;	/* to detect 'all children tried' */

	/* Allocate VRIOs and update links. */

	do
	{
	    ASSERT( mirror->readchild != (USHORT)-1 );
	    newio = AllocateVrio(mirror->child[mirror->readchild].vrd, myio);
	    if( newio == NULL )			/* memory exhausted? */
		break;

	    if( firstio == NULL )
		firstio = newio;		/* save this as first */
	    else
		io->link = newio;
	    io = newio;
	    io->link = NULL;

	    /* Child will receive the same parameters
	     * as parent (configuration sectors!). */

	    io->iorb = myio->iorb;
	    io->iocmd = myio->iocmd;
	    io->cSGList = myio->cSGList;
	    io->pSGList = myio->pSGList;
	    io->ppSGList = myio->ppSGList;

	    io->offset = myio->offset;
	    io->blk = myio->blk + 1;		/* as it is a configuration sector */
	    io->bcnt = myio->bcnt;
	    io->oserr = 0xFFFF;			/* invalid error code */

	    pushf();
	    cli();
	    mirror->readchild = NextValidChild(mirror, mirror->readchild);
	    popf();
	}
	while(0);
	break;
    } /* end[switch] */


    if( newio == NULL )
    {
	/* Not enough memory.  Free all allocated VRIOs
	 * and return error code to parent. */

	DEBMSG(DBG_ALL,dszMWorkerRelease);
	for( io = firstio; io != NULL; io = newio )
	{
	    newio = io->link;
	    io->link = NULL;
	    FreeVrio(io);
	    DO( io = NULL );
	}

	++self->busycnt;
	return 1;
    }


    /* All VRIOs were allocated, now pass them to children. */

    for( io = firstio; io != NULL; io = newio )
    {
	USHORT	rc;

	newio = io->link;
	io->link = NULL;
	rc = io->vrd->worker(io->vrd, io);
	if( rc )
	{
	    DEBMSG(DBG_ALL,dszMWorkerBusy);
	    QueueVrio(io);
	}
    }

    return 0;
} /* end[MirrorWorker] */




/*# ----------------------------------------------------------------------
 * MirrorUpdate(self,sec)
 *
 * PARAMETER
 *	mirror		a VRD which forms a mirror device
 *	sec		sector contains configuration
 *
 * RETURNS
 *	0		don't write back, nothing changed
 *	/0		modified, write back
 *
 * DESCRIPTION
 *	Update configuration sector if anything has changed.
 *
 * REMARKS
 */
PRIVATE USHORT NEAR _Cdecl
MirrorUpdate(PVRDEVHDR const self,PVOID const sec)
{
    PVRDMIRROR const	mirror = (PVRDMIRROR)self;
    PSEC_VRDEV2 const	cfg = sec;
    unsigned		i;


    ASSERT( mirror->header.size != 0 );
    ASSERT( mirror->header.notify == MirrorNotify );
    ASSERT( mirror->header.worker == MirrorWorker );
    ASSERT( mirror->header.update == MirrorUpdate );
    ASSERT( mirror->header.ioctl == MirrorCtrl );

    cfg->timestamp = pGlobalInfoSeg->SIS_BigTime;

    for( i = 0; i < mirror->children; ++i )
    {
	if( mirror->child[i].vrd == 0 )
	{
	    cfg->u.s.child[i].flags &= ~0x01;
	    cfg->u.s.child[i].valid = 0;	/* no sector at all */
	}
	else if( mirror->child[i].valid == (ULONG)-1 )
	{
	    cfg->u.s.child[i].flags |= 0x01;
	    cfg->u.s.child[i].valid = (ULONG)-1;
	}
	else
	{
	    cfg->u.s.child[i].flags &= ~0x01;	/* no completely valid */
	    cfg->u.s.child[i].valid = mirror->child[i].valid;
	}
    }


    if( (mirror->header.flags & RDFLAG_BUILDING) )
	cfg->u.s.flags |= RDFLAG_BUILDING;
    else
	cfg->u.s.flags &= ~RDFLAG_BUILDING;
    if( (mirror->header.flags & RDFLAG_REBUILD)
	&&  !(cfg->u.s.flags & RDFLAG_BUILDING) )
    {
	cfg->u.s.flags |= RDFLAG_REBUILD;
	cfg->rebuild = mirror->rebuild_sec;
	DEBMSG1(DBG_ALL,dszMUpdateRebuild,(PVOID)&cfg->rebuild);
	ASSERT( cfg->rebuild < mirror->header.size );
    }
    else
	cfg->u.s.flags &= ~RDFLAG_REBUILD;

    cfg->crc = Crc16(sec, SECTOR_SIZE-2);
    return 1;
} /* end[MirrorUpdate] */




/*# ----------------------------------------------------------------------
 * MirrorCtrl(self,cmd,param,data)
 *
 * PARAMETER
 *	mirror		a VRD which forms a mirror device
 *	cmd		I/O Control Code
 *	data		data buffer
 *
 * RETURNS
 *	0		OK
 *	/0		error (driver status code)
 *
 * DESCRIPTION
 *	Special function interface
 *
 * REMARKS
 */
PRIVATE DRVSTATUS NEAR _Cdecl
MirrorCtrl(PVRDEVHDR const self, USHORT const cmd,
	   PVOID const param, PVOID const data)
{
    PVRDMIRROR const	mirror = (PVRDMIRROR)self;
    DRVSTATUS		result = STATUS_ERR_UNKCMD;
    USHORT		i, j;


    ASSERT( mirror->header.size != 0 );
    ASSERT( mirror->header.notify == MirrorNotify );
    ASSERT( mirror->header.worker == MirrorWorker );
    ASSERT( mirror->header.update == MirrorUpdate );
    ASSERT( mirror->header.ioctl == MirrorCtrl );

    switch( cmd )
    {
      case VRDIOCTL_QUERY_CHILDREN:
	DEBMSG(DBG_PATH, "\r\nVRDIOCTL_QUERY_CHILDREN");
	*(PUSHORT)data = mirror->children;
	result = 0;
	break;


      case VRDIOCTL_INFO:
	DEBMSG(DBG_PATH, "\r\nVRDIOCTL_INFO");
	result = 0;
	{
	    PVRAID_ARRAYINFO_DATA	d = data;

	    _fmemcpy(d->id, mirror->header.id, sizeof(d->id));
	    d->reqcnt = mirror->header.iocnt;
	    d->size = mirror->header.size;

	    d->children = mirror->children;
	    d->flags = mirror->header.flags;
	    d->reserved = 0;
	}
	break;


      case VRDIOCTL_START_REBUILD:
	DEBMSG(DBG_PATH, "\r\nVRDIOCTL_START_REBUILD");
	result = 0;
	do
	{
	    PVRAID_REBUILD_PARM	p = param;

	    if( p->secno >= mirror->header.size )
	    {
		result = STATUS_ERR_NOTFOUND;
		break;
	    }
	    if( mirror->header.flags != 0 )	/* build, fail, error? */
	    {
		result = STATUS_ERR_DEVINUSE;;
		break;
	    }

	    mirror->header.flags |= RDFLAG_REBUILD;
	    mirror->rebuild_sec = ((PVRAID_REBUILD_PARM)param)->secno;

	    UpdateCfg(&mirror->header, TRUE);	/* save configuration first */
	    if( BuildCreate(&mirror->header, p->secno, 1) != 0 )
		result = STATUS_ERR_PAPEROUT;	/* out of memory */
	}
	while(0);
	self->ioctl(self, VRDIOCTL_INFO, 0, data);
	break;


      case VRDIOCTL_CHILD_STATE:
	DEBMSG(DBG_PATH, "\r\nVRDIOCTL_CHILD_STATE");
	result = 0;
	do
	{
	    PVRAID_CHILD_PARAM	p = param;

	    if( p->secno >= mirror->header.size )
	    {
		result = STATUS_ERR_NOTFOUND;
		break;
	    }

	    result = STATUS_ERR_INVPARAM;
	    for( i = 0; i < mirror->children; ++i )
	    {
		if( _fmemcmp(p->id, mirror->child[i].vrd->id, sizeof(DEVID)) == 0 )
		{
		    result = 0;			/* OK, even if ignored */

		    /* Should we allow someone to declare one of our
		     * children as valid even if we have another idea
		     * of the situation?  I don't think so... 		xxx */

		    if( (p->flags & 0x01) )
		    {
			result = STATUS_ERR_UNCERTAIN;
			break;
		    }
		    if( mirror->child[i].valid == (ULONG)-1
			||  p->secno < mirror->child[i].valid )
		    {
			mirror->child[i].valid = p->secno;
			if( mirror->header.flags == 0 )	/* build, fail, error? */
			{
			    mirror->header.flags |= RDFLAG_REBUILD;
			    mirror->rebuild_sec = p->secno;

			    UpdateCfg(&mirror->header, TRUE);
			    if( BuildCreate(&mirror->header, p->secno, 0) != 0 )
				result = STATUS_ERR_PAPEROUT;
			}
		    }
		    break;
		}
	    }
	}
	while(0);
	break;


      case VRDIOCTL_VERIFY:
	DEBMSG(DBG_PATH, "\r\nVRDIOCTL_VERIFY");
	result = 0;				/* assume OK */
	{
	    PVRAID_VERIFY_PARM const 	pvp = (PVOID)param;
	    USHORT const	vd_cb = (sizeof(VRAID_VERIFY_DATA) - 1
					 + pvp->seccnt * SECTOR_SIZE);
	    PUCHAR const	vdata = (PUCHAR)data + sizeof(VRAID_ARRAYINFO_DATA);
	    PVRAID_VERIFY_DATA	pvd;
	    ULONG	phys;


	    DEBMSG2(DBG_LEVEL3,"\r\nData: %p, 1st Dev Contents: %p",data,vdata);
	    DEBMSG1(DBG_LEVEL3,"\r\nPer device size: %w",vd_cb);
	    if( pvp->secno + pvp->seccnt > mirror->header.size )
	    {
		result = STATUS_ERR_NOTFOUND;
		break;
	    }
	    if( DevHelp_VirtToPhys(data, &phys) )
	    {
		DEBMSG1(DBG_LEVEL1,"\r\nVirtToPhys failed on %p",data);
		result = STATUS_ERR_INVPARAM;
		break;
	    }

	    phys += sizeof(VRAID_ARRAYINFO_DATA);
	    for( i = 0; i < mirror->children; ++i )
	    {
		pvd = (PVOID)&vdata[i * vd_cb];

		DEBMSG2(DBG_LEVEL3,"\r\n%w. Dev Contents: %p",i,pvd);
		_fmemcpy(pvd->id, mirror->child[i].vrd->id, sizeof(DEVID));
		pvd->valid = mirror->child[i].valid;
		pvd->bcnt = pvp->seccnt * SECTOR_SIZE;
		pvd->flags = 0;
		pvd->reserved = 0;

		result = ProcessUserIO(mirror->child[i].vrd, FALSE,
				       pvp->secno, pvp->seccnt,
				       phys + FIELDOFFSET(VRAID_VERIFY_DATA, data));
		/*if( result != 0 ) break;*/

		phys += vd_cb;
	    }


	    /* Now compare all children. */

	    for( i = 0; i < mirror->children; ++i )
		for( j = i+1; j < mirror->children; ++j )
		{
		    PVRAID_VERIFY_DATA const c1data = (PVOID)&vdata[i * vd_cb];
		    PVRAID_VERIFY_DATA const c2data = (PVOID)&vdata[j * vd_cb];

		    DEBMSG2(DBG_LEVEL2,"\r\n%w. child data: %p",i,c1data);
		    DEBMSG2(DBG_LEVEL2,"\r\n%w. child data: %p",j,c2data);

		    if( _fmemcmp(c1data->data, c2data->data,
				 pvp->seccnt * SECTOR_SIZE) == 0 )
		    {
			DEBMSG2(DBG_LEVEL2,"\r\nCompared %w and %w: OK",i,j);
			++c1data->reserved;
			++c2data->reserved;
		    }
		    else
		    {
			DEBMSG2(DBG_LEVEL1,"\r\nComparing %p and %p: failed",c1data->data,c2data->data);
			DBSTOP();
		    }
		}
	    for( i = 0; i < mirror->children; ++i )
	    {
		PVRAID_VERIFY_DATA const cdata = (PVOID)&vdata[i*vd_cb];
		DEBMSG2(DBG_LEVEL2,"\r\n%w. child data: %p",i,cdata);

		if( cdata->reserved == mirror->children-1 )
		{
		    cdata->flags |= VVDF_CONFIRMED;
		}
		else
		{
		    DEBMSG2(DBG_LEVEL1,"\r\n%w. child failed (%w goods)",i,cdata->reserved);
		}
	    }
	}
	self->ioctl(self, VRDIOCTL_INFO, 0, data);
	break;


      default:
	break;
    }

    return result;
} /* end[MirrorCtrl] */






/* **********************************************************************
 * **** INIT-time processing ********************************************
 * ******************************************************************* */






/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * MirrorCreate(pVrh,pPhys,sec,pSize)
 *
 * PARAMETER
 *	pVrh		create here
 *	tab		table of device to build from
 *	sec		contents of configuration sector
 *	pSize		return: size used at '*pVrh'
 *
 * RETURNS
 *	0		OK
 *	/0		couldn't create
 *
 * DESCRIPTION
 *	Analyses configuration sector and builds mirror VRAID structure
 *	in memory pointed to by 'pVrh'.
 *
 * REMARKS
 */
PUBLIC int NEAR _Cdecl
MirrorCreate(PVRDEVHDR pVrh, PVRDEVHDR FAR * tab, PSEC_VRDEV2 sec, PUSHORT pSize)
{
    PVRDMIRROR const mirror = (PVRDMIRROR)pVrh;
    USHORT	i, j, found;
    int		invalids = 0;
    int		result;


    DEBMSG2(DBG_INIT3,dszMCreateAdminData,(PVOID)sec,SECTOR_SIZE);
    _fmemset(mirror, 0, sizeof(*mirror));


    /* Initialize header elements */

    mirror->header.notify = MirrorNotify;
    mirror->header.worker = MirrorWorker;
    mirror->header.update = MirrorUpdate;
    mirror->header.ioctl = MirrorCtrl;
    mirror->header.size = 0;			/* initialize, will be modified */
    mirror->header.flags = 0;
    _fmemcpy(mirror->header.id, sec->u.s.id, sizeof(mirror->header.id));


    /* Build linking by looking up all parts in 'tab'. */

    mirror->children = sec->u.s.children;
    _fmemset(mirror->child, 0, mirror->children * sizeof(mirror->child[0]));
    found = 0;
    for( i = 0; i < mirror->children; ++i )
    {
	mirror->header.size = sec->u.s.child[i].size;
	for( j = 0; j < MAX_PDEV; ++j )
	{
	    if( tab[j] == NULL )
		continue;			/* ignore empty entries */

	    if( _fmemcmp(tab[j]->id, sec->u.s.child[i].id, sizeof(tab[j]->id)) == 0 )
	    {
		result = ValidateVSec2(tab[j], sec);
		if( result < 0 )
		{
		    sprintk(szMsgBuffer, szCreationDelayed);
		    DEBMSG1(DBG_INIT1,dszStr,(PSZ)szMsgBuffer);
		    if( (fDriverFlags & DF_VERBOSE) )
			SaveMessage();

		    return 1;			/* don't create mirror, be quiet */
		}
		else if( result != 0 )
		    continue;			/* don't use this child */

		mirror->child[i].vrd = tab[j];
		if( (sec->u.s.child[i].flags & 0x01) == 0 )
		{
		    ++invalids;
		    mirror->child[i].valid = sec->u.s.child[i].valid;
		}
		else
		{
		    mirror->child[i].valid = (ULONG)-1;
		}
		++found;
		break;				/* stop scan, next child */
	    }
	}
    }


    /* OK, we will indeed create an array.  Remove children from 'tab'
     * so that they don't get used for another array. */

    for( i = 0; i < mirror->children; ++i )
    {
	if( mirror->child[i].vrd != 0 )
	{
	    for( j = 0; j < MAX_PDEV; ++j )
		if( tab[j] == mirror->child[i].vrd )
		{
		    tab[j] = NULL;
		    break;
		}
	}
    }


    /* Update local flags and restart [re]build process if usefull. */

    if( found != mirror->children )
    {
	/* There is at least one child missing: set FAIL state.
	 * (should we restart Build/Rebuilds?  Currently they
	 *  are *not* started) */

	sprintk(szMsgBuffer, szMissingChild, mirror->children-found);
	DEBMSG1(DBG_INIT1,dszStr,(PSZ)szMsgBuffer);
	if( (fDriverFlags & DF_VERBOSE) )
	    SaveMessage();

	mirror->header.flags |= RDFLAG_FAIL;
	++cBeeps;
	mirror->need_updatecfg = TRUE;		/* better do an update */
    }
    else if( (sec->u.s.flags & RDFLAG_BUILDING)
	     ||  (sec->u.s.flags & RDFLAG_REBUILD)
	     ||  invalids != 0 )
    {
	ULONG	bdone = (ULONG)-1;

	/* There were either an invalid child detected or the
	 * 'flag' field of the array says 'BUILDING' or 'REBUILD'.
	 * Lets figure out what is meant to happen. */

	if( (sec->u.s.flags & RDFLAG_REBUILD) )
	{
	    bdone = sec->rebuild;
	    mirror->header.flags |= RDFLAG_REBUILD;
	    mirror->rebuild_sec = sec->rebuild;
	}
	for( i = 0; i < mirror->children; ++i )
	    bdone = MIN(bdone, mirror->child[i].valid);
 	if( bdone == (ULONG)-1 )
	{
 	    /* Well, there may be all configuration data from
 	     * an aborted BUILD or REBUILD (stopped via VSetup).
 	     * We *need* to update: no [RE]BUILD */

 	    mirror->need_updatecfg = TRUE;
 	    DEBMSG(DBG_INIT1,dszMCreateNoBuild);
 	}
  	else
	{
	    if( (sec->u.s.flags & RDFLAG_REBUILD) )
	    {
		/* Restart REBUILD process. */

		DEBMSG1(DBG_INIT1,dszMCreateRebuild,(PVOID)&bdone);

		mirror->header.flags |= RDFLAG_REBUILD;
		mirror->rebuild_sec = bdone;
		BuildCreate(pVrh, bdone, 1);
	    }
	    else
	    {
		DEBMSG(DBG_INIT1,dszMCreateBuild);
		mirror->header.flags |= RDFLAG_BUILDING;
		BuildCreate(pVrh, bdone, 0);
	    }
	}
    }


    mirror->readchild = NextValidChild(mirror, mirror->readchild);


    /* Size of administrative data depends on count of children. */

    *pSize = sizeof(*mirror) + mirror->children * sizeof(mirror->child[0]);

    DEBMSG4(DBG_INIT1,dszMirrorCreate,(PVOID)pVrh,(PVOID)sec->u.s.id,6,(PVOID)&mirror->header.size);
    return 0;
} /* end[MirrorCreate] */


/* **********************************************************************
 * **** End Of File *****************************************************
 * ******************************************************************* */
