#include "pwm.h"

/* Internal Function Prototype */
static void PWM_ConfigChannel(TIM_Handle_t* htim){
    TIM_TypeDef_t* TIMx = htim->Instance;

    switch (htim->Init.Channel)
    {
    case PWM_CHANNEL_1:
        /* code */
        TIMx->CCMR1 &= ~(0x7U << 4); // Clear OC1M
        TIMx->CCMR1 &= ~(0x3U << 0); // CC1S
        TIMx->CCMR1 |= TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC1PE;
        TIMx->CCER  |= TIM_CCER_CC1E;
        break;

    case PWM_CHANNEL_2:
        TIMx->CCMR1 &= ~(0x7U << 12);
        TIMx->CCMR1 &= ~(0x3U << 8); // CC2S
        TIMx->CCMR1 |= TIM_CCMR1_OC2M_PWM1 | TIM_CCMR1_OC2PE;
        TIMx->CCER  |= TIM_CCER_CC2E;
        break;

    case PWM_CHANNEL_3:
        TIMx->CCMR2 &= ~(0x7U << 4);
        TIMx->CCMR2 &= ~(0x3U << 0); // CC3S
        TIMx->CCMR2 |= TIM_CCMR2_OC3M_PWM1 | TIM_CCMR2_OC3PE;
        TIMx->CCER  |= TIM_CCER_CC3E;
        break;

    case PWM_CHANNEL_4:
        TIMx->CCMR2 &= ~(0x7U << 12);
        TIMx->CCMR2 &= ~(0x3U << 8); // CC4S
        TIMx->CCMR2 |= TIM_CCMR2_OC4M_PWM1 | TIM_CCMR2_OC4PE;
        TIMx->CCER  |= TIM_CCER_CC4E;
        break;

    default:
        break;
    }
}

void PWM_Init(TIM_Handle_t* htim){
    TIM_TypeDef_t* TIMx = htim->Instance;

    /* 1 & 2 - Set timing: PSC, ARR, CNT*/
    TIMx->PSC = htim->Init.Prescaler;
    TIMx->ARR = htim->Init.AutoReload;
    TIMx->CNT = htim->Init.CounterStart;

    /* 3 & 4. Output Compare and Enable Capture/ Compare Outputs */
    PWM_ConfigChannel(htim);

    /* 5 – Update event  */
    TIMx->EGR   |= TIM_EGR_UG;

    /* 6 – Enable ARPE and start counter */
    TIMx->CR1   |= TIM_CR1_ARPE | TIM_CR1_CEN;
}

void PWM_Generation(TIM_Handle_t* htim, uint8_t dutyCycle){
    TIM_TypeDef_t* TIMx = htim->Instance;

    if (dutyCycle > 100U) dutyCycle = 100U;

    uint32_t ccr_val = (dutyCycle * (htim->Init.AutoReload + 1U)) / 100U;

    switch (htim->Init.Channel)
    {
        case PWM_CHANNEL_1: TIMx->CCR1 = ccr_val; break;
        case PWM_CHANNEL_2: TIMx->CCR2 = ccr_val; break;
        case PWM_CHANNEL_3: TIMx->CCR3 = ccr_val; break;
        case PWM_CHANNEL_4: TIMx->CCR4 = ccr_val; break;
        default: break;
    }
}