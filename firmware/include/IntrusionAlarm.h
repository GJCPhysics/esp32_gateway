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
 * @file    IntrusionAlarm.h
 * @author  Surendra Nadkarni, Mayuri Kinare, Shubham Sawant
 * @date    01/03/2024
 * @brief    The state machine implementation of door sensor
 ********************************************************************************
 */
#ifndef INTRUSION_ALARM_H
#define INTRUSION_ALARM_H

#include <Arduino.h>

class IntrusionAlarm 
{
public:
    enum State 
    {
        Disarmed,
        Arming,
        Armed,
        WaitingForAlarmSituation,
        AlarmTriggered,
        AlarmNotified
    };

    IntrusionAlarm() : currentState(Disarmed) {}

    void serverArm() 
    {
        if (currentState == Disarmed) 
        {
            transitionTo(Armed);
            Serial.println("System armed by server command.");  
        }
    }

    void notifyServer() 
    {
        if (currentState == AlarmTriggered) 
        {
            transitionTo(AlarmNotified);
            Serial.println("Alarm notification sent to server.");
        }
    }

    void armSystem() 
    {
        if (currentState == Disarmed)
        {
            transitionTo(Arming);
            Serial.println("Arming the system...");
        }
    }

    void disarmSystem() 
    {
        if (currentState != Disarmed) 
        {
            transitionTo(Disarmed);
            Serial.println("Disarming the system...");
        }
    }

    void doorClosed() 
    {
        Serial.println("Door closed");
        if (currentState == AlarmTriggered)
        {
            transitionTo(Armed);
            Serial.println("Alarm triggered. System armed.");
        } 
        else if (currentState == WaitingForAlarmSituation) 
        {
            transitionTo(Disarmed);
            Serial.println("System disarmed due to door closed.");
        }
    }

    void doorOpened() 
    {
        if (currentState == Armed) 
        {
            transitionTo(WaitingForAlarmSituation);
            Serial.println("System waiting for alarm situation due to door opened.");
        } 
        else if (currentState == Disarmed) 
        {
            transitionTo(Disarmed);
        }
    }

    void timeout()
     {
        if (currentState == WaitingForAlarmSituation) 
        {
            transitionTo(AlarmTriggered);
            Serial.println("Timeout occurred. Alarm triggered.");
        }
    }

    State getState() const 
    {
        return currentState;
    }

    void transitionTo(State nextState) 
    {
        currentState = nextState;
    }

private:
    State currentState;
};
#endif
