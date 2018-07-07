;*
;* $Source: R:/source/driver/sleep/RCS/Segments.asm,v $
;* $Revision: 1.6 $
;* $Date: 1998/06/28 23:58:07 $
;* $Locker:  $
;*
;* Assembler Helper to order segments
;*
;* $Log: Segments.asm,v $
;* Revision 1.6  1998/06/28 23:58:07  vitus
;* - implemented _CallADD()
;*
;* Revision 1.5  1997/12/05 01:50:02  vitus
;* - moved segment declarations to include file segments.inc
;*
;* Revision 1.4  1997/05/07 23:45:53  vitus
;* - moved CONST segment to front (behind DDHeader)
;*
;* Revision 1.3  1997/03/03 01:27:22  vitus
;* Added SwapData
;*
;* Revision 1.2  1997/02/05 01:52:44  vitus
;* Added more device header flags (not required)
;*
;* Revision 1.1  1996/09/27 03:26:56  vitus
;* Initial revision
;* ----------------------------------------
;* This code is Copyright Vitus Jensen 1996
;*
.286c					; at least!

INCLUDE devhdr.inc			; device header format
INCLUDE segments.inc			; segment layout and order


	PUBLIC	_AsmTimer		; so C-code can install it
	PUBLIC	_CallADD		; passes IORB to ADD


	;; The very first segment has to contain the
	;; device driver header.  Use own segment for
	;; this purpose (but in DGROUP).

DDHeader	SEGMENT

DiskDDHeader	DD	-1
		DW	DEV_CHAR_DEV OR DEV_30 OR DEVLEV_3
		DW	OFFSET AsmStrategy
		DW	0
		DB	"DSLEEPS$"
		DW	0
		DW	0
		DW	0
		DW	0
		DD	DEV_INITCOMPLETE OR DEV_ADAPTER_DD OR DEV_16MB OR DEV_IOCTL2
		DW	0

DDHeader	ENDS



	;; Start of code segments
	;; There is really code contained: small stubs
	;; to call C routines and save/restore registers.

_TEXT		SEGMENT
	ASSUME	CS:StaticGroup

	EXTRN	_Strategy : NEAR	; C routines
	EXTRN	_Timer : NEAR


AsmStrategy	PROC	FAR
		push	es
		push	bx
		call	_Strategy
		add	sp, 4
		retf
AsmStrategy	ENDP


_AsmTimer	PROC	FAR
		pushf
		pusha			; timer has to save ALL!
		push	ds
		push	es
		sti			; run with interrupts enabled

		mov	ax, DGROUP
		mov	ds, ax
		call	_Timer

		pop	es
		pop	ds
		popa
		popf
		ret
_AsmTimer	ENDP




_CallADD	PROC	NEAR
		enter	0,0
		pusha
		push	ds

		les	bx,[bp+8]
		push	es
		push	bx
		call	DWORD PTR [bp+4]
		add	sp,4

		pop	ds
		popa
		leave
		ret
_CallADD	ENDP


_TEXT		ENDS


	END
