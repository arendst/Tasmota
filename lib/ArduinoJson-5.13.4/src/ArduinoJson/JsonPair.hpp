// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonVariant.hpp"

namespace ArduinoJson {

// A key value pair for JsonObject.
struct JsonPair {
  const char* key;
  JsonVariant value;
};
}
