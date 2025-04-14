#ifndef DMA_H
#define DMA_H

#include "error_status.h"
#include "std_types.h"

typedef enum 
{
    DMA1 = 0U,
    DMA2 = 1U,
    NUM_OF_DMA_MODULES,
} MDMA_enuDMAController_t;

/* configured for each stream (only one can work at a time) */
typedef enum 
{
    DMA_CHANNEL0 = 0b000,
    DMA_CHANNEL1 = 0b001,
    DMA_CHANNEL2 = 0b010,
    DMA_CHANNEL3 = 0b011,
    DMA_CHANNEL4 = 0b100,
    DMA_CHANNEL5 = 0b101,
    DMA_CHANNEL6 = 0b110,
    DMA_CHANNEL7 = 0b111,
} MDMA_enuChannel_t;

typedef enum 
{
    DMA_STREAM0 = 0U,
    DMA_STREAM1,
    DMA_STREAM2,
    DMA_STREAM3,
    DMA_STREAM4,
    DMA_STREAM5,
    DMA_STREAM6,
    DMA_STREAM7,
    NUM_OF_DMA_STREAMS,
} MDMA_enuStreams_t;

typedef enum 
{
    DMA1_STREAM0_CHANNEL0_SPI3_RX,
    DMA1_STREAM0_CHANNEL1_I2C1_RX,
    DMA1_STREAM0_CHANNEL2_TIM4_CH1,
    DMA1_STREAM0_CHANNEL3_I2S3_EXT_RX,
    DMA1_STREAM0_CHANNEL6_TIM5_CH3_AND_TIM5_UP,
    DMA1_STREAM1_CHANNEL1_I2C3_RX,
    DMA1_STREAM1_CHANNEL3_TIM2_CH3_AND_TIM2_UP,
    DMA1_STREAM1_CHANNEL6_TIM5_CH4_AND_TIM5_TRIG,
    DMA1_STREAM2_CHANNEL0_SPI3_RX,
    DMA1_STREAM2_CHANNEL2_I2S3_EXT_RX,
    DMA1_STREAM2_CHANNEL3_I2C3_RX,
    DMA1_STREAM2_CHANNEL5_TIM3_CH4_AND_TIM3_UP,
    DMA1_STREAM2_CHANNEL6_TIM5_CH1,
    DMA1_STREAM2_CHANNEL7_I2C2_RX,
    DMA1_STREAM3_CHANNEL0_SPI2_RX,
    DMA1_STREAM3_CHANNEL2_TIM4_CH2,
    DMA1_STREAM3_CHANNEL3_I2S2_EXT_RX,
    DMA1_STREAM3_CHANNEL6_TIM5_CH4_AND_TIM5_TRIG,
    DMA1_STREAM3_CHANNEL7_I2C2_RX,
    DMA1_STREAM4_CHANNEL0_SPI2_TX,
    DMA1_STREAM4_CHANNEL2_I2S2_EXT_TX,
    DMA1_STREAM4_CHANNEL3_I2C3_TX,
    DMA1_STREAM4_CHANNEL5_TIM3_CH1_AND_TIM3_TRIG,
    DMA1_STREAM4_CHANNEL6_TIM5_CH2,
    DMA1_STREAM5_CHANNEL0_SPI3_TX,
    DMA1_STREAM5_CHANNEL1_I2C1_RX,
    DMA1_STREAM5_CHANNEL2_I2S3_EXT_TX,
    DMA1_STREAM5_CHANNEL3_TIM2_CH1,
    DMA1_STREAM5_CHANNEL4_USART2_RX,
    DMA1_STREAM5_CHANNEL5_TIM3_CH2,
    DMA1_STREAM5_CHANNEL6_I2C3_TX,
    DMA1_STREAM6_CHANNEL1_I2C1_TX,
    DMA1_STREAM6_CHANNEL2_IM4_UP,
    DMA1_STREAM6_CHANNEL3_TIM2_CH2_AND_TIM2_CH4,
    DMA1_STREAM6_CHANNEL4_USART2_TX,
    DMA1_STREAM6_CHANNEL6_TIM5_UP,
    DMA1_STREAM7_CHANNEL0_SPI3_TX,
    DMA1_STREAM7_CHANNEL1_I2C1_TX,
    DMA1_STREAM7_CHANNEL2_TIM4_CH3,
    DMA1_STREAM7_CHANNEL3_TIM2_UP_AND_TIM2_CH4,
    DMA1_STREAM7_CHANNEL5_TIM3_CH3,
    DMA1_STREAM7_CHANNEL7_I2C_TX,
    DMA2_STREAM0_CHANNEL0_ADC1,
    DMA2_STREAM0_CHANNEL3_SPI1_RX,
    DMA2_STREAM0_CHANNEL4_SPI4_RX,
    DMA2_STREAM0_CHANNEL6_TIM1_TRIG,
    DMA2_STREAM1_CHANNEL4_SPI4_TX,
    DMA2_STREAM1_CHANNEL5_USART6_RX,
    DMA2_STREAM1_CHANNEL6_TIM1_CH1,
    DMA2_STREAM2_CHANNEL3_SPI1_RX,
    DMA2_STREAM2_CHANNEL4_USART1_RX,
    DMA2_STREAM2_CHANNEL5_USART6_RX,
    DMA2_STREAM2_CHANNEL6_TIM1_CH2,
    DMA2_STREAM3_CHANNEL3_SPI1_TX,
    DMA2_STREAM3_CHANNEL4_SDIO,
    DMA2_STREAM3_CHANNEL5_SPI4_RX,
    DMA2_STREAM3_CHANNEL6_TIM1_CH1,
    DMA2_STREAM4_CHANNEL0_ADC1,
    DMA2_STREAM4_CHANNEL5_SPI4_TX,
    DMA2_STREAM4_CHANNEL6_TIM1_CH4_AND_TIM1_TRIG_AND_TIM1_COM,
    DMA2_STREAM5_CHANNEL3_SPI1_TX,
    DMA2_STREAM5_CHANNEL4_USART1_RX,
    DMA2_STREAM5_CHANNEL6_TIM1_UP,
    DMA2_STREAM6_CHANNEL0_TIM1_CH1_AND_TIM1_CH2_AND_TIM1_CH3,
    DMA2_STREAM6_CHANNEL4_SDIO,
    DMA2_STREAM6_CHANNEL5_USART6_TX,
    DMA2_STREAM6_CHANNEL6_TIM1_CH3,
    DMA2_STREAM7_CHANNEL4_USART1_TX,
    DMA2_STREAM7_CHANNEL5_USART6_TX,
} MDMA_enuDMAPeripheralRequest_t;


