	title	dump.cpp
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
	extrn	__vn__FUi:proc
	extrn	PDskEnum:proc
	extrn	Verbose:proc
	extrn	__vd__FPv:proc
	extrn	PDskOpen:proc
	extrn	PDskQueryParam:proc
	extrn	PDskRead:proc
	extrn	memcmp:proc
	extrn	PDskClose:proc
	extrn	WinQueryWindowText:proc
	extrn	WinWindowFromID:proc
	extrn	MyMessageBox__FCiCUlPce:proc
	extrn	_sscanfieee:proc
	extrn	_beginthread:proc
	extrn	WinDismissDlg:proc
	extrn	WinDefDlgProc:proc
	extrn	_ctype:dword
	extrn	_fltused:dword
CONST32_RO	segment
@CBE1	db "%04X: ",0h
	align 04h
@CBE2	db "%02X",0h
	align 04h
@CBE3	db 09h,"%s",0ah,0h
@CBE4	db "-",0h
@CBE5	db " ",0h
	align 04h
@CBE6	db "Dump",0h
	align 04h
@CBE7	db "PDskEnum - rc %lu",0h
	align 04h
@CBE8	db "Physical disks: %lu",0h
@CBE9	db "PDskOpen(%lu) - rc %lu",0h
	align 04h
@CBE10	db "======== Disk %lu, handl"
db "e %#lx ========",0h
@CBE11	db "PDskQueryParam - rc %lu",0h
@CBE12	db "Physical drive parameter"
db "s: Cylinders: %d, Heads:"
db " %d, Sectors/Track: %d",0h
	align 04h
@CBE13	db "Drive capacity: %lu MByt"
db "es",0h
	align 04h
@CBE14	db "PDskRead - rc %lu",0h
	align 04h
@CBE15	db "-------- Dump of real pa"
db "rtition sector --------",0ah
db "%s",0h
	align 04h
@CBE16	db "no VRAID partition on dr"
db "ive %u",0h
	align 04h
@CBE17	db "-------- Dump of PHYSDEV"
db " sector (%lu) --------",0ah,"%"
db "s",0h
	align 04h
@CBE18	db "PHYSDEVICE      ",0h
	align 04h
@CBE19	db "no PHYSDEV sector on par"
db "tition",0h
	align 04h
@CBE20	db "-------- Dump of VRDEV s"
db "ector (%lu) --------",0ah,"%s",0h
@CBE21	db "VRAIDDEVICE     ",0h
	align 04h
@CBE22	db "no VRAIDEVICE mark",0h
	align 04h
@CBE23	db "-------- Dump of first d"
db "ata sector (%lu) -------"
db "-",0ah,"%s",0h
	align 04h
@CBE24	db "only numerics allowed!",0h
	align 04h
@CBE25	db " %ld",0h
	align 04h
@1vcid	db "$Id: dump.cpp,v 1.6 2003"
db "/09/21 00:56:53 vitus Ex"
db "p vitus $",0h
CONST32_RO	ends
BSS32	segment
@40parent	db 04h DUP (0h)
BSS32	ends
CODE32	segment

; 296 DumpDialogProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)

	public DumpDialogProc
DumpDialogProc	proc
	push	ebp
	mov	ebp,esp
	push	edi

; 300     switch( msg )
	mov	eax,[ebp+0ch];	msg

; 296 DumpDialogProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
	push	esi
	sub	esp,0d8h

; 300     switch( msg )
	cmp	eax,020h
	je	@BLBL58
	cmp	eax,03bh
	jne	@BLBL56
@BLBL57:

; 303 	parent = HWNDFROMMP(mp2);
	mov	eax,[ebp+014h];	mp2
	mov	dword ptr  @40parent,eax

; 304 	break;
	jmp	@BLBL56
@BLBL58:

; 307 	switch( SHORT1FROMMP(mp1) )
	mov	ax,[ebp+010h];	mp1
	movzx	eax,ax
	cmp	eax,01h
	je	@BLBL59
	cmp	eax,02h
	je	@BLBL60
	jmp	@BLBL56
@BLBL59:

