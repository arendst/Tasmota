// Copyright 2017-2021 David Conran

#include "ir_Sanyo.h"
#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for encodeSanyoLC7461().

TEST(TestEncodeSanyoLC7461, NormalEncoding) {
  IRsendTest irsend(kGpioUnused);
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
  IRsendTest irsend(kGpioUnused);
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
  IRsendTest irsend(kGpioUnused);
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
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
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
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
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
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
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
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
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
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
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
  // Sanyo A/C 88 Bit.
  ASSERT_EQ("SANYO_AC88", typeToString(decode_type_t::SANYO_AC88));
  ASSERT_EQ(decode_type_t::SANYO_AC88, strToDecodeType("SANYO_AC88"));
  ASSERT_TRUE(hasACState(decode_type_t::SANYO_AC88));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::SANYO_AC88));
  ASSERT_EQ(kSanyoAc88Bits, IRsend::defaultBits(decode_type_t::SANYO_AC88));
  ASSERT_EQ(kSanyoAc88MinRepeat, IRsend::minRepeats(decode_type_t::SANYO_AC88));
  // Sanyo A/C 152 Bit.
  ASSERT_EQ("SANYO_AC152", typeToString(decode_type_t::SANYO_AC152));
  ASSERT_EQ(decode_type_t::SANYO_AC152, strToDecodeType("SANYO_AC152"));
  ASSERT_TRUE(hasACState(decode_type_t::SANYO_AC152));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::SANYO_AC152));
  ASSERT_EQ(kSanyoAc152Bits, IRsend::defaultBits(decode_type_t::SANYO_AC152));
  ASSERT_EQ(kSanyoAc152MinRepeat,
            IRsend::minRepeats(decode_type_t::SANYO_AC152));
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
      "Swing(V): 5 (Upper Middle), Sleep: Off, Beep: On, Sensor: Wall, "
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
      "Swing(V): 5 (Upper Middle), Sleep: Off, Beep: On, Sensor: Wall, "
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
      "Sensor: Wall, Sensor Temp: 19C, Off Timer: 02:00",
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

