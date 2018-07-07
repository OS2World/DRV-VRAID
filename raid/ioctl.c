/*
 * $Source: R:/source/driver/raid/RCS\\ioctl.c,v $
 * $Revision: 2.4 $
 * $Date: 2001/02/11 01:24:56 $
 * $Locker:  $
 *
 *	General IOCtl interface
 *
 * $Log: ioctl.c,v $
 * Revision 2.4  2001/02/11 01:24:56  vitus
 * - changed all routines to return DRVSTATUS
 *
 * Revision 2.3  2000/08/22 01:58:07  vitus
 * - implemented VRAID_SET_CHILD_STATE
 *
 * Revision 2.2  2000/07/24 23:57:30  vitus
 * - implemented VRAID_START_REBUILD
 * - VRD.ioctl() may now returns error code
 *
 * Revision 2.1  2000/06/15 01:04:24  vitus
 * - removed most calls to DevHelp_VerifyAccess.  Not neccessary on the IOCtl
 *   packets itself (see OS2PDD mailing list)
 * - implmented VRAID_VERIFY_ARRAY
 *
 * Revision 1.12  2000/04/15 22:23:05  vitus
 * - more debug messages
 *
 * Revision 1.11  1999/10/01 00:58:20  vitus
 * - QueryPhysinfo: returns new fields in VRAID_PHYSINFO_DATA (request/busy counts)
 *   if caller has enough room in his buffer.
 *
 * Revision 1.10  1999/09/23 23:03:08  vitus
 * - defined all DBGMSG() strings as located in CONST segment
 *
 * Revision 1.9  1999/07/05 01:42:12  vitus
 * - VRAID_START_SETUP: do SaveArrayInfos() first to let VSetup know the current
 *   states and have this current state saved if VSetup inhibits saves on shutdown.
 *
 * Revision 1.8  1999/06/20 17:31:28  vitus
 * - DriverVersion: return some flags, too
 * - ChangeBeeper() implemented
 *
 * Revision 1.7  1999/05/12 01:54:22  vitus
 * - added VRAID_START_SETUP, VRAID_END_SETUP
 *
 * Revision 1.6  1999/04/21 00:30:09  vitus
 * - DoAdminIO: switched from PhysdevPartIO() and offset calculation
 *   to ProcessAdminIO() and offset 1 (fix!)
 *
 * Revision 1.5  1998/12/13 22:29:59  vitus
 * - implemented VRAID_QUERY_ARRAYLIST and VRAID_QUERY_ARRAYINFO
 *
 * Revision 1.4  1998/09/27 16:08:02  vitus
 * - implemented VRAID_QUERY_PHYSLIST, VRAID_QUERY_PHYSINFO, VRAID_ADMINSEC_IO
 *
 * Revision 1.3  1998/06/26 22:38:12  vitus
 * - switched to DDK macros (cli, sti, pushf)
 * - corrected swapable segment name (uppercase)
 *
 * Revision 1.2  1998/05/29 01:44:20  vitus
 * - added debug level parameter to DEBMSG macro
 *
 * Revision 1.1  1998/02/18 00:03:13  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1997-2001
 */
#if defined(__WATCOMC__)
# pragma off (unreferenced)
#endif
static char const id[]="$Id: ioctl.c,v 2.4 2001/02/11 01:24:56 vitus Exp $";
#if defined(__WATCOMC__)
# pragma on (unreferenced)
#endif

#include <string.h>

#define INCL_NOBASEAPI
#define INCL_DOSERRORS
#include "os2intf.h"
#include <bseerr.h>

#include "vraid.h"
#include "proto.h"
#include "extern.h"

#include "vraidpub.h"




/* Save messages here.  This buffer is located in a swapable
 * data segment and may only access at task time!
 * OBS: M$-C tends to allocate a const containing the
 * segment of these variables.  To prevent this use strange code
 * and no optimization at all. */

#define _inswap		_based(_segname("SwapData"))
PRIVATE USHORT _inswap	iNextString = 0;
PRIVATE UCHAR _inswap	DisplayBuffer[DISPLAYBUFFER_SIZE];




#if defined(DEBUG)
CONSTMSG dszIoctlParam2small[]=	"\r\n[IOCtl %w] need >=2 bytes parameter!";
CONSTMSG dszIoctlData2small[]=	"\r\n[IOCtl %w] need >=2 bytes data buffer!";
CONSTMSG dszIoctlDAccess[]= 	"\r\n[IOCtl %w] data buffer too small/invalid!";

CONSTMSG dszInvPHandle[]=	"\r\n[Hd2Dev] didn't find handle %w in table";
CONSTMSG dszInvHHandle[]= 	"\r\n[Hd2Drv] didn't find handle %w in table";

