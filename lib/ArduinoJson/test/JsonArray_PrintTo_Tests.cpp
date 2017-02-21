// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class JsonArray_PrintTo_Tests : public testing::Test {
 public:
  JsonArray_PrintTo_Tests() : array(json.createArray()) {}

 protected:
  StaticJsonBuffer<JSON_ARRAY_SIZE(2)> json;
  JsonArray &array;

  void outputMustBe(const char *expected) {
    size_t actualLen = array.printTo(buffer);
    size_t measuredLen = array.measureLength();

    EXPECT_STREQ(expected, buffer);
    EXPECT_EQ(strlen(expected), actualLen);
    EXPECT_EQ(strlen(expected), measuredLen);
  }

 private:
  char buffer[256];
};

TEST_F(JsonArray_PrintTo_Tests, Empty) {
  outputMustBe("[]");
}

TEST_F(JsonArray_PrintTo_Tests, Null) {
  array.add(static_cast<char *>(0));

  outputMustBe("[null]");
}

TEST_F(JsonArray_PrintTo_Tests, OneString) {
  array.add("hello");

  outputMustBe("[\"hello\"]");
}

TEST_F(JsonArray_PrintTo_Tests, TwoStrings) {
  array.add("hello");
  array.add("world");

  outputMustBe("[\"hello\",\"world\"]");
}

TEST_F(JsonArray_PrintTo_Tests, OneStringOverCapacity) {
  array.add("hello");
  array.add("world");
  array.add("lost");

  outputMustBe("[\"hello\",\"world\"]");
}

TEST_F(JsonArray_PrintTo_Tests, OneDoubleDefaultDigits) {
  array.add(3.14159265358979323846);
  outputMustBe("[3.14]");
}

TEST_F(JsonArray_PrintTo_Tests, OneDoubleFourDigits) {
  array.add(3.14159265358979323846, 4);
  outputMustBe("[3.1416]");
}

TEST_F(JsonArray_PrintTo_Tests, OneDoubleFourDigits_AlternativeSyntax) {
  array.add(double_with_n_digits(3.14159265358979323846, 4));
  outputMustBe("[3.1416]");
}

TEST_F(JsonArray_PrintTo_Tests, OneFloatDefaultDigits) {
  array.add(3.14159f);
  outputMustBe("[3.14]");
}

TEST_F(JsonArray_PrintTo_Tests, OneFloatFourDigits) {
  array.add(3.14159f, 4);
  outputMustBe("[3.1416]");
}

TEST_F(JsonArray_PrintTo_Tests, OneInteger) {
  array.add(1);

  outputMustBe("[1]");
}

TEST_F(JsonArray_PrintTo_Tests, TwoIntegers) {
  array.add(1);
  array.add(2);

  outputMustBe("[1,2]");
}

TEST_F(JsonArray_PrintTo_Tests, RawJson) {
  array.add(RawJson("{\"key\":\"value\"}"));

  outputMustBe("[{\"key\":\"value\"}]");
}

TEST_F(JsonArray_PrintTo_Tests, OneIntegerOverCapacity) {
  array.add(1);
  array.add(2);
  array.add(3);

  outputMustBe("[1,2]");
}

TEST_F(JsonArray_PrintTo_Tests, OneTrue) {
  array.add(true);

  outputMustBe("[true]");
}

TEST_F(JsonArray_PrintTo_Tests, OneFalse) {
  array.add(false);

  outputMustBe("[false]");
}

TEST_F(JsonArray_PrintTo_Tests, TwoBooleans) {
  array.add(false);
  array.add(true);

  outputMustBe("[false,true]");
}

TEST_F(JsonArray_PrintTo_Tests, OneBooleanOverCapacity) {
  array.add(false);
  array.add(true);
  array.add(false);

  outputMustBe("[false,true]");
}

TEST_F(JsonArray_PrintTo_Tests, OneEmptyNestedArray) {
  array.createNestedArray();

  outputMustBe("[[]]");
}

TEST_F(JsonArray_PrintTo_Tests, OneEmptyNestedHash) {
  array.createNestedObject();

  outputMustBe("[{}]");
}
