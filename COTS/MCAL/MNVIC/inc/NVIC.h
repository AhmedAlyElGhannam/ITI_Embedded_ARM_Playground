#ifndef NVIC_H
#define NVIC_H

SRV_enuErrorStatus_t MNVIC_enuEnableIRQ(uint8_t Copy_uint8IntNumber);
SRV_enuErrorStatus_t MNVIC_enuDisableIRQ(uint8_t Copy_uint8IntNumber);

SRV_enuErrorStatus_t MNVIC_enuSetPendingIRQ(uint8_t Copy_uint8IntNumber);
SRV_enuErrorStatus_t MNVIC_enuClrPendingIRQ(uint8_t Copy_uint8IntNumber);
SRV_enuErrorStatus_t MNVIC_enuGetPendingIRQ(uint8_t Copy_uint8IntNumber, uint8_t* Ptr_uint8Pending);


SRV_enuErrorStatus_t MNVIC_enuSetIRQPriority(uint8_t Copy_uint8IntNumber, uint8_t Copy_uint8Priority);
SRV_enuErrorStatus_t MNVIC_enuGetIRQPriority(uint8_t Copy_uint8IntNumber, uint8_t* Ptr_uint8Priority);

SRV_enuErrorStatus_t MNVIC_enuIsIRQActive(uint8_t Copy_uint8IntNumber, uint8_t* Ptr_uint8Active);

SRV_enuErrorStatus_t MNVIC_enuSWTriggerIRQ(uint8_t Copy_uint8IntNumber);


#endif