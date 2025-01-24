/**
 * @file RCC.c
 * @brief This file contains the implementation of RCC (Reset and Clock Control) driver functions.
 */

#include "std_types.h"
#include "error_status.h"
#include "RCC_Reg.h"
#include "RCC_PBCFG.h"
#include "RCC.h"

/** @defgroup RCC_Timeout_Configuration 
 *  @brief Macros related to RCC timeout settings.
 *  @{ */
#define MRCC_CLK_WAIT_TIMEOUT   1000 /**< Timeout value for RCC operations */
/** @} */

/** @defgroup RCC_Clock_Source_Masks
 *  @brief Macros for masking and configuring system clock sources.
 *  @{ */
#define RCC_SYS_CLK_SET_MASK                (0xFFFFFFFCUL) /**< Mask for setting the system clock source */
#define RCC_SYS_CLK_GET_MASK                (0xFFFFFFF3UL) /**< Mask for getting the system clock source */
/** @} */

/** @defgroup RCC_Ready_Masks
 *  @brief Macros for checking readiness of various clocks.
 *  @{ */
#define RCC_PLL_CLK_READY_MASK              (0xFDFFFFFFULL) /**< Mask for PLL clock readiness */
#define RCC_PLLI2S_CLK_READY_MASK           (0xF7FFFFFFULL) /**< Mask for PLLI2S clock readiness */
#define RCC_HSE_CLK_READY_MASK              (0xFFFDFFFFULL) /**< Mask for HSE clock readiness */
#define RCC_HSI_CLK_READY_MASK              (0x00000001ULL) /**< Mask for HSI clock readiness */
#define RCC_CSS_EN_MASK                     (0xFFF7FFFFULL) /**< Mask for CSS enable */
/** @} */

/** @defgroup RCC_PLL_Masks
 *  @brief Macros for configuring and enabling PLL.
 *  @{ */
#define RCC_PLL_MASK                        (0xF0BC8000ULL) /**< Mask for PLL configuration */
#define RCC_PLL_EN_MASK                     (0xFEFFFFFFULL) /**< Mask for enabling PLL */
/** @} */

/** @defgroup RCC_Bus_Prescaler_Masks
 *  @brief Macros for configuring bus prescalers.
 *  @{ */
#define RCC_BUS_PRESCALER_MASK              (0xFFFF030FULL) /**< Mask for bus prescaler configuration */
#define RCC_AHB1_PERIPHERAL_CLK_MASK        (0xFF9FEF60ULL) /**< Mask for AHB1 peripheral clock configuration */
#define RCC_AHB2_PERIPHERAL_CLK_MASK        (0x00000080ULL) /**< Mask for AHB2 peripheral clock configuration */
#define RCC_APB1_PERIPHERAL_CLK_MASK        (0xEF1D37F0ULL) /**< Mask for APB1 peripheral clock configuration */
#define RCC_APB2_PERIPHERAL_CLK_MASK        (0xFFF886CEULL) /**< Mask for APB2 peripheral clock configuration */
/** @} */

/** @defgroup RCC_Validation_Macros
 *  @brief Macros for validating various RCC parameters.
 *  @{ */
#define IS_INVALID_ALL_BUS_PRESCALER(PRE)   ((PRE & RCC_BUS_PRESCALER_MASK) != 0x00UL) /**< Checks if the bus prescaler configuration is invalid */
#define IS_PLL_CLK_SRC_HSI(SRC)             (((SRC >> 22) & (1U)) == 0x00UL) /**< Checks if the PLL clock source is HSI */
#define IS_PLL_CLK_SRC_HSE(SRC)             (((SRC >> 22) & (1U)) == 0x01UL) /**< Checks if the PLL clock source is HSE */
#define IS_VALID_PLL_CONFIG(CFG)            ((CFG) & MRCC_MASK_VERIFY_PLL_CFG) /**< Checks if the PLL configuration is valid */
#define IS_NULL_PTR(PTR)                    (PTR == NULL) /**< Checks if a pointer is null */
#define IS_VALID_SYS_CLK_SRC(SYSCLK)        ((SYSCLK != MRCC_SYS_CLK_HSI) && (SYSCLK != MRCC_SYS_CLK_HSE) && (SYSCLK != MRCC_SYS_CLK_PLL)) /**< Checks if the system clock source is valid */
#define IS_PLL_ENABLED()                    (((RCC->CR) >> 24) & 0x01U)
/** @} */



