	title	Mirror.cpp
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
	public	__vft7VMirror5VDisk
	extrn	memcpy:proc
	extrn	WinLoadPointer:proc
	extrn	_Exception_CPP:proc
	extrn	WinDestroyPointer:proc
	extrn	__dl__FPv:proc
	extrn	__vn__FUi:proc
	extrn	_sprintfieee:proc
	extrn	strlen:proc
	extrn	Verbose:proc
	extrn	__nw__FUi:proc
	extrn	memset:proc
	extrn	time:proc
	extrn	Crc16__FPvUs:proc
	extrn	memcmp:proc
	extrn	__vd__FPv:proc
	extrn	__PureVirtualCalled:proc
	extrn	_fltused:dword
CONST32_RO	segment
@CBE1	db "ready",0h
	align 04h
@CBE2	db "build",0h
	align 04h
@CBE3	db "fail",0h
	align 04h
@CBE4	db "error",0h
	align 04h
@CBE5	db "RAID 1 (Mirroring, %s)",0h
	align 04h
@CBE6	db " (OS/2 Drive %lu)",0h
	align 04h
@CBE7	db 0ah,"ID %02X%02X%02X%02X%02X"
db "%02X    %lu MByte",0h
	align 04h
@CBE8	db "VMirror",0h
@CBE9	db "can't find child to remo"
db "ve!",0h
@CBE10	db "VMirror::ioRemoveParent",0h
@CBE11	db "can't clear parent SEC_V"
db "RDEV - rc %lu",0h
	align 04h
@CBE12	db "can't read own SEC_VRDEV"
db "2 - rc %lu",0h
	align 04h
@CBE13	db "can't write own SEC_VRDE"
db "V2 - rc %lu",0h
@CBE14	db "VMirror::ioChangeID",0h
@CBE15	db "VRAIDDEVICE2    ",0h
	align 04h
@CBE16	db "updating current configu"
db "ration sector",0h
	align 04h
@CBE17	db "creating new configurati"
db "on sector",0h
	align 04h
@CBE18	db "VMirror::ioSync",0h
@CBE19	db "write(0,1,...) - rc %lu,"
db " not updated",0h
	align 04h
@CBE20	db "Data error when reading "
db "child %d, ignored",0h
	align 04h
@1vcid	db "$Id: mirror.cpp,v 1.13 2"
db "000/04/10 00:54:49 vitus"
db " Exp $",0h
CONST32_RO	ends
EH_DATA	segment
@1f__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__20VDriveWithRedundancyFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@36__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__20VDriveWithRedundancyFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@e0__fsm_tab60	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__7VRDriveFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@f3__fsm_tab62	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__7VRDriveFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@116__fsm_tab64	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__5VRDevFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@129__fsm_tab66	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__5VRDevFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
EH_DATA	ends
DATA32	segment
@57State2Str	dd offset FLAT:@CBE1
	dd offset FLAT:@CBE2
	dd offset FLAT:@CBE3
	dd offset FLAT:@CBE4
DATA32	ends
_VFT	segment
__vft7VMirror5VDisk	dd 0
	db 0h,0h,0h,0h
	dd offset FLAT:isWritable__7VMirrorFv
	db 0h,0h,0h,0h
	dd offset FLAT:querySize__7VMirrorFv
	db 0h,0h,0h,0h
	dd offset FLAT:read__7VMirrorFUlT1Pv
	db 0h,0h,0h,0h
	dd offset FLAT:write__7VMirrorFUlT1Pv
	db 0h,0h,0h,0h
	dd offset FLAT:__dt__7VMirrorFv
	db 0h,0h,0h,0h
	dd offset FLAT:forceUnwritable__7VMirrorFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioDeleteYourself__7VMirrorFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioRemoveParent__7VMirrorFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioChangeID__7VMirrorFPUc
	db 0h,0h,0h,0h
	dd offset FLAT:ioSync__7VMirrorFv
	db 0h,0h,0h,0h
	dd offset FLAT:queryWhere__5VRDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:isArray__7VMirrorFv
	db 0h,0h,0h,0h
	dd offset FLAT:isHostdrive__7VMirrorFv
	db 0h,0h,0h,0h
	dd offset FLAT:allocateDescription__7VMirrorFv
	db 0h,0h,0h,0h
	dd offset FLAT:queryIcon__7VMirrorFv
	db 0h,0h,0h,0h
	dd offset FLAT:setHostdrive__7VMirrorFUl
	db 0h,0h,0h,0h
	dd offset FLAT:addChild__7VMirrorFP5VRDev7BooleanT2
	db 0h,0h,0h,0h
	dd offset FLAT:queryChildren__7VMirrorFv
	db 0h,0h,0h,0h
	dd offset FLAT:getChild__7VMirrorFi
	db 0h,0h,0h,0h
	dd offset FLAT:isValidChild__7VMirrorFi
	db 0h,0h,0h,0h
	dd offset FLAT:isValidChild__7VMirrorFP5VRDev
	db 0h,0h,0h,0h
	dd offset FLAT:setBuild__7VMirrorFUl
	db 0h,0h,0h,0h
	dd offset FLAT:ioRemoveChild__7VMirrorFP5VRDev
	db 0h,0h,0h,0h
@de__vft20VDriveWithRedundancy5VDisk	dd 0
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__dt__20VDriveWithRedundancyFv
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
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
	dd offset FLAT:__PureVirtualCalled
	db 0h,0h,0h,0h
@114__vft7VRDrive5VDisk	dd 0
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
@13c__vft5VRDev5VDisk	dd 0
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
@143__vft5VDisk	dd 0
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

; 749 VMirror::querySize()

	public querySize__7VMirrorFv
querySize__7VMirrorFv	proc

; 751     return size;
	mov	eax,[eax+011ch]
	ret	
querySize__7VMirrorFv	endp

; 325 VMirror::setHostdrive(ULONG os2idx)

	public setHostdrive__7VMirrorFUl
setHostdrive__7VMirrorFUl	proc

; 328     os2access = os2idx;
	mov	[eax+014h],edx

; 327     hostdrive = True;
	mov	byte ptr [eax+010h],01h

; 329 }
	ret	
setHostdrive__7VMirrorFUl	endp

; 231 VMirror::isValidChild(VRDev * which)

	public isValidChild__7VMirrorFP5VRDev
isValidChild__7VMirrorFP5VRDev	proc
	push	edi
	mov	[esp+08h],eax;	this

; 233     for( int i = 0; i < children; ++i )
	cmp	dword ptr [eax+018h],0h
	jle	@BLBL18
	mov	edi,eax
	xor	ecx,ecx
@BLBL19:

; 234 	if( which == child[i].rdev )
	lea	eax,dword ptr [edi+ecx*08h]
	cmp	[eax+01ch],edx
	jne	@BLBL21
	mov	ecx,eax

; 235 	    return child[i].valid;
	pop	edi
	movzx	eax,byte ptr [ecx+020h]
	ret	
@BLBL21:

