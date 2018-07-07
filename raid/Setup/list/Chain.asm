	title	Chain.cpp
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
	public	__vft6VChain5VDisk
	extrn	memcpy:proc
	extrn	WinLoadPointer:proc
	extrn	_Exception_CPP:proc
	extrn	WinDestroyPointer:proc
	extrn	__dl__FPv:proc
	extrn	__vn__FUi:proc
	extrn	_sprintfieee:proc
	extrn	__nw__FUi:proc
	extrn	memset:proc
	extrn	Verbose:proc
	extrn	time:proc
	extrn	Crc16__FPvUs:proc
	extrn	memcmp:proc
	extrn	__vd__FPv:proc
	extrn	__PureVirtualCalled:proc
	extrn	_fltused:dword
CONST32_RO	segment
@CBE1	db "Chaining (OS/2 Drive %lu"
db ")",0ah,"ID %02X%02X%02X%02X%02"
db "X%02X    %lu MByte",0h
	align 04h
@CBE2	db "Chaining",0ah,"ID %02X%02X%02X"
db "%02X%02X%02X    %lu MByt"
db "e",0h
	align 04h
@CBE3	db "VChain::ioRemoveParent",0h
	align 04h
@CBE4	db "can't clear parent SEC_V"
db "RDEV - rc %lu",0h
	align 04h
@CBE5	db "can't read own SEC_VRDEV"
db " - rc %lu",0h
	align 04h
@CBE6	db "can't write own SEC_VRDE"
db "V - rc %lu",0h
	align 04h
@CBE7	db "VChain::ioChangeID",0h
	align 04h
@CBE8	db "VRAIDDEVICE     ",0h
	align 04h
@CBE9	db "VChain::ioSync",0h
	align 04h
@CBE10	db "write(0,1,...) - rc %lu,"
db " not updated",0h
	align 04h
@CBE11	db "VChain",0h
	align 04h
@CBE12	db "Data error when reading "
db "child %d, ignored",0h
	align 04h
@1vcid	db "$Id: chain.cpp,v 1.11 20"
db "00/04/10 00:56:56 vitus "
db "Exp $",0h
CONST32_RO	ends
EH_DATA	segment
@1c__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__7VRDriveFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@33__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__7VRDriveFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@bf__fsm_tab55	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__5VRDevFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@d2__fsm_tab57	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__5VRDevFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
EH_DATA	ends
_VFT	segment
__vft6VChain5VDisk	dd 0
	db 0h,0h,0h,0h
	dd offset FLAT:isWritable__6VChainFv
	db 0h,0h,0h,0h
	dd offset FLAT:querySize__6VChainFv
	db 0h,0h,0h,0h
	dd offset FLAT:read__6VChainFUlT1Pv
	db 0h,0h,0h,0h
	dd offset FLAT:write__6VChainFUlT1Pv
	db 0h,0h,0h,0h
	dd offset FLAT:__dt__6VChainFv
	db 0h,0h,0h,0h
	dd offset FLAT:forceUnwritable__6VChainFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioDeleteYourself__6VChainFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioRemoveParent__6VChainFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioChangeID__6VChainFPUc
	db 0h,0h,0h,0h
	dd offset FLAT:ioSync__6VChainFv
	db 0h,0h,0h,0h
	dd offset FLAT:queryWhere__5VRDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:isArray__6VChainFv
	db 0h,0h,0h,0h
	dd offset FLAT:isHostdrive__6VChainFv
	db 0h,0h,0h,0h
	dd offset FLAT:allocateDescription__6VChainFv
	db 0h,0h,0h,0h
	dd offset FLAT:queryIcon__6VChainFv
	db 0h,0h,0h,0h
	dd offset FLAT:setHostdrive__6VChainFUl
	db 0h,0h,0h,0h
	dd offset FLAT:addChild__6VChainFP5VRDev7BooleanT2
	db 0h,0h,0h,0h
	dd offset FLAT:queryChildren__6VChainFv
	db 0h,0h,0h,0h
	dd offset FLAT:getChild__6VChainFi
	db 0h,0h,0h,0h
	dd offset FLAT:isValidChild__7VRDriveFi
	db 0h,0h,0h,0h
	dd offset FLAT:isValidChild__7VRDriveFP5VRDev
	db 0h,0h,0h,0h
@bd__vft7VRDrive5VDisk	dd 0
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__dt__7VRDriveFv
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:queryWhere__5VRDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:isValidChild__7VRDriveFi
	db 0h,0h,0h,0h
	dd offset FLAT:isValidChild__7VRDriveFP5VRDev
	db 0h,0h,0h,0h
@eb__vft5VRDev5VDisk	dd 0
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__dt__5VRDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:queryWhere__5VRDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
@f2__vft5VDisk	dd 0
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
_VFT	ends
CODE32	segment

; 554 VChain::querySize()

	public querySize__6VChainFv
querySize__6VChainFv	proc

; 556     return size;
	mov	eax,[eax+011ch]
	ret	
querySize__6VChainFv	endp

; 258 VChain::setHostdrive(ULONG os2idx)

	public setHostdrive__6VChainFUl
setHostdrive__6VChainFUl	proc

; 261     os2access = os2idx;
	mov	[eax+014h],edx

; 260     hostdrive = True;
	mov	byte ptr [eax+010h],01h

