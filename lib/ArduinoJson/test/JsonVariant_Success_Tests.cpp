// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

TEST(JsonVariant_Success_Tests, ReturnsFalse_WhenUndefined) {
  JsonVariant variant;
  EXPECT_FALSE(variant.success());
}

TEST(JsonVariant_Success_Tests, ReturnsTrue_WhenInteger) {
  JsonVariant variant = 0;
  EXPECT_TRUE(variant.success());
}

TEST(JsonVariant_Success_Tests, ReturnsTrue_WhenEmptyArray) {
  DynamicJsonBuffer jsonBuffer;

  JsonVariant variant = jsonBuffer.createArray();
  EXPECT_TRUE(variant.success());
}

TEST(JsonVariant_Success_Tests, ReturnsTrue_WhenEmptyObject) {
  DynamicJsonBuffer jsonBuffer;

  JsonVariant variant = jsonBuffer.createObject();
  EXPECT_TRUE(variant.success());
}

TEST(JsonVariant_Success_Tests, ReturnsFalse_WhenInvalidArray) {
  JsonVariant variant = JsonArray::invalid();
  EXPECT_FALSE(variant.success());
}

TEST(JsonVariant_Success_Tests, ReturnsFalse_WhenInvalidObject) {
  JsonVariant variant = JsonObject::invalid();
  EXPECT_FALSE(variant.success());
}
