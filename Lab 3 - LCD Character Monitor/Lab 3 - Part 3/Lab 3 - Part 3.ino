/*
 * Calculator Application
 * Implements a basic calculator with keypad input and LCD display
 * Supports addition, subtraction, multiplication, and division
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

// Special key definitions
const char CLEAR_KEY = 'o';     // Key to clear calculator
const char EQUALS_KEY = '=';    // Key to perform calculation

// Initialize keypad and LCD objects
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

// Calculator state variables
String operand1 = "";           // First operand
String operand2 = "";           // Second operand
char mathOperator;              // Current operator (+, -, *, /)
boolean operatorEntered = false; // Flag indicating operator has been entered

/**
 * Setup function - runs once at startup
 * Initializes serial communication and LCD display
 */
void setup() {
  Serial.begin(9600);           // Initialize serial for debugging
  lcd.begin(16, 2);             // Initialize 16x2 LCD display
  lcd.clear();                  // Clear display
  lcd.setCursor(0, 0);          // Set initial cursor position
}

/**
 * Main loop function - runs continuously
 * Handles keypad input and calculator operations
 */
void loop() {
  char key = keypad.getKey();   // Check for keypress
  
  if (key) {
    processKeyInput(key);       // Process the pressed key
    debugCalculatorState();     // Output current state to serial
  }
}

/**
 * Processes calculator key input
 * @param key - The character of the pressed key
 */
void processKeyInput(char key) {
  lcd.print(key);               // Display key on LCD
  
  // Handle clear key
  if (key == CLEAR_KEY) {
    clearCalculator();
  }
  // Handle equals key (perform calculation)
  else if (key == EQUALS_KEY) {
    performCalculation();
  }
  // Handle operator keys (+, -, *, /)
  else if (!operatorEntered && isMathOperator(key)) {
    setOperator(key);
  }
  // Handle numeric input (before operator)
  else if (!operatorEntered) {
    operand1 += key;
  }
  // Handle numeric input (after operator)
  else {
    operand2 += key;
  }
}

/**
 * Clears calculator state and resets display
 */
void clearCalculator() {
  operand1 = "";
  operand2 = "";
  operatorEntered = false;
  lcd.clear();
  lcd.setCursor(0, 0);
}

/**
 * Performs calculation based on current operands and operator
 * Displays result on second line of LCD
 */
void performCalculation() {
  int num1 = operand1.toInt();
  int num2 = operand2.toInt();
  
  lcd.setCursor(0, 1);          // Move to second line for result
  
  switch (mathOperator) {
    case '+':
      lcd.print(num1 + num2);
      break;
    case '-':
      lcd.print(num1 - num2);
      break;
    case '*':
      lcd.print(num1 * num2);
      break;
    case '/':
      // Use float division for more precise results
      lcd.print((float)num1 / num2);
      break;
  }
}

/**
 * Sets the current mathematical operator
 * @param op - The operator character (+, -, *, /)
 */
void setOperator(char op) {
  mathOperator = op;
  operatorEntered = true;
}

/**
 * Checks if a key is a valid mathematical operator
 * @param key - The key to check
 * @return true if key is +, -, *, or /
 */
boolean isMathOperator(char key) {
  return key == '+' || key == '-' || key == '*' || key == '/';
}

/**
 * Outputs current calculator state to serial monitor for debugging
 */
void debugCalculatorState() {
  Serial.println("Op1: " + operand1);
  Serial.print("Op:  ");
  Serial.println(mathOperator);
  Serial.println("Op2: " + operand2);
  Serial.println("----------------");
}