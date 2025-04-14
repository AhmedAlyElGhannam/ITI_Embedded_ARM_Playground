/**
 * @file RCC.h
 * @brief This file contains the definitions and configurations for the Reset and Clock Control (RCC) module.
 */

 #ifndef RCC_H
 #define RCC_H
 
 #include "std_types.h"
 #include "error_status.h"
 
 /**
  * @defgroup RCC_AHB1_Bus AHB1 Bus Definitions
  * @{
  */
 #define MRCC_AHB1_BUS    (0xFF9FEF70UL)
 #define MRCC_AHB1_GPIOA  (0xFF9FEF7000000001ULL)
 #define MRCC_AHB1_GPIOB  (0xFF9FEF7000000002ULL)
 #define MRCC_AHB1_GPIOC  (0xFF9FEF7000000004ULL)
 #define MRCC_AHB1_GPIOD  (0xFF9FEF7000000008ULL)
 #define MRCC_AHB1_GPIOE  (0xFF9FEF7000000010ULL)
 #define MRCC_AHB1_GPIOH  (0xFF9FEF7000000080ULL)
 #define MRCC_AHB1_CRC    (0xFF9FEF7000001000ULL)
 #define MRCC_AHB1_DMA1   (0xFF9FEF7000200000ULL)
 #define MRCC_AHB1_DMA2   (0xFF9FEF7000400000ULL)
 #define MRCC_AHB1_FLITF  (0xFF9FEF70FFFF7FFFULL) /* with LP only */
 #define MRCC_AHB1_SRAM1  (0xFF9FEF70FFFEFFFFULL) /* with LP only */
 /** @} */
 
 /**
  * @defgroup RCC_AHB2_Bus AHB2 Bus Definitions
  * @{
  */
 #define MRCC_AHB2_BUS       (0xFFFFFF7FUL)
 #define MRCC_AHB2_USBOTG    (0xFFFFFF7F00000080ULL)
 /** @} */
 
 /**
  * @defgroup RCC_APB1_Bus APB1 Bus Definitions
  * @{
  */
 #define MRCC_APB1_BUS       (0xEF1D37F0UL)
 #define MRCC_APB1_TIM2      (0xEF1D37F000000001ULL)
 #define MRCC_APB1_TIM3      (0xEF1D37F000000002ULL)
 #define MRCC_APB1_TIM4      (0xEF1D37F000000004ULL)
 #define MRCC_APB1_TIM5      (0xEF1D37F000000008ULL)
 #define MRCC_APB1_WWDG      (0xEF1D37F000000800ULL)
 #define MRCC_APB1_SPI2      (0xEF1D37F000004000ULL)
 #define MRCC_APB1_SPI3      (0xEF1D37F000008000ULL)
 #define MRCC_APB1_USART2    (0xEF1D37F000020000ULL)
 #define MRCC_APB1_I2C1      (0xEF1D37F000200000ULL)
 #define MRCC_APB1_I2C2      (0xEF1D37F000400000ULL)
 #define MRCC_APB1_I2C3      (0xEF1D37F000800000ULL)
 #define MRCC_APB1_PWR       (0xEF1D37F010000000ULL)
 /** @} */
 
 /**
  * @defgroup RCC_APB2_Bus APB2 Bus Definitions
  * @{
  */
 #define MRCC_APB2_BUS       (0xFFF886CEUL)
 #define MRCC_APB2_TIM1      (0xFFF886CE00000001ULL)
 #define MRCC_APB2_USART1    (0xFFF886CE00000010ULL)
 #define MRCC_APB2_USART6    (0xFFF886CE00000020ULL)
 #define MRCC_APB2_ADC1      (0xFFF886CE00000100ULL)
 #define MRCC_APB2_SDIO      (0xFFF886CE00000800ULL)
 #define MRCC_APB2_SPI1      (0xFFF886CE00001000ULL)
 #define MRCC_APB2_SPI4      (0xFFF886CE00002000ULL)
 #define MRCC_APB2_SYSCFG    (0xFFF886CE00004000ULL)
 #define MRCC_APB2_TIM9      (0xFFF886CE00010000ULL)
 #define MRCC_APB2_TIM10     (0xFFF886CE00020000ULL)
 #define MRCC_APB2_TIM11     (0xFFF886CE00040000ULL)
 /** @} */
 
 /**
  * @enum MRCC_enuHSEBypassConfig_t
  * @brief Enumeration for HSE Bypass Configuration.
  */
 typedef enum 
 {
     BYPASS_ENABLE = 0x00080000UL, /**< Enable HSE Bypass */
     BYPASS_DISABLE = 0x00000000UL, /**< Disable HSE Bypass */
 } MRCC_enuHSEBypassConfig_t;
 
 /**
  * @enum MRCC_enuCSSStateConfig_t
  * @brief Enumeration for Clock Security System (CSS) State Configuration.
  */
 typedef enum 
 {
     CSS_ENABLE = 0x80000ULL, /**< Enable CSS */
     CSS_DISABLE = 0x00000ULL, /**< Disable CSS */
 } MRCC_enuCSSStateConfig_t;
 
 /**
  * @enum MRCC_enuSysClkSrc_t
  * @brief Enumeration for System Clock Source.
  */
 typedef enum
 {
     MRCC_SYS_CLK_HSI = 0UL, /**< HSI Clock Source */
     MRCC_SYS_CLK_HSE = 1UL, /**< HSE Clock Source */
     MRCC_SYS_CLK_PLL = 2UL, /**< PLL Clock Source */
 } MRCC_enuSysClkSrc_t;
 
 /**
  * @enum MRCC_enuClkType_t
  * @brief Enumeration for Clock Type.
  */
 typedef enum
 {
     MRCC_HSI_CLK = 0U, /**< HSI Clock */
     MRCC_HSE_CLK = 16U, /**< HSE Clock */
     MRCC_PLL_CLK = 24U, /**< PLL Clock */
     MRCC_PLLI2S_CLK = 26U, /**< PLLI2S Clock */
 } MRCC_enuClkType_t;
 
 /**
  * @enum MRCC_enuLowSpeedClkType_t
  * @brief Enumeration for Low Speed Clock Type.
  */
 typedef enum 
 {
     LSI_CLK = 0U, /**< LSI Clock */
     LSE_CLK = 1U, /**< LSE Clock */
 } MRCC_enuLowSpeedClkType_t;
 
 /**
  * @enum MRCC_enuLowSpeedClkState_t
  * @brief Enumeration for Low Speed Clock State.
  */
 typedef enum 
 {
     LS_CLK_ENABLE = 1U, /**< Enable Low Speed Clock */
     LS_CLK_DISABLE = 0U, /**< Disable Low Speed Clock */
 } MRCC_enuLowSpeedClkState_t;
 
 /**
  * @enum MRCC_enuClkSrcState_t
  * @brief Enumeration for Clock Source State.
  */
 typedef enum
 {
     MRCC_CLK_SRC_DISABLE = 0U, /**< Disable Clock Source */
     MRCC_CLK_SRC_ENABLE = 1U, /**< Enable Clock Source */
 } MRCC_enuClkSrcState_t;
 
 /**
  * @enum MRCC_enuPeripheralClkState_t
  * @brief Enumeration for Peripheral Clock State.
  */
 typedef enum
 {
     MRCC_PERIPHERAL_CLK_DISABLE = 0U, /**< Disable Peripheral Clock */
     MRCC_PERIPHERAL_CLK_ENABLE = 1U, /**< Enable Peripheral Clock */
     MRCC_PERIPHERAL_CLK_RESET = 2U, /**< Reset Peripheral Clock */
     MRCC_PERIPHERAL_CLK_LP_EN = 3U, /**< Enable Low Power Peripheral Clock */
 } MRCC_enuPeripheralClkState_t;
 
 /**
  * @enum MRCC_enuPLLClkSource_t
  * @brief Enumeration for PLL Clock Source.
  */
 typedef enum
 {
     MRCC_PLL_SRC_HSI = 0U, /**< HSI Clock Source for PLL */
     MRCC_PLL_SRC_HSE = 1U, /**< HSE Clock Source for PLL */
 } MRCC_enuPLLClkSource_t;
 
 
 
 /**
  * @enum MRCC_enuI2SClkSource_t
  * @brief Enumeration for I2S Clock Source.
  */
 typedef enum 
 {
     PLLI2S_CLK = 0U, /**< PLLI2S Clock Source */
     I2S_CKIN = 1U, /**< I2S_CKIN Clock Source */
 } MRCC_enuI2SClkSource_t;
 
 /**
  * @struct MRCC_structPLLConfig_t
  * @brief Structure for PLL Configuration.
  */
 typedef struct 
 {
     uint32_t M_Prescaler : 6; /**< M Prescaler */
     uint32_t P_Prescaler : 2; /**< P Prescaler */
     uint32_t Q_Prescaler : 4; /**< Q Prescaler */
     uint32_t N_Multiplier : 9; /**< N Multiplier */
     uint32_t CLK_Src : 1; /**< Clock Source */
 } MRCC_structPLLConfig_t;
 
 /**
  * @enum MRCC_enuAHBPrescaler_t
  * @brief Enumeration for AHB Prescaler.
  */
 typedef enum 
 {
     MRCC_AHB_DIV_BY_1 = (0b0111U), /**< AHB Clock divided by 1 */
     MRCC_AHB_DIV_BY_2 = (0b1000U), /**< AHB Clock divided by 2 */
     MRCC_AHB_DIV_BY_4 = (0b1001U), /**< AHB Clock divided by 4 */
     MRCC_AHB_DIV_BY_8 = (0b1010U), /**< AHB Clock divided by 8 */
     MRCC_AHB_DIV_BY_16 = (0b1011U), /**< AHB Clock divided by 16 */
     MRCC_AHB_DIV_BY_64 = (0b1100U), /**< AHB Clock divided by 64 */
     MRCC_AHB_DIV_BY_128 = (0b1101U), /**< AHB Clock divided by 128 */
     MRCC_AHB_DIV_BY_256 = (0b1110U), /**< AHB Clock divided by 256 */
     MRCC_AHB_DIV_BY_512 = (0b1111U), /**< AHB Clock divided by 512 */
 } MRCC_enuAHBPrescaler_t;
 
 /**
  * @enum MRCC_enuAPBPrescaler_t
  * @brief Enumeration for APB Prescaler.
  */
 typedef enum 
 {
     MRCC_APB_DIV_BY_1 = (0b011U), /**< APB Clock divided by 1 */
     MRCC_APB_DIV_BY_2 = (0b100U), /**< APB Clock divided by 2 */
     MRCC_APB_DIV_BY_4 = (0b101U), /**< APB Clock divided by 4 */
     MRCC_APB_DIV_BY_8 = (0b110U), /**< APB Clock divided by 8 */
     MRCC_APB_DIV_BY_16 = (0b111U), /**< APB Clock divided by 16 */
 } MRCC_enuAPBPrescaler_t;
 
 /**
  * @enum MRCC_enuTimerClkPrescalerConfig_t
  * @brief Enumeration for Timer Clock Prescaler Configuration.
  */
 typedef enum 
 {
     TIMES_2 = 0x00000000UL, /**< Timer Clock multiplied by 2 */
     TIMES_4 = 0x01000000UL, /**< Timer Clock multiplied by 4 */
 } MRCC_enuTimerClkPrescalerConfig_t;
 
 /**
  * @struct MRCC_structBusClkPrescaler_t
  * @brief Structure for Bus Clock Prescaler.
  */
 typedef struct
 {
     uint8_t APB_LowSpeedPrescaler; /**< APB1 Low Speed Prescaler */
     uint8_t APB_HighSpeedPrescaler; /**< APB2 High Speed Prescaler */
     uint16_t AHB_Prescaler; /**< AHB Prescaler */
 } MRCC_structBusClkPrescaler_t;
 
 /**
  * @enum MRCC_enuClkReadyInterruptState_t
  * @brief Enumeration for Clock Ready Interrupt State.
  */
 typedef enum
 {
     MRCC_INT_RDY_ON = 1U, /**< Clock Ready Interrupt On */
     MRCC_INT_RDY_OFF = 0U, /**< Clock Ready Interrupt Off */
 } MRCC_enuClkReadyInterruptState_t;
 
 /**
  * @struct MRCC_structClkReadyInterrupt_t
  * @brief Structure for Clock Ready Interrupt.
  */
 typedef struct 
 {
     uint8_t LSI     : 1; /**< LSI Clock Ready Interrupt */
     uint8_t LSE     : 1; /**< LSE Clock Ready Interrupt */
     uint8_t HSI     : 1; /**< HSI Clock Ready Interrupt */
     uint8_t HSE     : 1; /**< HSE Clock Ready Interrupt */
     uint8_t PLL     : 1; /**< PLL Clock Ready Interrupt */
     uint8_t PLLI2S  : 1; /**< PLLI2S Clock Ready Interrupt */
 } MRCC_structClkReadyInterrupt_t;
 
 /**
  * @struct MRCC_structClkResetFlags_t
  * @brief Structure for Clock Reset Flags.
  */
 typedef struct 
 {
     uint32_t LPWRRSTF   : 1; /**< Low Power Reset Flag */
     uint32_t WWDGRSTF   : 1; /**< Window Watchdog Reset Flag */
     uint32_t IWDGRSTF   : 1; /**< Independent Watchdog Reset Flag */
     uint32_t SFTRSTF    : 1; /**< Software Reset Flag */
     uint32_t PORRSTF    : 1; /**< Power On Reset Flag */
     uint32_t PINRSTF    : 1; /**< Pin Reset Flag */
     uint32_t BORRSTF    : 1; /**< Brown Out Reset Flag */
 } MRCC_structClkResetFlags_t;
 
 /**
  * @struct MRCC_structClkReadyInterruptFlags_t
  * @brief Structure for Clock Ready Interrupt Flags.
  */
 typedef struct 
 {
     uint8_t LSIRDYF : 1; /**< LSI Ready Flag */
     uint8_t LSERDYF : 1; /**< LSE Ready Flag */
     uint8_t HSIRDYF : 1; /**< HSI Ready Flag */
     uint8_t HSERDYF : 1; /**< HSE Ready Flag */
     uint8_t PLLRDYF : 1; /**< PLL Ready Flag */
     uint8_t PLLI2SRDYF : 1; /**< PLLI2S Ready Flag */
 } MRCC_structClkReadyInterruptFlags_t;
 
 /**
  * @struct MRCC_structPLLI2CConfig_t
  * @brief Structure for PLLI2C Configuration.
  */
 typedef struct 
 {
     uint32_t N_Multiplier : 9; /**< N Multiplier */
     uint32_t R_Prescaler  : 3; /**< R Prescaler */
 } MRCC_structPLLI2CConfig_t;
 
 /**
  * @enum MRCC_enuMCOPrescaler_t
  * @brief Enumeration for MCO Prescaler.
  */
 typedef enum 
 {
     DIVIDE_BY_2 = 0b100, /**< Divide by 2 */
     DIVIDE_BY_3 = 0b101, /**< Divide by 3 */
     DIVIDE_BY_4 = 0b110, /**< Divide by 4 */
     DIVIDE_BY_5 = 0b111, /**< Divide by 5 */
 } MRCC_enuMCOPrescaler_t;
 
 /**
  * @enum MRCC_enuMCO2ClkSource_t
  * @brief Enumeration for MCO2 Clock Source.
  */
 typedef enum 
 {
     MCO2_SRC_SYS_CLK = 0b00, /**< System Clock Source */
     MCO2_SRC_PLLI2S_CLK = 0b01, /**< PLLI2S Clock Source */
     MCO2_SRC_HSE_CLK = 0b10, /**< HSE Clock Source */
     MCO2_SRC_PLL_CLK = 0b11, /**< PLL Clock Source */
 } MRCC_enuMCO2ClkSource_t;
 
 /**
  * @enum MRCC_enuMCO1ClkSource_t
  * @brief Enumeration for MCO1 Clock Source.
  */
 typedef enum 
 {
     MCO1_SRC_HSI_CLK = 0b00, /**< HSI Clock Source */
     MCO1_SRC_LSE_CLK = 0b01, /**< LSE Clock Source */
     MCO1_SRC_HSE_CLK = 0b10, /**< HSE Clock Source */
     MCO1_SRC_PLL_CLK = 0b11, /**< PLL Clock Source */
 } MRCC_enuMCO1ClkSource_t;
 
 /**
  * @struct MRCC_structMCOConfig_t
  * @brief Structure for MCO Configuration.
  */
 typedef struct 
 {
     uint32_t MCO1_Prescaler : 3; /**< MCO1 Prescaler */
     uint32_t MCO1_Source    : 2; /**< MCO1 Source */
     uint32_t MCO2_Prescaler : 3; /**< MCO2 Prescaler */
     uint32_t MCO2_Source    : 2; /**< MCO2 Source */
 } MRCC_structMCOConfig_t;
 
 /**
  * @enum MRCC_enumRTCStateConfig_t
  * @brief Enumeration for RTC State Configuration.
  */
 typedef enum 
 {
     RTC_ENABLE = 0x8000, /**< Enable RTC */
     RTC_DISABLE = 0x0000, /**< Disable RTC */
 } MRCC_enumRTCStateConfig_t;
 
 /**
  * @struct MRCC_structRTCConfig_t
  * @brief Structure for RTC Configuration.
  */
 typedef struct 
 {
     uint32_t RTC_Prescaler : 5; /**< RTC Prescaler */
     uint32_t RTC_ClkSource : 2; /**< RTC Clock Source */
     uint32_t RTCEN  : 16; /**< RTC Enable */
 } MRCC_structRTCConfig_t;
 
 /**
  * @enum MRCC_enuSpreadSelectConfig_t
  * @brief Enumeration for Spread Select Configuration.
  */
 typedef enum 
 {
     CENTER_SPREAD = 0U, /**< Center Spread */
     DOWN_SPREAD = 1U, /**< Down Spread */
 } MRCC_enuSpreadSelectConfig_t;
 
 /**
  * @enum 
  * @brief
  */
  typedef enum 
  {
     SYS_BUS_AHB1,
     SYS_BUS_AHB2,
     SYS_BUS_APB1,
     SYS_BUS_APB2,
  } MRCC_enuSystemBuses_t;
 
 /**
  * @struct MRCC_structSpeadSpectrumClkGenConfig_t
  * @brief Structure for Spread Spectrum Clock Generator Configuration.
  */
 typedef struct 
 {
     uint32_t SSCGEN : 1; /**< Spread Spectrum Clock Generator Enable */
     uint32_t SPREADSEL : 1; /**< Spread Select */
     uint32_t INCSTEP : 15; /**< Increment Step */
     uint32_t MODPER : 13; /**< Modulation Period */
 } MRCC_structSpeadSpectrumClkGenConfig_t;
 
 
 /**
  * @defgroup ClockSourceManagement Clock Source Management
  * @brief Functions to manage and configure clock sources.
  * @{
  */
 
 /**
  * @brief Sets the state of a specific clock source (enable/disable).
  * 
  * @param copy_enuClk Clock type to configure.
  * @param copy_enuClkSrcState New state of the clock source (enabled/disabled).
  * @return SRV_enuErrorStatus_t Error status.
  */
 extern SRV_enuErrorStatus_t MRCC_enuSetClkSrcState(MRCC_enuClkType_t copy_enuClk, MRCC_enuClkSrcState_t copy_enuClkSrcState);
 
 /**
  * @brief Sets the system clock source.
  * 
  * @param copy_enuSysClkSrc System clock source to select.
  * @return SRV_enuErrorStatus_t Error status.
  */
 extern SRV_enuErrorStatus_t MRCC_enuSetSysClkSrc(MRCC_enuSysClkSrc_t copy_enuSysClkSrc);
 
 /**
  * @brief Gets the current system clock source.
  * 
  * @param ptr_uint32SysClkSrc Pointer to store the current system clock source.
  * @return SRV_enuErrorStatus_t Error status.
  */
 extern SRV_enuErrorStatus_t MRCC_enuGetSysClkSrc(uint32_t* ptr_uint32SysClkSrc);
 
 /**
  * @}
  * 
  * @defgroup PeripheralClockManagement Peripheral Clock Management
  * @brief Functions to manage peripheral clock configurations.
  * @{
  */
 
 /**
  * @brief Sets the state of a peripheral clock (enable/disable).
  * 
  * @param copy_uint64Peripheral Peripheral to configure.
  * @param copy_enuPeripheralClkState New state of the peripheral clock.
  * @return SRV_enuErrorStatus_t Error status.
  */
 extern SRV_enuErrorStatus_t MRCC_enuSetPeripheralClkState(uint64_t copy_uint64Peripheral, MRCC_enuPeripheralClkState_t copy_enuPeripheralClkState);
 
 /**
  * @brief Configures the RTC clock state.
  * 
  * @param copy_enuRTCState RTC clock state configuration.
  * @return SRV_enuErrorStatus_t Error status.
  */
 extern SRV_enuErrorStatus_t MRCC_enuRTCStateConfig(MRCC_enumRTCStateConfig_t copy_enuRTCState);
 
 /**
  * @}
  * 
  * @defgroup PLLManagement PLL Management
  * @brief Functions to configure PLL settings.
  * @{
  */
 
 /**
  * @brief Configures the PLL settings.
  * 
  * @param ptr_structPLLConfig Pointer to the PLL configuration structure.
  * @return SRV_enuErrorStatus_t Error status.
  */
 extern SRV_enuErrorStatus_t MRCC_enuConfigPLL(MRCC_structPLLConfig_t* ptr_structPLLConfig);
 
 /**
  * @brief Configures the PLLI2S settings.
  * 
  * @param ptr_structPLLConfig Pointer to the PLLI2S configuration structure.
  * @return SRV_enuErrorStatus_t Error status.
  */
 extern SRV_enuErrorStatus_t MRCC_enuConfigPLLI2S(MRCC_structPLLI2CConfig_t* ptr_structPLLConfig);
 
 /**
  * @}
  * 
  * @defgroup PrescalerManagement Prescaler Management
  * @brief Functions to configure clock prescalers.
  * @{
  */
 
 /**
  * @brief Sets the clock prescaler values for system buses.
  * 
  * @param ptr_structBusClkPrescalers Pointer to the bus clock prescaler configuration structure.
  * @return SRV_enuErrorStatus_t Error status.
  */
 extern SRV_enuErrorStatus_t MRCC_enuSetBusClkPrescaler(MRCC_structBusClkPrescaler_t* ptr_structBusClkPrescalers);
 
 /**
  * @brief Sets the timer clock prescaler configuration.
  * 
  * @param copy_enuTimerClkPrescalerConfig Timer clock prescaler configuration.
  * @return SRV_enuErrorStatus_t Error status.
  */
 extern SRV_enuErrorStatus_t MRCC_enuSetTimerPrescaler(MRCC_enuTimerClkPrescalerConfig_t copy_enuTimerClkPrescalerConfig);
 
 /**
  * @}
  * 
  * @defgroup InterruptAndFlagManagement Interrupt and Flag Management
  * @brief Functions to configure interrupts and manage flags.
  * @{
  */
 
 /**
  * @brief Sets the state of clock ready interrupt.
  * 
  * @param ptr_structClkReadyInterrupt Pointer to the clock ready interrupt configuration structure.
  * @return SRV_enuErrorStatus_t Error status.
  */
 extern SRV_enuErrorStatus_t MRCC_enuSetClkReadyInterruptState(MRCC_structClkReadyInterrupt_t* ptr_structClkReadyInterrupt);
 
 /**
  * @brief Gets the raised clock reset flags.
  * 
  * @param ptr_structClkResetFlags Pointer to the structure to store the raised flags.
  * @return SRV_enuErrorStatus_t Error status.
  */
 extern SRV_enuErrorStatus_t MRCC_enuGetRaisedClkResetFlags(MRCC_structClkResetFlags_t* ptr_structClkResetFlags);
 
 /**
  * @brief Gets the raised clock ready interrupt flags.
  * 
  * @param ptr_structClkInterruptFlags Pointer to the structure to store the raised flags.
  * @return SRV_enuErrorStatus_t Error status.
  */
 extern SRV_enuErrorStatus_t MRCC_enuGetRaisedClkReadyInterruptFlags(MRCC_structClkReadyInterruptFlags_t* ptr_structClkInterruptFlags);
 
 /**
  * @brief Clears the CSS flag.
  */
 extern void MRCC_voidClrCSSFlag(void);
 
 /**
  * @brief Clears the PLLI2S flag.
  */
 extern void MRCC_voidClrPLLI2SFlag(void);
 
 /**
  * @brief Clears the PLL flag.
  */
 extern void MRCC_voidClrPLLFlag(void);
 
 /**
  * @brief Clears the HSE flag.
  */
 extern void MRCC_voidClrHSEFlag(void);
 
 /**
  * @brief Clears the HSI flag.
  */
 extern void MRCC_voidClrHSIFlag(void);
 
 /**
  * @brief Clears the LSE flag.
  */
 extern void MRCC_voidClrLSEFlag(void);
 
 /**
  * @brief Clears the LSI flag.
  */
 extern void MRCC_voidClrLSIFlag(void);
 
 /**
  * @brief Clears all reset flags.
  */
 extern void MRCC_voidClrResetFlags(void);
 
 /**
  * @brief Performs a backup domain reset.
  */
 extern void MRCC_voidPerformBackupDomainReset(void);
 
 /**
  * @}
  * 
  * @defgroup MiscellaneousConfigurations Miscellaneous Configurations
  * @brief Other configuration functions.
  * @{
  */
 
 /**
  * @brief Configures the HSE bypass state.
  * 
  * @param copy_enuConfig HSE bypass configuration. @see MRCC_enuHSEBypassConfig_t
  * @return SRV_enuErrorStatus_t Error status.
  */
 extern SRV_enuErrorStatus_t MRCC_enuHSEBypassConfig(MRCC_enuHSEBypassConfig_t copy_enuConfig);
 
 /**
  * @brief Configures the LSE bypass state.
  * 
  * @param copy_enuConfig LSE bypass configuration.
  * @return SRV_enuErrorStatus_t Error status.
  */
 extern SRV_enuErrorStatus_t MRCC_enuLSEBypassConfig(MRCC_enuHSEBypassConfig_t copy_enuConfig);
 
 /**
  * @brief Configures the Clock Security System (CSS) state.
  * 
  * @param copy_enuConfig CSS state configuration.
  * @return SRV_enuErrorStatus_t Error status.
  */
 extern SRV_enuErrorStatus_t MRCC_voidCSSStateConfig(MRCC_enuCSSStateConfig_t copy_enuConfig);
 
 /**
  * @brief Configures the MCO output.
  * 
  * @param ptr_structMCOConfig Pointer to the MCO configuration structure.
  * @return SRV_enuErrorStatus_t Error status.
  */
 extern SRV_enuErrorStatus_t MRCC_enuSetMCOConfig(MRCC_structMCOConfig_t* ptr_structMCOConfig);
 
 /**
  * @brief Configures the RTC clock.
  * 
  * @param ptr_structRTCConfig Pointer to the RTC configuration structure.
  * @return SRV_enuErrorStatus_t Error status.
  */
 extern SRV_enuErrorStatus_t MRCC_enuSetRTCConfig(MRCC_structRTCConfig_t* ptr_structRTCConfig);
 
 /**
  * @brief Configures the PLL spread spectrum settings.
  * 
  * @param ptr_structSpreadSpectrumConfig Pointer to the spread spectrum configuration structure.
  * @return SRV_enuErrorStatus_t Error status.
  */
 extern SRV_enuErrorStatus_t MRCC_enuSetPLLSpreadSpectrumConfig(MRCC_structSpeadSpectrumClkGenConfig_t* ptr_structSpreadSpectrumConfig);
 
 /**
  * @brief 
  * 
  * @param
  * @return
  */
 extern SRV_enuErrorStatus_t MRCC_enuGetBusFreq(MRCC_enuSystemBuses_t copy_enuBus, uint64_t* ptr_uint64BusClkFreq);
 
 
 /**
  * @}
  */
 
 
 
 
 
 #endif /* RCC_H */
