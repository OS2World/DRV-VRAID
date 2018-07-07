/*
 * $Source: R:/source/driver/raid/RCS\\span.c,v $
 * $Revision: 2.4 $
 * $Date: 2001/07/10 01:00:34 $
 * $Locker:  $
 *
 *
 *	How to do I/O to spanning set.
 *
 *
 * $Log: span.c,v $
 * Revision 2.4  2001/07/10 01:00:34  vitus
 * - implemented NextValidChild() to select next device to read.
 *
 * Revision 2.3  2001/07/03 07:53:46  vitus
 * - VRDSPAN now contains sizes of all children.  Otherwise it is not possible
 *   to use the array when a child is missing!
 *
 * Revision 2.2  2001/02/11 01:16:22  vitus
 * - changed SpanCtrl() to return DRVSTATUS
 *
 * Revision 2.1  2000/06/13 01:44:11  vitus
 * - implemented ioctl method (VRDIOCTL_QUERY_CHILDREN, VRDIOCTL_INFO)
 *
 * Revision 1.12  1999/09/26 00:21:42  vitus
 * - defined all DBGMSG() strings as located in CONST segment
 *
 * Revision 1.11  1999/08/09 23:19:49  vitus
 * - SpanCreate: initialize new VRDSPAN structure with 0
 *
 * Revision 1.10  1999/06/21 02:28:26  vitus
 * - corrected english spelling
 * - handle missing children (NULL pointer)
 *
 * Revision 1.9  1999/05/26 00:46:24  vitus
 * - SpanCreate: return value now 'int', but as it is always 0...
 *
 * Revision 1.8  1999/05/12 01:48:49  vitus
 * - dummy SpanUpdate() implemented
 *
 * Revision 1.7  1999/04/28 01:47:40  vitus
 * - merged SpanAdmin() code with Worker/Notify, IOCMD_A* commands
 * - SpanNotify() uses CompleteVrio()
 *
 * Revision 1.6  1999/02/07 17:52:01  vitus
 * - implemented StripeAdmin() method
 * - changed for iorb -> vrio transition
 *
 * Revision 1.5  1998/09/20 14:24:06  vitus
 * - modified for AllocateVrio(working_dev,mother)
 *
 * Revision 1.4  1998/06/26 23:03:53  vitus
 * - switched to DDK macros (cli, sti, pushf)
 * - worker,notify get const pointers and become _Cdecl
 *
 * Revision 1.3  1998/05/29 01:56:42  vitus
 * - fine tuning of DEBMSG macro calls
 *
 * Revision 1.2  1998/04/08 00:56:38  vitus
 * - corrected VRIO handling
 * - test successfull
 *
 * Revision 1.1  1998/03/10 02:09:48  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2001
 */
#if defined(__WATCOMC__)
# pragma off (unreferenced)
#endif
static char const id[]="$Id: span.c,v 2.4 2001/07/10 01:00:34 vitus Exp $";
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
 * Our object: the spanning drive.
 */
typedef struct _VRDSPAN {

    VRDEVHDR	header;				/* ALWAYS FIRST! */

    USHORT	readchild;			/* for configuration sectors */

    USHORT	children;
    struct {
	ULONG		size;			/* size of child */
	PVRDEVHDR	dev;			/* pointer to device */
    } child[];

} VRDSPAN, FAR * PVRDSPAN;




PRIVATE USHORT NEAR _Cdecl	SpanWorker(PVRDEVHDR const self,PVRIO const myio);
PRIVATE USHORT NEAR _Cdecl	SpanUpdate(PVRDEVHDR const self,PVOID const sec);
PRIVATE DRVSTATUS NEAR _Cdecl	SpanCtrl(PVRDEVHDR const self, USHORT const cmd,
					 PVOID const param, PVOID const data);




