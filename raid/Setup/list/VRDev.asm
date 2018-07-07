	title	VRDev.cpp
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
	public	__vft8VOS2Disk5VDisk
	public	__vft6VFRDev5VDisk
	public	__vft6VORDev5VDisk
	extrn	__nw__FUi:proc
	extrn	Verbose:proc
	extrn	Crc16__FPvUs:proc
	extrn	memcmp:proc
	extrn	memcpy:proc
	extrn	__dl__FPv:proc
	extrn	time:proc
	extrn	WinLoadPointer:proc
	extrn	_Exception_CPP:proc
	extrn	WinDestroyPointer:proc
	extrn	__vn__FUi:proc
	extrn	_sprintfieee:proc
	extrn	memset:proc
	extrn	DriverPhyssize__FUsPUl:proc
	extrn	__vd__FPv:proc
	extrn	DriverPhysIO__FUsUlT2Pv:proc
	extrn	PDskOpen:proc
	extrn	PDskUnlockIO:proc
	extrn	PDskClose:proc
	extrn	PDskLockIO:proc
	extrn	PDskQueryParam:proc
	extrn	PDskRead:proc
	extrn	PDskWrite:proc
	extrn	__PureVirtualCalled:proc
	extrn	usHostId:word
	extrn	_fltused:dword
CONST32_RO	segment
@CBE1	db "VORDev::VORDev",0h
	align 04h
@CBE2	db "can't read PHYSDEV - rc "
db "%lu",0h
@CBE3	db "PHYSDEVICE      ",0h
	align 04h
@CBE4	db "VORDev",0h
	align 04h
@CBE5	db "missing PHYSDEV mark",0h
	align 04h
@CBE6	db "CRC error in PHYSDEV sec"
db "tor (%#x, %#x)",0h
	align 04h
@CBE7	db "PHYSDEV (OS/2 Device %lu"
db ")",0ah,"ID %02X%02X%02X%02X%02"
db "X%02X    %lu MByte",0h
	align 04h
@CBE8	db "VORDev::ioChangeID",0h
	align 04h
@CBE9	db "can't read own SEC_PHYSD"
db "EV - rc %lu",0h
@CBE10	db "can't write own SEC_PHYS"
db "DEV - rc %lu",0h
	align 04h
@CBE11	db "VORDev::ioRemoveParent",0h
	align 04h
@CBE12	db "can't clear parent SEC_V"
db "RDEV - rc %lu",0h
	align 04h
@CBE13	db "VORDev::ioDeleteYourself"
db 0h
	align 04h
@CBE14	db "can't clear SEC_PHYSDEV "
db "- rc %lu",0h
	align 04h
@CBE15	db "can't read MBR - rc %lu",0h
@CBE16	db "invalid MBR, done",0h
	align 04h
@CBE17	db "no VRAID partition, done"
db 0h
	align 04h
@CBE18	db "can't write MBR - rc %lu"
db 0h
	align 04h
@CBE19	db "VORDev::ioSync",0h
	align 04h
@CBE20	db "new PHYSDEV sector",0h
	align 04h
@CBE21	db "write(0,1,...) - rc %lu,"
db " not updated",0h
	align 04h
@CBE22	db "VFRDev::VFRDev",0h
	align 04h
@CBE23	db "DriverPhyssize - rc %lu",0h
@CBE24	db "read(0,1,...) - rc %lu",0h
	align 04h
@CBE25	db "VFRDev",0h
	align 04h
@CBE26	db "PHYSDEV (filter device h"
db "andle %#x)",0ah,"ID %02X%02X%0"
db "2X%02X%02X%02X    %lu MB"
db "yte partition",0h
	align 04h
@CBE27	db "VFRDev::ioRemoveParent",0h
	align 04h
@CBE28	db "VFRDev::ioChangeID",0h
	align 04h
@CBE29	db "VFRDev::ioDeleteYourself"
db 0h
	align 04h
@CBE30	db "can't clear own SEC_PHYS"
db "DEV - rc %lu",0h
	align 04h
@CBE31	db "VFRDev::ioSync",0h
	align 04h
@CBE32	db "can't read block %lu - r"
db "c %lu",0h
	align 04h
@CBE33	db "can't write block %lu - "
db "rc %lu",0h
	align 04h
@CBE34	db "VOS2Disk",0h
	align 04h
@CBE35	db "PDskOpen(%lu) - rc %lu",0h
	align 04h
@CBE36	db "PDskUnlockIO - rc %lu",0h
	align 04h
@CBE37	db "PDskLock(%lu) - rc %lu, "
db "ignored",0h
@CBE38	db "PDskQueryParam(%lu) - rc"
db " %lu",0h
	align 04h
@CBE39	db "physical drive parameter"
db "s:",0h
	align 04h
@CBE40	db "Cylinders: %d, Heads: %d"
db ", Sectors/Track: %d",0h
@CBE41	db "drive capacity: %lu MByt"
db "es",0h
	align 04h
@CBE42	db "can't read sector %lu - "
db "rc %lu",0h
	align 04h
@CBE43	db "can't write sector %lu -"
db " rc %lu",0h
@1vcid	db "$Id: vrdev.cpp,v 1.14 20"
db "00/04/10 01:10:30 vitus "
db "Exp $",0h
CONST32_RO	ends
EH_DATA	segment
@34__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__5VRDevFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@4b__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__5VRDevFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@a9__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__5VRDevFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
@bb__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	db 0h,0h,0h,0h
	dd offset FLAT:__dftbdt__5VRDevFv
	db 01h,0h,0h,0h
	db 010h,0h,0h,0h
	db 01h,0h
	db 0h,0h
EH_DATA	ends
_VFT	segment
__vft8VOS2Disk5VDisk	dd 0
	db 0h,0h,0h,0h
	dd offset FLAT:isWritable__8VOS2DiskFv
	db 0h,0h,0h,0h
	dd offset FLAT:querySize__8VOS2DiskFv
	db 0h,0h,0h,0h
	dd offset FLAT:read__8VOS2DiskFUlT1Pv
	db 0h,0h,0h,0h
	dd offset FLAT:write__8VOS2DiskFUlT1Pv
	db 0h,0h,0h,0h
__vft6VFRDev5VDisk	dd 0
	db 0h,0h,0h,0h
	dd offset FLAT:isWritable__6VFRDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:querySize__6VFRDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:read__6VFRDevFUlT1Pv
	db 0h,0h,0h,0h
	dd offset FLAT:write__6VFRDevFUlT1Pv
	db 0h,0h,0h,0h
	dd offset FLAT:__dt__6VFRDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:forceUnwritable__6VFRDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioDeleteYourself__6VFRDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioRemoveParent__6VFRDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioChangeID__6VFRDevFPUc
	db 0h,0h,0h,0h
	dd offset FLAT:ioSync__6VFRDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:queryWhere__5VRDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:isArray__6VFRDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:isHostdrive__6VFRDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:allocateDescription__6VFRDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:queryIcon__6VFRDevFv
	db 0h,0h,0h,0h
__vft6VORDev5VDisk	dd 0
	db 0h,0h,0h,0h
	dd offset FLAT:isWritable__6VORDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:querySize__6VORDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:read__6VORDevFUlT1Pv
	db 0h,0h,0h,0h
	dd offset FLAT:write__6VORDevFUlT1Pv
	db 0h,0h,0h,0h
	dd offset FLAT:__dt__6VORDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:forceUnwritable__6VORDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioDeleteYourself__6VORDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioRemoveParent__6VORDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:ioChangeID__6VORDevFPUc
	db 0h,0h,0h,0h
	dd offset FLAT:ioSync__6VORDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:queryWhere__6VORDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:isArray__6VORDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:isHostdrive__6VORDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:allocateDescription__6VORDevFv
	db 0h,0h,0h,0h
	dd offset FLAT:queryIcon__6VORDevFv
	db 0h,0h,0h,0h
@146__vft5VRDev5VDisk	dd 0
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
@13f__vft5VDisk	dd 0
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

; 1251 DEVICEPARAMETERBLOCK &

	public queryDPB__8VOS2DiskFv
queryDPB__8VOS2DiskFv	proc

; 1254     return devparam;
	add	eax,0ch
	ret	
queryDPB__8VOS2DiskFv	endp

; 1005 VFRDev::querySize()

	public querySize__6VFRDevFv
querySize__6VFRDevFv	proc

; 1007     return (size == 0 ? partsize : size);
	cmp	dword ptr [eax+018h],0h
	jne	@BLBL136
	mov	ecx,[eax+014h]
	jmp	@BLBL137
@BLBL136:
	mov	ecx,[eax+018h]
@BLBL137:
	mov	eax,ecx
	ret	
querySize__6VFRDevFv	endp

; 544 VORDev::querySize()

	public querySize__6VORDevFv
querySize__6VORDevFv	proc

; 546     return (size == 0 ? partsize : size);
	cmp	dword ptr [eax+01ch],0h
	jne	@BLBL74
	mov	ecx,[eax+018h]
	jmp	@BLBL75
@BLBL74:
	mov	ecx,[eax+01ch]
@BLBL75:
	mov	eax,ecx
	ret	
querySize__6VORDevFv	endp

; 1274 VOS2Disk::queryIndex()

	public queryIndex__8VOS2DiskFv
queryIndex__8VOS2DiskFv	proc

; 1276     return pdskidx;
	mov	eax,[eax+04h]
	ret	
queryIndex__8VOS2DiskFv	endp

; 1356 VOS2Disk::write(ULONG block,ULONG count,PVOID buffer)

	public write__8VOS2DiskFUlT1Pv
write__8VOS2DiskFUlT1Pv	proc
	sub	esp,04h
	push	ebx
	mov	ebx,edx
	push	edi

; 1360     APIRET const rc = PDskWrite(pdskhd, block, (USHORT)count, buffer);
	push	dword ptr [esp+01ch];	buffer
	mov	eax,[eax+08h]
	sub	esp,0ch
	call	PDskWrite
	add	esp,010h
	mov	edx,ebx
	mov	edi,eax

; 1361     if( rc != 0 )
	test	eax,eax
	je	@BLBL180

; 1362 	Verbose(1, "VOS2Disk", "can't write sector %lu - rc %lu", block, rc);
	push	eax
	mov	ecx,offset FLAT:@CBE43
	push	edx
	mov	edx,offset FLAT:@CBE34
	sub	esp,0ch
	mov	eax,01h
	call	Verbose
	add	esp,014h
