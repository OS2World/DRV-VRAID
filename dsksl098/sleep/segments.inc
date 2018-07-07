; -*- mode: asm -*-
; $Source: R:/source/driver/sleep/RCS/segments.inc,v $
; $Revision: 1.3 $
; $Date: 1999/11/29 00:41:38 $
; $Locker:  $
;
;	Order segments
; 
; $Log: segments.inc,v $
; Revision 1.3  1999/11/29 00:41:38  vitus
; - added CONST2 to M$ C segments (unused but simplifies)
;
; Revision 1.2  1998/06/28 23:57:32  vitus
; - added SGROUP (swappable group)
;
; Revision 1.1  1997/12/05 02:04:02  vitus
; Initial revision
; -------------------------------------------
; This code is Copyright Vitus Jensen 1997-99


DDHeader	SEGMENT WORD PUBLIC 'CONST'
DDHeader	ENDS

IFNDEF WATCOMC
CONST		SEGMENT DWORD PUBLIC 'CONST'
CONST		ENDS
CONST2		SEGMENT DWORD PUBLIC 'CONST'
CONST2		ENDS
ENDIF

IFDEF WATCOMC
CONST		SEGMENT WORD PUBLIC 'DATA'
CONST		ENDS
CONST2		SEGMENT WORD PUBLIC 'DATA'
CONST2		ENDS
ENDIF

LIBDATA		SEGMENT DWORD PUBLIC 'DATA'
LIBDATA		ENDS

_DATA		SEGMENT WORD PUBLIC 'DATA'
_DATA		ENDS

_BSS		SEGMENT WORD PUBLIC 'BSS'
_BSS		ENDS


_TEXT		SEGMENT DWORD PUBLIC 'CODE'
_TEXT		ENDS

CODE		SEGMENT DWORD PUBLIC 'CODE'
CODE		ENDS

LIBCODE		SEGMENT DWORD PUBLIC 'CODE'
LIBCODE		ENDS

SwapData	SEGMENT DWORD PUBLIC 'FAR_DATA'
SwapData	ENDS

SwapCode	SEGMENT DWORD PUBLIC 'SWAP_CODE'
SwapCode	ENDS

	;; Group segments together

DGROUP		GROUP	CONST, CONST2, _BSS, DDHeader, LIBDATA, _DATA
StaticGroup	GROUP	CODE, LIBCODE, _TEXT

SGROUP		GROUP	SwapData
