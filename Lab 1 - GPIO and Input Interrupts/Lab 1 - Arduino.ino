// Pin Definitions
const int BUTTON_PIN_0 = 10;
const int BUTTON_PIN_1 = 11;
const int BUTTON_PIN_2 = 12;
const int LED_PIN_0 = 0;
const int LED_PIN_1 = 1;
const int LED_PIN_2 = 2;
const int LED_PIN_3 = 3;
const int LED_PIN_4 = 4;
const int LED_PIN_5 = 5;
const int LED_PIN_6 = 6;

// Button state variables
int buttonState0 = 0;
int buttonState1 = 0;
int buttonState2 = 0;

// Number of blink cycles based on string length
int blinkCycles = strlen("amir");  // "amir" = 4 characters

// Setup function - runs once at startup
void setup() {
  // Configure button pins as inputs
  pinMode(BUTTON_PIN_0, INPUT);
  pinMode(BUTTON_PIN_1, INPUT);
  pinMode(BUTTON_PIN_2, INPUT);
  
  // Configure LED pins as outputs
  pinMode(LED_PIN_0, OUTPUT);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  pinMode(LED_PIN_4, OUTPUT);
  pinMode(LED_PIN_5, OUTPUT);
  pinMode(LED_PIN_6, OUTPUT);
}

// Main loop function - runs continuously
void loop() {
  // Read current state of all buttons
  buttonState0 = digitalRead(BUTTON_PIN_0);
  buttonState1 = digitalRead(BUTTON_PIN_1);
  buttonState2 = digitalRead(BUTTON_PIN_2);

  // Handle button 0 press: Sequential LED activation
  if (buttonState0 == HIGH) {
    sequentialLedActivation();
  } 
  // Handle button 1 press: Blink all LEDs multiple times
  else if (buttonState1 == HIGH) {
    blinkAllLeds(blinkCycles);
  } 
  // Handle button 2 press: Turn off all LEDs
  else if (buttonState2 == HIGH) {
    turnOffAllLeds();
  }
}

// Sequentially activates each LED with 1-second delays
void sequentialLedActivation() {
  digitalWrite(LED_PIN_0, HIGH);
  delay(500);
  digitalWrite(LED_PIN_1, HIGH);
  delay(500);
  digitalWrite(LED_PIN_2, HIGH);
  delay(500);
  digitalWrite(LED_PIN_3, HIGH);
  delay(500);
  digitalWrite(LED_PIN_4, HIGH);
  delay(500);
  digitalWrite(LED_PIN_5, HIGH);
  delay(500);
  digitalWrite(LED_PIN_6, HIGH);
  delay(500);
}

// Blinks all LEDs for specified number of cycles
void blinkAllLeds(int cycles) {
  while (cycles >= 0) {
    // Turn all LEDs on
    setAllLeds(HIGH);
    delay(350);
    
    // Turn all LEDs off
    setAllLeds(LOW);
    delay(350);
    
    cycles--;
  }
  
  // Leave all LEDs on after final cycle
  setAllLeds(HIGH);
}

// Turns all LEDs off
void turnOffAllLeds() {
  setAllLeds(LOW);
}

// Helper function to set all LEDs to specified state
void setAllLeds(int state) {
  digitalWrite(LED_PIN_0, state);
  digitalWrite(LED_PIN_1, state);
  digitalWrite(LED_PIN_2, state);
  digitalWrite(LED_PIN_3, state);
  digitalWrite(LED_PIN_4, state);
  digitalWrite(LED_PIN_5, state);
  digitalWrite(LED_PIN_6, state);
}