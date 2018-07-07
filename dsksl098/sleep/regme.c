/*
 * $Source: E:/source/driver/sleep/RCS/regme.c,v $
 * $Revision: 1.3 $
 * $Date: 1997/06/03 23:56:51 $
 * $Author: vitus $
 *
 * Patch DSKSleep.flt with registration code.
 *
 * $Log: regme.c,v $
 * Revision 1.3  1997/06/03 23:56:51  vitus
 * - corrected syntax error
 *
 * Revision 1.2  1997/04/05 01:46:40  vitus
 * Added -f switch to select file to patch/display
 * May display patched user
 *
 * Revision 1.1  1997/03/26 02:37:30  vitus
 * Initial revision
 * ----------------------------------------
 * This code is Copyright Vitus Jensen 1997
 */
static char const id[]="$Id: regme.c,v 1.3 1997/06/03 23:56:51 vitus Exp $";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#include <fcntl.h>

#define INCL_DOS
#include <os2.h>


#define REG_MAGIC	0x53A1B7F9
#define REG_KEYLEN	40
#pragma pack(1)
typedef struct {
    ULONG	ulMagic;
    UCHAR	szUser[REG_KEYLEN];
    UCHAR	szRegCode[REG_KEYLEN];
    ULONG	ulSize;
} REGSTRUCT;
#pragma pack()


char const	szProduct[]="SCSI Disk Sleeper";
char		szDriver[_MAX_PATH] = "DskSleep.flt";
char		szUser[REG_KEYLEN];
char		szKey[REG_KEYLEN];



static void
Usage(void)
{
    printf("usage: regme [-?] [-f:<file>] {-d | \"user name\" code}\n");
}


void
Help(void)
{
    printf("\t\tDSKSleep registration tool\n\n");
    Usage();
    printf("Options:\n"
	   " -d\t\tdisplay user name in %s (no checks)\n"
	   " -f:<file>\tuse <file> instead of %s\n"
	   " -?\t\tthis text\n", szDriver, szDriver);
    printf("\nregme will patch a non-registered DSKSleep.flt with "
	   "the provided\nuser/key combination.  There is only basic "
	   "validation of this combination.  \nAll real checks are "
	   "made by DSKSleep.flt at startup, so be sure \nto provide "
	   "the correct information.\n");
    return;
}




static int
ReadCompleteFile(int const hd,ULONG const size,PUCHAR const buffer)
{
    if( _lseek(hd, 0, SEEK_SET) != 0 )
    {
	fprintf(stderr, "Seek error (%s)\n", strerror(errno));
	return 1;
    }
    if( _read(hd, buffer, size) != size )
    {
	fprintf(stderr, "Read error (%s)\n", strerror(errno));
	return 2;
    }
    return 0;
}




static int
SearchRegInfo(PUCHAR const buffer,ULONG const size,REGSTRUCT ** const reg)
{
    PUCHAR	p;

    for( p = buffer; ((REGSTRUCT *)p)->ulMagic != REG_MAGIC; ++p )
	;

    /* Verify structure. For some reason (exepack?) 'ulSize'
     * is reduced to 8bit, so check only lowest byte. */

    *reg = (REGSTRUCT *)p;
    if( (*reg)->ulMagic != REG_MAGIC )
    {
	fprintf(stderr, "Internal error 1\n");
	return -4;
    }
    if( (UCHAR)(*reg)->ulSize != sizeof(REGSTRUCT) )
    {
	fprintf(stderr, "Internal error 2\n");
	return -4;
    }
    return 0;
}




