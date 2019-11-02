// Copyright 2017, 2018 David Conran

#include "ir_Panasonic.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
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
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kPanasonicBits, true));
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
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kPanasonicBits, true));
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
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kPanasonicBits, true));
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
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kPanasonicBits, true));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicBits, irsend.capture.bits);
  EXPECT_EQ(0x40040190ED7C, irsend.capture.value);
  EXPECT_EQ(0x4004, irsend.capture.address);
  EXPECT_EQ(0x190ED7C, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  irsend.makeDecodeResult(2 * kPanasonicBits + 4);
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kPanasonicBits, true));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicBits, irsend.capture.bits);
  EXPECT_EQ(0x40040190ED7C, irsend.capture.value);

  irsend.makeDecodeResult(2 * (2 * kPanasonicBits + 4));
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kPanasonicBits, true));
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
  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, kPanasonicBits, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kPanasonicBits, false));
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
  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, kPanasonicBits, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kPanasonicBits, false));
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

  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, kPanasonicBits, true));

  irsend.makeDecodeResult();
  // Should fail with strict when we ask for the wrong bit size.
  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, 32, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, 32, false));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(32, irsend.capture.bits);
  EXPECT_EQ(0x12345678, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x12345678, irsend.capture.command);

  // Illegal over length (56-bit) message.
  irsend.reset();
  irsend.sendPanasonic64(irsend.encodePanasonic(0x4004, 1, 2, 3), 56);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, kPanasonicBits, true));
  // Shouldn't pass if strict off and wrong bit size.
  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, kPanasonicBits, false));
  // Re-decode with correct bit size.
  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, 56, true));
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, 56, false));
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
  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, 64, true));
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, 64, false));
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

  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kPanasonicBits, true));
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
  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, kPanasonicBits, false));
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
  IRsendTest irsend(0);
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
  IRPanasonicAc pana(0);

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
  pana.setRaw(examplestate);
  // Extracting the state from the object should have a correct checksum.
  EXPECT_TRUE(IRPanasonicAc::validChecksum(pana.getRaw()));
  EXPECT_STATE_EQ(originalstate, pana.getRaw(), kPanasonicAcBits);
  examplestate[kPanasonicAcStateLength - 1] = 0x83;  // Restore old checksum.

  // Change the state to force a different checksum.
  examplestate[6] = 0x01;  // Should increase checksum by 1.
  EXPECT_FALSE(IRPanasonicAc::validChecksum(examplestate));
  EXPECT_EQ(0x84, IRPanasonicAc::calcChecksum(examplestate));
}

TEST(TestIRPanasonicAcClass, SetAndGetPower) {
  IRPanasonicAc pana(0);
  pana.on();
  EXPECT_TRUE(pana.getPower());
  pana.off();
  EXPECT_FALSE(pana.getPower());
  pana.setPower(true);
  EXPECT_TRUE(pana.getPower());
  pana.setPower(false);
  EXPECT_FALSE(pana.getPower());
}

TEST(TestIRPanasonicAcClass, SetAndGetModel) {
  IRPanasonicAc pana(0);
  EXPECT_EQ(kPanasonicJke, pana.getModel());
  pana.setModel(kPanasonicDke);
  EXPECT_EQ(kPanasonicDke, pana.getModel());
  pana.setModel(kPanasonicLke);
  EXPECT_EQ(kPanasonicLke, pana.getModel());
  pana.setModel(kPanasonicNke);
  EXPECT_EQ(kPanasonicNke, pana.getModel());
  pana.setModel(kPanasonicUnknown);  // shouldn't change.
  EXPECT_EQ(kPanasonicNke, pana.getModel());
  pana.setModel((panasonic_ac_remote_model_t)255);  // shouldn't change.
  EXPECT_EQ(kPanasonicNke, pana.getModel());
  pana.setModel(kPanasonicJke);
  EXPECT_EQ(kPanasonicJke, pana.getModel());

  // This state tickled a bug in getModel(). Should read as a JKE.
  uint8_t jkeState[27] = {0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02,
                          0x20, 0xE0, 0x04, 0x00, 0x32, 0x2E, 0x80, 0xA2, 0x00,
                          0x00, 0x06, 0x60, 0x00, 0x00, 0x80, 0x00, 0x06, 0x74};
  pana.setModel(kPanasonicDke);  // Make sure it isn't somehow set to JKE
  pana.setRaw(jkeState);
  EXPECT_EQ(kPanasonicJke, pana.getModel());
  EXPECT_STATE_EQ(jkeState, pana.getRaw(), kPanasonicAcBits);

  // This state tickled a bug in getModel(). Should read as CKP.
  uint8_t ckpState[27] = {0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02,
                          0x20, 0xE0, 0x04, 0x00, 0x67, 0x2E, 0x80, 0xAF, 0x00,
                          0xC0, 0x6B, 0x98, 0x10, 0x00, 0x81, 0x64, 0x05, 0x87};
  pana.setModel(kPanasonicDke);  // Make sure it isn't somehow set to CKP
  pana.setRaw(ckpState);
  EXPECT_EQ(kPanasonicCkp, pana.getModel());
  EXPECT_STATE_EQ(ckpState, pana.getRaw(), kPanasonicAcBits);
}

