#ifndef SYSTICK_REG_H
#define SYSTICK_REG_H

typedef struct
{
    volatile uint32_t STK_CTRL;
    volatile uint32_t STK_LOAD;
    volatile uint32_t STK_VAL;
    volatile uint32_t STK_CALIB;
} SYSTICK_Registers_t;

#define SYSTICK_BASE_ADDRESS ((volatile void*)0xE000E010)

#endif 