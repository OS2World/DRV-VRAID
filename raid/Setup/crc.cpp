/*
 * $Source: R:/Source/driver/raid/Setup/RCS/crc.cpp,v $
 * $Revision: 1.5 $
 * $Date: 2000/04/10 01:00:56 $
 * $Locker:  $
 *
 *	Support routines to calculate CRC.
 *
 * $Log: crc.cpp,v $
 * Revision 1.5  2000/04/10 01:00:56  vitus
 * - include VSetup.h instead of proto.h
 *
 * Revision 1.4  1999/06/20 17:04:00  vitus
 * - minor corrections to keep compiler happy
 *
 * Revision 1.3  1999/02/09 02:47:11  vitus
 * - Umstellung auf C++
 *
 * Revision 1.2  1998/12/13 23:06:15  vitus
 * - changed as global headers moved to /source/Lib
 *
 * Revision 1.1  1998/03/16 23:37:50  vitus
 * Initial revision
 * ---------------------------------------------
 * This code is Copyright Vitus Jensen 1998-2000
 */
static char const vcid[]="$Id: crc.cpp,v 1.5 2000/04/10 01:00:56 vitus Exp $";

#define __IBMC__	0
#define INCL_DOS
#define INCL_DOSDEVIOCTL
#define INCL_DOSERRORS
#include <os2.h>

#include "../../../Lib/defines.h"
#include "VSetup.h"



PRIVATE USHORT	CRCtable[256];




/* calculate CRC table entry */

PRIVATE USHORT
CalcTable(USHORT data,USHORT genpoly,USHORT accum)
{
    int i;

    for( i = 0; i < 8; ++i )
    {
	if( (accum^data) & 1 )
	{
	    accum >>=1;
	    accum ^= genpoly;
	}
	else
	    accum >>= 1;

	data >>= 1;
    }
    return accum;
}




/*# ----------------------------------------------------------------------
 * UpdateCRC(crc,byte);
 *
 * PARAMETER
 *	crc		bisherige CRC-Summe
 *	byte		Datenbyte zur Aktualisierung der CRC-Summe
 * RETURNS
 *	aktualisierte CRC-Summe
 * GLOBAL
 * DESCRIPTION
 *	CRC-Summe aktualisieren.
 *
 * REMARKS
 *	Das Generatorpolynom kann in CRC.H geaendert werden. Die CRC-
 *	Erzeugung benutzt Linksverschiebungen (LSB first) wie es bei
 *	Einsatz einer SIO erforderlich ist.
 *	InitCRC() muss zum Aufbau der Tabelle vorher aufgerufen sein.
 */
PRIVATE USHORT
UpdateCRC(USHORT crc,UCHAR byte)
{
    /* bei MSB first: */
    /* return( (crc << 8) ^ CRCtable[ (crc >> 8) ^ byte ] ); */

    /* bei LSB first: */
    return (USHORT)((crc >> 8) ^ CRCtable[ (crc ^ byte) & 0xFF ]);
}




/*# ----------------------------------------------------------------------
 * CrcInit(uPOLYNOM)
 *
 * PARAMETER
 *	uPOLYNOM		Generator-Polynom
 *		SDLC:	0x8408 (entspricht x^16 + x^12 + x^5 + x^0)
 *		CRC-16:	0xA001 (entspricht x^16 + x^15 + x^2 + x^0)
 * RETURNS
 * GLOBAL
 * DESCRIPTION
 *	CRC-Tabelle aufbauen.
 *
 * REMARKS
 *	Muss vor UpdateCRC() aufgerufen werden.
 *	Die Konstanten fr die Generator-Polynome erh„lt man, indem man
 *	aus den Koeffizienten des Polynoms einen 16-Bit-Wert macht,
 *	(x^16 dabei weglassen) und die Bitreihenfolge spiegelt (bei
 *	einer Sio: LSB first).
 *	Also z.B. fuer SDLC (x^16 + x^12 + x^5 + x^0):
 *	a) Koeffizienten:	1 0001 0000 0010 0001
 *	b) x^16 weglassen:	  0001 0000 0010 0001
 *	c) MSB/LSB tauschen:	  1000 0100 0000 1000 -> 0x8408 
 *	analog fuer CRC-16 (x^16 + x^15 + x^2 + x^0): -> 0xA001 
 */
PUBLIC void
CrcInit(USHORT uPOLYNOM)
{
    USHORT i;

    for( i = 0; i < 256u; ++i )
	CRCtable[i] = CalcTable(i, uPOLYNOM, 0);
}




/*# ----------------------------------------------------------------------
 * Crc16(buffer,cb)
 *
 * PARAMETER
 *	buffer
 *	cb
 *
 * RETURNS
 *	CRC 16
 *
 * DESCRIPTION
 *	Calculates CRC16 over 'cb' bytes starting at 'buf'.
 *
 * REMARKS
 */
PUBLIC USHORT
Crc16(PVOID buffer,USHORT cb)
{
    USHORT	crc = 0;
    PUCHAR	p = (PUCHAR)buffer;

    for(; cb != 0; --cb, ++p )
	crc = UpdateCRC(crc, *p);
    return crc;
}
