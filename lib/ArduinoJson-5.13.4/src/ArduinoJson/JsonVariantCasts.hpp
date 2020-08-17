// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/JsonVariantAs.hpp"
#include "Polyfills/attributes.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename TImpl>
class JsonVariantCasts {
 public:
#if ARDUINOJSON_ENABLE_DEPRECATED
  DEPRECATED("use as<JsonArray>() instead")
  FORCE_INLINE JsonArray &asArray() const {
    return impl()->template as<JsonArray>();
  }

  DEPRECATED("use as<JsonObject>() instead")
  FORCE_INLINE JsonObject &asObject() const {
    return impl()->template as<JsonObject>();
  }

  DEPRECATED("use as<char*>() instead")
  FORCE_INLINE const char *asString() const {
    return impl()->template as<const char *>();
  }
#endif

  // Gets the variant as an array.
  // Returns a reference to the JsonArray or JsonArray::invalid() if the
  // variant
  // is not an array.
  FORCE_INLINE operator JsonArray &() const {
    return impl()->template as<JsonArray &>();
  }

  // Gets the variant as an object.
  // Returns a reference to the JsonObject or JsonObject::invalid() if the
  // variant is not an object.
  FORCE_INLINE operator JsonObject &() const {
    return impl()->template as<JsonObject &>();
  }

  template <typename T>
  FORCE_INLINE operator T() const {
    return impl()->template as<T>();
  }

 private:
  const TImpl *impl() const {
    return static_cast<const TImpl *>(this);
  }
};
}
}
