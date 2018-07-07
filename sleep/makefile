# -*- makefile -*-
# $Source: R:/source/driver/sleep/RCS/makefile,v $
#
# $Revision: 2.3 $
# $Date: 2001/02/27 02:35:12 $
# $Locker:  $
#
# Driver Name: DSKSLEEP.FLT - Stop disks when unused
#
# Usage: DMake [debug] [all]
#
# History: see bottom of file
# ------------------------------------------------
# This code is Copyright Vitus Jensen 1996-99,2001
#

# Set defaults if not passed on commandline
#
LIST *= 0



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
APMLIB = $(DDK)\Src\Dev\Thinkpad\DockII\Apmcalls
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
LINK = $(DDK)\TOOLS\LINK /ba
MAPSYM = mapsym
H2INC = h2inc


# Compiler and Linker Options

.IF $(WATCOM) == $(NULL)
AINC	= -I$(INC)
AFLAGS	= -Cp
ADFLAGS	= -Cp -Zm -Zi -DDEBUG
CINC	= -I$(H) -I$(DISKH) -I$(APMLIB)
CDEF	=
CFLAGS	= -Zelp1 -G2s -ASw -W3 -Orx
CDFLAGS	= -Zelp1 -G2s -ASw -W3 -Ori -Zi -DDEBUG

LIBS	= $(DHLIB)\Dhcalls.lib $(ADDLIB)\Addcalls.lib $(APMLIB)\Apmcalls.lib

.ELSE
AINC	= -I:$(INC)
AFLAGS	= -D:WATCOMC
ADFLAGS	= -D:WATCOMC -D:DEBUG
CINC	= -i=$(H) -i=$(DISKH) -i=$(APMLIB)
CDEF	=
CFLAGS	= -ms -zu -zdp -zff -zgf -5 -oxs -s -wx -zl -zp1 -d1+
CDFLAGS	= -ms -zu -zdp -zff -zgf -zc -3 -oi -s -wx -zl -zp1 -hc -d1+ -DDEBUG

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
OBJ1 =  o\Segments.obj o\Data.obj o\Str1.obj
OBJ2 =  o\Iorb.obj o\Ioctl.obj o\Init1.obj o\Printk.obj
OBJ3 =	o\Cmdparse.obj

#       Debug files
#
DBJ1 =	$(OBJ1:s/obj/dbj/) o\Dprintf.dbj
DBJ2 =	$(OBJ2:s/obj/dbj/)
DBJ3 =	$(OBJ3:s/obj/dbj/)

# Complete lists
#
OBJS =	$(OBJ1) $(OBJ2) $(OBJ3)
DBJS =	$(DBJ1) $(DBJ2) $(DBJ3)

.IF $(WATCOM) == $(NULL)
CLIB =	..\Anulrem.obj ..\Anfauldi.obj ..\Anuldiv.obj
.ELSE
CLIB =	..\I4d.obj
.ENDIF



#****************************************************************************
#   Setup the inference rules for compiling and assembling source code to
#   obejct code.
#****************************************************************************


.IF $(WATCOM) == $(NULL)
o\%.obj : ..\%.asm
	$(AS) $(AFLAGS) $(AINC) -Fllist\$*.cod -Fo$@ ..\$*.asm

o\%.dbj : ..\%.asm
	$(AS) $(ADFLAGS) $(AINC) -Fllist\$*.dod -Fo$@ ..\$*.asm

o\%.obj : %.asm 'segments.inc'
	$(AS) $(AFLAGS) $(AINC) -Fllist\$*.cod -Fo$@ $*.asm

o\%.dbj : %.asm 'segments.inc'
	$(AS) $(ADFLAGS) $(AINC) -Fllist\$*.dod -Fo$@ $*.asm

o\%.obj : %.c 'dsksleep.h' 'proto.h' 'extern.h'
	$(CC) $(CFLAGS) $(CINC) $(CDEF) -Fllist\$*.cod -Fo$@ $*.c

o\%.dbj : %.c 'dsksleep.h' 'proto.h' 'extern.h'
	$(CC) $(CDFLAGS) $(CINC) $(CDEF) -Fllist\$*.dod -Fo$@ $*.c

.ELSE

o\%.obj : %.asm 'segments.inc'
	$(AS) $(AFLAGS) $(AINC) $*.asm -Fl:list\$*.cod -Fo:$@

o\%.dbj : %.asm 'segments.inc'
	$(AS) $(ADFLAGS) $(AINC) $*.asm -Fl:list\$*.dod -Fo:$@

o\%.obj : %.c 'dsksleep.h' 'proto.h' 'extern.h'
	$(CC) $(CFLAGS) $(CINC) $(CDEF) -fo=$@ $*.c
	wdisasm -b -l=list\$*.cod -s=$*.c $@

o\%.dbj : %.c 'dsksleep.h' 'proto.h' 'extern.h'
	$(CC) $(CDFLAGS) $(CINC) $(CDEF) -fo=$@ $*.c
	wdisasm -b -l=list\$*.dod -s=$*.c $@
