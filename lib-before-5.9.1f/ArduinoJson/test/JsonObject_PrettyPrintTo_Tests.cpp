// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class JsonObject_PrettyPrintTo_Tests : public testing::Test {
 public:
  JsonObject_PrettyPrintTo_Tests() : _object(_jsonBuffer.createObject()) {}

 protected:
  DynamicJsonBuffer _jsonBuffer;
  JsonObject &_object;

  void outputMustBe(const char *expected) {
    char buffer[256];

    size_t actualLen = _object.prettyPrintTo(buffer);
    size_t measuredLen = _object.measurePrettyLength();

    EXPECT_STREQ(expected, buffer);
    EXPECT_EQ(strlen(expected), actualLen);
    EXPECT_EQ(strlen(expected), measuredLen);
  }
};

TEST_F(JsonObject_PrettyPrintTo_Tests, EmptyObject) {
  outputMustBe("{}");
}

TEST_F(JsonObject_PrettyPrintTo_Tests, OneMember) {
  _object["key"] = "value";

  outputMustBe(
      "{\r\n"
      "  \"key\": \"value\"\r\n"
      "}");
}

TEST_F(JsonObject_PrettyPrintTo_Tests, TwoMembers) {
  _object["key1"] = "value1";
  _object["key2"] = "value2";

  outputMustBe(
      "{\r\n"
      "  \"key1\": \"value1\",\r\n"
      "  \"key2\": \"value2\"\r\n"
      "}");
}

TEST_F(JsonObject_PrettyPrintTo_Tests, EmptyNestedContainers) {
  _object.createNestedObject("key1");
  _object.createNestedArray("key2");

  outputMustBe(
      "{\r\n"
      "  \"key1\": {},\r\n"
      "  \"key2\": []\r\n"
      "}");
}

TEST_F(JsonObject_PrettyPrintTo_Tests, NestedContainers) {
  JsonObject &nested1 = _object.createNestedObject("key1");
  nested1["a"] = 1;

  JsonArray &nested2 = _object.createNestedArray("key2");
  nested2.add(2);

  outputMustBe(
      "{\r\n"
      "  \"key1\": {\r\n"
      "    \"a\": 1\r\n"
      "  },\r\n"
      "  \"key2\": [\r\n"
      "    2\r\n"
      "  ]\r\n"
      "}");
}
