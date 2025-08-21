/*
 * keypad.h
 *
 * Created: 8/12/2025 3:08:20 PM
 *  Author: ASUS
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <avr/io.h>        // Include AVR input/output library for I/O operations
#include <util/delay.h>    // Include delay utility for timing functions

// Define the port, pin, and data direction register for the keypad
#define KEYPAD_PORT     PORTB
#define KEYPAD_PIN      PINB
#define KEYPAD_DDR      DDRB

// Define masks for rows and columns of the keypad
#define KEYPAD_ROW_MASK 0x0F    // Mask for rows (lower 4 bits)
#define KEYPAD_COL_MASK 0xF0    // Mask for columns (upper 4 bits)

// Define the keypad layout in a 4x4 matrix
static char keypad[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};



void keypad_init(void);
// Function to read the pressed key on the keypad
char readKeypad();

// Function to scan the keypad and return the pressed key
char keypad_scan();



#endif /* KEYPAD_H_ */