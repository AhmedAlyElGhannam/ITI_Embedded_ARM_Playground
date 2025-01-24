/**
 * @file GPIO.c
 * @brief GPIO Driver Interface
 *
 * Provides functions and macros for configuring and manipulating GPIO pins.
 */

#include "std_types.h"  /**< Standard types definition header */
#include "error_status.h"  /**< Error status codes definition header */
#include "GPIO_Reg.h"  /**< GPIO registers mapping header */
#include "GPIO.h"  /**< GPIO module header */
#include "GPIO_LCFG.h"

/**
 * @brief Macro to check if the port is invalid.
 * @param GPIOx GPIO port to check.
 */
#define IS_INVALID_PORT(GPIOx)  ((GPIOx == GPIOF) || (GPIOx == GPIOG) || (GPIOx >= GPIOH))

/**
 * @brief Macro to check if the pin is invalid for a specific port.
 * @param GPIOx GPIO port.
 * @param PINx GPIO pin.
 */
#define IS_INVALID_PIN(PINx)    ((PINx > PIN15))

/**
 * @brief Macro to check if a pointer is NULL.
 * @param PTR Pointer to check.
 */
#define IS_INVALID_PTR(PTR)     (NULL == PTR)

/**
 * @brief Macro to check if the mode is invalid.
 * @param MODE Pin mode to check.
 */
#define IS_INVALID_MODE(MODE)   (MODE > ANALOG)

/**
 * @brief Macro to check if the alternate function is invalid.
 * @param AF Alternate function value to check.
 */
#define IS_INVALID_ALT_FUNC(AF) (AF > ALT_FUNC15)

/**
 * @brief Macro to check if the output type is invalid.
 * @param OUT Output type to check.
 */
#define IS_INVALID_OUT_TYPE(OUT)    (OUT > ODR)

/**
 * @brief Macro to check if the output speed is invalid.
 * @param SPD Output speed to check.
 */
#define IS_INVALID_OUT_SPEED(SPD)   (SPD > VHIGH_SPEED)

/**
 * @brief Macro to check if the resistor configuration is invalid.
 * @param RES Resistor configuration to check.
 */
#define IS_INVALID_RES_CONFIG(RES)  (RES > PD_RES)

/**
 * @brief Macro to check if the pin value is invalid.
 * @param VAL Pin value to check.
 */
#define IS_INVALID_PIN_VAL(VAL)     ((VAL != PIN_HIGH) && (VAL != PIN_LOW))

/**
 * @brief Array for pin configurations
 */
extern MGPIO_structPinConfig_t global_structArrPinConfig[MGPIO_NUM_OF_PORTS * MGPIO_NUM_OF_PINS];

/**
 * @brief Sets the mode of a specific GPIO pin.
 * @param copy_enuPortNum GPIO port.
 * @param copy_enuPinNum GPIO pin.
 * @param copy_enuPinMode Desired pin mode.
 * @return Error status.
 */
SRV_enuErrorStatus_t MGPIO_enuSetPinMode(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinMode_t copy_enuPinMode)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (IS_INVALID_PORT(copy_enuPortNum))
    {
        ret_enuErrorStatus = GPIO_INV_PORT;
    }
    else if (IS_INVALID_PIN(copy_enuPinNum))
    {
        ret_enuErrorStatus = GPIO_INV_PIN;
    }
    else if (IS_INVALID_MODE(copy_enuPinMode))
    {
        ret_enuErrorStatus = INV_ARG;
    }
    else
    {
        GPIOx_Registers_t* ptr_structGPIOPortBaseAdd = GPIO_GET_PORT_ADDRESS(copy_enuPortNum);

        ptr_structGPIOPortBaseAdd->MODER &= ~(0x03UL << (2 * copy_enuPinNum));
        ptr_structGPIOPortBaseAdd->MODER |= (copy_enuPinMode << (2 * copy_enuPinNum));
    }

    return ret_enuErrorStatus;
}

/**
 * @brief Configures the alternate function of a GPIO pin.
 * @param copy_enuPortNum GPIO port.
 * @param copy_enuPinNum GPIO pin.
 * @param copy_enuPinAltFunc Alternate function to set.
 * @return Error status.
 */
