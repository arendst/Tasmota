// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

// A meta-function that return the type T without the reference modifier.
template <typename T>
struct RemoveReference {
  typedef T type;
};
template <typename T>
struct RemoveReference<T&> {
  typedef T type;
};
}
}
