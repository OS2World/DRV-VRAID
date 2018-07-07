/*
 * $Source: R:/Source/driver/raid/Setup/RCS/drvsetup.h,v $
 * $Revision: 1.2 $
 * $Date: 1999/02/09 02:55:10 $
 * $Author: vitus $
 *
 *	Interface to DrvSetup.c
 *
 * $Log: drvsetup.h,v $
 * Revision 1.2  1999/02/09 02:55:10  vitus
 * - switched to C++
 *
 * Revision 1.1  1998/05/08 00:54:42  vitus
 * Initial revision
 * -------------------------------------------
 * This code is Copyright Vitus Jensen 1998-99
 */


typedef struct {
    HWND	parent;
} THR_DRVSETUP, * PTHR_DRVSETUP;



extern void _Optlink	DrvsetupThread(void *dummy);
