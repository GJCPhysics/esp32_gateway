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
 * @file    NanoCommunication.h
 * @author  Surendra Nadkarni, Mayuri Kinare, Shubham Sawant
 * @date    01/03/2024
 * @brief   defination of communcation with nano board via BLE GATT
 ********************************************************************************
 */
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
    };
}
#endif
