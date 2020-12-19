// Copyright 2017-2020 David Conran

#include "ir_Sanyo.h"
#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for encodeSanyoLC7461().

TEST(TestEncodeSanyoLC7461, NormalEncoding) {
  IRsendTest irsend(4);
  EXPECT_EQ(0x1FFF00FF, irsend.encodeSanyoLC7461(0, 0));
  EXPECT_EQ(0x3FFE01FE, irsend.encodeSanyoLC7461(1, 1));
  EXPECT_EQ(0x3FFE02FD, irsend.encodeSanyoLC7461(1, 2));
  EXPECT_EQ(0x3FFE000FF00, irsend.encodeSanyoLC7461(0x1FFF, 0xFF));
  EXPECT_EQ(0x2AAAAAA55AA, irsend.encodeSanyoLC7461(0x1555, 0x55));
  EXPECT_EQ(0x3FFE000FF00, irsend.encodeSanyoLC7461(0xFFFF, 0xFF));
  EXPECT_EQ(0x1D8113F00FF, irsend.encodeSanyoLC7461(0xEC0, 0x0));
}

// Tests for sendSanyoLC7461().

// Test sending typical data only.
TEST(TestEncodeSanyoLC7461, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSanyoLC7461(0x1D8113F00FF);
  EXPECT_EQ(
      "f38000d33"
      "m8960s4480"
      "m560s560m560s1680m560s1680m560s1680m560s560m560s1680m560s1680m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s1680m560s560m560s560"
      "m560s560m560s1680m560s560m560s560m560s1680m560s1680m560s1680m560s1680"
      "m560s1680m560s1680m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s1680m560s1680m560s23520",
      irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestEncodeSanyoLC7461, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSanyoLC7461(0x1D8113F00FF, kSanyoLC7461Bits, 1);  // 1 repeat.
  EXPECT_EQ(
      "f38000d33"
      "m8960s4480"
      "m560s560m560s1680m560s1680m560s1680m560s560m560s1680m560s1680m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s1680m560s560m560s560"
      "m560s560m560s1680m560s560m560s560m560s1680m560s1680m560s1680m560s1680"
      "m560s1680m560s1680m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s1680m560s1680m560s23520"
      "m8960s2240m560s96320",
      irsend.outputStr());
}

// Tests for decodeSanyoLC7461().

// Decode normal Sanyo LC7461 messages.
TEST(TestDecodeSanyoLC7461, NormalDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Sanyo LC7461 42-bit message.
  irsend.reset();
  irsend.sendSanyoLC7461(0x1D8113F00FF);
  irsend.makeDecodeResult();
  ASSERT_TRUE(
      irrecv.decodeSanyoLC7461(&irsend.capture, kStartOffset, kSanyoLC7461Bits,
                               true));
  EXPECT_EQ(SANYO_LC7461, irsend.capture.decode_type);
  EXPECT_EQ(kSanyoLC7461Bits, irsend.capture.bits);
  EXPECT_EQ(0x1D8113F00FF, irsend.capture.value);
  EXPECT_EQ(0xEC0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Synthesised Normal Sanyo LC7461 42-bit message.
  irsend.reset();
  irsend.sendSanyoLC7461(irsend.encodeSanyoLC7461(0x1234, 0x56));
  irsend.makeDecodeResult();
  ASSERT_TRUE(
      irrecv.decodeSanyoLC7461(&irsend.capture, kStartOffset, kSanyoLC7461Bits,
                               true));
  EXPECT_EQ(SANYO_LC7461, irsend.capture.decode_type);
  EXPECT_EQ(kSanyoLC7461Bits, irsend.capture.bits);
  EXPECT_EQ(0x2468DCB56A9, irsend.capture.value);
  EXPECT_EQ(0x1234, irsend.capture.address);
  EXPECT_EQ(0x56, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Synthesised Normal Sanyo LC7461 42-bit message.
  irsend.reset();
  irsend.sendSanyoLC7461(irsend.encodeSanyoLC7461(0x1, 0x1));
  irsend.makeDecodeResult();
  ASSERT_TRUE(
      irrecv.decodeSanyoLC7461(&irsend.capture, kStartOffset, kSanyoLC7461Bits,
                               true));
  EXPECT_EQ(SANYO_LC7461, irsend.capture.decode_type);
  EXPECT_EQ(kSanyoLC7461Bits, irsend.capture.bits);
  EXPECT_EQ(0x3FFE01FE, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x1, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode normal repeated Sanyo LC7461 messages.
TEST(TestDecodeSanyoLC7461, NormalDecodeWithRepeatAndStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Sanyo LC7461 16-bit message with 1 repeat.
  irsend.reset();
  irsend.sendSanyoLC7461(0x3FFE01FE, kSanyoLC7461Bits, 1);
  irsend.makeDecodeResult();
  ASSERT_TRUE(
      irrecv.decodeSanyoLC7461(&irsend.capture, kStartOffset, kSanyoLC7461Bits,
                               true));
  EXPECT_EQ(SANYO_LC7461, irsend.capture.decode_type);
  EXPECT_EQ(kSanyoLC7461Bits, irsend.capture.bits);
  EXPECT_EQ(0x3FFE01FE, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x1, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode unsupported Sanyo LC7461 messages.
TEST(TestDecodeSanyoLC7461, DecodeWithNonStrictValues) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSanyoLC7461(0x0);  // Illegal value Sanyo LC7461 message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(
      irrecv.decodeSanyoLC7461(&irsend.capture, kStartOffset, kSanyoLC7461Bits,
                               true));
  // Should pass if strict off.
  ASSERT_TRUE(
      irrecv.decodeSanyoLC7461(&irsend.capture, kStartOffset, kSanyoLC7461Bits,
                               false));
  EXPECT_EQ(SANYO_LC7461, irsend.capture.decode_type);
  EXPECT_EQ(kSanyoLC7461Bits, irsend.capture.bits);
  EXPECT_EQ(0x0, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  // Illegal value Sanyo LC7461 42-bit message.
  irsend.sendSanyoLC7461(0x1234567890A);
  irsend.makeDecodeResult();
  ASSERT_FALSE(
      irrecv.decodeSanyoLC7461(&irsend.capture, kStartOffset, kSanyoLC7461Bits,
                               true));

  // Should fail with strict when we ask for the wrong bit size.
  ASSERT_FALSE(irrecv.decodeSanyoLC7461(&irsend.capture, kStartOffset, 32,
                                        true));
  ASSERT_FALSE(irrecv.decodeSanyoLC7461(&irsend.capture, kStartOffset, 64,
                                        true));
  // And should fail for a bad value.
  ASSERT_FALSE(
      irrecv.decodeSanyoLC7461(&irsend.capture, kStartOffset, kSanyoLC7461Bits,
                               true));
  // Should pass if strict off.
  ASSERT_TRUE(
      irrecv.decodeSanyoLC7461(&irsend.capture, kStartOffset, kSanyoLC7461Bits,
                               false));
  EXPECT_EQ(SANYO_LC7461, irsend.capture.decode_type);
  EXPECT_EQ(kSanyoLC7461Bits, irsend.capture.bits);
  EXPECT_EQ(0x1234567890A, irsend.capture.value);
  EXPECT_EQ(0x91A, irsend.capture.address);
  EXPECT_EQ(0x89, irsend.capture.command);

  // Shouldn't pass if strict off and looking for a smaller size.
  ASSERT_FALSE(irrecv.decodeSanyoLC7461(&irsend.capture, kStartOffset, 34,
                                        false));
}

// Decode (non-standard) 64-bit messages.
TEST(TestDecodeSanyoLC7461, Decode64BitMessages) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Illegal value & size Sanyo LC7461 64-bit message.
  irsend.sendSanyoLC7461(0xFFFFFFFFFFFFFFFF, 64);
  irsend.makeDecodeResult();
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeSanyoLC7461(&irsend.capture, kStartOffset, 64,
                                       false));
  EXPECT_EQ(SANYO_LC7461, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, irsend.capture.value);
  EXPECT_EQ(0xFFFF, irsend.capture.address);
  EXPECT_EQ(0xFF, irsend.capture.command);
}

// Decode a 'real' example via GlobalCache
TEST(TestDecodeSanyoLC7461, DecodeGlobalCacheExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  uint16_t gc_test[95] = {
      38000, 1,  89, 342, 171, 21, 21, 21, 64, 21, 64,  21,  64,  21, 21,  21,
      64,    21, 64, 21,  21,  21, 21, 21, 21, 21, 21,  21,  21,  21, 21,  21,
      64,    21, 21, 21,  21,  21, 21, 21, 64, 21, 21,  21,  21,  21, 64,  21,
      64,    21, 64, 21,  64,  21, 64, 21, 64, 21, 21,  21,  21,  21, 21,  21,
      21,    21, 21, 21,  21,  21, 21, 21, 21, 21, 64,  21,  64,  21, 64,  21,
      64,    21, 64, 21,  64,  21, 64, 21, 64, 21, 875, 342, 171, 21, 3565};
  irsend.sendGC(gc_test, 95);
  irsend.makeDecodeResult();

  ASSERT_TRUE(
      irrecv.decodeSanyoLC7461(&irsend.capture, kStartOffset, kSanyoLC7461Bits,
                               true));
  EXPECT_EQ(SANYO_LC7461, irsend.capture.decode_type);
  EXPECT_EQ(kSanyoLC7461Bits, irsend.capture.bits);
  EXPECT_EQ(0x1D8113F00FF, irsend.capture.value);
  EXPECT_EQ(0xEC0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Confirm what the 42-bit NEC decode is.
  ASSERT_TRUE(irrecv.decodeNEC(&irsend.capture, kStartOffset, 42, false));
  EXPECT_EQ(0x1D8113F00FF, irsend.capture.value);
}

// Fail to decode a non-Sanyo LC7461 example via GlobalCache
TEST(TestDecodeSanyoLC7461, FailToDecodeNonSanyoLC7461Example) {
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

  ASSERT_FALSE(irrecv.decodeSanyoLC7461(&irsend.capture));
  ASSERT_FALSE(
      irrecv.decodeSanyoLC7461(&irsend.capture, kStartOffset, kSanyoLC7461Bits,
                               false));
}

TEST(TestUtils, Housekeeping) {
  // Sanyo LC7461
  ASSERT_EQ("SANYO_LC7461", typeToString(decode_type_t::SANYO_LC7461));
  ASSERT_EQ(decode_type_t::SANYO_LC7461, strToDecodeType("SANYO_LC7461"));
  ASSERT_FALSE(hasACState(decode_type_t::SANYO_LC7461));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::SANYO_LC7461));
  ASSERT_EQ(kSanyoLC7461Bits, IRsend::defaultBits(decode_type_t::SANYO_LC7461));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::SANYO_LC7461));
  // Sanyo A/C
  ASSERT_EQ("SANYO_AC", typeToString(decode_type_t::SANYO_AC));
  ASSERT_EQ(decode_type_t::SANYO_AC, strToDecodeType("SANYO_AC"));
  ASSERT_TRUE(hasACState(decode_type_t::SANYO_AC));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::SANYO_AC));
  ASSERT_EQ(kSanyoAcBits, IRsend::defaultBits(decode_type_t::SANYO_AC));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::SANYO_AC));
}

