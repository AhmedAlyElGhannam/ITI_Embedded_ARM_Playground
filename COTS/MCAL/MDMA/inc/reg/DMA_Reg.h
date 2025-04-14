#ifndef DMA_REG_H
#define DMA_REG_H

#include "std_types.h"


typedef struct 
{
    volatile uint32_t SxCR;
    volatile uint32_t SxNDTR;
    volatile uint32_t SxPAR;
    volatile uint32_t SxM0AR;
    volatile uint32_t SxM1AR;
    volatile uint32_t SxFCR;
} MDMA_StreamRegisters_t __attribute__((packed));

typedef struct 
{
    volatile uint32_t LISR;
    volatile uint32_t HISR;
    volatile uint32_t LIFCR;
    volatile uint32_t HIFCR;
    volatile MDMA_StreamRegisters_t Streamx[8];
} MDMA_Registers_t __attribute__((packed));

#define DMA_PERIPHERAL_BASE_ADDRESS                         (0x40026000UL)
#define DMA_GET_CONTROLLER_BASE_ADDRESS(X)                  (((DMA_PERIPHERAL_BASE_ADDRESS) + ((uint32_t)X * 0x400UL)))

#endif
