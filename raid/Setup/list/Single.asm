	title	Single.cpp
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
	public	__vft7VSingle5VDisk
	extrn	memcpy:proc
	extrn	WinLoadPointer:proc
	extrn	_Exception_CPP:proc
	extrn	__vec__delete2:proc
	extrn	__vd__FPv:proc
	extrn	WinDestroyPointer:proc
	extrn	__dl__FPv:proc
	extrn	__vn__FUi:proc
	extrn	_sprintfieee:proc
	extrn	__nw__FUi:proc
	extrn	memset:proc
	extrn	Verbose:proc
	extrn	time:proc
	extrn	Crc16__FPvUs:proc
	extrn	__PureVirtualCalled:proc
	extrn	_fltused:dword
CONST32_RO	segment
@CBE1	db "Single (OS/2 Drive %lu)",0ah
db "ID %02X%02X%02X%02X%02X%"
db "02X    %lu MByte",0h
	align 04h
@CBE2	db "Single",0ah,"ID %02X%02X%02X%0"
db "2X%02X%02X    %lu MByte",0h
@CBE3	db "VSingle::ioRemoveParent",0h
@CBE4	db "can't clear parent SEC_V"
db "RDEV - rc %lu",0h
	align 04h
@CBE5	db "can't read own SEC_VRDEV"
db " - rc %lu",0h
	align 04h
@CBE6	db "can't write own SEC_VRDE"
db "V - rc %lu",0h
	align 04h
@CBE7	db "VSingle::ioChangeID",0h
@CBE8	db "VRAIDDEVICE     ",0h
	align 04h
@CBE9	db "VSingle::ioSync",0h
@CBE10	db "write(0,1,...) - rc %lu,"
db " not updated",0h
	align 04h
@1vcid	db "$Id: single.cpp,v 1.11 2"
db "000/04/10 00:50:18 vitus"
db " Exp $",0h
CONST32_RO	ends
EH_DATA	segment
@1b__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__7VRDriveFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@35__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__7VRDriveFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@b3__fsm_tab54	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__5VRDevFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@c6__fsm_tab56	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__5VRDevFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
EH_DATA	ends
_VFT	segment
__vft7VSingle5VDisk	dd 0
	db 0h,0h,0h,0h
	dd offset FLAT:isWritable__7VSingleFv
	db 0h,0h,0h,0h
	dd offset FLAT:querySize__7VSingleFv
	db 0h,0h,0h,0h
	dd offset FLAT:read__7VSingleFUlT1Pv
	db 0h,0h,0h,0h
	dd offset FLAT:write__7VSingleFUlT1Pv
	db 0h,0h,0h,0h
	dd offset FLAT:__dt__7VSingleFv
	db 0h,0h,0h,0h
	dd offset FLAT:forceUnwritable__7VSingleFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioDeleteYourself__7VSingleFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioRemoveParent__7VSingleFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioChangeID__7VSingleFPUc
	db 0h,0h,0h,0h
	dd offset FLAT:ioSync__7VSingleFv
	db 0h,0h,0h,0h
	dd offset FLAT:queryWhere__5VRDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:isArray__7VSingleFv
	db 0h,0h,0h,0h
	dd offset FLAT:isHostdrive__7VSingleFv
	db 0h,0h,0h,0h
	dd offset FLAT:allocateDescription__7VSingleFv
	db 0h,0h,0h,0h
	dd offset FLAT:queryIcon__7VSingleFv
	db 0h,0h,0h,0h
	dd offset FLAT:setHostdrive__7VSingleFUl
	db 0h,0h,0h,0h
	dd offset FLAT:addChild__7VSingleFP5VRDev7BooleanT2
	db 0h,0h,0h,0h
	dd offset FLAT:queryChildren__7VSingleFv
	db 0h,0h,0h,0h
	dd offset FLAT:getChild__7VSingleFi
	db 0h,0h,0h,0h
	dd offset FLAT:isValidChild__7VRDriveFi
	db 0h,0h,0h,0h
	dd offset FLAT:isValidChild__7VRDriveFP5VRDev
	db 0h,0h,0h,0h
@b1__vft7VRDrive5VDisk	dd 0
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
@e0__vft5VRDev5VDisk	dd 0
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
@e7__vft5VDisk	dd 0
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

; 542 VSingle::querySize()

	public querySize__7VSingleFv
