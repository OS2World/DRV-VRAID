;*
;* $Source: R:/source/driver/raid/RCS\\startup.asm,v $
;* $Revision: 1.8 $
;* $Date: 2001/07/22 00:33:50 $
;* $Locker:  $
;*
;*	Assembler startup code.
;*
;* $Log: startup.asm,v $
;* Revision 1.8  2001/07/22 00:33:50  vitus
;* - BUILDHOOK: calls UpdateBuilds()
;*
;* Revision 1.7  2001/06/25 00:25:11  vitus
;* - MEMORYHOOK: calls FreeFixed() with EAX
;*
;* Revision 1.6  2000/03/14 02:00:03  vitus
;* - _lock/_unlock: changed to use BX.  Saves push,pop opcodes.
;*
;* Revision 1.5  2000/02/13 12:59:19  vitus
;* - _unlock: there was a window where it restored the wrong interrupt state
;* - _unlock: Warp Server SMB compatible
;*
;* Revision 1.4  1998/11/05 00:51:40  vitus
;* - szDriverName real ASCIIZ
;*
;* Revision 1.3  1998/06/26 23:04:20  vitus
;* - added _CallADD
;*
;* Revision 1.2  1998/02/17 23:41:51  vitus
;* - added AsmTimer()
;* - added _lock/_unlock
;*
;* Revision 1.1  1998/02/12 01:32:42  vitus
;* Initial revision
;* ---------------------------------------------
;* This code is Copyright Vitus Jensen 1997-2001
;*
.386p
.SEQ

	PUBLIC	_DiskDDHeader
	PUBLIC	_szDriverName
	PUBLIC	_AsmTimer
	PUBLIC	MEMORYHOOK,	BUILDHOOK
	PUBLIC	_CallADD			; passes IORB to ADD
	PUBLIC	__lock, __unlock

INCLUDE	devhdr.inc				; device header format
INCLUDE segments.inc				; segment order


	;; The very first segment has to contain the
	;; device driver header.  Use own segment for
	;; this purpose (but in DGROUP).

DDHeader	SEGMENT

_DiskDDHeader	DD	-1
		DW	DEV_CHAR_DEV OR DEV_30 OR DEVLEV_3
		DW	OFFSET AsmStrategy
		DW	0
		DB	"VUJRAID$"
		DW	0
		DW	0
		DW	0
		DW	0
		DD	DEV_INITCOMPLETE OR DEV_ADAPTER_DD OR DEV_16MB OR DEV_IOCTL2
		DW	0

_szDriverName	DB	"VUJRAID$",0

DDHeader	ENDS




	;; Start of code segments
	;; There is really code contained: small stubs
	;; to call C routines and save/restore registers.

_TEXT		SEGMENT
	ASSUME	CS:CGROUP
	EXTRN	_Strategy : NEAR	; C routines
	EXTRN	_Timer : NEAR
	EXTRN	_FreeFixed : NEAR
	EXTRN	_UpdateBuilds : NEAR


AsmStrategy	PROC	FAR
		push	es
		push	bx
		call	_Strategy
		add	sp, 4
		retf
AsmStrategy	ENDP




	;; Called every NTICKS to check all used commands for TIMEOUT_TICKS.
	;; Save all register!!

_AsmTimer	PROC	FAR
		pushf
		pusha
		push	ds
		push	es
		sti				; run with interrupts enabled

		mov	ax, DGROUP
		mov	ds, ax
		call	_Timer

		pop	es
		pop	ds
		popa
		popf
		ret
_AsmTimer	ENDP




;*# ----------------------------------------------------------------------
;* MEMORYHOOK
;*
;* PARAMETER
;*	EAX		parameter passed to ArmCtxHook
;*
;* RETURNS
;*	(nothing, no register changed!)
;*
;* DESCRIPTION
;*	Delayed execution if someone want to call FreeFixed()
;*	from interrupt context where this is not possible.
;*
;* REMARKS
;*
MEMORYHOOK	PROC	FAR
	db	66h
	pusha

	push	eax
	call	_FreeFixed
	add	sp,4

	db	66h
	popa
	ret
MEMORYHOOK	ENDP




;*# ----------------------------------------------------------------------
;* BUILDHOOK
;*
;* PARAMETER
;*	EAX		parameter passed to ArmCtxHook
;*
;* RETURNS
;*	(nothing, no register changed!)
;*
;* DESCRIPTION
;*	Delayed execution if someone want to call UpdateBuilds()
;*
;* REMARKS
;*
BUILDHOOK	PROC	FAR
	db	66h
	pusha

	push	eax
	call	_UpdateBuilds			; Parameter ignored
	add	sp,4

	db	66h
	popa
	ret
BUILDHOOK	ENDP




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




__lock		PROC	NEAR
	enter	0,0
	
	les	bx,[bp+4]		; pointer to lock handle (ULONG)
	pushf				; save flags (to be restored)
	
	;; Try to set bit 0 of lock handle. Do that in a way
	;; which inhibits interactions from all other processes
	;; (whether they run on this or any other processor).
lock_busy:	
	cli				; this processor
   lock bts	WORD PTR es:[bx],0	; all processors
	jc	lock_busy		; lock is busy, retry

	pop	WORD PTR es:[bx+2]	; save old flags
					; in highword of lock handle

	;; - least significant bit in lock handle is set to indicate
	;;   locked handle
	;; - interrupts are disabled (DO NOT ENABLE!!!)
	;; - previous state of [interrupt] flag is saved in
	;;   the lock handle

	leave
	ret
__lock		ENDP	

__unlock	PROC	NEAR
	enter	0,0
	les	bx,[bp+4]		; lock handle

	mov	ax,WORD PTR es:[bx+2]	; retrieve old flags
   lock btr	WORD PTR es:[bx],0	; clear lock bit
	;jnc	fatal			; fatal error: not locked!!!!!

	test	ax,0200h		; were interrupts enabled?
	jz	@@Z			; No -> keep disabled
	sti				; Yes -> enable
@@Z:
	leave
	ret
__unlock	ENDP




_TEXT		ENDS


	END
