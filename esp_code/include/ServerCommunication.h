#ifndef SERVER_COMMUNICATION_H
#define SERVER_COMMUNICATION_H
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>

namespace ServerCommunication 
{
    class Server 
    {
    public:
        Server();
        void initWiFi();
        void sendDataToServer(uint8_t sensorValue);
    private:
        WiFiClient wifiClient;
    };
} 
#endif  