// Copyright 2019 David Conran (crankyoldgit)

#include "ir_Neoclima.h"
#include <algorithm>
#include "IRac.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "gtest/gtest.h"

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("NEOCLIMA", typeToString(decode_type_t::NEOCLIMA));
  ASSERT_EQ(decode_type_t::NEOCLIMA, strToDecodeType("NEOCLIMA"));
  ASSERT_TRUE(hasACState(decode_type_t::NEOCLIMA));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::NEOCLIMA));
}

// Test sending typical data only.
TEST(TestSendNeoclima, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();

  uint8_t state[kNeoclimaStateLength] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x6A, 0x00, 0x2A, 0xA5, 0x39};
  irsend.reset();
  irsend.sendNeoclima(state);
  EXPECT_EQ(
      "f38000d50"
      "m6112s7391"
      "m537s571m537s571m537s571m537s571m537s571m537s571m537s571m537s571"
      "m537s571m537s571m537s571m537s571m537s571m537s571m537s571m537s571"
      "m537s571m537s571m537s571m537s571m537s571m537s571m537s571m537s571"
      "m537s571m537s571m537s571m537s571m537s571m537s571m537s571m537s571"
      "m537s571m537s571m537s571m537s571m537s571m537s571m537s571m537s571"
      "m537s571m537s571m537s571m537s571m537s571m537s571m537s571m537s571"
      "m537s571m537s571m537s571m537s571m537s571m537s571m537s571m537s571"
      "m537s571m537s1651m537s571m537s1651m537s571m537s1651m537s1651m537s571"
      "m537s571m537s571m537s571m537s571m537s571m537s571m537s571m537s571"
      "m537s571m537s1651m537s571m537s1651m537s571m537s1651m537s571m537s571"
      "m537s1651m537s571m537s1651m537s571m537s571m537s1651m537s571m537s1651"
      "m537s1651m537s571m537s571m537s1651m537s1651m537s1651m537s571m537s571"
      "m537s7391"
      "m537s100000",
      irsend.outputStr());
}

// https://github.com/crankyoldgit/IRremoteESP8266/issues/764#issuecomment-503755096
TEST(TestDecodeNeoclima, RealExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  uint16_t rawData[197] = {
      6112, 7392, 540, 602, 516, 578, 522, 604, 540, 554, 540, 554, 540, 576,
      518, 576, 516, 554, 540, 608, 542, 554, 540, 554, 540, 576, 518, 604, 516,
      556, 540, 576, 546, 580, 542, 578, 542, 602, 518, 554, 542, 554, 568, 582,
      540, 554, 540, 582, 540, 578, 518, 582, 542, 576, 544, 530, 566, 534, 562,
      534, 562, 552, 542, 582, 540, 604, 518, 608, 542, 554, 540, 582, 540, 604,
      518, 580, 540, 606, 544, 554, 542, 554, 542, 580, 542, 576, 520, 554, 540,
      578, 518, 578, 518, 582, 544, 552, 570, 580, 544, 580, 542, 554, 542, 604,
      520, 576, 520, 580, 540, 556, 540, 556, 542, 584, 566, 580, 542, 1622,
      542, 554, 542, 1620, 544, 604, 520, 1642, 518, 1674, 548, 560, 564, 580,
      544, 554, 544, 552, 544, 554, 542, 556, 542, 576, 522, 554, 542, 556, 542,
      580, 542, 1670, 520, 578, 520, 1622, 542, 580, 518, 1646, 520, 558, 568,
      552, 546, 1628, 566, 580, 544, 1668, 522, 576, 520, 578, 520, 1670, 522,
      576, 522, 1670, 496, 1676, 570, 560, 566, 532, 564, 1648, 544, 1670, 522,
      1650, 544, 552, 544, 576, 520, 7390, 544};  // UNKNOWN EE182D95

  uint8_t expectedState[kNeoclimaStateLength] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x6A, 0x00, 0x2A, 0xA5, 0x39};

  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 197, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::NEOCLIMA, irsend.capture.decode_type);
  ASSERT_EQ(kNeoclimaBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  IRNeoclimaAc ac(0);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 26C, Fan: 3 (Low), "
      "Swing(V): Off, Swing(H): On, Sleep: Off, Turbo: Off, Hold: Off, "
      "Ion: Off, Eye: Off, Light: Off, Follow: Off, 8C Heat: Off, Fresh: Off, "
      "Button: 0 (Power)",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

// Self decode.
TEST(TestDecodeNeoclima, SyntheticExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);

  uint8_t expectedState[kNeoclimaStateLength] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x6A, 0x00, 0x2A, 0xA5, 0x39};

  irsend.begin();
  irsend.reset();
  irsend.sendNeoclima(expectedState);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::NEOCLIMA, irsend.capture.decode_type);
  ASSERT_EQ(kNeoclimaBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
}

