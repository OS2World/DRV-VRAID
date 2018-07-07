/**************************************************************************
 *
 * SOURCE FILE NAME = CMDPDEFS.H
 *
 * DESCRIPTIVE NAME = ADD command line parser -
 *		      Token IDs
 *
 *
 *
 * COPYRIGHT	Copyright (C) 1992 IBM Corporation
 *
 * The following IBM OS/2 2.1 source code is provided to you solely for
 * the purpose of assisting you in your development of OS/2 2.x device
 * drivers. You may use this code in accordance with the IBM License
 * Agreement provided in the IBM Device Driver Source Kit for OS/2. This
 * Copyright statement may not be removed.
 *
 *
 *
 * VERSION = V2.0
 *
 * DATE
 *
 * DESCRIPTION :
 *
 * Purpose: This file is automatically included in the files
 *	    which generate the parser tables:
 *
 *	      CMDPDSKT.C - Command Table for Diskette ADDs
 *	      CMDPDISK.C - Command Table for ST-506 DASD ADDs
 *	      CMDPSCSI.C - Command Table for SCSI ADDs
 *
 * FUNCTIONS  :
 *
 *
 *
 *
 *
 *
 *
 * NOTES
 *
 *
 * STRUCTURES
 *
 * EXTERNAL REFERENCES
 *
 *
 *
 * EXTERNAL FUNCTIONS
 *
 ***************************************************************************/
/*
 * $Header: e:/source/driver/perf/RCS/cmdpdefs.h,v 1.1 1997/06/04 23:18:03 vitus Exp $
 *
 * $Log: cmdpdefs.h,v $
 * Revision 1.1  1997/06/04 23:18:03  vitus
 * Initial revision
 *
 */

/*
** TOKEN IDs  - opt.id definitions
**
** note:  - Assign a unique token id (1 - 255) for each valid
**	    option.
*/

#define TOK_ID_ADAPTER	      1
#define TOK_ID_PORT	      2
#define TOK_ID_UNIT	      3
#define TOK_ID_DMA	      4
#define TOK_ID_IRQ	      5
#define TOK_ID_AHS	      6
#define TOK_ID_NOT_AHS	      7
#define TOK_ID_FORMAT	      8
#define TOK_ID_S_BYTES	      9
#define TOK_ID_MCA	     10
#define TOK_ID_PS2	     TOK_ID_MCA
#define TOK_ID_SLOT	     11
#define TOK_ID_DM	     12
#define TOK_ID_NOT_DM	     13
#define TOK_ID_SM	     14
#define TOK_ID_NOT_SM	     15
#define TOK_ID_HCW	     16
#define TOK_ID_NOT_HCW	     17
#define TOK_ID_HCR	     18
#define TOK_ID_NOT_HCR	     19
#define TOK_ID_DEV0	     20
#define TOK_ID_CAM	     21
#define TOK_ID_NOT_CAM	     22
#define TOK_ID_FDT	     23
#define TOK_ID_NOT_FDT	     24
#define TOK_ID_GEO	     25
#define TOK_ID_SMS	     26
#define TOK_ID_NOT_SMS	     27
#define TOK_ID_SN	     28
#define TOK_ID_NOT_SN	     29
#define TOK_ID_ET	     30
#define TOK_ID_NOT_ET	     31
#define TOK_ID_CHGLINE	     32

#define TOK_ID_VERBOSE		40
#define TOK_ID_BS		41
#define TOK_ID_MAXBL		42
#define TOK_ID_EISA		43
#define TOK_ID_AM		44
#define TOK_ID_NOT_AM		45

#define	TOK_ID_RESERVE		46
#define	TOK_ID_NOSCAN		47
#define	TOK_ID_ABOVE16		48
#define	TOK_ID_UT		49
#define	TOK_ID_NOT_UT		50

#define	TOK_ID_FIX		51		/* enable fix 'd' */
#define	TOK_ID_NOT_FIX		52		/* disable fix 'd' */

#define TOK_ID_TIME		53
#define TOK_ID_DEBUG		54
