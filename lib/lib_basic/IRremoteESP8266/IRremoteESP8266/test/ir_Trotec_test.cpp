// Copyright 2019 David Conran
#include "ir_Trotec.h"
#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"


TEST(TestTrotecESPClass, toCommon) {
  IRTrotecESP ac(kGpioUnused);
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
  IRTrotecESP ac(kGpioUnused);
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
  IRsendTest irsend(kGpioUnused);
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
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
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
  IRTrotecESP ac(kGpioUnused);

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
  IRTrotecESP ac(kGpioUnused);

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
  IRTrotecESP ac(kGpioUnused);

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
  IRTrotecESP ac(kGpioUnused);
  ac.setSleep(false);
  ASSERT_FALSE(ac.getSleep());
  ac.setSleep(true);
  ASSERT_TRUE(ac.getSleep());
  ac.setSleep(false);
  ASSERT_FALSE(ac.getSleep());
}

TEST(TestTrotecESPClass, Power) {
  IRTrotecESP ac(kGpioUnused);
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

  ASSERT_EQ("TROTEC_3550", typeToString(decode_type_t::TROTEC_3550));
  ASSERT_EQ(decode_type_t::TROTEC_3550, strToDecodeType("TROTEC_3550"));
  ASSERT_TRUE(hasACState(decode_type_t::TROTEC_3550));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::TROTEC_3550));
}