TEST(TestIRNeoclimaAcClass, Power) {
  IRNeoclimaAc ac(0);
  ac.begin();

  ac.on();
  EXPECT_TRUE(ac.getPower());

  ac.off();
  EXPECT_FALSE(ac.getPower());

  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());

  ac.setPower(false);
  EXPECT_FALSE(ac.getPower());

  EXPECT_EQ(kNeoclimaButtonPower, ac.getButton());
}

TEST(TestIRNeoclimaAcClass, OperatingMode) {
  IRNeoclimaAc ac(0);
  ac.begin();

  ac.setMode(kNeoclimaAuto);
  EXPECT_EQ(kNeoclimaAuto, ac.getMode());
  EXPECT_EQ(kNeoclimaButtonMode, ac.getButton());


  ac.setMode(kNeoclimaCool);
  EXPECT_EQ(kNeoclimaCool, ac.getMode());

  ac.setMode(kNeoclimaHeat);
  EXPECT_EQ(kNeoclimaHeat, ac.getMode());

  ASSERT_NE(kNeoclimaFanHigh, kNeoclimaFanLow);
  ac.setFan(kNeoclimaFanHigh);
  ac.setMode(kNeoclimaDry);  // Dry should lock the fan to speed LOW.
  EXPECT_EQ(kNeoclimaDry, ac.getMode());
  EXPECT_EQ(kNeoclimaFanLow, ac.getFan());
  ac.setFan(kNeoclimaFanHigh);
  EXPECT_EQ(kNeoclimaFanLow, ac.getFan());

  ac.setMode(kNeoclimaFan);
  EXPECT_EQ(kNeoclimaFan, ac.getMode());

  ac.setMode(kNeoclimaHeat + 1);
  EXPECT_EQ(kNeoclimaAuto, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kNeoclimaAuto, ac.getMode());
}

TEST(TestIRNeoclimaAcClass, SetAndGetTemp) {
  IRNeoclimaAc ac(0);
  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());
  ac.setTemp(kNeoclimaMinTemp);
  EXPECT_EQ(kNeoclimaMinTemp, ac.getTemp());
  EXPECT_EQ(kNeoclimaButtonTempDown, ac.getButton());
  ac.setTemp(kNeoclimaMinTemp - 1);
  EXPECT_EQ(kNeoclimaMinTemp, ac.getTemp());
  ac.setTemp(kNeoclimaMaxTemp);
  EXPECT_EQ(kNeoclimaMaxTemp, ac.getTemp());
  EXPECT_EQ(kNeoclimaButtonTempUp, ac.getButton());
  ac.setTemp(kNeoclimaMaxTemp + 1);
  EXPECT_EQ(kNeoclimaMaxTemp, ac.getTemp());
}

TEST(TestIRNeoclimaAcClass, FanSpeed) {
  IRNeoclimaAc ac(0);
  ac.begin();

  ac.setFan(0);
  EXPECT_EQ(0, ac.getFan());

  ac.setFan(255);
  EXPECT_EQ(kNeoclimaFanAuto, ac.getFan());

  ac.setFan(kNeoclimaFanHigh);
  EXPECT_EQ(kNeoclimaFanHigh, ac.getFan());

  ac.setFan(std::max(kNeoclimaFanHigh, kNeoclimaFanLow) + 1);
  EXPECT_EQ(kNeoclimaFanAuto, ac.getFan());

  ac.setFan(kNeoclimaFanHigh - 1);
  EXPECT_EQ(kNeoclimaFanHigh - 1, ac.getFan());

  ac.setFan(1);
  EXPECT_EQ(1, ac.getFan());

  ac.setFan(1);
  EXPECT_EQ(1, ac.getFan());

  ac.setFan(3);
  EXPECT_EQ(3, ac.getFan());
  EXPECT_EQ(kNeoclimaButtonFanSpeed, ac.getButton());

  // Data from:
  //   https://drive.google.com/file/d/1kjYk4zS9NQcMQhFkak-L4mp4UuaAIesW/view
  uint8_t fan_low[12] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x6A, 0x00, 0x29, 0xA5, 0x3D};
  uint8_t fan_medium[12] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x4A, 0x00, 0x29, 0xA5, 0x1D};
  uint8_t fan_high[12] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x2A, 0x00, 0x29, 0xA5, 0xFD};
  uint8_t fan_auto[12] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x0A, 0x00, 0x29, 0xA5, 0xDD};
  ac.setRaw(fan_low);
  EXPECT_EQ(kNeoclimaFanLow, ac.getFan());
  EXPECT_EQ(kNeoclimaButtonFanSpeed, ac.getButton());
  ac.setRaw(fan_medium);
  EXPECT_EQ(kNeoclimaFanMed, ac.getFan());
  EXPECT_EQ(kNeoclimaButtonFanSpeed, ac.getButton());
  ac.setRaw(fan_high);
  EXPECT_EQ(kNeoclimaFanHigh, ac.getFan());
  EXPECT_EQ(kNeoclimaButtonFanSpeed, ac.getButton());
  ac.setRaw(fan_auto);
  EXPECT_EQ(kNeoclimaFanAuto, ac.getFan());
  EXPECT_EQ(kNeoclimaButtonFanSpeed, ac.getButton());
}

