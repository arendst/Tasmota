// Copyright 2019 David Conran
#include "ir_Trotec.h"
#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"


TEST(TestTrotecESPClass, toCommon) {
  IRTrotecESP ac(0);
  ac.setPower(true);
  ac.setMode(kTrotecCool);
  ac.setTemp(20);
  ac.setSpeed(kTrotecFanHigh);
  ac.setSleep(true);
  // Now test it.
  ASSERT_EQ(decode_type_t::TROTEC, ac.toCommon().protocol);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(0, ac.toCommon().sleep);
  // Unsupported.
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_EQ(stdAc::swingv_t::kOff, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

TEST(TestTrotecESPClass, MessageConstructon) {
  IRTrotecESP ac(0);
  ac.setPower(true);
  ac.setTemp(20);
  ac.setMode(kTrotecCool);
  ac.setSpeed(kTrotecFanMed);
  ac.setSleep(true);

  uint8_t expected[kTrotecStateLength] = {
      0x12, 0x34, 0x29, 0x82, 0x00, 0x00, 0x00, 0x00, 0xAB};
  EXPECT_STATE_EQ(expected, ac.getRaw(), kTrotecBits);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 20C, Fan: 2 (Medium), Sleep: On",
      ac.toString());
}

// Tests for sendTrotec().

// Test sending typical data only.
TEST(TestSendTrotec, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();
  uint8_t data[kTrotecStateLength] = {
      0x12, 0x34, 0x29, 0x82, 0x00, 0x00, 0x00, 0x00, 0xAB};

  irsend.sendTrotec(data);
  EXPECT_EQ(
      "f36000d50"
      "m5952s7364"
      "m592s592m592s1560m592s592m592s592m592s1560m592s592m592s592m592s592"
      "m592s592m592s592m592s1560m592s592m592s1560m592s1560m592s592m592s592"
      "m592s1560m592s592m592s592m592s1560m592s592m592s1560m592s592m592s592"
      "m592s592m592s1560m592s592m592s592m592s592m592s592m592s592m592s1560"
      "m592s592m592s592m592s592m592s592m592s592m592s592m592s592m592s592"
      "m592s592m592s592m592s592m592s592m592s592m592s592m592s592m592s592"
      "m592s592m592s592m592s592m592s592m592s592m592s592m592s592m592s592"
      "m592s592m592s592m592s592m592s592m592s592m592s592m592s592m592s592"
      "m592s1560m592s1560m592s592m592s1560m592s592m592s1560m592s592m592s1560"
      "m592s6184"
      "m592s1500",
      irsend.outputStr());
}

// Tests for decodeTrotec().
// Decode normal Trotec messages.

TEST(TestDecodeTrotec, SyntheticDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Synthesised Normal Trotec message.
  irsend.reset();
  uint8_t expectedState[kTrotecStateLength] = {
      0x12, 0x34, 0x29, 0x82, 0x00, 0x00, 0x00, 0x00, 0xAB};
  irsend.sendTrotec(expectedState);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::TROTEC, irsend.capture.decode_type);
  EXPECT_EQ(kTrotecBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 20C, Fan: 2 (Medium), Sleep: On",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}


TEST(TestTrotecESPClass, SetAndGetTemp) {
  IRTrotecESP ac(0);

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());
  ac.setTemp(kTrotecMinTemp);
  EXPECT_EQ(kTrotecMinTemp, ac.getTemp());
  ac.setTemp(kTrotecMaxTemp);
  EXPECT_EQ(kTrotecMaxTemp, ac.getTemp());
  ac.setTemp(kTrotecMinTemp - 1);
  EXPECT_EQ(kTrotecMinTemp, ac.getTemp());
  ac.setTemp(kTrotecMaxTemp + 1);
  EXPECT_EQ(kTrotecMaxTemp, ac.getTemp());
}

TEST(TestTrotecESPClass, SetAndGetMode) {
  IRTrotecESP ac(0);

  ac.setMode(kTrotecFan);
  EXPECT_EQ(kTrotecFan, ac.getMode());
  ac.setMode(kTrotecCool);
  EXPECT_EQ(kTrotecCool, ac.getMode());
  ac.setMode(kTrotecAuto);
  EXPECT_EQ(kTrotecAuto, ac.getMode());
  ac.setMode(kTrotecDry);
  EXPECT_EQ(kTrotecDry, ac.getMode());
  ac.setMode(255);
  EXPECT_EQ(kTrotecAuto, ac.getMode());
}

TEST(TestTrotecESPClass, SetAndGetFan) {
  IRTrotecESP ac(0);

  ac.setSpeed(kTrotecFanHigh);
  EXPECT_EQ(kTrotecFanHigh, ac.getSpeed());
  ac.setSpeed(kTrotecFanLow);
  EXPECT_EQ(kTrotecFanLow, ac.getSpeed());
  ac.setSpeed(kTrotecFanMed);
  EXPECT_EQ(kTrotecFanMed, ac.getSpeed());
  ac.setSpeed(kTrotecFanHigh);
  EXPECT_EQ(kTrotecFanHigh, ac.getSpeed());
  ASSERT_NE(7, kTrotecFanHigh);
  // Now try some unexpected value.
  ac.setSpeed(7);
  EXPECT_EQ(kTrotecFanHigh, ac.getSpeed());
}

TEST(TestTrotecESPClass, Sleep) {
  IRTrotecESP ac(0);
  ac.setSleep(false);
  ASSERT_FALSE(ac.getSleep());
  ac.setSleep(true);
  ASSERT_TRUE(ac.getSleep());
  ac.setSleep(false);
  ASSERT_FALSE(ac.getSleep());
}

TEST(TestTrotecESPClass, Power) {
  IRTrotecESP ac(0);
  ac.setPower(false);
  ASSERT_FALSE(ac.getPower());
  ac.setPower(true);
  ASSERT_TRUE(ac.getPower());
  ac.setPower(false);
  ASSERT_FALSE(ac.getPower());
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("TROTEC", typeToString(decode_type_t::TROTEC));
  ASSERT_EQ(decode_type_t::TROTEC, strToDecodeType("TROTEC"));
  ASSERT_TRUE(hasACState(decode_type_t::TROTEC));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::TROTEC));
}
