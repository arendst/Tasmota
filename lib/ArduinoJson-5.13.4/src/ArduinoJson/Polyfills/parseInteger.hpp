// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stdlib.h>

#include "../Configuration.hpp"
#include "./ctype.hpp"

namespace ArduinoJson {
namespace Internals {
template <typename T>
T parseInteger(const char *s) {
  if (!s) return 0;  // NULL

  if (*s == 't') return 1;  // "true"

  T result = 0;
  bool negative_result = false;

  switch (*s) {
    case '-':
      negative_result = true;
      s++;
      break;
    case '+':
      s++;
      break;
  }

  while (isdigit(*s)) {
    result = T(result * 10 + T(*s - '0'));
    s++;
  }

  return negative_result ? T(~result + 1) : result;
}
}
}
