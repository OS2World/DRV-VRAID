	title	Missing.cpp
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
	public	__vft8VMissing5VDisk
	extrn	memcpy:proc
	extrn	WinLoadPointer:proc
	extrn	_Exception_CPP:proc
	extrn	WinDestroyPointer:proc
	extrn	__dl__FPv:proc
	extrn	__vn__FUi:proc
	extrn	_sprintfieee:proc
	extrn	__PureVirtualCalled:proc
	extrn	_fltused:dword
CONST32_RO	segment
@CBE1	db "Missing device (unknown)"
db 0ah,"ID %02X%02X%02X%02X%02X"
db "%02X    %lu MByte",0h
	align 04h
@1vcid	db "$Id: missing.cpp,v 1.4 2"
db "000/04/10 00:48:36 vitus"
db " Exp $",0h
CONST32_RO	ends
EH_DATA	segment
@15__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__20VDriveWithRedundancyFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@2c__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__20VDriveWithRedundancyFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@79__fsm_tab45	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__7VRDriveFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@8c__fsm_tab47	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__7VRDriveFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@b8__fsm_tab49	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__5VRDevFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@cb__fsm_tab51	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__5VRDevFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
EH_DATA	ends
_VFT	segment
__vft8VMissing5VDisk	dd 0
	db 0h,0h,0h,0h
	dd offset FLAT:isWritable__8VMissingFv
	db 0h,0h,0h,0h
	dd offset FLAT:querySize__8VMissingFv
	db 0h,0h,0h,0h
	dd offset FLAT:read__8VMissingFUlT1Pv
	db 0h,0h,0h,0h
	dd offset FLAT:write__8VMissingFUlT1Pv
	db 0h,0h,0h,0h
	dd offset FLAT:__dt__8VMissingFv
	db 0h,0h,0h,0h
	dd offset FLAT:forceUnwritable__8VMissingFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioDeleteYourself__8VMissingFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioRemoveParent__8VMissingFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioChangeID__8VMissingFPUc
	db 0h,0h,0h,0h
	dd offset FLAT:ioSync__8VMissingFv
	db 0h,0h,0h,0h
	dd offset FLAT:queryWhere__5VRDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:isArray__8VMissingFv
	db 0h,0h,0h,0h
	dd offset FLAT:isHostdrive__8VMissingFv
	db 0h,0h,0h,0h
	dd offset FLAT:allocateDescription__8VMissingFv
	db 0h,0h,0h,0h
	dd offset FLAT:queryIcon__8VMissingFv
	db 0h,0h,0h,0h
	dd offset FLAT:setHostdrive__8VMissingFUl
	db 0h,0h,0h,0h
	dd offset FLAT:addChild__8VMissingFP5VRDev7BooleanT2
	db 0h,0h,0h,0h
	dd offset FLAT:queryChildren__8VMissingFv
	db 0h,0h,0h,0h
	dd offset FLAT:getChild__8VMissingFi
	db 0h,0h,0h,0h
	dd offset FLAT:isValidChild__7VRDriveFi
	db 0h,0h,0h,0h
	dd offset FLAT:isValidChild__7VRDriveFP5VRDev
	db 0h,0h,0h,0h
	dd offset FLAT:setBuild__8VMissingFUl
	db 0h,0h,0h,0h
	dd offset FLAT:ioRemoveChild__8VMissingFP5VRDev
	db 0h,0h,0h,0h
@77__vft20VDriveWithRedundancy5VDisk	dd 0
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
@b6__vft7VRDrive5VDisk	dd 0
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
@de__vft5VRDev5VDisk	dd 0
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
@e5__vft5VDisk	dd 0
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

; 288 VMissing::write(ULONG offset,ULONG count,PVOID buffer)

	public write__8VMissingFUlT1Pv
write__8VMissingFUlT1Pv	proc

; 292     return ERROR_INVALID_HANDLE;
	mov	eax,06h
	ret	
write__8VMissingFUlT1Pv	endp

; 261 VMissing::read(ULONG offset,ULONG count,PVOID buffer)

	public read__8VMissingFUlT1Pv
read__8VMissingFUlT1Pv	proc

