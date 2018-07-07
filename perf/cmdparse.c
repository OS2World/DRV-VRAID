/**************************************************************************
 *
 * SOURCE FILE NAME = CMDPARSE.C
 *
 * DESCRIPTIVE NAME = ADD Command Line Parser
 *		      ADD CONFIG.SYS Command Line Parser Helper Routine
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
 * Purpose: This module consists of the Command_Parser Function and
 *	    its associated local routines.  For detailed description
 *	    of the Command_Parser interface refer to the CMDPARSE.H
 *	    file.
 *
 * FUNCTIONS  :  Command_Parser
 *		 Insert_End_Token
 *		 Locate_First_Slash
 *		 FarStrLen
 *		 strncmp
 *		 Parse_Option_Value
 *		 Skip_over_Blanks
 *		 char_parser
 *		 d_parser
 *		 dd_parser
 *		 hh_parser
 *		 H_Char_To_Byte
 *		 hhhh_parser
 *		 format_parser
 *		 scsi_id_parser
 *		 dev_id_parser
 *		 geometry_parser
 *		 chgline_parser
 *		 Insert_Token
 *		 Locate_Next_Slash
 *		 Validate_State_Index
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



#if !defined(OS2_INCLUDED)
# define  INCL_NOBASEAPI
# define  INCL_NOPMAPI
# include "OS2.H"
#endif
#include "CMDPHDR.H"
#include "CMDPROTO.H"
#define  TOKVBUF_LEN   255
#define  UNDEFINED     -1


PSZ	  pcmdline1, pcmdline_slash, pcmdline_start;
INT	  tokv_index, state_index, length;
CHARBYTE  tokvbuf[TOKVBUF_LEN];
POPT	  pend_option, ptable_option;
PBYTE	  poutbuf1, poutbuf_end;
CC	  cc;



/*
**    Command_Parser -	external entry point into this module
*/


/***************************************************************************
 *
 * FUNCTION NAME = Command_Parser
 *
 *
 * DESCRIPTION	 =
 *
 *   Purpose:
 *
 *
 *   Function Calls:
 *
 *
 *
 *
 * INPUT	 = (pCmdLine,pOptTable,pOutBuf,OutBuf_Len)
 *
 *
 *
 *
 * OUTPUT	 = NONE
 *
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = NONE
 *
 ****************************************************************************/
CC FAR
Command_Parser(PSZ pCmdLine,
	       POPTIONTABLE pOptTable,
	       PBYTE pOutBuf,
	       USHORT OutBuf_Len)
{
    USHORT j,end_index;

    if (OutBuf_Len < (TOKL_ID_END+TOK_MIN_LENGTH))
    {
	cc.ret_code = BUF_TOO_SMALL_ERR;
	cc.err_index = 0;
	return cc;
    }
    poutbuf_end = pOutBuf+OutBuf_Len;
    poutbuf1 = pOutBuf;
    for( poutbuf1 = pOutBuf; poutbuf1 < poutbuf_end; ++poutbuf1 )
	*poutbuf1 = 0;
    poutbuf1 = pOutBuf;
    Insert_End_Token();

    /* Locate the last entry in the Option Table. This special entry
     * defines whether or not an option is required based on the index
     * in the state table. */

    for( end_index = 0;
	pOptTable->poption[end_index]->id != TOK_ID_END;
	++end_index )
	;
    pend_option = pOptTable->poption[end_index];

    /* Setup the initial index into the state table. */

    state_index = pOptTable->entry_state;
    if( !Validate_State_Index(pOptTable->max_states) )
	return cc;

    /* On return from Locate_First_Slash call pcmdline_slash
     * contains the ptr to the slash in the command line. */

    pcmdline_start = pCmdLine;
    pcmdline1 = pCmdLine;
    if( !Locate_First_Slash() )
	return cc;
    for( j = 0; j < end_index; ++j )
    {
	/* Locate valid options in Option Table, based
	 * on state table index. */

	if( pOptTable->poption[j]->state[state_index] != E )
	{
	    /* Found a valid option. Check to see if this is the option
	     * entered at this point in command line. */

	    ptable_option = pOptTable->poption[j];
	    length = FarStrLen(ptable_option->string);
	    if( strncmp(pcmdline_slash, ptable_option->string, length) == TRUE )
	    {
		/* Found the command line option.  Now, syntax check its
		 * associated value. */

		if ( !Parse_Option_Value() )
		    return cc;

		/* No syntax err detected.  Now, insert the option and its
		 * associated value into the output buffer in token format. */

		if( !Insert_Token() )
		    return cc;

		/* Setup next index into the state table. */

		state_index = ptable_option->state[state_index];
		if( !Validate_State_Index(pOptTable->max_states) )
		    return cc;

		/* Setup cmdline_slash to point the the next / (option) in
		 * the command line.
		 * Parsing stops once either an invalid character is
		 * found on the command line or the end of the command line
		 * is detected. */

		if( !Locate_Next_Slash() )
		    return cc;

		/* Setup for option search. Point to the top
		 * of the Option Table. */

		j = (USHORT)-1;
	    } /*endif*/
	} /*endif*/
    } /*endfor*/

    if( pend_option->state[state_index] == R )
    {
	/* A required option was not found on the command line. */

	cc.ret_code = REQ_OPT_ERR;
    }
    else
    {
	/* Characters on the command line are not defined in the Option Table
	 * as a valid option.  All options must start with a / character. */

	cc.ret_code = INVALID_OPT_ERR;
    }
    cc.err_index = pcmdline_slash-pCmdLine;
    return cc;
}




