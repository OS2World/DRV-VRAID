/*
 * $Source: e:/source/driver/perf/RCS/printk.c,v $
 * $Revision: 1.2 $
 * $Date: 1997/12/06 00:43:41 $
 * $Author: vitus $
 *
 * Simple sprintf-like function
 *
 * $Log: printk.c,v $
 * Revision 1.2  1997/12/06 00:43:41  vitus
 * - enabled support for WatCom C
 *
 * Revision 1.1  1997/06/04 23:43:45  vitus
 * Initial revision
 *
 */
#pragma off (unreferenced)
static char vcid[]="$Id: printk.c,v 1.2 1997/12/06 00:43:41 vitus Exp $";
#pragma on (unreferenced)


#define PRIVATE static
#define PUBLIC


#define FNULL	 ((void _far *)0)

typedef char _far *va_list;
#define va_start(ap,v) ap = (va_list)&v + sizeof(v)
#define va_arg(ap,t) ((t _far *)(ap += sizeof(t)))[-1]
#define va_end(ap) ap = FNULL


/* #define NO_LONGD	*/		  /* Longflags ignorieren */

/* three compile time options:
 *	NO_LONGD	%d and %ld/%D are equal
 */

PRIVATE char	nullStr[]=	"(null)";

#define MAXDIG		11	/* 32 bits in radix 8 */






PRIVATE char _far *
_itoa(char _far *p,unsigned num,int radix)
{
    int  i;
    char _far *q;

    q = p + MAXDIG;
    do
    {
	i = (int) (num % radix);
	i += '0';
	if (i > '9') i += 'A' - '0' - 10;
	*(--q) = (char)i;
    }
    while( num /= radix );
    i = p + MAXDIG - q;
    do
	*p++ = *q++;
    while( --i );
    return(p);
}




#ifndef NO_LONGD
PRIVATE char _far *
ltoa(char _far *p,unsigned long num,int radix)
{
    int       i;
    char _far *q;

    q = p + MAXDIG;
    do
    {
	i = (int)(num % radix);
	i += '0';
	if( i > '9' )
	    i += 'A' - '0' - 10;
	*(--q) = (char)i;
    }
    while( num /= radix );
    i = p + MAXDIG - q;
    do
	*p++ = *q++;
    while( --i );
    return(p);
}
#endif




#define GETARG(typ)	va_arg(args, typ)

PUBLIC void
sprintk(char _far *outs,const char _far *fmt,...)
{
    char buf[MAXDIG + 1];			/* +1 for sign */
    va_list   args;
    char _far *p;
    char _far *s;
    int   c, i;
    short width;
    short ndigit;
    int   ndfnd, ljust, zfill;
#ifndef NO_LONGD
    int   lflag;
    long  l;
#endif

    va_start( args, fmt );
    for (;;)
    {
	c = *(fmt++);
	if( c == 0 )				/* Ende des Strings? */
	{
	    va_end( args );
	    *outs = '\0';
	    return;
	}
	if (c != '%')				/* Kein Formatanfang? */
	{
	    *outs++ = (char)c;			/* ausgeben */
	    continue;				/* und n„chstes Zeichen */
	}
	p = buf;
	s = buf;
	ljust = 0;
	if (*fmt == '-')			/* Format beginnt '-' */
	{
	    fmt++;
	    ljust++;				/* left justified */
	}
	zfill = ' ';				/* Fllzeichen SPC */
	if (*fmt == '0')			/* Format dann '0' */
	{
	    fmt++;
	    zfill = '0';			/* Fllzeichen '0' */
	}
	for (width = 0;;)
	{
	    c = *(fmt++);
	    if( c >= '0'  &&  c <= '9' )
		c -= '0';
	    else
	    {
		if( c == '*' )
		    c = GETARG(int);
		else
		    break;
	    }
	    width *= 10;
	    width += c;
	}
	ndfnd = 0;
	ndigit = 0;
	if( c == '.' )
	{
	    for (;;)
	    {
		c = *(fmt++);
		if( c >= '0'  &&  c <= '9' )
		    c -= '0';
		else
		    if( c == '*' )
			c = GETARG(int);
		    else
			break;
		ndigit *= 10;
		ndigit += c;
		ndfnd++;
	    }
	}
#ifndef NO_LONGD
	lflag = 0;
#endif
	if( c == 'l' || c == 'L' )
	{
#ifndef NO_LONGD
	    lflag++;
#endif
	    if( *fmt )
		c = *(fmt++);
	}
	switch( c )
	{
	  case 'X':
#ifndef NO_LONGD
	    lflag++;
#endif
	  case 'x':
	    c = 16;
	    goto oxu;

	  case 'U':
#ifndef NO_LONGD
	    lflag++;
#endif
	  case 'u':
	    c = 10;
	    goto oxu;

	  case 'O':
#ifndef NO_LONGD
	    lflag++;
#endif
	  case 'o':
	    c = 8;
	  oxu:
#ifndef NO_LONGD
	    if( lflag )
	    {
		p = ltoa(p, (unsigned long)GETARG(long), c);
		break;
	    }
#endif
	    p = _itoa(p, (unsigned int)GETARG(int), c);
	    break;

	  case 'D':
#ifndef NO_LONGD
	    lflag++;
#endif
	  case 'd':
#ifndef NO_LONGD
	    if( lflag )
	    {
		if( (l=GETARG(long)) < 0 )
		{
		    *p++ = '-';
		    l = -l;
		}
		p = ltoa(p, (unsigned long)l, 10);
		break;
	    }
#endif
	    if( (i=GETARG(int)) < 0 )
	    {
		*p++ = '-';
		i = -i;
	    }
	    p = _itoa(p, (unsigned int)i, 10);
	    break;

	  case 'e':
	  case 'f':
	  case 'g':
	    zfill = ' ';
	    *p++ = '?';
	    break;

	  case 'c':
	    zfill = ' ';
	    *p++ = (char)GETARG(int);
	    break;

	  case 's':
	    zfill = ' ';
	    if ((s = GETARG(char _far *)) == 0) s = nullStr;
	    if (ndigit == 0) ndigit = 32767;
	    for( p=s; *p && --ndigit >= 0; p++ )
		;
	    break;

	  default:
	    *p++ = (char)c;
	    break;
	}
	i = p - s;
	if( (width -= i) < 0 )
	    width = 0;
	if( ljust == 0 )
	    width = -width;
	if( width < 0 )
	{
	    if( *s == '-' && zfill == '0' )
	    {
		*outs++ = *s++;
		i--;
	    }
	    do
		*outs++ = (char)zfill;
	    while( ++width != 0 );
	}
	while( --i >= 0 )
	    *outs++ = *s++;
	while( width )
	{
	    *outs++ = (char)zfill;
	    width--;
	}
    }

    return;
}