#define MRCC_MASK_VERIFY_CLK_CTRL       (0xF0FCFFFCUL)
#define IS_INVALID_CLK(CLK)     (MRCC_MASK_VERIFY_CLK_CTRL & (CLK))

#define MRCC_MASK_VERIFY_CLK_CFG        (0xFCUL)
#define IS_INVALID_CLK_CFG(CFG)         (MRCC_MASK_VERIFY_CLK_CFG & (CFG))

SRV_enuErrorStatus_t MRCC_enuSetClkSrcState(MRCC_enuClkType_t copy_enuClk, MRCC_enuClkSrcState_t copy_enuClkSrcState)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;
    uint16_t local_uint16Timeout = MRCC_CLK_WAIT_TIMEOUT;

    if (IS_INVALID_CLK(copy_enuClk))
    {
        ret_enuErrorStatus = RCC_INV_CLK;
    }
    else if (IS_INVALID_CLK_CFG(copy_enuClkSrcState))
    {
        ret_enuErrorStatus = RCC_INV_CFG;
    }
    else 
    {
        /* clear before setting */
        RCC->CR &= (~copy_enuClk);

        /* if state is on -> set it : else -> leave it 0 */
        if (copy_enuClkSrcState == MRCC_CLK_SRC_ENABLE)
        {
            RCC->CR |= (copy_enuClk);
        }
        else {}

        /* wait until clock is ready */
        while ((RCC->CR & (~copy_enuClk)) & (local_uint16Timeout--));

        /* if clk is supposed to be on && timeout reached 0 -> return error status */
        if ((local_uint16Timeout == 0) && (RCC->CR & copy_enuClk))
        {
            ret_enuErrorStatus = RCC_TIMEOUT;
        }
        else {}
    }

    return ret_enuErrorStatus;
}


#define MRCC_MASK_VERIFY_SYS_CLK_SRC    (0xFCUL)
#define IS_INVALID_SYS_CLK_SRC(CLK)     (MRCC_MASK_VERIFY_SYS_CLK_SRC & (CLK))

/**
 * @brief Sets the system clock source.
 *
 * @param copy_enuSysClkSrc Desired system clock source.
 * @return SRV_enuErrorStatus_t Error status of the operation.
 */
SRV_enuErrorStatus_t MRCC_enuSetSysClkSrc(MRCC_enuSysClkSrc_t copy_enuSysClkSrc)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;;

    if (IS_INVALID_SYS_CLK_SRC(copy_enuSysClkSrc))
    {
        ret_enuErrorStatus = RCC_INV_CLK_SRC;
    }
    else
    {
        /* clear before setting */
        RCC->CFGR &= (~copy_enuSysClkSrc);

        /* Set system clock source */
        RCC->CFGR |= copy_enuSysClkSrc;
    }

    return ret_enuErrorStatus;
}

#define MRCC_MASK_GET_SYS_CLK_SRC       (0xFFFFFFF3UL)

/**
 * @brief Gets the current system clock source.
 *
 * @param ptr_uint32SysClkSrc Pointer to store the current system clock source.
 * @return SRV_enuErrorStatus_t Error status of the operation.
 */
SRV_enuErrorStatus_t MRCC_enuGetSysClkSrc(uint32_t* ptr_uint32SysClkSrc)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;;

    if (IS_NULL_PTR(ptr_uint32SysClkSrc))
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else
    {
        (*ptr_uint32SysClkSrc) = ((RCC->CFGR & ~MRCC_MASK_GET_SYS_CLK_SRC) >> 2);
    }

    return ret_enuErrorStatus;
}


