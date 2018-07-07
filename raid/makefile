# -*- makefile -*-
# $Source: R:/source/driver/raid/RCS\\makefile,v $
#
# $Revision: 2.7.1.1 $
# $Date: 2003/11/14 00:39:16 $
# $Locker:  $
#
#	Driver Name: VRAID.FLT - Virtual RAID
#
#	Usage: DMake [debug] [all]
#
# History:
# $Log: makefile,v $
# Revision 2.7.1.1  2003/11/14 00:39:16  vitus
# - defines NORAID5 (rebuilding old version)
# - removed Addcalls.lib, not needed
# - increased FILEVER to 0.85
#
# Revision 2.7  2001/06/26 01:55:27  vitus
# - corrected ../dprintf.asm assembler options
#
# Revision 2.6  2001/06/25 00:34:43  vitus
# - added rule to translate ../dprintf.asm
#
# Revision 2.5  2001/04/24 04:17:26  vitus
# - moved general obj files to directory '..'
# - moved DDK to R:\dev\DDK
#
# Revision 2.4  2000/06/15 01:06:27  vitus
# - all RAID level moduls refer to vraidpub.h
#
# Revision 2.3  1999/11/16 22:57:33  vitus
# - obs, there was still a reference to vraid.inf...
#
# Revision 2.2  1999/11/16 22:11:41  vitus
# - removed creation of documentation (see subdir DOC)
# - debugging binary contains line numbers
#
# Revision 2.1  1999/09/23 23:09:25  vitus
# - changed to MS-C/C++ 7 (compiler options)
# - added FILTER.OBJ
#
# Revision 1.12  1999/08/09 23:23:53  vitus
# - changed CFLAGS for WatCom C to use less optimizations (remember the "two far
#   pointers pushed as one far/one near" bug?).  Last change to enable WatCom
#   usage: compiler is out of production!
#
# Revision 1.11  1999/07/08 01:43:04  vitus
# - buildlevel 0.71
# - added option -of for WatCom C: debugger useless w/o
# - added segment WORKMEM, no class for WatCom C
#
# Revision 1.10  1999/06/20 17:44:47  vitus
# - buildlevel 0.70
# - creates online documentation
#
# Revision 1.9  1999/05/12 01:46:38  vitus
# - debug version now uses codeview debug info for icat
#
# Revision 1.8  1999/04/21 00:30:59  vitus
# - added BUILD.OBJ
#
# Revision 1.7  1998/10/07 23:35:44  vitus
# - removed VERSION/SUBVERSION defines
# - bldlevel 0.50
#
# Revision 1.6  1998/09/20 15:26:14  vitus
# - version 0.40
# - correct bldlevel signature
# - driver name now VRaid
#
# Revision 1.5  1998/06/26 22:55:38  vitus
# - corrected segment names/attributes
# - added physdev.obj (OBJ1-4)
# - used with MS-C 6.00A
#
# Revision 1.4  1998/05/29 01:49:35  vitus
# - added i4m.obj
# - version now 0.20
# - DDK path changed
#
# Revision 1.3  1998/04/08 01:12:22  vitus
# - added more include files
# - sorted OBJ files in driver
#
# Revision 1.2  1998/03/10 02:44:25  vitus
# - added span.c
# - added dsklayout.h
#
# Revision 1.1  1998/02/17 23:37:00  vitus
# Initial revision
# ---------------------------------------------
# This code is Copyright Vitus Jensen 1997-2001
# 
LIST *= 0			# defaults if not passed on commandline



# Locations w/ includes, libs and tools
#
DDK = R:\Dev\Ddk


INC    = $(DDK)\Inc
H      = $(DDK)\H
LIB    = $(DDK)\Lib
DDKTOOLS = D:\Ddk\Tools
DISKH  = $(DDK)\Src\Dev\Dasd\Diskh
DHLIB  = $(DDK)\Src\Dev\Dasd\Devhelp
ADDLIB = $(DDK)\Src\Dev\Dasd\Addcalls
HFLAGS= -d -f -w -s


# Compiler and other tools
# Compiler are assumed to be installed so that they can
# compile applications (header, libraries, etc).  Special
# settings for driver development will be accounted for.

.IF $(WATCOM) == $(NULL)
CC = cl -nologo -c
AS = ml -nologo -c
.ELSE
CC = wcc -zq
AS =  alp -Mb
.ENDIF
LINK = $(DDK)\Tools\Link /ba
MAPSYM = $(DDK)\Tools\Mapsym
H2INC = h2inc