; 233     for( int i = 0; i < children; ++i )
	inc	ecx
	cmp	[edi+018h],ecx
	jg	@BLBL19
@BLBL18:

; 237     return False;				/* xxx */
	pop	edi
	xor	eax,eax
	ret	
isValidChild__7VMirrorFP5VRDev	endp

; 221 VMirror::isValidChild(int which)

	public isValidChild__7VMirrorFi
isValidChild__7VMirrorFi	proc

; 224     return child[which].valid;
	movzx	eax,byte ptr [eax+edx*08h+020h]
	ret	
isValidChild__7VMirrorFi	endp

; 209 VRDev *

	public getChild__7VMirrorFi
getChild__7VMirrorFi	proc

; 212     if( index >= children )
	cmp	[eax+018h],edx
	jg	@BLBL13

; 213 	return 0;
	xor	eax,eax
	ret	
@BLBL13:

; 214     return child[index].rdev;
	mov	eax,dword ptr [eax+edx*08h+01ch]
	ret	
getChild__7VMirrorFi	endp

; 188 VMirror::queryChildren()

	public queryChildren__7VMirrorFv
queryChildren__7VMirrorFv	proc

; 190     return children;
	mov	eax,[eax+018h]
	ret	
queryChildren__7VMirrorFv	endp

; 167 VMirror::setBuild(ULONG nextsec)

	public setBuild__7VMirrorFUl
setBuild__7VMirrorFUl	proc

; 169     state = Build;
	mov	byte ptr [eax+0130h],01h

; 170 }
	ret	
setBuild__7VMirrorFUl	endp

; 824 VMirror::write(ULONG offset,ULONG count,PVOID buffer)

	public write__7VMirrorFUlT1Pv
write__7VMirrorFUlT1Pv	proc
	push	ebp
	xchg	ecx,eax
	mov	ebp,esp
	sub	esp,0ch
	mov	[ebp+010h],eax;	count
	push	ebx
	mov	[ebp+08h],ecx;	this
	push	edi

; 826     APIRET	rc = 0;
	mov	dword ptr [ebp-04h],0h;	rc

; 827 
; 828     for( int i = 0; i < children; ++i )
	cmp	dword ptr [ecx+018h],0h

; 824 VMirror::write(ULONG offset,ULONG count,PVOID buffer)
	push	esi

; 828     for( int i = 0; i < children; ++i )
	jle	@BLBL142
	inc	edx
	mov	[ebp-08h],edx;	@CBE28
	mov	edx,ecx
	mov	[ebp+08h],ecx;	this
	mov	ecx,[ebp+014h];	buffer
	mov	[ebp+010h],eax;	count
	mov	ebx,eax
	xor	esi,esi
	xor	edi,edi
@BLBL143:
	mov	eax,edx
	xchg	eax,ebx

; 829 	rc |= child[i].rdev->write(offset+1, count, buffer);
	push	ecx

; 828     for( int i = 0; i < children; ++i )
	mov	[ebp-0ch],esi;	@CBE29

; 829 	rc |= child[i].rdev->write(offset+1, count, buffer);
	mov	ecx,eax
	mov	eax,dword ptr [ebx+edi*08h+01ch]
	sub	esp,0ch
	mov	ebx,[eax]
	mov	edx,[ebp-08h];	@CBE28
	mov	esi,[ebx+024h]
	add	eax,esi
	mov	esi,[ebp-0ch];	@CBE29
	call	dword ptr [ebx+020h]
	mov	edx,[ebp+08h];	this
	mov	ecx,[ebp+014h];	buffer
	mov	ebx,[ebp+010h];	count
	or	eax,esi

; 828     for( int i = 0; i < children; ++i )
	inc	edi

; 829 	rc |= child[i].rdev->write(offset+1, count, buffer);
	add	esp,010h

; 828     for( int i = 0; i < children; ++i )
	cmp	[edx+018h],edi

; 829 	rc |= child[i].rdev->write(offset+1, count, buffer);
	mov	esi,eax

; 828     for( int i = 0; i < children; ++i )
	jg	@BLBL143
	mov	[ebp-04h],esi;	rc
@BLBL142:

; 830     return rc;
	pop	esi
	mov	eax,[ebp-04h];	rc
	pop	edi
	pop	ebx
	leave	
	ret	
write__7VMirrorFUlT1Pv	endp

; 777 VMirror::read(ULONG offset,ULONG count,PVOID buffer)

	public read__7VMirrorFUlT1Pv
read__7VMirrorFUlT1Pv	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi
	mov	edi,edx
	push	esi
	mov	[ebp+010h],ecx;	count
	sub	esp,020h

; 779     PUCHAR	copybuf = new UCHAR[count];
	mov	eax,ecx
	call	__vn__FUi
	mov	ecx,ebx

; 777 VMirror::read(ULONG offset,ULONG count,PVOID buffer)
	mov	[ebp+08h],ebx;	this

; 779     PUCHAR	copybuf = new UCHAR[count];
	mov	[ebp-018h],eax;	copybuf
	mov	edx,edi

; 780     int		goodchildren = 0;
	mov	dword ptr [ebp-010h],0h;	goodchildren

; 781     APIRET	rc = VERROR_NO_CONTENTS;
	mov	dword ptr [ebp-014h],0ff00h;	rc

; 783     for( int i = 0; i < children; ++i )
	cmp	dword ptr [ecx+018h],0h
	jle	@BLBL128
	inc	edx
	mov	[ebp-01ch],edx;	@CBE30
	mov	[ebp-018h],eax;	copybuf
	mov	ebx,eax
	xor	esi,esi
	xor	edi,edi
@BLBL129:

; 785 	if( !child[i].cfgok )
	mov	eax,[ebp+08h];	this
	lea	eax,dword ptr [eax+edi*08h]
	cmp	byte ptr [eax+021h],0h
	je	@BLBL131
	mov	[ebp-020h],esi;	@CBE31

; 788 	rc = child[i].rdev->read(offset+1, count, buffer);
	mov	eax,[eax+01ch]
	mov	edx,[ebp+014h];	buffer
	push	edx
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edx,[ebp+010h];	count
	mov	esi,[ebx+01ch]
	mov	ecx,edx
	add	eax,esi
	mov	edx,[ebp-01ch];	@CBE30
	mov	esi,[ebp-020h];	@CBE31
	call	dword ptr [ebx+018h]
	mov	ebx,[ebp-018h];	copybuf
	add	esp,010h
	mov	edx,[ebp+014h];	buffer
	mov	ecx,[ebp+010h];	count
	mov	[ebp-014h],eax;	rc

; 789 	if( rc != 0 )
	test	eax,eax
	jne	@BLBL131

; 791 	if( goodchildren == 0 )
	test	esi,esi
	jne	@BLBL133

; 792 	    memcpy(copybuf, buffer, (size_t)count);
	mov	eax,ebx
	call	memcpy
	jmp	@BLBL134
@BLBL133:

