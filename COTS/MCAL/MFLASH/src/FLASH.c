#include "std_types.h"
#include "std_lib.h"
#include "error_status.h"
#include "FLASH_Reg.h"
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


static MFLASH_ptrCBF [NUM_OF_FLASH_IRQs] = {NULL};


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

uint8_t MFLASH_uint8IsFlashBusy(void)
{
    MFLASH_Registers_t* MFLASH = (MFLASH_Registers_t*)FLASH_PERIPHERAL_BASE_ADDRESS;
    return ((MFLASH->FLASH_SR >> 16) & 1);
}

uint8_t MFLASH_uint8IsReadProtectionErrRaised(void)
{
    MFLASH_Registers_t* MFLASH = (MFLASH_Registers_t*)FLASH_PERIPHERAL_BASE_ADDRESS;
    return ((MFLASH->FLASH_SR >> 8) & 1);
}

uint8_t MFLASH_uint8IsProgrammingSequenceErrRaised(void)
{
    MFLASH_Registers_t* MFLASH = (MFLASH_Registers_t*)FLASH_PERIPHERAL_BASE_ADDRESS;
    return ((MFLASH->FLASH_SR >> 7) & 1);
}

uint8_t MFLASH_uint8IsProgrammingParallelismErrRaised(void)
{
    MFLASH_Registers_t* MFLASH = (MFLASH_Registers_t*)FLASH_PERIPHERAL_BASE_ADDRESS;
    return ((MFLASH->FLASH_SR >> 6) & 1);
}

uint8_t MFLASH_uint8IsProgrammingAlignmentErrRaised(void)
{
    MFLASH_Registers_t* MFLASH = (MFLASH_Registers_t*)FLASH_PERIPHERAL_BASE_ADDRESS;
    return ((MFLASH->FLASH_SR >> 5) & 1);
}

uint8_t MFLASH_uint8IsWriteProtectionErrRaised(void)
{
    MFLASH_Registers_t* MFLASH = (MFLASH_Registers_t*)FLASH_PERIPHERAL_BASE_ADDRESS;
    return ((MFLASH->FLASH_SR >> 4) & 1);
}

uint8_t MFLASH_uint8IsOperationErrRaised(void)
{
    MFLASH_Registers_t* MFLASH = (MFLASH_Registers_t*)FLASH_PERIPHERAL_BASE_ADDRESS;
    return ((MFLASH->FLASH_SR >> 1) & 1);
}

uint8_t MFLASH_uint8IsEndOfOperation(void)
{
    MFLASH_Registers_t* MFLASH = (MFLASH_Registers_t*)FLASH_PERIPHERAL_BASE_ADDRESS;
    return ((MFLASH->FLASH_SR >> 0) & 1);
}

void MFLASH_voidLockFlash(void)
{
    MFLASH_Registers_t* MFLASH = (MFLASH_Registers_t*)FLASH_PERIPHERAL_BASE_ADDRESS;
    MFLASH->FLASH_CR |= (1 << 31);
    return;
}

void MFLASH_voidUnlockFlash(void)
{
    MFLASH_Registers_t* MFLASH = (MFLASH_Registers_t*)FLASH_PERIPHERAL_BASE_ADDRESS;
    MFLASH->FLASH_KEYR = MFLASH_KEY1;
    MFLASH->FLASH_KEYR = MFLASH_KEY2;
    return;
}

void MFLASH_voidStartEraseOperation(void)
{
    MFLASH_Registers_t* MFLASH = (MFLASH_Registers_t*)FLASH_PERIPHERAL_BASE_ADDRESS;
    MFLASH->FLASH_CR |= (1 << 16);
    return;
}



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