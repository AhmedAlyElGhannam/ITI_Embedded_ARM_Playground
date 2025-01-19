#include "RCC.h"
#include "RCC_Reg.h"


/*
    CFGR -> to set clock source and prescaler of:
            MCO1 && MCO2
         -> to set the prescaler of:
            AHB && APB
         -> clock selection of I2S
         -> select system clock && read the selected system clock ###
*/

/* MASKS (Should not be visible/accessible to user) */
#define RCC_SYS_CLK_SET_MASK                (0xFFFFFFFCULL)
#define RCC_SYS_CLK_GET_MASK                (0xFFFFFFF3ULL)
#define RCC_PLL_CLK_READY_MASK              (0xFDFFFFFFULL)
#define RCC_PLLI2S_CLK_READY_MASK           (0xF7FFFFFFULL)
#define RCC_HSE_CLK_READY_MASK              (0xFFFDFFFFULL)
#define RCC_HSI_CLK_READY_MASK              (0x00000001ULL)
#define RCC_CSS_EN_MASK                     (0xFFF7FFFFULL)
#define RCC_PLL_MASK                        (0xF0BC8000ULL)
#define RCC_PLL_EN_MASK                     (0xFEFFFFFFULL)
#define RCC_BUS_PRESCALER_MASK              (0xFFFF030FULL)
#define RCC_AHB1_PERIPHERAL_CLK_MASK        (0xFF9FEF60ULL)
#define RCC_AHB2_PERIPHERAL_CLK_MASK        (0x00000080ULL)
#define RCC_APB1_PERIPHERAL_CLK_MASK        (0xEF1D37F0ULL)
#define RCC_APB2_PERIPHERAL_CLK_MASK        (0xFFF886CEULL)


SRV_enuErrorStatus_t MRCC_enuSelectSysClkSource(MRCC_enuSYSCLK_t copy_enuSysClkSrc)
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if ((copy_enuSysClkSrc != MRCC_SYS_CLK_HSI) && (copy_enuSysClkSrc != MRCC_SYS_CLK_HSE) && (copy_enuSysClkSrc != MRCC_SYS_CLK_PLL))
    {
        ret_enuErrStatus = RCC_INV_CLK_SRC;
    }
    else
    {
        /* set sys clk source */
        RCC->CFGR = (RCC->CFGR & RCC_SYS_CLK_SET_MASK) | copy_enuSysClkSrc;
    }

    return ret_enuErrStatus;
}

SRV_enuErrorStatus_t MRCC_enuCheckSysClkSource(uint32_t* ptr_uint32UsedSysClk)
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if (ptr_uint32UsedSysClk == NULL)
    {
        ret_enuErrStatus = NULL_PTR;
    }
    else
    {
        #define SYS_CLK_SRC_POS   (2) 

        (*ptr_uint32UsedSysClk) = ((RCC->CFGR & ~RCC_SYS_CLK_GET_MASK) >> SYS_CLK_SRC_POS);
    }

    return ret_enuErrStatus;
}

SRV_enuErrorStatus_t MRCC_enuCheckPLLClkReadyStatus(uint32_t* ptr_uint32PLLClkStatus)
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if (ptr_uint32PLLClkStatus == NULL)
    {
        ret_enuErrStatus = NULL_PTR;
    }
    else
    {
        #define PLL_CLK_READY_POS   (25) 

        (*ptr_uint32PLLClkStatus) = ((RCC->CR & ~RCC_PLL_CLK_READY_MASK) >> PLL_CLK_READY_POS);
    }

    return ret_enuErrStatus;
}


SRV_enuErrorStatus_t MRCC_enuCheckPLLI2SClkReadyStatus(uint32_t* ptr_uint32PLLI2SClkStatus)
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if (ptr_uint32PLLI2SClkStatus == NULL)
    {
        ret_enuErrStatus = NULL_PTR;
    }
    else
    {
        #define PLLI2S_CLK_READY_POS   (27) 

        (*ptr_uint32PLLI2SClkStatus) = ((RCC->CR & ~RCC_PLLI2S_CLK_READY_MASK) >> PLLI2S_CLK_READY_POS);
    }

    return ret_enuErrStatus;
}

SRV_enuErrorStatus_t MRCC_enuCheckHSEReadyStatus(uint32_t* ptr_uint32HSEClkStatus)
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if (ptr_uint32HSEClkStatus == NULL)
    {
        ret_enuErrStatus = NULL_PTR;
    }
    else
    {
        #define HSE_CLK_READY_POS   (17)

        (*ptr_uint32HSEClkStatus) = ((RCC->CR & ~RCC_HSE_CLK_READY_MASK) >> HSE_CLK_READY_POS);
    }

    return ret_enuErrStatus;
}

