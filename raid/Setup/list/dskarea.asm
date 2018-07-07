	title	dskarea.cpp
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
	extrn	WinSendDlgItemMsg:proc
	extrn	Verbose:proc
	extrn	WinSetWindowPtr:proc
	extrn	WinWindowFromID:proc
	extrn	_sprintfieee:proc
	extrn	WinSendMsg:proc
	extrn	WinEnableWindow:proc
	extrn	WinShowWindow:proc
	extrn	MyMessageBox__FCiCUlPce:proc
	extrn	WinDismissDlg:proc
	extrn	WinQueryWindowPtr:proc
	extrn	WinSetWindowText:proc
	extrn	WinDefDlgProc:proc
	extrn	_fltused:dword
CONST32_RO	segment
@CBE1	db "DskArea",0h
@CBE2	db "%u of %u gives %.2lf per"
db "cent",0h
	align 04h
@CBE3	db "gives %lu sectors (of %l"
db "u)",0h
	align 04h
@CBE4	db "InitDlg (parameter %#x)",0h
@CBE5	db "WinSetWindowPtr failed",0h
	align 04h
@CBE6	db "hwnd(LB_AREASIZE): %#x",0h
	align 04h
@CBE7	db "%lu MB",0h
	align 04h
@CBE8	db "hwnd(SL_AREAPERCENT): %#"
db "x",0h
	align 04h
@CBE9	db "hwnd(ST_AREASIZE): %#x",0h
	align 04h
@CBE10	db "QueryDlgItems(parameter "
db "%#x)",0h
	align 04h
@CBE11	db "No disk area selected!",0h
	align 04h
@CBE12	db "You forgot to select an "
db "area...",0h
@CBE13	db "selected area %d",0h
	align 04h
@CBE14	db "Area too small!",0h
@CBE15	db "Please select a reasonab"
db "le size...",0h
	align 04h
@CBE16	db "WM_CONTROL",0h
	align 04h
@CBE17	db "WM_CONTROL, id %#x, code"
db " %#x",0h
	align 04h
@CBE18	db "LN_SELECT area %d",0h
	align 04h
@CBE19	db "Size:",09h,"%lu MByte",0h
@CBE20	dq 1.0000000000000000e+02
@1vcid	db "$Id: dskarea.cpp,v 2.3 2"
db "000/04/10 01:40:51 vitus"
db " Exp $",0h
CONST32_RO	ends
CODE32	segment

; 249 AreaDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)

	public AreaDlgProc
AreaDlgProc	proc
	push	ebp
	mov	ebp,esp
	push	ebx

; 251     switch( msg )
	mov	eax,[ebp+0ch];	msg

; 249 AreaDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
	push	edi
	push	esi
	sub	esp,030h

; 251     switch( msg )
	cmp	eax,020h
	je	@BLBL22
	cmp	eax,030h
	je	@BLBL23
	cmp	eax,03bh
	jne	@BLBL20
@BLBL21:

; 260 	    if( InitDlgItems(hwnd, p) )
	mov	ebx,[ebp+08h];	hwnd
	mov	edx,[ebp+014h];	mp2
	mov	eax,ebx
	call	InitDlgItems__FCUlCP8_DSKAREA
	test	eax,eax
	je	@BLBL17

; 262 		WinDismissDlg(hwnd, DID_ERROR); /* abort dialog */
	push	0ffffh
	push	ebx
	call	WinDismissDlg
	add	esp,08h

; 263 		return FALSE;
	add	esp,030h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL17:

; 266         break;
	jmp	@BLBL20
@BLBL22:

; 273 	switch( SHORT1FROMMP(mp1) )
	mov	ax,[ebp+010h];	mp1
	movzx	eax,ax
	cmp	eax,01h
	je	@BLBL24
	cmp	eax,02h
	je	@BLBL25
	jmp	@BLBL20
@BLBL24:

; 281 		PDSKAREA p = (PDSKAREA)WinQueryWindowPtr(hwnd, 0);
	push	0h
	mov	ebx,[ebp+08h];	hwnd
	push	ebx
	call	WinQueryWindowPtr
	mov	ecx,eax
	mov	eax,ebx

