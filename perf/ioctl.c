/*
 * $Source: e:/source/driver/perf/RCS/ioctl.c,v $
 * $Revision: 1.4 $
 * $Date: 1997/12/16 02:24:16 $
 * $Locker:  $
 *
 * General IOCtl interface
 *
 * $Log: ioctl.c,v $
 * Revision 1.4  1997/12/16 02:24:16  vitus
 * - renamed source files
 *
 * Revision 1.3  1997/12/11 01:52:47  vitus
 * - implemented DSKPF_READ_MSGS, DSKPF_CLEAR_MSGS IOCtls
 * - made DSKPF_QUERY_VERSION compatible to DSKSleep
 *
 * Revision 1.2  1997/12/06 01:01:16  vitus
 * - enabled support for WatCom C
 *
 * Revision 1.1  1997/10/28 02:09:49  vitus
 * Initial revision
 * ----------------------------------------
 * Author: Vitus Jensen, 2:2474/424, 1997
 */
#pragma off (unreferenced)
static char vcid[]="$Id: ioctl.c,v 1.4 1997/12/16 02:24:16 vitus Exp $";
#pragma on (unreferenced)

#include <string.h>

#define INCL_NOBASEAPI
#define INCL_DOSERRORS
#define INCL_NOPMAPI
#include <os2.h>
#include <bseerr.h>

#include <devcmd.h>
#include <strat2.h>
#include <reqpkt.h>

#include <iorb.h>
#include <addcalls.h>
#if defined(__WATCOMC__)
# include <devhelp.h>
typedef USHORT NEAR *	NPUSHORT;
typedef VOID NEAR *	NPVOID;
#else
# include <dhcalls.h>
#endif

#include "scsiopt.h"
#include "proto.h"
#include "extern.h"

#include "scsioptpub.h"




/* Save messages here.  This buffer is located in a swapable
 * data segment and may only access at task time!
 * OBS: M$-C tends to allocate a const containing the
 * segment of these variables.  To prevent this use strange code
 * and no optimization at all. */

#define _inswap		_based(_segname("SWAPDATA"))
PRIVATE USHORT _inswap	iNextString = 0;
PRIVATE UCHAR _inswap	DisplayBuffer[DISPLAYBUFFER_SIZE];



/* Macro to verify that a certain memory range is
 * readable/writable (depending on w). */

#define CheckPointer(p,s,w)	DevHelp_VerifyAccess(FP_SEL(p),(s),FP_OFF(p),(w))






/*#
 * NAME
 *	DriverVersion
 * CALL
 *	DriverVersion(rp)
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
    PUSHORT const	datasize = &rp->DataLen;
    USHORT const	dsize = (*datasize == 0 ? 0xFFFF : *datasize);
    PDSKPF_VER_DATA const data = (PDSKPF_VER_DATA)rp->DataPacket;

    if( dsize < sizeof(DSKPF_VER_DATA)
	||  CheckPointer(rp->DataPacket, sizeof(DSKPF_VER_DATA), 1) )
	return STATUS_ERR_INVPARAM;

    data->version = MAKEUSHORT(SUBVERSION,VERSION);
    data->flags = 0;
    return STATUS_DONE;
}




/*#
 * NAME
 *	ReadMessages
 * CALL
 *	ReadMessages(rp)
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
    PDSKPF_MSGS_DATA const data = (PDSKPF_MSGS_DATA)rp->DataPacket;
    PUSHORT		pcbMsg = &iNextString;
    PUCHAR		pMsg = DisplayBuffer;
    USHORT		rc = STATUS_ERR_INVPARAM;

    *datasize = *pcbMsg + FIELDOFFSET(DSKPF_MSGS_DATA,msg);

    if( dsize < FIELDOFFSET(DSKPF_MSGS_DATA,msg)
	||  CheckPointer(data, FIELDOFFSET(DSKPF_MSGS_DATA,msg), 1) )
	return rc;
    data->cb = *datasize;			/* tell caller */

    if( dsize > *pcbMsg + FIELDOFFSET(DSKPF_MSGS_DATA,msg) )
	dsize = *pcbMsg + FIELDOFFSET(DSKPF_MSGS_DATA,msg);
    if( CheckPointer(data, dsize, 1) )
	return rc;

    /* If some bytes available in caller buffer (more than
     * just 'cb'), copy part/all of message buffer. */

    if( dsize - FIELDOFFSET(DSKPF_MSGS_DATA,msg) > 0 )
    {
	_fmemcpy(data->msg, pMsg, dsize - FIELDOFFSET(DSKPF_MSGS_DATA,msg));
    }

    return STATUS_DONE;
}
#pragma optimize("",)




/*#
 * NAME
 *	ClearMessages
 * CALL
 *	ClearMessages(rp)
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






/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */

/*#
 * NAME
 *	SaveMessage
 * CALL
 *	SaveMessage(void)
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

    DISABLE();
    if( *pi+_fstrlen(szMsgBuffer) < DISPLAYBUFFER_SIZE )
    {
	_fstrcpy(&DisplayBuffer[*pi], szMsgBuffer);
	*pi += _fstrlen(szMsgBuffer) + 1;
    }
    ENABLE();
    DEBMSG1("\r\n%s",(PSZ)szMsgBuffer);
    if( !(fDriverFlags & DF_INITDONE) )
	DevHelp_Save_Message((NPBYTE)&msgBuffer);
    return;
}
#pragma optimize("",)




/*#
 * NAME
 *	GenIOCtl
 * CALL
 *	GenIOCtl(rp)
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
    USHORT rc;

    if( rp->Category != IOCTL_DSKPERF_CATEGORY )
    {
	return STATUS_ERR_UNKCMD;
    }

    switch( rp->Function )
    {
      case DSKPF_QUERY_VERSION:
	rc = DriverVersion( rp );
	break;

      case DSKPF_READ_MSGS:
	rc = ReadMessages( rp );
	break;

      case DSKPF_CLEAR_MSGS:
	rc = ClearMessages( rp );
	break;

      default:
	rc = STATUS_ERR_UNKCMD;
	break;
    }

    return rc;
}
