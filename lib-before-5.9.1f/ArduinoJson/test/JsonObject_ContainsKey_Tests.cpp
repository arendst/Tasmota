// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

#define TEST_(name) TEST(JsonObject_Basic_Tests, name)

TEST_(ContainsKeyReturnsFalseForNonExistingKey) {
  DynamicJsonBuffer _jsonBuffer;
  JsonObject& _object = _jsonBuffer.createObject();

  _object.set("hello", 42);

  EXPECT_FALSE(_object.containsKey("world"));
}

TEST_(ContainsKeyReturnsTrueForDefinedValue) {
  DynamicJsonBuffer _jsonBuffer;
  JsonObject& _object = _jsonBuffer.createObject();

  _object.set("hello", 42);

  EXPECT_TRUE(_object.containsKey("hello"));
}

TEST_(ContainsKeyReturnsFalseAfterRemove) {
  DynamicJsonBuffer _jsonBuffer;
  JsonObject& _object = _jsonBuffer.createObject();

  _object.set("hello", 42);
  _object.remove("hello");

  EXPECT_FALSE(_object.containsKey("hello"));
}
