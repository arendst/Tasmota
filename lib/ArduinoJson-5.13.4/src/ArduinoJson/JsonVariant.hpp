// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

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
class JsonVariant : public Internals::JsonVariantBase<JsonVariant> {
  template <typename Print>
  friend class Internals::JsonSerializer;

 public:
  // Creates an uninitialized JsonVariant
  JsonVariant() : _type(Internals::JSON_UNDEFINED) {}

  // Create a JsonVariant containing a boolean value.
  // It will be serialized as "true" or "false" in JSON.
  JsonVariant(bool value) {
    using namespace Internals;
    _type = JSON_BOOLEAN;
    _content.asInteger = static_cast<JsonUInt>(value);
  }

  // Create a JsonVariant containing a floating point value.
  // JsonVariant(double value);
  // JsonVariant(float value);
  template <typename T>
  JsonVariant(T value, typename Internals::EnableIf<
                           Internals::IsFloatingPoint<T>::value>::type * = 0) {
    using namespace Internals;
    _type = JSON_FLOAT;
    _content.asFloat = static_cast<JsonFloat>(value);
  }
  template <typename T>
  DEPRECATED("Second argument is not supported anymore")
  JsonVariant(T value, uint8_t,
              typename Internals::EnableIf<
                  Internals::IsFloatingPoint<T>::value>::type * = 0) {
    using namespace Internals;
    _type = JSON_FLOAT;
    _content.asFloat = static_cast<JsonFloat>(value);
  }

  // Create a JsonVariant containing an integer value.
  // JsonVariant(char)
  // JsonVariant(signed short)
  // JsonVariant(signed int)
  // JsonVariant(signed long)
  // JsonVariant(signed char)
  template <typename T>
  JsonVariant(
      T value,
      typename Internals::EnableIf<Internals::IsSignedIntegral<T>::value ||
                                   Internals::IsSame<T, char>::value>::type * =
          0) {
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
              typename Internals::EnableIf<
                  Internals::IsUnsignedIntegral<T>::value>::type * = 0) {
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
      typename Internals::EnableIf<Internals::IsChar<TChar>::value>::type * =
          0) {
    _type = Internals::JSON_STRING;
    _content.asString = reinterpret_cast<const char *>(value);
  }

