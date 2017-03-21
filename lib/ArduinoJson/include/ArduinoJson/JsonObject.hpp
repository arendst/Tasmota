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
#include "JsonPair.hpp"
#include "Serialization/JsonPrintable.hpp"
#include "StringTraits/StringTraits.hpp"
#include "TypeTraits/EnableIf.hpp"
#include "TypeTraits/IsArray.hpp"
#include "TypeTraits/IsFloatingPoint.hpp"
#include "TypeTraits/IsSame.hpp"

// Returns the size (in bytes) of an object with n elements.
// Can be very handy to determine the size of a StaticJsonBuffer.
#define JSON_OBJECT_SIZE(NUMBER_OF_ELEMENTS) \
  (sizeof(JsonObject) + (NUMBER_OF_ELEMENTS) * sizeof(JsonObject::node_type))

namespace ArduinoJson {

// Forward declarations
class JsonArray;
class JsonBuffer;

// A dictionary of JsonVariant indexed by string (char*)
//
// The constructor is private, instances must be created via
// JsonBuffer::createObject() or JsonBuffer::parseObject().
// A JsonObject can be serialized to a JSON string via JsonObject::printTo().
// It can also be deserialized from a JSON string via JsonBuffer::parseObject().
class JsonObject : public Internals::JsonPrintable<JsonObject>,
                   public Internals::ReferenceType,
                   public Internals::List<JsonPair>,
                   public Internals::JsonBufferAllocated {
 public:
  // Create an empty JsonArray attached to the specified JsonBuffer.
  // You should not use this constructor directly.
  // Instead, use JsonBuffer::createObject() or JsonBuffer.parseObject().
  explicit JsonObject(JsonBuffer* buffer) : Internals::List<JsonPair>(buffer) {}

  // Gets or sets the value associated with the specified key.
  //
  // JsonObjectSubscript operator[](TKey)
  // TKey = const std::string&, const String&
  template <typename TString>
  typename TypeTraits::EnableIf<!TypeTraits::IsArray<TString>::value,
                                JsonObjectSubscript<const TString&> >::type
  operator[](const TString& key) {
    return JsonObjectSubscript<const TString&>(*this, key);
  }
  //
  // JsonObjectSubscript operator[](TKey)
  // TKey = const char*, const char[N], const FlashStringHelper*
  template <typename TString>
  JsonObjectSubscript<const TString*> operator[](const TString* key) {
    return JsonObjectSubscript<const TString*>(*this, key);
  }

  // Gets the value associated with the specified key.
  //
  // const JsonObjectSubscript operator[](TKey) const;
  // TKey = const std::string&, const String&
  template <typename TString>
  typename TypeTraits::EnableIf<
      !TypeTraits::IsArray<TString>::value,
      const JsonObjectSubscript<const TString&> >::type
  operator[](const TString& key) const {
    return JsonObjectSubscript<const TString&>(*const_cast<JsonObject*>(this),
                                               key);
  }
  //
  // const JsonObjectSubscript operator[](TKey) const;
  // TKey = const char*, const char[N], const FlashStringHelper*
  template <typename TString>
  const JsonObjectSubscript<const TString*> operator[](
      const TString* key) const {
    return JsonObjectSubscript<const TString*>(*const_cast<JsonObject*>(this),
                                               key);
  }

  // Sets the specified key with the specified value.
  //
  // bool set(TKey, TValue);
  // TKey = const std::string&, const String&
  // TValue = bool, long, int, short, float, double, RawJson, JsonVariant,
  //          const std::string&, const String&,
  //          const JsonArray&, const JsonObject&
  template <typename TValue, typename TString>
  typename TypeTraits::EnableIf<!TypeTraits::IsArray<TString>::value &&
                                    !TypeTraits::IsArray<TValue>::value,
                                bool>::type
  set(const TString& key, const TValue& value) {
    return set_impl<const TString&, const TValue&>(key, value);
  }
  //
  // bool set(TKey, TValue);
  // TKey = const std::string&, const String&
  // TValue = const char*, const char[N], const FlashStringHelper*
  template <typename TValue, typename TString>
  typename TypeTraits::EnableIf<!TypeTraits::IsArray<TString>::value,
                                bool>::type
  set(const TString& key, const TValue* value) {
    return set_impl<const TString&, const TValue*>(key, value);
  }
  //
  // bool set(TKey, TValue);
  // TKey = const char*, const char[N], const FlashStringHelper*
  // TValue = bool, long, int, short, float, double, RawJson, JsonVariant,
  //          const std::string&, const String&,
  //          const JsonArray&, const JsonObject&
  template <typename TValue, typename TString>
  typename TypeTraits::EnableIf<!TypeTraits::IsArray<TValue>::value, bool>::type
  set(const TString* key, const TValue& value) {
    return set_impl<const TString*, const TValue&>(key, value);
  }
  //
  // bool set(TKey, TValue);
  // TKey = const char*, const char[N], const FlashStringHelper*
  // TValue = const char*, const char[N], const FlashStringHelper*
  template <typename TValue, typename TString>
  bool set(const TString* key, const TValue* value) {
    return set_impl<const TString*, const TValue*>(key, value);
  }
  //
  // bool set(TKey, TValue, uint8_t decimals);
  // TKey = const std::string&, const String&
  // TValue = float, double
  template <typename TValue, typename TString>
  typename TypeTraits::EnableIf<TypeTraits::IsFloatingPoint<TValue>::value &&
                                    !TypeTraits::IsArray<TString>::value,
                                bool>::type
  set(const TString& key, TValue value, uint8_t decimals) {
    return set_impl<const TString&, const JsonVariant&>(
        key, JsonVariant(value, decimals));
  }
  //
  // bool set(TKey, TValue, uint8_t decimals);
  // TKey = const char*, const char[N], const FlashStringHelper*
  // TValue = float, double
  template <typename TValue, typename TString>
  typename TypeTraits::EnableIf<TypeTraits::IsFloatingPoint<TValue>::value,
                                bool>::type
  set(const TString* key, TValue value, uint8_t decimals) {
    return set_impl<const TString*, const JsonVariant&>(
        key, JsonVariant(value, decimals));
  }

  // Gets the value associated with the specified key.
  //
  // TValue get<TValue>(TKey);
  // TKey = const std::string&, const String&
  // TValue = bool, char, long, int, short, float, double,
  //          const std::string&, const String&,
  //          const JsonArray&, const JsonObject&
  template <typename TValue, typename TString>
  typename TypeTraits::EnableIf<
      !TypeTraits::IsArray<TString>::value,
      typename Internals::JsonVariantAs<TValue>::type>::type
  get(const TString& key) const {
    return get_impl<const TString&, TValue>(key);
  }
  //
  // TValue get<TValue>(TKey);
  // TKey = const char*, const char[N], const FlashStringHelper*
  // TValue = bool, char, long, int, short, float, double,
  //          const std::string&, const String&,
  //          const JsonArray&, const JsonObject&
  template <typename TValue, typename TString>
  typename Internals::JsonVariantAs<TValue>::type get(
      const TString* key) const {
    return get_impl<const TString*, TValue>(key);
  }

  // Checks the type of the value associated with the specified key.
  //
  //
  // bool is<TValue>(TKey) const;
  // TKey = const std::string&, const String&
  // TValue = bool, char, long, int, short, float, double,
  //          const std::string&, const String&,
  //          const JsonArray&, const JsonObject&
  template <typename TValue, typename TString>
  typename TypeTraits::EnableIf<!TypeTraits::IsArray<TString>::value,
                                bool>::type
  is(const TString& key) const {
    return is_impl<const TString&, TValue>(key);
  }
  //
  // bool is<TValue>(TKey) const;
  // TKey = const char*, const char[N], const FlashStringHelper*
  // TValue = bool, char, long, int, short, float, double,
  //          const std::string&, const String&,
  //          const JsonArray&, const JsonObject&
  template <typename TValue, typename TString>
  bool is(const TString* key) const {
    return is_impl<const TString*, TValue>(key);
  }

  // Creates and adds a JsonArray.
  //
  // JsonArray& createNestedArray(TKey);
  // TKey = const std::string&, const String&
  template <typename TString>
  typename TypeTraits::EnableIf<!TypeTraits::IsArray<TString>::value,
                                JsonArray&>::type
  createNestedArray(const TString& key) {
    return createNestedArray_impl<const TString&>(key);
  }
  // JsonArray& createNestedArray(TKey);
  // TKey = const char*, const char[N], const FlashStringHelper*
  template <typename TString>
  JsonArray& createNestedArray(const TString* key) {
    return createNestedArray_impl<const TString*>(key);
  }

  // Creates and adds a JsonObject.
  //
  // JsonObject& createNestedObject(TKey);
  // TKey = const std::string&, const String&
  template <typename TString>
  typename TypeTraits::EnableIf<!TypeTraits::IsArray<TString>::value,
                                JsonObject&>::type
  createNestedObject(const TString& key) {
    return createNestedObject_impl<const TString&>(key);
  }
  //
  // JsonObject& createNestedObject(TKey);
  // TKey = const char*, const char[N], const FlashStringHelper*
  template <typename TString>
  JsonObject& createNestedObject(const TString* key) {
    return createNestedObject_impl<const TString*>(key);
  }

  // Tells weither the specified key is present and associated with a value.
  //
  // bool containsKey(TKey);
  // TKey = const std::string&, const String&
  template <typename TString>
  typename TypeTraits::EnableIf<!TypeTraits::IsArray<TString>::value,
                                bool>::type
  containsKey(const TString& key) const {
    return findNode<const TString&>(key) != NULL;
  }
  //
  // bool containsKey(TKey);
  // TKey = const char*, const char[N], const FlashStringHelper*
  template <typename TString>
  bool containsKey(const TString* key) const {
    return findNode<const TString*>(key) != NULL;
  }

  // Removes the specified key and the associated value.
  //
  // void remove(TKey);
  // TKey = const std::string&, const String&
  template <typename TString>
  typename TypeTraits::EnableIf<!TypeTraits::IsArray<TString>::value,
                                void>::type
  remove(const TString& key) {
    removeNode(findNode<const TString&>(key));
  }
  //
  // void remove(TKey);
  // TKey = const char*, const char[N], const FlashStringHelper*
  template <typename TString>
  void remove(const TString* key) {
    removeNode(findNode<const TString*>(key));
  }

  // Returns a reference an invalid JsonObject.
  // This object is meant to replace a NULL pointer.
  // This is used when memory allocation or JSON parsing fail.
  static JsonObject& invalid() {
    static JsonObject instance(NULL);
    return instance;
  }

 private:
  // Returns the list node that matches the specified key.
  template <typename TStringRef>
  node_type* findNode(TStringRef key) const {
    for (node_type* node = _firstNode; node; node = node->next) {
      if (Internals::StringTraits<TStringRef>::equals(key, node->content.key))
        return node;
    }
    return NULL;
  }

  template <typename TStringRef, typename TValue>
  typename Internals::JsonVariantAs<TValue>::type get_impl(
      TStringRef key) const {
    node_type* node = findNode<TStringRef>(key);
    return node ? node->content.value.as<TValue>()
                : Internals::JsonVariantDefault<TValue>::get();
  }

  template <typename TStringRef, typename TValueRef>
  bool set_impl(TStringRef key, TValueRef value) {
    node_type* node = findNode<TStringRef>(key);
    if (!node) {
      node = addNewNode();
      if (!node) return false;

      bool key_ok = Internals::ValueSetter<TStringRef>::set(
          _buffer, node->content.key, key);
      if (!key_ok) return false;
    }
    return Internals::ValueSetter<TValueRef>::set(_buffer, node->content.value,
                                                  value);
  }

  template <typename TStringRef, typename TValue>
  bool is_impl(TStringRef key) const {
    node_type* node = findNode<TStringRef>(key);
    return node ? node->content.value.is<TValue>() : false;
  }

  template <typename TStringRef>
  JsonArray& createNestedArray_impl(TStringRef key);

  template <typename TStringRef>
  JsonObject& createNestedObject_impl(TStringRef key);
};

namespace Internals {
template <>
struct JsonVariantDefault<JsonObject> {
  static JsonObject& get() {
    return JsonObject::invalid();
  }
};
}
}
