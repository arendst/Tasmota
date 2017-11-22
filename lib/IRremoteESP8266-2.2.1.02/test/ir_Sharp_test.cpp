// Copyright 2017 David Conran

#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for encodeSharp().

TEST(TestEncodeSharp, NormalEncoding) {
  IRsendTest irsend(4);
  EXPECT_EQ(0x2, irsend.encodeSharp(0, 0));
  EXPECT_EQ(0x4202, irsend.encodeSharp(1, 1));
  EXPECT_EQ(0x4102, irsend.encodeSharp(1, 2));
  EXPECT_EQ(0x62E2, irsend.encodeSharp(0x43, 0x1D));
  EXPECT_EQ(0x2AAA, irsend.encodeSharp(0xAA, 0x55));
  EXPECT_EQ(0x7FFE, irsend.encodeSharp(0x1F, 0xFF));
  EXPECT_EQ(0x454A, irsend.encodeSharp(0x11, 0x4A));
}

TEST(TestEncodeSharp, AdvancedEncoding) {
  IRsendTest irsend(4);
  EXPECT_EQ(0x0, irsend.encodeSharp(0, 0, 0, 0));
  EXPECT_EQ(0x1, irsend.encodeSharp(0, 0, 0, 1));
  EXPECT_EQ(0x2, irsend.encodeSharp(0, 0, 1, 0));
  EXPECT_EQ(0x3, irsend.encodeSharp(0, 0, 1, 1));
  EXPECT_EQ(0x4200, irsend.encodeSharp(1, 1, 0, 0));
  EXPECT_EQ(0x4203, irsend.encodeSharp(1, 1, 1, 1));

  EXPECT_EQ(0x4200, irsend.encodeSharp(1, 1, 0, 0, false));
  EXPECT_EQ(0x4201, irsend.encodeSharp(1, 1, 0, 1, false));
  EXPECT_EQ(0x4203, irsend.encodeSharp(1, 1, 1, 1, false));

  EXPECT_EQ(0x0404, irsend.encodeSharp(1, 1, 0, 0, true));
  EXPECT_EQ(0x0405, irsend.encodeSharp(1, 1, 0, 1, true));
  EXPECT_EQ(0x0407, irsend.encodeSharp(1, 1, 1, 1, true));

  EXPECT_EQ(0x454A, irsend.encodeSharp(0x11, 0x52, 1, 0, true));
}


// Tests for sendSharp().

// Test sending typical data only.
TEST(TestSendSharp, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSharp(0x11, 0x52);
  EXPECT_EQ(
      "m260s1820m260s780m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s1820m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s43602"
      "m260s1820m260s780m260s780m260s780m260s1820m260s1820m260s780m260s1820"
      "m260s780m260s1820m260s1820m260s780m260s1820m260s780m260s1820"
      "m260s43602", irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendSharp, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSharp(0x11, 0x52, SHARP_BITS, 1);  // 1 repeat.
  EXPECT_EQ(
      "m260s1820m260s780m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s1820m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s43602"
      "m260s1820m260s780m260s780m260s780m260s1820m260s1820m260s780m260s1820"
      "m260s780m260s1820m260s1820m260s780m260s1820m260s780m260s1820"
      "m260s43602"
      "m260s1820m260s780m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s1820m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s43602"
      "m260s1820m260s780m260s780m260s780m260s1820m260s1820m260s780m260s1820"
      "m260s780m260s1820m260s1820m260s780m260s1820m260s780m260s1820"
      "m260s43602", irsend.outputStr());
}

// Test sending an atypical data size.
TEST(TestSendSharp, SendUnusualSize) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSharp(0x0, 0x0, 8);
  EXPECT_EQ(
      "m260s780m260s780m260s780m260s780m260s780m260s780m260s1820m260s780"
      "m260s43602"
      "m260s1820m260s1820m260s1820m260s1820m260s1820m260s1820m260s780m260s1820"
      "m260s43602", irsend.outputStr());

  irsend.reset();
  irsend.sendSharp(0x0, 0x0, 16);
  EXPECT_EQ(
      "m260s780m260s780m260s780m260s780m260s780m260s780m260s780m260s780"
      "m260s780m260s780m260s780m260s780m260s780m260s780m260s1820m260s780"
      "m260s43602"
      "m260s780m260s780m260s780m260s780m260s780m260s780m260s1820m260s1820"
      "m260s1820m260s1820m260s1820m260s1820m260s1820m260s1820m260s780m260s1820"
      "m260s43602", irsend.outputStr());
}

