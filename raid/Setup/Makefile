#*
#* $Source: R:/source/driver/raid/setup/RCS/Makefile,v $
#* $Revision: 1.15 $
#* $Date: 2000/08/17 02:24:35 $
#* $Locker: vitus $
#*
#*	Create tests, samples, setup programs.
#*
#* $Log: Makefile,v $
#* Revision 1.15  2000/08/17 02:24:35  vitus
#* - added drverify.c, .h
#* - added VSETUP_RC to conatin the includes from vsetup.rc
#*
#* Revision 1.14  2000/05/28 17:27:49  vitus
#* - pcreate.cpp,.h
#*
#* Revision 1.13  2000/05/15 23:00:07  vitus
#* - /source/lib/pphnd.c/.h
#*
#* Revision 1.12  2000/04/10 01:07:50  vitus
#* - removed proto.h, use VSetup.h on all sources
#*
#* Revision 1.11  2000/03/09 23:16:17  vitus
#* - added PartTable.cpp/.hpp
#*
#* Revision 1.10  1999/12/14 00:18:25  vitus
#* - option argument LISTING enables assembler listing
#*
#* Revision 1.9  1999/11/16 22:12:34  vitus
#* - compress resources (-x2 option)
#*
#* Revision 1.8  1999/03/08 05:10:37  vitus
#* - seperate class definition files (.hpp)
#*
#* Revision 1.7  1999/03/01 02:24:03  vitus
#* - new object filess: VRDev, VRDrive, Chain, Stri0e, Mirror, Single
#*
#* Revision 1.6  1998/12/17 02:23:11  vitus
#* - changed for moved global headers
#* - creates regme.exe and genkey.exe
#*
#* Revision 1.5  1998/11/05 00:26:12  vitus
#* - default: DEBUG=0, 0.50 left as debug vesion :-(
#*
#* Revision 1.4  1998/10/07 23:19:22  vitus
#* - added _BETA_PKG define to all compile steps
#*
#* Revision 1.3  1998/09/21 00:59:44  vitus
#* - moved OBJ files to 'o' subdirectory
#* - added drvif.c/.h
#*
#* Revision 1.2  1998/06/09 01:32:09  vitus
#* - complete vsetup.exe source tree
#*
#* Revision 1.1  1998/03/14 14:06:40  vitus
#* Initial revision
#* ---------------------------------------------
#* This code is Copyright Vitus Jensen 1998-2000
#*
DEBUG *= 0
LISTING *= 0
COMPILER *= VAC
.INCLUDE: ../../../compiler.mkf

CFLAGS = $(_CFLAGS) -D_BETA_PKG -DNORAID5 -Gm
.IF $(LISTING) == 0
CLIST =
.ELSE
CLIST = -Falist\			# cause possible continuation char
.ENDIF


o/%.$(OBJ) : ../../../Lib/%.c '../../../Lib/defines.h' '../../../Lib/verbose.h'
	$(CC2) $(CFLAGS) $(CLIST) -Fo$@ $<

o/%.$(OBJ) : %.c '../../../Lib/defines.h' '../../../Lib/verbose.h' 'VSetup.h'
	$(CC2) $(CFLAGS) $(CLIST) -Fo$@ $<

o/%.$(OBJ) : %.cpp '../../../Lib/defines.h' '../../../Lib/verbose.h' 'VSetup.h'
	$(CC2) $(CFLAGS) $(CLIST) -Fo$@ $<



# **********************************************************************
#
# **********************************************************************

all: VSetup.exe regme.exe genkey.exe
#	pbench.exe csetup.exe


pbench.exe: o/pbench.$(OBJ) o/physdsk.$(OBJ)
	$(CL2VIO) $(CFLAGS) $<


csetup.exe: csetup.$(OBJ) physdsk.$(OBJ) crc.$(OBJ) system.$(OBJ) verbose.$(OBJ)
	$(CL2VIO) $(CFLAGS) $<

csetup.$(OBJ): ../dsklayout.h




# **********************************************************************
#
# **********************************************************************

