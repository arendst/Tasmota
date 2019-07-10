// Copyright 2017-2018 David Conran

#include "ir_Coolix.h"
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
      "f38000d50"
      "m4480s4480"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s5040"
      "m4480s4480"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s5040",
      irsend.outputStr());

  irsend.reset();
  irsend.sendCOOLIX(0xAA55AA);
  EXPECT_EQ(
      "f38000d50"
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
      "m560s5040",
      irsend.outputStr());

  irsend.reset();
  irsend.sendCOOLIX(0xFFFFFF);
  EXPECT_EQ(
      "f38000d50"
      "m4480s4480"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s5040"
      "m4480s4480"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s5040",
      irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendCoolix, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendCOOLIX(0xAA55AA, kCoolixBits, 1);  // 1 repeat.
  EXPECT_EQ(
      "f38000d50"
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
      "m560s5040",
      irsend.outputStr());
  irsend.sendCOOLIX(0xAA55AA, kCoolixBits, 2);  // 2 repeats.
  EXPECT_EQ(
      "f38000d50"
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
      "m560s5040",
      irsend.outputStr());
}

// Test sending an atypical data size.
TEST(TestSendCoolix, SendUnusualSize) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendCOOLIX(0x0, 8);
  EXPECT_EQ(
      "f38000d50"
      "m4480s4480"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s5040"
      "m4480s4480"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s5040",
      irsend.outputStr());

  irsend.reset();
  irsend.sendCOOLIX(0x1234567890ABCDEF, 64);
  EXPECT_EQ(
      "f38000d50"
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
      "m560s5040"
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
      "m560s5040",
      irsend.outputStr());

  // Bit sizes must be a multiple of 8.
  irsend.reset();
  irsend.sendCOOLIX(0x0, 17);
  EXPECT_EQ("", irsend.outputStr());
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
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, kCoolixBits, true));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(kCoolixBits, irsend.capture.bits);
  EXPECT_EQ(0x123456, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal Coolix 24-bit message.
  irsend.reset();
  irsend.sendCOOLIX(0x0);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, kCoolixBits, true));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(kCoolixBits, irsend.capture.bits);
  EXPECT_EQ(0x0, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal Coolix 24-bit message.
  irsend.reset();
  irsend.sendCOOLIX(0xFFFFFF);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, kCoolixBits, true));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(kCoolixBits, irsend.capture.bits);
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
  irsend.sendCOOLIX(0x123456, kCoolixBits, 2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, kCoolixBits, true));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(kCoolixBits, irsend.capture.bits);
  EXPECT_EQ(0x123456, irsend.capture.value);
  EXPECT_FALSE(irsend.capture.repeat);

  irsend.makeDecodeResult(4 * kCoolixBits + 4);
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, kCoolixBits, true));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(kCoolixBits, irsend.capture.bits);
  EXPECT_EQ(0x123456, irsend.capture.value);

  irsend.makeDecodeResult(2 * (4 * kCoolixBits + 4));
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, kCoolixBits, true));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(kCoolixBits, irsend.capture.bits);
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
  ASSERT_FALSE(irrecv.decodeCOOLIX(&irsend.capture, kCoolixBits, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, 8, false));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(8, irsend.capture.bits);
  EXPECT_EQ(0x12, irsend.capture.value);

  irsend.reset();
  irsend.sendCOOLIX(0x12345678, 32);  // Illegal value Coolix 32-bit message.
  irsend.makeDecodeResult();
  // Shouldn't pass with strict when we ask for less bits than we got.
  ASSERT_FALSE(irrecv.decodeCOOLIX(&irsend.capture, kCoolixBits, true));

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
  irsend.sendCOOLIX(0x123456, kCoolixBits, 2);
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
  uint16_t gc_test[39] = {38000, 1,  1,  322, 162, 20, 61,  20, 61, 20,
                          20,    20, 20, 20,  20,  20, 127, 20, 61, 9,
                          20,    20, 61, 20,  20,  20, 61,  20, 61, 20,
                          61,    20, 20, 20,  20,  20, 20,  20, 884};
  irsend.sendGC(gc_test, 39);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeCOOLIX(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeCOOLIX(&irsend.capture, kCoolixBits, false));
}

