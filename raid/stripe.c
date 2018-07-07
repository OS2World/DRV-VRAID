/*
 * $Source: R:/source/driver/raid/RCS\\stripe.c,v $
 * $Revision: 2.2 $
 * $Date: 2001/02/11 01:14:48 $
 * $Locker:  $
 *
 *
 *	How to do RAID 0 I/O.
 *
 *
 * $Log: stripe.c,v $
 * Revision 2.2  2001/02/11 01:14:48  vitus
 * - changed StripeCtrl() to return DRVSTATUS
 *
 * Revision 2.1  2000/06/13 01:44:56  vitus
 * - implemented ioctl method (VRDIOCTL_QUERY_CHILDREN, VRDIOCTL_INFO)
 *
 * Revision 1.12  1999/10/01 00:50:42  vitus
 * - updates new iocnt and busycnt fields in VRDEVHDR
 *
 * Revision 1.11  1999/09/20 00:03:12  vitus
 * - defined all DBGMSG() strings as located in CONST segment
 *
 * Revision 1.10  1999/06/21 02:29:17  vitus
 * - corrected english spelling
 * - handle missing children (NULL pointer)
 *
 * Revision 1.9  1999/05/26 00:44:09  vitus
 * - StripeCreate: return value now 'int', but as it is always 0...
 *
 * Revision 1.8  1999/05/12 01:49:09  vitus
 * - dummy StripeUpdate() implemented
 *
 * Revision 1.7  1999/04/28 01:46:15  vitus
 * - merged StripeAdmin() code with Worker/Notify, IOCMD_A* commands
 * - StripeNotify() uses CompleteVrio()
 *
 * Revision 1.6  1999/02/07 17:35:17  vitus
 * - implemented StripeAdmin() method
 *
 * Revision 1.5  1998/09/20 14:23:28  vitus
 * - modified for AllocateVrio(working_dev,mother)
 *
 * Revision 1.4  1998/06/26 23:08:35  vitus
 * - switched to DDK macros (cli, sti, pushf)
 * - worker,notify get const pointers and become _Cdecl
 *
 * Revision 1.3  1998/04/19 22:38:27  vitus
 * - corrected stripe distribution code
 * - test successfull
 *
 * Revision 1.2  1998/03/10 02:08:10  vitus
 * - changed to PUBLIC *Create() function and PRIVATE workers
 *
 * Revision 1.1  1998/02/12 01:28:05  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2001
 */
#if defined(__WATCOMC__)
# pragma off (unreferenced)
#endif
static char const id[]="$Id: stripe.c,v 2.2 2001/02/11 01:14:48 vitus Exp $";
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




/* Our object: the striping drive. */
typedef struct _VRDSTRIPE {

    VRDEVHDR	header;				/* ALWAYS FIRST! */

    USHORT	stripesize;			/* [sectors] */

    USHORT	readchild;			/* next when doing admin I/O */

    USHORT	children;
    PVRDEVHDR	child[];

} VRDSTRIPE, FAR * PVRDSTRIPE;




PRIVATE USHORT NEAR _Cdecl	StripeWorker(PVRDEVHDR const self,PVRIO const myio);
PRIVATE USHORT NEAR _Cdecl	StripeUpdate(PVRDEVHDR const self,PVOID const sec);
PRIVATE DRVSTATUS NEAR _Cdecl	StripeCtrl(PVRDEVHDR const self, USHORT const cmd,
					   PVOID const param, PVOID const data);