; 793 	else if( memcmp(copybuf, buffer, (size_t)count) != 0 )
	mov	ecx,[ebp+010h];	count
	mov	edx,[ebp+014h];	buffer
	mov	eax,ebx
	call	memcmp
	test	eax,eax
	je	@BLBL134

; 794 	    Verbose(0, "VMirror", "Data error when reading child %d, ignored", i);
	push	edi
	mov	ecx,offset FLAT:@CBE20
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE8
	xor	eax,eax
	call	Verbose
	add	esp,010h
@BLBL134:

; 795 	++goodchildren;
	inc	esi

; 796     }
@BLBL131:

; 783     for( int i = 0; i < children; ++i )
	mov	eax,[ebp+08h];	this
	inc	edi
	cmp	[eax+018h],edi
	jg	@BLBL129
	mov	[ebp-010h],esi;	goodchildren
@BLBL128:

; 798     delete[] copybuf;
	mov	eax,[ebp-018h];	copybuf
	call	__vd__FPv

; 799     return (goodchildren == 0 ? rc : 0);
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
read__7VMirrorFUlT1Pv	endp

; 622 VMirror::ioSync()

	public ioSync__7VMirrorFv
ioSync__7VMirrorFv	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi

; 624     PSEC_VRDEV2	sec = new SEC_VRDEV2;
	mov	eax,0200h

; 622 VMirror::ioSync()
	push	esi
	mov	[ebp+08h],ebx;	this
	sub	esp,024h

; 627     int		errors = 0;
	mov	dword ptr [ebp-014h],0h;	errors

; 624     PSEC_VRDEV2	sec = new SEC_VRDEV2;
	call	__nw__FUi
	mov	edi,eax

; 633 	rc = read(0, 1, sec);
	push	edi

; 624     PSEC_VRDEV2	sec = new SEC_VRDEV2;
	mov	[ebp-010h],edi;	sec

; 628     Boolean	update = False;			/* True: modify sector */
	mov	byte ptr [ebp-01dh],0h;	update

; 633 	rc = read(0, 1, sec);
	mov	esi,[ebx]
	sub	esp,0ch
	mov	eax,[esi+01ch]
	mov	ecx,01h
	xor	edx,edx
	add	eax,ebx
	call	dword ptr [esi+018h]
	add	esp,010h

; 634 	if( rc != 0 )
	test	eax,eax
	jne	@BLBL96

; 636 	if( memcmp(sec->sectype, "VRAIDDEVICE2    ", 16) != 0 )
	mov	eax,edi
	mov	ecx,010h
	mov	edx,offset FLAT:@CBE15
	call	memcmp
	test	eax,eax
	jne	@BLBL96

; 638 	if( Crc16(sec, sizeof(*sec)-2) != sec->crc )
	mov	eax,edi
	mov	dx,01feh
	call	Crc16__FPvUs
	cmp	[edi+01feh],ax
	mov	eax,ebx
	jne	@BLBL96

; 640 	if( memcmp(sec->u.s.id, id, sizeof(DEVID)) != 0 )
	add	eax,08h
	mov	edx,eax
	mov	ecx,06h
	lea	eax,[edi+014h]
	call	memcmp
	test	eax,eax
	jne	@BLBL96

; 642 	if( sec->u.s.type != RDTYPE_MIRROR )
	cmp	byte ptr [edi+01ah],04h
	jne	@BLBL96

; 645 	update = True;
	mov	byte ptr [ebp-01dh],01h;	update

; 647     while( 0 );
@BLBL96:

; 650     if( update == True )
	cmp	byte ptr [ebp-01dh],01h;	update
	jne	@BLBL103

; 657 	sec->timestamp = time(NULL);
	mov	ebx,[ebp-010h];	sec

; 652 	Verbose(1, "VMirror", "updating current configuration sector");
	mov	ecx,offset FLAT:@CBE16
	mov	edx,offset FLAT:@CBE8
	mov	eax,01h
	call	Verbose

; 657 	sec->timestamp = time(NULL);
	xor	eax,eax
	call	time
	mov	[ebx+010h],eax

; 658 	sec->u.s.flags = (UCHAR)(parent != 0 ? 0 : 0x80);
	mov	ecx,[ebp+08h];	this
	cmp	dword ptr [ecx+04h],0h
	setne	dl
	movzx	edx,dl
	dec	edx
	and	edx,080h
	mov	[ebx+01bh],dl

; 659 	sec->u.s.children = children;
	mov	ecx,[ecx+018h]
	mov	[ebx+01ch],cx

; 665 	ul = sec->u.s.child[0].size;		/* drive size = size of any child */
	mov	ebx,[ebx+02ah]
	mov	[ebp-018h],ebx;	ul

; 667     }
	jmp	@BLBL106
@BLBL103:

; 677 	memset(sec, 0, sizeof(*sec));
	mov	ebx,[ebp-010h];	sec

; 672 	Verbose(1, "VMirror", "creating new configuration sector");
	mov	ecx,offset FLAT:@CBE17
	mov	edx,offset FLAT:@CBE8
	mov	eax,01h
	call	Verbose

; 677 	memset(sec, 0, sizeof(*sec));
	mov	ecx,0200h
	xor	edx,edx
	mov	eax,ebx
	call	memset

; 678 	memcpy(sec->sectype, "VRAIDDEVICE2    ", 16);
	mov	ecx,010h
	mov	edx,offset FLAT:@CBE15
	mov	eax,ebx
	call	memcpy

; 679 	sec->timestamp = time(NULL);
	xor	eax,eax
	call	time
	mov	[ebx+010h],eax

; 681 	memcpy(sec->u.s.id, id, sizeof(DEVID));
	mov	edi,[ebp+08h];	this
	mov	ecx,06h
	lea	edx,[edi+08h]
	lea	eax,[ebx+014h]
	call	memcpy

; 682 	sec->u.s.type = RDTYPE_MIRROR;
	mov	byte ptr [ebx+01ah],04h

; 683 	sec->u.s.flags = (UCHAR)(parent != 0 ? 0 : 0x80);
	cmp	dword ptr [edi+04h],0h
	setne	cl
	movzx	ecx,cl
	dec	ecx
	and	ecx,080h
	mov	[ebx+01bh],cl

; 685 	sec->u.s.children = children;
	mov	ecx,[edi+018h]
	mov	[ebx+01ch],cx

; 691 	ul = (ULONG)ULONG_MAX;
	mov	dword ptr [ebp-018h],0ffffffffh;	ul

; 692 	for( i = 0; i < children; ++i )
	cmp	dword ptr [edi+018h],0h
	jle	@BLBL106
	mov	eax,edi
	mov	esi,[ebp-01ch];	__54
	or	ebx,0ffffffffh
	xor	edi,edi
@BLBL110:
	mov	ecx,eax

; 693 	    ul = min(ul, child[i].rdev->querySize());
	movzx	edx,di
	mov	eax,dword ptr [ecx+edx*08h+01ch]
	mov	ecx,[eax]
	mov	edx,[ecx+014h]
	add	eax,edx
	call	dword ptr [ecx+010h]
	mov	ecx,eax
	mov	eax,[ebp+08h];	this
	cmp	ecx,ebx
	jbe	@BLBL111
	mov	esi,ebx
	jmp	@BLBL112