/**
 * written only when EN = 0
 * forced to 0 in direct mode when EN = 1
 */
typedef enum 
{
    DMA_SINGLE_TRANSFER = 0b00,
    DMA_INCR4 = 0b01,
    DMA_INCR8 = 0b10,
    DMA_INCR16 = 0b11,
} MDMA_enuTransferConfig_t;


typedef enum 
{
    TARGET_MEM_MEM0 = 0U,
    TARGET_MEM_MEM1 = !TARGET_MEM_MEM0,
} MDMA_enuDoubleBufCurrTarget_t;

typedef enum 
{
    DBM_ENABLE = 1U,
    DBM_DISABLE = !DBM_ENABLE,
} MDMA_enuDoubleBufMode_t;

typedef enum 
{
    DMA_LOW = 0b00,
    DMA_MEDIUM = 0b01,
    DMA_HIGH = 0b10,
    DMA_VHIGH = 0b11,
} MDMA_enuPriorityLevel_t;

typedef enum 
{
    LINKED_TO_PSIZE = 0U,
    FIXED_TO_4 = !LINKED_TO_PSIZE,
} MDMA_enuPeripheralIncOffsetSize_t;

typedef enum 
{
    MSIZE_BYTE = 0b00,
    MSIZE_HWORD = 0b01,
    MSIZE_WORD = 0b10,
} MDMA_enuMemoryDataSize_t;

