/*
 * SPI Slave Communication Module
 * Receives messages from SPI master with interrupt-driven protocol handling
 */

#include <SPI.h>

// Hardware pin configuration
const int MISO_PIN = 50;   // Master In Slave Out (data from slave to master)
const int MOSI_PIN = 51;   // Master Out Slave In (data from master to slave)
const int SCK_PIN = 52;    // Serial Clock (clock signal from master)
const int SS_PIN = 53;     // Slave Select (chip select from master, active low)

// Message buffer configuration
const int MESSAGE_BUFFER_SIZE = 20;  // Maximum message length including null terminator
const char MESSAGE_TERMINATOR = '\r'; // Character indicating end of message

// Global state variables
volatile int bufferIndex = 0;        // Current position in message buffer
volatile boolean messageComplete = false;  // Flag indicating complete message received
char messageBuffer[MESSAGE_BUFFER_SIZE];   // Buffer for storing received message

/**
 * Setup function - runs once at startup
 * Initializes serial communication and configures SPI slave interface
 */
void setup() {
  Serial.begin(9600);                // Initialize serial for debugging
  Serial.println("Starting SPI Slave"); // Startup message
  
  initializeSpiPins();               // Configure SPI hardware pins
  configureSpiSlave();               // Set up SPI slave mode and interrupts
}

/**
 * Configures SPI pin modes and directions
 */
void initializeSpiPins() {
  pinMode(MOSI_PIN, INPUT);          // MOSI is input (data from master)
  pinMode(MISO_PIN, OUTPUT);         // MISO is output (data to master)
  pinMode(SCK_PIN, INPUT);           // SCK is input (clock from master)
  pinMode(SS_PIN, INPUT_PULLUP);     // SS is input with pull-up (active low)
}

/**
 * Configures SPI peripheral in slave mode with interrupts
 */
void configureSpiSlave() {
  SPCR |= _BV(SPE);                  // Enable SPI peripheral
  SPI.attachInterrupt();             // Enable SPI interrupt
}

/**
 * Main loop function - runs continuously
 * Processes completed messages received via SPI interrupts
 */
void loop() {
  if (messageComplete) {
    processReceivedMessage();        // Handle the complete message
    resetReceiverState();            // Prepare for next message
  }
}

/**
 * Processes a complete received message
 * Currently prints to serial monitor
 */
void processReceivedMessage() {
  Serial.println(messageBuffer);     // Output received message to serial
}

/**
 * Resets receiver state for next message reception
 */
void resetReceiverState() {
  bufferIndex = 0;                   // Reset buffer position
  messageComplete = false;           // Clear completion flag
}

/**
 * SPI Transfer Complete Interrupt Service Routine
 * Handles incoming data from SPI master character by character
 * Called automatically when SPI data transfer is complete
 */
ISR(SPI_STC_vect) {
  byte receivedByte = SPDR;          // Read received byte from SPI Data Register
  
  // Check if buffer has space for another character
  if (bufferIndex < MESSAGE_BUFFER_SIZE - 1) {
    // Store received character in buffer
    messageBuffer[bufferIndex] = receivedByte;
    bufferIndex++;
    
    // Check for message terminator character
    if (receivedByte == MESSAGE_TERMINATOR) {
      messageBuffer[bufferIndex] = '\0';  // Null-terminate the string
      messageComplete = true;             // Set completion flag
    }
  } else {
    // Buffer overflow protection
    handleBufferOverflow();
  }
}

/**
 * Handles buffer overflow situation
 * Terminates current message and marks it as complete
 */
void handleBufferOverflow() {
  messageBuffer[MESSAGE_BUFFER_SIZE - 1] = '\0';  // Ensure null termination
  messageComplete = true;                         // Force message completion
  // Note: Buffer index is not reset here to prevent infinite overflow loops
}