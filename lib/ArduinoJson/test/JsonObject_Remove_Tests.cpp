// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

#define TEST_(name) TEST(JsonObject_Remove_Tests, name)

TEST_(SizeDecreased_WhenValuesAreRemoved) {
  DynamicJsonBuffer _jsonBuffer;
  JsonObject& _object = _jsonBuffer.createObject();
  _object["hello"] = 1;

  _object.remove("hello");

  EXPECT_EQ(0, _object.size());
}

TEST_(SizeUntouched_WhenRemoveIsCalledWithAWrongKey) {
  DynamicJsonBuffer _jsonBuffer;
  JsonObject& _object = _jsonBuffer.createObject();
  _object["hello"] = 1;

  _object.remove("world");

  EXPECT_EQ(1, _object.size());
}
