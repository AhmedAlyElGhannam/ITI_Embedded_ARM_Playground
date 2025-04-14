#include "RCC.h"
#include "USART.h"
#include "DMA.h"
#include "HUSART_LCFG.h"
#include "HUSART.h"
#include "USART_Reg.h"

extern HUSART_structUsedModulesAndPortsAndPins_t HUSART_structHUSARTConfig;


static void HUSART_voidClksToggle(MRCC_enuPeripheralClkState_t copy_enuClkState)
{
    uint64_t local_uint64AHBMask = 0;
    uint64_t local_uint64APBMask = 0;

    /* set RCC clk mask for DMA depending on used module */
    switch (HUSART_structHUSARTConfig.DMAxModule)
    {
        case DMA1:
            local_uint64AHBMask |= MRCC_AHB1_DMA1;
        break;

        case DMA2:
            local_uint64AHBMask |= MRCC_AHB1_DMA2;
        break;
    
        default:
        break;
    }

    /* set RCC clk mask for Rx Port depending on used port */
    switch (HUSART_structHUSARTConfig.RxPort)
    {
        case GPIOA:
            local_uint64AHBMask |= MRCC_AHB1_GPIOA;
        break;

        case GPIOB:
            local_uint64AHBMask |= MRCC_AHB1_GPIOB;
        break;

        case GPIOC:
            local_uint64AHBMask |= MRCC_AHB1_GPIOC;
        break;
    }

    /* set RCC clk mask for Tx Port depending on used port */
    switch (HUSART_structHUSARTConfig.TxPort)
    {
        case GPIOA:
            local_uint64AHBMask |= MRCC_AHB1_GPIOA;
        break;

        case GPIOB:
            local_uint64AHBMask |= MRCC_AHB1_GPIOB;
        break;

        case GPIOC:
            local_uint64AHBMask |= MRCC_AHB1_GPIOC;
        break;
    }

    /* pass mask to activate their clk */
    MRCC_enuSetPeripheralClkState(local_uint64AHBMask, copy_enuClkState);

    /* set RCC clk mask for USART depending on used module */
    switch (HUSART_structHUSARTConfig.USARTxModule)
    {
        case USART1:
            local_uint64APBMask |= MRCC_APB2_USART1; 
        break;
    
        case USART2:
            local_uint64APBMask |= MRCC_APB1_USART2; 
        break;

        case USART6:
            local_uint64APBMask |= MRCC_APB2_USART6; 
        break;

    default:
        break;
    }

    /* pass mask to activate USART clk */
    MRCC_enuSetPeripheralClkState(local_uint64APBMask, MRCC_PERIPHERAL_CLK_ENABLE);

    return;
}

