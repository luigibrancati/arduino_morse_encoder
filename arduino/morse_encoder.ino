#include "include/morse_utils.h"

const short OUT_PIN = 8;
const short START_END_PIN = 7;

void setup() {
  Serial.begin(9600);
  pinMode(OUT_PIN, OUTPUT);
  pinMode(START_END_PIN, OUTPUT);
  digitalWrite(OUT_PIN, LOW);
  digitalWrite(START_END_PIN, LOW);
}

void loop ()
{
  //Start
  start_comm(START_END_PIN);
  delay(time_delay);
  //Convert
  String text = "test";
  String morse_text = text_to_morse(text);
  morse_to_led(morse_text, OUT_PIN);
  delay(time_delay);
  // End communications
  end_comm(START_END_PIN);
  Serial.println("Waiting "+String(time_between_broadcasts/1000)+" seconds before broadcasting again");
  delay(time_between_broadcasts);
}
