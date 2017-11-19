// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class NoMemoryAllocator {
 public:
  void* allocate(size_t) {
    return NULL;
  }
  void deallocate(void*) {}
};

class DynamicJsonBuffer_NoMemory_Tests : public ::testing::Test {
 protected:
  DynamicJsonBufferBase<NoMemoryAllocator> _jsonBuffer;
};

TEST_F(DynamicJsonBuffer_NoMemory_Tests, FixCodeCoverage) {
  // call this function to fix code coverage
  NoMemoryAllocator().deallocate(NULL);
}

TEST_F(DynamicJsonBuffer_NoMemory_Tests, CreateArray) {
  ASSERT_FALSE(_jsonBuffer.createArray().success());
}

TEST_F(DynamicJsonBuffer_NoMemory_Tests, CreateObject) {
  ASSERT_FALSE(_jsonBuffer.createObject().success());
}

TEST_F(DynamicJsonBuffer_NoMemory_Tests, ParseArray) {
  char json[] = "[]";
  ASSERT_FALSE(_jsonBuffer.parseArray(json).success());
}

TEST_F(DynamicJsonBuffer_NoMemory_Tests, ParseObject) {
  char json[] = "{}";
  ASSERT_FALSE(_jsonBuffer.parseObject(json).success());
}

TEST_F(DynamicJsonBuffer_NoMemory_Tests, String) {
  DynamicJsonBufferBase<NoMemoryAllocator>::String str = _jsonBuffer.startString();
  str.append('!');
  ASSERT_EQ(NULL, str.c_str());
}