// Tests for the IRCoolixAC class.

TEST(TestCoolixACClass, SetAndGetRaw) {
  IRCoolixAC ircoolix(0);

  ircoolix.setRaw(kCoolixOff);
  EXPECT_EQ(kCoolixOff, ircoolix.getRaw());
  ircoolix.setRaw(kCoolixDefaultState);
  EXPECT_EQ(kCoolixDefaultState, ircoolix.getRaw());
}

TEST(TestCoolixACClass, SetAndGetTemp) {
  IRCoolixAC ircoolix(0);

  ircoolix.setTemp(25);
  EXPECT_EQ(25, ircoolix.getTemp());
  ircoolix.setTemp(kCoolixTempMin);
  EXPECT_EQ(kCoolixTempMin, ircoolix.getTemp());
  ircoolix.setTemp(kCoolixTempMax);
  EXPECT_EQ(kCoolixTempMax, ircoolix.getTemp());
  ircoolix.setTemp(kCoolixTempMin - 1);
  EXPECT_EQ(kCoolixTempMin, ircoolix.getTemp());
  ircoolix.setTemp(kCoolixTempMax + 1);
  EXPECT_EQ(kCoolixTempMax, ircoolix.getTemp());
}

TEST(TestCoolixACClass, SetAndGetMode) {
  IRCoolixAC ircoolix(0);

  ircoolix.setMode(kCoolixHeat);
  EXPECT_EQ(kCoolixHeat, ircoolix.getMode());
  ircoolix.setMode(kCoolixCool);
  EXPECT_EQ(kCoolixCool, ircoolix.getMode());
  ircoolix.setMode(kCoolixDry);
  EXPECT_EQ(kCoolixDry, ircoolix.getMode());
  ircoolix.setMode(kCoolixAuto);
  EXPECT_EQ(kCoolixAuto, ircoolix.getMode());
  ircoolix.setMode(kCoolixFan);
  EXPECT_EQ(kCoolixFan, ircoolix.getMode());
}

TEST(TestCoolixACClass, SetAndGetFan) {
  IRCoolixAC ircoolix(0);

  ircoolix.setFan(kCoolixFanMax);
  EXPECT_EQ(kCoolixFanMax, ircoolix.getFan());
  ircoolix.setFan(kCoolixFanMin);
  EXPECT_EQ(kCoolixFanMin, ircoolix.getFan());
  ircoolix.setFan(kCoolixFanZoneFollow);
  EXPECT_EQ(kCoolixFanZoneFollow, ircoolix.getFan());
  ircoolix.setFan(kCoolixFanAuto);
  EXPECT_EQ(kCoolixFanAuto, ircoolix.getFan());
  ircoolix.setFan(kCoolixFanMax);
  EXPECT_EQ(kCoolixFanMax, ircoolix.getFan());
  ASSERT_NE(3, kCoolixFanAuto);
  // Now try some unexpected value.
  ircoolix.setFan(3);
  EXPECT_EQ(kCoolixFanAuto, ircoolix.getFan());
}

TEST(TestCoolixACClass, SetGetClearSensorTempAndZoneFollow) {
  IRCoolixAC ircoolix(0);

  ircoolix.setRaw(kCoolixDefaultState);
  EXPECT_FALSE(ircoolix.getZoneFollow());
  EXPECT_LT(kCoolixSensorTempMax, ircoolix.getSensorTemp());

  ircoolix.setSensorTemp(25);
  EXPECT_TRUE(ircoolix.getZoneFollow());
  EXPECT_EQ(25, ircoolix.getSensorTemp());

  // Lower bounds
  ircoolix.setSensorTemp(kCoolixSensorTempMin);
  EXPECT_TRUE(ircoolix.getZoneFollow());
  EXPECT_EQ(kCoolixSensorTempMin, ircoolix.getSensorTemp());
  ircoolix.setSensorTemp(kCoolixSensorTempMin - 1);
  EXPECT_TRUE(ircoolix.getZoneFollow());
  EXPECT_EQ(kCoolixSensorTempMin, ircoolix.getSensorTemp());
  // Upper bounds
  ircoolix.setSensorTemp(kCoolixSensorTempMax);
  EXPECT_TRUE(ircoolix.getZoneFollow());
  EXPECT_EQ(kCoolixSensorTempMax, ircoolix.getSensorTemp());
  ircoolix.setSensorTemp(kCoolixSensorTempMax + 1);
  EXPECT_TRUE(ircoolix.getZoneFollow());
  EXPECT_EQ(kCoolixSensorTempMax, ircoolix.getSensorTemp());
  // Clearing
  ircoolix.clearSensorTemp();
  EXPECT_FALSE(ircoolix.getZoneFollow());
  EXPECT_LT(kCoolixSensorTempMax, ircoolix.getSensorTemp());
}

