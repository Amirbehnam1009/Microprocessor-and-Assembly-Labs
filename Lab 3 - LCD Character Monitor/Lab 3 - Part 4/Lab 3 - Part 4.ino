/*
 * LCD Animation Display
 * Creates a diagonal animation pattern on a 16x2 LCD display
 */

#include <Keypad.h>
#include <LiquidCrystal.h>

// Keypad configuration (unused in current animation but preserved)
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

// Animation configuration
const char ANIMATION_CHAR = 'A';  // Character to display
const int ANIMATION_DELAY = 100;  // Delay between frames in milliseconds
const int LCD_COLUMNS = 16;       // Number of LCD columns
const int LCD_ROWS = 2;           // Number of LCD rows

// Initialize keypad and LCD objects
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

// Animation coordinates structure
struct Coordinate {
  int col;
  int row;
};

/**
 * Setup function - runs once at startup
 * Initializes LCD display and clears the screen
 */
void setup() {
  lcd.begin(LCD_COLUMNS, LCD_ROWS);  // Initialize LCD with 16 columns and 2 rows
  lcd.clear();                       // Clear display
  lcd.setCursor(0, 0);               // Set initial cursor position
  lcd.setCursor(0, 1);               // Set cursor to second row
  delay(300);                        // Initial delay
  lcd.clear();                       // Clear display again
}

/**
 * Main loop function - runs continuously
 * Executes the diagonal animation pattern
 */
void loop() {
  runDiagonalAnimation();  // Execute the animation sequence
}

/**
 * Creates a diagonal animation pattern across the LCD display
 * Moves from top-left to bottom-right with alternating rows
 */
void runDiagonalAnimation() {
  // Define animation coordinate sequence
  Coordinate animationPath[] = {
    {0, 0}, {1, 1}, {2, 0}, {3, 1}, {4, 0}, 
    {1, 5}, {3, 3}, {4, 3}, {4, 4}, {5, 4},
    {5, 5}, {6, 5}, {6, 6}, {7, 6}, {7, 7},
    {8, 7}, {8, 8}, {9, 8}, {9, 9}, {10, 9},
    {10, 10}, {11, 10}, {11, 11}, {12, 11}, {12, 12},
    {13, 12}, {13, 13}, {14, 13}, {14, 14}, {15, 14},
    {15, 15}, {16, 15}, {16, 16}
  };
  
  int totalFrames = sizeof(animationPath) / sizeof(animationPath[0]);
  
  // Execute each frame of the animation
  for (int i = 0; i < totalFrames; i++) {
    displayAnimationFrame(animationPath[i].col, animationPath[i].row);
  }
}

/**
 * Displays a single frame of the animation
 * @param col - Column position (0-15)
 * @param row - Row position (0-1)
 */
void displayAnimationFrame(int col, int row) {
  lcd.clear();                    // Clear the display
  lcd.setCursor(col, row);        // Set cursor to specified position
  lcd.print(ANIMATION_CHAR);      // Display animation character
  delay(ANIMATION_DELAY);         // Maintain frame timing
  lcd.clear();                    // Clear for next frame
}