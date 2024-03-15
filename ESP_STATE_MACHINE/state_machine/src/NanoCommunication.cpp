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
 * @file    NanoCommunication.cpp
 * @author  Surendra Nadkarni, Mayuri Kinare, Shubham Sawant
 * @date    01/03/2024
 * @brief   communcation with nano board via BLE GATT
 ********************************************************************************
 */
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
            getDoorSensorValue();
            getSmokeSensorValue();
        } 
        else 
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

    uint8_t Nano::getDoorSensorValue() 
    {
        if (pSensorCharacteristic == nullptr) 
        {
            return 0; 
        }
        return pSensorCharacteristic->readUInt8();
    }

    uint8_t Nano::getSmokeSensorValue() {
        if (pSensorCharacteristic == nullptr) {
            return 0; 
        }
        return pSensorCharacteristic->readUInt8();
    }
}
