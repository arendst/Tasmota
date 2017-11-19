// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class JsonVariant_Comparison_Tests : public ::testing::Test {
 protected:
  template <typename T>
  void testValue(T low, T mid, T high) {
    setValueTo(mid);
    mustBeEqualTo(mid);
    mustBeGreaterThan(low);
    mustBeLessThan(high);
  }

 private:
  template <typename T>
  void setValueTo(T expected) {
    actual = expected;
  }

  template <typename T>
  void mustBeEqualTo(T expected) {
    EXPECT_EQ(expected, actual);  // operator==
    EXPECT_EQ(actual, expected);  // operator==
    EXPECT_LE(expected, actual);  // operator<=
    EXPECT_LE(actual, expected);  // operator<=
    EXPECT_GE(expected, actual);  // operator>=
    EXPECT_GE(actual, expected);  // operator>=
  }

  template <typename T>
  void mustBeGreaterThan(T expected) {
    EXPECT_GT(actual, expected);  // operator>
    EXPECT_LT(expected, actual);  // operator<
    EXPECT_NE(actual, expected);  // operator!=
    EXPECT_NE(expected, actual);  // operator!=
  }

  template <typename T>
  void mustBeLessThan(T expected) {
    EXPECT_LT(actual, expected);  // operator<
    EXPECT_GT(expected, actual);  // operator<
    EXPECT_NE(actual, expected);  // operator!=
    EXPECT_NE(expected, actual);  // operator!=
  }

  JsonVariant actual;
};

TEST_F(JsonVariant_Comparison_Tests, Double) {
  testValue<double>(123.44, 123.45, 123.46);
}

TEST_F(JsonVariant_Comparison_Tests, Float) {
  testValue<float>(123.44f, 123.45f, 123.46f);
}

TEST_F(JsonVariant_Comparison_Tests, SChar) {
  testValue<signed char>(122, 123, 124);
}

TEST_F(JsonVariant_Comparison_Tests, SInt) {
  testValue<signed int>(122, 123, 124);
}

TEST_F(JsonVariant_Comparison_Tests, SLong) {
  testValue<signed long>(122L, 123L, 124L);
}

TEST_F(JsonVariant_Comparison_Tests, SShort) {
  testValue<signed short>(122, 123, 124);
}

TEST_F(JsonVariant_Comparison_Tests, UChar) {
  testValue<unsigned char>(122, 123, 124);
}

TEST_F(JsonVariant_Comparison_Tests, UInt) {
  testValue<unsigned int>(122, 123, 124);
}

TEST_F(JsonVariant_Comparison_Tests, ULong) {
  testValue<unsigned long>(122L, 123L, 124L);
}

TEST_F(JsonVariant_Comparison_Tests, UShort) {
  testValue<unsigned short>(122, 123, 124);
}

TEST_F(JsonVariant_Comparison_Tests, StringLiteral) {
  DynamicJsonBuffer jsonBuffer;
  JsonVariant variant = jsonBuffer.parse("\"hello\"");

  ASSERT_TRUE(variant == "hello");
  ASSERT_FALSE(variant != "hello");

  ASSERT_TRUE(variant != "world");
  ASSERT_FALSE(variant == "world");

  ASSERT_TRUE("hello" == variant);
  ASSERT_FALSE("hello" != variant);

  ASSERT_TRUE("world" != variant);
  ASSERT_FALSE("world" == variant);
}

TEST_F(JsonVariant_Comparison_Tests, String) {
  DynamicJsonBuffer jsonBuffer;
  JsonVariant variant = jsonBuffer.parse("\"hello\"");

  ASSERT_TRUE(variant == std::string("hello"));
  ASSERT_FALSE(variant != std::string("hello"));

  ASSERT_TRUE(variant != std::string("world"));
  ASSERT_FALSE(variant == std::string("world"));

  ASSERT_TRUE(std::string("hello") == variant);
  ASSERT_FALSE(std::string("hello") != variant);

  ASSERT_TRUE(std::string("world") != variant);
  ASSERT_FALSE(std::string("world") == variant);
}

TEST_F(JsonVariant_Comparison_Tests, IntegerInVariant) {
  JsonVariant variant1 = 42;
  JsonVariant variant2 = 42;
  JsonVariant variant3 = 666;

  ASSERT_TRUE(variant1 == variant2);
  ASSERT_FALSE(variant1 != variant2);

  ASSERT_TRUE(variant1 != variant3);
  ASSERT_FALSE(variant1 == variant3);
}

TEST_F(JsonVariant_Comparison_Tests, StringInVariant) {
  JsonVariant variant1 = "0hello" + 1;  // make sure they have
  JsonVariant variant2 = "1hello" + 1;  // different addresses
  JsonVariant variant3 = "world";

  ASSERT_TRUE(variant1 == variant2);
  ASSERT_FALSE(variant1 != variant2);

  ASSERT_TRUE(variant1 != variant3);
  ASSERT_FALSE(variant1 == variant3);
}

TEST_F(JsonVariant_Comparison_Tests, DoubleInVariant) {
  JsonVariant variant1 = 42.0;
  JsonVariant variant2 = 42.0;
  JsonVariant variant3 = 666.0;

  ASSERT_TRUE(variant1 == variant2);
  ASSERT_FALSE(variant1 != variant2);

  ASSERT_TRUE(variant1 != variant3);
  ASSERT_FALSE(variant1 == variant3);
}

TEST_F(JsonVariant_Comparison_Tests, BoolInVariant) {
  JsonVariant variant1 = true;
  JsonVariant variant2 = true;
  JsonVariant variant3 = false;

  ASSERT_TRUE(variant1 == variant2);
  ASSERT_FALSE(variant1 != variant2);

  ASSERT_TRUE(variant1 != variant3);
  ASSERT_FALSE(variant1 == variant3);
}

TEST_F(JsonVariant_Comparison_Tests, ArrayInVariant) {
  DynamicJsonBuffer jsonBuffer;
  JsonArray& array1 = jsonBuffer.createArray();
  JsonArray& array2 = jsonBuffer.createArray();

  JsonVariant variant1 = array1;
  JsonVariant variant2 = array1;
  JsonVariant variant3 = array2;

  ASSERT_TRUE(variant1 == variant2);
  ASSERT_FALSE(variant1 != variant2);

  ASSERT_TRUE(variant1 != variant3);
  ASSERT_FALSE(variant1 == variant3);
}

TEST_F(JsonVariant_Comparison_Tests, ObjectInVariant) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& obj1 = jsonBuffer.createObject();
  JsonObject& obj2 = jsonBuffer.createObject();

  JsonVariant variant1 = obj1;
  JsonVariant variant2 = obj1;
  JsonVariant variant3 = obj2;

  ASSERT_TRUE(variant1 == variant2);
  ASSERT_FALSE(variant1 != variant2);

  ASSERT_TRUE(variant1 != variant3);
  ASSERT_FALSE(variant1 == variant3);
}

TEST_F(JsonVariant_Comparison_Tests, VariantsOfDifferentTypes) {
  DynamicJsonBuffer jsonBuffer;
  JsonVariant variants[] = {
      true,
      42,
      666.667,
      "hello",
      jsonBuffer.createArray(),
      jsonBuffer.createObject(),
  };
  size_t n = sizeof(variants) / sizeof(variants[0]);

  for (size_t i = 0; i < n; i++) {
    for (size_t j = i + 1; j < n; j++) {
      ASSERT_TRUE(variants[i] != variants[j]);
      ASSERT_FALSE(variants[i] == variants[j]);
    }
  }
}
