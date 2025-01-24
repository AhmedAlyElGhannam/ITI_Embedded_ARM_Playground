/**
 * @file GPIO.h
 * @brief Header file for GPIO operations and configurations.
 */

#ifndef GPIO_H
#define GPIO_H

/**
 * @enum MGPIO_enuPortDesignator_t
 * @brief Enumeration for GPIO port designators.
 */
typedef enum
{
    GPIOA = 0x00UL, /**< GPIO Port A */
    GPIOB,          /**< GPIO Port B */
    GPIOC,          /**< GPIO Port C */
    GPIOD,          /**< GPIO Port D */
    GPIOE,          /**< GPIO Port E */
    GPIOF,          /**< GPIO Port F (unsupported) */
    GPIOG,          /**< GPIO Port G (unsupported) */
    GPIOH,          /**< GPIO Port H */
} MGPIO_enuPortDesignator_t;

/**
 * @enum MGPIO_enuPinDesignator_t
 * @brief Enumeration for GPIO pin designators.
 */
typedef enum
{
    PIN00 = 0x00UL, /**< GPIO Pin 0 */
    PIN01,          /**< GPIO Pin 1 */
    PIN02,          /**< GPIO Pin 2 */
    PIN03,          /**< GPIO Pin 3 */
    PIN04,          /**< GPIO Pin 4 */
    PIN05,          /**< GPIO Pin 5 */
    PIN06,          /**< GPIO Pin 6 */
    PIN07,          /**< GPIO Pin 7 */
    PIN08,          /**< GPIO Pin 8 */
    PIN09,          /**< GPIO Pin 9 */
    PIN10,          /**< GPIO Pin 10 */
    PIN11,          /**< GPIO Pin 11 */
    PIN12,          /**< GPIO Pin 12 */
    PIN13,          /**< GPIO Pin 13 */
    PIN14,          /**< GPIO Pin 14 */
    PIN15 = 0x0FUL, /**< GPIO Pin 15 */
} MGPIO_enuPinDesignator_t;

/**
 * @enum MGPIO_enuPinMode_t
 * @brief Enumeration for GPIO pin modes.
 */
typedef enum
{
    INPUT  = 0b00UL, /**< Input mode */
    GPOUT  = 0b01UL, /**< General-purpose output mode */
    ALTFNC = 0b10UL, /**< Alternate function mode */
    ANALOG = 0b11UL, /**< Analog mode */
} MGPIO_enuPinMode_t;

/**
 * @enum MGPIO_enuPinAltFunc_t
 * @brief Enumeration for GPIO alternate functions.
 */
typedef enum
{
    ALT_FUNC00 = 0x00UL, /**< Alternate Function 0 */
    ALT_FUNC01 = 0x01UL, /**< Alternate Function 1 */
    ALT_FUNC02 = 0x02UL, /**< Alternate Function 2 */
    ALT_FUNC03 = 0x03UL, /**< Alternate Function 3 */
    ALT_FUNC04 = 0x04UL, /**< Alternate Function 4 */
    ALT_FUNC05 = 0x05UL, /**< Alternate Function 5 */
    ALT_FUNC06 = 0x06UL, /**< Alternate Function 6 */
    ALT_FUNC07 = 0x07UL, /**< Alternate Function 7 */
    ALT_FUNC08 = 0x08UL, /**< Alternate Function 8 */
    ALT_FUNC09 = 0x09UL, /**< Alternate Function 9 */
    ALT_FUNC10 = 0x0AUL, /**< Alternate Function 10 */
    ALT_FUNC11 = 0x0BUL, /**< Alternate Function 11 */
    ALT_FUNC12 = 0x0CUL, /**< Alternate Function 12 */
    ALT_FUNC13 = 0x0DUL, /**< Alternate Function 13 */
    ALT_FUNC14 = 0x0EUL, /**< Alternate Function 14 */
    ALT_FUNC15 = 0x0FUL, /**< Alternate Function 15 */
    ALT_FUNC_NONE = 0xF0UL
} MGPIO_enuPinAltFunc_t;

/**
 * @enum MGPIO_enuPinOutType_t
 * @brief Enumeration for GPIO pin output types.
 */
