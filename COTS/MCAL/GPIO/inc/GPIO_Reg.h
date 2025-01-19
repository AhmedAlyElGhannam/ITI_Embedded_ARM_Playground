#ifndef GPIO_REG_H
#define GPIO_REG_H

typedef struct
{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
} GPIOx_Registers_t;

#define GPIO_BASE_ADDRESS   ((GPIOx_Registers_t*)0x40020000ULL)
#define GPIO_GET_PORT_ADDRESS(GPIOx)    \
    (GPIO_BASE_ADDRESS + (GPIOx * 0x400))


#endif