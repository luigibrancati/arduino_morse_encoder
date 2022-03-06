#ifndef _MORSE_UTILS_H
#define _MORSE_UTILS_H

const short dot_time = 300; // Time LED is ON for dot
const short dash_time = dot_time * 3; // Time LED is ON for dash
const short same_letter_space = dot_time; // Time LED is OFF between dots/dashes of the same letter
const short letter_space = dot_time * 3; // Time LED is OFF between letters
const short word_space = dot_time * 5; // Time LED is OFF between words
const char letter_separator = '_';
const char word_separator = '/';
const char unkown_letter = '?';

const char* morse_table_letters[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-",
  ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};
const char* morse_table_digits[] = {"-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."};

const char* letter_encoder(const char letter) {
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
    return &unkown_letter;
  }
}

String word_to_morse(const String word) {
  Serial.println("Encoding word: "+word);
  String morse_word = "";
  for (int l = 0; l < word.length(); l++) {
    morse_word.concat(letter_encoder(word.charAt(l)));
    if (l < word.length() - 1) {
      morse_word.concat(letter_separator);
    }
  }
  morse_word.concat(word_separator);
  Serial.println("Word result: "+morse_word);
  return morse_word;
}

String text_to_morse(const String text) {
  Serial.print("Encoding text: ");
  Serial.println(text);
  int swi = 0;
  String morse = "";
  for (int ewi = 0; ewi < text.length(); ewi++) {
    if (text.charAt(ewi + 1) == ' ' || text.charAt(ewi + 1) == '\0') {
      String word = text.substring(swi, ewi + 1);
      String morse_word = word_to_morse(word);
      morse.concat(morse_word);
      swi = ewi + 2;
    }
  }
  Serial.println("Text result: "+morse);
  return morse;
}

void morse_to_led(const String text_morse, const short out_pin) {
  Serial.println("Flashing morse with pins");
  for (int l = 0; l < text_morse.length(); l++) {
    char letter = text_morse.charAt(l);
    switch(letter){
      case '.':
        Serial.println('.');
        digitalWrite(out_pin, HIGH);
        delay(dot_time);
        break;
      case '-':
        Serial.println('-');
        digitalWrite(out_pin, HIGH);
        delay(dash_time);
        break;
      case letter_separator:
        Serial.println('_');
        digitalWrite(out_pin, LOW);
        delay(letter_space - same_letter_space);
        break;
      case word_separator:
        Serial.println('/');
        digitalWrite(out_pin, LOW);
        delay(word_space - same_letter_space);
        break;
    }
    digitalWrite(out_pin, LOW);
    delay(same_letter_space);
  }
}

void start_comm(const short end_pin) {
  Serial.println("Start comm.");
  digitalWrite(end_pin, HIGH);
  delay(dash_time);
  digitalWrite(end_pin, LOW);
}

void end_comm(const short end_pin) {
  Serial.println("End comm.");
  digitalWrite(end_pin, HIGH);
  delay(dot_time);
  digitalWrite(end_pin, LOW);
  delay(dot_time);
  digitalWrite(end_pin, HIGH);
  delay(dot_time);
  digitalWrite(end_pin, LOW);
}

#endif