/***************************************************************************
 *
 * FUNCTION NAME = Insert_End_Token
 *
 *
 * DESCRIPTION	 =
 *
 *   Purpose: Insert the end of token marker into the output buffer
 *
 *
 *   Function Calls:
 *
 *
 *
 *
 * INPUT	 = ()
 *
 *
 *
 *
 * OUTPUT	 = NONE
 *
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = NONE
 *
 ****************************************************************************/
STATIC void NEAR
Insert_End_Token(void)
{
    *poutbuf1 = TOKL_ID_END;
    *(poutbuf1+1) = (BYTE)TOK_ID_END;
    return;
}




/***************************************************************************
 *
 * FUNCTION NAME = Locate_First_Slash
 *
 *
 * DESCRIPTION	 =
 *
 *   Purpose:  Locate the / on the command line.  All characters entered prior
 *	       to the first / are ignored.  This allows the parser to bypass
 *	       the BASEDEV = xxxxxxxx.xxx portion of the command line.
 *
 *   Function Calls:
 *
 *
 *
 *
 * INPUT	 = ()
 *
 *
 *
 *
 * OUTPUT	 = NONE
 *
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = NONE
 *
 ****************************************************************************/
STATIC BOOL NEAR Locate_First_Slash(VOID)
{
    while( *pcmdline1 != '\0'  &&  *pcmdline1 != '\n'  &&  *pcmdline1 != '\r' )
    {
	if( *pcmdline1 == '/' )
	{
	    pcmdline_slash = pcmdline1;
	    return TRUE;
	}
	++pcmdline1;
    }
    cc.err_index = 0;
    cc.ret_code = NO_OPTIONS_FND_ERR;
    if( pend_option->state[state_index] == R )
	cc.ret_code = REQ_OPT_ERR;
    return FALSE;
}




/***************************************************************************
 *
 * FUNCTION NAME = FarStrLen
 *
 *
 * DESCRIPTION	 =
 *
 *   Purpose: Return the length of a string
 *
 *
 *   Function Calls:
 *
 *
 *
 *
 * INPUT	 = (s)
 *
 *
 *
 *
 * OUTPUT	 = NONE
 *
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = FALSE
 *
 ****************************************************************************/
STATIC INT NEAR FarStrLen(CHAR FAR *s)
{
    INT i;
    for ( i=0; *s != '\0'; ++s )
	++i;
    return i;
}


/***************************************************************************
 *
 * FUNCTION NAME = strncmp
 *
 *
 * DESCRIPTION	 =
 *
 *   Purpose: Compare n number of characters in 2 strings, return TRUE if =
 *	      If s1 is in lower case, convert to upper prior to comparing.
 *
 *   Function Calls:
 *
 *
 *
 *
 * INPUT	 = (s1,s2,n)
 *
 *
 *
 *
 * OUTPUT	 = NONE
 *
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = FALSE
 *
 ****************************************************************************/
STATIC BOOL NEAR strncmp(CHAR FAR *s1,CHAR FAR *s2,INT n)
{
    INT i;
    CHAR temp;

    for( i = 0; i < n; ++i, ++s1, ++s2 )
	if( *s1 != *s2 )
	{
	    if( *s1 >= 'a'  &&  *s1 <= 'z' )
	    {
		temp = *s1 - (CHAR)' ';
		if( temp == *s2 )
		    continue;
	    }
	    return FALSE;
	}
    return TRUE;
}




/***************************************************************************
 * FUNCTION NAME = Parse_Option_Value
 *
 * DESCRIPTION	 =
 *
 *   Purpose: Parse the command line for the value assigned to located option
 *
 *   Function Calls:
 *
 *
 * INPUT	 = ()
 *
 * OUTPUT	 = NONE
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = FALSE
 ****************************************************************************/
