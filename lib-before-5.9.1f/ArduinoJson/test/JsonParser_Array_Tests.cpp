// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class JsonParser_Array_Tests : public testing::Test {
 protected:
  void whenInputIs(const char *json) {
    strcpy(_jsonString, json);
  }

  void whenInputIs(const char *json, size_t len) {
    memcpy(_jsonString, json, len);
  }

  void parseMustSucceed() {
    _array = &_jsonBuffer.parseArray(_jsonString);
    EXPECT_TRUE(_array->success());
  }

  void parseMustFail() {
    _array = &_jsonBuffer.parseArray(_jsonString);
    EXPECT_FALSE(_array->success());
    EXPECT_EQ(0, _array->size());
  }

  void sizeMustBe(int expected) {
    ASSERT_EQ(expected, _array->size());
  }

  template <typename T>
  void firstElementMustBe(T expected) {
    elementAtIndexMustBe(0, expected);
  }

  template <typename T>
  void secondElementMustBe(T expected) {
    elementAtIndexMustBe(1, expected);
  }

  template <typename T>
  void elementAtIndexMustBe(int index, T expected) {
    EXPECT_EQ(expected, (*_array)[index].as<T>());
  }

  void elementAtIndexMustBe(int index, const char *expected) {
    EXPECT_STREQ(expected, (*_array)[index].as<const char *>());
  }

  DynamicJsonBuffer _jsonBuffer;
  JsonArray *_array;
  char _jsonString[256];
};

TEST_F(JsonParser_Array_Tests, EmptyArray) {
  whenInputIs("[]");

  parseMustSucceed();
  sizeMustBe(0);
}

TEST_F(JsonParser_Array_Tests, MissingOpeningBracket) {
  whenInputIs("]");
  parseMustFail();
}

TEST_F(JsonParser_Array_Tests, ArrayWithNoEnd) {
  whenInputIs("[");
  parseMustFail();
}

TEST_F(JsonParser_Array_Tests, EmptyArrayWithLeadingSpaces) {
  whenInputIs("  []");

  parseMustSucceed();
  sizeMustBe(0);
}

TEST_F(JsonParser_Array_Tests, Garbage) {
  whenInputIs("%*$£¤");

  parseMustFail();
}

TEST_F(JsonParser_Array_Tests, OneInteger) {
  whenInputIs("[42]");

  parseMustSucceed();
  sizeMustBe(1);
  firstElementMustBe(42);
}

TEST_F(JsonParser_Array_Tests, OneIntegerWithSpacesBefore) {
  whenInputIs("[ \t\r\n42]");

  parseMustSucceed();
  sizeMustBe(1);
  firstElementMustBe(42);
}

TEST_F(JsonParser_Array_Tests, OneIntegerWithSpaceAfter) {
  whenInputIs("[42 \t\r\n]");

  parseMustSucceed();
  sizeMustBe(1);
  firstElementMustBe(42);
}

TEST_F(JsonParser_Array_Tests, TwoIntegers) {
  whenInputIs("[42,84]");

  parseMustSucceed();
  sizeMustBe(2);
  firstElementMustBe(42);
  secondElementMustBe(84);
}

TEST_F(JsonParser_Array_Tests, TwoDoubles) {
  whenInputIs("[4.2,1e2]");

  parseMustSucceed();
  sizeMustBe(2);
  firstElementMustBe(4.2);
  secondElementMustBe(1e2);
}

TEST_F(JsonParser_Array_Tests, UnsignedLong) {
  whenInputIs("[4294967295]");

  parseMustSucceed();
  sizeMustBe(1);
  firstElementMustBe(4294967295UL);
}

TEST_F(JsonParser_Array_Tests, TwoBooleans) {
  whenInputIs("[true,false]");

  parseMustSucceed();
  sizeMustBe(2);
  firstElementMustBe(true);
  secondElementMustBe(false);
}

TEST_F(JsonParser_Array_Tests, TwoNulls) {
  const char *const nullCharPtr = 0;

  whenInputIs("[null,null]");

  parseMustSucceed();
  sizeMustBe(2);
  firstElementMustBe(nullCharPtr);
  secondElementMustBe(nullCharPtr);
}

TEST_F(JsonParser_Array_Tests, TwoStringsDoubleQuotes) {
  whenInputIs("[ \"hello\" , \"world\" ]");

  parseMustSucceed();
  sizeMustBe(2);
  firstElementMustBe("hello");
  secondElementMustBe("world");
}

TEST_F(JsonParser_Array_Tests, TwoStringsSingleQuotes) {
  whenInputIs("[ 'hello' , 'world' ]");

  parseMustSucceed();
  sizeMustBe(2);
  firstElementMustBe("hello");
  secondElementMustBe("world");
}

TEST_F(JsonParser_Array_Tests, TwoStringsNoQuotes) {
  whenInputIs("[ hello , world ]");

  parseMustSucceed();
  sizeMustBe(2);
  firstElementMustBe("hello");
  secondElementMustBe("world");
}

TEST_F(JsonParser_Array_Tests, EmptyStringsDoubleQuotes) {
  whenInputIs("[\"\",\"\"]");

  parseMustSucceed();
  sizeMustBe(2);
  firstElementMustBe("");
  secondElementMustBe("");
}

TEST_F(JsonParser_Array_Tests, EmptyStringSingleQuotes) {
  whenInputIs("[\'\',\'\']");

  parseMustSucceed();
  sizeMustBe(2);
  firstElementMustBe("");
  secondElementMustBe("");
}

