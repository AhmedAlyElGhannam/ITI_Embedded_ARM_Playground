#include "error_status.h"
#include "std_lib.h"
#include "std_types.h"
#include "RCC.h"
#include "USART_Reg.h"
#include "USART.h"

#define USART_POLLING_TIMEOUT   (1000U)

#define IS_TRANSMIT_DATA_REGISTER_EMPTY_FLAG_UP(USART_SR)   (((USART_SR) >> 7) & 1U)
#define IS_CTS_FLAG_UP(USART_SR)                            (((USART_SR) >> 9) & 1U)
#define IS_TRANSMISSION_COMPLETE_FLAG_UP(USART_SR)          (((USART_SR) >> 6) & 1U)
#define IS_RECEIVED_DATA_RDY_TO_BE_READ_FLAG_UP(USART_SR)   (((USART_SR) >> 5) & 1U)
#define IS_OVERRUN_ERR_DETECTED_FLAG_UP(USART_SR)           (((USART_SR) >> 3) & 1U)
#define IS_IDLE_LINE_DETECTED_FLAG_UP(USART_SR)             (((USART_SR) >> 4) & 1U)
#define IS_PARITY_ERR_FLAG_UP(USART_SR)                     (((USART_SR) >> 0) & 1U)
#define IS_LIN_BREAK_FLAG_UP(USART_SR)                      (((USART_SR) >> 8) & 1U)
#define IS_NOISE_OR_OVERRUN_PLUS_FRAMING_ERR_IN_MULTIBUF_COMM_FLAG_UP(USART_SR) ((((USART_SR) >> 2) & 1U) || ((((USART_SR) >> 1) & 1U) || (((USART_SR) >> 3) & 1U)))


static MUSART_ptrCBF MUSART1_CBFArr[NUM_OF_USART_IRQs] = {NULL};
static MUSART_ptrCBF MUSART2_CBFArr[NUM_OF_USART_IRQs] = {NULL};
static MUSART_ptrCBF MUSART6_CBFArr[NUM_OF_USART_IRQs] = {NULL};


static void MUSART_voidToggleIRQTransmitDataRegEmpty(MUSART_enuModule_t, MUSART_enuIRQConfig_t);
static void MUSART_voidToggleIRQCTS(MUSART_enuModule_t, MUSART_enuIRQConfig_t);
static void MUSART_voidToggleIRQTransmissionComplete(MUSART_enuModule_t, MUSART_enuIRQConfig_t);
static void MUSART_voidToggleIRQReceiveDataReadyToBeRead(MUSART_enuModule_t, MUSART_enuIRQConfig_t);
static void MUSART_voidToggleIRQOverrunErrDetected(MUSART_enuModule_t, MUSART_enuIRQConfig_t);
static void MUSART_voidToggleIRQIdleLineDetected(MUSART_enuModule_t, MUSART_enuIRQConfig_t);
static void MUSART_voidToggleIRQParityErr(MUSART_enuModule_t, MUSART_enuIRQConfig_t);
static void MUSART_voidToggleIRQLINBreakDetection(MUSART_enuModule_t, MUSART_enuIRQConfig_t);
static void MUSART_voidToggleIRQNoise(MUSART_enuModule_t, MUSART_enuIRQConfig_t);

static void MUSART_voidToggleSynchModeOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModeConfig_t copy_enuModeState);
static void MUSART_voidToggleLINModeOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModeConfig_t copy_enuModeState);
static void MUSART_voidToggleDMATxOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModeConfig_t copy_enuModeState);
static void MUSART_voidToggleDMARxOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModeConfig_t copy_enuModeState);
static void MUSART_voidToggleSmartCardModeOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModeConfig_t copy_enuModeState);
static void MUSART_voidToggleSingleWireModeOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModeConfig_t copy_enuModeState);
static void MUSART_voidToggleIrDAModeOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModeConfig_t copy_enuModeState);
static void MUSART_voidToggleHWFlowControlModeOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModeConfig_t copy_enuModeState);
static uint8_t MUSART_uint8GetOverSampleMode(MUSART_enuModule_t copy_enuUSARTModule);

/* send data if true */
static bool MUSART_boolIsTransmitDREmpty(MUSART_enuModule_t copy_enuUSARTModule)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

    return (((USARTx->SR >> 7) & 1U) == 1U);
}

/* read data if true */
static bool MUSART_boolIsReadDRNotEmpty(MUSART_enuModule_t copy_enuUSARTModule)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

    return (((USARTx->SR >> 5) & 1U) == 1U);
}