SRV_enuErrorStatus_t MRCC_enuCheckHSIReadyStatus(uint32_t* ptr_uint32HSIClkStatus)
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if (ptr_uint32HSIClkStatus == NULL)
    {
        ret_enuErrStatus = NULL_PTR;
    }
    else
    {
        #define HSI_CLK_READY_POS   (1)

        (*ptr_uint32HSIClkStatus) = ((RCC->CR & ~RCC_HSI_CLK_READY_MASK) >> HSI_CLK_READY_POS);
    }

    return ret_enuErrStatus;
}

SRV_enuErrorStatus_t MRCC_enuConfigCSS(SRV_enuConfig_t copy_enuCSSConfig)
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if ((copy_enuCSSConfig != CSS_ENABLE) && (copy_enuCSSConfig != CSS_DISABLE))
    {
        ret_enuErrStatus = INV_ARG;
    }
    else
    {
        RCC->CR = (RCC->CR & RCC_CSS_EN_MASK) | copy_enuCSSConfig;
    }

    return ret_enuErrStatus;
}

#define COLLECT_PLL_CONFIGS(M, N, P, Q, PLL_SRC)    ((uint32_t)(((Q) << 24) | ((PLL_SRC) << 22) | ((P) << 16) | ((N) << 6) | ((M) << 0)))
/* this function takes the input inside the above macro */
SRV_enuErrorStatus_t MRCC_enuConfigPLL(uint32_t copy_PLLConfig)
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if (IS_PLL_ENABLED())
    {
        ret_enuErrStatus = RCC_PLL_CFG_WHILE_EN;
    }
    else if ((copy_PLLConfig && RCC_PLL_MASK) == true)
    {
        ret_enuErrStatus = INV_ARG;
    }
    else /* arguments are valid */
    {
        if ((copy_PLLConfig & (1 << 22)) == 0) /* HSI */
        {
            /* check if it is ready + timeout */
            while (STATUS && TIMEOUT--);

            /* if exited due to timeout -> send error status */
        }
        else if ((copy_PLLConfig & (1 << 22)) == 1) /* HSE */
        {
            /* check if it is ready + timeout */
            while (STATUS && TIMEOUT--);

            /* if exited due to timeout -> send error status */
        }

        /* setting prescalers into register */
        RCC->PLLCFGR_R = ((RCC->PLLCFGR_R & RCC_PLL_MASK) | copy_PLLConfig);

        /* enable PLL */
        RCC->CR = (RCC->CR & RCC_PLL_EN_MASK) | (1 << 24);
    }

    return ret_enuErrStatus;    
}

// #define COLLECT_PLLI2S_CONFIGS(R, N, PLLI2S_SRC)    ((uint32_t)(((R) << 28) | ((PLL_SRC) << 22) | ((P) << 16) | ((N) << 6) | ((M) < 0)))

// SRV_enuErrorStatus_t MRCC_enuConfigPLLI2S() // source && prescalers
// {

// }
// SRV_enuErrorStatus_t MRCC_enuConfigMCO() // source && prescaler
// {

// }

#define COLLECT_BUS_PRESCALERS(APB2, APB1, AHB)    ((uint32_t)(((Q) << 24) | ((PLL_SRC) << 22) | ((P) << 16) | ((N) << 6) | ((M) << 0)))

SRV_enuErrorStatus_t MRCC_enuConfigBusPrescaler(uint32_t copy_uint32BusPrescalers) // prescaler for APB1/2 && AHB1/2 (ignore the one for USBOTG)
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if ((copy_uint32BusPrescalers && RCC_BUS_PRESCALER_MASK) == true)
    {
        ret_enuErrStatus = INV_ARG;
    }
    else /* arguments are valid */
    {
        /* setting prescalers into register */
        RCC->PLLCFGR_R = ((RCC->PLLCFGR_R & RCC_BUS_PRESCALER_MASK) | copy_uint32BusPrescalers);
    }

    return ret_enuErrStatus;
}

#define COLLECT_AHB1_PERPHERALS(GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOH, CRC, DMA1, DMA2)    \
((uint32_t)(((DMA2) << 22) | ((DMA1) << 21) | ((CRC) << 12) | ((GPIOH) << 7) | ((GPIOE) << 4) | ((GPIOD) << 3) | ((GPIOC) << 2) | ((GPIOB) << 1) | ((GPIOA) << 0)))

