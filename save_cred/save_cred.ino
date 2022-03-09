
#include "include/memory_utils.h"

const char* SSID = "YOUR_SSID";
const char* PWD = "YOUR_PWD";

void setup(){
    Serial.begin(115200);
    preferences.begin("credentials");
    preferences.clear();
    preferences.putString("ssid", SSID);
    preferences.putString("pwd", PWD);
    Serial.println(preferences.getString("ssid"));
    Serial.println(preferences.getString("pwd"));
    preferences.end();
}

void loop(){}
