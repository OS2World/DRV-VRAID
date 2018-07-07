/*
 * $Source: r:/source/driver/sleep/RCS/dsl.c,v $
 * $Revision: 1.8 $
 * $Date: 1999/11/29 01:08:53 $
 * $Locker:  $
 *
 *	Displays and/or modifies device sleeping timers.  Sample
 *	application.
 *	Compile with...
 *	IBM CSet++ 2.1		- icc -Q -W2all -O [-Gd] dsl.c
 *	IBM VAC++ 3.00		- icc -Q -W2all -O [-Gd] dsl.c
 *	GNU C 2.7.*		- gcc -Wall -O3 [-Zcrtdll] dsl.c
 *	WatCom C/C++ 10.5	- wcl386 -zq -bt=os2v2 -3s -wx -oax dsl.c
 *
 * $Log: dsl.c,v $
 * Revision 1.8  1999/11/29 01:08:53  vitus
 * - dumps IORB (which restarted a disk) to display if that IORB is available
 *
 * Revision 1.7  1997/09/03 01:29:11  vitus
 * added -l parameter
 * added DisplayLaststatus (if -l)
 *
 * Revision 1.6  1997/06/03 23:28:24  vitus
 * - expanded '-?' output
 *
 * Revision 1.5  1997/05/07 23:53:27  vitus
 * Tests both forms of version query
 * Calls READ_MSGS to get size of required buffer
 *
 * Revision 1.4  1997/04/07 03:35:03  vitus
 * Changed program options
 * Added possibility to change/display timeout counter
 *
 * Revision 1.3  1997/04/05 01:50:24  vitus
 * DisplayVersion shows registered-flag
 * Test: display datasize before/after call
 *
 * Revision 1.2  1997/03/05 21:58:26  vitus
 * Changed for new timeout in seconds
 * Added mode to display saved messages
 *
 * Revision 1.1  1997/02/06 01:25:42  vitus
 * Initial revision
 */
static char const id[]="$Id: dsl.c,v 1.8 1999/11/29 01:08:53 vitus Exp $";

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INCL_DOS
#include <os2.h>

#include "dskslpub.h"




#define DEVICENAME	"dsleeps$"		/* DSKSLEEP.FLT is actual... */

#define MODE_DISPLAY	0
#define MODE_CHANGE	1
#define MODE_MESSAGE	2
#define MODE_QUERY	3
#define MODE_IMMEDIATE	4
#define MODE_LASTSTATUS	5


/*
 * Global variables
 */
char	szPrgName[_MAX_PATH];
char	fVerbose = 0;
char	fMode = MODE_DISPLAY;


void
usage(void)
{
    printf("usage: %s [-?] [-m | -l | -q | {-c|-i} a1,d1,t1 [a2,d2,t2 ...]]\n", szPrgName);
}
void
help(void)
{
    usage();
    printf("\nDisplays and/or changes sleeping times/timers\n"
	   " -m\tdisplay saved messages\n"
	   " -l\tdisplay last status\n" 
	   " -c\tpermanent change of disk timeout (minutes)\n"
	   " -q\tdisplay *seconds* left until disk stops\n"
	   " -i\tchange *seconds* left until disk stops\n"
	   " -?\tthis text\n\n"
	   " a,d,t\ta value ('t' minutes or seconds) for disk 'd' on adapter 'a'\n"
	   "Examples:\n"
	   " 'dsl'\t\t\tdisplay current timeout values\n"
	   " 'dsl -c 1,0,10'\tchange timeout for first disk on second adapter\n\t\t\tto 10 minutes\n"
	   " 'dsl -c 0,0,0'\t\tchange timeout for first disk on first adapter\n\t\t\tto 0 minutes (never stop that disk)\n"
	   " 'dsl -i 1,1,0'\t\tset timer for second disk on second adapter\n\t\t\tto 0 seconds (immediately stop that disk)\n"
	   "\nRemember that any access to a disk will reset it's timer value.  So '-i'\nmight not work for you.\n");
}




void
DumpBuffer(PUCHAR buf,size_t siz)
{
    for(; siz; --siz, ++buf )
	printf(" %02x", *buf);
}




/*#
 * NAME
 *	DisplayVersion
 * CALL
 *	DisplayVersion(hd)
 * PARAMETER
 *	hd		handle to open device
 * RETURNS
 *	OS error code
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Displays driver version.
 * REMARKS
 */