CONSTMSG dszQueryPInfo[]= 	"\r\n[QueryPhysinfo] handle %w";
CONSTMSG dszQueryAInfo[]= 	"\r\n[QueryArrayinfo] handle %w";
CONSTMSG dszVerifyArray[]= 	"\r\n[VerifyArray] handle %w";
CONSTMSG dszStartRebuild[]= 	"\r\n[StartRebuild] handle %w";
CONSTMSG dszSetChildState[]= 	"\r\n[SetChildState] handle %w";

CONSTMSG dszQueryPListWrite[]="\r\n[QueryPhyslist] data buffer can't be written to!";
CONSTMSG dszQueryPListIndex[]=	"\r\n[QueryPhyslist] index %w = handle %w!";
CONSTMSG dszQAListAccess[]= "\r\n[QueryArraylist] data buffer can't be written to!";
CONSTMSG dszQAListIndex[]=	"\r\n[QueryArraylist] index %w = handle %w!";
CONSTMSG dszDAIoLock[]=		"\r\n[DoAdminIO] DevHelp_Lock(%p) failed!";
CONSTMSG dszDAIoPhys[]=		"\r\n[DoAdminIO] DevHelp_VirtToPhys(%p) failed!";
CONSTMSG dszScreen[]=		"\r\n--- screen --- %s";
CONSTMSG dszIoctlUnknownCat[]=	"\r\nUnknown IOCtl category %w";
CONSTMSG dszIoctlUnknownFct[]=	"\r\nUnknown IOCtl function %w";
#endif






/* Macro to verify that a certain memory range is
 * readable/writable (depending on w). */

#define CheckPointer(p,s,w)	DevHelp_VerifyAccess(FP_SEL(p),(s),FP_OFF(p),(w))






/* **********************************************************************
 * **** Private Support Routines ****************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * Hd2Dev(hd)
 *
 * PARAMETER
 *	hd		handle as provided by application
 * RETURNS
 *	PVRDEVHDR	physical device header
 * GLOBAL
 *	apPDev, cPDev
 * DESCRIPTION
 *	Translate handle reported to and provided by application
 *	back to a VRDEVHDR pointer.
 *
 * REMARKS
 */
PRIVATE PVRDEVHDR NEAR
Hd2Dev(USHORT const hd)
{
    USHORT	i;

    for( i = 0; i < cPDev; ++i )
	if( OFFSETOF(apPDev[i]) == hd )
	    return apPDev[i];

    DEBMSG1(DBG_LEVEL1,dszInvPHandle,hd);
    return NULL;
}




/*# ----------------------------------------------------------------------
 * Hd2Drv(hd)
 *
 * PARAMETER
 *	hd		handle as provided by application
 * RETURNS
 *	PHOSTDRIVE	host drive structure
 * GLOBAL
 *	apHDrv, cHDrv
 * DESCRIPTION
 *	Translate handle reported to and provided by application
 *	back to a HOSTDRIVE pointer.
 *
 * REMARKS
 */
PRIVATE PHOSTDRIVE NEAR
Hd2Drv(USHORT const hd)
{
    USHORT	i;

    for( i = 0; i < cHDrv; ++i )
	if( OFFSETOF(apHDrv[i]) == hd )
	    return apHDrv[i];

    DEBMSG1(DBG_LEVEL1,dszInvHHandle,hd);
    return NULL;
}






/* **********************************************************************
 * **** A Function for every IOCtl **************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * DriverVersion(rp)
 *
 * PARAMETER
 *	request packet
 * RETURNS
 *	driver status word
 * GLOBAL
 *	(none)
 * DESPRIPTION
 *	Returns version of this driver.  Coded like the MS-DOS
 *	version number.
 *
 * REMARKS
 */
PRIVATE DRVSTATUS NEAR
DriverVersion(PRP_GENIOCTL rp)
{
    PUSHORT const	datasize = &rp->DataLen;
    USHORT const	dsize = (*datasize == 0 ? 0xFFFF : *datasize);
    PVRAID_VER_DATA const data = (PVRAID_VER_DATA)rp->DataPacket;
    USHORT		cb;
    DRVSTATUS		rc = STATUS_DONE;


    do
    {
	cb = sizeof(data->version);		/* only version ... */
	if( dsize < cb  ||  CheckPointer(rp->DataPacket, cb, 1) )
	{
	    rc = STATUS_ERR_INVPARAM;
	    break;
	}

	data->version = MAKEUSHORT(SUBVERSION,VERSION);


	cb += sizeof(data->flags);
	if( dsize < cb  ||  CheckPointer(rp->DataPacket, cb, 1) )
	    break;				/* not enough room */

	data->flags = (UCHAR)uRegLevel;
	if( (fDriverFlags & DF_SETUPWRITTEN) )
	    data->flags |= 0x10;
	if( (fDriverFlags & DF_BEEPDISABLED) )
	    data->flags |= 0x20;


	cb += sizeof(data->builds);
	if( dsize < cb  ||  CheckPointer(rp->DataPacket, cb, 1) )
	    break;				/* not enough room */

	data->builds = cBuildProcess;
    }
    while( 0 );

    return rc;
}