TEST(TestDecodeSanyoAc88, DecodeRealExamples) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  // Ref: "On" from https://github.com/crankyoldgit/IRremoteESP8266/issues/1503#issuecomment-868450739
  const uint16_t rawData[539] = {5374, 1972, 512, 762, 522, 1510, 498, 780, 578,
      1486, 462, 790, 462, 1550, 544, 708, 510, 1500, 544, 1470, 516, 762, 482,
      770, 516, 1496, 518, 1510, 498, 780, 468, 1544, 466, 786, 466, 786, 466,
      786, 466, 786, 486, 748, 526, 756, 506, 1520, 516, 764, 516, 1492, 518,
      762, 544, 708, 548, 704, 602, 1434, 556, 1456, 524, 726, 552, 700, 554,
      698, 648, 1438, 548, 684, 536, 1478, 568, 682, 568, 682, 578, 702, 568,
      684, 570, 712, 624, 724, 528, 1482, 528, 726, 528, 1482, 526, 724, 546,
      1524, 568, 682, 540, 710, 544, 734, 542, 1516, 492, 762, 488, 744, 546,
      1482, 492, 788, 544, 734, 494, 758, 492, 760, 490, 744, 548, 828, 494,
      760, 494, 758, 494, 758, 494, 786, 492, 760, 494, 758, 494, 742, 508, 746,
      508, 788, 492, 760, 494, 786, 494, 786, 494, 742, 508, 744, 508, 788, 570,
      760, 494, 784, 494, 788, 568, 786, 492, 744, 508, 772, 608, 768, 538, 714,
      490, 786, 628, 728, 494, 786, 494, 758, 494, 742, 508, 1390, 494, 3692,
      5394, 1960, 494, 784, 494, 1516, 494, 760, 494, 1516, 494, 760, 492, 1516,
      494, 760, 492, 1518, 524, 1486, 494, 790, 486, 764, 492, 1524, 538, 1472,
      540, 740, 538, 1546, 546, 728, 524, 726, 526, 728, 524, 728, 526, 728,
      524, 728, 524, 1486, 526, 726, 546, 1464, 550, 702, 550, 728, 552, 702,
      550, 1460, 550, 1460, 550, 702, 552, 728, 550, 702, 550, 1460, 550, 728,
      550, 1464, 548, 704, 546, 704, 548, 732, 546, 704, 548, 704, 548, 730,
      548, 1462, 548, 706, 518, 1492, 546, 706, 518, 1490, 546, 706, 520, 732,
      520, 732, 522, 1492, 518, 760, 518, 732, 546, 1468, 516, 736, 516, 732,
      520, 732, 520, 758, 520, 818, 478, 772, 480, 802, 454, 894, 520, 734, 518,
      734, 518, 760, 518, 734, 518, 734, 518, 718, 506, 746, 480, 818, 488, 790,
      490, 744, 508, 790, 490, 744, 510, 770, 508, 788, 490, 762, 544, 788, 490,
      788, 462, 818, 566, 788, 490, 744, 480, 770, 456, 794, 458, 894, 492, 762,
      492, 760, 490, 744, 508, 1390, 492, 3664,
      5398, 1960, 490, 746, 476, 1552, 492, 742, 508, 1504, 508, 746, 504, 1522,
      494, 786, 492, 1516, 494, 1516, 494, 760, 492, 786, 494, 1520, 490, 1520,
      458, 822, 458, 1554, 510, 788, 494, 784, 494, 760, 492, 742, 508, 770,
      512, 786, 492, 1516, 520, 760, 520, 1488, 520, 734, 520, 760, 518, 734,
      520, 1490, 520, 1494, 516, 734, 518, 732, 520, 762, 516, 762, 488, 1522,
      458, 1554, 506, 774, 506, 744, 508, 774, 508, 770, 510, 742, 506, 746,
      540, 1472, 506, 744, 508, 1522, 488, 764, 488, 1504, 538, 758, 520, 734,
      520, 760, 520, 1490, 518, 734, 520, 734, 520, 1490, 520, 760, 518, 760,
      538, 770, 488, 764, 516, 734, 518, 734, 518, 818, 476, 802, 476, 774, 478,
      904, 538, 734, 520, 762, 516, 818, 510, 748, 476, 772, 458, 794, 458, 792,
      516, 736, 516, 736, 516, 736, 516, 738, 514, 762, 516, 764, 514, 738, 516,
      738, 538, 766, 516, 738, 514, 738, 514, 766, 512, 794, 486, 768, 486, 794,
      486, 766, 512, 738, 488, 1398, 504};  // UNKNOWN ABF4C698

  const uint8_t expectedState[kSanyoAc88StateLength] = {
        0xAA, 0x59, 0xA0, 0x18, 0x06, 0x2A, 0x12, 0x00, 0x00, 0x00, 0x80};
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 539, 38000);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(SANYO_AC88, irsend.capture.decode_type);
  EXPECT_EQ(kSanyoAc88Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_EQ(
      "Power: On, Mode: 2 (Cool), Temp: 24C, Fan: 0 (Auto), Swing(V): Off, "
      "Turbo: Off, Sleep: Off, Clock: 18:42",
      IRAcUtils::resultAcToString(&irsend.capture));
}