#if defined(DEBUG)
CONSTMSG dszStripeNotify[]=	"\r\n[StripeNotify] io %p, myio %p, error %w";
CONSTMSG dszSNotifyComplete[]=	"\r\n[StripeNotify] completed all of myio %p";
CONSTMSG dszStripeWorker[]=	"\r\n[StripeWorker] %p";
CONSTMSG dszSWorkerBlk[]=	"\r\n[StripeWorker] first blk %lx, last blk %lx";
CONSTMSG dszSWorkerStart[]=	"\r\n[StripeWorker] start %lx";
CONSTMSG dszSWorkerStripe[]=   "\r\n[StripeWorker] strno %lx, stroffs %w, devidx %w";
CONSTMSG dszSAdminNoalloc[]=	"\r\n[StripeAdmin] AllocateVrio failed";
CONSTMSG dszSWorkerRelease[]=	"\r\n[StripeWorker] releasing VRIOs";
CONSTMSG dszSWorkerChildMiss[]=	"\r\n[StripeWorker] child missing, cmd %w";
CONSTMSG dszSWorkerBusy[]=	"\r\n[StripeWorker] child busy, queuing VRIO";
CONSTMSG dszStripeCreate[]=	"\r\n[StripeCreate] at %p, %w children";
CONSTMSG dszSCreateID[]=	"\r\n[StripeCreate] ID %z";
CONSTMSG dszSCreateLooking[]=	"\r\n[StripeCreate] looking for %z";
CONSTMSG dszSCreateFound[]=	"\r\n[StripeCreate] found child %w in %p";
CONSTMSG dszSCreateOnly[]=	"\r\n[StripeCreate] only found %w out of %w";
#endif






/*# ----------------------------------------------------------------------
 * StripeNotify(io)
 *
 * PARAMETER
 *	io		VRIO which completed
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Used by a child to report that an I/O has completed.
 *	Updates father I/O (which was passed to this stripe set) and
 *	ends it if all I/Os to children ended.
 *
 * REMARKS
 */
