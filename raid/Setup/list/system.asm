	title	system.cpp
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
	extrn	__vn__FUi:proc
	extrn	DosOpen:proc
	extrn	Verbose:proc
	extrn	DosDevIOCtl:proc
	extrn	__vd__FPv:proc
	extrn	DosClose:proc
	extrn	_fltused:dword
CONST32_RO	segment
@CBE1	db "TESTCFG$",0h
	align 04h
@CBE2	db "VSETUP",0h
	align 04h
@CBE3	db "DosOpen(TESTCFG$) - erro"
db "r %lu",0h
	align 04h
@CBE4	db "DosDevIOCtl(BIOSADAPTER)"
db " - error %lu",0h
	align 04h
@CBE5	db "DosClose(TESTCFG$) - err"
db "or %lu",0h
	align 04h
@1vcid	db "$Id: system.cpp,v 1.3 19"
db "99/02/09 02:53:40 vitus "
db "Exp $",0h
CONST32_RO	ends
BSS32	segment
@2hdTestcfg	dd 0h
BSS32	ends
CODE32	segment

; 54 QueryHostId(PUSHORT const id)

	public QueryHostId__FCPUs
QueryHostId__FCPUs	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	[ebp+08h],eax;	id
	push	edi

; 61     PUCHAR const	data = new UCHAR[datasize];
	mov	eax,08000h

; 54 QueryHostId(PUSHORT const id)
	push	esi
	sub	esp,028h

; 60     ULONG		datasize = 0x8000;
	mov	dword ptr [ebp-010h],08000h;	datasize

; 61     PUCHAR const	data = new UCHAR[datasize];
	call	__vn__FUi

; 66     rc = DosOpen("TESTCFG$", &hdTestcfg, &action, 0, 0,
	push	0h
	lea	ecx,[ebp-024h];	action
	push	042h

; 61     PUCHAR const	data = new UCHAR[datasize];
	mov	[ebp-018h],eax;	data

; 66     rc = DosOpen("TESTCFG$", &hdTestcfg, &action, 0, 0,
	push	01h

; 62     PUCHAR	p = data;
	mov	[ebp-01ch],eax;	p

; 66     rc = DosOpen("TESTCFG$", &hdTestcfg, &action, 0, 0,
	push	0h
	push	0h
	push	ecx
	push	offset FLAT:@2hdTestcfg
	push	offset FLAT:@CBE1
	call	DosOpen
	mov	ebx,eax
	add	esp,020h
	mov	esi,ebx

; 69     if( rc )
	test	ebx,ebx
	je	@BLBL3

; 71 	Verbose(0, "VSETUP", "DosOpen(TESTCFG$) - error %lu", rc);
	push	ebx
	mov	ecx,offset FLAT:@CBE3
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE2
	xor	eax,eax
	call	Verbose

; 72 	return rc;
	add	esp,038h

; 71 	Verbose(0, "VSETUP", "DosOpen(TESTCFG$) - error %lu", rc);
	mov	eax,ebx

; 72 	return rc;
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL3:

; 79     rc = DosDevIOCtl(hdTestcfg,
	lea	ebx,[ebp-010h];	datasize
	push	ebx
	lea	ebx,[ebp-020h];	paramsize
	push	dword ptr [ebp-010h];	datasize

; 75     param.Command = 0;
	mov	dword ptr [ebp-030h],0h;	param

; 79     rc = DosDevIOCtl(hdTestcfg,
	push	dword ptr [ebp-018h];	data

; 76     param.Addr0 = 0x00F8000;
	mov	dword ptr [ebp-02ch],0f8000h;	param

; 79     rc = DosDevIOCtl(hdTestcfg,
	push	ebx
	lea	ebx,[ebp-030h];	param
	push	0ah

; 77     param.NumBytes = 0x7000;
	mov	word ptr [ebp-028h],07000h;	param

; 79     rc = DosDevIOCtl(hdTestcfg,
	push	ebx

; 78     paramsize = sizeof(param);
	mov	dword ptr [ebp-020h],0ah;	paramsize

; 79     rc = DosDevIOCtl(hdTestcfg,
	push	040h
	push	080h
	push	dword ptr  @2hdTestcfg
	call	DosDevIOCtl
	mov	ebx,eax
	add	esp,024h
	mov	esi,ebx

; 83     if( rc )
	test	ebx,ebx
	je	@BLBL4

; 85 	Verbose(0, "VSETUP", "DosDevIOCtl(BIOSADAPTER) - error %lu", rc );
	push	ebx
	mov	ecx,offset FLAT:@CBE4
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE2
	xor	eax,eax
	call	Verbose

; 86 	return rc;
	add	esp,038h

; 85 	Verbose(0, "VSETUP", "DosDevIOCtl(BIOSADAPTER) - error %lu", rc );
	mov	eax,ebx

; 86 	return rc;
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL4:

; 89     *id = 0;
	mov	edi,[ebp+08h];	id
	mov	word ptr [edi],0h

; 90     for( i = 0; i < datasize; ++i, ++p )
	cmp	dword ptr [ebp-010h],0h;	datasize
	jbe	@BLBL5
	mov	edx,[ebp-01ch];	p
	xor	ecx,ecx
@BLBL6:

; 92 	*id += *p;
	movzx	ebx,byte ptr [edx]
	movzx	eax,word ptr [edi]
	add	eax,ebx

; 90     for( i = 0; i < datasize; ++i, ++p )
	inc	ecx
	inc	edx

; 92 	*id += *p;
	mov	[edi],ax

; 90     for( i = 0; i < datasize; ++i, ++p )
	cmp	[ebp-010h],ecx;	datasize
	ja	@BLBL6
	mov	[ebp-01ch],edx;	p
@BLBL5:

; 95     delete[] data;
	mov	eax,[ebp-018h];	data
	call	__vd__FPv

; 96     rc = DosClose( hdTestcfg );
	push	dword ptr  @2hdTestcfg
	call	DosClose
	add	esp,04h
	mov	esi,eax

; 97     if( rc )
	test	eax,eax
	je	@BLBL10

; 99 	Verbose(1, "VSETUP", "DosClose(TESTCFG$) - error %lu", rc );
	push	eax
	mov	ecx,offset FLAT:@CBE5
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE2
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 100     }
@BLBL10:

; 101     return rc;
	mov	eax,esi
	add	esp,028h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
QueryHostId__FCPUs	endp
CODE32	ends
end
