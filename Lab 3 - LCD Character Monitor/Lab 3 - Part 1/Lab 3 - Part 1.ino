/*
 * LCD Text Scrolling Display
 * Displays text that moves across a 16x2 LCD screen
 */

#include <LiquidCrystal.h>

// LCD pin configuration
const int RS_PIN = 13;    // Register select pin
const int EN_PIN = 12;    // Enable pin
const int D4_PIN = 8;     // Data pin 4
const int D5_PIN = 9;     // Data pin 5
const int D6_PIN = 10;    // Data pin 6
const int D7_PIN = 11;    // Data pin 7

// Initialize LCD object with pin configuration
LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

// Cursor position variables
byte currentColumn = 0;   // Current horizontal position (0-15)
byte currentRow = 0;      // Current vertical position (0-1)

// Display text configuration
const char DISPLAY_TEXT[] = "Amir";  // Text to display
const int MAX_COLUMN = 12;           // Maximum column before reset

/**
 * Setup function - runs once at startup
 * Initializes LCD display and clears the screen
 */
void setup() {
  // Initialize LCD with 16 columns and 2 rows
  lcd.begin(16, 2);
  // Clear any existing content on the display
  lcd.clear();
}

/**
 * Main loop function - runs continuously
 * Updates LCD display with scrolling text effect
 */
void loop() {
  updateDisplay();    // Refresh the display with new position
  advanceCursor();    // Move cursor to next position
  delay(100);         // Control animation speed (100ms delay)
}

/**
 * Updates LCD display with text at current cursor position
 * Clears screen and prints text at configured location
 */
void updateDisplay() {
  lcd.clear();                // Clear the display
  lcd.setCursor(currentColumn, currentRow);  // Set cursor position
  lcd.print(DISPLAY_TEXT);    // Display the text
}

/**
 * Advances cursor position for scrolling effect
 * Wraps around columns and alternates between rows
 */
void advanceCursor() {
  currentColumn++;            // Move to next column
  
  // Reset position when reaching maximum column
  if (currentColumn == MAX_COLUMN) {
    currentColumn = 0;        // Reset to first column
    currentRow = 1 - currentRow;  // Alternate between rows (0 and 1)
  }
}