; 283 		if( 0 != QueryDlgItems(hwnd, p) )
	mov	edx,ecx
	call	QueryDlgItems__FUlCP8_DSKAREA
	add	esp,08h
	test	eax,eax
	je	@BLBL18

; 284 		    return 0;			/* restart from beginning */
	xor	eax,eax
	add	esp,030h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL18:

; 286 		WinDismissDlg(hwnd, DID_OK);
	push	01h
	push	dword ptr [ebp+08h];	hwnd
	call	WinDismissDlg
	add	esp,08h

; 288 	    return 0;
	add	esp,030h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL25:

; 291 	    WinDismissDlg(hwnd, DID_CANCEL);
	push	02h
	push	dword ptr [ebp+08h];	hwnd
	call	WinDismissDlg
	add	esp,08h

; 292 	    return 0;
	add	esp,030h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	

; 294 	break;
	jmp	@BLBL20
@BLBL23:

; 305 	    USHORT const	ncode = SHORT2FROMMP(mp1);
	mov	edi,[ebp+010h];	mp1

; 302 	Verbose(5, "DskArea", "WM_CONTROL");
	mov	esi,offset FLAT:@CBE1
	mov	ecx,offset FLAT:@CBE16
	mov	edx,esi
	mov	eax,05h
	call	Verbose

; 306 	    HWND const	hwndSlider = WinWindow
; 306 FromID(hwnd, SL_AREAPERCENT);
	push	02d2h
	mov	eax,[ebp+08h];	hwnd
	push	eax

; 305 	    USHORT const	ncode = SHORT2FROMMP(mp1);
	mov	ebx,edi

; 306 	    HWND const	hwndSlider = WinWindow
; 306 FromID(hwnd, SL_AREAPERCENT);
	call	WinWindowFromID

; 307 	    HWND const	hwndLB = WinWindowFromID(hwnd, LB_AREASIZE);
	push	02d1h

; 306 	    HWND const	hwndSlider = WinWindow
; 306 FromID(hwnd, SL_AREAPERCENT);
	mov	ecx,[ebp+08h];	hwnd

; 307 	    HWND const	hwndLB = WinWindowFromID(hwnd, LB_AREASIZE);
	push	ecx

; 306 	    HWND const	hwndSlider = WinWindow
; 306 FromID(hwnd, SL_AREAPERCENT);
	mov	[ebp-010h],eax;	hwndSlider

; 307 	    HWND const	hwndLB = WinWindowFromID(hwnd, LB_AREASIZE);
	call	WinWindowFromID

; 308 	    HWND const	hwndText = WinWindowFromID(hwnd, ST_AREASIZE);
	push	02d3h

; 307 	    HWND const	hwndLB = WinWindowFromID(hwnd, LB_AREASIZE);
	mov	ecx,[ebp+08h];	hwnd

; 308 	    HWND const	hwndText = WinWindowFromID(hwnd, ST_AREASIZE);
	push	ecx

; 307 	    HWND const	hwndLB = WinWindowFromID(hwnd, LB_AREASIZE);
	mov	[ebp-018h],eax;	hwndLB

; 305 	    USHORT const	ncode = SHORT2FROMMP(mp1);
	shr	edi,010h

; 308 	    HWND const	hwndText = WinWindowFromID(hwnd, ST_AREASIZE);
	call	WinWindowFromID

; 305 	    USHORT const	ncode = SHORT2FROMMP(mp1);
	mov	[ebp-01ah],di;	ncode

; 311 	    Verbose(3, "DskArea","WM_CONTROL, id %#x, code %#x", id, ncode);
	movzx	edi,di
	push	edi
	movzx	ebx,bx
	push	ebx

; 308 	    HWND const	hwndText = WinWindowFromID(hwnd, ST_AREASIZE);
	mov	[ebp-014h],eax;	hwndText
	mov	edx,esi

; 311 	    Verbose(3, "DskArea","WM_CONTROL, id %#x, code %#x", id, ncode);
	mov	ecx,offset FLAT:@CBE17
	mov	eax,03h
	sub	esp,0ch
	call	Verbose
	mov	eax,ebx
	add	esp,02ch

