#include "gpio.h"
//#include "rcc.h"
// GPIO Init and Deinit
GPIO_Status_t GPIO_Init(const GPIO_Handle_t* hGPIO)
{
    if(hGPIO == NULL || hGPIO->Instance == NULL)
        return GPIO_ERROR_NULL_PTR;

    uint16_t pinMask = hGPIO->GPIO_PinConfig.Pin;

    if(pinMask == 0)
        return GPIO_ERROR_INVALID_PARAM;
    
    // Enable Clock
    //GPIO_EnableClock(hGPIO->Instance);

    if(hGPIO->GPIO_PinConfig.Mode > GPIO_MODE_ANALOG)
        return GPIO_ERROR_INVALID_PARAM;

    if(hGPIO->GPIO_PinConfig.OType > GPIO_OTYPE_OPENDRAIN)
        return GPIO_ERROR_INVALID_PARAM;

    if(hGPIO->GPIO_PinConfig.Speed > GPIO_SPEED_VERY_HIGH)
        return GPIO_ERROR_INVALID_PARAM;

    if(hGPIO->GPIO_PinConfig.Pull > GPIO_PULLDOWN)
        return GPIO_ERROR_INVALID_PARAM;

    for(uint8_t pos = 0; pos < 16; pos++)
    {
        if(pinMask & (1U << pos))
        {
            uint32_t shift = pos * 2U;

            /* MODER */
            hGPIO->Instance->MODER &= ~(3U << shift);
            hGPIO->Instance->MODER |= ((uint32_t)hGPIO->GPIO_PinConfig.Mode << shift);

            /* OTYPER */
            hGPIO->Instance->OTYPER &= ~(1U << pos);
            hGPIO->Instance->OTYPER |= ((uint32_t)hGPIO->GPIO_PinConfig.OType << pos);

            /* OSPEEDR */
            hGPIO->Instance->OSPEEDR &= ~(3U << shift);
            hGPIO->Instance->OSPEEDR |= ((uint32_t)hGPIO->GPIO_PinConfig.Speed << shift);

            /* PUPDR */
            hGPIO->Instance->PUPDR &= ~(3U << shift);
            hGPIO->Instance->PUPDR |= ((uint32_t)hGPIO->GPIO_PinConfig.Pull << shift);

            /* Alternate function */
            if(hGPIO->GPIO_PinConfig.Mode == GPIO_MODE_ALTFUNC)
            {
                uint8_t index = pos / 8U;
                uint8_t af_shift = (pos % 8U) * 4U;

                hGPIO->Instance->AFR[index] &= ~(0xFU << af_shift);
                hGPIO->Instance->AFR[index] |= ((uint32_t)hGPIO->GPIO_PinConfig.AltFunc << af_shift);
            }
        }
    }
    return GPIO_OK;
}
// GPIO_Status_t GPIO_DeInit(GPIO_TypeDef_t* Port){
//     if(Port == NULL)
//         return GPIO_ERROR_NULL_PTR;

//     RCC_ResetGPIO(Port);

//     return GPIO_OK;
// }

// GPIO Read & Write for Pin
void GPIO_WritePin(GPIO_TypeDef_t* Port, GPIO_Pin_t Pin, GPIO_PinState_t State){
    if(Port == NULL || Pin == 0) return;

    if(State == GPIO_PIN_SET){
        Port->BSRR = Pin;
    }
    else{
        Port->BSRR = (uint32_t)Pin << 16U;
    }
}
GPIO_Pin_t GPIO_ReadPin(GPIO_TypeDef_t* Port, GPIO_Pin_t Pin){
    if((Port == NULL) || (Pin == 0U))
    {
        return 0U;
    }

    return (GPIO_Pin_t)(Port->IDR & Pin);
}

void GPIO_TogglePin(GPIO_TypeDef_t* Port, GPIO_Pin_t Pin){
    if((Port == NULL) || (Pin == 0U)) return;
    
    uint32_t odr = Port->ODR;

    uint32_t resetMask = (odr & Pin) << 16U;
    uint32_t setMask   = (~odr & Pin);

    Port->BSRR = resetMask | setMask;
}

// GPIO Read & Write for Port
/*-----Overwrite Full Port-----*/
void GPIO_WritePort(GPIO_TypeDef_t* Port, uint16_t value){
    if(Port == NULL) return;

    Port->BSRR = 0xFFFF0000U;
    Port->BSRR = value;
}

uint16_t GPIO_ReadPort(const GPIO_TypeDef_t* Port){
    if(Port == NULL) return 0U;

    return (uint16_t)(Port->IDR & 0xFFFFU);
}

void GPIO_SetMode(GPIO_TypeDef_t *GPIOx, uint16_t pins, uint32_t mode)
{
    for(uint8_t pos = 0; pos < 16; pos++)
    {
        if(pins & (1U << pos))
        {
            /* Clear 2-bit field */
            GPIOx->MODER &= ~(0x3U << (pos * 2));

            /* Set new mode */
            GPIOx->MODER |= (mode << (pos * 2));
        }
    }
}