@BLBL180:

; 1363     return rc;
	mov	eax,edi
	pop	edi
	pop	ebx
	add	esp,04h
	ret	
write__8VOS2DiskFUlT1Pv	endp

; 1325 VOS2Disk::read(ULONG block,ULONG count,PVOID buffer)

	public read__8VOS2DiskFUlT1Pv
read__8VOS2DiskFUlT1Pv	proc
	sub	esp,04h
	push	ebx
	mov	ebx,edx
	push	edi

; 1329     APIRET const rc = PDskRead(pdskhd, block, (USHORT)count, buffer);
	push	dword ptr [esp+01ch];	buffer
	mov	eax,[eax+08h]
	sub	esp,0ch
	call	PDskRead
	add	esp,010h
	mov	edx,ebx
	mov	edi,eax

; 1330     if( rc != 0 )
	test	eax,eax
	je	@BLBL177

; 1331 	Verbose(1, "VOS2Disk", "can't read sector %lu - rc %lu", block, rc);
	push	eax
	mov	ecx,offset FLAT:@CBE42
	push	edx
	mov	edx,offset FLAT:@CBE34
	sub	esp,0ch
	mov	eax,01h
	call	Verbose
	add	esp,014h
@BLBL177:

; 1332     return rc;
	mov	eax,edi
	pop	edi
	pop	ebx
	add	esp,04h
	ret	
read__8VOS2DiskFUlT1Pv	endp

; 1295 VOS2Disk::querySize()

	public querySize__8VOS2DiskFv
querySize__8VOS2DiskFv	proc
	sub	esp,04h

; 1297     ULONG const	size = (ULONG)devparam.cSectorsPerTrack
	movzx	ecx,word ptr [eax+010h]

; 1295 VOS2Disk::querySize()
	push	edi

; 1297     ULONG const	size = (ULONG)devparam.cSectorsPerTrack
	movzx	edx,word ptr [eax+012h]
	imul	edx,ecx
	movzx	ecx,word ptr [eax+0eh]
	imul	ecx,edx
	mov	edi,ecx

; 1300     Verbose(3, "VOS2Disk", "drive capacity: %lu MBytes", size / 2 / 1024ul);
	shr	ecx,01h
	shr	ecx,0ah
	push	ecx
	mov	ecx,offset FLAT:@CBE41
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE34
	mov	eax,03h
	call	Verbose
	add	esp,010h

; 1301     return size;
	mov	eax,edi
	pop	edi
	add	esp,04h
	ret	
querySize__8VOS2DiskFv	endp

; 1191 VOS2Disk::create(ULONG idx,ULONG handle)

	public create__8VOS2DiskFUlT1
create__8VOS2DiskFUlT1	proc
	push	ebx
	mov	ebx,ecx
	push	edi
	xchg	ebx,eax
	sub	esp,0ch

; 1200     memset(&devparam, 0, sizeof(devparam));
	mov	ecx,010h

; 1191 VOS2Disk::create(ULONG idx,ULONG handle)
	mov	[esp+018h],ebx;	this

; 1197     pdskidx = idx;
	mov	[ebx+04h],edx

; 1200     memset(&devparam, 0, sizeof(devparam));
	xor	edx,edx

; 1198     pdskhd = handle;
	mov	[ebx+08h],eax

; 1200     memset(&devparam, 0, sizeof(devparam));
	lea	eax,[ebx+0ch]
	call	memset

; 1208     rc = PDskLockIO(pdskhd);
	mov	eax,[ebx+08h]

; 1201     writ
; 1201 able = False;
	mov	byte ptr [ebx+01ch],0h

; 1208     rc = PDskLockIO(pdskhd);
	call	PDskLockIO

; 1209     if( rc != 0 )
	test	eax,eax
	je	@BLBL167

; 1210 	Verbose(2, "VOS2Disk", "PDskLock(%lu) - rc %lu, ignored", pdskidx, rc);
	push	eax
	mov	eax,ebx
	mov	ecx,offset FLAT:@CBE37
	push	dword ptr [eax+04h]
	mov	edx,offset FLAT:@CBE34
	sub	esp,0ch
	mov	eax,02h
	call	Verbose
	add	esp,014h
@BLBL167:

; 1211     writable = True;				/* device is changable! */
	mov	edi,[esp+018h];	this

; 1214     rc = PDskQueryParam(pdskhd,&devparam);
	mov	eax,[edi+08h]

; 1211     writable = True;				/* device is changable! */
	mov	byte ptr [edi+01ch],01h

; 1214     rc = PDskQueryParam(pdskhd,&devparam);
	lea	edx,[edi+0ch]
	call	PDskQueryParam
	mov	ebx,eax

; 1215     if( rc != 0 )
	test	ebx,ebx
	je	@BLBL168

; 1217 	Verbose(1, "VOS2Disk", "PDskQueryParam(%lu) - rc %lu", pdskidx, rc);
	push	ebx
	mov	ecx,offset FLAT:@CBE38
	push	dword ptr [edi+04h]
	mov	edx,offset FLAT:@CBE34
	sub	esp,0ch
	mov	eax,01h
	call	Verbose

; 1218 	PDskUnlockIO(pdskhd);
	mov	eax,[edi+08h]
	call	PDskUnlockIO
	mov	ecx,edi

; 1219 	PDskClose(pdskhd);
	mov	eax,[ecx+08h]
	call	PDskClose

; 1220 	return rc;
	add	esp,020h

; 1219 	PDskClose(pdskhd);
	mov	eax,ebx

; 1220 	return rc;
	pop	edi
	pop	ebx
	ret	
@BLBL168:

; 1223     Verbose(3, "VOS2Disk", "physical drive parameters:");
	mov	ebx,offset FLAT:@CBE34
	mov	ecx,offset FLAT:@CBE39
	mov	edx,ebx
	mov	eax,03h
	call	Verbose

