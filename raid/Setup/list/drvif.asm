	title	drvif.cpp
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
	extrn	DosOpen:proc
	extrn	DosDevIOCtl:proc
	extrn	WinMessageBox:proc
	extrn	DosClose:proc
	extrn	__vn__FUi:proc
	extrn	__vd__FPv:proc
	extrn	memcpy:proc
	extrn	_fltused:dword
CONST32_RO	segment
@CBE1	db "VUJRAID$",0h
	align 04h
@CBE2	db "You have modified disk c"
db "onfiguration, please reb"
db "oot as soon as possible "
db "to let VRAID.FLT read an"
db "y changes you did and re"
db "configure itself.",0ah,"Build "
db "processes will be paused"
db " until reboot.",0h
	align 04h
@CBE3	db "Reminder",0h
	align 04h
@1vcid	db "$Id: drvif.cpp,v 1.10 19"
db "99/10/01 01:06:42 vitus "
db "Exp $",0h
CONST32_RO	ends
DATA32	segment
@2hdDriver	dd 0ffffffffh
DATA32	ends
CODE32	segment

; 640 DriverSetBeeper(USHORT onoff)

	public DriverSetBeeper__FUs
DriverSetBeeper__FUs	proc
	sub	esp,08h
	mov	[esp+0ch],ax;	onoff

; 646     datasize = 0;
	mov	dword ptr [esp+04h],0h;	datasize

; 647     parmsize = sizeof(onoff);
; 648     rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_SET_BEEPER,
	lea	ecx,[esp+04h];	datasize
	push	ecx

; 647     parmsize = sizeof(onoff);
	mov	dword ptr [esp+04h],02h;	parmsize

; 648     rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_SET_BEEPER,
	push	0h
	lea	edx,[esp+08h];	parmsize
	push	0h
	lea	ecx,[esp+018h];	onoff
	push	edx
	push	02h
	push	ecx
	push	04ah
	push	0c0h
	push	dword ptr  @2hdDriver
	call	DosDevIOCtl

; 651     return rc;
	add	esp,02ch
	ret	
DriverSetBeeper__FUs	endp

; 544 DriverArrayinfo(USHORT hd,PVRAID_ARRAYINFO_DATA info)

	public DriverArrayinfo__FUsP21_VRAID_ARRAYINFO_DATA
DriverArrayinfo__FUsP21_VRAID_ARRAYINFO_DATA	proc
	sub	esp,08h
	mov	[esp+010h],edx;	info
	mov	[esp+0ch],ax;	hd

; 552     rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_ARRAYINFO,
	lea	ecx,[esp+04h];	datasize
	push	ecx

; 550     datasize = sizeof(*info);
	mov	dword ptr [esp+08h],0eh;	datasize

; 552     rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_ARRAYINFO,
	push	0eh

; 551     parmsize = sizeof(hd);
	mov	dword ptr [esp+08h],02h;	parmsize

; 552     rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_ARRAYINFO,
	push	dword ptr [esp+018h];	info
	lea	ecx,[esp+0ch];	parmsize
	push	ecx
	lea	ecx,[esp+01ch];	hd
	push	02h
	push	ecx
	push	068h
	push	0c0h
	push	dword ptr  @2hdDriver
	call	DosDevIOCtl

; 555     return rc;
	add	esp,02ch
	ret	
DriverArrayinfo__FUsP21_VRAID_ARRAYINFO_DATA	endp

; 472 DriverArraylist(PUSHORT count,PUSHORT * retbuf)

	public DriverArraylist__FPUsPPUs
DriverArraylist__FPUsPPUs	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,edx
	push	edi
	mov	[ebp+08h],eax;	count
	push	esi
	mov	[ebp+0ch],ebx;	retbuf
	sub	esp,018h

; 486 	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_ARRAYLIST,
	lea	edi,[ebp-010h];	datasize
	push	edi
	lea	ecx,[ebp-01eh];	cb
	push	02h