SRV_enuErrorStatus_t MRCC_enuResetAHB1PeripheralClk(uint32_t copy_uint32AHB1Peripherals) // mask for all peripheral resets
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if ((copy_uint32AHB1Peripherals && RCC_AHB1_PERIPHERAL_CLK_MASK) == true)
    {
        ret_enuErrStatus = INV_ARG;
    }
    else /* arguments are valid */
    {
        /* setting prescalers into register */
        RCC->AHB1RSTR = ((RCC->AHB1RSTR & RCC_AHB1_PERIPHERAL_CLK_MASK) | copy_uint32AHB1Peripherals);
    }

    return ret_enuErrStatus;
}

#define COLLECT_AHB2_PERPHERALS(USBOTG)    \
((uint32_t)(((USBOTG) << 7)))

SRV_enuErrorStatus_t MRCC_enuResetAHB2PeripheralClk(uint32_t copy_uint32AHB2Peripherals) // mask for all peripheral resets
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if ((copy_uint32AHB2Peripherals && RCC_AHB2_PERIPHERAL_CLK_MASK) == true)
    {
        ret_enuErrStatus = INV_ARG;
    }
    else /* arguments are valid */
    {
        /* setting prescalers into register */
        RCC->AHB2RSTR = ((RCC->AHB2RSTR & RCC_AHB2_PERIPHERAL_CLK_MASK) | copy_uint32AHB2Peripherals);
    }

    return ret_enuErrStatus;
}

#define COLLECT_APB1_PERPHERALS(TIM2RST, TIM3RST, TIM4RST, TIM5RST, WWDGRST, SPI2RST, SPI3RST, USART2RST, I2C1RST, IC2RST, I2C3RST, PWRRST)    \
((uint32_t)(((TIM2RST) << 0) | ((TIM3RST) << 1) | ((TIM4RST) << 2) | ((TIM5RST) << 3) | ((WWDGRST) << 11) | ((SPI2RST) << 14) | ((SPI3RST) << 15) | ((USART2RST) << 17) | ((I2C1RST) << 21) | ((IC2RST) << 22) | ((I2C3RST) << 23) | ((PWRRST) << 28)))

SRV_enuErrorStatus_t MRCC_enuResetAPB1PeripheralClk(uint32_t copy_uint32APB1Peripherals) // mask for all peripheral resets
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if ((copy_uint32APB1Peripherals && RCC_APB1_PERIPHERAL_CLK_MASK) == true)
    {
        ret_enuErrStatus = INV_ARG;
    }
    else /* arguments are valid */
    {
        /* setting prescalers into register */
        RCC->APB1RSTR = ((RCC->APB1RSTR & RCC_APB1_PERIPHERAL_CLK_MASK) | copy_uint32APB1Peripherals);
    }

    return ret_enuErrStatus;
}

#define COLLECT_APB2_PERPHERALS(TIM1RST, USART1RST, USART6RST, ADC1RST, SDIORST, SPI1RST, SPI4RST, SYSCFGRST, TIM9RST, TIM10RST, TIM11RST)    \
((uint32_t)(((TIM1RST) << 0) | ((USART1RST) << 4) | ((USART6RST) << 5) | ((ADC1RST) << 8) | ((SDIORST) << 11) | ((SPI1RST) << 12) | ((SPI4RST) << 13) | ((SYSCFGRST) << 14) | ((TIM9RST) << 16) | ((TIM10RST) << 17) | ((TIM11RST) << 18)))

SRV_enuErrorStatus_t MRCC_enuResetAPB2PeripheralClk(uint32_t copy_uint32APB2Peripherals) // mask for all peripheral resets
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if ((copy_uint32APB2Peripherals && RCC_APB2_PERIPHERAL_CLK_MASK) == true)
    {
        ret_enuErrStatus = INV_ARG;
    }
    else /* arguments are valid */
    {
        /* setting prescalers into register */
        RCC->APB2RSTR = ((RCC->APB2RSTR & RCC_APB2_PERIPHERAL_CLK_MASK) | copy_uint32APB2Peripherals);
    }

    return ret_enuErrStatus;
}


SRV_enuErrorStatus_t MRCC_enuEnableAHB1PeripheralClk(uint32_t copy_uint32AHB1Peripherals) // mask for all peripheral resets
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if ((copy_uint32AHB1Peripherals && RCC_AHB1_PERIPHERAL_CLK_MASK) == true)
    {
        ret_enuErrStatus = INV_ARG;
    }
    else /* arguments are valid */
    {
        /* setting prescalers into register */
        RCC->AHB1ENR = ((RCC->AHB1ENR & RCC_AHB1_PERIPHERAL_CLK_MASK) | copy_uint32AHB1Peripherals);
    }

    return ret_enuErrStatus;
}