/*# ----------------------------------------------------------------------
 * ReadMessages(rp)
 *
 * PARAMETER
 *	rp		request packet
 * RETURNS
 *	driver status word
 * GLOBAL
 *	DisplayBuffer,iNextString
 * DESPRIPTION
 *	Copies message buffer to user buffer.
 *
 * REMARKS
 *	Remember to append another 0 byte to end of buffer!  See SaveMessage()
 */
#pragma optimize("",off)			/* keep optimizer quiet */
PRIVATE DRVSTATUS NEAR
ReadMessages(PRP_GENIOCTL rp)
{
    PUSHORT const	datasize = &rp->DataLen;
    USHORT		dsize = (*datasize == 0 ? 0xFFFF : *datasize);
    PVRAID_MSGS_DATA const data = (PVRAID_MSGS_DATA)rp->DataPacket;
    PUSHORT		pcbMsg = &iNextString;
    PUCHAR		pMsg = DisplayBuffer;
    DRVSTATUS		rc = STATUS_ERR_INVPARAM;


    *datasize = *pcbMsg + FIELDOFFSET(VRAID_MSGS_DATA,msg) + 1;

    if( dsize < FIELDOFFSET(VRAID_MSGS_DATA,msg)
	||  CheckPointer(data, FIELDOFFSET(VRAID_MSGS_DATA,msg), 1) )
	return rc;
    data->cb = *datasize;			/* tell caller */

    if( dsize > *datasize )
	dsize = *datasize;
    if( CheckPointer(data, dsize, 1) )
	return rc;

    /* If some bytes available in caller buffer (more than
     * just 'cb'), copy part/all of message buffer. */

    if( dsize - FIELDOFFSET(VRAID_MSGS_DATA,msg) > 0 )
	_fmemcpy(data->msg, pMsg, dsize - FIELDOFFSET(VRAID_MSGS_DATA,msg));

    return STATUS_DONE;
}
#pragma optimize("",)




/*# ----------------------------------------------------------------------
 * ClearMessages(rp)
 *
 * PARAMETER
 *	rp		request packet
 * RETURNS
 *	driver status word
 * GLOBAL
 *	iNextString
 * DESPRIPTION
 *	Removes all messages from DisplayBuffer.
 *
 * REMARKS
 */
#pragma optimize("",off)			/* keep optimizer quiet */
PRIVATE DRVSTATUS NEAR
ClearMessages(PRP_GENIOCTL rp)
{
    PUSHORT const	parmsize = &rp->ParmLen;
    PUSHORT const	datasize = &rp->DataLen;

    PUSHORT	pi = &iNextString;
    USHORT	rc = STATUS_DONE;

    *parmsize = 0;
    *datasize = 0;

    *pi = 0;					/* to beginning of buffer */
    return rc;
}
#pragma optimize("",)




/*# ----------------------------------------------------------------------
 * QueryPhyslist(rp)
 *
 * PARAMETER
 *	rp		request packet
 * RETURNS
 *	driver status word
 * GLOBAL
 *	apPDev, cPDev
 * DESPRIPTION
 *	Copies table to user buffer.
 *
 * REMARKS
 */
PRIVATE DRVSTATUS NEAR
QueryPhyslist(PRP_GENIOCTL rp)
{
    PUSHORT const	datasize = &rp->DataLen;
    USHORT		dsize = (*datasize == 0 ? 0xFFFF : *datasize);
    PVRAID_PHYSLIST_DATA const data = (PVOID)rp->DataPacket;
    USHORT		i;
    DRVSTATUS		rc = STATUS_ERR_INVPARAM;


    *datasize = cPDev * sizeof(USHORT) + FIELDOFFSET(VRAID_PHYSLIST_DATA,hd);

    do
    {
	if( dsize < FIELDOFFSET(VRAID_PHYSLIST_DATA,hd)
	    ||  CheckPointer(data, FIELDOFFSET(VRAID_PHYSLIST_DATA,hd), 1) )
	{
	    DEBMSG1(DBG_LEVEL1,dszIoctlData2small,rp->rph.Cmd);
	    break;
	}
	data->cb = *datasize;			/* tell caller */

	if( dsize > *datasize )
	    dsize = *datasize;
	if( CheckPointer(data, dsize, 1) )
	{
	    DEBMSG(DBG_LEVEL1,dszQueryPListWrite);
	    break;
	}

	/* If some bytes available in caller buffer (more than
	 * just 'cb'), copy part/all of message buffer. */

	for( i = 0;
	     i * sizeof(USHORT) + FIELDOFFSET(VRAID_PHYSLIST_DATA,hd) < dsize;
	     ++i )
	{
	    data->hd[i] = OFFSETOF(apPDev[i]);	/* 'hd'? Take structure pointer! */
	    DEBMSG2(DBG_PATH,dszQueryPListIndex,i,data->hd[i]);
	}
	rc = STATUS_DONE;
    }
    while(0);

    return rc;
} /* end[QueryPhyslist()] */




