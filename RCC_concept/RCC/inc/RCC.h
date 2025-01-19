#ifndef RCC_H
#define RCC_H


// /* This function takes system_clock_x and selects it as system clock */
// RCC_selectSystemClock (uint32_t clock);



// /* This function takes pointer to hold the clock status as SWS_X_SELECTED*/
// RCC_getSystemClock(uint32_t *clockStatus);

// /* This function shall return system frequency */
// RCC_getSystemFrequency (uint32_t * systemFreq);

// /* This function takes set_x_status and sets its state Enabled or Disabled, it takes state_x */
// RCC_setClockStatus (uint32_t clock, uint32_t state);

// /* This functuin selects MCO clock , it takes MCO_x_Clk and returns Ok/Nok */
// RCC_selectMCO_Clock (uint32_t clock);

// /* This function enables/disables CSS clock and takes state_x*/
// RCC_setClockSecurity_system (uint32_t state);

// /* This function selects pll source clock as PLL_SRC_x */
// RCC_selectPLL_Source (uint32_t pll_src);

// /* This function enables /disables HSE divider for PLL entry, it takes PLLXTPRE_HSE_Clk_x */
// RCC_setPLL_HSE_Divider (uint32_t PLLXTPRE);

// /* This function takes value of PLL MUL as PLL_MUL_x */
// RCC_setPLL_Multiplication (uint32_t mul_value);

// /* This function takes value of HSI trimming */
// RCC_setHSI_Trimming (uint32_t trim_value);

// /* This function sets External high-speed clock bypass state as Enable or Disable, it takes state_x */
// RCC_setHSE_Bypass (uint32_t state);


// /* This function sets the division factor of AHB clock, it takes SCALER_AHB_x and return Ok/Nok */
// RCC_setAHB_Prescaler (uint32_t scale);

// /* This function sets the division factor of APB1 clock, it takes SCALER_APB1_x and return Ok/Nok */
// RCC_setAPB1_Prescaler (uint32_t scale);

// /* This function sets the division factor of APB2 clock, it takes SCALER_APB2_x and return Ok/Nok */
// RCC_setAPB2_Prescaler (uint32_t scale);

// /* This function selects the frequency of the clock to ADC, it takes SCALER_ADC_x and return Ok/Nok */
// RCC_setADC_Prescaler (uint32_t scale);


// /* This function selects if PLL clock is divided or not for UBS, it takes SCALER_USB_PLL_x and return Ok/Nok */
// RCC_setUBS_Prescaler (uint32_t scale);

// /* This function enables and disables peripherals on AHB Bus, it takes AHBENR_x and state_x */
// RCC_setAHB_PeripheralState (uint32_t peripheral, uint32_t state);

// /* This function enables and disables peripherals on APB2 Bus, it takes APB2ENR_x and state_x */
// RCC_setAPB2_PeripheralState (uint32_t peripheral, uint32_t state);


#endif