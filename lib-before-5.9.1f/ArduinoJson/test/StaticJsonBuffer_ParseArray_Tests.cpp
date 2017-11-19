// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class StaticJsonBuffer_ParseArray_Tests : public testing::Test {
 protected:
  void with(StaticJsonBufferBase& jsonBuffer) {
    _jsonBuffer = &jsonBuffer;
  }

  void whenInputIs(const char* json) {
    strcpy(_jsonString, json);
  }

  void parseMustSucceed() {
    EXPECT_TRUE(_jsonBuffer->parseArray(_jsonString).success());
  }

  void parseMustFail() {
    EXPECT_FALSE(_jsonBuffer->parseArray(_jsonString).success());
  }

 private:
  StaticJsonBufferBase* _jsonBuffer;
  char _jsonString[256];
};

TEST_F(StaticJsonBuffer_ParseArray_Tests, TooSmallBufferForEmptyArray) {
  StaticJsonBuffer<JSON_ARRAY_SIZE(0) - 1> bufferTooSmall;
  with(bufferTooSmall);
  whenInputIs("[]");
  parseMustFail();
}

TEST_F(StaticJsonBuffer_ParseArray_Tests, BufferOfTheRightSizeForEmptyArray) {
  StaticJsonBuffer<JSON_ARRAY_SIZE(0)> bufferOfRightSize;
  with(bufferOfRightSize);
  whenInputIs("[]");
  parseMustSucceed();
}

TEST_F(StaticJsonBuffer_ParseArray_Tests, TooSmallBufferForArrayWithOneValue) {
  StaticJsonBuffer<JSON_ARRAY_SIZE(1) - 1> bufferTooSmall;
  with(bufferTooSmall);
  whenInputIs("[1]");
  parseMustFail();
}

TEST_F(StaticJsonBuffer_ParseArray_Tests,
       BufferOfTheRightSizeForArrayWithOneValue) {
  StaticJsonBuffer<JSON_ARRAY_SIZE(1)> bufferOfRightSize;
  with(bufferOfRightSize);
  whenInputIs("[1]");
  parseMustSucceed();
}

TEST_F(StaticJsonBuffer_ParseArray_Tests,
       TooSmallBufferForArrayWithNestedObject) {
  StaticJsonBuffer<JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(0) - 1> bufferTooSmall;
  with(bufferTooSmall);
  whenInputIs("[{}]");
  parseMustFail();
}

TEST_F(StaticJsonBuffer_ParseArray_Tests,
       BufferOfTheRightSizeForArrayWithNestedObject) {
  StaticJsonBuffer<JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(0)> bufferOfRightSize;
  with(bufferOfRightSize);
  whenInputIs("[{}]");
  parseMustSucceed();
}

TEST_F(StaticJsonBuffer_ParseArray_Tests, CharPtrNull) {
  ASSERT_FALSE(
      StaticJsonBuffer<100>().parseArray(static_cast<char*>(0)).success());
}

TEST_F(StaticJsonBuffer_ParseArray_Tests, ConstCharPtrNull) {
  ASSERT_FALSE(StaticJsonBuffer<100>()
                   .parseArray(static_cast<const char*>(0))
                   .success());
}

TEST_F(StaticJsonBuffer_ParseArray_Tests, CopyStringNotSpaces) {
  StaticJsonBuffer<100> jsonBuffer;
  jsonBuffer.parseArray("  [ \"1234567\" ] ");
  ASSERT_EQ(JSON_ARRAY_SIZE(1) + sizeof("1234567"), jsonBuffer.size());
  // note we use a string of 8 bytes to be sure that the StaticJsonBuffer
  // will not insert bytes to enforce alignement
}