/*# ----------------------------------------------------------------------
 * QueryPhysinfo(rp)
 *
 * PARAMETER
 *	rp		request packet
 * RETURNS
 *	driver status word
 * GLOBAL
 *	apPDev, cPDev
 * DESPRIPTION
 *	Copies physical information to user buffer.
 *
 * REMARKS
 */
PRIVATE DRVSTATUS NEAR
QueryPhysinfo(PRP_GENIOCTL rp)
{
    PUSHORT const	parmsize = &rp->ParmLen;
    PUSHORT const	datasize = &rp->DataLen;
    USHORT const	psize = (*parmsize == 0 ? 0xFFFF : *parmsize);
    USHORT const	dsize = (*datasize == 0 ? 0xFFFF : *datasize);
    PUSHORT const	parm = (PUSHORT)rp->ParmPacket;
    PVRAID_PHYSINFO_DATA const data = (PVRAID_PHYSINFO_DATA)rp->DataPacket;

    DRVSTATUS		rc;


    /* Fill the (still not updated) size fields of the DosDevIOCtl()
     * call with the very maximum bytes count returned.
     * Despite the fact that this isn't passed to the application
     * we can use those values later in our function. */

    *parmsize = sizeof(USHORT);
    *datasize = sizeof(VRAID_PHYSINFO_DATA);

    do
    {
	PVRDEVHDR	vrd;

	if( psize < sizeof(USHORT)  ||  CheckPointer(parm, sizeof(USHORT), 0) )
	{
	    DEBMSG1(DBG_LEVEL1,dszIoctlParam2small,rp->rph.Cmd);
	    rc = STATUS_ERR_INVPARAM;
	    break;
	}

	vrd = Hd2Dev(*parm);
	if( vrd == NULL )
	{
	    rc = STATUS_ERR_UNKUNIT;
	    break;
	}

	DEBMSG1(DBG_LEVEL1,dszQueryPInfo,*parm);
	if( dsize < FIELDOFFSET(VRAID_PHYSINFO_DATA,adapter)
	    ||  CheckPointer(data, FIELDOFFSET(VRAID_PHYSINFO_DATA,adapter), 1) )
	{
	    DEBMSG1(DBG_LEVEL1,dszIoctlData2small,rp->rph.Cmd);
	    rc = STATUS_ERR_INVPARAM;
	    break;
	}

	data->cb = *datasize;			/* tell caller, there is room */


	/* If enough bytes available: fill structure. */

	if( dsize >= FIELDOFFSET(VRAID_PHYSINFO_DATA,reqcnt) )
	{
	    if( CheckPointer(data, FIELDOFFSET(VRAID_PHYSINFO_DATA,reqcnt), 1) )
	    {
		rc = STATUS_ERR_INVPARAM;
		break;
	    }

	    /* We should cast 'vrd' to type PVRDPHYS, but ... that type
	     * is internal to physdev.c! */

	    data->adapter = 0;
	    data->target = 0;
	    data->lun = 0;
	    data->partsize = vrd->size;
	    data->totalsize = 0;
	}
	if( dsize >= sizeof(VRAID_PHYSINFO_DATA) )
	{
	    if( CheckPointer(data, sizeof(VRAID_PHYSINFO_DATA), 1) )
	    {
		rc = STATUS_ERR_INVPARAM;
		break;
	    }

	    data->reqcnt = vrd->iocnt;
	    data->busycnt = vrd->busycnt;
	}


	rc = STATUS_DONE;
    }
    while(0);

    return rc;
} /* end[QueryPhysinfo] */




/*# ----------------------------------------------------------------------
 * QueryArraylist(rp)
 *
 * PARAMETER
 *	rp		request packet
 * RETURNS
 *	driver status word
 * GLOBAL
 *	apHDrv, cHDrv
 * DESPRIPTION
 *	Copies table to user buffer.
 *
 * REMARKS
 */
