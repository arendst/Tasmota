// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class DynamicJsonBuffer_Basic_Tests : public testing::Test {
 protected:
  DynamicJsonBuffer buffer;
};

TEST_F(DynamicJsonBuffer_Basic_Tests, InitialSizeIsZero) {
  ASSERT_EQ(0, buffer.size());
}

TEST_F(DynamicJsonBuffer_Basic_Tests, SizeIncreasesAfterAlloc) {
  buffer.alloc(1);
  ASSERT_LE(1U, buffer.size());
  buffer.alloc(1);
  ASSERT_LE(2U, buffer.size());
}

TEST_F(DynamicJsonBuffer_Basic_Tests, ReturnDifferentPointer) {
  void* p1 = buffer.alloc(1);
  void* p2 = buffer.alloc(2);
  ASSERT_NE(p1, p2);
}

static bool isAligned(void* ptr) {
  const size_t mask = sizeof(void*) - 1;
  size_t addr = reinterpret_cast<size_t>(ptr);
  return (addr & mask) == 0;
}

TEST_F(DynamicJsonBuffer_Basic_Tests, Alignment) {
  // make room for tow but not three
  buffer = DynamicJsonBuffer(2 * sizeof(void*) + 1);

  ASSERT_TRUE(isAligned(buffer.alloc(1)));  // this on is aligned by design
  ASSERT_TRUE(isAligned(buffer.alloc(1)));  // this one fits in the first block
  ASSERT_TRUE(isAligned(buffer.alloc(1)));  // this one requires a new block
}

TEST_F(DynamicJsonBuffer_Basic_Tests, strdup) {
  char original[] = "hello";
  char* copy = buffer.strdup(original);
  strcpy(original, "world");
  ASSERT_STREQ("hello", copy);
}

TEST_F(DynamicJsonBuffer_Basic_Tests, strdup_givenNull) {
  const char* original = NULL;
  char* copy = buffer.strdup(original);
  ASSERT_EQ(NULL, copy);
}
