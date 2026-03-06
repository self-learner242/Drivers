#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/*-----------BASE ADDRESS-----------------*/
#define ADC1_BASE        0x40012000U
#define ADC2_BASE        0x40012100U
#define ADC3_BASE        0x40012200U
#define ADC_COMMON_BASE  0x40012300U

/*--------------ADCx-----------------------*/
#define ADC1            ((ADC_TypeDef_t*)ADC1_BASE)
#define ADC2            ((ADC_TypeDef_t*)ADC2_BASE)
#define ADC3            ((ADC_TypeDef_t*)ADC3_BASE)
#define ADC_COMMON      ((ADC_Common_TypeDef_t*)ADC_COMMON_BASE)

/*------------REGISTERS------------------*/
typedef struct{
    volatile uint32_t   SR;
    volatile uint32_t   CR1;
    volatile uint32_t   CR2;
    volatile uint32_t   SMPR1;
    volatile uint32_t   SMPR2;
    volatile uint32_t   JOFR1;
    volatile uint32_t   JOFR2;
    volatile uint32_t   JOFR3;
    volatile uint32_t   JOFR4;
    volatile uint32_t   HTR;
    volatile uint32_t   LTR;
    volatile uint32_t   SQR1;
    volatile uint32_t   SQR2;
    volatile uint32_t   SQR3;
    volatile uint32_t   JSQR;
    volatile uint32_t   JDR1;
    volatile uint32_t   JDR2;
    volatile uint32_t   JDR3;
    volatile uint32_t   JDR4;
    volatile uint32_t   DR;
} ADC_TypeDef_t;

/*----------COMMON REGISTERS-------------*/
typedef struct{
    volatile uint32_t   CSR;
    volatile uint32_t   CCR;
    volatile uint32_t   CDR;
} ADC_Common_TypeDef_t;

/*----------ADC_RESOLUTION---------------*/
typedef enum{
    ADC_RESOLUTION_12 = 0x0U,
    ADC_RESOLUTION_10,
    ADC_RESOLUTION_8,
    ADC_RESOLUTION_6
} ADC_Resolution_t; // RES bit in ADC_CR1[25:24]

typedef enum{
    ADC_ALIGN_RIGHT = 0,
    ADC_ALIGN_LEFT
} ADC_Align_t; // ALIGN in ADC_CR2[11]

typedef enum{
    ADC_CONVERSION_SINGLE = 0,
    ADC_CONVERSION_CONTINUOUS
} ADC_ConversionMode_t;

typedef enum{
    ADC_SCAN_DISABLED = 0,
    ADC_SCAN_ENABLED
} ADC_ScanMode_t;

typedef enum{
    ADC_EXTEN_DISABLED = 0x00,
    ADC_EXTEN_RISING,
    ADC_EXTEN_FALLING,
    ADC_EXTEN_RISINGANDFALLING
} ADC_ExternalTrig_t;

typedef enum{
    ADC_SAMPLETIME_3CYCLES   = 0,
    ADC_SAMPLETIME_15CYCLES  = 1,
    ADC_SAMPLETIME_28CYCLES  = 2,
    ADC_SAMPLETIME_56CYCLES  = 3,
    ADC_SAMPLETIME_84CYCLES  = 4,
    ADC_SAMPLETIME_112CYCLES = 5,
    ADC_SAMPLETIME_144CYCLES = 6,
    ADC_SAMPLETIME_480CYCLES = 7
} ADC_SampleTime_t;

typedef enum{
    ADC_CHANNEL_0 = 0,
    ADC_CHANNEL_1,
    ADC_CHANNEL_2,
    ADC_CHANNEL_3,
    ADC_CHANNEL_4,
    ADC_CHANNEL_5,
    ADC_CHANNEL_6,
    ADC_CHANNEL_7,
    ADC_CHANNEL_8,
    ADC_CHANNEL_9,
    ADC_CHANNEL_10,
    ADC_CHANNEL_11,
    ADC_CHANNEL_12,
    ADC_CHANNEL_13,
    ADC_CHANNEL_14,
    ADC_CHANNEL_15,
    ADC_CHANNEL_16,
    ADC_CHANNEL_17,
    ADC_CHANNEL_18,
} ADC_Channel_t;

typedef struct {
    ADC_TypeDef_t*          Instance;
    ADC_Resolution_t        Resolution;
    ADC_Align_t             Align;
    ADC_ConversionMode_t    ConversionMode;

    ADC_ScanMode_t          ScanConvMode;

    ADC_ExternalTrig_t      ExternalTrig;
    ADC_SampleTime_t        SampleTime;
    ADC_Channel_t           Channel;
} ADC_Handle_t;

/*-----------------STATUS-------------------------------*/
// typedef enum{
//     ADC_OK = 0,
//     ADC_ERROR
// } ADC_Status_t;
/*-----------------REGISTERS BIT MASKS------------------*/
// SR
#define ADC_SR_EOC          (1 << 1)
// CR1
#define ADC_CR1_SCAN        (1 << 8)
// CR2
#define ADC_CR2_ADON        (1 << 0)
#define ADC_CR2_CONT        (1 << 1)
#define ADC_CR2_ALIGN       (1 << 11)
#define ADC_CR2_JSWSTART    (1 << 22)
#define ADC_CR2_SWSTART     (1 << 30)
// 
/*-------------APIs-----------------------*/
void ADC_Init(ADC_Handle_t *hadc);
void ADC_Start(ADC_Handle_t *hadc);
void ADC_WaitEOC(ADC_Handle_t *hadc);

uint16_t ADC_Read(ADC_Handle_t *hadc);

#endif /*ADC_H*/