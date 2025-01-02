#include <Arduino.h>
#include <ArduinoJson.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#include <SoftwareSerial.h>

// Wi-Fi and Firebase configuration
#define WIFI_SSID "vivo V29e"
#define WIFI_PASSWORD "bhumi@02"
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
SoftwareSerial espSerial(4, 0);  // RX (D2 = 4), TX (D3 = 0)

unsigned long previousTime1 = 0;
unsigned long previousTime2 = 0;
const unsigned long interval1 = 10000;     // 10 seconds
const unsigned long interval2 = 21600000; // 6 hours

int pValue = 0; // Motor status (0 = OFF, 1 = ON)
int lValue = 0; // Water level
int mValue = 0; // Soil moisture
int hValue = 0; // Humidity
int tValue = 0; // Temperature
int manualValue = 0;

void setup() {
  Serial.begin(9600); // Initialize serial communication
  espSerial.begin(9600); // Initialize ESP8266 serial communication

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
  receiveAndSendToArduino();

  if (Firebase.ready()) {
    FirebaseJson json;
    json.setDoubleDigits(3); // Ensure floating-point precision if needed

    // Debug: Print parsed values before sending
    Serial.println("Preparing data for Firebase:");
    Serial.print("Humidity: "); Serial.println(hValue);
    Serial.print("Moisture Level: "); Serial.println(mValue);
    Serial.print("Temperature: "); Serial.println(tValue);
    Serial.print("Water Level: "); Serial.println(lValue);
    Serial.print("Motor Status: "); Serial.println(pValue);

    // Add sensor data to JSON
    json.add("humidity", hValue);
    json.add("moistureLevel", mValue);
    json.add("temperature", tValue);
    json.add("waterLevel", lValue);
    json.add("timestamp", millis());

    // Update "current" node in Firebase
    if (Firebase.RTDB.setJSON(&fbdo, "/current", &json)) {
      Serial.println("Data sent successfully to /current.");
    } else {
      Serial.print("Failed to send data to /current: ");
      Serial.println(fbdo.errorReason());
    }

    // Update "motor" node
    FirebaseJson motorJson;
    motorJson.add("status", (pValue == 1) ? "ON" : "OFF");

    if (Firebase.RTDB.setJSON(&fbdo, "/motor", &motorJson)) {
      Serial.println("Motor status sent successfully to /motor.");
    } else {
      Serial.print("Failed to send motor status to /motor: ");
      Serial.println(fbdo.errorReason());
    }

    retriveManualMode();
  }
}

void pushWeatherData() {
  receiveAndSendToArduino();

  if (Firebase.ready()) {
    FirebaseJson json;
    json.setDoubleDigits(3);

    // Add sensor data to JSON
    json.add("humidity", hValue);
    json.add("moistureLevel", mValue);
    json.add("temperature", tValue);
    json.add("waterLevel", lValue);
    json.add("timestamp", millis());

    // Push to the "current" node in Firebase
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
  } else {
    Serial.println("Failed to retrieve manual mode value.");
  }
}

void receiveAndSendToArduino() {
    if (espSerial.available()) {
        str = espSerial.readString();
        Serial.println("Received raw data from Arduino: " + str); // Debug raw data
        parseSensorDataString(str); // Parse the received data
        espSerial.flush();
    } else {
        Serial.println("No data received from Arduino."); // Debug no data
    }
}

void parseSensorDataString(String inputString) {
  Serial.println("Raw input string: " + inputString); // Debug raw input

  int pIndex = inputString.indexOf("P:");
  int lIndex = inputString.indexOf("L:");
  int mIndex = inputString.indexOf("M:");
  int hIndex = inputString.indexOf("H:");
  int tIndex = inputString.indexOf("T:");

  if (pIndex != -1) {
    String motorValue = inputString.substring(pIndex + 2, inputString.indexOf(' ', pIndex));
    pValue = motorValue.toInt();
    Serial.print("Parsed P: "); Serial.println(pValue); // Debug
  }
  if (lIndex != -1) {
    String waterLevel = inputString.substring(lIndex + 2, inputString.indexOf(' ', lIndex));
    lValue = waterLevel.toInt();
    Serial.print("Parsed L: "); Serial.println(lValue); // Debug
  }
  if (mIndex != -1) {
    String moistureLevel = inputString.substring(mIndex + 2, inputString.indexOf(' ', mIndex));
    mValue = moistureLevel.toInt();
    Serial.print("Parsed M: "); Serial.println(mValue); // Debug
  }
  if (hIndex != -1) {
    String humidity = inputString.substring(hIndex + 2, inputString.indexOf('%', hIndex));
    hValue = humidity.toInt();
    Serial.print("Parsed H: "); Serial.println(hValue); // Debug
  }
  if (tIndex != -1) {
    String temperature = inputString.substring(tIndex + 2, inputString.indexOf('C', tIndex));
    tValue = temperature.toInt();
    Serial.print("Parsed T: "); Serial.println(tValue); // Debug
  }
}
