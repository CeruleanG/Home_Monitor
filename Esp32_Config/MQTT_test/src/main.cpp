#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "secret.h"

// Track the last time we tried to reconnect to avoid spamming the router
unsigned long lastReconnectAttempt = 0;
const unsigned long RECONNECT_INTERVAL = 10000; // 10 seconds
const unsigned long MESSAGE_INTERVAL = 10000; // 10 seconds

const char* host     = "10.42.0.1";
const uint16_t port  = 1883;

const char* mqtt_topic = "esp32/node1/data";
const char* client_id  = "ESP32_Sensor_Node_1";

// Track the previous WiFi status to detect changes
bool previousWifiStatus = false;

WiFiClient client;
PubSubClient mqttClient(client);

void connectToMQTT() {
    // Check if we are connected to the MQTT broker, if not, connect
    if (!mqttClient.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Connects using a unique Client ID. (You can also add user/pass here if configured)
        if (mqttClient.connect(client_id)) {
            Serial.println("connected to MQTT Broker!");
        } else {
            Serial.printf("failed, rc=%d. Will try again next interval.\n", mqttClient.state());
        }
    }
}

void messageToWiFi() {
    mqttClient.setServer(host, port);
    if (!mqttClient.connected()) {
        connectToMQTT();
    }

    if (mqttClient.connected()) {
        String payload = "Hello from ESP32 node via MQTT!";
        
        Serial.println("Publishing MQTT packet...");
        mqttClient.publish(mqtt_topic, payload.c_str());
    }
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

  Serial.println("\n=== ESP32 MQTT Client Starting ===\n");
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

      mqttClient.loop(); // Keep MQTT connection alive
      
      if (currentMillis - lastReconnectAttempt >= MESSAGE_INTERVAL) {
          lastReconnectAttempt = currentMillis;
          messageToWiFi();
      }
  }

  // Small delay to prevent CPU hogging
  delay(100);
}