# Compiler and Linker Options

CDEF	= -DNORAID5

.IF $(WATCOM) == $(NULL)
AINC	= -I$(INC)
AFLAGS	= -Cp
ADFLAGS	= -Zm -Cp -Zi -DDEBUG
CINC	= -I$(H) -I$(DISKH)
CFLAGS	= -Zelp1 -ASw -G2s -W3 -Orx
CDFLAGS	= -Zelp1 -ASw -G2s -W3 -Ori -Zi -DDEBUG

LIBS	= $(DHLIB)\Dhcalls.lib

.ELSE
AINC	= -I:$(INC)
AFLAGS	= -D:WATCOMC
ADFLAGS	= -D:WATCOMC -D:DEBUG
CINC	= -i=$(H) -i=$(DISKH)
CFLAGS	= -ms -zu -zdp -zff -zgf -5 -ofmirs -s -wx -zl -zp1 -d1+
CDFLAGS	= -ms -zu -zdp -zff -zgf -zc -5 -ofmi -s -wx -zl -zp1 -hc -d1+ -DDEBUG

LIBS	= 

.ENDIF

LFLAGS	= /map /nod /exepack /packd /a:16 /far
LDFLAGS	= /map /nod /exepack /packd /a:16 /co
RFLAGS	= -r




#****************************************************************************
# Set up Macros that will contain all the different dependencies for the
# executables and dlls etc. that are generated.
#****************************************************************************


# OBJ files
#
OBJ1 = o\startup.obj o\str1.obj o\memory.obj o\Vrio.obj
OBJ2 = o\iorb.obj o\Filter.obj o\Build.obj o\ioctl.obj
OBJ3 = o\physdev.obj o\mirror.obj o\stripe.obj o\span.obj o\single.obj
OBJ4 = o\data.obj o\init2.obj o\init1.obj o\printk.obj o\cmdparse.obj

#       Debug files
#
DBJ1 = $(OBJ1:s/obj/dbj/) o\Dprintf.dbj
DBJ2 = $(OBJ2:s/obj/dbj/)
DBJ3 = $(OBJ3:s/obj/dbj/)
DBJ4 = $(OBJ4:s/obj/dbj/)

GEN_CH = os2intf.h vraid.h dsklayout.h devstructs.h proto.h extern.h
PARSER_CH = cmdparse.h cmdpdefs.h


# Complete lists
#
OBJS =	$(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4)
DBJS =	$(DBJ1) $(DBJ2) $(DBJ3) $(DBJ4)

.IF $(WATCOM) == $(NULL)
CLIB =	..\Anulrem.obj ..\Anfauldi.obj ..\Anuldiv.obj ..\Anlmul.obj
.ELSE
CLIB =	..\I4d.obj ..\I4m.obj ..\Fstrncpy.obj
.ENDIF



#****************************************************************************
#   Setup the inference rules for compiling and assembling source code to
#   obejct code.
#****************************************************************************


.IF $(WATCOM) == $(NULL)
o\%.dbj : ..\%.asm
	$(AS) $(ADFLAGS) $(AINC) -Fllist\$*.dod -Fo$@ ..\$*.asm

o\%.obj : %.asm 'segments.inc'
	$(AS) $(AFLAGS) $(AINC) -Fllist\$*.cod -Fo$@ $*.asm

o\%.dbj : %.asm 'segments.inc'
	$(AS) $(ADFLAGS) $(AINC) -Fllist\$*.dod -Fo$@ $*.asm

o\%.obj : %.c '{$(GEN_CH)}'
	$(CC) $(CFLAGS) $(CINC) $(CDEF) -Fllist\$*.cod -Fo$@ $*.c

o\%.dbj : %.c '{$(GEN_CH)}'
	$(CC) $(CDFLAGS) $(CINC) $(CDEF) -Fllist\$*.dod -Fo$@ $*.c

.ELSE

o\%.obj : %.asm 'segments.inc'
	$(AS) $(AFLAGS) $(AINC) $*.asm -Fl:list\$*.cod -Fo:$@

o\%.dbj : %.asm 'segments.inc'
	$(AS) $(ADFLAGS) $(AINC) $*.asm -Fl:list\$*.dod -Fo:$@

o\%.obj : %.c '{$(GEN_CH)}'
	$(CC) $(CFLAGS) $(CINC) $(CDEF) -fo=$@ $*.c
	wdisasm -b -l=list\$*.cod -s=$*.c $@