; 1224     Verbose(3, "VOS2Disk", "Cylinders: %d, Heads: %d, Sectors/Track: %d",
	mov	eax,[esp+018h];	this

; 1223     Verbose(3, "VOS2Disk", "physical drive parameters:");
	mov	edx,ebx

; 1224     Verbose(3, "VOS2Disk", "Cylinders: %d, Heads: %d, Sectors/Track: %d",
	movzx	ecx,word ptr [eax+012h]
	push	ecx
	movzx	ecx,word ptr [eax+010h]
	push	ecx
	movzx	eax,word ptr [eax+0eh]
	push	eax
	mov	ecx,offset FLAT:@CBE40
	sub	esp,0ch
	mov	eax,03h
	call	Verbose

; 1226     return 0;
	add	esp,024h
	xor	eax,eax
	pop	edi
	pop	ebx
	ret	
create__8VOS2DiskFUlT1	endp

; 1068 VFRDev::write(ULONG block,ULONG count,PVOID buffer)

	public write__6VFRDevFUlT1Pv
write__6VFRDevFUlT1Pv	proc
	push	ebp
	mov	ebp,esp
	sub	esp,08h
	mov	[ebp+010h],ecx;	count
	push	ebx
	mov	[ebp+0ch],edx;	block
	push	edi

; 1072     for( ; count != 0; --count, ++block, block += SECTOR_SIZE )
	test	ecx,ecx

; 1068 VFRDev::write(ULONG block,ULONG count,PVOID buffer)
	push	esi
	mov	[ebp+08h],eax;	this

; 1070     APIRET	rc = 0;
	mov	dword ptr [ebp-04h],0h;	rc

; 1072     for( ; count != 0; --count, ++block, block += SECTOR_SIZE )
	je	@BLBL148
	mov	ebx,[ebp+014h];	buffer
	mov	esi,eax
	mov	edi,ecx
@BLBL149:

; 1074 	rc = DriverPhysIO(hd, TRUE, block, buffer);
	push	ebx
	mov	[ebp-08h],edx;	@CBE48
	sub	esp,0ch
	mov	ecx,edx
	mov	ax,[esi+010h]
	mov	edx,01h
	call	DriverPhysIO__FUsUlT2Pv
	mov	edx,[ebp-08h];	@CBE48
	add	esp,010h
	mov	[ebp-04h],eax;	rc

; 1075 	if( rc != 0 )
	test	eax,eax
	je	@BLBL152

; 1077 	    Verbose(1, "VFRDev", "can't write block %lu - rc %lu", block, rc);
	push	eax
	mov	ecx,offset FLAT:@CBE33
	push	edx
	mov	edx,offset FLAT:@CBE25
	sub	esp,0ch
	mov	eax,01h
	call	Verbose
	mov	edx,[ebp-08h];	@CBE48
	add	esp,014h

; 1078 	    break;
	jmp	@BLBL148
@BLBL152:

; 1072     for( ; count != 0; --count, ++block, block += SECTOR_SIZE )
	dec	edi
	add	edx,0201h
	test	edi,edi
	jne	@BLBL149
@BLBL148:

; 1081     return rc;
	pop	esi
	mov	eax,[ebp-04h];	rc
	pop	edi
	pop	ebx
	leave	
	ret	
write__6VFRDevFUlT1Pv	endp

; 1031 VFRDev::read(ULONG block,ULONG count,PVOID buffer)

	public read__6VFRDevFUlT1Pv
read__6VFRDevFUlT1Pv	proc
	push	ebp
	mov	ebp,esp
	sub	esp,08h
	mov	[ebp+010h],ecx;	count
	push	ebx
	mov	[ebp+0ch],edx;	block
	push	edi

; 1035     for( ; count != 0; --count, ++block, block += SECTOR_SIZE )
	test	ecx,ecx

; 1031 VFRDev::read(ULONG block,ULONG count,PVOID buffer)
	push	esi
	mov	[ebp+08h],eax;	this

; 1033     APIRET	rc = 0;
	mov	dword ptr [ebp-04h],0h;	rc

; 1035     for( ; count != 0; --count, ++block, block += SECTOR_SIZE )
	je	@BLBL140
	mov	ebx,[ebp+014h];	buffer
	mov	esi,eax
	mov	edi,ecx
@BLBL141:

; 1037 	rc = DriverPhysIO(hd, FALSE, block, buffer);
	push	ebx
	mov	[ebp-08h],edx;	@CBE49
	sub	esp,0ch
	mov	ecx,edx
	mov	ax,[esi+010h]
	xor	edx,edx
	call	DriverPhysIO__FUsUlT2Pv
	mov	edx,[ebp-08h];	@CBE49
	add	esp,010h
	mov	[ebp-04h],eax;	rc

; 1038 	if( rc != 0 )
	test	eax,eax
	je	@BLBL144

; 1040 	    Verbose(1, "VFRDev", "can't read block %lu - rc %lu", block, rc);
	push	eax
	mov	ecx,offset FLAT:@CBE32
	push	edx
	mov	edx,offset FLAT:@CBE25
	sub	esp,0ch
	mov	eax,01h
	call	Verbose
	mov	edx,[ebp-08h];	@CBE49
	add	esp,014h

; 1041 	    break;
	jmp	@BLBL140
@BLBL144:

; 1035     for( ; count != 0; --count, ++block, block += SECTOR_SIZE )
	dec	edi
	add	edx,0201h
	test	edi,edi
	jne	@BLBL141
@BLBL140:

; 1044     return rc;
	pop	esi
	mov	eax,[ebp-04h];	rc
	pop	edi
	pop	ebx
	leave	
	ret	
read__6VFRDevFUlT1Pv	endp

; 931 VFRDev::ioSync()

	public ioSync__6VFRDevFv
ioSync__6VFRDevFv	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi

; 933     PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
	mov	eax,0200h

; 931 VFRDev::ioSync()
	push	esi
	mov	[ebp+08h],ebx;	this
	sub	esp,018h

; 936     int			errors = 0;
	mov	dword ptr [ebp-018h],0h;	errors

; 933     PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
	call	__nw__FUi

; 941 	rc = read(0, 1, sec);
	push	eax

; 933     PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
	mov	[ebp-014h],eax;	sec
	mov	eax,ebx

; 941 	rc = read(0, 1, sec);
	sub	esp,0ch
	mov	ebx,[eax]
	mov	ecx,01h
	mov	edi,[ebx+01ch]
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+018h]

; 934     int			modified = 0;
	xor	esi,esi

; 941 	rc = read(0, 1, sec);
	add	esp,010h

; 942 	if( rc )
	test	eax,eax
	je	@BLBL125

; 944 	    Verbose(1, "VFRDev::ioSync", "read(0,1,...) - rc %lu", rc);
	push	eax

; 945 	    ++errors;
	mov	dword ptr [ebp-018h],01h;	errors

; 944 	    Verbose(1, "VFRDev::ioSync", "read(0,1,...) - rc %lu", rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE24
	mov	edx,offset FLAT:@CBE31
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 946 	    break;
	jmp	@BLBL126
@BLBL125:

; 949 	if( memcmp(sec->sectype, "PHYSDEVICE      ", 16) != 0
	mov	ebx,[ebp-014h];	sec
	mov	ecx,010h
	mov	edx,offset FLAT:@CBE3
	mov	eax,ebx
	call	memcmp
	test	eax,eax
	jne	@BLBL127
	mov	eax,ebx
	mov	dx,01feh
	call	Crc16__FPvUs
	cmp	[ebx+01feh],ax
	je	@BLBL128
@BLBL127:

; 954 	    memset(sec, 0, sizeof(*sec));
	mov	edi,[ebp-014h];	sec

; 952 	    Verbose(1, "VFRDev", "new PHYSDEV sector");
	mov	ecx,offset FLAT:@CBE20
	mov	edx,offset FLAT:@CBE25
	mov	eax,01h
	call	Verbose

; 954 	    memset(sec, 0, sizeof(*sec));
	mov	ecx,0200h
	xor	edx,edx
	mov	eax,edi
	call	memset

; 956 	    memcpy(sec->sectype, "PHYSDEVICE      ", 16);
	mov	ecx,010h
	mov	edx,offset FLAT:@CBE3
	mov	eax,edi
	call	memcpy
	mov	eax,edi

; 953 	    ++modified;
	inc	esi

; 957 	    sec->u.s.adminspace = VRAID_ADMINSECTORS;
	mov	word ptr [eax+01ah],020h

; 958 	    size = sec->u.s.size = (partsize - sec->u.s.adminspace);
	mov	edx,[ebp+08h];	this
	mov	ecx,[edx+014h]
	sub	ecx,020h
	mov	[eax+01ch],ecx
	mov	[edx+018h],ecx

; 960 	    memcpy(sec->u.s.id, id, sizeof(DEVID));
	add	edx,08h
	add	eax,014h
	mov	ecx,06h
	call	memcpy

; 961 	}
@BLBL128:

; 963 	if( size != sec->u.s.size )
	mov	ebx,[ebp+08h];	this
	mov	eax,[ebp-014h];	sec
	mov	ebx,[ebx+018h]
	cmp	[eax+01ch],ebx
	je	@BLBL129

; 965 	    ++modified;
	inc	esi

; 966 	    sec->u.s.size = size;
	mov	[eax+01ch],ebx

; 967 	}
@BLBL129:

; 969 	if( modified )
	cmp	esi,0h
	je	@BLBL126

; 971 	    sec->timestamp = time(NULL);
	mov	ebx,[ebp-014h];	sec
	xor	eax,eax
	call	time
	mov	[ebx+010h],eax

; 972 	    sec->crc = Crc16(sec, SECTOR_SIZE-2);
	mov	dx,01feh
	mov	eax,ebx
	call	Crc16__FPvUs
	mov	[ebx+01feh],ax

; 974 	    rc = write(0, 1, sec);
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

; 975 	    if( rc != 0 )
	test	eax,eax
	je	@BLBL126

; 977 		Verbose(1, "VFRDev::ioSync", "write(0,1,...) - rc %lu, not updated", rc);
	push	eax

; 978 		++errors;
	inc	dword ptr [ebp-018h];	errors

; 977 		Verbose(1, "VFRDev::ioSync", "write(0,1,...) - rc %lu, not updated", rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE21
	mov	edx,offset FLAT:@CBE31
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 979 		break;
@BLBL126:

; 985     return errors;
	mov	eax,[ebp-018h];	errors
	add	esp,018h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ioSync__6VFRDevFv	endp

; 894 VFRDev::ioDeleteYourself()

	public ioDeleteYourself__6VFRDevFv
ioDeleteYourself__6VFRDevFv	proc
	push	ebx
	mov	ebx,eax
	push	edi

; 896     PUCHAR	buffer = new UCHAR[SECTOR_SIZE];
	mov	eax,0200h

; 894 VFRDev::ioDeleteYourself()
	push	esi

; 898     int		errors = 0;
	xor	esi,esi

; 894 VFRDev::ioDeleteYourself()
	sub	esp,014h

; 896     PUCHAR	buffer = new UCHAR[SECTOR_SIZE];
	call	__vn__FUi
	mov	edi,eax

; 900     memset(buffer, 0, SECTOR_SIZE);
	mov	ecx,0200h
	xor	edx,edx
	call	memset
	mov	eax,ebx

; 896     PUCHAR	buffer = new UCHAR[SECTOR_SIZE];
	mov	[esp+0ch],edi;	buffer

; 901     rc = write(0, 1, buffer);			/* clear PHYSDEV sector */
	push	edi
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+024h]
	mov	ecx,01h
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+020h]
	add	esp,010h

; 902     if( rc != 0 )
	test	eax,eax
	je	@BLBL121

; 904 	Verbose(1, "VFRDev::ioDeleteYourself", "can't clear own SEC_PHYSDEV - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE30
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE29
	mov	eax,01h
	call	Verbose

; 905 	++errors;				/* ignore errors */
	mov	esi,01h

; 904 	Verbose(1, "VFRDev::ioDeleteYourself", "can't clear own SEC_PHYSDEV - rc %lu", rc);
	add	esp,010h

; 906     }
@BLBL121:

; 908     delete[] buffer;
	mov	eax,[esp+0ch];	buffer
	call	__vd__FPv

; 909     return errors;
	add	esp,014h
	mov	eax,esi
	pop	esi
	pop	edi
	pop	ebx
	ret	
ioDeleteYourself__6VFRDevFv	endp

; 840 VFRDev::ioChangeID(DEVID newid)

	public ioChangeID__6VFRDevFPUc
ioChangeID__6VFRDevFPUc	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,edx
	push	edi
	mov	edi,eax
	push	esi

; 842     PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
	mov	eax,0200h

; 840 VFRDev::ioChangeID(DEVID newid)
	sub	esp,014h
	mov	[ebp+0ch],ebx;	newid

; 842     PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
	call	__nw__FUi
	mov	esi,eax
	mov	edx,ebx

; 845     memcpy(id, newid, sizeof(DEVID));		/* update object */
	mov	ecx,06h
	lea	eax,[edi+08h]
	call	memcpy
	mov	eax,edi

; 851 	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
	push	esi

; 840 VFRDev::ioChangeID(DEVID newid)
	mov	[ebp+08h],edi;	this

; 842     PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
	mov	[ebp-014h],esi;	sec

; 851 	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
	mov	ecx,01h

; 843     int		errors = 0;
	mov	dword ptr [ebp-010h],0h;	errors

; 851 	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+01ch]
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+018h]
	add	esp,010h

; 852 	if( rc != 0 )
	test	eax,eax
	je	@BLBL114

; 854 	    Verbose(1,"VFRDev::ioChangeID","can't read own SEC_PHYSDEV - rc %lu",rc);
	push	eax

; 855 	    ++errors;
	mov	dword ptr [ebp-010h],01h;	errors

