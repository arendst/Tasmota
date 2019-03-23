// Copyright 2018 David Conran

#include "ir_Whirlpool.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendWhirlpoolAC().

// Test sending typical data only.
TEST(TestSendWhirlpoolAC, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();
  uint8_t data[kWhirlpoolAcStateLength] = {
      0x83, 0x06, 0x10, 0x71, 0x00, 0x00, 0x91, 0x1F, 0x00, 0x00, 0x00,
      0x00, 0x00, 0xEF, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02};

  irsend.sendWhirlpoolAC(data);
  EXPECT_EQ(
      "m8950s4484"
      "m597s1649m597s1649m597s533m597s533m597s533m597s533m597s533m597s1649"
      "m597s533m597s1649m597s1649m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s1649m597s533m597s533m597s533"
      "m597s1649m597s533m597s533m597s533m597s1649m597s1649m597s1649m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s7920"
      "m597s1649m597s533m597s533m597s533m597s1649m597s533m597s533m597s1649"
      "m597s1649m597s1649m597s1649m597s1649m597s1649m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s1649m597s1649m597s1649m597s1649m597s533m597s1649m597s1649m597s1649"
      "m597s7920"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s1649m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s1649m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s100000",
      irsend.outputStr());
}

// Tests for decodeWhirlpoolAC().
// Decode normal WhirlpoolAC messages.
TEST(TestDecodeWhirlpoolAC, SyntheticDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Synthesised Normal WhirlpoolAC message.
  irsend.reset();
  uint8_t expectedState[kWhirlpoolAcStateLength] = {
      0x83, 0x06, 0x10, 0x71, 0x00, 0x00, 0x91, 0x1F, 0x00, 0x00, 0x00,
      0x00, 0x00, 0xEF, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02};
  irsend.sendWhirlpoolAC(expectedState);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(WHIRLPOOL_AC, irsend.capture.decode_type);
  EXPECT_EQ(kWhirlpoolAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  IRWhirlpoolAc ac(0);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Model: 1 (DG11J13A), Power toggle: Off, Mode: 1 (AUTO), Temp: 25C, "
      "Fan: 0 (AUTO), Swing: Off, Light: On, Clock: 17:31, On Timer: Off, "
      "Off Timer: Off, Sleep: Off, Super: Off, Command: 2 (TEMP)",
      ac.toString());
}

TEST(TestDecodeWhirlpoolAC, Real26CFanAutoCoolingSwingOnClock1918) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  uint8_t expectedState[kWhirlpoolAcStateLength] = {
      0x83, 0x06, 0x80, 0x82, 0x00, 0x00, 0x93, 0x12, 0x40, 0x00, 0x00,
      0x00, 0x00, 0xC3, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x07};
  irsend.sendWhirlpoolAC(expectedState);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(WHIRLPOOL_AC, irsend.capture.decode_type);
  EXPECT_EQ(kWhirlpoolAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  IRWhirlpoolAc ac(0);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Model: 1 (DG11J13A), Power toggle: Off, Mode: 2 (COOL), Temp: 26C, "
      "Fan: 0 (AUTO), Swing: On, Light: On, Clock: 19:18, On Timer: Off, "
      "Off Timer: Off, Sleep: Off, Super: Off, Command: 7 (SWING)",
      ac.toString());
}

