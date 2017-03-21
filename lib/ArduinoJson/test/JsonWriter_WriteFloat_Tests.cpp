// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <gtest/gtest.h>
#include <limits>

#include <ArduinoJson/Serialization/JsonWriter.hpp>
#include <ArduinoJson/Serialization/StaticStringBuilder.hpp>

using namespace ArduinoJson::Internals;

class JsonWriter_WriteFloat_Tests : public testing::Test {
 protected:
  void whenInputIs(double input, uint8_t digits = 2) {
    StaticStringBuilder sb(buffer, sizeof(buffer));
    JsonWriter writer(sb);
    writer.writeFloat(input, digits);
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

TEST_F(JsonWriter_WriteFloat_Tests, NaN) {
  whenInputIs(std::numeric_limits<double>::signaling_NaN());
  outputMustBe("NaN");
}

TEST_F(JsonWriter_WriteFloat_Tests, PositiveInfinity) {
  whenInputIs(std::numeric_limits<double>::infinity());
  outputMustBe("Infinity");
}

TEST_F(JsonWriter_WriteFloat_Tests, NegativeInfinity) {
  whenInputIs(-std::numeric_limits<double>::infinity());
  outputMustBe("-Infinity");
}

TEST_F(JsonWriter_WriteFloat_Tests, Zero) {
  whenInputIs(0);
  outputMustBe("0.00");
}

TEST_F(JsonWriter_WriteFloat_Tests, ZeroDigits_Rounding) {
  whenInputIs(9.5, 0);
  outputMustBe("10");
}

TEST_F(JsonWriter_WriteFloat_Tests, ZeroDigits_NoRounding) {
  whenInputIs(9.4, 0);
  outputMustBe("9");
}

TEST_F(JsonWriter_WriteFloat_Tests, OneDigit_Rounding) {
  whenInputIs(9.95, 1);
  outputMustBe("10.0");
}

TEST_F(JsonWriter_WriteFloat_Tests, OneDigit_NoRounding) {
  whenInputIs(9.94, 1);
  outputMustBe("9.9");
}

TEST_F(JsonWriter_WriteFloat_Tests, TwoDigits_Rounding) {
  whenInputIs(9.995, 2);
  outputMustBe("10.00");
}

TEST_F(JsonWriter_WriteFloat_Tests, TwoDigits_NoRounding) {
  whenInputIs(9.994, 2);
  outputMustBe("9.99");
}

TEST_F(JsonWriter_WriteFloat_Tests, ThreeDigits_Rounding) {
  whenInputIs(9.9995, 3);
  outputMustBe("10.000");
}

TEST_F(JsonWriter_WriteFloat_Tests, ThreeDigits_NoRounding) {
  whenInputIs(9.9994, 3);
  outputMustBe("9.999");
}

TEST_F(JsonWriter_WriteFloat_Tests, FourDigits_Rounding) {
  whenInputIs(9.99995, 4);
  outputMustBe("10.0000");
}

TEST_F(JsonWriter_WriteFloat_Tests, FourDigits_NoRounding) {
  whenInputIs(9.99994, 4);
  outputMustBe("9.9999");
}

TEST_F(JsonWriter_WriteFloat_Tests, FiveDigits_Rounding) {
  whenInputIs(9.999995, 5);
  outputMustBe("10.00000");
}

TEST_F(JsonWriter_WriteFloat_Tests, FiveDigits_NoRounding) {
  whenInputIs(9.999994, 5);
  outputMustBe("9.99999");
}