SRV_enuErrorStatus_t MRCC_enuSetPeripheralClkState(uint64_t copy_uint64Peripheral, MRCC_enuPeripheralClkState_t copy_enuPeripheralClkState)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;
    uint16_t local_uint16Timeout = MRCC_CLK_WAIT_TIMEOUT;

    if (IS_INVALID_CLK_CFG(copy_enuPeripheralClkState))
    {
        ret_enuErrorStatus = RCC_INV_CFG;
    }
    else
    {
        switch (copy_uint64Peripheral >> 32)
        {
            case MRCC_AHB1_BUS:
                if (copy_enuPeripheralClkState == MRCC_PERIPHERAL_CLK_DISABLE)
                {
                    RCC->AHB1ENR &= ((uint32_t)((~copy_uint64Peripheral) & 0x00000000FFFFFFFFULL));
                }
                else if (copy_enuPeripheralClkState == MRCC_PERIPHERAL_CLK_ENABLE)
                {
                    RCC->AHB1ENR |= ((uint32_t)(copy_uint64Peripheral & 0x00000000FFFFFFFFULL));
                }
                else if (copy_enuPeripheralClkState == MRCC_PERIPHERAL_CLK_RESET)
                {
                    RCC->AHB1RSTR |= ((uint32_t)(copy_uint64Peripheral & 0x00000000FFFFFFFFULL));
                }
                else if (copy_enuPeripheralClkState == MRCC_PERIPHERAL_CLK_LP_EN)
                {
                    RCC->AHB1LPENR |= ((uint32_t)(copy_uint64Peripheral & 0x00000000FFFFFFFFULL));
                }
                else {}
            break;
    
            case MRCC_AHB2_BUS:
                if (copy_enuPeripheralClkState == MRCC_PERIPHERAL_CLK_DISABLE)
                {
                    RCC->AHB2ENR &= ((uint32_t)((~copy_uint64Peripheral) & 0x00000000FFFFFFFFULL));
                }
                else if (copy_enuPeripheralClkState == MRCC_PERIPHERAL_CLK_ENABLE)
                {
                    RCC->AHB2ENR |= ((uint32_t)(copy_uint64Peripheral & 0x00000000FFFFFFFFULL));
                }
                else if (copy_enuPeripheralClkState == MRCC_PERIPHERAL_CLK_RESET)
                {
                    RCC->AHB2RSTR |= ((uint32_t)(copy_uint64Peripheral & 0x00000000FFFFFFFFULL));
                }
                else if (copy_enuPeripheralClkState == MRCC_PERIPHERAL_CLK_LP_EN)
                {
                    RCC->AHB2LPENR |= ((uint32_t)(copy_uint64Peripheral & 0x00000000FFFFFFFFULL));
                }
                else {}
            break;
    
            case MRCC_APB1_BUS:
                if (copy_enuPeripheralClkState == MRCC_PERIPHERAL_CLK_DISABLE)
                {
                    RCC->APB1ENR &= ((uint32_t)((~copy_uint64Peripheral) & 0x00000000FFFFFFFFULL));
                }
                else if (copy_enuPeripheralClkState == MRCC_PERIPHERAL_CLK_ENABLE)
                {
                    RCC->APB1ENR |= ((uint32_t)(copy_uint64Peripheral & 0x00000000FFFFFFFFULL));
                }
                else if (copy_enuPeripheralClkState == MRCC_PERIPHERAL_CLK_RESET)
                {
                    RCC->APB1RSTR |= ((uint32_t)(copy_uint64Peripheral & 0x00000000FFFFFFFFULL));
                }
                else if (copy_enuPeripheralClkState == MRCC_PERIPHERAL_CLK_LP_EN)
                {
                    RCC->APB1LPENR |= ((uint32_t)(copy_uint64Peripheral & 0x00000000FFFFFFFFULL));
                }
                else {}
            break;
    
            case MRCC_APB2_BUS:
                if (copy_enuPeripheralClkState == MRCC_PERIPHERAL_CLK_DISABLE)
                {
                    RCC->APB2ENR &= ((uint32_t)((~copy_uint64Peripheral) & 0x00000000FFFFFFFFULL));
                }
                else if (copy_enuPeripheralClkState == MRCC_PERIPHERAL_CLK_ENABLE)
                {
                    RCC->APB2ENR |= ((uint32_t)(copy_uint64Peripheral & 0x00000000FFFFFFFFULL));
                }
                else if (copy_enuPeripheralClkState == MRCC_PERIPHERAL_CLK_RESET)
                {
                    RCC->APB2RSTR |= ((uint32_t)(copy_uint64Peripheral & 0x00000000FFFFFFFFULL));
                }
                else if (copy_enuPeripheralClkState == MRCC_PERIPHERAL_CLK_LP_EN)
                {
                    RCC->APB2LPENR |= ((uint32_t)(copy_uint64Peripheral & 0x00000000FFFFFFFFULL));
                }
                else {}
            break;
    
            default:
                ret_enuErrorStatus = INV_ARG;
        }
    }

    return ret_enuErrorStatus;

}


