#include "std_types.h"
#include "std_lib.h"
#include "error_status.h"
#include "GPIO.h"
#include "scheduler_PBCFG.h"
#include "scheduler.h"
#include "LCD.h"

typedef enum 
{
    INIT_POWER_ON,
    INIT_FUNC_SET,
    INIT_DISP_ON,
    INIT_DISP_CLR,
    INIT_ENTRY_MODE_SET,
    INIT_FINISH,
} HLCD_enuInitState_t;

typedef enum 
{
    REQ_WRITE_CHAR,
    REQ_WRITE_STR,
    REQ_SEND_COMMAND,
    REQ_SEND_NUM,
    REQ_FINISH,
} HLCD_enuUserRequest_t;

typedef enum 
{
    MODE_INIT,
    MODE_REQUEST,
    MODE_STANDBY,
} HLCD_enuMachineMode_t;

#define BUFFER_SIZE 16

static HLCD_structLCDObject_t* global_LCDObject = NULL;
static bool flag_isLCDBusy = false;
static HLCD_enuMachineMode_t currentMode;
static uint64_t stateMachineRunnerTicks = 0;
static char LCDBuffer[BUFFER_SIZE] = {NULL};
static HLCD_enuUserRequest_t CurrentReqState;
static uint8_t passedStrLen;


static void HLCD_voidStateMachineRunner(void);
static void HLCD_voidUserRequestStateMachine(void);
static void HLCD_voidWriteCommand(uint8_t local_uint8Command);
static void HLCD_voidInitStateMachine(void);


// gets run every 2ms
static void HLCD_voidStateMachineRunner(void)
{
    // increment it
    stateMachineRunnerTicks++;

    if (currentMode == MODE_INIT)
    {
        // call init state machine
        HLCD_voidInitStateMachine();
    }
    else if (currentMode == MODE_REQUEST)
    {
        // call request state machine
        HLCD_voidUserRequestStateMachine();
    }
    else {}

    return;
}

static void HLCD_voidUserRequestStateMachine(void)
{
    static uint8_t local_uint8StrCharCounter = 0;
    switch (CurrentReqState)
    {
        case REQ_WRITE_CHAR:
            HLCD_voidWriteCommand(LCDBuffer[0]);
            CurrentReqState = REQ_FINISH;
        break;
        
        case REQ_WRITE_STR:
            if ((local_uint8StrCharCounter < passedStrLen) && (LCDBuffer[local_uint8StrCharCounter] != NULL))
            {
                HLCD_voidWriteCommand(LCDBuffer[local_uint8StrCharCounter++]);
            }
            else 
            {
                local_uint8StrCharCounter = 0;
                CurrentReqState = REQ_FINISH;
            }
        break;

        case REQ_SEND_COMMAND:
            HLCD_voidWriteCommand(LCDBuffer[0]);
            CurrentReqState = REQ_FINISH;
        break;

        case REQ_SEND_NUM:
            // still needs to be implemented
            CurrentReqState = REQ_FINISH;
        break;

        case REQ_FINISH:
            MGPIO_enuSetPinVal(global_LCDObject->configPort, global_LCDObject->configPins[LCD_EN], PIN_LOW);
            flag_isLCDBusy = false;
        break;
    }
} 

static void HLCD_voidWriteCommand(uint8_t local_uint8Command)
{
    uint8_t local_uint8Iter;
    // set rs to low
    MGPIO_enuSetPinVal(global_LCDObject->configPort, global_LCDObject->configPins[LCD_RS], PIN_LOW);
    // set rw to low
    MGPIO_enuSetPinVal(global_LCDObject->configPort, global_LCDObject->configPins[LCD_RW], PIN_LOW);
    // write command on port pins
    for (local_uint8Iter = LCD_D0; local_uint8Iter <= LCD_D7; local_uint8Iter++)
    {
        local_uint8Command >>= local_uint8Iter;
        MGPIO_enuSetPinVal(global_LCDObject->dataPort, global_LCDObject->dataPins[local_uint8Iter], local_uint8Command & 0x01U ? PIN_HIGH : PIN_LOW);
    }
    // set en to high
    MGPIO_enuSetPinVal(global_LCDObject->configPort, global_LCDObject->configPins[LCD_EN], PIN_HIGH);
    // return
    return;
}

