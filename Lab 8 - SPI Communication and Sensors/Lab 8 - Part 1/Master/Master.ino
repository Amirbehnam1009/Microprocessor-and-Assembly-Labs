/*
 * SPI Master Communication Module
 * Sends a constant message to SPI slave device with proper protocol handling
 */

#include <SPI.h>

// Hardware configuration
const int SLAVE_SELECT_PIN = 45;  // Chip Select pin for SPI slave device

// Message configuration
const char SPI_MESSAGE[] = "Amir,9831133!\r";  // Message to transmit
const int MESSAGE_DELAY = 100;                 // Delay between transmissions (ms)
const int INTER_CHAR_DELAY = 5;                // Delay between characters (ms)
const int CS_ASSERT_DELAY = 10;                // Chip Select assert delay (ms)

/**
 * Setup function - runs once at startup
 * Initializes serial communication and SPI interface
 */
void setup() {
  Serial.begin(9600);                          // Initialize serial for debugging
  Serial.println("Starting SPI Master");       // Startup message
  
  initializeSpi();                             // Set up SPI communication
}

/**
 * Initializes SPI interface and configures slave select pin
 */
void initializeSpi() {
  // Configure slave select pin as output and set inactive (HIGH)
  pinMode(SLAVE_SELECT_PIN, OUTPUT);
  digitalWrite(SLAVE_SELECT_PIN, HIGH);
  
  // Initialize SPI communication
  SPI.begin();
}

/**
 * Main loop function - runs continuously
 * Transmits message to SPI slave device at regular intervals
 */
void loop() {
  transmitSpiMessage();      // Send message to slave device
  delay(MESSAGE_DELAY);      // Wait before next transmission
}

/**
 * Transmits the predefined message to the SPI slave device
 * Handles proper chip select protocol and character timing
 */
void transmitSpiMessage() {
  // Activate slave device by pulling CS low
  activateSlave();
  
  // Transmit each character of the message
  sendMessageCharacters();
  
  // Deactivate slave device by pulling CS high
  deactivateSlave();
  
  // Print newline for serial output formatting
  Serial.println();
}

/**
 * Activates the slave device by asserting chip select
 */
void activateSlave() {
  digitalWrite(SLAVE_SELECT_PIN, LOW);  // Assert chip select (active low)
  delay(CS_ASSERT_DELAY);               // Wait for slave to recognize selection
}

/**
 * Deactivates the slave device by deasserting chip select
 */
void deactivateSlave() {
  digitalWrite(SLAVE_SELECT_PIN, HIGH);  // Deassert chip select
}

/**
 * Transmits each character of the message with proper timing
 */
void sendMessageCharacters() {
  // Iterate through each character in the message
  for (const char *characterPointer = SPI_MESSAGE; *characterPointer != '\0'; characterPointer++) {
    char currentChar = *characterPointer;
    
    // Transmit character via SPI
    SPI.transfer(currentChar);
    
    // Echo character to serial for debugging
    Serial.print(currentChar);
    
    // Brief delay between characters
    delay(INTER_CHAR_DELAY);
  }
}