TEST(TestDecodeWhirlpoolAC, RealTimerExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  // Dehumidify timer on 7:40 off 8:05
  uint16_t rawData[343] = {
      9092, 4556, 604, 1664, 604, 1674, 630, 514,  630, 518,  628, 522,
      604,  550,  628, 530,  602, 1680, 630, 508,  630, 1644, 604, 1674,
      604,  544,  604, 548,  630, 524,  604, 554,  620, 530,  630, 506,
      602,  538,  602, 542,  604, 542,  604, 546,  630, 524,  602, 556,
      628,  518,  604, 1666, 632, 1644, 604, 540,  602, 546,  604, 1680,
      604,  1684, 604, 1686, 630, 520,  602, 534,  606, 538,  602, 540,
      604,  544,  604, 548,  602, 552,  630, 528,  602, 546,  602, 536,
      628,  510,  606, 540,  604, 544,  630, 522,  604, 554,  600, 554,
      602,  528,  602, 8032, 604, 1666, 604, 1668, 602, 1676, 630, 518,
      630,  520,  602, 550,  604, 554,  604, 1678, 630, 1640, 602, 1672,
      602,  542,  602, 544,  628, 522,  630, 1658, 604, 554,  628, 1652,
      630,  508,  602, 538,  630, 514,  630, 1652, 602, 546,  604, 550,
      602,  554,  602, 546,  630, 1638, 604, 536,  630, 1646, 602, 544,
      628,  522,  632, 524,  628, 528,  602, 1686, 594, 1666, 604, 1670,
      602,  1674, 632, 516,  604, 546,  638, 518,  622, 534,  628, 518,
      604,  532,  604, 536,  600, 550,  622, 1652, 630, 520,  602, 1684,
      602,  554,  602, 544,  630, 506,  628, 512,  602, 540,  628, 518,
      602,  550,  602, 552,  604, 554,  602, 544,  628, 1642, 602, 536,
      632,  1646, 630, 516,  602, 1680, 630, 1656, 604, 1688, 602, 1660,
      602,  8030, 604, 532,  604, 536,  604, 540,  602, 544,  628, 522,
      602,  552,  602, 556,  602, 544,  602, 1666, 630, 510,  602, 1674,
      604,  544,  628, 522,  602, 552,  630, 526,  628, 520,  602, 534,
      630,  510,  604, 540,  602, 544,  606, 544,  604, 550,  604, 554,
      602,  544,  604, 534,  602, 538,  602, 542,  604, 542,  604, 546,
      604,  550,  632, 526,  604, 544,  630, 506,  604, 536,  604, 540,
      628,  518,  602, 548,  604, 550,  604, 552,  630, 516,  602, 534,
      604,  536,  630, 512,  604, 544,  602, 548,  630, 524,  602, 554,
      602,  542,  604, 1666, 606, 532,  630, 1644, 602, 544,  630, 520,
      604,  550,  604, 554,  602, 526,  598};
  uint8_t expectedState[kWhirlpoolAcStateLength] = {
      0x83, 0x06, 0x00, 0x73, 0x00, 0x00, 0x87, 0xA3, 0x08, 0x85, 0x07,
      0x28, 0x00, 0xF5, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x05};
  irsend.sendRaw(rawData, 343, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(WHIRLPOOL_AC, irsend.capture.decode_type);
  EXPECT_EQ(kWhirlpoolAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  IRWhirlpoolAc ac(0);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Model: 1 (DG11J13A), Power toggle: Off, Mode: 3 (DRY), Temp: 25C, "
      "Fan: 0 (AUTO), Swing: Off, Light: On, Clock: 07:35, On Timer: 07:40, "
      "Off Timer: 08:05, Sleep: Off, Super: Off, Command: 5 (ONTIMER)",
      ac.toString());
}

// Decode a recorded example
TEST(TestDecodeWhirlpoolAC, RealExampleDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Real WhirlpoolAC message.
  // Ref: https://github.com/markszabo/IRremoteESP8266/issues/509
  uint16_t rawData[343] = {
      8950, 4484, 598, 1642, 598, 1646, 594, 534,  594, 538,  602, 532,
      598,  540,  600, 542,  598, 1650, 600, 522,  598, 1644, 596, 1650,
      600,  532,  598, 538,  602, 536,  594, 548,  592, 538,  602, 518,
      600,  524,  596, 532,  598, 532,  598, 1654, 596, 544,  596, 544,
      596,  536,  594, 1644, 596, 528,  600, 528,  592, 538,  602, 1648,
      602,  1654, 596, 1664, 598, 534,  594, 526,  594, 530,  598, 528,
      602,  530,  600, 534,  596, 542,  598, 542,  598, 534,  596, 526,
      594,  530,  600, 528,  602, 530,  600, 534,  596, 542,  598, 544,
      596,  518,  602, 7916, 598, 1642, 598, 528,  600, 528,  602, 530,
      600,  1652, 598, 542,  598, 544,  596, 1654, 596, 1644, 596, 1648,
      602,  1644, 596, 1654, 596, 1656, 604, 536,  594, 548,  602, 528,
      600,  520,  600, 524,  596, 532,  598, 532,  596, 538,  602, 536,
      594,  546,  594, 538,  602, 518,  600, 524,  596, 532,  598, 532,
      598,  536,  594, 544,  596, 544,  596, 536,  594, 526,  592, 530,
      600,  528,  600, 530,  602, 532,  596, 542,  598, 542,  598, 534,
      596,  524,  596, 528,  600, 526,  592, 538,  592, 542,  598, 540,
      600,  540,  600, 530,  598, 522,  598, 526,  594, 534,  596, 534,
      594,  540,  602, 536,  592, 548,  592, 538,  600, 1636, 594, 1648,
      602,  1642, 598, 1652, 598, 538,  602, 1680, 570, 1662, 598, 1634,
      596,  7924, 600, 520,  598, 526,  592, 534,  596, 534,  596, 540,
      600,  536,  604, 538,  602, 530,  600, 520,  598, 1640, 600, 528,
      600,  530,  600, 534,  594, 544,  596, 544,  596, 534,  596, 526,
      594,  528,  600, 526,  594, 536,  592, 542,  598, 538,  602, 538,
      602,  528,  600, 520,  600, 524,  596, 530,  600, 532,  598, 534,
      596,  542,  598, 542,  598, 532,  598, 524,  596, 528,  602, 526,
      594,  536,  594, 540,  600, 536,  594, 548,  592, 538,  602, 518,
      602,  522,  596, 530,  600, 530,  600, 534,  596, 542,  598, 544,
      596,  534,  596, 524,  594, 1644, 596, 532,  596, 534,  596, 538,
      602,  536,  594, 546,  594, 520,  600};
  uint8_t expectedState[kWhirlpoolAcStateLength] = {
      0x83, 0x06, 0x10, 0x71, 0x00, 0x00, 0x91, 0x1F, 0x00, 0x00, 0x00,
      0x00, 0x00, 0xEF, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02};

  irsend.reset();
  irsend.sendRaw(rawData, 343, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(WHIRLPOOL_AC, irsend.capture.decode_type);
  EXPECT_EQ(kWhirlpoolAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  IRWhirlpoolAc ac(0);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Model: 1 (DG11J13A), Power toggle: Off, Mode: 1 (AUTO), Temp: 25C, "
      "Fan: 0 (AUTO), Swing: Off, Light: On, Clock: 17:31, On Timer: Off, "
      "Off Timer: Off, Sleep: Off, Super: Off, Command: 2 (TEMP)",
      ac.toString());
}

// Tests for IRWhirlpoolAc class.

TEST(TestIRWhirlpoolAcClass, SetAndGetRaw) {
  uint8_t expectedState[kWhirlpoolAcStateLength] = {
      0x83, 0x06, 0x10, 0x71, 0x00, 0x00, 0x91, 0x1F, 0x00, 0x00, 0x00,
      0x00, 0x00, 0xEF, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02};
  IRWhirlpoolAc ac(0);
  ac.setRaw(expectedState);
  EXPECT_STATE_EQ(expectedState, ac.getRaw(), kWhirlpoolAcBits);
}

TEST(TestIRWhirlpoolAcClass, SetAndGetTemp) {
  IRWhirlpoolAc ac(0);
  ac.setCommand(0);  // Clear the previous command.

  ac.setModel(DG11J13A);
  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());
  EXPECT_EQ(kWhirlpoolAcCommandTemp, ac.getCommand());
  ac.setTemp(kWhirlpoolAcMinTemp);
  EXPECT_EQ(kWhirlpoolAcMinTemp, ac.getTemp());
  ac.setTemp(kWhirlpoolAcMinTemp - 1);
  EXPECT_EQ(kWhirlpoolAcMinTemp, ac.getTemp());
  ac.setTemp(kWhirlpoolAcMaxTemp);
  EXPECT_EQ(kWhirlpoolAcMaxTemp, ac.getTemp());
  ac.setTemp(kWhirlpoolAcMaxTemp + 1);
  EXPECT_EQ(kWhirlpoolAcMaxTemp, ac.getTemp());

  ac.setModel(DG11J191);  // Has a -2 offset on min/max temps.
  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());
  EXPECT_EQ(kWhirlpoolAcCommandTemp, ac.getCommand());
  ac.setTemp(kWhirlpoolAcMinTemp - 2);
  EXPECT_EQ(kWhirlpoolAcMinTemp - 2, ac.getTemp());
  ac.setTemp(kWhirlpoolAcMinTemp - 2 - 1);
  EXPECT_EQ(kWhirlpoolAcMinTemp - 2 , ac.getTemp());
  ac.setTemp(kWhirlpoolAcMaxTemp - 2);
  EXPECT_EQ(kWhirlpoolAcMaxTemp - 2, ac.getTemp());
  ac.setTemp(kWhirlpoolAcMaxTemp - 2 + 1);
  EXPECT_EQ(kWhirlpoolAcMaxTemp - 2, ac.getTemp());
}

