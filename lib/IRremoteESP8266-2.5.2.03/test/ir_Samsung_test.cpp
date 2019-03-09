// Copyright 2017 David Conran

#include "ir_Samsung.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendSAMSUNG().

// Test sending typical data only.
TEST(TestSendSamsung, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSAMSUNG(0xE0E09966);  // Samsung TV Power On.
  EXPECT_EQ(
      "m4480s4480"
      "m560s1680m560s1680m560s1680m560s560m560s560m560s560m560s560"
      "m560s560m560s1680m560s1680m560s1680m560s560m560s560m560s560"
      "m560s560m560s560m560s1680m560s560m560s560m560s1680m560s1680"
      "m560s560m560s560m560s1680m560s560m560s1680m560s1680m560s560"
      "m560s560m560s1680m560s1680m560s560m560s47040",
      irsend.outputStr());

  irsend.reset();
}

// Test sending with different repeats.
TEST(TestSendSamsung, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSAMSUNG(0xE0E09966, kSamsungBits, 1);  // 1 repeat.
  EXPECT_EQ(
      "m4480s4480"
      "m560s1680m560s1680m560s1680m560s560m560s560m560s560m560s560"
      "m560s560m560s1680m560s1680m560s1680m560s560m560s560m560s560"
      "m560s560m560s560m560s1680m560s560m560s560m560s1680m560s1680"
      "m560s560m560s560m560s1680m560s560m560s1680m560s1680m560s560"
      "m560s560m560s1680m560s1680m560s560m560s47040"
      "m4480s4480"
      "m560s1680m560s1680m560s1680m560s560m560s560m560s560m560s560"
      "m560s560m560s1680m560s1680m560s1680m560s560m560s560m560s560"
      "m560s560m560s560m560s1680m560s560m560s560m560s1680m560s1680"
      "m560s560m560s560m560s1680m560s560m560s1680m560s1680m560s560"
      "m560s560m560s1680m560s1680m560s560m560s47040",
      irsend.outputStr());
  irsend.sendSAMSUNG(0xE0E09966, kSamsungBits, 2);  // 2 repeats.
  EXPECT_EQ(
      "m4480s4480"
      "m560s1680m560s1680m560s1680m560s560m560s560m560s560m560s560"
      "m560s560m560s1680m560s1680m560s1680m560s560m560s560m560s560"
      "m560s560m560s560m560s1680m560s560m560s560m560s1680m560s1680"
      "m560s560m560s560m560s1680m560s560m560s1680m560s1680m560s560"
      "m560s560m560s1680m560s1680m560s560m560s47040"
      "m4480s4480"
      "m560s1680m560s1680m560s1680m560s560m560s560m560s560m560s560"
      "m560s560m560s1680m560s1680m560s1680m560s560m560s560m560s560"
      "m560s560m560s560m560s1680m560s560m560s560m560s1680m560s1680"
      "m560s560m560s560m560s1680m560s560m560s1680m560s1680m560s560"
      "m560s560m560s1680m560s1680m560s560m560s47040"
      "m4480s4480"
      "m560s1680m560s1680m560s1680m560s560m560s560m560s560m560s560"
      "m560s560m560s1680m560s1680m560s1680m560s560m560s560m560s560"
      "m560s560m560s560m560s1680m560s560m560s560m560s1680m560s1680"
      "m560s560m560s560m560s1680m560s560m560s1680m560s1680m560s560"
      "m560s560m560s1680m560s1680m560s560m560s47040",
      irsend.outputStr());
}

// Tests for encodeSAMSUNG().

TEST(TestEncodeSamsung, NormalEncoding) {
  IRsendTest irsend(4);
  EXPECT_EQ(0xFF, irsend.encodeSAMSUNG(0, 0));
  EXPECT_EQ(0x8080807F, irsend.encodeSAMSUNG(1, 1));
  EXPECT_EQ(0xF8F805FA, irsend.encodeSAMSUNG(0x1F, 0xA0));
  EXPECT_EQ(0xA0A0CC33, irsend.encodeSAMSUNG(0x05, 0x33));
  EXPECT_EQ(0xFFFFFF00, irsend.encodeSAMSUNG(0xFF, 0xFF));
  EXPECT_EQ(0xE0E09966, irsend.encodeSAMSUNG(0x07, 0x99));
}

// Tests for decodeSAMSUNG().

// Decode normal Samsung messages.
TEST(TestDecodeSamsung, NormalDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Samsung 32-bit message.
  irsend.reset();
  irsend.sendSAMSUNG(0xE0E09966);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSAMSUNG(&irsend.capture, kSamsungBits, true));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(kSamsungBits, irsend.capture.bits);
  EXPECT_EQ(0xE0E09966, irsend.capture.value);
  EXPECT_EQ(0x07, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);

  // Synthesised Normal Samsung 32-bit message.
  irsend.reset();
  irsend.sendSAMSUNG(irsend.encodeSAMSUNG(0x07, 0x99));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSAMSUNG(&irsend.capture, kSamsungBits, true));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(kSamsungBits, irsend.capture.bits);
  EXPECT_EQ(0xE0E09966, irsend.capture.value);
  EXPECT_EQ(0x07, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);

  // Synthesised Normal Samsung 32-bit message.
  irsend.reset();
  irsend.sendSAMSUNG(irsend.encodeSAMSUNG(0x1, 0x1));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSAMSUNG(&irsend.capture, kSamsungBits, true));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(kSamsungBits, irsend.capture.bits);
  EXPECT_EQ(0x8080807F, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x1, irsend.capture.command);
}

// Decode normal repeated Samsung messages.
TEST(TestDecodeSamsung, NormalDecodeWithRepeatAndStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Samsung 32-bit message.
  irsend.reset();
  irsend.sendSAMSUNG(0xE0E09966, kSamsungBits, 2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSAMSUNG(&irsend.capture, kSamsungBits, true));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(kSamsungBits, irsend.capture.bits);
  EXPECT_EQ(0xE0E09966, irsend.capture.value);
  EXPECT_EQ(0x07, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);
}

