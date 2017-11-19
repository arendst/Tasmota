// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class StaticJsonBuffer_ParseObject_Tests : public testing::Test {
 protected:
  void with(StaticJsonBufferBase& jsonBuffer) {
    _jsonBuffer = &jsonBuffer;
  }

  void whenInputIs(const char* json) {
    strcpy(_jsonString, json);
  }

  void parseMustSucceed() {
    EXPECT_TRUE(_jsonBuffer->parseObject(_jsonString).success());
  }

  void parseMustFail() {
    EXPECT_FALSE(_jsonBuffer->parseObject(_jsonString).success());
  }

 private:
  StaticJsonBufferBase* _jsonBuffer;
  char _jsonString[256];
};

TEST_F(StaticJsonBuffer_ParseObject_Tests, TooSmallBufferForEmptyObject) {
  StaticJsonBuffer<JSON_OBJECT_SIZE(0) - 1> bufferTooSmall;
  with(bufferTooSmall);
  whenInputIs("{}");
  parseMustFail();
}

TEST_F(StaticJsonBuffer_ParseObject_Tests, BufferOfTheRightSizeForEmptyObject) {
  StaticJsonBuffer<JSON_OBJECT_SIZE(0)> bufferOfRightSize;
  with(bufferOfRightSize);
  whenInputIs("{}");
  parseMustSucceed();
}

TEST_F(StaticJsonBuffer_ParseObject_Tests,
       TooSmallBufferForObjectWithOneValue) {
  StaticJsonBuffer<JSON_OBJECT_SIZE(1) - 1> bufferTooSmall;
  with(bufferTooSmall);
  whenInputIs("{\"a\":1}");
  parseMustFail();
}

TEST_F(StaticJsonBuffer_ParseObject_Tests,
       BufferOfTheRightSizeForObjectWithOneValue) {
  StaticJsonBuffer<JSON_OBJECT_SIZE(1)> bufferOfRightSize;
  with(bufferOfRightSize);
  whenInputIs("{\"a\":1}");
  parseMustSucceed();
}

TEST_F(StaticJsonBuffer_ParseObject_Tests,
       TooSmallBufferForObjectWithNestedObject) {
  StaticJsonBuffer<JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(0) - 1> bufferTooSmall;
  with(bufferTooSmall);
  whenInputIs("{\"a\":[]}");
  parseMustFail();
}

TEST_F(StaticJsonBuffer_ParseObject_Tests,
       BufferOfTheRightSizeForObjectWithNestedObject) {
  StaticJsonBuffer<JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(0)> bufferOfRightSize;
  with(bufferOfRightSize);
  whenInputIs("{\"a\":[]}");
  parseMustSucceed();
}

TEST_F(StaticJsonBuffer_ParseObject_Tests, CharPtrNull) {
  ASSERT_FALSE(
      StaticJsonBuffer<100>().parseObject(static_cast<char*>(0)).success());
}

TEST_F(StaticJsonBuffer_ParseObject_Tests, ConstCharPtrNull) {
  ASSERT_FALSE(StaticJsonBuffer<100>()
                   .parseObject(static_cast<const char*>(0))
                   .success());
}
