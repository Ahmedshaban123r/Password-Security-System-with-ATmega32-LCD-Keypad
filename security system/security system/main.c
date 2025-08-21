#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "LCD.h"
#include "keypad.h"

// ================= Employee Struct =================
typedef struct {
	uint8_t id;
	char name[10];
	char password[6];
} Employee;

Employee employees[10];   // max 10 users in RAM

// Master admin key
char masterKey[] = "1234";

// ================= LED Macros =================
#define GREEN_LED PD0
#define RED_LED   PD1

void LED_init() {
	DDRD |= (1<<GREEN_LED) | (1<<RED_LED);
	PORTD &= ~((1<<GREEN_LED) | (1<<RED_LED));
}

void LED_success() {
	PORTD |= (1<<GREEN_LED);
	PORTD &= ~(1<<RED_LED);
}

void LED_fail() {
	PORTD |= (1<<RED_LED);
	PORTD &= ~(1<<GREEN_LED);
}

// ================= Init Employees =================
void initEmployees() {
	employees[0].id = 1;
	strcpy(employees[0].name, "Shaban");
	strcpy(employees[0].password, "1111");

	employees[1].id = 2;
	strcpy(employees[1].name, "Basant");
	strcpy(employees[1].password, "2222");

	employees[2].id = 3;
	strcpy(employees[2].name, "Sara");
	strcpy(employees[2].password, "3333");

	for (int i=3; i<10; i++) {
		employees[i].id = 0;
		strcpy(employees[i].name, "");
		strcpy(employees[i].password, "");
	}
}

// ================= Helpers =================
int findEmptySlot() {
	for (int i=0; i<10; i++) {
		if (employees[i].id == 0) return i;
	}
	return -1;
}

int findByID(uint8_t id) {
	for (int i=0; i<10; i++) {
		if (employees[i].id == id) return i;
	}
	return -1;
}

// ================= Keypad Input =================
void getInput(char *buffer, uint8_t maxLen) {
	uint8_t index = 0;
	char key;
	while (1) {
		key = readKeypad();
		if (key != 0) {
			if (key == '#') {  // confirm
				buffer[index] = '\0';
				break;
				} else if (key == '*' && index > 0) { // backspace
				index--;
				buffer[index] = '\0';
				LCD_command(0x10);
				LCD_data(' ');
				LCD_command(0x10);
				} else if (index < maxLen-1 && key >= '0' && key <= '9') {
				buffer[index++] = key;
				LCD_data('*'); // hide input
			}
		}
	}
}

// ================= Menu Functions =================
void addUser() {
	int slot = findEmptySlot();
	if (slot == -1) {
		LCD_clear();
		LCD_display_string("No space!");
		_delay_ms(1000);
		return;
	}

	employees[slot].id = slot+1;
	snprintf(employees[slot].name, sizeof(employees[slot].name), "User%d", slot+1);

	LCD_clear();
	LCD_display_string("New Pass:");
	LCD_gotoxy(0,1);

	getInput(employees[slot].password, sizeof(employees[slot].password));

	LCD_clear();
	LCD_display_string("User Added!");
	_delay_ms(1000);
}

void editPassword() {
	char input[6];
	LCD_clear();
	LCD_display_string("Enter ID:");
	LCD_gotoxy(0,1);
	getInput(input, sizeof(input));
	uint8_t id = atoi(input);

	int index = findByID(id);
	if (index == -1) {
		LCD_clear();
		LCD_display_string("ID Not Found");
		_delay_ms(1000);
		return;
	}

	LCD_clear();
	LCD_display_string("New Pass:");
	LCD_gotoxy(0,1);
	getInput(employees[index].password, sizeof(employees[index].password));

	LCD_clear();
	LCD_display_string("Updated!");
	_delay_ms(1000);
}

void deleteUser() {
	char input[6];
	LCD_clear();
	LCD_display_string("Enter ID:");
	LCD_gotoxy(0,1);
	getInput(input, sizeof(input));
	uint8_t id = atoi(input);

	int index = findByID(id);
	if (index == -1) {
		LCD_clear();
		LCD_display_string("ID Not Found");
		_delay_ms(1000);
		return;
	}

	employees[index].id = 0;
	strcpy(employees[index].name, "");
	strcpy(employees[index].password, "");

	LCD_clear();
	LCD_display_string("Deleted!");
	_delay_ms(1000);
}

void displayUsers() {
	for (int i=0; i<10; i++) {
		if (employees[i].id != 0) {
			LCD_clear();
			char buf[16];
			snprintf(buf, sizeof(buf), "ID:%d N:%s", employees[i].id, employees[i].name);
			LCD_display_string(buf);
			_delay_ms(2000);
		}
	}
}

// ================= User Login =================
void userLogin() {
	char input[6];
	uint8_t tries = 0;

	while (1) {
		LCD_clear();
		LCD_display_string("Enter User ID:");
		LCD_gotoxy(0,1);
		getInput(input, sizeof(input));
		uint8_t id = atoi(input);

		int index = findByID(id);
		if (index == -1) {
			LCD_clear();
			LCD_display_string("ID Not Found");
			_delay_ms(1000);
			continue;
		}

		LCD_clear();
		LCD_display_string("Enter Pass:");
		LCD_gotoxy(0,1);
		getInput(input, sizeof(input));

		if (strcmp(input, employees[index].password) == 0) {
			LCD_clear();
			char buf[16];
			snprintf(buf, sizeof(buf), "Welcome %s", employees[index].name);
			LCD_display_string(buf);
			LED_success();
			_delay_ms(3000);
			return;
			} else {
			tries++;
			LCD_clear();
			LCD_display_string("Wrong Pass!");
			LED_fail();
			_delay_ms(1000);
			if (tries >= 3) {
				LCD_clear();
				LCD_display_string("User Blocked!");
				while(1); // system lock
			}
		}
	}
}

// ================= Main =================
int main(void) {
	LCD_init();
	keypad_init();
	LED_init();
	initEmployees();

	char input[10];
	uint8_t tries = 0;

	// Step 1: Admin login
	while (1) {
		LCD_clear();
		LCD_display_string("Enter Master Key:");
		LCD_gotoxy(0,1);
		getInput(input, sizeof(input));

		if (strcmp(input, masterKey) == 0) {
			LCD_clear();
			LCD_display_string("Master OK!");
			LED_success();
			_delay_ms(1000);
			break;
			} else {
			tries++;
			LCD_clear();
			LCD_display_string("Wrong Key!");
			LED_fail();
			_delay_ms(1000);
			if (tries >= 3) {
				LCD_clear();
				LCD_display_string("Access Denied!");
				while(1); // lock system
			}
		}
	}

	// Step 2: Admin menu
	while (1) {
		LCD_clear();
		LCD_display_string("A:Add B:Edit");
		LCD_gotoxy(0,1);
		LCD_display_string("C:Del D:List");

		char opt = readKeypad();
		if (opt == 'A') addUser();
		else if (opt == 'B') editPassword();
		else if (opt == 'C') deleteUser();
		else if (opt == 'D') displayUsers();
		else if (opt == '#') break; // Press # to exit admin menu
	}

	// Step 3: User login mode
	userLogin();

	while (1); // system idle
}
