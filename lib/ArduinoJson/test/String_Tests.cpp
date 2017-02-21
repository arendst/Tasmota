// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class StringTests : public ::testing::Test {
 protected:
  static void eraseString(std::string &str) {
    char *p = const_cast<char *>(str.c_str());
    while (*p) *p++ = '*';
  }

  DynamicJsonBuffer _jsonBuffer;
};

TEST_F(StringTests, JsonBuffer_ParseArray) {
  std::string json("[\"hello\"]");
  JsonArray &array = _jsonBuffer.parseArray(json);
  eraseString(json);
  ASSERT_TRUE(array.success());
  ASSERT_STREQ("hello", array[0]);
}

TEST_F(StringTests, JsonBuffer_ParseObject) {
  std::string json("{\"hello\":\"world\"}");
  JsonObject &object = _jsonBuffer.parseObject(json);
  eraseString(json);
  ASSERT_TRUE(object.success());
  ASSERT_STREQ("world", object["hello"]);
}

TEST_F(StringTests, JsonObject_Subscript) {
  char json[] = "{\"key\":\"value\"}";
  JsonObject &object = _jsonBuffer.parseObject(json);
  ASSERT_STREQ("value", object[std::string("key")]);
}

TEST_F(StringTests, JsonObject_ConstSubscript) {
  char json[] = "{\"key\":\"value\"}";
  const JsonObject &object = _jsonBuffer.parseObject(json);
  ASSERT_STREQ("value", object[std::string("key")]);
}

TEST_F(StringTests, JsonObject_SetKey) {
  JsonObject &object = _jsonBuffer.createObject();
  std::string key("hello");
  object.set(key, "world");
  eraseString(key);
  ASSERT_STREQ("world", object["hello"]);
}

TEST_F(StringTests, JsonObject_SetValue) {
  JsonObject &object = _jsonBuffer.createObject();
  std::string value("world");
  object.set("hello", value);
  eraseString(value);
  ASSERT_STREQ("world", object["hello"]);
}

TEST_F(StringTests, JsonObject_SetKeyValue) {
  JsonObject &object = _jsonBuffer.createObject();
  std::string key("hello");
  std::string value("world");
  object.set(key, value);
  eraseString(key);
  eraseString(value);
  ASSERT_STREQ("world", object["hello"]);
}

TEST_F(StringTests, JsonObject_SetToArraySubscript) {
  JsonArray &arr = _jsonBuffer.createArray();
  arr.add("world");

  JsonObject &object = _jsonBuffer.createObject();
  object.set(std::string("hello"), arr[0]);

  ASSERT_STREQ("world", object["hello"]);
}

TEST_F(StringTests, JsonObject_SetToObjectSubscript) {
  JsonObject &arr = _jsonBuffer.createObject();
  arr.set("x", "world");

  JsonObject &object = _jsonBuffer.createObject();
  object.set(std::string("hello"), arr["x"]);

  ASSERT_STREQ("world", object["hello"]);
}

TEST_F(StringTests, JsonObject_Get) {
  char json[] = "{\"key\":\"value\"}";
  const JsonObject &object = _jsonBuffer.parseObject(json);
  ASSERT_STREQ("value", object.get<const char *>(std::string("key")));
}

TEST_F(StringTests, JsonObject_GetT) {
  char json[] = "{\"key\":\"value\"}";
  const JsonObject &object = _jsonBuffer.parseObject(json);
  ASSERT_STREQ("value", object.get<const char *>(std::string("key")));
}

TEST_F(StringTests, JsonObject_IsT) {
  char json[] = "{\"key\":\"value\"}";
  const JsonObject &object = _jsonBuffer.parseObject(json);
  ASSERT_TRUE(object.is<const char *>(std::string("key")));
}

TEST_F(StringTests, JsonObject_CreateNestedObject) {
  std::string key = "key";
  char json[64];
  JsonObject &object = _jsonBuffer.createObject();
  object.createNestedObject(key);
  eraseString(key);
  object.printTo(json, sizeof(json));
  ASSERT_STREQ("{\"key\":{}}", json);
}

TEST_F(StringTests, JsonObject_CreateNestedArray) {
  std::string key = "key";
  char json[64];
  JsonObject &object = _jsonBuffer.createObject();
  object.createNestedArray(key);
  eraseString(key);
  object.printTo(json, sizeof(json));
  ASSERT_STREQ("{\"key\":[]}", json);
}

