// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace TypeTraits {

// A meta-function that return the type T if Condition is true.
template <bool Condition, typename T = void>
struct EnableIf {};

template <typename T>
struct EnableIf<true, T> {
  typedef T type;
};
}
}