querySize__7VSingleFv	proc

; 544     return size;
	mov	eax,[eax+01ch]
	ret	
querySize__7VSingleFv	endp

; 258 VSingle::setHostdrive(ULONG os2idx)

	public setHostdrive__7VSingleFUl
setHostdrive__7VSingleFUl	proc

; 261     os2access = os2idx;
	mov	[eax+014h],edx

; 260     hostdrive = True;
	mov	byte ptr [eax+010h],01h

; 262 }
	ret	
setHostdrive__7VSingleFUl	endp

; 169 VRDev *

	public getChild__7VSingleFi
getChild__7VSingleFi	proc

; 172     if( index >= 1 )
	cmp	edx,01h
	jl	@BLBL11

; 173 	return 0;
	xor	eax,eax
	ret	
@BLBL11:

; 174     return child;
	mov	eax,[eax+018h]
	ret	
getChild__7VSingleFi	endp

; 148 VSingle::queryChildren()

	public queryChildren__7VSingleFv
queryChildren__7VSingleFv	proc

; 150     return 1;
	mov	eax,01h
	ret	
queryChildren__7VSingleFv	endp

; 595 VSingle::write(ULONG offset,ULONG count,PVOID buffer)

	public write__7VSingleFUlT1Pv
write__7VSingleFUlT1Pv	proc
	push	ebx

; 597     return child->write(offset+1, count, buffer);
	inc	edx

; 595 VSingle::write(ULONG offset,ULONG count,PVOID buffer)
	push	edi

; 597     return child->write(offset+1, count, buffer);
	push	dword ptr [esp+018h];	buffer
	mov	eax,[eax+018h]
	sub	esp,0ch
	mov	ebx,[eax]
	mov	edi,[ebx+024h]
	add	eax,edi
	call	dword ptr [ebx+020h]
	add	esp,010h
	pop	edi
	pop	ebx
	ret	
write__7VSingleFUlT1Pv	endp

; 569 VSingle::read(ULONG offset,ULONG count,PVOID buffer)

	public read__7VSingleFUlT1Pv
read__7VSingleFUlT1Pv	proc
	push	ebx

; 571     return child->read(offset+1, count, buffer);
	inc	edx

; 569 VSingle::read(ULONG offset,ULONG count,PVOID buffer)
	push	edi

; 571     return child->read(offset+1, count, buffer);
	push	dword ptr [esp+018h];	buffer
	mov	eax,[eax+018h]
	sub	esp,0ch
	mov	ebx,[eax]
	mov	edi,[ebx+01ch]
	add	eax,edi
	call	dword ptr [ebx+018h]
	add	esp,010h
	pop	edi
	pop	ebx
	ret	
read__7VSingleFUlT1Pv	endp

; 480 VSingle::ioSync()

	public ioSync__7VSingleFv
ioSync__7VSingleFv	proc
	push	ebx
	mov	ebx,eax
	push	edi

; 482     PSEC_VRDEV	sec = new SEC_VRDEV;
	mov	eax,0200h

; 480 VSingle::ioSync()
	push	esi

; 483     int		errors = 0;
	xor	esi,esi

; 480 VSingle::ioSync()
	sub	esp,014h

; 482     PSEC_VRDEV	sec = new SEC_VRDEV;
	call	__nw__FUi
	mov	edi,eax

; 488     memset(sec, 0, sizeof(*sec));
	mov	ecx,0200h
	xor	edx,edx
	call	memset

; 489     memcpy(sec->sectype, "VRAIDDEVICE     ", 16);
	mov	ecx,010h
	mov	edx,offset FLAT:@CBE8
	mov	eax,edi
	call	memcpy

; 490     sec->timestamp = time(NULL);
	xor	eax,eax
	call	time

; 482     PSEC_VRDEV	sec = new SEC_VRDEV;
	mov	[esp+0ch],edi;	sec

; 490     sec->timestamp = time(NULL);
	mov	[edi+010h],eax

; 492     memcpy(sec->u.s.id, id, sizeof(DEVID));
	mov	ecx,06h
	lea	edx,[ebx+08h]
	lea	eax,[edi+014h]
	call	memcpy

; 493     sec->u.s.type = RDTYPE_SINGLE;
	mov	byte ptr [edi+01ah],01h

