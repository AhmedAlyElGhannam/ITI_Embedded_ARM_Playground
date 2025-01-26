#include "std_types.h"
#include "error_status.h"
#include "RCC.h"
#include "GPIO.h"



int main(void)
{
    // MRCC_HSI_ENABLE();

    MRCC_enuSetClkSrcState(MRCC_HSI_CLK, MRCC_CLK_SRC_ENABLE);
    MRCC_enuSetSysClkSrc(MRCC_SYS_CLK_HSI);
    MRCC_enuSetPeripheralClkState(MRCC_AHB1_GPIOA | MRCC_AHB1_GPIOB | MRCC_AHB1_GPIOC, MRCC_PERIPHERAL_CLK_ENABLE);

MGPIO_enuSetPinMode(GPIOC, PIN15, GPOUT);
    
    while (true)
    {
        MGPIO_enuSetPinVal(GPIOC, PIN15, PIN_HIGH);
        for (volatile uint32_t i = 0; i < 800000; i++);
        MGPIO_enuSetPinVal(GPIOC, PIN15, PIN_LOW);
        for (volatile uint32_t i = 0; i < 800000; i++);
    }
    return 0;
}
