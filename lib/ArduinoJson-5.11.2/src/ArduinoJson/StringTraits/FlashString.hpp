// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#if ARDUINOJSON_ENABLE_PROGMEM

namespace ArduinoJson {
namespace Internals {
template <>
struct StringTraits<const __FlashStringHelper*, void> {
  class Reader {
    const char* _ptr;

   public:
    Reader(const __FlashStringHelper* ptr)
        : _ptr(reinterpret_cast<const char*>(ptr)) {}

    void move() {
      _ptr++;
    }

    char current() const {
      return pgm_read_byte_near(_ptr);
    }

    char next() const {
      return pgm_read_byte_near(_ptr + 1);
    }
  };

  static bool equals(const __FlashStringHelper* str, const char* expected) {
    return strcmp_P(expected, (const char*)str) == 0;
  }

  template <typename Buffer>
  static char* duplicate(const __FlashStringHelper* str, Buffer* buffer) {
    if (!str) return NULL;
    size_t size = strlen_P((const char*)str) + 1;
    void* dup = buffer->alloc(size);
    if (dup != NULL) memcpy_P(dup, (const char*)str, size);
    return static_cast<char*>(dup);
  }

  static const bool has_append = false;
  static const bool has_equals = true;
  static const bool should_duplicate = true;
};
}
}

#endif
