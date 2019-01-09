// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonWriter.hpp"

namespace ArduinoJson {

class JsonArray;
class JsonObject;
class JsonVariant;

namespace Internals {

class JsonArraySubscript;
template <typename TKey>
class JsonObjectSubscript;

template <typename Writer>
class JsonSerializer {
 public:
  static void serialize(const JsonArray &, Writer &);
  static void serialize(const JsonArraySubscript &, Writer &);
  static void serialize(const JsonObject &, Writer &);
  template <typename TKey>
  static void serialize(const JsonObjectSubscript<TKey> &, Writer &);
  static void serialize(const JsonVariant &, Writer &);
};
}
}