PRIVATE DRVSTATUS NEAR
QueryArraylist(PRP_GENIOCTL rp)
{
    PUSHORT const	datasize = &rp->DataLen;
    USHORT		dsize = (*datasize == 0 ? 0xFFFF : *datasize);
    PVRAID_ARRAYLIST_DATA const data = (PVOID)rp->DataPacket;
    USHORT		i;
    DRVSTATUS		rc = STATUS_ERR_INVPARAM;


    *datasize = cHDrv * sizeof(USHORT) + FIELDOFFSET(VRAID_ARRAYLIST_DATA,hd);

    do
    {
	if( dsize < FIELDOFFSET(VRAID_ARRAYLIST_DATA,hd)
	    ||  CheckPointer(data, FIELDOFFSET(VRAID_ARRAYLIST_DATA,hd), 1) )
	{
	    DEBMSG1(DBG_LEVEL1,dszIoctlData2small,rp->rph.Cmd);
	    break;
	}
	data->cb = *datasize;			/* tell caller */

	if( dsize > *datasize )
	    dsize = *datasize;
	if( CheckPointer(data, dsize, 1) )
	{
	    DEBMSG(DBG_LEVEL1,dszQAListAccess);
	    break;
	}

	/* If some bytes available in caller buffer (more than
	 * just 'cb'), copy part/all of message buffer. */

	for( i = 0;
	     i * sizeof(USHORT) + FIELDOFFSET(VRAID_ARRAYLIST_DATA,hd) < dsize;
	     ++i )
	{
	    data->hd[i] = OFFSETOF(apHDrv[i]);	/* 'hd'? Take structure pointer! */
	    DEBMSG2(DBG_PATH,dszQAListIndex,i,data->hd[i]);
	}
	rc = STATUS_DONE;
    }
    while(0);

    return rc;
} /* end[QueryArraylist()] */




/*# ----------------------------------------------------------------------
 * QueryArrayinfo(rp)
 *
 * PARAMETER
 *	rp		request packet
 *	apHDrv, cHDrv
 *
 * RETURNS
 *	driver status word
 *
 * DESPRIPTION
 *	Copies array drive information to user buffer.
 *
 * REMARKS
 */
PRIVATE DRVSTATUS NEAR
QueryArrayinfo(PRP_GENIOCTL rp)
{
    PUSHORT const	parmsize = &rp->ParmLen;
    PUSHORT const	datasize = &rp->DataLen;
    USHORT const	psize = (*parmsize == 0 ? 0xFFFF : *parmsize);
    USHORT const	dsize = (*datasize == 0 ? 0xFFFF : *datasize);
    PUSHORT const	parm = (PUSHORT)rp->ParmPacket;
    PVRAID_ARRAYINFO_DATA const data = (PVRAID_ARRAYINFO_DATA)rp->DataPacket;

    DRVSTATUS		rc;


    *parmsize = sizeof(USHORT);
    *datasize = sizeof(VRAID_ARRAYINFO_DATA);

    do
    {
	PHOSTDRIVE	drv;

	if( psize < sizeof(USHORT) )
	{
	    DEBMSG1(DBG_LEVEL1,dszIoctlParam2small,rp->rph.Cmd);
	    rc = STATUS_ERR_INVPARAM;
	    break;
	}

	drv = Hd2Drv(*parm);
	if( drv == NULL )
	{
	    rc = STATUS_ERR_UNKUNIT;
	    break;
	}

	DEBMSG1(DBG_LEVEL1,dszQueryAInfo,*parm);
	if( dsize < FIELDOFFSET(VRAID_ARRAYINFO_DATA,id) )
	{
	    DEBMSG1(DBG_LEVEL1,dszIoctlData2small,rp->rph.Cmd);
	    rc = STATUS_ERR_INVPARAM;
	    break;
	}

	data->cb = *datasize;			/* tell caller */


	/* If enough bytes available: fill structure. */

	if( dsize >= sizeof(VRAID_ARRAYINFO_DATA) )
	{
	    drv->child->ioctl(drv->child, VRDIOCTL_INFO, 0, data);
	    data->reqcnt = drv->ReqCnt;		/* overlay request counter */
	}

	rc = STATUS_DONE;
    }
    while(0);

    return rc;
} /* end[QueryArrayinfo] */




/*# ----------------------------------------------------------------------
 * VerifyArray(rp)
 *
 * PARAMETER
 *	rp		request packet
 *	apHDrv, cHDrv
 *
 * RETURNS
 *	driver status word
 *
 * DESPRIPTION
 *	Verifies data integry.
 *
 * REMARKS
 */