; 316 		cbText = WinQueryWindowText(WinWindowFromID(hwnd, EF_DRVINDEX),
	push	0209h
	push	dword ptr [ebp+08h];	hwnd
	call	WinWindowFromID
	lea	ecx,[ebp-0d4h];	chText
	push	ecx
	push	0c8h
	push	eax
	call	WinQueryWindowText
	add	esp,014h

; 318 		for( i = 0; i < cbText; ++i )
	test	eax,eax
	jle	@BLBL50
	mov	esi,eax
	xor	edi,edi
@BLBL51:

; 319 		    if( !isdigit(chText[i])  &&  chText[i] != '-' )
	mov	ecx,dword ptr  _ctype
	mov	al,byte ptr [ebp+edi-0d4h]
	movzx	edx,al
	test	byte ptr [ecx+edx*02h],02h
	jne	@BLBL53
	cmp	al,02dh
	je	@BLBL53

; 321 			MyMessageBox( 1, hwnd, "only numerics allowed!" );
	mov	edx,[ebp+08h];	hwnd
	mov	ecx,offset FLAT:@CBE24
	mov	eax,01h
	call	MyMessageBox__FCiCUlPce

; 322 			return 0;
	add	esp,0d8h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebp
	ret	
@BLBL53:

; 318 		for( i = 0; i < cbText; ++i )
	inc	edi
	cmp	edi,esi
	jl	@BLBL51
@BLBL50:

; 327 		sscanf(chText, " %ld", &ul);
	lea	ecx,[ebp-0ch];	ul
	push	ecx
	mov	edx,offset FLAT:@CBE25
	sub	esp,08h
	lea	eax,[ebp-0d4h];	chText
	call	_sscanfieee

; 328 		_beginthread(DumpPartitions, NULL, STACK_SIZE, (PVOID)ul);
	mov	ecx,[ebp-0ch];	ul
	push	ecx
	mov	ecx,0100000h
	sub	esp,0ch
	xor	edx,edx
	mov	eax,offset FLAT: DumpPartitions
	call	_beginthread

; 330 		WinDismissDlg(hwnd, DID_START);
	push	0ah
	push	dword ptr [ebp+08h];	hwnd
	call	WinDismissDlg
	add	esp,024h

; 332 	    return 0;
	add	esp,0d8h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebp
	ret	
@BLBL60:

; 335 	    WinDismissDlg(hwnd, DID_CANCEL);
	push	02h
	push	dword ptr [ebp+08h];	hwnd
	call	WinDismissDlg
	add	esp,08h

; 336 	    return 0;
	add	esp,0d8h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebp
	ret	

; 341 	break;
@BLBL56:

; 344     return WinDefDlgProc(hwnd, msg, mp1, mp2);
	push	dword ptr [ebp+014h];	mp2
	push	dword ptr [ebp+010h];	mp1
	push	dword ptr [ebp+0ch];	msg
	push	dword ptr [ebp+08h];	hwnd
	call	WinDefDlgProc
	add	esp,0e8h
	pop	esi
	pop	edi
	pop	ebp
	ret	
DumpDialogProc	endp

; 113 DumpPartitions(PVOID dummy)

DumpPartitions	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	push	edi
	push	esi
	sub	esp,0238h

; 115     ULONG	which = (ULONG)dummy;
	mov	[ebp-020h],eax;	which

; 117     char	* output = new char[33*80];
	mov	eax,0a50h
	call	__vn__FUi
	mov	ebx,eax

; 123     rc = PDskEnum(&devcnt);
	lea	eax,[ebp-018h];	devcnt
	call	PDskEnum

; 117     char	* output = new char[33*80];
	mov	[ebp-01ch],ebx;	output

; 124     if( rc )
	test	eax,eax
	je	@BLBL17

; 126 	Verbose(0, "Dump", "PDskEnum - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE7
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE6
	xor	eax,eax
	call	Verbose
	mov	eax,ebx

; 127 	delete[] output;
	call	__vd__FPv

; 128 	return;
	add	esp,0248h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL17:

; 130     Verbose(0, "Dump", "Physical disks: %lu", devcnt);
	push	dword ptr [ebp-018h];	devcnt

; 132     for( devidx = 0; devidx < devcnt; ++devidx )
	mov	dword ptr [ebp-014h],0h;	devidx

; 130     Verbose(0, "Dump", "Physical disks: %lu", devcnt);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE8
	mov	edx,offset FLAT:@CBE6
	xor	eax,eax
	call	Verbose
	add	esp,010h

; 132     for( devidx = 0; devidx < devcnt; ++devidx )
	cmp	dword ptr [ebp-018h],0h;	devcnt
	jbe	@BLBL18
	mov	ebx,[ebp-01ch];	output
@BLBL19:

; 139 	if( which != (ULONG)-1  &&  devidx != which )
	mov	eax,[ebp-020h];	which

; 137 	ULONG			partoffset = 0;
	xor	esi,esi

; 139 	if( which != (ULONG)-1  &&  devidx != which )
	cmp	eax,0ffffffffh
	je	@BLBL20
	cmp	[ebp-014h],eax;	devidx
	jne	@BLBL21

; 140 	    continue;				/* ignore this one */
@BLBL20:

; 143 	rc = PDskOpen(devidx, &hd);
	mov	edi,[ebp-014h];	devidx
	lea	edx,[ebp-010h];	hd
	mov	eax,edi
	call	PDskOpen

; 144 	if( rc != 0 )
	test	eax,eax
	je	@BLBL23

; 146 	    Verbose(0, "Dump", "PDskOpen(%lu) - rc %lu", devidx, rc);
	push	eax
	mov	ecx,offset FLAT:@CBE9
	push	edi
	mov	edx,offset FLAT:@CBE6
	sub	esp,0ch
	xor	eax,eax
	call	Verbose
	add	esp,014h

; 147 	    continue;
	jmp	@BLBL21
@BLBL23:

; 152 	    Verbose(0,"Dump", "======== Disk %lu, handle %#lx ========", devidx, hd);
	push	dword ptr [ebp-010h];	hd
	mov	edi,offset FLAT:@CBE6
	push	dword ptr [ebp-014h];	devidx
	mov	ecx,offset FLAT:@CBE10
	sub	esp,0ch
	mov	edx,edi
	xor	eax,eax
	call	Verbose

; 154 	    rc = PDskQueryParam(hd, &dp);
	mov	eax,[ebp-010h];	hd
	lea	edx,[ebp-0238h];	dp
	call	PDskQueryParam
	add	esp,014h
	mov	edx,edi

; 155 	    if( rc != 0 )
	test	eax,eax
	je	@BLBL24

; 157 		Verbose(0, "Dump", "PDskQueryParam - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE11
	sub	esp,0ch
	xor	eax,eax
	call	Verbose
	add	esp,010h

; 158 		break;
	jmp	@BLBL25
@BLBL24:

; 163 		size *= (ULONG)dp.cCylinders;
	mov	di,[ebp-0232h];	dp
	movzx	ecx,di

; 165 		Verbose(0,"Dump",
	movzx	edi,di
	push	edi

; 163 		size *= (ULONG)dp.cCylinders;
	mov	ax,[ebp-0234h];	dp
	movzx	edx,ax

; 165 		Verbose(0,"Dump",
	movzx	eax,ax
	push	eax

; 163 		size *= (ULONG)dp.cCylinders;
	imul	ecx,edx
	mov	dx,[ebp-0236h];	dp
	movzx	eax,dx

; 165 		Verbose(0,"Dump",
	movzx	edx,dx
	push	edx

; 163 		size *= (ULONG)dp.cCylinders;
	shr	ecx,01h

; 165 		Verbose(0,"Dump",
	sub	esp,0ch

; 163 		size *= (ULONG)dp.cCylinders;
	shr	ecx,0ah
	imul	eax,ecx
	mov	[ebp-024h],eax;	size

; 165 		Verbose(0,"Dump",
	mov	edi,offset FLAT:@CBE6
	mov	ecx,offset FLAT:@CBE12
	mov	edx,edi
	xor	eax,eax
	call	Verbose

; 170 		Verbose(0, "Dump", "Drive capacity: %lu MBytes", size);
	push	dword ptr [ebp-024h];	size
	mov	ecx,offset FLAT:@CBE13
	sub	esp,0ch
	mov	edx,edi
	xor	eax,eax
	call	Verbose

; 173 	    rc = PDskRead(hd, 0, 1, buffer);
	lea	eax,[ebp-0228h];	buffer
	push	eax
	sub	esp,0ch
	mov	eax,[ebp-010h];	hd
	mov	cx,01h
	xor	edx,edx
	call	PDskRead
	add	esp,038h
	mov	edx,edi

; 174 	    if( rc != 0 )
	test	eax,eax
	je	@BLBL26

; 176 		Verbose(0, "Dump", "PDskRead - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE14
	sub	esp,0ch
	xor	eax,eax
	call	Verbose
	add	esp,010h

; 177 		break;
	jmp	@BLBL25
@BLBL26:

; 180 	    DumpBuffer(output, buffer, 512);
	lea	edx,[ebp-0228h];	buffer
	mov	cx,0200h
	mov	eax,ebx
	call	DumpBuffer__FPcPCvCUs

; 181 	    Verbose(0, "Dump", "-------- Dump of real partition sector --------\n%s",
	push	ebx
	mov	ecx,offset FLAT:@CBE15
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE6
	xor	eax,eax
	call	Verbose
	add	esp,010h

; 188 		for( i = 0; i < 4; ++i )
	xor	edx,edx
	xor	ecx,ecx
@BLBL28:

; 190 		    if( sec->PartitionTable[i].SysIndicator == VRAID_PARTTYPE )
	lea	edi,dword ptr [ebp+ecx-0228h]
	cmp	byte ptr [edi+01c2h],07ch;	buffer
	jne	@BLBL31
	mov	esi,edi

; 192 			partoffset = sec->PartitionTable[i].RelativeSectors;
	mov	esi,[esi+01c6h]

; 193 			break;
	jmp	@BLBL27
@BLBL31:

; 188 		for( i = 0; i < 4; ++i )
	inc	edx
	add	ecx,010h
	cmp	edx,04h
	jl	@BLBL28
@BLBL27:

; 196 		if( partoffset == 0 )
	test	esi,esi
	jne	@BLBL33

; 198 		    Verbose(1, "Dump", "no VRAID partition on drive %u", devidx);
	push	dword ptr [ebp-014h];	devidx
	mov	ecx,offset FLAT:@CBE16
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE6
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 199 		    break;
	jmp	@BLBL25
@BLBL33:

; 203 	    rc = PDskRead(hd, partoffset, 1, buffer);
	lea	eax,[ebp-0228h];	buffer
	push	eax
	mov	cx,01h
	sub	esp,0ch
	mov	eax,[ebp-010h];	hd
	mov	edx,esi
	call	PDskRead
	add	esp,010h

; 204 	    if( rc != 0 )
	test	eax,eax
	je	@BLBL34

; 206 		Verbose(0, "Dump", "PDskRead - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE14
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE6
	xor	eax,eax
	call	Verbose
	add	esp,010h

; 207 		break;
	jmp	@BLBL25
@BLBL34:

; 210 	    DumpBuffer(output, buffer, 512);
	lea	edi,[ebp-0228h];	buffer
	mov	edx,edi
	mov	cx,0200h
	mov	eax,ebx
	call	DumpBuffer__FPcPCvCUs

; 211 	    Verbose(0, "Dump", "-------- Dump of PHYSDEV sector (%lu) --------\n%s",
	push	ebx
	mov	edi,offset FLAT:@CBE6
	push	esi
	mov	ecx,offset FLAT:@CBE17
	sub	esp,0ch
	mov	edx,edi
	xor	eax,eax
	call	Verbose
	lea	eax,[ebp-0228h];	buffer

; 216 		if( memcmp(sec->sectype, "PHYSDEVICE      ", 16) != 0 )
	mov	ecx,010h
	mov	edx,offset FLAT:@CBE18
	call	memcmp
	add	esp,014h
	mov	edx,edi
	test	eax,eax
	je	@BLBL35

; 218 		    Verbose(1, "Dump", "no PHYSDEV sector on partition");
	mov	ecx,offset FLAT:@CBE19
	mov	eax,01h
	call	Verbose

; 219 		    break;
	jmp	@BLBL25
@BLBL35:

; 221 		dataoffset = sec->u.s.adminspace;
	mov	ax,[ebp-020eh];	buffer

; 224 	    for( i = 1; (((PSEC_VRDEV)buffer)->u.s.flags & 0x80) == 0; ++i )
	test	byte ptr [ebp-020dh],080h;	buffer

; 221 		dataoffset = sec->u.s.adminspace;
	mov	[ebp-026h],ax;	dataoffset

; 224 	    for( i = 1; (((PSEC_VRDEV)buffer)->u.s.flags & 0x80) == 0; ++i )
	jne	@BLBL36
	mov	edi,01h
@BLBL37:

; 226 		rc = PDskRead(hd, partoffset+i, 1, buffer);
	lea	eax,[ebp-0228h];	buffer
	push	eax
	mov	edx,edi
	sub	esp,0ch
	mov	eax,[ebp-010h];	hd
	mov	cx,01h
	add	edx,esi
	call	PDskRead
	add	esp,010h

; 227 		if( rc != 0 )
	test	eax,eax
	je	@BLBL38

; 229 		    Verbose(0, "Dump", "PDskRead - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE14
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE6
	xor	eax,eax
	call	Verbose
	add	esp,010h

; 230 		    break;
	jmp	@BLBL36
@BLBL38:

; 233 		DumpBuffer(output, buffer, 512);
	lea	eax,[ebp-0228h];	buffer
	mov	edx,eax
	mov	cx,0200h
	mov	eax,ebx
	call	DumpBuffer__FPcPCvCUs

; 234 		Verbose(0,
	push	ebx
	mov	eax,edi
	add	eax,esi
	push	eax
	mov	eax,offset FLAT:@CBE6
	sub	esp,0ch
	mov	edx,eax
	mov	ecx,offset FLAT:@CBE20
	xor	eax,eax
	call	Verbose
	lea	eax,[ebp-0228h];	buffer

; 237 		if( memcmp(((PSEC_VRDEV)buffer)->sectype,"VRAIDDEVICE     ",16) != 0)
	mov	ecx,010h
	mov	edx,offset FLAT:@CBE21
	call	memcmp
	add	esp,014h
	mov	edx,offset FLAT:@CBE6
	test	eax,eax
	je	@BLBL41

; 239 		    Verbose(1, "Dump", "no VRAIDEVICE mark");
	mov	ecx,offset FLAT:@CBE22
	mov	eax,01h
	call	Verbose

; 240 		    break;
	jmp	@BLBL36
@BLBL41:

; 224 	    for( i = 1; (((PSEC_VRDEV)buffer)->u.s.flags & 0x80) == 0; ++i )
	inc	edi
	test	byte ptr [ebp-020dh],080h;	buffer
	je	@BLBL37
@BLBL36:

; 246 	    rc = PDskRead(hd, partoffset+dataoffset, 1, buffer);
	lea	eax,[ebp-0228h];	buffer
	push	eax
	mov	cx,01h
	sub	esp,0ch
	movzx	edx,word ptr [ebp-026h];	dataoffset
	mov	eax,[ebp-010h];	hd
	add	edx,esi
	call	PDskRead
	add	esp,010h

; 247 	    if( rc != 0 )
	test	eax,eax
	je	@BLBL43

; 249 		Verbose(0, "Dump", "PDskRead - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE14
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE6
	xor	eax,eax
	call	Verbose
	add	esp,010h

; 250 		break;
	jmp	@BLBL25
@BLBL43:

; 253 	    DumpBuffer(output, buffer, 512);
	lea	edx,[ebp-0228h];	buffer
	mov	cx,0200h
	mov	eax,ebx
	call	DumpBuffer__FPcPCvCUs

; 254 	    Verbose(0,
	push	ebx
	movzx	eax,word ptr [ebp-026h];	dataoffset
	add	eax,esi
	push	eax
	mov	ecx,offset FLAT:@CBE23
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE6
	xor	eax,eax
	call	Verbose
	add	esp,014h

; 258 	while( 0 );
@BLBL25:

; 260 	PDskClose(hd);
	mov	eax,[ebp-010h];	hd
	call	PDskClose

; 262     } /* for every device*/
@BLBL21:

; 132     for( devidx = 0; devidx < devcnt; ++devidx )
	mov	eax,[ebp-014h];	devidx
	inc	eax
	cmp	[ebp-018h],eax;	devcnt
	mov	[ebp-014h],eax;	devidx
	ja	@BLBL19
@BLBL18:

; 264     delete[] output;
	mov	eax,[ebp-01ch];	output
	call	__vd__FPv

; 265     return;
	add	esp,0238h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
DumpPartitions	endp

; 67 DumpBuffer(char * output,void const * buffer,USHORT const cb)

DumpBuffer__FPcPCvCUs	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	[ebp+010h],cx;	cb
	push	edi
	push	esi
	sub	esp,024h

; 70     UCHAR const * p = (UCHAR *)buffer;
	mov	[ebp-010h],edx;	p

; 74     for( i = 0; i < cb; ++i, ++p )
	test	cx,cx
	jbe	@BLBL3
	mov	ebx,eax
	xor	esi,esi
	xor	edi,edi
@BLBL4:
	mov	eax,esi

; 76 	if( (i % 16) == 0 )
	test	al,0fh
	jne	@BLBL5

; 77 	    oi += sprintf(&output[oi], "%04X: ", i);
	push	esi
	lea	eax,dword ptr [ebx+edi]
	sub	esp,08h
	mov	edx,offset FLAT:@CBE1
	call	_sprintfieee
	add	eax,edi
	add	esp,0ch
	mov	edi,eax
@BLBL5:

; 78 	oi += sprintf(&output[oi], "%02X", *p);
	mov	ecx,[ebp-010h];	p
	mov	edx,offset FLAT:@CBE2
	movzx	eax,byte ptr [ecx]
	push	eax
	lea	eax,dword ptr [ebx+edi]
	sub	esp,08h
	call	_sprintfieee

; 79 	ascii[i%16] = (isprint(*p) ? *p : '.');
	mov	ecx,esi

; 78 	oi += sprintf(&output[oi], "%02X", *p);
	mov	edx,[ebp-010h];	p

; 79 	ascii[i%16] = (isprint(*p) ? *p : '.');
	and	ecx,0fh
	lea	ecx,dword ptr [ebp+ecx-028h]

; 78 	oi += sprintf(&output[oi], "%02X", *p);
	add	esp,0ch

; 79 	ascii[i%16] = (isprint(*p) ? *p : '.');
	mov	[ebp-014h],ecx;	@CBE26
	mov	ecx,dword ptr  _ctype

; 78 	oi += sprintf(&output[oi], "%02X", *p);
	add	eax,edi
	mov	edi,eax

; 79 	ascii[i%16] = (isprint(*p) ? *p : '.');
	movzx	eax,byte ptr [edx]
	test	byte ptr [ecx+eax*02h+01h],04h
	je	@BLBL6
	jmp	@BLBL7
@BLBL6:
	mov	eax,02eh
@BLBL7:
	mov	ecx,[ebp-014h];	@CBE26
	mov	edx,eax

; 81 	if( (i+1) % 16 == 0 )
	lea	eax,[esi+01h]

; 79 	ascii[i%16] = (isprint(*p) ? *p : '.');
	mov	[ecx],dl

; 81 	if( (i+1) % 16 == 0 )
	cdq	
	xor	eax,edx
	sub	eax,edx
	and	eax,0fh
	xor	eax,edx
	sub	eax,edx
	jne	@BLBL8

; 84 	    oi += sprintf(&output[oi], "\t%s\n", ascii);
	lea	ecx,[ebp-028h];	ascii
	push	ecx

; 83 	    ascii[16] = '\0';
	mov	byte ptr [ebp-018h],0h;	ascii

; 84 	    oi += sprintf(&output[oi], "\t%s\n", ascii);
	sub	esp,08h
	lea	eax,dword ptr [ebx+edi]
	mov	edx,offset FLAT:@CBE3
	call	_sprintfieee
	add	eax,edi
	add	esp,0ch
	mov	edi,eax

; 85 	}
	jmp	@BLBL12
@BLBL8:

; 86 	else if( (i+1) % 8 == 0 )
	lea	eax,[esi+01h]
	lea	ecx,dword ptr [ebx+edi]
	cdq	
	xchg	ecx,eax
	xor	ecx,edx
	sub	ecx,edx
	and	ecx,07h
	xor	ecx,edx
	sub	ecx,edx
	jne	@BLBL10

; 87 	    oi += sprintf(&output[oi], "-");
	mov	edx,offset FLAT:@CBE4
	call	_sprintfieee
	add	eax,edi
	mov	edi,eax
	jmp	@BLBL12
@BLBL10:

; 89 	    oi += sprintf(&output[oi], " ");
	mov	edx,offset FLAT:@CBE5
	call	_sprintfieee
	add	eax,edi
	mov	edi,eax

; 90     }
@BLBL12:

; 74     for( i = 0; i < cb; ++i, ++p )
	inc	dword ptr [ebp-010h];	p

; 90     }
	mov	eax,esi

; 74     for( i = 0; i < cb; ++i, ++p )
	inc	eax
	cmp	[ebp+010h],ax;	cb
	movzx	esi,ax
	ja	@BLBL4
@BLBL3:

; 91     return;
	add	esp,024h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
DumpBuffer__FPcPCvCUs	endp
CODE32	ends
end
