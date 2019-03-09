// Copyright 2017 David Conran
// Copyright 2018 denxhun

#include "ir_Mitsubishi.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendMitsubishi().

// Test sending typical data only.
TEST(TestSendMitsubishi, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendMitsubishi(0xE242);
  EXPECT_EQ(
      "m300s2100m300s2100m300s2100m300s900m300s900m300s900m300s2100m300s900"
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s900"
      "m300s28080"
      "m300s2100m300s2100m300s2100m300s900m300s900m300s900m300s2100m300s900"
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s900"
      "m300s28080",
      irsend.outputStr());

  irsend.reset();
  irsend.sendMitsubishi(0x0);
  EXPECT_EQ(
      "m300s900m300s900m300s900m300s900m300s900m300s900m300s900m300s900"
      "m300s900m300s900m300s900m300s900m300s900m300s900m300s900m300s900"
      "m300s34080"
      "m300s900m300s900m300s900m300s900m300s900m300s900m300s900m300s900"
      "m300s900m300s900m300s900m300s900m300s900m300s900m300s900m300s900"
      "m300s34080",
      irsend.outputStr());

  irsend.reset();
  irsend.sendMitsubishi(0x4321);
  EXPECT_EQ(
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s2100"
      "m300s900m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100"
      "m300s28080"
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s2100"
      "m300s900m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100"
      "m300s28080",
      irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendMitsubishi, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendMitsubishi(0xE242, kMitsubishiBits, 0);  // 0 repeat.
  EXPECT_EQ(
      "m300s2100m300s2100m300s2100m300s900m300s900m300s900m300s2100m300s900"
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s900"
      "m300s28080",
      irsend.outputStr());

  irsend.reset();
  irsend.sendMitsubishi(0xE242, kMitsubishiBits, 1);  // 1 repeat.
  EXPECT_EQ(
      "m300s2100m300s2100m300s2100m300s900m300s900m300s900m300s2100m300s900"
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s900"
      "m300s28080"
      "m300s2100m300s2100m300s2100m300s900m300s900m300s900m300s2100m300s900"
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s900"
      "m300s28080",
      irsend.outputStr());
  irsend.sendMitsubishi(0xE242, kMitsubishiBits, 2);  // 2 repeats.
  EXPECT_EQ(
      "m300s2100m300s2100m300s2100m300s900m300s900m300s900m300s2100m300s900"
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s900"
      "m300s28080"
      "m300s2100m300s2100m300s2100m300s900m300s900m300s900m300s2100m300s900"
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s900"
      "m300s28080"
      "m300s2100m300s2100m300s2100m300s900m300s900m300s900m300s2100m300s900"
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s900"
      "m300s28080",
      irsend.outputStr());
}

// Test sending an atypical data size.
TEST(TestSendMitsubishi, SendUnusualSize) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendMitsubishi(0x0, 8);
  EXPECT_EQ(
      "m300s900m300s900m300s900m300s900m300s900m300s900m300s900m300s900"
      "m300s43680"
      "m300s900m300s900m300s900m300s900m300s900m300s900m300s900m300s900"
      "m300s43680",
      irsend.outputStr());

  irsend.reset();
  irsend.sendMitsubishi(0x1234567890ABCDEF, 64);
  EXPECT_EQ(
      "m300s900m300s900m300s900m300s2100m300s900m300s900m300s2100m300s900"
      "m300s900m300s900m300s2100m300s2100m300s900m300s2100m300s900m300s900"
      "m300s900m300s2100m300s900m300s2100m300s900m300s2100m300s2100m300s900"
      "m300s900m300s2100m300s2100m300s2100m300s2100m300s900m300s900m300s900"
      "m300s2100m300s900m300s900m300s2100m300s900m300s900m300s900m300s900"
      "m300s2100m300s900m300s2100m300s900m300s2100m300s900m300s2100m300s2100"
      "m300s2100m300s2100m300s900m300s900m300s2100m300s2100m300s900m300s2100"
      "m300s2100m300s2100m300s2100m300s900m300s2100m300s2100m300s2100m300s2100"
      "m300s28080"
      "m300s900m300s900m300s900m300s2100m300s900m300s900m300s2100m300s900"
      "m300s900m300s900m300s2100m300s2100m300s900m300s2100m300s900m300s900"
      "m300s900m300s2100m300s900m300s2100m300s900m300s2100m300s2100m300s900"
      "m300s900m300s2100m300s2100m300s2100m300s2100m300s900m300s900m300s900"
      "m300s2100m300s900m300s900m300s2100m300s900m300s900m300s900m300s900"
      "m300s2100m300s900m300s2100m300s900m300s2100m300s900m300s2100m300s2100"
      "m300s2100m300s2100m300s900m300s900m300s2100m300s2100m300s900m300s2100"
      "m300s2100m300s2100m300s2100m300s900m300s2100m300s2100m300s2100m300s2100"
      "m300s28080",
      irsend.outputStr());
}

// Decode normal Mitsubishi messages.
TEST(TestDecodeMitsubishi, NormalDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Mitsubishi 16-bit message.
  irsend.reset();
  irsend.sendMitsubishi(0xC2B8);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeMitsubishi(&irsend.capture, kMitsubishiBits, true));
  EXPECT_EQ(MITSUBISHI, irsend.capture.decode_type);
  EXPECT_EQ(kMitsubishiBits, irsend.capture.bits);
  EXPECT_EQ(0xC2B8, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  irsend.reset();
  irsend.sendMitsubishi(0x0);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeMitsubishi(&irsend.capture, kMitsubishiBits, true));
  EXPECT_EQ(MITSUBISHI, irsend.capture.decode_type);
  EXPECT_EQ(kMitsubishiBits, irsend.capture.bits);
  EXPECT_EQ(0x0, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  irsend.reset();
  irsend.sendMitsubishi(0xFFFF);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeMitsubishi(&irsend.capture, kMitsubishiBits, true));
  EXPECT_EQ(MITSUBISHI, irsend.capture.decode_type);
  EXPECT_EQ(kMitsubishiBits, irsend.capture.bits);
  EXPECT_EQ(0xFFFF, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Non-standard Mitsubishi sizes should fail with strict.
  irsend.reset();
  // 12 bits.
  irsend.sendMitsubishi(0xFFF, 12);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeMitsubishi(&irsend.capture, kMitsubishiBits, true));
  ASSERT_FALSE(irrecv.decodeMitsubishi(&irsend.capture, 12, true));
  ASSERT_FALSE(irrecv.decodeMitsubishi(&irsend.capture, 64, true));

  // 32 bits.
  irsend.sendMitsubishi(0xFFF, 32);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeMitsubishi(&irsend.capture, kMitsubishiBits, true));
  ASSERT_FALSE(irrecv.decodeMitsubishi(&irsend.capture, 12, true));
  ASSERT_FALSE(irrecv.decodeMitsubishi(&irsend.capture, 32, true));
  ASSERT_FALSE(irrecv.decodeMitsubishi(&irsend.capture, 64, true));
}

