/*
 * Password Verification System
 * Combines keypad input with LCD display for password checking
 */

#include <LiquidCrystal.h>
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

// Keypad row pin connections
byte rowPins[ROWS] = {31, 33, 35, 37};

// Keypad column pin connections
byte colPins[COLS] = {23, 25, 27, 29};

// LCD pin configuration
const int RS_PIN = 13;  // Register select pin
const int EN_PIN = 12;  // Enable pin
const int D4_PIN = 8;   // Data pin 4
const int D5_PIN = 9;   // Data pin 5
const int D6_PIN = 10;  // Data pin 6
const int D7_PIN = 11;  // Data pin 7

// Password configuration
const String CORRECT_PASSWORD = "9831133";  // Predefined correct password
const char SUBMIT_KEY = '*';                // Key to submit password

// Initialize keypad and LCD objects
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

// Global variables
byte currentColumn = 0;     // Current cursor position on LCD
String inputPassword = "";  // Stores user input password

/**
 * Setup function - runs once at startup
 * Initializes serial communication and LCD display
 */
void setup() {
  Serial.begin(9600);       // Initialize serial for debugging
  lcd.begin(16, 2);         // Initialize 16x2 LCD display
  lcd.clear();              // Clear display on startup
}

/**
 * Main loop function - runs continuously
 * Handles keypad input and password verification
 */
void loop() {
  lcd.setCursor(currentColumn, 0);  // Set cursor position for input
  
  char key = keypad.getKey();       // Check for keypress
  
  if (key) {
    handleKeyPress(key);            // Process the pressed key
  }
}

/**
 * Processes key press events
 * @param key - The character of the pressed key
 */
void handleKeyPress(char key) {
  Serial.println(key);              // Output key to serial monitor
  lcd.print(key);                   // Display key on LCD
  
  // Check if submit key was pressed
  if (key == SUBMIT_KEY) {
    verifyPassword();               // Validate the input password
  }
  
  inputPassword += key;             // Add key to password string
  currentColumn++;                  // Move cursor to next position
}

/**
 * Verifies input password against correct password
 * Displays result on second line of LCD
 */
void verifyPassword() {
  lcd.setCursor(0, 1);             // Move to second line of LCD
  
  if (inputPassword == CORRECT_PASSWORD) {
    lcd.print("Correct password"); // Success message
  } else {
    lcd.print("Wrong password");   // Failure message
  }
}