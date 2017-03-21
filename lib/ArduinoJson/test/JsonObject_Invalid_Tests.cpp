// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

TEST(JsonObject_Invalid_Tests, SubscriptFails) {
  ASSERT_FALSE(JsonObject::invalid()["key"].success());
}

TEST(JsonObject_Invalid_Tests, AddFails) {
  JsonObject& object = JsonObject::invalid();
  object.set("hello", "world");
  ASSERT_EQ(0, object.size());
}

TEST(JsonObject_Invalid_Tests, CreateNestedArrayFails) {
  ASSERT_FALSE(JsonObject::invalid().createNestedArray("hello").success());
}

TEST(JsonObject_Invalid_Tests, CreateNestedObjectFails) {
  ASSERT_FALSE(JsonObject::invalid().createNestedObject("world").success());
}

TEST(JsonObject_Invalid_Tests, PrintToWritesBraces) {
  char buffer[32];
  JsonObject::invalid().printTo(buffer, sizeof(buffer));
  ASSERT_STREQ("{}", buffer);
}
