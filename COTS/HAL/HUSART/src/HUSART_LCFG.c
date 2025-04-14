#include "HUSART_LCFG.h"
#include "HUSART.h"

HUSART_structUsedModulesAndPortsAndPins_t HUSART_structHUSARTConfig = 
{
    .DMAxModule = DMA2,
    .USARTxModule = USART1,
    .TxPort = GPIOA,
    .RxPort = GPIOA,
    .TxPin = PIN00,
    .RxPin = PIN01,
    .TxStream = DMA_STREAM0,
    .RxStream = DMA_STREAM1
};