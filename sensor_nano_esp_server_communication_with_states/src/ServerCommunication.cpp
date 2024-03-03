#include "ServerCommunication.h"

namespace ServerCommunication
{
    Server::Server() {}

    void Server::initWiFi() 
    {
        const char *ssid = "realme 5i";
        const char *password = "s.n.s.03";
        Serial.print("Connecting to WiFi");
        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED) 
        {
            delay(1000);
            Serial.print(".");
        }
        Serial.println("\nWiFi connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    }

    void Server::sendDataToServer(uint8_t sensorValue) 
    {
        const char *serverAddress = "192.168.43.172";
        const int serverPort = 5000;

        if (wifiClient.connect(serverAddress, serverPort)) 
        {
            DynamicJsonDocument jsonDoc(256);

            String doorStatus;
            String smokeStatus;

            if (sensorValue == 0) 
            {
                doorStatus = "closed";
                smokeStatus = "not detected";
            } else if (sensorValue == 1)
            {
                doorStatus = "opened";
                smokeStatus = "not detected";
            } else if (sensorValue == 2)
            {
                doorStatus = "closed";
                smokeStatus = "detected";
            } else
            {
                doorStatus = "opened";
                smokeStatus = "detected";
            }

            jsonDoc["door_status"] = doorStatus;
            jsonDoc["smoke_status"] = smokeStatus;

            String jsonString;
            serializeJson(jsonDoc, jsonString);

            wifiClient.print("POST /update_sensor_status HTTP/1.1\r\n");
            wifiClient.print("Host: ");
            wifiClient.print(serverAddress);
            wifiClient.print("\r\n");
            wifiClient.print("Content-Type: application/json\r\n");
            wifiClient.print("Content-Length: ");
            wifiClient.print(jsonString.length());
            wifiClient.print("\r\n\r\n");
            wifiClient.print(jsonString);
            wifiClient.stop();
        } else 
        {
            Serial.println("Failed to connect to the server");
        }
    }

    void Server::notifyIntrusionAlarm() 
    {
        Serial.println("Notifying server about intrusion alarm...");
    }

    void Server::notifySmokeSensorAlarm() 
    {
        Serial.println("Notifying server about smoke sensor alarm...");
    }
}
