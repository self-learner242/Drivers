#include "rcc.h"

// AHB1ENR
void RCC_EnableAHB1Clock(uint32_t mask){
    RCC->AHB1ENR |= mask;
    (void)RCC->AHB1ENR;
}

void RCC_DisableAHB1Clock(uint32_t mask){
    RCC->AHB1ENR &= ~mask;
}

void RCC_ResetAHB1Clock(uint32_t mask){
    RCC->AHB1RSTR |= mask;
    RCC->AHB1RSTR &= ~mask;
}

// APB1ENR

void RCC_EnableAPB1Clock(uint32_t mask){
    RCC->APB1ENR |= mask;
    (void)RCC->APB1ENR;
}

// APB2ENR
void RCC_EnableAPB2Clock(uint32_t mask){
    RCC->APB2ENR |= mask;
    (void)RCC->APB2ENR;
}