// Decode unsupported Samsung messages.
TEST(TestDecodeSamsung, DecodeWithNonStrictValues) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSAMSUNG(0x0);  // Illegal value Samsung 32-bit message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture, kSamsungBits, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeSAMSUNG(&irsend.capture, kSamsungBits, false));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(kSamsungBits, irsend.capture.bits);
  EXPECT_EQ(0x0, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  irsend.sendSAMSUNG(0x12345678);  // Illegal value Samsung 32-bit message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture, kSamsungBits, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeSAMSUNG(&irsend.capture, kSamsungBits, false));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(kSamsungBits, irsend.capture.bits);
  EXPECT_EQ(0x12345678, irsend.capture.value);
  EXPECT_EQ(0x48, irsend.capture.address);
  EXPECT_EQ(0x6A, irsend.capture.command);

  // Illegal over length (36-bit) message.
  irsend.reset();
  irsend.sendSAMSUNG(irsend.encodeSAMSUNG(0, 0), 36);
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture, kSamsungBits, true));
  // Shouldn't pass if strict off and wrong expected bit size.
  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture, kSamsungBits, false));
  // Re-decode with correct bit size.
  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture, 36, true));
  ASSERT_TRUE(irrecv.decodeSAMSUNG(&irsend.capture, 36, false));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(36, irsend.capture.bits);
  EXPECT_EQ(0xFF, irsend.capture.value);  // We told it to expect 8 bits less.
  EXPECT_EQ(0x00, irsend.capture.address);
  EXPECT_EQ(0x00, irsend.capture.command);

  // Illegal under length (16-bit) message
  irsend.reset();
  irsend.sendSAMSUNG(irsend.encodeSAMSUNG(0x0, 0x0), 16);
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture, kSamsungBits, true));
  // And it should fail when we expect more bits.
  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture, kSamsungBits, false));

  // Should pass if strict off if we ask for correct nr. of bits sent.
  ASSERT_TRUE(irrecv.decodeSAMSUNG(&irsend.capture, 16, false));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(16, irsend.capture.bits);
  EXPECT_EQ(0xFF, irsend.capture.value);  // We told it to expect 4 bits less.
  EXPECT_EQ(0x00, irsend.capture.address);
  EXPECT_EQ(0x00, irsend.capture.command);

  // Should fail as we are expecting less bits than there are.
  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture, 12, false));
}

// Decode (non-standard) 64-bit messages.
// Decode unsupported Samsung messages.
TEST(TestDecodeSamsung, Decode64BitMessages) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Illegal value & size Samsung 64-bit message.
  irsend.sendSAMSUNG(0xFFFFFFFFFFFFFFFF, 64);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture, kSamsungBits, true));
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeSAMSUNG(&irsend.capture, 64, false));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, irsend.capture.value);
  EXPECT_EQ(0xFF, irsend.capture.address);
  EXPECT_EQ(0xFF, irsend.capture.command);
}

// Decode a 'real' example via GlobalCache
TEST(TestDecodeSamsung, DecodeGlobalCacheExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Samsung TV Power On from Global Cache.
  uint16_t gc_test[71] = {38000, 1,  1,  172, 172, 22, 64, 22, 64, 22, 64,  22,
                          21,    22, 21, 22,  21,  22, 21, 22, 21, 22, 64,  22,
                          64,    22, 64, 22,  21,  22, 21, 22, 21, 22, 21,  22,
                          21,    22, 64, 22,  21,  22, 21, 22, 64, 22, 64,  22,
                          21,    22, 21, 22,  64,  22, 21, 22, 64, 22, 64,  22,
                          21,    22, 21, 22,  64,  22, 64, 22, 21, 22, 1820};
  irsend.sendGC(gc_test, 71);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodeSAMSUNG(&irsend.capture));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(kSamsungBits, irsend.capture.bits);
  EXPECT_EQ(0xE0E09966, irsend.capture.value);
  EXPECT_EQ(0x07, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);
}

// Fail to decode a non-Samsung example via GlobalCache
TEST(TestDecodeSamsung, FailToDecodeNonSamsungExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Modified a few entries to unexpected values, based on previous test case.
  uint16_t gc_test[71] = {38000, 1,  1,  172, 172, 22, 64, 22, 64, 22, 64,  22,
                          21,    22, 21, 22,  21,  22, 11, 22, 21, 22, 128, 22,
                          64,    22, 64, 22,  21,  22, 21, 22, 21, 22, 21,  22,
                          21,    22, 64, 22,  21,  22, 21, 22, 64, 22, 64,  22,
                          21,    22, 21, 22,  64,  22, 21, 22, 64, 22, 64,  22,
                          21,    22, 21, 22,  64,  22, 64, 22, 21, 22, 1820};
  irsend.sendGC(gc_test, 71);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture, kSamsungBits, false));
}

// Tests for sendSamsungAC().