; 480 	ULONG			datasize = sizeof(cb);
	mov	dword ptr [ebp-010h],02h;	datasize

; 486 	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_ARRAYLIST,
	push	ecx

; 475     USHORT	cnt = 0;
	mov	word ptr [ebp-020h],0h;	cnt

; 486 	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_ARRAYLIST,
	push	0h

; 481 	char			* cp = NULL;
	mov	dword ptr [ebp-014h],0h;	cp

; 486 	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_ARRAYLIST,
	push	0h
	push	0h
	push	067h
	push	0c0h
	push	dword ptr  @2hdDriver
	call	DosDevIOCtl
	add	esp,024h
	mov	[ebp-018h],eax;	rc

; 488 	if( rc )
	test	eax,eax
	jne	@BLBL47

; 495 	datasize = cb;
	movzx	eax,word ptr [ebp-01eh];	cb
	mov	[ebp-010h],eax;	datasize

; 496 	cp = new char[datasize];
	call	__vn__FUi
	mov	esi,eax

; 498 	*retbuf = new USHORT[(cb - sizeof(data->cb))/2]; /* only handles */
	movzx	eax,word ptr [ebp-01eh];	cb
	sub	eax,02h
	shr	eax,01h
	add	eax,eax
	call	__vn__FUi

; 504 	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_ARRAYLIST,
	push	edi

; 496 	cp = new char[datasize];
	mov	[ebp-014h],esi;	cp

; 497 	data = (PVRAID_ARRAYLIST_DATA)cp;
	mov	[ebp-01ch],esi;	data

; 498 	*retbuf = new USHORT[(cb - sizeof(data->cb))/2]; /* only handles */
	mov	[ebx],eax

; 504 	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_ARRAYLIST,
	push	dword ptr [ebp-010h];	datasize
	push	esi
	push	0h
	push	0h
	push	0h
	push	067h
	push	0c0h
	push	dword ptr  @2hdDriver
	call	DosDevIOCtl
	add	esp,024h
	mov	[ebp-018h],eax;	rc

; 506 	if( rc )
	test	eax,eax
	mov	eax,esi
	je	@BLBL48

; 508 	    delete[] cp;
	call	__vd__FPv

; 509 	    delete[] *retbuf;
	mov	eax,[ebx]
	call	__vd__FPv
	mov	edx,ebx

; 510 	    *retbuf = NULL;
	mov	dword ptr [edx],0h

; 511 	    break;
	jmp	@BLBL47
@BLBL48:

