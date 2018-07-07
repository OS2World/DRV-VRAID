/**************************************************************************
 *
 * SOURCE FILE NAME = CMDPROTO.H
 *
 * DESCRIPTIVE NAME = ADD command line parser
 *		      C prototypes for Command Parser internal functions
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
 * Purpose:
 *
 *
 *
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
**  Command_Parser local function - PROTOTYPES
*/

#ifndef STATIC
  #define STATIC static
#endif

STATIC VOID NEAR Insert_End_Token(void);
STATIC BOOL NEAR Locate_First_Slash(void);
STATIC INT  NEAR FarStrLen(CHAR FAR *);
STATIC BOOL NEAR strncmp(CHAR FAR *,CHAR FAR *,INT);
STATIC BOOL NEAR Parse_Option_Value(void);
STATIC VOID NEAR Skip_Over_Blanks(void);
STATIC VOID NEAR char_parser(void);
STATIC VOID NEAR d_parser(void);
STATIC VOID NEAR dd_parser(void);
       void NEAR dddd_parser(void);
       void NEAR ulist_parser(void);
STATIC VOID NEAR hh_parser(void);
STATIC VOID NEAR hhhh_parser(void);
STATIC VOID NEAR format_parser(void);
STATIC VOID NEAR scsi_id_parser(void);
STATIC void NEAR dev_id_parser(void);
STATIC VOID NEAR geometry_parser(void);
STATIC VOID NEAR chgline_parser(void);
STATIC BOOL NEAR Insert_Token(void);
STATIC BOOL NEAR Locate_Next_Slash(void);
STATIC BOOL NEAR Validate_State_Index(USHORT);
STATIC BOOL NEAR HH_Char_To_Byte(void);


