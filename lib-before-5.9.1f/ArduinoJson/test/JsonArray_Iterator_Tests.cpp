// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

template <typename TIterator>
static void run_iterator_test() {
  StaticJsonBuffer<JSON_ARRAY_SIZE(2)> jsonBuffer;

  JsonArray &array = jsonBuffer.createArray();
  array.add(12);
  array.add(34);

  TIterator it = array.begin();
  TIterator end = array.end();

  EXPECT_NE(end, it);
  EXPECT_EQ(12, it->template as<int>());
  EXPECT_EQ(12, static_cast<int>(*it));
  ++it;
  EXPECT_NE(end, it);
  EXPECT_EQ(34, it->template as<int>());
  EXPECT_EQ(34, static_cast<int>(*it));
  ++it;
  EXPECT_EQ(end, it);
}

TEST(JsonArray_Iterator_Test, RunItertorToEnd) {
  run_iterator_test<JsonArray::iterator>();
}

TEST(JsonArray_Iterator_Test, RunConstItertorToEnd) {
  run_iterator_test<JsonArray::const_iterator>();
}