TEST(TestIRWhirlpoolAcClass, SetAndGetMode) {
  IRWhirlpoolAc ac(0);
  ac.setCommand(0);  // Clear the previous command.

  ac.setMode(kWhirlpoolAcCool);
  EXPECT_EQ(kWhirlpoolAcCool, ac.getMode());
  EXPECT_EQ(kWhirlpoolAcCommandMode, ac.getCommand());
  ac.setMode(kWhirlpoolAcHeat);
  EXPECT_EQ(kWhirlpoolAcHeat, ac.getMode());
  ac.setMode(kWhirlpoolAcAuto);
  EXPECT_EQ(kWhirlpoolAcAuto, ac.getMode());
  EXPECT_EQ(kWhirlpoolAcCommand6thSense, ac.getCommand());
  ac.setMode(kWhirlpoolAcDry);
  EXPECT_EQ(kWhirlpoolAcDry, ac.getMode());
  EXPECT_EQ(kWhirlpoolAcCommandMode, ac.getCommand());
}

TEST(TestIRWhirlpoolAcClass, SetAndGetFan) {
  IRWhirlpoolAc ac(0);
  ac.setCommand(0);  // Clear the previous command.

  ac.setFan(kWhirlpoolAcFanAuto);
  EXPECT_EQ(kWhirlpoolAcFanAuto, ac.getFan());
  EXPECT_EQ(kWhirlpoolAcCommandFanSpeed, ac.getCommand());
  ac.setFan(kWhirlpoolAcFanLow);
  EXPECT_EQ(kWhirlpoolAcFanLow, ac.getFan());
  ac.setFan(kWhirlpoolAcFanMedium);
  EXPECT_EQ(kWhirlpoolAcFanMedium, ac.getFan());
  ac.setFan(kWhirlpoolAcFanHigh);
  EXPECT_EQ(kWhirlpoolAcFanHigh, ac.getFan());
  ac.setFan(kWhirlpoolAcFanAuto);
  EXPECT_EQ(kWhirlpoolAcFanAuto, ac.getFan());

  // Known state with a non-auto fan mode.
  const uint8_t state[21] = {0x83, 0x06, 0x0B, 0x82, 0x00, 0x00, 0x93,
                             0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E,
                             0x00, 0x03, 0x00, 0x00, 0x08, 0x00, 0x0B};
  ac.setRaw(state);
  EXPECT_EQ(kWhirlpoolAcFanLow, ac.getFan());
}