// Tests for sendSharpRaw().

// Test sending typical data only.
TEST(TestSendSharpRaw, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSharpRaw(0x454A);
  EXPECT_EQ(
      "m260s1820m260s780m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s1820m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s43602"
      "m260s1820m260s780m260s780m260s780m260s1820m260s1820m260s780m260s1820"
      "m260s780m260s1820m260s1820m260s780m260s1820m260s780m260s1820"
      "m260s43602", irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendSharpRaw, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSharpRaw(0x454A, SHARP_BITS, 1);  // 1 repeat.
  EXPECT_EQ(
      "m260s1820m260s780m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s1820m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s43602"
      "m260s1820m260s780m260s780m260s780m260s1820m260s1820m260s780m260s1820"
      "m260s780m260s1820m260s1820m260s780m260s1820m260s780m260s1820"
      "m260s43602"
      "m260s1820m260s780m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s1820m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s43602"
      "m260s1820m260s780m260s780m260s780m260s1820m260s1820m260s780m260s1820"
      "m260s780m260s1820m260s1820m260s780m260s1820m260s780m260s1820"
      "m260s43602", irsend.outputStr());
}

// Test sending an atypical data size.
TEST(TestSendSharpRaw, SendUnusualSize) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSharpRaw(0x2, 8);
  EXPECT_EQ(
      "m260s780m260s780m260s780m260s780m260s780m260s780m260s1820m260s780"
      "m260s43602"
      "m260s1820m260s1820m260s1820m260s1820m260s1820m260s1820m260s780m260s1820"
      "m260s43602", irsend.outputStr());

  irsend.reset();
  irsend.sendSharpRaw(0x2, 16);
  EXPECT_EQ(
      "m260s780m260s780m260s780m260s780m260s780m260s780m260s780m260s780"
      "m260s780m260s780m260s780m260s780m260s780m260s780m260s1820m260s780"
      "m260s43602"
      "m260s780m260s780m260s780m260s780m260s780m260s780m260s1820m260s1820"
      "m260s1820m260s1820m260s1820m260s1820m260s1820m260s1820m260s780m260s1820"
      "m260s43602", irsend.outputStr());
}

// Tests for decodeSharp().

// Decode normal Sharp messages.
TEST(TestDecodeSharp, NormalDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Sharp 15-bit message.
  irsend.reset();
  irsend.sendSharpRaw(0x454A);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSharp(&irsend.capture, SHARP_BITS, true));
  EXPECT_EQ(SHARP, irsend.capture.decode_type);
  EXPECT_EQ(SHARP_BITS, irsend.capture.bits);
  EXPECT_EQ(0x454A, irsend.capture.value);
  EXPECT_EQ(0x11, irsend.capture.address);
  EXPECT_EQ(0x4A, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Synthesised Normal Sharp 15-bit message.
  irsend.reset();
  irsend.sendSharpRaw(irsend.encodeSharp(0x07, 0x99));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSharp(&irsend.capture, SHARP_BITS, true));
  EXPECT_EQ(SHARP, irsend.capture.decode_type);
  EXPECT_EQ(SHARP_BITS, irsend.capture.bits);
  EXPECT_EQ(0x7266, irsend.capture.value);
  EXPECT_EQ(0x07, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Synthesised Normal Sharp 15-bit message.
  irsend.reset();
  irsend.sendSharpRaw(irsend.encodeSharp(0x1, 0x1));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSharp(&irsend.capture, SHARP_BITS, true));
  EXPECT_EQ(SHARP, irsend.capture.decode_type);
  EXPECT_EQ(SHARP_BITS, irsend.capture.bits);
  EXPECT_EQ(0x4202, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x1, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode normal repeated Sharp messages.
TEST(TestDecodeSharp, NormalDecodeWithRepeatAndStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Sharp 15-bit message with 1 repeat.
  irsend.reset();
  irsend.sendSharpRaw(0x7266, SHARP_BITS, 1);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSharp(&irsend.capture, SHARP_BITS, true));
  EXPECT_EQ(SHARP, irsend.capture.decode_type);
  EXPECT_EQ(SHARP_BITS, irsend.capture.bits);
  EXPECT_EQ(0x7266, irsend.capture.value);
  EXPECT_EQ(0x07, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);

  irsend.makeDecodeResult(2 * (2 * SHARP_BITS + FOOTER));
  ASSERT_TRUE(irrecv.decodeSharp(&irsend.capture, SHARP_BITS, true));
  EXPECT_EQ(SHARP, irsend.capture.decode_type);
  EXPECT_EQ(SHARP_BITS, irsend.capture.bits);
  EXPECT_EQ(0x7266, irsend.capture.value);
}

