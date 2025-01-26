#define "GPIO.h"
#include "HLED_LCFG.h"
#include "HLED.h"

HLED_structLEDConfig_t HLED_structArrLEDConfig[LED_COUNT] = 
{
    [LED_START] = 
    {
        .port = GPIOA,
        .pin = PIN05,
        .connection = ACTIVE_HIGH,
        .initial_state = LED_OFF,
        .pin_type = PUSH_PULL
    }
};