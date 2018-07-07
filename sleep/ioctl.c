/*
 * $Source: R:/source/driver/sleep/RCS/ioctl.c,v $
 * $Revision: 1.11 $
 * $Date: 1999/11/29 00:38:33 $
 * $Locker:  $
 *
 *	General IOCtl interface
 *	These functions don't return an error if someone
 *	uses 16bit-DosDevIOCtl with a too-small buffer.  They
 *	just kills that app (because of an memory protection
 *	fault).
 *
 * $Log: ioctl.c,v $
 * Revision 1.11  1999/11/29 00:38:33  vitus
 * - moved all system includes to dsksleep.h
 *
 * Revision 1.10  1998/09/23 01:19:53  vitus
 * - switched to WatCom's intrinsic _f* routines
 *
 * Revision 1.9  1997/12/05 01:35:40  vitus
 * - added WatCom specific code and header
 * - moved history to bottom of file
 *
 * Revision 1.8  1997/11/09 00:56:20  vitus
 * - modified for new DDK (request contains length fields)
 *
 * Revision 1.7  1997/09/03 01:05:12  vitus
 * - implemented DSKSL_GET_LASTSTATUS
 *
 * Revision 1.6  1997/06/03 23:54:10  vitus
 * - corrected syntax error
 *
 * Revision 1.5  1997/05/11 01:51:13  vitus
 * - ChangeList: only update counter when reducing usSleep
 *
 * Revision 1.4  1997/05/07 23:04:26  vitus
 * - Handle partial copy of message buffer correctly
 * - Return consistent timer values even if timeout changed or zero
 * - Corrected data->cb/datasize values (thanks to N.Dufva)
 * - Allows IOCtl to query partial version info
 *
 * Revision 1.3  1997/04/07 01:04:22  vitus
 * Modifies/Checks application datasize values
 * VERSION returns registration flag, too
 *
 * Revision 1.2  1997/03/05 22:01:55  vitus
 * SaveMessage() to save/display messages
 * IOCtls to read/clear those messages
 * IOCtls now specify times in seconds
 * Query/ChangeDeviceState added
 *
 * Revision 1.1  1997/02/06 01:09:42  vitus
 * Initial revision
 * -------------------------------------------
 * This code is Copyright Vitus Jensen 1996-99
 */
static char id[]="$Id: ioctl.c,v 1.11 1999/11/29 00:38:33 vitus Exp $";

#define INCL_NOBASEAPI
#define INCL_DOSERRORS
#define INCL_NOPMAPI
#include "dsksleep.h"
#include "proto.h"
#include "extern.h"

#include "dskslpub.h"


#define PTRDIFF(a,b)	((PUCHAR)(a)-(PUCHAR)(b))



/* Save messages here.  This buffer is located in a swapable
 * data segment and may only access at task time!
 * OBS: M$-C tends to allocate a const containing the
 * segment of these variables.  To prevent this use strange code
 * and no optimization at all. */

#define _inswap		_based(_segname("SwapData"))
PRIVATE USHORT _inswap	iNextString = 0;
PRIVATE UCHAR _inswap	DisplayBuffer[DISPLAYBUFFER_SIZE];




/* Macro to verify that a certain memory range is
 * readable/writable (depending on w). */

#define CheckPointer(p,s,w)	DevHelp_VerifyAccess(FP_SEL(p),(s),FP_OFF(p),(w))





/*#
 * DriverVersion(rp)
 *
 * PARAMETER
 *	request packet
 * RETURNS
 *	driver status word
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Returns version of this driver.  Coded like the MS-DOS
 *	version number.
 * REMARKS
 */
