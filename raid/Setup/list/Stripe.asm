	title	Stripe.cpp
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
	public	__vft7VStripe5VDisk
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
@CBE1	db "RAID 0 (Striping) (OS/2 "
db "Drive %lu)",0ah,"ID %02X%02X%0"
db "2X%02X%02X%02X    %lu MB"
db "yte",0h
@CBE2	db "RAID 0 (Striping)",0ah,"ID %02"
db "X%02X%02X%02X%02X%02X   "
db " %lu MByte",0h
	align 04h
@CBE3	db "VStripe::ioRemoveParent",0h
@CBE4	db "can't clear parent SEC_V"
db "RDEV - rc %lu",0h
	align 04h
@CBE5	db "can't read own SEC_VRDEV"
db " - rc %lu",0h
	align 04h
@CBE6	db "can't write own SEC_VRDE"
db "V - rc %lu",0h
	align 04h
@CBE7	db "VStripe::ioChangeID",0h
@CBE8	db "VRAIDDEVICE     ",0h
	align 04h
@CBE9	db "VStripe::ioSync",0h
@CBE10	db "write(0,1,...) - rc %lu,"
db " not updated",0h
	align 04h
@CBE11	db "VStripe",0h
@CBE12	db "Data error when reading "
db "child %d, ignored",0h
	align 04h
@1vcid	db "$Id: stripe.cpp,v 1.13 2"
db "000/04/10 00:51:51 vitus"
db " Exp $",0h
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
@c4__fsm_tab56	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__5VRDevFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@d7__fsm_tab58	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__5VRDevFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
EH_DATA	ends
_VFT	segment
__vft7VStripe5VDisk	dd 0
	db 0h,0h,0h,0h
	dd offset FLAT:isWritable__7VStripeFv
	db 0h,0h,0h,0h
	dd offset FLAT:querySize__7VStripeFv
	db 0h,0h,0h,0h
	dd offset FLAT:read__7VStripeFUlT1Pv
	db 0h,0h,0h,0h
	dd offset FLAT:write__7VStripeFUlT1Pv
	db 0h,0h,0h,0h
	dd offset FLAT:__dt__7VStripeFv
	db 0h,0h,0h,0h
	dd offset FLAT:forceUnwritable__7VStripeFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioDeleteYourself__7VStripeFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioRemoveParent__7VStripeFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioChangeID__7VStripeFPUc
	db 0h,0h,0h,0h
	dd offset FLAT:ioSync__7VStripeFv
	db 0h,0h,0h,0h
	dd offset FLAT:queryWhere__5VRDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:isArray__7VStripeFv
	db 0h,0h,0h,0h
	dd offset FLAT:isHostdrive__7VStripeFv
	db 0h,0h,0h,0h
	dd offset FLAT:allocateDescription__7VStripeFv
	db 0h,0h,0h,0h
	dd offset FLAT:queryIcon__7VStripeFv
	db 0h,0h,0h,0h
	dd offset FLAT:setHostdrive__7VStripeFUl
	db 0h,0h,0h,0h
	dd offset FLAT:addChild__7VStripeFP5VRDev7BooleanT2
	db 0h,0h,0h,0h
	dd offset FLAT:queryChildren__7VStripeFv
	db 0h,0h,0h,0h
	dd offset FLAT:getChild__7VStripeFi
	db 0h,0h,0h,0h
	dd offset FLAT:isValidChild__7VRDriveFi
	db 0h,0h,0h,0h
	dd offset FLAT:isValidChild__7VRDriveFP5VRDev
	db 0h,0h,0h,0h
@c2__vft7VRDrive5VDisk	dd 0
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
@f0__vft5VRDev5VDisk	dd 0
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
@f7__vft5VDisk	dd 0
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

; 573 VStripe::querySize()

	public querySize__7VStripeFv
querySize__7VStripeFv	proc

; 575     return size;
	mov	eax,[eax+011ch]
	ret	
querySize__7VStripeFv	endp

; 263 VStripe::setHostdrive(ULONG os2idx)

	public setHostdrive__7VStripeFUl
setHostdrive__7VStripeFUl	proc

; 266     os2access = os2idx;
	mov	[eax+014h],edx

; 265     hostdrive = True;
	mov	byte ptr [eax+010h],01h

; 267 }
	ret	
setHostdrive__7VStripeFUl	endp

; 175 VRDev *

	public getChild__7VStripeFi
getChild__7VStripeFi	proc

; 178     if( index >= children )
	cmp	[eax+018h],edx
	jg	@BLBL11

; 179 	return 0;
	xor	eax,eax
	ret	
@BLBL11:

; 180     return child[index].rdev;
	mov	eax,dword ptr [eax+edx*08h+01ch]
	ret	
getChild__7VStripeFi	endp

; 154 VStripe::queryChildren()

	public queryChildren__7VStripeFv
queryChildren__7VStripeFv	proc

; 156     return children;
	mov	eax,[eax+018h]
	ret	
queryChildren__7VStripeFv	endp

; 648 VStripe::write(ULONG offset,ULONG count,PVOID buffer)

	public write__7VStripeFUlT1Pv
