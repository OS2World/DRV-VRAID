	title	other.cpp
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
	extrn	WinQuerySwitchHandle:proc
	extrn	_getpid:proc
	extrn	WinQuerySwitchEntry:proc
	extrn	strcpy:proc
	extrn	WinChangeSwitchEntry:proc
	extrn	WinSetWindowText:proc
	extrn	_vsprintfieee:proc
	extrn	WinMessageBox:proc
	extrn	strlen:proc
	extrn	DosQueryPathInfo:proc
	extrn	_sprintfieee:proc
	extrn	DosCreateDir:proc
	extrn	Verbose:proc
	extrn	abort:proc
	extrn	_fltused:dword
CONST32_RO	segment
@CBE1	db "FATAL",0h
	align 04h
@CBE2	db "Error",0h
	align 04h
@CBE3	db "Warning",0h
@CBE4	db "Other",0h
@CBE5	db 0h
	align 04h
@CBE6	db "missing directory",0h
	align 04h
@CBE7	db "Directory %s not existen"
db "t! Do you want to create"
db " it?",0h
	align 04h
@CBE8	db "Directory",0h
	align 04h
@CBE9	db "Error %#x creating %s",0h
	align 04h
@CBE10	db "CheckValidDir",0h
	align 04h
@CBE11	db "DosQueryPathInfo(%s) - e"
db "rror %#x",0h
	align 04h
@CBE12	db "Error %#x checking %s",0h
	align 04h
@CBE13	db "DosQueryPathInfo(%s) - a"
db "ttribute %#x",0h
	align 04h
@CBE14	db "%s is not a directory",0h
	align 04h
@CBE15	db "%s _is_ a directory",0h
@CBE16	db "Condition: %s",0ah,"File: %s",0ah,"L"
db "ine: %u",0ah,"Abort program?",0h
	align 04h
@CBE17	db "Assertion failed!",0h
	align 04h
@1vcid	db "$Id: other.cpp,v 1.5 200"
db "0/04/10 01:05:08 vitus E"
db "xp $",0h
	align 04h
@17pszTitle	dd offset FLAT:@CBE1
	dd offset FLAT:@CBE2
	dd offset FLAT:@CBE3
	dd offset FLAT:@CBE4
	dd offset FLAT:@CBE5
CONST32_RO	ends
DATA32	segment
@19flStyle	db "G",0h,0h,0h
	db "G",0h,0h,0h
	db "'",0h,0h,0h
	db "7",0h,0h,0h
	db 07h,0h,0h,0h
DATA32	ends
CODE32	segment

; 226 _LNK_CONV _assert(const char * expr, const char * file, unsigned int line)

	public _assert
_assert	proc
	xchg	edx,ecx
	sub	esp,03e8h
	push	ebx

; 231     sprintf(str,
	push	edx
	mov	edx,offset FLAT:@CBE16
	push	ecx
	lea	ebx,[esp+0ch];	str
	push	eax
	mov	eax,ebx
	sub	esp,08h
	call	_sprintfieee

; 232 	    "Condition: %s\n"
; 233 	    "File: %s\n"
; 234 	    "Line: %u\n"
; 235 	    "Abort program?", expr, file, line);
; 236 
; 237     resp = WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, str, "Assertion failed!",
	push	044h
	push	0ffffffffh
	push	offset FLAT:@CBE17
	push	ebx
	push	01h
	push	01h
	call	WinMessageBox
	add	esp,02ch

; 238 			 -1ul, MB_ERROR|MB_YESNO);
; 239     if( resp == MBID_YES )
	cmp	ax,06h
	jne	@BLBL25

; 240 	abort();
	call	abort
@BLBL25:

; 241 }
	pop	ebx
	add	esp,03e8h
	ret	
_assert	endp

; 150 CheckValidDir(HWND const hwnd,char const * const suspect_dir)

	public CheckValidDir__FCUlCPCc
CheckValidDir__FCUlCPCc	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi
	mov	[ebp+0ch],edx;	suspect_dir
	push	esi

; 156     if( strlen(suspect_dir) == 0 )
	mov	eax,edx

; 150 CheckValidDir(HWND const hwnd,char const * const suspect_dir)
	sub	esp,0230h
	mov	[ebp+08h],ebx;	hwnd

; 156     if( strlen(suspect_dir) == 0 )
	call	strlen
	test	eax,eax
	mov	eax,ebx
	jne	@BLBL8

; 158 	MyMessageBox(1, hwnd, "missing directory");
	mov	edx,eax
	mov	ecx,offset FLAT:@CBE6
	mov	eax,01h
	call	MyMessageBox__FCiCUlPce