typedef enum
{
    PUSH_PULL = 0, /**< Push-pull output */
    ODR       = 1, /**< Open-drain output */
} MGPIO_enuPinOutType_t;

/**
 * @enum MGPIO_enuPinOutSpeed_t
 * @brief Enumeration for GPIO pin output speeds.
 */
typedef enum
{
    LOW_SPEED   = 0b00UL, /**< Low speed */
    MED_SPEED   = 0b01UL, /**< Medium speed */
    HIGH_SPEED  = 0b10UL, /**< High speed */
    VHIGH_SPEED = 0b11UL, /**< Very high speed */
} MGPIO_enuPinOutSpeed_t;

/**
 * @enum MGPIO_enuPinResConfig_t
 * @brief Enumeration for GPIO pin resistor configurations.
 */
typedef enum
{
    NO_RES = 0b00UL, /**< No resistor */
    PU_RES = 0b01UL, /**< Pull-up resistor */
    PD_RES = 0b10UL, /**< Pull-down resistor */
} MGPIO_enuPinResConfig_t;

/**
 * @enum MGPIO_enuPinVal_t
 * @brief Enumeration for GPIO pin values.
 */
typedef enum
{
    PIN_HIGH = 0x00000001UL, /**< Pin High value */
    PIN_LOW  = 0x00010000UL, /**< Pin Low value */
} MGPIO_enuPinVal_t;


/**
 * @brief Sets the mode of a GPIO pin.
 * @param copy_enuPortNum Port designator.
 * @param copy_enuPinNum Pin designator.
 * @param copy_enuPinMode Pin mode.
 * @return Error status.
 */
SRV_enuErrorStatus_t MGPIO_enuSetPinMode(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinMode_t copy_enuPinMode);

/**
 * @brief Selects the alternate function of a GPIO pin.
 * @param copy_enuPortNum Port designator.
 * @param copy_enuPinNum Pin designator.
 * @param copy_enuPinAltFunc Alternate function.
 * @return Error status.
 */
SRV_enuErrorStatus_t MGPIO_enuAltFuncSel(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinAltFunc_t copy_enuPinAltFunc);

/**
 * @brief Sets the output type of a GPIO pin.
 * @param copy_enuPortNum Port designator.
 * @param copy_enuPinNum Pin designator.
 * @param copy_enuPinOutType Output type.
 * @return Error status.
 */
SRV_enuErrorStatus_t MGPIO_enuOutputType(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinOutType_t copy_enuPinOutType);

/**
 * @brief Sets the output speed of a GPIO pin.
 * @param copy_enuPortNum Port designator.
 * @param copy_enuPinNum Pin designator.
 * @param copy_enuPinOutSpeed Output speed.
 * @return Error status.
 */
SRV_enuErrorStatus_t MGPIO_enuOutputSpeed(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinOutSpeed_t copy_enuPinOutSpeed);

/**
 * @brief Configures the resistor settings of a GPIO pin.
 * @param copy_enuPortNum Port designator.
 * @param copy_enuPinNum Pin designator.
 * @param copy_enuPinResConfig Resistor configuration.
 * @return Error status.
 */
SRV_enuErrorStatus_t MGPIO_enuPinResConfig(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinResConfig_t copy_enuPinResConfig);

/**
 * @brief Sets the value of a GPIO pin.
 * @param copy_enuPortNum Port designator.
 * @param copy_enuPinNum Pin designator.
 * @param copy_enuPinVal Pin value.
 * @return Error status.
 */
SRV_enuErrorStatus_t MGPIO_enuSetPinVal(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinVal_t copy_enuPinVal);

/**
 * @brief Reads the value of a GPIO pin.
 * @param copy_enuPortNum Port designator.
 * @param copy_enuPinNum Pin designator.
 * @param ptr_uint32PinVal Pointer to store pin value.
 * @return Error status.
 */
SRV_enuErrorStatus_t MGPIO_enuGetPinVal(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, uint32_t* ptr_uint32PinVal);

/**
 * @brief Enables the lock for a GPIO pin.
 * @param copy_enuPortNum Port designator.
 * @param copy_enuPinNum Pin designator.
 * @return Error status.
 */
SRV_enuErrorStatus_t MGPIO_enuPinLockEnable(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum);

#endif
