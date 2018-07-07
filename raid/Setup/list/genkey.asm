	title	genkey.c
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
	public	szProduct
	extrn	memset:proc
	extrn	_printfieee:proc
	extrn	_exeentry:proc
	extrn	_sscanfieee:proc
	extrn	_fprintfieee:proc
	extrn	strncpy:proc
	extrn	strlen:proc
	extrn	stderr:dword
DATA32	segment
@STAT1	db "Key: |",0h
	align 04h
@STAT2	db "%02x",0h
@STAT3	db "|",0ah,0h
@STAT4	db "usage: genkey <level> <u"
db "ser name>",0ah,0h
	align 04h
@STAT5	db " %u",0h
@STAT6	db "Invalid level %u",0ah,0h
	align 04h
@STAT7	db "Generating key for %s",0ah,"Us"
db "er: |%s|",0ah,"Level: %u",0ah,0ah,0h
	dd	_exeentry
DATA32	ends
CONST32_RO	segment
@1id	db "$Id: genkey.c,v 1.3 1999"
db "/06/06 20:04:42 vitus Ex"
db "p $",0h
szProduct	db "Visual RAID Filter",0h
CONST32_RO	ends
BSS32	segment
	align 04h
comm	szUser:byte:028h
	align 04h
comm	szKey:byte:028h
	align 04h
comm	uLevel:dword
BSS32	ends
CODE32	segment

