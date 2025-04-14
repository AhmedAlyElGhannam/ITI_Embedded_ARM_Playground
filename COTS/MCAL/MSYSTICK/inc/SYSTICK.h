#ifndef SYSTICK_H
#define SYSTICK_H

typedef void (*MSYSTICK_CBF_t)(void);

typedef struct 
{
    MSYSTICK_CBF_t CBF;
    uint32_t cyclicTime;
} MSYSTICK_structTimerConfig_t;

extern void MSYSTICK_voidStart(void);
extern void MSYSTICK_voidStop(void);
// extern void Systick_Handler(void);
extern SRV_enuErrorStatus_t MSYSTICK_enuSetCyclicTime(MSYSTICK_structTimerConfig_t* Copy_structTimerConfig);
extern void MSYSTICK_voidInit(void);

#endif