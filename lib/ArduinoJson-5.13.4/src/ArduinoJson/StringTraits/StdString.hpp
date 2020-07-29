// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#if ARDUINOJSON_ENABLE_STD_STRING || ARDUINOJSON_ENABLE_ARDUINO_STRING

#if ARDUINOJSON_ENABLE_ARDUINO_STRING
#include <WString.h>
#endif

#if ARDUINOJSON_ENABLE_STD_STRING
#include <string>
#endif

namespace ArduinoJson {
namespace Internals {

template <typename TString>
struct StdStringTraits {
  typedef const char* duplicate_t;

  template <typename Buffer>
  static duplicate_t duplicate(const TString& str, Buffer* buffer) {
    if (!str.c_str()) return NULL;  // <- Arduino string can return NULL
    size_t size = str.length() + 1;
    void* dup = buffer->alloc(size);
    if (dup != NULL) memcpy(dup, str.c_str(), size);
    return static_cast<duplicate_t>(dup);
  }

  static bool is_null(const TString& str) {
    // Arduino's String::c_str() can return NULL
    return !str.c_str();
  }

  struct Reader : CharPointerTraits<char>::Reader {
    Reader(const TString& str) : CharPointerTraits<char>::Reader(str.c_str()) {}
  };

  static bool equals(const TString& str, const char* expected) {
    // Arduino's String::c_str() can return NULL
    const char* actual = str.c_str();
    if (!actual || !expected) return actual == expected;
    return 0 == strcmp(actual, expected);
  }

  static void append(TString& str, char c) {
    str += c;
  }

  static void append(TString& str, const char* s) {
    str += s;
  }

  static const bool has_append = true;
  static const bool has_equals = true;
  static const bool should_duplicate = true;
};

#if ARDUINOJSON_ENABLE_ARDUINO_STRING
template <>
struct StringTraits<String, void> : StdStringTraits<String> {};
template <>
struct StringTraits<StringSumHelper, void> : StdStringTraits<StringSumHelper> {
};
#endif

#if ARDUINOJSON_ENABLE_STD_STRING
template <>
struct StringTraits<std::string, void> : StdStringTraits<std::string> {};
#endif
}  // namespace Internals
}  // namespace ArduinoJson

#endif
