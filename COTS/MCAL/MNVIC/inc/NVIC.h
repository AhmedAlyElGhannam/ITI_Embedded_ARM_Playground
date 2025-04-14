#ifndef NVIC_H
#define NVIC_H

#include "std_types.h"
#include "error_status.h"


typedef enum 
{
	WWDG = 0, /*> Window Watchdog Interrupt*/
    EXTI16__PVD = 1,
    EXTI21__TAMP_STAMP = 2,
    EXTI22__RTC_WKUP = 3,
    FLASH = 4,
    RCC = 5,
    EXTI0 = 6,
    EXTI1 = 7,
    EXTI2 = 8,
    EXTI3 = 9,
    EXTI4 = 10,
    DMA1_STREAM0 = 11,
    DMA1_STREAM1 = 12,
    DMA1_STREAM2 = 13,
    DMA1_STREAM3 = 14,
    DMA1_STREAM4 = 15,
    DMA1_STREAM5 = 16,
    DMA1_STREAM6 = 17,
    ADC = 18,
    EXTI9__5 = 23,
    TIM1_BRK__TIM9 = 24,
    TIM1_UP_TIM10 = 25,
    TIM1_TRG_COM__TIM11 = 26,
    TIM1_CC = 27,
    TIM2 = 28,
    TIM3 = 29,
    TIM4 = 30,
    I2C1_EV = 31,
    I2C1_ER = 32,
    I2C2_EV = 33,
    I2C2_ER = 34,
    SPI1 = 35,
    SPI2 = 36,
    USART1 = 37,
    USART2 = 38,
    EXTI15__10 = 40,
    EXTI17__RTC_ALARM = 41,
    EXTI18__OTG_FS_WKUP = 42,
    DMA1_STREAM7 = 47,
    SDIO = 49,
    TIM5 = 50,
    SPI3 = 51,
    DMA2_STREAM0 = 56,
    DMA2_STREAM1 = 57,
    DMA2_STREAM2 = 58,
    DMA2_STREAM3 = 59,
    DMA2_STREAM4 = 60,
    OTG_FS = 67,
    DMA2_STREAM5 = 68,
    DMA2_STREAM6 = 69,
    DMA2_STREAM7 = 70,
    USART6 = 71,
    I2C3_EV = 72,
    I2C3_ER = 73,
    FPU = 81,
    SPI4 = 84,
} MNVIC_enuVectorTable_t;


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
