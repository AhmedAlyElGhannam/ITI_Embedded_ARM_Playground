#ifndef SCHEDULER_H
#define SCHEDULER_H


typedef void (*HSCHEDULER_CBF_t)(void);

typedef struct 
{
    uint32_t priority;
    uint32_t cyclicTime;
    HSCHEDULER_CBF_t CBF;
    uint32_t firstDelay;
} HSCHEDULER_structRunnableConfig_t;

extern void HSCHEDULER_voidInit(void);
extern void HSCHEDULER_voidStart(void);
extern void HSCHEDULER_voidStop(void);
extern SRV_enuErrorStatus_t HSCHEDULER_enuRegisterRunnable(HSCHEDULER_structRunnableConfig_t* Copy_structRunnableConfig);

#endif