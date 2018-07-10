// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "Data/JsonVariantAs.hpp"
#include "Polyfills/attributes.hpp"
#include "StringTraits/StringTraits.hpp"
#include "TypeTraits/EnableIf.hpp"

namespace ArduinoJson {

// Forward declarations.
class JsonArraySubscript;
template <typename TKey>
class JsonObjectSubscript;

template <typename TImpl>
class JsonVariantSubscripts {
 public:
  // Mimics an array or an object.
  // Returns the size of the array or object if the variant has that type.
  // Returns 0 if the variant is neither an array nor an object
  size_t size() const {
    return impl()->template as<JsonArray>().size() +
           impl()->template as<JsonObject>().size();
  }

  // Mimics an array.
  // Returns the element at specified index if the variant is an array.
  // Returns JsonVariant::invalid() if the variant is not an array.
  FORCE_INLINE const JsonArraySubscript operator[](size_t index) const;
  FORCE_INLINE JsonArraySubscript operator[](size_t index);

  // Mimics an object.
  // Returns the value associated with the specified key if the variant is
  // an object.
  // Return JsonVariant::invalid() if the variant is not an object.
  //
  // const JsonObjectSubscript operator[](TKey) const;
  // TKey = const std::string&, const String&
  template <typename TString>
  FORCE_INLINE typename TypeTraits::EnableIf<
      Internals::StringTraits<TString>::has_equals,
      const JsonObjectSubscript<const TString &> >::type
  operator[](const TString &key) const {
    return impl()->template as<JsonObject>()[key];
  }
  //
  // const JsonObjectSubscript operator[](TKey) const;
  // TKey = const std::string&, const String&
  template <typename TString>
  FORCE_INLINE typename TypeTraits::EnableIf<
      Internals::StringTraits<TString>::has_equals,
      JsonObjectSubscript<const TString &> >::type
  operator[](const TString &key) {
    return impl()->template as<JsonObject>()[key];
  }
  //
  // JsonObjectSubscript operator[](TKey);
  // TKey = const char*, const char[N], const FlashStringHelper*
  template <typename TString>
  FORCE_INLINE typename TypeTraits::EnableIf<
      Internals::StringTraits<const TString *>::has_equals,
      JsonObjectSubscript<const TString *> >::type
  operator[](const TString *key) {
    return impl()->template as<JsonObject>()[key];
  }
  //
  // JsonObjectSubscript operator[](TKey);
  // TKey = const char*, const char[N], const FlashStringHelper*
  template <typename TString>
  FORCE_INLINE typename TypeTraits::EnableIf<
      Internals::StringTraits<TString *>::has_equals,
      const JsonObjectSubscript<const TString *> >::type
  operator[](const TString *key) const {
    return impl()->template as<JsonObject>()[key];
  }

 private:
  const TImpl *impl() const {
    return static_cast<const TImpl *>(this);
  }
};
}
