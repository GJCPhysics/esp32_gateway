#include "NanoCommunication.h"

namespace NanoCommunication 
{
    BLE_GATT_ClientCallback::BLE_GATT_ClientCallback() {}

    void BLE_GATT_ClientCallback::onConnect(BLEClient* pClient) 
    {
        Serial.println("Connected to Nano");
    }

    void BLE_GATT_ClientCallback::onDisconnect(BLEClient* pClient) 
    {
        Serial.println("Disconnected from Nano");
    }

    Nano::Nano() : pClient(nullptr), pSensorCharacteristic(nullptr) {}

    void Nano::setup() 
    {
        BLEDevice::init("ESP DEVICE");
        connectToNano();
    }

    void Nano::loop() 
    {
        if (pClient->isConnected()) 
        {
            readSensorStatus();
        } else 
        {
            connectToNano();
        }
    }

    void Nano::connectToNano() 
    {
        BLEAddress nanoAddress("1E:F7:A4:F0:AF:DF");
        pClient = BLEDevice::createClient();
        pClient->setClientCallbacks(new BLE_GATT_ClientCallback());

        if (!pClient->connect(nanoAddress)) 
        {
            Serial.println("Failed to connect to Nano");
            delay(1000);
            return;
        }
        BLERemoteService* pService = pClient->getService(BLEUUID("180A"));
        pSensorCharacteristic = pService->getCharacteristic(BLEUUID("2A56"));

        if (pSensorCharacteristic == nullptr) 
        {
            Serial.println("Failed to find characteristic");
            pClient->disconnect();
            delay(1000);
            return;
        }
        Serial.println("Connected to Nano");
    }

    void Nano::readSensorStatus() 
    {
        uint8_t SensorValue = pSensorCharacteristic->readUInt8();
        Serial.print("Received Door Sensor Value: ");
        Serial.println(SensorValue);

        if (SensorValue == 0) 
        {
            Serial.println("Door closed");
            Serial.println("Smoke not detected");
        } 
        else if (SensorValue == 1)
        {
            Serial.println("Door opened");
            Serial.println("Smoke not detected");
        }
        else if (SensorValue == 2)
        {
            Serial.println("Door closed");
            Serial.println("Smoke detected");
        }
        else
        {
            Serial.println("Door opened");
            Serial.println("Smoke detected");
        }
        delay(3000);
    }

    uint8_t Nano::getDoorSensorValue() 
    {
    
       
    }

    uint8_t Nano::getSmokeSensorValue() 
    {
        
    }
}
