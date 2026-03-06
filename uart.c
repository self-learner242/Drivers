#include "uart.h"
#include "systick.h"

// Internal Function Prototype
static uint32_t UART_GetPCLK(USART_TypeDef_t* USARTx);
static USART_Status_t USART_WaitFlag(USART_Handle_t* huart,uint32_t flag, uint32_t tickstart, uint32_t timeout);

/*======================================================================================================================*/
USART_Status_t USART_Init(USART_Handle_t* huart){
    USART_TypeDef_t* USARTx = huart->Instance;

    /* 1. Disable USART */
    USARTx->CR1 &= ~USART_CR1_UE;

    /* 2. Configure Word Length */
    USARTx->CR1 &= ~USART_CR1_M;
    if(huart->WordLength == USART_WORDLENGTH_9B)
        USARTx->CR1 |= USART_CR1_M;

    /* 3. Configure Stop Bits */
    USARTx->CR2 &= ~(0x3U << 12);
    USARTx->CR2 |= (huart->StopBits << 12);

    /* 4. Configure Baudrate */
    uint32_t pclk = UART_GetPCLK(USARTx);
    uint32_t divider, mantissa, fraction;
    if(USARTx->CR1 & USART_CR1_OVER8){
        divider = 8 * huart->Baudrate;
        mantissa = pclk / divider;
        fraction = ((pclk % divider) * 8 + (divider / 2)) / divider;
        // OVER8 = 1 -> Fraction[3] = 0
        USARTx->BRR = (mantissa << 4) | (fraction & 0x07);
    }
    else{
        divider = 16 * huart->Baudrate;
        mantissa = pclk / divider;
        fraction = ((pclk % divider) * 16 + (divider / 2)) / divider;
        USARTx->BRR = (mantissa << 4) | (fraction & 0x0F);
    }

    /* 5. Configure Parity */
    USARTx->CR1 &= ~USART_CR1_PCE;
    USARTx->CR1 &= ~USART_CR1_PS;

    if(huart->Parity != USART_PARITY_NONE)
    {
        USARTx->CR1 |= USART_CR1_PCE;

        if(huart->Parity == USART_PARITY_ODD)
            USARTx->CR1 |= USART_CR1_PS;
    }

    /* 6. Enable Transmitter & Receiver */
    USARTx->CR1 |= USART_CR1_TE | USART_CR1_RE;

    /* 7. Enable USART */
    USARTx->CR1 |= USART_CR1_UE;

    /* 8. Tx Rx State*/
    huart->TxState = USART_STATE_READY;
    huart->RxState = USART_STATE_READY;

    return USART_OK;
}

/*-----------------------------------------------POLLING-------------------------------------------------------*/
USART_Status_t USART_Transmit(USART_Handle_t* huart, uint8_t* pData, uint16_t size, uint32_t timeout){
    uint8_t     *pdata8bits;
    uint16_t    *pdata16bits; // Handle 9-bit Data WordLength 
    uint32_t    tickstart = 0U;

    if(pData == NULL || size == 0)
        return USART_ERROR;

    if(huart->TxState != USART_STATE_READY)
        return USART_BUSY;

    huart->TxState = USART_STATE_BUSY_TX;

    tickstart = SysTick_GetTick();

    if((huart->WordLength == USART_WORDLENGTH_9B) && huart->Parity == USART_PARITY_NONE){
        // 9bit data none parity
        pdata8bits = NULL;
        pdata16bits =(uint16_t*) pData;
    }
    else{
        pdata8bits = pData;
        pdata16bits = NULL;
    }

    while(size > 0U){
        if(USART_WaitFlag(huart, USART_SR_TXE, tickstart,timeout) != USART_OK){
            huart->TxState = USART_STATE_READY;
            return USART_TIMEOUT;
        }

        if(pdata8bits == NULL){
            // 9bits, no parity
            huart->Instance->DR = *pdata16bits & 0x01FFU;
            pdata16bits++;
        } 
        else{
            if(huart->Parity == USART_PARITY_NONE){
                huart->Instance->DR = *pdata8bits & 0xFFU;
            }
            else{
                huart->Instance->DR = *pdata8bits & 0x7FU;
            }
            pdata8bits++;
        }
        size--;
    }

    if(USART_WaitFlag(huart, USART_SR_TC, tickstart, timeout) != USART_OK){
        huart->TxState = USART_STATE_READY;
        return USART_TIMEOUT;
    }

    huart->TxState = USART_STATE_READY;

    return USART_OK;
}

USART_Status_t USART_Receive(USART_Handle_t* huart, uint8_t* pData, uint16_t size, uint32_t timeout){

    uint8_t     *pdata8bits;
    uint16_t    *pdata16bits; // Handle 9-bit Data WordLength
    uint32_t    tickstart = 0U;

    if ((pData == NULL) || (size == 0U)) {
        return USART_ERROR;
    }

    /* Check Rx process is not ongoing*/
    if(huart->RxState != USART_STATE_READY){
        return USART_BUSY;
    }

    huart->RxState = USART_STATE_BUSY_RX;
    tickstart = SysTick_GetTick();

    if ((huart->WordLength == USART_WORDLENGTH_9B) && huart->Parity == USART_PARITY_NONE){
        pdata8bits = NULL;
        pdata16bits = (uint16_t*) pData;
    }
    else{
        pdata8bits = pData;
        pdata16bits = NULL;
    }

    while(size > 0U){
        if(USART_WaitFlag(huart, USART_SR_RXNE,tickstart, timeout) != USART_OK){
            huart->RxState = USART_STATE_READY;
            return USART_TIMEOUT;
        }

        if(pdata8bits == NULL){
            // 9bits, no parity
            *pdata16bits = (uint16_t)(huart->Instance->DR & 0x01FFU);
            pdata16bits++;
        } 
        else{
            if(huart->Parity == USART_PARITY_NONE)
                *pdata8bits = (uint8_t)(huart->Instance->DR & 0xFFU);
            else
                *pdata8bits = (uint8_t)(huart->Instance->DR & 0x7FU);

            pdata8bits++;
        }
        size--;
    }

    huart->RxState = USART_STATE_READY;
    return USART_OK;
}


/*-----------------------------------------------INTERRUPT-------------------------------------------------------*/
USART_Status_t USART_Transmit_IT(USART_Handle_t* huart, uint8_t* pData, uint16_t size){
    return USART_OK;
}

USART_Status_t USART_Receive_IT(USART_Handle_t* huart, uint8_t* pData, uint16_t size){
    return USART_OK;
}

/*----------------------------------------------Internal Function Implement---------------------------------------*/
static USART_Status_t USART_WaitFlag(USART_Handle_t* huart, uint32_t flag, uint32_t tickstart, uint32_t timeout){
    while((huart->Instance->SR & flag) == 0U){
        if (timeout != USART_MAX_TIMEOUT) {
            if ((SysTick_GetTick() - tickstart) >= timeout) {
                return USART_TIMEOUT;
            }
        }
    }
    return USART_OK;
}

// Temp GetPCLK. Phai viet tiep sau
static uint32_t UART_GetPCLK(USART_TypeDef_t* USARTx)
{
    if(USARTx == USART1 || USARTx == USART6)
        return 16000000;   // APB2
    else
        return 16000000;   // APB1
}

