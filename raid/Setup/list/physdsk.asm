	title	../../../Lib/physdsk.c
	.386
	.387
	includelib cppom30.lib
	includelib os2386.lib
CODE32	segment dword use32 public 'CODE'
CODE32	ends
DATA32	segment dword use32 public 'DATA'
DATA32	ends
CONST32_RO	segment dword use32 public 'CONST'
CONST32_RO	ends
BSS32	segment dword use32 public 'BSS'
BSS32	ends
DGROUP	group BSS32, DATA32
	assume	cs:FLAT, ds:FLAT, ss:FLAT, es:FLAT
	extrn	memset:proc
	extrn	DosPhysicalDisk:proc
	extrn	_sprintfieee:proc
	extrn	strlen:proc
	extrn	DosDevIOCtl:proc
	extrn	memcpy:proc
	extrn	malloc:proc
	extrn	free:proc
DATA32	segment
@STAT1	db "%lu:",0h
	align 04h
@2DevTab	db 0h,0h
	db 012h DUP (0h)
	db 026ch DUP (0h)
DATA32	ends
CONST32_RO	segment
@1id	db "$Id: physdsk.c,v 1.9 199"
db "9/11/11 21:49:41 vitus E"
db "xp $",0h
CONST32_RO	ends
CODE32	segment

