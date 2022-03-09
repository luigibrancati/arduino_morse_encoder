#ifndef _WIFI_SERVER_UTILS_H
#define _WIFI_SERVER_UTILS_H

#include "memory_utils.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Preferences.h>

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
  preferences.begin(credentialsNamespace);
  Serial.print("Connecting to ");
  Serial.println(preferences.getString("ssid"));

  WiFi.begin(preferences.getString("ssid").c_str(), preferences.getString("pwd").c_str());
  preferences.end();
  
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
  Serial.println("Updating message");
  if (server.hasArg("plain") == false) {
    Serial.println("Wrong json body!");
    server.send(400, "application/json", "{}");
  }
  String body = server.arg("plain");
  deserializeJson(responseJson, body);
  if(!responseJson.containsKey("message")){
    Serial.println("Wrong json body! Missing message field.");
    server.send(400, "application/json", "Request missing message key.");
    return;
  }
  else{
    if(setMessage(responseJson["message"])){
      Serial.println("Setting message");
      server.send(200, "application/json", "{}");
    }
    else{
      Serial.println("Last read message is the same, not updating it in memory");
      server.send(200, "application/json", "Last read message is the same, not updating it in memory.");
    }
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
