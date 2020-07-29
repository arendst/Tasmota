// Copyright 2017-2019 David Conran

#include "IRutils.h"
#include <stdint.h>
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests reverseBits().

// Tests reverseBits for typical use.
TEST(ReverseBitsTest, TypicalUse) {
  EXPECT_EQ(0xF, reverseBits(0xF0, 8));
  EXPECT_EQ(0xFFFF, reverseBits(0xFFFF0000, 32));
  EXPECT_EQ(0x555500005555FFFF, reverseBits(0xFFFFAAAA0000AAAA, 64));
  EXPECT_EQ(0, reverseBits(0, 64));
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, reverseBits(0xFFFFFFFFFFFFFFFF, 64));
}

// Tests reverseBits for bit size values <= 1
TEST(ReverseBitsTest, LessThanTwoBitsReversed) {
  EXPECT_EQ(0x12345678, reverseBits(0x12345678, 1));
  EXPECT_EQ(1234, reverseBits(1234, 0));
}

// Tests reverseBits for bit size larger than a uint64_t.
TEST(ReverseBitsTest, LargerThan64BitsReversed) {
  EXPECT_EQ(0, reverseBits(0, 65));
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, reverseBits(0xFFFFFFFFFFFFFFFF, 100));
  EXPECT_EQ(0x555500005555FFFF, reverseBits(0xFFFFAAAA0000AAAA, 3000));
}

// Tests reverseBits for bit sizes less than all the data stored.
TEST(ReverseBitsTest, LessBitsReversedThanInputHasSet) {
  EXPECT_EQ(0xF8, reverseBits(0xF1, 4));
  EXPECT_EQ(0xF5, reverseBits(0xFA, 4));
  EXPECT_EQ(0x12345678FFFF0000, reverseBits(0x123456780000FFFF, 32));
}

// Tests for uint64ToString()

TEST(TestUint64ToString, TrivialCases) {
  EXPECT_EQ("0", uint64ToString(0));      // Default base (10)
  EXPECT_EQ("0", uint64ToString(0, 2));   // Base-2
  EXPECT_EQ("0", uint64ToString(0, 8));   // Base-8
  EXPECT_EQ("0", uint64ToString(0, 10));  // Base-10
  EXPECT_EQ("0", uint64ToString(0, 16));  // Base-16

  EXPECT_EQ("1", uint64ToString(1, 2));   // Base-2
  EXPECT_EQ("2", uint64ToString(2, 8));   // Base-8
  EXPECT_EQ("3", uint64ToString(3, 10));  // Base-10
  EXPECT_EQ("4", uint64ToString(4, 16));  // Base-16
}

TEST(TestUint64ToString, NormalUse) {
  EXPECT_EQ("12345", uint64ToString(12345));
  EXPECT_EQ("100", uint64ToString(4, 2));
  EXPECT_EQ("3039", uint64ToString(12345, 16));
  EXPECT_EQ("123456", uint64ToString(123456));
  EXPECT_EQ("1E240", uint64ToString(123456, 16));
  EXPECT_EQ("FEEDDEADBEEF", uint64ToString(0xfeeddeadbeef, 16));
}

TEST(TestUint64ToString, Max64Bit) {
  EXPECT_EQ("18446744073709551615", uint64ToString(UINT64_MAX));  // Default
  EXPECT_EQ("1111111111111111111111111111111111111111111111111111111111111111",
            uint64ToString(UINT64_MAX, 2));                            // Base-2
  EXPECT_EQ("1777777777777777777777", uint64ToString(UINT64_MAX, 8));  // Base-8
  EXPECT_EQ("18446744073709551615", uint64ToString(UINT64_MAX, 10));  // Base-10
  EXPECT_EQ("FFFFFFFFFFFFFFFF", uint64ToString(UINT64_MAX, 16));      // Base-16
}

TEST(TestUint64ToString, Max32Bit) {
  EXPECT_EQ("4294967295", uint64ToString(UINT32_MAX));      // Default
  EXPECT_EQ("37777777777", uint64ToString(UINT32_MAX, 8));  // Base-8
  EXPECT_EQ("4294967295", uint64ToString(UINT32_MAX, 10));  // Base-10
  EXPECT_EQ("FFFFFFFF", uint64ToString(UINT32_MAX, 16));    // Base-16
}

TEST(TestUint64ToString, InterestingCases) {
  // Previous hacky-code didn't handle leading zeros in the lower 32 bits.
  EXPECT_EQ("100000000", uint64ToString(0x100000000, 16));
  EXPECT_EQ("100000001", uint64ToString(0x100000001, 16));
}

