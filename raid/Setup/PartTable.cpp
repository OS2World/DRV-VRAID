/*
 * $Source: R:/Source/driver/raid/Setup/RCS/PartTable.cpp,v $
 * $Revision: 1.5 $
 * $Date: 2000/05/28 17:37:22 $
 * $Locker:  $
 *
 *	Class PartTable to read/modify partition tables.
 *
 * $Log: PartTable.cpp,v $
 * Revision 1.5  2000/05/28 17:37:22  vitus
 * - adjusted verbosity levels
 *
 * Revision 1.4  2000/04/10 01:33:40  vitus
 * - *all* messages display disk no
 *
 * Revision 1.3  2000/03/17 02:06:47  vitus
 * - addForeign: handle positions above 1023 cylinder.
 * - addForeign: calculates cyl, hd, sec instead of using first/last hd,sec
 *
 * Revision 1.2  2000/03/09 23:14:57  vitus
 * - another constructor and other changes to keep and reference the position of
 *   the first extended partition.
 * - method clear()
 *
 * Revision 1.1  2000/02/14 00:48:58  vitus
 * Initial revision
 * ----------------------------------------
 * This code is Copyright Vitus Jensen 2000
 */
static char const vcid[]="$Id: PartTable.cpp,v 1.5 2000/05/28 17:37:22 vitus Exp $";
static char const modname[]="PartTable";

#include <assert.h>
#include <string.h>

#define INCL_DOS
#define INCL_DOSDEVIOCTL
#include <os2.h>

extern "C" {
#include "../../../Lib/defines.h"
#include "../../../Lib/verbose.h"
}
#include "../dsklayout.h"
#include "dskinfo.hpp"

#include "PartTable.hpp"





/*# ----------------------------------------------------------------------
 * PartTable::PartTable(d,n,rd)
 *
 * PARAMETER
 *	d		disk where partition table is located
 *	n		psn where it's located
 *	rd		read only?
 *
 * RETURNS
 *	(constructor)
 *
 * DESCRIPTION
 * REMARKS
 */
PartTable::PartTable(VOS2Disk * d,ULONG n,Boolean rd)
{
    disk = d;
    secno = n;
    rderr = 0;
    modified = FALSE;
    valid = TRUE;
    extstart = 0;				/* not an extended partition */

    entry = bootrecord.PartitionTable;

    Verbose(2, modname, "disk %lu, psn %lu", disk->queryIndex(), secno);

    readRecord();
    if( rd == true )
	rderr = 0xFFFF;				/* any error will inhibit writing */
    return;
}




/*# ----------------------------------------------------------------------
 * PartTable::PartTable(d,n,first_extended_start)
 *
 * PARAMETER
 *	d			disk where partition table is located
 *	n			psn where it's located
 *	first_extended_start	start of first extended partition
 *
 * RETURNS
 *	(constructor)
 *
 * DESCRIPTION
 * REMARKS
 */
PartTable::PartTable(VOS2Disk * d,ULONG n,ULONG first_extended_start)
{
    disk = d;
    secno = n;
    rderr = 0;
    modified = FALSE;
    valid = TRUE;
    extstart = first_extended_start;
    assert( secno != 0 );

    entry = bootrecord.PartitionTable;

    Verbose(2, modname, "disk %lu, psn %lu", disk->queryIndex(), secno);

    readRecord();

    return;
}




/*# ----------------------------------------------------------------------
 * PartTable::~PartTable()
 *
 * PARAMETER
 *	(none)
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Destructor
 *
 * REMARKS
 */
PartTable::~PartTable()
{
    flush();
}




void
PartTable::clear(void)
{
    if( rderr != 0 )
	return;

    memset(&bootrecord, 0, sizeof(bootrecord));
    bootrecord.Signature = 0xAA55;
    valid = FALSE;				/* only write if modified */
}




/*# ----------------------------------------------------------------------
 * PartTable::readRecord(void)
 *
 * PARAMETER
 *	(none)
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Reads partition table from disk.
 *
 * REMARKS
 */
