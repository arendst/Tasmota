// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../Configuration.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename TString, typename Enable = void>
struct StringTraits {};

template <typename TString>
struct StringTraits<const TString, void> : StringTraits<TString> {};

template <typename TString>
struct StringTraits<TString&, void> : StringTraits<TString> {};
}
}

#include "CharPointer.hpp"

#if ARDUINOJSON_ENABLE_STD_STRING || ARDUINOJSON_ENABLE_ARDUINO_STRING
#include "StdString.hpp"
#endif

#if ARDUINOJSON_ENABLE_STD_STREAM
#include "StdStream.hpp"
#endif

#if ARDUINOJSON_ENABLE_ARDUINO_STREAM
#include "ArduinoStream.hpp"
#endif

#if ARDUINOJSON_ENABLE_PROGMEM
#include "FlashString.hpp"
#endif

namespace ArduinoJson {
namespace TypeTraits {
template <typename T, typename Enable = void>
struct IsString {
  static const bool value = false;
};

template <typename T>
struct IsString<T, typename TypeTraits::EnableIf<
                       Internals::StringTraits<T>::has_equals>::type> {
  static const bool value = Internals::StringTraits<T>::has_equals;
};
}
}
