#ifndef GPIO_H
#define GPIO_H





// enable port clk
// set pin direction
SRV_enuErrorStatus_t MGPIO_enuSetPinDir();

// afsel 
SRV_enuErrorStatus_t MGPIO_enuAltFuncSel();

// digital || analog
SRV_enuErrorStatus_t MGPIO_enuAnalogEnable();

// speed
SRV_enuErrorStatus_t MGPIO_enuOutputType();

SRV_enuErrorStatus_t MGPIO_enuOutputSpeed();

// pu || pd
SRV_enuErrorStatus_t MGPIO_enuPinResConfig();

// write
SRV_enuErrorStatus_t MGPIO_enuSetPinVal();

// read
SRV_enuErrorStatus_t MGPIO_enuGetPinVal();

#endif