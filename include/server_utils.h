#ifndef _WIFI_SERVER_UTILS_H
#define _WIFI_SERVER_UTILS_H

#include "memory_utils.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Preferences.h>

const char* SSID = "YOUR_SSID";
const char* PWD = "YOUR_PASSWORD";
const String serverHostname = "esp32";
const int serverPort = 80;
// Set your Static IP address
const IPAddress serverIP(192, 168, 1, 184);
// Set your Gateway IP address
const IPAddress serverGateway(192, 168, 1, 1);
const IPAddress serverSubnet(255, 255, 0, 0);
const IPAddress serverPrimaryDNS(8, 8, 8, 8);   // optional
const IPAddress serverSecondaryDNS(8, 8, 4, 4); // optional
WebServer server(serverPort);
StaticJsonDocument<250> responseJson;
char buffer[250];

void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(SSID);
  
  WiFi.begin(SSID, PWD);
  
  while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
      // we can even make the ESP32 to sleep
  }
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
}

void endWifi(){
  WiFi.disconnect();
}

void createJson(String message) {
  responseJson.clear();
  responseJson["message"] = message;
  serializeJson(responseJson, buffer);
}

void getMessage(){
  Serial.println("Getting message");
  String message = readMessage("");
  createJson(message);
  server.send(200, "application/json", buffer);
}

void updateMessage(){
  Serial.println("Setting message");
  if (server.hasArg("plain") == false) {
    Serial.println("Wrong json body!");
    server.send(400, "application/text", "The request isn't as expected, please send a JSON formatted request with just a message field.");
  }
  String body = server.arg("plain");
  deserializeJson(responseJson, body);
  if(setMessage(responseJson["message"])){
    server.send(200, "application/json", "{}");
  }
  else{
    Serial.println("Last read message is the same, not updating it in memory");
    server.send(200, "application/json", "Last read message is the same, not updating it in memory");
  }
}

void setupRouting() {
  //Sets a server routing so that each endpoint is assigned to an handler
  // Message
  server.on("/message", getMessage);
  server.on("/message/set", HTTP_POST, updateMessage);
  // start server
  server.begin();
}

#endif