@BLBL111:
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
@BLBL112:

; 692 	for( i = 0; i < children; ++i )
	mov	ebx,esi

; 693 	    ul = min(ul, child[i].rdev->querySize());
	movzx	edi,di
	inc	edi

; 692 	for( i = 0; i < children; ++i )
	movzx	ecx,di
	cmp	[eax+018h],ecx
	jg	@BLBL110
	mov	[ebp-018h],ebx;	ul
	mov	[ebp-01ch],ebx;	__54

; 695     }
@BLBL106:
	mov	ecx,[ebp+08h];	this
	mov	eax,[ebp-018h];	ul

; 700     for( i = 0; i < children; ++i )
	cmp	dword ptr [ecx+018h],0h

; 695     }
	mov	[ecx+011ch],eax

; 700     for( i = 0; i < children; ++i )
	jle	@BLBL116
	mov	ebx,[ebp-010h];	sec
	xor	edi,edi
@BLBL117:

; 702 	child[i].rdev->ioSync();
	movzx	esi,di
	lea	edx,dword ptr [ecx+esi*08h]

; 703 	memcpy(sec->u.s.child[i].id, child[i].rdev->queryID(), sizeof(DEVID));
	sal	esi,04h

; 702 	child[i].rdev->ioSync();
	mov	eax,[edx+01ch]
	mov	[ebp-024h],edx;	@CBE33
	mov	ecx,[eax]

; 703 	memcpy(sec->u.s.child[i].id, child[i].rdev->queryID(), sizeof(DEVID));
	add	esi,ebx

; 702 	child[i].rdev->ioSync();
	mov	edx,[ecx+054h]
	add	eax,edx
	call	dword ptr [ecx+050h]
	mov	eax,[ebp-024h];	@CBE33

; 703 	memcpy(sec->u.s.child[i].id, child[i].rdev->queryID(), sizeof(DEVID));
	mov	ecx,06h
	mov	edx,[eax+01ch]
	add	edx,08h
	lea	eax,[esi+01eh]
	call	memcpy
	mov	ecx,[ebp+08h];	this
	mov	edx,[ebp-024h];	@CBE33

; 704 	sec->u.s.child[i].size = size;
	mov	eax,[ecx+011ch]
	mov	[esi+02ah],eax

; 705 	if( child[i].valid == True )
	cmp	byte ptr [edx+020h],01h
	jne	@BLBL118

; 707 	    sec->u.s.child[i].flags |= 0x01;
	or	byte ptr [esi+024h],01h

; 708 	    sec->u.s.child[i].valid = (ULONG)-1; /* better clear field */
	mov	dword ptr [esi+026h],0ffffffffh

; 709 	}
	jmp	@BLBL120
@BLBL118:

; 712 	    sec->u.s.child[i].valid = 0;	/* start from beginning */
	mov	dword ptr [esi+026h],0h

; 713 	    sec->u.s.flags |= RDFLAG_BUILDING;
	or	byte ptr [ebx+01bh],01h

; 715     }
@BLBL120:

; 700     for( i = 0; i < children; ++i )
	movzx	edi,di
	inc	edi
	movzx	eax,di
	cmp	[ecx+018h],eax
	jg	@BLBL117
@BLBL116:

; 720     sec->crc = Crc16(sec, SECTOR_SIZE-2);
	mov	ebx,[ebp-010h];	sec
	mov	dx,01feh
	mov	eax,ebx
	call	Crc16__FPvUs
	mov	[ebx+01feh],ax

; 721     rc = write(0, 1, sec);
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

; 722     if( rc != 0 )
	test	eax,eax
	je	@BLBL123

; 724 	Verbose(1, "VMirror::ioSync", "write(0,1,...) - rc %lu, not updated", rc);
	push	eax

; 725 	++errors;
	inc	dword ptr [ebp-014h];	errors

; 724 	Verbose(1, "VMirror::ioSync", "write(0,1,...) - rc %lu, not updated", rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE19
	mov	edx,offset FLAT:@CBE18
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 726     }
@BLBL123:

; 728     delete sec;
	mov	eax,[ebp-010h];	sec
	call	__dl__FPv

; 729     return errors;
	mov	eax,[ebp-014h];	errors
	add	esp,024h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ioSync__7VMirrorFv	endp

; 568 VMirror::ioChangeID(DEVID newid)

	public ioChangeID__7VMirrorFPUc
ioChangeID__7VMirrorFPUc	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,edx
	push	edi
	mov	edi,eax
	push	esi

; 570     PSEC_VRDEV2	sec = new SEC_VRDEV2;
	mov	eax,0200h

; 568 VMirror::ioChangeID(DEVID newid)
	sub	esp,014h
	mov	[ebp+0ch],ebx;	newid

; 570     PSEC_VRDEV2	sec = new SEC_VRDEV2;
	call	__nw__FUi
	mov	esi,eax
	mov	edx,ebx

; 573     memcpy(id, newid, sizeof(DEVID));		/* update object */
	mov	ecx,06h
	lea	eax,[edi+08h]
	call	memcpy
	mov	eax,edi

; 579 	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
	push	esi

; 568 VMirror::ioChangeID(DEVID newid)
	mov	[ebp+08h],edi;	this

; 570     PSEC_VRDEV2	sec = new SEC_VRDEV2;
	mov	[ebp-014h],esi;	sec

; 579 	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
	mov	ecx,01h

; 571     int		errors = 0;
	mov	dword ptr [ebp-010h],0h;	errors

; 579 	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+01ch]
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+018h]
	add	esp,010h

; 580 	if( rc != 0 )
	test	eax,eax
	je	@BLBL87

; 582 	    Verbose(1, "VMirror::ioChangeID", "can't read own SEC_VRDEV2 - rc %lu", rc);
	push	eax

; 583 	    ++errors;
	mov	dword ptr [ebp-010h],01h;	errors

