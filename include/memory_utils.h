#ifndef _MEMORY_UTILS_H
#define _MEMORY_UTILS_H

#include <Preferences.h>

Preferences preferences;
const char* prefNamespace = "message";
const char* varName = "messagevar";

String readMessage(const String def){
  preferences.begin(prefNamespace);
  if(preferences.isKey(varName)){
    Serial.println(String(varName)+" key found");
    return preferences.getString(varName);
  }
  else{
    Serial.println(String(varName)+" key not found, returning default value "+def);
    return def;
  }
  preferences.end();
}

bool setMessage(const String newValue){
  preferences.begin(prefNamespace);
  String oldValue = readMessage("");
  bool outcome = false;
  if(newValue != oldValue){
    Serial.println("Updating last read message!");
    preferences.putString(varName, newValue);
    outcome = true;
  }
  else{
    Serial.println("Last read message is the same, not updating it in memory");
  }
  preferences.end();
  return outcome;
}

#endif
