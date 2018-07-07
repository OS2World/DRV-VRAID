/*
 * $Source: r:/source/driver/raid/setup/RCS/drvsetup.cpp,v $
 * $Revision: 3.2 $
 * $Date: 2006/01/16 21:20:30 $
 * $Locker:  $
 *
 *	Select/define RAID devices.
 *
 * $Log: drvsetup.cpp,v $
 * Revision 3.2  2006/01/16 21:20:30  vitus
 * - define NORAID5: generate code for current VRAID.FLT, not the one
 *   that might follow and support RAID5 (if it ever will)
 * - more verbose calls
 *
 * Revision 3.1  2003/09/24 10:43:54  vitus
 * - added RAID 5 code (new major because of "stopped" development)
 *
 * Revision 2.15  2001/01/15 01:41:13  vitus
 * - changed handling of poup window.  It got too complicated to determine
 *   which entry to enable/disable.  Hopefully it's now easier.
 * - implemtned IDM_INVALID_CHILD related code
 *
 * Revision 2.14  2000/11/07 01:30:19  vitus
 * - WUM_DEVTREE: only save scanner HWND on first message
 *
 * Revision 2.13  2000/08/21 08:11:20  vitus
 * - moved filter access checking to DrvVerifyThread()
 * - modifications for changed addChild() prototype
 *
 * Revision 2.12  2000/08/05 02:58:30  vitus
 * - Rebuild and Verify are now in the context menue.  Added processing for them
 *
 * Revision 2.11  2000/05/28 17:44:09  vitus
 * - adjusted verbosity levels
 *
 * Revision 2.10  2000/05/15 22:59:01  vitus
 * - added calls to PPSubclassWindow(), saves presentation parameters
 *
 * Revision 2.9  2000/04/10 01:12:09  vitus
 * - converted calls to WinMessageBox() to MyMessageBox()
 *
 * Revision 2.8  2000/03/09 23:42:41  vitus
 * - changed C++ to C comments
 * - adjusted verbosity levels
 *
 * Revision 2.7  1999/07/05 01:00:08  vitus
 * - DragOver: corrected source/destination handling, should now work
 *
 * Revision 2.6  1999/06/21 01:29:26  vitus
 * - corrected english spelling
 *
 * Revision 2.5  1999/06/19 21:59:31  vitus
 * - minor corrections to keep compiler happy
 *
 * Revision 2.4  1999/06/02 02:18:00  vitus
 * - DoPopup: IDM_REMOVE_CHILD only possible if another child is valid
 * - IDM_DESTROY_DRV: WUM_DELDRV handles adding childs to container
 *
 * Revision 2.3  1999/05/13 23:04:18  vitus
 * - all warning messages include drive ID
 * - implemented IDM_REMOVE_CHILD: warns, posts WUM_REMCHILD
 *
 * Revision 2.2  1999/04/16 01:03:21  vitus
 * - DragDrop: update complete tree when dropping on a child
 *
 * Revision 2.1  1999/04/13 01:24:08  vitus
 * - implemented first step of drag-n-drop support: add something to chain, mirror
 *
 * Revision 1.11  1999/03/24 23:06:00  vitus
 * - corrected popup menue on new array drives, did contain remove PHYSDEV...
 * - WinPostMsg() instead of WinSendMsg()
 *
 * Revision 1.10  1999/03/08 05:05:34  vitus
 * - uses queryIcon() method, all local icon handling removed
 *
 * Revision 1.9  1999/03/01 02:26:05  vitus
 * - include dskinfo.hpp for drvbuild.h
 * - reworked code because DSKINFO != VRDev
 *
 * Revision 1.8  1999/02/09 02:49:48  vitus
 * - Umstellung auf C++
 *
 * Revision 1.7  1998/12/17 02:22:02  vitus
 * - display "OS/2 Device x" at host drives
 *
 * Revision 1.6  1998/11/03 03:03:48  vitus
 * - new DEVICE structures get 'avail = 0' if any of their childs are
 *   'avail = 0' otherwise they are created as 'avail = 1'
 * - modified for DSKINFO change: 'avail' element changed position
 *
 * Revision 1.5  1998/09/27 01:52:03  vitus
 * - added "disabled" icons for pdevs and arrays
 * - different messages why double click isn't possible
 * - recognizes  DSKACC_FLT
 *
 * Revision 1.4  1998/06/09 01:25:56  vitus
 * - seperate description for PHYSDEV devices to display logical
 *   size (not partition size)
 * - explanation of lower RAID levels
 * - implemented context menue IDM_DELETE_PDEV
 *
 * Revision 1.3  1998/05/27 01:58:23  vitus
 * - another icon: missing device
 * - implemented IDM_CREATE_PDEV
 *
 * Revision 1.2  1998/05/18 23:03:00  vitus
 * - seperate thread to scan/build devices/-tree
 * - displays devices with 3 diff. icons
 * - popup menu plus actions (not complete)
 *
 * Revision 1.1  1998/05/06 22:06:37  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2001
 */
static char const vcid[]="$Id: drvsetup.cpp,v 3.2 2006/01/16 21:20:30 vitus Exp $";
static char const modname[]="DrvSetup";

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define INCL_DOS
#define INCL_DOSDEVIOCTL
#define INCL_ERRORS
#define INCL_WIN
#define INCL_WINSTDDRAG
#define INCL_GPI
#include <os2.h>

extern "C" {
#include "../../../Lib/defines.h"
#include "../../../Lib/verbose.h"
#include "../../../Lib/profile.h"		/* profile handling */
#include "../../../Lib/pphnd.h"			/* presparam handling */
}
#include "VSetup.h"
#include "resources.h"
#include "../dsklayout.h"
#include "dskinfo.hpp"
#include "drvbuild.h"
#include "drverify.h"

#include "Single.hpp"
#include "Chain.hpp"
#include "Stripe.hpp"
#include "Mirror.hpp"
#if !defined(NORAID5)
# include "Raid5.hpp"
#endif

#include "drvsetup.h"

extern USHORT	usHostId;






#define WC_DRVSETUP	"DrvsetupWindow"



typedef struct _WORK {
    PTHR_DRVSETUP self;
    HAB		hab;				/* to retrieve errors */
    HWND	hwndCnr, hwndScanner;
    HWND	hwndNonePopup;
    HWND	hwndSngPopup;
    HWND	hwndMulPopup;

    PMYRECORD	pmrMenu;			/* this record has menu attached */

    PMYRECORD	pmrDragSource;			/* this is dragged */
    PMYRECORD	pmrDragDestination;		/* to this record */ 
} WORK, * PWORK;






/* **********************************************************************
 * **** Private Data ****************************************************
 * ******************************************************************* */

PRIVATE LONG	rgbBackground = RGB_ERROR;
PRIVATE LONG	rgbForeground = RGB_ERROR;






/* **********************************************************************
 * **** Window related Routines *****************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * LoadPresParam(app)
 *
 * PARAMETER
 * RETURNS
 * GLOBAL
 * DESCRIPTION
 *	If RGB value of background/foreground isn't set, retrieve
 *	system standard values first.
 *	Second: read profile settings, won't change anything if
 *	settings aren't defined.
 *
 * REMARKS
 */
PRIVATE VOID
LoadPresParam(char * app)
{
    if( rgbBackground == RGB_ERROR )
    {
	unsigned	cb;

	rgbBackground = WinQuerySysColor(HWND_DESKTOP, SYSCLR_WINDOW, 0);
	cb = sizeof(rgbBackground);
	ProfileRead(app, "Background", &cb, &rgbBackground);
    }
    if( rgbForeground == RGB_ERROR )
    {
	unsigned	cb;

	rgbForeground = WinQuerySysColor(HWND_DESKTOP, SYSCLR_OUTPUTTEXT, 0);
	cb = sizeof(rgbForeground);
	ProfileRead(app, "Foreground", &cb, &rgbForeground );
    }
    return;
}




/*# ----------------------------------------------------------------------
 * CreateCnr(hwndClient,work)
 *
 * PARAMETER
 *	hwndClient
 *	work
 *
 * RETURNS
 *	0		OK
 *	/0		can't create
 *
 * DESCRIPTION
 *	Creates a new container inside 'hwndClient'.
 *
 * REMARKS
 */
