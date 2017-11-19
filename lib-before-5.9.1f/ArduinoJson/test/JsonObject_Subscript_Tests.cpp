// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class JsonObject_Subscript_Tests : public ::testing::Test {
 public:
  JsonObject_Subscript_Tests() : _object(_jsonBuffer.createObject()) {}

 protected:
  DynamicJsonBuffer _jsonBuffer;
  JsonObject& _object;
};

#define TEST_(name) TEST_F(JsonObject_Subscript_Tests, name)

TEST_(SizeIncreased_WhenValuesAreAdded) {
  _object["hello"] = 1;
  EXPECT_EQ(1, _object.size());
}

TEST_(SizeUntouched_WhenSameValueIsAdded) {
  _object["hello"] = 1;
  _object["hello"] = 2;
  EXPECT_EQ(1, _object.size());
}

TEST_(StoreInteger) {
  _object["hello"] = 123;

  EXPECT_EQ(123, _object["hello"].as<int>());
  EXPECT_TRUE(_object["hello"].is<int>());
  EXPECT_FALSE(_object["hello"].is<double>());
}

TEST_(StoreVolatileInteger) {  // issue #415
  volatile int i = 123;
  _object["hello"] = i;

  EXPECT_EQ(123, _object["hello"].as<int>());
  EXPECT_TRUE(_object["hello"].is<int>());
  EXPECT_FALSE(_object["hello"].is<double>());
}

TEST_(StoreDouble) {
  _object["hello"] = 123.45;

  EXPECT_TRUE(_object["hello"].is<double>());
  EXPECT_FALSE(_object["hello"].is<long>());
  EXPECT_EQ(123.45, _object["hello"].as<double>());
}

TEST_(StoreDoubleWithDigits) {
  _object["hello"].set(123.45, 2);

  EXPECT_TRUE(_object["hello"].is<double>());
  EXPECT_FALSE(_object["hello"].is<long>());
  EXPECT_EQ(123.45, _object["hello"].as<double>());
}

TEST_(StoreBoolean) {
  _object["hello"] = true;

  EXPECT_TRUE(_object["hello"].is<bool>());
  EXPECT_FALSE(_object["hello"].is<long>());
  EXPECT_TRUE(_object["hello"].as<bool>());
}

TEST_(StoreString) {
  _object["hello"] = "h3110";

  EXPECT_TRUE(_object["hello"].is<const char*>());
  EXPECT_FALSE(_object["hello"].is<long>());
  EXPECT_STREQ("h3110", _object["hello"].as<const char*>());
  EXPECT_STREQ("h3110", _object["hello"].as<char*>());  // <- short hand
}

TEST_(StoreArray) {
  JsonArray& arr = _jsonBuffer.createArray();

  _object["hello"] = arr;

  EXPECT_EQ(&arr, &_object["hello"].as<JsonArray&>());
  EXPECT_EQ(&arr, &_object["hello"].as<JsonArray>());  // <- short hand
  EXPECT_EQ(&arr, &_object["hello"].as<const JsonArray&>());
  EXPECT_EQ(&arr, &_object["hello"].as<const JsonArray>());  // <- short hand
  EXPECT_TRUE(_object["hello"].is<JsonArray&>());
  EXPECT_TRUE(_object["hello"].is<JsonArray>());
  EXPECT_TRUE(_object["hello"].is<const JsonArray&>());
  EXPECT_TRUE(_object["hello"].is<const JsonArray>());
  EXPECT_FALSE(_object["hello"].is<JsonObject&>());
}

TEST_(StoreObject) {
  JsonObject& obj = _jsonBuffer.createObject();

  _object["hello"] = obj;

  EXPECT_EQ(&obj, &_object["hello"].as<JsonObject&>());
  EXPECT_EQ(&obj, &_object["hello"].as<JsonObject>());  // <- short hand
  EXPECT_EQ(&obj, &_object["hello"].as<const JsonObject&>());
  EXPECT_EQ(&obj, &_object["hello"].as<const JsonObject>());  // <- short hand
  EXPECT_TRUE(_object["hello"].is<JsonObject&>());
  EXPECT_TRUE(_object["hello"].is<JsonObject>());
  EXPECT_TRUE(_object["hello"].is<const JsonObject&>());
  EXPECT_TRUE(_object["hello"].is<const JsonObject>());
  EXPECT_FALSE(_object["hello"].is<JsonArray&>());
}

TEST_(StoreArraySubscript) {
  JsonArray& arr = _jsonBuffer.createArray();
  arr.add(42);

  _object["a"] = arr[0];

  EXPECT_EQ(42, _object["a"]);
}

TEST_(StoreObjectSubscript) {
  JsonObject& obj = _jsonBuffer.createObject();
  obj.set("x", 42);

  _object["a"] = obj["x"];

  EXPECT_EQ(42, _object["a"]);
}

TEST_(KeyAsCharArray) {  // issue #423
  char key[] = "hello";
  _object[key] = 42;
  EXPECT_EQ(42, _object[key]);
}