typedef enum 
{
    P_SIZE_BYTE = 0b00,
    P_SIZE_HWORD = 0b01,
    P_SIZE_WORD = 0b10,
} MDMA_enuPeripheralDataSize_t;

typedef enum 
{
    MINC_FIXED = 0U,
    MINC_ACCORDING_TO_MSIZE = !MINC_FIXED,
} MDMA_enuMemIncMode_t;

typedef enum 
{
    PINC_FIXED = 0U,
    PINC_ACCORDING_TO_PSIZE = !PINC_FIXED,
} MDMA_enuPeripheralIncMode_t;

typedef enum 
{
    CIRC_ENABLE = 1U,
    CIRC_DISABLE = !CIRC_ENABLE,
} MDMA_enuCircularModeConfig_t;

typedef enum 
{
    PERIPHERAL_TO_MEM = 0b00,
    MEM_TO_PERIPHERAL = 0b01,
    MEM_TO_MEM = 0b10,
} MDMA_enuDataTransferMode_t;

typedef enum 
{
    FLOW_CONTROLLER_IS_DMA = 0U,
    FLOW_CONTROLLER_IS_PER = !FLOW_CONTROLLER_IS_DMA,
} MDMA_enuPeripheralFlowController_t;

typedef enum 
{
    DMA_INT_ENABLE = 1U,
    DMA_INT_DISABLE = !DMA_INT_ENABLE
} MDMA_enuInterruptConfig_t;

typedef enum 
{
    DMA_STREAM_ENABLE = 1U,
    DMA_STREAM_DISABLE = !DMA_STREAM_ENABLE,
} MDMA_enuStreamConfig_t;

typedef enum 
{
    DMA_FIFO_LVL_LT_1_DIV_4 = 0b000,
    DMA_FIFO_LVL_LT_1_DIV_2 = 0b001,
    DMA_FIFO_LVL_LT_3_DIV_4 = 0b010,
    DMA_FIFO_LVL_LT_FULL = 0b011,
    DMA_FIFO_EMPTY = 0b100,
    DMA_FIFO_FULL = 0b101,
} MDMA_enuFIFOStatus_t;

typedef enum 
{
    DMDIS_ENABLE = 0U,
    DMDIS_DISABLE = !DMDIS_ENABLE,
} MDMA_enuDirectModeConfig_t;

typedef enum 
{
    DMA_FIFO_1_DIV_4_FULL = 0b00,
    DMA_FIFO_1_DIV_2_FULL = 0b01,
    DMA_FIFO_3_DIV_4_FULL = 0b10,
    DMA_FIFO_IS_FULL = 0b11,
} MDMA_enuFIFOThresholdSel_t;

typedef enum 
{
    DMA_HALF_TRANSFER_INT,
    DMA_TRANSFER_COMPLETE_INT,
    DMA_TRANSFER_ERR_INT,
    DMA_FIFO_OVERRUN_UNDERRUN,
    DMA_DIRECT_MODE_ERR,
    NUM_OF_DMA_IRQs
} MDMA_enuDMAIRQs_t;


typedef void (*MDMA_ptrCBF) (void);




extern SRV_enuErrorStatus_t MDMA_enuStreamEnable(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream);
extern SRV_enuErrorStatus_t MDMA_enuStreamDisable(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream);

extern SRV_enuErrorStatus_t MDMA_enuGetIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuDMAIRQs_t copy_enuIRQ, uint8_t* ptr_uint8IF);
extern SRV_enuErrorStatus_t MDMA_enuClrIRQFlag(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuDMAIRQs_t copy_enuIRQ);

extern SRV_enuErrorStatus_t MDMA_enuEnableClk(MDMA_enuDMAController_t copy_enuDMAModule);
extern SRV_enuErrorStatus_t MDMA_enuDisableClk(MDMA_enuDMAController_t copy_enuDMAModule);

