/*
 * $Source: R:/Source/driver/raid/Setup/RCS/container.cpp,v $
 * $Revision: 1.7 $
 * $Date: 1999/04/16 00:59:27 $
 * $Locker:  $
 *
 *	Handles container in tree view.
 *
 * $Log: container.cpp,v $
 * Revision 1.7  1999/04/16 00:59:27  vitus
 * - added LookupDisk(), searches record for specific VRDev
 *
 * Revision 1.6  1999/03/01 02:23:03  vitus
 * - include dskinfo.hpp for drvbuild.h
 *
 * Revision 1.5  1999/02/09 02:46:31  vitus
 * - Umstellung auf C++
 *
 * Revision 1.4  1998/12/13 23:36:36  vitus
 * - changed as global headers moved to /source/Lib
 *
 * Revision 1.3  1998/11/05 00:23:16  vitus
 * - some container handlers are now PUBLIC (non-DEBUG detects :-)
 *
 * Revision 1.2  1998/05/27 02:00:19  vitus
 * - added QueryAny() returns count/pointer to all records
 *
 * Revision 1.1  1998/05/18 00:45:47  vitus
 * Initial revision
 * -------------------------------------------
 * This code is Copyright Vitus Jensen 1998-99
 */
static char const id[]="$Id: container.cpp,v 1.7 1999/04/16 00:59:27 vitus Exp $";

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define __IBMC__	0
#define INCL_DOS
#define INCL_DOSDEVIOCTL
#define INCL_ERRORS
#define INCL_WIN
#define INCL_GPI
#include <os2.h>

extern "C" {
#include "../../../Lib/defines.h"
#include "../../../Lib/verbose.h"
#include "../../../Lib/profile.h"		/* profile handling */
}
#include "resources.h"
#include "vsetup.h"

#include "../dsklayout.h"
#include "dskinfo.hpp"
#include "drvbuild.h"






/*# ----------------------------------------------------------------------
 * CALL
 *	EmphSelect(hwnd,myrecord,pbSelect)
 * PARAMETER
 *	hwndCnr		handle of the container window
 *	myrecord	pointer to the container record
 *	pbSelect	pointer to a BOOL specifying whether to select the
 *			record or not
 * RETURNS
 *	FALSE
 *
 * DESCRIPTION
 *	This function is used to select/deselect all records.  Note that it
 *	always returns FALSE, so that SearchCnr() will traverse the entire
 *	record list.
 *
 * REMARKS
 */
PUBLIC BOOL
EmphSelect(HWND hwnd,PMYRECORD myrecord,PBOOL pbSelect,PULONG cnt)
{
    ++(*cnt);
    WinSendMsg(hwnd, CM_SETRECORDEMPHASIS,
	       MPFROMP(myrecord), MPFROM2SHORT(*pbSelect,CRA_SELECTED));
    return FALSE;
}




/*# ----------------------------------------------------------------------
 * CALL
 *	SearchSource(hwnd,myrecord,pbSource)
 * PARAMETER
 *	hwnd		handle of the container window
 *	myrecord	pointer to the container record
 *	pbSelect	pointer to a BOOL specifying whether to select the
 *			record or not
 * RETURNS
 *	FALSE
 *
 * DESCRIPTION
 *	This function is used to select/deselect all records.  Note that it
 *	always returns FALSE, so that SearchCnr() will traverse the entire
 *	record list.
 *
 * REMARKS
 */
PUBLIC BOOL
SearchSource(HWND hwnd,PMYRECORD myrecord,PBOOL pbSource,PULONG cnt)
{
    if( (myrecord->record.flRecordAttr & CRA_SELECTED) != 0 )
    {
	++(*cnt);
	WinSendMsg(hwnd, CM_SETRECORDEMPHASIS,
		   MPFROMP(myrecord), MPFROM2SHORT(*pbSource,CRA_SOURCE));
    }

    return FALSE;
}




