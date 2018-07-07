/*
 * $Source: R:/source/driver/raid/setup/RCS\\pbench.c,v $
 * $Revision: 1.2 $
 * $Date: 1998/11/05 00:26:43 $
 * $Locker:  $
 *
 * Physical disk benchmark from OS2.PROG.GER
 *
 * $Log: pbench.c,v $
 * Revision 1.2  1998/11/05 00:26:43  vitus
 * - corrected include path of PDsk* routines
 *
 * Revision 1.1  1998/03/11 02:19:28  vitus
 * Initial revision
 *
 * ----------------------------------------
 * From OS2.PROG.GER, Stephan Zerth
 */
static char const id[]="$Id: pbench.c,v 1.2 1998/11/05 00:26:43 vitus Exp $";

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INCL_DOS
#define INCL_DOSDEVIOCTL
#include <os2.h>

#include "../../../tools/defines.h"
#include "../../../tools/physdsk.h"




int
getrandom(int start,int end)
{
    int r = rand();

    return r / (RAND_MAX / (end - start)) + start;
}


struct {
    USHORT			cPhysDrive;
    ULONG			ulCapacity;
    double			lTrackSeek;
    double			lTrackPhysIO;
    double			lRandomSeek;
    double			lRandomPhysIO;
    double			lExtremeSeek;
    double			lExtremePhysIO;
    DEVICEPARAMETERBLOCK	dp;
} Summary;




