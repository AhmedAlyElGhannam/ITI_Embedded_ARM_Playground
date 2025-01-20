#ifndef RCC_H
#define RCC_H

/* 
    CR -> check for ready status && turn on the following
        1. PLL
        1. I2S PLL
        1. CSS (on only)
        1. HSE
        1. HSI
    also, it has values for trimming HSI clock -> HSITRIM
 */

/* 
    PLLCFGR -> give it the values of M, N, P, Q

 */

/*
    CFGR -> to set clock source and prescaler of:
            MCO1 && MCO2
         -> to set the prescaler of:
            AHB && APB
         -> clock selection of I2S
         -> select system clock && read the selected system clock ###
*/

/*
    CIR -> contain InterruptFlagClear, InterruptFlagStatus, and InterruptEnable for the following:
            CSS, PLL, PLLI2S, HSE, HSI, LSE, LSI
*/

/*
    AHB1RSTR -> resets the peripheral clock for the ones connected on AHB1 bus:
                DMA2, DMA1, CRC, GPIOA-H, 
*/

/*
    AHB2RSTR -> resets the peripheral clock for the ones connected on AHB2 bus:
                 USBOTG
*/  

/*
    APB1RSTR -> resets the peripheral clock for the ones connected on APB1 bus:
                 PowerInterfaceReset, I2C3, I2C2, I2C1, USART2, SPI3, SPI2, WindowWDG, TIM5-TIM2
*/ 

/*
    APB2RSTR -> resets the peripheral clock for the ones connected on APB2 bus:
                TIM11-TIM9, SystemConfigurationController, SPI4, SPI1, SDIO, ADC1, USART6, USART1, TIM1
*/ 

/*
    AHB1ENR -> enables clock for peripherals connected on AHB1:
                DMA2, DMA1, CRC, GPIOA-H,
*/

/*
    AHB2ENR -> enables clock for peripherals connected on AHB2:
                USBOTG
*/

/*
    APB1ENR -> enables clock for peripherals connected on APB1 bus:
                 PowerInterfaceReset, I2C3, I2C2, I2C1, USART2, SPI3, SPI2, WindowWDG, TIM5-TIM2
*/ 

/*
    APB2ENR -> enables clock for peripherals connected on APB2 bus:
                TIM11-TIM9, SystemConfigurationController, SPI4, SPI1, SDIO, ADC1, USART6, USART1, TIM1
*/ 

/*
    BDCR -> resets backup domain, enables RTC clock, select RTC clock source
         -> External Low-speed Oscillator Ready, ByPass, Enable
*/

/*
    CSR -> bit 31-24 are reset flags,
        -> Internal Low-speed Oscillator Ready, Enable
*/

/*
    SSCGR -> for spread spectrum (out of scope -- concerned with PLL performance)
*/

/*
    PLLI2SCFGR -> values for N && R in PLLI2S
*/

/*
    DCKCFGR -> sets prescaler for timers
*/



/*
    TODO:
        1. make functions to get the status of flags (make it return the value in a ptr and ALWAYS make the return value an error status)
        1. make a function for enabling interrupts that takes multiple ORed masks 
*/


/*
    ### for waiting
    when waiting for a flag, ALWAYS add a timeout condition
    DO NOT DO: while (RCC->CFGR & HSI_READY);
    DO: while ((RCC->CFGR & HSI_READY) && (timeout--));

    make a timeout error status after that:
    if (timeout == 0 && HSI_NOT_READY)
        status = timeout;
*/


/*
    use anonymous unions and bitfields for PLL configurations

    typedef struct
    {
        union
        {
            struct
            {
                M : 6
                N : 4
                .
                .
            } cfg_t;

            uint32_t reg;
        }
    }
*/

/*
    __attribute__((packed))

    #pragme pack(1) // to force compiler to not pack stuff
*/

/*
    Feature in PlatformIO:
        Project Inspect -> gives info about memory usage + static analysis for code
*/
#include "RCC_PBCFG.h"

typedef enum
{
    ALL_OK = 0x00,
    NOK,
    NULL_PTR,
    INV_ARG,
    RCC_TIMEOUT,
    RCC_INV_CLK_SRC,
    RCC_PLL_CFG_WHILE_EN,
} SRV_enuErrorStatus_t;