// Decode unsupported Sharp messages.
TEST(TestDecodeSharp, DecodeWithNonStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSharpRaw(0x0, 8);  // Illegal length Sharp 8-bit message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeSharp(&irsend.capture, SHARP_BITS, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeSharp(&irsend.capture, 8, false));
  EXPECT_EQ(SHARP, irsend.capture.decode_type);
  EXPECT_EQ(8, irsend.capture.bits);
  EXPECT_EQ(0x0, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  irsend.sendSharpRaw(0x12345678, 32);  // Illegal length Sharp 32-bit message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeSharp(&irsend.capture, SHARP_BITS, true));

  // Should fail with strict when we ask for the wrong bit size.
  ASSERT_FALSE(irrecv.decodeSharp(&irsend.capture, 32, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeSharp(&irsend.capture, 32, false));
  EXPECT_EQ(SHARP, irsend.capture.decode_type);
  EXPECT_EQ(32, irsend.capture.bits);
  EXPECT_EQ(0x12345678, irsend.capture.value);
  EXPECT_EQ(0x8, irsend.capture.address);
  EXPECT_EQ(0x79, irsend.capture.command);
}

// Decode (non-standard) 64-bit messages.
TEST(TestDecodeSharp, Decode64BitMessages) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Illegal value & size Sharp 64-bit message.
  irsend.sendSharpRaw(0xFFFFFFFFFFFFFFFF, 64);
  irsend.makeDecodeResult();
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeSharp(&irsend.capture, 64, false));
  EXPECT_EQ(SHARP, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, irsend.capture.value);
  EXPECT_EQ(0x1F, irsend.capture.address);
  EXPECT_EQ(0xFF, irsend.capture.command);
}

// Decode a 'real' example via GlobalCache
TEST(TestDecodeSharp, DecodeGlobalCacheExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Sharp Power On from Global Cache.
  uint16_t gc_test[67] = {38000, 1, 1,
                          10, 70, 10, 30, 10, 30, 10, 30, 10, 70, 10, 30,
                          10, 70, 10, 30, 10, 70, 10, 30, 10, 30, 10, 70,
                          10, 30, 10, 70, 10, 30,
                          10, 1657,
                          10, 70, 10, 30, 10, 30, 10, 30, 10, 70, 10, 70,
                          10, 30, 10, 70, 10, 30, 10, 70, 10, 70, 10, 30,
                          10, 70, 10, 30, 10, 70,
                          10, 1657};
  irsend.sendGC(gc_test, 67);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodeSharp(&irsend.capture));
  EXPECT_EQ(SHARP, irsend.capture.decode_type);
  EXPECT_EQ(SHARP_BITS, irsend.capture.bits);
  EXPECT_EQ(0x454A, irsend.capture.value);
  EXPECT_EQ(0x11, irsend.capture.address);
  EXPECT_EQ(0x4A, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Fail to decode a non-Sharp example via GlobalCache
TEST(TestDecodeSharp, FailToDecodeNonSharpExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Modified a few entries to unexpected values, based on previous test case.
  uint16_t gc_test[67] = {38000, 1, 1,
                          10, 70, 30, 30, 10, 30, 10, 30, 10, 70, 10, 30,
                          10, 70, 10, 30, 10, 70, 10, 30, 10, 30, 10, 70,
                          10, 30, 10, 70, 10, 30,
                          10, 1657,
                          10, 70, 10, 30, 10, 30, 10, 30, 10, 70, 10, 70,
                          10, 30, 10, 60, 10, 30, 10, 70, 10, 70, 10, 30,
                          10, 10, 70, 30, 10, 70,
                          10, 1657};
  irsend.sendGC(gc_test, 67);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeSharp(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeSharp(&irsend.capture, SHARP_BITS, false));

  // Test only half of a good message, as it is sent (sort of) twice.
  uint16_t gc_half[35] = {38000, 1, 1,
                          10, 70, 10, 30, 10, 30, 10, 30, 10, 70, 10, 30,
                          10, 70, 10, 30, 10, 70, 10, 30, 10, 30, 10, 70,
                          10, 30, 10, 70, 10, 30,
                          10, 1657};

  irsend.sendGC(gc_half, 35);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeSharp(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeSharp(&irsend.capture, SHARP_BITS, false));
}
