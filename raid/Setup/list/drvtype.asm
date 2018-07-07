	title	drvtype.cpp
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
	extrn	Verbose:proc
	extrn	WinSetWindowPtr:proc
	extrn	WinWindowFromID:proc
	extrn	WinSendMsg:proc
	extrn	WinSetWindowText:proc
	extrn	WinSendDlgItemMsg:proc
	extrn	MyMessageBox__FCiCUlPce:proc
	extrn	WinDismissDlg:proc
	extrn	WinQueryWindowPtr:proc
	extrn	_sprintfieee:proc
	extrn	WinDefDlgProc:proc
	extrn	_fltused:dword
CONST32_RO	segment
@CBE1	db "Single",0h
	align 04h
@CBE2	db "Chain",0h
	align 04h
@CBE3	db "Stripe (RAID 0)",0h
@CBE4	db "Mirror (RAID 1)",0h
@CBE5	db "DrvSetup",0h
	align 04h
@CBE6	db "InitDlg (parameter %#x)",0h
@CBE7	db "WinSetWindowPtr failed",0h
	align 04h
@CBE8	db "hwnd(CB_DRVTYPE): %#x",0h
	align 04h
@CBE9	db "hwnd(SC_DRVSIZE): %#x",0h
	align 04h
@CBE10	db "(still unknown)",0h
@CBE11	db "QueryDlgItems(parameter "
db "%#x)",0h
	align 04h
@CBE12	db "No drive type selected!",0h
@CBE13	db "You forgot to select a d"
db "rive type...",0h
	align 04h
@CBE14	db "DrvType.WM_CONTROL",0h
	align 04h
@CBE15	db "selected type %d",0h
	align 04h
@CBE16	db "appr. %lu MBytes",0h
	align 04h
@1vcid	db "$Id: drvtype.cpp,v 1.9 2"
db "000/04/10 01:00:01 vitus"
db " Exp $",0h
	align 04h
@2apcszRaidType	dd offset FLAT:@CBE1
	dd offset FLAT:@CBE2
	dd offset FLAT:@CBE3
	dd offset FLAT:@CBE4
CONST32_RO	ends
CODE32	segment

; 200 DrvtypeDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)

	public DrvtypeDlgProc
DrvtypeDlgProc	proc
	push	ebp
	mov	ebp,esp
	push	ebx

; 202     switch( msg )
	mov	eax,[ebp+0ch];	msg

; 200 DrvtypeDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
	push	edi
	push	esi
	sub	esp,044h

; 202     switch( msg )
	cmp	eax,020h
	je	@BLBL46
	cmp	eax,030h
	je	@BLBL47
	cmp	eax,03bh
	jne	@BLBL44
@BLBL45:

; 211 	    if( InitDlgItems(hwnd, p) )
	mov	ebx,[ebp+08h];	hwnd
	mov	edx,[ebp+014h];	mp2
	mov	eax,ebx
	call	InitDlgItems__FCUlCP9_DRVBUILD
	test	eax,eax
	je	@BLBL20

; 213 		WinDismissDlg(hwnd, DID_ERROR); /* abort dialog */
	push	0ffffh
	push	ebx
	call	WinDismissDlg
	add	esp,08h

; 214 		return FALSE;
	add	esp,044h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL20:

; 217 	return FALSE;				/*all done, focus not changed*/
	xor	eax,eax
	add	esp,044h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL46:

; 223 	switch( SHORT1FROMMP(mp1) )
	mov	ax,[ebp+010h];	mp1
	movzx	eax,ax
	cmp	eax,01h
	je	@BLBL48
	cmp	eax,02h
	je	@BLBL49
	jmp	@BLBL44
@BLBL48:

; 231 		PDRVBUILD p = (PDRVBUILD)WinQueryWindowPtr(hwnd, 0);
	push	0h
	mov	ebx,[ebp+08h];	hwnd
	push	ebx
	call	WinQueryWindowPtr
	mov	ecx,eax
	mov	eax,ebx