; 262 }
	ret	
setHostdrive__6VChainFUl	endp

; 170 VRDev *

	public getChild__6VChainFi
getChild__6VChainFi	proc

; 173     if( index >= children )
	cmp	[eax+018h],edx
	jg	@BLBL11

; 174 	return 0;
	xor	eax,eax
	ret	
@BLBL11:

; 175     return child[index].rdev;
	mov	eax,dword ptr [eax+edx*08h+01ch]
	ret	
getChild__6VChainFi	endp

; 150 VChain::queryChildren()

	public queryChildren__6VChainFv
queryChildren__6VChainFv	proc

; 152     return children;
	mov	eax,[eax+018h]
	ret	
queryChildren__6VChainFv	endp

; 626 VChain::write(ULONG offset,ULONG count,PVOID buffer)

	public write__6VChainFUlT1Pv
write__6VChainFUlT1Pv	proc
	push	ebp
	xchg	ecx,eax
	mov	ebp,esp
	sub	esp,0ch
	mov	[ebp+010h],eax;	count
	push	ebx
	mov	[ebp+08h],ecx;	this
	push	edi

; 628     APIRET	rc = 0;
	mov	dword ptr [ebp-04h],0h;	rc

; 629 
; 630     for( int i = 0; i < children; ++i )
	cmp	dword ptr [ecx+018h],0h

; 626 VChain::write(ULONG offset,ULONG count,PVOID buffer)
	push	esi

; 630     for( int i = 0; i < children; ++i )
	jle	@BLBL85
	inc	edx
	mov	[ebp-08h],edx;	@CBE18
	mov	edx,ecx
	mov	[ebp+08h],ecx;	this
	mov	ecx,[ebp+014h];	buffer
	mov	[ebp+010h],eax;	count
	mov	ebx,eax
	xor	esi,esi
	xor	edi,edi
@BLBL86:
	mov	eax,edx
	xchg	eax,ebx

; 631 	rc |= child[i].rdev->write(offset+1, count, buffer);
	push	ecx

; 630     for( int i = 0; i < children; ++i )
	mov	[ebp-0ch],esi;	@CBE19

; 631 	rc |= child[i].rdev->write(offset+1, count, buffer);
	mov	ecx,eax
	mov	eax,dword ptr [ebx+edi*08h+01ch]
	sub	esp,0ch
	mov	ebx,[eax]
	mov	edx,[ebp-08h];	@CBE18
	mov	esi,[ebx+024h]
	add	eax,esi
	mov	esi,[ebp-0ch];	@CBE19
	call	dword ptr [ebx+020h]
	mov	edx,[ebp+08h];	this
	mov	ecx,[ebp+014h];	buffer
	mov	ebx,[ebp+010h];	count
	or	eax,esi

; 630     for( int i = 0; i < children; ++i )
	inc	edi

; 631 	rc |= child[i].rdev->write(offset+1, count, buffer);
	add	esp,010h

; 630     for( int i = 0; i < children; ++i )
	cmp	[edx+018h],edi

; 631 	rc |= child[i].rdev->write(offset+1, count, buffer);
	mov	esi,eax

; 630     for( int i = 0; i < children; ++i )
	jg	@BLBL86
	mov	[ebp-04h],esi;	rc
@BLBL85:

; 632     return rc;
	pop	esi
	mov	eax,[ebp-04h];	rc
	pop	edi
	pop	ebx
	leave	
	ret	
write__6VChainFUlT1Pv	endp

; 580 VChain::read(ULONG offset,ULONG count,PVOID buffer)

	public read__6VChainFUlT1Pv
read__6VChainFUlT1Pv	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi
	mov	edi,edx
	push	esi
	mov	[ebp+010h],ecx;	count
	sub	esp,020h

; 582     PUCHAR	copybuf = new UCHAR[count];
	mov	eax,ecx
	call	__vn__FUi
	mov	ecx,ebx

; 580 VChain::read(ULONG offset,ULONG count,PVOID buffer)
	mov	[ebp+08h],ebx;	this

; 582     PUCHAR	copybuf = new UCHAR[count];
	mov	[ebp-018h],eax;	copybuf
	mov	edx,edi

; 583     int		goodchildren = 0;
	mov	dword ptr [ebp-010h],0h;	goodchildren

; 584     APIRET	rc = VERROR_NO_CONTENTS;
	mov	dword ptr [ebp-014h],0ff00h;	rc

; 586     for( int i = 0; i < children; ++i )
	cmp	dword ptr [ecx+018h],0h
	jle	@BLBL71
	inc	edx
	mov	[ebp-01ch],edx;	@CBE20
	mov	[ebp-018h],eax;	copybuf
	mov	ebx,eax
	xor	esi,esi
	xor	edi,edi
@BLBL72:

; 588 	if( !child[i].cfgok )
	mov	eax,[ebp+08h];	this
	lea	eax,dword ptr [eax+edi*08h]
	cmp	byte ptr [eax+020h],0h
	je	@BLBL74
	mov	[ebp-020h],esi;	@CBE21

