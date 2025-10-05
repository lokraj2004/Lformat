#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

const char* deviceName = "ESP32_BT_DEVICE";

// Example sensors
const char* sensorNames[] = {"Temp", "Humid", "Pressure"};
const char* sensorUnits[] = {"C", "%", "Pa"};
float sensorValues[] = {25.4, 60.2, 101325};
int sensorIds[] = {1, 2, 3};

unsigned long previousMillis = 0;
const long interval = 1000; // Send data every 1 second

void setup() {
  Serial.begin(115200);
  if(!SerialBT.begin(deviceName)) {
    Serial.println("❌ Bluetooth initialization failed!");
  } else {
    Serial.println("✅ Bluetooth initialized. Waiting for connection...");
  }
}

void loop() {
  // Check if a client is connected
  if (SerialBT.hasClient()) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      // Send all sensors
      for (int i = 0; i < 3; i++) {
        String line = LBFormat(sensorNames[i],sensorValues[i],sensorUnits[i],sensorIds[i]);
        // String line = String(sensorNames[i]) + ";" +
        //               String(sensorValues[i]) + ";" +
        //               String(sensorUnits[i]) + ";" +
        //               String(sensorIds[i]) + "\n";

        SerialBT.print(line);
        Serial.print("📤 Sent: ");
        Serial.print(line);
      }

      // Optionally, update values for demo
      sensorValues[0] += 0.1; // Temp
      sensorValues[1] += 0.2; // Humidity
      sensorValues[2] += 1;   // Pressure
    }
  }
}