SRV_enuErrorStatus_t MUSART_enuConfigOverSampleMode(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuOverSampleMode_t copy_enuOverSampleMode)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if ((copy_enuUSARTModule != USART1) && (copy_enuUSARTModule != USART2) && (copy_enuUSARTModule != USART6))
    {
        ret_enuErrorStatus = USART_INV_MODULE;
    }
    else if ((copy_enuOverSampleMode != OVERSAMPLE_BY_8) && (copy_enuOverSampleMode != OVERSAMPLE_BY_16))
    {
        ret_enuErrorStatus = USART_INV_OVERSAMPLE_MODE;
    }
    else 
    {
        volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

        if (copy_enuOverSampleMode == OVERSAMPLE_BY_8)
        {
            USARTx->CR1 |= (1U << 15);
        }
        else if (copy_enuOverSampleMode == OVERSAMPLE_BY_16)
        {
            USARTx->CR1 &= ~(1U << 15);
        }
        else {}
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MUSART_enuSetBaudRate(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuDefinedBaudRates_t copy_enuBaudRate)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    uint64_t local_uint64BusClk = 0;
    uint16_t local_uint16USARTDIVMantissa = 0;
    float32_t local_float32USARTDIVFrac = 0;
    uint8_t local_uint8USARTDIVFrac4Bit = 0;
    uint16_t local_uint16BRRTemp = 0;

    if ((copy_enuUSARTModule != USART1) && (copy_enuUSARTModule != USART2) && (copy_enuUSARTModule != USART6))
    {
        ret_enuErrorStatus = USART_INV_MODULE;
    }    
    else 
    {
        volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

        /* get bus freq */
        if (copy_enuUSARTModule == USART2)
        {
            MRCC_enuGetBusFreq(SYS_BUS_APB1, &local_uint64BusClk);
        }
        else /* applies for both USART1 && USART6 */ 
        {
            MRCC_enuGetBusFreq(SYS_BUS_APB2, &local_uint64BusClk);
        }

        /* calculate USARTDIV mantissa */ 
        local_uint16USARTDIVMantissa = local_uint64BusClk / (8 * (2 - MUSART_uint8GetOverSampleMode(copy_enuUSARTModule))) / (copy_enuBaudRate);

        if (local_uint16USARTDIVMantissa == 0U)
        {
            ret_enuErrorStatus = USART_ZERO_VAL_MANTISSA;
        }
        else /* valid mantissa */
        {
            /* calculate USARTDIV frac as float */
            local_float32USARTDIVFrac = ((float64_t)local_uint64BusClk / (8 * (2 - MUSART_uint8GetOverSampleMode(copy_enuUSARTModule))) / (copy_enuBaudRate)) - local_uint16USARTDIVMantissa;
            local_float32USARTDIVFrac *= 0x10; 

            /* convert float frac to int */
            local_uint8USARTDIVFrac4Bit = ceil(local_float32USARTDIVFrac);

            /* if frac part overflowed -> add it to mantissa */
            while (local_uint8USARTDIVFrac4Bit >= 0x10)
            {
                local_uint8USARTDIVFrac4Bit -= 0x10;
                local_uint16USARTDIVMantissa += 1;
            }

            /* set mantissa & frac parts into temp */
            local_uint16BRRTemp = local_uint8USARTDIVFrac4Bit | (local_uint16USARTDIVMantissa << 4);

            /* set temp value into BRR */
            USARTx->BRR = (USARTx->BRR & 0xFFFF0000) | local_uint16BRRTemp;
        }

    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MUSART_enuToggleUSARTOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModuleConfig_t copy_enuModuleState)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if ((copy_enuUSARTModule != USART1) && (copy_enuUSARTModule != USART2) && (copy_enuUSARTModule != USART6))
    {
        ret_enuErrorStatus = USART_INV_MODULE;
    }
    else if ((copy_enuModuleState != USART_ON) && (copy_enuModuleState != USART_OFF))
    {
        ret_enuErrorStatus = USART_INV_ARG;
    }
    else 
    {
        volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

        if (copy_enuModuleState == USART_ON)
        {
            USARTx->CR1 |= (1U << 13);
        }
        else if (copy_enuModuleState == USART_OFF)
        {
            USARTx->CR1 &= ~(1U << 13);
        }
        else {}
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MUSART_enuToggleTxOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuTxControl_t copy_enuTxState)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if ((copy_enuUSARTModule != USART1) && (copy_enuUSARTModule != USART2) && (copy_enuUSARTModule != USART6))
    {
        ret_enuErrorStatus = USART_INV_MODULE;
    }
    else if ((copy_enuTxState != TX_EN) && (copy_enuTxState != TX_DIS))
    {
        ret_enuErrorStatus = USART_INV_OVERSAMPLE_MODE;
    }
    else 
    {
        volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

        if (copy_enuTxState == TX_EN)
        {
            USARTx->CR1 |= (1U << 3);
        }
        else if (copy_enuTxState == TX_DIS)
        {
            USARTx->CR1 &= ~(1U << 3);
        }
        else {}
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MUSART_enuToggleRxOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuRxControl_t copy_enuRxState)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if ((copy_enuUSARTModule != USART1) && (copy_enuUSARTModule != USART2) && (copy_enuUSARTModule != USART6))
    {
        ret_enuErrorStatus = USART_INV_MODULE;
    }
    else if ((copy_enuRxState != RX_EN) && (copy_enuRxState != RX_DIS))
    {
        ret_enuErrorStatus = USART_INV_OVERSAMPLE_MODE;
    }
    else 
    {
        volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

        if (copy_enuRxState == RX_EN)
        {
            USARTx->CR1 |= (1U << 2);
        }
        else if (copy_enuRxState == RX_DIS)
        {
            USARTx->CR1 &= ~(1U << 2);
        }
        else {}
    }

    return ret_enuErrorStatus;
}