#define MRCC_MASK_VERIFY_PLL_CFG    (0xF0BC8000ULL)
#define IS_VALID_PLL_CONFIG(CFG)    ((CFG) & MRCC_MASK_VERIFY_PLL_CFG) /**< Checks if the PLL configuration is valid */

#define MRCC_MASK_ENABLE_PLL        (0x01UL << 24U)

#define MRCC_MASK_VERIFY_PLL_RDY    (0xFDFFFFFFUL)

/**
 * @brief Configures the PLL (Phase Locked Loop).
 *
 * @param copy_enuPLLConfig Desired PLL configuration.
 * @return SRV_enuErrorStatus_t Error status of the operation.
 */
SRV_enuErrorStatus_t MRCC_enuConfigPLL(MRCC_enuPLLConfig_t copy_enuPLLConfig)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    uint16_t local_uint16WaitTimeout = MRCC_CLK_WAIT_TIMEOUT;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;;

    if (IS_PLL_ENABLED())
    {
        ret_enuErrorStatus = RCC_PLL_CFG_WHILE_EN;
    }
    else if (IS_VALID_PLL_CONFIG(copy_enuPLLConfig))
    {
        ret_enuErrorStatus = INV_ARG;
    }
    else
    {
        RCC->PLLCFGR &= (~copy_enuPLLConfig);

        RCC->PLLCFGR |= (copy_enuPLLConfig);

        RCC->CR |= MRCC_MASK_ENABLE_PLL;

        while ((RCC->CR & (~MRCC_MASK_VERIFY_PLL_RDY)) && local_uint16WaitTimeout--);

        if ((local_uint16WaitTimeout == 0) && (RCC->CR & MRCC_MASK_VERIFY_PLL_RDY))
        {
            ret_enuErrorStatus = RCC_TIMEOUT;
        }
    }

    return ret_enuErrorStatus;
}

/**
 * @brief Sets the peripheral bus prescaler configuration.
 *
 * @param copy_enuBusPrescalers Desired bus prescaler configuration.
 * @return SRV_enuErrorStatus_t Error status of the operation.
 */
SRV_enuErrorStatus_t MRCC_enuSetPeripheralBusesPrescaler(MRCC_enuPeriphBusPresConfig_t copy_enuBusPrescalers)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;

    if (IS_INVALID_ALL_BUS_PRESCALER(copy_enuBusPrescalers))
    {
        ret_enuErrorStatus = INV_ARG;
    }
    else
    {
        RCC->CFGR = ((RCC->CFGR & RCC_BUS_PRESCALER_MASK) | copy_enuBusPrescalers);
    }

    return ret_enuErrorStatus;
}
