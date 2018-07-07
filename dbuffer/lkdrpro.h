/*static char *SCCSID = "src/dev/dasd/lockdrv/lkdrpro.h, lockdrv, ddk_subset, kira_r206.035 93/03/21";*/
/**************************************************************************
 *
 * SOURCE FILE NAME = LKDRPRO.H
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
 * DESCRIPTION : C function prototypes
 *
 *
 *
 ***************************************************************************/





typedef void   near *NPVOID;
typedef USHORT near *NPUSHORT;

/*----------------------*/
/* Module: LKDRSTR1.C   */
/*----------------------*/

VOID   NEAR LKDRStr1();
VOID   NEAR StatusError(PRPH pRPH, USHORT ErrorCode );
USHORT NEAR LKDRUnlock( PRPH pRPH );
USHORT NEAR SendIORB( PIORB pIORB, VOID (FAR *pADDEntry)(PIORB) );
VOID   FAR _loadds  SendIORBNotify( PIORB pIORB );

/*----------------------*/
/* Module: LKDRINIT.C   */
/*----------------------*/

USHORT NEAR LKDRInit(PRPINITIN pRPI);


