// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace Internals {

template <typename TChar>
struct CharPointerTraits {
  class Reader {
    const TChar* _ptr;

   public:
    Reader(const TChar* ptr)
        : _ptr(ptr ? ptr : reinterpret_cast<const TChar*>("")) {}

    void move() {
      ++_ptr;
    }

    char current() const {
      return char(_ptr[0]);
    }

    char next() const {
      return char(_ptr[1]);
    }
  };

  static bool equals(const TChar* str, const char* expected) {
    return strcmp(reinterpret_cast<const char*>(str), expected) == 0;
  }

  template <typename Buffer>
  static char* duplicate(const TChar* str, Buffer* buffer) {
    if (!str) return NULL;
    size_t size = strlen(reinterpret_cast<const char*>(str)) + 1;
    void* dup = buffer->alloc(size);
    if (dup != NULL) memcpy(dup, str, size);
    return static_cast<char*>(dup);
  }

  static const bool has_append = false;
  static const bool has_equals = true;
  static const bool should_duplicate = false;
};

template <typename TChar>
struct StringTraits<TChar*, typename TypeTraits::EnableIf<
                                TypeTraits::IsChar<TChar>::value>::type>
    : CharPointerTraits<TChar> {};
}
}
