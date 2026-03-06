#include "systick.h"
extern uint32_t SystemCoreClock;

// void SysTick_Init(uint32_t tick_hz){
//     // Assign value for SYST_CSR, SYST_RVR, SYST_CVR
//     uint32_t reload = (SystemCoreClock / tick_hz) - 1;

//     SYSTICK->RVR = reload;
//     SYSTICK->CVR  = 0;
//     SYSTICK->CSR = SYST_CSR_CLKSOURCE;
// }

// void SysTick_Start(void){
//     SYSTICK->CVR = 0;
//     SYSTICK->CSR |= SYST_CSR_ENABLE;
// }

// void SysTick_Stop(void){
//      SYSTICK->CSR &= ~SYST_CSR_ENABLE;
// }

// void SysTick_DelayMs(uint32_t ms){
//     SYSTICK->RVR = (SystemCoreClock / 1000) - 1;
//     for(uint32_t i = 0; i < ms; i++){
//         SYSTICK->CVR = 0;
//         SYSTICK->CSR = SYST_CSR_ENABLE | SYST_CSR_CLKSOURCE;
//         while((SYSTICK->CSR & SYST_CSR_COUNTFLAG) == 0);

//         SYSTICK->CSR = 0; // Disable SysTick Timer
//     }
    
// }


// SysTick Interrupt
volatile uint32_t g_tick = 0;

void SysTick_Init(uint32_t tick_hz)
{
    uint32_t reload = (SystemCoreClock / tick_hz) - 1;

    if(reload > 0xFFFFFF) return; // Out of range 24-bit

    SYSTICK->RVR = reload;
    SYSTICK->CVR = 0;

    SYSTICK->CSR = SYST_CSR_CLKSOURCE | SYST_CSR_TICKINT | SYST_CSR_ENABLE;
}

void SysTick_Handler(void)
{
    g_tick++;
}

void delay_ms(uint32_t ms)
{
    uint32_t start = g_tick;
    while ((g_tick - start) < ms);
}

uint32_t SysTick_GetTick(void){
    return g_tick;
}

void delay_us(uint32_t us){
    uint32_t ticks = (SystemCoreClock / 1000000) * us;

    uint32_t start = SYSTICK->CVR;
    uint32_t reload = SYSTICK->RVR;
    uint32_t elapsed = 0;

    while (elapsed < ticks)
    {
        uint32_t current = SYSTICK->CVR;

        if (current <= start)
            elapsed += (start - current);
        else
            elapsed += (start + (reload - current));

        start = current;
    }
}