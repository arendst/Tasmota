// Copyright 2017-2019 David Conran
// Copyright 2019 kuchel77
// Copyright 2018 denxhun

#include "ir_Mitsubishi.h"
#include "IRac.h"
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
      "f33000d50"
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
      "f33000d50"
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
      "f33000d50"
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
      "f33000d50"
      "m300s2100m300s2100m300s2100m300s900m300s900m300s900m300s2100m300s900"
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s900"
      "m300s28080",
      irsend.outputStr());

  irsend.reset();
  irsend.sendMitsubishi(0xE242, kMitsubishiBits, 1);  // 1 repeat.
  EXPECT_EQ(
      "f33000d50"
      "m300s2100m300s2100m300s2100m300s900m300s900m300s900m300s2100m300s900"
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s900"
      "m300s28080"
      "m300s2100m300s2100m300s2100m300s900m300s900m300s900m300s2100m300s900"
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s900"
      "m300s28080",
      irsend.outputStr());
  irsend.sendMitsubishi(0xE242, kMitsubishiBits, 2);  // 2 repeats.
  EXPECT_EQ(
      "f33000d50"
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
      "f33000d50"
      "m300s900m300s900m300s900m300s900m300s900m300s900m300s900m300s900"
      "m300s43680"
      "m300s900m300s900m300s900m300s900m300s900m300s900m300s900m300s900"
      "m300s43680",
      irsend.outputStr());

  irsend.reset();
  irsend.sendMitsubishi(0x1234567890ABCDEF, 64);
  EXPECT_EQ(
      "f33000d50"
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
      "f38000d50"
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
      "f38000d50"
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
      "f38000d50"
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
      "f38000d50"
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
      "f38000d50"
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
      "Power: On, Mode: 1 (Heat), Temp: 22C, Fan: 6 (Quiet), "
      "Swing(V): 0 (Auto), Swing(H): 3 (UNKNOWN), "
      "Clock: 17:10, On Timer: 00:00, Off Timer: 00:00, Timer: -",
      irMitsu.toString());
}

