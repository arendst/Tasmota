// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <gtest/gtest.h>

#include <ArduinoJson/Serialization/JsonWriter.hpp>
#include <ArduinoJson/Serialization/StaticStringBuilder.hpp>

using namespace ArduinoJson::Internals;

class JsonWriter_WriteString_Tests : public testing::Test {
 protected:
  void whenInputIs(const char *input) {
    StaticStringBuilder sb(buffer, sizeof(buffer));
    JsonWriter writer(sb);
    writer.writeString(input);
    returnValue = writer.bytesWritten();
  }

  void outputMustBe(const char *expected) {
    EXPECT_STREQ(expected, buffer);
    EXPECT_EQ(strlen(expected), returnValue);
  }

 private:
  char buffer[1024];
  size_t returnValue;
};

TEST_F(JsonWriter_WriteString_Tests, Null) {
  whenInputIs(0);
  outputMustBe("null");
}

TEST_F(JsonWriter_WriteString_Tests, EmptyString) {
  whenInputIs("");
  outputMustBe("\"\"");
}

TEST_F(JsonWriter_WriteString_Tests, QuotationMark) {
  whenInputIs("\"");
  outputMustBe("\"\\\"\"");
}

TEST_F(JsonWriter_WriteString_Tests, ReverseSolidus) {
  whenInputIs("\\");
  outputMustBe("\"\\\\\"");
}

TEST_F(JsonWriter_WriteString_Tests, Solidus) {
  whenInputIs("/");
  outputMustBe("\"/\"");  // but the JSON format allows \/
}

TEST_F(JsonWriter_WriteString_Tests, Backspace) {
  whenInputIs("\b");
  outputMustBe("\"\\b\"");
}

TEST_F(JsonWriter_WriteString_Tests, Formfeed) {
  whenInputIs("\f");
  outputMustBe("\"\\f\"");
}

TEST_F(JsonWriter_WriteString_Tests, Newline) {
  whenInputIs("\n");
  outputMustBe("\"\\n\"");
}

TEST_F(JsonWriter_WriteString_Tests, CarriageReturn) {
  whenInputIs("\r");
  outputMustBe("\"\\r\"");
}

TEST_F(JsonWriter_WriteString_Tests, HorizontalTab) {
  whenInputIs("\t");
  outputMustBe("\"\\t\"");
}
