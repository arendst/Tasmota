// Copyright 2017-2018 David Conran

#include "ir_Coolix.h"
#include "IRac.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"


TEST(TestUtils, Housekeeping) {
  // COOLIX
  ASSERT_EQ("COOLIX", typeToString(decode_type_t::COOLIX));
  ASSERT_EQ(decode_type_t::COOLIX, strToDecodeType("COOLIX"));
  ASSERT_FALSE(hasACState(decode_type_t::COOLIX));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::COOLIX));
  ASSERT_EQ(kCoolixBits, IRsend::defaultBits(decode_type_t::COOLIX));
  ASSERT_EQ(kSingleRepeat, IRsend::minRepeats(decode_type_t::COOLIX));

  // COOLIX48
  ASSERT_EQ("COOLIX48", typeToString(decode_type_t::COOLIX48));
  ASSERT_EQ(decode_type_t::COOLIX48, strToDecodeType("COOLIX48"));
  ASSERT_FALSE(hasACState(decode_type_t::COOLIX48));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::COOLIX48));
  ASSERT_EQ(kCoolix48Bits, IRsend::defaultBits(decode_type_t::COOLIX48));
  ASSERT_EQ(kSingleRepeat, IRsend::minRepeats(decode_type_t::COOLIX48));
}

// Tests for sendCOOLIX().

// Test sending typical data only.
TEST(TestSendCoolix, SendDataOnly) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendCOOLIX(0x0);
  EXPECT_EQ(
      "f38000d50"
      "m4692s4416"
      "m552s552m552s552m552s552m552s552m552s552m552s552m552s552m552s552"
      "m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s552m552s552m552s552m552s552m552s552m552s552m552s552m552s552"
      "m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s552m552s552m552s552m552s552m552s552m552s552m552s552m552s552"
      "m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s5244"
      "m4692s4416"
      "m552s552m552s552m552s552m552s552m552s552m552s552m552s552m552s552"
      "m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s552m552s552m552s552m552s552m552s552m552s552m552s552m552s552"
      "m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s552m552s552m552s552m552s552m552s552m552s552m552s552m552s552"
      "m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s105244",
      irsend.outputStr());

  irsend.reset();
  irsend.sendCOOLIX(0xAA55AA);
  EXPECT_EQ(
      "f38000d50"
      "m4692s4416"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s5244"
      "m4692s4416"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s105244",
      irsend.outputStr());

  irsend.reset();
  irsend.sendCOOLIX(0xFFFFFF);
  EXPECT_EQ(
      "f38000d50"
      "m4692s4416"
      "m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s552m552s552m552s552m552s552m552s552m552s552m552s552m552s552"
      "m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s552m552s552m552s552m552s552m552s552m552s552m552s552m552s552"
      "m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s552m552s552m552s552m552s552m552s552m552s552m552s552m552s552"
      "m552s5244"
      "m4692s4416"
      "m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s552m552s552m552s552m552s552m552s552m552s552m552s552m552s552"
      "m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s552m552s552m552s552m552s552m552s552m552s552m552s552m552s552"
      "m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s552m552s552m552s552m552s552m552s552m552s552m552s552m552s552"
      "m552s105244",
      irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendCoolix, SendWithRepeats) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendCOOLIX(0xAA55AA, kCoolixBits, 1);  // 1 repeat.
  EXPECT_EQ(
      "f38000d50"
      "m4692s4416"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s5244"
      "m4692s4416"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s105244",
      irsend.outputStr());
  irsend.sendCOOLIX(0xAA55AA, kCoolixBits, 2);  // 2 repeats.
  EXPECT_EQ(
      "f38000d50"
      "m4692s4416"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s5244"
      "m4692s4416"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s5244"
      "m4692s4416"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s105244",
      irsend.outputStr());
}