TEST(TestDecodeSanyoAc, DecodeRealExamples) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  // Ref: "On" from https://github.com/crankyoldgit/IRremoteESP8266/issues/1211#issue-650997449
  const uint16_t rawData[148] = {
      8456, 4192,
      624, 448, 584, 1508, 608, 452, 580, 1512, 628, 452, 604, 1468, 560, 1552,
      600, 472, 584, 1532, 580, 472, 528, 516, 512, 540, 576, 1516, 628, 1472,
      612, 1508, 612, 452, 580, 1512, 628, 1468, 612, 1496, 632, 444, 580, 480,
      580, 476, 580, 1496, 564, 508, 576, 480, 576, 480, 580, 476, 584, 472,
      584, 468, 584, 480, 520, 512, 580, 480, 576, 480, 580, 476, 584, 472,
      584, 472, 528, 508, 524, 1568, 600, 480, 576, 480, 584, 1492, 560, 512,
      580, 1536, 576, 480, 580, 476, 580, 476, 528, 528, 524, 1568, 580, 476,
      584, 476, 580, 476, 580, 472, 528, 512, 520, 536, 576, 480, 580, 480,
      576, 480, 576, 476, 532, 528, 520, 512, 576, 480, 584, 476, 580, 476,
      580, 480, 576, 472, 528, 1548, 600, 480, 576, 480, 576, 1520, 592, 1496,
      600, 476, 580, 480, 576};
  const uint8_t expectedState[kSanyoAcStateLength] = {
        0x6A, 0x71, 0x47, 0x00, 0x20, 0x85, 0x00, 0x00, 0x32};
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 148, 38000);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SANYO_AC, irsend.capture.decode_type);
  EXPECT_EQ(kSanyoAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_EQ(
      "Power: On, Mode: 2 (Cool), Temp: 21C, Fan: 0 (Auto), "
      "Swing(V): 5 (Upper Middle), Sleep: Off, Beep: On, Sensor: Room, "
      "Sensor Temp: 11C, Off Timer: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
}

TEST(TestDecodeSanyoAc, SyntheticSelfDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  const uint8_t expectedState[kSanyoAcStateLength] = {
        0x6A, 0x71, 0x47, 0x00, 0x20, 0x85, 0x00, 0x00, 0x32};
  irsend.begin();
  irsend.reset();
  irsend.sendSanyoAc(expectedState);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SANYO_AC, irsend.capture.decode_type);
  EXPECT_EQ(kSanyoAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_EQ(
      "Power: On, Mode: 2 (Cool), Temp: 21C, Fan: 0 (Auto), "
      "Swing(V): 5 (Upper Middle), Sleep: Off, Beep: On, Sensor: Room, "
      "Sensor Temp: 11C, Off Timer: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  EXPECT_EQ(
      "f38000d50"
      "m8500s4200"
      "m500s550m500s1600m500s550m500s1600m500s550m500s1600m500s1600m500s550"
      "m500s1600m500s550m500s550m500s550m500s1600m500s1600m500s1600m500s550"
      "m500s1600m500s1600m500s1600m500s550m500s550m500s550m500s1600m500s550"
      "m500s550m500s550m500s550m500s550m500s550m500s550m500s550m500s550m500s550"
      "m500s550m500s550m500s550m500s550m500s1600m500s550m500s550m500s1600"
      "m500s550m500s1600m500s550m500s550m500s550m500s550m500s1600m500s550"
      "m500s550m500s550m500s550m500s550m500s550m500s550m500s550m500s550m500s550"
      "m500s550m500s550m500s550m500s550m500s550m500s550m500s550m500s1600"
      "m500s550m500s550m500s1600m500s1600m500s550m500s550"
      "m500s100000",
      irsend.outputStr());
}

// Tests for IRSanyoAc class.

TEST(TestSanyoAcClass, Power) {
  IRSanyoAc ac(kGpioUnused);
  ac.begin();

  ac.on();
  EXPECT_TRUE(ac.getPower());

  ac.off();
  EXPECT_FALSE(ac.getPower());

  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());

  ac.setPower(false);
  EXPECT_FALSE(ac.getPower());
}

