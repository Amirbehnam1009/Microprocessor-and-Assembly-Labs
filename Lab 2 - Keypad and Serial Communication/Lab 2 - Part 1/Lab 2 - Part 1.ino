/*
 * Keypad LED Controller
 * Demonstrates basic keypad input with LED output patterns
 * Based on original code by Alexander Brevig
 */

#include <Keypad.h>

// Keypad configuration
const byte ROWS = 4;    // Number of rows in keypad
const byte COLS = 4;    // Number of columns in keypad

// Keypad matrix layout
char keys[ROWS][COLS] = {
  {'7', '8', '9', '/'},
  {'4', '5', '6', '*'},
  {'1', '2', '3', '-'},
  {'o', '0', '=', '+'}
};

// Pin connections for keypad rows
byte rowPins[ROWS] = {22, 23, 24, 25};

// Pin connections for keypad columns  
byte colPins[COLS] = {26, 27, 28, 29};

// LED pin assignments (9 LEDs total)
const byte LED_PINS[9] = {A0, A1, A2, A3, A4, A5, A6, A7, A8};

// Keypad object initialization
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Setup function - runs once at startup
void setup() {
  // Initialize all LED pins as outputs
  for (byte i = 0; i < 9; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }
}

// Main loop function - runs continuously
void loop() {
  // Check for keypress
  char key = keypad.getKey();
  
  // Handle numeric key inputs (1-9)
  if (key >= '1' && key <= '9') {
    int keyNumber = key - '0';  // Convert char to integer (1-9)
    activateLedsSequentially(keyNumber);
  }
}

/**
 * Activates LEDs sequentially based on the pressed key number
 * @param count - Number of LEDs to activate (1-9)
 * For key '1': 1 LED, key '2': 2 LEDs, ..., key '9': 9 LEDs
 */
void activateLedsSequentially(int count) {
  // Turn on LEDs one by one with delay
  for (int i = 0; i < count; i++) {
    digitalWrite(LED_PINS[i], HIGH);
    delay(350);
  }
  
  // Turn off all LEDs simultaneously
  turnOffAllLeds();
}

/**
 * Turns off all LEDs
 */
void turnOffAllLeds() {
  for (byte i = 0; i < 9; i++) {
    digitalWrite(LED_PINS[i], LOW);
  }
}