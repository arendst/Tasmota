// Copyright 2017, 2018 David Conran

#include "ir_Panasonic.h"
#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "IRtext.h"
#include "IRutils.h"
#include "gtest/gtest.h"

// Tests for encodePanasonic().

TEST(TestEncodePanasonic, General) {
  IRsendTest irsend(4);
  EXPECT_EQ(0x0, irsend.encodePanasonic(0, 0, 0, 0));
  EXPECT_EQ(0x101010101, irsend.encodePanasonic(1, 1, 1, 1));
  EXPECT_EQ(0xFFFF, irsend.encodePanasonic(0, 0, 0, 0xFF));
  EXPECT_EQ(0xFF00FF, irsend.encodePanasonic(0, 0, 0xFF, 0));
  EXPECT_EQ(0xFF0000FF, irsend.encodePanasonic(0, 0xFF, 0, 0));
  EXPECT_EQ(0xFFFF00000000, irsend.encodePanasonic(0xFFFF, 0, 0, 0));
  EXPECT_EQ(0xFFFFFFFFFFFF, irsend.encodePanasonic(0xFFFF, 0xFF, 0xFF, 0xFF));
  EXPECT_EQ(0x40040190ED7C, irsend.encodePanasonic(0x4004, 0x01, 0x90, 0xED));
}

// Tests for sendPanasonic64().

// Test sending typical data only.
TEST(TestSendPanasonic64, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendPanasonic64(0x0);
  EXPECT_EQ(
      "f36700d50"
      "m3456s1728"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s116208",
      irsend.outputStr());

  irsend.reset();
  irsend.sendPanasonic64(0x40040190ED7C);
  EXPECT_EQ(
      "f36700d50"
      "m3456s1728"
      "m432s432m432s1296m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s1296"
      "m432s1296m432s432m432s432m432s1296m432s432m432s432m432s432m432s432"
      "m432s1296m432s1296m432s1296m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s432m432s1296m432s1296m432s1296m432s1296m432s1296m432s432m432s432"
      "m432s102384",
      irsend.outputStr());

  irsend.reset();
  irsend.sendPanasonic64(0xFFFFFFFFFFFF);
  EXPECT_EQ(
      "f36700d50"
      "m3456s1728"
      "m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296"
      "m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296"
      "m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296"
      "m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296"
      "m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296"
      "m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296"
      "m432s74736",
      irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendPanasonic64, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendPanasonic64(0x40040190ED7C, kPanasonicBits, 0);  // 0 repeats.
  EXPECT_EQ(
      "f36700d50"
      "m3456s1728"
      "m432s432m432s1296m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s1296"
      "m432s1296m432s432m432s432m432s1296m432s432m432s432m432s432m432s432"
      "m432s1296m432s1296m432s1296m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s432m432s1296m432s1296m432s1296m432s1296m432s1296m432s432m432s432"
      "m432s102384",
      irsend.outputStr());

  irsend.reset();
  irsend.sendPanasonic64(0x40040190ED7C, kPanasonicBits, 1);  // 1 repeat.
  EXPECT_EQ(
      "f36700d50"
      "m3456s1728"
      "m432s432m432s1296m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s1296"
      "m432s1296m432s432m432s432m432s1296m432s432m432s432m432s432m432s432"
      "m432s1296m432s1296m432s1296m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s432m432s1296m432s1296m432s1296m432s1296m432s1296m432s432m432s432"
      "m432s102384"
      "m3456s1728"
      "m432s432m432s1296m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s1296"
      "m432s1296m432s432m432s432m432s1296m432s432m432s432m432s432m432s432"
      "m432s1296m432s1296m432s1296m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s432m432s1296m432s1296m432s1296m432s1296m432s1296m432s432m432s432"
      "m432s102384",
      irsend.outputStr());

  irsend.sendPanasonic64(0x40040190ED7C, kPanasonicBits, 2);  // 2 repeats.
  EXPECT_EQ(
      "f36700d50"
      "m3456s1728"
      "m432s432m432s1296m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s1296"
      "m432s1296m432s432m432s432m432s1296m432s432m432s432m432s432m432s432"
      "m432s1296m432s1296m432s1296m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s432m432s1296m432s1296m432s1296m432s1296m432s1296m432s432m432s432"
      "m432s102384"
      "m3456s1728"
      "m432s432m432s1296m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s1296"
      "m432s1296m432s432m432s432m432s1296m432s432m432s432m432s432m432s432"
      "m432s1296m432s1296m432s1296m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s432m432s1296m432s1296m432s1296m432s1296m432s1296m432s432m432s432"
      "m432s102384"
      "m3456s1728"
      "m432s432m432s1296m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s1296"
      "m432s1296m432s432m432s432m432s1296m432s432m432s432m432s432m432s432"
      "m432s1296m432s1296m432s1296m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s432m432s1296m432s1296m432s1296m432s1296m432s1296m432s432m432s432"
      "m432s102384",
      irsend.outputStr());
}

// Test sending an atypical data size.
TEST(TestSendPanasonic64, SendUnusualSize) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendPanasonic64(0x0, 8);
  EXPECT_EQ(
      "f36700d50"
      "m3456s1728"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s150768",
      irsend.outputStr());

  irsend.reset();
  irsend.sendPanasonic64(0x1234567890ABCDEF, 64);
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

// Tests for sendPanasonic().

TEST(TestSendPanasonic, CompareToSendPanasonic64) {
  IRsendTest panasonic(4);
  IRsendTest panasonic64(0);

  panasonic.begin();
  panasonic64.begin();

  panasonic.reset();
  panasonic64.reset();

  panasonic.sendPanasonic(0x4004, 0x0190ED7C);
  panasonic64.sendPanasonic64(0x40040190ED7C);
  EXPECT_EQ(panasonic64.outputStr(), panasonic.outputStr());

  panasonic.sendPanasonic(0x0, 0x0);
  panasonic64.sendPanasonic64(0x0);
  EXPECT_EQ(panasonic64.outputStr(), panasonic.outputStr());

  panasonic.sendPanasonic(0x0, 0x0, 8);
  panasonic64.sendPanasonic64(0x0, 8);
  EXPECT_EQ(panasonic64.outputStr(), panasonic.outputStr());

  panasonic.sendPanasonic(0x1234, 0x567890AB, 64);
  panasonic64.sendPanasonic64(0x1234567890AB, 64);
  EXPECT_EQ(panasonic64.outputStr(), panasonic.outputStr());

  panasonic.sendPanasonic(0x1234, 0x567890AB, kPanasonicBits, 2);
  panasonic64.sendPanasonic64(0x1234567890AB, kPanasonicBits, 2);
  EXPECT_EQ(panasonic64.outputStr(), panasonic.outputStr());
}

// Tests for decodePanasonic().

// Decode normal Panasonic messages.
TEST(TestDecodePanasonic, NormalDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Panasonic 48-bit message.
  irsend.reset();
  irsend.sendPanasonic64(0x40040190ED7C);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kStartOffset,
                                     kPanasonicBits, true));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicBits, irsend.capture.bits);
  EXPECT_EQ(0x40040190ED7C, irsend.capture.value);
  EXPECT_EQ(0x4004, irsend.capture.address);
  EXPECT_EQ(0x0190ED7C, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Synthesised Normal Panasonic 48-bit message.
  irsend.reset();
  irsend.sendPanasonic64(irsend.encodePanasonic(0x4004, 0x12, 0x34, 0x56));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kStartOffset,
                                     kPanasonicBits, true));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicBits, irsend.capture.bits);
  EXPECT_EQ(0x400412345670, irsend.capture.value);
  EXPECT_EQ(0x4004, irsend.capture.address);
  EXPECT_EQ(0x12345670, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Synthesised Normal Panasonic 48-bit message.
  irsend.reset();
  irsend.sendPanasonic64(irsend.encodePanasonic(0x4004, 0x1, 0x1, 0x1));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kStartOffset,
                                     kPanasonicBits, true));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicBits, irsend.capture.bits);
  EXPECT_EQ(0x400401010101, irsend.capture.value);
  EXPECT_EQ(0x4004, irsend.capture.address);
  EXPECT_EQ(0x1010101, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode normal repeated Panasonic messages.
TEST(TestDecodePanasonic, NormalDecodeWithRepeatAndStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Panasonic 48-bit message with 2 repeats.
  irsend.reset();
  irsend.sendPanasonic64(0x40040190ED7C, kPanasonicBits, 2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kStartOffset,
                                     kPanasonicBits, true));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicBits, irsend.capture.bits);
  EXPECT_EQ(0x40040190ED7C, irsend.capture.value);
  EXPECT_EQ(0x4004, irsend.capture.address);
  EXPECT_EQ(0x190ED7C, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  irsend.makeDecodeResult(2 * kPanasonicBits + 4);
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kStartOffset,
                                     kPanasonicBits, true));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicBits, irsend.capture.bits);
  EXPECT_EQ(0x40040190ED7C, irsend.capture.value);

  irsend.makeDecodeResult(2 * (2 * kPanasonicBits + 4));
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kStartOffset,
                                     kPanasonicBits, true));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicBits, irsend.capture.bits);
  EXPECT_EQ(0x40040190ED7C, irsend.capture.value);
}

