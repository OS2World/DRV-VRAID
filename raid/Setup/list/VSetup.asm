	title	VSetup.cpp
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
	public	fSetupMode
	public	szPrgName
	extrn	__vn__FUi:proc
	extrn	_sprintfieee:proc
	extrn	_vsprintfieee:proc
	extrn	DosWrite:proc
	extrn	strlen:proc
	extrn	WinPostMsg:proc
	extrn	DriverGetVersion__FPUsT1:proc
	extrn	DriverReadMessages__FPPUc:proc
	extrn	__vd__FPv:proc
	extrn	WinQueryWindowPtr:proc
	extrn	ProfileClose:proc
	extrn	WinQueryWindowPos:proc
	extrn	WinQueryWindow:proc
	extrn	ProfileWrite:proc
	extrn	WinSetWindowPos:proc
	extrn	_fprintfieee:proc
	extrn	WinSendMsg:proc
	extrn	WinWindowFromID:proc
	extrn	WinGetLastError:proc
	extrn	DriverSetBeeper__FUs:proc
	extrn	__nw__FUi:proc
	extrn	_beginthread:proc
	extrn	DrvsetupThread:proc
	extrn	_errno:proc
	extrn	WinDlgBox:proc
	extrn	DumpDialogProc:proc
	extrn	AboutDialogProc:proc
	extrn	WinDefWindowProc:proc
	extrn	_splitpath:proc
	extrn	_makepath:proc
	extrn	DosOpen:proc
	extrn	DosSetFilePtr:proc
	extrn	_exeentry:proc
	extrn	WinInitialize:proc
	extrn	WinCreateMsgQueue:proc
	extrn	WinRegisterClass:proc
	extrn	WinCreateStdWindow:proc
	extrn	WinCreateWindow:proc
	extrn	MyMessageBox__FCiCUlPce:proc
	extrn	WinSetWindowPtr:proc
	extrn	DriverOpen__Fv:proc
	extrn	PrfQueryProfileString:proc
	extrn	CheckValidDir__FCUlCPCc:proc
	extrn	strcpy:proc
	extrn	InilocDialogProc:proc
	extrn	PrfWriteProfileString:proc
	extrn	strcat:proc
	extrn	ProfileOpen:proc
	extrn	WinQueryTaskSizePos:proc
	extrn	ProfileRead:proc
	extrn	WinGetMsg:proc
	extrn	WinDispatchMsg:proc
	extrn	DriverClose__Fv:proc
	extrn	WinDestroyWindow:proc
	extrn	WinDestroyMsgQueue:proc
	extrn	WinTerminate:proc
	extrn	__dl__FPv:proc
	extrn	stderr:dword
	extrn	_fltused:dword
CONST32_RO	segment
@CBE1	db 0dh,"%s - ",0h
	align 04h
@CBE2	db "VSETUP",0h
	align 04h
@CBE3	db "Can't read driver versio"
db "n - error %lu",0h
	align 04h
@CBE4	db "Driver Version %u.%02u i"
db "nstalled",0h
	align 04h
@CBE5	db "Can't read driver messag"
db "es - error %lu",0h
	align 04h
@CBE6	db "VRAID",0h
	align 04h
@CBE7	db "Line %u: %s",0h
@CBE8	db "WM_CREATE",0h
	align 04h
@CBE9	db "WM_CLOSE",0h
	align 04h
@CBE10	db "WUM_START",0h
	align 04h
@CBE11	db "WM_SAVEAPPLICATION",0h
	align 04h
@CBE12	db "WM_SIZE",0h
@CBE13	db "MainWindow",0h
	align 04h
@CBE14	db "Size",0h
	align 04h
@CBE15	db "WM_MOVE",0h
@CBE16	db "Position",0h
	align 04h
@CBE17	db "mismatch: %lu of %lu imp"
db "orted",0ah,0h
	align 04h
@CBE18	db "hwndFrame %#x",0h
	align 04h
@CBE19	db "hwndMenu %#x",0h
	align 04h
@CBE20	db "VSetup",0h
	align 04h
@CBE21	db "WinEnableMenuItem failed"
db " (last error %#x)",0h
	align 04h
@CBE22	db "WM_COMMAND: %#x",0h
@CBE23	db "Couldn't create Drvsetup"
db "Thread - error %d",0h
	align 04h
@CBE24	db "created thread id %d",0h
	align 04h
@CBE25	db "WinDlgBox(IDW_DRVDUMP) r"
db "eturned %lu - last error"
db " %#x",0h
	align 04h
@CBE26	db "WinDlgBox(IDW_ABOUT) ret"
db "urned %lu - last error %"
db "#x",0h
@CBE27	db 0h
@CBE28	db "New log level %d",0h
	align 04h
@CBE29	db "unknown parameter '%c'",0h
	align 04h
@CBE30	db "VSETUP",09h,"main - memory pro"
db "blem",0ah,0h
	align 04h
@CBE31	db "WinInitialize failed!?!",0h
@CBE32	db "WinCreateMsgQueue - erro"
db "r %#x",0h
	align 04h
@CBE33	db "WinRegisterClass - error"
db " %#x",0h
	align 04h
@CBE34	db "VSetup - Main Window",0h
	align 04h
@CBE35	db "WinCreateStdWindow - err"
db "or %#x",0h
	align 04h
@CBE36	db "client window %u,%u %u,%"
db "u",0h
	align 04h
@CBE37	db "WinCreateWindow(...,WC_M"
db "LE,...) - error %#x",0h
@CBE38	db "WinSetWindowPtr failed -"
db " error %#",0h
	align 04h
@CBE39	db "INI Path",0h
	align 04h
@CBE40	db "WinDlgBox(IDW_INILOCATIO"
db "N) returned %lu - last e"
db "rror %#x",0h
	align 04h
@CBE41	db "Sorry, can't continue (n"
db "o directory to place ini"
db ")",0h
	align 04h
@CBE42	db "ini",0h
@CBE43	db "Copying old size of main"
db " window",0h
@CBE44	db "Copying old position of "
db "main window",0h
@1vcid	db "$Id: vsetup.cpp,v 1.14 2"
db "000/04/15 23:47:43 vitus"
db " Exp $",0h
	align 04h
@8szMleFirst	db "Startup",0h
CONST32_RO	ends
BSS32	segment
fSetupMode	dw 0h
	align 04h
szPrgName	db 0100h DUP (0h)
@4szPrgDir	db 0104h DUP (0h)
@5hwndClient	dd 0h
@6uLogLevel	dd 0h
@7hdLogFile	dd 0h
@35regular	dd 0h
BSS32	ends
DATA32	segment
@43lOffset	dd 08h
	dd	_exeentry
DATA32	ends
CODE32	segment

; 237 SetLogfile(HFILE newfile)

	public SetLogfile
SetLogfile	proc
	mov	ecx,eax

; 239     HFILE old = hdLogFile;
	mov	eax,dword ptr  @7hdLogFile

; 240     hdLogFile = newfile;
	mov	dword ptr  @7hdLogFile,ecx

; 241     return old;
	ret	
SetLogfile	endp

; 211 SetLoglevel(unsigned newlevel)

	public SetLoglevel
SetLoglevel	proc
	mov	ecx,eax

; 213     unsigned old = uLogLevel;
	mov	eax,dword ptr  @6uLogLevel

; 215     uLogLevel = newlevel;
	mov	dword ptr  @6uLogLevel,ecx

; 216     return old;
	ret	
SetLoglevel	endp

; 674 main(int argc,char *argv[])

	public main
main	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	[ebp+08h],eax;	argc
	push	edi
	mov	[ebp+0ch],edx;	argv
	push	esi

