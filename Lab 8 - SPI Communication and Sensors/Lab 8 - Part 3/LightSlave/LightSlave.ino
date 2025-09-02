/*
 * SPI Light Sensor Slave Receiver
 * Receives light percentage values from SPI master with interrupt-driven protocol
 */

#include <SPI.h>

// Hardware pin configuration
const int MISO_PIN = 50;   // Master In Slave Out (data from slave to master)
const int MOSI_PIN = 51;   // Master Out Slave In (data from master to slave)
const int SCK_PIN = 52;    // Serial Clock (clock signal from master)
const int SS_PIN = 53;     // Slave Select (chip select from master, active low)

// Global state variables
volatile boolean dataReceived = false;  // Flag indicating new data received
volatile uint8_t lightPercentage = 0;   // Stores received light percentage value

/**
 * Setup function - runs once at startup
 * Initializes serial communication and configures SPI slave interface
 */
void setup() {
  Serial.begin(9600);                // Initialize serial for debugging
  Serial.println("Starting Light Sensor Slave"); // Startup message
  
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
 * Processes received light percentage data from SPI master
 */
void loop() {
  if (dataReceived) {
    processLightData();              // Handle the received data
    resetReceiverState();            // Prepare for next data reception
  }
}

/**
 * Processes received light percentage data
 * Outputs the value to serial monitor
 */
void processLightData() {
  Serial.print("Light: ");
  Serial.print(lightPercentage);
  Serial.println("%");
}

/**
 * Resets receiver state for next data reception
 */
void resetReceiverState() {
  dataReceived = false;              // Clear reception flag
}

/**
 * SPI Transfer Complete Interrupt Service Routine
 * Handles incoming data from SPI master
 * Called automatically when SPI data transfer is complete
 */
ISR(SPI_STC_vect) {
  byte receivedData = SPDR;          // Read received byte from SPI Data Register
  lightPercentage = (uint8_t)receivedData; // Store the light percentage value
  dataReceived = true;               // Set data received flag
}