TEST(TestDecodeTrotec3550, RealDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Data from:
  // https://docs.google.com/spreadsheets/d/1j4H1TDJ2QRFLPnls_sxkTZ3c9922TWxqIxwyGWwBIn0/edit#gid=2015400244&range=F8:EV8
  const uint16_t rawData[147] = {
      12005, 5130,
      545, 500, 600, 1950, 540, 500, 595, 1950, 545, 500, 595, 1950, 545, 500,
      595, 1950, 550, 495, 545, 495, 595, 1950, 545, 500, 545, 500, 540, 500,
      600, 1950, 590, 1950, 545, 500, 540, 500, 545, 500, 545, 495, 545, 500,
      545, 500, 545, 500, 540, 500, 545, 495, 545, 500, 545, 500, 545, 500,
      540, 500, 595, 1950, 520, 520, 595, 1950, 545, 500, 540, 500, 545, 525,
      515, 500, 545, 520, 520, 525, 520, 500, 540, 525, 520, 500, 540, 500,
      545, 500, 540, 500, 545, 520, 525, 495, 520, 520, 545, 500, 570, 475,
      540, 505, 590, 1950, 595, 1950, 545, 500, 540, 500, 570, 470, 595, 1950,
      570, 1975, 545, 500, 545, 500, 545, 520, 570, 1950, 550, 495, 545, 500,
      545, 495, 570, 470, 550, 495, 595, 1950, 570, 1975, 545, 525, 570, 1950,
      595, 1950, 565, 475, 545};
  const uint8_t expectedState[kTrotecStateLength] = {
      0x55, 0x23, 0x00, 0x05, 0x00, 0x00, 0x31, 0x88, 0x36};
  irsend.reset();
  irsend.sendRaw(rawData, 147, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::TROTEC_3550, irsend.capture.decode_type);
  EXPECT_EQ(kTrotecBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 18C, Fan: 3 (High), Swing(V): On, "
      "Timer: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

TEST(TestDecodeTrotec3550, SyntheticDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Synthesised Normal Trotec message.
  irsend.reset();
  const uint8_t expectedState[kTrotecStateLength] = {
      0x55, 0x23, 0x00, 0x05, 0x00, 0x00, 0x31, 0x88, 0x36};
  irsend.sendTrotec3550(expectedState);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::TROTEC_3550, irsend.capture.decode_type);
  EXPECT_EQ(kTrotecBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 18C, Fan: 3 (High), Swing(V): On, "
      "Timer: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

TEST(TestTrotec3550Class, SetAndGetMode) {
  IRTrotec3550 ac(kGpioUnused);

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
  const uint8_t fan[9] = {0x55, 0x23, 0x00, 0x05, 0x00, 0x00, 0x23, 0x88, 0x28};
  ac.setRaw(fan);
  EXPECT_EQ(kTrotecFan, ac.getMode());
}

TEST(TestTrotec3550Class, SetAndGetFan) {
  IRTrotec3550 ac(kGpioUnused);

  ac.setFan(kTrotecFanHigh);
  EXPECT_EQ(kTrotecFanHigh, ac.getFan());
  ac.setFan(kTrotecFanLow);
  EXPECT_EQ(kTrotecFanLow, ac.getFan());
  ac.setFan(kTrotecFanMed);
  EXPECT_EQ(kTrotecFanMed, ac.getFan());
  ac.setFan(kTrotecFanHigh);
  EXPECT_EQ(kTrotecFanHigh, ac.getFan());
  ASSERT_NE(7, kTrotecFanHigh);
  // Now try some unexpected value.
  ac.setFan(7);
  EXPECT_EQ(kTrotecFanHigh, ac.getFan());
}

TEST(TestTrotec3550Class, SetAndGetTemp) {
  IRTrotec3550 ac(kGpioUnused);

  // Celsius
  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());
  EXPECT_TRUE(ac.getTempUnit());
  ac.setTemp(kTrotec3550MinTempC);
  EXPECT_EQ(kTrotec3550MinTempC, ac.getTemp());
  ac.setTemp(kTrotec3550MaxTempC);
  EXPECT_EQ(kTrotec3550MaxTempC, ac.getTemp());
  ac.setTemp(kTrotec3550MinTempC - 1);
  EXPECT_EQ(kTrotec3550MinTempC, ac.getTemp());
  ac.setTemp(kTrotec3550MaxTempC + 1);
  EXPECT_EQ(kTrotec3550MaxTempC, ac.getTemp());
  // Fahrenheit
  ac.setTemp(72, false);
  EXPECT_EQ(72, ac.getTemp());
  EXPECT_FALSE(ac.getTempUnit());
  ac.setTemp(kTrotec3550MinTempF, false);
  EXPECT_EQ(kTrotec3550MinTempF, ac.getTemp());
  ac.setTemp(kTrotec3550MaxTempF, false);
  EXPECT_EQ(kTrotec3550MaxTempF, ac.getTemp());
  ac.setTemp(kTrotec3550MinTempF - 1, false);
  EXPECT_EQ(kTrotec3550MinTempF, ac.getTemp());
  ac.setTemp(kTrotec3550MaxTempF + 1, false);
  EXPECT_EQ(kTrotec3550MaxTempF, ac.getTemp());
  // Celsius
  ac.setTemp(25, true);
  EXPECT_EQ(25, ac.getTemp());
  EXPECT_TRUE(ac.getTempUnit());
  uint8_t deg79F[9] = {0x55, 0xA3, 0x00, 0x14, 0x00, 0x00, 0x31, 0x40, 0x7D};
  ac.setRaw(deg79F);
  EXPECT_FALSE(ac.getTempUnit());  // Fahrenheit
  EXPECT_EQ(79, ac.getTemp());
}

TEST(TestTrotec3550Class, SwingV) {
  IRTrotec3550 ac(kGpioUnused);
  ac.setSwingV(false);
  ASSERT_FALSE(ac.getSwingV());
  ac.setSwingV(true);
  ASSERT_TRUE(ac.getSwingV());
  ac.setSwingV(false);
  ASSERT_FALSE(ac.getSwingV());
}

TEST(TestTrotec3550Class, Power) {
  IRTrotec3550 ac(kGpioUnused);
  ac.setPower(false);
  ASSERT_FALSE(ac.getPower());
  ac.setPower(true);
  ASSERT_TRUE(ac.getPower());
  ac.setPower(false);
  ASSERT_FALSE(ac.getPower());
}

TEST(TestTrotec3550Class, Timer) {
  IRTrotec3550 ac(kGpioUnused);
  EXPECT_FALSE(ac._.TimerSet);
  EXPECT_EQ(0, ac.getTimer());

  ac.setTimer(1);
  EXPECT_TRUE(ac._.TimerSet);
  EXPECT_EQ(0, ac.getTimer());

  ac.setTimer(0);
  EXPECT_FALSE(ac._.TimerSet);
  EXPECT_EQ(0, ac.getTimer());

  ac.setTimer(1 * 60);
  EXPECT_TRUE(ac._.TimerSet);
  EXPECT_EQ(1 * 60, ac.getTimer());

  ac.setTimer(2 * 60 + 37);
  EXPECT_TRUE(ac._.TimerSet);
  EXPECT_EQ(2 * 60, ac.getTimer());

  ac.setTimer(7 * 60 + 59);
  EXPECT_TRUE(ac._.TimerSet);
  EXPECT_EQ(7 * 60, ac.getTimer());

  ac.setTimer(8 * 60);  // Max
  EXPECT_TRUE(ac._.TimerSet);
  EXPECT_EQ(8 * 60, ac.getTimer());  // Max

  ac.setTimer(9 * 60);  // Beyond max
  EXPECT_TRUE(ac._.TimerSet);
  EXPECT_EQ(8 * 60, ac.getTimer());  // Max

  ac.setTimer(0);
  EXPECT_FALSE(ac._.TimerSet);
  EXPECT_EQ(0, ac.getTimer());

  // Real data.
  uint8_t onehour[9] = {0x55, 0xBB, 0x01, 0x15, 0x00, 0x00, 0x31, 0x02, 0x59};
  ac.setRaw(onehour);
  EXPECT_TRUE(ac._.TimerSet);
  EXPECT_EQ(1 * 60, ac.getTimer());
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 80F, Fan: 3 (High), Swing(V): On, "
      "Timer: 01:00", ac.toString());
}

TEST(TestTrotec3550Class, HumanReadable) {
  IRTrotec3550 ac(kGpioUnused);
  EXPECT_EQ(
      "Power: Off, Mode: 0 (Auto), Temp: 22C, Fan: 1 (Low), Swing(V): Off, "
      "Timer: Off",
      ac.toString());
  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1563#issuecomment-898318429
  const uint8_t oncool18f3swing[9] = {
      0x55, 0x23, 0x00, 0x05, 0x00, 0x00, 0x31, 0x88, 0x36};
  ac.setRaw(oncool18f3swing);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 18C, Fan: 3 (High), Swing(V): On, "
      "Timer: Off",
      ac.toString());
  const uint8_t offcool18f3swing[9] = {
      0x55, 0x21, 0x00, 0x05, 0x00, 0x00, 0x31, 0x88, 0x34};
  ac.setRaw(offcool18f3swing);
  EXPECT_EQ(
      "Power: Off, Mode: 1 (Cool), Temp: 18C, Fan: 3 (High), Swing(V): On, "
      "Timer: Off",
      ac.toString());
  const uint8_t modeauto[9] = {
      0x55, 0x60, 0x00, 0x0D, 0x00, 0x00, 0x10, 0x88, 0x5A};
  ac.setRaw(modeauto);
  EXPECT_EQ(
      "Power: Off, Mode: 0 (Auto), Temp: 22C, Fan: 1 (Low), Swing(V): Off, "
      "Timer: Off",
      ac.toString());
}

TEST(TestTrotec3550Class, Checksums) {
  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1563#issuecomment-898318429
  const uint8_t oncool18f3swing[9] = {
      0x55, 0x23, 0x00, 0x05, 0x00, 0x00, 0x31, 0x88, 0x36};
  EXPECT_EQ(0x36, IRTrotec3550::calcChecksum(oncool18f3swing));
  ASSERT_TRUE(IRTrotec3550::validChecksum(oncool18f3swing));
}

TEST(TestTrotec3550Class, toCommon) {
  IRTrotec3550 ac(kGpioUnused);
  ac.setPower(true);
  ac.setMode(kTrotecCool);
  ac.setTemp(20);
  ac.setFan(kTrotecFanHigh);
  ac.setSwingV(true);
  // Now test it.
  ASSERT_EQ(decode_type_t::TROTEC_3550, ac.toCommon().protocol);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kAuto, ac.toCommon().swingv);
  // Unsupported.
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}