; 591 	rc = child[i].rdev->read(offset+1, count, buffer);
	mov	eax,[eax+01ch]
	mov	edx,[ebp+014h];	buffer
	push	edx
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edx,[ebp+010h];	count
	mov	esi,[ebx+01ch]
	mov	ecx,edx
	add	eax,esi
	mov	edx,[ebp-01ch];	@CBE20
	mov	esi,[ebp-020h];	@CBE21
	call	dword ptr [ebx+018h]
	mov	ebx,[ebp-018h];	copybuf
	add	esp,010h
	mov	edx,[ebp+014h];	buffer
	mov	ecx,[ebp+010h];	count
	mov	[ebp-014h],eax;	rc

; 592 	if( rc != 0 )
	test	eax,eax
	jne	@BLBL74

; 594 	if( goodchildren == 0 )
	test	esi,esi
	jne	@BLBL76

; 595 	    memcpy(copybuf, buffer, (size_t)count);
	mov	eax,ebx
	call	memcpy
	jmp	@BLBL77
@BLBL76:

; 596 	else if( memcmp(copybuf, buffer, (size_t)count) != 0 )
	mov	ecx,[ebp+010h];	count
	mov	edx,[ebp+014h];	buffer
	mov	eax,ebx
	call	memcmp
	test	eax,eax
	je	@BLBL77

; 597 	    Verbose(0, "VChain", "Data error when reading child %d, ignored", i);
	push	edi
	mov	ecx,offset FLAT:@CBE12
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE11
	xor	eax,eax
	call	Verbose
	add	esp,010h
@BLBL77:

; 598 	++goodchildren;
	inc	esi

; 599     }
@BLBL74:

; 586     for( int i = 0; i < children; ++i )
	mov	eax,[ebp+08h];	this
	inc	edi
	cmp	[eax+018h],edi
	jg	@BLBL72
	mov	[ebp-010h],esi;	goodchildren
@BLBL71:

; 601     delete[] copybuf;
	mov	eax,[ebp-018h];	copybuf
	call	__vd__FPv

; 602     return (goodchildren == 0 ? rc : 0);
	cmp	dword ptr [ebp-010h],0h;	goodchildren
	sete	al
	movzx	eax,al
	neg	eax
	and	eax,[ebp-014h];	rc
	add	esp,020h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
read__6VChainFUlT1Pv	endp

; 483 VChain::ioSync()

	public ioSync__6VChainFv
ioSync__6VChainFv	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi

; 485     PSEC_VRDEV	sec = new SEC_VRDEV;
	mov	eax,0200h

; 483 VChain::ioSync()
	push	esi
	mov	[ebp+08h],ebx;	this
	sub	esp,018h

; 487     int		errors = 0;
	mov	dword ptr [ebp-014h],0h;	errors

; 485     PSEC_VRDEV	sec = new SEC_VRDEV;
	call	__nw__FUi
	mov	edi,eax

; 492     memset(sec, 0, sizeof(*sec));
	mov	ecx,0200h
	xor	edx,edx
	call	memset

; 493     memcpy(sec->sectype, "VRAIDDEVICE     ", 16);
	mov	ecx,010h
	mov	edx,offset FLAT:@CBE8
	mov	eax,edi
	call	memcpy

; 494     sec->timestamp = time(NULL);
	xor	eax,eax
	call	time

; 485     PSEC_VRDEV	sec = new SEC_VRDEV;
	mov	[ebp-010h],edi;	sec

; 494     sec->timestamp = time(NULL);
	mov	[edi+010h],eax

; 496     memcpy(sec->u.s.id, id, sizeof(DEVID));
	mov	ecx,06h
	lea	edx,[ebx+08h]
	lea	eax,[edi+014h]
	call	memcpy
	mov	eax,ebx

; 497     sec->u.s.type = RDTYPE_CHAIN;
	mov	byte ptr [edi+01ah],02h

; 498     sec->u.s.flags = (UCHAR)(parent != 0 ? 0 : 0x80);
	cmp	dword ptr [eax+04h],0h
	setne	cl
	movzx	ecx,cl
	dec	ecx
	and	ecx,080h
	mov	[edi+01bh],cl

; 500     sec->u.s.children = children;
	mov	ecx,[eax+018h]
	mov	[edi+01ch],cx

; 507     for( i = 0; i < children; ++i )
	cmp	dword ptr [eax+018h],0h

; 506     size = 0;
	mov	dword ptr [eax+011ch],0h

; 507     for( i = 0; i < children; ++i )
	jle	@BLBL56
	mov	esi,edi
	xor	edi,edi
@BLBL57:

; 509 	sec->u.s.child[i].size = child[i].rdev->querySize();
	movzx	edi,di
	mov	eax,dword ptr [ebx+edi*08h+01ch]
	mov	ecx,[eax]
	mov	edx,[ecx+014h]
	add	eax,edx
	call	dword ptr [ecx+010h]
	imul	ecx,edi,0ah
	mov	dword ptr [esi+ecx+024h],eax

; 510 	size += sec->u.s.child[i].size;
	add	[ebx+011ch],eax

; 507     for( i = 0; i < children; ++i )
	inc	edi
	movzx	ecx,di
	cmp	[ebx+018h],ecx
	jg	@BLBL57
@BLBL56:

; 516     for( i = 0; i < children; ++i )
	mov	ebx,[ebp+08h];	this
	cmp	dword ptr [ebx+018h],0h
	jle	@BLBL61
	mov	esi,[ebp-010h];	sec
	xor	edi,edi