TEST(TestUint64ToString, SillyBases) {
  // If we are given a silly base, we should defer to Base-10.
  EXPECT_EQ("12345", uint64ToString(12345, 0));  // Super silly, makes no sense.
  EXPECT_EQ("12345", uint64ToString(12345, 1));  // We don't do unary.
  EXPECT_EQ("12345", uint64ToString(12345, 100));  // We can't print base-100.
  EXPECT_EQ("12345", uint64ToString(12345, 37));   // Base-37 is one to far.
  EXPECT_EQ("9IX", uint64ToString(12345, 36));     // But we *can* do base-36.
}

TEST(TestGetCorrectedRawLength, NoLargeValues) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  uint16_t test_data[7] = {1, 2, 3, 4, 5, 6, 7};
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(test_data, 7, 38000);
  irsend.makeDecodeResult();
  irrecv.decode(&irsend.capture);
  EXPECT_EQ(7, getCorrectedRawLength(&irsend.capture));
}

TEST(TestGetCorrectedRawLength, WithLargeValues) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  uint16_t test_data[7] = {10, 20, 30, 40, 50, 60, 70};
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(test_data, 7, 38000);
  irsend.makeDecodeResult();
  irrecv.decode(&irsend.capture);
  irsend.capture.rawbuf[3] = 60000;
  ASSERT_EQ(2, kRawTick);  // The following values rely on kRawTick being 2.
  EXPECT_EQ(7 + 2, getCorrectedRawLength(&irsend.capture));
  irsend.capture.rawbuf[4] = UINT16_MAX - 1;
  EXPECT_EQ(7 + 2 * 2, getCorrectedRawLength(&irsend.capture));
  irsend.capture.rawbuf[4] = UINT16_MAX;
  EXPECT_EQ(7 + 2 * 2, getCorrectedRawLength(&irsend.capture));
}

TEST(TestResultToSourceCode, SimpleTests) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  uint16_t test_data[7] = {10, 20, 30, 40, 50, 60, 70};
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(test_data, 7, 38000);
  irsend.makeDecodeResult();
  irrecv.decode(&irsend.capture);
  EXPECT_EQ(
      "uint16_t rawData[7] = {10, 20,  30, 40,  50, 60,  70};"
      "  // UNKNOWN A5E5F35D\n",
      resultToSourceCode(&irsend.capture));

  // Stick in some large values.
  irsend.capture.rawbuf[3] = 60000;
  EXPECT_EQ(
      "uint16_t rawData[9] = {10, 20,  65535, 0,  54465, 40,"
      "  50, 60,  70};  // UNKNOWN A5E5F35D\n",
      resultToSourceCode(&irsend.capture));
  irsend.capture.rawbuf[5] = UINT16_MAX;
  EXPECT_EQ(
      "uint16_t rawData[11] = {10, 20,  65535, 0,  54465, 40,"
      "  65535, 0,  65535, 60,  70};  // UNKNOWN A5E5F35D\n",
      resultToSourceCode(&irsend.capture));

  // Reset and put the large value in a space location.
  irsend.reset();
  irsend.sendRaw(test_data, 7, 38000);
  irsend.makeDecodeResult();
  irrecv.decode(&irsend.capture);
  irsend.capture.rawbuf[4] = UINT16_MAX - 1;
  EXPECT_EQ(
      "uint16_t rawData[9] = {10, 20,  30, 65535,  0, 65533,"
      "  50, 60,  70};  // UNKNOWN A5E5F35D\n",
      resultToSourceCode(&irsend.capture));
}

