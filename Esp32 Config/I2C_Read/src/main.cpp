#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Create the sensor object
Adafruit_BME280 bme; 

void setup() {
  Serial.begin(115200);
  while(!Serial); // Wait for Serial Monitor to open

  Serial.println(F("BME280 Test"));

  // Start I2C on pins 21 (SDA) and 22 (SCL)
  Wire.begin(21, 22);

  // Initialize the sensor. The default I2C address for BME280 is 0x76, but it can also be 0x77 depending on the wiring.
  unsigned status = bme.begin(0x76, &Wire);  
  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1) delay(10);
  }

  Serial.println("-- Sensor Found & Initialized --");
}

void loop() {
  // Read and print Temperature
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");

  // Read and print Humidity
  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  // Read and print Pressure
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F); // Convert Pa to hPa
  Serial.println(" hPa");

  Serial.println("-----------------------");
  delay(2000); // Wait 2 seconds between readings
}