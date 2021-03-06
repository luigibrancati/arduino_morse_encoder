#include "include/memory_utils.h"
#include "include/morse_utils.h"
#include "include/wifi_utils.h"


const short OUT_PIN = 23;
const short START_END_PIN = 22;
const int uS_TO_mS_FACTOR = 1000;  // Conversion factor for micro seconds to milli seconds
RTC_DATA_ATTR int bootCount = 0; // Boot count

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(OUT_PIN, OUTPUT);
  pinMode(START_END_PIN, OUTPUT);
  digitalWrite(OUT_PIN, LOW);
  digitalWrite(START_END_PIN, LOW);
  print_wakeup_reason();
  // Increase boot count
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  // Setup wakeup reason
  esp_sleep_enable_timer_wakeup(time_between_broadcasts * uS_TO_mS_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(time_between_broadcasts/1000) + " Seconds");
  // Fetch message
  String text = fetchMessageFromServer();
  if(text.length()==0){
    // If the server didn't return a message, use the last read message in memory
    Serial.println("No message from server, fetching last read message from memory");
    text = readMessage("");
  }
  else{
    // If the server returned a message, upload the last read message in memory
    Serial.println("Message from server not empty, updating last read message in memory");
    updateMessage(text);
  }
  // Start communications
  start_comm(START_END_PIN);
  delay(time_delay);
  Serial.println("Sending message: "+text);
  String morse_text = text_to_morse(text);
  morse_to_led(morse_text, OUT_PIN);
  morse_text.clear();
  delay(time_delay);
  // End communications
  end_comm(START_END_PIN);
  Serial.println("Going into deep sleep for "+String(time_between_broadcasts/1000)+" seconds");
  esp_deep_sleep_start();
}

void loop ()
{
  // Not going to be called
}