TEST(TestIRPanasonicAcClass, SetAndGetMode) {
  IRPanasonicAc pana(0);
  pana.setMode(kPanasonicAcCool);
  pana.setTemp(21);
  EXPECT_EQ(kPanasonicAcCool, pana.getMode());
  pana.setMode(kPanasonicAcHeat);
  EXPECT_EQ(kPanasonicAcHeat, pana.getMode());
  pana.setMode(kPanasonicAcAuto);
  EXPECT_EQ(kPanasonicAcAuto, pana.getMode());
  pana.setMode(kPanasonicAcDry);
  EXPECT_EQ(kPanasonicAcDry, pana.getMode());
  EXPECT_EQ(21, pana.getTemp());  // Temp should be unchanged.
  pana.setMode(kPanasonicAcFan);
  EXPECT_EQ(kPanasonicAcFan, pana.getMode());
  EXPECT_EQ(kPanasonicAcFanModeTemp, pana.getTemp());  // Temp should change.
  pana.setMode(kPanasonicAcCool);
  EXPECT_EQ(kPanasonicAcCool, pana.getMode());
  // Temp should be unchanged from the last manual change.
  EXPECT_EQ(21, pana.getTemp());
}

TEST(TestIRPanasonicAcClass, SetAndGetTemp) {
  IRPanasonicAc pana(0);
  pana.setTemp(25);
  EXPECT_EQ(25, pana.getTemp());
  pana.setTemp(kPanasonicAcMinTemp);
  EXPECT_EQ(kPanasonicAcMinTemp, pana.getTemp());
  pana.setTemp(kPanasonicAcMinTemp - 1);
  EXPECT_EQ(kPanasonicAcMinTemp, pana.getTemp());
  pana.setTemp(kPanasonicAcMaxTemp);
  EXPECT_EQ(kPanasonicAcMaxTemp, pana.getTemp());
  pana.setTemp(kPanasonicAcMaxTemp + 1);
  EXPECT_EQ(kPanasonicAcMaxTemp, pana.getTemp());
}

TEST(TestIRPanasonicAcClass, SetAndGetFan) {
  IRPanasonicAc pana(0);
  pana.setFan(kPanasonicAcFanAuto);
  EXPECT_EQ(kPanasonicAcFanAuto, pana.getFan());
  pana.setFan(kPanasonicAcFanMin);
  EXPECT_EQ(kPanasonicAcFanMin, pana.getFan());
  pana.setFan(kPanasonicAcFanMin - 1);
  EXPECT_EQ(kPanasonicAcFanAuto, pana.getFan());
  pana.setFan(kPanasonicAcFanMed);
  EXPECT_EQ(kPanasonicAcFanMed, pana.getFan());
  pana.setFan(kPanasonicAcFanMin + 1);
  EXPECT_EQ(kPanasonicAcFanAuto, pana.getFan());
  pana.setFan(kPanasonicAcFanMax);
  EXPECT_EQ(kPanasonicAcFanMax, pana.getFan());
  pana.setFan(kPanasonicAcFanMax + 1);
  EXPECT_EQ(kPanasonicAcFanAuto, pana.getFan());
  pana.setFan(kPanasonicAcFanMax - 1);
  EXPECT_EQ(kPanasonicAcFanAuto, pana.getFan());
}

