/*
 * Multi-Stage Timer Controller with LCD Display
 * Controls 4-stage process with EEPROM storage for mode persistence
 */

#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Wire.h>

// I2C EEPROM configuration
#define EEPROM_ADDRESS 0b1010000  // I2C address for external memory
#define EEPROM_MARKER 0xAA        // Data validation marker

// LED pin assignments
const int LED_PIN_1 = 29;
const int LED_PIN_2 = 30;
const int LED_PIN_3 = 31;
const int LED_PIN_4 = 32;

// LCD pin configuration
const int RS_PIN = 22, EN_PIN = 23, D4_PIN = 24, D5_PIN = 25, D6_PIN = 26, D7_PIN = 27;
LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

// Keypad configuration
const byte ROWS = 4, COLS = 4;
char keys[ROWS][COLS] = {
  {'7', '8', '9', '/'},
  {'4', '5', '6', 'x'},
  {'1', '2', '3', '-'},
  {'*', '0', '=', '+'}
};
const byte ROW_PINS[ROWS] = {46, 47, 48, 49};
const byte COL_PINS[COLS] = {50, 51, 52, 53};
Keypad keypad = Keypad(makeKeymap(keys), ROW_PINS, COL_PINS, ROWS, COLS);

// Special key definitions
const char PAUSE_KEY = '*';
const char CHANGE_TIME_KEY = '-';
const char CHANGE_MODE_KEY = '+';

// Timer stage configuration
enum TimerStage {
  STAGE_PISH = 1,     // Pre-wash stage
  STAGE_SHOOYANDE,    // Wash stage
  STAGE_AB,           // Rinse stage
  STAGE_KHOSK         // Dry stage
};

// Default timer durations (seconds)
int stageDurations[] = {0, 50, 60, 70, 80}; // Index 0 unused, 1-4 for stages

// System state variables
int currentStage = STAGE_PISH;
int defaultStage = STAGE_PISH;
char pressedKey;

/**
 * Setup function - runs once at startup
 * Initializes all hardware components and loads saved stage
 */
void setup() {
  Serial.begin(9600);
  
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.clear();
  
  // Initialize I2C
  Wire.begin();
  
  // Initialize LED pins
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  pinMode(LED_PIN_4, OUTPUT);
  
  // Load saved stage from EEPROM
  currentStage = loadSavedStage();
  if (currentStage < STAGE_PISH || currentStage > STAGE_KHOSK) {
    currentStage = STAGE_PISH;
  }
  defaultStage = currentStage;
}

/**
 * Main loop function - runs continuously
 * Controls the multi-stage timer process with user interaction
 */
void loop() {
  int timerCount = 0;
  int stageDuration = getStageDuration(currentStage);
  int ledPin = getStageLedPin(currentStage);
  
  // Activate current stage LED
  digitalWrite(ledPin, HIGH);
  
  // Execute stage timer
  while (timerCount < stageDuration) {
    processUserInput(timerCount, stageDuration);
    delay(10);
    timerCount++;
  }
  
  // Deactivate current stage LED
  digitalWrite(ledPin, LOW);
  
  // Move to next stage or finish
  advanceToNextStage();
}

/**
 * Returns the duration for the specified stage
 * @param stage - The stage number (1-4)
 * @return Duration in seconds
 */
int getStageDuration(int stage) {
  return stageDurations[stage];
}

/**
 * Returns the LED pin for the specified stage
 * @param stage - The stage number (1-4)
 * @return LED pin number
 */
int getStageLedPin(int stage) {
  switch (stage) {
    case STAGE_PISH:    return LED_PIN_1;
    case STAGE_SHOOYANDE: return LED_PIN_2;
    case STAGE_AB:      return LED_PIN_3;
    case STAGE_KHOSK:   return LED_PIN_4;
    default:           return LED_PIN_1;
  }
}

/**
 * Processes user input during timer operation
 * @param currentTime - Current elapsed time
 * @param totalTime - Total stage duration
 */
void processUserInput(int currentTime, int totalTime) {
  updateDisplay(currentTime, totalTime);
  
  pressedKey = keypad.getKey();
  
  if (pressedKey == PAUSE_KEY) {
    handlePause();
  } else if (pressedKey == CHANGE_TIME_KEY) {
    handleTimeChange();
  } else if (pressedKey == CHANGE_MODE_KEY) {
    handleModeChange();
  }
}

