#ifndef INTRUSION_ALARM_H
#define INTRUSION_ALARM_H

class IntrusionAlarm 
{
public:
    enum State
    {
        Disarmed,
        Arming,
        Armed,
        WaitingForAlarmSituation,
        AlarmTriggered
    };

    IntrusionAlarm() : currentState(Disarmed) {}

    void armSystem() 
    {
        transitionTo(Arming);
    }

    void disarmSystem() 
    {
        transitionTo(Disarmed);
    }

    void doorClosed() 
    {
        if (currentState == Arming) 
        {
            transitionTo(Armed);
        } 
        else if (currentState == WaitingForAlarmSituation) 
        {
            transitionTo(Disarmed);
        }
    }

    void doorOpened()
     {
        if (currentState == Armed) 
        {
            transitionTo(WaitingForAlarmSituation);
        }
    }

    void timeout()
     {
        if (currentState == WaitingForAlarmSituation) 
        {
            transitionTo(AlarmTriggered);
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
