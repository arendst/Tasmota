// Copyright 2017 David Conran

#include "IRsend.h"
#include "IRsend_test.h"
#include "ir_Mitsubishi.h"
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
      "m300s53580"
      "m300s2100m300s2100m300s2100m300s900m300s900m300s900m300s2100m300s900"
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s900"
      "m300s53580", irsend.outputStr());

  irsend.reset();
  irsend.sendMitsubishi(0x0);
  EXPECT_EQ(
      "m300s900m300s900m300s900m300s900m300s900m300s900m300s900m300s900"
      "m300s900m300s900m300s900m300s900m300s900m300s900m300s900m300s900"
      "m300s53580"
      "m300s900m300s900m300s900m300s900m300s900m300s900m300s900m300s900"
      "m300s900m300s900m300s900m300s900m300s900m300s900m300s900m300s900"
      "m300s53580", irsend.outputStr());

  irsend.reset();
  irsend.sendMitsubishi(0x4321);
  EXPECT_EQ(
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s2100"
      "m300s900m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100"
      "m300s53580"
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s2100"
      "m300s900m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100"
      "m300s53580", irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendMitsubishi, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendMitsubishi(0xE242, MITSUBISHI_BITS, 0);  // 0 repeat.
  EXPECT_EQ(
      "m300s2100m300s2100m300s2100m300s900m300s900m300s900m300s2100m300s900"
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s900"
      "m300s53580", irsend.outputStr());

  irsend.reset();
  irsend.sendMitsubishi(0xE242, MITSUBISHI_BITS, 1);  // 1 repeat.
  EXPECT_EQ(
      "m300s2100m300s2100m300s2100m300s900m300s900m300s900m300s2100m300s900"
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s900"
      "m300s53580"
      "m300s2100m300s2100m300s2100m300s900m300s900m300s900m300s2100m300s900"
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s900"
      "m300s53580", irsend.outputStr());
  irsend.sendMitsubishi(0xE242, MITSUBISHI_BITS, 2);  // 2 repeats.
  EXPECT_EQ(
      "m300s2100m300s2100m300s2100m300s900m300s900m300s900m300s2100m300s900"
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s900"
      "m300s53580"
      "m300s2100m300s2100m300s2100m300s900m300s900m300s900m300s2100m300s900"
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s900"
      "m300s53580"
      "m300s2100m300s2100m300s2100m300s900m300s900m300s900m300s2100m300s900"
      "m300s900m300s2100m300s900m300s900m300s900m300s900m300s2100m300s900"
      "m300s53580", irsend.outputStr());
}