void
PartTable::readRecord(void)
{
    DEVICEPARAMETERBLOCK dpb = disk->queryDPB();
    USHORT const	spt = dpb.cSectorsPerTrack;
    ULONG const		cylsize = dpb.cHeads * spt;

    if( (secno % cylsize) != 0 )
    {
	secno /= cylsize;
	secno *= cylsize;
    }

    rderr = disk->read(secno, 1, &bootrecord);
    if( rderr != 0 )
    {
	/* 'rderr' is now /0,  the disk will never be written to. */

	Verbose(1, modname, "can't read disk %lu, psn %lu - rc %lu",
		disk->queryIndex(), secno, rderr);
    }

    if( bootrecord.Signature != 0xAA55 )
    {
	Verbose(1, modname,
		"clearing invalid partition table (disk %lu, psn %lu)",
		disk->queryIndex(), secno);
	memset(&bootrecord, 0, sizeof(bootrecord));
	bootrecord.Signature = 0xAA55;
	valid = FALSE;				/* only write if modified */
    }
}




/*# ----------------------------------------------------------------------
 * PartTable::flush()
 *
 * PARAMETER
 *	(none)
 *
 * RETURNS
 *	APIRET
 *
 * DESCRIPTION
 *	Writes any modified data back to disk.
 *
 * REMARKS
 */
APIRET
PartTable::flush()
{
    if( rderr != 0 )
	return rderr;				/* don't write if not read! */

    APIRET	rc = 0;
    if( modified )
    {
	rc = disk->write(secno, 1, &bootrecord);
	if( rc != 0 )
	{
	    Verbose(1, modname, "can't write disk %lu, psn %lu - rc %lu",
		    disk->queryIndex(), secno, rc);
	}
	else
	{
	    modified = FALSE;
	    valid = TRUE;
	}
    }
    return rc;
}




/*# ----------------------------------------------------------------------
 * PartTable::delPE(partentry)
 *
 * PARAMETER
 *	partentry	points to partition entry to clear
 *
 * RETURNS
 *	TRUE		did as caller wanted
 *	FALSE		Some error.  Probably invalid pointer passed.
 *
 * DESCRIPTION
 *	Clears the requested partition entry.
 *
 * REMARKS
 */
BOOL
PartTable::delPE(PPARTITIONENTRY partentry)
{
    Verbose(2, modname, "delPE(disk %lu, table %lu, type %#x, rel %lu, size %lu)",
	    disk->queryIndex(), secno,
	    partentry->SysIndicator,
	    partentry->RelativeSectors, partentry->NumSectors);

    if( rderr != 0 )
	return FALSE;

    BOOL	done = FALSE;
    for( int i = 0; i < 4; ++i )
    {
	if( partentry == &bootrecord.PartitionTable[i] )
	{
	    memset(&bootrecord.PartitionTable[i], 0, sizeof(*partentry));
	    modified = TRUE;
	    done = TRUE;
	    break;
	}
    }
    return done;
}




/*# ----------------------------------------------------------------------
 * PartTable::addPE(partentry)
 *
 * PARAMETER
 *	partentry	partition entry to add
 *
 * RETURNS
 *	TRUE		did as caller wanted
 *	FALSE		Some error.  Probably invalid pointer passed.
 *
 * DESCRIPTION
 *	Copies 'partentry' to partition sector.
 *
 * REMARKS
 */
BOOL
PartTable::addPE(PARTITIONENTRY const &partentry)
{
    Verbose(2, modname, "addPE(disk %lu, table %lu, type %#x, rel %lu, size %lu)",
	    disk->queryIndex(), secno,
	    partentry.SysIndicator,
	    partentry.RelativeSectors, partentry.NumSectors);

    if( rderr != 0 )
	return FALSE;

    int		free = -1;
    for( int i = 0; i < 4; ++i )
	if( bootrecord.PartitionTable[i].SysIndicator == 0 )
	{
	    free = i;
	    break;
	}
    if( free == -1 )
    {
	Verbose(1, modname, "addPE(disk %lu, table %lu) - no free entry",
		disk->queryIndex(), secno);
	return FALSE;				/* scanning assured a free entry? */
    }

    bootrecord.PartitionTable[i] = partentry;
    modified = TRUE;
    return TRUE;
}




