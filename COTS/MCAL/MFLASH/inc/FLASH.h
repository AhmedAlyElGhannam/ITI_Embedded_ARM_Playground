#ifndef FLASH_H
#define FLASH_H


typedef enum 
{
    MFLASH_MAIN_MEM_SECTOR0,
    MFLASH_MAIN_MEM_SECTOR1,
    MFLASH_MAIN_MEM_SECTOR2,
    MFLASH_MAIN_MEM_SECTOR3,
    MFLASH_MAIN_MEM_SECTOR4,
    MFLASH_MAIN_MEM_SECTOR5,
    MFLASH_MAIN_MEM_SECTOR6,
    MFLASH_MAIN_MEM_SECTOR7,
    MFLASH_SYSTEM_MEM,
    MFLASH_OTP_AREA,
    MFLASH_OPTION_BYTES,
} MFLASH_enuMainMemorySectors_t;

typedef enum 
{
    MFLASH_KEY1 = 0x45670123,
    MFLASH_KEY2 = 0xCDEF89AB
} MFLASH_enuFlashWriteUnlockKey_t;

typedef enum 
{
    MFLASH_OPTKEY1 = 0x08192A3B,
    MFLASH_OPTKEY2 = 0x4C5D6E7F,
} MFLASH_enuOptionByteUnlockKey_t;

typedef enum 
{
    DATA_CACHE_RESET = 1U,
    DATA_CACHE_NOT_RESET = !DATA_CACHE_RESET,
} MFLASH_enuDataCacheReset_t;

typedef enum 
{
    INSTRUCTION_CACHE_RESET = 1U,
    INSTRUCTION_CACHE_NOT_RESET = !INSTRUCTION_CACHE_RESET,
} MFLASH_enuInstructionCacheReset_t;

typedef enum 
{
    DATA_CACHE_EN = 1U,
    DATA_CACHE_DIS = !DATA_CACHE_EN,
} MFLASH_enuDataCacheConfig_t;

typedef enum 
{
    INSTRUCTION_CACHE_EN = 1U,
    INSTRUCTION_CACHE_DIS = !INSTRUCTION_CACHE_EN,
} MFLASH_enuIntructionCacheConfig_t;

typedef enum 
{
    PREFETCH_EN = 1U,
    PREFETCH_DIS = !PREFETCH_EN,
} MFLASH_enuPrefetchConfig_t;

typedef enum 
{
    LATENCY_0_WAIT_STATES = 0x00,
    LATENCY_1_WAIT_STATE,
    LATENCY_2_WAIT_STATES,
    LATENCY_3_WAIT_STATES,
    LATENCY_4_WAIT_STATES,
    LATENCY_5_WAIT_STATES,
    LATENCY_6_WAIT_STATES,
    LATENCY_7_WAIT_STATES,
    LATENCY_8_WAIT_STATES,
    LATENCY_9_WAIT_STATES,
    LATENCY_10_WAIT_STATES,
    LATENCY_11_WAIT_STATES,
    LATENCY_12_WAIT_STATES,
    LATENCY_13_WAIT_STATES,
    LATENCY_14_WAIT_STATES,
    LATENCY_15_WAIT_STATES = 0x0F,
} MFLASH_enuLatency_t;

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

typedef enum 
{
    ERROR_INT_EN = 1U,
    ERROR_INT_DIS = !ERROR_INT_EN,
} MFLASH_enuErrorIRQConfig_t;


typedef enum 
{
    END_OF_OPERATION_INT_EN = 1U,
    END_OF_OPERATION_INT_DIS = !END_OF_OPERATION_INT_EN,
} MFLASH_enuEndOfOperationIRQConfig_t;

/* function for STRT bit in CR */

typedef enum 
{
    PROGRAM_x8 = 0b00,
    PROGRAM_x16 = 0b01,
    PROGRAM_x32 = 0b10,
    PROGRAM_x64 = 0b11
} MFLASH_enuProgramSize_t;

/* to select which sector to erase */
typedef enum 
{
    SECTOR0 = 0x00,
    SECTOR1 = 0x01,
    SECTOR2 = 0x02,
    SECTOR3 = 0x03,
    SECTOR4 = 0x04,
    SECTOR5 = 0x05,
    SECTOR6 = 0x06,
    SECTOR7 = 0x07,
    USER_SPECIFIC_SECTOR = 0x0C,
    USER_CONFIGURATION_SECTOR = 0x0D,
} MFLASH_enuSectorNumber_t;

/* function for mass erase (MER bit in CR)*/
/* function for sector erase (SER bit in CR)*/
/* function for activating flash programming (PG bit in CR)*/

typedef enum 
{
    FLASH_END_OF_OPERATION_INT, /*< EOP && EOPIE */
    FLASH_WRITE_PROTECC_ERR_INT, /*< WRPERR && ERRIE */
    FLASH_PROGRAMMING_ERR_INT, /*< PGAERR, PGPERR, PGSERR && ERRIE */
    NUM_OF_FLASH_IRQs,
} MFLASH_enuIRQSources_t;

typedef enum 
{
    RDP_LVL0, /*< no read protection */
    RDP_LVL1, /*< read protecc enabled */
    RDP_LVL2, /*< debug/chip read protection disabled */
} MFLASH_enuReadProtectionLevels_t;

// typedef enum 
// {
//     PCROP_EN = 1U,
//     PCROP_DIS = !PCROP_EN,
// } MFLASH_enuSelectionofProteccModeofnWPRiConfig_t;

// typedef enum 
// {
//     /**/
// } MFLASH_enuNotWriteProtect_t;

/* function for option start */
/* function for option lock */


/*
    Requirements:
    1. Erase_sector
    1. Program (aka write)
    1. unlock_flash
    1. lock_flash
    1. mass_tablee3
*/

/* Function(s) for User Option Byte modifications */



#endif