#ifndef ERROR_STATUS_H
#define ERROR_STATUS_H


typedef enum
{
    ALL_OK = 0x00,
    NOK,
    NULL_PTR,
    INV_ARG,
    RCC_TIMEOUT,
    RCC_INV_CLK,
    RCC_INV_CFG,
    RCC_INV_CLK_SRC,
    RCC_PLL_CFG_WHILE_EN,
    GPIO_INV_PORT,
    GPIO_INV_PIN,
} SRV_enuErrorStatus_t;


#endif