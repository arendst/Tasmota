// Copyright 2020 David Conran

#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for decodeMultibrackets().

// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1103

TEST(TestDecodeMultibrackets, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  // The 1 + ok keypress:
  uint16_t rawData_1[7] = {20100, 20472, 15092, 30704, 20102, 20472, 15086};
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData_1, 7, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::MULTIBRACKETS, irsend.capture.decode_type);
  ASSERT_EQ(kMultibracketsBits, irsend.capture.bits);
  EXPECT_EQ(0x87, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  // ok keypress.
  const uint16_t rawData_2[11] = {
      25124, 5108, 5038, 5110, 5034, 40940, 25132, 5108, 5036, 5110, 5036};

  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData_2, 11, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::MULTIBRACKETS, irsend.capture.decode_type);
  ASSERT_EQ(kMultibracketsBits, irsend.capture.bits);
  EXPECT_EQ(0xD4, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

TEST(TestDecodeMultibrackets, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  irsend.reset();
  irsend.sendMultibrackets(0x87);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::MULTIBRACKETS, irsend.capture.decode_type);
  EXPECT_EQ(kMultibracketsBits, irsend.capture.bits);
  EXPECT_EQ(0x87, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  // Real data is:
  //   uint16_t rawData[7] = {20100, 20472, 15092, 30704, 20102, 20472, 15086};

  EXPECT_EQ(
      "f38000d50m20000s20000m15000s30000m20000s20000m15000s30000",
      irsend.outputStr());
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("MULTIBRACKETS", typeToString(decode_type_t::MULTIBRACKETS));
  ASSERT_EQ(decode_type_t::MULTIBRACKETS, strToDecodeType("MULTIBRACKETS"));
  ASSERT_FALSE(hasACState(decode_type_t::MULTIBRACKETS));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::MULTIBRACKETS));
  ASSERT_EQ(kMultibracketsBits,
            IRsend::defaultBits(decode_type_t::MULTIBRACKETS));
  ASSERT_EQ(kMultibracketsDefaultRepeat,
            IRsend::minRepeats(decode_type_t::MULTIBRACKETS));
}

TEST(TestDecodeMultibrackets, ShortNoRepeatExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  // The 1 + ok keypress: (edited to be bare minimum)
  uint16_t rawData[3] = {20100, 20472, 15092};
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 3, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::MULTIBRACKETS, irsend.capture.decode_type);
  ASSERT_EQ(kMultibracketsBits, irsend.capture.bits);
  EXPECT_EQ(0x87, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}
