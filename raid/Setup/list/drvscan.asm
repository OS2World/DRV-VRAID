	title	drvscan.cpp
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
	public	usHostId
	extrn	memcpy:proc
	extrn	memcmp:proc
	extrn	__vn__FUi:proc
	extrn	__nw__FUi:proc
	extrn	memset:proc
	extrn	DriverArrayinfo__FUsP21_VRAID_ARRAYINFO_DATA:proc
	extrn	Verbose:proc
	extrn	queryIndex__8VOS2DiskFv:proc
	extrn	DosSleep:proc
	extrn	__vd__FPv:proc
	extrn	DriverPhysinfo__FUsP20_VRAID_PHYSINFO_DATA:proc
	extrn	__dl__FPv:proc
	extrn	DriverPhyslist__FPUsPPUs:proc
	extrn	__ct__6VFRDevFUs:proc
	extrn	_Exception_CPP:proc
	extrn	DriverArraylist__FPUsPPUs:proc
	extrn	PDskEnum:proc
	extrn	PDskOpen:proc
	extrn	MyMessageBox__FCiCUlPce:proc
	extrn	__ct__8VOS2DiskFUlT1:proc
	extrn	__dt__8VOS2DiskFv:proc
	extrn	Crc16__FPvUs:proc
	extrn	__ct__6VChainFPUci:proc
	extrn	__ct__7VStripeFPUci:proc
	extrn	__ct__7VSingleFPUc:proc
	extrn	__ct__8VMissingFPUcUl:proc
	extrn	__ct__7VMirrorFPUci:proc
	extrn	__ct__9PartTableFP8VOS2DiskUl7Boolean:proc
	extrn	__dt__9PartTableFv:proc
	extrn	__ct__9VOS2DriveFP8VOS2Disk:proc
	extrn	__ct__6VORDevFP8VOS2DiskUlT2:proc
	extrn	queryDPB__8VOS2DiskFv:proc
	extrn	__ct__9PartTableFP8VOS2DiskUlT2:proc
	extrn	clear__9PartTableFv:proc
	extrn	addPE__9PartTableFRC15_PARTITIONENTRY:proc
	extrn	delPE__9PartTableFP15_PARTITIONENTRY:proc
	extrn	addForeign__9PartTableFUcT1UlT3:proc
	extrn	addOwn__9PartTableFUcT1UlT3:proc
	extrn	WinDlgBox:proc
	extrn	AreaDlgProc:proc
	extrn	WinQueryWindowPtr:proc
	extrn	CrcInit__FUs:proc
	extrn	QueryHostId__FCPUs:proc
	extrn	DriverStartSetup__Fv:proc
	extrn	WinPostMsg:proc
	extrn	WinDefWindowProc:proc
	extrn	WinInitialize:proc
	extrn	WinCreateMsgQueue:proc
	extrn	WinGetLastError:proc
	extrn	WinCancelShutdown:proc
	extrn	WinRegisterClass:proc
	extrn	WinCreateWindow:proc
	extrn	WinSetWindowPtr:proc
	extrn	WinGetMsg:proc
	extrn	WinDispatchMsg:proc
	extrn	WinDestroyWindow:proc
	extrn	WinDestroyMsgQueue:proc
	extrn	WinTerminate:proc
	extrn	fSetupMode:word
	extrn	_fltused:dword
CONST32_RO	segment
@CBE1	db "DrvScan",0h
@CBE2	db "Testing array %u",0h
	align 04h
@CBE3	db "Array Info %u - rc %lu",0h
	align 04h
@CBE4	db "Array %u accessed when r"
db "eading OS/2 Drive %lu",0h
	align 04h
@CBE5	db "Testing Physdev %u",0h
	align 04h
@CBE6	db "Physdev Info %u - rc %lu"
db 0h
	align 04h
@CBE7	db "PHYSDEV %u accessed when"
db " reading OS/2 disk %lu",0h
	align 04h
@CBE8	db "AllocateFltDevices",0h
	align 04h
@CBE9	db "DrvSetup",0h
	align 04h
@CBE10	db "DriverPhyslist - rc %lu",0h
@CBE11	db "FLT drive capacity: %lu "
db "MBytes",0h
	align 04h
@CBE12	db "AllocateOs2Devices",0h
	align 04h
@CBE13	db "DriverArraylist - rc %lu"
db 0h
	align 04h
@CBE14	db "PDskEnum - rc %lu",0h
	align 04h
@CBE15	db "PDskOpen(%lu) - rc %lu",0h
	align 04h
@CBE16	db "An OS2 disk cannot be ac"
db "cessed.  There may be an"
db " instance of FDISK runni"
db "ng somewhere.",0ah,"Please don"
db "'t change drive configur"
db "ation but end VSetup and"
db " any disk management sof"
db "tware like FDISK and res"
db "tart VSetup!",0h
	align 04h
@CBE17	db "CRC error in VRAIDDEVICE"
db " sector (%#x, %#x)",0h
	align 04h
@CBE18	db "Unknown VRAID type %u",0h
	align 04h
@CBE19	db "ValidateVSec2( child %p)"
db 0h
	align 04h
@CBE20	db "memory problem, can't va"
db "lidate child",0h
	align 04h
@CBE21	db "read error, can't valida"
db "te child",0h
	align 04h
@CBE22	db "VRAIDDEVICE2    ",0h
	align 04h
@CBE23	db "CRC error in VRAIDDEVICE"
db "2 sector (%#x, %#x)",0h
@CBE24	db "MIRROR creation delayed",0h
@CBE25	db "no VRAID partition on dr"
db "ive %u",0h
	align 04h
@CBE26	db "found VRAID partition on"
db " drive %u, size %lu",0h
@CBE27	db "read error %lu, device i"
db "gnored",0h
	align 04h
@CBE28	db "VRAIDDEVICE     ",0h
	align 04h
@CBE29	db "%s area (start %lu, size"
db " %lu), PT %lu, EXT %lu",0h
	align 04h
@CBE30	db "ScanExtAllocated(%lu, %l"
db "u)",0h
	align 04h
@CBE31	db "ScanExtAllocated - found"
db 0h
	align 04h
@CBE32	db "FindFreeExtSpace(%lu, %l"
db "u)",0h
	align 04h
@CBE33	db "FindFreeExtSpace - found"
db " used",0h
	align 04h
@CBE34	db "FindFreeSpace(%lu): no p"
db "artition entry available"
db 0h
	align 04h
@CBE35	db "FindFreeSpace - found fr"
db "ee",0h
	align 04h
@CBE36	db "FindFreeSpace - found",0h
	align 04h
@CBE37	db "FindFreeSpace - rc %lu",0h
	align 04h
@CBE38	db "FindFreeSpace(%lu, %lu)",0h
@CBE39	db "CreateLogPartition",0h
	align 04h
@CBE40	db "start %lu, size %lu",0h
@CBE41	db "table %lu",0h
	align 04h
@CBE42	db "invalid partition entry "
db "%d in sector %lu",0h
	align 04h
@CBE43	db "CreatePriPartition",0h
	align 04h
@CBE44	db "VRAID partition already "
db "there, stopping",0h
@CBE45	db "No room on disk, abortin"
db "g",0h
	align 04h
@CBE46	db "There is no room on requ"
db "ested disk to create a V"
db "RAID partition.",0h
@CBE47	db "CreatePhysdev: user abor"
db "ted",0h
@CBE48	db "DevScan",0h
@CBE49	db "CreatePhysdev: can't cle"
db "ar SEC_PHYSDEV- rc %lu",0h
	align 04h
@CBE50	db "WM_CREATE -> default",0h
	align 04h
@CBE51	db "couldn't determine host "
db "id",0h
	align 04h
@CBE52	db "WM_CLOSE",0h
	align 04h
@CBE53	db "WUM_START",0h
	align 04h
@CBE54	db "WUM_MKPDEV",0h
	align 04h
@CBE55	db "WUM_DELPDEV",0h
@CBE56	db "Make sure you delete the"
db " VRAID partition by hand"
db " after reboot.",0ah,"Use the c"
db "ontext menu of the drive"
db "s folder for this purpos"
db "e.",0h
	align 04h
@CBE57	db "WUM_DEVTREE",0h
@CBE58	db "WUM_DELDRV",0h
	align 04h
@CBE59	db "WUM_CLOSEDEV",0h
	align 04h
@CBE60	db "WUM_REMCHILD",0h
	align 04h
@CBE61	db "WinInitialize failed!?!",0h
@CBE62	db "WinCreateMsgQueue - erro"
db "r %#x",0h
	align 04h
@CBE63	db "DrvscanWindow",0h
	align 04h
@CBE64	db "WinRegisterClass - error"
db " %#x",0h
	align 04h
@CBE65	db "WinCreateWindow - error "
db "%#x",0h
@CBE66	db "WinSetWindowPtr failed -"
db " error %#",0h
	align 04h
@CBE67	db "DrvScanThread: stopped",0h
	align 04h
@1vcid	db "$Id: drvscan.cpp,v 2.5 2"
db "000/04/15 23:50:12 vitus"
db " Exp $",0h
CONST32_RO	ends
BSS32	segment
usHostId	dw 0h
	align 04h
@3ahsOs2Devid	db 0300h DUP (0h)
BSS32	ends
EH_DATA	segment
@59__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	dd -010h
	dd offset FLAT:__dl__FPv
	db 0fdh,0ffh,0ffh,0ffh
	db ",",0h,0h,0h
	db 0h,0h
	db 0h,0h
@7d__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	dd -010h
	dd offset FLAT:__dl__FPv
	db 0fdh,0ffh,0ffh,0ffh
	db " ",0h,0h,0h
	db 0h,0h
	db 0h,0h
@b5__fsm_tab	db 0adh,0deh,0efh,0beh
	db 04h,0h,0h,0h
	dd -010h
	dd offset FLAT:__dl__FPv
	db 0fdh,0ffh,0ffh,0ffh
	db "0",01h,0h,0h
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
	db "0",0h,0h,0h
	db 0h,0h
	db 0h,0h
	dd -01ch
	dd offset FLAT:__dl__FPv
	db 0fdh,0ffh,0ffh,0ffh
	db 018h,0h,0h,0h
	db 0h,0h
	db 0h,0h
@ea__fsm_tab	db 0adh,0deh,0efh,0beh
	db 02h,0h,0h,0h
	dd -010h
	dd offset FLAT:__dl__FPv
	db 0fdh,0ffh,0ffh,0ffh
	db "4",01h,0h,0h
	db 0h,0h
	db 0h,0h
	dd -014h
	dd offset FLAT:__dl__FPv
	db 0fdh,0ffh,0ffh,0ffh
	db 018h,0h,0h,0h
	db 0h,0h
	db 0h,0h
@109__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	dd -0228h
	dd offset FLAT:__dftdt__9PartTableFv
	db 01h,0h,0h,0h
	db 01ch,02h,0h,0h
	db 0h,0h
	db 0h,0h
@128__fsm_tab	db 0adh,0deh,0efh,0beh
	db 02h,0h,0h,0h
	dd -010h
	dd offset FLAT:__dl__FPv
	db 0fdh,0ffh,0ffh,0ffh
	db " ",0h,0h,0h
	db 0h,0h
	db 0h,0h
	dd -014h
	dd offset FLAT:__dl__FPv
	db 0fdh,0ffh,0ffh,0ffh
	db "0",0h,0h,0h
	db 0h,0h
	db 0h,0h
@14f__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	dd -0228h
	dd offset FLAT:__dftdt__9PartTableFv
	db 01h,0h,0h,0h
	db 01ch,02h,0h,0h
	db 0h,0h
	db 0h,0h
@174__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	dd -0228h
	dd offset FLAT:__dftdt__9PartTableFv
	db 01h,0h,0h,0h
	db 01ch,02h,0h,0h
	db 0h,0h
	db 0h,0h
@190__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	dd -0228h
	dd offset FLAT:__dftdt__9PartTableFv
	db 01h,0h,0h,0h
	db 01ch,02h,0h,0h
	db 0h,0h
	db 0h,0h
@1b8__fsm_tab	db 0adh,0deh,0efh,0beh
	db 03h,0h,0h,0h
	dd -0228h
	dd offset FLAT:__dftdt__9PartTableFv
	db 01h,0h,0h,0h
	db 01ch,02h,0h,0h
	db 0h,0h
	db 0h,0h
	dd -0444h
	dd offset FLAT:__dftdt__9PartTableFv
	db 01h,0h,0h,0h
	db 01ch,02h,0h,0h
	db 0h,0h
	db 01h,0h
	dd -0660h
	dd offset FLAT:__dftdt__9PartTableFv
	db 01h,0h,0h,0h
	db 01ch,02h,0h,0h
	db 0h,0h
	db 01h,0h
@1cb__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	dd -0228h
	dd offset FLAT:__dftdt__9PartTableFv
	db 01h,0h,0h,0h
	db 01ch,02h,0h,0h
	db 0h,0h
	db 0h,0h
@1ef__fsm_tab	db 0adh,0deh,0efh,0beh
	db 02h,0h,0h,0h
	dd -010h
	dd offset FLAT:__dl__FPv
	db 0fdh,0ffh,0ffh,0ffh
	db "0",0h,0h,0h
	db 0h,0h
	db 0h,0h
	dd -014h
	dd offset FLAT:__dl__FPv
	db 0fdh,0ffh,0ffh,0ffh
	db "0",0h,0h,0h
	db 0h,0h
	db 0h,0h
@227__fsm_tab	db 0adh,0deh,0efh,0beh
	db 01h,0h,0h,0h
	dd -010h
	dd offset FLAT:__dl__FPv
	db 0fdh,0ffh,0ffh,0ffh
	db " ",0h,0h,0h
	db 0h,0h
	db 0h,0h
EH_DATA	ends
CODE32	segment

; 2176 DrvscanThread(void *dummy)

	public DrvscanThread
DrvscanThread	proc
	push	ebp

; 2187 	memset(ahsOs2Devid, 0, sizeof(ahsOs2Devid));
	mov	ecx,0300h

; 2176 DrvscanThread(void *dummy)
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi

; 2187 	memset(ahsOs2Devid, 0, sizeof(ahsOs2Devid));
	xor	edx,edx

; 2176 DrvscanThread(void *dummy)
	push	esi

; 2187 	memset(ahsOs2Devid, 0, sizeof(ahsOs2Devid));
	mov	eax,offset FLAT:@3ahsOs2Devid

; 2176 DrvscanThread(void *dummy)
	sub	esp,03ch

; 2178     PTHR_DRVSCAN dta = (PTHR_DRVSCAN)dummy;
	mov	[ebp-01ch],ebx;	dta

; 2187 	memset(ahsOs2Devid, 0, sizeof(ahsOs2Devid));
	call	memset

; 2189 	work = new WORK;
	mov	eax,020h
	call	__nw__FUi
	mov	edi,eax

; 2190 	memset(work, 0, sizeof(*work));
	mov	ecx,020h
	xor	edx,edx
	call	memset
	mov	eax,ebx

; 2191 	work->self = dta;
	mov	[edi],eax

; 2193 	work->apOs2Dsk = new VOS2Disk *[MAXDRIVES];
	mov	eax,0100h
	call	__vn__FUi
	mov	[edi+08h],eax

; 2194 	memset(work->apOs2Dsk, 0, MAXDRIVES * sizeof(VOS2Disk *));
	mov	ecx,0100h
	xor	edx,edx
	call	memset

; 2196 	work->apDskInfo = new VRDev *[MAXDRIVES];
	mov	eax,0100h
	call	__vn__FUi

; 2189 	work = new WORK;
	mov	esi,edi

; 2196 	work->apDskInfo = new VRDev *[MAXDRIVES];
	mov	[edi+010h],eax

; 2197 	memset(work->apDskInfo, 0, MAXDRIVES * sizeof(VRDev *));
	mov	ecx,0100h
	xor	edx,edx
	call	memset

; 2199 	work->apEndPts = new VRDev *[MAXDRIVES];
	mov	eax,0100h
	call	__vn__FUi

; 2201 	if( !(work->hab = WinInitialize(0)) )
	push	0h

; 2199 	work->apEndPts = new VRDev *[MAXDRIVES];
	mov	[edi+018h],eax

; 2201 	if( !(work->hab = WinInitialize(0)) )
	call	WinInitialize
	add	esp,04h
	mov	[edi+04h],eax
	cmp	dword ptr [edi+04h],0h
	jne	@BLBL463

; 2203 	    Verbose(0, "DrvScan", "WinInitialize failed!?!");
	xor	eax,eax
	mov	ecx,offset FLAT:@CBE61
	mov	edx,offset FLAT:@CBE1
	call	Verbose

; 2204 	    break;
	jmp	@BLBL464
@BLBL463:

; 2207 	if( !(hmq = WinCreateMsgQueue(work->hab, 0)) )
	push	0h
	mov	ebx,esi
	push	dword ptr [ebx+04h]
	call	WinCreateMsgQueue
	add	esp,08h
	mov	[ebp-018h],eax;	hmq
	test	eax,eax
	jne	@BLBL465

