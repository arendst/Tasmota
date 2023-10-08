// Copyright 2021 Tom Rosenback

#include "IRac.h"
#include "ir_Rhoss.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "IRutils.h"
#include "gtest/gtest.h"

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("RHOSS", typeToString(decode_type_t::RHOSS));
  ASSERT_EQ(decode_type_t::RHOSS, strToDecodeType("RHOSS"));
  ASSERT_TRUE(hasACState(decode_type_t::RHOSS));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::RHOSS));
}

// Test sending typical data only.
TEST(TestSendRhoss, SendDataOnly) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  uint8_t expectedState[kRhossStateLength] = {
    0xAA, 0x05, 0x60, 0x00, 0x50, 0x80, 0x54, 0x00, 0x00, 0x00, 0x00, 0x33 };

  irsend.reset();
  irsend.sendRhoss(expectedState);

  EXPECT_EQ(
    "f38000d50"
    "m3042s4248"
    "m648s457m648s1545m648s457m648s1545m648s457m648s1545m648s457m648s1545"
    "m648s1545m648s457m648s1545m648s457m648s457m648s457m648s457m648s457"
    "m648s457m648s457m648s457m648s457m648s457m648s1545m648s1545m648s457"
    "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
    "m648s457m648s457m648s457m648s457m648s1545m648s457m648s1545m648s457"
    "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s1545"
    "m648s457m648s457m648s1545m648s457m648s1545m648s457m648s1545m648s457"
    "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
    "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
    "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
    "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
    "m648s1545m648s1545m648s457m648s457m648s1545m648s1545m648s457m648s457"
    "m648s457m648"
    "s100000",
    irsend.outputStr());
}

// Test send typical data with repeats
TEST(TestSendRhoss, SendWithRepeats) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  irsend.reset();

uint8_t expectedState[kRhossStateLength] = {
    0xAA, 0x05, 0x60, 0x00, 0x50, 0x80, 0x54, 0x00, 0x00, 0x00, 0x00, 0x33 };

  irsend.sendRhoss(expectedState, kRhossStateLength, 0);  // 0 repeats.
  EXPECT_EQ(
      "f38000d50"
      "m3042s4248"
      "m648s457m648s1545m648s457m648s1545m648s457m648s1545m648s457m648s1545"
      "m648s1545m648s457m648s1545m648s457m648s457m648s457m648s457m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s1545m648s1545m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
      "m648s457m648s457m648s457m648s457m648s1545m648s457m648s1545m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s1545"
      "m648s457m648s457m648s1545m648s457m648s1545m648s457m648s1545m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
      "m648s1545m648s1545m648s457m648s457m648s1545m648s1545m648s457m648s457"
      "m648s457m648"
      "s100000",
      irsend.outputStr());

  irsend.sendRhoss(expectedState, kRhossStateLength, 2);  // 2 repeats.
  EXPECT_EQ(
      "f38000d50"
      "m3042s4248"
      "m648s457m648s1545m648s457m648s1545m648s457m648s1545m648s457m648s1545"
      "m648s1545m648s457m648s1545m648s457m648s457m648s457m648s457m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s1545m648s1545m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
      "m648s457m648s457m648s457m648s457m648s1545m648s457m648s1545m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s1545"
      "m648s457m648s457m648s1545m648s457m648s1545m648s457m648s1545m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
      "m648s1545m648s1545m648s457m648s457m648s1545m648s1545m648s457m648s457"
      "m648s457m648"
      "s100000"
      "m3042s4248"
      "m648s457m648s1545m648s457m648s1545m648s457m648s1545m648s457m648s1545"
      "m648s1545m648s457m648s1545m648s457m648s457m648s457m648s457m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s1545m648s1545m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
      "m648s457m648s457m648s457m648s457m648s1545m648s457m648s1545m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s1545"
      "m648s457m648s457m648s1545m648s457m648s1545m648s457m648s1545m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
      "m648s1545m648s1545m648s457m648s457m648s1545m648s1545m648s457m648s457"
      "m648s457m648"
      "s100000"
      "m3042s4248"
      "m648s457m648s1545m648s457m648s1545m648s457m648s1545m648s457m648s1545"
      "m648s1545m648s457m648s1545m648s457m648s457m648s457m648s457m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s1545m648s1545m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
      "m648s457m648s457m648s457m648s457m648s1545m648s457m648s1545m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s1545"
      "m648s457m648s457m648s1545m648s457m648s1545m648s457m648s1545m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
      "m648s457m648s457m648s457m648s457m648s457m648s457m648s457m648s457"
      "m648s1545m648s1545m648s457m648s457m648s1545m648s1545m648s457m648s457"
      "m648s457m648"
      "s100000",
      irsend.outputStr());
}