; 263     return ERROR_INVALID_HANDLE;
	mov	eax,06h
	ret	
read__8VMissingFUlT1Pv	endp

; 233 VMissing::querySize()

	public querySize__8VMissingFv
querySize__8VMissingFv	proc

; 235     return size;
	mov	eax,[eax+010h]
	ret	
querySize__8VMissingFv	endp

; 211 VMissing::ioSync()

	public ioSync__8VMissingFv
ioSync__8VMissingFv	proc

; 213     return 0;
	xor	eax,eax
	ret	
ioSync__8VMissingFv	endp

; 141 VMissing::queryIcon()

	public queryIcon__8VMissingFv
queryIcon__8VMissingFv	proc

; 143     return icon;
	mov	eax,[eax+014h]
	ret	
queryIcon__8VMissingFv	endp

; 187 VMissing::ioChangeID(DEVID newid)

	public ioChangeID__8VMissingFPUc
ioChangeID__8VMissingFPUc	proc
	sub	esp,0ch

; 189     memcpy(id, newid, sizeof(DEVID));		/* update object */
	add	eax,08h
	mov	ecx,06h
	call	memcpy

; 190     return 0;
	add	esp,0ch
	xor	eax,eax
	ret	
ioChangeID__8VMissingFPUc	endp

; 164 VMissing::ioRemoveParent()

	public ioRemoveParent__8VMissingFv
ioRemoveParent__8VMissingFv	proc
	push	ebx
	push	edi
	sub	esp,0ch

; 166     delete this;
	test	eax,eax
	je	@BLBL14
	mov	ebx,[eax]
	xor	ecx,ecx
	mov	edi,[ebx+02ch]
	mov	edx,03h
	add	eax,edi
	call	dword ptr [ebx+028h]
@BLBL14:

; 167     return 0;					/* no errors */
	xor	eax,eax
	add	esp,0ch
	pop	edi
	pop	ebx
	ret	
ioRemoveParent__8VMissingFv	endp

; 113 char *

	public allocateDescription__8VMissingFv
allocateDescription__8VMissingFv	proc
	push	ebx
	mov	ebx,eax
	sub	esp,04h

; 116     char * descr = new char[80];
	mov	eax,050h
	call	__vn__FUi
	xchg	ebx,eax

