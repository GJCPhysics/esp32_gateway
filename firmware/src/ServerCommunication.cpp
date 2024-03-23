/*
 * Copyright © 2023 Samarth Holdings AS
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the “Software”), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
 * OR OTHER DEALINGS IN THE SOFTWARE.
*/

/**
 ********************************************************************************
 * @file    ServerCommunication.cpp
 * @author  Surendra Nadkarni, Mayuri Kinare, Shubham Sawant
 * @date    01/03/2024
 * @brief   server communication over wifi
 ********************************************************************************
 */
#include "ServerCommunication.h"
#include "IntrusionAlarm.h"
#include <HTTPClient.h>
#include<ArduinoJson.h>

namespace ServerCommunication
{
    const char* serverAddress = "192.168.43.172"; 
    const int serverPort = 5000; 

    WiFiClient wifiClient;
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
    void Server::sendDataToServer(uint8_t DoorSensorValue, uint8_t SmokeSensorValue) 
    {
        if (wifiClient.connect(serverAddress, serverPort)) 
        {
            DynamicJsonDocument jsonDoc(256);
            jsonDoc["DoorSensorValue"] = DoorSensorValue;
            jsonDoc["SmokeSensorValue"] = SmokeSensorValue;
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
        } 
        else 
        {
            Serial.println("Failed to connect to the server");
        }
    }

    void Server::notifyIntrusionAlarm() 
    {
        if (wifiClient.connect(serverAddress, serverPort)) 
        {
            wifiClient.println("POST /notify/intrusion_alarm HTTP/1.1");
            wifiClient.println("Host: " + String(serverAddress));
            wifiClient.println("Connection: close");
            wifiClient.println();
            wifiClient.stop();
        } 
        else 
        {
            Serial.println("Failed to connect to the server");
        }
    }

    void Server::notifySmokeSensorAlarm() 
    {
        if (wifiClient.connect(serverAddress, serverPort)) 
        {
            wifiClient.println("POST /notify/smoke_sensor_alarm HTTP/1.1");
            wifiClient.println("Host: " + String(serverAddress));
            wifiClient.println("Connection: close");
            wifiClient.println();
            wifiClient.stop();
        } 
        else 
        {
            Serial.println("Failed to connect to the server");
        }
    }

   void Server::receiveCommands(IntrusionAlarm& intrusionAlarm) 
   {
        if (WiFi.status() == WL_CONNECTED) 
        {
            HTTPClient http;
            http.begin(wifiClient, "http://" + String(serverAddress) + ":" + String(serverPort) + "/send_commands");
            int httpResponseCode = http.GET();
            
            if (httpResponseCode > 0) 
            {
                if (http.getSize() > 0) 
                {
                    DynamicJsonDocument jsonBuffer(http.getSize());
                    deserializeJson(jsonBuffer, http.getStream());
                    
                    JsonObject root = jsonBuffer.as<JsonObject>();
                    if (root.containsKey("command")) 
                    {
                        String command = root["command"];
                        Serial.print("Received command from server: ");
                        Serial.println(command);
                        delay(3000);
                        if (command == "arm") 
                        {                   
                            intrusionAlarm.serverArm();
                            Serial.println(intrusionAlarm.getState());
                        } else if (command == "disarm") 
                        {                
                            intrusionAlarm.disarmSystem();
                        }
                    }
                }
            } 
            else 
            {
                Serial.print("Error code: ");
                Serial.println(httpResponseCode);
            }
            http.end();
        } 
        else 
        {
            Serial.println("WiFi not connected");
        }
    }
}