; 514 	cnt = (USHORT)((data->cb - FIELDOFFSET(VRAID_ARRAYLIST_DATA,hd))
	mov	edi,[ebp-01ch];	data
	movzx	eax,word ptr [edi]
	sub	eax,02h
	shr	eax,01h
	mov	[ebp-020h],ax;	cnt

; 516 	for( cb = 0; cb < cnt; ++cb )
	mov	cx,[ebp-020h];	cnt
	mov	word ptr [ebp-01eh],0h;	cb
	test	cx,cx
	jbe	@BLBL49
	mov	edx,[ebp+0ch];	retbuf
@BLBL50:

; 517 	    (*retbuf)[cb] = data->hd[cb];
	mov	bx,[ebp-01eh];	cb
	movzx	eax,bx
	mov	si,word ptr [edi+eax*02h+02h]
	mov	eax,[edx]
	movzx	ebx,bx
	mov	word ptr [eax+ebx*02h],si

; 516 	for( cb = 0; cb < cnt; ++cb )
	movzx	eax,word ptr [ebp-01eh];	cb
	inc	eax
	mov	[ebp-01eh],ax;	cb
	cmp	ax,cx
	jb	@BLBL50
@BLBL49:

; 518 	delete[] cp;
	mov	eax,[ebp-014h];	cp
	call	__vd__FPv

; 520     while(0);
@BLBL47:

; 522     *count = cnt;
	mov	ebx,[ebp+08h];	count
	mov	cx,[ebp-020h];	cnt
	mov	[ebx],cx

; 523     return rc;
	mov	eax,[ebp-018h];	rc
	add	esp,018h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
DriverArraylist__FPUsPPUs	endp

; 431 DriverPhysIO(USHORT hd,BOOL write,ULONG offset,PVOID buffer)

	public DriverPhysIO__FUsUlT2Pv
DriverPhysIO__FUsUlT2Pv	proc
	push	ebp
	mov	ebp,esp
	sub	esp,010h

; 438     parm.hd = hd;
	mov	[ebp-08h],ax;	parm

; 439     parm.flags = 0;
	mov	byte ptr [ebp-06h],0h;	parm

; 440     if( write )
	test	edx,edx
	je	@BLBL42

; 441 	parm.flags |= VIOF_WRITE;
	mov	byte ptr [ebp-06h],01h;	parm
@BLBL42:

; 447     rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_ADMINSEC_IO,
	lea	eax,[ebp-0ch];	datasize
	push	eax
	lea	eax,[ebp-010h];	parmsize
	push	0200h

; 442     parm.reserved = 0;
	mov	byte ptr [ebp-05h],0h;	parm

; 447     rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_ADMINSEC_IO,
	push	dword ptr [ebp+014h];	buffer

; 443     parm.partsecno = offset;
	mov	[ebp-04h],ecx;	parm

; 447     rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_ADMINSEC_IO,
	push	eax
	lea	eax,[ebp-08h];	parm
	push	08h

; 445     datasize = 512;
	mov	dword ptr [ebp-0ch],0200h;	datasize

; 447     rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_ADMINSEC_IO,
	push	eax

; 446     parmsize = sizeof(parm);
	mov	dword ptr [ebp-010h],08h;	parmsize

; 447     rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_ADMINSEC_IO,
	push	044h
	push	0c0h
	push	dword ptr  @2hdDriver
	call	DosDevIOCtl
	add	esp,024h

; 449     return rc;
	leave	
	ret	
DriverPhysIO__FUsUlT2Pv	endp

; 393 DriverPhyssize(USHORT hd,PULONG partsize)

	public DriverPhyssize__FUsPUl
DriverPhyssize__FUsPUl	proc
	push	ebx
	mov	ebx,edx
	sub	esp,01ch

; 398     rc = DriverPhysinfo(hd, &data);
	lea	edx,[esp+08h];	data
	call	DriverPhysinfo__FUsP20_VRAID_PHYSINFO_DATA
	mov	edx,ebx

; 400 	*partsize = 0;
	test	eax,eax
	setne	cl
	movzx	ecx,cl
	dec	ecx
	and	ecx,[esp+0dh];	data
	mov	[edx],ecx

; 404     return rc;
	add	esp,01ch
	pop	ebx
	ret	
DriverPhyssize__FUsPUl	endp

; 360 DriverPhysinfo(USHORT hd,PVRAID_PHYSINFO_DATA info)

	public DriverPhysinfo__FUsP20_VRAID_PHYSINFO_DATA
DriverPhysinfo__FUsP20_VRAID_PHYSINFO_DATA	proc
	sub	esp,08h
	mov	[esp+010h],edx;	info
	mov	[esp+0ch],ax;	hd

; 368     rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_PHYSINFO,
	lea	ecx,[esp+04h];	datasize
	push	ecx

; 366     datasize = sizeof(*info);
	mov	dword ptr [esp+08h],011h;	datasize

; 368     rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_PHYSINFO,
	push	011h

; 367     parmsize = sizeof(hd);
	mov	dword ptr [esp+08h],02h;	parmsize

; 368     rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_PHYSINFO,
	push	dword ptr [esp+018h];	info
	lea	ecx,[esp+0ch];	parmsize
	push	ecx
	lea	ecx,[esp+01ch];	hd
	push	02h
	push	ecx
	push	066h
	push	0c0h
	push	dword ptr  @2hdDriver
	call	DosDevIOCtl

; 370     return rc;
	add	esp,02ch
	ret	
DriverPhysinfo__FUsP20_VRAID_PHYSINFO_DATA	endp

; 286 DriverPhyslist(PUSHORT count,PUSHORT * retbuf)

	public DriverPhyslist__FPUsPPUs
DriverPhyslist__FPUsPPUs	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,edx
	push	edi
	mov	[ebp+08h],eax;	count
	push	esi
	mov	[ebp+0ch],ebx;	retbuf
	sub	esp,018h

; 300 	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_PHYSLIST,
	lea	edi,[ebp-010h];	datasize
	push	edi
	lea	ecx,[ebp-01eh];	cb
	push	02h

; 294 	ULONG			datasize = sizeof(cb);
	mov	dword ptr [ebp-010h],02h;	datasize

; 300 	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_PHYSLIST,
	push	ecx

; 289     USHORT	cnt = 0;
	mov	word ptr [ebp-020h],0h;	cnt

; 300 	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_PHYSLIST,
	push	0h

; 295 	char			* cp = NULL;
	mov	dword ptr [ebp-014h],0h;	cp

; 300 	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_PHYSLIST,
	push	0h
	push	0h
	push	065h
	push	0c0h
	push	dword ptr  @2hdDriver
	call	DosDevIOCtl
	add	esp,024h
	mov	[ebp-018h],eax;	rc

; 302 	if( rc )
	test	eax,eax
	jne	@BLBL25

; 309 	datasize = cb;
	movzx	eax,word ptr [ebp-01eh];	cb
	mov	[ebp-010h],eax;	datasize

; 310 	cp = new char[datasize];
	call	__vn__FUi
	mov	esi,eax

; 312 	*retbuf = new USHORT[(cb - sizeof(data->cb))/2]; /* only handles */
	movzx	eax,word ptr [ebp-01eh];	cb
	sub	eax,02h
	shr	eax,01h
	add	eax,eax
	call	__vn__FUi

; 318 	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_PHYSLIST,
	push	edi

; 310 	cp = new char[datasize];
	mov	[ebp-014h],esi;	cp

; 311 	data = (PVRAID_PHYSLIST_DATA)cp;
	mov	[ebp-01ch],esi;	data

; 312 	*retbuf = new USHORT[(cb - sizeof(data->cb))/2]; /* only handles */
	mov	[ebx],eax

; 318 	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_PHYSLIST,
	push	dword ptr [ebp-010h];	datasize
	push	esi
	push	0h
	push	0h
	push	0h
	push	065h
	push	0c0h
	push	dword ptr  @2hdDriver
	call	DosDevIOCtl
	add	esp,024h
	mov	[ebp-018h],eax;	rc

; 320 	if( rc )
	test	eax,eax
	mov	eax,esi
	je	@BLBL26

; 322 	    delete[] cp;
	call	__vd__FPv

; 323 	    delete[] *retbuf;
	mov	eax,[ebx]
	call	__vd__FPv
	mov	edx,ebx

; 324 	    *retbuf = NULL;
	mov	dword ptr [edx],0h

; 325 	    break;
	jmp	@BLBL25
@BLBL26:

; 328 	cnt = (USHORT)((data->cb - FIELDOFFSET(VRAID_PHYSLIST_DATA,hd))
	mov	edi,[ebp-01ch];	data
	movzx	eax,word ptr [edi]
	sub	eax,02h
	shr	eax,01h
	mov	[ebp-020h],ax;	cnt

; 330 	for( cb = 0; cb < cnt; ++cb )
	mov	cx,[ebp-020h];	cnt
	mov	word ptr [ebp-01eh],0h;	cb
	test	cx,cx
	jbe	@BLBL27
	mov	edx,[ebp+0ch];	retbuf
@BLBL28:

; 331 	    (*retbuf)[cb] = data->hd[cb];
	mov	bx,[ebp-01eh];	cb
	movzx	eax,bx
	mov	si,word ptr [edi+eax*02h+02h]
	mov	eax,[edx]
	movzx	ebx,bx
	mov	word ptr [eax+ebx*02h],si

; 330 	for( cb = 0; cb < cnt; ++cb )
	movzx	eax,word ptr [ebp-01eh];	cb
	inc	eax
	mov	[ebp-01eh],ax;	cb
	cmp	ax,cx
	jb	@BLBL28
@BLBL27:

; 332 	delete[] cp;
	mov	eax,[ebp-014h];	cp
	call	__vd__FPv

; 334     while(0);
@BLBL25:

; 336     *count = cnt;
	mov	ebx,[ebp+08h];	count
	mov	cx,[ebp-020h];	cnt
	mov	[ebx],cx

; 337     return rc;
	mov	eax,[ebp-018h];	rc
	add	esp,018h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
DriverPhyslist__FPUsPPUs	endp

; 213 DriverReadMessages(PUCHAR * retbuf)

	public DriverReadMessages__FPPUc
DriverReadMessages__FPPUc	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi
	mov	[ebp+08h],ebx;	retbuf
	push	esi
	sub	esp,020h

; 226 	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_READ_MSGS,
	lea	edi,[ebp-010h];	datasize
	push	edi
	lea	ecx,[ebp-01eh];	cb
	push	02h

; 216     USHORT		cb = 0;			/* keep compiler happy */
	mov	word ptr [ebp-01eh],0h;	cb

; 226 	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_READ_MSGS,
	push	ecx

; 225 	datasize = sizeof(cb);
	mov	dword ptr [ebp-010h],02h;	datasize

; 226 	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_READ_MSGS,
	push	0h

; 218     char		* cp = NULL;
	mov	dword ptr [ebp-014h],0h;	cp

; 226 	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_READ_MSGS,
	push	0h
	push	0h
	push	063h
	push	0c0h
	push	dword ptr  @2hdDriver
	call	DosDevIOCtl
	add	esp,024h
	mov	[ebp-018h],eax;	rc

; 228 	if( rc )
	test	eax,eax
	jne	@BLBL17

; 235 	datasize = FIELDOFFSET(VRAID_MSGS_DATA,msg) + (ULONG)cb;
	movzx	eax,word ptr [ebp-01eh];	cb
	add	eax,02h
	mov	[ebp-010h],eax;	datasize

; 236 	cp = new char[datasize];
	call	__vn__FUi
	mov	esi,eax

; 238 	*retbuf = new UCHAR[cb];		/* only messages */
	movzx	eax,word ptr [ebp-01eh];	cb
	call	__vn__FUi

; 244 	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_READ_MSGS,
	push	edi

; 236 	cp = new char[datasize];
	mov	[ebp-014h],esi;	cp

; 237 	data = (PVRAID_MSGS_DATA)cp;
	mov	[ebp-01ch],esi;	data

; 238 	*retbuf = new UCHAR[cb];		/* only messages */
	mov	[ebx],eax

; 244 	rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_READ_MSGS,
	push	dword ptr [ebp-010h];	datasize
	push	esi
	push	0h
	push	0h
	push	0h
	push	063h
	push	0c0h
	push	dword ptr  @2hdDriver
	call	DosDevIOCtl
	add	esp,024h
	mov	[ebp-018h],eax;	rc

; 246 	if( rc )
	test	eax,eax
	mov	eax,esi
	je	@BLBL18

; 248 	    delete[] cp;
	call	__vd__FPv

; 249 	    delete[] *retbuf;
	mov	eax,[ebx]
	call	__vd__FPv
	mov	eax,ebx

; 250 	    *retbuf = NULL;
	mov	dword ptr [eax],0h

; 251 	    break;
	jmp	@BLBL17
@BLBL18:

; 254 	memcpy(*retbuf,
	mov	ebx,[ebp-01ch];	data
	mov	edi,[ebp+08h];	retbuf
	movzx	ecx,word ptr [ebx]
	mov	eax,[edi]
	sub	ecx,02h
	lea	edx,[ebx+02h]
	call	memcpy

; 257 	(*retbuf)[data->cb-FIELDOFFSET(VRAID_MSGS_DATA,msg)] = '\0'; /* drv err! */
	mov	ecx,[edi]
	movzx	edx,word ptr [ebx]
	mov	byte ptr [ecx+edx-02h],0h

; 259 	delete[] cp;
	mov	eax,[ebp-014h];	cp
	call	__vd__FPv

; 261     while(0);
@BLBL17:

; 263     return rc;
	mov	eax,[ebp-018h];	rc
	add	esp,020h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
DriverReadMessages__FPPUc	endp

; 174 DriverGetVersion(PUSHORT ver,PUSHORT flags)

	public DriverGetVersion__FPUsT1
DriverGetVersion__FPUsT1	proc
	sub	esp,0ch

; 180     if( hdDriver == -1ul )
	cmp	dword ptr  @2hdDriver,0ffffffffh

; 174 DriverGetVersion(PUSHORT ver,PUSHORT flags)
	push	ebx
	push	edi

; 177     ULONG		datasize = sizeof(data);
	mov	dword ptr [esp+010h],06h;	datasize

; 180     if( hdDriver == -1ul )
	jne	@BLBL11

; 181 	return ERROR_INVALID_HANDLE;
	pop	edi
	mov	eax,06h
	pop	ebx
	add	esp,0ch
	ret	
@BLBL11:
	mov	ebx,edx

; 182     rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_VERSION,
	lea	ecx,[esp+010h];	datasize
	push	ecx

; 181 	return ERROR_INVALID_HANDLE;
	mov	edi,eax

; 182     rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_VERSION,
	lea	ecx,[esp+0ch];	data
	push	dword ptr [esp+014h];	datasize
	push	ecx
	push	0h
	push	0h
	push	0h
	push	060h
	push	0c0h
	push	dword ptr  @2hdDriver
	call	DosDevIOCtl
	mov	ecx,eax
	add	esp,024h
	mov	edx,ebx
	mov	eax,edi

; 184     if( rc )
	test	ecx,ecx
	je	@BLBL12

; 185 	return rc;
	pop	edi

; 184     if( rc )
	mov	eax,ecx

; 185 	return rc;
	pop	ebx
	add	esp,0ch
	ret	
@BLBL12:

; 186     *ver = data.version;
	mov	cx,[esp+08h];	data
	mov	[eax],cx

; 187     *flags = data.flags;
	mov	cx,[esp+0ah];	data

; 188     return 0;
	pop	edi

; 187     *flags = data.flags;
	mov	[edx],cx

; 188     return 0;
	pop	ebx
	xor	eax,eax
	add	esp,0ch
	ret	
DriverGetVersion__FPUsT1	endp

; 607 DriverEndSetup(void)

	public DriverEndSetup__Fv
DriverEndSetup__Fv	proc
	sub	esp,08h

; 613     datasize = 0;
	mov	dword ptr [esp+04h],0h;	datasize

; 614     parmsize = 0;
	mov	dword ptr [esp],0h;	parmsize

; 615     rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_END_SETUP,
	lea	ecx,[esp+04h];	datasize
	push	ecx
	lea	ecx,[esp+04h];	parmsize
	push	0h
	push	0h
	push	ecx
	push	0h
	push	0h
	push	049h
	push	0c0h
	push	dword ptr  @2hdDriver
	call	DosDevIOCtl

; 616 		     NULL, parmsize, &parmsize, NULL, datasize, &datasize);
; 617 
; 618     return rc;
	add	esp,02ch
	ret	
DriverEndSetup__Fv	endp

; 128 DriverClose(VOID)

	public DriverClose__Fv
DriverClose__Fv	proc
	sub	esp,0ch

; 134     if( hdDriver == -1ul )
	cmp	dword ptr  @2hdDriver,0ffffffffh

; 131     ULONG		datasize = sizeof(data);
	mov	dword ptr [esp+08h],06h;	datasize

; 134     if( hdDriver == -1ul )
	jne	@BLBL5

; 135 	return ERROR_INVALID_HANDLE;
	add	esp,0ch
	mov	eax,06h
	ret	
@BLBL5:

; 136     rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_QUERY_VERSION,
	lea	ecx,[esp+08h];	datasize
	push	ecx
	lea	ecx,[esp+04h];	data
	push	dword ptr [esp+0ch];	datasize
	push	ecx
	push	0h
	push	0h
	push	0h
	push	060h
	push	0c0h
	push	dword ptr  @2hdDriver
	call	DosDevIOCtl
	add	esp,024h

; 138     if( rc == 0 )
	test	eax,eax
	jne	@BLBL6

; 140 	if( (data.flags & 0x10) == 0 )
	test	byte ptr [esp+02h],010h;	data
	jne	@BLBL7

; 141 	    DriverEndSetup();			// no writing -> restart builds
	call	DriverEndSetup__Fv
	jmp	@BLBL6
@BLBL7:

; 143 	    WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
	push	04037h
	push	0ffffffffh
	push	offset FLAT:@CBE3
	push	offset FLAT:@CBE2
	push	01h
	push	01h
	call	WinMessageBox
	add	esp,018h

; 149     }
@BLBL6:

; 151     rc = DosClose(hdDriver);
	push	dword ptr  @2hdDriver
	call	DosClose

; 153     return rc;
	add	esp,010h

; 152     hdDriver = -1ul;
	mov	dword ptr  @2hdDriver,0ffffffffh

; 153     return rc;
	ret	
DriverClose__Fv	endp

; 575 DriverStartSetup(void)

	public DriverStartSetup__Fv
DriverStartSetup__Fv	proc
	sub	esp,08h

; 581     datasize = 0;
	mov	dword ptr [esp+04h],0h;	datasize

; 582     parmsize = 0;
	mov	dword ptr [esp],0h;	parmsize

; 583     rc = DosDevIOCtl(hdDriver, IOCTL_VRAID_CATEGORY, VRAID_START_SETUP,
	lea	ecx,[esp+04h];	datasize
	push	ecx
	lea	ecx,[esp+04h];	parmsize
	push	0h
	push	0h
	push	ecx
	push	0h
	push	0h
	push	069h
	push	0c0h
	push	dword ptr  @2hdDriver
	call	DosDevIOCtl

; 584 		     NULL, parmsize, &parmsize, NULL, datasize, &datasize);
; 585 
; 586     return rc;
	add	esp,02ch
	ret	
DriverStartSetup__Fv	endp

; 95 DriverOpen(VOID)

	public DriverOpen__Fv
DriverOpen__Fv	proc
	sub	esp,04h
	push	ebx

; 100     rc = DosOpen(DEVICENAME, &hdDriver, &action_taken, 0, 0,
	push	0h
	lea	ecx,[esp+08h];	action_taken
	push	042h
	push	01h
	push	0h
	push	0h
	push	ecx
	push	offset FLAT:@2hdDriver
	push	offset FLAT:@CBE1
	call	DosOpen
	mov	ebx,eax

; 104     DriverStartSetup();
	call	DriverStartSetup__Fv
	add	esp,020h
	mov	eax,ebx

; 105     return rc;
	pop	ebx
	add	esp,04h
	ret	
DriverOpen__Fv	endp
CODE32	ends
end