TEST(TestResultToSourceCode, SimpleProtocols) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  // Generate a code which has address & command values.
  irsend.reset();
  irsend.sendNEC(irsend.encodeNEC(0x10, 0x20));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(NEC, irsend.capture.decode_type);
  ASSERT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(
      "uint16_t rawData[68] = {8960, 4480,  560, 560,  560, 560,  560, 560,  "
      "560, 560,  560, 1680,  560, 560,  560, 560,  560, 560,  560, 1680,  "
      "560, 1680,  560, 1680,  560, 1680,  560, 560,  560, 1680,  560, 1680,  "
      "560, 1680,  560, 560,  560, 560,  560, 560,  560, 560,  560, 560,  "
      "560, 1680,  560, 560,  560, 560,  560, 1680,  560, 1680,  560, 1680,  "
      "560, 1680,  560, 1680,  560, 560,  560, 1680,  560, 1680,  560, 40320 "
      "};  // NEC 8F704FB\n"
      "uint32_t address = 0x10;\n"
      "uint32_t command = 0x20;\n"
      "uint64_t data = 0x8F704FB;\n",
      resultToSourceCode(&irsend.capture));

  // Generate a code which DOESN'T have address & command values.
  irsend.reset();
  irsend.sendNikai(0xD0F2F);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(NIKAI, irsend.capture.decode_type);
  ASSERT_EQ(kNikaiBits, irsend.capture.bits);
  EXPECT_EQ(
      "uint16_t rawData[52] = {4000, 4000,  500, 2000,  500, 2000,  "
      "500, 2000,  500, 2000,  500, 1000,  500, 1000,  500, 2000,  500, 1000,  "
      "500, 2000,  500, 2000,  500, 2000,  500, 2000,  500, 1000,  500, 1000,  "
      "500, 1000,  500, 1000,  500, 2000,  500, 2000,  500, 1000,  500, 2000,  "
      "500, 1000,  500, 1000,  500, 1000,  500, 1000,  500, 8500 };"
      "  // NIKAI D0F2F\n"
      "uint64_t data = 0xD0F2F;\n",
      resultToSourceCode(&irsend.capture));
}

TEST(TestResultToSourceCode, ComplexProtocols) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  uint8_t state[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                          0x00, 0x00, 0x00, 0x01};

  irsend.reset();
  irsend.sendToshibaAC(state);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(TOSHIBA_AC, irsend.capture.decode_type);
  ASSERT_EQ(kToshibaACBits, irsend.capture.bits);
  EXPECT_EQ(
      "uint16_t rawData[296] = {4400, 4300,  542, 1622,  542, 1622,  "
      "542, 1622,  542, 1622,  542, 472,  542, 472,  542, 1622,  542, 472,  "
      "542, 472,  542, 472,  542, 472,  542, 472,  542, 1622,  542, 1622,  "
      "542, 472,  542, 1622,  542, 472,  542, 472,  542, 472,  542, 472,  "
      "542, 472,  542, 472,  542, 1622,  542, 1622,  542, 1622,  542, 1622,  "
      "542, 1622,  542, 1622,  542, 1622,  542, 1622,  542, 472,  542, 472,  "
      "542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  "
      "542, 472,  542, 1622,  542, 472,  542, 472,  542, 472,  542, 472,  "
      "542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  "
      "542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  "
      "542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  "
      "542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  "
      "542, 472,  542, 472,  542, 472,  542, 1622,  542, 7048,  4400, 4300,  "
      "542, 1622,  542, 1622,  542, 1622,  542, 1622,  542, 472,  542, 472,  "
      "542, 1622,  542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  "
      "542, 1622,  542, 1622,  542, 472,  542, 1622,  542, 472,  542, 472,  "
      "542, 472,  542, 472,  542, 472,  542, 472,  542, 1622,  542, 1622,  "
      "542, 1622,  542, 1622,  542, 1622,  542, 1622,  542, 1622,  542, 1622,  "
      "542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  "
      "542, 472,  542, 472,  542, 472,  542, 1622,  542, 472,  542, 472,  "
      "542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  "
      "542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  "
      "542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  "
      "542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  "
      "542, 472,  542, 472,  542, 472,  542, 472,  542, 472,  542, 1622,  "
      "542, 7048 };  // TOSHIBA_AC\n"
      "uint8_t state[9] = {0xF2, 0x0D, 0x03, 0xFC, 0x01, 0x00, 0x00, 0x00, "
      "0x01};\n",
      resultToSourceCode(&irsend.capture));
}

TEST(TestResultToTimingInfo, General) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  irsend.reset();
  irsend.sendNEC(irsend.encodeNEC(0x10, 0x20));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(NEC, irsend.capture.decode_type);
  ASSERT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(
      "Raw Timing[68]:\n"
      "   +  8960, -  4480,    +   560, -   560,    +   560, -   560,"
      "    +   560, -   560, \n"
      "   +   560, -   560,    +   560, -  1680,    +   560, -   560,"
      "    +   560, -   560, \n"
      "   +   560, -   560,    +   560, -  1680,    +   560, -  1680,"
      "    +   560, -  1680, \n"
      "   +   560, -  1680,    +   560, -   560,    +   560, -  1680,"
      "    +   560, -  1680, \n"
      "   +   560, -  1680,    +   560, -   560,    +   560, -   560,"
      "    +   560, -   560, \n"
      "   +   560, -   560,    +   560, -   560,    +   560, -  1680,"
      "    +   560, -   560, \n"
      "   +   560, -   560,    +   560, -  1680,    +   560, -  1680,"
      "    +   560, -  1680, \n"
      "   +   560, -  1680,    +   560, -  1680,    +   560, -   560,"
      "    +   560, -  1680, \n"
      "   +   560, -  1680,    +   560, - 40320\n",
      resultToTimingInfo(&irsend.capture));

  irsend.reset();
  uint16_t rawData[9] = {10, 20, 30, 40, 50, 60, 70, 80, 90};
  irsend.sendRaw(rawData, 9, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(
      "Raw Timing[9]:\n"
      "   +    10, -    20,    +    30, -    40,    +    50, -    60,"
      "    +    70, -    80, \n"
      "   +    90\n",
      resultToTimingInfo(&irsend.capture));
}

