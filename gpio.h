#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <stdlib.h>
/*-------------------------Preprocessor-------------------------------------*/
#define IS_GPIO_PORT(PORT) \
    ((PORT == GPIOA) || (PORT == GPIOB) || \
     (PORT == GPIOC) || (PORT == GPIOD) || \
     (PORT == GPIOE) || (PORT == GPIOF) || \
     (PORT == GPIOG) || (PORT == GPIOH) || \
     (PORT == GPIOI))
/*-------------------------Global Variables---------------------------------*/
typedef uint16_t GPIO_Pin_t;

/*-------------------------Base Address-------------------------------------*/
#define GPIOA_BASE      0x40020000
#define GPIOB_BASE      0x40020400
#define GPIOC_BASE      0x40020800
#define GPIOD_BASE      0x40020C00
#define GPIOE_BASE      0x40021000
#define GPIOF_BASE      0x40021400
#define GPIOG_BASE      0x40021800
#define GPIOH_BASE      0x40021C00
#define GPIOI_BASE      0x40022000

/*------------------------GPIO Registers--------------------------------*/
typedef struct{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2]; // AFRH and AFRL
} GPIO_TypeDef_t;

/*------------------------Peripheral Port Declaration--------------------------*/
#define GPIOA       ((GPIO_TypeDef_t*)  GPIOA_BASE)
#define GPIOB       ((GPIO_TypeDef_t*)  GPIOB_BASE)
#define GPIOC       ((GPIO_TypeDef_t*)  GPIOC_BASE)
#define GPIOD       ((GPIO_TypeDef_t*)  GPIOD_BASE)
#define GPIOE       ((GPIO_TypeDef_t*)  GPIOE_BASE)
#define GPIOF       ((GPIO_TypeDef_t*)  GPIOF_BASE)
#define GPIOG       ((GPIO_TypeDef_t*)  GPIOG_BASE)
#define GPIOH       ((GPIO_TypeDef_t*)  GPIOH_BASE)
#define GPIOI       ((GPIO_TypeDef_t*)  GPIOI_BASE)

/*--------------------------Pin Mask----------------------------------------*/
#define GPIO_PIN_0      (1U << 0)
#define GPIO_PIN_1      (1U << 1)
#define GPIO_PIN_2      (1U << 2)
#define GPIO_PIN_3      (1U << 3)
#define GPIO_PIN_4      (1U << 4)
#define GPIO_PIN_5      (1U << 5)
#define GPIO_PIN_6      (1U << 6)
#define GPIO_PIN_7      (1U << 7)
#define GPIO_PIN_8      (1U << 8)
#define GPIO_PIN_9      (1U << 9)
#define GPIO_PIN_10     (1U << 10)
#define GPIO_PIN_11     (1U << 11)
#define GPIO_PIN_12     (1U << 12)
#define GPIO_PIN_13     (1U << 13)
#define GPIO_PIN_14     (1U << 14)
#define GPIO_PIN_15     (1U << 15)

/*--------------------------Status------------------------------------*/
typedef enum{
    GPIO_OK = 0,
    GPIO_ERROR_INVALID_PARAM,
    GPIO_ERROR_NULL_PTR
} GPIO_Status_t;

/*----------------Pin States-------------------*/
typedef enum{
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET
} GPIO_PinState_t;

/*----------------Pin Modes--------------------*/
typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_ALTFUNC,
    GPIO_MODE_ANALOG
} GPIO_Mode_t;

/*---------------Output Types------------------*/
typedef enum {
    GPIO_OTYPE_PUSHPULL = 0,
    GPIO_OTYPE_OPENDRAIN
} GPIO_OutputType_t;

/*---------------Output Speed------------------*/
typedef enum {
    GPIO_SPEED_LOW = 0,
    GPIO_SPEED_MEDIUM,
    GPIO_SPEED_HIGH,
    GPIO_SPEED_VERY_HIGH
} GPIO_Speed_t;

/*------------Pull up Pull down---------------*/
typedef enum{
    GPIO_NOPULL = 0,
    GPIO_PULLUP,
    GPIO_PULLDOWN
} GPIO_PullUpPullDown_t;


/*------------------------ Config --------------------------------*/
typedef struct{
    GPIO_Pin_t              Pin; 
    GPIO_Mode_t             Mode;
    GPIO_OutputType_t       OType;
    GPIO_Speed_t            Speed;
    GPIO_PullUpPullDown_t   Pull;
    uint8_t                 AltFunc;
} GPIO_PinConfig_t;

typedef struct{
    GPIO_TypeDef_t*     Instance; 
    GPIO_PinConfig_t    GPIO_PinConfig; 
} GPIO_Handle_t;

/*------------------------API--------------------------------*/

// GPIO Init and Deinit
GPIO_Status_t GPIO_Init(const GPIO_Handle_t* hGPIO);
// GPIO_Status_t GPIO_DeInit(GPIO_TypeDef_t* Port);

// GPIO Read & Write for Pin
void GPIO_WritePin(GPIO_TypeDef_t* Port, GPIO_Pin_t Pin, GPIO_PinState_t State);
GPIO_Pin_t GPIO_ReadPin(GPIO_TypeDef_t* Port, GPIO_Pin_t Pin);
void GPIO_TogglePin(GPIO_TypeDef_t* Port, GPIO_Pin_t Pin);

// GPIO Read & Write for Port
void GPIO_WritePort(GPIO_TypeDef_t* Port, uint16_t value);
uint16_t GPIO_ReadPort(const GPIO_TypeDef_t* Port);

// SetMode
void GPIO_SetMode(GPIO_TypeDef_t *GPIOx, uint16_t pins, uint32_t mode);
// GPIO Interrupt

#endif