; 676     MAIN_WINDOW_DATA *windata = new MAIN_WINDOW_DATA;
	mov	eax,08h

; 674 main(int argc,char *argv[])
	sub	esp,05c4h

; 677     int		rc = 0;
	mov	dword ptr [ebp-014h],0h;	rc

; 676     MAIN_WINDOW_DATA *windata = new MAIN_WINDOW_DATA;
	call	__nw__FUi
	mov	edx,eax

; 684     SetLoglevel(1);
	mov	eax,01h
	call	SetLoglevel

; 676     MAIN_WINDOW_DATA *windata = new MAIN_WINDOW_DATA;
	mov	[ebp-010h],edx;	windata

; 685     if( windata == NULL )
	test	edx,edx
	jne	@BLBL54

; 687 	fprintf(stderr, "VSETUP\tmain - memory problem\n");
	mov	eax,dword ptr  stderr
	mov	edx,offset FLAT:@CBE30
	call	_fprintfieee

; 688 	return 1;
	add	esp,05c4h
	mov	eax,01h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL54:

; 690     if( ExamineArgs(argc, argv) != 0 )
	mov	edx,[ebp+0ch];	argv
	mov	eax,[ebp+08h];	argc
	call	ExamineArgs__FiPPc
	test	eax,eax
	je	@BLBL56

; 691 	return 2;
	add	esp,05c4h
	mov	eax,02h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL56:

; 695 	if( !(windata->hab = WinInitialize(0)) )
	push	0h
	call	WinInitialize
	add	esp,04h
	mov	ecx,[ebp-010h];	windata
	test	eax,eax
	mov	[ecx],eax
	jne	@BLBL57

; 697 	    Verbose(0, "VSETUP", "WinInitialize failed!?!");
	xor	eax,eax
	mov	ecx,offset FLAT:@CBE31
	mov	edx,offset FLAT:@CBE2
	call	Verbose

; 698 	    rc = 1;
	mov	dword ptr [ebp-014h],01h;	rc

; 699 	    break;
	jmp	@BLBL58
@BLBL57:

; 702 	if( !(hmq = WinCreateMsgQueue(windata->hab, 0)) )
	mov	ebx,[ebp-010h];	windata
	push	0h
	push	dword ptr [ebx]
	call	WinCreateMsgQueue
	add	esp,08h
	mov	[ebp-024h],eax;	hmq
	test	eax,eax
	jne	@BLBL59

