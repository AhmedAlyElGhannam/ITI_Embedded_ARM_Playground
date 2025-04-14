#include "std_types.h"
#include "error_status.h"
#include "SYSTICK_Reg.h"
#include "SYSTICK.h"

static volatile uint32_t Global_uint32CyclicTime = 0;
static volatile uint32_t Global_uint32LoadValue = 0;
static volatile uint32_t Global_uint32Counter = 0;
static MSYSTICK_CBF_t SYSTICK_CBF = NULL;

SYSTICK_Registers_t* SYSTICK = (SYSTICK_Registers_t*)SYSTICK_BASE_ADDRESS;

void MSYSTICK_voidStart(void)
{
    SYSTICK->STK_CTRL = SYSTICK->STK_CTRL | 1U;
}

void MSYSTICK_voidStop(void)
{
    SYSTICK->STK_CTRL = SYSTICK->STK_CTRL & 0xFFFFFFFE;
}

void SysTick_Handler(void)
{
    Global_uint32Counter++;
    if((SYSTICK_CBF != NULL) && (Global_uint32Counter % Global_uint32CyclicTime == 0) && (Global_uint32Counter != 0))
    {
        SYSTICK_CBF();
    }
}

SRV_enuErrorStatus_t MSYSTICK_enuSetCyclicTime(MSYSTICK_structTimerConfig_t* Copy_structTimerConfig)
{
    SRV_enuErrorStatus_t Local_enuErrorStatus = ALL_OK;
    if (Copy_structTimerConfig == NULL)
    {
        Local_enuErrorStatus = NULL_PTR;
    }
    else if (Copy_structTimerConfig->CBF == NULL)
    {
        Local_enuErrorStatus = NULL_PTR;
    }
    else 
    {
        /* 1ms tick needs a value 50000 to be loaded in  */
        Global_uint32CyclicTime = Copy_structTimerConfig->cyclicTime;  // set cbf cyclic time
        // set cbf
        SYSTICK_CBF = Copy_structTimerConfig->CBF;
    }

    return Local_enuErrorStatus;
}

void MSYSTICK_voidInit(void)
{
    /* ticktime is 20ns for 50MHz */
    Global_uint32LoadValue = 16000U;
    // set right load value
    SYSTICK->STK_LOAD = Global_uint32LoadValue;
    SYSTICK->STK_VAL = 0U;
    SYSTICK->STK_CTRL = (SYSTICK->STK_CTRL) | 6U; // set clk source and int enable
    // SYSTICK->STK_CTRL = (SYSTICK->STK_CTRL) | 1U; // enable int
}