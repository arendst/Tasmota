// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stddef.h>  // for size_t
#include <stdint.h>  // for uint8_t
#include <string.h>

#include "Data/NonCopyable.hpp"
#include "JsonVariant.hpp"
#include "TypeTraits/EnableIf.hpp"
#include "TypeTraits/IsArray.hpp"

namespace ArduinoJson {
class JsonArray;
class JsonObject;

// Entry point for using the library.
//
// Handle the memory management (done in derived classes) and calls the parser.
// This abstract class is implemented by StaticJsonBuffer which implements a
// fixed memory allocation.
class JsonBuffer : Internals::NonCopyable {
 public:
  // Allocates an empty JsonArray.
  //
  // Returns a reference to the new JsonArray or JsonArray::invalid() if the
  // allocation fails.
  JsonArray &createArray();

  // Allocates an empty JsonObject.
  //
  // Returns a reference to the new JsonObject or JsonObject::invalid() if the
  // allocation fails.
  JsonObject &createObject();

  // Duplicates a string
  //
  // const char* strdup(TValue);
  // TValue = const std::string&, const String&,
  template <typename TString>
  DEPRECATED("char* are duplicated, you don't need strdup() anymore")
  typename Internals::EnableIf<!Internals::IsArray<TString>::value,
                               const char *>::type strdup(const TString &src) {
    return Internals::StringTraits<TString>::duplicate(src, this);
  }
  //
  // const char* strdup(TValue);
  // TValue = char*, const char*, const FlashStringHelper*
  template <typename TString>
  DEPRECATED("char* are duplicated, you don't need strdup() anymore")
  const char *strdup(TString *src) {
    return Internals::StringTraits<TString *>::duplicate(src, this);
  }

  // Allocates n bytes in the JsonBuffer.
  // Return a pointer to the allocated memory or NULL if allocation fails.
  virtual void *alloc(size_t size) = 0;

 protected:
  // CAUTION: NO VIRTUAL DESTRUCTOR!
  // If we add a virtual constructor the Arduino compiler will add malloc()
  // and free() to the binary, adding 706 useless bytes.
  ~JsonBuffer() {}

  // Preserve aligment if necessary
  static FORCE_INLINE size_t round_size_up(size_t bytes) {
#if ARDUINOJSON_ENABLE_ALIGNMENT
    const size_t x = sizeof(void *) - 1;
    return (bytes + x) & ~x;
#else
    return bytes;
#endif
  }
};
}
