// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "./ctype.hpp"

namespace ArduinoJson {
namespace Internals {

inline bool isInteger(const char* s) {
  if (!s || !*s) return false;
  if (issign(*s)) s++;
  while (isdigit(*s)) s++;
  return *s == '\0';
}
}  // namespace Internals
}  // namespace ArduinoJson