; 854 	    Verbose(1,"VFRDev::ioChangeID","can't read own SEC_PHYSDEV - rc %lu",rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE9
	mov	edx,offset FLAT:@CBE28
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 856 	    break;
	jmp	@BLBL115
@BLBL114:

; 859 	memcpy(sec->u.s.id, newid, sizeof(DEVID));
	mov	ebx,[ebp-014h];	sec
	mov	edx,[ebp+0ch];	newid
	mov	ecx,06h
	lea	eax,[ebx+014h]
	call	memcpy

; 860 	sec->timestamp = time(NULL);		/* UTC of change */
	xor	eax,eax
	call	time
	mov	[ebx+010h],eax

; 861 	sec->crc = Crc16(sec, SECTOR_SIZE-2);	/* !!! */
	mov	dx,01feh
	mov	eax,ebx
	call	Crc16__FPvUs
	mov	[ebx+01feh],ax

; 863 	rc = write(0, 1, sec);
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

; 864 	if( rc != 0 )
	test	eax,eax
	je	@BLBL115

; 866 	    Verbose(1, "VFRDev::ioChangeID", "can't write own SEC_PHYSDEV - rc %lu", rc);
	push	eax

; 867 	    ++errors;
	inc	dword ptr [ebp-010h];	errors

; 866 	    Verbose(1, "VFRDev::ioChangeID", "can't write own SEC_PHYSDEV - rc %lu", rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE10
	mov	edx,offset FLAT:@CBE28
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 868 	    break;
@BLBL115:

; 873     delete sec;
	mov	eax,[ebp-014h];	sec
	call	__dl__FPv

; 874     return errors;
	mov	eax,[ebp-010h];	errors
	add	esp,014h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ioChangeID__6VFRDevFPUc	endp

; 773 VFRDev::ioRemoveParent()
; 773 

	public ioRemoveParent__6VFRDevFv
ioRemoveParent__6VFRDevFv	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi

; 775     PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
	mov	eax,0200h

; 773 VFRDev::ioRemoveParent()
; 773 
	push	esi
	mov	[ebp+08h],ebx;	this
	sub	esp,014h

; 776     int		errors = 0;
	xor	esi,esi

; 775     PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
	call	__nw__FUi
	mov	edi,eax

; 782     memset(sec, 0, SECTOR_SIZE);
	mov	ecx,0200h
	xor	edx,edx
	call	memset
	mov	eax,ebx

; 783     rc = write(1, 1, sec);
	push	edi

; 775     PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
	mov	[ebp-014h],edi;	sec

; 783     rc = write(1, 1, sec);
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+024h]
	mov	ecx,01h
	mov	edx,01h
	add	eax,edi
	call	dword ptr [ebx+020h]
	add	esp,010h

; 784     if( rc != 0 )
	test	eax,eax
	je	@BLBL105

; 786 	Verbose(1, "VFRDev::ioRemoveParent", "can't clear parent SEC_VRDEV - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE12
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE27
	mov	eax,01h
	call	Verbose

; 787 	++errors;
	mov	esi,01h

; 786 	Verbose(1, "VFRDev::ioRemoveParent", "can't clear parent SEC_VRDEV - rc %lu", rc);
	add	esp,010h

; 793     do
@BLBL105:

; 795 	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
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

; 796 	if( rc != 0 )
	test	eax,eax
	je	@BLBL106

; 798 	    Verbose(1, "VFRDev::ioRemoveParent", "can't read own SEC_PHYSDEV - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE9
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE27
	mov	eax,01h
	call	Verbose

; 799 	    ++errors;
	inc	esi

; 798 	    Verbose(1, "VFRDev::ioRemoveParent", "can't read own SEC_PHYSDEV - rc %lu", rc);
	add	esp,010h

; 800 	    break;
	jmp	@BLBL107
@BLBL106:

; 803 	sec->u.s.size = size = partsize - sec->u.s.adminspace;
	mov	ebx,[ebp-014h];	sec
	mov	edi,[ebp+08h];	this
	movzx	edx,word ptr [ebx+01ah]
	mov	ecx,[edi+014h]
	sub	ecx,edx
	mov	[edi+018h],ecx
	mov	[ebx+01ch],ecx

; 804 	sec->timestamp = time(NULL);
	xor	eax,eax
	call	time
	mov	[ebx+010h],eax

; 805 	sec->crc = Crc16(sec, SECTOR_SIZE-2);	/* !!! */
	mov	dx,01feh
	mov	eax,ebx
	call	Crc16__FPvUs
	mov	[ebx+01feh],ax
	mov	eax,edi

; 807 	rc = write(0, 1, sec);
	push	ebx
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+024h]
	mov	ecx,01h
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+020h]
	add	esp,010h

; 808 	if( rc != 0 )
	test	eax,eax
	je	@BLBL108

; 810 	    Verbose(1, "VFRDev::ioRemoveParent", "can't write own SEC_PHYSDEV - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE10
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE27
	mov	eax,01h
	call	Verbose

; 811 	    ++errors;
	inc	esi

; 810 	    Verbose(1, "VFRDev::ioRemoveParent", "can't write own SEC_PHYSDEV - rc %lu", rc);
	add	esp,010h

; 812 	    break;
	jmp	@BLBL107
@BLBL108:

; 815 	parent = NULL;				/* none */
	mov	eax,[ebp+08h];	this
	mov	dword ptr [eax+04h],0h

; 817     while(0);
@BLBL107:

; 819     delete sec;
	mov	eax,[ebp-014h];	sec
	call	__dl__FPv

; 820     return errors;
	add	esp,014h
	mov	eax,esi
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ioRemoveParent__6VFRDevFv	endp

; 746 VFRDev::queryIcon()

	public queryIcon__6VFRDevFv
queryIcon__6VFRDevFv	proc
	push	ebx

; 748     if( parent != 0  &&  parent->isValidChild(this) == False )
	mov	ebx,[eax+04h]

; 746 VFRDev::queryIcon()
	push	edi
	sub	esp,08h

; 748     if( parent != 0  &&  parent->isValidChild(this) == False )
	test	ebx,ebx
	je	@BLBL98
	xchg	ebx,eax
	mov	edx,ebx
	mov	ecx,[eax]
	mov	edi,[ecx+0ach]
	add	eax,edi
	call	dword ptr [ecx+0a8h]
	test	al,al
	mov	eax,ebx
	jne	@BLBL98

; 749 	return inv_icon;
	mov	eax,[eax+020h]
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
@BLBL98:

; 750     else if( writable == True )
	cmp	byte ptr [eax+01ch],01h
	jne	@BLBL100

; 751 	return rw_icon;
	mov	eax,[eax+024h]
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
@BLBL100:

; 753 	return ro_icon;
	mov	eax,[eax+028h]
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
queryIcon__6VFRDevFv	endp

; 717 char *

	public allocateDescription__6VFRDevFv
allocateDescription__6VFRDevFv	proc
	push	ebx
	mov	ebx,eax
	sub	esp,04h

; 720     char * descr = new char[80];
	mov	eax,050h
	call	__vn__FUi
	xchg	ebx,eax

; 721     sprintf(descr, "PHYSDEV (filter device handle %#x)\n"
	mov	edx,offset FLAT:@CBE26
	mov	ecx,[eax+014h]
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
	movzx	ecx,word ptr [eax+010h]
	push	ecx
	mov	eax,ebx
	sub	esp,08h
	call	_sprintfieee

; 726     return descr;
	add	esp,02ch

; 721     sprintf(descr, "PHYSDEV (filter device handle %#x)\n"
	mov	eax,ebx

; 726     return descr;
	pop	ebx
	ret	
allocateDescription__6VFRDevFv	endp

; 596 VORDev::write(ULONG block,ULONG count,PVOID buffer)

	public write__6VORDevFUlT1Pv
write__6VORDevFUlT1Pv	proc
	push	ebx
	push	edi

; 598     return theDisk->write(partstart+block, count, buffer);
	push	dword ptr [esp+018h];	buffer
	add	edx,[eax+014h]
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
write__6VORDevFUlT1Pv	endp

; 570 VORDev::read(ULONG block,ULONG count,PVOID buffer)

	public read__6VORDevFUlT1Pv
read__6VORDevFUlT1Pv	proc
	push	ebx
	push	edi

; 572     return theDisk->read(partstart+block, count, buffer);
	push	dword ptr [esp+018h];	buffer
	add	edx,[eax+014h]
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
read__6VORDevFUlT1Pv	endp

; 471 VORDev::ioSync()

	public ioSync__6VORDevFv
ioSync__6VORDevFv	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi

; 473     PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
	mov	eax,0200h

; 471 VORDev::ioSync()
	push	esi
	mov	[ebp+08h],ebx;	this
	sub	esp,018h

; 476     int			errors = 0;
	mov	dword ptr [ebp-018h],0h;	errors

; 473     PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
	call	__nw__FUi

; 481 	rc = read(0, 1, sec);
	push	eax

; 473     PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
	mov	[ebp-014h],eax;	sec
	mov	eax,ebx

; 481 	rc = read(0, 1, sec);
	sub	esp,0ch
	mov	ebx,[eax]
	mov	ecx,01h
	mov	edi,[ebx+01ch]
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+018h]

; 474     int			modified = 0;
	xor	esi,esi

; 481 	rc = read(0, 1, sec);
	add	esp,010h

; 482 	if( rc )
	test	eax,eax
	je	@BLBL63

; 484 	    Verbose(1, "VORDev::ioSync", "can't read own SEC_PHYSDEV - rc %lu", rc);
	push	eax

; 485 	    ++errors;
	mov	dword ptr [ebp-018h],01h;	errors

; 484 	    Verbose(1, "VORDev::ioSync", "can't read own SEC_PHYSDEV - rc %lu", rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE9
	mov	edx,offset FLAT:@CBE19
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 486 	    break;
	jmp	@BLBL64
@BLBL63:

; 489 	if( memcmp(sec->sectype, "PHYSDEVICE      ", 16) != 0
	mov	ebx,[ebp-014h];	sec
	mov	ecx,010h
	mov	edx,offset FLAT:@CBE3
	mov	eax,ebx
	call	memcmp
	test	eax,eax
	jne	@BLBL65
	mov	eax,ebx
	mov	dx,01feh
	call	Crc16__FPvUs
	cmp	[ebx+01feh],ax
	je	@BLBL66
@BLBL65:

; 494 	    memset(sec, 0, sizeof(*sec));
	mov	edi,[ebp-014h];	sec

; 492 	    Verbose(1, "VORDev", "new PHYSDEV sector");
	mov	ecx,offset FLAT:@CBE20
	mov	edx,offset FLAT:@CBE4
	mov	eax,01h
	call	Verbose

; 494 	    memset(sec, 0, sizeof(*sec));
	mov	ecx,0200h
	xor	edx,edx
	mov	eax,edi
	call	memset

; 496 	    memcpy(sec->sectype, "PHYSDEVICE      ", 16);
	mov	ecx,010h
	mov	edx,offset FLAT:@CBE3
	mov	eax,edi
	call	memcpy
	mov	eax,edi

; 493 	    ++modified;
	inc	esi

; 497 	    sec->u.s.adminspace = VRAID_ADMINSECTORS;
	mov	word ptr [eax+01ah],020h

; 498 	    size = sec->u.s.size = (partsize - sec->u.s.adminspace);
	mov	edx,[ebp+08h];	this
	mov	ecx,[edx+018h]
	sub	ecx,020h
	mov	[eax+01ch],ecx
	mov	[edx+01ch],ecx

; 500 	    memcpy(sec->u.s.id, id, sizeof(DEVID));
	add	edx,08h
	add	eax,014h
	mov	ecx,06h
	call	memcpy

; 501 	}
@BLBL66:

; 503 	if( size != sec->u.s.size )
	mov	ebx,[ebp+08h];	this
	mov	eax,[ebp-014h];	sec
	mov	ebx,[ebx+01ch]
	cmp	[eax+01ch],ebx
	je	@BLBL67

; 505 	    ++modified;
	inc	esi

; 506 	    sec->u.s.size = size;
	mov	[eax+01ch],ebx

; 507 	}
@BLBL67:

; 509 	if( modified )
	cmp	esi,0h
	je	@BLBL64

; 511 	    sec->timestamp = time(NULL);
	mov	ebx,[ebp-014h];	sec
	xor	eax,eax
	call	time
	mov	[ebx+010h],eax

; 512 	    sec->crc = Crc16(sec, SECTOR_SIZE-2); /* !!! */
	mov	dx,01feh
	mov	eax,ebx
	call	Crc16__FPvUs
	mov	[ebx+01feh],ax

; 513 	    rc = write(0, 1, sec);
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

; 514 	    if( rc != 0 )
	test	eax,eax
	je	@BLBL64

; 516 		Verbose(1, "VORDev::ioSync", "write(0,1,...) - rc %lu, not updated", rc);
	push	eax

; 517 		++errors;
	inc	dword ptr [ebp-018h];	errors

; 516 		Verbose(1, "VORDev::ioSync", "write(0,1,...) - rc %lu, not updated", rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE21
	mov	edx,offset FLAT:@CBE19
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 518 		break;
@BLBL64:

; 524     return errors;
	mov	eax,[ebp-018h];	errors
	add	esp,018h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ioSync__6VORDevFv	endp

; 388 VORDev::ioDeleteYourself()

	public ioDeleteYourself__6VORDevFv
ioDeleteYourself__6VORDevFv	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi

; 390     PUCHAR	buffer = new UCHAR[SECTOR_SIZE];
	mov	eax,0200h

; 388 VORDev::ioDeleteYourself()
	push	esi
	mov	[ebp+08h],ebx;	this
	sub	esp,01ch

; 393     int		errors = 0;
	mov	dword ptr [ebp-010h],0h;	errors

; 390     PUCHAR	buffer = new UCHAR[SECTOR_SIZE];
	call	__vn__FUi
	mov	edi,eax
	mov	[ebp-014h],edi;	buffer

; 391     PMBR const	mbr = (PMBR)buffer;
	mov	[ebp-018h],edi;	mbr

; 399 	memset(buffer, 0, SECTOR_SIZE);
	mov	ecx,0200h

; 395     delete_disk = False;			/* destructor: don't delete theDisk */
	mov	byte ptr [ebx+021h],0h

; 399 	memset(buffer, 0, SECTOR_SIZE);
	xor	edx,edx
	call	memset
	mov	eax,ebx

; 400 	rc = write(0, 1, buffer);		/* clear PHYSDEV sector */
	push	edi
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+024h]
	mov	ecx,01h
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+020h]
	add	esp,010h

; 401 	if( rc != 0 )
	test	eax,eax
	je	@BLBL46

; 403 	    Verbose(1, "VORDev::ioDeleteYourself", "can't clear SEC_PHYSDEV - rc %lu", rc);
	push	eax

; 404 	    ++errors;				/* ignore errors */
	mov	dword ptr [ebp-010h],01h;	errors

; 403 	    Verbose(1, "VORDev::ioDeleteYourself", "can't clear SEC_PHYSDEV - rc %lu", rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE14
	mov	edx,offset FLAT:@CBE13
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 405 	}
@BLBL46:

; 410 	rc = theDisk->read(0, 1, buffer);
	mov	eax,[ebp+08h];	this
	push	dword ptr [ebp-014h];	buffer

; 408 	int	found = -1;			/* indices */
	mov	dword ptr [ebp-01ch],0ffffffffh;	found

; 410 	rc = theDisk->read(0, 1, buffer);
	mov	eax,[eax+010h]
	sub	esp,0ch
	mov	ebx,[eax]
	mov	ecx,01h
	mov	edi,[ebx+01ch]
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+018h]
	add	esp,010h

; 411 	if( rc != 0 )
	test	eax,eax
	je	@BLBL47

; 413 	    Verbose(1, "VORDev::ioDeleteYourself", "can't read MBR - rc %lu", rc);
	push	eax

; 414 	    ++errors;
	inc	dword ptr [ebp-010h];	errors

; 413 	    Verbose(1, "VORDev::ioDeleteYourself", "can't read MBR - rc %lu", rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE15
	mov	edx,offset FLAT:@CBE13
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 415 	    break;
	jmp	@BLBL48
@BLBL47:

; 418 	if( mbr->Signature != 0xAA55 )
	mov	eax,[ebp-018h];	mbr
	cmp	word ptr [eax+01feh],0aa55h
	je	@BLBL49

; 420 	    Verbose(1, "VORDev", "invalid MBR, done");
	mov	ecx,offset FLAT:@CBE16
	mov	edx,offset FLAT:@CBE4
	mov	eax,01h
	call	Verbose

; 421 	    break;
	jmp	@BLBL48
@BLBL49:

; 424 	for( i = 0; i < 4; ++i )
	mov	ebx,[ebp-018h];	mbr
	xor	esi,esi
	xor	edi,edi
@BLBL51:

; 426 	    if( mbr->PartitionTable[i].SysIndicator == VRAID_PARTTYPE )
	lea	eax,dword ptr [ebx+edi]
	cmp	byte ptr [eax+01c2h],07ch
	jne	@BLBL54

; 428 		memset(&mbr->PartitionTable[i], 0, sizeof(mbr->PartitionTable[i]));
	xor	edx,edx
	add	eax,01beh
	mov	ecx,010h
	call	memset

; 429 		found = i;
	mov	[ebp-01ch],esi;	found

; 430 		break;
	jmp	@BLBL50
@BLBL54:

; 424 	for( i = 0; i < 4; ++i )
	inc	esi
	add	edi,010h
	cmp	esi,04h
	jl	@BLBL51
@BLBL50:

; 434 	if( found == -1 )
	cmp	dword ptr [ebp-01ch],0ffffffffh;	found
	jne	@BLBL56

; 436 	    Verbose(0, "VORDev", "no VRAID partition, done");
	xor	eax,eax
	mov	ecx,offset FLAT:@CBE17
	mov	edx,offset FLAT:@CBE4
	call	Verbose

; 437 	    break;				/* rc still 0 */
	jmp	@BLBL48
@BLBL56:

; 440 	rc = theDisk->write(0, 1, buffer);
	mov	eax,[ebp+08h];	this
	push	dword ptr [ebp-014h];	buffer
	mov	eax,[eax+010h]
	sub	esp,0ch
	mov	ebx,[eax]
	mov	ecx,01h
	mov	edi,[ebx+024h]
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+020h]
	add	esp,010h

; 441 	if( rc != 0 )
	test	eax,eax
	je	@BLBL48

; 443 	    Verbose(1, "VORDev::ioDeleteYourself", "can't write MBR - rc %lu", rc);
	push	eax

; 444 	    ++errors;
	inc	dword ptr [ebp-010h];	errors

; 443 	    Verbose(1, "VORDev::ioDeleteYourself", "can't write MBR - rc %lu", rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE18
	mov	edx,offset FLAT:@CBE13
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 445 	    break;
@BLBL48:

; 450     return errors;
	mov	eax,[ebp-010h];	errors
	add	esp,01ch
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ioDeleteYourself__6VORDevFv	endp

; 321 VORDev::ioRemoveParent()

	public ioRemoveParent__6VORDevFv
ioRemoveParent__6VORDevFv	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi

; 323     PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
	mov	eax,0200h

; 321 VORDev::ioRemoveParent()
	push	esi
	mov	[ebp+08h],ebx;	this
	sub	esp,014h

; 324     int		errors = 0;
	xor	esi,esi

; 323     PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
	call	__nw__FUi
	mov	edi,eax

; 330     memset(sec, 0, SECTOR_SIZE);
	mov	ecx,0200h
	xor	edx,edx
	call	memset
	mov	eax,ebx

; 331     rc = write(1, 1, sec);
	push	edi

; 323     PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
	mov	[ebp-014h],edi;	sec

; 331     rc = write(1, 1, sec);
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+024h]
	mov	ecx,01h
	mov	edx,01h
	add	eax,edi
	call	dword ptr [ebx+020h]
	add	esp,010h

; 332     if( rc != 0 )
	test	eax,eax
	je	@BLBL37

; 334 	Verbose(1, "VORDev::ioRemoveParent", "can't clear parent SEC_VRDEV - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE12
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE11
	mov	eax,01h
	call	Verbose

; 335 	++errors;
	mov	esi,01h

; 334 	Verbose(1, "VORDev::ioRemoveParent", "can't clear parent SEC_VRDEV - rc %lu", rc);
	add	esp,010h

; 341     do
@BLBL37:

; 343 	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
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

; 344 	if( rc != 0 )
	test	eax,eax
	je	@BLBL38

; 346 	    Verbose(1, "VORDev::ioRemoveParent", "can't read own SEC_PHYSDEV - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE9
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE11
	mov	eax,01h
	call	Verbose

; 347 	    ++errors;
	inc	esi

; 346 	    Verbose(1, "VORDev::ioRemoveParent", "can't read own SEC_PHYSDEV - rc %lu", rc);
	add	esp,010h

; 348 	    break;
	jmp	@BLBL39
@BLBL38:

; 351 	sec->u.s.size = size = partsize - sec->u.s.adminspace;
	mov	ebx,[ebp-014h];	sec
	mov	edi,[ebp+08h];	this
	movzx	edx,word ptr [ebx+01ah]
	mov	ecx,[edi+018h]
	sub	ecx,edx
	mov	[edi+01ch],ecx
	mov	[ebx+01ch],ecx

; 352 	sec->timestamp = time(NULL);
	xor	eax,eax
	call	time
	mov	[ebx+010h],eax

; 353 	sec->crc = Crc16(sec, SECTOR_SIZE-2);	/* !!! */
	mov	dx,01feh
	mov	eax,ebx
	call	Crc16__FPvUs
	mov	[ebx+01feh],ax
	mov	eax,edi

; 355 	rc = write(
; 355 0, 1, sec);
	push	ebx
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+024h]
	mov	ecx,01h
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+020h]
	add	esp,010h

; 356 	if( rc != 0 )
	test	eax,eax
	je	@BLBL40

; 358 	    Verbose(1, "VORDev::ioRemoveParent", "can't write own SEC_PHYSDEV - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE10
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE11
	mov	eax,01h
	call	Verbose

; 359 	    ++errors;
	inc	esi

; 358 	    Verbose(1, "VORDev::ioRemoveParent", "can't write own SEC_PHYSDEV - rc %lu", rc);
	add	esp,010h

; 360 	    break;
	jmp	@BLBL39
@BLBL40:

; 363 	parent = NULL;				/* none */
	mov	eax,[ebp+08h];	this
	mov	dword ptr [eax+04h],0h

; 365     while(0);
@BLBL39:

; 367     delete sec;
	mov	eax,[ebp-014h];	sec
	call	__dl__FPv

; 368     return errors;
	add	esp,014h
	mov	eax,esi
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ioRemoveParent__6VORDevFv	endp

; 267 VORDev::ioChangeID(DEVID newid)

	public ioChangeID__6VORDevFPUc
ioChangeID__6VORDevFPUc	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,edx
	push	edi
	mov	edi,eax
	push	esi

; 269     PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
	mov	eax,0200h

; 267 VORDev::ioChangeID(DEVID newid)
	sub	esp,014h
	mov	[ebp+0ch],ebx;	newid

; 269     PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
	call	__nw__FUi
	mov	esi,eax
	mov	edx,ebx

; 272     memcpy(id, newid, sizeof(DEVID));		/* update object */
	mov	ecx,06h
	lea	eax,[edi+08h]
	call	memcpy
	mov	eax,edi

; 278 	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
	push	esi

; 267 VORDev::ioChangeID(DEVID newid)
	mov	[ebp+08h],edi;	this

; 269     PSEC_PHYSDEV	sec = new SEC_PHYSDEV;
	mov	[ebp-014h],esi;	sec

; 278 	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
	mov	ecx,01h

; 270     int			errors = 0;
	mov	dword ptr [ebp-010h],0h;	errors

; 278 	rc = read(0, 1, sec);			/* remember: 0 defines ourself */
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+01ch]
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+018h]
	add	esp,010h