; 582 	    Verbose(1, "VMirror::ioChangeID", "can't read own SEC_VRDEV2 - rc %lu", rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE12
	mov	edx,offset FLAT:@CBE14
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 584 	    break;
	jmp	@BLBL88
@BLBL87:

; 587 	memcpy(sec->u.s.id, newid, sizeof(DEVID));
	mov	ebx,[ebp-014h];	sec
	mov	edx,[ebp+0ch];	newid
	mov	ecx,06h
	lea	eax,[ebx+014h]
	call	memcpy

; 588 	sec->timestamp = time(NULL);		/* UTC of change */
	xor	eax,eax
	call	time
	mov	[ebx+010h],eax

; 589 	sec->crc = Crc16(sec, SECTOR_SIZE-2);	/* !!! */
	mov	dx,01feh
	mov	eax,ebx
	call	Crc16__FPvUs
	mov	[ebx+01feh],ax

; 591 	rc = write(0, 1, sec);
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

; 592 	if( rc != 0 )
	test	eax,eax
	je	@BLBL88

; 594 	    Verbose(1, "VMirror::ioChangeID", "can't write own SEC_VRDEV2 - rc %lu", rc);
	push	eax

; 595 	    ++errors;
	inc	dword ptr [ebp-010h];	errors

; 594 	    Verbose(1, "VMirror::ioChangeID", "can't write own SEC_VRDEV2 - rc %lu", rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE13
	mov	edx,offset FLAT:@CBE14
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 596 	    break;
@BLBL88:

; 601     delete sec;
	mov	eax,[ebp-014h];	sec
	call	__dl__FPv

; 602     return errors;
	mov	eax,[ebp-010h];	errors
	add	esp,014h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ioChangeID__7VMirrorFPUc	endp

; 501 VMirror::ioRemoveParent()

	public ioRemoveParent__7VMirrorFv
ioRemoveParent__7VMirrorFv	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi

; 503     PSEC_VRDEV2	sec = new SEC_VRDEV2;
	mov	eax,0200h

; 501 VMirror::ioRemoveParent()
	push	esi
	mov	[ebp+08h],ebx;	this
	sub	esp,014h

; 504     int		errors = 0;
	xor	esi,esi

; 503     PSEC_VRDEV2	sec = new SEC_VRDEV2;
	call	__nw__FUi
	mov	edi,eax

; 510     memset(sec, 0, SECTOR_SIZE);
	mov	ecx,0200h
	xor	edx,edx
	call	memset
	mov	eax,ebx

; 511     rc = write(1, 1, sec);
	push	edi

; 503     PSEC_VRDEV2	sec = new SEC_VRDEV2;
	mov	[ebp-014h],edi;	sec

; 511     rc = write(1, 1, sec);
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+024h]
	mov	ecx,01h
	mov	edx,01h
	add	eax,edi
	call	dword ptr [ebx+020h]
	add	esp,010h

; 512     if( rc != 0 )
	test	eax,eax
	je	@BLBL78

; 514 	Verbose(1, "VMirror::ioRemoveParent", "can't clear parent SEC_VRDEV - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE11
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE10
	mov	eax,01h
	call	Verbose

; 515 	++errors;
	mov	esi,01h

; 514 	Verbose(1, "VMirror::ioRemoveParent", "can't clear parent SEC_VRDEV - rc %lu", rc);
	add	esp,010h

; 521     do
@BLBL78:

; 523 	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
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

; 524 	if( rc != 0 )
	test	eax,eax
	je	@BLBL79

; 526 	    Verbose(1, "VMirror::ioRemoveParent", "can't read own SEC_VRDEV2 - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE12
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE10
	mov	eax,01h
	call	Verbose

; 527 	    ++errors;
	inc	esi

; 526 	    Verbose(1, "VMirror::ioRemoveParent", "can't read own SEC_VRDEV2 - rc %lu", rc);
	add	esp,010h

; 528 	    break;
	jmp	@BLBL80
@BLBL79:

; 531 	sec->u.s.flags |= RDFLAG_HOSTDRIVE;	/* no parent means hostdrive */
	mov	ebx,[ebp-014h];	sec

; 532 	sec->timestamp = time(NULL);		/* UTC of change */
	xor	eax,eax

; 531 	sec->u.s.flags |= RDFLAG_HOSTDRIVE;	/* no parent means hostdrive */
	or	byte ptr [ebx+01bh],080h

; 532 	sec->timestamp = time(NULL);		/* UTC of change */
	call	time
	mov	[ebx+010h],eax

; 533 	sec->crc = Crc16(sec, SECTOR_SIZE-2);	/* !!! */
	mov	dx,01feh
	mov	eax,ebx
	call	Crc16__FPvUs
	mov	[ebx+01feh],ax

; 535 	rc = write(0, 1, sec);
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

; 536 	if( rc != 0 )
	test	eax,eax
	je	@BLBL81

; 538 	    Verbose(1, "VMirror::ioRemoveParent", "can't write own SEC_VRDEV2 - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE13
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE10
	mov	eax,01h
	call	Verbose

; 539 	    ++errors;
	inc	esi

; 538 	    Verbose(1, "VMirror::ioRemoveParent", "can't write own SEC_VRDEV2 - rc %lu", rc);
	add	esp,010h

; 540 	    break;
	jmp	@BLBL80
@BLBL81:

; 543 	parent = NULL;				/* none */
	mov	eax,[ebp+08h];	this
	mov	dword ptr [eax+04h],0h

; 545     while(0);
@BLBL80:

; 547     delete sec;
	mov	eax,[ebp-014h];	sec
	call	__dl__FPv

; 548     return errors;
	add	esp,014h
	mov	eax,esi
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ioRemoveParent__7VMirrorFv	endp

; 467 VMirror::ioDeleteYourself()

	public ioDeleteYourself__7VMirrorFv
ioDeleteYourself__7VMirrorFv	proc
	push	ebx
	push	edi
	push	esi
	sub	esp,0ch
	mov	[esp+01ch],eax;	this

; 470     int		errors = 0;
	mov	dword ptr [esp+08h],0h;	errors

; 474     for( i = 0; i < children; ++i )
	cmp	dword ptr [eax+018h],0h
	jle	@BLBL70
	mov	ebx,eax
	xor	esi,esi
	xor	edi,edi
@BLBL71:

; 476 	errors += child[i].rdev->ioRemoveParent();
	lea	ecx,dword ptr [ebx+edi*08h]

; 474     for( i = 0; i < children; ++i )
	inc	edi

; 476 	errors += child[i].rdev->ioRemoveParent();
	mov	eax,[ecx+01ch]
	mov	[esp+04h],ecx;	@CBE34
	mov	ecx,[eax]
	mov	edx,[ecx+044h]
	add	eax,edx
	call	dword ptr [ecx+040h]
	mov	ecx,[esp+04h];	@CBE34
	add	eax,esi

; 477 	child[i].rdev = NULL;
	mov	dword ptr [ecx+01ch],0h

; 474     for( i = 0; i < children; ++i )
	cmp	[ebx+018h],edi

; 476 	errors += child[i].rdev->ioRemoveParent();
	mov	esi,eax

; 474     for( i = 0; i < children; ++i )
	jg	@BLBL71
	mov	[esp+08h],esi;	errors
@BLBL70:

; 480     return errors;
	mov	eax,[esp+08h];	errors
	add	esp,0ch
	pop	esi
	pop	edi
	pop	ebx
	ret	
ioDeleteYourself__7VMirrorFv	endp

; 401 VMirror::ioRemoveChild(VRDev * rdev)

	public ioRemoveChild__7VMirrorFP5VRDev
ioRemoveChild__7VMirrorFP5VRDev	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	[ebp+0ch],edx;	rdev
	push	edi
	mov	[ebp+08h],eax;	this
	push	esi
	sub	esp,018h

; 404     int		errors = 0;
	mov	dword ptr [ebp-010h],0h;	errors

; 405     Boolean	found = False;
	mov	byte ptr [ebp-011h],0h;	found

; 412 	for( i = 0; i < children; ++i )
	cmp	dword ptr [eax+018h],0h
	jle	@BLBL53
	mov	esi,eax
	xor	ebx,ebx
@BLBL54:

; 414 	    if( child[i].rdev == rdev )
	mov	eax,dword ptr [esi+ebx*08h+01ch]
	cmp	[ebp+0ch],eax;	rdev
	jne	@BLBL62

; 418 		errors += child[i].rdev->ioRemoveParent();
	mov	ecx,[eax]

; 419 		for( j = i, k = i+1; k < children; ++j, ++k )
	lea	edi,[ebx+01h]

; 418 		errors += child[i].rdev->ioRemoveParent();
	mov	edx,[ecx+044h]
	add	eax,edx
	call	dword ptr [ecx+040h]
	add	[ebp-010h],eax;	errors

; 419 		for( j = i, k = i+1; k < children; ++j, ++k )
	cmp	[esi+018h],edi
	jle	@BLBL56
	mov	ecx,ebx
	mov	[ebp-018h],ecx;	i
@BLBL57:

; 421 		    child[j].rdev = child[k].rdev;
	lea	ebx,dword ptr [esi+edi*08h]
	lea	eax,dword ptr [esi+ecx*08h]
	mov	edx,[ebx+01ch]
	mov	[eax+01ch],edx

; 422 		    child[j].valid = child[k].valid;
	mov	bl,[ebx+020h]

; 419 		for( j = i, k = i+1; k < children; ++j, ++k )
	inc	edi
	inc	ecx

; 422 		    child[j].valid = child[k].valid;
	mov	[eax+020h],bl

; 419 		for( j = i, k = i+1; k < children; ++j, ++k )
	cmp	[esi+018h],edi
	jg	@BLBL57
	mov	ebx,[ebp-018h];	i
@BLBL56:

; 424 		--children;
	dec	dword ptr [esi+018h]

; 425 		found = True;
	mov	byte ptr [ebp-011h],01h;	found

; 426 		break;
	jmp	@BLBL53
@BLBL62:

; 412 	for( i = 0; i < children; ++i )
	inc	ebx
	cmp	[esi+018h],ebx
	jg	@BLBL54
@BLBL53:

; 429 	if( !found )
	cmp	byte ptr [ebp-011h],0h;	found
	jne	@BLBL64

; 431 	    Verbose(1, "VMirror", "can't find child to remove!");
	mov	ecx,offset FLAT:@CBE9
	mov	edx,offset FLAT:@CBE8
	mov	eax,01h
	call	Verbose

; 432 	    ++errors;
	mov	eax,[ebp-010h];	errors
	inc	eax

; 433 	    break;
	jmp	@BLBL65
@BLBL64:

; 439 	errors += ioSync();
	mov	eax,[ebp+08h];	this
	mov	ebx,[eax]
	mov	ecx,[ebx+054h]
	add	eax,ecx
	call	dword ptr [ebx+050h]
	add	eax,[ebp-010h];	errors

; 441     while(0);
@BLBL65:

; 443     return errors;
	add	esp,018h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ioRemoveChild__7VMirrorFP5VRDev	endp

; 295 VMirror::queryIcon()

	public queryIcon__7VMirrorFv
queryIcon__7VMirrorFv	proc
	push	ebx

; 297     if( parent != 0  &&  parent->isValidChild(this) == False )
	mov	ebx,[eax+04h]

; 295 VMirror::queryIcon()
	push	edi
	sub	esp,08h

; 297     if( parent != 0  &&  parent->isValidChild(this) == False )
	test	ebx,ebx
	je	@BLBL36
	xchg	ebx,eax
	mov	edx,ebx
	mov	ecx,[eax]
	mov	edi,[ecx+0ach]
	add	eax,edi
	call	dword ptr [ecx+0a8h]
	test	al,al
	mov	eax,ebx
	jne	@BLBL36

; 298 	return inv_icon;
	mov	eax,[eax+0124h]
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
@BLBL36:

; 299     else if( writable == True )
	cmp	byte ptr [eax+0120h],01h
	jne	@BLBL38

; 300 	return rw_icon;
	mov	eax,[eax+0128h]
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
@BLBL38:

; 302 	return ro_icon;
	mov	eax,[eax+012ch]
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
queryIcon__7VMirrorFv	endp

; 256 char *

	public allocateDescription__7VMirrorFv
allocateDescription__7VMirrorFv	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi

; 260     char *	descr = new char[80];
	mov	eax,050h

; 256 char *
	push	esi
	mov	[ebp+08h],ebx;	this
	sub	esp,0ch

; 261     int		found = 0;
	mov	dword ptr [ebp-014h],0h;	found

; 260     char *	descr = new char[80];
	call	__vn__FUi
	mov	[ebp-010h],eax;	descr
	mov	eax,ebx

; 263     for( int i = 0; i < children; ++i )
	cmp	dword ptr [eax+018h],0h
	jle	@BLBL26
	xor	esi,esi
	xor	edi,edi
@BLBL27:

; 264 	if( child[i].rdev != 0  &&  child[i].rdev->isArray() != 0xFF )
	mov	ebx,dword ptr [eax+edi*08h+01ch]
	test	ebx,ebx
	je	@BLBL29
	xchg	ebx,esi
	xchg	esi,eax

; 265 	    ++found;
	mov	ecx,[eax]
	mov	edx,[ecx+064h]
	add	eax,edx
	call	dword ptr [ecx+060h]
	cmp	al,0ffh
	setne	al
	movzx	edx,al
	mov	eax,esi
	mov	ecx,ebx
	mov	esi,edx
	add	esi,ecx
@BLBL29:

; 263     for( int i = 0; i < children; ++i )
	inc	edi
	cmp	[eax+018h],edi
	jg	@BLBL27
	mov	[ebp-014h],esi;	found
@BLBL26:

; 266     if( found < children )
	mov	eax,[ebp+08h];	this
	mov	ebx,[ebp-014h];	found
	cmp	[eax+018h],ebx
	jle	@BLBL32

; 267 	state = Fail;				/* always! */
	mov	byte ptr [eax+0130h],02h
@BLBL32:

; 269     sprintf(descr, "RAID 1 (Mirroring, %s)", State2Str[state]);
	mov	ebx,[ebp+08h];	this
	mov	edx,offset FLAT:@CBE5
	movzx	ecx,byte ptr [ebx+0130h]
	push	dword ptr [ecx*04h+@57State2Str]
	sub	esp,08h
	mov	esi,[ebp-010h];	descr
	mov	eax,esi
	call	_sprintfieee
	add	esp,0ch

; 271     if( hostdrive == True  &&  os2access != -1UL )
	cmp	byte ptr [ebx+010h],01h
	jne	@BLBL33
	cmp	dword ptr [ebx+014h],0ffffffffh
	je	@BLBL33

; 272 	sprintf(&descr[strlen(descr)], " (OS/2 Drive %lu)", os2access);
	mov	eax,esi
	call	strlen
	push	dword ptr [ebx+014h]
	mov	ecx,eax
	sub	esp,08h
	mov	eax,esi
	add	eax,ecx
	mov	edx,offset FLAT:@CBE6
	call	_sprintfieee
	add	esp,0ch
@BLBL33:

; 274     sprintf(&descr[strlen(descr)], "\nID %02X%02X%02X%02X%02X%02X    %lu MByte",
	mov	esi,[ebp-010h];	descr
	mov	eax,esi
	call	strlen
	mov	ecx,[ebp+08h];	this
	add	eax,esi
	mov	edx,[ecx+011ch]
	shr	edx,01h
	shr	edx,0ah
	push	edx
	movzx	edx,byte ptr [ecx+0dh]
	push	edx
	movzx	edx,byte ptr [ecx+0ch]
	push	edx
	movzx	edx,byte ptr [ecx+0bh]
	push	edx
	movzx	edx,byte ptr [ecx+0ah]
	push	edx
	movzx	edx,byte ptr [ecx+09h]
	push	edx
	movzx	ecx,byte ptr [ecx+08h]
	push	ecx
	mov	edx,offset FLAT:@CBE7
	sub	esp,08h
	call	_sprintfieee

; 276     return descr;
	add	esp,030h

; 274     sprintf(&descr[strlen(descr)], "\nID %02X%02X%02X%02X%02X%02X    %lu MByte",
	mov	eax,esi

; 276     return descr;
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
allocateDescription__7VMirrorFv	endp

; 295 };

__dt__20VDriveWithRedundancyFv	proc
	push	ebp
	xor	ecx,ecx
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffe0h
	sub	esp,018h
	mov	[ebp-024h],edx;	@CBE35
	push	ebx
	mov	ebx,eax
	push	edi
	xor	edx,edx
	push	esi
	sub	esp,0ch
	mov	dword ptr [ebp-020h],0h;	__es
	mov	dword ptr [ebp-018h],offset FLAT:@f3__fsm_tab62;	__es
	mov	[ebp-014h],ebx;	__es
	mov	dword ptr [ebp-010h],0h;	__es
	mov	dword ptr [ebx],offset FLAT:@de__vft20VDriveWithRedundancy5VDisk
	mov	dword ptr [ebp-020h],0h;	__es
	call	__dt__7VRDriveFv
	mov	edx,[ebp-024h];	@CBE35
	add	esp,0ch
	mov	eax,ebx
	test	dl,01h
	je	@BLBL156
	sub	esp,04h
	call	__dl__FPv
	mov	eax,ebx
	add	esp,04h
@BLBL156:
	pop	esi
	pop	edi
	pop	ebx
	add	esp,01ch
	pop	fs:[0h]
	leave	
	ret	
__dt__20VDriveWithRedundancyFv	endp

; 143 VMirror::~VMirror()

	public __dt__7VMirrorFv
__dt__7VMirrorFv	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffe0h
	sub	esp,018h

; 144 {
	mov	dword ptr [ebp-020h],0h;	__es

; 143 VMirror::~VMirror()
	push	ebx
	mov	ebx,eax
	push	edi
	mov	[ebp-024h],edx;	@CBE36
	push	esi
	mov	[ebp+08h],ebx;	this

; 144 {
	mov	dword ptr [ebp-018h],offset FLAT:@36__fsm_tab;	__es
	mov	[ebp-014h],ebx;	__es
	mov	dword ptr [ebp-010h],0h;	__es

; 143 VMirror::~VMirror()
	mov	dword ptr [ebp-020h],01h;	__es

; 145     WinDestroyPointer(rw_icon);
	push	dword ptr [ebx+0128h]

; 143 VMirror::~VMirror()
	mov	dword ptr [ebx],offset FLAT:__vft7VMirror5VDisk

; 145     WinDestroyPointer(rw_icon);
	call	WinDestroyPointer
	add	esp,04h

; 146     WinDestroyPointer(ro_icon);
	push	dword ptr [ebx+012ch]
	call	WinDestroyPointer
	sub	esp,08h

; 147 }
	mov	dword ptr [ebp-020h],0h;	__es
	xor	ecx,ecx
	xor	edx,edx
	mov	eax,ebx
	call	__dt__20VDriveWithRedundancyFv
	mov	edx,[ebp-024h];	@CBE36
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
__dt__7VMirrorFv	endp

; 295 };

__ct__20VDriveWithRedundancyFv	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffe0h
	sub	esp,014h
	mov	dword ptr [ebp-020h],0h;	__es
	push	ebx
	mov	dword ptr [ebp-018h],offset FLAT:@e0__fsm_tab60;	__es
	push	edi
	mov	ebx,eax
	push	esi
	sub	esp,04h
	mov	[ebp-014h],ebx;	__es
	mov	dword ptr [ebp-010h],0h;	__es
	call	__ct__7VRDriveFv
	add	esp,04h
	mov	eax,ebx
	pop	esi
	mov	dword ptr [ebp-020h],01h;	__es
	pop	edi
	mov	dword ptr [eax],offset FLAT:@de__vft20VDriveWithRedundancy5VDisk
	pop	ebx
	add	esp,018h
	pop	fs:[0h]
	leave	
	ret	
__ct__20VDriveWithRedundancyFv	endp

; 112 VMirror::VMirror(DEVID drive_id,int nchd)

	public __ct__7VMirrorFPUci
__ct__7VMirrorFPUci	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffe0h
	sub	esp,014h

; 113 {
	mov	dword ptr [ebp-020h],0h;	__es

; 112 VMirror::VMirror(DEVID drive_id,int nchd)
	push	ebx

; 113 {
	mov	dword ptr [ebp-018h],offset FLAT:@1f__fsm_tab;	__es

; 112 VMirror::VMirror(DEVID drive_id,int nchd)
	push	edi
	mov	ebx,eax
	push	esi
	mov	edi,edx

; 112 VMirror::VMirror(DEVID drive_id,int nchd)
	sub	esp,04h

; 113 {
	mov	[ebp-014h],ebx;	__es
	mov	dword ptr [ebp-010h],0h;	__es

; 112 VMirror::VMirror(DEVID drive_id,int nchd)
	call	__ct__20VDriveWithRedundancyFv
	sub	esp,08h
	mov	dword ptr [ebp-020h],01h;	__es
	mov	edx,edi
	mov	dword ptr [ebx],offset FLAT:__vft7VMirror5VDisk

; 114     parent = NULL;
	mov	dword ptr [ebx+04h],0h

; 120     memcpy(id, drive_id, sizeof(DEVID));
	mov	ecx,06h

; 115     children = 0;
	mov	dword ptr [ebx+018h],0h

; 116     size = -1ul;
	mov	dword ptr [ebx+011ch],0ffffffffh

; 120     memcpy(id, drive_id, sizeof(DEVID));
	lea	eax,[ebx+08h]

; 117     hostdrive = False;
	mov	byte ptr [ebx+010h],0h

; 118     writable = True;
	mov	byte ptr [ebx+0120h],01h

; 119     state = Ready;
	mov	byte ptr [ebx+0130h],0h

; 120     memcpy(id, drive_id, sizeof(DEVID));
	call	memcpy
	add	esp,0ch

; 122     inv_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_INVCHILDARRAY);
	push	0199h
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	[ebx+0124h],eax

; 123     rw_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_RAIDDRIVE);
	push	0194h
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	[ebx+0128h],eax

; 124     ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDARRAY);
	push	0197h
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	ecx,eax

; 125 }
	pop	esi

; 124     ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDARRAY);
	mov	eax,ebx

; 125 }
	pop	edi

; 124     ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDARRAY);
	mov	[eax+012ch],ecx

; 125 }
	pop	ebx
	add	esp,018h
	pop	fs:[0h]
	leave	
	ret	