write__7VStripeFUlT1Pv	proc
	push	ebp
	xchg	ecx,eax
	mov	ebp,esp
	sub	esp,0ch
	mov	[ebp+010h],eax;	count
	push	ebx
	mov	[ebp+08h],ecx;	this
	push	edi

; 650     APIRET	rc = 0;
	mov	dword ptr [ebp-04h],0h;	rc

; 651 
; 652     for( int i = 0; i < children; ++i )
	cmp	dword ptr [ecx+018h],0h

; 648 VStripe::write(ULONG offset,ULONG count,PVOID buffer)
	push	esi

; 652     for( int i = 0; i < children; ++i )
	jle	@BLBL98
	inc	edx
	mov	[ebp-08h],edx;	@CBE17
	mov	edx,ecx
	mov	[ebp+08h],ecx;	this
	mov	ecx,[ebp+014h];	buffer
	mov	[ebp+010h],eax;	count
	mov	ebx,eax
	xor	esi,esi
	xor	edi,edi
@BLBL99:
	mov	eax,edx
	xchg	eax,ebx

; 653 	rc |= child[i].rdev->write(offset+1, count, buffer);
	push	ecx

; 652     for( int i = 0; i < children; ++i )
	mov	[ebp-0ch],esi;	@CBE18

; 653 	rc |= child[i].rdev->write(offset+1, count, buffer);
	mov	ecx,eax
	mov	eax,dword ptr [ebx+edi*08h+01ch]
	sub	esp,0ch
	mov	ebx,[eax]
	mov	edx,[ebp-08h];	@CBE17
	mov	esi,[ebx+024h]
	add	eax,esi
	mov	esi,[ebp-0ch];	@CBE18
	call	dword ptr [ebx+020h]
	mov	edx,[ebp+08h];	this
	mov	ecx,[ebp+014h];	buffer
	mov	ebx,[ebp+010h];	count
	or	eax,esi

; 652     for( int i = 0; i < children; ++i )
	inc	edi

; 653 	rc |= child[i].rdev->write(offset+1, count, buffer);
	add	esp,010h

; 652     for( int i = 0; i < children; ++i )
	cmp	[edx+018h],edi

; 653 	rc |= child[i].rdev->write(offset+1, count, buffer);
	mov	esi,eax

; 652     for( int i = 0; i < children; ++i )
	jg	@BLBL99
	mov	[ebp-04h],esi;	rc
@BLBL98:

; 654     return rc;
	pop	esi
	mov	eax,[ebp-04h];	rc
	pop	edi
	pop	ebx
	leave	
	ret	
write__7VStripeFUlT1Pv	endp

; 601 VStripe::read(ULONG offset,ULONG count,PVOID buffer)

	public read__7VStripeFUlT1Pv
read__7VStripeFUlT1Pv	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi
	mov	edi,edx
	push	esi
	mov	[ebp+010h],ecx;	count
	sub	esp,020h

; 603     PUCHAR	copybuf = new UCHAR[count];
	mov	eax,ecx
	call	__vn__FUi
	mov	ecx,ebx

; 601 VStripe::read(ULONG offset,ULONG count,PVOID buffer)
	mov	[ebp+08h],ebx;	this

; 603     PUCHAR	copybuf = new UCHAR[count];
	mov	[ebp-018h],eax;	copybuf
	mov	edx,edi

; 604     int		goodchildren = 0;
	mov	dword ptr [ebp-010h],0h;	goodchildren

; 605     APIRET	rc = VERROR_NO_CONTENTS;
	mov	dword ptr [ebp-014h],0ff00h;	rc

; 607     for( int i = 0; i < children; ++i )
	cmp	dword ptr [ecx+018h],0h
	jle	@BLBL84
	inc	edx
	mov	[ebp-01ch],edx;	@CBE19
	mov	[ebp-018h],eax;	copybuf
	mov	ebx,eax
	xor	esi,esi
	xor	edi,edi
@BLBL85:

; 609 	if( !child[i].cfgok )
	mov	eax,[ebp+08h];	this
	lea	eax,dword ptr [eax+edi*08h]
	cmp	byte ptr [eax+020h],0h
	je	@BLBL87
	mov	[ebp-020h],esi;	@CBE20

; 612 	rc = child[i].rdev->read(offset+1, count, buffer);
	mov	eax,[eax+01ch]
	mov	edx,[ebp+014h];	buffer
	push	edx
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edx,[ebp+010h];	count
	mov	esi,[ebx+01ch]
	mov	ecx,edx
	add	eax,esi
	mov	edx,[ebp-01ch];	@CBE19
	mov	esi,[ebp-020h];	@CBE20
	call	dword ptr [ebx+018h]
	mov	ebx,[ebp-018h];	copybuf
	add	esp,010h
	mov	edx,[ebp+014h];	buffer
	mov	ecx,[ebp+010h];	count
	mov	[ebp-014h],eax;	rc

; 613 	if( rc != 0 )
	test	eax,eax
	jne	@BLBL87

; 615 	if( goodchildren == 0 )
	test	esi,esi
	jne	@BLBL89

