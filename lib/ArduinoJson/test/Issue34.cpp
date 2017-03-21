// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class Issue34 : public testing::Test {
 protected:
  template <typename T>
  void test_with_value(T expected) {
    StaticJsonBuffer<JSON_OBJECT_SIZE(1)> jsonBuffer;

    JsonObject& jsonObject = jsonBuffer.createObject();

    jsonObject["key"] = expected;
    T actual = jsonObject["key"];

    ASSERT_EQ(expected, actual);
  }
};

TEST_F(Issue34, int8_t) {
  test_with_value<int8_t>(1);
}

TEST_F(Issue34, uint8_t) {
  test_with_value<uint8_t>(2);
}

TEST_F(Issue34, int16_t) {
  test_with_value<int16_t>(3);
}

TEST_F(Issue34, uint16_t) {
  test_with_value<uint16_t>(4);
}

TEST_F(Issue34, int32_t) {
  test_with_value<int32_t>(5);
}

TEST_F(Issue34, uint32_t) {
  test_with_value<uint32_t>(6);
}

TEST_F(Issue34, float) {
  test_with_value<float>(7);
}

TEST_F(Issue34, double) {
  test_with_value<double>(8);
}