TEST(TestDecodeSanyoAc88, SyntheticSelfDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  const uint8_t expectedState[kSanyoAc88StateLength] = {
      0xAA, 0x59, 0xA0, 0x18, 0x06, 0x2A, 0x12, 0x00, 0x00, 0x00, 0x80};
  irsend.begin();
  irsend.reset();
  irsend.sendSanyoAc88(expectedState);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SANYO_AC88, irsend.capture.decode_type);
  EXPECT_EQ(kSanyoAc88Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_EQ(
      "Power: On, Mode: 2 (Cool), Temp: 24C, Fan: 0 (Auto), Swing(V): Off, "
      "Turbo: Off, Sleep: Off, Clock: 18:42",
      IRAcUtils::resultAcToString(&irsend.capture));
  EXPECT_EQ(
      "f38000d50"
      "m5400s2000"
      "m500s750m500s1500m500s750m500s1500m500s750m500s1500m500s750m500s1500"
      "m500s1500m500s750m500s750m500s1500m500s1500m500s750m500s1500m500s750"
      "m500s750m500s750m500s750m500s750m500s750m500s1500m500s750m500s1500"
      "m500s750m500s750m500s750m500s1500m500s1500m500s750m500s750m500s750"
      "m500s750m500s1500m500s1500m500s750m500s750m500s750m500s750m500s750"
      "m500s750m500s1500m500s750m500s1500m500s750m500s1500m500s750m500s750"
      "m500s750m500s1500m500s750m500s750m500s1500m500s750m500s750m500s750"
      "m500s750m500s750m500s750m500s750m500s750m500s750m500s750m500s750"
      "m500s750m500s750m500s750m500s750m500s750m500s750m500s750m500s750"
      "m500s750m500s750m500s750m500s750m500s750m500s750m500s750m500s750"
      "m500s750m500s750m500s750m500s750m500s750m500s750m500s750m500s1500"
      "m500s3675"
      "m5400s2000"
      "m500s750m500s1500m500s750m500s1500m500s750m500s1500m500s750m500s1500"
      "m500s1500m500s750m500s750m500s1500m500s1500m500s750m500s1500m500s750"
      "m500s750m500s750m500s750m500s750m500s750m500s1500m500s750m500s1500"
      "m500s750m500s750m500s750m500s1500m500s1500m500s750m500s750m500s750"
      "m500s750m500s1500m500s1500m500s750m500s750m500s750m500s750m500s750"
      "m500s750m500s1500m500s750m500s1500m500s750m500s1500m500s750m500s750"
      "m500s750m500s1500m500s750m500s750m500s1500m500s750m500s750m500s750"
      "m500s750m500s750m500s750m500s750m500s750m500s750m500s750m500s750"
      "m500s750m500s750m500s750m500s750m500s750m500s750m500s750m500s750"
      "m500s750m500s750m500s750m500s750m500s750m500s750m500s750m500s750"
      "m500s750m500s750m500s750m500s750m500s750m500s750m500s750m500s1500"
      "m500s3675"
      "m5400s2000"
      "m500s750m500s1500m500s750m500s1500m500s750m500s1500m500s750m500s1500"
      "m500s1500m500s750m500s750m500s1500m500s1500m500s750m500s1500m500s750"
      "m500s750m500s750m500s750m500s750m500s750m500s1500m500s750m500s1500"
      "m500s750m500s750m500s750m500s1500m500s1500m500s750m500s750m500s750"
      "m500s750m500s1500m500s1500m500s750m500s750m500s750m500s750m500s750"
      "m500s750m500s1500m500s750m500s1500m500s750m500s1500m500s750m500s750"
      "m500s750m500s1500m500s750m500s750m500s1500m500s750m500s750m500s750"
      "m500s750m500s750m500s750m500s750m500s750m500s750m500s750m500s750"
      "m500s750m500s750m500s750m500s750m500s750m500s750m500s750m500s750"
      "m500s750m500s750m500s750m500s750m500s750m500s750m500s750m500s750"
      "m500s750m500s750m500s750m500s750m500s750m500s750m500s750m500s1500"
      "m500s103675",
      irsend.outputStr());
}

// Tests for IRSanyoAc88 class.