TEST(TestResultToHumanReadableBasic, SimpleCodes) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  irsend.reset();
  irsend.sendNEC(irsend.encodeNEC(0x10, 0x20));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(NEC, irsend.capture.decode_type);
  ASSERT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(
      "Protocol  : NEC\n"
      "Code      : 0x8F704FB (32 Bits)\n",
      resultToHumanReadableBasic(&irsend.capture));
}

TEST(TestResultToHumanReadableBasic, ComplexCodes) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  uint8_t state[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                          0x00, 0x00, 0x00, 0x01};

  irsend.reset();
  irsend.sendToshibaAC(state);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(TOSHIBA_AC, irsend.capture.decode_type);
  ASSERT_EQ(kToshibaACBits, irsend.capture.bits);
  EXPECT_EQ(
      "Protocol  : TOSHIBA_AC\n"
      "Code      : 0xF20D03FC0100000001 (72 Bits)\n",
      resultToHumanReadableBasic(&irsend.capture));
}

TEST(TestInvertBits, Normal) {
  ASSERT_EQ(0xAAAA5555AAAA5555, invertBits(0x5555AAAA5555AAAA, 64));
  ASSERT_EQ(0xAAAA5555, invertBits(0x5555AAAA, 32));
  ASSERT_EQ(0xFFFFFFFFFFFFFFFF, invertBits(0x0, 64));
  ASSERT_EQ(0x0, invertBits(invertBits(0x0, 64), 64));
  ASSERT_EQ(0x2, invertBits(0x1, 2));
}

TEST(TestInvertBits, ZeroBits) {
  ASSERT_EQ(0xAAAA5555AAAA5555, invertBits(0xAAAA5555AAAA5555, 0));
  ASSERT_EQ(0x0, invertBits(0x0, 0));
  ASSERT_EQ(0x1, invertBits(0x1, 0));
}

TEST(TestInvertBits, MoreThan64Bits) {
  ASSERT_EQ(0xAAAA5555AAAA5555, invertBits(0x5555AAAA5555AAAA, 70));
  ASSERT_EQ(0xFFFFFFFFFFFFFFFF, invertBits(0x0, 128));
}

TEST(TestCountBits, Pointer) {
  uint8_t data[14] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

  ASSERT_EQ(0, countBits(data, 0));
  ASSERT_EQ(0, countBits(data, 1));
  ASSERT_EQ(0, countBits(data, 1, true));
  ASSERT_EQ(8, countBits(data, 1, false));
  ASSERT_EQ(1, countBits(data, 2));
  ASSERT_EQ(15, countBits(data, 2, false));
  ASSERT_EQ(1, countBits(data + 1, 1));
  ASSERT_EQ(2, countBits(data, 3));
  ASSERT_EQ(4, countBits(data, 4));
  ASSERT_EQ(25, countBits(data, 14));
  ASSERT_EQ(25, countBits(data, 14));
  ASSERT_EQ(14 * 8, countBits(data, 14, true) + countBits(data, 14, false));
  ASSERT_EQ(125, countBits(data, 14, true, 100));
}

TEST(TestCountBits, Integer) {
  uint64_t data = 0xAAAAAAAAAAAAAAAA;

  ASSERT_EQ(0, countBits(data, 0));
  ASSERT_EQ(0, countBits(data, 1));
  ASSERT_EQ(0, countBits(data, 1, true));
  ASSERT_EQ(1, countBits(data, 1, false));
  ASSERT_EQ(1, countBits(data, 3));
  ASSERT_EQ(2, countBits(data, 3, false));
  ASSERT_EQ(4, countBits(data, 8));
  ASSERT_EQ(4, countBits(data, 8, false));
  ASSERT_EQ(32, countBits(data, 64));
  ASSERT_EQ(32, countBits(data, 64, false));

  data = 0;
  ASSERT_EQ(0, countBits(data, 1, true));
  ASSERT_EQ(1, countBits(data, 1, false));
  ASSERT_EQ(0, countBits(data, 64));
  ASSERT_EQ(64, countBits(data, 64, false));

  data = 0xFFFFFFFFFFFFFFFF;
  ASSERT_EQ(1, countBits(data, 1, true));
  ASSERT_EQ(0, countBits(data, 1, false));
  ASSERT_EQ(64, countBits(data, 64));
  ASSERT_EQ(0, countBits(data, 64, false));
}

