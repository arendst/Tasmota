// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

TEST(JsonArray_CopyFrom_Tests, OneDimension) {
  DynamicJsonBuffer jsonBuffer;
  JsonArray& array = jsonBuffer.createArray();
  char json[32];
  int source[] = {1, 2, 3};

  bool ok = array.copyFrom(source);
  ASSERT_TRUE(ok);

  array.printTo(json, sizeof(json));
  ASSERT_STREQ("[1,2,3]", json);
}

TEST(JsonArray_CopyFrom_Tests, OneDimension_JsonBufferTooSmall) {
  const size_t SIZE = JSON_ARRAY_SIZE(2);
  StaticJsonBuffer<SIZE> jsonBuffer;
  JsonArray& array = jsonBuffer.createArray();
  char json[32];
  int source[] = {1, 2, 3};

  bool ok = array.copyFrom(source);
  ASSERT_FALSE(ok);

  array.printTo(json, sizeof(json));
  ASSERT_STREQ("[1,2]", json);
}

TEST(JsonArray_CopyFrom_Tests, TwoDimensions) {
  DynamicJsonBuffer jsonBuffer;
  JsonArray& array = jsonBuffer.createArray();
  char json[32];
  int source[][3] = {{1, 2, 3}, {4, 5, 6}};

  bool ok = array.copyFrom(source);
  ASSERT_TRUE(ok);

  array.printTo(json, sizeof(json));
  ASSERT_STREQ("[[1,2,3],[4,5,6]]", json);
}

TEST(JsonArray_CopyFrom_Tests, TwoDimensions_JsonBufferTooSmall) {
  const size_t SIZE =
      JSON_ARRAY_SIZE(2) + JSON_ARRAY_SIZE(3) + JSON_ARRAY_SIZE(2);
  StaticJsonBuffer<SIZE> jsonBuffer;
  JsonArray& array = jsonBuffer.createArray();
  char json[32];
  int source[][3] = {{1, 2, 3}, {4, 5, 6}};

  bool ok = array.copyFrom(source);
  ASSERT_FALSE(ok);

  array.printTo(json, sizeof(json));
  ASSERT_STREQ("[[1,2,3],[4,5]]", json);
}