#if defined(DEBUG)
CONSTMSG dszSpanNotify[]=	"\r\n[SpanNotify] io %p, myio %p, error %w";
CONSTMSG dszSNotifyAll[]=	"\r\n[SpanNotify] completed all of myio %p";
CONSTMSG dszSpanWorker[]=	"\r\n[SpanWorker] %p";
CONSTMSG dszSWorkerRange[]=	"\r\n[SpanWorker] first blk %lx, last blk %lx";
CONSTMSG dszSWorkerChild[]=	"\r\n[SpanWorker] child %w: start %lx length %lx";
CONSTMSG dszSWorkerNochild[]=	"\r\n[SpanWorker] uneffected child %w";
CONSTMSG dszSWorkerComplete[]=	"\r\n[SpanWorker] completed (last %lx, %w. start %lx)";
CONSTMSG dszSWorkerVrio[]=	"\r\n[SpanWorker] creating VRIO for device %w";
CONSTMSG dszSWorkerAlloc[]=	"\r\n[SpanWorker] couldn't allocate VRIO";
CONSTMSG dszSWorkerRelease[]=	"\r\n[SpanWorker] releasing VRIOs";
CONSTMSG dszSWorkerMissing[]=	"\r\n[SpanWorker] child missing, cmd %w";
CONSTMSG dszSWorkerBusy[]=	"\r\n[SpanWorker] child busy, queuing VRIO";
CONSTMSG dszSCreateAdminData[]=	"\r\n[SpanCreate] admin data\r\n%z";
CONSTMSG dszSCreateFound[]=	"\r\n[SpanCreate] found child %w in %p";
CONSTMSG dszSCreateOnly[]=	"\r\n[SpanCreate] only found %w out of %w";
CONSTMSG dszSpanCreate[]=	"\r\n[SpanCreate] at %p"
				"\r\n             ID %z"
				"\r\n             Size 0x%lx";
#endif






/* **********************************************************************
 * **** Private Routines ************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * NextValidChild(span, previous)
 *
 * PARAMETER
 *	span		a VRD which forms a spanning device
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
static USHORT
NextValidChild(PVRDSPAN const span, USHORT const previous)
{
    USHORT	next = previous;
    USHORT	tries = span->children;
    BOOL	found = FALSE;

    do
    {
	next = (next + 1 == span->children ? 0 : next + 1);
	if( span->child[next].dev != NULL )
	{
	    found = TRUE;
	    break;
	}
    }
    while( tries-- );
    return (found ? next : (USHORT)-1);
}




/*# ----------------------------------------------------------------------
 * SpanNotify(child,io)
 *
 * PARAMETER
 *	io		VRIO which completed
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Used by a child to report that it's I/O has completed.
 *	Updates own I/O (which was passed to this spanning set) and
 *	ends it if all I/Os to children ended.
 *
 * REMARKS
 */
PRIVATE VOID NEAR _Cdecl
SpanNotify(PVRIO const io)
{
    PVRIO const		myio = io->mother;
    PVRDSPAN const	span = (PVRDSPAN)myio->vrd;

    DEBMSG3(DBG_PATH,dszSpanNotify,io,myio,io->oserr);

    ASSERT( io->bcnt != 0 );
    ASSERT( io->link == NULL );
    ASSERT( myio->link == NULL );
    ASSERT( myio->bcnt != 0 );
    ASSERT( myio->vrd->size != 0 );
    ASSERT( myio->vrd->notify == SpanNotify );
    ASSERT( myio->vrd->worker == SpanWorker );

    pushf();
    cli();

    /* Error handling (different for each RAID level). */

    if( myio->oserr == 0xFFFF )			/* still not set? */
	myio->oserr = io->oserr;		/* propagate error */
    else if( myio->oserr == 0 )			/* this I/O still OK? */
	myio->oserr = io->oserr;		/* keep any error */


    switch( myio->iocmd )
    {
      case IOCMD_READ:
      case IOCMD_WRITE:
      case IOCMD_AWRITE:
	/* Reduce child I/O count.  If all completed, call parent's notification. */

	--myio->children;
	break;

      case IOCMD_AREAD:
	/* **** AREAD ****
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
		USHORT	rc;
		USHORT	i, found = 0;

		/* Select next child. */

		for( i = 0; i < span->children; ++i )
		    if( child == span->child[i].dev )
		    {
			USHORT j = i;

			j = (j + 1 == span->children ? 0 : j+1);
			if( span->child[j].dev == NULL )
			{
			    /* missing child */

			    --myio->children;
			    continue;		/* search again */
			}
			child = span->child[j].dev;
			found = 1;
			break;
		    }
		ASSERT( found == 1 );

		/* Patch new child into VRIO and start it. */

		io->vrd = child;
		rc = io->vrd->worker(io->vrd, io);
		if( rc )
		    QueueVrio(io);
	    }
	}
	break;
    }


    if( myio->children == 0 )			/* my I/O ready? */
    {
	DEBMSG1(DBG_LEVEL2,dszSNotifyAll,myio);

	if( myio->oserr != 0 )
	    span->header.flags |= RDFLAG_ERROR;

	CompleteVrio(myio);
    }

    popf();
    return;
}




