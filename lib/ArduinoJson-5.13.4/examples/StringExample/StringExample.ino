// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License
//
// This example shows the different ways you can use String with ArduinoJson.
//
// Use String objects sparingly, because ArduinoJson duplicates them in the
// JsonBuffer. Prefer plain old char[], as they are more efficient in term of
// code size, speed, and memory usage.

#include <ArduinoJson.h>

void setup() {
  DynamicJsonBuffer jsonBuffer;

  // You can use a String as your JSON input.
  // WARNING: the content of the String will be duplicated in the JsonBuffer.
  String input =
      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
  JsonObject& root = jsonBuffer.parseObject(input);

  // You can use a String to get an element of a JsonObject
  // No duplication is done.
  long time = root[String("time")];

  // You can use a String to set an element of a JsonObject
  // WARNING: the content of the String will be duplicated in the JsonBuffer.
  root[String("time")] = time;

  // You can get a String from a JsonObject or JsonArray:
  // No duplication is done, at least not in the JsonBuffer.
  String sensor = root["sensor"];

  // Unfortunately, the following doesn't work (issue #118):
  // sensor = root["sensor"]; // <-  error "ambiguous overload for 'operator='"
  // As a workaround, you need to replace by:
  sensor = root["sensor"].as<String>();

  // You can set a String to a JsonObject or JsonArray:
  // WARNING: the content of the String will be duplicated in the JsonBuffer.
  root["sensor"] = sensor;

  // It works with RawJson too:
  root["sensor"] = RawJson(sensor);

  // You can also concatenate strings
  // WARNING: the content of the String will be duplicated in the JsonBuffer.
  root[String("sen") + "sor"] = String("gp") + "s";

  // You can compare the content of a JsonObject with a String
  if (root["sensor"] == sensor) {
    // ...
  }

  // Lastly, you can print the resulting JSON to a String
  String output;
  root.printTo(output);
}

void loop() {
  // not used in this example
}

// See also
// --------
//
// https://arduinojson.org/ contains the documentation for all the functions
// used above. It also includes an FAQ that will help you solve any problem.
//
// The book "Mastering ArduinoJson" contains a quick C++ course that explains
// how your microcontroller stores strings in memory. On several occasions, it
// shows how you can avoid String in your program.
// Learn more at https://arduinojson.org/book/
// Use the coupon code TWENTY for a 20% discount ❤❤❤❤❤
