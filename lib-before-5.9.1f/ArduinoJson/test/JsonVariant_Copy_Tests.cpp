// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class JsonVariant_Copy_Tests : public ::testing::Test {
 protected:
  DynamicJsonBuffer _jsonBuffer;
  JsonVariant _variant1;
  JsonVariant _variant2;
};

TEST_F(JsonVariant_Copy_Tests, IntegersAreCopiedByValue) {
  _variant1 = 123;
  _variant2 = _variant1;
  _variant1 = 456;

  EXPECT_EQ(123, _variant2.as<int>());
}

TEST_F(JsonVariant_Copy_Tests, DoublesAreCopiedByValue) {
  _variant1 = 123.45;
  _variant2 = _variant1;
  _variant1 = 456.78;

  EXPECT_EQ(123.45, _variant2.as<double>());
}

TEST_F(JsonVariant_Copy_Tests, BooleansAreCopiedByValue) {
  _variant1 = true;
  _variant2 = _variant1;
  _variant1 = false;

  EXPECT_TRUE(_variant2.as<bool>());
}

TEST_F(JsonVariant_Copy_Tests, StringsAreCopiedByValue) {
  _variant1 = "hello";
  _variant2 = _variant1;
  _variant1 = "world";

  EXPECT_STREQ("hello", _variant2.as<const char *>());
}

TEST_F(JsonVariant_Copy_Tests, ObjectsAreCopiedByReference) {
  JsonObject &object = _jsonBuffer.createObject();

  _variant1 = object;

  object["hello"] = "world";

  EXPECT_EQ(1, _variant1.as<JsonObject>().size());
}

TEST_F(JsonVariant_Copy_Tests, ArraysAreCopiedByReference) {
  JsonArray &array = _jsonBuffer.createArray();

  _variant1 = array;

  array.add("world");

  EXPECT_EQ(1, _variant1.as<JsonArray>().size());
}