TEST(TestStrToDecodeType, strToDecodeType) {
  EXPECT_EQ(decode_type_t::NEC, strToDecodeType("NEC"));
  EXPECT_EQ(decode_type_t::KELVINATOR, strToDecodeType("KELVINATOR"));
  EXPECT_EQ(decode_type_t::UNKNOWN, strToDecodeType("foo"));
}

TEST(TestUtils, htmlEscape) {
  EXPECT_EQ("", irutils::htmlEscape(""));
  EXPECT_EQ("No Changes", irutils::htmlEscape("No Changes"));
  EXPECT_EQ("No\tChanges+_%^$@~`\n:\\",
            irutils::htmlEscape("No\tChanges+_%^$@~`\n:\\"));
  EXPECT_EQ("&quot;With Changes&quot;",
            irutils::htmlEscape("\"With Changes\""));
  EXPECT_EQ(
      "&apos;&semi;&excl;&dash;&quot;&lt;&gt;&#equals;&amp;&num;&lcub;&rcub;"
      "&lpar;&rpar;", irutils::htmlEscape("';!-\"<>=&#{}()"));
  EXPECT_EQ("&quot;&quot;", irutils::htmlEscape("\"\""));
  EXPECT_EQ(
      "&amp;quot&semi;&amp;lt&semi;&amp;apos&semi;&amp;gt&semi;&amp;amp&semi;",
      irutils::htmlEscape("&quot;&lt;&apos;&gt;&amp;"));
}

TEST(TestUtils, TemperatureConversion) {
  // Freezing point of water.
  ASSERT_EQ(32.0, celsiusToFahrenheit(0.0));
  ASSERT_EQ(0.0, fahrenheitToCelsius(32.0));
  // Boiling point of water.
  ASSERT_EQ(212.0, celsiusToFahrenheit(100.0));
  ASSERT_EQ(100.0, fahrenheitToCelsius(212.0));
  // Room Temp. (RTP)
  ASSERT_EQ(77.0, celsiusToFahrenheit(25.0));
  ASSERT_EQ(25.0, fahrenheitToCelsius(77.0));
  // Misc
  ASSERT_EQ(-40.0, fahrenheitToCelsius(-40.0));
}

TEST(TestResultToRawArray, TypicalCase) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  // Generate a known message.
  irsend.reset();
  irsend.sendNikai(0xD0F2F);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(NIKAI, irsend.capture.decode_type);
  ASSERT_EQ(kNikaiBits, irsend.capture.bits);
  EXPECT_EQ(
      "uint16_t rawData[52] = {4000, 4000,  500, 2000,  500, 2000,  "
      "500, 2000,  500, 2000,  500, 1000,  500, 1000,  500, 2000,  500, 1000,  "
      "500, 2000,  500, 2000,  500, 2000,  500, 2000,  500, 1000,  500, 1000,  "
      "500, 1000,  500, 1000,  500, 2000,  500, 2000,  500, 1000,  500, 2000,  "
      "500, 1000,  500, 1000,  500, 1000,  500, 1000,  500, 8500 };"
      "  // NIKAI D0F2F\n"
      "uint64_t data = 0xD0F2F;\n",
      resultToSourceCode(&irsend.capture));
  uint16_t rawData[52] = {  // Data taken from above.
      4000, 4000, 500, 2000, 500, 2000, 500, 2000, 500, 2000, 500, 1000, 500,
      1000, 500, 2000, 500, 1000, 500, 2000, 500, 2000, 500, 2000, 500, 2000,
      500, 1000, 500, 1000, 500, 1000, 500, 1000, 500, 2000, 500, 2000, 500,
      1000, 500, 2000, 500, 1000, 500, 1000, 500, 1000, 500, 1000, 500, 8500};
  uint16_t * result = resultToRawArray(&irsend.capture);
  ASSERT_EQ(52, getCorrectedRawLength(&irsend.capture));
  EXPECT_STATE_EQ(rawData, result, getCorrectedRawLength(&irsend.capture));
  if (result != NULL) delete[] result;
}

