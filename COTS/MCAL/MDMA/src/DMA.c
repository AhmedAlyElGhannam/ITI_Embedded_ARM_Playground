#include "std_types.h"
#include "std_lib.h"
#include "error_status.h"
#include "RCC.h"
#include "DMA_Reg.h"
#include "DMA.h"

#define MDMA_MAX_ITEM_NUM_TO_TRANSFER   (0xFFFFU)
#define IS_DMA_IRQ_FLAG_RAISED(X)       ((X) == 1U)
#define DMA_TRANSMISSION_TIMEOUT        (1000U)

static MDMA_ptrCBF MDMA_ptrCBFArr[NUM_OF_DMA_MODULES][NUM_OF_DMA_STREAMS][NUM_OF_DMA_IRQs] = 
{
    [DMA1] = {{NULL}}, 
    [DMA2] = {{NULL}}
};

static void MDMA_voidClrHalfTransferIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream);
static void MDMA_voidClrTransferCompleteIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream);
static void MDMA_voidClrTransferErrIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream);
static void MDMA_voidClrDirectModeErrIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream);
static void MDMA_voidClrFIFOErrIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream);
static void MDMA_voidGetHalfTransferIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, uint8_t* ptr_uint8IF);
static void MDMA_voidGetTransferCompleteIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, uint8_t* ptr_uint8IF);
static void MDMA_voidGetTransferErrIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, uint8_t* ptr_uint8IF);
static void MDMA_voidGetDirectModeErrIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, uint8_t* ptr_uint8IF);
static void MDMA_voidGetFIFOErrIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, uint8_t* ptr_uint8IF);
static void MDMA_voidStreamChannelSel(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuChannel_t copy_enuChannel);

