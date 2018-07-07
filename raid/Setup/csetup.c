/*
 * $Source: R:/source/driver/raid/setup/RCS\\csetup.c,v $
 * $Revision: 1.5 $
 * $Date: 1998/09/20 20:36:52 $
 * $Locker:  $
 *
 *	Commandline setup (isn't it visual? :).
 *
 * $Log: csetup.c,v $
 * Revision 1.5  1998/09/20 20:36:52  vitus
 * - PDsk*() now in seperate include file physdsk.h
 *
 * Revision 1.4  1998/04/22 00:14:42  vitus
 * - hex AND ascii dump
 * - creates chain and stripe drives
 * - clears first data sector after creating host drive
 *
 * Revision 1.3  1998/03/26 02:37:00  vitus
 * - uses PDskRead/PDskWrite: LSN based
 * - more intelligent dumps
 * - dumps first data sector, too
 * - BuildParttitionTable might coexist with other partition entries (enable?)
 *
 * Revision 1.2  1998/03/25 00:44:51  vitus
 * - may create SINGLE hostdrives
 * - complete dump of VRAID disk structures
 *
 * Revision 1.1  1998/03/11 23:01:33  vitus
 * Initial revision
 * ----------------------------------------
 * This code is Copyright Vitus Jensen 1998
 */
static char const id[]="$Id: csetup.c,v 1.5 1998/09/20 20:36:52 vitus Stab $";

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INCL_DOS
#define INCL_DOSDEVIOCTL
#define INCL_ERRORS
#include <os2.h>

#include "../../../tools/defines.h"
#include "../../../tools/verbose.h"
#include "../../../tools/physdsk.h"
#include "../dsklayout.h"
#include "proto.h"


/*
 * Error codes
 */
#define ERR_PARAMETER		-1
#define ERR_INVALIDDRIVE	-2
#define ERR_DATAOVERWRITE	-3


typedef struct _CHILDTAB {
    DEVID	id;
    ULONG	size;
} CHILDTAB, * PCHILDTAB;



PUBLIC char	szPrgName[_MAX_PATH];
PUBLIC USHORT 	usHostId = 0;



PRIVATE void
DumpBuffer(VOID const * buffer,USHORT const cb)
{
    UCHAR	 ascii[17];
    UCHAR const * p = buffer;
    USHORT	 i;

    for( i = 0; i < cb; ++i, ++p )
    {
	if( (i % 16) == 0 )
	    printf("%04X: ", i);
	printf("%02X", *p);
	ascii[i%16] = (isprint(*p) ? *p : '.');

	if( (i+1) % 16 == 0 )
	{
	    ascii[16] = '\0';
	    printf("\t%s\n", ascii);
	}
	else if( (i+1) % 8 == 0 )
	    putchar('-');
	else
	    putchar(' ');
    }
    return;
}


PRIVATE void
Usage(FILE * out)
{
    fprintf(out, "Usage: %s [-?]\n", szPrgName);
    return;
}


PRIVATE void
Help(void)
{
    Usage(stdout);
    printf("\n"
	   "-?\t\tthis text\n"
	   "-D [n]\t\tdump disk structures of drive n or all\n"
	   "-S n\t\tcreate hostdrive from single on drive n\n"
	   "-C n1 [n2 ...]\tcreate hostdrive from chain using n1 to ....\n"
	   "-P n1 [n2 ...]\tcreate hostdrive from stripe using n1 to ....\n"
	);
}




/*#
 * CALL
 *	DumpPartitions(which)
 * PARAMETER
 *	which		drive to dump or -1 for all drives
 * RETURNS
 *	0		OK
 * GLOBAL
 * DESCRIPTION
 * REMARKS
 */