TEST(TestIRPanasonicAcClass, SetAndGetSwings) {
  IRPanasonicAc pana(0);

  // Vertical
  pana.setSwingVertical(kPanasonicAcSwingVAuto);
  EXPECT_EQ(kPanasonicAcSwingVAuto, pana.getSwingVertical());

  pana.setSwingVertical(kPanasonicAcSwingVHighest);
  EXPECT_EQ(kPanasonicAcSwingVHighest, pana.getSwingVertical());
  pana.setSwingVertical(kPanasonicAcSwingVHighest - 1);
  EXPECT_EQ(kPanasonicAcSwingVHighest, pana.getSwingVertical());
  pana.setSwingVertical(kPanasonicAcSwingVHighest + 1);
  EXPECT_EQ(kPanasonicAcSwingVHighest + 1, pana.getSwingVertical());

  pana.setSwingVertical(kPanasonicAcSwingVLowest);
  EXPECT_EQ(kPanasonicAcSwingVLowest, pana.getSwingVertical());
  pana.setSwingVertical(kPanasonicAcSwingVLowest + 1);
  EXPECT_EQ(kPanasonicAcSwingVLowest, pana.getSwingVertical());
  pana.setSwingVertical(kPanasonicAcSwingVLowest - 1);
  EXPECT_EQ(kPanasonicAcSwingVLowest - 1, pana.getSwingVertical());

  pana.setSwingVertical(kPanasonicAcSwingVAuto);
  EXPECT_EQ(kPanasonicAcSwingVAuto, pana.getSwingVertical());

  // Horizontal is model dependant.
  pana.setModel(kPanasonicNke);  // NKE is always fixed in the middle.
  EXPECT_EQ(kPanasonicAcSwingHMiddle, pana.getSwingHorizontal());
  pana.setSwingHorizontal(kPanasonicAcSwingHAuto);
  EXPECT_EQ(kPanasonicAcSwingHMiddle, pana.getSwingHorizontal());

  pana.setModel(kPanasonicJke);  // JKE has no H swing.
  EXPECT_EQ(0, pana.getSwingHorizontal());
  pana.setSwingHorizontal(kPanasonicAcSwingHMiddle);
  EXPECT_EQ(0, pana.getSwingHorizontal());

  pana.setModel(kPanasonicLke);  // LKE is always fixed in the middle.
  EXPECT_EQ(kPanasonicAcSwingHMiddle, pana.getSwingHorizontal());
  pana.setSwingHorizontal(kPanasonicAcSwingHAuto);
  EXPECT_EQ(kPanasonicAcSwingHMiddle, pana.getSwingHorizontal());

  pana.setModel(kPanasonicDke);  // DKE has full control.
  ASSERT_EQ(kPanasonicDke, pana.getModel());
  // Auto was last requested.
  EXPECT_EQ(kPanasonicAcSwingHAuto, pana.getSwingHorizontal());
  pana.setSwingHorizontal(kPanasonicAcSwingHLeft);
  EXPECT_EQ(kPanasonicAcSwingHLeft, pana.getSwingHorizontal());
  // Changing models from DKE to something else, then back should not change
  // the intended swing.
  pana.setModel(kPanasonicLke);
  EXPECT_EQ(kPanasonicAcSwingHMiddle, pana.getSwingHorizontal());
  pana.setModel(kPanasonicDke);
  EXPECT_EQ(kPanasonicAcSwingHLeft, pana.getSwingHorizontal());
}

TEST(TestIRPanasonicAcClass, QuietAndPowerful) {
  IRPanasonicAc pana(0);
  pana.setQuiet(false);
  EXPECT_FALSE(pana.getQuiet());
  pana.setQuiet(true);
  EXPECT_TRUE(pana.getQuiet());
  EXPECT_FALSE(pana.getPowerful());
  pana.setPowerful(false);
  EXPECT_FALSE(pana.getPowerful());
  EXPECT_TRUE(pana.getQuiet());
  pana.setPowerful(true);
  EXPECT_TRUE(pana.getPowerful());
  EXPECT_FALSE(pana.getQuiet());
  pana.setPowerful(false);
  EXPECT_FALSE(pana.getPowerful());
  EXPECT_FALSE(pana.getQuiet());
  pana.setPowerful(true);
  pana.setQuiet(true);
  EXPECT_TRUE(pana.getQuiet());
  EXPECT_FALSE(pana.getPowerful());
}

