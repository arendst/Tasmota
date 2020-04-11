// Copyright 2017 David Conran

#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendJVC().

// Test sending typical data only.
TEST(TestSendJVC, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendJVC(0xC2B8);  // JVC VCR Power On.
  EXPECT_EQ(
      "f38000d33"
      "m8400s4200"
      "m525s1725m525s1725m525s525m525s525m525s525m525s525m525s1725m525s525"
      "m525s1725m525s525m525s1725m525s1725m525s1725m525s525m525s525m525s525"
      "m525s21675",
      irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendJVC, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendJVC(0xC2B8, kJvcBits, 1);  // 1 repeat.
  EXPECT_EQ(
      "f38000d33"
      "m8400s4200"
      "m525s1725m525s1725m525s525m525s525m525s525m525s525m525s1725m525s525"
      "m525s1725m525s525m525s1725m525s1725m525s1725m525s525m525s525m525s525"
      "m525s21675"
      "m525s1725m525s1725m525s525m525s525m525s525m525s525m525s1725m525s525"
      "m525s1725m525s525m525s1725m525s1725m525s1725m525s525m525s525m525s525"
      "m525s34275",
      irsend.outputStr());
  irsend.sendJVC(0xC2B8, kJvcBits, 2);  // 2 repeats.
  EXPECT_EQ(
      "f38000d33"
      "m8400s4200"
      "m525s1725m525s1725m525s525m525s525m525s525m525s525m525s1725m525s525"
      "m525s1725m525s525m525s1725m525s1725m525s1725m525s525m525s525m525s525"
      "m525s21675"
      "m525s1725m525s1725m525s525m525s525m525s525m525s525m525s1725m525s525"
      "m525s1725m525s525m525s1725m525s1725m525s1725m525s525m525s525m525s525"
      "m525s34275"
      "m525s1725m525s1725m525s525m525s525m525s525m525s525m525s1725m525s525"
      "m525s1725m525s525m525s1725m525s1725m525s1725m525s525m525s525m525s525"
      "m525s34275",
      irsend.outputStr());
}

// Test sending an atypical data size.
TEST(TestSendJVC, SendUnusualSize) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendJVC(0x0, 8);
  EXPECT_EQ(
      "f38000d33"
      "m8400s4200"
      "m525s525m525s525m525s525m525s525m525s525m525s525m525s525m525s525"
      "m525s38475",
      irsend.outputStr());

  irsend.reset();
  irsend.sendJVC(0x1234567890ABCDEF, 64);
  EXPECT_EQ(
      "f38000d33"
      "m8400s4200"
      "m525s525m525s525m525s525m525s1725m525s525m525s525m525s1725m525s525"
      "m525s525m525s525m525s1725m525s1725m525s525m525s1725m525s525m525s525"
      "m525s525m525s1725m525s525m525s1725m525s525m525s1725m525s1725m525s525"
      "m525s525m525s1725m525s1725m525s1725m525s1725m525s525m525s525m525s525"
      "m525s1725m525s525m525s525m525s1725m525s525m525s525m525s525m525s525"
      "m525s1725m525s525m525s1725m525s525m525s1725m525s525m525s1725m525s1725"
      "m525s1725m525s1725m525s525m525s525m525s1725m525s1725m525s525m525s1725"
      "m525s1725m525s1725m525s1725m525s525m525s1725m525s1725m525s1725m525s1725"
      "m525s10875",
      irsend.outputStr());
}

// Tests for encodeJVC().

TEST(TestEncodeJVC, NormalEncoding) {
  IRsendTest irsend(4);
  EXPECT_EQ(0x0, irsend.encodeJVC(0, 0));
  EXPECT_EQ(0x8080, irsend.encodeJVC(1, 1));
  EXPECT_EQ(0x8040, irsend.encodeJVC(1, 2));
  EXPECT_EQ(0xC2B8, irsend.encodeJVC(0x43, 0x1D));
  EXPECT_EQ(0x55AA, irsend.encodeJVC(0xAA, 0x55));
  EXPECT_EQ(0xFFFF, irsend.encodeJVC(0xFF, 0xFF));
}

// Tests for decodeJVC().