// Test sending an atypical data size.
TEST(TestSendCoolix, SendUnusualSize) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendCOOLIX(0x0, 8);
  EXPECT_EQ(
      "f38000d50"
      "m4692s4416"
      "m552s552m552s552m552s552m552s552m552s552m552s552m552s552m552s552"
      "m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s5244"
      "m4692s4416"
      "m552s552m552s552m552s552m552s552m552s552m552s552m552s552m552s552"
      "m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s105244",
      irsend.outputStr());

  irsend.reset();
  irsend.sendCOOLIX(0x1234567890ABCDEF, 64);
  EXPECT_EQ(
      "f38000d50"
      "m4692s4416"
      "m552s552m552s552m552s552m552s1656m552s552m552s552m552s1656m552s552"
      "m552s1656m552s1656m552s1656m552s552m552s1656m552s1656m552s552m552s1656"
      "m552s552m552s552m552s1656m552s1656m552s552m552s1656m552s552m552s552"
      "m552s1656m552s1656m552s552m552s552m552s1656m552s552m552s1656m552s1656"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s1656m552s552"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s552m552s1656"
      "m552s552m552s1656m552s1656m552s1656m552s1656m552s552m552s552m552s552"
      "m552s1656m552s552m552s552m552s552m552s552m552s1656m552s1656m552s1656"
      "m552s1656m552s552m552s552m552s1656m552s552m552s552m552s552m552s552"
      "m552s552m552s1656m552s1656m552s552m552s1656m552s1656m552s1656m552s1656"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s1656"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s552"
      "m552s1656m552s1656m552s552m552s552m552s1656m552s1656m552s552m552s1656"
      "m552s552m552s552m552s1656m552s1656m552s552m552s552m552s1656m552s552"
      "m552s1656m552s1656m552s1656m552s552m552s1656m552s1656m552s1656m552s1656"
      "m552s552m552s552m552s552m552s1656m552s552m552s552m552s552m552s552"
      "m552s5244"
      "m4692s4416"
      "m552s552m552s552m552s552m552s1656m552s552m552s552m552s1656m552s552"
      "m552s1656m552s1656m552s1656m552s552m552s1656m552s1656m552s552m552s1656"
      "m552s552m552s552m552s1656m552s1656m552s552m552s1656m552s552m552s552"
      "m552s1656m552s1656m552s552m552s552m552s1656m552s552m552s1656m552s1656"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s1656m552s552"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s552m552s1656"
      "m552s552m552s1656m552s1656m552s1656m552s1656m552s552m552s552m552s552"
      "m552s1656m552s552m552s552m552s552m552s552m552s1656m552s1656m552s1656"
      "m552s1656m552s552m552s552m552s1656m552s552m552s552m552s552m552s552"
      "m552s552m552s1656m552s1656m552s552m552s1656m552s1656m552s1656m552s1656"
      "m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s1656"
      "m552s552m552s1656m552s552m552s1656m552s552m552s1656m552s552m552s552"
      "m552s1656m552s1656m552s552m552s552m552s1656m552s1656m552s552m552s1656"
      "m552s552m552s552m552s1656m552s1656m552s552m552s552m552s1656m552s552"
      "m552s1656m552s1656m552s1656m552s552m552s1656m552s1656m552s1656m552s1656"
      "m552s552m552s552m552s552m552s1656m552s552m552s552m552s552m552s552"
      "m552s105244",
      irsend.outputStr());

  // Bit sizes must be a multiple of 8.
  irsend.reset();
  irsend.sendCOOLIX(0x0, 17);
  EXPECT_EQ("", irsend.outputStr());
}

// Tests for decodeCOOLIX().

// Decode normal Coolix messages.
TEST(TestDecodeCoolix, NormalDecodeWithStrict) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Normal Coolix 24-bit message.
  irsend.reset();
  irsend.sendCOOLIX(0x123456);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, kStartOffset, kCoolixBits,
              true));
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
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, kStartOffset, kCoolixBits,
                                  true));
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
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, kStartOffset, kCoolixBits,
                                  true));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(kCoolixBits, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFF, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode normal repeated Coolix messages.
TEST(TestDecodeCoolix, NormalDecodeWithRepeatAndStrict) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Normal Coolix 16-bit message with 2 repeats.
  irsend.reset();
  irsend.sendCOOLIX(0x123456, kCoolixBits, 2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, kStartOffset, kCoolixBits,
                                  true));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(kCoolixBits, irsend.capture.bits);
  EXPECT_EQ(0x123456, irsend.capture.value);
  EXPECT_FALSE(irsend.capture.repeat);

  irsend.makeDecodeResult(4 * kCoolixBits + 4);
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, kStartOffset, kCoolixBits,
                                  true));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(kCoolixBits, irsend.capture.bits);
  EXPECT_EQ(0x123456, irsend.capture.value);

  irsend.makeDecodeResult(2 * (4 * kCoolixBits + 4));
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, kStartOffset, kCoolixBits,
                                  true));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(kCoolixBits, irsend.capture.bits);
  EXPECT_EQ(0x123456, irsend.capture.value);
}

// Decode unsupported Coolix messages.
TEST(TestDecodeCoolix, DecodeWithNonStrictSizes) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendCOOLIX(0x12, 8);  // Illegal value Coolix 8-bit message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeCOOLIX(&irsend.capture, kStartOffset, kCoolixBits,
                                   true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, kStartOffset, 8, false));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(8, irsend.capture.bits);
  EXPECT_EQ(0x12, irsend.capture.value);

  irsend.reset();
  irsend.sendCOOLIX(0x12345678, 32);  // Illegal value Coolix 32-bit message.
  irsend.makeDecodeResult();
  // Shouldn't pass with strict when we ask for less bits than we got.
  ASSERT_FALSE(irrecv.decodeCOOLIX(&irsend.capture, kStartOffset, kCoolixBits,
                                   true));

  irsend.makeDecodeResult();
  // Should fail with strict when we ask for the wrong bit size.
  ASSERT_FALSE(irrecv.decodeCOOLIX(&irsend.capture, kStartOffset, 32, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, kStartOffset, 32, false));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(32, irsend.capture.bits);
  EXPECT_EQ(0x12345678, irsend.capture.value);

  // Decode should fail if asked to decode non-multiples of 8 bits.
  irsend.reset();
  irsend.sendCOOLIX(0x123456, kCoolixBits, 2);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeCOOLIX(&irsend.capture, kStartOffset, 9, false));
}