@BLBL62:

; 518 	child[i].rdev->ioSync();
	movzx	edi,di
	lea	ecx,dword ptr [ebx+edi*08h]
	mov	eax,[ecx+01ch]
	mov	[ebp-018h],ecx;	@CBE22
	mov	ecx,[eax]
	mov	edx,[ecx+054h]
	add	eax,edx
	call	dword ptr [ecx+050h]
	mov	edx,[ebp-018h];	@CBE22

; 519 	memcpy(sec->u.s.child[i].id, child[i].rdev->queryID(), sizeof(DEVID));
	imul	eax,edi,0ah
	mov	edx,[edx+01ch]
	add	eax,esi
	mov	ecx,06h
	add	edx,08h
	add	eax,01eh
	call	memcpy

; 516     for( i = 0; i < children; ++i )
	inc	edi
	movzx	ecx,di
	cmp	[ebx+018h],ecx
	jg	@BLBL62
@BLBL61:

; 525     sec->crc = Crc16(sec, SECTOR_SIZE-2);
	mov	ebx,[ebp-010h];	sec
	mov	dx,01feh
	mov	eax,ebx
	call	Crc16__FPvUs
	mov	[ebx+01feh],ax

; 526     APIRET rc = write(0, 1, sec);
	mov	eax,[ebp+08h];	this
	push	ebx
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+024h]
	mov	ecx,01h
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+020h]
	add	esp,010h

; 527     if( rc != 0 )
	test	eax,eax
	je	@BLBL66

; 529 	Verbose(1, "VChain::ioSync", "write(0,1,...) - rc %lu, not updated", rc);
	push	eax

; 530 	++errors;
	inc	dword ptr [ebp-014h];	errors

; 529 	Verbose(1, "VChain::ioSync", "write(0,1,...) - rc %lu, not updated", rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE10
	mov	edx,offset FLAT:@CBE9
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 531     }
@BLBL66:

; 533     delete sec;
	mov	eax,[ebp-010h];	sec
	call	__dl__FPv

; 534     return errors;
	mov	eax,[ebp-014h];	errors
	add	esp,018h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ioSync__6VChainFv	endp

; 429 VChain::ioChangeID(DEVID newid)

	public ioChangeID__6VChainFPUc
ioChangeID__6VChainFPUc	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,edx
	push	edi
	mov	edi,eax
	push	esi

; 431     PSEC_VRDEV	sec = new SEC_VRDEV;
	mov	eax,0200h

; 429 VChain::ioChangeID(DEVID newid)
	sub	esp,014h
	mov	[ebp+0ch],ebx;	newid

; 431     PSEC_VRDEV	sec = new SEC_VRDEV;
	call	__nw__FUi
	mov	esi,eax
	mov	edx,ebx

; 434     memcpy(id, newid, sizeof(DEVID));		/* update object */
	mov	ecx,06h
	lea	eax,[edi+08h]
	call	memcpy
	mov	eax,edi

; 440 	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
	push	esi

; 429 VChain::ioChangeID(DEVID newid)
	mov	[ebp+08h],edi;	this

; 431     PSEC_VRDEV	sec = new SEC_VRDEV;
	mov	[ebp-014h],esi;	sec

; 440 	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
	mov	ecx,01h

; 432     int		errors = 0;
	mov	dword ptr [ebp-010h],0h;	errors

; 440 	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+01ch]
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+018h]
	add	esp,010h

; 441 	if( rc != 0 )
	test	eax,eax
	je	@BLBL47

; 443 	    Verbose(1,"VChain::ioChangeID", "can't read own SEC_VRDEV - rc %lu", rc);
	push	eax

; 444 	    ++errors;
	mov	dword ptr [ebp-010h],01h;	errors

; 443 	    Verbose(1,"VChain::ioChangeID", "can't read own SEC_VRDEV - rc %lu", rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE5
	mov	edx,offset FLAT:@CBE7
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 445 	    break;
	jmp	@BLBL48
@BLBL47:

; 448 	memcpy(sec->u.s.id, newid, sizeof(DEVID));
	mov	ebx,[ebp-014h];	sec
	mov	edx,[ebp+0ch];	newid
	mov	ecx,06h
	lea	eax,[ebx+014h]
	call	memcpy

; 449 	sec->timestamp = time(NULL);		/* UTC of change */
	xor	eax,eax
	call	time
	mov	[ebx+010h],eax

; 450 	sec->crc = Crc16(sec, SECTOR_SIZE-2);	/* update CRC!!! */
	mov	dx,01feh
	mov	eax,ebx
	call	Crc16__FPvUs
	mov	[ebx+01feh],ax

; 452 	rc = write(0, 1, sec);
	mov	eax,[ebp+08h];	this
	push	ebx
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+024h]
	mov	ecx,01h
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+020h]
	add	esp,010h

; 453 	if( rc != 0 )
	test	eax,eax
	je	@BLBL48

; 455 	    Verbose(1,"VChain::ioChangeID","can't write own SEC_VRDEV - rc %lu", rc);
	push	eax

; 456 	    ++errors;
	inc	dword ptr [ebp-010h];	errors

