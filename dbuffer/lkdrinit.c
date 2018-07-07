/*PRIVATE char *SCCSID = "src/dev/dasd/lockdrv/lkdrinit.c, lockdrv, ddk_subset, kira_r206.035 93/03/21";*/

/**************************************************************************
 *
 * SOURCE FILE NAME = LKDRINIT.C
 *
 * DESCRIPTIVE NAME = LOCKDRV.FLT - Convert Removable DASD to Fixed Drive
 *                    Initialization Routines
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
 * DESCRIPTION :  Builds configuration dependent data in response to OS/2
 *                KERNEL initialization request.
 *
 *
 ***************************************************************************/

#include <string.h>

#define INCL_NOBASEAPI
#define INCL_NOPMAPI
#include <os2.h>

#include <devcmd.h>
#include <devclass.h>
#include <dskinit.h>

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




PRIVATE PSZ      pCmd;


PRIVATE void
Code_End(void) {}





PRIVATE void
SkipToken(void)
{
    while( *pCmd  &&  *pCmd != ' '  &&  *pCmd != ','  &&  *pCmd != '/' )
	++pCmd;
}




PRIVATE UCHAR
toupper( UCHAR c )
{
    return (c >= 'a' && c <= 'z' ? ((c - (UCHAR)'a') + (UCHAR)'A') : c);
}




PRIVATE void
UpperCase(void)
{
    PSZ s = pCmd;

    while( (*pCmd=toupper(*pCmd)) )
	++pCmd;
    pCmd = s;
}




PRIVATE USHORT
AtoI(void)
{
    USHORT	i, n;

    n = 0;

    for( i = 0; i < 2; ++i )
    {
	if( *pCmd >= '0'  &&  *pCmd <= '9' )
        {
	    n  = n * 10;
	    n += (*pCmd++) - '0';
        }
	else
        {
	    break;
        }
    }

    return (i ? n : -1);
}




PRIVATE void
SkipBlanks(void)
{
    while( *pCmd  &&  *pCmd == ' ' )
	++pCmd;
}




PRIVATE USHORT
NextUnitNum(void)
{
    if (*pCmd == ',' )
    {
	pCmd++;
	return( 1 );
    }
    else
    {
	return( 0 );
    }
}




/*--------------------------------------------------*/
/*                                                  */
/* Parse Command Line Parameters                    */
/* -----------------------------                    */
/*                                                  */
/*                                                  */
/* BASEDEV=LOCKDRV.FLT /UNIT:n0,n1,....             */
/*                                                  */
/* Where:                                           */
/*   n0,n2,..... are list of removable unit numbers */
/*               starting at 0.                     */
/*                                                  */
/*               CDROM and A:, B: drives are not    */
/*               included in this count.            */
/*                                                  */
/*--------------------------------------------------*/
PRIVATE USHORT
ParseCmdLine(void)
{
    USHORT Rc = -1;
    USHORT UnitId;

    UpperCase();


    /*------------------------*/
    /* Skip LOCKDRV.FLT token */
    /*------------------------*/
    SkipToken();

    SkipBlanks();

    /*------------------------*/
    /* Check for /UNIT: token */
    /*------------------------*/

    if( *pCmd )
    {
	if( _fstrcmp(pCmd,"/UNIT:") || _fstrcmp(pCmd, "/UNIT=" ))
        {
	    pCmd += sizeof("/UNIT:") - 1;

	    /*--------------------------*/
	    /* Process list of UNIT Ids */
	    /*--------------------------*/

	    while( UnitListCount < MAX_UNITS )
            {
		if( (UnitId=AtoI()) != -1 )
                {
		    UnitList[UnitListCount++] = UnitId;

		    if( !NextUnitNum() )
                    {
			break;
                    }
                }
		else
                {
		    break;
                }
            }
	    SkipBlanks();

	    /*-----------------------------------------*/
	    /* We should be a the end of the line now, */
	    /* with at least one unit specified        */
	    /*-----------------------------------------*/

	    if( !*pCmd  &&  UnitListCount )
		Rc = 0;
        }
    }
    return Rc;
}






/* **********************************************************************
 * **********************************************************************
 * ******************************************************************* */


