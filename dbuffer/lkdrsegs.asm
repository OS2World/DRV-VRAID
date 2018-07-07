;       SCCSID = src/dev/dasd/lockdrv/lkdrsegs.asm, lockdrv, ddk_subset, kira_r206.035 93/03/21

        page    ,132

;/*****************************************************************************
;*
;* SOURCE FILE NAME = LKDRSEGS.ASM
;*
;* DESCRIPTIVE NAME = LOCKDRV.FLT - Convert Removable DASD to Fixed Drive
;*                    Segment Declarations
;*
;* 
;* 
;* COPYRIGHT    Copyright (C) 1992 IBM Corporation
;* 
;* The following IBM OS/2 2.1 source code is provided to you solely for
;* the purpose of assisting you in your development of OS/2 2.x device
;* drivers. You may use this code in accordance with the IBM License
;* Agreement provided in the IBM Device Driver Source Kit for OS/2. This
;* Copyright statement may not be removed.
;* 
;* 
;*
;* VERSION      V2.0
;*
;* DATE
;*
;* DESCRIPTION  Assembler Helper to order segments
;*
;*
;***************************************************************************/




        include devhdr.inc

;/*-------------------------------------*/
;/* Assembler Helper to order segments  */
;/*-------------------------------------*/

DDHeader        segment dword public 'DATA'

DiskDDHeader   dd      -1
               dw      DEVLEV_3 + DEV_CHAR_DEV
               dw      _LKDRStr1
               dw      0
               db      "LKDRFLT$"
               dw      0
               dw      0
               dw      0
               dw      0
               dd      (DEV_ADAPTER_DD OR DEV_IOCTL2)
               dw      0

DDHeader        ends

LIBDATA         segment dword public 'DATA'
LIBDATA         ends

_DATA           segment dword public 'DATA'
_DATA           ends

CONST           segment dword public 'CONST'
CONST           ends

_BSS            segment dword public 'BSS'
_BSS            ends

_TEXT           segment dword public 'CODE'

                extrn  _LKDRStr1:near

_TEXT           ends

Code            segment dword public 'CODE'
Code            ends

LIBCODE         segment dword public 'CODE'
LIBCODE         ends

SwapCode        segment dword public 'CODE'
SwapCode        ends

DGROUP          group   CONST, _BSS, DDHeader, LIBDATA, _DATA
StaticGroup     group   Code, LIBCODE, _TEXT
SwapGroup       group   SwapCode

        end
