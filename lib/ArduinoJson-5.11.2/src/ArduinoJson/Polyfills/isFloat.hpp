// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include <string.h>  // for strcmp
#include "./ctype.hpp"

namespace ArduinoJson {
namespace Polyfills {

inline bool isFloat(const char* s) {
  if (!s) return false;

  if (!strcmp(s, "NaN")) return true;
  if (issign(*s)) s++;
  if (!strcmp(s, "Infinity")) return true;
  if (*s == '\0') return false;

  while (isdigit(*s)) s++;

  if (*s == '.') {
    s++;
    while (isdigit(*s)) s++;
  }

  if (*s == 'e' || *s == 'E') {
    s++;
    if (issign(*s)) s++;
    if (!isdigit(*s)) return false;
    while (isdigit(*s)) s++;
  }

  return *s == '\0';
}
}
}
