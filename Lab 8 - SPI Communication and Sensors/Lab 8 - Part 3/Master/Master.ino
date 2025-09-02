/*
 * Multi-Sensor SPI Master Controller
 * Reads temperature and light sensors and transmits data to SPI slaves
 */

#include <SPI.h>

// Hardware pin configuration
const int SLAVE_SELECT_LIGHT_PIN = 44;  // Chip Select for light sensor slave
const int SLAVE_SELECT_TEMP_PIN = 45;   // Chip Select for temperature sensor slave
const int TEMPERATURE_SENSOR_PIN = A8;  // Analog pin for temperature sensor
const int LIGHT_SENSOR_PIN = A9;        // Analog pin for light sensor

// Sensor calibration constants
const int ANALOG_MAX = 1023;            // Maximum analog read value
const int LIGHT_OUTPUT_MAX = 100;       // Maximum light percentage value
const int TEMP_ANALOG_MAX = 306;        // Temperature sensor maximum analog value
const int TEMP_OUTPUT_MAX = 150;        // Maximum temperature value in Celsius

// Timing constants
const int INTER_SLAVE_DELAY = 500;      // Delay between slave communications (ms)
const int SPI_TRANSFER_DELAY = 5;       // Delay after SPI transfer (ms)

// Global variables
int analogValue = 0;                    // Raw analog reading
uint8_t mappedValue = 0;                // Mapped sensor value

/**
 * Setup function - runs once at startup
 * Initializes serial communication, SPI interface, and sensor pins
 */
void setup() {
  Serial.begin(9600);                   // Initialize serial for debugging
  Serial.println("Starting Sensor SPI Master"); // Startup message
  
  initializeHardware();                 // Set up all hardware components
}

/**
 * Initializes all hardware pins and SPI interface
 */
void initializeHardware() {
  // Configure slave select pins as outputs and set inactive (HIGH)
  pinMode(SLAVE_SELECT_LIGHT_PIN, OUTPUT);
  pinMode(SLAVE_SELECT_TEMP_PIN, OUTPUT);
  digitalWrite(SLAVE_SELECT_LIGHT_PIN, HIGH);
  digitalWrite(SLAVE_SELECT_TEMP_PIN, HIGH);
  
  // Configure sensor pins as inputs
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  pinMode(TEMPERATURE_SENSOR_PIN, INPUT);
  
  // Initialize SPI communication
  SPI.begin();
}

/**
 * Main loop function - runs continuously
 * Reads sensors and transmits data to appropriate SPI slaves
 */
void loop() {
  // Read and transmit light sensor data
  processLightSensor();
  delay(INTER_SLAVE_DELAY);
  
  // Read and transmit temperature sensor data
  processTemperatureSensor();
  delay(INTER_SLAVE_DELAY);
  
  Serial.println();  // Empty line for better serial output formatting
}

/**
 * Reads light sensor, processes data, and transmits to light slave
 */
void processLightSensor() {
  // Read and map light sensor value
  analogValue = analogRead(LIGHT_SENSOR_PIN);
  mappedValue = map(analogValue, 0, ANALOG_MAX, 0, LIGHT_OUTPUT_MAX);
  
  // Transmit to light sensor slave
  transmitToSlave(SLAVE_SELECT_LIGHT_PIN, mappedValue);
  
  // Output to serial monitor
  Serial.print("Light: ");
  Serial.print(mappedValue);
  Serial.println("%");
}

/**
 * Reads temperature sensor, processes data, and transmits to temperature slave
 */
void processTemperatureSensor() {
  // Read and map temperature sensor value
  analogValue = analogRead(TEMPERATURE_SENSOR_PIN);
  mappedValue = map(analogValue, 0, TEMP_ANALOG_MAX, 0, TEMP_OUTPUT_MAX);
  
  // Transmit to temperature sensor slave
  transmitToSlave(SLAVE_SELECT_TEMP_PIN, mappedValue);
  
  // Output to serial monitor
  Serial.print("Temp: ");
  Serial.print(mappedValue);
  Serial.println(" C");
}

/**
 * Transmits data to a specific slave device
 * @param slaveSelectPin - The chip select pin for the target slave
 * @param data - The data byte to transmit
 */
void transmitToSlave(int slaveSelectPin, uint8_t data) {
  activateSlave(slaveSelectPin);    // Select the target slave
  SPI.transfer(data);               // Transmit the data
  delay(SPI_TRANSFER_DELAY);        // Brief delay for transmission stability
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