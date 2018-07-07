	title	container.cpp
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
	extrn	WinSendMsg:proc
	extrn	_fltused:dword
CONST32_RO	segment
@1id	db "$Id: container.cpp,v 1.7"
db " 1999/04/16 00:59:27 vit"
db "us Exp $",0h
CONST32_RO	ends
CODE32	segment

; 302 LookupDisk(HWND hwnd,PMYRECORD myrecord,VRDev * disk,PMYRECORD * record_returned)

	public LookupDisk__FUlP9_MYRECORDP5VRDevPP9_MYRECORD
LookupDisk__FUlP9_MYRECORDP5VRDevPP9_MYRECORD	proc

; 304     if( myrecord->dsk == disk )
	cmp	[edx+01ch],ecx
	jne	@BLBL20

; 305     {
; 306 	*record_returned = myrecord;
	mov	ecx,[esp+010h];	record_returned

; 307 	return TRUE;				// found matching record, end search
	mov	eax,01h

; 306 	*record_returned = myrecord;
	mov	[ecx],edx

; 307 	return TRUE;				// found matching record, end search
	ret	
@BLBL20:

; 309     return FALSE;
	xor	eax,eax
	ret	
LookupDisk__FUlP9_MYRECORDP5VRDevPP9_MYRECORD	endp

; 274 QueryAny(HWND hwnd,PMYRECORD myrecord,PULONG cnt,PMYRECORD record_array[])

	public QueryAny__FUlP9_MYRECORDPUlPP9_MYRECORD
QueryAny__FUlP9_MYRECORDPUlPP9_MYRECORD	proc
	push	ebx

; 276     record_array[*cnt] = myrecord;
	mov	ebx,[ecx]
	mov	eax,[esp+014h];	record_array
	mov	dword ptr [eax+ebx*04h],edx

; 278     return FALSE;
	pop	ebx

; 277     ++(*cnt);
	inc	dword ptr [ecx]

; 278     return FALSE;
	xor	eax,eax
	ret	
QueryAny__FUlP9_MYRECORDPUlPP9_MYRECORD	endp

; 335 SearchCnr(HWND hwndCnr,PFNSRCH pfnSearch,PVOID pvUser1,PVOID pvUser2,PVOID pvRsrvd)

	public SearchCnr__FUlPFUlPvT2PUl_UlPvN23
SearchCnr__FUlPFUlPvT2PUl_UlPvN23	proc
	push	ebp
	mov	ebp,esp
	sub	esp,04h

; 344     if( pvRsrvd == NULL )
	cmp	dword ptr [ebp+018h],0h;	pvRsrvd

; 335 SearchCnr(HWND hwndCnr,PFNSRCH pfnSearch,PVOID pvUser1,PVOID pvUser2,PVOID pvRsrvd)
	push	ebx
	mov	[ebp+010h],ecx;	pvUser1
	push	edi
	mov	[ebp+0ch],edx;	pfnSearch
	push	esi
	mov	[ebp+08h],eax;	hwndCnr

; 344     if( pvRsrvd == NULL )
	jne	@BLBL23

; 346 	pvChild = (PVOID)WinSendMsg(hwndCnr, CM_QUERYRECORD,
	push	010010h
	push	0h
	push	0340h
	push	eax
	call	WinSendMsg
	add	esp,010h
	mov	esi,eax

; 349     }
	jmp	@BLBL24
@BLBL23:

; 352 	pvChild = (PVOID)WinSendMsg(hwndCnr, CM_QUERYRECORD,
	push	010002h
	push	dword ptr [ebp+018h];	pvRsrvd
	push	0340h
	push	dword ptr [ebp+08h];	hwndCnr
	call	WinSendMsg
	add	esp,010h
	mov	esi,eax

; 355     }
@BLBL24:
	mov	[ebp-04h],esi;	pvChild

; 359     while( pvChild != NULL )
	mov	edi,[ebp-04h];	pvChild
	test	edi,edi
	je	@BLBL25
	mov	ebx,[ebp+0ch];	pfnSearch
	mov	esi,[ebp+08h];	hwndCnr
@BLBL26:

; 361 	pvResult = SearchCnr(hwndCnr, pfnSearch, pvUser1, pvUser2, pvChild);
	push	edi
	mov	ecx,[ebp+010h];	pvUser1
	push	dword ptr [ebp+014h];	pvUser2
	mov	edx,ebx
	sub	esp,0ch
	mov	eax,esi
	call	SearchCnr__FUlPFUlPvT2PUl_UlPvN23
	add	esp,014h

; 362 	if( pvResult != NULL )
	test	eax,eax
	je	@BLBL27

; 363 	    return pvResult;
	pop	esi
	pop	edi
	pop	ebx
	leave	
	ret	
@BLBL27:

; 365 	pvChild = (PVOID)WinSendMsg(hwndCnr, CM_QUERYRECORD,
	push	010100h
	push	edi
	push	0340h
	push	esi
	call	WinSendMsg
	add	esp,010h
	mov	edi,eax

; 359     while( pvChild != NULL )
	test	edi,edi
	jne	@BLBL26
	mov	[ebp-04h],edi;	pvChild
@BLBL25:

; 372     if( pvRsrvd != NULL )
	mov	esi,[ebp+018h];	pvRsrvd
	test	esi,esi
	je	@BLBL31

; 374 	if( (*pfnSearch)(hwndCnr, pvRsrvd, pvUser1, (PULONG)pvUser2))
	push	dword ptr [ebp+014h];	pvUser2
	mov	ecx,[ebp+010h];	pvUser1
	sub	esp,0ch
	mov	eax,[ebp+08h];	hwndCnr
	mov	edx,esi
	call	dword ptr [ebp+0ch];	pfnSearch
	add	esp,010h
	test	eax,eax
	mov	eax,esi
	je	@BLBL32

; 375 	    return pvRsrvd;
	pop	esi
	pop	edi
	pop	ebx
	leave	
	ret	
@BLBL32:

; 377 	    return NULL;
	pop	esi
	xor	eax,eax
	pop	edi
	pop	ebx
	leave	
	ret	
@BLBL31:

; 384 	return NULL;
	pop	esi
	xor	eax,eax
	pop	edi
	pop	ebx
	leave	
	ret	
SearchCnr__FUlPFUlPvT2PUl_UlPvN23	endp

; 241 QueryInuse(HWND hwnd,PMYRECORD myrecord,PULONG cnt,PMYRECORD record_array[])

	public QueryInuse__FUlP9_MYRECORDPUlPP9_MYRECORD
QueryInuse__FUlP9_MYRECORDPUlPP9_MYRECORD	proc
	push	ebx

; 243     if( (myrecord->record.flRecordAttr & CRA_INUSE) != 0 )
	test	byte ptr [edx+04h],08h

; 241 QueryInuse(HWND hwnd,PMYRECORD myrecord,PULONG cnt,PMYRECORD record_array[])
	push	edi

; 243     if( (myrecord->record.flRecordAttr & CRA_INUSE) != 0 )
	je	@BLBL15
	mov	ebx,ecx

; 245 	record_array[*cnt] = myrecord;
	mov	ecx,[esp+018h];	record_array
	mov	edi,[ebx]
	mov	dword ptr [ecx+edi*04h],edx

; 246 	++(*cnt);
	mov	ecx,[ebx]
	inc	ecx
	mov	[ebx],ecx

; 247 	WinSendMsg(hwnd, CM_SETRECORDEMPHASIS,
	movzx	ecx,cx
	or	ecx,040000000h
	push	ecx
	push	edx
	push	034ah
	push	eax
	call	WinSendMsg
	add	esp,010h

; 249     }
@BLBL15:

; 250     return FALSE;
	pop	edi
	xor	eax,eax
	pop	ebx
	ret	
QueryInuse__FUlP9_MYRECORDPUlPP9_MYRECORD	endp

; 210 EmphSource(HWND hwnd,PMYRECORD myrecord,PBOOL pbSource,PULONG cnt)

	public EmphSource__FUlP9_MYRECORDPUlT3
EmphSource__FUlP9_MYRECORDPUlT3	proc
	push	ebx

; 212     ++(*cnt);
	mov	ebx,[esp+014h];	cnt
	inc	dword ptr [ebx]

; 213     WinSendMsg(hwnd, CM_SETRECORDEMPHASIS,
	mov	cx,[ecx]
	movzx	ecx,cx
	or	ecx,040000000h
	push	ecx
	push	edx
	push	034ah
	push	eax
	call	WinSendMsg
	add	esp,010h

; 215     return FALSE;
	xor	eax,eax
	pop	ebx
	ret	
EmphSource__FUlP9_MYRECORDPUlT3	endp

; 180 EmphInuse(HWND hwnd,PMYRECORD myrecord,PBOOL pbSource,PULONG cnt)

	public EmphInuse__FUlP9_MYRECORDPUlT3
EmphInuse__FUlP9_MYRECORDPUlT3	proc
	push	ebx

; 182     ++(*cnt);
	mov	ebx,[esp+014h];	cnt
	inc	dword ptr [ebx]

; 183     WinSendMsg(hwnd, CM_SETRECORDEMPHASIS,
	mov	cx,[ecx]
	movzx	ecx,cx
	or	ecx,080000h
	push	ecx
	push	edx
	push	034ah
	push	eax
	call	WinSendMsg
	add	esp,010h

; 185     return FALSE;
	xor	eax,eax
	pop	ebx
	ret	
EmphInuse__FUlP9_MYRECORDPUlT3	endp

; 147 EmphInuse2Source(HWND hwnd,PMYRECORD myrecord,PBOOL pbSource,PULONG cnt)

	public EmphInuse2Source__FUlP9_MYRECORDPUlT3
EmphInuse2Source__FUlP9_MYRECORDPUlT3	proc
	push	ebx

; 149     if( (myrecord->record.flRecordAttr & CRA_INUSE) != 0 )
	test	byte ptr [edx+04h],08h
	je	@BLBL8

; 151 	++(*cnt);
	mov	ebx,[esp+014h];	cnt
	inc	dword ptr [ebx]

; 152 	WinSendMsg(hwnd, CM_SETRECORDEMPHASIS,
	mov	cx,[ecx]
	movzx	ecx,cx
	or	ecx,040000000h
	push	ecx
	push	edx
	push	034ah
	push	eax
	call	WinSendMsg
	add	esp,010h

; 154     }
@BLBL8:

; 155     return FALSE;
	pop	ebx
	xor	eax,eax
	ret	
EmphInuse2Source__FUlP9_MYRECORDPUlT3	endp

; 113 SearchSource(HWND hwnd,PMYRECORD myrecord,PBOOL pbSource,PULONG cnt)

	public SearchSource__FUlP9_MYRECORDPUlT3
SearchSource__FUlP9_MYRECORDPUlT3	proc
	push	ebx

; 115     if( (myrecord->record.flRecordAttr & CRA_SELECTED) != 0 )
	test	byte ptr [edx+04h],01h
	je	@BLBL5

; 117 	++(*cnt);
	mov	ebx,[esp+014h];	cnt
	inc	dword ptr [ebx]

; 118 	WinSendMsg(hwnd, CM_SETRECORDEMPHASIS,
	mov	cx,[ecx]
	movzx	ecx,cx
	or	ecx,040000000h
	push	ecx
	push	edx
	push	034ah
	push	eax
	call	WinSendMsg
	add	esp,010h

; 120     }
@BLBL5:

; 122     return FALSE;
	pop	ebx
	xor	eax,eax
	ret	
SearchSource__FUlP9_MYRECORDPUlT3	endp

; 83 EmphSelect(HWND hwnd,PMYRECORD myrecord,PBOOL pbSelect,PULONG cnt)

	public EmphSelect__FUlP9_MYRECORDPUlT3
EmphSelect__FUlP9_MYRECORDPUlT3	proc
	push	ebx

; 85     ++(*cnt);
	mov	ebx,[esp+014h];	cnt
	inc	dword ptr [ebx]

; 86     WinSendMsg(hwnd, CM_SETRECORDEMPHASIS,
	mov	cx,[ecx]
	movzx	ecx,cx
	or	ecx,010000h
	push	ecx
	push	edx
	push	034ah
	push	eax
	call	WinSendMsg
	add	esp,010h

; 88     return FALSE;
	xor	eax,eax
	pop	ebx
	ret	
EmphSelect__FUlP9_MYRECORDPUlT3	endp
CODE32	ends
end