__ct__7VMirrorFPUci	endp

; 123     virtual	~VRDev() {}			// see Stroustroup pg216!!!

__dt__5VRDevFv	proc
	push	ebx
	mov	dword ptr [eax],offset FLAT:@13c__vft5VRDev5VDisk
	sub	esp,04h
	test	dl,01h
	je	@BLBL184
	mov	ebx,eax
	call	__dl__FPv
	mov	eax,ebx
@BLBL184:
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
	mov	dword ptr [ebp-018h],offset FLAT:@129__fsm_tab66;	__es
	push	edi
	mov	[ebp-014h],eax;	__es
	mov	dword ptr [ebp-010h],0h;	__es
	push	esi
	mov	dword ptr [eax],offset FLAT:@114__vft7VRDrive5VDisk
	mov	dword ptr [ebp-020h],0h;	__es

; 123     virtual	~VRDev() {}			// see Stroustroup pg216!!!
	test	dl,01h
	mov	dword ptr [eax],offset FLAT:@13c__vft5VRDev5VDisk
	je	@BLBL178
	mov	ebx,eax
	sub	esp,04h
	call	__dl__FPv
	mov	eax,ebx
	add	esp,04h
@BLBL178:
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
	mov	dword ptr [eax],offset FLAT:@13c__vft5VRDev5VDisk
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
	mov	dword ptr [ebp-018h],offset FLAT:@116__fsm_tab64;	__es
	push	edi
	mov	[ebp-014h],eax;	__es
	push	esi
	mov	dword ptr [ebp-010h],0h;	__es