// Decode Panasonic messages with unsupported values.
TEST(TestDecodePanasonic, DecodeWithNonStrictValues) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  irsend.sendPanasonic64(0x0);  // Illegal value Panasonic 48-bit message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, kStartOffset,
                                      kPanasonicBits, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kStartOffset,
                                     kPanasonicBits, false));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicBits, irsend.capture.bits);
  EXPECT_EQ(0x0, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  // Illegal address/Manufacturer code. The rest is legal.
  irsend.sendPanasonic64(irsend.encodePanasonic(0, 1, 2, 3));
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, kStartOffset,
                                      kPanasonicBits, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kStartOffset,
                                     kPanasonicBits, false));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicBits, irsend.capture.bits);
  EXPECT_EQ(0x1020300, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x1020300, irsend.capture.command);
}

// Decode Panasonic messages with unsupported size/lengths.
TEST(TestDecodePanasonic, DecodeWithNonStrictSize) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  irsend.sendPanasonic64(0x12345678, 32);  // Illegal size Panasonic message.
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, kStartOffset,
                                      kPanasonicBits, true));

  irsend.makeDecodeResult();
  // Should fail with strict when we ask for the wrong bit size.
  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, kStartOffset, 32, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kStartOffset, 32, false));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(32, irsend.capture.bits);
  EXPECT_EQ(0x12345678, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x12345678, irsend.capture.command);

  // Illegal over length (56-bit) message.
  irsend.reset();
  irsend.sendPanasonic64(irsend.encodePanasonic(0x4004, 1, 2, 3), 56);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, kStartOffset,
                                      kPanasonicBits, true));
  // Shouldn't pass if strict off and wrong bit size.
  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, kStartOffset,
                                      kPanasonicBits, false));
  // Re-decode with correct bit size.
  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, kStartOffset, 56, true));
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kStartOffset, 56, false));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(56, irsend.capture.bits);
  EXPECT_EQ(0x400401020300, irsend.capture.value);
  EXPECT_EQ(0x4004, irsend.capture.address);
  EXPECT_EQ(0x01020300, irsend.capture.command);
}

// Decode (non-standard) 64-bit messages.
TEST(TestDecodePanasonic, Decode64BitMessages) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Illegal value & size Panasonic 64-bit message.
  irsend.sendPanasonic64(0xFFFFFFFFFFFFFFFF, 64);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, kStartOffset, 64, true));
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kStartOffset, 64, false));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, irsend.capture.value);
  EXPECT_EQ(0xFFFFFFFF, irsend.capture.address);
  EXPECT_EQ(0xFFFFFFFF, irsend.capture.command);
}

// Decode a 'real' example via GlobalCache
TEST(TestDecodePanasonic, DecodeGlobalCacheExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Panasonic code from Global Cache.
  uint16_t gc_test[103] = {
      37000, 1,  1,  126, 64, 16, 17, 16, 49, 15, 16, 16, 16,  16, 16,
      16,    17, 15, 17,  15, 17, 15, 17, 15, 16, 16, 16, 16,  16, 16,
      17,    15, 49, 16,  16, 16, 16, 16, 17, 15, 17, 15, 17,  15, 17,
      15,    16, 16, 16,  16, 16, 16, 49, 15, 49, 16, 17, 15,  17, 15,
      49,    16, 16, 16,  17, 16, 17, 15, 17, 15, 49, 16, 49,  15, 49,
      16,    17, 16, 49,  15, 49, 16, 17, 15, 48, 16, 16, 16,  49, 15,
      48,    16, 49, 15,  49, 16, 49, 15, 17, 15, 16, 16, 2721};
  irsend.sendGC(gc_test, 103);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kStartOffset,
                                     kPanasonicBits, true));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicBits, irsend.capture.bits);
  EXPECT_EQ(0x40040190ED7C, irsend.capture.value);
  EXPECT_EQ(0x4004, irsend.capture.address);
  EXPECT_EQ(0x0190ED7C, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicBits, irsend.capture.bits);
  EXPECT_EQ(0x40040190ED7C, irsend.capture.value);
  EXPECT_EQ(0x4004, irsend.capture.address);
  EXPECT_EQ(0x0190ED7C, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Fail to decode a non-Panasonic example via GlobalCache
TEST(TestDecodePanasonic, FailToDecodeNonPanasonicExample) {
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

  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture));
  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, kStartOffset,
                                      kPanasonicBits, false));
}

// Failing to decode Panasonic in Issue #245
TEST(TestDecodePanasonic, DecodeIssue245) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();

  uint16_t rawData[100] = {
      3550, 1750, 500, 450,  500, 1300, 500, 450,  500, 450,  500, 450,
      500,  450,  500, 450,  500, 450,  500, 450,  500, 450,  500, 450,
      500,  450,  500, 450,  500, 1300, 500, 450,  500, 450,  500, 450,
      500,  450,  500, 450,  500, 450,  500, 450,  500, 450,  500, 450,
      500,  1300, 500, 450,  500, 450,  500, 450,  500, 450,  500, 450,
      500,  450,  500, 450,  500, 450,  500, 1300, 500, 450,  500, 1300,
      500,  1300, 500, 1300, 500, 1300, 500, 450,  500, 450,  500, 1300,
      500,  450,  500, 1300, 500, 1300, 500, 1300, 500, 1300, 500, 450,
      500,  1300, 500, 5000};

  irsend.sendRaw(rawData, 100, 37);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicBits, irsend.capture.bits);
  EXPECT_EQ(0x40040100BCBD, irsend.capture.value);
  EXPECT_EQ(0x4004, irsend.capture.address);
  EXPECT_EQ(0x100BCBD, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  irsend.reset();
  irsend.sendRaw(rawData, 99, 37);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicBits, irsend.capture.bits);
  EXPECT_EQ(0x40040100BCBD, irsend.capture.value);
  EXPECT_EQ(0x4004, irsend.capture.address);
  EXPECT_EQ(0x100BCBD, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Tests for sendPanasonicAC().

// Test sending typical data only.
TEST(TestSendPanasonicAC, SendDataOnly) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  irsend.reset();

  uint8_t state[kPanasonicAcStateLength] = {
      0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02,
      0x20, 0xE0, 0x04, 0x00, 0x30, 0x32, 0x80, 0xAF, 0x00,
      0x00, 0x06, 0x60, 0x00, 0x00, 0x80, 0x00, 0x06, 0x83};
  irsend.sendPanasonicAC(state);
  EXPECT_EQ(
      "f36700d50"
      "m3456s1728"
      "m432s432m432s1296m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s1296m432s1296m432s1296"
      "m432s432m432s432m432s1296m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s1296m432s1296m432s432m432s432m432s432m432s432m432s432"
      "m432s10000"
      "m3456s1728"
      "m432s432m432s1296m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s1296m432s1296m432s1296"
      "m432s432m432s432m432s1296m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s1296m432s1296m432s432m432s432"
      "m432s432m432s1296m432s432m432s432m432s1296m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s1296"
      "m432s1296m432s1296m432s1296m432s1296m432s432m432s1296m432s432m432s1296"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s1296m432s1296m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s1296m432s1296m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s1296"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s1296m432s1296m432s432m432s432m432s432m432s432m432s432"
      "m432s1296m432s1296m432s432m432s432m432s432m432s432m432s432m432s1296"
      "m432s100000",
      irsend.outputStr());
}

// Tests for the IRPanasonicAc class.

TEST(TestIRPanasonicAcClass, ChecksumCalculation) {
  IRPanasonicAc ac(kGpioUnused);

  const uint8_t originalstate[kPanasonicAcStateLength] = {
      0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02,
      0x20, 0xE0, 0x04, 0x00, 0x30, 0x32, 0x80, 0xAF, 0x00,
      0x00, 0x06, 0x60, 0x00, 0x00, 0x80, 0x00, 0x06, 0x83};
  uint8_t examplestate[kPanasonicAcStateLength] = {
      0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02,
      0x20, 0xE0, 0x04, 0x00, 0x30, 0x32, 0x80, 0xAF, 0x00,
      0x00, 0x06, 0x60, 0x00, 0x00, 0x80, 0x00, 0x06, 0x83};

  EXPECT_TRUE(IRPanasonicAc::validChecksum(examplestate));
  EXPECT_EQ(0x83, IRPanasonicAc::calcChecksum(examplestate));

  examplestate[kPanasonicAcStateLength - 1] = 0x0;  // Set incorrect checksum.
  EXPECT_FALSE(IRPanasonicAc::validChecksum(examplestate));
  EXPECT_EQ(0x83, IRPanasonicAc::calcChecksum(examplestate));
  ac.setRaw(examplestate);
  // Extracting the state from the object should have a correct checksum.
  EXPECT_TRUE(IRPanasonicAc::validChecksum(ac.getRaw()));
  EXPECT_STATE_EQ(originalstate, ac.getRaw(), kPanasonicAcBits);
  examplestate[kPanasonicAcStateLength - 1] = 0x83;  // Restore old checksum.

  // Change the state to force a different checksum.
  examplestate[6] = 0x01;  // Should increase checksum by 1.
  EXPECT_FALSE(IRPanasonicAc::validChecksum(examplestate));
  EXPECT_EQ(0x84, IRPanasonicAc::calcChecksum(examplestate));
}

