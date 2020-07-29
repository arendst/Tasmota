// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

// A metafunction that returns the type of the value returned by
// JsonVariant::as<T>()
template <typename T>
struct JsonVariantAs {
  typedef T type;
};

template <>
struct JsonVariantAs<char*> {
  typedef const char* type;
};

template <>
struct JsonVariantAs<JsonArray> {
  typedef JsonArray& type;
};

template <>
struct JsonVariantAs<const JsonArray> {
  typedef const JsonArray& type;
};

template <>
struct JsonVariantAs<JsonObject> {
  typedef JsonObject& type;
};

template <>
struct JsonVariantAs<const JsonObject> {
  typedef const JsonObject& type;
};
}
}
