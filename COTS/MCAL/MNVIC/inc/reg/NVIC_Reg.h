#ifndef NVIC_REG_H
#define NVIC_REG_H

typedef struct 
{
    volatile uint32_t ISER[8]; // set enable
    const uint32_t RESERVED0[24]; // 96 bytes in-between
    volatile uint32_t ICER[8]; // clear enable
    const uint32_t RESERVED1[24]; // 96 bytes in-between
    volatile uint32_t ISPR[8]; // set pending
    const uint32_t RESERVED2[24]; // 96 bytes in-between
    volatile uint32_t ICPR[8]; // clear pending
    const uint32_t RESERVED3[24]; // 96 bytes in-between
    volatile uint32_t IABR[8]; // active bit
    const uint32_t RESERVED4[56]; // 224 bytes in-between
    volatile uint32_t IPR[60]; // priority
    const uint32_t RESERVED5[644]; // 2576 bytes in-between
    volatile uint32_t STIR; // software trigger
} NVIC_Registers_t;

#define NVIC_BASE_ADDRESS ((volatile void*)0xE000E100)

#endif