STATIC BOOL NEAR
Parse_Option_Value()
{
    pcmdline1 = pcmdline_slash+length;
    Skip_Over_Blanks();
    for (tokv_index=0; tokv_index < TOKVBUF_LEN ; ++tokv_index)
	tokvbuf[tokv_index].byte_value = 0;

    tokv_index = UNDEFINED;
    cc.ret_code = NO_ERR;
    cc.err_index = 0;

    switch (ptable_option->type)
    {
      case TYPE_0:
	break;

      case TYPE_CHAR:
	char_parser();
	break;

      case TYPE_D:
	d_parser();
	break;

      case TYPE_DD:
	dd_parser();
	break;

      case TYPE_DDDD:
	dddd_parser();
	break;

      case TYPE_ULIST:
	ulist_parser();
	break;

      case TYPE_HH:
	hh_parser();
	break;

      case TYPE_HHHH:
	hhhh_parser();
	break;

      case TYPE_FORMAT:
	format_parser();
	break;

      case TYPE_SCSI_ID:
	scsi_id_parser();
	break;

      case TYPE_DEV_ID:
	dev_id_parser();
	break;

      case TYPE_GEOMETRY:
	geometry_parser();
	break;

      case TYPE_CHGLINE:
	chgline_parser();
	break;

      default:
	cc.ret_code = UNDEFINED_TYPE_ERR;
    } /* endswitch */

    if (cc.ret_code != NO_ERR)
    {
	cc.err_index = pcmdline1 - pcmdline_start;
	return FALSE;
    }
    return TRUE;
}




/***************************************************************************
 * FUNCTION NAME = Skip_Over_Blanks
 *
 * DESCRIPTION	 =
 *
 *   Purpose:  Skip over all the blank and tab characters
 *
 *
 *   Function Calls:
 *
 * INPUT	 = ()
 *
 * OUTPUT	 = NONE
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = FALSE
 ****************************************************************************/
STATIC void NEAR
Skip_Over_Blanks()
{
    while( *pcmdline1 == ' '  ||  *pcmdline1 == '\t' )
	++pcmdline1;
    return;
}




/***************************************************************************
 * FUNCTION NAME = char_parser
 *
 * DESCRIPTION	 =
 *
 *   Purpose: TYPE_CHAR option parser - scan till blank,tab,cr,new line or
 *					end of string char
 *
 *   Function Calls:
 *
 * INPUT	 = ()
 *
 * OUTPUT	 = NONE
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = FALSE
 ****************************************************************************/
STATIC void NEAR
char_parser()
{
    while( *pcmdline1 != '\0'  &&  *pcmdline1 != '\n'
	  &&  *pcmdline1 != '\r'  &&  *pcmdline1 != '/' )
    {
	tokvbuf[++tokv_index].char_value = *pcmdline1;
	++pcmdline1;
    }
    return;
}




/***************************************************************************
 * FUNCTION NAME = d_parser
 *
 * DESCRIPTION	 =
 *
 *   Purpose: TYPE_D option parser - one digit decimal number (d)
 *
 *   Function Calls:
 *
 * INPUT	 = ()
 *
 * OUTPUT	 = NONE
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = FALSE
 ****************************************************************************/
STATIC void NEAR
d_parser()
{
    if( (*pcmdline1 >= '0') && (*pcmdline1 <= '9') )
    {
       tokvbuf[++tokv_index].byte_value = *pcmdline1 - (CHAR)'0';
       pcmdline1++;
   }
    else
    {
       cc.ret_code = SYNTAX_ERR;
   }
    return;
}




/***************************************************************************
 * FUNCTION NAME = dd_parser
 *
 * DESCRIPTION	 =
 *
 *   Purpose: TYPE_DD option parser - two digit decimal number (dd)
 *
 *   Function Calls:
 *
 * INPUT	 = ()
 *
 * OUTPUT	 = NONE
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = FALSE
 ****************************************************************************/
STATIC void NEAR
dd_parser()
{
    INT i;
    BYTE n;
    BOOL flag;

    n = 0;
    flag = FALSE;
    for ( i=0; i < 2; i++ )
    {
	if( (*pcmdline1 >= '0') && (*pcmdline1 <= '9') )
	{
	    n = (BYTE)(10 * n + *pcmdline1 - '0');
	    ++pcmdline1;
	    flag = TRUE;
	}
	else
	{
	    /* Was at least 1 digit found on the command line? */

	    if( flag )
		break;
	    cc.ret_code = SYNTAX_ERR;
	    return;
	}
    }
    tokvbuf[++tokv_index].byte_value = n;
    return;
}




/*
 * NAME
 *	dddd_parser
 * CALL
 *	dddd_parser(void)
 * PARAMETER
 *	none
 * RETURNS
 *	nothing
 * GLOBAL
 *	cc
 * DESPRIPTION
 *	TYPE_DDDD option parser - four digit decimal number (dddd)
 * REMARKS
 */
