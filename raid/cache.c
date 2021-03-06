/*
 * $Source: /home/vitus/Data/Source/driver/raid/RCS/cache.c,v $
 * $Revision: 1.1 $
 * $Date: 2008/09/23 06:20:15 $
 * $Locker:  $
 *
 *
 *	This module administrates a shared memory arena for all
 *	created caches.
 *	The memory arena is allocated in one or more linear
 *	continous ranges, non-swapable to make this cache usable
 *	in any context.
 *	OBS: caches aren't physically continous!
 *
 * $Log: cache.c,v $
 * Revision 1.1  2008/09/23 06:20:15  vitus
 * Initial revision
 *
 * ----------------------------------------
 * This code is Copyright Vitus Jensen 2001
 */
static char const vcid[]="$Id: cache.c,v 1.1 2008/09/23 06:20:15 vitus Exp $";

#include <string.h>

#define INCL_NOBASEAPI
#define INCL_INITRP_ONLY
#include "os2intf.h"

#include "vraid.h"
#include "proto.h"
#include "extern.h"

#define CACHE_BLKS	32




/* ******************************************************************* *
 * **** Memory Pool ************************************************** *
 * ******************************************************************* */

typedef struct {
    struct {
	int	valid : 1;
	int	dirty : 1;
    };

    ULONG	stripe;
    LIN		data;
} CACHEBLK, * PCACHEBLK;


PRIVATE struct _CACHEDRV {
    PVRDEVHDR	drv;
    USHORT	stripesize;

    CACHEBLK	aBlk[CACHE_BLKS];

} CacheDrv[MAX_PDEV] = {0};






/* **********************************************************************
 * **** Private Routines ************************************************
 * ******************************************************************* */






/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * <fkt>
 *
 * PARAMETER
 * RETURNS
 * DESCRIPTION
 * REMARKS
 */
PUBLIC USHORT
RequestCacheBlock(PVRDEVHDR drv, ULONG stripeno, PVOID data)
{
}




/*# ----------------------------------------------------------------------
 * <fkt>
 *
 * PARAMETER
 * RETURNS
 * DESCRIPTION
 * REMARKS
 */
PUBLIC USHORT
CreateCache(PVRDEVHDR drv, USHORT stripesize)
{
    USHORT	rc;
    int		i, j;
    int		allocated = 0;


    for( i = 0; i < MAX_PDEV; ++i )
    {
	if( CacheDrv[i].drv == NULL )
	{
	    CacheDrv[i].drv = drv;
	    CacheDrv[i].stripesize = stripesize;

	    for( j = 0; j < CACHE_BLKS; ++j )
	    {
		LIN	new;

		CacheDrv[i].aBlk[j].valid = 0;
		CacheDrv[i].aBlk[j].dirty = 0;
		rc = AllocateFixed(stripesize, &new);
		if( rc == 0 )
		{
		    ++allocated;
		    CacheDrv[i].aBlk[j].data = new;
		}
		else
		    CacheDrv[i].aBlk[j].data = 0;
	    }
	}
    }

    if( allocated != 0 )
	return 0;
    else
	return STATUS_ERR_PAPEROUT;
}




/*# ----------------------------------------------------------------------
 * DestroyCache(drv)
 *
 * PARAMETER
 * RETURNS
 * DESCRIPTION
 * REMARKS
 */
PUBLIC USHORT
DestroyCache(PVRDEVHDR drv)
{
    USHORT	rc = STATUS_ERR_UNKUNIT;
    int		i, j;

    for( i = 0; i < MAX_PDEV; ++i )
    {
	if( CacheDrv[i].drv == drv )
	{
	    for( j = 0; j < CACHE_BLKS; ++j )
	    {
		if( CacheDrv[i].aBlk[j].data != 0 )
		    FreeFixed(CacheDrv[i].aBlk[j].data);
		CacheDrv[i].aBlk[j].valid = 0;
		CacheDrv[i].aBlk[j].dirty = 0;
	    }
	    CacheDrv[i].drv = 0;
	    rc = 0;
	    break;
	}
    }

    return rc;
}