PRIVATE int
CreateCnr(HWND hwndClient,PWORK work)
{
    SWP		size;
    CNRINFO	cnrinfo;

    WinQueryWindowPos(hwndClient, &size);
    Verbose(4,modname, "client window %u,%u %u,%u",
	    size.x, size.y, size.cx, size.cy);

    work->hwndCnr = WinCreateWindow(hwndClient, WC_CONTAINER,
				    NULL,
				    WS_VISIBLE
				    |CCS_AUTOPOSITION|CCS_MINIRECORDCORE
				    |CCS_VERIFYPOINTERS
				    |CCS_EXTENDSEL|CCS_READONLY,
				    0, 0, size.cx, size.cy,
				    hwndClient, HWND_TOP,
				    IDW_DRVCN, NULL, NULL);
    if( work->hwndCnr == NULLHANDLE )
    {
	MyMessageBox(0, HWND_DESKTOP,
		     "WinCreateWindow(...,WC_CONTAINER,...) - error %#x",
		     WinGetLastError(work->hab));
	return 1;
    }
    PPSubclassWindow(work->hwndCnr, WC_DRVSETUP);

    memset(&cnrinfo, 0, sizeof(cnrinfo));
    cnrinfo.cb = sizeof(cnrinfo);
    cnrinfo.flWindowAttr = CV_TREE|CV_ICON |CA_TREELINE;
    WinSendMsg(work->hwndCnr, CM_SETCNRINFO,
	       MPFROMP(&cnrinfo), MPFROMLONG(CMA_FLWINDOWATTR));


    /* 1st: menues */

    work->hwndNonePopup = WinLoadMenu(HWND_OBJECT, NULLHANDLE, IDPM_NODRV);
    work->hwndSngPopup = WinLoadMenu(HWND_OBJECT, NULLHANDLE, IDPM_SNGDRV);
    work->hwndMulPopup = WinLoadMenu(HWND_OBJECT, NULLHANDLE, IDPM_MULDRV);
    if( work->hwndSngPopup == NULLHANDLE
	||  work->hwndMulPopup == NULLHANDLE
	||  work->hwndNonePopup == NULLHANDLE )
	Verbose(1,modname, "WinLoadMenu failed (last error %#x)", WinGetLastError(work->hab));
    Verbose(4, "DrvSetup", "popup menues %#x,%#x,%#x", work->hwndSngPopup, work->hwndMulPopup, work->hwndNonePopup);

    PPSubclassWindow(work->hwndNonePopup, WC_DRVSETUP);
    PPSubclassWindow(work->hwndSngPopup, WC_DRVSETUP);
    PPSubclassWindow(work->hwndMulPopup, WC_DRVSETUP);


    /* Container is now ready to accept items. */

    return 0;
}




/*# ----------------------------------------------------------------------
 * DoPopup(hwndClient,work)
 *
 * PARAMETER
 *	hwndClient		client area (container)
 *	work			PWORK
 *
 * RETURNS
 *	TRUE		???
 *
 * DESCRIPTION
 *	The user requested that the menu be displayed.
 *
 *	Pseudo code (EDM/2 1#5):
 *	if mouse over a container record
 *	    if record is selected
 *		add source emphasis to all selected records
 *	    else
 *		add source emphasis to this record
 *	else
 *	    select this whole container
 *	call WinPopupMenu
 *	undo source empasis changes
 *
 * REMARKS
 *	Note that there's a bug in the container such that
 *	if the user uses the keyboard to do this, we don't get
 *	notified, so that is why this function is called
 *	from both WM_CONTROL/CN_CONTEXTMENU and WM_CONTEXTMENU.
 */
PRIVATE BOOL
DoPopup(HWND hwndClient,PWORK const work)
{
    HWND		popup = NULLHANDLE;
    BOOL		bSet;

    POINTL		ptlMouse;
    WinQueryPointerPos(HWND_DESKTOP, &ptlMouse);

    QUERYRECFROMRECT	qrfrQuery;
    qrfrQuery.cb = sizeof(QUERYRECFROMRECT);
    qrfrQuery.rect.xLeft = ptlMouse.x;
    qrfrQuery.rect.xRight = ptlMouse.x+1;
    qrfrQuery.rect.yBottom = ptlMouse.y;
    qrfrQuery.rect.yTop = ptlMouse.y+1;
    WinMapWindowPoints(HWND_DESKTOP, hwndClient, (PPOINTL)&qrfrQuery.rect, 2);

    qrfrQuery.fsSearch = CMA_PARTIAL|CMA_ITEMORDER;

    work->pmrMenu
	= (PMYRECORD)WinSendMsg(work->hwndCnr, CM_QUERYRECORDFROMRECT,
				MPFROMP(CMA_FIRST), MPFROMP(&qrfrQuery));

    if( work->pmrMenu != NULL )
    {
	if( (work->pmrMenu->record.flRecordAttr & CRA_INUSE) != 0 )
	{
	    ULONG	cnt = 0;

	    bSet = TRUE;
	    SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse2Source, &bSet, &cnt, NULL);
	    Verbose(2,modname, "DoPopup: %lu records in use", cnt);
#if 0
	    if( cnt == 1 )
		popup = work->hwndSngPopup;
	    else
#endif
		popup = work->hwndMulPopup;
	}
	else
	{
	    popup = work->hwndSngPopup;
	    WinSendMsg(work->hwndCnr, CM_SETRECORDEMPHASIS,
		       MPFROMP(work->pmrMenu), MPFROM2SHORT(TRUE,CRA_SOURCE));
	}
    }
    else
    {
	popup = work->hwndNonePopup;
	WinSendMsg(work->hwndCnr, CM_SETRECORDEMPHASIS,
		   MPFROMP(NULL), MPFROM2SHORT(TRUE,CRA_SOURCE));
    }

    WinPopupMenu(HWND_DESKTOP, hwndClient, popup,
		 ptlMouse.x, ptlMouse.y,
		 0,
		 PU_HCONSTRAIN | PU_VCONSTRAIN | PU_NONE
		 | PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_MOUSEBUTTON2);


    if( popup == work->hwndSngPopup )
    {
	/* Single icon selected, modify menu according to drive type. */

	WinEnableMenuItem(popup, IDM_CREATE_PDEV, FALSE); /* assume: nothing */
	WinEnableMenuItem(popup, IDM_DELETE_PDEV, FALSE);
	WinEnableMenuItem(popup, IDM_REMOVE_CHILD, FALSE);
	WinEnableMenuItem(popup, IDM_INVALID_CHILD, FALSE);
	WinEnableMenuItem(popup, IDM_DESTROY_DRV, FALSE);
	WinEnableMenuItem(popup, IDM_VERIFY_DRV, FALSE);
	WinEnableMenuItem(popup, IDM_REBUILD_DRV, FALSE);

	if( work->pmrMenu->dsk != NULL )
	{
	    /* Figure out the verify / rebuild option.  They are
	     * independ from a possible array structure. */

	    if( work->pmrMenu->dsk != NULL )
	    {
		switch( work->pmrMenu->dsk->isArray() )
		{
		  case RDTYPE_MIRROR:
#if !defined(NORAID5)
		  case RDTYPE_RAID4:
		  case RDTYPE_RAID5:
#endif
		  {
		      VDriveWithRedundancy * const array
			  = (VDriveWithRedundancy*)work->pmrMenu->dsk;

		      if( array->queryState() == Ready )
		      {
			  WinEnableMenuItem(popup, IDM_VERIFY_DRV, TRUE);
			  WinEnableMenuItem(popup, IDM_REBUILD_DRV, TRUE);
		      }
		  }
		  break;

		  default:
		    break;
		}
	    }


	    /* Figure out whether this is a child which can be removed
	     * or declared as invalid. */

	    if( work->pmrMenu->dsk->getParent() != NULL )
	    {
		VRDrive * const	parent = work->pmrMenu->dsk->getParent();

		/* "dsk->parent != 0"
		 * This is part of a larger drive array.
		 * - it can't be destroyed as there is something
		 *	 relying on it.
		 * Nothing can be done.  Exceptions:
		 * - the parent is RAID 1 and this child isn't the last
		 *   valid child.
		 * - the parent is RAID 5 and higher and this child is
		 *   in "stand by" mode. */

		if( work->pmrMenu->dsk->getParent()->isArray() == RDTYPE_MIRROR )
		{
		    int const	cnt = parent->queryChildren();
		    int		i, v;

		    for( i = v = 0; i < cnt; ++i )
		    {
			VRDev * const	vrd = parent->getChild(i);
			if( vrd != work->pmrMenu->dsk
			    &&  parent->isValidChild(i) == True )
			    ++v;
		    }
		    if( v != 0 )
		    {
			/* Oh, there is another valid one!  So we can
			 * be removed... */

			WinEnableMenuItem(popup, IDM_REMOVE_CHILD, TRUE);
			WinEnableMenuItem(popup, IDM_INVALID_CHILD, TRUE);
		    }
		}

	    }

	    /* May this disk / drive / array be destroyed? */

	    if( work->pmrMenu->dsk->getParent() == NULL
		&&  work->pmrMenu->dsk->isArray() != 0 )
	    {
		WinEnableMenuItem(popup, IDM_DESTROY_DRV, TRUE);
	    }

	    /* Something to create a PHYSDEV on? */

	    if( work->pmrMenu->dsk->getParent() == NULL
		&&  work->pmrMenu->dsk->isArray() == 0
		&&  work->pmrMenu->dsk->isWritable() == True
		&&  work->pmrMenu->dsk->isHostdrive() == True )
	    {
		WinEnableMenuItem(popup, IDM_CREATE_PDEV, TRUE);
	    }

	    /* A PHYSDEV which could be deleted? */

	    if( work->pmrMenu->dsk->getParent() == NULL
		&&  work->pmrMenu->dsk->isArray() == 0
		&&  work->pmrMenu->dsk->isWritable() == True
		&&  work->pmrMenu->dsk->isHostdrive() == False )
	    {
		WinEnableMenuItem(popup, IDM_DELETE_PDEV, TRUE);
	    }
	}
    } /* hwndSngPopup */

    return TRUE;				/* xxx TRUE? */
}




