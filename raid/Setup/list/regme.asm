	title	regme.c
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
	public	szDriver
	extrn	_printfieee:proc
	extrn	_lseek:proc
	extrn	_errno:proc
	extrn	strerror:proc
	extrn	_fprintfieee:proc
	extrn	_read:proc
	extrn	_filelength:proc
	extrn	malloc:proc
	extrn	memcpy:proc
	extrn	_write:proc
	extrn	free:proc
	extrn	_exeentry:proc
	extrn	_sscanfieee:proc
	extrn	_open:proc
	extrn	_close:proc
	extrn	strncpy:proc
	extrn	strlen:proc
	extrn	stderr:dword
	extrn	_ctype:dword
DATA32	segment
@STAT1	db "usage: regme [-?] [-f:<f"
db "ile>] {-d | ",022h,"user name",022h," "
db "code}",0ah,0h
	align 04h
@STAT2	db 09h,09h,"VRAID registration too"
db "l",0ah,0ah,0h
@STAT3	db "Options:",0ah," -d",09h,09h,"display us"
db "er name in %s (no checks"
db ")",0ah," -f:<file>",09h,"use <file> "
db "instead of %s",0ah," -?",09h,09h,"this "
db "text",0ah,0h
	align 04h
@STAT4	db 0ah,"regme will patch a non-"
db "registered VRAID.flt wit"
db "h the provided",0ah,"user/key "
db "combination.  There is o"
db "nly basic validation of "
db "this combination.  ",0ah,"All "
db "real checks are made by "
db "VRAID.flt at startup, so"
db " be sure ",0ah,"to provide the"
db " correct information.",0ah,0h
	align 04h
@STAT5	db "Seek error (%s)",0ah,0h
	align 04h
@STAT6	db "Read error (%s)",0ah,0h
	align 04h
@STAT7	db "Internal error 1",0ah,0h
	align 04h
@STAT8	db "Internal error 2",0ah,0h
	align 04h
@STAT9	db "Memory allocation failed"
db 0ah,0h
	align 04h
@STATa	db "Already registered versi"
db "on, aborting",0ah,0h
	align 04h
@STATb	db "Seek error (%s)",0ah,0h
	align 04h
@STATc	db "Write error (%s)",0ah,0h
	align 04h
@STATd	db "Memory allocation failed"
db 0ah,0h
	align 04h
@STATe	db "Non-registered version, "
db "aborting",0ah,0h
	align 04h
@STATf	db "Current user: ",022h,"%s",022h,0ah,0h
@STAT10	db "%s: unknown arg ",022h,"%s",022h,0ah,0h
	align 04h
@STAT11	db "regme",0h
@STAT12	db "%s",0h
	align 04h
@STAT13	db "%s: unknown arg ",022h,"%s",022h,0ah,0h
	align 04h
@STAT14	db "regme",0h
	align 04h
@STAT15	db "Can't open %s (%s)",0ah,0h
@STAT16	db "Invalid key |%s|",0ah,0h
	align 04h
@STAT17	db "%02x",0h
	align 04h
@STAT18	db "Registering %s",0ah,"User: |%s"
db "|",0ah,"Key: |%s|",0ah,0h
	align 04h
@STAT19	db "Can't open %s (%s)",0ah,0h
@STAT1a	db "Done",0ah,0h
	align 04h
szDriver	db "VRAID.flt",0h
	db 0fah DUP (00H)
	dd	_exeentry
DATA32	ends
CONST32_RO	segment
@1id	db "$Id: regme.c,v 1.3 1998/"
db "12/13 23:08:57 vitus Exp"
db " $",0h
	align 04h
szProduct	db "Visual RAID Filter",0h
CONST32_RO	ends
BSS32	segment
	align 04h
comm	szUser:byte:028h
	align 04h
comm	szKey:byte:028h
BSS32	ends
CODE32	segment