TEST(TestSanyoAcClass, Temperature) {
  IRSanyoAc ac(kGpioUnused);
  ac.begin();

  ac.setTemp(0);
  EXPECT_EQ(kSanyoAcTempMin, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kSanyoAcTempMax, ac.getTemp());

  ac.setTemp(kSanyoAcTempMin);
  EXPECT_EQ(kSanyoAcTempMin, ac.getTemp());

  ac.setTemp(kSanyoAcTempMax);
  EXPECT_EQ(kSanyoAcTempMax, ac.getTemp());

  ac.setTemp(kSanyoAcTempMin - 1);
  EXPECT_EQ(kSanyoAcTempMin, ac.getTemp());

  ac.setTemp(kSanyoAcTempMax + 1);
  EXPECT_EQ(kSanyoAcTempMax, ac.getTemp());

  ac.setTemp(17);
  EXPECT_EQ(17, ac.getTemp());

  ac.setTemp(21);
  EXPECT_EQ(21, ac.getTemp());

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());

  ac.setTemp(30);
  EXPECT_EQ(30, ac.getTemp());
}

TEST(TestSanyoAcClass, OperatingMode) {
  IRSanyoAc ac(kGpioUnused);
  ac.begin();

  ac.setMode(kSanyoAcAuto);
  EXPECT_EQ(kSanyoAcAuto, ac.getMode());

  ac.setMode(kSanyoAcCool);
  EXPECT_EQ(kSanyoAcCool, ac.getMode());

  ac.setMode(kSanyoAcHeat);
  EXPECT_EQ(kSanyoAcHeat, ac.getMode());

  ac.setMode(kSanyoAcDry);
  EXPECT_EQ(kSanyoAcDry, ac.getMode());

  ac.setMode(kSanyoAcAuto + 1);
  EXPECT_EQ(kSanyoAcAuto, ac.getMode());

  ac.setMode(0);
  EXPECT_EQ(kSanyoAcAuto, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kSanyoAcAuto, ac.getMode());
}