PRIVATE VOID NEAR _Cdecl
StripeNotify(PVRIO const io)
{
    PVRIO const		myio = io->mother;
    PVRDSTRIPE const	stripe = (PVRDSTRIPE)myio->vrd;


    DEBMSG3(DBG_PATH,dszStripeNotify,io,myio,io->oserr);

    ASSERT( io->bcnt != 0 );
    ASSERT( io->link == NULL );
    ASSERT( myio->link == NULL );
    ASSERT( myio->bcnt != 0 );
    ASSERT( myio->vrd->size != 0 );
    ASSERT( myio->vrd->notify == StripeNotify );
    ASSERT( myio->vrd->worker == StripeWorker );
    ASSERT( myio->vrd->update == StripeUpdate );
    ASSERT( myio->vrd->ioctl == StripeCtrl );

    pushf();
    cli();

    /* Error handling (different for each RAID level). */

    if( myio->oserr == 0xFFFF )			/* still not set? */
	myio->oserr = io->oserr;		/* propagate first error code */
    else if( myio->oserr == 0 )			/* my I/O still OK? */
	myio->oserr = io->oserr;		/* keep any error */


    switch( myio->iocmd )
    {
      case IOCMD_READ:
      case IOCMD_WRITE:
      case IOCMD_AWRITE:
	/* Remove VRIO.  If last, call parent notification. */

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

		for( i = 0; i < stripe->children; ++i )
		    if( child == stripe->child[i] )
		    {
			i = (i + 1 == stripe->children ? 0 : i+1);
			child = stripe->child[i];
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
    } /* end[switch] */


    if( myio->children == 0 )			/* my I/O ready? */
    {
	DEBMSG1(DBG_LEVEL3,dszSNotifyComplete,myio);
	if( myio->oserr != 0 )
	    stripe->header.flags |= RDFLAG_ERROR;
	CompleteVrio(myio);
    }

    popf();
    return;
} /* end[StripeNotify] */




/*# ----------------------------------------------------------------------
 * StripeWorker(stripe,father_io)
 *
 * PARAMETER
 *	stripe		set building a stripe
 *	father_io	I/O to stripe set
 *
 * RETURNS
 *	0		OK
 *	/0		I/O not processed, please retry
 *
 * DESCRIPTION
 *	Pass VRIO to underlying I/O routine of affected children..
 *
 * REMARKS
 */
PRIVATE USHORT NEAR _Cdecl
StripeWorker(PVRDEVHDR const self,PVRIO const myio)
{
    PVRDSTRIPE const	stripe = (PVRDSTRIPE)self;
    PVRIO	io;
    PVRIO	newio;
    PVRIO	firstio = NULL;

    ULONG	firstblk = myio->blk;		/* first blk */
    ULONG const	lastblk = firstblk + myio->bcnt - 1;


    DEBMSG1(DBG_PATH,dszStripeWorker,(PVOID)myio);
    DEBMSG2(DBG_LEVEL3,dszSWorkerBlk,(PVOID)&firstblk,(PVOID)&lastblk);

    ASSERT( myio->vrd == self );
    ASSERT( myio->link == NULL );
    ASSERT( myio->bcnt != 0 );
    ASSERT( stripe->header.size != 0 );
    ASSERT( stripe->header.notify == StripeNotify );
    ASSERT( stripe->header.worker == StripeWorker );
    ASSERT( stripe->header.update == StripeUpdate );
    ASSERT( stripe->header.ioctl == StripeCtrl );

    ++self->iocnt;

    switch( myio->iocmd )
    {
	USHORT	i;

      case IOCMD_READ:
      case IOCMD_WRITE:
	/* Create VRIO to all affected children. */

	myio->children = 0;
	for(; firstblk <= lastblk ; firstblk += io->bcnt)
	{
	    USHORT	devidx, stroffs;
	    ULONG	strno;

	    DEBMSG1(DBG_LEVEL3,dszSWorkerStart,(PVOID)&firstblk);

	    /* Calculate child, offset and count for this block. */

	    /* Which band? */

	    strno = firstblk / (stripe->stripesize * stripe->children);

	    /* Wich device? */

	    devidx = (USHORT)((firstblk / stripe->stripesize) % stripe->children);

	    /* Offset in band? */

	    stroffs = (USHORT)(firstblk % stripe->stripesize);

	    DEBMSG3(DBG_LEVEL3,dszSWorkerStripe,(PVOID)&strno,stroffs,devidx);

	    /* Build linked list of VRIOs. */

	    newio = AllocateVrio(stripe->child[devidx], myio);
	    if( newio == NULL )			/* memory exhausted? */
		break;

	    if( firstio == NULL )
		firstio = newio;		/* save this as first */
	    else
		io->link = newio;
	    io = newio;
	    io->link = NULL;
	    ++myio->children;

	    io->iorb = myio->iorb;
	    io->iocmd = myio->iocmd;
	    io->cSGList = myio->cSGList;
	    io->pSGList = myio->pSGList;
	    io->ppSGList = myio->ppSGList;

	    io->offset = (USHORT)(firstblk - myio->blk);
	    io->blk = (strno * stripe->stripesize) + stroffs;
	    io->bcnt = (lastblk - firstblk + 1 < stripe->stripesize - stroffs
			? (USHORT)(lastblk - firstblk + 1) : stripe->stripesize - stroffs);

	    io->oserr = 0xFFFF;			/* invalidate error code */
	}
	break;


      case IOCMD_AREAD:
	/* Read: create for 'readchild' child and do another in Notify()
	 * if this first failed!
	 * Don't forget to increment 'readchild'. */

	myio->children = stripe->children;	/* to detect 'all children tried' */

	/* Allocate VRIOs and update links. */

	do
	{
	    newio = AllocateVrio(stripe->child[stripe->readchild], myio);
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

	    /* Move 'readchild'. */

	    pushf();
	    cli();
	    do
	    {
		stripe->readchild = (stripe->readchild + 1 == stripe->children
				     ? 0 : stripe->readchild + 1);
	    }
	    while( stripe->child[stripe->readchild] == NULL );
	    popf();
	}
	while(0);
	break;


      case IOCMD_AWRITE:
	/* Write: create VRIO to all children. */

	myio->children = 0;

	for( i = 0; i < stripe->children; ++i )
	{
	    if( stripe->child[i] == NULL )
		continue;			/* skip non-existing child */


	    /* Allocate VRIOs and update links. */

	    newio = AllocateVrio(stripe->child[i], myio);
	    if( newio == NULL )
	    {
		DEBMSG(DBG_ALL,dszSAdminNoalloc);
		break;				/* memory exhausted? */
	    }

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
    }


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

	    DEBMSG1(DBG_ALL,dszSWorkerChildMiss,io->iocmd);
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
} /* end[StripeWorker] */




/*# ----------------------------------------------------------------------
 * StripeUpdate(self,sec)
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
StripeUpdate(PVRDEVHDR const self,PVOID const sec)
{
    PVRDSTRIPE const	stripe = (PVRDSTRIPE)self;

    ASSERT( stripe->header.size != 0 );
    ASSERT( stripe->header.notify == StripeNotify );
    ASSERT( stripe->header.worker == StripeWorker );
    ASSERT( stripe->header.update == StripeUpdate );
    ASSERT( stripe->header.ioctl == StripeCtrl );

    return 0;
}




/*# ----------------------------------------------------------------------
 * StripeCtrl(self,cmd,param,data)
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
StripeCtrl(PVRDEVHDR const self, USHORT const cmd,
	   PVOID const param, PVOID const data)
{
    PVRDSTRIPE const	stripe = (PVRDSTRIPE)self;
    USHORT		result = STATUS_ERR_UNKCMD;

    ASSERT( stripe->header.size != 0 );
    ASSERT( stripe->header.notify == StripeNotify );
    ASSERT( stripe->header.worker == StripeWorker );
    ASSERT( stripe->header.update == StripeUpdate );
    ASSERT( stripe->header.ioctl == StripeCtrl );

    switch( cmd )
    {
      case VRDIOCTL_QUERY_CHILDREN:
	*(PUSHORT)data = stripe->children;
	result = 0;
	break;


      case VRDIOCTL_INFO:
	DEBMSG(DBG_PATH, "VRDIOCTL_INFO");
	{
	    PVRAID_ARRAYINFO_DATA	d = data;

	    _fmemcpy(d->id, stripe->header.id, sizeof(d->id));
	    d->reqcnt = stripe->header.iocnt;
	    d->size = stripe->header.size;

	    d->children = stripe->children;
	    d->flags = stripe->header.flags;
	    d->reserved = 0;
	}
	result = 0;
	break;


      default:
	break;
    }

    return result;
} /* end[StripeCtrl] */






/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * StripeCreate(pVrh,pPhys,sec,pSize)
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
StripeCreate(PVRDEVHDR pVrh,PVRDEVHDR FAR * tab,PSEC_VRDEV sec,PUSHORT pSize)
{
    PVRDSTRIPE const	stripe = (PVRDSTRIPE)pVrh;
    USHORT	i, found;


    DEBMSG2(DBG_INIT1,dszStripeCreate,(PVOID)pVrh,sec->u.s.children);

    stripe->header.worker = StripeWorker;
    stripe->header.notify = StripeNotify;
    stripe->header.update = StripeUpdate;
    stripe->header.ioctl = StripeCtrl;
    stripe->header.size = 0;			/* initialize, will be added to */
    stripe->header.flags = 0;
    _fmemcpy(stripe->header.id, sec->u.s.id, sizeof(stripe->header.id));
    DEBMSG2(DBG_INIT3,dszSCreateID,(PVOID)stripe->header.id,6);

    stripe->stripesize = STRIPE_SIZE;


    /* Build linking by looking up all parts in 'tab'. */

    stripe->children = sec->u.s.children;
    _fmemset(stripe->child, 0, stripe->children * sizeof(stripe->child[0]));
    found = 0;
    for( i = 0; i < stripe->children; ++i )
    {
	PDEVID const	id1 = &sec->u.s.child[i].id;
	USHORT		j;

	DEBMSG2(DBG_INIT3,dszSCreateLooking,(PVOID)id1,6);
	stripe->header.size += sec->u.s.child[i].size;
	for( j = 0; j < MAX_PDEV; ++j )
	{
	    if( tab[j] != NULL )		/* ignore empty entries */
	    {
		PDEVID const	id2 = &tab[j]->id;

		if( _fmemcmp(id1, id2, sizeof(DEVID)) == 0 )
		{
		    DEBMSG2(DBG_INIT1,dszSCreateFound,i,tab[j]);
		    stripe->child[i] = tab[j];
		    tab[j] = NULL;
		    ++found;
		    break;			/* stop scan, next child */
		}
	    }
	}
    }
    if( found != stripe->children )
    {
	DEBMSG2(DBG_ALL,dszSCreateOnly,found,stripe->children);
	stripe->header.flags = RDFLAG_ERROR;
    }

    while( stripe->child[stripe->readchild] == NULL )
    {
	stripe->readchild = (stripe->readchild + 1 == stripe->children
			     ? 0 : stripe->readchild + 1);
    }


    /* Size of administrative data depends on count of children. */

    *pSize = sizeof(*stripe) + stripe->children * sizeof(stripe->child[0]);
    return 0;
}