/*# ----------------------------------------------------------------------
 * SpanWorker(stripe,myio)
 *
 * PARAMETER
 *	span		set building a spanning set
 *	myio		I/O to spanning set (to myself)
 *
 * RETURNS
 *	0		OK
 *	/0		I/O not processed, please retry
 *
 * DESCRIPTION
 *	Pass VRIO to underlying I/O routine of affected children.
 *
 * REMARKS
 */
PRIVATE USHORT NEAR _Cdecl
SpanWorker(PVRDEVHDR const self,PVRIO const myio)
{
    PVRDSPAN const	span = (PVRDSPAN)self;
    PVRIO	io;
    PVRIO	newio;
    PVRIO	firstio = NULL;

    ULONG const	firstblk = myio->blk;		/* first blk */
    ULONG const	lastblk = firstblk + myio->bcnt - 1;
    ULONG	devstart = 0;
    USHORT	i;


    DEBMSG1(DBG_PATH,dszSpanWorker,(PVOID)myio);
    DEBMSG2(DBG_LEVEL3,dszSWorkerRange,(PVOID)&firstblk,(PVOID)&lastblk);

    ASSERT( myio->vrd == self );
    ASSERT( myio->link == NULL );
    ASSERT( myio->bcnt != 0 );
    ASSERT( span->header.size != 0 );
    ASSERT( span->header.notify == SpanNotify );
    ASSERT( span->header.worker == SpanWorker );
    ASSERT( span->header.update == SpanUpdate );
    ASSERT( span->header.ioctl == SpanCtrl );

    ++self->iocnt;

    switch( myio->iocmd )
    {
      case IOCMD_READ:
      case IOCMD_WRITE:
	/* Create VRIO to all affected children. */

	myio->children = 0;
	for( i = 0; i < span->children; devstart += span->child[i].size, ++i )
	{
	    DEBMSG3(DBG_LEVEL3,dszSWorkerChild,i,(PVOID)&devstart,(PVOID)&span->child[i].size);
	    if( firstblk >= devstart + span->child[i].size )
	    {
		DEBMSG1(DBG_LEVEL4,dszSWorkerNochild,i);
		continue;			/* not affected */
	    }
	    if( lastblk < devstart )
	    {
		DEBMSG3(DBG_LEVEL3,dszSWorkerComplete,(PVOID)&lastblk,i,(PVOID)&devstart);
		break;				/* did complete distribution */
	    }

	    DEBMSG1(DBG_LEVEL4,dszSWorkerVrio,i);

	    /* Build linked list of VRIOs.  */

	    newio = AllocateVrio(span->child[i].dev, myio);
	    if( newio == NULL )			/* memory exhausted? */
	    {
		DEBMSG(DBG_LEVEL1,dszSWorkerAlloc);
		break;
	    }

	    if( firstio == NULL )
		firstio = newio;		/* save this as first */
	    else
		io->link = newio;
	    io = newio;
	    io->link = NULL;
	    ++myio->children;

	    /* Pass range to this child. */

	    io->iorb = myio->iorb;
	    io->iocmd = myio->iocmd;
	    io->cSGList = myio->cSGList;
	    io->pSGList = myio->pSGList;
	    io->ppSGList = myio->ppSGList;

	    if( firstblk < devstart )
	    {
		io->blk = 0;
		io->offset = (USHORT)(devstart - firstblk);
	    }
	    else
	    {
		io->blk = firstblk - devstart;
		io->offset = 0;
	    }
	    if( lastblk > devstart + span->child[i].size )
		io->bcnt = (USHORT)(span->child[i].size - io->blk);
	    else if( firstblk < devstart )
		io->bcnt = (USHORT)((lastblk - devstart) + 1);
	    else
		io->bcnt = (USHORT)((lastblk - firstblk) + 1);

	    io->oserr = 0xFFFF;			/* invalidate error code */
	}
	break;


      case IOCMD_AWRITE:
	/* Write: create VRIO to all children. */

	myio->children = 0;

	for( i = 0; i < span->children; ++i )
	{
	    if( span->child[i].dev == NULL )
		continue;			/* skip non-existing child */

	    /* Allocate VRIOs and update links. */

	    newio = AllocateVrio(span->child[i].dev, myio);
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
	    io->blk = myio->blk + 1;		/* as usual */
	    io->bcnt = myio->bcnt;
	    io->oserr = 0xFFFF;			/* invalid error code */
	}
	break;

      case IOCMD_AREAD:
	/* Read: create for 'readchild' child and do another in Notify()
	 * if this first failed!
	 * Don't forget to increment 'readchild'. */

	myio->children = span->children;

	/* Allocate VRIOs and update links. */

	do
	{
	    ASSERT( span->readchild != (USHORT)-1 );
	    newio = AllocateVrio(span->child[span->readchild].dev, myio);
	    if( newio == NULL )			/* memory exhausted? */
		break;

	    if( firstio == NULL )
		firstio = newio;		/* save this as first */
	    else
		io->link = newio;
	    io = newio;
	    io->link = NULL;

	    /* Child will receive the same parameters
	     * as parent (configuration secotrs!). */

	    io->iorb = myio->iorb;
	    io->iocmd = myio->iocmd;
	    io->cSGList = myio->cSGList;
	    io->pSGList = myio->pSGList;
	    io->ppSGList = myio->ppSGList;

	    io->offset = myio->offset;
	    io->blk = myio->blk + 1;		/* as usual */
	    io->bcnt = myio->bcnt;
	    io->oserr = 0xFFFF;			/* invalid error code */


	    pushf();
	    cli();
	    span->readchild = NextValidChild(span, span->readchild);
	    popf();
	}
	while(0);
	break;
    } /* end[switch] */



    if( newio == NULL )
    {
	/* Not enough memory.  Free all allocated VRIOs
	 * and return error code to parent. */

	DEBMSG(DBG_ALL,dszSWorkerRelease);
	for( io = firstio; io != NULL; io = newio )
	{
	    newio = io->link;
	    FreeVrio(io);
	    DO( io = NULL );
	}
	++self->busycnt;
	return 1;
    }


    /* Now pass all allocated VRIOs to children. */

    for( io = firstio; io != NULL; io = newio )
    {
	USHORT	rc;

	newio = io->link;
	io->link = NULL;
	if( io->vrd == NULL )
	{
	    /* There is a child missing and somebody wants to read/write
	     * from/to it's share of data.  Return error! */

	    DEBMSG1(DBG_ALL,dszSWorkerMissing,io->iocmd);
	    io->oserr = IOERR_ADAPTER_DEVICE_TIMEOUT;
	    CompleteVrio(io);
	}
	else
	{
	    rc = io->vrd->worker(io->vrd, io);
	    if( rc )
	    {
		DEBMSG(DBG_ALL,dszSWorkerBusy);
		QueueVrio(io);
	    }
	}
    }

    return 0;
} /* end[SpanWorker] */




