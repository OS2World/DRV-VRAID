/*
 * $Source$
 * $Revision$
 * $Date$
 * $Author$
 *
 * Types and Constants
 *
 * $Log$
 * ----------------------------------------
 * This code is Copyright Vitus Jensen 1996
 */



#define MAX_UNITS		24
#define MAX_DEVICETABLE_SIZE	(1*1024)

#define ADDNAME		"DBUFFER_FILTER"
#define ADDFLAGS	0
#define DRIVERCLASS_ADD	1



#define PRIVATE		static
#define PUBLIC


#define SECTOR_SHIFT	9

#define ENABLE		_asm{ sti }
#define DISABLE		_asm{ cli }




typedef struct _UNITENTRY {

    void	(FAR *pADDEntry)();
    USHORT	UnitHandle;
    UNITINFO	NewUnitInfo;

} UNITENTRY, NEAR * NPUNITENTRY;

#define UNIT_ALLOCATED          0x0001
#define UNIT_LOCKED             0x0002
#define UNIT_INFOCHANGED        0x0004


/*---------------------------------------------------*/
/*                                                   */
/*                                                   */
/*---------------------------------------------------*/
#define STERR_GENERAL_FAILURE   0x810c
