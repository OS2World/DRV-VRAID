	title	PartTable.cpp
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
	extrn	Verbose:proc
	extrn	queryIndex__8VOS2DiskFv:proc
	extrn	__dl__FPv:proc
	extrn	memset:proc
	extrn	queryDPB__8VOS2DiskFv:proc
CONST32_RO	segment
@CBE1	db "PartTable",0h
	align 04h
@CBE2	db "disk %lu, sector %lu",0h
	align 04h
@CBE3	db "can't read disk %lu, sec"
db "tor %lu - rc %lu",0h
	align 04h
@CBE4	db "clearing invalid partiti"
db "on table (disk %lu, sect"
db "or %lu)",0h
@CBE5	db "can't write disk %lu, se"
db "ctor %lu - rc %lu",0h
	align 04h
@CBE6	db "delPE(disk %lu, table %l"
db "u, type %#x, rel %lu, si"
db "ze %lu)",0h
@CBE7	db "addPE(disk %lu, table %l"
db "u, type %#x, rel %lu, si"
db "ze %lu)",0h
@CBE8	db "addPE(disk %lu, table %l"
db "u) - no free entry",0h
	align 04h
@CBE9	db "addForeign(disk %lu, tab"
db "le %lu, type %#x, start "
db "%lu, size %lu)",0h
	align 04h
@CBE10	db "addOwn(disk %lu, table %"
db "lu, type %#x, start %lu,"
db " size %lu)",0h
	align 04h
@CBE11	db "size adjusted to %lu",0h
	align 04h
@1vcid	db "$Id: parttable.cpp,v 1.4"
db " 2000/04/10 01:33:40 vit"
db "us Exp $",0h
CONST32_RO	ends
CODE32	segment

; 440 PartTable::addOwn(UCHAR type,UCHAR bootable,ULONG start,ULONG size)

	public addOwn__9PartTableFUcT1UlT3
addOwn__9PartTableFUcT1UlT3	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi
	mov	[ebp+0ch],dl;	type
	push	esi
	movzx	edx,dl
	sub	esp,018h
	mov	[ebp+010h],cl;	bootable
	mov	[ebp-020h],edx;	@CBE14
	mov	[ebp+08h],ebx;	this

; 442     Verbose(2,"PartTable","addOwn(disk %lu, table %lu, type %#x,"
	mov	eax,[ebx]
	call	queryIndex__8VOS2DiskFv
	push	dword ptr [ebp+018h];	size
	mov	edx,[ebp-020h];	@CBE14
	push	dword ptr [ebp+014h];	start
	mov	edi,eax
	mov	eax,edx
	push	eax
	mov	ecx,offset FLAT:@CBE10
	push	dword ptr [ebx+04h]
	mov	edx,offset FLAT:@CBE1
	push	edi
	mov	eax,02h
	sub	esp,0ch
	call	Verbose
	mov	eax,ebx
	add	esp,020h

; 443 	    " start %lu, size %lu)",
; 444 	    disk->queryIndex(), secno, type, start, size);
; 445 
; 446     if( rderr != 0 )
	cmp	dword ptr [eax+0214h],0h
	je	@BLBL54

; 447 	return FALSE;
	xor	eax,eax
	add	esp,018h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL54:

; 448 
; 449     /* Normalize ending point to last sector of cylinder!
; 450      * .start is already correct. xxx */
; 451 
; 452     DEVICEPARAMETERBLOCK dpb = disk->queryDPB();
	mov	eax,[ebp+08h];	this
	lea	edi,[ebp-01ch];	dpb
	mov	eax,[eax]
	call	queryDPB__8VOS2DiskFv
	mov	esi,eax
	mov	ecx,04h

; 453     ULONG const		cylsize = dpb.cHeads * dpb.cSectorsPerTrack;
; 454     ULONG		end = size + start;
; 455 
; 456     if( (end % cylsize) != 0 )
	xor	edx,edx

; 452     DEVICEPARAMETERBLOCK dpb = disk->queryDPB();
	rep movsd	

