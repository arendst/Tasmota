// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

#define SUITE JsonVariant_Is_Tests

template <typename TTo, typename TFrom>
void assertIsNot(TFrom value) {
  JsonVariant variant = value;
  ASSERT_FALSE(variant.is<TTo>());
}

template <typename TTo>
void assertIsNot(JsonArray& value) {
  JsonVariant variant = value;
  ASSERT_FALSE(variant.is<TTo>());
}

template <typename TTo, typename TFrom>
void assertIs(TFrom value) {
  JsonVariant variant = value;
  ASSERT_TRUE(variant.is<TTo>());
}

template <typename TTo>
void assertIs(JsonArray& value) {
  JsonVariant variant = value;
  ASSERT_TRUE(variant.is<TTo>());
}

TEST(SUITE, ArrayIsArray) {
  DynamicJsonBuffer jsonBuffer;
  assertIs<JsonArray&>(jsonBuffer.createArray());
}
TEST(SUITE, ArrayIsNotBool) {
  DynamicJsonBuffer jsonBuffer;
  assertIsNot<bool>(jsonBuffer.createArray());
}
TEST(SUITE, ArrayIsNotDouble) {
  DynamicJsonBuffer jsonBuffer;
  assertIsNot<double>(jsonBuffer.createArray());
}
TEST(SUITE, ArrayIsNotFloat) {
  DynamicJsonBuffer jsonBuffer;
  assertIsNot<float>(jsonBuffer.createArray());
}
TEST(SUITE, ArrayIsNotInt) {
  DynamicJsonBuffer jsonBuffer;
  assertIsNot<int>(jsonBuffer.createArray());
}
TEST(SUITE, ArrayIsNotLong) {
  DynamicJsonBuffer jsonBuffer;
  assertIsNot<long>(jsonBuffer.createArray());
}
TEST(SUITE, ArrayIsNotString) {
  DynamicJsonBuffer jsonBuffer;
  assertIsNot<const char*>(jsonBuffer.createArray());
}

TEST(SUITE, BoolIsArray) {
  assertIsNot<JsonArray&>(true);
}
TEST(SUITE, BoolIsBool) {
  assertIs<bool>(true);
}
TEST(SUITE, BoolIsDouble) {
  assertIsNot<double>(true);
}
TEST(SUITE, BoolIsFloat) {
  assertIsNot<float>(true);
}
TEST(SUITE, BoolIsInt) {
  assertIsNot<int>(true);
}
TEST(SUITE, BoolIsLong) {
  assertIsNot<long>(true);
}
TEST(SUITE, BoolIsString) {
  assertIsNot<const char*>(true);
}

TEST(SUITE, DoubleIsArray) {
  assertIsNot<JsonArray&>(4.2);
}
TEST(SUITE, DoubleIsBool) {
  assertIsNot<bool>(4.2);
}
TEST(SUITE, DoubleIsDouble) {
  assertIs<double>(4.2);
}
TEST(SUITE, DoubleIsFloat) {
  assertIs<float>(4.2);
}
TEST(SUITE, DoubleIsInt) {
  assertIsNot<int>(4.2);
}
TEST(SUITE, DoubleIsLong) {
  assertIsNot<long>(4.2);
}
TEST(SUITE, DoubleIsString) {
  assertIsNot<const char*>(4.2);
}

TEST(SUITE, LongIsArray) {
  assertIsNot<JsonArray&>(42L);
}
TEST(SUITE, LongIsBool) {
  assertIsNot<bool>(42L);
}
TEST(SUITE, LongIsDouble) {
  assertIsNot<double>(42L);
}
TEST(SUITE, LongIsFloat) {
  assertIsNot<float>(42L);
}
TEST(SUITE, LongIsInt) {
  assertIs<int>(42L);
}
TEST(SUITE, LongIsLong) {
  assertIs<long>(42L);
}
TEST(SUITE, LongIsString) {
  assertIsNot<const char*>(42L);
}

TEST(SUITE, StringIsArray) {
  assertIsNot<JsonArray&>("42");
}
TEST(SUITE, StringIsBool) {
  assertIsNot<bool>("42");
}
TEST(SUITE, StringIsDouble) {
  assertIsNot<double>("42");
}
TEST(SUITE, StringIsFloat) {
  assertIsNot<float>("42");
}
TEST(SUITE, StringIsInt) {
  assertIsNot<int>("42");
}
TEST(SUITE, StringIsLong) {
  assertIsNot<long>("42");
}
TEST(SUITE, StringIsString) {
  assertIs<const char*>("42");
}