// Decode normal repeated Mitsubishi messages.
TEST(TestDecodeMitsubishi, NormalDecodeWithRepeatAndStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Mitsubishi 16-bit message with 2 repeats.
  irsend.reset();
  irsend.sendMitsubishi(0xC2B8, kMitsubishiBits, 2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeMitsubishi(&irsend.capture, kMitsubishiBits, true));
  EXPECT_EQ(MITSUBISHI, irsend.capture.decode_type);
  EXPECT_EQ(kMitsubishiBits, irsend.capture.bits);
  EXPECT_EQ(0xC2B8, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal Mitsubishi 16-bit message with 0 repeats.
  irsend.reset();
  irsend.sendMitsubishi(0xC2B8, kMitsubishiBits, 0);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeMitsubishi(&irsend.capture, kMitsubishiBits, true));
  EXPECT_EQ(MITSUBISHI, irsend.capture.decode_type);
  EXPECT_EQ(kMitsubishiBits, irsend.capture.bits);
  EXPECT_EQ(0xC2B8, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode unsupported Mitsubishi messages.
TEST(TestDecodeMitsubishi, DecodeWithNonStrictValues) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  irsend.sendMitsubishi(0x0, 8);  // Illegal sized Mitsubishi 8-bit message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeMitsubishi(&irsend.capture, kMitsubishiBits, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeMitsubishi(&irsend.capture, 8, false));
  EXPECT_EQ(MITSUBISHI, irsend.capture.decode_type);
  EXPECT_EQ(8, irsend.capture.bits);
  EXPECT_EQ(0x0, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  ASSERT_FALSE(irrecv.decodeMitsubishi(&irsend.capture, 64, false));

  irsend.reset();
  // Illegal sized Mitsubishi 64-bit message.
  irsend.sendMitsubishi(0xFEDCBA9876543210, 64);
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeMitsubishi(&irsend.capture, kMitsubishiBits, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeMitsubishi(&irsend.capture, 64, false));
  EXPECT_EQ(MITSUBISHI, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0xFEDCBA9876543210, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  // Should fail when we are after a shorter message than we got.
  ASSERT_FALSE(irrecv.decodeMitsubishi(&irsend.capture, 8, false));
}

// Decode a 'real' example via GlobalCache
TEST(TestDecodeMitsubishi, DecodeGlobalCacheExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Mitsubishi "Power On" (16-bit) code from Global Cache.
  uint16_t gc_test[37] = {33000, 1,  1,  10, 70, 10, 70, 10, 70, 10, 30, 10, 30,
                          10,    30, 10, 70, 10, 30, 10, 30, 10, 70, 10, 30, 10,
                          30,    10, 30, 10, 30, 10, 70, 10, 30, 10, 936};
  irsend.sendGC(gc_test, 37);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodeMitsubishi(&irsend.capture));
  EXPECT_EQ(MITSUBISHI, irsend.capture.decode_type);
  EXPECT_EQ(kMitsubishiBits, irsend.capture.bits);
  EXPECT_EQ(0xE242, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Fail to decode a non-Mitsubishi example via GlobalCache
TEST(TestDecodeMitsubishi, FailToDecodeNonMitsubishiExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Modified a few entries to unexpected values, based on previous test case.
  uint16_t gc_test[39] = {38000, 1,  1,  322, 162, 20, 61,  20, 61, 20,
                          20,    20, 20, 20,  20,  20, 127, 20, 61, 9,
                          20,    20, 61, 20,  20,  20, 61,  20, 61, 20,
                          61,    20, 20, 20,  20,  20, 20,  20, 884};
  irsend.sendGC(gc_test, 39);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeMitsubishi(&irsend.capture));
  ASSERT_FALSE(
      irrecv.decodeMitsubishi(&irsend.capture, kMitsubishiBits, false));
}

// Tests for Mitsubishi A/C methods.

// Test sending typical data only.
TEST(TestSendMitsubishiAC, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  uint8_t mitsub_code[kMitsubishiACStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x20, 0x08, 0x06, 0x30,
      0x45, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F};
  irsend.reset();
  irsend.sendMitsubishiAC(mitsub_code);
  EXPECT_EQ(
      "m3400s1750"
      "m450s1300m450s1300m450s420m450s420m450s420m450s1300m450s420m450s420"
      "m450s1300m450s1300m450s420m450s1300m450s420m450s420m450s1300m450s1300"
      "m450s420m450s1300m450s1300m450s420m450s420m450s1300m450s420m450s420"
      "m450s1300m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s1300m450s420m450s420"
      "m450s420m450s420m450s420m450s1300m450s420m450s420m450s420m450s420"
      "m450s420m450s1300m450s1300m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s1300m450s1300m450s420m450s420"
      "m450s1300m450s420m450s1300m450s420m450s420m450s420m450s1300m450s420"
      "m450s1300m450s1300m450s1300m450s420m450s420m450s1300m450s1300m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s1300m450s1300m450s1300m450s1300m450s1300m450s420m450s420m450s420"
      "m440s17100"
      "m3400s1750"
      "m450s1300m450s1300m450s420m450s420m450s420m450s1300m450s420m450s420"
      "m450s1300m450s1300m450s420m450s1300m450s420m450s420m450s1300m450s1300"
      "m450s420m450s1300m450s1300m450s420m450s420m450s1300m450s420m450s420"
      "m450s1300m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s1300m450s420m450s420"
      "m450s420m450s420m450s420m450s1300m450s420m450s420m450s420m450s420"
      "m450s420m450s1300m450s1300m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s1300m450s1300m450s420m450s420"
      "m450s1300m450s420m450s1300m450s420m450s420m450s420m450s1300m450s420"
      "m450s1300m450s1300m450s1300m450s420m450s420m450s1300m450s1300m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s1300m450s1300m450s1300m450s1300m450s1300m450s420m450s420m450s420"
      "m440s17100",
      irsend.outputStr());
}

// Test sending with repeats.
TEST(TestSendMitsubishiAC, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  uint8_t mitsub_code[kMitsubishiACStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x20, 0x08, 0x06, 0x30,
      0x45, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F};

  irsend.sendMitsubishiAC(mitsub_code, kMitsubishiACStateLength, 0);
  EXPECT_EQ(
      "m3400s1750"
      "m450s1300m450s1300m450s420m450s420m450s420m450s1300m450s420m450s420"
      "m450s1300m450s1300m450s420m450s1300m450s420m450s420m450s1300m450s1300"
      "m450s420m450s1300m450s1300m450s420m450s420m450s1300m450s420m450s420"
      "m450s1300m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s1300m450s420m450s420"
      "m450s420m450s420m450s420m450s1300m450s420m450s420m450s420m450s420"
      "m450s420m450s1300m450s1300m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s1300m450s1300m450s420m450s420"
      "m450s1300m450s420m450s1300m450s420m450s420m450s420m450s1300m450s420"
      "m450s1300m450s1300m450s1300m450s420m450s420m450s1300m450s1300m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s1300m450s1300m450s1300m450s1300m450s1300m450s420m450s420m450s420"
      "m440s17100",
      irsend.outputStr());

  irsend.reset();
  irsend.sendMitsubishiAC(mitsub_code, kMitsubishiACStateLength, 2);
  EXPECT_EQ(
      "m3400s1750"
      "m450s1300m450s1300m450s420m450s420m450s420m450s1300m450s420m450s420"
      "m450s1300m450s1300m450s420m450s1300m450s420m450s420m450s1300m450s1300"
      "m450s420m450s1300m450s1300m450s420m450s420m450s1300m450s420m450s420"
      "m450s1300m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s1300m450s420m450s420"
      "m450s420m450s420m450s420m450s1300m450s420m450s420m450s420m450s420"
      "m450s420m450s1300m450s1300m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s1300m450s1300m450s420m450s420"
      "m450s1300m450s420m450s1300m450s420m450s420m450s420m450s1300m450s420"
      "m450s1300m450s1300m450s1300m450s420m450s420m450s1300m450s1300m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s1300m450s1300m450s1300m450s1300m450s1300m450s420m450s420m450s420"
      "m440s17100"
      "m3400s1750"
      "m450s1300m450s1300m450s420m450s420m450s420m450s1300m450s420m450s420"
      "m450s1300m450s1300m450s420m450s1300m450s420m450s420m450s1300m450s1300"
      "m450s420m450s1300m450s1300m450s420m450s420m450s1300m450s420m450s420"
      "m450s1300m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s1300m450s420m450s420"
      "m450s420m450s420m450s420m450s1300m450s420m450s420m450s420m450s420"
      "m450s420m450s1300m450s1300m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s1300m450s1300m450s420m450s420"
      "m450s1300m450s420m450s1300m450s420m450s420m450s420m450s1300m450s420"
      "m450s1300m450s1300m450s1300m450s420m450s420m450s1300m450s1300m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s1300m450s1300m450s1300m450s1300m450s1300m450s420m450s420m450s420"
      "m440s17100"
      "m3400s1750"
      "m450s1300m450s1300m450s420m450s420m450s420m450s1300m450s420m450s420"
      "m450s1300m450s1300m450s420m450s1300m450s420m450s420m450s1300m450s1300"
      "m450s420m450s1300m450s1300m450s420m450s420m450s1300m450s420m450s420"
      "m450s1300m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s1300m450s420m450s420"
      "m450s420m450s420m450s420m450s1300m450s420m450s420m450s420m450s420"
      "m450s420m450s1300m450s1300m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s1300m450s1300m450s420m450s420"
      "m450s1300m450s420m450s1300m450s420m450s420m450s420m450s1300m450s420"
      "m450s1300m450s1300m450s1300m450s420m450s420m450s1300m450s1300m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s1300m450s1300m450s1300m450s1300m450s1300m450s420m450s420m450s420"
      "m440s17100",
      irsend.outputStr());
}

// Test sending atypical sizes.
TEST(TestSendMitsubishiAC, SendUnexpectedSizes) {
  IRsendTest irsend(4);
  irsend.begin();

  uint8_t mitsub_short_code[17] = {0x23, 0xCB, 0x26, 0x01, 0x00, 0x20,
                                   0x08, 0x06, 0x30, 0x45, 0x67, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t mitsub_long_code[19] = {0x23, 0xCB, 0x26, 0x01, 0x00, 0x20, 0x08,
                                  0x06, 0x30, 0x45, 0x67, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x1F, 0x00};
  irsend.reset();
  irsend.sendMitsubishiAC(mitsub_short_code, 17);
  ASSERT_EQ("", irsend.outputStr());

  irsend.reset();
  irsend.sendMitsubishiAC(mitsub_long_code, 19);
  ASSERT_EQ(
      "m3400s1750"
      "m450s1300m450s1300m450s420m450s420m450s420m450s1300m450s420m450s420"
      "m450s1300m450s1300m450s420m450s1300m450s420m450s420m450s1300m450s1300"
      "m450s420m450s1300m450s1300m450s420m450s420m450s1300m450s420m450s420"
      "m450s1300m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s1300m450s420m450s420"
      "m450s420m450s420m450s420m450s1300m450s420m450s420m450s420m450s420"
      "m450s420m450s1300m450s1300m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s1300m450s1300m450s420m450s420"
      "m450s1300m450s420m450s1300m450s420m450s420m450s420m450s1300m450s420"
      "m450s1300m450s1300m450s1300m450s420m450s420m450s1300m450s1300m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s1300m450s1300m450s1300m450s1300m450s1300m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m440s17100"
      "m3400s1750"
      "m450s1300m450s1300m450s420m450s420m450s420m450s1300m450s420m450s420"
      "m450s1300m450s1300m450s420m450s1300m450s420m450s420m450s1300m450s1300"
      "m450s420m450s1300m450s1300m450s420m450s420m450s1300m450s420m450s420"
      "m450s1300m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s1300m450s420m450s420"
      "m450s420m450s420m450s420m450s1300m450s420m450s420m450s420m450s420"
      "m450s420m450s1300m450s1300m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s1300m450s1300m450s420m450s420"
      "m450s1300m450s420m450s1300m450s420m450s420m450s420m450s1300m450s420"
      "m450s1300m450s1300m450s1300m450s420m450s420m450s1300m450s1300m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s1300m450s1300m450s1300m450s1300m450s1300m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m440s17100",
      irsend.outputStr());
}

// Tests for IRMitsubishiAC class.

TEST(TestMitsubishiACClass, Power) {
  IRMitsubishiAC mitsub(0);
  mitsub.begin();

  mitsub.on();
  EXPECT_TRUE(mitsub.getPower());

  mitsub.off();
  EXPECT_FALSE(mitsub.getPower());

  mitsub.setPower(true);
  EXPECT_TRUE(mitsub.getPower());

  mitsub.setPower(false);
  EXPECT_FALSE(mitsub.getPower());
}

TEST(TestMitsubishiACClass, Temperature) {
  IRMitsubishiAC mitsub(0);
  mitsub.begin();

  mitsub.setTemp(0);
  EXPECT_EQ(kMitsubishiAcMinTemp, mitsub.getTemp());

  mitsub.setTemp(255);
  EXPECT_EQ(kMitsubishiAcMaxTemp, mitsub.getTemp());

  mitsub.setTemp(kMitsubishiAcMinTemp);
  EXPECT_EQ(kMitsubishiAcMinTemp, mitsub.getTemp());

  mitsub.setTemp(kMitsubishiAcMaxTemp);
  EXPECT_EQ(kMitsubishiAcMaxTemp, mitsub.getTemp());

  mitsub.setTemp(kMitsubishiAcMinTemp - 1);
  EXPECT_EQ(kMitsubishiAcMinTemp, mitsub.getTemp());

  mitsub.setTemp(kMitsubishiAcMaxTemp + 1);
  EXPECT_EQ(kMitsubishiAcMaxTemp, mitsub.getTemp());

  mitsub.setTemp(17);
  EXPECT_EQ(17, mitsub.getTemp());

  mitsub.setTemp(21);
  EXPECT_EQ(21, mitsub.getTemp());

  mitsub.setTemp(25);
  EXPECT_EQ(25, mitsub.getTemp());

  mitsub.setTemp(30);
  EXPECT_EQ(30, mitsub.getTemp());
}

TEST(TestMitsubishiACClass, OperatingMode) {
  IRMitsubishiAC mitsub(0);
  mitsub.begin();

  mitsub.setMode(kMitsubishiAcAuto);
  EXPECT_EQ(kMitsubishiAcAuto, mitsub.getMode());

  mitsub.setMode(kMitsubishiAcCool);
  EXPECT_EQ(kMitsubishiAcCool, mitsub.getMode());

  mitsub.setMode(kMitsubishiAcHeat);
  EXPECT_EQ(kMitsubishiAcHeat, mitsub.getMode());

  mitsub.setMode(kMitsubishiAcDry);
  EXPECT_EQ(kMitsubishiAcDry, mitsub.getMode());

  mitsub.setMode(kMitsubishiAcAuto + 1);
  EXPECT_EQ(kMitsubishiAcAuto, mitsub.getMode());

  mitsub.setMode(255);
  EXPECT_EQ(kMitsubishiAcAuto, mitsub.getMode());
}

TEST(TestMitsubishiACClass, VaneMode) {
  IRMitsubishiAC mitsub(0);
  mitsub.begin();

  mitsub.setVane(kMitsubishiAcVaneAuto);
  EXPECT_EQ(kMitsubishiAcVaneAuto, mitsub.getVane());

  mitsub.setVane(kMitsubishiAcVaneAuto + 1);
  EXPECT_EQ(kMitsubishiAcVaneAuto + 1, mitsub.getVane());

  mitsub.setVane(kMitsubishiAcVaneAutoMove);
  EXPECT_EQ(kMitsubishiAcVaneAutoMove, mitsub.getVane());

  mitsub.setVane(kMitsubishiAcVaneAutoMove + 1);
  EXPECT_EQ(kMitsubishiAcVaneAutoMove, mitsub.getVane());

  mitsub.setVane(kMitsubishiAcVaneAutoMove - 1);
  EXPECT_EQ(kMitsubishiAcVaneAutoMove - 1, mitsub.getVane());
}

TEST(TestMitsubishiACClass, FanSpeed) {
  IRMitsubishiAC mitsub(0);
  mitsub.begin();

  mitsub.setFan(kMitsubishiAcFanAuto);
  EXPECT_EQ(kMitsubishiAcFanAuto, mitsub.getFan());

  mitsub.setFan(255);
  EXPECT_EQ(kMitsubishiAcFanRealMax, mitsub.getFan());

  mitsub.setFan(kMitsubishiAcFanMax);
  EXPECT_EQ(kMitsubishiAcFanRealMax, mitsub.getFan());

  mitsub.setFan(kMitsubishiAcFanMax - 1);
  EXPECT_EQ(kMitsubishiAcFanMax - 1, mitsub.getFan());

  mitsub.setFan(1);
  EXPECT_EQ(1, mitsub.getFan());

  mitsub.setFan(2);
  EXPECT_EQ(2, mitsub.getFan());

  mitsub.setFan(3);
  EXPECT_EQ(3, mitsub.getFan());

  mitsub.setFan(4);
  EXPECT_EQ(4, mitsub.getFan());

  mitsub.setFan(kMitsubishiAcFanSilent);
  EXPECT_EQ(kMitsubishiAcFanSilent, mitsub.getFan());

  mitsub.setFan(kMitsubishiAcFanSilent + 1);
  EXPECT_EQ(kMitsubishiAcFanRealMax, mitsub.getFan());
}

TEST(TestMitsubishiACClass, MessageConstuction) {
  IRMitsubishiAC mitsub(0);
  IRsendTest irsend(4);
  mitsub.begin();
  irsend.begin();

  mitsub.setFan(1);
  mitsub.setMode(kMitsubishiAcCool);
  mitsub.setTemp(27);
  mitsub.setVane(3);
  mitsub.on();

  // Check everything for kicks.
  EXPECT_EQ(1, mitsub.getFan());
  EXPECT_EQ(kMitsubishiAcCool, mitsub.getMode());
  EXPECT_EQ(27, mitsub.getTemp());
  EXPECT_EQ(3, mitsub.getVane());
  EXPECT_TRUE(mitsub.getPower());

  irsend.reset();
  irsend.sendMitsubishiAC(mitsub.getRaw());
  EXPECT_EQ(
      "m3400s1750"
      "m450s1300m450s1300m450s420m450s420m450s420m450s1300m450s420m450s420"
      "m450s1300m450s1300m450s420m450s1300m450s420m450s420m450s1300m450s1300"
      "m450s420m450s1300m450s1300m450s420m450s420m450s1300m450s420m450s420"
      "m450s1300m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s1300m450s420m450s420"
      "m450s420m450s420m450s420m450s1300m450s1300m450s420m450s420m450s420"
      "m450s1300m450s1300m450s420m450s1300m450s420m450s420m450s420m450s420"
      "m450s420m450s1300m450s1300m450s420m450s1300m450s1300m450s420m450s420"
      "m450s1300m450s420m450s420m450s1300m450s1300m450s420m450s1300m450s420"
      "m450s1300m450s1300m450s1300m450s420m450s420m450s1300m450s1300m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s1300m450s1300m450s1300m450s420m450s420m450s1300m450s420"
      "m440s17100"
      "m3400s1750"
      "m450s1300m450s1300m450s420m450s420m450s420m450s1300m450s420m450s420"
      "m450s1300m450s1300m450s420m450s1300m450s420m450s420m450s1300m450s1300"
      "m450s420m450s1300m450s1300m450s420m450s420m450s1300m450s420m450s420"
      "m450s1300m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s1300m450s420m450s420"
      "m450s420m450s420m450s420m450s1300m450s1300m450s420m450s420m450s420"
      "m450s1300m450s1300m450s420m450s1300m450s420m450s420m450s420m450s420"
      "m450s420m450s1300m450s1300m450s420m450s1300m450s1300m450s420m450s420"
      "m450s1300m450s420m450s420m450s1300m450s1300m450s420m450s1300m450s420"
      "m450s1300m450s1300m450s1300m450s420m450s420m450s1300m450s1300m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s1300m450s1300m450s1300m450s420m450s420m450s1300m450s420"
      "m440s17100",
      irsend.outputStr());
}

// Tests for decodeMitsubishiAC() with real captured example.
TEST(TestDecodeMitsubishiAC, DecodeRealExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Mitsubishi AC "Power On, 16C, low fan, vane auto move".
  uint16_t rawData[583] = {
      3476, 1708, 416, 1264,  420,  1260, 414, 400,  448, 390,  446, 392,
      444,  1236, 440, 400,   446,  392,  446, 1234, 440, 1266, 418, 396,
      442,  1264, 420, 394,   444,  394,  442, 1264, 422, 1260, 414, 398,
      440,  1266, 418, 1264,  420,  392,  446, 392,  444, 1264, 422, 392,
      446,  392,  446, 1260,  414,  400,  448, 390,  446, 392,  444, 394,
      442,  396,  442, 398,   440,  424,  412, 400,  448, 390,  446, 392,
      446,  392,  444, 394,   442,  396,  442, 396,  440, 398,  438, 400,
      448,  390,  446, 392,   446,  392,  444, 396,  442, 396,  440, 398,
      440,  400,  438, 400,   448,  390,  446, 392,  444, 1236, 440, 1266,
      418,  394,  442, 396,   440,  398,  438, 402,  446, 1232, 444, 396,
      440,  1268, 418, 394,   442,  396,  440, 398,  440, 400,  448, 390,
      448,  1232, 442, 1266,  420,  394,  444, 1264, 422, 1260, 416, 396,
      440,  398,  450, 1230,  444,  396,  442, 398,  440, 1266, 418, 1264,
      422,  1258, 416, 1266,  418,  394,  442, 396,  440, 398,  440, 398,
      438,  400,  446, 392,   446,  392,  446, 392,  444, 396,  442, 396,
      440,  398,  438, 398,   438,  400,  448, 392,  446, 392,  444, 394,
      444,  396,  442, 396,   440,  398,  438, 400,  448, 390,  448, 392,
      444,  394,  444, 394,   442,  396,  442, 396,  440, 398,  438, 400,
      448,  390,  446, 392,   446,  392,  444, 394,  442, 396,  442, 396,
      440,  398,  438, 400,   448,  390,  446, 392,  444, 394,  444, 394,
      442,  396,  440, 398,   440,  398,  438, 400,  448, 390,  446, 392,
      444,  394,  444, 394,   442,  396,  440, 398,  438, 400,  438, 400,
      448,  392,  446, 392,   444,  394,  442, 396,  442, 396,  440, 398,
      438,  1240, 444, 1264,  422,  390,  446, 392,  446, 1260, 414, 1268,
      418,  1264, 422, 12984, 3478, 1708, 418, 1264, 422, 1234, 442, 398,
      448,  390,  446, 392,   446,  1234, 440, 400,  448, 390,  446, 1234,
      442,  1266, 420, 392,   444,  1264, 420, 392,  446, 394,  444, 1236,
      448,  1260, 416, 398,   440,  1266, 418, 1262, 422, 390,  446, 392,
      444,  1234, 440, 400,   448,  392,  446, 1234, 440, 398,  450, 390,
      446,  392,  444, 394,   444,  394,  442, 396,  442, 398,  440, 400,
      438,  400,  448, 390,   446,  392,  444, 394,  442, 396,  442, 396,
      440,  398,  438, 400,   448,  390,  446, 392,  446, 392,  444, 394,
      442,  396,  442, 396,   440,  398,  438, 400,  448, 416,  420, 392,
      444,  1234, 440, 1240,  446,  394,  442, 396,  440, 398,  438, 400,
      448,  1232, 444, 396,   440,  1240, 446, 394,  442, 396,  440, 398,
      440,  400,  448, 390,   446,  1232, 444, 1238, 446, 394,  444, 1236,
      448,  1232, 442, 396,   440,  398,  448, 1232, 444, 396,  440, 398,
      438,  1242, 444, 1238,  448,  1234, 442, 1240, 444, 394,  442, 396,
      440,  398,  438, 400,   448,  390,  446, 394,  444, 420,  416, 394,
      444,  396,  440, 398,   440,  398,  438, 400,  448, 418,  420, 418,
      418,  394,  442, 396,   442,  396,  440, 424,  412, 400,  448, 390,
      446,  392,  446, 420,   418,  420,  416, 396,  440, 398,  440, 424,
      412,  426,  420, 418,   420,  392,  444, 394,  444, 422,  416, 422,
      414,  398,  440, 426,   422,  388,  448, 416,  420, 418,  418, 422,
      416,  422,  414, 424,   414,  398,  438, 426,  422, 418,  420, 390,
      446,  418,  418, 420,   416,  396,  440, 424,  412, 426,  412, 400,
      446,  418,  420, 420,   418,  420,  416, 422,  414, 422,  414, 424,
      412,  426,  422, 390,   446,  1232, 442, 1240, 446, 394,  444, 394,
      442,  1238, 446, 1234,  440,  1240, 444};  // UNKNOWN F6FDB82B

  irsend.sendRaw(rawData, 583, 33);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MITSUBISHI_AC, irsend.capture.decode_type);
  EXPECT_EQ(kMitsubishiACBits, irsend.capture.bits);
  uint8_t expected[kMitsubishiACStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x00, 0x18, 0x0A, 0x36,
      0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE6};
  EXPECT_STATE_EQ(expected, irsend.capture.state, kMitsubishiACBits);
}

// Tests for decodeMitsubishiAC() when the first payload has an error.
TEST(TestDecodeMitsubishiAC, DecodeRealExampleRepeatNeeded) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Mitsubishi AC "Power On, 16C, low fan, vane auto move".
  uint16_t rawData[583] = {
      3476, 1708, 416, 1264, 420, 1260, 414, 400, 448, 390, 446, 392, 444, 1236,
      440, 400, 446, 392, 446, 1234, 440, 1266, 418, 396, 442, 1264, 420, 394,
      444, 394, 442, 1264, 422, 1260, 414, 398, 440, 1266, 418, 1264, 420, 392,
      446, 392, 444, 1264, 422, 392, 446, 392, 446, 1260, 414, 400, 448, 390,
      446, 392, 444, 394, 442, 396, 442, 398, 440, 424, 412, 400, 448, 390, 446,
      392, 446, 392, 444, 394, 442, 396, 442, 396, 440, 398, 438, 400, 448, 390,
      446, 392, 446, 392, 444, 396, 442, 396, 440, 398, 440, 400, 438, 400, 448,
      390, 446, 392, 444, 1236, 440, 1266, 418, 394, 442, 396, 440, 398, 438,
      402, 446, 1232, 444, 396, 440, 1268, 418, 394, 442, 396, 440, 398,
      // space 699 is not recognizable:
      440, 400, 448, 390, 448, 1232, 442, 1266, 420, 394, 444, 1264, 699, 1260,
      416, 396, 440, 398, 450, 1230, 444, 396, 442, 398, 440, 1266, 418, 1264,
      422, 1258, 416, 1266, 418, 394, 442, 396, 440, 398, 440, 398, 438, 400,
      446, 392, 446, 392, 446, 392, 444, 396, 442, 396, 440, 398, 438, 398, 438,
      400, 448, 392, 446, 392, 444, 394, 444, 396, 442, 396, 440, 398, 438, 400,
      448, 390, 448, 392, 444, 394, 444, 394, 442, 396, 442, 396, 440, 398, 438,
      400, 448, 390, 446, 392, 446, 392, 444, 394, 442, 396, 442, 396, 440, 398,
      438, 400, 448, 390, 446, 392, 444, 394, 444, 394, 442, 396, 440, 398, 440,
      398, 438, 400, 448, 390, 446, 392, 444, 394, 444, 394, 442, 396, 440, 398,
      438, 400, 438, 400, 448, 392, 446, 392, 444, 394, 442, 396, 442, 396, 440,
      398, 438, 1240, 444, 1264, 422, 390, 446, 392, 446, 1260, 414, 1268, 418,
      1264, 422, 12984, 3478, 1708, 418, 1264, 422, 1234, 442, 398, 448, 390,
      446, 392, 446, 1234, 440, 400, 448, 390, 446, 1234, 442, 1266, 420, 392,
      444, 1264, 420, 392, 446, 394, 444, 1236, 448, 1260, 416, 398, 440, 1266,
      418, 1262, 422, 390, 446, 392, 444, 1234, 440, 400, 448, 392, 446, 1234,
      440, 398, 450, 390, 446, 392, 444, 394, 444, 394, 442, 396, 442, 398, 440,
      400, 438, 400, 448, 390, 446, 392, 444, 394, 442, 396, 442, 396, 440, 398,
      438, 400, 448, 390, 446, 392, 446, 392, 444, 394, 442, 396, 442, 396, 440,
      398, 438, 400, 448, 416, 420, 392, 444, 1234, 440, 1240, 446, 394, 442,
      396, 440, 398, 438, 400, 448, 1232, 444, 396, 440, 1240, 446, 394, 442,
      396, 440, 398, 440, 400, 448, 390, 446, 1232, 444, 1238, 446, 394, 444,
      1236, 448, 1232, 442, 396, 440, 398, 448, 1232, 444, 396, 440, 398, 438,
      1242, 444, 1238, 448, 1234, 442, 1240, 444, 394, 442, 396, 440, 398, 438,
      400, 448, 390, 446, 394, 444, 420, 416, 394, 444, 396, 440, 398, 440, 398,
      438, 400, 448, 418, 420, 418, 418, 394, 442, 396, 442, 396, 440, 424, 412,
      400, 448, 390, 446, 392, 446, 420, 418, 420, 416, 396, 440, 398, 440, 424,
      412, 426, 420, 418, 420, 392, 444, 394, 444, 422, 416, 422, 414, 398, 440,
      426, 422, 388, 448, 416, 420, 418, 418, 422, 416, 422, 414, 424, 414, 398,
      438, 426, 422, 418, 420, 390, 446, 418, 418, 420, 416, 396, 440, 424, 412,
      426, 412, 400, 446, 418, 420, 420, 418, 420, 416, 422, 414, 422, 414, 424,
      412, 426, 422, 390, 446, 1232, 442, 1240, 446, 394, 444, 394, 442, 1238,
      446, 1234, 440, 1240, 444};  // UNKNOWN F6FDB82B

  irsend.sendRaw(rawData, 583, 33);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MITSUBISHI_AC, irsend.capture.decode_type);
  EXPECT_EQ(kMitsubishiACBits, irsend.capture.bits);
  uint8_t expected[kMitsubishiACStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x00, 0x18, 0x0A, 0x36,
      0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE6};
  EXPECT_STATE_EQ(expected, irsend.capture.state, kMitsubishiACBits);
}

// Tests for decodeMitsubishiAC() when the repeat mark is wrong.
TEST(TestDecodeMitsubishiAC, DecodeRealExampleRepeatMarkError) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Mitsubishi AC "Power On, 16C, low fan, vane auto move".
  uint16_t rawData[583] = {
      3476, 1708, 416, 1264, 420, 1260, 414, 400, 448, 390, 446, 392, 444, 1236,
      440, 400, 446, 392, 446, 1234, 440, 1266, 418, 396, 442, 1264, 420, 394,
      444, 394, 442, 1264, 422, 1260, 414, 398, 440, 1266, 418, 1264, 420, 392,
      446, 392, 444, 1264, 422, 392, 446, 392, 446, 1260, 414, 400, 448, 390,
      446, 392, 444, 394, 442, 396, 442, 398, 440, 424, 412, 400, 448, 390, 446,
      392, 446, 392, 444, 394, 442, 396, 442, 396, 440, 398, 438, 400, 448, 390,
      446, 392, 446, 392, 444, 396, 442, 396, 440, 398, 440, 400, 438, 400, 448,
      390, 446, 392, 444, 1236, 440, 1266, 418, 394, 442, 396, 440, 398, 438,
      402, 446, 1232, 444, 396, 440, 1268, 418, 394, 442, 396, 440, 398, 440,
      400, 448, 390, 448, 1232, 442, 1266, 420, 394, 444, 1264, 422, 1260, 416,
      396, 440, 398, 450, 1230, 444, 396, 442, 398, 440, 1266, 418, 1264, 422,
      1258, 416, 1266, 418, 394, 442, 396, 440, 398, 440, 398, 438, 400, 446,
      392, 446, 392, 446, 392, 444, 396, 442, 396, 440, 398, 438, 398, 438, 400,
      448, 392, 446, 392, 444, 394, 444, 396, 442, 396, 440, 398, 438, 400, 448,
      390, 448, 392, 444, 394, 444, 394, 442, 396, 442, 396, 440, 398, 438, 400,
      448, 390, 446, 392, 446, 392, 444, 394, 442, 396, 442, 396, 440, 398, 438,
      400, 448, 390, 446, 392, 444, 394, 444, 394, 442, 396, 440, 398, 440, 398,
      438, 400, 448, 390, 446, 392, 444, 394, 444, 394, 442, 396, 440, 398, 438,
      400, 438, 400, 448, 392, 446, 392, 444, 394, 442, 396, 442, 396, 440, 398,
      438, 1240, 444, 1264, 422, 390, 446, 392, 446,
      // Repeat mark (1111) wrong:
      1260, 414, 1268, 418, 1264, 422, 1111, 347, 1708, 418, 1264, 422, 1234,
      442, 398, 448, 390, 446, 392, 446, 1234, 440, 400, 448, 390, 446, 1234,
      442, 1266, 420, 392, 444, 1264, 420, 392, 446, 394, 444, 1236, 448, 1260,
      416, 398, 440, 1266, 418, 1262, 422, 390, 446, 392, 444, 1234, 440, 400,
      448, 392, 446, 1234, 440, 398, 450, 390, 446, 392, 444, 394, 444, 394,
      442, 396, 442, 398, 440, 400, 438, 400, 448, 390, 446, 392, 444, 394, 442,
      396, 442, 396, 440, 398, 438, 400, 448, 390, 446, 392, 446, 392, 444, 394,
      442, 396, 442, 396, 440, 398, 438, 400, 448, 416, 420, 392, 444, 1234,
      440, 1240, 446, 394, 442, 396, 440, 398, 438, 400, 448, 1232, 444, 396,
      440, 1240, 446, 394, 442, 396, 440, 398, 440, 400, 448, 390, 446, 1232,
      444, 1238, 446, 394, 444, 1236, 448, 1232, 442, 396, 440, 398, 448, 1232,
      444, 396, 440, 398, 438, 1242, 444, 1238, 448, 1234, 442, 1240, 444, 394,
      442, 396, 440, 398, 438, 400, 448, 390, 446, 394, 444, 420, 416, 394, 444,
      396, 440, 398, 440, 398, 438, 400, 448, 418, 420, 418, 418, 394, 442, 396,
      442, 396, 440, 424, 412, 400, 448, 390, 446, 392, 446, 420, 418, 420, 416,
      396, 440, 398, 440, 424, 412, 426, 420, 418, 420, 392, 444, 394, 444, 422,
      416, 422, 414, 398, 440, 426, 422, 388, 448, 416, 420, 418, 418, 422, 416,
      422, 414, 424, 414, 398, 438, 426, 422, 418, 420, 390, 446, 418, 418, 420,
      416, 396, 440, 424, 412, 426, 412, 400, 446, 418, 420, 420, 418, 420, 416,
      422, 414, 422, 414, 424, 412, 426, 422, 390, 446, 1232, 442, 1240, 446,
      394, 444, 394, 442, 1238, 446, 1234, 440, 1240, 444};  // UNKNOWN F6FDB82B

  irsend.sendRaw(rawData, 583, 33);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MITSUBISHI_AC, irsend.capture.decode_type);
  EXPECT_EQ(kMitsubishiACBits, irsend.capture.bits);
  uint8_t expected[kMitsubishiACStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x00, 0x18, 0x0A, 0x36,
      0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE6};
  EXPECT_STATE_EQ(expected, irsend.capture.state, kMitsubishiACBits);
}