SRV_enuErrorStatus_t MUSART_enuEnableUSARTMode(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModes_t copy_enuModes)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if ((copy_enuUSARTModule != USART1) && (copy_enuUSARTModule != USART2) && (copy_enuUSARTModule != USART6))
    {
        ret_enuErrorStatus = USART_INV_MODULE;
    }
    else 
    {
        switch (copy_enuModes)
        {
            case MUSART_NORMAL_MODE:
            break;

            case MUSART_HW_FLOW_CONTROL_MODE:
                MUSART_voidToggleHWFlowControlModeOnOff(copy_enuUSARTModule, USART_MODE_ON);
            break;

            case MUSART_DMA_MODE:
                MUSART_voidToggleDMATxOnOff(copy_enuUSARTModule, USART_MODE_ON);
                MUSART_voidToggleDMARxOnOff(copy_enuUSARTModule, USART_MODE_ON);
            break;

            case MUSART_SYNC_MODE:
                MUSART_voidToggleSynchModeOnOff(copy_enuUSARTModule, USART_MODE_ON);
            break;

            case MUSART_SMARTCARD_MODE:
                MUSART_voidToggleSmartCardModeOnOff(copy_enuUSARTModule, USART_MODE_ON);
            break;

            case MUSART_SINGLE_WIRE_MODE:
                MUSART_voidToggleSingleWireModeOnOff(copy_enuUSARTModule, USART_MODE_ON);
            break;

            case MUSART_IrDA_MODE:
                MUSART_voidToggleIrDAModeOnOff(copy_enuUSARTModule, USART_MODE_ON);
            break;

            case MUSART_LIN_MODE:
                MUSART_voidToggleLINModeOnOff(copy_enuUSARTModule, USART_MODE_ON);
            break;
            
            default:
                ret_enuErrorStatus = USART_INV_MODE;
            break;
        }
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MUSART_enuDisableUSARTMode(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModes_t copy_enuModes)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if ((copy_enuUSARTModule != USART1) && (copy_enuUSARTModule != USART2) && (copy_enuUSARTModule != USART6))
    {
        ret_enuErrorStatus = USART_INV_MODULE;
    }
    else 
    {
        switch (copy_enuModes)
        {
            case MUSART_NORMAL_MODE:
            break;

            case MUSART_HW_FLOW_CONTROL_MODE:
                MUSART_voidToggleHWFlowControlModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);
            break;

            case MUSART_DMA_MODE:
                MUSART_voidToggleDMATxOnOff(copy_enuUSARTModule, USART_MODE_OFF);
                MUSART_voidToggleDMARxOnOff(copy_enuUSARTModule, USART_MODE_OFF);
            break;

            case MUSART_SYNC_MODE:
                MUSART_voidToggleSynchModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);
            break;

            case MUSART_SMARTCARD_MODE:
                MUSART_voidToggleSmartCardModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);
            break;

            case MUSART_SINGLE_WIRE_MODE:
                MUSART_voidToggleSingleWireModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);
            break;

            case MUSART_IrDA_MODE:
                MUSART_voidToggleIrDAModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);
            break;

            case MUSART_LIN_MODE:
                MUSART_voidToggleLINModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);
            break;
            
            default:
                ret_enuErrorStatus = USART_INV_MODE;
            break;
        }
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MUSART_enuEnableUSARTInt(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuIRQSources_t copy_enuIRQSource)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if ((copy_enuUSARTModule != USART1) && (copy_enuUSARTModule != USART2) && (copy_enuUSARTModule != USART6))
    {
        ret_enuErrorStatus = USART_INV_MODULE;
    }
    else 
    {
        switch (copy_enuIRQSource)
        {
            case IRQ_TRANSMIT_DATA_REG_EMPTY:
                MUSART_voidToggleIRQTransmitDataRegEmpty(copy_enuUSARTModule, USART_IRQ_EN);
            break;

            case IRQ_CTS_FLAG:
                MUSART_voidToggleIRQCTS(copy_enuUSARTModule, USART_IRQ_EN);
            break;

            case IRQ_TRANSMISSION_COMPLETE:
            MUSART_voidToggleIRQTransmissionComplete(copy_enuUSARTModule, USART_IRQ_EN);
            break;

            case IRQ_RECEIVE_DATA_RDY_TO_BE_READ:
                MUSART_voidToggleIRQReceiveDataReadyToBeRead(copy_enuUSARTModule, USART_IRQ_EN);
            break;

            case IRQ_OVERRUN_ERR_DETECTED:
                MUSART_voidToggleIRQOverrunErrDetected(copy_enuUSARTModule, USART_IRQ_EN);
            break;

            case IRQ_IDLE_LINE_DETECTED:
                MUSART_voidToggleIRQIdleLineDetected(copy_enuUSARTModule, USART_IRQ_EN);
            break;

            case IRQ_PARITY_ERR:
                MUSART_voidToggleIRQParityErr(copy_enuUSARTModule, USART_IRQ_EN);
            break;

            case IRQ_BREAK_FLAG:
                MUSART_voidToggleIRQLINBreakDetection(copy_enuUSARTModule, USART_IRQ_EN);
            break;

            case IRQ_NOISE_FLAG:
                MUSART_voidToggleIRQNoise(copy_enuUSARTModule, USART_IRQ_EN);
            break;
            
            default:
                ret_enuErrorStatus = USART_INV_IRQ;
            break;
        }
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MUSART_enuDisableUSARTInt(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuIRQSources_t copy_enuIRQSource)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if ((copy_enuUSARTModule != USART1) && (copy_enuUSARTModule != USART2) && (copy_enuUSARTModule != USART6))
    {
        ret_enuErrorStatus = USART_INV_MODULE;
    }
    else 
    {
        switch (copy_enuIRQSource)
        {
            case IRQ_TRANSMIT_DATA_REG_EMPTY:
                MUSART_voidToggleIRQTransmitDataRegEmpty(copy_enuUSARTModule, USART_IRQ_DIS);
            break;

            case IRQ_CTS_FLAG:
                MUSART_voidToggleIRQCTS(copy_enuUSARTModule, USART_IRQ_DIS);
            break;

            case IRQ_TRANSMISSION_COMPLETE:
            MUSART_voidToggleIRQTransmissionComplete(copy_enuUSARTModule, USART_IRQ_DIS);
            break;

            case IRQ_RECEIVE_DATA_RDY_TO_BE_READ:
                MUSART_voidToggleIRQReceiveDataReadyToBeRead(copy_enuUSARTModule, USART_IRQ_DIS);
            break;

            case IRQ_OVERRUN_ERR_DETECTED:
                MUSART_voidToggleIRQOverrunErrDetected(copy_enuUSARTModule, USART_IRQ_DIS);
            break;

            case IRQ_IDLE_LINE_DETECTED:
                MUSART_voidToggleIRQIdleLineDetected(copy_enuUSARTModule, USART_IRQ_DIS);
            break;

            case IRQ_PARITY_ERR:
                MUSART_voidToggleIRQParityErr(copy_enuUSARTModule, USART_IRQ_DIS);
            break;

            case IRQ_BREAK_FLAG:
                MUSART_voidToggleIRQLINBreakDetection(copy_enuUSARTModule, USART_IRQ_DIS);
            break;

            case IRQ_NOISE_FLAG:
                MUSART_voidToggleIRQNoise(copy_enuUSARTModule, USART_IRQ_DIS);
            break;
            
            default:
                ret_enuErrorStatus = USART_INV_IRQ;
            break;
        }
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MUSART_enuInitUSARTClk(MUSART_enuModule_t copy_enuUSARTModule)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    switch (copy_enuUSARTModule)
    {
        case USART1:
            MRCC_enuSetPeripheralClkState(MRCC_APB2_USART1, MRCC_PERIPHERAL_CLK_ENABLE);
        break;

        case USART2:
            MRCC_enuSetPeripheralClkState(MRCC_APB1_USART2, MRCC_PERIPHERAL_CLK_ENABLE);
        break;

        case USART6:
            MRCC_enuSetPeripheralClkState(MRCC_APB2_USART6, MRCC_PERIPHERAL_CLK_ENABLE);
        break;

        default:
            ret_enuErrorStatus = USART_INV_MODULE;
        break;
    }

    return ret_enuErrorStatus;
}

/* frame settings */
SRV_enuErrorStatus_t MUSART_enuConfigUSARTFrameSettings(MUSART_enuModule_t copy_enuUSARTModule, MUSART_structFrameSettings_t* ptr_structFrameSettings)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if ((copy_enuUSARTModule != USART1) && (copy_enuUSARTModule != USART2) && (copy_enuUSARTModule != USART6))
    {
        ret_enuErrorStatus = USART_INV_MODULE;
    }
    else if (ptr_structFrameSettings == NULL)
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else if ((ptr_structFrameSettings->wordLen != ONE_START_8_DATA_nSTOP) && (ptr_structFrameSettings->wordLen !=  ONE_START_9_DATA_nSTOP))
    {
        ret_enuErrorStatus = USART_INV_WORD_LEN;
    }
    else if ((ptr_structFrameSettings->paritySel != EVEN_PARITY) && (ptr_structFrameSettings->paritySel !=  ODD_PARITY) && (ptr_structFrameSettings->paritySel != PARITY_NONE))
    {
        ret_enuErrorStatus = USART_INV_PARITY_SEL;
    }
    else if ((ptr_structFrameSettings->parityCon != PARITY_CONTROL_DIS) && (ptr_structFrameSettings->parityCon !=  PARITY_CONTROL_EN))
    {
        ret_enuErrorStatus = USART_INV_PARITY_CON;
    }
    else if ((ptr_structFrameSettings->stopBit != ONE_HALF_STOP_BIT) && (ptr_structFrameSettings->stopBit !=  ONE_STOP_BIT) && (ptr_structFrameSettings->stopBit !=  TWO_STOP_BIT) && (ptr_structFrameSettings->stopBit !=  HALF_STOP_BIT))
    {
        ret_enuErrorStatus = USART_INV_STOP_BIT;
    }
    else 
    {
        /* setting word length */
        MUSART_enuSetWordLen(copy_enuUSARTModule, ptr_structFrameSettings->wordLen);

        /* setting parity control */
        MUSART_enuSetParityControl(copy_enuUSARTModule, ptr_structFrameSettings->parityCon);

        /* setting parity selection */
        if (ptr_structFrameSettings->paritySel != PARITY_NONE)
        {
            MUSART_enuSetParitySel(copy_enuUSARTModule, ptr_structFrameSettings->paritySel);
        }
        else {}

        /* setting stop bits */
        MUSART_enuConfigStopBits(copy_enuUSARTModule, ptr_structFrameSettings->stopBit);
    }


    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MUSART_enuSetWordLen(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuWordLen_t copy_enuWordLen)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if ((copy_enuUSARTModule != USART1) && (copy_enuUSARTModule != USART2) && (copy_enuUSARTModule != USART6))
    {
        ret_enuErrorStatus = USART_INV_MODULE;
    }
    else if ((copy_enuWordLen != ONE_START_8_DATA_nSTOP) && (copy_enuWordLen !=  ONE_START_9_DATA_nSTOP))
    {
        ret_enuErrorStatus = USART_INV_WORD_LEN;
    }
    else 
    {
        volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

        if (copy_enuWordLen == ONE_START_8_DATA_nSTOP)
        {
            USARTx->CR1 &= ~(1U << 12);
        }
        else if (copy_enuWordLen == ONE_START_9_DATA_nSTOP)
        {
            USARTx->CR1 |= (1U << 12);
        }
        else {}
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MUSART_enuSetParityControl(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuParityControl_t copy_enuParityControl)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if ((copy_enuUSARTModule != USART1) && (copy_enuUSARTModule != USART2) && (copy_enuUSARTModule != USART6))
    {
        ret_enuErrorStatus = USART_INV_MODULE;
    }
    else if ((copy_enuParityControl != PARITY_CONTROL_EN) && (copy_enuParityControl !=  PARITY_CONTROL_DIS))
    {
        ret_enuErrorStatus = USART_INV_PARITY_CON;
    }
    else 
    {
        volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

        if (copy_enuParityControl == PARITY_CONTROL_DIS)
        {
            USARTx->CR1 &= ~(1U << 10);
        }
        else if (copy_enuParityControl == PARITY_CONTROL_EN)
        {
            USARTx->CR1 |= (1U << 10);
        }
        else {}
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MUSART_enuConfigStopBits(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuStopBitConfig_t copy_enuStopBitConfig)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if ((copy_enuUSARTModule != USART1) && (copy_enuUSARTModule != USART2) && (copy_enuUSARTModule != USART6))
    {
        ret_enuErrorStatus = USART_INV_MODULE;
    }
    else if ((copy_enuStopBitConfig != ONE_HALF_STOP_BIT) && (copy_enuStopBitConfig !=  ONE_STOP_BIT) && (copy_enuStopBitConfig !=  TWO_STOP_BIT) && (copy_enuStopBitConfig !=  HALF_STOP_BIT))
    {
        ret_enuErrorStatus = USART_INV_STOP_BIT;
    }
    else 
    {
        volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

        USARTx->CR2 = ((USARTx->CR2 & 0xFFFFCFFFUL) | (copy_enuStopBitConfig << 12));
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MUSART_enuSetParitySel(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuParitySelection_t copy_enuParitySel)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if ((copy_enuUSARTModule != USART1) && (copy_enuUSARTModule != USART2) && (copy_enuUSARTModule != USART6))
    {
        ret_enuErrorStatus = USART_INV_MODULE;
    }
    else if ((copy_enuParitySel != EVEN_PARITY) && (copy_enuParitySel !=  ODD_PARITY))
    {
        ret_enuErrorStatus = USART_INV_WORD_LEN;
    }
    else 
    {
        volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

        if (copy_enuParitySel == EVEN_PARITY)
        {
            USARTx->CR1 &= ~(1U << 9);
        }
        else if (copy_enuParitySel == ODD_PARITY)
        {
            USARTx->CR1 |= (1U << 9);
        }
        else {}
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MUSART_enuSendBytePolling(MUSART_enuModule_t copy_enuUSARTModule, uint8_t copy_uint8Data)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    uint16_t local_uint16Timeout = USART_POLLING_TIMEOUT;

    if ((copy_enuUSARTModule != USART1) && (copy_enuUSARTModule != USART2) && (copy_enuUSARTModule != USART6))
    {
        ret_enuErrorStatus = USART_INV_MODULE;
    }
    else 
    {
        volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

        /* wait until transmit buffer is empty */
        while ((MUSART_boolIsTransmitDREmpty(copy_enuUSARTModule) == 0) && (--local_uint16Timeout));

        /* if clk is supposed to be on && timeout reached 0 -> return error status */
        if (local_uint16Timeout == 0)
        {
            ret_enuErrorStatus = USART_TIMEOUT;
        }
        else 
        {
            /* write data into USART's DR */
            USARTx->DR = (copy_uint8Data);
        }
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MUSART_enuReceiveBytePolling(MUSART_enuModule_t copy_enuUSARTModule, uint8_t* ptr_uint8Data)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    uint16_t local_uint16Timeout = USART_POLLING_TIMEOUT;

    if ((copy_enuUSARTModule != USART1) && (copy_enuUSARTModule != USART2) && (copy_enuUSARTModule != USART6))
    {
        ret_enuErrorStatus = USART_INV_MODULE;
    }
    else if (ptr_uint8Data == NULL)
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else 
    {
        volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

        /* wait until transmit buffer is empty */
        while ((MUSART_boolIsReadDRNotEmpty(copy_enuUSARTModule) == false) && (--local_uint16Timeout));

        /* if clk is supposed to be on && timeout reached 0 -> return error status */
        if (local_uint16Timeout == 0)
        {
            ret_enuErrorStatus = USART_TIMEOUT;
        }
        else 
        {
            /* read data from USART's DR */
            (*ptr_uint8Data) = (uint8_t)(USARTx->DR & 0xFF);
        }
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MUSART_enuRegisterCBF(MUSART_enuModule_t copy_enuUSARTModule, MUSART_ptrCBF ptr_CBF, MUSART_enuIRQSources_t copy_enuIRQSource)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if ((copy_enuUSARTModule != USART1) && (copy_enuUSARTModule != USART2) && (copy_enuUSARTModule != USART6))
    {
        ret_enuErrorStatus = USART_INV_MODULE;
    }
    else if (ptr_CBF == NULL)
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else if (copy_enuIRQSource > IRQ_NOISE_FLAG)
    {
        ret_enuErrorStatus = USART_INV_IRQ;
    }
    else 
    {
        volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

        switch (copy_enuUSARTModule)
        {
            case USART1:
                MUSART1_CBFArr[copy_enuIRQSource] = ptr_CBF;
            break;

            case USART2:
                MUSART2_CBFArr[copy_enuIRQSource] = ptr_CBF;
            break;

            case USART6:
                MUSART6_CBFArr[copy_enuIRQSource] = ptr_CBF;
            break;
        }
    }

    return ret_enuErrorStatus;
}

static void MUSART_voidToggleSynchModeOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModeConfig_t copy_enuModeState)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

    if (copy_enuModeState == USART_MODE_ON)
    {
        /* clear LINEN + clear SCEN + clear HDSEL + clear IREN */
        MUSART_voidToggleSmartCardModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);
        MUSART_voidToggleSingleWireModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);
        MUSART_voidToggleIrDAModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);
        MUSART_voidToggleLINModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);

        USARTx->CR2 |= (1 << 11);
    }
    else if (copy_enuModeState == USART_MODE_OFF)
    {
        USARTx->CR2 &= ~(1 << 11);
    }
    else {}
}
/* LINEN */
static void MUSART_voidToggleLINModeOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModeConfig_t copy_enuModeState)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

    if (copy_enuModeState == USART_MODE_ON)
    {
        /* disable SCEN & HDSEL (cr3) */
        MUSART_voidToggleSmartCardModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);
        MUSART_voidToggleSingleWireModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);
        MUSART_voidToggleIrDAModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);

        /* disable STOP[1:0] + CLKEN (cr2) */
        MUSART_voidToggleSynchModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);
        USARTx->CR2 &= ~(0b11 << 12);

        USARTx->CR2 |= (1 << 14);
    }
    else if (copy_enuModeState == USART_MODE_OFF)
    {
        USARTx->CR2 &= ~(1 << 14);
    }
    else {}
}
/* DMAT */
static void MUSART_voidToggleDMATxOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModeConfig_t copy_enuModeState)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

    if (copy_enuModeState == USART_MODE_ON)
    {
        USARTx->CR3 |= (1 << 7);
    }
    else if (copy_enuModeState == USART_MODE_OFF)
    {
        USARTx->CR3 &= ~(1 << 7);
    }
    else {}
}
/* DMAR */
static void MUSART_voidToggleDMARxOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModeConfig_t copy_enuModeState)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

    if (copy_enuModeState == USART_MODE_ON)
    {
        USARTx->CR3 |= (1 << 6);
    }
    else if (copy_enuModeState == USART_MODE_OFF)
    {
        USARTx->CR3 &= ~(1 << 6);
    }
    else {}
}
/* SCEN */
static void MUSART_voidToggleSmartCardModeOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModeConfig_t copy_enuModeState)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

    if (copy_enuModeState == USART_MODE_ON)
    {
        /* LINEN (cr2) + HDSEL + IREN (cr3) */
        MUSART_voidToggleLINModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);
        MUSART_voidToggleSingleWireModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);
        MUSART_voidToggleIrDAModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);

        USARTx->CR3 |= (1 << 5);
    }
    else if (copy_enuModeState == USART_MODE_OFF)
    {
        USARTx->CR3 &= ~(1 << 5);
    }
    else {}
}
/* HDSEL */
static void MUSART_voidToggleSingleWireModeOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModeConfig_t copy_enuModeState)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

    if (copy_enuModeState == USART_MODE_ON)
    {
        /* LIN + CLKEN (cr2) + SCEN + IREN (cr3) */
        MUSART_voidToggleSmartCardModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);
        MUSART_voidToggleLINModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);
        MUSART_voidToggleIrDAModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);

        /* disable CLKEN (cr2) */
        MUSART_voidToggleSynchModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);

        USARTx->CR3 |= (1 << 3);
    }
    else if (copy_enuModeState == USART_MODE_OFF)
    {
        USARTx->CR3 &= ~(1 << 3);
    }
    else {}
}
/* IREN (IrDA) */
static void MUSART_voidToggleIrDAModeOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModeConfig_t copy_enuModeState)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

    if (copy_enuModeState == USART_MODE_ON)
    {
        /* disable SCEN & HDSEL (cr3) */
        MUSART_voidToggleSmartCardModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);
        MUSART_voidToggleSingleWireModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);

        /* disable LINEN + STOP + CLKEN (cr2) */
        MUSART_voidToggleLINModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);
        MUSART_voidToggleSynchModeOnOff(copy_enuUSARTModule, USART_MODE_OFF);
        USARTx->CR2 &= ~(0b11 << 12);

        USARTx->CR3 |= (1 << 1);
    }
    else if (copy_enuModeState == USART_MODE_OFF)
    {
        USARTx->CR3 &= ~(1 << 1);
    }
    else {}
}
/* cts && rts */
static void MUSART_voidToggleHWFlowControlModeOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModeConfig_t copy_enuModeState)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

    if (copy_enuModeState == USART_MODE_ON)
    {
        USARTx->CR3 |= ((1 << 9) | (1 << 8));
    }
    else if (copy_enuModeState == USART_MODE_OFF)
    {
        USARTx->CR3 &= ~((1 << 9) | (1 << 8));
    }
    else {}
}