PRIVATE int
DumpPartitions(ULONG which)
{
    static UCHAR buffer[512];
    APIRET	rc;
    ULONG	devidx, devcnt;
    int		i;

    rc = PDskEnum(&devcnt);
    if( rc )
    {
	Verbose(0, "VSETUP", "PDskEnum - rc %lu", rc);
	return rc;
    }
    printf("Physical disks: %lu\n", devcnt);

    for( devidx = 0; devidx < devcnt; ++devidx )
    {
	ULONG			hd;
	DEVICEPARAMETERBLOCK	dp;
	USHORT			dataoffset;
	ULONG			partoffset = 0;

	if( which != (ULONG)-1  &&  devidx != which )
	    continue;				/* ignore this one */

	rc = PDskOpen(devidx, &hd);
	if( rc != 0 )
	{
	    Verbose(0, "VSETUP", "PDskOpen - rc %lu", rc);
	    continue;
	}
	printf("======== Disk %lu, handle %lu ========\n", devidx, hd);

	rc = PDskQueryParam(hd, &dp);
	if( rc != 0 )
	{
	    Verbose(0, "VSETUP", "PDskQueryParam - rc %lu", rc);
	    continue;
	}

	printf("Physical drive parameters: Cylinders: %d, Heads: %d,"
	       " Sectors/Track: %d\n",
	       dp.cCylinders, dp.cHeads, dp.cSectorsPerTrack);
	printf("Drive capacity: %lu MBytes\n",
	       (ULONG)(((ULONG)dp.cSectorsPerTrack
			* 512 * ((ULONG)dp.cHeads)
			* (ULONG)dp.cCylinders) / 1024) / 1024);

	rc = PDskRead(hd, 0, 1, buffer);
	if( rc != 0 )
	{
	    Verbose(0, "VSETUP", "PDskRead - rc %lu", rc);
	    continue;
	}

	printf("-------- Dump of real partition sector --------\n");
	DumpBuffer(buffer, 512);
	{
	    PMBR	sec = (PVOID)buffer;
	    int		i;

	    for( i = 0; i < 4; ++i )
	    {
		if( sec->PartitionTable[i].SysIndicator == VRAID_PARTTYPE )
		{
		    partoffset = sec->PartitionTable[i].RelativeSectors;
		    break;
		}
	    }
	    if( partoffset == 0 )
	    {
		Verbose(1, "VSETUP", "no VRAID partition on drive %u", devidx);
		continue;
	    }
	}

	rc = PDskRead(hd, partoffset, 1, buffer);
	if( rc != 0 )
	{
	    Verbose(0, "VSETUP", "PDskRead - rc %lu", rc);
	    continue;
	}

	printf("-------- Dump of PHYSDEV sector (%lu) --------\n", partoffset);
	DumpBuffer(buffer, 512);
	{
	    PSEC_PHYSDEV	sec = (PVOID)buffer;

	    dataoffset = sec->u.s.adminspace;
	}

	for( i = 1; (((PSEC_VRDEV)buffer)->u.s.flags & 0x80) == 0; ++i )
	{
	    rc = PDskRead(hd, partoffset+i, 1, buffer);
	    if( rc != 0 )
	    {
		Verbose(0, "VSETUP", "PDskRead - rc %lu", rc);
		break;
	    }

	    printf("-------- Dump of VRDEV sector (%lu) --------\n", partoffset+i);
	    DumpBuffer(buffer, 512);
	}

	/* Now dump first data sector, it may contain a partition sector. */

	rc = PDskRead(hd, partoffset+dataoffset, 1, buffer);
	if( rc != 0 )
	{
	    Verbose(0, "VSETUP", "PDskRead - rc %lu", rc);
	    break;
	}

	printf("-------- Dump of first data sector (%lu) --------\n",
	       partoffset+dataoffset);
	DumpBuffer(buffer, 512);

	PDskClose(hd);
    }

    return 0;
}




/*#
 * CALL
 *	BuildPartitionTable(dp,mbr,start,size)
 * PARAMETER
 *	dp
 *	mbr
 *	start		returns start of partition
 *	size		returns size of partition
 * RETURNS
 * GLOBAL
 * DESCRIPTION
 * REMARKS
 */
PRIVATE APIRET
BuildPartitionTable(PDEVICEPARAMETERBLOCK dp,PMBR mbr,PULONG start,PULONG size)
{
    int	i;

#if 0
    Verbose(2, "VSETUP", "dump of sector:");
    DumpBuffer(mbr, sizeof(*mbr));
#endif
    if( mbr->Signature != 0xAA55 )
    {
	Verbose(1, "VSETUP", "clearing invalid MBR");
	memset(mbr, 0, sizeof(*mbr));
	mbr->Signature = 0xAA55;
    }
    for( i = 0; i < 4; ++i )
    {
	if( mbr->PartitionTable[i].SysIndicator != 0 )
	{
	    if( mbr->PartitionTable[i].SysIndicator == VRAID_PARTTYPE )
		break;
	    else
	    {
		Verbose(0, "VSETUP", "disk contains partition[s]!");
		return ERR_DATAOVERWRITE;
	    }
	}
    }
    if( i > 3 )	i = 0;

    mbr->PartitionTable[i].BootIndicator = 0;	/* don't boot from here */
    mbr->PartitionTable[i].BegHead = 1;
    mbr->PartitionTable[i].BegSector = 1;
    mbr->PartitionTable[i].BegCylinder = 0;
    mbr->PartitionTable[i].SysIndicator = VRAID_PARTTYPE;
    mbr->PartitionTable[i].EndHead = dp->cHeads - 1;
    mbr->PartitionTable[i].EndSector = dp->cSectorsPerTrack;
    mbr->PartitionTable[i].EndCylinder = dp->cCylinders - 1;
    mbr->PartitionTable[i].RelativeSectors = dp->cSectorsPerTrack;
    mbr->PartitionTable[i].NumSectors = dp->cSectorsPerTrack
	* (ULONG)dp->cHeads * dp->cCylinders - dp->cSectorsPerTrack;

    *start = mbr->PartitionTable[i].RelativeSectors;
    *size = mbr->PartitionTable[i].NumSectors;
    return 0;
}




