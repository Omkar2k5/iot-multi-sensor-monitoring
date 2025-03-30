#include "DHT.h"
#include <SoftwareSerial.h>

// Define pins
const int motorPin = 7;
#define WATER_MOISTURE_PIN A0
#define WATER_MOISTURE_PIN2 A2  // Add second moisture sensor pin
#define WATER_LEVEL_PIN A1
#define DHT11_PIN 2        // Digital pin 2 for DHT11
#define DHTTYPE DHT11

// Software Serial for ESP8266 communication
SoftwareSerial mySerial(10, 11);  // RX (10), TX (11)

// DHT sensor object
DHT dht(DHT11_PIN, DHTTYPE);

// Update thresholds for better moisture control
int moistureThreshold = 40;   // New single threshold value

// Global variables
String message;
int value_motor_status = 0;
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 3000; // Non-blocking delay

void setup() {
  // Initialize pins
  pinMode(motorPin, OUTPUT);
  pinMode(DHT11_PIN, INPUT);

  // Initialize Serial communication
  Serial.begin(9600);  // Serial monitor
  mySerial.begin(9600);  // ESP8266 communication

  // Initialize DHT sensor
  dht.begin();
  delay(2000);  // Give DHT time to stabilize

  // Test DHT
  Serial.println("Testing DHT sensor...");
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println("DHT sensor not reading! Check connections");
    Serial.println("VCC → 5V");
    Serial.println("DATA → Pin 2");
    Serial.println("GND → GND");
  } else {
    Serial.println("DHT sensor working!");
    Serial.println("Initial readings:");
    Serial.println("Humidity: " + String(h) + "%");
    Serial.println("Temperature: " + String(t) + "°C");
  }

  Serial.println("Arduino Initialized");
}

void loop() {
  if (millis() - lastUpdate >= updateInterval) {
    lastUpdate = millis();

    // Read water level sensor with averaging
    long waterLevelSum = 0;
    for(int i = 0; i < 5; i++) {  // Take 5 readings
      waterLevelSum += analogRead(WATER_LEVEL_PIN);
      delay(10);
    }
    int raw_water_level = waterLevelSum / 5;  // Average reading
    
    // Map water level (inverted since sensor reads high when dry)
    int value_water_level = map(raw_water_level, 750, 250, 0, 100);
    value_water_level = constrain(value_water_level, 0, 100);

    // Debug water level readings
    Serial.println("\nWater Level Readings:");
    Serial.println("Raw Value: " + String(raw_water_level));
    Serial.println("Mapped Value: " + String(value_water_level) + "%");

    // Read sensor values
    Serial.println("\n--- Reading Sensors ---");
    
    // Read DHT sensor
    float value_humidity = 0;
    float value_temperature = 0;
    
    for (int i = 0; i < 3; i++) {
      value_humidity = dht.readHumidity();
      value_temperature = dht.readTemperature();
      
      if (!isnan(value_humidity) && !isnan(value_temperature)) {
        Serial.println("DHT read successful!");
        break;
      }
      delay(1000);
    }

    // Read moisture sensors
    int value_water_moisture = 100 - map(analogRead(WATER_MOISTURE_PIN), 400, 1023, 0, 100);
    value_water_moisture = constrain(value_water_moisture, 0, 100);
    
    // Read second moisture sensor
    int value_water_moisture2 = 100 - map(analogRead(WATER_MOISTURE_PIN2), 400, 1023, 0, 100);
    value_water_moisture2 = constrain(value_water_moisture2, 0, 100);

    // Debug output
    Serial.println("\nSensor Values:");
    Serial.println("Water Level (raw): " + String(raw_water_level));
    Serial.println("Water Level (mapped): " + String(value_water_level) + "%");
    Serial.println("Moisture 1 (raw): " + String(analogRead(WATER_MOISTURE_PIN)));
    Serial.println("Moisture 1 (mapped): " + String(value_water_moisture) + "%");
    Serial.println("Moisture 2 (raw): " + String(analogRead(WATER_MOISTURE_PIN2)));
    Serial.println("Moisture 2 (mapped): " + String(value_water_moisture2) + "%");
    Serial.println("Humidity: " + String(value_humidity) + "%");
    Serial.println("Temperature: " + String(value_temperature) + "°C");

    // New motor control logic based on either sensor being below threshold
    if (value_water_moisture < moistureThreshold || value_water_moisture2 < moistureThreshold) {
      // Turn ON pump if either moisture sensor is too low and water level is sufficient
      if (value_water_level > 20) {  // Make sure there's enough water
        value_motor_status = 1;
        Serial.println("Motor ON - Low Moisture Detected");
        if (value_water_moisture < moistureThreshold) {
          Serial.println("Sensor 1 triggered motor: " + String(value_water_moisture) + "%");
        }
        if (value_water_moisture2 < moistureThreshold) {
          Serial.println("Sensor 2 triggered motor: " + String(value_water_moisture2) + "%");
        }
      } else {
        value_motor_status = 0;
        Serial.println("Motor OFF - Water Level Too Low");
      }
    } 
    else if (value_water_moisture >= moistureThreshold && value_water_moisture2 >= moistureThreshold) {
      // Turn OFF pump only if both moisture levels are above threshold
      value_motor_status = 0;
      Serial.println("Motor OFF - Both Moisture Levels Sufficient");
      Serial.println("Sensor 1: " + String(value_water_moisture) + "%");
      Serial.println("Sensor 2: " + String(value_water_moisture2) + "%");
    }
    
    // Apply motor control
    motorControl();

    // Send sensor data with retries
    sendSensorDataToESP(value_humidity, value_water_moisture, value_water_moisture2, value_water_level, value_temperature);
  }
}

void motorControl() {
  digitalWrite(motorPin, value_motor_status ? HIGH : LOW);
  Serial.print("Motor set to: ");
  Serial.println(value_motor_status ? "ON" : "OFF");
}

void sendSensorDataToESP(float humidity, int moisture1, int moisture2, int waterLevel, float temperature) {
  String dataToSend = "P:" + String(value_motor_status) +
                      " L:" + String(waterLevel) +
                      " M1:" + String(moisture1) +
                      " M2:" + String(moisture2) +  // Add moisture2 to the data string
                      " H:" + String((int)humidity) + "%" +
                      " T:" + String((int)temperature) + "C";

  // Send data multiple times to ensure reception
  for (int attempt = 0; attempt < 3; attempt++) {
    Serial.println("\n--- Sending Data to ESP (Attempt " + String(attempt + 1) + ") ---");
    mySerial.println(dataToSend);
    delay(100);
  }
  
  Serial.println("Sent to ESP: " + dataToSend);
}