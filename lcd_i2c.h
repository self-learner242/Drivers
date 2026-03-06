#ifndef LCD_I2C_H
#define LCD_I2C_H

#include <stdint.h>
#include "i2c.h"

#define LCD_I2C_ADDR   0x27

#define LCD_BACKLIGHT  0x08
#define LCD_ENABLE     0x04
#define LCD_RS         0x01

typedef struct {
    I2C_Handle_t *hi2c;
    uint8_t address;
} LCD_I2C_Handle_t;

/* APIs */
void LCD_Init(LCD_I2C_Handle_t *hlcd);
void LCD_SendCmd(LCD_I2C_Handle_t *hlcd, uint8_t cmd);
void LCD_SendData(LCD_I2C_Handle_t *hlcd, uint8_t data);
void LCD_SendString(LCD_I2C_Handle_t *hlcd, char *str);
void LCD_SetCursor(LCD_I2C_Handle_t *hlcd, uint8_t row, uint8_t col);
void LCD_Clear(LCD_I2C_Handle_t *hlcd);

#endif