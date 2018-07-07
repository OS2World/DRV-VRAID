	title	VOS2Drive.cpp
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
	public	__vft9VOS2Drive5VDisk
	extrn	memset:proc
	extrn	WinLoadPointer:proc
	extrn	_Exception_CPP:proc
	extrn	__dt__8VOS2DiskFv:proc
	extrn	WinDestroyPointer:proc
	extrn	__dl__FPv:proc
	extrn	__vn__FUi:proc
	extrn	_sprintfieee:proc
	extrn	queryIndex__8VOS2DiskFv:proc
	extrn	__PureVirtualCalled:proc
	extrn	_fltused:dword
CONST32_RO	segment
@CBE1	db "OS/2 Device %lu",0ah,"%lu MByt"
db "e",0h
	align 04h
@1vcid	db "$Id: vos2drive.cpp,v 1.5"
db " 2000/04/10 00:57:54 vit"
db "us Exp $",0h
CONST32_RO	ends
EH_DATA	segment
@1a__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__5VRDevFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@32__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__5VRDevFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
EH_DATA	ends
_VFT	segment
__vft9VOS2Drive5VDisk	dd 0
	db 0h,0h,0h,0h
	dd offset FLAT:isWritable__9VOS2DriveFv
	db 0h,0h,0h,0h
	dd offset FLAT:querySize__9VOS2DriveFv
	db 0h,0h,0h,0h
	dd offset FLAT:read__9VOS2DriveFUlT1Pv
	db 0h,0h,0h,0h
	dd offset FLAT:write__9VOS2DriveFUlT1Pv
	db 0h,0h,0h,0h
	dd offset FLAT:__dt__9VOS2DriveFv
	db 0h,0h,0h,0h
	dd offset FLAT:forceUnwritable__9VOS2DriveFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioDeleteYourself__9VOS2DriveFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioRemoveParent__9VOS2DriveFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioChangeID__9VOS2DriveFPUc
	db 0h,0h,0h,0h
	dd offset FLAT:ioSync__9VOS2DriveFv
	db 0h,0h,0h,0h
	dd offset FLAT:queryWhere__9VOS2DriveFv
	db 0h,0h,0h,0h
	dd offset FLAT:isArray__9VOS2DriveFv
	db 0h,0h,0h,0h
	dd offset FLAT:isHostdrive__9VOS2DriveFv
	db 0h,0h,0h,0h
	dd offset FLAT:allocateDescription__9VOS2DriveFv
	db 0h,0h,0h,0h
	dd offset FLAT:queryIcon__9VOS2DriveFv
	db 0h,0h,0h,0h
@6e__vft5VRDev5VDisk	dd 0
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
@7d__vft5VDisk	dd 0
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

; 177 VOS2Drive::ioSync()

	public ioSync__9VOS2DriveFv
ioSync__9VOS2DriveFv	proc

; 180     return 1;
	mov	eax,01h
	ret	
ioSync__9VOS2DriveFv	endp

; 167 VOS2Drive::ioChangeID(DEVID)

	public ioChangeID__9VOS2DriveFPUc
ioChangeID__9VOS2DriveFPUc	proc

; 170     return 1;					/* "1" error :-/ */
	mov	eax,01h
	ret	
ioChangeID__9VOS2DriveFPUc	endp

; 157 VOS2Drive::ioRemoveParent()

	public ioRemoveParent__9VOS2DriveFv
ioRemoveParent__9VOS2DriveFv	proc

; 160     return 1;					/* "1" error :-/ */
	mov	eax,01h
	ret	
ioRemoveParent__9VOS2DriveFv	endp

; 147 VOS2Drive::ioDeleteYourself()

	public ioDeleteYourself__9VOS2DriveFv
ioDeleteYourself__9VOS2DriveFv	proc

; 149     delete_disk = False;
	mov	byte ptr [eax+015h],0h

; 150     return 0;
	xor	eax,eax
	ret	
ioDeleteYourself__9VOS2DriveFv	endp

; 119 VOS2Drive::queryIcon()

	public queryIcon__9VOS2DriveFv
queryIcon__9VOS2DriveFv	proc

; 121     if( writable == True )
	cmp	byte ptr [eax+014h],01h
	jne	@BLBL14

; 122 	return rw_icon;
	mov	eax,[eax+018h]
	ret	
@BLBL14:

; 124 	return ro_icon;
	mov	eax,[eax+01ch]
	ret	
queryIcon__9VOS2DriveFv	endp

; 205 VOS2Drive::write(ULONG block,ULONG count,PVOID buffer)

	public write__9VOS2DriveFUlT1Pv
write__9VOS2DriveFUlT1Pv	proc
	push	ebx
	push	edi

; 207     return theDisk->write(block,count,buffer);
	push	dword ptr [esp+018h];	buffer
	mov	eax,[eax+010h]
	sub	esp,0ch
	mov	ebx,[eax]
	mov	edi,[ebx+024h]
	add	eax,edi
	call	dword ptr [ebx+020h]
	add	esp,010h
	pop	edi
	pop	ebx
	ret	
