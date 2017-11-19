// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>
#include <sstream>

using namespace ArduinoJson::TypeTraits;

TEST(TypeTraits, IsBaseOf) {
  ASSERT_FALSE((IsBaseOf<std::istream, std::ostringstream>::value));
  ASSERT_TRUE((IsBaseOf<std::istream, std::istringstream>::value));
  ASSERT_TRUE((IsBaseOf<JsonVariantBase<JsonObjectSubscript<const char*> >,
                        JsonObjectSubscript<const char*> >::value));
}

TEST(TypeTraits, IsArray) {
  ASSERT_FALSE((IsArray<const char*>::value));
  ASSERT_TRUE((IsArray<const char[]>::value));
  ASSERT_TRUE((IsArray<const char[10]>::value));
}

TEST(TypeTraits, IsVariant) {
  ASSERT_TRUE((IsVariant<JsonObjectSubscript<const char*> >::value));
  ASSERT_TRUE((IsVariant<JsonVariant>::value));
}

TEST(TypeTraits, IsString) {
  ASSERT_TRUE((IsString<const char*>::value));
  ASSERT_TRUE((IsString<std::string>::value));
  ASSERT_FALSE((IsString<double>::value));
}
