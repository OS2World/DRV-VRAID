	title	about.cpp
	.386
	.387
	includelib CPPOM30.LIB
	includelib OS2386.LIB
CODE32	segment dword use32 public 'CODE'
CODE32	ends
DATA32	segment dword use32 public 'DATA'
DATA32	ends
CONST32_RO	segment dword use32 public 'CONST'
CONST32_RO	ends
BSS32	segment dword use32 public 'BSS'
BSS32	ends
EH_CODE	segment dword use32 public 'CODE'
EH_CODE	ends
CTOR_DTOR1	segment dword use32 public 'DATA'
CTOR_DTOR1	ends
CTOR_DTOR2	segment dword use32 public 'DATA'
CTOR_DTOR2	ends
CTOR_DTOR3	segment dword use32 public 'DATA'
CTOR_DTOR3	ends
EH_DATA	segment para use32 public 'DATA'
EH_DATA	ends
_VFT	segment para use32 public 'DATA'
_VFT	ends
DGROUP	group BSS32, DATA32
	assume	cs:FLAT, ds:FLAT, ss:FLAT, es:FLAT
	extrn	_sprintfieee:proc
	extrn	WinSetWindowText:proc
	extrn	WinWindowFromID:proc
	extrn	WinDefDlgProc:proc
CONST32_RO	segment
@CBE1	db "VSetup - Version %u.%02u"
db 0dh,0ah,"A simple program to vi"
db "sual setup your VRAID de"
db "vices.",0h
CONST32_RO	ends
DATA32	segment
@1vcid	db "$Id: about.cpp,v 1.4 199"
db "9/02/09 02:54:12 vitus E"
db "xp $",0h
DATA32	ends
CODE32	segment

; 61 AboutDialogProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)

	public AboutDialogProc
AboutDialogProc	proc
	sub	esp,012ch
	push	ebx

; 63     switch( msg )
	mov	eax,[esp+0138h];	msg
	cmp	eax,03bh
	jne	@BLBL3
@BLBL4:

; 64     {
; 65 	/*
; 66 	 * WM_INITDLG:
; 67 	 *	mp2	PUSHORT	- program version, format like MS-DOG
; 68 	 */
; 69       case WM_INITDLG:
; 70 	{
; 71 	    USHORT	version = *(PUSHORT)PVOIDFROMMP(mp2);
	mov	ecx,[esp+0140h];	mp2
	mov	cx,[ecx]

; 72 	    char	str[300];
; 73 
; 74 	    sprintf(str,
	movzx	edx,cx
	sar	edx,08h
	and	edx,0ffh
	movzx	edx,dl
	push	edx
	movzx	ecx,cl
	push	ecx
	mov	edx,offset FLAT:@CBE1
	lea	ebx,[esp+0ch];	str
	sub	esp,08h
	mov	eax,ebx
	call	_sprintfieee

; 75 		    "VSetup - Version %u.%02u\r\n"
; 76 		    "A simple program to visual setup your VRAID devices.",
; 77 		    LOBYTE(version), HIBYTE(version) );
; 78 	    WinSetWindowText(WinWindowFromID(hwnd, ST_DESCRIPTION), str);
	push	01f5h
	push	dword ptr [esp+0148h];	hwnd
	call	WinWindowFromID
	push	ebx
	push	eax
	call	WinSetWindowText
	add	esp,020h

; 79 	}
; 80 	break;
; 81 
; 82       default:
; 83 	break;
@BLBL3:

; 84     }
; 85     return WinDefDlgProc(hwnd, msg, mp1, mp2);
	push	dword ptr [esp+0140h];	mp2
	push	dword ptr [esp+0140h];	mp1
	push	dword ptr [esp+0140h];	msg
	push	dword ptr [esp+0140h];	hwnd
	call	WinDefDlgProc
	add	esp,010h
	pop	ebx
	add	esp,012ch
	ret	
AboutDialogProc	endp
CODE32	ends
end