/*--------------------------------------------------*/
/*                                                  */
/* Process Base Initialization Request Packet       */
/* ------------------------------------------       */
/*                                                  */
/*                                                  */
/*--------------------------------------------------*/
PUBLIC USHORT
LKDRInit(PRPINITIN pRPI)
{
    USHORT               i,j,k,l;
    USHORT               Rc;
    PRPINITOUT           pRPO = (PRPINITOUT) pRPI;

    USHORT               NumADDs;
    USHORT               AddIndx;
    USHORT               NumUnits;
    USHORT               NumAdapt;
    USHORT               UnitState;
    USHORT               UnitFlags;
    USHORT               UnitType;
    USHORT               UnitId;
    USHORT               FilterADDHandle;

    PIORB_CONFIGURATION  pIOCF = (PIORB_CONFIGURATION)  LkdrIORB;
    PIORB_UNIT_CONTROL   pIOUC = (PIORB_UNIT_CONTROL)   LkdrIORB;
    PIORB_DEVICE_CONTROL pIODC = (PIORB_DEVICE_CONTROL) LkdrIORB;
    PIORB_UNIT_STATUS    pIOUS = (PIORB_UNIT_STATUS)    LkdrIORB;
    PDEVICETABLE         pDT;

    NPBYTE               npCFGData;

    NPADAPTERINFO        npAI;
    NPUNITINFO           npUI, npModUI;
    NPUNITENTRY          npUE = UnitEntryTable;

    void                 (FAR *pADDEntry)();
    void                 (FAR *pADDBaseEP)();

    struct DevClassTableEntry FAR *pDCE, FAR *pDCEFilter;
    struct DevClassTableStruc FAR *pDCT;

    InitComplete = 1;

    Device_Help = pRPI->DevHlpEP;

    /*--------------------------*/
    /* Parse Command Line Args  */
    /*--------------------------*/

    pCmd           = pRPI->InitArgs;
    OFFSETOF(pCmd) = (USHORT) ((PDDD_PARM_LIST) pCmd)->cmd_line_args;

    if ( ParseCmdLine() )
    {
	goto LCKDRV_Failed;
    }

    /*-------------------------------------*/
    /* Get address of Registered ADD Table */
    /*-------------------------------------*/

    if ( DevHelp_GetDOSVar( DHGETDOSV_DEVICECLASSTABLE, DRIVERCLASS_ADD, &pDCT ) )
    {
	goto LCKDRV_Failed;
    }

    NumADDs = pDCT->DCCount;

    UnitEntryCount = 0;

    /*--------------------------*/
    /* For each ADD Registered  */
    /*--------------------------*/

    for (pDCE=pDCT->DCTableEntries, AddIndx=0;
	 AddIndx < NumADDs;
	 AddIndx++, pDCE++                       )
    {

	/*------------------------*/
	/* Get ADD's Device Table */
	/*------------------------*/

	pIOCF->iorbh.Length          = sizeof(IORB_CONFIGURATION);
	pIOCF->iorbh.UnitHandle      = 0;
	pIOCF->iorbh.CommandCode     = IOCC_CONFIGURATION;
	pIOCF->iorbh.CommandModifier = IOCM_GET_DEVICE_TABLE;
	pIOCF->iorbh.RequestControl  = IORB_ASYNC_POST;

	pIOCF->pDeviceTable          = (PDEVICETABLE) DeviceTable;
	pIOCF->DeviceTableLen        = sizeof(DeviceTable);

	(PVOID) pADDBaseEP = (PVOID) MAKEP(pDCE->DCSelector, pDCE->DCOffset);

	if ( SendIORB((PIORB) pIOCF, pADDBaseEP) )
        {
	    continue;
        }

	/*----------------------------------------*/
	/* For each Adapter in ADD's Device Table */
	/*----------------------------------------*/

	pDT      = (PDEVICETABLE) DeviceTable;

	NumAdapt = pDT->TotalAdapters;

	UnitId   = -1;

	for (j=0; j < NumAdapt; j++ )
        {
	    npAI = pDT->pAdapter[j];

	    NumUnits = npAI->AdapterUnits;

	    /*--------------------------------------------*/
	    /* Access UNITINFO for each Device on adapter */
	    /*--------------------------------------------*/

	    for ( npUI=npAI->UnitInfo,k=0; k < NumUnits; k++, npUI++ )
            {
		UnitFlags = npUI->UnitFlags;
		UnitType  = npUI->UnitType;
		UnitState = 0;

		/*------------------------------------------------*/
		/* If a Filter ADD has registered for this unit   */
		/*   Then                                         */
		/*     send subsequent requests to the Filter ADD */
		/*------------------------------------------------*/

		if ( FilterADDHandle = npUI->FilterADDHandle )
                {
		    pDCEFilter = &pDCT->DCTableEntries[FilterADDHandle-1];

		    (PVOID) pADDEntry = (PVOID) MAKEP(pDCEFilter->DCSelector,
						      pDCEFilter->DCOffset    );
                }
		else
                {
		    pADDEntry  = pADDBaseEP;
                }

		/*--------------------------------------------------------*/
		/* If this unit is a removable magnetic or optical device */
		/*   Then                                                 */
		/*     include device in numbering scheme                 */
		/*--------------------------------------------------------*/

		if ( (UnitType == UIB_TYPE_DISK ||
		      UnitType == UIB_TYPE_OPTICAL_MEMORY)  &&
		    (UnitFlags & UF_REMOVABLE)             &&
		    !(UnitFlags & (UF_A_DRIVE | UF_B_DRIVE)    ))
                {
		    UnitId++;

		    /*--------------------------------------------------*/
		    /* If this device number appears in the /UNIT: list */
		    /*   Then                                           */
		    /*      Start LOCKDRIVE processing on this device   */
		    /*--------------------------------------------------*/

		    for (  i=0;
			 (i < UnitListCount) && (UnitList[i] != UnitId);
                         i++ )
			;
		    if ( i == UnitListCount )
                    {
			continue;
                    }

		    /*--------------------------------------------------------*/
		    /* If the ADD command line indicates NO DASD/SCSI support */
		    /*   Then                                                 */
		    /*      Skip the unit                                     */
		    /*--------------------------------------------------------*/

		    if ( UnitFlags & (UF_DEFECTIVE    |
				      UF_NODASD_SUPT  |
				      UF_NOSCSI_SUPT   )  )
                    {
			continue;
                    }

		    /*--------------------------------*/
		    /* Allocate the unit to be LOCKED */
		    /*--------------------------------*/

		    pIOUC->iorbh.Length          = sizeof(IORB_UNIT_CONTROL);
		    pIOUC->iorbh.UnitHandle      = npUI->UnitHandle;
		    pIOUC->iorbh.CommandCode     = IOCC_UNIT_CONTROL;
		    pIOUC->iorbh.CommandModifier = IOCM_ALLOCATE_UNIT;

		    if ( SendIORB((PIORB) pIOUC, pADDEntry) )
                    {
			continue;
                    }

		    UnitState |= UNIT_ALLOCATED;

		    /*-----------------------------------------*/
		    /* Clear any pending SCSI CHECK CONDITIONS */
		    /* and that unit is READY                  */
		    /*-----------------------------------------*/

		    pIOUS->iorbh.Length          = sizeof(IORB_UNIT_STATUS);
		    pIOUS->iorbh.UnitHandle      = npUI->UnitHandle;
		    pIOUS->iorbh.CommandCode     = IOCC_UNIT_STATUS;
		    pIOUS->iorbh.CommandModifier = IOCM_GET_UNIT_STATUS;
		    pIOUS->UnitStatus            = 0;

		    for (l=0; l < 5 ; l++ )
                    {
			Rc = SendIORB((PIORB) pIOUS, pADDEntry);
                    }

		    if (Rc || !(pIOUS->UnitStatus & US_READY) )
                    {
			goto Unit_Error;
                    }

		    /*-----------------------------------------------*/
		    /* Send request to unit to DISABLE MEDIA REMOVAL */
		    /*-----------------------------------------------*/

		    pIODC->iorbh.Length          = sizeof(IORB_DEVICE_CONTROL);
		    pIODC->iorbh.UnitHandle      = npUI->UnitHandle;
		    pIODC->iorbh.CommandCode     = IOCC_DEVICE_CONTROL;
		    pIODC->iorbh.CommandModifier = IOCM_LOCK_MEDIA;
		    pIODC->Flags                 = 0;

		    if ( SendIORB((PIORB) pIODC, pADDEntry) )
                    {
			goto Unit_Error;
                    }

		    UnitState |= UNIT_LOCKED;

		    /*---------------------------------*/
		    /* Send request to CHANGE UNITINFO */
		    /*---------------------------------*/

		    npModUI  = &npUE->NewUnitInfo;

		    _asm { cld }
		    *npModUI = *npUI;

		    npModUI->UnitFlags &= ~UF_REMOVABLE;
		    npModUI->UnitType   = UIB_TYPE_DISK;

		    pIOUC->iorbh.Length          = sizeof(IORB_UNIT_CONTROL);
		    pIOUC->iorbh.UnitHandle      = npUI->UnitHandle;
		    pIOUC->iorbh.CommandCode     = IOCC_UNIT_CONTROL;
		    pIOUC->iorbh.CommandModifier = IOCM_CHANGE_UNITINFO;
		    pIOUC->Flags                 = 0;
		    pIOUC->pUnitInfo             = npModUI;
		    pIOUC->UnitInfoLen           = sizeof(UNITINFO);

		    if ( SendIORB((PIORB) pIOUC, pADDEntry) )
                    {
			goto Unit_Error;
                    }

		    UnitState |= UNIT_INFOCHANGED;

		    npCFGData += sizeof(UNITINFO);

		    /*---------------------------------*/
		    /* Send request to DEALLOCATE UNIT */
		    /*---------------------------------*/

		    pIOUC->iorbh.Length          = sizeof(IORB_UNIT_CONTROL);
		    pIOUC->iorbh.UnitHandle      = npUI->UnitHandle;
		    pIOUC->iorbh.CommandCode     = IOCC_UNIT_CONTROL;
		    pIOUC->iorbh.CommandModifier = IOCM_DEALLOCATE_UNIT;

		    if ( SendIORB((PIORB) pIOUC, pADDEntry) )
                    {
			continue;
                    }

		    UnitState &= ~UNIT_ALLOCATED;

		    npUE->pADDEntry  = pADDEntry;
		    npUE->UnitHandle = npUI->UnitHandle;

		    npUE++;
		    UnitEntryCount++;

		    continue;

		    /*-------------------------------------------------*/
		    /* If an error occurrs in one of the above steps   */
		    /*   Then                                          */
		    /*     UNLOCK/DEALLOCATE the unit                  */
		    /*-------------------------------------------------*/
		  Unit_Error:
		    if ( UnitState & UNIT_LOCKED )
                    {
			pIODC->iorbh.Length          = sizeof(IORB_DEVICE_CONTROL);
			pIODC->iorbh.UnitHandle      = npUI->UnitHandle;
			pIODC->iorbh.CommandCode     = IOCC_DEVICE_CONTROL;
			pIODC->iorbh.CommandModifier = IOCM_UNLOCK_MEDIA;
			pIODC->Flags                 = 0;

			SendIORB((PIORB) pIODC, pADDEntry);

			UnitState &= ~UNIT_LOCKED;
		    }

		    if ( UnitState & UNIT_ALLOCATED )
                    {
			pIOUC->iorbh.Length          = sizeof(IORB_UNIT_CONTROL);
			pIOUC->iorbh.UnitHandle      = npUI->UnitHandle;
			pIOUC->iorbh.CommandCode     = IOCC_UNIT_CONTROL;
			pIOUC->iorbh.CommandModifier = IOCM_DEALLOCATE_UNIT;

			SendIORB((PIORB) pIOUC, pADDEntry);
                    }

                }
            }
        }
    }

    if ( UnitEntryCount != UnitListCount )
    {
        DevHelp_Save_Message( (NPBYTE) &LockDrvWngMsg );
    }

    /*-----------------------------------------------*/
    /* We do not need to register, since this filter */
    /* does not 'own' any units                      */
    /*-----------------------------------------------*/


    pRPO->Unit    = 0;
    pRPO->CodeEnd = (USHORT) Code_End;
    pRPO->DataEnd = (USHORT) npUE;

    return 0x0100;

  LCKDRV_Failed:
    pRPO->Unit    = 0;
    pRPO->CodeEnd = 0;
    pRPO->DataEnd = 0;

    return 0x810C;
}
