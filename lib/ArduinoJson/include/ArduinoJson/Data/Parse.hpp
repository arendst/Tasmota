// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include <stdlib.h>

namespace ArduinoJson {
namespace Internals {
template <typename TFloat>
TFloat parse(const char *);

template <>
inline float parse<float>(const char *s) {
  return static_cast<float>(strtod(s, NULL));
}

template <>
inline double parse<double>(const char *s) {
  return strtod(s, NULL);
}

template <>
inline long parse<long>(const char *s) {
  return strtol(s, NULL, 10);
}

template <>
inline unsigned long parse<unsigned long>(const char *s) {
  return strtoul(s, NULL, 10);
}

template <>
inline int parse<int>(const char *s) {
  return atoi(s);
}

#if ARDUINOJSON_USE_LONG_LONG
template <>
inline long long parse<long long>(const char *s) {
  return strtoll(s, NULL, 10);
}

template <>
inline unsigned long long parse<unsigned long long>(const char *s) {
  return strtoull(s, NULL, 10);
}
#endif

#if ARDUINOJSON_USE_INT64
template <>
inline __int64 parse<__int64>(const char *s) {
  return _strtoi64(s, NULL, 10);
}

template <>
inline unsigned __int64 parse<unsigned __int64>(const char *s) {
  return _strtoui64(s, NULL, 10);
}
#endif
}
}
