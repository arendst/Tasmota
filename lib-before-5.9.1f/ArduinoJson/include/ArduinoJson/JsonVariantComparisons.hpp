// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "Data/JsonVariantComparer.hpp"

namespace ArduinoJson {
template <typename TVariant, typename TComparand>
inline bool operator==(const JsonVariantBase<TVariant> &variant,
                       TComparand comparand) {
  return Internals::JsonVariantComparer<TComparand>::equals(variant, comparand);
}

template <typename TVariant, typename TComparand>
inline typename TypeTraits::EnableIf<!TypeTraits::IsVariant<TComparand>::value,
                                     bool>::type
operator==(TComparand comparand, const JsonVariantBase<TVariant> &variant) {
  return Internals::JsonVariantComparer<TComparand>::equals(variant, comparand);
}

template <typename TVariant, typename TComparand>
inline bool operator!=(const JsonVariantBase<TVariant> &variant,
                       TComparand comparand) {
  return !Internals::JsonVariantComparer<TComparand>::equals(variant,
                                                             comparand);
}

template <typename TVariant, typename TComparand>
inline typename TypeTraits::EnableIf<!TypeTraits::IsVariant<TComparand>::value,
                                     bool>::type
operator!=(TComparand comparand, const JsonVariantBase<TVariant> &variant) {
  return !Internals::JsonVariantComparer<TComparand>::equals(variant,
                                                             comparand);
}

template <typename TVariant, typename TComparand>
inline bool operator<=(const JsonVariantBase<TVariant> &left,
                       TComparand right) {
  return left.template as<TComparand>() <= right;
}

template <typename TVariant, typename TComparand>
inline bool operator<=(TComparand comparand,
                       const JsonVariantBase<TVariant> &variant) {
  return comparand <= variant.template as<TComparand>();
}

template <typename TVariant, typename TComparand>
inline bool operator>=(const JsonVariantBase<TVariant> &variant,
                       TComparand comparand) {
  return variant.template as<TComparand>() >= comparand;
}

template <typename TVariant, typename TComparand>
inline bool operator>=(TComparand comparand,
                       const JsonVariantBase<TVariant> &variant) {
  return comparand >= variant.template as<TComparand>();
}

template <typename TVariant, typename TComparand>
inline bool operator<(const JsonVariantBase<TVariant> &varian,
                      TComparand comparand) {
  return varian.template as<TComparand>() < comparand;
}

template <typename TVariant, typename TComparand>
inline bool operator<(TComparand comparand,
                      const JsonVariantBase<TVariant> &variant) {
  return comparand < variant.template as<TComparand>();
}

template <typename TVariant, typename TComparand>
inline bool operator>(const JsonVariantBase<TVariant> &variant,
                      TComparand comparand) {
  return variant.template as<TComparand>() > comparand;
}

template <typename TVariant, typename TComparand>
inline bool operator>(TComparand comparand,
                      const JsonVariantBase<TVariant> &variant) {
  return comparand > variant.template as<TComparand>();
}
}