; 279 	if( rc != 0 )
	test	eax,eax
	je	@BLBL29

; 281 	    Verbose(1,"VORDev::ioChangeID","can't read own SEC_PHYSDEV - rc %lu",rc);
	push	eax

; 282 	    ++errors;
	mov	dword ptr [ebp-010h],01h;	errors

; 281 	    Verbose(1,"VORDev::ioChangeID","can't read own SEC_PHYSDEV - rc %lu",rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE9
	mov	edx,offset FLAT:@CBE8
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 283 	    break;
	jmp	@BLBL30
@BLBL29:

; 286 	memcpy(sec->u.s.id, newid, sizeof(DEVID));
	mov	ebx,[ebp-014h];	sec
	mov	edx,[ebp+0ch];	newid
	mov	ecx,06h
	lea	eax,[ebx+014h]
	call	memcpy

; 287 	sec->timestamp = time(NULL);		/* UTC of change */
	xor	eax,eax
	call	time
	mov	[ebx+010h],eax

; 288 	sec->crc = Crc16(sec, SECTOR_SIZE-2);	/* !!! */
	mov	dx,01feh
	mov	eax,ebx
	call	Crc16__FPvUs
	mov	[ebx+01feh],ax

; 290 	rc = write(0, 1, sec);
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

; 291 	if( rc != 0 )
	test	eax,eax
	je	@BLBL30

; 293 	    Verbose(1, "VORDev::ioChangeID", "can't write own SEC_PHYSDEV - rc %lu", rc);
	push	eax

; 294 	    ++errors;
	inc	dword ptr [ebp-010h];	errors

; 293 	    Verbose(1, "VORDev::ioChangeID", "can't write own SEC_PHYSDEV - rc %lu", rc);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE10
	mov	edx,offset FLAT:@CBE8
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 295 	    break;
@BLBL30:

; 300     delete sec;
	mov	eax,[ebp-014h];	sec
	call	__dl__FPv

; 301     return errors;
	mov	eax,[ebp-010h];	errors
	add	esp,014h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ioChangeID__6VORDevFPUc	endp

; 240 VORDev::queryIcon()

	public queryIcon__6VORDevFv
queryIcon__6VORDevFv	proc
	push	ebx

; 242     if( parent != 0  &&  parent->isValidChild(this) == False )
	mov	ebx,[eax+04h]

; 240 VORDev::queryIcon()
	push	edi
	sub	esp,08h

; 242     if( parent != 0  &&  parent->isValidChild(this) == False )
	test	ebx,ebx
	je	@BLBL22
	xchg	ebx,eax
	mov	edx,ebx
	mov	ecx,[eax]
	mov	edi,[ecx+0ach]
	add	eax,edi
	call	dword ptr [ecx+0a8h]
	test	al,al
	mov	eax,ebx
	jne	@BLBL22

; 243 	return inv_icon;
	mov	eax,[eax+024h]
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
@BLBL22:

; 244     else if( writable == True )
	cmp	byte ptr [eax+020h],01h
	jne	@BLBL24

; 245 	return rw_icon;
	mov	eax,[eax+028h]
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
@BLBL24:

; 247 	return ro_icon;
	mov	eax,[eax+02ch]
	add	esp,08h
	pop	edi
	pop	ebx
	ret	
queryIcon__6VORDevFv	endp

; 211 char *

	public allocateDescription__6VORDevFv
allocateDescription__6VORDevFv	proc
	push	ebx
	mov	ebx,eax
	sub	esp,04h

; 214     char * descr = new char[80];
	mov	eax,050h
	call	__vn__FUi
	mov	edx,ebx
	mov	ebx,eax

; 215     sprintf(descr, "PHYSDEV (OS/2 Device %lu)\n"
	mov	eax,[edx+010h]
	call	queryIndex__8VOS2DiskFv
	mov	ecx,[edx+01ch]
	shr	ecx,01h
	shr	ecx,0ah
	push	ecx
	movzx	ecx,byte ptr [edx+0dh]
	push	ecx
	movzx	ecx,byte ptr [edx+0ch]
	push	ecx
	movzx	ecx,byte ptr [edx+0bh]
	push	ecx
	movzx	ecx,byte ptr [edx+0ah]
	push	ecx
	movzx	ecx,byte ptr [edx+09h]
	push	ecx
	movzx	edx,byte ptr [edx+08h]
	push	edx
	mov	edx,offset FLAT:@CBE7
	push	eax
	mov	eax,ebx
	sub	esp,08h
	call	_sprintfieee
	mov	eax,ebx

; 220     return descr;
	add	esp,02ch
	pop	ebx
	ret	
allocateDescription__6VORDevFv	endp

; 1163 VOS2Disk::~VOS2Disk()

	public __dt__8VOS2DiskFv
__dt__8VOS2DiskFv	proc
	push	ebx
	sub	esp,04h
	mov	[esp+010h],edx;	__dtorFlags
	mov	[esp+0ch],eax;	this

; 1167     if( writable == True )
	cmp	byte ptr [eax+01ch],01h

; 1163 VOS2Disk::~VOS2Disk()
	mov	dword ptr [eax],offset FLAT:__vft8VOS2Disk5VDisk

; 1167     if( writable == True )
	jne	@BLBL161

; 1169 	rc = PDskUnlockIO(pdskhd);
	mov	eax,[eax+08h]
	call	PDskUnlockIO

; 1170 	if( rc != 0 )
	test	eax,eax
	je	@BLBL161

; 1171 	    Verbose(0, "VOS2Disk", "PDskUnlockIO - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE36
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE34
	xor	eax,eax
	call	Verbose
	add	esp,010h

; 1172     }
@BLBL161:

; 1173     rc = PDskClose(pdskhd);
	mov	ebx,[esp+0ch];	this
	mov	eax,[ebx+08h]
	call	PDskClose

; 1174 }
	test	byte ptr [esp+010h],01h;	__dtorFlags

; 1173     rc = PDskClose(pdskhd);
	mov	eax,ebx

; 1174 }
	je	@BLBL164
	call	__dl__FPv
