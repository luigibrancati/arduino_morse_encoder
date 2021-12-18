#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OUT_PIN 8
#define END_PIN 7

const short dot_time = 300; // Time LED is ON for dot
const short dash_time = dot_time * 3; // Time LED is ON for dash
const short same_letter_space = dot_time; // Time LED is OFF between dots/dashes of the same letter
const short letter_space = dot_time * 3; // Time LED is OFF between letters
const short word_space = dot_time * 5; // Time LED is OFF between words
const char letter_separator[] = "_";
const char word_separator[] = "/";
const char unkown_letter[] = "?";

const char* morse_table_letters[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-",
  ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};
const char* morse_table_digits[] = {"-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."};

const char* letter_encoder(char letter) {
  int letter_value = (int) letter;
  if (letter_value >= 97 && letter_value <= 122) {
    //lowercase
    return morse_table_letters[letter_value - 97];
  }
  else if (letter_value >= 65 && letter_value <= 90) {
    //uppercase
    return morse_table_letters[letter_value - 65];
  }
  else if (letter_value>=48 && letter_value<=57) {
    //digits
    return morse_table_digits[letter_value];
  }
  else {
    //Boh
    return unkown_letter;
  }
}

void word_to_morse(char* morse_word, char* word, int len) {
  Serial.print("Encoding word: ");
  Serial.println(word);
  for (int l = 0; l < len; l++) {
    strcat(morse_word, letter_encoder(word[l]));
    if (l < len - 1) {
      strcat(morse_word, letter_separator);
    }
  }
  strcat(morse_word, word_separator);
  Serial.print("Word result: ");
  Serial.println(morse_word);
}

void text_to_morse(char* morse_text, char* text, int len) {
  Serial.print("Encoding text: ");
  Serial.println(text);
  char* swp = text;
  int morse_len = 0;
  for (char* ewp = text; ewp != (text + len); ewp++) {
    if (*(ewp + 1) == ' ' || *(ewp + 1) == '\0') {
      int w_len = ewp - swp + 1;
      char * morse_word = (char*) malloc((5*w_len+w_len)*sizeof(char));
      strcpy(morse_word, "");
      word_to_morse(morse_word, swp, w_len);
      morse_len += strlen(morse_word);
      Serial.print("Word result: ");
      Serial.println(morse_word);
      Serial.print("Word len: ");
      Serial.println(morse_len);

      strcat(morse_text, morse_word);
      swp = ewp + 2;
      free(morse_word);
    }
  }
  morse_text = (char*) realloc(morse_text, ++morse_len*sizeof(char));
  Serial.print("Text result: ");
  Serial.println(morse_text);
}

void morse_to_led(const char* text_morse, int len) {
  Serial.println("Flashing morse with pins");
  for (int l = 0; l < len; l++) {
    char letter = text_morse[l];
    switch(letter){
      case '.':
        Serial.println('.');
        digitalWrite(OUT_PIN, HIGH);
        delay(dot_time);
        break;
      case '-':
        Serial.println('-');
        digitalWrite(OUT_PIN, HIGH);
        delay(dash_time);
        break;
      case '_':
        Serial.println('_');
        digitalWrite(OUT_PIN, LOW);
        delay(letter_space - same_letter_space);
        break;
      case '/':
        Serial.println('/');
        digitalWrite(OUT_PIN, LOW);
        delay(word_space - same_letter_space);
        break;
    }
    digitalWrite(OUT_PIN, LOW);
    delay(same_letter_space);
  }
}

void start_comm() {
  Serial.println("Start comm.");
  digitalWrite(END_PIN, HIGH);
  delay(dash_time);
  digitalWrite(END_PIN, LOW);
}

void end_comm() {
  Serial.println("End comm.");
  digitalWrite(END_PIN, HIGH);
  delay(dot_time);
  digitalWrite(END_PIN, LOW);
  delay(dot_time);
  digitalWrite(END_PIN, HIGH);
  delay(dot_time);
  digitalWrite(END_PIN, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(OUT_PIN, OUTPUT);
  pinMode(END_PIN, OUTPUT);
  digitalWrite(OUT_PIN, LOW);
  digitalWrite(END_PIN, LOW);
}

void loop ()
{
  //Start
  start_comm();
  delay(2000);

  //Convert
  char test_text[] = "aaa";
  int len = strlen(test_text);
  char * morse_text = (char*) malloc((5*len+len)*sizeof(char));
  strcpy(morse_text, "");
  text_to_morse(morse_text, test_text, len);
  morse_to_led(morse_text, strlen(morse_text));
  free(morse_text);
  delay(2000);
  
  //End
  end_comm();
  delay(2000);
}