  // Create a JsonVariant containing an unparsed string
  JsonVariant(Internals::RawJsonString<const char *> value) {
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
  // char as<char>() const;
  // signed char as<signed char>() const;
  // signed short as<signed short>() const;
  // signed int as<signed int>() const;
  // signed long as<signed long>() const;
  // unsigned char as<unsigned char>() const;
  // unsigned short as<unsigned short>() const;
  // unsigned int as<unsigned int>() const;
  // unsigned long as<unsigned long>() const;
  template <typename T>
  const typename Internals::EnableIf<Internals::IsIntegral<T>::value, T>::type
  as() const {
    return variantAsInteger<T>();
  }
  // bool as<bool>() const
  template <typename T>
  const typename Internals::EnableIf<Internals::IsSame<T, bool>::value, T>::type
  as() const {
    return variantAsInteger<int>() != 0;
  }
  //
  // double as<double>() const;
  // float as<float>() const;
  template <typename T>
  const typename Internals::EnableIf<Internals::IsFloatingPoint<T>::value,
                                     T>::type
  as() const {
    return variantAsFloat<T>();
  }
  //
  // const char* as<const char*>() const;
  // const char* as<char*>() const;
  template <typename T>
  typename Internals::EnableIf<Internals::IsSame<T, const char *>::value ||
                                   Internals::IsSame<T, char *>::value,
                               const char *>::type
  as() const {
    return variantAsString();
  }
  //
  // std::string as<std::string>() const;
  // String as<String>() const;
  template <typename T>
  typename Internals::EnableIf<Internals::StringTraits<T>::has_append, T>::type
  as() const {
    const char *cstr = variantAsString();
    if (cstr) return T(cstr);
    T s;
    printTo(s);
    return s;
  }
  //
  // JsonArray& as<JsonArray> const;
  // JsonArray& as<JsonArray&> const;
  template <typename T>
  typename Internals::EnableIf<
      Internals::IsSame<typename Internals::RemoveReference<T>::type,
                        JsonArray>::value,
      JsonArray &>::type
  as() const {
    return variantAsArray();
  }
  //
  // const JsonArray& as<const JsonArray&> const;
  template <typename T>
  typename Internals::EnableIf<
      Internals::IsSame<typename Internals::RemoveReference<T>::type,
                        const JsonArray>::value,
      const JsonArray &>::type
  as() const {
    return variantAsArray();
  }
  //
  // JsonObject& as<JsonObject> const;
  // JsonObject& as<JsonObject&> const;
  template <typename T>
  typename Internals::EnableIf<
      Internals::IsSame<typename Internals::RemoveReference<T>::type,
                        JsonObject>::value,
      JsonObject &>::type
  as() const {
    return variantAsObject();
  }
  //
  // JsonObject& as<const JsonObject> const;
  // JsonObject& as<const JsonObject&> const;
  template <typename T>
  typename Internals::EnableIf<
      Internals::IsSame<typename Internals::RemoveReference<T>::type,
                        const JsonObject>::value,
      const JsonObject &>::type
  as() const {
    return variantAsObject();
  }
  //
  // JsonVariant as<JsonVariant> const;
  template <typename T>
  typename Internals::EnableIf<Internals::IsSame<T, JsonVariant>::value,
                               T>::type
  as() const {
    return *this;
  }

  // Tells weither the variant has the specified type.
  // Returns true if the variant has type type T, false otherwise.
  //
  // bool is<char>() const;
  // bool is<signed char>() const;
  // bool is<signed short>() const;
  // bool is<signed int>() const;
  // bool is<signed long>() const;
  // bool is<unsigned char>() const;
  // bool is<unsigned short>() const;
  // bool is<unsigned int>() const;
  // bool is<unsigned long>() const;
  template <typename T>
  typename Internals::EnableIf<Internals::IsIntegral<T>::value, bool>::type is()
      const {
    return variantIsInteger();
  }
  //
  // bool is<double>() const;
  // bool is<float>() const;
  template <typename T>
  typename Internals::EnableIf<Internals::IsFloatingPoint<T>::value, bool>::type
  is() const {
    return variantIsFloat();
  }
  //
  // bool is<bool>() const
  template <typename T>
  typename Internals::EnableIf<Internals::IsSame<T, bool>::value, bool>::type
  is() const {
    return variantIsBoolean();
  }
  //
  // bool is<const char*>() const;
  // bool is<char*>() const;
  // bool is<std::string>() const;
  template <typename T>
  typename Internals::EnableIf<Internals::IsSame<T, const char *>::value ||
                                   Internals::IsSame<T, char *>::value ||
                                   Internals::StringTraits<T>::has_append,
                               bool>::type
  is() const {
    return variantIsString();
  }
  //
  // bool is<JsonArray> const;
  // bool is<JsonArray&> const;
  // bool is<const JsonArray&> const;
  template <typename T>
  typename Internals::EnableIf<
      Internals::IsSame<typename Internals::RemoveConst<
                            typename Internals::RemoveReference<T>::type>::type,
                        JsonArray>::value,
      bool>::type
  is() const {
    return variantIsArray();
  }
  //
  // bool is<JsonObject> const;
  // bool is<JsonObject&> const;
  // bool is<const JsonObject&> const;
  template <typename T>
  typename Internals::EnableIf<
      Internals::IsSame<typename Internals::RemoveConst<
                            typename Internals::RemoveReference<T>::type>::type,
                        JsonObject>::value,
      bool>::type
  is() const {
    return variantIsObject();
  }

  // Returns true if the variant has a value
  bool success() const {
    return _type != Internals::JSON_UNDEFINED;
  }

 private:
  JsonArray &variantAsArray() const;
  JsonObject &variantAsObject() const;
  const char *variantAsString() const;
  template <typename T>
  T variantAsFloat() const;
  template <typename T>
  T variantAsInteger() const;
  bool variantIsBoolean() const;
  bool variantIsFloat() const;
  bool variantIsInteger() const;
  bool variantIsArray() const {
    return _type == Internals::JSON_ARRAY;
  }
  bool variantIsObject() const {
    return _type == Internals::JSON_OBJECT;
  }
  bool variantIsString() const {
    return _type == Internals::JSON_STRING ||
           (_type == Internals::JSON_UNPARSED && _content.asString &&
            !strcmp("null", _content.asString));
  }

  // The current type of the variant
  Internals::JsonVariantType _type;

  // The various alternatives for the value of the variant.
  Internals::JsonVariantContent _content;
};

DEPRECATED("Decimal places are ignored, use the float value instead")
inline JsonVariant float_with_n_digits(float value, uint8_t) {
  return JsonVariant(value);
}

DEPRECATED("Decimal places are ignored, use the double value instead")
inline JsonVariant double_with_n_digits(double value, uint8_t) {
  return JsonVariant(value);
}
}  // namespace ArduinoJson
