// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>

using namespace ArduinoJson::Internals;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    // wait serial port initialization
  }

  IndentedPrint serial(Serial);
  serial.setTabSize(4);

  serial.println("This is at indentation 0");
  serial.indent();
  serial.println("This is at indentation 1");
  serial.println("This is also at indentation 1");
  serial.indent();
  serial.println("This is at indentation 2");

  serial.unindent();
  serial.unindent();
  serial.println("This is back at indentation 0");
}

void loop() {
  // not used in this example
}
