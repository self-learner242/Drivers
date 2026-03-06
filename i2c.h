#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stdlib.h>

/* BASE ADDRESS */
#define I2C1_BASE       0x40005400
#define I2C2_BASE       0x40005800
#define I2C3_BASE       0x40005C00

/* STATUS */

typedef enum {
    I2C_OK = 0,
    I2C_ERROR,
    I2C_BUSY,
    I2C_TIMEOUT,
    I2C_ACK_FAIL
} I2C_Status_t;

/* REGISTERS */
typedef struct{
    volatile uint32_t   CR1;
    volatile uint32_t   CR2;
    volatile uint32_t   OAR1;
    volatile uint32_t   OAR2;
    volatile uint32_t   DR;
    volatile uint32_t   SR1;
    volatile uint32_t   SR2;
    volatile uint32_t   CCR;
    volatile uint32_t   TRISE;
    volatile uint32_t   FLTR;
} I2C_TypeDef_t;

/* INSTANCE */
#define I2C1        ((I2C_TypeDef_t*)I2C1_BASE)
#define I2C2        ((I2C_TypeDef_t*)I2C2_BASE)
#define I2C3        ((I2C_TypeDef_t*)I2C3_BASE)

/* MASTER */

typedef enum{
    I2C_MASTER_MODE_SM = 0,
    I2C_MASTER_MODE_FM
} I2C_MasterModeSel;

typedef enum{
    I2C_MASTER_FM_DUTY_2        = 0, // Tlow / Thigh = 2
    I2C_MASTER_FM_DUTY_16_9          // Tlow / Thigh = 16/9
} I2C_Master_FM_Duty;

/* SLAVE */

/* CONFIG */

typedef struct {
    uint32_t    PeriInputClock;   
    uint32_t    ClockSpeed;  
    I2C_MasterModeSel MasterMode;
    I2C_Master_FM_Duty DutyCycle;
} I2C_Config_t;

/* HANDLE*/
typedef struct{
    I2C_TypeDef_t*  Instance;
    I2C_Config_t    Init;
} I2C_Handle_t;

/* BIT MASKS */
// CR1
#define I2C_CR1_PE          (1 << 0)
#define I2C_CR1_START       (1 << 8)
#define I2C_CR1_STOP        (1 << 9)
#define I2C_CR1_ACK         (1 << 10)
#define I2C_CR1_SWRST       (1 << 15)
// CR2

// SR1
#define I2C_SR1_SB          (1 << 0)
#define I2C_SR1_ADDR        (1 << 1)
#define I2C_SR1_BTF         (1 << 2)
#define I2C_SR1_RXNE        (1 << 6)
#define I2C_SR1_TXE         (1 << 7)
#define I2C_SR1_AF          (1 << 10)

// SR2
#define I2C_SR2_MSL         (1 << 0)
#define I2C_SR2_BUSY        (1 << 1)

// CCR
#define I2C_CCR_DUTY        (1 << 14)
#define I2C_CCR_FS          (1 << 15)

/* APIs */ 
// Currently support for Master Only
I2C_Status_t I2C_Init(I2C_Handle_t* hi2c);
I2C_Status_t I2C_Master_Transmit(I2C_Handle_t* hi2c, uint8_t slaveAddr, uint8_t *pdata, uint16_t size, uint32_t timeout);
I2C_Status_t I2C_Master_Receive(I2C_Handle_t* hi2c, uint8_t slaveAddr, uint8_t *pdata, uint16_t size, uint32_t timeout);


#endif