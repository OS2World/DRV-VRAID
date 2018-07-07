# -*- Mode: makefile -*-
# $Source: d:/source/driver/perf/RCS/makefile,v $
#
# $Revision: 1.5 $
# $Date: 1998/03/04 02:13:51 $
# $Locker:  $
#
# Driver Name: SCSOpt.flt - modify performance related mode pages
#
# Usage: DMake [debug] [all]
#
# $Log: makefile,v $
# Revision 1.5  1998/03/04 02:13:51  vitus
# - changed DDK location
# - new version 0.71
#
# Revision 1.4  1997/12/16 02:28:17  vitus
# - renamed driver
# - added generation of SCSIOpt.exe
#
# Revision 1.3  1997/12/08 23:06:34  vitus
# - removed traces from DSKSleep and LOCKDRV
#
# Revision 1.2  1997/12/06 01:01:54  vitus
# - enabled support for WatCom C
#
# Revision 1.1  1997/10/28 02:10:24  vitus
# Initial revision
#


# Set defaults if not passed on commandline
#
LIST *= 0

VERSION = 0
SUBVERSION = 71




# Locations w/ includes, libs and tools
#
DDK = D:\Ddk\Base
DDKTOOLS = D:\Ddk\Tools


INC    = $(DDK)\Inc
H      = $(DDK)\H
LIB    = $(DDK)\Lib
DISKH  = $(DDK)\Src\Dev\dasd\Diskh
DHLIB  = $(DDK)\Src\Dev\Dasd\Devhelp
ADDLIB = $(DDK)\Src\Dev\Dasd\Addcalls
HFLAGS= -d -f -w -s


# Compiler and other tools
# Compiler are assumed to be installed so that they can
# compile applications (header, libraries, etc).  Special
# settings for driver development will be accounted for.
# If environment variable WATCOM isn't set we assume MicroSoft C 5.1
# or higher (tested with 6.0).

.IF $(WATCOM) == $(NULL)
CC = cl -nologo -c
AS = ml -nologo -c
.ELSE
CC = wcc -zq
AS =  $(DDKTOOLS)\Alp -Mb
.ENDIF
LINK = $(DDK)\Tools\Link /ba
MAPSYM = mapsym
H2INC = h2inc


# Compiler and Linker Options

.IF $(WATCOM) == $(NULL)
AINC	= -I$(INC)
AFLAGS	= -Cp
ADFLAGS	= -Zm -Cp -DDEBUG
CINC	= -I$(H) -I$(DISKH)
CDEF	=  -DVERSION=$(VERSION) -DSUBVERSION=$(SUBVERSION)
CFLAGS	= -Zp1 -G2ms -ASw -W3 -Ox
CDFLAGS	= -Zp1 -G2ms -ASw -W3 -DDEBUG -Oi

LIBS	= $(DHLIB)\Dhcalls.lib $(ADDLIB)\Addcalls.lib

.ELSE
AINC	= -I:$(INC)
AFLAGS	= -D:WATCOMC
ADFLAGS	= -D:WATCOMC -D:DEBUG
CINC	= -i=$(H) -i=$(DISKH)
CDEF	= -dVERSION=$(VERSION) -dSUBVERSION=$(SUBVERSION)
CFLAGS	= -ms -zu -zdp -zff -zgf -5 -oxs -s -wx -zl -zp1 -d1+
CDFLAGS	= -ms -zu -zdp -zff -zgf -zc -3 -oi -s -wx -zl -zp1 -hc -d1+ -DDEBUG

LIBS	= 

.ENDIF

LFLAGS	= /map /nod /exepack /packd /a:16 /far
RFLAGS	= -r



#****************************************************************************
# Set up Macros that will contain all the different dependencies for the
# executables and dlls etc. that are generated.
#****************************************************************************


# OBJ files
#
OBJ1 =  o\Startup.obj o\data.obj o\Str1.obj
OBJ2 =  o\Ioctl.obj o\Init1.obj o\Printk.obj
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
CLIB =	anulrem.obj anfauldi.obj anuldiv.obj
.ELSE
CLIB =	i4d.obj
.ENDIF



#****************************************************************************
#   Setup the inference rules for compiling and assembling source code to
#   obejct code.
#****************************************************************************


.IF $(WATCOM) == $(NULL)
o\%.obj : %.asm
	$(AS) $(AFLAGS) $(AINC) -Fllist\$*.cod -Fo$@ $*.asm