@BLBL164:
	mov	eax,[esp+0ch];	this
	add	esp,04h
	pop	ebx
	ret	
__dt__8VOS2DiskFv	endp

; 214     Boolean	isWritable() { return writable; }

isWritable__6VORDevFv	proc
	movzx	eax,byte ptr [eax+020h]
	ret	
isWritable__6VORDevFv	endp

; 213     void	forceUnwritable() { writable = False; }

forceUnwritable__6VORDevFv	proc
	mov	byte ptr [eax+020h],0h
	ret	
forceUnwritable__6VORDevFv	endp

; 209     Boolean	isHostdrive() { return False; }

isHostdrive__6VORDevFv	proc
	xor	eax,eax
	ret	
isHostdrive__6VORDevFv	endp

; 208     UCHAR	isArray() { return 0; }

isArray__6VORDevFv	proc
	xor	eax,eax
	ret	
isArray__6VORDevFv	endp

; 206     VOS2Disk *	queryWhere() { return theDisk; }

queryWhere__6VORDevFv	proc
	mov	eax,[eax+010h]
	ret	
queryWhere__6VORDevFv	endp

; 179     Boolean	isWritable() { return writable; }

isWritable__6VFRDevFv	proc
	movzx	eax,byte ptr [eax+01ch]
	ret	
isWritable__6VFRDevFv	endp

; 178     void	forceUnwritable() { writable = False; }

forceUnwritable__6VFRDevFv	proc
	mov	byte ptr [eax+01ch],0h
	ret	
forceUnwritable__6VFRDevFv	endp

; 174     Boolean	isHostdrive() { return False; }

isHostdrive__6VFRDevFv	proc
	xor	eax,eax
	ret	
isHostdrive__6VFRDevFv	endp

; 173     UCHAR	isArray() { return 0; }

isArray__6VFRDevFv	proc
	xor	eax,eax
	ret	
isArray__6VFRDevFv	endp

; 137     virtual VOS2Disk *	queryWhere() { return NULL; }

