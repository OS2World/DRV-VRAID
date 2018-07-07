/*
 * $Source: d:/source/driver/perf/RCS/modepages.h,v $
 * $Revision: 1.5 $
 * $Date: 1998/03/04 01:59:16 $
 * $Locker:  $
 *
 * Defines structures to access SCSI mode pages.
 * Refer to SCSI-II
 *
 * $Log: modepages.h,v $
 * Revision 1.5  1998/03/04 01:59:16  vitus
 * - added SCSI_MODEPAGE_D
 *
 * Revision 1.4  1997/12/06 00:45:49  vitus
 * - WatCom detected duplicate typedef
 *
 * Revision 1.3  1997/11/10 01:04:05  vitus
 * - corrected SCSI_MODEPAGE_2
 *
 * Revision 1.2  1997/10/28 01:55:59  vitus
 * - switched from bitfields to complete bytes
 * - named structures after page number
 *
 * Revision 1.1  1997/06/04 23:19:11  vitus
 * Initial revision
 * ----------------------------------------
 * Author: Vitus Jensen, 2:2474/424
 */

#if !defined(_MODEPAGES_H)
#define _MODEPAGES_H
#pragma pack(1)


/* Missing in my DDK... */
#define SCSI_MODE_SELECT_10	0x55
#define SCSI_MODE_SENSE_10	0x5A
/* end */


typedef struct _SCSI_MODEPAGE_HDR {

    UCHAR	len;
    UCHAR	medium;
    UCHAR	specific;
    UCHAR	descrlen;

} SCSI_MODEPAGE_HDR, * PSCSI_MODEPAGE_HDR, NEAR * NPSCSI_MODEPAGE_HDR;


typedef struct _SCSI_MODEPAGE_DESCR {

    UCHAR	density;
    UCHAR	blocks[3];
    UCHAR	reserved;
    UCHAR	blklen[3];

} SCSI_MODEPAGE_DESCR, * PSCSI_MODEPAGE_DESCR, NEAR * NPSCSI_MODEPAGE_DESCR;



typedef struct _SCSI_MODEPAGE_2 {		/* disconnect-reconnect */

    UCHAR	pcode;				/* <0:5> page code
						   <7>   page is savable */
    UCHAR	plength;			/* excluding this field */
    UCHAR	bfull;				/* reads: x/256 before device
						   reconnects */
    UCHAR	bempty;				/* write: x/256 before device
						   reconnects */
    UCHAR	businactive[2];			/* max time in 100us */
    UCHAR	disconnect[2];			/* max time in 100us */
    UCHAR	connect[2];			/* max time in 100us */
    UCHAR	burst[2];			/* max sectors per data burst */
    UCHAR	dtdc;				/* disable/enable disconnect
						   in data transfers */
    UCHAR	reserved[3];

} SCSI_MODEPAGE_2, * PSCSI_MODEPAGE_2, NEAR * NPSCSI_MODEPAGE_2;


typedef struct _SCSI_MODEPAGE_8 {		/* caching */

    UCHAR	pcode;				/* s.a. */
    UCHAR	plength;			/* s.a. */
    UCHAR	cache;				/* <0>  read cache disable
						   <2>  write cache enable */
    UCHAR	priority;			/* pre-fetch priority */
    UCHAR	disprefetch[2];			/* disable pre-fetch on long
						   transfers */
    UCHAR	minprefetch[2];			/* minimum pre-fetch done */
    UCHAR	maxprefetch[2];			/* maximum pre-fetch done */
    UCHAR	prefetchceiling[2];		/* maximum pre-fetch done */

} SCSI_MODEPAGE_8, * PSCSI_MODEPAGE_8, NEAR * NPSCSI_MODEPAGE_8;


typedef struct _SCSI_MODEPAGE_A {		/* control */

    UCHAR	pcode;				/* s.a. */
    UCHAR	plength;			/* s.a. */
    UCHAR	reserved1;
    UCHAR	queuing;			/* <0>   disable tagged queuing
						   <4:7> queue algorithm */
    UCHAR	events;				/* enable asynchr. events */
    UCHAR	reserved2;
    UCHAR	aenholdoff[2];			/* related to asynchr. events */

} SCSI_MODEPAGE_A, * PSCSI_MODEPAGE_A, NEAR * NPSCSI_MODEPAGE_A;


typedef struct _SCSI_MODEPAGE_D {		/* CD-ROM */

    UCHAR	pcode;				/* s.a. */
    UCHAR	plength;			/* s.a. */
    UCHAR	reserved1;
    UCHAR	multiplier;			/* timeout code (see SCSI-II) */
    UCHAR	s_units[2];			/* ??? */
    UCHAR	f_units[2];			/* ??? */

} SCSI_MODEPAGE_D, * PSCSI_MODEPAGE_D, NEAR * NPSCSI_MODEPAGE_D;



#pragma pack()
#endif /* _MODEPAGES_H */
