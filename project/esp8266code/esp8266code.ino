#include <Arduino.h>
#include <ArduinoJson.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"

#include "addons/RTDBHelper.h"

#include <SoftwareSerial.h>

// Wi-Fi and Firebase configuration
#define WIFI_SSID "vivo Y35"
#define WIFI_PASSWORD "12345678"
#define API_KEY "AIzaSyCCUFV-XhN4yUODnncdOiFFGi1Ozp99vI4" // Firebase API Key
#define DATABASE_URL "https://iot-monitoring-system-d1df9-default-rtdb.firebaseio.com"
#define USER_EMAIL "gondkaromkar53@gmail.com"
#define USER_PASSWORD "123456789"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Global variables
String str;
#define ESP_RX 4    // GPIO4 (D2), connects to Arduino TX (pin 11)
#define ESP_TX 5    // GPIO5 (D1), connects to Arduino RX (pin 10)
SoftwareSerial espSerial(ESP_RX, ESP_TX);  // RX, TX

unsigned long previousTime1 = 0;
unsigned long previousTime2 = 0;
const unsigned long interval1 = 5000;      // 5 seconds for sensor data
const unsigned long interval2 = 21600000;  // 6 hours for data logging

int pValue = 0;    // Motor status (0 = OFF, 1 = ON)
int lValue = 0;    // Water level
int m1Value = 0;   // Soil moisture sensor 1
int m2Value = 0;   // Soil moisture sensor 2
int hValue = 0;    // Humidity
int tValue = 0;    // Temperature
int manualValue = 0;

void setup() {
  Serial.begin(9600);       // Debug serial
  espSerial.begin(9600);    // Arduino communication
  
  Serial.println("\nESP8266 Starting");
  Serial.println("Serial pins:");
  Serial.println("RX: GPIO4 (D2) - Connect to Arduino TX (Pin 11)");
  Serial.println("TX: GPIO5 (D1) - Connect to Arduino RX (Pin 10)");
  
  // Clear any garbage data
  while(espSerial.available()) {
    espSerial.read();
  }

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  // Firebase configuration
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Wait for UID
  Serial.println("Getting User UID");
  while (auth.token.uid == "") {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Firebase setup completed.");
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - previousTime1 >= interval1) {
    updateWeatherData();
    previousTime1 = currentTime;
  }

  if (currentTime - previousTime2 >= interval2) {
    pushWeatherData();
    previousTime2 = currentTime;
  }
}

void updateWeatherData() {
  Serial.println("\n=== Starting Weather Data Update ===");
  
  // Try to get Arduino data multiple times
  bool dataReceived = false;
  for (int attempt = 0; attempt < 3; attempt++) {
    receiveAndSendToArduino();
    if (lValue != 0) {
      Serial.println("Water Level Reading: " + String(lValue));
      dataReceived = true;
      break;
    }
    delay(100);
  }
  
  if (!dataReceived) {
    Serial.println("Failed to get valid water level data");
    return;
  }
  
  // Update Firebase with both moisture values
  if (Firebase.ready()) {
    FirebaseJson json;
    json.add("humidity", hValue);
    json.add("moistureLevel", m1Value);
    json.add("moistureLevel2", m2Value);  // Add second moisture level
    json.add("temperature", tValue);
    json.add("waterLevel", lValue);
    json.add("motorStatus", pValue);
    json.add("timestamp", millis());

    if (Firebase.RTDB.setJSON(&fbdo, "/current", &json)) {
      Serial.println("Data sent successfully to /current.");
      Serial.println("Water Level: " + String(lValue));
      Serial.println("Moisture 1: " + String(m1Value));
      Serial.println("Moisture 2: " + String(m2Value));
      Serial.println("Temperature: " + String(tValue));
      Serial.println("Humidity: " + String(hValue));
    } else {
      Serial.printf("Failed to send data: %s\n", fbdo.errorReason().c_str());
    }
  }
}

void pushWeatherData() {
  receiveAndSendToArduino();

  if (Firebase.ready()) {
    FirebaseJson json;
    json.setDoubleDigits(3);

    // Add sensor data including both moisture values
    json.add("humidity", hValue);
    json.add("moistureLevel", m1Value);
    json.add("moistureLevel2", m2Value);
    json.add("temperature", tValue);
    json.add("waterLevel", lValue);
    json.add("timestamp", millis());

    if (Firebase.RTDB.pushJSON(&fbdo, "/current", &json)) {
      Serial.println("Data pushed successfully to /current.");
    } else {
      Serial.print("Failed to push data to /current: ");
      Serial.println(fbdo.errorReason());
    }
  }
}

