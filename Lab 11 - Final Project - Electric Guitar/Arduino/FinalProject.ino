/*
 * Multi-Channel Musical Instrument with Serial Control
 * Allows playing multiple notes simultaneously with pitch control and visual feedback
 */

#include <Tone.h>
#include <Keypad.h>

// Frequency multiplier constant
const int FREQUENCY_MULTIPLIER = 8;

// Musical note matrix (3 channels, 6 notes each)
const int NOTE_MATRIX[3][6] = {
  {NOTE_G3, NOTE_A4, NOTE_GS3, NOTE_B4, NOTE_AS4, NOTE_G3},
  {NOTE_C4, NOTE_FS3, NOTE_F3, NOTE_E3, NOTE_DS3, NOTE_D3},
  {NOTE_C3, NOTE_B3, NOTE_AS3, NOTE_A3, NOTE_GS2, NOTE_G2}
};

// Analog input pins for pitch control
const int PITCH_PINS[3] = {A1, A2, A3};  // Pitch control for channels 0, 1, 2

// LED indicator pins for each channel
const int LED_PINS[3] = {3, 4, 5};

// Tone objects for each channel
Tone toneChannel0;
Tone toneChannel1;
Tone toneChannel2;

// Channel state variables
int currentNoteIndices[3] = {0};      // Current selected note index for each channel
bool channelActive[3] = {false};      // Activation state for each channel
float pitchFactors[3] = {1.0};        // Pitch adjustment factors for each channel

/**
 * Setup function - runs once at startup
 * Initializes serial communication, tone objects, and LED pins
 */
void setup() {
  Serial.begin(9600);                 // Initialize serial communication
  
  initializeToneObjects();            // Set up tone generators
  initializeLedPins();                // Configure LED indicators
}

/**
 * Initializes tone objects with their respective output pins
 */
void initializeToneObjects() {
  toneChannel0.begin(5);              // Channel 0 output on pin 5
  toneChannel1.begin(10);             // Channel 1 output on pin 10
  toneChannel2.begin(15);             // Channel 2 output on pin 15
}

/**
 * Configures LED pins as outputs and sets initial state
 */
void initializeLedPins() {
  for (int i = 0; i < 3; i++) {
    pinMode(LED_PINS[i], OUTPUT);     // Set LED pins as outputs
    digitalWrite(LED_PINS[i], LOW);   // Turn off all LEDs initially
  }
}

/**
 * Main loop function - runs continuously
 * Processes serial input and updates tone generation
 */
void loop() {
  processSerialInput();               // Check for and process serial commands
  updateToneGeneration();             // Update tone output based on current state
}

/**
 * Processes incoming serial data for channel control
 */
void processSerialInput() {
  if (Serial.available() > 0) {
    char inputChar = Serial.read();   // Read incoming character
    Serial.println(inputChar);        // Echo back for debugging
    
    handleSerialCommand(inputChar);   // Process the command
  }
}

/**
 * Handles serial commands for channel control and note selection
 * @param command - The character command received
 */
void handleSerialCommand(char command) {
  // Channel toggle commands
  switch (command) {
    case 'o': toggleChannel(0); break;  // Toggle channel 0
    case 'k': toggleChannel(1); break;  // Toggle channel 1
    case 'm': toggleChannel(2); break;  // Toggle channel 2
    
    // Channel 0 note selection
    case 'i': setChannelNote(0, 0); break;
    case 'u': setChannelNote(0, 1); break;
    case 'y': setChannelNote(0, 2); break;
    case 't': setChannelNote(0, 3); break;
    case 'r': setChannelNote(0, 4); break;
    case 'e': setChannelNote(0, 5); break;
    
    // Channel 1 note selection
    case 'j': setChannelNote(1, 0); break;
    case 'h': setChannelNote(1, 1); break;
    case 'g': setChannelNote(1, 2); break;
    case 'f': setChannelNote(1, 3); break;
    case 'd': setChannelNote(1, 4); break;
    case 's': setChannelNote(1, 5); break;
    
    // Channel 2 note selection
    case 'n': setChannelNote(2, 0); break;
    case 'b': setChannelNote(2, 1); break;
    case 'v': setChannelNote(2, 2); break;
    case 'c': setChannelNote(2, 3); break;
    case 'x': setChannelNote(2, 4); break;
    case 'z': setChannelNote(2, 5); break;
  }
}

/**
 * Toggles a channel's active state
 * @param channel - The channel index (0-2)
 */
void toggleChannel(int channel) {
  channelActive[channel] = !channelActive[channel];
  
  // Update LED indicator
  digitalWrite(LED_PINS[channel], channelActive[channel] ? HIGH : LOW);
  
  // Stop tone if channel is deactivated
  if (!channelActive[channel]) {
    stopChannelTone(channel);
  }
}

/**
 * Sets the current note for a channel
 * @param channel - The channel index (0-2)
 * @param noteIndex - The note index within the channel (0-5)
 */
void setChannelNote(int channel, int noteIndex) {
  currentNoteIndices[channel] = noteIndex;
}

/**
 * Stops tone generation for a specific channel
 * @param channel - The channel index (0-2)
 */
void stopChannelTone(int channel) {
  switch (channel) {
    case 0: toneChannel0.stop(); break;
    case 1: toneChannel1.stop(); break;
    case 2: toneChannel2.stop(); break;
  }
}

/**
 * Updates tone generation for all active channels
 */
void updateToneGeneration() {
  // Update pitch factors from analog inputs
  for (int channel = 0; channel < 3; channel++) {
    pitchFactors[channel] = analogRead(PITCH_PINS[channel]) / 512.0;
  }
  
  // Generate tones for active channels
  if (channelActive[0]) {
    playChannelTone(0, toneChannel0, currentNoteIndices[0], pitchFactors[0]);
  }
  if (channelActive[1]) {
    playChannelTone(1, toneChannel1, currentNoteIndices[1], pitchFactors[1]);
  }
  if (channelActive[2]) {
    playChannelTone(2, toneChannel2, currentNoteIndices[2], pitchFactors[2]);
  }
}

/**
 * Plays a tone for a specific channel with pitch adjustment
 * @param channel - The channel index (0-2)
 * @param toneObject - The Tone object for this channel
 * @param noteIndex - The note index to play
 * @param pitchFactor - The pitch adjustment factor
 */
void playChannelTone(int channel, Tone &toneObject, int noteIndex, float pitchFactor) {
  int baseFrequency = NOTE_MATRIX[channel][noteIndex];
  int adjustedFrequency = (int)(FREQUENCY_MULTIPLIER * pitchFactor * baseFrequency);
  toneObject.play(adjustedFrequency);
}