/**
 * Updates LCD display with current status
 * @param currentTime - Current elapsed time
 * @param totalTime - Total stage duration
 */
void updateDisplay(int currentTime, int totalTime) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Default Mode: ");
  lcd.print(defaultStage);
  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(totalTime - currentTime);
}

/**
 * Handles pause functionality
 */
void handlePause() {
  pressedKey = keypad.waitForKey();
  while (pressedKey != PAUSE_KEY) {
    pressedKey = keypad.waitForKey();
  }
}

/**
 * Handles time change for stages
 */
void handleTimeChange() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Change Time");
  
  pressedKey = keypad.waitForKey();
  lcd.setCursor(0, 1);
  lcd.print(pressedKey);
  delay(100);
  
  if (pressedKey >= '1' && pressedKey <= '4') {
    int stage = pressedKey - '0';
    changeStageTime(stage);
  }
}

/**
 * Changes the duration for a specific stage
 * @param stage - Stage number to modify (1-4)
 */
void changeStageTime(int stage) {
  // Read two-digit time value
  char digit1 = keypad.waitForKey();
  char digit2 = keypad.waitForKey();
  
  int newTime = (digit1 - '0') * 10 + (digit2 - '0');
  stageDurations[stage] = newTime;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(newTime);
  delay(250);
}

/**
 * Handles mode/stage change
 */
void handleModeChange() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Change Mode");
  
  pressedKey = keypad.waitForKey();
  lcd.setCursor(0, 1);
  lcd.print(pressedKey);
  delay(100);
  
  if (pressedKey >= '1' && pressedKey <= '4') {
    int newStage = pressedKey - '0';
    saveStage(newStage);
    defaultStage = newStage;
  }
}

/**
 * Advances to the next stage or completes the cycle
 */
void advanceToNextStage() {
  currentStage++;
  
  // Check if all stages are complete
  if (currentStage > STAGE_KHOSK) {
    indicateCompletion();
    resetSystem();
  }
}

/**
 * Indicates process completion with all LEDs and message
 */
void indicateCompletion() {
  // Turn on all LEDs
  digitalWrite(LED_PIN_1, HIGH);
  digitalWrite(LED_PIN_2, HIGH);
  digitalWrite(LED_PIN_3, HIGH);
  digitalWrite(LED_PIN_4, HIGH);
  
  // Display completion message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("FINISH");
  
  // Wait for user acknowledgment
  pressedKey = keypad.waitForKey();
  while (pressedKey != PAUSE_KEY) {
    pressedKey = keypad.waitForKey();
  }
  
  // Turn off all LEDs
  digitalWrite(LED_PIN_1, LOW);
  digitalWrite(LED_PIN_2, LOW);
  digitalWrite(LED_PIN_3, LOW);
  digitalWrite(LED_PIN_4, LOW);
}

/**
 * Resets the system to the saved default stage
 */
void resetSystem() {
  currentStage = loadSavedStage();
  if (currentStage < STAGE_PISH || currentStage > STAGE_KHOSK) {
    currentStage = STAGE_PISH;
  }
  defaultStage = currentStage;
}

/**
 * Saves stage to EEPROM
 * @param stage - Stage number to save (1-4)
 */
void saveStage(int stage) {
  Wire.beginTransmission(EEPROM_ADDRESS);
  Wire.write(0x00);           // Memory address
  Wire.write(EEPROM_MARKER);  // Validation marker
  Wire.write(stage);          // Stage data
  Wire.endTransmission();
}

/**
 * Loads saved stage from EEPROM
 * @return Saved stage number, or 1 if invalid
 */
int loadSavedStage() {
  Wire.beginTransmission(EEPROM_ADDRESS);
  Wire.write(0x00);           // Memory address
  Wire.write(EEPROM_MARKER);  // Validation marker
  Wire.endTransmission();
  
  Wire.requestFrom(EEPROM_ADDRESS, 1);
  if (Wire.available()) {
    return Wire.read();
  }
  return STAGE_PISH;  // Default if read fails
}