SRV_enuErrorStatus_t HUSART_enuInit(void)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;
    MDMA_enuDMAPeripheralRequest_t local_enuTxChannel;
    MDMA_enuDMAPeripheralRequest_t local_enuRxChannel;

    if ((HUSART_structHUSARTConfig.DMAxModule != DMA1) && (HUSART_structHUSARTConfig.DMAxModule != DMA2))
    {
        ret_enuErrorStatus = DMA_INV_MODULE;
    }
    else if ((HUSART_structHUSARTConfig.TxPort > GPIOC) || (HUSART_structHUSARTConfig.RxPort > GPIOC))
    {
        ret_enuErrorStatus = GPIO_INV_PORT;
    }
    else if ((HUSART_structHUSARTConfig.TxPin > PIN15) || (HUSART_structHUSARTConfig.TxPin > PIN15))
    {
        ret_enuErrorStatus = GPIO_INV_PIN;
    } 
    else if ((HUSART_structHUSARTConfig.USARTxModule != USART1) && (HUSART_structHUSARTConfig.USARTxModule != USART2) && (HUSART_structHUSARTConfig.USARTxModule != USART6))
    {
        ret_enuErrorStatus = USART_INV_MODULE;
    }
    else if ((HUSART_structHUSARTConfig.TxStream > DMA_STREAM7) || (HUSART_structHUSARTConfig.RxStream > DMA_STREAM7)) 
    {
        ret_enuErrorStatus = DMA_INV_STREAM;
    }
    else 
    {
        /* variable to get DR address of USART */
        volatile MUSART_Registers_t* USARTx = (volatile MUSART_Registers_t*)USART_GET_MODULE_BASE_ADDRESS(HUSART_structHUSARTConfig.USARTxModule);

        /* RCC: initialize GPIO && DMA && USART clks */
        HUSART_voidClksToggle(MRCC_PERIPHERAL_CLK_ENABLE);

        /* GPIO: set mode to AFSEL */
        MGPIO_enuSetPinMode(HUSART_structHUSARTConfig.TxPort, HUSART_structHUSARTConfig.TxPin, ALTFNC);
        MGPIO_enuSetPinMode(HUSART_structHUSARTConfig.RxPort, HUSART_structHUSARTConfig.RxPin, ALTFNC);

        /* GPIO: set GPIO pin alt function depending on USART module used */
        MGPIO_enuAltFuncSel(HUSART_structHUSARTConfig.TxPort, HUSART_structHUSARTConfig.TxPin, HUSART_structHUSARTConfig.USARTxModule == USART6 ? ALT_FUNC08 : ALT_FUNC07);
        MGPIO_enuAltFuncSel(HUSART_structHUSARTConfig.RxPort, HUSART_structHUSARTConfig.RxPin, HUSART_structHUSARTConfig.USARTxModule == USART6 ? ALT_FUNC08 : ALT_FUNC07);

        /* DMA: disable Tx && Rx streams */
        MDMA_enuStreamDisable(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.TxStream);
        MDMA_enuStreamDisable(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.RxStream);

        /* DMA: select stream channel based on used USART module */
        switch (HUSART_structHUSARTConfig.USARTxModule)
        {
            case USART1:            
                local_enuTxChannel = DMA2_STREAM7_CHANNEL4_USART1_TX;

                if (HUSART_structHUSARTConfig.RxStream == DMA_STREAM2)
                    local_enuRxChannel = DMA2_STREAM2_CHANNEL4_USART1_RX;
                else if (HUSART_structHUSARTConfig.RxStream == DMA_STREAM5)
                    local_enuRxChannel = DMA2_STREAM5_CHANNEL4_USART1_RX;
                else {}
            break;

            case USART2:
                local_enuTxChannel = DMA1_STREAM6_CHANNEL4_USART2_TX;
                local_enuRxChannel = DMA1_STREAM5_CHANNEL4_USART2_RX;
            break;

            case USART6:
                if (HUSART_structHUSARTConfig.TxStream == DMA_STREAM6)
                local_enuTxChannel = DMA2_STREAM6_CHANNEL5_USART6_TX;
                else if (HUSART_structHUSARTConfig.TxStream == DMA_STREAM7)
                    local_enuTxChannel = DMA2_STREAM7_CHANNEL5_USART6_TX;
                else {}

                if (HUSART_structHUSARTConfig.RxStream == DMA_STREAM1)
                local_enuRxChannel = DMA2_STREAM1_CHANNEL5_USART6_RX;
                else if (HUSART_structHUSARTConfig.RxStream == DMA_STREAM2)
                    local_enuRxChannel = DMA2_STREAM2_CHANNEL5_USART6_RX;
                else {}                
            break;
        }
        MDMA_enuStreamSetChannelPeripheralRequest(local_enuTxChannel);
        MDMA_enuStreamSetChannelPeripheralRequest(local_enuRxChannel);

        /* DMA: set Tx stream from mem-to-peripheral */
        MDMA_enuStreamSetDataTransferDir(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.TxStream, MEM_TO_PERIPHERAL);

        /* DMA: set Rx stream from peripheral-to-mem */
        MDMA_enuStreamSetDataTransferDir(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.RxStream, PERIPHERAL_TO_MEM);

        /* DMA: set inc mode for Tx stream source */
        MDMA_enuStreamMemIncMode(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.TxStream, MINC_ACCORDING_TO_MSIZE);
        MDMA_enuStreamPeripheralIncMode(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.TxStream, PINC_FIXED);

        /* DMA: set data size for Tx stream source */
        // MDMA_enuStreamPeripheralDataSize()
        MDMA_enuStreamMemDataSize(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.TxStream, MSIZE_BYTE);

        /* DMA: set inc mode for Rx stream source */
        MDMA_enuStreamMemIncMode(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.RxStream, MINC_ACCORDING_TO_MSIZE);
        MDMA_enuStreamPeripheralIncMode(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.RxStream, PINC_FIXED);

        /* DMA: set data size for Rx stream source */
        // MDMA_enuStreamPeripheralDataSize()
        MDMA_enuStreamMemDataSize(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.RxStream, MSIZE_BYTE);

        /* DMA: set Tx stream destination address as USART DR */
        MDMA_enuStreamSetPeripheralAddress(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.TxStream, &(USARTx->DR));

        /* DMA: set Rx stream destination address as USART DR */
        MDMA_enuStreamSetPeripheralAddress(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.RxStream, &(USARTx->DR));

        /* DMA: set flow controller to USART for Tx && Rx streams */
        MDMA_enuStreamPeripheralFlowControllerConfig(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.TxStream, FLOW_CONTROLLER_IS_PER);
        MDMA_enuStreamPeripheralFlowControllerConfig(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.RxStream, FLOW_CONTROLLER_IS_PER);

        /* USART: set baud rate */
        MUSART_enuSetBaudRate(HUSART_structHUSARTConfig.USARTxModule, BAUD_RATE_115200);

        /* USART: enable DMA mode */
        MUSART_enuEnableUSARTMode(HUSART_structHUSARTConfig.USARTxModule, MUSART_DMA_MODE);

        /* USART: set frame settings */
        // MUSART_enuConfigUSARTFrameSettings

        /* USART: enable Tx && Rx */
        MUSART_enuToggleTxOnOff(HUSART_structHUSARTConfig.USARTxModule, TX_EN);
        MUSART_enuToggleRxOnOff(HUSART_structHUSARTConfig.USARTxModule, RX_EN);
    }

    return ret_enuErrorStatus;
} 

