// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../Print.hpp"

namespace ArduinoJson {
namespace Internals {

// A Print implementation that allows to write in a char[]
class StaticStringBuilder : public Print {
 public:
  StaticStringBuilder(char *buf, size_t size)
      : buffer(buf), capacity(size - 1), length(0) {
    buffer[0] = '\0';
  }

  virtual size_t write(uint8_t c) {
    if (length >= capacity) return 0;

    buffer[length++] = c;
    buffer[length] = '\0';
    return 1;
  }

 private:
  char *buffer;
  size_t capacity;
  size_t length;
};
}
}
