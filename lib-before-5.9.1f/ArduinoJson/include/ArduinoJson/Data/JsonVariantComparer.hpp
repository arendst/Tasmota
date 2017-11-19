// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../JsonVariantBase.hpp"
#include "../StringTraits/StringTraits.hpp"
#include "../TypeTraits/EnableIf.hpp"

namespace ArduinoJson {
namespace Internals {
template <typename TComparand, typename Enable = void>
struct JsonVariantComparer {};

template <typename TString>
struct JsonVariantComparer<
    TString,
    typename TypeTraits::EnableIf<TypeTraits::IsString<TString>::value>::type> {
  template <typename TVariant>
  static bool equals(const JsonVariantBase<TVariant> &variant,
                     const TString &comparand) {
    const char *value = variant.template as<const char *>();
    return Internals::StringTraits<TString>::equals(comparand, value);
  }
};

template <typename TComparand>
struct JsonVariantComparer<
    TComparand, typename TypeTraits::EnableIf<
                    !TypeTraits::IsVariant<TComparand>::value &&
                    !TypeTraits::IsString<TComparand>::value>::type> {
  template <typename TVariant>
  static bool equals(const JsonVariantBase<TVariant> &variant,
                     const TComparand &comparand) {
    return variant.template as<TComparand>() == comparand;
  }
};

template <typename TVariant2>
struct JsonVariantComparer<TVariant2,
                           typename TypeTraits::EnableIf<
                               TypeTraits::IsVariant<TVariant2>::value>::type> {
  template <typename TVariant1>
  static bool equals(const JsonVariantBase<TVariant1> &left,
                     const TVariant2 &right) {
    if (left.template is<bool>() && right.template is<bool>())
      return left.template as<bool>() == right.template as<bool>();
    if (left.template is<JsonInteger>() && right.template is<JsonInteger>())
      return left.template as<JsonInteger>() ==
             right.template as<JsonInteger>();
    if (left.template is<JsonFloat>() && right.template is<JsonFloat>())
      return left.template as<JsonFloat>() == right.template as<JsonFloat>();
    if (left.template is<JsonArray>() && right.template is<JsonArray>())
      return left.template as<JsonArray>() == right.template as<JsonArray>();
    if (left.template is<JsonObject>() && right.template is<JsonObject>())
      return left.template as<JsonObject>() == right.template as<JsonObject>();
    if (left.template is<char *>() && right.template is<char *>())
      return strcmp(left.template as<char *>(), right.template as<char *>()) ==
             0;

    return false;
  }
};
}
}
