	title	../../../Lib/profile.c
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
	extrn	PrfWriteProfileData:proc
	extrn	WinGetLastError:proc
	extrn	Verbose:proc
	extrn	free:proc
	extrn	strdup:proc
	extrn	DosCreateMutexSem:proc
	extrn	PrfOpenProfile:proc
	extrn	PrfQueryProfileData:proc
	extrn	malloc:proc
	extrn	memset:proc
	extrn	memcpy:proc
	extrn	PrfCloseProfile:proc
	extrn	DosCloseMutexSem:proc
	extrn	DosRequestMutexSem:proc
	extrn	strcmp:proc
	extrn	memcmp:proc
	extrn	DosReleaseMutexSem:proc
	extrn	strcpy:proc
DATA32	segment
@STAT1	db "Profile",0h
@STAT2	db "PrfWriteProfileData fail"
db "s - error %#x",0h
	align 04h
@STAT3	db "Profile",0h
@STAT4	db "OpenProfile: %s already "
db "open",0h
	align 04h
@STAT5	db "Profile",0h
@STAT6	db "OpenProfile: memory prob"
db "lem",0h
@STAT7	db "Profile",0h
@STAT8	db "PrfOpenProfile fails - e"
db "rror %#x",0h
	align 04h
@STAT9	db "Profile",0h
@STATa	db "PrfCloseProfile fails - "
db "error %#x",0h
	align 04h
@STATb	db "Profile",0h
@STATc	db "Saved key %s in profile",0h
@1vcid	db "$Id: profile.c,v 2.1 200"
db "0/05/14 01:25:51 vitus E"
db "xp $",0h
DATA32	ends
BSS32	segment
@2hmtxSetting	dd 0h
@3pSetting	dd 0h
@4pszIniFile	dd 0h
BSS32	ends
CODE32	segment