.ENDIF

%.inc : %.h
	$(H2INC) $(HFLAGS) $*.h -o $*.inc




#****************************************************************************
#   Target Information
#****************************************************************************
#
# This is a very important step. The following small amount of code MUST
# NOT be removed from the program. The following directive will do
# dependency checking every time this component is built UNLESS the
# following is performed:
#                    A specific tag is used -- ie. all
#
# This allows the developer as well as the B & I group to perform incremental
# build with a degree of accuracy that has not been used before.
# There are some instances where certain types of INCLUDE files must be
# created first. This type of format will allow the developer to require
# that file to be created first. In order to achive that, all that has to
# be done is to make the DEPEND.MAK tag have your required target. Below is
# an example:
#
#    depend.mak:   { your file(s) } dephold
#
# Please DON'T remove the following line
#

FILEVER = @\043Vitus Jensen:0.98\043@


# Should be the default tag for all general processing

all: dsksleep.flt

debug: dskslpd.flt


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

o\Cmdparse.obj o\Cmdparse.dbj : cmdparse.c cmdproto.h cmdphdr.h cmdparse.h cmdpdefs.h

o\Init1.obj o\Init1.dbj : init1.c cmdphdr.h cmdparse.h cmdpdefs.h

o\Ioctl.obj o\Ioctl.dbj : ioctl.c dskslpub.h




#*****************************************************************************
#   Linker commands
#*****************************************************************************


dsksleep.flt: $(OBJS) $(LIBS) makefile
	$(LINK) $(LFLAGS) @$(mktmp,$*.lnk	\
		$(OBJ1) +			\n\
		$(CLIB) +			\n\
		$(OBJ2) $(OBJ3)			\n\
		$*.flt				\n\
		$*.map				\n\
		$(LIBS)				\n\
		$(mktmp,$*.def			\
			LIBRARY DSKSLEEP	\n\
			DESCRIPTION "$(FILEVER) Disk Sleep Filter"	\n\
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
			 SwapData	CLASS 'FAR_DATA' IOPL	\n\
			 INITDATA	CLASS 'FAR_DATA'\n); )
	$(MAPSYM) $*.map >nul


dskslpd.flt: $(DBJS) $(LIBS) makefile
	$(LINK) $(LDFLAGS) @$(mktmp,$*.lnk	\
		$(DBJ1) +			\n\
		$(CLIB) +			\n\
		$(DBJ2) $(DBJ3)			\n\
		$*.flt				\n\
		$*.map				\n\
		$(LIBS)				\n\
		$(mktmp,$*.def			\
			LIBRARY DSKSLEEP	\n\
			DESCRIPTION "$(FILEVER) Disk Sleep Filter"	\n\
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
			 SwapData	CLASS 'FAR_DATA' IOPL	\n\
			 INITDATA	CLASS 'FAR_DATA'\n); )
	$(MAPSYM) $*.map >nul


#****************************************************************************
# History:
#
# $Log: makefile,v $
# Revision 2.3  2001/02/27 02:35:12  vitus
# - moved general obj files to directory '..'
#
# Revision 2.2  2001/02/21 23:19:52  vitus
# - moved DDK to R:\dev\DDK
# - changed version to 0.98
# - slightly changed assembler, compiler and linker options.  Now the same
#   as in vraid
#
# Revision 2.1  1999/11/29 00:46:33  vitus
# - filenames are again mixed case
# - usable with M$ C 7.0
#
# Revision 1.13  1999/11/28 23:51:35  vitus
# - more changes for WatCom C
# - moved DDK to D:\DDK
# - moved version no to dsksleep.h
#
# Revision 1.12  1997/12/05 01:52:14  vitus
# - modified to be usable for MSC and WatCom C
#
# Revision 1.11  1997/11/23 22:38:14  vitus
# - changed version to 0.96
#
# Revision 1.10  1997/11/22 01:07:14  vitus
# - changed version to 0.95
#
# Revision 1.9  1997/10/24 23:31:41  vitus
# -  ew DDK (i ter et)
# - 0.93
#
# Revision 1.8  1997/09/03 01:19:09  vitus
# Version 0.93 (internal, U.Koziolek)
#
# Revision 1.7  1997/05/11 14:50:50  vitus
# Version 0.90
# Switched characters behind backslash to uppercase
#
# Revision 1.6  1997/04/08 02:18:32  vitus
# Version 0.86
# Moved OBJs to subdirectory o
#
# Revision 1.5  1997/03/05 22:25:56  vitus
# Corrected depencies in implicit rules
# Added SwapData segment to linker command
# Version 0.85
#
# Revision 1.4  1997/02/06 01:33:09  vitus
# Added ioctl.c
# Changed version to 0.80
#
# Revision 1.3  1996/10/25 00:38:48  vitus
# Different include file depency
# Changed version number to 0.60
#
# Revision 1.2  1996/09/30 00:28:13  vitus
# Added cmdparse.c
#
# Revision 1.1  1996/09/27 03:47:32  vitus
# Initial revision
#
