// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class JsonParser_Object_Test : public testing::Test {
 protected:
  void whenInputIs(const char *jsonString) {
    strcpy(_jsonString, jsonString);
    _object = &_jsonBuffer.parseObject(_jsonString);
  }

  void parseMustSucceed() {
    EXPECT_TRUE(_object->success());
  }

  void parseMustFail() {
    EXPECT_FALSE(_object->success());
  }

  void sizeMustBe(int expected) {
    EXPECT_EQ(expected, _object->size());
  }

  void keyMustHaveValue(const char *key, const char *expected) {
    EXPECT_STREQ(expected, (*_object)[key]);
  }

  template <typename T>
  void keyMustHaveValue(const char *key, T expected) {
    EXPECT_EQ(expected, (*_object)[key].as<T>());
  }

 private:
  DynamicJsonBuffer _jsonBuffer;
  JsonObject *_object;
  char _jsonString[256];
};

TEST_F(JsonParser_Object_Test, EmptyObject) {
  whenInputIs("{}");
  parseMustSucceed();
  sizeMustBe(0);
}

TEST_F(JsonParser_Object_Test, MissingOpeningBrace) {
  whenInputIs("}");
  parseMustFail();
}

TEST_F(JsonParser_Object_Test, MissingClosingBrace) {
  whenInputIs("{");
  parseMustFail();
}

TEST_F(JsonParser_Object_Test, MissingColonAndValue) {
  whenInputIs("{\"key\"}");
  parseMustFail();
}

TEST_F(JsonParser_Object_Test, MissingQuotesAndColonAndValue) {
  whenInputIs("{key}");
  parseMustFail();
}

TEST_F(JsonParser_Object_Test, OneString) {
  whenInputIs("{\"key\":\"value\"}");
  parseMustSucceed();
  sizeMustBe(1);
  keyMustHaveValue("key", "value");
}

TEST_F(JsonParser_Object_Test, OneStringSingleQuotes) {
  whenInputIs("{'key':'value'}");
  parseMustSucceed();
  sizeMustBe(1);
  keyMustHaveValue("key", "value");
}

TEST_F(JsonParser_Object_Test, OneStringNoQuotes) {
  whenInputIs("{key:value}");
  parseMustSucceed();
  sizeMustBe(1);
  keyMustHaveValue("key", "value");
}

TEST_F(JsonParser_Object_Test, OneStringSpaceBeforeKey) {
  whenInputIs("{ \"key\":\"value\"}");
  parseMustSucceed();
  sizeMustBe(1);
  keyMustHaveValue("key", "value");
}

TEST_F(JsonParser_Object_Test, OneStringSpaceAfterKey) {
  whenInputIs("{\"key\" :\"value\"}");
  parseMustSucceed();
  sizeMustBe(1);
  keyMustHaveValue("key", "value");
}

TEST_F(JsonParser_Object_Test, OneStringSpaceBeforeValue) {
  whenInputIs("{\"key\": \"value\"}");
  parseMustSucceed();
  sizeMustBe(1);
  keyMustHaveValue("key", "value");
}

TEST_F(JsonParser_Object_Test, OneStringSpaceAfterValue) {
  whenInputIs("{\"key\":\"value\" }");
  parseMustSucceed();
  sizeMustBe(1);
  keyMustHaveValue("key", "value");
}

TEST_F(JsonParser_Object_Test, TwoStrings) {
  whenInputIs("{\"key1\":\"value1\",\"key2\":\"value2\"}");
  parseMustSucceed();
  sizeMustBe(2);
  keyMustHaveValue("key1", "value1");
  keyMustHaveValue("key2", "value2");
}

TEST_F(JsonParser_Object_Test, TwoStringsSpaceBeforeComma) {
  whenInputIs("{\"key1\":\"value1\" ,\"key2\":\"value2\"}");
  parseMustSucceed();
  sizeMustBe(2);
  keyMustHaveValue("key1", "value1");
  keyMustHaveValue("key2", "value2");
}

TEST_F(JsonParser_Object_Test, TwoStringsSpaceAfterComma) {
  whenInputIs("{\"key1\":\"value1\" ,\"key2\":\"value2\"}");
  parseMustSucceed();
  sizeMustBe(2);
  keyMustHaveValue("key1", "value1");
  keyMustHaveValue("key2", "value2");
}

TEST_F(JsonParser_Object_Test, EndingWithAComma) {
  whenInputIs("{\"key1\":\"value1\",}");
  parseMustFail();
  sizeMustBe(0);
}

TEST_F(JsonParser_Object_Test, TwoIntergers) {
  whenInputIs("{\"key1\":42,\"key2\":-42}");
  parseMustSucceed();
  sizeMustBe(2);
  keyMustHaveValue("key1", 42);
  keyMustHaveValue("key2", -42);
}

TEST_F(JsonParser_Object_Test, TwoDoubles) {
  whenInputIs("{\"key1\":12.345,\"key2\":-7E89}");
  parseMustSucceed();
  sizeMustBe(2);
  keyMustHaveValue("key1", 12.345);
  keyMustHaveValue("key2", -7E89);
}

TEST_F(JsonParser_Object_Test, TwoBooleans) {
  whenInputIs("{\"key1\":true,\"key2\":false}");
  parseMustSucceed();
  sizeMustBe(2);
  keyMustHaveValue("key1", true);
  keyMustHaveValue("key2", false);
}

TEST_F(JsonParser_Object_Test, TwoNulls) {
  const char *const nullstr = 0;

  whenInputIs("{\"key1\":null,\"key2\":null}");
  parseMustSucceed();
  sizeMustBe(2);
  keyMustHaveValue("key1", nullstr);
  keyMustHaveValue("key2", nullstr);
}

TEST_F(JsonParser_Object_Test, NullForKey) {
  whenInputIs("null:\"value\"}");
  parseMustFail();
}
