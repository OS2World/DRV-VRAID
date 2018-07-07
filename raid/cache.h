/*
 * $Source: /home/vitus/Data/Source/driver/raid/RCS/cache.h,v $
 * $Revision: 1.1 $
 * $Date: 2008/09/23 06:20:51 $
 * $Locker:  $
 *
 *	Interface to Caching
 *
 * $Log: cache.h,v $
 * Revision 1.1  2008/09/23 06:20:51  vitus
 * Initial revision
 *
 * ----------------------------------------
 * This code is Copyright Vitus Jensen 2001
 */

typedef struct _CACHEBLK {

    /* Only used by cache module: */

    PVOID	buffer;				/* of 'stripesize' bytes */
    PVRIO	io;				/* allocated if 'inuse' */
    struct {
	int	inuse : 1;
	int	valid : 1;
	int	dirty : 1;
    };

    /* Public elements: */

    PVRDEVHDR	drv;				/* data coming from this drive */
    ULONG	secno;
    
} CACHEBLK, FAR * PCACHEBLK;


extern USHORT	CacheCreate(PVRDEVHDR drv, USHORT stripesize);
extern USHORT	CacheDestroy(PVRDEVHDR drv);

extern USHORT	CacheRequestBlock(PVRDEVHDR child, ULONG secno, ULONG seccnt,
				  PCACHEBLK allocated);
extern USHORT	CacheProcessBlock(PCACHEBLK cblk);
extern USHORT	CacheReleaseBlock(PCACHEBLK cblk);
