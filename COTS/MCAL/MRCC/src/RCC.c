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

/** @defgroup RCC_Masks
 *  @brief Macros for validating various RCC parameters.
 *  @{ */
#define MRCC_MASK_VERIFY_CLK_CTRL       (0xFAFEFFFEUL)
#define MRCC_MASK_VERIFY_CLK_CFG        (0xFCUL)
#define MRCC_MASK_VERIFY_SYS_CLK_SRC    (0xFCUL)
#define MRCC_MASK_GET_SYS_CLK_SRC       (0xFFFFFFF3UL)
#define MRCC_MASK_VERIFY_PLL_CFG    (0xF0BC8000ULL)
#define MRCC_MASK_ENABLE_PLL        (0x01UL << 24U)
#define MRCC_MASK_VERIFY_PLL_RDY    (0xFDFFFFFFUL)
#define MRCC_MASK_VERIFY_PLL_SRC        (0xFEUL)
#define MRCC_MASK_SET_BUS_CLK_PRESCALER     (0xFFFF030FUL)
#define MRCC_MASK_SET_CLK_INT_RDY   (0xFFFFC0FFUL)
#define MRCC_MASK_CLR_CSS_FLAG  (0x00800000UL)
#define MRCC_MASK_CLR_PLLI2S_FLAG  (0x00200000UL)
#define MRCC_MASK_CLR_PLL_FLAG  (0x00100000UL)
#define MRCC_MASK_CLR_HSE_FLAG  (0x00080000UL)
#define MRCC_MASK_CLR_HSI_FLAG  (0x00040000UL)
#define MRCC_MASK_CLR_LSE_FLAG  (0x00020000UL)
#define MRCC_MASK_CLR_LSI_FLAG  (0x00010000UL)
#define MRCC_MASK_RTC_STATE    (0xFFFF7FFFUL)
#define MRCC_MASK_CLR_RST_FLAGS (0x01000000UL)
#define MRCC_MASK_VERIFY_HSE_BYPASS_CONFIG  (0xFFFBFFFFUL)
#define MRCC_MASK_VERIFY_LSE_BYPASS_CONFIG  (0xFFFFFFFBUL)
#define MRCC_MASK_VERIFY_CSS_CONFIG    (0xFFF7FFFFUL)
#define MRCC_MASK_CLR_MCO_CFG   (0x009FFFFFUL)
#define MRCC_MASK_CLR_RTC_BDCR  (0xFFFF7CFFUL)
#define MRCC_MASK_CLR_RTC_PRES  (0xFFE0FFFFUL)
#define MRCC_MASK_CLR_SSCGR  (0x30000000UL)
#define MRCC_MASK_VERIFY_TIMPRE_CONFIG  (0xFEFFFFFFUL)
#define MRCC_MASK_VERIFY_LS_CLK_CFG        (0xFEUL)

/** @} */

/** @defgroup RCC_Validation_Macros
 *  @brief Macros for validating various RCC parameters.
 *  @{ */