// Test sending typical data only.
TEST(TestSendMitsubishi2, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendMitsubishi2(0xF82);
  EXPECT_EQ(
      "f33000d50"
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
      "f33000d50"
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
      "f33000d50"
      "m8400s4200"
      "m560s520m560s520m560s520m560s520m560s1560m560s1560m560s1560m560s1560"
      "m560s4200"
      "m560s1560m560s520m560s520m560s520m560s520m560s520m560s1560m560s520"
      "m560s28500",
      irsend.outputStr());

  irsend.reset();
  irsend.sendMitsubishi2(0xF82, kMitsubishiBits, 2);
  EXPECT_EQ(
      "f33000d50"
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

TEST(TestMitsubishiACClass, toCommon) {
  IRMitsubishiAC ac(0);
  ac.setPower(true);
  ac.setMode(kMitsubishiAcCool);
  ac.setTemp(20);
  ac.setFan(kMitsubishiAcFanSilent);
  ac.setVane(kMitsubishiAcVaneAuto);
  ac.setWideVane(kMitsubishiAcWideVaneAuto);
  // Now test it.
  ASSERT_EQ(decode_type_t::MITSUBISHI_AC, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMin, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kAuto, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kAuto, ac.toCommon().swingh);
  ASSERT_TRUE(ac.toCommon().quiet);
  // Unsupported.
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

// Decode a 'real' example.
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/888
TEST(TestDecodeMitsubishi136, DecodeRealExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  // Mitsubishi Electric Ducted A/C - ON, 20C, Cooling, MaxFan.
  uint16_t rawData[275] = {
      3324, 1474, 520, 1110, 492, 1110, 524, 314, 498, 318, 466, 336, 474, 1124,
      514, 322, 464, 338, 472, 1124, 516, 1112, 482, 342, 480, 1118, 488, 338,
      466, 344, 480, 1124, 480, 1124, 510, 328, 484, 1114, 480, 1132, 510, 330,
      456, 344, 464, 1134, 506, 334, 452, 346, 462, 1136, 504, 336, 450, 348,
      460, 350, 472, 338, 474, 1124, 472, 352, 472, 340, 474, 342, 446, 354,
      454, 354, 468, 344, 470, 344, 442, 356, 450, 358, 466, 346, 466, 348, 440,
      360, 448, 360, 462, 350, 464, 352, 438, 360, 434, 1162, 490, 350, 438,
      1148, 486, 350, 464, 352, 436, 362, 432, 376, 462, 352, 462, 1138, 448,
      376, 460, 1142, 462, 1150, 484, 1140, 462, 360, 446, 1152, 492, 1132, 460,
      362, 466, 348, 466, 348, 438, 360, 446, 1152, 492, 348, 436, 360, 434,
      374, 462, 350, 464, 350, 436, 362, 434, 376, 460, 352, 462, 352, 434, 364,
      432, 378, 458, 354, 460, 356, 434, 364, 430, 380, 456, 356, 458, 356, 432,
      366, 428, 382, 454, 358, 456, 358, 430, 1158, 476, 1146, 458, 1156, 478,
      1150, 454, 1154, 480, 1142, 460, 362, 434, 1164, 488, 352, 436, 1148, 488,
      1140, 462, 1144, 490, 1136, 466, 1142, 492, 346, 466, 1132, 462, 360, 466,
      348, 466, 350, 438, 1152, 482, 348, 464, 350, 438, 1144, 490, 1142, 462,
      1148, 486, 1138, 466, 354, 450, 1146, 496, 1132, 460, 1150, 494, 1130,
      464, 1146, 498, 1130, 464, 1144, 498, 1130, 462, 1144, 500, 1126, 468,
      1142, 502, 1122, 470, 1142, 502, 1130, 464, 1140, 504, 1124, 468, 1140,
      504, 1122, 472, 1142, 502, 1122, 472, 1138, 506};  // UNKNOWN 66B4490E

  irsend.sendRaw(rawData, 275, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(MITSUBISHI136, irsend.capture.decode_type);
  EXPECT_EQ(kMitsubishi136Bits, irsend.capture.bits);
  uint8_t expected[kMitsubishi136StateLength] = {
      0x23, 0xCB, 0x26, 0x21, 0x00, 0x40, 0x41, 0x37, 0x04,
      0x00, 0x00, 0xBF, 0xBE, 0xC8, 0xFB, 0xFF, 0xFF};
  EXPECT_STATE_EQ(expected, irsend.capture.state, kMitsubishi136Bits);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 20C, Fan: 3 (High), "
      "Swing(V): 3 (Highest), Quiet: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
}

// Self decode a synthetic example.
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/888
TEST(TestDecodeMitsubishi136, SyntheticExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  // Mitsubishi Electric Ducted A/C - ON, 20C, Cooling, MaxFan.
  uint8_t expected[kMitsubishi136StateLength] = {
      0x23, 0xCB, 0x26, 0x21, 0x00, 0x40, 0x41, 0x37, 0x04,
      0x00, 0x00, 0xBF, 0xBE, 0xC8, 0xFB, 0xFF, 0xFF};

  irsend.sendMitsubishi136(expected);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(MITSUBISHI136, irsend.capture.decode_type);
  EXPECT_EQ(kMitsubishi136Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, kMitsubishi136Bits);
}

// General housekeeping
TEST(TestMitsubishi, Housekeeping) {
  ASSERT_EQ("MITSUBISHI", typeToString(decode_type_t::MITSUBISHI));
  ASSERT_FALSE(hasACState(decode_type_t::MITSUBISHI));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::MITSUBISHI));

  ASSERT_EQ("MITSUBISHI2", typeToString(decode_type_t::MITSUBISHI2));
  ASSERT_FALSE(hasACState(decode_type_t::MITSUBISHI2));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::MITSUBISHI2));

  ASSERT_EQ("MITSUBISHI_AC", typeToString(decode_type_t::MITSUBISHI_AC));
  ASSERT_TRUE(hasACState(decode_type_t::MITSUBISHI_AC));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::MITSUBISHI_AC));

  ASSERT_EQ("MITSUBISHI136", typeToString(decode_type_t::MITSUBISHI136));
  ASSERT_TRUE(hasACState(decode_type_t::MITSUBISHI136));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::MITSUBISHI136));
}

