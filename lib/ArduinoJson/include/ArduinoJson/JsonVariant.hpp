// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include <stddef.h>
#include <stdint.h>  // for uint8_t

#include "Data/JsonVariantContent.hpp"
#include "Data/JsonVariantDefault.hpp"
#include "Data/JsonVariantType.hpp"
#include "JsonVariantBase.hpp"
#include "RawJson.hpp"
#include "Serialization/JsonPrintable.hpp"
#include "TypeTraits/EnableIf.hpp"
#include "TypeTraits/IsChar.hpp"
#include "TypeTraits/IsFloatingPoint.hpp"
#include "TypeTraits/IsIntegral.hpp"
#include "TypeTraits/IsSame.hpp"
#include "TypeTraits/IsSignedIntegral.hpp"
#include "TypeTraits/IsUnsignedIntegral.hpp"
#include "TypeTraits/RemoveConst.hpp"
#include "TypeTraits/RemoveReference.hpp"

namespace ArduinoJson {

// Forward declarations.
class JsonArray;
class JsonObject;

// A variant that can be a any value serializable to a JSON value.
//
// It can be set to:
// - a boolean
// - a char, short, int or a long (signed or unsigned)
// - a string (const char*)
// - a reference to a JsonArray or JsonObject
class JsonVariant : public JsonVariantBase<JsonVariant> {
  friend void Internals::JsonSerializer::serialize(const JsonVariant &,
                                                   JsonWriter &);

 public:
  // Creates an uninitialized JsonVariant
  JsonVariant() : _type(Internals::JSON_UNDEFINED) {}

  // Create a JsonVariant containing a boolean value.
  // It will be serialized as "true" or "false" in JSON.
  JsonVariant(bool value) {
    using namespace Internals;
    _type = JSON_BOOLEAN;
    _content.asInteger = static_cast<JsonInteger>(value);
  }

  // Create a JsonVariant containing a floating point value.
  // The second argument specifies the number of decimal digits to write in
  // the JSON string.
  // JsonVariant(double value, uint8_t decimals);
  // JsonVariant(float value, uint8_t decimals);
  template <typename T>
  JsonVariant(T value, uint8_t decimals = 2,
              typename TypeTraits::EnableIf<
                  TypeTraits::IsFloatingPoint<T>::value>::type * = 0) {
    using namespace Internals;
    _type = static_cast<JsonVariantType>(JSON_FLOAT_0_DECIMALS + decimals);
    _content.asFloat = static_cast<JsonFloat>(value);
  }

  // Create a JsonVariant containing an integer value.
  // JsonVariant(signed short)
  // JsonVariant(signed int)
  // JsonVariant(signed long)
  template <typename T>
  JsonVariant(T value,
              typename TypeTraits::EnableIf<
                  TypeTraits::IsSignedIntegral<T>::value>::type * = 0) {
    using namespace Internals;
    if (value >= 0) {
      _type = JSON_POSITIVE_INTEGER;
      _content.asInteger = static_cast<JsonUInt>(value);
    } else {
      _type = JSON_NEGATIVE_INTEGER;
      _content.asInteger = static_cast<JsonUInt>(-value);
    }
  }
  // JsonVariant(unsigned short)
  // JsonVariant(unsigned int)
  // JsonVariant(unsigned long)
  template <typename T>
  JsonVariant(T value,
              typename TypeTraits::EnableIf<
                  TypeTraits::IsUnsignedIntegral<T>::value>::type * = 0) {
    using namespace Internals;
    _type = JSON_POSITIVE_INTEGER;
    _content.asInteger = static_cast<JsonUInt>(value);
  }

  // Create a JsonVariant containing a string.
  // JsonVariant(const char*);
  // JsonVariant(const signed char*);
  // JsonVariant(const unsigned char*);
  template <typename TChar>
  JsonVariant(
      const TChar *value,
      typename TypeTraits::EnableIf<TypeTraits::IsChar<TChar>::value>::type * =
          0) {
    _type = Internals::JSON_STRING;
    _content.asString = reinterpret_cast<const char *>(value);
  }

