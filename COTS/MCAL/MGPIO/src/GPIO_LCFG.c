#include "std_types.h"  /**< Standard types definition header */
#include "error_status.h"  /**< Error status codes definition header */
#include "GPIO_Reg.h"  /**< GPIO registers mapping header */
#include "GPIO.h"  /**< GPIO module header */
#include "GPIO_LCFG.h"

MGPIO_structPinConfig_t global_structArrPinConfig[MGPIO_NUM_OF_PORTS * MGPIO_NUM_OF_PINS] = 
{
    [(GPIOA * MGPIO_NUM_OF_PINS) + PIN00] = 
    {
        .altFunc = ALT_FUNC_NONE,
        .outSpeed = VHIGH_SPEED,
        .outType = PUSH_PULL,
        .pinMode = GPOUT,
        .resConfig = NO_RES
    }
};