PRIVATE USHORT NEAR
DriverVersion(PRP_GENIOCTL rp)
{
    PUSHORT const	parmsize = &rp->ParmLen;
    PUSHORT const	datasize = &rp->DataLen;
    USHORT const	dsize = (*datasize == 0 ? 0xFFFF : *datasize);
    DSKSL_VER_DATA FAR * const data = (DSKSL_VER_DATA FAR *)rp->DataPacket;

    *datasize = sizeof(DSKSL_VER_DATA);		/* set required values */
    *parmsize = 0;
    if( dsize < sizeof(data->version)
	||  CheckPointer(rp->DataPacket, sizeof(data->version), 1) )
	return STATUS_ERR_INVPARAM;

    data->version = MAKEUSHORT(SUBVERSION,VERSION);
    if( dsize >= sizeof(DSKSL_VER_DATA)
	&&  !CheckPointer(rp->DataPacket, sizeof(DSKSL_VER_DATA), 1) )
    {
	data->flags = 0;
	if( (fDriverFlags & DF_REGISTERED) )
	    data->flags |= 0x01;
    }

    return STATUS_DONE;
}




/*#
 * DeviceList(rp)
 *
 * PARAMETER
 *	rp		request packet
 * RETURNS
 *	driver status word
 * GLOBAL
 *	cUnit, anpUnit
 * DESPRIPTION
 *	Builds list of all devices in user space.
 * REMARKS
 */
PRIVATE USHORT NEAR
DeviceList(PRP_GENIOCTL rp)
{
    PUSHORT const	parmsize = &rp->ParmLen;
    PUSHORT const	datasize = &rp->DataLen;
    USHORT const	dsize = (*datasize == 0 ? 0xFFFF : *datasize);
    DSKSL_QL_DATA FAR * const data = (DSKSL_QL_DATA FAR *)rp->DataPacket;
    USHORT		i;

    *parmsize = 0;
    *datasize = FIELDOFFSET(DSKSL_QL_DATA,list) + sizeof(DEVICE_TIMEOUT) * cUnit;

    if( dsize < FIELDOFFSET(DSKSL_QL_DATA,list)
	||  CheckPointer(data, FIELDOFFSET(DSKSL_QL_DATA,list), 1) )
    {
	return STATUS_ERR_INVPARAM;
    }

    data->reserved[0] = data->reserved[1] = 0;
    data->cb = *datasize;
    for( i = 0; i < cUnit  &&  i < dsize / sizeof(DEVICE_TIMEOUT); ++i )
    {
	DEVICE_TIMEOUT FAR * p = &data->list[i];

	if( CheckPointer(p, sizeof(DEVICE_TIMEOUT), 1) )
	{
	    return STATUS_ERR_INVPARAM;
	}

	p->adapter = (UCHAR)anpUnit[i]->iAdapter;
	p->unit = (UCHAR)anpUnit[i]->iUnit;
	p->reserved[0] = p->reserved[1] = 0;
	p->seconds = anpUnit[i]->usSleep;
    }

    return STATUS_DONE;
}




/*#
 * ChangeList(rp)
 *
 * PARAMETER
 *	rp		request packet
 * RETURNS
 *	driver status word
 * GLOBAL
 *	cUnit, anpUnit
 *
 * DESPRIPTION
 *	Changes one or more timeout values.
 * REMARKS
 */