SRV_enuErrorStatus_t HUSART_enuSendBufferConfig(uint8_t* ptr_uint8Buffer, uint16_t copy_uint16BufferSize)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (ptr_uint8Buffer == NULL)
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else if (copy_uint16BufferSize < 2) /* must be at least 1 + null */
    {
        ret_enuErrorStatus = HUSART_INV_BUF_SIZE;
    }
    else 
    {
        /* DMA: set Tx source address */
        MDMA_enuStreamSetMem0Address(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.TxStream, ptr_uint8Buffer);

        /* DMA: set number of items (-1 accounting for NULL)*/
        MDMA_enuStreamSetNumOfDataItemsToTransfer(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.TxStream, copy_uint16BufferSize);
    }
    
    return ret_enuErrorStatus;    
}

SRV_enuErrorStatus_t HUSART_enuReceiveBufferConfig(uint8_t* ptr_uint8Buffer, uint16_t copy_uint16BufferSize)
{
    SRV_enuErrorStatus_t ret_enuErrorStatus = ALL_OK;

    if (ptr_uint8Buffer == NULL)
    {
        ret_enuErrorStatus = NULL_PTR;
    }
    else if (copy_uint16BufferSize < 2) /* must be at least 1 + null */
    {
        ret_enuErrorStatus = HUSART_INV_BUF_SIZE;
    }
    else 
    {
        /* DMA: set Rx destination address */
        MDMA_enuStreamSetMem0Address(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.RxStream, ptr_uint8Buffer);

        /* DMA: set number of items */
        MDMA_enuStreamSetNumOfDataItemsToTransfer(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.RxStream, copy_uint16BufferSize);
    }
    
    return ret_enuErrorStatus;    
}

void HUSART_voidStartTransferOperations(void)
{
    /* USART: enable USART module */
    MUSART_enuToggleUSARTOnOff(HUSART_structHUSARTConfig.USARTxModule, USART_ON);

    /* DMA: enable Tx stream */
    MDMA_enuStreamEnable(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.TxStream);

    /* DMA: enable Rx stream */
    MDMA_enuStreamEnable(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.RxStream);
}

SRV_enuErrorStatus_t HUSART_enuDeInit(void)
{
    /* DMA: disable Tx && Rx streams */
    MDMA_enuStreamDisable(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.TxStream);
    MDMA_enuStreamDisable(HUSART_structHUSARTConfig.DMAxModule, HUSART_structHUSARTConfig.RxStream);

    /* USART: disable USART */
    MUSART_enuToggleUSARTOnOff(HUSART_structHUSARTConfig.USARTxModule, USART_OFF);

    /* RCC: disable clk of GPIO && DMA && USART */
    HUSART_voidClksToggle(MRCC_PERIPHERAL_CLK_DISABLE);
}














    /*GPIO*/
    /*AFSEL for Tx && Rx Pins for GPIO (AF07 for USART1/2 && AF08 for USART6) */
    // set mode THEN set AFSEL
    /* apparently you can set ANY GPIO pin of any port as Tx && Rx! */
    /*GPIO*/

    /*DMA (Tx)*/
    // disable stream
    // select channel based on used USART
    // set inc mode of source to 1byte
    // set inc mode of dest to no inc (keep writing to USART DR)
    // set mode to mem-to-peripheral
    // set PSIZE && MSIZE
    // set destination address as USART DR
    /*DMA (Tx)*/

    /*DMA (Rx)*/
    // disable stream
    // select channel based on used USART
    // set inc mode of source to no inc (keep reading from USART DR)
    // set inc mode of dest to 1 byte 
    // set mode to peripheral-to-mem
    // set PSIZE && MSIZE
    // set src address as USART DR
    /*DMA (Rx)*/

    /*USART*/
    // MUSART_enuSetBaudRate
    // MUSART_enuEnableUSARTMode
    // MUSART_enuConfigUSARTFrameSettings
    // no need to activate any of USART's interrupts
    // enable Tx
    // enable Rx
    // enable USART
    /*USART*/

/***********************************************/
    
    // will leave these comments for now just in case I needed to look into them again

    /* enable USARTx + DMA + GPIO clk */
    /* Tx: disable stream */
    /* Rx: disable stream */
    /* disable USART */
    /* disable Tx */
    /* disable Rx */

    
    /*********DMA*********/

    /* Tx: select channel based on used USART */
    /* Rx: select channel based on used USART */

    /* Tx: destination increment mode */
    /* Rx: destination increment mode */

    /* Tx: source increment mode (PER) */
    /* Rx: source increment mode (PER) */

    /* Tx: MSIZE = word (src) */
    /* Rx: MSIZE = word (src) */

    /* Tx: PSIZE = word (dest) */
    /* Rx: PSIZE = word (dest) */

    /* Tx: mem-to-per mode */
    /* Rx: per-to-mem mode */

    /* MDMA_enuRegisterCBF */

    /*********DMA*********/

    /*********USART*********/

    /*  */
    /*  */
    /* MUSART_enuEnableUSARTInt */
    /*  */

    /*********USART*********/
/***************************************************************************/