; 494     sec->u.s.flags = (UCHAR)(parent != 0 ? 0 : 0x80);
	cmp	dword ptr [ebx+04h],0h
	setne	cl
	movzx	ecx,cl
	dec	ecx
	and	ecx,080h

; 496     sec->u.s.children = 1;
	mov	word ptr [edi+01ch],01h

; 494     sec->u.s.flags = (UCHAR)(parent != 0 ? 0 : 0x80);
	mov	[edi+01bh],cl

; 502     sec->u.s.child[0].size = size = child->querySize();
	mov	eax,[ebx+018h]
	mov	ecx,[eax]
	mov	edx,[ecx+014h]
	add	eax,edx
	call	dword ptr [ecx+010h]
	mov	[ebx+01ch],eax
	mov	[edi+024h],eax

; 507     child->ioSync();
	mov	eax,[ebx+018h]
	mov	ecx,[eax]
	mov	edx,[ecx+054h]
	add	eax,edx
	call	dword ptr [ecx+050h]

; 508     memcpy(sec->u.s.child[0].id, child->queryID(), sizeof(DEVID));
	mov	edx,[ebx+018h]
	mov	ecx,06h
	add	edx,08h
	lea	eax,[edi+01eh]
	call	memcpy

; 513     sec->crc = Crc16(sec, SECTOR_SIZE-2);
	mov	dx,01feh
	mov	eax,edi
	call	Crc16__FPvUs
	mov	[edi+01feh],ax
	mov	eax,ebx

; 514     APIRET rc = write(0, 1, sec);
	push	edi
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+024h]
	mov	ecx,01h
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+020h]
	add	esp,010h

; 515     if( rc != 0 )
	test	eax,eax
	je	@BLBL51

; 517 	Verbose(1, "VSingle::ioSync", "write(0,1,...) - rc %lu, not updated", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE10
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE9
	mov	eax,01h
	call	Verbose

; 518 	++errors;
	mov	esi,01h

; 517 	Verbose(1, "VSingle::ioSync", "write(0,1,...) - rc %lu, not updated", rc);
	add	esp,010h

; 519     }
@BLBL51:

; 521     delete sec;
	mov	eax,[esp+0ch];	sec
	call	__dl__FPv

; 522     return errors;
	add	esp,014h
	mov	eax,esi
	pop	esi
	pop	edi
	pop	ebx
	ret	
ioSync__7VSingleFv	endp

; 426 VSingle::ioChangeID(DEVID newid)

	public ioChangeID__7VSingleFPUc
ioChangeID__7VSingleFPUc	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,edx
	push	edi
	mov	edi,eax
	push	esi

; 428     PSEC_VRDEV	sec = new SEC_VRDEV;
	mov	eax,0200h

; 426 VSingle::ioChangeID(DEVID newid)
	sub	esp,014h
	mov	[ebp+0ch],ebx;	newid

; 428     PSEC_VRDEV	sec = new SEC_VRDEV;
	call	__nw__FUi
	mov	esi,eax
	mov	edx,ebx

; 431     memcpy(id, newid, sizeof(DEVID));		// update object
	mov	ecx,06h
	lea	eax,[edi+08h]
	call	memcpy
	mov	eax,edi

; 437 	rc = read(0, 1, sec);			// remember: 0 defines ourself
	push	esi

; 426 VSingle::ioChangeID(DEVID newid)
	mov	[ebp+08h],edi;	this

; 428     PSEC_VRDEV	sec = new SEC_VRDEV;
	mov	[ebp-014h],esi;	sec

; 437 	rc = read(0, 1, sec);			// remember: 0 defines ourself
	mov	ecx,01h

; 429     int		errors = 0;
	mov	dword ptr [ebp-010h],0h;	errors

; 437 	rc = read(0, 1, sec);			// remember: 0 defines ourself
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+01ch]
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+018h]
	add	esp,010h

; 438 	if( rc != 0 )
	test	eax,eax
	je	@BLBL42

; 440 	    Verbose(1,"VSingle::ioChangeID","can't read own SEC_VRDEV - rc %lu", rc);
	push	eax

; 441 	    ++errors;
	mov	dword ptr [ebp-010h],01h;	errors