// Tests for decodeMitsubishiAC() when first payload has an error and the
//   repeat mark is wrong.
TEST(TestDecodeMitsubishiAC, DecodeRealExampleRepeatNeededButError) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Mitsubishi AC "Power On, 16C, low fan, vane auto move".
  uint16_t rawData[583] = {
      3476, 1708, 416, 1264, 420, 1260, 414, 400, 448, 390, 446, 392, 444, 1236,
      440, 400, 446, 392, 446, 1234, 440, 1266, 418, 396, 442, 1264, 420, 394,
      444, 394, 442, 1264, 422, 1260, 414, 398, 440, 1266, 418, 1264, 420, 392,
      446, 392, 444, 1264, 422, 392, 446, 392, 446, 1260, 414, 400, 448, 390,
      446, 392, 444, 394, 442, 396, 442, 398, 440, 424, 412, 400, 448, 390, 446,
      392, 446, 392, 444, 394, 442, 396, 442, 396, 440, 398, 438, 400, 448, 390,
      446, 392, 446, 392, 444, 396, 442, 396, 440, 398, 440, 400, 438, 400, 448,
      390, 446, 392, 444, 1236, 440, 1266, 418, 394, 442, 396, 440, 398, 438,
      402, 446, 1232, 444, 396, 440, 1268, 418, 394, 442, 396, 440, 398,
      // space 699 is not recognizable:
      440, 400, 448, 390, 448, 1232, 442, 1266, 420, 394, 444, 1264, 699, 1260,
      416, 396, 440, 398, 450, 1230, 444, 396, 442, 398, 440, 1266, 418, 1264,
      422, 1258, 416, 1266, 418, 394, 442, 396, 440, 398, 440, 398, 438, 400,
      446, 392, 446, 392, 446, 392, 444, 396, 442, 396, 440, 398, 438, 398, 438,
      400, 448, 392, 446, 392, 444, 394, 444, 396, 442, 396, 440, 398, 438, 400,
      448, 390, 448, 392, 444, 394, 444, 394, 442, 396, 442, 396, 440, 398, 438,
      400, 448, 390, 446, 392, 446, 392, 444, 394, 442, 396, 442, 396, 440, 398,
      438, 400, 448, 390, 446, 392, 444, 394, 444, 394, 442, 396, 440, 398, 440,
      398, 438, 400, 448, 390, 446, 392, 444, 394, 444, 394, 442, 396, 440, 398,
      438, 400, 438, 400, 448, 392, 446, 392, 444, 394, 442, 396, 442, 396, 440,
      398, 438, 1240, 444, 1264, 422, 390, 446, 392, 446,
      // 1111 is not a valid repeat mark:
      1260, 414, 1268, 418, 1264, 422, 1111, 3478, 1708, 418, 1264, 422, 1234,
      442, 398, 448, 390, 446, 392, 446, 1234, 440, 400, 448, 390, 446, 1234,
      442, 1266, 420, 392, 444, 1264, 420, 392, 446, 394, 444, 1236, 448, 1260,
      416, 398, 440, 1266, 418, 1262, 422, 390, 446, 392, 444, 1234, 440, 400,
      448, 392, 446, 1234, 440, 398, 450, 390, 446, 392, 444, 394, 444, 394,
      442, 396, 442, 398, 440, 400, 438, 400, 448, 390, 446, 392, 444, 394, 442,
      396, 442, 396, 440, 398, 438, 400, 448, 390, 446, 392, 446, 392, 444, 394,
      442, 396, 442, 396, 440, 398, 438, 400, 448, 416, 420, 392, 444, 1234,
      440, 1240, 446, 394, 442, 396, 440, 398, 438, 400, 448, 1232, 444, 396,
      440, 1240, 446, 394, 442, 396, 440, 398, 440, 400, 448, 390, 446, 1232,
      444, 1238, 446, 394, 444, 1236, 448, 1232, 442, 396, 440, 398, 448, 1232,
      444, 396, 440, 398, 438, 1242, 444, 1238, 448, 1234, 442, 1240, 444, 394,
      442, 396, 440, 398, 438, 400, 448, 390, 446, 394, 444, 420, 416, 394, 444,
      396, 440, 398, 440, 398, 438, 400, 448, 418, 420, 418, 418, 394, 442, 396,
      442, 396, 440, 424, 412, 400, 448, 390, 446, 392, 446, 420, 418, 420, 416,
      396, 440, 398, 440, 424, 412, 426, 420, 418, 420, 392, 444, 394, 444, 422,
      416, 422, 414, 398, 440, 426, 422, 388, 448, 416, 420, 418, 418, 422, 416,
      422, 414, 424, 414, 398, 438, 426, 422, 418, 420, 390, 446, 418, 418, 420,
      416, 396, 440, 424, 412, 426, 412, 400, 446, 418, 420, 420, 418, 420, 416,
      422, 414, 422, 414, 424, 412, 426, 422, 390, 446, 1232, 442, 1240, 446,
      394, 444, 394, 442, 1238, 446, 1234, 440, 1240, 444};  // UNKNOWN F6FDB82B

  irsend.sendRaw(rawData, 583, 33);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(UNKNOWN, irsend.capture.decode_type);
}

