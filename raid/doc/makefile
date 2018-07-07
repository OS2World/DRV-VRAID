#*
#* $Source: r:/source/driver/raid/doc/RCS/makefile,v $
#* $Revision: 1.2 $
#* $Date: 2000/04/20 23:25:19 $
#* $Locker:  $
#*
#*	Create online documentation.
#*
#* $Log: makefile,v $
#* Revision 1.2  2000/04/20 23:25:19  vitus
#* - added BMP files to dependencies
#*
#* Revision 1.1  1999/11/03 22:11:47  vitus
#* Initial revision
#* ---------------------------------------------
#* This code is Copyright Vitus Jensen 1999-2000
#*/


vraid.inf: vraid.ipf Array.bmp InvalidArray.bmp PDev.bmp locked.bmp \
		DisabledArray.bmp InvalidPDev.bmp bmtmicro.bmp DisabledPDev.bmp \
		Missing.bmp free.bmp
	ipfc -i -d:049 -c:437 vraid


vraid.ipf: vraid.rip vraid.def
	+R:\DEV\IPFPP vraid.rip vraid.ipf vraid.def