o\%.dbj : %.c '{$(GEN_CH)}'
	$(CC) $(CDFLAGS) $(CINC) $(CDEF) -fo=$@ $*.c
	wdisasm -b -l=list\$*.dod -s=$*.c $@
.ENDIF

%.inc : %.h
	$(H2INC) $(HFLAGS) $*.h -o $*.inc




#****************************************************************************
#   Target Information
#****************************************************************************
#

FILEVER = @\043Vitus Jensen:0.85\043@


# Should be the default tag for all general processing

all: vraid.flt

debug: vraidd.flt


clean:
	+if exist *.map		del *.map
	+if exist *.sym		del *.sym
	+if exist *.sys		del *.sys
	+if exist *.add		del *.add
	+if exist *.flt		del *.flt
	+if exist o\*.obj	del o\*.obj
	+if exist o\*.dbj	del o\*.dbj
	+if exist list\*.cod	del list\*.cod
	+if exist list\*.dod	del list\*.dod



#****************************************************************************
#   OBJ dependant  rules for compiling and assembling source code
#****************************************************************************

o\cmdparse.obj o\cmdparse.dbj : cmdparse.c $(PARSER_CH) cmdproto.h cmdphdr.h

o\init1.obj o\init1.dbj : cmdphdr.h $(PARSER_CH) dsklayout.h

o\ioctl.obj o\ioctl.dbj : ioctl.c vraidpub.h

o\single.obj o\single.dbj : vraidpub.h

o\span.obj o\span.dbj : vraidpub.h

o\stripe.obj o\stripe.dbj : vraidpub.h

o\mirror.obj o\mirror.dbj : vraidpub.h



#*****************************************************************************
#   Linker commands
#*****************************************************************************


vraid.flt: $(OBJS) $(LIBS) makefile
	$(LINK) $(LFLAGS) @$(mktmp,$*.lnk	\
		$(OBJ1) +			\n\
		$(CLIB) +			\n\
		$(OBJ2) $(OBJ3) $(OBJ4)		\n\
		$*.flt				\n\
		$*.map				\n\
		$(LIBS)				\n\
		$(mktmp,$*.def			\
			LIBRARY VRAID		\n\
			DESCRIPTION "$(FILEVER) Visual RAID Filter <g>"	\n\
			PROTMODE			\n\
			SEGMENTS			\n\
			 DDHeader	CLASS 'CONST'	\n\
			 CONST		CLASS 'CONST'	\n\
			 CONST		CLASS 'DATA'	\n\
			 CONST2		CLASS 'DATA'	\n\
			 LIBDATA	CLASS 'DATA'	\n\
			 _DATA		CLASS 'DATA'	\n\
			 _BSS		CLASS 'BSS'	\n\
			 LIBCODE	CLASS 'CODE'	\n\
			 'Code'		CLASS 'CODE'	\n\
			 _TEXT		CLASS 'CODE'	\n\
			 WORKMEM	CLASS 'FAR_DATA' IOPL	\n\
			 SwapData	CLASS 'FAR_DATA' IOPL	\n\
			 INITDATA	CLASS 'FAR_DATA'\n); )
	$(MAPSYM) $*.map >NUL


vraidd.flt: $(DBJS) $(LIBS) makefile
	$(LINK) $(LDFLAGS) @$(mktmp,$*.lnk	\
		$(DBJ1) +			\n\
		$(CLIB) +			\n\
		$(DBJ2) $(DBJ3) $(DBJ4)		\n\
		$*.flt				\n\
		$*.map				\n\
		$(LIBS)				\n\
		$(mktmp,$*.def			\
			LIBRARY VRAID		\n\
			DESCRIPTION "$(FILEVER) Visual RAID Filter <g>"	\n\
			PROTMODE			\n\
			SEGMENTS			\n\
			 DDHeader	CLASS 'CONST'	\n\
			 CONST		CLASS 'CONST'	\n\
			 CONST		CLASS 'DATA'	\n\
			 CONST2		CLASS 'DATA'	\n\
			 LIBDATA	CLASS 'DATA'	\n\
			 _DATA		CLASS 'DATA'	\n\
			 _BSS		CLASS 'BSS'	\n\
			 LIBCODE	CLASS 'CODE'	\n\
			 'Code'		CLASS 'CODE'	\n\
			 _TEXT		CLASS 'CODE'	\n\
			 WORKMEM	CLASS 'FAR_DATA' IOPL	\n\
			 SwapData	CLASS 'FAR_DATA' IOPL	\n\
			 INITDATA	CLASS 'FAR_DATA'\n); )
	$(MAPSYM) $*.map >NUL

