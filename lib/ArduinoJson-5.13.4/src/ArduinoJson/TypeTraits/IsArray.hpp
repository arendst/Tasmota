// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

// A meta-function that return the type T without the const modifier
template <typename T>
struct IsArray {
  static const bool value = false;
};
template <typename T>
struct IsArray<T[]> {
  static const bool value = true;
};
template <typename T, size_t N>
struct IsArray<T[N]> {
  static const bool value = true;
};
}
}