// Test sending an atypical data size.
TEST(TestSendMitsubishi, SendUsualSize) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendMitsubishi(0x0, 8);
  EXPECT_EQ(
      "m300s900m300s900m300s900m300s900m300s900m300s900m300s900m300s900"
      "m300s53580"
      "m300s900m300s900m300s900m300s900m300s900m300s900m300s900m300s900"
      "m300s53580", irsend.outputStr());

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
      "m300s53580"
      "m300s900m300s900m300s900m300s2100m300s900m300s900m300s2100m300s900"
      "m300s900m300s900m300s2100m300s2100m300s900m300s2100m300s900m300s900"
      "m300s900m300s2100m300s900m300s2100m300s900m300s2100m300s2100m300s900"
      "m300s900m300s2100m300s2100m300s2100m300s2100m300s900m300s900m300s900"
      "m300s2100m300s900m300s900m300s2100m300s900m300s900m300s900m300s900"
      "m300s2100m300s900m300s2100m300s900m300s2100m300s900m300s2100m300s2100"
      "m300s2100m300s2100m300s900m300s900m300s2100m300s2100m300s900m300s2100"
      "m300s2100m300s2100m300s2100m300s900m300s2100m300s2100m300s2100m300s2100"
      "m300s53580", irsend.outputStr());
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
  ASSERT_TRUE(irrecv.decodeMitsubishi(&irsend.capture, MITSUBISHI_BITS, true));
  EXPECT_EQ(MITSUBISHI, irsend.capture.decode_type);
  EXPECT_EQ(MITSUBISHI_BITS, irsend.capture.bits);
  EXPECT_EQ(0xC2B8, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  irsend.reset();
  irsend.sendMitsubishi(0x0);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeMitsubishi(&irsend.capture, MITSUBISHI_BITS, true));
  EXPECT_EQ(MITSUBISHI, irsend.capture.decode_type);
  EXPECT_EQ(MITSUBISHI_BITS, irsend.capture.bits);
  EXPECT_EQ(0x0, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  irsend.reset();
  irsend.sendMitsubishi(0xFFFF);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeMitsubishi(&irsend.capture, MITSUBISHI_BITS, true));
  EXPECT_EQ(MITSUBISHI, irsend.capture.decode_type);
  EXPECT_EQ(MITSUBISHI_BITS, irsend.capture.bits);
  EXPECT_EQ(0xFFFF, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Non-standard Mitsubishi sizes should fail with strict.
  irsend.reset();
  // 12 bits.
  irsend.sendMitsubishi(0xFFF, 12);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeMitsubishi(&irsend.capture, MITSUBISHI_BITS, true));
  ASSERT_FALSE(irrecv.decodeMitsubishi(&irsend.capture, 12, true));
  ASSERT_FALSE(irrecv.decodeMitsubishi(&irsend.capture, 64, true));

  // 32 bits.
  irsend.sendMitsubishi(0xFFF, 32);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeMitsubishi(&irsend.capture, MITSUBISHI_BITS, true));
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
  irsend.sendMitsubishi(0xC2B8, MITSUBISHI_BITS, 2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeMitsubishi(&irsend.capture, MITSUBISHI_BITS, true));
  EXPECT_EQ(MITSUBISHI, irsend.capture.decode_type);
  EXPECT_EQ(MITSUBISHI_BITS, irsend.capture.bits);
  EXPECT_EQ(0xC2B8, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal Mitsubishi 16-bit message with 0 repeats.
  irsend.reset();
  irsend.sendMitsubishi(0xC2B8, MITSUBISHI_BITS, 0);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeMitsubishi(&irsend.capture, MITSUBISHI_BITS, true));
  EXPECT_EQ(MITSUBISHI, irsend.capture.decode_type);
  EXPECT_EQ(MITSUBISHI_BITS, irsend.capture.bits);
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
  ASSERT_FALSE(irrecv.decodeMitsubishi(&irsend.capture, MITSUBISHI_BITS, true));
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
  ASSERT_FALSE(irrecv.decodeMitsubishi(&irsend.capture, MITSUBISHI_BITS, true));
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
  uint16_t gc_test[37] = {33000, 1, 1, 10, 70, 10, 70, 10, 70, 10, 30, 10, 30,
                          10, 30, 10, 70, 10, 30, 10, 30, 10, 70, 10, 30,
                          10, 30, 10, 30, 10, 30, 10, 70, 10, 30, 10, 936};
  irsend.sendGC(gc_test, 37);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodeMitsubishi(&irsend.capture));
  EXPECT_EQ(MITSUBISHI, irsend.capture.decode_type);
  EXPECT_EQ(MITSUBISHI_BITS, irsend.capture.bits);
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
  uint16_t gc_test[39] = {38000, 1, 1, 322, 162, 20, 61, 20, 61, 20, 20, 20, 20,
                          20, 20, 20, 127, 20, 61, 9, 20, 20, 61, 20, 20, 20,
                          61, 20, 61, 20, 61, 20, 20, 20, 20, 20, 20, 20, 884};
  irsend.sendGC(gc_test, 39);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeMitsubishi(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeMitsubishi(&irsend.capture, MITSUBISHI_BITS,
                                       false));
}