#define IS_VALID_PLL_CONFIG(CFG)                ((CFG) & MRCC_MASK_VERIFY_PLL_CFG) /**< Checks if the PLL configuration is valid */
#define IS_NULL_PTR(PTR)                        (PTR == NULL) /**< Checks if a pointer is null */
#define IS_VALID_SYS_CLK_SRC(SYSCLK)            ((SYSCLK != MRCC_SYS_CLK_HSI) && (SYSCLK != MRCC_SYS_CLK_HSE) && (SYSCLK != MRCC_SYS_CLK_PLL)) /**< Checks if the system clock source is valid */
#define IS_PLL_ENABLED()                        (((RCC->CR) >> 24) & 0x01U)
#define IS_INVALID_CLK(CLK)                     (MRCC_MASK_VERIFY_CLK_CTRL & (CLK))
#define IS_INVALID_CLK_CFG(CFG)                 (MRCC_MASK_VERIFY_CLK_CFG & (CFG))
#define IS_INVALID_SYS_CLK_SRC(CLK)             (MRCC_MASK_VERIFY_SYS_CLK_SRC & (CLK))
#define IS_VALID_PLL_CONFIG(CFG)                ((CFG) & MRCC_MASK_VERIFY_PLL_CFG) /**< Checks if the PLL configuration is valid */
#define IS_INVALID_PLL_M_PRES_RANGE(M)          (((M) < 2) || ((M) > 63))
#define IS_INVALID_PLL_N_MULT_RANGE(N)          (((N) < 2) || ((N) > 432))
#define IS_INVALID_PLL_P_PRES_RANGE(P)          (((P) != 2) && ((P) != 4) && ((P) != 6) && ((P) != 8))
#define IS_INVALID_PLL_Q_PRES_RANGE(Q)          (((Q) < 2) || ((Q) > 15))
#define IS_INVALID_VCO_INPUT_FREQ(VCO)          (((VCO) < 1000000) || ((VCO) > 2000000))
#define IS_INVALID_PLL_SRC(CLK)                 ((CLK) & MRCC_MASK_VERIFY_PLL_SRC)
#define IS_INVALID_AHB_CLK_PRESCALER(AHB)       (((AHB) < 0b0111) || ((AHB) > 0b1111))
#define IS_INVALID_APB_LS_CLK_PRESCALER(APB)    (((APB) < 0b011) || ((APB) > 0b111))
#define IS_INVALID_APB_HS_CLK_PRESCALER(APB)    (((APB) < 0b100) || ((APB) > 0b111))
#define IS_PLLI2S_ENABLED()                     (((RCC->CR) >> 26) & 0x01U)
#define IS_INVALID_PLLI2S_N_MULT_RANGE(N)       (((N) < 0b000000010) || ((N) > 0b110110000))
#define IS_INVALID_PLLI2S_R_PRES_RANGE(R)       (((R) < 0b010) || ((R) > 0b111))
#define IS_INVALID_RTC_STATE_CONFIG(CFG)        ((CFG) & MRCC_MASK_RTC_STATE)
#define IS_INVALID_HSE_BYPASS_CONFIG(CFG)       ((CFG) & MRCC_MASK_VERIFY_HSE_BYPASS_CONFIG)
#define IS_INVALID_LSE_BYPASS_CONFIG(CFG)       ((CFG) & MRCC_MASK_VERIFY_LSE_BYPASS_CONFIG)
#define IS_INVALID_CSS_CONFIG(CFG)              ((CFG) & MRCC_MASK_VERIFY_CSS_CONFIG)
#define IS_INVALID_MCO_PRESCALER(PRE)           (((PRE) < 0b100) || ((PRE) > 0b111))
#define IS_INVALID_TIMPRE_CONFIG(CFG)           ((CFG) & MRCC_MASK_VERIFY_TIMPRE_CONFIG)
#define IS_INVALID_LS_CLK_CFG(CFG)              (MRCC_MASK_VERIFY_LS_CLK_CFG & (CFG))

/** @} */

/** @defgroup RCC_Is_Reset_Flag_Raised
 *  @brief Macros for checking if a reset flag is raised.
 *  @{ */
#define MRCC_IS_LPWR_RESET_FLAG_RAISED()    (((RCC->CSR) >> (31UL)) & 0x01UL)    
#define MRCC_IS_WWDG_RESET_FLAG_RAISED()    (((RCC->CSR) >> (30UL)) & 0x01UL)
#define MRCC_IS_IWDG_RESET_FLAG_RAISED()    (((RCC->CSR) >> (29UL)) & 0x01UL)
#define MRCC_IS_SFTRESET_FLAG_RAISED()      (((RCC->CSR) >> (28UL)) & 0x01UL)
#define MRCC_IS_POR_RESET_FLAG_RAISED()     (((RCC->CSR) >> (27UL)) & 0x01UL)
#define MRCC_IS_PIN_RESET_FLAG_RAISED()     (((RCC->CSR) >> (26UL)) & 0x01UL)
#define MRCC_IS_BOR_RESET_FLAG_RAISED()     (((RCC->CSR) >> (25UL)) & 0x01UL)
/** @} */

/** @defgroup RCC_Is_Clock_Ready_Flag_Raised
 *  @brief Macros for checking if a clock ready flag is raised.
 *  @{ */