void retriveManualMode() {
  if (Firebase.RTDB.getInt(&fbdo, "/manualMode", &manualValue)) {
    Serial.print("Manual mode value: ");
    Serial.println(manualValue);
    sendManualModeToArduino();  // Send to Arduino
  } else {
    Serial.println("Failed to retrieve manual mode value.");
  }
}

void receiveAndSendToArduino() {
  Serial.println("\nWaiting for Arduino data...");
  
  String buffer = "";
  bool messageStarted = false;
  unsigned long startTime = millis();
  
  // Wait up to 2 seconds for data
  while (millis() - startTime < 2000) {
    if (espSerial.available()) {
      char c = espSerial.read();
      
      // Start collecting data when we see 'P:'
      if (c == 'P') {
        messageStarted = true;
        buffer = "P";
        continue;
      }
      
      if (messageStarted) {
        buffer += c;
        
        // End of message
        if (c == '\n' || c == '\r') {
          buffer.trim();
          if (buffer.length() > 10) {  // Minimum valid message length
            Serial.println("Received from Arduino: " + buffer);
            parseSensorDataString(buffer);
            return;
          }
          messageStarted = false;
          buffer = "";
        }
      }
    }
    yield();  // Let ESP8266 handle background tasks
  }
  
  Serial.println("No valid data received from Arduino");
}

void parseSensorDataString(String inputString) {
    Serial.println("\n--- Parsing Data ---");
    Serial.println("Raw input: " + inputString);

    // Store previous values
    int prev_hValue = hValue;
    int prev_m1Value = m1Value;
    int prev_m2Value = m2Value;
    int prev_tValue = tValue;
    int prev_lValue = lValue;
    int prev_pValue = pValue;

    // Parse values
    int pIndex = inputString.indexOf("P:");
    int lIndex = inputString.indexOf("L:");
    int m1Index = inputString.indexOf("M1:");
    int m2Index = inputString.indexOf("M2:");
    int hIndex = inputString.indexOf("H:");
    int tIndex = inputString.indexOf("T:");

    if (pIndex != -1) {
        String motorValue = inputString.substring(pIndex + 2, inputString.indexOf(' ', pIndex));
        pValue = motorValue.toInt();
        Serial.println("Motor Status: " + String(pValue) + " (was: " + String(prev_pValue) + ")");
    }
    if (lIndex != -1) {
        String waterLevel = inputString.substring(lIndex + 2, inputString.indexOf(' ', lIndex));
        lValue = waterLevel.toInt();
        Serial.println("Water Level: " + String(lValue) + " (was: " + String(prev_lValue) + ")");
    }
    if (m1Index != -1) {
        String moisture1 = inputString.substring(m1Index + 3, inputString.indexOf(' ', m1Index));
        m1Value = moisture1.toInt();
        Serial.println("Moisture 1: " + String(m1Value) + " (was: " + String(prev_m1Value) + ")");
    }
    if (m2Index != -1) {
        String moisture2 = inputString.substring(m2Index + 3, inputString.indexOf(' ', m2Index));
        m2Value = moisture2.toInt();
        Serial.println("Moisture 2: " + String(m2Value) + " (was: " + String(prev_m2Value) + ")");
    }
    if (hIndex != -1) {
        String humidity = inputString.substring(hIndex + 2, inputString.indexOf('%', hIndex));
        hValue = humidity.toInt();
        Serial.println("Humidity: " + String(hValue) + " (was: " + String(prev_hValue) + ")");
    }
    if (tIndex != -1) {
        String temperature = inputString.substring(tIndex + 2, inputString.indexOf('C', tIndex));
        tValue = temperature.toInt();
        Serial.println("Temperature: " + String(tValue) + " (was: " + String(prev_tValue) + ")");
    }
}

// Update the sendManualModeToArduino function to send acknowledgment
void sendManualModeToArduino() {
  String command = "M:" + String(manualValue);  // Shorter command
  
  // Clear receive buffer
  while(espSerial.available()) {
    espSerial.read();
  }
  
  Serial.println("Sending to Arduino: " + command);
  
  // Send command multiple times to ensure reception
  for (int attempt = 0; attempt < 3; attempt++) {
    espSerial.println(command);
    delay(50);
  }
}
