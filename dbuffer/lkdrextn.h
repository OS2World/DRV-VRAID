/*static char *SCCSID = "src/dev/dasd/lockdrv/lkdrextn.h, lockdrv, ddk_subset, kira_r206.035 93/03/21";*/
/**************************************************************************
 *
 * SOURCE FILE NAME = LKDREXTN.H
 *
 * DESCRIPTIVE NAME = LOCKDRV.FLT - Convert Removable DASD to Fixed Drive
 *                    Static/Initialization Data Externs
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
 * DESCRIPTION : Maps internal data used by this ADD in LKDRDATA.C
 *
 *               When editing this file, try to keep the text aligned with
 *               LKDRDATA.C. This will make tracking EXTERNS for data
 *               items a lot easier!
 *
 *
 ***************************************************************************/




/*--------------*/
/* Static Data  */
/*--------------*/

extern PFN     Device_Help;
extern USHORT  InitComplete;
extern BYTE    LkdrIORB[MAX_IORB_SIZE];
extern USHORT  UnitListCount;
extern USHORT  UnitList[MAX_UNITS];



/*--------------------*/
/* Configuration Data */
/*--------------------*/

extern USHORT    UnitEntryCount;
extern UNITENTRY UnitEntryTable[MAX_UNITS];



/*---------------------*/
/* Initialization Data */
/*---------------------*/

extern BYTE      InitDataStart;
extern USHORT    ADDHandle;
extern BYTE      DeviceTable[MAX_DEVICETABLE_SIZE];
extern MSGTABLE  LockDrvWngMsg;