TEST(TestSanyoAcClass, FanSpeed) {
  IRSanyoAc ac(kGpioUnused);
  ac.begin();

  ac.setFan(kSanyoAcFanAuto);
  EXPECT_EQ(kSanyoAcFanAuto, ac.getFan());

  ac.setFan(kSanyoAcFanHigh);
  EXPECT_EQ(kSanyoAcFanHigh, ac.getFan());

  ac.setFan(kSanyoAcFanLow);
  EXPECT_EQ(kSanyoAcFanLow, ac.getFan());

  ac.setFan(kSanyoAcFanMedium);
  EXPECT_EQ(kSanyoAcFanMedium, ac.getFan());
}

TEST(TestSanyoAcClass, Sleep) {
  IRSanyoAc ac(kGpioUnused);
  ac.begin();

  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
  ac.setSleep(false);
  EXPECT_FALSE(ac.getSleep());
  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
}

TEST(TestSanyoAcClass, SwingV) {
  IRSanyoAc ac(kGpioUnused);
  ac.begin();

  ac.setSwingV(kSanyoAcSwingVAuto);
  EXPECT_EQ(kSanyoAcSwingVAuto, ac.getSwingV());

  ac.setSwingV(kSanyoAcSwingVHigh);
  EXPECT_EQ(kSanyoAcSwingVHigh, ac.getSwingV());

  ac.setSwingV(kSanyoAcSwingVLow);
  EXPECT_EQ(kSanyoAcSwingVLow, ac.getSwingV());

  ac.setSwingV(kSanyoAcSwingVUpperMiddle);
  EXPECT_EQ(kSanyoAcSwingVUpperMiddle, ac.getSwingV());

  ac.setSwingV(0);
  EXPECT_EQ(kSanyoAcSwingVAuto, ac.getSwingV());
  ac.setSwingV(255);
  EXPECT_EQ(kSanyoAcSwingVAuto, ac.getSwingV());
}