; 117     sprintf(descr, "Missing device (unknown)\n"
	mov	edx,offset FLAT:@CBE1
	mov	ecx,[eax+010h]
	shr	ecx,01h
	shr	ecx,0ah
	push	ecx
	movzx	ecx,byte ptr [eax+0dh]
	push	ecx
	movzx	ecx,byte ptr [eax+0ch]
	push	ecx
	movzx	ecx,byte ptr [eax+0bh]
	push	ecx
	movzx	ecx,byte ptr [eax+0ah]
	push	ecx
	movzx	ecx,byte ptr [eax+09h]
	push	ecx
	movzx	ecx,byte ptr [eax+08h]
	push	ecx
	mov	eax,ebx
	sub	esp,08h
	call	_sprintfieee

; 121     return descr;
	add	esp,028h

; 117     sprintf(descr, "Missing device (unknown)\n"
	mov	eax,ebx

; 121     return descr;
	pop	ebx
	ret	
allocateDescription__8VMissingFv	endp

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
	mov	[ebp-024h],edx;	@CBE2
	push	ebx
	mov	ebx,eax
	push	edi
	xor	edx,edx
	push	esi
	sub	esp,0ch
	mov	dword ptr [ebp-020h],0h;	__es
	mov	dword ptr [ebp-018h],offset FLAT:@8c__fsm_tab47;	__es
	mov	[ebp-014h],ebx;	__es
	mov	dword ptr [ebp-010h],0h;	__es
	mov	dword ptr [ebx],offset FLAT:@77__vft20VDriveWithRedundancy5VDisk
	mov	dword ptr [ebp-020h],0h;	__es
	call	__dt__7VRDriveFv
	mov	edx,[ebp-024h];	@CBE2
	add	esp,0ch
	mov	eax,ebx
	test	dl,01h
	je	@BLBL36
	sub	esp,04h
	call	__dl__FPv
	mov	eax,ebx
	add	esp,04h
@BLBL36:
	pop	esi
	pop	edi
	pop	ebx
	add	esp,01ch
	pop	fs:[0h]
	leave	
	ret	
__dt__20VDriveWithRedundancyFv	endp

; 92 VMissing::~VMissing()

	public __dt__8VMissingFv
__dt__8VMissingFv	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffe0h
	sub	esp,018h

; 93 {
	mov	dword ptr [ebp-020h],0h;	__es

; 92 VMissing::~VMissing()
	push	ebx
	mov	ebx,eax
	push	edi
	mov	[ebp-024h],edx;	@CBE3
	push	esi
	mov	[ebp+08h],ebx;	this

; 93 {
	mov	dword ptr [ebp-018h],offset FLAT:@2c__fsm_tab;	__es
	mov	[ebp-014h],ebx;	__es
	mov	dword ptr [ebp-010h],0h;	__es

; 92 VMissing::~VMissing()
	mov	dword ptr [ebp-020h],01h;	__es

; 94     WinDestroyPointer(icon);
	push	dword ptr [ebx+014h]

; 92 VMissing::~VMissing()
	mov	dword ptr [ebx],offset FLAT:__vft8VMissing5VDisk

; 94     WinDestroyPointer(icon);
	call	WinDestroyPointer
	sub	esp,08h

; 95 }
	mov	dword ptr [ebp-020h],0h;	__es
	xor	ecx,ecx
	xor	edx,edx
	mov	eax,ebx
	call	__dt__20VDriveWithRedundancyFv
	mov	edx,[ebp-024h];	@CBE3
	mov	eax,ebx
	add	esp,0ch
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
__dt__8VMissingFv	endp

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
	mov	dword ptr [ebp-018h],offset FLAT:@79__fsm_tab45;	__es
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
	mov	dword ptr [eax],offset FLAT:@77__vft20VDriveWithRedundancy5VDisk
	pop	ebx
	add	esp,018h
	pop	fs:[0h]
	leave	
	ret	
__ct__20VDriveWithRedundancyFv	endp

; 69 VMissing::VMissing(DEVID drive_id,ULONG mysize)

	public __ct__8VMissingFPUcUl
__ct__8VMissingFPUcUl	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffe0h
	sub	esp,014h

; 70 {
	mov	dword ptr [ebp-020h],0h;	__es

; 69 VMissing::VMissing(DEVID drive_id,ULONG mysize)
	push	ebx

; 70 {
	mov	dword ptr [ebp-018h],offset FLAT:@15__fsm_tab;	__es

; 69 VMissing::VMissing(DEVID drive_id,ULONG mysize)
	push	edi
	mov	ebx,eax
	push	esi
	mov	esi,edx

; 69 VMissing::VMissing(DEVID drive_id,ULONG mysize)
	sub	esp,04h

; 69 VMissing::VMissing(DEVID drive_id,ULONG mysize)
	mov	edi,ecx

; 70 {
	mov	[ebp-014h],ebx;	__es
	mov	dword ptr [ebp-010h],0h;	__es

; 69 VMissing::VMissing(DEVID drive_id,ULONG mysize)
	call	__ct__20VDriveWithRedundancyFv
	sub	esp,08h
	mov	dword ptr [ebp-020h],01h;	__es
	mov	ecx,edi

; 71     size = mysize;
	mov	[ebx+010h],ecx

; 69 VMissing::VMissing(DEVID drive_id,ULONG mysize)
	mov	edx,esi
	mov	dword ptr [ebx],offset FLAT:__vft8VMissing5VDisk

; 72     memcpy(id, drive_id, sizeof(DEVID));
	mov	ecx,06h
	lea	eax,[ebx+08h]
	call	memcpy
	add	esp,0ch

; 73     icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_MISSINGDRIVE);
	push	0195h
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	ecx,eax

; 74 }
	pop	esi

; 73     icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_MISSINGDRIVE);
	mov	eax,ebx

; 74 }
	pop	edi

; 73     icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_MISSINGDRIVE);
	mov	[eax+014h],ecx

; 74 }
	pop	ebx
	add	esp,018h
	pop	fs:[0h]
	leave	
	ret	
__ct__8VMissingFPUcUl	endp

