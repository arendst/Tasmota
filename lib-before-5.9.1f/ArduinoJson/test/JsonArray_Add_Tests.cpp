// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class JsonArray_Add_Tests : public ::testing::Test {
 protected:
  JsonArray_Add_Tests() : _array(_jsonBuffer.createArray()) {}

  DynamicJsonBuffer _jsonBuffer;
  JsonArray& _array;
};

#define TEST_(name) TEST_F(JsonArray_Add_Tests, name)

TEST_(SizeIncreased_WhenValuesAreAdded) {
  _array.add("hello");
  EXPECT_EQ(1U, _array.size());
}

TEST_(StoreInteger) {
  _array.add(123);
  EXPECT_EQ(123, _array[0].as<int>());
  EXPECT_TRUE(_array[0].is<int>());
  EXPECT_FALSE(_array[0].is<double>());
}

TEST_(StoreDouble) {
  _array.add(123.45);
  EXPECT_EQ(123.45, _array[0].as<double>());
  EXPECT_TRUE(_array[0].is<double>());
  EXPECT_FALSE(_array[0].is<int>());
}

TEST_(StoreBoolean) {
  _array.add(true);
  EXPECT_EQ(true, _array[0].as<bool>());
  EXPECT_TRUE(_array[0].is<bool>());
  EXPECT_FALSE(_array[0].is<int>());
}

TEST_(StoreString) {
  _array.add("hello");
  EXPECT_STREQ("hello", _array[0].as<const char*>());
  EXPECT_TRUE(_array[0].is<const char*>());
  EXPECT_FALSE(_array[0].is<int>());
}

TEST_(StoreNestedArray) {
  JsonArray& arr = _jsonBuffer.createArray();

  _array.add(arr);

  EXPECT_EQ(&arr, &_array[0].as<JsonArray&>());
  EXPECT_TRUE(_array[0].is<JsonArray&>());
  EXPECT_FALSE(_array[0].is<int>());
}

TEST_(StoreNestedObject) {
  JsonObject& obj = _jsonBuffer.createObject();

  _array.add(obj);

  EXPECT_EQ(&obj, &_array[0].as<JsonObject&>());
  EXPECT_TRUE(_array[0].is<JsonObject&>());
  EXPECT_FALSE(_array[0].is<int>());
}

TEST_(StoreArraySubscript) {
  JsonArray& arr = _jsonBuffer.createArray();
  arr.add("hello");

  _array.add(arr[0]);

  EXPECT_STREQ("hello", _array[0]);
}

TEST_(StoreObjectSubscript) {
  JsonObject& obj = _jsonBuffer.createObject();
  obj["x"] = "hello";

  _array.add(obj["x"]);

  EXPECT_STREQ("hello", _array[0]);
}
