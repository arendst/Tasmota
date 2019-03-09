// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "IsSame.hpp"
#include "IsSignedIntegral.hpp"
#include "IsUnsignedIntegral.hpp"

namespace ArduinoJson {
namespace TypeTraits {

// A meta-function that returns true if T is an integral type.
template <typename T>
struct IsIntegral {
  static const bool value = TypeTraits::IsSignedIntegral<T>::value ||
                            TypeTraits::IsUnsignedIntegral<T>::value ||
                            TypeTraits::IsSame<T, char>::value;
  // CAUTION: differs from std::is_integral as it doesn't include bool
};

template <typename T>
struct IsIntegral<const T> : IsIntegral<T> {};
}
}