TEST_F(StringTests, JsonObject_ContainsKey) {
  char json[] = "{\"key\":\"value\"}";
  const JsonObject &object = _jsonBuffer.parseObject(json);
  ASSERT_TRUE(object.containsKey(std::string("key")));
}

TEST_F(StringTests, JsonObject_Remove) {
  char json[] = "{\"key\":\"value\"}";
  JsonObject &object = _jsonBuffer.parseObject(json);
  ASSERT_EQ(1, object.size());
  object.remove(std::string("key"));
  ASSERT_EQ(0, object.size());
}

TEST_F(StringTests, JsonObjectSubscript_SetKey) {
  JsonObject &object = _jsonBuffer.createObject();
  std::string key("hello");
  object[key] = "world";
  eraseString(key);
  ASSERT_STREQ("world", object["hello"]);
}

TEST_F(StringTests, JsonObjectSubscript_SetValue) {
  JsonObject &object = _jsonBuffer.createObject();
  std::string value("world");
  object["hello"] = value;
  eraseString(value);
  ASSERT_STREQ("world", object["hello"]);
}

TEST_F(StringTests, JsonArray_Add) {
  JsonArray &array = _jsonBuffer.createArray();
  std::string value("hello");
  array.add(value);
  eraseString(value);
  ASSERT_STREQ("hello", array[0]);
}

TEST_F(StringTests, JsonArray_Set) {
  JsonArray &array = _jsonBuffer.createArray();
  std::string value("world");
  array.add("hello");
  array.set(0, value);
  eraseString(value);
  ASSERT_STREQ("world", array[0]);
}

TEST_F(StringTests, JsonArraySubscript) {
  JsonArray &array = _jsonBuffer.createArray();
  std::string value("world");
  array.add("hello");
  array[0] = value;
  eraseString(value);
  ASSERT_STREQ("world", array[0]);
}

TEST_F(StringTests, JsonArray_PrintTo) {
  JsonArray &array = _jsonBuffer.createArray();
  array.add(4);
  array.add(2);
  std::string json;
  array.printTo(json);
  ASSERT_EQ(std::string("[4,2]"), json);
}

TEST_F(StringTests, JsonArray_PrettyPrintTo) {
  JsonArray &array = _jsonBuffer.createArray();
  array.add(4);
  array.add(2);
  std::string json;
  array.prettyPrintTo(json);
  ASSERT_EQ(std::string("[\r\n  4,\r\n  2\r\n]"), json);
}

TEST_F(StringTests, JsonObject_PrintTo) {
  JsonObject &object = _jsonBuffer.createObject();
  object["key"] = "value";
  std::string json;
  object.printTo(json);
  ASSERT_EQ(std::string("{\"key\":\"value\"}"), json);
}

TEST_F(StringTests, JsonObject_PrettyPrintTo) {
  JsonObject &object = _jsonBuffer.createObject();
  object["key"] = "value";
  std::string json;
  object.prettyPrintTo(json);
  ASSERT_EQ(std::string("{\r\n  \"key\": \"value\"\r\n}"), json);
}

TEST_F(StringTests, JsonBuffer_GrowWhenAddingNewKey) {
  JsonObject &object = _jsonBuffer.createObject();
  std::string key1("hello"), key2("world");

  object[key1] = 1;
  size_t sizeBefore = _jsonBuffer.size();
  object[key2] = 2;
  size_t sizeAfter = _jsonBuffer.size();

  ASSERT_GT(sizeAfter - sizeBefore, key2.size());
}

TEST_F(StringTests, JsonBuffer_DontGrowWhenReusingKey) {
  JsonObject &object = _jsonBuffer.createObject();
  std::string key("hello");

  object[key] = 1;
  size_t sizeBefore = _jsonBuffer.size();
  object[key] = 2;
  size_t sizeAfter = _jsonBuffer.size();

  ASSERT_EQ(sizeBefore, sizeAfter);
}

TEST_F(StringTests, JsonBuffer_strdup) {
  std::string original("hello");
  char *copy = _jsonBuffer.strdup(original);
  original[0] = 'w';
  ASSERT_STREQ("hello", copy);
}
