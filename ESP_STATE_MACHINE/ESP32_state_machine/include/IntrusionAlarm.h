#ifndef INTRUSION_ALARM_H
#define INTRUSION_ALARM_H

#include <Arduino.h>

class IntrusionAlarm {
public:
    enum State {
        Disarmed,
        Arming,
        Armed,
        WaitingForAlarmSituation,
        AlarmTriggered,
        AlarmNotified
    };

    IntrusionAlarm() : currentState(Disarmed) {}

    void serverArm() {
        if (currentState == Disarmed) {
            transitionTo(Armed);
            Serial.println("System armed by server command.");
            Serial.print("Current state: ");
            Serial.println(currentState);
        }
    }

    void notifyServer() {
        if (currentState == AlarmTriggered) {
            transitionTo(AlarmNotified);
            Serial.println("Alarm notification sent to server.");
            Serial.print("Current state: ");
            Serial.println(currentState);
        }
    }

    void armSystem() {
        if (currentState == Disarmed) {
            transitionTo(Arming);
            Serial.println("Arming the system...");
            Serial.print("Current state: ");
            Serial.println(currentState);
        }
    }

    void disarmSystem() {
        if (currentState != Disarmed) {
            transitionTo(Disarmed);
            Serial.println("Disarming the system...");
            Serial.print("Current state: ");
            Serial.println(currentState);
        }
    }

    void doorClosed() {
        Serial.println("Door closed");
        if (currentState == AlarmTriggered) {
            transitionTo(Armed);
            Serial.println("Alarm triggered. System armed.");
        } else if (currentState == WaitingForAlarmSituation) {
            transitionTo(Disarmed);
            Serial.println("System disarmed due to door closed.");
        }
        Serial.print("Current state: ");
        Serial.println(currentState);
    }

    void doorOpened() {
        if (currentState == Armed) {
            transitionTo(WaitingForAlarmSituation);
            Serial.println("System waiting for alarm situation due to door opened.");
            Serial.print("Current state: ");
            Serial.println(currentState);
        } else if (currentState == Disarmed) {
            disarmSystem();
        }
    }

    void timeout() {
        if (currentState == WaitingForAlarmSituation) {
            transitionTo(AlarmTriggered);
            Serial.println("Timeout occurred. Alarm triggered.");
            Serial.print("Current state: ");
            Serial.println(currentState);
        }
    }

    State getState() const {
        return currentState;
    }

    void transitionTo(State nextState) {
        currentState = nextState;
    }

private:
    State currentState;




};

#endif
