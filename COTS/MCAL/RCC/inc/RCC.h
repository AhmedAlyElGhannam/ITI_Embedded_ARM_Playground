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
    PLL_M   = (MRCC_PLL_M_PRESCALER << 0)  & (0x0000003FULL),
    PLL_N   = (MRCC_PLL_N_PRESCALER << 6)  & (0x00007FC0ULL),
    PLL_P   = (MRCC_PLL_P_PRESCALER << 16) & (0x00030000ULL),
    PLL_Q   = (MRCC_PLL_Q_PRESCALER << 24) & (0x0F000000ULL),
    PLL_SRC = (MRCC_PLL_SRC << 22)         & (0x00400000ULL),
    PLL_COMBINED_PRE = (PLL_M | PLL_N | PLL_P | PLL_Q | PLL_SRC),  /* use this value in PLL_config function */
} MRCC_enuPLLConfig_t;

// typedef enum
// {
//     ENABLE = 1,
//     DISABLE = 0,
//     RESET = 1,
// } MRCC_enuPeripheralClkConfig_t;

// typedef enum 
// {
//     AHB1_GPIOA = (1 << 0)  & (0x0000001ULL),
// } MRCC_enuAHB1ClkConfig_t;


#define COLLECT_PLL_CONFIGS(M, N, P, Q, PLL_SRC)    ((uint32_t)(((Q) << 24) | ((PLL_SRC) << 22) | ((P) << 16) | ((N) << 6) | ((M) < 0)))


SRV_enuErrorStatus_t MRCC_enuSelectSysClkSource();

SRV_enuErrorStatus_t MRCC_enuCheckSysClkSource();

SRV_enuErrorStatus_t MRCC_enuCheckPLLReadyStatus();
SRV_enuErrorStatus_t MRCC_enuCheckPLLI2SReadyStatus();
SRV_enuErrorStatus_t MRCC_enuCheckHSEReadyStatus();
SRV_enuErrorStatus_t MRCC_enuCheckHSIReadyStatus();

SRV_enuErrorStatus_t MRCC_enuConfigCSS();
SRV_enuErrorStatus_t MRCC_enuConfigPLL();
SRV_enuErrorStatus_t MRCC_enuConfigPLLI2S(); // source && prescalers
SRV_enuErrorStatus_t MRCC_enuConfigMCO(); // source && prescaler
SRV_enuErrorStatus_t MRCC_enuConfigBusPrescaler(); // prescaler for APB1/2 && AHB1/2 (ignore the one for USBOTG)

SRV_enuErrorStatus_t MRCC_enuResetPeripheralClk(); // mask for all peripheral resets
SRV_enuErrorStatus_t MRCC_enuEnablePeripheralClk(); // mask for all peripheral resets


SRV_enuErrorStatus_t MRCC_enuClrClkInterruptFlag(); // mask for all flags that has to be cleared

SRV_enuErrorStatus_t MRCC_enuEnableCSSInterrupt();
SRV_enuErrorStatus_t MRCC_enuEnablePLLInterrupt();
SRV_enuErrorStatus_t MRCC_enuEnablePLLI2SInterrupt();
SRV_enuErrorStatus_t MRCC_enuEnableHSEInterrupt();
SRV_enuErrorStatus_t MRCC_enuEnableHSIInterrupt();
SRV_enuErrorStatus_t MRCC_enuEnableLSEInterrupt();
SRV_enuErrorStatus_t MRCC_enuEnableLSIInterrupt();

SRV_enuErrorStatus_t MRCC_enuCheckCSSInterruptFlagStatus();
SRV_enuErrorStatus_t MRCC_enuCheckPLLInterruptFlagStatus();
SRV_enuErrorStatus_t MRCC_enuCheckPLLI2SInterruptFlagStatus();
SRV_enuErrorStatus_t MRCC_enuCheckHSEInterruptFlagStatus();
SRV_enuErrorStatus_t MRCC_enuCheckHSIInterruptFlagStatus();
SRV_enuErrorStatus_t MRCC_enuCheckLSEInterruptFlagStatus();
SRV_enuErrorStatus_t MRCC_enuCheckLSIInterruptFlagStatus();

SRV_enuErrorStatus_t MRCC_enuConfigRTC();

SRV_enuErrorStatus_t MRCC_enuEnableLSI();
SRV_enuErrorStatus_t MRCC_enuCheckLSIReadyStatus();

SRV_enuErrorStatus_t MRCC_enuEnableLSE();
SRV_enuErrorStatus_t MRCC_enuCheckLSEReadyStatus();

SRV_enuErrorStatus_t MRCC_enuConfigurePLLSpreadSpectrum();

SRV_enuErrorStatus_t MRCC_enuClrResetFlags();

SRV_enuErrorStatus_t MRCC_enuCheckLPWRResetFlagStatus();
SRV_enuErrorStatus_t MRCC_enuCheckWWDGResetFlagStatus();
SRV_enuErrorStatus_t MRCC_enuCheckIWDGResetFlagStatus();
SRV_enuErrorStatus_t MRCC_enuCheckPORResetFlagStatus();
SRV_enuErrorStatus_t MRCC_enuCheckPINResetFlagStatus();
SRV_enuErrorStatus_t MRCC_enuCheckBORResetFlagStatus();




#endif