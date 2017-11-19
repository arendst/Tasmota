// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "IsBaseOf.hpp"

namespace ArduinoJson {
namespace TypeTraits {

class JsonVariantTag {};

template <typename T>
struct IsVariant : IsBaseOf<JsonVariantTag, T> {};
}
}