// Tests for IRMitsubishi136 class.

TEST(TestMitsubishi136Class, Power) {
  IRMitsubishi136 ac(0);
  ac.begin();

  ac.on();
  EXPECT_TRUE(ac.getPower());

  ac.off();
  EXPECT_FALSE(ac.getPower());

  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());

  ac.setPower(false);
  EXPECT_FALSE(ac.getPower());
}

TEST(TestMitsubishi136Class, Temperature) {
  IRMitsubishi136 ac(0);
  ac.begin();

  ac.setTemp(0);
  EXPECT_EQ(kMitsubishi136MinTemp, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kMitsubishi136MaxTemp, ac.getTemp());

  ac.setTemp(kMitsubishi136MinTemp);
  EXPECT_EQ(kMitsubishi136MinTemp, ac.getTemp());

  ac.setTemp(kMitsubishi136MaxTemp);
  EXPECT_EQ(kMitsubishi136MaxTemp, ac.getTemp());

  ac.setTemp(kMitsubishi136MinTemp - 1);
  EXPECT_EQ(kMitsubishi136MinTemp, ac.getTemp());

  ac.setTemp(kMitsubishi136MaxTemp + 1);
  EXPECT_EQ(kMitsubishi136MaxTemp, ac.getTemp());

  ac.setTemp(19);
  EXPECT_EQ(19, ac.getTemp());

  ac.setTemp(21);
  EXPECT_EQ(21, ac.getTemp());

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());

  ac.setTemp(29);
  EXPECT_EQ(29, ac.getTemp());
}

TEST(TestMitsubishi136Class, OperatingMode) {
  IRMitsubishi136 ac(0);
  ac.begin();

  ac.setMode(kMitsubishi136Auto);
  EXPECT_EQ(kMitsubishi136Auto, ac.getMode());

  ac.setMode(kMitsubishi136Fan);
  EXPECT_EQ(kMitsubishi136Fan, ac.getMode());

  ac.setMode(kMitsubishi136Cool);
  EXPECT_EQ(kMitsubishi136Cool, ac.getMode());

  ac.setMode(kMitsubishi136Heat);
  EXPECT_EQ(kMitsubishi136Heat, ac.getMode());

  ac.setMode(kMitsubishi136Dry);
  EXPECT_EQ(kMitsubishi136Dry, ac.getMode());

  ac.setMode(kMitsubishi136Dry + 1);
  EXPECT_EQ(kMitsubishi136Auto, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kMitsubishi136Auto, ac.getMode());
}

TEST(TestMitsubishi136Class, FanSpeed) {
  IRMitsubishi136 ac(0);
  ac.begin();

  ac.setFan(kMitsubishi136FanMax);
  EXPECT_EQ(kMitsubishi136FanMax, ac.getFan());

  ac.setFan(kMitsubishi136FanMin);
  EXPECT_EQ(kMitsubishi136FanMin, ac.getFan());

  ac.setFan(255);
  EXPECT_EQ(kMitsubishi136FanMax, ac.getFan());

  ac.setFan(kMitsubishi136FanMed);
  EXPECT_EQ(kMitsubishi136FanMed, ac.getFan());

  ac.setFan(kMitsubishi136FanLow);
  EXPECT_EQ(kMitsubishi136FanLow, ac.getFan());

  ac.setFan(kMitsubishi136FanQuiet);
  EXPECT_EQ(kMitsubishi136FanQuiet, ac.getFan());
}

TEST(TestMitsubishi136Class, Quiet) {
  IRMitsubishi136 ac(0);
  ac.begin();

  ac.setQuiet(true);
  EXPECT_TRUE(ac.getQuiet());
  ac.setQuiet(false);
  EXPECT_FALSE(ac.getQuiet());
  ac.setQuiet(true);
  EXPECT_TRUE(ac.getQuiet());
}

