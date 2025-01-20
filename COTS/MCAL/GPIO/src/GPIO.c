#include "std_types.h"
#include "error_status.h"
#include "GPIO_Reg.h"
#include "GPIO.h"

#define NULL ((void*)0)

#define IS_INVALID_PORT(GPIOx)  ((GPIOx == GPIOF) || (GPIOx == GPIOG) || (GPIOx > GPIOH))

#define IS_INVALID_PIN(GPIOx, PINx)    ((PINx > PIN15) || ((PINx > PIN02) && (GPIOx == GPIOH)))

#define IS_INVALID_PTR(PTR)     (NULL == PTR)

#define IS_INVALID_MODE(MODE)   (MODE > ANALOG)

#define IS_INVALID_ALT_FUNC(AF) (AF > ALT_FUNC15)

#define IS_INVALID_OUT_TYPE(OUT)    (OUT > ODR)

#define IS_INVALID_OUT_SPEED(SPD)   (SPD > VHIGH_SPEED)

#define IS_INVALID_RES_CONFIG(RES)  (RES > PD_RES)

#define IS_INVALID_PIN_VAL(VAL)     ((VAL != PIN_HIGH) && (VAL != PIN_LOW))


// set pin mode
SRV_enuErrorStatus_t MGPIO_enuSetPinMode(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinMode_t copy_enuPinMode)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (IS_INVALID_PORT(copy_enuPortNum))
    {
        ret_enuErrorStatus = GPIO_INV_PORT;
    }
    else if (IS_INVALID_PIN(copy_enuPortNum, copy_enuPinNum))
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

        ptr_structGPIOPortBaseAdd->MODER = (ptr_structGPIOPortBaseAdd->MODER & ~(0x03UL << (2 * copy_enuPinNum))) | (copy_enuPinMode << (2 * copy_enuPinNum));
    }

    return ret_enuErrorStatus;
}

// afsel 
SRV_enuErrorStatus_t MGPIO_enuAltFuncSel(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinAltFunc_t copy_enuPinAltFunc)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (IS_INVALID_PORT(copy_enuPortNum))
    {
        ret_enuErrorStatus = GPIO_INV_PORT;
    }
    else if (IS_INVALID_PIN(copy_enuPortNum, copy_enuPinNum))
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

// speed
SRV_enuErrorStatus_t MGPIO_enuOutputType(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinOutType_t copy_enuPinOutType)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (IS_INVALID_PORT(copy_enuPortNum))
    {
        ret_enuErrorStatus = GPIO_INV_PORT;
    }
    else if (IS_INVALID_PIN(copy_enuPortNum, copy_enuPinNum))
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

SRV_enuErrorStatus_t MGPIO_enuOutputSpeed(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinOutSpeed_t copy_enuPinOutSpeed)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (IS_INVALID_PORT(copy_enuPortNum))
    {
        ret_enuErrorStatus = GPIO_INV_PORT;
    }
    else if (IS_INVALID_PIN(copy_enuPortNum, copy_enuPinNum))
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

// pu || pd
SRV_enuErrorStatus_t MGPIO_enuPinResConfig(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinResConfig_t copy_enuPinResConfig)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (IS_INVALID_PORT(copy_enuPortNum))
    {
        ret_enuErrorStatus = GPIO_INV_PORT;
    }
    else if (IS_INVALID_PIN(copy_enuPortNum, copy_enuPinNum))
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

// write
SRV_enuErrorStatus_t MGPIO_enuSetPinVal(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinVal_t copy_enuPinVal)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (IS_INVALID_PORT(copy_enuPortNum))
    {
        ret_enuErrorStatus = GPIO_INV_PORT;
    }
    else if (IS_INVALID_PIN(copy_enuPortNum, copy_enuPinNum))
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

// read
SRV_enuErrorStatus_t MGPIO_enuGetPinVal(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, uint32_t* ptr_uint32PinVal)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (IS_INVALID_PORT(copy_enuPortNum))
    {
        ret_enuErrorStatus = GPIO_INV_PORT;
    }
    else if (IS_INVALID_PIN(copy_enuPortNum, copy_enuPinNum))
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
