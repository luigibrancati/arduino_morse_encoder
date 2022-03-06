#ifndef _WIFI_SERVER_UTILS_H
#define _WIFI_SERVER_UTILS_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* SSID = "FASTWEB-M4TDm4-2.4GHZ";
const char* PWD = "6j6dP38v22";
const int serverPort = 80;
// Set your Static IP address
const IPAddress serverIP(192, 168, 1, 184);
StaticJsonDocument<250> jsonDocument;

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

String fetchMessageFromServer(){
  connectToWiFi();
  String returnString = "";
  if(WiFi.status()==WL_CONNECTED){
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    String serverPath = "http://"+String(serverIP.toString())+":"+serverPort+"/message";
    http.begin(serverPath);
    
    // Send HTTP GET request
    Serial.println("Fetching message from server "+serverPath);
    int httpResponseCode = http.GET();
    
    if (httpResponseCode==200) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      deserializeJson(jsonDocument, payload);
      String message = jsonDocument["message"];
      Serial.println(message);
      returnString = message;
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  endWifi();
  return returnString;
}

#endif