typedef enum
{
    PLL_CLK_READY = 0x2000000ULL,
    PLL_CLK_NOT_READY = 0x00000000ULL,
} SRV_enuPLLClkStatus_t;

typedef enum
{
    PLLI2S_CLK_READY = 0x8000000ULL,
    PLLI2S_CLK_NOT_READY = 0x00000000ULL,
} SRV_enuPLLI2SClkStatus_t;

typedef enum
{
    HSE_CLK_READY = 0x20000ULL,
    HSE_CLK_NOT_READY = 0x00000000ULL,
} SRV_enuHSEClkStatus_t;

typedef enum
{
    CSS_ENABLE = 0x80000ULL,
    CSS_DISABLE = 0x00000ULL,
} SRV_enuCSSConfig_t;

typedef enum
{
    MRCC_SYS_CLK_HSI = 0x00000000ULL,
    MRCC_SYS_CLK_HSE = 0x00000001ULL,
    MRCC_SYS_CLK_PLL = 0x00000010ULL,
} MRCC_enuSYSCLK_t;

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned long int uint32_t;

typedef enum 
{
    PLL_M   = (MRCC_PLL_M_PRESCALER << 0)  & (0x0000003FUL),
    PLL_N   = (MRCC_PLL_N_PRESCALER << 6)  & (0x00007FC0UL),
    PLL_P   = (MRCC_PLL_P_PRESCALER << 16) & (0x00030000UL),
    PLL_Q   = (MRCC_PLL_Q_PRESCALER << 24) & (0x0F000000UL),
    PLL_SRC = (MRCC_PLL_SRC << 22)         & (0x00400000UL),
    PLL_COMBINED_PRE = (PLL_M | PLL_N | PLL_P | PLL_Q | PLL_SRC),  /* use this value in PLL_config function */
} MRCC_enuPLLConfig_t;

#define MRCC_APB_DIV_BY_2       (0b100UL)       
#define MRCC_APB_DIV_BY_4       (0b101UL)
#define MRCC_APB_DIV_BY_8       (0b110UL)
#define MRCC_APB_DIV_BY_16      (0b111UL)

#define MRCC_AHB_DIV_BY_2           (0b1000UL)
#define MRCC_AHB_DIV_BY_4           (0b1001UL)
#define MRCC_AHB_DIV_BY_8           (0b1010UL)
#define MRCC_AHB_DIV_BY_16          (0b1011UL)
#define MRCC_AHB_DIV_BY_64          (0b1100UL)
#define MRCC_AHB_DIV_BY_128         (0b1101UL)
#define MRCC_AHB_DIV_BY_256         (0b1110UL)
#define MRCC_AHB_DIV_BY_512         (0b1111UL)


typedef enum 
{
    AHB1_PRE = (MRCC_AHB_DIV_BY_2 << 4) & (0xF0UL),
    APB1_PRE = (MRCC_APB_DIV_BY_4 << 10) & (0x1C00UL),
    APB2_PRE = (MRCC_APB_DIV_BY_4 << 13) & (0xE000UL),
    PERIPH_COMBINED_PRE = (AHB1_PRE | APB1_PRE | APB2_PRE),  /* use this value in bus prescaler function */
} MRCC_enuPeriphBusPresConfig_t;

typedef enum
{
    CSSC = (1U << 23),
    PLLI2S_RDYC = (1U << 21),
    PLL_RDYC = (1U << 20),
    HSE_RDYC = (1U << 19),
    HSI_RDYC = (1U << 18),
    LSE_RDYC = (1U << 17),
    LSI_RDYC = (1U << 16),
} MRCC_enuClrIntRdyFlags_t;

#define MRCC_AHB1_GPIOA_POS     (0UL)
#define MRCC_AHB1_GPIOA_MSK     (0x1UL << MRCC_AHB1_GPIOA_POS)

#define MRCC_AHB1_GPIOB_POS     (1UL)
#define MRCC_AHB1_GPIOB_MSK     (0x1UL << MRCC_AHB1_GPIOB_POS)

