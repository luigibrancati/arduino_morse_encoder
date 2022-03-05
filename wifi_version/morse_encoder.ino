#include "morse_utils.h"
#include "server_utils.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define OUT_PIN 23
#define START_END_PIN 22

StaticJsonDocument<250> jsonDocument;

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

void setup() {
  Serial.begin(115200);
  pinMode(OUT_PIN, OUTPUT);
  pinMode(START_END_PIN, OUTPUT);
  digitalWrite(OUT_PIN, LOW);
  digitalWrite(START_END_PIN, LOW);
}

void loop ()
{
  // Fetch message
  String text = fetchMessageFromServer();
  if(text.length()==0){
    // If the server didn't return a message, use the last read message in memory
    Serial.println("No message from server, fetching last read message from memory");
    text = readMessage("aaa");
  }
  else{
    // If the server returned a message, upload the last read message in memory
    Serial.println("Message from server not empty, updating last read message in memory");
    setMessage(text);
  }
  // Start communications
  start_comm(START_END_PIN);
  delay(2000);
  Serial.println("Sending message: "+text);
  String morse_text = text_to_morse(text);
  morse_to_led(morse_text, OUT_PIN);
  morse_text.clear();
  delay(2000);
  
  // End communications
  end_comm(START_END_PIN);
  delay(2000);
}
