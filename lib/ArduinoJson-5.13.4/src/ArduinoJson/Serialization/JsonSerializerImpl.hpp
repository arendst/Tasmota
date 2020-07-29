// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../JsonArray.hpp"
#include "../JsonArraySubscript.hpp"
#include "../JsonObject.hpp"
#include "../JsonObjectSubscript.hpp"
#include "../JsonVariant.hpp"
#include "JsonSerializer.hpp"

template <typename Writer>
inline void ArduinoJson::Internals::JsonSerializer<Writer>::serialize(
    const JsonArray& array, Writer& writer) {
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

template <typename Writer>
inline void ArduinoJson::Internals::JsonSerializer<Writer>::serialize(
    const JsonArraySubscript& arraySubscript, Writer& writer) {
  serialize(arraySubscript.as<JsonVariant>(), writer);
}

template <typename Writer>
inline void ArduinoJson::Internals::JsonSerializer<Writer>::serialize(
    const JsonObject& object, Writer& writer) {
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

template <typename Writer>
template <typename TKey>
inline void ArduinoJson::Internals::JsonSerializer<Writer>::serialize(
    const JsonObjectSubscript<TKey>& objectSubscript, Writer& writer) {
  serialize(objectSubscript.template as<JsonVariant>(), writer);
}

template <typename Writer>
inline void ArduinoJson::Internals::JsonSerializer<Writer>::serialize(
    const JsonVariant& variant, Writer& writer) {
  switch (variant._type) {
    case JSON_FLOAT:
      writer.writeFloat(variant._content.asFloat);
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
      writer.writeRaw('-');  // Falls through.

    case JSON_POSITIVE_INTEGER:
      writer.writeInteger(variant._content.asInteger);
      return;

    case JSON_BOOLEAN:
      writer.writeBoolean(variant._content.asInteger != 0);
      return;

    default:  // JSON_UNDEFINED
      return;
  }
}
