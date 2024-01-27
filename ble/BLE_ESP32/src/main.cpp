#include <Wire.h>
#include <ArduinoBLE.h>

const int f_doorSensorPin = 2;

BLEService f_securityService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLECharacteristic f_doorCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLENotify, 20);

int f_lastDoorStatus = LOW;
bool f_isArmed = false;  

void setup()
{
  Serial.begin(9600);
  pinMode(f_doorSensorPin, INPUT);
  Serial.println("Setting up BLE...");

  if (!BLE.begin())
  {
    Serial.println("Failed to initialize BLE!");
    while (1);
  }

  BLE.setLocalName("NANO_33_BLE");
  BLE.setAdvertisedService(f_securityService);
  f_securityService.addCharacteristic(f_doorCharacteristic);
  BLE.addService(f_securityService);
  Serial.println("BLE setup completed successfully");

  while (!BLE.advertise())
  {
    Serial.println("Still processing...");
    delay(500);
  }
  Serial.println("Waiting for Bluetooth connection...");
}

void loop()
{
  BLEDevice central = BLE.central();

  if (central)
  {
    Serial.println("Bluetooth connected successfully!");

    while (BLE.connected())
    {
      int f_doorStatus = digitalRead(f_doorSensorPin);

      if (f_doorStatus != f_lastDoorStatus) {
        String jsonData = "{\"armed\":" + String(f_isArmed) + ",\"door\":\"" + String(f_doorStatus == HIGH ? "closed" : "open") + "\"}";
        Serial.print("JSON Data Sent: ");
        Serial.println(jsonData);
        f_doorCharacteristic.writeValue(jsonData.c_str());
        Serial.print("JSON Data: ");
        Serial.println(jsonData);
        Serial.print("Connection Status: Connected");
        delay(1000);
        f_doorCharacteristic.writeValue("");  
        f_lastDoorStatus = f_doorStatus;
      }
      delay(1000);
    }
    Serial.println("Bluetooth disconnected. ");
  }
}