TEST(TestMitsubishiACClass, HumanReadable) {
  IRMitsubishiAC irMitsu(0);
  EXPECT_EQ(
      "Power: On (HEAT), Temp: 22C, FAN: SILENT, VANE: AUTO, "
      "Time: 17:10, On timer: 00:00, Off timer: 00:00, Timer: -",
      irMitsu.toString());
}

// Test sending typical data only.
TEST(TestSendMitsubishi2, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendMitsubishi2(0xF82);
  EXPECT_EQ(
      "m8400s4200"
      "m560s520m560s520m560s520m560s520m560s1560m560s1560m560s1560m560s1560"
      "m560s4200"
      "m560s1560m560s520m560s520m560s520m560s520m560s520m560s1560m560s520"
      "m560s28500"
      "m8400s4200"
      "m560s520m560s520m560s520m560s520m560s1560m560s1560m560s1560m560s1560"
      "m560s4200"
      "m560s1560m560s520m560s520m560s520m560s520m560s520m560s1560m560s520"
      "m560s28500",
      irsend.outputStr());

  irsend.reset();
  irsend.sendMitsubishi2(0x0);
  EXPECT_EQ(
      "m8400s4200"
      "m560s520m560s520m560s520m560s520m560s520m560s520m560s520m560s520"
      "m560s4200"
      "m560s520m560s520m560s520m560s520m560s520m560s520m560s520m560s520"
      "m560s28500"
      "m8400s4200"
      "m560s520m560s520m560s520m560s520m560s520m560s520m560s520m560s520"
      "m560s4200"
      "m560s520m560s520m560s520m560s520m560s520m560s520m560s520m560s520"
      "m560s28500",
      irsend.outputStr());
}