TEST(TestMitsubishi136Class, SwingV) {
  IRMitsubishi136 ac(0);
  ac.begin();

  ac.setSwingV(kMitsubishi136SwingVAuto);
  EXPECT_EQ(kMitsubishi136SwingVAuto, ac.getSwingV());

  ac.setSwingV(kMitsubishi136SwingVAuto + 1);
  EXPECT_EQ(kMitsubishi136SwingVAuto, ac.getSwingV());

  ac.setSwingV(kMitsubishi136SwingVLowest);
  EXPECT_EQ(kMitsubishi136SwingVLowest, ac.getSwingV());

  ac.setSwingV(kMitsubishi136SwingVLow);
  EXPECT_EQ(kMitsubishi136SwingVLow, ac.getSwingV());

  ac.setSwingV(kMitsubishi136SwingVHighest);
  EXPECT_EQ(kMitsubishi136SwingVHighest, ac.getSwingV());

  ac.setSwingV(kMitsubishi136SwingVHigh);
  EXPECT_EQ(kMitsubishi136SwingVHigh, ac.getSwingV());
}

TEST(TestMitsubishi136Class, toCommon) {
  IRMitsubishi136 ac(0);
  ac.setPower(true);
  ac.setMode(kMitsubishi136Dry);
  ac.setTemp(22);
  ac.setFan(kMitsubishi136FanQuiet);
  ac.setSwingV(kMitsubishi136SwingVAuto);
  // Now test it.
  ASSERT_EQ(decode_type_t::MITSUBISHI136, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(22, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kDry, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMin, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kAuto, ac.toCommon().swingv);
  ASSERT_TRUE(ac.toCommon().quiet);
  // Unsupported.
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

TEST(TestMitsubishi136Class, toCommonMode) {
  ASSERT_EQ(stdAc::opmode_t::kCool,
            IRMitsubishi136::toCommonMode(kMitsubishi136Cool));
  ASSERT_EQ(kMitsubishi136Cool,
            IRMitsubishi136::convertMode(stdAc::opmode_t::kCool));
  ASSERT_EQ(stdAc::opmode_t::kDry,
            IRMitsubishi136::toCommonMode(kMitsubishi136Dry));
  ASSERT_EQ(kMitsubishi136Dry,
            IRMitsubishi136::convertMode(stdAc::opmode_t::kDry));
}

// https://github.com/crankyoldgit/IRremoteESP8266/issues/891#issuecomment-531484295
TEST(TestDecodeMitsubishiAC, Issue891) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  uint16_t rawData[583] = {
      3418, 1742, 386, 1342, 398, 1324, 366, 498, 342, 524, 384, 482, 366, 1354,
      340, 528, 366, 500, 396, 1328, 366, 1354, 340, 522, 340, 1384, 342, 528,
      366, 496, 340, 1382, 398, 1324, 386, 482, 338, 1386, 420, 1320, 340, 526,
      366, 500, 396, 1324, 398, 466, 384, 480, 340, 1382, 340, 530, 340, 526,
      380, 486, 366, 500, 384, 480, 424, 452, 398, 466, 380, 488, 366, 500, 396,
      470, 340, 526, 366, 496, 366, 502, 396, 468, 342, 522, 384, 482, 342, 530,
      386, 482, 340, 524, 396, 468, 366, 500, 382, 486, 366, 500, 366, 502, 396,
      468, 366, 502, 366, 1356, 340, 1380, 382, 484, 386, 482, 342, 526, 362,
      506, 340, 526, 338, 526, 340, 1388, 366, 500, 380, 486, 366, 500, 366,
      498, 380, 488, 416, 1308, 412, 1316, 368, 500, 384, 1338, 396, 1324, 382,
      488, 368, 498, 380, 488, 340, 524, 366, 502, 384, 480, 418, 452, 396, 468,
      340, 1382, 366, 498, 366, 500, 366, 496, 340, 528, 366, 506, 342, 528,
      340, 526, 340, 524, 412, 458, 340, 528, 366, 502, 340, 526, 338, 528, 396,
      466, 396, 466, 366, 496, 366, 500, 366, 502, 366, 498, 366, 500, 396, 470,
      396, 470, 386, 484, 366, 498, 382, 496, 396, 470, 368, 498, 366, 500, 396,
      474, 342, 524, 342, 524, 366, 500, 396, 470, 366, 502, 366, 498, 380, 488,
      340, 522, 412, 460, 396, 468, 396, 468, 366, 496, 340, 522, 366, 504, 396,
      466, 396, 470, 340, 526, 396, 468, 412, 470, 396, 470, 366, 502, 384, 482,
      366, 498, 418, 452, 424, 450, 366, 496, 342, 524, 340, 524, 366, 1356,
      396, 1326, 366, 496, 340, 1382, 396, 470, 366, 1356, 342, 526, 396, 1322,
      386, 17100, 3524, 1772, 366, 1358, 396, 1326, 364, 506, 366, 500, 384,
      482, 396, 1324, 340, 526, 340, 524, 342, 1380, 342, 1382, 342, 526, 338,
      1382, 386, 484, 428, 450, 364, 1356, 366, 1358, 366, 498, 412, 1312, 382,
      1346, 368, 500, 384, 482, 398, 1326, 366, 500, 396, 466, 412, 1314, 342,
      526, 380, 490, 340, 526, 384, 484, 396, 466, 366, 498, 340, 522, 342, 524,
      382, 488, 366, 494, 340, 524, 366, 496, 352, 520, 340, 522, 380, 486, 366,
      498, 340, 526, 340, 524, 382, 488, 366, 498, 396, 470, 342, 524, 340, 524,
      366, 500, 366, 498, 366, 498, 414, 1312, 366, 1354, 362, 508, 340, 524,
      340, 528, 422, 454, 422, 452, 396, 468, 384, 1340, 366, 502, 412, 460,
      426, 450, 396, 466, 382, 486, 366, 1358, 382, 1344, 414, 458, 366, 1356,
      382, 1342, 386, 482, 366, 494, 386, 482, 342, 524, 342, 524, 380, 484,
      366, 500, 384, 480, 428, 1306, 366, 502, 396, 472, 340, 526, 366, 496,
      420, 456, 380, 486, 366, 498, 366, 496, 398, 466, 340, 524, 382, 490, 366,
      494, 342, 524, 396, 466, 380, 490, 340, 524, 396, 470, 394, 478, 422, 452,
      396, 466, 362, 508, 396, 466, 396, 466, 364, 498, 340, 528, 412, 454, 342,
      522, 416, 450, 366, 498, 340, 530, 366, 498, 396, 466, 366, 500, 396, 468,
      340, 530, 366, 502, 412, 458, 396, 468, 384, 482, 366, 498, 340, 522, 380,
      488, 366, 498, 340, 528, 342, 534, 396, 472, 380, 484, 380, 486, 386, 484,
      342, 526, 396, 470, 366, 500, 396, 466, 366, 502, 412, 460, 426, 450, 396,
      468, 380, 1344, 340, 1380, 414, 460, 380, 1342, 386, 482, 366, 1354, 340,
      526, 340, 1386, 396};  // MITSUBISHI_AC

  irsend.sendRaw(rawData, 583, 33);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(MITSUBISHI_AC, irsend.capture.decode_type);
  ASSERT_EQ(kMitsubishiACBits, irsend.capture.bits);
  uint8_t expected[kMitsubishiACStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x00, 0x18, 0x08, 0x36,
      0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAB};
  EXPECT_STATE_EQ(expected, irsend.capture.state, kMitsubishiACBits);
  IRMitsubishiAC ac(0);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: Off, Mode: 3 (Cool), Temp: 24C, Fan: 0 (Auto), "
      "Swing(V): 0 (Auto), Swing(H): 3 (UNKNOWN), "
      "Clock: 00:00, On Timer: 00:00, Off Timer: 00:00, Timer: -",
      ac.toString());
}

