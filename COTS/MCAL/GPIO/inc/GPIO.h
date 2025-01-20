#ifndef GPIO_H
#define GPIO_H



typedef enum
{
    GPIOA = 0x00UL,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOF, /* unsupported */
    GPIOG, /* unsupported */
    GPIOH,
} MGPIO_enuPortDesignator_t;

typedef enum
{
    PIN00 = 0x00UL,
    PIN01,
    PIN02,
    PIN03,
    PIN04,
    PIN05,
    PIN06,
    PIN07,
    PIN08,
    PIN09,
    PIN10,
    PIN11,
    PIN12,
    PIN13,
    PIN14,
    PIN15 = 0xFFUL,
} MGPIO_enuPinDesignator_t;

typedef enum
{
    INPUT  = 0b00UL,
    GPOUT  = 0b01UL,
    ALTFNC = 0b10UL,
    ANALOG = 0b11UL,
} MGPIO_enuPinMode_t;

typedef enum
{
    ALT_FUNC00 = 0x00UL,
    ALT_FUNC01 = 0x01UL,
    ALT_FUNC02 = 0x02UL,
    ALT_FUNC03 = 0x03UL,
    ALT_FUNC04 = 0x04UL,
    ALT_FUNC05 = 0x05UL,
    ALT_FUNC06 = 0x06UL,
    ALT_FUNC07 = 0x07UL,
    ALT_FUNC08 = 0x08UL,
    ALT_FUNC09 = 0x09UL,
    ALT_FUNC10 = 0x0AUL,
    ALT_FUNC11 = 0x0BUL,
    ALT_FUNC12 = 0x0CUL,
    ALT_FUNC13 = 0x0DUL,
    ALT_FUNC14 = 0x0EUL,
    ALT_FUNC15 = 0x0FUL,
} MGPIO_enuPinAltFunc_t;

typedef enum 
{
    PUSH_PULL = 0,
    ODR = 1,
} MGPIO_enuPinOutType_t;

typedef enum
{
    LOW_SPEED   = 0b00UL,
    MED_SPEED   = 0b01UL,
    HIGH_SPEED  = 0b10UL,
    VHIGH_SPEED = 0b11UL,
} MGPIO_enuPinOutSpeed_t;

typedef enum
{
    NO_RES = 0b00UL,
    PU_RES = 0b01UL,
    PD_RES = 0b10UL,
} MGPIO_enuPinResConfig_t;

typedef enum
{
    PIN_HIGH = 0x00000001UL,
    PIN_LOW  = 0x00010000UL,
} MGPIO_enuPinVal_t;






// set pin direction
SRV_enuErrorStatus_t MGPIO_enuSetPinMode(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinMode_t copy_enuPinMode);

// afsel 
SRV_enuErrorStatus_t MGPIO_enuAltFuncSel(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinAltFunc_t copy_enuPinAltFunc);

// speed
SRV_enuErrorStatus_t MGPIO_enuOutputType(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinOutType_t copy_enuPinOutType);

SRV_enuErrorStatus_t MGPIO_enuOutputSpeed(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinOutSpeed_t copy_enuPinOutSpeed);

// pu || pd
SRV_enuErrorStatus_t MGPIO_enuPinResConfig(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinResConfig_t copy_enuPinResConfig);
// write
SRV_enuErrorStatus_t MGPIO_enuSetPinVal(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, MGPIO_enuPinVal_t copy_enuPinVal);

// read
SRV_enuErrorStatus_t MGPIO_enuGetPinVal(MGPIO_enuPortDesignator_t copy_enuPortNum, MGPIO_enuPinDesignator_t copy_enuPinNum, uint32_t* ptr_uint32PinVal);

#endif