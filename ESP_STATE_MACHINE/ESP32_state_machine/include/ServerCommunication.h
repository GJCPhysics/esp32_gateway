// ServerCommunication.h

#ifndef SERVER_COMMUNICATION_H
#define SERVER_COMMUNICATION_H

#include <Arduino.h>
#include "IntrusionAlarm.h"

namespace ServerCommunication {
    class Server {
    public:
        Server();
           public:
        // Declare receiveCommands() with IntrusionAlarm parameter
        static void receiveCommands(IntrusionAlarm& intrusionAlarm);
        void initWiFi();
        void sendDataToServer(uint8_t DoorSensorValue, uint8_t SmokeSensorValue);
        void notifyIntrusionAlarm();
        void notifySmokeSensorAlarm();
        void receiveCommands();
        void setReceivedCommand(String command);

    private:
        String receivedCommand;
    };
}

#endif
