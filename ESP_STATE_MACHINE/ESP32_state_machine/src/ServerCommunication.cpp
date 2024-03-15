#include "ServerCommunication.h"
#include "IntrusionAlarm.h"
#include <HTTPClient.h>
#include<ArduinoJson.h>

namespace ServerCommunication {
    WiFiServer server(5000);
   

    const char* serverAddress = "192.168.43.172"; // Replace with your server IP
    const int serverPort = 5000; // Replace with your server port

     WiFiClient wifiClient;
    Server::Server() {}

    void Server::initWiFi() {
        const char *ssid = "realme 5i";
        const char *password = "s.n.s.03";
        Serial.print("Connecting to WiFi");
        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.print(".");
        }
        Serial.println("\nWiFi connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    }

    void Server::sendDataToServer(uint8_t DoorSensorValue, uint8_t SmokeSensorValue) {
        if (wifiClient.connect(serverAddress, serverPort)) {
            DynamicJsonDocument jsonDoc(256);
            jsonDoc["DoorSensorValue"] = DoorSensorValue;
            jsonDoc["SmokeSensorValue"] = SmokeSensorValue;
            String jsonString;
            serializeJson(jsonDoc, jsonString);
            
            wifiClient.print("POST /update_sensor_status HTTP/1.1\r\n");
            wifiClient.print("Host: ");
            wifiClient.print(serverAddress);
            wifiClient.print("\r\n");
            wifiClient.print("Content-Type: application/json\r\n");
            wifiClient.print("Content-Length: ");
            wifiClient.print(jsonString.length());
            wifiClient.print("\r\n\r\n");
            wifiClient.print(jsonString);
            wifiClient.stop();
        } else {
            Serial.println("Failed to connect to the server");
        }
    }

    void Server::notifyIntrusionAlarm() {
        if (wifiClient.connect(serverAddress, serverPort)) {
            wifiClient.println("POST /notify/intrusion_alarm HTTP/1.1");
            wifiClient.println("Host: " + String(serverAddress));
            wifiClient.println("Connection: close");
            wifiClient.println();
            wifiClient.stop();
        } else {
            Serial.println("Failed to connect to the server");
        }
    }

    void Server::notifySmokeSensorAlarm() {
        if (wifiClient.connect(serverAddress, serverPort)) {
            wifiClient.println("POST /notify/smoke_sensor_alarm HTTP/1.1");
            wifiClient.println("Host: " + String(serverAddress));
            wifiClient.println("Connection: close");
            wifiClient.println();
            wifiClient.stop();
        } else {
            Serial.println("Failed to connect to the server");
        }
    }

/*String ServerCommunication::Server::getCommand() {
    // Implement the logic to get the command from the server
    // For example, you can return a predefined command or fetch it from a server endpoint
    return "arm"; // Example: returning a predefined command "arm"
}*/

   void Server::receiveCommands(IntrusionAlarm& intrusionAlarm) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(wifiClient, "http://" + String(serverAddress) + ":" + String(serverPort) + "/send_commands");
        int httpResponseCode = http.GET();
        
        if (httpResponseCode > 0) {
            if (http.getSize() > 0) {
                DynamicJsonDocument jsonBuffer(http.getSize());
                deserializeJson(jsonBuffer, http.getStream());
                
                JsonObject root = jsonBuffer.as<JsonObject>();
                if (root.containsKey("command")) {
                    String command = root["command"];
                    Serial.print("Received command from server: ");
                    Serial.println(command);
                    delay(3000);
                    if (command == "arm") {
                        // Change the currentState to "Armed" and allow other states to work
                        
                        intrusionAlarm.serverArm();
                        Serial.println(intrusionAlarm.getState());
                    } else if (command == "disarm") {
                        // Change the currentState to "Disarmed" and disable other states
                        intrusionAlarm.disarmSystem();
                    }
                }
            }
        } else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
        }
        http.end();
    } else {
        Serial.println("WiFi not connected");
    }
}


}
