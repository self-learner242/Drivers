#include "stm32f407xx.h"
#include <stdint.h>

#define LCD_DATA_PORT		GPIOC
#define LCD_CTRL_PORT		GPIOB

/* 4bit Data mode: PC[4:7] */
#define LCD_D4_PIN       4 
#define LCD_D5_PIN       5
#define LCD_D6_PIN       6
#define LCD_D7_PIN       7

/* CTRL PIN */
#define RS_PIN					 5 // PB5
/* #define RW_PIN					 6 // PB6 to GND in 4bit mode */
#define EN_PIN					 7 // PB7

/* COMMON CMD */
#define LCD_CMD_CLEAR_DISPLAY		0x01
#define LCD_CMD_RETURN_HOME			0x02
#define LCD_CMD_ADDR_LINE1      0x80
#define LCD_CMD_ADDR_LINE2      0xC0

/* Public APIs */
void LCD_Init(void);
void LCD_SendCmd(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_SendString(char *str);
void LCD_SetCursor(uint8_t row, uint8_t col);