TEST(TestSanyoAcClass, Timers) {
  IRSanyoAc ac(kGpioUnused);
  ac.begin();

  ac.setOffTimer(0);
  EXPECT_EQ(0, ac.getOffTimer());
  ac.setOffTimer(59);
  EXPECT_EQ(0, ac.getOffTimer());
  ac.setOffTimer(60);
  EXPECT_EQ(60, ac.getOffTimer());
  ac.setOffTimer(61);
  EXPECT_EQ(60, ac.getOffTimer());
  ac.setOffTimer(15 * 60 + 59);
  EXPECT_EQ(15 * 60, ac.getOffTimer());
  ac.setOffTimer(16 * 60);
  EXPECT_EQ(15 * 60, ac.getOffTimer());

  const uint8_t offTimer2Hr[kSanyoAcStateLength] = {
      0x6A, 0x6D, 0x4F, 0x02, 0x14, 0x85, 0x00, 0x00, 0x4A};
  ac.setRaw(offTimer2Hr);
  EXPECT_EQ(2 * 60, ac.getOffTimer());
  EXPECT_EQ(
      "Power: On, Mode: 1 (Heat), Temp: 17C, Fan: 0 (Auto), "
      "Swing(V): 5 (Upper Middle), Sleep: Off, Beep: On, "
      "Sensor: Room, Sensor Temp: 19C, Off Timer: 02:00",
      ac.toString());
}

TEST(TestSanyoAcClass, Beep) {
  IRSanyoAc ac(kGpioUnused);
  ac.begin();

  ac.setBeep(true);
  EXPECT_TRUE(ac.getBeep());
  ac.setBeep(false);
  EXPECT_FALSE(ac.getBeep());
  ac.setBeep(true);
  EXPECT_TRUE(ac.getBeep());

  const uint8_t beep_off[kSanyoAcStateLength] = {
      0x6A, 0x6D, 0x11, 0x00, 0x10, 0x85, 0x00, 0x00, 0x33};
  ac.setRaw(beep_off);
  EXPECT_FALSE(ac.getBeep());
  const uint8_t beep_on[kSanyoAcStateLength] = {
      0x6A, 0x6E, 0x54, 0x00, 0x10, 0x83, 0x00, 0x00, 0x39};
  ac.setRaw(beep_on);
  EXPECT_TRUE(ac.getBeep());
}