o\%.dbj : %.asm
	$(AS) $(ADFLAGS) $(AINC) -Fllist\$*.dod -Fo$@ $*.asm

o\%.obj : %.c 'scsiopt.h' 'proto.h' 'extern.h'
	$(CC) $(CFLAGS) $(CINC) -Fllist\$*.cod -Fo$@ $*.c

o\%.dbj : %.c 'scsiopt.h' 'proto.h' 'extern.h'
	$(CC) $(CDFLAGS) $(CINC) -Fllist\$*.dod -Fo$@ $*.c

.ELSE

o\%.obj : %.asm
	$(AS) $(AFLAGS) $(AINC) $*.asm -Fl:list\$*.cod -Fo:$@

o\%.dbj : %.asm
	$(AS) $(ADFLAGS) $(AINC) $*.asm -Fl:list\$*.dod -Fo:$@

o\%.obj : %.c 'scsiopt.h' 'proto.h' 'extern.h'
	$(CC) $(CFLAGS) $(CINC) $(CDEF) -fo=$@ $*.c
	wdisasm -b -l=list\$*.cod -s=$*.c $@

o\%.dbj : %.c 'scsiopt.h' 'proto.h' 'extern.h'
	$(CC) $(CDFLAGS) $(CINC) $(CDEF) -fo=$@ $*.c
	wdisasm -b -l=list\$*.dod -s=$*.c $@
.ENDIF

%.inc : %.h
	$(H2INC) $(HFLAGS) $*.h -o $*.inc




#****************************************************************************
#   Target Information
#****************************************************************************

# to be scanned by bldlevel.exe (but doesn't work?)

FILEVER = @\#Vitus Jensen:0.70\#@



# Should be the default tag for all general processing

.IF $(WATCOM) == $(NULL)
all: scsiopt.flt
.ELSE
all: scsiopt.flt scsiopt.exe
.ENDIF

debug: scsioptd.flt

scsiopt.exe: scsiopt.c scsioptpub.h
	wcl386 -zq -bt=OS2 -oax -i=$(WATCOM)\H\OS2 scsiopt.c


clean:
	+if exist *.lnk		del *.lnk
	+if exist o\*.obj	del o\*.obj
	+if exist o\*.dbj	del o\*.dbj
	+if exist list\*.cod	del list\*.cod
	+if exist list\*.dod	del list\*.dod
	+if exist *.sym		del *.sym
	+if exist *.sys		del *.sys
	+if exist *.tff		del *.tff


#****************************************************************************
#   OBJ dependant  rules for compiling and assembling source code
#****************************************************************************

o\Cmdparse.obj o\Cmdparse.dbj : cmdparse.c cmdproto.h \
	cmdphdr.h cmdparse.h cmdpdefs.h

o\Init1.obj o\Init1.dbj : init1.c modepages.h cmdphdr.h cmdparse.h cmdpdefs.h

o\Ioctl.obj o\Ioctl.dbj : ioctl.c scsioptpub.h





#*****************************************************************************
#   Linker commands
#*****************************************************************************


scsiopt.flt: $(OBJS) $(LIBS) makefile
	$(LINK) $(LFLAGS) @$(mktmp,$*.lnk	\
		$(OBJ1) +			\n\
		$(CLIB) +			\n\
		$(OBJ2) +			\n\
		$(OBJ3)				\n\
		$*.flt				\n\
		$*.map				\n\
		$(LIBS)				\n\
		$(mktmp,$*.def			\
			LIBRARY SCSIOpt		\n\
			DESCRIPTION "$(FILEVER) SCSI Optimizer" \n\
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
	$(MAPSYM) $*.map


scsioptd.flt: $(DBJS) $(LIBS) makefile
	$(LINK) $(LFLAGS) @$(mktmp,$*.lnk	\
		$(DBJ1) +			\n\
		$(CLIB) +			\n\
		$(DBJ2) +			\n\
		$(DBJ3)				\n\
		$*.flt				\n\
		$*.map				\n\
		$(LIBS)				\n\
		$(mktmp,$*.def			\
			LIBRARY SCSIOpt		\n\
			DESCRIPTION "$(FILEVER) SCSI Optimizer" \n\
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
	$(MAPSYM) $*.map


#****************************************************************************
