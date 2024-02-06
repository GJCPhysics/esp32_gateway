#pragma once
#include <cstdint>

namespace DoorSensor
{
    enum State : uint8_t
    {
        DOOR_OPENED,
        DOOR_CLOSED
    };

    typedef void (*cb_notify)(void *, State);
    void setup(cb_notify, void *);
    void loop(void);
    DoorSensor::State getStatus(void);

    uint8_t getSensorValue(void);
}
