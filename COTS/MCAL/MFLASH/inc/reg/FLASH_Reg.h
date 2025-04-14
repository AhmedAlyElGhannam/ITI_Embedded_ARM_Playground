#ifndef FLASH_REGISTERS_H
#define FLASH_REGISTERS_H

#include "std_types.h"

typedef struct 
{
    volatile uint32_t FLASH_ACR;
    volatile uint32_t FLASH_KEYR;
    volatile uint32_t FLASH_OPTKEYR;
    volatile uint32_t FLASH_SR;
    volatile uint32_t FLASH_CR;
    volatile uint32_t FLASH_OPTCR;
} MFLASH_Registers_t __attribute__((packed));

#define FLASH_PERIPHERAL_BASE_ADDRESS   (0x40023C00UL)

#endif