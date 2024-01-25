#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>

const char* ssid = "OPPO A3s"; // my mobile hotspot's SSID
const char* password = "12345678"; // hotspot's password

WiFiServer server(80);

void setup() 
{
 Serial.begin(9600);
 delay(1000);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) 
 {
   delay(1000);
   Serial.println("Connecting to WiFi…………..Wait!!!");
 }

 Serial.println("Connected to WiFi");
 Serial.println("IP Address: " + WiFi.localIP().toString());

 server.begin();
}
void loop() 
{
 WiFiClient client = server.available();

 if (client) 
 {
   Serial.println("Client connected");
   while (client.connected()) 
   {
     if (client.available()) 
     {
       String data = client.readStringUntil('\r');
       Serial.println(data);
       DynamicJsonDocument document(1024);
       deserializeJson(document, data);
       const char* message = document["message"];
       Serial.println("Received Message: " + String(message));
       JsonObject response = document.to<JsonObject>();
       response["response"] = "Suceessfully_Acknowledged";
       String jsonResponse;
       serializeJson(response, jsonResponse);
       client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n");
       client.print(jsonResponse);
       delay(100);
       client.stop();
       Serial.println("Client disconnected");
     }
   }
 }
}




