/*
 * Basic Keypad Input Demo
 * Demonstrates simple keypad input with serial output
 * Based on original code by Alexander Brevig
 */

#include <Keypad.h>

// Keypad configuration constants
const byte ROWS = 4;    // Number of rows in the keypad matrix
const byte COLS = 4;    // Number of columns in the keypad matrix

// Keypad character matrix layout
char keys[ROWS][COLS] = {
  {'7', '8', '9', '/'},
  {'4', '5', '6', '*'},
  {'1', '2', '3', '-'},
  {'o', '0', '=', '+'}
};

// Microcontroller pin connections for keypad rows
byte rowPins[ROWS] = {22, 23, 24, 25};

// Microcontroller pin connections for keypad columns
byte colPins[COLS] = {26, 27, 28, 29};

// Initialize keypad object with configuration
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

/**
 * Setup function - runs once at program start
 * Initializes serial communication for debugging output
 */
void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud rate
}

/**
 * Main program loop - runs continuously
 * Checks for keypad input and processes any detected key presses
 */
void loop() {
  // Read keypad for any pressed keys
  char key = keypad.getKey();
  
  // If a valid key is detected, process it
  if (key) {
    handleKeyPress(key);
  }
}

/**
 * Handles key press events by printing the key value to serial monitor
 * @param key - The character value of the pressed key
 */
void handleKeyPress(char key) {
  Serial.println(key);  // Output the pressed key to serial monitor
}