// gets run every 2ms
static void HLCD_voidInitStateMachine(void)
{
    static HLCD_enuInitState_t local_enuCurrentInitState = INIT_POWER_ON;

    switch (local_enuCurrentInitState)
    {
        case INIT_POWER_ON:
            flag_isLCDBusy = true;
            if ((stateMachineRunnerTicks % 25 == 0) && (stateMachineRunnerTicks != 0))
            {
                local_enuCurrentInitState = INIT_FUNC_SET;
            }
            else {}
        break;

        case INIT_FUNC_SET:
            HLCD_voidWriteCommand(HLCD_CMD06_8_BIT_MODE | HLCD_CMD06_5x8_DOTS | HLCD_CMD06_2_DISPLAY_LINE);
            local_enuCurrentInitState = INIT_DISP_ON;
        break;

        case INIT_DISP_ON:
            // EN = 0
            MGPIO_enuSetPinVal(global_LCDObject->configPort, global_LCDObject->configPins[LCD_EN], PIN_LOW);
            HLCD_voidWriteCommand(HLCD_CMD04_DISPLAY_ON | HLCD_CMD04_CURSOR_ON | HLCD_CMD04_CURSOR_BLINK_ON);
            local_enuCurrentInitState = INIT_DISP_CLR;
        break;

        case INIT_DISP_CLR:
            // EN = 0
            MGPIO_enuSetPinVal(global_LCDObject->configPort, global_LCDObject->configPins[LCD_EN], PIN_LOW);
            HLCD_voidWriteCommand(HLCD_CMD01_CLR_DISPLAY);
            local_enuCurrentInitState = INIT_ENTRY_MODE_SET;
        break;

        case INIT_ENTRY_MODE_SET:
            // EN = 0
            MGPIO_enuSetPinVal(global_LCDObject->configPort, global_LCDObject->configPins[LCD_EN], PIN_LOW);
            HLCD_voidWriteCommand(HLCD_CMD03_CURSOR_MOVEMENT_DIR_INC | HLCD_CMD03_DISPLAY_SHIFT_OFF);
            local_enuCurrentInitState = INIT_FINISH;
        break;

        case INIT_FINISH:
            // EN = 0
            MGPIO_enuSetPinVal(global_LCDObject->configPort, global_LCDObject->configPins[LCD_EN], PIN_LOW);
            flag_isLCDBusy = false;
        break;
    }

    return;
}

SRV_enuErrorStatus_t HLCD_enuAsyncInit(const HLCD_structLCDObject_t* const ptr_structLCDObject)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    uint8_t local_uint8Iter;
    // check if it equals null
    if (ptr_structLCDObject == NULL)
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else if (flag_isLCDBusy == true)  // check if busy 
    {
        ret_enuErrorStatus = LCD_BUSY;
    }
    else 
    {
        // set busy
        flag_isLCDBusy = true;
        // set mode to init
        currentMode = MODE_INIT;
        // save LCD object
        global_LCDObject = ptr_structLCDObject;
        // call GPIO functions to set pin mode for config pins
        for (local_uint8Iter = LCD_RS; local_uint8Iter <= LCD_EN; local_uint8Iter++)
        {
            ret_enuErrorStatus = MGPIO_enuSetPinMode(global_LCDObject->configPort, global_LCDObject->configPins[local_uint8Iter], GPOUT);
            if (ret_enuErrorStatus != ALL_OK)
            {
                break;
            }
            ret_enuErrorStatus = MGPIO_enuOutputSpeed(global_LCDObject->configPort, global_LCDObject->configPins[local_uint8Iter], VHIGH_SPEED);
            if (ret_enuErrorStatus != ALL_OK)
            {
                break;
            }
        }
        // call GPIO function to set pin mode for data pins
        for (local_uint8Iter = LCD_D0; local_uint8Iter <= LCD_D7; local_uint8Iter++)
        {
            ret_enuErrorStatus = MGPIO_enuSetPinMode(global_LCDObject->dataPort, global_LCDObject->dataPins[local_uint8Iter], GPOUT);
            if (ret_enuErrorStatus != ALL_OK)
            {
                break;
            }
            ret_enuErrorStatus = MGPIO_enuOutputSpeed(global_LCDObject->dataPort, global_LCDObject->dataPins[local_uint8Iter], VHIGH_SPEED);
            if (ret_enuErrorStatus != ALL_OK)
            {
                break;
            }
        }
        // set callback to sched
        HSCHEDULER_enuRegisterRunnable(HLCD_voidStateMachineRunner);
    }
    
    // return
    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t HLCD_enuAsyncWriteStr(const char* const ptr_string)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    passedStrLen = strlen(ptr_string);
    if (ptr_string == NULL)
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else if (flag_isLCDBusy == true)  // check if busy 
    {
        ret_enuErrorStatus = LCD_BUSY;
    }
    else if (passedStrLen > BUFFER_SIZE)
    {
        ret_enuErrorStatus = LCD_BUF_OVF;
    }
    else 
    {
        currentMode = MODE_REQUEST;
        flag_isLCDBusy = true;
        CurrentReqState = REQ_WRITE_STR;
        // copy string into buffer
        strncpy(LCDBuffer, ptr_string, passedStrLen);
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t HLCD_enuWriteChar(const uint8_t local_uint8Char)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    if (flag_isLCDBusy == true) // check if busy
    {
        ret_enuErrorStatus = LCD_BUSY;
    }
    else 
    {
        currentMode = MODE_REQUEST;
        flag_isLCDBusy = true;
        CurrentReqState = REQ_WRITE_CHAR;
        // copy char into buffer (count is only one char)
        strncpy(LCDBuffer, &local_uint8Char, 1);
    }
    
    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t HLCD_enuSendCommand(const uint8_t local_uint8Command)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    if (flag_isLCDBusy == true) // check if busy
    {
        ret_enuErrorStatus = LCD_BUSY;
    }
    else 
    {
        currentMode = MODE_REQUEST;
        flag_isLCDBusy = true;
        CurrentReqState = REQ_SEND_COMMAND;
        // copy command into buffer (count is only one char)
        strncpy(LCDBuffer, &local_uint8Command, 1);
    }
    
    return ret_enuErrorStatus;
}

