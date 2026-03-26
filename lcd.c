#include "lcd.h"

/* Internal Function Prototype */
static void LCD_nibble_write(uint8_t data, uint8_t control);
static void delay_ms(int ms);


static void LCD_nibble_write(uint8_t data, uint8_t control){
		// Clear PC4–PC7
		GPIOC->BSRR = (0xF << 4) << 16;

		// Set new data
		GPIOC->BSRR = (data & 0xF0);
	
		if(control){
				// control = 1----> Data
				GPIOB->BSRR = 1 << RS_PIN;
		}
		else{
				// control = 0----> Cmd
				GPIOB->BSRR = (1 << RS_PIN) << 16;
		}
		
		// Pulse in EN
		GPIOB->BSRR = 1 << EN_PIN;
		delay_ms(1);
		GPIOB->BSRR = (1 << EN_PIN) << 16;
		delay_ms(1);
}

static void delay_ms(int ms){
		for(;ms > 0; ms--){
				for(int i = 0; i < 3195; i++){
						__NOP();
				}
		}
}

void GPIO_Init(){
		// Enable Clock
		RCC->AHB1ENR |= 1 << 1 | 1 << 2; // 0x06
	
		// Set all as OUTPUT
	
		// PB[5:7]
		GPIOB->MODER &= ~0xFC00;
		GPIOB->MODER |= 0x4400; // 0100 0100 0000 0000 
		GPIOB->BSRR = 0x00800000; // Set EN and RW to low
	
		// PC[7:4]
		GPIOC->MODER &= ~0xFF00;
		GPIOC->MODER |= 0x5500;
		
}

void LCD_Init(void){
		GPIO_Init();
	
		// Init Sequence
		delay_ms(30);
		LCD_nibble_write(0x30,0);
		delay_ms(5);
		LCD_nibble_write(0x30,0);
		delay_ms(1);
		LCD_nibble_write(0x30,0);
		delay_ms(1);
	
		// CONFIGURE LCD PARA
		LCD_nibble_write(0x20,0); // LCD 4 bit data mode
		delay_ms(1);
	
		LCD_SendCmd(0x28); // 4 bit mode, 2 - lines, 5 x 7 font
		LCD_SendCmd(0x06); // Move Cursor Home
		LCD_SendCmd(0x01); // Clear Screen, Move cursor home
		LCD_SendCmd(0x0F); // Turn on display and Blink Cursor
}

void LCD_SendCmd(uint8_t cmd) {
    LCD_nibble_write(cmd & 0xF0, 0);    // 4bit upper
    LCD_nibble_write(cmd << 4, 0);      // 4bit lower
    if (cmd < 4) delay_ms(2); else delay_ms(1);
}

void LCD_SendData(uint8_t data) {
    LCD_nibble_write(data & 0xF0, 1);
    LCD_nibble_write(data << 4, 1);
    delay_ms(1);
}

void LCD_SendString(char *str) {
    while(*str) {
        LCD_SendData(*str++);
    }
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 0) ? (0x80 + col) : (0xC0 + col);
    LCD_SendCmd(addr);
}