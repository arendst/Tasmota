// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>
#include <sstream>

TEST(StdStream_Tests, JsonVariantFalse) {
  std::ostringstream os;
  JsonVariant variant = false;
  os << variant;
  ASSERT_EQ("false", os.str());
}

TEST(StdStream_Tests, JsonVariantString) {
  std::ostringstream os;
  JsonVariant variant = "coucou";
  os << variant;
  ASSERT_EQ("\"coucou\"", os.str());
}

TEST(StdStream_Tests, JsonObject) {
  std::ostringstream os;
  DynamicJsonBuffer jsonBuffer;
  JsonObject& object = jsonBuffer.createObject();
  object["key"] = "value";
  os << object;
  ASSERT_EQ("{\"key\":\"value\"}", os.str());
}

TEST(StdStream_Tests, JsonObjectSubscript) {
  std::ostringstream os;
  DynamicJsonBuffer jsonBuffer;
  JsonObject& object = jsonBuffer.createObject();
  object["key"] = "value";
  os << object["key"];
  ASSERT_EQ("\"value\"", os.str());
}

TEST(StdStream_Tests, JsonArray) {
  std::ostringstream os;
  DynamicJsonBuffer jsonBuffer;
  JsonArray& array = jsonBuffer.createArray();
  array.add("value");
  os << array;
  ASSERT_EQ("[\"value\"]", os.str());
}

TEST(StdStream_Tests, JsonArraySubscript) {
  std::ostringstream os;
  DynamicJsonBuffer jsonBuffer;
  JsonArray& array = jsonBuffer.createArray();
  array.add("value");
  os << array[0];
  ASSERT_EQ("\"value\"", os.str());
}

TEST(StdStream_Tests, ParseArray) {
  std::istringstream json(" [ 42 /* comment */ ] ");
  DynamicJsonBuffer jsonBuffer;
  JsonArray& arr = jsonBuffer.parseArray(json);
  ASSERT_TRUE(arr.success());
  ASSERT_EQ(1, arr.size());
  ASSERT_EQ(42, arr[0]);
}

TEST(StdStream_Tests, ParseObject) {
  std::istringstream json(" { hello : world // comment\n }");
  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.parseObject(json);
  ASSERT_TRUE(obj.success());
  ASSERT_EQ(1, obj.size());
  ASSERT_STREQ("world", obj["hello"]);
}

TEST(StdStream_Tests, ShouldNotReadPastTheEnd) {
  std::istringstream json("{}123");
  DynamicJsonBuffer jsonBuffer;
  jsonBuffer.parseObject(json);
  ASSERT_EQ('1', json.get());
}
