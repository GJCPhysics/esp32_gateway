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
 * @file    SmokeSensorAlarm.h
 * @author  Surendra Nadkarni, Mayuri Kinare, Shubham Sawant
 * @date    01/03/2024
 * @brief   implementation of state machine of smoke sensor
 ********************************************************************************
 */
#ifndef SMOKE_SENSOR_ALARM_H
#define SMOKE_SENSOR_ALARM_H

#include <Arduino.h>

class SmokeSensorAlarm 
{
public:
    enum State 
    {
        Arming,
        Armed,
        Disarmed,
        SmokeDetected,
        SmokeNotDetected,
        AlarmTriggered,
        AlarmNotified
    };

    SmokeSensorAlarm() : currentState(Armed) {}

    void manualArm() 
    {
        transitionTo(Armed);
    }

    void manualDisarm() 
    {
        transitionTo(Disarmed);
    }

    void smokeDetected() 
    {
        Serial.println("Smoke detected");
        if (currentState == Armed) 
        {
            transitionTo(AlarmTriggered);
        }
    }

    void smokeNotDetected() 
    {
        Serial.println("Smoke not detected");
        if (currentState == AlarmTriggered) 
        {
            transitionTo(Armed);
        } 
        else if (currentState == Armed) 
        {
            transitionTo(Armed);
        }
    }

    void resetAlarm() 
    {
        transitionTo(Armed);
    }

    void notifyServer() 
    {
        if (currentState == AlarmTriggered) 
        {
            transitionTo(AlarmNotified);
        }
    }

    State getState() const 
    {
        return currentState;
    }

private:
    State currentState;

    void transitionTo(State nextState)
    {
        currentState = nextState;
    }
};

#endif