/*#
 * CALL
 *	BuildPhysdevSector(sec,size,flags)
 * PARAMETER
 * RETURNS
 * GLOBAL
 * DESCRIPTION
 * REMARKS
 */
PRIVATE APIRET
BuildPhysdevSector(PSEC_PHYSDEV const sec,ULONG partsize,UCHAR flags)
{
    int	i;

    if( memcmp(sec->sectype, "PHYSDEVICE      ", sizeof(sec->sectype)) == 0
	&&  Crc16(sec, sizeof(*sec)-2) == sec->crc )
    {
	Verbose(1, "VSETUP", "PHYSDEVICE sector already valid");
    }
    else
    {
	memset(sec, 0, sizeof(*sec));
	memcpy(sec->sectype, "PHYSDEVICE      ", sizeof(sec->sectype));
	memcpy(sec->u.s.id, &usHostId, sizeof(usHostId));
	*(PULONG)&sec->u.s.id[2] = time(NULL);
    }
    sec->u.s.adminspace = VRAID_ADMINSECTORS;
    sec->u.s.size = partsize - VRAID_ADMINSECTORS;

    sec->u.s.flags = flags;			/* as caller reqested */
    sec->timestamp = time(NULL);

    sec->crc = Crc16(sec, sizeof(*sec)-2);
#if 0
    Verbose(2, "VSETUP", "dump of sector:");
    DumpBuffer(sec, sizeof(*sec));
#endif
    return 0;
}




PRIVATE APIRET
BuildVraidSector(PSEC_VRDEV const sec,UCHAR type,UCHAR flags,
		 UCHAR childs,CHILDTAB child[])
{
    int	i;

    memset(sec, 0, sizeof(*sec));
    memcpy(sec->sectype, "VRAIDDEVICE     ", sizeof(sec->sectype));
    sec->timestamp = time(NULL);

    memcpy(sec->u.s.id, &usHostId, sizeof(usHostId));
    *(PULONG)&sec->u.s.id[2] = time(NULL);
    sec->u.s.type = type;			/* as caller reqested */
    sec->u.s.flags = flags;			/* --- "" --- */
    sec->u.s.childs = childs;			/* --- "" --- */

    for( i = 0; i < childs; ++i )
    {
	memcpy(sec->u.s.child[i].id, child[i].id, sizeof(sec->u.s.child[i].id));
	sec->u.s.child[i].size = child[i].size;
    }
    sec->crc = Crc16(sec, sizeof(*sec)-2);
#if 0
    Verbose(2, "VSETUP", "dump of sector:");
    DumpBuffer(sec, sizeof(*sec));
#endif
    return 0;
}




/* **********************************************************************
 * **********************************************************************
 * ******************************************************************* */


/*#
 * CALL
 *	MakeSingle(which)
 * PARAMETER
 *	which		physical drive to access
 * RETURNS
 *	0		OK
 *	/0		APIRET or negative number
 * GLOBAL
 * DESCRIPTION
 * REMARKS
 */