; 616 	    memcpy(copybuf, buffer, (size_t)count);
	mov	eax,ebx
	call	memcpy
	jmp	@BLBL90
@BLBL89:

; 617 	else if( memcmp(copybuf, buffer, (size_t)count) != 0 )
	mov	ecx,[ebp+010h];	count
	mov	edx,[ebp+014h];	buffer
	mov	eax,ebx
	call	memcmp
	test	eax,eax
	je	@BLBL90

; 618 	    Verbose(0, "VStripe", "Data error when reading child %d, ignored", i);
	push	edi
	mov	ecx,offset FLAT:@CBE12
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE11
	xor	eax,eax
	call	Verbose
	add	esp,010h
@BLBL90:

; 619 	++goodchildren;
	inc	esi

; 620     }
@BLBL87:

; 607     for( int i = 0; i < children; ++i )
	mov	eax,[ebp+08h];	this
	inc	edi
	cmp	[eax+018h],edi
	jg	@BLBL85
	mov	[ebp-010h],esi;	goodchildren
@BLBL84:

; 622     delete[] copybuf;
	mov	eax,[ebp-018h];	copybuf
	call	__vd__FPv

; 623     return (goodchildren == 0 ? rc : 0);
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
read__7VStripeFUlT1Pv	endp

; 496 VStripe::ioSync()

	public ioSync__7VStripeFv
ioSync__7VStripeFv	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi

; 498     PSEC_VRDEV	sec = new SEC_VRDEV;
	mov	eax,0200h

; 496 VStripe::ioSync()
	push	esi
	mov	[ebp+08h],ebx;	this
	sub	esp,020h

; 501     int		errors = 0;
	mov	dword ptr [ebp-014h],0h;	errors

; 498     PSEC_VRDEV	sec = new SEC_VRDEV;
	call	__nw__FUi
	mov	edi,eax

; 506     memset(sec, 0, sizeof(*sec));
	mov	ecx,0200h
	xor	edx,edx
	call	memset

; 507     memcpy(sec->sectype, "VRAIDDEVICE     ", 16);
	mov	ecx,010h
	mov	edx,offset FLAT:@CBE8
	mov	eax,edi
	call	memcpy

; 508     sec->timestamp = time(NULL);
	xor	eax,eax
	call	time

; 498     PSEC_VRDEV	sec = new SEC_VRDEV;
	mov	[ebp-010h],edi;	sec

; 508     sec->timestamp = time(NULL);
	mov	[edi+010h],eax

; 510     memcpy(sec->u.s.id, id, sizeof(DEVID));
	mov	ecx,06h
	lea	edx,[ebx+08h]
	lea	eax,[edi+014h]
	call	memcpy
	mov	eax,ebx

; 511     sec->u.s.type = RDTYPE_STRIPE;
	mov	byte ptr [edi+01ah],03h

; 512     sec->u.s.flags = (UCHAR)(parent != 0 ? 0 : 0x80);
	cmp	dword ptr [eax+04h],0h
	setne	cl
	movzx	ecx,cl
	dec	ecx
	and	ecx,080h
	mov	[edi+01bh],cl

; 514     sec->u.s.children = children;
	mov	ecx,[eax+018h]
	mov	[edi+01ch],cx

; 520     size = 0;
	mov	dword ptr [eax+011ch],0h

; 521     ul = (ULONG)ULONG_MAX;
	mov	dword ptr [ebp-018h],0ffffffffh;	ul

; 522     for( i = 0; i < children; ++i )
	cmp	dword ptr [eax+018h],0h
	jle	@BLBL62
	mov	esi,[ebp-01ch];	__50
	mov	[ebp+08h],eax;	this
	or	ebx,0ffffffffh
	xor	edi,edi
@BLBL63:
	mov	ecx,eax

; 524 	ul = min(ul, child[i].rdev->querySize());
	movzx	edx,di
	mov	eax,dword ptr [ecx+edx*08h+01ch]
	mov	ecx,[eax]
	mov	edx,[ecx+014h]
	add	eax,edx
	call	dword ptr [ecx+010h]
	mov	ecx,eax
	mov	eax,[ebp+08h];	this
	cmp	ecx,ebx
	jbe	@BLBL64
	mov	esi,ebx
	jmp	@BLBL65
@BLBL64:
	mov	esi,eax
	movzx	ecx,di
	mov	eax,dword ptr [esi+ecx*08h+01ch]
	mov	ecx,[eax]
	mov	edx,[ecx+014h]
	add	eax,edx
	call	dword ptr [ecx+010h]
	mov	ecx,eax
	mov	eax,esi
	mov	esi,ecx
@BLBL65:

; 522     for( i = 0; i < children; ++i )
	mov	ebx,esi

; 524 	ul = min(ul, child[i].rdev->querySize());
	movzx	edi,di
	inc	edi

; 522     for( i = 0; i < children; ++i )
	movzx	ecx,di
	cmp	[eax+018h],ecx
	jg	@BLBL63
	mov	[ebp-018h],ebx;	ul
	mov	[ebp-01ch],ebx;	__50
@BLBL62:

; 526     for( i = 0; i < children; ++i )
	mov	eax,[ebp+08h];	this
	cmp	dword ptr [eax+018h],0h
	jle	@BLBL69
	mov	edi,[ebp-018h];	ul
	mov	edx,[ebp-010h];	sec
	xor	ecx,ecx
@BLBL70:

; 528 	sec->u.s.child[i].size = ul;
	movzx	ecx,cx
	imul	ebx,ecx,0ah
	mov	dword ptr [edx+ebx+024h],edi

; 529 	size += ul;
	add	[eax+011ch],edi

; 526     for( i = 0; i < children; ++i )
	inc	ecx
	movzx	ebx,cx
	cmp	[eax+018h],ebx
	jg	@BLBL70
@BLBL69:

; 535     for( i = 0; i < children; ++i )
	mov	ebx,[ebp+08h];	this
	cmp	dword ptr [ebx+018h],0h
	jle	@BLBL74
	mov	esi,[ebp-010h];	sec
	xor	edi,edi
@BLBL75:

; 537 	child[i].rdev->ioSync();
	movzx	edi,di
	lea	ecx,dword ptr [ebx+edi*08h]
	mov	eax,[ecx+01ch]
	mov	[ebp-020h],ecx;	@CBE21
	mov	ecx,[eax]
	mov	edx,[ecx+054h]
	add	eax,edx
	call	dword ptr [ecx+050h]
	mov	edx,[ebp-020h];	@CBE21

; 538 	memcpy(sec->u.s.child[i].id, child[i].rdev->queryID(), sizeof(DEVID));
	imul	eax,edi,0ah
	mov	edx,[edx+01ch]
	add	eax,esi
	mov	ecx,06h
	add	edx,08h
	add	eax,01eh
	call	memcpy

; 535     for( i = 0; i < children; ++i )
	inc	edi
	movzx	ecx,di
	cmp	[ebx+018h],ecx
	jg	@BLBL75
@BLBL74:

; 544     sec->crc = Crc16(sec, SECTOR_SIZE-2);
	mov	ebx,[ebp-010h];	sec
	mov	dx,01feh
	mov	eax,ebx
	call	Crc16__FPvUs
	mov	[ebx+01feh],ax

; 545     APIRET rc = write(0, 1, sec);
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

; 546     if( rc != 0 )
	test	eax,eax
	je	@BLBL79

; 548 	Verbose(1, "VStripe::ioSync", "write(0,1,...) - rc %lu, not updated", rc);
	push	eax

; 549 	++errors;
	inc	dword ptr [ebp-014h];	errors

; 548 	Verbose(1, "VStripe::ioSync", "write(0,1,...) - rc %lu, not updated", rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE10
	mov	edx,offset FLAT:@CBE9
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 550     }
@BLBL79:

; 552     delete sec;
	mov	eax,[ebp-010h];	sec
	call	__dl__FPv

; 553     return errors;
	mov	eax,[ebp-014h];	errors
	add	esp,020h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ioSync__7VStripeFv	endp

; 442 VStripe::ioChangeID(DEVID newid)

	public ioChangeID__7VStripeFPUc
ioChangeID__7VStripeFPUc	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,edx
	push	edi
	mov	edi,eax
	push	esi

; 444     PSEC_VRDEV	sec = new SEC_VRDEV;
	mov	eax,0200h

; 442 VStripe::ioChangeID(DEVID newid)
	sub	esp,014h
	mov	[ebp+0ch],ebx;	newid

; 444     PSEC_VRDEV	sec = new SEC_VRDEV;
	call	__nw__FUi
	mov	esi,eax
	mov	edx,ebx

; 447     memcpy(id, newid, sizeof(DEVID));		// update object
	mov	ecx,06h
	lea	eax,[edi+08h]
	call	memcpy
	mov	eax,edi

; 453 	rc = read(0, 1, sec);			// remember: 0 defines ourself
	push	esi

; 442 VStripe::ioChangeID(DEVID newid)
	mov	[ebp+08h],edi;	this

; 444     PSEC_VRDEV	sec = new SEC_VRDEV;
	mov	[ebp-014h],esi;	sec

; 453 	rc = read(0, 1, sec);			// remember: 0 defines ourself
	mov	ecx,01h

; 445     int		errors = 0;
	mov	dword ptr [ebp-010h],0h;	errors

; 453 	rc = read(0, 1, sec);			// remember: 0 defines ourself
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+01ch]
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+018h]
	add	esp,010h

; 454 	if( rc != 0 )
	test	eax,eax
	je	@BLBL53

; 456 	    Verbose(1,"VStripe::ioChangeID","can't read own SEC_VRDEV - rc %lu", rc);
	push	eax

; 457 	    ++errors;
	mov	dword ptr [ebp-010h],01h;	errors