/*# ----------------------------------------------------------------------
 * DisplayDisk(work,parent,dsk)
 *
 * PARAMETER
 *	work		thread specific data
 *	parent		drive to which 'dsk' is a child
 *	dsk		drive to display
 * RETURNS
 *	(nothing)
 *
 * DESCRIPTION
 *	Display device tree (recursive).
 *
 * REMARKS
 */
PRIVATE void
DisplayDisk(PWORK work,PMYRECORD parent,VRDev * dsk)
{
    RECORDINSERT	ins;
    PMYRECORD		address;
    char *		descr;
    ULONG const		extra = sizeof(MYRECORD)-sizeof(MINIRECORDCORE);


    address = (PMYRECORD)WinSendMsg(work->hwndCnr, CM_ALLOCRECORD,
				    MPFROMLONG(extra), MPFROMLONG(1));
    Verbose(4,modname, "CM_ALLOCRECORD: %#x",address);

    address->dsk = dsk;
    descr = dsk->allocateDescription();
    address->record.hptrIcon = dsk->queryIcon();
    address->record.pszIcon = descr;

    memset(&ins, 0, sizeof(ins));
    ins.cb = sizeof(ins);
    ins.pRecordOrder = (PRECORDCORE)CMA_END;
    ins.pRecordParent = (PRECORDCORE)parent;	/* none (only RAID drives have) */
    ins.zOrder = CMA_TOP;
    ins.fInvalidateRecord = TRUE;		/* invalidate record now/later */
    ins.cRecordsInsert = 1;


    int	i;
    i = (int)WinSendMsg(work->hwndCnr, CM_INSERTRECORD,
			  MPFROMP(address), MPFROMP(&ins));
    if( i == 0 )
	Verbose(1,modname, "WinSendMsg(CM_INSERTRECORD) failed - last error %#x", WinGetLastError(work->hab));
    else if( dsk != NULL  &&  dsk->isArray() )
    {
	VRDrive *	drv = (VRDrive *)dsk;	/* we are sure it's at least this */

	for( i = 0; i < drv->queryChildren(); ++i )
	    DisplayDisk(work, address, drv->getChild(i));
    }
    return;
}




/*# ----------------------------------------------------------------------
 * DragInit(hwndClient,work,drag)
 *
 * PARAMETER
 *	hwndClient		handle of client window, never (?) used
 *	work			window work structure
 *	drag			describes drag operation
 *
 * RETURNS
 *	0			always
 *
 * DESCRIPTION
 *	Called whenever a drag'n drop operation is requested by the
 *	user.  Verifies source record (valid PHYSDEVICE or higher, writable,
 *	etc.) and starts dragging by calling the approbiate Drg*() routines.
 *
 * REMARKS
 */
PRIVATE MRESULT
DragInit(HWND hwndClient,PWORK const work,PCNRDRAGINIT const drag)
{
    Verbose(2,modname, "DragInit: record %#x", drag->pRecord);
    if( drag->pRecord == NULL )
	return 0;				/* no record selected */
    if( work->pmrDragSource != NULL )
	return 0;				/* already active drag */


    /* Analyse drive to be dragged for logical possible and impossible
     * operations. */

    VRDev * const rdev = ((PMYRECORD)drag->pRecord)->dsk;
    if( rdev->getParent() != NULL )
	return 0;				/* sorry, no dragging of children */
    if( rdev->isWritable() == False )
	return 0;				/* can't write = no drag */
    if( rdev->isHostdrive() == True  &&  rdev->isArray() == 0 )
	return 0;				/* not even PHYSDEVICE! */


    /* Well, all informations tell me it is valid to drag
     * this drive around.  Tell presentation manager! */

    Verbose(2,modname, "DragInit: record passed checks");
    work->pmrDragSource = (PMYRECORD)drag->pRecord;

    PDRAGINFO	dinfo = DrgAllocDraginfo(1);
    DRAGITEM	ditem;
    ditem.hwndItem = hwndClient;
    ditem.ulItemID = (ULONG)drag->pRecord;
    ditem.hstrType = DrgAddStrHandle("DRT_CUSTOMER");
    ditem.hstrRMF = DrgAddStrHandle("DRM_SHAREMEM,DRM_UNKNOWN");
    ditem.fsControl = DC_REMOVEABLEMEDIA;	/* can't be recovered... */
    ditem.fsSupportedOps = DO_MOVEABLE;

    DrgSetDragitem(dinfo,			/* Set item in DRAGINFO */
		   &ditem,			/* Pointer to DRAGITEM */
		   sizeof(ditem),		/* Size of DRAGITEM */
		   0);				/* Index of DRAGITEM */

    DRAGIMAGE	dimage;
    dimage.cb = sizeof(DRAGIMAGE);		/* Initialize DRAGIMAGE */
    dimage.cptl = 0;				/* Not a polygon */
    dimage.hImage = work->pmrDragSource->dsk->queryIcon();
    dimage.fl = DRG_ICON;			/* Dragging an icon */
    dimage.cxOffset = 0;			/* No hotspot */
    dimage.cyOffset = 0;

    /*hDrop =*/ DrgDrag(hwndClient,		/* initiate drag */
			dinfo,			/* DRAGINFO structure */
			&dimage,		/* DRAGIMAGE structure */
			1,			/* Only one DRAGIMAGE */
			VK_ENDDRAG,		/* End of drag indicator */
			NULL);			/* Reserved */
 
    DrgFreeDraginfo(dinfo);			/* Free DRAGINFO struct */

    return 0;
}




/*# ----------------------------------------------------------------------
 * DragLeave(hwndClient,work,drag)
 *
 * PARAMETER
 *	hwndClient		handle of client window, never (?) used
 *	work			window work structure
 *	drag			describes drag operation
 * RETURNS
 *	0
 *
 * DESCRIPTION
 *	Drag operation aborted.  Cleanup work area.
 *
 * REMARKS
 */