PRIVATE USHORT NEAR
ChangeList(PRP_GENIOCTL rp)
{
    PUSHORT const	parmsize = &rp->ParmLen;
    PUSHORT const	datasize = &rp->DataLen;
    USHORT const	psize = (*parmsize == 0 ? 0xFFFF : *parmsize);
    DSKSL_SETTO_PARM FAR * const parm = (DSKSL_SETTO_PARM FAR *)rp->ParmPacket;
    USHORT		i, cnt;
    USHORT		rc = STATUS_DONE;

    *datasize = 0;				/* not interested */
    *parmsize = FIELDOFFSET(DSKSL_SETTO_PARM,list);

    if( psize < FIELDOFFSET(DSKSL_SETTO_PARM,list)
       ||  CheckPointer(parm, FIELDOFFSET(DSKSL_SETTO_PARM,list), 0) )
    {
	return STATUS_ERR_INVPARAM;
    }
    if( parm->reserved[0] != 0  ||  parm->reserved[1] != 0 )
	return STATUS_ERR_INVPARAM;
    if( psize < parm->cb  ||  CheckPointer(parm, parm->cb, 0) )
    {
	*parmsize = parm->cb;
	return STATUS_ERR_INVPARAM;
    }

    cnt = (parm->cb - FIELDOFFSET(DSKSL_SETTO_PARM,list))
		/ sizeof(DEVICE_TIMEOUT);

    for( i = 0; i < cnt; ++i )
    {
	DEVICE_TIMEOUT FAR * const p = &parm->list[i];
	USHORT		u;

	if( p->reserved[0] != 0  ||  p->reserved[1] != 0 )
	{
	    rc = STATUS_ERR_INVPARAM;
	    break;
	}

	rc = STATUS_ERR_INVPARAM;
	for( u = 0; u < cUnit; ++u )
	{
	    if( p->adapter == (UCHAR)anpUnit[u]->iAdapter
	       &&  p->unit == (UCHAR)anpUnit[u]->iUnit )
	    {
		/* Change timeout value for this device.  Don't
		 * let timer value exceed this new timeout. */

		cli();
		anpUnit[u]->usSleep = (USHORT)p->seconds;
		if( anpUnit[u]->counter > anpUnit[u]->usSleep )
		    anpUnit[u]->counter = anpUnit[u]->usSleep;
		sti();
		rc = STATUS_DONE;
		break;
	    }
	}
	if( rc != STATUS_DONE )
	    break;
    }

    return rc;
}




/*#
 * QueryDeviceState(rp)
 *
 * PARAMETER
 *	rp		request packet
 * RETURNS
 *	driver status word
 * GLOBAL
 *	cUnit, anpUnit
 * DESPRIPTION
 *	Returns state of a single device
 * REMARKS
 */
PRIVATE USHORT NEAR
QueryDeviceState(PRP_GENIOCTL rp)
{
    PUSHORT const	parmsize = &rp->ParmLen;
    PUSHORT const	datasize = &rp->DataLen;
    USHORT const	psize = (*parmsize == 0 ? 0xFFFF : *parmsize);
    USHORT const	dsize = (*datasize == 0 ? 0xFFFF : *datasize);
    DSKSL_DEVSTATE_PARM FAR * const parm = (DSKSL_DEVSTATE_PARM FAR *)rp->ParmPacket;
    DSKSL_DEVSTATE_DATA FAR * const data = (DSKSL_DEVSTATE_DATA FAR *)rp->DataPacket;
    USHORT		u;
    USHORT		rc = STATUS_ERR_INVPARAM;

    *parmsize = sizeof(DSKSL_DEVSTATE_PARM);
    *datasize = sizeof(DSKSL_DEVSTATE_DATA);

    if( psize < sizeof(DSKSL_DEVSTATE_PARM)
       ||  CheckPointer(parm, sizeof(DSKSL_DEVSTATE_PARM), 0) )
    {
	return rc;
    }
    if( dsize < sizeof(data->seconds)  ||  CheckPointer(data, dsize, 1) )
    {
	return rc;
    }

    for( u = 0; u < cUnit; ++u )
    {
	if( parm->adapter == (UCHAR)anpUnit[u]->iAdapter
	   &&  parm->unit == (UCHAR)anpUnit[u]->iUnit )
	{
	    if( anpUnit[u]->usSleep == 0 )
		data->seconds = (USHORT)-1;	/* never stopped */
	    else
		data->seconds = anpUnit[u]->usSleep - anpUnit[u]->counter;

	    if( dsize >= sizeof(DSKSL_DEVSTATE_DATA) ) /* room for complete struc? */
		_fmemcpy(data->restart, anpUnit[u]->rst_reason,
			 min(dsize, sizeof(anpUnit[u]->rst_reason)));
	    rc = STATUS_DONE;
	    break;
	}
    }

    return rc;
}




/*#
 * SetDeviceState(rp)
 *
 * PARAMETER
 *	rp		request packet
 * RETURNS
 *	driver status word
 * GLOBAL
 *	cUnit, anpUnit
 * DESPRIPTION
 *	Changes state of a single device
 * REMARKS
 */
