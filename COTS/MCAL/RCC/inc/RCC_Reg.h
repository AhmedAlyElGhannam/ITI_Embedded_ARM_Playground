#ifndef RCC_REG_H
#define RCC_REG_H

#include "std_types.h"

typedef unsigned long int uint32_t;

// typedef struct
// {
//   volatile uint32_t CR;            
//   volatile uint32_t PLLCFGR;       
//   volatile uint32_t CFGR;          
//   volatile uint32_t CIR;           
//   volatile uint32_t AHB1RSTR;      
//   volatile uint32_t AHB2RSTR;      
//   volatile uint32_t AHB3RSTR;      
//            uint32_t RESERVED0;     
//   volatile uint32_t APB1RSTR;      
//   volatile uint32_t APB2RSTR;      
//            uint32_t RESERVED1[2];  
//   volatile uint32_t AHB1ENR;       
//   volatile uint32_t AHB2ENR;       
//   volatile uint32_t AHB3ENR;       
//            uint32_t RESERVED2;     
//   volatile uint32_t APB1ENR;       
//   volatile uint32_t APB2ENR;       
//            uint32_t RESERVED3[2];  
//   volatile uint32_t AHB1LPENR;     
//   volatile uint32_t AHB2LPENR;     
//   volatile uint32_t AHB3LPENR;     
//            uint32_t RESERVED4;     
//   volatile uint32_t APB1LPENR;     
//   volatile uint32_t APB2LPENR;     
//            uint32_t RESERVED5[2];  
//   volatile uint32_t BDCR;          
//   volatile uint32_t CSR;           
//            uint32_t RESERVED6[2];  
//   volatile uint32_t SSCGR;         
//   volatile uint32_t PLLI2SCFGR;    
// } RCC_Registers_t;

typedef struct
{
    volatile uint32_t CR;
    
    union 
    {
        volatile struct
        {
            const uint32_t reserved1          : 4;
            volatile uint32_t PLLQ            : 4;
            const uint32_t reserved2          : 1;
            volatile uint32_t PLLSRC          : 1;
            const uint32_t reserved2          : 4;
            volatile uint32_t PLLP            : 2;
            const uint32_t reserved3          : 1;
            volatile uint32_t PLLN            : 9;
            volatile uint32_t PLLM            : 6;
        }PLLCFGR __attribute__((packed)); 

        volatile uint32_t PLLCFGR_R; 
    };       

    volatile uint32_t CFGR;          
    volatile uint32_t CIR;           
    volatile uint32_t AHB1RSTR;      
    volatile uint32_t AHB2RSTR;      
             uint32_t RESERVED0;     
    volatile uint32_t APB1RSTR;      
    volatile uint32_t APB2RSTR;      
             uint32_t RESERVED1[2];  
    volatile uint32_t AHB1ENR;       
    volatile uint32_t AHB2ENR;       
    volatile uint32_t AHB3ENR;       
             uint32_t RESERVED2;     
    volatile uint32_t APB1ENR;       
    volatile uint32_t APB2ENR;       
             uint32_t RESERVED3[2];  
    volatile uint32_t AHB1LPENR;     
    volatile uint32_t AHB2LPENR;     
    volatile uint32_t AHB3LPENR;     
             uint32_t RESERVED4;     
    volatile uint32_t APB1LPENR;     
    volatile uint32_t APB2LPENR;     
             uint32_t RESERVED5[2];  
    volatile uint32_t BDCR;          
    volatile uint32_t CSR;           
             uint32_t RESERVED6[2];  
    volatile uint32_t SSCGR;         
    volatile uint32_t PLLI2SCFGR;    
} RCC_Registers_t;


#define RCC    ((volatile RCC_Registers_t*)(0x40023800))

#endif