int
DisplayVersion(HFILE hd)
{
    APIRET		rc;
    DSKSL_VER_DATA	data;
    ULONG		datasize;


    /* Issue IOCtl in two fashions
     * - 16bit data: only version
     * - DSKSL_VER_DATA: registered info, too */

    do
    {
	datasize = sizeof(data.version);
	rc = DosDevIOCtl(hd, IOCTL_DSKSLEEP_CATEGORY, DSKSL_QUERY_VERSION,
			 NULL, 0, NULL, &data.version, datasize, &datasize);
	if( rc )
	{
	    fprintf(stderr, "DSKSL_QUERY_VERSION - error %lu\n", rc);
	    break;
	}
	printf("Installed: DSKSLEEP.FLT %u.%02u\n",
	       HIBYTE(data.version), LOBYTE(data.version));

	datasize = sizeof(data);
	rc = DosDevIOCtl(hd, IOCTL_DSKSLEEP_CATEGORY, DSKSL_QUERY_VERSION,
			 NULL, 0, NULL, &data, datasize, &datasize);
	if( rc )
	{
	    fprintf(stderr, "DSKSL_QUERY_VERSION - error %lu\n", rc);
	    break;
	}

	printf("Installed: DSKSLEEP.FLT %u.%02u (%s)\n",
	       HIBYTE(data.version), LOBYTE(data.version),
	       (data.flags&0x01 ? "registered" : "unregistered"));
    }
    while( 0 );

    return (int)rc;
}




/*#
 * NAME
 *	DisplayMessages
 * CALL
 *	DisplayMessages(hd)
 * PARAMETER
 *	hd		handle to open device
 * RETURNS
 *	OS error code
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Displays messages saved during driver startup.
 * REMARKS
 */
int
DisplayMessages(HFILE hd)
{
    APIRET	rc;
    USHORT	cb = 0;				/* keep compiler happy */
    unsigned	i;
    ULONG	datasize;
    DSKSL_MSGS_DATA * data = NULL;

    do
    {
	/* First: read size of stored messages */

	datasize = sizeof(cb);
	rc = DosDevIOCtl(hd, IOCTL_DSKSLEEP_CATEGORY, DSKSL_READ_MSGS,
			 NULL, 0, NULL, &cb, datasize, &datasize);
	if( rc )
	{
	    fprintf(stderr, "DSKSL_READ_MSGS - error %lu\n", rc);
	    break;
	}

	/* Second: allocate buffer big enough to hold
	   those messages (but no single byte more) */

	datasize = FIELDOFFSET(DSKSL_MSGS_DATA,msg) + (ULONG)cb;
	data = malloc(datasize);
	if( data == NULL )
	{
	    fprintf(stderr, "out of memory\n");
	    rc = (APIRET)-3;
	    break;
	}

	/* Third: read and display messages */

	rc = DosDevIOCtl(hd, IOCTL_DSKSLEEP_CATEGORY, DSKSL_READ_MSGS,
			 NULL, 0, NULL, data, datasize, &datasize);
	if( rc )
	{
	    fprintf(stderr, "DSKSL_READ_MSGS - error %lu\n", rc);
	    break;
	}

	printf("------------------------------"
	       "------------------------------\n");
	for( i = 0;
	    i < data->cb - FIELDOFFSET(DSKSL_MSGS_DATA,msg);
	    i += strlen(&data->msg[i])+1 )
	{
	    printf("%s\n", &data->msg[i]);
	}
	printf("------------------------------"
	       "------------------------------\n");
    }
    while( 0 );

    if( data != NULL )
	free(data);
    return (int)rc;
}




/*#
 * NAME
 * CALL
 * PARAMETER
 * RETURNS
 * GLOBAL
 * DESPRIPTION
 * REMARKS
 */
int
DisplayLaststatus(HFILE hd,UCHAR a,UCHAR d)
{
    APIRET	rc;
    ULONG		parmsize = sizeof(DSKSL_DEVSTATE_PARM);
    DSKSL_DEVSTATE_PARM	parm;
    ULONG		datasize = sizeof(DSKSL_STATUS_DATA);
    DSKSL_STATUS_DATA	data;

    unsigned	i;
    static char	* iotab[] = {"device allocation",
			     "device deallocation",
			     "start",
			     "stop",
			     "verify",
			     "test unit ready"};

    parm.adapter = a;
    parm.unit = d;

    if( fVerbose )
    {
	printf("parmsize(in) %lu\n", parmsize);
	printf("datasize(in) %lu\n", datasize);
    }
    rc = DosDevIOCtl(hd, IOCTL_DSKSLEEP_CATEGORY, DSKSL_GET_LASTSTATUS,
		     &parm, parmsize, &parmsize, &data, datasize, &datasize);
    if( fVerbose )
    {
	printf("parmsize(in) %lu\n", parmsize);
	printf("datasize(out) %lu\n", datasize);
    }

    if( rc )
	fprintf(stderr, "DSKSL_GET_LASTSTATUS - error %lu\n", rc);
    else
    {
	printf("Last Status refers to %s\n",
	       (data.iotype > sizeof(iotab) / sizeof(iotab[0])
		? "unknown" : iotab[data.iotype]));
	printf("Status %#x, Error %#x\nStatusblock:", data.status, data.error);
	DumpBuffer(data.statusblock, sizeof(data.statusblock));
	printf("\nSensedata:");
	DumpBuffer(data.sensedata, sizeof(data.sensedata));
	printf("\n");
    }

    return (int)rc;
}




