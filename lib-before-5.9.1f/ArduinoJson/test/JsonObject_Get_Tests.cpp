// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class JsonObject_Get_Tests : public ::testing::Test {
 public:
  JsonObject_Get_Tests() : _object(_jsonBuffer.createObject()) {}

 protected:
  DynamicJsonBuffer _jsonBuffer;
  JsonObject& _object;
};

#define TEST_(name) TEST_F(JsonObject_Get_Tests, name)

TEST_(GetConstCharPointer_GivenStringLiteral) {
  _object.set("hello", "world");
  const char* value = _object.get<const char*>("hello");
  EXPECT_STREQ("world", value);
}