#define MRCC_AHB1_GPIOC_POS     (2UL)
#define MRCC_AHB1_GPIOC_MSK     (0x1UL << MRCC_AHB1_GPIOC_POS)

#define MRCC_AHB1_GPIOD_POS     (3UL)
#define MRCC_AHB1_GPIOD_MSK     (0x1UL << MRCC_AHB1_GPIOD_POS)

#define MRCC_AHB1_GPIOE_POS     (0UL)
#define MRCC_AHB1_GPIOE_MSK     (0x1UL << MRCC_AHB1_GPIOE_POS)

#define MRCC_AHB1_GPIOH_POS     (7UL)
#define MRCC_AHB1_GPIOH_MSK     (0x1UL << MRCC_AHB1_GPIOH_POS)

#define MRCC_AHB1_DMA1_POS      (21UL)
#define MRCC_AHB1_DMA1_MSK      (0x1UL << MRCC_AHB1_DMA1_POS)

#define MRCC_AHB1_DMA2_POS      (22UL)
#define MRCC_AHB1_DMA2_MSK      (0x1UL << MRCC_AHB1_DMA2_POS)

#define MRCC_AHB1_SRAM1_POS     (16UL)
#define MRCC_AHB1_SRAM1_MSK     (0x1UL << MRCC_AHB1_SRAM1_POS)

#define MRCC_AHB1_FLITF_POS     (15UL)
#define MRCC_AHB1_FLITF_MSK     (0x1UL << MRCC_AHB1_FLITF_POS)

#define MRCC_AHB1_CRC_POS       (12UL)
#define MRCC_AHB1_CRC_MSK       (0x1UL << MRCC_AHB1_CRC_POS)

#define MRCC_AHB2_USBOTG_POS       (7UL)
#define MRCC_AHB2_USBOTG_MSK       (0x1UL << MRCC_AHB2_USBOTG_POS)

#define MRCC_APB1_PWR_POS       (28UL)
#define MRCC_APB1_PWR_MSK       (0x1UL << MRCC_APB1_PWR_POS)

#define MRCC_APB1_I2C3_POS       (23UL)
#define MRCC_APB1_I2C3_MSK       (0x1UL << MRCC_APB1_I2C3_POS)

#define MRCC_APB1_I2C2_POS       (22UL)
#define MRCC_APB1_I2C2_MSK       (0x1UL << MRCC_APB1_I2C2_POS)

#define MRCC_APB1_I2C1_POS       (21UL)
#define MRCC_APB1_I2C1_MSK       (0x1UL << MRCC_APB1_I2C1_POS)

#define MRCC_APB1_USART2_POS       (17UL)
#define MRCC_APB1_USART2_MSK       (0x1UL << MRCC_APB1_USART2_POS)

#define MRCC_APB1_SPI3_POS       (15UL)
#define MRCC_APB1_SPI3_MSK       (0x1UL << MRCC_APB1_SPI3_POS)

#define MRCC_APB1_SPI2_POS       (14UL)
#define MRCC_APB1_SPI2_MSK       (0x1UL << MRCC_APB1_SPI2_POS)

#define MRCC_APB1_WWDG_POS       (11UL)
#define MRCC_APB1_WWDG_MSK       (0x1UL << MRCC_APB1_WWDG_POS)

#define MRCC_APB1_TIM5_POS       (3UL)
#define MRCC_APB1_TIM5_MSK       (0x1UL << MRCC_APB1_TIM5_POS)

#define MRCC_APB1_TIM4_POS       (2UL)
#define MRCC_APB1_TIM4_MSK       (0x1UL << MRCC_APB1_TIM4_POS)

#define MRCC_APB1_TIM3_POS       (1UL)
#define MRCC_APB1_TIM3_MSK       (0x1UL << MRCC_APB1_TIM3_POS)

#define MRCC_APB1_TIM2_POS       (0UL)
#define MRCC_APB1_TIM2_MSK       (0x1UL << MRCC_APB1_TIM2_POS)

#define MRCC_APB2_TIM11_POS       (18UL)
#define MRCC_APB2_TIM11_MSK       (0x1UL << MRCC_APB2_TIM11_POS)