static void MUSART_voidToggleIRQTransmitDataRegEmpty(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuIRQConfig_t copy_enuIRQState)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

    if (copy_enuIRQState == USART_IRQ_EN)
    {
        USARTx->CR1 |= (1 << 7);
    }
    else if (copy_enuIRQState == USART_IRQ_DIS)
    {
        USARTx->CR1 &= ~(1 << 7);
    }
    else {}
}
static void MUSART_voidToggleIRQCTS(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuIRQConfig_t copy_enuIRQState)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

    if (copy_enuIRQState == USART_IRQ_EN)
    {
        USARTx->CR3 |= (1 << 10);
    }
    else if (copy_enuIRQState == USART_IRQ_DIS)
    {
        USARTx->CR3 &= ~(1 << 10);
    }
    else {}
}
static void MUSART_voidToggleIRQTransmissionComplete(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuIRQConfig_t copy_enuIRQState)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

    if (copy_enuIRQState == USART_IRQ_EN)
    {
        USARTx->CR1 |= (1 << 6);
    }
    else if (copy_enuIRQState == USART_IRQ_DIS)
    {
        USARTx->CR1 &= ~(1 << 6);
    }
    else {}
}
static void MUSART_voidToggleIRQReceiveDataReadyToBeRead(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuIRQConfig_t copy_enuIRQState)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

    if (copy_enuIRQState == USART_IRQ_EN)
    {
        USARTx->CR1 |= (1 << 5);
    }
    else if (copy_enuIRQState == USART_IRQ_DIS)
    {
        USARTx->CR1 &= ~(1 << 5);
    }
    else {}
}
static void MUSART_voidToggleIRQOverrunErrDetected(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuIRQConfig_t copy_enuIRQState)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

    if (copy_enuIRQState == USART_IRQ_EN)
    {
        USARTx->CR3 |= (1 << 0);
    }
    else if (copy_enuIRQState == USART_IRQ_DIS)
    {
        USARTx->CR3 &= ~(1 << 0);
    }
    else {}
}
static void MUSART_voidToggleIRQIdleLineDetected(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuIRQConfig_t copy_enuIRQState)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

    if (copy_enuIRQState == USART_IRQ_EN)
    {
        USARTx->CR1 |= (1 << 4);
    }
    else if (copy_enuIRQState == USART_IRQ_DIS)
    {
        USARTx->CR1 &= ~(1 << 4);
    }
    else {}
}
static void MUSART_voidToggleIRQParityErr(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuIRQConfig_t copy_enuIRQState)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

    if (copy_enuIRQState == USART_IRQ_EN)
    {
        USARTx->CR1 |= (1 << 8);
    }
    else if (copy_enuIRQState == USART_IRQ_DIS)
    {
        USARTx->CR1 &= ~(1 << 8);
    }
    else {}
}
static void MUSART_voidToggleIRQLINBreakDetection(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuIRQConfig_t copy_enuIRQState)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

    if (copy_enuIRQState == USART_IRQ_EN)
    {
        USARTx->CR2 |= (1 << 6);
    }
    else if (copy_enuIRQState == USART_IRQ_DIS)
    {
        USARTx->CR2 &= ~(1 << 6);
    }
    else {}
}
static void MUSART_voidToggleIRQNoise(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuIRQConfig_t copy_enuIRQState)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

    if (copy_enuIRQState == USART_IRQ_EN)
    {
        USARTx->CR3 |= (1 << 0);
    }
    else if (copy_enuIRQState == USART_IRQ_DIS)
    {
        USARTx->CR3 &= ~(1 << 0);
    }
    else {}
}