/*# ----------------------------------------------------------------------
 * CALL
 *	EmphInuse2Source(hwnd,myrecord,pbSource)
 * PARAMETER
 *	hwnd		handle of the container window
 *	myrecord	pointer to the container record
 *	pbSelect	pointer to a BOOL specifying whether to select the
 *			record or not
 * RETURNS
 *	FALSE
 *
 * DESCRIPTION
 *	This function is used to select/deselect records.  Note that it
 *	always returns FALSE, so that SearchCnr() will traverse the entire
 *	record list.
 *
 * REMARKS
 */
PUBLIC BOOL
EmphInuse2Source(HWND hwnd,PMYRECORD myrecord,PBOOL pbSource,PULONG cnt)
{
    if( (myrecord->record.flRecordAttr & CRA_INUSE) != 0 )
    {
	++(*cnt);
	WinSendMsg(hwnd, CM_SETRECORDEMPHASIS,
		   MPFROMP(myrecord), MPFROM2SHORT(*pbSource,CRA_SOURCE));
    }
    return FALSE;
}




/*# ----------------------------------------------------------------------
 * CALL
 *	EmphInuse(hwnd,myrecord,pbSource)
 * PARAMETER
 *	hwnd		handle of the container window
 *	myrecord	pointer to the container record
 *	pbSelect	pointer to a BOOL specifying whether to select the
 *			record or not
 * RETURNS
 *	FALSE
 *
 * DESCRIPTION
 *	This function is used to mark/unmark all records as 'inuse'.  Note
 *	that it always returns FALSE, so that SearchCnr() will traverse the
 *	entire record list.
 *
 * REMARKS
 */
PUBLIC BOOL
EmphInuse(HWND hwnd,PMYRECORD myrecord,PBOOL pbSource,PULONG cnt)
{
    ++(*cnt);
    WinSendMsg(hwnd, CM_SETRECORDEMPHASIS,
	       MPFROMP(myrecord), MPFROM2SHORT(*pbSource,CRA_INUSE));
    return FALSE;
}




/*# ----------------------------------------------------------------------
 * CALL
 *	EmphSource(hwnd,myrecord,pbSource)
 * PARAMETER
 *	hwnd		handle of the container window
 *	myrecord	pointer to the container record
 *	pbSelect	pointer to a BOOL specifying whether to select the
 *			record or not
 * RETURNS
 *	FALSE
 *
 * DESCRIPTION
 *	This function is used to mark/unmark all records as 'source'.  Note
 *	that it always returns FALSE, so that SearchCnr() will traverse the
 *	entire record list.
 *
 * REMARKS
 */
PUBLIC BOOL
EmphSource(HWND hwnd,PMYRECORD myrecord,PBOOL pbSource,PULONG cnt)
{
    ++(*cnt);
    WinSendMsg(hwnd, CM_SETRECORDEMPHASIS,
	       MPFROMP(myrecord), MPFROM2SHORT(*pbSource,CRA_SOURCE));
    return FALSE;
}




/*# ----------------------------------------------------------------------
 * CALL
 *	QueryInuse(hwnd,myrecord,pulCnt,record_array)
 * PARAMETER
 *	hwnd		handle of the container window
 *	myrecord	pointer to the container record
 *	cnt
 *	record_array
 *
 * RETURNS
 *	FALSE
 *
 * DESCRIPTION
 *	This function is used scan all records for those which are marked
 *	"in-use".  Note that this function always returns FALSE, so that SearchCnr()
 *	will traverse the entire record list.
 *
 * REMARKS
 */
PUBLIC BOOL
QueryInuse(HWND hwnd,PMYRECORD myrecord,PULONG cnt,PMYRECORD record_array[])
{
    if( (myrecord->record.flRecordAttr & CRA_INUSE) != 0 )
    {
	record_array[*cnt] = myrecord;
	++(*cnt);
	WinSendMsg(hwnd, CM_SETRECORDEMPHASIS,
	           MPFROMP(myrecord), MPFROM2SHORT(*cnt,CRA_SOURCE));
    }
    return FALSE;
}