/*#
 * NAME
 *	DisplaySettings
 * CALL
 *	DisplaySettings(hd)
 * PARAMETER
 *	hd		handle to open device
 * RETURNS
 *	OS error code
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Displays all devices and timeout values.
 * REMARKS
 */
int
DisplaySettings(HFILE hd)
{
    APIRET	    rc;
    ULONG	    datasize = 4 + 40 * sizeof(DEVICE_TIMEOUT);
    PDSKSL_QL_DATA  data = malloc( datasize );
    unsigned	    i;

    if( data == NULL )
    {
	fprintf(stderr, "out of memory\n");
	return -3;
    }

    if( fVerbose )
	printf("datasize(in) %lu\n", datasize);
    rc = DosDevIOCtl(hd, IOCTL_DSKSLEEP_CATEGORY, DSKSL_QUERY_TIMEOUT,
		     NULL, 0, NULL, data, datasize, &datasize);
    if( fVerbose )
	printf("datasize(out) %lu\n", datasize);

    if( rc )
	fprintf(stderr, "DSKSL_QUERY_TIMEOUT - error %lu\n", rc);
    else
    {
	/* Calculate number of entries. */

	unsigned cnt = ((data->cb > datasize ?
			datasize : data->cb)
			- FIELDOFFSET(DSKSL_QL_DATA,list))
			 / sizeof(data->list[0]);

	/* Display nice table */

	printf(" Adapter Unit\tMinutes\n");
	for( i = 0; i < cnt; ++i )
	    printf(" %3u\t %3u\t %4lu\n",
		   data->list[i].adapter, data->list[i].unit,
		   data->list[i].seconds / 60UL );
    }

    free( data );
    return (int)rc;
}


		 

/*#
 * NAME
 *	ChangeSetting
 * CALL
 *	ChangeSetting(hd,a,d,m)
 * PARAMETER
 *	a		adapter index
 *	d		unit index
 *	m		minutes before sleep (0: never)
 * RETURNS
 *	OS error code
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Changes the timeout value for a single device.
 * REMARKS
 */
int
ChangeSetting(HFILE hd,UCHAR a,UCHAR d,ULONG m)
{
    APIRET		rc;
    ULONG		parmsize = sizeof(DSKSL_SETTO_PARM);
    DSKSL_SETTO_PARM	parm;

    if( fVerbose )
	printf("--- new: %u %u %lu\n", a, d, m);

    parm.cb = (USHORT)parmsize;
    parm.reserved[0] = parm.reserved[1] = 0;
    parm.list[0].adapter = a;
    parm.list[0].unit = d;
    parm.list[0].seconds = m * 60UL;
    parm.list[0].reserved[0] = parm.list[0].reserved[1] = 0;

    if( fVerbose )
	printf("parmsize(in) %lu\n", parmsize);
    rc = DosDevIOCtl(hd, IOCTL_DSKSLEEP_CATEGORY, DSKSL_SET_TIMEOUT,
		     &parm, parmsize, &parmsize, NULL, 0, NULL);
    if( fVerbose )
	printf("parmsize(out) %lu\n", parmsize);

    if( rc )
	fprintf(stderr, "DSKSL_SET_TIMEOUT - error %lu\n", rc);
    return (int)rc;
}




/*#
 * NAME
 *	DisplayCurrent
 * CALL
 *	DisplayCurrent(hd)
 * PARAMETER
 *	hd		handle to open device
 * RETURNS
 *	OS error code
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Displays all device timers.
 * REMARKS
 */
