// Copyright 2017 David Conran

#include "IRsend.h"
#include "IRsend_test.h"
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
      "m3456s1728"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s129600", irsend.outputStr());

  irsend.reset();
  irsend.sendPanasonic64(0x40040190ED7C);
  EXPECT_EQ(
      "m3456s1728"
      "m432s432m432s1296m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s1296"
      "m432s1296m432s432m432s432m432s1296m432s432m432s432m432s432m432s432"
      "m432s1296m432s1296m432s1296m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s432m432s1296m432s1296m432s1296m432s1296m432s1296m432s432m432s432"
      "m432s129600", irsend.outputStr());

  irsend.reset();
  irsend.sendPanasonic64(0xFFFFFFFFFFFF);
  EXPECT_EQ(
      "m3456s1728"
      "m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296"
      "m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296"
      "m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296"
      "m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296"
      "m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296"
      "m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296m432s1296"
      "m432s129600", irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendPanasonic64, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendPanasonic64(0x40040190ED7C, PANASONIC_BITS, 0);  // 0 repeats.
  EXPECT_EQ(
      "m3456s1728"
      "m432s432m432s1296m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s1296"
      "m432s1296m432s432m432s432m432s1296m432s432m432s432m432s432m432s432"
      "m432s1296m432s1296m432s1296m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s432m432s1296m432s1296m432s1296m432s1296m432s1296m432s432m432s432"
      "m432s129600", irsend.outputStr());

  irsend.reset();
  irsend.sendPanasonic64(0x40040190ED7C, PANASONIC_BITS, 1);  // 1 repeat.
  EXPECT_EQ(
      "m3456s1728"
      "m432s432m432s1296m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s1296"
      "m432s1296m432s432m432s432m432s1296m432s432m432s432m432s432m432s432"
      "m432s1296m432s1296m432s1296m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s432m432s1296m432s1296m432s1296m432s1296m432s1296m432s432m432s432"
      "m432s129600"
      "m3456s1728"
      "m432s432m432s1296m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s1296"
      "m432s1296m432s432m432s432m432s1296m432s432m432s432m432s432m432s432"
      "m432s1296m432s1296m432s1296m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s432m432s1296m432s1296m432s1296m432s1296m432s1296m432s432m432s432"
      "m432s129600", irsend.outputStr());

  irsend.sendPanasonic64(0x40040190ED7C, PANASONIC_BITS, 2);  // 2 repeats.
  EXPECT_EQ(
      "m3456s1728"
      "m432s432m432s1296m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s1296"
      "m432s1296m432s432m432s432m432s1296m432s432m432s432m432s432m432s432"
      "m432s1296m432s1296m432s1296m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s432m432s1296m432s1296m432s1296m432s1296m432s1296m432s432m432s432"
      "m432s129600"
      "m3456s1728"
      "m432s432m432s1296m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s1296"
      "m432s1296m432s432m432s432m432s1296m432s432m432s432m432s432m432s432"
      "m432s1296m432s1296m432s1296m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s432m432s1296m432s1296m432s1296m432s1296m432s1296m432s432m432s432"
      "m432s129600"
      "m3456s1728"
      "m432s432m432s1296m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s1296m432s432m432s432"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s1296"
      "m432s1296m432s432m432s432m432s1296m432s432m432s432m432s432m432s432"
      "m432s1296m432s1296m432s1296m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s432m432s1296m432s1296m432s1296m432s1296m432s1296m432s432m432s432"
      "m432s129600", irsend.outputStr());
}