APIRET
PhysicalDiskTest(USHORT usDisk)
{
    DEVICEPARAMETERBLOCK dp;

    ULONG	i,j;
    ULONG	ulDisks = 0L;
    ULONG	ulDiskHandle = 0L;
    APIRET	rc;
    UCHAR	SectorBuffer[512];
    double	lTimeBegin;
    double	lTimeEnd;
    PUSHORT	SeekList;
    PUSHORT	SectorList;
    PUSHORT	HeadList;
    ULONG	idx;

    rc = PDskEnum(&ulDisks);
    printf("Number of physical disks: %d\n", ulDisks);

    if( ulDisks > 1 )
    {
	do
	{
	    printf("Which drive to test? [0-%lu]: ", ulDisks-1),	flushall();
	    scanf(" %lu", &idx);
	}
	while( idx >= ulDisks );
    }

    Summary.cPhysDrive = idx;


    rc = PDskOpen(idx, &ulDiskHandle);
    if( rc != 0 )
    {
	printf("\nPDskOpen(%lu,) - rc %lu\n", idx, rc);
	return rc;
    }

    rc = PDskQueryParam(ulDiskHandle, &dp);

    Summary.dp.cCylinders       = dp.cCylinders;
    Summary.dp.cHeads           = dp.cHeads;
    Summary.dp.cSectorsPerTrack = dp.cSectorsPerTrack;
    printf("Physical drive parameters: Cylinders: %d, Heads: %d, Sectors/Track: %d\n",
	   dp.cCylinders, dp.cHeads, dp.cSectorsPerTrack);

    Summary.ulCapacity = (ULONG)(((ULONG)dp.cSectorsPerTrack
				  * 512 * ((ULONG)dp.cHeads) * (ULONG)dp.cCylinders)
				 / 1024) / 1024;
    printf("Drive capacity: %d MBytes\n", Summary.ulCapacity);


    SectorList = malloc(dp.cCylinders * sizeof(USHORT));
    srand((unsigned)time(NULL));
    for( i = 0; i < dp.cCylinders; ++i )
	SectorList[i] = getrandom(1, dp.cSectorsPerTrack);

    HeadList = malloc(dp.cCylinders * sizeof(USHORT));
    srand((unsigned)time(NULL));
    for( i = 0; i < dp.cCylinders; ++i )
	HeadList[i] = getrandom(0, dp.cHeads - 1);

    printf("\nTesting track to track seek:\t"),	flushall();

    lTimeBegin = (double)clock();
    lTimeBegin = lTimeBegin / CLOCKS_PER_SEC;

    for( i = 0; i < dp.cCylinders; ++i )
    {
	rc = PDskReadTrack(ulDiskHandle, i, HeadList[i], SectorList[i], 1,
			   &SectorBuffer[0]);
	if( rc != 0 )
	{
	    printf("\nPDskReadTrack - rc %lu\n", rc);
	    break;
	}
    }

    lTimeEnd = (double)clock();
    lTimeEnd = lTimeEnd / CLOCKS_PER_SEC;

    Summary.lTrackSeek = ((lTimeEnd - lTimeBegin) / dp.cCylinders) * 1000;
    Summary.lTrackPhysIO = ((512*dp.cCylinders) / 1024) / (lTimeEnd - lTimeBegin);
    printf("%7.3f ms, physical I/O was: %7.3f KB/s\n",
	   Summary.lTrackSeek, Summary.lTrackPhysIO);


    printf("Testing random seeking:\t\t"),	flushall();

    SeekList = malloc(dp.cCylinders * sizeof(USHORT));
    srand((unsigned)time(NULL));
    for( i = 0; i < dp.cCylinders; i++ )
	SeekList[i] = getrandom(0, dp.cCylinders -1);

    lTimeBegin = (double)clock();
    lTimeBegin = lTimeBegin / CLOCKS_PER_SEC;

    j = dp.cCylinders;

    for( i = 0; i < dp.cCylinders; i++ )
    {
	rc = PDskReadTrack(ulDiskHandle, SeekList[i], HeadList[i], SectorList[i],
			   1, &SectorBuffer[0]);
	if( rc != 0 )
	{
	    printf("\nPDskReadTrack - rc %lu\n", rc);
	    break;
	}
    }

    lTimeEnd = (double)clock();
    lTimeEnd = lTimeEnd / CLOCKS_PER_SEC;

    Summary.lRandomSeek = ((lTimeEnd - lTimeBegin) / dp.cCylinders) * 1000;
    Summary.lRandomPhysIO = ((512*dp.cCylinders) / 1024) / (lTimeEnd - lTimeBegin);
    printf("%7.3f ms, physical I/O was: %7.3f KB/s\n",
	   Summary.lRandomSeek, Summary.lRandomPhysIO);

    free(SeekList);


    printf("Testing extreme seeking:\t"),	flushall();

    lTimeBegin = (double)clock();
    lTimeBegin = lTimeBegin / CLOCKS_PER_SEC;

    j = dp.cCylinders;

    for( i = 0; i < dp.cCylinders; i++ )
    {
	rc = PDskReadTrack(ulDiskHandle, i, HeadList[i], SectorList[i],
			   1, &SectorBuffer[0]);
	if( rc != 0 )
	{
	    printf("\nPDskReadTrack - rc %lu\n", rc);
	    break;
	}

	rc = PDskReadTrack(ulDiskHandle, --j, HeadList[i], SectorList[i],
			   1, &SectorBuffer[0]);
	if( rc != 0 )
	{
	    printf("\nPDskReadTrack - rc %lu\n", rc);
	    break;
	}
    }

    lTimeEnd = (double)clock();
    lTimeEnd = lTimeEnd / CLOCKS_PER_SEC;

    Summary.lExtremeSeek = (((lTimeEnd - lTimeBegin) / dp.cCylinders) * 1000) / 2;
    Summary.lExtremePhysIO = ((1024*dp.cCylinders) / 1024)
	/ (lTimeEnd - lTimeBegin);
    printf("%7.3f ms, physical I/O was: %7.3f KB/s\n",
	   Summary.lExtremeSeek, Summary.lExtremePhysIO);

    rc = PDskClose(ulDiskHandle);

    free(SectorList);
    free(HeadList);
    return 0;
}




int
main(void)
{
    APIRET	rc;

    rc = PhysicalDiskTest(0);
    return rc;
}
