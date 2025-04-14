#ifndef HUSART_H
#define HUSART_H

#include "std_types.h"
#include "std_lib.h"
#include "error_status.h"


/* HAL abstaction for USART with DMA */

extern SRV_enuErrorStatus_t HUSART_enuInit(void);
extern SRV_enuErrorStatus_t HUSART_enuSendBufferConfig(uint8_t* ptr_uint8Buffer, uint16_t copy_uint16BufferSize);
extern SRV_enuErrorStatus_t HUSART_enuReceiveBufferConfig(uint8_t* ptr_uint8Buffer, uint16_t copy_uint16BufferSize);
extern void HUSART_voidStartTransferOperations(void);
extern SRV_enuErrorStatus_t HUSART_enuDeInit(void);

#endif