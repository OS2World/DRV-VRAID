/*
 * $Source: R:/Source/driver/raid/Setup/RCS/resources.h,v $
 * $Revision: 1.13 $
 * $Date: 2001/01/15 01:31:48 $
 * $Locker:  $
 *
 *	List of resource identifiers used.
 *
 * $Log: resources.h,v $
 * Revision 1.13  2001/01/15 01:31:48  vitus
 * - added IDM_INVALID_CHILD
 *
 * Revision 1.12  2000/08/05 02:55:28  vitus
 * - added IDs for Array Verification Progress
 *
 * Revision 1.11  2000/04/10 01:24:03  vitus
 * - added DID_HELP to give Help buttons an ID as long as they aren't used
 *   to display a help panel.  This way help buttons may be hidden.
 *
 * Revision 1.10  2000/04/10 01:06:23  vitus
 * - added ST_AREASIZE
 *
 * Revision 1.9  1999/06/19 01:35:50  vitus
 * - added IDM_NOBEEP!  Liesbeth told me to stop the beeping...
 *
 * Revision 1.8  1999/06/02 01:48:15  vitus
 * - added  ICO_INVCHILDPDEV, ICO_INVCHILDARRAY
 *
 * Revision 1.7  1999/05/13 22:48:43  vitus
 * - added IDM_REMOVE_CHILD
 *
 * Revision 1.6  1998/09/27 00:12:49  vitus
 * - added ICO_DISABLEDPDEV, ICO_DISABLEDARRAY
 *
 * Revision 1.5  1998/09/20 20:42:40  vitus
 * - added IDM_DRIVER submenu
 *
 * Revision 1.4  1998/06/09 01:33:20  vitus
 * - added disk area dialog components
 *
 * Revision 1.3  1998/05/24 02:21:02  vitus
 * - added drive type dialog
 * - more icons
 *
 * Revision 1.2  1998/05/17 16:27:28  vitus
 * - added popup menues
 *
 * Revision 1.1  1998/05/06 22:58:38  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2001
 */

/* Other IDs */

#define DID_HELP	9			/* Help Button (if unused) */
#define	DID_START	10
#define IDW_LOGBOOK	98


/* Main window and menu */

#define IDW_MAIN	100

#define IDM_FILE	110
#define IDM_SAVE	111
#define IDM_SAVEAS	112
#define IDM_PRINT	114

#define IDM_EDIT	120
#define IDM_CUT		121
#define IDM_COPY	122
#define IDM_PASTE	123
#define IDM_CLEAR	124

#define IDM_DRIVER	130
#define IDM_VERSION	131
#define IDM_MSGDUMP	132
#define IDM_NOBEEP	133

#define IDM_DRIVES	140
#define IDM_DRVSETUP	141
#define IDM_DRVDUMP	142

#define IDM_HELP	170
#define IDM_HELPINDEX	171
#define IDM_GENERALHELP	172
#define IDM_USINGHELP	173
#define IDM_KEYSHELP	174
#define IDM_ABOUT	175


/* Drive setup window and popup menues */

#define IDW_DRVSETUP	400
#define ICO_LOCKEDDRIVE	401
#define ICO_FREEDRIVE	402
#define ICO_PDEVDRIVE	403
#define ICO_RAIDDRIVE	404
#define ICO_MISSINGDRIVE 405
#define ICO_DISABLEDPDEV 406
#define ICO_DISABLEDARRAY 407
#define ICO_INVCHILDPDEV 408
#define ICO_INVCHILDARRAY 409

#define IDW_DRVCN	420

#define IDPM_SNGDRV	430
#define IDPM_MULDRV	431
#define IDPM_NODRV	432
#define IDM_CREATE_PDEV	440
#define IDM_DELETE_PDEV	441
#define IDM_DESTROY_DRV	442
#define IDM_REMOVE_CHILD 443
#define IDM_INVALID_CHILD 444
#define IDM_REMOVE_SEL	445
#define IDM_COMBINE_DRV	446
#define IDM_NOTHING	447
#define IDM_VERIFY_DRV	448
#define IDM_REBUILD_DRV	449


/* Some smaller windows */

#define IDW_ABOUT	500
#define ST_DESCRIPTION	501

#define IDW_DRVDUMP	520
#define EF_DRVINDEX	521

#define IDW_INILOCATION	600
#define EF_INILOCATION	601


/* Drive type selection dialog */

#define IDW_DRVTYPE	700
#define CB_DRVTYPE	702
#define SC_DRVSIZE	703

/* disk area selection */

#define IDW_DSKAREA	720
#define LB_AREASIZE	721
#define SL_AREAPERCENT	722
#define ST_AREASIZE	723

/* Array Verification Progress */

#define IDW_PROGRESS	740
#define SL_PROGPERCENT	742
#define PB_PAUSERESUME	743
