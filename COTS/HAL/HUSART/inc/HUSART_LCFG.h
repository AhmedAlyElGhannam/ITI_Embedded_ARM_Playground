#ifndef HUSART_LCFG_H
#define HUSART_LCFG_H

#include "std_types.h"
#include "error_status.h"
#include "DMA.h"
#include "USART.h"
#include "GPIO.h"

typedef struct 
{
    MDMA_enuDMAController_t DMAxModule;
    MUSART_enuModule_t USARTxModule;
    MGPIO_enuPortDesignator_t TxPort;
    MGPIO_enuPortDesignator_t RxPort;
    MGPIO_enuPinDesignator_t TxPin;
    MGPIO_enuPinDesignator_t RxPin;
    MDMA_enuStreams_t TxStream;
    MDMA_enuStreams_t RxStream;
} HUSART_structUsedModulesAndPortsAndPins_t;

#endif