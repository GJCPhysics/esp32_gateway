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
 * @file    main.cpp
 * @author  Surendra Nadkarni, Mayuri Kinare, Shubham Sawant
 * @date    01/03/2024
 * @brief   The ESP32 application to handle event and notify to server
 *          if changed on binary sensor.
 ********************************************************************************
 */
#include <Arduino.h>
#include "NanoCommunication.h"
#include "ServerCommunication.h"
#include "IntrusionAlarm.h"
#include "SmokeSensorAlarm.h"

#define BUZZER_PIN 2

NanoCommunication::Nano nanoCommunication;
ServerCommunication::Server serverCommunication;
IntrusionAlarm intrusionAlarm;
SmokeSensorAlarm smokeSensorAlarm;

bool isBuzzerOn = false;

void activateBuzzer(unsigned long duration) 
{
    isBuzzerOn = true;
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duration);
    digitalWrite(BUZZER_PIN, LOW);
    isBuzzerOn = false;
}

void setup() 
{
    Serial.begin(9600);
    nanoCommunication.setup();
    serverCommunication.initWiFi();
    serverCommunication.setup();
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
}

void loop()
{
    nanoCommunication.loop();
    ServerCommunication::Server::receiveCommands(intrusionAlarm);

    uint8_t DoorSensorValue = nanoCommunication.getDoorSensorValue();
    uint8_t SmokeSensorValue = nanoCommunication.getSmokeSensorValue();

    if (DoorSensorValue == 1 || DoorSensorValue == 3) 
    {
        Serial.println("Door opened");
        if (intrusionAlarm.getState() == IntrusionAlarm::Armed)
        {
            activateBuzzer(30000); 
        }
    } 
    else 
    {
        Serial.println("Door closed");
        if (intrusionAlarm.getState() == IntrusionAlarm::Disarmed)
        {
            isBuzzerOn = false;
            digitalWrite(BUZZER_PIN, LOW); 
        }
    }

    if (SmokeSensorValue == 2 || SmokeSensorValue == 3) 
    {
        Serial.println("Smoke detected");
        activateBuzzer(30000); 
    }
    else 
    {
        smokeSensorAlarm.smokeNotDetected();
    }

    if (smokeSensorAlarm.getState() == SmokeSensorAlarm::AlarmTriggered ||
        intrusionAlarm.getState() == IntrusionAlarm::AlarmTriggered ||
        smokeSensorAlarm.getState() == SmokeSensorAlarm::SmokeDetected) 
    {    
        Serial.println("Sending sensor data to the server...");
        serverCommunication.sendDataToServer(DoorSensorValue, SmokeSensorValue);
        delay(3000);
    }
    switch (intrusionAlarm.getState()) 
    {
        case IntrusionAlarm::AlarmTriggered:
            Serial.println("Intrusion alarm triggered. Notifying server...");
            serverCommunication.notifyIntrusionAlarm();
            break;
        default:
            break;
    }

    switch (smokeSensorAlarm.getState())
    {
        case SmokeSensorAlarm::AlarmTriggered:
            Serial.println("Smoke sensor alarm triggered. Notifying server...");
            serverCommunication.notifySmokeSensorAlarm();
            break;
        default:
            break;
    }
    delay(1000);
}