TEST(TestResultToRawArray, LargeValues) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  uint16_t test_data[9] = {10, 20, 30, 40, 50, 60, 70, 80, 90};
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(test_data, 9, 38000);
  irsend.makeDecodeResult();
  irrecv.decode(&irsend.capture);
  uint16_t * result = resultToRawArray(&irsend.capture);
  ASSERT_EQ(9, getCorrectedRawLength(&irsend.capture));
  EXPECT_STATE_EQ(test_data, result, 9);
  if (result != NULL) delete[] result;
  // Stick in some large values.
  irsend.capture.rawbuf[3] = 60000;
  EXPECT_EQ(
      "uint16_t rawData[11] = {10, 20,  65535, 0,  54465, 40,  50, 60,  70, "
      "80,  90};  // UNKNOWN 54051FFD\n",
      resultToSourceCode(&irsend.capture));
  uint16_t large_test_data[11] = {
      10, 20, 65535, 0, 54465, 40, 50, 60, 70, 80, 90};
  ASSERT_EQ(11, getCorrectedRawLength(&irsend.capture));
  result = resultToRawArray(&irsend.capture);
  EXPECT_STATE_EQ(large_test_data, result, 11);
  if (result != NULL) delete[] result;
}

TEST(TestUtils, TypeStringConversionRangeTests) {
  ASSERT_EQ("UNKNOWN", typeToString((decode_type_t)(kLastDecodeType + 1)));
  ASSERT_EQ("UNKNOWN", typeToString(decode_type_t::UNKNOWN));
  for (int i = 0; i <= kLastDecodeType; i++) {
    EXPECT_NE("UNKNOWN", typeToString((decode_type_t)i)) << "Protocol " << i <<
        " doesn't have a valid string for it.";
    EXPECT_EQ(i, strToDecodeType(typeToString((decode_type_t)i).c_str())) <<
        "Protocol " << typeToString((decode_type_t)i) <<
        " doesn't decode from a string correctly";
  }
}

TEST(TestUtils, MinsToString) {
  EXPECT_EQ("00:00", irutils::minsToString(0));
  EXPECT_EQ("00:01", irutils::minsToString(1));
  EXPECT_EQ("00:10", irutils::minsToString(10));
  EXPECT_EQ("00:59", irutils::minsToString(59));

  EXPECT_EQ("01:00", irutils::minsToString(60));
  EXPECT_EQ("01:01", irutils::minsToString(61));
  EXPECT_EQ("01:59", irutils::minsToString(60 + 59));
  EXPECT_EQ("18:59", irutils::minsToString(18 * 60 + 59));
  EXPECT_EQ("23:59", irutils::minsToString(23 * 60 + 59));
}

TEST(TestUtils, sumNibbles) {
  uint8_t testdata[] = {0x01, 0x23, 0x45};
  EXPECT_EQ(0, irutils::sumNibbles(testdata, 0));
  EXPECT_EQ(1, irutils::sumNibbles(testdata, 0, 1));
  EXPECT_EQ(1, irutils::sumNibbles(testdata, 1));
  EXPECT_EQ(2, irutils::sumNibbles(testdata, 1, 1));
  EXPECT_EQ(15, irutils::sumNibbles(testdata, 3));
  EXPECT_EQ(115, irutils::sumNibbles(testdata, 3, 100));
}

TEST(TestUtils, BCD) {
  EXPECT_EQ(0, irutils::uint8ToBcd(0));
  EXPECT_EQ(0, irutils::bcdToUint8(0));
  EXPECT_EQ(1, irutils::uint8ToBcd(1));
  EXPECT_EQ(10, irutils::bcdToUint8(0x10));
  EXPECT_EQ(0x10, irutils::uint8ToBcd(10));
  EXPECT_EQ(11, irutils::bcdToUint8(0x11));
  EXPECT_EQ(0x11, irutils::uint8ToBcd(11));
  EXPECT_EQ(99, irutils::bcdToUint8(0x99));
  EXPECT_EQ(0x99, irutils::uint8ToBcd(99));
  EXPECT_EQ(255, irutils::bcdToUint8(0x9A));
  EXPECT_EQ(255, irutils::uint8ToBcd(100));
}

