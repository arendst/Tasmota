// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {

namespace Internals {
// A special type of data that can be used to insert pregenerated JSON portions.
template <typename T>
class RawJsonString {
 public:
  explicit RawJsonString(T str) : _str(str) {}
  operator T() const {
    return _str;
  }

 private:
  T _str;
};

template <typename String>
struct StringTraits<RawJsonString<String>, void> {
  static bool is_null(RawJsonString<String> source) {
    return StringTraits<String>::is_null(static_cast<String>(source));
  }

  typedef RawJsonString<const char*> duplicate_t;

  template <typename Buffer>
  static duplicate_t duplicate(RawJsonString<String> source, Buffer* buffer) {
    return duplicate_t(StringTraits<String>::duplicate(source, buffer));
  }

  static const bool has_append = false;
  static const bool has_equals = false;
  static const bool should_duplicate = StringTraits<String>::should_duplicate;
};
}

template <typename T>
inline Internals::RawJsonString<T> RawJson(T str) {
  return Internals::RawJsonString<T>(str);
}
}