TEST_F(JsonParser_Array_Tests, EmptyStringNoQuotes) {
  whenInputIs("[,]");

  parseMustSucceed();
  sizeMustBe(2);
  firstElementMustBe("");
  secondElementMustBe("");
}

TEST_F(JsonParser_Array_Tests, ClosingDoubleQuoteMissing) {
  whenInputIs("[\"]");

  parseMustFail();
}

TEST_F(JsonParser_Array_Tests, ClosingSignleQuoteMissing) {
  whenInputIs("[\']");

  parseMustFail();
}

TEST_F(JsonParser_Array_Tests, StringWithEscapedChars) {
  whenInputIs("[\"1\\\"2\\\\3\\/4\\b5\\f6\\n7\\r8\\t9\"]");

  parseMustSucceed();
  sizeMustBe(1);
  firstElementMustBe("1\"2\\3/4\b5\f6\n7\r8\t9");
}

TEST_F(JsonParser_Array_Tests, StringWithUnterminatedEscapeSequence) {
  whenInputIs("\"\\\0\"", 4);
  parseMustFail();
}

TEST_F(JsonParser_Array_Tests, CCommentBeforeOpeningBracket) {
  whenInputIs("/*COMMENT*/  [\"hello\"]");

  parseMustSucceed();
  sizeMustBe(1);
  firstElementMustBe("hello");
}

TEST_F(JsonParser_Array_Tests, CCommentAfterOpeningBracket) {
  whenInputIs("[/*COMMENT*/ \"hello\"]");

  parseMustSucceed();
  sizeMustBe(1);
  firstElementMustBe("hello");
}

TEST_F(JsonParser_Array_Tests, CCommentBeforeClosingBracket) {
  whenInputIs("[\"hello\"/*COMMENT*/]");

  parseMustSucceed();
  sizeMustBe(1);
  firstElementMustBe("hello");
}

TEST_F(JsonParser_Array_Tests, CCommentAfterClosingBracket) {
  whenInputIs("[\"hello\"]/*COMMENT*/");

  parseMustSucceed();
  sizeMustBe(1);
  firstElementMustBe("hello");
}

TEST_F(JsonParser_Array_Tests, CCommentBeforeComma) {
  whenInputIs("[\"hello\"/*COMMENT*/,\"world\"]");

  parseMustSucceed();
  sizeMustBe(2);
  firstElementMustBe("hello");
  secondElementMustBe("world");
}

TEST_F(JsonParser_Array_Tests, CCommentAfterComma) {
  whenInputIs("[\"hello\",/*COMMENT*/ \"world\"]");

  parseMustSucceed();
  sizeMustBe(2);
  firstElementMustBe("hello");
  secondElementMustBe("world");
}

TEST_F(JsonParser_Array_Tests, CppCommentBeforeOpeningBracket) {
  whenInputIs("//COMMENT\n\t[\"hello\"]");

  parseMustSucceed();
  sizeMustBe(1);
  firstElementMustBe("hello");
}

TEST_F(JsonParser_Array_Tests, CppCommentAfterOpeningBracket) {
  whenInputIs("[//COMMENT\n\"hello\"]");

  parseMustSucceed();
  sizeMustBe(1);
  firstElementMustBe("hello");
}

TEST_F(JsonParser_Array_Tests, CppCommentBeforeClosingBracket) {
  whenInputIs("[\"hello\"//COMMENT\r\n]");

  parseMustSucceed();
  sizeMustBe(1);
  firstElementMustBe("hello");
}

TEST_F(JsonParser_Array_Tests, CppCommentAfterClosingBracket) {
  whenInputIs("[\"hello\"]//COMMENT\n");

  parseMustSucceed();
  sizeMustBe(1);
  firstElementMustBe("hello");
}

TEST_F(JsonParser_Array_Tests, CppCommentBeforeComma) {
  whenInputIs("[\"hello\"//COMMENT\n,\"world\"]");

  parseMustSucceed();
  sizeMustBe(2);
  firstElementMustBe("hello");
  secondElementMustBe("world");
}

TEST_F(JsonParser_Array_Tests, CppCommentAfterComma) {
  whenInputIs("[\"hello\",//COMMENT\n\"world\"]");

  parseMustSucceed();
  sizeMustBe(2);
  firstElementMustBe("hello");
  secondElementMustBe("world");
}

TEST_F(JsonParser_Array_Tests, InvalidCppComment) {
  whenInputIs("[/COMMENT\n]");
  parseMustFail();
}

TEST_F(JsonParser_Array_Tests, InvalidComment) {
  whenInputIs("[/*/\n]");
  parseMustFail();
}

TEST_F(JsonParser_Array_Tests, UnfinishedCComment) {
  whenInputIs("[/*COMMENT]");
  parseMustFail();
}

TEST_F(JsonParser_Array_Tests, EndsInCppComment) {
  whenInputIs("[//COMMENT");
  parseMustFail();
}

TEST_F(JsonParser_Array_Tests, AfterClosingStar) {
  whenInputIs("[/*COMMENT*");
  parseMustFail();
}

TEST_F(JsonParser_Array_Tests, DeeplyNested) {
  whenInputIs("[[[[[[[[[[[[[[[[[[[\"Not too deep\"]]]]]]]]]]]]]]]]]]]");
  parseMustSucceed();
}
