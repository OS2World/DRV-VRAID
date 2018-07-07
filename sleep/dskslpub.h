/*
 * $Source: R:/source/driver/sleep/RCS/dskslpub.h,v $
 * $Revision: 1.7 $
 * $Date: 1999/11/29 00:47:49 $
 * $Locker:  $
 *
 * Describes IOCtl interface to dsksleep.flt
 *
 * $Log: dskslpub.h,v $
 * Revision 1.7  1999/11/29 00:47:49  vitus
 * - DSKSL_DEVSTATE_DATA contains restart reason
 *
 * Revision 1.6  1997/08/24 01:53:44  vitus
 * Added DSKSL_GET_LASTSTATUS
 *
 * Revision 1.5  1997/06/03 23:07:52  vitus
 * Removed syntax error
 *
 * Revision 1.4  1997/04/05 01:56:25  vitus
 * Expanded QUERY_VERSION to return additional bitfield
 *
 * Revision 1.3  1997/03/03 01:21:15  vitus
 * Added DSKSL_MSGS_* IOCtls and structure
 *
 * Revision 1.2  1997/02/26 01:49:54  vitus
 * Added SET/QUERY_DEVSTATE, Changed timeout units to seconds
 *
 * Revision 1.1  1997/02/06 01:07:05  vitus
 * Initial revision
 * -------------------------------------------
 * This code is Copyright Vitus Jensen 1996-99
 */

#if !defined(_DSKSLPUB_H)
#define _DSKSLPUB_H
#pragma pack(1)					/* all structures packed */


/*
 * IOCtl category and function codes
 */
#define IOCTL_DSKSLEEP_CATEGORY	0xC0		/* a user-defined category */
#define DSKSL_QUERY_VERSION	0x60		/* returns version and more */
#define DSKSL_QUERY_TIMEOUT	0x61		/* return device list */
#define DSKSL_SET_TIMEOUT	0x41		/* changes single entry */
#define DSKSL_QUERY_DEVSTATE	0x62		/* stopped? */
#define DSKSL_SET_DEVSTATE	0x42		/* stop now! */
#define DSKSL_READ_MSGS		0x63		/* read startup messages */
#define DSKSL_CLEAR_MSGS	0x43		/* forget all messages */
#define DSKSL_GET_LASTSTATUS	0x64		/* reads statusblock + sense */


/* Describes installed DSKSleep.flt */
typedef struct _DSKSL_VER_DATA {
    USHORT	version;			/* as in PC-DOS */
    USHORT	flags;				/* BIT0: registered */
} DSKSL_VER_DATA;



/* Defines device settings */
typedef struct _DEVICE_TIMEOUT {
    UCHAR	adapter;			/* adapter index */
    UCHAR	unit;				/* unit index */
    UCHAR	reserved[2];			/* (padding) */
    ULONG	seconds;			/* 0 -> no sleep */
} DEVICE_TIMEOUT;



/* Returned by DSKSL_QUERY_TIMEOUT */
typedef struct _DSKSL_QL_DATA {
    USHORT		cb;			/* byte count driver wants
						   to return (may be larger
						   than application buffer) */
    UCHAR		reserved[2];		/* (padding) */
    DEVICE_TIMEOUT	list[1];		/* some compiler don't
						   support '[]' ... */
} DSKSL_QL_DATA, * PDSKSL_QL_DATA;



/* Passed to DSKSL_SET_TIMEOUT */
typedef struct _DSKSL_SETTO_PARM {
    USHORT		cb;			/* size of complete structure */
    UCHAR		reserved[2];		/* (padding) */
    DEVICE_TIMEOUT	list[1];		/* see above */
} DSKSL_SETTO_PARM, * PDSKSL_SETTO_PARM;



/* Passed to DSKSL_QUERY_DEVSTATE, DSKSL_SET_DEVSTATE and DSKSL_GET_LASTSTATUS */
typedef struct _DSKSL_DEVSTATE_PARM {
    UCHAR	adapter;			/* adapter index */
    UCHAR	unit;				/* unit index */
} DSKSL_DEVSTATE_PARM, * PDSKSL_DEVSTATE_PARM;



/* Returned from DSKSL_QUERY_DEVSTATE
 * -- x seconds until device is stopped
 * -- restart code valid (0 if device is stopped)
 * Passed to DSKSL_SET_DEVSTATE
 * -- change current timeout to x seconds
 *    setting this value to 0 will immediately stop the
 *    device.
 *    OBS: any disk I/O will reset the device timeout to it's maximum. */
typedef struct _DSKSL_DEVSTATE_DATA {
    ULONG	seconds;			/* seconds still to go
						   until device is stopped */
    UCHAR	restart[1];			/* IORB which restarted this disk */
} DSKSL_DEVSTATE_DATA, * PDSKSL_DEVSTATE_DATA;



/* Returned by DSKSL_READ_MSGS
 * 'msg' will contain a buffer filled with ASCIIZ strings.  Strings
 * usually don't contain newlines.
 * Some time in the future this buffer might even contain messages
 * added after system startup.
 * To clear the buffer just issue DSKSL_CLEAR_MSGS w/o parameter or
 * data buffer. */
typedef struct _DSKSL_MSGS_DATA {
    USHORT	cb;				/* byte count driver wants
						   to return */
    char	msg[1];				/* some compiler don't
						   support '[]' */
} DSKSL_MSGS_DATA;



/* Reads statusblock and sense data from last I/O issued.  See DDK
 * information for format of status block and a SCSI II+ spec for
 * sense data.
 * Warning: this data may not be consistent, i.e. status and statusblock
 * may come from different I/Os if you happen to call this IOCtl during
 * device start or stop. */
typedef struct _DSKSL_STATUS_DATA {
    UCHAR	iotype;				/* DSKIO_* */
    UCHAR	reserved;			/* 0 */
    USHORT	status;				/* see DDK */
    USHORT	error;				/* see DDK */
    UCHAR	statusblock[23];		/* see DDK */
    UCHAR	sensedata[18];			/* see SCSI spec */
} DSKSL_STATUS_DATA, * PDSKSL_STATUS_DATA;

#define DSKIO_ALLOC	1
#define DSKIO_DEALLOC	2
#define DSKIO_CHANGEUI	3
#define DSKIO_START	4
#define DSKIO_STOP	5
#define DSKIO_VERIFY	6
#define DSKIO_TESTREADY	7

#pragma pack()
#endif /* _DSKSLPUB_H */
