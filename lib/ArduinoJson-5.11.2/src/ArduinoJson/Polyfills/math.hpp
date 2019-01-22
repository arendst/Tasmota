// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace Polyfills {
template <typename T>
bool isNaN(T x) {
  return x != x;
}

template <typename T>
bool isInfinity(T x) {
  return x != 0.0 && x * 2 == x;
}
}
}