PRIVATE int
MakeSingle(ULONG const which)
{
    static UCHAR buffer[SECTOR_SIZE];
    APIRET	rc;
    ULONG	count, dummy;
    ULONG	hd = 0;

    rc = PDskEnum(&count);
    if( rc )
    {
	Verbose(0, "VSETUP", "PDskEnum - rc %lu", rc);
	return rc;
    }
    Verbose(2, "VSETUP", "physical disks: %lu", count);
    if( which >= count )
    {
	Verbose(0, "VSETUP", "invalid drive index %lu (0-%lu)", which, count-1);
	return ERR_INVALIDDRIVE;;
    }

    do
    {
	DEVICEPARAMETERBLOCK	dp;
	ULONG			ulPartStart;
	ULONG			ulPartSize;

	rc = PDskOpen(which, &hd);
	if( rc != 0 )
	{
	    Verbose(0, "VSETUP", "PDskOpen - rc %lu", rc);
	    break;
	}
	Verbose(2, "VSETUP", "======== Disk %lu, handle %lu ========", which, hd);

	rc = PDskQueryParam(hd, &dp);
	if( rc != 0 )
	{
	    Verbose(0, "VSETUP", "PDskQueryParam - rc %lu", rc);
	    break;
	}

	Verbose(3, "VSETUP", "physical drive parameters:");
	Verbose(3, "VSETUP", "Cylinders: %d, Heads: %d, Sectors/Track: %d",
		dp.cCylinders, dp.cHeads, dp.cSectorsPerTrack);
	Verbose(3, "VSETUP", "drive capacity: %lu MBytes",
		(ULONG)(((ULONG)dp.cSectorsPerTrack
			 * 512 * ((ULONG)dp.cHeads)
			 * (ULONG)dp.cCylinders) / 1024) / 1024);


	/* First: lock disk, we want no other to modify it now. */

	rc = PDskLock(hd);
	if( rc != 0 )
	{
	    Verbose(0, "VSETUP", "PDskLock - rc %lu", rc);
	    break;
	}

	/* Read, modify, write partition table (master boot record). */

	Verbose(2, "VSETUP", "-------- Partition Sector --------");

	rc = PDskRead(hd, 0, 1, buffer);
	if( rc != 0 )
	{
	    Verbose(0, "VSETUP", "PDskRead - rc %lu", rc);
	    break;
	}
	rc = BuildPartitionTable(&dp, (PMBR)buffer, &ulPartStart, &ulPartSize);
	if( rc != 0 )
	    break;
	rc = PDskWrite(hd, 0, 1, buffer);
	if( rc != 0 )
	{
	    Verbose(0, "VSETUP", "PDskWrite - rc %lu", rc);
	    break;
	}
#if 0
	printf("-------- Dump of real partition sector --------\n");
	DumpBuffer(buffer, 512);
#endif

	/* Read, modify, write first sector in VRAID partition (PHYSDEV record). */

	Verbose(2, "VSETUP", "-------- PHYSDEV Sector (%lu) --------", ulPartStart);

	rc = PDskRead(hd, ulPartStart, 1, buffer);
	if( rc != 0 )
	{
	    Verbose(0, "VSETUP", "PDskRead - rc %lu", rc);
	    break;
	}

	rc = BuildPhysdevSector((PVOID)buffer, ulPartSize, 0x80);
	if( rc != 0 )
	{
	    Verbose(0, "VSETUP", "BuildPhysdevSector - rc %lu", rc);
	    break;
	}

	rc = PDskWrite(hd, ulPartStart, 1, buffer);
	if( rc != 0 )
	{
	    Verbose(0, "VSETUP", "PDskWrite - rc %lu", rc);
	    break;
	}
#if 0
	printf("-------- Dump of PHYSDEV sector (%lu) --------\n", ulPartStart);
	DumpBuffer(buffer, SECTOR_SIZE);
#endif

#if 1
	/* Clear first data sector (will become partition sector) */

	memset(buffer, 0, SECTOR_SIZE);
	rc = PDskWrite(hd, ulPartStart+VRAID_ADMINSECTORS, 1, buffer);
	if( rc != 0 )
	{
	    Verbose(0, "VSETUP", "PDskWrite - rc %lu", rc);
	    break;
	}
#endif
    }
    while(0);

    if( hd != 0 )
    {
	PDskUnlock(hd);
	PDskClose(hd);
    }
    return rc;
}




/*#
 * CALL
 *	MakeChain(cnt,which[])
 * PARAMETER
 *	cnt		count of drives involved
 *	which		physical drives to build from
 * RETURNS
 *	0		OK
 *	/0		APIRET or negative number
 * GLOBAL
 * DESCRIPTION
 *	Build RAID 0.5 device.
 *
 * REMARKS
 */
