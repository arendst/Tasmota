// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

TEST(DynamicJsonBuffer_Object_Tests, GrowsWithObject) {
  DynamicJsonBuffer json;

  JsonObject &obj = json.createObject();
  ASSERT_EQ(JSON_OBJECT_SIZE(0), json.size());

  obj["hello"] = 1;
  ASSERT_EQ(JSON_OBJECT_SIZE(1), json.size());

  obj["world"] = 2;
  ASSERT_EQ(JSON_OBJECT_SIZE(2), json.size());

  obj["world"] = 3;  // <- same key, should not grow
  ASSERT_EQ(JSON_OBJECT_SIZE(2), json.size());
}