TEST(TestIRWhirlpoolAcClass, SetAndGetSwing) {
  IRWhirlpoolAc ac(0);
  ac.setCommand(0);  // Clear the previous command.

  ac.setSwing(true);
  EXPECT_TRUE(ac.getSwing());
  EXPECT_EQ(kWhirlpoolAcCommandSwing, ac.getCommand());
  ac.setSwing(false);
  EXPECT_FALSE(ac.getSwing());
  ac.setSwing(true);
  EXPECT_TRUE(ac.getSwing());
}

TEST(TestIRWhirlpoolAcClass, SetAndGetLight) {
  IRWhirlpoolAc ac(0);
  ac.setCommand(0);  // Clear the previous command.

  ac.setLight(true);
  EXPECT_TRUE(ac.getLight());
  ac.setLight(false);
  EXPECT_FALSE(ac.getLight());
  ac.setLight(true);
  EXPECT_TRUE(ac.getLight());
}

TEST(TestIRWhirlpoolAcClass, SetAndGetClock) {
  IRWhirlpoolAc ac(0);
  ac.setClock(0);
  EXPECT_EQ(0, ac.getClock());
  EXPECT_EQ("00:00", ac.timeToString(ac.getClock()));
  ac.setClock(1);
  EXPECT_EQ(1, ac.getClock());
  EXPECT_EQ("00:01", ac.timeToString(ac.getClock()));
  ac.setClock(12 * 60 + 34);
  EXPECT_EQ(12 * 60 + 34, ac.getClock());
  EXPECT_EQ("12:34", ac.timeToString(ac.getClock()));
  ac.setClock(7 * 60 + 5);
  EXPECT_EQ(7 * 60 + 5, ac.getClock());
  EXPECT_EQ("07:05", ac.timeToString(ac.getClock()));
  ac.setClock(23 * 60 + 59);
  EXPECT_EQ(23 * 60 + 59, ac.getClock());
  EXPECT_EQ("23:59", ac.timeToString(ac.getClock()));
  ac.setClock(24 * 60 + 0);
  EXPECT_EQ(0, ac.getClock());
  EXPECT_EQ("00:00", ac.timeToString(ac.getClock()));
  ac.setClock(25 * 60 + 23);
  EXPECT_EQ(1 * 60 + 23, ac.getClock());
  EXPECT_EQ("01:23", ac.timeToString(ac.getClock()));
}