; 76 {

Allocate	proc

; 79     for( i = 0; i < MAX_DEVICES; ++i )
	xor	edx,edx
	xor	ecx,ecx
@BLBL2:

; 80 	if( !DevTab[i].open )
	lea	eax,dword ptr [ecx+ @2DevTab]
	cmp	word ptr [eax],0h;	@2DevTab
	jne	@BLBL3

; 81 	{
; 82 	    DevTab[i].open = 1;
	mov	word ptr [eax],01h

; 83 	    return &DevTab[i];
	ret	
@BLBL3:

; 79     for( i = 0; i < MAX_DEVICES; ++i )
	inc	edx
	add	ecx,014h
	cmp	edx,020h
	jl	@BLBL2

; 85     return NULL;
	xor	eax,eax
	ret	
Allocate	endp

; 382 {

	public PDskWrite
PDskWrite	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,ecx
	push	edi
	mov	[ebp+010h],bx;	seccnt
	push	esi

; 386     ULONG	ulParmLen = sizeof(TRACKLAYOUT) + (seccnt - 1) * 4;
	movzx	ebx,bx

; 382 {
	sub	esp,018h

; 385     PDEVTAB	dt = (PDEVTAB
; 385 )handle;
	mov	[ebp-018h],eax;	dt

; 386     ULONG	ulParmLen = sizeof(TRACKLAYOUT) + (seccnt - 1) * 4;
	lea	eax,dword ptr [ebx*04h]
	sub	eax,04h
	add	eax,0dh

; 382 {
	mov	[ebp+0ch],edx;	secno

; 386     ULONG	ulParmLen = sizeof(TRACKLAYOUT) + (seccnt - 1) * 4;
	mov	[ebp-010h],eax;	ulParmLen

; 387     PTRACKLAYOUT	tl = malloc(ulParmLen);
	call	malloc

; 388     ULONG	ulDataLen = seccnt * SECTORSIZE;
	sal	ebx,09h

; 387     PTRACKLAYOUT	tl = malloc(ulParmLen);
	mov	ecx,ebx
	mov	[ebp-01ch],eax;	tl

; 388     ULONG	ulDataLen = seccnt * SECTORSIZE;
	mov	[ebp-014h],ecx;	ulDataLen

; 391     if( tl == NULL )
	test	eax,eax
	jne	@BLBL14

; 392 	return ERROR_NOT_ENOUGH_MEMORY;
	add	esp,018h
	mov	eax,08h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL14:

; 394     tl->bCommand = 0;
	mov	esi,[ebp-01ch];	tl

; 395     sector = secno % dt->layout.cSectorsPerTrack + 1;
	xor	edx,edx

; 394     tl->bCommand = 0;
	mov	byte ptr [esi],0h

; 395     sector = secno % dt->layout.cSectorsPerTrack + 1;
	mov	ebx,[ebp-018h];	dt
	mov	edi,[ebp+0ch];	secno
	mov	eax,edi
	movzx	ecx,word ptr [ebx+0ah]
	div	ecx
	inc	edx
	mov	[ebp-01eh],dx;	sector

; 396     tl->usHead = (secno / dt->layout.cSectorsPerTrack) % dt->layout.cHeads;
	xor	edx,edx
	movzx	ecx,word ptr [ebx+08h]
	div	ecx
	mov	eax,edi
	mov	[esi+01h],dx

; 397     tl->usCylinder = secno / (dt->layout.cSectorsPerTrack
	xor	edx,edx
	movzx	ecx,word ptr [ebx+08h]
	movzx	ebx,word ptr [ebx+0ah]
	imul	ebx,ecx
	div	ebx

; 399     tl->usFirstSector = 0;			/* index in track table? */
	mov	word ptr [esi+05h],0h

; 397     tl->usCylinder = secno / (dt->layout.cSectorsPerTrack
	mov	[esi+03h],ax

; 400     tl->cSectors = seccnt;
	mov	cx,[ebp+010h];	seccnt
	mov	[esi+07h],cx

; 401     for( i = 0; i < seccnt; ++i )
	test	cx,cx
	jbe	@BLBL15
	xor	edi,edi
	mov	bx,[ebp-01eh];	sector
@BLBL16:

; 403 	tl->TrackTable[i].usSectorNumber = sector + i;
	movzx	eax,di
	lea	eax,dword ptr [esi+eax*04h]
	lea	dx,word ptr [edi+ebx*01h]

; 401     for( i = 0; i < seccnt; ++i )
	inc	di

; 403 	tl->TrackTable[i].usSectorNumber = sector + i;
	mov	[eax+09h],dx

; 404 	tl->TrackTable[i].usSectorSize = SECTORSIZE;
	mov	word ptr [eax+0bh],0200h

; 401     for( i = 0; i < seccnt; ++i )
	cmp	di,cx
	jb	@BLBL16
@BLBL15:

; 406     rc = DosDevIOCtl(dt->hd, IOCTL_PHYSICALDISK, PDSK_WRITEPHYSTRACK,
	lea	ecx,[ebp-014h];	ulDataLen
	push	ecx
	lea	edx,[ebp-010h];	ulParmLen
	push	dword ptr [ebp-014h];	ulDataLen
	mov	esi,[ebp-01ch];	tl
	push	dword ptr [ebp+014h];	data
	mov	ecx,[ebp-018h];	dt
	push	edx
	push	dword ptr [ebp-010h];	ulParmLen
	movzx	ecx,word ptr [ecx+02h]
	push	esi
	push	044h
	push	09h
	push	ecx
	call	DosDevIOCtl
	xchg	esi,eax

; 409     free(tl);
	call	free

; 410     return rc;
	add	esp,03ch

; 409     free(tl);
	mov	eax,esi

; 410     return rc;
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
PDskWrite	endp

; 326 {

	public PDskRead
PDskRead	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,ecx
	push	edi
	mov	[ebp+010h],bx;	seccnt
	push	esi

; 330     ULONG	ulParmLen = sizeof(TRACKLAYOUT) + (seccnt - 1) * 4;
	movzx	ebx,bx

; 326 {
	sub	esp,018h

; 329     PDEVTAB	dt = (PDEVTAB)handle;
	mov	[ebp-018h],eax;	dt

; 330     ULONG	ulParmLen = sizeof(TRACKLAYOUT) + (seccnt - 1) * 4;
	lea	eax,dword ptr [ebx*04h]
	sub	eax,04h
	add	eax,0dh

; 326 {
	mov	[ebp+0ch],edx;	secno

; 330     ULONG	ulParmLen = sizeof(TRACKLAYOUT) + (seccnt - 1) * 4;
	mov	[ebp-010h],eax;	ulParmLen

; 331     PTRACKLAYOUT	tl = malloc(ulParmLen);
	call	malloc

; 332     ULONG	ulDataLen = seccnt * SECTORSIZE;
	sal	ebx,09h

; 331     PTRACKLAYOUT	tl = malloc(ulParmLen);
	mov	ecx,ebx
	mov	[ebp-01ch],eax;	tl

; 332     ULONG	ulDataLen = seccnt * SECTORSIZE;
	mov	[ebp-014h],ecx;	ulDataLen

; 335     if( tl == NULL )
	test	eax,eax
	jne	@BLBL10

; 336 	return ERROR_NOT_ENOUGH_MEMORY;
	add	esp,018h
	mov	eax,08h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL10:

; 338     tl->bCommand = 0;
	mov	esi,[ebp-01ch];	tl

; 339     sector = secno % dt->layout.cSectorsPerTrack + 1;
	xor	edx,edx

; 338     tl->bCommand = 0;
	mov	byte ptr [esi],0h

; 339     sector = secno % dt->layout.cSectorsPerTrack + 1;
	mov	ebx,[ebp-018h];	dt
	mov	edi,[ebp+0ch];	secno
	mov	eax,edi
	movzx	ecx,word ptr [ebx+0ah]
	div	ecx
	inc	edx
	mov	[ebp-01eh],dx;	sector

; 340     tl->usHead = (secno / dt->layout.cSectorsPerTrack) % dt->layout.cHeads;
	xor	edx,edx
	movzx	ecx,word ptr [ebx+08h]
	div	ecx
	mov	eax,edi
	mov	[esi+01h],dx

; 341     tl->usCylinder = secno / (dt->layout.cSectorsPerTrack
	xor	edx,edx
	movzx	ecx,word ptr [ebx+08h]
	movzx	ebx,word ptr [ebx+0ah]
	imul	ebx,ecx
	div	ebx

; 343     tl->usFirstSector = 0;			/* index in track table? */
	mov	word ptr [esi+05h],0h

; 341     tl->usCylinder = secno / (dt->layout.cSectorsPerTrack
	mov	[esi+03h],ax

; 344     tl->cSectors = seccnt;
	mov	cx,[ebp+010h];	seccnt
	mov	[esi+07h],cx

; 345     for( i = 0; i < seccnt; ++i )
	test	cx,cx
	jbe	@BLBL11
	xor	edi,edi
	mov	bx,[ebp-01eh];	sector
@BLBL12:

; 347 	tl->TrackTable[i].usSectorNumber = sector + i;
	movzx	eax,di
	lea	eax,dword ptr [esi+eax*04h]
	lea	dx,word ptr [edi+ebx*01h]

; 345     for( i = 0; i < seccnt; ++i )
	inc	di

; 347 	tl->TrackTable[i].usSectorNumber = sector + i;
	mov	[eax+09h],dx

; 348 	tl->TrackTable[i].usSectorSize = SECTORSIZE;
	mov	word ptr [eax+0bh],0200h

; 345     for( i = 0; i < seccnt; ++i )
	cmp	di,cx
	jb	@BLBL12
@BLBL11:

; 350     rc = DosDevIOCtl(dt->hd, IOCTL_PHYSICALDISK, PDSK_READPHYSTRACK,
	lea	ecx,[ebp-014h];	ulDataLen
	push	ecx
	lea	edx,[ebp-010h];	ulParmLen
	push	dword ptr [ebp-014h];	ulDataLen
	mov	esi,[ebp-01ch];	tl
	push	dword ptr [ebp+014h];	data
	mov	ecx,[ebp-018h];	dt
	push	edx
	push	dword ptr [ebp-010h];	ulParmLen
	movzx	ecx,word ptr [ecx+02h]
	push	esi
	push	064h
	push	09h
	push	ecx
	call	DosDevIOCtl
	xchg	esi,eax

; 354     free(tl);
	call	free

; 355     return rc;
	add	esp,03ch

; 354     free(tl);
	mov	eax,esi

; 355     return rc;
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
PDskRead	endp

; 287 {

	public PDskUnlockIO
PDskUnlockIO	proc
	push	ebp

; 295     memset(aucData, 0, sizeof(aucData));
	mov	ecx,01fh

; 287 {
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi

; 295     memset(aucData, 0, sizeof(aucData));
	xor	edx,edx

; 287 {
	sub	esp,034h

; 295     memset(aucData, 0, sizeof(aucData));
	lea	edi,[ebp-030h];	aucData

; 290     UCHAR	ucParm = 0;
	mov	byte ptr [ebp-011h],0h;	ucParm

; 291     ULONG	ulParmLen = sizeof(ucParm);
	mov	dword ptr [ebp-0ch],01h;	ulParmLen

; 295     memset(aucData, 0, sizeof(aucData));
	mov	eax,edi

; 293     ULONG	ulDataLen = sizeof(aucData);
	mov	dword ptr [ebp-010h],01fh;	ulDataLen

; 295     memset(aucData, 0, sizeof(aucData));
	call	memset

; 296     rc = DosDevIOCtl(dt->hd, IOCTL_PHYSICALDISK, PDSK_UNLOCKPHYSDRIVE,
	lea	ecx,[ebp-010h];	ulDataLen
	push	ecx
	lea	ecx,[ebp-0ch];	ulParmLen
	push	dword ptr [ebp-010h];	ulDataLen
	lea	edx,[ebp-011h];	ucParm
	push	edi

; 295     memset(aucData, 0, sizeof(aucData));
	mov	eax,ebx

; 296     rc = DosDevIOCtl(dt->hd, IOCTL_PHYSICALDISK, PDSK_UNLOCKPHYSDRIVE,
	push	ecx
	push	dword ptr [ebp-0ch];	ulParmLen
	movzx	ecx,word ptr [eax+02h]
	push	edx
	push	01h
	push	09h
	push	ecx
	call	DosDevIOCtl

; 299     return rc;
	add	esp,058h
	pop	edi
	pop	ebx
	pop	ebp
	ret	
PDskUnlockIO	endp

; 253 {

	public PDskLockIO
PDskLockIO	proc
	push	ebp

; 261     memset(aucData, 0, sizeof(aucData));
	mov	ecx,01fh

; 253 {
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi

; 261     memset(aucData, 0, sizeof(aucData));
	xor	edx,edx

; 253 {
	sub	esp,034h

; 261     memset(aucData, 0, sizeof(aucData));
	lea	edi,[ebp-030h];	aucData

; 256     UCHAR	ucParm = 0;
	mov	byte ptr [ebp-011h],0h;	ucParm

; 257     ULONG	ulParmLen = sizeof(ucParm);
	mov	dword ptr [ebp-0ch],01h;	ulParmLen

; 261     memset(aucData, 0, sizeof(aucData));
	mov	eax,edi

; 259     ULONG	ulDataLen = sizeof(aucData);
	mov	dword ptr [ebp-010h],01fh;	ulDataLen

; 261     memset(aucData, 0, sizeof(aucData));
	call	memset

; 262     rc = DosDevIOCtl(dt->hd, IOCTL_PHYSICALDISK, PDSK_LOCKPHYSDRIVE,
	lea	ecx,[ebp-010h];	ulDataLen
	push	ecx
	lea	ecx,[ebp-0ch];	ulParmLen
	push	dword ptr [ebp-010h];	ulDataLen
	lea	edx,[ebp-011h];	ucParm
	push	edi

; 261     memset(aucData, 0, sizeof(aucData));
	mov	eax,ebx

; 262     rc = DosDevIOCtl(dt->hd, IOCTL_PHYSICALDISK, PDSK_LOCKPHYSDRIVE,
	push	ecx
	push	dword ptr [ebp-0ch];	ulParmLen
	movzx	ecx,word ptr [eax+02h]
	push	edx
	push	0h
	push	09h
	push	ecx
	call	DosDevIOCtl

; 265     return rc;
	add	esp,058h
	pop	edi
	pop	ebx
	pop	ebp
	ret	
PDskLockIO	endp

; 226 {

	public PDskQueryParam
PDskQueryParam	proc
	xchg	edx,eax
	sub	esp,0ch

; 229     memcpy(dp, &dt->layout, sizeof(dt->layout));
	add	edx,04h
	mov	ecx,010h
	call	memcpy

; 230     return 0;
	add	esp,0ch
	xor	eax,eax
	ret	
PDskQueryParam	endp

; 194 {

	public PDskClose
PDskClose	proc
	push	ebx
	mov	ebx,eax
	sub	esp,04h

; 198     rc = DosPhysicalDisk(INFO_FREEIOCTLHANDLE,
	lea	ecx,[ebx+02h]
	push	02h
	push	ecx
	push	0h
	push	0h
	push	03h
	call	DosPhysicalDisk
	xchg	ebx,eax

; 201     Free(dt);
	call	Free

; 202     return rc;
	add	esp,018h

; 201     Free(dt);
	mov	eax,ebx

; 202     return rc;
	pop	ebx
	ret	
PDskClose	endp

; 146 {

	public PDskOpen
PDskOpen	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	[ebp+0ch],edx;	handle
	sub	esp,02ch
	mov	[ebp+08h],eax;	idx

; 149     PDEVTAB	dt = Allocate();
	call	Allocate
	mov	[ebp-08h],eax;	dt

; 151     if( dt == NULL )
	test	eax,eax
	jne	@BLBL6

; 152 	return ERROR_TOO_MANY_OPEN_FILES;
	add	esp,02ch
	mov	eax,04h
	pop	ebx
	pop	ebp
	ret	
@BLBL6:

; 154     sprintf(str, "%lu:", idx+1);
	mov	ecx,[ebp+08h];	idx
	inc	ecx
	push	ecx
	lea	ebx,[ebp-02ch];	str
	sub	esp,08h
	mov	edx,offset FLAT:@STAT1
	mov	eax,ebx
	call	_sprintfieee

; 155     rc = DosPhysicalDisk(INFO_GETIOCTLHANDLE,
	mov	eax,ebx
	call	strlen
	inc	eax
	push	eax
	mov	ecx,ebx
	push	ecx
	mov	ebx,[ebp-08h];	dt
	push	02h
	lea	ecx,[ebx+02h]
	push	ecx
	push	02h
	call	DosPhysicalDisk
	add	esp,020h
	mov	[ebp-0ch],eax;	rc

; 158     if( rc == 0 )
	test	eax,eax
	jne	@BLBL7

; 164 	rc = DosDevIOCtl(dt->hd, IOCTL_PHYSICALDISK, PDSK_GETPHYSDEVICEPARAMS,
	lea	ecx,[ebp-014h];	ulDataLen
	push	ecx
	lea	ecx,[ebx+04h]
	push	010h
	lea	edx,[ebp-015h];	ucParm
	push	ecx
	lea	ecx,[ebp-010h];	ulParmLen
	push	ecx

; 160 	UCHAR	ucParm = 0;
	mov	byte ptr [ebp-015h],0h;	ucParm

; 164 	rc = DosDevIOCtl(dt->hd, IOCTL_PHYSICALDISK, PDSK_GETPHYSDEVICEPARAMS,
	push	01h

; 161 	ULONG	ulParmLen = sizeof(ucParm);
	mov	dword ptr [ebp-010h],01h;	ulParmLen

; 164 	rc = DosDevIOCtl(dt->hd, IOCTL_PHYSICALDISK, PDSK_GETPHYSDEVICEPARAMS,
	push	edx

; 162 	ULONG	ulDataLen = sizeof(dt->layout);
	mov	dword ptr [ebp-014h],010h;	ulDataLen

; 164 	rc = DosDevIOCtl(dt->hd, IOCTL_PHYSICALDISK, PDSK_GETPHYSDEVICEPARAMS,
	push	063h
	movzx	ecx,word ptr [ebx+02h]
	push	09h
	push	ecx
	call	DosDevIOCtl
	add	esp,024h
	mov	[ebp-0ch],eax;	rc

; 167 	if( rc != 0 )
	test	eax,eax
	mov	eax,ebx
	je	@BLBL8

; 168 	    PDskClose((ULONG)dt);
	call	PDskClose
	jmp	@BLBL7
@BLBL8:

; 170 	    *handle = (ULONG)dt;
	mov	eax,[ebp+0ch];	handle
	mov	ebx,[ebp-08h];	dt
	mov	[eax],ebx
@BLBL7:

; 172     return rc;
	mov	eax,[ebp-0ch];	rc
	add	esp,02ch
	pop	ebx
	pop	ebp
	ret	
PDskOpen	endp

; 114 {

	public PDskEnum
PDskEnum	proc
	sub	esp,08h
	push	ebx
	mov	ebx,eax
	push	edi

; 118     rc = DosPhysicalDisk(INFO_COUNT_PARTITIONABLE_DISKS,
	push	0h
	lea	ecx,[esp+0eh];	us
	push	0h
	push	02h
	push	ecx
	push	01h
	call	DosPhysicalDisk
	add	esp,014h
	mov	edi,eax

; 121     if( rc == 0 )
	test	eax,eax
	mov	eax,ebx
	jne	@BLBL5

; 122 	*count = us;
	movzx	ecx,word ptr [esp+0ah];	us
	mov	[eax],ecx
@BLBL5:

; 123     return rc;
	mov	eax,edi
	pop	edi
	pop	ebx
	add	esp,08h
	ret	
PDskEnum	endp

; 90 {

Free	proc

; 92     memset(dt, 0, sizeof(*dt));
	xor	edx,edx

; 90 {
	sub	esp,0ch

; 92     memset(dt, 0, sizeof(*dt));
	mov	ecx,014h
	call	memset

; 93 }
	add	esp,0ch
	ret	
Free	endp
CODE32	ends
end