STATIC void NEAR
dddd_parser()
{
    int		i;
    NUMBER	un;
    BYTE	c;
    BOOL	flag = FALSE;

    un.n = 0;
    flag = FALSE;
    for( i = 0; i < 4; ++i )
    {
	c = *pcmdline1;
	if( c >= '0'  &&  c <= '9' )
	{
	    un.n = 10 * un.n + c - '0';
	    ++pcmdline1;
	    flag = TRUE;
	}
	else
	{
	    /* Was at least 1 digit found on the command line? */

	    if( flag == TRUE )
		break;
	    cc.ret_code = SYNTAX_ERR;
	    return;
	}
    }

    tokvbuf[++tokv_index].byte_value = un.two_bytes.byte1;
    tokvbuf[++tokv_index].byte_value = un.two_bytes.byte2;
    return;
}




/*
 * NAME
 * CALL
 * PARAMETER
 * RETURNS
 * GLOBAL
 * DESPRIPTION
 *	TYPE_ULIST option parser - two or three decimal
 * 	number (d,d[,d]) list (!)
 *	If only two decimal are supplied the third is
 *	set to '0'.
 * REMARKS
 */
/*STATIC*/ void NEAR
ulist_parser()
{
    int		i;
    int		counter = 0;
    NUMBER	un_number;
    BOOL	flag, inside;

    while( TRUE )
    {
	if( counter == 0 )			/* first number in coordinate? */
	{
	    if( *pcmdline1 != '(' )
	    {
		cc.ret_code = SYNTAX_ERR;	/* no allowed */
		return;
	    }
	    ++pcmdline1;
	    Skip_Over_Blanks();
	    inside = TRUE;			/* (inside) */
	}

	un_number.n = 0;
	flag = FALSE;
	for( i = 0; i < 4; ++i )
	{
	    if( *pcmdline1 >= '0'  &&  *pcmdline1 <= '9' )
	    {
		un_number.n = 10 * un_number.n + (*pcmdline1 - '0');
		++pcmdline1;
		flag = TRUE;
	    }
	    else
	    {
		/* Was at least 1 digit found on the command line? */
		if( flag )
		    break;
		cc.ret_code = SYNTAX_ERR;
		return;
	    }
	}
	tokvbuf[++tokv_index].byte_value = un_number.two_bytes.byte1;
	tokvbuf[++tokv_index].byte_value = un_number.two_bytes.byte2;
	Skip_Over_Blanks();

	if( *pcmdline1 == ')'  &&  (counter == 1  ||  counter == 2) )
	{
	    if( counter == 1 )			/* only 2? Third = 0! */
	    {
		tokvbuf[++tokv_index].byte_value = 0;
		tokvbuf[++tokv_index].byte_value = 0;
	    }
	    counter = -1;			/* new coordinate */
	    ++pcmdline1;			/* skip ')' */
	    Skip_Over_Blanks();
	    inside = FALSE;			/* not (inside) */
	}
	else if( counter == 2 )
	{
	    cc.ret_code = SYNTAX_ERR;
	    return;
	}
	if( *pcmdline1 != ',' )
	{
	    if( inside )
		cc.ret_code = SYNTAX_ERR;	/* missing ')' */
	    return;
	}
	++pcmdline1;				/* skip ',' */
	Skip_Over_Blanks();
	++counter;
    }

    return;
}




/***************************************************************************
 * FUNCTION NAME = hh_parser
 *
 * DESCRIPTION	 =
 *
 *   Purpose: TYPE_HH option parser	   hh,hh format (h = hex char)
 *
 *   Function Calls:
 *
 * INPUT	 = ()
 *
 * OUTPUT	 = NONE
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = FALSE
 ****************************************************************************/
STATIC void NEAR
hh_parser()
{
    /* Convert command line HH char and setup token value buffer */

    if( !HH_Char_To_Byte() )
	return;
    Skip_Over_Blanks();

    if( *pcmdline1 != ',' )
    {
	cc.ret_code = SYNTAX_ERR;
	return;
    }
    ++pcmdline1;
    Skip_Over_Blanks();

    /* Convert command line HH char and setup token value buffer */

    HH_Char_To_Byte();
    return;
}




/***************************************************************************
 * FUNCTION NAME = HH_Char_To_Byte
 *
 * DESCRIPTION	 =
 *
 *   Purpose: Convert HH char to byte value
 *
 *   Function Calls:
 *
 * INPUT	 = ()
 *
 * OUTPUT	 = NONE
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = FALSE
 ****************************************************************************/