// Decode normal JVC messages.
TEST(TestDecodeJVC, NormalDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal JVC 16-bit message.
  irsend.reset();
  irsend.sendJVC(0xC2B8);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeJVC(&irsend.capture, kStartOffset, kJvcBits, true));
  EXPECT_EQ(JVC, irsend.capture.decode_type);
  EXPECT_EQ(kJvcBits, irsend.capture.bits);
  EXPECT_EQ(0xC2B8, irsend.capture.value);
  EXPECT_EQ(0x43, irsend.capture.address);
  EXPECT_EQ(0x1D, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Synthesised Normal JVC 16-bit message.
  irsend.reset();
  irsend.sendJVC(irsend.encodeJVC(0x07, 0x99));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeJVC(&irsend.capture, kStartOffset, kJvcBits, true));
  EXPECT_EQ(JVC, irsend.capture.decode_type);
  EXPECT_EQ(kJvcBits, irsend.capture.bits);
  EXPECT_EQ(0xE099, irsend.capture.value);
  EXPECT_EQ(0x07, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Synthesised Normal JVC 16-bit message.
  irsend.reset();
  irsend.sendJVC(irsend.encodeJVC(0x1, 0x1));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeJVC(&irsend.capture, kStartOffset, kJvcBits, true));
  EXPECT_EQ(JVC, irsend.capture.decode_type);
  EXPECT_EQ(kJvcBits, irsend.capture.bits);
  EXPECT_EQ(0x8080, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x1, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode normal repeated JVC messages.
TEST(TestDecodeJVC, NormalDecodeWithRepeatAndStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal JVC 16-bit message with 2 repeats.
  irsend.reset();
  irsend.sendJVC(0xC2B8, kJvcBits, 2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeJVC(&irsend.capture, kStartOffset, kJvcBits, true));
  EXPECT_EQ(JVC, irsend.capture.decode_type);
  EXPECT_EQ(kJvcBits, irsend.capture.bits);
  EXPECT_EQ(0xC2B8, irsend.capture.value);
  EXPECT_EQ(0x43, irsend.capture.address);
  EXPECT_EQ(0x1D, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  irsend.makeDecodeResult(2 * kJvcBits + 4);
  ASSERT_TRUE(irrecv.decodeJVC(&irsend.capture, kStartOffset, kJvcBits, true));
  EXPECT_EQ(JVC, irsend.capture.decode_type);
  EXPECT_EQ(kJvcBits, irsend.capture.bits);
  EXPECT_EQ(0xC2B8, irsend.capture.value);
  EXPECT_TRUE(irsend.capture.repeat);

  irsend.makeDecodeResult(2 * kJvcBits + 4 + 2 * kJvcBits + 2);
  ASSERT_TRUE(irrecv.decodeJVC(&irsend.capture, kStartOffset, kJvcBits, true));
  EXPECT_EQ(JVC, irsend.capture.decode_type);
  EXPECT_EQ(kJvcBits, irsend.capture.bits);
  EXPECT_EQ(0xC2B8, irsend.capture.value);
  EXPECT_TRUE(irsend.capture.repeat);

  // Simulate 'just' a JVC repeat command.
  // JVC VCR Power On from Global Cache, but modified to be a repeat message.
  uint16_t gc_test[37] = {38000, 1,  1,  20, 61, 20, 61, 20, 20, 20, 20, 20, 20,
                          20,    20, 20, 61, 20, 20, 20, 61, 20, 20, 20, 61, 20,
                          61,    20, 61, 20, 20, 20, 20, 20, 20, 20, 884};
  irsend.reset();
  irsend.sendGC(gc_test, 37);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodeJVC(&irsend.capture));
  EXPECT_EQ(JVC, irsend.capture.decode_type);
  EXPECT_EQ(kJvcBits, irsend.capture.bits);
  EXPECT_EQ(0xC2B8, irsend.capture.value);
  EXPECT_EQ(0x43, irsend.capture.address);
  EXPECT_EQ(0x1D, irsend.capture.command);
  EXPECT_TRUE(irsend.capture.repeat);
}

// Decode unsupported JVC messages.
TEST(TestDecodeJVC, DecodeWithNonStrictValues) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  irsend.sendJVC(0x0, 8);  // Illegal value JVC 8-bit message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeJVC(&irsend.capture, kStartOffset, kJvcBits, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeJVC(&irsend.capture, kStartOffset, 8, false));
  EXPECT_EQ(JVC, irsend.capture.decode_type);
  EXPECT_EQ(8, irsend.capture.bits);
  EXPECT_EQ(0x0, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  irsend.sendJVC(0x12345678, 32);  // Illegal value JVC 32-bit message.
  irsend.makeDecodeResult();
  // Should not pass with strict when we ask for less bits than we got.
  ASSERT_FALSE(irrecv.decodeJVC(&irsend.capture, kStartOffset, kJvcBits, true));

  irsend.makeDecodeResult();
  // Should fail with strict when we ask for the wrong bit size.
  ASSERT_FALSE(irrecv.decodeJVC(&irsend.capture, kStartOffset, 32, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeJVC(&irsend.capture, kStartOffset, 32, false));
  EXPECT_EQ(JVC, irsend.capture.decode_type);
  EXPECT_EQ(32, irsend.capture.bits);
  EXPECT_EQ(0x12345678, irsend.capture.value);
  EXPECT_EQ(0x12346A, irsend.capture.address);
  EXPECT_EQ(0x1E, irsend.capture.command);

  // Illegal over length (36-bit) message.
  irsend.reset();
  irsend.sendJVC(irsend.encodeJVC(2, 3), 36);
  irsend.makeDecodeResult();

  // Shouldn't pass if strict off and the wrong expected bits.
  ASSERT_FALSE(irrecv.decodeJVC(&irsend.capture, kStartOffset, kJvcBits,
                                false));

  // Re-decode with correct bit size.
  ASSERT_FALSE(irrecv.decodeJVC(&irsend.capture, kStartOffset, 36, true));
  ASSERT_TRUE(irrecv.decodeJVC(&irsend.capture, kStartOffset, 36, false));
  EXPECT_EQ(JVC, irsend.capture.decode_type);
  EXPECT_EQ(36, irsend.capture.bits);
  EXPECT_EQ(0x40C0, irsend.capture.value);
  EXPECT_EQ(0x2, irsend.capture.address);
  EXPECT_EQ(0x3, irsend.capture.command);
}

// Decode (non-standard) 64-bit messages.
TEST(TestDecodeJVC, Decode64BitMessages) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Illegal value & size JVC 64-bit message.
  irsend.sendJVC(0xFFFFFFFFFFFFFFFF, 64);
  irsend.makeDecodeResult();
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeJVC(&irsend.capture, kStartOffset, 64, false));
  EXPECT_EQ(JVC, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, irsend.capture.value);
  EXPECT_EQ(0xFFFFFFFF, irsend.capture.address);
  EXPECT_EQ(0xFF, irsend.capture.command);
}

