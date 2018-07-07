	title	iniloc.cpp
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
	extrn	WinSetWindowText:proc
	extrn	WinWindowFromID:proc
	extrn	WinQueryWindowText:proc
	extrn	CheckValidDir__FCUlCPCc:proc
	extrn	strcpy:proc
	extrn	WinDismissDlg:proc
	extrn	WinDefDlgProc:proc
	extrn	_fltused:dword
DATA32	segment
@1vcid	db "$Id: iniloc.cpp,v 1.3 19"
db "99/02/09 02:51:36 vitus "
db "Exp $",0h
DATA32	ends
BSS32	segment
@7pszDir	db 04h DUP (0h)
BSS32	ends
CODE32	segment

; 55 InilocDialogProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)

	public InilocDialogProc
InilocDialogProc	proc
	push	ebp
	mov	ebp,esp
	push	ebx

; 59     switch( msg )
	mov	eax,[ebp+0ch];	msg

; 55 InilocDialogProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
	push	edi
	sub	esp,010ch

; 59     switch( msg )
	cmp	eax,020h
	je	@BLBL6
	cmp	eax,03bh
	jne	@BLBL4
@BLBL5:

; 63 	WinSetWindowText(WinWindowFromID(hwnd, EF_INILOCATION), pszDir);
	push	0259h

; 62 	pszDir = (char *)PVOIDFROMMP(mp2);
	mov	ecx,[ebp+014h];	mp2

; 63 	WinSetWindowText(WinWindowFromID(hwnd, EF_INILOCATION), pszDir);
	push	dword ptr [ebp+08h];	hwnd

; 62 	pszDir = (char *)PVOIDFROMMP(mp2);
	mov	dword ptr  @7pszDir,ecx

; 63 	WinSetWindowText(WinWindowFromID(hwnd, EF_INILOCATION), pszDir);
	call	WinWindowFromID
	push	dword ptr  @7pszDir
	push	eax
	call	WinSetWindowText
	add	esp,010h

; 64 	break;
	jmp	@BLBL4
@BLBL6:

; 67 	switch( SHORT1FROMMP(mp1) )
	mov	ax,[ebp+010h];	mp1
	movzx	eax,ax
	cmp	eax,01h
	je	@BLBL7
	cmp	eax,02h
	je	@BLBL8
	jmp	@BLBL4
@BLBL7:

; 74 		cbText = WinQueryWindowText(WinWindowFromID(hwnd, EF_INILOCATION),
	push	0259h
	mov	ebx,[ebp+08h];	hwnd
	push	ebx
	lea	edi,[ebp-010ch];	chText
	call	WinWindowFromID
	push	edi
	push	0104h
	push	eax
	call	WinQueryWindowText
	mov	edx,edi
	mov	ecx,eax

; 76 		chText[cbText] = '\0';
	mov	byte ptr [edx+ecx],0h

; 74 		cbText = WinQueryWindowText(WinWindowFromID(hwnd, EF_INILOCATION),
	mov	eax,ebx

; 77 		if( CheckValidDir(hwnd, chText) != 0 )
	call	CheckValidDir__FCUlCPCc
	add	esp,014h
	test	eax,eax
	je	@BLBL3

; 80 		    return 0;			/* redo from start */
	xor	eax,eax
	add	esp,010ch
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL3:

; 82 		strcpy(pszDir, chText);		/* copy to caller */
	lea	edx,[ebp-010ch];	chText
	mov	eax,dword ptr  @7pszDir
	call	strcpy

; 83 		WinDismissDlg(hwnd, DID_OK);
	push	01h
	push	dword ptr [ebp+08h];	hwnd
	call	WinDismissDlg
	add	esp,08h

; 85 	    return 0;
	add	esp,010ch
	xor	eax,eax
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL8:

; 88 	    WinDismissDlg(hwnd, DID_CANCEL);
	push	02h
	push	dword ptr [ebp+08h];	hwnd
	call	WinDismissDlg
	add	esp,08h

; 89 	    return 0;
	add	esp,010ch
	xor	eax,eax
	pop	edi
	pop	ebx
	pop	ebp
	ret	

; 94 	break;
@BLBL4:

; 96     return WinDefDlgProc( hwnd, msg, mp1, mp2 );
	push	dword ptr [ebp+014h];	mp2
	push	dword ptr [ebp+010h];	mp1
	push	dword ptr [ebp+0ch];	msg
	push	dword ptr [ebp+08h];	hwnd
	call	WinDefDlgProc
	add	esp,011ch
	pop	edi
	pop	ebx
	pop	ebp
	ret	
InilocDialogProc	endp
CODE32	ends
end
