// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>

#include <gtest/gtest.h>
#include <stdint.h>

static const char* null = 0;

TEST(JsonVariant_As_Tests, DoubleAsBool) {
  JsonVariant variant = 4.2;
  ASSERT_TRUE(variant.as<bool>());
}

TEST(JsonVariant_As_Tests, DoubleAsCstr) {
  JsonVariant variant = 4.2;
  ASSERT_FALSE(variant.as<const char*>());
}

TEST(JsonVariant_As_Tests, DoubleAsString) {
  JsonVariant variant = 4.2;
  ASSERT_EQ(std::string("4.20"), variant.as<std::string>());
}

TEST(JsonVariant_As_Tests, DoubleAsLong) {
  JsonVariant variant = 4.2;
  ASSERT_EQ(4L, variant.as<long>());
}

TEST(JsonVariant_As_Tests, DoubleAsUnsigned) {
  JsonVariant variant = 4.2;
  ASSERT_EQ(4U, variant.as<unsigned>());
}

TEST(JsonVariant_As_Tests, DoubleZeroAsBool) {
  JsonVariant variant = 0.0;
  ASSERT_FALSE(variant.as<bool>());
}

TEST(JsonVariant_As_Tests, DoubleZeroAsLong) {
  JsonVariant variant = 0.0;
  ASSERT_EQ(0L, variant.as<long>());
}

TEST(JsonVariant_As_Tests, FalseAsBool) {
  JsonVariant variant = false;
  ASSERT_FALSE(variant.as<bool>());
}

TEST(JsonVariant_As_Tests, FalseAsDouble) {
  JsonVariant variant = false;
  ASSERT_EQ(0.0, variant.as<double>());
}

TEST(JsonVariant_As_Tests, FalseAsLong) {
  JsonVariant variant = false;
  ASSERT_EQ(0L, variant.as<long>());
}

TEST(JsonVariant_As_Tests, FalseAsString) {
  JsonVariant variant = false;
  ASSERT_EQ(std::string("false"), variant.as<std::string>());
}

TEST(JsonVariant_As_Tests, TrueAsBool) {
  JsonVariant variant = true;
  ASSERT_TRUE(variant.as<bool>());
}

TEST(JsonVariant_As_Tests, TrueAsDouble) {
  JsonVariant variant = true;
  ASSERT_EQ(1.0, variant.as<double>());
}

TEST(JsonVariant_As_Tests, TrueAsLong) {
  JsonVariant variant = true;
  ASSERT_EQ(1L, variant.as<long>());
}

TEST(JsonVariant_As_Tests, TrueAsString) {
  JsonVariant variant = true;
  ASSERT_EQ(std::string("true"), variant.as<std::string>());
}

TEST(JsonVariant_As_Tests, LongAsBool) {
  JsonVariant variant = 42L;
  ASSERT_TRUE(variant.as<bool>());
}

TEST(JsonVariant_As_Tests, LongZeroAsBool) {
  JsonVariant variant = 0L;
  ASSERT_FALSE(variant.as<bool>());
}

TEST(JsonVariant_As_Tests, PositiveLongAsDouble) {
  JsonVariant variant = 42L;
  ASSERT_EQ(42.0, variant.as<double>());
}

TEST(JsonVariant_As_Tests, NegativeLongAsDouble) {
  JsonVariant variant = -42L;
  ASSERT_EQ(-42.0, variant.as<double>());
}

TEST(JsonVariant_As_Tests, LongAsString) {
  JsonVariant variant = 42L;
  ASSERT_EQ(std::string("42"), variant.as<std::string>());
}

TEST(JsonVariant_As_Tests, LongZeroAsDouble) {
  JsonVariant variant = 0L;
  ASSERT_EQ(0.0, variant.as<double>());
}

TEST(JsonVariant_As_Tests, NullAsBool) {
  JsonVariant variant = null;
  ASSERT_FALSE(variant.as<bool>());
}