TEST(TestUtils, getBit) {
  // uint8_t method.
  EXPECT_FALSE(irutils::getBit((uint8_t)0, 0));
  EXPECT_TRUE(irutils::getBit((uint8_t)1, 0));
  EXPECT_FALSE(irutils::getBit((uint8_t)0b01, 1));
  EXPECT_TRUE(irutils::getBit((uint8_t)0b10, 1));
  EXPECT_FALSE(irutils::getBit((uint8_t)0b01111111, 7));
  EXPECT_TRUE(irutils::getBit((uint8_t)0b10000000, 7));
  // 8-bit macro method
  EXPECT_FALSE(GETBIT8((uint8_t)0, 0));
  EXPECT_TRUE(GETBIT8((uint8_t)1, 0));
  EXPECT_FALSE(GETBIT8((uint8_t)0b01, 1));
  EXPECT_TRUE(GETBIT8((uint8_t)0b10, 1));
  EXPECT_FALSE(GETBIT8((uint8_t)0b01111111, 7));
  EXPECT_TRUE(GETBIT8((uint8_t)0b10000000, 7));
  // uint64_t method.
  EXPECT_FALSE(irutils::getBit((uint64_t)0, 0));
  EXPECT_TRUE(irutils::getBit((uint64_t)1, 0));
  EXPECT_FALSE(irutils::getBit((uint64_t)0b01, 1));
  EXPECT_TRUE(irutils::getBit((uint64_t)0b10, 1));
  EXPECT_FALSE(irutils::getBit((uint64_t)0b01111111, 7));
  EXPECT_TRUE(irutils::getBit((uint64_t)0b10000000, 7));
}

TEST(TestUtils, setBit) {
  // uint8_t method.
  EXPECT_EQ(0, irutils::setBit((uint8_t)0, 0, false));
  EXPECT_EQ(0, irutils::setBit((uint8_t)1, 0, false));
  EXPECT_EQ(1, irutils::setBit((uint8_t)0, 0, true));
  EXPECT_EQ(1, irutils::setBit((uint8_t)1, 0, true));
  EXPECT_EQ(0b101, irutils::setBit((uint8_t)0b101, 1, false));
  EXPECT_EQ(0b100, irutils::setBit((uint8_t)0b110, 1, false));
  EXPECT_EQ(0b111, irutils::setBit((uint8_t)0b101, 1, true));
  EXPECT_EQ(0b110, irutils::setBit((uint8_t)0b110, 1, true));
  EXPECT_EQ(0b11111111, irutils::setBit((uint8_t)0b01111111, 7, true));
  EXPECT_EQ(0, irutils::setBit((uint8_t)0b10000000, 7, false));
  // uint64_t method.
  EXPECT_EQ(0, irutils::setBit((uint64_t)0, 0, false));
  EXPECT_EQ(0, irutils::setBit((uint64_t)1, 0, false));
  EXPECT_EQ(1, irutils::setBit((uint64_t)0, 0, true));
  EXPECT_EQ(1, irutils::setBit((uint64_t)1, 0, true));
  EXPECT_EQ(0b101, irutils::setBit((uint64_t)0b101, 1, false));
  EXPECT_EQ(0b100, irutils::setBit((uint64_t)0b110, 1, false));
  EXPECT_EQ(0b111, irutils::setBit((uint64_t)0b101, 1, true));
  EXPECT_EQ(0b110, irutils::setBit((uint64_t)0b110, 1, true));
  EXPECT_EQ(0b11111111, irutils::setBit((uint64_t)0b01111111, 7, true));
  EXPECT_EQ(0, irutils::setBit((uint64_t)0b10000000, 7, false));
  // uint8_t Pointer method.
  uint8_t data = 0;
  irutils::setBit(&data, 0, false);
  EXPECT_EQ(0, data);
  data = 1;
  irutils::setBit(&data, 0, false);
  ASSERT_EQ(0, data);
  irutils::setBit(&data, 0, true);
  ASSERT_EQ(1, data);
  irutils::setBit(&data, 0, true);
  ASSERT_EQ(1, data);
  // uint64_t Pointer method.
  uint64_t data64 = 0;
  irutils::setBit(&data64, 38, true);
  ASSERT_EQ(1ULL << 38, data64);
  irutils::setBit(&data64, 38, true);
  ASSERT_EQ(1ULL << 38, data64);
}

