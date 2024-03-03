#ifndef SMOKE_SENSOR_ALARM_H
#define SMOKE_SENSOR_ALARM_H

class SmokeSensorAlarm 
{
public:
    enum State 
    {
        Arming,
        Armed,
        Disarmed,
        SmokeDetected,
        AlarmTriggered,
        AlarmNotified
    };

    SmokeSensorAlarm() : currentState(Arming) {}

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
        if (currentState == Armed)
         {
            transitionTo(AlarmTriggered);
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