static int
DoRegistration(int const hd)
{
    ULONG const size = (ULONG)_filelength(hd);
    PUCHAR const buffer = malloc(size);
    REGSTRUCT *	reg = NULL;

    if( buffer == NULL )
    {
	fprintf(stderr, "Memory allocation failed\n");
	return -3;
    }

    do
    {
	if( ReadCompleteFile(hd, size, buffer) )
	    break;

	if( (errno=SearchRegInfo(buffer, size, &reg)) != 0 )
	    break;

	/* Don't register an already registered version.  That
	 * way no one can destroy his key. */

	if( reg->szUser[0] != 0 )
	{
	    fprintf(stderr, "Already registered version, aborting\n");
	    errno = -5;
	    break;
	}

	memcpy(reg->szUser, szUser, REG_KEYLEN);
	memcpy(reg->szRegCode, szKey, REG_KEYLEN);

	if( _lseek(hd, 0, SEEK_SET) != 0 )
	{
	    fprintf(stderr, "Seek error (%s)\n", strerror(errno));
	    break;
	}
	if( _write(hd, buffer, size) != size )
	{
	    fprintf(stderr, "Write error (%s)\n", strerror(errno));
	    break;
	}
	errno = 0;
    }
    while( 0 );

    free(buffer);
    return errno;
}




static int
DisplayUser(int const hd)
{
    ULONG const size = (ULONG)_filelength(hd);
    PUCHAR const buffer = malloc(size);
    REGSTRUCT *	reg = NULL;

    if( buffer == NULL )
    {
	fprintf(stderr, "Memory allocation failed\n");
	return -3;
    }

    do
    {
	if( ReadCompleteFile(hd, size, buffer) )
	    break;

	if( (errno=SearchRegInfo(buffer, size, &reg)) != 0 )
	    break;

	if( reg->szUser[0] == 0 )
	{
	    fprintf(stderr, "Non-registered version, aborting\n");
	    errno = -5;
	    break;
	}

	printf("Current user: \"%s\"\n", reg->szUser);
	errno = 0;
    }
    while( 0 );

    free(buffer);
    return errno;
}




int
main(int argc,char *argv[])
{
    int	display = 0;
    int	rc;

    while( argc > 1  &&  argv[1][0] == '-' )
    {
	switch( argv[1][1] )
	{
	  case '?':
	    Help();
	    return 0;

	  case 'd':
	    display = 1;
	    break;

	  case 'f':
	    if( argv[1][2] != ':'  ||  argv[1][3] == '\0' )
	    {
		fprintf(stderr, "%s: unknown arg \"%s\"\n", "regme", argv[1]);
		return -1;
	    }
	    sscanf(&argv[1][3], "%s", szDriver);
	    break;

	  default:
	    fprintf(stderr, "%s: unknown arg \"%s\"\n", "regme", argv[1]);
	    return -1;
	}
	--argc;
	++argv;
    }

    if( display )
    {
	int	hd;

	if( argc != 1 )
	{
	    Usage();
	    return -1;
	}

	if( (hd=_open(szDriver, O_RDONLY|O_BINARY)) == -1 )
	{
	    fprintf(stderr, "Can't open %s (%s)\n", szDriver, strerror(errno));
	    return errno;
	}

	rc = DisplayUser(hd);
	_close(hd);
    }
    else
    {
	int	i;
	int	hd;

	if( argc != 3 )
	{
	    Usage();
	    return -1;
	}

	if( argv[1][0] == '"' )
	    i = 1;
	else
	    i = 0;

	strncpy(szUser, &argv[1][i], REG_KEYLEN);
	if( szUser[strlen(szUser)-1] == '"' )
	    szUser[strlen(szUser)-1] = '\0';
	for( i = 0; i < strlen(argv[2]); i += 2 )
	{
	    if( !isxdigit(argv[2][i]) || !isxdigit(argv[2][i+1]) )
	    {
		fprintf(stderr, "Invalid key |%s|\n", argv[2]);
		return -2;
	    }
	    sscanf(&argv[2][i], "%02x", &szKey[i/2]);
	}

	printf("Registering %s\nUser: |%s|\nKey: |%s|\n", szProduct, szUser, argv[2]);

	if( (hd=_open(szDriver, O_RDWR|O_BINARY)) == -1 )
	{
	    fprintf(stderr, "Can't open %s (%s)\n", szDriver, strerror(errno));
	    return errno;
	}

	rc = DoRegistration(hd);
	_close(hd);

	if( rc == 0 )
	    printf("Done\n");
    }

    return rc;
}