// Test send raw data
TEST(TestSendRhoss, RawData) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  irsend.reset();

  // Power on, mode cool, temp 20, fan auto, swing off
  const uint16_t rawData[197] = {
    3044, 4248,
    648, 458, 650, 1540, 646, 458, 650, 1538,
    650, 458, 650, 1538, 650, 458, 650, 1540,   // byte 0
    648, 458, 650, 458, 650, 1540, 646, 484,
    624, 456, 650, 456, 650, 456, 650, 456,     // byte 1
    650, 456, 650, 456, 650, 456, 650, 456,
    650, 458, 650, 1540, 650, 1538, 650, 456,   // byte 2
    650, 456, 650, 456, 650, 456, 650, 458,
    650, 456, 650, 456, 650, 456, 650, 458,     // byte 3
    650, 458, 650, 456, 650, 458, 650, 458,
    650, 458, 650, 1538, 650, 458, 650, 458,    // byte 4
    650, 458, 648, 458, 674, 434, 648, 458,
    672, 434, 648, 458, 650, 458, 648, 1540,    // byte 5
    672, 434, 650, 458, 672, 1518, 644, 488,
    622, 1540, 644, 464, 672, 1516, 672, 434,   // byte 6
    672, 434, 672, 434, 650, 458, 648, 458,
    672, 434, 674, 434, 672, 434, 650, 458,     // byte 7
    672, 434, 648, 458, 650, 458, 672, 434,
    672, 436, 648, 458, 648, 456, 650, 458,     // byte 8
    650, 458, 650, 456, 674, 434, 650, 458,
    650, 456, 650, 458, 674, 432, 650, 458,     // byte 9
    650, 456, 650, 456, 650, 458, 648, 458,
    674, 432, 650, 456, 674, 434, 650, 458,     // byte 10
    650, 458, 650, 1538, 650, 458, 650, 458,
    650, 456, 650, 458, 650, 456, 650, 458,     // byte 11
    650, 456,
    650 };  // UNKNOWN 93E7BDB2

  irsend.sendRaw(rawData,  197, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(RHOSS, irsend.capture.decode_type);
  EXPECT_EQ(kRhossBits, irsend.capture.bits);

  uint8_t expected[kRhossStateLength] = {
      0xAA, 0x04, 0x60, 0x00, 0x20, 0x80, 0x54, 0x00, 0x00, 0x00, 0x00, 0x02 };
  EXPECT_STATE_EQ(expected, irsend.capture.state, kRhossBits);

  EXPECT_EQ(
      "Power: On, Mode: 2 (Cool), Temp: 20C, Fan: 0 (Auto), Swing(V): Off",
      IRAcUtils::resultAcToString(&irsend.capture));

  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

// Test synthetic decode
TEST(TestDecodeRhoss, SyntheticSelfDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(0);
  IRRhossAc ac(0);

  uint8_t expectedState[kRhossStateLength] = {
    0xAA, 0x05, 0x60, 0x00, 0x50, 0x80, 0x54, 0x00, 0x00, 0x00, 0x00, 0x33 };

  irsend.begin();
  irsend.reset();
  irsend.sendRhoss(expectedState);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(RHOSS, irsend.capture.decode_type);
  EXPECT_EQ(kRhossBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: On, Mode: 5 (Auto), Temp: 21C, Fan: 0 (Auto), Swing(V): Off",
      ac.toString());
}

// Test strict decoding
TEST(TestDecodeRhoss, StrictDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(0);
  IRRhossAc ac(0);

  uint8_t expectedState[kRhossStateLength] = {
    0xAA, 0x05, 0x60, 0x00, 0x50, 0x80, 0x54, 0x00, 0x00, 0x00, 0x00, 0x33 };

  irsend.begin();
  irsend.reset();
  irsend.sendRhoss(expectedState);
  irsend.makeDecodeResult();
  ASSERT_TRUE(
    irrecv.decodeRhoss(&irsend.capture,
                      kStartOffset, kRhossBits, true));
  EXPECT_EQ(RHOSS, irsend.capture.decode_type);
  EXPECT_EQ(kRhossBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: On, Mode: 5 (Auto), Temp: 21C, Fan: 0 (Auto), Swing(V): Off",
      ac.toString());
}

// Tests for IRRhossAc class.

TEST(TestRhossAcClass, Power) {
  IRRhossAc ac(0);
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

TEST(TestRhossAcClass, Temperature) {
  IRRhossAc ac(0);
  ac.begin();

  ac.setTemp(0);
  EXPECT_EQ(kRhossTempMin, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kRhossTempMax, ac.getTemp());

  ac.setTemp(kRhossTempMin);
  EXPECT_EQ(kRhossTempMin, ac.getTemp());

  ac.setTemp(kRhossTempMax);
  EXPECT_EQ(kRhossTempMax, ac.getTemp());

  ac.setTemp(kRhossTempMin - 1);
  EXPECT_EQ(kRhossTempMin, ac.getTemp());

  ac.setTemp(kRhossTempMax + 1);
  EXPECT_EQ(kRhossTempMax, ac.getTemp());

  ac.setTemp(17);
  EXPECT_EQ(17, ac.getTemp());

  ac.setTemp(21);
  EXPECT_EQ(21, ac.getTemp());

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());

  ac.setTemp(29);
  EXPECT_EQ(29, ac.getTemp());
}


TEST(TestRhossAcClass, OperatingMode) {
  IRRhossAc ac(0);
  ac.begin();

  ac.setMode(kRhossModeAuto);
  EXPECT_EQ(kRhossModeAuto, ac.getMode());

  ac.setMode(kRhossModeCool);
  EXPECT_EQ(kRhossModeCool, ac.getMode());

  ac.setMode(kRhossModeHeat);
  EXPECT_EQ(kRhossModeHeat, ac.getMode());

  ac.setMode(kRhossModeDry);
  EXPECT_EQ(kRhossModeDry, ac.getMode());

  ac.setMode(kRhossModeFan);
  EXPECT_EQ(kRhossModeFan, ac.getMode());

  ac.setMode(kRhossModeAuto + 1);
  EXPECT_EQ(kRhossDefaultMode, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kRhossDefaultMode, ac.getMode());
}

TEST(TestRhossAcClass, FanSpeed) {
  IRRhossAc ac(0);
  ac.begin();

  ac.setFan(0);
  EXPECT_EQ(kRhossFanAuto, ac.getFan());

  ac.setFan(255);
  EXPECT_EQ(kRhossFanAuto, ac.getFan());

  ac.setFan(kRhossFanMax);
  EXPECT_EQ(kRhossFanMax, ac.getFan());

  ac.setFan(kRhossFanMax + 1);
  EXPECT_EQ(kRhossFanAuto, ac.getFan());

  ac.setFan(kRhossFanMax - 1);
  EXPECT_EQ(kRhossFanMax - 1, ac.getFan());

  ac.setFan(1);
  EXPECT_EQ(1, ac.getFan());

  ac.setFan(1);
  EXPECT_EQ(1, ac.getFan());

  ac.setFan(3);
  EXPECT_EQ(3, ac.getFan());
}

TEST(TestRhossAcClass, Swing) {
  IRRhossAc ac(0);
  ac.begin();

  ac.setSwing(false);
  EXPECT_FALSE(ac.getSwing());

  ac.setSwing(true);
  EXPECT_TRUE(ac.getSwing());
}

TEST(TestRhossAcClass, Checksums) {
  uint8_t state[kRhossStateLength] = {
    0xAA, 0x05, 0x60, 0x00, 0x50, 0x80, 0x54, 0x00, 0x00, 0x00, 0x00, 0x33 };

  ASSERT_EQ(0x33, IRRhossAc::calcChecksum(state));
  EXPECT_TRUE(IRRhossAc::validChecksum(state));
  // Change the array so the checksum is invalid.
  state[0] ^= 0xFF;
  EXPECT_FALSE(IRRhossAc::validChecksum(state));
  // Restore the previous change, and change another byte.
  state[0] ^= 0xFF;
  state[4] ^= 0xFF;
  EXPECT_FALSE(IRRhossAc::validChecksum(state));
  state[4] ^= 0xFF;
  EXPECT_TRUE(IRRhossAc::validChecksum(state));

  // Additional known good states.
  uint8_t knownGood1[kRhossStateLength] = {
    0xAA, 0x06, 0x60, 0x00, 0x50, 0x80, 0x54, 0x00, 0x00, 0x00, 0x00, 0x34 };
  EXPECT_TRUE(IRRhossAc::validChecksum(knownGood1));
  ASSERT_EQ(0x34, IRRhossAc::calcChecksum(knownGood1));

  uint8_t knownGood2[kRhossStateLength] = {
    0xAA, 0x07, 0x60, 0x00, 0x50, 0x80, 0x54, 0x00, 0x00, 0x00, 0x00, 0x35 };
  EXPECT_TRUE(IRRhossAc::validChecksum(knownGood2));
  ASSERT_EQ(0x35, IRRhossAc::calcChecksum(knownGood2));

  uint8_t knownGood3[kRhossStateLength] = {
    0xAA, 0x07, 0x60, 0x00, 0x53, 0x80, 0x54, 0x00, 0x00, 0x00, 0x00, 0x38 };
  EXPECT_TRUE(IRRhossAc::validChecksum(knownGood3));
  ASSERT_EQ(0x38, IRRhossAc::calcChecksum(knownGood3));

  // Validate calculation of checksum,
  // same as knownGood3 except for the checksum.
  uint8_t knownBad[kRhossStateLength] = {
    0xAA, 0x07, 0x60, 0x00, 0x53, 0x80, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00 };
  EXPECT_FALSE(IRRhossAc::validChecksum(knownBad));
  IRRhossAc ac(0);
  ac.setRaw(knownBad);
  EXPECT_STATE_EQ(knownGood3, ac.getRaw(), kRhossBits);
}