// Test sending typical data only.
TEST(TestSendSamsungAC, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();
  uint8_t data[kSamsungAcStateLength] = {0x02, 0x92, 0x0F, 0x00, 0x00,
                                         0x00, 0xF0, 0x01, 0x02, 0xAF,
                                         0x71, 0x00, 0x15, 0xF0};
  irsend.sendSamsungAC(data);
  EXPECT_EQ(
      "m690s17844"
      "m3086s8864"
      "m586s436m586s1432m586s436m586s436m586s436m586s436m586s436m586s436"
      "m586s436m586s1432m586s436m586s436m586s1432m586s436m586s436m586s1432"
      "m586s1432m586s1432m586s1432m586s1432m586s436m586s436m586s436m586s436"
      "m586s436m586s436m586s436m586s436m586s436m586s436m586s436m586s436"
      "m586s436m586s436m586s436m586s436m586s436m586s436m586s436m586s436"
      "m586s436m586s436m586s436m586s436m586s436m586s436m586s436m586s436"
      "m586s436m586s436m586s436m586s436m586s1432m586s1432m586s1432m586s1432"
      "m586s2886"
      "m3086s8864"
      "m586s1432m586s436m586s436m586s436m586s436m586s436m586s436m586s436"
      "m586s436m586s1432m586s436m586s436m586s436m586s436m586s436m586s436"
      "m586s1432m586s1432m586s1432m586s1432m586s436m586s1432m586s436m586s1432"
      "m586s1432m586s436m586s436m586s436m586s1432m586s1432m586s1432m586s436"
      "m586s436m586s436m586s436m586s436m586s436m586s436m586s436m586s436"
      "m586s1432m586s436m586s1432m586s436m586s1432m586s436m586s436m586s436"
      "m586s436m586s436m586s436m586s436m586s1432m586s1432m586s1432m586s1432"
      "m586s100000",
      irsend.outputStr());
}

// Test sending extended data.
TEST(TestSendSamsungAC, SendExtendedData) {
  IRsendTest irsend(0);
  irsend.begin();
  // "Off" message.
  uint8_t data[kSamsungAcExtendedStateLength] = {
      0x02, 0xB2, 0x0F, 0x00, 0x00, 0x00, 0xC0, 0x01, 0xD2, 0x0F, 0x00,
      0x00, 0x00, 0x00, 0x01, 0x02, 0xFF, 0x71, 0x80, 0x11, 0xC0};
  irsend.sendSamsungAC(data, kSamsungAcExtendedStateLength);
  EXPECT_EQ(
      "m690s17844"
      "m3086s8864"
      "m586s436m586s1432m586s436m586s436m586s436m586s436m586s436m586s436"
      "m586s436m586s1432m586s436m586s436m586s1432m586s1432m586s436m586s1432"
      "m586s1432m586s1432m586s1432m586s1432m586s436m586s436m586s436m586s436"
      "m586s436m586s436m586s436m586s436m586s436m586s436m586s436m586s436"
      "m586s436m586s436m586s436m586s436m586s436m586s436m586s436m586s436"
      "m586s436m586s436m586s436m586s436m586s436m586s436m586s436m586s436"
      "m586s436m586s436m586s436m586s436m586s436m586s436m586s1432m586s1432"
      "m586s2886"
      "m3086s8864"
      "m586s1432m586s436m586s436m586s436m586s436m586s436m586s436m586s436"
      "m586s436m586s1432m586s436m586s436m586s1432m586s436m586s1432m586s1432"
      "m586s1432m586s1432m586s1432m586s1432m586s436m586s436m586s436m586s436"
      "m586s436m586s436m586s436m586s436m586s436m586s436m586s436m586s436"
      "m586s436m586s436m586s436m586s436m586s436m586s436m586s436m586s436"
      "m586s436m586s436m586s436m586s436m586s436m586s436m586s436m586s436"
      "m586s436m586s436m586s436m586s436m586s436m586s436m586s436m586s436"
      "m586s2886"
      "m3086s8864"
      "m586s1432m586s436m586s436m586s436m586s436m586s436m586s436m586s436"
      "m586s436m586s1432m586s436m586s436m586s436m586s436m586s436m586s436"
      "m586s1432m586s1432m586s1432m586s1432m586s1432m586s1432m586s1432m586s1432"
      "m586s1432m586s436m586s436m586s436m586s1432m586s1432m586s1432m586s436"
      "m586s436m586s436m586s436m586s436m586s436m586s436m586s436m586s1432"
      "m586s1432m586s436m586s436m586s436m586s1432m586s436m586s436m586s436"
      "m586s436m586s436m586s436m586s436m586s436m586s436m586s1432m586s1432"
      "m586s100000",
      irsend.outputStr());
}

// Tests for IRSamsungAc class.

TEST(TestIRSamsungAcClass, SetAndGetRaw) {
  uint8_t expectedState[kSamsungAcStateLength] = {0x02, 0x92, 0x0F, 0x00, 0x00,
                                                  0x00, 0xF0, 0x01, 0xE2, 0xFE,
                                                  0x71, 0x40, 0x11, 0xF0};
  IRSamsungAc samsung(0);
  samsung.setRaw(expectedState);
  EXPECT_STATE_EQ(expectedState, samsung.getRaw(), kSamsungAcBits);
  uint8_t extendedState[kSamsungAcExtendedStateLength] = {
      0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0, 0x01, 0xD2, 0x0F, 0x00,
      0x00, 0x00, 0x00, 0x01, 0xE2, 0xFE, 0x71, 0x40, 0x11, 0xF0};
  samsung.setRaw(extendedState, kSamsungAcExtendedStateLength);
  // We should NOT get the extended state back.
  EXPECT_STATE_EQ(expectedState, samsung.getRaw(), kSamsungAcBits);
}

TEST(TestIRSamsungAcClass, SetAndGetPower) {
  IRSamsungAc samsung(0);
  samsung.on();
  EXPECT_TRUE(samsung.getPower());
  samsung.off();
  EXPECT_FALSE(samsung.getPower());
  samsung.setPower(true);
  EXPECT_TRUE(samsung.getPower());
  samsung.setPower(false);
  EXPECT_FALSE(samsung.getPower());
}