// Tests for IRMitsubishi112 class.

TEST(TestMitsubishi112Class, Power) {
  IRMitsubishi112 ac(0);
  ac.begin();

  ac.on();
  EXPECT_TRUE(ac.getPower());

  ac.off();
  EXPECT_FALSE(ac.getPower());

  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());

  ac.setPower(false);
  EXPECT_FALSE(ac.getPower());
}

TEST(TestMitsubishi112Class, Temperature) {
  IRMitsubishi112 ac(0);
  ac.begin();

  ac.setTemp(0);
  EXPECT_EQ(kMitsubishi112MinTemp, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kMitsubishi112MaxTemp, ac.getTemp());

  ac.setTemp(kMitsubishi112MinTemp);
  EXPECT_EQ(kMitsubishi112MinTemp, ac.getTemp());

  ac.setTemp(kMitsubishi112MaxTemp);
  EXPECT_EQ(kMitsubishi112MaxTemp, ac.getTemp());

  ac.setTemp(kMitsubishi112MinTemp - 1);
  EXPECT_EQ(kMitsubishi112MinTemp, ac.getTemp());

  ac.setTemp(kMitsubishi112MaxTemp + 1);
  EXPECT_EQ(kMitsubishi112MaxTemp, ac.getTemp());

  ac.setTemp(19);
  EXPECT_EQ(19, ac.getTemp());

  ac.setTemp(21);
  EXPECT_EQ(21, ac.getTemp());

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());

  ac.setTemp(29);
  EXPECT_EQ(29, ac.getTemp());
}

