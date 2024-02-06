#include "NanoCommunication.h"

namespace NanoCommunication 
{

    MyClientCallback::MyClientCallback() {}  
    
    void MyClientCallback::onConnect(BLEClient* pClient) 
    {
        Serial.println("Connected to Nano");
    }

    void MyClientCallback::onDisconnect(BLEClient* pClient) 
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
            readDoorSensor();
        } else 
        {
            connectToNano();
        }
    }

    void Nano::connectToNano() 
    {
        BLEAddress nanoAddress("1E:F7:A4:F0:AF:DF");
        pClient = BLEDevice::createClient();
        pClient->setClientCallbacks(new MyClientCallback());

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

    void Nano::readDoorSensor() 
    {
        uint8_t sensorValue = pSensorCharacteristic->readUInt8();
        Serial.print("Received Sensor Value: ");
        Serial.println(sensorValue);
        
        if (sensorValue == 1) 
        {
            Serial.println("Door opened");
        } else 
        {
            Serial.println("Door closed");
        }
        delay(1000);
    }

    uint8_t Nano::getSensorValue() 
    {
        return pSensorCharacteristic->readUInt8();
    }

} 
