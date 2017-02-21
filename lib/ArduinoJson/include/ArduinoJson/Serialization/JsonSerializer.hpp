// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "JsonWriter.hpp"

namespace ArduinoJson {

class JsonArray;
class JsonArraySubscript;
class JsonObject;
template <typename TKey>
class JsonObjectSubscript;
class JsonVariant;

namespace Internals {

class JsonSerializer {
 public:
  static void serialize(const JsonArray &, JsonWriter &);
  static void serialize(const JsonArraySubscript &, JsonWriter &);
  static void serialize(const JsonObject &, JsonWriter &);
  template <typename TKey>
  static void serialize(const JsonObjectSubscript<TKey> &, JsonWriter &);
  static void serialize(const JsonVariant &, JsonWriter &);
};
}
}