write__9VOS2DriveFUlT1Pv	endp

; 196 VOS2Drive::read(ULONG block,ULONG count,PVOID buffer)

	public read__9VOS2DriveFUlT1Pv
read__9VOS2DriveFUlT1Pv	proc
	push	ebx
	push	edi

; 198     return theDisk->read(block,count,buffer);
	push	dword ptr [esp+018h];	buffer
	mov	eax,[eax+010h]
	sub	esp,0ch
	mov	ebx,[eax]
	mov	edi,[ebx+01ch]
	add	eax,edi
	call	dword ptr [ebx+018h]
	add	esp,010h
	pop	edi
	pop	ebx
	ret	
read__9VOS2DriveFUlT1Pv	endp

; 187 VOS2Drive::querySize()

	public querySize__9VOS2DriveFv
querySize__9VOS2DriveFv	proc

; 189     return theDisk->querySize();
	mov	eax,[eax+010h]

; 187 VOS2Drive::querySize()
	sub	esp,04h

; 189     return theDisk->querySize();
	mov	ecx,[eax]
	mov	edx,[ecx+014h]
	add	eax,edx
	call	dword ptr [ecx+010h]
	add	esp,04h
	ret	
querySize__9VOS2DriveFv	endp

; 106 char *

	public allocateDescription__9VOS2DriveFv
allocateDescription__9VOS2DriveFv	proc
	push	ebx
	mov	ebx,eax
	push	edi

; 109     char * descr = new char[80];
	mov	eax,050h

; 106 char *
	sub	esp,04h

; 109     char * descr = new char[80];
	call	__vn__FUi

