// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace Internals {

// A Print implementation that allows to write in a char[]
class StaticStringBuilder {
 public:
  StaticStringBuilder(char *buf, size_t size) : end(buf + size - 1), p(buf) {
    *p = '\0';
  }

  size_t print(char c) {
    if (p >= end) return 0;
    *p++ = c;
    *p = '\0';
    return 1;
  }

  size_t print(const char *s) {
    char *begin = p;
    while (p < end && *s) *p++ = *s++;
    *p = '\0';
    return size_t(p - begin);
  }

 private:
  char *end;
  char *p;
};
}
}
