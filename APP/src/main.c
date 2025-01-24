#include "std_types.h"
#include "RCC.h"
#include "RCC_Reg.h"
#include "GPIO.h"



int main(void)
{
    MRCC_HSI_ENABLE();

    MRCC_enuSetSysClkSrc(MRCC_SYS_CLK_HSI);

    MRCC_ENABLE_GPIOA_CLK();
    MRCC_ENABLE_GPIOB_CLK();
    MRCC_ENABLE_GPIOC_CLK();

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