; 123     virtual	~VRDev() {}			// see Stroustroup pg216!!!

__dt__5VRDevFv	proc
	push	ebx
	mov	dword ptr [eax],offset FLAT:@de__vft5VRDev5VDisk
	sub	esp,04h
	test	dl,01h
	je	@BLBL74
	mov	ebx,eax
	call	__dl__FPv
	mov	eax,ebx
@BLBL74:
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
	mov	dword ptr [ebp-018h],offset FLAT:@cb__fsm_tab51;	__es
	push	edi
	mov	[ebp-014h],eax;	__es
	mov	dword ptr [ebp-010h],0h;	__es
	push	esi
	mov	dword ptr [eax],offset FLAT:@b6__vft7VRDrive5VDisk
	mov	dword ptr [ebp-020h],0h;	__es

; 123     virtual	~VRDev() {}			// see Stroustroup pg216!!!
	test	dl,01h
	mov	dword ptr [eax],offset FLAT:@de__vft5VRDev5VDisk
	je	@BLBL68
	mov	ebx,eax
	sub	esp,04h
	call	__dl__FPv
	mov	eax,ebx
	add	esp,04h
@BLBL68:
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
	mov	dword ptr [eax],offset FLAT:@de__vft5VRDev5VDisk
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
	mov	dword ptr [ebp-018h],offset FLAT:@b8__fsm_tab49;	__es
	push	edi
	mov	[ebp-014h],eax;	__es
	push	esi
	mov	dword ptr [ebp-010h],0h;	__es

; 279 };
	pop	esi

; 80 };
	mov	dword ptr [eax],offset FLAT:@de__vft5VRDev5VDisk

; 279 };
	pop	edi

; 143 };
	mov	dword ptr [ebp-020h],01h;	__es

; 279 };
	pop	ebx

; 143 };
	mov	dword ptr [eax],offset FLAT:@b6__vft7VRDrive5VDisk

; 279 };
	add	esp,018h
	pop	fs:[0h]
	leave	
	ret	
__ct__7VRDriveFv	endp
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

; 70     Boolean	isWritable() { return True; }
EH_CODE	ends
CODE32	segment

isWritable__8VMissingFv	proc
	mov	eax,01h
	ret	
isWritable__8VMissingFv	endp

; 69     void	forceUnwritable() { }

forceUnwritable__8VMissingFv	proc
	ret	
forceUnwritable__8VMissingFv	endp

; 67     void	addChild(VRDev * newchild,Boolean cfgv,Boolean datav=True) { }

addChild__8VMissingFP5VRDev7BooleanT2	proc
	ret	
addChild__8VMissingFP5VRDev7BooleanT2	endp

; 66     void	setHostdrive(ULONG os2idx = -1UL) { }

setHostdrive__8VMissingFUl	proc
	ret	
setHostdrive__8VMissingFUl	endp

; 64     VRDev *	getChild(int which) { return NULL; }

getChild__8VMissingFi	proc
	xor	eax,eax
	ret	
getChild__8VMissingFi	endp

; 63     int		queryChildren() { return 0; }

queryChildren__8VMissingFv	proc
	xor	eax,eax
	ret	
queryChildren__8VMissingFv	endp

; 60     Boolean	isHostdrive() { return False; }

isHostdrive__8VMissingFv	proc
	xor	eax,eax
	ret	
isHostdrive__8VMissingFv	endp

; 59     UCHAR	isArray() { return 0xFF; }

isArray__8VMissingFv	proc
	mov	eax,0ffh
	ret	
isArray__8VMissingFv	endp

; 54     int		ioDeleteYourself() { return 0; }

ioDeleteYourself__8VMissingFv	proc
	xor	eax,eax
	ret	
ioDeleteYourself__8VMissingFv	endp

; 52     int		ioRemoveChild(VRDev * child) { return 0; }

ioRemoveChild__8VMissingFP5VRDev	proc
	xor	eax,eax
	ret	
ioRemoveChild__8VMissingFP5VRDev	endp

; 51     void	setBuild(ULONG nextsec) { }

setBuild__8VMissingFUl	proc
	ret	
setBuild__8VMissingFUl	endp

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
CODE32	ends
end