PRIVATE MRESULT
DragLeave(HWND hwndClient,PWORK const work,PCNRDRAGINFO drag)
{
    Verbose(2,modname, "DragLeave, record %#x", drag->pRecord);

    work->pmrDragSource = NULL;			/* reflect in work area */
    work->pmrDragDestination = NULL;
    return 0;
}




/*# ----------------------------------------------------------------------
 * DragOver(hwndClient,work,drag)
 *
 * PARAMETER
 *	hwndClient		handle of client window, never (?) used
 *	work			window work structure
 *	drag			describes drag operation
 *
 * RETURNS
 *	Information abount drap operation's status DOR_*, DO_*
 *
 * DESCRIPTION
 *	Called whenever a dragged object changes it's position above
 *	our client area.  Has to determine wehther a drop would be possible.
 *	As we have to do those checks anyway, keep references in global
 *	variables for DROP operation.
 *
 * REMARKS
 *	Valid target for dropping are RDTYPE_MIRROR and RDTYPE_CHAIN.  Adding
 *	to any other target would destroy their data or require another user
 *	interaction (type of new VRAID drive).		xxx
 *
 */
PRIVATE MRESULT
DragOver(HWND hwndClient,PWORK const work,PCNRDRAGINFO drag)
{
    Verbose(2,modname, "DragOver, record %#x", drag->pRecord);


    /* Check the destination: if pRecord is NULL, the target of
     * drop operation is the container background.  This drop
     * doesn't make sense. */

    if( drag->pRecord == NULL )
    {
	Verbose(2,modname, "DragOver, no target");
	return MRFROM2SHORT(DOR_NODROP,DO_DEFAULT);
    }


    /* Determine position of dragged object (= mouse pointer). */

    POINTL		ptlMouse;
    WinQueryPointerPos(HWND_DESKTOP, &ptlMouse);

    QUERYRECFROMRECT	qrfrQuery;
    qrfrQuery.cb = sizeof(QUERYRECFROMRECT);
    qrfrQuery.rect.xLeft = ptlMouse.x;
    qrfrQuery.rect.xRight = ptlMouse.x+1;
    qrfrQuery.rect.yBottom = ptlMouse.y;
    qrfrQuery.rect.yTop = ptlMouse.y+1;
    WinMapWindowPoints(HWND_DESKTOP, hwndClient, (PPOINTL)&qrfrQuery.rect, 2);

    qrfrQuery.fsSearch = CMA_PARTIAL|CMA_ITEMORDER;


    /* Are there any objects located below the dragged object? */

    work->pmrDragDestination
	= (PMYRECORD)WinSendMsg(work->hwndCnr, CM_QUERYRECORDFROMRECT,
				MPFROMP(CMA_FIRST), MPFROMP(&qrfrQuery));
    if( work->pmrDragDestination == NULL )
    {
	Verbose(2,modname, "DragOver, no destination");
	return MRFROM2SHORT(DOR_NODROP,DO_DEFAULT);
    }

    if( work->pmrDragDestination != (PMYRECORD)drag->pRecord )
    {
	Verbose(1,modname, "DragOver, pRecord != pmrDragDestination!");
	return MRFROM2SHORT(DOR_NODROP,DO_DEFAULT);
    }


    /* Now let us have a look at the destination: is it possible
     * to add something to it? */

    VRDev * const rdev = work->pmrDragDestination->dsk;
    UCHAR const	rdtype = rdev->isArray();

    Verbose(2,modname, "DragOver, destination %#x", rdev);
    if( work->pmrDragSource->dsk == rdev )
    {
	Verbose(2,modname, "DragOver, source == destination!");
	return MRFROM2SHORT(DOR_NODROP,DO_DEFAULT);
    }


    switch( rdtype )
    {
	/* Mirroring:
	 *	Always possible to add another child.  But the child should
	 *	be large enough... */
      case RDTYPE_MIRROR:
	if( work->pmrDragSource->dsk->querySize() < rdev->querySize() )
	{
	    Verbose(2,modname, "DragOver, source too small (%lu < %lu)",work->pmrDragSource->dsk->querySize(),rdev->querySize());
	    work->pmrDragDestination = NULL;
	    return MRFROM2SHORT(DOR_NODROP,DO_DEFAULT);
	}
	break;

	/* Chaining:
	 *	Adding another children means increasing the size
	 *	of the array.  Impossible with a parent! */
      case RDTYPE_CHAIN:
	if( rdev->getParent() != NULL )
	{
	    /* There is a parent: we aren't allowed to change
	     * destinations size! */

	    Verbose(2,modname, "DragOver, bad destination");
	    work->pmrDragDestination = NULL;
	    return MRFROM2SHORT(DOR_NODROP,DO_DEFAULT);
	}
	break;

	/* Striping, PHYSDEV, nothing:
	 *	Adding a child destroys user data or creates
	 *	a new level.  Create a new array instead. */
      default:
	Verbose(2,modname, "DragOver, bad destination type");
	work->pmrDragDestination = NULL;
	return MRFROM2SHORT(DOR_NODROP,DO_DEFAULT);
    }

    Verbose(4,modname, "DragOver, destination is acceptable");
    return MRFROM2SHORT(DOR_DROP,DO_MOVE);
}




/*# ----------------------------------------------------------------------
 * DragDrop(hwndClient,work,drag)
 *
 * PARAMETER
 *	hwndClient		handle of client window, never (?) used
 *	work			window work structure
 *	drag			describes drag operation
 * RETURNS
 *	0
 *
 * DESCRIPTION
 *	Handles the dropping of a PHYSDEV or higher onto an array.
 *
 * REMARKS
 *	Since we would not receive a DM_DROP message unless a DOR_DROP reply
 *	had been given for a DM_DRAGOVER, we can assume we have successfully
 *	validated that we can accept the drop. Note that drag would have
 *	sent another DM_DRAGOVER if the operation had changed.
 *
 *	Therefore we don't have to re-validate the operation.
 *	We can go ahead and perform the operation.
 */
PRIVATE MRESULT
DragDrop(HWND hwndClient,PWORK const work,PCNRDRAGINFO drag)
{
    VRDev * const dest = ((PMYRECORD)drag->pRecord)->dsk;
    char	str[200];
    DEVID const	* id;
    ULONG	response;

    Verbose(2,modname, "DragDrop, record %#x", drag->pRecord);

    if( drag->pDragInfo->hwndSource != hwndClient )
    {
	Verbose(2,modname, "DragDrop, can't use external data");
	return 0;
    }

    id = work->pmrDragSource->dsk->queryID();


    /* All checks have been done by 'DragOver()', so just execute
     * what is requested. */

    switch( dest->isArray() )
    {
      case RDTYPE_MIRROR:
      case RDTYPE_CHAIN:
	sprintf(str, "This will destroy all data on the"
		" dropped device ID %02X%02X%02X%02X%02X%02X.\n"
		"Are you sure you want to do this?",
		(*id)[0],(*id)[1],(*id)[2],(*id)[3],(*id)[4],(*id)[5]);
	response = WinMessageBox(HWND_DESKTOP, hwndClient,
				 str, "Just curious...",
				 -1UL, MB_YESNO|MB_QUERY|MB_MOVEABLE);
	if( response == MBID_YES )
	{
	    PMINIRECORDCORE	ap[2];
	    PMYRECORD		myrcd = (PMYRECORD)drag->pRecord;

	    ((VRDrive *)dest)->addChild(work->pmrDragSource->dsk, False, 0);
	    if( dest->isArray() == RDTYPE_MIRROR )
		((VDriveWithRedundancy *)dest)->setBuild(0);


	    /* Remove new child and destination (destinations parent)
	     * from container.
	     * We need the parent to the whole tree. */

	    VRDev *	highest = dest;
	    while( highest->getParent() != 0 )
		highest = highest->getParent();
	    SearchCnr(work->hwndCnr, (PFNSRCH)LookupDisk, highest, &myrcd, NULL);
	    Verbose(2,modname, "DragDrop: found as record %lu", myrcd);

	    ap[0] = (PMINIRECORDCORE)work->pmrDragSource;
	    ap[1] = (PMINIRECORDCORE)myrcd;
	    WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
		       MPFROMP(ap), MPFROM2SHORT(2, CMA_FREE|CMA_INVALIDATE));


	    /* Post a message to the scanner thread.  This will result in
	     * disk update and reposting the device tree to our thread,
	     * now properly configured. */

	    WinPostMsg(work->hwndScanner, WUM_DEVTREE, highest, NULL);
	}
	break;

      case 0:					/* no array: PHYSDEV */
      default:
	assert( 0 );
	break;
    }


    /* Cleanup. */

    work->pmrDragSource = NULL;
    work->pmrDragDestination = NULL;
    return 0;
}






