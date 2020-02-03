// Copyright 2017 David Conran

#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendDenon().

// Test sending typical data only.
TEST(TestSendDenon, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendDenon(0x2278);  // Denon AVR Power On. (Sharp)
  EXPECT_EQ(
      "f38000d33"
      "m260s780m260s1820m260s780m260s780m260s780m260s1820m260s780m260s780"
      "m260s1820m260s1820m260s1820m260s1820m260s780m260s780m260s780"
      "m260s43602"
      "m260s780m260s1820m260s780m260s780m260s780m260s780m260s1820m260s1820"
      "m260s780m260s780m260s780m260s780m260s1820m260s1820m260s1820"
      "m260s43602",
      irsend.outputStr());

  irsend.reset();
  // Denon Eco Mode On. (Panasonic/Kaseikyo)
  irsend.sendDenon(0x2A4C028D6CE3, kDenon48Bits);
  EXPECT_EQ(
      "f36700d50"
      "m3456s1728"
      "m432s432m432s432m432s1296m432s432m432s1296m432s432m432s1296m432s432"
      "m432s432m432s1296m432s432m432s432m432s1296m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s1296m432s432"
      "m432s1296m432s432m432s432m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s432m432s1296m432s1296m432s432m432s1296m432s1296m432s432m432s432"
      "m432s1296m432s1296m432s1296m432s432m432s432m432s432m432s1296m432s1296"
      "m432s98928",
      irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendDenon, SendNormalWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendDenon(0x2278, kDenonBits, 1);  // 1 repeat.
  EXPECT_EQ(
      "f38000d33"
      "m260s780m260s1820m260s780m260s780m260s780m260s1820m260s780m260s780"
      "m260s1820m260s1820m260s1820m260s1820m260s780m260s780m260s780"
      "m260s43602"
      "m260s780m260s1820m260s780m260s780m260s780m260s780m260s1820m260s1820"
      "m260s780m260s780m260s780m260s780m260s1820m260s1820m260s1820"
      "m260s43602"
      "m260s780m260s1820m260s780m260s780m260s780m260s1820m260s780m260s780"
      "m260s1820m260s1820m260s1820m260s1820m260s780m260s780m260s780"
      "m260s43602"
      "m260s780m260s1820m260s780m260s780m260s780m260s780m260s1820m260s1820"
      "m260s780m260s780m260s780m260s780m260s1820m260s1820m260s1820"
      "m260s43602",
      irsend.outputStr());
  irsend.sendDenon(0x2278, kDenonBits, 2);  // 2 repeats.
  EXPECT_EQ(
      "f38000d33"
      "m260s780m260s1820m260s780m260s780m260s780m260s1820m260s780m260s780"
      "m260s1820m260s1820m260s1820m260s1820m260s780m260s780m260s780"
      "m260s43602"
      "m260s780m260s1820m260s780m260s780m260s780m260s780m260s1820m260s1820"
      "m260s780m260s780m260s780m260s780m260s1820m260s1820m260s1820"
      "m260s43602"
      "m260s780m260s1820m260s780m260s780m260s780m260s1820m260s780m260s780"
      "m260s1820m260s1820m260s1820m260s1820m260s780m260s780m260s780"
      "m260s43602"
      "m260s780m260s1820m260s780m260s780m260s780m260s780m260s1820m260s1820"
      "m260s780m260s780m260s780m260s780m260s1820m260s1820m260s1820"
      "m260s43602"
      "m260s780m260s1820m260s780m260s780m260s780m260s1820m260s780m260s780"
      "m260s1820m260s1820m260s1820m260s1820m260s780m260s780m260s780"
      "m260s43602"
      "m260s780m260s1820m260s780m260s780m260s780m260s780m260s1820m260s1820"
      "m260s780m260s780m260s780m260s780m260s1820m260s1820m260s1820"
      "m260s43602",
      irsend.outputStr());
}

TEST(TestSendDenon, Send48BitWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendDenon(0x2A4C028D6CE3, kDenon48Bits, 1);  // 1 repeat.
  EXPECT_EQ(
      "f36700d50"
      "m3456s1728"
      "m432s432m432s432m432s1296m432s432m432s1296m432s432m432s1296m432s432"
      "m432s432m432s1296m432s432m432s432m432s1296m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s1296m432s432"
      "m432s1296m432s432m432s432m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s432m432s1296m432s1296m432s432m432s1296m432s1296m432s432m432s432"
      "m432s1296m432s1296m432s1296m432s432m432s432m432s432m432s1296m432s1296"
      "m432s98928"
      "m3456s1728"
      "m432s432m432s432m432s1296m432s432m432s1296m432s432m432s1296m432s432"
      "m432s432m432s1296m432s432m432s432m432s1296m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s1296m432s432"
      "m432s1296m432s432m432s432m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s432m432s1296m432s1296m432s432m432s1296m432s1296m432s432m432s432"
      "m432s1296m432s1296m432s1296m432s432m432s432m432s432m432s1296m432s1296"
      "m432s98928",
      irsend.outputStr());
  irsend.sendDenon(0x2A4C028D6CE3, kDenon48Bits, 2);  // 2 repeats.
  EXPECT_EQ(
      "f36700d50"
      "m3456s1728"
      "m432s432m432s432m432s1296m432s432m432s1296m432s432m432s1296m432s432"
      "m432s432m432s1296m432s432m432s432m432s1296m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s1296m432s432"
      "m432s1296m432s432m432s432m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s432m432s1296m432s1296m432s432m432s1296m432s1296m432s432m432s432"
      "m432s1296m432s1296m432s1296m432s432m432s432m432s432m432s1296m432s1296"
      "m432s98928"
      "m3456s1728"
      "m432s432m432s432m432s1296m432s432m432s1296m432s432m432s1296m432s432"
      "m432s432m432s1296m432s432m432s432m432s1296m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s1296m432s432"
      "m432s1296m432s432m432s432m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s432m432s1296m432s1296m432s432m432s1296m432s1296m432s432m432s432"
      "m432s1296m432s1296m432s1296m432s432m432s432m432s432m432s1296m432s1296"
      "m432s98928"
      "m3456s1728"
      "m432s432m432s432m432s1296m432s432m432s1296m432s432m432s1296m432s432"
      "m432s432m432s1296m432s432m432s432m432s1296m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s1296m432s432"
      "m432s1296m432s432m432s432m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s432m432s1296m432s1296m432s432m432s1296m432s1296m432s432m432s432"
      "m432s1296m432s1296m432s1296m432s432m432s432m432s432m432s1296m432s1296"
      "m432s98928",
      irsend.outputStr());
}

// Test sending an atypical data size.
TEST(TestSendDenon, SendUnusualSize) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendDenon(0x12, 8);
  EXPECT_EQ(
      "f38000d33"
      "m260s780m260s780m260s780m260s1820m260s780m260s780m260s1820m260s780"
      "m260s43602"
      "m260s1820m260s1820m260s1820m260s780m260s1820m260s1820m260s780m260s1820"
      "m260s43602",
      irsend.outputStr());

  irsend.reset();
  irsend.sendDenon(0x1234567890ABCDEF, 64);
  EXPECT_EQ(
      "f36700d50"
      "m3456s1728"
      "m432s432m432s432m432s432m432s1296m432s432m432s432m432s1296m432s432"
      "m432s432m432s432m432s1296m432s1296m432s432m432s1296m432s432m432s432"
      "m432s432m432s1296m432s432m432s1296m432s432m432s1296m432s1296m432s432"
      "m432s432m432s1296m432s1296m432s1296m432s1296m432s432m432s432m432s432"
      "m432s1296m432s432m432s432m432s1296m432s432m432s432m432s432m432s432"
      "m432s1296m432s432m432s1296m432s432m432s1296m432s432m432s1296m432s1296"
      "m432s1296m432s1296m432s432m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s1296m432s1296m432s1296m432s432m432s1296m432s1296m432s1296m432s1296"
      "m432s74736",
      irsend.outputStr());
}

