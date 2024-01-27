#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <ArduinoJson.h>

BLEClient* f_pClient;
BLERemoteCharacteristic* f_pCharacteristic;

void f_onNotifyCallback
  (BLERemoteCharacteristic* pBLERemoteCharacteristic,
    uint8_t* pData,
    size_t length,
    bool isNotify)
{
  Serial.println("Notification received!");
  String value = "";
  for (int i = 0; i < length; i++)
  {
    value += (char)pData[i];
  }
  Serial.print("Received value: ");
  Serial.println(value.c_str());
  DynamicJsonDocument jsonDoc(256);
  DeserializationError error = deserializeJson(jsonDoc, value);

  if (error)
  {
    Serial.print("JSON parsing failed! Error: ");
    Serial.println(error.c_str());
    Serial.println("Check if the JSON data format is correct.");
    return;
  }

  if (jsonDoc.containsKey("armed") && jsonDoc.containsKey("door"))
  {
    bool isArmed = jsonDoc["armed"];
    String doorStatus = jsonDoc["door"];
    Serial.print("Armed: ");
    Serial.println(isArmed);
    Serial.print("Door Status: ");
    Serial.println(doorStatus);
  } 
  else
  {
    Serial.println("Missing expected keys ('armed' and 'door') in JSON data.");
  }
}

void setup()
{
  Serial.begin(9600);
  BLEDevice::init("ESP32Client");
  BLEScan* pBLEScan = BLEDevice::getScan();
  Serial.println("Scanning for NanoBLE");
  BLEScanResults results = pBLEScan->start(5);
  for (int i = 0; i < results.getCount(); i++)
  {
    BLEAdvertisedDevice device = results.getDevice(i);

    if (device.haveServiceUUID() && device.getServiceUUID().equals(BLEUUID("19B10000-E8F2-537E-4F6C-D104768A1214")))
    {
      Serial.print("Connecting to NanoBLE on device: ");
      Serial.println(device.getAddress().toString().c_str());
      f_pClient = BLEDevice::createClient();  

      if (f_pClient->connect(&device))
      {
        Serial.println("Connected!");
        BLERemoteService* pRemoteService = f_pClient->getService("19B10000-E8F2-537E-4F6C-D104768A1214");
        f_pCharacteristic = pRemoteService->getCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214");

        if (f_pCharacteristic)
        {
          f_pCharacteristic->registerForNotify(f_onNotifyCallback);
          f_pCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)"\x01\x00", 2, true);
          Serial.println("Waiting for notifications...");
        } 
        else
        {
          Serial.println("Error: Characteristic not found!");
          f_pClient->disconnect();
        }
      } 
      else
      {
        Serial.println("Error: Connection failed!");
      }
    }
  }
}

void loop()
{
}
