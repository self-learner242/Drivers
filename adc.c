#include "adc.h"

void ADC_Init(ADC_Handle_t *hadc){
    // 1. Always DISABLE before config
    hadc->Instance->CR2 &= ~ADC_CR2_ADON;

    // 2. Configure Resolution
    hadc->Instance->CR1 &= ~(0x3U << 24);
    hadc->Instance->CR1 |= (hadc->Resolution) << 24;

    // 3. Configure Alignment
    hadc->Instance->CR2 &= ~ADC_CR2_ALIGN;
    hadc->Instance->CR2 |= (hadc->Align) << 11;

    // 4. Configure Conversion Mode
    hadc->Instance->CR2 &= ~ADC_CR2_CONT;
    hadc->Instance->CR2 |= (hadc->ConversionMode) << 1;

    // 5. Configure Sequence length: 1
    hadc->Instance->SQR1 &= ~(0xF << 20);

    // 6. Configure Channel Rank
    hadc->Instance->SQR3 &= ~(0x1F << 0);
    hadc->Instance->SQR3 |= (hadc->Channel) << 0; // First in Sequence

    // 7. Configure Sample Time
    if(hadc->Channel <= 9){
        hadc->Instance->SMPR2 &= ~(0x7U << (3 * hadc->Channel));
        hadc->Instance->SMPR2 |= (hadc->SampleTime) << 3 * hadc->Channel;
    }
    else{
        hadc->Instance->SMPR1 &= ~(0x7U << (3 * (hadc->Channel - 10)));
        hadc->Instance->SMPR1 |= (hadc->SampleTime) << 3 * (hadc->Channel - 10);
    }

    // 8. Enbale ADC
    hadc->Instance->CR2 |= ADC_CR2_ADON;
}

void ADC_Start(ADC_Handle_t *hadc){
    hadc->Instance->CR2 |= ADC_CR2_SWSTART;
}

void ADC_WaitEOC(ADC_Handle_t *hadc){
    while(!(hadc->Instance->SR & ADC_SR_EOC));
}

uint16_t ADC_Read(ADC_Handle_t *hadc){
    switch (hadc->Resolution)
    {
    // Only true for RIGHT ALIGNMENT
    case ADC_RESOLUTION_12:
        return (uint16_t)hadc->Instance->DR & 0xFFF;
        break;
    case ADC_RESOLUTION_10:
        return (uint16_t)hadc->Instance->DR & 0x3FF;
        break;
    case ADC_RESOLUTION_8:
        return (uint16_t)hadc->Instance->DR & 0x0FF;
        break;
    case ADC_RESOLUTION_6:
        return (uint16_t)hadc->Instance->DR & 0x03F;
        break;
    default:
        return (uint16_t)hadc->Instance->DR & 0xFFF;
        break;
    }
}