/*# ----------------------------------------------------------------------
 * DrvsetupWindowProc(hwnd,msg,mp1,mp2)
 *
 * PARAMETER
 *	hwnd,msg,mp1,mp2	see presentation manager reference
 * RETURNS
 *	MRESULT			see presentation manager reference
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Window procedure
 *
 * REMARKS
 */
PRIVATE MRESULT EXPENTRY
DrvsetupWindowProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
    static int	regular = FALSE;		/* in message loop? */
    WORK * const work = (PWORK)WinQueryWindowPtr(hwnd, QWL_USER);
    BOOL	bool;				/* usable by many messages */

    switch( msg )
    {
	/* ============================================================
	 * WUM_START:
	 *	window configuration ended, application enters
	 *	message loop. All further messages are caused
	 *	by the user.
	 */
      case WUM_START:
	Verbose(4,modname, "WUM_START");
	CreateCnr(hwnd, work);
	{
	    int			tid;
	    PTHR_DRVSCAN	dta = new THR_DRVSCAN;

	    dta->parent = hwnd;
	    tid = _beginthread(DrvscanThread, NULL, STACK_SIZE, dta);
	    if( tid == -1 )
		Verbose(1,modname, "_beginthread(DrvscanThread) failed - errno %u", errno);
	}
	regular = TRUE;
	return 0;


      case WUM_DEVTREE:
	Verbose(3,modname, "WUM_DEVTREE");
	if( work->hwndScanner == NULLHANDLE )
	{
	    HWND const	hwnd = HWNDFROMMP(mp2);

	    /* There very first time the connection between scanner and
	     * display thread are build by passing windows handles. */

	    assert( WinIsWindow(work->hab, hwnd) == TRUE ); /* has to be valid */
	    work->hwndScanner = hwnd;
	}
	DisplayDisk(work, NULL, (VRDev *)PVOIDFROMMP(mp1));
	return 0;


      case WUM_THREADENDED:
	Verbose(3,modname, "WUM_THREADENDED");
	{
	    PTHR_VERIFY	dta = (PTHR_VERIFY)PVOIDFROMMP(mp1);

	    WinPostMsg(hwnd, WUM_DEVTREE, dta->drv, NULL);
	    delete dta;
	}
	return 0;


	/* ============================================================
	 * WM_CONTROL	-	Information from container
	 *
	 */
      case WM_CONTROL:
	Verbose(4,modname, "WM_CONTROL");
	{
	    USHORT const	id = SHORT1FROMMP(mp1);
	    USHORT const	ncode = SHORT2FROMMP(mp1);


	    switch( ncode )
	    {
		/* The user requested that the menu be displayed.  Note
		 * that there's a bug in the container such that if the
		 * user uses the keyboard to do this, we don't get notified. */

	      case CN_CONTEXTMENU:
		Verbose(4,modname, "WM_CONTROL (id %#x, CN_CONTEXTMENU)", id);
		DoPopup(hwnd, work);
		break;

	      case CN_ENTER:
		Verbose(4,modname, "WM_CONTROL (id %#x, CN_ENTER)",id);
		{
		    PNOTIFYRECORDENTER	rec = (PNOTIFYRECORDENTER)PVOIDFROMMP(mp2);

		    if( rec == NULL )
			return 0;		/* dblk click on background? */

		    VRDev *	dsk = ((PMYRECORD)(rec->pRecord))->dsk;
		    if( dsk == NULL )
			MyMessageBox(0, hwnd,
				     "Internal error, can't use NULL device");
		    else if( dsk->getParent() != NULL )
			MyMessageBox(1, hwnd,
				     "Can't work on this device,"
				     " remove parent first.");
		    else if( dsk->isHostdrive()  &&  dsk->isArray() == 0 )
			MyMessageBox(1, hwnd,
				     "Can't work on this device,"
				     " create physdevice first.");
		    else if( !dsk->isWritable() )
			MyMessageBox(1, hwnd,
				     "This device (or part of it) is in use"
				     " by OS/2, remove recognized filesystems"
				     " first.");
		    else
		    {
			bool = (BOOL)WinSendMsg(rec->hwndCnr,
						CM_SETRECORDEMPHASIS,
						MPFROMP(rec->pRecord),
						MPFROM2SHORT(TRUE,CRA_INUSE));
			if( bool == FALSE )
			    Verbose(1,modname, "WinSendMsg(CM_SETRECORDEMPHASIS) failed - last error %#x)", WinGetLastError(work->hab));
		    }
		}
		return 0;


	      case CN_INITDRAG:
		Verbose(4,modname, "WM_CONTROL (id %#x, CN_INITDRAG)", id);
		return DragInit(hwnd, work,(PCNRDRAGINIT)PVOIDFROMMP(mp2));

	      case CN_DRAGOVER:
		Verbose(4,modname, "WM_CONTROL (id %#x, CN_DRAGOVER)", id);
		return DragOver(hwnd, work, (PCNRDRAGINFO)PVOIDFROMMP(mp2));

	      case CN_DRAGLEAVE:
		Verbose(4,modname, "WM_CONTROL (id %#x, CN_DRAGLEAVE)", id);
		return DragLeave(hwnd, work, (PCNRDRAGINFO)PVOIDFROMMP(mp2));

	      case CN_DROP:
		Verbose(4,modname, "WM_CONTROL (id %#x, CN_DROP)", id);
		return DragDrop(hwnd, work, (PCNRDRAGINFO)PVOIDFROMMP(mp2));

	      default:
		Verbose(4,modname, "WM_CONTROL (id %#x, notify code %#x)", id, ncode);
		break;
	    }
	}
	break;


	/* ============================================================
	 * WM_CONTEXTMENU	- create popup menu
	 */
      case WM_CONTEXTMENU:
	Verbose(4,modname, "WM_CONTEXTMENU");
	DoPopup(hwnd, work);
	break;


	/* ============================================================
	 * WM_MENUEND	- popup menu is removed
	 *	Ich weiá, auf welchem Record das Menu steht, welches Menu
	 *	es war.  Auch welcher Punkt ausgew„hlt wurde??
	 */
      case WM_MENUEND:
	Verbose(4,modname, "WM_MENUEND");
	if( work->pmrMenu == NULL )
	{
	    WinSendMsg(work->hwndCnr, CM_SETRECORDEMPHASIS,
		       MPFROMP(NULL), MPFROM2SHORT(FALSE,CRA_SOURCE));
	}
	else
	{
	    ULONG	dummy;

	    /* Remove any remaining source emphasis. */

	    bool = FALSE;
	    dummy = 0;
	    SearchCnr(work->hwndCnr, (PFNSRCH)EmphSource, &bool, &dummy, NULL);
	}
	break;


	/* ============================================================
	 * WM_COMMAND	-	command issued
	 *	Currently only virtual keys
	 */
      case WM_COMMAND:
	Verbose(4,modname, "WM_COMMAND");
	{
	    USHORT const cmd = SHORT1FROMMP(mp1);
	    ULONG	dummy;

	    switch( cmd )
	    {
		/* IDM_CREATE_PDEV:
		 *	Create a PHYSDEV (partition and PHYSDEVICE sector)
		 *	on an OS/2 disk. */
	      case IDM_CREATE_PDEV:
		Verbose(3,modname, "WM_COMMAND, IDM_CREATE_PDEV");
		{
		    PMINIRECORDCORE	ap[1] = {(PMINIRECORDCORE)work->pmrMenu};

		    /* Remove from container */

		    WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
			       MPFROMP(ap), MPFROM2SHORT(1,CMA_FREE|CMA_INVALIDATE));

		    /* Send this VRDev to Scanner Thread.  It will assume
		     * that it's a VOS2Drive so make sure it is! */

		    WinPostMsg(work->hwndScanner, WUM_MKPDEV,
			       work->pmrMenu->dsk, NULL);
		}
		return 0;


		/* IDM_COMBINE_DRV:
		 *	Combine one or more devices into a VRAID device. */
	      case IDM_COMBINE_DRV:
		Verbose(3,modname, "WM_COMMAND, IDM_COMBINE_DRV");
		{
		    DRVBUILD	info;
		    ULONG	ul;

		    /* Query container for entries "in-use". */

		    info.cnt = 0;
		    SearchCnr(work->hwndCnr, (PFNSRCH)QueryInuse,
			      &info.cnt, info.child, NULL);
		    Verbose(2,modname, "%lu drives are \"in-use\"",info.cnt);

		    /* Let user select type to create. */

		    ul = WinDlgBox(HWND_DESKTOP, hwnd, DrvtypeDlgProc,
				   NULLHANDLE, IDW_DRVTYPE, &info);
		    if( DID_OK == ul  &&  info.type != RDTYPE_RAID4 )
		    {
			/* Build new drive object, update parent/child
			 * relations and pass this tree to drvscan thread.
			 * That thread will update disk strutures
			 * and repost a message to us to display the tree. */

			VRDrive * drv;
			DEVID	newid;

			memcpy(&newid[0], &usHostId, sizeof(usHostId));
			*(PULONG)&newid[2] = time(NULL);

			switch( info.type )
			{
			  case RDTYPE_SINGLE:
			    drv = new VSingle(newid);
			    break;
			  case RDTYPE_CHAIN:
			    drv = new VChain(newid, (int)info.cnt);
			    break;
			  case RDTYPE_STRIPE:
			    drv = new VStripe(newid, (int)info.cnt);
			    break;
			  case RDTYPE_MIRROR:
			    drv = new VMirror(newid, (int)info.cnt);
			    break;
#if !defined(NORAID5)
			  case RDTYPE_RAID5:
			    drv = new VRaid5(newid, (int)info.cnt);
			    break;
#endif
			}

			/* Add children to 'drv'.  On arrays w/o redundancy all
			 * children have to be valid.  On other a build process
			 * should be forced to allow later verify runs.
			 * (otherwise it's not technically neccessary to build
			 * a RAID 1 array!) */

			for( ul = 0; ul < info.cnt; ++ul )
			{
			    PMINIRECORDCORE	ap[1]
				= {(PMINIRECORDCORE)info.child[ul]};

			    if( info.type == RDTYPE_MIRROR  &&  ul != 0 )
				drv->addChild(info.child[ul]->dsk, False, 0);
			    else
				drv->addChild(info.child[ul]->dsk, False);
			    if( !info.child[ul]->dsk->isWritable() )
				drv->forceUnwritable();

			    /* Remove from container */

			    WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
				       MPFROMP(ap),
				       MPFROM2SHORT(1, CMA_FREE|CMA_INVALIDATE));
			}
			WinPostMsg(work->hwndScanner, WUM_DEVTREE, drv, NULL);
		    }
		    else
		    {
			bool = FALSE;
			dummy = 0;
			SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);
		    }
		}
		return 0;


		/* IDM_VERIFY_DRV:
		 *	Verify redundance information. */
	      case IDM_VERIFY_DRV:
		Verbose(4,modname, "WM_COMMAND, IDM_VERIFY_DRV");
		{
		    /* Start Thread to do the verification. */

		    int		tid;
		    PTHR_VERIFY	dta = new THR_VERIFY;

		    assert( work->pmrMenu->dsk->isArray() >= RDTYPE_MIRROR );
		    dta->parent = hwnd;
		    VDriveWithRedundancy * drv
			= (VDriveWithRedundancy *)work->pmrMenu->dsk;
		    dta->drv = drv;

		    Verbose(2,modname,"Starting DrvVerifyThread()");
		    tid = _beginthread(DrvVerifyThread, NULL, STACK_SIZE, dta);
		    if( tid == -1 )
		    {
			Verbose(1,modname, "_beginthread(DrvVerifyThread) failed - errno %u", errno);
		    }
		    else
		    {
			PMINIRECORDCORE	ap[1] = {(PMINIRECORDCORE)work->pmrMenu};

			/* OK, remove from container */

			WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
				   MPFROMP(ap),
				   MPFROM2SHORT(1,CMA_FREE|CMA_INVALIDATE));
		    }
		}
		return 0;


		/* IDM_REBUILD_DRV:
		 *	Start rebuild process. */
	      case IDM_REBUILD_DRV:
		Verbose(4,modname, "WM_COMMAND, IDM_REBUILD_DRV");

		/* First: make sure, no record is selected. */

		bool = FALSE;
		dummy = 0;
		SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);

		/* Second: let the user confirm the action */

		ULONG	response;
		response = WinMessageBox(HWND_DESKTOP, hwnd,
					 "Starting a Rebuild will degrade"
					 " performance when accessing the array.\n"
					 "Are you sure you want to start a Rebuild?",
					 "Just curious...",
					 -1UL, MB_YESNO|MB_QUERY|MB_MOVEABLE);
		if( response == MBID_YES )
		{
		    PMINIRECORDCORE	ap[1] = {(PMINIRECORDCORE)work->pmrMenu};

		    /* Remove from container */

		    WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
			       MPFROMP(ap),
			       MPFROM2SHORT(1,CMA_FREE|CMA_INVALIDATE));

		    /* Post to scanner thread to let it do the setup. */

		    VRDrive * drv = (VRDrive *)work->pmrMenu->dsk;
		    assert( drv->isArray() >= RDTYPE_MIRROR );
		    Verbose(0,modname, "About to post WUM_REBUILD_DRV to %u",
			    drv->queryFltHd());
		    bool = WinPostMsg(work->hwndScanner, WUM_REBUILD_DRV, drv, 0);
		    if( FALSE == bool )
			Verbose(0,modname, "WinPostMsg(WUM_REBUILD_DRV,) failed");
		}
		return 0;


		/* IDM_DESTROY_DRV:
		 *	Destroy a VRAID drive.  All parameters are OK, just
		 *	verify with the user and execute! */
	      case IDM_DESTROY_DRV:
		Verbose(3,modname, "WM_COMMAND, IDM_DESTROY_DRV");

		/* First: make sure, no record is selected. */

		bool = FALSE;
		dummy = 0;
		SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);

		/* Second: ask user and destroy. */

	        {
		    VRDrive *	drv = (VRDrive *)work->pmrMenu->dsk; /* !!! */
		    ULONG	response;

		    response = WinMessageBox(HWND_DESKTOP, hwnd,
					     "Are you sure?", "Just curious...",
					     -1UL, MB_YESNO|MB_QUERY|MB_MOVEABLE);
		    if( response == MBID_YES )
		    {
			PMINIRECORDCORE	ap[1] = {(PMINIRECORDCORE)work->pmrMenu};

			/* Remove from container */

			WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
				    MPFROMP(ap),
				    MPFROM2SHORT(1,CMA_FREE|CMA_INVALIDATE));

			/* Remove from disk.  Will post WUM_DEVTREE for all
			 * children that exist. */

			WinPostMsg(work->hwndScanner, WUM_DELDRV, drv, NULL);
		    }
		}
		return 0;


		/* IDM_DELETE_PDEV:
		 *	Remove PHYSDEVICE sector (and partition) from a disk. */
	      case IDM_DELETE_PDEV:
		Verbose(3,modname, "WM_COMMAND, IDM_DELETE_PDEV");

		/* First: make sure, no record is selected. */

		bool = FALSE;
		dummy = 0;
		SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);
		
		/* Second: ask user and destroy. */

	        {
		    VRDev *	rdev = work->pmrMenu->dsk;
		    DEVID const * id = rdev->queryID();
		    char	str[200];

		    sprintf(str,"Are you sure you want to destroy"
			    " ID %02X%02X%02X%02X%02X%02X?",
			    (*id)[0],(*id)[1],(*id)[2],(*id)[3],(*id)[4],(*id)[5]);

		    ULONG	response;
		    response = WinMessageBox(HWND_DESKTOP, hwnd,
					     str, "Just curious...",
					     -1UL, MB_YESNO|MB_QUERY|MB_MOVEABLE);
		    if( response == MBID_YES )
		    {
			PMINIRECORDCORE	ap[1] = {(PMINIRECORDCORE)work->pmrMenu};

			/* Remove from container */

			WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
				    MPFROMP(ap),
				    MPFROM2SHORT(1,CMA_FREE|CMA_INVALIDATE));

			WinPostMsg(work->hwndScanner, WUM_DELPDEV, rdev, NULL);
		    }
		}
		return 0;


		/* IDM_REMOVE_CHILD:
		 *	Remove part of array. */
	      case IDM_REMOVE_CHILD:
		Verbose(3,modname, "WM_COMMAND, IDM_REMOVE_CHILD");

		/* First: make sure, no record is selected. */

		bool = FALSE;
		dummy = 0;
		SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);

		/* Second: ask user and destroy. */

	        {
		    VRDev *	rdev = work->pmrMenu->dsk;
		    DEVID const * id = rdev->queryID();
		    char	str[200];

		    sprintf(str,"Are you sure you want to remove"
			    " ID %02X%02X%02X%02X%02X%02X from array?",
			    (*id)[0],(*id)[1],(*id)[2],(*id)[3],(*id)[4],(*id)[5]);

		    ULONG	response;
		    response = WinMessageBox(HWND_DESKTOP, hwnd,
					     str, "Just curious...",
					     -1UL, MB_YESNO|MB_QUERY|MB_MOVEABLE);
		    if( response == MBID_YES )
		    {
			PMINIRECORDCORE	ap[1];
			PMYRECORD	myrcd;
			VRDrive *	parent = rdev->getParent();

			/* Remove complete tree from container. */

			VRDev *	highest = parent;
			while( highest->getParent() != 0 )
			    highest = highest->getParent();
			SearchCnr(work->hwndCnr, (PFNSRCH)LookupDisk,
				  highest, &myrcd, NULL);
			ap[0] = (PMINIRECORDCORE)myrcd;

			WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
				   MPFROMP(ap),
				   MPFROM2SHORT(1,CMA_FREE|CMA_INVALIDATE));

			/* Post message to drive scanner.  It will remove
			 * the child and post two WUM_DEVTREE back to us. */

			WinPostMsg(work->hwndScanner, WUM_REMCHILD, rdev, parent);
		    }
		}
		return 0;


		/* IDM_INVALID_CHILD:
		 *	Declare part of an array as containing invalid data. */
	      case IDM_INVALID_CHILD:
		Verbose(3,modname, "WM_COMMAND, IDM_INVALID_CHILD");

		/* First: make sure, no record is selected. */

		bool = FALSE;
		dummy = 0;
		SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);

		/* Second: ask user and pass work to scanner. */

	        {
		    VRDev *	rdev = work->pmrMenu->dsk;
		    DEVID const * id = rdev->queryID();
		    char	str[200];

		    sprintf(str,"Are you sure you want to declare"
			    " ID %02X%02X%02X%02X%02X%02X as invalid?"
			    "\nArray rebuild will start as soon as you"
			    " leave VSetup.",
			    (*id)[0],(*id)[1],(*id)[2],(*id)[3],(*id)[4],(*id)[5]);

		    ULONG	response;
		    response = WinMessageBox(HWND_DESKTOP, hwnd,
					     str, "Just curious...",
					     -1UL, MB_YESNO|MB_QUERY|MB_MOVEABLE);
		    if( response == MBID_YES )
		    {
			PMINIRECORDCORE	ap[1];
			PMYRECORD	myrcd;
			VRDrive *	parent = rdev->getParent();

			/* Remove complete tree from container. */

			VRDev *	highest = parent;
			while( highest->getParent() != 0 )
			    highest = highest->getParent();
			SearchCnr(work->hwndCnr, (PFNSRCH)LookupDisk,
				  highest, &myrcd, NULL);
			ap[0] = (PMINIRECORDCORE)myrcd;

			WinSendMsg(work->hwndCnr, CM_REMOVERECORD,
				   MPFROMP(ap),
				   MPFROM2SHORT(1,CMA_FREE|CMA_INVALIDATE));

			/* Post message to drive scanner.  It will remove
			 * the child and post two WUM_DEVTREE back to us. */

			WinPostMsg(work->hwndScanner, WUM_INVCHILD, rdev, parent);
		    }
		}
		return 0;



		/* IDM_REMOVE_SEL:
		 *	Remove selected state from all icons. */
	      case IDM_REMOVE_SEL:
		bool = FALSE;
		dummy = 0;
		SearchCnr(work->hwndCnr, (PFNSRCH)EmphInuse, &bool, &dummy, NULL);
		return 0;


	      default:
		Verbose(2,modname, "WM_COMMAND %u", cmd);
		break;				/* ignore */
	    }
	}
	break;					/* continue with default */


	/* ============================================================
	 * WM_CREATE	-	window created
	 *	Window created but not yet visible
	 */
      case WM_CREATE:
	Verbose(4,modname, "WM_CREATE");
	LoadPresParam(WC_DRVSETUP);
	break;					/* keep on going... */


	/* ============================================================
	 * WM_CLOSE	-	this window closed
	 *	Stop all further processing, cleanup.	
	 */
      case WM_CLOSE:
	Verbose(4,modname, "WM_CLOSE");
	{
	    DRVBUILD	info;
	    ULONG	ul;

	    /* Query container for all entries "in-use". */

	    info.cnt = 0;
	    SearchCnr(work->hwndCnr, (PFNSRCH)QueryAny,
		      &info.cnt, info.child, NULL);
	    Verbose(2,modname, "%lu drives are available", info.cnt);
	    for( ul = 0; ul < info.cnt; ++ul )
	    {
		WinPostMsg(work->hwndScanner, WUM_CLOSEDEV,
			   info.child[ul]->dsk, NULL);
	    }
	}
	WinPostMsg(work->hwndScanner, WM_CLOSE, 0, 0);
	break;					/* continue... */


	/* ============================================================
	 * WM_DESTROY	-	this window destroyed
	 *	This message may be posted after WM_CLOSE or without
	 *	any WM_CLOSE (if user close per windowlist).
	 *	So do here the same as in WM_CLOSE (if possible).
	 */
      case WM_DESTROY:
	Verbose(4,modname, "WM_DESTROY");
	{
	    WinDestroyWindow(work->hwndSngPopup);
	    WinDestroyWindow(work->hwndMulPopup);
	    WinDestroyWindow(work->hwndCnr);
	    WinPostMsg(work->self->parent, WUM_THREADENDED, work->self, 0);
	}
	break;


	/* ============================================================
	 * WM_SIZE	-	client area resized
	 *	All child windows of client area have to be
	 *	resized, too.
	 *	Don't issue this message before WUM_START has been processed!
	 */
      case WM_SIZE:
	Verbose(4,modname, "WM_SIZE");
	{
	    SWP		size;

	    if( WinQueryWindowPos(WinQueryWindow(hwnd, QW_PARENT), &size) == TRUE )
		ProfileWrite(WC_DRVSETUP, "Size", sizeof(size), &size);

	    if( WinQueryWindowPos(hwnd, &size) == TRUE )
		WinSetWindowPos(work->hwndCnr, HWND_TOP,
				0, 0,
				SHORT1FROMMP(mp2), SHORT2FROMMP(mp2),
				SWP_SIZE|SWP_MOVE|SWP_SHOW|SWP_ACTIVATE);
	}
	break;


	/* ============================================================
	 * WM_MOVE:
	 *	Parent window has moved, record new position in profile.
	 */
      case WM_MOVE:
	Verbose(4,modname, "WM_MOVE");
	{
	    SWP	swp;

	    if( WinQueryWindowPos(WinQueryWindow(hwnd, QW_PARENT), &swp) == TRUE )
		ProfileWrite(WC_DRVSETUP, "Position", sizeof(swp), &swp);
	}
	break;


	/* ============================================================
	 * WM_QUERYTRACKINFO	-	restrict size of window
	 *	I never saw this message.
	 */
      case WM_QUERYTRACKINFO:
	{
	    USHORT const tf = SHORT1FROMMP(mp1);

	    Verbose(1,modname, "WM_QUERYTRACKINFO (tracking flags %#x)",tf);
	}
	break;				/* has to do default processing! */


	/* ============================================================
	 * WM_PRESPARAMCHANGED:
	 *	Notifies the program that a certain presentation
	 *	parameter was changed.
	 */
      case WM_PRESPARAMCHANGED:
	{
	    char	fontname[256];
	    ULONG	cb;

	    Verbose(4,modname, "WM_PRESPARAMCHANGED(type %#x)", LONGFROMMP(mp1));
	    if( work == NULL )
		break;

	    switch( LONGFROMMP(mp1) )
	    {
	      case PP_FOREGROUNDCOLOR:
		cb = WinQueryPresParam(hwnd, PP_FOREGROUNDCOLOR, 0, NULL,
				       sizeof(rgbForeground), &rgbForeground,
				       QPF_NOINHERIT);
		if( cb == 0 )
		    Verbose(1,modname, "WinQueryPresParam failed - last error %#x", WinGetLastError(work->hab));
		else
		{
		    Verbose(2,modname, "Set foreground colour to %#x", rgbForeground);
		    ProfileWrite(WC_DRVSETUP, "Foreground", sizeof(rgbForeground), &rgbForeground);
		    WinInvalidateRect(hwnd, NULL, FALSE);
		}
		break;

	      case PP_BACKGROUNDCOLOR:
		cb = WinQueryPresParam(hwnd, PP_BACKGROUNDCOLOR, 0, NULL,
				       sizeof(rgbBackground), &rgbBackground,
				       QPF_NOINHERIT);
		if( cb == 0 )
		    Verbose(1,modname, "WinQueryPresParam failed - last error %#x", WinGetLastError(work->hab));
		else
		{
		    Verbose(2,modname, "Set background colour to %#x", rgbBackground);
		    ProfileWrite(WC_DRVSETUP, "Background", sizeof(rgbBackground), &rgbBackground);
		    WinInvalidateRect(hwnd, NULL, FALSE);
		}
		break;
		    
	      case PP_FONTNAMESIZE:
		cb = WinQueryPresParam(hwnd, PP_FONTNAMESIZE, 0, NULL,
				       sizeof(fontname), fontname,
				       QPF_NOINHERIT);
		if( cb == 0 )
		    Verbose(1,modname, "WinQueryPresParam failed - last error %#x", WinGetLastError(work->hab));
		else
		{
		    Verbose(2,modname, "Set font to \"%s\"", fontname);
		    ProfileWrite(WC_DRVSETUP, "Font", strlen(fontname), &fontname);
		    WinInvalidateRect(hwnd, NULL, FALSE);
		}
		break;

	      default:
		Verbose(1,modname, "WM_PRESPARAMCHANGED(unsupported type %#x)", LONGFROMMP(mp1));
		break;
	    }
	} /* end[case(WM_PRESPARAMCHANGED)] */
      break;


      /* ============================================================
       * WM_ERASEBACKGROUND:
       *	Should system erase?
       */
      case WM_ERASEBACKGROUND:
	return (MRESULT)TRUE;


      default:
	break;
    } /* end[switch] */

    return WinDefWindowProc(hwnd, msg, mp1, mp2);
}