; 456     if( (end % cylsize) != 0 )
	movzx	ebx,word ptr [ebp-016h];	dpb
	movzx	ecx,word ptr [ebp-018h];	dpb
	imul	ecx,ebx
	mov	ebx,[ebp+014h];	start
	mov	eax,[ebp+018h];	size
	add	eax,ebx
	div	ecx
	test	edx,edx
	je	@BLBL55

; 460 	Verbose(2, "PartTable", "size adjusted to %lu", end - start);
	imul	eax,ecx
	sub	eax,ebx
	push	eax

; 461 	size = end - start;
	mov	[ebp+018h],eax;	size

; 460 	Verbose(2, "PartTable", "size adjusted to %lu", end - start);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE11
	mov	edx,offset FLAT:@CBE1
	mov	eax,02h
	call	Verbose
	add	esp,010h

; 462     }
@BLBL55:

; 463     return addForeign(type, bootable, start, size);
	mov	cl,[ebp+010h];	bootable
	add	esp,018h
	mov	dl,[ebp+0ch];	type
	pop	esi
	mov	eax,[ebp+08h];	this
	pop	edi
	pop	ebx
	pop	ebp
	jmp	addForeign__9PartTableFUcT1UlT3
addOwn__9PartTableFUcT1UlT3	endp

; 355 PartTable::addForeign(UCHAR type,UCHAR bootable,ULONG start,ULONG size)

	public addForeign__9PartTableFUcT1UlT3
addForeign__9PartTableFUcT1UlT3	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi
	mov	[ebp+0ch],dl;	type
	push	esi
	movzx	edx,dl
	sub	esp,038h
	mov	[ebp+010h],cl;	bootable
	mov	[ebp-03ch],edx;	@CBE15
	mov	[ebp+08h],ebx;	this

; 357     Verbose(2,"PartTable","addForeign(disk %lu, table %lu, type %#x,"
	mov	eax,[ebx]
	call	queryIndex__8VOS2DiskFv
	push	dword ptr [ebp+018h];	size
	mov	edx,[ebp-03ch];	@CBE15
	push	dword ptr [ebp+014h];	start
	mov	edi,eax
	mov	eax,edx
	push	eax
	mov	ecx,offset FLAT:@CBE9
	push	dword ptr [ebx+04h]
	mov	edx,offset FLAT:@CBE1
	push	edi
	mov	eax,02h
	sub	esp,0ch
	call	Verbose
	mov	eax,ebx
	add	esp,020h

; 361     if( rderr != 0 )
	cmp	dword ptr [eax+0214h],0h
	je	@BLBL45

; 362 	return FALSE;
	xor	eax,eax
	add	esp,038h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL45:

; 364     DEVICEPARAMETERBLOCK dpb = disk->queryDPB();
	mov	eax,[ebp+08h];	this
	lea	edi,[ebp-038h];	dpb
	mov	eax,[eax]
	call	queryDPB__8VOS2DiskFv
	mov	esi,eax
	mov	ecx,04h
	rep movsd	

; 373     pe.BootIndicator = bootable;
	mov	dl,[ebp+010h];	bootable
	mov	[ebp-028h],dl;	pe

; 375     cyl = (USHORT)(start / cylsize);
	mov	edx,[ebp+014h];	start
	mov	eax,edx
	xor	edx,edx

; 365     USHORT const	spt = dpb.cSectorsPerTrack;
	mov	si,[ebp-032h];	dpb

; 366     ULONG const		cylsize = dpb.cHeads * spt;
	mov	di,[ebp-034h];	dpb
	movzx	ebx,di
	movzx	ecx,si
	imul	ecx,ebx

; 375     cyl = (USHORT)(start / cylsize);
	div	ecx

; 366     ULONG const		cylsize = dpb.cHeads * spt;
	mov	[ebp-010h],ecx;	cylsize

; 375     cyl = (USHORT)(start / cylsize);
	mov	ecx,[ebp+014h];	start
	xchg	ecx,eax
	xor	edx,edx

; 365     USHORT const	spt = dpb.cSectorsPerTrack;
	mov	[ebp-014h],si;	spt

; 376     sc = (start % spt) + 1;
	movzx	esi,si
	div	esi

; 377     hd = (start / spt) % dpb.cHeads;
	movzx	edi,di

; 376     sc = (start % spt) + 1;
	inc	edx
	mov	[ebp-015h],dl;	sc
	xor	edx,edx

; 375     cyl = (USHORT)(start / cylsize);
	mov	[ebp-012h],cx;	cyl

; 377     hd = (start / spt) % dpb.cHeads;
	div	edi
	mov	[ebp-016h],dl;	hd

; 378     if( cyl >= 1024 )
	cmp	cx,0400h
	jb	@BLBL46

; 380 	pe.BegHead = dpb.cHeads - 1;
	dec	ebx
	mov	[ebp-027h],bl;	pe

; 381 	pe.BegSector = 0xE0;
	mov	byte ptr [ebp-026h],0e0h;	pe

; 382 	pe.BegCylinder = 0xFF;			/* Cylinder 1023, Sector 0 */
	mov	byte ptr [ebp-025h],0ffh;	pe

; 383     }
	jmp	@BLBL47