TEST(TestSanyoAc88Class, Power) {
  IRSanyoAc88 ac(kGpioUnused);
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

TEST(TestSanyoAc88Class, FanSpeed) {
  IRSanyoAc88 ac(kGpioUnused);
  ac.begin();

  ac.setFan(kSanyoAc88FanAuto);
  EXPECT_EQ(kSanyoAc88FanAuto, ac.getFan());

  ac.setFan(kSanyoAc88FanHigh);
  EXPECT_EQ(kSanyoAc88FanHigh, ac.getFan());

  ac.setFan(kSanyoAc88FanLow);
  EXPECT_EQ(kSanyoAc88FanLow, ac.getFan());

  ac.setFan(kSanyoAc88FanMedium);
  EXPECT_EQ(kSanyoAc88FanMedium, ac.getFan());
}

TEST(TestSanyoAc88Class, Sleep) {
  IRSanyoAc88 ac(kGpioUnused);
  ac.begin();

  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
  ac.setSleep(false);
  EXPECT_FALSE(ac.getSleep());
  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
}

TEST(TestSanyoAc88Class, SwingV) {
  IRSanyoAc88 ac(kGpioUnused);
  ac.begin();

  ac.setSwingV(true);
  EXPECT_TRUE(ac.getSwingV());
  ac.setSwingV(false);
  EXPECT_FALSE(ac.getSwingV());
  ac.setSwingV(true);
  EXPECT_TRUE(ac.getSwingV());
}

TEST(TestSanyoAc88Class, Filter) {
  IRSanyoAc88 ac(kGpioUnused);
  ac.begin();

  ac.setFilter(true);
  EXPECT_TRUE(ac.getFilter());
  ac.setFilter(false);
  EXPECT_FALSE(ac.getFilter());
  ac.setFilter(true);
  EXPECT_TRUE(ac.getFilter());
}

TEST(TestSanyoAc88Class, Turbo) {
  IRSanyoAc88 ac(kGpioUnused);
  ac.begin();

  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());
  ac.setTurbo(false);
  EXPECT_FALSE(ac.getTurbo());
  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());
}

TEST(TestSanyoAc88Class, Temperature) {
  IRSanyoAc88 ac(kGpioUnused);
  ac.begin();

  ac.setTemp(0);
  EXPECT_EQ(kSanyoAc88TempMin, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kSanyoAc88TempMax, ac.getTemp());

  ac.setTemp(kSanyoAc88TempMin);
  EXPECT_EQ(kSanyoAc88TempMin, ac.getTemp());

  ac.setTemp(kSanyoAc88TempMax);
  EXPECT_EQ(kSanyoAc88TempMax, ac.getTemp());

  ac.setTemp(kSanyoAc88TempMin - 1);
  EXPECT_EQ(kSanyoAc88TempMin, ac.getTemp());

  ac.setTemp(kSanyoAc88TempMax + 1);
  EXPECT_EQ(kSanyoAc88TempMax, ac.getTemp());

  ac.setTemp(17);
  EXPECT_EQ(17, ac.getTemp());

  ac.setTemp(21);
  EXPECT_EQ(21, ac.getTemp());

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());
}

TEST(TestSanyoAc88Class, OperatingMode) {
  IRSanyoAc88 ac(kGpioUnused);
  ac.begin();

  ac.setMode(kSanyoAc88Auto);
  EXPECT_EQ(kSanyoAc88Auto, ac.getMode());

  ac.setMode(kSanyoAc88Cool);
  EXPECT_EQ(kSanyoAc88Cool, ac.getMode());

  ac.setMode(kSanyoAc88Heat);
  EXPECT_EQ(kSanyoAc88Heat, ac.getMode());

  ac.setMode(kSanyoAc88Fan);
  EXPECT_EQ(kSanyoAc88Fan, ac.getMode());

  ac.setMode(kSanyoAc88Fan + 1);
  EXPECT_EQ(kSanyoAc88Auto, ac.getMode());

  ac.setMode(0);
  EXPECT_EQ(kSanyoAc88Auto, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kSanyoAc88Auto, ac.getMode());
}

TEST(TestSanyoAc88Class, Clock) {
  IRSanyoAc88 ac(kGpioUnused);
  ac.begin();

  EXPECT_EQ(0, ac.getClock());

  ac.setClock(21 * 60 + 19);
  EXPECT_EQ(21 * 60 + 19, ac.getClock());

  ac.setClock(0);
  EXPECT_EQ(0, ac.getClock());

  ac.setClock(25 * 60 + 61);
  EXPECT_EQ(23 * 60 + 59, ac.getClock());
}

