#ifndef USART_REG_H
#define USART_REG_H


#include "std_types.h"

typedef struct 
{
    uint32_t SR;
    uint32_t DR;
    uint32_t BRR;
    uint32_t CR1;
    uint32_t CR2;
    uint32_t CR3;
    uint32_t GTPR;
} MUSART_Registers_t;

#define USART_PERIPHERAL_BASE_ADDRESS (0x40004800UL)
#define USART_GET_MODULE_BASE_ADDRESS(X) ((void*)(USART_PERIPHERAL_BASE_ADDRESS + ((uint32_t)X)))


#endif