// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "../TypeTraits/FloatTraits.hpp"
#include "./ctype.hpp"
#include "./math.hpp"

namespace ArduinoJson {
namespace Polyfills {

template <typename T>
inline T parseFloat(const char* s) {
  typedef TypeTraits::FloatTraits<T> traits;
  typedef typename traits::mantissa_type mantissa_t;
  typedef typename traits::exponent_type exponent_t;

  if (!s) return 0;  // NULL

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

  if (*s == 't') return 1;  // true
  if (*s == 'n' || *s == 'N') return traits::nan();
  if (*s == 'i' || *s == 'I')
    return negative_result ? -traits::inf() : traits::inf();

  mantissa_t mantissa = 0;
  exponent_t exponent_offset = 0;

  while (isdigit(*s)) {
    if (mantissa < traits::mantissa_max / 10)
      mantissa = mantissa * 10 + (*s - '0');
    else
      exponent_offset++;
    s++;
  }

  if (*s == '.') {
    s++;
    while (isdigit(*s)) {
      if (mantissa < traits::mantissa_max / 10) {
        mantissa = mantissa * 10 + (*s - '0');
        exponent_offset--;
      }
      s++;
    }
  }

  int exponent = 0;
  if (*s == 'e' || *s == 'E') {
    s++;
    bool negative_exponent = false;
    if (*s == '-') {
      negative_exponent = true;
      s++;
    } else if (*s == '+') {
      s++;
    }

    while (isdigit(*s)) {
      exponent = exponent * 10 + (*s - '0');
      if (exponent + exponent_offset > traits::exponent_max) {
        if (negative_exponent)
          return negative_result ? -0.0f : 0.0f;
        else
          return negative_result ? -traits::inf() : traits::inf();
      }
      s++;
    }
    if (negative_exponent) exponent = -exponent;
  }
  exponent += exponent_offset;

  T result = traits::make_float(static_cast<T>(mantissa), exponent);

  return negative_result ? -result : result;
}
}
}
