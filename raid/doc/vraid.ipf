.*
.* $Source: r:/source/driver/raid/doc/RCS/vraid.rip,v $
.* $Revision: 1.10 $
.* $Date: 2001/02/11 18:25:15 $
.* $Locker: vitus $
.*
.*      Written with EMACS 19.33, compiled with IPFC Version 4.00.000
.*      Compile: ipfc -i -d:049 -c:437 vraid
.*
.* $Log: vraid.rip,v $
.* Revision 1.10  2001/02/11 18:25:15  vitus
.* - rebuild, declare as invalid
.* - notes FLT and EXE as release version
.*
.* Revision 1.9  2001/02/03 22:49:51  vitus
.* - bank account
.* - order form
.* - 1201DEMO key
.*
.* Revision 1.8  2000/12/13 02:41:12  vitus
.* - many more NAMEIT symbols
.* - describes array verification
.* - history contains flt 0.84 and exe 0.84
.*
.* Revision 1.7  2000/04/20 23:32:59  vitus
.* - filter ordering explained
.* - filter: /!SHARE option
.* - exe: -l:file option
.* - registration seoerated into author and BMT micro (incl. links)
.* - not selling level 3...
.* - history includes VRAID22.ZIP
.*
.* Revision 1.6  1999/11/27 23:04:45  vitus
.* - paragraph describing LX compression and OS/2 2.x
.* - describes VRAID21.ZIP contents
.*
.* Revision 1.5  1999/11/16 22:08:55  vitus
.* - filled :H3 res=ID_ADDPART.Add something to an &ARRAY.
.*
.* Revision 1.4  1999/10/25 09:05:00  vitus
.* - halfway through to VRAID21 release.  DnD has to be described
.* - more footnotes
.* - uses definitions from vraid.def
.*
.* Revision 1.3  1999/10/21 00:32:17  vitus
.* - Filter version 0.80 partly described
.*
.* Revision 1.2  1999/06/26 00:10:08  vitus
.* - more general description, RAID levels
.* - incorporated vraid.history
.* - overwriting means...
.* - SCSI problems/hangs when testing driver failures
.* - history describes VRAID20.ZIP
.*
.* Revision 1.1  1999/06/13 01:41:57  vitus
.* Initial revision
.* ----------------------------------------
.* This code is Copyright Vitus Jensen 1999
.*
:USERDOC.
:TITLE.VRAID Documentation
.NAMEIT SYMBOL=EXE TEXT='VSetup.exe'
.NAMEIT SYMBOL=FLT TEXT='VRAID.FLT'
.NAMEIT SYMBOL=PDEV TEXT='PHYSDEVICE'
.NAMEIT SYMBOL=ARRAY TEXT='Array Drive'
.NAMEIT SYMBOL=CFG TEXT='Config.Sys'
.NAMEIT SYMBOL=LL TEXT='Logical Drive'
.NAMEIT SYMBOL=EP TEXT='Extended Partition'
.NAMEIT SYMBOL=SP TEXT='Secondary Partition'
.NAMEIT SYMBOL=OS2DEV TEXT='OS/2 Device'
.NAMEIT SYMBOL=OS2DRV TEXT='OS/2 Drive'
.NAMEIT SYMBOL=LXLITE TEXT=':LINK REFID=lxlite REFTYPE=fn.LXLite:ELINK.'
.NAMEIT SYMBOL=BUILD TEXT=':LINK REFID=Build REFTYPE=fn.Build:ELINK.'
.NAMEIT SYMBOL=REBUILD TEXT=':LINK REFID=Rebuild REFTYPE=fn.Rebuild:ELINK.'
.NAMEIT SYMBOL=FAIL TEXT=':LINK refid=Fail REFTYPE=fn.Fail:ELINK.'
.NAMEIT SYMBOL=ERROR TEXT=':LINK refid=Error REFTYPE=fn.Error:ELINK.'
.NAMEIT SYMBOL=MBR TEXT=':LINK REFID=MBR REFTYPE=fn.MBR:ELINK.'





.* ======================================================================
:H1 res=1000.General
:P.
Welcome to VRAID, the first (to my knowledge) Shareware RAID
solution for OS/2.  Instead of buying an expensive RAID controller you
add this driver to your system, connect some drives (the more the
better) and are ready for operation!
:P.
This software :LINK REFID=features REFTYPE=fn.features:ELINK.&colon.

:UL.
:LI.:HP2.Single:EHP2. (an array containing a single disk or array)

:LI.:HP2.Chaining:EHP2. (two or more disks or arrays combined by
adding one behind the other)

:LI.:HP2.Striping:EHP2. (RAID 0)

:LI.:HP2.Mirroring:EHP2. (RAID 1)

:LI.You can combine any combination of RAID levels to create the array
OS/2 should use for file storage.  There is no
:LINK REFID=limits REFTYPE=fn.limit:ELINK. on how complicated your
array structure becomes.

:LI.&BUILD. and &REBUILD. processes are done in the background.
:EUL.

:P.
This software package should technically work on every OS/2 version
above 2.0.  It was tested under Warp 4 and the new :HP1.Warp
Server for E-Business:EHP1. (uni).  There are reports that it works
well on a SMP machine (I currently have no test machine with multiple
CPUs).


.BR
To get the archive a little smaller and as most people use OS/2 Warp 3
or higher I used the new LX compression available in that version.  If
you still run an older version you have to unpack &EXE. with
&LXLITE. or contact me and I will send you an uncompressed &EXE..





.* ======================================================================
:H2.How is it done?
:P.
Well, &FLT. is a small filter driver which intercepts OS2DASD.DMD
operation and combines non-removable disks of any kind to RAID arrays.
This process is transparent to anything installed after &FLT..  For
FAT, hpfs.ifs, jfs.ifs, ext2fs.ifs and any other software you might use a
RAID array looks just like an ordinary disk.

:P.
To use VRAID you have to complete the following steps:

:OL.
:LI.Connect disks to any [SCSI or IDE or MFM or RLL or ESDI or ...]
controller supported by OS/2.

:LI.Install &FLT.

:LI.Let &EXE. create VRAID partitions on all disks you want to use for
arrays.

:LI.Define arrays (using disk space inside VRAID partitions) via
&EXE..

