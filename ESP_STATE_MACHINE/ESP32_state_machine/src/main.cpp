#include <Arduino.h>
#include "NanoCommunication.h"
#include "ServerCommunication.h"
#include "IntrusionAlarm.h"
#include "SmokeSensorAlarm.h"

NanoCommunication::Nano nanoCommunication;
ServerCommunication::Server serverCommunication;
IntrusionAlarm intrusionAlarm;
SmokeSensorAlarm smokeSensorAlarm;

void setup() {
    Serial.begin(9600);
    nanoCommunication.setup();
    serverCommunication.initWiFi();
}

void loop() {
    nanoCommunication.loop();
    ServerCommunication::Server::receiveCommands(intrusionAlarm);

    
    Serial.println(intrusionAlarm.getState());
    uint8_t DoorSensorValue = nanoCommunication.getDoorSensorValue();
    uint8_t SmokeSensorValue = nanoCommunication.getSmokeSensorValue();

    // Update intrusion alarm state based on sensor value
    if (DoorSensorValue == 1 || DoorSensorValue == 3) {
        
        
        Serial.println("Door opened");
        intrusionAlarm.doorOpened();
        intrusionAlarm.timeout();
        
    } else {
        intrusionAlarm.doorClosed();
    }

    

    // Update smoke sensor alarm state
    if (SmokeSensorValue == 2 || SmokeSensorValue == 3) {
        smokeSensorAlarm.smokeDetected();
    } else {
        smokeSensorAlarm.smokeNotDetected();
    }

    // Check if both smoke is detected and door is opened
    if (intrusionAlarm.getState() == IntrusionAlarm::WaitingForAlarmSituation ||
        smokeSensorAlarm.getState() == SmokeSensorAlarm::AlarmTriggered ||
        intrusionAlarm.getState() == IntrusionAlarm::AlarmTriggered ||
        smokeSensorAlarm.getState() == SmokeSensorAlarm::SmokeDetected) {
        // Send sensor data to the server if either door is opened or smoke is detected
        Serial.println("Sending sensor data to the server...");
        serverCommunication.sendDataToServer(DoorSensorValue, SmokeSensorValue);
        delay(3000);
    }

    // Check intrusion alarm state
    switch (intrusionAlarm.getState()) {
        case IntrusionAlarm::AlarmTriggered:
            Serial.println("Intrusion alarm triggered. Notifying server...");
            serverCommunication.notifyIntrusionAlarm();
            break;
        default:
            break;
    }

    // Check smoke sensor alarm state
    switch (smokeSensorAlarm.getState()) {
        case SmokeSensorAlarm::AlarmTriggered:
            Serial.println("Smoke sensor alarm triggered. Notifying server...");
            serverCommunication.notifySmokeSensorAlarm();
            break;
        default:
            break;
    }
    /*if (serverCommunication.getCommand() == "Arm") {
        intrusionAlarm.armSystem();  // Change the state to "Armed"
        Serial.println("System armed.");
    }*/
    // Receive commands from the server
    //serverCommunication.receiveCommands();

    // Receive data from clients
    //serverCommunication.receiveDataFromClient(); // Add this line to receive data from clients

    delay(1000);
}
