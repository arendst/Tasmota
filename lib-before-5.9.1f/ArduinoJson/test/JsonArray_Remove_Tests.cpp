// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class JsonArray_Remove_Tests : public ::testing::Test {
 protected:
  JsonArray_Remove_Tests() : _array(_jsonBuffer.createArray()) {
    _array.add("one");
    _array.add("two");
    _array.add("three");
  }

  DynamicJsonBuffer _jsonBuffer;
  JsonArray& _array;
};

#define TEST_(name) TEST_F(JsonArray_Remove_Tests, name)

TEST_(RemoveFirstElement) {
  _array.removeAt(0);

  EXPECT_EQ(2, _array.size());
  EXPECT_STREQ("two", _array[0]);
  EXPECT_STREQ("three", _array[1]);
}

TEST_(RemoveMiddleElement) {
  _array.removeAt(1);

  EXPECT_EQ(2, _array.size());
  EXPECT_STREQ("one", _array[0]);
  EXPECT_STREQ("three", _array[1]);
}

TEST_(RemoveLastElement) {
  _array.removeAt(2);

  EXPECT_EQ(2, _array.size());
  EXPECT_STREQ("one", _array[0]);
  EXPECT_STREQ("two", _array[1]);
}
