// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "StringTraits/StringTraits.hpp"
#include "TypeTraits/EnableIf.hpp"
#include "TypeTraits/IsVariant.hpp"

namespace ArduinoJson {

template <typename TImpl>
class JsonVariantComparisons {
 public:
  template <typename TComparand>
  friend bool operator==(const JsonVariantComparisons &variant,
                         TComparand comparand) {
    return variant.equals(comparand);
  }

  template <typename TComparand>
  friend
      typename TypeTraits::EnableIf<!TypeTraits::IsVariant<TComparand>::value,
                                    bool>::type
      operator==(TComparand comparand, const JsonVariantComparisons &variant) {
    return variant.equals(comparand);
  }

  template <typename TComparand>
  friend bool operator!=(const JsonVariantComparisons &variant,
                         TComparand comparand) {
    return !variant.equals(comparand);
  }

  template <typename TComparand>
  friend
      typename TypeTraits::EnableIf<!TypeTraits::IsVariant<TComparand>::value,
                                    bool>::type
      operator!=(TComparand comparand, const JsonVariantComparisons &variant) {
    return !variant.equals(comparand);
  }

  template <typename TComparand>
  friend bool operator<=(const JsonVariantComparisons &left, TComparand right) {
    return left.as<TComparand>() <= right;
  }

  template <typename TComparand>
  friend bool operator<=(TComparand comparand,
                         const JsonVariantComparisons &variant) {
    return comparand <= variant.as<TComparand>();
  }

  template <typename TComparand>
  friend bool operator>=(const JsonVariantComparisons &variant,
                         TComparand comparand) {
    return variant.as<TComparand>() >= comparand;
  }

  template <typename TComparand>
  friend bool operator>=(TComparand comparand,
                         const JsonVariantComparisons &variant) {
    return comparand >= variant.as<TComparand>();
  }

  template <typename TComparand>
  friend bool operator<(const JsonVariantComparisons &varian,
                        TComparand comparand) {
    return varian.as<TComparand>() < comparand;
  }

  template <typename TComparand>
  friend bool operator<(TComparand comparand,
                        const JsonVariantComparisons &variant) {
    return comparand < variant.as<TComparand>();
  }

  template <typename TComparand>
  friend bool operator>(const JsonVariantComparisons &variant,
                        TComparand comparand) {
    return variant.as<TComparand>() > comparand;
  }

  template <typename TComparand>
  friend bool operator>(TComparand comparand,
                        const JsonVariantComparisons &variant) {
    return comparand > variant.as<TComparand>();
  }

 private:
  const TImpl *impl() const {
    return static_cast<const TImpl *>(this);
  }

  template <typename T>
  const typename Internals::JsonVariantAs<T>::type as() const {
    return impl()->template as<T>();
  }

  template <typename T>
  bool is() const {
    return impl()->template is<T>();
  }

  template <typename TString>
  typename TypeTraits::EnableIf<TypeTraits::IsString<TString>::value,
                                bool>::type
  equals(const TString &comparand) const {
    const char *value = as<const char *>();
    return Internals::StringTraits<TString>::equals(comparand, value);
  }

  template <typename TComparand>
  typename TypeTraits::EnableIf<!TypeTraits::IsVariant<TComparand>::value &&
                                    !TypeTraits::IsString<TComparand>::value,
                                bool>::type
  equals(const TComparand &comparand) const {
    return as<TComparand>() == comparand;
  }

  template <typename TVariant2>
  bool equals(const JsonVariantComparisons<TVariant2> &right) const {
    using namespace Internals;
    if (is<bool>() && right.template is<bool>())
      return as<bool>() == right.template as<bool>();
    if (is<JsonInteger>() && right.template is<JsonInteger>())
      return as<JsonInteger>() == right.template as<JsonInteger>();
    if (is<JsonFloat>() && right.template is<JsonFloat>())
      return as<JsonFloat>() == right.template as<JsonFloat>();
    if (is<JsonArray>() && right.template is<JsonArray>())
      return as<JsonArray>() == right.template as<JsonArray>();
    if (is<JsonObject>() && right.template is<JsonObject>())
      return as<JsonObject>() == right.template as<JsonObject>();
    if (is<char *>() && right.template is<char *>())
      return strcmp(as<char *>(), right.template as<char *>()) == 0;

    return false;
  }
};
}