SRV_enuErrorStatus_t MGPIO_enuAltFuncSel(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinAltFunc_t copy_enuPinAltFunc)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (IS_INVALID_PORT(copy_enuPortNum))
    {
        ret_enuErrorStatus = GPIO_INV_PORT;
    }
    else if (IS_INVALID_PIN(copy_enuPinNum))
    {
        ret_enuErrorStatus = GPIO_INV_PIN;
    }
    else if (IS_INVALID_ALT_FUNC(copy_enuPinAltFunc))
    {
        ret_enuErrorStatus = INV_ARG;
    }
    else
    {
        GPIOx_Registers_t* ptr_structGPIOPortBaseAdd = GPIO_GET_PORT_ADDRESS(copy_enuPortNum);

        if (copy_enuPinNum < PIN08)
        {
            ptr_structGPIOPortBaseAdd->AFRL = (ptr_structGPIOPortBaseAdd->AFRL & ~(0xFFUL << (4 * copy_enuPinNum))) | (copy_enuPinAltFunc << (4 * copy_enuPinNum));
        }
        else if (copy_enuPinNum >= PIN08)
        {
            ptr_structGPIOPortBaseAdd->AFRH = (ptr_structGPIOPortBaseAdd->AFRH & ~(0xFFUL << (4 * (copy_enuPinNum - PIN08)))) | (copy_enuPinAltFunc << (4 * (copy_enuPinNum - PIN08)));
        }
        else {}
        
    }

    return ret_enuErrorStatus;
}

/**
 * @brief Sets the output type of a GPIO pin.
 * @param copy_enuPortNum GPIO port.
 * @param copy_enuPinNum GPIO pin.
 * @param copy_enuPinOutType Output type to set.
 * @return Error status.
 */
SRV_enuErrorStatus_t MGPIO_enuOutputType(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinOutType_t copy_enuPinOutType)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (IS_INVALID_PORT(copy_enuPortNum))
    {
        ret_enuErrorStatus = GPIO_INV_PORT;
    }
    else if (IS_INVALID_PIN(copy_enuPinNum))
    {
        ret_enuErrorStatus = GPIO_INV_PIN;
    }
    else if (IS_INVALID_OUT_TYPE(copy_enuPinOutType))
    {
        ret_enuErrorStatus = INV_ARG;
    }
    else
    {
        GPIOx_Registers_t* ptr_structGPIOPortBaseAdd = GPIO_GET_PORT_ADDRESS(copy_enuPortNum);

        ptr_structGPIOPortBaseAdd->OTYPER = (ptr_structGPIOPortBaseAdd->OTYPER & ~(0x01UL << copy_enuPinNum)) | (copy_enuPinOutType << copy_enuPinNum);
    }

    return ret_enuErrorStatus;
}

/**
 * @brief Sets the output speed of a GPIO pin.
 * @param copy_enuPortNum GPIO port.
 * @param copy_enuPinNum GPIO pin.
 * @param copy_enuPinOutSpeed Output speed to set.
 * @return Error status.
 */
SRV_enuErrorStatus_t MGPIO_enuOutputSpeed(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinOutSpeed_t copy_enuPinOutSpeed)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (IS_INVALID_PORT(copy_enuPortNum))
    {
        ret_enuErrorStatus = GPIO_INV_PORT;
    }
    else if (IS_INVALID_PIN(copy_enuPinNum))
    {
        ret_enuErrorStatus = GPIO_INV_PIN;
    }
    else if (IS_INVALID_OUT_SPEED(copy_enuPinOutSpeed))
    {
        ret_enuErrorStatus = INV_ARG;
    }
    else
    {
        GPIOx_Registers_t* ptr_structGPIOPortBaseAdd = GPIO_GET_PORT_ADDRESS(copy_enuPortNum);

        ptr_structGPIOPortBaseAdd->OSPEEDR = (ptr_structGPIOPortBaseAdd->OSPEEDR & ~(0x03UL << (2 * copy_enuPinNum))) | (copy_enuPinOutSpeed << (2 * copy_enuPinNum));
    }

    return ret_enuErrorStatus;
}

/**
 * @brief Configures pull-up or pull-down resistors for a GPIO pin.
 * @param copy_enuPortNum GPIO port.
 * @param copy_enuPinNum GPIO pin.
 * @param copy_enuPinResConfig Resistor configuration.
 * @return Error status.
 */
