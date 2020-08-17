// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Configuration.hpp"
#include "IsSame.hpp"

namespace ArduinoJson {
namespace Internals {

// A meta-function that returns true if T is an integral type.
template <typename T>
struct IsSignedIntegral {
  static const bool value =
      IsSame<T, signed char>::value || IsSame<T, signed short>::value ||
      IsSame<T, signed int>::value || IsSame<T, signed long>::value ||
#if ARDUINOJSON_USE_LONG_LONG
      IsSame<T, signed long long>::value ||
#endif
#if ARDUINOJSON_USE_INT64
      IsSame<T, signed __int64>::value ||
#endif
      false;
};
}
}
