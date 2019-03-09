// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace Polyfills {

inline bool isdigit(char c) {
  return '0' <= c && c <= '9';
}

inline bool issign(char c) {
  return '-' == c || c == '+';
}
}
}
