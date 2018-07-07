/**************************************************************************
 *
 * SOURCE FILE NAME = LKDRSTR1.C
 *
 * DESCRIPTIVE NAME = LOCKDRV.FLT - Convert Removable DASD to Fixed Drive
 *
 *
 * 
 * 
 * COPYRIGHT    Copyright (C) 1992 IBM Corporation
 * 
 * The following IBM OS/2 2.1 source code is provided to you solely for
 * the purpose of assisting you in your development of OS/2 2.x device
 * drivers. You may use this code in accordance with the IBM License
 * Agreement provided in the IBM Device Driver Source Kit for OS/2. This
 * Copyright statement may not be removed.
 * 
 * 
 *
 * VERSION = V2.0
 *
 * DATE
 *
 * DESCRIPTION : Strategy 1 Entry Point
 *
 *
 *
 ***************************************************************************/




#define INCL_NOBASEAPI
#define INCL_NOPMAPI
#include <os2.h>

#include <devcmd.h>

#define INCL_INITRP_ONLY
#include <reqpkt.h>

#include <scb.h>
#include <abios.h>

#include <iorb.h>
#include <addcalls.h>

#include <dhcalls.h>

#include "lkdrcons.h"
#include "lkdrpro.h"
#include "lkdrextn.h"



extern USHORT InitComplete;



void NEAR
LKDRStr1()
{
    PRPH	pRPH;
    USHORT	Cmd;

    _asm
    {
	mov word ptr pRPH[0], bx	;
	mov word ptr pRPH[2], es	;
    }

    Cmd = pRPH->Cmd;
    if( (Cmd==CMDInitBase)  &&  !InitComplete )
    {
	pRPH->Status = LKDRInit( (PRPINITIN) pRPH );
    }
    else if( Cmd == CMDShutdown )
    {
	pRPH->Status = LKDRUnlock( (PRPH) pRPH );
    }
    else if( (Cmd == CMDOpen) || (Cmd == CMDClose) )
    {                                                               
	pRPH->Status |= STATUS_DONE;                                  
    }                                                               
    else
    {
	StatusError( pRPH, STATUS_ERR_UNKCMD );
    }

    _asm
    {
	leave				;
	retf				;
    }
}


/*---------------------------------------------------------------------------*/
/* Strategy 1 Error Processing                                               */
/* ---------------------------                                               */
/*                                                                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/

VOID NEAR
StatusError(PRPH pRPH,USHORT ErrorCode)
{
    pRPH->Status |= ErrorCode;
    pRPH->Status |= STATUS_DONE;                                      
    return;
}




/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/


USHORT
LKDRUnlock(PRPH pRPH)
{
    USHORT        i;

    PIORB_UNIT_CONTROL	 pIOUC = (PIORB_UNIT_CONTROL)LkdrIORB;
    PIORB_DEVICE_CONTROL pIODC = (PIORB_DEVICE_CONTROL)LkdrIORB;
    NPUNITENTRY		 npUE = (NPUNITENTRY)UnitEntryTable;

    for (i=0; i < UnitEntryCount; i++, npUE++ )
    {
	pIOUC->iorbh.Length          = sizeof(IORB_UNIT_CONTROL);
	pIOUC->iorbh.UnitHandle      = npUE->UnitHandle;
	pIOUC->iorbh.CommandCode     = IOCC_UNIT_CONTROL;
	pIOUC->iorbh.CommandModifier = IOCM_ALLOCATE_UNIT;

	SendIORB((PIORB) pIOUC, npUE->pADDEntry);

	pIODC->iorbh.Length          = sizeof(IORB_DEVICE_CONTROL);
	pIODC->iorbh.UnitHandle      = npUE->UnitHandle;
	pIODC->iorbh.CommandCode     = IOCC_DEVICE_CONTROL;
	pIODC->iorbh.CommandModifier = IOCM_UNLOCK_MEDIA;
	pIODC->Flags                 = 0;

	SendIORB((PIORB) pIODC, npUE->pADDEntry);

    }
    return ( 0x0100 );
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/


USHORT
SendIORB(PIORB pIORB, VOID (FAR *pADDEntry)(PIORB))
{
    pIORB->NotifyAddress  = SendIORBNotify;
    pIORB->RequestControl = IORB_ASYNC_POST;
    pIORB->ErrorCode      = 0;
    pIORB->Status         = 0;

    (*pADDEntry)( pIORB );

    DISABLE

	while ( !(pIORB->Status & IORB_DONE) )
	{
	    DevHelp_ProcBlock( (ULONG) pIORB, -1L, WAIT_IS_NOT_INTERRUPTABLE );
	    DISABLE
	}

    ENABLE

	return pIORB->ErrorCode;
}




VOID FAR _loadds
SendIORBNotify( PIORB pIORB )
{
    USHORT AwakeCount;

    DevHelp_ProcRun( (ULONG) pIORB, &AwakeCount );
}
