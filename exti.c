#include "exti.h"


void EXTI_Init(GPIO_Handle_t* hGPIO){
    /* 0. Disable Global Interrupt */
    /* 1. Enable Clock for GPIO */
    GPIO_TypeDef_t* GPIOx = hGPIO->Instance;
    if(GPIOx == GPIOA){
        RCC_EnableAHB1Clock(RCC_AHB1ENR_GPIOAEN);
    } else if(GPIOx == GPIOB) {
        RCC_EnableAHB1Clock(RCC_AHB1ENR_GPIOBEN);
    } else if(GPIOx == GPIOC) {
        RCC_EnableAHB1Clock(RCC_AHB1ENR_GPIOCEN);
    } else if(GPIOx == GPIOD) {
        RCC_EnableAHB1Clock(RCC_AHB1ENR_GPIODEN);
    } else if(GPIOx == GPIOE) {
        RCC_EnableAHB1Clock(RCC_AHB1ENR_GPIOEEN);
    } else if(GPIOx == GPIOF) {
        RCC_EnableAHB1Clock(RCC_AHB1ENR_GPIOFEN);
    } else if(GPIOx == GPIOG) {
        RCC_EnableAHB1Clock(RCC_AHB1ENR_GPIOGEN);
    } else if(GPIOx == GPIOH) {
        RCC_EnableAHB1Clock(RCC_AHB1ENR_GPIOHEN);
    } else if(GPIOx == GPIOI) {
        RCC_EnableAHB1Clock(RCC_AHB1ENR_GPIOIEN);
    }
    /* 1.1. Set GPIO as INPUT */
    GPIO_SetMode(GPIOx,hGPIO->GPIO_PinConfig.Pin,GPIO_MODE_INPUT);
    /* 2. Enable Clock for SYSCFG */
    RCC_EnableAPB2Clock(RCC_APB2ENR_SYSCFG_EN);
    /* 3. SELECT EXTIx(PA0 or PB13 or .. will use different EXTIx)*/
    uint8_t pin      = hGPIO->GPIO_PinConfig.Pin;           // 0–15
    uint8_t crIndex  = pin / 4;                             // 0→CR1, 1→CR2, 2→CR3, 3→CR4
    uint8_t crShift  = (pin % 4) * 4;                       // bit position inside CRx (0,4,8,12)

    // Derive port code: GPIOA=0, GPIOB=1, ... GPIOI=8
    uint8_t portCode = ((uint32_t)GPIOx - (uint32_t)GPIOA) / 0x400;

    // Clear the 4-bit field then write port code
    SYSCFG->EXTICR[crIndex] &= ~(0xF << crShift);
    SYSCFG->EXTICR[crIndex] |=  (portCode << crShift);

    /* 4. Unmask EXTIx ? */
    EXTI->IMR |= (1U << pin);
    /* 5. Select falling edge trigger */
    if(hGPIO->GPIO_PinConfig.Trigger == EXTI_TRIGGER_FALLING){
        EXTI->FTSR |=  (1U << pin);   // enable  falling
        EXTI->RTSR &= ~(1U << pin);   // disable rising
    } else if(hGPIO->GPIO_PinConfig.Trigger == EXTI_TRIGGER_RISING){
        EXTI->RTSR |=  (1U << pin);   // enable  rising
        EXTI->FTSR &= ~(1U << pin);   // disable falling
    } else if(hGPIO->GPIO_PinConfig.Trigger == EXTI_TRIGGER_BOTH){
        EXTI->FTSR |= (1U << pin);    // enable both
        EXTI->RTSR |= (1U << pin);
    }
    /* 6. Enable EXTIx Line in NVIC */
    IRQn_Type irqNumber;
    if      (pin == 0)               irqNumber = EXTI0_IRQn;
    else if (pin == 1)               irqNumber = EXTI1_IRQn;
    else if (pin == 2)               irqNumber = EXTI2_IRQn;
    else if (pin == 3)               irqNumber = EXTI3_IRQn;
    else if (pin == 4)               irqNumber = EXTI4_IRQn;
    else if (pin >= 5  && pin <= 9)  irqNumber = EXTI9_5_IRQn;
    else if (pin >= 10 && pin <= 15) irqNumber = EXTI15_10_IRQn;

    NVIC_SetPriority(irqNumber, 15);   // lowest priority; adjust as needed
    NVIC_EnableIRQ(irqNumber);
    /* 7. Enable GLOBAL Interrupt */
}

static void EXTI_IRQHandler(){

}