PRIVATE int
MakeChain(int cnt,ULONG const which[])
{
    static UCHAR buffer[SECTOR_SIZE];
    APIRET	rc;
    ULONG	devcnt;
    int		i;
    struct _DSKINFO {
	ULONG			hd;
	DEVICEPARAMETERBLOCK	dp;
	DEVID			physid;
	ULONG			partstart;
	ULONG			partsize;
    } * const dskinfo = malloc(cnt * sizeof(struct _DSKINFO));

    if( dskinfo == NULL )
    {
	Verbose(0, "VSETUP", "malloc failed");
	return ERROR_NOT_ENOUGH_MEMORY;
    }
    memset(dskinfo, 0, cnt * sizeof(struct _DSKINFO));

    rc = PDskEnum(&devcnt);
    if( rc )
    {
	Verbose(0, "VSETUP", "PDskEnum - rc %lu", rc);
	free(dskinfo);
	return rc;
    }
    Verbose(2, "VSETUP", "physical disks: %lu", devcnt);

    for( i = 0; i < cnt; ++i )
	if( which[i] >= devcnt )
	{
	    Verbose(0,"VSETUP","invalid drive index %lu (0-%lu)",which[i],devcnt-1);
	    free(dskinfo);
	    return ERR_INVALIDDRIVE;;
	}

    do
    {
	/* Do preliminary tasks for all involved devices. */

	for( i = 0; i < cnt; ++i )
	{
	    /* We need a handle for every device, that's for sure. */

	    rc = PDskOpen(which[i], &dskinfo[i].hd);
	    if( rc != 0 )
	    {
		Verbose(0, "VSETUP", "PDskOpen(%lu) - rc %lu", which[i], rc);
		break;
	    }
	    Verbose(2, "VSETUP", "======== Disk %lu, handle %lu ========",
		    which[i], dskinfo[i].hd);

	    /* Some informations may be nice. */

	    rc = PDskQueryParam(dskinfo[i].hd, &dskinfo[i].dp);
	    if( rc != 0 )
	    {
		Verbose(0, "VSETUP", "PDskQueryParam - rc %lu", rc);
		break;
	    }

	    Verbose(3, "VSETUP", "physical drive parameters:");
	    Verbose(3, "VSETUP", "Cylinders: %d, Heads: %d, Sectors/Track: %d",
		    dskinfo[i].dp.cCylinders, dskinfo[i].dp.cHeads,
		    dskinfo[i].dp.cSectorsPerTrack);
	    Verbose(3, "VSETUP", "drive capacity: %lu MBytes",
		    (ULONG)(((ULONG)dskinfo[i].dp.cSectorsPerTrack
			     * SECTOR_SIZE * ((ULONG)dskinfo[i].dp.cHeads)
			     * (ULONG)dskinfo[i].dp.cCylinders) / 1024) / 1024);

	    /* And please, don't disturb us. */

	    rc = PDskLock(dskinfo[i].hd);
	    if( rc != 0 )
	    {
		Verbose(0, "VSETUP", "PDskLock - rc %lu", rc);
		break;
	    }
	}
	if( rc != 0 )
	    break;


	/* Read, modify, write partition table (master boot record) of
	 * all involved devices.  They need a VRAID partition. */

	for( i = 0; i < cnt; ++i )
	{
	    Verbose(2, "VSETUP", "-------- Partition Sector --------");

	    rc = PDskRead(dskinfo[i].hd, 0, 1, buffer);
	    if( rc != 0 )
	    {
		Verbose(0, "VSETUP", "PDskRead - rc %lu", rc);
		break;
	    }
	    rc = BuildPartitionTable(&dskinfo[i].dp, (PMBR)buffer,
				     &dskinfo[i].partstart, &dskinfo[i].partsize);
	    if( rc != 0 )
		break;

	    rc = PDskWrite(dskinfo[i].hd, 0, 1, buffer);
	    if( rc != 0 )
	    {
		Verbose(0, "VSETUP", "PDskWrite - rc %lu", rc);
		break;
	    }
#if 0
	    printf("-------- Dump of real partition sector --------\n");
	    DumpBuffer(buffer, 512);
#endif
	}
	if( rc != 0 )
	    break;


	/* Read, modify, write first sector in VRAID partition
	 * (PHYSDEV record) on every involved device. */

	for( i = 0; i < cnt; ++i )
	{
	    Verbose(2, "VSETUP", "-------- PHYSDEV Sector --------");

	    rc = PDskRead(dskinfo[i].hd, dskinfo[i].partstart, 1, buffer);
	    if( rc != 0 )
	    {
		Verbose(0, "VSETUP", "PDskRead - rc %lu", rc);
		break;
	    }

	    rc = BuildPhysdevSector((PVOID)buffer, dskinfo[i].partsize, 0);
	    if( rc != 0 )
	    {
		Verbose(0, "VSETUP", "BuildPhysdevSector - rc %lu", rc);
		break;
	    }
	    memcpy(dskinfo[i].physid, ((PSEC_PHYSDEV)buffer)->u.s.id,
		   sizeof(dskinfo[i].physid));

	    rc = PDskWrite(dskinfo[i].hd, dskinfo[i].partstart, 1, buffer);
	    if( rc != 0 )
	    {
		Verbose(0, "VSETUP", "PDskWrite - rc %lu", rc);
		break;
	    }
#if 0
	    printf("-------- Dump of PHYSSDEV sector (%lu) --------\n",
		   dskinfo[i].partstart);
	    DumpBuffer(buffer, 512);
#endif
	}
	if( rc != 0 )
	    break;

	/* Now build a VRDEV sector on next (second) sector of VRAID
	 * partition on every involved device. */

	{
	    PCHILDTAB childs  = malloc(cnt * sizeof(CHILDTAB));

	    if( childs == NULL )
	    {
		Verbose(0, "VSETUP", "malloc failed");
		rc = ERROR_NOT_ENOUGH_MEMORY;
		break;
	    }
	    for( i = 0; i < cnt; ++i )
	    {
		memcpy(childs[i].id, dskinfo[i].physid, sizeof(DEVID));
		childs[i].size = dskinfo[i].partsize - VRAID_ADMINSECTORS;
	    }

	    for( i = 0; i < cnt; ++i )
	    {
		Verbose(2, "VSETUP", "-------- VRAIDDEVICE Sector --------");

		rc = PDskRead(dskinfo[i].hd, dskinfo[i].partstart+1, 1, buffer);
		if( rc != 0 )
		{
		    Verbose(0, "VSETUP", "PDskRead - rc %lu", rc);
		    break;
		}

		rc = BuildVraidSector((PVOID)buffer, RDTYPE_CHAIN, 0x80, cnt,childs);
		if( rc != 0 )
		{
		    Verbose(0, "VSETUP", "BuildVraidSector - rc %lu", rc);
		    break;
		}

		rc = PDskWrite(dskinfo[i].hd, dskinfo[i].partstart+1, 1, buffer);
		if( rc != 0 )
		{
		    Verbose(0, "VSETUP", "PDskWrite - rc %lu", rc);
		    break;
		}
#if 0
		printf("-------- Dump of VRAID sector (%lu) --------\n",
		       dskinfo[i].partstart+1);
		DumpBuffer(buffer, SECTOR_SIZE);
#endif

#if 1
		if( i == 0 )
		{
		    /* Clear first data sector (will become partition sector) */

		    memset(buffer, 0, SECTOR_SIZE);
		    rc = PDskWrite(dskinfo[i].hd,
				   dskinfo[i].partstart+VRAID_ADMINSECTORS,
				   1, buffer);
		    if( rc != 0 )
		    {
			Verbose(0, "VSETUP", "PDskWrite - rc %lu", rc);
			break;
		    }
		}
#endif
	    } /* end[for()] */

	    free(childs);
	}
    }
    while( 0 );


    for( i = 0; i < cnt; ++i )
	if( dskinfo[i].hd != 0 )
	{
	    PDskUnlock(dskinfo[i].hd);
	    PDskClose(dskinfo[i].hd);
	}
    free(dskinfo);
    return rc;
}