TEST(TestCoolixACClass, SpecialModesAndReset) {
  IRCoolixAC ircoolix(0);
  ASSERT_NE(kCoolixSwing, ircoolix.getRaw());
  ircoolix.setSwing();
  ASSERT_EQ(kCoolixSwing, ircoolix.getRaw());
  ircoolix.setTurbo();
  ASSERT_EQ(kCoolixTurbo, ircoolix.getRaw());
  ircoolix.setSleep();
  ASSERT_EQ(kCoolixSleep, ircoolix.getRaw());
  ircoolix.setLed();
  ASSERT_EQ(kCoolixLed, ircoolix.getRaw());
  ircoolix.setClean();
  ASSERT_EQ(kCoolixClean, ircoolix.getRaw());
  ircoolix.stateReset();
  ASSERT_NE(kCoolixClean, ircoolix.getRaw());
}

TEST(TestCoolixACClass, HumanReadable) {
  IRCoolixAC ircoolix(0);

  // Initial starting point.
  EXPECT_EQ(
      "Power: On, Mode: 2 (AUTO), Fan: 5 (AUTO), Temp: 25C, "
      "Zone Follow: Off, Sensor Temp: Ignored",
      ircoolix.toString());

  ircoolix.setSensorTemp(24);
  ircoolix.setTemp(22);
  ircoolix.setMode(kCoolixCool);
  ircoolix.setFan(kCoolixFanMin);
  EXPECT_EQ(
      "Power: On, Mode: 0 (COOL), Fan: 4 (MIN), Temp: 22C, "
      "Zone Follow: On, Sensor Temp: 24C",
      ircoolix.toString());
  ircoolix.setSwing();
  EXPECT_EQ("Power: On, Swing: Toggle", ircoolix.toString());
  ircoolix.setPower(false);
  EXPECT_EQ("Power: Off", ircoolix.toString());
}

TEST(TestCoolixACClass, KnownExamples) {
  IRCoolixAC ircoolix(0);

  ircoolix.setRaw(0b101100101011111111100100);
  EXPECT_EQ(
      "Power: On, Mode: 4 (FAN), Fan: 5 (AUTO), Zone Follow: Off, "
      "Sensor Temp: Ignored",
      ircoolix.toString());
  ircoolix.setRaw(0b101100101001111100000000);
  EXPECT_EQ(
      "Power: On, Mode: 0 (COOL), Fan: 4 (MIN), Temp: 17C, "
      "Zone Follow: Off, Sensor Temp: Ignored",
      ircoolix.toString());
}

TEST(TestCoolixACClass, Issue579FanAuto0) {
  IRCoolixAC ircoolix(0);

  ircoolix.setRaw(0xB21F28);
  EXPECT_EQ(
      "Power: On, Mode: 2 (AUTO), Fan: 0 (AUTO0), Temp: 20C, "
      "Zone Follow: Off, Sensor Temp: Ignored",
      ircoolix.toString());
}