#define MRCC_APB2_TIM10_POS       (17UL)
#define MRCC_APB2_TIM10_MSK       (0x1UL << MRCC_APB2_TIM10_POS)

#define MRCC_APB2_TIM9_POS       (16UL)
#define MRCC_APB2_TIM9_MSK       (0x1UL << MRCC_APB2_TIM9_POS)

#define MRCC_APB2_SYSCFG_POS       (28UL)
#define MRCC_APB2_SYSCFG_MSK       (0x1UL << MRCC_APB2_SYSCFG_POS)

#define MRCC_APB2_SPI4_POS       (13UL)
#define MRCC_APB2_SPI4_MSK       (0x1UL << MRCC_APB2_SPI4_POS)

#define MRCC_APB2_SPI1_POS       (12UL)
#define MRCC_APB2_SPI1_MSK       (0x1UL << MRCC_APB2_SPI1_POS)

#define MRCC_APB2_SDIO_POS       (11UL)
#define MRCC_APB2_SDIO_MSK       (0x1UL << MRCC_APB2_SDIO_POS)

#define MRCC_APB2_ADC1_POS       (8UL)
#define MRCC_APB2_ADC1_MSK       (0x1UL << MRCC_APB2_ADC1_POS)

#define MRCC_APB2_USART6_POS       (5UL)
#define MRCC_APB2_USART6_MSK       (0x1UL << MRCC_APB2_USART6_POS)

#define MRCC_APB2_USART1_POS       (4UL)
#define MRCC_APB2_USART1_MSK       (0x1UL << MRCC_APB2_USART1_POS)

#define MRCC_APB2_TIM1_POS       (0UL)
#define MRCC_APB2_TIM1_MSK       (0x1UL << MRCC_APB2_TIM1_POS)



/* SYSCFG CLK */
#define MRCC_ENABLE_SYSCFG_CLK()        ((RCC->APB2ENR) |= MRCC_APB2_SYSCFG_MSK)
#define MRCC_DISABLE_SYSCFG_CLK()       ((RCC->APB2ENR) &= ~MRCC_APB2_SYSCFG_MSK)
#define MRCC_RESET_SYSCFG_CLK()         ((RCC->APB2RSTR) |= MRCC_APB2_SYSCFG_MSK)
/***/


/* SDIO */
#define MRCC_ENABLE_SDIO_CLK()          ((RCC->APB2ENR) |= MRCC_APB2_SDIO_MSK)
#define MRCC_DISABLE_SDIO_CLK()         ((RCC->APB2ENR) &= ~MRCC_APB2_SDIO_MSK)
#define MRCC_RESET_SDIO_CLK()           ((RCC->APB2RSTR) |= MRCC_APB2_SDIO_MSK)
/***/


/* USART */
#define MRCC_ENABLE_USART6_CLK()        ((RCC->APB2ENR) |= MRCC_APB2_USART6_MSK)
#define MRCC_ENABLE_USART2_CLK()        ((RCC->APB1ENR) |= MRCC_APB1_USART2_MSK)
#define MRCC_ENABLE_USART1_CLK()        ((RCC->APB2ENR) |= MRCC_APB2_USART1_MSK)

#define MRCC_DISABLE_USART6_CLK()       ((RCC->APB2ENR) &= ~MRCC_APB2_USART6_MSK)
#define MRCC_DISABLE_USART2_CLK()       ((RCC->APB1ENR) &= ~MRCC_APB1_USART2_MSK)
#define MRCC_DISABLE_USART1_CLK()       ((RCC->APB2ENR) &= ~MRCC_APB2_USART1_MSK)

#define MRCC_RESET_USART6_CLK()         ((RCC->APB2RSTR) |= MRCC_APB2_USART6_MSK)
#define MRCC_RESET_USART2_CLK()         ((RCC->APB1RSTR) |= MRCC_APB1_USART2_MSK)
#define MRCC_RESET_USART1_CLK()         ((RCC->APB2RSTR) |= MRCC_APB2_USART1_MSK)
/***/