// Tests for decodeDenon().

// Decode normal Denon messages.
TEST(TestDecodeDenon, NormalDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Denon 15-bit message. (Sharp)
  irsend.reset();
  irsend.sendDenon(0x2278);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodeDenon(&irsend.capture, kDenonBits, true));
  EXPECT_EQ(DENON, irsend.capture.decode_type);
  EXPECT_EQ(kDenonBits, irsend.capture.bits);
  EXPECT_EQ(0x2278, irsend.capture.value);
  EXPECT_EQ(0x2, irsend.capture.address);
  EXPECT_EQ(0x79, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Legacy Denon 14-bit message.
  irsend.reset();
  irsend.sendDenon(0x1278, kDenonLegacyBits);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodeDenon(&irsend.capture, kDenonLegacyBits, true));
  EXPECT_EQ(DENON, irsend.capture.decode_type);
  EXPECT_EQ(kDenonLegacyBits, irsend.capture.bits);
  EXPECT_EQ(0x1278, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal Denon 48-bit message. (Panasonic/Kaseikyo)
  irsend.reset();
  irsend.sendDenon(0x2A4C028D6CE3, kDenon48Bits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeDenon(&irsend.capture, kDenon48Bits, true));
  EXPECT_EQ(DENON, irsend.capture.decode_type);
  EXPECT_EQ(kDenon48Bits, irsend.capture.bits);
  EXPECT_EQ(0x2A4C028D6CE3, irsend.capture.value);
  EXPECT_EQ(0x2A4C, irsend.capture.address);
  EXPECT_EQ(0x028D6CE3, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode a 'real' example via GlobalCache
TEST(TestDecodeDenon, DecodeGlobalCacheExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Denon AVR series Power On code from Global Cache. (Sharp style)
  uint16_t gc_test_power[67] = {
      38000, 1,  1,  10, 30, 10, 70, 10, 30, 10, 30, 10, 30, 10, 70, 10,  30,
      10,    30, 10, 70, 10, 70, 10, 70, 10, 70, 10, 30, 10, 30, 10, 30,  10,
      1657,  10, 30, 10, 70, 10, 30, 10, 30, 10, 30, 10, 30, 10, 70, 10,  70,
      10,    30, 10, 30, 10, 30, 10, 30, 10, 70, 10, 70, 10, 70, 10, 1657};
  irsend.sendGC(gc_test_power, 67);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodeDenon(&irsend.capture, kDenonBits, true));
  EXPECT_EQ(DENON, irsend.capture.decode_type);
  EXPECT_EQ(kDenonBits, irsend.capture.bits);
  EXPECT_EQ(0x2278, irsend.capture.value);
  EXPECT_EQ(0x2, irsend.capture.address);
  EXPECT_EQ(0x79, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Denon "Eco Mode Auto" code from Global Cache. (Panasonic style)
  uint16_t gc_test_eco[103] = {
      37000, 1,  1,  128, 64, 16, 16, 16, 16, 16, 48, 16, 16,  16, 48,
      16,    16, 16, 48,  16, 16, 16, 16, 16, 48, 16, 16, 16,  16, 16,
      48,    16, 48, 16,  16, 16, 16, 16, 16, 16, 16, 16, 16,  16, 16,
      16,    16, 16, 16,  16, 48, 16, 16, 16, 48, 16, 16, 16,  16, 16,
      16,    16, 48, 16,  48, 16, 16, 16, 48, 16, 16, 16, 48,  16, 48,
      16,    16, 16, 48,  16, 48, 16, 16, 16, 16, 16, 48, 16,  48, 16,
      48,    16, 16, 16,  16, 16, 16, 16, 48, 16, 48, 16, 2766};
  irsend.reset();
  irsend.sendGC(gc_test_eco, 103);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodeDenon(&irsend.capture, kDenon48Bits, true));
  EXPECT_EQ(DENON, irsend.capture.decode_type);
  EXPECT_EQ(kDenon48Bits, irsend.capture.bits);
  EXPECT_EQ(0x2A4C028D6CE3, irsend.capture.value);
  EXPECT_EQ(0x2A4C, irsend.capture.address);
  EXPECT_EQ(0x028D6CE3, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Fail to decode a non-Denon example via GlobalCache
TEST(TestDecodeDenon, FailToDecodeNonDenonExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  uint16_t gc_test[39] = {38000, 1,  1,  322, 162, 20, 61,  20, 61, 20,
                          20,    20, 20, 20,  20,  20, 127, 20, 61, 9,
                          20,    20, 61, 20,  20,  20, 61,  20, 61, 20,
                          61,    20, 20, 20,  20,  20, 20,  20, 884};
  irsend.sendGC(gc_test, 39);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeDenon(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeDenon(&irsend.capture, kDenonLegacyBits, false));
  ASSERT_FALSE(irrecv.decodeDenon(&irsend.capture, kDenonBits, false));
  ASSERT_FALSE(irrecv.decodeDenon(&irsend.capture, kDenon48Bits, false));
}
