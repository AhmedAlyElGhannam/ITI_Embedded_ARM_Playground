#ifndef RCC_PBCFG_H
#define RCC_PBCFG_H


#define MRCC_APB_DIV_BY_2       (0b100UL)       
#define MRCC_APB_DIV_BY_4       (0b101UL)
#define MRCC_APB_DIV_BY_8       (0b110UL)
#define MRCC_APB_DIV_BY_16      (0b111UL)

#define MRCC_AHB_DIV_BY_2           (0b1000UL)
#define MRCC_AHB_DIV_BY_4           (0b1001UL)
#define MRCC_AHB_DIV_BY_8           (0b1010UL)
#define MRCC_AHB_DIV_BY_16          (0b1011UL)
#define MRCC_AHB_DIV_BY_64          (0b1100UL)
#define MRCC_AHB_DIV_BY_128         (0b1101UL)
#define MRCC_AHB_DIV_BY_256         (0b1110UL)
#define MRCC_AHB_DIV_BY_512         (0b1111UL)

#define MRCC_PLL_M_PRESCALER    (0b111111) /** 0b000010 <= /M <= 0b111111 */
#define MRCC_PLL_N_PRESCALER    (0b111111111) /** 0b000000010 <= *N <= 0b111111111*/
#define MRCC_PLL_P_PRESCALER    (0b11) /** /P = 0b00 -> 2, 0b01 -> 4, 0b10 -> 6, 0b11 -> 8 */
#define MRCC_PLL_Q_PRESCALER    (0b1111) /** 0b0010 <= /Q <= 0b1111 */
#define MRCC_PLL_SRC            (0b1) /** PLLSRC = 0b0 -> HSI, 0b1 -> HSE */

#define MRCC_AHB1_PRESCALER     (MRCC_AHB_DIV_BY_512)
#define MRCC_APB1_PRESCALER     (MRCC_APB_DIV_BY_16)
#define MRCC_APB2_PRESCALER     (MRCC_APB_DIV_BY_16)


#define RCC_HSI_CLK_HZ           16000000
#define RCC_HSE_CLK_HZ           25000000

#endif