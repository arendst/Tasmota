// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "JsonArray.hpp"
#include "JsonObject.hpp"
#include "JsonObjectSubscript.hpp"

namespace ArduinoJson {

template <typename TStringRef>
inline JsonArray &JsonObject::createNestedArray_impl(TStringRef key) {
  if (!_buffer) return JsonArray::invalid();
  JsonArray &array = _buffer->createArray();
  set(key, array);
  return array;
}

template <typename TStringRef>
inline JsonObject &JsonObject::createNestedObject_impl(TStringRef key) {
  if (!_buffer) return JsonObject::invalid();
  JsonObject &object = _buffer->createObject();
  set(key, object);
  return object;
}
}