  // Create a JsonVariant containing an unparsed string
  JsonVariant(RawJson value) {
    _type = Internals::JSON_UNPARSED;
    _content.asString = value;
  }

  // Create a JsonVariant containing a reference to an array.
  // CAUTION: we are lying about constness, because the array can be modified if
  // the variant is converted back to a JsonArray&
  JsonVariant(const JsonArray &array);

  // Create a JsonVariant containing a reference to an object.
  // CAUTION: we are lying about constness, because the object can be modified
  // if the variant is converted back to a JsonObject&
  JsonVariant(const JsonObject &object);

  // Get the variant as the specified type.
  //
  // short as<signed short>() const;
  // int as<signed int>() const;
  // long as<signed long>() const;
  template <typename T>
  const typename TypeTraits::EnableIf<TypeTraits::IsSignedIntegral<T>::value,
                                      T>::type
  as() const {
    return static_cast<T>(variantAsInteger());
  }
  //
  // short as<unsigned short>() const;
  // int as<unsigned int>() const;
  // long as<unsigned long>() const;
  template <typename T>
  const typename TypeTraits::EnableIf<TypeTraits::IsUnsignedIntegral<T>::value,
                                      T>::type
  as() const {
    return static_cast<T>(asUnsignedInteger());
  }
  //
  // double as<double>() const;
  // float as<float>() const;
  template <typename T>
  const typename TypeTraits::EnableIf<TypeTraits::IsFloatingPoint<T>::value,
                                      T>::type
  as() const {
    return static_cast<T>(variantAsFloat());
  }
  //
  // const char* as<const char*>() const;
  // const char* as<char*>() const;
  template <typename T>
  typename TypeTraits::EnableIf<TypeTraits::IsSame<T, const char *>::value ||
                                    TypeTraits::IsSame<T, char *>::value,
                                const char *>::type
  as() const {
    return variantAsString();
  }
  //
  // std::string as<std::string>() const;
  // String as<String>() const;
  template <typename T>
  typename TypeTraits::EnableIf<Internals::StringTraits<T>::has_append, T>::type
  as() const {
    const char *cstr = variantAsString();
    if (cstr) return T(cstr);
    T s;
    printTo(s);
    return s;
  }
  //
  // const bool as<bool>() const
  template <typename T>
  const typename TypeTraits::EnableIf<TypeTraits::IsSame<T, bool>::value,
                                      T>::type
  as() const {
    return variantAsInteger() != 0;
  }
  //
  // JsonArray& as<JsonArray> const;
  // JsonArray& as<JsonArray&> const;
  template <typename T>
  typename TypeTraits::EnableIf<
      TypeTraits::IsSame<typename TypeTraits::RemoveReference<T>::type,
                         JsonArray>::value,
      JsonArray &>::type
  as() const {
    return variantAsArray();
  }
  //
  // const JsonArray& as<const JsonArray&> const;
  template <typename T>
  typename TypeTraits::EnableIf<
      TypeTraits::IsSame<typename TypeTraits::RemoveReference<T>::type,
                         const JsonArray>::value,
      const JsonArray &>::type
  as() const {
    return variantAsArray();
  }
  //
  // JsonObject& as<JsonObject> const;
  // JsonObject& as<JsonObject&> const;
  template <typename T>
  typename TypeTraits::EnableIf<
      TypeTraits::IsSame<typename TypeTraits::RemoveReference<T>::type,
                         JsonObject>::value,
      JsonObject &>::type
  as() const {
    return variantAsObject();
  }
  //
  // JsonObject& as<const JsonObject> const;
  // JsonObject& as<const JsonObject&> const;
  template <typename T>
  typename TypeTraits::EnableIf<
      TypeTraits::IsSame<typename TypeTraits::RemoveReference<T>::type,
                         const JsonObject>::value,
      const JsonObject &>::type
  as() const {
    return variantAsObject();
  }
  //
  // JsonVariant as<JsonVariant> const;
  template <typename T>
  typename TypeTraits::EnableIf<TypeTraits::IsSame<T, JsonVariant>::value,
                                T>::type
  as() const {
    return *this;
  }