/*# ----------------------------------------------------------------------
 * PartTable::addForeign(type,bootable,start,size)
 *
 * PARAMETER
 *	type
 *	bootable
 *	start		start of partition [absolute]
 *	size		size of partition [sectors]
 *
 * RETURNS
 *	TRUE		OK, all done
 *	FALSE		some error occurred
 *
 * DESCRIPTION
 * REMARKS
 */
BOOL
PartTable::addForeign(UCHAR type,UCHAR bootable,ULONG start,ULONG size)
{
    Verbose(2, modname, "addForeign(disk %lu, table %lu, type %#x,"
	    " start %lu, size %lu)",
	    disk->queryIndex(), secno, type, start, size);

    if( rderr != 0 )
	return FALSE;

    DEVICEPARAMETERBLOCK dpb = disk->queryDPB();
    USHORT const	spt = dpb.cSectorsPerTrack;
    ULONG const		cylsize = dpb.cHeads * spt;

    USHORT cyl;
    UCHAR  sc, hd;

    PARTITIONENTRY	pe;

    pe.BootIndicator = bootable;

    cyl = (USHORT)(start / cylsize);
    sc = (start % spt) + 1;
    hd = (start / spt) % dpb.cHeads;
    if( cyl >= 1024 )
    {
	pe.BegHead = dpb.cHeads - 1;
	pe.BegSector = 0xE0;
	pe.BegCylinder = 0xFF;			/* Cylinder 1023, Sector 0 */
    }
    else
    {
	pe.BegHead = hd;
	pe.BegSector = (UCHAR)((HIUCHAR(cyl) << 6) | sc);
	pe.BegCylinder = LOUCHAR(cyl);
    }

    pe.SysIndicator = type;

    ULONG	last = start + size - 1;
    cyl = (USHORT)(last / cylsize);
    sc = (last % spt) + 1;
    hd = (last / spt) % dpb.cHeads;
    if( cyl >= 1024 )
    {
	pe.EndHead = dpb.cHeads - 1;
	pe.EndSector = 0xE0;
	pe.EndCylinder = 0xFF;			/* Cylinder 1023, Sector 0 */
    }
    else
    {
	pe.EndHead = hd;
	pe.EndSector = (UCHAR)((HIUCHAR(cyl) << 6) | sc);
	pe.EndCylinder = LOUCHAR(cyl);
    }
    if( type == EXTENDED_PARTTYPE )
	pe.RelativeSectors = start - extstart;	/* extended refer to the first */
    else
	pe.RelativeSectors = start - secno;
    pe.NumSectors = size;

    return addPE(pe);
}




/*# ----------------------------------------------------------------------
 * PartTable::addOwn(type,bootable,start,size)
 *
 * PARAMETER
 *	type
 *	bootable	0 or 0x80
 *	start		start of partition [absolute]
 *	size		size of partition [sectors]
 *
 * RETURNS
 *	TRUE		did as you requested
 *	FALSE		something went wrong, nothing changed
 *
 * DESCRIPTION
 *	Adjust size according to geometry and update partition table(s).
 *
 * REMARKS
 */
BOOL
PartTable::addOwn(UCHAR type,UCHAR bootable,ULONG start,ULONG size)
{
    Verbose(2, modname, "addOwn(disk %lu, table %lu, type %#x,"
	    " start %lu, size %lu)",
	    disk->queryIndex(), secno, type, start, size);

    if( rderr != 0 )
	return FALSE;

    /* Normalize ending point to last sector of cylinder!
     * .start is already correct. xxx */

    DEVICEPARAMETERBLOCK dpb = disk->queryDPB();
    ULONG const		cylsize = dpb.cHeads * dpb.cSectorsPerTrack;
    ULONG		end = size + start;

    if( (end % cylsize) != 0 )
    {
	end /= cylsize;
	end *= cylsize;
	Verbose(2, modname, "size adjusted to %lu", end - start);
	size = end - start;
    }
    return addForeign(type, bootable, start, size);
}
