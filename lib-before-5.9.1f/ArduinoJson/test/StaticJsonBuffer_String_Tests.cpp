// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

TEST(StaticJsonBuffer_String_Tests, WorksWhenBufferIsBigEnough) {
  StaticJsonBuffer<6> jsonBuffer;

  StaticJsonBufferBase::String str = jsonBuffer.startString();
  str.append('h');
  str.append('e');
  str.append('l');
  str.append('l');
  str.append('o');

  ASSERT_STREQ("hello", str.c_str());
}

TEST(StaticJsonBuffer_String_Tests, ReturnsNullWhenTooSmall) {
  StaticJsonBuffer<5> jsonBuffer;

  StaticJsonBufferBase::String str = jsonBuffer.startString();
  str.append('h');
  str.append('e');
  str.append('l');
  str.append('l');
  str.append('o');

  ASSERT_EQ(NULL, str.c_str());
}

TEST(StaticJsonBuffer_String_Tests, SizeIncreases) {
  StaticJsonBuffer<5> jsonBuffer;

  StaticJsonBufferBase::String str = jsonBuffer.startString();
  ASSERT_EQ(0, jsonBuffer.size());

  str.append('h');
  ASSERT_EQ(1, jsonBuffer.size());

  str.c_str();
  ASSERT_EQ(2, jsonBuffer.size());
}