  // Tells weither the variant has the specified type.
  // Returns true if the variant has type type T, false otherwise.
  //
  // short as<short>() const;
  // int as<int>() const;
  // long as<long>() const;
  template <typename T>
  const typename TypeTraits::EnableIf<TypeTraits::IsIntegral<T>::value &&
                                          !TypeTraits::IsSame<T, bool>::value,
                                      bool>::type
  is() const {
    return isInteger();
  }
  //
  // double is<double>() const;
  // float is<float>() const;
  template <typename T>
  const typename TypeTraits::EnableIf<TypeTraits::IsFloatingPoint<T>::value,
                                      bool>::type
  is() const {
    return isFloat();
  }
  //
  // const bool is<bool>() const
  template <typename T>
  const typename TypeTraits::EnableIf<TypeTraits::IsSame<T, bool>::value,
                                      bool>::type
  is() const {
    return isBoolean();
  }
  //
  // bool is<const char*>() const;
  // bool is<char*>() const;
  template <typename T>
  typename TypeTraits::EnableIf<TypeTraits::IsSame<T, const char *>::value ||
                                    TypeTraits::IsSame<T, char *>::value,
                                bool>::type
  is() const {
    return isString();
  }
  //
  // bool is<JsonArray> const;
  // bool is<JsonArray&> const;
  // bool is<const JsonArray&> const;
  template <typename T>
  typename TypeTraits::EnableIf<
      TypeTraits::IsSame<
          typename TypeTraits::RemoveConst<
              typename TypeTraits::RemoveReference<T>::type>::type,
          JsonArray>::value,
      bool>::type
  is() const {
    return isArray();
  }
  //
  // bool is<JsonObject> const;
  // bool is<JsonObject&> const;
  // bool is<const JsonObject&> const;
  template <typename T>
  typename TypeTraits::EnableIf<
      TypeTraits::IsSame<
          typename TypeTraits::RemoveConst<
              typename TypeTraits::RemoveReference<T>::type>::type,
          JsonObject>::value,
      bool>::type
  is() const {
    return isObject();
  }

  // Returns true if the variant has a value
  bool success() const {
    return _type != Internals::JSON_UNDEFINED;
  }

 private:
  // It's not allowed to store a char
  template <typename T>
  JsonVariant(T value, typename TypeTraits::EnableIf<
                           TypeTraits::IsSame<T, char>::value>::type * = 0);

  JsonArray &variantAsArray() const;
  JsonObject &variantAsObject() const;
  const char *variantAsString() const;
  Internals::JsonFloat variantAsFloat() const;
  Internals::JsonInteger variantAsInteger() const;
  Internals::JsonUInt asUnsignedInteger() const;
  bool isBoolean() const;
  bool isFloat() const;
  bool isInteger() const;
  bool isArray() const {
    return _type == Internals::JSON_ARRAY;
  }
  bool isObject() const {
    return _type == Internals::JSON_OBJECT;
  }
  bool isString() const {
    return _type == Internals::JSON_STRING ||
           (_type == Internals::JSON_UNPARSED && _content.asString &&
            !strcmp("null", _content.asString));
  }

  // The current type of the variant
  Internals::JsonVariantType _type;

  // The various alternatives for the value of the variant.
  Internals::JsonVariantContent _content;
};

inline JsonVariant float_with_n_digits(float value, uint8_t digits) {
  return JsonVariant(value, digits);
}

inline JsonVariant double_with_n_digits(double value, uint8_t digits) {
  return JsonVariant(value, digits);
}
}
