/*
 * $Source: R:/source/driver/raid/RCS\\filter.c,v $
 * $Revision: 1.4 $
 * $Date: 2000/04/15 22:22:32 $
 * $Locker:  $
 *
 *	IORB Filter which passes most IORBs to the underlying ADD.
 *
 * $Log: filter.c,v $
 * Revision 1.4  2000/04/15 22:22:32  vitus
 * - now updates iocnt of PHYSDEV.  Again.
 *   For some reason I don't see where the changes between 1.2 and 1.3 contained
 *   this update stuff and as it needed a check anyways (PHYSDEV may not exist),
 *   here is the new revision.
 *
 * Revision 1.3  2000/03/27 22:58:56  vitus
 * - removed update of iocnt: there might be no parent and -- more important --
 *   don't update an array's iocount as it is used to get the OS/2 drive - array
 *   relationship.
 *
 * Revision 1.2  1999/10/01 00:59:28  vitus
 * - FilterHandler: updates iocnt in VRDEVHDR of PHYSDEV residing on same unit
 *
 * Revision 1.1  1999/09/18 14:37:38  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1999-2000
 */
#if defined(__WATCOMC__)
# pragma off (unreferenced)
#endif
static char const id[]="$Id: filter.c,v 1.4 2000/04/15 22:22:32 vitus Exp $";
#if defined(__WATCOMC__)
# pragma on (unreferenced)
#endif

#include <string.h>

#define INCL_NOBASEAPI
#define INCL_NOPMAPI
#define INCL_INITRP_ONLY
#include "os2intf.h"

#include "vraid.h"
#include "proto.h"
#include "extern.h"


#if defined(DEBUG)
CONSTMSG dszFilterHandler[]=	"\r\n[FilterHandler] disk %p, iorb %p";
CONSTMSG dszFilterIo[]=		"\r\n[FilterHandler] Command Code %w, Modifier %w";
CONSTMSG dszAllocUnit[]=	"\r\n[FilterHandler] IOCM_ALLOCATE_UNIT";
CONSTMSG dszAllocUnitAlloc[]=	"\r\n[FilterHandler] HD already allocated";
CONSTMSG dszDeallocUnit[]=	"\r\n[FilterHandler] IOCM_DEALLOCATE_UNIT";
CONSTMSG dszDeallocUnitAlloc[]=	"\r\n[FilterHandler] HD not allocated";
CONSTMSG dszChangeUnit[]=	"\r\n[FilterHandler] IOCM_CHANGE_UNITINFO";
CONSTMSG dszIorbUnknown[]=	"\r\n[FilterHandler] Command %w, Modifier %w not supported";
CONSTMSG dszIorbPass[]=		"\r\n[FilterHandler] pass IORB to ADD (%p)";
#endif






/*# ----------------------------------------------------------------------
 * FilterHandler(pIorb)
 *
 * PARAMETER
 *	disk			Baseunit to route to
 *	pIorb			I/O request buffer
 *
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 * REMARKS
 */
PUBLIC void NEAR _Cdecl
FilterHandler(PBASEDISK const disk,PIORB const pIorb)
{
    USHORT const	CommandCode	= pIorb->CommandCode;
    USHORT const	CommandModifier	= pIorb->CommandModifier;
    PIORB		p;


    DEBMSG2(DBG_PATH,dszFilterHandler,(PVOID)disk,(PVOID)pIorb);
    DEBMSG2(DBG_LEVEL3,dszFilterIo,CommandCode,CommandModifier);
    ASSERT( (fDriverFlags & DF_NOROUTER) == 0 );

    switch( CommandCode )
    {
      case IOCC_UNIT_CONTROL:			/* all mandatory */
	switch( CommandModifier )
	{
	    /* ****************************************************
	     **** IOCM_ALLOCATE_UNIT:			   
	     **** Used by device manager to get control of   
	     **** a single unit; I/O may be done only to	
	     **** allocated units and units can only be	   
	     **** allocated once. 			   
	     **************************************************** */
	  case IOCM_ALLOCATE_UNIT:
	    DEBMSG(DBG_LEVEL2,dszAllocUnit);
	    if( (disk->OsFlags & OF_ALLOCATED) )
	    {
		DEBMSG(DBG_ALL,dszAllocUnitAlloc);
		NotifyIorb(pIorb, IOERR_UNIT_ALLOCATED);
		break;
	    }
	    disk->OsFlags |= OF_ALLOCATED;
	    NotifyIorb(pIorb, 0);
	    break;


	    /* **************************************************** 
	     **** IOCM_DEALLOCATE_UNIT:			  
	     **** Used by device manager to release control  
	     **** of an unit, so other manager can use it.   
	     **** Currently only OS2ASPI.DMD uses this call. 
	     **************************************************** */
	  case IOCM_DEALLOCATE_UNIT:
	    DEBMSG(DBG_LEVEL2,dszDeallocUnit);
	    if( !(disk->OsFlags & OF_ALLOCATED) )
	    {
		DEBMSG(DBG_ALL,dszDeallocUnitAlloc);
		NotifyIorb(pIorb, IOERR_UNIT_NOT_ALLOCATED);
		break;
	    }
	    NotifyIorb(pIorb, 0);
	    break;


	    /* ****************************************************
	     **** IOCM_CHANGE_UNITINFO:			   
	     **** Used by: ????				   
	     **** Usage: ????				   
	     **** May by used to set a different unit info   
	     **** (FilterADDHandle, ...)		       
	     **************************************************** */
	  case IOCM_CHANGE_UNITINFO:
	    DEBMSG(DBG_LEVEL2,dszChangeUnit);
	    disk->info = *((PIORB_UNIT_CONTROL)pIorb)->pUnitInfo;
	    NotifyIorb(pIorb, 0);
	    break;

	  default:
	    DEBMSG2(DBG_ALL,dszIorbUnknown,CommandCode,CommandModifier);
	    DBSTOP();
	    NotifyIorb(pIorb, IOERR_CMD_NOT_SUPPORTED);
	    break;
	}
	break;

	/* All other requests: pass directly to ADD. */
      default:
	ASSERT( disk != NULL );
	ASSERT( disk->phys != NULL );
	if( disk->phys->parent != NULL )
	    ++disk->phys->parent->iocnt;
	DEBMSG1(DBG_LEVEL2,dszIorbPass,(PVOID)disk->phys->pADDEntry);
	for( p = pIorb; p != NULL;
	     p = (p->RequestControl & IORB_CHAIN ? p->pNxtIORB : NULL) )
	    p->UnitHandle = disk->phys->hdADDUnit;
	CallADD(disk->phys->pADDEntry, pIorb);
	break;
    }

    return;
}
