
#include "LCD.h"

void LCD_command(unsigned char cmnd)
{
	LCD_DATA_PORT = cmnd; // Send the command to the data port
	LCD_CTRL_PORT &= ~(1 << RS_PIN); // RS = 0 for command
	LCD_CTRL_PORT &= ~(1 << RW_PIN); // RW = 0 for write
	LCD_CTRL_PORT |= (1 << E_PIN); // Enable high
	_delay_ms(1);
	LCD_CTRL_PORT &= ~(1 << E_PIN); // Enable low
	_delay_ms(1);
}

void LCD_data(unsigned char data)
{
	LCD_DATA_PORT = data; // Send the data to the data port
	LCD_CTRL_PORT |= (1 << RS_PIN); // RS = 1 for data
	LCD_CTRL_PORT &= ~(1 << RW_PIN); // RW = 0 for write
	LCD_CTRL_PORT |= (1 << E_PIN); // Enable high
	_delay_ms(1);
	LCD_CTRL_PORT &= ~(1 << E_PIN); // Enable low
	_delay_ms(1);
}

void LCD_init()
{
	// Set data and control ports as outputs
	DDRC = 0xFF; // Data port
	DDRA |= (1 << RS_PIN) | (1 << RW_PIN) | (1 << E_PIN); // Control port
	_delay_ms(20); // LCD Power ON delay
	LCD_command(0x38); // 2 lines, 5x7 matrix
	LCD_command(0x0E); // Display on, cursor on
	LCD_command(0x01); // Clear display
	_delay_ms(2);
}

void LCD_display_string(const char *str)
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		LCD_data(str[i]);//A--> N--> A--> S--> \0 BREAK
		_delay_ms(100);
	}
}
void LCD_display_char( const char *chr )

{
	LCD_data(chr);
}
void LCD_goto(uint8_t row, uint8_t col) {
	uint8_t address;
	if (row == 0)
	address = 0x80 + col;
	else
	address = 0xC0 + col;
	LCD_command(address);
}
void LCD_createChar(uint8_t loc, uint8_t *pattern) {
	loc &= 0x07; // Only 8 locations (0-7)
	LCD_command(0x40 + (loc * 8)); // Set CGRAM address
	for (uint8_t i = 0; i < 8; i++) {
		LCD_data(pattern[i]);
		_delay_ms(200);
	}
}
void LCD_clear(void)
{
	LCD_command(0x01); // Clear display command
	_delay_ms(10);
}

void LCD_gotoxy(uint8_t x, uint8_t y)
{
	uint8_t pos;
	if (y == 0)
	pos = 0x80 + x; // First line
	else
	pos = 0xC0 + x; // Second line
	LCD_command(pos);
}