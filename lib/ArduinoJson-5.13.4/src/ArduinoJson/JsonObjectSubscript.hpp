// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Configuration.hpp"
#include "JsonVariantBase.hpp"
#include "TypeTraits/EnableIf.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4522)
#endif

namespace ArduinoJson {
namespace Internals {

template <typename TStringRef>
class JsonObjectSubscript
    : public JsonVariantBase<JsonObjectSubscript<TStringRef> > {
  typedef JsonObjectSubscript<TStringRef> this_type;

 public:
  FORCE_INLINE JsonObjectSubscript(JsonObject& object, TStringRef key)
      : _object(object), _key(key) {}

  FORCE_INLINE this_type& operator=(const this_type& src) {
    _object.set(_key, src);
    return *this;
  }

  // Set the specified value
  //
  // operator=(const TValue&);
  // TValue = bool, char, long, int, short, float, double,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue>
  FORCE_INLINE typename EnableIf<!IsArray<TValue>::value, this_type&>::type
  operator=(const TValue& src) {
    _object.set(_key, src);
    return *this;
  }
  //
  // operator=(TValue);
  // TValue = char*, const char*, const FlashStringHelper*
  template <typename TValue>
  FORCE_INLINE this_type& operator=(TValue* src) {
    _object.set(_key, src);
    return *this;
  }

  FORCE_INLINE bool success() const {
    return _object.containsKey(_key);
  }

  template <typename TValue>
  FORCE_INLINE typename JsonVariantAs<TValue>::type as() const {
    return _object.get<TValue>(_key);
  }

  template <typename TValue>
  FORCE_INLINE bool is() const {
    return _object.is<TValue>(_key);
  }

  // Sets the specified value.
  //
  // bool set(const TValue&);
  // TValue = bool, char, long, int, short, float, double, RawJson, JsonVariant,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue>
  FORCE_INLINE typename EnableIf<!IsArray<TValue>::value, bool>::type set(
      const TValue& value) {
    return _object.set(_key, value);
  }
  //
  // bool set(TValue);
  // TValue = char*, const char, const FlashStringHelper*
  template <typename TValue>
  FORCE_INLINE bool set(const TValue* value) {
    return _object.set(_key, value);
  }
  //
  // bool set(TValue, uint8_t decimals);
  // TValue = float, double
  template <typename TValue>
  DEPRECATED("Second argument is not supported anymore")
  FORCE_INLINE bool set(const TValue& value, uint8_t) {
    return _object.set(_key, value);
  }

 private:
  JsonObject& _object;
  TStringRef _key;
};

#if ARDUINOJSON_ENABLE_STD_STREAM
template <typename TStringRef>
inline std::ostream& operator<<(std::ostream& os,
                                const JsonObjectSubscript<TStringRef>& source) {
  return source.printTo(os);
}
#endif
}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
