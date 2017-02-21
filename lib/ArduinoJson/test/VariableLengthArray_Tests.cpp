// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wvla-extension"
#define CONFLICTS_WITH_BUILTIN_OPERATOR
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wvla"
#else
#define VLA_NOT_SUPPORTED
#endif

#ifndef VLA_NOT_SUPPORTED

TEST(VariableLengthArray, ParseArray) {
  int i = 8;
  char vla[i];
  strcpy(vla, "[42]");

  StaticJsonBuffer<JSON_ARRAY_SIZE(1)> jsonBuffer;
  JsonArray& arr = jsonBuffer.parseArray(vla);

  EXPECT_TRUE(arr.success());
}

TEST(VariableLengthArray, ParseObject) {
  int i = 16;
  char vla[i];
  strcpy(vla, "{\"a\":42}");

  StaticJsonBuffer<JSON_OBJECT_SIZE(1)> jsonBuffer;
  JsonObject& obj = jsonBuffer.parseObject(vla);

  EXPECT_TRUE(obj.success());
}

TEST(VariableLengthArray, Parse) {
  int i = 16;
  char vla[i];
  strcpy(vla, "42");

  StaticJsonBuffer<1> jsonBuffer;
  JsonVariant variant = jsonBuffer.parse(vla);

  EXPECT_EQ(42, variant.as<int>());
}

TEST(VariableLengthArray, JsonVariant_Constructor) {
  int i = 16;
  char vla[i];
  strcpy(vla, "42");

  JsonVariant variant(vla);

  EXPECT_EQ(42, variant.as<int>());
}

TEST(VariableLengthArray, JsonVariant_Assign) {
  int i = 16;
  char vla[i];
  strcpy(vla, "42");

  JsonVariant variant(666);
  variant = vla;

  EXPECT_EQ(42, variant.as<int>());
}

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
TEST(VariableLengthArray, JsonVariant_Subscript) {
  int i = 16;
  char vla[i];
  strcpy(vla, "hello");

  DynamicJsonBuffer jsonBuffer;
  JsonVariant variant = jsonBuffer.parseObject("{\"hello\":\"world\"}");

  EXPECT_STREQ("world", variant[vla]);
}
#endif

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
TEST(VariableLengthArray, JsonVariant_Subscript_Const) {
  int i = 16;
  char vla[i];
  strcpy(vla, "hello");

  DynamicJsonBuffer jsonBuffer;
  const JsonVariant variant = jsonBuffer.parseObject("{\"hello\":\"world\"}");

  EXPECT_STREQ("world", variant[vla]);
}
#endif

TEST(VariableLengthArray, JsonVariant_Equals) {
  int i = 16;
  char vla[i];
  strcpy(vla, "hello");

  DynamicJsonBuffer jsonBuffer;
  const JsonVariant variant = "hello";

  EXPECT_TRUE(vla == variant);
  EXPECT_TRUE(variant == vla);
  EXPECT_FALSE(vla != variant);
  EXPECT_FALSE(variant != vla);
}

TEST(VariableLengthArray, JsonVariant_Differs) {
  int i = 16;
  char vla[i];
  strcpy(vla, "hello");

  DynamicJsonBuffer jsonBuffer;
  const JsonVariant variant = "world";

  EXPECT_TRUE(vla != variant);
  EXPECT_TRUE(variant != vla);
  EXPECT_FALSE(vla == variant);
  EXPECT_FALSE(variant == vla);
}

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
TEST(VariableLengthArray, JsonObject_Subscript) {
  int i = 16;
  char vla[i];
  strcpy(vla, "hello");

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.createObject();
  obj[vla] = "world";

  EXPECT_STREQ("world", obj["hello"]);
}
#endif

TEST(VariableLengthArray, JsonObject_Subscript_Assign) {  // issue #416
  int i = 32;
  char vla[i];
  strcpy(vla, "world");

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.createObject();
  obj["hello"] = vla;

  EXPECT_STREQ("world", obj["hello"].as<char*>());
}

TEST(VariableLengthArray, JsonObject_Subscript_Set) {
  int i = 32;
  char vla[i];
  strcpy(vla, "world");

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.createObject();
  obj["hello"].set(vla);

  EXPECT_STREQ("world", obj["hello"].as<char*>());
}

#ifndef CONFLICTS_WITH_BUILTIN_OPERATOR
TEST(VariableLengthArray, JsonObject_Subscript_Const) {
  int i = 16;
  char vla[i];
  strcpy(vla, "hello");

  DynamicJsonBuffer jsonBuffer;
  const JsonObject& obj = jsonBuffer.parseObject("{\"hello\":\"world\"}");

  EXPECT_STREQ("world", obj[vla]);
}
#endif

