#ifndef PWM_H
#define PWM_H

/* LIBRARY */
#include <stdint.h>
/* BASE ADDRESS */ 
#define TIM2_BASE       0x40000000
#define TIM3_BASE       0x40000400
#define TIM4_BASE       0x40000800
#define TIM5_BASE       0x40000C00

/* INSTANCE */
#define TIM2            ((TIM_TypeDef_t*)TIM2_BASE)
#define TIM3            ((TIM_TypeDef_t*)TIM3_BASE)
#define TIM4            ((TIM_TypeDef_t*)TIM4_BASE)
#define TIM5            ((TIM_TypeDef_t*)TIM5_BASE)

/* REGISTERS */
typedef struct{
    volatile uint32_t   CR1;
    volatile uint32_t   CR2;
    volatile uint32_t   SMCR;
    volatile uint32_t   DIER;
    volatile uint32_t   SR;
    volatile uint32_t   EGR;
    volatile uint32_t   CCMR1;
    volatile uint32_t   CCMR2;
    volatile uint32_t   CCER;
    volatile uint32_t   CNT;
    volatile uint32_t   PSC;
    volatile uint32_t   ARR;
    uint32_t            RESERVED0;
    volatile uint32_t   CCR1;
    volatile uint32_t   CCR2;
    volatile uint32_t   CCR3;
    volatile uint32_t   CCR4;
    uint32_t            RESERVED1;
    volatile uint32_t   DCR;
    volatile uint32_t   DMAR;
} TIM_TypeDef_t;

/* CHANNEL */
typedef enum{
    PWM_CHANNEL_1,
    PWM_CHANNEL_2,
    PWM_CHANNEL_3,
    PWM_CHANNEL_4
} TIM_Channel_t;
/* CONFIG */
typedef struct{
    uint32_t            CounterStart;
    uint16_t            Prescaler;
    uint32_t            AutoReload;
    TIM_Channel_t       Channel;
} TIM_Config_t;

/* INSTANCE */
typedef struct{
    TIM_TypeDef_t*      Instance;
    TIM_Config_t        Init;
} TIM_Handle_t;

/* BITS MASK */
/* CR1 */
#define TIM_CR1_CEN         (1U << 0)   
#define TIM_CR1_ARPE        (1U << 7)   

/* CCMR1 (Output Compare, channel 1 & 2) */
#define TIM_CCMR1_OC1M_PWM1 (0x6U << 4)  
#define TIM_CCMR1_OC1PE     (1U << 3)    
#define TIM_CCMR1_OC2M_PWM1 (0x6U << 12) 
#define TIM_CCMR1_OC2PE     (1U << 11)   

/* CCMR2 (Output Compare, channel 3 & 4) */
#define TIM_CCMR2_OC3M_PWM1 (0x6U << 4)  
#define TIM_CCMR2_OC3PE     (1U << 3)    
#define TIM_CCMR2_OC4M_PWM1 (0x6U << 12) 
#define TIM_CCMR2_OC4PE     (1U << 11)   

/* CCER – capture/compare output enable (all 4 channels) */
#define TIM_CCER_CC1E       (1U << 0)
#define TIM_CCER_CC2E       (1U << 4)
#define TIM_CCER_CC3E       (1U << 8)
#define TIM_CCER_CC4E       (1U << 12)

/* EGR */
#define TIM_EGR_UG          (1U << 0)   /* Update generation          */

/* APIs */
void PWM_Init(TIM_Handle_t* htim);
void PWM_Generation(TIM_Handle_t* htim, uint8_t dutyCycle);

#endif


// Auto Reload
// Prescaler
// CNT