STATIC BOOL NEAR
HH_Char_To_Byte()
{
    BYTE  n;
    INT   i;
    BOOL  flag = FALSE;
    BYTE  c;

    n = 0;
    for( i = 0; i < 2; ++i )
    {
	c = *pcmdline1;
	if( c >= '0'  &&  c <= '9' )
	{
	    n = (BYTE)(16 * n + c - '0');
	    ++pcmdline1;
	    flag = TRUE;
	    continue;
	}
	c |= 0x20;				/* convert to lower case */
	if( c >= 'a'  &&  c <= 'f' )
	{
	    n = (BYTE)(16 * n + c - 'W');
	    ++pcmdline1;
	    flag = TRUE;
	    continue;
	}

	/* Was at least 1 hex digit found on the command line? */

	if( flag )
	    break;
	cc.ret_code = SYNTAX_ERR;
	return FALSE;
    }

    tokvbuf[++tokv_index].byte_value = n;
    return TRUE;
}




/***************************************************************************
 * FUNCTION NAME = hhhh_parser
 *
 * DESCRIPTION	 =
 *
 *   Purpose: TYPE_HHHH option parser	    hhhh format (h = hex char)
 *
 *   Function Calls:
 *
 * INPUT	 = ()
 *
 * OUTPUT	 = NONE
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = FALSE
 ****************************************************************************/
STATIC void NEAR
hhhh_parser()
{
    INT    i;
    BOOL   flag;
    NUMBER un_number;
    BYTE   c;

    un_number.n = 0;
    flag = FALSE;
    for( i = 0; i < 4; ++i )
    {
	c = *pcmdline1;
	if( c >= '0'  &&  c <= '9' )
	{
	    un_number.n = 16 * un_number.n + c - '0';
	    ++pcmdline1;
	    flag = TRUE;
	    continue;
	}
	c |= 0x20;
	if( c >= 'a'  &&  c <= 'f' )
	{
	    un_number.n = 16*un_number.n + c - 'W';
	    ++pcmdline1;
	    flag = TRUE;
	    continue;
	}

	/* Was at least 1 hex digit found on the command line? */

	if( flag )
	    break;
	cc.ret_code = SYNTAX_ERR;
	return;
    }

    tokvbuf[++tokv_index].byte_value = un_number.two_bytes.byte1;
    tokvbuf[++tokv_index].byte_value = un_number.two_bytes.byte2;
    return;
}




/***************************************************************************
 * FUNCTION NAME = format_parser
 *
 * DESCRIPTION	 =
 *
 *   Purpose: TYPE_FORMAT option parser -  format_table chars accepted
 *
 *   Function Calls:
 *
 * INPUT	 = ()
 *
 * OUTPUT	 = NONE
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = NONE
 ****************************************************************************/
STATIC void NEAR
format_parser()
{
    typedef struct _formattable {
	PSZ string;
	USHORT type;
	NUMBER MegBytes;
    } FORMATTABLE;

    /*
     * FORMATTABLE type definitions
     * accepts # string followed by:
     */

#define  M_BYTES       1	       /* accepts MB,mb,Mb,mB, or nothing */
#define  K_BYTES       2	       /* accepts KB,kb,Kb,kB, or nothing */

    INT str_len,k;
    static FORMATTABLE format_table[] = {
	{ "360",K_BYTES,360 } ,
	{ "720",K_BYTES,720 } ,
	{ "1200",K_BYTES,1200 } ,
	{ "1.2",M_BYTES,1200 } ,
	{ "1440",K_BYTES,1440 } ,
	{ "1.44",M_BYTES,1440 } ,
	{ "2880",K_BYTES,2880 } ,
	{ "2	.88",M_BYTES,2880 } ,
	{ "-1" } ,                          /* , ???? */
    };

    cc.ret_code = SYNTAX_ERR;
    for( k = 0; (format_table[k].string != "-1"); ++k )
    {
	str_len = FarStrLen(format_table[k].string);
	if( strncmp(pcmdline1, format_table[k].string, str_len) == TRUE )
	{
	    pcmdline1 += str_len;
	    if( format_table[k].type == K_BYTES )
	    {
		if( *pcmdline1 == 'K'  || *pcmdline1 == 'k' )
		{
		    ++pcmdline1;
		    if( *(pcmdline1) == 'B'  ||  *(pcmdline1) == 'b' )
			++pcmdline1;
		}
	    }
	    else
	    {
		if( *pcmdline1 == 'M'  ||  *pcmdline1 == 'm' )
		{
		    ++pcmdline1;
		    if( *(pcmdline1) == 'B'  ||  *(pcmdline1) == 'b' )
			++pcmdline1;
		}
	    }
	    ++tokv_index;
	    tokvbuf[tokv_index].byte_value
		= format_table[k].MegBytes.two_bytes.byte1;
	    ++tokv_index;
	    tokvbuf[tokv_index].byte_value
		= format_table[k].MegBytes.two_bytes.byte2;
	    cc.ret_code = NO_ERR;
	    break;
	}
    }

    return;
}




