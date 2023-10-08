// Copyright 2017 David Conran

#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendWhynter().

// Test sending typical data only.
TEST(TestSendWhynter, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendWhynter(0x0);
  EXPECT_EQ(
      "f38000d50"
      "m750s750m2850s2850"
      "m750s750m750s750m750s750m750s750m750s750m750s750m750s750m750s750"
      "m750s750m750s750m750s750m750s750m750s750m750s750m750s750m750s750"
      "m750s750m750s750m750s750m750s750m750s750m750s750m750s750m750s750"
      "m750s750m750s750m750s750m750s750m750s750m750s750m750s750m750s750"
      "m750s52050",
      irsend.outputStr());

  irsend.reset();
  irsend.sendWhynter(0xFFFFFFFF);
  EXPECT_EQ(
      "f38000d50"
      "m750s750m2850s2850"
      "m750s2150m750s2150m750s2150m750s2150m750s2150m750s2150m750s2150m750s2150"
      "m750s2150m750s2150m750s2150m750s2150m750s2150m750s2150m750s2150m750s2150"
      "m750s2150m750s2150m750s2150m750s2150m750s2150m750s2150m750s2150m750s2150"
      "m750s2150m750s2150m750s2150m750s2150m750s2150m750s2150m750s2150m750s2150"
      "m750s12200",
      irsend.outputStr());

  irsend.reset();
  irsend.sendWhynter(0x87654321);
  EXPECT_EQ(
      "f38000d50"
      "m750s750m2850s2850"
      "m750s2150m750s750m750s750m750s750m750s750m750s2150m750s2150m750s2150"
      "m750s750m750s2150m750s2150m750s750m750s750m750s2150m750s750m750s2150"
      "m750s750m750s2150m750s750m750s750m750s750m750s750m750s2150m750s2150"
      "m750s750m750s750m750s2150m750s750m750s750m750s750m750s750m750s2150"
      "m750s33850",
      irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendWhynter, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendWhynter(0x87654321, kWhynterBits, 0);  // 0 repeats.
  EXPECT_EQ(
      "f38000d50"
      "m750s750m2850s2850"
      "m750s2150m750s750m750s750m750s750m750s750m750s2150m750s2150m750s2150"
      "m750s750m750s2150m750s2150m750s750m750s750m750s2150m750s750m750s2150"
      "m750s750m750s2150m750s750m750s750m750s750m750s750m750s2150m750s2150"
      "m750s750m750s750m750s2150m750s750m750s750m750s750m750s750m750s2150"
      "m750s33850",
      irsend.outputStr());

  irsend.reset();
  irsend.sendWhynter(0x87654321, kWhynterBits, 1);  // 1 repeat.
  EXPECT_EQ(
      "f38000d50"
      "m750s750m2850s2850"
      "m750s2150m750s750m750s750m750s750m750s750m750s2150m750s2150m750s2150"
      "m750s750m750s2150m750s2150m750s750m750s750m750s2150m750s750m750s2150"
      "m750s750m750s2150m750s750m750s750m750s750m750s750m750s2150m750s2150"
      "m750s750m750s750m750s2150m750s750m750s750m750s750m750s750m750s2150"
      "m750s33850"
      "m750s750m2850s2850"
      "m750s2150m750s750m750s750m750s750m750s750m750s2150m750s2150m750s2150"
      "m750s750m750s2150m750s2150m750s750m750s750m750s2150m750s750m750s2150"
      "m750s750m750s2150m750s750m750s750m750s750m750s750m750s2150m750s2150"
      "m750s750m750s750m750s2150m750s750m750s750m750s750m750s750m750s2150"
      "m750s33850",
      irsend.outputStr());

  irsend.reset();
  irsend.sendWhynter(0x87654321, kWhynterBits, 2);  // 2 repeats.
  EXPECT_EQ(
      "f38000d50"
      "m750s750m2850s2850"
      "m750s2150m750s750m750s750m750s750m750s750m750s2150m750s2150m750s2150"
      "m750s750m750s2150m750s2150m750s750m750s750m750s2150m750s750m750s2150"
      "m750s750m750s2150m750s750m750s750m750s750m750s750m750s2150m750s2150"
      "m750s750m750s750m750s2150m750s750m750s750m750s750m750s750m750s2150"
      "m750s33850"
      "m750s750m2850s2850"
      "m750s2150m750s750m750s750m750s750m750s750m750s2150m750s2150m750s2150"
      "m750s750m750s2150m750s2150m750s750m750s750m750s2150m750s750m750s2150"
      "m750s750m750s2150m750s750m750s750m750s750m750s750m750s2150m750s2150"
      "m750s750m750s750m750s2150m750s750m750s750m750s750m750s750m750s2150"
      "m750s33850"
      "m750s750m2850s2850"
      "m750s2150m750s750m750s750m750s750m750s750m750s2150m750s2150m750s2150"
      "m750s750m750s2150m750s2150m750s750m750s750m750s2150m750s750m750s2150"
      "m750s750m750s2150m750s750m750s750m750s750m750s750m750s2150m750s2150"
      "m750s750m750s750m750s2150m750s750m750s750m750s750m750s750m750s2150"
      "m750s33850",
      irsend.outputStr());
}

// Test sending an atypical data size.
TEST(TestSendWhynter, SendUnusualSize) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendWhynter(0x0, 8);
  EXPECT_EQ(
      "f38000d50"
      "m750s750m2850s2850"
      "m750s750m750s750m750s750m750s750m750s750m750s750m750s750m750s750"
      "m750s88050",
      irsend.outputStr());

  irsend.reset();
  irsend.sendWhynter(0x1234567890ABCDEF, 64);
  EXPECT_EQ(
      "f38000d50"
      "m750s750m2850s2850"
      "m750s750m750s750m750s750m750s2150m750s750m750s750m750s2150m750s750"
      "m750s750m750s750m750s2150m750s2150m750s750m750s2150m750s750m750s750"
      "m750s750m750s2150m750s750m750s2150m750s750m750s2150m750s2150m750s750"
      "m750s750m750s2150m750s2150m750s2150m750s2150m750s750m750s750m750s750"
      "m750s2150m750s750m750s750m750s2150m750s750m750s750m750s750m750s750"
      "m750s2150m750s750m750s2150m750s750m750s2150m750s750m750s2150m750s2150"
      "m750s2150m750s2150m750s750m750s750m750s2150m750s2150m750s750m750s2150"
      "m750s2150m750s2150m750s2150m750s750m750s2150m750s2150m750s2150m750s2150"
      "m750s12200",
      irsend.outputStr());
}

