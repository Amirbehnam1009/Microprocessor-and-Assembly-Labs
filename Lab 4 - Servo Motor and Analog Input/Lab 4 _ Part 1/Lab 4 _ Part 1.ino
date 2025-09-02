/*
 * Servo Motor Sweep Controller
 * Demonstrates basic servo motor movement through a 90-degree range
 */

#include <Servo.h>

// Servo configuration
const int SERVO_PIN = 2;          // Digital pin connected to servo
const int SWEEP_DELAY = 15;       // Delay between position changes (ms)
const int MIN_ANGLE = 0;          // Minimum servo position (degrees)
const int MAX_ANGLE = 90;         // Maximum servo position (degrees)

// Servo object initialization
Servo myServo;                    // Create servo object to control servo motor

// Current servo position variable
int currentPosition = 0;          // Stores the current servo angle

/**
 * Setup function - runs once at startup
 * Initializes the servo motor and attaches it to the specified pin
 */
void setup() {
  myServo.attach(SERVO_PIN);      // Attach servo to digital pin
}

/**
 * Main loop function - runs continuously
 * Sweeps servo back and forth between minimum and maximum angles
 */
void loop() {
  sweepServoForward();            // Sweep from 0 to 90 degrees
  sweepServoBackward();           // Sweep from 90 back to 0 degrees
}

/**
 * Sweeps servo from minimum to maximum angle
 * Moves in 1-degree increments with specified delay
 */
void sweepServoForward() {
  for (currentPosition = MIN_ANGLE; currentPosition <= MAX_ANGLE; currentPosition += 1) {
    myServo.write(currentPosition);  // Set servo position
    delay(SWEEP_DELAY);              // Control sweep speed
  }
}

/**
 * Sweeps servo from maximum back to minimum angle
 * Moves in 1-degree decrements with specified delay
 */
void sweepServoBackward() {
  for (currentPosition = MAX_ANGLE; currentPosition >= MIN_ANGLE; currentPosition -= 1) {
    myServo.write(currentPosition);  // Set servo position
    delay(SWEEP_DELAY);              // Control sweep speed
  }
}