; 456 	    Verbose(1,"VStripe::ioChangeID","can't read own SEC_VRDEV - rc %lu", rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE5
	mov	edx,offset FLAT:@CBE7
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 458 	    break;
	jmp	@BLBL54
@BLBL53:

; 461 	memcpy(sec->u.s.id, newid, sizeof(DEVID));
	mov	ebx,[ebp-014h];	sec
	mov	edx,[ebp+0ch];	newid
	mov	ecx,06h
	lea	eax,[ebx+014h]
	call	memcpy

; 462 	sec->timestamp = time(NULL);		// UTC of change
	xor	eax,eax
	call	time
	mov	[ebx+010h],eax

; 463 	sec->crc = Crc16(sec, SECTOR_SIZE-2);	// !!!
	mov	dx,01feh
	mov	eax,ebx
	call	Crc16__FPvUs
	mov	[ebx+01feh],ax

; 465 	rc = write(0, 1, sec);
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

; 466 	if( rc != 0 )
	test	eax,eax
	je	@BLBL54

; 468 	    Verbose(1, "VStripe::ioChangeID", "can't write own SEC_VRDEV - rc %lu", rc);
	push	eax

; 469 	    ++errors;
	inc	dword ptr [ebp-010h];	errors

; 468 	    Verbose(1, "VStripe::ioChangeID", "can't write own SEC_VRDEV - rc %lu", rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE6
	mov	edx,offset FLAT:@CBE7
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 470 	    break;
@BLBL54:

; 475     delete sec;
	mov	eax,[ebp-014h];	sec
	call	__dl__FPv

; 476     return errors;
	mov	eax,[ebp-010h];	errors
	add	esp,014h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ioChangeID__7VStripeFPUc	endp

; 376 VStripe::ioRemoveParent()

	public ioRemoveParent__7VStripeFv
ioRemoveParent__7VStripeFv	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi

; 378     PSEC_VRDEV	sec = new SEC_VRDEV;
	mov	eax,0200h

; 376 VStripe::ioRemoveParent()
	push	esi
	mov	[ebp+08h],ebx;	this
	sub	esp,014h

; 379     int		errors = 0;
	xor	esi,esi

; 378     PSEC_VRDEV	sec = new SEC_VRDEV;
	call	__nw__FUi
	mov	edi,eax

; 385     memset(sec, 0, SECTOR_SIZE);
	mov	ecx,0200h
	xor	edx,edx
	call	memset
	mov	eax,ebx

; 386     rc = write(1, 1, sec)
; 386 ;
	push	edi

; 378     PSEC_VRDEV	sec = new SEC_VRDEV;
	mov	[ebp-014h],edi;	sec

; 386     rc = write(1, 1, sec)
; 386 ;
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+024h]
	mov	ecx,01h
	mov	edx,01h
	add	eax,edi
	call	dword ptr [ebx+020h]
	add	esp,010h

; 387     if( rc != 0 )
	test	eax,eax
	je	@BLBL44

; 389 	Verbose(1, "VStripe::ioRemoveParent", "can't clear parent SEC_VRDEV - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE4
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,01h
	call	Verbose

; 390 	++errors;
	mov	esi,01h

; 389 	Verbose(1, "VStripe::ioRemoveParent", "can't clear parent SEC_VRDEV - rc %lu", rc);
	add	esp,010h

; 395     do
@BLBL44:

; 397 	rc = read(0, 1, sec);			// remember: 0 defines ourself
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

; 398 	if( rc != 0 )
	test	eax,eax
	je	@BLBL45

; 400 	    Verbose(1, "VStripe::ioRemoveParent", "can't read own SEC_VRDEV - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE5
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,01h
	call	Verbose

; 401 	    ++errors;
	inc	esi

; 400 	    Verbose(1, "VStripe::ioRemoveParent", "can't read own SEC_VRDEV - rc %lu", rc);
	add	esp,010h

; 402 	    break;
	jmp	@BLBL46
@BLBL45:

; 405 	sec->u.s.flags |= RDFLAG_HOSTDRIVE;	// no parent means hostdrive
	mov	ebx,[ebp-014h];	sec

; 406 	sec->timestamp = time(NULL);		// UTC of change
	xor	eax,eax

; 405 	sec->u.s.flags |= RDFLAG_HOSTDRIVE;	// no parent means hostdrive
	or	byte ptr [ebx+01bh],080h

; 406 	sec->timestamp = time(NULL);		// UTC of change
	call	time
	mov	[ebx+010h],eax

; 407 	sec->crc = Crc16(sec, SECTOR_SIZE-2);	// !!!
	mov	dx,01feh
	mov	eax,ebx
	call	Crc16__FPvUs
	mov	[ebx+01feh],ax

; 409 	rc = write(0, 1, sec);
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

; 410 	if( rc != 0 )
	test	eax,eax
	je	@BLBL47

; 412 	    Verbose(1, "VStripe::ioRemoveParent", "can't write own SEC_VRDEV - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE6
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,01h
	call	Verbose

; 413 	    ++errors;
	inc	esi

; 412 	    Verbose(1, "VStripe::ioRemoveParent", "can't write own SEC_VRDEV - rc %lu", rc);
	add	esp,010h

; 414 	    break;
	jmp	@BLBL46
@BLBL47:

; 417 	parent = NULL;				// none
	mov	eax,[ebp+08h];	this
	mov	dword ptr [eax+04h],0h

; 419     while(0);
@BLBL46:

; 421     delete sec;
	mov	eax,[ebp-014h];	sec
	call	__dl__FPv

; 422     return errors;
	add	esp,014h
	mov	eax,esi
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ioRemoveParent__7VStripeFv	endp

; 338 VStripe::ioDeleteYourself()

	public ioDeleteYourself__7VStripeFv
ioDeleteYourself__7VStripeFv	proc
	push	ebx
	push	edi
	push	esi
	sub	esp,0ch

; 341     int		errors = 0;
	mov	dword ptr [esp+08h],0h;	errors

; 346     for( i = 0; i < children; ++i )
	cmp	dword ptr [eax+018h],0h
	jle	@BLBL35
	xor	esi,esi
	xor	edi,edi
@BLBL36:

; 348 	if( child[i].rdev != NULL )
	lea	ecx,dword ptr [eax+edi*08h]
	mov	ebx,[ecx+01ch]
	mov	[esp+04h],ecx;	@CBE22
	test	ebx,ebx
	je	@BLBL38
	xchg	ebx,eax

; 350 	    errors += child[i].rdev->ioRemoveParent();
	mov	ecx,[eax]
	mov	edx,[ecx+044h]
	add	eax,edx
	call	dword ptr [ecx+040h]
	mov	edx,[esp+04h];	@CBE22
	add	eax,esi
	mov	esi,eax

; 351 	    child[i].rdev = NULL;		// forget this child
	mov	dword ptr [edx+01ch],0h

; 353     }
	mov	eax,ebx
@BLBL38:

; 346     for( i = 0; i < children; ++i )
	inc	edi
	cmp	[eax+018h],edi
	jg	@BLBL36
	mov	[esp+08h],esi;	errors
@BLBL35:

; 355     return errors;
	mov	eax,[esp+08h];	errors
	add	esp,0ch
	pop	esi
	pop	edi
	pop	ebx
	ret	
ioDeleteYourself__7VStripeFv	endp

; 233 VStripe::queryIcon()

	public queryIcon__7VStripeFv
queryIcon__7VStripeFv	proc
	push	ebx

; 235     if( parent != 0  &&  parent->isValidChild(this) == False )
	mov	ebx,[eax+04h]

; 233 VStripe::queryIcon()
	push	edi
	sub	esp,08h

; 235     if( parent != 0  &&  parent->isValidChild(this) == False )
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

; 236 	return inv_icon;
	mov	eax,[eax+0124h]
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
@BLBL18:

; 237     else if( writable == True )
	cmp	byte ptr [eax+0120h],01h
	jne	@BLBL20

; 238 	return rw_icon;
	mov	eax,[eax+0128h]
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
@BLBL20:

; 240 	return ro_icon;
	mov	eax,[eax+012ch]
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
queryIcon__7VStripeFv	endp

; 199 char *

	public allocateDescription__7VStripeFv
allocateDescription__7VStripeFv	proc
	push	ebx
	mov	ebx,eax
	push	edi

; 202     char * descr = new char[80];
	mov	eax,050h

; 199 char *
	sub	esp,08h

; 202     char * descr = new char[80];
	call	__vn__FUi
	xchg	ebx,eax
	mov	edi,ebx

; 203     if( hostdrive == True  &&  os2access != - 1UL )
	cmp	byte ptr [eax+010h],01h
	jne	@BLBL14
	mov	ecx,[eax+014h]
	cmp	ecx,0ffffffffh
	je	@BLBL14
	xchg	ebx,eax

; 204 	sprintf(descr, "RAID 0 (Striping) (OS/2 Drive %lu)\n"
	mov	edx,[ebx+011ch]
	shr	edx,01h
	shr	edx,0ah
	push	edx
	movzx	edx,byte ptr [ebx+0dh]
	push	edx
	movzx	edx,byte ptr [ebx+0ch]
	push	edx
	movzx	edx,byte ptr [ebx+0bh]
	push	edx
	movzx	edx,byte ptr [ebx+0ah]
	push	edx
	movzx	edx,byte ptr [ebx+09h]
	push	edx
	movzx	edx,byte ptr [ebx+08h]
	push	edx
	mov	edx,offset FLAT:@CBE1
	push	ecx
	sub	esp,08h
	call	_sprintfieee
	mov	eax,ebx
	add	esp,028h

; 208 		size / 2 / 1024);
	jmp	@BLBL15
@BLBL14:

; 210 	sprintf(descr, "RAID 0 (Striping)\n"
	mov	ebx,[eax+011ch]
	shr	ebx,01h
	shr	ebx,0ah
	push	ebx
	movzx	ebx,byte ptr [eax+0dh]
	push	ebx
	movzx	ebx,byte ptr [eax+0ch]
	push	ebx
	movzx	ebx,byte ptr [eax+0bh]
	push	ebx
	movzx	ebx,byte ptr [eax+0ah]
	push	ebx
	movzx	ebx,byte ptr [eax+09h]
	push	ebx
	movzx	ebx,byte ptr [eax+08h]
	push	ebx
	mov	edx,offset FLAT:@CBE2
	sub	esp,08h
	mov	eax,edi
	call	_sprintfieee
	add	esp,024h

; 213 		size / 2 / 1024);
@BLBL15:

; 214     return descr;
	mov	eax,edi
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
allocateDescription__7VStripeFv	endp

; 132 VStripe::~VStripe()

	public __dt__7VStripeFv
__dt__7VStripeFv	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffe0h
	sub	esp,018h

; 133 {
	mov	dword ptr [ebp-020h],0h;	__es

; 132 VStripe::~VStripe()
	push	ebx
	mov	ebx,eax
	push	edi
	mov	[ebp-024h],edx;	@CBE23
	push	esi
	mov	[ebp+08h],ebx;	this

; 133 {
	mov	dword ptr [ebp-018h],offset FLAT:@33__fsm_tab;	__es
	mov	[ebp-014h],ebx;	__es
	mov	dword ptr [ebp-010h],0h;	__es

; 132 VStripe::~VStripe()
	mov	dword ptr [ebp-020h],01h;	__es

; 134     WinDestroyPointer(rw_icon);
	push	dword ptr [ebx+0128h]

; 132 VStripe::~VStripe()
	mov	dword ptr [ebx],offset FLAT:__vft7VStripe5VDisk

; 134     WinDestroyPointer(rw_icon);
	call	WinDestroyPointer
	add	esp,04h

; 135     WinDestroyPointer(ro_icon);
	push	dword ptr [ebx+012ch]
	call	WinDestroyPointer
	sub	esp,08h

; 136 }
	mov	dword ptr [ebp-020h],0h;	__es
	xor	ecx,ecx
	xor	edx,edx
	mov	eax,ebx
	call	__dt__7VRDriveFv
	mov	edx,[ebp-024h];	@CBE23
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
__dt__7VStripeFv	endp

; 102 VStripe::VStripe(DEVID drive_id,int nchd)

	public __ct__7VStripeFPUci
__ct__7VStripeFPUci	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffe0h
	sub	esp,014h

; 103 {
	mov	dword ptr [ebp-020h],0h;	__es

; 102 VStripe::VStripe(DEVID drive_id,int nchd)
	push	ebx

; 103 {
	mov	dword ptr [ebp-018h],offset FLAT:@1c__fsm_tab;	__es

; 102 VStripe::VStripe(DEVID drive_id,int nchd)
	push	edi
	mov	ebx,eax
	push	esi
	mov	edi,edx

; 102 VStripe::VStripe(DEVID drive_id,int nchd)
	sub	esp,04h

; 103 {
	mov	[ebp-014h],ebx;	__es
	mov	dword ptr [ebp-010h],0h;	__es

; 102 VStripe::VStripe(DEVID drive_id,int nchd)
	call	__ct__7VRDriveFv
	sub	esp,08h
	mov	dword ptr [ebp-020h],01h;	__es
	mov	edx,edi
	mov	dword ptr [ebx],offset FLAT:__vft7VStripe5VDisk

; 104     parent = NULL;
	mov	dword ptr [ebx+04h],0h

; 109     memcpy(id, drive_id, sizeof(DEVID));
	mov	ecx,06h

; 105     children = 0;
	mov	dword ptr [ebx+018h],0h

; 106     size = 0;
	mov	dword ptr [ebx+011ch],0h

; 109     memcpy(id, drive_id, sizeof(DEVID));
	lea	eax,[ebx+08h]

; 107     hostdrive = False;
	mov	byte ptr [ebx+010h],0h

; 108     writable = True;
	mov	byte ptr [ebx+0120h],01h

; 109     memcpy(id, drive_id, sizeof(DEVID));
	call	memcpy
	add	esp,0ch

; 111     inv_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_INVCHILDARRAY);
	push	0199h
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	[ebx+0124h],eax

; 112     rw_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_RAIDDRIVE);
	push	0194h
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	[ebx+0128h],eax

; 113     ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDARRAY);
	push	0197h
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	ecx,eax

; 114 }
	pop	esi

; 113     ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDARRAY);
	mov	eax,ebx

; 114 }
	pop	edi

; 113     ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDARRAY);
	mov	[eax+012ch],ecx

