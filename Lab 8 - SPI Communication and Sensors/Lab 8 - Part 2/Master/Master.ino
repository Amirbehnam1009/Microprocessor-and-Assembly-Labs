/*
 * Multi-Slave SPI Master Communication Module
 * Sends different messages to multiple SPI slave devices with proper protocol handling
 */

#include <SPI.h>

// Message definitions
const char MESSAGE_SLAVE_0[] = "Amir!\r";    // Message for first slave device
const char MESSAGE_SLAVE_1[] = "Hello Amir!\r"; // Message for second slave device

// Hardware configuration
const int SLAVE_SELECT_0_PIN = 44;  // Chip Select pin for first slave device
const int SLAVE_SELECT_1_PIN = 45;  // Chip Select pin for second slave device

// Timing configuration
const int INTER_CHAR_DELAY = 5;     // Delay between character transmissions (ms)
const int INTER_MESSAGE_DELAY = 1000; // Delay between messages to different slaves (ms)

/**
 * Setup function - runs once at startup
 * Initializes serial communication and SPI interface
 */
void setup() {
  Serial.begin(9600);                          // Initialize serial for debugging
  Serial.println("Starting Multi-Slave SPI Master"); // Startup message
  
  initializeSpi();                             // Set up SPI communication
}

/**
 * Initializes SPI interface and configures all slave select pins
 */
void initializeSpi() {
  // Configure slave select pins as outputs and set inactive (HIGH)
  pinMode(SLAVE_SELECT_0_PIN, OUTPUT);
  pinMode(SLAVE_SELECT_1_PIN, OUTPUT);
  
  digitalWrite(SLAVE_SELECT_0_PIN, HIGH);  // Deactivate slave 0
  digitalWrite(SLAVE_SELECT_1_PIN, HIGH);  // Deactivate slave 1
  
  // Initialize SPI communication
  SPI.begin();
}

/**
 * Main loop function - runs continuously
 * Alternates between sending messages to two different slave devices
 */
void loop() {
  // Send message to first slave device
  transmitToSlave(SLAVE_SELECT_0_PIN, MESSAGE_SLAVE_0);
  delay(INTER_MESSAGE_DELAY);
  
  // Send message to second slave device
  transmitToSlave(SLAVE_SELECT_1_PIN, MESSAGE_SLAVE_1);
  delay(INTER_MESSAGE_DELAY);
}

/**
 * Transmits a message to a specific slave device
 * @param slaveSelectPin - The chip select pin for the target slave
 * @param message - The message string to transmit
 */
void transmitToSlave(int slaveSelectPin, const char *message) {
  activateSlave(slaveSelectPin);    // Select the target slave
  sendMessage(message);             // Transmit the message
  deactivateSlave(slaveSelectPin);  // Deselect the slave
}

/**
 * Activates a slave device by asserting its chip select pin
 * @param slaveSelectPin - The chip select pin to assert
 */
void activateSlave(int slaveSelectPin) {
  digitalWrite(slaveSelectPin, LOW);  // Assert chip select (active low)
}

/**
 * Deactivates a slave device by deasserting its chip select pin
 * @param slaveSelectPin - The chip select pin to deassert
 */
void deactivateSlave(int slaveSelectPin) {
  digitalWrite(slaveSelectPin, HIGH);  // Deassert chip select
}

/**
 * Transmits a message string via SPI with character timing
 * @param message - The null-terminated message string to send
 */
void sendMessage(const char *message) {
  // Iterate through each character in the message until null terminator
  for (const char *characterPointer = message; *characterPointer != '\0'; characterPointer++) {
    char currentChar = *characterPointer;
    
    // Transmit character via SPI
    SPI.transfer(currentChar);
    
    // Echo character to serial for debugging
    Serial.print(currentChar);
    
    // Brief delay between characters for timing
    delay(INTER_CHAR_DELAY);
  }
  
  Serial.println();  // New line after complete message
}