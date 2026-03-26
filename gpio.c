#include "gpio.h"
#include "rcc.h"

GPIO_Status_t GPIO_Init(const GPIO_Handle_t* hGPIO)
{
    // Checking valid param
    if(hGPIO == NULL || hGPIO->Instance == NULL)
        return GPIO_ERROR_NULL_PTR;

    uint16_t pinMask = hGPIO->GPIO_PinConfig.Pin;

    if(pinMask == 0)
        return GPIO_ERROR_INVALID_PARAM;

    if(hGPIO->GPIO_PinConfig.Mode > GPIO_MODE_ANALOG)
        return GPIO_ERROR_INVALID_PARAM;

    if(hGPIO->GPIO_PinConfig.OType > GPIO_OTYPE_OPENDRAIN)
        return GPIO_ERROR_INVALID_PARAM;

    if(hGPIO->GPIO_PinConfig.Speed > GPIO_SPEED_VERY_HIGH)
        return GPIO_ERROR_INVALID_PARAM;

    if(hGPIO->GPIO_PinConfig.Pull > GPIO_PULLDOWN)
        return GPIO_ERROR_INVALID_PARAM;

    GPIO_TypeDef_t* GPIOx = hGPIO->Instance;

    // Enable Clock
    if (GPIOx == GPIOA){
        RCC_EnableAHB1Clock(RCC_AHB1ENR_GPIOAEN);
    } else if (GPIOx == GPIOB) {
        RCC_EnableAHB1Clock(RCC_AHB1ENR_GPIOBEN);
    } else if (GPIOx == GPIOC) {
        RCC_EnableAHB1Clock(RCC_AHB1ENR_GPIOCEN);
    } else if (GPIOx == GPIOD) {
        RCC_EnableAHB1Clock(RCC_AHB1ENR_GPIODEN);
    } else if (GPIOx == GPIOE) {
        RCC_EnableAHB1Clock(RCC_AHB1ENR_GPIOEEN);
    } else if (GPIOx == GPIOF) {
        RCC_EnableAHB1Clock(RCC_AHB1ENR_GPIOFEN);
    } else if (GPIOx == GPIOG) {
        RCC_EnableAHB1Clock(RCC_AHB1ENR_GPIOGEN);
    } else if (GPIOx == GPIOH) {
        RCC_EnableAHB1Clock(RCC_AHB1ENR_GPIOHEN);
    } else if (GPIOx == GPIOI) {
        RCC_EnableAHB1Clock(RCC_AHB1ENR_GPIOIEN);
    }

    for(uint8_t pos = 0; pos < 16; pos++)
    {
        if(pinMask & (1U << pos))
        {
            uint32_t shift = pos * 2U;

            /* MODER */
            GPIOx->MODER &= ~(3U << shift);
            hGPIO->Instance->MODER |= ((uint32_t)hGPIO->GPIO_PinConfig.Mode << shift);

            /* OTYPER */
            GPIOx->OTYPER &= ~(1U << pos);
            GPIOx->OTYPER |= ((uint32_t)hGPIO->GPIO_PinConfig.OType << pos);

            /* OSPEEDR */
            GPIOx->OSPEEDR &= ~(3U << shift);
            GPIOx->OSPEEDR |= ((uint32_t)hGPIO->GPIO_PinConfig.Speed << shift);

            /* PUPDR */
            GPIOx->PUPDR &= ~(3U << shift);
            GPIOx->PUPDR |= ((uint32_t)hGPIO->GPIO_PinConfig.Pull << shift);

            /* Alternate function */
            if(hGPIO->GPIO_PinConfig.Mode == GPIO_MODE_ALTFUNC)
            {
                uint8_t index = pos / 8U;
                uint8_t af_shift = (pos % 8U) * 4U;

                GPIOx->AFR[index] &= ~(0xFU << af_shift);
                GPIOx->AFR[index] |= ((uint32_t)hGPIO->GPIO_PinConfig.AltFunc << af_shift);
            }
        }
    }
    return GPIO_OK;
}

// GPIO Read & Write for Pin
void GPIO_WritePin(GPIO_TypeDef_t* GPIOx, GPIO_Pin_t Pin, GPIO_PinState_t State){
    if(GPIOx == NULL || Pin == 0) return;

    if(State == GPIO_PIN_SET){
        GPIOx->BSRR = Pin;
    }
    else{
        GPIOx->BSRR = (uint32_t)Pin << 16U;
    }
}
GPIO_Pin_t GPIO_ReadPin(GPIO_TypeDef_t* GPIOx, GPIO_Pin_t Pin){
    if((GPIOx == NULL) || (Pin == 0U))
    {
        return 0U;
    }

    return (GPIO_Pin_t)(GPIOx->IDR & Pin);
}

void GPIO_TogglePin(GPIO_TypeDef_t* GPIOx, GPIO_Pin_t Pin){ 
    if((GPIOx == NULL) || (Pin == 0U)) return;
    
    uint32_t odr = GPIOx->ODR;

    uint32_t resetMask = (odr & Pin) << 16U;
    uint32_t setMask   = (~odr & Pin);

    GPIOx->BSRR = resetMask | setMask;
}

// GPIO Read & Write for GPIOx
/*-----Overwrite Full GPIOx-----*/
void GPIO_WritePort(GPIO_TypeDef_t* GPIOx, uint16_t value){
    if(GPIOx == NULL) return;

    GPIOx->BSRR = 0xFFFF0000U;
    GPIOx->BSRR = value;
}

uint16_t GPIO_ReadPort(const GPIO_TypeDef_t* GPIOx){
    if(GPIOx == NULL) return 0U;

    return (uint16_t)(GPIOx->IDR & 0xFFFFU);
}

void GPIO_SetMode(GPIO_TypeDef_t* GPIOx, uint16_t Pins, GPIO_Mode_t mode){
    for(uint8_t pos = 0; pos < 16; pos++)
    {
        if(Pins & (1U << pos))
        {
            /* Clear 2-bit field */
            GPIOx->MODER &= ~(0x3U << (pos * 2));

            /* Set new mode */
            GPIOx->MODER |= (mode << (pos * 2));
        }
    }
}
