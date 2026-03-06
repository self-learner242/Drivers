#ifndef RCC_H
#define RCC_H


#include <stdint.h>

/*---------------------------RCC BASE ADDR-----------------------*/
#define RCC_BASE_ADDR   0x40023800U
#define RCC             ((volatile RCC_RegDef_t*)RCC_BASE_ADDR)

/*------------------------REGISTER MAP---------------------------*/
typedef struct{
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
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
} RCC_RegDef_t;

/*-----------------------BIT MASKS--------------------------------*/
// AHB1ENR
#define RCC_AHB1ENR_GPIOAEN     (1U << 0)
#define RCC_AHB1ENR_GPIOBEN     (1U << 1)
#define RCC_AHB1ENR_GPIOCEN     (1U << 2)
#define RCC_AHB1ENR_GPIODEN     (1U << 3)
#define RCC_AHB1ENR_GPIOEEN     (1U << 4)
#define RCC_AHB1ENR_GPIOFEN     (1U << 5)
#define RCC_AHB1ENR_GPIOGEN     (1U << 6)
#define RCC_AHB1ENR_GPIOHEN     (1U << 7)
#define RCC_AHB1ENR_GPIOIEN     (1U << 8)

// APB1ENR
#define RCC_APB1ENR_TIM2EN      (1 << 0)
#define RCC_APB1ENR_TIM3EN      (1 << 1)
#define RCC_APB1ENR_TIM4EN      (1 << 2)
#define RCC_APB1ENR_TIM5EN      (1 << 5)
#define RCC_APB1ENR_USART2EN    (1 << 17)
#define RCC_APB1ENR_USART3EN    (1 << 18)
#define RCC_APB1ENR_I2C1EN      (1 << 21)
#define RCC_APB1ENR_I2C2EN      (1 << 22)
#define RCC_APB1ENR_I2C3EN      (1 << 23)


// APB2ENR
#define RCC_APB2ENR_USART1EN    (1 << 4)
#define RCC_APB2ENR_USART6EN    (1 << 5) 
#define RCC_APB2ENR_ADC1_EN     (1 << 8)
#define RCC_APB2ENR_ADC2_EN     (1 << 9)
#define RCC_APB2ENR_ADC3_EN     (1 << 10)

/*--------------------------API------------------------------*/
void RCC_EnableAHB1Clock(uint32_t mask);
void RCC_DisableAHB1Clock(uint32_t mask);
void RCC_ResetAHB1Clock(uint32_t mask);

void RCC_EnableAPB1Clock(uint32_t mask);
void RCC_EnableAPB2Clock(uint32_t mask);


#endif