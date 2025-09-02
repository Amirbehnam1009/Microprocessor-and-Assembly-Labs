/*
 * Servo Angle Controller with Keypad Input
 * Allows setting servo angles via keypad input with automatic angle normalization
 * Supports angles from 0 to 360 degrees with proper normalization to servo range (0-180)
 */

#include <Keypad.h>
#include <Servo.h>

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

// Servo configuration
const int SERVO_PIN = 2;          // Digital pin connected to servo
const int MAX_SERVO_ANGLE = 180;  // Maximum valid servo angle

// Keypad command constants
const char SUBMIT_KEY = '=';      // Key to submit angle input
const char NEGATIVE_KEY = '-';    // Key for negative input

// Initialize keypad and servo objects
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Servo myServo;                    // Servo object to control the motor

// Input handling variables
String inputBuffer = "";          // Stores user input digits
bool isNegative = false;          // Flag for negative angle input

/**
 * Setup function - runs once at startup
 * Initializes serial communication and attaches servo to pin
 */
void setup() {
  Serial.begin(9600);             // Initialize serial for debugging
  myServo.attach(SERVO_PIN);      // Attach servo to specified pin
  Serial.println("Servo Controller Ready!");
  Serial.println("Enter angle (0-360) and press * to submit");
  Serial.println("Use - for negative angles");
  Serial.println("----------------------------");
}

/**
 * Main loop function - runs continuously
 * Handles keypad input and processes servo angle commands
 */
void loop() {
  char key = keypad.getKey();     // Check for keypress
  
  if (key) {
    handleKeyInput(key);          // Process the pressed key
  }
}

/**
 * Processes keypad input for servo angle control
 * @param key - The character of the pressed key
 */
void handleKeyInput(char key) {
  // Handle angle submission
  if (key == SUBMIT_KEY) {
    processAngleInput();
  } 
  // Handle negative sign
  else if (key == NEGATIVE_KEY) {
    isNegative = true;
    Serial.println("Negative angle selected");
  }
  // Handle numeric input
  else if (key >= '0' && key <= '9') {
    inputBuffer += key;           // Add digit to input buffer
    Serial.print(key);            // Echo the input
  }
}

/**
 * Processes and validates angle input, then moves servo
 * Handles angle normalization for values beyond servo range
 */
void processAngleInput() {
  if (inputBuffer.length() == 0) {
    Serial.println("No input provided!");
    inputBuffer = "";
    isNegative = false;
    return;
  }
  
  long inputAngle = inputBuffer.toInt();  // Convert input to integer
  
  // Apply negative sign if needed
  if (isNegative) {
    inputAngle = -inputAngle;
  }
  
  // Normalize angle to 0-360 range
  inputAngle = inputAngle % 360;
  if (inputAngle < 0) {
    inputAngle += 360;
  }
  
  // Convert to servo range (0-180)
  // Angles 0-180 remain the same, angles 181-359 become 0-179
  int servoAngle = inputAngle;
  if (inputAngle > 180) {
    servoAngle = inputAngle - 180;
  }
  
  moveServoAndReport(servoAngle, inputAngle);
  
  // Reset input variables
  inputBuffer = "";
  isNegative = false;
}

/**
 * Moves servo to specified angle and reports action to serial
 * @param targetAngle - The actual angle to set the servo to (0-180)
 * @param enteredAngle - The original angle entered by user (0-359)
 */
void moveServoAndReport(int targetAngle, long enteredAngle) {
  myServo.write(targetAngle);     // Move servo to target angle
  
  // Read current servo position for verification
  int currentPosition = myServo.read();
  
  // Report action to serial monitor
  Serial.println("\n----------------------------");
  Serial.println("Entered angle: " + String(enteredAngle) + "°");
  Serial.println("Servo set to: " + String(targetAngle) + "°");
  Serial.println("Current servo position: " + String(currentPosition) + "°");
  Serial.println("----------------------------\n");
}