// Test sending odd repeats.
TEST(TestSendMitsubishi2, Repeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendMitsubishi2(0xF82, kMitsubishiBits, 0);
  EXPECT_EQ(
      "m8400s4200"
      "m560s520m560s520m560s520m560s520m560s1560m560s1560m560s1560m560s1560"
      "m560s4200"
      "m560s1560m560s520m560s520m560s520m560s520m560s520m560s1560m560s520"
      "m560s28500",
      irsend.outputStr());

  irsend.reset();
  irsend.sendMitsubishi2(0xF82, kMitsubishiBits, 2);
  EXPECT_EQ(
      "m8400s4200"
      "m560s520m560s520m560s520m560s520m560s1560m560s1560m560s1560m560s1560"
      "m560s4200"
      "m560s1560m560s520m560s520m560s520m560s520m560s520m560s1560m560s520"
      "m560s28500"
      "m8400s4200"
      "m560s520m560s520m560s520m560s520m560s1560m560s1560m560s1560m560s1560"
      "m560s4200"
      "m560s1560m560s520m560s520m560s520m560s520m560s520m560s1560m560s520"
      "m560s28500"
      "m8400s4200"
      "m560s520m560s520m560s520m560s520m560s1560m560s1560m560s1560m560s1560"
      "m560s4200"
      "m560s1560m560s520m560s520m560s520m560s520m560s520m560s1560m560s520"
      "m560s28500",
      irsend.outputStr());
}

