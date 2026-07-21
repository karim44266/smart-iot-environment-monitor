/*
  SMART IOT ENVIRONMENT MONITOR — Firmware
  Board: ESP32 (simulated in Wokwi)
  Sensors: DHT22 (temperature + humidity), LDR (light level)
  Sends data to Adafruit IO every 15 seconds over WiFi.

  Fill in AIO_USERNAME and AIO_KEY below before running (Step 4 of the guide).
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// ---------- PIN SETUP ----------
#define DHTPIN 4        // DHT22 data pin connected to GPIO4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define LDRPIN 34        // LDR analog output connected to GPIO34

// ---------- WIFI (Wokwi's built-in simulated network) ----------
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ---------- ADAFRUIT IO CREDENTIALS ----------
// Get these from https://io.adafruit.com  ->  click the yellow "AIO Key" button
const char* AIO_USERNAME = "YOUR_ADAFRUIT_IO_USERNAME";
const char* AIO_KEY      = "YOUR_ADAFRUIT_IO_KEY";

// ---------- TIMING ----------
unsigned long lastSend = 0;
const unsigned long intervalMs = 15000; // send every 15 seconds

// Sends one value to one Adafruit IO feed
void sendToAdafruit(const char* feedKey, float value) {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  String url = "http://io.adafruit.com/api/v2/" + String(AIO_USERNAME) +
               "/feeds/" + String(feedKey) + "/data";

  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-AIO-Key", AIO_KEY);

  String payload = "{\"value\":" + String(value) + "}";
  int httpCode = http.POST(payload);

  Serial.print("  -> ");
  Serial.print(feedKey);
  Serial.print(" sent, server replied with code: ");
  Serial.println(httpCode);

  http.end();
}

void setup() {
  Serial.begin(115200);
  delay(500);

  dht.begin();

  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP address: " + WiFi.localIP().toString());
}

void loop() {
  if (millis() - lastSend < intervalMs) return;
  lastSend = millis();

  float temperature = dht.readTemperature();
  float humidity    = dht.readHumidity();
  int   lightRaw     = analogRead(LDRPIN);              // 0-4095
  float lightPercent = map(lightRaw, 0, 4095, 0, 100);   // convert to 0-100%

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Sensor read failed, skipping this cycle.");
    return;
  }

  Serial.println("---- New reading ----");
  Serial.printf("Temperature: %.1f C\n", temperature);
  Serial.printf("Humidity:    %.1f %%\n", humidity);
  Serial.printf("Light:       %.0f %%\n", lightPercent);

  sendToAdafruit("temperature", temperature);
  sendToAdafruit("humidity", humidity);
  sendToAdafruit("light", lightPercent);
}
