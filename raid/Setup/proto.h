/*
 * $Source: R:/source/driver/raid/setup/RCS\\proto.h,v $
 * $Revision: 1.2 $
 * $Date: 1998/06/27 00:52:25 $
 * $Author: vitus $
 *
 * Function prototyping for support routines.
 *
 * $Log: proto.h,v $
 * Revision 1.2  1998/06/27 00:52:25  vitus
 * - removed PDsk* Routines, moved to /source/tools
 *
 * Revision 1.1  1998/03/14 14:08:27  vitus
 * Initial revision
 * ----------------------------------------
 * This code is Copyright Vitus Jensen 1998
 */

extern APIRET	QueryHostId(PUSHORT const id);

extern void	CrcInit(USHORT polynom);
extern USHORT	Crc16(PVOID buffer,USHORT cb);