PRIVATE DRVSTATUS NEAR
VerifyArray(PRP_GENIOCTL rp)
{
    PUSHORT const	parmsize = &rp->ParmLen;
    PUSHORT const	datasize = &rp->DataLen;
    USHORT const	psize = (*parmsize == 0 ? 0xFFFF : *parmsize);
    USHORT const	dsize = (*datasize == 0 ? 0xFFFF : *datasize);
    PVRAID_VERIFY_PARM const	parm = (PVOID)rp->ParmPacket;
    PVRAID_ARRAYINFO_DATA const data = (PVOID)rp->DataPacket;

    USHORT		us;
    DRVSTATUS		rc;


    *parmsize = sizeof(VRAID_VERIFY_PARM);
    *datasize = sizeof(VRAID_ARRAYINFO_DATA);

    do
    {
	PHOSTDRIVE	drv;

	if( psize < sizeof(VRAID_VERIFY_PARM) )
	{
	    DEBMSG1(DBG_LEVEL1,dszIoctlData2small,rp->rph.Cmd);
	    rc = STATUS_ERR_INVPARAM;
	    break;
	}

	drv = Hd2Drv(parm->handle);
	if( drv == NULL )
	{
	    rc = STATUS_ERR_UNKUNIT;
	    break;
	}
	DEBMSG1(DBG_LEVEL2,dszVerifyArray,parm->handle);

	drv->child->ioctl(drv->child, VRDIOCTL_QUERY_CHILDREN, 0, &us);
	*datasize += us * (sizeof(VRAID_VERIFY_DATA) - 1
			   + parm->seccnt * SECTOR_SIZE);

	if( dsize < *datasize )
	{
	    DEBMSG1(DBG_LEVEL1,dszIoctlDAccess,rp->rph.Cmd);
	    rc = STATUS_ERR_INVPARAM;
	    break;
	}

	data->cb = *datasize;			/* tell caller */


	rc = drv->child->ioctl(drv->child, VRDIOCTL_VERIFY, parm, data);
	data->reqcnt = drv->ReqCnt;		/* overlay request counter */

	if( rc == 0 )
	    rc = STATUS_DONE;
    }
    while(0);

    return rc;
} /* end[VerifyArray] */




/*# ----------------------------------------------------------------------
 * StartRebuild(rp)
 *
 * PARAMETER
 *	rp		request packet
 *	apHDrv, cHDrv
 *
 * RETURNS
 *	driver status word
 *
 * DESPRIPTION
 *	Forces array to rebuild.
 *
 * REMARKS
 */
PRIVATE DRVSTATUS NEAR
StartRebuild(PRP_GENIOCTL rp)
{
    PUSHORT const	parmsize = &rp->ParmLen;
    PUSHORT const	datasize = &rp->DataLen;
    USHORT const	psize = (*parmsize == 0 ? 0xFFFF : *parmsize);
    USHORT const	dsize = (*datasize == 0 ? 0xFFFF : *datasize);
    PVRAID_REBUILD_PARM const	parm = (PVOID)rp->ParmPacket;
    PVRAID_ARRAYINFO_DATA const data = (PVOID)rp->DataPacket;

    DRVSTATUS		rc;


    *parmsize = sizeof(VRAID_REBUILD_PARM);
    *datasize = sizeof(VRAID_ARRAYINFO_DATA);

    do
    {
	PHOSTDRIVE	drv;

	if( psize < sizeof(VRAID_REBUILD_PARM) )
	{
	    DEBMSG1(DBG_LEVEL1,dszIoctlData2small,rp->rph.Cmd);
	    rc = STATUS_ERR_INVPARAM;
	    break;
	}

	drv = Hd2Drv(parm->handle);
	if( drv == NULL )
	{
	    rc = STATUS_ERR_UNKUNIT;
	    break;
	}
	DEBMSG1(DBG_LEVEL1,dszStartRebuild,parm->handle);

	data->cb = *datasize;			/* tell caller */


	rc = drv->child->ioctl(drv->child, VRDIOCTL_START_REBUILD, parm, data);
	data->reqcnt = drv->ReqCnt;		/* overlay request counter */

	if( rc == 0 )
	    rc = STATUS_DONE;
    }
    while(0);

    return rc;
} /* end[StartRebuild] */




/*# ----------------------------------------------------------------------
 * SetChildState(rp)
 *
 * PARAMETER
 *	rp		request packet
 *	apHDrv, cHDrv
 *
 * RETURNS
 *	driver status word
 *
 * DESPRIPTION
 *	Tells array something about a child.
 *
 * REMARKS
 */