extern SRV_enuErrorStatus_t MDMA_enuStreamIRQEnable(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuDMAIRQs_t copy_enuIRQ);
extern SRV_enuErrorStatus_t MDMA_enuStreamIRQDisable(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuDMAIRQs_t copy_enuIRQ);


extern SRV_enuErrorStatus_t MDMA_enuMemCpy(MDMA_enuStreams_t copy_enuStream, const uint8_t* ptr_uint32SrcAddress, uint8_t* ptr_uint32DestAddress, uint16_t copy_enuuint16DataSize);


extern SRV_enuErrorStatus_t MDMA_enuRegisterCBF(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuDMAIRQs_t copy_enuIRQ, MDMA_ptrCBF ptr_CBF);


extern SRV_enuErrorStatus_t MDMA_enuStreamFIFOThresholdSel(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuFIFOThresholdSel_t copy_enuFTH);
extern SRV_enuErrorStatus_t MDMA_enuStreamGetFIFOStatus(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuFIFOStatus_t* ptr_enuFS);


extern SRV_enuErrorStatus_t MDMA_enuStreamSetPeripheralAddress(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, uint8_t* ptr_uint32PeripheralRegAddress);
extern SRV_enuErrorStatus_t MDMA_enuStreamSetMem0Address(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, uint8_t* ptr_uint32Mem0Address);
extern SRV_enuErrorStatus_t MDMA_enuStreamSetMem1Address(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, uint8_t* ptr_uint32Mem1Address);

extern SRV_enuErrorStatus_t MDMA_enuStreamSetNumOfDataItemsToTransfer(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, uint16_t copy_uint16NumOfDataItems);

extern SRV_enuErrorStatus_t MDMA_enuStreamMemIncMode(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuMemIncMode_t copy_enuMINC);
extern SRV_enuErrorStatus_t MDMA_enuStreamPeripheralIncMode(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuPeripheralIncMode_t copy_enuPINC);

extern SRV_enuErrorStatus_t MDMA_enuStreamPeripheralDataSize(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuPeripheralDataSize_t copy_enuPSIZE);
extern SRV_enuErrorStatus_t MDMA_enuStreamMemDataSize(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuMemoryDataSize_t copy_enuMSIZE);

extern SRV_enuErrorStatus_t MDMA_enuStreamMemoryBurstTransferConfig(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuTransferConfig_t copy_enuMBURST);
extern SRV_enuErrorStatus_t MDMA_enuStreamPeripheralBurstTransferConfig(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuTransferConfig_t copy_enuPBURST);

extern SRV_enuErrorStatus_t MDMA_enuStreamSetChannelPeripheralRequest(MDMA_enuDMAPeripheralRequest_t copy_enuRequest);

extern SRV_enuErrorStatus_t MDMA_enuStreamSetPriorityLvl(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuPriorityLevel_t copy_enuPriority);

extern SRV_enuErrorStatus_t MDMA_enuStreamDirectModeConfig(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuDirectModeConfig_t copy_enuDMDIS);

extern SRV_enuErrorStatus_t MDMA_enuStreamSetDataTransferDir(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuDataTransferMode_t copy_enuDIR);

extern SRV_enuErrorStatus_t MDMA_enuStreamPeripheralFlowControllerConfig(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuPeripheralFlowController_t copy_enuPFCTRL);

extern SRV_enuErrorStatus_t MDMA_enuStreamPeripheralIncOffsetSize(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuPeripheralIncOffsetSize_t copy_enuPINCOS);

extern SRV_enuErrorStatus_t MDMA_enuStreamCircularMode(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuCircularModeConfig_t copy_enuCIRC);

extern SRV_enuErrorStatus_t MDMA_enuStreamSetCurrMemTargetDBM(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuDoubleBufCurrTarget_t copy_enuTarget);

extern SRV_enuErrorStatus_t MDMA_enuStreamDoubleBufferModeConfig(MDMA_enuDMAController_t copy_enuDMAModule, MDMA_enuStreams_t copy_enuStream, MDMA_enuDoubleBufMode_t copy_enuDBM);



#endif