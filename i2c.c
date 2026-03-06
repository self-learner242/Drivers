#include "i2c.h"
#include "systick.h"

/* FUNCTION PROTOTYPE */
static I2C_Status_t I2C_WaitFlag(I2C_Handle_t* hi2c, volatile uint32_t *reg, uint32_t flag, uint32_t tickstart,  uint32_t timeout);

/* Main APIs*/
I2C_Status_t I2C_Init(I2C_Handle_t* hi2c){
    /*0. Check if hi2c NULL */
    if(hi2c == NULL || hi2c->Instance == NULL) return I2C_ERROR;

    /* 1. Reset I2C */
    hi2c->Instance->CR1 |= I2C_CR1_SWRST;
    hi2c->Instance->CR1 &= ~I2C_CR1_SWRST;

    /* 2. Peripheral input clock in I2C_CR2 */
    hi2c->Instance->CR2 &= ~0x3F;
    hi2c->Instance->CR2 |= hi2c->Init.PeriInputClock & 0x3F;    

    /* 3 & 4. Configure CCR and TRISE*/
    uint32_t pclk = hi2c->Init.PeriInputClock * 1000000U;

    if(hi2c->Init.ClockSpeed <= 100000U)
    {
        /* Standard mode */
        hi2c->Instance->CCR = pclk / (2U * hi2c->Init.ClockSpeed);
        hi2c->Instance->TRISE = hi2c->Init.PeriInputClock + 1;
    }
    else
    {
        /* Fast mode */
        hi2c->Instance->CCR &= ~(I2C_CCR_FS | I2C_CCR_DUTY);

        if(hi2c->Init.DutyCycle == I2C_MASTER_FM_DUTY_2){
            hi2c->Instance->CCR = (pclk / (3U * hi2c->Init.ClockSpeed)) | I2C_CCR_FS;
        }
        else{
            hi2c->Instance->CCR =
            (pclk / (25U * hi2c->Init.ClockSpeed)) | I2C_CCR_FS | I2C_CCR_DUTY;
        }

        /* TRISE for FM */
        hi2c->Instance->TRISE = ((hi2c->Init.PeriInputClock * 300) / 1000) + 1;
    }

    /* 5. Enable I2C */
    hi2c->Instance->CR1 |= I2C_CR1_PE;

    /**/
    return I2C_OK;
}

I2C_Status_t I2C_Master_Transmit(I2C_Handle_t* hi2c, uint8_t slaveAddr, uint8_t *pdata, uint16_t size, uint32_t timeout){

    if(hi2c == NULL || pdata == NULL) return I2C_ERROR;

    uint32_t tickstart = SysTick_GetTick();

    /* 1. Wait BUSY clear ( = 0) */
    while(hi2c->Instance->SR2 & I2C_SR2_BUSY)
        if((SysTick_GetTick() - tickstart) >= timeout)
            return I2C_TIMEOUT;

    /* 2. Generate START */
    hi2c->Instance->CR1 |= I2C_CR1_START;

    /* 3. Wait SB */
    if(I2C_WaitFlag(hi2c, &hi2c->Instance->SR1, I2C_SR1_SB, tickstart, timeout) != I2C_OK)
        goto error;

    /* 4. Send Address */
    hi2c->Instance->DR = (slaveAddr << 1);

    /* 5. Wait ADDR */
    if(I2C_WaitFlag(hi2c, &hi2c->Instance->SR1, I2C_SR1_ADDR, tickstart, timeout) != I2C_OK)
        goto error;

    /* 6. Clear ADDR */
    volatile uint32_t tmp;
    tmp = hi2c->Instance->SR1;
    tmp = hi2c->Instance->SR2;
    (void)tmp;

    /* 7. Send data */
    for(uint16_t i = 0; i < size; i++)
    {
        if(I2C_WaitFlag(hi2c, &hi2c->Instance->SR1, I2C_SR1_TXE, tickstart, timeout) != I2C_OK) // Wait TXE
            goto error;

        hi2c->Instance->DR = pdata[i]; // Wait DR
    }

    /* 8. Wait BTF */
    if(I2C_WaitFlag(hi2c, &hi2c->Instance->SR1, I2C_SR1_BTF, tickstart, timeout) != I2C_OK)
        goto error;

    /* 9. STOP */
    hi2c->Instance->CR1 |= I2C_CR1_STOP;

    return I2C_OK;

error:
    hi2c->Instance->CR1 |= I2C_CR1_STOP;
    return I2C_TIMEOUT;
}