; 455 	    Verbose(1,"VChain::ioChangeID","can't write own SEC_VRDEV - rc %lu", rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE6
	mov	edx,offset FLAT:@CBE7
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 457 	    break;
@BLBL48:

; 462     delete sec;
	mov	eax,[ebp-014h];	sec
	call	__dl__FPv

; 463     return errors;
	mov	eax,[ebp-010h];	errors
	add	esp,014h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ioChangeID__6VChainFPUc	endp

; 362 VChain::ioRemoveParent()

	public ioRemoveParent__6VChainFv
ioRemoveParent__6VChainFv	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi

; 364     PSEC_VRDEV	sec = new SEC_VRDEV;
	mov	eax,0200h

; 362 VChain::ioRemoveParent()
	push	esi
	mov	[ebp+08h],ebx;	this
	sub	esp,014h

; 365     int		errors = 0;
	xor	esi,esi

; 364     PSEC_VRDEV	sec = new SEC_VRDEV;
	call	__nw__FUi
	mov	edi,eax

; 371     memset(sec, 0, SECTOR_SIZE);
	mov	ecx,0200h
	xor	edx,edx
	call	memset
	mov	eax,ebx

; 372     rc = write(1, 1, sec);
	push	edi

; 364     PSEC_VRDEV	sec = new SEC_VRDEV;
	mov	[ebp-014h],edi;	sec

; 372     rc = write(1, 1, sec);
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+024h]
	mov	ecx,01h
	mov	edx,01h
	add	eax,edi
	call	dword ptr [ebx+020h]
	add	esp,010h

; 373     if( rc != 0 )
	test	eax,eax
	je	@BLBL38

; 375 	Verbose(1, "VChain::ioRemoveParent", "can't clear parent SEC_VRDEV - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE4
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,01h
	call	Verbose

; 376 	++errors;
	mov	esi,01h

; 375 	Verbose(1, "VChain::ioRemoveParent", "can't clear parent SEC_VRDEV - rc %lu", rc);
	add	esp,010h

; 382     do
@BLBL38:

; 384 	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
	push	dword ptr [ebp-014h];	sec
	mov	eax,[ebp+08h];	this
	sub	esp,0ch
	mov	ebx,[eax]
	mov	ecx,01h
	mov	edi,[ebx+01ch]
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+018h]
	add	esp,010h

; 385 	if( rc != 0 )
	test	eax,eax
	je	@BLBL39

; 387 	    Verbose(1, "VChain::ioRemoveParent", "can't read own SEC_VRDEV - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE5
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,01h
	call	Verbose

; 388 	    ++errors;
	inc	esi

; 387 	    Verbose(1, "VChain::ioRemoveParent", "can't read own SEC_VRDEV - rc %lu", rc);
	add	esp,010h

; 389 	    break;
	jmp	@BLBL40
@BLBL39:

; 392 	sec->u.s.flags |= RDFLAG_HOSTDRIVE;	/* no parent means hostdrive */
	mov	ebx,[ebp-014h];	sec

; 393 	sec->timestamp = time(NULL);		/* UTC of change */
	xor	eax,eax

; 392 	sec->u.s.flags |= RDFLAG_HOSTDRIVE;	/* no parent means hostdrive */
	or	byte ptr [ebx+01bh],080h

; 393 	sec->timestamp = time(NULL);		/* UTC of change */
	call	time
	mov	[ebx+010h],eax

; 394 	sec->crc = Crc16(sec, SECTOR_SIZE-2);	/* update CRC!!! */
	mov	dx,01feh
	mov	eax,ebx
	call	Crc16__FPvUs
	mov	[ebx+01feh],ax

; 396 	rc = write(0, 1, sec);
	mov	eax,[ebp+08h];	this
	push	ebx
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+024h]
	mov	ecx,01h
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+020h]
	add	esp,010h

; 397 	if( rc != 0 )
	test	eax,eax
	je	@BLBL41

; 399 	    Verbose(1, "VChain::ioRemoveParent", "can't write own SEC_VRDEV - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE6
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,01h
	call	Verbose

; 400 	    ++errors;
	inc	esi

; 399 	    Verbose(1, "VChain::ioRemoveParent", "can't write own SEC_VRDEV - rc %lu", rc);
	add	esp,010h

; 401 	    break;
	jmp	@BLBL40
@BLBL41:

; 404 	parent = NULL;				/* none */
	mov	eax,[ebp+08h];	this
	mov	dword ptr [eax+04h],0h

; 406     while(0);
@BLBL40:

; 408     delete sec;
	mov	eax,[ebp-014h];	sec
	call	__dl__FPv

; 409     return errors;
	add	esp,014h
	mov	eax,esi
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ioRemoveParent__6VChainFv	endp

; 324 VChain::ioDeleteYourself()

	public ioDeleteYourself__6VChainFv
ioDeleteYourself__6VChainFv	proc
	push	ebx
	push	edi
	push	esi
	sub	esp,0ch

; 327     int		errors = 0;
	mov	dword ptr [esp+08h],0h;	errors

; 332     for( i = 0; i < children; ++i )
	cmp	dword ptr [eax+018h],0h
	jle	@BLBL29
	xor	esi,esi
	xor	edi,edi
@BLBL30:

; 334 	if( child[i].rdev != NULL )
	lea	ecx,dword ptr [eax+edi*08h]
	mov	ebx,[ecx+01ch]
	mov	[esp+04h],ecx;	@CBE23
	test	ebx,ebx
	je	@BLBL32
	xchg	ebx,eax

; 336 	    errors += child[i].rdev->ioRemoveParent();
	mov	ecx,[eax]
	mov	edx,[ecx+044h]
	add	eax,edx
	call	dword ptr [ecx+040h]
	mov	edx,[esp+04h];	@CBE23
	add	eax,esi
	mov	esi,eax

; 337 	    child[i].rdev = NULL;		/* we don't need pointer anymore */
	mov	dword ptr [edx+01ch],0h

; 339     }
	mov	eax,ebx
