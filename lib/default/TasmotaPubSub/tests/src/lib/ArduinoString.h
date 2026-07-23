/*
  ArduinoString.h - Host-side minimal Arduino `String` shim for TasmotaPubSub tests.

  The library only uses a tiny slice of the Arduino String API:
    - `String domain;`            (default construction)
    - `this->domain = domain;`    (assignment from const char*)
    - `this->domain = "";`        (assignment from string literal)
    - `domain.length()`           (is-empty test)
    - `domain.c_str()`            (passed to Client::connect)
  This class backs those with std::string and handles nullptr safely.
*/

#ifndef TASMOTA_PUBSUB_TEST_ARDUINO_STRING_H
#define TASMOTA_PUBSUB_TEST_ARDUINO_STRING_H

#include <cstddef>
#include <string>

class String {
public:
    String();
    String(const char* cstr);
    String(const String& other) = default;

    String& operator=(const char* cstr);
    String& operator=(const String& other) = default;

    size_t length() const;
    const char* c_str() const;

private:
    std::string _str;
};

#endif  // TASMOTA_PUBSUB_TEST_ARDUINO_STRING_H