TEST(TestMitsubishi112Class, OperatingMode) {
  IRMitsubishi112 ac(0);
  ac.begin();

  ac.setMode(kMitsubishi112Auto);
  EXPECT_EQ(kMitsubishi112Auto, ac.getMode());

  ac.setMode(kMitsubishi112Cool);
  EXPECT_EQ(kMitsubishi112Cool, ac.getMode());

  ac.setMode(kMitsubishi112Heat);
  EXPECT_EQ(kMitsubishi112Heat, ac.getMode());

  ac.setMode(kMitsubishi112Dry);
  EXPECT_EQ(kMitsubishi112Dry, ac.getMode());
}

TEST(TestMitsubishi112Class, FanSpeed) {
  IRMitsubishi112 ac(0);
  ac.begin();

  ac.setFan(kMitsubishi112FanMax);
  EXPECT_EQ(kMitsubishi112FanMax, ac.getFan());

  ac.setFan(kMitsubishi112FanMin);
  EXPECT_EQ(kMitsubishi112FanMin, ac.getFan());

  ac.setFan(255);
  EXPECT_EQ(kMitsubishi112FanMax, ac.getFan());

  ac.setFan(kMitsubishi112FanMed);
  EXPECT_EQ(kMitsubishi112FanMed, ac.getFan());

  ac.setFan(kMitsubishi112FanLow);
  EXPECT_EQ(kMitsubishi112FanLow, ac.getFan());

  ac.setFan(kMitsubishi112FanQuiet);
  EXPECT_EQ(kMitsubishi112FanQuiet, ac.getFan());
}


TEST(TestMitsubishi112Class, SwingV) {
  IRMitsubishi112 ac(0);
  ac.begin();

  ac.setSwingV(kMitsubishi112SwingVAuto);
  EXPECT_EQ(kMitsubishi112SwingVAuto, ac.getSwingV());

  ac.setSwingV(kMitsubishi112SwingVAuto + 1);
  EXPECT_EQ(kMitsubishi112SwingVAuto, ac.getSwingV());

  ac.setSwingV(kMitsubishi112SwingVLowest);
  EXPECT_EQ(kMitsubishi112SwingVLowest, ac.getSwingV());

  ac.setSwingV(kMitsubishi112SwingVLow);
  EXPECT_EQ(kMitsubishi112SwingVLow, ac.getSwingV());

  ac.setSwingV(kMitsubishi112SwingVHighest);
  EXPECT_EQ(kMitsubishi112SwingVHighest, ac.getSwingV());

  ac.setSwingV(kMitsubishi112SwingVHigh);
  EXPECT_EQ(kMitsubishi112SwingVHigh, ac.getSwingV());
}

