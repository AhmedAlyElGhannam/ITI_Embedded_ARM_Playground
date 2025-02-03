#include "std_types.h"
#include "error_status.h"
#include "RCC.h"
#include "GPIO.h"
#include "NVIC.h"


int main(void)
{
    MRCC_structPLLConfig_t local_structPLLConfig = 
    {
        .M_Prescaler = 13,
        .N_Multiplier = 192,
        .P_Prescaler = 0b10,
        .Q_Prescaler = 10,
        .CLK_Src = MRCC_PLL_SRC_HSI,
    };

    MRCC_structBusClkPrescaler_t local_structBusClkPrescalers = 
    {
        .AHB_Prescaler = MRCC_AHB_DIV_BY_1,
        .APB_LowSpeedPrescaler = MRCC_APB_DIV_BY_2,
        .APB_HighSpeedPrescaler = MRCC_APB_DIV_BY_1,
    };

    // MRCC_enuSetClkSrcState(MRCC_HSE_CLK, MRCC_CLK_SRC_ENABLE);
    // MRCC_enuSetSysClkSrc(MRCC_SYS_CLK_HSE);
    // MRCC_enuSetClkSrcState(MRCC_HSI_CLK, MRCC_CLK_SRC_DISABLE);
    MRCC_enuSetBusClkPrescaler(&local_structBusClkPrescalers);
    MRCC_enuConfigPLL(&local_structPLLConfig);
    MRCC_enuSetSysClkSrc(MRCC_SYS_CLK_PLL);
    /*
    MRCC_enuSetClkSrcState(MRCC_HSI_CLK, MRCC_CLK_SRC_ENABLE);
    MRCC_enuSetSysClkSrc(MRCC_SYS_CLK_HSI);*/

    MRCC_enuSetPeripheralClkState(MRCC_AHB1_GPIOA | MRCC_AHB1_GPIOB | MRCC_AHB1_GPIOC, MRCC_PERIPHERAL_CLK_ENABLE);

    MGPIO_enuSetPinMode(GPIOA, PIN05, GPOUT);
    MGPIO_enuOutputSpeed(GPIOA, PIN05, VHIGH_SPEED);
    
    while (true)
    {
        MGPIO_enuSetPinVal(GPIOA, PIN05, PIN_HIGH);
        for (volatile uint32_t i = 0; i < 800000; i++);
        MGPIO_enuSetPinVal(GPIOA, PIN05, PIN_LOW);
        for (volatile uint32_t i = 0; i < 800000; i++);
    }
    return 0;
}
