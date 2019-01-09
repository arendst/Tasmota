// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

// A meta-function that return the type T without the const modifier
template <typename T>
struct RemoveConst {
  typedef T type;
};
template <typename T>
struct RemoveConst<const T> {
  typedef T type;
};
}
}