/* ADC CLK */
#define MRCC_ENABLE_ADC1_CLK()          ((RCC->APB2ENR) |= MRCC_APB2_ADC1_MSK)
#define MRCC_DISABLE_ADC1_CLK()         ((RCC->APB2ENR) &= ~MRCC_APB2_ADC1_MSK)
#define MRCC_RESET_ADC1_CLK()           ((RCC->APB2RSTR) |= MRCC_APB2_ADC1_MSK)
/***/


/* CRC CLK */
#define MRCC_ENABLE_CRC_CLK()           ((RCC->APB1ENR) |= MRCC_AHB1_CRC_MSK)
#define MRCC_DISABLE_CRC_CLK()          ((RCC->APB1ENR) &= ~MRCC_AHB1_CRC_MSK)
#define MRCC_RESET_CRC_CLK()            ((RCC->APB1RSTR) |= MRCC_AHB1_CRC_MSK)
/***/


/* USBOTG CLK */
#define MRCC_ENABLE_USBOTG_CLK()         ((RCC->AHB2ENR) |= MRCC_AHB2_USBOTG_MSK)
#define MRCC_DISABLE_USBOTG_CLK()        ((RCC->AHB2ENR) &= ~MRCC_AHB2_USBOTG_MSK)
#define MRCC_RESET_USBOTG_CLK()          ((RCC->AHB2RSTR) |= MRCC_AHB2_USBOTG_MSK)
/***/



/* WWDG CLK */
#define MRCC_ENABLE_WWDG_CLK()           ((RCC->APB1ENR) |= MRCC_APB1_WWDG_MSK)
#define MRCC_DISABLE_WWDG_CLK()          ((RCC->APB1ENR) &= ~MRCC_APB1_WWDG_MSK)
#define MRCC_RESET_WWDG_CLK()            ((RCC->APB1RSTR) |= MRCC_APB1_WWDG_MSK)
/***/

/* PWR CLK */
#define MRCC_ENABLE_PWR_CLK()           ((RCC->APB1ENR) |= MRCC_APB1_PWR_MSK)
#define MRCC_DISABLE_PWR_CLK()          ((RCC->APB1ENR) &= ~MRCC_APB1_PWR_MSK)
#define MRCC_RESET_PWR_CLK()            ((RCC->APB1RSTR) |= MRCC_APB1_PWR_MSK)
/***/


/* DMA CLK */
#define MRCC_ENABLE_DMA2_CLK()          ((RCC->AHB1ENR) |= MRCC_AHB1_DMA2_MSK)
#define MRCC_ENABLE_DMA1_CLK()          ((RCC->AHB1ENR) |= MRCC_AHB1_DMA1_MSK)

#define MRCC_DISABLE_DMA2_CLK()         ((RCC->AHB1ENR) &= ~MRCC_AHB1_DMA2_MSK)
#define MRCC_DISABLE_DMA1_CLK()         ((RCC->AHB1ENR) &= ~MRCC_AHB1_DMA1_MSK)

#define MRCC_RESET_DMA2_CLK()           ((RCC->AHB1RSTR) |= MRCC_AHB1_DMA2_MSK)
#define MRCC_RESET_DMA1_CLK()           ((RCC->AHB1RSTR) |= MRCC_AHB1_DMA1_MSK)
/***/


/* TIM CLK */
#define MRCC_ENABLE_TIM11_CLK()         ((RCC->APB2ENR) |= MRCC_APB2_TIM11_MSK)
#define MRCC_ENABLE_TIM10_CLK()         ((RCC->APB2ENR) |= MRCC_APB2_TIM10_MSK)
#define MRCC_ENABLE_TIM9_CLK()          ((RCC->APB2ENR) |= MRCC_APB2_TIM9_MSK)
#define MRCC_ENABLE_TIM5_CLK()          ((RCC->APB1ENR) |= MRCC_APB1_TIM5_MSK)
#define MRCC_ENABLE_TIM4_CLK()          ((RCC->APB1ENR) |= MRCC_APB1_TIM4_MSK)
#define MRCC_ENABLE_TIM3_CLK()          ((RCC->APB1ENR) |= MRCC_APB1_TIM3_MSK)
#define MRCC_ENABLE_TIM2_CLK()          ((RCC->APB1ENR) |= MRCC_APB1_TIM2_MSK)
#define MRCC_ENABLE_TIM1_CLK()          ((RCC->APB2ENR) |= MRCC_APB2_TIM1_MSK)

