// Copyright 2022 crankyoldgit (David Conran)

#include "IRac.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "IRutils.h"
#include "gtest/gtest.h"


// General housekeeping
TEST(TestToto, Housekeeping) {
  ASSERT_EQ("TOTO", typeToString(TOTO));
  ASSERT_FALSE(hasACState(TOTO));
  ASSERT_EQ(kTotoBits, IRsend::defaultBits(decode_type_t::TOTO));
  ASSERT_EQ(kSingleRepeat, IRsend::minRepeats(decode_type_t::TOTO));
}

// Tests for decodeToto().

// Decode normal Toto messages.
TEST(TestDecodeToto, SyntheticShortDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Short Toto 24-bit message.
  irsend.reset();
  irsend.sendToto(0x0D0D00);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(TOTO, irsend.capture.decode_type);
  EXPECT_EQ(kTotoBits, irsend.capture.bits);
  EXPECT_EQ(0x0D0D00, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0x0D00, irsend.capture.command);
}

// Decode real example via Issue #1806
TEST(TestDecodeToto, RealShortDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  // Toto Full Flush from Issue #1806
  const uint16_t rawData[163] = {
      6266, 2734,
      598, 540, 598, 1626, 598, 512, 622, 516, 598, 514, 598, 510, 598, 514,
      628, 512, 596, 514, 600, 512, 598, 538, 600, 1622, 600, 512, 598, 540,
      602, 510, 598, 512, 598, 512, 624, 514, 598, 512, 598, 512, 598, 514,
      624, 512, 598, 514, 598, 1652, 596, 514, 598, 1626, 598, 1650, 598, 514,
      598, 512, 598, 540, 598, 514, 596, 1626, 626, 512, 574, 1648, 598, 1650,
      598, 514, 598, 512, 594, 544, 596, 514,
      598, 37996,
      6182, 2764,
      598, 514, 600, 1648, 600, 512, 596, 514, 598, 540, 598, 512, 600, 512,
      598, 512, 624, 514, 598, 514, 598, 512, 596, 1652, 598, 514, 598, 512,
      596, 540, 598, 514, 598, 512, 598, 512, 598, 540, 596, 516, 596, 514, 598,
      512, 574, 564, 598, 1626, 568, 542, 624, 1624, 626, 1622, 598, 514, 596,
      514, 598, 514, 596, 540, 600, 1622, 598, 512, 600, 1650, 598, 1624, 596,
      540, 600, 512, 598, 514, 596, 514,
      622};  // UNKNOWN 43BD67B3

  irsend.sendRaw(rawData, 163, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(TOTO, irsend.capture.decode_type);
  EXPECT_EQ(kTotoBits, irsend.capture.bits);
  EXPECT_EQ(0x0D0D00, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0x0D00, irsend.capture.command);
}

// Decode real example via Issue #1806
TEST(TestDecodeToto, RealLongDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  // Oscillate Bidet Function from Issue #1806
  const uint16_t rawData[491] = {
      6262, 2738,
      600, 538, 596, 1624, 598, 512, 652, 486, 628, 484, 600, 510, 596, 516,
      650, 488, 622, 488, 626, 484, 598, 540, 600, 1624, 602, 510, 596, 540,
      602, 510, 598, 514, 598, 512, 626, 512, 598, 512, 598, 512, 598, 514,
      626, 510, 598, 514, 598, 512, 598, 1650, 600, 1622, 600, 538, 596, 514,
      600, 510, 596, 514, 596, 542, 598, 514, 598, 1624, 626, 1622, 598, 512,
      600, 512, 620, 516, 598, 512, 598, 514,
      598, 40244,
      6184, 2764,
      598, 514, 598, 1650, 598, 514, 596, 516, 596, 542, 598, 512, 596, 516,
      600, 510, 624, 512, 598, 512, 596, 516, 598, 1652, 600, 512, 598, 512,
      598, 540, 596, 518, 594, 514, 598, 512, 596, 542, 598, 512, 596, 514,
      598, 512, 598, 540, 598, 514, 598, 1622, 624, 1626, 598, 514, 598, 512,
      626, 512, 596, 514, 596, 514, 596, 540, 598, 1624, 600, 1650, 600, 512,
      600, 510, 598, 512, 596, 540, 598, 512,
      600, 40244,
      6186, 2760,
      600, 512, 596, 1626, 624, 514, 598, 512, 600, 512, 598, 512, 626, 512,
      598, 512, 598, 512, 596, 540, 600, 510, 600, 1622, 600, 538, 596, 514,
      596, 514, 598, 514, 624, 514, 596, 516, 596, 514, 600, 512, 622, 514, 600,
      512, 600, 510, 600, 538, 602, 1622, 600, 1648, 626, 512, 572, 512, 598,
      514, 650, 488, 598, 512, 594, 516, 598, 1652, 598, 1626, 598, 514, 624,
      514, 596, 512, 600, 512, 598, 540,
      596, 40246,
      6184, 2736,
      598, 538, 598, 1624, 598, 512, 598, 540, 598, 512, 600, 512, 596, 514,
      600, 538, 626, 486, 598, 514, 596, 514, 652, 1596, 594, 516, 624, 514,
      594, 516, 598, 514, 622, 490, 596, 540, 596, 514, 624, 488, 594, 514, 598,
      538, 598, 512, 598, 514, 594, 516, 596, 540, 600, 1622, 572, 566, 600,
      512, 570, 542, 598, 512, 598, 540, 600, 512, 594, 516, 598, 1652, 600,
      512, 598, 514, 596, 514, 624, 514,
      598, 42468,
      6182, 2764,
      624, 490, 594, 1652, 600, 512, 596, 514, 596, 540, 600, 512, 598, 516,
      596, 514, 598, 538, 598, 516, 598, 512, 570, 1678, 596, 514, 596, 514,
      598, 512, 624, 512, 594, 516, 596, 514, 600, 510, 626, 512, 596, 516,
      594, 516, 598, 538, 598, 512, 596, 514, 596, 514, 596, 1652, 600, 514,
      594, 516, 622, 514, 600, 512, 598, 514, 596, 514, 624, 514, 596, 1626,
      598, 514, 622, 516, 594, 516, 598, 514,
      596, 42496,
      6184, 2764,
      596, 516, 598, 1624, 626, 512, 598, 512, 596, 516, 600, 512, 624, 514,
      598, 514, 594, 516, 596, 516, 620, 516, 600, 1624, 596, 540, 598, 514,
      600, 512, 596, 516, 596, 542, 594, 516, 598, 514, 594, 516, 624, 512, 598,
      512, 596, 514, 596, 514, 624, 514, 596, 516, 598, 1650, 594, 518, 596,
      514, 596, 516, 596, 542, 596, 514, 596, 516, 596, 514, 596, 1652, 598,
      514, 596, 514, 624, 514, 622, 488, 596};  // UNKNOWN 4AC5E8B3


  irsend.sendRaw(rawData, 491, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(TOTO, irsend.capture.decode_type);
  EXPECT_EQ(kTotoLongBits, irsend.capture.bits);
  EXPECT_EQ(0x60600080800, irsend.capture.value);
  EXPECT_EQ(0x0600, irsend.capture.address);
  EXPECT_EQ(0x0800, irsend.capture.command);
}

TEST(TestDecodeToto, SyntheticLongDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Long Toto 48-bit message.
  irsend.reset();
  irsend.sendToto(0x60600080800, kTotoLongBits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(TOTO, irsend.capture.decode_type);
  EXPECT_EQ(kTotoLongBits, irsend.capture.bits);
  EXPECT_EQ(0x60600080800, irsend.capture.value);
  EXPECT_EQ(0x0600, irsend.capture.address);
  EXPECT_EQ(0x0800, irsend.capture.command);
}