TEST(TestIRPanasonicAcClass, SetAndGetPower) {
  IRPanasonicAc ac(kGpioUnused);
  ac.on();
  EXPECT_TRUE(ac.getPower());
  ac.off();
  EXPECT_FALSE(ac.getPower());
  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());
  ac.setPower(false);
  EXPECT_FALSE(ac.getPower());
}

TEST(TestIRPanasonicAcClass, SetAndGetModel) {
  IRPanasonicAc ac(kGpioUnused);
  EXPECT_EQ(kPanasonicJke, ac.getModel());
  ac.setModel(kPanasonicDke);
  EXPECT_EQ(kPanasonicDke, ac.getModel());
  ac.setModel(kPanasonicLke);
  EXPECT_EQ(kPanasonicLke, ac.getModel());
  ac.setModel(kPanasonicNke);
  EXPECT_EQ(kPanasonicNke, ac.getModel());
  ac.setModel(kPanasonicUnknown);  // shouldn't change.
  EXPECT_EQ(kPanasonicNke, ac.getModel());
  ac.setModel((panasonic_ac_remote_model_t)255);  // shouldn't change.
  EXPECT_EQ(kPanasonicNke, ac.getModel());
  ac.setModel(kPanasonicJke);
  EXPECT_EQ(kPanasonicJke, ac.getModel());

  // This state tickled a bug in getModel(). Should read as a JKE.
  uint8_t jkeState[27] = {0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02,
                          0x20, 0xE0, 0x04, 0x00, 0x32, 0x2E, 0x80, 0xA2, 0x00,
                          0x00, 0x06, 0x60, 0x00, 0x00, 0x80, 0x00, 0x06, 0x74};
  ac.setModel(kPanasonicDke);  // Make sure it isn't somehow set to JKE
  ac.setRaw(jkeState);
  EXPECT_EQ(kPanasonicJke, ac.getModel());
  EXPECT_STATE_EQ(jkeState, ac.getRaw(), kPanasonicAcBits);

  // This state tickled a bug in getModel(). Should read as CKP.
  uint8_t ckpState[27] = {0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02,
                          0x20, 0xE0, 0x04, 0x00, 0x67, 0x2E, 0x80, 0xAF, 0x00,
                          0xC0, 0x6B, 0x98, 0x10, 0x00, 0x81, 0x64, 0x05, 0x87};
  ac.setModel(kPanasonicDke);  // Make sure it isn't somehow set to CKP
  ac.setRaw(ckpState);
  EXPECT_EQ(kPanasonicCkp, ac.getModel());
  EXPECT_STATE_EQ(ckpState, ac.getRaw(), kPanasonicAcBits);
}

TEST(TestIRPanasonicAcClass, SetAndGetMode) {
  IRPanasonicAc ac(kGpioUnused);
  ac.setMode(kPanasonicAcCool);
  ac.setTemp(21);
  EXPECT_EQ(kPanasonicAcCool, ac.getMode());
  ac.setMode(kPanasonicAcHeat);
  EXPECT_EQ(kPanasonicAcHeat, ac.getMode());
  ac.setMode(kPanasonicAcAuto);
  EXPECT_EQ(kPanasonicAcAuto, ac.getMode());
  ac.setMode(kPanasonicAcDry);
  EXPECT_EQ(kPanasonicAcDry, ac.getMode());
  EXPECT_EQ(21, ac.getTemp());  // Temp should be unchanged.
  ac.setMode(kPanasonicAcFan);
  EXPECT_EQ(kPanasonicAcFan, ac.getMode());
  EXPECT_EQ(kPanasonicAcFanModeTemp, ac.getTemp());  // Temp should change.
  ac.setMode(kPanasonicAcCool);
  EXPECT_EQ(kPanasonicAcCool, ac.getMode());
  // Temp should be unchanged from the last manual change.
  EXPECT_EQ(21, ac.getTemp());
}

TEST(TestIRPanasonicAcClass, SetAndGetTemp) {
  IRPanasonicAc ac(kGpioUnused);
  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());
  ac.setTemp(kPanasonicAcMinTemp);
  EXPECT_EQ(kPanasonicAcMinTemp, ac.getTemp());
  ac.setTemp(kPanasonicAcMinTemp - 1);
  EXPECT_EQ(kPanasonicAcMinTemp, ac.getTemp());
  ac.setTemp(kPanasonicAcMaxTemp);
  EXPECT_EQ(kPanasonicAcMaxTemp, ac.getTemp());
  ac.setTemp(kPanasonicAcMaxTemp + 1);
  EXPECT_EQ(kPanasonicAcMaxTemp, ac.getTemp());
}

TEST(TestIRPanasonicAcClass, SetAndGetFan) {
  IRPanasonicAc ac(kGpioUnused);
  ac.setFan(kPanasonicAcFanAuto);
  EXPECT_EQ(kPanasonicAcFanAuto, ac.getFan());
  ac.setFan(kPanasonicAcFanMin);
  EXPECT_EQ(kPanasonicAcFanMin, ac.getFan());
  ac.setFan(kPanasonicAcFanMin - 1);
  EXPECT_EQ(kPanasonicAcFanAuto, ac.getFan());
  ac.setFan(kPanasonicAcFanLow);
  EXPECT_EQ(kPanasonicAcFanLow, ac.getFan());
  ac.setFan(kPanasonicAcFanMed);
  EXPECT_EQ(kPanasonicAcFanMed, ac.getFan());
  ac.setFan(kPanasonicAcFanHigh);
  EXPECT_EQ(kPanasonicAcFanHigh, ac.getFan());
  ac.setFan(kPanasonicAcFanMax);
  EXPECT_EQ(kPanasonicAcFanMax, ac.getFan());
  ac.setFan(kPanasonicAcFanMax + 1);
  EXPECT_EQ(kPanasonicAcFanAuto, ac.getFan());
}

TEST(TestIRPanasonicAcClass, SetAndGetSwings) {
  IRPanasonicAc ac(kGpioUnused);

  // Vertical
  ac.setSwingVertical(kPanasonicAcSwingVAuto);
  EXPECT_EQ(kPanasonicAcSwingVAuto, ac.getSwingVertical());

  ac.setSwingVertical(kPanasonicAcSwingVHighest);
  EXPECT_EQ(kPanasonicAcSwingVHighest, ac.getSwingVertical());
  ac.setSwingVertical(kPanasonicAcSwingVHighest - 1);
  EXPECT_EQ(kPanasonicAcSwingVHighest, ac.getSwingVertical());
  ac.setSwingVertical(kPanasonicAcSwingVHighest + 1);
  EXPECT_EQ(kPanasonicAcSwingVHighest + 1, ac.getSwingVertical());

  ac.setSwingVertical(kPanasonicAcSwingVLowest);
  EXPECT_EQ(kPanasonicAcSwingVLowest, ac.getSwingVertical());
  ac.setSwingVertical(kPanasonicAcSwingVLowest + 1);
  EXPECT_EQ(kPanasonicAcSwingVLowest, ac.getSwingVertical());
  ac.setSwingVertical(kPanasonicAcSwingVLowest - 1);
  EXPECT_EQ(kPanasonicAcSwingVLowest - 1, ac.getSwingVertical());

  ac.setSwingVertical(kPanasonicAcSwingVAuto);
  EXPECT_EQ(kPanasonicAcSwingVAuto, ac.getSwingVertical());

  // Horizontal is model dependant.
  ac.setModel(kPanasonicNke);  // NKE is always fixed in the middle.
  EXPECT_EQ(kPanasonicAcSwingHMiddle, ac.getSwingHorizontal());
  ac.setSwingHorizontal(kPanasonicAcSwingHAuto);
  EXPECT_EQ(kPanasonicAcSwingHMiddle, ac.getSwingHorizontal());

  ac.setModel(kPanasonicJke);  // JKE has no H swing.
  EXPECT_EQ(0, ac.getSwingHorizontal());
  ac.setSwingHorizontal(kPanasonicAcSwingHMiddle);
  EXPECT_EQ(0, ac.getSwingHorizontal());

  ac.setModel(kPanasonicLke);  // LKE is always fixed in the middle.
  EXPECT_EQ(kPanasonicAcSwingHMiddle, ac.getSwingHorizontal());
  ac.setSwingHorizontal(kPanasonicAcSwingHAuto);
  EXPECT_EQ(kPanasonicAcSwingHMiddle, ac.getSwingHorizontal());

  ac.setModel(kPanasonicDke);  // DKE has full control.
  ASSERT_EQ(kPanasonicDke, ac.getModel());
  // Auto was last requested.
  EXPECT_EQ(kPanasonicAcSwingHAuto, ac.getSwingHorizontal());
  ac.setSwingHorizontal(kPanasonicAcSwingHLeft);
  EXPECT_EQ(kPanasonicAcSwingHLeft, ac.getSwingHorizontal());
  // Changing models from DKE to something else, then back should not change
  // the intended swing.
  ac.setModel(kPanasonicLke);
  EXPECT_EQ(kPanasonicAcSwingHMiddle, ac.getSwingHorizontal());
  ac.setModel(kPanasonicDke);
  EXPECT_EQ(kPanasonicAcSwingHLeft, ac.getSwingHorizontal());
}