; 92 {

	public main
main	proc
	push	ebx
	sub	esp,0ch

; 95     if( argc != 3 )
	cmp	eax,03h

; 92 {
	mov	[esp+018h],edx;	argv

; 95     if( argc != 3 )
	je	@BLBL9

; 97 	Help();
	call	Help

; 98 	return -1;
	add	esp,0ch
	or	eax,0ffffffffh
	pop	ebx
	ret	
@BLBL9:

; 101     sscanf(argv[1], " %u", &uLevel);
	push	offset FLAT:uLevel
	mov	edx,offset FLAT:@STAT5
	sub	esp,08h
	mov	eax,[esp+024h];	argv
	mov	eax,[eax+04h]
	call	_sscanfieee

; 102     if( uLevel < 1  ||  uLevel > 3 )
	mov	ecx,dword ptr  uLevel

; 101     sscanf(argv[1], " %u", &uLevel);
	add	esp,0ch

; 102     if( uLevel < 1  ||  uLevel > 3 )
	cmp	ecx,01h
	jb	@BLBL10
	cmp	ecx,03h
	jbe	@BLBL11
@BLBL10:

; 104 	fprintf(stderr, "Invalid level %u\n", uLevel);
	push	dword ptr  uLevel
	mov	eax,dword ptr  stderr
	sub	esp,08h
	mov	edx,offset FLAT:@STAT6
	call	_fprintfieee

; 105 	return -2;
	add	esp,018h
	mov	eax,0fffffffeh
	pop	ebx
	ret	
@BLBL11:

; 113     strncpy(szUser, &argv[2][i], REG_KEYLEN);
	mov	edx,[esp+018h];	argv
	mov	ebx,offset FLAT:szUser
	mov	edx,[edx+08h]
	mov	ecx,028h
	cmp	byte ptr [edx],022h
	sete	al
	movzx	eax,al
	add	edx,eax
	mov	eax,ebx
	call	strncpy

; 114     if( szUser[strlen(szUser)-1] == '"' )
	mov	eax,ebx
	call	strlen
	cmp	byte ptr [ebx+eax-01h],022h
	jne	@BLBL14

; 115 	szUser[strlen(szUser)-1] = '\0';
	mov	eax,ebx
	call	strlen
	mov	byte ptr [ebx+eax-01h],0h
@BLBL14:

; 117     printf("Generating key for %s\nUser: |%s|\nLevel: %u\n\n",
	push	dword ptr  uLevel
	mov	eax,offset FLAT:@STAT7
	push	offset FLAT:szUser
	push	offset FLAT:szProduct
	sub	esp,04h
	call	_printfieee

; 120     GenKey();
	call	GenKey

; 122     return 0;
	add	esp,01ch
	xor	eax,eax
	pop	ebx
	ret	
main	endp

; 82 {

Help	proc
	sub	esp,04h

; 83     printf("usage: genkey <level> <user name>\n");
	mov	eax,offset FLAT:@STAT4
	call	_printfieee

; 84     return;
	add	esp,04h
	ret	
Help	endp

; 47 {

GenKey	proc
	push	ebp

; 48     UCHAR	c = szProduct[0];		/* start value */
	mov	cl,byte ptr  szProduct

; 47 {
	mov	ebp,esp
	push	ebx

; 53     memset(szKey, 0, REG_KEYLEN);
	xor	edx,edx

; 47 {
	push	edi

; 53     memset(szKey, 0, REG_KEYLEN);
	mov	eax,offset FLAT:szKey

; 47 {
	push	esi
	sub	esp,01ch

; 48     UCHAR	c = szProduct[0];		/* start value */
	mov	[ebp-015h],cl;	c

; 53     memset(szKey, 0, REG_KEYLEN);
	mov	ecx,028h
	call	memset

; 54     for( len = 0, p = szUser, k = szProduct;
	cmp	byte ptr  szUser,0h
	mov	dword ptr [ebp-010h],0h;	len
	je	@BLBL1
	mov	bl,[ebp-015h];	c
	mov	edi,[ebp-014h];	T000000000
	xor	esi,esi
	mov	edx,offset FLAT:szUser
	mov	ecx,offset FLAT:szProduct
@BLBL2:
	mov	eax,edx

; 58 	c += (UCHAR)((*p & 0xf0) >> 2 ^ (*p & 0x0f));
	mov	dl,[eax]

; 56 	 ++p, ++len, k = (*(k+1) == '\0' ? szProduct : k+1) )
	inc	eax
	mov	[ebp-01ch],eax;	@CBE8

; 58 	c += (UCHAR)((*p & 0xf0) >> 2 ^ (*p & 0x0f));
	movzx	eax,dl
	and	eax,0f0h
	and	dl,0fh
	sar	eax,02h
	movzx	edx,dl
	xor	edx,eax

; 59 	c *= *k + uLevel;
	mov	al,[ecx]

; 58 	c += (UCHAR)((*p & 0xf0) >> 2 ^ (*p & 0x0f));
	add	dl,bl

; 59 	c *= *k + uLevel;
	movzx	ebx,al
	add	ebx,dword ptr  uLevel
	movzx	edx,dl
	imul	ebx,edx

; 60 	c += *k;
	add	bl,al

; 61 	szKey[len] = c;
	mov	byte ptr [esi+ szKey],bl

; 59 	c *= *k + uLevel;
	mov	edx,[ebp-01ch];	@CBE8

; 56 	 ++p, ++len, k = (*(k+1) == '\0' ? szProduct : k+1) )
	inc	esi
	cmp	byte ptr [ecx+01h],0h
	jne	@BLBL3
	mov	edi,offset FLAT:szProduct
	jmp	@BLBL4
@BLBL3:
	lea	edi,[ecx+01h]
@BLBL4:

; 55 	 *p != '\0';
	cmp	byte ptr [edx],0h

; 56 	 ++p, ++len, k = (*(k+1) == '\0' ? szProduct : k+1) )
	mov	ecx,edi

; 55 	 *p != '\0';
	jne	@BLBL2
	mov	[ebp-015h],bl;	c
	mov	[ebp-010h],esi;	len
	mov	[ebp-014h],ecx;	T000000000
@BLBL1:

; 68 	for( i = 0; i < len; ++i )
	mov	esi,[ebp-010h];	len

; 67 	printf("Key: |");
	mov	eax,offset FLAT:@STAT1
	call	_printfieee

; 68 	for( i = 0; i < len; ++i )
	test	esi,esi
	jle	@BLBL6
	xor	edi,edi
@BLBL7:

; 70 	    printf("%02x", szKey[i]);
	movzx	eax,byte ptr [edi+ szKey]
	push	eax
	mov	eax,offset FLAT:@STAT2
	sub	esp,04h

; 68 	for( i = 0; i < len; ++i )
	inc	edi

; 70 	    printf("%02x", szKey[i]);
	call	_printfieee
	add	esp,08h

; 68 	for( i = 0; i < len; ++i )
	cmp	edi,esi
	jl	@BLBL7
@BLBL6:

; 72 	printf("|\n");
	mov	eax,offset FLAT:@STAT3
	call	_printfieee

; 74     return;
	add	esp,01ch
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
GenKey	endp
CODE32	ends
end
