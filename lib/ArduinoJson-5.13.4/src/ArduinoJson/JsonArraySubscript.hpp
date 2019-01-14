// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Configuration.hpp"
#include "JsonVariantBase.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4522)
#endif

namespace ArduinoJson {
namespace Internals {
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
  // operator=(const TValue&)
  // TValue = bool, long, int, short, float, double, RawJson, JsonVariant,
  //          std::string, String, JsonArray, JsonObject
  template <typename T>
  FORCE_INLINE JsonArraySubscript& operator=(const T& src) {
    _array.set(_index, src);
    return *this;
  }
  //
  // operator=(TValue)
  // TValue = char*, const char*, const FlashStringHelper*
  template <typename T>
  FORCE_INLINE JsonArraySubscript& operator=(T* src) {
    _array.set(_index, src);
    return *this;
  }

  FORCE_INLINE bool success() const {
    return _index < _array.size();
  }

  template <typename T>
  FORCE_INLINE typename JsonVariantAs<T>::type as() const {
    return _array.get<T>(_index);
  }

  template <typename T>
  FORCE_INLINE bool is() const {
    return _array.is<T>(_index);
  }

  // Replaces the value
  //
  // bool set(const TValue&)
  // TValue = bool, long, int, short, float, double, RawJson, JsonVariant,
  //          std::string, String, JsonArray, JsonObject
  template <typename TValue>
  FORCE_INLINE bool set(const TValue& value) {
    return _array.set(_index, value);
  }
  //
  // bool set(TValue)
  // TValue = char*, const char*, const FlashStringHelper*
  template <typename TValue>
  FORCE_INLINE bool set(TValue* value) {
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

#if ARDUINOJSON_ENABLE_STD_STREAM
inline std::ostream& operator<<(std::ostream& os,
                                const JsonArraySubscript& source) {
  return source.printTo(os);
}
#endif
}

inline Internals::JsonArraySubscript JsonArray::operator[](size_t index) {
  return Internals::JsonArraySubscript(*this, index);
}

inline const Internals::JsonArraySubscript JsonArray::operator[](
    size_t index) const {
  return Internals::JsonArraySubscript(*const_cast<JsonArray*>(this), index);
}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
