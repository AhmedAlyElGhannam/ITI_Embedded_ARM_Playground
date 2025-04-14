#include "std_types.h"
#include "error_status.h"
#include "SYSTICK.h"
#include "scheduler_PBCFG.h"
#include "scheduler.h"

static void HSCHEDULER_enuCyclicRunRunnable(void);
static HSCHEDULER_structRunnableConfig_t* Global_structRunnableConfigArr[SCHEDULER_MAX_TASKS] = {NULL};

void HSCHEDULER_voidInit(void)
{
    static MSYSTICK_structTimerConfig_t Local_structTimerConfig = 
    {
        .CBF = (MSYSTICK_CBF_t)HSCHEDULER_enuCyclicRunRunnable,
        .cyclicTime = 1
    };

    MSYSTICK_voidInit();


    MSYSTICK_enuSetCyclicTime(&Local_structTimerConfig);
}

void HSCHEDULER_voidStart(void)
{
    MSYSTICK_voidStart();
}

void HSCHEDULER_voidStop(void)
{
    MSYSTICK_voidStop();
}

SRV_enuErrorStatus_t HSCHEDULER_enuRegisterRunnable(HSCHEDULER_structRunnableConfig_t* Copy_structRunnableConfig)
{
    SRV_enuErrorStatus_t Local_enuErrorStatus = ALL_OK;
    
    if (Copy_structRunnableConfig == NULL)
    {
        Local_enuErrorStatus = NULL_PTR;
    }
    else if (Copy_structRunnableConfig->CBF == NULL)
    {
        Local_enuErrorStatus = NULL_PTR;
    }
    else 
    {
        if (Global_structRunnableConfigArr[Copy_structRunnableConfig->priority] == NULL)
        {
            Global_structRunnableConfigArr[Copy_structRunnableConfig->priority] = Copy_structRunnableConfig;
        }
    }

    return Local_enuErrorStatus;
}

static void HSCHEDULER_enuCyclicRunRunnable(void)
{

    static uint64_t Local_uint64Tick = 0;
    uint32_t Local_uint32Iter;

    for (Local_uint32Iter = 0; Local_uint32Iter < SCHEDULER_MAX_TASKS; Local_uint32Iter++)
    {
        if ((Global_structRunnableConfigArr[Local_uint32Iter] != NULL) && (Global_structRunnableConfigArr[Local_uint32Iter]->CBF != NULL))
        {
            if ((Global_structRunnableConfigArr[Local_uint32Iter]->firstDelay == 0) && (Local_uint64Tick % Global_structRunnableConfigArr[Local_uint32Iter]->cyclicTime == 0))
            {
                Global_structRunnableConfigArr[Local_uint32Iter]->CBF();
            }
            else if (!(Global_structRunnableConfigArr[Local_uint32Iter]->firstDelay == 0))
            {
                Global_structRunnableConfigArr[Local_uint32Iter]->firstDelay--;
            }
        }
        else {}
    }

    Local_uint64Tick++;
}