static uint8_t MUSART_uint8GetOverSampleMode(MUSART_enuModule_t copy_enuUSARTModule)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);
    return ((USARTx->CR1 >> 15) & 1U);
}

static void MUSART_voidEveryIRQHandler(MUSART_enuModule_t copy_enuUSARTModule)
{
    volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(copy_enuUSARTModule);

    if (IS_TRANSMIT_DATA_REGISTER_EMPTY_FLAG_UP(USARTx->SR))
    {
        /* clear TXE flag */
        USARTx->SR &= ~(1U << 7);

        /* call CBF if it is not null */
        switch (copy_enuUSARTModule)
        {
            case USART1:
                if (MUSART1_CBFArr[IRQ_TRANSMIT_DATA_REG_EMPTY] != NULL)
                {
                    MUSART1_CBFArr[IRQ_TRANSMIT_DATA_REG_EMPTY]();
                }
                else {}
            break;

            case USART2:
                if (MUSART2_CBFArr[IRQ_TRANSMIT_DATA_REG_EMPTY] != NULL)
                {
                    MUSART2_CBFArr[IRQ_TRANSMIT_DATA_REG_EMPTY]();
                }
                else {}
            break;

            case USART6:
                if (MUSART6_CBFArr[IRQ_TRANSMIT_DATA_REG_EMPTY] != NULL)
                {
                    MUSART6_CBFArr[IRQ_TRANSMIT_DATA_REG_EMPTY]();
                }
                else {}
            break;
        }
    }
    else if (IS_TRANSMISSION_COMPLETE_FLAG_UP(USARTx->SR))
    {
        /* clear TC flag */
        USARTx->SR &= ~(1U << 6);

        /* call CBF if it is not null */
        switch (copy_enuUSARTModule)
        {
            case USART1:
                if (MUSART1_CBFArr[IRQ_TRANSMISSION_COMPLETE] != NULL)
                {
                    MUSART1_CBFArr[IRQ_TRANSMISSION_COMPLETE]();
                }
                else {}
            break;

            case USART2:
                if (MUSART2_CBFArr[IRQ_TRANSMISSION_COMPLETE] != NULL)
                {
                    MUSART2_CBFArr[IRQ_TRANSMISSION_COMPLETE]();
                }
                else {}
            break;

            case USART6:
                if (MUSART6_CBFArr[IRQ_TRANSMISSION_COMPLETE] != NULL)
                {
                    MUSART6_CBFArr[IRQ_TRANSMISSION_COMPLETE]();
                }
                else {}
            break;
        }
    }
    else if (IS_RECEIVED_DATA_RDY_TO_BE_READ_FLAG_UP(USARTx->SR))
    {
        /* clear RXNE flag */
        USARTx->SR &= ~(1U << 5);

        /* call CBF if it is not null */
        switch (copy_enuUSARTModule)
        {
            case USART1:
                if (MUSART1_CBFArr[IRQ_RECEIVE_DATA_RDY_TO_BE_READ] != NULL)
                {
                    MUSART1_CBFArr[IRQ_RECEIVE_DATA_RDY_TO_BE_READ]();
                }
                else {}
            break;

            case USART2:
                if (MUSART2_CBFArr[IRQ_RECEIVE_DATA_RDY_TO_BE_READ] != NULL)
                {
                    MUSART2_CBFArr[IRQ_RECEIVE_DATA_RDY_TO_BE_READ]();
                }
                else {}
            break;

            case USART6:
                if (MUSART6_CBFArr[IRQ_RECEIVE_DATA_RDY_TO_BE_READ] != NULL)
                {
                    MUSART6_CBFArr[IRQ_RECEIVE_DATA_RDY_TO_BE_READ]();
                }
                else {}
            break;
        }
    }
    else if (IS_CTS_FLAG_UP(USARTx->SR))
    {
        /* clear CTS flag */
        USARTx->SR &= ~(1U << 9);

        /* call CBF if it is not null */
        switch (copy_enuUSARTModule)
        {
            case USART1:
                if (MUSART1_CBFArr[IRQ_CTS_FLAG] != NULL)
                {
                    MUSART1_CBFArr[IRQ_CTS_FLAG]();
                }
                else {}
            break;

            case USART2:
                if (MUSART2_CBFArr[IRQ_CTS_FLAG] != NULL)
                {
                    MUSART2_CBFArr[IRQ_CTS_FLAG]();
                }
                else {}
            break;

            case USART6:
                if (MUSART6_CBFArr[IRQ_CTS_FLAG] != NULL)
                {
                    MUSART6_CBFArr[IRQ_CTS_FLAG]();
                }
                else {}
            break;
        }
    }
    else if (IS_OVERRUN_ERR_DETECTED_FLAG_UP(USARTx->SR))
    {
        /* clear ORE flag */
        USARTx->SR &= ~(1U << 3);

        /* call CBF if it is not null */
        switch (copy_enuUSARTModule)
        {
            case USART1:
                if (MUSART1_CBFArr[IRQ_OVERRUN_ERR_DETECTED] != NULL)
                {
                    MUSART1_CBFArr[IRQ_OVERRUN_ERR_DETECTED]();
                }
                else {}
            break;

            case USART2:
                if (MUSART2_CBFArr[IRQ_OVERRUN_ERR_DETECTED] != NULL)
                {
                    MUSART2_CBFArr[IRQ_OVERRUN_ERR_DETECTED]();
                }
                else {}
            break;

            case USART6:
                if (MUSART6_CBFArr[IRQ_OVERRUN_ERR_DETECTED] != NULL)
                {
                    MUSART6_CBFArr[IRQ_OVERRUN_ERR_DETECTED]();
                }
                else {}
            break;
        }
    }
    else if (IS_IDLE_LINE_DETECTED_FLAG_UP(USARTx->SR))
    {
        /* clear IDLE flag */
        USARTx->SR &= ~(1U << 4);

        /* call CBF if it is not null */
        switch (copy_enuUSARTModule)
        {
            case USART1:
                if (MUSART1_CBFArr[IRQ_IDLE_LINE_DETECTED] != NULL)
                {
                    MUSART1_CBFArr[IRQ_IDLE_LINE_DETECTED]();
                }
                else {}
            break;

            case USART2:
                if (MUSART2_CBFArr[IRQ_IDLE_LINE_DETECTED] != NULL)
                {
                    MUSART2_CBFArr[IRQ_IDLE_LINE_DETECTED]();
                }
                else {}
            break;

            case USART6:
                if (MUSART6_CBFArr[IRQ_IDLE_LINE_DETECTED] != NULL)
                {
                    MUSART6_CBFArr[IRQ_IDLE_LINE_DETECTED]();
                }
                else {}
            break;
        }
    }
    else if (IS_PARITY_ERR_FLAG_UP(USARTx->SR))
    {
        /* clear PE flag */
        USARTx->SR &= ~(1U << 0);

        /* call CBF if it is not null */
        switch (copy_enuUSARTModule)
        {
            case USART1:
                if (MUSART1_CBFArr[IRQ_PARITY_ERR] != NULL)
                {
                    MUSART1_CBFArr[IRQ_PARITY_ERR]();
                }
                else {}
            break;

            case USART2:
                if (MUSART2_CBFArr[IRQ_PARITY_ERR] != NULL)
                {
                    MUSART2_CBFArr[IRQ_PARITY_ERR]();
                }
                else {}
            break;

            case USART6:
                if (MUSART6_CBFArr[IRQ_PARITY_ERR] != NULL)
                {
                    MUSART6_CBFArr[IRQ_PARITY_ERR]();
                }
                else {}
            break;
        }
    }
    else if (IS_LIN_BREAK_FLAG_UP(USARTx->SR))
    {
        /* clear LBD flag */
        USARTx->SR &= ~(1U << 9);

        /* call CBF if it is not null */
        switch (copy_enuUSARTModule)
        {
            case USART1:
                if (MUSART1_CBFArr[IRQ_BREAK_FLAG] != NULL)
                {
                    MUSART1_CBFArr[IRQ_BREAK_FLAG]();
                }
                else {}
            break;

            case USART2:
                if (MUSART2_CBFArr[IRQ_BREAK_FLAG] != NULL)
                {
                    MUSART2_CBFArr[IRQ_BREAK_FLAG]();
                }
                else {}
            break;

            case USART6:
                if (MUSART6_CBFArr[IRQ_BREAK_FLAG] != NULL)
                {
                    MUSART6_CBFArr[IRQ_BREAK_FLAG]();
                }
                else {}
            break;
        }
    }
    else if (IS_NOISE_OR_OVERRUN_PLUS_FRAMING_ERR_IN_MULTIBUF_COMM_FLAG_UP(USARTx->SR))
    {
        /* clear NF && ORE && FE flag */
        USARTx->SR &= ~((1U << 3) | (1U << 2) | (1U << 1));

        /* call CBF if it is not null */
        switch (copy_enuUSARTModule)
        {
            case USART1:
                if (MUSART1_CBFArr[IRQ_NOISE_FLAG] != NULL)
                {
                    MUSART1_CBFArr[IRQ_NOISE_FLAG]();
                }
                else {}
            break;

            case USART2:
                if (MUSART2_CBFArr[IRQ_NOISE_FLAG] != NULL)
                {
                    MUSART2_CBFArr[IRQ_NOISE_FLAG]();
                }
                else {}
            break;

            case USART6:
                if (MUSART6_CBFArr[IRQ_NOISE_FLAG] != NULL)
                {
                    MUSART6_CBFArr[IRQ_NOISE_FLAG]();
                }
                else {}
            break;
        }
    }
    else {}

}

void USART1_IRQHandler(void)
{
    /* should handle each type of interrupt from USART1 here */
    // MUSART_voidEveryIRQHandler(USART1);
    MUSART1_CBFArr[IRQ_RECEIVE_DATA_RDY_TO_BE_READ]();
}

void USART2_IRQHandler(void)
{
    /* should handle each type of interrupt from USART2 here */
    MUSART_voidEveryIRQHandler(USART2);
}

void USART6_IRQHandler(void)
{
    /* should handle each type of interrupt from USART6 here */
    MUSART_voidEveryIRQHandler(USART6);
}