@BLBL46:

; 386 	pe.BegHead = hd;
	mov	al,[ebp-016h];	hd

; 387 	pe.BegSector = (UCH
; 387 AR)((HIUCHAR(cyl) << 6) | sc);
	mov	bx,[ebp-012h];	cyl

; 386 	pe.BegHead = hd;
	mov	[ebp-027h],al;	pe

; 387 	pe.BegSector = (UCH
; 387 AR)((HIUCHAR(cyl) << 6) | sc);
	movzx	eax,bx
	sar	eax,08h
	and	eax,0ffh
	sal	al,06h
	or	al,[ebp-015h];	sc

; 388 	pe.BegCylinder = LOUCHAR(cyl);
	mov	[ebp-025h],bl;	pe

; 387 	pe.BegSector = (UCH
; 387 AR)((HIUCHAR(cyl) << 6) | sc);
	mov	[ebp-026h],al;	pe

; 389     }
@BLBL47:

; 394     cyl = (USHORT)(last / cylsize);
	mov	ecx,[ebp+018h];	size
	add	ecx,[ebp+014h];	start
	dec	ecx
	mov	eax,ecx
	xor	edx,edx

; 391     pe.SysIndicator = type;
	mov	bl,[ebp+0ch];	type

; 394     cyl = (USHORT)(last / cylsize);
	div	dword ptr [ebp-010h];	cylsize

; 391     pe.SysIndicator = type;
	mov	[ebp-024h],bl;	pe

; 394     cyl = (USHORT)(last / cylsize);
	xchg	ecx,eax

; 395     sc = (last % spt) + 1;
	xor	edx,edx
	movzx	ebx,word ptr [ebp-014h];	spt
	div	ebx

; 396     hd = (last / spt) % dpb.cHeads;
	mov	bx,[ebp-034h];	dpb

; 395     sc = (last % spt) + 1;
	inc	edx
	mov	[ebp-015h],dl;	sc

; 396     hd = (last / spt) % dpb.cHeads;
	xor	edx,edx
	movzx	edi,bx
	div	edi

; 394     cyl = (USHORT)(last / cylsize);
	mov	[ebp-012h],cx;	cyl

; 396     hd = (last / spt) % dpb.cHeads;
	mov	[ebp-016h],dl;	hd

; 397     if( cyl >= 1024 )
	cmp	cx,0400h
	jb	@BLBL48

; 400 	pe.EndSector = 0xE0;
	mov	byte ptr [ebp-022h],0e0h;	pe

; 399 	pe.EndHead = dpb.cHeads - 1;
	movzx	ebx,bx
	dec	ebx
	mov	[ebp-023h],bl;	pe

; 401 	pe.EndCylinder = 0xFF;			/* Cylinder 1023, Sector 0 */
	mov	byte ptr [ebp-021h],0ffh;	pe

; 402     }
	jmp	@BLBL49
@BLBL48:

; 405 	pe.EndHead = hd;
	mov	al,[ebp-016h];	hd