regme.exe : o/regme.$(OBJ)
	$(CL2VIO) $(CFLAGS) $<

genkey.exe : o/genkey.$(OBJ)
	$(CL2VIO) $(CFLAGS) $<



# **********************************************************************
#
# **********************************************************************

VSETUP_OBJS = o/VSetup.$(OBJ) o/drvscan.$(OBJ) o/drvsetup.$(OBJ) o/drverify.$(OBJ) \
	o/drvtype.$(OBJ) o/dskarea.$(OBJ) o/dump.$(OBJ) o/about.$(OBJ) \
	o/container.$(OBJ) o/iniloc.$(OBJ) o/other.$(OBJ) \
	o/pcreate.$(OBJ) o/PartTable.$(OBJ) \
	o/VRDev.$(OBJ) o/VOS2Drive.$(OBJ) o/Missing.$(OBJ) \
	o/Single.$(OBJ) o/Chain.$(OBJ) o/Stripe.$(OBJ) o/Mirror.$(OBJ) \
	o/profile.$(OBJ) o/physdsk.$(OBJ) o/pphnd.$(OBJ) \
	o/drvif.$(OBJ) o/crc.$(OBJ) o/system.$(OBJ)

DRVIF_H = drvif.h ../vraidpub.h ../dsklayout.h
VSETUP_RC = VSetup.rc about.dlg iniloc.dlg drvdump.dlg \
	drvtype.dlg dskarea.dlg progress.dlg

VSetup.exe: $(VSETUP_OBJS) VSetup.res
	$(CL2PM) $(CFLAGS) -Fm $(VSETUP_OBJS)
	$(RC) -x2 VSetup.res VSetup.exe


VSetup.res: $(VSETUP_RC) resources.h
	$(RC) -r VSetup.rc


o/VSetup.$(OBJ): ../../../Lib/pphnd.h resources.h drvsetup.h $(DRVIF_H)

o/drvsetup.$(OBJ): ../../../Lib/profile.h ../../../Lib/pphnd.h \
		resources.h drvsetup.h drvbuild.h drverify.h \
		dskinfo.hpp Single.hpp Chain.hpp Stripe.hpp Mirror.hpp

o/drvscan.$(OBJ): ../../../Lib/physdsk.h $(DRVIF_H) pcreate.h PartTable.hpp \
		dskinfo.hpp Single.hpp Chain.hpp Stripe.hpp Mirror.hpp Missing.hpp \
		resources.h drvbuild.h

o/pcreate.$(OBJ):$(DRVIF_H) PartTable.hpp dskinfo.hpp pcreate.h

o/PartTable.$(OBJ): $(DRVIF_H) dskinfo.hpp PartTable.hpp

o/VOS2Drive.$(OBJ): $(DRVIF_H) dskinfo.hpp resources.h

o/VRDev.$(OBJ): $(DRVIF_H) dskinfo.hpp resources.h

o/Single.$(OBJ): $(DRVIF_H) dskinfo.hpp Single.hpp resources.h

o/Chain.$(OBJ): $(DRVIF_H) dskinfo.hpp Chain.hpp resources.h

o/Stripe.$(OBJ): $(DRVIF_H) dskinfo.hpp Stripe.hpp resources.h

o/Mirror.$(OBJ): $(DRVIF_H) dskinfo.hpp Mirror.hpp resources.h

o/Missing.$(OBJ): $(DRVIF_H) dskinfo.hpp Missing.hpp resources.h


o/container.$(OBJ): resources.h drvbuild.h

o/pphnd.$(OBJ): ../../../Lib/pphnd.h

o/drvtype.$(OBJ): resources.h drvbuild.h

o/dskarea.$(OBJ): resources.h drvbuild.h pcreate.h

o/dump.$(OBJ): ../../../Lib/physdsk.h resources.h ../dsklayout.h

o/physdsk.$(OBJ): ../../../Lib/physdsk.h

o/drvif.$(OBJ): $(DRVIF_H)

o/other.$(OBJ): resources.h

o/about.$(OBJ): resources.h

o/iniloc.$(OBJ): resources.h
