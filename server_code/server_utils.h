#ifndef _WIFI_SERVER_UTILS_H
#define _WIFI_SERVER_UTILS_H

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Preferences.h>

const char* SSID = "YOUR_SSID";
const char* PWD = "YOUR_PASSWORD";
const String serverHostname = "esp32";
const int serverPort = 80;
const char* prefNamespace = "message";
const char* varName = "messagevar";
// Set your Static IP address
const IPAddress serverIP(192, 168, 1, 184);
// Set your Gateway IP address
const IPAddress serverGateway(192, 168, 1, 1);
const IPAddress serverSubnet(255, 255, 0, 0);
const IPAddress serverPrimaryDNS(8, 8, 8, 8);   // optional
const IPAddress serverSecondaryDNS(8, 8, 4, 4); // optional
WebServer server(serverPort);
StaticJsonDocument<250> jsonDocument;
char buffer[250];
Preferences preferences;

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
  jsonDocument.clear();
  jsonDocument["message"] = message;
  serializeJson(jsonDocument, buffer);
}

String readMessage(){
  preferences.begin(prefNamespace);
  if(preferences.isKey(varName)){
    Serial.println(String(varName)+" key found");
    return preferences.getString(varName);
  }
  else{
    Serial.println(String(varName)+" key not found, returning empty string");
    return "";
  }
  preferences.end();
}

void getMessage(){
  Serial.println("Getting message");
  String message = readMessage();
  createJson(message);
  server.send(200, "application/json", buffer);
}

void setMessage(){
  Serial.println("Setting message");
  if (server.hasArg("plain") == false) {
    Serial.println("Wrong json body!");
    return;
  }
  preferences.begin(prefNamespace);
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  String newValue = jsonDocument["message"];
  String oldValue = readMessage();
  if(newValue != oldValue){
    Serial.println("Updating last read message!");
    preferences.putString(varName, newValue);
    // Respond to the client
    server.send(200, "application/json", "{}");
  }
  else{
    Serial.println("Last read message is the same, not updating it in memory");
    // Respond to the client
    server.send(200, "application/json", "Last read message is the same, not updating it in memory");
  }
  preferences.end();
}

void setupRouting() {
  //Sets a server routing so that each endpoint is assigned to an handler
  // Message
  server.on("/message", getMessage);
  server.on("/message/set", HTTP_POST, setMessage);
  // start server
  server.begin();
}

#endif