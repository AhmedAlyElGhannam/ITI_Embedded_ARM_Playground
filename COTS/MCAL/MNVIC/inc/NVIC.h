#ifndef NVIC_H
#define NVIC_H

extern SRV_enuErrorStatus_t MNVIC_enuEnableIRQ(uint8_t Copy_uint8IntNumber);
extern SRV_enuErrorStatus_t MNVIC_enuDisableIRQ(uint8_t Copy_uint8IntNumber);

extern SRV_enuErrorStatus_t MNVIC_enuSetPendingIRQ(uint8_t Copy_uint8IntNumber);
extern SRV_enuErrorStatus_t MNVIC_enuClrPendingIRQ(uint8_t Copy_uint8IntNumber);
extern SRV_enuErrorStatus_t MNVIC_enuGetPendingIRQ(uint8_t Copy_uint8IntNumber, uint8_t* Ptr_uint8Pending);


extern SRV_enuErrorStatus_t MNVIC_enuSetIRQPriority(uint8_t Copy_uint8IntNumber, uint8_t Copy_uint8Priority);
extern SRV_enuErrorStatus_t MNVIC_enuGetIRQPriority(uint8_t Copy_uint8IntNumber, uint8_t* Ptr_uint8Priority);

extern SRV_enuErrorStatus_t MNVIC_enuIsIRQActive(uint8_t Copy_uint8IntNumber, uint8_t* Ptr_uint8Active);

extern SRV_enuErrorStatus_t MNVIC_enuSWTriggerIRQ(uint8_t Copy_uint8IntNumber);

extern void MNVIC_voidEnableAllIRQ(void);
extern void MNVIC_voidDisableAllIRQ(void);

#endif