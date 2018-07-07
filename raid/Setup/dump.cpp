/*
 * $Source: r:/source/driver/raid/setup/RCS/dump.cpp,v $
 * $Revision: 1.7 $
 * $Date: 2005/06/17 06:01:08 $
 * $Locker:  $
 *
 *      Dialog box to select which drive to dump and dump thread.
 *
 * $Log: dump.cpp,v $
 * Revision 1.7  2005/06/17 06:01:08  vitus
 * - DumpPartitions: corrected display of drive size (hopefully)
 * - added New BSD license statement
 * - removed tabs
 *
 * Revision 1.6  2003/09/21 00:56:53  vitus
 * - DumpPartitions: fixed disk size calculation
 *
 * Revision 1.5  1999/03/25 00:30:45  vitus
 * - dumping calls PDskClose() whatever happens
 * - dumping stops at sectors w/o PHYSDEVICE/VRAIDDEVICE mark
 * - remaining printf's converted to Verbose(0,...)
 *
 * Revision 1.4  1999/02/09 02:51:05  vitus
 * - switched to C++
 *
 * Revision 1.3  1998/12/13 23:08:41  vitus
 * - changed as global headers moved to /source/Lib
 *
 * Revision 1.2  1998/09/20 20:39:05  vitus
 * - moved physdsk.h to ../../tools directory
 *
 * Revision 1.1  1998/04/29 02:27:59  vitus
 * Initial revision
 * -------------------------------------------
 *      Copyright (c) 1998-2001,2005, Vitus Jensen
 *      All rights reserved.
 *
 *      Redistribution and use in source and binary forms, with or without
 *      modification, are permitted provided that the following conditions are
 *      met:
 *
 *          * Redistributions of source code must retain the above copyright
 *            notice, this list of conditions and the following disclaimer.
 *          * Redistributions in binary form must reproduce the above copyright
 *            notice, this list of conditions and the following disclaimer in
 *            the documentation and/or other materials provided with the
 *            distribution.
 *          * Neither the name of Vitus Jensen nor the names of its
 *            contributors may be used to endorse or promote products derived
 *            from this software without specific prior written permission.
 *
 *      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *      "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *      LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *      A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *      OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *      SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *      LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *      DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *      THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *      (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *      OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
static char const vcid[]="$Id: dump.cpp,v 1.7 2005/06/17 06:01:08 vitus Exp $";

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define __IBMC__        0
#define INCL_DOS
#define INCL_DOSDEVIOCTL
#define INCL_ERRORS
#define INCL_WIN
#include <os2.h>

extern "C" {
#include "../../../Lib/defines.h"
#include "../../../Lib/verbose.h"
#include "../../../Lib/physdsk.h"
}
#include "resources.h"
#include "vsetup.h"
#include "../dsklayout.h"





/*# ----------------------------------------------------------------------
 * CALL
 * PARAMETER
 * RETURNS
 * DESCRIPTION
 * REMARKS
 */
PRIVATE void
DumpBuffer(char * output,void const * buffer,USHORT const cb)
{
    UCHAR       ascii[17];
    UCHAR const * p = (UCHAR *)buffer;
    USHORT      i;
    int         oi = 0;

    for( i = 0; i < cb; ++i, ++p )
    {
        if( (i % 16) == 0 )
            oi += sprintf(&output[oi], "%04X: ", i);
        oi += sprintf(&output[oi], "%02X", *p);
        ascii[i%16] = (isprint(*p) ? *p : '.');

        if( (i+1) % 16 == 0 )
        {
            ascii[16] = '\0';
            oi += sprintf(&output[oi], "\t%s\n", ascii);
        }
        else if( (i+1) % 8 == 0 )
            oi += sprintf(&output[oi], "-");
        else
            oi += sprintf(&output[oi], " ");
    }
    return;
}




/*# ----------------------------------------------------------------------
 * DumpPartitions(which)
 *
 * PARAMETER
 *      which           drive to dump or -1 for all drives
 *
 * RETURNS
 *      (nothing)
 *
 * DESCRIPTION
 *      Dumps all interesting sectors on one or all disks: MBR, PHYSDEV and
 *      all VRDEV sectors.
 *
 * REMARKS
 */
