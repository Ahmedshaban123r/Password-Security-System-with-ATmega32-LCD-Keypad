#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <avr/delay.h>
#define LCD_DATA_PORT PORTC
#define LCD_CTRL_PORT PORTA
#define RS_PIN PA0
#define RW_PIN PA1
#define E_PIN PA2

void LCD_command(unsigned char cmnd);
void LCD_data(unsigned char data);
void LCD_init();
void LCD_display_string(const char *str);
void LCD_display_char( const char *chr );
void LCD_goto(uint8_t row, uint8_t col);
void LCD_createChar(uint8_t loc, uint8_t *pattern) ;
void LCD_clear(void);
void LCD_gotoxy(uint8_t x, uint8_t y);
#endif /* LCD_H_ */