TEST(TestIRPanasonicAcClass, HumanReadable) {
  IRPanasonicAc pana(0);
  EXPECT_EQ(
      "Model: 4 (JKE), Power: Off, Mode: 0 (Auto), Temp: 0C, "
      "Fan: 253 (UNKNOWN), Swing(V): 0 (UNKNOWN), Quiet: Off, "
      "Powerful: Off, Clock: 00:00, On Timer: Off, Off Timer: Off",
      pana.toString());
  pana.setPower(true);
  pana.setTemp(kPanasonicAcMaxTemp);
  pana.setMode(kPanasonicAcHeat);
  pana.setFan(kPanasonicAcFanMax);
  pana.setSwingVertical(kPanasonicAcSwingVAuto);
  pana.setPowerful(true);
  EXPECT_EQ(
      "Model: 4 (JKE), Power: On, Mode: 4 (Heat), Temp: 30C, "
      "Fan: 4 (High), Swing(V): 15 (Auto), Quiet: Off, "
      "Powerful: On, Clock: 00:00, On Timer: Off, Off Timer: Off",
      pana.toString());
  pana.setQuiet(true);
  pana.setModel(kPanasonicLke);
  EXPECT_EQ(
      "Model: 1 (LKE), Power: Off, Mode: 4 (Heat), Temp: 30C, "
      "Fan: 4 (High), Swing(V): 15 (Auto), "
      "Swing(H): 6 (Middle), Quiet: On, Powerful: Off, "
      "Clock: 00:00, On Timer: 00:00, Off Timer: Off",
      pana.toString());
  pana.setModel(kPanasonicDke);
  pana.setSwingHorizontal(kPanasonicAcSwingHRight);
  EXPECT_EQ(
      "Model: 3 (DKE), Power: Off, Mode: 4 (Heat), Temp: 30C, "
      "Fan: 4 (High), Swing(V): 15 (Auto), "
      "Swing(H): 11 (Right), Quiet: On, Powerful: Off, "
      "Clock: 00:00, On Timer: Off, Off Timer: Off",
      pana.toString());
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
  IRsendTest irsend(0);
  IRrecv irrecv(0);
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

  IRPanasonicAc pana(0);
  pana.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Model: 4 (JKE), Power: Off, Mode: 3 (Cool), Temp: 25C, "
      "Fan: 7 (Auto), Swing(V): 15 (Auto), Quiet: Off, "
      "Powerful: Off, Clock: 00:00, On Timer: Off, Off Timer: Off",
      pana.toString());
}

// Tests for general utility functions.
TEST(TestGeneralPanasonic, hasACState) {
  EXPECT_TRUE(hasACState(PANASONIC_AC));
  ASSERT_FALSE(hasACState(PANASONIC));
}

TEST(TestGeneralPanasonic, typeToString) {
  EXPECT_EQ("PANASONIC_AC", typeToString(PANASONIC_AC));
  EXPECT_EQ("PANASONIC", typeToString(PANASONIC));
}