PRIVATE USHORT NEAR
SetDeviceState(PRP_GENIOCTL rp)
{
    PUSHORT const	parmsize = &rp->ParmLen;
    PUSHORT const	datasize = &rp->DataLen;
    USHORT const	psize = (*parmsize == 0 ? 0xFFFF : *parmsize);
    USHORT const	dsize = (*datasize == 0 ? 0xFFFF : *datasize);
    DSKSL_DEVSTATE_PARM FAR * const parm = (DSKSL_DEVSTATE_PARM FAR *)rp->ParmPacket;
    DSKSL_DEVSTATE_DATA FAR * const data = (DSKSL_DEVSTATE_DATA FAR *)rp->DataPacket;
    USHORT		u;
    USHORT		rc = STATUS_ERR_INVPARAM;

    *parmsize = sizeof(DSKSL_DEVSTATE_PARM);
    *datasize = sizeof(DSKSL_DEVSTATE_DATA);

    if( psize < sizeof(DSKSL_DEVSTATE_PARM)
       ||  CheckPointer(parm, sizeof(DSKSL_DEVSTATE_PARM), 0) )
    {
	return rc;
    }
    if( dsize < sizeof(DSKSL_DEVSTATE_DATA)
       ||  CheckPointer(data, sizeof(DSKSL_DEVSTATE_DATA), 1) )
    {
	return rc;
    }

    for( u = 0; u < cUnit; ++u )
    {
	if( parm->adapter == (UCHAR)anpUnit[u]->iAdapter
	   &&  parm->unit == (UCHAR)anpUnit[u]->iUnit )
	{
	    ULONG const	ul = data->seconds;

	    if( ul >= anpUnit[u]->usSleep )
		anpUnit[u]->counter = 0;
	    else
		anpUnit[u]->counter = (USHORT)(anpUnit[u]->usSleep - ul);
	    rc = STATUS_DONE;
	    break;
	}
    }

    return rc;
}




/*#
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
 * REMARKS
 */
#pragma optimize("",off)			/* keep optimizer quiet */
PRIVATE USHORT NEAR
ReadMessages(PRP_GENIOCTL rp)
{
    PUSHORT const	datasize = &rp->DataLen;
    USHORT		dsize = (*datasize == 0 ? 0xFFFF : *datasize);
    DSKSL_MSGS_DATA FAR * const data = (DSKSL_MSGS_DATA FAR *)rp->DataPacket;
    PUSHORT		pcbMsg = &iNextString;
    PUCHAR		pMsg = DisplayBuffer;
    USHORT		rc = STATUS_ERR_INVPARAM;

    *datasize = *pcbMsg + FIELDOFFSET(DSKSL_MSGS_DATA,msg);

    if( dsize < FIELDOFFSET(DSKSL_MSGS_DATA,msg)
       ||  CheckPointer(data, FIELDOFFSET(DSKSL_MSGS_DATA,msg), 1) )
	return rc;
    data->cb = *datasize;			/* tell caller */

    if( dsize > *pcbMsg + FIELDOFFSET(DSKSL_MSGS_DATA,msg) )
	dsize = *pcbMsg + FIELDOFFSET(DSKSL_MSGS_DATA,msg);
    if( CheckPointer(data, dsize, 1) )
	return rc;

    /* If some bytes available in caller buffer (more than
     * just 'cb'), copy part/all of message buffer. */

    if( dsize - FIELDOFFSET(DSKSL_MSGS_DATA,msg) > 0 )
    {
	_fmemcpy(data->msg, pMsg, dsize - FIELDOFFSET(DSKSL_MSGS_DATA,msg));
    }

    return STATUS_DONE;
}
#pragma optimize("",)




/*#
 * ClearMessages(rp)
 *
 * PARAMETER
 *	rp		request packet
 * RETURNS
 *	driver status word
 * GLOBAL
 *	iNextString
 * DESPRIPTION
 *	Removes messages from DisplayBuffer.
 * REMARKS
 */
#pragma optimize("",off)			/* keep optimizer quiet */
PRIVATE USHORT NEAR
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




