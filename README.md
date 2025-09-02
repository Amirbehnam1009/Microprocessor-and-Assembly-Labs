# 🧠 Microprocessor and Assembly Language Labs - Fall 99 / Spring 1400

![Platform](https://img.shields.io/badge/Platform-Arduino_MEGA_2560-00979D?logo=arduino)  ![Simulator](https://img.shields.io/badge/Simulator-Proteus_8-FF0000?logo=proteus)  ![Tool](https://img.shields.io/badge/Tool-Keil_%C2%B5Vision-1E90FF)  ![Code-C++](https://img.shields.io/badge/Code-C++-00599C?logo=cplusplus)  
![Code-Assembly](https://img.shields.io/badge/Code-Assembly-6E4C13)

A comprehensive collection of labs from the **Microprocessor and Assembly Language** course at **Amirkabir University of Technology (Tehran Polytechnic)**.  
This repository documents the practical implementation of embedded systems concepts using the **Arduino MEGA 2560** platform (simulated in Proteus), and later moving to low-level **ARM assembly programming** with the **SAM3X8E (Arduino Due)** in **Keil µVision**.

---

## 📚 Table of Contents

1. [Lab 1: GPIO and Input Interrupts](#-lab-1-gpio-and-input-interrupts)
2. [Lab 2: Keypad and Serial Communication](#-lab-2-keypad-and-serial-communication)  
3. [Lab 3: LCD Character Monitor](#-lab-3-lcd-character-monitor)  
4. [Lab 4: Servo Motor and Analog Input](#-lab-4-servo-motor-and-analog-input)  
5. [Lab 5: Relay Driver Circuit](#-lab-5-relay-driver-circuit)  
6. [Lab 6: Midterm Project - 🗝️ Safe Box](#-lab-6-midterm-project---safe-box)  
7. [Lab 7: EEPROM and TWI (I2C) Protocol](#-lab-7-eeprom-and-twi-i2c-protocol)  
8. [Lab 8: SPI Communication and Sensors](#-lab-8-spi-communication-and-sensors)  
9. [Lab 9: Piezo Speaker and Header Files](#-lab-9-piezo-speaker-and-header-files)  
10. [Lab 11: Final Project - 🎸 Electric Guitar](#-lab-11-final-project---electric-guitar)  
11. [Assembly Labs](#-assembly-labs)   

---

## 🧪 Lab 1: GPIO and Input Interrupts
**Concepts:** Digital I/O, Pull-up/Pull-down resistors, Button debouncing, Polling vs. Interrupt-driven programming.  
**Implementation:** Circuit with multiple LEDs and buttons. Sequential lighting, blinking patterns, and interrupt-driven input.  
**Key Components:** `pinMode()`, `digitalRead()`, `digitalWrite()`, `attachInterrupt()`.

---

## ⌨️ Lab 2: Keypad and Serial Communication
**Concepts:** Matrix keypad scanning, UART, ASCII handling.  
**Implementation:** Interfaced a 4×4 keypad, displayed input on Serial Monitor, oscilloscope analysis of TX/RX.  
**Key Components:** `Keypad.h`, `Serial.begin()`, `Serial.read()`, `Serial.parseInt()`, `Serial.println()`.

---

## 📟 Lab 3: LCD Character Monitor
**Concepts:** LCD interfacing, parallel communication, simple UI.  
**Implementation:** 16×2 LCD projects: scrolling text, password system, calculator, bouncing animation.  
**Key Components:** `LiquidCrystal.h`, `lcd.begin()`, `lcd.print()`, `lcd.setCursor()`, `lcd.scrollDisplayLeft()`.

---

## 🎛️ Lab 4: Servo Motor and Analog Input
**Concepts:** PWM, ADC, Servo control.  
**Implementation:** Servo controlled via potentiometer, auto sweep, and serial angle input.  
**Key Components:** `Servo.h`, `analogRead()`, `servo.attach()`, `servo.write()`, `map()`.

---

## ⚡ Lab 5: Relay Driver Circuit
**Concepts:** Relays, transistor switching, flyback diode, load driving.  
**Implementation:** Arduino-controlled transistor driver for a 5V relay, powering motor & LED.  
**Key Components:** Relay, NPN transistor, 1N4001 diode, resistors.

---

## 🗝️ Lab 6: Midterm Project - Safe Box
**Concepts:** Integration (LCD, Keypad, Servo, Buzzer, EEPROM), state machines, security logic.  
**Implementation:** Passcode-protected safe box with servo unlock, failed-attempt lockout, buzzer alerts, and LCD feedback.  
**Key Components:** `Keypad.h`, `LiquidCrystal.h`, `Servo.h`, `EEPROM.h`.

---

## 💾 Lab 7: EEPROM and TWI (I2C) Protocol
**Concepts:** I2C protocol, external EEPROM, persistent storage.  
**Implementation:** Washing machine simulation storing cycle settings in AT24C02 EEPROM.  
**Key Components:** `Wire.h`, `Wire.beginTransmission()`, `Wire.write()`, `Wire.endTransmission()`, `Wire.requestFrom()`.

---

## 🔄 Lab 8: SPI Communication and Sensors
**Concepts:** SPI protocol, master/slave communication, sensors.  
**Implementation:** Master Arduino reads LM35 temp + LDR light sensors, sends data to slaves via SPI.  
**Key Components:** `SPI.h`, `SPI.transfer()`, `analogRead()`, `map()`.

---

## 🎵 Lab 9: Piezo Speaker and Header Files
**Concepts:** PWM sound, tone generation, modular code with headers.  
**Implementation:** Music with piezo buzzer (`Jingle Bells`, `Ode to Joy`), dynamic pitch control.  
**Key Components:** `tone()`, `noTone()`, `#define`, arrays, custom headers.

---

## 🎸 Lab 11: Final Project - Electric Guitar
**Concepts:** PWM audio, multitasking simulation, input parsing.  
**Implementation:** Simulated polyphonic electric guitar with keypad frets, strum keys, and pitch-bend potentiometer.  
**Key Components:** `tone()`, `Keypad.h`, state machines, `map()`.

---

## ⚙️ Assembly Labs
**Toolchain:** Keil µVision  
**Target:** ARM Cortex-M3 (ATSAM3N Series)

- **Lab ASM1: Low-Level GPIO**  
  Assembly-only GPIO control, startup file study, delay loops.

- **Lab ASM2: Inline Assembly**  
  Recursive factorial with inline ASM in C, register management.

- **Bonus Projects:**  
  - Stopwatch with SysTick + 7-seg display  
  - LED Matrix scrolling text ("HELLO") with POV multiplexing  

---

## 🛠️ Setup & Tools

1. **Simulator:** Proteus 8 Professional  
2. **IDE (Arduino):** Arduino IDE / VS Code + PlatformIO  
3. **IDE (Assembly):** Keil MDK-ARM (µVision)  
4. **Libraries:** Included in Arduino IDE (`LiquidCrystal`, `Servo`, `Keypad`, `Wire`, `SPI`)  
   - Install **Keypad library** manually if missing.

---

## 📁 Repository Structure

``` bash
Microprocessor-Lab-Fall99/
├── Lab_01_GPIO_Interrupt/
├── Lab_02_Keypad_Serial/
├── Lab_03_LCD_Display/
├── Lab_04_Servo_Analog/
├── Lab_05_Relay_Driver/
├── Lab_06_Midterm_SafeBox/ # 🗝️ My Implementation
├── Lab_07_EEPROM_TWI/
├── Lab_08_SPI_Sensors/
├── Lab_09_Piezo_Headers/
├── Lab_11_Final_Guitar/ # 🎸 My Implementation
├── ASM_Labs/
│ ├── Lab_ASM1_GPIO/
│ ├── Lab_ASM2_Inline/
│ └── Bonus_Projects/
└── README.md
```