@BLBL32:

; 332     for( i = 0; i < children; ++i )
	inc	edi
	cmp	[eax+018h],edi
	jg	@BLBL30
	mov	[esp+08h],esi;	errors
@BLBL29:

; 341     return errors;
	mov	eax,[esp+08h];	errors
	add	esp,0ch
	pop	esi
	pop	edi
	pop	ebx
	ret	
ioDeleteYourself__6VChainFv	endp

; 228 VChain::queryIcon()

	public queryIcon__6VChainFv
queryIcon__6VChainFv	proc
	push	ebx

; 230     if( parent != 0  &&  parent->isValidChild(this) == False )
	mov	ebx,[eax+04h]

; 228 VChain::queryIcon()
	push	edi
	sub	esp,08h

; 230     if( parent != 0  &&  parent->isValidChild(this) == False )
	test	ebx,ebx
	je	@BLBL18
	xchg	ebx,eax
	mov	edx,ebx
	mov	ecx,[eax]
	mov	edi,[ecx+0ach]
	add	eax,edi
	call	dword ptr [ecx+0a8h]
	test	al,al
	mov	eax,ebx
	jne	@BLBL18

; 231 	return inv_icon;
	mov	eax,[eax+0124h]
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
@BLBL18:

; 232     else if( writable == True )
	cmp	byte ptr [eax+0120h],01h
	jne	@BLBL20

; 233 	return rw_icon;
	mov	eax,[eax+0128h]
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
@BLBL20:

; 235 	return ro_icon;
	mov	eax,[eax+012ch]
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
queryIcon__6VChainFv	endp

; 194 char *

	public allocateDescription__6VChainFv
allocateDescription__6VChainFv	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi

; 197     char * descr = new char[80];
	mov	eax,050h

; 194 char *
	push	esi
	sub	esp,01ch

; 197     char * descr = new char[80];
	call	__vn__FUi
	mov	ecx,ebx
	mov	[ebp-010h],eax;	descr
	mov	[ebp-01ch],eax;	@CBE31

; 198     if( hostdrive )
	movzx	edx,byte ptr [ecx+08h]
	mov	[ebp-020h],edx;	@CBE32
	movzx	edx,byte ptr [ecx+09h]
	movzx	esi,byte ptr [ecx+0ah]
	movzx	edi,byte ptr [ecx+0bh]
	movzx	ebx,byte ptr [ecx+0ch]
	movzx	eax,byte ptr [ecx+0dh]
	mov	[ebp-014h],eax;	@CBE29
	mov	[ebp-024h],eax;	@CBE33
	mov	eax,[ecx+011ch]
	shr	eax,01h
	shr	eax,0ah
	mov	[ebp-018h],eax;	@CBE30
	cmp	byte ptr [ecx+010h],0h
	xchg	[ebp-020h],ecx;	@CBE32
	je	@BLBL14
	xchg	eax,ebx

