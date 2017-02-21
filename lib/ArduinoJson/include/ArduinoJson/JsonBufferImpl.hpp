// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "Deserialization/JsonParser.hpp"

inline ArduinoJson::JsonArray &ArduinoJson::JsonBuffer::createArray() {
  JsonArray *ptr = new (this) JsonArray(this);
  return ptr ? *ptr : JsonArray::invalid();
}

inline ArduinoJson::JsonObject &ArduinoJson::JsonBuffer::createObject() {
  JsonObject *ptr = new (this) JsonObject(this);
  return ptr ? *ptr : JsonObject::invalid();
}