PRIVATE void _Optlink
DumpPartitions(PVOID dummy)
{
    ULONG       which = (ULONG)dummy;
    UCHAR       buffer[512];
    char        * output = new char[33*80];
    APIRET      rc;
    ULONG       devidx, devcnt;
    int         i;


    rc = PDskEnum(&devcnt);
    if( rc )
    {
        Verbose(0, "Dump", "PDskEnum - rc %lu", rc);
        delete[] output;
        return;
    }
    Verbose(0, "Dump", "Physical disks: %lu", devcnt);

    for( devidx = 0; devidx < devcnt; ++devidx )
    {
        ULONG                   hd;
        DEVICEPARAMETERBLOCK    dp;
        USHORT                  dataoffset;
        ULONG                   partoffset = 0;

        if( which != (ULONG)-1  &&  devidx != which )
            continue;                           /* ignore this one */


        rc = PDskOpen(devidx, &hd);
        if( rc != 0 )
        {
            Verbose(0, "Dump", "PDskOpen(%lu) - rc %lu", devidx, rc);
            continue;
        }

        do
        {
            Verbose(0,"Dump", "======== Disk %lu, handle %#lx ========", devidx, hd);

            rc = PDskQueryParam(hd, &dp);
            if( rc != 0 )
            {
                Verbose(0, "Dump", "PDskQueryParam - rc %lu", rc);
                break;
            }
            {
                ULONG size = (ULONG)dp.cSectorsPerTrack * (ULONG)dp.cHeads / 2ul;
                size /= 1024ul;
                size *= (ULONG)dp.cCylinders;

                Verbose(0,"Dump",
                        "Physical drive parameters: Cylinders: %d,"
                        " Heads: %d,"
                        " Sectors/Track: %d",
                        dp.cCylinders, dp.cHeads, dp.cSectorsPerTrack);
                Verbose(0, "Dump", "Drive capacity: %lu MBytes", size);
            }

            rc = PDskRead(hd, 0, 1, buffer);
            if( rc != 0 )
            {
                Verbose(0, "Dump", "PDskRead - rc %lu", rc);
                break;
            }

            DumpBuffer(output, buffer, 512);
            Verbose(0, "Dump", "-------- Dump of real partition sector --------\n%s",
                    output);

            {
                PMBR    sec = (PMBR)buffer;
                int     i;

                for( i = 0; i < 4; ++i )
                {
                    if( sec->PartitionTable[i].SysIndicator == VRAID_PARTTYPE )
                    {
                        partoffset = sec->PartitionTable[i].RelativeSectors;
                        break;
                    }
                }
                if( partoffset == 0 )
                {
                    Verbose(1, "Dump", "no VRAID partition on drive %u", devidx);
                    break;
                }
            }

            rc = PDskRead(hd, partoffset, 1, buffer);
            if( rc != 0 )
            {
                Verbose(0, "Dump", "PDskRead - rc %lu", rc);
                break;
            }

            DumpBuffer(output, buffer, 512);
            Verbose(0, "Dump", "-------- Dump of PHYSDEV sector (%lu) --------\n%s",
                    partoffset, output);
            {
                PSEC_PHYSDEV    sec = (PSEC_PHYSDEV)buffer;

                if( memcmp(sec->sectype, "PHYSDEVICE      ", 16) != 0 )
                {
                    Verbose(1, "Dump", "no PHYSDEV sector on partition");
                    break;
                }
                dataoffset = sec->u.s.adminspace;
            }

            for( i = 1; (((PSEC_VRDEV)buffer)->u.s.flags & 0x80) == 0; ++i )
            {
                rc = PDskRead(hd, partoffset+i, 1, buffer);
                if( rc != 0 )
                {
                    Verbose(0, "Dump", "PDskRead - rc %lu", rc);
                    break;
                }

                DumpBuffer(output, buffer, 512);
                Verbose(0,
                        "Dump", "-------- Dump of VRDEV sector (%lu) --------\n%s",
                        partoffset+i, output);
                if( memcmp(((PSEC_VRDEV)buffer)->sectype,"VRAIDDEVICE     ",16) != 0)
                {
                    Verbose(1, "Dump", "no VRAIDEVICE mark");
                    break;
                }
            }

            /* Now dump first data sector, it may contain a partition sector. */

            rc = PDskRead(hd, partoffset+dataoffset, 1, buffer);
            if( rc != 0 )
            {
                Verbose(0, "Dump", "PDskRead - rc %lu", rc);
                break;
            }

            DumpBuffer(output, buffer, 512);
            Verbose(0,
                    "Dump", "-------- Dump of first data sector (%lu) --------\n%s",
                    partoffset+dataoffset, output);
        }
        while( 0 );

        PDskClose(hd);

    } /* for every device*/

    delete[] output;
    return;
}








/* **********************************************************************
 * **** Public Entries **************************************************
 * ******************************************************************* */

/*# ----------------------------------------------------------------------
 * CALL
 *      DumpDialogProc(hwnd,msg,mp1,mp2)
 * PARAMETER
 *      hwnd,msg,mp1,mp2        standard parameter
 *      WM_INITDLG              mp2 contains hwnd
 * RETURNS
 *      MRESULT                 standard result code
 *      dialog handling returns id of control which ended dialog
 * GLOBAL
 *      none
 * DESPRIPTION
 *      Nothing special, selfcontained dialog procedure.
 *
 * REMARKS
 */
PUBLIC MRESULT EXPENTRY
DumpDialogProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
    static HWND parent;

    switch( msg )
    {
      case WM_INITDLG:
        parent = HWNDFROMMP(mp2);
        break;

      case WM_COMMAND:
        switch( SHORT1FROMMP(mp1) )
        {
          case DID_OK:
            {
                char    chText[200];
                LONG    cbText;
                ULONG   ul;
                int     i;

                cbText = WinQueryWindowText(WinWindowFromID(hwnd, EF_DRVINDEX),
                                            sizeof(chText), chText);
                for( i = 0; i < cbText; ++i )
                    if( !isdigit(chText[i])  &&  chText[i] != '-' )
                    {
                        MyMessageBox( 1, hwnd, "only numerics allowed!" );
                        return 0;
                    }

                /* Start thread. */

                sscanf(chText, " %ld", &ul);
                _beginthread(DumpPartitions, NULL, STACK_SIZE, (PVOID)ul);

                WinDismissDlg(hwnd, DID_START);
            }
            return 0;

          case DID_CANCEL:
            WinDismissDlg(hwnd, DID_CANCEL);
            return 0;
        }
        break;

      default:
        break;
    }

    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}