#define MRCC_DISABLE_TIM11_CLK()        ((RCC->APB2ENR) &= ~MRCC_APB2_TIM11_MSK)
#define MRCC_DISABLE_TIM10_CLK()        ((RCC->APB2ENR) &= ~MRCC_APB2_TIM10_MSK)
#define MRCC_DISABLE_TIM9_CLK()         ((RCC->APB2ENR) &= ~MRCC_APB2_TIM9_MSK)
#define MRCC_DISABLE_TIM5_CLK()         ((RCC->APB1ENR) &= ~MRCC_APB1_TIM5_MSK)
#define MRCC_DISABLE_TIM4_CLK()         ((RCC->APB1ENR) &= ~MRCC_APB1_TIM4_MSK)
#define MRCC_DISABLE_TIM3_CLK()         ((RCC->APB1ENR) &= ~MRCC_APB1_TIM3_MSK)
#define MRCC_DISABLE_TIM2_CLK()         ((RCC->APB1ENR) &= ~MRCC_APB1_TIM2_MSK)
#define MRCC_DISABLE_TIM1_CLK()         ((RCC->APB2ENR) &= ~MRCC_APB2_TIM1_MSK)

#define MRCC_RESET_TIM11_CLK()          ((RCC->APB2RSTR) |= MRCC_APB2_TIM11_MSK)
#define MRCC_RESET_TIM10_CLK()          ((RCC->APB2RSTR) |= MRCC_APB2_TIM10_MSK)
#define MRCC_RESET_TIM9_CLK()           ((RCC->APB2RSTR) |= MRCC_APB2_TIM9_MSK)
#define MRCC_RESET_TIM5_CLK()           ((RCC->APB1RSTR) |= MRCC_APB1_TIM5_MSK)
#define MRCC_RESET_TIM4_CLK()           ((RCC->APB1RSTR) |= MRCC_APB1_TIM4_MSK)
#define MRCC_RESET_TIM3_CLK()           ((RCC->APB1RSTR) |= MRCC_APB1_TIM3_MSK)
#define MRCC_RESET_TIM2_CLK()           ((RCC->APB1RSTR) |= MRCC_APB1_TIM2_MSK)
#define MRCC_RESET_TIM1_CLK()           ((RCC->APB2RSTR) |= MRCC_APB2_TIM1_MSK)
/***/


/* I2C CLK */
#define MRCC_ENABLE_I2C3_CLK()          ((RCC->APB1ENR) |= MRCC_APB1_I2C3_MSK)      
#define MRCC_ENABLE_I2C2_CLK()          ((RCC->APB1ENR) |= MRCC_APB1_I2C2_MSK)
#define MRCC_ENABLE_I2C1_CLK()          ((RCC->APB1ENR) |= MRCC_APB1_I2C1_MSK)

#define MRCC_DISABLE_I2C3_CLK()         ((RCC->APB1ENR) &= ~MRCC_APB1_I2C3_MSK)
#define MRCC_DISABLE_I2C2_CLK()         ((RCC->APB1ENR) &= ~MRCC_APB1_I2C2_MSK)
#define MRCC_DISABLE_I2C1_CLK()         ((RCC->APB1ENR) &= ~MRCC_APB1_I2C1_MSK)

#define MRCC_RESET_I2C3_CLK()           ((RCC->APB1RSTR) |= MRCC_APB1_I2C3_MSK)
#define MRCC_RESET_I2C2_CLK()           ((RCC->APB1RSTR) |= MRCC_APB1_I2C2_MSK)
#define MRCC_RESET_I2C1_CLK()           ((RCC->APB1RSTR) |= MRCC_APB1_I2C1_MSK)
/***/


/* SPI CLK */
#define MRCC_ENABLE_SPI4_CLK()          ((RCC->APB2ENR) |= MRCC_APB2_SPI4_MSK)
#define MRCC_ENABLE_SPI3_CLK()          ((RCC->APB1ENR) |= MRCC_APB1_SPI3_MSK)
#define MRCC_ENABLE_SPI2_CLK()          ((RCC->APB1ENR) |= MRCC_APB1_SPI2_MSK)
#define MRCC_ENABLE_SPI1_CLK()          ((RCC->APB2ENR) |= MRCC_APB2_SPI1_MSK)

