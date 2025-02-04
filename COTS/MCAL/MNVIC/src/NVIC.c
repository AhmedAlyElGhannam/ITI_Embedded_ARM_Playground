#include "std_types.h"
#include "error_status.h"
#include "NVIC_Reg.h"
#include "NVIC.h"

#define IS_NULL_PTR(PTR)            ((PTR) == NULL)
#define IS_INVALID_IRQ_NUM(IRQ)     ((IRQ) >= 240)
#define IS_INVALID_IRQ_PRI(PRI)     ((PRI) > 255)

SRV_enuErrorStatus_t MNVIC_enuEnableIRQ(uint8_t Copy_uint8IntNumber)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile NVIC_Registers_t* NVIC = (volatile NVIC_Registers_t*)NVIC_BASE_ADDRESS;

    if (IS_INVALID_IRQ_NUM(Copy_uint8IntNumber))
    {
        ret_enuErrorStatus = NVIC_INV_IRQ_NUM;
    }
    else
    {
        NVIC->ISER[Copy_uint8IntNumber / 32] = (1 << (Copy_uint8IntNumber % 32));
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MNVIC_enuDisableIRQ(uint8_t Copy_uint8IntNumber)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile NVIC_Registers_t* NVIC = (volatile NVIC_Registers_t*)NVIC_BASE_ADDRESS;

    if (IS_INVALID_IRQ_NUM(Copy_uint8IntNumber))
    {
        ret_enuErrorStatus = NVIC_INV_IRQ_NUM;
    }
    else
    {
        NVIC->ICER[Copy_uint8IntNumber / 32] = (1 << (Copy_uint8IntNumber % 32));
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MNVIC_enuSetPendingIRQ(uint8_t Copy_uint8IntNumber)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile NVIC_Registers_t* NVIC = (volatile NVIC_Registers_t*)NVIC_BASE_ADDRESS;

    if (IS_INVALID_IRQ_NUM(Copy_uint8IntNumber))
    {
        ret_enuErrorStatus = NVIC_INV_IRQ_NUM;
    }
    else
    {
        NVIC->ISPR[Copy_uint8IntNumber / 32] = (1 << (Copy_uint8IntNumber % 32));
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MNVIC_enuClrPendingIRQ(uint8_t Copy_uint8IntNumber)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile NVIC_Registers_t* NVIC = (volatile NVIC_Registers_t*)NVIC_BASE_ADDRESS;

    if (IS_INVALID_IRQ_NUM(Copy_uint8IntNumber))
    {
        ret_enuErrorStatus = NVIC_INV_IRQ_NUM;
    }
    else
    {
        NVIC->ICPR[Copy_uint8IntNumber / 32] = (1 << (Copy_uint8IntNumber % 32));
    }
    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MNVIC_enuGetPendingIRQ(uint8_t Copy_uint8IntNumber, uint8_t* Ptr_uint8Pending)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile NVIC_Registers_t* NVIC = (volatile NVIC_Registers_t*)NVIC_BASE_ADDRESS;
    if (IS_NULL_PTR(Ptr_uint8Pending))
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else if (IS_INVALID_IRQ_NUM(Copy_uint8IntNumber))
    {
        
        ret_enuErrorStatus = NVIC_INV_IRQ_NUM;
    }
    else
    {
        (*Ptr_uint8Pending) = (NVIC->ISPR[Copy_uint8IntNumber / 32] >> (Copy_uint8IntNumber % 32)) & 1U;
    }
    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MNVIC_enuSetIRQPriority(uint8_t Copy_uint8IntNumber, uint8_t Copy_uint8Priority)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile NVIC_Registers_t* NVIC = (volatile NVIC_Registers_t*)NVIC_BASE_ADDRESS;

    if (IS_INVALID_IRQ_NUM(Copy_uint8IntNumber))
    {
        ret_enuErrorStatus = NVIC_INV_IRQ_NUM;
    }
    else if (IS_INVALID_IRQ_PRI(Copy_uint8Priority))
    {
        ret_enuErrorStatus = NVIC_INV_IRQ_PRI;
    }
    else
    {
        NVIC->IPR[Copy_uint8IntNumber / 4] = (Copy_uint8Priority) << (Copy_uint8IntNumber / 60);

        // 0 -> 239
        // /4 ==>> 0 -> 59
        // /60 ==>> 0 -> 3
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MNVIC_enuGetIRQPriority(uint8_t Copy_uint8IntNumber, uint8_t* Ptr_uint8Priority)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile NVIC_Registers_t* NVIC = (volatile NVIC_Registers_t*)NVIC_BASE_ADDRESS;

    if (IS_NULL_PTR(Ptr_uint8Priority))
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else if (IS_INVALID_IRQ_NUM(Copy_uint8IntNumber))
    {
        ret_enuErrorStatus = NVIC_INV_IRQ_NUM;
    }
    else
    {
        (*Ptr_uint8Priority) = (NVIC->IPR[Copy_uint8IntNumber / 4] >> (Copy_uint8IntNumber / 60)) & 0xFFU;
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MNVIC_enuIsIRQActive(uint8_t Copy_uint8IntNumber, uint8_t* Ptr_uint8Active)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile NVIC_Registers_t* NVIC = (volatile NVIC_Registers_t*)NVIC_BASE_ADDRESS;

    if (IS_NULL_PTR(Ptr_uint8Active))
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else if (IS_INVALID_IRQ_NUM(Copy_uint8IntNumber))
    {
        ret_enuErrorStatus = NVIC_INV_IRQ_NUM;
    }
    else
    {
        (*Ptr_uint8Active) = (NVIC->IABR[Copy_uint8IntNumber / 32] >> (Copy_uint8IntNumber % 32)) & 1U;
    }

    return ret_enuErrorStatus;
}

SRV_enuErrorStatus_t MNVIC_enuSWTriggerIRQ(uint8_t Copy_uint8IntNumber)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    volatile NVIC_Registers_t* NVIC = (volatile NVIC_Registers_t*)NVIC_BASE_ADDRESS;

    if (IS_INVALID_IRQ_NUM(Copy_uint8IntNumber))
    {
        ret_enuErrorStatus = NVIC_INV_IRQ_NUM;
    }
    else
    {
        NVIC->STIR = Copy_uint8IntNumber;
    }

    return ret_enuErrorStatus;
}

void MNVIC_voidEnableAllIRQ(void)
{
    __asm volatile ("CPSIE i" : : : "memory");
}

void MNVIC_voidDisableAllIRQ(void)
{
    __asm volatile ("CPSID i" : : : "memory");
}