// Tests for Mitsubishi A/C methods.

// Test sending typical data only.
TEST(TestSendMitsubishiAC, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  uint8_t mitsub_code[MITSUBISHI_AC_STATE_LENGTH] = {
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
      "m440s17100", irsend.outputStr());
}

// Test sending with repeats.
TEST(TestSendMitsubishiAC, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  uint8_t mitsub_code[MITSUBISHI_AC_STATE_LENGTH] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x20, 0x08, 0x06, 0x30,
      0x45, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F};

  irsend.sendMitsubishiAC(mitsub_code, MITSUBISHI_AC_STATE_LENGTH, 0);
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
      "m440s17100", irsend.outputStr());

  irsend.reset();
  irsend.sendMitsubishiAC(mitsub_code, MITSUBISHI_AC_STATE_LENGTH, 2);
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
      "m440s17100", irsend.outputStr());
}

// Test sending atypical sizes.
TEST(TestSendMitsubishiAC, SendUnexpectedSizes) {
  IRsendTest irsend(4);
  irsend.begin();

  uint8_t mitsub_short_code[17] = {0x23, 0xCB, 0x26, 0x01, 0x00, 0x20,
                                   0x08, 0x06, 0x30, 0x45, 0x67, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t mitsub_long_code[19] = {0x23, 0xCB, 0x26, 0x01, 0x00, 0x20,
                                  0x08, 0x06, 0x30, 0x45, 0x67, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x1F,
                                  0x00};
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
      "m440s17100", irsend.outputStr());
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
  EXPECT_EQ(MITSUBISHI_AC_MIN_TEMP, mitsub.getTemp());

  mitsub.setTemp(255);
  EXPECT_EQ(MITSUBISHI_AC_MAX_TEMP, mitsub.getTemp());

  mitsub.setTemp(MITSUBISHI_AC_MIN_TEMP);
  EXPECT_EQ(MITSUBISHI_AC_MIN_TEMP, mitsub.getTemp());

  mitsub.setTemp(MITSUBISHI_AC_MAX_TEMP);
  EXPECT_EQ(MITSUBISHI_AC_MAX_TEMP, mitsub.getTemp());

  mitsub.setTemp(MITSUBISHI_AC_MIN_TEMP - 1);
  EXPECT_EQ(MITSUBISHI_AC_MIN_TEMP, mitsub.getTemp());

  mitsub.setTemp(MITSUBISHI_AC_MAX_TEMP + 1);
  EXPECT_EQ(MITSUBISHI_AC_MAX_TEMP, mitsub.getTemp());

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

  mitsub.setMode(MITSUBISHI_AC_AUTO);
  EXPECT_EQ(MITSUBISHI_AC_AUTO, mitsub.getMode());

  mitsub.setMode(MITSUBISHI_AC_COOL);
  EXPECT_EQ(MITSUBISHI_AC_COOL, mitsub.getMode());

  mitsub.setMode(MITSUBISHI_AC_HEAT);
  EXPECT_EQ(MITSUBISHI_AC_HEAT, mitsub.getMode());

  mitsub.setMode(MITSUBISHI_AC_DRY);
  EXPECT_EQ(MITSUBISHI_AC_DRY, mitsub.getMode());

  mitsub.setMode(MITSUBISHI_AC_AUTO + 1);
  EXPECT_EQ(MITSUBISHI_AC_AUTO, mitsub.getMode());

  mitsub.setMode(255);
  EXPECT_EQ(MITSUBISHI_AC_AUTO, mitsub.getMode());
}

