#include <door_sensor.h>
#include <Arduino.h>
#include <board.h>

namespace DoorSensor
{
    static uint8_t  f_last_pin_state = LOW;
    static enum State f_state = DoorSensor::DOOR_CLOSED;
    static cb_notify f_cb = NULL;
    static void *f_context = NULL;

    void setup(cb_notify i_cb, void *i_context)
    {
        pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP); // set arduino pin to input pull-up mode
        f_cb = i_cb;
        f_context = i_context;
    }

    void loop(void)
    {
        static uint8_t current_pin_state;
        current_pin_state  = digitalRead(DOOR_SENSOR_PIN); // read new state

        if ((f_last_pin_state == LOW) && (current_pin_state == HIGH))  // state change: LOW -> HIGH
        {
            f_state = DoorSensor::DOOR_OPENED;
            if(f_cb) f_cb(f_context, f_state);
            Serial.println("Door open");
        }
        else if ((f_last_pin_state == HIGH) && (current_pin_state == LOW))  // state change: HIGH -> LOW
        {
            f_state = DoorSensor::DOOR_CLOSED;
            if(f_cb) f_cb(f_context, f_state);
            Serial.println("Door closed");
        }
        else
        {
        }
        f_last_pin_state = current_pin_state;              // save the last state
    }

    DoorSensor::State getStatus(void)
    {
        return f_state;
    }
}