/***************************************************************************
 * FUNCTION NAME = scsi_id_parser
 *
 * DESCRIPTION	 =
 *
 *   Purpose: TYPE_SCSI_ID option parser - format d and (d,d) accepted
 *					   where d = 0 - 7
 *
 *   Function Calls:
 *
 * INPUT	 = ()
 *
 * OUTPUT	 = NONE
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = FALSE
 ****************************************************************************/
STATIC void NEAR
scsi_id_parser()
{
    BOOL found_bracket, found_one;

    found_bracket = FALSE;
    found_one = FALSE;
    if( *pcmdline1 == '(' )
    {
	found_bracket = TRUE;
	++pcmdline1;
	Skip_Over_Blanks();
    }
    while( *pcmdline1 >= '0'  &&  *pcmdline1 <= '7' )
    {
	found_one = TRUE;
	tokv_index++;
	tokvbuf[tokv_index].byte_value = (BYTE)(*pcmdline1 - '0');
	++pcmdline1;
	if( !found_bracket )
	{
	    ++tokv_index;
	    tokvbuf[tokv_index].byte_value = 0;
	}
	Skip_Over_Blanks();
	if( *pcmdline1 != ',' )
	{
	    if( found_bracket )
		cc.ret_code = SYNTAX_ERR;
	    return;
	}
	++pcmdline1;
	Skip_Over_Blanks();
	if( found_bracket )
	{
	    if( *pcmdline1 >= '0'  && *pcmdline1 <= '7' )
	    {
		++tokv_index;
		tokvbuf[tokv_index].byte_value = (BYTE)(*pcmdline1 - '0');
		++pcmdline1;
		Skip_Over_Blanks();
		if( *pcmdline1 != ')' )
		{
		    cc.ret_code = SYNTAX_ERR;
		    return;
		}
		++pcmdline1;
		Skip_Over_Blanks();
		found_bracket = FALSE;
		if( *pcmdline1 != ',' )
		    return;
		++pcmdline1;
		Skip_Over_Blanks();
	    }
	}
	if( *pcmdline1 == '(' )
	{
	    found_bracket = TRUE;
	    pcmdline1++;
	    Skip_Over_Blanks();
	}
    }

    if( found_bracket )
	cc.ret_code = SYNTAX_ERR;
    if( !found_one )
	cc.ret_code = SYNTAX_ERR;
    return;
}




/***************************************************************************
 * FUNCTION NAME = dev_id_parser
 *
 * DESCRIPTION	 =
 *
 *   Purpose: TYPE_DEV_ID option parser - format c, (c,d) and (c,d,d) accepted
 *					   where d = 0 - 7
 *					   and	 c = 0 - 9
 *
 *   Function Calls:
 *
 * INPUT	 = ()
 *
 * OUTPUT	 = NONE
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = FALSE
 *
 * 940215/VJ: Generated from scsi_id_parser() see CMDPDEFS.H for description
 *	      of token
 ****************************************************************************/
VOID NEAR dev_id_parser()
{
    BOOL found_bracket=FALSE;
    BOOL found_one=FALSE;

    /* A SCSI coordinate may be contained in brackets */
    if( *pcmdline1 == '(' )
    {
	found_bracket = TRUE;
	++pcmdline1;
	Skip_Over_Blanks();
    }

    /* Scan coordinates, first is cache device or scsi channel
     * and may evaluate to any value (currently supported 0-9). */

    while( *pcmdline1 >= '0'  &&  *pcmdline1 <= '9' )
    {
	found_one = TRUE;
	tokvbuf[++tokv_index].byte_value = *pcmdline1 - (CHAR)'0';
	++pcmdline1;
	Skip_Over_Blanks();

	if( *pcmdline1 != ',' )			/* more coordinates? */
	{					/*  NO */
	    if( found_bracket )			/* started w/ bracket? */
		cc.ret_code = SYNTAX_ERR;	/*  have to stop w/ bracket! */
	    tokvbuf[++tokv_index].byte_value = (CHAR)-1;
	    tokvbuf[++tokv_index].byte_value = (CHAR)-1; /* fill w/ -1 */
	    return;
	}
	++pcmdline1;
	Skip_Over_Blanks();

	if( found_bracket )			/* started w/ brackets? */
	{					/*  YES, more may come */
	    if ( *pcmdline1 >= '0'  &&  *pcmdline1 <= '7' )
	    {
		tokvbuf[++tokv_index].byte_value = *pcmdline1 - (CHAR)'0';
		++pcmdline1;
		Skip_Over_Blanks();

		if( *pcmdline1 == ',' )		/* ',' behind 2nd coord. ? */
		{				/*  YES, 3rd coord. follows */
		    ++pcmdline1;
		    Skip_Over_Blanks();
		    if( *pcmdline1 >= '0'  &&  *pcmdline1 <= '7' )
		    {
			tokvbuf[++tokv_index].byte_value = *pcmdline1 - (CHAR)'0';
			++pcmdline1;
			Skip_Over_Blanks();
		    }
		    else
		    {
			cc.ret_code = SYNTAX_ERR;
			return;
		    }
		}
		else
		{
		    tokvbuf[++tokv_index].byte_value = (CHAR)-1; /* 3rd coord. not supplied */
		}

		if( *pcmdline1 != ')' )
		{
		    cc.ret_code = SYNTAX_ERR;
		    return;
		}
		++pcmdline1;
		Skip_Over_Blanks();

		found_bracket = FALSE;
		if( *pcmdline1 != ',' )
		    return ;
		++pcmdline1;
		Skip_Over_Blanks();
	    }
	}
	else
	{
	    tokvbuf[++tokv_index].byte_value = (CHAR)-1;
	    tokvbuf[++tokv_index].byte_value = (CHAR)-1; /* fill w/ -1 */
	}

	if( *pcmdline1 == '(' )
	{
	    found_bracket = TRUE;
	    ++pcmdline1;
	    Skip_Over_Blanks();
	}
    }

    if( found_bracket )				/* missing closing bracket */
	cc.ret_code = SYNTAX_ERR;
    if( !found_one )				/* no numbers */
	cc.ret_code = SYNTAX_ERR;
    return;
}