TEST(TestMitsubishiACClass, VaneMode) {
  IRMitsubishiAC mitsub(0);
  mitsub.begin();

  mitsub.setVane(MITSUBISHI_AC_VANE_AUTO);
  EXPECT_EQ(MITSUBISHI_AC_VANE_AUTO, mitsub.getVane());

  mitsub.setVane(MITSUBISHI_AC_VANE_AUTO + 1);
  EXPECT_EQ(MITSUBISHI_AC_VANE_AUTO + 1, mitsub.getVane());

  mitsub.setVane(MITSUBISHI_AC_VANE_AUTO_MOVE);
  EXPECT_EQ(MITSUBISHI_AC_VANE_AUTO_MOVE, mitsub.getVane());

  mitsub.setVane(MITSUBISHI_AC_VANE_AUTO_MOVE + 1);
  EXPECT_EQ(MITSUBISHI_AC_VANE_AUTO_MOVE, mitsub.getVane());

  mitsub.setVane(MITSUBISHI_AC_VANE_AUTO_MOVE - 1);
  EXPECT_EQ(MITSUBISHI_AC_VANE_AUTO_MOVE - 1, mitsub.getVane());
}

TEST(TestMitsubishiACClass, FanSpeed) {
  IRMitsubishiAC mitsub(0);
  mitsub.begin();

  mitsub.setFan(MITSUBISHI_AC_FAN_AUTO);
  EXPECT_EQ(MITSUBISHI_AC_FAN_AUTO, mitsub.getFan());

  mitsub.setFan(255);
  EXPECT_EQ(MITSUBISHI_AC_FAN_REAL_MAX, mitsub.getFan());

  mitsub.setFan(MITSUBISHI_AC_FAN_MAX);
  EXPECT_EQ(MITSUBISHI_AC_FAN_REAL_MAX, mitsub.getFan());

  mitsub.setFan(MITSUBISHI_AC_FAN_MAX - 1);
  EXPECT_EQ(MITSUBISHI_AC_FAN_MAX - 1, mitsub.getFan());

  mitsub.setFan(1);
  EXPECT_EQ(1, mitsub.getFan());

  mitsub.setFan(2);
  EXPECT_EQ(2, mitsub.getFan());

  mitsub.setFan(3);
  EXPECT_EQ(3, mitsub.getFan());

  mitsub.setFan(4);
  EXPECT_EQ(4, mitsub.getFan());

  mitsub.setFan(MITSUBISHI_AC_FAN_SILENT);
  EXPECT_EQ(MITSUBISHI_AC_FAN_SILENT, mitsub.getFan());

  mitsub.setFan(MITSUBISHI_AC_FAN_SILENT + 1);
  EXPECT_EQ(MITSUBISHI_AC_FAN_REAL_MAX, mitsub.getFan());
}

TEST(TestMitsubishiACClass, MessageConstuction) {
  IRMitsubishiAC mitsub(0);
  IRsendTest irsend(4);
  mitsub.begin();
  irsend.begin();

  mitsub.setFan(1);
  mitsub.setMode(MITSUBISHI_AC_COOL);
  mitsub.setTemp(27);
  mitsub.setVane(3);
  mitsub.on();

  // Check everything for kicks.
  EXPECT_EQ(1, mitsub.getFan());
  EXPECT_EQ(MITSUBISHI_AC_COOL, mitsub.getMode());
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
      "m450s420m450s420m450s420m450s420m450s1300m450s1300m450s420m450s420"
      "m450s1300m450s420m450s420m450s1300m450s1300m450s420m450s1300m450s420"
      "m450s1300m450s1300m450s1300m450s420m450s420m450s1300m450s1300m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s1300m450s420m450s420m450s1300m450s420"
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
      "m450s420m450s420m450s420m450s420m450s1300m450s1300m450s420m450s420"
      "m450s1300m450s420m450s420m450s1300m450s1300m450s420m450s1300m450s420"
      "m450s1300m450s1300m450s1300m450s420m450s420m450s1300m450s1300m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s420m450s420m450s420m450s420m450s420"
      "m450s420m450s420m450s420m450s1300m450s420m450s420m450s1300m450s420"
      "m440s17100", irsend.outputStr());
}
