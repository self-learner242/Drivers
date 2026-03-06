#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>
/*------Base Address--------*/
#define SYST_BASE 0xE000E010

/*-------Registers----------------*/
typedef struct{
    volatile uint32_t CSR; // Control and Status
    volatile uint32_t RVR; // Reload Value
    volatile uint32_t CVR; // Current Value
    volatile uint32_t CALIB;
} SYSTICK_TypeDef_t;

/*---------------Pointer to SysTick---------------*/
#define SYSTICK    ((SYSTICK_TypeDef_t *) SYST_BASE)

/*----------------Bit field Mask------------------*/
#define SYST_CSR_COUNTFLAG      (1 << 16)
#define SYST_CSR_CLKSOURCE      (1 << 2)
#define SYST_CSR_TICKINT        (1 << 1)
#define SYST_CSR_ENABLE         (1 << 0)

/*---------------Global Variables-----------------*/
extern volatile uint32_t g_tick;

/*-----------------------API----------------------*/
// void SysTick_Init(uint32_t tick_hz);
// void SysTick_DelayMs(uint32_t ms); // Polling
// void SysTick_Start(void);
// void SysTick_Stop(void);
// SysTick Interrupt
void SysTick_Init(uint32_t tick_hz);
uint32_t SysTick_GetTick(void);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

#endif