/*#
 * CALL
 *	MakeStripe(cnt,which[])
 * PARAMETER
 *	cnt		count of drives involved
 *	which		physical drives to build from
 * RETURNS
 *	0		OK
 *	/0		APIRET or negative number
 * GLOBAL
 * DESCRIPTION
 *	Build RAID 0 device.
 *
 * REMARKS
 */
PRIVATE int
MakeStripe(int cnt,ULONG const which[])
{
    static UCHAR buffer[SECTOR_SIZE];
    APIRET	rc;
    ULONG	devcnt;
    int		i;
    struct _DSKINFO {
	ULONG			hd;
	DEVICEPARAMETERBLOCK	dp;
	DEVID			physid;
	ULONG			partstart;
	ULONG			partsize;
    } * const dskinfo = malloc(cnt * sizeof(struct _DSKINFO));

    if( dskinfo == NULL )
    {
	Verbose(0, "VSETUP", "malloc failed");
	return ERROR_NOT_ENOUGH_MEMORY;
    }
    memset(dskinfo, 0, cnt * sizeof(struct _DSKINFO));

    rc = PDskEnum(&devcnt);
    if( rc )
    {
	Verbose(0, "VSETUP", "PDskEnum - rc %lu", rc);
	free(dskinfo);
	return rc;
    }
    Verbose(2, "VSETUP", "physical disks: %lu", devcnt);

    for( i = 0; i < cnt; ++i )
	if( which[i] >= devcnt )
	{
	    Verbose(0,"VSETUP","invalid drive index %lu (0-%lu)",which[i],devcnt-1);
	    free(dskinfo);
	    return ERR_INVALIDDRIVE;;
	}

    do
    {
	/* Do preliminary tasks for all involved devices. */

	for( i = 0; i < cnt; ++i )
	{
	    /* We need a handle for every device, that's for sure. */

	    rc = PDskOpen(which[i], &dskinfo[i].hd);
	    if( rc != 0 )
	    {
		Verbose(0, "VSETUP", "PDskOpen(%lu) - rc %lu", which[i], rc);
		break;
	    }
	    Verbose(2, "VSETUP", "======== Disk %lu, handle %lu ========",
		    which[i], dskinfo[i].hd);

	    /* Some informations may be nice. */

	    rc = PDskQueryParam(dskinfo[i].hd, &dskinfo[i].dp);
	    if( rc != 0 )
	    {
		Verbose(0, "VSETUP", "PDskQueryParam - rc %lu", rc);
		break;
	    }

	    Verbose(3, "VSETUP", "physical drive parameters:");
	    Verbose(3, "VSETUP", "Cylinders: %d, Heads: %d, Sectors/Track: %d",
		    dskinfo[i].dp.cCylinders, dskinfo[i].dp.cHeads,
		    dskinfo[i].dp.cSectorsPerTrack);
	    Verbose(3, "VSETUP", "drive capacity: %lu MBytes",
		    (ULONG)(((ULONG)dskinfo[i].dp.cSectorsPerTrack
			     * SECTOR_SIZE * ((ULONG)dskinfo[i].dp.cHeads)
			     * (ULONG)dskinfo[i].dp.cCylinders) / 1024) / 1024);

	    /* And please, don't disturb us. */

	    rc = PDskLock(dskinfo[i].hd);
	    if( rc != 0 )
	    {
		Verbose(0, "VSETUP", "PDskLock - rc %lu", rc);
		break;
	    }
	}
	if( rc != 0 )
	    break;


	/* Read, modify, write partition table (master boot record) of
	 * all involved devices.  They need a VRAID partition. */

	for( i = 0; i < cnt; ++i )
	{
	    Verbose(2, "VSETUP", "-------- Partition Sector --------");

	    rc = PDskRead(dskinfo[i].hd, 0, 1, buffer);
	    if( rc != 0 )
	    {
		Verbose(0, "VSETUP", "PDskRead - rc %lu", rc);
		break;
	    }
	    rc = BuildPartitionTable(&dskinfo[i].dp, (PMBR)buffer,
				     &dskinfo[i].partstart, &dskinfo[i].partsize);
	    if( rc != 0 )
		break;

	    rc = PDskWrite(dskinfo[i].hd, 0, 1, buffer);
	    if( rc != 0 )
	    {
		Verbose(0, "VSETUP", "PDskWrite - rc %lu", rc);
		break;
	    }
#if 0
	    printf("-------- Dump of real partition sector --------\n");
	    DumpBuffer(buffer, 512);
#endif
	}
	if( rc != 0 )
	    break;


	/* Read, modify, write first sector in VRAID partition
	 * (PHYSDEV record) on every involved device. */

	for( i = 0; i < cnt; ++i )
	{
	    Verbose(2, "VSETUP", "-------- PHYSDEV Sector --------");

	    rc = PDskRead(dskinfo[i].hd, dskinfo[i].partstart, 1, buffer);
	    if( rc != 0 )
	    {
		Verbose(0, "VSETUP", "PDskRead - rc %lu", rc);
		break;
	    }

	    rc = BuildPhysdevSector((PVOID)buffer, dskinfo[i].partsize, 0);
	    if( rc != 0 )
	    {
		Verbose(0, "VSETUP", "BuildPhysdevSector - rc %lu", rc);
		break;
	    }
	    memcpy(dskinfo[i].physid, ((PSEC_PHYSDEV)buffer)->u.s.id,
		   sizeof(dskinfo[i].physid));

	    rc = PDskWrite(dskinfo[i].hd, dskinfo[i].partstart, 1, buffer);
	    if( rc != 0 )
	    {
		Verbose(0, "VSETUP", "PDskWrite - rc %lu", rc);
		break;
	    }
#if 0
	    printf("-------- Dump of PHYSSDEV sector (%lu) --------\n",
		   dskinfo[i].partstart);
	    DumpBuffer(buffer, 512);
#endif
	}
	if( rc != 0 )
	    break;

	/* Now build a VRDEV sector on next (second) sector of VRAID
	 * partition on every involved device. */

	{
	    PCHILDTAB childs  = malloc(cnt * sizeof(CHILDTAB));

	    if( childs == NULL )
	    {
		Verbose(0, "VSETUP", "malloc failed");
		rc = ERROR_NOT_ENOUGH_MEMORY;
		break;
	    }
	    for( i = 0; i < cnt; ++i )
	    {
		memcpy(childs[i].id, dskinfo[i].physid, sizeof(DEVID));
		childs[i].size = dskinfo[i].partsize - VRAID_ADMINSECTORS;
	    }

	    for( i = 0; i < cnt; ++i )
	    {
		Verbose(2, "VSETUP", "-------- VRAIDDEVICE Sector --------");

		rc = PDskRead(dskinfo[i].hd, dskinfo[i].partstart+1, 1, buffer);
		if( rc != 0 )
		{
		    Verbose(0, "VSETUP", "PDskRead - rc %lu", rc);
		    break;
		}

		rc = BuildVraidSector((PVOID)buffer, RDTYPE_STRIPE,0x80, cnt,childs);
		if( rc != 0 )
		{
		    Verbose(0, "VSETUP", "BuildVraidSector - rc %lu", rc);
		    break;
		}

		rc = PDskWrite(dskinfo[i].hd, dskinfo[i].partstart+1, 1, buffer);
		if( rc != 0 )
		{
		    Verbose(0, "VSETUP", "PDskWrite - rc %lu", rc);
		    break;
		}
#if 0
		printf("-------- Dump of VRAID sector (%lu) --------\n",
		       dskinfo[i].partstart+1);
		DumpBuffer(buffer, SECTOR_SIZE);
#endif

#if 1
		if( i == 0 )
		{
		    /* Clear first data sector (will become partition sector) */

		    memset(buffer, 0, SECTOR_SIZE);
		    rc = PDskWrite(dskinfo[i].hd,
				   dskinfo[i].partstart+VRAID_ADMINSECTORS,
				   1, buffer);
		    if( rc != 0 )
		    {
			Verbose(0, "VSETUP", "PDskWrite - rc %lu", rc);
			break;
		    }
		}
#endif
	    } /* end[for()] */

	    free(childs);
	}
    }
    while( 0 );


    for( i = 0; i < cnt; ++i )
	if( dskinfo[i].hd != 0 )
	{
	    PDskUnlock(dskinfo[i].hd);
	    PDskClose(dskinfo[i].hd);
	}
    free(dskinfo);
    return rc;
}