TEST(TestIRPanasonicAcClass, QuietAndPowerful) {
  IRPanasonicAc ac(kGpioUnused);
  ac.setQuiet(false);
  EXPECT_FALSE(ac.getQuiet());
  ac.setQuiet(true);
  EXPECT_TRUE(ac.getQuiet());
  EXPECT_FALSE(ac.getPowerful());
  ac.setPowerful(false);
  EXPECT_FALSE(ac.getPowerful());
  EXPECT_TRUE(ac.getQuiet());
  ac.setPowerful(true);
  EXPECT_TRUE(ac.getPowerful());
  EXPECT_FALSE(ac.getQuiet());
  ac.setPowerful(false);
  EXPECT_FALSE(ac.getPowerful());
  EXPECT_FALSE(ac.getQuiet());
  ac.setPowerful(true);
  ac.setQuiet(true);
  EXPECT_TRUE(ac.getQuiet());
  EXPECT_FALSE(ac.getPowerful());
}

TEST(TestIRPanasonicAcClass, SetAndGetIon) {
  IRPanasonicAc ac(0);
  // Ion Filter only works for DKE.
  ac.setModel(kPanasonicDke);
  ac.setIon(true);
  EXPECT_TRUE(ac.getIon());
  ac.setIon(false);
  EXPECT_FALSE(ac.getIon());
  ac.setIon(true);
  EXPECT_TRUE(ac.getIon());

  // Now try a different (a guess at unsupported) model.
  ac.setModel(kPanasonicRkr);
  EXPECT_FALSE(ac.getIon());
  ac.setIon(true);
  EXPECT_FALSE(ac.getIon());
  ac.setIon(false);
  EXPECT_FALSE(ac.getIon());
}

TEST(TestIRPanasonicAcClass, HumanReadable) {
  IRPanasonicAc ac(kGpioUnused);
  EXPECT_EQ(
      "Model: 4 (JKE), Power: Off, Mode: 0 (Auto), Temp: 0C, "
      "Fan: 253 (UNKNOWN), Swing(V): 0 (UNKNOWN), Quiet: Off, "
      "Powerful: Off, Clock: 00:00, On Timer: Off, Off Timer: Off",
      ac.toString());
  ac.setPower(true);
  ac.setTemp(kPanasonicAcMaxTemp);
  ac.setMode(kPanasonicAcHeat);
  ac.setFan(kPanasonicAcFanMax);
  ac.setSwingVertical(kPanasonicAcSwingVAuto);
  ac.setPowerful(true);
  EXPECT_EQ(
      "Model: 4 (JKE), Power: On, Mode: 4 (Heat), Temp: 30C, "
      "Fan: 4 (Maximum), Swing(V): 15 (Auto), Quiet: Off, "
      "Powerful: On, Clock: 00:00, On Timer: Off, Off Timer: Off",
      ac.toString());
  ac.setQuiet(true);
  ac.setModel(kPanasonicLke);
  EXPECT_EQ(
      "Model: 1 (LKE), Power: Off, Mode: 4 (Heat), Temp: 30C, "
      "Fan: 4 (Maximum), Swing(V): 15 (Auto), "
      "Swing(H): 6 (Middle), Quiet: On, Powerful: Off, "
      "Clock: 00:00, On Timer: 00:00, Off Timer: Off",
      ac.toString());
  ac.setModel(kPanasonicDke);
  ac.setSwingHorizontal(kPanasonicAcSwingHRight);
  EXPECT_EQ(
      "Model: 3 (DKE), Power: Off, Mode: 4 (Heat), Temp: 30C, "
      "Fan: 4 (Maximum), Swing(V): 15 (Auto), "
      "Swing(H): 11 (Right), Quiet: On, Powerful: Off, Ion: Off, "
      "Clock: 00:00, On Timer: Off, Off Timer: Off",
      ac.toString());
}

// Tests for decodePanasonicAC().

