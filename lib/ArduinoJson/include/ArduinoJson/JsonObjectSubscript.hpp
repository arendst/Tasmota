// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "Configuration.hpp"
#include "JsonVariantBase.hpp"
#include "TypeTraits/EnableIf.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4522)
#endif

namespace ArduinoJson {

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
  // operator=(TValue);
  // TValue = bool, char, long, int, short, float, double,
  //          const std::string&, const String&,
  //          const JsonArray&, const JsonObject&
  template <typename TValue>
  FORCE_INLINE
      typename TypeTraits::EnableIf<!TypeTraits::IsArray<TValue>::value,
                                    this_type&>::type
      operator=(const TValue& src) {
    _object.set(_key, src);
    return *this;
  }
  //
  // operator=(TValue);
  // TValue = const char*, const char[N], const FlashStringHelper*
  template <typename TValue>
  FORCE_INLINE this_type& operator=(const TValue* src) {
    _object.set(_key, src);
    return *this;
  }

  FORCE_INLINE bool success() const {
    return _object.containsKey(_key);
  }

  template <typename TValue>
  FORCE_INLINE typename Internals::JsonVariantAs<TValue>::type as() const {
    return _object.get<TValue>(_key);
  }

  template <typename TValue>
  FORCE_INLINE bool is() const {
    return _object.is<TValue>(_key);
  }

  // Sets the specified value.
  //
  // bool set(TValue);
  // TValue = bool, char, long, int, short, float, double, RawJson, JsonVariant,
  //          const std::string&, const String&,
  //          const JsonArray&, const JsonObject&
  template <typename TValue>
  FORCE_INLINE
      typename TypeTraits::EnableIf<!TypeTraits::IsArray<TValue>::value,
                                    bool>::type
      set(const TValue& value) {
    return _object.set(_key, value);
  }
  //
  // bool set(TValue);
  // TValue = const char*, const char[N], const FlashStringHelper*
  template <typename TValue>
  FORCE_INLINE bool set(const TValue* value) {
    return _object.set(_key, value);
  }
  //
  // bool set(TValue, uint8_t decimals);
  // TValue = float, double
  template <typename TValue>
  FORCE_INLINE bool set(const TValue& value, uint8_t decimals) {
    return _object.set(_key, value, decimals);
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
}  // namespace ArduinoJson

#ifdef _MSC_VER
#pragma warning(pop)
#endif
