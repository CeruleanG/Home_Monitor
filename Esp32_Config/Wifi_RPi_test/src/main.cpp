#include <Arduino.h>
#include <WiFi.h>
#include "secret.h"

// Track the last time we tried to reconnect to avoid spamming the router
unsigned long lastReconnectAttempt = 0;
const unsigned long RECONNECT_INTERVAL = 10000; // 10 seconds
const unsigned long MESSAGE_INTERVAL = 10000; // 10 seconds

const char* host     = "10.42.0.1";
const uint16_t port  = 1883;

// Track the previous WiFi status to detect changes
bool previousWifiStatus = false;

void messageToWiFi() {
    WiFiClient client;

    // Attempt to open a TCP connection to the Pi
    if (!client.connect(host, port)) {
        Serial.println("Connection to RPi failed.");
        return;
    }

    // This is the raw data payload we are streaming to your Pi console
    String payload = "Test: Hello from ESP32 node! \n";
    
    Serial.println("Sending data packet...");
    client.print(payload);

    Serial.println("Closing connection.");
    client.stop();
}

void connectToWiFi() {
  Serial.printf("\nAttempting to connect...");
  WiFi.begin(SECRET_SSID, SECRET_PASS);
}

void printWifiStatus() {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("[WiFi] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
    } else {
        Serial.printf("[WiFi] Disconnected. Status: %d\n", WiFi.status());
    }
}

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);

  Serial.println("\n=== ESP32 WiFi Client Starting ===\n");
  connectToWiFi();
}

void loop() {
  bool currentlyConnected = (WiFi.status() == WL_CONNECTED);

  // LED status
  digitalWrite(2, currentlyConnected ? HIGH : LOW);

  // Print status changes only
  if (currentlyConnected != previousWifiStatus) {
      printWifiStatus();
      previousWifiStatus = currentlyConnected;
  }

  // Reconnection logic
  if (!currentlyConnected) {
      unsigned long currentMillis = millis();
      
      if (currentMillis - lastReconnectAttempt >= RECONNECT_INTERVAL) {
          lastReconnectAttempt = currentMillis;
          connectToWiFi();
      }
  }
  else {
      unsigned long currentMillis = millis();
      
      if (currentMillis - lastReconnectAttempt >= MESSAGE_INTERVAL) {
          lastReconnectAttempt = currentMillis;
          messageToWiFi();
      }
  }

  // Small delay to prevent CPU hogging
  delay(100);
}