queryWhere__5VRDevFv	proc
	xor	eax,eax
	ret	
queryWhere__5VRDevFv	endp

; 100     Boolean		isWritable() { return writable; }

isWritable__8VOS2DiskFv	proc
	movzx	eax,byte ptr [eax+01ch]
	ret	
isWritable__8VOS2DiskFv	endp

; 1143 VOS2Disk::VOS2Disk(ULONG idx,ULONG handle)

	public __ct__8VOS2DiskFUlT1
__ct__8VOS2DiskFUlT1	proc
	push	ebx
	mov	ebx,eax
	sub	esp,0ch

; 80 };
	mov	dword ptr [ebx],offset FLAT:__vft8VOS2Disk5VDisk

; 1145 
	call	create__8VOS2DiskFUlT1

; 1146 
	add	esp,0ch

; 1145     create(idx, handle);
	mov	eax,ebx

; 1146 }
	pop	ebx
	ret	
__ct__8VOS2DiskFUlT1	endp

; 1106 VOS2Disk::VOS2Disk(ULONG idx)

	public __ct__8VOS2DiskFUl
__ct__8VOS2DiskFUl	proc
	push	ebx
	mov	ebx,eax
	push	edi
	mov	edi,edx
	sub	esp,010h

; 80 };
	mov	dword ptr [ebx],offset FLAT:__vft8VOS2Disk5VDisk

; 1113 
	lea	edx,[esp+0ch];	handle
	mov	eax,edi
	call	PDskOpen
	xchg	ebx,eax
	mov	edx,edi

; 1114 
	test	ebx,ebx
	je	@BLBL156
	xchg	ebx,eax

; 1116 
	push	eax
	mov	ecx,offset FLAT:@CBE35
	push	dword ptr [ebx+04h]
	mov	edx,offset FLAT:@CBE34
	sub	esp,0ch
	mov	eax,01h
	call	Verbose

; 1117 
	add	esp,024h

; 1116 	Verbose(1, "VOS2Disk", "PDskOpen(%lu) - rc %lu", pdskidx, rc);
	mov	eax,ebx

; 1117 	return;
	pop	edi
	pop	ebx
	ret	
@BLBL156:
	mov	ebx,eax

; 1120     rc = create(idx, handle);
	mov	ecx,[esp+0ch];	handle
	call	create__8VOS2DiskFUlT1

; 1121     return;
	add	esp,010h

; 1120     rc = create(idx, handle);
	mov	eax,ebx

; 1121     return;
	pop	edi
	pop	ebx
	ret	
__ct__8VOS2DiskFUl	endp

; 123     virtual	~VRDev() {}			// see Stroustroup pg216!!!

__dt__5VRDevFv	proc
	push	ebx
	mov	dword ptr [eax],offset FLAT:@146__vft5VRDev5VDisk
	sub	esp,04h
	test	dl,01h
	je	@BLBL188
	mov	ebx,eax
	call	__dl__FPv
	mov	eax,ebx
@BLBL188:
	add	esp,04h
	pop	ebx
	ret	
__dt__5VRDevFv	endp

; 696 VFRDev::~VFRDev()

	public __dt__6VFRDevFv
__dt__6VFRDevFv	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffe0h
	sub	esp,018h

; 697 {
	mov	dword ptr [ebp-020h],0h;	__es

; 696 VFRDev::~VFRDev()
	push	ebx
	mov	ebx,eax
	push	edi
	mov	[ebp-024h],edx;	@CBE57
	push	esi
	mov	[ebp+08h],ebx;	this

; 697 {
	mov	dword ptr [ebp-018h],offset FLAT:@bb__fsm_tab;	__es
	mov	[ebp-014h],ebx;	__es
	mov	dword ptr [ebp-010h],0h;	__es

; 696 VFRDev::~VFRDev()
	mov	dword ptr [ebp-020h],01h;	__es

; 698     WinDestroyPointer(rw_icon);
	push	dword ptr [ebx+024h]

; 696 VFRDev::~VFRDev()
	mov	dword ptr [ebx],offset FLAT:__vft6VFRDev5VDisk

; 698     WinDestroyPointer(rw_icon);
	call	WinDestroyPointer
	add	esp,04h

; 699     WinDestroyPointer(ro_icon);
	push	dword ptr [ebx+028h]
	call	WinDestroyPointer
	mov	eax,ebx
	mov	edx,[ebp-024h];	@CBE57
	add	esp,04h

; 700 }
	mov	dword ptr [ebp-020h],0h;	__es

; 123     virtual	~VRDev() {}			// see Stroustroup pg216!!!
	test	dl,01h
	mov	dword ptr [eax],offset FLAT:@146__vft5VRDev5VDisk
	je	@BLBL93
	sub	esp,04h
	call	__dl__FPv
	add	esp,04h
@BLBL93:
	pop	esi
	mov	eax,[ebp+08h];	this
	pop	edi
	pop	ebx
	add	esp,01ch
	pop	fs:[0h]
	leave	
	ret	
__dt__6VFRDevFv	endp

; 187 VORDev::~VORDev()

	public __dt__6VORDevFv
__dt__6VORDevFv	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffe0h
	mov	[ebp+0ch],edx;	__dtorFlags
	sub	esp,014h

; 188 {
	mov	dword ptr [ebp-020h],0h;	__es

; 187 VORDev::~VORDev()
	push	ebx
	mov	[ebp+08h],eax;	this
	push	edi

; 188 {
	mov	dword ptr [ebp-018h],offset FLAT:@4b__fsm_tab;	__es
	mov	[ebp-014h],eax;	__es

; 187 VORDev::~VORDev()
	push	esi

; 188 {
	mov	dword ptr [ebp-010h],0h;	__es

; 187 VORDev::~VORDev()
	mov	dword ptr [ebp-020h],01h;	__es

; 189     if( delete_disk == True )
	cmp	byte ptr [eax+021h],01h

; 187 VORDev::~VORDev()
	mov	dword ptr [eax],offset FLAT:__vft6VORDev5VDisk

; 189     if( delete_disk == True )
	jne	@BLBL13

; 190 	delete theDisk;				/* no need for that disk object */
	mov	eax,[eax+010h]
	test	eax,eax
	je	@BLBL13
	sub	esp,08h
	mov	edx,03h
	call	__dt__8VOS2DiskFv
	add	esp,08h
@BLBL13:

; 192     WinDestroyPointer(rw_icon);
	mov	ebx,[ebp+08h];	this
	push	dword ptr [ebx+028h]
	call	WinDestroyPointer
	add	esp,04h

; 193     WinDestroyPointer(ro_icon);
	push	dword ptr [ebx+02ch]
	call	WinDestroyPointer
	mov	eax,ebx

; 194 }
	mov	dword ptr [ebp-020h],0h;	__es

; 193     WinDestroyPointer(ro_icon);
	add	esp,04h

; 123     virtual	~VRDev() {}			// see Stroustroup pg216!!!
	mov	dword ptr [eax],offset FLAT:@146__vft5VRDev5VDisk
	test	byte ptr [ebp+0ch],01h;	__dtorFlags
	je	@BLBL17
	sub	esp,04h
	call	__dl__FPv
	add	esp,04h
@BLBL17:
	pop	esi
	mov	eax,[ebp+08h];	this
	pop	edi
	pop	ebx
	add	esp,018h
	pop	fs:[0h]
	leave	
	ret	
__dt__6VORDevFv	endp
CODE32	ends
EH_CODE	segment

__dftbdt__5VRDevFv	proc
	mov	dword ptr [eax],offset FLAT:@146__vft5VRDev5VDisk
	ret	
__dftbdt__5VRDevFv	endp

; 622 
EH_CODE	ends
CODE32	segment

	public __ct__6VFRDevFUs
__ct__6VFRDevFUs	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffdch
	sub	esp,018h

; 623 
	mov	dword ptr [ebp-024h],0h;	__es

; 622 VFRDev::VFRDev(USHORT flthd)
	push	ebx
	mov	ebx,edx
	push	edi
	mov	edi,eax
	push	esi
	mov	[ebp+08h],edi;	this

; 625     PSEC_PHYSDEV sec = new SEC_PHYSDEV;
	sub	esp,04h

; 623 {
	mov	dword ptr [ebp-01ch],offset FLAT:@a9__fsm_tab;	__es
	mov	[ebp-018h],edi;	__es

; 625     PSEC_PHYSDEV sec = new SEC_PHYSDEV;
	mov	eax,0200h

; 623 {
	mov	dword ptr [ebp-014h],0h;	__es

; 80 };
	mov	dword ptr [edi],offset FLAT:@146__vft5VRDev5VDisk

; 143 };
	mov	dword ptr [ebp-024h],01h;	__es
	mov	dword ptr [edi],offset FLAT:__vft6VFRDev5VDisk

; 625 
	call	__nw__FUi
	mov	ecx,eax
	mov	eax,edi
	sub	esp,04h
	mov	[ebp-010h],ecx;	sec
	mov	edx,ebx

; 627 
	mov	[eax+010h],dx

; 628 
	mov	dword ptr [eax+04h],0h

; 640 
	lea	edx,[eax+014h]

; 634     writable = True;
	mov	byte ptr [eax+01ch],01h

; 640 	rc = DriverPhyssize(hd, &partsize);
	mov	ax,[eax+010h]
	call	DriverPhyssize__FUsPUl
	add	esp,08h

; 641 	if( rc != 0 )
	test	eax,eax
	je	@BLBL83

; 643 	    Verbose(1, "VFRDev::VFRDev", "DriverPhyssize - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE23
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE22
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 644 	    break;
	jmp	@BLBL84
@BLBL83:

; 647 	rc = read(0, 1, sec);
	push	dword ptr [ebp-010h];	sec
	mov	eax,[ebp+08h];	this
	sub	esp,0ch
	mov	ebx,[eax]
	mov	ecx,01h
	mov	edi,[ebx+01ch]
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+018h]
	add	esp,010h

; 648 	if( rc != 0 )
	test	eax,eax
	je	@BLBL85

; 650 	    Verbose(1, "VFRDev::VFRDev", "read(0,1,...) - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE24
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE22
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 651 	    break;
	jmp	@BLBL84
@BLBL85:

; 654 	if( memcmp(sec->sectype, "PHYSDEVICE      ", 16) != 0 )
	sub	esp,0ch
	mov	eax,[ebp-010h];	sec
	mov	ecx,010h
	mov	edx,offset FLAT:@CBE3
	call	memcmp
	add	esp,0ch
	test	eax,eax
	je	@BLBL86

; 656 	    Verbose(1, "VFRDev", "missing PHYSDEV mark");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE5
	mov	edx,offset FLAT:@CBE25
	mov	eax,01h
	call	Verbose
	add	esp,0ch

; 658 	    break;
	jmp	@BLBL84
@BLBL86:

; 661 	crc = Crc16(sec, SECTOR_SIZE-2);
	sub	esp,08h
	mov	ebx,[ebp-010h];	sec
	mov	dx,01feh
	mov	eax,ebx
	call	Crc16__FPvUs

; 662 	if( crc != sec->crc )
	mov	cx,[ebx+01feh]

; 661 	crc = Crc16(sec, SECTOR_SIZE-2);
	add	esp,08h

; 662 	if( crc != sec->crc )
	cmp	ax,cx
	je	@BLBL87

; 664 	    Verbose(1, "VFRDev", "CRC error in PHYSDEV sector (%#x, %#x)",
	mov	edx,offset FLAT:@CBE25
	movzx	ecx,cx
	push	ecx
	movzx	eax,ax
	push	eax
	mov	ecx,offset FLAT:@CBE6
	sub	esp,0ch
	mov	eax,01h
	call	Verbose
	add	esp,014h

; 667 	    break;
	jmp	@BLBL84
@BLBL87:

; 669 	memcpy(id, sec->u.s.id, sizeof(DEVID));
	mov	edi,[ebp+08h];	this
	sub	esp,0ch
	mov	ebx,[ebp-010h];	sec
	mov	ecx,06h
	lea	edx,[ebx+014h]
	lea	eax,[edi+08h]
	call	memcpy

; 670 	size = sec->u.s.size;
	mov	ecx,[ebx+01ch]

; 669 	memcpy(id, sec->u.s.id, sizeof(DEVID));
	add	esp,0ch

; 670 	size = sec->u.s.size;
	mov	[edi+018h],ecx

; 672     while(0);
@BLBL84:

; 675     inv_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_INVCHILDPDEV);
	mov	edi,[ebp+08h];	this

; 673     delete sec;
	sub	esp,04h
	mov	eax,[ebp-010h];	sec
	call	__dl__FPv
	add	esp,04h

; 675     inv_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_INVCHILDPDEV);
	push	0198h
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	[edi+020h],eax

; 676     rw_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_PDEVDRIVE);
	push	0193h
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	[edi+024h],eax

; 677     ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDPDEV);
	push	0196h
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	ecx,eax

; 678 }
	pop	esi

