// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../JsonArray.hpp"
#include "../JsonArraySubscript.hpp"
#include "../JsonObject.hpp"
#include "../JsonObjectSubscript.hpp"
#include "../JsonVariant.hpp"
#include "JsonSerializer.hpp"

inline void ArduinoJson::Internals::JsonSerializer::serialize(
    const JsonArray& array, JsonWriter& writer) {
  writer.beginArray();

  JsonArray::const_iterator it = array.begin();
  while (it != array.end()) {
    serialize(*it, writer);

    ++it;
    if (it == array.end()) break;

    writer.writeComma();
  }

  writer.endArray();
}

inline void ArduinoJson::Internals::JsonSerializer::serialize(
    const JsonArraySubscript& arraySubscript, JsonWriter& writer) {
  serialize(arraySubscript.as<JsonVariant>(), writer);
}

inline void ArduinoJson::Internals::JsonSerializer::serialize(
    const JsonObject& object, JsonWriter& writer) {
  writer.beginObject();

  JsonObject::const_iterator it = object.begin();
  while (it != object.end()) {
    writer.writeString(it->key);
    writer.writeColon();
    serialize(it->value, writer);

    ++it;
    if (it == object.end()) break;

    writer.writeComma();
  }

  writer.endObject();
}

template <typename TKey>
inline void ArduinoJson::Internals::JsonSerializer::serialize(
    const JsonObjectSubscript<TKey>& objectSubscript, JsonWriter& writer) {
  serialize(objectSubscript.template as<JsonVariant>(), writer);
}

inline void ArduinoJson::Internals::JsonSerializer::serialize(
    const JsonVariant& variant, JsonWriter& writer) {
  switch (variant._type) {
    case JSON_UNDEFINED:
      return;

    case JSON_ARRAY:
      serialize(*variant._content.asArray, writer);
      return;

    case JSON_OBJECT:
      serialize(*variant._content.asObject, writer);
      return;

    case JSON_STRING:
      writer.writeString(variant._content.asString);
      return;

    case JSON_UNPARSED:
      writer.writeRaw(variant._content.asString);
      return;

    case JSON_NEGATIVE_INTEGER:
      writer.writeRaw('-');
    case JSON_POSITIVE_INTEGER:
      writer.writeInteger(variant._content.asInteger);
      return;

    case JSON_BOOLEAN:
      writer.writeBoolean(variant._content.asInteger != 0);
      return;

    default:
      uint8_t decimals =
          static_cast<uint8_t>(variant._type - JSON_FLOAT_0_DECIMALS);
      writer.writeFloat(variant._content.asFloat, decimals);
  }
}