:LI.Restart OS/2

:EOL.

:P.
&FLT. will report your defined arrays to OS/2 as a new hard disk.  All
you have to do now is to make use of your new "disk" and enjoy!

:P.
Installation and configuration is described in greater detail in later
chapters.
:P.
See RAID.DOC for details how each RAID level works.






.* ======================================================================
:H2 RES=1100.Author
:P.
Vitus Jensen, Dipl.-Ing. techn. inf.

:PARML TSIZE=20 BREAK=NONE COMPACT.
:PT.Address
:PD.Tischlerstr. 8, D-30916 Isernhagen, Germany

:PT.Bank Account
:PD.Kto 476861-203, Postbank Hamburg, BLZ 20010020

:PT.Fidonet
:PD.2&colon.2474/424

:PT.OS2Net
:PD.81&colon.491/3035

:PT.EMail
:PD.vraid&atsign.gmx.de, Vitus_Jensen&atsign.gmx.de

:PT.FAX
:PD.+49-5136-893003 (thanks to Harald Pollack's FRec)

:PT.BBS
:PD.+49-5136-893003 (V34, V90S, X75)
:EPARML.

:P.
Music: Louis Armstrong's Greatest Hits, Big T and Mighty Max,
Billie Holiday Collection

:P.
Written for &FLT. Version: 0.82

:P.
BTW, if ever wondered about the name (Visual RAID Filter) of this
thing: it's as visual as many products which use this attribute in
their names.  Frankly, I couldn't make sense to it, either.






.* ============================================================
:H2 res=1020.Legal Stuff
:P.
The author disclaims all warranties as to this software, whether
express or implied, including without limitation any implied
warranties of merchantability, fitness for a particular purpose,
functionality, data integrity or protection, in so far as permitted by
applicable legislation.

:P.
All trademarks are property of their respective owners and appear
for identification purposes only.






.* ============================================================
:H2 res=1030.Archive Contents
:P.
:PARML TSIZE=20 BREAK=NONE COMPACT.
:PT.VRAID.inf
:PD.this documentation

:PT.&FLT.
:PD.the filter itself

:PT.VRAID.ddp
:PD.installation script for DDINSTAL

:PT.&EXE.
:PD.graphical setup program

:PT.raid.doc
:PD.short explaination of RAID and RAID levels

:PT.regme.exe
:PD.inserts your registration code into &FLT.

:PT.vraidpub.h
:PD.public interface to &FLT. (for programmers)

:PT.dsklayout.h
:PD.describes how configuration data is saved to disk.

:PT.VRAID.sym
:PD.symbol file, to help in remote debugging once a problem occurs
(and someone pays the telephone bill...)

:PT.file_id.diz
:PD.short description for BBS
:EPARML.






.* ============================================================
:H1 res=2000.Driver Installation

:P.
Make a backup of your system.  You should not need it but better
safe than sorry...

:P.
It is recommended that you have an OS/2 boot disk ready, just in
case the system locks up during the boot process and you want to
remove the driver.  On Warp you may use use the ALT-F1 menue for the
same purpose (to boot to command line-only) with your original
&CFG. (this "original" &CFG. is located in
C&colon.\OS2\BOOT\CONFIG.X, make sure it's bootable!).

:P.
Execute DDINSTAL from an OS/2 command prompt or double-click
the Device Driver Install object (located in the System Setup folder)
if your system is Warp.  Change the source directory if neccessary and
a selection list will open asking you to install the 'VRAID Filter'.
Mark that line and click on OK.

:P.
This automatic install will place &FLT. and &EXE. in the root
directory of your OS/2 boot drive.  &EXE. can be moved to any place
you want but &FLT. must reside in the root or \OS2 or \OS2\BOOT.


:P.
Installation by hand is easy:

:UL.
:LI.Copy &FLT. to C&colon.\ or C&colon.\OS2\BOOT, assuming
C&colon. is your boot drive.

:LI.Copy &EXE. to a directory of your choice.  You may wish to create
a program object for this file and place it on your desktop.

:LI.Edit your C&colon.\&CFG. file (i.e. using the OS/2
"System Editor")

:SL.
:LI.Add the following line to your &CFG.&colon.

:LI.BASEDEV=&FLT. /V              (no subdirectory name!)
:ESL.
:EUL.

:P.The position of 'BASEDEV=&FLT.' in &CFG. doesn't matter
for VRAID operation.  But it may affect other filters&colon.

:UL.
:LI.Load DSKSLEEP.FLT before &FLT. so DSKSLEEP.FLT can see the
original disks.  If you do it the other way round DSKSLEEP.FLT may try
to stop a disk while &FLT. does a rebuild!

:LI.Load PARTFILT.FLT (from EXT2FS or FAT32.IFS) behind &FLT..
PARTFILT.FLT duplicates any disk with an FAT32 or EXT2FS partition
which will duplicate any VRAID partition residing on the same disks.
:EUL.

:P.If you encounter problems try putting BASEDEV=&FLT. to either end
of &CFG. (if this solves your problems please report with a listing of
your &CFG.).


:P.
Changes to &CFG. require you to reboot your machine.  But
before doing this you may want to look at chapter
:LINK REFTYPE=hd RES=3000.Configuration:ELINK. as defining
RAID arrays doesn't require you to have &FLT. loaded.






.* ============================================================
:H2 res=2010.&FLT. Options
:P.
:PARML TSIZE=20 BREAK=NONE.
:PT./V
:PD.Verbose, display some usefull and unusefull information (this
switch is always enabled in the unregistered version)

:PT./DEBUG
:PD.Displays even more messages.  Not generally usefull.

:PT./!SHARE
:PD.Don't share disks with OS/2.  Any disk with at least a PHYSDEV
will be hidden (as in driver versions below 0.80) from OS/2.
:EPARML.






.* ============================================================
:H1 res=3000.Configuration
:P.
This chapter describes &EXE., a program to define RAID drives
build by &FLT. on the next system startup.  Additional some
settings/statistics of &FLT. may be changed/retrieved at runtime.

:P.As it is difficult to discuss the whole matter (drives made out of
drives which are known by OS/2 as drives) I deciced to put up a
nomenclature.  Fairly simple:

:UL.
:LI.OS/2 Device
.BR
A disk accessable through OS/2 and going directly to the physical
medium.  No translation by VRAID, this is a piece of hardware.

:LI.Primary Partition
.BR
I think you know already what this term means&colon. a partition
entered in the very first partition sector (&MBR.).

:LI.Extended Partition
.BR
This is another well known term&colon. a partition described in the
&MBR. and holding one or more partitions.

:LI.Secondary Partition
.BR
This term is invented by me&colon. often a partition inside an
&EP. ist called &LL..  But as &LL. sounds like drive letter assignment
and not every &LL. is assigned a drive letter I will use the term &SP..

:LI.&OS2DRV.
.BR
In most situations the same as &OS2DEV..  An &OS2DRV. contains zero or
more partitions and every partition may contain a filesystem supported
by OS/2.  A supported filesystem (FAT, HPFS, EXT2FS, ...) is assigned
a drive letter.

:LI.&PDEV.
.BR
Whenever a &OS2DEV. contains a VRAID partition (partition type 0x7C
if you wonder) that partition is called a &PDEV..
:P.
&PDEV.s may become part of...

:LI.&ARRAY. (Single, Striping, Chaining, Mirror, ...)
.BR
&ARRAY.s are build from &PDEV.s and may be part of other &ARRAY.s,
building a freely definable disk structure.  The topmost &ARRAY. is
reported by &FLT. to OS/2.  It looks to OS/2 like a normal disk and as
has to be partitioned and formatted like usual.
:EUL.






.* ============================================================
:H2.&EXE. Options
:P.
:PARML TSIZE=20 BREAK=NONE.
:PT.-v
:PD.Verbose, display some more usefull and unusefull information.  You
may added a colon and a number to specify how much is displayed.
Specifying -v without a number is the same like -v&colon.2.
:P.
Increasing this value is usefull if you recognize an odd behaviour of
&EXE. and want to report this to the author.  Specify
:HP1.-v&colon.2:EHP1. or :HP1.-v&colon.3:EHP1. and add the output to
your error report.

:PT.-l&colon.filename
:PD.log messages to file.
:EPARML.



.* ------------------------------------------------------------
:H2.Main Window
:P.
That's the window you see when you start &EXE..  It contains an
action log and a menu bar to select tasks.
:P.
The amount of messages which appear inside the action log is controlled
by option "-v".



.* ------------------------------------------------------------
:H3 res=3110.File Menu
:P.
This menu let you save or print the contents of the action log.

:P.
--- beta --- beta --- beta --- beta --- beta --- beta --- beta ---
.BR
None of the choices in the file menu are currently implemented.
.BR
--- beta --- beta --- beta --- beta --- beta --- beta --- beta ---




.* ------------------------------------------------------------
:H3 res=3120.Edit Menu
:P.
Entries "Cut", "Copy" and "Paste" interact with the action log and
the clipboard (same actions as the usual CUA keys which work, too),
"Clear log" clears the action log.




.* ------------------------------------------------------------
:H3 res=3130.Driver Menu
:P.
This menu is only enabled if an installed &FLT. is detected.
Choices are to add the driver version or it's startup messages to the
action log.
:LINK refid=Beep REFTYPE=fn.Beeps:ELINK. generated
internally in the &FLT. may be disabled here, too.




.* ------------------------------------------------------------
:H3 res=3140.Drives Menu
:P.
This menu contains the entry "Setup" which opens the Drive
Definition Window (see
:LINK reftype=hd res=3200.below:ELINK.) and a "Dump..." choice.

:P.
Selecting "Dump..." opens a dialog where you can select which drive
to dump.  Possible selections are the the index of an &OS2DRV.
(index 0 - n-1) and the value "-1", meaning to dump all &OS2DRV.s.

:P.
This dump facility will add hexdumps of the following sectors into the
action log&colon.

:OL.
:LI.partition sector

:LI.first sector of VRAID partition (if available).  This sector
describes the &PDEV.

:LI.a sector for every array this &PDEV. is part of.
:EOL.

:P.
The reason for the dump facility is to diagnose a situation where
&FLT. no longer recognizes your &ARRAY. definitions.  Remove
&FLT. from &CFG., reboot OS/2, start &EXE. and dump all drives.  The
result will tell the experts (you? me?) what is wrong.




.* ------------------------------------------------------------
:H3 res=3150.Help

:P.
The only choice here is to display version information about
&EXE. itself.  There is currently no online help for &EXE..






.* ============================================================
:H2 res=3200.Drive Definition Window

:P.
Whenever you select Drives->Setup a new windows opens. It displays
a container showing all devices found by &EXE..  Drives are found by
quering &FLT. (if installed) and OS/2.

:P.
Existing &ARRAY.s are displayed in tree view showing which &ARRAY.s
are made from which &PDEV..  There are several object types showing
different device types.  They are:


:UL.
:LI.3D look grey boxes (&OS2DEV.s)
.BR
Displayed as :ARTWORK NAME='Free.bmp' RUNIN.

:P.
They are devices known to OS/2.  You may create a &PDEV. on them and
afterwards use those to build a new array.


:LI.2D drive with grey front (&PDEV.)
.BR
Displayed as :ARTWORK NAME='PDev.bmp' RUNIN.

:P.
A &PDEV., a drive where a VRAID partition resides.  Those objects
build or can be used to build an array.


:LI.2D drive with grey front and red cross (missing &PDEV.)
.BR
Displayed as :ARTWORK NAME='Missing.bmp' RUNIN.

:P.
A &PDEV. which is known to be part of an array but is missing
from this computer.  Array is in state "failure" or "error".


:LI.two 2D drives with grey front (&ARRAY.)
.BR
Displayed as :ARTWORK NAME='Array.bmp' RUNIN.

:P.
An array being build from &PDEV.s and available for OS/2 or
being part of a larger array.


:LI.2D drive with red front (&PDEV. not up-to-date)
.BR
Displayed as :ARTWORK NAME='InvalidPDev.bmp' RUNIN.

:P.
A &PDEV. being part of an array but this &PDEV. is not
up-to-date in respect to the contents of the complete array.  A
&BUILD. or &REBUILD. process is running or will start on reboot.  This
process updates the device until it becomes up-to-date.


:LI.two 2D drives with red front (&ARRAY. not up-to-date)
.BR
Displayed as :ARTWORK NAME='InvalidArray.bmp' RUNIN.

:P.
Same as above but the device displayed is an &ARRAY. (not at
topmost level but being part of another array).
:EUL.



:P.
Whenever you create a new &PDEV. two messages may appear in the
action log:

:SL.
:LI."VORDev - missing PHYSDEV mark"
:LI."VORDev - new PHYSDEV sector"
:ESL.

:P.
Those messages are non-critical, they tell you that you are really
creating a new &PDEV..




.* ------------------------------------------------------------
:H3 res=3210.Mouse Interaction

:P.
All navigation keys or mouse actions you know from other tree views
(i.e. the drive objects) are working here as well.   But there are
special considerations when it comes to clicking.  See here&colon.


:DL break=all.
:DT.Double clicking with first mouse button
:DD.Selects a &PDEV. or &ARRAY..  Selected devices can be used to
build &ARRAY.s.


:DT.2nd mouse button click
:DD.Display a context sensitive popup menu.  This menu allows you to
create or destroy &PDEV.s, build an &ARRAY. from marked
devices, destroy &ARRAY.s, remove a part of an &ARRAY. from
the array, ...

:P.
Try the second mouse button everywhere to learn what's possible!


:DT.Drag and Drop
:DD.It is possible to drag an &ARRAY. or &PDEV. onto a RAID 1,
4 or 5 array.  The dragged device will become part of the array.

:P.
Becoming part of an &ARRAY. means that the dropped device gets data
from the array and will be overwritten.  This overwriting is either a
&BUILD. or &REBUILD. (RAID 4 and 5 in &FAIL. state) starting on next
reboot or the device is added as a spare (RAID 4 and 5) and will
automatically replace a failed part of an array when it fails.

:EDL.






.* ============================================================
:H2 res=3300.Sample actions

:P.
The following pages describe step-by-step how to do common tasks with
&EXE..  As &EXE. is a PM program most people would say the interface
is intuitive, but one never knows...




.* ------------------------------------------------------------
:H3 res=3310.Create a &PDEV.

:P.
We assume it's the first time you create a &PDEV..

:OL.
:LI.Start &EXE.

:LI.Select Drives->Setup

:LI.after some drive activity you should see some grey 3D drive
icons (:ARTWORK NAME='Free.bmp' RUNIN.).  Those objects represent your
OS/2 disks.

:LI.click on an &OS2DEV. using the 2nd mouse button
.BR
and select "Create physdevice".  A dialog will prompt you for data used to
create a VRAID partition&colon.

:UL.
:LI.Select one of the areas of unused disk space under "Free area".
.BR
If there isn't already another partition on the disk, a single entry
with the whole disk space will be shown.
.BR
Free areas include even free space inside extended partitions.

:LI.Adjust the slider if you don't want to use all of the free area.
The VRAID partition will be created at beginning of the free area.
:EUL.

:P.
Note how the icon style changes after creation of the &PDEV..

:LI.Stop &EXE. or
:LINK reftype=hd res=3330.create &ARRAY.s:ELINK..
:EOL.




.* ------------------------------------------------------------
:H3 res=3320.Destroy a &PDEV.

:P.
Destroying a &PDEV. will remove any configuration data from the disk,
including the VRAID partition.


:OL.
:LI.Start &EXE.

:LI.Select Drives->Setup

:LI.After some drive activity you should see some grey 2D icons
(:ARTWORK NAME='PDev.bmp' RUNIN.) representing &PDEV.s.  If the &PDEV.
is still part of an &ARRAY. you have to
:LINK reftype=hd res=3340.destroy:ELINK. it first.

:LI.Click on a &PDEV. using the 2nd mouse button
.BR
and select "Destroy physdevice".  Confirm the action.

:P.
Note how the icon style changes afterwards.

:LI.Stop &EXE. and reboot.
:EOL.




.* ------------------------------------------------------------
:H3 res=3330.Define an &ARRAY.

:P.
You need something to build &ARRAY.s from. See
:LINK reftype=hd res=3310.here:ELINK. how to create
&PDEV.s if you haven't already done this.


:OL.
:LI.Start &EXE.

:LI.Select Drives->Setup

:LI.Double click on one or more &PDEV. or &ARRAY. objects.  They
will be get selected (greyed).

:LI.Use 2nd mouse button and select "Combine drives...".  You will be
prompted for a RAID level (don't select levels higher than your
registration allows&colon. those arrays are ignored by your &FLT.).
:P.
The choosable RAID level is sensitive to the count of selected
(greyed) objects.
:HP2.Chaining:EHP2.,
:HP2.Striping:EHP2.,
:HP2.Mirroring:EHP2. require two or more parts,
:HP2.RAID 4:EHP2. und
:HP2.RAID 5:EHP2. require at least three.  
:HP2.Single:EHP2. is only possible
when you selected a single object (obvious, isn't it?).
:P.
Most RAID levels (all but :HP2.Chaining:EHP2.) can only handle
equally-sized parts.  &EXE. will account for this by using the size of
the smallest part.

:LI.Repeat the last steps until you are satisfied.

:LI.End &EXE. and reboot your machine to let &FLT. analyse the new
situation.
:EOL.




.* ------------------------------------------------------------
:H3 res=3340.Destroy an &ARRAY.

:OL.
:LI.Start &EXE.

:LI.Select Drives->Setup

:LI.Use 2nd mouse button on an &ARRAY. and select "Destroy array".
Confirm the action.
.BR
It is only possible to destroy the topmost &ARRAY..  See
:LINK reftype=hd res=3370.Remove part of an
&ARRAY.:ELINK. for other actions.

:LI.Repeat the last step until you are satisfied.

:LI.Use the parts to create new arrays or end &EXE. and reboot your
machine to let the changes take effect.
:EOL.




.* ------------------------------------------------------------
:H3 res=3350.Verify &ARRAY. data

:P.
&ARRAY.s created using &EXE. versions below 0.84 should not be
verified as there were never a &BUILD. process started/completed.
This is no problem in normal operation or when any part of the
&ARRAY. fails but a verify run will return data errors on those
arrays.



:OL.
:LI.Start &EXE.

:LI.Select Drives->Setup

:LI.Use 2nd mouse button on an &ARRAY. and select "Verify array".

:LI.While the verify is active you have buttons to temporary pause
the process or to abort it completely.  &EXE. dedicates it's work to
the verification process, you won't be able to do other configuration
work in parallel to the verification.

:LI.if any verify errors are encountered you have several choices:

:UL.
:LI.ignore the error and continue.
:LI.abort the verification and start a &REBUILD. process.  It might be
neccessary to select which part should be considered valid (if there
is no majority on a sector contents).
:LI.abort the verification and do nothing.  It is highly recommended
that you do :HP1.not:EHP1. select this option if you value the data on
the array.
:EUL.

:LI.Repeat the last steps until you are satisfied.

:EOL.




.* ------------------------------------------------------------
:H3 res=3360.Add something to an &ARRAY.

:P.
You may add &ARRAY.s or &PDEV.s to :HP2.Chaining:EHP2. or
:HP2.Mirroring:EHP2. objects.
.BR
Adding is done by means of drag-and-drop.  Pickup a the new part (only
topmost icons are possible) and drop it to an &ARRAY.  The new part
will be destroyed by this action (there is a message box asking
whether this is OK).


:UL.
:LI.Adding to a :HP2.Chaining:EHP2. device:
:P.
Only possible if the :HP2.Chaining:EHP2. device is the topmost icon.
The &ARRAY. drive will become larger by this action.  Create a new
partition on the additional space or use something like :HP1.Partition
Magic:EHP1. to enlarge an existing partition.


:LI.Adding to a :HP2.Mirroring:EHP2. device:
:P.
Always possible.  The new part will increase redundancy.  A
&REBUILD. process will be started on the next reboot.
:EUL.




.* ------------------------------------------------------------
:H3 RES=3370.Remove part of an &ARRAY.

:P.
There may be several reasons why you want to remove part of an array
while keeping the array definition.  One reason would be if a part of
an :HP2.RAID 1:EHP2. array fails (&FLT. will warn you through regular
:LINK refid=Beep REFTYPE=fn.Beeps:ELINK.) and you decided that the
failed part will never be usuable again. So you wish to remove it from
the array.

:P.
Take the following steps to do so&colon.

:OL.
:LI.Start &EXE.

:LI.Select Drives->Setup

:LI.Expand the &ARRAY. trees by clinking on all 'plus' signs.

:LI.Locate the missing part.  It has an icon with a red cross through it.

:LI.Use 2nd mouse button on the missing part and "Remove from array".

:LI.End &EXE. and reboot your machine to let the changes take effect.
:EOL.




.* ------------------------------------------------------------
:H3 RES=3380.Tell &FLT. that a part of an &ARRAY. is invalid

:P.
There may be sitations where you know more about your disks than
&FLT.... Well, actually I can't think of such a situation but lets
assume that you have a disk belonging to a :HP2.RAID 1:EHP2. array
which makes funny noises and does intermediate shutdowns.  You should
obviously replace the disk but lets assume you just want to rewrite
the data on that particular disk.


:P.
Take the following steps to do so&colon.

:OL.
:LI.Start &EXE.

:LI.Select Drives->Setup

:LI.Expand the &ARRAY. trees by clinking on all 'plus' signs.

:LI.Locate the interesting part.

:LI.Use 2nd mouse button on that part and select "Declare as invalid".

:LI.The array state changes to &REBUILD. and the part you clicked on
gets an red icon.

:LI.End &EXE. to let &FLT. do the work.
:EOL.

&FLT. will now react the same way as when it had detected the disk
problem by itself (which it will have done in the example given above)
and starts a rebuild process to copy all data from the remaining valid
parts of the array to the invalid part.




.* ------------------------------------------------------------
:H3 RES=3390.Manually tell &FLT. to rebuild the array

:P.
You may tell &FLT. to rebuild the array data with doing a verify first
or declaring any part of the array as invalid.  This will trigger a
rebuild using all parts of the array as the data source.  Other than
to demonstrate the effect of a rebuild I don't think this option is
usefull for every day use.  It is defenitely usefull during
development of &FLT..

:P.
Take the following steps start a rebuild&colon.

:OL.
:LI.Start &EXE.

:LI.Select Drives->Setup

:LI.Use 2nd mouse button on the &ARRAY. and select "Rebuild array".

:LI.The array state changes to &REBUILD.

:LI.End &EXE. to let &FLT. do the work.
:EOL.






.* ============================================================
:H1.Security

:P.RAID generally means increased security.  But as the only secure
RAID level this driver currently implements is RAID 1 security isn't
completely tested with all RAID levels.

:P.On non-RAID 1 drives all drive errors are directly reported to OS/2
(which may or may not survive this situation).

:P.On RAID 1 drive errors aren't reported to OS/2 as long as there is
one part of the array still working.  However, &FLT. will start
:LINK REFID=Beep REFTYPE=fn.beeping:ELINK. every 10 seconds if it
detects a drive error.  Once this happens you may either start
&EXE. and remove the failed part from the array or -- if the failure
is temporarly -- correct it and reboot the machine.  &FLT. will detect
a failed part of an array when rebooting and will start a
&REBUILD. process.

:P.If you ever intend to simulate a drive failure :HP2.DON'T:EHP2. remove the
SCSI cable but remove the power instead.  Or -- good idea -- use some
hot-swappable drive bay.  If you remove the SCSI cable you may end up
with a blown up SCSI controller.  You have been warned!

:P.Please connect a LED to the LED connector of your SCSI controller
and monitor it's state.  When you happen to pull the plug while the
SCSI controller is busy doing I/Os on this device it may never detect
the failure.  I have noticed that AHA154X.ADD from OS/2 Warp 4 has no
timeout detection and waits forever!






.* ======================================================================
:H1.Registration

:P.This package is Shareware meaning you may test before buy.  The
archive you are looking at contains a non-crippled 'level 1' (see
:LINK REFTYPE=HD RES=5300.Demonstration Key:ELINK. for
'level 2') filter driver with the exception of the startup message
which you may only disable in the registrated version.  The test
period is 60 days, after that time I expect you to remove this
software package from your system or register it.

:P.
You are encouraged to copy and pass on the
:HP1.unregistered:EHP1. version of this driver, if the archive is
distributed in it's original form (complete and unmodified).  Please
don't charge any money for the distribution itself exceeding the costs
of media, etc.
.BR
This includes the distribution in a software collection on CD-ROM.

:P.
On registration you will receive a key based on your name and the
requested operation level of the driver.  This key has to be passed to
the regme.exe program to change the startup logo and -- if you bought
more than level 1 -- allow operation of higher RAID levels.  You will
NOT receive a new archive.  Your key will remain valid for all
versions of the driver.

:P.The following keys are available:

:TABLE COLS='8 35 10' RULES=both.
:ROW.
:C.Level 1
:C.Spanning, Striping (RAID 0)
:C.40,- EUR
:ROW.
:C.Level 2
:C.Level 1 plus Mirroring (RAID 1)
:C.70,- EUR
:ROW.
:C.Level 3
:C.Level 2 plus RAID 4 and 5
:C.:LINK REFID=nolevel3 REFTYPE=fn.100,- EUR:ELINK.
:ETABLE.

:P.If you want to upgrade your registration to a higher level: just
pay the difference and you will receive a new key for that level.
I.e. if you own level 1 and want to upgrade to level 2 the price will
be 30,- EUR or the equivalent in another currency.

:P.Upgrades are only available directly from the author.

:P.For all orders: please :HP1.state clearly:EHP1. on which name the
key should be based.  If you want the key to belong to a firm, state
this in your order (if you use corporate paper you may end up with a
key registrated to yourself when you wanted a license for "Obscore
Software GmbH").



.* ----------------------------------------------------------------------
:H2.Registration through BMT Micro

:ARTWORK NAME='BMTMicro.bmp' ALIGN=LEFT.

:P.BMT Micro is a US company that is specialized on shareware
registration to the world.  There are several ordering and payment
methods available, see
:LINK REFTYPE=launch OBJECT='netscape.exe'
DATA='http://www.bmtmicro.com/'.
http&colon.//www.bmtmicro.com/:ELINK.
for details.  Even for european users this is probably the easiest way
to register.

:P.If you want to order online, BMT Micro supplies secure URLs for this
purpose:

:TABLE COLS='8 35 60' RULES=both.
:ROW.
:C.Level 1
:C.Spanning, Striping (RAID 0)
:C.:LINK REFTYPE=launch OBJECT='netscape.exe'
DATA='https://secure.bmtmicro.com/ECommerce-OnSite/61900.html'.
https&colon.//secure.bmtmicro.com/ECommerce-OnSite/61900.html:ELINK.

:ROW.
:C.Level 2
:C.Level 1 plus Mirroring (RAID 1)
:C.:LINK REFTYPE=launch OBJECT='netscape.exe'
DATA='https://secure.bmtmicro.com/ECommerce-OnSite/61901.html'.
https&colon.//secure.bmtmicro.com/ECommerce-OnSite/61901.html:ELINK.
:ETABLE.

:P.Please note that prices at BMT Micro are in US dollar.




.* ----------------------------------------------------------------------
:H2.Registration with the author

:P.Ordering with the
:LINK REFTYPE=hd RES=1100.author:ELINK. can
be done by EMail, FAX or snail mail.

:P.Payment can be made through Eurocheque (europe) or postal money
order or transfer to my
:LINK REFTYPE=hd RES=1100.bank account:ELINK..  If you decide to
send this money inside a simple envelope (I'm fair but the postman may
not <g>) use DM (Euro after 1.1.2002), or DKR or SEK as I travel those
countries regulary.
.BR
It may be best to first test an electronic channel before sending
money in any way.

:P.Response will be through Fidonet crashmail (strongly preferred),
Fidonet netmail, EMail or FAX.

:P.If you cannot be reached electronically and don't have a friend who
can, you may request an answer by snail mail.  Please add the
estimated costs for the letter to the registration fee.  German users:
simply include a self-addressed envelope (with stamps on it).  I will
NOT return a key without at least some afford from your side to
account for the costs of the reply mail!




.* ----------------------------------------------------------------------
:H3.Order Form

To order from the author fill in the following form and then pass it
to one of
:LINK REFTYPE=hd RES=1100.my addresses:ELINK..
:P.

:XMP.
Please send me:

  ___ keys for VRAID, level 1 (Spanning, Striping)
                Price: 40,- EUR

  ___ keys for VRAID, level 2 (level 1 plus Mirroring)
                Price: 70,- EUR


  Total:        EUR ________



  The key(s) shall be based on the following string (personal or
  company name):

                            ______________________________________



Personal Information:

  Name:                     ______________________________________

  email, netmail:           ______________________________________

  Phone:                    __________________________________ (opt)

  Fax:                      __________________________________ (opt)



Payment Method:

        ___ see enclosed eurocheque

        ___ I will transfer to your bank account once you acknowledged
            my order
:EXMP.






.* ----------------------------------------------------------------------
:H2 RES=5300.Demonstration Key

:P.If you are of the opinion that you won't buy a higher level of
VRAID without actually testing RAID 1 support, I do understand this.
Feel free to ask me for a demo key and I will mail you a key which
will stop working after some time (usually 3 month).  You may than
again request a key but I thing after 3 month of personal testing time
should have figured out whether this software is usefull to you (see,
I might ask questions! ;-).

:P.To get started you may use the following key (it won't work after
december 2001):

:SL.
:LI.User Name: 1201DEMO
:LI.Code: 5E8D60207D284AC6
:ESL.

:P.Change to the directory where you copied &FLT. to and enter:

:XMP.
regme "1201DEMO" 5E8D60207D284AC6
:EXMP.

:P.If you want to register a driver in some other directory you may use
the "-f" option to tell regme.exe which file to patch.

:XMP.
regme -f&colon.d&colon.\some\other\path\vraid.flt "User Name" key
:EXMP.






.* ============================================================
:H1 res=6000.Source Code

:P.Source code isn't available for this product.  Having said this I
realize that having the source of a vital product is important to many
people nowerdays.  I understand this (especially when seeing the
unclear marketing directions of IBM's OS/2 folks) and you may contact
me if your policy tends toward this idea.  We will most certainly work
something out.

:P.To give you some ideas what to expect:
:UL.

:LI.&FLT. is a pure 16 Bit driver and is written for an old version of
Microsoft's C compiler for Microsoft OS/2.

:LI.&EXE. is pure 32 Bit, written directly to the presentation manager
API.  Compiled with IBM VisualAge for C++ 3.0.

:LI.This documentation is pure IPF source.  No fancy toolkits.
:eul.


:nt.The archive already contains part of the source code:
:UL.

:LI.dsklayout.h

:P.This files documents the way array information is saved to disk.
Using the information you should be able to write a driver replacing
&FLT. (i.e. for another OS) or to reconstruct any data stored on
&ARRAY.s without using &FLT..

:LI.vraidpub.h

:P.This files documents to interface between &EXE. and &FLT..  Using
information from this file and dsklayout.h you may write a replacement
for &EXE..

:eul.
:ent.






.* ======================================================================
:h1 res=7000.History

:P.Despite of the limited count of releases there is quite a large
history of internal and published software versions.  All those
versions are documented here...






.* ======================================================================
:h2 res=7100.&FLT. History


:DL.

:DT.Version 0.85
:DD.
:UL compact.
:LI.build uses larger I/O size, this speeds up the process
:LI.init code could "loose" parts while looking for arrays.  fixed.
:EUL.


:DT.Version 0.84 - 11/02-2001, published in VRAID23.ZIP
:DD.
:UL compact.
:LI.IOCtls to verify arrays
:LI.IOCtls to modify array children, clarified build vs rebuild
:EUL.


:DT.Version 0.82 - 15/04-2000, published in VRAID22.ZIP
:UL compact.
:LI.VRAID partition inside extended partitions are found and used.
:LI.the disk sharing introduced in version 0.80 may be disabled via
commandline switch "/!SHARE".
:LI.resource locking now truly SMB compatible (but there were
reports it worked before so the old code might have been 'Warp Server
compatible').
:EUL.

:DT.Version 0.80 - 27/11-1999, published in VRAID21.ZIP
:DD.
:UL compact.
:LI.Disks containing a VRAID partition now remain accessable to
OS/2.  This means you can have a VRAID and a HPFS partition on the same disk
and can access both when &FLT. is loaded.  In previous versions you
had no access to the HPFS partition...
:LI.Smaller memory footprint due to new segment layout.
:EUL.


:DT.Version 0.71
:DD.
:UL compact.
:LI.Changes for WatCom C 10.5 (only minor to circumvent bugs
of that compiler).  The compiler change has been stopped as WatCom C is no
longer a maintained software package.
:LI.Driver doesn't update disk when build processes where stopped due to &EXE.
:EUL.


:dt.Version 0.70 - 26/06-1999, published in VRAID20.ZIP
:dd.
:ul compact.
:LI.complete support of RAID 1 arrays.  Fully tested disk failures,
reading or failed and/or removed disks.  He, it's working now!
:LI.changed the way a registration key is build and checked.  Old keys
won't work with this driver release and vice versa!  Registrated users
will get an updated key.
:LI.added demo key code (timebombed) for registration level 2 and above.
:eul.


:dt.Version 0.66 - 02/06-1999
:dd.
:ul compact.
:LI.build process saves state on shutdown and completion
:LI.tested RAID 1 code: OK, drive failure has to be tested
:LI.ignores disks with VRAID partition but no &PDEV.
configuration.  That way you don't have to remove &FLT. from
&CFG. when removing a disk from VRAID's disk pool.
:eul.


:dt.Version 0.60 - 22/12-1998, published in VRAID10.ZIP
:dd.
:ul compact.
:LI.Added IOCtls to support VSETUP's detection "OS/2 device = Array
Drive"
:LI.Memory is now allocated as "LOCKTYPE_LONG_ANYMEM", should work on
machines with less than 16MB, too
:LI.actually implemented registration code
:eul.


:DT.Version 0.51 - 6/11-1998, published as beta version
:DD.
:UL compact.
:LI.smaller changes (WAIT_IS_INTERRUPTABLE)
:LI.switched from timebomb to "not registered" mode
:eul.



:dt.Version 0.50 - 7/10-1998, published as beta version
:dd.
:ul compact.
:LI.added IOCtls to allow VSetup.exe access disks when &FLT. is
installed.  Incomplete but needs some information about kernel and
OS2DASD working.

:LI.added "timebomb" code
:eul.



:dt.Version 0.40 - September 1998
:dd.

:ul compact.
:LI.fixed nasty bug causing hangs or traps with HPFS386.  Didn't
happen with simple HPFS...
:eul.



:dt.Version 0.30 - 25/6-1998
:dd.
:ul compact.
:LI.stable enough to install it on a working machine (Warp Server
Advanced with HPFS386)

:LI.changed to HOSTDRIVE -> VRAIDRIVE -> &PDEV. workers because
setup program happened to work that way (tip: write setup first and
driver afterwards, seems to eliminate a rework <g>).
:eul.



:dt.Version 0.20 - 29/5-1998
:dd.
:ul compact.
:LI.changed layout of administrative sectors
:eul.



:dt.Version 0.10 - 8/4-1998
:dd.
:ul compact.
:LI.accepts single, stripe, chain
:eul.

:edl.






.* ======================================================================
:H2 RES=7200.&EXE. History

:DL.

:DT.Version 0.84 - 11/02-2001, published in VRAID23.ZIP
:DD.
:UL compact.
:LI.when RAID 1 arrays are created a &BUILD. process is started
immediately.
:LI.user interface to verify arrays, start &REBUILD. processes,
declare a child invalid, remove a child, ...
:EUL.

:DT.Version 0.83 (send as 0.82 to some users with partitioning problems)
:DD.
:UL compact.
:LI.corrected creation of secondary partitions.  Whenever two or more
&LL.s did already exist a self-referencing table was created which
caused an endless loop on reboot (inside OS2DASD.DMD).
:LI.added precessing of changes in most presentation parameters.  Just
drop colors, fonts etc to &EXE.'s windows and be suprised what is
retained.
:EUL.

:DT.Version 0.82 - 15/04-2000, published in VRAID22.ZIP
:DD.
:UL compact.
:LI.creates VRAID partition inside extended partitions.
:LI.area dialog shows selected percentage as MB value.
:LI.hopefully fixed the overrun which prevented PHYSDEVs with more
than 9GB.
:EUL.


:DT.Version 0.81 - 27/11-1999, published in VRAID21.ZIP
:DD.
:UL compact.
:LI.modified to handle disks accessable through &FLT. and OS/2.  See
&FLT. version 0.80.
:LI.does no longer try to lock any disk from other programs (FDISK
doesn't do it neither).
:LI.prohibits restart of build processes if arrays were modified
:EUL.



:dt.Version 0.80 - 26/06-1999, published in VRAID20.ZIP
:dd.
:ul compact.
:LI.menu entry to disable driver beeping
:LI.add disk space to Mirror/Chain with DnD
:LI.remove members of Mirror
:LI.supports new layout of configuration sectors (Mirror)
:eul.



:dt.Version 0.71 -  30/4-1998, published in VSETUP071.ZIP
:dd.
:ul compact.
:LI.fixed a nasty bug which could lead to overwriting other partitions
or even the MBR!  Don't use any vsetup.exe below 0.71!
:LI.doesn't accept partition sizes below 10MB
:eul.


:dt.Version 0.70 -  25/3-1998, published in VSETUP07.ZIP
:dd.
:ul compact.
:LI.asks user whether it's OK to write to a "non-lockable"
disk. Suggestion from pdd@os2ss.com
:LI.completely recoded in C++, all arrays/container icons are objects!
:LI.displays internal IDs in icon descriptions.  Will be used when
drag'n drop is implemented.
:LI.corrected Drives->Dump code
:eul.


:dt.Version 0.60 -  22/12-1998, published in VRAID10.ZIP
:dd.
:ul compact.
:LI.added code to detect OS/2 devices which are array
drives. Hopefully this will work in any situation.  The effect is that
you no longer need to remove &FLT. if you want to modify or create &ARRAY.s.
:LI.displays which OS/2 devices are &ARRAY.s
:LI.removed that "beta, beta" warning
:eul.


:dt.Version 0.52 -  24/11-1998, published as beta version
:dd.
:ul compact.
:LI.BUGFIX: the code which searches OS/2 devices was removed from
0.51.  Sorry about that, hopefully this version will finally work (?)
:eul.


:dt.Version 0.51 -  6/11-1998, published as beta version
:dd.
:ul compact.
:LI.modifies &PDEV.s when &FLT. is installed but new PYHSDEVICEs
can't be defined (OS/2 device aren't scanned at all)
:LI.BUGFIX: see above, modify all already created &PDEV.s
:LI.BUGFIX: removed SYS3175 when not specifying array type or
partition position
:LI.BUGFIX: 0.50 was published as debug version &colon.-(
:eul.


:dt.Version 0.50 - 7/10-1998, published as beta version
:dd.
:ul compact.
:LI.added "beta, beta" messages
:LI.added code to :hp1.display:ehp1. configuration when &FLT. is loaded
:eul.


:dt.Version 0.30 - August 1998
:dd.
:ul compact.
:LI.working code
:eul.

:edl.






.* ============================================================
:H1 res=8000.ToDo List
:P.
VRAID isn't complete yet and may in fact never become.  There are
numerous ways to enhance configuration or operation.  However some
things have already been reported as annoying or missing and will
therefore been looked at.  This includes but is not limited the
following items:

:UL.
:LI.there situations where a &REBUILD. should be started automatically
but &FLT. of today needs a reboot.  Perhaps &FLT. could scan bad disks
in the background or &EXE. get's an option to force this scanning.

:LI.to better handle failed disks I am considering adding timeout
detection to &FLT..  This stuff should definitely handled inside the
responsible ADD but I'm so feed up how the adaptec driver I'm using
handles it :HP2.not:EHP2. that I might take the time and code this
inside &FLT..

:LI.RAID levels 4,5

:LI.Resource Manager Calls (plug n' pray <g>)
:EUL.

:P.
More?  IOCtls?  Documentation?  Netmail any ideas.






.* ============================================================
.* Footnotes
.* ============================================================


:FN ID=Features.:P.
As this is work in process only some RAID levels are
implemented. However, the intention is to supply all major RAID
levels to the average OS/2 user.
:EFN.

:FN ID=Limits.:P.
Well, there is a memory limitation in &FLT. but I don't expect you to
reach this.
:EFN.

:FN ID=nolevel3.:P.
As there is currently no working level 3 (e.g. RAID 5) code I will
not accept level 3 orders.
:EFN.

:FN ID=Error.:P.
An &ARRAY. is in :HP1.error:EHP1. state when there is data missing.
You can no longer access all of your data (you might not even able to
mount any filesystems residing on the &ARRAY.).
:P.
You should have a look to your backups...
:EFN.

:FN ID=Fail.:P.
An &ARRAY. is in :HP1.fail:EHP1. state when there is no longer
redundancy.  You can still access all data on the &ARRAY., there is no
data loss.  But the next failure will switch your &ARRAY. into
&ERROR. state.
:P.
For obvious resons a
:HP2.Single:EHP2.,
:HP2.Chaining:EHP2. or
:HP2.Striping:EHP2. device will never enter
:HP1.fail:EHP1. state&colon. it will enter &ERROR. state (Chaining,
Striping) or disappear completely (Single).
:EFN.

:FN ID=Build.:P.
A :HP1.Build:EHP1. is the first time creation of an disk array.
:EFN.

:FN ID=Rebuild.:P.
A :HP1.Rebuild:EHP1. is the automatic recreation of a disk array once
a failure has been detected and corrected.
:EFN.

:FN ID=Beep.:P.
&FLT. will beep regularly (once every 5 seconds) when a disk
failure resulting in a loss of redundancy is detected.  You should
replace the failed device as soon as possible.
:EFN.

:FN ID=MBR.:P.
The :HP1.MBR:EHP1. reside on physical sector 0 on every disk and holds
up to 4 partition entries.
:EFN.

:FN ID=lxlite.:P.
LXLite V1.2.1  By Andrew Zabolotny. An OS/2 executables packer. Lots
of features, maximal compression, nice interface (GNU) EN.
.BR
Available on http&colon.//hobbes.nmsu.edu as lxlt121.zip.
:EFN.


:EUSERDOC.
.* ==== End of Document =================================================
