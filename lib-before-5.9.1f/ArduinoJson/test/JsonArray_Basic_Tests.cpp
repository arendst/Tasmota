// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

#define TEST_(name) TEST(JsonArray_Basic_Tests, name)

TEST_(SuccessIsTrue) {
  DynamicJsonBuffer _jsonBuffer;
  JsonArray& array = _jsonBuffer.createArray();

  EXPECT_TRUE(array.success());
}

TEST_(InitialSizeIsZero) {
  DynamicJsonBuffer _jsonBuffer;
  JsonArray& array = _jsonBuffer.createArray();

  EXPECT_EQ(0U, array.size());
}

TEST_(CreateNestedArray) {
  DynamicJsonBuffer _jsonBuffer;
  JsonArray& array = _jsonBuffer.createArray();

  JsonArray& arr = array.createNestedArray();
  EXPECT_EQ(&arr, &array[0].as<JsonArray&>());
}

TEST_(CreateNestedObject) {
  DynamicJsonBuffer _jsonBuffer;
  JsonArray& array = _jsonBuffer.createArray();

  JsonObject& obj = array.createNestedObject();
  EXPECT_EQ(&obj, &array[0].as<JsonObject&>());
}
