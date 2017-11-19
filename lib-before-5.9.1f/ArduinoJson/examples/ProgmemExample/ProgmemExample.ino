// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>

// About
// -----
// This example shows the different ways you can use PROGMEM with ArduinoJson.
// Please don't see this as an invitation to use PROGMEM.
// On the contrary, you should always use char[] when possible, it's much more
// efficient in term of code size, speed and memory usage.

void setup() {
#ifdef PROGMEM
  DynamicJsonBuffer jsonBuffer;

  // You can use a Flash String as your JSON input.
  // WARNING: the content of the Flash String will be duplicated in the
  // JsonBuffer.
  JsonObject& root =
      jsonBuffer.parseObject(F("{\"sensor\":\"gps\",\"time\":1351824120,"
                               "\"data\":[48.756080,2.302038]}"));

  // You can use a Flash String to get an element of a JsonObject
  // No duplication is done.
  long time = root[F("time")];

  // You can use a Flash String to set an element of a JsonObject
  // WARNING: the content of the Flash String will be duplicated in the
  // JsonBuffer.
  root[F("time")] = time;

  // You can set a Flash String to a JsonObject or JsonArray:
  // WARNING: the content of the Flash String will be duplicated in the
  // JsonBuffer.
  root["sensor"] = F("gps");

  // You can compare the content of a JsonVariant to a Flash String
  if (root["sensor"] == F("gps")) {
    // ...
  }

#else

#warning PROGMEM is not supported on this platform

#endif
}

void loop() {
  // not used in this example
}
