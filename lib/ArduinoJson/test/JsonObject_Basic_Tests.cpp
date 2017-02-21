// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

#define TEST_(name) TEST(JsonObject_Basic_Tests, name)

TEST_(InitialSizeIsZero) {
  DynamicJsonBuffer _jsonBuffer;
  JsonObject& _object = _jsonBuffer.createObject();

  EXPECT_EQ(0, _object.size());
}

TEST_(SuccessIsTrue) {
  DynamicJsonBuffer _jsonBuffer;
  JsonObject& _object = _jsonBuffer.createObject();

  EXPECT_TRUE(_object.success());
}
