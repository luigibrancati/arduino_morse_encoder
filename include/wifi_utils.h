#ifndef _WIFI_UTILS_H
#define _WIFI_UTILS_H

#include "memory_utils.h"
#include "server_utils.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

StaticJsonDocument<250> httpResponseJson;

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
      Serial.println("HTTP Response code: "+String(httpResponseCode));
      String payload = http.getString();
      httpResponseJson.clear();
      deserializeJson(httpResponseJson, payload);
      String message = httpResponseJson["message"];
      Serial.println(message);
      returnString = message;
    }
    else {
      Serial.println("Error code: "+String(httpResponseCode));
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