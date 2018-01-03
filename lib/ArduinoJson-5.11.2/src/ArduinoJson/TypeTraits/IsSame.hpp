// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace TypeTraits {

// A meta-function that returns true if types T and U are the same.
template <typename T, typename U>
struct IsSame {
  static const bool value = false;
};

template <typename T>
struct IsSame<T, T> {
  static const bool value = true;
};
}
}
