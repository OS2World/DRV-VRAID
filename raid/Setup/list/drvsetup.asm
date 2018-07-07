	title	drvsetup.cpp
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
	extrn	WinQuerySysColor:proc
	extrn	ProfileRead:proc
	extrn	WinQueryWindowPos:proc
	extrn	Verbose:proc
	extrn	WinCreateWindow:proc
	extrn	MyMessageBox__FCiCUlPce:proc
	extrn	WinGetLastError:proc
	extrn	memset:proc
	extrn	WinSendMsg:proc
	extrn	WinLoadMenu:proc
	extrn	WinQueryPointerPos:proc
	extrn	WinMapWindowPoints:proc
	extrn	SearchCnr__FUlPFUlPvT2PUl_UlPvN23:proc
	extrn	EmphInuse2Source__FUlP9_MYRECORDPUlT3:proc
	extrn	WinPopupMenu:proc
	extrn	DrgAllocDraginfo:proc
	extrn	DrgAddStrHandle:proc
	extrn	DrgSetDragitem:proc
	extrn	DrgDrag:proc
	extrn	DrgFreeDraginfo:proc
	extrn	_sprintfieee:proc
	extrn	WinMessageBox:proc
	extrn	LookupDisk__FUlP9_MYRECORDP5VRDevPP9_MYRECORD:proc
	extrn	WinPostMsg:proc
	extrn	WinQueryWindowPtr:proc
	extrn	__nw__FUi:proc
	extrn	_beginthread:proc
	extrn	DrvscanThread:proc
	extrn	_errno:proc
	extrn	EmphSource__FUlP9_MYRECORDPUlT3:proc
	extrn	QueryInuse__FUlP9_MYRECORDPUlPP9_MYRECORD:proc
	extrn	WinDlgBox:proc
	extrn	DrvtypeDlgProc:proc
	extrn	memcpy:proc
	extrn	time:proc
	extrn	__dl__FPv:proc
	extrn	__ct__7VSingleFPUc:proc
	extrn	__ct__6VChainFPUci:proc
	extrn	__ct__7VStripeFPUci:proc
	extrn	__ct__7VMirrorFPUci:proc
	extrn	EmphInuse__FUlP9_MYRECORDPUlT3:proc
	extrn	QueryAny__FUlP9_MYRECORDPUlPP9_MYRECORD:proc
	extrn	WinDestroyWindow:proc
	extrn	WinQueryWindow:proc
	extrn	ProfileWrite:proc
	extrn	WinSetWindowPos:proc
	extrn	WinQueryPresParam:proc
	extrn	WinInvalidateRect:proc
	extrn	strlen:proc
	extrn	WinDefWindowProc:proc
	extrn	_Exception_CPP:proc
	extrn	WinInitialize:proc
	extrn	WinCreateMsgQueue:proc
	extrn	WinCancelShutdown:proc
	extrn	WinRegisterClass:proc
	extrn	WinCreateStdWindow:proc
	extrn	WinSetWindowPtr:proc
	extrn	WinQueryTaskSizePos:proc
	extrn	WinGetMsg:proc
	extrn	WinDispatchMsg:proc
	extrn	WinDestroyMsgQueue:proc
	extrn	WinTerminate:proc
	extrn	usHostId:word
	extrn	_fltused:dword
CONST32_RO	segment
@CBE1	db "Background",0h
	align 04h
@CBE2	db "Foreground",0h
	align 04h
@CBE3	db "DrvSetup",0h
	align 04h
@CBE4	db "client window %u,%u %u,%"
db "u",0h
	align 04h
@CBE5	db "WinCreateWindow(...,WC_C"
db "ONTAINER,...) - error %#"
db "x",0h
	align 04h
@CBE6	db "WinLoadMenu failed (last"
db " error %#x)",0h
@CBE7	db "popup menues %#x,%#x,%#x"
db 0h
	align 04h
@CBE8	db "DoPopup: %lu records in "
db "use",0h
@CBE9	db "CM_ALLOCRECORD: %#x",0h
@CBE10	db "WinSendMsg(CM_INSERTRECO"
db "RD) failed - last error "
db "%#x",0h
@CBE11	db "DragInit: record %#x",0h
	align 04h
@CBE12	db "DragInit: record passed "
db "checks",0h
	align 04h
@CBE13	db "DRT_CUSTOMER",0h
	align 04h
@CBE14	db "DRM_SHAREMEM,DRM_UNKNOWN"
db 0h
	align 04h
@CBE15	db "DragLeave, record %#x",0h
	align 04h
@CBE16	db "DragOver, record %#x",0h
	align 04h
@CBE17	db "DragOver, no target",0h
@CBE18	db "DragOver, no destination"
db 0h
	align 04h
@CBE19	db "DragOver, pRecord != pmr"
db "DragDestination!",0h
	align 04h
@CBE20	db "DragOver, destination %#"
db "x",0h
	align 04h
@CBE21	db "DragOver, source == dest"
db "ination!",0h
	align 04h
@CBE22	db "DragOver, source too sma"
db "ll",0h
	align 04h
@CBE23	db "DragOver, bad destinatio"
db "n",0h
	align 04h
@CBE24	db "DragOver, bad destinatio"
db "n type",0h
	align 04h
@CBE25	db "DragOver, destination is"
db " acceptable",0h
@CBE26	db "DragDrop, record %#x",0h
	align 04h
@CBE27	db "DragDrop, can't use exte"
db "rnal data",0h
	align 04h
@CBE28	db "This will destroy all da"
db "ta on the dropped device"
db " ID %02X%02X%02X%02X%02X"
db "%02X.  Are you sure you "
db "want to do this?",0h
	align 04h
@CBE29	db "Just curious...",0h
@CBE30	db "DragDrop: found as recor"
db "d %lu",0h
	align 04h
@CBE31	db "WUM_START",0h
	align 04h
@CBE32	db "_beginthread(DrvscanThre"
db "ad) failed - errno %u",0h
	align 04h
@CBE33	db "WUM_DEVTREE",0h
@CBE34	db "WM_CONTROL",0h
	align 04h
@CBE35	db "WM_CONTROL (id %#x, CN_C"
db "ONTEXTMENU)",0h
@CBE36	db "WM_CONTROL (id %#x, CN_E"
db "NTER)",0h
	align 04h
@CBE37	db "Internal error, can't us"
db "e NULL device",0h
	align 04h
@CBE38	db "Can't work on this devic"
db "e, remove parent first.",0h
@CBE39	db "Can't work on this devic"
db "e, create physdevice fir"
db "st.",0h
@CBE40	db "This device (or part of "
db "it) is in use by OS/2, r"
db "emove recognized filesys"
db "tems first.",0h
@CBE41	db "WinSendMsg(CM_SETRECORDE"
db "MPHASIS) failed - last e"
db "rror %#x)",0h
	align 04h
@CBE42	db "WM_CONTROL (id %#x, CN_I"
db "NITDRAG)",0h
	align 04h
@CBE43	db "WM_CONTROL (id %#x, CN_D"
db "RAGOVER)",0h
	align 04h
@CBE44	db "WM_CONTROL (id %#x, CN_D"
db "RAGLEAVE)",0h
	align 04h
@CBE45	db "WM_CONTROL (id %#x, CN_D"
db "ROP)",0h
	align 04h
@CBE46	db "WM_CONTROL (id %#x, noti"
db "fy code %#x)",0h
	align 04h
@CBE47	db "WM_CONTEXTMENU",0h
	align 04h
@CBE48	db "WM_MENUEND",0h
	align 04h
@CBE49	db "WM_COMMAND",0h
	align 04h
@CBE50	db "WM_COMMAND, IDM_CREATE_P"
db "DEV",0h
@CBE51	db "WM_COMMAND, IDM_COMBINE_"
db "DRV",0h
@CBE52	db "%lu drives are ",022h,"in-use",022h,0h
@CBE53	db "WM_COMMAND, IDM_DESTROY_"
db "DRV",0h
@CBE54	db "Are you sure?",0h
	align 04h
@CBE55	db "WM_COMMAND, IDM_DELETE_P"
db "DEV",0h
@CBE56	db "Are you sure you want to"
db " destroy ID %02X%02X%02X"
db "%02X%02X%02X?",0h
	align 04h
@CBE57	db "WM_COMMAND, IDM_REMOVE_C"
db "HILD",0h
	align 04h
@CBE58	db "Are you sure you want to"
db " remove ID %02X%02X%02X%"
db "02X%02X%02X from array?",0h
@CBE59	db "WM_COMMAND %u",0h
	align 04h
@CBE60	db "WM_CREATE",0h
	align 04h
@CBE61	db "DrvsetupWindow",0h
	align 04h
@CBE62	db "WM_CLOSE",0h
	align 04h
@CBE63	db "%lu drives are available"
db 0h
	align 04h
@CBE64	db "WM_DESTROY",0h
	align 04h
@CBE65	db "WM_SIZE",0h
@CBE66	db "Size",0h
	align 04h
@CBE67	db "WM_MOVE",0h
@CBE68	db "Position",0h
	align 04h
@CBE69	db "WM_QUERYTRACKINFO (track"
db "ing flags %#x)",0h
	align 04h
@CBE70	db "WM_PRESPARAMCHANGED(type"
db " %#x)",0h
	align 04h
@CBE71	db "WinQueryPresParam failed"
db " - last error %#x",0h
	align 04h
@CBE72	db "Set foreground colour to"
db " %#x",0h
	align 04h
@CBE73	db "Set background colour to"
db " %#x",0h
	align 04h
@CBE74	db "Set font to ",022h,"%s",022h,0h
	align 04h
@CBE75	db "Font",0h
	align 04h
@CBE76	db "WM_PRESPARAMCHANGED(unsu"
db "pported type %#x)",0h
	align 04h
@CBE77	db "startup",0h
@CBE78	db "WinInitialize failed!?!",0h
@CBE79	db "WinCreateMsgQueue - erro"
db "r %#x",0h
	align 04h
@CBE80	db "WinRegisterClass - error"
db " %#x",0h
	align 04h
@CBE81	db "VSetup - Drive Definitio"
db "n Window",0h
	align 04h
@CBE82	db "WinCreateStdWindow - err"
db "or %#x",0h
	align 04h
@CBE83	db "WinSetWindowPtr failed -"
db " error %#",0h
	align 04h
@CBE84	db "Copying old size of wind"
db "ow",0h
	align 04h
@CBE85	db "Copying old position of "
db "window",0h
	align 04h
@CBE86	db "WinSetWindowPos failed ("
db "last error %#x)",0h
@CBE87	db "DrvSetupThread: stopped",0h
@1vcid	db "$Id: drvsetup.cpp,v 2.9 "
db "2000/04/10 01:12:09 vitu"
db "s Exp $",0h
CONST32_RO	ends
DATA32	segment
@2rgbBackground	dd 0ffffff01h
@3rgbForeground	dd 0ffffff01h
DATA32	ends
BSS32	segment
@bcregular	dd 0h
BSS32	ends
EH_DATA	segment
@11e__fsm_tab	db 0adh,0deh,0efh,0beh
	db 04h,0h,0h,0h
	dd -010h
	dd offset FLAT:__dl__FPv
	db 0fdh,0ffh,0ffh,0ffh
	db "0",0h,0h,0h
	db 0h,0h
	db 0h,0h
	dd -014h
	dd offset FLAT:__dl__FPv
	db 0fdh,0ffh,0ffh,0ffh
	db "0",01h,0h,0h
	db 0h,0h
	db 0h,0h
	dd -018h
	dd offset FLAT:__dl__FPv
	db 0fdh,0ffh,0ffh,0ffh
	db "0",01h,0h,0h
	db 0h,0h
	db 0h,0h
	dd -01ch
	dd offset FLAT:__dl__FPv
	db 0fdh,0ffh,0ffh,0ffh
	db "4",01h,0h,0h
	db 0h,0h
	db 0h,0h
EH_DATA	ends
CODE32	segment

; 1524 DrvsetupThread(void *dummy)

	public DrvsetupThread
DrvsetupThread	proc
	push	ebp

; 1534     Verbose(2, "DrvSetup", "startup");
	mov	ecx,offset FLAT:@CBE77

; 1524 DrvsetupThread(void *dummy)
	mov	ebp,esp
	push	ebx

; 1534     Verbose(2, "DrvSetup", "startup");
	mov	ebx,offset FLAT:@CBE3

; 1524 DrvsetupThread(void *dummy)
	push	edi
	mov	edi,eax
	push	esi

; 1534     Verbose(2, "DrvSetup", "startup");
	mov	edx,ebx

; 1524 DrvsetupThread(void *dummy)
	sub	esp,08ch

; 1534     Verbose(2, "DrvSetup", "startup");
	mov	eax,02h
	call	Verbose

; 1537 	work = new WORK;
	mov	eax,028h
	call	__nw__FUi
	mov	esi,eax

; 1538 	memset(work, 0, sizeof(WORK));
	mov	ecx,028h
	xor	edx,edx
	call	memset

; 1541 	if( !(work->hab = WinInitialize(0)) )
	push	0h

; 1537 	work = new WORK;
	mov	[ebp-010h],esi;	work

; 1538 	memset(work, 0, sizeof(WORK));
	mov	eax,edi

; 1539 	work->self = dta;
	mov	[esi],eax

; 1541 	if( !(work->hab = WinInitialize(0)) )
	call	WinInitialize
	add	esp,04h
	mov	[esi+04h],eax
	cmp	dword ptr [esi+04h],0h
	mov	edx,ebx
	jne	@BLBL130

; 1543 	    Verbose(0, "DrvSetup", "WinInitialize failed!?!");
	xor	eax,eax
	mov	ecx,offset FLAT:@CBE78
	call	Verbose

; 1544 	    break;
	jmp	@BLBL131
@BLBL130:

; 1547 	if( !(hmq = WinCreateMsgQueue(work->hab, 0)) )
	mov	ebx,[ebp-010h];	work
	push	0h
	push	dword ptr [ebx+04h]
	call	WinCreateMsgQueue
	add	esp,08h
	mov	[ebp-01ch],eax;	hmq
	test	eax,eax
	jne	@BLBL132