// Decode a problematic Panasonic AC message
TEST(TestDecodePanasonicAC, Issue540) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
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
  IRPanasonicAc pana(0);
  pana.setRaw(irsend.capture.state);
  // TODO(crankyoldgit): Try to figure out what model this should be.
  EXPECT_EQ(
      "Model: 0 (UNKNOWN), Power: On, Mode: 3 (Cool), Temp: 26C, "
      "Fan: 7 (Auto), Swing(V): 15 (Auto), "
      "Swing(H): 13 (Auto), Quiet: Off, Powerful: Off, "
      "Clock: 00:00, On Timer: Off, Off Timer: Off",
      pana.toString());
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
  IRPanasonicAc pana(0);
  // Data from Issue #544
  uint8_t state[27] = {0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02,
                       0x20, 0xE0, 0x04, 0x00, 0x4E, 0x2E, 0x80, 0xAF, 0x00,
                       0xCA, 0x6B, 0x98, 0x10, 0x00, 0x01, 0x48, 0x04, 0xDB};
  pana.setRaw(state);
  EXPECT_TRUE(pana.isOnTimerEnabled());
  EXPECT_EQ(0x3CA, pana.getOnTimer());
  EXPECT_TRUE(pana.isOffTimerEnabled());
  EXPECT_EQ(0x186, pana.getOffTimer());
  EXPECT_EQ(0x448, pana.getClock());

  pana.cancelOnTimer();
  EXPECT_FALSE(pana.isOnTimerEnabled());
  EXPECT_EQ(0, pana.getOnTimer());
  EXPECT_TRUE(pana.isOffTimerEnabled());
  EXPECT_EQ(0x186, pana.getOffTimer());
  EXPECT_EQ(0x448, pana.getClock());

  pana.cancelOffTimer();
  EXPECT_FALSE(pana.isOnTimerEnabled());
  EXPECT_EQ(0, pana.getOnTimer());
  EXPECT_FALSE(pana.isOffTimerEnabled());
  EXPECT_EQ(0, pana.getOffTimer());
  EXPECT_EQ(0x448, pana.getClock());

  pana.setOnTimer(7 * 60 + 50);
  EXPECT_TRUE(pana.isOnTimerEnabled());
  EXPECT_EQ(7 * 60 + 50, pana.getOnTimer());
  EXPECT_FALSE(pana.isOffTimerEnabled());
  EXPECT_EQ(0, pana.getOffTimer());
  EXPECT_EQ(0x448, pana.getClock());

  pana.setOnTimer(7 * 60 + 57);  // It should round down.
  EXPECT_EQ(7 * 60 + 50, pana.getOnTimer());
  pana.setOnTimer(28 * 60);  // It should round down.
  EXPECT_EQ(kPanasonicAcTimeMax - 9, pana.getOnTimer());
  pana.setOnTimer(kPanasonicAcTimeSpecial);
  EXPECT_EQ(0, pana.getOnTimer());

  pana.setOnTimer(7 * 60 + 50);
  pana.setOffTimer(19 * 60 + 30);

  EXPECT_TRUE(pana.isOnTimerEnabled());
  EXPECT_EQ(7 * 60 + 50, pana.getOnTimer());
  EXPECT_TRUE(pana.isOffTimerEnabled());
  EXPECT_EQ(19 * 60 + 30, pana.getOffTimer());
  EXPECT_EQ(0x448, pana.getClock());

  pana.setOffTimer(19 * 60 + 57);  // It should round down.
  EXPECT_EQ(19 * 60 + 50, pana.getOffTimer());
  pana.setOffTimer(28 * 60);  // It should round down.
  EXPECT_EQ(kPanasonicAcTimeMax - 9, pana.getOffTimer());
  pana.setOffTimer(kPanasonicAcTimeSpecial);
  EXPECT_EQ(0, pana.getOffTimer());

  pana.setClock(0);
  EXPECT_EQ(0, pana.getClock());
  pana.setClock(kPanasonicAcTimeMax);
  EXPECT_EQ(kPanasonicAcTimeMax, pana.getClock());
  pana.setClock(kPanasonicAcTimeMax - 1);
  EXPECT_EQ(kPanasonicAcTimeMax - 1, pana.getClock());
  pana.setClock(kPanasonicAcTimeMax + 1);
  EXPECT_EQ(kPanasonicAcTimeMax, pana.getClock());
  pana.setClock(kPanasonicAcTimeSpecial);
  EXPECT_EQ(0, pana.getClock());
}

// Decode a real short Panasonic AC message
TEST(TestDecodePanasonicAC, RealExampleOfShortMessage) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
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
  IRsendTest irsend(0);
  IRrecv irrecv(0);
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
  IRsendTest irsend(0);
  IRrecv irrecv(0);
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

  IRPanasonicAc pana(0);
  pana.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Model: 5 (CKP), Power: Off, Mode: 4 (Heat), Temp: 23C, "
      "Fan: 7 (Auto), Swing(V): 15 (Auto), Quiet: Off, "
      "Powerful: On, Clock: 00:00, On Timer: 00:00, Off Timer: 00:00",
      pana.toString());

  pana.setQuiet(true);
  EXPECT_FALSE(pana.getPowerful());
  EXPECT_TRUE(pana.getQuiet());
  EXPECT_EQ(kPanasonicCkp, pana.getModel());
  EXPECT_STATE_EQ(ckpQuietOn, pana.getRaw(), kPanasonicAcBits);

  pana.setPowerful(true);
  EXPECT_TRUE(pana.getPowerful());
  EXPECT_FALSE(pana.getQuiet());
  EXPECT_EQ(kPanasonicCkp, pana.getModel());
  EXPECT_STATE_EQ(ckpPowerfulOn, pana.getRaw(), kPanasonicAcBits);
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
