// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class StaticJsonBuffer_Basic_Tests : public testing::Test {
 protected:
  StaticJsonBuffer<64> buffer;
};

TEST_F(StaticJsonBuffer_Basic_Tests, CapacityMatchTemplateParameter) {
  ASSERT_EQ(64, buffer.capacity());
}

TEST_F(StaticJsonBuffer_Basic_Tests, InitialSizeIsZero) {
  ASSERT_EQ(0, buffer.size());
}

TEST_F(StaticJsonBuffer_Basic_Tests, GrowsAfterAlloc) {
  buffer.alloc(1);
  ASSERT_LE(1U, buffer.size());
  buffer.alloc(1);
  ASSERT_LE(2U, buffer.size());
}

TEST_F(StaticJsonBuffer_Basic_Tests, DoesntGrowWhenFull) {
  buffer.alloc(64);
  buffer.alloc(1);
  ASSERT_EQ(64, buffer.size());
}

TEST_F(StaticJsonBuffer_Basic_Tests, DoesntGrowWhenTooSmall) {
  buffer.alloc(65);
  ASSERT_EQ(0, buffer.size());
}

TEST_F(StaticJsonBuffer_Basic_Tests, ReturnsNonNull) {
  void *p = buffer.alloc(64);
  ASSERT_NE(static_cast<void *>(0), p);
}

TEST_F(StaticJsonBuffer_Basic_Tests, ReturnsNullWhenFull) {
  buffer.alloc(64);
  void *p = buffer.alloc(1);
  ASSERT_EQ(NULL, p);
}

TEST_F(StaticJsonBuffer_Basic_Tests, ReturnsNullWhenTooSmall) {
  void *p = buffer.alloc(65);
  ASSERT_EQ(NULL, p);
}

TEST_F(StaticJsonBuffer_Basic_Tests, Alignment) {
  size_t mask = sizeof(void *) - 1;

  for (size_t size = 1; size <= sizeof(void *); size++) {
    size_t addr = reinterpret_cast<size_t>(buffer.alloc(1));
    ASSERT_EQ(0, addr & mask);
  }
}