TEST(TestIRWhirlpoolAcClass, OnOffTimers) {
  IRWhirlpoolAc ac(0);
  ac.setCommand(0);  // Clear the previous command.

  // On Timer
  ac.enableOnTimer(false);
  ac.setOnTimer(0);
  EXPECT_EQ(0, ac.getOnTimer());
  EXPECT_EQ("00:00", ac.timeToString(ac.getOnTimer()));
  EXPECT_FALSE(ac.isOnTimerEnabled());
  EXPECT_EQ(kWhirlpoolAcCommandOnTimer, ac.getCommand());
  ac.setOnTimer(1);
  EXPECT_EQ(1, ac.getOnTimer());
  EXPECT_EQ("00:01", ac.timeToString(ac.getOnTimer()));
  ac.enableOnTimer(true);
  ac.setOnTimer(12 * 60 + 34);
  EXPECT_EQ(12 * 60 + 34, ac.getOnTimer());
  EXPECT_EQ("12:34", ac.timeToString(ac.getOnTimer()));
  EXPECT_TRUE(ac.isOnTimerEnabled());
  ac.setOnTimer(7 * 60 + 5);
  EXPECT_EQ(7 * 60 + 5, ac.getOnTimer());
  EXPECT_EQ("07:05", ac.timeToString(ac.getOnTimer()));
  ac.setOnTimer(23 * 60 + 59);
  EXPECT_EQ(23 * 60 + 59, ac.getOnTimer());
  EXPECT_EQ("23:59", ac.timeToString(ac.getOnTimer()));
  ac.setOnTimer(24 * 60 + 0);
  EXPECT_EQ(0, ac.getOnTimer());
  EXPECT_EQ("00:00", ac.timeToString(ac.getOnTimer()));
  ac.setOnTimer(25 * 60 + 23);
  EXPECT_EQ(1 * 60 + 23, ac.getOnTimer());
  EXPECT_EQ("01:23", ac.timeToString(ac.getOnTimer()));
  // Off Timer
  ac.enableOffTimer(false);
  ac.setOffTimer(0);
  EXPECT_EQ(0, ac.getOffTimer());
  EXPECT_EQ("00:00", ac.timeToString(ac.getOffTimer()));
  EXPECT_FALSE(ac.isOffTimerEnabled());
  EXPECT_EQ(kWhirlpoolAcCommandOffTimer, ac.getCommand());
  ac.setOffTimer(1);
  EXPECT_EQ(1, ac.getOffTimer());
  EXPECT_EQ("00:01", ac.timeToString(ac.getOffTimer()));
  ac.enableOffTimer(true);
  ac.setOffTimer(12 * 60 + 34);
  EXPECT_EQ(12 * 60 + 34, ac.getOffTimer());
  EXPECT_EQ("12:34", ac.timeToString(ac.getOffTimer()));
  EXPECT_TRUE(ac.isOffTimerEnabled());
  ac.setOffTimer(7 * 60 + 5);
  EXPECT_EQ(7 * 60 + 5, ac.getOffTimer());
  EXPECT_EQ("07:05", ac.timeToString(ac.getOffTimer()));
  ac.setOffTimer(23 * 60 + 59);
  EXPECT_EQ(23 * 60 + 59, ac.getOffTimer());
  EXPECT_EQ("23:59", ac.timeToString(ac.getOffTimer()));
  ac.setOffTimer(24 * 60 + 0);
  EXPECT_EQ(0, ac.getOffTimer());
  EXPECT_EQ("00:00", ac.timeToString(ac.getOffTimer()));
  ac.setOffTimer(25 * 60 + 23);
  EXPECT_EQ(1 * 60 + 23, ac.getOffTimer());
  EXPECT_EQ("01:23", ac.timeToString(ac.getOffTimer()));
}