#define MRCC_DISABLE_SPI4_CLK()         ((RCC->APB2ENR) &= ~MRCC_APB2_SPI4_MSK)
#define MRCC_DISABLE_SPI3_CLK()         ((RCC->APB1ENR) &= ~MRCC_APB1_SPI3_MSK)
#define MRCC_DISABLE_SPI2_CLK()         ((RCC->APB1ENR) &= ~MRCC_APB1_SPI2_MSK)
#define MRCC_DISABLE_SPI1_CLK()         ((RCC->APB2ENR) &= ~MRCC_APB2_SPI1_MSK)

#define MRCC_RESET_SPI4_CLK()           ((RCC->APB2RSTR) |= MRCC_APB2_SPI4_MSK)
#define MRCC_RESET_SPI3_CLK()           ((RCC->APB1RSTR) |= MRCC_APB1_SPI3_MSK)   
#define MRCC_RESET_SPI2_CLK()           ((RCC->APB1RSTR) |= MRCC_APB1_SPI2_MSK)   
#define MRCC_RESET_SPI1_CLK()           ((RCC->APB2RSTR) |= MRCC_APB2_SPI1_MSK)   
/***/


/* GPIO CLK */
#define MRCC_ENABLE_GPIOA_CLK()     ((RCC->AHB1ENR) |= MRCC_AHB1_GPIOA_MSK)
#define MRCC_ENABLE_GPIOB_CLK()     ((RCC->AHB1ENR) |= MRCC_AHB1_GPIOB_MSK)
#define MRCC_ENABLE_GPIOC_CLK()     ((RCC->AHB1ENR) |= MRCC_AHB1_GPIOC_MSK)
#define MRCC_ENABLE_GPIOD_CLK()     ((RCC->AHB1ENR) |= MRCC_AHB1_GPIOD_MSK)
#define MRCC_ENABLE_GPIOE_CLK()     ((RCC->AHB1ENR) |= MRCC_AHB1_GPIOE_MSK)
#define MRCC_ENABLE_GPIOH_CLK()     ((RCC->AHB1ENR) |= MRCC_AHB1_GPIOH_MSK)

#define MRCC_DISABLE_GPIOA_CLK()    ((RCC->AHB1ENR) &= ~MRCC_AHB1_GPIOA_MSK)    
#define MRCC_DISABLE_GPIOB_CLK()    ((RCC->AHB1ENR) &= ~MRCC_AHB1_GPIOB_MSK)
#define MRCC_DISABLE_GPIOC_CLK()    ((RCC->AHB1ENR) &= ~MRCC_AHB1_GPIOC_MSK)
#define MRCC_DISABLE_GPIOD_CLK()    ((RCC->AHB1ENR) &= ~MRCC_AHB1_GPIOD_MSK)
#define MRCC_DISABLE_GPIOE_CLK()    ((RCC->AHB1ENR) &= ~MRCC_AHB1_GPIOE_MSK)
#define MRCC_DISABLE_GPIOH_CLK()    ((RCC->AHB1ENR) &= ~MRCC_AHB1_GPIOH_MSK)

#define MRCC_RESET_GPIOA_CLK()      ((RCC->AHB1RSTR) |= MRCC_AHB1_GPIOA_MSK)
#define MRCC_RESET_GPIOB_CLK()      ((RCC->AHB1RSTR) |= MRCC_AHB1_GPIOB_MSK)
#define MRCC_RESET_GPIOC_CLK()      ((RCC->AHB1RSTR) |= MRCC_AHB1_GPIOC_MSK)
#define MRCC_RESET_GPIOD_CLK()      ((RCC->AHB1RSTR) |= MRCC_AHB1_GPIOD_MSK)
#define MRCC_RESET_GPIOE_CLK()      ((RCC->AHB1RSTR) |= MRCC_AHB1_GPIOE_MSK)
#define MRCC_RESET_GPIOH_CLK()      ((RCC->AHB1RSTR) |= MRCC_AHB1_GPIOH_MSK)
/***/


