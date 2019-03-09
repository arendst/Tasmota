// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "../JsonBuffer.hpp"
#include "../JsonVariant.hpp"
#include "../StringTraits/StringTraits.hpp"
#include "../TypeTraits/EnableIf.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename TSourceRef, typename Enable = void>
struct ValueSetter {
  template <typename TDestination>
  static bool set(JsonBuffer*, TDestination& destination, TSourceRef source) {
    destination = source;
    return true;
  }
};

template <typename TSourceRef>
struct ValueSetter<TSourceRef, typename TypeTraits::EnableIf<StringTraits<
                                   TSourceRef>::should_duplicate>::type> {
  template <typename TDestination>
  static bool set(JsonBuffer* buffer, TDestination& destination,
                  TSourceRef source) {
    const char* copy = buffer->strdup(source);
    if (!copy) return false;
    destination = copy;
    return true;
  }
};

template <typename TSourceRef>
struct ValueSetter<TSourceRef, typename TypeTraits::EnableIf<!StringTraits<
                                   TSourceRef>::should_duplicate>::type> {
  template <typename TDestination>
  static bool set(JsonBuffer*, TDestination& destination, TSourceRef source) {
    // unsigned char* -> char*
    destination = reinterpret_cast<const char*>(source);
    return true;
  }
};
}
}