// Decode normal Panasonic AC messages.
TEST(TestDecodePanasonicAC, RealExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Data from Issue #525
  uint16_t rawData[439] = {
      3582, 1686, 488, 378,  488, 1238, 488, 378,  488, 378,  488, 378,
      488,  378,  488, 378,  488, 384,  488, 378,  488, 378,  488, 378,
      488,  378,  488, 378,  488, 1242, 486, 378,  488, 384,  488, 378,
      488,  378,  488, 380,  486, 382,  484, 382,  484, 1264, 464, 1266,
      460,  1272, 462, 378,  488, 406,  460, 1266, 462, 380,  488, 382,
      484,  388,  478, 406,  462, 410,  462, 404,  462, 406,  462, 396,
      470,  406,  462, 404,  462, 406,  460, 404,  462, 410,  462, 404,
      462,  404,  462, 406,  464, 406,  462, 404,  462, 406,  462, 404,
      462,  410,  462, 404,  462, 406,  462, 404,  462, 404,  462, 404,
      462,  406,  460, 406,  462, 410,  462, 404,  462, 1264, 484, 1244,
      486,  382,  482, 382,  486, 382,  486, 378,  486, 382,  488, 9924,
      3554, 1686, 488, 378,  490, 1240, 486, 378,  488, 378,  488, 378,
      488,  378,  488, 382,  484, 386,  486, 378,  488, 382,  486, 378,
      488,  382,  486, 382,  484, 1242, 486, 380,  488, 386,  484, 382,
      486,  380,  486, 382,  486, 380,  486, 380,  486, 1242, 486, 1242,
      484,  1248, 484, 380,  488, 382,  484, 1242, 486, 382,  484, 382,
      484,  382,  484, 382,  486, 386,  484, 382,  486, 382,  484, 382,
      486,  382,  486, 380,  484, 382,  486, 382,  488, 380,  486, 382,
      484,  380,  462, 406,  488, 376,  484, 1246, 482, 1246, 460, 404,
      480,  392,  484, 386,  482, 1244, 484, 382,  484, 382,  484, 1242,
      482,  1244, 484, 382,  464, 410,  460, 404,  462, 406,  462, 404,
      462,  404,  470, 396,  462, 406,  462, 404,  462, 1286, 460, 1268,
      458,  1268, 460, 1266, 460, 1266, 460, 406,  460, 1266, 462, 406,
      460,  1272, 462, 406,  460, 406,  460, 406,  460, 406,  462, 404,
      462,  406,  460, 406,  462, 410,  462, 404,  462, 406,  460, 406,
      460,  406,  462, 404,  462, 406,  460, 406,  460, 410,  462, 406,
      460,  1268, 460, 1266, 460, 404,  460, 406,  462, 406,  460, 406,
      460,  412,  456, 410,  460, 410,  438, 428,  460, 410,  456, 410,
      456,  1272, 436, 1288, 438, 434,  438, 428,  438, 428,  438, 428,
      438,  428,  438, 428,  438, 428,  438, 428,  438, 434,  438, 428,
      438,  428,  438, 428,  438, 428,  438, 428,  440, 428,  438, 428,
      438,  432,  438, 428,  438, 428,  438, 428,  438, 428,  438, 428,
      438,  428,  438, 430,  438, 1294, 438, 428,  438, 428,  438, 428,
      438,  428,  438, 428,  438, 428,  438, 428,  438, 434,  438, 428,
      438,  1288, 438, 1290, 438, 428,  438, 428,  438, 428,  438, 428,
      438,  432,  438, 1288, 438, 1290, 438, 430,  438, 428,  438, 428,
      438,  428,  438, 428,  438, 1292, 438};
  uint8_t expectedState[kPanasonicAcStateLength] = {
      0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02,
      0x20, 0xE0, 0x04, 0x00, 0x30, 0x32, 0x80, 0xAF, 0x00,
      0x00, 0x06, 0x60, 0x00, 0x00, 0x80, 0x00, 0x06, 0x83};

  irsend.sendRaw(rawData, 439, kPanasonicFreq);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(PANASONIC_AC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode synthetic Panasonic AC message.
TEST(TestDecodePanasonicAC, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Data from Issue #525
  uint8_t expectedState[kPanasonicAcStateLength] = {
      0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02,
      0x20, 0xE0, 0x04, 0x00, 0x30, 0x32, 0x80, 0xAF, 0x00,
      0x00, 0x06, 0x60, 0x00, 0x00, 0x80, 0x00, 0x06, 0x83};

  irsend.sendPanasonicAC(expectedState);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(PANASONIC_AC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_EQ(
      "Model: 4 (JKE), Power: Off, Mode: 3 (Cool), Temp: 25C, "
      "Fan: 7 (Auto), Swing(V): 15 (Auto), Quiet: Off, "
      "Powerful: Off, Clock: 00:00, On Timer: Off, Off Timer: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

// Tests for general utility functions.
TEST(TestGeneralPanasonic, hasACState) {
  EXPECT_TRUE(hasACState(PANASONIC_AC));
  ASSERT_FALSE(hasACState(PANASONIC));
  ASSERT_FALSE(hasACState(PANASONIC_AC32));
}

TEST(TestGeneralPanasonic, typeToString) {
  EXPECT_EQ("PANASONIC_AC", typeToString(PANASONIC_AC));
  EXPECT_EQ("PANASONIC", typeToString(PANASONIC));
}

// Decode a problematic Panasonic AC message
TEST(TestDecodePanasonicAC, Issue540) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Data from Issue #540
  uint16_t rawData[439] = {
      3512, 1714, 466, 408,  466, 1280, 470, 408,  466, 412,  466, 408,
      466,  412,  462, 412,  466, 414,  466, 408,  466, 412,  462, 412,
      466,  412,  466, 408,  466, 1280, 466, 412,  462, 416,  462, 412,
      466,  408,  466, 412,  462, 416,  462, 412,  462, 1282, 462, 1284,
      462,  1288, 466, 412,  462, 412,  462, 1284, 462, 416,  440, 438,
      462,  412,  462, 412,  462, 416,  466, 412,  462, 412,  462, 412,
      440,  442,  462, 412,  462, 412,  460, 418,  462, 416,  462, 412,
      462,  418,  462, 412,  462, 416,  462, 412,  436, 442,  462, 412,
      460,  418,  462, 416,  462, 412,  460, 412,  462, 420,  436, 438,
      462,  412,  462, 416,  432, 448,  436, 438,  436, 1310, 436, 1310,
      462,  420,  432, 442,  436, 438,  462, 416,  432, 444,  432, 10008,
      3480, 1744, 492, 382,  492, 1254, 492, 386,  488, 390,  492, 382,
      492,  386,  488, 386,  492, 386,  492, 386,  488, 386,  488, 386,
      492,  386,  492, 382,  492, 1258, 488, 386,  488, 390,  492, 386,
      488,  386,  488, 386,  492, 390,  488, 386,  488, 1256, 488, 1258,
      488,  1262, 488, 390,  488, 386,  488, 1258, 488, 390,  488, 392,
      488,  386,  488, 386,  488, 394,  488, 386,  488, 386,  488, 390,
      488,  390,  488, 386,  488, 390,  462, 412,  488, 390,  462, 1282,
      488,  390,  456, 416,  458, 1292, 456, 1288, 488, 1258, 488, 392,
      456,  422,  488, 390,  484, 392,  484, 1262, 458, 420,  484, 1262,
      482,  1262, 488, 392,  484, 394,  484, 416,  436, 442,  458, 416,
      458,  422,  430, 448,  432, 442,  458, 416,  458, 1296, 432, 1314,
      458,  1288, 432, 1312, 432, 1322, 428, 446,  428, 1318, 432, 442,
      432,  1318, 432, 1318, 428, 446,  428, 1318, 428, 1322, 430, 448,
      426,  448,  428, 452,  426, 452,  426, 448,  428, 472,  400, 478,
      402,  478,  402, 472,  402, 476,  402, 472,  402, 478,  402, 472,
      402,  1348, 398, 1348, 398, 1352, 398, 508,  370, 478,  398, 476,
      398,  512,  366, 508,  370, 502,  372, 508,  340, 538,  372, 504,
      344,  1400, 344, 1400, 346, 1434, 314, 560,  316, 588,  290, 560,
      314,  564,  396, 400,  474, 400,  480, 394,  480, 404,  474, 400,
      454,  446,  454, 426,  448, 430,  424, 450,  428, 452,  448, 426,
      426,  452,  424, 1322, 454, 426,  450, 424,  426, 452,  428, 452,
      450,  424,  428, 446,  426, 1322, 454, 426,  422, 450,  454, 426,
      448,  430,  454, 426,  448, 426,  428, 446,  454, 430,  454, 422,
      452,  424,  424, 452,  452, 430,  424, 452,  452, 426,  448, 426,
      426,  456,  448, 426,  448, 1296, 424, 1322, 426, 1326, 450, 1270,
      478,  422,  454, 424,  424, 450,  454};
  uint8_t expectedState[kPanasonicAcStateLength] = {
      0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02,
      0x20, 0xE0, 0x04, 0x00, 0x39, 0x34, 0x80, 0xAF, 0x0D,
      0x00, 0x0E, 0xE0, 0x00, 0x00, 0x81, 0x00, 0x00, 0x1E};

  irsend.sendRaw(rawData, 439, kPanasonicFreq);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(PANASONIC_AC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  IRPanasonicAc ac(kGpioUnused);
  ac.setRaw(irsend.capture.state);
  // TODO(crankyoldgit): Try to figure out what model this should be.
  EXPECT_EQ(
      "Model: 0 (UNKNOWN), Power: On, Mode: 3 (Cool), Temp: 26C, "
      "Fan: 7 (Auto), Swing(V): 15 (Auto), "
      "Swing(H): 13 (Auto), Quiet: Off, Powerful: Off, "
      "Clock: 00:00, On Timer: Off, Off Timer: Off",
      ac.toString());
}

TEST(TestIRPanasonicAcClass, TimeBasics) {
  EXPECT_EQ(0x186, IRPanasonicAc::encodeTime(6, 30));
  EXPECT_EQ(0x3CA, IRPanasonicAc::encodeTime(16, 10));
  EXPECT_EQ(0x448, IRPanasonicAc::encodeTime(18, 16));
  EXPECT_EQ(0, IRPanasonicAc::encodeTime(0, 0));
  EXPECT_EQ(kPanasonicAcTimeMax, IRPanasonicAc::encodeTime(23, 59));
  EXPECT_EQ(kPanasonicAcTimeMax, IRPanasonicAc::encodeTime(23, 59));
  EXPECT_EQ(kPanasonicAcTimeMax, IRPanasonicAc::encodeTime(25, 72));
  EXPECT_EQ(59, IRPanasonicAc::encodeTime(0, 72));
  EXPECT_EQ(23 * 60, IRPanasonicAc::encodeTime(27, 0));
}

TEST(TestIRPanasonicAcClass, TimersAndClock) {
  IRPanasonicAc ac(kGpioUnused);
  // Data from Issue #544
  uint8_t state[27] = {0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02,
                       0x20, 0xE0, 0x04, 0x00, 0x4E, 0x2E, 0x80, 0xAF, 0x00,
                       0xCA, 0x6B, 0x98, 0x10, 0x00, 0x01, 0x48, 0x04, 0xDB};
  ac.setRaw(state);
  EXPECT_TRUE(ac.isOnTimerEnabled());
  EXPECT_EQ(0x3CA, ac.getOnTimer());
  EXPECT_TRUE(ac.isOffTimerEnabled());
  EXPECT_EQ(0x186, ac.getOffTimer());
  EXPECT_EQ(0x448, ac.getClock());

  ac.cancelOnTimer();
  EXPECT_FALSE(ac.isOnTimerEnabled());
  EXPECT_EQ(0, ac.getOnTimer());
  EXPECT_TRUE(ac.isOffTimerEnabled());
  EXPECT_EQ(0x186, ac.getOffTimer());
  EXPECT_EQ(0x448, ac.getClock());

  ac.cancelOffTimer();
  EXPECT_FALSE(ac.isOnTimerEnabled());
  EXPECT_EQ(0, ac.getOnTimer());
  EXPECT_FALSE(ac.isOffTimerEnabled());
  EXPECT_EQ(0, ac.getOffTimer());
  EXPECT_EQ(0x448, ac.getClock());

  ac.setOnTimer(7 * 60 + 50);
  EXPECT_TRUE(ac.isOnTimerEnabled());
  EXPECT_EQ(7 * 60 + 50, ac.getOnTimer());
  EXPECT_FALSE(ac.isOffTimerEnabled());
  EXPECT_EQ(0, ac.getOffTimer());
  EXPECT_EQ(0x448, ac.getClock());

  ac.setOnTimer(7 * 60 + 57);  // It should round down.
  EXPECT_EQ(7 * 60 + 50, ac.getOnTimer());
  ac.setOnTimer(28 * 60);  // It should round down.
  EXPECT_EQ(kPanasonicAcTimeMax - 9, ac.getOnTimer());
  ac.setOnTimer(kPanasonicAcTimeSpecial);
  EXPECT_EQ(0, ac.getOnTimer());

  ac.setOnTimer(7 * 60 + 50);
  ac.setOffTimer(19 * 60 + 30);

  EXPECT_TRUE(ac.isOnTimerEnabled());
  EXPECT_EQ(7 * 60 + 50, ac.getOnTimer());
  EXPECT_TRUE(ac.isOffTimerEnabled());
  EXPECT_EQ(19 * 60 + 30, ac.getOffTimer());
  EXPECT_EQ(0x448, ac.getClock());

  ac.setOffTimer(19 * 60 + 57);  // It should round down.
  EXPECT_EQ(19 * 60 + 50, ac.getOffTimer());
  ac.setOffTimer(28 * 60);  // It should round down.
  EXPECT_EQ(kPanasonicAcTimeMax - 9, ac.getOffTimer());
  ac.setOffTimer(kPanasonicAcTimeSpecial);
  EXPECT_EQ(0, ac.getOffTimer());

  ac.setClock(0);
  EXPECT_EQ(0, ac.getClock());
  ac.setClock(kPanasonicAcTimeMax);
  EXPECT_EQ(kPanasonicAcTimeMax, ac.getClock());
  ac.setClock(kPanasonicAcTimeMax - 1);
  EXPECT_EQ(kPanasonicAcTimeMax - 1, ac.getClock());
  ac.setClock(kPanasonicAcTimeMax + 1);
  EXPECT_EQ(kPanasonicAcTimeMax, ac.getClock());
  ac.setClock(kPanasonicAcTimeSpecial);
  EXPECT_EQ(0, ac.getClock());
}

// Decode a real short Panasonic AC message
TEST(TestDecodePanasonicAC, RealExampleOfShortMessage) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Data from Issue #544 (Odour Wash)
  uint16_t rawData[263] = {
      3496, 1734, 506, 366,  448, 1294, 504, 368,  498, 374,  452, 418,
      448,  424,  444, 428,  450, 422,  446, 426,  450, 420,  448, 424,
      452,  418,  448, 422,  444, 1300, 498, 374,  504, 368,  448, 424,
      452,  418,  448, 424,  444, 428,  450, 422,  446, 1296, 500, 1242,
      502,  1242, 504, 368,  498, 374,  452, 1292, 504, 366,  450, 422,
      444,  426,  450, 420,  446, 424,  452, 418,  448, 424,  444, 428,
      450,  422,  444, 426,  450, 420,  446, 424,  452, 418,  448, 422,
      444,  428,  450, 422,  446, 426,  452, 420,  446, 426,  452, 418,
      448,  424,  442, 428,  448, 422,  444, 426,  450, 420,  446, 426,
      452,  418,  448, 424,  444, 428,  450, 422,  444, 1298, 500, 1244,
      500,  372,  444, 428,  450, 422,  446, 426,  452, 418,  448, 10020,
      3500, 1732, 498, 372,  452, 1290, 506, 366,  450, 422,  446, 426,
      452,  420,  448, 424,  452, 418,  448, 422,  444, 426,  450, 420,
      446,  426,  452, 420,  446, 1296, 500, 370,  444, 428,  450, 422,
      446,  426,  452, 420,  446, 424,  442, 428,  448, 1294, 502, 1240,
      504,  1238, 506, 366,  448, 422,  444, 1298, 498, 374,  452, 418,
      448,  424,  444, 428,  450, 422,  446, 426,  450, 420,  446, 424,
      452,  418,  448, 422,  444, 428,  450, 420,  446, 1298, 498, 1244,
      500,  1242, 502, 368,  446, 1298, 500, 1244, 500, 372,  444, 428,
      450,  1292, 504, 368,  446, 1296, 502, 370,  444, 426,  452, 1290,
      504,  1238, 506, 366,  450, 422,  446, 1298, 498, 1246, 500, 372,
      444,  428,  450, 1294, 452, 420,  446, 1296, 448, 422,  444};  // UNKNOWN
                                                                     // 1FB51F79

  uint8_t expectedState[kPanasonicAcStateShortLength] = {
      0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06,
      0x02, 0x20, 0xE0, 0x04, 0x80, 0x9B, 0x32, 0x53};

  irsend.sendRaw(rawData, 263, kPanasonicFreq);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(PANASONIC_AC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicAcShortBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
}

// Create and decode a short Panasonic AC message
TEST(TestDecodePanasonicAC, SyntheticShortMessage) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  uint8_t odourWash[kPanasonicAcStateShortLength] = {
      0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06,
      0x02, 0x20, 0xE0, 0x04, 0x80, 0x9B, 0x32, 0x53};

  irsend.sendPanasonicAC(odourWash, kPanasonicAcStateShortLength);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(PANASONIC_AC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicAcShortBits, irsend.capture.bits);
  EXPECT_STATE_EQ(odourWash, irsend.capture.state, irsend.capture.bits);
}
//
// Test for CKP model / see issue #544
TEST(TestDecodePanasonicAC, CkpModelSpecifics) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Data from Issue #544
  uint8_t ckpPowerfulOn[kPanasonicAcStateLength] = {
      0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02,
      0x20, 0xE0, 0x04, 0x00, 0x4E, 0x2E, 0x80, 0xAF, 0x00,
      0x00, 0x0E, 0xE0, 0x11, 0x00, 0x01, 0x00, 0x06, 0xB7};
  uint8_t ckpQuietOn[kPanasonicAcStateLength] = {
      0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02,
      0x20, 0xE0, 0x04, 0x00, 0x4E, 0x2E, 0x80, 0xAF, 0x00,
      0x00, 0x0E, 0xE0, 0x30, 0x00, 0x01, 0x00, 0x06, 0xD6};

  irsend.sendPanasonicAC(ckpPowerfulOn);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(PANASONIC_AC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(ckpPowerfulOn, irsend.capture.state, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);

  IRPanasonicAc ac(kGpioUnused);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Model: 5 (CKP), Power: Off, Mode: 4 (Heat), Temp: 23C, "
      "Fan: 7 (Auto), Swing(V): 15 (Auto), Quiet: Off, "
      "Powerful: On, Clock: 00:00, On Timer: 00:00, Off Timer: 00:00",
      ac.toString());

  ac.setQuiet(true);
  EXPECT_FALSE(ac.getPowerful());
  EXPECT_TRUE(ac.getQuiet());
  EXPECT_EQ(kPanasonicCkp, ac.getModel());
  EXPECT_STATE_EQ(ckpQuietOn, ac.getRaw(), kPanasonicAcBits);

  ac.setPowerful(true);
  EXPECT_TRUE(ac.getPowerful());
  EXPECT_FALSE(ac.getQuiet());
  EXPECT_EQ(kPanasonicCkp, ac.getModel());
  EXPECT_STATE_EQ(ckpPowerfulOn, ac.getRaw(), kPanasonicAcBits);
}

TEST(TestIRPanasonicAcClass, toCommon) {
  IRPanasonicAc ac(0);
  ac.setModel(panasonic_ac_remote_model_t::kPanasonicDke);
  ac.setPower(true);
  ac.setMode(kPanasonicAcCool);
  ac.setTemp(20);
  ac.setFan(kPanasonicAcFanMax);
  ac.setSwingVertical(kPanasonicAcSwingVAuto);
  ac.setSwingHorizontal(kPanasonicAcSwingHMiddle);
  ac.setPowerful(true);
  ac.setQuiet(false);
  // Now test it.
  ASSERT_EQ(decode_type_t::PANASONIC_AC, ac.toCommon().protocol);
  ASSERT_EQ(panasonic_ac_remote_model_t::kPanasonicDke, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kAuto, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kMiddle, ac.toCommon().swingh);
  ASSERT_TRUE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().quiet);
  // Unsupported.
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);

  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/921#issuecomment-532267240
  ac.setSwingVertical(kPanasonicAcSwingVLow);
  ASSERT_EQ(stdAc::swingv_t::kLow, ac.toCommon().swingv);
  // Real data.
  uint8_t swingVMiddle[27] = {
      0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02, 0x20, 0xE0, 0x04,
      0x00, 0x39, 0x32, 0x80, 0x73, 0x00, 0x00, 0x0E, 0xE0, 0x00, 0x00, 0x89,
      0x00, 0x00, 0xDB};
  ac.setRaw(swingVMiddle);
  ASSERT_EQ(stdAc::swingv_t::kMiddle, ac.toCommon().swingv);
  ASSERT_EQ(kPanasonicAcSwingVMiddle,
            IRPanasonicAc::convertSwingV(stdAc::swingv_t::kMiddle));
}

//
// Test for DKE/DKW model / see issue #1024
TEST(TestDecodePanasonicAC, DkeIonRealMessages) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Data from Issue #1024
  // 0x0220E004000000060220E004004F3280AF0D000660000001000630
  uint8_t dkeIonOff[kPanasonicAcStateLength] = {
      0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02,
      0x20, 0xE0, 0x04, 0x00, 0x4F, 0x32, 0x80, 0xAF, 0x0D,
      0x00, 0x06, 0x60, 0x00, 0x00, 0x01, 0x00, 0x06, 0x30};

  // 0x0220E004000000060220E004004F3280AF0D000660000101000631
  uint8_t dkeIonOn[kPanasonicAcStateLength] = {
      0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02,
      0x20, 0xE0, 0x04, 0x00, 0x4F, 0x32, 0x80, 0xAF, 0x0D,
      0x00, 0x06, 0x60, 0x00, 0x01, 0x01, 0x00, 0x06, 0x31};

  IRPanasonicAc ac(0);
  ac.setRaw(dkeIonOff);
  EXPECT_EQ(
      "Model: 3 (DKE), Power: On, Mode: 4 (Heat), Temp: 25C, Fan: 7 (Auto), "
      "Swing(V): 15 (Auto), Swing(H): 13 (Auto), Quiet: Off, Powerful: Off, "
      "Ion: Off, Clock: 00:00, On Timer: 00:00, Off Timer: 00:00",
      ac.toString());
  ac.setRaw(dkeIonOn);
  EXPECT_EQ(
      "Model: 3 (DKE), Power: On, Mode: 4 (Heat), Temp: 25C, Fan: 7 (Auto), "
      "Swing(V): 15 (Auto), Swing(H): 13 (Auto), Quiet: Off, Powerful: Off, "
      "Ion: On, Clock: 00:00, On Timer: 00:00, Off Timer: 00:00",
      ac.toString());
}

