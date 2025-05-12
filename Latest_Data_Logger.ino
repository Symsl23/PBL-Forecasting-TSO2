#include <WiFi.h>            // WiFi library for ESP32
#include <HTTPClient.h>      // HTTP client for sending requests
#include <ArduinoJson.h>     // JSON library for creating payload
#include <DHT.h>             // DHT sensor library

// Replace with your WiFi credentials
const char* ssid = "Redmi 13C";
const char* password = "arararar";

// Replace with your Google Apps Script Web App URL
const char* scriptURL = "https://script.google.com/macros/s/AKfycbz0xzAoNbme_R32kf42NGx3SNDkvSWpQTeS9oOvrG0_yBnouk51Hkt3bXXNYqExCPzD/exec";

// DHT11 setup
#define DHTPIN 16            // GPIO pin connected to DHT11
#define DHTTYPE DHT11        // Define sensor type
DHT dht(DHTPIN, DHTTYPE);    // Create DHT sensor object

// Interval to send data (in milliseconds)
const unsigned long sendInterval = 30000;  // 30 seconds
unsigned long previousMillis = 0;          // Store time of last data send

// Define thresholds for alerts
const float TEMP_HIGH_THRESHOLD = 35.0;
const float TEMP_LOW_THRESHOLD = 10.0;
const float HUMIDITY_HIGH_THRESHOLD = 85.0;
const float HUMIDITY_LOW_THRESHOLD = 20.0;

// Function prototype
bool sendDataToGoogleSheets(float temperature, float humidity, const String& status, const String& alert, int retries);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 DHT11 Sensor Logger Starting...");

  dht.begin();  // Initialize DHT sensor

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi connected. IP: " + WiFi.localIP().toString());
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if it's time to send data
  if (currentMillis - previousMillis >= sendInterval || previousMillis == 0) {
    previousMillis = currentMillis;

    // Read data from DHT11
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    // Check if reading failed
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("❌ Failed to read from DHT sensor!");
      return;
    }

    // Filter out unrealistic sensor values
    if (temperature < -40 || temperature > 80 || humidity < 0 || humidity > 100) {
      Serial.println("⚠️ Invalid sensor data. Skipping send.");
      return;
    }

    // Default status and alert
    String status = "Normal";
    String alert = "None";

    // Check if data is abnormal and set alerts
    if (temperature > TEMP_HIGH_THRESHOLD || temperature < TEMP_LOW_THRESHOLD ||
        humidity > HUMIDITY_HIGH_THRESHOLD || humidity < HUMIDITY_LOW_THRESHOLD) {
      status = "Abnormal";
      alert = "";
      if (temperature > TEMP_HIGH_THRESHOLD) alert += "High Temp ";
      if (temperature < TEMP_LOW_THRESHOLD) alert += "Low Temp ";
      if (humidity > HUMIDITY_HIGH_THRESHOLD) alert += "High Humidity ";
      if (humidity < HUMIDITY_LOW_THRESHOLD) alert += "Low Humidity ";
    }

    // Log readings and status to Serial Monitor
    Serial.printf("🌡️ Temp: %.2f °C | 💧 Humidity: %.2f %% | Status: %s | Alert: %s\n",
                  temperature, humidity, status.c_str(), alert.c_str());

    // Try sending data up to 3 times if it fails
    int retryCount = 0;
    const int maxRetries = 3;
    bool success = false;

    while (!success && retryCount < maxRetries) {
      success = sendDataToGoogleSheets(temperature, humidity, status, alert, retryCount);
      if (!success) {
        retryCount++;
        Serial.println("🔁 Retrying in 2 seconds...");
        delay(2000);
      }
    }

    if (!success) {
      Serial.println("❌ Failed to send data after retries.");
    }
  }
}

// Function to send the data to Google Sheets via HTTP POST
bool sendDataToGoogleSheets(float temperature, float humidity, const String& status, const String& alert, int retries) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ WiFi not connected.");
    return false;
  }

  HTTPClient http;
  http.begin(scriptURL);
  http.addHeader("Content-Type", "application/json");  // Set header

  // Prepare JSON data
  StaticJsonDocument<256> doc;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["status"] = status;
  doc["alert"] = alert;
  doc["retries"] = retries;

  String jsonString;
  serializeJson(doc, jsonString);  // Convert to JSON string

  // Send POST request
  int httpResponseCode = http.POST(jsonString);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("✅ Sent successfully! Response: " + response);
    http.end();
    return true;
  } else {
    Serial.printf("❌ POST failed. HTTP code: %d\n", httpResponseCode);
    http.end();
    return false;
  }
}