; 114 }
	pop	ebx
	add	esp,018h
	pop	fs:[0h]
	leave	
	ret	
__ct__7VStripeFPUci	endp
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

isWritable__7VStripeFv	proc
	movzx	eax,byte ptr [eax+0120h]
	ret	
isWritable__7VStripeFv	endp

; 65     Boolean	isHostdrive() { return hostdrive; }

isHostdrive__7VStripeFv	proc
	movzx	eax,byte ptr [eax+010h]
	ret	
isHostdrive__7VStripeFv	endp

; 64     UCHAR	isArray() { return RDTYPE_STRIPE; }

isArray__7VStripeFv	proc
	mov	eax,03h
	ret	
isArray__7VStripeFv	endp

; 57     void	forceUnwritable() { writable = False; }

forceUnwritable__7VStripeFv	proc
	mov	byte ptr [eax+0120h],0h
	ret	
forceUnwritable__7VStripeFv	endp

; 123     virtual	~VRDev() {}			// see Stroustroup pg216!!!

__dt__5VRDevFv	proc
	push	ebx
	mov	dword ptr [eax],offset FLAT:@f0__vft5VRDev5VDisk
	sub	esp,04h
	test	dl,01h
	je	@BLBL132
	mov	ebx,eax
	call	__dl__FPv
	mov	eax,ebx
