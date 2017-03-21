// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

#if defined(__clang__)
#define CONFLICTS_WITH_BUILTIN_OPERATOR
#endif

TEST(UnsignedCharArray, ParseArray) {
  unsigned char json[] = "[42]";

  StaticJsonBuffer<JSON_ARRAY_SIZE(1)> jsonBuffer;
  JsonArray& arr = jsonBuffer.parseArray(json);

  EXPECT_TRUE(arr.success());
}

TEST(UnsignedCharArray, ParseObject) {
  unsigned char json[] = "{\"a\":42}";

  StaticJsonBuffer<JSON_OBJECT_SIZE(1)> jsonBuffer;
  JsonObject& obj = jsonBuffer.parseObject(json);

  EXPECT_TRUE(obj.success());
}

TEST(UnsignedCharArray, JsonVariant_Constructor) {
  unsigned char value[] = "42";

  JsonVariant variant(value);

  EXPECT_EQ(42, variant.as<int>());
}

TEST(UnsignedCharArray, JsonVariant_Assign) {
  unsigned char value[] = "42";

  JsonVariant variant(666);
  variant = value;

  EXPECT_EQ(42, variant.as<int>());
}

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
TEST(UnsignedCharArray, JsonVariant_Subscript) {
  unsigned char key[] = "hello";

  DynamicJsonBuffer jsonBuffer;
  JsonVariant variant = jsonBuffer.parseObject("{\"hello\":\"world\"}");

  EXPECT_STREQ("world", variant[key]);
}
#endif

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
TEST(UnsignedCharArray, JsonVariant_Subscript_Const) {
  unsigned char key[] = "hello";

  DynamicJsonBuffer jsonBuffer;
  const JsonVariant variant = jsonBuffer.parseObject("{\"hello\":\"world\"}");

  EXPECT_STREQ("world", variant[key]);
}
#endif

TEST(UnsignedCharArray, JsonVariant_Equals) {
  unsigned char comparand[] = "hello";

  DynamicJsonBuffer jsonBuffer;
  const JsonVariant variant = "hello";

  EXPECT_TRUE(comparand == variant);
  EXPECT_TRUE(variant == comparand);
  EXPECT_FALSE(comparand != variant);
  EXPECT_FALSE(variant != comparand);
}

TEST(UnsignedCharArray, JsonVariant_Differs) {
  unsigned char comparand[] = "hello";

  DynamicJsonBuffer jsonBuffer;
  const JsonVariant variant = "world";

  EXPECT_TRUE(comparand != variant);
  EXPECT_TRUE(variant != comparand);
  EXPECT_FALSE(comparand == variant);
  EXPECT_FALSE(variant == comparand);
}

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
TEST(UnsignedCharArray, JsonObject_Subscript) {
  unsigned char key[] = "hello";

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.createObject();
  obj[key] = "world";

  EXPECT_STREQ("world", obj["hello"]);
}
#endif

TEST(UnsignedCharArray, JsonObject_Subscript_Assign) {  // issue #416
  unsigned char value[] = "world";

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.createObject();
  obj["hello"] = value;

  EXPECT_STREQ("world", obj["hello"]);
}

TEST(UnsignedCharArray, JsonObject_Subscript_Set) {
  unsigned char value[] = "world";

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.createObject();
  obj["hello"].set(value);

  EXPECT_STREQ("world", obj["hello"]);
}

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
TEST(UnsignedCharArray, JsonObject_Subscript_Const) {
  unsigned char key[] = "hello";

  DynamicJsonBuffer jsonBuffer;
  const JsonObject& obj = jsonBuffer.parseObject("{\"hello\":\"world\"}");

  EXPECT_STREQ("world", obj[key]);
}
#endif

TEST(UnsignedCharArray, JsonObject_Get) {
  unsigned char key[] = "hello";

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.parseObject("{\"hello\":\"world\"}");

  EXPECT_STREQ("world", obj.get<char*>(key));
}

TEST(UnsignedCharArray, JsonObject_Set_Key) {
  unsigned char key[] = "hello";

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.createObject();
  obj.set(key, "world");

  EXPECT_STREQ("world", obj["hello"]);
}

TEST(UnsignedCharArray, JsonObject_Set_Value) {
  unsigned char value[] = "world";

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.createObject();
  obj.set("hello", value);

  EXPECT_STREQ("world", obj["hello"]);
}

TEST(UnsignedCharArray, JsonObject_Set_Key_WithDecimals) {
  unsigned char key[] = "hello";

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.createObject();
  obj.set(key, 3.14, 2);

  EXPECT_EQ(3.14, obj["hello"]);
}

TEST(UnsignedCharArray, JsonObject_Set_KeyAndValue) {
  unsigned char key[] = "world";

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.createObject();
  obj.set(key, key);

  EXPECT_STREQ("world", obj["world"]);
}

TEST(UnsignedCharArray, JsonObject_ContainsKey) {
  unsigned char key[] = "hello";

  DynamicJsonBuffer jsonBuffer;
  const JsonObject& obj = jsonBuffer.parseObject("{\"hello\":\"world\"}");

  EXPECT_TRUE(obj.containsKey(key));
}

TEST(UnsignedCharArray, JsonObject_Remove) {
  unsigned char key[] = "hello";

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.parseObject("{\"hello\":\"world\"}");
  obj.remove(key);

  EXPECT_EQ(0, obj.size());
}

TEST(UnsignedCharArray, JsonObject_Is) {
  unsigned char key[] = "hello";

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.parseObject("{\"hello\":42}");

  EXPECT_TRUE(obj.is<int>(key));
}

TEST(UnsignedCharArray, JsonObject_CreateNestedArray) {
  unsigned char key[] = "hello";

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.createObject();
  obj.createNestedArray(key);
}

TEST(UnsignedCharArray, JsonObject_CreateNestedObject) {
  unsigned char key[] = "hello";

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.createObject();
  obj.createNestedObject(key);
}

TEST(UnsignedCharArray, JsonArray_Add) {
  unsigned char value[] = "world";

  DynamicJsonBuffer jsonBuffer;
  JsonArray& arr = jsonBuffer.createArray();
  arr.add(value);

  EXPECT_STREQ("world", arr[0]);
}

TEST(UnsignedCharArray, JsonArray_Set) {
  unsigned char value[] = "world";

  DynamicJsonBuffer jsonBuffer;
  JsonArray& arr = jsonBuffer.createArray();
  arr.add("hello");
  arr.set(0, value);

  EXPECT_STREQ("world", arr[0]);
}

TEST(UnsignedCharArray, JsonArraySubscript_Set) {
  unsigned char value[] = "world";

  DynamicJsonBuffer jsonBuffer;
  JsonArray& arr = jsonBuffer.createArray();
  arr.add("hello");
  arr[0].set(value);

  EXPECT_STREQ("world", arr[0]);
}

TEST(UnsignedCharArray, JsonArraySubscript_Assign) {
  unsigned char value[] = "world";

  DynamicJsonBuffer jsonBuffer;
  JsonArray& arr = jsonBuffer.createArray();
  arr.add("hello");
  arr[0] = value;

  EXPECT_STREQ("world", arr[0]);
}

TEST(UnsignedCharArray, JsonBuffer_strdup) {
  unsigned char value[] = "world";

  DynamicJsonBuffer jsonBuffer;
  const char* dup = jsonBuffer.strdup(value);

  EXPECT_NE(static_cast<const void*>(value), static_cast<const void*>(dup));
  EXPECT_STREQ("world", dup);
}