; 199 	sprintf(descr, "Chaining (OS/2 Drive %lu)\n"
	push	ebx
	mov	ebx,[ebp-024h];	@CBE33
	push	ebx

; 198     if( hostdrive )
	xchg	edi,edx

; 199 	sprintf(descr, "Chaining (OS/2 Drive %lu)\n"
	push	eax
	mov	ebx,[ebp-020h];	@CBE32
	push	edx
	mov	eax,[ebp-01ch];	@CBE31
	push	esi
	mov	edx,offset FLAT:@CBE1
	push	edi
	push	ecx
	push	dword ptr [ebx+014h]
	sub	esp,08h
	call	_sprintfieee
	add	esp,028h

; 203 		size / 2 / 1024);
	jmp	@BLBL15
@BLBL14:

; 205 	sprintf(descr, "Chaining\n"
	push	dword ptr [ebp-018h];	@CBE30

; 203 		size / 2 / 1024);
	xchg	edx,ebx

; 205 	sprintf(descr, "Chaining\n"
	push	dword ptr [ebp-014h];	@CBE29
	mov	eax,[ebp-010h];	descr
	push	edx
	mov	edx,offset FLAT:@CBE2
	push	edi
	push	esi
	push	ebx
	push	ecx
	sub	esp,08h
	call	_sprintfieee
	add	esp,024h

; 208 		size / 2 / 1024);
@BLBL15:

; 209     return descr;
	mov	eax,[ebp-010h];	descr
	add	esp,01ch
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
allocateDescription__6VChainFv	endp

; 128 VChain::~VChain()

	public __dt__6VChainFv
__dt__6VChainFv	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffe0h
	sub	esp,018h

; 129 {
	mov	dword ptr [ebp-020h],0h;	__es

; 128 VChain::~VChain()
	push	ebx
	mov	ebx,eax
	push	edi
	mov	[ebp-024h],edx;	@CBE34
	push	esi
	mov	[ebp+08h],ebx;	this

; 129 {
	mov	dword ptr [ebp-018h],offset FLAT:@33__fsm_tab;	__es
	mov	[ebp-014h],ebx;	__es
	mov	dword ptr [ebp-010h],0h;	__es

; 128 VChain::~VChain()
	mov	dword ptr [ebp-020h],01h;	__es

; 130     WinDestroyPointer(rw_icon);
	push	dword ptr [ebx+0128h]

; 128 VChain::~VChain()
	mov	dword ptr [ebx],offset FLAT:__vft6VChain5VDisk

; 130     WinDestroyPointer(rw_icon);
	call	WinDestroyPointer
	add	esp,04h

; 131     WinDestroyPointer(ro_icon);
	push	dword ptr [ebx+012ch]
	call	WinDestroyPointer
	sub	esp,08h

; 132 }
	mov	dword ptr [ebp-020h],0h;	__es
	xor	ecx,ecx
	xor	edx,edx
	mov	eax,ebx
	call	__dt__7VRDriveFv
	mov	edx,[ebp-024h];	@CBE34
	add	esp,0ch
	mov	eax,ebx
	test	dl,01h
	je	@BLBL6
	sub	esp,04h
	call	__dl__FPv
	add	esp,04h
@BLBL6:
	pop	esi
	mov	eax,[ebp+08h];	this
	pop	edi
	pop	ebx
	add	esp,01ch
	pop	fs:[0h]
	leave	
	ret	
__dt__6VChainFv	endp

; 99 VChain::VChain(DEVID drive_id,int nchd)

	public __ct__6VChainFPUci
__ct__6VChainFPUci	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffe0h
	sub	esp,014h

; 100 {
	mov	dword ptr [ebp-020h],0h;	__es

; 99 VChain::VChain(DEVID drive_id,int nchd)
	push	ebx

; 100 {
	mov	dword ptr [ebp-018h],offset FLAT:@1c__fsm_tab;	__es

; 99 VChain::VChain(DEVID drive_id,int nchd)
	push	edi
	mov	ebx,eax
	push	esi
	mov	edi,edx

; 99 VChain::VChain(DEVID drive_id,int nchd)
	sub	esp,04h

; 100 {
	mov	[ebp-014h],ebx;	__es
	mov	dword ptr [ebp-010h],0h;	__es

; 99 VChain::VChain(DEVID drive_id,int nchd)
	call	__ct__7VRDriveFv
	sub	esp,08h
	mov	dword ptr [ebp-020h],01h;	__es
	mov	edx,edi
	mov	dword ptr [ebx],offset FLAT:__vft6VChain5VDisk

; 101     parent = NULL;
	mov	dword ptr [ebx+04h],0h

; 106     memcpy(id, drive_id, sizeof(DEVID));
	mov	ecx,06h

; 102     children = 0;
	mov	dword ptr [ebx+018h],0h

; 103     size = 0;
	mov	dword ptr [ebx+011ch],0h

; 106     memcpy(id, drive_id, sizeof(DEVID));
	lea	eax,[ebx+08h]

; 104     hostdrive = False;
	mov	byte ptr [ebx+010h],0h

; 105     writable = True;
	mov	byte ptr [ebx+0120h],01h

; 106     memcpy(id, drive_id, sizeof(DEVID));
	call	memcpy
	add	esp,0ch

; 108     inv_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_INVCHILDARRAY);
	push	0199h
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	[ebx+0124h],eax

; 109     rw_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_RAIDDRIVE);
	push	0194h
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	[ebx+0128h],eax

; 110     ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDARRAY);
	push	0197h
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	ecx,eax

; 111 }
	pop	esi

; 110     ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDARRAY);
	mov	eax,ebx

; 111 }
	pop	edi

; 110     ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDARRAY);
	mov	[eax+012ch],ecx

; 111 }
	pop	ebx
	add	esp,018h
	pop	fs:[0h]
	leave	
	ret	
__ct__6VChainFPUci	endp
CODE32	ends
EH_CODE	segment

__dftbdt__7VRDriveFv	proc
	xor	ecx,ecx
	sub	esp,0ch
	xor	edx,edx
	call	__dt__7VRDriveFv
	add	esp,0ch
	ret	
__dftbdt__7VRDriveFv	endp

; 74     Boolean	isWritable() { return writable; }
EH_CODE	ends
CODE32	segment

isWritable__6VChainFv	proc
	movzx	eax,byte ptr [eax+0120h]
	ret	
isWritable__6VChainFv	endp

; 65     Boolean	isHostdrive() { return hostdrive; }

isHostdrive__6VChainFv	proc
	movzx	eax,byte ptr [eax+010h]
	ret	
isHostdrive__6VChainFv	endp

; 64     UCHAR	isArray() { return RDTYPE_CHAIN; }

isArray__6VChainFv	proc
	mov	eax,02h
	ret	
isArray__6VChainFv	endp

; 57     void	forceUnwritable() { writable = False; }

forceUnwritable__6VChainFv	proc
	mov	byte ptr [eax+0120h],0h
	ret	
forceUnwritable__6VChainFv	endp

; 123     virtual	~VRDev() {}			// see Stroustroup pg216!!!