/*# ----------------------------------------------------------------------
 * CALL
 *	QueryAny(hwnd,myrecord,pulCnt,record_array)
 * PARAMETER
 *	hwnd		handle of the container window
 *	myrecord	pointer to the container record
 *
 * RETURNS
 *	FALSE
 *
 * DESCRIPTION
 *	This function is used scan all records-
 *	Note that this function always returns FALSE, so that
 *	SearchCnr() will traverse the entire record list.
 *
 * REMARKS
 */
PUBLIC BOOL
QueryAny(HWND hwnd,PMYRECORD myrecord,PULONG cnt,PMYRECORD record_array[])
{
    record_array[*cnt] = myrecord;
    ++(*cnt);
    return FALSE;
}




/*# ----------------------------------------------------------------------
 * CALL
 *	LookupDisk(hwnd,myrecord,disk,record_returned)
 * PARAMETER
 *	hwnd		handle of the container window
 *	myrecord	pointer to the container record
 *
 * RETURNS
 *	FALSE
 *
 * DESCRIPTION
 *	This function is used scan all records-
 *	Note that this function always returns FALSE, so that
 *	SearchCnr() will traverse the entire record list.
 *
 * REMARKS
 */
PUBLIC BOOL
LookupDisk(HWND hwnd,PMYRECORD myrecord,VRDev * disk,PMYRECORD * record_returned)
{
    if( myrecord->dsk == disk )
    {
	*record_returned = myrecord;
	return TRUE;				// found matching record, end search
    }
    return FALSE;
}




/*# ----------------------------------------------------------------------
 * CALL
 *	SearchCnr(hwndCnr,pfnSearch,pvUser1,pvUser2,pvRsrvd)
 * PARAMETER
 *	hwndCnr		handle of the container
 *	pfnSearch	pointer to the search function
 *	pvUser		pointer to user-data, passed to pfnSearch()
 *	pvRsrvd		reserved and must be NULL
 * RETURNS
 *	record found, or NULL otherwise
 *
 * DESCRIPTION
 *	This function performs a post-traversal search on the container records
 *	until either 1) pfnSearch() returns TRUE (specifying that the record
 *	matched the criteria in pvUser) or 2) the last record is checked without
 *	a match being found.  The record, if any, is returned.
 *
 * REMARKS
 */
PUBLIC PVOID
SearchCnr(HWND hwndCnr,PFNSRCH pfnSearch,PVOID pvUser1,PVOID pvUser2,PVOID pvRsrvd)
{
    PVOID	pvChild;
    PVOID	pvResult;

    /* If the reserved parm is NULL, this is not a recursive call.  Get the
     * first child of the record specified here (or the first record if
     * pvRsrvd is NULL). */

    if( pvRsrvd == NULL )
    {
	pvChild = (PVOID)WinSendMsg(hwndCnr, CM_QUERYRECORD,
				    MPFROMP(NULL),
				    MPFROM2SHORT(CMA_FIRST,CMA_ITEMORDER));
    }
    else
    {
	pvChild = (PVOID)WinSendMsg(hwndCnr, CM_QUERYRECORD,
				    MPFROMP(pvRsrvd),
				    MPFROM2SHORT(CMA_FIRSTCHILD,CMA_ITEMORDER));
    }

    /* Process each child */

    while( pvChild != NULL )
    {
	pvResult = SearchCnr(hwndCnr, pfnSearch, pvUser1, pvUser2, pvChild);
	if( pvResult != NULL )
	    return pvResult;

	pvChild = (PVOID)WinSendMsg(hwndCnr, CM_QUERYRECORD,
				    MPFROMP(pvChild),
				    MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));
    }

    /* Finally, check ourselves */

    if( pvRsrvd != NULL )
    {
	if( (*pfnSearch)(hwndCnr, pvRsrvd, pvUser1, (PULONG)pvUser2))
	    return pvRsrvd;
	else
	    return NULL;
    }
    else
    {
	/* If we reach here, it means we traversed the entire container
	 * without finding a match. */

	return NULL;
    }
}