/***************************************************************************
 * FUNCTION NAME = geometry_parser
 *
 * DESCRIPTION	 =
 *
 *   Purpose: TYPE_GEOMETRY option parser - dd or (dddd,dddd,dddd) accepted
 *
 *   Function Calls:
 *
 * INPUT	 = ()
 *
 * OUTPUT	 = NONE
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = FALSE
 ****************************************************************************/
STATIC void NEAR
geometry_parser()
{
    INT		i, counter;
    USHORT	base;
    NUMBER	un_number;
    BOOL	flag;

    if( *pcmdline1 != '(' )
    {
	dd_parser();
	return;
    }
    ++pcmdline1;
    Skip_Over_Blanks();
    counter = 0;
    while( TRUE )
    {
	un_number.n = 0;
	base = 10;
	flag = FALSE;

	for( i = 0; i < 4; ++i )		/* max. 4 digits */
	{
	  REDO_NO:
	    if( *pcmdline1 >= '0'  &&  *pcmdline1 <= '9' )
	    {
		un_number.n = base * un_number.n + *pcmdline1 - '0';
		++pcmdline1;
		flag = TRUE;
	    }
	    else if( base == 16  &&  *pcmdline1 >= 'A'  &&  *pcmdline1 <= 'F' )
	    {
		un_number.n = base * un_number.n + *pcmdline1 - 'A' + 10;
		++pcmdline1;
		flag = TRUE;
	    }
	    else if( base == 16  &&  *pcmdline1 >= 'a'  &&  *pcmdline1 <= 'f' )
	    {
		un_number.n = base * un_number.n + *pcmdline1 - 'a' + 10;
		++pcmdline1;
		flag = TRUE;
	    }
	    else if( *pcmdline1 == 'x' )
	    {
		++pcmdline1;
		un_number.n = 0;
		base = 16;
		flag = FALSE;
		i = 0;
		goto REDO_NO;			/* switch to hex */
	    }
	    else
	    {
		/* Was at least 1 digit found on the command line? */

		if( flag )
		    break;
		cc.ret_code = SYNTAX_ERR;
		return;
	    }
	} /* max. 4 digits */

	++tokv_index;
	tokvbuf[tokv_index].byte_value = un_number.two_bytes.byte1;
	++tokv_index;
	tokvbuf[tokv_index].byte_value = un_number.two_bytes.byte2;
	Skip_Over_Blanks();
	if( counter == 2 )			/* got a complete triple */
	{
	    if( *pcmdline1 != ')' )
	    {
		cc.ret_code = SYNTAX_ERR;
		return;
	    }
	    ++pcmdline1;			/* skip ')' */
	    if( *pcmdline1 != ',' )
		return;				/* end of option, OK */

	    ++pcmdline1;			/* skip ',' */
	    if( *pcmdline1 != '(')		/* start of next triple? */
	    {
		cc.ret_code = SYNTAX_ERR;
		return;
	    }
	    counter = 0;			/* reset counter */
	}
	else
	{
	    if( *pcmdline1 != ',' )
	    {
		cc.ret_code = SYNTAX_ERR;
		return;
	    }
	    ++counter;				/* another part of a triple */
	}
	++pcmdline1;				/* skip ',' or '(' */
	Skip_Over_Blanks();
    } /* end[while(TRUE)] */

    return;
}




