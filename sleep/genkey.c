/*
 * $Source: E:/source/driver/sleep/RCS/genkey.c,v $
 * $Revision: 1.1 $
 * $Date: 1997/03/27 00:10:16 $
 * $Author: vitus $
 *
 * Generates user key for DSKSleep.flt
 *
 * $Log: genkey.c,v $
 * Revision 1.1  1997/03/27 00:10:16  vitus
 * Initial revision
 *
 * ----------------------------------------
 * This code is Copyright Vitus Jensen 1997
 */
static char const id[]="$Id: genkey.c,v 1.1 1997/03/27 00:10:16 vitus Exp $";

#include <stdio.h>
#include <string.h>

#define INCL_BASE
#include <os2.h>


#define REG_MAGIC	0x53A1B7F9
#define REG_KEYLEN	40

UCHAR const	szProduct[]="SCSI Disk Sleeper";
UCHAR		szUser[REG_KEYLEN];
UCHAR		szKey[REG_KEYLEN];



void
GenKey(void)
{
    UCHAR	c;
    int		len;
    UCHAR const	*p;
    UCHAR const *k;

    memset( szKey, 0, REG_KEYLEN );
    for( c = 0, len = 0, p = szUser, k = szProduct;
	*p != '\0';
	++p, ++len, k = (*(k+1) == '\0' ? szProduct : k+1) )
    {
	c = (UCHAR)((*p & 0xf0) >> 4 | (*p & 0x0f));
	c *= *k;
	c += *k;
	szKey[len] = c;
    }

    {
	int	i;

	printf("Key: |");
	for( i = 0; i < len; ++i )
	{
	    printf("%02x", szKey[i]);
	}
	printf("|\n");
    }
    return;
}




void
Help(void)
{
    printf("usage: genkey <user name>\n");
    return;
}




int
main(int argc,char *argv[])
{
    int	i;

    if( argc != 2 )
    {
	Help();
	return -1;
    }

    if( argv[1][0] == '"' )
	i = 1;
    else
	i = 0;

    strncpy(szUser, &argv[1][i], REG_KEYLEN);
    if( szUser[strlen(szUser)-1] == '"' )
	szUser[strlen(szUser)-1] = '\0';

    printf("Generating key for %s\nUser: |%s|\n\n", szProduct, szUser);

    GenKey();

    return 0;
}