// Decode (non-standard) 64-bit messages.
TEST(TestDecodeCoolix, Decode64BitMessages) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  // Illegal size Coolix 64-bit message.
  irsend.sendCOOLIX(0xFFFFFFFFFFFFFFFF, 64);
  irsend.makeDecodeResult();
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeCOOLIX(&irsend.capture, kStartOffset, 64, false));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, irsend.capture.value);
}

// Fail to decode a non-Coolix example via GlobalCache
TEST(TestDecodeCoolix, FailToDecodeNonCoolixExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
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
  ASSERT_FALSE(irrecv.decodeCOOLIX(&irsend.capture, kStartOffset, kCoolixBits,
                                   false));
}

// Tests for the IRCoolixAC class.
TEST(TestCoolixACClass, SetAndGetRaw) {
  IRCoolixAC ac(kGpioUnused);

  ac.setRaw(0xB21F28);
  EXPECT_EQ(0xB21F28, ac.getRaw());
  ac.setRaw(kCoolixDefaultState);
  EXPECT_EQ(kCoolixDefaultState, ac.getRaw());
}

TEST(TestCoolixACClass, SetAndGetTemp) {
  IRCoolixAC ac(kGpioUnused);

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());
  ac.setTemp(kCoolixTempMin);
  EXPECT_EQ(kCoolixTempMin, ac.getTemp());
  ac.setTemp(kCoolixTempMax);
  EXPECT_EQ(kCoolixTempMax, ac.getTemp());
  ac.setTemp(kCoolixTempMin - 1);
  EXPECT_EQ(kCoolixTempMin, ac.getTemp());
  ac.setTemp(kCoolixTempMax + 1);
  EXPECT_EQ(kCoolixTempMax, ac.getTemp());
}

TEST(TestCoolixACClass, SetAndGetMode) {
  IRCoolixAC ac(kGpioUnused);

  ac.setMode(kCoolixHeat);
  EXPECT_EQ(kCoolixHeat, ac.getMode());
  ac.setMode(kCoolixCool);
  EXPECT_EQ(kCoolixCool, ac.getMode());
  ac.setMode(kCoolixDry);
  EXPECT_EQ(kCoolixDry, ac.getMode());
  ac.setMode(kCoolixAuto);
  EXPECT_EQ(kCoolixAuto, ac.getMode());
  ac.setMode(kCoolixFan);
  EXPECT_EQ(kCoolixFan, ac.getMode());
}

TEST(TestCoolixACClass, SetAndGetFan) {
  IRCoolixAC ac(kGpioUnused);

  // This mode allows pretty much everything except Auto0 speed.
  ac.setMode(kCoolixCool);
  ac.setFan(kCoolixFanMax);
  EXPECT_EQ(kCoolixFanMax, ac.getFan());
  ac.setFan(kCoolixFanMin);
  EXPECT_EQ(kCoolixFanMin, ac.getFan());
  ac.setFan(kCoolixFanZoneFollow);
  EXPECT_EQ(kCoolixFanZoneFollow, ac.getFan());
  ac.setFan(kCoolixFanAuto);
  EXPECT_EQ(kCoolixFanAuto, ac.getFan());
  ac.setFan(kCoolixFanAuto0);
  EXPECT_EQ(kCoolixFanAuto, ac.getFan());
  ac.setFan(kCoolixFanMax);
  EXPECT_EQ(kCoolixFanMax, ac.getFan());
  ASSERT_NE(3, kCoolixFanAuto);
  // Now try some unexpected value.
  ac.setFan(3);
  EXPECT_EQ(kCoolixFanAuto, ac.getFan());

  // These modes allows pretty much everything except Auto speed.
  ac.setMode(kCoolixDry);
  EXPECT_EQ(kCoolixFanAuto0, ac.getFan());
  ac.setFan(kCoolixFanMax);
  EXPECT_EQ(kCoolixFanMax, ac.getFan());
  ac.setFan(kCoolixFanAuto);
  EXPECT_EQ(kCoolixFanAuto0, ac.getFan());

  ac.setMode(kCoolixAuto);
  EXPECT_EQ(kCoolixFanAuto0, ac.getFan());
  ac.setFan(kCoolixFanMax);
  EXPECT_EQ(kCoolixFanMax, ac.getFan());
  ac.setFan(kCoolixFanAuto0);
  EXPECT_EQ(kCoolixFanAuto0, ac.getFan());
}

