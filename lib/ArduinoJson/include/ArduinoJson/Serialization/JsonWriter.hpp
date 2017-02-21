// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../Data/Encoding.hpp"
#include "../Data/JsonFloat.hpp"
#include "../Data/JsonInteger.hpp"
#include "../Polyfills/attributes.hpp"
#include "../Polyfills/math.hpp"
#include "../Polyfills/normalize.hpp"
#include "../Print.hpp"

namespace ArduinoJson {
namespace Internals {

// Writes the JSON tokens to a Print implementation
// This class is used by:
// - JsonArray::writeTo()
// - JsonObject::writeTo()
// - JsonVariant::writeTo()
// Its derived by PrettyJsonWriter that overrides some members to add
// indentation.
class JsonWriter {
 public:
  explicit JsonWriter(Print &sink) : _sink(sink), _length(0) {}

  // Returns the number of bytes sent to the Print implementation.
  // This is very handy for implementations of printTo() that must return the
  // number of bytes written.
  size_t bytesWritten() const {
    return _length;
  }

  void beginArray() {
    writeRaw('[');
  }
  void endArray() {
    writeRaw(']');
  }

  void beginObject() {
    writeRaw('{');
  }
  void endObject() {
    writeRaw('}');
  }

  void writeColon() {
    writeRaw(':');
  }
  void writeComma() {
    writeRaw(',');
  }

  void writeBoolean(bool value) {
    writeRaw(value ? "true" : "false");
  }

  void writeString(const char *value) {
    if (!value) {
      writeRaw("null");
    } else {
      writeRaw('\"');
      while (*value) writeChar(*value++);
      writeRaw('\"');
    }
  }

  void writeChar(char c) {
    char specialChar = Encoding::escapeChar(c);
    if (specialChar) {
      writeRaw('\\');
      writeRaw(specialChar);
    } else {
      writeRaw(c);
    }
  }

  void writeFloat(JsonFloat value, uint8_t digits = 2) {
    if (Polyfills::isNaN(value)) return writeRaw("NaN");

    if (value < 0.0) {
      writeRaw('-');
      value = -value;
    }

    if (Polyfills::isInfinity(value)) return writeRaw("Infinity");

    short powersOf10;
    if (value > 1000 || value < 0.001) {
      powersOf10 = Polyfills::normalize(value);
    } else {
      powersOf10 = 0;
    }

    // Round up last digit (so that print(1.999, 2) prints as "2.00")
    value += getRoundingBias(digits);

    // Extract the integer part of the value and print it
    JsonUInt int_part = static_cast<JsonUInt>(value);
    JsonFloat remainder = value - static_cast<JsonFloat>(int_part);
    writeInteger(int_part);

    // Print the decimal point, but only if there are digits beyond
    if (digits > 0) {
      writeRaw('.');
    }

    // Extract digits from the remainder one at a time
    while (digits-- > 0) {
      // Extract digit
      remainder *= 10.0;
      char currentDigit = char(remainder);
      remainder -= static_cast<JsonFloat>(currentDigit);

      // Print
      writeRaw(char('0' + currentDigit));
    }

    if (powersOf10 < 0) {
      writeRaw("e-");
      writeInteger(-powersOf10);
    }

    if (powersOf10 > 0) {
      writeRaw('e');
      writeInteger(powersOf10);
    }
  }

  void writeInteger(JsonUInt value) {
    char buffer[22];
    char *ptr = buffer + sizeof(buffer) - 1;

    *ptr = 0;
    do {
      *--ptr = static_cast<char>(value % 10 + '0');
      value /= 10;
    } while (value);

    writeRaw(ptr);
  }

  void writeRaw(const char *s) {
    _length += _sink.print(s);
  }
  void writeRaw(char c) {
    _length += _sink.write(c);
  }

 protected:
  Print &_sink;
  size_t _length;

 private:
  JsonWriter &operator=(const JsonWriter &);  // cannot be assigned

  static JsonFloat getLastDigit(uint8_t digits) {
    // Designed as a compromise between code size and speed
    switch (digits) {
      case 0:
        return 1e-0;
      case 1:
        return 1e-1;
      case 2:
        return 1e-2;
      case 3:
        return 1e-3;
      default:
        return getLastDigit(uint8_t(digits - 4)) * 1e-4;
    }
  }

  FORCE_INLINE static JsonFloat getRoundingBias(uint8_t digits) {
    return 0.5 * getLastDigit(digits);
  }
};
}
}
