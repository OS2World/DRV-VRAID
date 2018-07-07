/**************************************************************************
 *
 * SOURCE FILE NAME = CMDPHDR.H
 *
 * DESCRIPTIVE NAME = ADD command line parser
 *		      Command Parser Top Level Include file
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



#include "cmdparse.h"
#include "cmdpdefs.h"

/*
**  external references resolved in related C file.
*/

extern	 CC	       cc;
extern	 USHORT        outbuf_len;
extern	 OPTIONTABLE   opttable;
extern	 PBYTE	       poutbuf;