; 1549 	    Verbose(0, "DrvSetup", "WinCreateMsgQueue - error %#x",
	push	dword ptr [ebx+04h]
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE79
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	xor	eax,eax
	call	Verbose
	add	esp,014h

; 1551 	    break;
	jmp	@BLBL131
@BLBL132:

; 1553 	WinCancelShutdown(hmq, TRUE);		/* don't want to handle it */
	push	01h

; 1555 	if( !WinRegisterClass(work->hab, WC_DRVSETUP, DrvsetupWindowProc,
	mov	ebx,[ebp-010h];	work

; 1553 	WinCancelShutdown(hmq, TRUE);		/* don't want to handle it */
	push	dword ptr [ebp-01ch];	hmq
	call	WinCancelShutdown

; 1555 	if( !WinRegisterClass(work->hab, WC_DRVSETUP, DrvsetupWindowProc,
	push	04h
	push	01h
	push	offset FLAT: DrvsetupWindowProc
	push	offset FLAT:@CBE61
	push	dword ptr [ebx+04h]
	call	WinRegisterClass
	add	esp,01ch
	test	eax,eax
	jne	@BLBL133

; 1558 	    Verbose(0, "DrvSetup", "WinRegisterClass - error %#x",
	push	dword ptr [ebx+04h]
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE80
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	xor	eax,eax
	call	Verbose
	add	esp,014h

; 1560 	    break;
	jmp	@BLBL131
@BLBL133:

; 1566 	hwndFrame = WinCreateStdWindow(HWND_DESKTOP, WS_ANIMATE, &flCreate,
	lea	ebx,[ebp-014h];	hwndClient
	push	ebx
	lea	ebx,[ebp-020h];	flCreate
	push	0190h

; 1563 	flCreate = FCF_SIZEBORDER | FCF_TASKLIST | FCF_SYSMENU | FCF_TITLEBAR
	mov	dword ptr [ebp-020h],0483bh;	flCreate

; 1566 	hwndFrame = WinCreateStdWindow(HWND_DESKTOP, WS_ANIMATE, &flCreate,
	push	0h
	push	0h
	push	offset FLAT:@CBE81
	push	offset FLAT:@CBE61
	push	ebx
	push	0400000h
	push	01h
	call	WinCreateStdWindow
	add	esp,024h
	mov	[ebp-024h],eax;	hwndFrame

; 1572 	if( hwndFrame == 0  ||  hwndClient == 0 )
	test	eax,eax
	je	@BLBL134
	cmp	dword ptr [ebp-014h],0h;	hwndClient
	jne	@BLBL135
@BLBL134:

; 1574 	    Verbose(0, "DrvSetup", "WinCreateStdWindow - error %#x",
	mov	ebx,[ebp-010h];	work
	push	dword ptr [ebx+04h]
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE82
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	xor	eax,eax
	call	Verbose
	add	esp,014h

; 1576 	    break;				/* couldn't create window for some
	jmp	@BLBL131
@BLBL135:

; 1582 	bool = WinSetWindowPtr(hwndClient, 0, work);
	mov	ebx,[ebp-010h];	work
	push	ebx
	push	0h
	push	dword ptr [ebp-014h];	hwndClient
	call	WinSetWindowPtr
	add	esp,0ch

; 1583 	if( bool == FALSE )
	test	eax,eax
	jne	@BLBL136

; 1585 	    MyMessageBox(0, HWND_DESKTOP,
	push	dword ptr [ebx+04h]
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE83
	sub	esp,0ch
	mov	edx,01h
	xor	eax,eax
	call	MyMessageBox__FCiCUlPce
	add	esp,014h

; 1588 	    break;
	jmp	@BLBL131
@BLBL136:

; 1591 	WinPostMsg(hwndClient, WUM_START, 0, 0);
	push	0h

; 1603 	    WinQueryTaskSizePos(work->hab, 0, &swp);
	lea	ebx,[ebp-048h];	swp

; 1591 	WinPostMsg(hwndClient, WUM_START, 0, 0);
	push	0h
	push	01002h
	push	dword ptr [ebp-014h];	hwndClient
	call	WinPostMsg

; 1603 	    WinQueryTaskSizePos(work->hab, 0, &swp);
	push	ebx
	mov	ebx,[ebp-010h];	work
	push	0h
	push	dword ptr [ebx+04h]

; 1606 	    if( ProfileRead(WC_DRVSETUP, "Size", &cb, &tmp) == 0 )
	lea	ebx,[ebp-06ch];	tmp

; 1603 	    WinQueryTaskSizePos(work->hab, 0, &swp);
	call	WinQueryTaskSizePos

; 1606 	    if( ProfileRead(WC_DRVSETUP, "Size", &cb, &tmp) == 0 )
	push	ebx

; 1605 	    cb = sizeof(tmp);
	mov	dword ptr [ebp-018h],024h;	cb

; 1606 	    if( ProfileRead(WC_DRVSETUP, "Size", &cb, &tmp) == 0 )
	sub	esp,0ch
	lea	ecx,[ebp-018h];	cb
	mov	edx,offset FLAT:@CBE66
	mov	eax,offset FLAT:@CBE61
	call	ProfileRead
	add	esp,02ch
	test	eax,eax
	jne	@BLBL137

; 1608 		Verbose(3, "DrvSetup", "Copying old size of window");
	mov	ecx,offset FLAT:@CBE84
	mov	edx,offset FLAT:@CBE3
	mov	eax,03h
	call	Verbose

; 1609 		swp.cx = tmp.cx;
	mov	eax,[ebp-064h];	tmp
	mov	[ebp-040h],eax;	swp

; 1610 		swp.cy = tmp.cy;
	mov	eax,[ebp-068h];	tmp

; 1611 		swp.fl |= SWP_SIZE;
	or	byte ptr [ebp-048h],01h;	swp

; 1610 		swp.cy = tmp.cy;
	mov	[ebp-044h],eax;	swp

; 1612 	    }
@BLBL137:

; 1614 	    if( ProfileRead(WC_DRVSETUP, "Position", &cb, &tmp) == 0 )
	lea	ebx,[ebp-06ch];	tmp
	push	ebx

; 1613 	    cb = sizeof(tmp);
	mov	dword ptr [ebp-018h],024h;	cb

; 1614 	    if( ProfileRead(WC_DRVSETUP, "Position", &cb, &tmp) == 0 )
	sub	esp,0ch
	lea	ecx,[ebp-018h];	cb
	mov	edx,offset FLAT:@CBE68
	mov	eax,offset FLAT:@CBE61
	call	ProfileRead
	add	esp,010h
	test	eax,eax
	jne	@BLBL138

; 1616 		Verbose(3, "DrvSetup", "Copying old position of window");
	mov	ecx,offset FLAT:@CBE85
	mov	edx,offset FLAT:@CBE3
	mov	eax,03h
	call	Verbose

; 1617 		swp.x = tmp.x;
	mov	eax,[ebp-05ch];	tmp
	mov	[ebp-038h],eax;	swp

; 1618 		swp.y = tmp.y;
	mov	eax,[ebp-060h];	tmp

; 1619 		swp.fl |= SWP_MOVE;
	or	byte ptr [ebp-048h],02h;	swp

; 1618 		swp.y = tmp.y;
	mov	[ebp-03ch],eax;	swp

; 1620 	    }
@BLBL138:

; 1622 	    swp.fl |= SWP_SHOW;
	mov	ebx,[ebp-048h];	swp
	or	bl,08h

; 1623 	    bool = WinSetWindowPos(hwndFrame, HWND_TOP,
	push	ebx

; 1622 	    swp.fl |= SWP_SHOW;
	mov	[ebp-048h],ebx;	swp

; 1623 	    bool = WinSetWindowPos(hwndFrame, HWND_TOP,
	push	dword ptr [ebp-044h];	swp
	push	dword ptr [ebp-040h];	swp
	push	dword ptr [ebp-03ch];	swp
	push	dword ptr [ebp-038h];	swp
	push	03h
	push	dword ptr [ebp-024h];	hwndFrame
	call	WinSetWindowPos
	add	esp,01ch

; 1625 	    if( bool == FALSE )
	test	eax,eax
	jne	@BLBL139

; 1626 		Verbose(1, "DrvSetup", "WinSetWindowPos failed (last error %#x)",
	mov	ebx,[ebp-010h];	work
	push	dword ptr [ebx+04h]
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE86
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,01h
	call	Verbose
	add	esp,014h

; 1627 			WinGetLastError(work->hab));
@BLBL139:

; 1632 	while( WinGetMsg(work->hab, &qmsg, 0L, 0, 0) )
	push	0h
	lea	ecx,[ebp-08ch];	qmsg
	push	0h
	mov	ebx,[ebp-010h];	work
	push	0h
	push	ecx
	push	dword ptr [ebx+04h]
	call	WinGetMsg
	add	esp,014h
	test	eax,eax
	je	@BLBL140
	mov	edi,ebx
@BLBL141:

; 1633 	    WinDispatchMsg(work->hab, &qmsg);
	lea	ebx,[ebp-08ch];	qmsg
	push	ebx
	push	dword ptr [edi+04h]
	call	WinDispatchMsg

; 1632 	while( WinGetMsg(work->hab, &qmsg, 0L, 0, 0) )
	push	0h
	push	0h
	push	0h
	push	ebx
	push	dword ptr [edi+04h]
	call	WinGetMsg
	add	esp,01ch
	test	eax,eax
	jne	@BLBL141
@BLBL140:

; 1637 	WinDestroyWindow(hwndFrame);
	push	dword ptr [ebp-024h];	hwndFrame

; 1639 	WinTerminate(work->hab);
	mov	esi,[ebp-010h];	work

; 1637 	WinDestroyWindow(hwndFrame);
	call	WinDestroyWindow

; 1638 	WinDestroyMsgQueue(hmq);
	push	dword ptr [ebp-01ch];	hmq
	call	WinDestroyMsgQueue

; 1639 	WinTerminate(work->hab);
	push	dword ptr [esi+04h]
	call	WinTerminate
	mov	eax,esi

; 1640 	delete work;
	call	__dl__FPv
	add	esp,0ch

; 1642     while(0);
@BLBL131:

; 1644     Verbose(2, "DrvSetup", "DrvSetupThread: stopped");
	mov	ecx,offset FLAT:@CBE87
	mov	edx,offset FLAT:@CBE3
	mov	eax,02h
	call	Verbose

; 1645     return;
	add	esp,08ch
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
DrvsetupThread	endp

; 888 DrvsetupWindowProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)

DrvsetupWindowProc	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0fffffefch
	sub	esp,045ch

; 889 {
	mov	dword ptr [ebp-0104h],0h;	__es

; 888 DrvsetupWindowProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
	push	ebx

; 889 {
	mov	dword ptr [ebp-0fch],offset FLAT:@11e__fsm_tab;	__es

; 888 DrvsetupWindowProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
	push	edi
	push	esi

; 891     WORK * const work = (PWORK)WinQueryWindowPtr(hwnd, 0);
	push	0h
	push	dword ptr [ebp+08h];	hwnd
	call	WinQueryWindowPtr

; 894     switch( msg )
	mov	ecx,[ebp+0ch];	msg

; 891     WORK * const work = (PWORK)WinQueryWindowPtr(hwnd, 0);
	add	esp,08h
	mov	[ebp-020h],eax;	work

; 894     switch( msg )
	cmp	ecx,02fh
	jl	@BLBL162
	je	@BLBL160
	cmp	ecx,04fh
	jl	@BLBL163
	je	@BLBL161
	cmp	ecx,0424h
	je	@BLBL151
	cmp	ecx,01002h
	je	@BLBL148
	cmp	ecx,0100ah
	je	@BLBL149
	jmp	@BLBL147
@BLBL163:
	cmp	ecx,030h
	je	@BLBL150
	cmp	ecx,035h
	je	@BLBL152
	cmp	ecx,04ch
	je	@BLBL159
	jmp	@BLBL147
@BLBL162:
	cmp	ecx,06h
	jl	@BLBL164
	je	@BLBL158
	cmp	ecx,07h
	je	@BLBL157
	cmp	ecx,020h
	je	@BLBL153
	cmp	ecx,029h
	je	@BLBL155
	jmp	@BLBL147
@BLBL164:
	cmp	ecx,01h
	je	@BLBL154
	cmp	ecx,02h
	je	@BLBL156
	jmp	@BLBL147
@BLBL148:

; 904 	CreateCnr(hwnd, work);
	mov	edi,[ebp+08h];	hwnd

; 903 	Verbose(3, "DrvSetup", "WUM_START");
	sub	esp,0ch
	mov	ebx,offset FLAT:@CBE3
	mov	ecx,offset FLAT:@CBE31
	mov	edx,ebx
	mov	eax,03h
	call	Verbose
	add	esp,04h

; 904 	CreateCnr(hwnd, work);
	mov	edx,[ebp-020h];	work
	mov	eax,edi
	call	CreateCnr__FUlP5_WORK
	add	esp,04h

; 907 	    PTHR_DRVSCAN	dta = new THR_DRVSCAN;
	mov	eax,04h
	call	__nw__FUi
	add	esp,04h

; 909 	    dta->parent = hwnd;
	mov	[eax],edi

; 910 	    tid = _beginthread(DrvscanThread, NULL, STACK_SIZE, dta);
	push	eax
	mov	ecx,0100000h
	sub	esp,0ch
	xor	edx,edx
	mov	eax,offset FLAT: DrvscanThread
	call	_beginthread
	add	esp,010h

; 911 	    if( tid == -1 )
	cmp	eax,0ffffffffh
	jne	@BLBL75

; 912 		Verbose(1, "DrvSetup",
	call	_errno
	mov	edx,ebx
	push	dword ptr [eax]
	mov	ecx,offset FLAT:@CBE32
	sub	esp,0ch
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 913 			"_beginthread(DrvscanThread) failed - errno %u", errno);
@BLBL75:

; 916 	return 0;
	pop	esi

; 915 	regular = TRUE;
	mov	dword ptr  @bcregular,01h

; 916 	return 0;
	pop	edi
	xor	eax,eax
	pop	ebx
	add	esp,0460h
	pop	fs:[0h]
	leave	
	ret	
@BLBL149:

; 920 	Verbose(3, "DrvSetup", "WUM_DEVTREE");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE33
	mov	edx,offset FLAT:@CBE3
	mov	eax,03h
	call	Verbose

; 921 	work->hwndScanner = HWNDFROMMP(mp2);
	mov	eax,[ebp-020h];	work
	mov	ecx,[ebp+014h];	mp2
	mov	[eax+0ch],ecx

; 922 	DisplayDisk(work, NULL, (VRDev *)PVOIDFROMMP(mp1));
	mov	ecx,[ebp+010h];	mp1
	xor	edx,edx
	call	DisplayDisk__FP5_WORKP9_MYRECORDP5VRDev
	add	esp,0ch

; 923 	return 0;
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	add	esp,0460h
	pop	fs:[0h]
	leave	
	ret	
@BLBL150:

; 931 	Verbose(3, "DrvSetup", "WM_CONTROL");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE34
	mov	edx,offset FLAT:@CBE3
	mov	eax,03h
	call	Verbose

; 933 	    USHORT const	id = SHORT1FROMMP(mp1);
	mov	eax,[ebp+010h];	mp1

; 931 	Verbose(3, "DrvSetup", "WM_CONTROL");
	add	esp,0ch

; 933 	    USHORT const	id = SHORT1FROMMP(mp1);
	mov	[ebp-02ah],ax;	id

; 934 	    USHORT const	ncode = SHORT2FROMMP(mp1);
	shr	eax,010h
	mov	[ebp-066h],ax;	ncode

; 937 	    switch( ncode )
	movzx	eax,ax
	cmp	eax,068h
	jl	@BLBL172
	je	@BLBL170
	cmp	eax,06ah
	je	@BLBL166
	cmp	eax,06bh
	je	@BLBL167
	cmp	eax,077h
	je	@BLBL165
	jmp	@BLBL171
@BLBL172:
	cmp	eax,066h
	je	@BLBL169
	cmp	eax,067h
	je	@BLBL168
	jmp	@BLBL171
@BLBL165:

; 944 		Verbose(3, "DrvSetup", "WM_CONTROL (id %#x, CN_CONTEXTMENU)", id);
	mov	ecx,offset FLAT:@CBE35
	movzx	ebx,word ptr [ebp-02ah];	id
	push	ebx
	mov	edx,offset FLAT:@CBE3
	sub	esp,0ch
	mov	eax,03h
	call	Verbose
	add	esp,08h

; 945 		DoPopup(hwnd, work);
	mov	edx,[ebp-020h];	work
	mov	eax,[ebp+08h];	hwnd
	call	DoPopup__FUlCP5_WORK
	add	esp,08h

; 946 		break;
	jmp	@BLBL147
@BLBL166:

; 949 		Verbose(3, "DrvSetup", "WM_CONTROL (id %#x, CN_ENTER)",id);
	mov	ecx,offset FLAT:@CBE36
	movzx	ebx,word ptr [ebp-02ah];	id
	push	ebx
	mov	edx,offset FLAT:@CBE3
	sub	esp,0ch
	mov	eax,03h
	call	Verbose

; 951 		    PNOTIFYRECORDENTER	rec = (PNOTIFYRECORDENTER)PVOIDFROMMP(mp2);
	mov	ebx,[ebp+014h];	mp2

; 949 		Verbose(3, "DrvSetup", "WM_CONTROL (id %#x, CN_ENTER)",id);
	add	esp,010h

; 951 		    PNOTIFYRECORDENTER	rec = (PNOTIFYRECORDENTER)PVOIDFROMMP(mp2);
	mov	[ebp-03ch],ebx;	rec

; 953 		    if( rec == NULL )
	test	ebx,ebx
	jne	@BLBL76

; 954 			return 0;		/* dblk click on background? */
	pop	esi
	xor	eax,eax
	pop	edi
	pop	ebx
	add	esp,0460h
	pop	fs:[0h]
	leave	
	ret	
@BLBL76:

; 956 		    VRDev *	dsk = ((PMYRECORD)(rec->pRecord))->dsk;
	mov	eax,[ebp-03ch];	rec
	mov	eax,[eax+08h]
	mov	eax,[eax+01ch]
	mov	[ebp-038h],eax;	dsk

; 957 		    if( dsk == NULL )
	test	eax,eax
	jne	@BLBL77

; 958 			MyMessageBox(0, hwnd,
	mov	edx,[ebp+08h];	hwnd
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE37
	xor	eax,eax
	call	MyMessageBox__FCiCUlPce
	add	esp,0ch

; 959 				     "Internal error, can't use NULL device");
	jmp	@BLBL78
@BLBL77:

; 960 		    else if( dsk->getParent() != NULL )
	mov	eax,[ebp-038h];	dsk
	cmp	dword ptr [eax+04h],0h
	je	@BLBL79

; 961 			MyMessageBox(1, hwnd,
	mov	edx,[ebp+08h];	hwnd
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE38
	mov	eax,01h
	call	MyMessageBox__FCiCUlPce
	add	esp,0ch

; 963 				     " remove parent first.");
	jmp	@BLBL78
@BLBL79:

; 964 		    else if( dsk->isHostdrive()  &&  dsk->isArray() == 0 )
	sub	esp,04h
	mov	edi,[ebp-038h];	dsk
	mov	ecx,[edi]
	mov	eax,[ecx+06ch]
	add	eax,edi
	call	dword ptr [ecx+068h]
	add	esp,04h
	test	al,al
	mov	eax,edi
	je	@BLBL81
	mov	ecx,[eax]
	sub	esp,04h
	mov	edx,[ecx+064h]
	add	eax,edx
	call	dword ptr [ecx+060h]
	add	esp,04h
	test	al,al
	jne	@BLBL81

; 965 			MyMessageBox(1, hwnd,
	mov	edx,[ebp+08h];	hwnd
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE39
	mov	eax,01h
	call	MyMessageBox__FCiCUlPce
	add	esp,0ch

; 967 				     " create physdevice first.");
	jmp	@BLBL78
@BLBL81:

; 968 		    else if( !dsk->isWritable() )
	sub	esp,04h
	mov	eax,[ebp-038h];	dsk
	mov	ebx,[eax]
	mov	ecx,[ebx+0ch]
	add	eax,ecx
	call	dword ptr [ebx+08h]
	add	esp,04h
	test	al,al
	jne	@BLBL83

; 969 			MyMessageBox(1, hwnd,
	mov	edx,[ebp+08h];	hwnd
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE40
	mov	eax,01h
	call	MyMessageBox__FCiCUlPce
	add	esp,0ch

; 972 				     " first.");
	jmp	@BLBL78
@BLBL83:

; 975 			bool = (BOOL)WinSendMsg(rec->hwndCnr,
	mov	ebx,[ebp-03ch];	rec
	push	080001h
	push	dword ptr [ebx+08h]
	push	034ah
	push	dword ptr [ebx]
	call	WinSendMsg
	add	esp,010h
	mov	[ebp-024h],eax;	bool

; 979 			if( bool == FALSE )
	test	eax,eax
	jne	@BLBL78

; 980 			    Verbose(1, "DrvSetup",
	mov	ebx,[ebp-020h];	work
	push	dword ptr [ebx+04h]
	call	WinGetLastError
	add	esp,04h
	mov	ecx,offset FLAT:@CBE41
	push	eax
	mov	edx,offset FLAT:@CBE3
	sub	esp,0ch
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 984 		    }
@BLBL78:

; 986 		return 0;
	pop	esi
	xor	eax,eax
	pop	edi
	pop	ebx
	add	esp,0460h
	pop	fs:[0h]
	leave	
	ret	
@BLBL167:

; 990 		Verbose(3, "DrvSetup", "WM_CONTROL (id %#x, CN_INITDRAG)", id);
	mov	ecx,offset FLAT:@CBE42
	movzx	ebx,word ptr [ebp-02ah];	id
	push	ebx
	mov	edx,offset FLAT:@CBE3
	sub	esp,0ch
	mov	eax,03h
	call	Verbose
	add	esp,04h

; 991 		return DragInit(hwnd, work,(PCNRDRAGINIT)PVOIDFROMMP(mp2));
	mov	ecx,[ebp+014h];	mp2
	mov	edx,[ebp-020h];	work
	mov	eax,[ebp+08h];	hwnd
	call	DragInit__FUlCP5_WORKCP12_CNRDRAGINIT
	add	esp,0ch
	pop	esi
	pop	edi
	pop	ebx
	add	esp,0460h
	pop	fs:[0h]
	leave	
	ret	
@BLBL168:

; 994 		Verbose(3, "DrvSetup", "WM_CONTROL (id %#x, CN_DRAGOVER)", id);
	mov	ecx,offset FLAT:@CBE43
	movzx	ebx,word ptr [ebp-02ah];	id
	push	ebx
	mov	edx,offset FLAT:@CBE3
	sub	esp,0ch
	mov	eax,03h
	call	Verbose
	add	esp,04h

; 995 		return DragOver(hwnd, work, (PCNRDRAGINFO)PVOIDFROMMP(mp2));
	mov	ecx,[ebp+014h];	mp2
	mov	edx,[ebp-020h];	work
	mov	eax,[ebp+08h];	hwnd
	call	DragOver__FUlCP5_WORKP12_CNRDRAGINFO
	add	esp,0ch
	pop	esi
	pop	edi
	pop	ebx
	add	esp,0460h
	pop	fs:[0h]
	leave	
	ret	
@BLBL169:

; 998 		Verbose(3, "DrvSetup", "WM_CONTROL (id %#x, CN_DRAGLEAVE)", id);
	mov	ecx,offset FLAT:@CBE44
	movzx	ebx,word ptr [ebp-02ah];	id
	push	ebx
	mov	edx,offset FLAT:@CBE3
	sub	esp,0ch
	mov	eax,03h
	call	Verbose
	add	esp,04h

; 999 		return DragLeave(hwnd, work, (PCNRDRAGINFO)PVOIDFROMMP(mp2));
	mov	ecx,[ebp+014h];	mp2
	mov	edx,[ebp-020h];	work
	mov	eax,[ebp+08h];	hwnd
	call	DragLeave__FUlCP5_WORKP12_CNRDRAGINFO
	add	esp,0ch
	pop	esi
	pop	edi
	pop	ebx
	add	esp,0460h
	pop	fs:[0h]
	leave	
	ret	
@BLBL170:

; 1002 		Verbose(3, "DrvSetup", "WM_CONTROL (id %#x, CN_DROP)", id);
	mov	ecx,offset FLAT:@CBE45
	movzx	ebx,word ptr [ebp-02ah];	id
	push	ebx
	mov	edx,offset FLAT:@CBE3
	sub	esp,0ch
	mov	eax,03h
	call	Verbose
	add	esp,04h

; 1003 		return DragDrop(hwnd, work, (PCNRDRAGINFO)PVOIDFROMMP(mp2));
	mov	ecx,[ebp+014h];	mp2
	mov	edx,[ebp-020h];	work
	mov	eax,[ebp+08h];	hwnd
	call	DragDrop__FUlCP5_WORKP12_CNRDRAGINFO
	add	esp,0ch
	pop	esi
	pop	edi
	pop	ebx
	add	esp,0460h
	pop	fs:[0h]
	leave	
	ret	
@BLBL171:

; 1006 		Verbose(3, "DrvSetup", "WM_CONTROL (id %#x, notify code %#x)",
	mov	ecx,offset FLAT:@CBE46
	movzx	eax,word ptr [ebp-066h];	ncode
	push	eax
	movzx	eax,word ptr [ebp-02ah];	id
	push	eax
	mov	edx,offset FLAT:@CBE3
	sub	esp,0ch
	mov	eax,03h
	call	Verbose
	add	esp,014h

; 1011 	break;
	jmp	@BLBL147
@BLBL151:

; 1018 	Verbose(3, "DrvSetup", "WM_CONTEXTMENU");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE47
	mov	edx,offset FLAT:@CBE3
	mov	eax,03h
	call	Verbose
	add	esp,04h

; 1019 	DoPopup(hwnd, work);
	mov	edx,[ebp-020h];	work
	mov	eax,[ebp+08h];	hwnd
	call	DoPopup__FUlCP5_WORK
	add	esp,08h

; 1020 	break;
	jmp	@BLBL147
@BLBL152:

; 1029 	Verbose(3, "DrvSetup", "WM_MENUEND");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE48
	mov	edx,offset FLAT:@CBE3
	mov	eax,03h
	call	Verbose
	add	esp,0ch

; 1030 	if( work->pmrMenu == NULL )
	mov	ebx,[ebp-020h];	work
	cmp	dword ptr [ebx+01ch],0h
	jne	@BLBL86

; 1032 	    WinSendMsg(work->hwndCnr, CM_SETRECORDEMPHASIS,
	push	040000000h
	push	0h
	push	034ah
	push	dword ptr [ebx+08h]
	call	WinSendMsg
	add	esp,010h

; 1034 	}
	jmp	@BLBL87
@BLBL86:

; 1043 	    SearchCnr(work->hwndCnr, (PFNSRCH)EmphSource, &bool, &dummy, NULL);
	push	0h
	lea	ebx,[ebp-040h];	dummy
	push	ebx

; 1041 	    bool = FALSE;
	mov	dword ptr [ebp-024h],0h;	bool

; 1042 	    dummy = 0;
	mov	dword ptr [ebp-040h],0h;	dummy

; 1043 	    SearchCnr(work->hwndCnr, (PFNSRCH)EmphSource, &bool, &dummy, NULL);
	sub	esp,0ch
	mov	eax,[ebp-020h];	work
	lea	ecx,[ebp-024h];	bool
	mov	eax,[eax+08h]
	mov	edx,offset FLAT: EmphSource__FUlP9_MYRECORDPUlT3
	call	SearchCnr__FUlPFUlPvT2PUl_UlPvN23
	add	esp,014h

; 1044 	}
@BLBL87:

; 1045 	break;
	jmp	@BLBL147
@BLBL153:

; 1053 	Verbose(3, "DrvSetup", "WM_COMMAND");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE49
	mov	edx,offset FLAT:@CBE3
	mov	eax,03h
	call	Verbose

; 1055 	    USHORT const cmd = SHORT1FROMMP(mp1);
	mov	eax,[ebp+010h];	mp1

; 1053 	Verbose(3, "DrvSetup", "WM_COMMAND");
	add	esp,0ch

; 1055 	    USHORT const cmd = SHORT1FROMMP(mp1);
	mov	[ebp-068h],ax;	cmd

; 1058 	    switch( cmd )
	movzx	eax,ax
	cmp	eax,01bah
	jl	@BLBL180
	je	@BLBL176
	cmp	eax,01bbh
	je	@BLBL178
	cmp	eax,01bch
	je	@BLBL175
	cmp	eax,01bdh
	je	@BLBL174
	jmp	@BLBL179
@BLBL180:
	cmp	eax,01b8h
	je	@BLBL173
	cmp	eax,01b9h
	je	@BLBL177
	jmp	@BLBL179
@BLBL173:

; 1061 		Verbose(3, "DrvSetup", "WM_COMMAND, IDM_CREATE_PDEV");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE50
	mov	edx,offset FLAT:@CBE3
	mov	eax,03h
	call	Verbose
	add	esp,0ch

; 1063 		    PMINIRECORDCORE	ap[1] = {(PMINIRECORDCORE)work->pmrMenu};
	mov	ebx,[ebp-020h];	work

; 1067 		    WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
	push	030001h

; 1063 		    PMINIRECORDCORE	ap[1] = {(PMINIRECORDCORE)work->pmrMenu};
	mov	ecx,[ebx+01ch]
	mov	[ebp-044h],ecx;	ap

; 1067 		    WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
	lea	ecx,[ebp-044h];	ap
	push	ecx
	push	0346h
	push	dword ptr [ebx+08h]
	call	WinSendMsg
	add	esp,010h

; 1073 		    WinPostMsg(work->hwndScanner, WUM_MKPDEV,
	mov	ecx,[ebx+01ch]
	push	0h
	push	dword ptr [ecx+01ch]
	push	0100dh
	push	dword ptr [ebx+0ch]
	call	WinPostMsg
	add	esp,010h

; 1076 		return 0;
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	add	esp,0460h
	pop	fs:[0h]
	leave	
	ret	
@BLBL174:

; 1081 		Verbose(3, "DrvSetup", "WM_COMMAND, IDM_COMBINE_DRV");
	sub	esp,0ch
	mov	ebx,offset FLAT:@CBE3
	mov	ecx,offset FLAT:@CBE51
	mov	edx,ebx
	mov	eax,03h
	call	Verbose
	add	esp,0ch

; 1089 		    SearchCnr(work->hwndCnr, (PFNSRCH)QueryInuse,
	lea	eax,[ebp-0e8h];	info
	push	0h

; 1088 		    info.cnt = 0;
	mov	dword ptr [ebp-0ech],0h;	info

; 1089 		    SearchCnr(work->hwndCnr, (PFNSRCH)QueryInuse,
	push	eax
	lea	ecx,[ebp-0ech];	info
	sub	esp,0ch
	mov	eax,[ebp-020h];	work
	mov	edx,offset FLAT: QueryInuse__FUlP9_MYRECORDPUlPP9_MYRECORD
	mov	eax,[eax+08h]
	call	SearchCnr__FUlPFUlPvT2PUl_UlPvN23
	add	esp,014h
	mov	edx,ebx

; 1091 		    Verbose(2, "DrvSetup", "%lu drives are \"in-use\"",info.cnt);
	push	dword ptr [ebp-0ech];	info
	mov	ecx,offset FLAT:@CBE52
	sub	esp,0ch
	mov	eax,02h
	call	Verbose
	add	esp,010h

; 1095 		    ul = WinDlgBox(HWND_DESKTOP, hwnd, DrvtypeDlgProc,
	lea	eax,[ebp-0f0h];	info
	push	eax
	push	02bch
	push	0h
	push	offset FLAT: DrvtypeDlgProc
	push	dword ptr [ebp+08h];	hwnd
	push	01h
	call	WinDlgBox
	add	esp,018h

; 1097 		    if( DID_OK == ul )
	cmp	eax,01h
	jne	@BLBL88

; 1107 			memcpy(&newid[0], &usHostId, sizeof(usHostId));
	lea	eax,[ebp-030h];	newid
	sub	esp,0ch
	mov	ecx,02h
	mov	edx,offset FLAT:usHostId
	call	memcpy
	add	esp,08h

; 1108 			*(PULONG)&newid[2] = time(NULL);
	xor	eax,eax
	call	time
	add	esp,04h

; 1110 			switch( info.type )
	mov	ebx,[ebp-0f0h];	info

; 1108 			*(PULONG)&newid[2] = time(NULL);
	mov	[ebp-02eh],eax;	newid

; 1110 			switch( info.type )
	cmp	ebx,02h
	jl	@BLBL186
	je	@BLBL183
	cmp	ebx,03h
	je	@BLBL184
	cmp	ebx,04h
	je	@BLBL185
	jmp	@BLBL181
@BLBL186:
	cmp	ebx,01h
	jne	@BLBL181
@BLBL182:

; 1113 			    drv = new VSingle(newid);
	sub	esp,04h
	mov	eax,030h
	call	__nw__FUi
	add	esp,04h
	mov	[ebp-010h],eax;	__66
	test	eax,eax
	je	@BLBL90
	lea	edx,[ebp-030h];	newid
	sub	esp,08h
	mov	dword ptr [ebp-0104h],01h;	__es
	call	__ct__7VSingleFPUc
	add	esp,08h
	mov	dword ptr [ebp-0104h],0h;	__es
@BLBL90:
	mov	eax,[ebp-010h];	__66
	mov	[ebp-034h],eax;	drv

; 1114 			    break;
	jmp	@BLBL181
@BLBL183:

; 1116 			    drv = new VChain(newid, (int)info.cnt);
	sub	esp,04h
	mov	eax,0130h
	call	__nw__FUi
	add	esp,04h
	mov	[ebp-014h],eax;	__67
	test	eax,eax
	je	@BLBL92
	lea	edx,[ebp-030h];	newid
	sub	esp,0ch
	mov	ecx,[ebp-0ech];	info
	mov	dword ptr [ebp-0104h],02h;	__es
	call	__ct__6VChainFPUci
	add	esp,0ch
	mov	dword ptr [ebp-0104h],0h;	__es
@BLBL92:
	mov	eax,[ebp-014h];	__67
	mov	[ebp-034h],eax;	drv

; 1117 			    break;
	jmp	@BLBL181
@BLBL184:

; 1119 			    drv = new VStripe(newid, (int)info.cnt);
	sub	esp,04h
	mov	eax,0130h
	call	__nw__FUi
	add	esp,04h
	mov	[ebp-018h],eax;	__68
	test	eax,eax
	je	@BLBL94
	lea	edx,[ebp-030h];	newid
	sub	esp,0ch
	mov	ecx,[ebp-0ech];	info
	mov	dword ptr [ebp-0104h],03h;	__es
	call	__ct__7VStripeFPUci
	add	esp,0ch
	mov	dword ptr [ebp-0104h],0h;	__es
@BLBL94:
	mov	eax,[ebp-018h];	__68
	mov	[ebp-034h],eax;	drv

; 1120 			    break;
	jmp	@BLBL181
@BLBL185:

; 1122 			    drv = new VMirror(newid, (int)info.cnt);
	sub	esp,04h
	mov	eax,0134h
	call	__nw__FUi
	add	esp,04h
	mov	[ebp-01ch],eax;	__69
	test	eax,eax
	je	@BLBL96
	lea	edx,[ebp-030h];	newid
	sub	esp,0ch
	mov	ecx,[ebp-0ech];	info
	mov	dword ptr [ebp-0104h],04h;	__es
	call	__ct__7VMirrorFPUci
	add	esp,0ch
	mov	dword ptr [ebp-0104h],0h;	__es
@BLBL96:
	mov	eax,[ebp-01ch];	__69
	mov	[ebp-034h],eax;	drv

; 1123 			    break;
@BLBL181:

; 1130 			for( ul = 0; ul < info.cnt; ++ul )
	cmp	dword ptr [ebp-0ech],0h;	info
	jbe	@BLBL97
	mov	ebx,[ebp-034h];	drv
	mov	esi,[ebp-020h];	work
	xor	edi,edi
@BLBL98:

; 1135 			    drv->addChild(info.child[ul]->dsk, False, Tru
; 1135 e);
	push	01h

; 1132 			    PMINIRECORDCORE	ap[1]
	lea	eax,dword ptr [ebp+edi*04h-0f0h]

; 1135 			    drv->addChild(info.child[ul]->dsk, False, Tru
; 1135 e);
	sub	esp,0ch

; 1132 			    PMINIRECORDCORE	ap[1]
	mov	edx,[eax+08h]
	mov	[ebp-0468h],eax;	@CBE89
	mov	[ebp-048h],edx;	ap

; 1135 			    drv->addChild(info.child[ul]->dsk, False, Tru
; 1135 e);
	mov	esi,[ebx]
	mov	edx,[edx+01ch]
	mov	eax,[esi+08ch]
	xor	ecx,ecx
	add	eax,ebx
	call	dword ptr [esi+088h]
	mov	eax,[ebp-0468h];	@CBE89
	mov	esi,[ebp-020h];	work

; 1136 			    if( !info.child[ul]->dsk->isWritable() )
	mov	eax,[eax+08h]

; 1135 			    drv->addChild(info.child[ul]->dsk, False, Tru
; 1135 e);
	add	esp,0ch

; 1136 			    if( !info.child[ul]->dsk->isWritable() )
	mov	eax,[eax+01ch]
	mov	ecx,[eax]
	mov	edx,[ecx+0ch]
	add	eax,edx
	call	dword ptr [ecx+08h]
	add	esp,04h
	test	al,al
	jne	@BLBL99

; 1137 				drv->forceUnwritable();
	mov	ecx,[ebx]
	sub	esp,04h
	mov	eax,[ecx+034h]
	add	eax,ebx
	call	dword ptr [ecx+030h]
	add	esp,04h
@BLBL99:

; 1141 			    WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
	push	030001h
	lea	eax,[ebp-048h];	ap
	push	eax

; 1130 			for( ul = 0; ul < info.cnt; ++ul )
	inc	edi

; 1141 			    WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
	push	0346h
	push	dword ptr [esi+08h]
	call	WinSendMsg
	add	esp,010h

; 1130 			for( ul = 0; ul < info.cnt; ++ul )
	cmp	[ebp-0ech],edi;	info
	ja	@BLBL98
@BLBL97:

; 1145 			WinPostMsg(work->hwndScanner, WUM_DEVTREE, drv, NULL);
	push	0h
	mov	ebx,[ebp-020h];	work
	push	dword ptr [ebp-034h];	drv
	push	0100ah
	push	dword ptr [ebx+0ch]
	call	WinPostMsg
	add	esp,010h

; 1146 		    }
	jmp	@BLBL103
@BLBL88:

; 1151 			SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);
	push	0h
	lea	ebx,[ebp-028h];	dummy
	push	ebx

; 1149 			bool = FALSE;
	mov	dword ptr [ebp-024h],0h;	bool

; 1150 			dummy = 0;
	mov	dword ptr [ebp-028h],0h;	dummy

; 1151 			SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);
	sub	esp,0ch
	mov	eax,[ebp-020h];	work
	lea	ecx,[ebp-024h];	bool
	mov	eax,[eax+08h]
	mov	edx,offset FLAT: EmphInuse__FUlP9_MYRECORDPUlT3
	call	SearchCnr__FUlPFUlPvT2PUl_UlPvN23
	add	esp,014h

; 1152 		    }
@BLBL103:

; 1154 		return 0;
	pop	esi
	xor	eax,eax
	pop	edi
	pop	ebx
	add	esp,0460h
	pop	fs:[0h]
	leave	
	ret	
@BLBL175:

; 1162 		SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);
	push	0h
	lea	ebx,[ebp-028h];	dummy
	push	ebx

; 1160 		bool = FALSE;
	mov	dword ptr [ebp-024h],0h;	bool

; 1161 		dummy = 0;
	mov	dword ptr [ebp-028h],0h;	dummy

; 1162 		SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);
	sub	esp,0ch
	mov	eax,[ebp-020h];	work
	lea	ecx,[ebp-024h];	bool
	mov	eax,[eax+08h]
	mov	edx,offset FLAT: EmphInuse__FUlP9_MYRECORDPUlT3
	call	SearchCnr__FUlPFUlPvT2PUl_UlPvN23
	add	esp,014h

; 1163 		return 0;
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	add	esp,0460h
	pop	fs:[0h]
	leave	
	ret	
@BLBL176:

; 1170 		Verbose(3, "DrvSetup", "WM_COMMAND, IDM_DESTROY_DRV");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE53
	mov	edx,offset FLAT:@CBE3
	mov	eax,03h
	call	Verbose
	add	esp,0ch

; 1176 		SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);
	lea	ecx,[ebp-028h];	dummy
	push	0h

; 1174 		bool = FALSE;
	mov	dword ptr [ebp-024h],0h;	bool

; 1176 		SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);
	push	ecx

; 1175 		dummy = 0;
	mov	dword ptr [ebp-028h],0h;	dummy

; 1176 		SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);
	sub	esp,0ch
	mov	ebx,[ebp-020h];	work
	lea	ecx,[ebp-024h];	bool
	mov	eax,[ebx+08h]
	mov	edx,offset FLAT: EmphInuse__FUlP9_MYRECORDPUlT3
	call	SearchCnr__FUlPFUlPvT2PUl_UlPvN23
	add	esp,014h

; 1181 		    VRDrive *	drv = (VRDrive *)work->pmrMenu->dsk; /* !!! */
	mov	edi,[ebx+01ch]

; 1184 		    response = WinMessageBox(HWND_DESKTOP, hwnd,
	push	04014h

; 1181 		    VRDrive *	drv = (VRDrive *)work->pmrMenu->dsk; /* !!! */
	mov	edi,[edi+01ch]

; 1184 		    response = WinMessageBox(HWND_DESKTOP, hwnd,
	push	0ffffffffh
	push	offset FLAT:@CBE29
	push	offset FLAT:@CBE54
	push	dword ptr [ebp+08h];	hwnd
	push	01h
	call	WinMessageBox
	add	esp,018h

; 1187 		    if( response == MBID_YES )
	cmp	eax,06h
	jne	@BLBL104

; 1193 			WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
	push	030001h

; 1189 			PMINIRECORDCORE	ap[1] = {(PMINIRECORDCORE)work->pmrMenu};
	mov	ecx,[ebx+01ch]
	mov	[ebp-04ch],ecx;	ap

; 1193 			WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
	lea	ecx,[ebp-04ch];	ap
	push	ecx
	push	0346h
	push	dword ptr [ebx+08h]
	call	WinSendMsg
	add	esp,010h

; 1200 			WinPostMsg(work->hwndScanner, WUM_DELDRV, drv, NULL);
	push	0h
	push	edi
	push	0100ch
	push	dword ptr [ebx+0ch]
	call	WinPostMsg
	add	esp,010h

; 1201 		    }
@BLBL104:

; 1203 		return 0;
	pop	esi
	xor	eax,eax
	pop	edi
	pop	ebx
	add	esp,0460h
	pop	fs:[0h]
	leave	
	ret	
@BLBL177:

; 1206 		Verbose(3, "DrvSetup", "WM_COMMAND, IDM_DELETE_PDEV");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE55
	mov	edx,offset FLAT:@CBE3
	mov	eax,03h
	call	Verbose
	add	esp,0ch

; 1212 		SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);
	lea	ecx,[ebp-028h];	dummy
	push	0h

; 1210 		bool = FALSE;
	mov	dword ptr [ebp-024h],0h;	bool

; 1212 		SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);
	push	ecx

; 1211 		dummy = 0;
	mov	dword ptr [ebp-028h],0h;	dummy

; 1212 		SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);
	sub	esp,0ch
	mov	ebx,[ebp-020h];	work
	lea	ecx,[ebp-024h];	bool
	mov	eax,[ebx+08h]
	mov	edx,offset FLAT: EmphInuse__FUlP9_MYRECORDPUlT3
	call	SearchCnr__FUlPFUlPvT2PUl_UlPvN23

; 1217 		    VRDev *	rdev = work->pmrMenu->dsk;
	mov	edi,[ebx+01ch]

; 1212 		SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);
	add	esp,014h

; 1217 		    VRDev *	rdev = work->pmrMenu->dsk;
	mov	edi,[edi+01ch]

; 1221 		    sprintf(str,"Are you sure you want to destroy"
	lea	ecx,[edi+08h]
	movzx	edx,byte ptr [ecx+05h]
	push	edx
	movzx	edx,byte ptr [ecx+04h]
	push	edx
	movzx	edx,byte ptr [ecx+03h]
	push	edx
	movzx	edx,byte ptr [ecx+02h]
	push	edx
	movzx	edx,byte ptr [ecx+01h]
	push	edx
	movzx	ecx,byte ptr [ecx]
	push	ecx
	lea	esi,[ebp-0378h];	str
	sub	esp,08h
	mov	edx,offset FLAT:@CBE56
	mov	eax,esi
	call	_sprintfieee
	add	esp,020h

; 1226 		    response = WinMessageBox(HWND_DESKTOP, hwnd,
	push	04014h
	push	0ffffffffh
	push	offset FLAT:@CBE29
	push	esi
	push	dword ptr [ebp+08h];	hwnd
	push	01h
	call	WinMessageBox
	add	esp,018h

; 1229 		    if( response == MBID_YES )
	cmp	eax,06h
	jne	@BLBL105

; 1235 			WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
	push	030001h

; 1231 			PMINIRECORDCORE	ap[1] = {(PMINIRECORDCORE)work->pmrMenu};
	mov	ecx,[ebx+01ch]
	mov	[ebp-050h],ecx;	ap

; 1235 			WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
	lea	ecx,[ebp-050h];	ap
	push	ecx
	push	0346h
	push	dword ptr [ebx+08h]
	call	WinSendMsg
	add	esp,010h

; 1239 			WinPostMsg(work->hwndScanner, WUM_DELPDEV, rdev, NULL);
	push	0h
	push	edi
	push	0100bh
	push	dword ptr [ebx+0ch]
	call	WinPostMsg
	add	esp,010h

; 1240 		    }
@BLBL105:

; 1242 		return 0;
	pop	esi
	xor	eax,eax
	pop	edi
	pop	ebx
	add	esp,0460h
	pop	fs:[0h]
	leave	
	ret	
@BLBL178:

; 1245 		Verbose(3, "DrvSetup", "WM_COMMAND, IDM_REMOVE_CHILD");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE57
	mov	edx,offset FLAT:@CBE3
	mov	eax,03h
	call	Verbose
	add	esp,0ch

; 1251 		SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);
	lea	ecx,[ebp-028h];	dummy
	push	0h

; 1249 		bool = FALSE;
	mov	dword ptr [ebp-024h],0h;	bool

; 1251 		SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);
	push	ecx

; 1250 		dummy = 0;
	mov	dword ptr [ebp-028h],0h;	dummy

; 1251 		SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);
	sub	esp,0ch
	mov	ebx,[ebp-020h];	work
	lea	ecx,[ebp-024h];	bool
	mov	eax,[ebx+08h]
	mov	edx,offset FLAT: EmphInuse__FUlP9_MYRECORDPUlT3
	call	SearchCnr__FUlPFUlPvT2PUl_UlPvN23