TEST(TestDecodeSanyoAc152, DecodeRealExamples) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  // Ref: "16c" from https://github.com/crankyoldgit/IRremoteESP8266/issues/1826#issuecomment-1160708653
  const uint16_t rawData[307] = {
      3294, 1726, 420, 330, 458, 462, 382, 452, 438, 330, 456, 458, 384, 454,
      384, 1312, 422, 324, 512, 336, 454, 458, 384, 450, 438, 358, 436, 464,
      424, 326, 458, 476, 372, 458, 430, 328, 458, 464, 382, 1308, 424, 326,
      510, 1264, 424, 268, 520, 460, 380, 460, 436, 324, 462, 400, 436, 474,
      372, 456, 430, 342, 452, 450, 388, 446, 442, 1262, 422, 266, 520, 1314,
      372, 1306, 424, 1258, 370, 390, 448, 1314, 372, 1310, 426, 308, 522, 338,
      454, 470, 370, 454, 438, 330, 456, 468, 370, 456, 384, 464, 384, 1306,
      422, 328, 460, 472, 374, 448, 442, 1258, 426, 1256, 426, 268, 520, 464,
      382, 460, 430, 328, 508, 1264, 426, 262, 572, 1262, 424, 228, 604, 1262,
      372, 1312, 372, 1310, 426, 1256, 422, 1258, 424, 262, 524, 418, 428, 456,
      382, 1308, 372, 456, 386, 456, 382, 464, 378, 1308, 424, 360, 436, 454,
      430, 344, 450, 1306, 372, 1310, 424, 326, 510, 338, 452, 456, 384, 456,
      436, 328, 510, 1258, 372, 1310, 422, 338, 454, 466, 424, 328, 460, 1310,
      372, 1312, 424, 1258, 450, 262, 496, 1310, 372, 1310, 426, 1260, 424, 260,
      526, 442, 442, 1264, 426, 268, 520, 458, 380, 450, 386, 462, 436, 320,
      518, 1256, 372, 394, 446, 398, 494, 334, 506, 326, 510, 276, 518, 460,
      430, 332, 508, 326, 510, 356, 440, 448, 444, 264, 572, 336, 456, 408, 434,
      454, 438, 332, 506, 336, 454, 460, 384, 448, 444, 326, 510, 332, 456, 442,
      400, 456, 384, 456, 386, 452, 388, 454, 440, 326, 512, 272, 518, 450, 440,
      334, 454, 458, 440, 320, 516, 268, 570, 340, 452, 468, 424, 260, 574, 336,
      456, 394, 444, 458, 438, 328, 508, 1258, 370, 1312, 372, 1312, 424, 314,
      468, 1312, 450, 1232, 370, 1314, 420, 324, 514};  // UNKNOWN 584FBE80

  const uint8_t expectedState[kSanyoAc152StateLength] = {
        0x40, 0x00, 0x14, 0x80, 0x6E, 0x80, 0x18, 0xEA, 0x23, 0x62,
        0x30, 0xEE, 0x04, 0x01, 0x00, 0x00, 0x00, 0x00, 0x77};
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 307, 38000);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(SANYO_AC152, irsend.capture.decode_type);
  EXPECT_EQ(kSanyoAc152Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_EQ(
      "",
      IRAcUtils::resultAcToString(&irsend.capture));
}

TEST(TestDecodeSanyoAc152, SyntheticSelfDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  const uint8_t expectedState[kSanyoAc152StateLength] = {
        0x40, 0x00, 0x14, 0x80, 0x6E, 0x80, 0x18, 0xEA, 0x23, 0x62,
        0x30, 0xEE, 0x04, 0x01, 0x00, 0x00, 0x00, 0x00, 0x77};
  irsend.begin();
  irsend.reset();
  irsend.sendSanyoAc152(expectedState);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SANYO_AC152, irsend.capture.decode_type);
  EXPECT_EQ(kSanyoAc152Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_EQ(
      "",
      IRAcUtils::resultAcToString(&irsend.capture));
}