; 406 	pe.EndSector = (UCHAR)((HIUCHAR(cyl) << 6) | sc);
	mov	bx,[ebp-012h];	cyl

; 405 	pe.EndHead = hd;
	mov	[ebp-023h],al;	pe

; 406 	pe.EndSector = (UCHAR)((HIUCHAR(cyl) << 6) | sc);
	movzx	eax,bx
	sar	eax,08h
	and	eax,0ffh
	sal	al,06h
	or	al,[ebp-015h];	sc

; 407 	pe.EndCylinder = LOUCHAR(cyl);
	mov	[ebp-021h],bl;	pe

; 406 	pe.EndSector = (UCHAR)((HIUCHAR(cyl) << 6) | sc);
	mov	[ebp-022h],al;	pe

; 408     }
@BLBL49:

; 409     if( type == EXTENDED_PARTTYPE )
	cmp	byte ptr [ebp+0ch],05h;	type
	jne	@BLBL50

; 410 	pe.RelativeSectors = start - extstart;	/* extended refer to the first */
	mov	ebx,[ebp+08h];	this
	mov	eax,[ebp+014h];	start
	sub	eax,[ebx+08h]
	mov	[ebp-020h],eax;	pe
	jmp	@BLBL51
@BLBL50:

; 412 	pe.RelativeSectors = start - secno;
	mov	ebx,[ebp+08h];	this
	mov	eax,[ebp+014h];	start
	sub	eax,[ebx+04h]
	mov	[ebp-020h],eax;	pe
@BLBL51:

; 413     pe.NumSectors = size;
	mov	ebx,[ebp+018h];	size

; 415     return addPE(pe);
	mov	eax,[ebp+08h];	this

; 413     pe.NumSectors = size;
	mov	[ebp-01ch],ebx;	pe

; 415     return addPE(pe);
	lea	edx,[ebp-028h];	pe
	call	addPE__9PartTableFRC15_PARTITIONENTRY
	add	esp,038h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
addForeign__9PartTableFUcT1UlT3	endp

; 306 PartTable::addPE(PARTITIONENTRY const &partentry)

	public addPE__9PartTableFRC15_PARTITIONENTRY
addPE__9PartTableFRC15_PARTITIONENTRY	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi
	mov	edi,edx
	push	esi
	mov	[ebp+0ch],edi;	partentry
	sub	esp,0ch
	mov	[ebp+08h],ebx;	this