TEST(TestIRSamsungAcClass, SetAndGetSwing) {
  IRSamsungAc samsung(0);
  samsung.setSwing(true);
  EXPECT_TRUE(samsung.getSwing());
  samsung.setSwing(false);
  EXPECT_FALSE(samsung.getSwing());
  samsung.setSwing(true);
  EXPECT_TRUE(samsung.getSwing());

  // Real examples from:
  // https://github.com/markszabo/IRremoteESP8266/issues/505#issuecomment-424036602
  // TODO(Hollako): Explain why state[9] lowest bit changes between on and off.
  const uint8_t expected_off[kSamsungAcStateLength] = {
      0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0,
      0x01, 0xE2, 0xFE, 0x71, 0x80, 0x11, 0xF0};
  samsung.setRaw(expected_off);
  EXPECT_FALSE(samsung.getSwing());
  const uint8_t expected_on[kSamsungAcStateLength] = {
      0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0,
      0x01, 0x02, 0xAF, 0x71, 0x80, 0x11, 0xF0};
  samsung.setRaw(expected_on);
  EXPECT_TRUE(samsung.getSwing());
}

TEST(TestIRSamsungAcClass, SetAndGetClean) {
  IRSamsungAc samsung(0);
  samsung.setClean(true);
  EXPECT_TRUE(samsung.getClean());
  samsung.setClean(false);
  EXPECT_FALSE(samsung.getClean());
  samsung.setClean(true);
  EXPECT_TRUE(samsung.getClean());
}

TEST(TestIRSamsungAcClass, SetAndGetBeep) {
  IRSamsungAc samsung(0);
  samsung.setBeep(false);
  EXPECT_FALSE(samsung.getBeep());
  samsung.setBeep(true);
  EXPECT_TRUE(samsung.getBeep());
  samsung.setBeep(false);
  EXPECT_FALSE(samsung.getBeep());
  samsung.setBeep(true);
  EXPECT_TRUE(samsung.getBeep());
}

TEST(TestIRSamsungAcClass, SetAndGetTemp) {
  IRSamsungAc samsung(0);
  samsung.setTemp(25);
  EXPECT_EQ(25, samsung.getTemp());
  samsung.setTemp(kSamsungAcMinTemp);
  EXPECT_EQ(kSamsungAcMinTemp, samsung.getTemp());
  samsung.setTemp(kSamsungAcMinTemp - 1);
  EXPECT_EQ(kSamsungAcMinTemp, samsung.getTemp());
  samsung.setTemp(kSamsungAcMaxTemp);
  EXPECT_EQ(kSamsungAcMaxTemp, samsung.getTemp());
  samsung.setTemp(kSamsungAcMaxTemp + 1);
  EXPECT_EQ(kSamsungAcMaxTemp, samsung.getTemp());
}

TEST(TestIRSamsungAcClass, SetAndGetMode) {
  IRSamsungAc samsung(0);
  samsung.setMode(kSamsungAcCool);
  EXPECT_EQ(kSamsungAcCool, samsung.getMode());
  EXPECT_NE(kSamsungAcFanAuto2, samsung.getFan());
  samsung.setMode(kSamsungAcHeat);
  EXPECT_EQ(kSamsungAcHeat, samsung.getMode());
  EXPECT_NE(kSamsungAcFanAuto2, samsung.getFan());
  samsung.setMode(kSamsungAcAuto);
  EXPECT_EQ(kSamsungAcAuto, samsung.getMode());
  EXPECT_EQ(kSamsungAcFanAuto2, samsung.getFan());
  samsung.setMode(kSamsungAcDry);
  EXPECT_EQ(kSamsungAcDry, samsung.getMode());
  EXPECT_NE(kSamsungAcFanAuto2, samsung.getFan());
}

TEST(TestIRSamsungAcClass, SetAndGetFan) {
  IRSamsungAc samsung(0);
  samsung.setMode(kSamsungAcCool);  // Most fan modes avail in this setting.
  samsung.setFan(kSamsungAcFanAuto);
  EXPECT_EQ(kSamsungAcFanAuto, samsung.getFan());
  samsung.setFan(kSamsungAcFanLow);
  EXPECT_EQ(kSamsungAcFanLow, samsung.getFan());
  samsung.setFan(kSamsungAcFanAuto2);             // Not available in Cool mode.
  EXPECT_EQ(kSamsungAcFanLow, samsung.getFan());  // Shouldn't change.
  samsung.setMode(kSamsungAcAuto);                // Has special fan setting.
  EXPECT_EQ(kSamsungAcFanAuto2, samsung.getFan());
  samsung.setFan(kSamsungAcFanLow);  // Shouldn't be available in Auto mode.
  EXPECT_EQ(kSamsungAcFanAuto2, samsung.getFan());
  samsung.setMode(kSamsungAcHeat);  // Most fan modes avail in this setting.
  samsung.setFan(kSamsungAcFanHigh);
  EXPECT_EQ(kSamsungAcFanHigh, samsung.getFan());
}

TEST(TestIRSamsungAcClass, SetAndGetQuiet) {
  IRSamsungAc samsung(0);
  samsung.setQuiet(false);
  EXPECT_FALSE(samsung.getQuiet());
  samsung.setFan(kSamsungAcFanHigh);
  samsung.setQuiet(true);
  EXPECT_TRUE(samsung.getQuiet());
  EXPECT_EQ(kSamsungAcFanAuto, samsung.getFan());
  samsung.setQuiet(false);
  EXPECT_FALSE(samsung.getQuiet());
}