/*#
 * GetLaststatus(rp)
 *
 * PARAMETER
 *	rp		request packet
 * RETURNS
 *	driver status word
 * GLOBAL
 *	(none)
 * DESPRIPTION
 *	Copies status info from device structure to user buffer.
 * REMARKS
 */
#pragma optimize("",off)			/* keep optimizer quiet */
PRIVATE USHORT NEAR
GetLaststatus(PRP_GENIOCTL rp)
{
    PUSHORT const	parmsize = &rp->ParmLen;
    PUSHORT const	datasize = &rp->DataLen;
    USHORT const	psize = (*parmsize == 0 ? 0xFFFF : *parmsize);
    USHORT		dsize = (*datasize == 0 ? 0xFFFF : *datasize);
    DSKSL_STATUS_DATA FAR * const data = (DSKSL_STATUS_DATA FAR *)rp->DataPacket;
    DSKSL_DEVSTATE_PARM FAR * const parm = (DSKSL_DEVSTATE_PARM FAR *)rp->ParmPacket;
    USHORT		u;
    USHORT		rc = STATUS_ERR_INVPARAM;

    *parmsize = sizeof(DSKSL_DEVSTATE_PARM);
    *datasize = sizeof(DSKSL_STATUS_DATA);

    if( psize < sizeof(DSKSL_DEVSTATE_PARM)
       ||  CheckPointer(parm, sizeof(DSKSL_DEVSTATE_PARM), 0) )
    {
	return rc;
    }
    if( dsize < sizeof(DSKSL_STATUS_DATA)
	||  CheckPointer(data, sizeof(DSKSL_STATUS_DATA), 1) )
	return rc;

    for( u = 0; u < cUnit; ++u )
    {
	if( parm->adapter == (UCHAR)anpUnit[u]->iAdapter
	   &&  parm->unit == (UCHAR)anpUnit[u]->iUnit )
	{
	    data->status = anpUnit[u]->laststatus;
	    data->error = anpUnit[u]->lasterror;
	    *(PSCSI_STATUS_BLOCK)data->statusblock = anpUnit[u]->statusblock;
	    *(PSCSI_REQSENSE_DATA)data->sensedata = anpUnit[u]->sensedata;
	    rc = STATUS_DONE;
	    break;
	}
    }

    return rc;
}






/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */

/*#
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
 * REMARKS
 */
#pragma optimize("",off)
PUBLIC void NEAR
SaveMessage(void)
{
    PUSHORT	pi = &iNextString;

    cli();
    if( *pi+_fstrlen(szMsgBuffer) < DISPLAYBUFFER_SIZE )
    {
	_fstrcpy(&DisplayBuffer[*pi], szMsgBuffer);
	*pi += _fstrlen(szMsgBuffer) + 1;
    }
    sti();
    DEBMSG(szMsgBuffer);
    if( !(fDriverFlags & DF_INITDONE) )
	DevHelp_Save_Message((NPBYTE)&msgBuffer);
    return;
}
#pragma optimize("",)




/*#
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
PUBLIC USHORT
GenIOCtl(PRP_GENIOCTL rp)
{
    USHORT rc = STATUS_ERR_UNKCMD;

    if( rp->Category == IOCTL_DSKSLEEP_CATEGORY )
    {
	switch( rp->Function )
	{
	  case DSKSL_QUERY_VERSION:
	    rc = DriverVersion( rp );
	    break;

	  case DSKSL_QUERY_TIMEOUT:
	    rc = DeviceList( rp );
	    break;

	  case DSKSL_SET_TIMEOUT:
	    rc = ChangeList( rp );
	    break;

	  case DSKSL_QUERY_DEVSTATE:
	    rc = QueryDeviceState( rp );
	    break;

	  case DSKSL_SET_DEVSTATE:
	    rc = SetDeviceState( rp );
	    break;

	  case DSKSL_READ_MSGS:
	    rc = ReadMessages( rp );
	    break;

	  case DSKSL_CLEAR_MSGS:
	    rc = ClearMessages( rp );
	    break;

	  case DSKSL_GET_LASTSTATUS:
	    rc = GetLaststatus(rp);
	    break;
	}
    }

    return rc;
}
