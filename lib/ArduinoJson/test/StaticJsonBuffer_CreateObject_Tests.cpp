// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

TEST(StaticJsonBuffer_CreateObject_Tests, GrowsWithObject) {
  StaticJsonBuffer<JSON_OBJECT_SIZE(3)> buffer;

  JsonObject &obj = buffer.createObject();
  ASSERT_EQ(JSON_OBJECT_SIZE(0), buffer.size());

  obj["hello"];
  ASSERT_EQ(JSON_OBJECT_SIZE(0), buffer.size());

  obj["hello"] = 1;
  ASSERT_EQ(JSON_OBJECT_SIZE(1), buffer.size());

  obj["world"] = 2;
  ASSERT_EQ(JSON_OBJECT_SIZE(2), buffer.size());

  obj["world"] = 3;  // <- same key, should not grow
  ASSERT_EQ(JSON_OBJECT_SIZE(2), buffer.size());
}

TEST(StaticJsonBuffer_CreateObject_Tests, SucceedWhenBigEnough) {
  StaticJsonBuffer<JSON_OBJECT_SIZE(0)> buffer;

  JsonObject &object = buffer.createObject();
  ASSERT_TRUE(object.success());
}

TEST(StaticJsonBuffer_CreateObject_Tests, FailsWhenTooSmall) {
  StaticJsonBuffer<JSON_OBJECT_SIZE(0) - 1> buffer;

  JsonObject &object = buffer.createObject();
  ASSERT_FALSE(object.success());
}

TEST(StaticJsonBuffer_CreateObject_Tests, ObjectDoesntGrowWhenFull) {
  StaticJsonBuffer<JSON_OBJECT_SIZE(1)> buffer;

  JsonObject &obj = buffer.createObject();
  obj["hello"] = 1;
  obj["world"] = 2;

  ASSERT_EQ(JSON_OBJECT_SIZE(1), buffer.size());
  ASSERT_EQ(1, obj.size());

  char json[64];
  obj.printTo(json, sizeof(json));
  ASSERT_STREQ("{\"hello\":1}", json);
}
