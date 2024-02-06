#ifndef NANO_COMMUNICATION_H
#define NANO_COMMUNICATION_H
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

namespace NanoCommunication 
{
    class MyClientCallback : public BLEClientCallbacks 
    {
    public:
        MyClientCallback();  
        void onConnect(BLEClient* pClient);
        void onDisconnect(BLEClient* pClient);
    };

    class Nano 
    {
    public:
        Nano();
        void setup();
        void loop();
        uint8_t getSensorValue();
    private:
        BLEClient* pClient;
        BLERemoteCharacteristic* pSensorCharacteristic;
        void connectToNano();
        void readDoorSensor();
    };
} 
#endif  