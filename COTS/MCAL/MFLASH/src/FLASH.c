#include "FLASH.h"

#define MFLASH_SECTOR0_16KB_START   (0x08000000UL)
#define MFLASH_SECTOR0_16KB_END     (0x08003FFFUL)
#define MFLASH_SECTOR1_16KB_START   (0x08004000UL)
#define MFLASH_SECTOR1_16KB_END     (0x08007FFFUL)
#define MFLASH_SECTOR2_16KB_START   (0x08008000UL)
#define MFLASH_SECTOR2_16KB_END     (0x0800BFFFUL)
#define MFLASH_SECTOR3_16KB_START   (0x0800C000UL)
#define MFLASH_SECTOR3_16KB_END     (0x0800FFFFUL)
#define MFLASH_SECTOR4_64KB_START   (0x08010000UL)
#define MFLASH_SECTOR4_64KB_END     (0x0801FFFFUL)
#define MFLASH_SECTOR5_128KB_START  (0x08020000UL)
#define MFLASH_SECTOR5_128KB_END    (0x0803FFFFUL)
#define MFLASH_SECTOR6_128KB_START  (0x08040000UL)
#define MFLASH_SECTOR6_128KB_END    (0x0805FFFFUL)
#define MFLASH_SECTOR7_128KB_START  (0x08060000UL)
#define MFLASH_SECTOR7_128KB_END    (0x0807FFFFUL)


/*
    @TODO: Make get + clr functions for these flags
    Flags in SR (all are cleared by writing 1 to the same bit EXCEPT Busy flag (read-only))
    Busy
    Read Protection Error
    Programming Sequence Error
    Programming Parallelism Error
    Programming Alignment Error
    Write Protection Error
    Operation Error
    End of Operation
*/

/* function for LOCK bit in CR */

/* function for STRT bit in CR */

/* function for mass erase (MER bit in CR)*/
/* function for sector erase (SER bit in CR)*/
/* function for activating flash programming (PG bit in CR)*/

/*
    Requirements:
    1. Erase_sector
    1. Program (aka write)
    1. unlock_flash
    1. lock_flash
    1. mass_tablee3
*/