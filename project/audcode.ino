#include <dht.h>
#include <SoftwareSerial.h>

// Define pins
const int motorPin = 8;
#define WATER_MOISTURE_PIN A0
#define WATER_LEVEL_PIN A1
#define DHT11_PIN 2

// Software Serial for ESP8266 communication
SoftwareSerial mySerial(10, 11);  // RX (Arduino), TX (ESP8266)

// DHT sensor object
dht DHT;

// Thresholds
int waterLevelThreshold = 30;
int moistureThreshold = 30;

// Global variables
String message;
int value_motor_status = 0;
int manualValue = 0; // Manual mode value
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 3000; // Non-blocking delay

void setup() {
  // Initialize pins
  pinMode(motorPin, OUTPUT);

  // Initialize Serial communication
  Serial.begin(9600);  // Serial monitor
  mySerial.begin(9600);  // ESP8266 communication

  Serial.println("Arduino Initialized");
}

void loop() {
  // Non-blocking delay for periodic updates
  if (millis() - lastUpdate >= updateInterval) {
    lastUpdate = millis();

    // Read sensor values
    int WATER_MIN_LEVEL = 0, WATER_MAX_LEVEL = 250;
    int MOISTURE_MIN_LEVEL = 400, MOISTURE_MAX_LEVEL = 1023;

    int value_water_level = map(analogRead(WATER_LEVEL_PIN), WATER_MIN_LEVEL, WATER_MAX_LEVEL, 0, 100) / 2;
    int value_water_moisture = 100 - map(analogRead(WATER_MOISTURE_PIN), MOISTURE_MIN_LEVEL, MOISTURE_MAX_LEVEL, 0, 100);
    int value_dht = DHT.read11(DHT11_PIN);
    int value_humidity = DHT.humidity;
    int value_temperature = DHT.temperature;

    // Handle invalid DHT readings
    if (isnan(value_temperature) || isnan(value_humidity)) {
      Serial.println("Failed to read from DHT sensor. Using default values.");
      value_temperature = 25;
      value_humidity = 50;
    }

    // Motor control logic (both automatic and manual)
    if (manualValue != 0) {
      // Manual mode: motor control based on Firebase button
      value_motor_status = (manualValue == 1) ? 1 : 0;  // 1 = ON, 0 = OFF
    } else if (value_water_moisture < moistureThreshold) {
      // Auto mode: motor ON when moisture is low
      value_motor_status = 1;
    } else {
      // Auto mode: motor OFF when moisture level is sufficient
      value_motor_status = 0;
    }
    
    motorControl();

    // Send sensor data to ESP8266
    sendSensorDataToESP(value_humidity, value_water_moisture, value_water_level, value_temperature);

    // Debug output
    Serial.print("P:"); Serial.print(value_motor_status);
    Serial.print(" L:"); Serial.print(value_water_level);
    Serial.print(" M:"); Serial.print(value_water_moisture);
    Serial.print(" H:"); Serial.print(value_humidity); Serial.print("% ");
    Serial.print(" T:"); Serial.print(value_temperature); Serial.println("C");
  }

  // Receive and handle commands from ESP8266
  receiveAndHandleESPCommands();
}

void motorControl() {
  // Control motor based on the value_motor_status
  digitalWrite(motorPin, value_motor_status ? HIGH : LOW);
}

void sendSensorDataToESP(int humidity, int moisture, int waterLevel, int temperature) {
  // Format the data to send to ESP8266
  String dataToSend = "P:" + String(value_motor_status) +
                      " L:" + String(waterLevel) +
                      " M:" + String(moisture) +
                      " H:" + String(humidity) + "%" +
                      " T:" + String(temperature) + "C";
  Serial.print("Sending to ESP: "); // Debug output
  Serial.println(dataToSend); // Print the data being sent
  mySerial.println(dataToSend);  // Send data to ESP8266
}

void receiveAndHandleESPCommands() {
  // Check if data is available from ESP8266
  if (mySerial.available()) {
    message = mySerial.readString();  // Read the incoming message
    parseManualValueString(message);  // Parse the manual control value
    mySerial.flush();  // Clear the serial buffer
  }
}

void parseManualValueString(String inputString) {
  // Look for the keyword "manualValue:" and extract the value
  int colonIndex = inputString.indexOf("manualValue:");
  if (colonIndex != -1) {
    manualValue = inputString.substring(colonIndex + 12).toInt();  // Offset to skip "manualValue:"
    Serial.print("Manual Value: ");
    Serial.println(manualValue);
  } else {
    manualValue = 0;  // Default to auto mode if no manualValue is found
    Serial.println("Keyword 'manualValue' not found in input");
  }
}