#define MRCC_IS_PLL_INT_FLAG_RAISED()       (((RCC->CIR) >> (4UL)) & 0x01UL)
#define MRCC_IS_PLLI2S_INT_FLAG_RAISED()    (((RCC->CIR) >> (5UL)) & 0x01UL)
#define MRCC_IS_HSE_INT_FLAG_RAISED()       (((RCC->CIR) >> (3UL)) & 0x01UL)
#define MRCC_IS_HSI_INT_FLAG_RAISED()       (((RCC->CIR) >> (2UL)) & 0x01UL)
#define MRCC_IS_LSE_INT_FLAG_RAISED()       (((RCC->CIR) >> (1UL)) & 0x01UL)
#define MRCC_IS_LSI_INT_FLAG_RAISED()       (((RCC->CIR) >> (0UL)) & 0x01UL)
/** @} */



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
        while ((RCC->CR & (~copy_enuClk)) && (local_uint16Timeout--));

        /* if clk is supposed to be on && timeout reached 0 -> return error status */
        if ((local_uint16Timeout == 0) && (RCC->CR & copy_enuClk))
        {
            ret_enuErrorStatus = RCC_TIMEOUT;
        }
        else {}
    }

    return ret_enuErrorStatus;
}

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

/**
 * @brief Configures the PLL (Phase Locked Loop).
 *
 * @param ptr_structPLLConfig Desired PLL configuration.
 * @return SRV_enuErrorStatus_t Error status of the operation.
 */