; 704 	    Verbose(0, "VSETUP", "WinCreateMsgQueue - error %#x",
	push	dword ptr [ebx]

; 706 	    rc = 2;
	mov	dword ptr [ebp-014h],02h;	rc

; 704 	    Verbose(0, "VSETUP", "WinCreateMsgQueue - error %#x",
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE32
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE2
	xor	eax,eax
	call	Verbose
	add	esp,014h

; 707 	    break;
	jmp	@BLBL58
@BLBL59:

; 710 	if( !WinRegisterClass(windata->hab, WC_MAIN, WindowProc,
	push	04h
	mov	ebx,[ebp-010h];	windata
	push	01h
	push	offset FLAT: WindowProc
	push	offset FLAT:@CBE13
	push	dword ptr [ebx]
	call	WinRegisterClass
	add	esp,014h
	test	eax,eax
	jne	@BLBL60

; 713 	    Verbose(0, "VSETUP", "WinRegisterClass - error %#x",
	push	dword ptr [ebx]

; 715 	    rc = 3;
	mov	dword ptr [ebp-014h],03h;	rc

; 713 	    Verbose(0, "VSETUP", "WinRegisterClass - error %#x",
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE33
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE2
	xor	eax,eax
	call	Verbose
	add	esp,014h

; 716 	    break;
	jmp	@BLBL58
@BLBL60:

; 722 	hwndFrame = WinCreateStdWindow(HWND_DESKTOP, WS_ANIMATE, &flCreate,
	push	offset FLAT:@5hwndClient
	lea	ebx,[ebp-020h];	flCreate
	push	064h

; 719 	flCreate = FCF_SIZEBORDER | FCF_TASKLIST | FCF_SYSMENU | FCF_TITLEBAR
	mov	dword ptr [ebp-020h],0483fh;	flCreate

; 722 	hwndFrame = WinCreateStdWindow(HWND_DESKTOP, WS_ANIMATE, &flCreate,
	push	0h
	push	0h
	push	offset FLAT:@CBE34
	push	offset FLAT:@CBE13
	push	ebx
	push	0400000h
	push	01h
	call	WinCreateStdWindow
	add	esp,024h
	mov	[ebp-018h],eax;	hwndFrame

; 727 	if( !hwndFrame || !hwndClient )
	test	eax,eax
	je	@BLBL61
	cmp	dword ptr  @5hwndClient,0h
	jne	@BLBL62
@BLBL61:

; 729 	    Verbose(0, "VSETUP", "WinCreateStdWindow - error %#x",
	mov	ebx,[ebp-010h];	windata
	push	dword ptr [ebx]

; 731 	    rc = 4;				/* couldn't create window for some
	mov	dword ptr [ebp-014h],04h;	rc

; 729 	    Verbose(0, "VSETUP", "WinCreateStdWindow - error %#x",
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE35
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE2
	xor	eax,eax
	call	Verbose
	add	esp,014h

; 733 	    break;
	jmp	@BLBL58
@BLBL62:

; 742 	    WinQueryWindowPos(hwndClient, &size);
	lea	ebx,[ebp-078h];	size
	push	ebx
	push	dword ptr  @5hwndClient
	call	WinQueryWindowPos

; 743 	    Verbose(3, "VSETUP", "client window %u,%u %u,%u",
	push	dword ptr [ebp-074h];	size
	mov	ecx,offset FLAT:@CBE36
	push	dword ptr [ebp-070h];	size
	mov	edx,offset FLAT:@CBE2
	push	dword ptr [ebp-06ch];	size
	mov	eax,03h
	push	dword ptr [ebp-068h];	size
	sub	esp,0ch
	call	Verbose

; 746 	    windata->hwndMle = WinCreateWindow(hwndClient, WC_MLE, szMleFirst,
	push	0h
	mov	ebx,dword ptr  @5hwndClient
	push	0h
	push	062h
	push	03h
	push	ebx
	push	dword ptr [ebp-074h];	size
	push	dword ptr [ebp-070h];	size
	push	dword ptr [ebp-06ch];	size
	push	dword ptr [ebp-068h];	size
	push	08000001ch
	push	offset FLAT:@8szMleFirst
	push	0ffff000ah
	push	ebx
	call	WinCreateWindow
	add	esp,058h
	mov	ebx,[ebp-010h];	windata

; 753 	    if( windata->hwndMle == NULLHANDLE )
	test	eax,eax

; 746 	    windata->hwndMle = WinCreateWindow(hwndClient, WC_MLE, szMleFirst,
	mov	[ebx+04h],eax

; 753 	    if( windata->hwndMle == NULLHANDLE )
	jne	@BLBL63

; 755 		MyMessageBox(0, HWND_DESKTOP,
	push	dword ptr [ebx]

; 758 		rc = 5;
	mov	dword ptr [ebp-014h],05h;	rc

; 755 		MyMessageBox(0, HWND_DESKTOP,
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE37
	sub	esp,0ch
	mov	edx,01h
	xor	eax,eax
	call	MyMessageBox__FCiCUlPce
	add	esp,014h

; 759 		break;
	jmp	@BLBL58
@BLBL63:

; 762 	    bool = WinSetWindowPtr(hwndClient, 0, windata);
	mov	ebx,[ebp-010h];	windata
	push	ebx
	push	0h
	push	dword ptr  @5hwndClient
	call	WinSetWindowPtr
	add	esp,0ch

; 763 	    if( bool == FALSE )
	test	eax,eax
	jne	@BLBL64

; 765 		MyMessageBox(0, HWND_DESKTOP,
	push	dword ptr [ebx]

; 768 		rc = 6;
	mov	dword ptr [ebp-014h],06h;	rc

; 765 		MyMessageBox(0, HWND_DESKTOP,
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE38
	sub	esp,0ch
	mov	edx,01h
	xor	eax,eax
	call	MyMessageBox__FCiCUlPce
	add	esp,014h

; 769 		break;
	jmp	@BLBL58
@BLBL64:

; 771 	    WinPostMsg(windata->hwndMle, MLM_RESETUNDO, NULL, NULL);
	push	0h
	mov	ebx,[ebp-010h];	windata
	push	0h
	push	01cfh
	push	dword ptr [ebx+04h]
	call	WinPostMsg

; 777 	    HWND hwndMenu = WinWindowFromID(hwndFrame, FID_MENU);
	push	08005h
	push	dword ptr [ebp-018h];	hwndFrame
	call	WinWindowFromID

; 779 	    WinEnableMenuItem(hwndMenu, IDM_CUT, FALSE);
	push	040004000h

; 777 	    HWND hwndMenu = WinWindowFromID(hwndFrame, FID_MENU);
	mov	ebx,eax

; 779 	    WinEnableMenuItem(hwndMenu, IDM_CUT, FALSE);
	push	010079h

; 777 	    HWND hwndMenu = WinWindowFromID(hwndFrame, FID_MENU);
	mov	[ebp-028h],ebx;	hwndMenu

; 779 	    WinEnableMenuItem(hwndMenu, IDM_CUT, FALSE);
	push	0192h
	push	ebx
	call	WinSendMsg

; 780 	    WinEnableMenuItem(hwndMenu, IDM_PASTE, FALSE);
	push	040004000h
	push	01007bh
	push	0192h
	push	ebx
	call	WinSendMsg

; 783 	    WinEnableMenuItem(hwndMenu, IDM_FILE, FALSE);
	push	040004000h
	push	01006eh
	push	0192h
	push	ebx
	call	WinSendMsg

; 789 	    if( DriverOpen() != 0 )
	call	DriverOpen__Fv
	add	esp,048h
	test	eax,eax
	je	@BLBL65

; 791 		WinEnableMenuItem(hwndMenu, IDM_DRIVER, FALSE);
	push	040004000h
	push	010082h
	push	0192h
	push	ebx
	call	WinSendMsg
	add	esp,010h

; 792 	    }
	jmp	@BLBL66
@BLBL65:

; 798 		DriverGetVersion(&ver, &flags);
	lea	edx,[ebp-02eh];	flags

; 795 		fSetupMode |= SM_FLTAVAIL;
	or	byte ptr  fSetupMode,01h

; 798 		DriverGetVersion(&ver, &flags);
	lea	eax,[ebp-030h];	ver
	call	DriverGetVersion__FPUsT1

; 799 		if( (flags & 0x20) )
	test	byte ptr [ebp-02eh],020h;	flags
	je	@BLBL66

; 800 		    WinCheckMenuItem(hwndMenu,IDM_NOBEEP, 1);
	push	020002000h
	push	010085h
	push	0192h
	push	dword ptr [ebp-028h];	hwndMenu
	call	WinSendMsg
	add	esp,010h

; 801 	    }
@BLBL66:

; 814 	    length = PrfQueryProfileString(HINI_USERPROFILE,
	push	0104h
	lea	ebx,[ebp-01a0h];	path
	push	ebx
	push	0h
	push	offset FLAT:@CBE39
	push	offset FLAT:@CBE20
	push	0ffffffffh
	call	PrfQueryProfileString
	add	esp,018h
	mov	edx,ebx

; 820 	    if( length == 0  ||  CheckValidDir(hwndFrame, path) )
	test	eax,eax
	je	@BLBL68
	mov	eax,[ebp-018h];	hwndFrame
	call	CheckValidDir__FCUlCPCc
	test	eax,eax
	je	@BLBL69
@BLBL68:

; 824 		strcpy(path, szPrgDir);		/* default value */
	lea	ebx,[ebp-01a0h];	path
	mov	edx,offset FLAT:@4szPrgDir
	mov	eax,ebx
	call	strcpy

; 825 		result = WinDlgBox(HWND_DESKTOP, hwndFrame, InilocDialogProc,
	push	ebx

; 827 		Verbose(2, "VSETUP", "WinDlgBox(IDW_INILOCATION) returned %lu "
	mov	edi,offset FLAT:@CBE2

; 825 		result = WinDlgBox(HWND_DESKTOP, hwndFrame, InilocDialogProc,
	push	0258h
	push	0h
	push	offset FLAT: InilocDialogProc
	push	dword ptr [ebp-018h];	hwndFrame
	push	01h
	call	WinDlgBox

; 827 		Verbose(2, "VSETUP", "WinDlgBox(IDW_INILOCATION) returned %lu "
	mov	ecx,[ebp-010h];	windata

; 825 		result = WinDlgBox(HWND_DESKTOP, hwndFrame, InilocDialogProc,
	mov	ebx,eax

; 827 		Verbose(2, "VSETUP", "WinDlgBox(IDW_INILOCATION) returned %lu "
	push	dword ptr [ecx]
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE40
	push	ebx
	mov	edx,edi
	sub	esp,0ch
	mov	eax,02h
	call	Verbose
	add	esp,030h
	mov	edx,edi

; 829 		if( result != DID_OK )
	cmp	ebx,01h
	je	@BLBL70

; 831 		    Verbose(0, "VSETUP",
	xor	eax,eax
	mov	ecx,offset FLAT:@CBE41
	call	Verbose

; 833 		    return 4;
	add	esp,05c4h
	mov	eax,04h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL70:

; 838 		PrfWriteProfileString(HINI_USERPROFILE,
	lea	ebx,[ebp-01a0h];	path
	push	ebx
	push	offset FLAT:@CBE39
	push	offset FLAT:@CBE20
	push	0ffffffffh
	call	PrfWriteProfileString
	add	esp,010h

; 840 	    }
@BLBL69:

; 842 	    _splitpath(path, drv, dir, fname, ext);
	lea	esi,[ebp-04c0h];	ext
	push	esi
	lea	edi,[ebp-03c0h];	fname
	push	edi
	lea	ebx,[ebp-02c0h];	dir
	sub	esp,0ch
	lea	edi,[ebp-02ch];	drv
	mov	ecx,ebx
	mov	edx,edi
	lea	eax,[ebp-01a0h];	path
	call	_splitpath
	lea	edx,[ebp-03c0h];	fname

; 843 	    strcat(dir, fname ),	strcat( dir, ext);
	mov	eax,ebx
	call	strcat
	mov	edx,esi
	mov	eax,ebx
	call	strcat

; 844 	    _makepath(inifile, drv, dir, szPrgName, "ini");
	push	offset FLAT:@CBE42

; 843 	    strcat(dir, fname ),	strcat( dir, ext);
	mov	edx,edi

; 844 	    _makepath(inifile, drv, dir, szPrgName, "ini");
	push	offset FLAT:szPrgName

; 843 	    strcat(dir, fname ),	strcat( dir, ext);
	mov	ecx,ebx

; 844 	    _makepath(inifile, drv, dir, szPrgName, "ini");
	sub	esp,0ch

; 845 	    ProfileOpen(windata->hab, inifile);
	mov	edi,[ebp-010h];	windata

; 844 	    _makepath(inifile, drv, dir, szPrgName, "ini");
	lea	ebx,[ebp-05c4h];	inifile
	mov	eax,ebx
	call	_makepath

; 845 	    ProfileOpen(windata->hab, inifile);
	mov	eax,[edi]

; 844 	    _makepath(inifile, drv, dir, szPrgName, "ini");
	mov	edx,ebx

; 845 	    ProfileOpen(windata->hab, inifile);
	call	ProfileOpen

; 854 	    WinQueryTaskSizePos(windata->hab, 0, &swp); /* initialize 'swp' */
	lea	ecx,[ebp-054h];	swp
	push	ecx
	push	0h
	push	dword ptr [edi]

; 857 	    if( ProfileRead(WC_MAIN, "Size", &cb, &tmp) == 0 )
	lea	ebx,[ebp-09ch];	tmp

; 854 	    WinQueryTaskSizePos(windata->hab, 0, &swp); /* initialize 'swp' */
	call	WinQueryTaskSizePos

; 857 	    if( ProfileRead(WC_MAIN, "Size", &cb, &tmp) == 0 )
	push	ebx

; 856 	    cb = sizeof(tmp);
	mov	dword ptr [ebp-01ch],024h;	cb

; 857 	    if( ProfileRead(WC_MAIN, "Size", &cb, &tmp) == 0 )
	sub	esp,0ch
	lea	ecx,[ebp-01ch];	cb
	mov	edx,offset FLAT:@CBE14
	mov	eax,offset FLAT:@CBE13
	call	ProfileRead
	add	esp,044h
	test	eax,eax
	jne	@BLBL71

; 859 		Verbose(3, "VSETUP", "Copying old size of main window");
	mov	ecx,offset FLAT:@CBE43
	mov	edx,offset FLAT:@CBE2
	mov	eax,03h
	call	Verbose

; 860 		swp.cx = tmp.cx;
	mov	eax,[ebp-094h];	tmp
	mov	[ebp-04ch],eax;	swp

; 861 		swp.cy = tmp.cy;
	mov	eax,[ebp-098h];	tmp

; 862 		swp.fl |= SWP_SIZE;
	or	byte ptr [ebp-054h],01h;	swp

; 861 		swp.cy = tmp.cy;
	mov	[ebp-050h],eax;	swp

; 863 	    }
@BLBL71:

; 865 	    if( ProfileRead(WC_MAIN, "Position", &cb, &tmp) == 0 )
	lea	ebx,[ebp-09ch];	tmp
	push	ebx

; 864 	    cb = sizeof(tmp);
	mov	dword ptr [ebp-01ch],024h;	cb

; 865 	    if( ProfileRead(WC_MAIN, "Position", &cb, &tmp) == 0 )
	sub	esp,0ch
	lea	ecx,[ebp-01ch];	cb
	mov	edx,offset FLAT:@CBE16
	mov	eax,offset FLAT:@CBE13
	call	ProfileRead
	add	esp,010h
	test	eax,eax
	jne	@BLBL72

; 867 		Verbose(3, "VSETUP", "Copying old position of main window");
	mov	ecx,offset FLAT:@CBE44
	mov	edx,offset FLAT:@CBE2
	mov	eax,03h
	call	Verbose

; 868 		swp.x = tmp.x;
	mov	eax,[ebp-08ch];	tmp
	mov	[ebp-044h],eax;	swp

; 869 		swp.y = tmp.y;
	mov	eax,[ebp-090h];	tmp

; 870 		swp.fl |= SWP_MOVE;
	or	byte ptr [ebp-054h],02h;	swp

; 869 		swp.y = tmp.y;
	mov	[ebp-048h],eax;	swp

; 871 	    }
@BLBL72:

; 872 	    WinSetWindowPos(hwndFrame, HWND_TOP, swp.x, swp.y, swp.cx, swp.cy,
	push	08bh

; 880 	while( WinGetMsg(windata->hab, &qmsg, 0L, 0, 0) )
	mov	ebx,[ebp-010h];	windata

; 872 	    WinSetWindowPos(hwndFrame, HWND_TOP, swp.x, swp.y, swp.cx, swp.cy,
	push	dword ptr [ebp-050h];	swp
	push	dword ptr [ebp-04ch];	swp
	push	dword ptr [ebp-048h];	swp
	push	dword ptr [ebp-044h];	swp
	push	03h
	push	dword ptr [ebp-018h];	hwndFrame
	call	WinSetWindowPos

; 879 	bool = WinPostMsg(hwndClient, WUM_START, 0, 0);
	push	0h
	push	0h
	push	01002h
	push	dword ptr  @5hwndClient
	call	WinPostMsg

; 880 	while( WinGetMsg(windata->hab, &qmsg, 0L, 0, 0) )
	push	0h
	lea	ecx,[ebp-01c0h];	qmsg
	push	0h
	push	0h
	push	ecx
	push	dword ptr [ebx]
	call	WinGetMsg
	add	esp,040h
	test	eax,eax
	je	@BLBL73
	mov	edi,ebx
@BLBL74:

; 881 	    WinDispatchMsg(windata->hab, &qmsg);
	lea	ebx,[ebp-01c0h];	qmsg
	push	ebx
	push	dword ptr [edi]
	call	WinDispatchMsg

; 880 	while( WinGetMsg(windata->hab, &qmsg, 0L, 0, 0) )
	push	0h
	push	0h
	push	0h
	push	ebx
	push	dword ptr [edi]
	call	WinGetMsg
	add	esp,01ch
	test	eax,eax
	jne	@BLBL74
@BLBL73:

; 889 	WinTerminate(windata->hab);
	mov	esi,[ebp-010h];	windata

; 885 	DriverClose();				/* ignore errors */
	call	DriverClose__Fv

; 887 	WinDestroyWindow(hwndFrame);
	push	dword ptr [ebp-018h];	hwndFrame
	call	WinDestroyWindow

; 888 	WinDestroyMsgQueue(hmq);
	push	dword ptr [ebp-024h];	hmq
	call	WinDestroyMsgQueue

; 889 	WinTerminate(windata->hab);
	push	dword ptr [esi]
	call	WinTerminate
	mov	eax,esi

; 890 	delete windata;
	call	__dl__FPv
	add	esp,0ch

; 892     while(0);
@BLBL58:

; 894     return rc;
	mov	eax,[ebp-014h];	rc
	add	esp,05c4h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
main	endp

; 594 ExamineArgs(int argc,char *argv[])

ExamineArgs__FiPPc	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,edx
	push	edi
	mov	[ebp+0ch],ebx;	argv
	push	esi
	mov	esi,eax
	sub	esp,0218h

; 599     _splitpath(argv[0], drv, dir, szPrgName, ext);
	lea	ecx,[ebp-021ch];	ext
	push	ecx
	lea	eax,[ebp-014h];	drv
	push	offset FLAT:szPrgName

; 594 ExamineArgs(int argc,char *argv[])
	mov	[ebp+08h],esi;	argc

; 599     _splitpath(argv[0], drv, dir, szPrgName, ext);
	sub	esp,0ch
	mov	edx,eax
	lea	edi,[ebp-011ch];	dir
	mov	eax,[ebx]
	mov	ecx,edi
	call	_splitpath
	mov	ecx,edi

; 600     _makepath(szPrgDir, drv, dir, "", "");
	mov	edi,offset FLAT:@CBE27
	push	edi

; 599     _splitpath(argv[0], drv, dir, szPrgName, ext);
	lea	edx,[ebp-014h];	drv

; 600     _makepath(szPrgDir, drv, dir, "", "");
	push	edi
	mov	eax,offset FLAT:@4szPrgDir
	sub	esp,0ch
	call	_makepath
	mov	eax,esi
	add	esp,028h
	mov	edx,ebx

; 602     while( argc > 1  &&  argv[1][0] == '-' )
	cmp	eax,01h
	jle	@BLBL42
	mov	ecx,[edx+04h]
	cmp	byte ptr [ecx],02dh
	jne	@BLBL42
	mov	esi,[ebp-01ch];	newlevel
	mov	edi,eax
@BLBL43:

; 604 	switch( argv[1][1] )
	mov	eax,[edx+04h]
	movzx	eax,byte ptr [eax+01h]
	cmp	eax,06ch
	je	@BLBL84
	cmp	eax,071h
	je	@BLBL82
	cmp	eax,076h
	je	@BLBL83
	jmp	@BLBL85
@BLBL82:

; 607 	    SetLoglevel(VL_FATAL);
	xor	eax,eax
	call	SetLoglevel

; 608 	    break;
	jmp	@BLBL81
@BLBL83:

; 611 	    if( argv[1][2] == ':' )
	mov	eax,[edx+04h]
	cmp	byte ptr [eax+02h],03ah
	jne	@BLBL44
	mov	ebx,edx

; 613 		int newlevel = argv[1][3] - '0';
	movzx	esi,byte ptr [eax+03h]
	sub	esi,030h

; 615 		Verbose(0, "VSETUP", "New log level %d", newlevel);
	push	esi
	mov	ecx,offset FLAT:@CBE28
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE2
	xor	eax,eax
	call	Verbose

; 616 		SetLoglevel(newlevel);
	mov	eax,esi
	call	SetLoglevel

; 615 		Verbose(0, "VSETUP", "New log level %d", newlevel);
	mov	edx,ebx

; 616 		SetLoglevel(newlevel);
	add	esp,010h

; 617 	    }
	jmp	@BLBL45
@BLBL44:

; 619 		SetLoglevel(2);
	mov	eax,02h
	call	SetLoglevel
@BLBL45:

; 620 	    break;
	jmp	@BLBL81
@BLBL84:

; 623 	    if( argv[1][2] == ':' )
	mov	eax,[edx+04h]
	cmp	byte ptr [eax+02h],03ah
	jne	@BLBL46

; 629 		rc = DosOpen(&argv[1][3], &hf, &ul,
	push	0h
	lea	ebx,[ebp-018h];	ul
	push	06121h
	lea	ecx,[ebp-010h];	hf
	push	011h
	add	eax,03h
	push	0h

; 623 	    if( argv[1][2] == ':' )
	mov	[ebp-0220h],edx;	@CBE50

; 629 		rc = DosOpen(&argv[1][3], &hf, &ul,
	push	0h
	push	ebx
	push	ecx
	push	eax
	call	DosOpen
	mov	edx,[ebp-0220h];	@CBE50
	add	esp,020h
	mov	ecx,ebx

; 636 		if( rc == 0 )
	test	eax,eax
	jne	@BLBL46

; 638 		    DosSetFilePtr(hf, 0, FILE_END, &ul);
	push	ecx

; 636 		if( rc == 0 )
	mov	ebx,edx

; 638 		    DosSetFilePtr(hf, 0, FILE_END, &ul);
	push	02h
	push	0h
	push	dword ptr [ebp-010h];	hf
	call	DosSetFilePtr

; 639 		    SetLogfile(hf);
	mov	eax,[ebp-010h];	hf
	call	SetLogfile

; 638 		    DosSetFilePtr(hf, 0, FILE_END, &ul);
	mov	edx,ebx

; 639 		    SetLogfile(hf);
	add	esp,010h

; 641 	    }
@BLBL46:

; 642 	    break;
	jmp	@BLBL81
@BLBL85:

; 645 	    Verbose(0, "VSETUP", "unknown parameter \'%c\'", argv[1][1]);
	mov	eax,[edx+04h]
	mov	ecx,offset FLAT:@CBE29
	movzx	eax,byte ptr [eax+01h]
	push	eax
	mov	edx,offset FLAT:@CBE2
	sub	esp,0ch
	xor	eax,eax
	call	Verbose
	add	esp,010h

; 646 	    return 1;
	add	esp,0218h
	mov	eax,01h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL81:

; 648 	--argc;
	dec	edi

; 649 	++argv;
	add	edx,04h

; 602     while( argc > 1  &&  argv[1][0] == '-' )
	cmp	edi,01h
	jle	@BLBL42
	mov	eax,[edx+04h]
	cmp	byte ptr [eax],02dh
	je	@BLBL43
@BLBL42:

; 651     return 0;
	xor	eax,eax
	add	esp,0218h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ExamineArgs__FiPPc	endp

; 306 WindowProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)

WindowProc	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	push	edi
	push	esi
	sub	esp,068h

; 308     MAIN_WINDOW_DATA * const windata = (PMAIN_WINDOW_DATA)WinQueryWindowPtr(hwnd, 0);
	push	0h
	push	dword ptr [ebp+08h];	hwnd
	call	WinQueryWindowPtr

; 312     switch( msg )
	mov	ecx,[ebp+0ch];	msg

; 308     MAIN_WINDOW_DATA * const windata = (PMAIN_WINDOW_DATA)WinQueryWindowPtr(hwnd, 0);
	add	esp,08h
	mov	esi,eax

; 312     switch( msg )
	cmp	ecx,029h
	jl	@BLBL97
	je	@BLBL88
	cmp	ecx,01001h
	jl	@BLBL98
	je	@BLBL93
	cmp	ecx,01002h
	je	@BLBL89
	cmp	ecx,01003h
	je	@BLBL94
	jmp	@BLBL86
@BLBL98:
	cmp	ecx,03eh
	je	@BLBL90
	cmp	ecx,04fh
	je	@BLBL96
	jmp	@BLBL86
@BLBL97:
	cmp	ecx,06h
	jl	@BLBL99
	je	@BLBL92
	cmp	ecx,07h
	je	@BLBL91
	cmp	ecx,020h
	je	@BLBL95
	jmp	@BLBL86
@BLBL99:
	cmp	ecx,01h
	jne	@BLBL86
@BLBL87:

; 316 	Verbose(3, "VSETUP", "WM_CREATE");
	mov	ecx,offset FLAT:@CBE8
	mov	edx,offset FLAT:@CBE2
	mov	eax,03h
	call	Verbose

; 317 	break;
	jmp	@BLBL86
@BLBL88:

; 324 	Verbose(3, "VSETUP", "WM_CLOSE");
	mov	ecx,offset FLAT:@CBE9
	mov	edx,offset FLAT:@CBE2
	mov	eax,03h
	call	Verbose

; 325 	break;					/* continue... */
	jmp	@BLBL86
@BLBL89:

; 345 	Verbose(3, "VSETUP", "WUM_START");
	mov	ecx,offset FLAT:@CBE10
	mov	edx,offset FLAT:@CBE2
	mov	eax,03h
	call	Verbose

; 347 	return 0;
	add	esp,068h

; 346 	regular = TRUE;
	mov	dword ptr  @35regular,01h

; 347 	return 0;
	pop	esi
	xor	eax,eax
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL90:

; 354 	Verbose(3, "VSETUP", "WM_SAVEAPPLICATION");
	mov	ecx,offset FLAT:@CBE11
	mov	edx,offset FLAT:@CBE2
	mov	eax,03h
	call	Verbose

; 355 	ProfileClose(windata->hab);		/* there will be no more */
	mov	eax,esi
	mov	eax,[eax]
	call	ProfileClose

; 356 	return NULL;
	add	esp,068h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL91:

; 364 	Verbose(3, "VSETUP", "WM_SIZE");
	mov	ecx,offset FLAT:@CBE12
	mov	edx,offset FLAT:@CBE2
	mov	eax,03h
	call	Verbose

; 368 	    if( windata != NULL )
	cmp	esi,0h
	je	@BLBL24

; 370 		if( regular == TRUE
	cmp	dword ptr  @35regular,01h
	jne	@BLBL25
	push	05h
	lea	ebx,[ebp-044h];	size
	push	dword ptr [ebp+08h];	hwnd
	call	WinQueryWindow
	push	ebx
	push	eax
	call	WinQueryWindowPos
	add	esp,010h
	cmp	eax,01h
	jne	@BLBL25

; 373 		    ProfileWrite(WC_MAIN, "Size", sizeof(size), &size);
	push	ebx
	mov	ecx,024h
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE14
	mov	eax,offset FLAT:@CBE13
	call	ProfileWrite
	add	esp,010h
@BLBL25:

; 375 		if( WinQueryWindowPos(hwnd, &size) == TRUE )
	lea	ebx,[ebp-044h];	size
	push	ebx
	push	dword ptr [ebp+08h];	hwnd
	call	WinQueryWindowPos
	add	esp,08h
	cmp	eax,01h
	jne	@BLBL24

; 376 		    WinSetWindowPos(windata->hwndMle, HWND_TOP,
	push	03h
	mov	ebx,esi
	push	dword ptr [ebp-040h];	size
	push	dword ptr [ebp-03ch];	size
	push	0h
	push	0h
	push	03h
	push	dword ptr [ebx+04h]
	call	WinSetWindowPos
	add	esp,01ch

; 378 	    }
@BLBL24:

; 380 	break;
	jmp	@BLBL86
@BLBL92:

; 387 	Verbose(3, "VSETUP", "WM_MOVE");
	mov	ecx,offset FLAT:@CBE15
	mov	edx,offset FLAT:@CBE2
	mov	eax,03h
	call	Verbose

; 391 	    if( regular == TRUE
	cmp	dword ptr  @35regular,01h
	jne	@BLBL27
	push	05h
	lea	ebx,[ebp-068h];	swp
	push	dword ptr [ebp+08h];	hwnd
	call	WinQueryWindow
	push	ebx
	push	eax
	call	WinQueryWindowPos
	add	esp,010h
	cmp	eax,01h
	jne	@BLBL27

; 395 		ProfileWrite(WC_MAIN, "Position", sizeof(swp), &swp);
	push	ebx
	mov	ecx,024h
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE16
	mov	eax,offset FLAT:@CBE13
	call	ProfileWrite
	add	esp,010h

; 396 	    }
@BLBL27:

; 398 	return 0;
	xor	eax,eax
	add	esp,068h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL93:

; 411 	    char * str = (char *)PVOIDFROMMP(mp1);
	mov	ebx,[ebp+010h];	mp1

; 414 	    ULONG const cbBytes = strlen(str);
	mov	eax,ebx
	call	strlen

; 411 	    char * str = (char *)PVOIDFROMMP(mp1);
	mov	[ebp-014h],ebx;	str

; 414 	    ULONG const cbBytes = strlen(str);
	mov	edx,ebx
	mov	[ebp-018h],eax;	cbBytes

; 417 	    if( windata == NULL )
	cmp	esi,0h
	jne	@BLBL28

; 418 		fprintf(stderr, str);		/* no MLE created */
	mov	eax,dword ptr  stderr
	call	_fprintfieee
	jmp	@BLBL29
@BLBL28:

; 421 		bool = (BOOL)WinSendMsg(windata->hwndMle, MLM_SETIMPORTEXPORT,
	mov	bx,[ebp-018h];	cbBytes
	movzx	ebx,bx
	push	ebx
	mov	ebx,esi
	push	dword ptr [ebp+010h];	mp1
	push	01c2h
	push	dword ptr [ebx+04h]
	call	WinSendMsg
	add	esp,010h

; 423 		if( bool == FALSE )
	test	eax,eax
	jne	@BLBL30

; 424 		    fprintf(stderr, str);
	mov	edx,[ebp-014h];	str
	mov	eax,dword ptr  stderr
	call	_fprintfieee
	jmp	@BLBL29
@BLBL30:

; 427 		    cb = (ULONG)WinSendMsg(windata->hwndMle, MLM_IMPORT,
	mov	ebx,[ebp-018h];	cbBytes
	mov	ecx,ebx
	movzx	ecx,cx
	push	ecx
	mov	ecx,esi
	push	offset FLAT:@43lOffset
	push	01c3h
	push	dword ptr [ecx+04h]
	call	WinSendMsg
	add	esp,010h

; 430 		    if( cb != cbBytes )
	cmp	eax,ebx
	je	@BLBL29

; 431 			fprintf(stderr,"mismatch: %lu of %lu imported\n",
	push	ebx
	mov	edx,offset FLAT:@CBE17
	push	eax
	mov	eax,dword ptr  stderr
	sub	esp,08h
	call	_fprintfieee
	add	esp,010h

; 434 	    }
@BLBL29:

; 435 	    delete[] str;				/* dynamically allocated! */
	mov	eax,[ebp-014h];	str
	call	__vd__FPv

; 437 	return 0;
	add	esp,068h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL94:

; 446 	    HWND const hwndFrame = WinQueryWindow(hwnd, QW_PARENT);
	push	05h

; 449 	    Verbose(3, "VSETUP", "hwndFrame %#x", hwndFrame);
	mov	edi,offset FLAT:@CBE2

; 446 	    HWND const hwndFrame = WinQueryWindow(hwnd, QW_PARENT);
	push	dword ptr [ebp+08h];	hwnd
	call	WinQueryWindow

; 447 	    HWND const hwndMenu = WinWindowFromID(hwndFrame, FID_MENU);
	push	08005h

; 446 	    HWND const hwndFrame = WinQueryWindow(hwnd, QW_PARENT);
	mov	ebx,eax

; 447 	    HWND const hwndMenu = WinWindowFromID(hwndFrame, FID_MENU);
	push	ebx
	call	WinWindowFromID

; 449 	    Verbose(3, "VSETUP", "hwndFrame %#x", hwndFrame);
	push	ebx
	mov	ebx,eax
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE18
	mov	edx,edi
	mov	eax,03h
	call	Verbose

; 450 	    Verbose(3, "VSETUP", "hwndMenu %#x", hwndMenu);
	push	ebx

; 449 	    Verbose(3, "VSETUP", "hwndFrame %#x", hwndFrame);
	mov	edx,edi

; 450 	    Verbose(3, "VSETUP", "hwndMenu %#x", hwndMenu);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE19
	mov	eax,03h
	call	Verbose

; 451 	    bool = WinEnableMenuItem(hwndMenu, IDM_DRVSETUP, TRUE);
	push	04000h
	push	01008dh
	push	0192h
	push	ebx
	call	WinSendMsg
	add	esp,040h

; 452 	    if( bool == FALSE )
	test	eax,eax
	jne	@BLBL33

; 453 		Verbose(1, "VSetup",
	mov	ebx,esi
	push	dword ptr [ebx]
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE21
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE20
	mov	eax,01h
	call	Verbose
	add	esp,014h

; 456 	}
@BLBL33:

; 457 	return 0;
	xor	eax,eax
	add	esp,068h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL95:

; 467 	    Verbose(3, "VSETUP", "WM_COMMAND: %#x",cmd);
	mov	di,[ebp+010h];	mp1
	movzx	edi,di
	push	edi
	mov	ecx,offset FLAT:@CBE22
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE2
	mov	eax,03h
	call	Verbose
	mov	eax,edi
	add	esp,010h

; 468 	    switch( cmd )
	cmp	eax,084h
	jl	@BLBL109
	je	@BLBL101
	cmp	eax,08dh
	jl	@BLBL110
	je	@BLBL103
	cmp	eax,08eh
	je	@BLBL104
	cmp	eax,0afh
	je	@BLBL108
	jmp	@BLBL86
@BLBL110:
	cmp	eax,085h
	je	@BLBL102
	jmp	@BLBL86
@BLBL109:
	cmp	eax,07ah
	jl	@BLBL111
	je	@BLBL107
	cmp	eax,07ch
	je	@BLBL106
	cmp	eax,083h
	je	@BLBL100
	jmp	@BLBL86
@BLBL111:
	cmp	eax,06fh
	je	@BLBL105
	jmp	@BLBL86
@BLBL100:

; 471 		DisplayDriverVersion();
	call	DisplayDriverVersion__Fv

; 472 		return 0;
	add	esp,068h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL101:

; 475 		DisplayDriverMessages();
	call	DisplayDriverMessages__Fv

; 476 		return 0;
	add	esp,068h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL102:

; 480 		  HWND const	hwndFrame = WinQueryWindow(hwnd, QW_PARENT);
	push	05h
	push	dword ptr [ebp+08h];	hwnd
	call	WinQueryWindow

; 481 		  HWND const	hwndMenu = WinWindowFromID(hwndFrame, FID_MENU);
	push	08005h
	push	eax
	call	WinWindowFromID

; 484 		  checked = WinIsMenuItemChecked(hwndMenu, IDM_NOBEEP);
	push	02000h

; 481 		  HWND const	hwndMenu = WinWindowFromID(hwndFrame, FID_MENU);
	mov	ebx,eax

; 484 		  checked = WinIsMenuItemChecked(hwndMenu, IDM_NOBEEP);
	push	010085h
	push	0191h
	push	ebx
	call	WinSendMsg
	mov	edi,eax

; 485 		  DriverSetBeeper((USHORT)checked);
	call	DriverSetBeeper__FUs

; 486 		  checked = !checked;
	add	edi,0ffffffffh
	sbb	edi,edi
	inc	edi

; 487 		  WinCheckMenuItem(hwndMenu,IDM_NOBEEP, checked);
	test	di,di
	setne	cl
	movzx	ecx,cl
	neg	ecx
	and	ecx,02000h
	movzx	ecx,cx
	sal	ecx,010h
	or	ch,020h
	push	ecx
	push	010085h
	push	0192h
	push	ebx
	call	WinSendMsg
	add	esp,030h

; 489 	      return 0;
	xor	eax,eax
	add	esp,068h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL103:

; 496 		    dta->parent = hwnd;
	mov	ebx,[ebp+08h];	hwnd

; 494 		    PTHR_DRVSETUP	dta = new THR_DRVSETUP;
	mov	eax,04h
	call	__nw__FUi

; 497 		    tid = _beginthread(DrvsetupThread, NULL, STACK_SIZE, dta);
	push	eax

; 496 		    dta->parent = hwnd;
	mov	[eax],ebx

; 497 		    tid = _beginthread(DrvsetupThread, NULL, STACK_SIZE, dta);
	sub	esp,0ch
	mov	ecx,0100000h
	xor	edx,edx
	mov	eax,offset FLAT: DrvsetupThread
	call	_beginthread
	add	esp,010h
	mov	[ebp-01ch],eax;	tid

; 498 		    if( tid == -1 )
	cmp	eax,0ffffffffh
	jne	@BLBL37

; 499 			Verbose(1, "VSETUP",
	call	_errno
	mov	ecx,offset FLAT:@CBE23
	push	dword ptr [eax]
	mov	edx,offset FLAT:@CBE2
	sub	esp,0ch
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 500 				"Couldn't create DrvsetupThread - error %d", errno);
	jmp	@BLBL38
@BLBL37:

; 503 			HWND const hwndFrame = WinQueryWindow(hwnd, QW_PARENT);
	push	05h

; 506 			Verbose(3, "VSETUP", "hwndFrame %#x", hwndFrame);
	mov	edi,offset FLAT:@CBE2

; 503 			HWND const hwndFrame = WinQueryWindow(hwnd, QW_PARENT);
	push	dword ptr [ebp+08h];	hwnd
	call	WinQueryWindow

; 504 			HWND const hwndMenu = WinWindowFromID(hwndFrame, FID_MENU);
	push	08005h

; 503 			HWND const hwndFrame = WinQueryWindow(hwnd, QW_PARENT);
	mov	ebx,eax

; 504 			HWND const hwndMenu = WinWindowFromID(hwndFrame, FID_MENU);
	push	ebx
	call	WinWindowFromID

; 506 			Verbose(3, "VSETUP", "hwndFrame %#x", hwndFrame);
	push	ebx
	mov	ebx,eax
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE18
	mov	edx,edi
	mov	eax,03h
	call	Verbose

; 507 			Verbose(3, "VSETUP", "hwndMenu %#x", hwndMenu);
	push	ebx

; 506 			Verbose(3, "VSETUP", "hwndFrame %#x", hwndFrame);
	mov	edx,edi

; 507 			Verbose(3, "VSETUP", "hwndMenu %#x", hwndMenu);
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE19
	mov	eax,03h
	call	Verbose

; 508 			bool = WinEnableMenuItem(hwndMenu, IDM_DRVSETUP, FALSE);
	push	040004000h
	push	01008dh
	push	0192h
	push	ebx
	call	WinSendMsg
	add	esp,040h

; 509 			if( bool == FALSE )
	test	eax,eax
	jne	@BLBL39

; 510 			    Verbose(1, "VSetup",
	mov	ebx,esi
	push	dword ptr [ebx]
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE21
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE20
	mov	eax,01h
	call	Verbose
	add	esp,014h

; 512 				    WinGetLastError(windata->hab));
@BLBL39:

; 513 			Verbose(2, "VSETUP", "created thread id %d", tid);
	push	dword ptr [ebp-01ch];	tid
	mov	ecx,offset FLAT:@CBE24
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE2
	mov	eax,02h
	call	Verbose
	add	esp,010h

; 514 		    }
@BLBL38:

; 516 		return 0;
	xor	eax,eax
	add	esp,068h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL104:

; 522 		  result = WinDlgBox(HWND_DESKTOP, hwnd, DumpDialogProc,
	mov	ebx,[ebp+08h];	hwnd
	push	ebx
	push	0208h
	push	0h
	push	offset FLAT: DumpDialogProc
	push	ebx
	push	01h
	call	WinDlgBox

; 524 		  Verbose(2, "VSETUP", "WinDlgBox(IDW_DRVDUMP) returned %lu "
	mov	ecx,esi

; 522 		  result = WinDlgBox(HWND_DESKTOP, hwnd, DumpDialogProc,
	mov	ebx,eax

; 524 		  Verbose(2, "VSETUP", "WinDlgBox(IDW_DRVDUMP) returned %lu "
	push	dword ptr [ecx]
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE25
	push	ebx
	mov	edx,offset FLAT:@CBE2
	sub	esp,0ch
	mov	eax,02h
	call	Verbose
	add	esp,030h

; 528 	      return 0;
	add	esp,068h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL105:

; 531 		return 0;
	xor	eax,eax
	add	esp,068h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL106:

; 537 		    textlen = (IPT)WinSendMsg(windata->hwndMle,
	push	0h
	mov	ebx,esi
	push	0h
	push	01c0h
	push	dword ptr [ebx+04h]
	call	WinSendMsg

; 539 		    WinSendMsg(windata->hwndMle, MLM_DELETE,
	push	eax
	push	0h
	push	01c6h
	push	dword ptr [ebx+04h]
	call	WinSendMsg
	add	esp,020h

; 542 		return 0;
	add	esp,068h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL107:

; 545 		WinSendMsg(windata->hwndMle, MLM_COPY, 0, 0);
	push	0h
	mov	ebx,esi
	push	0h
	push	01d9h
	push	dword ptr [ebx+04h]
	call	WinSendMsg
	add	esp,010h

; 546 		return 0;
	add	esp,068h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL108:

; 553 		    result = WinDlgBox(HWND_DESKTOP, hwnd, AboutDialogProc,
	lea	ebx,[ebp-01eh];	version
	push	ebx

; 550 		    USHORT version = (SUBVER << 8) | VER;
	mov	word ptr [ebp-01eh],05200h;	version

; 553 		    result = WinDlgBox(HWND_DESKTOP, hwnd, AboutDialogProc,
	push	01f4h
	push	0h
	push	offset FLAT: AboutDialogProc
	push	dword ptr [ebp+08h];	hwnd
	push	01h
	call	WinDlgBox

; 555 		    Verbose(2, "VSETUP", "WinDlgBox(IDW_ABOUT) returned %lu "
	mov	ecx,esi

; 553 		    result = WinDlgBox(HWND_DESKTOP, hwnd, AboutDialogProc,
	mov	ebx,eax

; 555 		    Verbose(2, "VSETUP", "WinDlgBox(IDW_ABOUT) returned %lu "
	push	dword ptr [ecx]
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE26
	push	ebx
	mov	edx,offset FLAT:@CBE2
	sub	esp,0ch
	mov	eax,02h
	call	Verbose
	add	esp,030h

; 559 		return 0;
	add	esp,068h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	

; 565 	break;					/* continue with default */
	jmp	@BLBL86
@BLBL96:

; 568 	return (MRESULT)TRUE;
	add	esp,068h
	mov	eax,01h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	

; 571 	break;
@BLBL86:

; 574     return WinDefWindowProc(hwnd, msg, mp1, mp2);
	push	dword ptr [ebp+014h];	mp2
	push	dword ptr [ebp+010h];	mp1
	push	dword ptr [ebp+0ch];	msg
	push	dword ptr [ebp+08h];	hwnd
	call	WinDefWindowProc
	add	esp,078h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
WindowProc	endp

; 271 DisplayDriverMessages(VOID)

DisplayDriverMessages__Fv	proc
	push	edi
	push	esi
	sub	esp,08h

; 276     if( (rc=DriverReadMessages((PUCHAR *)&buffer)) != 0 )
	lea	eax,[esp+04h];	buffer
	call	DriverReadMessages__FPPUc
	test	eax,eax
	je	@BLBL15

; 277 	Verbose(0, "VSETUP", "Can't read driver messages - error %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE5
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE2
	xor	eax,eax
	call	Verbose
	add	esp,018h
	pop	esi
	pop	edi
	ret	
@BLBL15:

; 282 	for( l = i = 0; buffer[i] != '\0'; i += strlen(&buffer[i])+1, ++l )
	mov	eax,[esp+04h];	buffer
	cmp	byte ptr [eax],0h
	je	@BLBL17
	xor	esi,esi
	xor	edi,edi
@BLBL18:

; 283 	    Verbose(0, "VRAID", "Line %u: %s", l, &buffer[i]);
	mov	eax,[esp+04h];	buffer
	add	eax,esi
	push	eax
	mov	ecx,offset FLAT:@CBE7
	push	edi
	mov	edx,offset FLAT:@CBE6
	sub	esp,0ch
	xor	eax,eax
	call	Verbose

; 282 	for( l = i = 0; buffer[i] != '\0'; i += strlen(&buffer[i])+1, ++l )
	mov	eax,[esp+018h];	buffer
	add	eax,esi
	call	strlen
	add	esp,014h
	inc	eax
	mov	ecx,[esp+04h];	buffer
	add	eax,esi
	inc	edi
	mov	esi,eax
	cmp	byte ptr [ecx+esi],0h
	jne	@BLBL18
@BLBL17:

; 284 	delete[] buffer;
	mov	eax,[esp+04h];	buffer
	call	__vd__FPv

; 286     return;
	add	esp,08h
	pop	esi
	pop	edi
	ret	
DisplayDriverMessages__Fv	endp

; 254 DisplayDriverVersion(VOID)

DisplayDriverVersion__Fv	proc
	sub	esp,0ch

; 259     if( (rc=DriverGetVersion(&ver, &us)) != 0 )
	lea	edx,[esp+08h];	us
	lea	eax,[esp+0ah];	ver
	call	DriverGetVersion__FPUsT1
	test	eax,eax
	je	@BLBL11

; 260 	Verbose(0, "VSETUP", "Can't read driver version - error %lu",rc);
	push	eax
	mov	ecx,offset FLAT:@CBE3
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE2
	xor	eax,eax
	call	Verbose
	add	esp,01ch
	ret	
@BLBL11:

; 262 	Verbose(0, "VSETUP", "Driver Version %u.%02u installed",
	mov	edx,offset FLAT:@CBE2
	mov	ax,[esp+0ah];	ver
	mov	cx,ax
	movzx	ecx,cl
	push	ecx
	movzx	eax,ax
	sar	eax,08h
	and	eax,0ffh
	movzx	eax,al
	push	eax
	mov	ecx,offset FLAT:@CBE4
	sub	esp,0ch
	xor	eax,eax
	call	Verbose

; 264     return;
	add	esp,020h
	ret	
DisplayDriverVersion__Fv	endp

; 170 Verbose(unsigned level,char const * module,char const * fmt,...)

	public Verbose
Verbose	proc
	push	ebx
	mov	ebx,edx
	push	edi
	sub	esp,014h

; 172     if( level <= uLogLevel )
	cmp	dword ptr  @6uLogLevel,eax

; 170 Verbose(unsigned level,char const * module,char const * fmt,...)
	mov	[esp+028h],ecx;	fmt

; 172     if( level <= uLogLevel )
	jb	@BLBL3

; 174 	char * const	str = new char[4000];
	mov	eax,0fa0h
	call	__vn__FUi

; 178 	i = sprintf(str, "\r%s - ", module);
	push	ebx
	mov	ebx,eax
	sub	esp,08h
	mov	edx,offset FLAT:@CBE1
	call	_sprintfieee

; 180 	vsprintf(&str[i], fmt, argp);
	mov	edx,[esp+034h];	fmt
	add	eax,ebx
	lea	ecx,[esp+038h];	fmt
	call	_vsprintfieee
	add	esp,0ch

; 183 	if( hdLogFile != 0 )
	cmp	dword ptr  @7hdLogFile,0h

; 174 	char * const	str = new char[4000];
	mov	edi,ebx

; 183 	if( hdLogFile != 0 )
	je	@BLBL4

; 187 	    DosWrite(hdLogFile, str, strlen(str), &ul);
	mov	eax,ebx
	call	strlen
	lea	ecx,[esp+0ch];	ul
	push	ecx
	push	eax
	push	ebx
	push	dword ptr  @7hdLogFile
	call	DosWrite
	add	esp,010h

; 188 	}
@BLBL4:

; 189 	WinPostMsg(hwndClient, WUM_ADDLOG, MPFROMP(str), 0);
	push	0h
	push	edi
	push	01001h
	push	dword ptr  @5hwndClient
	call	WinPostMsg
	add	esp,010h

; 190     }
@BLBL3:

; 191 }
	add	esp,014h
	pop	edi
	pop	ebx
	ret	
Verbose	endp
CODE32	ends
end
