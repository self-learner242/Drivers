#include "lcd_i2c.h"

static void LCD_Delay(volatile uint32_t t)
{
    while(t--);
}

static void LCD_Write(LCD_I2C_Handle_t *hlcd, uint8_t data)
{
    I2C_Master_Transmit(hlcd->hi2c, hlcd->address, &data, 1, 20);
}

static void LCD_WriteInitNibble(LCD_I2C_Handle_t *hlcd, uint8_t nibble)
{
    uint8_t data = (nibble & 0xF0) | LCD_BACKLIGHT;

    LCD_Write(hlcd, data | LCD_ENABLE);
    LCD_Delay(5000);
    LCD_Write(hlcd, data & ~LCD_ENABLE);
    LCD_Delay(5000);
}

static void LCD_Write4Bit(LCD_I2C_Handle_t *hlcd, uint8_t nibble, uint8_t control)
{
    uint8_t data = (nibble & 0xF0) | control | LCD_BACKLIGHT;

    // EN = 1
    LCD_Write(hlcd, data | LCD_ENABLE);
    LCD_Delay(2000);

    // EN = 0
    LCD_Write(hlcd, data & ~LCD_ENABLE);
    LCD_Delay(2000);
}

void LCD_SendCmd(LCD_I2C_Handle_t *hlcd, uint8_t cmd)
{
    LCD_Write4Bit(hlcd, cmd & 0xF0, 0x00);
    LCD_Write4Bit(hlcd, (cmd << 4) & 0xF0, 0x00);

    if(cmd == 0x01 || cmd == 0x02)
        LCD_Delay(200000);
    else
        LCD_Delay(50000);
}

void LCD_SendData(LCD_I2C_Handle_t *hlcd, uint8_t data)
{
    LCD_Write4Bit(hlcd, data & 0xF0, LCD_RS);
    LCD_Write4Bit(hlcd, (data << 4) & 0xF0, LCD_RS);
    LCD_Delay(50000);
}

void LCD_Init(LCD_I2C_Handle_t *hlcd)
{
    LCD_Delay(5000000);

    LCD_WriteInitNibble(hlcd, 0x30);
    LCD_Delay(2000000);

    LCD_WriteInitNibble(hlcd, 0x30);
    LCD_Delay(2000000);

    LCD_WriteInitNibble(hlcd, 0x30);
    LCD_Delay(2000000);

    LCD_WriteInitNibble(hlcd, 0x20);
    LCD_Delay(2000000);

    LCD_SendCmd(hlcd, 0x28);  // 2-line mode
    LCD_SendCmd(hlcd, 0x0C);
    LCD_SendCmd(hlcd, 0x06);
    LCD_SendCmd(hlcd, 0x01);
}

void LCD_SendString(LCD_I2C_Handle_t *hlcd, char *str)
{
    while(*str)
        LCD_SendData(hlcd, *str++);
}

void LCD_SetCursor(LCD_I2C_Handle_t *hlcd, uint8_t row, uint8_t col){ 
    uint8_t addr = (row == 0) ? (0x80 + col) : (0xC0 + col); 
    LCD_SendCmd(hlcd, addr); 
}

void LCD_Clear(LCD_I2C_Handle_t *hlcd)
{
    LCD_SendCmd(hlcd, 0x01);
    LCD_Delay(200000);
}