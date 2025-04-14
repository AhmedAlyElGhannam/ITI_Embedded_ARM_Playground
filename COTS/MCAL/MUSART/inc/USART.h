#ifndef USART_H
#define USART_H

#include "std_types.h"
#include "error_status.h"

typedef enum 
{
    USART2 = 0x0U,
    USART1 = 0xC800U,
    USART6 = 0xCC00U,
} MUSART_enuModule_t;

typedef enum 
{
    MUSART_NORMAL_MODE,
    MUSART_HW_FLOW_CONTROL_MODE,
    MUSART_DMA_MODE, // multibuf mode
    MUSART_SYNC_MODE,
    MUSART_SMARTCARD_MODE,
    MUSART_SINGLE_WIRE_MODE, // half-duplex
    MUSART_IrDA_MODE,
    MUSART_LIN_MODE,
} MUSART_enuUSARTModes_t;

typedef enum 
{
    ONE_START_8_DATA_nSTOP = 0U,
    ONE_START_9_DATA_nSTOP = !ONE_START_8_DATA_nSTOP,
} MUSART_enuWordLen_t;

typedef enum 
{
    OVERSAMPLE_BY_16 = 0U,
    OVERSAMPLE_BY_8 = !OVERSAMPLE_BY_16,
} MUSART_enuOverSampleMode_t;

typedef enum 
{
    IDLE_LINE = 0U,
    ADDRESS_MARK = !IDLE_LINE,
} MUSART_enuWakeUpMethod_t;

typedef enum 
{
    PARITY_CONTROL_DIS = 0U,
    PARITY_CONTROL_EN = !PARITY_CONTROL_DIS,
} MUSART_enuParityControl_t;

typedef enum 
{
    EVEN_PARITY = 0U,
    ODD_PARITY = !EVEN_PARITY,
    PARITY_NONE,
} MUSART_enuParitySelection_t;

typedef enum 
{
    TX_EN = 1U,
    TX_DIS = !TX_EN,
} MUSART_enuTxControl_t;

typedef enum 
{
    RX_EN = 1U,
    RX_DIS = !RX_EN,
} MUSART_enuRxControl_t;

typedef enum 
{
    RX_ACTIVE_MODE = 0U,
    RX_MUTE_MODE = !RX_ACTIVE_MODE,
} MUSART_enuRxWakeUpMode_t;

typedef enum 
{
    NO_BREAK_CHAR_TRANSMITTED = 0U,
    BREAK_CHAR_WILL_BE_TRANSMITTED = !NO_BREAK_CHAR_TRANSMITTED,
} MUSART_enuSendBreakControl_t;

typedef enum 
{
    ONE_STOP_BIT = 0b00,
    HALF_STOP_BIT = 0b01,
    TWO_STOP_BIT = 0b10,
    ONE_HALF_STOP_BIT = 0b11,
} MUSART_enuStopBitConfig_t;

typedef enum 
{
    STEADY_LOW = 0U,
    STEADY_HIGH = 1U,
} MUSART_enuClkPolarityConfig_t;

typedef enum 
{
    FIRST_CLK_TRANSITION = 0U,
    SECOND_CLK_TRANSITION = !FIRST_CLK_TRANSITION,
} MUSART_enuClkPhaseConfig_t;

typedef enum 
{
    LBCL_NOUT = 0U,
    LBCL_OUT = !LBCL_NOUT,
} MUSART_enuLastBitClkPulse_t;

typedef enum 
{
    BREAK_DET_10 = 0U,
    BREAK_DET_11 = !BREAK_DET_10,
} MUSART_enuLINBreakDetectionLenConfig_t;

typedef enum 
{
    THREE_SAMPLE_BIT_METHOD = 0U,
    ONE_SAMPLE_BIT_METHOD = 1U,
} MUSART_enuSampleBitMethodConfig_t;

typedef enum 
{
    IRQ_TRANSMIT_DATA_REG_EMPTY = 0,
    IRQ_CTS_FLAG,
    IRQ_TRANSMISSION_COMPLETE,
    IRQ_RECEIVE_DATA_RDY_TO_BE_READ,
    IRQ_OVERRUN_ERR_DETECTED,
    IRQ_IDLE_LINE_DETECTED,
    IRQ_PARITY_ERR,
    IRQ_BREAK_FLAG,
    IRQ_NOISE_FLAG,
    NUM_OF_USART_IRQs
} MUSART_enuIRQSources_t;

typedef enum
{
    USART_IRQ_EN = 1U,
    USART_IRQ_DIS = !USART_IRQ_EN,
} MUSART_enuIRQConfig_t;

typedef enum 
{
    USART_ON = 1U,
    USART_OFF = !USART_ON,
} MUSART_enuUSARTModuleConfig_t;

typedef enum 
{
    USART_MODE_ON = 1U,
    USART_MODE_OFF = !USART_MODE_ON,
} MUSART_enuUSARTModeConfig_t;

typedef enum 
{
    BAUD_RATE_1200 = 1200UL,
    BAUD_RATE_2400 = 2400UL,
    BAUD_RATE_4800 = 4800UL,
    BAUD_RATE_9600 = 9600UL,
    BAUD_RATE_19200 = 19200UL,
    BAUD_RATE_38400 = 38400UL,
    BAUD_RATE_57600 = 57600UL,
    BAUD_RATE_115200 = 115200UL,
    BAUD_RATE_230400 = 230400UL,
    BAUD_RATE_460800 = 460800UL,
    BAUD_RATE_921600 = 921600UL,
    BAUD_RATE_1792000 = 1792000UL,
    BAUD_RATE_1843200 = 1843200UL,
    BAUD_RATE_3584000 = 3584000UL,
    BAUD_RATE_3686400 = 3686400UL,
} MUSART_enuDefinedBaudRates_t;