TEST(TestCoolixACClass, SetGetClearSensorTempAndZoneFollow) {
  IRCoolixAC ac(kGpioUnused);

  ac.setRaw(kCoolixDefaultState);
  EXPECT_FALSE(ac.getZoneFollow());
  EXPECT_LE(kCoolixSensorTempMax, ac.getSensorTemp());

  ac.setSensorTemp(25);
  EXPECT_TRUE(ac.getZoneFollow());
  EXPECT_EQ(25, ac.getSensorTemp());

  // Lower bounds
  ac.setSensorTemp(0);
  EXPECT_TRUE(ac.getZoneFollow());
  EXPECT_EQ(0, ac.getSensorTemp());

  // Upper bounds
  ac.setSensorTemp(kCoolixSensorTempMax);
  EXPECT_TRUE(ac.getZoneFollow());
  EXPECT_EQ(kCoolixSensorTempMax, ac.getSensorTemp());
  ac.setSensorTemp(kCoolixSensorTempMax + 1);
  EXPECT_TRUE(ac.getZoneFollow());
  EXPECT_EQ(kCoolixSensorTempMax, ac.getSensorTemp());
  // Clearing
  ac.clearSensorTemp();
  EXPECT_FALSE(ac.getZoneFollow());
  EXPECT_LT(kCoolixSensorTempMax, ac.getSensorTemp());

  // toString.
  // For https://github.com/crankyoldgit/IRremoteESP8266/issues/1318#issuecomment-729663834
  ac.setRaw(0xBAD34E);
  EXPECT_EQ(
      "Power: On, Mode: 3 (Heat), Fan: 6 (Zone Follow), Temp: 24C, "
      "Zone Follow: On, Sensor Temp: 19C", ac.toString());
}

TEST(TestCoolixACClass, SpecialModesAndReset) {
  IRCoolixAC ac(kGpioUnused);
  ASSERT_NE(kCoolixSwing, ac.getRaw());
  ac.setSwing();
  ASSERT_EQ(kCoolixSwing, ac.getRaw());
  ac.setTurbo();
  ASSERT_EQ(kCoolixTurbo, ac.getRaw());
  ac.setSleep();
  ASSERT_EQ(kCoolixSleep, ac.getRaw());
  ac.setLed();
  ASSERT_EQ(kCoolixLed, ac.getRaw());
  ac.setClean();
  ASSERT_EQ(kCoolixClean, ac.getRaw());
  ac.stateReset();
  ASSERT_NE(kCoolixClean, ac.getRaw());
}

TEST(TestCoolixACClass, HumanReadable) {
  IRCoolixAC ac(kGpioUnused);
  ac.begin();
  ac.setPower(true);

  // Initial starting point.
  EXPECT_EQ(
      "Power: On, Mode: 2 (Auto), Fan: 0 (Auto0), Temp: 25C, "
      "Zone Follow: Off, Sensor Temp: Off",
      ac.toString());
  ac.setSensorTemp(24);
  ac.setTemp(22);
  ac.setMode(kCoolixCool);
  ac.setFan(kCoolixFanMin);
  EXPECT_EQ(
      "Power: On, Mode: 0 (Cool), Fan: 4 (Min), Temp: 22C, "
      "Zone Follow: On, Sensor Temp: 24C",
      ac.toString());
  ac.setSwing();
  EXPECT_EQ("Power: On, Swing: Toggle", ac.toString());
  ac.setPower(false);
  EXPECT_EQ("Power: Off", ac.toString());
}

TEST(TestCoolixACClass, KnownExamples) {
  IRCoolixAC ac(kGpioUnused);
  ac.begin();
  ac.setPower(true);
  ac.setRaw(0b101100101011111111100100);
  EXPECT_EQ(
      "Power: On, Mode: 4 (Fan), Fan: 5 (Auto), Zone Follow: Off, "
      "Sensor Temp: Off",
      ac.toString());
  ac.setRaw(0b101100101001111100000000);
  EXPECT_EQ(
      "Power: On, Mode: 0 (Cool), Fan: 4 (Min), Temp: 17C, "
      "Zone Follow: Off, Sensor Temp: Off",
      ac.toString());
}

TEST(TestCoolixACClass, Issue579FanAuto0) {
  IRCoolixAC ac(kGpioUnused);
  ac.begin();
  ac.setPower(true);
  ac.setRaw(0xB21F28);
  EXPECT_EQ(
      "Power: On, Mode: 2 (Auto), Fan: 0 (Auto0), Temp: 20C, "
      "Zone Follow: Off, Sensor Temp: Off",
      ac.toString());
}