; 440 	    Verbose(1,"VSingle::ioChangeID","can't read own SEC_VRDEV - rc %lu", rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE5
	mov	edx,offset FLAT:@CBE7
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 442 	    break;
	jmp	@BLBL43
@BLBL42:

; 445 	memcpy(sec->u.s.id, newid, sizeof(DEVID));
	mov	ebx,[ebp-014h];	sec
	mov	edx,[ebp+0ch];	newid
	mov	ecx,06h
	lea	eax,[ebx+014h]
	call	memcpy

; 446 	sec->timestamp = time(NULL);		// UTC of change
	xor	eax,eax
	call	time
	mov	[ebx+010h],eax

; 447 	sec->crc = Crc16(sec, SECTOR_SIZE-2);	// !!!
	mov	dx,01feh
	mov	eax,ebx
	call	Crc16__FPvUs
	mov	[ebx+01feh],ax

; 449 	rc = write(0, 1, sec);
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

; 450 	if( rc != 0 )
	test	eax,eax
	je	@BLBL43

; 452 	    Verbose(1, "VSingle::ioChangeID", "can't write own SEC_VRDEV - rc %lu", rc);
	push	eax

; 453 	    ++errors;
	inc	dword ptr [ebp-010h];	errors

; 452 	    Verbose(1, "VSingle::ioChangeID", "can't write own SEC_VRDEV - rc %lu", rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE6
	mov	edx,offset FLAT:@CBE7
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 454 	    break;
@BLBL43:

; 459     delete sec;
	mov	eax,[ebp-014h];	sec
	call	__dl__FPv

; 460     return errors;
	mov	eax,[ebp-010h];	errors
	add	esp,014h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ioChangeID__7VSingleFPUc	endp

; 359 VSingle::ioRemoveParent()

	public ioRemoveParent__7VSingleFv
ioRemoveParent__7VSingleFv	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi

; 361     PSEC_VRDEV	sec = new SEC_VRDEV;
	mov	eax,0200h

; 359 VSingle::ioRemoveParent()
	push	esi
	mov	[ebp+08h],ebx;	this
	sub	esp,014h

; 362     int		errors = 0;
	xor	esi,esi

; 361     PSEC_VRDEV	sec = new SEC_VRDEV;
	call	__nw__FUi
	mov	edi,eax

; 368     memset(sec, 0, SECTOR_SIZE);
	mov	ecx,0200h
	xor	edx,edx
	call	memset
	mov	eax,ebx

; 369     rc = write(1, 1, sec);
	push	edi

; 361     PSEC_VRDEV	sec = new SEC_VRDEV;
	mov	[ebp-014h],edi;	sec

; 369     rc = write(1, 1, sec);
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+024h]
	mov	ecx,01h
	mov	edx,01h
	add	eax,edi
	call	dword ptr [ebx+020h]
	add	esp,010h

; 370     if( rc != 0 )
	test	eax,eax
	je	@BLBL33

; 372 	Verbose(1, "VSingle::ioRemoveParent", "can't clear parent SEC_VRDEV - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE4
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,01h
	call	Verbose

; 373 	++errors;
	mov	esi,01h

; 372 	Verbose(1, "VSingle::ioRemoveParent", "can't clear parent SEC_VRDEV - rc %lu", rc);
	add	esp,010h

; 379     do
@BLBL33:

; 381 	rc = read(0, 1, sec);			// remember: 0 defines ourself
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

; 382 	if( rc != 0 )
	test	eax,eax
	je	@BLBL34

; 384 	    Verbose(1, "VSingle::ioRemoveParent", "can't read own SEC_VRDEV - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE5
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,01h
	call	Verbose

; 385 	    ++errors;
	inc	esi

; 384 	    Verbose(1, "VSingle::ioRemoveParent", "can't read own SEC_VRDEV - rc %lu", rc);
	add	esp,010h

; 386 	    break;
	jmp	@BLBL35
@BLBL34:

; 389 	sec->u.s.flags |= RDFLAG_HOSTDRIVE;	// no parent means hostdrive
	mov	ebx,[ebp-014h];	sec

; 390 	sec->timestamp = time(NULL);		// UTC of change
	xor	eax,eax

; 389 	sec->u.s.flags |= RDFLAG_HOSTDRIVE;	// no parent means hostdrive
	or	byte ptr [ebx+01bh],080h

; 390 	sec->timestamp = time(NULL);		// UTC of change
	call	time
	mov	[ebx+010h],eax

; 391 	sec->crc = Crc16(sec, SECTOR_SIZE-2);	// !!!
	mov	dx,01feh
	mov	eax,ebx
	call	Crc16__FPvUs
	mov	[ebx+01feh],ax

; 393 	rc = write(0, 1, sec);
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

; 394 	if
; 394 ( rc != 0 )
	test	eax,eax
	je	@BLBL36

; 396 	    Verbose(1, "VSingle::ioRemoveParent", "can't write own SEC_VRDEV - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE6
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,01h
	call	Verbose

; 397 	    ++errors;
	inc	esi

; 396 	    Verbose(1, "VSingle::ioRemoveParent", "can't write own SEC_VRDEV - rc %lu", rc);
	add	esp,010h

; 398 	    break;
	jmp	@BLBL35
@BLBL36:

; 401 	parent = NULL;				// none
	mov	eax,[ebp+08h];	this
	mov	dword ptr [eax+04h],0h

; 403     while(0);
@BLBL35:

; 405     delete sec;
	mov	eax,[ebp-014h];	sec
	call	__dl__FPv

; 406     return errors;
	add	esp,014h
	mov	eax,esi
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ioRemoveParent__7VSingleFv	endp

; 325 VSingle::ioDeleteYourself()

	public ioDeleteYourself__7VSingleFv
ioDeleteYourself__7VSingleFv	proc
	push	ebx
	mov	ebx,eax
	push	edi

; 327     int		errors = 0;
	xor	edi,edi

; 325 VSingle::ioDeleteYourself()
	sub	esp,08h

; 332     if( child != NULL )
	mov	eax,[ebx+018h]
	test	eax,eax
	je	@BLBL29

; 334 	errors += child->ioRemoveParent();
	mov	ecx,[eax]
	mov	edx,[ecx+044h]
	add	eax,edx
	call	dword ptr [ecx+040h]
	mov	edi,eax
	mov	eax,ebx

; 335 	child = NULL;
	mov	dword ptr [eax+018h],0h

; 336     }
@BLBL29:

; 338     return errors;
	mov	eax,edi
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
ioDeleteYourself__7VSingleFv	endp

; 228 VSingle::queryIcon()

	public queryIcon__7VSingleFv
queryIcon__7VSingleFv	proc
	push	ebx

; 230     if( parent != 0  &&  parent->isValidChild(this) == False )
	mov	ebx,[eax+04h]

; 228 VSingle::queryIcon()
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
	mov	eax,[eax+024h]
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
@BLBL18:

; 232     else if( writable == True )
	cmp	byte ptr [eax+020h],01h
	jne	@BLBL20

; 233 	return rw_icon;
	mov	eax,[eax+028h]
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
@BLBL20:

; 235 	return ro_icon;
	mov	eax,[eax+02ch]
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
queryIcon__7VSingleFv	endp

; 194 char *

	public allocateDescription__7VSingleFv
allocateDescription__7VSingleFv	proc
	push	ebx
	mov	ebx,eax
	push	edi

; 197     char * descr = new char[80];
	mov	eax,050h

; 194 char *
	sub	esp,08h

; 197     char * descr = new char[80];
	call	__vn__FUi
	xchg	ebx,eax
	mov	edi,ebx

; 198     if( hostdrive == True  &&  os2access != -1UL )
	cmp	byte ptr [eax+010h],01h
	jne	@BLBL14
	mov	ecx,[eax+014h]
	cmp	ecx,0ffffffffh
	je	@BLBL14
	xchg	ebx,eax

; 199 	sprintf(descr, "Single (OS/2 Drive %lu)\n"
	mov	edx,[ebx+01ch]
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

; 203 		size / 2 / 1024);
	jmp	@BLBL15
@BLBL14:

; 205 	sprintf(descr, "Single\n"
	mov	ebx,[eax+01ch]
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

; 208 		size / 2 / 1024);
@BLBL15:

; 209     return descr;
	mov	eax,edi
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
allocateDescription__7VSingleFv	endp

; 124 VSingle::~VSingle()

	public __dt__7VSingleFv
__dt__7VSingleFv	proc
	push	ebp

; 126     delete[] child;
	mov	ecx,010h

; 124 VSingle::~VSingle()
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffe0h
	sub	esp,018h
	mov	[ebp-024h],edx;	@CBE15
	push	ebx
	mov	ebx,eax
	push	edi

; 125 {
	mov	dword ptr [ebp-020h],0h;	__es

; 124 VSingle::~VSingle()
	push	esi

; 125 {
	mov	dword ptr [ebp-018h],offset FLAT:@35__fsm_tab;	__es

; 126     delete[] child;
	push	offset FLAT: __vd__FPv

; 125 {
	mov	[ebp-014h],ebx;	__es

; 126     delete[] child;
	push	offset FLAT: __dftdt__5VRDevFv

; 125 {
	mov	dword ptr [ebp-010h],0h;	__es

; 126     delete[] child;
	sub	esp,0ch

; 124 VSingle::~VSingle()
	mov	[ebp+08h],ebx;	this

; 124 VSingle::~VSingle()
	mov	dword ptr [ebp-020h],01h;	__es

; 126     delete[] child;
	mov	eax,[ebx+018h]

; 124 VSingle::~VSingle()
	mov	dword ptr [ebx],offset FLAT:__vft7VSingle5VDisk

; 126     delete[] child;
	or	edx,0ffffffffh
	call	__vec__delete2
	add	esp,014h

; 127     WinDestroyPointer(rw_icon);
	push	dword ptr [ebx+028h]
	call	WinDestroyPointer
	add	esp,04h

; 128     WinDestroyPointer(ro_icon);
	push	dword ptr [ebx+02ch]
	call	WinDestroyPointer
	sub	esp,08h

; 129 }
	mov	dword ptr [ebp-020h],0h;	__es
	xor	ecx,ecx
	xor	edx,edx
	mov	eax,ebx
	call	__dt__7VRDriveFv
	mov	edx,[ebp-024h];	@CBE15
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
__dt__7VSingleFv	endp

; 94 VSingle::VSingle(DEVID drive_id)

	public __ct__7VSingleFPUc
__ct__7VSingleFPUc	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffe0h
	sub	esp,014h

; 95 {
	mov	dword ptr [ebp-020h],0h;	__es

; 94 VSingle::VSingle(DEVID drive_id)
	push	ebx

; 95 {
	mov	dword ptr [ebp-018h],offset FLAT:@1b__fsm_tab;	__es

; 94 VSingle::VSingle(DEVID drive_id)
	push	edi
	mov	ebx,eax
	push	esi
	mov	edi,edx

; 94 VSingle::VSingle(DEVID drive_id)
	sub	esp,04h

; 95 {
	mov	[ebp-014h],ebx;	__es
	mov	dword ptr [ebp-010h],0h;	__es

; 94 VSingle::VSingle(DEVID drive_id)
	call	__ct__7VRDriveFv
	sub	esp,08h
	mov	dword ptr [ebp-020h],01h;	__es
	mov	edx,edi
	mov	dword ptr [ebx],offset FLAT:__vft7VSingle5VDisk

; 96     parent = NULL;
	mov	dword ptr [ebx+04h],0h

; 101     memcpy(id, drive_id, sizeof(DEVID));
	mov	ecx,06h

; 97     child = 0;
	mov	dword ptr [ebx+018h],0h

; 98     size = 0;
	mov	dword ptr [ebx+01ch],0h

; 101     memcpy(id, drive_id, sizeof(DEVID));
	lea	eax,[ebx+08h]

; 99     hostdrive = False;
	mov	byte ptr [ebx+010h],0h

; 100     writable = True;
	mov	byte ptr [ebx+020h],01h

; 101     memcpy(id, drive_id, sizeof(DEVID));
	call	memcpy
	add	esp,0ch

; 103     inv_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_INVCHILDARRAY);
	push	0199h
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	[ebx+024h],eax

; 104     rw_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_RAIDDRIVE);
	push	0194h
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	[ebx+028h],eax

; 105     ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDARRAY);
	push	0197h
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	ecx,eax

; 106 }
	pop	esi

; 105     ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDARRAY);
	mov	eax,ebx

; 106 }
	pop	edi

; 105     ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDARRAY);
	mov	[eax+02ch],ecx

; 106 }
	pop	ebx
	add	esp,018h
	pop	fs:[0h]
	leave	
	ret	
__ct__7VSingleFPUc	endp
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

; 64     Boolean	isWritable() { return writable; }
EH_CODE	ends
CODE32	segment

isWritable__7VSingleFv	proc
	movzx	eax,byte ptr [eax+020h]
	ret	
isWritable__7VSingleFv	endp

; 55     Boolean	isHostdrive() { return hostdrive; }

isHostdrive__7VSingleFv	proc
	movzx	eax,byte ptr [eax+010h]
	ret	
isHostdrive__7VSingleFv	endp

; 54     UCHAR	isArray() { return RDTYPE_SINGLE; }

isArray__7VSingleFv	proc
	mov	eax,01h
	ret	
isArray__7VSingleFv	endp

; 47     void	forceUnwritable() { writable = False; }

forceUnwritable__7VSingleFv	proc
	mov	byte ptr [eax+020h],0h
	ret	
forceUnwritable__7VSingleFv	endp

; 123     virtual	~VRDev() {}			// see Stroustroup pg216!!!

__dt__5VRDevFv	proc
	push	ebx
	mov	dword ptr [eax],offset FLAT:@e0__vft5VRDev5VDisk
	sub	esp,04h
	test	dl,01h
	je	@BLBL89
	mov	ebx,eax
	call	__dl__FPv
	mov	eax,ebx
@BLBL89:
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
	mov	dword ptr [ebp-018h],offset FLAT:@c6__fsm_tab56;	__es
	push	edi
	mov	[ebp-014h],eax;	__es
	mov	dword ptr [ebp-010h],0h;	__es
	push	esi
	mov	dword ptr [eax],offset FLAT:@b1__vft7VRDrive5VDisk
	mov	dword ptr [ebp-020h],0h;	__es

; 123     virtual	~VRDev() {}			// see Stroustroup pg216!!!
	test	dl,01h
	mov	dword ptr [eax],offset FLAT:@e0__vft5VRDev5VDisk
	je	@BLBL71
	mov	ebx,eax
	sub	esp,04h
	call	__dl__FPv
	mov	eax,ebx
	add	esp,04h
@BLBL71:
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
	mov	dword ptr [eax],offset FLAT:@e0__vft5VRDev5VDisk
	ret	
__dftbdt__5VRDevFv	endp
EH_CODE	ends
CODE32	segment

__dftdt__5VRDevFv	proc
	mov	dword ptr [eax],offset FLAT:@e0__vft5VRDev5VDisk
	ret	
__dftdt__5VRDevFv	endp

; 279 };

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
	mov	dword ptr [ebp-018h],offset FLAT:@b3__fsm_tab54;	__es
	push	edi
	mov	[ebp-014h],eax;	__es
	push	esi
	mov	dword ptr [ebp-010h],0h;	__es

