#ifndef HLED_H
#define HLED_H

typedef struct 
{
    uint8_t port : 4;
    uint8_t pin : 4;
    uint16_t connection : 4;
    uint16_t initial_state : 8;
    uint16_t pin_type : 4;
} HLED_structLEDConfig_t;

typedef enum 
{
    LED_ON = 1U,
    LED_OFF = 0U,
} HLED_enuLEDState_t;

typedef enum 
{
    ACTIVE_LOW = 0U,
    ACTIVE_HIGH = 1U,
} HLED_enuLEDConnection_t;


#endif