TEST(TestIRWhirlpoolAcClass, SetAndGetCommand) {
  IRWhirlpoolAc ac(0);
  ac.setCommand(0);
  EXPECT_EQ(0, ac.getCommand());
  ac.setCommand(kWhirlpoolAcCommandFanSpeed);
  EXPECT_EQ(kWhirlpoolAcCommandFanSpeed, ac.getCommand());
  ac.setCommand(255);
  EXPECT_EQ(255, ac.getCommand());
}

TEST(TestIRWhirlpoolAcClass, SetAndGetPowerToggle) {
  IRWhirlpoolAc ac(0);
  ac.setCommand(0);

  ac.setPowerToggle(false);
  EXPECT_FALSE(ac.getPowerToggle());
  ac.setPowerToggle(true);
  EXPECT_TRUE(ac.getPowerToggle());
  ac.setPowerToggle(false);
  EXPECT_FALSE(ac.getPowerToggle());

  // Known state with a power toggle in it.
  uint8_t state[21] = {0x83, 0x06, 0x07, 0x82, 0x00, 0x00, 0x93,
                       0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12,
                       0x00, 0x01, 0x00, 0x00, 0x08, 0x00, 0x09};
  ac.setRaw(state);
  EXPECT_TRUE(ac.getPowerToggle());
}

TEST(TestIRWhirlpoolAcClass, SetAndGetModel) {
  IRWhirlpoolAc ac(0);
  ac.setTemp(19);
  ac.setCommand(0);  // Set model shouldn't change the command setting.

  ac.setModel(DG11J191);
  EXPECT_EQ(DG11J191, ac.getModel());
  EXPECT_EQ(19, ac.getTemp());
  EXPECT_EQ(0, ac.getCommand());
  ac.setModel(DG11J13A);
  EXPECT_EQ(DG11J13A, ac.getModel());
  EXPECT_EQ(19, ac.getTemp());
  ac.setModel(DG11J191);
  EXPECT_EQ(DG11J191, ac.getModel());
  EXPECT_EQ(19, ac.getTemp());
  EXPECT_EQ(0, ac.getCommand());

  // One of the models has a lower min temp. Check that desired temp is kept.
  ac.setTemp(16);
  ac.setCommand(0);  // Set model shouldn't change the command setting.
  EXPECT_EQ(16, ac.getTemp());
  EXPECT_EQ(0, ac.getCommand());
  ac.setModel(DG11J13A);
  EXPECT_EQ(DG11J13A, ac.getModel());
  EXPECT_EQ(18, ac.getTemp());
  ac.setModel(DG11J191);
  EXPECT_EQ(DG11J191, ac.getModel());
  EXPECT_EQ(16, ac.getTemp());
  EXPECT_EQ(0, ac.getCommand());

  // Known states with different models.
  uint8_t state_1[21] = {0x83, 0x06, 0x01, 0x30, 0x00, 0x00, 0x92,
                         0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95,
                         0x00, 0x02, 0x00, 0x00, 0x08, 0x00, 0x0A};
  uint8_t state_2[21] = {0x83, 0x06, 0x00, 0x30, 0x00, 0x00, 0x8B,
                         0x35, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8E,
                         0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02};

  ac.setRaw(state_1);
  EXPECT_EQ(DG11J191, ac.getModel());
  ac.setRaw(state_2);
  EXPECT_EQ(DG11J13A, ac.getModel());
}

TEST(TestIRWhirlpoolAcClass, SetAndGetSleep) {
  IRWhirlpoolAc ac(0);
  ac.setFan(kWhirlpoolAcFanAuto);
  ac.setCommand(0);

  ac.setSleep(false);
  EXPECT_FALSE(ac.getSleep());
  EXPECT_EQ(kWhirlpoolAcCommandSleep, ac.getCommand());
  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
  EXPECT_EQ(kWhirlpoolAcCommandSleep, ac.getCommand());
  EXPECT_EQ(kWhirlpoolAcFanLow, ac.getFan());
  ac.setSleep(false);
  EXPECT_FALSE(ac.getSleep());

  // Known state with sleep mode in it.
  uint8_t state[21] = {0x83, 0x06, 0x0B, 0x73, 0x00, 0x00, 0x90,
                       0x9E, 0x00, 0xA0, 0x17, 0x3A, 0x00, 0xFB,
                       0x00, 0x03, 0x00, 0x00, 0x08, 0x00, 0x0B};
  ac.setRaw(state);
  EXPECT_TRUE(ac.getSleep());
}

