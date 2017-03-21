// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

using namespace ArduinoJson::Internals;

class StringBuilderTests : public testing::Test {
 protected:
  virtual void SetUp() {
    _stringBuilder = new StaticStringBuilder(_buffer, sizeof(_buffer));
  }

  virtual void TearDown() {
    delete _stringBuilder;
  }

  void print(const char *value) {
    _returnValue = _stringBuilder->print(value);
  }

  void outputMustBe(const char *expected) {
    EXPECT_STREQ(expected, _buffer);
  }

  void resultMustBe(size_t expected) {
    EXPECT_EQ(expected, _returnValue);
  }

 private:
  char _buffer[20];
  Print *_stringBuilder;
  size_t _returnValue;
};

TEST_F(StringBuilderTests, InitialState) {
  outputMustBe("");
}

TEST_F(StringBuilderTests, OverCapacity) {
  print("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  resultMustBe(19);

  print("ABC");
  resultMustBe(0);

  outputMustBe("ABCDEFGHIJKLMNOPQRS");
}

TEST_F(StringBuilderTests, EmptyString) {
  print("");
  resultMustBe(0);
  outputMustBe("");
}

TEST_F(StringBuilderTests, OneString) {
  print("ABCD");
  resultMustBe(4);
  outputMustBe("ABCD");
}

TEST_F(StringBuilderTests, TwoStrings) {
  print("ABCD");
  resultMustBe(4);

  print("EFGH");
  resultMustBe(4);

  outputMustBe("ABCDEFGH");
}