; 233 		if( QueryDlgItems(hwnd, p) != 0 )
	mov	edx,ecx
	call	QueryDlgItems__FUlCP9_DRVBUILD
	add	esp,08h
	test	eax,eax
	je	@BLBL21

; 234 		    return 0;			/* restart from beginning */
	xor	eax,eax
	add	esp,044h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL21:

; 236 		WinDismissDlg(hwnd, DID_OK);
	push	01h
	push	dword ptr [ebp+08h];	hwnd
	call	WinDismissDlg
	add	esp,08h

; 238 	    return 0;
	add	esp,044h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL49:

; 241 	    WinDismissDlg(hwnd, DID_CANCEL);
	push	02h
	push	dword ptr [ebp+08h];	hwnd
	call	WinDismissDlg
	add	esp,08h

; 242 	    return 0;
	add	esp,044h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	

; 244 	break;
	jmp	@BLBL44
@BLBL47:

; 253 	    HWND	hwndItem = HWNDFROMMP(mp2);
	mov	ebx,[ebp+014h];	mp2

; 249 	Verbose(3, "DrvSetup", "DrvType.WM_CONTROL");
	mov	ecx,offset FLAT:@CBE14
	mov	edx,offset FLAT:@CBE5
	mov	eax,03h
	call	Verbose

; 251 	    PDRVBUILD	p = (PDRVBUILD)WinQueryWindowPtr(hwnd, 0);
	push	0h
	push	dword ptr [ebp+08h];	hwnd
	call	WinQueryWindowPtr
	add	esp,08h

; 253 	    HWND	hwndItem = HWNDFROMMP(mp2);
	mov	[ebp-018h],ebx;	hwndItem

; 258 	    switch( code )
	mov	ebx,[ebp+010h];	mp1
	shr	ebx,010h
	movzx	ebx,bx

; 251 	    PDRVBUILD	p = (PDRVBUILD)WinQueryWindowPtr(hwnd, 0);
	mov	[ebp-014h],eax;	p

; 258 	    switch( code )
	cmp	ebx,07h
	jne	@BLBL44
@BLBL50:

; 261 		mresult = WinSendMsg(hwndItem, LM_QUERYSELECTION,
	push	0h
	push	0ffffffffh
	push	0165h
	push	dword ptr [ebp-018h];	hwndItem
	call	WinSendMsg

; 263 		idx = SHORT1FROMMR(mresult);
	mov	[ebp-022h],ax;	idx

; 261 		mresult = WinSendMsg(hwndItem, LM_QUERYSELECTION,
	add	esp,010h

; 264 		if( LIT_NONE != idx )
	cmp	ax,0ffffh
	je	@BLBL22

; 268 		    if( 1 != p->cnt )
	mov	ebx,[ebp-014h];	p

; 266 		    ULONG	size = 0;
	mov	dword ptr [ebp-010h],0h;	size

; 268 		    if( 1 != p->cnt )
	cmp	dword ptr [ebx+04h],01h
	je	@BLBL23

; 269 			idx += 2;		/* first entry not displayed */
	add	ax,02h
	jmp	@BLBL24
@BLBL23:

; 273 			idx += 1;
	mov	ax,[ebp-022h];	idx
	inc	ax

; 274 		    }
@BLBL24:

; 276 		    Verbose(2, "DrvSetup", "selected type %d", idx);
	mov	[ebp-022h],ax;	idx
	movsx	ebx,ax
	push	ebx
	mov	ecx,offset FLAT:@CBE15
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE5
	mov	eax,02h
	call	Verbose
	mov	eax,ebx
	add	esp,010h

; 277 		    switch( idx )
	cmp	eax,02h
	jl	@BLBL56
	je	@BLBL55
	cmp	eax,03h
	je	@BLBL54
	cmp	eax,04h
	je	@BLBL53
	jmp	@BLBL51
@BLBL56:
	cmp	eax,01h
	jne	@BLBL51
@BLBL52:

; 282 			size = p->child[0]->dsk->querySize();
	mov	eax,[ebp-014h];	p
	mov	eax,[eax+08h]
	mov	eax,[eax+01ch]
	mov	ebx,[eax]
	mov	ecx,[ebx+014h]
	add	eax,ecx
	call	dword ptr [ebx+010h]
	mov	[ebp-010h],eax;	size

; 283 			break;
	jmp	@BLBL51
@BLBL53:

; 287 			for( i = 0; i < p->cnt; ++i )
	mov	ebx,[ebp-014h];	p

; 286 			size = (ULONG)ULONG_MAX;
	mov	dword ptr [ebp-010h],0ffffffffh;	size

; 287 			for( i = 0; i < p->cnt; ++i )
	cmp	dword ptr [ebx+04h],0h
	jbe	@BLBL25
	mov	esi,[ebp-01ch];	__39
	or	ecx,0ffffffffh
	xor	edi,edi
@BLBL26:
	mov	[ebp-044h],ecx;	@CBE17

; 288 			    size = min(size, p->child[i]->dsk->querySize());
	mov	eax,dword ptr [ebx+edi*04h+08h]
	mov	eax,[eax+01ch]
	mov	ecx,[eax]
	mov	edx,[ecx+014h]
	add	eax,edx
	call	dword ptr [ecx+010h]
	mov	ecx,[ebp-044h];	@CBE17
	cmp	eax,ecx
	jbe	@BLBL27
	mov	esi,ecx
	jmp	@BLBL28
@BLBL27:
	mov	esi,ecx
	mov	eax,dword ptr [ebx+edi*04h+08h]
	mov	eax,[eax+01ch]
	mov	ecx,[eax]
	mov	edx,[ecx+014h]
	add	eax,edx
	call	dword ptr [ecx+010h]
	mov	ecx,esi
	mov	esi,eax
@BLBL28:
	inc	edi

; 287 			for( i = 0; i < p->cnt; ++i )
	cmp	[ebx+04h],edi
	mov	ecx,esi
	ja	@BLBL26
	mov	[ebp-010h],ecx;	size
	mov	[ebp-01ch],ecx;	__39
@BLBL25:

; 289 			break;
	jmp	@BLBL51
@BLBL54:

; 293 			for( i = 0; i < p->cnt; ++i )
	mov	ebx,[ebp-014h];	p

; 292 			size = (ULONG)ULONG_MAX;
	mov	dword ptr [ebp-010h],0ffffffffh;	size

; 293 			for( i = 0; i < p->cnt; ++i )
	cmp	dword ptr [ebx+04h],0h
	jbe	@BLBL32
	mov	esi,[ebp-020h];	__40
	or	ecx,0ffffffffh
	xor	edi,edi
@BLBL33:
	mov	[ebp-044h],ecx;	@CBE17

; 294 			    size = min(size, p->child[i]->dsk->querySize());
	mov	eax,dword ptr [ebx+edi*04h+08h]
	mov	eax,[eax+01ch]
	mov	ecx,[eax]
	mov	edx,[ecx+014h]
	add	eax,edx
	call	dword ptr [ecx+010h]
	mov	ecx,[ebp-044h];	@CBE17
	cmp	eax,ecx
	jbe	@BLBL34
	mov	esi,ecx
	jmp	@BLBL35
@BLBL34:
	mov	esi,ecx
	mov	eax,dword ptr [ebx+edi*04h+08h]
	mov	eax,[eax+01ch]
	mov	ecx,[eax]
	mov	edx,[ecx+014h]
	add	eax,edx
	call	dword ptr [ecx+010h]
	mov	ecx,esi
	mov	esi,eax
@BLBL35:
	inc	edi

; 293 			for( i = 0; i < p->cnt; ++i )
	cmp	[ebx+04h],edi
	mov	ecx,esi
	ja	@BLBL33
	mov	[ebp-010h],ecx;	size
	mov	[ebp-020h],ecx;	__40
@BLBL32:

; 295 			size *= p->cnt;
	mov	ebx,[ebp-014h];	p
	mov	eax,[ebp-010h];	size
	imul	eax,[ebx+04h]
	mov	[ebp-010h],eax;	size

; 296 			break;
	jmp	@BLBL51
@BLBL55:

; 299 			for( i = 0; i < p->cnt; ++i )
	mov	ebx,[ebp-014h];	p
	cmp	dword ptr [ebx+04h],0h
	jbe	@BLBL39
	mov	esi,[ebp-010h];	size
	xor	edi,edi
@BLBL40:

; 300 			    size += p->child[i]->dsk->querySize();
	mov	eax,dword ptr [ebx+edi*04h+08h]

; 299 			for( i = 0; i < p->cnt; ++i )
	inc	edi

; 300 			    size += p->child[i]->dsk->querySize();
	mov	eax,[eax+01ch]
	mov	ecx,[eax]
	mov	edx,[ecx+014h]
	add	eax,edx
	call	dword ptr [ecx+010h]
	add	eax,esi

; 299 			for( i = 0; i < p->cnt; ++i )
	cmp	[ebx+04h],edi

; 300 			    size += p->child[i]->dsk->querySize();
	mov	esi,eax

; 299 			for( i = 0; i < p->cnt; ++i )
	ja	@BLBL40
	mov	[ebp-010h],esi;	size
@BLBL39:

; 301 			break;
@BLBL51:

; 303 		    sprintf(str, "appr. %lu MBytes", size/2/1024);
	mov	ecx,[ebp-010h];	size
	shr	ecx,01h
	shr	ecx,0ah
	push	ecx
	lea	ebx,[ebp-040h];	str
	sub	esp,08h
	mov	edx,offset FLAT:@CBE16
	mov	eax,ebx
	call	_sprintfieee

; 304 		    hwndItem = WinWindowFromID(hwnd, SC_DRVSIZE);
	push	02bfh
	push	dword ptr [ebp+08h];	hwnd
	call	WinWindowFromID
	mov	edi,eax

; 305 		    Verbose(3, "DrvSetup", "hwnd(SC_DRVSIZE): %#x", hwndItem);
	push	edi
	mov	ecx,offset FLAT:@CBE9
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE5
	mov	eax,03h
	call	Verbose

; 306 		    WinSetWindowText(hwndItem, str);
	push	ebx
	push	edi
	call	WinSetWindowText
	add	esp,02ch

; 307 		}
@BLBL22:

; 308 		return 0;
	xor	eax,eax
	add	esp,044h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	

; 318 	break;
@BLBL44:

; 321     return WinDefDlgProc(hwnd, msg, mp1, mp2);
	push	dword ptr [ebp+014h];	mp2
	push	dword ptr [ebp+010h];	mp1
	push	dword ptr [ebp+0ch];	msg
	push	dword ptr [ebp+08h];	hwnd
	call	WinDefDlgProc
	add	esp,054h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
DrvtypeDlgProc	endp

; 157 QueryDlgItems(HWND hwndDlg,PDRVBUILD const p)

QueryDlgItems__FUlCP9_DRVBUILD	proc
	push	ebx

; 162     Verbose(3, "DrvSetup", "QueryDlgItems(parameter %#x)", p);
	mov	ebx,offset FLAT:@CBE5

; 157 QueryDlgItems(HWND hwndDlg,PDRVBUILD const p)
	push	edi
	mov	edi,eax
	push	esi

; 162     Verbose(3, "DrvSetup", "QueryDlgItems(parameter %#x)", p);
	mov	ecx,offset FLAT:@CBE11

; 157 QueryDlgItems(HWND hwndDlg,PDRVBUILD const p)
	sub	esp,010h

; 162     Verbose(3, "DrvSetup", "QueryDlgItems(parameter %#x)", p);
	mov	eax,03h
	push	edx