/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * DrvsetupThread(dummy)
 *
 * PARAMETER
 *	dummy		pointer to information from parent
 * RETURNS
 *	none
 * GLOBAL
 *	none
 * DESPRIPTION
 *	Runs as a seperate thread.
 *
 * REMARK
 */
PUBLIC void
DrvsetupThread(void *dummy)
{
    PTHR_DRVSETUP const dta = (PTHR_DRVSETUP)dummy;
    HMQ		hmq;
    HWND	hwndFrame, hwndClient;
    ULONG	flCreate;
    QMSG	qmsg;
    BOOL	bool;
    PWORK	work;

    Verbose(2,modname, "startup");
    do
    {
	work = new WORK;
	memset(work, 0, sizeof(WORK));
	work->self = dta;

	if( !(work->hab = WinInitialize(0)) )
	{
	    Verbose(0,modname, "WinInitialize failed!?!");
	    break;
	}

	if( !(hmq = WinCreateMsgQueue(work->hab, 0)) )
	{
	    Verbose(0,modname, "WinCreateMsgQueue - error %#x", WinGetLastError(work->hab));
	    break;
	}
	WinCancelShutdown(hmq, TRUE);		/* don't want to handle it */

	if( !WinRegisterClass(work->hab, WC_DRVSETUP, DrvsetupWindowProc,
			      CS_MOVENOTIFY, sizeof(PVOID)) )
	{
	    Verbose(0,modname, "WinRegisterClass - error %#x", WinGetLastError(work->hab));
	    break;
	}

	flCreate = FCF_SIZEBORDER | FCF_TASKLIST | FCF_SYSMENU | FCF_TITLEBAR
	    | FCF_ICON | FCF_MINMAX;

	hwndFrame = WinCreateStdWindow(HWND_DESKTOP, WS_ANIMATE, &flCreate,
				       WC_DRVSETUP,
				       "VSetup - Drive Definition Window",
				       0, 0,
				       IDW_DRVSETUP, &hwndClient);

	if( hwndFrame == 0  ||  hwndClient == 0 )
	{
	    Verbose(0,modname, "WinCreateStdWindow - error %#x", WinGetLastError(work->hab));
	    break;				/* couldn't create window for some
						   reason. */
	}
	PPSubclassWindow(WinWindowFromID(hwndFrame, FID_MENU), WC_DRVSETUP);
	PPSubclassWindow(WinWindowFromID(hwndFrame, FID_TITLEBAR), WC_DRVSETUP);
	PPSubclassWindow(WinWindowFromID(hwndFrame, FID_SYSMENU), WC_DRVSETUP);

	/* Configure element of window or add more elements to window. */

	bool = WinSetWindowPtr(hwndClient, QWL_USER, work);
	if( bool == FALSE )
	{
	    MyMessageBox(0, HWND_DESKTOP,
			 "WinSetWindowPtr failed - error %#",
			 WinGetLastError(work->hab));
	    break;
	}

	WinPostMsg(hwndClient, WUM_START, 0, 0);

	/* All done, show window now (make visible)
	 * and start processing messages. */
	{
	    SWP		swp, tmp;
	    unsigned	cb;
	    BOOL	bool;

	    /* Change our window size and position
	     * to recommanded or saved values. */

	    WinQueryTaskSizePos(work->hab, 0, &swp);

	    cb = sizeof(tmp);
	    if( ProfileRead(WC_DRVSETUP, "Size", &cb, &tmp) == 0 )
	    {
		Verbose(4,modname, "Copying old size of window");
		swp.cx = tmp.cx;
		swp.cy = tmp.cy;
		swp.fl |= SWP_SIZE;
	    }
	    cb = sizeof(tmp);
	    if( ProfileRead(WC_DRVSETUP, "Position", &cb, &tmp) == 0 )
	    {
		Verbose(4,modname, "Copying old position of window");
		swp.x = tmp.x;
		swp.y = tmp.y;
		swp.fl |= SWP_MOVE;
	    }

	    swp.fl |= SWP_SHOW;
	    bool = WinSetWindowPos(hwndFrame, HWND_TOP,
				   swp.x, swp.y, swp.cx, swp.cy, swp.fl);
	    if( bool == FALSE )
		Verbose(1,modname, "WinSetWindowPos failed (last error %#x)", WinGetLastError(work->hab));
	}

	/* Message processing loop. */

	while( WinGetMsg(work->hab, &qmsg, 0L, 0, 0) )
	    WinDispatchMsg(work->hab, &qmsg);

	/* Message processing ended (WM_QUIT sent), clean up. */

	WinDestroyWindow(hwndFrame);
	WinDestroyMsgQueue(hmq);
	WinTerminate(work->hab);
	delete work;
    }
    while(0);

    Verbose(2,modname, "DrvSetupThread: stopped");
    return;
}
