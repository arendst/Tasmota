// Copyright 2017 David Conran

#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendCOOLIX().

// Test sending typical data only.
TEST(TestSendCoolix, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendCOOLIX(0x0);
  EXPECT_EQ(
      "m4480s4480"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s5040", irsend.outputStr());

  irsend.reset();
  irsend.sendCOOLIX(0xAA55AA);
  EXPECT_EQ(
      "m4480s4480"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s5040", irsend.outputStr());

  irsend.reset();
  irsend.sendCOOLIX(0xFFFFFF);
  EXPECT_EQ(
      "m4480s4480"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s5040", irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendCoolix, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendCOOLIX(0xAA55AA, COOLIX_BITS, 1);  // 1 repeat.
  EXPECT_EQ(
      "m4480s4480"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s5040"
      "m4480s4480"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s5040", irsend.outputStr());
  irsend.sendCOOLIX(0xAA55AA, COOLIX_BITS, 2);  // 2 repeats.
  EXPECT_EQ(
      "m4480s4480"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s5040"
      "m4480s4480"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s5040"
      "m4480s4480"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s5040", irsend.outputStr());
}

// Test sending an atypical data size.
TEST(TestSendCoolix, SendUsualSize) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendCOOLIX(0x0, 8);
  EXPECT_EQ(
      "m4480s4480"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s5040", irsend.outputStr());

  irsend.reset();
  irsend.sendCOOLIX(0x1234567890ABCDEF, 64);
  EXPECT_EQ(
      "m4480s4480"
      "m560s560m560s560m560s560m560s1680m560s560m560s560m560s1680m560s560"
      "m560s1680m560s1680m560s1680m560s560m560s1680m560s1680m560s560m560s1680"
      "m560s560m560s560m560s1680m560s1680m560s560m560s1680m560s560m560s560"
      "m560s1680m560s1680m560s560m560s560m560s1680m560s560m560s1680m560s1680"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s1680m560s560"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s560m560s1680"
      "m560s560m560s1680m560s1680m560s1680m560s1680m560s560m560s560m560s560"
      "m560s1680m560s560m560s560m560s560m560s560m560s1680m560s1680m560s1680"
      "m560s1680m560s560m560s560m560s1680m560s560m560s560m560s560m560s560"
      "m560s560m560s1680m560s1680m560s560m560s1680m560s1680m560s1680m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s1680"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s560"
      "m560s1680m560s1680m560s560m560s560m560s1680m560s1680m560s560m560s1680"
      "m560s560m560s560m560s1680m560s1680m560s560m560s560m560s1680m560s560"
      "m560s1680m560s1680m560s1680m560s560m560s1680m560s1680m560s1680m560s1680"
      "m560s560m560s560m560s560m560s1680m560s560m560s560m560s560m560s560"
      "m560s5040", irsend.outputStr());

  // Bit sizes must be a multiple of 8.
  irsend.reset();
  irsend.sendCOOLIX(0x0, 17);
  EXPECT_EQ("" , irsend.outputStr());
}

// Tests for decodeCOOLIX().

// Decode normal Coolix messages.
TEST(TestDecodeCoolix, NormalDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Coolix 24-bit message.
  irsend.reset();
  irsend.sendCOOLIX(0x123456);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, COOLIX_BITS, true));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(COOLIX_BITS, irsend.capture.bits);
  EXPECT_EQ(0x123456, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal Coolix 24-bit message.
  irsend.reset();
  irsend.sendCOOLIX(0x0);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, COOLIX_BITS, true));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(COOLIX_BITS, irsend.capture.bits);
  EXPECT_EQ(0x0, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal Coolix 24-bit message.
  irsend.reset();
  irsend.sendCOOLIX(0xFFFFFF);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, COOLIX_BITS, true));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(COOLIX_BITS, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFF, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode normal repeated Coolix messages.
TEST(TestDecodeCoolix, NormalDecodeWithRepeatAndStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Coolix 16-bit message with 2 repeats.
  irsend.reset();
  irsend.sendCOOLIX(0x123456, COOLIX_BITS, 2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, COOLIX_BITS, true));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(COOLIX_BITS, irsend.capture.bits);
  EXPECT_EQ(0x123456, irsend.capture.value);
  EXPECT_FALSE(irsend.capture.repeat);

  irsend.makeDecodeResult(4 * COOLIX_BITS + 4);
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, COOLIX_BITS, true));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(COOLIX_BITS, irsend.capture.bits);
  EXPECT_EQ(0x123456, irsend.capture.value);

  irsend.makeDecodeResult(2 * (4 * COOLIX_BITS + 4));
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, COOLIX_BITS, true));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(COOLIX_BITS, irsend.capture.bits);
  EXPECT_EQ(0x123456, irsend.capture.value);
}

// Decode unsupported Coolix messages.
TEST(TestDecodeCoolix, DecodeWithNonStrictSizes) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  irsend.sendCOOLIX(0x12, 8);  // Illegal value Coolix 8-bit message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeCOOLIX(&irsend.capture, COOLIX_BITS, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, 8, false));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(8, irsend.capture.bits);
  EXPECT_EQ(0x12, irsend.capture.value);

  irsend.reset();
  irsend.sendCOOLIX(0x12345678, 32);  // Illegal value Coolix 32-bit message.
  irsend.makeDecodeResult();
  // Shouldn't pass with strict when we ask for less bits than we got.
  ASSERT_FALSE(irrecv.decodeCOOLIX(&irsend.capture, COOLIX_BITS, true));

  irsend.makeDecodeResult();
  // Should fail with strict when we ask for the wrong bit size.
  ASSERT_FALSE(irrecv.decodeCOOLIX(&irsend.capture, 32, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, 32, false));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(32, irsend.capture.bits);
  EXPECT_EQ(0x12345678, irsend.capture.value);

  // Decode should fail if asked to decode non-multiples of 8 bits.
  irsend.reset();
  irsend.sendCOOLIX(0x123456, COOLIX_BITS, 2);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeCOOLIX(&irsend.capture, 9, false));
}

// Decode (non-standard) 64-bit messages.
TEST(TestDecodeCoolix, Decode64BitMessages) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Illegal size Coolix 64-bit message.
  irsend.sendCOOLIX(0xFFFFFFFFFFFFFFFF, 64);
  irsend.makeDecodeResult();
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, 64, false));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, irsend.capture.value);
}

// Fail to decode a non-Coolix example via GlobalCache
TEST(TestDecodeCoolix, FailToDecodeNonCoolixExample) {
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

  ASSERT_FALSE(irrecv.decodeCOOLIX(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeCOOLIX(&irsend.capture, COOLIX_BITS, false));
}