SRV_enuErrorStatus_t MGPIO_enuPinResConfig(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinResConfig_t copy_enuPinResConfig)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (IS_INVALID_PORT(copy_enuPortNum))
    {
        ret_enuErrorStatus = GPIO_INV_PORT;
    }
    else if (IS_INVALID_PIN(copy_enuPinNum))
    {
        ret_enuErrorStatus = GPIO_INV_PIN;
    }
    else if (IS_INVALID_RES_CONFIG(copy_enuPinResConfig))
    {
        ret_enuErrorStatus = INV_ARG;
    }
    else
    {
        GPIOx_Registers_t* ptr_structGPIOPortBaseAdd = GPIO_GET_PORT_ADDRESS(copy_enuPortNum);

        ptr_structGPIOPortBaseAdd->PUPDR = (ptr_structGPIOPortBaseAdd->PUPDR & ~(0x03UL << (2 * copy_enuPinNum))) | (copy_enuPinResConfig << (2 * copy_enuPinNum));
    }

    return ret_enuErrorStatus;
}

/**
 * @brief Sets the value of a GPIO pin.
 * @param copy_enuPortNum GPIO port.
 * @param copy_enuPinNum GPIO pin.
 * @param copy_enuPinVal Value to set.
 * @return Error status.
 */
SRV_enuErrorStatus_t MGPIO_enuSetPinVal(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinVal_t copy_enuPinVal)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (IS_INVALID_PORT(copy_enuPortNum))
    {
        ret_enuErrorStatus = GPIO_INV_PORT;
    }
    else if (IS_INVALID_PIN(copy_enuPinNum))
    {
        ret_enuErrorStatus = GPIO_INV_PIN;
    }
    else if (IS_INVALID_PIN_VAL(copy_enuPinVal))
    {
        ret_enuErrorStatus = INV_ARG;
    }
    else
    {
        GPIOx_Registers_t* ptr_structGPIOPortBaseAdd = GPIO_GET_PORT_ADDRESS(copy_enuPortNum);

        ptr_structGPIOPortBaseAdd->BSRR = (ptr_structGPIOPortBaseAdd->BSRR & ~(copy_enuPinVal << copy_enuPinNum)) | (copy_enuPinVal << copy_enuPinNum);
    }

    return ret_enuErrorStatus;
}

/**
 * @brief Reads the value of a GPIO pin.
 * @param copy_enuPortNum GPIO port.
 * @param copy_enuPinNum GPIO pin.
 * @param ptr_uint32PinVal Pointer to store the pin value.
 * @return Error status.
 */
SRV_enuErrorStatus_t MGPIO_enuGetPinVal(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, uint32_t* ptr_uint32PinVal)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (IS_INVALID_PORT(copy_enuPortNum))
    {
        ret_enuErrorStatus = GPIO_INV_PORT;
    }
    else if (IS_INVALID_PIN(copy_enuPinNum))
    {
        ret_enuErrorStatus = GPIO_INV_PIN;
    }
    else if (IS_INVALID_PTR(ptr_uint32PinVal))
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else
    {
        GPIOx_Registers_t* ptr_structGPIOPortBaseAdd = GPIO_GET_PORT_ADDRESS(copy_enuPortNum);

        (*ptr_uint32PinVal) = ((ptr_structGPIOPortBaseAdd->IDR >> copy_enuPinNum) & 0x01UL);
    }

    return ret_enuErrorStatus;
}

/**
 * @brief Enables the pin lock mechanism for a GPIO pin.
 * @param copy_enuPortNum GPIO port.
 * @param copy_enuPinNum GPIO pin.
 * @return Error status.
 */
SRV_enuErrorStatus_t MGPIO_enuPinLockEnable(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (IS_INVALID_PORT(copy_enuPortNum))
    {
        ret_enuErrorStatus = GPIO_INV_PORT;
    }
    else if (IS_INVALID_PIN(copy_enuPinNum))
    {
        ret_enuErrorStatus = GPIO_INV_PIN;
    }
    else
    {
        GPIOx_Registers_t* ptr_structGPIOPortBaseAdd = GPIO_GET_PORT_ADDRESS(copy_enuPortNum);

        /* set bit corresponding to pin */
        ptr_structGPIOPortBaseAdd->LCKR = (ptr_structGPIOPortBaseAdd->LCKR & ~(0x01UL << copy_enuPinNum)) | (0x01UL << copy_enuPinNum);

        /* set LCKK */
        ptr_structGPIOPortBaseAdd->LCKR |= 0x10000UL;

        /* clr LCKK */
        ptr_structGPIOPortBaseAdd->LCKR &= ~(0x10000UL);

        /* set LCKK */
        ptr_structGPIOPortBaseAdd->LCKR |= 0x10000UL;

        /* if LCKK is not set -> return NOK */
        if (!((ptr_structGPIOPortBaseAdd->LCKR >> 16) & 0x01UL))
        {
            ret_enuErrorStatus = NOK;
        }
        else {} /* lock was set successfully */

    }

    return ret_enuErrorStatus;
}