TEST(TestUtils, setBits8Bit) {
  uint8_t data = 0b00000001;
  // Trivial/corner cases.
  irutils::setBits(&data, 0, 0, 0);
  EXPECT_EQ(1, data);
  irutils::setBits(&data, 0, 0, 17);
  EXPECT_EQ(1, data);
  irutils::setBits(&data, 22, 0, 22);
  EXPECT_EQ(1, data);
  irutils::setBits(&data, 8, 23, 3);
  EXPECT_EQ(1, data);
  irutils::setBits(&data, 8, 0, 3);
  EXPECT_EQ(1, data);
  // Single bit.
  irutils::setBits(&data, 0, 1, 0);
  EXPECT_EQ(0, data);
  irutils::setBits(&data, 0, 1, 1);
  EXPECT_EQ(0b1, data);
  irutils::setBits(&data, 1, 1, 0);
  EXPECT_EQ(0b1, data);
  irutils::setBits(&data, 1, 1, 1);
  EXPECT_EQ(0b11, data);
  irutils::setBits(&data, 1, 1, 0);
  EXPECT_EQ(0b1, data);
  irutils::setBits(&data, 2, 1, 1);
  EXPECT_EQ(0b101, data);
  irutils::setBits(&data, 7, 1, 1);
  EXPECT_EQ(0b10000101, data);
  // Larger value than bits desired to be set.
  irutils::setBits(&data, 5, 1, 255);
  EXPECT_EQ(0b10100101, data);
  // Set multiple bits
  data = 0;
  irutils::setBits(&data, 0, 8, 255);
  EXPECT_EQ(0b11111111, data);
  irutils::setBits(&data, 0, 8, 0);
  EXPECT_EQ(0, data);
  irutils::setBits(&data, 0, 4, 0xF);
  EXPECT_EQ(0xF, data);
  irutils::setBits(&data, 4, 4, 0xF);
  EXPECT_EQ(0xFF, data);
  irutils::setBits(&data, 4, 4, 0x3);
  EXPECT_EQ(0x3F, data);
  irutils::setBits(&data, 3, 4, 0x3);
  EXPECT_EQ(0x1F, data);
  irutils::setBits(&data, 1, 4, 0x3);
  EXPECT_EQ(0b00000111, data);
  irutils::setBits(&data, 1, 4, 0b1001);
  EXPECT_EQ(0b00010011, data);
  // Partial overrun.
  irutils::setBits(&data, 6, 4, 0b1001);
  EXPECT_EQ(0b01010011, data);
  irutils::setBits(&data, 7, 4, 0b1001);
  EXPECT_EQ(0b11010011, data);
}

TEST(TestUtils, setBits64Bit) {
  uint64_t data = 1;
  // Trivial/corner cases.
  irutils::setBits(&data, 0, 0, 0);
  EXPECT_EQ(1, data);
  irutils::setBits(&data, 0, 0, 17);
  EXPECT_EQ(1, data);
  irutils::setBits(&data, 100, 0, 22);
  EXPECT_EQ(1, data);
  irutils::setBits(&data, 64, 23, 3);
  EXPECT_EQ(1, data);
  irutils::setBits(&data, 64, 0, 3);
  EXPECT_EQ(1, data);
  // Single bit.
  irutils::setBits(&data, 0, 1, 0);
  EXPECT_EQ(0, data);
  irutils::setBits(&data, 0, 1, 1);
  EXPECT_EQ(0b1, data);
  irutils::setBits(&data, 1, 1, 0);
  EXPECT_EQ(0b1, data);
  irutils::setBits(&data, 1, 1, 1);
  EXPECT_EQ(0b11, data);
  irutils::setBits(&data, 1, 1, 0);
  EXPECT_EQ(0b1, data);
  irutils::setBits(&data, 2, 1, 1);
  EXPECT_EQ(0b101, data);
  irutils::setBits(&data, 7, 1, 1);
  EXPECT_EQ(0b10000101, data);
  // Larger value than bits desired to be set.
  irutils::setBits(&data, 5, 1, 255);
  EXPECT_EQ(0b10100101, data);
  // Set multiple bits
  data = 0;
  irutils::setBits(&data, 0, 8, 255);
  EXPECT_EQ(0b11111111, data);
  irutils::setBits(&data, 0, 8, 0);
  EXPECT_EQ(0, data);
  irutils::setBits(&data, 0, 4, 0xF);
  EXPECT_EQ(0xF, data);
  irutils::setBits(&data, 4, 4, 0xF);
  EXPECT_EQ(0xFF, data);
  irutils::setBits(&data, 4, 4, 0x3);
  EXPECT_EQ(0x3F, data);
  irutils::setBits(&data, 3, 4, 0x3);
  EXPECT_EQ(0x1F, data);
  irutils::setBits(&data, 1, 4, 0x3);
  EXPECT_EQ(0b00000111, data);
  irutils::setBits(&data, 1, 4, 0b1001);
  EXPECT_EQ(0b00010011, data);
  // Partial overrun.
  irutils::setBits(&data, 62, 4, 0b1001);
  EXPECT_EQ(0x4000000000000013, data);
  // General
  irutils::setBits(&data, 32, 4, 0b1001);
  EXPECT_EQ(0x4000000900000013, data);
}
