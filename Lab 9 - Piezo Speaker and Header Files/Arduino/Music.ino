// Include necessary libraries and header files
#include "pitches.h"          // Contains note frequency definitions
#include <Keypad.h>           // Keypad library for matrix keypad input
#include "jingle_bells.h"     // Jingle Bells melody data
#include "Ode_to_Joy.h"       // Ode to Joy melody data
#include "the_imperial_march.h" // Imperial March melody data
#include "frere_jacques.h"    // Frere Jacques melody data

// Define tempo for music playback (beats per minute)
#define TEMPO 210
// Alternative tempo options (commented out)
//#define TEMPO 150
//#define TEMPO 150
//#define TEMPO 135
//#define TEMPO 210

// Keypad configuration
const byte ROWS = 4;          // Number of rows in keypad
const byte COLS = 4;          // Number of columns in keypad

// Define keypad layout (4x4 matrix)
char keys[ROWS][COLS] = {
  {'7','8','9', '/'},        // Row 1 keys
  {'4','5','6', '*'},        // Row 2 keys
  {'1','2','3', '-'},        // Row 3 keys
  {'o','0','=', '+'}         // Row 4 keys
};

// Pin connections for keypad rows
byte rowPins[ROWS] = {22, 23, 24, 25};

// Pin connections for keypad columns
byte colPins[COLS] = {26, 27, 28, 29};

// LED pin array (9 pins from A0 to A8)
const byte ledPins[9] = {A0, A1, A2, A3, A4, A5, A6, A7, A8};

// Initialize keypad object with configuration
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Pin definitions
const int potPin = A0,        // Potentiometer pin for pitch control
          buttonPin = 0,      // Button pin (with internal pull-up)
          soundPin = 8;       // Speaker/buzzer output pin

// Global variables
float number;                 // Stores pitch scaling factor from potentiometer
bool now = false;             // Flag to indicate button press state

void setup() {
  Serial.begin(9600);         // Initialize serial communication for debugging
  
  // Configure button pin with internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // Check for keypad input
  char key = keypad.getKey();
  
  // Read button state (inverted due to pull-up: LOW = pressed)
  now = 1 - digitalRead(buttonPin);
  
  // Only process if button is pressed
  if (now) {
    // Check if a key was pressed
    if (key) {
      switch(key) {
        case '*':  // Jingle Bells
          // Loop through all notes in the melody
          for (int note = 0; note < sizeof(melody) / sizeof(int); note++) {
            // Read potentiometer to get pitch scaling factor (0.0 to 2.0)
            number = analogRead(potPin) / 512.0;
            
            // Calculate note duration based on tempo and note type
            int duration = 2 * 60 * 1000.0 / (TEMPO * noteDurations[note]);
            
            // Play the note with adjusted pitch and duration
            tone(soundPin, (int)(melody[note] * number), duration);
            
            // Wait for the note to finish playing
            delay((int)duration);
          }
          
        case '+':  // Ode to Joy
          for (int note = 0; note < sizeof(melody) / sizeof(int); note++) {
            number = analogRead(potPin) / 512.0;
            int duration = 2 * 60 * 1000.0 / (TEMPO * noteDurations1[note]);
            tone(soundPin, (int)(melody1[note] * number), duration);
            delay((int)duration);
          }
          
        case '-':  // Imperial March
          for (int note = 0; note < sizeof(melody) / sizeof(int); note++) {
            number = analogRead(potPin) / 512.0;
            int duration = 2 * 60 * 1000.0 / (TEMPO * noteDurations2[note]);
            tone(soundPin, (int)(melody2[note] * number), duration);
            delay((int)duration);
          }
          
        case '=':  // Frere Jacques
          for (int note = 0; note < sizeof(melody) / sizeof(int); note++) {
            number = analogRead(potPin) / 512.0;
            int duration = 2 * 60 * 1000.0 / (TEMPO * noteDurations3[note]);
            tone(soundPin, (int)(melody3[note] * number), duration);
            delay((int)duration);
          }
          
        default:  // Any other key
          Serial.println(key);  // Print key to serial monitor for debugging
      }
    }
  }
}