TEST(TestCoolixACClass, RealCaptureExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);

  // From Issue #579
  uint16_t powerOffRawData[199] = {
      4444, 4434, 590, 1578, 698,  446,  590, 1578, 622, 1596, 622, 500,
      644,  476,  644, 1548, 588,  532,  594, 530,  612, 1578, 590, 532,
      588,  534,  672, 1518, 594,  1598, 590, 510,  612, 1580, 644, 480,
      612,  1578, 644, 1548, 644,  1548, 594, 1598, 642, 506,  644, 1550,
      644,  1548, 594, 1600, 644,  478,  644, 478,  642, 480,  644, 478,
      642,  1548, 594, 530,  590,  532,  614, 1578, 644, 1548, 594, 1600,
      588,  534,  566, 556,  588,  530,  590, 532,  586, 514,  612, 532,
      588,  532,  590, 534,  588,  1578, 642, 1576, 642, 1550, 588, 1602,
      588,  1580, 642, 4712, 4546, 4406, 588, 1606, 642, 478,  644, 1550,
      590,  1604, 588, 534,  586,  532,  586, 1582, 642, 480,  642, 480,
      668,  1550, 642, 480,  642,  478,  642, 1552, 612, 1578, 586, 538,
      588,  1580, 674, 472,  590,  1602, 586, 1580, 618, 1576, 642, 1548,
      594,  530,  590, 1584, 608,  1578, 644, 1550, 642, 480,  642, 478,
      642,  480,  642, 480,  642,  1550, 590, 530,  592, 528,  592, 1602,
      642,  1548, 592, 1604, 586,  584,  642, 480,  640, 480,  640, 480,
      642,  480,  642, 480,  642,  480,  642, 480,  642, 1552, 590, 1604,
      588,  1578, 642, 1552, 640,  1550, 592};  // COOLIX B27BE0

  irsend.begin();

  irsend.reset();

  irsend.sendRaw(powerOffRawData, 199, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(kCoolixBits, irsend.capture.bits);
  EXPECT_EQ(kCoolixOff, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}


// Tests to debug/fix:
//   https://github.com/markszabo/IRremoteESP8266/issues/624
TEST(TestCoolixACClass, Issue624HandleSpecialStatesBetter) {
  IRCoolixAC ac(0);
  ac.begin();
  // Default
  EXPECT_EQ(
      "Power: On, Mode: 2 (AUTO), Fan: 5 (AUTO), Temp: 25C, Zone Follow: Off, "
      "Sensor Temp: Ignored",
      ac.toString());
  EXPECT_EQ(0xB2BFC8, ac.getRaw());
  // Change of settings.
  ac.setPower(true);
  ac.setTemp(24);
  ac.setMode(kCoolixCool);
  ac.setFan(kCoolixFanAuto);
  EXPECT_EQ(
      "Power: On, Mode: 0 (COOL), Fan: 5 (AUTO), Temp: 24C, Zone Follow: Off, "
      "Sensor Temp: Ignored",
      ac.toString());
  EXPECT_EQ(0xB2BF40, ac.getRaw());
  // Turn the unit off.
  ac.setPower(false);
  EXPECT_EQ(
      "Power: Off",
      ac.toString());
  EXPECT_EQ(kCoolixOff, ac.getRaw());
  // Repeat change of settings.
  ac.setPower(true);
  ac.setTemp(24);
  ac.setMode(kCoolixCool);
  ac.setFan(kCoolixFanAuto);
  EXPECT_EQ(
      "Power: On, Mode: 0 (COOL), Fan: 5 (AUTO), Temp: 24C, Zone Follow: Off, "
      "Sensor Temp: Ignored",
      ac.toString());
  EXPECT_EQ(0xB2BF40, ac.getRaw());

  // Now test if we setRaw() a special state first.
  ac.setRaw(kCoolixSwing);
  // Repeat change of settings.
  ac.setPower(true);
  ac.setTemp(24);
  ac.setMode(kCoolixCool);
  ac.setFan(kCoolixFanAuto);
  EXPECT_EQ(
      "Power: On, Mode: 0 (COOL), Fan: 5 (AUTO), Temp: 24C, Zone Follow: Off, "
      "Sensor Temp: Ignored",
      ac.toString());
  EXPECT_EQ(0xB2BF40, ac.getRaw());
}
