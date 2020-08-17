// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../JsonBuffer.hpp"
#include "../JsonVariant.hpp"
#include "../StringTraits/StringTraits.hpp"
#include "../TypeTraits/EnableIf.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename Source, typename Enable = void>
struct ValueSaver {
  template <typename Destination>
  static bool save(JsonBuffer*, Destination& destination, Source source) {
    destination = source;
    return true;
  }
};

template <typename Source>
struct ValueSaver<
    Source, typename EnableIf<StringTraits<Source>::should_duplicate>::type> {
  template <typename Destination>
  static bool save(JsonBuffer* buffer, Destination& dest, Source source) {
    if (!StringTraits<Source>::is_null(source)) {
      typename StringTraits<Source>::duplicate_t dup =
          StringTraits<Source>::duplicate(source, buffer);
      if (!dup) return false;
      dest = dup;
    } else {
      dest = reinterpret_cast<const char*>(0);
    }
    return true;
  }
};

// const char*, const signed char*, const unsigned char*
template <typename Char>
struct ValueSaver<
    Char*, typename EnableIf<!StringTraits<Char*>::should_duplicate>::type> {
  template <typename Destination>
  static bool save(JsonBuffer*, Destination& dest, Char* source) {
    dest = reinterpret_cast<const char*>(source);
    return true;
  }
};
}
}