TEST(TestDecodeCoolix, RealCaptureExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);

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
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/624
TEST(TestCoolixACClass, Issue624HandleSpecialStatesBetter) {
  IRCoolixAC ac(kGpioUnused);
  ac.begin();
  ac.setPower(true);
  // Default
  EXPECT_EQ(
      "Power: On, Mode: 2 (Auto), Fan: 0 (Auto0), Temp: 25C, Zone Follow: Off, "
      "Sensor Temp: Off",
      ac.toString());
  EXPECT_EQ(0xB21FC8, ac.getRaw());
  // Change of settings.
  ac.setTemp(24);
  ac.setMode(kCoolixCool);
  ac.setFan(kCoolixFanAuto);
  EXPECT_EQ(
      "Power: On, Mode: 0 (Cool), Fan: 5 (Auto), Temp: 24C, Zone Follow: Off, "
      "Sensor Temp: Off",
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
      "Power: On, Mode: 0 (Cool), Fan: 5 (Auto), Temp: 24C, Zone Follow: Off, "
      "Sensor Temp: Off",
      ac.toString());
  EXPECT_EQ(0xB2BF40, ac.getRaw());

  // Repeat change of settings.
  ac.setTemp(24);
  ac.setMode(kCoolixCool);
  ac.setFan(kCoolixFanAuto);
  EXPECT_EQ(
      "Power: On, Mode: 0 (Cool), Fan: 5 (Auto), Temp: 24C, Zone Follow: Off, "
      "Sensor Temp: Off",
      ac.toString());
  EXPECT_EQ(0xB2BF40, ac.getRaw());
}

TEST(TestCoolixACClass, toCommon) {
  IRCoolixAC ac(kGpioUnused);
  ac.begin();
  ac.setPower(true);
  ac.setMode(kCoolixCool);
  ac.setTemp(20);
  ac.setFan(kCoolixFanMax);

  // Now test it.
  ASSERT_EQ(decode_type_t::COOLIX, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kOff, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  // Unsupported.
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

TEST(TestCoolixACClass, Issue722) {
  IRrecv irrecv(kGpioUnused);
  IRCoolixAC ac(kGpioUnused);

  // Auto 17C ON pressed
  uint32_t on_auto_17c_fan_auto0 = 0xB21F08;
  ac.begin();
  ac.setPower(true);
  ac.setMode(kCoolixAuto);
  ac.setFan(kCoolixFanAuto);
  ac.setTemp(17);
  EXPECT_EQ(on_auto_17c_fan_auto0, ac.getRaw());

  // Off
  uint32_t off = 0xB27BE0;
  ac.off();
  EXPECT_EQ(off, ac.getRaw());

  // ON Auto Temp 18C
  uint32_t on_auto_18c_fan_auto0 = 0xB21F18;
  ac.on();
  ac.setTemp(18);
  EXPECT_EQ(on_auto_18c_fan_auto0, ac.getRaw());

  // Set Mode Cool 18C
  uint32_t on_cool_18c_fan_auto = 0xB2BF10;
  ac.setMode(kCoolixCool);
  EXPECT_EQ(on_cool_18c_fan_auto, ac.getRaw());

  // Set Mode DRY 18C
  uint32_t on_dry_18c_fan_auto0 = 0xB21F14;
  ac.setMode(kCoolixDry);
  EXPECT_EQ(on_dry_18c_fan_auto0, ac.getRaw());

  // Set Mode HEAT 18C
  uint32_t on_heat_18c_fan_auto = 0xB2BF1C;
  ac.setMode(kCoolixHeat);
  EXPECT_EQ(on_heat_18c_fan_auto, ac.getRaw());

  // Set mode FAN
  uint32_t on_fan_18c_fan_auto = 0xB2BFE4;
  ac.setMode(kCoolixFan);
  EXPECT_EQ(on_fan_18c_fan_auto, ac.getRaw());

  // Fan level 2 (initial was auto)
  uint32_t on_fan_18c_fan_min = 0xB29FE4;
  ac.setFan(kCoolixFanMin);
  EXPECT_EQ(on_fan_18c_fan_min, ac.getRaw());

  // Fan level 3
  uint32_t on_fan_18c_fan_med = 0xB25FE4;
  ac.setFan(kCoolixFanMed);
  EXPECT_EQ(on_fan_18c_fan_med, ac.getRaw());

  // Fan level 4
  uint32_t on_fan_18c_fan_max = 0xB23FE4;
  ac.setFan(kCoolixFanMax);
  EXPECT_EQ(on_fan_18c_fan_max, ac.getRaw());

  // Test sending the last message to verify the class send() method works.
  ac.send();
  ac._irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&ac._irsend.capture));
  EXPECT_EQ(COOLIX, ac._irsend.capture.decode_type);
  EXPECT_EQ(kCoolixBits, ac._irsend.capture.bits);
  EXPECT_EQ(on_fan_18c_fan_max, ac._irsend.capture.value);
  EXPECT_EQ(0x0, ac._irsend.capture.address);
  EXPECT_EQ(0x0, ac._irsend.capture.command);
  EXPECT_EQ(
      // Raw data supplied by @mariusmotea
      "f38000d50"
      // 4434,4376,
      "m4692s4416"
      // 566,1614,592,504,566,1618,566,1616,568,528,564,532,564,1616,568,532,
      "m552s1656m552s552m552s1656m552s1656m552s552m552s552m552s1656m552s552"
      // 566,530,566,1620,568,528,566,530,566,1618,564,1618,566,530,564,1624,
      "m552s552m552s1656m552s552m552s552m552s1656m552s1656m552s552m552s1656"
      // 538,560,566,530,564,1620,566,1618,566,1618,566,1616,566,1616,566,1620,
      "m552s552m552s552m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656"
      // 568,1620,566,1616,566,530,566,530,564,530,562,532,564,530,566,530,
      "m552s1656m552s1656m552s552m552s552m552s552m552s552m552s552m552s552"
      // 566,1622,566,1616,540,1642,566,528,566,530,566,1616,566,530,566,532,
      "m552s1656m552s1656m552s1656m552s552m552s552m552s1656m552s552m552s552"
      // 564,532,564,530,566,530,566,1614,566,1616,562,532,564,1620,566,1618,
      "m552s552m552s552m552s552m552s1656m552s1656m552s552m552s1656m552s1656"
      // 538,5254,4432,4364,566,1616,568,530,564,1620,568,1616,564,532,564,530,
      "m552s5244m4692s4416m552s1656m552s552m552s1656m552s1656m552s552m552s552"
      // 566,1616,566,532,564,532,566,1620,568,528,566,530,566,1616,564,1618,
      "m552s1656m552s552m552s552m552s1656m552s552m552s552m552s1656m552s1656"
      // 566,530,566,1622,566,532,566,528,566,1620,568,1614,566,1618,566,1618,
      "m552s552m552s1656m552s552m552s552m552s1656m552s1656m552s1656m552s1656"
      // 566,1614,568,1618,566,1622,568,1616,566,530,564,530,566,530,566,528,
      "m552s1656m552s1656m552s1656m552s1656m552s552m552s552m552s552m552s552"
      // 564,530,566,532,566,1622,564,1616,566,1616,564,532,564,530,564,1616,
      "m552s552m552s552m552s1656m552s1656m552s1656m552s552m552s552m552s1656"
      // 564,530,564,532,566,530,564,530,566,528,564,1618,564,1618,564,532,
      "m552s552m552s552m552s552m552s552m552s552m552s1656m552s1656m552s552"
      // 564,1620,566,1618,562  // Raw data matches what is expected.
      "m552s1656m552s1656m552s105244", ac._irsend.outputStr());
}