/*# ----------------------------------------------------------------------
 * SpanUpdate(self,sec)
 *
 * PARAMETER
 *	self		a VRD which forms an array
 *	sec		sector contains configuration
 *
 * RETURNS
 *	0		don't write back, nothing changed
 *
 * DESCRIPTION
 *	Do nothing.
 *
 * REMARKS
 */
PRIVATE USHORT NEAR _Cdecl
SpanUpdate(PVRDEVHDR const self,PVOID const sec)
{
    PVRDSPAN const	span = (PVRDSPAN)self;

    ASSERT( span->header.size != 0 );
    ASSERT( span->header.notify == SpanNotify );
    ASSERT( span->header.worker == SpanWorker );
    ASSERT( span->header.update == SpanUpdate );
    ASSERT( span->header.ioctl == SpanCtrl );

    return 0;
}




/*# ----------------------------------------------------------------------
 * SpanCtrl(self,cmd,param,data)
 *
 * PARAMETER
 *	mirror		a VRD which forms a spanning device
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
SpanCtrl(PVRDEVHDR const self, USHORT const cmd,
	 PVOID const param, PVOID const data)
{
    PVRDSPAN const	span = (PVRDSPAN)self;
    DRVSTATUS		result = STATUS_ERR_UNKCMD;


    ASSERT( span->header.size != 0 );
    ASSERT( span->header.notify == SpanNotify );
    ASSERT( span->header.worker == SpanWorker );
    ASSERT( span->header.update == SpanUpdate );
    ASSERT( span->header.ioctl == SpanCtrl );

    switch( cmd )
    {
      case VRDIOCTL_QUERY_CHILDREN:
	*(PUSHORT)data = span->children;
	result = 0;
	break;


      case VRDIOCTL_INFO:
	DEBMSG(DBG_PATH, "VRDIOCTL_INFO");
	{
	    PVRAID_ARRAYINFO_DATA	d = data;

	    _fmemcpy(d->id, span->header.id, sizeof(d->id));
	    d->reqcnt = span->header.iocnt;
	    d->size = span->header.size;

	    d->children = span->children;
	    d->flags = span->header.flags;
	    d->reserved = 0;
	}
	result = 0;
	break;


      default:
	break;
    }

    return result;
} /* end[SpanCtrl] */






