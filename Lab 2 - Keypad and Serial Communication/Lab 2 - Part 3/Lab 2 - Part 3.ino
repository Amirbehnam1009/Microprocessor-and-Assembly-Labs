/*
 * Keypad LED Sequencer
 * Demonstrates keypad input with sequential LED activation patterns
 * Based on original code by Alexander Brevig
 */

#include <Keypad.h>

// Keypad configuration
const byte ROWS = 4;    // Number of rows in keypad matrix
const byte COLS = 4;    // Number of columns in keypad matrix

// Keypad character layout
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

// LED pin assignments (9 LEDs total)
const byte LED_PINS[9] = {A0, A1, A2, A3, A4, A5, A6, A7, A8};

// Keypad object initialization
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

/**
 * Setup function - runs once at program start
 * Initializes serial communication and configures LED pins as outputs
 */
void setup() {
  Serial.begin(9600);  // Initialize serial communication for debugging
  
  // Configure all LED pins as outputs
  for (byte i = 0; i < 9; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }
}

/**
 * Main program loop - runs continuously
 * Checks for keypad input and processes numeric key presses
 */
void loop() {
  char key = keypad.getKey();  // Read keypad for pressed keys
  
  // Process numeric keys 1-9 for LED sequences
  if (key >= '1' && key <= '9') {
    handleNumericKey(key);
  }
}

/**
 * Handles numeric key presses by activating LED sequences
 * @param key - The numeric key character ('1' through '9')
 */
void handleNumericKey(char key) {
  int keyNumber = key - '0';  // Convert char to integer (1-9)
  
  Serial.println(key);  // Output pressed key to serial monitor
  
  // Execute LED sequence based on key number
  executeLedSequence(keyNumber);
}

/**
 * Executes LED activation sequence for specified number of LEDs
 * @param ledCount - Number of LEDs to activate sequentially (1-9)
 */
void executeLedSequence(int ledCount) {
  // Turn on LEDs one by one with delay between each
  for (int i = 0; i < ledCount; i++) {
    digitalWrite(LED_PINS[i], HIGH);
    delay(350);
  }
  
  // Turn off all LEDs simultaneously after sequence completes
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