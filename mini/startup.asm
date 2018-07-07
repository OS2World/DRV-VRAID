;*
;* $Source: r:/source/driver/mini/RCS/startup.asm,v $
;* $Revision: 1.1 $
;* $Date: 2000/06/12 01:42:52 $
;* $Locker:  $
;*
;*	Assembler Helper to order segments
;*
;* $Log: startup.asm,v $
;* Revision 1.1  2000/06/12 01:42:52  vitus
;* Initial revision
;*
;* ----------------------------------------
;* Author: Vitus Jensen, 2:2474/424, 1997
;*
.286c						; at least!

	PUBLIC	_DiskDDHeader
	PUBLIC	_szDriverName

INCLUDE	devhdr.inc				; device header format


	;; The very first segment has to contain the
	;; device driver header.  Use own segment for
	;; this purpose (but in DGROUP).

DDHeader	SEGMENT DWORD PUBLIC 'CONST'

_DiskDDHeader	DD	-1
		DW	DEV_CHAR_DEV OR DEV_30 OR DEVLEV_3
		DW	OFFSET AsmStrategy
		DW	0
_szDriverName	DB	"Mini$   "
		DW	0
		DW	0
		DW	0
		DW	0
		DD	DEV_INITCOMPLETE OR DEV_ADAPTER_DD OR DEV_16MB OR DEV_IOCTL2
		DW	0

DDHeader	ENDS

CONST		SEGMENT DWORD PUBLIC 'CONST'	; places strings here
CONST		ENDS

LIBDATA		SEGMENT DWORD PUBLIC 'DATA'
LIBDATA		ENDS

_DATA		SEGMENT DWORD PUBLIC 'DATA'
_DATA		ENDS

_BSS		SEGMENT DWORD PUBLIC 'BSS'
_BSS		ENDS



	;; Start of code segments
	;; There is really code contained: small stubs
	;; to call C routines and save/restore registers.

_TEXT		SEGMENT DWORD PUBLIC 'CODE'

	EXTRN	_Strategy : NEAR		; C routines


AsmStrategy	PROC	FAR
		push	es
		push	bx
		call	_Strategy
		add	sp, 4
		retf
AsmStrategy	ENDP


_TEXT		ENDS

CODE		SEGMENT DWORD PUBLIC 'CODE'
CODE		ENDS

LIBCODE		SEGMENT DWORD PUBLIC 'CODE'
LIBCODE		ENDS


	;; Group segments together

DGROUP		GROUP	CONST, _BSS, DDHeader, LIBDATA, _DATA
StaticGroup	GROUP	CODE, LIBCODE, _TEXT

	END
