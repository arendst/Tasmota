// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../Configuration.hpp"
#include "IsSame.hpp"

namespace ArduinoJson {
namespace TypeTraits {

// A meta-function that returns true if T is an integral type.
template <typename T>
struct IsUnsignedIntegral {
  static const bool value = TypeTraits::IsSame<T, unsigned char>::value ||
                            TypeTraits::IsSame<T, unsigned short>::value ||
                            TypeTraits::IsSame<T, unsigned int>::value ||
                            TypeTraits::IsSame<T, unsigned long>::value ||
#if ARDUINOJSON_USE_LONG_LONG
                            TypeTraits::IsSame<T, unsigned long long>::value ||
#endif

#if ARDUINOJSON_USE_INT64
                            TypeTraits::IsSame<T, unsigned __int64>::value ||
#endif
                            false;
};
}
}
