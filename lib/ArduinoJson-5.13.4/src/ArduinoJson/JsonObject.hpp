// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/JsonBufferAllocated.hpp"
#include "Data/List.hpp"
#include "Data/ReferenceType.hpp"
#include "Data/ValueSaver.hpp"
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
namespace Internals {
template <typename>
class JsonObjectSubscript;
}

// A dictionary of JsonVariant indexed by string (char*)
//
// The constructor is private, instances must be created via
// JsonBuffer::createObject() or JsonBuffer::parseObject().
// A JsonObject can be serialized to a JSON string via JsonObject::printTo().
// It can also be deserialized from a JSON string via JsonBuffer::parseObject().
class JsonObject : public Internals::JsonPrintable<JsonObject>,
                   public Internals::ReferenceType,
                   public Internals::NonCopyable,
                   public Internals::List<JsonPair>,
                   public Internals::JsonBufferAllocated {
 public:
  // Create an empty JsonArray attached to the specified JsonBuffer.
  // You should not use this constructor directly.
  // Instead, use JsonBuffer::createObject() or JsonBuffer.parseObject().
  explicit JsonObject(JsonBuffer* buffer) throw()
      : Internals::List<JsonPair>(buffer) {}

  // Gets or sets the value associated with the specified key.
  //
  // JsonObjectSubscript operator[](TKey)
  // TKey = const std::string&, const String&
  template <typename TString>
  Internals::JsonObjectSubscript<const TString&> operator[](
      const TString& key) {
    return Internals::JsonObjectSubscript<const TString&>(*this, key);
  }
  //
  // JsonObjectSubscript operator[](TKey)
  // TKey = char*, const char*, char[], const char[N], const FlashStringHelper*
  template <typename TString>
  Internals::JsonObjectSubscript<TString*> operator[](TString* key) {
    return Internals::JsonObjectSubscript<TString*>(*this, key);
  }

  // Gets the value associated with the specified key.
  //
  // const JsonObjectSubscript operator[](TKey) const;
  // TKey = const std::string&, const String&
  template <typename TString>
  const Internals::JsonObjectSubscript<const TString&> operator[](
      const TString& key) const {
    return Internals::JsonObjectSubscript<const TString&>(
        *const_cast<JsonObject*>(this), key);
  }
  //
  // const JsonObjectSubscript operator[](TKey) const;
  // TKey = const char*, const char[N], const FlashStringHelper*
  template <typename TString>
  const Internals::JsonObjectSubscript<TString*> operator[](
      TString* key) const {
    return Internals::JsonObjectSubscript<TString*>(
        *const_cast<JsonObject*>(this), key);
  }

  // Sets the specified key with the specified value.
  //
  // bool set(TKey, TValue);
  // TKey = const std::string&, const String&
  // TValue = bool, long, int, short, float, double, RawJson, JsonVariant,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TString>
  bool set(const TString& key, const TValue& value) {
    return set_impl<const TString&, const TValue&>(key, value);
  }
  //
  // bool set(TKey, TValue);
  // TKey = const std::string&, const String&
  // TValue = char*, const char*, const FlashStringHelper*
  template <typename TValue, typename TString>
  bool set(const TString& key, TValue* value) {
    return set_impl<const TString&, TValue*>(key, value);
  }
  //
  // bool set(TKey, const TValue&);
  // TKey = char*, const char*, const FlashStringHelper*
  // TValue = bool, long, int, short, float, double, RawJson, JsonVariant,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TString>
  bool set(TString* key, const TValue& value) {
    return set_impl<TString*, const TValue&>(key, value);
  }
  //
  // bool set(TKey, TValue);
  // TKey = char*, const char*, const FlashStringHelper*
  // TValue = char*, const char*, const FlashStringHelper*
  template <typename TValue, typename TString>
  bool set(TString* key, TValue* value) {
    return set_impl<TString*, TValue*>(key, value);
  }
  //
  // bool set(TKey, TValue, uint8_t decimals);
  // TKey = const std::string&, const String&
  // TValue = float, double
  template <typename TValue, typename TString>
  DEPRECATED("Second argument is not supported anymore")
  typename Internals::EnableIf<Internals::IsFloatingPoint<TValue>::value,
                               bool>::type
      set(const TString& key, TValue value, uint8_t) {
    return set_impl<const TString&, const JsonVariant&>(key,
                                                        JsonVariant(value));
  }
  //
  // bool set(TKey, TValue, uint8_t decimals);
  // TKey = char*, const char*, const FlashStringHelper*
  // TValue = float, double
  template <typename TValue, typename TString>
  DEPRECATED("Second argument is not supported anymore")
  typename Internals::EnableIf<Internals::IsFloatingPoint<TValue>::value,
                               bool>::type
      set(TString* key, TValue value, uint8_t) {
    return set_impl<TString*, const JsonVariant&>(key, JsonVariant(value));
  }

  // Gets the value associated with the specified key.
  //
  // TValue get<TValue>(TKey) const;
  // TKey = const std::string&, const String&
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TString>
  typename Internals::JsonVariantAs<TValue>::type get(
      const TString& key) const {
    return get_impl<const TString&, TValue>(key);
  }
  //
  // TValue get<TValue>(TKey) const;
  // TKey = char*, const char*, const FlashStringHelper*
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TString>
  typename Internals::JsonVariantAs<TValue>::type get(TString* key) const {
    return get_impl<TString*, TValue>(key);
  }

  // Checks the type of the value associated with the specified key.
  //
  //
  // bool is<TValue>(TKey) const;
  // TKey = const std::string&, const String&
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TString>
  bool is(const TString& key) const {
    return is_impl<const TString&, TValue>(key);
  }
  //
  // bool is<TValue>(TKey) const;
  // TKey = char*, const char*, const FlashStringHelper*
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue, typename TString>
  bool is(TString* key) const {
    return is_impl<TString*, TValue>(key);
  }

  // Creates and adds a JsonArray.
  //
  // JsonArray& createNestedArray(TKey);
  // TKey = const std::string&, const String&
  template <typename TString>
  JsonArray& createNestedArray(const TString& key) {
    return createNestedArray_impl<const TString&>(key);
  }
  // JsonArray& createNestedArray(TKey);
  // TKey = char*, const char*, char[], const char[], const FlashStringHelper*
  template <typename TString>
  JsonArray& createNestedArray(TString* key) {
    return createNestedArray_impl<TString*>(key);
  }

  // Creates and adds a JsonObject.
  //
  // JsonObject& createNestedObject(TKey);
  // TKey = const std::string&, const String&
  template <typename TString>
  JsonObject& createNestedObject(const TString& key) {
    return createNestedObject_impl<const TString&>(key);
  }
  //
  // JsonObject& createNestedObject(TKey);
  // TKey = char*, const char*, char[], const char[], const FlashStringHelper*
  template <typename TString>
  JsonObject& createNestedObject(TString* key) {
    return createNestedObject_impl<TString*>(key);
  }

  // Tells weither the specified key is present and associated with a value.
  //
  // bool containsKey(TKey);
  // TKey = const std::string&, const String&
  template <typename TString>
  bool containsKey(const TString& key) const {
    return findKey<const TString&>(key) != end();
  }
  //
  // bool containsKey(TKey);
  // TKey = char*, const char*, char[], const char[], const FlashStringHelper*
  template <typename TString>
  bool containsKey(TString* key) const {
    return findKey<TString*>(key) != end();
  }

  // Removes the specified key and the associated value.
  //
  // void remove(TKey);
  // TKey = const std::string&, const String&
  template <typename TString>
  void remove(const TString& key) {
    remove(findKey<const TString&>(key));
  }
  //
  // void remove(TKey);
  // TKey = char*, const char*, char[], const char[], const FlashStringHelper*
  template <typename TString>
  void remove(TString* key) {
    remove(findKey<TString*>(key));
  }
  //
  // void remove(iterator)
  using Internals::List<JsonPair>::remove;

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
  iterator findKey(TStringRef key) {
    iterator it;
    for (it = begin(); it != end(); ++it) {
      if (Internals::StringTraits<TStringRef>::equals(key, it->key)) break;
    }
    return it;
  }
  template <typename TStringRef>
  const_iterator findKey(TStringRef key) const {
    return const_cast<JsonObject*>(this)->findKey<TStringRef>(key);
  }

  template <typename TStringRef, typename TValue>
  typename Internals::JsonVariantAs<TValue>::type get_impl(
      TStringRef key) const {
    const_iterator it = findKey<TStringRef>(key);
    return it != end() ? it->value.as<TValue>()
                       : Internals::JsonVariantDefault<TValue>::get();
  }

  template <typename TStringRef, typename TValueRef>
  bool set_impl(TStringRef key, TValueRef value) {
    // ignore null key
    if (Internals::StringTraits<TStringRef>::is_null(key)) return false;

    // search a matching key
    iterator it = findKey<TStringRef>(key);
    if (it == end()) {
      // add the key
      it = Internals::List<JsonPair>::add();
      if (it == end()) return false;
      bool key_ok =
          Internals::ValueSaver<TStringRef>::save(_buffer, it->key, key);
      if (!key_ok) return false;
    }

    // save the value
    return Internals::ValueSaver<TValueRef>::save(_buffer, it->value, value);
  }

  template <typename TStringRef, typename TValue>
  bool is_impl(TStringRef key) const {
    const_iterator it = findKey<TStringRef>(key);
    return it != end() ? it->value.is<TValue>() : false;
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
}  // namespace Internals
}  // namespace ArduinoJson
