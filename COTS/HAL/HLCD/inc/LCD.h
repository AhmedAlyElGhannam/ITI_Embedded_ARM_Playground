#ifndef LCD_H
#define LCD_H

typedef enum 
{
    HLCD_CMD01_CLR_DISPLAY = 0b00000001U,
    HLCD_CMD02_RET_HOME = 0b00000010U,
    HLCD_CMD03_CURSOR_MOVEMENT_DIR_INC = 0b00000110U,
    HLCD_CMD03_CURSOR_MOVEMENT_DIR_DEC = 0b00000100U,
    HLCD_CMD03_DISPLAY_SHIFT_ON = 0b00000101U,
    HLCD_CMD03_DISPLAY_SHIFT_OFF = 0b00000100U,
    HLCD_CMD04_DISPLAY_ON = 0b00001100U,
    HLCD_CMD04_DISPLAY_OFF = 0b00001000U,
    HLCD_CMD04_CURSOR_ON = 0b00001010U,
    HLCD_CMD04_CURSOR_OFF = 0b00001000U,
    HLCD_CMD04_CURSOR_BLINK_ON = 0b00001001U,
    HLCD_CMD04_CURSOR_BLINK_OFF = 0b00001000U,
    HLCD_CMD05_DISPLAY_SHIFT_SET = 0b00011000U,
    HLCD_CMD05_CURSOR_SHIFT_SET = 0b00010000U,
    HLCD_CMD05_SHIFT_RIGHT = 0b00010100U,
    HLCD_CMD05_SHIFT_LEFT = 0b00010000U,
    HLCD_CMD06_8_BIT_MODE = 0b00110000U,
    HLCD_CMD06_4_BIT_MODE = 0b00100000U,
    HLCD_CMD06_2_DISPLAY_LINE = 0b00101000U,
    HLCD_CMD06_1_DISPLAY_LINE = 0b00100000U,
    HLCD_CMD06_5x11_DOTS = 0b00100100U,
    HLCD_CMD06_5x8_DOTS = 0b00100000U,
} HLCD_enuLCDCommands_t;

typedef enum 
{
    LCD_D0 = 0,
    LCD_D1,
    LCD_D2,
    LCD_D3,
    LCD_D4,
    LCD_D5,
    LCD_D6,
    LCD_D7,
} HLCD_enuDataPins_t;

typedef enum 
{
    LCD_RS = 0,
    LCD_RW,
    LCD_EN,
} HLCD_enuConfigPins_t;

typedef struct 
{
    uint8_t configPort;
    uint8_t dataPort;
    uint8_t configPins[3];
    uint8_t dataPins[8];
} HLCD_structLCDObject_t;

SRV_enuErrorStatus_t HLCD_enuAsyncInit(const HLCD_structLCDObject_t* const ptr_structLCDObject);
SRV_enuErrorStatus_t HLCD_enuAsyncWriteStr(const char* const ptr_string);
SRV_enuErrorStatus_t HLCD_enuWriteChar(const uint8_t local_uint8Char);
SRV_enuErrorStatus_t HLCD_enuSendCommand(const uint8_t local_uint8Command);

#endif