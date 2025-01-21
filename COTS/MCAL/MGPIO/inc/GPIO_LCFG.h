#ifndef GPIO_LCFG_H
#define GPIO_LCFG_H

/**
 * @struct
 * @brief
 */
typedef struct
{
    MGPIO_enuPinMode_t pinMode;
    MGPIO_enuPinAltFunc_t altFunc;
    MGPIO_enuPinOutType_t outType;
    MGPIO_enuPinOutSpeed_t outSpeed;
    MGPIO_enuPinResConfig_t resConfig;
} MGPIO_structPinConfig_t;

#define MGPIO_NUM_OF_PORTS      (7UL)
#define MGPIO_NUM_OF_PINS       (16UL)

#endif