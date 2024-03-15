#ifndef SMOKE_SENSOR_ALARM_H
#define SMOKE_SENSOR_ALARM_H

#include <Arduino.h>

class SmokeSensorAlarm {
public:
    enum State {
        Arming,
        Armed,
        Disarmed,
        SmokeDetected,
        SmokeNotDetected,
        AlarmTriggered,
        AlarmNotified
    };

    SmokeSensorAlarm() : currentState(Armed) {}

    void manualArm() {
        transitionTo(Armed);
    }

    void manualDisarm() {
        transitionTo(Disarmed);
    }

    void smokeDetected() {
        Serial.println("Smoke detected");
        if (currentState == Armed) {
            transitionTo(AlarmTriggered);
        }
    }

    void smokeNotDetected() {
        Serial.println("Smoke not detected");
        if (currentState == AlarmTriggered) {
            transitionTo(Armed);
        } else if (currentState == Armed) {
            transitionTo(Armed);
        }
    }

    void resetAlarm() {
        transitionTo(Armed);
    }

    void notifyServer() {
        if (currentState == AlarmTriggered) {
            transitionTo(AlarmNotified);
        }
    }

    State getState() const {
        return currentState;
    }

private:
    State currentState;

    void transitionTo(State nextState) {
        currentState = nextState;
    }
};

#endif