SRV_enuErrorStatus_t MRCC_enuConfigPLL(MRCC_structPLLConfig_t* ptr_structPLLConfig)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    uint16_t local_uint16WaitTimeout = MRCC_CLK_WAIT_TIMEOUT;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;
    uint32_t local_uint32VCO = 0;

    if (IS_PLL_ENABLED())
    {
        ret_enuErrorStatus = RCC_PLL_CFG_WHILE_EN;
    }
    else if (IS_NULL_PTR(ptr_structPLLConfig))
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else if (IS_INVALID_PLL_M_PRES_RANGE(ptr_structPLLConfig->M_Prescaler))
    {
        ret_enuErrorStatus = RCC_PLL_INV_M;
    }
    else if (IS_INVALID_PLL_N_MULT_RANGE(ptr_structPLLConfig->N_Multiplier))
    {
        ret_enuErrorStatus = RCC_PLL_INV_N;
    }
    else if (IS_INVALID_PLL_P_PRES_RANGE(ptr_structPLLConfig->P_Prescaler))
    {
        ret_enuErrorStatus = RCC_PLL_INV_P;
    }
    else if (IS_INVALID_PLL_Q_PRES_RANGE(ptr_structPLLConfig->Q_Prescaler))
    {
        ret_enuErrorStatus = RCC_PLL_INV_Q;
    }
    else
    {
        if (ptr_structPLLConfig->CLK_Src == MRCC_PLL_SRC_HSI)
        {
            local_uint32VCO = RCC_HSI_CLK_HZ / ptr_structPLLConfig->M_Prescaler;
        }
        else if (ptr_structPLLConfig->CLK_Src == MRCC_PLL_SRC_HSE)
        {
            local_uint32VCO = RCC_HSE_CLK_HZ / ptr_structPLLConfig->M_Prescaler;
        }
        else {}

        if (IS_INVALID_VCO_INPUT_FREQ(local_uint32VCO))
        {
            ret_enuErrorStatus = RCC_INV_PLL_SRC;
        }
        else
        {
            RCC->PLLCFGR &= MRCC_MASK_VERIFY_PLL_CFG;
            RCC->PLLCFGR |= (ptr_structPLLConfig->Q_Prescaler << 24) | (ptr_structPLLConfig->CLK_Src << 22) | (ptr_structPLLConfig->P_Prescaler << 16) | (ptr_structPLLConfig->N_Multiplier << 6) | (ptr_structPLLConfig->M_Prescaler);

           ret_enuErrorStatus = MRCC_enuSetClkSrcState(MRCC_PLL_CLK, MRCC_CLK_SRC_ENABLE);
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
SRV_enuErrorStatus_t MRCC_enuSetBusClkPrescaler(MRCC_structBusClkPrescaler_t* ptr_structBusClkPrescalers)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;

    if (IS_NULL_PTR(ptr_structBusClkPrescalers))
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else if (IS_INVALID_AHB_CLK_PRESCALER(ptr_structBusClkPrescalers->AHB_Prescaler))
    {
        ret_enuErrorStatus = RCC_INV_AHB_CLK_PRE;
    }
    else if (IS_INVALID_APB_LS_CLK_PRESCALER(ptr_structBusClkPrescalers->APB_LowSpeedPrescaler))
    {
        ret_enuErrorStatus = RCC_INV_APB_LS_CLK_PRE;
    }
    else if (IS_INVALID_APB_HS_CLK_PRESCALER(ptr_structBusClkPrescalers->APB_HighSpeedPrescaler))
    {
        ret_enuErrorStatus = RCC_INV_APB_HS_CLK_PRE;
    }
    else
    {
        RCC->CFGR = ((RCC->CFGR & MRCC_MASK_SET_BUS_CLK_PRESCALER) | (ptr_structBusClkPrescalers->AHB_Prescaler << 4) | (ptr_structBusClkPrescalers->APB_LowSpeedPrescaler << 10) | (ptr_structBusClkPrescalers->APB_HighSpeedPrescaler << 13));
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MRCC_enuSetClkReadyInterruptState(MRCC_structClkReadyInterrupt_t* ptr_structClkReadyInterrupt)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;

    if (IS_NULL_PTR(ptr_structClkReadyInterrupt))
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else 
    {
        RCC->CIR = ((RCC->CIR) & MRCC_MASK_SET_CLK_INT_RDY) | (ptr_structClkReadyInterrupt->HSE << 11) | (ptr_structClkReadyInterrupt->HSI << 10) | (ptr_structClkReadyInterrupt->LSE << 9) | (ptr_structClkReadyInterrupt->LSI << 8) | (ptr_structClkReadyInterrupt->PLL << 12) | (ptr_structClkReadyInterrupt->PLLI2S << 13);
    }
    
    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MRCC_enuConfigPLLI2S(MRCC_structPLLI2CConfig_t* ptr_structPLLConfig)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    uint16_t local_uint16WaitTimeout = MRCC_CLK_WAIT_TIMEOUT;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;
    uint32_t local_uint32VCO = 0;

    if (IS_NULL_PTR(ptr_structPLLConfig))
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else if (IS_PLLI2S_ENABLED())
    {
        ret_enuErrorStatus = RCC_PLL_CFG_WHILE_EN;
    }
    else if (IS_INVALID_PLLI2S_N_MULT_RANGE(ptr_structPLLConfig->N_Multiplier))
    {
        ret_enuErrorStatus = RCC_PLL_INV_N;
    }
    else if (IS_INVALID_PLLI2S_R_PRES_RANGE(ptr_structPLLConfig->R_Prescaler))
    {
        ret_enuErrorStatus = RCC_PLL_INV_R;
    }
    else 
    {
        // #define GET_PLL_M_PRES_VAL()    (RCC->PLLCFGR & 0b111111)
        // if (MRCC_PLL_SRC == MRCC_PLL_SRC_HSI)
        // {
        //     local_uint32VCO = RCC_HSI_CLK_HZ / ptr_structPLLConfig->M_Prescaler;
        // }
        // else if (MRCC_PLL_SRC == MRCC_PLL_SRC_HSE)
        // {
        //     local_uint32VCO = RCC_HSE_CLK_HZ / ptr_structPLLConfig->M_Prescaler;
        // }
    }
    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MRCC_enuGetRaisedClkResetFlags(MRCC_structClkResetFlags_t* ptr_structClkResetFlags)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;

    if (IS_NULL_PTR(ptr_structClkResetFlags))
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else 
    {
        ptr_structClkResetFlags->LPWRRSTF = MRCC_IS_LPWR_RESET_FLAG_RAISED();
        ptr_structClkResetFlags->WWDGRSTF = MRCC_IS_WWDG_RESET_FLAG_RAISED();
        ptr_structClkResetFlags->IWDGRSTF = MRCC_IS_IWDG_RESET_FLAG_RAISED();
        ptr_structClkResetFlags->SFTRSTF = MRCC_IS_SFTRESET_FLAG_RAISED();
        ptr_structClkResetFlags->PORRSTF = MRCC_IS_POR_RESET_FLAG_RAISED();
        ptr_structClkResetFlags->PINRSTF = MRCC_IS_PIN_RESET_FLAG_RAISED();
        ptr_structClkResetFlags->BORRSTF = MRCC_IS_BOR_RESET_FLAG_RAISED();
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MRCC_enuGetRaisedClkReadyInterruptFlags(MRCC_structClkReadyInterruptFlags_t* ptr_structClkInterruptFlags)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;

    if (IS_NULL_PTR(ptr_structClkInterruptFlags))
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else 
    {
        ptr_structClkInterruptFlags->LSIRDYF = MRCC_IS_LSI_INT_FLAG_RAISED();
        ptr_structClkInterruptFlags->LSERDYF = MRCC_IS_LSE_INT_FLAG_RAISED();
        ptr_structClkInterruptFlags->HSIRDYF = MRCC_IS_HSI_INT_FLAG_RAISED();
        ptr_structClkInterruptFlags->HSERDYF = MRCC_IS_HSE_INT_FLAG_RAISED();
        ptr_structClkInterruptFlags->PLLRDYF = MRCC_IS_PLL_INT_FLAG_RAISED();
        ptr_structClkInterruptFlags->PLLI2SRDYF = MRCC_IS_PLLI2S_INT_FLAG_RAISED();
    }

    return ret_enuErrorStatus;
}

inline void MRCC_voidClrCSSFlag(void)
{
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;
    RCC->CIR |= (MRCC_MASK_CLR_CSS_FLAG);
}

inline void MRCC_voidClrPLLI2SFlag(void)
{
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;
    RCC->CIR |= (MRCC_MASK_CLR_PLLI2S_FLAG);
}

inline void MRCC_voidClrPLLFlag(void)
{
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;
    RCC->CIR |= (MRCC_MASK_CLR_PLL_FLAG);
}

inline void MRCC_voidClrHSEFlag(void)
{
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;
    RCC->CIR |= (MRCC_MASK_CLR_HSE_FLAG);
}

inline void MRCC_voidClrHSIFlag(void)
{
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;
    RCC->CIR |= (MRCC_MASK_CLR_HSI_FLAG);
}

inline void MRCC_voidClrLSEFlag(void)
{
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;
    RCC->CIR |= (MRCC_MASK_CLR_LSE_FLAG);
}

inline void MRCC_voidClrLSIFlag(void)
{
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;
    RCC->CIR |= (MRCC_MASK_CLR_LSI_FLAG);
}

inline void MRCC_voidClrResetFlags(void)
{
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;
    RCC->CSR &= ~MRCC_MASK_CLR_RST_FLAGS;
    RCC->CSR |= MRCC_MASK_CLR_RST_FLAGS;
}

inline void MRCC_voidPerformBackupDomainReset(void)
{
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;
    RCC->BDCR |= (0x00010000UL);
}

SRV_enuErrorStatus_t MRCC_enuRTCStateConfig(MRCC_enumRTCStateConfig_t copy_enuRTCState)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;
    if (IS_INVALID_HSE_BYPASS_CONFIG(copy_enuRTCState))
    {
        ret_enuErrorStatus = INV_ARG;
    }
    else 
    {
        RCC->BDCR &= ~MRCC_MASK_RTC_STATE;
        RCC->BDCR |= copy_enuRTCState;
    }
    
    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MRCC_enuHSEBypassConfig(MRCC_enuHSEBypassConfig_t copy_enuConfig)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;
    if (IS_INVALID_HSE_BYPASS_CONFIG(copy_enuConfig))
    {
        ret_enuErrorStatus = INV_ARG;
    }
    else 
    {
        RCC->CR &= MRCC_MASK_VERIFY_HSE_BYPASS_CONFIG;
        RCC->CR |= (copy_enuConfig);
    }
    
    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MRCC_enuLSEBypassConfig(MRCC_enuHSEBypassConfig_t copy_enuConfig)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;
    if (IS_INVALID_LSE_BYPASS_CONFIG(copy_enuConfig))
    {
        ret_enuErrorStatus = INV_ARG;
    }
    else 
    {
        RCC->BDCR &= MRCC_MASK_VERIFY_LSE_BYPASS_CONFIG;
        RCC->BDCR |= (copy_enuConfig);
    }
    
    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MRCC_voidCSSStateConfig(MRCC_enuCSSStateConfig_t copy_enuConfig)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;
    if (IS_INVALID_CSS_CONFIG(copy_enuConfig))
    {
        ret_enuErrorStatus = INV_ARG;
    }
    else 
    {
        RCC->CR &= MRCC_MASK_VERIFY_CSS_CONFIG;
        RCC->CR |= (copy_enuConfig);
    }
    
    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MRCC_enuSetMCOConfig(MRCC_structMCOConfig_t* ptr_structMCOConfig)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;

    if (IS_NULL_PTR(ptr_structMCOConfig))
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else if (IS_INVALID_MCO_PRESCALER(ptr_structMCOConfig->MCO1_Prescaler) || IS_INVALID_MCO_PRESCALER(ptr_structMCOConfig->MCO2_Prescaler))
    {
        ret_enuErrorStatus = RCC_INV_MCO_PRE;
    }
    else 
    {
        RCC->CFGR &= MRCC_MASK_CLR_MCO_CFG;
        RCC->CFGR |= (ptr_structMCOConfig->MCO1_Prescaler << 24) | (ptr_structMCOConfig->MCO2_Prescaler << 27) | (ptr_structMCOConfig->MCO1_Source << 21) | (ptr_structMCOConfig->MCO2_Source << 30);
    }

    return ret_enuErrorStatus;
} 

SRV_enuErrorStatus_t MRCC_enuSetRTCConfig(MRCC_structRTCConfig_t* ptr_structRTCConfig)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;

    if (IS_NULL_PTR(ptr_structRTCConfig))
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else
    {
        RCC->CFGR &= MRCC_MASK_CLR_RTC_PRES;
        RCC->CFGR |= (ptr_structRTCConfig->RTC_Prescaler << 16);

        RCC->BDCR &= MRCC_MASK_CLR_RTC_BDCR;
        RCC->BDCR |= (ptr_structRTCConfig->RTC_ClkSource << 8);

        ret_enuErrorStatus = MRCC_enuRTCStateConfig(ptr_structRTCConfig->RTCEN);
    }

    return ret_enuErrorStatus;
} 

SRV_enuErrorStatus_t MRCC_enuSetPLLSpreadSpectrumConfig(MRCC_structSpeadSpectrumClkGenConfig_t* ptr_structSpreadSpectrumConfig)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;

    if (IS_NULL_PTR(ptr_structSpreadSpectrumConfig))
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else if (IS_PLL_ENABLED())
    {
        ret_enuErrorStatus = RCC_PLL_CFG_WHILE_EN;
    }
    else
    {
        RCC->SSCGR &= MRCC_MASK_CLR_SSCGR;
        RCC->SSCGR |= (ptr_structSpreadSpectrumConfig->INCSTEP << 13) | (ptr_structSpreadSpectrumConfig->MODPER << 0) | (ptr_structSpreadSpectrumConfig->SSCGEN << 31) | (ptr_structSpreadSpectrumConfig->SPREADSEL << 30); 
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MRCC_enuSetTimerPrescaler(MRCC_enuTimerClkPrescalerConfig_t copy_enuTimerClkPrescalerConfig)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;
    if (IS_INVALID_TIMPRE_CONFIG(copy_enuTimerClkPrescalerConfig))
    {
        ret_enuErrorStatus = INV_ARG;
    }
    else 
    {
        RCC->DCKCFGR &= MRCC_MASK_VERIFY_TIMPRE_CONFIG;
        RCC->DCKCFGR |= copy_enuTimerClkPrescalerConfig;
    }
    
    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MRCC_enuSetLSIClkState(MRCC_enuClkSrcState_t copy_enuLSClkState)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile RCC_Registers_t* RCC = (volatile RCC_Registers_t*)RCC_BASE_ADDRESS;
    uint16_t local_uint16Timeout = MRCC_CLK_WAIT_TIMEOUT;

    if (IS_INVALID_LS_CLK_CFG(copy_enuLSClkState))
    {
        ret_enuErrorStatus = RCC_INV_CFG;
    }
    else 
    {
        /* clear before setting */
        RCC->CSR &= MRCC_MASK_VERIFY_LS_CLK_CFG;

        /* if state is on -> set it : else -> leave it 0 */
        if (copy_enuLSClkState == LS_CLK_ENABLE)
        {
            RCC->CSR |= (0x01U);

            /* wait until clock is ready */
            while ((RCC->CSR  & (~(0b10U))) && (local_uint16Timeout--));

            /* if clk is supposed to be on && timeout reached 0 -> return error status */
            if ((local_uint16Timeout == 0) && (RCC->CSR & (0b10U)))
            {
                ret_enuErrorStatus = RCC_TIMEOUT;
            }
            else {}
        }
        else {}
    }

    return ret_enuErrorStatus;
}
