// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class JsonArray_PrettyPrintTo_Tests : public testing::Test {
 public:
  JsonArray_PrettyPrintTo_Tests() : array(jsonBuffer.createArray()) {}

 protected:
  DynamicJsonBuffer jsonBuffer;
  JsonArray& array;

  void outputMustBe(const char* expected) {
    char actual[256];

    size_t actualLen = array.prettyPrintTo(actual);
    size_t measuredLen = array.measurePrettyLength();

    EXPECT_STREQ(expected, actual);
    EXPECT_EQ(strlen(expected), actualLen);
    EXPECT_EQ(strlen(expected), measuredLen);
  }
};

TEST_F(JsonArray_PrettyPrintTo_Tests, Empty) {
  outputMustBe("[]");
}

TEST_F(JsonArray_PrettyPrintTo_Tests, OneElement) {
  array.add(1);

  outputMustBe(
      "[\r\n"
      "  1\r\n"
      "]");
}

TEST_F(JsonArray_PrettyPrintTo_Tests, TwoElements) {
  array.add(1);
  array.add(2);

  outputMustBe(
      "[\r\n"
      "  1,\r\n"
      "  2\r\n"
      "]");
}

TEST_F(JsonArray_PrettyPrintTo_Tests, EmptyNestedArrays) {
  array.createNestedArray();
  array.createNestedArray();

  outputMustBe(
      "[\r\n"
      "  [],\r\n"
      "  []\r\n"
      "]");
}

TEST_F(JsonArray_PrettyPrintTo_Tests, NestedArrays) {
  JsonArray& nested1 = array.createNestedArray();
  nested1.add(1);
  nested1.add(2);

  JsonObject& nested2 = array.createNestedObject();
  nested2["key"] = 3;

  outputMustBe(
      "[\r\n"
      "  [\r\n"
      "    1,\r\n"
      "    2\r\n"
      "  ],\r\n"
      "  {\r\n"
      "    \"key\": 3\r\n"
      "  }\r\n"
      "]");
}
