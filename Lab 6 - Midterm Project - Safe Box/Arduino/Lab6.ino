/*
 * Secure Door Lock System
 * Combination keypad, LCD display, and servo-controlled door lock
 * Features password protection, timer, and configuration options
 */

#include <Servo.h>
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

// Hardware pin assignments
const byte DOOR_LED_PIN = 50;  // Door status LED pin
const byte BUZZER_PIN = 53;    // Buzzer pin
const byte SERVO_PIN = 7;      // Servo control pin

// System state constants
enum SystemState {
  STATE_PASSWORD_ENTRY,    // 0: Waiting for password input
  STATE_DOOR_OPEN,         // 1: Door is unlocked
  STATE_CHANGE_PASSWORD,   // 2: Changing password
  STATE_CHANGE_TIMER       // 3: Changing timer duration
};

// User interface prompts
const String PROMPTS[] = {
  "Password: ",      // STATE_PASSWORD_ENTRY
  "Door Unlocked",   // STATE_DOOR_OPEN
  "ChangePassword: ",    // STATE_CHANGE_PASSWORD
  "ChangeTime: "     // STATE_CHANGE_TIMER
};

// Special key definitions
const char ENTER_KEY = '=';     // Key to submit input
const char CHANGE_PASS_KEY = '*'; // Key to change password
const char CHANGE_TIME_KEY = '-'; // Key to change timer
const char LOCK_DOOR_KEY = 'o';  // Key to manually lock door

// Initialize hardware objects
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);
Servo doorServo;  // Servo object for door lock mechanism

// System state variables
SystemState currentState = STATE_PASSWORD_ENTRY;
String inputBuffer = "";        // Stores user input
String password = "1234";       // Default password
bool timerActive = false;       // Timer status flag
int timerDuration = 10;         // Default timer duration (seconds)
long timerStartTime = 0;        // Timer start timestamp

/**
 * Setup function - runs once at startup
 * Initializes all hardware components and system state
 */
void setup() {
  Serial.begin(9600);           // Initialize serial communication
  
  // Initialize servo
  doorServo.attach(SERVO_PIN);
  doorServo.write(0);           // Start with door locked
  
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(PROMPTS[currentState]);
  
  // Initialize GPIO pins
  pinMode(DOOR_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

/**
 * Updates and displays the current door timer
 * @return Remaining time in seconds, or 0 if timer expired
 */
float updateTimerDisplay() {
  if (!timerActive) {
    return 0;
  }
  
  long currentTime = millis();
  float remainingTime = (float)timerDuration - ((currentTime - timerStartTime) / 1000.0);
  
  // Check if timer has expired
  if (remainingTime <= 0) {
    lockDoor();
    remainingTime = 0;
  }
  
  // Display timer on second line of LCD
  lcd.setCursor(0, 1);
  lcd.print("Timer: " + String(remainingTime) + "  ");
  
  return remainingTime;
}

/**
 * Activates the buzzer briefly for user feedback
 */
void activateBuzzer() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
}

/**
 * Unlocks the door and starts the timer
 */
void unlockDoor() {
  timerStartTime = millis();
  timerActive = true;
  digitalWrite(DOOR_LED_PIN, HIGH);
  activateBuzzer();
  doorServo.write(180);  // Unlock position
  currentState = STATE_DOOR_OPEN;
}

/**
 * Locks the door and resets system state
 */
void lockDoor() {
  doorServo.write(0);    // Lock position
  digitalWrite(DOOR_LED_PIN, LOW);
  timerActive = false;
  currentState = STATE_PASSWORD_ENTRY;
  displayPrompt();
  activateBuzzer();
}

/**
 * Displays the current state prompt and clears input buffer
 */
void displayPrompt() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(PROMPTS[currentState]);
  inputBuffer = "";
}

/**
 * Validates entered password and unlocks door if correct
 */
void validatePassword() {
  lcd.setCursor(0, 0);
  
  if (inputBuffer == password) {
    lcd.print("Correct password");
    unlockDoor();
  } else {
    lcd.print("Wrong password");
    currentState = STATE_PASSWORD_ENTRY;
  }
  
  delay(1000);
  displayPrompt();
}

/**
 * Processes special function keys when door is unlocked
 * @param key - The pressed key character
 */
void processFunctionKey(char key) {
  if (key == CHANGE_PASS_KEY) {
    currentState = STATE_CHANGE_PASSWORD;
  } else if (key == CHANGE_TIME_KEY) {
    currentState = STATE_CHANGE_TIMER;
  } else if (key == LOCK_DOOR_KEY) {
    lockDoor();
    return;  // Skip displayPrompt() since lockDoor() calls it
  }
  
  displayPrompt();
}

/**
 * Processes the ENTER key based on current system state
 */
void processEnterKey() {
  switch (currentState) {
    case STATE_PASSWORD_ENTRY:
      validatePassword();
      break;
      
    case STATE_CHANGE_PASSWORD:
      password = inputBuffer;  // Update password
      currentState = STATE_DOOR_OPEN;
      break;
      
    case STATE_CHANGE_TIMER:
      timerDuration = inputBuffer.toInt();  // Update timer duration
      currentState = STATE_DOOR_OPEN;
      break;
      
    default:
      break;
  }
  
  displayPrompt();
}

/**
 * Main loop function - runs continuously
 * Handles keypad input, timer updates, and system state management
 */
void loop() {
  // Update and display timer
  updateTimerDisplay();
  
  // Check for keypad input
  char key = keypad.getKey();
  
  if (key) {
    // Display key press on LCD
    lcd.setCursor(PROMPTS[currentState].length() + inputBuffer.length(), 0);
    lcd.print(key);
    
    // Process key based on type
    if (key == ENTER_KEY) {
      processEnterKey();
    } else if (timerActive && (key == CHANGE_PASS_KEY || key == CHANGE_TIME_KEY || key == LOCK_DOOR_KEY)) {
      processFunctionKey(key);
    } else {
      inputBuffer += key;  // Add to input buffer
    }
  }
  
  delay(50);  // Main loop delay
}