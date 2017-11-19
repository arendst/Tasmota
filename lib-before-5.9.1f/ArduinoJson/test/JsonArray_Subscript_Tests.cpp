// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>
#include <stdint.h>

class JsonArray_Subscript_Tests : public ::testing::Test {
 protected:
  JsonArray_Subscript_Tests() : _array(_jsonBuffer.createArray()) {
    _array.add(0);
  }

  DynamicJsonBuffer _jsonBuffer;
  JsonArray& _array;
};

#define TEST_(name) TEST_F(JsonArray_Subscript_Tests, name)

TEST_(SizeIsUnchanged) {
  _array[0] = "hello";
  EXPECT_EQ(1U, _array.size());
}

TEST_(StoreInteger) {
  _array[0] = 123;
  EXPECT_EQ(123, _array[0].as<int>());
  EXPECT_TRUE(_array[0].is<int>());
  EXPECT_FALSE(_array[0].is<double>());
}

#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
TEST_(StoreLongLong) {
  _array[0] = 9223372036854775807;
  EXPECT_EQ(9223372036854775807, _array[0].as<long long>());
  EXPECT_TRUE(_array[0].is<int>());
  EXPECT_FALSE(_array[0].is<double>());
}
#endif

TEST_(StoreDouble) {
  _array[0] = 123.45;
  EXPECT_EQ(123.45, _array[0].as<double>());
  EXPECT_TRUE(_array[0].is<double>());
  EXPECT_FALSE(_array[0].is<int>());
}

TEST_(StoreDoubleWithDecimals) {
  _array[0].set(123.45, 2);
  EXPECT_EQ(123.45, _array[0].as<double>());
  EXPECT_TRUE(_array[0].is<double>());
  EXPECT_FALSE(_array[0].is<int>());
}

TEST_(StoreBoolean) {
  _array[0] = true;
  EXPECT_EQ(true, _array[0].as<bool>());
  EXPECT_TRUE(_array[0].is<bool>());
  EXPECT_FALSE(_array[0].is<int>());
}

TEST_(StoreString) {
  _array[0] = "hello";
  EXPECT_STREQ("hello", _array[0].as<const char*>());
  EXPECT_STREQ("hello", _array[0].as<char*>());  // <- short hand
  EXPECT_TRUE(_array[0].is<const char*>());
  EXPECT_FALSE(_array[0].is<int>());
}

TEST_(StoreNestedArray) {
  JsonArray& arr = _jsonBuffer.createArray();

  _array[0] = arr;

  EXPECT_EQ(&arr, &_array[0].as<JsonArray&>());
  EXPECT_EQ(&arr, &_array[0].as<JsonArray>());  // <- short hand
  EXPECT_EQ(&arr, &_array[0].as<const JsonArray&>());
  EXPECT_EQ(&arr, &_array[0].as<const JsonArray>());  // <- short hand
  EXPECT_TRUE(_array[0].is<JsonArray&>());
  EXPECT_FALSE(_array[0].is<int>());
}

TEST_(StoreNestedObject) {
  JsonObject& obj = _jsonBuffer.createObject();

  _array[0] = obj;

  EXPECT_EQ(&obj, &_array[0].as<JsonObject&>());
  EXPECT_EQ(&obj, &_array[0].as<JsonObject>());  // <- short hand
  EXPECT_EQ(&obj, &_array[0].as<const JsonObject&>());
  EXPECT_EQ(&obj, &_array[0].as<const JsonObject>());  // <- short hand
  EXPECT_TRUE(_array[0].is<JsonObject&>());
  EXPECT_FALSE(_array[0].is<int>());
}

TEST_(StoreArraySubscript) {
  JsonArray& arr = _jsonBuffer.createArray();
  arr.add("hello");

  _array[0] = arr[0];

  EXPECT_STREQ("hello", _array[0]);
}

TEST_(StoreObjectSubscript) {
  JsonObject& obj = _jsonBuffer.createObject();
  obj["x"] = "hello";

  _array[0] = obj["x"];

  EXPECT_STREQ("hello", _array[0]);
}