TEST(VariableLengthArray, JsonObject_Get) {
  int i = 16;
  char vla[i];
  strcpy(vla, "hello");

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.parseObject("{\"hello\":\"world\"}");

  EXPECT_STREQ("world", obj.get<char*>(vla));
}

TEST(VariableLengthArray, JsonObject_Set_Key) {
  int i = 16;
  char vla[i];
  strcpy(vla, "hello");

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.createObject();
  obj.set(vla, "world");

  EXPECT_STREQ("world", obj["hello"]);
}

TEST(VariableLengthArray, JsonObject_Set_Value) {
  int i = 16;
  char vla[i];
  strcpy(vla, "world");

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.createObject();
  obj.set("hello", vla);

  EXPECT_STREQ("world", obj["hello"]);
}

TEST(VariableLengthArray, JsonObject_Set_Key_WithDecimals) {
  int i = 16;
  char vla[i];
  strcpy(vla, "hello");

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.createObject();
  obj.set(vla, 3.14, 2);

  EXPECT_EQ(3.14, obj["hello"]);
}

TEST(VariableLengthArray, JsonObject_Set_KeyAndValue) {
  int i = 16;
  char vla[i];
  strcpy(vla, "world");

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.createObject();
  obj.set(vla, vla);

  EXPECT_STREQ("world", obj["world"]);
}

TEST(VariableLengthArray, JsonObject_ContainsKey) {
  int i = 16;
  char vla[i];
  strcpy(vla, "hello");

  DynamicJsonBuffer jsonBuffer;
  const JsonObject& obj = jsonBuffer.parseObject("{\"hello\":\"world\"}");

  EXPECT_TRUE(obj.containsKey(vla));
}

TEST(VariableLengthArray, JsonObject_Remove) {
  int i = 16;
  char vla[i];
  strcpy(vla, "hello");

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.parseObject("{\"hello\":\"world\"}");
  obj.remove(vla);

  EXPECT_EQ(0, obj.size());
}

TEST(VariableLengthArray, JsonObject_Is) {
  int i = 16;
  char vla[i];
  strcpy(vla, "hello");

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.parseObject("{\"hello\":42}");

  EXPECT_TRUE(obj.is<int>(vla));
}

TEST(VariableLengthArray, JsonObject_CreateNestedArray) {
  int i = 16;
  char vla[i];
  strcpy(vla, "hello");

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.createObject();
  obj.createNestedArray(vla);
}

TEST(VariableLengthArray, JsonObject_CreateNestedObject) {
  int i = 16;
  char vla[i];
  strcpy(vla, "hello");

  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj = jsonBuffer.createObject();
  obj.createNestedObject(vla);
}

TEST(VariableLengthArray, JsonArray_Add) {
  int i = 16;
  char vla[i];
  strcpy(vla, "world");

  DynamicJsonBuffer jsonBuffer;
  JsonArray& arr = jsonBuffer.createArray();
  arr.add(vla);

  EXPECT_STREQ("world", arr[0]);
}

TEST(VariableLengthArray, JsonArray_Set) {
  int i = 16;
  char vla[i];
  strcpy(vla, "world");

  DynamicJsonBuffer jsonBuffer;
  JsonArray& arr = jsonBuffer.createArray();
  arr.add("hello");
  arr.set(0, vla);

  EXPECT_STREQ("world", arr[0]);
}

TEST(VariableLengthArray, JsonArraySubscript_Set) {
  int i = 16;
  char vla[i];
  strcpy(vla, "world");

  DynamicJsonBuffer jsonBuffer;
  JsonArray& arr = jsonBuffer.createArray();
  arr.add("hello");
  arr[0].set(vla);

  EXPECT_STREQ("world", arr[0]);
}

TEST(VariableLengthArray, JsonArraySubscript_Assign) {
  int i = 16;
  char vla[i];
  strcpy(vla, "world");

  DynamicJsonBuffer jsonBuffer;
  JsonArray& arr = jsonBuffer.createArray();
  arr.add("hello");
  arr[0] = vla;

  EXPECT_STREQ("world", arr[0]);
}

TEST(VariableLengthArray, JsonBuffer_strdup) {
  int i = 16;
  char vla[i];
  strcpy(vla, "world");

  DynamicJsonBuffer jsonBuffer;
  const char* dup = jsonBuffer.strdup(vla);

  EXPECT_NE(static_cast<const void*>(vla), static_cast<const void*>(dup));
  EXPECT_STREQ("world", dup);
}

#endif