__dt__5VRDevFv	proc
	push	ebx
	mov	dword ptr [eax],offset FLAT:@eb__vft5VRDev5VDisk
	sub	esp,04h
	test	dl,01h
	je	@BLBL119
	mov	ebx,eax
	call	__dl__FPv
	mov	eax,ebx
@BLBL119:
	add	esp,04h
	pop	ebx
	ret	
__dt__5VRDevFv	endp

; 279 };

__dt__7VRDriveFv	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffe0h
	sub	esp,014h
	mov	dword ptr [ebp-020h],0h;	__es
	push	ebx
	mov	dword ptr [ebp-018h],offset FLAT:@d2__fsm_tab57;	__es
	push	edi
	mov	[ebp-014h],eax;	__es
	mov	dword ptr [ebp-010h],0h;	__es
	push	esi
	mov	dword ptr [eax],offset FLAT:@bd__vft7VRDrive5VDisk
	mov	dword ptr [ebp-020h],0h;	__es

; 123     virtual	~VRDev() {}			// see Stroustroup pg216!!!
	test	dl,01h
	mov	dword ptr [eax],offset FLAT:@eb__vft5VRDev5VDisk
	je	@BLBL103
	mov	ebx,eax
	sub	esp,04h
	call	__dl__FPv
	mov	eax,ebx
	add	esp,04h
@BLBL103:
	pop	esi
	pop	edi
	pop	ebx
	add	esp,018h
	pop	fs:[0h]
	leave	
	ret	
__dt__7VRDriveFv	endp
CODE32	ends
EH_CODE	segment

__dftbdt__5VRDevFv	proc
	mov	dword ptr [eax],offset FLAT:@eb__vft5VRDev5VDisk
	ret	
__dftbdt__5VRDevFv	endp

; 279 };
EH_CODE	ends
CODE32	segment

__ct__7VRDriveFv	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffe0h
	sub	esp,014h
	mov	dword ptr [ebp-020h],0h;	__es
	push	ebx
	mov	dword ptr [ebp-018h],offset FLAT:@bf__fsm_tab55;	__es
	push	edi
	mov	[ebp-014h],eax;	__es
	push	esi
	mov	dword ptr [ebp-010h],0h;	__es

; 279 };
	pop	esi

; 80 };
	mov	dword ptr [eax],offset FLAT:@eb__vft5VRDev5VDisk

; 279 };
	pop	edi

; 143 };
	mov	dword ptr [ebp-020h],01h;	__es

; 279 };
	pop	ebx

; 143 };
	mov	dword ptr [eax],offset FLAT:@bd__vft7VRDrive5VDisk

; 279 };
	add	esp,018h
	pop	fs:[0h]
	leave	
	ret	
__ct__7VRDriveFv	endp

; 278     virtual Boolean	isValidChild(VRDev * child) { return True; }

isValidChild__7VRDriveFP5VRDev	proc
	mov	eax,01h
	ret	
isValidChild__7VRDriveFP5VRDev	endp

; 277     virtual Boolean	isValidChild(int which) { return True; }

isValidChild__7VRDriveFi	proc
	mov	eax,01h
	ret	
isValidChild__7VRDriveFi	endp

; 137     virtual VOS2Disk *	queryWhere() { return NULL; }

queryWhere__5VRDevFv	proc
	xor	eax,eax
	ret	
queryWhere__5VRDevFv	endp

; 284 VChain::addChild(VRDev * newchild,Boolean cfgv,Boolean datav)

	public addChild__6VChainFP5VRDev7BooleanT2
addChild__6VChainFP5VRDev7BooleanT2	proc
	push	ebx
	mov	ebx,eax
	push	edi
	mov	edi,edx

; 290     child[children].rdev = newchild;
	mov	eax,[ebx+018h]
	mov	dword ptr [ebx+eax*08h+01ch],edi

; 291     child[children].cfgok = cfgv;
	mov	eax,[ebx+018h]
	mov	byte ptr [ebx+eax*08h+020h],cl

; 292     newchild->setParent(this);
; 293 
; 294     /* Update our object with information from child. */
; 295 
; 296     size += newchild->querySize();
	mov	ecx,[edi]

; 284 VChain::addChild(VRDev * newchild,Boolean cfgv,Boolean datav)
	sub	esp,04h

; 125     void		setParent(VRDrive * newparent) { parent = newparent; }
	mov	[edi+04h],ebx

; 296     size += newchild->querySize();
	mov	eax,[ecx+014h]
	add	eax,edi
	call	dword ptr [ecx+010h]
	mov	edx,edi
	add	[ebx+011ch],eax

; 297     if( newchild->isWritable() == False )
	mov	ecx,[edx]
	mov	eax,[ecx+0ch]
	add	edx,eax
	mov	eax,edx
	call	dword ptr [ecx+08h]
	test	al,al
	mov	eax,ebx
	jne	@BLBL26

; 298 	writable = False;			/* oups, it isn't 'changable' */
	mov	byte ptr [eax+0120h],0h
@BLBL26:

; 300     ++children;
	inc	dword ptr [eax+018h]

; 301 }
	add	esp,04h
	pop	edi
	pop	ebx
	ret	
addChild__6VChainFP5VRDev7BooleanT2	endp
CODE32	ends
end