// Test sending an atypical data size.
TEST(TestSendPanasonic64, SendUnusualSize) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendPanasonic64(0x0, 8);
  EXPECT_EQ(
      "m3456s1728"
      "m432s432m432s432m432s432m432s432m432s432m432s432m432s432m432s432"
      "m432s129600", irsend.outputStr());

  irsend.reset();
  irsend.sendPanasonic64(0x1234567890ABCDEF, 64);
  EXPECT_EQ(
      "m3456s1728"
      "m432s432m432s432m432s432m432s1296m432s432m432s432m432s1296m432s432"
      "m432s432m432s432m432s1296m432s1296m432s432m432s1296m432s432m432s432"
      "m432s432m432s1296m432s432m432s1296m432s432m432s1296m432s1296m432s432"
      "m432s432m432s1296m432s1296m432s1296m432s1296m432s432m432s432m432s432"
      "m432s1296m432s432m432s432m432s1296m432s432m432s432m432s432m432s432"
      "m432s1296m432s432m432s1296m432s432m432s1296m432s432m432s1296m432s1296"
      "m432s1296m432s1296m432s432m432s432m432s1296m432s1296m432s432m432s1296"
      "m432s1296m432s1296m432s1296m432s432m432s1296m432s1296m432s1296m432s1296"
      "m432s129600", irsend.outputStr());
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

  panasonic.sendPanasonic(0x1234, 0x567890AB, PANASONIC_BITS, 2);
  panasonic64.sendPanasonic64(0x1234567890AB, PANASONIC_BITS, 2);
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
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, PANASONIC_BITS, true));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(PANASONIC_BITS, irsend.capture.bits);
  EXPECT_EQ(0x40040190ED7C, irsend.capture.value);
  EXPECT_EQ(0x4004, irsend.capture.address);
  EXPECT_EQ(0x0190ED7C, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Synthesised Normal Panasonic 48-bit message.
  irsend.reset();
  irsend.sendPanasonic64(irsend.encodePanasonic(0x4004, 0x12, 0x34, 0x56));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, PANASONIC_BITS, true));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(PANASONIC_BITS, irsend.capture.bits);
  EXPECT_EQ(0x400412345670, irsend.capture.value);
  EXPECT_EQ(0x4004, irsend.capture.address);
  EXPECT_EQ(0x12345670, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Synthesised Normal Panasonic 48-bit message.
  irsend.reset();
  irsend.sendPanasonic64(irsend.encodePanasonic(0x4004, 0x1, 0x1, 0x1));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, PANASONIC_BITS, true));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(PANASONIC_BITS, irsend.capture.bits);
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
  irsend.sendPanasonic64(0x40040190ED7C, PANASONIC_BITS, 2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, PANASONIC_BITS, true));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(PANASONIC_BITS, irsend.capture.bits);
  EXPECT_EQ(0x40040190ED7C, irsend.capture.value);
  EXPECT_EQ(0x4004, irsend.capture.address);
  EXPECT_EQ(0x190ED7C, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  irsend.makeDecodeResult(2 * PANASONIC_BITS + 4);
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, PANASONIC_BITS, true));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(PANASONIC_BITS, irsend.capture.bits);
  EXPECT_EQ(0x40040190ED7C, irsend.capture.value);

  irsend.makeDecodeResult(2 * (2 * PANASONIC_BITS + 4));
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, PANASONIC_BITS, true));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(PANASONIC_BITS, irsend.capture.bits);
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
  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, PANASONIC_BITS, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, PANASONIC_BITS, false));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(PANASONIC_BITS, irsend.capture.bits);
  EXPECT_EQ(0x0, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  // Illegal address/Manufacturer code. The rest is legal.
  irsend.sendPanasonic64(irsend.encodePanasonic(0, 1, 2, 3));
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, PANASONIC_BITS, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, PANASONIC_BITS, false));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(PANASONIC_BITS, irsend.capture.bits);
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

  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, PANASONIC_BITS, true));

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

  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, PANASONIC_BITS, true));
  // Shouldn't pass if strict off and wrong bit size.
  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, PANASONIC_BITS, false));
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
  uint16_t gc_test[103] = {37000, 1, 1, 126, 64, 16, 17, 16, 49, 15, 16, 16, 16,
                          16, 16, 16, 17, 15, 17, 15, 17, 15, 17, 15, 16, 16,
                          16, 16, 16, 16, 17, 15, 49, 16, 16, 16, 16, 16, 17,
                          15, 17, 15, 17, 15, 17, 15, 16, 16, 16, 16, 16, 16,
                          49, 15, 49, 16, 17, 15, 17, 15, 49, 16, 16, 16, 17,
                          16, 17, 15, 17, 15, 49, 16, 49, 15, 49, 16, 17, 16,
                          49, 15, 49, 16, 17, 15, 48, 16, 16, 16, 49, 15, 48,
                          16, 49, 15, 49, 16, 49, 15, 17, 15, 16, 16, 2721};
  irsend.sendGC(gc_test, 103);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, PANASONIC_BITS, true));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(PANASONIC_BITS, irsend.capture.bits);
  EXPECT_EQ(0x40040190ED7C, irsend.capture.value);
  EXPECT_EQ(0x4004, irsend.capture.address);
  EXPECT_EQ(0x0190ED7C, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(PANASONIC_BITS, irsend.capture.bits);
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
  uint16_t gc_test[39] = {38000, 1, 1, 322, 162, 20, 61, 20, 61, 20, 20, 20, 20,
                          20, 20, 20, 127, 20, 61, 9, 20, 20, 61, 20, 20, 20,
                          61, 20, 61, 20, 61, 20, 20, 20, 20, 20, 20, 20, 884};
  irsend.sendGC(gc_test, 39);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture));
  ASSERT_FALSE(irrecv.decodePanasonic(&irsend.capture, PANASONIC_BITS, false));
}

// Failing to decode Panasonic in Issue #245
TEST(TestDecodePanasonic, DecodeIssue245) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();

  uint16_t rawData[100] = {3550, 1750, 500, 450, 500, 1300, 500, 450, 500, 450,
                          500, 450, 500, 450, 500, 450, 500, 450, 500, 450,
                          500, 450, 500, 450, 500, 450, 500, 450, 500, 1300,
                          500, 450, 500, 450, 500, 450, 500, 450, 500, 450,
                          500, 450, 500, 450, 500, 450, 500, 450, 500, 1300,
                          500, 450, 500, 450, 500, 450, 500, 450, 500, 450,
                          500, 450, 500, 450, 500, 450, 500, 1300, 500, 450,
                          500, 1300, 500, 1300, 500, 1300, 500, 1300, 500, 450,
                          500, 450, 500, 1300, 500, 450, 500, 1300, 500, 1300,
                          500, 1300, 500, 1300, 500, 450, 500, 1300, 500, 5000};

  irsend.sendRaw(rawData, 100, 37);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(PANASONIC_BITS, irsend.capture.bits);
  EXPECT_EQ(0x40040100BCBD, irsend.capture.value);
  EXPECT_EQ(0x4004, irsend.capture.address);
  EXPECT_EQ(0x100BCBD, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  irsend.reset();
  irsend.sendRaw(rawData, 99, 37);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(PANASONIC_BITS, irsend.capture.bits);
  EXPECT_EQ(0x40040100BCBD, irsend.capture.value);
  EXPECT_EQ(0x4004, irsend.capture.address);
  EXPECT_EQ(0x100BCBD, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}
