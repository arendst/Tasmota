// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class JsonParser_Variant_Test : public testing::Test {
 protected:
  void whenInputIs(const char* jsonString) {
    strcpy(_jsonString, jsonString);
    _result = _jsonBuffer.parse(_jsonString);
  }

  template <typename T>
  void resultMustEqual(T expected) {
    EXPECT_EQ(expected, _result.as<T>());
  }

  void resultMustEqual(const char* expected) {
    EXPECT_STREQ(expected, _result.as<char*>());
  }

  void resultMustEqual(double expected) {
    EXPECT_DOUBLE_EQ(expected, _result.as<double>());
  }

  template <typename T>
  void resultTypeMustBe() {
    EXPECT_TRUE(_result.is<T>());
  }

  void resultMustBeInvalid() {
    EXPECT_FALSE(_result.success());
  }
  void resultMustBeValid() {
    EXPECT_TRUE(_result.success());
  }

  template <typename T>
  void verify(const char* input, T expected) {
    whenInputIs(input);
    resultMustBeValid();
    resultTypeMustBe<T>();
    resultMustEqual(expected);
  }

 private:
  DynamicJsonBuffer _jsonBuffer;
  JsonVariant _result;
  char _jsonString[256];
};

TEST_F(JsonParser_Variant_Test, EmptyObject) {
  whenInputIs("{}");
  resultMustBeValid();
  resultTypeMustBe<JsonObject>();
}

TEST_F(JsonParser_Variant_Test, EmptyArray) {
  whenInputIs("[]");
  resultMustBeValid();
  resultTypeMustBe<JsonArray>();
}

TEST_F(JsonParser_Variant_Test, Integer) {
  verify("42", 42);
  verify("-42", -42);
}

TEST_F(JsonParser_Variant_Test, Double) {
  verify("3.14", 3.14);
  verify("3.14", 3.14);
  verify("1E+10", 1E+10);
  verify("-1E+10", -1E+10);
  verify("1.234E+10", 1.234E+10);
  verify("1.79769e+308", 1.79769e+308);
  verify("-1.79769e+308", -1.79769e+308);
  verify("1.7976931348623157e+308", 1.7976931348623157e+308);
  verify("0.017976931348623157e+310", 0.017976931348623157e+310);
}

TEST_F(JsonParser_Variant_Test, String) {
  verify("\"hello world\"", "hello world");
}

TEST_F(JsonParser_Variant_Test, True) {
  verify("true", true);
  verify("false", false);
}

TEST_F(JsonParser_Variant_Test, OpenBrace) {
  whenInputIs("{");
  resultMustBeInvalid();
}

TEST_F(JsonParser_Variant_Test, IncompleteStrings) {
  verify("\"", "");
  verify("\"hello", "hello");
  verify("\'", "");
  verify("\'world", "world");
}
