
#include "include/memory_utils.h"

const char* SSID = "FASTWEB-M4TDm4-2.4GHZ";
const char* PWD = "6j6dP38v22";

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