; 1256 		    VRDev *	rdev = work->pmrMenu->dsk;
	mov	ebx,[ebx+01ch]

; 1251 		SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);
	add	esp,014h

; 1256 		    VRDev *	rdev = work->pmrMenu->dsk;
	mov	ebx,[ebx+01ch]
	mov	[ebp-060h],ebx;	rdev

; 1260 		    sprintf(str,"Are you sure you want to remove"
	lea	ecx,[ebx+08h]
	movzx	edx,byte ptr [ecx+05h]
	push	edx
	movzx	edx,byte ptr [ecx+04h]
	push	edx
	movzx	edx,byte ptr [ecx+03h]
	push	edx
	movzx	edx,byte ptr [ecx+02h]
	push	edx
	movzx	edx,byte ptr [ecx+01h]
	push	edx
	movzx	ecx,byte ptr [ecx]
	push	ecx
	lea	edi,[ebp-0440h];	str
	sub	esp,08h
	mov	edx,offset FLAT:@CBE58
	mov	eax,edi
	call	_sprintfieee
	add	esp,020h

; 1265 		    response = WinMessageBox(HWND_DESKTOP, hwnd,
	push	04014h
	push	0ffffffffh
	push	offset FLAT:@CBE29
	push	edi
	push	dword ptr [ebp+08h];	hwnd
	push	01h
	call	WinMessageBox
	add	esp,018h

; 1268 		    if( response == MBID_YES )
	cmp	eax,06h
	jne	@BLBL106

; 1272 			VRDrive *	parent = rdev->getParent();
	mov	ecx,[ebx+04h]
	mov	[ebp-064h],ecx;	parent

; 1276 			VRDev *	highest = parent;
	mov	[ebp-054h],ecx;	highest

; 1277 			while( highest->getParent() != 0 )
	cmp	dword ptr [ecx+04h],0h
	je	@BLBL107
@BLBL108:

; 1278 			    highest = highest->getParent();
	mov	ecx,[ecx+04h]

; 1277 			while( highest->getParent() != 0 )
	cmp	dword ptr [ecx+04h],0h
	jne	@BLBL108
	mov	[ebp-054h],ecx;	highest
@BLBL107:

; 1279 			SearchCnr(work->hwndCnr, (PFNSRCH)LookupDisk,
	push	0h
	lea	ecx,[ebp-058h];	myrcd
	push	ecx
	mov	ecx,[ebp-054h];	highest
	sub	esp,0ch
	mov	ebx,[ebp-020h];	work
	mov	edx,offset FLAT: LookupDisk__FUlP9_MYRECORDP5VRDevPP9_MYRECORD
	mov	eax,[ebx+08h]
	call	SearchCnr__FUlPFUlPvT2PUl_UlPvN23
	add	esp,014h

; 1281 			ap[0] = (PMINIRECORDCORE)myrcd;
	mov	ecx,[ebp-058h];	myrcd

; 1283 			WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
	push	030001h

; 1281 			ap[0] = (PMINIRECORDCORE)myrcd;
	mov	[ebp-05ch],ecx;	ap

; 1283 			WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
	lea	ecx,[ebp-05ch];	ap
	push	ecx
	push	0346h
	push	dword ptr [ebx+08h]
	call	WinSendMsg
	add	esp,010h

; 1290 			WinPostMsg(work->hwndScanner, WUM_REMCHILD, rdev, parent);
	push	dword ptr [ebp-064h];	parent
	push	dword ptr [ebp-060h];	rdev
	push	0100eh
	push	dword ptr [ebx+0ch]
	call	WinPostMsg
	add	esp,010h

; 1291 		    }
@BLBL106:

; 1293 		return 0;
	pop	esi
	xor	eax,eax
	pop	edi
	pop	ebx
	add	esp,0460h
	pop	fs:[0h]
	leave	
	ret	
@BLBL179:

; 1297 		Verbose(2, "DrvSetup", "WM_COMMAND %u", cmd);
	mov	ecx,offset FLAT:@CBE59
	movzx	eax,word ptr [ebp-068h];	cmd
	push	eax
	mov	edx,offset FLAT:@CBE3
	sub	esp,0ch
	mov	eax,02h
	call	Verbose
	add	esp,010h

; 1301 	break;					/* continue with default */
	jmp	@BLBL147
@BLBL154:

; 1309 	Verbose(3, "DrvSetup", "WM_CREATE");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE60
	mov	edx,offset FLAT:@CBE3
	mov	eax,03h
	call	Verbose
	add	esp,08h

; 1310 	LoadPresParam(WC_DRVSETUP);
	mov	eax,offset FLAT:@CBE61
	call	LoadPresParam__FPc
	add	esp,04h

; 1311 	break;					/* keep on going... */
	jmp	@BLBL147
@BLBL155:

; 1319 	Verbose(3, "DrvSetup", "WM_CLOSE");
	sub	esp,0ch
	mov	ebx,offset FLAT:@CBE3
	mov	ecx,offset FLAT:@CBE62
	mov	edx,ebx
	mov	eax,03h
	call	Verbose
	add	esp,0ch

; 1327 	    SearchCnr(work->hwndCnr, (PFNSRCH)QueryAny,
	lea	eax,[ebp-0184h];	info
	push	0h

; 1326 	    info.cnt = 0;
	mov	dword ptr [ebp-0188h],0h;	info

; 1327 	    SearchCnr(work->hwndCnr, (PFNSRCH)QueryAny,
	push	eax
	lea	ecx,[ebp-0188h];	info
	sub	esp,0ch
	mov	edi,[ebp-020h];	work
	mov	edx,offset FLAT: QueryAny__FUlP9_MYRECORDPUlPP9_MYRECORD
	mov	eax,[edi+08h]
	call	SearchCnr__FUlPFUlPvT2PUl_UlPvN23
	add	esp,014h
	mov	edx,ebx

; 1329 	    Verbose(2, "DrvSetup", "%lu drives are available", info.cnt);
	push	dword ptr [ebp-0188h];	info
	mov	ecx,offset FLAT:@CBE63
	sub	esp,0ch
	mov	eax,02h
	call	Verbose
	add	esp,010h

; 1330 	    for( ul = 0; ul < info.cnt; ++ul )
	cmp	dword ptr [ebp-0188h],0h;	info
	jbe	@BLBL112
	mov	esi,edi
	xor	edi,edi
@BLBL113:

; 1332 		WinPostMsg(work->hwndScanner, WUM_CLOSEDEV,
	push	0h
	mov	eax,dword ptr [ebp+edi*04h-0184h]

; 1330 	    for( ul = 0; ul < info.cnt; ++ul )
	inc	edi

; 1332 		WinPostMsg(work->hwndScanner, WUM_CLOSEDEV,
	push	dword ptr [eax+01ch]
	push	0100fh
	push	dword ptr [esi+0ch]
	call	WinPostMsg
	add	esp,010h

; 1330 	    for( ul = 0; ul < info.cnt; ++ul )
	cmp	[ebp-0188h],edi;	info
	ja	@BLBL113
@BLBL112:

; 1336 	WinPostMsg(work->hwndScanner, WM_CLOSE, 0, 0);
	push	0h
	mov	ebx,[ebp-020h];	work
	push	0h
	push	029h
	push	dword ptr [ebx+0ch]
	call	WinPostMsg
	add	esp,010h

; 1337 	break;					/* continue... */
	jmp	@BLBL147
@BLBL156:

; 1347 	Verbose(3, "DrvSetup", "WM_DESTROY");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE64
	mov	edx,offset FLAT:@CBE3
	mov	eax,03h
	call	Verbose
	add	esp,0ch

; 1349 	    WinDestroyWindow(work->hwndSngPopup);
	mov	ebx,[ebp-020h];	work
	push	dword ptr [ebx+014h]
	call	WinDestroyWindow
	add	esp,04h

; 1350 	    WinDestroyWindow(work->hwndMulPopup);
	push	dword ptr [ebx+018h]
	call	WinDestroyWindow
	add	esp,04h

; 1351 	    WinDestroyWindow(work->hwndCnr);
	push	dword ptr [ebx+08h]
	call	WinDestroyWindow
	add	esp,04h

; 1352 	    WinPostMsg(work->self->parent, WUM_THREADENDED, work->self, 0);
	mov	ecx,[ebx]
	push	0h
	push	ecx
	push	01003h
	push	dword ptr [ecx]
	call	WinPostMsg
	add	esp,010h

; 1354 	break;
	jmp	@BLBL147
@BLBL157:

; 1368 	    if( WinQueryWindowPos(WinQueryWindow(hwnd, QW_PARENT), &size) == TRUE )
	lea	ebx,[ebp-01b0h];	size

; 1364 	Verbose(3, "DrvSetup", "WM_SIZE");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE65
	mov	edx,offset FLAT:@CBE3
	mov	eax,03h
	call	Verbose
	add	esp,0ch

; 1368 	    if( WinQueryWindowPos(WinQueryWindow(hwnd, QW_PARENT), &size) == TRUE )
	push	05h
	push	dword ptr [ebp+08h];	hwnd
	call	WinQueryWindow
	add	esp,08h
	push	ebx
	push	eax
	call	WinQueryWindowPos
	add	esp,08h
	cmp	eax,01h
	jne	@BLBL117

; 1369 		ProfileWrite(WC_DRVSETUP, "Size", sizeof(size), &size);
	push	ebx
	mov	ecx,024h
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE66
	mov	eax,offset FLAT:@CBE61
	call	ProfileWrite
	add	esp,010h
@BLBL117:

; 1371 	    if( WinQueryWindowPos(hwnd, &size) == TRUE )
	lea	ebx,[ebp-01b0h];	size
	push	ebx
	push	dword ptr [ebp+08h];	hwnd
	call	WinQueryWindowPos
	add	esp,08h
	cmp	eax,01h
	jne	@BLBL118

; 1372 		WinSetWindowPos(work->hwndCnr, HWND_TOP,
	push	08bh
	mov	ecx,[ebp+014h];	mp2
	mov	ebx,ecx
	shr	ecx,010h
	movzx	ecx,cx
	push	ecx
	movzx	ebx,bx
	push	ebx
	mov	ebx,[ebp-020h];	work
	push	0h
	push	0h
	push	03h
	push	dword ptr [ebx+08h]
	call	WinSetWindowPos
	add	esp,01ch

; 1375 				SWP_SIZE|SWP_MOVE|SWP_SHOW|SWP_ACTIVATE);
@BLBL118:

; 1377 	break;
	jmp	@BLBL147
@BLBL158:

; 1389 	    if( WinQueryWindowPos(WinQueryWindow(hwnd, QW_PARENT), &swp) == TRUE )
	lea	ebx,[ebp-0464h];	swp

; 1385 	Verbose(3, "DrvSetup", "WM_MOVE");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE67
	mov	edx,offset FLAT:@CBE3
	mov	eax,03h
	call	Verbose
	add	esp,0ch

; 1389 	    if( WinQueryWindowPos(WinQueryWindow(hwnd, QW_PARENT), &swp) == TRUE )
	push	05h
	push	dword ptr [ebp+08h];	hwnd
	call	WinQueryWindow
	add	esp,08h
	push	ebx
	push	eax
	call	WinQueryWindowPos
	add	esp,08h
	cmp	eax,01h
	jne	@BLBL119

; 1390 		ProfileWrite(WC_DRVSETUP, "Position", sizeof(swp), &swp);
	push	ebx
	mov	ecx,024h
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE68
	mov	eax,offset FLAT:@CBE61
	call	ProfileWrite
	add	esp,010h
@BLBL119:

; 1392 	break;
	jmp	@BLBL147
@BLBL159:

; 1403 	    Verbose(1, "DrvSetup", "WM_QUERYTRACKINFO (tracking flags %#x)",tf);
	mov	ax,[ebp+010h];	mp1
	movzx	eax,ax
	push	eax
	mov	ecx,offset FLAT:@CBE69
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 1405 	break;				/* has to do default processing! */
	jmp	@BLBL147
@BLBL160:

; 1418 	    Verbose(3, "DrvSetup", "WM_PRESPARAMCHANGED(type %#x)", LONGFROMMP(mp1));
	mov	eax,[ebp+010h];	mp1
	push	eax
	mov	ecx,offset FLAT:@CBE70
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,03h
	call	Verbose
	add	esp,010h

; 1419 	    if( work == NULL )
	cmp	dword ptr [ebp-020h],0h;	work
	jne	@BLBL120

; 1420 		break;
	jmp	@BLBL147
@BLBL120:

; 1422 	    switch( LONGFROMMP(mp1) )
	mov	eax,[ebp+010h];	mp1
	cmp	eax,01h
	je	@BLBL187
	cmp	eax,03h
	je	@BLBL188
	cmp	eax,0fh
	je	@BLBL189
	jmp	@BLBL190
@BLBL187:

; 1425 		cb = WinQueryPresParam(hwnd, PP_FOREGROUNDCOLOR, 0, NULL,
	push	01h
	push	offset FLAT:@3rgbForeground
	push	04h
	push	0h
	push	0h
	push	01h
	push	dword ptr [ebp+08h];	hwnd
	call	WinQueryPresParam
	add	esp,01ch

; 1428 		if( cb == 0 )
	test	eax,eax
	jne	@BLBL121

; 1429 		    Verbose(1, "DrvSetup",
	mov	ebx,[ebp-020h];	work
	push	dword ptr [ebx+04h]
	call	WinGetLastError
	add	esp,04h
	mov	ecx,offset FLAT:@CBE71
	push	eax
	mov	edx,offset FLAT:@CBE3
	sub	esp,0ch
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 1431 			    WinGetLastError(work-
; 1431 >hab));
	jmp	@BLBL122
@BLBL121:

; 1434 		    Verbose(2, "DrvSetup", "Set foreground colour to %#x",
	push	dword ptr  @3rgbForeground
	mov	ecx,offset FLAT:@CBE72
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,02h
	call	Verbose
	add	esp,010h

; 1436 		    ProfileWrite(WC_DRVSETUP, "Foreground",
	mov	ecx,04h
	push	offset FLAT:@3rgbForeground
	mov	edx,offset FLAT:@CBE2
	sub	esp,0ch
	mov	eax,offset FLAT:@CBE61
	call	ProfileWrite
	add	esp,010h

; 1438 		    WinInvalidateRect(hwnd, NULL, FALSE);
	push	0h
	push	0h
	push	dword ptr [ebp+08h];	hwnd
	call	WinInvalidateRect
	add	esp,0ch

; 1439 		}
@BLBL122:

; 1440 		break;
	jmp	@BLBL147
@BLBL188:

; 1443 		cb = WinQueryPresParam(hwnd, PP_BACKGROUNDCOLOR, 0, NULL,
	push	01h
	push	offset FLAT:@2rgbBackground
	push	04h
	push	0h
	push	0h
	push	03h
	push	dword ptr [ebp+08h];	hwnd
	call	WinQueryPresParam
	add	esp,01ch

; 1446 		if( cb == 0 )
	test	eax,eax
	jne	@BLBL123

; 1447 		    Verbose(1, "DrvSetup",
	mov	ebx,[ebp-020h];	work
	push	dword ptr [ebx+04h]
	call	WinGetLastError
	add	esp,04h
	mov	ecx,offset FLAT:@CBE71
	push	eax
	mov	edx,offset FLAT:@CBE3
	sub	esp,0ch
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 1449 			    WinGetLastError(work->hab));
	jmp	@BLBL124
@BLBL123:

; 1452 		    Verbose(2, "DrvSetup", "Set background colour to %#x",
	push	dword ptr  @2rgbBackground
	mov	ecx,offset FLAT:@CBE73
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,02h
	call	Verbose
	add	esp,010h

; 1454 		    ProfileWrite(WC_DRVSETUP, "Background",
	mov	ecx,04h
	push	offset FLAT:@2rgbBackground
	mov	edx,offset FLAT:@CBE1
	sub	esp,0ch
	mov	eax,offset FLAT:@CBE61
	call	ProfileWrite
	add	esp,010h

; 1456 		    WinInvalidateRect(hwnd, NULL, FALSE);
	push	0h
	push	0h
	push	dword ptr [ebp+08h];	hwnd
	call	WinInvalidateRect
	add	esp,0ch

; 1457 		}
@BLBL124:

; 1458 		break;
	jmp	@BLBL147
@BLBL189:

; 1461 		cb = WinQueryPresParam(hwnd, PP_FONTNAMESIZE, 0, NULL,
	push	01h
	lea	ebx,[ebp-02b0h];	fontname
	push	ebx
	push	0100h
	push	0h
	push	0h
	push	0fh
	push	dword ptr [ebp+08h];	hwnd
	call	WinQueryPresParam
	add	esp,01ch

; 1464 		if( cb == 0 )
	test	eax,eax
	jne	@BLBL125

; 1465 		    Verbose(1, "DrvSetup",
	mov	ebx,[ebp-020h];	work
	push	dword ptr [ebx+04h]
	call	WinGetLastError
	add	esp,04h
	mov	ecx,offset FLAT:@CBE71
	push	eax
	mov	edx,offset FLAT:@CBE3
	sub	esp,0ch
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 1467 			    WinGetLastError(work->hab));
	jmp	@BLBL126
@BLBL125:

; 1470 		    Verbose(2, "DrvSetup", "Set font to \"%s\"", fontname);
	lea	ebx,[ebp-02b0h];	fontname
	push	ebx
	mov	ecx,offset FLAT:@CBE74
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,02h
	call	Verbose
	add	esp,0ch

; 1471 		    ProfileWrite(WC_DRVSETUP, "Font", strlen(fontname), &fontname);
	mov	eax,ebx
	call	strlen
	add	esp,04h
	mov	ecx,eax
	push	ebx
	mov	edx,offset FLAT:@CBE75
	sub	esp,0ch
	mov	eax,offset FLAT:@CBE61
	call	ProfileWrite
	add	esp,010h

; 1472 		    WinInvalidateRect(hwnd, NULL, FALSE);
	push	0h
	push	0h
	push	dword ptr [ebp+08h];	hwnd
	call	WinInvalidateRect
	add	esp,0ch

; 1473 		}
@BLBL126:

; 1474 		break;
	jmp	@BLBL147
@BLBL190:

; 1477 		Verbose(1, "DrvSetup", "WM_PRESPARAMCHANGED(unsupported type %#x)",
	mov	eax,[ebp+010h];	mp1
	push	eax
	mov	ecx,offset FLAT:@CBE76
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 1482       break;
	jmp	@BLBL147
@BLBL161:

; 1490 	return (MRESULT)TRUE;
	pop	esi
	mov	eax,01h
	pop	edi
	pop	ebx
	add	esp,0460h
	pop	fs:[0h]
	leave	
	ret	

; 1494 	break;
@BLBL147:

; 1497     return WinDefWindowProc(hwnd, msg, mp1, mp2);
	push	dword ptr [ebp+014h];	mp2
	push	dword ptr [ebp+010h];	mp1
	push	dword ptr [ebp+0ch];	msg
	push	dword ptr [ebp+08h];	hwnd
	call	WinDefWindowProc
	add	esp,010h
	pop	esi
	pop	edi
	pop	ebx
	add	esp,0460h
	pop	fs:[0h]
	leave	
	ret	
DrvsetupWindowProc	endp

; 791 DragDrop(HWND hwndClient,PWORK const work,PCNRDRAGINFO drag)

DragDrop__FUlCP5_WORKP12_CNRDRAGINFO	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,ecx
	push	edi
	mov	[ebp+010h],ebx;	drag
	push	esi
	mov	esi,eax
	sub	esp,0ech
	mov	[ebp+0ch],edx;	work
	mov	[ebp+08h],esi;	hwndClient

; 793     VRDev * const dest = ((PMYRECORD)drag->pRecord)->dsk;
	mov	eax,[ebx+04h]

; 798     Verbose(2, "DrvSetup", "DragDrop, record %#x", drag->pRecord);
	push	eax

; 793     VRDev * const dest = ((PMYRECORD)drag->pRecord)->dsk;
	mov	eax,[eax+01ch]

; 798     Verbose(2, "DrvSetup", "DragDrop, record %#x", drag->pRecord);
	sub	esp,0ch

; 793     VRDev * const dest = ((PMYRECORD)drag->pRecord)->dsk;
	mov	[ebp-01ch],eax;	dest

; 798     Verbose(2, "DrvSetup", "DragDrop, record %#x", drag->pRecord);
	mov	edi,offset FLAT:@CBE3
	mov	ecx,offset FLAT:@CBE26
	mov	edx,edi
	mov	eax,02h
	call	Verbose
	mov	ecx,ebx
	mov	eax,esi

; 800     if( drag->pDragInfo->hwndSource != hwndClient )
	mov	ecx,[ecx]

; 798     Verbose(2, "DrvSetup", "DragDrop, record %#x", drag->pRecord);
	add	esp,010h

; 800     if( drag->pDragInfo->hwndSource != hwndClient )
	cmp	[ecx+08h],eax

; 798     Verbose(2, "DrvSetup", "DragDrop, record %#x", drag->pRecord);
	mov	edx,edi

; 800     if( drag->pDragInfo->hwndSource != hwndClient )
	je	@BLBL66

; 802 	Verbose(2, "DrvSetup", "DragDrop, can't use external data");
	mov	ecx,offset FLAT:@CBE27
	mov	eax,02h
	call	Verbose

; 803 	return 0;
	add	esp,0ech
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL66:

; 806     id = work->pmrDragSource->dsk->queryID();
	mov	ebx,[ebp+0ch];	work

; 812     switch( dest->isArray() )
	mov	eax,[ebp-01ch];	dest

; 806     id = work->pmrDragSource->dsk->queryID();
	mov	ebx,[ebx+020h]
	mov	ebx,[ebx+01ch]
	add	ebx,08h
	mov	[ebp-024h],ebx;	id

; 812     switch( dest->isArray() )
	mov	ebx,[eax]
	mov	ecx,[ebx+064h]
	add	eax,ecx
	call	dword ptr [ebx+060h]
	movzx	eax,al
	cmp	eax,02h
	je	@BLBL192
	cmp	eax,04h
	jne	@BLBL191
@BLBL192:

; 816 	sprintf(str, "This will destroy all data on the"
	mov	ecx,[ebp-024h];	id
	lea	ebx,[ebp-0ech];	str
	movzx	edx,byte ptr [ecx+05h]
	push	edx
	movzx	edx,byte ptr [ecx+04h]
	push	edx
	movzx	edx,byte ptr [ecx+03h]
	push	edx
	movzx	edx,byte ptr [ecx+02h]
	push	edx
	movzx	edx,byte ptr [ecx+01h]
	push	edx
	movzx	ecx,byte ptr [ecx]
	push	ecx
	mov	edx,offset FLAT:@CBE28
	sub	esp,08h
	mov	eax,ebx
	call	_sprintfieee

; 820 	response = WinMessageBox(HWND_DESKTOP, hwndClient,
	push	04014h
	push	0ffffffffh
	push	offset FLAT:@CBE29
	push	ebx
	push	dword ptr [ebp+08h];	hwndClient
	push	01h
	call	WinMessageBox
	add	esp,038h

; 823 	if( response == MBID_YES )
	cmp	eax,06h
	jne	@BLBL67

; 828 	    ((VRDrive *)dest)->addChild(work->pmrDragSource->dsk, False, False);
	push	0h

; 826 	    PMYRECORD		myrcd = (PMYRECORD)drag->pRecord;
	mov	ebx,[ebp+010h];	drag

; 828 	    ((VRDrive *)dest)->addChild(work->pmrDragSource->dsk, False, False);
	sub	esp,0ch
	mov	edi,[ebp-01ch];	dest
	mov	edx,[ebp+0ch];	work

; 826 	    PMYRECORD		myrcd = (PMYRECORD)drag->pRecord;
	mov	ebx,[ebx+04h]
	mov	[ebp-010h],ebx;	myrcd

; 828 	    ((VRDrive *)dest)->addChild(work->pmrDragSource->dsk, False, False);
	mov	ebx,[edi]
	mov	edx,[edx+020h]
	mov	eax,[ebx+08ch]
	mov	edx,[edx+01ch]
	xor	ecx,ecx
	add	eax,edi
	call	dword ptr [ebx+088h]
	add	esp,010h

; 834 	    VRDev *	highest = dest;
	mov	[ebp-020h],edi;	highest

; 835 	    while( highest->getParent() != 0 )
	cmp	dword ptr [edi+04h],0h
	je	@BLBL68
@BLBL69:

; 836 		highest = highest->getParent();
	mov	edi,[edi+04h]

; 835 	    while( highest->getParent() != 0 )
	cmp	dword ptr [edi+04h],0h
	jne	@BLBL69
	mov	[ebp-020h],edi;	highest
@BLBL68:

; 837 	    SearchCnr(work->hwndCnr, (PFNSRCH)LookupDisk, highest, 
; 837 &myrcd, NULL);
	push	0h
	lea	ecx,[ebp-010h];	myrcd
	push	ecx
	mov	edi,[ebp+0ch];	work
	mov	ebx,[ebp-020h];	highest
	sub	esp,0ch
	mov	eax,[edi+08h]
	mov	ecx,ebx
	mov	edx,offset FLAT: LookupDisk__FUlP9_MYRECORDP5VRDevPP9_MYRECORD
	call	SearchCnr__FUlPFUlPvT2PUl_UlPvN23

; 838 	    Verbose(2, "DrvSetup", "DragDrop: found as record %lu", myrcd);
	push	dword ptr [ebp-010h];	myrcd
	mov	ecx,offset FLAT:@CBE30
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,02h
	call	Verbose

; 842 	    WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
	push	030002h

; 840 	    ap[0] = (PMINIRECORDCORE)work->pmrDragSource;
	mov	eax,[edi+020h]
	mov	[ebp-018h],eax;	ap

; 841 	    ap[1] = (PMINIRECORDCORE)myrcd;
	mov	eax,[ebp-010h];	myrcd
	mov	[ebp-014h],eax;	ap

; 842 	    WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
	lea	eax,[ebp-018h];	ap
	push	eax
	push	0346h
	push	dword ptr [edi+08h]
	call	WinSendMsg

; 850 	    WinPostMsg(work->hwndScanner, WUM_DEVTREE, highest, NULL);
	push	0h
	push	ebx
	push	0100ah
	push	dword ptr [edi+0ch]
	call	WinPostMsg
	add	esp,044h

; 851 	}
@BLBL67:

; 857 	break;
@BLBL191:

; 863     work->pmrDragSource = NULL;
	mov	ebx,[ebp+0ch];	work

; 865     return 0;
	xor	eax,eax

; 863     work->pmrDragSource = NULL;
	mov	dword ptr [ebx+020h],0h

; 864     work->pmrDragDestination = NULL;
	mov	dword ptr [ebx+024h],0h

; 865     return 0;
	add	esp,0ech
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
DragDrop__FUlCP5_WORKP12_CNRDRAGINFO	endp

; 659 DragOver(HWND hwndClient,PWORK const work,PCNRDRAGINFO drag)

DragOver__FUlCP5_WORKP12_CNRDRAGINFO	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,ecx
	push	edi
	mov	[ebp+08h],eax;	hwndClient
	push	esi
	mov	[ebp+010h],ebx;	drag
	sub	esp,034h
	mov	[ebp+0ch],edx;	work

; 661     Verbose(2, "DrvSetup", "DragOver, record %#x", drag->pRecord);
	push	dword ptr [ebx+04h]
	mov	edi,offset FLAT:@CBE3
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE16
	mov	edx,edi
	mov	eax,02h
	call	Verbose
	mov	ecx,ebx
	add	esp,010h

; 668     if( drag->pRecord == NULL )
	cmp	dword ptr [ecx+04h],0h

; 661     Verbose(2, "DrvSetup", "DragOver, record %#x", drag->pRecord);
	mov	edx,edi

; 668     if( drag->pRecord == NULL )
	jne	@BLBL58

; 670 	Verbose(2, "DrvSetup", "DragOver, no target");
	mov	ecx,offset FLAT:@CBE17
	mov	eax,02h
	call	Verbose

; 671 	return MRFROM2SHORT(DOR_NODROP,DO_DEFAULT);
	add	esp,034h
	mov	eax,0bffe0000h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL58:

; 678     WinQueryPointerPos(HWND_DESKTOP, &ptlMouse);
	lea	ebx,[ebp-014h];	ptlMouse
	push	ebx
	push	01h
	call	WinQueryPointerPos

; 686     WinMapWindowPoints(HWND_DESKTOP, hwndClient, (PPOINTL)&qrfrQuery.rect, 2);
	push	02h

; 682     qrfrQuery.rect.xLeft = ptlMouse.x;
	mov	ebx,[ebp-014h];	ptlMouse
	mov	[ebp-030h],ebx;	qrfrQuery

; 683     qrfrQuery.rect.xRight = ptlMouse.x+1;
	inc	ebx
	mov	[ebp-028h],ebx;	qrfrQuery

; 684     qrfrQuery.rect.yBottom = ptlMouse.y;
	mov	ebx,[ebp-010h];	ptlMouse
	mov	[ebp-02ch],ebx;	qrfrQuery

; 685     qrfrQuery.rect.yTop = ptlMouse.y+1;
	inc	ebx
	mov	[ebp-024h],ebx;	qrfrQuery

; 686     WinMapWindowPoints(HWND_DESKTOP, hwndClient, (PPOINTL)&qrfrQuery.rect, 2);
	lea	ebx,[ebp-030h];	qrfrQuery
	push	ebx

; 681     qrfrQuery.cb = sizeof(QUERYRECFROMRECT);
	mov	dword ptr [ebp-034h],018h;	qrfrQuery

; 686     WinMapWindowPoints(HWND_DESKTOP, hwndClient, (PPOINTL)&qrfrQuery.rect, 2);
	push	dword ptr [ebp+08h];	hwndClient

; 693     work->pmrDragDestination
	lea	ebx,[ebp-034h];	qrfrQuery

; 686     WinMapWindowPoints(HWND_DESKTOP, hwndClient, (PPOINTL)&qrfrQuery.rect, 2);
	push	01h
	call	WinMapWindowPoints

; 693     work->pmrDragDestination
	push	ebx
	mov	ebx,[ebp+0ch];	work
	push	010h

; 688     qrfrQuery.fsSearch = CMA_PARTIAL|CMA_ITEMORDER;
	mov	dword ptr [ebp-020h],02001h;	qrfrQuery

; 693     work->pmrDragDestination
	push	0342h
	push	dword ptr [ebx+08h]
	call	WinSendMsg
	add	esp,028h
	mov	[ebx+024h],eax

; 696     if( work->pmrDragDestination == NULL )
	test	eax,eax
	jne	@BLBL59

; 698 	Verbose(2, "DrvSetup", "DragOver, no destination");
	mov	ecx,offset FLAT:@CBE18
	mov	edx,offset FLAT:@CBE3
	mov	eax,02h
	call	Verbose

; 699 	return MRFROM2SHORT(DOR_NODROP,DO_DEFAULT);
	add	esp,034h
	mov	eax,0bffe0000h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL59:

; 702     if( work->pmrDragDestination != (PMYRECORD)drag->pRecord )
	mov	ecx,[ebp+010h];	drag
	mov	ebx,[ebp+0ch];	work
	mov	ecx,[ecx+04h]
	cmp	[ebx+024h],ecx
	je	@BLBL60

; 704 	Verbose(1, "DrvSetup", "DragOver, pRecord != pmrDragDestination!");
	mov	ecx,offset FLAT:@CBE19
	mov	edx,offset FLAT:@CBE3
	mov	eax,01h
	call	Verbose

; 705 	return MRFROM2SHORT(DOR_NODROP,DO_DEFAULT);
	add	esp,034h
	mov	eax,0bffe0000h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL60:

; 712     VRDev * const rdev = work->pmrDragDestination->dsk;
	mov	ebx,[ebp+0ch];	work

; 715     Verbose(2, "DrvSetup", "DragOver, destination %#x", rdev);
	mov	esi,offset FLAT:@CBE3

; 712     VRDev * const rdev = work->pmrDragDestination->dsk;
	mov	edi,[ebx+024h]
	mov	edi,[edi+01ch]
	mov	[ebp-018h],edi;	rdev

; 713     UCHAR const	rdtype = rdev->isArray();
	mov	ecx,[edi]
	mov	eax,[ecx+064h]
	add	eax,edi
	call	dword ptr [ecx+060h]

; 715     Verbose(2, "DrvSetup", "DragOver, destination %#x", rdev);
	push	edi
	mov	ecx,offset FLAT:@CBE20
	sub	esp,0ch

; 713     UCHAR const	rdtype = rdev->isArray();
	mov	[ebp-019h],al;	rdtype

; 715     Verbose(2, "DrvSetup", "DragOver, destination %#x", rdev);
	mov	edx,esi
	mov	eax,02h
	call	Verbose

; 716     if( work->pmrDragSource->dsk == rdev )
	mov	eax,[ebx+020h]

; 715     Verbose(2, "DrvSetup", "DragOver, destination %#x", rdev);
	add	esp,010h

; 716     if( work->pmrDragSource->dsk == rdev )
	cmp	[eax+01ch],edi

; 715     Verbose(2, "DrvSetup", "DragOver, destination %#x", rdev);
	mov	edx,esi

; 716     if( work->pmrDragSource->dsk == rdev )
	jne	@BLBL61

; 718 	Verbose(2, "DrvSetup", "DragOver, source == destination!");
	mov	ecx,offset FLAT:@CBE21
	mov	eax,02h
	call	Verbose

; 719 	return MRFROM2SHORT(DOR_NODROP,DO_DEFAULT);
	add	esp,034h
	mov	eax,0bffe0000h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL61:

; 723     switch( rdtype )
	movzx	eax,byte ptr [ebp-019h];	rdtype
	cmp	eax,02h
	je	@BLBL195
	cmp	eax,04h
	jne	@BLBL196
@BLBL194:

; 729 	if( work->pmrDragSource->dsk->querySize() < rdev->querySize() )
	mov	ebx,[ebp+0ch];	work
	mov	eax,[ebx+020h]
	mov	eax,[eax+01ch]
	mov	ecx,[eax]
	mov	edx,[ecx+014h]
	add	eax,edx
	call	dword ptr [ecx+010h]
	mov	edi,eax
	mov	eax,[ebp-018h];	rdev
	mov	ecx,[eax]
	mov	edx,[ecx+014h]
	add	eax,edx
	call	dword ptr [ecx+010h]
	cmp	edi,eax
	jae	@BLBL62

; 731 	    Verbose(2, "DrvSetup", "DragOver, source too small");
	mov	ecx,offset FLAT:@CBE22
	mov	edx,offset FLAT:@CBE3
	mov	eax,02h
	call	Verbose

; 732 	    work->pmrDragDestination = NULL;
	mov	dword ptr [ebx+024h],0h

; 733 	    return MRFROM2SHORT(DOR_NODROP,DO_DEFAULT);
	add	esp,034h
	mov	eax,0bffe0000h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL62:

; 735 	break;
	jmp	@BLBL193
@BLBL195:

; 741 	if( rdev->getParent() != NULL )
	mov	ebx,[ebp-018h];	rdev
	cmp	dword ptr [ebx+04h],0h
	je	@BLBL63

; 747 	    work->pmrDragDestination = NULL;
	mov	ebx,[ebp+0ch];	work

; 746 	    Verbose(2, "DrvSetup", "DragOver, bad destination");
	mov	ecx,offset FLAT:@CBE23
	mov	edx,offset FLAT:@CBE3
	mov	eax,02h
	call	Verbose

; 747 	    work->pmrDragDestination = NULL;
	mov	dword ptr [ebx+024h],0h

; 748 	    return MRFROM2SHORT(DOR_NODROP,DO_DEFAULT);
	add	esp,034h
	mov	eax,0bffe0000h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL63:

; 750 	break;
	jmp	@BLBL193
@BLBL196:

; 757 	work->pmrDragDestination = NULL;
	mov	ebx,[ebp+0ch];	work

; 756 	Verbose(2, "DrvSetup", "DragOver, bad destination type");
	mov	ecx,offset FLAT:@CBE24
	mov	edx,offset FLAT:@CBE3
	mov	eax,02h
	call	Verbose

; 757 	work->pmrDragDestination = NULL;
	mov	dword ptr [ebx+024h],0h

; 758 	return MRFROM2SHORT(DOR_NODROP,DO_DEFAULT);
	add	esp,034h
	mov	eax,0bffe0000h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL193:

; 761     Verbose(3, "DrvSetup", "DragOver, destination is acceptable");
	mov	ecx,offset FLAT:@CBE25
	mov	edx,offset FLAT:@CBE3
	mov	eax,03h
	call	Verbose

; 762     return MRFROM2SHORT(DOR_DROP,DO_MOVE);
	add	esp,034h
	mov	eax,0200001h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
DragOver__FUlCP5_WORKP12_CNRDRAGINFO	endp

; 623 DragLeave(HWND hwndClient,PWORK const work,PCNRDRAGINFO drag)

DragLeave__FUlCP5_WORKP12_CNRDRAGINFO	proc
	push	ebx
	mov	ebx,edx

; 625     Verbose(2, "DrvSetup", "DragLeave, record %#x", drag->pRecord);
	push	dword ptr [ecx+04h]
	mov	ecx,offset FLAT:@CBE15
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,02h
	call	Verbose
	add	esp,010h
	mov	edx,ebx

; 629     return 0;
	pop	ebx

; 627     work->pmrDragSource = NULL;			/* reflect in work area */
	mov	dword ptr [edx+020h],0h

; 628     work->pmrDragDestination = NULL;
	mov	dword ptr [edx+024h],0h

; 629     return 0;
	xor	eax,eax
	ret	
DragLeave__FUlCP5_WORKP12_CNRDRAGINFO	endp

; 542 DragInit(HWND hwndClient,PWORK const work,PCNRDRAGINIT const drag)

DragInit__FUlCP5_WORKCP12_CNRDRAGINIT	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,ecx
	push	edi
	mov	[ebp+0ch],edx;	work
	push	esi
	mov	[ebp+08h],eax;	hwndClient
	sub	esp,04ch
	mov	[ebp+010h],ebx;	drag

; 544     Verbose(2, "DrvSetup", "DragInit: record %#x",drag->pRecord);
	push	dword ptr [ebx+04h]
	mov	ecx,offset FLAT:@CBE11
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,02h
	call	Verbose
	mov	ecx,ebx
	add	esp,010h

; 546     if( drag->pRecord == NULL )
	cmp	dword ptr [ecx+04h],0h
	jne	@BLBL49

; 547 	return 0;				/* no record selected */
	xor	eax,eax
	add	esp,04ch
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL49:

; 548     if( work->pmrDragSource != NULL )
	mov	ecx,[ebp+0ch];	work
	cmp	dword ptr [ecx+020h],0h
	je	@BLBL50

; 549 	return 0;				/* already active drag */
	xor	eax,eax
	add	esp,04ch
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL50:

; 555     VRDev * const rdev = ((PMYRECORD)drag->pRecord)->dsk;
	mov	ecx,[ebp+010h];	drag
	mov	ecx,[ecx+04h]
	mov	ecx,[ecx+01ch]
	mov	[ebp-010h],ecx;	rdev

; 556     if( rdev->getParent() != NULL )
	cmp	dword ptr [ecx+04h],0h
	je	@BLBL51

; 557 	return 0;				/* sorry, no dragging of children */
	xor	eax,eax
	add	esp,04ch
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL51:

; 558     if( rdev->isWritable() == False )
	mov	eax,[ebp-010h];	rdev
	mov	ecx,[eax]
	mov	edx,[ecx+0ch]
	add	eax,edx
	call	dword ptr [ecx+08h]
	test	al,al
	jne	@BLBL52

; 559 	return 0;				/* can't write = no drag */
	xor	eax,eax
	add	esp,04ch
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL52:

; 560     if( rdev->isHostdrive() == True  &&  rdev->isArray() == 0 )
	mov	ebx,[ebp-010h];	rdev
	mov	ecx,[ebx]
	mov	eax,[ecx+06ch]
	add	eax,ebx
	call	dword ptr [ecx+068h]
	cmp	al,01h
	mov	eax,ebx
	jne	@BLBL53
	mov	ecx,[eax]
	mov	edx,[ecx+064h]
	add	eax,edx
	call	dword ptr [ecx+060h]
	test	al,al
	jne	@BLBL53

; 561 	return 0;				/* not even PHYSDEVICE! */
	xor	eax,eax
	add	esp,04ch
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL53:

; 568     work->pmrDragSource 
; 568 = (PMYRECORD)drag->pRecord;
	mov	ebx,[ebp+010h];	drag

; 567     Verbose(2, "DrvSetup", "DragInit: record passed checks");
	mov	ecx,offset FLAT:@CBE12
	mov	edx,offset FLAT:@CBE3
	mov	eax,02h
	call	Verbose

; 568     work->pmrDragSource 
; 568 = (PMYRECORD)drag->pRecord;
	mov	edi,[ebp+0ch];	work

; 570     PDRAGINFO	dinfo = DrgAllocDraginfo(1);
	push	01h

; 568     work->pmrDragSource 
; 568 = (PMYRECORD)drag->pRecord;
	mov	ecx,[ebx+04h]
	mov	[edi+020h],ecx

; 570     PDRAGINFO	dinfo = DrgAllocDraginfo(1);
	call	DrgAllocDraginfo

; 574     ditem.hstrType = DrgAddStrHandle("DRT_CUSTOMER");
	push	offset FLAT:@CBE13

; 572     ditem.hwndItem = hwndClient;
	mov	esi,[ebp+08h];	hwndClient
	mov	[ebp-034h],esi;	ditem

; 573     ditem.ulItemID = (ULONG)drag->pRecord;
	mov	ecx,[ebx+04h]
	mov	ebx,eax
	mov	[ebp-030h],ecx;	ditem

; 574     ditem.hstrType = DrgAddStrHandle("DRT_CUSTOMER");
	call	DrgAddStrHandle

; 575     ditem.hstrRMF = DrgAddStrHandle("DRM_SHAREMEM,DRM_UNKNOWN");
	push	offset FLAT:@CBE14

; 574     ditem.hstrType = DrgAddStrHandle("DRT_CUSTOMER");
	mov	[ebp-02ch],eax;	ditem

; 575     ditem.hstrRMF = DrgAddStrHandle("DRM_SHAREMEM,DRM_UNKNOWN");
	call	DrgAddStrHandle

; 579     DrgSetDragitem(dinfo,			/* Set item in DRAGINFO */
	push	0h
	lea	ecx,[ebp-034h];	ditem
	push	024h

; 575     ditem.hstrRMF = DrgAddStrHandle("DRM_SHAREMEM,DRM_UNKNOWN");
	mov	[ebp-028h],eax;	ditem

; 579     DrgSetDragitem(dinfo,			/* Set item in DRAGINFO */
	push	ecx

; 576     ditem.fsControl = DC_REMOVEABLEMEDIA;	/* can't be recovered... */
	mov	word ptr [ebp-014h],020h;	ditem

; 579     DrgSetDragitem(dinfo,			/* Set item in DRAGINFO */
	push	ebx

; 577     ditem.fsSupportedOps = DO_MOVEABLE;
	mov	word ptr [ebp-012h],02h;	ditem

; 579     DrgSetDragitem(dinfo,			/* Set item in DRAGINFO */
	call	DrgSetDragitem

; 585     dimage.cb = sizeof(DRAGIMAGE);		/* Initialize DRAGIMAGE */
	mov	word ptr [ebp-04ch],018h;	dimage

; 586     dimage.cptl = 0;				/* Not a polygon */
	mov	word ptr [ebp-04ah],0h;	dimage

; 587     dimage.hImage = work->pmrDragSource->dsk->queryIcon();
	mov	eax,[edi+020h]
	mov	eax,[eax+01ch]
	mov	ecx,[eax]
	mov	edx,[ecx+07ch]
	add	eax,edx
	call	dword ptr [ecx+078h]

; 592     /*hDrop =*/ DrgDrag(hwndClient,		/* initiate drag */
	push	0h
	lea	ecx,[ebp-04ch];	dimage
	push	038h

; 587     dimage.hImage = work->pmrDragSource->dsk->queryIcon();
	mov	[ebp-048h],eax;	dimage

; 592     /*hDrop =*/ DrgDrag(hwndClient,		/* initiate drag */
	push	01h

; 588     dimage.fl = DRG_ICON;			/* Dragging an icon */
	mov	dword ptr [ebp-03ch],01h;	dimage

; 592     /*hDrop =*/ DrgDrag(hwndClient,		/* initiate drag */
	push	ecx

; 589     dimage.cxOffset = 0;			/* No hotspot */
	mov	word ptr [ebp-038h],0h;	dimage

; 592     /*hDrop =*/ DrgDrag(hwndClient,		/* initiate drag */
	push	ebx

; 590     dimage.cyOffset = 0;
	mov	word ptr [ebp-036h],0h;	dimage

; 592     /*hDrop =*/ DrgDrag(hwndClient,		/* initiate drag */
	push	esi
	call	DrgDrag

; 599     DrgFreeDraginfo(dinfo);			/* Free DRAGINFO struct */
	push	ebx
	call	DrgFreeDraginfo
	add	esp,038h

; 601     return 0;
	add	esp,04ch
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
DragInit__FUlCP5_WORKCP12_CNRDRAGINIT	endp

; 478 DisplayDisk(PWORK work,PMYRECORD parent,VRDev * dsk)

DisplayDisk__FP5_WORKP9_MYRECORDP5VRDev	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,ecx
	push	edi
	mov	edi,eax
	push	esi
	mov	[ebp+010h],ebx;	dsk
	sub	esp,030h
	mov	[ebp-030h],edx;	@CBE90

; 486     address = (PMYRECORD)WinSendMsg(work->hwndCnr, CM_ALLOCRECORD,
	push	01h

; 478 DisplayDisk(PWORK work,PMYRECORD parent,VRDev * dsk)
	mov	[ebp+08h],edi;	work

; 486     address = (PMYRECORD)WinSendMsg(work->hwndCnr, CM_ALLOCRECORD,
	push	04h
	push	0331h
	push	dword ptr [edi+08h]
	call	WinSendMsg
	mov	esi,eax

; 488     Verbose(3, "DrvSetup", "CM_ALLOCRECORD: %#x",address);
	push	esi

; 486     address = (PMYRECORD)WinSendMsg(work->hwndCnr, CM_ALLOCRECORD,
	mov	[ebp-010h],esi;	address

; 488     Verbose(3, "DrvSetup", "CM_ALLOCRECORD: %#x",address);
	sub	esp,0ch
	mov	eax,offset FLAT:@CBE3
	mov	edx,eax
	mov	ecx,offset FLAT:@CBE9
	mov	eax,03h
	call	Verbose

; 490     address->dsk = dsk;
	mov	[esi+01ch],ebx

; 491     descr = dsk->allocateDescription();
	mov	ecx,[ebx]
	mov	eax,[ecx+074h]
	add	eax,ebx
	call	dword ptr [ecx+070h]
	mov	edx,ebx
	mov	ebx,eax

; 492     address->record.hptrIcon = dsk->queryIcon();
	mov	ecx,[edx]
	mov	eax,[ecx+07ch]
	add	edx,eax
	mov	eax,edx
	call	dword ptr [ecx+078h]
	mov	ecx,ebx

; 493     address->record.pszIcon = descr;
	mov	[esi+014h],ecx

; 492     address->record.hptrIcon = dsk->queryIcon();
	mov	[esi+018h],eax

; 495     memset(&ins, 0, sizeof(ins));
	lea	ebx,[ebp-02ch];	ins
	mov	eax,ebx
	mov	ecx,018h
	xor	edx,edx
	call	memset
	lea	ecx,[ebp-02ch];	ins

; 505     i = (int)WinSendMsg(work->hwndCnr, CM_INSERTRECORD,
	push	ecx

; 495     memset(&ins, 0, sizeof(ins));
	mov	edx,[ebp-030h];	@CBE90

; 505     i = (int)WinSendMsg(work->hwndCnr, CM_INSERTRECORD,
	push	esi

; 496     ins.cb = sizeof(ins);
	mov	dword ptr [ebp-02ch],018h;	ins

; 505     i = (int)WinSendMsg(work->hwndCnr, CM_INSERTRECORD,
	push	0339h

; 497     ins.pRecordOrder = (PRECORDCORE)CMA_END;
	mov	dword ptr [ebp-028h],040h;	ins

; 498     ins.pRecordParent = (PRECORDCORE)parent;	/* none (only RAID drives have) */
	mov	[ebp-024h],edx;	ins

; 495     memset(&ins, 0, sizeof(ins));
	mov	ebx,offset FLAT:@CBE3

; 499     ins.zOrder = CMA_TOP;
	mov	dword ptr [ebp-01ch],01h;	ins

; 500     ins.fInvalidateRecord = TRUE;		/* invalidate record now/later */
	mov	dword ptr [ebp-020h],01h;	ins

; 501     ins.cRecordsInsert = 1;
	mov	dword ptr [ebp-018h],01h;	ins

; 505     i = (int)WinSendMsg(work->hwndCnr, CM_INSERTRECORD,
	push	dword ptr [edi+08h]
	call	WinSendMsg
	add	esp,030h

; 507     if( i == 0 )
	test	eax,eax
	mov	eax,edi
	jne	@BLBL39

; 508 	Verbose(1, "DrvSetup", "WinSendMsg(CM_INSERTRECORD) failed - last error %#x",
	push	dword ptr [eax+04h]
	call	WinGetLastError
	push	eax
	mov	edx,ebx
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE10
	mov	eax,01h
	call	Verbose

; 509 		WinGetLastError(work->hab));
	add	esp,044h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL39:

; 510     else if( dsk != NULL  &&  dsk->isArray() )
	mov	esi,[ebp+010h];	dsk
	test	esi,esi
	je	@BLBL40
	mov	ecx,[esi]
	mov	eax,[ecx+064h]
	add	eax,esi
	call	dword ptr [ecx+060h]
	test	al,al
	mov	eax,esi
	je	@BLBL40

; 512 	VRDrive *	drv = (VRDrive *)dsk;	/* xxx  hack? */
	mov	[ebp-014h],eax;	drv

; 514 	for( i = 0; i < drv->queryChildren(); ++i )
	mov	ecx,[eax]
	mov	edx,[ecx+094h]
	add	eax,edx
	call	dword ptr [ecx+090h]
	test	eax,eax
	jle	@BLBL40
	mov	ebx,[ebp-010h];	address
	mov	esi,[ebp+08h];	work
	xor	edi,edi
@BLBL43:

; 515 	    DisplayDisk(work, address, drv->getChild(i));
	mov	ebx,[ebp-014h];	drv
	mov	edx,edi
	mov	ecx,[ebx]

; 514 	for( i = 0; i < drv->queryChildren(); ++i )
	inc	edi

; 515 	    DisplayDisk(work, address, drv->getChild(i));
	mov	eax,[ecx+09ch]
	add	eax,ebx
	mov	ebx,[ebp-010h];	address
	call	dword ptr [ecx+098h]
	mov	ecx,eax
	mov	edx,ebx
	mov	eax,esi
	call	DisplayDisk__FP5_WORKP9_MYRECORDP5VRDev
	mov	eax,[ebp-014h];	drv

; 514 	for( i = 0; i < drv->queryChildren(); ++i )
	mov	ecx,[eax]
	mov	edx,[ecx+094h]
	add	eax,edx
	call	dword ptr [ecx+090h]
	cmp	eax,edi
	jg	@BLBL43
@BLBL40:

; 517     return;
	add	esp,030h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
DisplayDisk__FP5_WORKP9_MYRECORDP5VRDev	endp

; 309 DoPopup(HWND hwndClient,PWORK const work)

DoPopup__FUlCP5_WORK	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,edx
	push	edi
	mov	edi,eax
	push	esi
	mov	[ebp+0ch],ebx;	work
	sub	esp,044h

; 315     WinQueryPointerPos(HWND_DESKTOP, &ptlMouse);
	lea	ecx,[ebp-018h];	ptlMouse
	push	ecx

; 309 DoPopup(HWND hwndClient,PWORK const work)
	mov	[ebp+08h],edi;	hwndClient

; 315     WinQueryPointerPos(HWND_DESKTOP, &ptlMouse);
	push	01h

; 311     HWND		popup = NULLHANDLE;
	mov	dword ptr [ebp-010h],0h;	popup

; 315     WinQueryPointerPos(HWND_DESKTOP, &ptlMouse);
	call	WinQueryPointerPos

; 323     WinMapWindowPoints(HWND_DESKTOP, hwndClient, (PPOINTL)&qrfrQuery.rect, 2);
	push	02h

; 319     qrfrQuery.rect.xLeft = ptlMouse.x;
	mov	ecx,[ebp-018h];	ptlMouse
	mov	[ebp-03ch],ecx;	qrfrQuery

; 320     qrfrQuery.rect.xRight = ptlMouse.x+1;
	inc	ecx
	mov	[ebp-034h],ecx;	qrfrQuery

; 321     qrfrQuery.rect.yBottom = ptlMouse.y;
	mov	ecx,[ebp-014h];	ptlMouse
	mov	[ebp-038h],ecx;	qrfrQuery

; 322     qrfrQuery.rect.yTop = ptlMouse.y+1;
	inc	ecx
	mov	[ebp-030h],ecx;	qrfrQuery

; 323     WinMapWindowPoints(HWND_DESKTOP, hwndClient, (PPOINTL)&qrfrQuery.rect, 2);
	lea	ecx,[ebp-03ch];	qrfrQuery
	push	ecx

; 315     WinQueryPointerPos(HWND_DESKTOP, &ptlMouse);
	mov	eax,edi

; 323     WinMapWindowPoints(HWND_DESKTOP, hwndClient, (PPOINTL)&qrfrQuery.rect, 2);
	push	eax

; 318     qrfrQuery.cb = sizeof(QUERYRECFROMRECT);
	mov	dword ptr [ebp-040h],018h;	qrfrQuery

; 323     WinMapWindowPoints(HWND_DESKTOP, hwndClient, (PPOINTL)&qrfrQuery.rect, 2);
	push	01h
	call	WinMapWindowPoints

; 325     qrfrQuery.fsSearch = CMA_PARTIAL|CMA_ITEMORDER;
	mov	dword ptr [ebp-02ch],02001h;	qrfrQuery

; 327     work->pmrMenu
	lea	ecx,[ebp-040h];	qrfrQuery
	push	ecx
	push	010h
	push	0342h
	push	dword ptr [ebx+08h]
	call	WinSendMsg
	add	esp,028h
	mov	[ebx+01ch],eax

; 331     if( work->pmrMenu != NULL )
	test	eax,eax
	je	@BLBL12

; 333 	if( (work->pmrMenu->record.flRecordAttr & CRA_INUSE) != 0 )
	test	byte ptr [eax+04h],08h
	je	@BLBL13

; 338 	    SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse2Source, &bSet, &cnt, NULL);
	push	0h
	lea	ecx,[ebp-01ch];	cnt
	push	ecx

; 335 	    ULONG	cnt = 0;
	mov	dword ptr [ebp-01ch],0h;	cnt

; 337 	    bSet = TRUE;
	mov	dword ptr [ebp-020h],01h;	bSet

; 338 	    SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse2Source, &bSet, &cnt, NULL);
	sub	esp,0ch
	mov	eax,[ebx+08h]
	lea	ecx,[ebp-020h];	bSet
	mov	edx,offset FLAT: EmphInuse2Source__FUlP9_MYRECORDPUlT3
	call	SearchCnr__FUlPFUlPvT2PUl_UlPvN23

; 339 	    Verbose(2, "DrvSetup", "DoPopup: %lu records in use", cnt);
	push	dword ptr [ebp-01ch];	cnt
	mov	ecx,offset FLAT:@CBE8
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,02h
	call	Verbose
	mov	edx,ebx
	add	esp,024h

; 345 		popup = work->hwndMulPopup;
	mov	eax,[edx+018h]
	mov	[ebp-010h],eax;	popup

; 346 	}
	jmp	@BLBL15
@BLBL13:

; 349 	    popup = work->hwndSngPopup;
	mov	ebx,[ebp+0ch];	work

; 350 	    WinSendMsg(work->hwndCnr, CM_SETRECORDEMPHASIS,
	push	040000001h

; 349 	    popup = work->hwndSngPopup;
	mov	ecx,[ebx+014h]
	mov	[ebp-010h],ecx;	popup

; 350 	    WinSendMsg(work->hwndCnr, CM_SETRECORDEMPHASIS,
	push	dword ptr [ebx+01ch]
	push	034ah
	push	dword ptr [ebx+08h]
	call	WinSendMsg
	add	esp,010h

; 353     }
	jmp	@BLBL15
@BLBL12:

; 357 	WinSendMsg(work->hwndCnr, CM_SETRECORDEMPHASIS,
	push	040000001h

; 356 	popup = work->hwndNonePopup;
	mov	ebx,[ebp+0ch];	work

; 357 	WinSendMsg(work->hwndCnr, CM_SETRECORDEMPHASIS,
	push	0h

; 356 	popup = work->hwndNonePopup;
	mov	ecx,[ebx+010h]

; 357 	WinSendMsg(work->hwndCnr, CM_SETRECORDEMPHASIS,
	push	034ah

; 356 	popup = work->hwndNonePopup;
	mov	[ebp-010h],ecx;	popup

; 357 	WinSendMsg(work->hwndCnr, CM_SETRECORDEMPHASIS,
	push	dword ptr [ebx+08h]
	call	WinSendMsg
	add	esp,010h

; 359     }
@BLBL15:

; 361     WinPopupMenu(HWND_DESKTOP, hwndClient, popup,
	push	02c6h
	mov	ebx,[ebp-010h];	popup
	push	0h
	push	dword ptr [ebp-014h];	ptlMouse
	push	dword ptr [ebp-018h];	ptlMouse
	push	ebx
	push	dword ptr [ebp+08h];	hwndClient
	push	01h
	call	WinPopupMenu

; 367     if( popup == work->hwndSngPopup )
	mov	ecx,[ebp+0ch];	work

; 361     WinPopupMenu(HWND_DESKTOP, hwndClient, popup,
	add	esp,01ch

; 367     if( popup == work->hwndSngPopup )
	cmp	[ecx+014h],ebx
	jne	@BLBL16

; 371 	if( work->pmrMenu->dsk == NULL )
	mov	ecx,[ecx+01ch]
	cmp	dword ptr [ecx+01ch],0h
	jne	@BLBL17

; 375 	    WinEnableMenuItem(popup, IDM_CREATE_PDEV, FALSE);
	push	040004000h
	push	0101b8h
	push	0192h
	push	ebx
	call	WinSendMsg

; 376 	    WinEnableMenuItem(popup, IDM_DELETE_PDEV, FALSE);
	push	040004000h
	push	0101b9h
	push	0192h
	push	ebx
	call	WinSendMsg

; 377 	    WinEnableMenuItem(popup, IDM_DESTROY_DRV, FALSE);
	push	040004000h
	push	0101bah
	push	0192h
	push	ebx
	call	WinSendMsg

; 378 	    WinEnableMenuItem(popup, IDM_REMOVE_CHILD, FALSE);
	push	040004000h
	push	0101bbh
	push	0192h
	push	ebx
	call	WinSendMsg
	add	esp,040h

; 379 	}
	jmp	@BLBL16
@BLBL17:

; 380 	else if( work->pmrMenu->dsk->getParent() != NULL )
	mov	ebx,[ebp+0ch];	work
	mov	eax,[ebx+01ch]
	mov	eax,[eax+01ch]
	mov	esi,[eax+04h]
	test	esi,esi
	je	@BLBL19

; 382 	    VRDrive * const	parent = work->pmrMenu->dsk->getParent();
	mov	[ebp-028h],esi;	parent

; 392 	    if( work->pmrMenu->dsk->getParent()->isArray() == RDTYPE_MIRROR )
	mov	ecx,[esi]
	mov	eax,[ecx+064h]
	add	eax,esi
	call	dword ptr [ecx+060h]
	cmp	al,04h
	mov	eax,esi
	jne	@BLBL20

; 394 		int const	cnt = parent->queryChildren();
	mov	ecx,[eax]
	mov	edx,[ecx+094h]
	add	eax,edx
	call	dword ptr [ecx+090h]

; 397 		for( i = v = 0; i < cnt; ++i )
	mov	dword ptr [ebp-024h],0h;	v
	test	eax,eax
	jle	@BLBL21
	mov	ecx,ebx
	mov	[ebp-048h],eax;	cnt
	mov	ebx,eax
	xor	esi,esi
	xor	edi,edi
@BLBL22:

; 399 		    VRDev * const	vrd = parent->getChild(i);
	mov	ebx,[ebp-028h];	parent
	mov	edx,edi
	mov	ecx,[ebx]
	mov	eax,[ecx+09ch]
	add	eax,ebx
	call	dword ptr [ecx+098h]
	mov	ecx,[ebp+0ch];	work
	xchg	ebx,eax

; 400 		    if( vrd != work->pmrMenu->dsk
	mov	edx,[ecx+01ch]
	cmp	[edx+01ch],ebx
	mov	ebx,[ebp-048h];	cnt
	je	@BLBL24
	mov	[ebp-044h],esi;	@CBE92

; 402 			++v;
	mov	ecx,[eax]
	mov	edx,edi
	mov	esi,[ecx+0a4h]
	add	eax,esi
	mov	esi,[ebp+0ch];	work
	call	dword ptr [ecx+0a0h]
	cmp	al,01h
	sete	al
	movzx	eax,al
	mov	ecx,esi
	mov	edx,[ebp-044h];	@CBE92
	mov	esi,eax
	add	esi,edx

; 403 		}
@BLBL24:

; 397 		for( i = v = 0; i < cnt; ++i )
	inc	edi
	cmp	edi,ebx
	jl	@BLBL22
	mov	[ebp-024h],esi;	v
@BLBL21:

; 404 		if( v != 0 )
	cmp	dword ptr [ebp-024h],0h;	v
	je	@BLBL27

; 405 		    WinEnableMenuItem(popup, IDM_REMOVE_CHILD, TRUE);
	push	04000h
	push	0101bbh
	push	0192h
	push	dword ptr [ebp-010h];	popup
	call	WinSendMsg
	add	esp,010h
	jmp	@BLBL29
@BLBL27:

; 407 		    WinEnableMenuItem(popup, IDM_REMOVE_CHILD, FALSE);
	push	040004000h
	push	0101bbh
	push	0192h
	push	dword ptr [ebp-010h];	popup
	call	WinSendMsg
	add	esp,010h

; 408 	    }
	jmp	@BLBL29
@BLBL20:

; 410 		WinEnableMenuItem(popup, IDM_REMOVE_CHILD, FALSE);
	push	040004000h
	push	0101bbh
	push	0192h
	push	dword ptr [ebp-010h];	popup
	call	WinSendMsg
	add	esp,010h
@BLBL29:

; 411 	    WinEnableMenuItem(popup, IDM_CREATE_PDEV, FALSE);
	push	040004000h
	mov	ebx,[ebp-010h];	popup
	push	0101b8h
	push	0192h
	push	ebx
	call	WinSendMsg

; 412 	    WinEnableMenuItem(popup, IDM_DELETE_PDEV, FALSE);
	push	040004000h
	push	0101b9h
	push	0192h
	push	ebx
	call	WinSendMsg

; 413 	    WinEnableMenuItem(popup, IDM_DESTROY_DRV, FALSE);
	push	040004000h
	push	0101bah
	push	0192h
	push	ebx
	call	WinSendMsg
	add	esp,030h

; 414 	}
	jmp	@BLBL16
@BLBL19:

; 415 	else if( work->pmrMenu->dsk->isWritable() == False )
	mov	ebx,[eax]
	mov	ecx,[ebx+0ch]
	add	eax,ecx
	call	dword ptr [ebx+08h]
	test	al,al
	jne	@BLBL31

; 420 	    WinEnableMenuItem(popup, IDM_CREATE_PDEV, FALSE);
	push	040004000h
	mov	ebx,[ebp-010h];	popup
	push	0101b8h
	push	0192h
	push	ebx
	call	WinSendMsg

; 421 	    WinEnableMenuItem(popup, IDM_DELETE_PDEV, FALSE);
	push	040004000h
	push	0101b9h
	push	0192h
	push	ebx
	call	WinSendMsg

; 422 	    WinEnableMenuItem(popup, IDM_DESTROY_DRV, FALSE);
	push	040004000h
	push	0101bah
	push	0192h
	push	ebx
	call	WinSendMsg

; 423 	    WinEnableMenuItem(popup, IDM_REMOVE_CHILD, FALSE);
	push	040004000h
	push	0101bbh
	push	0192h
	push	ebx
	call	WinSendMsg
	add	esp,040h

; 424 	}
	jmp	@BLBL16
@BLBL31:

; 425 	else if(  work->pmrMenu->dsk->isArray() != 0 )
	mov	eax,[ebp+0ch];	work
	mov	eax,[eax+01ch]
	mov	eax,[eax+01ch]
	mov	ebx,[eax]
	mov	ecx,[ebx+064h]
	add	eax,ecx
	call	dword ptr [ebx+060h]
	test	al,al
	je	@BLBL33

; 429 	    WinEnableMenuItem(popup, IDM_CREATE_PDEV, FALSE);
	push	040004000h
	mov	ebx,[ebp-010h];	popup
	push	0101b8h
	push	0192h
	push	ebx
	call	WinSendMsg

; 430 	    WinEnableMenuItem(popup, IDM_DELETE_PDEV, FALSE);
	push	040004000h
	push	0101b9h
	push	0192h
	push	ebx
	call	WinSendMsg

; 431 	    WinEnableMenuItem(popup, IDM_DESTROY_DRV, TRUE);
	push	04000h
	push	0101bah
	push	0192h
	push	ebx
	call	WinSendMsg

; 432 	    WinEnableMenuItem(popup, IDM_REMOVE_CHILD, FALSE);
	push	040004000h
	push	0101bbh
	push	0192h
	push	ebx
	call	WinSendMsg
	add	esp,040h

; 433 	}
	jmp	@BLBL16
@BLBL33:

; 434 	else if( work->pmrMenu->dsk->isHostdrive() == True )
	mov	eax,[ebp+0ch];	work
	mov	eax,[eax+01ch]
	mov	eax,[eax+01ch]
	mov	ebx,[eax]
	mov	ecx,[ebx+06ch]
	add	eax,ecx
	call	dword ptr [ebx+068h]
	cmp	al,01h
	jne	@BLBL35

; 439 	    WinEnableMenuItem(popup, IDM_CREATE_PDEV, TRUE);
	push	04000h
	mov	ebx,[ebp-010h];	popup
	push	0101b8h
	push	0192h
	push	ebx
	call	WinSendMsg

; 440 	    WinEnableMenuItem(popup, IDM_DELETE_PDEV, FALSE);
	push	040004000h
	push	0101b9h
	push	0192h
	push	ebx
	call	WinSendMsg

; 441 	    WinEnableMenuItem(popup, IDM_DESTROY_DRV, FALSE);
	push	040004000h
	push	0101bah
	push	0192h
	push	ebx
	call	WinSendMsg

; 442 	    WinEnableMenuItem(popup, IDM_REMOVE_CHILD, FALSE);
	push	040004000h
	push	0101bbh
	push	0192h
	push	ebx
	call	WinSendMsg
	add	esp,040h

; 443 	}
	jmp	@BLBL16
@BLBL35:

; 449 	    WinEnableMenuItem(popup, IDM_CREATE_PDEV, FALSE);
	push	040004000h
	mov	ebx,[ebp-010h];	popup
	push	0101b8h
	push	0192h
	push	ebx
	call	WinSendMsg

; 450 	    WinEnableMenuItem(popup, IDM_DELETE_PDEV, TRUE);
	push	04000h
	push	0101b9h
	push	0192h
	push	ebx
	call	WinSendMsg

; 451 	    WinEnableMenuItem(popup, IDM_DESTROY_DRV, FALSE);
	push	040004000h
	push	0101bah
	push	0192h
	push	ebx
	call	WinSendMsg

; 452 	    WinEnableMenuItem(popup, IDM_REMOVE_CHILD, FALSE);
	push	040004000h
	push	0101bbh
	push	0192h
	push	ebx
	call	WinSendMsg
	add	esp,040h

; 454     } /* hwndSngPopup */
@BLBL16:

; 456     return TRUE;				/* xxx TRUE? */
	add	esp,044h
	mov	eax,01h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
DoPopup__FUlCP5_WORK	endp

; 224 CreateCnr(HWND hwndClient,PWORK work)

CreateCnr__FUlP5_WORK	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,edx
	push	edi
	mov	edi,eax
	sub	esp,08ch

; 229     WinQueryWindowPos(hwndClient, &size);
	lea	ecx,[ebp-02ch];	size
	push	ecx

; 224 CreateCnr(HWND hwndClient,PWORK work)
	mov	[ebp+0ch],ebx;	work

; 229     WinQueryWindowPos(hwndClient, &size);
	push	edi
	call	WinQueryWindowPos

; 230     Verbose(3, "DrvSetup", "client window %u,%u %u,%u",
	push	dword ptr [ebp-028h];	size
	mov	ecx,offset FLAT:@CBE4
	push	dword ptr [ebp-024h];	size
	mov	edx,offset FLAT:@CBE3
	push	dword ptr [ebp-020h];	size
	mov	eax,03h
	push	dword ptr [ebp-01ch];	size
	sub	esp,0ch
	call	Verbose

; 233     work->hwndCnr = WinCreateWindow(hwndClient, WC_CONTAINER,
	push	0h

; 230     Verbose(3, "DrvSetup", "client window %u,%u %u,%u",
	mov	eax,edi

; 233     work->hwndCnr = WinCreateWindow(hwndClient, WC_CONTAINER,
	push	0h
	push	01a4h
	push	03h
	push	eax
	push	dword ptr [ebp-028h];	size
	push	dword ptr [ebp-024h];	size
	push	0h
	push	0h
	push	080000079h
	push	0h
	push	0ffff0025h
	push	eax
	call	WinCreateWindow
	mov	edx,ebx
	add	esp,058h
	mov	[edx+08h],eax

; 242     if( work->hwndCnr == NULLHANDLE )
	test	eax,eax
	jne	@BLBL7

; 244 	MyMessageBox(0, HWND_DESKTOP,
	push	dword ptr [edx+04h]
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE5
	sub	esp,0ch
	mov	edx,01h
	xor	eax,eax
	call	MyMessageBox__FCiCUlPce
	add	esp,014h

; 247 	return 1;
	add	esp,08ch
	mov	eax,01h
	pop	edi
	pop	ebx
	pop	ebp
	ret	
@BLBL7:

; 250     memset(&cnrinfo, 0, sizeof(cnrinfo));
	lea	ebx,[ebp-088h];	cnrinfo
	mov	ecx,05ch
	xor	edx,edx
	mov	eax,ebx
	call	memset

; 253     WinSendMsg(work->hwndCnr, CM_SETCNRINFO,
	push	04h

; 251     cnrinfo.cb = sizeof(cnrinfo);
	mov	dword ptr [ebp-088h],05ch;	cnrinfo

; 253     WinSendMsg(work->hwndCnr, CM_SETCNRINFO,
	push	ebx
	mov	ebx,[ebp+0ch];	work
	push	0349h

; 252     cnrinfo.flWindowAttr = CV_TREE|CV_ICON |CA_TREELINE;
	mov	dword ptr [ebp-074h],0400044h;	cnrinfo

; 253     WinSendMsg(work->hwndCnr, CM_SETCNRINFO,
	push	dword ptr [ebx+08h]
	call	WinSendMsg

; 259     work->hwndNonePopup = WinLoadMenu(HWND_OBJECT, NULLHANDLE, IDPM_NODRV);
	push	01b0h
	push	0h
	push	02h
	call	WinLoadMenu

; 260     work->hwndSngPopup = WinLoadMenu(HWND_OBJECT, NULLHANDLE, IDPM_SNGDRV);
	push	01aeh

; 259     work->hwndNonePopup = WinLoadMenu(HWND_OBJECT, NULLHANDLE, IDPM_NODRV);
	mov	[ebx+010h],eax

; 260     work->hwndSngPopup = WinLoadMenu(HWND_OBJECT, NULLHANDLE, IDPM_SNGDRV);
	push	0h
	push	02h
	call	WinLoadMenu

; 261     work->hwndMulPopup = WinLoadMenu(HWND_OBJECT, NULLHANDLE, IDPM_MULDRV);
	push	01afh

; 260     work->hwndSngPopup = WinLoadMenu(HWND_OBJECT, NULLHANDLE, IDPM_SNGDRV);
	mov	[ebx+014h],eax

; 261     work->hwndMulPopup = WinLoadMenu(HWND_OBJECT, NULLHANDLE, IDPM_MULDRV);
	push	0h
	push	02h
	call	WinLoadMenu
	add	esp,034h

; 262     if( work->hwndSngPopup == NULLHANDLE
	cmp	dword ptr [ebx+014h],0h

; 261     work->hwndMulPopup = WinLoadMenu(HWND_OBJECT, NULLHANDLE, IDPM_MULDRV);
	mov	[ebx+018h],eax

; 262     if( work->hwndSngPopup == NULLHANDLE
	je	@BLBL8
	test	eax,eax
	je	@BLBL8
	cmp	dword ptr [ebx+010h],0h
	jne	@BLBL9
@BLBL8:

; 265 	Verbose(1, "DrvSetup", "WinLoadMenu failed (last error %#x)",
	mov	ebx,[ebp+0ch];	work
	push	dword ptr [ebx+04h]
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE6
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE3
	mov	eax,01h
	call	Verbose
	add	esp,014h

; 266 		WinGetLastError(work->hab));
@BLBL9:

; 267     Verbose(3, "DrvSetup", "popup menues %#x,%#x,%#x",
	mov	ebx,[ebp+0ch];	work
	mov	ecx,offset FLAT:@CBE7
	push	dword ptr [ebx+010h]
	mov	edx,offset FLAT:@CBE3
	push	dword ptr [ebx+018h]
	mov	eax,03h
	push	dword ptr [ebx+014h]
	sub	esp,0ch
	call	Verbose
	add	esp,018h

; 272     return 0;
	add	esp,08ch
	xor	eax,eax
	pop	edi
	pop	ebx
	pop	ebp
	ret	
CreateCnr__FUlP5_WORK	endp

; 183 LoadPresParam(char * app)

LoadPresParam__FPc	proc
	sub	esp,08h

; 185     if( rgbBackground == RGB_ERROR )
	cmp	dword ptr  @2rgbBackground,0ffffff01h

; 183 LoadPresParam(char * app)
	push	ebx

; 185     if( rgbBackground == RGB_ERROR )
	jne	@BLBL3

; 189 	rgbBackground = WinQuerySysColor(HWND_DESKTOP, SYSCLR_WINDOW, 0);
	push	0h

; 185     if( rgbBackground == RGB_ERROR )
	mov	ebx,eax

; 189 	rgbBackground = WinQuerySysColor(HWND_DESKTOP, SYSCLR_WINDOW, 0);
	push	0ffffffech
	push	01h
	call	WinQuerySysColor

; 191 	ProfileRead(app, "Background", &cb, &rgbBackground);
	push	offset FLAT:@2rgbBackground

; 189 	rgbBackground = WinQuerySysColor(HWND_DESKTOP, SYSCLR_WINDOW, 0);
	mov	dword ptr  @2rgbBackground,eax

; 190 	cb = sizeof(rgbBackground);
	mov	dword ptr [esp+018h],04h;	cb

; 191 	ProfileRead(app, "Background", &cb, &rgbBackground);
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE1
	lea	ecx,[esp+024h];	cb
	mov	eax,ebx
	call	ProfileRead
	mov	eax,ebx
	add	esp,01ch

; 192     }
@BLBL3:

; 193     if( rgbForeground == RGB_ERROR )
	cmp	dword ptr  @3rgbForeground,0ffffff01h
	jne	@BLBL4

; 197 	rgbForeground = WinQuerySysColor(HWND_DESKTOP, SYSCLR_OUTPUTTEXT, 0);
	push	0h

; 193     if( rgbForeground == RGB_ERROR )
	mov	ebx,eax

; 197 	rgbForeground = WinQuerySysColor(HWND_DESKTOP, SYSCLR_OUTPUTTEXT, 0);
	push	0ffffffe5h
	push	01h
	call	WinQuerySysColor

; 199 	ProfileRead(app, "Foreground", &cb, &rgbForeground );
	push	offset FLAT:@3rgbForeground

; 197 	rgbForeground = WinQuerySysColor(HWND_DESKTOP, SYSCLR_OUTPUTTEXT, 0);
	mov	dword ptr  @3rgbForeground,eax

; 199 	ProfileRead(app, "Foreground", &cb, &rgbForeground );
	sub	esp,0ch

; 197 	rgbForeground = WinQuerySysColor(HWND_DESKTOP, SYSCLR_OUTPUTTEXT, 0);
	mov	eax,ebx

; 198 	cb = sizeof(rgbForeground);
	mov	dword ptr [esp+020h],04h;	cb

; 199 	ProfileRead(app, "Foreground", &cb, &rgbForeground );
	lea	ecx,[esp+020h];	cb
	mov	edx,offset FLAT:@CBE2
	call	ProfileRead
	add	esp,01ch

; 200     }
@BLBL4:

; 201     return;
	pop	ebx
	add	esp,08h
	ret	
LoadPresParam__FPc	endp
CODE32	ends
end