typedef void (*MUSART_ptrCBF) (void);


typedef struct 
{
    MUSART_enuParitySelection_t paritySel;
    MUSART_enuWordLen_t wordLen;
    MUSART_enuStopBitConfig_t stopBit;
    MUSART_enuParityControl_t parityCon;
} MUSART_structFrameSettings_t;

// typedef struct 
// {
//     MUSART_enuClkPolarityConfig_t;
//     MUSART_enuClkPhaseConfig_t;
//     MUSART_enuLastBitClkPulse_t;
// } MUSART_structSyncModeConfig_t;

// typedef struct 
// {
//     MUSART_enuSendBreakControl_t;
//     MUSART_enuLINBreakDetectionLenConfig_t;
// } MUSART_structLINModeConfig_t;

// /* keep empty for now */
// typedef struct 
// {
//     // page 542
// } MUSART_structDMAModeConfig_t;


// typedef struct
// {
//     MUSART_enuWakeUpMethod_t wakeUpMethod;
//     MUSART_enuRxWakeUpMode_t wakeUpMethodRx;
// } MUSART_structMultiProcessorCommConfig_t;

/* as suggested by kishk */
typedef struct 
{
    uint8_t* buff; 
    uint16_t len; 
    uint16_t index;
} MUSART_structDataBuffer_t;

/* will probably ditch the obj struct */
typedef struct 
{
    MUSART_enuModule_t module;
    // baudrate
    // wordlen
    // mode of operation (Async/DMA/LIN/Smartcard...)
    // 
} MUSART_structUSARTObject_t;


/* multiprocessor communication */
// extern SRV_enuErrorStatus_t MUSART_enuSetUSARTNodeAddress(MUSART_enuModule_t, uint8_t copy_uint8Address);
// extern SRV_enuErrorStatus_t MUSART_enuSetWakeUpMethod(MUSART_enuModule_t, MUSART_enuWakeUpMethod_t);
// extern SRV_enuErrorStatus_t MUSART_enuConfigRxWakeUp(MUSART_enuModule_t, MUSART_enuRxWakeUpMode_t);

/* sync mode configs */
// extern SRV_enuErrorStatus_t MUSART_enuClkPolarityConfig(MUSART_enuModule_t, MUSART_enuClkPolarityConfig_t);
// extern SRV_enuErrorStatus_t MUSART_enuClkPhaseConfig(MUSART_enuModule_t, MUSART_enuClkPhaseConfig_t);
// extern SRV_enuErrorStatus_t MUSART_enuLastBitClkPulse(MUSART_enuModule_t, MUSART_enuLastBitClkPulse_t);

/* LIN mode configs */
// extern SRV_enuErrorStatus_t MUSART_enuConfigSendBreak(MUSART_enuModule_t, MUSART_enuSendBreakControl_t);
// extern SRV_enuErrorStatus_t MUSART_enuLINBreakDetectionLen(MUSART_enuModule_t, MUSART_enuLINBreakDetectionLenConfig_t);





extern SRV_enuErrorStatus_t MUSART_enuConfigOverSampleMode(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuOverSampleMode_t copy_enuOverSampleMode);
extern SRV_enuErrorStatus_t MUSART_enuSetBaudRate(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuDefinedBaudRates_t copy_enuBaudRate);
extern SRV_enuErrorStatus_t MUSART_enuToggleUSARTOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModuleConfig_t copy_enuModuleState);
extern SRV_enuErrorStatus_t MUSART_enuToggleTxOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuTxControl_t copy_enuTxState);
extern SRV_enuErrorStatus_t MUSART_enuToggleRxOnOff(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuRxControl_t copy_enuRxState);
extern SRV_enuErrorStatus_t MUSART_enuEnableUSARTMode(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModes_t copy_enuModes);
extern SRV_enuErrorStatus_t MUSART_enuDisableUSARTMode(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuUSARTModes_t copy_enuModes);
extern SRV_enuErrorStatus_t MUSART_enuEnableUSARTInt(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuIRQSources_t copy_enuIRQSource);
extern SRV_enuErrorStatus_t MUSART_enuDisableUSARTInt(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuIRQSources_t copy_enuIRQSource);
extern SRV_enuErrorStatus_t MUSART_enuInitUSARTClk(MUSART_enuModule_t copy_enuUSARTModule);
extern SRV_enuErrorStatus_t MUSART_enuConfigUSARTFrameSettings(MUSART_enuModule_t copy_enuUSARTModule, MUSART_structFrameSettings_t* ptr_structFrameSettings);
extern SRV_enuErrorStatus_t MUSART_enuSetWordLen(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuWordLen_t copy_enuWordLen);
extern SRV_enuErrorStatus_t MUSART_enuSetParityControl(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuParityControl_t copy_enuParityControl);
extern SRV_enuErrorStatus_t MUSART_enuConfigStopBits(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuStopBitConfig_t copy_enuStopBitConfig);
extern SRV_enuErrorStatus_t MUSART_enuSetParitySel(MUSART_enuModule_t copy_enuUSARTModule, MUSART_enuParitySelection_t copy_enuParitySel);
extern SRV_enuErrorStatus_t MUSART_enuSendBytePolling(MUSART_enuModule_t copy_enuUSARTModule, uint8_t copy_uint8Data);
extern SRV_enuErrorStatus_t MUSART_enuReceiveBytePolling(MUSART_enuModule_t copy_enuUSARTModule, uint8_t* ptr_uint8Data);
extern SRV_enuErrorStatus_t MUSART_enuRegisterCBF(MUSART_enuModule_t copy_enuUSARTModule, MUSART_ptrCBF ptr_CBF, MUSART_enuIRQSources_t copy_enuIRQSource);

// send buffer
// receive buffer



#endif
