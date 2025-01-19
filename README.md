# APIs


## HAL layer that collects all common drivers/MCAL (like RCC---usually done for RCC only) and initializes them (this is done for post-build config) (this is called a wrapper function)

## define masks for registers to enable/disable features (use #define)

## MASKS ARE VERY IMPORTANT => also it is way faster (start using them more)

## create a PLL function that takes constants directly instead of a function that takes the frequency to save the time to calculate them. Just use an external tool (like a python script) to find the constant values and plug them into the function directly.



```c

#define RCC_CTRL_HSI	0x01
#define RCC_CTRL_HSE	0x00010000
#define RCC_CTRL_PLL	0x01000000
#define RCC_CTRL_VAL	0xFEFEFFFE // add all previous masks then ~ them


typedef struct 
{
	clk,
	pll_cfg,
	peripheral_init
} RCC_config_t;


typedef struct
{
	uint32_t CR;
	uint32_t PLLCFG;
	.
	.
	.
	uint32_t reserved;
	.
	.
	uint32_t reserved2[2]; // or can be uint64_t reserved;
}RCC_reg_t;

#define RCC_Base_Address	(RCC_reg_t*)(0x40023800)
RCC_reg_t* reg_var = RCC_Base_Address;



/* this function is used for post build configuration by passing all configs as struct elements  */
RCC_cfgCLK(RCC_config_t)




/*
	CLK_TYPE: 0->HSI, 1->HSE, 2->PLL
	ctrl_option: 0->OFF, 1->ON
*/
RCC_controlCLK(uint8_t CLK_TYPE, uint8_t ctrl_option)
{

	RCC_reg_t* temp = (RCC_reg_t*) RCC_Base_Address;
	// validate clock name
	if (CLK_TYPE & RCC_CTRL_VAL)
	{
	
	}

	// action
	temp->CR &= ~CLK_TYPE;
	temp->CR |= CLK_TYPE;
}

/*
	
*/
RCC_controlHSI(uint8_t ctrl_option) 


RCC_selectSysCLK(CLK_TYPE)
{
	uint32_t temp;
	temp = CFGR;
	temp = 
}

typedef struct
{
	uint32_t M_VAL;
	uint32_t N_VAL;
	uint32_t P_VAL;
	uint32_t PLLSRC;
	uint32_t Q_VAL;
} PLL_CFG_t;

/*
	ALTERNATIVELY,
	you can use UNIONS *wink wink*
*/

RCC_Cfg_PLL(PLL_CFG_t* config)
{
	/*voila! it is easy!*/
}


#define AHP1	0
#define APB1	1

#define RCC_GPIO_A	1

#define RCC_TIMER2	1

/* cuz peripherals work with APB && AHB  */
/*passed argument is a mask for BOTH peripheral make (low DW) and used bus (high DW)  */
RCC_enablePeripheralCLK(uint64_t which_peripheral1 | which_peripheral2 | ...) // ORing all the peripheral masks to enable them with one function call
{

}


/* pre-build  */
RCC_init()
RCC_cfgPLL()
RCC_selectSysCLK()
RCC_enablePeri()
RCC_disablePeri()
```