; 159 	return 1;
	add	esp,0230h
	mov	eax,01h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL8:

; 161     strcpy(path, suspect_dir);			/* further processing w/ path */
	lea	ebx,[ebp-0114h];	path
	mov	edx,[ebp+0ch];	suspect_dir
	mov	eax,ebx
	call	strcpy

; 162     if( strlen(path) == 1 )
	mov	eax,ebx
	call	strlen
	cmp	eax,01h
	je	@BLBL10

; 164     else if( strlen(path) == 3  &&  path[strlen(path)-2] == ':' )
	mov	eax,ebx
	call	strlen
	cmp	eax,03h
	jne	@BLBL11
	mov	eax,ebx
	call	strlen
	cmp	byte ptr [ebx+eax-02h],03ah
	je	@BLBL10

; 165 	;
@BLBL11:

; 166     else if( path[strlen(path)-1] == '\\'  ||  path[strlen(path)-1] == '/' )
	lea	ebx,[ebp-0114h];	path
	mov	eax,ebx
	call	strlen
	cmp	byte ptr [ebx+eax-01h],05ch
	je	@BLBL13
	mov	eax,ebx
	call	strlen
	cmp	byte ptr [ebx+eax-01h],02fh
	jne	@BLBL10
@BLBL13:

; 167 	path[strlen(path)-1] = '\0';
	lea	ebx,[ebp-0114h];	path
	mov	eax,ebx
	call	strlen
	mov	byte ptr [ebx+eax-01h],0h
@BLBL10:

; 169     rc = DosQueryPathInfo(path, FIL_STANDARD, (PVOID)&buf, sizeof(buf));
	push	018h
	lea	ebx,[ebp-012ch];	buf
	push	ebx
	lea	ebx,[ebp-0114h];	path
	push	01h
	push	ebx
	call	DosQueryPathInfo
	add	esp,010h
	mov	esi,eax

; 170     if( rc == ERROR_PATH_NOT_FOUND  || rc == ERROR_FILE_NOT_FOUND )
	cmp	eax,03h
	je	@BLBL15
	cmp	eax,02h
	jne	@BLBL16
@BLBL15:

; 175 	sprintf(str, "Directory %s not existent! Do you want to create it?",
	lea	ecx,[ebp-0114h];	path
	push	ecx
	lea	ebx,[ebp-0230h];	str
	sub	esp,08h
	mov	edx,offset FLAT:@CBE7
	mov	eax,ebx
	call	_sprintfieee

; 177 	resp = (USHORT)WinMessageBox(HWND_DESKTOP, hwnd,
	push	04h
	push	0h
	push	offset FLAT:@CBE8
	push	ebx
	push	dword ptr [ebp+08h];	hwnd
	push	01h
	call	WinMessageBox
	add	esp,024h

; 179 	if( resp == MBID_NO )
	cmp	ax,07h
	jne	@BLBL17

; 180 	    return 1;
	add	esp,0230h
	mov	eax,01h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL17:

; 181 	rc = DosCreateDir( path, NULL );
	push	0h
	lea	ebx,[ebp-0114h];	path
	push	ebx
	call	DosCreateDir
	add	esp,08h
	mov	esi,eax

; 182 	if( rc )
	test	eax,eax
	je	@BLBL18

; 184 	    MyMessageBox(1, hwnd, "Error %#x creating %s", rc, path);
	push	ebx
	mov	edx,[ebp+08h];	hwnd
	push	eax
	mov	ecx,offset FLAT:@CBE9
	sub	esp,0ch
	mov	eax,01h
	call	MyMessageBox__FCiCUlPce
	add	esp,014h

; 185 	    return 1;
	add	esp,0230h
	mov	eax,01h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL18:

; 187 	return 0;
	xor	eax,eax
	add	esp,0230h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL16:

; 189     else if( rc )
	mov	ebx,esi
	test	ebx,ebx
	je	@BLBL20

; 191 	Verbose(1, "CheckValidDir", "DosQueryPathInfo(%s) - error %#x", path, rc);
	push	ebx
	lea	edi,[ebp-0114h];	path
	push	edi
	mov	ecx,offset FLAT:@CBE11
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE10
	mov	eax,01h
	call	Verbose

; 192 	MyMessageBox(1, hwnd, "Error %#x checking %s", rc, path);
	push	edi
	mov	edx,[ebp+08h];	hwnd
	push	ebx
	mov	ecx,offset FLAT:@CBE12
	sub	esp,0ch
	mov	eax,01h
	call	MyMessageBox__FCiCUlPce
	add	esp,028h

; 193 	return 1;
	add	esp,0230h
	mov	eax,01h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL20:

; 195     else if( !(buf.attrFile & FILE_DIRECTORY) )
	mov	eax,[ebp-0118h];	buf
	test	al,010h
	jne	@BLBL19

; 197 	Verbose(2, "CheckValidDir", "DosQueryPathInfo(%s) - attribute %#x",
	push	eax
	lea	ebx,[ebp-0114h];	path
	push	ebx
	mov	ecx,offset FLAT:@CBE13
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE10
	mov	eax,02h
	call	Verbose

; 199 	MyMessageBox(1, hwnd, "%s is not a directory", path);
	push	ebx
	mov	edx,[ebp+08h];	hwnd
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE14
	mov	eax,01h
	call	MyMessageBox__FCiCUlPce
	add	esp,024h

; 200 	return 1;
	add	esp,0230h
	mov	eax,01h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL19:

; 202     Verbose(2, "CheckValidDir", "%s _is_ a directory", path);
	lea	ebx,[ebp-0114h];	path
	push	ebx
	mov	ecx,offset FLAT:@CBE15
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE10
	mov	eax,02h
	call	Verbose
	add	esp,010h

; 203     return 0;
	add	esp,0230h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
CheckValidDir__FCUlCPCc	endp

; 108 MyMessageBox(int const level,HWND const owner,char *fmt,...)

	public MyMessageBox__FCiCUlPce
MyMessageBox__FCiCUlPce	proc
	push	ebx
	mov	ebx,edx
	push	edi
	mov	edi,eax
	push	esi
	sub	esp,03f4h
	mov	[esp+040ch],ecx;	fmt

; 121     vsprintf(str, fmt, argp);
	lea	esi,[esp+0ch];	str
	mov	edx,[esp+040ch];	fmt
	lea	ecx,[esp+0410h];	fmt
	mov	eax,esi
	call	_vsprintfieee
	mov	eax,edi

; 124     WinMessageBox(HWND_DESKTOP, owner, str, (PSZ)pszTitle[level],
	push	dword ptr [eax*04h+@19flStyle]

; 121     vsprintf(str, fmt, argp);
	mov	edx,ebx

; 124     WinMessageBox(HWND_DESKTOP, owner, str, (PSZ)pszTitle[level],
	push	0ffffh
	push	dword ptr [eax*04h+@17pszTitle]
	push	esi
	push	edx
	push	01h
	call	WinMessageBox

; 126 }
	add	esp,040ch
	pop	esi
	pop	edi
	pop	ebx
	ret	
MyMessageBox__FCiCUlPce	endp

; 66 ChangeWindowTitle(HWND const hwndFrame,char const * const title)

	public ChangeWindowTitle__FCUlCPCc
ChangeWindowTitle__FCUlCPCc	proc
	push	ebx
	mov	ebx,edx
	push	edi
	push	esi
	sub	esp,06ch

; 71     if( (hswitch=WinQuerySwitchHandle(NULLHANDLE, getpid())) != NULLHANDLE )
	call	_getpid
	push	eax
	push	0h
	call	WinQuerySwitchHandle
	mov	edi,eax
	add	esp,08h
	test	edi,edi
	je	@BLBL3

; 77         WinQuerySwitchEntry(hswitch, &swctl);
	lea	esi,[esp+08h];	swctl

; 75         swctl.szSwtitle[0] = '\0';
	mov	byte ptr [esp+024h],0h;	swctl

; 77         WinQuerySwitchEntry(hswitch, &swctl);
	push	esi
	push	edi
	call	WinQuerySwitchEntry

; 78         hwndWindowHandle = swctl.hwnd;
	mov	ecx,[esp+010h];	swctl
	mov	[esp+070h],ecx;	hwndWindowHandle

; 80 	strcpy(swctl.szSwtitle, title);
	mov	edx,ebx
	lea	eax,[esp+02ch];	swctl
	call	strcpy

; 82         WinChangeSwitchEntry(hswitch, &swctl);
	push	esi
	push	edi
	call	WinChangeSwitchEntry
	mov	edx,ebx

; 83         WinSetWindowText(hwndWindowHandle, (PSZ)title);
	push	edx
	push	dword ptr [esp+07ch];	hwndWindowHandle
	call	WinSetWindowText
	add	esp,018h

; 84     }
@BLBL3:

; 86 }
	add	esp,06ch
	pop	esi
	pop	edi
	pop	ebx
	ret	
ChangeWindowTitle__FCUlCPCc	endp
CODE32	ends
end