TEST(TestIRNeoclimaAcClass, Sleep) {
  IRNeoclimaAc ac(0);
  ac.begin();
  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
  ac.setSleep(false);
  EXPECT_FALSE(ac.getSleep());
  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
  EXPECT_EQ(kNeoclimaButtonSleep, ac.getButton());
}

TEST(TestIRNeoclimaAcClass, Turbo) {
  IRNeoclimaAc ac(0);
  ac.begin();
  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());
  ac.setTurbo(false);
  EXPECT_FALSE(ac.getTurbo());
  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());
  EXPECT_EQ(kNeoclimaButtonTurbo, ac.getButton());
  // Data from:
  //   https://drive.google.com/file/d/1tA09Gu_ZqDcHucscnqzv0V3cIUWOE0d1/view
  uint8_t turbo_on[12] = {
      0x00, 0x00, 0x00, 0x08, 0x00, 0x0A, 0x00, 0x6A, 0x00, 0x88, 0xA5, 0xA9};
  uint8_t turbo_off[12] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x6A, 0x00, 0x88, 0xA5, 0xA1};
  ac.setRaw(turbo_on);
  EXPECT_TRUE(ac.getTurbo());
  EXPECT_EQ(kNeoclimaButtonTurbo, ac.getButton());
  ac.setRaw(turbo_off);
  EXPECT_EQ(kNeoclimaButtonTurbo, ac.getButton());
  EXPECT_FALSE(ac.getTurbo());
}

TEST(TestIRNeoclimaAcClass, Fresh) {
  IRNeoclimaAc ac(0);
  ac.begin();
  ac.setFresh(true);
  EXPECT_TRUE(ac.getFresh());
  ac.setFresh(false);
  EXPECT_FALSE(ac.getFresh());
  ac.setFresh(true);
  EXPECT_TRUE(ac.getFresh());
  EXPECT_EQ(kNeoclimaButtonFresh, ac.getButton());
  // Data from:
  //   https://drive.google.com/file/d/1kjYk4zS9NQcMQhFkak-L4mp4UuaAIesW/view
  uint8_t on[12] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0x00, 0x6A, 0x00, 0x29, 0xA5, 0xCD};
  uint8_t off[12] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x00, 0x6A, 0x00, 0x29, 0xA5, 0x4D};
  ac.setRaw(on);
  EXPECT_TRUE(ac.getFresh());
  EXPECT_EQ(kNeoclimaButtonFresh, ac.getButton());
  ac.setRaw(off);
  EXPECT_EQ(kNeoclimaButtonFresh, ac.getButton());
  EXPECT_FALSE(ac.getFresh());
}

TEST(TestIRNeoclimaAcClass, Hold) {
  IRNeoclimaAc ac(0);
  ac.begin();
  ac.setHold(true);
  EXPECT_TRUE(ac.getHold());
  ac.setHold(false);
  EXPECT_FALSE(ac.getHold());
  ac.setHold(true);
  EXPECT_TRUE(ac.getHold());
  EXPECT_EQ(kNeoclimaButtonHold, ac.getButton());
}

TEST(TestIRNeoclimaAcClass, 8CHeat) {
  IRNeoclimaAc ac(0);
  ac.begin();
  ac.set8CHeat(true);
  EXPECT_TRUE(ac.get8CHeat());
  ac.set8CHeat(false);
  EXPECT_FALSE(ac.get8CHeat());
  ac.set8CHeat(true);
  EXPECT_TRUE(ac.get8CHeat());
  EXPECT_EQ(kNeoclimaButton8CHeat, ac.getButton());
}