SRV_enuErrorStatus_t MRCC_enuSetSysClkSrc(MRCC_enuSYSCLK_t copy_enuSysClkSrc);
SRV_enuErrorStatus_t MRCC_enuGetSysClkSrc(uint32_t* ptr_uint32SysClkSrc);
SRV_enuErrorStatus_t MRCC_enuSetPLLConfig(MRCC_enuPLLConfig_t copy_enuPLLConfig);

/* POSTPONED FOR NOW
SRV_enuErrorStatus_t MRCC_enuSetCSSConfig(); 
SRV_enuErrorStatus_t MRCC_enuSetPLLI2SConfig(); 
SRV_enuErrorStatus_t MRCC_enuSetMCOConfig(); 
SRV_enuErrorStatus_t MRCC_enuSetRTCConfig(); 
SRV_enuErrorStatus_t MRCC_enuSetPLLSpreadSpectrumConfig(); 
SRV_enuErrorStatus_t MRCC_enuSetTimerPrescaler();
*/

SRV_enuErrorStatus_t MRCC_enuSetPeripheralBusesPrescaler(MRCC_enuPeriphBusPresConfig_t copy_enuBusPrescalers);


SRV_enuErrorStatus_t MRCC_enuClrAllClkIntFlags(MRCC_enuClrIntRdyFlags_t copy_enuIntFlags); 


#define MRCC_CLR_RST_FLAGS()    

#define MRCC_IS_LSE_READY()
#define MRCC_IS_LSI_READY()
#define MRCC_IS_PLL_READY()
#define MRCC_IS_PLLI2S_READY()
#define MRCC_IS_HSE_READY()
#define MRCC_IS_HSI_READY()

#define MRCC_LSE_ENABLE()
#define MRCC_LSI_ENABLE()
#define MRCC_PLL_ENABLE()
#define MRCC_PLLI2S_ENABLE()
#define MRCC_HSE_ENABLE()
#define MRCC_HSI_ENABLE()

#define MRCC_LSE_DISABLE()
#define MRCC_LSI_DISABLE()
#define MRCC_PLL_DISABLE()
#define MRCC_PLLI2S_DISABLE()
#define MRCC_HSE_DISABLE()
#define MRCC_HSI_DISABLE()

#define MRCC_LSE_BYPASS()

#define MRCC_BACKUP_DOMAIN_RESET()

#define MRCC_IS_PWR_RESET_FLAG_RAISED()
#define MRCC_IS_WWDG_RESET_FLAG_RAISED()
#define MRCC_IS_IWDG_RESET_FLAG_RAISED()
#define MRCC_IS_POR_RESET_FLAG_RAISED()
#define MRCC_IS_PIN_RESET_FLAG_RAISED()
#define MRCC_IS_BOR_RESET_FLAG_RAISED()

#define MRCC_IS_CSS_INT_FLAG_RAISED()
#define MRCC_IS_PLL_INT_FLAG_RAISED()
#define MRCC_IS_PLLI2S_INT_FLAG_RAISED()
#define MRCC_IS_HSE_INT_FLAG_RAISED()
#define MRCC_IS_HSI_INT_FLAG_RAISED()
#define MRCC_IS_LSE_INT_FLAG_RAISED()
#define MRCC_IS_LSI_INT_FLAG_RAISED()


#define MRCC_ENABLE_CSS_INT()
#define MRCC_ENABLE_PLL_INT()
#define MRCC_ENABLE_PLLI2S_INT()
#define MRCC_ENABLE_HSE_INT()
#define MRCC_ENABLE_HSI_INT()
#define MRCC_ENABLE_LSE_INT()
#define MRCC_ENABLE_LSI_INT()

#define MRCC_DISABLE_CSS_INT()
#define MRCC_DISABLE_PLL_INT()
#define MRCC_DISABLE_PLLI2S_INT()
#define MRCC_DISABLE_HSE_INT()
#define MRCC_DISABLE_HSI_INT()
#define MRCC_DISABLE_LSE_INT()
#define MRCC_DISABLE_LSI_INT()




#endif