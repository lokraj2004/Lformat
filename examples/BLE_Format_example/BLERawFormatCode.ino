

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Lformat.h>

// UUIDs for BLE service and single characteristic
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define DATA_CHAR_UUID      "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// Device name
const char* deviceName = "ESP32_BLE_DEVICE_SINGLE";

// Sensor values
float tempValue = 25.4;
float humidValue = 60.2;
float pressureValue = 101325;

// BLE objects
BLEServer* pServer = nullptr;
BLECharacteristic* pDataChar = nullptr;

bool deviceConnected = false;
bool oldDeviceConnected = false;
bool sendingEnabled = false;   // Controlled by "Start"/"Stop"
unsigned long previousMillis = 0;
const long interval = 1000; // send every 1 second

// ================== BLE CALLBACKS ====================

// Server connection callbacks
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) override {
    deviceConnected = true;
    Serial.println("✅ Client connected.");
  }

  void onDisconnect(BLEServer* pServer) override {
    deviceConnected = false;
    Serial.println("❌ Client disconnected.");
  }
};

class MyDataCallbacks: public BLECharacteristicCallbacks {
 void onWrite(BLECharacteristic* pCharacteristic) {
      String value = pCharacteristic->getValue();  // Directly use Arduino String
      value.trim();
      Serial.print("📥 Received command: ");
      Serial.println(value);


      if (value.equalsIgnoreCase("START")) {
        sendingEnabled = true;
        Serial.println("▶️ Data streaming started.");
      } 
      else if (value.equalsIgnoreCase("STOP")) {
        sendingEnabled = false;
        Serial.println("⏹ Data streaming stopped.");
      } 
      else {
        Serial.println("⚠️ Unknown command received.");
      }
    }
};

// ================== SETUP ====================

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE server...");

  BLEDevice::init(deviceName);
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService* pService = pServer->createService(SERVICE_UUID);

  // Single characteristic: can READ, NOTIFY, and WRITE
  pDataChar = pService->createCharacteristic(
    DATA_CHAR_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_NOTIFY |
    BLECharacteristic::PROPERTY_WRITE
  );
  pDataChar->addDescriptor(new BLE2902());
  pDataChar->setCallbacks(new MyDataCallbacks());

  pService->start();

  // Start advertising
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("✅ Advertising started. Waiting for client...");
}

// ================== LOOP ====================

void loop() {
  if (deviceConnected && sendingEnabled) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      // Update sensor values
      tempValue += 0.1;
      humidValue += 0.2;
      pressureValue += 1;

      // Send temperature
      String tempLine = "Temperature;" + String(tempValue, 2) + ";C;1\n";
      pDataChar->setValue(tempLine.c_str());
      pDataChar->notify();
      Serial.print("📤 Sent BLE notification: ");
      Serial.println(tempLine);

      // Send humidity
      String humidLine = "Humidity;" + String(humidValue, 2) + ";%RH;2\n";
      pDataChar->setValue(humidLine.c_str());
      pDataChar->notify();
      Serial.print("📤 Sent BLE notification: ");
      Serial.println(humidLine);

      // Send pressure
      String pressureLine = LBFormat("Pressure", pressureValue, "Pa", 3);
      pDataChar->setValue(pressureLine.c_str());
      pDataChar->notify();
      Serial.print("📤 Sent BLE notification: ");
      Serial.println(pressureLine);
    }
  }

  // Restart advertising if disconnected
  if (!deviceConnected && oldDeviceConnected) {
    delay(500);
    pServer->startAdvertising();
    Serial.println("🔄 Restarting advertising...");
  }

  oldDeviceConnected = deviceConnected;
}