; 2209 	    Verbose(0, "DrvScan", "WinCreateMsgQueue - error %#x",
	push	dword ptr [ebx+04h]
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE62
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE1
	xor	eax,eax
	call	Verbose
	add	esp,014h

; 2211 	    break;
	jmp	@BLBL464
@BLBL465:

; 2213 	WinCancelShutdown(hmq, TRUE);		/* don't want to handle it */
	push	01h

; 2215 	if( !WinRegisterClass(work->hab, WC_DRVSCAN, DrvscanWindowProc,
	mov	ebx,esi

; 2213 	WinCancelShutdown(hmq, TRUE);		/* don't want to handle it */
	push	dword ptr [ebp-018h];	hmq
	call	WinCancelShutdown

; 2215 	if( !WinRegisterClass(work->hab, WC_DRVSCAN, DrvscanWindowProc,
	push	04h
	push	0h
	push	offset FLAT: DrvscanWindowProc
	push	offset FLAT:@CBE63
	push	dword ptr [ebx+04h]
	call	WinRegisterClass
	add	esp,01ch
	test	eax,eax
	jne	@BLBL466

; 2218 	    Verbose(0, "DrvSetup", "WinRegisterClass - error %#x",
	push	dword ptr [ebx+04h]
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE64
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE9
	xor	eax,eax
	call	Verbose
	add	esp,014h

; 2220 	    break;
	jmp	@BLBL464
@BLBL466:

; 2223 	hwnd = WinCreateWindow(HWND_OBJECT, WC_DRVSCAN, NULL,
	push	0h
	push	0h
	push	0h
	push	04h
	push	0h
	push	0h
	push	0h
	push	0h
	push	0h
	push	0h
	push	0h
	push	offset FLAT:@CBE63
	push	02h
	call	WinCreateWindow
	add	esp,034h
	mov	[ebp-014h],eax;	hwnd

; 2230 	if( hwnd == NULL )
	test	eax,eax
	jne	@BLBL467

; 2232 	    Verbose(0, "DrvScan", "WinCreateWindow - error %#x",
	mov	ebx,esi
	push	dword ptr [ebx+04h]
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE65
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE1
	xor	eax,eax
	call	Verbose
	add	esp,014h

; 2234 	    break;
	jmp	@BLBL464
@BLBL467:

; 2237 	bool = WinSetWindowPtr(hwnd, 0, work);
	mov	ebx,esi
	push	ebx
	push	0h
	push	dword ptr [ebp-014h];	hwnd
	call	WinSetWindowPtr
	add	esp,0ch

; 2238 	if( bool == FALSE )
	test	eax,eax
	jne	@BLBL468

; 2240 	    MyMessageBox(0, HWND_DESKTOP,
	push	dword ptr [ebx+04h]
	call	WinGetLastError
	push	eax
	mov	ecx,offset FLAT:@CBE66
	sub	esp,0ch
	mov	edx,01h
	xor	eax,eax
	call	MyMessageBox__FCiCUlPce
	add	esp,014h

; 2243 	    break;
	jmp	@BLBL464
@BLBL468:

; 2248 	WinPostMsg(hwnd, WUM_START, 0, 0);
	push	0h

; 2249 	while( WinGetMsg(work->hab, &qmsg, 0L, 
; 2249 0, 0) )
	mov	ebx,esi

; 2248 	WinPostMsg(hwnd, WUM_START, 0, 0);
	push	0h
	push	01002h
	push	dword ptr [ebp-014h];	hwnd
	call	WinPostMsg

; 2249 	while( WinGetMsg(work->hab, &qmsg, 0L, 
; 2249 0, 0) )
	push	0h
	lea	ecx,[ebp-03ch];	qmsg
	push	0h
	push	0h
	push	ecx
	push	dword ptr [ebx+04h]
	call	WinGetMsg
	add	esp,024h
	test	eax,eax
	je	@BLBL469
	mov	edi,ebx
@BLBL470:

; 2250 	    WinDispatchMsg(work->hab, &qmsg);
	lea	ebx,[ebp-03ch];	qmsg
	push	ebx
	push	dword ptr [edi+04h]
	call	WinDispatchMsg

; 2249 	while( WinGetMsg(work->hab, &qmsg, 0L, 
; 2249 0, 0) )
	push	0h
	push	0h
	push	0h
	push	ebx
	push	dword ptr [edi+04h]
	call	WinGetMsg
	add	esp,01ch
	test	eax,eax
	jne	@BLBL470
@BLBL469:

; 2254 	WinDestroyWindow(hwnd);
	push	dword ptr [ebp-014h];	hwnd

; 2256 	WinTerminate(work->hab);
	mov	edi,esi

; 2254 	WinDestroyWindow(hwnd);
	call	WinDestroyWindow

; 2255 	WinDestroyMsgQueue(hmq);
	push	dword ptr [ebp-018h];	hmq
	call	WinDestroyMsgQueue

; 2256 	WinTerminate(work->hab);
	push	dword ptr [edi+04h]
	call	WinTerminate
	mov	eax,edi

; 2257 	delete work;
	call	__dl__FPv
	add	esp,0ch

; 2259     while(0);
@BLBL464:

; 2262     WinPostMsg(dta->parent, WUM_THREADENDED, dta, 0);
	mov	ebx,[ebp-01ch];	dta

; 2261     Verbose(2, "DrvScan", "DrvScanThread: stopped");
	mov	ecx,offset FLAT:@CBE67
	mov	edx,offset FLAT:@CBE1
	mov	eax,02h
	call	Verbose

; 2262     WinPostMsg(dta->parent, WUM_THREADENDED, dta, 0);
	push	0h
	push	ebx
	push	01003h
	push	dword ptr [ebx]
	call	WinPostMsg

; 2263     return;
	add	esp,04ch
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
DrvscanThread	endp

; 1941 DrvscanWindowProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)

DrvscanWindowProc	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffb4h
	sub	esp,044h

; 1942 {
	mov	dword ptr [ebp-04ch],0h;	__es

; 1941 DrvscanWindowProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
	push	ebx

; 1942 {
	mov	dword ptr [ebp-044h],offset FLAT:@227__fsm_tab;	__es

; 1941 DrvscanWindowProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
	push	edi
	push	esi

; 1943     WORK * const work = (PWORK)WinQueryWindowPtr(hwnd, 0);
	push	0h
	push	dword ptr [ebp+08h];	hwnd
	call	WinQueryWindowPtr

; 1947     switch( msg )
	mov	ecx,[ebp+0ch];	msg

; 1943     WORK * const work = (PWORK)WinQueryWindowPtr(hwnd, 0);
	add	esp,08h
	mov	[ebp-014h],eax;	work

; 1947     switch( msg )
	cmp	ecx,0100bh
	jl	@BLBL488
	je	@BLBL483
	cmp	ecx,0100dh
	jl	@BLBL489
	je	@BLBL482
	cmp	ecx,0100eh
	je	@BLBL487
	cmp	ecx,0100fh
	je	@BLBL486
	jmp	@BLBL478
@BLBL489:
	cmp	ecx,0100ch
	je	@BLBL485
	jmp	@BLBL478
@BLBL488:
	cmp	ecx,029h
	jl	@BLBL490
	je	@BLBL480
	cmp	ecx,01002h
	je	@BLBL481
	cmp	ecx,0100ah
	je	@BLBL484
	jmp	@BLBL478
@BLBL490:
	cmp	ecx,01h
	jne	@BLBL478
@BLBL479:

; 1950 	Verbose(3, "DrvScan", "WM_CREATE -> default");
	sub	esp,0ch
	mov	ebx,offset FLAT:@CBE1
	mov	ecx,offset FLAT:@CBE50
	mov	edx,ebx
	mov	eax,03h
	call	Verbose
	add	esp,08h

; 1951 	CrcInit(0xA001);			/* CRC16 table */
	mov	ax,0a001h
	call	CrcInit__FUs

; 1952 	if( (rc=QueryHostId(&usHostId)) != 0 )
	mov	eax,offset FLAT:usHostId
	call	QueryHostId__FCPUs
	add	esp,04h
	mov	edx,ebx
	test	eax,eax
	je	@BLBL425

; 1954 	    Verbose(0, "DrvScan", "couldn't determine host id");
	xor	eax,eax
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE51
	call	Verbose
	add	esp,0ch

; 1955 	    return 0;
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	add	esp,048h
	pop	fs:[0h]
	leave	
	ret	
@BLBL425:

; 1957 	break;
	jmp	@BLBL478
@BLBL480:

; 1964 	Verbose(3, "DrvScan", "WM_CLOSE");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE52
	mov	edx,offset FLAT:@CBE1
	mov	eax,03h
	call	Verbose
	add	esp,0ch

; 1965 	break;
	jmp	@BLBL478
@BLBL481:

; 1972 	Verbose(3, "DrvScan", "WUM_START");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE53
	mov	edx,offset FLAT:@CBE1
	mov	eax,03h
	call	Verbose
	add	esp,0ch

; 1974 	    if( (fSetupMode & SM_FLTAVAIL) )
	test	byte ptr  fSetupMode,01h
	je	@BLBL427

; 1976 		DriverStartSetup();		/* end [re]builds, etc. */
	call	DriverStartSetup__Fv

; 1978 		AllocateFltDevices(work);
	sub	esp,04h
	mov	eax,[ebp-014h];	work
	call	AllocateFltDevices__FCP5_WORK
	add	esp,04h

; 1980 	    }
@BLBL427:
	sub	esp,04h
	mov	ebx,[ebp-014h];	work
	mov	eax,ebx
	call	AllocateOs2Devices__FCP5_WORK

; 1983 	    BuildDevTree(work);
	mov	eax,ebx
	call	BuildDevTree__FCP5_WORK
	add	esp,04h

; 1986 	    for( i = 0; i < work->cEndPts; ++i )
	cmp	dword ptr [ebx+01ch],0h
	jbe	@BLBL428
	mov	esi,[ebp+08h];	hwnd
	xor	edi,edi
@BLBL429:

; 1988 		VRDev * const	dev = work->apEndPts[i];
	mov	eax,[ebx+018h]

; 1990 		WinPostMsg(work->self->parent, WUM_DEVTREE, dev, MPFROMHWND(hwnd));
	push	esi

; 1988 		VRDev * const	dev = work->apEndPts[i];
	mov	eax,dword ptr [eax+edi*04h]

; 1990 		WinPostMsg(work->self->parent, WUM_DEVTREE, dev, MPFROMHWND(hwnd));
	push	eax
	mov	eax,[ebx]
	push	0100ah

; 1986 	    for( i = 0; i < work->cEndPts; ++i )
	inc	edi

; 1990 		WinPostMsg(work->self->parent, WUM_DEVTREE, dev, MPFROMHWND(hwnd));
	push	dword ptr [eax]
	call	WinPostMsg
	add	esp,010h

; 1986 	    for( i = 0; i < work->cEndPts; ++i )
	cmp	[ebx+01ch],edi
	ja	@BLBL429
@BLBL428:

; 1993 	return 0;
	pop	esi
	xor	eax,eax
	pop	edi
	pop	ebx
	add	esp,048h
	pop	fs:[0h]
	leave	
	ret	
@BLBL482:

; 2002 	    VOS2Drive *	drive = (VOS2Drive *)PVOIDFROMMP(mp1);
	mov	esi,[ebp+010h];	mp1

; 2000 	Verbose(3, "DrvScan", "WUM_MKPDEV");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE54
	mov	edx,offset FLAT:@CBE1
	mov	eax,03h
	call	Verbose
	add	esp,08h

; 2002 	    VOS2Drive *	drive = (VOS2Drive *)PVOIDFROMMP(mp1);
	mov	[ebp-028h],esi;	drive

; 2003 	    VOS2Disk *	disk = drive->queryWhere();
	mov	ecx,[esi]
	mov	eax,[ecx+05ch]
	add	eax,esi
	call	dword ptr [ecx+058h]
	sub	esp,08h

; 2006 	    rc = CreatePhysdev(hwnd, disk, &rdev);
	mov	edx,eax
	mov	eax,[ebp+08h];	hwnd
	lea	ecx,[ebp-018h];	rdev
	call	CreatePhysdev__FUlCP8VOS2DiskPP6VORDev
	add	esp,0ch

; 2007 	    if( rc == 0 )
	test	eax,eax
	jne	@BLBL433

; 2014                 drive->ioDeleteYourself();
	mov	ecx,[esi]
	sub	esp,04h
	mov	eax,[ecx+03ch]
	add	eax,esi
	call	dword ptr [ecx+038h]
	mov	eax,esi
	add	esp,04h

; 2015                 delete drive;
	test	eax,eax
	je	@BLBL435
	mov	edi,[eax]
	sub	esp,0ch
	mov	esi,[edi+02ch]
	xor	ecx,ecx
	mov	edx,03h
	add	eax,esi
	call	dword ptr [edi+028h]
	add	esp,0ch
@BLBL435:

; 2017 		rdev->ioSync();
	sub	esp,04h
	mov	eax,[ebp-018h];	rdev
	mov	ebx,[eax]
	mov	ecx,[ebx+054h]
	add	eax,ecx
	call	dword ptr [ebx+050h]
	add	esp,04h

; 2018 		WinPostMsg(work->self->parent, WUM_DEVTREE, rdev, MPFROMHWND(hwnd));
	mov	ebx,[ebp+08h];	hwnd
	push	ebx
	mov	ebx,[ebp-014h];	work
	push	dword ptr [ebp-018h];	rdev
	mov	ebx,[ebx]
	push	0100ah
	push	dword ptr [ebx]
	call	WinPostMsg
	add	esp,010h

; 2019 	    }
	jmp	@BLBL436
@BLBL433:

; 2021 		WinPostMsg(work->self->parent, WUM_DEVTREE, drive, MPFROMHWND(hwnd));
	mov	ebx,[ebp+08h];	hwnd
	push	ebx
	mov	ebx,[ebp-014h];	work
	push	dword ptr [ebp-028h];	drive
	mov	ebx,[ebx]
	push	0100ah
	push	dword ptr [ebx]
	call	WinPostMsg
	add	esp,010h
@BLBL436:

; 2023 	return 0;
	pop	esi
	xor	eax,eax
	pop	edi
	pop	ebx
	add	esp,048h
	pop	fs:[0h]
	leave	
	ret	
@BLBL483:

; 2037 	    VRDev *	dev = (VRDev *)PVOIDFROMMP(mp1);
	mov	esi,[ebp+010h];	mp1

; 2035 	Verbose(3, "DrvScan", "WUM_DELPDEV");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE55
	mov	edx,offset FLAT:@CBE1
	mov	eax,03h
	call	Verbose
	add	esp,08h

; 2037 	    VRDev *	dev = (VRDev *)PVOIDFROMMP(mp1);
	mov	[ebp-01ch],esi;	dev

; 2038 	    VOS2Disk *	disk = dev->queryWhere();
	mov	ecx,[esi]
	mov	eax,[ecx+05ch]
	add	eax,esi
	call	dword ptr [ecx+058h]
	mov	edi,eax
	mov	[ebp-02ch],edi;	disk

; 2041 	    dev->ioDeleteYourself();		/* removes SEC_PHYSDEV [+partition]*/
	mov	ecx,[esi]
	mov	eax,[ecx+03ch]
	add	eax,esi
	call	dword ptr [ecx+038h]
	add	esp,04h
	mov	eax,esi

; 2043 	    if( disk != NULL )
	test	edi,edi
	je	@BLBL437

; 2045 		delete dev,	dev = NULL;
	test	eax,eax
	je	@BLBL439
	mov	edi,[eax]
	sub	esp,0ch
	mov	esi,[edi+02ch]
	xor	ecx,ecx
	mov	edx,03h
	add	eax,esi
	call	dword ptr [edi+028h]
	add	esp,0ch
@BLBL439:

; 2047 		VOS2Drive *	drive = new VOS2Drive(disk);
	sub	esp,04h
	mov	eax,020h
	call	__nw__FUi

; 2045 		delete dev,	dev = NULL;
	mov	dword ptr [ebp-01ch],0h;	dev

; 2047 		VOS2Drive *	drive = new VOS2Drive(disk);
	add	esp,04h
	mov	[ebp-010h],eax;	__107
	test	eax,eax
	je	@BLBL441
	sub	esp,08h
	mov	edx,[ebp-02ch];	disk
	mov	dword ptr [ebp-04ch],01h;	__es
	call	__ct__9VOS2DriveFP8VOS2Disk
	add	esp,08h
	mov	dword ptr [ebp-04ch],0h;	__es
@BLBL441:

; 2048 		WinPostMsg(work->self->parent, WUM_DEVTREE, drive, MPFROMHWND(hwnd));
	mov	ebx,[ebp+08h];	hwnd
	push	ebx
	mov	ebx,[ebp-014h];	work
	push	dword ptr [ebp-010h];	__107
	mov	ebx,[ebx]
	push	0100ah
	push	dword ptr [ebx]
	call	WinPostMsg
	add	esp,010h

; 2049 	    }
	jmp	@BLBL442
@BLBL437:

; 2056 		WinPostMsg(work->self->parent, WUM_DEVTREE, dev, MPFROMHWND(hwnd));
	mov	ebx,[ebp+08h];	hwnd

; 2052 		MyMessageBox(3, HWND_DESKTOP,
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE56
	mov	edx,01h
	mov	eax,03h
	call	MyMessageBox__FCiCUlPce
	add	esp,0ch

; 2056 		WinPostMsg(work->self->parent, WUM_DEVTREE, dev, MPFROMHWND(hwnd));
	push	ebx
	mov	ebx,[ebp-014h];	work
	push	dword ptr [ebp-01ch];	dev
	mov	ebx,[ebx]
	push	0100ah
	push	dword ptr [ebx]
	call	WinPostMsg
	add	esp,010h

; 2057 	    }
@BLBL442:

; 2059 	return 0;
	pop	esi
	xor	eax,eax
	pop	edi
	pop	ebx
	add	esp,048h
	pop	fs:[0h]
	leave	
	ret	
@BLBL484:

; 2075 	    dev->ioSync();			/* write to disk[s] */
	mov	ebx,[ebp+010h];	mp1

; 2071 	Verbose(3, "DrvScan", "WUM_DEVTREE");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE57
	mov	edx,offset FLAT:@CBE1
	mov	eax,03h
	call	Verbose

; 2075 	    dev->ioSync();			/* write to disk[s] */
	mov	ecx,[ebx]

; 2071 	Verbose(3, "DrvScan", "WUM_DEVTREE");
	add	esp,08h

; 2075 	    dev->ioSync();			/* write to disk[s] */
	mov	eax,[ecx+054h]
	add	eax,ebx
	call	dword ptr [ecx+050h]
	add	esp,04h

; 2076 	    WinPostMsg(work->self->parent, WUM_DEVTREE, dev, MPFROMHWND(hwnd));
	mov	ecx,[ebp+08h];	hwnd
	push	ecx
	mov	ecx,[ebp-014h];	work
	push	ebx
	mov	ecx,[ecx]
	push	0100ah
	push	dword ptr [ecx]
	call	WinPostMsg
	add	esp,010h

; 2078 	return 0;
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	add	esp,048h
	pop	fs:[0h]
	leave	
	ret	
@BLBL485:

; 2088 	    VRDrive * const	drv = (VRDrive *)PVOIDFROMMP(mp1);
	mov	ebx,[ebp+010h];	mp1

; 2086 	Verbose(3, "DrvScan", "WUM_DELDRV");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE58
	mov	edx,offset FLAT:@CBE1
	mov	eax,03h
	call	Verbose
	add	esp,08h

; 2088 	    VRDrive * const	drv = (VRDrive *)PVOIDFROMMP(mp1);
	mov	[ebp-030h],ebx;	drv

; 2092 	    for( int i = 0; i < drv->queryChildren(); ++i )
	mov	ecx,[ebx]
	mov	eax,[ecx+094h]
	add	eax,ebx
	call	dword ptr [ecx+090h]
	add	esp,04h
	mov	ecx,ebx
	test	eax,eax
	jle	@BLBL443
	mov	ebx,[ebp-014h];	work
	mov	esi,ecx
	xor	edi,edi
@BLBL444:

; 2094 		VRDev *	const	child = drv->getChild(i);
	mov	ecx,[esi]
	sub	esp,08h
	mov	eax,[ecx+09ch]
	mov	edx,edi
	add	eax,esi
	call	dword ptr [ecx+098h]
	mov	edx,eax
	add	esp,04h

; 2096 		if( child->isArray() != 0xFF )
	mov	ecx,[edx]
	mov	eax,[ecx+064h]
	add	eax,edx
	mov	[ebp-050h],edx;	@CBE115
	call	dword ptr [ecx+060h]
	mov	edx,[ebp-050h];	@CBE115
	add	esp,04h
	cmp	al,0ffh
	je	@BLBL446

; 2097 		    WinPostMsg(work->self->parent, WUM_DEVTREE, child, MPFROMHWND(hwnd));
	mov	ecx,[ebp+08h];	hwnd
	push	ecx
	mov	ecx,[ebx]
	push	edx
	push	0100ah
	push	dword ptr [ecx]
	call	WinPostMsg
	add	esp,010h

; 2098 	    }
@BLBL446:

; 2092 	    for( int i = 0; i < drv->queryChildren(); ++i )
	mov	ecx,[esi]
	sub	esp,04h
	mov	eax,[ecx+094h]
	add	eax,esi
	call	dword ptr [ecx+090h]
	inc	edi
	add	esp,04h
	cmp	eax,edi
	jg	@BLBL444
@BLBL443:

; 2100 	    drv->ioDeleteYourself();
	sub	esp,04h
	mov	esi,[ebp-030h];	drv
	mov	ecx,[esi]
	mov	eax,[ecx+03ch]
	add	eax,esi
	call	dword ptr [ecx+038h]
	mov	eax,esi
	add	esp,04h

; 2101 	    delete drv;
	test	eax,eax
	je	@BLBL450
	mov	edi,[eax]
	sub	esp,0ch
	mov	esi,[edi+02ch]
	xor	ecx,ecx
	mov	edx,03h
	add	eax,esi
	call	dword ptr [edi+028h]
	add	esp,0ch
@BLBL450:

; 2103 	return 0;
	pop	esi
	xor	eax,eax
	pop	edi
	pop	ebx
	add	esp,048h
	pop	fs:[0h]
	leave	
	ret	
@BLBL486:

; 2111 	Verbose(3, "DrvScan", "WUM_CLOSEDEV");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE59
	mov	edx,offset FLAT:@CBE1
	mov	eax,03h
	call	Verbose

; 2114 	    delete dev;
	mov	eax,[ebp+010h];	mp1

; 2111 	Verbose(3, "DrvScan", "WUM_CLOSEDEV");
	add	esp,0ch

; 2114 	    delete dev;
	test	eax,eax
	je	@BLBL452
	mov	ebx,[eax]
	sub	esp,0ch
	mov	edi,[ebx+02ch]
	xor	ecx,ecx
	mov	edx,03h
	add	eax,edi
	call	dword ptr [ebx+028h]
	add	esp,0ch
@BLBL452:

; 2116 	return 0;
	pop	esi
	xor	eax,eax
	pop	edi
	pop	ebx
	add	esp,048h
	pop	fs:[0h]
	leave	
	ret	
@BLBL487:

; 2127 	    VDriveWithRedundancy * parent = (VDriveWithRedundancy *)PVOIDFROMMP(mp2);
	mov	ebx,[ebp+014h];	mp2

; 2123 	Verbose(3, "DrvScan", "WUM_REMCHILD");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE60
	mov	edx,offset FLAT:@CBE1
	mov	eax,03h
	call	Verbose

; 2126 	    VRDev *	child = (VRDev *)PVOIDFROMMP(mp1);
	mov	eax,[ebp+010h];	mp1

; 2125 	    Boolean	postchild = True;
	mov	byte ptr [ebp-035h],01h;	postchild

; 2123 	Verbose(3, "DrvScan", "WUM_REMCHILD");
	add	esp,08h

; 2126 	    VRDev *	child = (VRDev *)PVOIDFROMMP(mp1);
	mov	[ebp-020h],eax;	child

; 2127 	    VDriveWithRedundancy * parent = (VDriveWithRedundancy *)PVOIDFROMMP(mp2);
	mov	[ebp-034h],ebx;	parent

; 2129 	    if( child->isArray() == 0xFF )	/* VMissing!  Don't post. */
	mov	ebx,[eax]
	mov	ecx,[ebx+064h]
	add	eax,ecx
	call	dword ptr [ebx+060h]
	add	esp,04h
	cmp	al,0ffh
	jne	@BLBL453

; 2130 		postchild = False;
	mov	byte ptr [ebp-035h],0h;	postchild
@BLBL453:

; 2132 	    parent->ioRemoveChild(child);
	sub	esp,08h
	mov	ebx,[ebp-034h];	parent
	mov	edx,[ebp-020h];	child
	mov	ecx,[ebx]
	mov	eax,[ecx+0bch]
	add	eax,ebx
	call	dword ptr [ecx+0b8h]
	mov	ecx,ebx
	add	esp,08h

; 2134 	    VRDev * higher = parent;
	mov	[ebp-024h],ecx;	higher

; 2135 	    while( higher->getParent() != 0  )
	cmp	dword ptr [ecx+04h],0h
	je	@BLBL454
@BLBL455:

; 2136 		higher = higher->getParent();
	mov	ecx,[ecx+04h]

; 2135 	    while( higher->getParent() != 0  )
	cmp	dword ptr [ecx+04h],0h
	jne	@BLBL455
	mov	[ebp-024h],ecx;	higher
@BLBL454:

; 2137 	    WinPostMsg(work->self->parent, WUM_DEVTREE, higher, MPFROMHWND(hwnd));
	mov	ebx,[ebp+08h];	hwnd
	push	ebx
	mov	edi,[ebp-014h];	work
	push	dword ptr [ebp-024h];	higher
	mov	ecx,[edi]
	push	0100ah
	push	dword ptr [ecx]
	call	WinPostMsg
	add	esp,010h

; 2139 	    if( postchild == True )
	cmp	byte ptr [ebp-035h],01h;	postchild
	jne	@BLBL459

; 2140 		WinPostMsg(work->self->parent, WUM_DEVTREE, child, MPFROMHWND(hwnd));
	push	ebx
	push	dword ptr [ebp-020h];	child
	mov	ecx,[edi]
	push	0100ah
	push	dword ptr [ecx]
	call	WinPostMsg
	add	esp,010h
@BLBL459:

; 2142 	return 0;
	pop	esi
	xor	eax,eax
	pop	edi
	pop	ebx
	add	esp,048h
	pop	fs:[0h]
	leave	
	ret	

; 2145 	break;
@BLBL478:

; 2148     return WinDefWindowProc(hwnd, msg, mp1, mp2);
	push	dword ptr [ebp+014h];	mp2
	push	dword ptr [ebp+010h];	mp1
	push	dword ptr [ebp+0ch];	msg
	push	dword ptr [ebp+08h];	hwnd
	call	WinDefWindowProc
	add	esp,010h
	pop	esi
	pop	edi
	pop	ebx
	add	esp,048h
	pop	fs:[0h]
	leave	
	ret	
DrvscanWindowProc	endp

; 1842 CreatePhysdev(HWND hwnd,VOS2Disk * const disk,VORDev ** dev)

CreatePhysdev__FUlCP8VOS2DiskPP6VORDev	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0fffffef4h
	mov	[ebp+08h],eax;	hwnd
	sub	esp,0100h

; 1845     DSKAREA	info = {0};
	xor	eax,eax

; 1842 CreatePhysdev(HWND hwnd,VOS2Disk * const disk,VORDev ** dev)
	push	ebx
	mov	ebx,edx
	push	edi
	mov	edx,ecx
	push	esi

; 1845     DSKAREA	info = {0};
	lea	edi,[ebp-0f4h];	info
	mov	ecx,032h

; 1852 	rc = FindFreeSpace(disk, 0, disk->querySize(), &info, &found);
	sub	esp,04h

; 1842 CreatePhysdev(HWND hwnd,VOS2Disk * const disk,VORDev ** dev)
	mov	[ebp+010h],edx;	dev
	mov	[ebp+0ch],ebx;	disk

; 1843 {
	mov	dword ptr [ebp-010ch],0h;	__es
	mov	dword ptr [ebp-0104h],offset FLAT:@1ef__fsm_tab;	__es

; 1844     int		found = -1;			/* indices */
	mov	dword ptr [ebp-01ch],0ffffffffh;	found

; 1845     DSKAREA	info = {0};
	mov	dword ptr [ebp-0f8h],0h;	info
	rep stosd	

; 1852 	rc = FindFreeSpace(disk, 0, disk->querySize(), &info, &found);
	mov	ecx,[ebx]

; 1845     DSKAREA	info = {0};
	mov	edi,edx

; 1852 	rc = FindFreeSpace(disk, 0, disk->querySize(), &info, &found);
	mov	eax,[ecx+014h]
	add	eax,ebx
	call	dword ptr [ecx+010h]
	lea	ecx,[ebp-01ch];	found
	add	esp,04h
	lea	esi,[ebp-0f8h];	info
	push	ecx
	mov	ecx,eax
	push	esi
	xor	edx,edx
	sub	esp,0ch
	mov	eax,ebx
	call	FindFreeSpace__FCP8VOS2DiskCUlT2CP8_DSKAREACPi
	add	esp,014h
	mov	ecx,edi
	mov	[ebp-018h],eax;	rc

; 1853 	if( rc != 0 )
	test	eax,eax
	jne	@BLBL409

; 1856 	if( found != -1 )
	cmp	dword ptr [ebp-01ch],0ffffffffh;	found
	je	@BLBL410

; 1858 	    Verbose(0, "DrvScan", "VRAID partition already there, stopping");
	xor	eax,eax
	sub	esp,0ch

; 1859 	    *dev = new VORDev(disk, info.area[found].start, info.area[found].size);
	mov	[ebp-028h],ecx;	@CBE112

; 1858 	    Verbose(0, "DrvScan", "VRAID partition already there, stopping");
	mov	ecx,offset FLAT:@CBE44
	mov	edx,offset FLAT:@CBE1
	call	Verbose
	add	esp,08h

; 1859 	    *dev = new VORDev(disk, info.area[found].start, info.area[found].size);
	mov	eax,030h
	call	__nw__FUi
	add	esp,04h
	mov	edx,ebx
	mov	[ebp-010h],eax;	__97
	test	eax,eax
	je	@BLBL412
	mov	ecx,[ebp-01ch];	found
	imul	ecx,014h
	add	ecx,esi
	mov	dword ptr [ebp-010ch],01h;	__es
	push	dword ptr [ecx+08h]
	mov	ecx,[ecx+04h]
	sub	esp,0ch
	call	__ct__6VORDevFP8VOS2DiskUlT2
	add	esp,010h
	mov	dword ptr [ebp-010ch],0h;	__es
@BLBL412:
	mov	eax,[ebp-028h];	@CBE112
	mov	ebx,[ebp-010h];	__97
	mov	[eax],ebx

; 1860 	    break;				/* rc still 0 */
	jmp	@BLBL409
@BLBL410:

; 1863 	if( info.cnt == 0 )
	cmp	dword ptr [ebp-0f8h],0h;	info
	jne	@BLBL413

; 1865 	    Verbose(1, "DrvScan", "No room on disk, aborting");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE45
	mov	edx,offset FLAT:@CBE1
	mov	eax,01h
	call	Verbose

; 1866 	    MyMessageBox(3, HWND_DESKTOP,
	mov	ecx,offset FLAT:@CBE46
	mov	edx,01h
	mov	eax,03h
	call	MyMessageBox__FCiCUlPce

; 1869 	    rc = ERROR_HANDLE_DISK_FULL;
	mov	dword ptr [ebp-018h],027h;	rc

; 1866 	    MyMessageBox(3, HWND_DESKTOP,
	add	esp,0ch

; 1870 	    break;
	jmp	@BLBL409
@BLBL413:

; 1874 	ul = WinDlgBox(HWND_DESKTOP, hwnd, AreaDlgProc,
	lea	ebx,[ebp-0f8h];	info
	push	ebx
	push	02d0h
	push	0h
	push	offset FLAT: AreaDlgProc
	push	dword ptr [ebp+08h];	hwnd
	push	01h
	call	WinDlgBox
	add	esp,018h

; 1876 	if( DID_OK != ul )
	cmp	eax,01h
	je	@BLBL414

; 1878 	    Verbose(3, "DrvScan", "CreatePhysdev: user aborted");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE47
	mov	edx,offset FLAT:@CBE1
	mov	eax,03h
	call	Verbose

; 1879 	    rc = ERROR_HANDLE_DISK_FULL;
	mov	dword ptr [ebp-018h],027h;	rc

; 1878 	    Verbose(3, "DrvScan", "CreatePhysdev: user aborted");
	add	esp,0ch

; 1880 	    break;
	jmp	@BLBL409
@BLBL414:

; 1883 	struct _DSKAREA_ENTRY * vpart = &info.area[info.cnt];
	mov	ebx,[ebp-0f8h];	info
	imul	ebx,014h
	lea	ebx,dword ptr [ebp+ebx-0f4h]
	mov	[ebp-020h],ebx;	vpart

; 1884 	vpart->type = VRAID_PARTTYPE;
	mov	byte ptr [ebx+08h],07ch

; 1885 	vpart->bootable = 0;
	mov	ecx,[ebp-020h];	vpart

; 1886 	if( vpart->ptsec == 0 )
	mov	edx,[ecx+0ch]

; 1885 	vpart->bootable = 0;
	mov	byte ptr [ecx+09h],0h

; 1886 	if( vpart->ptsec == 0 )
	test	edx,edx
	jne	@BLBL415

; 1887 	    rc = CreatePriPartition(disk, vpart->ptsec, vpart);
	mov	eax,[ebp+0ch];	disk
	sub	esp,0ch
	call	CreatePriPartition__FCP8VOS2DiskCUlCP14_DSKAREA_ENTRY
	add	esp,0ch
	jmp	@BLBL416
@BLBL415:

; 1891 	    rc = CreateLogPartition(disk, vpart->ptsec, vpart);
	sub	esp,0ch

; 1890 	    vpart->bootable = 0x80;
	mov	edx,[ebp-020h];	vpart

; 1891 	    rc = CreateLogPartition(disk, vpart->ptsec, vpart);
	mov	ecx,edx

; 1890 	    vpart->bootable = 0x80;
	mov	byte ptr [edx+09h],080h

; 1891 	    rc = CreateLogPartition(disk, vpart->ptsec, vpart);
	mov	edx,[edx+0ch]
	mov	eax,[ebp+0ch];	disk
	call	CreateLogPartition__FCP8VOS2DiskCUlCP14_DSKAREA_ENTRY
	add	esp,0ch

; 1892 	}
@BLBL416:

; 1893 	if( rc != 0 )
	mov	[ebp-018h],eax;	rc
	test	eax,eax
	jne	@BLBL409

; 1901 	PUCHAR	p = new UCHAR[2*SECTOR_SIZE];
	sub	esp,04h
	mov	eax,0400h
	call	__vn__FUi
	sub	esp,08h
	mov	ebx,eax

; 1902 	memset(p, 0, 2*SECTOR_SIZE);
	mov	ecx,0400h
	xor	edx,edx
	call	memset

; 1901 	PUCHAR	p = new UCHAR[2*SECTOR_SIZE];
	mov	[ebp-024h],ebx;	p

; 1902 	memset(p, 0, 2*SECTOR_SIZE);
	add	esp,0ch

; 1903 	rc = disk->write(vpart->start, 2, p);
	mov	eax,[ebp+0ch];	disk
	push	ebx
	mov	ebx,[eax]
	mov	edx,[ebp-020h];	vpart
	sub	esp,0ch
	mov	edi,[ebx+024h]
	mov	edx,[edx]
	mov	ecx,02h
	add	eax,edi
	call	dword ptr [ebx+020h]
	add	esp,010h
	mov	[ebp-018h],eax;	rc

; 1904 	if( rc != 0 )
	test	eax,eax
	je	@BLBL418

; 1905 	    Verbose(1,"DevScan","CreatePhysdev: can't clear SEC_PHYSDEV- rc %lu",rc);
	push	eax
	mov	ecx,offset FLAT:@CBE49
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE48
	mov	eax,01h
	call	Verbose
	add	esp,010h
@BLBL418:

; 1908 	*dev = new VORDev(disk, vpart->start, vpart->size);
	mov	ebx,[ebp+010h];	dev

; 1906 	delete[] p;
	sub	esp,04h
	mov	eax,[ebp-024h];	p
	call	__vd__FPv

; 1908 	*dev = new VORDev(disk, vpart->start, vpart->size);
	mov	eax,030h
	call	__nw__FUi
	mov	[ebp-02ch],ebx;	@CBE113
	add	esp,04h
	mov	[ebp-014h],eax;	__99
	test	eax,eax
	je	@BLBL420
	mov	ecx,[ebp-020h];	vpart
	mov	dword ptr [ebp-010ch],02h;	__es
	push	dword ptr [ecx+04h]
	mov	ecx,[ecx]
	sub	esp,0ch
	mov	edx,[ebp+0ch];	disk
	call	__ct__6VORDevFP8VOS2DiskUlT2
	add	esp,010h
	mov	dword ptr [ebp-010ch],0h;	__es
@BLBL420:

; 1909 	(*dev)->ioSync();
	sub	esp,04h

; 1908 	*dev = new VORDev(disk, vpart->start, vpart->size);
	mov	ebx,[ebp-02ch];	@CBE113
	mov	ecx,[ebp-014h];	__99
	mov	[ebx],ecx

; 1909 	(*dev)->ioSync();
	mov	eax,[ebp+010h];	dev
	mov	eax,[eax]
	mov	ebx,[eax]
	mov	ecx,[ebx+054h]
	add	eax,ecx
	call	dword ptr [ebx+050h]
	add	esp,04h

; 1911     while(0);
@BLBL409:

; 1913     return rc;
	pop	esi
	mov	eax,[ebp-018h];	rc
	pop	edi
	pop	ebx
	add	esp,0104h
	pop	fs:[0h]
	leave	
	ret	
CreatePhysdev__FUlCP8VOS2DiskPP6VORDev	endp

; 1804 CreatePriPartition(VOS2Disk * const disk,ULONG const ptsec,

CreatePriPartition__FCP8VOS2DiskCUlCP14_DSKAREA_ENTRY	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0fffffdc4h
	sub	esp,0230h

; 1806 {
	mov	dword ptr [ebp-023ch],0h;	__es

; 1804 CreatePriPartition(VOS2Disk * const disk,ULONG const ptsec,
	push	ebx

; 1806 {
	mov	dword ptr [ebp-0234h],offset FLAT:@1cb__fsm_tab;	__es

; 1804 CreatePriPartition(VOS2Disk * const disk,ULONG const ptsec,
	push	edi
	mov	edi,ecx
	push	esi
	mov	esi,eax

; 1808     Verbose(2,"CreatePriPartition","start %lu, size %lu",vpart->start,vpart->size);
	push	dword ptr [edi+04h]

; 1804 CreatePriPartition(VOS2Disk * const disk,ULONG const ptsec,
	mov	ebx,edx

; 1808     Verbose(2,"CreatePriPartition","start %lu, size %lu",vpart->start,vpart->size);
	push	dword ptr [edi]
	mov	eax,offset FLAT:@CBE43
	sub	esp,0ch
	mov	edx,eax
	mov	ecx,offset FLAT:@CBE40
	mov	eax,02h
	call	Verbose
	add	esp,014h
	mov	edx,offset FLAT:@CBE43

; 1809     Verbose(2,"CreatePriPartition","table %lu", ptsec);
	push	ebx
	mov	ecx,offset FLAT:@CBE41
	sub	esp,0ch
	mov	eax,02h
	call	Verbose
	mov	edx,ebx
	add	esp,010h
	mov	eax,esi

; 1811     PartTable	pt(disk, ptsec);
	push	0h
	mov	ecx,edx
	sub	esp,0ch
	mov	edx,eax
	lea	ebx,[ebp-0228h];	pt
	mov	eax,ebx
	call	__ct__9PartTableFP8VOS2DiskUl7Boolean
	add	esp,010h
	mov	edx,edi
	mov	dword ptr [ebp-023ch],01h;	__es

; 1812     pt.addOwn(vpart->type, vpart->bootable, vpart->start, vpart->size);
	push	dword ptr [edx+04h]
	mov	cl,[edx+09h]
	push	dword ptr [edx]
	mov	dl,[edx+08h]
	sub	esp,0ch
	mov	eax,ebx
	call	addOwn__9PartTableFUcT1UlT3
	add	esp,0ch
	mov	eax,ebx

; 1814     return 0;
	mov	dword ptr [ebp-023ch],0h;	__es
	mov	edx,02h
	call	__dt__9PartTableFv
	add	esp,08h
	xor	ebx,ebx
	pop	esi
	mov	eax,ebx
	pop	edi
	pop	ebx
	add	esp,0234h
	pop	fs:[0h]
	leave	
	ret	
CreatePriPartition__FCP8VOS2DiskCUlCP14_DSKAREA_ENTRY	endp

; 1658 CreateLogPartition(VOS2Disk * const disk,ULONG const ptsec,

CreateLogPartition__FCP8VOS2DiskCUlCP14_DSKAREA_ENTRY	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0fffff970h
	sub	esp,0684h

; 1660 {
	mov	dword ptr [ebp-0690h],0h;	__es

; 1658 CreateLogPartition(VOS2Disk * const disk,ULONG const ptsec,
	push	ebx
	mov	ebx,edx
	push	edi
	mov	edi,ecx
	push	esi
	mov	esi,eax
	mov	[ebp+010h],edi;	vpart
	mov	[ebp+0ch],ebx;	ptsec

; 1662     Verbose(2,"CreateLogPartition","start %lu, size %lu",vpart->start,vpart->size);
	mov	eax,offset FLAT:@CBE39

; 1658 CreateLogPartition(VOS2Disk * const disk,ULONG const ptsec,
	mov	[ebp+08h],esi;	disk

; 1660 {
	mov	dword ptr [ebp-0688h],offset FLAT:@1b8__fsm_tab;	__es

; 1662     Verbose(2,"CreateLogPartition","start %lu, size %lu",vpart->start,vpart->size);
	push	dword ptr [edi+04h]
	mov	edx,eax
	push	dword ptr [edi]
	mov	ecx,offset FLAT:@CBE40
	sub	esp,0ch
	mov	eax,02h
	call	Verbose
	add	esp,014h
	mov	edx,offset FLAT:@CBE39

; 1663     Verbose(2,"CreateLogPartition","table %lu", ptsec);
	mov	ecx,offset FLAT:@CBE41
	push	ebx
	sub	esp,0ch
	mov	eax,02h
	call	Verbose
	mov	edx,edi
	add	esp,010h
	mov	eax,esi

; 1665     PartTable	pt(disk, ptsec, vpart->extstart);
	push	dword ptr [edx+010h]

; 1671 	int		log_idx = -1;
	mov	dword ptr [ebp-066ch],0ffffffffh;	log_idx

; 1665     PartTable	pt(disk, ptsec, vpart->extstart);
	sub	esp,0ch
	mov	edx,eax

; 1663     Verbose(2,"CreateLogPartition","table %lu", ptsec);
	mov	ecx,ebx

; 1665     PartTable	pt(disk, ptsec, vpart->extstart);
	lea	eax,[ebp-0228h];	pt
	call	__ct__9PartTableFP8VOS2DiskUlT2

; 1672 	int		ext_idx = -1;
	mov	dword ptr [ebp-0664h],0ffffffffh;	ext_idx

; 1665     PartTable	pt(disk, ptsec, vpart->extstart);
	add	esp,010h

; 1673 	for( int i = 0; i < 4; ++i )
	mov	ebx,[ebp-010h];	pt

; 1665     PartTable	pt(disk, ptsec, vpart->extstart);
	mov	dword ptr [ebp-0690h],01h;	__es

; 1673 	for( int i = 0; i < 4; ++i )
	xor	esi,esi
	xor	edi,edi
@BLBL389:

; 1675 	    switch( pt.entry[i].SysIndicator )
	movzx	eax,byte ptr [ebx+esi+04h]
	test	eax,eax
	je	@BLBL491
	cmp	eax,05h
	jne	@BLBL493
@BLBL492:

; 1678 		ext_idx = i;
	mov	[ebp-0664h],edi;	ext_idx

; 1679 		break;
	jmp	@BLBL491
@BLBL493:

; 1682 		log_idx = i;
	mov	[ebp-066ch],edi;	log_idx

; 1686 		break;				/* nothing */
@BLBL491:

; 1673 	for( int i = 0; i < 4; ++i )
	inc	edi
	add	esi,010h
	cmp	edi,04h
	jl	@BLBL389

; 1690 	if( log_idx != -1 )
	mov	eax,[ebp-066ch];	log_idx
	cmp	eax,0ffffffffh
	je	@BLBL393

; 1692 	    PPARTITIONENTRY const le = &pt.entry[log_idx];
	mov	ecx,eax
	mov	edx,[ebp-010h];	pt
	sal	ecx,04h
	add	edx,ecx

; 1697 	    if( ptsec + le->RelativeSectors > vpart->start )
	mov	ebx,[ebp+010h];	vpart

; 1692 	    PPARTITIONENTRY const le = &pt.entry[log_idx];
	mov	[ebp-0668h],edx;	le

; 1697 	    if( ptsec + le->RelativeSectors > vpart->start )
	mov	ecx,[ebp+0ch];	ptsec
	mov	edi,[edx+08h]
	add	edi,ecx
	cmp	[ebx],edi
	jae	@BLBL394

; 1708 		if( le->BegHead != 1 )
	cmp	byte ptr [edx+01h],01h
	je	@BLBL395

; 1710 		    Verbose(1, "CreateLogPartition",
	push	ecx
	mov	ecx,offset FLAT:@CBE42
	push	eax
	mov	edx,offset FLAT:@CBE39
	sub	esp,0ch
	mov	eax,01h
	call	Verbose
	add	esp,0ch

; 1713 		    return (APIRET)-1;
	mov	dword ptr [ebp-0690h],0h;	__es
	mov	edx,02h
	lea	eax,[ebp-0228h];	pt
	call	__dt__9PartTableFv
	add	esp,08h
	or	esi,0ffffffffh
	mov	eax,esi
	pop	esi
	pop	edi
	pop	ebx
	add	esp,0688h
	pop	fs:[0h]
	leave	
	ret	
@BLBL395:

; 1716 		ULONG const	spt = disk->queryDPB().cSectorsPerTrack;
	mov	ebx,[ebp+08h];	disk
	sub	esp,04h
	mov	eax,ebx
	call	queryDPB__8VOS2DiskFv

; 1717 		PartTable	newpt(disk, ptsec + le->RelativeSectors - spt,
	mov	ecx,[ebp+010h];	vpart

; 1716 		ULONG const	spt = disk->queryDPB().cSectorsPerTrack;
	add	esp,04h
	movzx	esi,word ptr [eax+06h]
	mov	[ebp-0674h],esi;	spt

; 1717 		PartTable	newpt(disk, ptsec + le->RelativeSectors - spt,
	push	dword ptr [ecx+010h]

; 1716 		ULONG const	spt = disk->queryDPB().cSectorsPerTrack;
	mov	edx,ebx

; 1717 		PartTable	newpt(disk, ptsec + le->RelativeSectors - spt,
	sub	esp,0ch
	mov	edi,[ebp-0668h];	le
	lea	ebx,[ebp-0444h];	newpt
	mov	ecx,[edi+08h]
	add	ecx,[ebp+0ch];	ptsec
	sub	ecx,esi
	mov	eax,ebx
	call	__ct__9PartTableFP8VOS2DiskUlT2
	add	esp,0ch
	mov	dword ptr [ebp-0690h],02h;	__es

; 1719 		newpt.clear();			/* completely new! */
	mov	eax,ebx
	call	clear__9PartTableFv
	sub	esp,04h
	mov	eax,esi

; 1721 		le->RelativeSectors = spt;
	mov	[edi+08h],eax

; 1722 		newpt.addPE(*le);
	mov	edx,edi
	mov	eax,ebx
	call	addPE__9PartTableFRC15_PARTITIONENTRY

; 1723 		pt.delPE(le);
	lea	esi,[ebp-0228h];	pt

; 1722 		newpt.addPE(*le);
	mov	edx,edi

; 1723 		pt.delPE(le);
	mov	eax,esi
	call	delPE__9PartTableFP15_PARTITIONENTRY
	mov	eax,ebx
	add	esp,08h

; 1725 		if( ext_idx != -1 )
	mov	ecx,[ebp-0664h];	ext_idx
	cmp	ecx,0ffffffffh
	je	@BLBL396

; 1728 		    newpt.addPE(*ee);
	sub	esp,08h

; 1727 		    PPARTITIONENTRY const ee = &pt.entry[ext_idx];
	mov	edx,[ebp-010h];	pt
	sal	ecx,04h
	add	edx,ecx
	mov	[ebp-0678h],edx;	ee

; 1728 		    newpt.addPE(*ee);
	call	addPE__9PartTableFRC15_PARTITIONENTRY

; 1729 		    pt.delPE(ee);
	mov	edx,[ebp-0678h];	ee

; 1728 		    newpt.addPE(*ee);
	mov	eax,esi

; 1729 		    pt.delPE(ee);
	call	delPE__9PartTableFP15_PARTITIONENTRY
	add	esp,08h

; 1730 		}
@BLBL396:

; 1732 		pt.addForeign(EXTENDED_PARTTYPE, 0,
	mov	edx,[ebp-0668h];	le
	mov	ecx,[ebp-0674h];	spt
	add	ecx,[edx+0ch]
	push	ecx
	lea	esi,[ebp-0228h];	pt
	push	dword ptr [ebp-0440h];	newpt
	xor	ecx,ecx
	sub	esp,0ch
	mov	dl,05h
	mov	eax,esi
	call	addForeign__9PartTableFUcT1UlT3

; 1734 		pt.addOwn(VRAID_PARTTYPE, 0x80, vpart->start, vpart->size);
	mov	edi,[ebp+010h];	vpart

; 1732 		pt.addForeign(EXTENDED_PARTTYPE, 0,
	add	esp,014h
	mov	eax,esi

; 1734 		pt.addOwn(VRAID_PARTTYPE, 0x80, vpart->start, vpart->size);
	push	dword ptr [edi+04h]
	mov	cl,080h
	push	dword ptr [edi]
	mov	dl,07ch
	sub	esp,0ch
	call	addOwn__9PartTableFUcT1UlT3
	add	esp,0ch

; 1735 	    }
	mov	dword ptr [ebp-0690h],01h;	__es
	mov	edx,02h
	lea	eax,[ebp-0444h];	newpt
	call	__dt__9PartTableFv
	add	esp,08h
	jmp	@BLBL400
@BLBL394:

; 1746 		ULONG const spt = disk->queryDPB().cSectorsPerTrack;
	mov	ebx,[ebp+08h];	disk
	sub	esp,04h
	mov	eax,ebx
	call	queryDPB__8VOS2DiskFv

; 1748 		PartTable   newpt(disk, newpt_pos, vpart->extstart);
	mov	esi,[ebp+010h];	vpart

; 1746 		ULONG const spt = disk->queryDPB().cSectorsPerTrack;
	add	esp,04h
	movzx	edi,word ptr [eax+06h]

; 1748 		PartTable   newpt(disk, newpt_pos, vpart->extstart);
	push	dword ptr [esi+010h]
	mov	eax,[ebp-0668h];	le
	sub	esp,0ch
	mov	ecx,[eax+08h]
	add	ecx,[ebp+0ch];	ptsec
	add	ecx,[eax+0ch]

; 1746 		ULONG const spt = disk->queryDPB().cSectorsPerTrack;
	mov	edx,ebx

; 1748 		PartTable   newpt(disk, newpt_pos, vpart->extstart);
	lea	ebx,[ebp-0660h];	newpt
	mov	eax,ebx
	call	__ct__9PartTableFP8VOS2DiskUlT2
	add	esp,0ch
	mov	dword ptr [ebp-0690h],03h;	__es

; 1749 		newpt.clear();			/* completely new! */
	mov	eax,ebx
	call	clear__9PartTableFv

; 1752 		vpart->size -= spt;
	mov	ecx,[esi+04h]

; 1751 		vpart->start += spt;
	add	[esi],edi

; 1752 		vpart->size -= spt;
	sub	ecx,edi
	mov	[esi+04h],ecx

; 1749 		newpt.clear();			/* completely new! */
	add	esp,04h

; 1754 		ULONG	vpart_ext_size = vpart->size + spt;
	add	ecx,edi
	mov	[ebp-0670h],ecx;	vpart_ext_size

; 1755 		if( ext_idx != -1 )
	mov	ecx,[ebp-0664h];	ext_idx

; 1749 		newpt.clear();			/* completely new! */
	mov	eax,ebx

; 1755 		if( ext_idx != -1 )
	cmp	ecx,0ffffffffh
	je	@BLBL398

; 1761 		    newpt.addPE(*ee);
	sub	esp,08h

; 1757 		    PPARTITIONENTRY const ee = &pt.entry[ext_idx];
	mov	edx,[ebp-010h];	pt
	sal	ecx,04h
	add	edx,ecx
	mov	[ebp-067ch],edx;	ee

; 1758 		    vpart_ext_size
	mov	ecx,[esi+010h]
	add	ecx,[edx+08h]
	sub	ecx,[ebp-065ch];	newpt
	mov	[ebp-0670h],ecx;	vpart_ext_size

; 1761 		    newpt.addPE(*ee);
	call	addPE__9PartTableFRC15_PARTITIONENTRY

; 1762 		    pt.delPE(ee);
	mov	edx,[ebp-067ch];	ee
	lea	eax,[ebp-0228h];	pt
	call	delPE__9PartTableFP15_PARTITIONENTRY
	add	esp,08h

; 1763 		}
@BLBL398:

; 1765 		pt.addOwn(EXTENDED_PARTTYPE,0, newpt.queryPos(), vpart_ext_size);
	push	dword ptr [ebp-0670h];	vpart_ext_size
	xor	ecx,ecx
	push	dword ptr [ebp-065ch];	newpt
	mov	dl,05h
	sub	esp,0ch
	lea	eax,[ebp-0228h];	pt
	call	addOwn__9PartTableFUcT1UlT3

; 1766 		newpt.addOwn(VRAID_PARTTYPE, 0x80, vpart->start, vpart->size);
	mov	ebx,[ebp+010h];	vpart

; 1765 		pt.addOwn(EXTENDED_PARTTYPE,0, newpt.queryPos(), vpart_ext_size);
	add	esp,014h

; 1766 		newpt.addOwn(VRAID_PARTTYPE, 0x80, vpart->start, vpart->size);
	lea	esi,[ebp-0660h];	newpt
	push	dword ptr [ebx+04h]
	mov	cl,080h
	push	dword ptr [ebx]
	mov	dl,07ch
	sub	esp,0ch
	mov	eax,esi
	call	addOwn__9PartTableFUcT1UlT3
	add	esp,0ch
	mov	eax,esi

; 1767 	    }
	mov	dword ptr [ebp-0690h],01h;	__es
	mov	edx,02h
	call	__dt__9PartTableFv
	add	esp,08h

; 1768 	}
	jmp	@BLBL400
@BLBL393:

; 1774 	    pt.addOwn(VRAID_PARTTYPE, 0x80, vpart->start, vpart->size);
	mov	ebx,[ebp+010h];	vpart
	mov	cl,080h
	push	dword ptr [ebx+04h]
	mov	dl,07ch
	push	dword ptr [ebx]
	lea	eax,[ebp-0228h];	pt
	sub	esp,0ch
	call	addOwn__9PartTableFUcT1UlT3
	add	esp,014h

; 1776     }
@BLBL400:

; 1779     return 0;
	lea	eax,[ebp-0228h];	pt
	sub	esp,08h
	mov	dword ptr [ebp-0690h],0h;	__es
	mov	edx,02h
	call	__dt__9PartTableFv
	add	esp,08h
	xor	eax,eax
	pop	esi
	pop	edi
	pop	ebx
	add	esp,0688h
	pop	fs:[0h]
	leave	
	ret	
CreateLogPartition__FCP8VOS2DiskCUlCP14_DSKAREA_ENTRY	endp

; 1496 FindFreeSpace(VOS2Disk * const disk, ULONG const secno, ULONG const seccnt,

FindFreeSpace__FCP8VOS2DiskCUlT2CP8_DSKAREACPi	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0fffffda8h
	mov	[ebp+010h],ecx;	seccnt
	sub	esp,0254h

; 1498 {
	mov	dword ptr [ebp-0258h],0h;	__es

; 1496 FindFreeSpace(VOS2Disk * const disk, ULONG const secno, ULONG const seccnt,
	push	ebx
	mov	ebx,edx
	push	edi

; 1498 {
	mov	dword ptr [ebp-0250h],offset FLAT:@190__fsm_tab;	__es

; 1496 FindFreeSpace(VOS2Disk * const disk, ULONG const secno, ULONG const seccnt,
	push	esi
	mov	edi,eax

; 1502     Verbose(2, "DrvScan", "FindFreeSpace(%lu, %lu)", secno, seccnt);
	push	ecx
	mov	ecx,offset FLAT:@CBE38
	push	ebx
	mov	edx,offset FLAT:@CBE1
	sub	esp,0ch
	mov	eax,02h
	call	Verbose

; 1499     APIRET	rc = 0;
	mov	dword ptr [ebp-0230h],0h;	rc

; 1515 	int		free = -1;
	mov	dword ptr [ebp-0234h],0ffffffffh;	free

; 1502     Verbose(2, "DrvScan", "FindFreeSpace(%lu, %lu)", secno, seccnt);
	mov	eax,edi

; 1516 	for( used = i = 0; i < 4; ++i )
	mov	dword ptr [ebp-0238h],0h;	used

; 1496 FindFreeSpace(VOS2Disk * const disk, ULONG const secno, ULONG const seccnt,
	mov	[ebp+08h],edi;	disk

; 1502     Verbose(2, "DrvScan", "FindFreeSpace(%lu, %lu)", secno, seccnt);
	add	esp,014h

; 1516 	for( used = i = 0; i < 4; ++i )
	mov	[ebp+0ch],ebx;	secno

; 1506 	PartTable	pt(disk, secno, true);
	mov	ecx,ebx
	push	01h
	mov	edx,eax
	sub	esp,0ch
	lea	eax,[ebp-0228h];	pt
	call	__ct__9PartTableFP8VOS2DiskUl7Boolean
	mov	edx,ebx
	add	esp,010h

; 1516 	for( used = i = 0; i < 4; ++i )
	mov	ecx,[ebp+014h];	info

; 1506 	PartTable	pt(disk, secno, true);
	mov	dword ptr [ebp-0258h],01h;	__es

; 1516 	for( used = i = 0; i < 4; ++i )
	xor	ebx,ebx
	xor	esi,esi
	xor	edi,edi
@BLBL356:

; 1518 	    switch( pt.entry[i].SysIndicator )
	mov	eax,[ebp-010h];	pt
	movzx	eax,byte ptr [eax+esi+04h]
	test	eax,eax
	je	@BLBL495
	cmp	eax,05h
	je	@BLBL496
	cmp	eax,07ch
	je	@BLBL497
	jmp	@BLBL498
@BLBL495:

; 1521 		free = i;
	mov	[ebp-0234h],ebx;	free

; 1522 		break;
	jmp	@BLBL494
@BLBL496:

; 1526 		rc = FindFreeExtSpace(disk,
	push	dword ptr [ebp+018h];	found

; 1522 		break;
	mov	eax,edx

; 1526 		rc = FindFreeExtSpace(disk,
	push	ecx
	mov	edx,[ebp-010h];	pt
	add	edx,esi
	sub	esp,0ch
	mov	ecx,[edx+0ch]
	mov	edx,[edx+08h]
	add	edx,eax
	mov	eax,[ebp+08h];	disk
	call	FindFreeExtSpace__FCP8VOS2DiskCUlT2CP8_DSKAREACPi
	add	esp,014h
	mov	edx,[ebp+0ch];	secno
	mov	ecx,[ebp+014h];	info

; 1525 		++used;
	inc	edi

; 1526 		rc = FindFreeExtSpace(disk,
	mov	[ebp-0230h],eax;	rc

; 1529 		break;
	jmp	@BLBL494
@BLBL497:
	mov	eax,edx
	mov	edx,ecx

; 1532 		++used;
	inc	edi

; 1533 		info->area[info->cnt].start = secno + pt.entry[i].RelativeSectors;
	mov	ecx,[edx]
	mov	edx,[ebp-010h];	pt
	imul	ecx,014h
	mov	edx,dword ptr [edx+esi+08h]
	add	edx,eax
	mov	eax,[ebp+014h];	info
	mov	dword ptr [eax+ecx+04h],edx

; 1534 		info->area[info->cnt].size = pt.entry[i].NumSectors;
	mov	edx,[ebp-010h];	pt
	mov	ecx,[eax]
	imul	ecx,014h
	mov	edx,dword ptr [edx+esi+0ch]
	mov	dword ptr [eax+ecx+08h],edx

; 1535 		*found = info->cnt++;
	mov	ecx,[eax]
	lea	edx,[ecx+01h]
	mov	[eax],edx
	mov	eax,[ebp+018h];	found
	mov	edx,[ebp+0ch];	secno
	mov	[eax],ecx
	mov	ecx,[ebp+014h];	info

; 1536 		break;
	jmp	@BLBL494
@BLBL498:

; 1539 		++used;
	inc	edi

; 1540 		break;
@BLBL494:

; 1516 	for( used = i = 0; i < 4; ++i )
	inc	ebx
	add	esi,010h
	cmp	ebx,04h
	jb	@BLBL356
	mov	[ebp-0238h],edi;	used

; 1544 	if( free == -1 )
	cmp	dword ptr [ebp-0234h],0ffffffffh;	free
	jne	@BLBL360

; 1546 	    Verbose(1, "DrvScan", "FindFreeSpace(%lu): no partition entry available",
	push	dword ptr [ebp+0ch];	secno
	mov	ecx,offset FLAT:@CBE34
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE1
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 1548 	    break;				/* no error */
	jmp	@BLBL361
@BLBL360:

; 1550 	if( *found != -1 )
	mov	ecx,[ebp+018h];	found
	cmp	dword ptr [ecx],0ffffffffh
	jne	@BLBL361

; 1559 	if( used == 0 )
	cmp	dword ptr [ebp-0238h],0h;	used
	jne	@BLBL363

; 1564 	    if( secno == 0 )
	mov	esi,[ebp+014h];	info
	sub	esp,04h
	mov	ecx,[esi]
	imul	ecx,014h
	mov	edi,[ebp+08h];	disk
	add	esi,ecx
	mov	eax,edi
	call	queryDPB__8VOS2DiskFv
	add	esp,04h
	mov	cx,[eax+06h]
	cmp	dword ptr [ebp+0ch],0h;	secno
	mov	eax,esi
	jne	@BLBL364
	xchg	edi,eax

; 1565 		info->area[info->cnt].start
	sub	esp,04h
	movzx	esi,cx
	call	queryDPB__8VOS2DiskFv
	add	esp,04h
	movzx	ecx,word ptr [eax+04h]
	mov	eax,edi
	imul	ecx,esi
	mov	[eax+04h],ecx

; 1566 		    = disk->queryDPB().cHeads * disk->queryDPB().cSectorsPerTrack;
	jmp	@BLBL365
@BLBL364:

; 1568 		info->area[info->cnt].start
	movzx	ecx,cx
	add	ecx,[ebp+0ch];	secno
	mov	[eax+04h],ecx

; 1569 		    = secno + disk->queryDPB().cSectorsPerTrack;
@BLBL365:

; 1570 	    info->area[info->cnt].size = seccnt - info->area[info->cnt].start;
	mov	ebx,[ebp+014h];	info

; 1573 	    SayArea(2, "FindFreeSpace - found", &info->area[info->cnt]);
	sub	esp,0ch

; 1570 	    info->area[info->cnt].size = seccnt - info->area[info->cnt].start;
	mov	eax,[ebx]
	imul	eax,014h
	add	eax,ebx
	mov	ecx,[ebp+010h];	seccnt
	sub	ecx,[eax+04h]
	mov	[eax+08h],ecx

; 1571 	    info->area[info->cnt].ptsec = secno;
	mov	eax,[ebx]
	imul	eax,014h
	mov	ecx,[ebp+0ch];	secno
	mov	dword ptr [ebx+eax+010h],ecx

; 1572 	    info->area[info->cnt].extstart = 0;
	mov	eax,[ebx]
	imul	eax,014h
	mov	dword ptr [ebx+eax+014h],0h

; 1573 	    SayArea(2, "FindFreeSpace - found", &info->area[info->cnt]);
	mov	ecx,[ebx]
	imul	ecx,014h
	add	ecx,ebx
	add	ecx,04h
	mov	edx,offset FLAT:@CBE36
	mov	eax,02h
	call	SayArea__FUiCPcP14_DSKAREA_ENTRY

; 1574 	    ++info->cnt;
	inc	dword ptr [ebx]

; 1573 	    SayArea(2, "FindFreeSpace - found", &info->area[info->cnt]);
	add	esp,0ch

; 1575 	}
	jmp	@BLBL361
@BLBL363:

; 1578 	    ULONG const spt = disk->queryDPB().cSectorsPerTrack;
	mov	eax,[ebp+08h];	disk
	sub	esp,04h

; 1582 	    info->area[info->cnt].start = secno + spt;
	mov	ebx,[ebp+0ch];	secno

; 1578 	    ULONG const spt = disk->queryDPB().cSectorsPerTrack;
	call	queryDPB__8VOS2DiskFv

; 1582 	    info->area[info->cnt].start = secno + spt;
	mov	edx,[ebp+014h];	info

; 1578 	    ULONG const spt = disk->queryDPB().cSectorsPerTrack;
	movzx	ecx,word ptr [eax+06h]
	add	esp,04h
	mov	[ebp-0240h],ecx;	spt

; 1580 	    BOOL	inside = FALSE;
	mov	dword ptr [ebp-022ch],0h;	inside

; 1582 	    info->area[info->cnt].start = secno + spt;
	mov	edi,[edx]
	imul	edi,014h
	add	ecx,ebx
	mov	dword ptr [edx+edi+04h],ecx

; 1583 	    for( ul = secno + spt; ul < secno + seccnt; ++ul )
	add	ebx,[ebp+010h];	seccnt
	mov	[ebp-023ch],ecx;	ul
	cmp	ecx,ebx
	jae	@BLBL367
	mov	[ebp-0244h],ebx;	@CBE121
@BLBL368:

; 1585 		for( i = 0; i < 4; ++i )
	mov	edx,[ebp+0ch];	secno
	mov	esi,[ebp-022ch];	inside
	xor	ebx,ebx
	xor	edi,edi
@BLBL370:

; 1587 		    if( ul == secno + pt.entry[i].RelativeSectors )
	mov	eax,[ebp-010h];	pt
	mov	eax,dword ptr [eax+edi+08h]
	add	eax,edx
	cmp	eax,ecx
	jne	@BLBL371

; 1589 			if( !inside )
	test	esi,esi
	jne	@BLBL372

; 1591 			    info->area[info->cnt].size
	mov	eax,[ebp+014h];	info

; 1589 			if( !inside )
	mov	[ebp-0260h],ebx;	@CBE123

; 1591 			    info->area[info->cnt].size
	mov	ebx,[eax]
	imul	ebx,014h
	add	ebx,eax
	mov	edx,ecx
	sub	edx,[ebx+04h]
	mov	[ebx+08h],edx

; 1593 			    if( info->area[info->cnt].size >= MIN_VRAID_PART
	mov	ebx,[eax]
	imul	ebx,014h
	lea	edx,dword ptr [eax+ebx]
	mov	ebx,[ebp-0260h];	@CBE123
	mov	eax,[edx+08h]
	mov	[ebp-025ch],edx;	@CBE122
	mov	edx,[ebp+0ch];	secno
	cmp	eax,02800h
	jb	@BLBL373
	cmp	[ebp-0240h],eax;	spt
	mov	eax,[ebp+014h];	info
	jae	@BLBL373
	xchg	[ebp-025ch],ecx;	@CBE122

; 1598 				SayArea(2, "FindFreeSpace - found",
	sub	esp,0ch

; 1596 				info->area[info->cnt].ptsec = secno;
	mov	[ecx+010h],edx

; 1597 				info->area[info->cnt].extstart = 0;
	mov	ecx,[eax]
	imul	ecx,014h
	mov	dword ptr [eax+ecx+014h],0h

; 1598 				SayArea(2, "FindFreeSpace - found",
	mov	ecx,[eax]
	imul	ecx,014h
	add	ecx,eax
	add	ecx,04h
	mov	edx,offset FLAT:@CBE36
	mov	eax,02h
	call	SayArea__FUiCPcP14_DSKAREA_ENTRY
	mov	eax,[ebp+014h];	info
	mov	ecx,[ebp-025ch];	@CBE122
	mov	edx,[ebp+0ch];	secno

; 1600 				++info->cnt;
	inc	dword ptr [eax]

; 1598 				SayArea(2, "FindFreeSpace - found",
	add	esp,0ch

; 1601 			    }
@BLBL373:

; 1602 			    inside = TRUE;
	mov	esi,01h

; 1603 			}
@BLBL372:
	mov	eax,ecx

; 1604 			ul += pt.entry[i].NumSectors - 1;
	mov	ecx,[ebp-010h];	pt

; 1603 			}
	mov	[ebp-0260h],ebx;	@CBE123

; 1604 			ul += pt.entry[i].NumSectors - 1;
	mov	ecx,dword ptr [ecx+edi+0ch]
	dec	ecx
	add	ecx,eax

; 1605 			info->area[info->cnt].start = ul + 1;
	mov	eax,[ebp+014h];	info
	lea	ebx,[ecx+01h]
	mov	edx,[eax]
	imul	edx,014h
	mov	dword ptr [eax+edx+04h],ebx
	mov	ebx,[ebp-0260h];	@CBE123
	mov	edx,[ebp+0ch];	secno

; 1606 			break;
	jmp	@BLBL379
@BLBL371:
	mov	[ebp-0260h],ecx;	@CBE123

; 1608 		    else if( ul == (secno + pt.entry[i].RelativeSectors
	mov	ecx,[ebp-010h];	pt
	add	ecx,edi
	mov	eax,[ecx+08h]
	add	eax,edx
	add	eax,[ecx+0ch]
	mov	ecx,[ebp-0260h];	@CBE123
	cmp	eax,ecx
	jne	@BLBL377

; 1611 			inside = FALSE;
	xor	esi,esi

; 1613 		} /* end[for(every partition entry)] */
@BLBL377:

; 1585 		for( i = 0; i < 4; ++i )
	inc	ebx
	add	edi,010h
	cmp	ebx,04h
	jb	@BLBL370
@BLBL379:

; 1583 	    for( ul = secno + spt; ul < secno + seccnt; ++ul )
	inc	ecx
	cmp	[ebp-0244h],ecx;	@CBE121

; 1585 		for( i = 0; i < 4; ++i )
	mov	[ebp-022ch],esi;	inside

; 1583 	    for( ul = secno + spt; ul < secno + seccnt; ++ul )
	ja	@BLBL368
	mov	[ebp-023ch],ecx;	ul
@BLBL367:

; 1615 	    if( !inside )
	cmp	dword ptr [ebp-022ch],0h;	inside
	jne	@BLBL361

; 1617 		info->area[info->cnt].size = ul - info->area[info->cnt].start;
	mov	ebx,[ebp+014h];	info

; 1620 		SayArea(2, "FindFreeSpace - found", &info->area[info->cnt]);
	sub	esp,0ch

; 1617 		info->area[info->cnt].size = ul - info->area[info->cnt].start;
	mov	eax,[ebx]
	imul	eax,014h
	add	eax,ebx
	mov	ecx,[ebp-023ch];	ul
	sub	ecx,[eax+04h]
	mov	[eax+08h],ecx

; 1618 		info->area[info->cnt].ptsec = secno;
	mov	eax,[ebx]
	imul	eax,014h
	mov	ecx,[ebp+0ch];	secno
	mov	dword ptr [ebx+eax+010h],ecx

; 1619 		info->area[info->cnt].extstart = 0;
	mov	eax,[ebx]
	imul	eax,014h
	mov	dword ptr [ebx+eax+014h],0h

; 1620 		SayArea(2, "FindFreeSpace - found", &info->area[info->cnt]);
	mov	ecx,[ebx]
	imul	ecx,014h
	add	ecx,ebx
	add	ecx,04h
	mov	edx,offset FLAT:@CBE36
	mov	eax,02h
	call	SayArea__FUiCPcP14_DSKAREA_ENTRY
	add	esp,0ch

; 1621 		++info->cnt;
	inc	dword ptr [ebx]

; 1625     while( 0 );
@BLBL361:
	lea	eax,[ebp-0228h];	pt
	sub	esp,08h
	mov	dword ptr [ebp-0258h],0h;	__es
	mov	edx,02h
	call	__dt__9PartTableFv
	add	esp,08h

; 1627     Verbose(3, "DrvScan", "FindFreeSpace - rc %lu", rc);
	mov	esi,[ebp-0230h];	rc
	push	esi
	mov	ecx,offset FLAT:@CBE37
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE1
	mov	eax,03h
	call	Verbose
	add	esp,010h
	mov	eax,esi

; 1628     return rc;
	pop	esi
	pop	edi
	pop	ebx
	add	esp,0258h
	pop	fs:[0h]
	leave	
	ret	
FindFreeSpace__FCP8VOS2DiskCUlT2CP8_DSKAREACPi	endp

; 1334 FindFreeExtSpace(VOS2Disk * const disk,

FindFreeExtSpace__FCP8VOS2DiskCUlT2CP8_DSKAREACPi	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0fffffce0h
	mov	[ebp+010h],ecx;	seccnt
	sub	esp,031ch

; 1337 {
	mov	dword ptr [ebp-0320h],0h;	__es

; 1334 FindFreeExtSpace(VOS2Disk * const disk,
	push	ebx
	mov	ebx,edx
	push	edi

; 1337 {
	mov	dword ptr [ebp-0318h],offset FLAT:@174__fsm_tab;	__es

; 1334 FindFreeExtSpace(VOS2Disk * const disk,
	push	esi
	mov	edi,eax

; 1342     Verbose(2, "DrvScan", "FindFreeExtSpace(%lu, %lu)", secno, seccnt);
	push	ecx
	mov	ecx,offset FLAT:@CBE32
	push	ebx
	mov	edx,offset FLAT:@CBE1
	sub	esp,0ch
	mov	eax,02h
	call	Verbose

; 1338     APIRET	rc = 0;
	mov	dword ptr [ebp-022ch],0h;	rc

; 1356 	int		free = -1;
	mov	dword ptr [ebp-0230h],0ffffffffh;	free

; 1342     Verbose(2, "DrvScan", "FindFreeExtSpace(%lu, %lu)", secno, seccnt);
	add	esp,014h

; 1334 FindFreeExtSpace(VOS2Disk * const disk,
	mov	[ebp+08h],edi;	disk

; 1357 	for( used = i = 0; i < 4; ++i )
	mov	[ebp+0ch],ebx;	secno

; 1342     Verbose(2, "DrvScan", "FindFreeExtSpace(%lu, %lu)", secno, seccnt);
	mov	eax,edi

; 1343     allocated.cnt = 0;
	mov	dword ptr [ebp-030ch],0h;	allocated

; 1347 	PartTable	pt(disk, secno, true);
	push	01h
	mov	edx,eax
	sub	esp,0ch
	mov	ecx,ebx
	lea	eax,[ebp-0228h];	pt
	call	__ct__9PartTableFP8VOS2DiskUl7Boolean
	mov	edx,ebx
	add	esp,010h
	mov	dword ptr [ebp-0320h],01h;	__es

; 1357 	for( used = i = 0; i < 4; ++i )
	mov	ecx,edx
	xor	ebx,ebx
	xor	esi,esi
	xor	edi,edi
@BLBL326:

; 1359 	    switch( pt.entry[i].SysIndicator )
	mov	eax,[ebp-010h];	pt
	movzx	eax,byte ptr [eax+edi+04h]
	test	eax,eax
	je	@BLBL501
	cmp	eax,05h
	je	@BLBL502
	cmp	eax,07ch
	je	@BLBL503
	jmp	@BLBL504
@BLBL501:

; 1362 		free = i;
	mov	[ebp-0230h],ebx;	free

; 1363 		break;
	jmp	@BLBL500
@BLBL502:

; 1367 		rc = ScanExtAllocated(disk,
	mov	edx,[ebp+018h];	found
	push	edx
	lea	edx,[ebp-030ch];	allocated
	push	edx

; 1363 		break;
	mov	eax,ecx

; 1367 		rc = ScanExtAllocated(disk,
	push	eax
	mov	edx,[ebp-010h];	pt
	add	edx,edi
	sub	esp,0ch
	mov	ecx,[edx+0ch]
	mov	edx,[edx+08h]
	add	edx,eax
	mov	eax,[ebp+08h];	disk
	call	ScanExtAllocated__FCP8VOS2DiskCUlN22CP8_DSKAREACPi
	mov	edx,eax
	add	esp,018h

; 1366 		++used;
	inc	esi

; 1367 		rc = ScanExtAllocated(disk,
	mov	ecx,[ebp+0ch];	secno
	mov	eax,[ebp+018h];	found
	mov	[ebp-022ch],edx;	rc

; 1371 		if( rc == 0  &&  *found != -1 )
	test	edx,edx
	jne	@BLBL327
	mov	edx,[eax]
	cmp	edx,0ffffffffh
	je	@BLBL327

; 1378 		    memcpy(&freelist->area[freelist->cnt], &allocated.area[*found],
	mov	ecx,[ebp+014h];	freelist
	sub	esp,0ch
	mov	eax,[ecx]
	imul	eax,014h
	imul	edx,014h
	add	eax,ecx
	lea	edx,dword ptr [ebp+edx-0308h]
	mov	ecx,014h
	add	eax,04h
	call	memcpy
	mov	ecx,[ebp+014h];	freelist
	add	esp,0ch

; 1380 		    *found = freelist->cnt++;
	mov	eax,[ecx]
	lea	edx,[eax+01h]
	mov	[ecx],edx
	mov	edx,[ebp+018h];	found
	mov	ecx,[ebp+0ch];	secno
	mov	[edx],eax

; 1381 		}
@BLBL327:

; 1382 		break;
	jmp	@BLBL500
@BLBL503:
	mov	eax,ecx

; 1386 		freelist->area[freelist->cnt].start
	mov	ecx,[ebp+014h];	freelist

; 1385 		++used;
	inc	esi

; 1386 		freelist->area[freelist->cnt].start
	mov	edx,[ecx]
	mov	ecx,[ebp-010h];	pt
	imul	edx,014h
	mov	ecx,dword ptr [ecx+edi+08h]
	add	ecx,eax
	mov	eax,[ebp+014h];	freelist
	mov	dword ptr [eax+edx+04h],ecx

; 1388 		freelist->area[freelist->cnt].size = pt.entry[i].NumSectors;
	mov	ecx,[ebp-010h];	pt
	mov	edx,[eax]
	imul	edx,014h
	mov	ecx,dword ptr [ecx+edi+0ch]
	mov	dword ptr [eax+edx+08h],ecx

; 1389 		*found = freelist->cnt++;
	mov	edx,[eax]
	lea	ecx,[edx+01h]
	mov	[eax],ecx
	mov	eax,[ebp+018h];	found
	mov	ecx,[ebp+0ch];	secno
	mov	[eax],edx

; 1390 		break;
	jmp	@BLBL500
@BLBL504:
	mov	eax,ecx

; 1399 		SayArea(2, "FindFreeExtSpace - found used",
	sub	esp,0ch

; 1394 		allocated.area[allocated.cnt].start
	mov	edx,[ebp-010h];	pt
	add	edx,edi

; 1393 		++used;
	inc	esi

; 1394 		allocated.area[allocated.cnt].start
	mov	ecx,[edx+08h]
	add	ecx,eax
	mov	eax,[ebp-030ch];	allocated
	imul	eax,014h
	mov	dword ptr [ebp+eax-0308h],ecx

; 1396 		allocated.area[allocated.cnt].size = pt.entry[i].NumSectors;
	mov	eax,[ebp-030ch];	allocated
	imul	eax,014h
	mov	edx,[edx+0ch]
	mov	dword ptr [ebp+eax-0304h],edx

; 1397 		allocated.area[allocated.cnt].ptsec = secno;
	mov	eax,[ebp-030ch];	allocated
	imul	eax,014h

; 1394 		allocated.area[allocated.cnt].start
	mov	ecx,[ebp+0ch];	secno

; 1397 		allocated.area[allocated.cnt].ptsec = secno;
	mov	dword ptr [ebp+eax-02fch],ecx

; 1398 		allocated.area[allocated.cnt].extstart = secno;
	mov	eax,[ebp-030ch];	allocated
	imul	eax,014h
	mov	dword ptr [ebp+eax-02f8h],ecx

; 1399 		SayArea(2, "FindFreeExtSpace - found used",
	mov	eax,[ebp-030ch];	allocated
	imul	eax,014h
	lea	ecx,dword ptr [ebp+eax-0308h]
	mov	edx,offset FLAT:@CBE33
	mov	eax,02h
	call	SayArea__FUiCPcP14_DSKAREA_ENTRY

; 1401 		++allocated.cnt;
	inc	dword ptr [ebp-030ch];	allocated

; 1399 		SayArea(2, "FindFreeExtSpace - found used",
	mov	ecx,[ebp+0ch];	secno
	add	esp,0ch

; 1402 		break;
@BLBL500:

; 1357 	for( used = i = 0; i < 4; ++i )
	inc	ebx
	add	edi,010h
	cmp	ebx,04h
	jb	@BLBL326

; 1406 	if( free == -1 )
	cmp	dword ptr [ebp-0230h],0ffffffffh;	free
	jne	@BLBL331

; 1408 	    Verbose(1, "DrvScan", "FindFreeSpace(%lu): no partition entry available",
	push	dword ptr [ebp+0ch];	secno
	mov	ecx,offset FLAT:@CBE34
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE1
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 1410 	    break;				/* no error */
	jmp	@BLBL332
@BLBL331:

; 1412 	if( *found != -1 )
	mov	ebx,[ebp+018h];	found
	cmp	dword ptr [ebx],0ffffffffh
	jne	@BLBL332

; 1421 	ULONG const spt = disk->queryDPB().cSectorsPerTrack;
	mov	eax,[ebp+08h];	disk
	sub	esp,04h

; 1425 	freelist->area[freelist->cnt].start = secno + spt;
	mov	ebx,[ebp+0ch];	secno

; 1421 	ULONG const spt = disk->queryDPB().cSectorsPerTrack;
	call	queryDPB__8VOS2DiskFv

; 1425 	freelist->area[freelist->cnt].start = secno + spt;
	mov	edx,[ebp+014h];	freelist

; 1421 	ULONG const spt = disk->queryDPB().cSectorsPerTrack;
	movzx	ecx,word ptr [eax+06h]
	add	esp,04h
	mov	[ebp-0238h],ecx;	spt

; 1423 	Boolean	inside = false;
	mov	byte ptr [ebp-023dh],0h;	inside

; 1425 	freelist->area[freelist->cnt].start = secno + spt;
	mov	edi,[edx]
	imul	edi,014h
	add	ecx,ebx
	mov	dword ptr [edx+edi+04h],ecx

; 1426 	for( ul = secno + spt; ul < secno + seccnt; ++ul )
	add	ebx,[ebp+010h];	seccnt
	mov	[ebp-0234h],ecx;	ul
	cmp	ecx,ebx
	jae	@BLBL334
	mov	[ebp-023ch],ebx;	@CBE126
@BLBL335:

; 1428 	    for( i = 0; i < allocated.cnt; ++i )
	cmp	dword ptr [ebp-030ch],0h;	allocated
	jbe	@BLBL346
	mov	edx,[ebp+0ch];	secno
	mov	bl,[ebp-023dh];	inside
	xor	esi,esi
	xor	edi,edi
@BLBL337:

; 1430 		if( ul == allocated.area[i].start )
	cmp	dword ptr [ebp+edi-0308h],ecx
	jne	@BLBL338

; 1432 		    if( !inside )
	test	bl,bl
	jne	@BLBL339

; 1434 			freelist->area[freelist->cnt].size
	mov	eax,[ebp+014h];	freelist

; 1432 		    if( !inside )
	mov	[ebp-0328h],ebx;	@CBE128

; 1434 			freelist->area[freelist->cnt].size
	mov	ebx,[eax]
	imul	ebx,014h
	add	ebx,eax
	mov	edx,ecx
	sub	edx,[ebx+04h]
	mov	[ebx+08h],edx

; 1436 			if( freelist->area[freelist->cnt].size >= MIN_VRAID_PART
	mov	ebx,[eax]
	imul	ebx,014h
	lea	edx,dword ptr [eax+ebx]
	mov	ebx,[ebp-0328h];	@CBE128
	mov	eax,[edx+08h]
	mov	[ebp-0324h],edx;	@CBE127
	mov	edx,[ebp+0ch];	secno
	cmp	eax,02800h
	jb	@BLBL340
	cmp	[ebp-0238h],eax;	spt
	mov	eax,[ebp+014h];	freelist
	jae	@BLBL340
	xchg	[ebp-0324h],ecx;	@CBE127

; 1441 			    SayArea(2, "FindFreeSpace - found free",
	sub	esp,0ch

; 1439 			    freelist->area[freelist->cnt].ptsec = secno;
	mov	[ecx+010h],edx

; 1440 			    freelist->area[freelist->cnt].extstart = secno;
	mov	ecx,[eax]
	imul	ecx,014h
	mov	dword ptr [eax+ecx+014h],edx

; 1441 			    SayArea(2, "FindFreeSpace - found free",
	mov	ecx,[eax]
	imul	ecx,014h
	add	ecx,eax
	add	ecx,04h
	mov	edx,offset FLAT:@CBE35
	mov	eax,02h
	call	SayArea__FUiCPcP14_DSKAREA_ENTRY
	mov	eax,[ebp+014h];	freelist
	mov	ecx,[ebp-0324h];	@CBE127
	mov	edx,[ebp+0ch];	secno

; 1443 			    ++freelist->cnt;
	inc	dword ptr [eax]

; 1441 			    SayArea(2, "FindFreeSpace - found free",
	add	esp,0ch

; 1444 			}
@BLBL340:

; 1445 			inside = true;
	mov	bl,01h

; 1446 		    }
@BLBL339:
	mov	eax,ecx

; 1447 		    ul += allocated.area[i].size - 1;
	mov	ecx,dword ptr [ebp+edi-0304h]
	dec	ecx
	add	ecx,eax

; 1448 		    freelist->area[freelist->cnt].start = ul + 1;
	mov	eax,[ebp+014h];	freelist

; 1446 		    }
	mov	[ebp-0328h],ebx;	@CBE128

; 1448 		    freelist->area[freelist->cnt].start = ul + 1;
	mov	edx,[eax]
	imul	edx,014h
	lea	ebx,[ecx+01h]
	mov	dword ptr [eax+edx+04h],ebx
	mov	ebx,[ebp-0328h];	@CBE128
	mov	edx,[ebp+0ch];	secno

; 1449 		    break;
	jmp	@BLBL499
@BLBL338:

; 1451 		else if( ul == allocated.area[i].start + allocated.area[i].size )
	lea	edx,dword ptr [ebp+edi-030ch]
	mov	eax,[edx+08h];	allocated
	add	eax,[edx+04h]
	mov	edx,[ebp+0ch];	secno
	cmp	eax,ecx
	jne	@BLBL344

; 1453 		    inside = false;
	xor	ebx,ebx

; 1455 	    } /* end[for(every partition entry)] */
@BLBL344:

; 1428 	    for( i = 0; i < allocated.cnt; ++i )
	inc	esi
	add	edi,014h
	cmp	[ebp-030ch],esi;	allocated
	ja	@BLBL337
@BLBL499:
	mov	[ebp-023dh],bl;	inside
@BLBL346:

; 1426 	for( ul = secno + spt; ul < secno + seccnt; ++ul )
	inc	ecx
	cmp	[ebp-023ch],ecx;	@CBE126
	ja	@BLBL335
	mov	[ebp-0234h],ecx;	ul
@BLBL334:

; 1457 	if( !inside )
	cmp	byte ptr [ebp-023dh],0h;	inside
	jne	@BLBL332

; 1459 	    freelist->area[freelist->cnt].size
	mov	ebx,[ebp+014h];	freelist

; 1463 	    SayArea(2, "FindFreeSpace - found", &freelist->area[freelist->cnt]);
	sub	esp,0ch

; 1459 	    freelist->area[freelist->cnt].size
	mov	eax,[ebx]
	imul	eax,014h
	add	eax,ebx
	mov	ecx,[ebp-0234h];	ul
	sub	ecx,[eax+04h]
	mov	[eax+08h],ecx

; 1461 	    freelist->area[freelist->cnt].ptsec = secno;
	mov	eax,[ebx]
	imul	eax,014h
	mov	ecx,[ebp+0ch];	secno
	mov	dword ptr [ebx+eax+010h],ecx

; 1462 	    freelist->area[freelist->cnt].extstart = secno;
	mov	eax,[ebx]
	imul	eax,014h
	mov	dword ptr [ebx+eax+014h],ecx

; 1463 	    SayArea(2, "FindFreeSpace - found", &freelist->area[freelist->cnt]);
	mov	ecx,[ebx]
	imul	ecx,014h
	add	ecx,ebx
	add	ecx,04h
	mov	edx,offset FLAT:@CBE36
	mov	eax,02h
	call	SayArea__FUiCPcP14_DSKAREA_ENTRY
	add	esp,0ch

; 1464 	    ++freelist->cnt;
	inc	dword ptr [ebx]

; 1467     while( 0 );
@BLBL332:
	lea	eax,[ebp-0228h];	pt
	sub	esp,08h
	mov	dword ptr [ebp-0320h],0h;	__es
	mov	edx,02h
	call	__dt__9PartTableFv
	add	esp,08h

; 1469     Verbose(3, "DrvScan", "FindFreeSpace - rc %lu", rc);
	mov	esi,[ebp-022ch];	rc
	push	esi
	mov	ecx,offset FLAT:@CBE37
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE1
	mov	eax,03h
	call	Verbose
	add	esp,010h
	mov	eax,esi

; 1470     return rc;
	pop	esi
	pop	edi
	pop	ebx
	add	esp,0320h
	pop	fs:[0h]
	leave	
	ret	
FindFreeExtSpace__FCP8VOS2DiskCUlT2CP8_DSKAREACPi	endp

; 1260 ScanExtAllocated(VOS2Disk * const disk,

ScanExtAllocated__FCP8VOS2DiskCUlN22CP8_DSKAREACPi	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0fffffdc0h
	sub	esp,0234h

; 1264 {
	mov	dword ptr [ebp-0240h],0h;	__es

; 1260 ScanExtAllocated(VOS2Disk * const disk,
	push	ebx
	mov	ebx,edx
	push	edi

; 1264 {
	mov	dword ptr [ebp-0238h],offset FLAT:@14f__fsm_tab;	__es

; 1260 ScanExtAllocated(VOS2Disk * const disk,
	push	esi

; 1265     APIRET	rc = 0;
	mov	dword ptr [ebp-022ch],0h;	rc

; 1267     Verbose(2, "DrvScan", "ScanExtAllocated(%lu, %lu)", secno, seccnt);
	push	ecx

; 1260 ScanExtAllocated(VOS2Disk * const disk,
	mov	esi,eax

; 1267     Verbose(2, "DrvScan", "ScanExtAllocated(%lu, %lu)", secno, seccnt);
	push	ebx
	mov	ecx,offset FLAT:@CBE30
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE1
	mov	eax,02h
	call	Verbose
	add	esp,014h
	mov	eax,esi

; 1269     PartTable	pt(disk, secno, true);
	push	01h

; 1260 ScanExtAllocated(VOS2Disk * const disk,
	mov	[ebp+08h],esi;	disk

; 1276     for( unsigned i = 0; i < 4; ++i )
	mov	[ebp+0ch],ebx;	secno

; 1269     PartTable	pt(disk, secno, true);
	mov	ecx,ebx
	mov	edx,eax
	sub	esp,0ch
	lea	eax,[ebp-0228h];	pt
	call	__ct__9PartTableFP8VOS2DiskUl7Boolean
	mov	ecx,ebx
	add	esp,010h

; 1276     for( unsigned i = 0; i < 4; ++i )
	mov	edx,[ebp+014h];	extstart
	mov	esi,[ebp+018h];	usedspace

; 1269     PartTable	pt(disk, secno, true);
	mov	dword ptr [ebp-0240h],01h;	__es

; 1276     for( unsigned i = 0; i < 4; ++i )
	xor	ebx,ebx
	xor	edi,edi
@BLBL318:

; 1278 	switch( pt.entry[i].SysIndicator )
	mov	eax,[ebp-010h];	pt
	movzx	eax,byte ptr [eax+edi+04h]
	test	eax,eax
	je	@BLBL505
	cmp	eax,05h
	je	@BLBL506
	cmp	eax,07ch
	je	@BLBL507
	jmp	@BLBL508
@BLBL506:

; 1281 	    rc = ScanExtAllocated(disk, extstart + pt.entry[i].RelativeSectors,
	push	dword ptr [ebp+01ch];	found

; 1278 	switch( pt.entry[i].SysIndicator )
	mov	eax,edx

; 1281 	    rc = ScanExtAllocated(disk, extstart + pt.entry[i].RelativeSectors,
	push	esi
	mov	edx,[ebp-010h];	pt
	push	eax
	add	edx,edi
	sub	esp,0ch
	mov	ecx,[edx+0ch]
	mov	edx,[edx+08h]
	add	edx,eax
	mov	eax,[ebp+08h];	disk
	call	ScanExtAllocated__FCP8VOS2DiskCUlN22CP8_DSKAREACPi
	add	esp,018h
	mov	edx,[ebp+014h];	extstart
	mov	ecx,[ebp+0ch];	secno
	mov	[ebp-022ch],eax;	rc

; 1284 	    break;
	jmp	@BLBL505
@BLBL507:

; 1287 	    usedspace->area[usedspace->cnt].start
	mov	eax,[esi]

; 1284 	    break;
	mov	edx,ecx

; 1287 	    usedspace->area[usedspace->cnt].start
	mov	ecx,[ebp-010h];	pt
	imul	eax,014h
	mov	ecx,dword ptr [ecx+edi+08h]
	add	ecx,edx
	mov	edx,[ebp+014h];	extstart
	mov	dword ptr [esi+eax+04h],ecx

; 1289 	    usedspace->area[usedspace->cnt].size = pt.entry[i].NumSectors;
	mov	ecx,[ebp-010h];	pt
	mov	eax,[esi]
	imul	eax,014h
	mov	ecx,dword ptr [ecx+edi+0ch]
	mov	dword ptr [esi+eax+08h],ecx

; 1290 	    *found = usedspace->cnt++;
	mov	ecx,[esi]
	lea	eax,[ecx+01h]
	mov	[esi],eax
	mov	eax,[ebp+01ch];	found
	mov	[eax],ecx
	mov	ecx,[ebp+0ch];	secno

; 1291 	    break;
	jmp	@BLBL505
@BLBL508:

; 1294 	    usedspace->area[usedspace->cnt].start
	mov	eax,[esi]

; 1291 	    break;
	mov	edx,ecx

; 1299 	    SayArea(2, "ScanExtAllocated - foun
; 1299 d", &usedspace->area[usedspace->cnt]);
	sub	esp,0ch

; 1294 	    usedspace->area[usedspace->cnt].start
	mov	ecx,[ebp-010h];	pt
	imul	eax,014h
	mov	ecx,dword ptr [ecx+edi+08h]
	add	ecx,edx
	mov	edx,[ebp+014h];	extstart
	mov	dword ptr [esi+eax+04h],ecx

; 1296 	    usedspace->area[usedspace->cnt].size = pt.entry[i].NumSectors;
	mov	ecx,[ebp-010h];	pt
	mov	eax,[esi]
	imul	eax,014h
	mov	ecx,dword ptr [ecx+edi+0ch]
	mov	dword ptr [esi+eax+08h],ecx

; 1297 	    usedspace->area[usedspace->cnt].ptsec = secno;
	mov	eax,[esi]
	imul	eax,014h

; 1296 	    usedspace->area[usedspace->cnt].size = pt.entry[i].NumSectors;
	mov	ecx,[ebp+0ch];	secno

; 1297 	    usedspace->area[usedspace->cnt].ptsec = secno;
	mov	dword ptr [esi+eax+010h],ecx

; 1298 	    usedspace->area[usedspace->cnt].extstart = extstart;
	mov	eax,[esi]
	imul	eax,014h
	mov	dword ptr [esi+eax+014h],edx

; 1299 	    SayArea(2, "ScanExtAllocated - foun
; 1299 d", &usedspace->area[usedspace->cnt]);
	mov	ecx,[esi]
	imul	ecx,014h
	add	ecx,esi
	add	ecx,04h
	mov	edx,offset FLAT:@CBE31
	mov	eax,02h
	call	SayArea__FUiCPcP14_DSKAREA_ENTRY
	mov	edx,[ebp+014h];	extstart
	mov	ecx,[ebp+0ch];	secno

; 1300 	    usedspace->cnt++;
	inc	dword ptr [esi]

; 1299 	    SayArea(2, "ScanExtAllocated - foun
; 1299 d", &usedspace->area[usedspace->cnt]);
	add	esp,0ch

; 1304 	    break;
@BLBL505:

; 1276     for( unsigned i = 0; i < 4; ++i )
	inc	ebx
	add	edi,010h
	cmp	ebx,04h
	jb	@BLBL318

; 1308     return rc;
	lea	eax,[ebp-0228h];	pt
	sub	esp,08h
	mov	dword ptr [ebp-0240h],0h;	__es
	mov	edx,02h
	call	__dt__9PartTableFv
	mov	esi,[ebp-022ch];	rc
	add	esp,08h
	mov	eax,esi
	pop	esi
	pop	edi
	pop	ebx
	add	esp,0238h
	pop	fs:[0h]
	leave	
	ret	
ScanExtAllocated__FCP8VOS2DiskCUlN22CP8_DSKAREACPi	endp

; 1230 SayArea(unsigned level,PSZ const msgstart,struct _DSKAREA_ENTRY * area)

SayArea__FUiCPcP14_DSKAREA_ENTRY	proc
	push	ebx
	mov	ebx,edx
	push	edi
	mov	edi,ecx

; 1232     Verbose(level, "DrvScan", "%s area (start %lu, size %lu), PT %lu, EXT %lu",
	mov	ecx,offset FLAT:@CBE29
	push	dword ptr [edi+010h]
	mov	edx,offset FLAT:@CBE1
	push	dword ptr [edi+0ch]
	push	dword ptr [edi+04h]
	push	dword ptr [edi]
	push	ebx
	sub	esp,0ch
	call	Verbose
	add	esp,020h

; 1234 }
	pop	edi
	pop	ebx
	ret	
SayArea__FUiCPcP14_DSKAREA_ENTRY	endp

; 1114 BuildDevTree(PWORK const work)

BuildDevTree__FCP5_WORK	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffc4h
	mov	[ebp+08h],eax;	work
	sub	esp,034h

; 1115 {
	mov	dword ptr [ebp-03ch],0h;	__es

; 1114 BuildDevTree(PWORK const work)
	push	ebx

; 1115 {
	mov	dword ptr [ebp-034h],offset FLAT:@128__fsm_tab;	__es

; 1114 BuildDevTree(PWORK const work)
	push	edi

; 1123     for( i = 0; i < work->cOs2Dsk; ++i )
	cmp	dword ptr [eax+0ch],0h

; 1114 BuildDevTree(PWORK const work)
	push	esi

; 1123     for( i = 0; i < work->cOs2Dsk; ++i )
	jbe	@BLBL277
	mov	ebx,[ebp-014h];	__88
	mov	[ebp+08h],eax;	work
	mov	esi,eax
	xor	edi,edi
@BLBL278:

; 1125 	VOS2Disk * const dsk = work->apOs2Dsk[i];
	mov	edx,[esi+08h]
	mov	edx,dword ptr [edx+edi*04h]
	mov	[ebp-028h],edx;	dsk

; 1126 	ULONG		partstart, partsize = 0;
	mov	dword ptr [ebp-018h],0h;	partsize

; 1128 	if( dsk == NULL )
	test	edx,edx
	je	@BLBL280

; 1131 	rc = FindVRaidPartition(dsk, 0, 0, &partstart, &partsize);
	lea	eax,[ebp-018h];	partsize
	push	eax
	lea	eax,[ebp-024h];	partstart
	push	eax

; 1128 	if( dsk == NULL )
	mov	esi,edx

; 1131 	rc = FindVRaidPartition(dsk, 0, 0, &partstart, &partsize);
	sub	esp,0ch
	xor	ecx,ecx
	xor	edx,edx
	mov	eax,esi
	call	FindVRaidPartition__FCP8VOS2DiskCUlUlCPUlT4
	mov	[ebp-040h],esi;	@CBE130
	add	esp,014h

; 1133 	if( partsize == 0 )
	cmp	dword ptr [ebp-018h],0h;	partsize

; 1131 	rc = FindVRaidPartition(dsk, 0, 0, &partstart, &partsize);
	mov	esi,[ebp+08h];	work

; 1133 	if( partsize == 0 )
	jne	@BLBL281

; 1135 	    Verbose(3, "DrvScan", "no VRAID partition on drive %u", i);
	push	edi
	mov	ecx,offset FLAT:@CBE25
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE1
	mov	eax,03h
	call	Verbose
	add	esp,0ch

; 1137 	    VOS2Drive * drv = new VOS2Drive(dsk);
	mov	eax,020h
	call	__nw__FUi
	mov	edx,[ebp-040h];	@CBE130
	add	esp,04h
	mov	[ebp-010h],eax;	__87
	test	eax,eax
	je	@BLBL283
	sub	esp,08h
	mov	dword ptr [ebp-03ch],01h;	__es
	call	__ct__9VOS2DriveFP8VOS2Disk
	add	esp,08h
	mov	dword ptr [ebp-03ch],0h;	__es
@BLBL283:

; 1138 	    work->apEndPts[work->cEndPts++] = drv;
	mov	ecx,[esi+01ch]
	lea	eax,[ecx+01h]
	mov	[esi+01ch],eax
	mov	eax,[esi+018h]
	mov	edx,[ebp-010h];	__87
	mov	dword ptr [eax+ecx*04h],edx

; 1139 	    work->apOs2Dsk[i] = NULL;
	mov	eax,[esi+08h]
	mov	dword ptr [eax+edi*04h],0h

; 1140 	}
	jmp	@BLBL280
@BLBL281:

; 1143 	    Verbose(2, "DrvScan", "found VRAID partition on drive %u, size %lu", i, partsize);
	push	dword ptr [ebp-018h];	partsize
	mov	ecx,offset FLAT:@CBE26
	push	edi
	mov	edx,offset FLAT:@CBE1
	sub	esp,0ch
	mov	eax,02h
	call	Verbose
	add	esp,010h

; 1144 	    VORDev * dev = new VORDev(dsk, partstart, partsize);
	mov	eax,030h
	call	__nw__FUi
	add	esp,04h
	mov	ebx,eax
	test	ebx,ebx
	je	@BLBL286
	push	dword ptr [ebp-018h];	partsize
	mov	dword ptr [ebp-03ch],02h;	__es
	sub	esp,0ch
	mov	ecx,[ebp-024h];	partstart
	mov	edx,[ebp-028h];	dsk
	call	__ct__6VORDevFP8VOS2DiskUlT2
	add	esp,010h
	mov	dword ptr [ebp-03ch],0h;	__es
@BLBL286:

; 1145 	    work->apDskInfo[work->cDskInfo++] = dev;
	mov	ecx,[esi+014h]
	mov	eax,[esi+010h]
	lea	edx,[ecx+01h]
	mov	[esi+014h],edx
	mov	dword ptr [eax+ecx*04h],ebx

; 1147     } /* end[for every cOs2Dsk] */
@BLBL280:

; 1123     for( i = 0; i < work->cOs2Dsk; ++i )
	inc	edi
	cmp	[esi+0ch],edi
	ja	@BLBL278
	mov	[ebp-014h],ebx;	__88
@BLBL277:

; 1156     PUCHAR	buffer = new UCHAR[SECTOR_SIZE];
	sub	esp,04h
	mov	eax,0200h
	call	__vn__FUi
	add	esp,04h
	mov	[ebp-01ch],eax;	buffer
@BLBL289:

; 1160 	ULONG const	cnt = work->cDskInfo;
	mov	eax,[ebp+08h];	work

; 1159 	int		found = 0;
	mov	dword ptr [ebp-020h],0h;	found

; 1160 	ULONG const	cnt = work->cDskInfo;
	mov	edx,[eax+014h]

; 1162 	for( i = 0; i < cnt; ++i )
	test	edx,edx
	jbe	@BLBL290
	xor	ecx,ecx
@BLBL291:

; 1163 	    if( work->apDskInfo[i] != NULL )
	mov	ebx,[eax+010h]
	cmp	dword ptr [ebx+ecx*04h],0h
	je	@BLBL294

; 1165 		++found;
	inc	dword ptr [ebp-020h];	found

; 1166 		break;
	jmp	@BLBL290
@BLBL294:

; 1162 	for( i = 0; i < cnt; ++i )
	inc	ecx
	cmp	ecx,edx
	jb	@BLBL291
@BLBL290:

; 1168 	if( found == 0 )			/* nothing left? */
	cmp	dword ptr [ebp-020h],0h;	found
	jne	@BLBL296

; 1198     delete[] buffer;
	sub	esp,04h
	mov	eax,[ebp-01ch];	buffer
	call	__vd__FPv
	add	esp,04h

; 1200     return;
	pop	esi
	pop	edi
	pop	ebx
	add	esp,038h
	pop	fs:[0h]
	leave	
	ret	
@BLBL296:

; 1171 	for( i = 0; i < cnt; ++i )
	test	edx,edx
	jbe	@BLBL289
	mov	esi,[ebp+08h];	work
	xor	edi,edi
@BLBL299:

; 1173 	    VRDev * const	dev = work->apDskInfo[i];
	mov	ebx,[esi+010h]
	mov	ebx,dword ptr [ebx+edi*04h]

; 1175 	    if( dev == NULL )
	test	ebx,ebx
	je	@BLBL301

; 1178 	    rc = dev->read(1, 1, buffer);	/* config sector of next level */
	push	dword ptr [ebp-01ch];	buffer

; 1175 	    if( dev == NULL )
	mov	[ebp-040h],edx;	@CBE130

; 1178 	    rc = dev->read(1, 1, buffer);	/* config sector of next level */
	mov	esi,[ebx]
	sub	esp,0ch
	mov	eax,[esi+01ch]
	mov	ecx,01h
	mov	edx,01h
	add	eax,ebx
	call	dword ptr [esi+018h]
	mov	edx,[ebp-040h];	@CBE130
	add	esp,010h
	mov	esi,[ebp+08h];	work

; 1179 	    if( rc != 0 )
	test	eax,eax
	je	@BLBL302

; 1181 		Verbose(1, "DrvScan", "read error %lu, device ignored", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE27
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE1
	mov	eax,01h
	call	Verbose
	mov	edx,[ebp-040h];	@CBE130
	add	esp,010h

; 1182 		delete dev;
	test	ebx,ebx
	je	@BLBL304
	mov	esi,[ebx]
	sub	esp,0ch
	mov	eax,[esi+02ch]
	xor	ecx,ecx
	mov	edx,03h
	add	eax,ebx
	call	dword ptr [esi+028h]
	mov	edx,[ebp-040h];	@CBE130
	mov	esi,[ebp+08h];	work
	add	esp,0ch
@BLBL304:

; 1183 		work->apDskInfo[i] = NULL;
	mov	eax,[esi+010h]
	mov	dword ptr [eax+edi*04h],0h

; 1184 		continue;
	jmp	@BLBL301
@BLBL302:
	mov	[ebp-040h],edx;	@CBE130

; 1187 	    if( memcmp(buffer, "VRAIDDEVICE     ", 16) == 0 )
	sub	esp,0ch
	mov	esi,[ebp-01ch];	buffer
	mov	eax,esi
	mov	ecx,010h
	mov	edx,offset FLAT:@CBE28
	call	memcmp
	mov	edx,[ebp-040h];	@CBE130
	mov	ecx,[ebp-01ch];	buffer
	add	esp,0ch
	mov	esi,[ebp+08h];	work
	test	eax,eax
	jne	@BLBL305

; 1188 		AnalyseVRDev(work, dev, (PSEC_VRDEV)buffer);
	mov	edx,ebx
	sub	esp,0ch
	mov	eax,esi
	call	AnalyseVRDev__FCP5_WORKP5VRDevCP10_SEC_VRDEV
	mov	edx,[ebp-040h];	@CBE130
	add	esp,0ch
	jmp	@BLBL301
@BLBL305:
	mov	[ebp-040h],edx;	@CBE130

; 1189 	    else if( memcmp(buffer, "VRAIDDEVICE2    ", 16) == 0 )
	sub	esp,0ch
	mov	esi,[ebp-01ch];	buffer
	mov	eax,esi
	mov	ecx,010h
	mov	edx,offset FLAT:@CBE22
	call	memcmp
	mov	edx,[ebp-040h];	@CBE130
	mov	ecx,[ebp-01ch];	buffer
	add	esp,0ch
	mov	esi,[ebp+08h];	work
	test	eax,eax
	jne	@BLBL307

; 1190 		AnalyseVRDev2(work, dev, (PSEC_VRDEV2)buffer);
	mov	edx,ebx
	sub	esp,0ch
	mov	eax,esi
	call	AnalyseVRDev2__FCP5_WORKP5VRDevCP11_SEC_VRDEV2
	mov	edx,[ebp-040h];	@CBE130
	add	esp,0ch
	jmp	@BLBL301
@BLBL307:

; 1193 		work->apEndPts[work->cEndPts++] = dev;
	mov	ecx,[esi+01ch]
	lea	eax,[ecx+01h]
	mov	[esi+01ch],eax
	mov	eax,[esi+018h]
	mov	dword ptr [eax+ecx*04h],ebx

; 1194 		work->apDskInfo[i] = NULL;
	mov	eax,[esi+010h]
	mov	dword ptr [eax+edi*04h],0h

; 1196 	}
@BLBL301:

; 1171 	for( i = 0; i < cnt; ++i )
	inc	edi
	cmp	edi,edx
	jb	@BLBL299

; 1157     for(;;)
	jmp	@BLBL289
BuildDevTree__FCP5_WORK	endp

; 1060 FindVRaidPartition(VOS2Disk * const dsk, ULONG const secno, ULONG extstart,

FindVRaidPartition__FCP8VOS2DiskCUlUlCPUlT4	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0fffffdc0h
	mov	[ebp+010h],ecx;	extstart
	sub	esp,0238h

; 1062 {
	mov	dword ptr [ebp-0240h],0h;	__es

; 1060 FindVRaidPartition(VOS2Disk * const dsk, ULONG const secno, ULONG extstart,
	push	ebx

; 1062 {
	mov	dword ptr [ebp-0238h],offset FLAT:@109__fsm_tab;	__es

; 1060 FindVRaidPartition(VOS2Disk * const dsk, ULONG const secno, ULONG extstart,
	push	edi
	mov	[ebp+0ch],edx;	secno
	push	esi

; 1064     PartTable	pt(dsk, secno, true);
	mov	ecx,edx
	push	01h

; 1060 FindVRaidPartition(VOS2Disk * const dsk, ULONG const secno, ULONG extstart,
	mov	[ebp+08h],eax;	dsk

; 1064     PartTable	pt(dsk, secno, true);
	sub	esp,0ch
	mov	edx,eax
	lea	ebx,[ebp-0228h];	pt
	mov	eax,ebx
	call	__ct__9PartTableFP8VOS2DiskUl7Boolean

; 1063     APIRET	rc = 0;
	mov	dword ptr [ebp-022ch],0h;	rc

; 1064     PartTable	pt(dsk, secno, true);
	add	esp,010h

; 1067     if( !pt.isValid() )
	cmp	dword ptr [ebp-018h],0h;	pt

; 1064     PartTable	pt(dsk, secno, true);
	mov	eax,ebx
	mov	dword ptr [ebp-0240h],01h;	__es

; 1067     if( !pt.isValid() )
	jne	@BLBL267

; 1068 	return rc;
	xor	ebx,ebx
	sub	esp,08h
	mov	dword ptr [ebp-0240h],0h;	__es
	mov	edx,02h
	call	__dt__9PartTableFv
	add	esp,08h
	mov	eax,ebx
	pop	esi
	pop	edi
	pop	ebx
	add	esp,023ch
	pop	fs:[0h]
	leave	
	ret	
@BLBL267:

; 1070     if( secno != 0  &&  extstart == 0 )
	mov	eax,[ebp+0ch];	secno
	test	eax,eax
	je	@BLBL268
	cmp	dword ptr [ebp+010h],0h;	extstart
	jne	@BLBL268

; 1071 	extstart = secno;			/* first table is located here */
	mov	[ebp+010h],eax;	extstart
@BLBL268:

; 1073     for( int j = 0; j < 4  &&  *psize == 0; ++j )
	mov	esi,[ebp+018h];	psize
	cmp	dword ptr [esi],0h
	jne	@BLBL269
	mov	edx,[ebp+08h];	dsk
	mov	ecx,[ebp+014h];	pstart
	xor	ebx,ebx
	xor	edi,edi
@BLBL270:

; 1075 	switch( pt.entry[j].SysIndicator )
	mov	eax,[ebp-010h];	pt
	movzx	eax,byte ptr [eax+edi+04h]
	cmp	eax,05h
	je	@BLBL513
	cmp	eax,07ch
	jne	@BLBL511
@BLBL512:

; 1078 	    *pstart = secno + pt.entry[j].RelativeSectors;
	mov	edx,[ebp-010h];	pt
	mov	eax,[ebp+0ch];	secno
	add	eax,dword ptr [edx+edi+08h]
	mov	edx,[ebp+08h];	dsk
	mov	[ecx],eax

; 1079 	    *psize = pt.entry[j].NumSectors;
	mov	eax,[ebp-010h];	pt
	mov	eax,dword ptr [eax+edi+0ch]
	mov	[esi],eax

; 1080 	    break;
	jmp	@BLBL511
@BLBL513:

; 1083 	    rc = FindVRaidPartition(dsk, extstart + pt.entry[j].RelativeSectors,
	push	esi

; 1080 	    break;
	mov	[ebp-0244h],ebx;	@CBE132

; 1083 	    rc = FindVRaidPartition(dsk, extstart + pt.entry[j].RelativeSectors,
	push	ecx
	mov	esi,edx
	sub	esp,0ch
	mov	ebx,[ebp-010h];	pt
	mov	edx,[ebp+010h];	extstart
	mov	ecx,edx
	add	edx,dword ptr [ebx+edi+08h]
	mov	eax,esi
	call	FindVRaidPartition__FCP8VOS2DiskCUlUlCPUlT4
	mov	ebx,[ebp-0244h];	@CBE132
	add	esp,014h
	mov	esi,[ebp+018h];	psize
	mov	edx,[ebp+08h];	dsk
	mov	ecx,[ebp+014h];	pstart
	mov	[ebp-022ch],eax;	rc

; 1089 	    break;				/* ignore empty or other types */
@BLBL511:

; 1073     for( int j = 0; j < 4  &&  *psize == 0; ++j )
	inc	ebx
	add	edi,010h
	cmp	ebx,04h
	jge	@BLBL269
	cmp	dword ptr [esi],0h
	je	@BLBL270
@BLBL269:

; 1092     return rc;
	lea	eax,[ebp-0228h];	pt
	sub	esp,08h
	mov	dword ptr [ebp-0240h],0h;	__es
	mov	edx,02h
	call	__dt__9PartTableFv
	add	esp,08h
	mov	eax,[ebp-022ch];	rc
	pop	esi
	pop	edi
	pop	ebx
	add	esp,023ch
	pop	fs:[0h]
	leave	
	ret	
FindVRaidPartition__FCP8VOS2DiskCUlUlCPUlT4	endp

; 911 AnalyseVRDev2(PWORK const work,VRDev * dsk,PSEC_VRDEV2 const sec)

AnalyseVRDev2__FCP5_WORKP5VRDevCP11_SEC_VRDEV2	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffb8h
	sub	esp,040h

; 912 {
	mov	dword ptr [ebp-048h],0h;	__es

; 911 AnalyseVRDev2(PWORK const work,VRDev * dsk,PSEC_VRDEV2 const sec)
	push	ebx
	mov	ebx,edx
	push	edi
	mov	edi,eax
	push	esi
	mov	esi,ecx

; 915     crc = Crc16(sec, SECTOR_SIZE-2);
	sub	esp,08h

; 912 {
	mov	dword ptr [ebp-040h],offset FLAT:@ea__fsm_tab;	__es

; 915     crc = Crc16(sec, SECTOR_SIZE-2);
	mov	dx,01feh
	mov	eax,esi
	call	Crc16__FPvUs
	mov	ecx,esi

; 911 AnalyseVRDev2(PWORK const work,VRDev * dsk,PSEC_VRDEV2 const sec)
	mov	[ebp+010h],esi;	sec
	mov	[ebp+0ch],ebx;	dsk

; 915     crc = Crc16(sec, SECTOR_SIZE-2);
	add	esp,08h

; 911 AnalyseVRDev2(PWORK const work,VRDev * dsk,PSEC_VRDEV2 const sec)
	mov	[ebp+08h],edi;	work

; 916     if( crc != sec->crc )
	mov	cx,[ecx+01feh]
	cmp	ax,cx
	je	@BLBL206

; 918 	Verbose(1, "DrvScan", "CRC error in VRAIDDEVICE2 sector (%#x, %#x)",
	mov	edx,offset FLAT:@CBE1
	movzx	ecx,cx
	push	ecx
	movzx	eax,ax
	push	eax
	mov	ecx,offset FLAT:@CBE23
	sub	esp,0ch
	mov	eax,01h
	call	Verbose
	add	esp,014h
	mov	eax,edi

; 921 	return;
	pop	esi

; 920 	work->apEndPts[work->cEndPts++] = dsk;
	mov	ecx,[eax+01ch]

; 921 	return;
	pop	edi

; 918 	Verbose(1, "DrvScan", "CRC error in VRAIDDEVICE2 sector (%#x, %#x)",
	mov	edx,ebx

; 920 	work->apEndPts[work->cEndPts++] = dsk;
	lea	ebx,[ecx+01h]
	mov	[eax+01ch],ebx
	mov	eax,[eax+018h]

; 921 	return;
	pop	ebx

; 920 	work->apEndPts[work->cEndPts++] = dsk;
	mov	dword ptr [eax+ecx*04h],edx

; 921 	return;
	add	esp,044h
	pop	fs:[0h]
	leave	
	ret	
@BLBL206:

; 931     switch( sec->u.s.type )
	mov	eax,[ebp+010h];	sec
	movzx	eax,byte ptr [eax+01ah]
	cmp	eax,04h
	jne	@BLBL519
@BLBL518:

; 934 	newdrv = new VMirror(sec->u.s.id, sec->u.s.children);
	sub	esp,04h
	mov	eax,0134h
	call	__nw__FUi
	add	esp,04h
	mov	[ebp-010h],eax;	__79
	test	eax,eax
	je	@BLBL208
	mov	edx,[ebp+010h];	sec
	sub	esp,0ch
	mov	dword ptr [ebp-048h],01h;	__es
	movzx	ecx,word ptr [edx+01ch]
	add	edx,014h
	call	__ct__7VMirrorFPUci
	add	esp,0ch
	mov	dword ptr [ebp-048h],0h;	__es
@BLBL208:
	mov	eax,[ebp-010h];	__79
	mov	[ebp-018h],eax;	newdrv

; 935 	break;
	jmp	@BLBL517
@BLBL519:

; 938 	Verbose(1, "DrvScan", "Unknown VRAID type %u", sec->u.s.type);
	mov	eax,[ebp+010h];	sec
	mov	ecx,offset FLAT:@CBE18
	movzx	eax,byte ptr [eax+01ah]
	push	eax
	mov	edx,offset FLAT:@CBE1
	sub	esp,0ch
	mov	eax,01h
	call	Verbose

; 939 	work->apEndPts[work->cEndPts++] = dsk;
	mov	eax,[ebp+08h];	work

; 938 	Verbose(1, "DrvScan", "Unknown VRAID type %u", sec->u.s.type);
	add	esp,010h

; 939 	work->apEndPts[work->cEndPts++] = dsk;
	mov	ebx,[eax+01ch]

; 940 	return;
	pop	esi

; 939 	work->apEndPts[work->cEndPts++] = dsk;
	lea	ecx,[ebx+01h]
	mov	[eax+01ch],ecx
	mov	eax,[eax+018h]

; 940 	return;
	pop	edi

; 939 	work->apEndPts[work->cEndPts++] = dsk;
	mov	ecx,[ebp+0ch];	dsk
	mov	dword ptr [eax+ebx*04h],ecx

; 940 	return;
	pop	ebx
	add	esp,044h
	pop	fs:[0h]
	leave	
	ret	
@BLBL517:

; 946     for( j = 0; j < sec->u.s.children; ++j )
	mov	eax,[ebp+010h];	sec
	mov	word ptr [ebp-032h],0h;	j
	cmp	word ptr [eax+01ch],0h
	jbe	@BLBL209
@BLBL210:

; 950 	for( k = 0; k < work->cDskInfo; ++k )
	mov	ebx,[ebp+08h];	work

; 948 	Boolean	found = False;
	mov	byte ptr [ebp-033h],0h;	found

; 950 	for( k = 0; k < work->cDskInfo; ++k )
	cmp	dword ptr [ebx+014h],0h
	jbe	@BLBL211
	mov	eax,[ebp+010h];	sec
	movzx	ecx,word ptr [ebp-032h];	j
	sal	ecx,04h
	add	eax,ecx
	mov	[ebp-030h],eax;	@CBE134
	add	eax,01eh
	mov	esi,[ebp-01ch];	result
	mov	[ebp-02ch],eax;	@CBE133
	xor	edi,edi
@BLBL212:

; 952 	    if( work->apDskInfo[k] == NULL )
	mov	ecx,[ebx+010h]
	movzx	edx,di
	mov	eax,dword ptr [ecx+edx*04h]
	mov	[ebp-04ch],edx;	@CBE135
	test	eax,eax
	je	@BLBL214

; 955 	    if( memcmp(work->apDskInfo[k]->queryID(),
	sub	esp,0ch
	mov	edx,[ebp-02ch];	@CBE133
	add	eax,08h
	mov	ecx,06h
	call	memcmp
	mov	ecx,[ebp-04ch];	@CBE135
	add	esp,0ch
	test	eax,eax
	jne	@BLBL214

; 959 		result = ValidateVSec2(work->apDskInfo[k], sec);
	mov	eax,[ebx+010h]
	sub	esp,08h
	mov	eax,dword ptr [eax+ecx*04h]
	mov	edx,[ebp+010h];	sec
	call	ValidateVSec2__FCP5VRDevCP11_SEC_VRDEV2
	add	esp,08h
	mov	esi,eax

; 960 		if( result < 0 )
	test	esi,esi
	jge	@BLBL216

; 962 		    Verbose(2,"DrvScan","MIRROR creation delayed");
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE24
	mov	edx,offset FLAT:@CBE1
	mov	eax,02h
	call	Verbose
	add	esp,0ch

; 963 		    delete newdrv;		/* don't create mirror */
	mov	eax,[ebp-018h];	newdrv
	test	eax,eax
	je	@BLBL515
	mov	[ebp-04ch],esi;	@CBE135
	mov	ebx,[eax]
	sub	esp,0ch
	mov	esi,[ebx+02ch]
	add	eax,esi
	mov	esi,[ebp-04ch];	@CBE135
	xor	ecx,ecx
	mov	edx,03h
	call	dword ptr [ebx+028h]
	mov	ebx,[ebp+08h];	work
	add	esp,0ch
@BLBL515:
	mov	[ebp-01ch],esi;	result
	jmp	@BLBL236
@BLBL216:

; 966 		else if( result != 0 )
	test	esi,esi
	jne	@BLBL214
	mov	[ebp-04ch],esi;	@CBE135

; 970 		found = True;
	mov	byte ptr [ebp-033h],01h;	found

; 971 		newdrv->addChild(work->apDskInfo[k], Tr
; 971 ue,
	mov	edx,[ebx+010h]
	movzx	eax,di
	mov	edx,dword ptr [edx+eax*04h]
	mov	eax,[ebp-030h];	@CBE134
	mov	cl,01h
	test	byte ptr [eax+024h],01h
	setne	al
	push	eax
	sub	esp,0ch
	mov	eax,[ebp-018h];	newdrv
	mov	ebx,[eax]
	mov	esi,[ebx+08ch]
	add	eax,esi
	mov	esi,[ebp-04ch];	@CBE135
	call	dword ptr [ebx+088h]
	mov	ebx,[ebp+08h];	work
	add	esp,010h

; 974 		break;
	jmp	@BLBL514
@BLBL214:

; 950 	for( k = 0; k < work->cDskInfo; ++k )
	movzx	edi,di
	inc	edi
	movzx	eax,di
	cmp	[ebx+014h],eax
	ja	@BLBL212
@BLBL514:
	mov	[ebp-01ch],esi;	result
@BLBL211:

; 977 	if( found == False )
	cmp	byte ptr [ebp-033h],0h;	found
	jne	@BLBL237

; 979 	    VRDev * dummy = new VMissing(sec->u.s.child[j].id,
	sub	esp,04h
	mov	eax,018h
	call	__nw__FUi
	add	esp,04h
	mov	[ebp-014h],eax;	__82
	test	eax,eax
	je	@BLBL234
	mov	edx,[ebp+010h];	sec
	mov	dword ptr [ebp-048h],02h;	__es
	sub	esp,0ch
	movzx	ebx,word ptr [ebp-032h];	j
	sal	ebx,04h
	add	edx,ebx
	mov	ecx,[edx+02ah]
	add	edx,01eh
	call	__ct__8VMissingFPUcUl
	add	esp,0ch
	mov	dword ptr [ebp-048h],0h;	__es
@BLBL234:

; 981 	    newdrv->addChild(dummy, False, False);
	push	0h
	xor	ecx,ecx
	sub	esp,0ch
	mov	eax,[ebp-018h];	newdrv
	mov	edx,[ebp-014h];	__82
	mov	ebx,[eax]
	mov	edi,[ebx+08ch]
	add	eax,edi
	call	dword ptr [ebx+088h]
	add	esp,010h

; 983     }
@BLBL237:

; 946     for( j = 0; j < sec->u.s.children; ++j )
	mov	eax,[ebp+010h];	sec
	movzx	ebx,word ptr [ebp-032h];	j
	inc	ebx
	mov	[ebp-032h],bx;	j
	cmp	[eax+01ch],bx
	ja	@BLBL210
@BLBL209:

; 989     for( j = 0; j < newdrv->queryChildren(); ++j )
	mov	word ptr [ebp-032h],0h;	j
	sub	esp,04h
	mov	eax,[ebp-018h];	newdrv
	mov	ebx,[eax]
	mov	ecx,[ebx+094h]
	add	eax,ecx
	call	dword ptr [ebx+090h]
	add	esp,04h
	test	eax,eax
	jle	@BLBL240
	xor	ebx,ebx
@BLBL241:

; 991 	VRDev *	child = newdrv->getChild(j);
	sub	esp,08h
	mov	eax,[ebp-018h];	newdrv
	movzx	edx,bx
	mov	ecx,[eax]
	mov	edi,[ecx+09ch]
	add	eax,edi
	call	dword ptr [ecx+098h]
	mov	ecx,eax

; 993 	for( k = 0; k < work->cDskInfo; ++k )
	mov	eax,[ebp+08h];	work

; 991 	VRDev *	child = newdrv->getChild(j);
	add	esp,08h

; 993 	for( k = 0; k < work->cDskInfo; ++k )
	cmp	dword ptr [eax+014h],0h
	jbe	@BLBL248
	mov	esi,ecx
	xor	edi,edi
@BLBL243:

; 995 	    if( work->apDskInfo[k] == child )
	mov	ecx,[eax+010h]
	movzx	edx,di
	lea	ecx,dword ptr [ecx+edx*04h]
	cmp	[ecx],esi
	jne	@BLBL246

; 997 		work->apDskInfo[k] = NULL;
	mov	dword ptr [ecx],0h

; 998 		break;
	jmp	@BLBL516
@BLBL246:

; 993 	for( k = 0; k < work->cDskInfo; ++k )
	movzx	edi,di
	inc	edi
	movzx	ecx,di
	cmp	[eax+014h],ecx
	ja	@BLBL243
@BLBL516:
@BLBL248:

; 989     for( j = 0; j < newdrv->queryChildren(); ++j )
	sub	esp,04h
	mov	eax,[ebp-018h];	newdrv
	movzx	ebx,bx
	mov	ecx,[eax]
	inc	ebx
	mov	edx,[ecx+094h]
	add	eax,edx
	call	dword ptr [ecx+090h]
	movzx	edi,bx
	add	esp,04h
	cmp	eax,edi
	jg	@BLBL241
	mov	[ebp-032h],bx;	j
@BLBL240:

; 1004     if( (sec->u.s.flags & RDFLAG_BUILDING) )
	mov	ecx,[ebp+010h];	sec
	test	byte ptr [ecx+01bh],01h
	je	@BLBL251

; 1006 	ULONG	bdone = (ULONG)-1;
	mov	dword ptr [ebp-020h],0ffffffffh;	bdone

; 1008 	for( j = 0; j < sec->u.s.children; ++j )
	mov	word ptr [ebp-032h],0h;	j
	cmp	word ptr [ecx+01ch],0h
	jbe	@BLBL252
	mov	esi,[ebp-024h];	__83
	or	ebx,0ffffffffh
	xor	edi,edi
@BLBL253:

; 1009 	    if( (sec->u.s.child[j].flags & 0x01) == 0 )
	movzx	eax,di
	sal	eax,04h
	add	eax,ecx
	test	byte ptr [eax+024h],01h
	jne	@BLBL257

; 1010 		bdone = min(bdone,sec->u.s.child[j].valid);
	cmp	[eax+026h],ebx
	jbe	@BLBL255
	mov	esi,ebx
	jmp	@BLBL256
@BLBL255:
	movzx	eax,di
	sal	eax,04h
	mov	esi,dword ptr [ecx+eax+026h]
@BLBL256:
	mov	ebx,esi
@BLBL257:

; 1008 	for( j = 0; j < sec->u.s.children; ++j )
	movzx	edi,di
	inc	edi
	cmp	[ecx+01ch],di
	ja	@BLBL253
	mov	[ebp-020h],ebx;	bdone
	mov	[ebp-024h],esi;	__83
	mov	[ebp-032h],di;	j
@BLBL252:

; 1011 	newdrv->setBuild(bdone);		/* build in progress... */
	sub	esp,08h
	mov	eax,[ebp-018h];	newdrv
	mov	edx,[ebp-020h];	bdone
	mov	ebx,[eax]
	mov	ecx,[ebx+0b4h]
	add	eax,ecx
	call	dword ptr [ebx+0b0h]
	add	esp,08h

; 1012     }
@BLBL251:

; 1015     if( (sec->u.s.flags & RDFLAG_HOSTDRIVE) )
	mov	ecx,[ebp+010h];	sec
	test	byte ptr [ecx+01bh],080h
	je	@BLBL260

; 1019 	struct _OS2DEVID * const info = QueryArrayInfo(newdrv->queryID());
	sub	esp,04h
	mov	esi,[ebp-018h];	newdrv
	lea	eax,[esi+08h]
	call	QueryArrayInfo__FCPA6_CUc
	add	esp,04h
	mov	[ebp-028h],eax;	info

; 1021 	if( info != 0 )
	test	eax,eax
	je	@BLBL261

; 1023 	    if( info->avail == 0 )
	cmp	byte ptr [eax+05h],0h
	mov	eax,esi
	jne	@BLBL262

; 1024 		newdrv->forceUnwritable();
	mov	ecx,[eax]
	sub	esp,04h
	mov	edx,[ecx+034h]
	add	eax,edx
	call	dword ptr [ecx+030h]
	add	esp,04h
@BLBL262:

; 1025 	    newdrv->setHostdrive(info->os2idx);
	sub	esp,08h
	mov	eax,[ebp-018h];	newdrv
	mov	edx,[ebp-028h];	info
	mov	ebx,[eax]
	mov	edx,[edx]
	mov	ecx,[ebx+084h]
	add	eax,ecx
	call	dword ptr [ebx+080h]
	add	esp,08h

; 1026 	}
	jmp	@BLBL263
@BLBL261:

; 1028 	    newdrv->setHostdrive();
	sub	esp,08h
	mov	eax,[ebp-018h];	newdrv
	or	edx,0ffffffffh
	mov	ebx,[eax]
	mov	ecx,[ebx+084h]
	add	eax,ecx
	call	dword ptr [ebx+080h]
	add	esp,08h
@BLBL263:

; 1029 	work->apEndPts[work->cEndPts++] = newdrv;
	mov	ecx,[ebp+08h];	work
	mov	ebx,[ecx+01ch]
	mov	eax,[ecx+018h]
	lea	edx,[ebx+01h]
	mov	[ecx+01ch],edx
	mov	ecx,[ebp-018h];	newdrv
	mov	dword ptr [eax+ebx*04h],ecx

; 1030     }
	jmp	@BLBL236
@BLBL260:

; 1032 	work->apDskInfo[work->cDskInfo++] = newdrv;
	mov	ecx,[ebp+08h];	work
	mov	ebx,[ecx+014h]
	mov	eax,[ecx+010h]
	lea	edx,[ebx+01h]
	mov	[ecx+014h],edx
	mov	ecx,[ebp-018h];	newdrv
	mov	dword ptr [eax+ebx*04h],ecx

; 1034   XABORT:
@BLBL236:

; 1035     return;
	pop	esi

; 1034   XABORT:
	mov	dword ptr [ebp-048h],0h;	__es

; 1035     return;
	pop	edi
	pop	ebx
	add	esp,044h
	pop	fs:[0h]
	leave	
	ret	
AnalyseVRDev2__FCP5_WORKP5VRDevCP11_SEC_VRDEV2	endp

; 805 ValidateVSec2(VRDev * const child,PSEC_VRDEV2 const sec)

ValidateVSec2__FCP5VRDevCP11_SEC_VRDEV2	proc
	push	ebp
	mov	ebp,esp
	push	ebx
	mov	ebx,eax
	push	edi
	mov	[ebp+0ch],edx;	sec
	push	esi

; 810     PSEC_VRDEV2 const	childsec = new SEC_VRDEV2;
	mov	eax,0200h

; 805 ValidateVSec2(VRDev * const child,PSEC_VRDEV2 const sec)
	sub	esp,01ch
	mov	[ebp+08h],ebx;	child

; 810     PSEC_VRDEV2 const	childsec = new SEC_VRDEV2;
	call	__nw__FUi
	xchg	ebx,eax

; 813     Verbose(3,"DrvScan","ValidateVSec2( child %p)",child);
	push	eax

; 810     PSEC_VRDEV2 const	childsec = new SEC_VRDEV2;
	mov	[ebp-014h],ebx;	childsec

; 817 	++result;
	mov	dword ptr [ebp-010h],01h;	result

; 813     Verbose(3,"DrvScan","ValidateVSec2( child %p)",child);
	sub	esp,0ch
	mov	edi,offset FLAT:@CBE1
	mov	ecx,offset FLAT:@CBE19
	mov	edx,edi
	mov	eax,03h
	call	Verbose
	add	esp,010h
	mov	edx,edi

; 818 	if( childsec == NULL )
	test	ebx,ebx
	jne	@BLBL170

; 820 	    Verbose(0, "DrvScan", "memory problem, can't validate child");
	xor	eax,eax
	mov	ecx,offset FLAT:@CBE20
	call	Verbose

; 821 	    break;
	jmp	@BLBL200
@BLBL170:

; 825 	rc = child->read(1, 1, childsec);
	mov	eax,[ebp+08h];	child
	mov	edi,[ebp-014h];	childsec
	push	edi

; 824 	++result;
	inc	dword ptr [ebp-010h];	result

; 825 	rc = child->read(1, 1, childsec);
	mov	ebx,[eax]
	sub	esp,0ch
	mov	esi,[ebx+01ch]
	mov	ecx,01h
	mov	edx,01h
	add	eax,esi
	call	dword ptr [ebx+018h]
	add	esp,010h

; 826 	if( rc != 0 )
	test	eax,eax
	je	@BLBL172

; 828 	    Verbose(0, "DrvScan", "read error, can't validate child");
	xor	eax,eax
	mov	ecx,offset FLAT:@CBE21
	mov	edx,offset FLAT:@CBE1
	call	Verbose
	mov	eax,edi

; 829 	    delete childsec;
	call	__dl__FPv

; 830 	    break;
	jmp	@BLBL200
@BLBL172:

; 834 	if( memcmp(sec, childsec, SECTOR_SIZE) != 0 )
	mov	edi,[ebp-014h];	childsec
	mov	esi,[ebp+0ch];	sec
	mov	ecx,0200h
	mov	edx,edi
	mov	eax,esi
	call	memcmp

; 833 	++result;
	mov	ebx,[ebp-010h];	result
	inc	ebx
	mov	[ebp-010h],ebx;	result

; 834 	if( memcmp(sec, childsec, SECTOR_SIZE) != 0 )
	test	eax,eax
	je	@BLBL173

; 836 	    if( memcmp(childsec->sectype, "VRAIDDEVICE2    ", 16) != 0 )
	mov	eax,edi
	mov	ecx,010h
	mov	edx,offset FLAT:@CBE22
	call	memcmp
	test	eax,eax
	jne	@BLBL200

; 840 	    if( Crc16(childsec, SECTOR_SIZE-2) != childsec->crc )
	mov	eax,edi
	mov	dx,01feh
	call	Crc16__FPvUs

; 839 	    ++result;
	inc	ebx
	mov	[ebp-010h],ebx;	result

; 840 	    if( Crc16(childsec, SECTOR_SIZE-2) != childsec->crc )
	cmp	[edi+01feh],ax
	jne	@BLBL200

; 848 	    ++result;
	inc	ebx
	mov	[ebp-010h],ebx;	result

; 849 	    if( childsec->timestamp > sec->timestamp )
	mov	ecx,[esi+010h]
	cmp	[edi+010h],ecx
	jbe	@BLBL176

; 856 		result = -1;
	mov	dword ptr [ebp-010h],0ffffffffh;	result

; 857 		break;
	jmp	@BLBL200
@BLBL176:

; 860 	    if( memcmp(childsec->u.s.id, sec->u.s.id, sizeof(DEVID)) != 0 )
	mov	ebx,[ebp+0ch];	sec
	mov	edi,[ebp-014h];	childsec
	mov	ecx,06h
	lea	edx,[ebx+014h]
	lea	eax,[edi+014h]
	call	memcmp
	test	eax,eax
	jne	@BLBL200

; 863 	    ++result;
	mov	edx,[ebp-010h];	result
	inc	edx
	mov	[ebp-010h],edx;	result

; 864 	    if( childsec->u.s.type != sec->u.s.type )
	mov	cl,[ebx+01ah]
	cmp	[edi+01ah],cl
	jne	@BLBL200

; 867 	    ++result;
	inc	edx
	mov	[ebp-010h],edx;	result

; 868 	    if( childsec->u.s.children != sec->u.s.children )
	mov	cx,[ebx+01ch]
	cmp	[edi+01ch],cx
	jne	@BLBL200

; 871 	    ++result;
	inc	edx
	mov	[ebp-010h],edx;	result

; 872 	    for( i = 0; i < sec->u.s.children; ++i )
	test	cx,cx
	jbe	@BLBL173
	mov	esi,edi
	xor	edi,edi
@BLBL181:
	mov	eax,ebx

; 874 		if( memcmp(childsec->u.s.child[i].id, sec->u.s.child[i].id, sizeof(DEVID)) != 0 )
	movzx	edx,di
	mov	ecx,edx
	sal	ecx,04h
	lea	ebx,dword ptr [eax+ecx]
	add	ecx,esi
	mov	[ebp-01ch],edx;	@CBE137
	lea	eax,[ecx+01eh]
	mov	[ebp-018h],ecx;	@CBE136
	lea	edx,[ebx+01eh]
	mov	ecx,06h
	call	memcmp
	xchg	ebx,eax
	test	ebx,ebx
	mov	edx,[ebp-01ch];	@CBE137
	mov	ecx,[ebp-018h];	@CBE136
	mov	ebx,[ebp+0ch];	sec
	jne	@BLBL200

; 876 		if( childsec->u.s.child[i].size != sec->u.s.child[i].size )
	mov	eax,[eax+02ah]
	cmp	[ecx+02ah],eax
	jne	@BLBL200
	mov	edi,edx

; 872 	    for( i = 0; i < sec->u.s.children; ++i )
	inc	edi
	cmp	[ebx+01ch],di
	ja	@BLBL181

; 879 	}
@BLBL173:

; 884 	result = 0;
	mov	dword ptr [ebp-010h],0h;	result

; 886     while( 0 );
@BLBL200:

; 889     delete childsec;
	mov	eax,[ebp-014h];	childsec
	call	__dl__FPv

; 890     return result;
	mov	eax,[ebp-010h];	result
	add	esp,01ch
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
ValidateVSec2__FCP5VRDevCP11_SEC_VRDEV2	endp

; 669 AnalyseVRDev(PWORK const work,VRDev * dsk,PSEC_VRDEV const sec)

AnalyseVRDev__FCP5_WORKP5VRDevCP10_SEC_VRDEV	proc
	push	ebp
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffc4h
	sub	esp,03ch

; 670 {
	mov	dword ptr [ebp-03ch],0h;	__es

; 669 AnalyseVRDev(PWORK const work,VRDev * dsk,PSEC_VRDEV const sec)
	push	ebx
	mov	ebx,edx
	push	edi
	mov	edi,eax
	push	esi
	mov	esi,ecx

; 673     crc = Crc16(sec, SECTOR_SIZE-2);
	sub	esp,08h

; 670 {
	mov	dword ptr [ebp-034h],offset FLAT:@b5__fsm_tab;	__es

; 673     crc = Crc16(sec, SECTOR_SIZE-2);
	mov	dx,01feh
	mov	eax,esi
	call	Crc16__FPvUs
	mov	ecx,esi

; 669 AnalyseVRDev(PWORK const work,VRDev * dsk,PSEC_VRDEV const sec)
	mov	[ebp+010h],esi;	sec
	mov	[ebp+0ch],ebx;	dsk

; 673     crc = Crc16(sec, SECTOR_SIZE-2);
	add	esp,08h

; 669 AnalyseVRDev(PWORK const work,VRDev * dsk,PSEC_VRDEV const sec)
	mov	[ebp+08h],edi;	work

; 674     if( crc != sec->crc )
	mov	cx,[ecx+01feh]
	cmp	ax,cx
	je	@BLBL129

; 676 	Verbose(1, "DrvScan", "CRC error in VRAIDDEVICE sector (%#x, %#x)",
	mov	edx,offset FLAT:@CBE1
	movzx	ecx,cx
	push	ecx
	movzx	eax,ax
	push	eax
	mov	ecx,offset FLAT:@CBE17
	sub	esp,0ch
	mov	eax,01h
	call	Verbose
	add	esp,014h
	mov	eax,edi

; 679 	return;
	pop	esi

; 678 	work->apEndPts[work->cEndPts++] = dsk;
	mov	ecx,[eax+01ch]

; 679 	return;
	pop	edi

; 676 	Verbose(1, "DrvScan", "CRC error in VRAIDDEVICE sector (%#x, %#x)",
	mov	edx,ebx

; 678 	work->apEndPts[work->cEndPts++] = dsk;
	lea	ebx,[ecx+01h]
	mov	[eax+01ch],ebx
	mov	eax,[eax+018h]

; 679 	return;
	pop	ebx

; 678 	work->apEndPts[work->cEndPts++] = dsk;
	mov	dword ptr [eax+ecx*04h],edx

; 679 	return;
	add	esp,040h
	pop	fs:[0h]
	leave	
	ret	
@BLBL129:

; 689     switch( sec->u.s.type )
	mov	eax,[ebp+010h];	sec
	movzx	eax,byte ptr [eax+01ah]
	cmp	eax,01h
	je	@BLBL526
	cmp	eax,02h
	je	@BLBL524
	cmp	eax,03h
	je	@BLBL525
	jmp	@BLBL527
@BLBL524:

; 692 	newdrv = new VChain(sec->u.s.id, sec->u.s.children);
	sub	esp,04h
	mov	eax,0130h
	call	__nw__FUi
	add	esp,04h
	mov	[ebp-010h],eax;	__68
	test	eax,eax
	je	@BLBL131
	mov	edx,[ebp+010h];	sec
	sub	esp,0ch
	mov	dword ptr [ebp-03ch],01h;	__es
	movzx	ecx,word ptr [edx+01ch]
	add	edx,014h
	call	__ct__6VChainFPUci
	add	esp,0ch
	mov	dword ptr [ebp-03ch],0h;	__es
@BLBL131:
	mov	eax,[ebp-010h];	__68
	mov	[ebp-020h],eax;	newdrv

; 693 	break;
	jmp	@BLBL523
@BLBL525:

; 696 	newdrv = new VStripe(sec->u.s.id, sec->u.s.children);
	sub	esp,04h
	mov	eax,0130h
	call	__nw__FUi
	add	esp,04h
	mov	[ebp-014h],eax;	__69
	test	eax,eax
	je	@BLBL133
	mov	edx,[ebp+010h];	sec
	sub	esp,0ch
	mov	dword ptr [ebp-03ch],02h;	__es
	movzx	ecx,word ptr [edx+01ch]
	add	edx,014h
	call	__ct__7VStripeFPUci
	add	esp,0ch
	mov	dword ptr [ebp-03ch],0h;	__es
@BLBL133:
	mov	eax,[ebp-014h];	__69
	mov	[ebp-020h],eax;	newdrv

; 697 	break;
	jmp	@BLBL523
@BLBL526:

; 700 	newdrv = new VSingle(sec->u.s.id);
	sub	esp,04h
	mov	eax,030h
	call	__nw__FUi
	add	esp,04h
	mov	[ebp-018h],eax;	__70
	test	eax,eax
	je	@BLBL135
	mov	edx,[ebp+010h];	sec
	sub	esp,08h
	mov	dword ptr [ebp-03ch],03h;	__es
	add	edx,014h
	call	__ct__7VSingleFPUc
	add	esp,08h
	mov	dword ptr [ebp-03ch],0h;	__es
@BLBL135:
	mov	eax,[ebp-018h];	__70
	mov	[ebp-020h],eax;	newdrv

; 701 	break;
	jmp	@BLBL523
@BLBL527:

; 704 	Verbose(1, "DrvScan", "Unknown VRAID type %u", sec->u.s.type);
	mov	eax,[ebp+010h];	sec
	mov	ecx,offset FLAT:@CBE18
	movzx	eax,byte ptr [eax+01ah]
	push	eax
	mov	edx,offset FLAT:@CBE1
	sub	esp,0ch
	mov	eax,01h
	call	Verbose

; 705 	work->apEndPts[work->cEndPts++] = dsk;
	mov	eax,[ebp+08h];	work

; 704 	Verbose(1, "DrvScan", "Unknown VRAID type %u", sec->u.s.type);
	add	esp,010h

; 705 	work->apEndPts[work->cEndPts++] = dsk;
	mov	ebx,[eax+01ch]

; 706 	return;
	pop	esi

; 705 	work->apEndPts[work->cEndPts++] = dsk;
	lea	ecx,[ebx+01h]
	mov	[eax+01ch],ecx
	mov	eax,[eax+018h]

; 706 	return;
	pop	edi

; 705 	work->apEndPts[work->cEndPts++] = dsk;
	mov	ecx,[ebp+0ch];	dsk
	mov	dword ptr [eax+ebx*04h],ecx

; 706 	return;
	pop	ebx
	add	esp,040h
	pop	fs:[0h]
	leave	
	ret	
@BLBL523:

; 712     for( j = 0; j < sec->u.s.children; ++j )
	mov	eax,[ebp+010h];	sec
	mov	word ptr [ebp-026h],0h;	j
	cmp	word ptr [eax+01ch],0h
	jbe	@BLBL136
@BLBL137:

; 716 	for( k = 0; k < work->cDskInfo; ++k )
	mov	esi,[ebp+08h];	work

; 714 	Boolean	found = False;
	mov	byte ptr [ebp-027h],0h;	found

; 716 	for( k = 0; k < work->cDskInfo; ++k )
	cmp	dword ptr [esi+014h],0h
	jbe	@BLBL138
	mov	ebx,[ebp+010h];	sec
	movzx	eax,word ptr [ebp-026h];	j
	imul	eax,0ah
	add	ebx,eax
	add	ebx,01eh
	mov	[ebp-048h],ebx;	@CBE138
	xor	edi,edi
@BLBL139:

; 718 	    if( work->apDskInfo[k] == NULL )
	mov	ecx,[esi+010h]
	movzx	edx,di
	mov	eax,dword ptr [ecx+edx*04h]
	mov	[ebp-044h],edx;	@CBE140
	test	eax,eax
	je	@BLBL141

; 721 	    if( memcmp(work->apDskInfo[k]->queryID(),
	mov	edx,ebx
	sub	esp,0ch
	add	eax,08h
	mov	ecx,06h
	call	memcmp
	mov	ecx,eax
	mov	eax,[ebp-044h];	@CBE140
	add	esp,0ch
	test	ecx,ecx
	jne	@BLBL141
	mov	[ebp-040h],edi;	@CBE139

; 724 		found = True;
	mov	byte ptr [ebp-027h],01h;	found

; 725 		newdrv->addChild(work->apDskInfo[k], True);
	mov	edx,[esi+010h]
	push	01h
	mov	edx,dword ptr [edx+eax*04h]
	sub	esp,0ch
	mov	eax,[ebp-020h];	newdrv
	mov	cl,01h
	mov	ebx,[eax]
	mov	edi,[ebx+08ch]
	add	eax,edi
	mov	edi,[ebp-040h];	@CBE139
	call	dword ptr [ebx+088h]
	mov	ebx,[ebp-048h];	@CBE138
	add	esp,010h

; 727 		break;
	jmp	@BLBL138
@BLBL141:

; 716 	for( k = 0; k < work->cDskInfo; ++k )
	movzx	edi,di
	inc	edi
	movzx	eax,di
	cmp	[esi+014h],eax
	ja	@BLBL139
@BLBL138:

; 730 	if( found == False )
	cmp	byte ptr [ebp-027h],0h;	found
	jne	@BLBL148

; 732 	    VRDev *	dummy = new VMissing(sec->u.s.child[j].id,
	sub	esp,04h
	mov	eax,018h
	call	__nw__FUi
	add	esp,04h
	mov	[ebp-01ch],eax;	__72
	test	eax,eax
	je	@BLBL147
	mov	edx,[ebp+010h];	sec
	mov	dword ptr [ebp-03ch],04h;	__es
	sub	esp,0ch
	movzx	ebx,word ptr [ebp-026h];	j
	imul	ebx,0ah
	add	edx,ebx
	mov	ecx,[edx+024h]
	add	edx,01eh
	call	__ct__8VMissingFPUcUl
	add	esp,0ch
	mov	dword ptr [ebp-03ch],0h;	__es
@BLBL147:

; 734 	    newdrv->addChild(dummy, False, False);
	push	0h
	xor	ecx,ecx
	sub	esp,0ch
	mov	eax,[ebp-020h];	newdrv
	mov	edx,[ebp-01ch];	__72
	mov	ebx,[eax]
	mov	edi,[ebx+08ch]
	add	eax,edi
	call	dword ptr [ebx+088h]
	add	esp,010h

; 736     }
@BLBL148:

; 712     for( j = 0; j < sec->u.s.children; ++j )
	mov	eax,[ebp+010h];	sec
	movzx	ebx,word ptr [ebp-026h];	j
	inc	ebx
	mov	[ebp-026h],bx;	j
	cmp	[eax+01ch],bx
	ja	@BLBL137
@BLBL136:

; 742     for( j = 0; j < newdrv->queryChildren(); ++j )
	mov	word ptr [ebp-026h],0h;	j
	sub	esp,04h
	mov	eax,[ebp-020h];	newdrv
	mov	ebx,[eax]
	mov	ecx,[ebx+094h]
	add	eax,ecx
	call	dword ptr [ebx+090h]
	add	esp,04h
	test	eax,eax
	jle	@BLBL151
	xor	ebx,ebx
@BLBL152:

; 744 	VRDev *	child = newdrv->getChild(j);
	sub	esp,08h
	mov	eax,[ebp-020h];	newdrv
	movzx	edx,bx
	mov	ecx,[eax]
	mov	edi,[ecx+09ch]
	add	eax,edi
	call	dword ptr [ecx+098h]
	mov	ecx,eax

; 746 	for( k = 0; k < work->cDskInfo; ++k )
	mov	eax,[ebp+08h];	work

; 744 	VRDev *	child = newdrv->getChild(j);
	add	esp,08h

; 746 	for( k = 0; k < work->cDskInfo; ++k )
	cmp	dword ptr [eax+014h],0h
	jbe	@BLBL159
	mov	esi,ecx
	xor	edi,edi
@BLBL154:

; 748 	    if( work->apDskInfo[k] == child )
	mov	ecx,[eax+010h]
	movzx	edx,di
	lea	ecx,dword ptr [ecx+edx*04h]
	cmp	[ecx],esi
	jne	@BLBL157

; 750 		work->apDskInfo[k] = NULL;
	mov	dword ptr [ecx],0h

; 751 		break;
	jmp	@BLBL522
@BLBL157:

; 746 	for( k = 0; k < work->cDskInfo; ++k )
	movzx	edi,di
	inc	edi
	movzx	ecx,di
	cmp	[eax+014h],ecx
	ja	@BLBL154
@BLBL522:
@BLBL159:

; 742     for( j = 0; j < newdrv->queryChildren(); ++j )
	sub	esp,04h
	mov	eax,[ebp-020h];	newdrv
	movzx	ebx,bx
	mov	ecx,[eax]
	inc	ebx
	mov	edx,[ecx+094h]
	add	eax,edx
	call	dword ptr [ecx+090h]
	movzx	edi,bx
	add	esp,04h
	cmp	eax,edi
	jg	@BLBL152
	mov	[ebp-026h],bx;	j
@BLBL151:

; 757     if( (sec->u.s.flags & RDFLAG_HOSTDRIVE) )
	mov	ecx,[ebp+010h];	sec
	test	byte ptr [ecx+01bh],080h
	je	@BLBL162

; 761 	struct _OS2DEVID * const info = QueryArrayInfo(newdrv->queryID());
	sub	esp,04h
	mov	esi,[ebp-020h];	newdrv
	lea	eax,[esi+08h]
	call	QueryArrayInfo__FCPA6_CUc
	add	esp,04h
	mov	[ebp-024h],eax;	info

; 763 	if( info != 0 )
	test	eax,eax
	je	@BLBL163

; 765 	    if( info->avail == 0 )
	cmp	byte ptr [eax+05h],0h
	mov	eax,esi
	jne	@BLBL164

; 766 		newdrv->forceUnwritable();
	mov	ecx,[eax]
	sub	esp,04h
	mov	edx,[ecx+034h]
	add	eax,edx
	call	dword ptr [ecx+030h]
	add	esp,04h
@BLBL164:

; 767 	    newdrv->setHostdrive(info->os2idx);
	sub	esp,08h
	mov	eax,[ebp-020h];	newdrv
	mov	edx,[ebp-024h];	info
	mov	ebx,[eax]
	mov	edx,[edx]
	mov	ecx,[ebx+084h]
	add	eax,ecx
	call	dword ptr [ebx+080h]
	add	esp,08h

; 768 	}
	jmp	@BLBL165
@BLBL163:

; 770 	    newdrv->setHostdrive();
	sub	esp,08h
	mov	eax,[ebp-020h];	newdrv
	or	edx,0ffffffffh
	mov	ebx,[eax]
	mov	ecx,[ebx+084h]
	add	eax,ecx
	call	dword ptr [ebx+080h]
	add	esp,08h
@BLBL165:

; 771 	work->apEndPts[work->cEndPts++] = newdrv;
	mov	ecx,[ebp+08h];	work

; 772     }
	pop	esi

; 771 	work->apEndPts[work->cEndPts++] = newdrv;
	mov	ebx,[ecx+01ch]
	mov	eax,[ecx+018h]

; 772     }
	pop	edi

; 771 	work->apEndPts[work->cEndPts++] = newdrv;
	lea	edx,[ebx+01h]
	mov	[ecx+01ch],edx
	mov	ecx,[ebp-020h];	newdrv
	mov	dword ptr [eax+ebx*04h],ecx

; 772     }
	pop	ebx
	add	esp,040h
	pop	fs:[0h]
	leave	
	ret	
@BLBL162:

; 774 	work->apDskInfo[work->cDskInfo++] = newdrv;
	mov	ecx,[ebp+08h];	work

; 776     return;
	pop	esi

; 774 	work->apDskInfo[work->cDskInfo++] = newdrv;
	mov	ebx,[ecx+014h]
	mov	eax,[ecx+010h]

; 776     return;
	pop	edi

; 774 	work->apDskInfo[work->cDskInfo++] = newdrv;
	lea	edx,[ebx+01h]
	mov	[ecx+014h],edx
	mov	ecx,[ebp-020h];	newdrv
	mov	dword ptr [eax+ebx*04h],ecx

; 776     return;
	pop	ebx
	add	esp,040h
	pop	fs:[0h]
	leave	
	ret	
AnalyseVRDev__FCP5_WORKP5VRDevCP10_SEC_VRDEV	endp

; 541 AllocateOs2Devices(PWORK const work)

AllocateOs2Devices__FCP5_WORK	proc
	push	ebp

; 552     Verbose(3, "DrvScan", "AllocateOs2Devices");
	mov	ecx,offset FLAT:@CBE12

; 541 AllocateOs2Devices(PWORK const work)
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP

; 552     Verbose(3, "DrvScan", "AllocateOs2Devices");
	mov	edx,offset FLAT:@CBE1

; 541 AllocateOs2Devices(PWORK const work)
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffc4h
	mov	[ebp+08h],eax;	work
	sub	esp,030h

; 552     Verbose(3, "DrvScan", "AllocateOs2Devices");
	mov	eax,03h

; 541 AllocateOs2Devices(PWORK const work)
	push	ebx
	push	edi
	push	esi

; 552     Verbose(3, "DrvScan", "AllocateOs2Devices");
	sub	esp,0ch

; 542 {
	mov	dword ptr [ebp-03ch],0h;	__es
	mov	dword ptr [ebp-034h],offset FLAT:@7d__fsm_tab;	__es

; 546     USHORT		arraycnt = 0;
	mov	word ptr [ebp-026h],0h;	arraycnt

; 547     PUSHORT		arrayhd = NULL;
	mov	dword ptr [ebp-018h],0h;	arrayhd

; 548     USHORT		physcnt = 0;
	mov	word ptr [ebp-028h],0h;	physcnt

; 549     PUSHORT		physhd = NULL;
	mov	dword ptr [ebp-01ch],0h;	physhd

; 552     Verbose(3, "DrvScan", "AllocateOs2Devices");
	call	Verbose
	add	esp,0ch

; 560     if( (fSetupMode & SM_FLTAVAIL) )
	test	byte ptr  fSetupMode,01h
	je	@BLBL103

; 564 	    rc = DriverArraylist(&arraycnt, &arrayhd);
	lea	edx,[ebp-018h];	arrayhd
	sub	esp,08h
	lea	eax,[ebp-026h];	arraycnt
	call	DriverArraylist__FPUsPPUs
	add	esp,08h

; 565 	    if( rc )
	test	eax,eax
	je	@BLBL109

; 567 		Verbose(1, "DrvSetup", "DriverArraylist - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE13
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE9
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 568 		arraycnt = 0;
	mov	word ptr [ebp-026h],0h;	arraycnt

; 569 		break;
@BLBL109:

; 576 	    rc = DriverPhyslist(&physcnt, &physhd);
	lea	edx,[ebp-01ch];	physhd
	sub	esp,08h
	lea	eax,[ebp-028h];	physcnt
	call	DriverPhyslist__FPUsPPUs
	add	esp,08h

; 577 	    if( rc )
	test	eax,eax
	je	@BLBL103

; 579 		Verbose(1, "DrvSetup", "DriverPhyslist - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE10
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE9
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 580 		physcnt = 0;
	mov	word ptr [ebp-028h],0h;	physcnt

; 581 		break;
@BLBL103:

; 589     rc = PDskEnum(&os2cnt);
	lea	eax,[ebp-014h];	os2cnt
	sub	esp,04h
	call	PDskEnum
	add	esp,04h

; 590     if( rc )
	test	eax,eax
	je	@BLBL114

; 592 	Verbose(1, "DrvSetup", "PDskEnum - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE14
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE9
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 593 	os2cnt = 0;
	mov	dword ptr [ebp-014h],0h;	os2cnt

; 594     }
@BLBL114:

; 595     for( i = 0; i < os2cnt; ++i )
	cmp	dword ptr [ebp-014h],0h;	os2cnt
	jbe	@BLBL115
	mov	ebx,[ebp-010h];	__66
	mov	esi,[ebp-024h];	newdev
	xor	edi,edi
@BLBL116:

; 600 	rc = PDskOpen(i, &handle);
	lea	edx,[ebp-020h];	handle
	sub	esp,08h
	mov	eax,edi
	call	PDskOpen
	add	esp,08h

; 601 	if( rc != 0 )
	test	eax,eax
	je	@BLBL117

; 603 	    Verbose(1, "DrvScan", "PDskOpen(%lu) - rc %lu", i, rc);
	push	eax
	mov	ecx,offset FLAT:@CBE15
	push	edi
	mov	edx,offset FLAT:@CBE1
	sub	esp,0ch
	mov	eax,01h
	call	Verbose
	add	esp,08h

; 604 	    MyMessageBox(2, HWND_DESKTOP,
	mov	ecx,offset FLAT:@CBE16
	mov	edx,01h
	mov	eax,02h
	call	MyMessageBox__FCiCUlPce
	add	esp,0ch

; 610 	    continue;
	jmp	@BLBL118
@BLBL117:

; 613 	VOS2Disk * newdev = new VOS2Disk(i, handle);
	sub	esp,04h
	mov	eax,020h
	call	__nw__FUi
	add	esp,04h
	mov	ebx,eax
	test	ebx,ebx
	je	@BLBL120
	mov	edx,edi
	mov	dword ptr [ebp-03ch],01h;	__es
	sub	esp,0ch
	mov	ecx,[ebp-020h];	handle
	call	__ct__8VOS2DiskFUlT1
	add	esp,0ch
	mov	dword ptr [ebp-03ch],0h;	__es
@BLBL120:
	mov	esi,ebx

; 624 	if( (fSetupMode & SM_FLTAVAIL) )
	test	byte ptr  fSetupMode,01h
	je	@BLBL121

; 625 	    if( IsProvidedByFlt(newdev, arraycnt, arrayhd, physcnt, physhd) )
	push	dword ptr [ebp-01ch];	physhd
	mov	ax,[ebp-028h];	physcnt
	push	eax
	mov	ecx,[ebp-018h];	arrayhd
	sub	esp,0ch
	mov	dx,[ebp-026h];	arraycnt
	mov	eax,esi
	call	IsProvidedByFlt__FCP8VOS2DiskUsPUsT2T3
	add	esp,014h
	test	eax,eax
	je	@BLBL121

; 627 		delete newdev;
	test	esi,esi
	je	@BLBL118
	mov	eax,esi
	sub	esp,08h
	mov	edx,03h
	call	__dt__8VOS2DiskFv
	add	esp,08h
	jmp	@BLBL118
@BLBL121:

; 635 	work->apOs2Dsk[work->cOs2Dsk] = newdev;
	mov	eax,[ebp+08h];	work
	mov	ecx,[eax+08h]
	mov	edx,[eax+0ch]
	mov	dword ptr [ecx+edx*04h],esi

; 636 	++work->cOs2Dsk;
	inc	dword ptr [eax+0ch]

; 638     } /* end[for(every device)] */
@BLBL118:

; 595     for( i = 0; i < os2cnt; ++i )
	inc	edi
	cmp	[ebp-014h],edi;	os2cnt
	ja	@BLBL116
	mov	[ebp-010h],ebx;	__66
@BLBL115:

; 641     return os2cnt;
	pop	esi
	mov	eax,[ebp-014h];	os2cnt
	pop	edi
	pop	ebx
	add	esp,034h
	pop	fs:[0h]
	leave	
	ret	
AllocateOs2Devices__FCP5_WORK	endp

; 484 AllocateFltDevices(PWORK const work)

AllocateFltDevices__FCP5_WORK	proc
	push	ebp

; 491     Verbose(3, "DrvScan", "AllocateFltDevices");
	mov	ecx,offset FLAT:@CBE8

; 484 AllocateFltDevices(PWORK const work)
	mov	ebp,esp
	push	offset FLAT: _Exception_CPP

; 491     Verbose(3, "DrvScan", "AllocateFltDevices");
	mov	edx,offset FLAT:@CBE1

; 484 AllocateFltDevices(PWORK const work)
	push	fs:[0h]
	mov	fs:[0h],esp
	push	0ffffffcch
	sub	esp,028h
	push	ebx
	mov	ebx,eax
	push	edi

; 491     Verbose(3, "DrvScan", "AllocateFltDevices");
	mov	eax,03h

; 484 AllocateFltDevices(PWORK const work)
	push	esi
	mov	[ebp+08h],ebx;	work

; 491     Verbose(3, "DrvScan", "AllocateFltDevices");
	sub	esp,0ch

; 485 {
	mov	dword ptr [ebp-034h],0h;	__es
	mov	dword ptr [ebp-02ch],offset FLAT:@59__fsm_tab;	__es

; 491     Verbose(3, "DrvScan", "AllocateFltDevices");
	call	Verbose
	mov	eax,ebx
	add	esp,04h

; 493     idx = work->cDskInfo;			/* new index: behind already found
	mov	eax,[eax+014h]
	mov	[ebp-014h],eax;	idx

; 497 	rc = DriverPhyslist(&cnt, &list);
	lea	edx,[ebp-018h];	list
	lea	eax,[ebp-01eh];	cnt
	call	DriverPhyslist__FPUsPPUs
	add	esp,08h

; 498 	if( rc )
	test	eax,eax
	je	@BLBL90

; 500 	    Verbose(1, "DrvSetup", "DriverPhyslist - rc %lu", rc);
	push	eax
	mov	ecx,offset FLAT:@CBE10
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE9
	mov	eax,01h
	call	Verbose
	add	esp,010h

; 501 	    cnt = 0;
	mov	word ptr [ebp-01eh],0h;	cnt

; 502 	    break;
	jmp	@BLBL91
@BLBL90:

; 505 	for( i = 0; i < cnt; ++i )
	cmp	word ptr [ebp-01eh],0h;	cnt
	jbe	@BLBL91
	mov	esi,[ebp-014h];	idx
	xor	edi,edi
@BLBL93:

; 507 	    work->apDskInfo[idx] = new VFRDev(list[i]);
	mov	eax,[ebp+08h];	work
	sub	esp,04h
	mov	eax,[eax+010h]
	lea	eax,dword ptr [eax+esi*04h]
	mov	[ebp-01ch],eax;	@CBE68
	mov	eax,02ch
	call	__nw__FUi
	add	esp,04h
	mov	ebx,eax
	test	ebx,ebx
	je	@BLBL95
	mov	dword ptr [ebp-034h],01h;	__es
	sub	esp,08h
	mov	edx,[ebp-018h];	list
	mov	dx,word ptr [edx+edi*02h]
	call	__ct__6VFRDevFUs
	add	esp,08h
	mov	dword ptr [ebp-034h],0h;	__es
@BLBL95:

; 505 	for( i = 0; i < cnt; ++i )
	inc	edi

; 511 	    Verbose(2, "DrvSetup", "FLT drive capacity: %lu MBytes",
	sub	esp,04h

; 507 	    work->apDskInfo[idx] = new VFRDev(list[i]);
	mov	eax,[ebp-01ch];	@CBE68
	mov	[eax],ebx

; 509 	    ++work->cDskInfo;			/* keep this device! */
	mov	eax,[ebp+08h];	work
	inc	dword ptr [eax+014h]

; 511 	    Verbose(2, "DrvSetup", "FLT drive capacity: %lu MBytes",
	mov	eax,[eax+010h]
	mov	eax,dword ptr [eax+esi*04h]

; 514 	    ++idx;
	inc	esi

; 511 	    Verbose(2, "DrvSetup", "FLT drive capacity: %lu MBytes",
	mov	ecx,[eax]
	mov	edx,[ecx+014h]
	add	eax,edx
	call	dword ptr [ecx+010h]
	add	esp,04h
	shr	eax,01h
	shr	eax,0ah
	push	eax
	mov	ecx,offset FLAT:@CBE11
	sub	esp,0ch
	mov	edx,offset FLAT:@CBE9
	mov	eax,02h
	call	Verbose
	add	esp,010h

; 505 	for( i = 0; i < cnt; ++i )
	movzx	eax,word ptr [ebp-01eh];	cnt
	cmp	eax,edi
	ja	@BLBL93
	mov	[ebp-010h],ebx;	__63
	mov	[ebp-014h],esi;	idx

; 517     while(0);					/* end[OS/2 devices] */
@BLBL91:

; 519     return idx;
	pop	esi
	mov	eax,[ebp-014h];	idx
	pop	edi
	pop	ebx
	add	esp,02ch
	pop	fs:[0h]
	leave	
	ret	
AllocateFltDevices__FCP5_WORK	endp

; 319 IsProvidedByFlt(VOS2Disk * const newdev,

IsProvidedByFlt__FCP8VOS2DiskUsPUsT2T3	proc
	push	ebp
	xchg	ecx,eax
	mov	ebp,esp
	push	ebx
	mov	ebx,edx
	push	edi
	mov	[ebp+010h],eax;	ahd
	push	esi

; 334     ainfo = new PVRAID_ARRAYINFO_DATA[acnt];
	movzx	eax,bx

; 319 IsProvidedByFlt(VOS2Disk * const newdev,
	sub	esp,060h

; 334     ainfo = new PVRAID_ARRAYINFO_DATA[acnt];
	sal	eax,02h

; 319 IsProvidedByFlt(VOS2Disk * const newdev,
	mov	[ebp+08h],ecx;	newdev

; 334     ainfo = new PVRAID_ARRAYINFO_DATA[acnt];
	call	__vn__FUi
	mov	edx,ebx

; 319 IsProvidedByFlt(VOS2Disk * const newdev,
	mov	[ebp+0ch],bx;	acnt

; 326     BOOL	isarray = FALSE;
	mov	dword ptr [ebp-010h],0h;	isarray

; 335     for( j = 0; j < acnt; ++j )
	test	dx,dx

; 328     BOOL	isphys = FALSE;
	mov	dword ptr [ebp-024h],0h;	isphys

; 334     ainfo = new PVRAID_ARRAYINFO_DATA[acnt];
	mov	[ebp-014h],eax;	ainfo

; 335     for( j = 0; j < acnt; ++j )
	jbe	@BLBL19
	mov	ebx,eax
	mov	si,dx
	xor	edi,edi
@BLBL20:

; 337 	ainfo[j] = new VRAID_ARRAYINFO_DATA;
	movzx	eax,di
	mov	[ebp-060h],eax;	@CBE141
	mov	eax,0eh
	call	__nw__FUi
	mov	ecx,[ebp-060h];	@CBE141
	mov	dword ptr [ebx+ecx*04h],eax

; 338 	memset(ainfo[j], 0, sizeof(VRAID_ARRAYINFO_DATA));
	mov	ecx,0eh
	xor	edx,edx
	call	memset

; 335     for( j = 0; j < acnt; ++j )
	movzx	edi,di
	inc	edi
	cmp	di,si
	jb	@BLBL20
@BLBL19:

; 341     pinfo = new PVRAID_PHYSINFO_DATA[pcnt];
	mov	bx,[ebp+014h];	pcnt
	movzx	eax,bx
	sal	eax,02h
	call	__vn__FUi
	mov	ecx,ebx
	mov	[ebp-020h],eax;	pinfo

; 342     for( j = 0; j < pcnt; ++j )
	test	cx,cx
	jbe	@BLBL24
	mov	ebx,eax
	mov	si,cx
	xor	edi,edi
@BLBL25:

; 344 	pinfo[j] = new VRAID_PHYSINFO_DATA;
	movzx	eax,di
	mov	[ebp-060h],eax;	@CBE141
	mov	eax,011h
	call	__nw__FUi
	mov	ecx,[ebp-060h];	@CBE141
	mov	dword ptr [ebx+ecx*04h],eax

; 345 	memset(pinfo[j], 0, sizeof(VRAID_PHYSINFO_DATA));
	mov	ecx,011h
	xor	edx,edx
	call	memset

; 342     for( j = 0; j < pcnt; ++j )
	movzx	edi,di
	inc	edi
	cmp	di,si
	jb	@BLBL25
@BLBL24:

; 349     if( acnt != 0 )
	mov	bx,[ebp+0ch];	acnt
	test	bx,bx
	je	@BLBL29

; 353 	PUCHAR		dummybuffer = new UCHAR[SECTOR_SIZE];
	mov	eax,0200h
	call	__vn__FUi

; 355 	for( retry = 0; retry < 3; ++retry )
	mov	si,bx
	mov	ebx,[ebp+010h];	ahd
	mov	ecx,[ebp-014h];	ainfo

; 352 	unsigned	flt_changes = 0;
	mov	dword ptr [ebp-018h],0h;	flt_changes

; 355 	for( retry = 0; retry < 3; ++retry )
	mov	dword ptr [ebp-02ch],0h;	retry

; 353 	PUCHAR		dummybuffer = new UCHAR[SECTOR_SIZE];
	mov	[ebp-028h],eax;	dummybuffer

; 355 	for( retry = 0; retry < 3; ++retry )
@BLBL31:

; 360 	    for( j = 0; j < acnt; ++j )
	test	si,si
	jbe	@BLBL32
	mov	eax,ecx
	mov	[ebp-014h],eax;	ainfo
	xor	edi,edi
@BLBL33:

; 361 		rc = DriverArrayinfo(ahd[j], ainfo[j]);
	movzx	eax,di
	mov	edx,dword ptr [ecx+eax*04h]
	mov	ax,word ptr [ebx+eax*02h]
	call	DriverArrayinfo__FUsP21_VRAID_ARRAYINFO_DATA

; 360 	    for( j = 0; j < acnt; ++j )
	movzx	edi,di
	inc	edi

; 361 		rc = DriverArrayinfo(ahd[j], ainfo[j]);
	mov	ecx,[ebp-014h];	ainfo

; 360 	    for( j = 0; j < acnt; ++j )
	cmp	di,si
	jb	@BLBL33
@BLBL32:

; 363 	    rc = newdev->read(0, 1, dummybuffer);
	push	dword ptr [ebp-028h];	dummybuffer
	mov	eax,[ebp+08h];	newdev

; 360 	    for( j = 0; j < acnt; ++j )
	mov	[ebp-060h],esi;	@CBE141

; 363 	    rc = newdev->read(0, 1, dummybuffer);
	mov	edi,[eax]
	sub	esp,0ch
	mov	esi,[edi+01ch]
	add	eax,esi
	mov	esi,[ebp-060h];	@CBE141
	mov	ecx,01h
	xor	edx,edx
	call	dword ptr [edi+018h]
	mov	ecx,[ebp-014h];	ainfo
	add	esp,010h

; 365 	    for( j = 0; j < acnt; ++j )
	test	si,si
	jbe	@BLBL37
	xor	edi,edi
	mov	[ebp+0ch],si;	acnt
@BLBL38:

; 367 		Verbose(3, "DrvScan", "Testing array %u", j);
	mov	eax,offset FLAT:@CBE1
	movzx	esi,di
	push	esi
	mov	edx,eax
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE2
	mov	eax,03h
	call	Verbose

; 368 		rc = DriverArrayinfo(ahd[j], &this_info);
	mov	ax,word ptr [ebx+esi*02h]
	lea	edx,[ebp-048h];	this_info
	call	DriverArrayinfo__FUsP21_VRAID_ARRAYINFO_DATA
	mov	ecx,[ebp-014h];	ainfo
	xchg	esi,eax
	add	esp,010h

; 369 		if( rc != 0 )
	test	esi,esi
	mov	[ebp-060h],esi;	@CBE141
	mov	esi,[ebp+0ch];	acnt

; 368 		rc = DriverArrayinfo(ahd[j], &this_info);
	mov	edx,offset FLAT:@CBE1

; 369 		if( rc != 0 )
	je	@BLBL39
	mov	ecx,[ebp-060h];	@CBE141

; 370 		    Verbose(1, "DrvScan", "Array Info %u - rc %lu", j, rc);
	push	ecx
	mov	ecx,offset FLAT:@CBE3
	push	eax
	mov	eax,01h
	sub	esp,0ch
	call	Verbose
	mov	ecx,[ebp-014h];	ainfo
	add	esp,014h
	jmp	@BLBL42
@BLBL39:

; 371 		else if( this_info.reqcnt > ainfo[j]->reqcnt )
	movzx	edx,di
	mov	eax,dword ptr [ecx+edx*04h]
	mov	[ebp-060h],edx;	@CBE141
	mov	dx,[ebp-040h];	this_info
	cmp	[eax+08h],dx
	jae	@BLBL42

; 373 		    Verbose(2, "DrvScan",
	mov	eax,[ebp+08h];	newdev
	call	queryIndex__8VOS2DiskFv
	push	eax
	mov	edx,[ebp-060h];	@CBE141
	push	edx

; 376 		    ++flt_changes;
	inc	dword ptr [ebp-018h];	flt_changes

; 377 		    accessed_array = j;
	mov	[ebp-03ah],di;	accessed_array

; 373 		    Verbose(2, "DrvScan",
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE4
	mov	edx,offset FLAT:@CBE1
	mov	eax,02h
	call	Verbose
	mov	ecx,[ebp-014h];	ainfo
	add	esp,014h

; 379 	    }
@BLBL42:

; 365 	    for( j = 0; j < acnt; ++j )
	movzx	edi,di
	inc	edi
	cmp	di,si
	jb	@BLBL38
@BLBL37:

; 380 	    if( flt_changes == 1 )
	cmp	dword ptr [ebp-018h],01h;	flt_changes
	jne	@BLBL45

; 382 		isarray = TRUE;
	mov	dword ptr [ebp-010h],01h;	isarray

; 384 		break;
	jmp	@BLBL30
@BLBL45:

; 386 	    else if( flt_changes == 0 )
	cmp	dword ptr [ebp-018h],0h;	flt_changes
	je	@BLBL30

; 389 	    DosSleep(1000);		/* wait for peace :-) */
	push	03e8h

; 386 	    else if( flt_changes == 0 )
	mov	edi,ecx

; 389 	    DosSleep(1000);		/* wait for peace :-) */
	call	DosSleep
	add	esp,04h

; 355 	for( retry = 0; retry < 3; ++retry )
	mov	edx,[ebp-02ch];	retry
	inc	edx

; 389 	    DosSleep(1000);		/* wait for peace :-) */
	mov	ecx,edi

; 355 	for( retry = 0; retry < 3; ++retry )
	mov	[ebp-02ch],edx;	retry
	cmp	edx,03h
	jb	@BLBL31
@BLBL30:

; 391 	delete[] dummybuffer;
	mov	eax,[ebp-028h];	dummybuffer
	call	__vd__FPv

; 392     }
@BLBL29:

; 394     if( !isarray )
	cmp	dword ptr [ebp-010h],0h;	isarray
	jne	@BLBL51

; 400 	for( retry = 0; retry < 3; ++retry )
	mov	ebx,[ebp+018h];	phd

; 398 	PUCHAR		dummybuffer = new UCHAR[SECTOR_SIZE];
	mov	eax,0200h
	call	__vn__FUi

; 400 	for( retry = 0; retry < 3; ++retry )
	mov	si,[ebp+014h];	pcnt
	mov	ecx,[ebp-020h];	pinfo

; 397 	unsigned	flt_changes = 0;
	mov	dword ptr [ebp-01ch],0h;	flt_changes

; 400 	for( retry = 0; retry < 3; ++retry )
	mov	dword ptr [ebp-034h],0h;	retry

; 398 	PUCHAR		dummybuffer = new UCHAR[SECTOR_SIZE];
	mov	[ebp-030h],eax;	dummybuffer

; 400 	for( retry = 0; retry < 3; ++retry )
@BLBL53:

; 405 	    for( j = 0; j < pcnt; ++j )
	test	si,si
	jbe	@BLBL54
	mov	eax,ecx
	mov	[ebp-020h],eax;	pinfo
	xor	edi,edi
@BLBL55:

; 406 		rc = DriverPhysinfo(phd[j], pinfo[j]);
	movzx	eax,di
	mov	edx,dword ptr [ecx+eax*04h]
	mov	ax,word ptr [ebx+eax*02h]
	call	DriverPhysinfo__FUsP20_VRAID_PHYSINFO_DATA

; 405 	    for( j = 0; j < pcnt; ++j )
	movzx	edi,di
	inc	edi

; 406 		rc = DriverPhysinfo(phd[j], pinfo[j]);
	mov	ecx,[ebp-020h];	pinfo

; 405 	    for( j = 0; j < pcnt; ++j )
	cmp	di,si
	jb	@BLBL55
@BLBL54:

; 408 	    rc = newdev->read(0, 1, dummybuffer);
	push	dword ptr [ebp-030h];	dummybuffer
	mov	eax,[ebp+08h];	newdev

; 405 	    for( j = 0; j < pcnt; ++j )
	mov	[ebp-060h],esi;	@CBE141

; 408 	    rc = newdev->read(0, 1, dummybuffer);
	mov	edi,[eax]
	sub	esp,0ch
	mov	esi,[edi+01ch]
	add	eax,esi
	mov	esi,[ebp-060h];	@CBE141
	mov	ecx,01h
	xor	edx,edx
	call	dword ptr [edi+018h]
	mov	ecx,[ebp-020h];	pinfo
	add	esp,010h

; 410 	    for( j = 0; j < pcnt; ++j )
	test	si,si
	jbe	@BLBL59
	xor	edi,edi
	mov	[ebp+014h],si;	pcnt
@BLBL60:

; 412 		Verbose(3, "DrvScan", "Testing Physdev %u", j);
	mov	eax,offset FLAT:@CBE1
	movzx	esi,di
	push	esi
	mov	edx,eax
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE5
	mov	eax,03h
	call	Verbose

; 413 		rc = DriverPhysinfo(phd[j], &this_info);
	mov	ax,word ptr [ebx+esi*02h]
	lea	edx,[ebp-05ch];	this_info
	call	DriverPhysinfo__FUsP20_VRAID_PHYSINFO_DATA
	mov	ecx,[ebp-020h];	pinfo
	xchg	esi,eax
	add	esp,010h

; 414 		if( rc != 0 )
	test	esi,esi
	mov	[ebp-060h],esi;	@CBE141
	mov	esi,[ebp+014h];	pcnt

; 413 		rc = DriverPhysinfo(phd[j], &this_info);
	mov	edx,offset FLAT:@CBE1

; 414 		if( rc != 0 )
	je	@BLBL61
	mov	ecx,[ebp-060h];	@CBE141

; 415 		    Verbose(1, "DrvScan", "Physdev Info %u - rc %lu", j, rc);
	push	ecx
	mov	ecx,offset FLAT:@CBE6
	push	eax
	mov	eax,01h
	sub	esp,0ch
	call	Verbose
	mov	ecx,[ebp-020h];	pinfo
	add	esp,014h
	jmp	@BLBL64
@BLBL61:

; 416 		else if( this_info.reqcnt > pinfo[j]->reqcnt )
	movzx	edx,di
	mov	eax,dword ptr [ecx+edx*04h]
	mov	[ebp-060h],edx;	@CBE141
	mov	dx,[ebp-04fh];	this_info
	cmp	[eax+0dh],dx
	jae	@BLBL64

; 418 		    Verbose(2, "DrvScan",
	mov	eax,[ebp+08h];	newdev
	call	queryIndex__8VOS2DiskFv
	push	eax
	mov	edx,[ebp-060h];	@CBE141
	push	edx

; 421 		    ++flt_changes;
	inc	dword ptr [ebp-01ch];	flt_changes

; 418 		    Verbose(2, "DrvScan",
	sub	esp,0ch
	mov	ecx,offset FLAT:@CBE7
	mov	edx,offset FLAT:@CBE1
	mov	eax,02h
	call	Verbose
	mov	ecx,[ebp-020h];	pinfo
	add	esp,014h

; 424 	    }
@BLBL64:

; 410 	    for( j = 0; j < pcnt; ++j )
	movzx	edi,di
	inc	edi
	cmp	di,si
	jb	@BLBL60
@BLBL59:

; 425 	    if( flt_changes == 1 )
	cmp	dword ptr [ebp-01ch],01h;	flt_changes
	jne	@BLBL67

; 427 		isphys = TRUE;
	mov	dword ptr [ebp-024h],01h;	isphys

; 428 		break;
	jmp	@BLBL52
@BLBL67:

; 430 	    else if( flt_changes == 0 )
	cmp	dword ptr [ebp-01ch],0h;	flt_changes
	je	@BLBL52

; 433 	    DosSleep(1000);		/* wait for peace :-) */
	push	03e8h

; 430 	    else if( flt_changes == 0 )
	mov	edi,ecx

; 433 	    DosSleep(1000);		/* wait for peace :-) */
	call	DosSleep
	add	esp,04h

; 400 	for( retry = 0; retry < 3; ++retry )
	mov	edx,[ebp-034h];	retry
	inc	edx

; 433 	    DosSleep(1000);		/* wait for peace :-) */
	mov	ecx,edi

; 400 	for( retry = 0; retry < 3; ++retry )
	mov	[ebp-034h],edx;	retry
	cmp	edx,03h
	jb	@BLBL53
@BLBL52:

; 435 	delete[] dummybuffer;
	mov	eax,[ebp-030h];	dummybuffer
	call	__vd__FPv

; 436     }
@BLBL51:

; 438     if( isarray  ||  isphys )
	cmp	dword ptr [ebp-010h],0h;	isarray
	jne	@BLBL73
	cmp	dword ptr [ebp-024h],0h;	isphys
	je	@BLBL74
@BLBL73:

; 440 	if( isarray )
	cmp	dword ptr [ebp-010h],0h;	isarray
	je	@BLBL75

; 441 	    SetArrayInfo(ainfo[accessed_array]->id,
	mov	ebx,[ebp+08h];	newdev
	mov	ecx,[ebx]
	mov	eax,[ecx+0ch]
	add	eax,ebx
	call	dword ptr [ecx+08h]
	xchg	ebx,eax
	call	queryIndex__8VOS2DiskFv
	mov	edx,eax
	mov	eax,[ebp-014h];	ainfo
	movzx	edi,word ptr [ebp-03ah];	accessed_array
	mov	eax,dword ptr [eax+edi*04h]
	mov	ecx,ebx
	add	eax,02h
	call	SetArrayInfo__FPCUcCUlCUc

; 442 			 newdev->queryIndex(), newdev->isWritable());
@BLBL75:

; 445 	result = TRUE;				/* accessable through VRAID.FLT! */
	mov	eax,01h

; 446     }
	jmp	@BLBL76
@BLBL74:

; 449 	result = FALSE;
	xor	eax,eax

; 450     }
@BLBL76:
	mov	[ebp-038h],eax;	result

; 455     for( j = 0; j < acnt; ++j )
	mov	si,[ebp+0ch];	acnt
	test	si,si
	jbe	@BLBL77
	mov	ebx,[ebp-014h];	ainfo
	xor	edi,edi
@BLBL78:

; 456 	delete ainfo[j];
	movzx	eax,di
	mov	eax,dword ptr [ebx+eax*04h]
	call	__dl__FPv

; 455     for( j = 0; j < acnt; ++j )
	movzx	edi,di
	inc	edi
	cmp	di,si
	jb	@BLBL78
@BLBL77:

; 457     delete[] ainfo;
	mov	eax,[ebp-014h];	ainfo
	call	__vd__FPv

; 458     for( j = 0; j < pcnt; ++j )
	mov	si,[ebp+014h];	pcnt
	test	si,si
	jbe	@BLBL82
	mov	ebx,[ebp-020h];	pinfo
	xor	edi,edi
@BLBL83:

; 459 	delete pinfo[j];
	movzx	eax,di
	mov	eax,dword ptr [ebx+eax*04h]
	call	__dl__FPv

; 458     for( j = 0; j < pcnt; ++j )
	movzx	edi,di
	inc	edi
	cmp	di,si
	jb	@BLBL83
@BLBL82:

; 460     delete[] pinfo;
	mov	eax,[ebp-020h];	pinfo
	call	__vd__FPv

; 462     return result;
	mov	eax,[ebp-038h];	result
	add	esp,060h
	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret	
IsProvidedByFlt__FCP8VOS2DiskUsPUsT2T3	endp

; 282 PRIVATE struct _OS2DEVID *

QueryArrayInfo__FCPA6_CUc	proc
	push	ebx

; 287     for( i = 0; i < MAXDRIVES; ++i )
	mov	ebx,eax

; 282 PRIVATE struct _OS2DEVID *
	push	edi

; 287     for( i = 0; i < MAXDRIVES; ++i )
	xor	edi,edi

; 282 PRIVATE struct _OS2DEVID *
	push	esi

; 287     for( i = 0; i < MAXDRIVES; ++i )
	xor	esi,esi

; 282 PRIVATE struct _OS2DEVID *
	sub	esp,010h

; 287     for( i = 0; i < MAXDRIVES; ++i )
	mov	[esp+020h],eax;	array_devid
@BLBL12:

; 288 	if( ahsOs2Devid[i].fl != 0
	lea	ecx,dword ptr [edi+ @3ahsOs2Devid]
	cmp	byte ptr [ecx+04h],0h;	@3ahsOs2Devid
	je	@BLBL14
	lea	eax,[ecx+06h]
	mov	[esp+0ch],ecx;	@CBE143
	mov	edx,ebx
	mov	ecx,04h
	call	memcmp
	mov	ecx,eax
	mov	eax,[esp+0ch];	@CBE143
	test	ecx,ecx
	jne	@BLBL14

; 291 	    return &ahsOs2Devid[i];
	add	esp,010h
	pop	esi
	pop	edi
	pop	ebx
	ret	
@BLBL14:

; 287     for( i = 0; i < MAXDRIVES; ++i )
	inc	esi
	add	edi,0ch
	cmp	esi,040h
	jb	@BLBL12

; 294     return NULL;
	xor	eax,eax
	add	esp,010h
	pop	esi
	pop	edi
	pop	ebx
	ret	
QueryArrayInfo__FCPA6_CUc	endp

; 246 SetArrayInfo(DEVID const array_devid,ULONG const os2idx,UCHAR const avail)

SetArrayInfo__FPCUcCUlCUc	proc
	push	ebx
	push	edi

; 250     for( i = 0; i < MAXDRIVES; ++i )
	xor	edi,edi

; 246 SetArrayInfo(DEVID const array_devid,ULONG const os2idx,UCHAR const avail)
	push	esi

; 250     for( i = 0; i < MAXDRIVES; ++i )
	xor	esi,esi

; 246 SetArrayInfo(DEVID const array_devid,ULONG const os2idx,UCHAR const avail)
	sub	esp,0ch
	mov	[esp+024h],cl;	avail

; 250     for( i = 0; i < MAXDRIVES; ++i )
@BLBL4:

; 251 	if( ahsOs2Devid[i].fl == 0 )
	lea	ebx,dword ptr [edi+ @3ahsOs2Devid]
	cmp	byte ptr [ebx+04h],0h;	@3ahsOs2Devid
	jne	@BLBL7

; 254 	    ahsOs2Devid[i].os2idx = os2idx;
	mov	[ebx],edx

; 255 	    memcpy(ahsOs2Devid[i].id, array_devid, sizeof(array_devid));
	mov	edx,eax

; 253 	    ahsOs2Devid[i].fl = 1;
	mov	byte ptr [ebx+04h],01h

; 255 	    memcpy(ahsOs2Devid[i].id, array_devid, sizeof(array_devid));
	mov	ecx,04h
	lea	eax,[ebx+06h]
	call	memcpy

; 256 	    ahsOs2Devid[i].avail = avail;
	mov	cl,[esp+024h];	avail
	mov	[ebx+05h],cl

; 257 	    break;
	add	esp,0ch
	pop	esi
	pop	edi
	pop	ebx
	ret	
@BLBL7:

; 250     for( i = 0; i < MAXDRIVES; ++i )
	inc	esi
	add	edi,0ch
	cmp	esi,040h
	jb	@BLBL4

; 260     return;
	add	esp,0ch
	pop	esi
	pop	edi
	pop	ebx
	ret	
SetArrayInfo__FPCUcCUlCUc	endp
CODE32	ends
EH_CODE	segment

__dftdt__9PartTableFv	proc
	sub	esp,08h
	mov	edx,02h
	call	__dt__9PartTableFv
	add	esp,08h
	ret	
__dftdt__9PartTableFv	endp
EH_CODE	ends
end
