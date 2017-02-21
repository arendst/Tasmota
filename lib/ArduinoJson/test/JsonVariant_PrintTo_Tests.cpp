// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>
#include <limits>

class JsonVariant_PrintTo_Tests : public testing::Test {
 protected:
  JsonVariant variant;

  void outputMustBe(const char *expected) {
    char buffer[256] = "";
    size_t n = variant.printTo(buffer, sizeof(buffer));
    ASSERT_STREQ(expected, buffer);
    ASSERT_EQ(strlen(expected), n);
  }
};

TEST_F(JsonVariant_PrintTo_Tests, Empty) {
  outputMustBe("");
}

TEST_F(JsonVariant_PrintTo_Tests, Null) {
  variant = static_cast<char *>(0);
  outputMustBe("null");
}

TEST_F(JsonVariant_PrintTo_Tests, String) {
  variant = "hello";
  outputMustBe("\"hello\"");
}

TEST_F(JsonVariant_PrintTo_Tests, DoubleZero) {
  variant = 0.0;
  outputMustBe("0.00");
}

TEST_F(JsonVariant_PrintTo_Tests, DoubleDefaultDigits) {
  variant = 3.14159265358979323846;
  outputMustBe("3.14");
}

TEST_F(JsonVariant_PrintTo_Tests, DoubleFourDigits) {
  variant = JsonVariant(3.14159265358979323846, 4);
  outputMustBe("3.1416");
}

TEST_F(JsonVariant_PrintTo_Tests, Infinity) {
  variant = std::numeric_limits<double>::infinity();
  outputMustBe("Infinity");
}

TEST_F(JsonVariant_PrintTo_Tests, MinusInfinity) {
  variant = -std::numeric_limits<double>::infinity();
  outputMustBe("-Infinity");
}

TEST_F(JsonVariant_PrintTo_Tests, SignalingNaN) {
  variant = std::numeric_limits<double>::signaling_NaN();
  outputMustBe("NaN");
}

TEST_F(JsonVariant_PrintTo_Tests, QuietNaN) {
  variant = std::numeric_limits<double>::quiet_NaN();
  outputMustBe("NaN");
}

TEST_F(JsonVariant_PrintTo_Tests, VeryBigPositiveDouble) {
  variant = JsonVariant(3.14159265358979323846e42, 4);
  outputMustBe("3.1416e42");
}

TEST_F(JsonVariant_PrintTo_Tests, VeryBigNegativeDouble) {
  variant = JsonVariant(-3.14159265358979323846e42, 4);
  outputMustBe("-3.1416e42");
}

TEST_F(JsonVariant_PrintTo_Tests, VerySmallPositiveDouble) {
  variant = JsonVariant(3.14159265358979323846e-42, 4);
  outputMustBe("3.1416e-42");
}

TEST_F(JsonVariant_PrintTo_Tests, VerySmallNegativeDouble) {
  variant = JsonVariant(-3.14159265358979323846e-42, 4);
  outputMustBe("-3.1416e-42");
}

TEST_F(JsonVariant_PrintTo_Tests, Integer) {
  variant = 42;
  outputMustBe("42");
}

TEST_F(JsonVariant_PrintTo_Tests, NegativeLong) {
  variant = -42;
  outputMustBe("-42");
}

TEST_F(JsonVariant_PrintTo_Tests, UnsignedLong) {
  variant = 4294967295UL;
  outputMustBe("4294967295");
}

TEST_F(JsonVariant_PrintTo_Tests, True) {
  variant = true;
  outputMustBe("true");
}

TEST_F(JsonVariant_PrintTo_Tests, OneFalse) {
  variant = false;
  outputMustBe("false");
}

#if ARDUINOJSON_USE_LONG_LONG || ARDUINOJSON_USE_INT64
TEST_F(JsonVariant_PrintTo_Tests, NegativeInt64) {
  variant = -9223372036854775807 - 1;
  outputMustBe("-9223372036854775808");
}

TEST_F(JsonVariant_PrintTo_Tests, PositiveInt64) {
  variant = 9223372036854775807;
  outputMustBe("9223372036854775807");
}

TEST_F(JsonVariant_PrintTo_Tests, UInt64) {
  variant = 18446744073709551615;
  outputMustBe("18446744073709551615");
}
#endif
