#include <Arduino.h>
#include "NanoCommunication.h"
#include "ServerCommunication.h"
#include "IntrusionAlarm.h"
#include "SmokeSensorAlarm.h"

NanoCommunication::Nano nanoCommunication;
ServerCommunication::Server serverCommunication;
IntrusionAlarm intrusionAlarm;
SmokeSensorAlarm smokeSensorAlarm;

const char* ssid = "realme 5i"; 
const char* password = "s.n.s.03"; 

void setup() 
{
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(1000);
    }
    
    nanoCommunication.setup();
    delay(1000);
    serverCommunication.initWiFi();
}

void loop() 
{
    nanoCommunication.loop();
    
    unsigned long currentMillis = millis();
    uint8_t doorSensorValue = nanoCommunication.getDoorSensorValue();
    uint8_t smokeSensorValue = nanoCommunication.getSmokeSensorValue();

    if (doorSensorValue == 1) 
    {
        intrusionAlarm.doorOpened();
    } else
    {
        intrusionAlarm.doorClosed();
    }

    if (smokeSensorValue == 1) 
    {
        smokeSensorAlarm.smokeDetected();
    }

    switch (intrusionAlarm.getState()) 
    {
        case IntrusionAlarm::WaitingForAlarmSituation:
            break;
        case IntrusionAlarm::AlarmTriggered:
            serverCommunication.notifyIntrusionAlarm();
            break;
    }

    switch (smokeSensorAlarm.getState()) {
        case SmokeSensorAlarm::AlarmTriggered:
            serverCommunication.notifySmokeSensorAlarm();
            break;
        case SmokeSensorAlarm::AlarmNotified:
            break;
    }

    delay(1000);  
}