; 157 QueryDlgItems(HWND hwndDlg,PDRVBUILD const p)
	mov	[esp+028h],edx;	p

; 162     Verbose(3, "DrvSetup", "QueryDlgItems(parameter %#x)", p);
	sub	esp,0ch
	mov	edx,ebx
	call	Verbose

; 166     mresult = WinSendDlgItemMsg(hwndDlg, CB_DRVTYPE, LM_QUERYSELECTION,
	push	0h
	push	0ffffffffh
	push	0165h
	push	02beh
	push	edi
	call	WinSendDlgItemMsg
	add	esp,024h
	mov	edx,ebx

; 168     idx = SHORT1FROMMR(mresult);
	mov	si,ax

; 169     if( LIT_NONE == idx )
	cmp	ax,0ffffh
	jne	@BLBL14

; 171 	Verbose(1, "DrvSetup", "No drive type selected!");
	mov	ecx,offset FLAT:@CBE12
	mov	eax,01h
	call	Verbose
	mov	eax,edi

; 172 	MyMessageBox(1, hwndDlg,
	mov	edx,eax
	mov	ecx,offset FLAT:@CBE13
	mov	eax,01h
	call	MyMessageBox__FCiCUlPce

; 174 	return 1;
	add	esp,010h
	mov	eax,01h
	pop	esi
	pop	edi
	pop	ebx
	ret	
@BLBL14:

; 176     else if( 1 != p->cnt )
	mov	eax,[esp+024h];	p
	cmp	dword ptr [eax+04h],01h
	je	@BLBL16

; 177 	p->type = idx + 2;			/* first entry not displayed */
	movsx	ebx,si
	add	ebx,02h
	mov	[eax],ebx
	jmp	@BLBL15
@BLBL16:

; 181 	p->type = idx + 1;
	mov	eax,[esp+024h];	p
	movsx	ebx,si
	inc	ebx
	mov	[eax],ebx

; 182     }
@BLBL15:

; 184     return 0;
	xor	eax,eax
	add	esp,010h
	pop	esi
	pop	edi
	pop	ebx
	ret	
QueryDlgItems__FUlCP9_DRVBUILD	endp

; 97 InitDlgItems(HWND const hwndDlg,PDRVBUILD const p)

InitDlgItems__FCUlCP9_DRVBUILD	proc
	push	ebx

; 103     Verbose(3, "DrvSetup", "InitDlg (parameter %#x)", p);
	mov	ebx,offset FLAT:@CBE5

; 97 InitDlgItems(HWND const hwndDlg,PDRVBUILD const p)
	push	edi
	mov	edi,edx
	push	esi
	mov	esi,eax
	sub	esp,0ch

; 103     Verbose(3, "DrvSetup", "InitDlg (parameter %#x)", p);
	mov	ecx,offset FLAT:@CBE6

; 97 InitDlgItems(HWND const hwndDlg,PDRVBUILD const p)
	mov	[esp+020h],edi;	p

; 103     Verbose(3, "DrvSetup", "InitDlg (parameter %#x)", p);
	push	edi
	mov	edx,ebx

; 97 InitDlgItems(HWND const hwndDlg,PDRVBUILD const p)
	mov	[esp+020h],esi;	hwndDlg

; 103     Verbose(3, "DrvSetup", "InitDlg (parameter %#x)", p);
	sub	esp,0ch
	mov	eax,03h
	call	Verbose
	mov	edx,edi

; 107     if( (bool = WinSetWindowPtr(hwndDlg, 0, (PVOID)p)) == FALSE )
	push	edx

; 103     Verbose(3, "DrvSetup", "InitDlg (parameter %#x)", p);
	mov	eax,esi

; 107     if( (bool = WinSetWindowPtr(hwndDlg, 0, (PVOID)p)) == FALSE )
	push	0h
	push	eax
	call	WinSetWindowPtr
	add	esp,01ch
	mov	edx,ebx
	test	eax,eax
	jne	@BLBL3

; 109 	Verbose(1, "DrvSetup", "WinSetWindowPtr failed");
	mov	ecx,offset FLAT:@CBE7
	mov	eax,01h
	call	Verbose

; 110 	return 1;				/* can't continue */
	add	esp,0ch
	mov	eax,01h
	pop	esi
	pop	edi
	pop	ebx
	ret	
@BLBL3:

; 115     hwndItem = WinWindowFromID(hwndDlg, CB_DRVTYPE);
	push	02beh

; 117     for( idx = l = 0; l < sizeof(apcszRaidType)/sizeof(apcszRaidType[0]); ++l )
	xor	esi,esi

; 115     hwndItem = WinWindowFromID(hwndDlg, CB_DRVTYPE);
	push	dword ptr [esp+020h];	hwndDlg

; 117     for( idx = l = 0; l < sizeof(apcszRaidType)/sizeof(apcszRaidType[0]); ++l )
	xor	edi,edi

; 115     hwndItem = WinWindowFromID(hwndDlg, CB_DRVTYPE);
	call	WinWindowFromID
	mov	ebx,eax

; 116     Verbose(3, "DrvSetup", "hwnd(CB_DRVTYPE): %#x", hwndItem);
	push	ebx
	mov	ecx,offset FLAT:@CBE8
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE5
	mov	eax,03h
	call	Verbose
	add	esp,018h

; 117     for( idx = l = 0; l < sizeof(apcszRaidType)/sizeof(apcszRaidType[0]); ++l )
	mov	edx,[esp+020h];	p
@BLBL5:

; 119 	if( 1 == p->cnt  &&  0 != l )
	cmp	dword ptr [edx+04h],01h
	jne	@BLBL6
	test	edi,edi
	jne	@BLBL7

; 120 	    continue;				/* only "single" possible */
@BLBL6:

; 121 	else if( 1 != p->cnt  &&  0 == l )
	cmp	dword ptr [edx+04h],01h
	je	@BLBL8
	test	edi,edi
	je	@BLBL7

; 122 	    continue;				/* "single" not possible */
@BLBL8:

; 124 	WinSendMsg(hwndItem, LM_INSERTITEM, MPFROMLONG(idx), apcszRaidType[l]);
	push	dword ptr [edi*04h+@2apcszRaidType]
	push	esi

; 125 	++idx;
	inc	esi

; 124 	WinSendMsg(hwndItem, LM_INSERTITEM, MPFROMLONG(idx), apcszRaidType[l]);
	push	0161h
	push	ebx
	call	WinSendMsg
	mov	edx,[esp+030h];	p
	add	esp,010h

; 126     }
@BLBL7:

; 117     for( idx = l = 0; l < sizeof(apcszRaidType)/sizeof(apcszRaidType[0]); ++l )
	inc	edi
	cmp	edi,04h
	jb	@BLBL5

; 128     hwndItem = WinWindowFromID(hwndDlg, SC_DRVSIZE);
	push	02bfh
	push	dword ptr [esp+020h];	hwndDlg
	call	WinWindowFromID
	mov	ebx,eax

; 129     Verbose(3, "DrvSetup", "hwnd(SC_DRVSIZE): %#x", hwndItem);
	push	ebx
	mov	ecx,offset FLAT:@CBE9
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE5
	mov	eax,03h
	call	Verbose

; 130     WinSetWindowText(hwndItem, "(still unknown)");
	push	offset FLAT:@CBE10
	push	ebx
	call	WinSetWindowText

; 132     return 0;
	add	esp,02ch
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	ret	
InitDlgItems__FCUlCP9_DRVBUILD	endp
CODE32	ends
end