TEST(TestIRNeoclimaAcClass, Light) {
  IRNeoclimaAc ac(0);
  ac.begin();
  ac.setLight(true);
  EXPECT_TRUE(ac.getLight());
  ac.setLight(false);
  EXPECT_FALSE(ac.getLight());
  ac.setLight(true);
  EXPECT_TRUE(ac.getLight());
  EXPECT_EQ(kNeoclimaButtonLight, ac.getButton());
}

TEST(TestIRNeoclimaAcClass, Ion) {
  IRNeoclimaAc ac(0);
  ac.begin();
  ac.setIon(true);
  EXPECT_TRUE(ac.getIon());
  ac.setIon(false);
  EXPECT_FALSE(ac.getIon());
  ac.setIon(true);
  EXPECT_TRUE(ac.getIon());
  EXPECT_EQ(kNeoclimaButtonIon, ac.getButton());
}

TEST(TestIRNeoclimaAcClass, Eye) {
  IRNeoclimaAc ac(0);
  ac.begin();
  ac.setEye(true);
  EXPECT_TRUE(ac.getEye());
  ac.setEye(false);
  EXPECT_FALSE(ac.getEye());
  ac.setEye(true);
  EXPECT_TRUE(ac.getEye());
  EXPECT_EQ(kNeoclimaButtonEye, ac.getButton());
}

TEST(TestIRNeoclimaAcClass, Follow) {
  IRNeoclimaAc ac(0);
  ac.begin();
  /*  DISABLED: See TODO in ir_Neoclima.cpp
  ac.setFollow(true);
  EXPECT_TRUE(ac.getFollow());
  ac.setFollow(false);
  EXPECT_FALSE(ac.getFollow());
  ac.setFollow(true);
  EXPECT_TRUE(ac.getFollow());
  EXPECT_EQ(kNeoclimaButtonFollow, ac.getButton());
  */
  uint8_t on_5F[12] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x00, 0x0A, 0x5F, 0x89, 0xA5, 0xAA};
  uint8_t on_5D[12] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x00, 0x6A, 0x5D, 0x29, 0xA5, 0xA8};
  uint8_t off[12] = {
      0x00, 0x04, 0x00, 0x40, 0x00, 0x13, 0x00, 0x6B, 0x00, 0x29, 0xA5, 0x90};
  ac.setRaw(on_5F);
  EXPECT_TRUE(ac.getFollow());
  ac.setRaw(off);
  EXPECT_FALSE(ac.getFollow());
  ac.setRaw(on_5D);
  EXPECT_TRUE(ac.getFollow());
}

TEST(TestIRNeoclimaAcClass, ChecksumCalculation) {
  uint8_t examplestate[kNeoclimaStateLength] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x6A, 0x00, 0x2A, 0xA5, 0x39};
  const uint8_t originalstate[kNeoclimaStateLength] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x6A, 0x00, 0x2A, 0xA5, 0x39};

  EXPECT_TRUE(IRNeoclimaAc::validChecksum(examplestate));
  EXPECT_EQ(0x39, IRNeoclimaAc::calcChecksum(examplestate));

  examplestate[11] = 0x12;  // Set an incorrect checksum.
  EXPECT_FALSE(IRNeoclimaAc::validChecksum(examplestate));
  EXPECT_EQ(0x39, IRNeoclimaAc::calcChecksum(examplestate));
  IRNeoclimaAc ac(0);
  ac.setRaw(examplestate);
  // Extracting the state from the object should have a correct checksum.
  EXPECT_TRUE(IRNeoclimaAc::validChecksum(ac.getRaw()));
  EXPECT_STATE_EQ(originalstate, ac.getRaw(), kNeoclimaBits);
  examplestate[11] = 0x39;  // Restore old checksum value.

  // Change the state to force a different checksum.
  examplestate[8] = 0x01;
  EXPECT_FALSE(IRNeoclimaAc::validChecksum(examplestate));
  EXPECT_EQ(0x3A, IRNeoclimaAc::calcChecksum(examplestate));
}

TEST(TestIRNeoclimaAcClass, toCommon) {
  IRNeoclimaAc ac(0);
  ac.setPower(true);
  ac.setMode(kNeoclimaCool);
  ac.setTemp(20);
  ac.setFan(kNeoclimaFanHigh);
  ac.setSwingV(true);
  ac.setSwingH(true);
  ac.setTurbo(false);
  ac.setIon(true);
  ac.setLight(true);
  ac.setSleep(true);
  // Now test it.
  ASSERT_EQ(decode_type_t::NEOCLIMA, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_TRUE(ac.toCommon().filter);
  ASSERT_TRUE(ac.toCommon().light);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kAuto, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kAuto, ac.toCommon().swingh);
  ASSERT_EQ(0, ac.toCommon().sleep);
  // Unsupported.
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}