TEST(JsonVariant_As_Tests, NullAsDouble) {
  JsonVariant variant = null;
  ASSERT_EQ(0.0, variant.as<double>());
}

TEST(JsonVariant_As_Tests, NullAsLong) {
  JsonVariant variant = null;
  ASSERT_EQ(0L, variant.as<long>());
}

TEST(JsonVariant_As_Tests, NullAsString) {
  JsonVariant variant = null;
  ASSERT_EQ(std::string("null"), variant.as<std::string>());
}

TEST(JsonVariant_As_Tests, NumberStringAsBool) {
  JsonVariant variant = "42";
  ASSERT_TRUE(variant.as<bool>());
}

TEST(JsonVariant_As_Tests, NumberStringAsLong) {
  JsonVariant variant = "42";
  ASSERT_EQ(42L, variant.as<long>());
}

#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
TEST(JsonVariant_As_Tests, NumberStringAsInt64Negative) {
  JsonVariant variant = "-9223372036854775808";
  ASSERT_EQ(-9223372036854775807 - 1, variant.as<long long>());
}

TEST(JsonVariant_As_Tests, NumberStringAsInt64Positive) {
  JsonVariant variant = "9223372036854775807";
  ASSERT_EQ(9223372036854775807, variant.as<long long>());
}
#endif

TEST(JsonVariant_As_Tests, RandomStringAsBool) {
  JsonVariant variant = "hello";
  ASSERT_FALSE(variant.as<bool>());
}

TEST(JsonVariant_As_Tests, RandomStringAsLong) {
  JsonVariant variant = "hello";
  ASSERT_EQ(0L, variant.as<long>());
}

TEST(JsonVariant_As_Tests, RandomStringAsConstCharPtr) {
  JsonVariant variant = "hello";
  ASSERT_STREQ("hello", variant.as<const char*>());
}

TEST(JsonVariant_As_Tests, RandomStringAsCharPtr) {
  JsonVariant variant = "hello";
  ASSERT_STREQ("hello", variant.as<char*>());
}

TEST(JsonVariant_As_Tests, RandomStringAsString) {
  JsonVariant variant = "hello";
  ASSERT_EQ(std::string("hello"), variant.as<std::string>());
}

TEST(JsonVariant_As_Tests, TrueStringAsBool) {
  JsonVariant variant = "true";
  ASSERT_TRUE(variant.as<bool>());
}

TEST(JsonVariant_As_Tests, TrueStringAsLong) {
  JsonVariant variant = "true";
  ASSERT_EQ(1L, variant.as<long>());
}

TEST(JsonVariant_As_Tests, ObjectAsString) {
  DynamicJsonBuffer buffer;

  JsonObject& obj = buffer.createObject();
  obj["key"] = "value";

  JsonVariant variant = obj;
  ASSERT_EQ(std::string("{\"key\":\"value\"}"), variant.as<std::string>());
}

TEST(JsonVariant_As_Tests, ArrayAsString) {
  DynamicJsonBuffer buffer;

  JsonArray& arr = buffer.createArray();
  arr.add(4);
  arr.add(2);

  JsonVariant variant = arr;
  ASSERT_EQ(std::string("[4,2]"), variant.as<std::string>());
}

TEST(JsonVariant_As_Tests, ArrayAsJsonArray) {
  DynamicJsonBuffer buffer;
  JsonArray& arr = buffer.createArray();

  JsonVariant variant = arr;
  ASSERT_EQ(&arr, &variant.as<JsonArray&>());
  ASSERT_EQ(&arr, &variant.as<JsonArray>());  // <- shorthand
}

TEST(JsonVariant_As_Tests, ObjectAsJsonObject) {
  DynamicJsonBuffer buffer;
  JsonObject& arr = buffer.createObject();

  JsonVariant variant = arr;
  ASSERT_EQ(&arr, &variant.as<JsonObject&>());
  ASSERT_EQ(&arr, &variant.as<JsonObject>());  // <- shorthand
}