; 677     ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDPDEV);
	mov	eax,edi

; 678 }
	pop	edi

; 677     ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDPDEV);
	mov	[eax+028h],ecx

; 678 }
	pop	ebx
	add	esp,01ch
	pop	fs:[0h]
	leave	
	ret	
__ct__6VFRDevFUs	endp

; 109 VORDev::VORDev(VOS2Disk * where,ULONG pstart,ULONG psize)

	public __ct__6VORDevFP8VOS2DiskUlT2
__ct__6VORDevFP8VOS2DiskUlT2	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffd4h
	sub	esp,020h

; 110 {
	mov	dword ptr [ebp-02ch],0h;	__es

; 109 VORDev::VORDev(VOS2Disk * where,ULONG pstart,ULONG psize)
	push	ebx
	mov	ebx,eax
	push	edi
	mov	[ebp+08h],ebx;	this
	push	esi
	mov	esi,edx

; 111     PSEC_PHYSDEV sec = new SEC_PHYSDEV;
	sub	esp,04h

; 110 {
	mov	dword ptr [ebp-024h],offset FLAT:@34__fsm_tab;	__es
	mov	[ebp-020h],ebx;	__es

; 109 VORDev::VORDev(VOS2Disk * where,ULONG pstart,ULONG psize)
	mov	edi,ecx

; 110 {
	mov	dword ptr [ebp-01ch],0h;	__es

; 80 };
	mov	dword ptr [ebx],offset FLAT:@146__vft5VRDev5VDisk

; 111 
	mov	eax,0200h

; 143 };
	mov	dword ptr [ebp-02ch],01h;	__es
	mov	dword ptr [ebx],offset FLAT:__vft6VORDev5VDisk

; 111     PSEC_PHYSDEV sec = new SEC_PHYSDEV;
	call	__nw__FUi
	mov	ecx,edi
	mov	edi,eax
	mov	[ebp-010h],edi;	sec
	mov	edx,esi

; 115     theDisk = where;
	mov	[ebx+010h],edx

; 114     delete_disk = True;
	mov	byte ptr [ebx+021h],01h

; 120     writable = theDisk->isWritable();
	mov	eax,[ebx+010h]

; 116     partstart = pstart;
	mov	[ebx+014h],ecx

; 117     partsize = psize;
	mov	ecx,[ebp+014h];	psize
	mov	[ebx+018h],ecx

; 118     parent = NULL;
	mov	dword ptr [ebx+04h],0h

; 119     size = 0;
	mov	dword ptr [ebx+01ch],0h

; 120     writable = theDisk->isWritable();
	mov	ecx,[eax]
	mov	edx,[ecx+0ch]
	add	eax,edx
	call	dword ptr [ecx+08h]
	add	esp,04h
	mov	ecx,eax
	mov	eax,ebx

; 126 	rc = read(0, 1, sec);
	push	edi
	mov	ebx,[eax]
	sub	esp,0ch

; 120     writable = theDisk->isWritable();
	mov	[eax+020h],cl

; 126 	rc = read(0, 1, sec);
	mov	edi,[ebx+01ch]
	mov	ecx,01h
	xor	edx,edx
	add	eax,edi
	call	dword ptr [ebx+018h]
	add	esp,010h
	mov	[ebp-014h],eax;	rc

; 127 	if( rc != 0 )
	test	eax,eax
	je	@BLBL4

; 129 	    Verbose(1, "VORDev::VORDev", "can't read PHYSDEV - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE2
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE1
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 130 	    break;
	jmp	@BLBL5
@BLBL4:

; 133 	crc = Crc16(sec, SECTOR_SIZE-2);
	sub	esp,08h
	mov	esi,[ebp-010h];	sec
	mov	dx,01feh
	mov	eax,esi
	call	Crc16__FPvUs
	sub	esp,04h
	mov	[ebp-016h],ax;	crc
	mov	eax,esi

; 134 	if( memcmp(sec->sectype, "PHYSDEVICE      ", 16) != 0 )
	mov	ecx,010h
	mov	edx,offset FLAT:@CBE3
	call	memcmp
	add	esp,0ch
	test	eax,eax
	je	@BLBL6

; 136 	    Verbose(1, "VORDev", "missing PHYSDEV mark");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE5
	mov	edx,offset FLAT:@CBE4
	mov	eax,01h
	call	Verbose

; 137 	    rc = -1ul;				/* anything != 0 */
	mov	dword ptr [ebp-014h],0ffffffffh;	rc

; 136 	    Verbose(1, "VORDev", "missing PHYSDEV mark");
	add	esp,0ch

; 138 	    break;
	jmp	@BLBL5
@BLBL6:

; 140 	if( crc != sec->crc )
	mov	ebx,[ebp-010h];	sec
	mov	ax,[ebp-016h];	crc
	mov	bx,[ebx+01feh]
	cmp	bx,ax
	je	@BLBL7

; 144 	    rc = -2ul;				/* anything != 0 */
	mov	dword ptr [ebp-014h],0fffffffeh;	rc

; 142 	    Verbose(1, "VORDev", "CRC error in PHYSDEV sector (%#x, %#x)",
	movzx	ebx,bx
	push	ebx
	movzx	eax,ax
	push	eax
	mov	ecx,offset FLAT:@CBE6
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE4
	mov	eax,01h
	call	Verbose
	add	esp,014h

; 145 	    break;
	jmp	@BLBL5
@BLBL7:

; 147 	memcpy(id, sec->u.s.id, sizeof(DEVID));
	mov	edi,[ebp+08h];	this
	sub	esp,0ch
	mov	ebx,[ebp-010h];	sec
	mov	ecx,06h
	lea	edx,[ebx+014h]
	lea	eax,[edi+08h]
	call	memcpy

; 148 	size = sec->u.s.size;
	mov	ecx,[ebx+01ch]

; 147 	memcpy(id, sec->u.s.id, sizeof(DEVID));
	add	esp,0ch

; 148 	size = sec->u.s.size;
	mov	[edi+01ch],ecx

; 150     while(0);
@BLBL5:

; 151     delete sec;
	sub	esp,04h
	mov	eax,[ebp-010h];	sec
	call	__dl__FPv
	add	esp,04h

; 153     if( rc != 0 )
	cmp	dword ptr [ebp-014h],0h;	rc
	je	@BLBL10

; 160 	memcpy(&id[0], &usHostId, sizeof(usHostId));
	mov	ebx,[ebp+08h];	this
	sub	esp,0ch
	mov	ecx,02h
	mov	edx,offset FLAT:usHostId
	lea	eax,[ebx+08h]
	call	memcpy
	add	esp,08h

; 161 	*(PULONG)&id[2] = time(NULL);
	xor	eax,eax
	call	time
	mov	[ebx+0ah],eax

; 163 	size = psize - VRAID_ADMINSECTORS;
	mov	ecx,[ebp+014h];	psize
	sub	ecx,020h

; 161 	*(PULONG)&id[2] = time(NULL);
	add	esp,04h

; 163 	size = psize - VRAID_ADMINSECTORS;
	mov	[ebx+01ch],ecx

; 164     }
@BLBL10:

; 166     inv_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_INVCHILDPDEV);
	push	0198h
	mov	esi,[ebp+08h];	this
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	[esi+024h],eax

; 167     rw_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_PDEVDRIVE);
	push	0193h
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	[esi+028h],eax

; 168     ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDPDEV);
	push	0196h
	push	0h
	push	01h
	call	WinLoadPointer
	add	esp,0ch
	mov	ecx,eax
	mov	eax,esi

; 169 }
	pop	esi

; 168     ro_icon = WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ICO_DISABLEDPDEV);
	mov	[eax+02ch],ecx

; 169 }
	pop	edi
	pop	ebx
	add	esp,024h
	pop	fs:[0h]
	leave	
	ret	
__ct__6VORDevFP8VOS2DiskUlT2	endp
CODE32	ends
end
