/*
 * $Source: E:/source/driver/perf/RCS/scsioptpub.h,v $
 * $Revision: 1.3 $
 * $Date: 1997/12/11 01:50:53 $
 * $Locker:  $
 *
 * Describes IOCtl interface to dskperf.flt (none)
 *
 * $Log: scsioptpub.h,v $
 * Revision 1.3  1997/12/11 01:50:53  vitus
 * - added IOCtl to query saved messages (so this header is now usefull)
 *
 * Revision 1.2  1997/12/06 01:03:52  vitus
 * - clarified uselessness of this header
 *
 * Revision 1.1  1997/06/04 23:41:09  vitus
 * Initial revision
 * ----------------------------------------
 * Author: Vitus Jensen, 2:2474/424, 1997
 */

#if !defined(_DSKPFPUB_H)
#define _DSKPFPUB_H
#pragma pack(1)					/* all structures packed */


/*
 * IOCtl category and function codes
 */
#define IOCTL_DSKPERF_CATEGORY	0xC0		/* a user-defined category */
#define DSKPF_QUERY_VERSION	0x60		/* returns USHORT */
#define DSKPF_READ_MSGS		0x63		/* read startup messages */
#define DSKPF_CLEAR_MSGS	0x43		/* forget all messages */



/* Describes installed DSKPerf.flt */
typedef struct _DSKPF_VER_DATA {
    USHORT	version;			/* as in PC-DOS */
    USHORT	flags;				/* currently undefined */
} DSKPF_VER_DATA, NEAR * NPDSKPF_VER_DATA, FAR * PDSKPF_VER_DATA;



/* Returned by DSKPF_READ_MSGS
 * 'msg' will contain a buffer filled with ASCIIZ strings.  Strings
 * usually don't contain newlines.
 * Some time in the future this buffer might even contain messages
 * added after system startup.
 * To clear the buffer just issue DSKPF_CLEAR_MSGS w/o parameter or
 * data buffer. */
typedef struct _DSKPF_MSGS_DATA {
    USHORT	cb;				/* byte count driver wants
						   to return */
    char	msg[1];				/* some compiler don't
						   support '[]' */
} DSKPF_MSGS_DATA, NEAR * NPDSKPF_MSGS_DATA, FAR * PDSKPF_MSGS_DATA;


#pragma pack()
#endif /* _DSKPFPUB_H */
