// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

TEST(JsonArray_CopyTo_Tests, BiggerOneDimensionIntegerArray) {
  char json[] = "[1,2,3]";

  DynamicJsonBuffer jsonBuffer;
  JsonArray& array = jsonBuffer.parseArray(json);

  int destination[4] = {0};
  size_t result = array.copyTo(destination);

  ASSERT_EQ(3, result);
  ASSERT_EQ(1, destination[0]);
  ASSERT_EQ(2, destination[1]);
  ASSERT_EQ(3, destination[2]);
  ASSERT_EQ(0, destination[3]);
}

TEST(JsonArray_CopyTo_Tests, SmallerOneDimensionIntegerArray) {
  char json[] = "[1,2,3]";

  DynamicJsonBuffer jsonBuffer;
  JsonArray& array = jsonBuffer.parseArray(json);

  int destination[2] = {0};
  size_t result = array.copyTo(destination);

  ASSERT_EQ(2, result);
  ASSERT_EQ(1, destination[0]);
  ASSERT_EQ(2, destination[1]);
}

TEST(JsonArray_CopyTo_Tests, TwoOneDimensionIntegerArray) {
  char json[] = "[[1,2],[3],[4]]";

  DynamicJsonBuffer jsonBuffer;
  JsonArray& array = jsonBuffer.parseArray(json);

  int destination[3][2] = {{0}};
  array.copyTo(destination);

  ASSERT_EQ(1, destination[0][0]);
  ASSERT_EQ(2, destination[0][1]);
  ASSERT_EQ(3, destination[1][0]);
  ASSERT_EQ(0, destination[1][1]);
  ASSERT_EQ(4, destination[2][0]);
  ASSERT_EQ(0, destination[2][1]);
}
