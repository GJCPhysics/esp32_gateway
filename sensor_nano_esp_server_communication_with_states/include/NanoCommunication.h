#ifndef NANO_COMMUNICATION_H
#define NANO_COMMUNICATION_H
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

namespace NanoCommunication 
{
    class BLE_GATT_ClientCallback : public BLEClientCallbacks 
    {
    public:
        BLE_GATT_ClientCallback();  
        void onConnect(BLEClient* pClient);
        void onDisconnect(BLEClient* pClient);
    };

    class Nano 
    {
    public:
        Nano();
        void setup();
        void loop();
        uint8_t getDoorSensorValue();
        uint8_t getSmokeSensorValue();
    private:
        BLEClient* pClient;
        BLERemoteCharacteristic* pSensorCharacteristic;
        void connectToNano();
        void readSensorStatus();
     
        void sendSensorDataToServer(uint8_t doorSensorValue, uint8_t smokeSensorValue);
    };
} 
#endif