/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * SpanCreate(pVrh,pPhys,sec,pSize)
 *
 * PARAMETER
 *	pVrh		room to create device structure
 *	tab		table of device to build from
 *	sec		contents of configuration sector
 *	pSize		return: size used at '*pVrh'
 *
 * RETURNS
 *	0		OK
 *
 * DESCRIPTION
 * REMARKS
 */
PUBLIC int NEAR _Cdecl
SpanCreate(PVRDEVHDR pVrh,PVRDEVHDR FAR * tab,PSEC_VRDEV sec,PUSHORT pSize)
{
    PVRDSPAN const	span = (PVRDSPAN)pVrh;
    USHORT	i, found;


    DEBMSG2(DBG_INIT3,dszSCreateAdminData,(PVOID)sec,SECTOR_SIZE);
    _fmemset(span, 0, sizeof(*span));


    /* Initialize header elements */

    span->header.worker = SpanWorker;
    span->header.notify = SpanNotify;
    span->header.update = SpanUpdate;
    span->header.ioctl = SpanCtrl;
    span->header.size = 0;			/* initialize, will be added to */
    span->header.flags = 0;
    _fmemcpy(span->header.id, sec->u.s.id, sizeof(span->header.id));


    /* Build linking by looking up all parts in 'tab'. */

    span->children = sec->u.s.children;
    _fmemset(span->child, 0, span->children * sizeof(span->child[0]));
    found = 0;
    for( i = 0; i < span->children; ++i )
    {
	PDEVID const	id1 = &sec->u.s.child[i].id;
	USHORT		j;

	span->child[i].size = sec->u.s.child[i].size;
	span->header.size += span->child[i].size;
	for( j = 0; j < MAX_PDEV; ++j )
	{
	    if( tab[j] != NULL )		/* ignore empty entries */
	    {
		PDEVID const	id2 = &tab[j]->id;

		if( _fmemcmp(id1, id2, sizeof(DEVID)) == 0 )
		{
		    DEBMSG2(DBG_INIT1,dszSCreateFound,i,tab[j]);
		    span->child[i].dev = tab[j];
		    tab[j] = NULL;
		    ++found;
		    break;			/* stop scan, next child */
		}
	    }
	}
    }
    if( found != span->children )
    {
	DEBMSG2(DBG_ALL,dszSCreateOnly,found,span->children);
	span->header.flags = RDFLAG_ERROR;
    }


    span->readchild = NextValidChild(span, span->readchild);


    /* Size of administrative data depends on count of children. */

    *pSize = sizeof(*span) + span->children * sizeof(span->child[0]);

    DEBMSG4(DBG_INIT1,dszSpanCreate,(PVOID)pVrh,(PVOID)sec->u.s.id,6,(PVOID)&span->header.size);
    return 0;
} /* end[SpanCreate] */