; 308     Verbose(2,"PartTable","addPE(disk %lu, table %lu, type %#x, rel %lu, size %lu)",
	mov	eax,[ebx]
	call	queryIndex__8VOS2DiskFv
	push	dword ptr [edi+0ch]
	movzx	edx,byte ptr [edi+04h]
	push	dword ptr [edi+08h]
	mov	edi,eax
	push	edx
	mov	ecx,offset FLAT:@CBE7
	push	dword ptr [ebx+04h]
	mov	edx,offset FLAT:@CBE1
	push	edi
	mov	eax,02h
	sub	esp,0ch
	call	Verbose
	mov	eax,ebx
	add	esp,020h

; 313     if( rderr != 0 )
	cmp	dword ptr [eax+0214h],0h
	je	@BLBL35

; 314 	return FALSE;
	xor	eax,eax
	add	esp,0ch
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL35:

; 317     for( int i = 0; i < 4; ++i )
	mov	esi,[ebp+08h];	this

; 316     int		free = -1;
	mov	dword ptr [ebp-010h],0ffffffffh;	free

; 317     for( int i = 0; i < 4; ++i )
	mov	dword ptr [ebp-014h],0h;	i
	xor	edi,edi
	xor	ecx,ecx
@BLBL37:

; 318 	if( bootrecord.PartitionTable[i].SysIndicator == 0 )
	cmp	byte ptr [esi+ecx+01ceh],0h
	jne	@BLBL40

; 320 	    free = i;
	mov	[ebp-010h],edi;	free

; 321 	    break;
	jmp	@BLBL36
@BLBL40:

; 317     for( int i = 0; i < 4; ++i )
	inc	edi
	add	ecx,010h
	cmp	edi,04h
	jl	@BLBL37
@BLBL36:
	mov	[ebp-014h],edi;	i

; 323     if( free == -1 )
	cmp	dword ptr [ebp-010h],0ffffffffh;	free
	jne	@BLBL42

; 325 	Verbose(1, "PartTable", "addPE(disk %lu, table %lu) - no free entry",
	mov	ebx,[ebp+08h];	this
	mov	eax,[ebx]
	call	queryIndex__8VOS2DiskFv
	push	dword ptr [ebx+04h]
	mov	ecx,offset FLAT:@CBE8
	push	eax
	mov	edx,offset FLAT:@CBE1
	sub	esp,0ch
	mov	eax,01h
	call	Verbose
	add	esp,014h

; 327 	return FALSE;				/* scanning assured a free entry? */
	add	esp,0ch
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL42:

; 330     bootrecord.PartitionTable[i] = partentry;
	mov	edi,[ebp-014h];	i
	mov	ebx,[ebp+08h];	this
	sal	edi,04h
	add	edi,ebx
	mov	esi,[ebp+0ch];	partentry
	add	edi,01cah
	mov	ecx,04h

; 332     return TRUE;
	mov	eax,01h

; 330     bootrecord.PartitionTable[i] = partentry;
	rep movsd	

; 331     modified = TRUE;
	mov	dword ptr [ebx+020ch],01h

; 332     return TRUE;
	add	esp,0ch
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
addPE__9PartTableFRC15_PARTITIONENTRY	endp

; 263 PartTable::delPE(PPARTITIONENTRY partentry)

	public delPE__9PartTableFP15_PARTITIONENTRY
delPE__9PartTableFP15_PARTITIONENTRY	proc
	push	ebx
	mov	ebx,eax
	push	edi
	mov	edi,edx
	push	esi
	sub	esp,010h
	mov	[esp+024h],edi;	partentry
	mov	[esp+020h],ebx;	this

; 265     Verbose(2,"PartTable","delPE(disk %lu, table %lu, type %#x, rel %lu, size %lu)",
	mov	eax,[ebx]
	call	queryIndex__8VOS2DiskFv
	push	dword ptr [edi+0ch]
	movzx	edx,byte ptr [edi+04h]
	push	dword ptr [edi+08h]
	mov	edi,eax
	push	edx
	mov	ecx,offset FLAT:@CBE6
	push	dword ptr [ebx+04h]
	mov	edx,offset FLAT:@CBE1
	push	edi
	mov	eax,02h
	sub	esp,0ch
	call	Verbose
	mov	eax,ebx
	add	esp,020h

; 270     if( rderr != 0 )
	cmp	dword ptr [eax+0214h],0h
	je	@BLBL26

; 271 	return FALSE;
	xor	eax,eax
	add	esp,010h
	pop	esi
	pop	edi
	pop	ebx
	ret	
@BLBL26:

; 274     for( int i = 0; i < 4; ++i )
	xor	esi,esi
	mov	ecx,[esp+024h];	partentry
	mov	ebx,[esp+020h];	this

; 273     BOOL	done = FALSE;
	mov	dword ptr [esp+0ch],0h;	done

; 274     for( int i = 0; i < 4; ++i )
	xor	edi,edi
@BLBL28:

; 276 	if( partentry == &bootrecord.PartitionTable[i] )
	lea	eax,dword ptr [ebx+edi+01cah]
	cmp	eax,ecx
	jne	@BLBL31

; 278 	    memset(&bootrecord.PartitionTable[i], 0, sizeof(*partentry));
	xor	edx,edx
	mov	ecx,010h
	call	memset
	mov	ecx,[esp+024h];	partentry

; 279 	    modified = TRUE;
	mov	dword ptr [ebx+020ch],01h

; 280 	    done = TRUE;
	mov	dword ptr [esp+0ch],01h;	done

; 281 	    break;
	jmp	@BLBL27
@BLBL31:

; 274     for( int i = 0; i < 4; ++i )
	inc	esi
	add	edi,010h
	cmp	esi,04h
	jl	@BLBL28
@BLBL27:

; 284     return done;
	mov	eax,[esp+0ch];	done
	add	esp,010h
	pop	esi
	pop	edi
	pop	ebx
	ret	
delPE__9PartTableFP15_PARTITIONENTRY	endp

; 143 PartTable::clear(void)

	public clear__9PartTableFv
clear__9PartTableFv	proc
	push	ebx
	sub	esp,0ch

; 145     if( rderr != 0 )
	cmp	dword ptr [eax+0214h],0h
	je	@BLBL12

; 146 	return;
	add	esp,0ch
	pop	ebx
	ret	
@BLBL12:
	mov	ebx,eax

; 148     memset(&bootrecord, 0, sizeof(bootrecord));
	mov	ecx,0200h
	lea	eax,[ebx+0ch]
	xor	edx,edx
	call	memset
	mov	eax,ebx

; 149     bootrecord.Signature = 0xAA55;
	mov	word ptr [eax+020ah],0aa55h

; 150     valid = FALSE;				/* only write if modified */
	mov	dword ptr [eax+0210h],0h

; 151 }
	add	esp,0ch
	pop	ebx
	ret	
clear__9PartTableFv	endp

; 221 PartTable::flush()

	public flush__9PartTableFv
flush__9PartTableFv	proc
	push	ebx
	push	edi
	push	esi
	sub	esp,08h

; 223     if( rderr != 0 )
	mov	edx,[eax+0214h]
	test	edx,edx
	je	@BLBL20
	mov	eax,edx

; 224 	return rderr;				/* don't write if not read! */
	add	esp,08h
	pop	esi
	pop	edi
	pop	ebx
	ret	
@BLBL20:

; 225 
; 226     APIRET	rc = 0;
	mov	dword ptr [esp+04h],0h;	rc

; 227     if( modified )
	cmp	dword ptr [eax+020ch],0h
	je	@BLBL21
	mov	edi,eax

; 228     {
; 229 	rc = disk->write(secno, 1, &bootrecord);
	mov	ecx,01h
	mov	eax,[edi]
	lea	ebx,[edi+0ch]
	push	ebx
	mov	ebx,[eax]
	sub	esp,0ch
	mov	esi,[ebx+024h]
	mov	edx,[edi+04h]
	add	eax,esi
	call	dword ptr [ebx+020h]
	add	esp,010h
	xchg	edi,eax
	mov	[esp+04h],edi;	rc

; 230 	if( rc != 0 )
	test	edi,edi
	je	@BLBL22
	mov	ebx,eax

; 231 	{
; 232 	    Verbose(1, "PartTable", "can't write disk %lu, sector %lu - rc %lu",
	mov	eax,[ebx]
	call	queryIndex__8VOS2DiskFv
	push	edi
	mov	ecx,offset FLAT:@CBE5
	push	dword ptr [ebx+04h]
	mov	edx,offset FLAT:@CBE1
	push	eax
	mov	eax,01h
	sub	esp,0ch
	call	Verbose
	mov	eax,ebx
	add	esp,018h

; 233 		    disk->queryIndex(), secno, rc);
; 234 	}
	jmp	@BLBL21
@BLBL22:

; 235 	else
; 236 	{
; 237 	    modified = FALSE;
	mov	dword ptr [eax+020ch],0h

; 238 	    valid = TRUE;
	mov	dword ptr [eax+0210h],01h

; 239 	}
; 240     }
@BLBL21:

; 241     return rc;
	mov	eax,[esp+04h];	rc
	add	esp,08h
	pop	esi
	pop	edi
	pop	ebx
	ret	
flush__9PartTableFv	endp

; 134 PartTable::~PartTable()

	public __dt__9PartTableFv
__dt__9PartTableFv	proc
	push	ebx
	mov	ebx,eax
	sub	esp,08h
	mov	[esp+04h],edx;	@CBE18

; 136     flush();
	call	flush__9PartTableFv
	mov	edx,[esp+04h];	@CBE18
	mov	eax,ebx

; 137 }
	test	dl,01h
	je	@BLBL9
	call	__dl__FPv
	mov	eax,ebx
@BLBL9:
	add	esp,08h
	pop	ebx
	ret	
__dt__9PartTableFv	endp

; 99 PartTable::PartTable(VOS2Disk * d,ULONG n,ULONG first_extended_start)

	public __ct__9PartTableFP8VOS2DiskUlT2
__ct__9PartTableFP8VOS2DiskUlT2	proc
	push	ebx
	mov	ebx,eax
	sub	esp,04h

; 101     disk = d;
	mov	[ebx],edx

; 102     secno = n;
	mov	[ebx+04h],ecx

; 103     rderr = 0;
	mov	dword ptr [ebx+0214h],0h

; 104     modified = FALSE;
	mov	dword ptr [ebx+020ch],0h

; 105     valid = TRUE;
	mov	dword ptr [ebx+0210h],01h

; 106     extstart = first_extended_start;
	mov	eax,[esp+018h];	first_extended_start
	mov	[ebx+08h],eax

; 109     entry = bootrecord.PartitionTable;
	lea	eax,[ebx+01cah]
	mov	[ebx+0218h],eax

; 111     Verbose(2, "PartTable", "disk %lu, sector %lu", disk->queryIndex(), secno);
	mov	eax,[ebx]
	call	queryIndex__8VOS2DiskFv
	push	dword ptr [ebx+04h]
	mov	ecx,offset FLAT:@CBE2
	push	eax
	mov	edx,offset FLAT:@CBE1
	sub	esp,0ch
	mov	eax,02h
	call	Verbose

; 113     readRecord();
	mov	eax,ebx
	call	readRecord__9PartTableFv

; 115     return;
	add	esp,018h

; 113     readRecord();
	mov	eax,ebx

; 115     return;
	pop	ebx
	ret	
__ct__9PartTableFP8VOS2DiskUlT2	endp

; 171 PartTable::readRecord(void)

	public readRecord__9PartTableFv
readRecord__9PartTableFv	proc
	push	ebx
	mov	ebx,eax
	push	edi
	push	esi
	sub	esp,01ch
	mov	[esp+02ch],ebx;	this

; 173     DEVICEPARAMETERBLOCK dpb = disk->queryDPB();
	mov	eax,[ebx]
	call	queryDPB__8VOS2DiskFv
	lea	edi,[esp+0ch];	dpb
	mov	esi,eax
	mov	ecx,04h
	rep movsd	

; 174     USHORT const	spt = dpb.cSectorsPerTrack;
; 175     ULONG const		cylsize = dpb.cHeads * spt;
; 176 
; 177     if( (secno % cylsize) != 0 )
	movzx	edx,word ptr [esp+012h];	dpb
	movzx	ecx,word ptr [esp+010h];	dpb
	imul	ecx,edx
	mov	eax,[ebx+04h]
	xor	edx,edx
	div	ecx
	test	edx,edx
	je	@BLBL15

; 178     {
; 179 	secno /= cylsize;
	mov	[ebx+04h],eax

; 180 	secno *= cylsize;
	imul	eax,ecx
	mov	[ebx+04h],eax

; 181     }
@BLBL15:

; 182 
; 183     rderr = disk->read(secno, 1, &bootrecord);
	mov	ebx,[esp+02ch];	this
	mov	eax,[ebx]
	lea	ecx,[ebx+0ch]
	push	ecx
	mov	edi,[eax]
	sub	esp,0ch
	mov	esi,[edi+01ch]
	mov	edx,[ebx+04h]
	mov	ecx,01h
	add	eax,esi
	call	dword ptr [edi+018h]
	add	esp,010h
	mov	[ebx+0214h],eax

; 184     if( rderr != 0 )
	test	eax,eax
	je	@BLBL16

; 185     {
; 186 	/* 'rderr' is now /0,  the disk will never be written to. */
; 187 
; 188 	Verbose(1, "PartTable", "can't read disk %lu, sector %lu - rc %lu",
	mov	eax,[ebx]
	call	queryIndex__8VOS2DiskFv
	push	dword ptr [ebx+0214h]
	mov	ecx,offset FLAT:@CBE3
	push	dword ptr [ebx+04h]
	mov	edx,offset FLAT:@CBE1
	push	eax
	mov	eax,01h
	sub	esp,0ch
	call	Verbose
	add	esp,018h

; 189 		disk->queryIndex(), secno, rderr);
; 190     }
@BLBL16:

; 191 
; 192     if( bootrecord.Signature != 0xAA55 )
	mov	ebx,[esp+02ch];	this
	cmp	word ptr [ebx+020ah],0aa55h
	je	@BLBL17

; 193     {
; 194 	Verbose(1, "PartTable",
	mov	eax,[ebx]
	call	queryIndex__8VOS2DiskFv
	push	dword ptr [ebx+04h]
	mov	ecx,offset FLAT:@CBE4
	push	eax
	mov	edx,offset FLAT:@CBE1
	sub	esp,0ch
	mov	eax,01h
	call	Verbose

; 195 		"clearing invalid partition table (disk %lu, sector %lu)",
; 196 		disk->queryIndex(), secno);
; 197 	memset(&bootrecord, 0, sizeof(bootrecord));
	mov	ecx,0200h
	xor	edx,edx
	lea	eax,[ebx+0ch]
	call	memset
	add	esp,014h

; 198 	bootrecord.Signature = 0xAA55;
	mov	word ptr [ebx+020ah],0aa55h

; 199 	valid = FALSE;				/* only write if modified */
	mov	dword ptr [ebx+0210h],0h

; 200     }
@BLBL17:

; 201 }
	add	esp,01ch
	pop	esi
	pop	edi
	pop	ebx
	ret	
