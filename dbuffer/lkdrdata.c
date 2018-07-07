/*static char *SCCSID = "src/dev/dasd/lockdrv/lkdrdata.c, lockdrv, ddk_subset, kira_r206.035 93/03/21";*/
/**************************************************************************
 *
 * SOURCE FILE NAME = LKDRDATA.C
 *
 * DESCRIPTIVE NAME = LOCKDRV.FLT - Convert Removable DASD to Fixed Drive
 *                    Static/Initialization Data
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
 * DESCRIPTION : Declares all internal data used by this ADD.
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



/*--------------*/
/* Static Data  */
/*--------------*/

PFN     Device_Help             = 0;
USHORT  InitComplete            = 0;
BYTE    LkdrIORB[MAX_IORB_SIZE] = {0};
USHORT  UnitListCount;
USHORT  UnitList[MAX_UNITS];



/*--------------------*/
/* Configuration Data */
/*--------------------*/

USHORT    UnitEntryCount            =  0;
UNITENTRY UnitEntryTable[MAX_UNITS] = {0};



/*---------------------*/
/* Initialization Data */
/*---------------------*/

BYTE    InitDataStart                     = 0;
USHORT  ADDHandle                         = 0;
BYTE    DeviceTable[MAX_DEVICETABLE_SIZE] = {0};



#define MSG_REPLACEMENT_STRING  1178

static UCHAR LockDrvWng[] =                                        \
"\n\r"                                                             \
"LockDrive Filter: Some devices requested could not be locked.\n\r";

MSGTABLE  LockDrvWngMsg = {
    MSG_REPLACEMENT_STRING,
    1,
    LockDrvWng
};
