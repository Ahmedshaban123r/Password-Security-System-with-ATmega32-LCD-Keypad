# 🔐 Password Security System with ATmega32, LCD & Keypad

## 📖 Overview
This project implements a **password-protected security system** using:
- **ATmega32A microcontroller**
- **16x2 LCD display**
- **4x4 Keypad**
- **Green & Red LEDs** for status indication  

The system allows an **Admin (Master Key access)** to manage employees and their passwords, and regular users can log in with their credentials.

---

## ⚡ Features
- **Master Key Security**  
  - Admin login with **3 trials only**.  
  - On failure, system exits.  

- **Employee Management** (via keypad options):  
  - `A`: Add a new user (auto ID, random name placeholder, user sets password).  
  - `B`: Edit an existing user’s password.  
  - `C`: Delete a user.  
  - `D`: Display all user IDs & passwords on LCD.  

- **User Login Mode**  
  - Users can log in with their **ID and password**.  
  - `*` for **backspace** and `#` to **confirm input**.  
  - Green LED → login success.  
  - Red LED → login failure.  

---

## 🛠 Hardware Requirements
- ATmega32A microcontroller  
- 16x2 LCD Display  
- 4x4 Key
- /Password-Security-System
│── main.c # Main system logic
│── keypad.h/.c # Keypad driver
│── lcd.h/.c # LCD driver
│── Makefile # For compiling with avr-gcc
│── README.md # Project documentation



---

## 🚀 How It Works
1. System starts → asks for **Admin Master Key**.  
2. If correct → Admin menu opens (A/B/C/D).  
3. Admin can add/edit/delete users or display user list.  
4. In user login mode →  
   - Enter **ID**, then **password**.  
   - Confirm with `#`.  
   - System checks credentials → shows success/failure on LCD & LEDs.  

---

## 🎮 Keypad Controls
- `*` → Backspace  
- `#` → Confirm input  
- `A` → Add user  
- `B` → Edit password  
- `C` → Delete user  
- `D` → Display users  

---

## 📸 Demo
<img width="1245" height="872" alt="image" src="https://github.com/user-attachments/assets/ad4cfef8-a152-471d-90f4-66fcf18078b4" />


---

## ⚙️ Compilation & Upload
1. Compile with **avr-gcc**:
   ```sh
   avr-gcc -mmcu=atmega32 -Os main.c lcd.c keypad.c -o security.elf
   avr-objcopy -O ihex -R .eeprom security.elf security.hex