; 279 };
	pop	esi

; 80 };
	mov	dword ptr [eax],offset FLAT:@13c__vft5VRDev5VDisk

; 279 };
	pop	edi

; 143 };
	mov	dword ptr [ebp-020h],01h;	__es

; 279 };
	pop	ebx

; 143 };
	mov	dword ptr [eax],offset FLAT:@114__vft7VRDrive5VDisk

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

__dftbdt__20VDriveWithRedundancyFv	proc
	xor	ecx,ecx
	sub	esp,0ch
	xor	edx,edx
	call	__dt__20VDriveWithRedundancyFv
	add	esp,0ch
	ret	
__dftbdt__20VDriveWithRedundancyFv	endp

; 94     Boolean	isWritable() { return writable; }
EH_CODE	ends
CODE32	segment

isWritable__7VMirrorFv	proc
	movzx	eax,byte ptr [eax+0120h]
	ret	
isWritable__7VMirrorFv	endp

; 93     void	forceUnwritable() { writable = False; }

forceUnwritable__7VMirrorFv	proc
	mov	byte ptr [eax+0120h],0h
	ret	
forceUnwritable__7VMirrorFv	endp

; 82     Boolean	isHostdrive() { return hostdrive; }

isHostdrive__7VMirrorFv	proc
	movzx	eax,byte ptr [eax+010h]
	ret	