PRIVATE DRVSTATUS NEAR
SetChildState(PRP_GENIOCTL rp)
{
    PUSHORT const	parmsize = &rp->ParmLen;
    PUSHORT const	datasize = &rp->DataLen;
    USHORT const	psize = (*parmsize == 0 ? 0xFFFF : *parmsize);
    USHORT const	dsize = (*datasize == 0 ? 0xFFFF : *datasize);
    PVRAID_CHILD_PARAM const	parm = (PVOID)rp->ParmPacket;
    PVOID const 	data = (PVOID)rp->DataPacket;

    DRVSTATUS		rc;


    *parmsize = sizeof(PVRAID_CHILD_PARAM);
    *datasize = 0;

    do
    {
	PHOSTDRIVE	drv;

	if( psize < sizeof(PVRAID_CHILD_PARAM) )
	{
	    DEBMSG1(DBG_LEVEL1,dszIoctlData2small,rp->rph.Cmd);
	    rc = STATUS_ERR_INVPARAM;
	    break;
	}

	drv = Hd2Drv(parm->handle);
	if( drv == NULL )
	{
	    rc = STATUS_ERR_UNKUNIT;
	    break;
	}
	DEBMSG1(DBG_LEVEL1,dszSetChildState,parm->handle);


	rc = drv->child->ioctl(drv->child, VRDIOCTL_CHILD_STATE, parm, data);
	if( rc == 0 )
	    rc = STATUS_DONE;
    }
    while(0);

    return rc;
} /* end[SetChildState] */




/*# ----------------------------------------------------------------------
 * DoAdminIO(rp)
 *
 * PARAMETER
 *	rp		request packet
 * RETURNS
 *	driver status word
 * GLOBAL
 *	apPDev, cPDev
 * DESPRIPTION
 *	Read a single sector from administratiive area.
 *
 * REMARKS
 */
PRIVATE DRVSTATUS NEAR
DoAdminIO(PRP_GENIOCTL rp)
{
    PUSHORT const	parmsize = &rp->ParmLen;
    PUSHORT const	datasize = &rp->DataLen;
    USHORT const	psize = (*parmsize == 0 ? 0xFFFF : *parmsize);
    USHORT const	dsize = (*datasize == 0 ? 0xFFFF : *datasize);
    PVRAID_IO_PARM const parm = (PVOID)rp->ParmPacket;
    PVOID const		data = rp->DataPacket;
    DRVSTATUS		rc;


    *parmsize = sizeof(VRAID_IO_PARM);
    *datasize = SECTOR_SIZE;

    do
    {
	PVRDEVHDR	vrd;
	ULONG		phys, hLock;

	if( psize < sizeof(VRAID_IO_PARM)
	    ||  CheckPointer(parm, sizeof(VRAID_IO_PARM), 0) )
	{
	    DEBMSG1(DBG_LEVEL1,dszIoctlParam2small,rp->rph.Cmd);
	    rc = STATUS_ERR_INVPARAM;
	    break;
	}
	if( dsize < SECTOR_SIZE  ||  CheckPointer(data, SECTOR_SIZE, 1) )
	{
	    DEBMSG1(DBG_LEVEL1,dszIoctlData2small,rp->rph.Cmd);
	    rc = STATUS_ERR_INVPARAM;
	    break;
	}

	vrd = Hd2Dev(parm->hd);
	if( vrd == NULL )
	{
	    rc = STATUS_ERR_UNKUNIT;
	    break;
	}


	if( DevHelp_Lock(FP_SEL(data), LOCKTYPE_LONG_ANYMEM, 0, &hLock) )
	{
	    DEBMSG1(DBG_LEVEL1,dszDAIoLock,data);
	    rc = STATUS_ERR_INVPARAM;
	    break;
	}
	if( DevHelp_VirtToPhys(data, &phys) )
	{
	    DEBMSG1(DBG_LEVEL1,dszDAIoPhys,data);
	    DevHelp_UnLock(hLock);
	    rc = STATUS_ERR_INVPARAM;
	    break;
	}

	if( (parm->flags & VIOF_WRITE) )
	    fDriverFlags |= DF_SETUPWRITTEN;
	rc = ProcessAdminIO(vrd, (parm->flags & VIOF_WRITE ? TRUE : FALSE),
			    parm->partsecno, 1, phys);

	DevHelp_UnLock(hLock);			/* release memory lock */
	if( rc )
	    break;
	rc = STATUS_DONE;
    }
    while(0);

    return rc;
} /* end[DoAdminIO] */




/*# ----------------------------------------------------------------------
 * ChangeBeeper(rp)
 *
 * PARAMETER
 *	rp		request packet
 *
 * RETURNS
 *	driver status word
 *
 * DESPRIPTION
 *	Allows to disable/enable beeping.
 *
 * REMARKS
 */