readRecord__9PartTableFv	endp

; 63 PartTable::PartTable(VOS2Disk * d,ULONG n,Boolean rd)

	public __ct__9PartTableFP8VOS2DiskUl7Boolean
__ct__9PartTableFP8VOS2DiskUl7Boolean	proc
	push	ebx
	mov	ebx,ecx
	xchg	ebx,eax
	sub	esp,04h

; 72     entry = bootrecord.PartitionTable;
	lea	ecx,[ebx+01cah]

; 63 PartTable::PartTable(VOS2Disk * d,ULONG n,Boolean rd)
	mov	[esp+0ch],ebx;	this

; 65     disk = d;
	mov	[ebx],edx

; 66     secno = n;
	mov	[ebx+04h],eax

; 74     Verbose(2, "PartTable", "disk %lu, sector %lu", disk->queryIndex(), secno);
	mov	eax,[ebx]

; 67     rderr = 0;
	mov	dword ptr [ebx+0214h],0h

; 68     modified = FALSE;
	mov	dword ptr [ebx+020ch],0h

; 69     valid = TRUE;
	mov	dword ptr [ebx+0210h],01h

; 70     extstart = 0;				/* not an extended partition */
	mov	dword ptr [ebx+08h],0h

; 72     entry = bootrecord.PartitionTable;
	mov	[ebx+0218h],ecx

; 74     Verbose(2, "PartTable", "disk %lu, sector %lu", disk->queryIndex(), secno);
	call	queryIndex__8VOS2DiskFv
	push	dword ptr [ebx+04h]
	mov	ecx,offset FLAT:@CBE2
	push	eax
	mov	edx,offset FLAT:@CBE1
	sub	esp,0ch
	mov	eax,02h
	call	Verbose

; 76     readRecord();
	mov	eax,ebx
	call	readRecord__9PartTableFv
	add	esp,014h
	mov	eax,ebx

; 77     if( rd == true )
	cmp	byte ptr [esp+018h],01h;	rd
	jne	@BLBL3

; 78 	rderr = 0xFFFF;				/* any error will inhibit writing */
	mov	dword ptr [eax+0214h],0ffffh
@BLBL3:

; 79     return;
	mov	eax,[esp+0ch];	this
	add	esp,04h
	pop	ebx
	ret	
__ct__9PartTableFP8VOS2DiskUl7Boolean	endp
CODE32	ends
end