isHostdrive__7VMirrorFv	endp

; 81     UCHAR	isArray() { return RDTYPE_MIRROR; }

isArray__7VMirrorFv	proc
	mov	eax,04h
	ret	
isArray__7VMirrorFv	endp

; 137     virtual VOS2Disk *	queryWhere() { return NULL; }

queryWhere__5VRDevFv	proc
	xor	eax,eax
	ret	
queryWhere__5VRDevFv	endp

; 351 

	public addChild__7VMirrorFP5VRDev7BooleanT2
addChild__7VMirrorFP5VRDev7BooleanT2	proc
	push	ebx
	mov	ebx,eax
	sub	esp,0ch

; 357 
	mov	eax,[ebx+018h]
	mov	dword ptr [ebx+eax*08h+01ch],edx

; 358 
	mov	eax,[ebx+018h]
	mov	byte ptr [ebx+eax*08h+021h],cl
	mov	[esp+08h],ecx;	@CBE41

; 359 
	mov	eax,[ebx+018h]
	mov	cl,[esp+020h];	datav
	mov	byte ptr [ebx+eax*08h+020h],cl

; 360 
; 361 
; 362 
; 363 
; 364 
	mov	ecx,[edx]

; 125     void		setParent(VRDrive * newparent) { parent = newparent; }
	mov	[edx+04h],ebx

; 364     ULONG	childsize = newchild->querySize();
	mov	eax,[ecx+014h]
	add	eax,edx
	mov	[esp+04h],edx;	@CBE42
	call	dword ptr [ecx+010h]
	xchg	ebx,eax
	mov	edx,[esp+04h];	@CBE42
	mov	ecx,[esp+08h];	@CBE41

; 365     if( size > childsize )
	cmp	[eax+011ch],ebx
	jbe	@BLBL44

; 366 	size = childsize;
	mov	[eax+011ch],ebx
@BLBL44:
	mov	ebx,eax
	mov	[esp+04h],ecx;	@CBE42

; 367     if( newchild->isWritable() == False )
	mov	ecx,[edx]
	mov	eax,[ecx+0ch]
	add	edx,eax
	mov	eax,edx
	call	dword ptr [ecx+08h]
	test	al,al
	mov	ecx,[esp+04h];	@CBE42
	mov	eax,ebx
	jne	@BLBL45

; 368 	writable = False;		
; 368 	/* oups, it isn't 'changable' */
	mov	byte ptr [eax+0120h],0h
@BLBL45:

; 370     if( state == Ready )
	cmp	byte ptr [eax+0130h],0h
	jne	@BLBL46

; 372 	if( !cfgv )
	test	cl,cl
	jne	@BLBL47

; 373 	    state = Build;
	mov	byte ptr [eax+0130h],01h
	jmp	@BLBL46
@BLBL47:

; 374 	else if( !datav )
	cmp	byte ptr [esp+020h],0h;	datav
	jne	@BLBL46

; 375 	    state = Fail;
	mov	byte ptr [eax+0130h],02h

; 376     }
@BLBL46:

; 377     ++children;
	inc	dword ptr [eax+018h]

; 378 }
	add	esp,0ch
	pop	ebx
	ret	
addChild__7VMirrorFP5VRDev7BooleanT2	endp
CODE32	ends
end