PRIVATE DRVSTATUS NEAR
ChangeBeeper(PRP_GENIOCTL const rp)
{
    PUSHORT const	parmsize = &rp->ParmLen;
    PUSHORT const	datasize = &rp->DataLen;
    USHORT const	psize = (*parmsize == 0 ? 0xFFFF : *parmsize);
    USHORT const	dsize = (*datasize == 0 ? 0xFFFF : *datasize);
    PUSHORT const	parm = (PUSHORT)rp->ParmPacket;


    *parmsize = sizeof(USHORT);
    *datasize = 0;

    if( psize < sizeof(USHORT)  ||  CheckPointer(parm, sizeof(USHORT), 0) )
    {
	DEBMSG1(DBG_LEVEL1,dszIoctlParam2small,rp->rph.Cmd);
	return STATUS_ERR_INVPARAM;
    }

    if( *parm == 0 )
	fDriverFlags |= DF_BEEPDISABLED;
    else
	fDriverFlags &= ~DF_BEEPDISABLED;

    return STATUS_DONE;
} /* end[ChangeBeeper] */






/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * SaveMessage(void)
 *
 * PARAMETER
 *	none
 * RETURNS
 *	nothing
 * GLOBAL
 *	szMsgBuffer,msgBuffer		message
 *	DisplayBuffer,iNextString	swapdata buffer
 * DESPRIPTION
 *	Copies 'szMsgBuffer' to swapdata buffer and, if still
 *	in init phase 1, to screen via DevHelp_Save_Messsage.
 *
 * REMARKS
 */
#pragma optimize("",off)
PUBLIC void NEAR
SaveMessage(void)
{
    PUSHORT	pi = &iNextString;

    pushf();
    cli();
    if( *pi+_fstrlen(szMsgBuffer) < DISPLAYBUFFER_SIZE )
    {
	_fstrcpy(&DisplayBuffer[*pi], szMsgBuffer);
	*pi += _fstrlen(szMsgBuffer) + 1;
	DisplayBuffer[*pi] = '\0';		/* double 0 */
    }
    popf();

    DEBMSG1(DBG_ALL,dszScreen,(PSZ)szMsgBuffer);
    if( !(fDriverFlags & DF_INIT1DONE) )
	DevHelp_Save_Message((NPBYTE)&msgBuffer);
    return;
}
#pragma optimize("",)




/*# ----------------------------------------------------------------------
 * GenIOCtl(rp)
 *
 * PARAMETER
 *	rp		request packet
 * RETURNS
 *	driver status word
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Calls approbiate subfunction.
 * REMARKS
 */
PUBLIC DRVSTATUS
GenIOCtl(PRP_GENIOCTL rp)
{
    DRVSTATUS	rc;


    if( rp->Category != IOCTL_VRAID_CATEGORY )
    {
	DEBMSG1(DBG_LEVEL1,dszIoctlUnknownCat,rp->Category);
	return STATUS_ERR_UNKCMD;
    }

    switch( rp->Function )
    {
      case VRAID_QUERY_VERSION:
	rc = DriverVersion(rp);
	break;

      case VRAID_READ_MSGS:
	rc = ReadMessages(rp);
	break;

      case VRAID_CLEAR_MSGS:
	rc = ClearMessages(rp);
	break;

      case VRAID_QUERY_PHYSLIST:
	rc = QueryPhyslist(rp);
	break;

      case VRAID_QUERY_PHYSINFO:
	rc = QueryPhysinfo(rp);
	break;

      case VRAID_QUERY_ARRAYLIST:
	rc = QueryArraylist(rp);
	break;

      case VRAID_QUERY_ARRAYINFO:
	rc = QueryArrayinfo(rp);
	break;

      case VRAID_VERIFY_ARRAY:
	rc = VerifyArray(rp);
	break;

      case VRAID_START_REBUILD:
	rc = StartRebuild(rp);
	break;

      case VRAID_SET_CHILD_STATE:
	rc = SetChildState(rp);
	break;

      case VRAID_ADMINSEC_IO:
	rc = DoAdminIO(rp);
	break;

      case VRAID_START_SETUP:
	SaveArrayInfos();			/* just in case it gets modified */
	fDriverFlags |= DF_SETUPACTIVE;
	rc = STATUS_DONE;
	break;

      case VRAID_END_SETUP:
	fDriverFlags &= ~DF_SETUPACTIVE;
	rc = STATUS_DONE;
	break;

      case VRAID_SET_BEEPER:
	rc = ChangeBeeper(rp);
	break;

      default:
	DEBMSG1(DBG_LEVEL1,dszIoctlUnknownFct,rp->Function);
	rc = STATUS_ERR_UNKCMD;
	break;
    }

    return rc;
}