; 227 {

	public main
main	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	[ebp+0ch],edx;	argv
	push	edi
	mov	[ebp+08h],eax;	argc
	push	esi
	sub	esp,01ch

; 231     while( argc > 1  &&  argv[1][0] == '-' )
	cmp	eax,01h

; 228     int	display = 0;
	mov	dword ptr [ebp-010h],0h;	display

; 231     while( argc > 1  &&  argv[1][0] == '-' )
	jle	@BLBL24
	mov	ecx,[edx+04h]
	cmp	byte ptr [ecx],02dh
	jne	@BLBL24
	mov	edi,eax
@BLBL25:

; 233 	switch( argv[1][1] )
	mov	eax,[edx+04h]
	movzx	eax,byte ptr [eax+01h]
	cmp	eax,03fh
	je	@BLBL47
	cmp	eax,064h
	je	@BLBL48
	cmp	eax,066h
	je	@BLBL49
	jmp	@BLBL50
@BLBL47:

; 236 	    Help();
	call	Help

; 237 	    return 0;
	add	esp,01ch
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL48:

; 240 	    display = 1;
	mov	dword ptr [ebp-010h],01h;	display

; 241 	    break;
	jmp	@BLBL46
@BLBL49:

; 244 	    if( argv[1][2] != ':'  ||  argv[1][3] == '\0' )
	mov	eax,[edx+04h]
	cmp	byte ptr [eax+02h],03ah
	jne	@BLBL26
	cmp	byte ptr [eax+03h],0h
	jne	@BLBL27
@BLBL26:

; 246 		fprintf(stderr, "%s: unknown arg \"%s\"\n", "regme", argv[1]);
	push	dword ptr [edx+04h]
	mov	eax,dword ptr  stderr
	push	offset FLAT:@STAT11
	mov	edx,offset FLAT:@STAT10
	sub	esp,08h
	call	_fprintfieee
	add	esp,010h

; 247 		return -1;
	add	esp,01ch
	or	eax,0ffffffffh
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL27:

; 249 	    sscanf(&argv[1][3], "%s", szDriver);
	push	offset FLAT:szDriver

; 247 		return -1;
	mov	ebx,edx

; 249 	    sscanf(&argv[1][3], "%s", szDriver);
	sub	esp,08h
	mov	eax,[ebx+04h]
	mov	edx,offset FLAT:@STAT12
	add	eax,03h
	call	_sscanfieee
	mov	edx,ebx
	add	esp,0ch

; 250 	    break;
	jmp	@BLBL46
@BLBL50:

; 253 	    fprintf(stderr, "%s: unknown arg \"%s\"\n", "regme", argv[1]);
	push	dword ptr [edx+04h]
	mov	eax,dword ptr  stderr
	push	offset FLAT:@STAT14
	mov	edx,offset FLAT:@STAT13
	sub	esp,08h
	call	_fprintfieee
	add	esp,010h

; 254 	    return -1;
	add	esp,01ch
	or	eax,0ffffffffh
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL46:

; 256 	--argc;
	dec	edi

; 257 	++argv;
	add	edx,04h

; 231     while( argc > 1  &&  argv[1][0] == '-' )
	cmp	edi,01h
	jle	@BLBL45
	mov	eax,[edx+04h]
	cmp	byte ptr [eax],02dh
	je	@BLBL25
@BLBL45:
	mov	[ebp+08h],edi;	argc
	mov	[ebp+0ch],edx;	argv
@BLBL24:

; 260     if( display )
	cmp	dword ptr [ebp-010h],0h;	display
	je	@BLBL30

; 264 	if( argc != 1 )
	cmp	dword ptr [ebp+08h],01h;	argc
	je	@BLBL31

; 266 	    Usage();
	call	Usage

; 267 	    return -1;
	add	esp,01ch
	or	eax,0ffffffffh
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL31:

; 270 	if( (hd=_open(szDriver, O_RDONLY|O_BINARY)) == -1 )
	mov	ebx,offset FLAT:szDriver
	mov	edx,08004h
	mov	eax,ebx
	call	_open
	mov	[ebp-018h],eax;	hd
	cmp	eax,0ffffffffh
	jne	@BLBL32

; 272 	    fprintf(stderr, "Can't open %s (%s)\n", szDriver, strerror(errno));
	call	_errno
	mov	eax,[eax]
	call	strerror
	push	eax
	mov	eax,dword ptr  stderr
	push	ebx
	mov	edx,offset FLAT:@STAT15
	sub	esp,08h
	call	_fprintfieee

; 273 	    return errno;
	call	_errno
	add	esp,010h
	mov	eax,[eax]
	add	esp,01ch
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL32:

; 276 	rc = DisplayUser(hd);
	mov	edi,[ebp-018h];	hd
	mov	eax,edi
	call	DisplayUser
	mov	ecx,eax
	mov	eax,edi
	mov	[ebp-014h],ecx;	rc

; 277 	_close(hd);
	call	_close
	jmp	@BLBL33
@BLBL30:

; 284 	if( argc != 3 )
	cmp	dword ptr [ebp+08h],03h;	argc
	je	@BLBL34

; 286 	    Usage();
	call	Usage

; 287 	    return -1;
	add	esp,01ch
	or	eax,0ffffffffh
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL34:

; 295 	strncpy(szUser, &argv[1][i], REG_KEYLEN);
	mov	edx,[ebp+0ch];	argv
	mov	ebx,offset FLAT:szUser
	mov	edx,[edx+04h]
	mov	ecx,028h
	cmp	byte ptr [edx],022h
	sete	al
	movzx	eax,al
	add	edx,eax
	mov	eax,ebx
	call	strncpy

; 296 	if( szUser[strlen(szUser)-1] == '"' )
	mov	eax,ebx
	call	strlen
	cmp	byte ptr [ebx+eax-01h],022h
	jne	@BLBL37

; 297 	    szUser[strlen(szUser)-1] = '\0';
	mov	eax,ebx
	call	strlen
	mov	byte ptr [ebx+eax-01h],0h
@BLBL37:

; 298 	for( i = 0; i < strlen(argv[2]); i += 2 )
	mov	ebx,[ebp+0ch];	argv
	add	ebx,08h
	mov	eax,[ebx]
	call	strlen
	test	eax,eax
	jbe	@BLBL38
	mov	esi,ebx
	xor	edi,edi
@BLBL39:

; 300 	    if( !isxdigit(argv[2][i]) || !isxdigit(argv[2][i+1]) )
	mov	ebx,[esi]
	add	ebx,edi
	mov	eax,dword ptr  _ctype
	movzx	ecx,byte ptr [ebx]
	movzx	ecx,word ptr [eax+ecx*02h]
	and	ecx,01h
	je	@BLBL40
	movzx	ebx,byte ptr [ebx+01h]
	movzx	eax,word ptr [eax+ebx*02h]
	and	eax,01h
	jne	@BLBL41
@BLBL40:

; 302 		fprintf(stderr, "Invalid key |%s|\n", argv[2]);
	push	dword ptr [esi]
	mov	eax,dword ptr  stderr
	sub	esp,08h
	mov	edx,offset FLAT:@STAT16
	call	_fprintfieee
	add	esp,0ch

; 303 		return -2;
	add	esp,01ch
	mov	eax,0fffffffeh
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL41:

; 305 	    sscanf(&argv[2][i], "%02x", &szKey[i/2]);
	mov	eax,edi
	cdq	
	and	edx,01h
	add	eax,edx
	sar	eax,01h
	mov	edx,offset FLAT:@STAT17
	lea	ebx,dword ptr [eax+ szKey]
	push	ebx
	mov	eax,[esi]
	sub	esp,08h
	add	eax,edi
	call	_sscanfieee

; 298 	for( i = 0; i < strlen(argv[2]); i += 2 )
	mov	eax,[esi]
	call	strlen
	add	edi,02h
	mov	ebx,edi
	add	esp,0ch
	cmp	eax,ebx
	ja	@BLBL39
@BLBL38:

; 308 	printf("Registering %s\nUser: |%s|\nKey: |%s|\n", szProduct, szUser, argv[2]);
	mov	ebx,[ebp+0ch];	argv
	mov	eax,offset FLAT:@STAT18
	push	dword ptr [ebx+08h]

; 310 	if( (hd=_open(szDriver, O_RDWR|O_BINARY)) == -1 )
	mov	ebx,offset FLAT:szDriver

; 308 	printf("Registering %s\nUser: |%s|\nKey: |%s|\n", szProduct, szUser, argv[2]);
	push	offset FLAT:szUser
	push	offset FLAT:szProduct
	sub	esp,04h
	call	_printfieee

; 310 	if( (hd=_open(szDriver, O_RDWR|O_BINARY)) == -1 )
	mov	edx,08002h
	mov	eax,ebx
	call	_open
	add	esp,010h
	mov	[ebp-01ch],eax;	hd
	cmp	eax,0ffffffffh
	jne	@BLBL43

; 312 	    fprintf(stderr, "Can't open %s (%s)\n", szDriver, strerror(errno));
	call	_errno
	mov	eax,[eax]
	call	strerror
	push	eax
	mov	eax,dword ptr  stderr
	push	ebx
	mov	edx,offset FLAT:@STAT19
	sub	esp,08h
	call	_fprintfieee

; 313 	    return errno;
	call	_errno
	add	esp,010h
	mov	eax,[eax]
	add	esp,01ch
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL43:

; 316 	rc = DoRegistration(hd);
	mov	edi,[ebp-01ch];	hd
	mov	eax,edi
	call	DoRegistration
	xchg	edi,eax

; 317 	_close(hd);
	call	_close

; 316 	rc = DoRegistration(hd);
	mov	[ebp-014h],edi;	rc

; 319 	if( rc == 0 )
	test	edi,edi
	jne	@BLBL33

; 320 	    printf("Done\n");
	mov	eax,offset FLAT:@STAT1a
	call	_printfieee
@BLBL33:

; 323     return rc;
	mov	eax,[ebp-014h];	rc
	add	esp,01ch
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
main	endp

; 187 {

DisplayUser	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	[ebp+08h],eax;	hd
	push	edi
	push	esi
	sub	esp,018h

; 188     ULONG const size = (ULONG)_filelength(hd);
	call	_filelength
	mov	[ebp-018h],eax;	size

; 189     PUCHAR const buffer = malloc(size);
	call	malloc
	mov	edi,eax

; 190     REGSTRUCT *	reg = NULL;
	mov	dword ptr [ebp-010h],0h;	reg

; 192     if( buffer == NULL )
	test	eax,eax
	jne	@BLBL17

; 194 	fprintf(stderr, "Memory allocation failed\n");
	mov	eax,dword ptr  stderr
	mov	edx,offset FLAT:@STATd
	call	_fprintfieee

; 195 	return -3;
	add	esp,018h
	mov	eax,0fffffffdh
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL17:

; 200 	if( ReadCompleteFile(hd, size, buffer) )
	mov	ebx,[ebp-018h];	size
	mov	esi,edi
	mov	eax,[ebp+08h];	hd
	mov	ecx,esi
	mov	edx,ebx
	call	ReadCompleteFile
	test	eax,eax
	jne	@BLBL19

; 203 	if( (errno=SearchRegInfo(buffer, size, &reg)) != 0 )
	call	_errno
	xchg	esi,eax
	mov	edx,ebx
	lea	ecx,[ebp-010h];	reg
	call	SearchRegInfo
	mov	[esi],eax
	test	eax,eax
	jne	@BLBL19

; 206 	if( reg->szUser[0] == 0 )
	mov	ecx,[ebp-010h];	reg
	cmp	byte ptr [ecx+04h],0h
	jne	@BLBL21

; 208 	    fprintf(stderr, "Non-registered version, aborting\n");
	mov	eax,dword ptr  stderr
	mov	edx,offset FLAT:@STATe
	call	_fprintfieee

; 209 	    errno = -5;
	call	_errno
	mov	dword ptr [eax],0fffffffbh

; 210 	    break;
	jmp	@BLBL19
@BLBL21:

; 213 	printf("Current user: \"%s\"\n", reg->szUser);
	mov	ebx,[ebp-010h];	reg
	add	ebx,04h
	push	ebx
	mov	eax,offset FLAT:@STATf
	sub	esp,04h
	call	_printfieee

; 214 	errno = 0;
	call	_errno
	add	esp,08h
	mov	dword ptr [eax],0h

; 216     while( 0 );
@BLBL19:

; 218     free(buffer);
	mov	eax,edi
	call	free

; 219     return errno;
	call	_errno
	mov	eax,[eax]
	add	esp,018h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
DisplayUser	endp

; 132 {

DoRegistration	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	[ebp+08h],eax;	hd
	push	edi
	push	esi
	sub	esp,018h

; 133     ULONG const size = (ULONG)_filelength(hd);
	call	_filelength
	mov	[ebp-018h],eax;	size

; 134     PUCHAR const buffer = malloc(size);
	call	malloc
	mov	edi,eax

; 135     REGSTRUCT *	reg = NULL;
	mov	dword ptr [ebp-010h],0h;	reg

; 137     if( buffer == NULL )
	test	eax,eax
	jne	@BLBL9

; 139 	fprintf(stderr, "Memory allocation failed\n");
	mov	eax,dword ptr  stderr
	mov	edx,offset FLAT:@STAT9
	call	_fprintfieee

; 140 	return -3;
	add	esp,018h
	mov	eax,0fffffffdh
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL9:

; 145 	if( ReadCompleteFile(hd, size, buffer) )
	mov	ebx,[ebp-018h];	size
	mov	esi,edi
	mov	eax,[ebp+08h];	hd
	mov	ecx,esi
	mov	edx,ebx
	call	ReadCompleteFile
	test	eax,eax
	jne	@BLBL11

; 148 	if( (errno=SearchRegInfo(buffer, size, &reg)) != 0 )
	call	_errno
	xchg	esi,eax
	mov	edx,ebx
	lea	ecx,[ebp-010h];	reg
	call	SearchRegInfo
	mov	[esi],eax
	test	eax,eax
	jne	@BLBL11

; 154 	if( reg->szUser[0] != 0 )
	mov	ecx,[ebp-010h];	reg
	cmp	byte ptr [ecx+04h],0h
	je	@BLBL13

; 156 	    fprintf(stderr, "Already registered version, aborting\n");
	mov	eax,dword ptr  stderr
	mov	edx,offset FLAT:@STATa
	call	_fprintfieee

; 157 	    errno = -5;
	call	_errno
	mov	dword ptr [eax],0fffffffbh

; 158 	    break;
	jmp	@BLBL11
@BLBL13:

; 161 	memcpy(reg->szUser, szUser, REG_KEYLEN);
	mov	eax,[ebp-010h];	reg
	mov	ecx,028h
	mov	edx,offset FLAT:szUser
	add	eax,04h
	call	memcpy

; 162 	memcpy(reg->szRegCode, szKey, REG_KEYLEN);
	mov	eax,[ebp-010h];	reg
	mov	ecx,028h
	mov	edx,offset FLAT:szKey
	add	eax,02ch
	call	memcpy

; 164 	if( _lseek(hd, 0, SEEK_SET) != 0 )
	mov	eax,[ebp+08h];	hd
	xor	ecx,ecx
	xor	edx,edx
	call	_lseek
	test	eax,eax
	je	@BLBL14

; 166 	    fprintf(stderr, "Seek error (%s)\n", strerror(errno));
	call	_errno
	mov	eax,[eax]
	call	strerror
	push	eax
	mov	eax,dword ptr  stderr
	sub	esp,08h
	mov	edx,offset FLAT:@STATb
	call	_fprintfieee
	add	esp,0ch

; 167 	    break;
	jmp	@BLBL11
@BLBL14:

; 169 	if( _write(hd, buffer, size) != size )
	mov	ebx,[ebp-018h];	size
	mov	eax,[ebp+08h];	hd
	mov	ecx,ebx
	mov	edx,edi
	call	_write
	cmp	eax,ebx
	je	@BLBL15

; 171 	    fprintf(stderr, "Write error (%s)\n", strerror(errno));
	call	_errno
	mov	eax,[eax]
	call	strerror
	push	eax
	mov	eax,dword ptr  stderr
	sub	esp,08h
	mov	edx,offset FLAT:@STATc
	call	_fprintfieee
	add	esp,0ch

; 172 	    break;
	jmp	@BLBL11
@BLBL15:

; 174 	errno = 0;
	call	_errno
	mov	dword ptr [eax],0h

; 176     while( 0 );
@BLBL11:

; 178     free(buffer);
	mov	eax,edi
	call	free

; 179     return errno;
	call	_errno
	mov	eax,[eax]
	add	esp,018h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
DoRegistration	endp

; 104 {

SearchRegInfo	proc
	push	edi
	push	esi

; 107     for( p = buffer; ((REGSTRUCT *)p)->ulMagic != REG_MAGIC; ++p )
	mov	esi,eax

; 104 {
	sub	esp,0ch

; 107     for( p = buffer; ((REGSTRUCT *)p)->ulMagic != REG_MAGIC; ++p )
	cmp	dword ptr [eax],053a1b7f9h
	je	@BLBL3
	mov	edi,eax
@BLBL4:

; 107     for( p = buffer; ((REGSTRUCT *)p)->ulMagic != REG_MAGIC; ++p )
	inc	edi
	cmp	dword ptr [edi],053a1b7f9h
	jne	@BLBL4
	mov	esi,edi
@BLBL3:

; 113     *reg = (REGSTRUCT *)p;
	mov	eax,esi
	mov	[ecx],eax

; 114     if( (*reg)->ulMagic != REG_MAGIC )
	cmp	dword ptr [eax],053a1b7f9h
	je	@BLBL6

; 116 	fprintf(stderr, "Internal error 1\n");
	mov	eax,dword ptr  stderr
	mov	edx,offset FLAT:@STAT7
	call	_fprintfieee

; 117 	return -4;
	add	esp,0ch
	mov	eax,0fffffffch
	pop	esi
	pop	edi
	ret	
@BLBL6:

; 119     if( (UCHAR)(*reg)->ulSize != sizeof(REGSTRUCT) )
	mov	eax,[ecx]
	mov	al,[eax+054h]
	cmp	al,058h
	je	@BLBL7

; 121 	fprintf(stderr, "Internal error 2\n");
	mov	eax,dword ptr  stderr
	mov	edx,offset FLAT:@STAT8
	call	_fprintfieee

; 122 	return -4;
	add	esp,0ch
	mov	eax,0fffffffch
	pop	esi
	pop	edi
	ret	
@BLBL7:

; 124     return 0;
	xor	eax,eax
	add	esp,0ch
	pop	esi
	pop	edi
	ret	
SearchRegInfo	endp

; 85 {

ReadCompleteFile	proc
	push	ebx
	sub	esp,0ch
	mov	[esp+01ch],ecx;	buffer
	mov	[esp+018h],edx;	size

; 86     if( _lseek(hd, 0, SEEK_SET) != 0 )
	xor	ecx,ecx

; 85 {
	mov	[esp+014h],eax;	hd

; 86     if( _lseek(hd, 0, SEEK_SET) != 0 )
	xor	edx,edx
	call	_lseek
	test	eax,eax
	je	@BLBL1

; 88 	fprintf(stderr, "Seek error (%s)\n", strerror(errno));
	call	_errno
	mov	eax,[eax]
	call	strerror
	push	eax
	mov	eax,dword ptr  stderr
	sub	esp,08h
	mov	edx,offset FLAT:@STAT5
	call	_fprintfieee

; 89 	return 1;
	add	esp,018h
	mov	eax,01h
	pop	ebx
	ret	
@BLBL1:

; 91     if( _read(hd, buffer, size) != size )
	mov	ebx,[esp+018h];	size
	mov	edx,[esp+01ch];	buffer
	mov	eax,[esp+014h];	hd
	mov	ecx,ebx
	call	_read
	cmp	eax,ebx
	je	@BLBL2

; 93 	fprintf(stderr, "Read error (%s)\n", strerror(errno));
	call	_errno
	mov	eax,[eax]
	call	strerror
	push	eax
	mov	eax,dword ptr  stderr
	sub	esp,08h
	mov	edx,offset FLAT:@STAT6
	call	_fprintfieee

; 94 	return 2;
	add	esp,018h
	mov	eax,02h
	pop	ebx
	ret	
@BLBL2:

; 96     return 0;
	xor	eax,eax
	add	esp,0ch
	pop	ebx
	ret	
ReadCompleteFile	endp

; 65 {

Help	proc
	sub	esp,04h

; 66     printf("\t\tVRAID registration tool\n\n");
	mov	eax,offset FLAT:@STAT2
	call	_printfieee

; 67     Usage();
	call	Usage

; 68     printf("Options:\n"
	mov	ecx,offset FLAT:szDriver
	push	ecx
	mov	eax,offset FLAT:@STAT3
	push	ecx
	sub	esp,04h
	call	_printfieee

; 72     printf("\nregme will patch a non-registered VRAID.flt with "
	mov	eax,offset FLAT:@STAT4
	call	_printfieee
	add	esp,0ch

; 77     return;
	add	esp,04h
	ret	
Help	endp

; 58 {

Usage	proc
	sub	esp,04h

; 59     printf("usage: regme [-?] [-f:<file>] {-d | \"user name\" code}\n");
	mov	eax,offset FLAT:@STAT1
	call	_printfieee

; 60 }
	add	esp,04h
	ret	
Usage	endp
CODE32	ends
end