// Decode a 'real' example via GlobalCache
TEST(TestDecodeJVC, DecodeGlobalCacheExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // JVC VCR Power On from Global Cache.
  uint16_t gc_test[39] = {38000, 1,  1,  322, 162, 20, 61, 20, 61, 20,
                          20,    20, 20, 20,  20,  20, 20, 20, 61, 20,
                          20,    20, 61, 20,  20,  20, 61, 20, 61, 20,
                          61,    20, 20, 20,  20,  20, 20, 20, 884};
  // 38000,1,37,320,161,21,59,21,59,21,19,21,19,21,19,21,19,21,59,21,19,21,59,21,59,21,19,21,59,21,19,21,19,21,19,21,19,21,838,21,59,21,59,21,19,21,19,21,19,21,19,21,59,21,19,21,59,21,19,21,59,21,59,21,59,21,19,21,19,21,19,21,850};
  irsend.sendGC(gc_test, 39);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodeJVC(&irsend.capture));
  EXPECT_EQ(JVC, irsend.capture.decode_type);
  EXPECT_EQ(kJvcBits, irsend.capture.bits);
  EXPECT_EQ(0xC2B8, irsend.capture.value);
  EXPECT_EQ(0x43, irsend.capture.address);
  EXPECT_EQ(0x1D, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Fail to decode a non-JVC example via GlobalCache
TEST(TestDecodeJVC, FailToDecodeNonJVCExample) {
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

  ASSERT_FALSE(irrecv.decodeJVC(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeJVC(&irsend.capture, kStartOffset, kJvcBits,
                                false));
}