// Tests for decodeMitsubishi2().

// Decode synthetic examples.
TEST(TestDecodeMitsubishi2, DecodeSyntheticExamples) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Mitsubishi Projector "Power On" (16-bit).
  irsend.sendMitsubishi2(0xF82);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MITSUBISHI2, irsend.capture.decode_type);
  EXPECT_EQ(kMitsubishiBits, irsend.capture.bits);
  EXPECT_EQ(0xF82, irsend.capture.value);
  EXPECT_EQ(0xF, irsend.capture.address);
  EXPECT_EQ(0x82, irsend.capture.command);

  irsend.reset();
  irsend.sendMitsubishi2(0x0);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MITSUBISHI2, irsend.capture.decode_type);
  EXPECT_EQ(kMitsubishiBits, irsend.capture.bits);
  EXPECT_EQ(0x0, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  irsend.sendMitsubishi2(0x1234);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MITSUBISHI2, irsend.capture.decode_type);
  EXPECT_EQ(kMitsubishiBits, irsend.capture.bits);
  EXPECT_EQ(0x1234, irsend.capture.value);
  EXPECT_EQ(0x12, irsend.capture.address);
  EXPECT_EQ(0x34, irsend.capture.command);
}

// Decode a 'real' example from Issue #441
TEST(TestDecodeMitsubishi2, DecodeRealExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Mitsubishi Projector "Power On" (16-bit).
  uint16_t rawData[75] = {
      8402, 4172, 554,  490, 562,   484,  560,  514,  532,  512,  534,
      1566, 526,  1572, 526, 1542,  560,  1568, 532,  4192, 534,  1564,
      532,  484,  560,  512, 532,   512,  532,  514,  530,  514,  526,
      1570, 524,  520,  526, 28506, 8454, 4170, 560,  514,  528,  516,
      526,  520,  524,  490, 556,   1572, 534,  1534, 560,  1568, 530,
      1538, 558,  4166, 560, 1538,  558,  490,  560,  512,  530,  514,
      532,  484,  558,  514, 532,   1566, 530,  486,  554};  // UNKNOWN 96A1512F

  irsend.sendRaw(rawData, 75, 33);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MITSUBISHI2, irsend.capture.decode_type);
  EXPECT_EQ(kMitsubishiBits, irsend.capture.bits);
  EXPECT_EQ(0xF82, irsend.capture.value);
  EXPECT_EQ(0xF, irsend.capture.address);
  EXPECT_EQ(0x82, irsend.capture.command);
}
