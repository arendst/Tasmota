// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "IsSame.hpp"

namespace ArduinoJson {
namespace Internals {

// A meta-function that returns true if T is a charater
template <typename T>
struct IsChar {
  static const bool value = IsSame<T, char>::value ||
                            IsSame<T, signed char>::value ||
                            IsSame<T, unsigned char>::value;
};

template <typename T>
struct IsChar<const T> : IsChar<T> {};
}
}