int
DisplayCurrent(HFILE hd)
{
    APIRET	rc;
    ULONG	parmsize;
    ULONG	datasize = 4 + 40 * sizeof(DEVICE_TIMEOUT);
    PDSKSL_QL_DATA  data = malloc( datasize );
    unsigned	devcnt;
    unsigned	i;

    if( data == NULL )
    {
	fprintf(stderr, "out of memory\n");
	return -3;
    }

    do
    {
	if( fVerbose )
	    printf("datasize(in) %lu\n", datasize);
	rc = DosDevIOCtl(hd, IOCTL_DSKSLEEP_CATEGORY, DSKSL_QUERY_TIMEOUT,
			 NULL, 0, NULL, data, datasize, &datasize);
	if( fVerbose )
	    printf("datasize(out) %lu\n", datasize);

	if( rc )
	{
	    fprintf(stderr, "DSKSL_QUERY_TIMEOUT - error %lu\n", rc);
	    break;
	}

	/* Calculate number of entries. */

	devcnt = ((data->cb > datasize ?
		   datasize : data->cb)
		  - FIELDOFFSET(DSKSL_QL_DATA,list))
	    / sizeof(data->list[0]);

	/* Display nice table */

	printf(" Adapter Unit\tSeconds\n");
	for( i = 0; i < devcnt; ++i )
	{
	    DSKSL_DEVSTATE_PARM		coord;
	    PDSKSL_DEVSTATE_DATA	current = malloc(128);

	    coord.adapter = data->list[i].adapter;
	    coord.unit = data->list[i].unit;

	    parmsize = sizeof(coord);
	    datasize = 128;
	    if( fVerbose )
		printf("datasize(in) %lu\n", datasize);
	    rc = DosDevIOCtl(hd, IOCTL_DSKSLEEP_CATEGORY, DSKSL_QUERY_DEVSTATE,
			     &coord, parmsize, &parmsize,
			     current, datasize, &datasize);
	    if( fVerbose )
		printf("datasize(out) %lu\n", datasize);

	    if( rc )
	    {
		fprintf(stderr, "DSKSL_QUERY_DEVSTATE - error %lu\n", rc);
		break;
	    }

	    printf(" %3u\t %3u\t %4lu\n",
		   coord.adapter, coord.unit, current->seconds);
	    if( *(PUSHORT)current->restart != 0 )
	    {
		printf("\t");
		DumpBuffer(current->restart, *(PUSHORT)current->restart);
		printf("\n");
	    }
	}
    }
    while( 0 );

    free(data);
    return (int)rc;
}




/*#
 * NAME
 *	ChangeCurrent
 * CALL
 *	ChangeCurrent(hd,a,d,s)
 * PARAMETER
 *	a		adapter index
 *	d		unit index
 *	s		seconds before sleep (0: immediate)
 * RETURNS
 *	OS error code
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Changes the current counter for a single device.
 * REMARKS
 */
int
ChangeCurrent(HFILE hd,UCHAR a,UCHAR d,ULONG s)
{
    APIRET		rc;
    ULONG		parmsize = sizeof(DSKSL_DEVSTATE_PARM);
    DSKSL_DEVSTATE_PARM	parm;
    ULONG		datasize = sizeof(DSKSL_DEVSTATE_DATA);
    DSKSL_DEVSTATE_DATA	data;

    if( fVerbose )
	printf("--- new: %u %u %lu\n", a, d, s);

    parm.adapter = a;
    parm.unit = d;
    data.seconds = s;

    if( fVerbose )
    {
	printf("parmsize(in) %lu\n", parmsize);
	printf("datasize(in) %lu\n", datasize);
    }
    rc = DosDevIOCtl(hd, IOCTL_DSKSLEEP_CATEGORY, DSKSL_SET_DEVSTATE,
		     &parm, parmsize, &parmsize,
		     &data, datasize, &datasize);
    if( fVerbose )
    {
	printf("parmsize(in) %lu\n", parmsize);
	printf("datasize(in) %lu\n", datasize);
    }

    if( rc )
	fprintf(stderr, "DSKSL_SET_DEVSTATE - error %lu\n", rc);
    return (int)rc;
}




/*#
 * NAME
 *	main
 * CALL
 *	main(argc,argv)
 * PARAMETER
 *	argc,argv	as usual
 * RETURNS
 *	0		OK
 *	/0		some error
 * GLOBAL
 *	szPrgName, fVerbose
 * DESPRIPTION
 *	Main routine, parses parameter and calls requested
 *	subfunctions.
 * REMARKS
 */