/***************************************************************************
 * FUNCTION NAME = chgline_parser
 *
 * DESCRIPTION	 =
 *
 *   Purpose: TYPE_CHGLINE option parser - Valid options: PS2
 *							  AT
 *							  NONE
 *
 *   Function Calls:
 *
 * INPUT	 = ()
 *
 * OUTPUT	 = NONE
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = NONE
 ****************************************************************************/
STATIC void NEAR
chgline_parser()
{
    USHORT k, str_len;
    NPBYTE chgline_opts[] = { "\0","NONE","AT","PS2" };

    for( k = 1; k < sizeof(chgline_opts)/sizeof(chgline_opts[0]); ++k )
    {
	str_len = FarStrLen( chgline_opts[k] );
	if( strncmp(pcmdline1, chgline_opts[k], str_len) )
	{
	    pcmdline1 += str_len;
	    ++tokv_index;
	    tokvbuf[tokv_index].byte_value = (BYTE)k;
	    return;
	}
    }
    cc.ret_code = SYNTAX_ERR;
    return;
}




/***************************************************************************
 * FUNCTION NAME = Insert_Token
 *
 * DESCRIPTION	 =
 *
 *   Purpose: Insert the parsed option (token) into the output buffer.
 *
 *   Function Calls:
 *
 * INPUT	 = ()
 *
 * OUTPUT	 = NONE
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = FALSE
 ****************************************************************************/
STATIC BOOL NEAR
Insert_Token()
{
    USHORT	tok_size;
    INT		t;

    tok_size = TOK_MIN_LENGTH + tokv_index;
    if( (poutbuf1+tok_size+TOKL_ID_END) >= poutbuf_end )
    {
	cc.err_index = pcmdline_slash - pcmdline_start;
	cc.ret_code = BUF_TOO_SMALL_ERR;
	return FALSE;
    }
    *poutbuf1 = (BYTE)(tok_size + 1);
    ++poutbuf1;
    *poutbuf1 = (BYTE)ptable_option->id;
    ++poutbuf1;
    if( tokv_index != UNDEFINED )
    {
	for( t = 0; t <= tokv_index; ++t )
	{
	    *poutbuf1 = tokvbuf[t].byte_value;
	    ++poutbuf1;
	}
    }
    Insert_End_Token();
    return TRUE;
}




/***************************************************************************
 * FUNCTION NAME = Locate_Next_Slash
 *
 * DESCRIPTION	 =
 *
 *   Purpose: Locate the next / char.
 *
 *   Function Calls:
 *
 * INPUT	 = ()
 *
 * OUTPUT	 = NONE
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = NONE
 ****************************************************************************/
STATIC BOOL NEAR
Locate_Next_Slash()
{
    while( *pcmdline1 != '\0'  &&  *pcmdline1 != '\n'  &&  *pcmdline1 != '\r' )
    {
	if( *pcmdline1 == ' '  ||  *pcmdline1 == '\t' )
	{
	    ++pcmdline1;
	}
	else
	{
	    if( *pcmdline1 == '/' )
	    {
		pcmdline_slash = pcmdline1;
		return TRUE;
	    }
	    else
	    {
		cc.ret_code = INVALID_OPT_ERR;
		cc.err_index = pcmdline1 - pcmdline_start;
		return FALSE;
	    }
	}
    } /* endwhile */
    if( pend_option->state[state_index] == R )
    {
	cc.ret_code = REQ_OPT_ERR;
	cc.err_index = pcmdline1 - pcmdline_start;
    }
    else
    {
	cc.ret_code = NO_ERR;
	cc.err_index = 0;
    }

    return FALSE;
}




/***************************************************************************
 * FUNCTION NAME = Validate_State_Index
 *
 * DESCRIPTION	 =
 *
 *   Purpose: Validate the State Index
 *
 *   Function Calls:
 *
 * INPUT	 = (maxstate)
 *
 * OUTPUT	 = NONE
 *
 * RETURN-NORMAL = NONE
 *
 * RETURN-ERROR  = FALSE
 ****************************************************************************/
STATIC BOOL NEAR
Validate_State_Index(USHORT maxstate)
{
    if( state_index > maxstate  ||  state_index < 0 )
    {
	cc.ret_code = UNDEFINED_STATE_ERR;
	cc.err_index = 0;
	return FALSE;
    }
    return TRUE;
}


/* History:
 *
 * $Log: cmdparse.c,v $
 * Revision 1.4  1997/12/08 22:57:06  vitus
 * - did overwrite memory in Parse_Option_Value, fixed
 *
 * Revision 1.3  1997/12/06 01:00:30  vitus
 * - geometry_parser: expanded to accept several triples
 *
 * Revision 1.2  1997/10/28 01:53:33  vitus
 * - geometry_parser accepts hex values
 *
 * Imported from DSKSleep code (original IBM DDK for OS/2)
 */