TEST(TestMitsubishi112Class, toCommon) {
  IRMitsubishi112 ac(0);
  ac.setPower(true);
  ac.setMode(kMitsubishi112Dry);
  ac.setTemp(22);
  ac.setFan(kMitsubishi112FanQuiet);
  ac.setSwingV(kMitsubishi112SwingVAuto);
  ac.setSwingH(kMitsubishi112SwingHAuto);
  // Now test it.
  ASSERT_EQ(decode_type_t::MITSUBISHI112, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(22, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kDry, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMin, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kAuto, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kAuto, ac.toCommon().swingh);
  ASSERT_TRUE(ac.toCommon().quiet);
  // Unsupported.
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

TEST(TestMitsubishi112Class, toCommonMode) {
  ASSERT_EQ(stdAc::opmode_t::kCool,
            IRMitsubishi112::toCommonMode(kMitsubishi112Cool));
  ASSERT_EQ(kMitsubishi112Cool,
            IRMitsubishi112::convertMode(stdAc::opmode_t::kCool));
  ASSERT_EQ(stdAc::opmode_t::kDry,
            IRMitsubishi112::toCommonMode(kMitsubishi112Dry));
  ASSERT_EQ(kMitsubishi112Dry,
            IRMitsubishi112::convertMode(stdAc::opmode_t::kDry));
}

// Decode a 'real' example.
TEST(TestDecodeMitsubishi112, DecodeRealExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  uint16_t rawData[227] = {3468, 1694, 464, 1248, 464, 1248, 466, 376, 468,
      422, 470, 380, 488, 1246, 464, 382, 462, 422, 470, 1248, 466, 1246,
      442, 420, 472, 1246, 466, 380, 488, 382, 486, 1248, 442, 1272, 440,
      422, 472, 1246, 464, 1248, 466, 378, 490, 382, 464, 1270, 466, 380,
      464, 420, 448, 1270, 440, 422, 472, 380, 488, 380, 490, 378, 490,
      376, 466, 424, 468, 386, 486, 380, 488, 382, 462, 422, 448, 422,
      448, 422, 472, 378, 464, 422, 472, 380, 488, 380, 490, 380, 486,
      1248, 466, 380, 488, 380, 490, 1246, 440, 424, 468, 382, 488, 1246,
      466, 1246, 466, 380, 490, 380, 464, 422, 472, 380, 464, 422, 472,
      380, 464, 424, 472, 376, 466, 422, 448, 1270, 444, 420, 448, 420,
      470, 384, 486, 380, 490, 380, 486, 1248, 442, 422, 474, 1244, 464,
      1246, 466, 1246, 466, 384, 462, 422, 472, 378, 490, 382, 486, 384,
      462, 422, 470, 382, 488, 380, 464, 420, 448, 420, 448, 422, 470,
      380, 466, 420, 472, 378, 490, 380, 490, 378, 490, 378, 466, 422,
      446, 422, 446, 422, 446, 422, 472, 378, 488, 380, 490, 380, 466,
      420, 472, 380, 490, 380, 486, 384, 462, 422, 472, 378, 466, 1270,
      466, 1246, 442, 422, 470, 380, 488, 384, 486, 1246, 466, 1246, 442,
      1270, 440, 420, 472, 1248, 466, 384, 462, 1270, 440};  // MITSUBISHI112

  irsend.sendRaw(rawData, 227, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(MITSUBISHI112, irsend.capture.decode_type);
  EXPECT_EQ(kMitsubishi112Bits, irsend.capture.bits);
  uint8_t expected[kMitsubishi112StateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03, 0x08, 0x3A, 0x00,
      0x00, 0x00, 0x30, 0xAE};
  EXPECT_STATE_EQ(expected, irsend.capture.state, kMitsubishi112Bits);
  EXPECT_EQ(
      "Power: On, Mode: 3 (Cool), Temp: 23C, Fan: 2 (Quiet), "
      "Swing(V): 7 (Auto), Swing(H): 12 (Auto), Quiet: On",
      IRAcUtils::resultAcToString(&irsend.capture));
}

// Self decode a synthetic example.
TEST(TestDecodeMitsubishi112, SyntheticExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  uint8_t expected[kMitsubishi112StateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03, 0x08, 0x3A,
      0x00, 0x00, 0x00, 0x30, 0xAE};

  irsend.sendMitsubishi112(expected);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(MITSUBISHI112, irsend.capture.decode_type);
  EXPECT_EQ(kMitsubishi112Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, kMitsubishi112Bits);
}
