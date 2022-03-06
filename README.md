# arduino_morse_encoder

See the post [here](https://lbranco93.pythonanywhere.com/post/3) for an explanation of the code.

Each .ino sketch is meant to be uploaded on a different microprocessor:
- Arduino: basic morse code conversion for Arduino without wifi;
- Wifi version: version of the morse code for an ESP32 DEVKIT, uses wifi to fetch the message from a remote server;
- Server code: code to be run on a ESP32 DEVKIT, it stores a message and provide APIs to fetch/upload the message from remote. To be used along with the wifi version.
