#include <Arduino.h>
#include "NanoCommunication.h"
#include "ServerCommunication.h"

NanoCommunication::Nano nanoCommunication;
ServerCommunication::Server serverCommunication;

const char *ssid = "realme 5i";
const char *password = "s.n.s.03";
unsigned long previousMillis = 0;
const long interval = 30000;

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
    uint8_t sensorValue = nanoCommunication.getSensorValue();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        serverCommunication.sendDataToServer(sensorValue);
    }
}