TEST(TestIRSamsungAcClass, ChecksumCalculation) {
  IRSamsungAc samsung(0);

  const uint8_t originalstate[kSamsungAcStateLength] = {
      0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0,
      0x01, 0x02, 0xAF, 0x71, 0x00, 0x15, 0xF0};
  uint8_t examplestate[kSamsungAcStateLength] = {0x02, 0x92, 0x0F, 0x00, 0x00,
                                                 0x00, 0xF0, 0x01, 0x02, 0xAF,
                                                 0x71, 0x00, 0x15, 0xF0};

  EXPECT_TRUE(IRSamsungAc::validChecksum(examplestate));
  EXPECT_EQ(0, IRSamsungAc::calcChecksum(examplestate));

  examplestate[8] = 0x12;  // Set an incoorect checksum.
  EXPECT_FALSE(IRSamsungAc::validChecksum(examplestate));
  EXPECT_EQ(0, IRSamsungAc::calcChecksum(examplestate));
  samsung.setRaw(examplestate);
  // Extracting the state from the object should have a correct checksum.
  EXPECT_TRUE(IRSamsungAc::validChecksum(samsung.getRaw()));
  EXPECT_STATE_EQ(originalstate, samsung.getRaw(), kSamsungAcBits);
  examplestate[8] = 0x02;  // Restore old checksum value.

  // Change the state to force a different checksum.
  examplestate[11] = 0x01;
  EXPECT_FALSE(IRSamsungAc::validChecksum(examplestate));
  EXPECT_EQ(0xF, IRSamsungAc::calcChecksum(examplestate));
}

TEST(TestIRSamsungAcClass, HumanReadable) {
  IRSamsungAc samsung(0);
  EXPECT_EQ(
      "Power: On, Mode: 1 (COOL), Temp: 16C, Fan: 2 (LOW), Swing: On, "
      "Beep: Off, Clean: Off, Quiet: Off",
      samsung.toString());
  samsung.setTemp(kSamsungAcMaxTemp);
  samsung.setMode(kSamsungAcHeat);
  samsung.off();
  samsung.setFan(kSamsungAcFanHigh);
  samsung.setSwing(false);
  samsung.setBeep(true);
  samsung.setClean(true);
  EXPECT_EQ(
      "Power: Off, Mode: 4 (HEAT), Temp: 30C, Fan: 5 (HIGH), Swing: Off, "
      "Beep: On, Clean: On, Quiet: Off",
      samsung.toString());
  samsung.setQuiet(true);
  EXPECT_EQ(
      "Power: Off, Mode: 4 (HEAT), Temp: 30C, Fan: 0 (AUTO), Swing: Off, "
      "Beep: On, Clean: On, Quiet: On",
      samsung.toString());
}

TEST(TestIRSamsungAcClass, GeneralConstruction) {
  IRSamsungAc samsung(0);

  uint8_t OnCoolFAutoBOffCOffQOffT20Soff[kSamsungAcStateLength] = {
      0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0,
      0x01, 0xE2, 0xFE, 0x71, 0x40, 0x11, 0xF0};

  samsung.setPower(true);
  samsung.setMode(kSamsungAcCool);
  samsung.setFan(kSamsungAcFanAuto);
  samsung.setSwing(false);
  samsung.setBeep(false);
  samsung.setClean(false);
  samsung.setQuiet(false);
  samsung.setTemp(20);
  EXPECT_STATE_EQ(OnCoolFAutoBOffCOffQOffT20Soff, samsung.getRaw(),
                  kSamsungAcBits);
  /* Disabled until we understand why LSB bit of the swing byte changes.
  // TODO(Hollako): Explain why sometimes the LSB of state[9] is a 1.
  // e.g. 0xAE or 0XAF for swing move.
  uint8_t OnHeatFAutoBOffCOffQOffT17Son[kSamsungAcStateLength] = {
      0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0,
      0x01, 0x02, 0xAF, 0x71, 0x10, 0x41, 0xF0};
  samsung.setPower(true);
  samsung.setMode(kSamsungAcHeat);
  samsung.setFan(kSamsungAcFanAuto);
  samsung.setSwing(true);
  samsung.setBeep(false);
  samsung.setClean(false);
  samsung.setQuiet(false);
  samsung.setTemp(17);
  EXPECT_STATE_EQ(OnHeatFAutoBOffCOffQOffT17Son, samsung.getRaw(),
                  kSamsungAcBits);
  */
}
// Tests for decodeSamsungAC().

// Decode normal SamsungAC messages.
TEST(TestDecodeSamsungAC, SyntheticDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();
  irsend.reset();
  uint8_t expectedState[kSamsungAcStateLength] = {0x02, 0x92, 0x0F, 0x00, 0x00,
                                                  0x00, 0xF0, 0x01, 0x02, 0xAF,
                                                  0x71, 0x00, 0x15, 0xF0};
  // Synthesised Normal Samsung A/C message.
  irsend.sendSamsungAC(expectedState);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SAMSUNG_AC, irsend.capture.decode_type);
  EXPECT_EQ(kSamsungAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
}