TEST(TestCoolixACClass, Issue985) {
  IRrecv irrecv(kGpioUnused);
  IRCoolixAC ac(kGpioUnused);

  // Test that if we ONLY turn the power off, it only sends a "power off" mesg.
  // i.e. Code from: https://github.com/crankyoldgit/IRremoteESP8266/issues/985#issue-516210106
  // First block in the first code included.
  ac.setPower(false);
  ac.send();

  ac._irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&ac._irsend.capture));
  EXPECT_EQ(COOLIX, ac._irsend.capture.decode_type);
  EXPECT_EQ(kCoolixBits, ac._irsend.capture.bits);
  EXPECT_EQ(kCoolixOff, ac._irsend.capture.value);
  EXPECT_EQ("Power: Off", IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));

  ac._irsend.reset();

  // Turn the unit on, cool mode, and set the temp.
  // Code from: https://github.com/crankyoldgit/IRremoteESP8266/issues/985#issue-516210106
  // Second block in the first code included.
  uint8_t aircon_temp = 20;  // Random value chosen.
  ac.setPower(true);
  ac.setMode(kCoolixCool);
  ac.setTemp(aircon_temp);
  ac.send();

  ac._irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&ac._irsend.capture));
  EXPECT_EQ(COOLIX, ac._irsend.capture.decode_type);
  EXPECT_EQ(kCoolixBits, ac._irsend.capture.bits);
  EXPECT_NE(kCoolixOff, ac._irsend.capture.value);
  EXPECT_EQ(
      "Power: On, Mode: 0 (Cool), Fan: 5 (Auto), Temp: 20C, Zone Follow: Off, "
      "Sensor Temp: Off", IRAcUtils::resultAcToString(&ac._irsend.capture));
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));

  ac._irsend.reset();

  // Now repeat the first block again.
  // i.e. Code from: https://github.com/crankyoldgit/IRremoteESP8266/issues/985#issue-516210106
  // First block in the first code included.
  ac.setPower(false);
  ac.send();

  ac._irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&ac._irsend.capture));
  EXPECT_EQ(COOLIX, ac._irsend.capture.decode_type);
  EXPECT_EQ(kCoolixBits, ac._irsend.capture.bits);
  EXPECT_EQ(kCoolixOff, ac._irsend.capture.value);
  EXPECT_EQ("Power: Off", IRAcUtils::resultAcToString(&ac._irsend.capture));
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestCoolixACClass, PowerStateWithSetRaw) {
  IRrecv irrecv(kGpioUnused);
  IRCoolixAC ac(kGpioUnused);

  // Problem reported that power is always off via decodeToState()
  // Ref:
  //   https://github.com/crankyoldgit/IRremoteESP8266/pull/1040
  //   https://github.com/arendst/Tasmota/issues/7660

  const uint32_t on_code =  0xB2BFCC;  // A valid "on" message.

  // Check the off case.
  ac.setRaw(kCoolixOff);
  ASSERT_FALSE(ac.getPower());
  EXPECT_FALSE(ac.toCommon().power);

  // Check the "on" case.
  ac.setRaw(on_code);
  ASSERT_TRUE(ac.getPower());
  EXPECT_TRUE(ac.toCommon().power);
  // Now check the reported decodeToState() is also fixed.
  ac._irsend.reset();
  ac.send();
  ac._irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&ac._irsend.capture));
  EXPECT_EQ(COOLIX, ac._irsend.capture.decode_type);
  EXPECT_EQ(kCoolixBits, ac._irsend.capture.bits);
  EXPECT_EQ(on_code, ac._irsend.capture.value);
  EXPECT_EQ(
      "Power: On, Mode: 3 (Heat), Fan: 5 (Auto), Temp: 25C, Zone Follow: Off, "
      "Sensor Temp: Off", IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t result;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &result));
  EXPECT_TRUE(result.power);

  // Recheck the off case to ensure it changes.
  ac.setRaw(kCoolixOff);
  ASSERT_FALSE(ac.getPower());
  EXPECT_FALSE(ac.toCommon().power);
}