int
main(int argc,char *argv[])
{
    enum { DUMP,
	   SHOW,
	   SINGLE,
	   CHAIN,
	   STRIPE }	mode = DUMP;
    int			rc;

    strcpy(szPrgName, argv[0]);
    SetLoglevel(2);
    while( argc > 1  &&  argv[1][0] == '-' )
    {
	switch( argv[1][1] )
	{
	  case 'D':
	    mode = DUMP;
	    break;

	  case 'S':
	    mode = SINGLE;
	    break;

	  case 'C':
	    mode = CHAIN;
	    break;

	  case 'P':
	    mode = STRIPE;
	    break;

	  case '?':
	    Help();
	    return 0;

	  case 'v':
	    if( argv[1][2] == ':' )
	    {
		unsigned new_value = argv[1][3] - '0';

		Verbose(2, "VSETUP", "new log level %d",new_value);
		SetLoglevel(new_value);
	    }
	    else
		SetLoglevel(2);
	    break;

	  case 'q':
	    SetLoglevel(VL_FATAL);
	    break;

	  default:
	    Verbose(0, "VSETUP", "unknown arg \"%s\"", argv[1]);
	    Usage(stderr);
	    return ERR_PARAMETER;
	}
	--argc;
	++argv;
    }

    if( (rc=QueryHostId(&usHostId)) != 0 )
    {
	Verbose(0, "VSETUP", "couldn't determine host id");
	return rc;
    }
    Verbose(2,"VSETUP", "host id: %#x", usHostId);
    CrcInit(0xA001);				/* CRC16 table */

    switch( mode )
    {
	ULONG	no;

      case DUMP:
	if( argc == 1 )				/* no more parameter */
	    no = -1UL;
	else
	    sscanf(argv[1], "%lu", &no);
	rc = DumpPartitions(no);
	break;

      case SINGLE:
	if( argc != 2 )				/* no more parameter */
	{
	    Verbose(0, "VSETUP", "missing drive index");
	    rc = ERR_PARAMETER;
	}
	else
	    sscanf(argv[1], "%lu", &no);
	rc = MakeSingle(no);
	break;

      case CHAIN:
	if( argc < 2 )				/* no more parameter */
	{
	    Verbose(0, "VSETUP", "missing drive indeces");
	    rc = ERR_PARAMETER;
	}
	else
	{
	    int		i;
	    PULONG	tab = malloc((argc-1) * sizeof(ULONG));

	    if( tab == NULL )
	    {
		Verbose(0, "VSETUP", "malloc failed");
		rc = ERROR_NOT_ENOUGH_MEMORY;
		break;
	    }

	    for( i = 0; i < argc-1; ++i )
		sscanf(argv[i+1], "%lu", &tab[i]);

	    rc = MakeChain(argc-1, tab);
	}
	break;

      case STRIPE:
	if( argc < 2 )				/* no more parameter */
	{
	    Verbose(0, "VSETUP", "missing drive indeces");
	    rc = ERR_PARAMETER;
	}
	else
	{
	    int		i;
	    PULONG	tab = malloc((argc-1) * sizeof(ULONG));

	    if( tab == NULL )
	    {
		Verbose(0, "VSETUP", "malloc failed");
		rc = ERROR_NOT_ENOUGH_MEMORY;
		break;
	    }

	    for( i = 0; i < argc-1; ++i )
		sscanf(argv[i+1], "%lu", &tab[i]);

	    rc = MakeStripe(argc-1, tab);
	}
	break;
    }

    return rc;
}
