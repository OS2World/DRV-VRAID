/*
 * $Source: R:/Source/driver/raid/Setup/RCS/drverify.h,v $
 * $Revision: 1.2 $
 * $Date: 2000/08/17 02:07:44 $
 * $Locker:  $
 *
 *	Interface to drverify.cpp
 *
 * $Log: drverify.h,v $
 * Revision 1.2  2000/08/17 02:07:44  vitus
 * - removed flthd from parameter structure of DrvVerifyThread().  This
 *   info is already in VDriveWithRedundancy *.
 *
 * Revision 1.1  2000/06/06 01:06:39  vitus
 * Initial revision
 * ----------------------------------------
 * This code is Copyright Vitus Jensen 2000
 */


typedef struct {
    HWND		parent;			/* post devices here */
    VDriveWithRedundancy * drv;
} THR_VERIFY, * PTHR_VERIFY;			/* parameter to DrvVerifyThread*/



extern void _Optlink	DrvVerifyThread(void *dummy);
