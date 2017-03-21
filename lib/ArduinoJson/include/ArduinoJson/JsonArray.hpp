// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "Data/JsonBufferAllocated.hpp"
#include "Data/List.hpp"
#include "Data/ReferenceType.hpp"
#include "Data/ValueSetter.hpp"
#include "JsonVariant.hpp"
#include "Serialization/JsonPrintable.hpp"
#include "StringTraits/StringTraits.hpp"
#include "TypeTraits/EnableIf.hpp"
#include "TypeTraits/IsArray.hpp"
#include "TypeTraits/IsFloatingPoint.hpp"
#include "TypeTraits/IsSame.hpp"

// Returns the size (in bytes) of an array with n elements.
// Can be very handy to determine the size of a StaticJsonBuffer.
#define JSON_ARRAY_SIZE(NUMBER_OF_ELEMENTS) \
  (sizeof(JsonArray) + (NUMBER_OF_ELEMENTS) * sizeof(JsonArray::node_type))

namespace ArduinoJson {

// Forward declarations
class JsonObject;
class JsonBuffer;
class JsonArraySubscript;

// An array of JsonVariant.
//
// The constructor is private, instances must be created via
// JsonBuffer::createArray() or JsonBuffer::parseArray().
// A JsonArray can be serialized to a JSON string via JsonArray::printTo().
// It can also be deserialized from a JSON string via JsonBuffer::parseArray().
class JsonArray : public Internals::JsonPrintable<JsonArray>,
                  public Internals::ReferenceType,
                  public Internals::List<JsonVariant>,
                  public Internals::JsonBufferAllocated {
 public:
  // Create an empty JsonArray attached to the specified JsonBuffer.
  // You should not call this constructor directly.
  // Instead, use JsonBuffer::createArray() or JsonBuffer::parseArray().
  explicit JsonArray(JsonBuffer *buffer)
      : Internals::List<JsonVariant>(buffer) {}

  // Gets the value at the specified index
  const JsonArraySubscript operator[](size_t index) const;

  // Gets or sets the value at specified index
  JsonArraySubscript operator[](size_t index);

  // Adds the specified value at the end of the array.
  //
  // bool add(TValue);
  // TValue = bool, long, int, short, float, double, RawJson, JsonVariant,
  //          const std::string&, const String&,
  //          const JsonArray&, const JsonObject&
  template <typename T>
  typename TypeTraits::EnableIf<!TypeTraits::IsArray<T>::value, bool>::type add(
      const T &value) {
    return add_impl<const T &>(value);
  }
  //
  // bool add(TValue);
  // TValue = const char*, const char[N], const FlashStringHelper*
  template <typename T>
  bool add(const T *value) {
    return add_impl<const T *>(value);
  }
  //
  // bool add(TValue value, uint8_t decimals);
  // TValue = float, double
  template <typename T>
  bool add(T value, uint8_t decimals) {
    return add_impl<const JsonVariant &>(JsonVariant(value, decimals));
  }

  // Sets the value at specified index.
  //
  // bool add(size_t index, TValue);
  // TValue = bool, long, int, short, float, double, RawJson, JsonVariant,
  //          const std::string&, const String&,
  //          const JsonArray&, const JsonObject&
  template <typename T>
  typename TypeTraits::EnableIf<!TypeTraits::IsArray<T>::value, bool>::type set(
      size_t index, const T &value) {
    return set_impl<const T &>(index, value);
  }
  //
  // bool add(size_t index, TValue);
  // TValue = const char*, const char[N], const FlashStringHelper*
  template <typename T>
  bool set(size_t index, const T *value) {
    return set_impl<const T *>(index, value);
  }
  //
  // bool set(size_t index, TValue value, uint8_t decimals);
  // TValue = float, double
  template <typename T>
  typename TypeTraits::EnableIf<TypeTraits::IsFloatingPoint<T>::value,
                                bool>::type
  set(size_t index, T value, uint8_t decimals) {
    return set_impl<const JsonVariant &>(index, JsonVariant(value, decimals));
  }

  // Gets the value at the specified index.
  template <typename T>
  typename Internals::JsonVariantAs<T>::type get(size_t index) const {
    node_type *node = findNode(index);
    return node ? node->content.as<T>()
                : Internals::JsonVariantDefault<T>::get();
  }

  // Check the type of the value at specified index.
  template <typename T>
  bool is(size_t index) const {
    node_type *node = findNode(index);
    return node ? node->content.is<T>() : false;
  }

  // Creates a JsonArray and adds a reference at the end of the array.
  // It's a shortcut for JsonBuffer::createArray() and JsonArray::add()
  JsonArray &createNestedArray();

  // Creates a JsonObject and adds a reference at the end of the array.
  // It's a shortcut for JsonBuffer::createObject() and JsonArray::add()
  JsonObject &createNestedObject();

  // Removes element at specified index.
  void removeAt(size_t index) {
    removeNode(findNode(index));
  }

  // Returns a reference an invalid JsonArray.
  // This object is meant to replace a NULL pointer.
  // This is used when memory allocation or JSON parsing fail.
  static JsonArray &invalid() {
    static JsonArray instance(NULL);
    return instance;
  }

  // Imports a 1D array
  template <typename T, size_t N>
  bool copyFrom(T (&array)[N]) {
    return copyFrom(array, N);
  }

  // Imports a 1D array
  template <typename T>
  bool copyFrom(T *array, size_t len) {
    bool ok = true;
    for (size_t i = 0; i < len; i++) {
      ok &= add(array[i]);
    }
    return ok;
  }

  // Imports a 2D array
  template <typename T, size_t N1, size_t N2>
  bool copyFrom(T (&array)[N1][N2]) {
    bool ok = true;
    for (size_t i = 0; i < N1; i++) {
      JsonArray &nestedArray = createNestedArray();
      for (size_t j = 0; j < N2; j++) {
        ok &= nestedArray.add(array[i][j]);
      }
    }
    return ok;
  }

  // Exports a 1D array
  template <typename T, size_t N>
  size_t copyTo(T (&array)[N]) const {
    return copyTo(array, N);
  }

  // Exports a 1D array
  template <typename T>
  size_t copyTo(T *array, size_t len) const {
    size_t i = 0;
    for (const_iterator it = begin(); it != end() && i < len; ++it)
      array[i++] = *it;
    return i;
  }

  // Exports a 2D array
  template <typename T, size_t N1, size_t N2>
  void copyTo(T (&array)[N1][N2]) const {
    size_t i = 0;
    for (const_iterator it = begin(); it != end() && i < N1; ++it) {
      it->as<JsonArray>().copyTo(array[i++]);
    }
  }

 private:
  node_type *findNode(size_t index) const {
    node_type *node = _firstNode;
    while (node && index--) node = node->next;
    return node;
  }

  template <typename TValueRef>
  bool set_impl(size_t index, TValueRef value) {
    node_type *node = findNode(index);
    if (!node) return false;

    return Internals::ValueSetter<TValueRef>::set(_buffer, node->content,
                                                  value);
  }

  template <typename TValueRef>
  bool add_impl(TValueRef value) {
    node_type *node = addNewNode();
    if (!node) return false;

    return Internals::ValueSetter<TValueRef>::set(_buffer, node->content,
                                                  value);
  }
};

namespace Internals {
template <>
struct JsonVariantDefault<JsonArray> {
  static JsonArray &get() {
    return JsonArray::invalid();
  }
};
}
}