; 110     sprintf(descr, "OS/2 Device %lu\n%lu MByte",
	mov	ecx,[ebx]

; 109     char * descr = new char[80];
	mov	edi,eax

; 110     sprintf(descr, "OS/2 Device %lu\n%lu MByte",
	mov	eax,[ecx+014h]
	add	eax,ebx
	call	dword ptr [ecx+010h]
	xchg	ebx,eax
	shr	ebx,01h
	mov	eax,[eax+010h]
	call	queryIndex__8VOS2DiskFv
	shr	ebx,0ah
	push	ebx
	mov	edx,offset FLAT:@CBE1
	push	eax
	mov	eax,edi
	sub	esp,08h
	call	_sprintfieee

; 112     return descr;
	add	esp,014h

; 110     sprintf(descr, "OS/2 Device %lu\n%lu MByte",
	mov	eax,edi

; 112     return descr;
	pop	edi
	pop	ebx
	ret	
allocateDescription__9VOS2DriveFv	endp

; 137     virtual VOS2Disk *	queryWhere() { return NULL; }

queryWhere__5VRDevFv	proc
	xor	eax,eax
	ret	
queryWhere__5VRDevFv	endp

; 255     Boolean	isWritable() { return writable; }

isWritable__9VOS2DriveFv	proc
	movzx	eax,byte ptr [eax+014h]
	ret	
isWritable__9VOS2DriveFv	endp

; 254     void	forceUnwritable() { writable = False; }

forceUnwritable__9VOS2DriveFv	proc
	mov	byte ptr [eax+014h],0h
	ret	
forceUnwritable__9VOS2DriveFv	endp

; 250     Boolean	isHostdrive() { return True; }

isHostdrive__9VOS2DriveFv	proc
	mov	eax,01h
	ret	
isHostdrive__9VOS2DriveFv	endp

; 249     UCHAR	isArray() { return 0; }

isArray__9VOS2DriveFv	proc
	xor	eax,eax
	ret	
isArray__9VOS2DriveFv	endp

; 243     VOS2Disk *  queryWhere() { return theDisk; }

queryWhere__9VOS2DriveFv	proc
	mov	eax,[eax+010h]
	ret	
queryWhere__9VOS2DriveFv	endp

; 123     virtual	~VRDev() {}			// see Stroustroup pg216!!!

__dt__5VRDevFv	proc
	push	ebx
	mov	dword ptr [eax],offset FLAT:@6e__vft5VRDev5VDisk
	sub	esp,04h
	test	dl,01h
	je	@BLBL33
	mov	ebx,eax
	call	__dl__FPv
	mov	eax,ebx
@BLBL33:
	add	esp,04h
	pop	ebx
	ret	
__dt__5VRDevFv	endp

; 94 VOS2Drive::~VOS2Drive()

	public __dt__9VOS2DriveFv
__dt__9VOS2DriveFv	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffe0h
	mov	[ebp+0ch],edx;	__dtorFlags
	sub	esp,014h

; 95 {
	mov	dword ptr [ebp-020h],0h;	__es

; 94 VOS2Drive::~VOS2Drive()
	push	ebx
	mov	[ebp+08h],eax;	this
	push	edi

; 95 {
	mov	dword ptr [ebp-018h],offset FLAT:@32__fsm_tab;	__es
	mov	[ebp-014h],eax;	__es

; 94 VOS2Drive::~VOS2Drive()
	push	esi

; 95 {
	mov	dword ptr [ebp-010h],0h;	__es

; 94 VOS2Drive::~VOS2Drive()
	mov	dword ptr [ebp-020h],01h;	__es

; 96     if( delete_disk == True )
	cmp	byte ptr [eax+015h],01h

; 94 VOS2Drive::~VOS2Drive()
	mov	dword ptr [eax],offset FLAT:__vft9VOS2Drive5VDisk

; 96     if( delete_disk == True )
	jne	@BLBL5

; 97         delete theDisk;				/* no need for that disk object */
	mov	eax,[eax+010h]
	test	eax,eax
	je	@BLBL5
	sub	esp,08h
	mov	edx,03h
	call	__dt__8VOS2DiskFv
	add	esp,08h
@BLBL5:

; 99     WinDestroyPointer(rw_icon);
	mov	ebx,[ebp+08h];	this
	push	dword ptr [ebx+018h]
	call	WinDestroyPointer
	add	esp,04h

; 100     WinDestroyPointer(ro_icon);
	push	dword ptr [ebx+01ch]
	call	WinDestroyPointer
	mov	eax,ebx

; 101 }
	mov	dword ptr [ebp-020h],0h;	__es

; 100     WinDestroyPointer(ro_icon);
	add	esp,04h

; 123     virtual	~VRDev() {}			// see Stroustroup pg216!!!
	mov	dword ptr [eax],offset FLAT:@6e__vft5VRDev5VDisk
	test	byte ptr [ebp+0ch],01h;	__dtorFlags
	je	@BLBL9
	sub	esp,04h
	call	__dl__FPv
	add	esp,04h
@BLBL9:
	pop	esi
	mov	eax,[ebp+08h];	this
	pop	edi
	pop	ebx
	add	esp,018h
	pop	fs:[0h]
	leave	
	ret	
__dt__9VOS2DriveFv	endp
CODE32	ends
EH_CODE	segment

__dftbdt__5VRDevFv	proc
	mov	dword ptr [eax],offset FLAT:@6e__vft5VRDev5VDisk
	ret	
__dftbdt__5VRDevFv	endp

; 75 VOS2Drive::VOS2Drive(VOS2Disk * disk)
EH_CODE	ends
CODE32	segment

	public __ct__9VOS2DriveFP8VOS2Disk
__ct__9VOS2DriveFP8VOS2Disk	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffe0h
	sub	esp,014h

; 76 {
	mov	dword ptr [ebp-020h],0h;	__es

; 75 VOS2Drive::VOS2Drive(VOS2Disk * disk)
	push	ebx
	mov	ebx,eax
	push	edi

; 76 {
	mov	dword ptr [ebp-018h],offset FLAT:@1a__fsm_tab;	__es

; 75 VOS2Drive::VOS2Drive(VOS2Disk * disk)
	push	esi

; 83     writable = theDisk->isWritable();
	sub	esp,04h

; 76 {
	mov	[ebp-014h],ebx;	__es
	mov	dword ptr [ebp-010h],0h;	__es

; 80 };
	mov	dword ptr [ebx],offset FLAT:@6e__vft5VRDev5VDisk

; 143 };
	mov	dword ptr [ebp-020h],01h;	__es

; 81 
	mov	[ebx+010h],edx

; 83 
	mov	eax,[ebx+010h]

; 143 };
	mov	dword ptr [ebx],offset FLAT:__vft9VOS2Drive5VDisk

; 80     delete_disk = True;
	mov	byte ptr [ebx+015h],01h

; 82     parent = NULL;
	mov	dword ptr [ebx+04h],0h

; 83     writable = theDisk->isWritable();
	mov	ecx,[eax]
	mov	edx,[ecx+0ch]
	add	eax,edx
	call	dword ptr [ecx+08h]
	sub	esp,08h
	mov	[ebx+014h],al

; 84     memset(&id, 0, sizeof(DEVID));
	mov	ecx,06h
	xor	edx,edx
	lea	eax,[ebx+08h]
	call	memset
	add	esp,0ch

; 86     rw_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_FREEDRIVE);
	push	0192h
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	[ebx+018h],eax

; 87     ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_LOCKEDDRIVE);
	push	0191h
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	ecx,eax

; 89 }
	pop	esi

; 87     ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_LOCKEDDRIVE);
	mov	eax,ebx

; 89 }
	pop	edi

; 87     ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_LOCKEDDRIVE);
	mov	[eax+01ch],ecx

; 89 }
	pop	ebx
	add	esp,018h
	pop	fs:[0h]
	leave	
	ret	
__ct__9VOS2DriveFP8VOS2Disk	endp
CODE32	ends
end
