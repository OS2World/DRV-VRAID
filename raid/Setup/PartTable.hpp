/*
 * $Source: R:/Source/driver/raid/Setup/RCS/PartTable.hpp,v $
 * $Revision: 1.2 $
 * $Date: 2000/03/09 23:12:07 $
 * $Locker:  $
 *
 *	Class PartTable to read/modify partition tables.
 *
 * $Log: PartTable.hpp,v $
 * Revision 1.2  2000/03/09 23:12:07  vitus
 * - another constructor to tell object where extended partitions start.  This
 *   is important for new Type 5 partitions as their position ist calculated
 *   for the very first on.
 * - method clear() to force object to create a new partition table/sector.  Otherwise
 *   any previous contents of the read sector (even an old partition table!) would
 *   be preserved.
 *
 * Revision 1.1  2000/02/13 23:56:32  vitus
 * Initial revision
 * ----------------------------------------
 * This code is Copyright Vitus Jensen 2000
 */


class PartTable {
    VOS2Disk *  disk;
    ULONG	secno;
    ULONG	extstart;

    MBR		bootrecord;
    BOOL	modified, valid;
    APIRET	rderr;				/* if !0 -> never write */

    void	readRecord(void);

public:
    PPARTITIONENTRY	entry;

    PartTable(VOS2Disk * d,ULONG n,Boolean rd=false);
    PartTable(VOS2Disk * d,ULONG n,ULONG first_extended_start);
    ~PartTable();

    void	clear(void);
    BOOL	isValid() { return valid; }
    ULONG	queryPos() { return secno; }

    BOOL	delPE(PPARTITIONENTRY partentry);

    BOOL	addPE(PARTITIONENTRY const &partentry);
    BOOL	addForeign(UCHAR type,UCHAR bootable,ULONG start,ULONG size);
    BOOL	addOwn(UCHAR type,UCHAR bootable,ULONG start,ULONG size);
    APIRET	flush();
};