; 154 {

NextWord	proc
	mov	[esp+04h],eax;	cp

; 155     for(; *cp != '\0'; ++cp )
	cmp	byte ptr [eax],0h
	je	@BLBL4
	mov	ecx,eax
@BLBL5:

; 155     for(; *cp != '\0'; ++cp )
	inc	ecx
	cmp	byte ptr [ecx],0h
	jne	@BLBL5
	mov	[esp+04h],ecx;	cp
@BLBL4:

; 157     return ++cp;
	mov	eax,[esp+04h];	cp
	inc	eax
	ret	
NextWord	endp

; 531 {

	public ProfileDelete
ProfileDelete	proc
	push	ebx
	mov	ebx,edx
	push	edi
	push	esi
	sub	esp,08h
	mov	[esp+01ch],ebx;	key
	mov	[esp+018h],eax;	app

; 537     DosRequestMutexSem( hmtxSetting, (ULONG)SEM_INDEFINITE_WAIT );
	push	0ffffffffh
	push	dword ptr  @2hmtxSetting
	call	DosRequestMutexSem

; 538     for( prev = NULL, p = pSetting; p != NULL ; prev = p, p = p->next )
	mov	edi,dword ptr  @3pSetting

; 537     DosRequestMutexSem( hmtxSetting, (ULONG)SEM_INDEFINITE_WAIT );
	add	esp,08h
	mov	edx,ebx

; 538     for( prev = NULL, p = pSetting; p != NULL ; prev = p, p = p->next )
	test	edi,edi
	je	@BLBL47
	xor	esi,esi
@BLBL48:

; 540 	if( strcmp(app, p->app) == 0  &&  strcmp(key, p->key) == NULL )
	mov	edx,[edi+08h]
	mov	eax,[esp+018h];	app
	call	strcmp
	test	eax,eax
	jne	@BLBL49
	mov	edx,[edi+0ch]
	mov	eax,ebx
	call	strcmp
	test	eax,eax
	jne	@BLBL49

; 542 	    free( p->data );
	mov	eax,[edi+014h]
	call	free

; 543 	    if( prev != NULL )
	test	esi,esi
	je	@BLBL50

; 544 		prev->next = p->next;
	mov	eax,[edi]
	mov	[esi],eax
	jmp	@BLBL51
@BLBL50:

; 546 		pSetting = p->next;
	mov	eax,[edi]
	mov	dword ptr  @3pSetting,eax
@BLBL51:

; 547 	    free( p );
	mov	eax,edi
	call	free

; 548 	    break;
	jmp	@BLBL47
@BLBL49:

; 538     for( prev = NULL, p = pSetting; p != NULL ; prev = p, p = p->next )
	mov	esi,edi
	mov	edi,[esi]
	test	edi,edi
	jne	@BLBL48
@BLBL47:

; 551     DosReleaseMutexSem( hmtxSetting );
	push	dword ptr  @2hmtxSetting
	call	DosReleaseMutexSem

; 553     return 0;
	add	esp,0ch
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	ret	
ProfileDelete	endp

; 471 {

	public ProfileReadNext
ProfileReadNext	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,edx
	push	edi
	mov	edi,eax
	push	esi
	mov	[ebp+010h],ecx;	bufsiz
	sub	esp,014h

; 473     ULONG	rc = (ULONG)-1;
	mov	dword ptr [ebp-010h],0ffffffffh;	rc

; 478     DosRequestMutexSem(hmtxSetting, (ULONG)SEM_INDEFINITE_WAIT);
	push	0ffffffffh

; 471 {
	mov	[ebp+0ch],ebx;	key

; 478     DosRequestMutexSem(hmtxSetting, (ULONG)SEM_INDEFINITE_WAIT);
	push	dword ptr  @2hmtxSetting
	call	DosRequestMutexSem
	mov	eax,edi

; 479     for( p = pSetting; p != NULL ; p = p->next )
	mov	edi,dword ptr  @3pSetting

; 478     DosRequestMutexSem(hmtxSetting, (ULONG)SEM_INDEFINITE_WAIT);
	add	esp,08h
	mov	edx,ebx

; 479     for( p = pSetting; p != NULL ; p = p->next )
	test	edi,edi
	je	@BLBL36
	xor	ebx,ebx
	mov	esi,edx
@BLBL37:

; 481 	if( strcmp(app, p->app) == 0 )
	mov	edx,[edi+08h]

; 479     for( p = pSetting; p != NULL ; p = p->next )
	mov	ecx,eax

; 481 	if( strcmp(app, p->app) == 0 )
	mov	[ebp-014h],ecx;	@CBE13
	call	strcmp
	mov	ecx,eax
	mov	eax,[ebp-014h];	@CBE13
	test	ecx,ecx
	jne	@BLBL38

; 483 	    if( key[0] == '\0'  ||  found_first == TRUE )
	cmp	byte ptr [esi],0h
	je	@BLBL39
	cmp	ebx,01h
	jne	@BLBL40
@BLBL39:
	mov	[ebp-014h],eax;	@CBE13

; 485 		strcpy(key, p->key);
	mov	edx,[edi+0ch]
	mov	eax,esi
	call	strcpy

; 486 		if( p->len > *bufsiz )
	mov	ecx,[ebp+010h];	bufsiz

; 485 		strcpy(key, p->key);
	mov	eax,[ebp-014h];	@CBE13

; 486 		if( p->len > *bufsiz )
	mov	edx,[edi+010h]
	cmp	[ecx],edx
	jae	@BLBL41

; 488 		    *bufsiz = p->len;
	mov	[ecx],edx

; 489 		    rc = (ULONG)-2;
	mov	dword ptr [ebp-010h],0fffffffeh;	rc
	jmp	@BLBL36
@BLBL41:

; 493 		    if( buf != NULL )
	mov	edx,[ebp+014h];	buf
	test	edx,edx
	je	@BLBL43
	mov	[ebp-014h],eax;	@CBE13
	mov	eax,edx

; 494 			memcpy(buf, p->data, p->len);
	mov	ecx,[edi+010h]
	mov	edx,[edi+014h]
	call	memcpy
	mov	eax,[ebp-014h];	@CBE13
@BLBL43:

; 495 		    *bufsiz = p->len;
	mov	ecx,[ebp+010h];	bufsiz
	mov	edx,[edi+010h]
	mov	[ecx],edx

; 496 		    rc = 0;
	mov	dword ptr [ebp-010h],0h;	rc

; 498 		break;
	jmp	@BLBL36
@BLBL40:
	mov	[ebp-014h],eax;	@CBE13

; 500 	    else if( strcmp(key, p->key) == NULL )
	mov	edx,[edi+0ch]
	mov	eax,esi
	call	strcmp
	mov	ecx,eax
	mov	eax,[ebp-014h];	@CBE13
	test	ecx,ecx
	jne	@BLBL38

; 501 		found_first = TRUE;
	mov	ebx,01h
@BLBL38:

; 479     for( p = pSetting; p != NULL ; p = p->next )
	mov	edi,[edi]
	test	edi,edi
	jne	@BLBL37
@BLBL36:

; 504     DosReleaseMutexSem(hmtxSetting);
	push	dword ptr  @2hmtxSetting
	call	DosReleaseMutexSem
	add	esp,04h

; 506     return rc;
	mov	eax,[ebp-010h];	rc
	add	esp,014h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ProfileReadNext	endp

; 403 {

	public ProfileRead
ProfileRead	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,ecx
	push	edi
	mov	[ebp+0ch],edx;	key
	push	esi
	mov	[ebp+08h],eax;	app
	sub	esp,010h

; 405     ULONG	rc = (ULONG)-1;
	mov	dword ptr [ebp-010h],0ffffffffh;	rc

; 410     DosRequestMutexSem( hmtxSetting, (ULONG)SEM_INDEFINITE_WAIT );
	push	0ffffffffh

; 403 {
	mov	[ebp+010h],ebx;	size

; 410     DosRequestMutexSem( hmtxSetting, (ULONG)SEM_INDEFINITE_WAIT );
	push	dword ptr  @2hmtxSetting
	call	DosRequestMutexSem

; 411     for( p = pSetting; p != NULL ; p = p->next )
	mov	edi,dword ptr  @3pSetting

; 410     DosRequestMutexSem( hmtxSetting, (ULONG)SEM_INDEFINITE_WAIT );
	add	esp,08h
	mov	ecx,ebx

; 411     for( p = pSetting; p != NULL ; p = p->next )
	test	edi,edi
	je	@BLBL29
	mov	esi,[ebp+014h];	buf
@BLBL30:

; 413 	if( strcmp(app, p->app) == 0  &&  strcmp(key, p->key) == NULL )
	mov	edx,[edi+08h]
	mov	eax,[ebp+08h];	app
	call	strcmp
	test	eax,eax
	jne	@BLBL31
	mov	edx,[edi+0ch]
	mov	eax,[ebp+0ch];	key
	call	strcmp
	test	eax,eax
	jne	@BLBL31

; 415 	    if( p->len > *size )
	mov	ecx,[edi+010h]
	cmp	[ebx],ecx
	jae	@BLBL32

; 417 		*size = p->len;
	mov	[ebx],ecx

; 418 		rc = (ULONG)-2;
	mov	dword ptr [ebp-010h],0fffffffeh;	rc
	jmp	@BLBL29
@BLBL32:

; 422 		if( buf != NULL )
	test	esi,esi
	je	@BLBL34

; 423 		    memcpy( buf, p->data, p->len );
	mov	ecx,[edi+010h]
	mov	edx,[edi+014h]
	mov	eax,esi
	call	memcpy
@BLBL34:

; 424 		*size = p->len;
	mov	eax,[edi+010h]
	mov	[ebx],eax

; 425 		rc = 0;
	mov	dword ptr [ebp-010h],0h;	rc

; 427 	    break;
	jmp	@BLBL29
@BLBL31:

; 411     for( p = pSetting; p != NULL ; p = p->next )
	mov	edi,[edi]
	test	edi,edi
	jne	@BLBL30
@BLBL29:

; 430     DosReleaseMutexSem( hmtxSetting );
	push	dword ptr  @2hmtxSetting
	call	DosReleaseMutexSem
	add	esp,04h

; 432     return rc;
	mov	eax,[ebp-010h];	rc
	add	esp,010h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ProfileRead	endp

; 336 {

	public ProfileWrite
ProfileWrite	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,ecx
	push	edi
	mov	[ebp+0ch],edx;	key
	push	esi
	mov	[ebp+08h],eax;	app
	sub	esp,010h
	mov	[ebp+010h],ebx;	bufsiz

; 344     DosRequestMutexSem( hmtxSetting, (ULONG)SEM_INDEFINITE_WAIT );
	push	0ffffffffh
	push	dword ptr  @2hmtxSetting
	call	DosRequestMutexSem

; 345     for( p = pSetting; p != NULL ; p = p->next )
	mov	edi,dword ptr  @3pSetting

; 344     DosRequestMutexSem( hmtxSetting, (ULONG)SEM_INDEFINITE_WAIT );
	add	esp,08h
	mov	ecx,ebx

; 345     for( p = pSetting; p != NULL ; p = p->next )
	mov	[ebp-010h],edi;	p
	test	edi,edi
	je	@BLBL21
	mov	esi,[ebp+014h];	buf
@BLBL22:

; 347 	if( strcmp(app, p->app) == 0  &&  strcmp(key, p->key) == NULL )
	mov	edx,[edi+08h]
	mov	eax,[ebp+08h];	app
	call	strcmp
	test	eax,eax
	jne	@BLBL23
	mov	edx,[edi+0ch]
	mov	eax,[ebp+0ch];	key
	call	strcmp
	test	eax,eax
	jne	@BLBL23

; 349 	    if( bufsiz == p->
; 349 len  &&  memcmp(buf, p->data, bufsiz) == 0 )
	cmp	[edi+010h],ebx
	jne	@BLBL24
	mov	edx,[edi+014h]
	mov	ecx,ebx
	mov	eax,esi
	call	memcmp
	test	eax,eax
	je	@BLBL56

; 350 		;
@BLBL24:

; 352 		free( p->data ), p->data = NULL;
	mov	eax,[edi+014h]
	call	free
	mov	dword ptr [edi+014h],0h

; 353 	    break;
	jmp	@BLBL56
@BLBL23:

; 345     for( p = pSetting; p != NULL ; p = p->next )
	mov	edi,[edi]
	test	edi,edi
	jne	@BLBL22
@BLBL56:
	mov	[ebp-010h],edi;	p
@BLBL21:

; 356     if( p == NULL )				/* not already in table? */
	cmp	dword ptr [ebp-010h],0h;	p
	jne	@BLBL27

; 358 	p = malloc( sizeof(SETTING) );
	mov	eax,018h
	call	malloc
	mov	ebx,eax

; 359 	p->next = pSetting;
	mov	ecx,dword ptr  @3pSetting

; 358 	p = malloc( sizeof(SETTING) );
	mov	[ebp-010h],ebx;	p

; 359 	p->next = pSetting;
	mov	[ebx],ecx

; 360 	p->app = strdup( app );
	mov	eax,[ebp+08h];	app
	call	strdup
	mov	[ebx+08h],eax

; 361 	p->key = strdup( key );
	mov	eax,[ebp+0ch];	key
	call	strdup
	mov	[ebx+0ch],eax

; 362 	p->data = NULL;
	mov	dword ptr [ebx+014h],0h

; 363 	pSetting = p;
	mov	dword ptr  @3pSetting,ebx
@BLBL27:

; 365     if( p->data == NULL )
	mov	ebx,[ebp-010h];	p
	cmp	dword ptr [ebx+014h],0h
	jne	@BLBL28

; 367 	p->changed = 1;
	mov	dword ptr [ebx+04h],01h

; 368 	p->data = malloc( bufsiz );
	mov	edi,[ebp+010h];	bufsiz
	mov	eax,edi
	call	malloc
	mov	ecx,edi
	mov	[ebx+014h],eax

; 369 	p->len = bufsiz;
	mov	[ebx+010h],ecx

; 370 	memcpy( p->data, buf, bufsiz );
	mov	edx,[ebp+014h];	buf
	mov	eax,[ebx+014h]
	call	memcpy

; 371 	Verbose(3, "Profile", "Saved key %s in profile", key);
	push	dword ptr [ebp+0ch];	key
	mov	ecx,offset FLAT:@STATc
	sub	esp,0ch
	mov	edx,offset FLAT:@STATb
	mov	eax,03h
	call	Verbose
	add	esp,010h
@BLBL28:

; 373     DosReleaseMutexSem( hmtxSetting );
	push	dword ptr  @2hmtxSetting
	call	DosReleaseMutexSem
	add	esp,04h

; 375     return 0;
	add	esp,010h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ProfileWrite	endp

; 275 {

	public ProfileClose
ProfileClose	proc
	push	ebx
	mov	ebx,eax
	push	edi

; 276     SETTING	*p = pSetting;
	mov	ecx,dword ptr  @3pSetting

; 275 {
	push	esi

; 276     SETTING	*p = pSetting;
	mov	esi,ecx

; 275 {
	sub	esp,014h
	mov	[esp+024h],ebx;	hab

; 277     HINI const	hd = PrfOpenProfile( hab, pszIniFile );
	push	dword ptr  @4pszIniFile
	push	ebx
	call	PrfOpenProfile
	add	esp,08h
	mov	edi,eax

; 280     if( hd == NULLHANDLE )
	test	eax,eax
	mov	eax,ebx
	jne	@BLBL18

; 282 	ULONG error = WinGetLastError( hab );
	push	eax
	call	WinGetLastError
	mov	ebx,eax

; 283 	Verbose(1, "Profile", "PrfOpenProfile fails - error %#x", error);
	push	ebx
	mov	ecx,offset FLAT:@STAT8
	sub	esp,0ch
	mov	edx,offset FLAT:@STAT7
	mov	eax,01h
	call	Verbose

; 284 	return error;
	add	esp,028h

; 283 	Verbose(1, "Profile", "PrfOpenProfile fails - error %#x", error);
	mov	eax,ebx

; 284 	return error;
	pop	esi
	pop	edi
	pop	ebx
	ret	
@BLBL18:

; 289     if( p != NULL )
	mov	ecx,esi
	test	ecx,ecx
	je	@BLBL19

; 291 	YankSetting( hab, hd, p );
	mov	edx,edi
	mov	eax,[esp+024h];	hab
	call	YankSetting

; 292 	p = NULL;
@BLBL19:

; 297     bool = PrfCloseProfile( hd );
	push	edi
	call	PrfCloseProfile
	mov	ebx,eax

; 298     free( pszIniFile ),	pszIniFile = NULL;
	mov	eax,dword ptr  @4pszIniFile
	call	free

; 299     DosCloseMutexSem( hmtxSetting ),	hmtxSetting = 0;
	push	dword ptr  @2hmtxSetting

; 298     free( pszIniFile ),	pszIniFile = NULL;
	mov	dword ptr  @4pszIniFile,0h

; 299     DosCloseMutexSem( hmtxSetting ),	hmtxSetting = 0;
	call	DosCloseMutexSem
	add	esp,08h
	mov	dword ptr  @2hmtxSetting,0h

; 301     if( bool == FALSE )
	test	ebx,ebx
	jne	@BLBL20

; 303 	ULONG error = WinGetLastError( hab );
	push	dword ptr [esp+024h];	hab
	call	WinGetLastError
	mov	ebx,eax

; 304 	Verbose(1, "Profile", "PrfCloseProfile fails - error %#x", error);
	push	ebx
	mov	ecx,offset FLAT:@STATa
	sub	esp,0ch
	mov	edx,offset FLAT:@STAT9
	mov	eax,01h
	call	Verbose

; 305 	return error;
	add	esp,028h

; 304 	Verbose(1, "Profile", "PrfCloseProfile fails - error %#x", error);
	mov	eax,ebx

; 305 	return error;
	pop	esi
	pop	edi
	pop	ebx
	ret	
@BLBL20:

; 307     return 0;
	xor	eax,eax
	add	esp,014h
	pop	esi
	pop	edi
	pop	ebx
	ret	
ProfileClose	endp

; 190 {

	public ProfileOpen
ProfileOpen	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	push	edi
	push	esi
	push	ecx
	mov	ecx,0fe4h
@BLBL57:
	sub	ecx,01000h
	mov	byte ptr [esp+ecx],0h
	cmp	ecx,0ffff9fe4h
	jg	@BLBL57
	pop	ecx

; 194     if( pszIniFile != NULL )
	mov	ecx,dword ptr  @4pszIniFile

; 190 {
	sub	esp,06028h
	mov	[ebp+0ch],edx;	filename
	mov	[ebp+08h],eax;	hab

; 194     if( pszIniFile != NULL )
	test	ecx,ecx
	je	@BLBL7

; 196 	Verbose(1, "Profile", "OpenProfile: %s already open", pszIniFile);
	push	ecx
	mov	ecx,offset FLAT:@STAT4
	sub	esp,0ch
	mov	edx,offset FLAT:@STAT3
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 197 	return (ULONG)-2;			/* only _one_ ini-file! */
	add	esp,06028h
	mov	eax,0fffffffeh
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL7:

; 199     pszIniFile = strdup( filename );
	mov	eax,[ebp+0ch];	filename
	call	strdup
	mov	dword ptr  @4pszIniFile,eax

; 200     if( pszIniFile == NULL )
	test	eax,eax
	jne	@BLBL8

; 202 	Verbose(1, "Profile", "OpenProfile: memory problem");
	mov	ecx,offset FLAT:@STAT6
	mov	edx,offset FLAT:@STAT5
	mov	eax,01h
	call	Verbose

; 203 	return (ULONG)-1;
	add	esp,06028h
	or	eax,0ffffffffh
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL8:

; 206     rc = DosCreateMutexSem( NULL, &hmtxSetting, 0, FALSE );
	push	0h

; 214 	bool = PrfQueryProfileData( hini, NULL, NULL, applist, &appcnt );
	lea	ebx,[ebp-018h];	appcnt

; 206     rc = DosCreateMutexSem( NULL, &hmtxSetting, 0, FALSE );
	push	0h
	push	offset FLAT:@2hmtxSetting
	push	0h
	call	DosCreateMutexSem

; 209 	HINI const hini = PrfOpenProfile( hab, (PSZ)filename );
	push	dword ptr [ebp+0ch];	filename

; 206     rc = DosCreateMutexSem( NULL, &hmtxSetting, 0, FALSE );
	mov	[ebp-020h],eax;	rc

; 209 	HINI const hini = PrfOpenProfile( hab, (PSZ)filename );
	push	dword ptr [ebp+08h];	hab
	call	PrfOpenProfile

; 214 	bool = PrfQueryProfileData( hini, NULL, NULL, applist, &appcnt );
	push	ebx
	lea	ebx,[ebp-02020h];	applist
	push	ebx

; 209 	HINI const hini = PrfOpenProfile( hab, (PSZ)filename );
	mov	[ebp-014h],eax;	hini

; 214 	bool = PrfQueryProfileData( hini, NULL, NULL, applist, &appcnt );
	push	0h

; 212 	ULONG	appcnt = sizeof(applist);
	mov	dword ptr [ebp-018h],02000h;	appcnt

; 214 	bool = PrfQueryProfileData( hini, NULL, NULL, applist, &appcnt );
	push	0h
	push	eax
	call	PrfQueryProfileData
	add	esp,02ch

; 215 	if( bool == TRUE )			/* OK? */
	cmp	eax,01h
	jne	@BLBL9

; 218 	    for(; *ap != '\0'; ap = NextWord(ap) )
	cmp	byte ptr [ebp-02020h],0h;	applist
	je	@BLBL9
@BLBL11:

; 223 		bool = PrfQueryProfileData( hini, ap, NULL, keylist, &keycnt );
	lea	eax,[ebp-01ch];	keycnt
	push	eax
	lea	edi,[ebp-04020h];	keylist
	push	edi
	mov	esi,[ebp-014h];	hini
	push	0h

; 221 		ULONG	keycnt = sizeof(keylist);
	mov	dword ptr [ebp-01ch],02000h;	keycnt

; 223 		bool = PrfQueryProfileData( hini, ap, NULL, keylist, &keycnt );
	push	ebx
	push	esi
	call	PrfQueryProfileData
	add	esp,014h

; 224 		if( bool == TRUE )
	cmp	eax,01h
	jne	@BLBL12

; 227 		    for(; *kp != '\0'; kp = NextWord(kp) )
	cmp	byte ptr [ebp-04020h],0h;	keylist
	je	@BLBL12
	mov	[ebp-06028h],ebx;	ap
@BLBL14:

; 232 			bool = PrfQueryProfileData( hini, ap, kp, value, &size );
	lea	eax,[ebp-010h];	size
	push	eax
	lea	edx,[ebp-06020h];	value
	push	edx

; 230 			ULONG size = sizeof(value);
	mov	dword ptr [ebp-010h],02000h;	size

; 232 			bool = PrfQueryProfileData( hini, ap, kp, value, &size );
	push	edi
	push	ebx
	push	esi
	call	PrfQueryProfileData
	add	esp,014h

; 233 			if( bool == TRUE )
	cmp	eax,01h
	jne	@BLBL15

; 235 			    SETTING * p = malloc( sizeof(SETTING) );
	mov	eax,018h
	call	malloc
	mov	ebx,eax

; 236 			    memset( p, 0, sizeof(SETTING) );
	mov	ecx,018h
	xor	edx,edx
	call	memset
	mov	[ebp-06024h],ebx;	@CBE14
	mov	ebx,[ebp-06028h];	ap

; 237 			    p->app = strdup(ap);
	mov	eax,ebx
	call	strdup
	mov	ecx,[ebp-06024h];	@CBE14
	mov	[ecx+08h],eax

; 238 			    p->key = strdup(kp);
	mov	eax,edi
	call	strdup
	mov	ecx,[ebp-06024h];	@CBE14
	mov	[ecx+0ch],eax

; 239 			    p->data = malloc(size);
	mov	eax,[ebp-010h];	size
	call	malloc
	mov	edx,[ebp-06024h];	@CBE14
	mov	[edx+014h],eax

; 240 			    p->len = size;
	mov	ecx,[ebp-010h];	size
	mov	[edx+010h],ecx

; 241 			    memcpy( p->data, value, size );
	mov	eax,[edx+014h]
	mov	ecx,[ebp-010h];	size
	lea	edx,[ebp-06020h];	value
	call	memcpy
	mov	ecx,[ebp-06024h];	@CBE14

; 242 			    p->changed = 0;
	mov	dword ptr [ecx+04h],0h

; 243 			    p->next = pSetting;
	mov	edx,dword ptr  @3pSetting
	mov	[ecx],edx

; 244 			    pSetting = p;
	mov	dword ptr  @3pSetting,ecx
@BLBL15:

; 227 		    for(; *kp != '\0'; kp = NextWord(kp) )
	mov	eax,edi
	call	NextWord
	mov	edi,eax
	cmp	byte ptr [edi],0h
	jne	@BLBL14
@BLBL12:

; 218 	    for(; *ap != '\0'; ap = NextWord(ap) )
	mov	eax,ebx
	call	NextWord
	mov	ebx,eax
	cmp	byte ptr [ebx],0h
	jne	@BLBL11
@BLBL9:

; 250 	PrfCloseProfile( hini );
	push	dword ptr [ebp-014h];	hini
	call	PrfCloseProfile
	add	esp,04h

; 253     return rc;
	mov	eax,[ebp-020h];	rc
	add	esp,06028h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ProfileOpen	endp

; 112 {

YankSetting	proc
	push	ebx
	sub	esp,0ch
	mov	[esp+01ch],ecx;	p
	mov	[esp+018h],edx;	hd
	mov	[esp+014h],eax;	hab

; 116     if( p->next != NULL )
	mov	ecx,[ecx]
	test	ecx,ecx
	je	@BLBL1

; 117 	YankSetting(hab, hd, p->next);
	call	YankSetting
@BLBL1:

; 119     if( p->changed != 0 )
	mov	ecx,[esp+01ch];	p
	cmp	dword ptr [ecx+04h],0h
	je	@BLBL2

; 123 	bool = PrfWriteProfileData(hd, p->app, p->key, p->data, p->len);
	push	dword ptr [ecx+010h]
	push	dword ptr [ecx+014h]
	push	dword ptr [ecx+0ch]
	push	dword ptr [ecx+08h]
	push	dword ptr [esp+028h];	hd
	call	PrfWriteProfileData
	add	esp,014h

; 124 	if( bool == FALSE )
	test	eax,eax
	jne	@BLBL2

; 125 	    Verbose(1, "Profile", "PrfWriteProfileData fails - error %#x",
	push	dword ptr [esp+014h];	hab
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@STAT2
	sub	esp,0ch
	mov	edx,offset FLAT:@STAT1
	mov	eax,01h
	call	Verbose
	add	esp,014h
@BLBL2:

; 129     free(p->app), free(p->key), free(p->data);
	mov	ebx,[esp+01ch];	p
	mov	eax,[ebx+08h]
	call	free
	mov	eax,[ebx+0ch]
	call	free
	mov	eax,[ebx+014h]
	call	free
	mov	eax,ebx

; 130     free(p);
	call	free

; 131     return 0;
	add	esp,0ch
	xor	eax,eax
	pop	ebx
	ret	
YankSetting	endp
CODE32	ends
end
