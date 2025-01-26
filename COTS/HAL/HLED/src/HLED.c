#include "std_types.h"
#include "error_status.h"
#include "GPIO_LCFG.h"
#include "GPIO.h"
#include "HLED_LCFG.h"
#include "HLED.h"

extern HLED_structLEDConfig_t HLED_structArrLEDConfig[LED_COUNT];

void HLED_voidInitUserLEDs(void)
{
    uint8_t local_uint8Iter;

    for (local_uint8Iter = 0; local_uint8Iter < LED_COUNT; local_uint8Iter++)
    {
        MGPIO_enuSetPinMode(HLED_structArrLEDConfig[local_uint8Iter].port, HLED_structArrLEDConfig[local_uint8Iter].pin, GPOUT);
        MGPIO_enuOutputType(HLED_structArrLEDConfig[local_uint8Iter].port, HLED_structArrLEDConfig[local_uint8Iter].pin, HLED_structArrLEDConfig[local_uint8Iter].pin_type);
        MGPIO_enuPinResConfig(HLED_structArrLEDConfig[local_uint8Iter].port, HLED_structArrLEDConfig[local_uint8Iter].pin, NO_RES);

        /* setting default led state */
    }
}

SRV_enuErrorStatus_t HLED_enuSetLEDState(HLED_enuUserDefinedLEDNames_t copy_enuLEDName, HLED_enuLEDState_t copy_enuLEDState)
{
    /* do some error detection */
    uint8_t state = copy_enuLEDState ^ HLED_structArrLEDConfig[copy_enuLEDName].connection;

    /* set this in gpio function */
}