// Decode a real Samsung A/C example from Issue #505
TEST(TestDecodeSamsungAC, DecodeRealExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Samsung A/C example from Issue #505
  uint16_t rawData[233] = {
      690, 17844, 3084, 8864, 606, 406,  586, 1410, 580, 436,  570,  424,
      570, 426,   570,  404,  596, 418,  580, 416,  584, 410,  586,  1402,
      588, 408,   586,  410,  584, 1380, 610, 408,  586, 408,  586,  1404,
      586, 1404,  586,  1408, 594, 1396, 596, 1394, 602, 418,  582,  410,
      586, 408,   584,  408,  586, 408,  586, 410,  586, 408,  586,  410,
      586, 408,   586,  408,  586, 408,  586, 408,  586, 410,  584,  436,
      558, 436,   570,  424,  570, 424,  574, 420,  578, 416,  582,  412,
      586, 410,   586,  408,  584, 410,  586, 408,  586, 410,  584,  410,
      584, 408,   586,  408,  586, 410,  586, 408,  586, 412,  584,  436,
      556, 1410,  592,  1396, 602, 1390, 608, 1384, 608, 2886, 3086, 8858,
      610, 1380,  610,  410,  586, 408,  586, 410,  586, 408,  586,  410,
      586, 408,   586,  436,  558, 436,  554, 1410, 594, 426,  572,  422,
      578, 418,   582,  412,  586, 410,  584, 410,  586, 1380, 610,  1382,
      608, 1404,  586,  1404, 586, 408,  586, 1432, 558, 436,  554,  1414,
      590, 1398,  602,  418,  580, 414,  586, 410,  584, 1382, 606,  1382,
      608, 1382,  608,  408,  586, 408,  586, 408,  586, 408,  586,  410,
      584, 436,   560,  434,  570, 426,  566, 430,  568, 1400, 600,  416,
      584, 1406,  586,  410,  584, 1384, 606, 410,  586, 410,  584,  408,
      586, 408,   586,  408,  586, 408,  588, 410,  584, 1408, 590,  1400,
      592, 1398,  602,  1388, 612};
  uint8_t expectedState[kSamsungAcStateLength] = {0x02, 0x92, 0x0F, 0x00, 0x00,
                                                  0x00, 0xF0, 0x01, 0x02, 0xAF,
                                                  0x71, 0x00, 0x15, 0xF0};

  irsend.sendRaw(rawData, 233, 38000);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(SAMSUNG_AC, irsend.capture.decode_type);
  EXPECT_EQ(kSamsungAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);

  IRSamsungAc samsung(0);
  samsung.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: On, Mode: 1 (COOL), Temp: 16C, Fan: 2 (LOW), Swing: On, "
      "Beep: Off, Clean: Off, Quiet: Off",
      samsung.toString());
}

// Decode a real Samsung A/C example from Issue #505
TEST(TestDecodeSamsungAC, DecodeRealExample2) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Samsung A/C example from Issue #505
  uint16_t rawData[233] = {
      668, 17834, 3092, 8862, 608, 410,  586, 1378, 612, 410,  584,  410,
      586, 410,   584,  410,  586, 408,  586, 408,  586, 410,  586,  1404,
      588, 436,   558,  436,  570, 1398, 592, 424,  576, 420,  578,  1388,
      608, 1382,  610,  1382, 608, 1380, 610, 1384, 606, 408,  586,  408,
      588, 408,   588,  408,  586, 436,  558, 436,  570, 424,  570,  426,
      572, 422,   578,  418,  582, 412,  586, 408,  586, 410,  584,  410,
      584, 410,   584,  410,  586, 410,  586, 408,  586, 408,  586,  408,
      586, 408,   586,  408,  586, 438,  558, 436,  568, 426,  570,  424,
      574, 422,   576,  418,  582, 414,  584, 410,  586, 410,  584,  410,
      586, 1380,  610,  1382, 608, 1404, 586, 1404, 602, 2872, 3096, 8878,
      582, 1432,  570,  426,  568, 426,  574, 420,  578, 416,  582,  412,
      586, 410,   584,  410,  586, 410,  586, 1382, 608, 410,  586,  410,
      586, 408,   586,  1404, 586, 1408, 582, 1410, 590, 428,  568,  1400,
      598, 1394,  606,  1382, 610, 1382, 608, 1378, 612, 1382, 608,  1384,
      606, 1404,  586,  408,  586, 414,  582, 436,  558, 1410, 590,  1422,
      576, 1390,  608,  410,  586, 410,  586, 410,  584, 410,  584,  410,
      586, 410,   586,  410,  584, 410,  586, 1404, 586, 1404, 588,  436,
      560, 436,   486,  510,  566, 1400, 598, 420,  576, 418,  582,  414,
      586, 410,   584,  410,  584, 410,  586, 410,  584, 1382, 608,  1384,
      606, 1384,  606,  1408, 600};
  uint8_t expectedState[kSamsungAcStateLength] = {0x02, 0x92, 0x0F, 0x00, 0x00,
                                                  0x00, 0xF0, 0x01, 0xE2, 0xFE,
                                                  0x71, 0x80, 0x11, 0xF0};

  irsend.sendRaw(rawData, 233, 38000);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(SAMSUNG_AC, irsend.capture.decode_type);
  EXPECT_EQ(kSamsungAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);

  IRSamsungAc samsung(0);
  samsung.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: On, Mode: 1 (COOL), Temp: 24C, Fan: 0 (AUTO), Swing: Off, "
      "Beep: Off, Clean: Off, Quiet: Off",
      samsung.toString());
}

