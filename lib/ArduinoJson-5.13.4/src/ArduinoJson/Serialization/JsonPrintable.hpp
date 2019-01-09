// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Configuration.hpp"
#include "../TypeTraits/EnableIf.hpp"
#include "DummyPrint.hpp"
#include "DynamicStringBuilder.hpp"
#include "IndentedPrint.hpp"
#include "JsonSerializer.hpp"
#include "JsonWriter.hpp"
#include "Prettyfier.hpp"
#include "StaticStringBuilder.hpp"

#if ARDUINOJSON_ENABLE_STD_STREAM
#include "StreamPrintAdapter.hpp"
#endif

namespace ArduinoJson {
namespace Internals {

// Implements all the overloads of printTo() and prettyPrintTo()
// Caution: this class use a template parameter to avoid virtual methods.
// This is a bit curious but allows to reduce the size of JsonVariant, JsonArray
// and JsonObject.
template <typename T>
class JsonPrintable {
 public:
  template <typename Print>
  typename EnableIf<!StringTraits<Print>::has_append, size_t>::type printTo(
      Print &print) const {
    JsonWriter<Print> writer(print);
    JsonSerializer<JsonWriter<Print> >::serialize(downcast(), writer);
    return writer.bytesWritten();
  }

#if ARDUINOJSON_ENABLE_STD_STREAM
  std::ostream &printTo(std::ostream &os) const {
    StreamPrintAdapter adapter(os);
    printTo(adapter);
    return os;
  }
#endif

  size_t printTo(char *buffer, size_t bufferSize) const {
    StaticStringBuilder sb(buffer, bufferSize);
    return printTo(sb);
  }

  template <size_t N>
  size_t printTo(char (&buffer)[N]) const {
    return printTo(buffer, N);
  }

  template <typename TString>
  typename EnableIf<StringTraits<TString>::has_append, size_t>::type printTo(
      TString &str) const {
    DynamicStringBuilder<TString> sb(str);
    return printTo(sb);
  }

  template <typename Print>
  size_t prettyPrintTo(IndentedPrint<Print> &print) const {
    Prettyfier<Print> p(print);
    return printTo(p);
  }

  size_t prettyPrintTo(char *buffer, size_t bufferSize) const {
    StaticStringBuilder sb(buffer, bufferSize);
    return prettyPrintTo(sb);
  }

  template <size_t N>
  size_t prettyPrintTo(char (&buffer)[N]) const {
    return prettyPrintTo(buffer, N);
  }

  template <typename Print>
  typename EnableIf<!StringTraits<Print>::has_append, size_t>::type
  prettyPrintTo(Print &print) const {
    IndentedPrint<Print> indentedPrint(print);
    return prettyPrintTo(indentedPrint);
  }

  template <typename TString>
  typename EnableIf<StringTraits<TString>::has_append, size_t>::type
  prettyPrintTo(TString &str) const {
    DynamicStringBuilder<TString> sb(str);
    return prettyPrintTo(sb);
  }

  size_t measureLength() const {
    DummyPrint dp;
    return printTo(dp);
  }

  size_t measurePrettyLength() const {
    DummyPrint dp;
    return prettyPrintTo(dp);
  }

 private:
  const T &downcast() const {
    return *static_cast<const T *>(this);
  }
};

#if ARDUINOJSON_ENABLE_STD_STREAM
template <typename T>
inline std::ostream &operator<<(std::ostream &os, const JsonPrintable<T> &v) {
  return v.printTo(os);
}
#endif
}
}