SRV_enuErrorStatus_t MRCC_enuEnableAHB2PeripheralClk(uint32_t copy_uint32AHB2Peripherals) // mask for all peripheral resets
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if ((copy_uint32AHB2Peripherals && RCC_AHB2_PERIPHERAL_CLK_MASK) == true)
    {
        ret_enuErrStatus = INV_ARG;
    }
    else /* arguments are valid */
    {
        /* setting prescalers into register */
        RCC->AHB2ENR = ((RCC->AHB2ENR & RCC_AHB2_PERIPHERAL_CLK_MASK) | copy_uint32AHB2Peripherals);
    }

    return ret_enuErrStatus;
}

SRV_enuErrorStatus_t MRCC_enuEnableAPB1PeripheralClk(uint32_t copy_uint32APB1Peripherals) // mask for all peripheral resets
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if ((copy_uint32APB1Peripherals && RCC_APB1_PERIPHERAL_CLK_MASK) == true)
    {
        ret_enuErrStatus = INV_ARG;
    }
    else /* arguments are valid */
    {
        /* setting prescalers into register */
        RCC->APB1ENR = ((RCC->APB1ENR & RCC_APB1_PERIPHERAL_CLK_MASK) | copy_uint32APB1Peripherals);
    }

    return ret_enuErrStatus;
}

SRV_enuErrorStatus_t MRCC_enuEnableAPB2PeripheralClk(uint32_t copy_uint32APB2Peripherals) // mask for all peripheral resets
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if ((copy_uint32APB2Peripherals && RCC_APB2_PERIPHERAL_CLK_MASK) == true)
    {
        ret_enuErrStatus = INV_ARG;
    }
    else /* arguments are valid */
    {
        /* setting prescalers into register */
        RCC->APB2ENR = ((RCC->APB2ENR & RCC_APB2_PERIPHERAL_CLK_MASK) | copy_uint32APB2Peripherals);
    }

    return ret_enuErrStatus;
}

SRV_enuErrorStatus_t MRCC_enuEnableAHB1PeripheralClkLowPower(uint32_t copy_uint32AHB1Peripherals) // mask for all peripheral resets
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if ((copy_uint32AHB1Peripherals && RCC_AHB1_PERIPHERAL_CLK_MASK) == true)
    {
        ret_enuErrStatus = INV_ARG;
    }
    else /* arguments are valid */
    {
        /* setting prescalers into register */
        RCC->AHB1LPENR = ((RCC->AHB1LPENR & RCC_AHB1_PERIPHERAL_CLK_MASK) | copy_uint32AHB1Peripherals);
    }

    return ret_enuErrStatus;
}


SRV_enuErrorStatus_t MRCC_enuEnableAHB2PeripheralClkLowPower(uint32_t copy_uint32AHB2Peripherals) // mask for all peripheral resets
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if ((copy_uint32AHB2Peripherals && RCC_AHB2_PERIPHERAL_CLK_MASK) == true)
    {
        ret_enuErrStatus = INV_ARG;
    }
    else /* arguments are valid */
    {
        /* setting prescalers into register */
        RCC->AHB2LPENR = ((RCC->AHB2LPENR & RCC_AHB2_PERIPHERAL_CLK_MASK) | copy_uint32AHB2Peripherals);
    }

    return ret_enuErrStatus;
}

SRV_enuErrorStatus_t MRCC_enuEnableAPB1PeripheralClkLowPower(uint32_t copy_uint32APB1Peripherals) // mask for all peripheral resets
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if ((copy_uint32APB1Peripherals && RCC_APB1_PERIPHERAL_CLK_MASK) == true)
    {
        ret_enuErrStatus = INV_ARG;
    }
    else /* arguments are valid */
    {
        /* setting prescalers into register */
        RCC->APB1LPENR = ((RCC->APB1LPENR & RCC_APB1_PERIPHERAL_CLK_MASK) | copy_uint32APB1Peripherals);
    }

    return ret_enuErrStatus;
}

SRV_enuErrorStatus_t MRCC_enuEnableAPB2PeripheralClkLowPower(uint32_t copy_uint32APB2Peripherals) // mask for all peripheral resets
{
    SRV_enuErrorStatus_t ret_enuErrStatus = ALL_OK;

    if ((copy_uint32APB2Peripherals && RCC_APB2_PERIPHERAL_CLK_MASK) == true)
    {
        ret_enuErrStatus = INV_ARG;
    }
    else /* arguments are valid */
    {
        /* setting prescalers into register */
        RCC->APB2LPENR = ((RCC->APB2LPENR & RCC_APB2_PERIPHERAL_CLK_MASK) | copy_uint32APB2Peripherals);
    }

    return ret_enuErrStatus;
}

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