int
main(int argc,char *argv[])
{
    HFILE	hd = 0;
    ULONG	action_taken;
    APIRET	rc;
    int		result;

    strcpy( szPrgName, argv[0] );
    while( argc > 1  &&  argv[1][0] == '-' )
    {
	switch( argv[1][1] )
	{
	  case '?':
	    help();
	    return 0;

	  case 'v':
	    fVerbose = 1;
	    break;

	  case 'm':
	    fMode = MODE_MESSAGE;
	    break;

	  case 'l':
	    fMode = MODE_LASTSTATUS;
	    break;

	  case 'c':
	    fMode = MODE_CHANGE;
	    break;

	  case 'q':
	    fMode = MODE_QUERY;
	    break;

	  case 'i':
	    fMode = MODE_IMMEDIATE;
	    break;

	  default:
	    fprintf(stderr, "%s: unknown arg \"%s\"\n", szPrgName, argv[1]);
	    return -1;
	}
	--argc;
	++argv;
    }

    /* Open device driver, no special settings.  Use DENYNONE
     * so other processes (daemon?) may open concurrently. */

    rc = DosOpen( DEVICENAME, &hd, &action_taken,
		 0, 0,
		 OPEN_ACTION_FAIL_IF_NEW|OPEN_ACTION_OPEN_IF_EXISTS,
		 OPEN_SHARE_DENYNONE|OPEN_ACCESS_READWRITE,
		 NULL );
    if( rc )
    {
	fprintf(stderr, "DosOpen(%s) - error %lu\n", DEVICENAME, rc);
	return (int)rc;
    }

    DisplayVersion(hd);				/* always display version */
    switch( fMode )
    {
      case MODE_MESSAGE:
	if( argc != 1 )
	{
	    fprintf(stderr, "argument count error\n");
	    result = -1;
	}
	else
	    result = DisplayMessages(hd);
	break;

      case MODE_LASTSTATUS:
	if( argc <= 1 )				/* at least one double */
	{
	    fprintf(stderr, "missing device coordinates\n");
	    result = -1;
	}
	else
	{
	    for(; argc > 1; --argc,++argv )
	    {
		unsigned a = 0xFF, d = 0xFF;	/* dsksleep won't find them... */

		/* Parse argument (spaces important!) and
		 * call ChangeSetting(). */

		sscanf(argv[1], "%u ,%u", &a, &d);
		result = DisplayLaststatus(hd, a, d);
	    }
	}
	break;

      case MODE_DISPLAY:
	if( argc != 1 )
	{
	    fprintf(stderr, "argument count error\n");
	    result = -1;
	}
	else
	{
	    printf("Current settings:\n");
	    result = DisplaySettings(hd);
	}
	break;

      case MODE_CHANGE:
	printf("Current settings:\n");
	result = DisplaySettings(hd);
	if( result != 0 )
	    break;				/* don't change */
	if( argc <= 1 )				/* at least one triple */
	{
	    fprintf(stderr, "missing device timeout\n");
	    result = -1;
	}
	else
	{
	    for(; argc > 1; --argc,++argv )
	    {
		unsigned a = 0xFF, d = 0xFF;	/* dsksleep won't find them... */
		ULONG	 m = 0;

		/* Parse argument (spaces important!) and
		 * call ChangeSetting(). */

		sscanf(argv[1], "%u ,%u ,%lu", &a, &d, &m);
		result = ChangeSetting(hd, (UCHAR)a, (UCHAR)d, m);
	    }
	    printf("After changing:\n");
	    DisplaySettings(hd);		/* ignore errors */
	}
	break;

      case MODE_QUERY:
	if( argc != 1 )
	{
	    fprintf(stderr, "argument count error\n");
	    result = -1;
	}
	else
	    result = DisplayCurrent(hd);
	break;

      case MODE_IMMEDIATE:
	if( argc <= 1 )				/* at least one triple */
	{
	    fprintf(stderr, "missing device timeout\n");
	    result = -1;
	}
	else
	{
	    for(; argc > 1; --argc,++argv )
	    {
		unsigned a = 0xFF, d = 0xFF;	/* dsksleep won't find them... */
		ULONG	 m = 0xFFFFFF;

		/* Parse argument (spaces important!) and
		 * call ChangeCurrent(). */

		sscanf(argv[1], "%u ,%u ,%lu", &a, &d, &m);
		result = ChangeCurrent(hd, (UCHAR)a, (UCHAR)d, m);
	    }
	    printf("After changing:\n");
	    DisplayCurrent(hd);			/* ignore errors */
	}
	break;

      default:
	result = -1;
	break;
    }

    DosClose( hd );				/* be a nice guy */

    return result;
}