// General housekeeping
TEST(TestPanasonic, Housekeeping) {
  // Simple Panasonic protocol. e.g. TVs etc.
  ASSERT_EQ(D_STR_PANASONIC, typeToString(decode_type_t::PANASONIC));
  ASSERT_FALSE(hasACState(decode_type_t::PANASONIC));
  ASSERT_EQ(decode_type_t::PANASONIC, strToDecodeType(D_STR_PANASONIC));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::PANASONIC));
  ASSERT_EQ(kPanasonicBits, IRsend::defaultBits(decode_type_t::PANASONIC));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::PANASONIC));
  // Panasonic A/Cs
  ASSERT_EQ(D_STR_PANASONIC_AC, typeToString(decode_type_t::PANASONIC_AC));
  ASSERT_TRUE(hasACState(decode_type_t::PANASONIC_AC));
  ASSERT_EQ(decode_type_t::PANASONIC_AC, strToDecodeType(D_STR_PANASONIC_AC));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::PANASONIC_AC));
  ASSERT_EQ(kPanasonicAcBits, IRsend::defaultBits(decode_type_t::PANASONIC_AC));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::PANASONIC_AC));
  // Panasonic A/Cs (32bit)
  ASSERT_EQ(D_STR_PANASONIC_AC32, typeToString(decode_type_t::PANASONIC_AC32));
  ASSERT_EQ(decode_type_t::PANASONIC_AC32,
            strToDecodeType(D_STR_PANASONIC_AC32));
  ASSERT_FALSE(hasACState(decode_type_t::PANASONIC_AC32));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::PANASONIC_AC32));
  ASSERT_EQ(kPanasonicAc32Bits,
            IRsend::defaultBits(decode_type_t::PANASONIC_AC32));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::PANASONIC_AC32));
}