@BLBL132:
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
	mov	dword ptr [ebp-018h],offset FLAT:@d7__fsm_tab58;	__es
	push	edi
	mov	[ebp-014h],eax;	__es
	mov	dword ptr [ebp-010h],0h;	__es
	push	esi
	mov	dword ptr [eax],offset FLAT:@c2__vft7VRDrive5VDisk
	mov	dword ptr [ebp-020h],0h;	__es

; 123     virtual	~VRDev() {}			// see Stroustroup pg216!!!
	test	dl,01h
	mov	dword ptr [eax],offset FLAT:@f0__vft5VRDev5VDisk
	je	@BLBL116
	mov	ebx,eax
	sub	esp,04h
	call	__dl__FPv
	mov	eax,ebx
	add	esp,04h
@BLBL116:
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
	mov	dword ptr [eax],offset FLAT:@f0__vft5VRDev5VDisk
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
	mov	dword ptr [ebp-018h],offset FLAT:@c4__fsm_tab56;	__es
	push	edi
	mov	[ebp-014h],eax;	__es
	push	esi
	mov	dword ptr [ebp-010h],0h;	__es

; 279 };
	pop	esi

; 80 };
	mov	dword ptr [eax],offset FLAT:@f0__vft5VRDev5VDisk

; 279 };
	pop	edi

