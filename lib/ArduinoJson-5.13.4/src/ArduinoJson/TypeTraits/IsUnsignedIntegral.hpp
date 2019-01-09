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
struct IsUnsignedIntegral {
  static const bool value =
      IsSame<T, unsigned char>::value || IsSame<T, unsigned short>::value ||
      IsSame<T, unsigned int>::value || IsSame<T, unsigned long>::value ||
#if ARDUINOJSON_USE_LONG_LONG
      IsSame<T, unsigned long long>::value ||
#endif
#if ARDUINOJSON_USE_INT64
      IsSame<T, unsigned __int64>::value ||
#endif
      false;
};
}
}
