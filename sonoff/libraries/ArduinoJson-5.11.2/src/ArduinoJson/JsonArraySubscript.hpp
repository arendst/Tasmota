// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "Configuration.hpp"
#include "JsonVariantBase.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4522)
#endif

namespace ArduinoJson {
class JsonArraySubscript : public JsonVariantBase<JsonArraySubscript> {
 public:
  FORCE_INLINE JsonArraySubscript(JsonArray& array, size_t index)
      : _array(array), _index(index) {}

  FORCE_INLINE JsonArraySubscript& operator=(const JsonArraySubscript& src) {
    _array.set(_index, src);
    return *this;
  }

  // Replaces the value
  //
  // operator=(TValue)
  // TValue = bool, long, int, short, float, double, RawJson, JsonVariant,
  //          const std::string&, const String&,
  //          const JsonArray&, const JsonObject&
  template <typename T>
  FORCE_INLINE JsonArraySubscript& operator=(const T& src) {
    _array.set(_index, src);
    return *this;
  }
  //
  // operator=(TValue)
  // TValue = const char*, const char[N], const FlashStringHelper*
  template <typename T>
  FORCE_INLINE JsonArraySubscript& operator=(const T* src) {
    _array.set(_index, src);
    return *this;
  }

  FORCE_INLINE bool success() const {
    return _index < _array.size();
  }

  template <typename T>
  FORCE_INLINE typename Internals::JsonVariantAs<T>::type as() const {
    return _array.get<T>(_index);
  }

  template <typename T>
  FORCE_INLINE bool is() const {
    return _array.is<T>(_index);
  }

  // Replaces the value
  //
  // bool set(TValue)
  // TValue = bool, long, int, short, float, double, RawJson, JsonVariant,
  //          const std::string&, const String&,
  //          const JsonArray&, const JsonObject&
  template <typename TValue>
  FORCE_INLINE bool set(const TValue& value) {
    return _array.set(_index, value);
  }
  //
  // bool set(TValue)
  // TValue = const char*, const char[N], const FlashStringHelper*
  template <typename TValue>
  FORCE_INLINE bool set(const TValue* value) {
    return _array.set(_index, value);
  }
  //
  // bool set(TValue, uint8_t decimals);
  // TValue = float, double
  template <typename TValue>
  DEPRECATED("Second argument is not supported anymore")
  FORCE_INLINE bool set(const TValue& value, uint8_t) {
    return _array.set(_index, value);
  }

 private:
  JsonArray& _array;
  const size_t _index;
};

#if ARDUINOJSON_ENABLE_STD_STREAM
inline std::ostream& operator<<(std::ostream& os,
                                const JsonArraySubscript& source) {
  return source.printTo(os);
}
#endif

inline JsonArraySubscript JsonArray::operator[](size_t index) {
  return JsonArraySubscript(*this, index);
}

inline const JsonArraySubscript JsonArray::operator[](size_t index) const {
  return JsonArraySubscript(*const_cast<JsonArray*>(this), index);
}

template <typename TImpl>
inline JsonArraySubscript JsonVariantSubscripts<TImpl>::operator[](
    size_t index) {
  return impl()->template as<JsonArray>()[index];
}

template <typename TImpl>
inline const JsonArraySubscript JsonVariantSubscripts<TImpl>::operator[](
    size_t index) const {
  return impl()->template as<JsonArray>()[index];
}

}  // namespace ArduinoJson

#ifdef _MSC_VER
#pragma warning(pop)
#endif
