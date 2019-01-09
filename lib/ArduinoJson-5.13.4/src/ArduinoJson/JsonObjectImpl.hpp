// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

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