TEST(TestIRWhirlpoolAcClass, SetAndGetSuper) {
  IRWhirlpoolAc ac(0);
  ac.setFan(kWhirlpoolAcFanAuto);
  ac.setMode(kWhirlpoolAcDry);
  ac.setCommand(0);

  ac.setSuper(false);
  EXPECT_FALSE(ac.getSuper());
  EXPECT_EQ(kWhirlpoolAcCommandSuper, ac.getCommand());
  ac.setSuper(true);
  EXPECT_TRUE(ac.getSuper());
  EXPECT_EQ(kWhirlpoolAcCommandSuper, ac.getCommand());
  EXPECT_EQ(kWhirlpoolAcFanHigh, ac.getFan());
  EXPECT_EQ(kWhirlpoolAcCool, ac.getMode());
  EXPECT_EQ(kWhirlpoolAcMinTemp, ac.getTemp());

  ac.setSuper(false);
  EXPECT_FALSE(ac.getSuper());
  EXPECT_EQ(kWhirlpoolAcFanHigh, ac.getFan());
  EXPECT_EQ(kWhirlpoolAcCool, ac.getMode());
  EXPECT_EQ(kWhirlpoolAcMinTemp, ac.getTemp());

  // When in heat mode, it should stay in heat mode.
  ac.setFan(kWhirlpoolAcFanAuto);
  ac.setMode(kWhirlpoolAcHeat);
  ac.setSuper(true);
  EXPECT_TRUE(ac.getSuper());
  EXPECT_EQ(kWhirlpoolAcCommandSuper, ac.getCommand());
  EXPECT_EQ(kWhirlpoolAcFanHigh, ac.getFan());
  EXPECT_EQ(kWhirlpoolAcHeat, ac.getMode());
  EXPECT_EQ(kWhirlpoolAcMaxTemp, ac.getTemp());

  // Changing mode/temp/fan/power should cancel super,
  ac.setMode(kWhirlpoolAcCool);
  EXPECT_FALSE(ac.getSuper());
  ac.setSuper(true);
  ac.setTemp(25);
  EXPECT_FALSE(ac.getSuper());
  ac.setSuper(true);
  ac.setFan(kWhirlpoolAcFanMedium);
  EXPECT_FALSE(ac.getSuper());
  ac.setSuper(true);
  ac.setPowerToggle(true);
  EXPECT_FALSE(ac.getSuper());

  // Known state with Super mode in it.
  uint8_t state[21] = {0x83, 0x06, 0x01, 0x02, 0x00, 0x90, 0x90,
                       0x9F, 0x00, 0xA0, 0x17, 0x3A, 0x00, 0x11,
                       0x00, 0x04, 0x00, 0x00, 0x08, 0x00, 0x0C};
  ac.setRaw(state);
  EXPECT_TRUE(ac.getSuper());
}

// Build a known good message from scratch.
TEST(TestIRWhirlpoolAcClass, MessageConstruction) {
  // Real example captured from a remote. (ref: RealTimerExample)
  uint8_t expectedState[kWhirlpoolAcStateLength] = {
      0x83, 0x06, 0x00, 0x73, 0x00, 0x00, 0x87, 0xA3, 0x08, 0x85, 0x07,
      0x28, 0x00, 0xF5, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x05};
  IRWhirlpoolAc ac(0);
  ac.setModel(DG11J13A);
  ac.setTemp(25);
  ac.setPowerToggle(false);
  ac.setMode(kWhirlpoolAcDry);
  ac.setFan(kWhirlpoolAcFanAuto);
  ac.setSwing(false);
  ac.setLight(true);
  ac.setClock(7 * 60 + 35);
  ac.setOnTimer(7 * 60 + 40);
  ac.setOffTimer(8 * 60 + 5);
  ac.enableOffTimer(true);
  ac.setSleep(false);
  ac.setSuper(false);
  ac.enableOnTimer(true);

  EXPECT_EQ(
      "Model: 1 (DG11J13A), Power toggle: Off, Mode: 3 (DRY), Temp: 25C, "
      "Fan: 0 (AUTO), Swing: Off, Light: On, Clock: 07:35, On Timer: 07:40, "
      "Off Timer: 08:05, Sleep: Off, Super: Off, Command: 5 (ONTIMER)",
      ac.toString());
  EXPECT_STATE_EQ(expectedState, ac.getRaw(), kWhirlpoolAcBits);
}