// Decode a real Panasonic AC 32 bit message
TEST(TestDecodePanasonicAC32, RealMessage) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  // https://github.com/crankyoldgit/IRremoteESP8266/issues/1307#issuecomment-717436003
  // https://github.com/crankyoldgit/IRremoteESP8266/files/5446781/temp-take3_36khz.txt
  // # Remote: 17 C, auto mode, auto top swing, auto left/right swing, auto fan
  const uint16_t rawData[271] = {
      3544, 3448,
      922, 830, 918, 2576, 922, 830, 918, 830,
      918, 2576, 922, 2574, 922, 2576, 922, 2576,
      922, 826, 922, 2574, 922, 828, 920, 826,
      922, 2574, 922, 2574, 922, 2576, 920, 2576,
      918, 2578, 918, 830, 918, 2574, 922, 2574,
      922, 830, 918, 830, 918, 830, 918, 826,
      922, 2574, 922, 826, 922, 2576, 922, 2574,
      922, 826, 922, 826, 922, 826, 922, 826,
      3544, 3452,
      918, 830, 918, 2574, 922, 830, 918, 830,
      918, 2576, 922, 2576, 922, 2576, 918, 2580,
      918, 830, 918, 2580, 918, 830, 918, 832,
      916, 2576, 922, 2576, 922, 2576, 918, 2580,
      918, 2580, 916, 832, 918, 2576, 922, 2574,
      922, 826, 922, 826, 922, 826, 922, 826,
      922, 2576, 916, 830, 918, 2580, 918, 2578,
      918, 832, 920, 826, 922, 826, 922, 826,
      3544, 3450,
      922, 13946,
      3542, 3450,
      922, 826, 922, 826, 922, 2574, 922, 2574,
      922, 2574, 922, 2574, 922, 2574, 922, 2574,
      920, 830, 918, 830, 916, 2578, 918, 2580,
      916, 2580, 916, 2580, 918, 2578, 922, 2574,
      922, 826, 922, 2576, 922, 2574, 922, 826,
      922, 2574, 922, 2574, 922, 826, 922, 826,
      922, 830, 918, 2574, 922, 2574, 922, 830,
      916, 2574, 922, 2574, 922, 830, 918, 830,
      3542, 3450,
      918, 830, 916, 830, 918, 2578, 922, 2574,
      922, 2574, 922, 2574, 922, 2574, 922, 2574,
      922, 830, 918, 830, 916, 2574, 922, 2574,
      922, 2576, 920, 2574, 922, 2574, 922, 2576,
      922, 830, 916, 2580, 916, 2580, 916, 830,
      918, 2578, 916, 2580, 918, 830, 922, 826,
      922, 826, 922, 2574, 922, 2574, 924, 824,
      922, 2574, 922, 2574, 922, 826, 922, 826,
      3542, 3452,
      922};  // UNKNOWN AA94330B

  irsend.sendRaw(rawData, 271, kPanasonicFreq);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(PANASONIC_AC32, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicAc32Bits, irsend.capture.bits);
  EXPECT_EQ(0x0DF236FC, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  EXPECT_EQ(
      "Power Toggle: Off, Mode: 5 (UNKNOWN), Temp: 17C, Fan: 15 (Auto), "
      "Swing(H): On, Swing(V): 7 (Auto)",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

// Decode a synthetic Panasonic AC 32 bit message
TEST(TestDecodePanasonicAC32, SyntheticMessage) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.sendPanasonicAC32(0x0DF236FC);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(PANASONIC_AC32, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicAc32Bits, irsend.capture.bits);
  EXPECT_EQ(0x0DF236FC, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  EXPECT_EQ(
      "f36700d50"
      "m3543s3450"
      "m920s828m920s2575m920s828m920s828m920s2575m920s2575m920s2575m920s2575"
      "m920s828m920s2575m920s828m920s828m920s2575m920s2575m920s2575m920s2575"
      "m920s2575m920s828m920s2575m920s2575m920s828m920s828m920s828m920s828"
      "m920s2575m920s828m920s2575m920s2575m920s828m920s828m920s828m920s828"
      "m3543s3450"
      "m920s828m920s2575m920s828m920s828m920s2575m920s2575m920s2575m920s2575"
      "m920s828m920s2575m920s828m920s828m920s2575m920s2575m920s2575m920s2575"
      "m920s2575m920s828m920s2575m920s2575m920s828m920s828m920s828m920s828"
      "m920s2575m920s828m920s2575m920s2575m920s828m920s828m920s828m920s828"
      "m3543s3450"
      "m920s13946"
      "m3543s3450"
      "m920s828m920s828m920s2575m920s2575m920s2575m920s2575m920s2575m920s2575"
      "m920s828m920s828m920s2575m920s2575m920s2575m920s2575m920s2575m920s2575"
      "m920s828m920s2575m920s2575m920s828m920s2575m920s2575m920s828m920s828"
      "m920s828m920s2575m920s2575m920s828m920s2575m920s2575m920s828m920s828"
      "m3543s3450"
      "m920s828m920s828m920s2575m920s2575m920s2575m920s2575m920s2575m920s2575"
      "m920s828m920s828m920s2575m920s2575m920s2575m920s2575m920s2575m920s2575"
      "m920s828m920s2575m920s2575m920s828m920s2575m920s2575m920s828m920s828"
      "m920s828m920s2575m920s2575m920s828m920s2575m920s2575m920s828m920s828"
      "m3543s3450"
      "m920s13946",
      irsend.outputStr());
}

// Decode a real *short* (16 bit) Panasonic AC 32 bit message
TEST(TestDecodePanasonicAC32, RealShortMessage) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  // https://github.com/crankyoldgit/IRremoteESP8266/issues/1307#issuecomment-719038870
  // https://github.com/crankyoldgit/IRremoteESP8266/files/5461382/quiet-powerful.txt
  const uint16_t rawData[201] = {
      3548, 3448, 922, 826, 922, 2576, 922, 2574, 918, 832, 916, 830, 918, 830,
      918, 830, 918, 2574, 924, 824, 922, 2576, 922, 2574, 922, 826, 922, 826,
      922, 826, 922, 826, 922, 2574, 922, 2574, 918, 830, 918, 2574, 922, 832,
      916, 2574, 922, 2574, 922, 830, 918, 830, 918, 2574, 922, 826, 922, 2576,
      922, 826, 922, 2574, 922, 2574, 922, 826, 922, 826, 3542, 3452, 918, 830,
      918, 2574, 922, 2574, 922, 830, 916, 830, 918, 830, 918, 830, 918, 2574,
      922, 832, 916, 2574, 922, 2574, 922, 830, 918, 830, 916, 830, 918, 830,
      918, 2576, 922, 2574, 922, 826, 922, 2574, 922, 826, 922, 2574, 922, 2574,
      918, 830, 916, 830, 918, 2580, 918, 830, 918, 2578, 918, 830, 918, 2580,
      918, 2574, 922, 830, 918, 830, 3544, 3446, 922, 832, 916, 2574, 922, 2574,
      922, 826, 922, 826, 922, 826, 922, 826, 922, 2574, 918, 830, 918, 2580,
      916, 2580, 918, 830, 918, 830, 918, 830, 916, 830, 918, 2580, 918, 2574,
      922, 830, 918, 2574, 922, 830, 916, 2576, 922, 2574, 922, 830, 918, 830,
      918, 2574, 922, 830, 916, 2576, 922, 830, 918, 2574, 922, 2576, 922, 830,
      918, 830, 3538, 3450, 922};  // UNKNOWN A9E430DB

  irsend.sendRaw(rawData, 201, kPanasonicFreq);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(PANASONIC_AC32, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicAc32Bits / 2, irsend.capture.bits);
  EXPECT_EQ(0x3586, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  EXPECT_EQ(
      "", IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_FALSE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

// Decode a synthetic *short Panasonic AC 64 bit message
TEST(TestDecodePanasonicAC32, SyntheticShortMessage) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.sendPanasonicAC32(0x3586, kPanasonicAc32Bits / 2);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(PANASONIC_AC32, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicAc32Bits / 2, irsend.capture.bits);
  EXPECT_EQ(0x3586, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  EXPECT_EQ(
      "f36700d50"
      "m3543s3450"
      "m920s828m920s2575m920s2575m920s828m920s828m920s828m920s828m920s2575"
      "m920s828m920s2575m920s2575m920s828m920s828m920s828m920s828m920s2575"
      "m920s2575m920s828m920s2575m920s828m920s2575m920s2575m920s828m920s828"
      "m920s2575m920s828m920s2575m920s828m920s2575m920s2575m920s828m920s828"
      "m3543s3450"
      "m920s828m920s2575m920s2575m920s828m920s828m920s828m920s828m920s2575"
      "m920s828m920s2575m920s2575m920s828m920s828m920s828m920s828m920s2575"
      "m920s2575m920s828m920s2575m920s828m920s2575m920s2575m920s828m920s828"
      "m920s2575m920s828m920s2575m920s828m920s2575m920s2575m920s828m920s828"
      "m3543s3450"
      "m920s828m920s2575m920s2575m920s828m920s828m920s828m920s828m920s2575"
      "m920s828m920s2575m920s2575m920s828m920s828m920s828m920s828m920s2575"
      "m920s2575m920s828m920s2575m920s828m920s2575m920s2575m920s828m920s828"
      "m920s2575m920s828m920s2575m920s828m920s2575m920s2575m920s828m920s828"
      "m3543s3450"
      "m920s13946",
      irsend.outputStr());
}

TEST(TestIRPanasonicAc32Class, SetAndGetPower) {
  IRPanasonicAc32 ac(kGpioUnused);
  ac.setPowerToggle(false);
  EXPECT_FALSE(ac.getPowerToggle());
  ac.setPowerToggle(true);
  EXPECT_TRUE(ac.getPowerToggle());
  ac.setPowerToggle(false);
  EXPECT_FALSE(ac.getPowerToggle());
  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1364#issuecomment-750427966
  ac.setRaw(0x04FF36A0);
  EXPECT_TRUE(ac.getPowerToggle());
}

TEST(TestIRPanasonicAc32Class, SetAndGetFan) {
  IRPanasonicAc32 ac(kGpioUnused);
  ac.setFan(kPanasonicAc32FanAuto);
  EXPECT_EQ(kPanasonicAc32FanAuto, ac.getFan());
  ac.setFan(kPanasonicAc32FanMin);
  EXPECT_EQ(kPanasonicAc32FanMin, ac.getFan());
  ac.setFan(kPanasonicAc32FanMin - 1);
  EXPECT_EQ(kPanasonicAc32FanAuto, ac.getFan());
  ac.setFan(kPanasonicAc32FanMed);
  EXPECT_EQ(kPanasonicAc32FanMed, ac.getFan());
  ac.setFan(kPanasonicAc32FanMax);
  EXPECT_EQ(kPanasonicAc32FanMax, ac.getFan());
  ac.setFan(kPanasonicAc32FanMax + 1);
  EXPECT_EQ(kPanasonicAc32FanAuto, ac.getFan());
}

TEST(TestIRPanasonicAc32Class, SetAndGetMode) {
  IRPanasonicAc32 ac(kGpioUnused);
  ac.setMode(kPanasonicAc32Cool);
  EXPECT_EQ(kPanasonicAc32Cool, ac.getMode());
  ac.setMode(kPanasonicAc32Heat);
  EXPECT_EQ(kPanasonicAc32Heat, ac.getMode());
  ac.setMode(kPanasonicAc32Auto);
  EXPECT_EQ(kPanasonicAc32Auto, ac.getMode());
  ac.setMode(kPanasonicAc32Dry);
  EXPECT_EQ(kPanasonicAc32Dry, ac.getMode());
  ac.setMode(kPanasonicAcFan);
  EXPECT_EQ(kPanasonicAcFan, ac.getMode());
  ac.setMode(255);
  EXPECT_EQ(kPanasonicAc32Auto, ac.getMode());
}

TEST(TestIRPanasonicAc32Class, SetAndGetTemp) {
  IRPanasonicAc ac(kGpioUnused);
  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());
  ac.setTemp(kPanasonicAcMinTemp);
  EXPECT_EQ(kPanasonicAcMinTemp, ac.getTemp());
  ac.setTemp(kPanasonicAcMinTemp - 1);
  EXPECT_EQ(kPanasonicAcMinTemp, ac.getTemp());
  ac.setTemp(kPanasonicAcMaxTemp);
  EXPECT_EQ(kPanasonicAcMaxTemp, ac.getTemp());
  ac.setTemp(kPanasonicAcMaxTemp + 1);
  EXPECT_EQ(kPanasonicAcMaxTemp, ac.getTemp());
}

TEST(TestIRPanasonicAc32Class, SetAndGetSwings) {
  IRPanasonicAc32 ac(kGpioUnused);

  // Horizontal
  ac.setSwingHorizontal(true);
  EXPECT_TRUE(ac.getSwingHorizontal());
  ac.setSwingHorizontal(false);
  EXPECT_FALSE(ac.getSwingHorizontal());
  ac.setSwingHorizontal(true);
  EXPECT_TRUE(ac.getSwingHorizontal());

  // Vertical
  ac.setSwingVertical(kPanasonicAc32SwingVAuto);
  EXPECT_EQ(kPanasonicAc32SwingVAuto, ac.getSwingVertical());

  ac.setSwingVertical(kPanasonicAcSwingVHighest);
  EXPECT_EQ(kPanasonicAcSwingVHighest, ac.getSwingVertical());
  ac.setSwingVertical(kPanasonicAcSwingVHighest - 1);
  EXPECT_EQ(kPanasonicAcSwingVHighest, ac.getSwingVertical());
  ac.setSwingVertical(kPanasonicAcSwingVHighest + 1);
  EXPECT_EQ(kPanasonicAcSwingVHighest + 1, ac.getSwingVertical());

  ac.setSwingVertical(kPanasonicAcSwingVLowest);
  EXPECT_EQ(kPanasonicAcSwingVLowest, ac.getSwingVertical());
  ac.setSwingVertical(kPanasonicAcSwingVLowest + 1);
  EXPECT_EQ(kPanasonicAcSwingVLowest, ac.getSwingVertical());
  ac.setSwingVertical(kPanasonicAcSwingVLowest - 1);
  EXPECT_EQ(kPanasonicAcSwingVLowest - 1, ac.getSwingVertical());

  ac.setSwingVertical(kPanasonicAc32SwingVAuto);
  EXPECT_EQ(kPanasonicAc32SwingVAuto, ac.getSwingVertical());
}

TEST(TestIRPanasonicAc32Class, HumanReadable) {
  IRPanasonicAc32 ac(kGpioUnused);

  EXPECT_EQ(
      "Power Toggle: Off, Mode: 2 (Cool), Temp: 16C, Fan: 15 (Auto), "
      "Swing(H): On, Swing(V): 7 (Auto)",
      ac.toString());

  ac.setMode(kPanasonicAc32Heat);
  ac.setTemp(24);
  ac.setFan(kPanasonicAc32FanMed);
  ac.setSwingHorizontal(false);
  ac.setSwingVertical(kPanasonicAcSwingVLowest);
  ac.setPowerToggle(true);
  EXPECT_EQ(
      "Power Toggle: On, Mode: 4 (Heat), Temp: 24C, Fan: 4 (Medium), "
      "Swing(H): Off, Swing(V): 5 (Lowest)",
      ac.toString());
}