SRV_enuErrorStatus_t MDMA_enuEnableClk(MDMA_enuDMAController_t copy_enuDMAModule)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    switch (copy_enuDMAModule)
    {
        case DMA1:
            MRCC_enuSetPeripheralClkState(MRCC_AHB1_DMA1, MRCC_PERIPHERAL_CLK_ENABLE);
        break;

        case DMA2:
            MRCC_enuSetPeripheralClkState(MRCC_AHB1_DMA2, MRCC_PERIPHERAL_CLK_ENABLE);
        break;

        default:
            ret_enuErrorStatus = DMA_INV_MODULE;
        break;
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuDisableClk(MDMA_enuDMAController_t copy_enuDMAModule)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    switch (copy_enuDMAModule)
    {
        case DMA1:
            MRCC_enuSetPeripheralClkState(MRCC_AHB1_DMA1, MRCC_PERIPHERAL_CLK_DISABLE);
        break;

        case DMA2:
            MRCC_enuSetPeripheralClkState(MRCC_AHB1_DMA2, MRCC_PERIPHERAL_CLK_DISABLE);
        break;

        default:
            ret_enuErrorStatus = DMA_INV_MODULE;
        break;
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MDMA_enuClrIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuDMAIRQs_t copy_enuIRQ)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if (copy_enuIRQ > DMA_DIRECT_MODE_ERR)
    {
        ret_enuErrorStatus = DMA_INV_IRQ;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        switch (copy_enuIRQ)
        {
            case DMA_HALF_TRANSFER_INT:
                MDMA_voidClrHalfTransferIRQFlag(copy_enuDMAModule, copy_enuStream);
            break;

            case DMA_TRANSFER_COMPLETE_INT:
                MDMA_voidClrTransferCompleteIRQFlag(copy_enuDMAModule, copy_enuStream);
            break;

            case DMA_TRANSFER_ERR_INT:
                MDMA_voidClrTransferErrIRQFlag(copy_enuDMAModule, copy_enuStream);
            break;

            case DMA_FIFO_OVERRUN_UNDERRUN:
                MDMA_voidClrDirectModeErrIRQFlag(copy_enuDMAModule, copy_enuStream);
            break;

            case DMA_DIRECT_MODE_ERR:
                MDMA_voidClrFIFOErrIRQFlag(copy_enuDMAModule, copy_enuStream);
            break;
        }
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuGetIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuDMAIRQs_t copy_enuIRQ, uint8_t* ptr_uint8IF)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if (copy_enuIRQ > DMA_DIRECT_MODE_ERR)
    {
        ret_enuErrorStatus = DMA_INV_IRQ;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else if (ptr_uint8IF == NULL)
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else 
    {
        switch (copy_enuIRQ)
        {
            case DMA_HALF_TRANSFER_INT:
                MDMA_voidGetHalfTransferIRQFlag(copy_enuDMAModule, copy_enuStream, ptr_uint8IF);
            break;

            case DMA_TRANSFER_COMPLETE_INT:
                MDMA_voidGetTransferCompleteIRQFlag(copy_enuDMAModule, copy_enuStream, ptr_uint8IF);
            break;

            case DMA_TRANSFER_ERR_INT:
                MDMA_voidGetTransferErrIRQFlag(copy_enuDMAModule, copy_enuStream, ptr_uint8IF);
            break;

            case DMA_FIFO_OVERRUN_UNDERRUN:
                MDMA_voidGetDirectModeErrIRQFlag(copy_enuDMAModule, copy_enuStream, ptr_uint8IF);
            break;

            case DMA_DIRECT_MODE_ERR:
                MDMA_voidGetFIFOErrIRQFlag(copy_enuDMAModule, copy_enuStream, ptr_uint8IF);
            break;
        }
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MDMA_enuStreamEnable(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxCR |= (1U << 0);
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamDisable(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxCR &= ~(1UL << 0);
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MDMA_enuStreamIRQEnable(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuDMAIRQs_t copy_enuIRQ)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if (copy_enuIRQ > DMA_DIRECT_MODE_ERR)
    {
        ret_enuErrorStatus = DMA_INV_IRQ;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        switch (copy_enuIRQ)
        {
            case DMA_HALF_TRANSFER_INT:
                DMAx->Streamx[copy_enuStream].SxCR |= (1U << 3);
            break;

            case DMA_TRANSFER_COMPLETE_INT:
                DMAx->Streamx[copy_enuStream].SxCR |= (1U << 4);
            break;

            case DMA_TRANSFER_ERR_INT:
                DMAx->Streamx[copy_enuStream].SxCR |= (1U << 2);
            break;

            case DMA_FIFO_OVERRUN_UNDERRUN:
                DMAx->Streamx[copy_enuStream].SxFCR |= (1U << 7);
            break;

            case DMA_DIRECT_MODE_ERR:
                DMAx->Streamx[copy_enuStream].SxCR |= (1U << 1);
            break;
        }
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamIRQDisable(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuDMAIRQs_t copy_enuIRQ)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if (copy_enuIRQ > DMA_DIRECT_MODE_ERR)
    {
        ret_enuErrorStatus = DMA_INV_IRQ;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        switch (copy_enuIRQ)
        {
            case DMA_HALF_TRANSFER_INT:
                DMAx->Streamx[copy_enuStream].SxCR &= ~(1U << 3);
            break;

            case DMA_TRANSFER_COMPLETE_INT:
                DMAx->Streamx[copy_enuStream].SxCR &= ~(1U << 4);
            break;

            case DMA_TRANSFER_ERR_INT:
                DMAx->Streamx[copy_enuStream].SxCR &= ~(1U << 2);
            break;

            case DMA_FIFO_OVERRUN_UNDERRUN:
                DMAx->Streamx[copy_enuStream].SxFCR &= ~(1U << 7);
            break;

            case DMA_DIRECT_MODE_ERR:
                DMAx->Streamx[copy_enuStream].SxCR &= ~(1U << 1);
            break;
        }
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MDMA_enuStreamSetChannelPeripheralRequest(MDMA_enuDMAPeripheralRequest_t copy_enuRequest)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    switch (copy_enuRequest)
    {
        case DMA1_STREAM0_CHANNEL0_SPI3_RX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM0, DMA_CHANNEL0);
        break;

        case DMA1_STREAM0_CHANNEL1_I2C1_RX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM0, DMA_CHANNEL1);
        break;

        case DMA1_STREAM0_CHANNEL2_TIM4_CH1: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM0, DMA_CHANNEL2);
        break;

        case DMA1_STREAM0_CHANNEL3_I2S3_EXT_RX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM0, DMA_CHANNEL3);
        break;

        case DMA1_STREAM0_CHANNEL6_TIM5_CH3_AND_TIM5_UP: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM0, DMA_CHANNEL6);
        break;

        case DMA1_STREAM1_CHANNEL1_I2C3_RX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM1, DMA_CHANNEL1);
        break;

        case DMA1_STREAM1_CHANNEL3_TIM2_CH3_AND_TIM2_UP: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM1, DMA_CHANNEL3);
        break;

        case DMA1_STREAM1_CHANNEL6_TIM5_CH4_AND_TIM5_TRIG: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM1, DMA_CHANNEL6);
        break;

        case DMA1_STREAM2_CHANNEL0_SPI3_RX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM2, DMA_CHANNEL0);
        break;

        case DMA1_STREAM2_CHANNEL2_I2S3_EXT_RX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM2, DMA_CHANNEL2);
        break;

        case DMA1_STREAM2_CHANNEL3_I2C3_RX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM2, DMA_CHANNEL3);
        break;

        case DMA1_STREAM2_CHANNEL5_TIM3_CH4_AND_TIM3_UP: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM2, DMA_CHANNEL5);
        break;

        case DMA1_STREAM2_CHANNEL6_TIM5_CH1: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM2, DMA_CHANNEL6);
        break;

        case DMA1_STREAM2_CHANNEL7_I2C2_RX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM2, DMA_CHANNEL7);
        break;

        case DMA1_STREAM3_CHANNEL0_SPI2_RX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM3, DMA_CHANNEL0);
        break;

        case DMA1_STREAM3_CHANNEL2_TIM4_CH2: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM3, DMA_CHANNEL2);
        break;

        case DMA1_STREAM3_CHANNEL3_I2S2_EXT_RX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM3, DMA_CHANNEL3);
        break;

        case DMA1_STREAM3_CHANNEL6_TIM5_CH4_AND_TIM5_TRIG: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM3, DMA_CHANNEL6);
        break;

        case DMA1_STREAM3_CHANNEL7_I2C2_RX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM3, DMA_CHANNEL7);
        break;

        case DMA1_STREAM4_CHANNEL0_SPI2_TX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM4, DMA_CHANNEL0);
        break;

        case DMA1_STREAM4_CHANNEL2_I2S2_EXT_TX:
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM4, DMA_CHANNEL2);
        break;

        case DMA1_STREAM4_CHANNEL3_I2C3_TX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM4, DMA_CHANNEL3);
        break;

        case DMA1_STREAM4_CHANNEL5_TIM3_CH1_AND_TIM3_TRIG: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM4, DMA_CHANNEL5);
        break;

        case DMA1_STREAM4_CHANNEL6_TIM5_CH2: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM4, DMA_CHANNEL6);
        break;

        case DMA1_STREAM5_CHANNEL0_SPI3_TX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM5, DMA_CHANNEL0);
        break;

        case DMA1_STREAM5_CHANNEL1_I2C1_RX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM5, DMA_CHANNEL1);
        break;

        case DMA1_STREAM5_CHANNEL2_I2S3_EXT_TX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM5, DMA_CHANNEL2);
        break;

        case DMA1_STREAM5_CHANNEL3_TIM2_CH1: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM5, DMA_CHANNEL3);
        break;

        case DMA1_STREAM5_CHANNEL4_USART2_RX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM5, DMA_CHANNEL4);
        break;

        case DMA1_STREAM5_CHANNEL5_TIM3_CH2: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM5, DMA_CHANNEL5);
        break;

        case DMA1_STREAM5_CHANNEL6_I2C3_TX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM5, DMA_CHANNEL6);
        break;

        case DMA1_STREAM6_CHANNEL1_I2C1_TX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM6, DMA_CHANNEL1);
        break;

        case DMA1_STREAM6_CHANNEL2_IM4_UP: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM6, DMA_CHANNEL2);
        break;

        case DMA1_STREAM6_CHANNEL3_TIM2_CH2_AND_TIM2_CH4: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM6, DMA_CHANNEL3);
        break;

        case DMA1_STREAM6_CHANNEL4_USART2_TX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM6, DMA_CHANNEL4);
        break;

        case DMA1_STREAM6_CHANNEL6_TIM5_UP: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM6, DMA_CHANNEL6);
        break;

        case DMA1_STREAM7_CHANNEL0_SPI3_TX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM7, DMA_CHANNEL0);
        break;

        case DMA1_STREAM7_CHANNEL1_I2C1_TX:     
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM7, DMA_CHANNEL1);
        break;

        case DMA1_STREAM7_CHANNEL2_TIM4_CH3: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM7, DMA_CHANNEL2);
        break;

        case DMA1_STREAM7_CHANNEL3_TIM2_UP_AND_TIM2_CH4: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM7, DMA_CHANNEL3);
        break;

        case DMA1_STREAM7_CHANNEL5_TIM3_CH3: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM7, DMA_CHANNEL5);
        break;

        case DMA1_STREAM7_CHANNEL7_I2C_TX: 
            MDMA_voidStreamChannelSel(DMA1, DMA_STREAM7, DMA_CHANNEL7);
        break;

        case DMA2_STREAM0_CHANNEL0_ADC1: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM0, DMA_CHANNEL0);
        break;

        case DMA2_STREAM0_CHANNEL3_SPI1_RX: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM0, DMA_CHANNEL3);
        break;

        case DMA2_STREAM0_CHANNEL4_SPI4_RX: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM0, DMA_CHANNEL4);
        break;

        case DMA2_STREAM0_CHANNEL6_TIM1_TRIG: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM0, DMA_CHANNEL6);
        break;

        case DMA2_STREAM1_CHANNEL4_SPI4_TX: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM1, DMA_CHANNEL4);
        break;

        case DMA2_STREAM1_CHANNEL5_USART6_RX: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM1, DMA_CHANNEL5);
        break;

        case DMA2_STREAM1_CHANNEL6_TIM1_CH1: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM1, DMA_CHANNEL6);
        break;

        case DMA2_STREAM2_CHANNEL3_SPI1_RX: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM2, DMA_CHANNEL3);
        break;

        case DMA2_STREAM2_CHANNEL4_USART1_RX: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM2, DMA_CHANNEL4);
        break;

        case DMA2_STREAM2_CHANNEL5_USART6_RX: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM2, DMA_CHANNEL5);
        break;

        case DMA2_STREAM2_CHANNEL6_TIM1_CH2: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM2, DMA_CHANNEL6);
        break;

        case DMA2_STREAM3_CHANNEL3_SPI1_TX: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM3, DMA_CHANNEL3);
        break;

        case DMA2_STREAM3_CHANNEL4_SDIO: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM3, DMA_CHANNEL4);
        break;

        case DMA2_STREAM3_CHANNEL5_SPI4_RX: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM3, DMA_CHANNEL5);
        break;

        case DMA2_STREAM3_CHANNEL6_TIM1_CH1: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM3, DMA_CHANNEL6);
        break;

        case DMA2_STREAM4_CHANNEL0_ADC1: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM4, DMA_CHANNEL0);
        break;

        case DMA2_STREAM4_CHANNEL5_SPI4_TX: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM4, DMA_CHANNEL5);
        break;

        case DMA2_STREAM4_CHANNEL6_TIM1_CH4_AND_TIM1_TRIG_AND_TIM1_COM: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM4, DMA_CHANNEL6);
        break;

        case DMA2_STREAM5_CHANNEL3_SPI1_TX: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM5, DMA_CHANNEL3);
        break;

        case DMA2_STREAM5_CHANNEL4_USART1_RX: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM5, DMA_CHANNEL4);
        break;

        case DMA2_STREAM5_CHANNEL6_TIM1_UP: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM5, DMA_CHANNEL6);
        break;

        case DMA2_STREAM6_CHANNEL0_TIM1_CH1_AND_TIM1_CH2_AND_TIM1_CH3: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM6, DMA_CHANNEL0);
        break;

        case DMA2_STREAM6_CHANNEL4_SDIO: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM6, DMA_CHANNEL4);
        break;

        case DMA2_STREAM6_CHANNEL5_USART6_TX: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM6, DMA_CHANNEL5);
        break;

        case DMA2_STREAM6_CHANNEL6_TIM1_CH3: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM6, DMA_CHANNEL6);
        break;

        case DMA2_STREAM7_CHANNEL4_USART1_TX: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM7, DMA_CHANNEL4);
        break;

        case DMA2_STREAM7_CHANNEL5_USART6_TX: 
            MDMA_voidStreamChannelSel(DMA2, DMA_STREAM7, DMA_CHANNEL5);
        break;

        default:
            ret_enuErrorStatus = DMA_INV_PERIPHERAL_REQUEST;
        break;
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamPeripheralBurstTransferConfig(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuTransferConfig_t copy_enuPBURST)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if (copy_enuPBURST > DMA_INCR16)
    {
        ret_enuErrorStatus = DMA_INV_BURST;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxCR = (DMAx->Streamx[copy_enuStream].SxCR & ~(0b11 << 21)) | (copy_enuPBURST << 21);
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamMemoryBurstTransferConfig(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuTransferConfig_t copy_enuMBURST)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if (copy_enuMBURST > DMA_INCR16)
    {
        ret_enuErrorStatus = DMA_INV_BURST;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxCR = (DMAx->Streamx[copy_enuStream].SxCR & ~(0b11 << 23)) | (copy_enuMBURST << 23);
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamSetPriorityLvl(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuPriorityLevel_t copy_enuPriority)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if (copy_enuPriority > DMA_VHIGH)
    {
        ret_enuErrorStatus = DMA_INV_PRIORITY;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxCR = (DMAx->Streamx[copy_enuStream].SxCR & ~(0b11 << 16)) | (copy_enuPriority << 16);
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamDoubleBufferModeConfig(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuDoubleBufMode_t copy_enuDBM)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if ((copy_enuDBM != DBM_ENABLE) && (copy_enuDBM != DBM_DISABLE)) 
    {
        ret_enuErrorStatus = DMA_INV_DBM;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxCR = (DMAx->Streamx[copy_enuStream].SxCR & ~(1U << 18)) | (copy_enuDBM << 18);
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamSetCurrMemTargetDBM(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuDoubleBufCurrTarget_t copy_enuTarget)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if ((copy_enuTarget != TARGET_MEM_MEM0) && (copy_enuTarget != TARGET_MEM_MEM1)) 
    {
        ret_enuErrorStatus = DMA_INV_DBM;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxCR = (DMAx->Streamx[copy_enuStream].SxCR & ~(1U << 19)) | (copy_enuTarget << 19);
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamPeripheralIncOffsetSize(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuPeripheralIncOffsetSize_t copy_enuPINCOS)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if ((copy_enuPINCOS != LINKED_TO_PSIZE) && (copy_enuPINCOS != FIXED_TO_4)) 
    {
        ret_enuErrorStatus = DMA_INV_INC_OFF;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxCR = (DMAx->Streamx[copy_enuStream].SxCR & ~(1U << 15)) | (copy_enuPINCOS << 15);
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamMemDataSize(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuMemoryDataSize_t copy_enuMSIZE)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if (copy_enuMSIZE > MSIZE_WORD)
    {
        ret_enuErrorStatus = DMA_INV_DATA_SIZE;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxCR = (DMAx->Streamx[copy_enuStream].SxCR & ~(0b11 << 13)) | (copy_enuMSIZE << 13);
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamPeripheralDataSize(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuPeripheralDataSize_t copy_enuPSIZE)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if (copy_enuPSIZE > P_SIZE_WORD)
    {
        ret_enuErrorStatus = DMA_INV_DATA_SIZE;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxCR = (DMAx->Streamx[copy_enuStream].SxCR & ~(0b11 << 13)) | (copy_enuPSIZE << 13);
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamPeripheralIncMode(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuPeripheralIncMode_t copy_enuPINC)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if ((copy_enuPINC != PINC_FIXED) && (copy_enuPINC != PINC_ACCORDING_TO_PSIZE)) 
    {
        ret_enuErrorStatus = DMA_INV_INC_MODE;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxCR = (DMAx->Streamx[copy_enuStream].SxCR & ~(1U << 9)) | (copy_enuPINC << 9);
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamMemIncMode(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuMemIncMode_t copy_enuMINC)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if ((copy_enuMINC != MINC_FIXED) && (copy_enuMINC != MINC_ACCORDING_TO_MSIZE)) 
    {
        ret_enuErrorStatus = DMA_INV_INC_MODE;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxCR = (DMAx->Streamx[copy_enuStream].SxCR & ~(1U << 10)) | (copy_enuMINC << 10);
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamCircularMode(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuCircularModeConfig_t copy_enuCIRC)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if ((copy_enuCIRC != CIRC_ENABLE) && (copy_enuCIRC != CIRC_DISABLE)) 
    {
        ret_enuErrorStatus = DMA_INV_CIRC_MODE;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxCR = (DMAx->Streamx[copy_enuStream].SxCR & ~(1U << 8)) | (copy_enuCIRC << 8);
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamSetDataTransferDir(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuDataTransferMode_t copy_enuDIR)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if (copy_enuDIR > MEM_TO_MEM)
    {
        ret_enuErrorStatus = DMA_INV_TRANSFER_DIR;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxCR = (DMAx->Streamx[copy_enuStream].SxCR & ~(0b11 << 6)) | (copy_enuDIR << 6);
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamPeripheralFlowControllerConfig(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuPeripheralFlowController_t copy_enuPFCTRL)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if ((copy_enuPFCTRL != FLOW_CONTROLLER_IS_DMA) && (copy_enuPFCTRL != FLOW_CONTROLLER_IS_PER)) 
    {
        ret_enuErrorStatus = DMA_INV_PERIPHERAL_FLOW_CONTROLLER;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxCR = (DMAx->Streamx[copy_enuStream].SxCR & ~(1U << 5)) | (copy_enuPFCTRL << 5);
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamSetNumOfDataItemsToTransfer(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, uint16_t copy_uint16NumOfDataItems)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxNDTR = (DMAx->Streamx[copy_enuStream].SxNDTR & 0xFFFF0000UL) | copy_uint16NumOfDataItems;
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamSetPeripheralAddress(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, uint8_t* ptr_uint32PeripheralRegAddress)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else if (ptr_uint32PeripheralRegAddress == NULL)
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxPAR = ptr_uint32PeripheralRegAddress;
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamSetMem0Address(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, uint8_t* ptr_uint32Mem0Address)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else if (ptr_uint32Mem0Address == NULL)
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxM0AR = ptr_uint32Mem0Address;
    }

    return ret_enuErrorStatus;
}
/* only with double buffer mode */
SRV_enuErrorStatus_t MDMA_enuStreamSetMem1Address(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, uint8_t* ptr_uint32Mem1Address)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else if (ptr_uint32Mem1Address == NULL)
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxM1AR = ptr_uint32Mem1Address;
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamDirectModeConfig(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuDirectModeConfig_t copy_enuDMDIS)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if ((copy_enuDMDIS != DMDIS_ENABLE) && (copy_enuDMDIS != DMDIS_DISABLE)) 
    {
        ret_enuErrorStatus = DMA_INV_DMDIS;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxCR = (DMAx->Streamx[copy_enuStream].SxCR & ~(1U << 2)) | (copy_enuDMDIS << 2);
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamFIFOThresholdSel(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuFIFOThresholdSel_t copy_enuFTH)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if (copy_enuFTH > DMA_FIFO_IS_FULL)
    {
        ret_enuErrorStatus = DMA_INV_FIFO_THRESHOLD;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        DMAx->Streamx[copy_enuStream].SxFCR = (DMAx->Streamx[copy_enuStream].SxFCR & ~(0b11 << 0)) | (copy_enuFTH << 0);
    }

    return ret_enuErrorStatus;
}
SRV_enuErrorStatus_t MDMA_enuStreamGetFIFOStatus(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuFIFOStatus_t* ptr_enuFS)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else if (ptr_enuFS == NULL)
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else 
    {
        volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

        (*ptr_enuFS) = (DMAx->Streamx[copy_enuStream].SxFCR >> 3) & (0b111);
    }

    return ret_enuErrorStatus;
}

/* only DMA2 is able to perform mem-to-mem operations */
/* source: DMA_SxPAR, destination: DMA_SxM0AR */
SRV_enuErrorStatus_t MDMA_enuMemCpy(MDMA_enuStreams_t copy_enuStream, const uint8_t* ptr_uint32SrcAddress, uint8_t* ptr_uint32DestAddress, uint16_t copy_enuuint16DataSize)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    uint8_t local_uint8TransmitCompleteFlag = 0U;
    uint16_t local_uint16Timeout = DMA_TRANSMISSION_TIMEOUT;
    uint8_t local_uintIRQIter;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if ((ptr_uint32SrcAddress == NULL) || (ptr_uint32DestAddress == NULL))
    {
        ret_enuErrorStatus = NULL_PTR;
    } 
    else 
    {
        /* enable clk */
        MDMA_enuEnableClk(DMA2);
        
        /* disable stream */
        MDMA_enuStreamDisable(DMA2, copy_enuStream);

        /* set source address */
        MDMA_enuStreamSetPeripheralAddress(DMA2, copy_enuStream, ptr_uint32SrcAddress);

        /* set destination address */
        MDMA_enuStreamSetMem0Address(DMA2, copy_enuStream, ptr_uint32DestAddress);

        /* set data size */
        MDMA_enuStreamSetNumOfDataItemsToTransfer(DMA2, copy_enuStream, copy_enuuint16DataSize);

        /* select channel 0 */
        MDMA_voidStreamChannelSel(DMA2, copy_enuStream, DMA_CHANNEL0);

        /* destination increment mode (M0)*/
        MDMA_enuStreamMemIncMode(DMA2, copy_enuStream, MINC_ACCORDING_TO_MSIZE);

        /* source increment mode (PER) */
        MDMA_enuStreamPeripheralIncMode(DMA2, copy_enuStream, PINC_ACCORDING_TO_PSIZE);

        /* MSIZE = word (des) */
        MDMA_enuStreamMemIncMode(DMA2, copy_enuStream, MSIZE_WORD);

        /* PSIZE = word (src) */
        MDMA_enuStreamPeripheralDataSize(DMA2, copy_enuStream, P_SIZE_WORD);

        /* mem-to-mem mode */
        MDMA_enuStreamSetDataTransferDir(DMA2, copy_enuStream, MEM_TO_MEM);

        /* disable all of the stream's interrupts */
        for (local_uintIRQIter = DMA_HALF_TRANSFER_INT; local_uintIRQIter < NUM_OF_DMA_IRQs; local_uintIRQIter++)
            MDMA_enuStreamIRQDisable(DMA2, copy_enuStream, local_uintIRQIter);

        /* enable stream */
        MDMA_enuStreamEnable(DMA2, copy_enuStream);

        /* wait for the transfer to complete */
        while ((!local_uint8TransmitCompleteFlag) && (--local_uint16Timeout))
        {
            MDMA_voidGetTransferCompleteIRQFlag(DMA2, copy_enuStream, &local_uint8TransmitCompleteFlag);
        }
        /* if clk is supposed to be on && timeout reached 0 -> return error status */
        if ((local_uint16Timeout == 0) && (!local_uint8TransmitCompleteFlag))
        {
            ret_enuErrorStatus = DMA_MEMCPY_TIMEOUT;
        }
        else 
        {
            /* clear the transfer complete flag */
            MDMA_voidClrTransferCompleteIRQFlag(DMA2, copy_enuStream);

            /* stream is disabled automatically upon transmission completion */

            /* disable DMA2 clk */
            MDMA_enuDisableClk(DMA2);
        } 
    
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MDMA_enuRegisterCBF(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuDMAIRQs_t copy_enuIRQ, MDMA_ptrCBF ptr_CBF)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (copy_enuStream > DMA_STREAM7)
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else if (copy_enuIRQ > DMA_DIRECT_MODE_ERR)
    {
        ret_enuErrorStatus = DMA_INV_IRQ;
    }
    else if ((copy_enuDMAModule != DMA1) && (copy_enuDMAModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else 
    {
        MDMA_ptrCBFArr[copy_enuDMAModule][copy_enuStream][copy_enuIRQ] = ptr_CBF;
    }

    return ret_enuErrorStatus;
}


void MDMA_voidEveryIRQHandler(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream)
{
    uint8_t local_uint8IF;
    uint8_t local_uint8IRQSourceIter;

    /* checking for IRQ flags one by one to call their handler */
    for (local_uint8IRQSourceIter = DMA_HALF_TRANSFER_INT; local_uint8IRQSourceIter < NUM_OF_DMA_IRQs; local_uint8IRQSourceIter++)
    {
        /* check if IRQ source flag is raised */
        MDMA_enuGetIRQFlag(copy_enuDMAModule, copy_enuStream, local_uint8IRQSourceIter, &local_uint8IF);

        if (IS_DMA_IRQ_FLAG_RAISED(local_uint8IF))
        {
            /* clear IRQ flag */
            MDMA_enuClrIRQFlag(copy_enuDMAModule, copy_enuStream, local_uint8IRQSourceIter);

            /* check if its handler is not null then launch it */
            if (MDMA_ptrCBFArr[copy_enuDMAModule][copy_enuStream][local_uint8IRQSourceIter] != NULL)
            {
                MDMA_ptrCBFArr[copy_enuDMAModule][copy_enuStream][local_uint8IRQSourceIter]();
            }
            else {} /* do nothing */
        }
        else {} /* continue */
    }
    
    return;
}

static void MDMA_voidStreamChannelSel(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuChannel_t copy_enuChannel)
{
    volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

    DMAx->Streamx[copy_enuStream].SxCR |= (copy_enuChannel << 25);
}
static void MDMA_voidGetHalfTransferIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, uint8_t* ptr_uint8IF)
{
    volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

    switch (copy_enuStream)
    {
        case DMA_STREAM0:
            (*ptr_uint8IF) = (DMAx->LISR >> 4) & 1U;
        break;

        case DMA_STREAM1:
            (*ptr_uint8IF) = (DMAx->LISR >> 10) & 1U;
        break;

        case DMA_STREAM2:
            (*ptr_uint8IF) = (DMAx->LISR >> 20) & 1U;
        break;

        case DMA_STREAM3:
            (*ptr_uint8IF) = (DMAx->LISR >> 26) & 1U;
        break;

        case DMA_STREAM4:
            (*ptr_uint8IF) = (DMAx->HISR >> 4) & 1U;
        break;

        case DMA_STREAM5:
            (*ptr_uint8IF) = (DMAx->HISR >> 10) & 1U;
        break;

        case DMA_STREAM6:
            (*ptr_uint8IF) = (DMAx->HISR >> 20) & 1U;
        break;

        case DMA_STREAM7:
            (*ptr_uint8IF) = (DMAx->HISR >> 26) & 1U;
        break;
    }

    return;
}
static void MDMA_voidGetTransferCompleteIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, uint8_t* ptr_uint8IF)
{
    volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

    switch (copy_enuStream)
    {
        case DMA_STREAM0:
            (*ptr_uint8IF) = (DMAx->LISR >> 5) & 1U;
        break;

        case DMA_STREAM1:
            (*ptr_uint8IF) = (DMAx->LISR >> 11) & 1U;
        break;

        case DMA_STREAM2:
            (*ptr_uint8IF) = (DMAx->LISR >> 21) & 1U;
        break;

        case DMA_STREAM3:
            (*ptr_uint8IF) = (DMAx->LISR >> 27) & 1U;
        break;

        case DMA_STREAM4:
            (*ptr_uint8IF) = (DMAx->HISR >> 5) & 1U;
        break;

        case DMA_STREAM5:
            (*ptr_uint8IF) = (DMAx->HISR >> 11) & 1U;
        break;

        case DMA_STREAM6:
            (*ptr_uint8IF) = (DMAx->HISR >> 21) & 1U;
        break;

        case DMA_STREAM7:
            (*ptr_uint8IF) = (DMAx->HISR >> 27) & 1U;
        break;
    }

    return;
}
static void MDMA_voidGetTransferErrIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, uint8_t* ptr_uint8IF)
{
    volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

    switch (copy_enuStream)
    {
        case DMA_STREAM0:
            (*ptr_uint8IF) = (DMAx->LISR >> 3) & 1U;
        break;

        case DMA_STREAM1:
            (*ptr_uint8IF) = (DMAx->LISR >> 9) & 1U;
        break;

        case DMA_STREAM2:
            (*ptr_uint8IF) = (DMAx->LISR >> 19) & 1U;
        break;

        case DMA_STREAM3:
            (*ptr_uint8IF) = (DMAx->LISR >> 25) & 1U;
        break;

        case DMA_STREAM4:
            (*ptr_uint8IF) = (DMAx->HISR >> 3) & 1U;
        break;

        case DMA_STREAM5:
            (*ptr_uint8IF) = (DMAx->HISR >> 9) & 1U;
        break;

        case DMA_STREAM6:
            (*ptr_uint8IF) = (DMAx->HISR >> 19) & 1U;
        break;

        case DMA_STREAM7:
            (*ptr_uint8IF) = (DMAx->HISR >> 25) & 1U;
        break;
    }

    return;
}
static void MDMA_voidGetDirectModeErrIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, uint8_t* ptr_uint8IF)
{
    volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

    switch (copy_enuStream)
    {
        case DMA_STREAM0:
            (*ptr_uint8IF) = (DMAx->LISR >> 2) & 1U;
        break;

        case DMA_STREAM1:
            (*ptr_uint8IF) = (DMAx->LISR >> 8) & 1U;
        break;

        case DMA_STREAM2:
            (*ptr_uint8IF) = (DMAx->LISR >> 18) & 1U;
        break;

        case DMA_STREAM3:
            (*ptr_uint8IF) = (DMAx->LISR >> 24) & 1U;
        break;

        case DMA_STREAM4:
            (*ptr_uint8IF) = (DMAx->HISR >> 2) & 1U;
        break;

        case DMA_STREAM5:
            (*ptr_uint8IF) = (DMAx->HISR >> 8) & 1U;
        break;

        case DMA_STREAM6:
            (*ptr_uint8IF) = (DMAx->HISR >> 18) & 1U;
        break;

        case DMA_STREAM7:
            (*ptr_uint8IF) = (DMAx->HISR >> 24) & 1U;
        break;
    }

    return;
}
static void MDMA_voidGetFIFOErrIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, uint8_t* ptr_uint8IF)
{
    volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

    switch (copy_enuStream)
    {
        case DMA_STREAM0:
            (*ptr_uint8IF) = (DMAx->LISR >> 0) & 1U;
        break;

        case DMA_STREAM1:
            (*ptr_uint8IF) = (DMAx->LISR >> 6) & 1U;
        break;

        case DMA_STREAM2:
            (*ptr_uint8IF) = (DMAx->LISR >> 16) & 1U;
        break;

        case DMA_STREAM3:
            (*ptr_uint8IF) = (DMAx->LISR >> 22) & 1U;
        break;

        case DMA_STREAM4:
            (*ptr_uint8IF) = (DMAx->HISR >> 0) & 1U;
        break;

        case DMA_STREAM5:
            (*ptr_uint8IF) = (DMAx->HISR >> 6) & 1U;
        break;

        case DMA_STREAM6:
            (*ptr_uint8IF) = (DMAx->HISR >> 16) & 1U;
        break;

        case DMA_STREAM7:
            (*ptr_uint8IF) = (DMAx->HISR >> 22) & 1U;
        break;
    }

    return;
}
static void MDMA_voidClrHalfTransferIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream)
{
    volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

    switch (copy_enuStream)
    {
        case DMA_STREAM0:
            DMAx->LIFCR |= (1U << 4);
        break;

        case DMA_STREAM1:
            DMAx->LIFCR |= (1U << 10);
        break;

        case DMA_STREAM2:
            DMAx->LIFCR |= (1U << 20);
        break;

        case DMA_STREAM3:
            DMAx->LIFCR |= (1U << 26);
        break;

        case DMA_STREAM4:
            DMAx->HIFCR |= (1U << 4);
        break;

        case DMA_STREAM5:
            DMAx->HIFCR |= (1U << 10);
        break;

        case DMA_STREAM6:
            DMAx->HIFCR |= (1U << 20);
        break;

        case DMA_STREAM7:
            DMAx->HIFCR |= (1U << 26);
        break;
    }

    return;
}
static void MDMA_voidClrTransferCompleteIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream)
{
    volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

    switch (copy_enuStream)
    {
        case DMA_STREAM0:
            DMAx->LIFCR |= (1U << 5);
        break;

        case DMA_STREAM1:
            DMAx->LIFCR |= (1U << 11);
        break;

        case DMA_STREAM2:
            DMAx->LIFCR |= (1U << 21);
        break;

        case DMA_STREAM3:
            DMAx->LIFCR |= (1U << 27);
        break;

        case DMA_STREAM4:
            DMAx->HIFCR |= (1U << 5);
        break;

        case DMA_STREAM5:
            DMAx->HIFCR |= (1U << 11);
        break;

        case DMA_STREAM6:
            DMAx->HIFCR |= (1U << 21);
        break;

        case DMA_STREAM7:
            DMAx->HIFCR |= (1U << 27);
        break;
    }

    return;
}
static void MDMA_voidClrTransferErrIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream)
{
    volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

    switch (copy_enuStream)
    {
        case DMA_STREAM0:
            DMAx->LIFCR |= (1U << 3);
        break;

        case DMA_STREAM1:
            DMAx->LIFCR |= (1U << 9);
        break;

        case DMA_STREAM2:
            DMAx->LIFCR |= (1U << 19);
        break;

        case DMA_STREAM3:
            DMAx->LIFCR |= (1U << 25);
        break;

        case DMA_STREAM4:
            DMAx->HIFCR |= (1U << 3);
        break;

        case DMA_STREAM5:
            DMAx->HIFCR |= (1U << 9);
        break;

        case DMA_STREAM6:
            DMAx->HIFCR |= (1U << 19);
        break;

        case DMA_STREAM7:
            DMAx->HIFCR |= (1U << 25);
        break;
    }

    return;
}
static void MDMA_voidClrDirectModeErrIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream)
{
    volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

    switch (copy_enuStream)
    {
        case DMA_STREAM0:
            DMAx->LIFCR |= (1U << 2);
        break;

        case DMA_STREAM1:
            DMAx->LIFCR |= (1U << 8);
        break;

        case DMA_STREAM2:
            DMAx->LIFCR |= (1U << 18);
        break;

        case DMA_STREAM3:
            DMAx->LIFCR |= (1U << 24);
        break;

        case DMA_STREAM4:
            DMAx->HIFCR |= (1U << 2);
        break;

        case DMA_STREAM5:
            DMAx->HIFCR |= (1U << 8);
        break;

        case DMA_STREAM6:
            DMAx->HIFCR |= (1U << 18);
        break;

        case DMA_STREAM7:
            DMAx->HIFCR |= (1U << 24);
        break;
    }

    return;
}
static void MDMA_voidClrFIFOErrIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream)
{
    volatile MDMA_Registers_t* DMAx = (volatile MDMA_Registers_t*)DMA_GET_CONTROLLER_BASE_ADDRESS(copy_enuDMAModule);

    switch (copy_enuStream)
    {
        case DMA_STREAM0:
            DMAx->LIFCR |= (1U << 0);
        break;

        case DMA_STREAM1:
            DMAx->LIFCR |= (1U << 6);
        break;

        case DMA_STREAM2:
            DMAx->LIFCR |= (1U << 16);
        break;

        case DMA_STREAM3:
            DMAx->LIFCR |= (1U << 22);
        break;

        case DMA_STREAM4:
            DMAx->HIFCR |= (1U << 0);
        break;

        case DMA_STREAM5:
            DMAx->HIFCR |= (1U << 6);
        break;

        case DMA_STREAM6:
            DMAx->HIFCR |= (1U << 16);
        break;

        case DMA_STREAM7:
            DMAx->HIFCR |= (1U << 22);
        break;
    }

    return;
}

void DMA1_Stream0_IRQHandler(void)
{
    MDMA_voidEveryIRQHandler(DMA1, DMA_STREAM0);
}                          
void DMA1_Stream1_IRQHandler(void)
{
    MDMA_voidEveryIRQHandler(DMA1, DMA_STREAM1);
}                           
void DMA1_Stream2_IRQHandler(void)
{
    MDMA_voidEveryIRQHandler(DMA1, DMA_STREAM2);
}                           
void DMA1_Stream3_IRQHandler(void)
{
    MDMA_voidEveryIRQHandler(DMA1, DMA_STREAM3);
}                           
void DMA1_Stream4_IRQHandler(void)
{
    MDMA_voidEveryIRQHandler(DMA1, DMA_STREAM4);
}                           
void DMA1_Stream5_IRQHandler(void)
{
    MDMA_voidEveryIRQHandler(DMA1, DMA_STREAM5);
}                           
void DMA1_Stream6_IRQHandler(void)
{
    MDMA_voidEveryIRQHandler(DMA1, DMA_STREAM6);
}
void DMA1_Stream7_IRQHandler(void)
{
    MDMA_voidEveryIRQHandler(DMA1, DMA_STREAM7);
}

void DMA2_Stream0_IRQHandler(void)
{
    MDMA_voidEveryIRQHandler(DMA2, DMA_STREAM0);
}                          
void DMA2_Stream1_IRQHandler(void)
{
    MDMA_voidEveryIRQHandler(DMA2, DMA_STREAM1);
}                           
void DMA2_Stream2_IRQHandler(void)
{
    MDMA_voidEveryIRQHandler(DMA2, DMA_STREAM2);
}                           
void DMA2_Stream3_IRQHandler(void)
{
    MDMA_voidEveryIRQHandler(DMA2, DMA_STREAM3);
}                           
void DMA2_Stream4_IRQHandler(void)
{
    MDMA_voidEveryIRQHandler(DMA2, DMA_STREAM4);
}                           
void DMA2_Stream5_IRQHandler(void)
{
    MDMA_voidEveryIRQHandler(DMA2, DMA_STREAM5);
}                           
void DMA2_Stream6_IRQHandler(void)
{
    MDMA_voidEveryIRQHandler(DMA2, DMA_STREAM6);
}
void DMA2_Stream7_IRQHandler(void)
{
    MDMA_voidEveryIRQHandler(DMA2, DMA_STREAM7);
}