; 279 };
	pop	esi

; 80 };
	mov	dword ptr [eax],offset FLAT:@e0__vft5VRDev5VDisk

; 279 };
	pop	edi

; 143 };
	mov	dword ptr [ebp-020h],01h;	__es

; 279 };
	pop	ebx

; 143 };
	mov	dword ptr [eax],offset FLAT:@b1__vft7VRDrive5VDisk

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

; 286 VSingle::addChild(VRDev * newchild,Boolean cfgv,Boolean datav)

	public addChild__7VSingleFP5VRDev7BooleanT2
addChild__7VSingleFP5VRDev7BooleanT2	proc
	push	ebx
	mov	ebx,eax
	push	edi
	mov	edi,edx

; 294     child = newchild;
	mov	[ebx+018h],edi

; 295     newchild->setParent(this);
; 296 
; 297     /* Update own object with child's information. */
; 298 
; 299     size += newchild->querySize();
	mov	ecx,[edi]

; 286 VSingle::addChild(VRDev * newchild,Boolean cfgv,Boolean datav)
	sub	esp,04h

; 125     void		setParent(VRDrive * newparent) { parent = newparent; }
	mov	[edi+04h],ebx

; 299     size += newchild->querySize();
	mov	eax,[ecx+014h]
	add	eax,edi
	call	dword ptr [ecx+010h]
	mov	edx,edi
	add	[ebx+01ch],eax

; 300     if( newchild->isWritable() == False )
	mov	ecx,[edx]
	mov	eax,[ecx+0ch]
	add	edx,eax
	mov	eax,edx
	call	dword ptr [ecx+08h]
	test	al,al
	mov	eax,ebx
	jne	@BLBL26

; 301 	writable = False;			/* oups, it isn't 'changable' */
	mov	byte ptr [eax+020h],0h
@BLBL26:

; 302 }
	add	esp,04h
	pop	edi
	pop	ebx
	ret	
addChild__7VSingleFP5VRDev7BooleanT2	endp
CODE32	ends
end