// Decode a real Samsung A/C example from:
// https://github.com/markszabo/IRremoteESP8266/issues/505#issuecomment-424036602
TEST(TestDecodeSamsungAC, DecodePowerOnSample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  uint16_t rawData[349] = {
      662, 17870, 3026, 8966, 540, 484,  514,  1482, 518, 482,  514,  482,
      518, 482,   516,  510,  490, 508,  490,  508,  572, 428,  576,  1398,
      542, 482,   514,  484,  514, 1460, 540,  482,  518, 482,  516,  1456,
      544, 1480,  518,  1480, 518, 1480, 518,  1484, 514, 510,  566,  432,
      576, 424,   574,  426,  540, 458,  516,  482,  516, 482,  516,  482,
      518, 480,   518,  482,  518, 482,  518,  482,  516, 482,  518,  482,
      516, 482,   518,  480,  516, 508,  492,  508,  490, 508,  572,  428,
      576, 422,   572,  428,  542, 456,  514,  484,  518, 480,  518,  480,
      518, 480,   516,  482,  516, 482,  520,  478,  518, 482,  518,  480,
      518, 1480,  518,  1480, 516, 1484, 594,  1428, 518, 2964, 3032, 8964,
      540, 1458,  542,  480,  518, 480,  520,  480,  518, 482,  520,  480,
      520, 478,   518,  480,  520, 478,  520,  1478, 522, 478,  518,  506,
      494, 1484,  594,  426,  574, 1400, 564,  1434, 540, 1454, 544,  1478,
      520, 1454,  544,  1458, 540, 480,  520,  480,  518, 480,  520,  480,
      518, 508,   490,  506,  568, 432,  572,  426,  576, 424,  544,  454,
      518, 480,   516,  482,  520, 478,  520,  478,  522, 478,  518,  480,
      520, 478,   520,  478,  520, 478,  520,  478,  520, 478,  518,  478,
      522, 506,   494,  504,  566, 432,  576,  424,  576, 424,  570,  428,
      518, 482,   518,  480,  518, 482,  520,  478,  520, 478,  520,  480,
      520, 478,   520,  478,  520, 2964, 3032, 8986, 520, 1478, 520,  506,
      492, 506,   492,  506,  568, 430,  574,  424,  546, 454,  516,  482,
      518, 482,   518,  1456, 544, 478,  546,  452,  520, 478,  544,  1432,
      542, 1478,  520,  1478, 520, 478,  520,  1482, 586, 1412, 598,  1400,
      564, 1432,  540,  1458, 544, 1454, 544,  1454, 544, 1456, 542,  480,
      518, 480,   520,  480,  520, 1462, 536,  1482, 588, 1410, 598,  424,
      572, 426,   542,  456,  518, 482,  520,  478,  520, 478,  522,  478,
      520, 478,   520,  1456, 542, 1458, 540,  478,  520, 478,  520,  478,
      520, 1482,  540,  482,  568, 430,  576,  424,  570, 428,  542,  458,
      518, 480,   520,  480,  520, 1454, 568,  1430, 566, 1432, 566,  1454,
      594};
  uint8_t expectedState[kSamsungAcExtendedStateLength] = {
      0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0, 0x01, 0xD2, 0x0F, 0x00,
      0x00, 0x00, 0x00, 0x01, 0xE2, 0xFE, 0x71, 0x80, 0x11, 0xF0};

  irsend.sendRaw(rawData, 349, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(SAMSUNG_AC, irsend.capture.decode_type);
  EXPECT_EQ(kSamsungAcExtendedBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);

  IRSamsungAc samsung(0);
  samsung.setRaw(irsend.capture.state, kSamsungAcExtendedStateLength);
  EXPECT_EQ(
      "Power: On, Mode: 1 (COOL), Temp: 24C, Fan: 0 (AUTO), Swing: Off, "
      "Beep: Off, Clean: Off, Quiet: Off",
      samsung.toString());
}

// Decode a real Samsung A/C example from:
// https://github.com/markszabo/IRremoteESP8266/issues/505#issuecomment-424036602
TEST(TestDecodeSamsungAC, DecodePowerOffSample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  uint16_t rawData[349] = {
      670, 17802, 3096, 8898, 602, 420,  580,  1418, 582, 418,  582,  416,
      582, 416,   584,  442,  550, 448,  568,  430,  570, 430,  576,  1396,
      600, 424,   546,  452,  578, 1394, 604,  1396, 600, 420,  580,  1398,
      602, 1416,  586,  1412, 582, 1420, 576,  1422, 592, 428,  574,  424,
      576, 422,   578,  420,  548, 452,  578,  420,  578, 420,  584,  416,
      580, 418,   580,  418,  582, 418,  580,  418,  582, 414,  584,  414,
      584, 416,   582,  418,  584, 442,  558,  442,  568, 430,  576,  424,
      578, 420,   576,  424,  576, 422,  580,  420,  584, 414,  584,  416,
      584, 414,   582,  418,  580, 418,  582,  416,  582, 416,  584,  414,
      584, 414,   586,  442,  554, 1420, 570,  1452, 578, 2884, 3120, 8898,
      596, 1400,  602,  422,  582, 418,  584,  414,  582, 416,  584,  414,
      584, 416,   582,  416,  584, 416,  584,  1410, 586, 414,  582,  444,
      556, 1420,  590,  432,  572, 1402, 602,  1396, 600, 1398, 606,  1414,
      582, 1394,  604,  1394, 604, 414,  584,  414,  586, 412,  586,  410,
      586, 442,   556,  440,  544, 456,  568,  430,  576, 424,  578,  420,
      578, 420,   576,  424,  584, 412,  586,  412,  586, 412,  584,  414,
      586, 412,   584,  414,  586, 412,  586,  412,  586, 414,  586,  412,
      584, 442,   558,  442,  558, 440,  566,  432,  574, 424,  578,  422,
      576, 422,   578,  420,  586, 414,  586,  414,  586, 412,  584,  414,
      586, 414,   586,  414,  586, 2902, 3096, 8900, 600, 1416, 586,  442,
      556, 442,   558,  440,  564, 434,  572,  428,  578, 420,  580,  420,
      578, 420,   584,  1392, 608, 414,  586,  414,  582, 414,  586,  412,
      586, 412,   586,  414,  584, 1394, 606,  1416, 580, 1418, 568,  1432,
      594, 1402,  602,  1398, 606, 1392, 606,  1390, 608, 1390, 608,  414,
      584, 414,   586,  414,  584, 1412, 586,  1398, 600, 1418, 590,  430,
      566, 432,   576,  422,  578, 420,  578,  422,  582, 416,  586,  414,
      586, 412,   584,  1390, 606, 1392, 608,  414,  586, 412,  584,  412,
      588, 1410,  586,  442,  558, 440,  568,  430,  566, 434,  574,  426,
      578, 420,   578,  420,  582, 416,  586,  412,  586, 1390, 608,  1390,
      608};

  uint8_t expectedState[kSamsungAcExtendedStateLength] = {
      0x02, 0xB2, 0x0F, 0x00, 0x00, 0x00, 0xC0, 0x01, 0xD2, 0x0F, 0x00,
      0x00, 0x00, 0x00, 0x01, 0x02, 0xFF, 0x71, 0x80, 0x11, 0xC0};

  irsend.sendRaw(rawData, 349, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(SAMSUNG_AC, irsend.capture.decode_type);
  EXPECT_EQ(kSamsungAcExtendedBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);

  IRSamsungAc samsung(0);
  samsung.setRaw(irsend.capture.state, kSamsungAcExtendedStateLength);
  EXPECT_EQ(
      "Power: Off, Mode: 1 (COOL), Temp: 24C, Fan: 0 (AUTO), Swing: Off, "
      "Beep: Off, Clean: Off, Quiet: Off",
      samsung.toString());
}

TEST(TestDecodeSamsungAC, DecodeHeatSample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  uint16_t rawData[233] = {
      650, 16260, 3014, 8934, 534, 486,  508, 1478, 514, 484,  510,  486,
      508, 512,   484,  510,  562, 432,  572, 422,  540, 454,  514,  1452,
      534, 486,   510,  484,  510, 1456, 536, 484,  510, 484,  510,  1454,
      536, 1478,  512,  1476, 514, 1482, 508, 1482, 592, 428,  570,  424,
      538, 456,   508,  486,  510, 484,  512, 484,  510, 486,  510,  484,
      510, 484,   510,  486,  510, 484,  510, 484,  510, 484,  510,  484,
      510, 484,   510,  486,  508, 510,  484, 510,  568, 428,  570,  424,
      538, 458,   512,  482,  510, 486,  510, 484,  510, 484,  510,  484,
      510, 484,   510,  484,  510, 484,  510, 484,  510, 484,  510,  484,
      510, 1474,  516,  1502, 534, 1432, 594, 1398, 536, 2954, 3018, 8932,
      536, 1458,  532,  484,  510, 484,  512, 484,  510, 484,  510,  484,
      510, 484,   512,  484,  510, 484,  510, 1480, 508, 510,  530,  464,
      568, 426,   568,  426,  514, 480,  508, 486,  508, 1456, 534,  1478,
      514, 1452,  538,  1478, 512, 484,  510, 1456, 534, 486,  510,  1478,
      512, 1480,  570,  450,  570, 424,  540, 454,  512, 1452, 534,  1458,
      534, 1454,  536,  484,  512, 482,  512, 484,  512, 484,  512,  482,
      512, 1474,  514,  484,  512, 510,  486, 508,  534, 1430, 594,  426,
      512, 482,   512,  482,  510, 484,  512, 482,  512, 1452, 538,  482,
      512, 482,   512,  482,  510, 484,  510, 484,  510, 1478, 512,  1504,
      488, 1480,  560,  1454, 514};

  uint8_t expectedState[kSamsungAcStateLength] = {0x02, 0x92, 0x0F, 0x00, 0x00,
                                                  0x00, 0xF0, 0x01, 0x02, 0xAF,
                                                  0x71, 0x10, 0x41, 0xF0};

  irsend.sendRaw(rawData, 233, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(SAMSUNG_AC, irsend.capture.decode_type);
  EXPECT_EQ(kSamsungAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);

  IRSamsungAc samsung(0);
  samsung.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: On, Mode: 4 (HEAT), Temp: 17C, Fan: 0 (AUTO), Swing: On, "
      "Beep: Off, Clean: Off, Quiet: Off",
      samsung.toString());
}

TEST(TestDecodeSamsungAC, DecodeCoolSample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  uint16_t rawData[233] = {
      690, 17854, 3086, 8862, 610, 410,  584, 1382, 610, 410,  586,  408,
      586, 408,   586,  408,  588, 410,  584, 436,  558, 436,  570,  1396,
      598, 422,   576,  418,  580, 1384, 612, 410,  586, 410,  584,  1380,
      612, 1382,  608,  1384, 606, 1404, 586, 1404, 586, 436,  558,  438,
      566, 428,   568,  426,  570, 424,  576, 418,  578, 416,  584,  410,
      586, 408,   584,  410,  586, 408,  586, 408,  586, 410,  586,  408,
      586, 408,   588,  408,  586, 408,  588, 408,  586, 438,  558,  436,
      568, 426,   568,  428,  568, 426,  576, 418,  578, 416,  584,  412,
      584, 410,   586,  408,  586, 410,  586, 410,  584, 410,  586,  408,
      586, 1384,  606,  1402, 588, 1410, 580, 1410, 608, 2864, 3108, 8864,
      594, 1394,  604,  416,  584, 410,  586, 410,  586, 410,  586,  410,
      584, 410,   586,  410,  586, 408,  586, 1404, 588, 408,  586,  408,
      586, 436,   560,  1408, 592, 1400, 596, 1396, 600, 416,  584,  1382,
      608, 1380,  610,  1404, 586, 1384, 608, 1384, 606, 1402, 588,  1408,
      582, 1410,  564,  452,  568, 428,  572, 424,  576, 1414, 582,  1386,
      608, 1382,  608,  410,  584, 410,  584, 410,  586, 408,  586,  408,
      586, 408,   586,  408,  588, 1408, 582, 436,  540, 1426, 590,  428,
      574, 420,   578,  418,  580, 1384, 610, 410,  584, 410,  584,  410,
      584, 412,   584,  408,  586, 410,  586, 408,  586, 1404, 586,  1408,
      582, 1410,  562,  1426, 610};

  uint8_t expectedState[kSamsungAcStateLength] = {0x02, 0x92, 0x0F, 0x00, 0x00,
                                                  0x00, 0xF0, 0x01, 0xE2, 0xFE,
                                                  0x71, 0x40, 0x11, 0xF0};

  irsend.sendRaw(rawData, 233, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(SAMSUNG_AC, irsend.capture.decode_type);
  EXPECT_EQ(kSamsungAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);

  IRSamsungAc samsung(0);
  samsung.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: On, Mode: 1 (COOL), Temp: 20C, Fan: 0 (AUTO), Swing: Off, "
      "Beep: Off, Clean: Off, Quiet: Off",
      samsung.toString());
}