; 312 	    switch( id )
	cmp	eax,02d1h
	je	@BLBL26
	cmp	eax,02d2h
	je	@BLBL27
	jmp	@BLBL20
@BLBL26:

; 321 		switch( ncode )
	movzx	eax,word ptr [ebp-01ah];	ncode
	cmp	eax,01h
	jne	@BLBL20
@BLBL28:

; 325 		    mresult = WinSendMsg(hwndLB, LM_QUERYSELECTION,
	push	0h
	push	0ffffffffh
	push	0165h
	push	dword ptr [ebp-018h];	hwndLB
	call	WinSendMsg
	mov	ebx,eax
	add	esp,010h

; 328 		    if( LIT_NONE != idx )
	cmp	bx,0ffffh
	je	@BLBL19

; 330 			PDSKAREA	p = (PDSKAREA)WinQueryWindowPtr(hwnd, 0);
	push	0h
	push	dword ptr [ebp+08h];	hwnd
	call	WinQueryWindowPtr

; 332 			Verbose(3, "DskArea", "LN_SELECT area %d", idx);
	movsx	ecx,bx
	push	ecx

; 330 			PDSKAREA	p = (PDSKAREA)WinQueryWindowPtr(hwnd, 0);
	mov	edi,eax

; 332 			Verbose(3, "DskArea", "LN_SELECT area %d", idx);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE18
	mov	edx,offset FLAT:@CBE1
	mov	eax,03h
	call	Verbose

; 333 			WinEnableWindow(hwndSlider, TRUE);
	push	01h
	mov	esi,[ebp-010h];	hwndSlider
	push	esi

; 339 			sprintf(str, "Size:\t%lu MByte",
	movsx	ebx,bx

; 333 			WinEnableWindow(hwndSlider, TRUE);
	call	WinEnableWindow

; 334 			WinSendMsg(hwndSlider, SLM_SETSLIDERINFO,
	push	04h

; 339 			sprintf(str, "Size:\t%lu MByte",
	imul	ebx,014h

; 334 			WinSendMsg(hwndSlider, SLM_SETSLIDERINFO,
	push	010003h
	push	0371h
	push	esi
	call	WinSendMsg

; 339 			sprintf(str, "Size:\t%lu MByte",
	mov	ecx,dword ptr [edi+ebx+08h]
	shr	ecx,01h
	shr	ecx,0ah
	push	ecx
	lea	ebx,[ebp-030h];	str
	sub	esp,08h
	mov	edx,offset FLAT:@CBE19
	mov	eax,ebx
	call	_sprintfieee

; 341 			WinSetWindowText(hwndText, str);
	push	ebx
	mov	ebx,[ebp-014h];	hwndText
	push	ebx
	call	WinSetWindowText

; 342 			WinShowWindow(hwndText, TRUE);
	push	01h
	push	ebx
	call	WinShowWindow
	add	esp,04ch

; 343 		    }
@BLBL19:

; 344 		    return 0;
	xor	eax,eax
	add	esp,030h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	

; 349 		break;
	jmp	@BLBL20
@BLBL27:

; 354 		switch( ncode )
	movzx	eax,word ptr [ebp-01ah];	ncode
	cmp	eax,01h
	jne	@BLBL20
@BLBL29:

; 360 		    mresult = WinSendMsg(hwndSlider, LM_QUERYSELECTION,
	push	0h

; 362 		    ul = CalcArea(hwnd,
	mov	esi,[ebp+08h];	hwnd

; 360 		    mresult = WinSendMsg(hwndSlider, LM_QUERYSELECTION,
	push	0ffffffffh
	push	0165h
	push	dword ptr [ebp-010h];	hwndSlider
	call	WinSendMsg

; 362 		    ul = CalcArea(hwnd,
	push	0h

; 360 		    mresult = WinSendMsg(hwndSlider, LM_QUERYSELECTION,
	mov	ebx,eax

; 362 		    ul = CalcArea(hwnd,
	push	esi
	call	WinQueryWindowPtr
	mov	edx,eax
	mov	cx,bx
	mov	eax,esi
	call	CalcArea__FUlCPC8_DSKAREAs

; 365 		    sprintf(str, "Size:\t%lu MByte", ul / 2ul / 1024);
	shr	eax,01h
	shr	eax,0ah
	push	eax
	lea	ebx,[ebp-030h];	str
	sub	esp,08h
	mov	edx,offset FLAT:@CBE19
	mov	eax,ebx
	call	_sprintfieee

; 366 		    WinSetWindowText(hwndText, str);
	push	ebx
	push	dword ptr [ebp-014h];	hwndText
	call	WinSetWindowText
	add	esp,02ch

; 379 	break;
@BLBL20:

; 381     return WinDefDlgProc(hwnd, msg, mp1, mp2);
	push	dword ptr [ebp+014h];	mp2
	push	dword ptr [ebp+010h];	mp1
	push	dword ptr [ebp+0ch];	msg
	push	dword ptr [ebp+08h];	hwnd
	call	WinDefDlgProc
	add	esp,040h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
AreaDlgProc	endp

; 197 QueryDlgItems(HWND hwndDlg,PDSKAREA const p)

QueryDlgItems__FUlCP8_DSKAREA	proc
	push	ebp

; 202     Verbose(3, "DskArea", "QueryDlgItems(parameter %#x)", p);
	mov	ecx,offset FLAT:@CBE10

; 197 QueryDlgItems(HWND hwndDlg,PDSKAREA const p)
	mov	ebp,esp
	push	ebx
	mov	[ebp+0ch],edx;	p
	push	edi
	mov	edi,eax
	push	esi

; 202     Verbose(3, "DskArea", "QueryDlgItems(parameter %#x)", p);
	mov	ebx,offset FLAT:@CBE1

; 197 QueryDlgItems(HWND hwndDlg,PDSKAREA const p)
	sub	esp,014h

; 202     Verbose(3, "DskArea", "QueryDlgItems(parameter %#x)", p);
	mov	eax,03h
	push	edx
	mov	edx,ebx
	sub	esp,0ch

; 197 QueryDlgItems(HWND hwndDlg,PDSKAREA const p)
	mov	[ebp+08h],edi;	hwndDlg

; 202     Verbose(3, "DskArea", "QueryDlgItems(parameter %#x)", p);
	call	Verbose

; 206     mresult = WinSendDlgItemMsg(hwndDlg, LB_AREASIZE, LM_QUERYSELECTION,
	push	0h
	push	0ffffffffh
	push	0165h
	push	02d1h
	push	edi
	call	WinSendDlgItemMsg
	add	esp,024h
	mov	edx,ebx

; 208     idx = SHORT1FROMMR(mresult);
	mov	si,ax

; 209     if( LIT_NONE == idx )
	cmp	ax,0ffffh
	jne	@BLBL13

; 211 	Verbose(1, "DskArea", "No disk area selected!");
	mov	ecx,offset FLAT:@CBE11
	mov	eax,01h
	call	Verbose
	mov	eax,edi

; 212 	MyMessageBox(1, hwndDlg, "You forgot to select an area...");
	mov	edx,eax
	mov	ecx,offset FLAT:@CBE12
	mov	eax,01h
	call	MyMessageBox__FCiCUlPce

; 213 	return 1;
	add	esp,014h
	mov	eax,01h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL13:

; 216     Verbose(2, "DskArea", "selected area %d", idx);
	mov	bx,si
	movsx	eax,bx
	push	eax
	mov	edi,offset FLAT:@CBE1
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE13
	mov	edx,edi
	mov	eax,02h
	call	Verbose
	mov	ecx,ebx

; 217     ULONG	size = CalcArea(hwndDlg, p, idx);
	mov	ebx,[ebp+08h];	hwndDlg
	mov	edx,[ebp+0ch];	p
	mov	eax,ebx
	call	CalcArea__FUlCPC8_DSKAREAs
	add	esp,010h
	mov	edx,edi
	mov	[ebp-010h],eax;	size

; 218     if( size < 2048 )
	cmp	eax,0800h
	jae	@BLBL14

; 220 	Verbose(1, "DskArea", "Area too small!");
	mov	ecx,offset FLAT:@CBE14
	mov	eax,01h
	call	Verbose
	mov	edx,ebx

; 221 	MyMessageBox(1, hwndDlg, "Please select a reasonable size...");
	mov	ecx,offset FLAT:@CBE15
	mov	eax,01h
	call	MyMessageBox__FCiCUlPce

; 222 	return 2;
	add	esp,014h
	mov	eax,02h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL14:

; 225     p->cnt = idx;				/* move selected area to 'cnt' */
	mov	ebx,[ebp+0ch];	p
	mov	cx,si
	movsx	edx,cx

; 226     p->area[idx].size = size;
	movsx	ecx,cx
	imul	ecx,014h

; 225     p->cnt = idx;				/* move selected area to 'cnt' */
	mov	[ebx],edx

; 226     p->area[idx].size = size;
	mov	edx,[ebp-010h];	size
	mov	dword ptr [ebx+ecx+08h],edx

; 228     return 0;
	add	esp,014h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
QueryDlgItems__FUlCP8_DSKAREA	endp

; 126 InitDlgItems(HWND const hwndDlg,PDSKAREA const p)

InitDlgItems__FCUlCP8_DSKAREA	proc
	push	ebx

; 132     Verbose(3, "DskArea", "InitDlg (parameter %#x)", p);
	mov	ebx,offset FLAT:@CBE1

; 126 InitDlgItems(HWND const hwndDlg,PDSKAREA const p)
	push	edi
	mov	edi,edx
	push	esi
	mov	esi,eax
	sub	esp,040h

; 132     Verbose(3, "DskArea", "InitDlg (parameter %#x)", p);
	mov	ecx,offset FLAT:@CBE4

; 126 InitDlgItems(HWND const hwndDlg,PDSKAREA const p)
	mov	[esp+054h],edi;	p

; 132     Verbose(3, "DskArea", "InitDlg (parameter %#x)", p);
	push	edi
	mov	edx,ebx

; 126 InitDlgItems(HWND const hwndDlg,PDSKAREA const p)
	mov	[esp+054h],esi;	hwndDlg

; 132     Verbose(3, "DskArea", "InitDlg (parameter %#x)", p);
	sub	esp,0ch
	mov	eax,03h
	call	Verbose
	mov	edx,edi

; 136     if( (bool = WinSetWindowPtr(hwndDlg, 0, (PVOID)p)) == FALSE )
	push	edx

; 132     Verbose(3, "DskArea", "InitDlg (parameter %#x)", p);
	mov	eax,esi

; 136     if( (bool = WinSetWindowPtr(hwndDlg, 0, (PVOID)p)) == FALSE )
	push	0h
	push	eax
	call	WinSetWindowPtr
	add	esp,01ch
	mov	edx,ebx
	test	eax,eax
	jne	@BLBL5

; 138 	Verbose(0, "DskArea", "WinSetWindowPtr failed");
	xor	eax,eax
	mov	ecx,offset FLAT:@CBE5
	call	Verbose

; 139 	return 1;				/* can't continue */
	add	esp,040h
	mov	eax,01h
	pop	esi
	pop	edi
	pop	ebx
	ret	
@BLBL5:

; 144     hwndItem = WinWindowFromID(hwndDlg, LB_AREASIZE);
	push	02d1h
	push	dword ptr [esp+054h];	hwndDlg
	call	WinWindowFromID
	mov	ebx,eax

; 145     Verbose(3, "DskArea", "hwnd(LB_AREASIZE): %#x", hwndItem);
	push	ebx
	mov	ecx,offset FLAT:@CBE6
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE1
	mov	eax,03h
	call	Verbose
	add	esp,018h

; 146     for( l = 0; l < p->cnt; ++l )
	mov	ecx,[esp+054h];	p
	cmp	dword ptr [ecx],0h
	jbe	@BLBL6
	xor	esi,esi
	xor	edi,edi
@BLBL7:

; 150 	sprintf(s, "%lu MB", p->area[l].size / 2 / 1024);
	mov	edx,offset FLAT:@CBE7
	mov	eax,dword ptr [ecx+esi+08h]
	shr	eax,01h
	shr	eax,0ah
	push	eax

; 146     for( l = 0; l < p->cnt; ++l )
	add	esi,014h

; 150 	sprintf(s, "%lu MB", p->area[l].size / 2 / 1024);
	sub	esp,08h
	lea	ecx,[esp+018h];	s
	mov	eax,ecx
	call	_sprintfieee
	lea	ecx,[esp+018h];	s

; 151 	WinSendMsg(hwndItem, LM_INSERTITEM, MPFROMLONG(l), s);
	push	ecx
	push	edi

; 146     for( l = 0; l < p->cnt; ++l )
	inc	edi

; 151 	WinSendMsg(hwndItem, LM_INSERTITEM, MPFROMLONG(l), s);
	push	0161h
	push	ebx
	call	WinSendMsg
	mov	ecx,[esp+070h];	p
	add	esp,01ch

; 146     for( l = 0; l < p->cnt; ++l )
	cmp	[ecx],edi
	ja	@BLBL7
@BLBL6:

; 156     hwndItem = WinWindowFromID(hwndDlg, SL_AREAPERCENT);
	push	02d2h

; 157     Verbose(3, "DskArea", "hwnd(SL_AREAPERCENT): %#x", hwndItem);
	mov	edi,offset FLAT:@CBE1

; 156     hwndItem = WinWindowFromID(hwndDlg, SL_AREAPERCENT);
	mov	ebx,[esp+054h];	hwndDlg
	push	ebx
	call	WinWindowFromID
	mov	esi,eax

; 157     Verbose(3, "DskArea", "hwnd(SL_AREAPERCENT): %#x", hwndItem);
	push	esi
	mov	ecx,offset FLAT:@CBE8
	sub	esp,0ch
	mov	edx,edi
	mov	eax,03h
	call	Verbose

; 159     WinSendMsg(hwndItem, SLM_SETTICKSIZE, MPFROM2SHORT(SMA_SETALLTICKS,6), NULL);
	push	0h
	push	06ffffh
	push	0372h
	push	esi
	call	WinSendMsg

; 160     WinEnableWindow(hwndItem, FALSE);
	push	0h

; 159     WinSendMsg(hwndItem, SLM_SETTICKSIZE, MPFROM2SHORT(SMA_SETALLTICKS,6), NULL);
	mov	ecx,esi

; 160     WinEnableWindow(hwndItem, FALSE);
	push	ecx
	call	WinEnableWindow

; 164     hwndItem = WinWindowFromID(hwndDlg, ST_AREASIZE);
	push	02d3h
	push	ebx
	call	WinWindowFromID
	mov	edx,edi
	mov	edi,eax

; 165     Verbose(3, "DskArea", "hwnd(ST_AREASIZE): %#x", hwndItem);
	push	edi
	mov	ecx,offset FLAT:@CBE9
	sub	esp,0ch
	mov	eax,03h
	call	Verbose

; 166     WinShowWindow(hwndItem, FALSE);
	push	0h
	push	edi
	call	WinShowWindow

; 168     hwndItem = WinWindowFromID(hwndDlg, DID_HELP);
	push	09h
	push	ebx
	call	WinWindowFromID

; 169     WinShowWindow(hwndItem, FALSE);
	push	0h
	push	eax
	call	WinShowWindow

; 171     return 0;
	add	esp,0a0h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	ret	
InitDlgItems__FCUlCP8_DSKAREA	endp

; 85 CalcArea(HWND hwndDlg, DSKAREA const * const p, SHORT idx)

CalcArea__FUlCPC8_DSKAREAs	proc
	push	ebp
	mov	ebp,esp
	sub	esp,014h
	push	ebx
	mov	ebx,ecx
	push	edi
	mov	edi,edx

; 89     ULONG ul = (ULONG)WinSendDlgItemMsg(hwndDlg, SL_AREAPERCENT,
	push	0h

; 93     ULONG const		freerange = p->area[idx].size;
	movsx	ebx,bx

; 89     ULONG ul = (ULONG)WinSendDlgItemMsg(hwndDlg, SL_AREAPERCENT,
	push	03h

; 93     ULONG const		freerange = p->area[idx].size;
	imul	ebx,014h

; 89     ULONG ul = (ULONG)WinSendDlgItemMsg(hwndDlg, SL_AREAPERCENT,
	push	036ch
	push	02d2h
	push	eax
	call	WinSendDlgItemMsg
	mov	edx,edi
	mov	ecx,eax

; 96     double const 	percent = 100.0 * selected / total;
	shr	ecx,010h
	and	ecx,0ffffh

; 93     ULONG const		freerange = p->area[idx].size;
	mov	ebx,dword ptr [edx+ebx+08h]

; 96     double const 	percent = 100.0 * selected / total;
	movzx	ecx,cx
	mov	[ebp-010h],ecx;	@CBE22
	fild	dword ptr [ebp-010h];	@CBE22
	inc	ax
	movzx	eax,ax
	mov	[ebp-010h],eax;	@CBE22

; 98     Verbose(2, "DskArea", "%u of %u gives %.2lf percent",
	movzx	ecx,cx
	movzx	eax,ax

; 96     double const 	percent = 100.0 * selected / total;
	fild	dword ptr [ebp-010h];	@CBE22
	fmul	qword ptr  @CBE20
	fdivrp	st(1),st(0)
	fstp	qword ptr [ebp-08h];	percent

; 98     Verbose(2, "DskArea", "%u of %u gives %.2lf percent",
	push	dword ptr [ebp-04h];	percent

; 100     ULONG const 	size = freerange * percent / 100.0;
	mov	[ebp-010h],ebx;	@CBE22

; 98     Verbose(2, "DskArea", "%u of %u gives %.2lf percent",
	push	dword ptr [ebp-08h];	percent
	mov	edi,offset FLAT:@CBE1
	push	ecx
	mov	ecx,offset FLAT:@CBE2
	push	eax
	mov	edx,edi
	sub	esp,0ch
	mov	eax,02h
	call	Verbose

; 101     Verbose(2, "DskArea", "gives %lu sectors (of %lu)", size, freerange);
	push	ebx

; 100     ULONG const 	size = freerange * percent / 100.0;
	mov	dword ptr [ebp-0ch],0h;	@CBE22
	fild	qword ptr [ebp-010h];	@CBE22

; 98     Verbose(2, "DskArea", "%u of %u gives %.2lf percent",
	mov	edx,edi

; 101     Verbose(2, "DskArea", "gives %lu sectors (of %lu)", size, freerange);
	mov	ecx,offset FLAT:@CBE3

; 100     ULONG const 	size = freerange * percent / 100.0;
	fmul	qword ptr [ebp-08h];	percent
	fdiv	qword ptr  @CBE20
	fstcw	word ptr [ebp-012h];	@CBE23
	mov	ax,[ebp-012h];	@CBE23
	or	ax,0c3fh
	mov	[ebp-010h],ax;	@CBE22
	fldcw	word ptr [ebp-010h];	@CBE22

; 101     Verbose(2, "DskArea", "gives %lu sectors (of %lu)", size, freerange);
	mov	eax,02h

; 100     ULONG const 	size = freerange * percent / 100.0;
	fistp	qword ptr [ebp-010h];	@CBE22
	mov	edi,[ebp-010h];	@CBE22

; 101     Verbose(2, "DskArea", "gives %lu sectors (of %lu)", size, freerange);
	push	edi
	sub	esp,0ch

; 100     ULONG const 	size = freerange * percent / 100.0;
	fclex	
	fldcw	word ptr [ebp-012h];	@CBE23

; 101     Verbose(2, "DskArea", "gives %lu sectors (of %lu)", size, freerange);
	call	Verbose
	mov	eax,edi
	add	esp,044h

; 102     return size;
	pop	edi
	pop	ebx
	leave	
	ret	
CalcArea__FUlCPC8_DSKAREAs	endp
CODE32	ends
end
