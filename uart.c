#include "uart.h"
#include "systick.h"

// Internal Function Prototype
static uint32_t UART_GetPCLK(USART_TypeDef_t* USARTx);
static USART_Status_t USART_WaitFlag(USART_Handle_t* huart,uint32_t flag, uint32_t tickstart, uint32_t timeout);

/*======================================================================================================================*/
USART_Status_t USART_Init(USART_Handle_t* huart){
    /* 0. Checking */
    if (huart == NULL || huart->Instance == NULL) return USART_ERROR;
    if (huart->Baudrate == 0)                     return USART_ERROR;

    USART_TypeDef_t* USARTx = huart->Instance;

    /* 1. Disable USART */
    USARTx->CR1 &= ~USART_CR1_UE;

    /* 2. Configure Word Length */
    USARTx->CR1 &= ~USART_CR1_M;
    if(huart->WordLength == USART_WORDLENGTH_9B)
        USARTx->CR1 |= USART_CR1_M;

    /* 3. Configure Stop Bits */
    USARTx->CR2 &= ~USART_CR2_STOP_Msk;
    USARTx->CR2 |= ((huart->StopBits << USART_CR2_STOP_Pos) & USART_CR2_STOP_Msk);

    /* 4. Configure Parity */
    USARTx->CR1 &= ~USART_CR1_PCE;
    USARTx->CR1 &= ~USART_CR1_PS;

    if(huart->Parity != USART_PARITY_NONE)
    {
        USARTx->CR1 |= USART_CR1_PCE;

        if(huart->Parity == USART_PARITY_ODD)
            USARTx->CR1 |= USART_CR1_PS;
    }

    /* 5. Configure Baudrate */
    USARTx->CR1 &= ~USART_CR1_OVER8;
    if(huart->OverSampling == USART_OVERSAMPLING_8){
        USARTx->CR1 |= USART_CR1_OVER8;
    }

    uint32_t pclk = UART_GetPCLK(USARTx);
    uint32_t divider, mantissa, fraction;
    if(USARTx->CR1 & USART_CR1_OVER8){
        divider = 8 * huart->Baudrate;
        mantissa = pclk / divider;
        fraction = (uint32_t)(((uint64_t)(pclk % divider) * 8 + (divider / 2)) / divider);
        // Fraction can be overflow
        if(fraction >= 8){ 
            mantissa++; fraction = 0;
        }
        // OVER8 = 1 -> Fraction[3] = 0
        USARTx->BRR = (mantissa << 4) | (fraction & 0x07);
    }
    else{
        divider = 16 * huart->Baudrate;
        mantissa = pclk / divider;
        fraction = (uint32_t)(((uint64_t)(pclk % divider) * 16  + (divider / 2)) / divider);
        if(fraction >= 16){ 
            mantissa++; fraction = 0; 
        }
        USARTx->BRR = (mantissa << 4) | (fraction & 0x0F);
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
    uint32_t tickstart;
    uint32_t data_mask;

    if(huart == NULL || huart->Instance == NULL) return USART_ERROR;
    if(pData == NULL || size == 0)               return USART_ERROR;
    if(huart->TxState != USART_STATE_READY)      return USART_BUSY;

    USART_TypeDef_t* USARTx = huart->Instance;
    huart->TxState = USART_STATE_BUSY_TX;

    if (huart->Parity == USART_PARITY_NONE)
        data_mask = (huart->WordLength == USART_WORDLENGTH_9B) ? 0x01FFU : 0x00FFU;
    else
        data_mask = (huart->WordLength == USART_WORDLENGTH_9B) ? 0x00FFU : 0x007FU;

    if ((huart->WordLength == USART_WORDLENGTH_9B) && (huart->Parity == USART_PARITY_NONE)){
        uint16_t *pdata16bits = (uint16_t*)pData;
        while (size > 0U){
            tickstart = SysTick_GetTick();
            if (USART_WaitFlag(huart, USART_SR_TXE, tickstart, timeout) != USART_OK) goto timeout_error;
            uint16_t val;
            memcpy(&val, pdata16bits, sizeof(val));
            USARTx->DR = val & data_mask;
            pdata16bits++;
            size--;
        }
    }
    else{
        uint8_t *pdata8bits = pData;
        while (size > 0U){
            tickstart = SysTick_GetTick();
            if (USART_WaitFlag(huart, USART_SR_TXE, tickstart, timeout) != USART_OK) goto timeout_error;
            USARTx->DR = *pdata8bits & data_mask;
            pdata8bits++;
            size--;
        }
    }

    tickstart = SysTick_GetTick();
    if (USART_WaitFlag(huart, USART_SR_TC, tickstart, timeout) != USART_OK) goto timeout_error;

    huart->TxState = USART_STATE_READY;
    return USART_OK;

timeout_error:
    huart->TxState = USART_STATE_READY;
    return USART_TIMEOUT;
}


USART_Status_t USART_Receive(USART_Handle_t* huart, uint8_t* pData, uint16_t size, uint32_t timeout){
    uint32_t tickstart;
    uint32_t data_mask;

    if (huart == NULL || huart->Instance == NULL) return USART_ERROR;
    if (pData == NULL || size == 0U)              return USART_ERROR;
    if (huart->RxState != USART_STATE_READY)      return USART_BUSY;

    USART_TypeDef_t* USARTx = huart->Instance;
    huart->RxState = USART_STATE_BUSY_RX;

    if (huart->Parity == USART_PARITY_NONE)
        data_mask = (huart->WordLength == USART_WORDLENGTH_9B) ? 0x01FFU : 0x00FFU;
    else
        data_mask = (huart->WordLength == USART_WORDLENGTH_9B) ? 0x00FFU : 0x007FU;

    if ((huart->WordLength == USART_WORDLENGTH_9B) && (huart->Parity == USART_PARITY_NONE)) {
        uint16_t *pdata16bits = (uint16_t*)pData;
        while (size > 0U) {
            tickstart = SysTick_GetTick();
            if (USART_WaitFlag(huart, USART_SR_RXNE, tickstart, timeout) != USART_OK) goto timeout_error;
            uint16_t val = (uint16_t)(USARTx->DR & data_mask);
            memcpy(pdata16bits, &val, sizeof(val));
            pdata16bits++;
            size--;
        }
    }
    else {
        uint8_t *pdata8bits = pData;
        while (size > 0U) {
            tickstart = SysTick_GetTick();
            if (USART_WaitFlag(huart, USART_SR_RXNE, tickstart, timeout) != USART_OK) goto timeout_error;
            *pdata8bits = (uint8_t)(USARTx->DR & data_mask);
            pdata8bits++;
            size--;
        }
    }

    huart->RxState = USART_STATE_READY;
    return USART_OK;

timeout_error:
    huart->RxState = USART_STATE_READY;
    return USART_TIMEOUT;
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
    USART_TypeDef_t* USARTx = huart->Instance;

    while((USARTx->SR & flag) == 0U){
        if(USARTx->SR & (USART_SR_PE | USART_SR_FE | USART_SR_NF | USART_SR_ORE)) return USART_ERROR;

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

