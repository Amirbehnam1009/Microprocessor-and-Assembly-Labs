/*
 * Servo Angle Controller with Serial Input
 * Allows setting servo angles via serial input with angle transformation
 */

#include <Keypad.h>
#include <Servo.h>

// Servo configuration
const int SERVO_PIN = 2;          // Digital pin connected to servo
const int MAX_SERVO_ANGLE = 180;  // Maximum standard servo angle
const int FULL_CIRCLE = 360;      // Degrees in full circle for transformation

// Serial command constants
const char EXECUTE_COMMAND = '\n'; // Use the "Enter" key as the trigger

// Servo object initialization
Servo myServo;                    // Servo object to control the motor

// Input processing variables
int accumulatedValue = 0;         // Accumulates numeric input digits

/**
 * Setup function - runs once at startup
 * Initializes serial communication and attaches servo to pin
 */
void setup() {
  Serial.begin(9600);             // Initialize serial communication
  myServo.attach(SERVO_PIN);      // Attach servo to specified pin
}

/**
 * Main loop function - runs continuously
 * Handles serial input and processes servo angle commands
 */
void loop() {
  processSerialInput();           // Check and process any serial input
}

/**
 * Processes incoming serial data for servo control
 * Handles digit accumulation and command execution
 */
void processSerialInput() {
  if (Serial.available() > 0) {
    char inputChar = Serial.read();  // Read incoming character
    
    // Process numeric digits (0-9)
    if (isDigit(inputChar)) {
      handleDigitInput(inputChar);
    } 
    // Process execute command (ASCII '1' which is 49)
    else if (inputChar == EXECUTE_COMMAND) {
      executeServoMovement();
    }
  }
}

/**
 * Handles numeric digit input and accumulates the value
 * @param digitChar - The digit character to process
 */
void handleDigitInput(char digitChar) {
  int digitValue = digitChar - '0';  // Convert char to integer
  accumulatedValue = (accumulatedValue * 10) + digitValue;  // Build multi-digit number
  Serial.print(digitChar);           // Echo digit back to serial
}

/**
 * Executes servo movement based on accumulated input value
 * Applies transformation rules and moves servo accordingly
 */
void executeServoMovement() {
  Serial.println();  // New line for command output
  
  // Handle values exceeding maximum servo angle
  if (accumulatedValue > MAX_SERVO_ANGLE) {
    int transformedAngle = FULL_CIRCLE - accumulatedValue;
    moveServoWithReport(transformedAngle, accumulatedValue, true);
  } 
  // Handle values within normal servo range
  else {
    int transformedAngle = MAX_SERVO_ANGLE - accumulatedValue;
    moveServoWithReport(transformedAngle, accumulatedValue, false);
  }
  
  resetInput();  // Prepare for next input sequence
}

/**
 * Moves servo to target angle and reports action to serial monitor
 * @param targetAngle - The actual angle to set the servo to
 * @param inputAngle - The original angle entered by user
 * @param isFullCircleTransform - Flag indicating transformation type
 */
void moveServoWithReport(int targetAngle, int inputAngle, bool isFullCircleTransform) {
  myServo.write(targetAngle);  // Move servo to calculated angle
  
  // Read current position for verification
  int currentPosition = myServo.read();
  
  // Report results to serial monitor
  Serial.println("Current servo position: " + String(currentPosition));
  
  if (isFullCircleTransform) {
    Serial.println("Input angle: " + String(inputAngle));
    Serial.println("Transformed angle (360 - input): " + String(targetAngle));
  } else {
    Serial.println("Input angle: " + String(inputAngle));
    Serial.println("Transformed angle (180 - input): " + String(targetAngle));
  }
  
  Serial.println("----------------------------");
}

/**
 * Resets input accumulator for next command sequence
 */
void resetInput() {
  accumulatedValue = 0;  // Reset accumulated value
}