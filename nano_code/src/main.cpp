#include <Arduino.h>
#include <ArduinoBLE.h>
#include "door_sensor.h"

#define RESET_DOOR_SENSOR(val) (val & (~BIT_0))
#define SET_DOOR_SENSOR(val) (val | BIT_0)

static BLEService f_device_service("180A");
static BLEByteCharacteristic f_digital_output_service("2A57", BLERead | BLEWrite);
static BLEByteCharacteristic f_sensor_characteristics("2A56", BLERead | BLENotify);
static uint8_t sensor_value = 0;

void door_sensor_callback(void *i_context, DoorSensor::State i_state)
{
    if (DoorSensor::State::DOOR_CLOSED == i_state)
    {
        sensor_value = RESET_DOOR_SENSOR(sensor_value);
    }
    else
    {
        sensor_value = SET_DOOR_SENSOR(sensor_value);
    }
    f_sensor_characteristics.writeValue(sensor_value);
}

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;

    Serial.println("Initializing...");

    DoorSensor::setup(door_sensor_callback, NULL);

    if (!BLE.begin())
    {
        Serial.println("Starting Bluetooth® Low Energy failed!");
        while (1)
            ;
    }

    BLE.setLocalName("Physics BLE");
    BLE.setAdvertisedService(f_device_service);

    f_device_service.addCharacteristic(f_digital_output_service);
    f_device_service.addCharacteristic(f_sensor_characteristics);

    BLE.addService(f_device_service);

    f_digital_output_service.writeValue(0);
    f_sensor_characteristics.writeValue(sensor_value);

    BLE.advertise();

    Serial.println("Initialization complete. Waiting for a connection...");
}

void loop()
{
    BLEDevice central = BLE.central();

    if (central)
    {
        Serial.print("Connected to central: ");
        Serial.println(central.address());

        while (central.connected())
        {
            DoorSensor::loop();

            if (f_digital_output_service.written())
            {
                Serial.print("Digital output service value written: ");
                Serial.println(f_digital_output_service.value());
            }
        }

        Serial.print(F("Disconnected from central: "));
        Serial.println(central.address());

        Serial.println("Waiting for a new connection...");
    }
}
