// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class JsonParser_NestingLimit_Tests : public testing::Test {
 protected:
  void whenNestingLimitIs(uint8_t nestingLimit) {
    _nestingLimit = nestingLimit;
  }

  void parseArrayMustFail(const char *json) {
    ASSERT_FALSE(tryParseArray(json));
  }

  void parseArrayMustSucceed(const char *json) {
    ASSERT_TRUE(tryParseArray(json));
  }

  void parseObjectMustFail(const char *json) {
    ASSERT_FALSE(tryParseObject(json));
  }

  void parseObjectMustSucceed(const char *json) {
    ASSERT_TRUE(tryParseObject(json));
  }

 private:
  bool tryParseArray(const char *json) {
    DynamicJsonBuffer buffer;
    char s[256];
    strcpy(s, json);
    return buffer.parseArray(s, _nestingLimit).success();
  }

  bool tryParseObject(const char *json) {
    DynamicJsonBuffer buffer;
    char s[256];
    strcpy(s, json);
    return buffer.parseObject(s, _nestingLimit).success();
  }

  uint8_t _nestingLimit;
};

TEST_F(JsonParser_NestingLimit_Tests, ParseArrayWithNestingLimit0) {
  whenNestingLimitIs(0);
  parseArrayMustSucceed("[]");
  parseArrayMustFail("[[]]");
}

TEST_F(JsonParser_NestingLimit_Tests, ParseArrayWithNestingLimit1) {
  whenNestingLimitIs(1);
  parseArrayMustSucceed("[[]]");
  parseArrayMustFail("[[[]]]");
}

TEST_F(JsonParser_NestingLimit_Tests, ParseArrayWithNestingLimit2) {
  whenNestingLimitIs(2);
  parseArrayMustSucceed("[[[]]]");
  parseArrayMustFail("[[[[]]]]");
}

TEST_F(JsonParser_NestingLimit_Tests, ParseObjectWithNestingLimit0) {
  whenNestingLimitIs(0);
  parseObjectMustSucceed("{}");
  parseObjectMustFail("{\"key\":{}}");
}

TEST_F(JsonParser_NestingLimit_Tests, ParseObjectWithNestingLimit1) {
  whenNestingLimitIs(1);
  parseObjectMustSucceed("{\"key\":{}}");
  parseObjectMustFail("{\"key\":{\"key\":{}}}");
}

TEST_F(JsonParser_NestingLimit_Tests, ParseObjectWithNestingLimit2) {
  whenNestingLimitIs(2);
  parseObjectMustSucceed("{\"key\":{\"key\":{}}}");
  parseObjectMustFail("{\"key\":{\"key\":{\"key\":{}}}}");
}
