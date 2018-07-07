	title	crc.cpp
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
CONST32_RO	segment
@1vcid	db "$Id: crc.cpp,v 1.5 2000/"
db "04/10 01:00:56 vitus Exp"
db " $",0h
CONST32_RO	ends
BSS32	segment
@2CRCtable	db 0200h DUP (0h)
BSS32	ends
CODE32	segment

; 89 UpdateCRC(USHORT crc,UCHAR byte)

UpdateCRC__FUsUc	proc

; 95     return (USHORT)((crc >> 8) ^ CRCtable[ (crc ^ byte) & 0xFF ]);
	movzx	edx,dl
	movzx	ecx,ax
	xor	ecx,edx
	shr	ax,08h
	and	ecx,0ffh
	xor	ax,word ptr [ecx*02h+@2CRCtable]
	movzx	eax,ax
	ret	
UpdateCRC__FUsUc	endp

; 48 CalcTable(USHORT data,USHORT genpoly,USHORT accum)

CalcTable__FUsN21	proc
	push	ebx
	push	edi

; 52     for( i = 0; i < 8; ++i )
	xor	edi,edi
@BLBL4:

; 54 	if( (accum^data) & 1 )
	mov	bx,ax
	xor	bx,cx
	test	bl,01h
	je	@BLBL5

; 56 	    accum >>=1;
	shr	cx,01h

; 57 	    accum ^= genpoly;
	xor	cx,dx

; 58 	}
	jmp	@BLBL6
@BLBL5:

; 60 	    accum >>= 1;
	shr	cx,01h
@BLBL6:

; 52     for( i = 0; i < 8; ++i )
	inc	edi

; 62 	data >>= 1;
	shr	ax,01h

; 52     for( i = 0; i < 8; ++i )
	cmp	edi,08h
	jl	@BLBL4

; 64     return accum;
	pop	edi
	movzx	ecx,cx
	pop	ebx
	mov	eax,ecx
	ret	
CalcTable__FUsN21	endp

; 153 Crc16(PVOID buffer,USHORT cb)

	public Crc16__FPvUs
Crc16__FPvUs	proc
	push	ebx
	push	edi
	push	esi
	sub	esp,0ch

; 158     for(; cb != 0; --cb, ++p )
	test	dx,dx

; 155     USHORT	crc = 0;
	mov	word ptr [esp+0ah],0h;	crc

; 158     for(; cb != 0; --cb, ++p )
	je	@BLBL21
	mov	esi,eax
	xor	edi,edi
@BLBL22:
	mov	ebx,edx

; 159 	crc = UpdateCRC(crc, *p);
	mov	dl,[esi]
	mov	ax,di
	call	UpdateCRC__FUsUc

; 158     for(; cb != 0; --cb, ++p )
	movzx	ebx,bx
	dec	ebx
	inc	esi

; 159 	crc = UpdateCRC(crc, *p);
	mov	edx,ebx

; 158     for(; cb != 0; --cb, ++p )
	test	dx,dx

; 159 	crc = UpdateCRC(crc, *p);
	mov	di,ax

; 158     for(; cb != 0; --cb, ++p )
	jne	@BLBL22
	mov	[esp+0ah],di;	crc
@BLBL21:

; 160     return crc;
	movzx	eax,word ptr [esp+0ah];	crc
	add	esp,0ch
	pop	esi
	pop	edi
	pop	ebx
	ret	
Crc16__FPvUs	endp

; 126 CrcInit(USHORT uPOLYNOM)

	public CrcInit__FUs
CrcInit__FUs	proc
	push	edi

; 130     for( i = 0; i < 256u; ++i )
	xor	edi,edi

; 126 CrcInit(USHORT uPOLYNOM)
	push	esi

; 130     for( i = 0; i < 256u; ++i )
	mov	si,ax

; 126 CrcInit(USHORT uPOLYNOM)
	sub	esp,0ch

; 130     for( i = 0; i < 256u; ++i )
	mov	[esp+018h],ax;	uPOLYNOM
@BLBL15:
	mov	eax,edi

; 131 	CRCtable[i] = CalcTable(i, uPOLYNOM, 0);
	movzx	edi,ax
	xor	ecx,ecx
	mov	dx,si
	call	CalcTable__FUsN21
	mov	word ptr [edi*02h+@2CRCtable],ax

; 130     for( i = 0; i < 256u; ++i )
	inc	edi
	cmp	di,0100h
	jb	@BLBL15

; 132 }
	add	esp,0ch
	pop	esi
	pop	edi
	ret	
CrcInit__FUs	endp
CODE32	ends
end
