// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#define ARDUINOJSON_ENABLE_DEPRECATED 1

#include <ArduinoJson.h>
#include <gtest/gtest.h>

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

TEST(Deprecated, asArray) {
  DynamicJsonBuffer jsonBuffer;
  JsonVariant variant = jsonBuffer.createArray();
  ASSERT_TRUE(variant.asArray().success());
}

TEST(Deprecated, asObject) {
  DynamicJsonBuffer jsonBuffer;
  JsonVariant variant = jsonBuffer.createObject();
  ASSERT_TRUE(variant.asObject().success());
}

TEST(Deprecated, asString) {
  JsonVariant variant = "hello";
  ASSERT_STREQ("hello", variant.asString());
}