TEST(TestDecodeCoolix, Issue1318_DirectMessage) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);

  // From https://github.com/crankyoldgit/IRremoteESP8266/issues/1318#issuecomment-727611979
  const uint16_t direct[99] = {
      4386, 4366, 546, 1622, 520, 522, 548, 1622, 496, 1646, 520, 524, 548, 522,
      548, 1622, 522, 522, 548, 520, 550, 1620, 520, 522, 548, 522, 546, 1622,
      496, 1646, 522, 520, 526, 1646, 520, 522, 548, 522, 524, 546, 550, 520,
      550, 1620, 498, 1644, 520, 1622, 522, 1622, 520, 1620, 522, 1620, 524,
      1618, 524, 1618, 524, 520, 550, 520, 550, 522, 550, 518, 552, 1618, 524,
      1618, 524, 1618, 524, 520, 550, 520, 550, 520, 550, 520, 550, 520, 552,
      516, 552, 518, 550, 522, 550, 1618, 526, 1616, 524, 1618, 524, 1618, 524,
      1618, 550};  // UNKNOWN B0473CC8

  irsend.begin();
  irsend.reset();

  irsend.sendRaw(direct, 99, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(kCoolixBits, irsend.capture.bits);
  EXPECT_EQ(kCoolixSwingV, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_EQ(
      "Power: On, Swing(V): Step",
      IRAcUtils::resultAcToString(&irsend.capture));
}

TEST(TestCoolixACClass, SendStep) {
  IRrecv irrecv(kGpioUnused);
  IRCoolixAC ac(kGpioUnused);

  ac.setSwingVStep();
  ac.send();
  ac._irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&ac._irsend.capture));
  EXPECT_EQ(COOLIX, ac._irsend.capture.decode_type);
  EXPECT_EQ(kCoolixBits, ac._irsend.capture.bits);
  EXPECT_EQ(kCoolixSwingV, ac._irsend.capture.value);
  EXPECT_EQ(
      "Power: On, Swing(V): Step",
      IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
  EXPECT_EQ(
      "f38000d50"
      "m4692s4416"
      "m552s1656m552s552m552s1656m552s1656m552s552m552s552m552s1656"
      "m552s552m552s552m552s1656m552s552m552s552m552s1656m552s1656"
      "m552s552m552s1656m552s552m552s552m552s552m552s552m552s1656"
      "m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s552m552s552m552s552m552s552m552s1656m552s1656m552s1656"
      "m552s552m552s552m552s552m552s552m552s552m552s552m552s552"
      "m552s552m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s105244",
      ac._irsend.outputStr());
}

// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1318#issuecomment-731578060
// Confirm ZoneFollow Fan is being set correctly when SensorTemp is set.
TEST(TestCoolixACClass, VerifyZoneFollowFan) {
  IRCoolixAC ac(kGpioUnused);
  EXPECT_NE(kCoolixFanZoneFollow, ac.getFan());
  EXPECT_FALSE(ac.getZoneFollow());
  ac.setPower(true);
  ac.setMode(kCoolixHeat);
  ac.setTemp(24);  // C
  EXPECT_NE(kCoolixFanZoneFollow, ac.getFan());
  EXPECT_FALSE(ac.getZoneFollow());
  ac.setSensorTemp(19);  // C
  EXPECT_EQ(kCoolixFanZoneFollow, ac.getFan());
  EXPECT_TRUE(ac.getZoneFollow());
  EXPECT_EQ(0xBAD34E, ac.getRaw());
  EXPECT_EQ(
      "Power: On, Mode: 3 (Heat), Fan: 6 (Zone Follow), Temp: 24C, "
      "Zone Follow: On, Sensor Temp: 19C",
      ac.toString());
}

TEST(TestDecodeCoolix48, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);

  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1694#issue-1068786691
  // Off Timer: 1 hour
  const uint16_t rawData[199] = {
      4342, 4454, 486, 1724, 436, 658, 438, 1748, 464, 1718, 462, 634, 440, 656,
      462, 1696, 488, 634, 462, 634, 436, 1722, 516, 608, 462, 660, 436, 1694,
      488, 1720, 440, 630, 488, 1700, 488, 1704, 458, 660, 462, 1698, 490, 632,
      462, 634, 436, 684, 436, 1700, 464, 1748, 462, 634, 462, 1720, 436, 658,
      462, 1700, 488, 1692, 512, 1696, 438, 684, 410, 686, 434, 688, 408, 1696,
      488, 1694, 464, 682, 414, 1748, 436, 1722, 488, 632, 438, 686, 408, 662,
      462, 1696, 488, 1722, 462, 1696, 462, 1746, 436, 1798, 386, 1694, 490,
      1720, 516, 5234, 4370, 4446, 490, 1690, 492, 658, 434, 1726, 436, 1746,
      464, 604, 488, 658, 412, 1718, 490, 636, 460, 660, 438, 1698, 460, 662,
      458, 632, 436, 1718, 490, 1720, 488, 608, 436, 1754, 462, 1726, 438, 682,
      414, 1748, 464, 632, 460, 660, 410, 658, 438, 1748, 464, 1694, 464, 660,
      436, 1720, 488, 634, 460, 1726, 462, 1724, 462, 1692, 490, 606, 462, 714,
      384, 660, 460, 1722, 460, 1722, 490, 606, 464, 1718, 490, 1670, 486, 634,
      462, 662, 410, 660, 460, 1722, 464, 1718, 460, 1696, 464, 1720, 462, 1720,
      462, 1722, 486, 1700, 462};  // UNKNOWN 1F691B97

  irsend.begin();
  irsend.reset();

  irsend.sendRaw(rawData, 199, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(COOLIX48, irsend.capture.decode_type);
  EXPECT_EQ(kCoolix48Bits, irsend.capture.bits);
  EXPECT_EQ(0xB24DA35C6C7F, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

TEST(TestDecodeCoolix48, SyntheticSelfDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendCoolix48(0xB24DA35C6C7F);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(COOLIX48, irsend.capture.decode_type);
  EXPECT_EQ(kCoolix48Bits, irsend.capture.bits);
  EXPECT_EQ(0xB24DA35C6C7F, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  EXPECT_EQ(
      "f38000d33"
      "m4692s4416"  // Message.
      "m552s1656m552s552m552s1656m552s1656m552s552m552s552m552s1656m552s552"
      "m552s552m552s1656m552s552m552s552m552s1656m552s1656m552s552m552s1656"
      "m552s1656m552s552m552s1656m552s552m552s552m552s552m552s1656m552s1656"
      "m552s552m552s1656m552s552m552s1656m552s1656m552s1656m552s552m552s552"
      "m552s552m552s1656m552s1656m552s552m552s1656m552s1656m552s552m552s552"
      "m552s552m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s5244"
      "m4692s4416"  // Repeat
      "m552s1656m552s552m552s1656m552s1656m552s552m552s552m552s1656m552s552"
      "m552s552m552s1656m552s552m552s552m552s1656m552s1656m552s552m552s1656"
      "m552s1656m552s552m552s1656m552s552m552s552m552s552m552s1656m552s1656"
      "m552s552m552s1656m552s552m552s1656m552s1656m552s1656m552s552m552s552"
      "m552s552m552s1656m552s1656m552s552m552s1656m552s1656m552s552m552s552"
      "m552s552m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s5244",
      irsend.outputStr());
}