; 143 };
	mov	dword ptr [ebp-020h],01h;	__es

; 279 };
	pop	ebx

; 143 };
	mov	dword ptr [eax],offset FLAT:@c2__vft7VRDrive5VDisk

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

; 289 VStripe::addChild(VRDev * newchild,Boolean cfgv,Boolean datav)

	public addChild__7VStripeFP5VRDev7BooleanT2
addChild__7VStripeFP5VRDev7BooleanT2	proc
	push	ebx
	mov	ebx,eax
	push	edi
	push	esi
	sub	esp,08h
	mov	[esp+01ch],edx;	newchild
	mov	[esp+018h],ebx;	this

; 295     child[children].rdev = newchild;
	mov	eax,[ebx+018h]
	mov	dword ptr [ebx+eax*08h+01ch],edx

; 296     child[children].cfgok = cfgv;
	mov	eax,[ebx+018h]
	mov	byte ptr [ebx+eax*08h+020h],cl

; 297     newchild->setParent(this);
; 298 
; 299     /* Update our object with child's information. */
; 300 
; 301     ULONG	childsize = newchild->querySize();
	mov	ecx,[edx]

; 125     void		setParent(VRDrive * newparent) { parent = newparent; }
	mov	[edx+04h],ebx

; 301     ULONG	childsize = newchild->querySize();
	mov	eax,[ecx+014h]
	add	edx,eax
	mov	eax,edx
	call	dword ptr [ecx+010h]
	mov	ecx,eax
	mov	eax,ebx
	mov	[esp+04h],ecx;	childsize

; 302     for( int i = 0; i < children; ++i )
	cmp	dword ptr [eax+018h],0h
	jle	@BLBL26
	mov	esi,ecx
	xor	edi,edi
@BLBL27:
	mov	ebx,eax

; 306 	if( (ul = child[i].rdev->querySize()) < childsize )
	mov	eax,dword ptr [ebx+edi*08h+01ch]
	mov	ecx,[eax]
	mov	edx,[ecx+014h]
	add	eax,edx
	call	dword ptr [ecx+010h]
	xchg	ebx,eax
	cmp	ebx,esi
	jae	@BLBL29

; 307 	    childsize = ul;
	mov	esi,ebx

; 308     }
@BLBL29:

; 302     for( int i = 0; i < children; ++i )
	inc	edi
	cmp	[eax+018h],edi
	jg	@BLBL27
	mov	[esp+04h],esi;	childsize
@BLBL26:

; 310     size = (children + 1) * childsize;
	mov	ebx,[esp+018h];	this
	mov	ecx,[ebx+018h]
	inc	ecx
	imul	ecx,[esp+04h];	childsize
	mov	[ebx+011ch],ecx

; 311     if( newchild->isWritable() == False )
	mov	eax,[esp+01ch];	newchild
	mov	ecx,[eax]
	mov	edx,[ecx+0ch]
	add	eax,edx
	call	dword ptr [ecx+08h]
	test	al,al
	jne	@BLBL32

; 312 	writable = False;			/* oups, it isn't 'changable' */
	mov	byte ptr [ebx+0120h],0h
@BLBL32:

; 314     ++children;
	mov	eax,[esp+018h];	this
	inc	dword ptr [eax+018h]

; 315 }
	add	esp,08h
	pop	esi
	pop	edi
	pop	ebx
	ret	
addChild__7VStripeFP5VRDev7BooleanT2	endp
CODE32	ends
end
