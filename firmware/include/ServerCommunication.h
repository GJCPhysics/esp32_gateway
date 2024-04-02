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
 * @file    ServerCommunication.h
 * @author  Surendra Nadkarni, Mayuri Kinare, Shubham Sawant
 * @date    01/03/2024
 * @brief   defination of server communication
 ********************************************************************************
 */
#ifndef SERVER_COMMUNICATION_H
#define SERVER_COMMUNICATION_H

#include <Arduino.h>
#include "IntrusionAlarm.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

namespace ServerCommunication
 {
    class Server
    {
    public:
        Server();
        static void receiveCommands(IntrusionAlarm& intrusionAlarm);
        void initWiFi();
        void sendDataToServer(uint8_t DoorSensorValue, uint8_t SmokeSensorValue);
        void notifyIntrusionAlarm();
        void notifySmokeSensorAlarm();
        void receiveCommands();
        void setup();
        void setupWiFiAP();
        void saveWiFiCredentials(String ssid, String password);
        void connectToWiFi(String ssid, String password);
        void setupRoutes();
        void readWiFiCredentials(String &ssid, String &password);
        void connectToSavedWiFi();
    private:
        String receivedCommand;
    };
}
#endif
