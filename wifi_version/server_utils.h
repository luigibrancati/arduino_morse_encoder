#ifndef _WIFI_SERVER_UTILS_H
#define _WIFI_SERVER_UTILS_H

#include <WiFi.h>

const char* SSID = "FASTWEB-M4TDm4-2.4GHZ";
const char* PWD = "6j6dP38v22";
const int serverPort = 80;
// Set your Static IP address
const IPAddress serverIP(192, 168, 1, 184);

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

#endif