// Tests for decodeWhynter().

// Decode normal Whynter messages.
TEST(TestDecodeWhynter, NormalDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Whynter 32-bit message.
  irsend.reset();
  irsend.sendWhynter(0x87654321);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(WHYNTER, irsend.capture.decode_type);
  EXPECT_EQ(kWhynterBits, irsend.capture.bits);
  EXPECT_EQ(0x87654321, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode normal repeated Whynter messages.
TEST(TestDecodeWhynter, NormalDecodeWithRepeatAndStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Whynter 32-bit message with 2 repeats.
  irsend.reset();
  irsend.sendWhynter(0x87654321, kWhynterBits, 2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(WHYNTER, irsend.capture.decode_type);
  EXPECT_EQ(kWhynterBits, irsend.capture.bits);
  EXPECT_EQ(0x87654321, irsend.capture.value);
  EXPECT_FALSE(irsend.capture.repeat);

  irsend.makeDecodeResult(2 * kWhynterBits + 6);
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(WHYNTER, irsend.capture.decode_type);
  EXPECT_EQ(kWhynterBits, irsend.capture.bits);
  EXPECT_EQ(0x87654321, irsend.capture.value);

  irsend.makeDecodeResult(2 * (2 * kWhynterBits + 6));
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(WHYNTER, irsend.capture.decode_type);
  EXPECT_EQ(kWhynterBits, irsend.capture.bits);
  EXPECT_EQ(0x87654321, irsend.capture.value);
}

// Decode unsupported Whynter messages.
TEST(TestDecodeWhynter, DecodeWithNonStrictSizes) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  irsend.sendWhynter(0x12, 8);  // Illegal sized Whynter 8-bit message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeWhynter(&irsend.capture, kStartOffset, kWhynterBits,
                                    true));
  ASSERT_FALSE(irrecv.decodeWhynter(&irsend.capture, kStartOffset, 8, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeWhynter(&irsend.capture, kStartOffset, 8, false));
  EXPECT_EQ(WHYNTER, irsend.capture.decode_type);
  EXPECT_EQ(8, irsend.capture.bits);
  EXPECT_EQ(0x12, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  irsend.sendWhynter(0x1234567890, 40);  // Illegal size Whynter 40-bit message.
  irsend.makeDecodeResult();
  // Shouldn't pass with strict when we ask for less bits than we got.
  ASSERT_FALSE(irrecv.decodeWhynter(&irsend.capture, kStartOffset, kWhynterBits,
                                    true));

  irsend.makeDecodeResult();
  // Should fail with strict when we ask for the wrong bit size.
  ASSERT_FALSE(irrecv.decodeWhynter(&irsend.capture, kStartOffset, 40, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeWhynter(&irsend.capture, kStartOffset, 40, false));
  EXPECT_EQ(WHYNTER, irsend.capture.decode_type);
  EXPECT_EQ(40, irsend.capture.bits);
  EXPECT_EQ(0x1234567890, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

// Decode (non-standard) 64-bit messages.
TEST(TestDecodeWhynter, Decode64BitMessages) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Illegal size Whynter 64-bit message.
  irsend.sendWhynter(0xFFFFFFFFFFFFFFFF, 64);
  irsend.makeDecodeResult();
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeWhynter(&irsend.capture, kStartOffset, 64, false));
  EXPECT_EQ(WHYNTER, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

// Fail to decode a non-Whynter example via GlobalCache
TEST(TestDecodeWhynter, FailToDecodeNonWhynterExample) {
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

  ASSERT_FALSE(irrecv.decodeWhynter(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeWhynter(&irsend.capture, kStartOffset, kWhynterBits,
                                    false));
}