TEST(SUITE, UnparsedTrueIsArray) {
  assertIsNot<JsonArray&>(RawJson("true"));
}
TEST(SUITE, UnparsedTrueIsBool) {
  assertIs<bool>(RawJson("true"));
}
TEST(SUITE, UnparsedTrueIsDouble) {
  assertIsNot<double>(RawJson("true"));
}
TEST(SUITE, UnparsedTrueIsFloat) {
  assertIsNot<float>(RawJson("true"));
}
TEST(SUITE, UnparsedTrueIsInt) {
  assertIsNot<int>(RawJson("true"));
}
TEST(SUITE, UnparsedTrueIsLong) {
  assertIsNot<long>(RawJson("true"));
}
TEST(SUITE, UnparsedTrueIsString) {
  assertIsNot<const char*>(RawJson("true"));
}

TEST(SUITE, UnparsedFalseIsArray) {
  assertIsNot<JsonArray&>(RawJson("false"));
}
TEST(SUITE, UnparsedFalseIsBool) {
  assertIs<bool>(RawJson("false"));
}
TEST(SUITE, UnparsedFalseIsDouble) {
  assertIsNot<double>(RawJson("false"));
}
TEST(SUITE, UnparsedFalseIsFloat) {
  assertIsNot<float>(RawJson("false"));
}
TEST(SUITE, UnparsedFalseIsInt) {
  assertIsNot<int>(RawJson("false"));
}
TEST(SUITE, UnparsedFalseIsLong) {
  assertIsNot<long>(RawJson("false"));
}
TEST(SUITE, UnparsedFalseIsString) {
  assertIsNot<const char*>(RawJson("false"));
}

TEST(SUITE, UnparsedIntIsArray) {
  assertIsNot<JsonArray&>(RawJson("42"));
}
TEST(SUITE, UnparsedIntIsBool) {
  assertIsNot<bool>(RawJson("42"));
}
TEST(SUITE, UnparsedIntIsDouble) {
  assertIsNot<double>(RawJson("42"));
}
TEST(SUITE, UnparsedIntIsFloat) {
  assertIsNot<float>(RawJson("42"));
}
TEST(SUITE, UnparsedIntIsInt) {
  assertIs<int>(RawJson("42"));
}
TEST(SUITE, UnparsedIntIsLong) {
  assertIs<long>(RawJson("42"));
}
TEST(SUITE, UnparsedIntIsString) {
  assertIsNot<const char*>(RawJson("42"));
}

TEST(SUITE, UnparsedFloatIsBool) {
  assertIsNot<bool>(RawJson("4.2e-10"));
}
TEST(SUITE, UnparsedFloatIsDouble) {
  assertIs<double>(RawJson("4.2e-10"));
}
TEST(SUITE, UnparsedFloatIsFloat) {
  assertIs<float>(RawJson("4.2e-10"));
}
TEST(SUITE, UnparsedFloatIsInt) {
  assertIsNot<int>(RawJson("4.2e-10"));
}
TEST(SUITE, UnparsedFloatIsLong) {
  assertIsNot<long>(RawJson("4.2e-10"));
}
TEST(SUITE, UnparsedFloatIsStr) {
  assertIsNot<const char*>(RawJson("4.2"));
}

TEST(SUITE, UnparsedNullIsArray) {
  assertIsNot<JsonArray&>(RawJson("null"));
}
TEST(SUITE, UnparsedNullIsBool) {
  assertIsNot<bool>(RawJson("null"));
}
TEST(SUITE, UnparsedNullIsDouble) {
  assertIsNot<double>(RawJson("null"));
}
TEST(SUITE, UnparsedNullIsFloat) {
  assertIsNot<float>(RawJson("null"));
}
TEST(SUITE, UnparsedNullIsInt) {
  assertIsNot<int>(RawJson("null"));
}
TEST(SUITE, UnparsedNullIsLong) {
  assertIsNot<long>(RawJson("null"));
}
TEST(SUITE, UnparsedNullIsConstCharPtr) {
  assertIs<const char*>(RawJson("null"));
}
TEST(SUITE, UnparsedNullIsCharPtr) {
  assertIs<char*>(RawJson("null"));
}