// * Support 1 byte only
I2C_Status_t I2C_Master_Receive(I2C_Handle_t* hi2c, uint8_t addr, uint8_t *pdata, uint16_t size, uint32_t timeout){
    if(hi2c == NULL || pdata == NULL || size == 0) return I2C_ERROR;

    uint32_t tickstart = SysTick_GetTick();

    /*1. Wait BUSY clear(= 0)*/
    while(hi2c->Instance->SR2 & I2C_SR2_BUSY){
        if((SysTick_GetTick() - tickstart) >= timeout){
            return I2C_TIMEOUT;
        }
    }

    /*2. Enable ACK*/
    hi2c->Instance->CR1 |= I2C_CR1_ACK;

    /*3. Generate START*/
    hi2c->Instance->CR1 |= I2C_CR1_START;

    if(I2C_WaitFlag(hi2c, &hi2c->Instance->SR1, I2C_SR1_SB, tickstart, timeout) != I2C_OK)
        goto error;

    /*4. Send address + Read */
    hi2c->Instance->DR = (addr << 1) | 0x01;

    if(I2C_WaitFlag(hi2c, &hi2c->Instance->SR1, I2C_SR1_ADDR, tickstart, timeout) != I2C_OK)
        goto error;

    // Special case: Size == 1
    if(size == 1){
        hi2c->Instance->CR1 &= ~I2C_CR1_ACK;

        volatile uint32_t tmp;
        tmp = hi2c->Instance->SR1;
        tmp = hi2c->Instance->SR2;
        (void)tmp;

        hi2c->Instance->CR1 |= I2C_CR1_STOP;

        if(I2C_WaitFlag(hi2c, &hi2c->Instance->SR1, I2C_SR1_RXNE, tickstart, timeout) != I2C_OK)
            goto error;

        pdata[0] = hi2c->Instance->DR;

        return I2C_OK;
    }
    /*5. Clear Address*/

    volatile uint32_t tmp;
    tmp = hi2c->Instance->SR1;
    tmp = hi2c->Instance->SR2;
    (void)tmp;

    // Size > 1

    for(uint16_t i = 0; i < size; i++){
        if(i == size - 1){
            hi2c->Instance->CR1 &= ~I2C_CR1_ACK;
            hi2c->Instance->CR1 |= I2C_CR1_STOP;
        }

        if(I2C_WaitFlag(hi2c, &hi2c->Instance->SR1, I2C_SR1_RXNE, tickstart, timeout) != I2C_OK)
            goto error;

        pdata[i] = hi2c->Instance->DR;
    }

    return I2C_OK;

error:
    hi2c->Instance->CR1 |= I2C_CR1_STOP;
    return I2C_TIMEOUT;
}


/* Internal Implementation */
static I2C_Status_t I2C_WaitFlag(I2C_Handle_t* hi2c, volatile uint32_t *reg, uint32_t flag, uint32_t tickstart,  uint32_t timeout){
    while(!(*reg & flag))
    {
        /* Detect ACK failure */
        if(hi2c->Instance->SR1 & I2C_SR1_AF)
        {
            hi2c->Instance->SR1 &= ~I2C_SR1_AF;
            return I2C_ACK_FAIL;
        }

        if((SysTick_GetTick() - tickstart) >= timeout)
            return I2C_TIMEOUT;
    }
    return I2C_OK;
}
