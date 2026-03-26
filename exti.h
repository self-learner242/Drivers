#ifndef EXTI_H
#define EXTI_H

#include <stdint.h>
#include "rcc.h"
#include "gpio.h"

typedef struct{
    volatile uint32_t   IMR;
    volatile uint32_t   EMR;
    volatile uint32_t   RTSR;
    volatile uint32_t   FTSR;
    volatile uint32_t   SWIER;
    volatile uint32_t   PR;
} EXTI_TypeDef_t;
/* APIs */
void EXTI_Init(GPIO_Handle_t* hGPIO);

#endif /* EXTI_H */