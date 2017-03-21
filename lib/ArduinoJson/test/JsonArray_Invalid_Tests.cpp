// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

TEST(JsonArray_Invalid_Tests, SubscriptFails) {
  ASSERT_FALSE(JsonArray::invalid()[0].success());
}

TEST(JsonArray_Invalid_Tests, AddFails) {
  JsonArray& array = JsonArray::invalid();
  array.add(1);
  ASSERT_EQ(0, array.size());
}

TEST(JsonArray_Invalid_Tests, CreateNestedArrayFails) {
  ASSERT_FALSE(JsonArray::invalid().createNestedArray().success());
}

TEST(JsonArray_Invalid_Tests, CreateNestedObjectFails) {
  ASSERT_FALSE(JsonArray::invalid().createNestedObject().success());
}

TEST(JsonArray_Invalid_Tests, PrintToWritesBrackets) {
  char buffer[32];
  JsonArray::invalid().printTo(buffer, sizeof(buffer));
  ASSERT_STREQ("[]", buffer);
}
