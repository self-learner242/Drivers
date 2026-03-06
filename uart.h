#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdlib.h>
/*--------------Base Address---------------*/
#define USART1_BASE     0x40011000
#define USART2_BASE     0x40004400
#define USART3_BASE     0x40004800
#define USART6_BASE     0x40011400
/*--------------USARTx---------------------*/
#define USART1      ((USART_TypeDef_t*)USART1_BASE)
#define USART2      ((USART_TypeDef_t*)USART2_BASE)
#define USART3      ((USART_TypeDef_t*)USART3_BASE)
#define USART6      ((USART_TypeDef_t*)USART6_BASE)

/*--------------MAX TIMEOUT----------------*/
#define USART_MAX_TIMEOUT   0xFFFFFFFFU

/*--------------Registers------------------*/
typedef struct{
    volatile uint32_t   SR;
    volatile uint32_t   DR;
    volatile uint32_t   BRR;
    volatile uint32_t   CR1;
    volatile uint32_t   CR2;
    volatile uint32_t   CR3;
    volatile uint32_t   GTPR;
} USART_TypeDef_t;

/*--------------USART WordLength------------*/
typedef enum{
    USART_WORDLENGTH_8B = 0,
    USART_WORDLENGTH_9B
} USART_WordLength_t;

/*-------------USART Parity----------------*/
typedef enum{
    USART_PARITY_NONE = 0,
    USART_PARITY_ODD,
    USART_PARITY_EVEN
} USART_Parity_t;

/*--------------USART Mode------------------*/
typedef enum{
    USART_MODE_POLLING = 0,
    USART_MODE_INTERRUPT,
    USART_MODE_DMA
} USART_Mode_t;

typedef enum{
    USART_STOPBITS_1 = 0,
    USART_STOPBITS_0_5,
    USART_STOPBITS_2,
    USART_STOPBITS_1_5
} USART_StopBits_t;
/*-----------------Status-------------------------*/
typedef enum{
    USART_OK = 0x00U,
    USART_ERROR,
    USART_BUSY,
    USART_TIMEOUT
} USART_Status_t;
/*----------------TX/RX Status--------------------*/
typedef enum{
    USART_STATE_RESET = 0,
    USART_STATE_READY,
    USART_STATE_BUSY_TX,
    USART_STATE_BUSY_RX
} USART_State_t;

/*-----------------USART Handle-------------------*/
typedef struct{
    USART_TypeDef_t*        Instance;
    uint32_t                Baudrate;
    USART_WordLength_t      WordLength;
    USART_Parity_t          Parity;
    USART_StopBits_t        StopBits;
    USART_Mode_t            Mode;

    USART_State_t           TxState; // TX state
    USART_State_t           RxState; // RX state
} USART_Handle_t;
/*-------------------Registers Bit Mask---------------------*/
// SR
#define USART_SR_PE         (1 << 0)
#define USART_SR_FE         (1 << 1)
#define USART_SR_NF         (1 << 2)
#define USART_SR_ORE        (1 << 3)
#define USART_SR_IDLE       (1 << 4)
#define USART_SR_RXNE       (1 << 5)
#define USART_SR_TC         (1 << 6)
#define USART_SR_TXE        (1 << 7)
#define USART_SR_LBD        (1 << 8)
#define USART_SR_CTS        (1 << 9)
// CR1
#define USART_CR1_SBK       (1 << 0)
#define USART_CR1_RWU       (1 << 1)
#define USART_CR1_RE        (1 << 2)
#define USART_CR1_TE        (1 << 3)
#define USART_CR1_IDLEIE    (1 << 4)
#define USART_CR1_RXNEIE    (1 << 5)
#define USART_CR1_TCIE      (1 << 6)
#define USART_CR1_TXEIE     (1 << 7)
#define USART_CR1_PEIE      (1 << 8)
#define USART_CR1_PS        (1 << 9)
#define USART_CR1_PCE       (1 << 10)
#define USART_CR1_WAKE      (1 << 11)
#define USART_CR1_M         (1 << 12)
#define USART_CR1_UE        (1 << 13)
#define USART_CR1_OVER8     (1 << 15)
// CR2
#define USART_CR2_LBDL      (1 << 5)
#define USART_CR2_LBDIE     (1 << 6)
#define USART_CR2_LBCL      (1 << 8)
#define USART_CR2_CPHA      (1 << 9)
#define USART_CR2_CPOL      (1 << 10)
#define USART_CR2_CLKEN     (1 << 11)
#define USART_CR2_LINEN     (1 << 14)
// CR3
#define USART_CR3_EIE       (1 << 0)
#define USART_CR3_IREN      (1 << 1)
#define USART_CR3_IRLP      (1 << 2)
#define USART_CR3_HDSEL     (1 << 3)
#define USART_CR3_NACK      (1 << 4)
#define USART_CR3_SCEN      (1 << 5)
#define USART_CR3_DMAR      (1 << 6)
#define USART_CR3_DMAT      (1 << 7)
#define USART_CR3_RTSE      (1 << 8)
#define USART_CR3_CTSE      (1 << 9)
#define USART_CR3_CTSIE     (1 << 10)
#define USART_CR3_ONEBIT    (1 << 11)

/*-------------------API--------------------------*/
USART_Status_t USART_Init(USART_Handle_t* huart);
// Polling Mode
USART_Status_t USART_Transmit(USART_Handle_t* huart, uint8_t* pData, uint16_t size,uint32_t timeout);
USART_Status_t USART_Receive(USART_Handle_t* huart, uint8_t* pData, uint16_t size, uint32_t timeout);
// Interrupt
USART_Status_t USART_Transmit_IT(USART_Handle_t* huart, uint8_t* pData, uint16_t size);
USART_Status_t USART_Receive_IT(USART_Handle_t* huart, uint8_t* pData, uint16_t size);
#endif /*UART_H*/