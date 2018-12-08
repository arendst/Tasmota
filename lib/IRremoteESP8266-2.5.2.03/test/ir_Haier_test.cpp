// Copyright 2018 David Conran

#include "ir_Haier.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendHaierAC()

// Test sending typical data only.
TEST(TestSendHaierAC, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();
  uint8_t haier_zero[kHaierACStateLength] = {0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x00};

  irsend.reset();
  irsend.sendHaierAC(haier_zero);
  EXPECT_EQ(
      "m3000s3000m3000s4300"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s150000",
      irsend.outputStr());

  uint8_t haier_test[kHaierACStateLength] = {0xA5, 0x01, 0x20, 0x01, 0x00,
                                             0xC0, 0x20, 0x00, 0xA7};
  irsend.reset();
  irsend.sendHaierAC(haier_test);
  EXPECT_EQ(
      "m3000s3000m3000s4300"
      "m520s1650m520s650m520s1650m520s650m520s650m520s1650m520s650m520s1650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s1650"
      "m520s650m520s650m520s1650m520s650m520s650m520s650m520s650m520s650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s1650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s1650m520s1650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s650m520s650m520s1650m520s650m520s650m520s650m520s650m520s650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s1650m520s650m520s1650m520s650m520s650m520s1650m520s1650m520s1650"
      "m520s150000",
      irsend.outputStr());
}

// Test sending typical data with repeats.
TEST(TestSendHaierAC, SendWithRepeats) {
  IRsendTest irsend(0);
  irsend.begin();

  irsend.reset();
  uint8_t haier_test[kHaierACStateLength] = {0xA5, 0x01, 0x20, 0x01, 0x00,
                                             0xC0, 0x20, 0x00, 0xA7};
  irsend.reset();
  irsend.sendHaierAC(haier_test, kHaierACStateLength, 2);  // two repeats.
  EXPECT_EQ(
      "m3000s3000m3000s4300"
      "m520s1650m520s650m520s1650m520s650m520s650m520s1650m520s650m520s1650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s1650"
      "m520s650m520s650m520s1650m520s650m520s650m520s650m520s650m520s650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s1650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s1650m520s1650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s650m520s650m520s1650m520s650m520s650m520s650m520s650m520s650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s1650m520s650m520s1650m520s650m520s650m520s1650m520s1650m520s1650"
      "m520s150000"
      "m3000s3000m3000s4300"
      "m520s1650m520s650m520s1650m520s650m520s650m520s1650m520s650m520s1650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s1650"
      "m520s650m520s650m520s1650m520s650m520s650m520s650m520s650m520s650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s1650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s1650m520s1650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s650m520s650m520s1650m520s650m520s650m520s650m520s650m520s650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s1650m520s650m520s1650m520s650m520s650m520s1650m520s1650m520s1650"
      "m520s150000"
      "m3000s3000m3000s4300"
      "m520s1650m520s650m520s1650m520s650m520s650m520s1650m520s650m520s1650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s1650"
      "m520s650m520s650m520s1650m520s650m520s650m520s650m520s650m520s650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s1650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s1650m520s1650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s650m520s650m520s1650m520s650m520s650m520s650m520s650m520s650"
      "m520s650m520s650m520s650m520s650m520s650m520s650m520s650m520s650"
      "m520s1650m520s650m520s1650m520s650m520s650m520s1650m520s1650m520s1650"
      "m520s150000",
      irsend.outputStr());
}

// Tests for IRHaierAC class.

TEST(TestHaierACClass, Command) {
  IRHaierAC haier(0);
  haier.begin();

  haier.setCommand(kHaierAcCmdOff);
  EXPECT_EQ(kHaierAcCmdOff, haier.getCommand());
  haier.setCommand(kHaierAcCmdOn);
  EXPECT_EQ(kHaierAcCmdOn, haier.getCommand());
  haier.setCommand(kHaierAcCmdMode);
  EXPECT_EQ(kHaierAcCmdMode, haier.getCommand());
  haier.setCommand(kHaierAcCmdFan);
  EXPECT_EQ(kHaierAcCmdFan, haier.getCommand());
  haier.setCommand(kHaierAcCmdTempUp);
  EXPECT_EQ(kHaierAcCmdTempUp, haier.getCommand());
  haier.setCommand(kHaierAcCmdTempDown);
  EXPECT_EQ(kHaierAcCmdTempDown, haier.getCommand());
  haier.setCommand(kHaierAcCmdSleep);
  EXPECT_EQ(kHaierAcCmdSleep, haier.getCommand());
  haier.setCommand(kHaierAcCmdTimerSet);
  EXPECT_EQ(kHaierAcCmdTimerSet, haier.getCommand());
  haier.setCommand(kHaierAcCmdTimerCancel);
  EXPECT_EQ(kHaierAcCmdTimerCancel, haier.getCommand());
  haier.setCommand(kHaierAcCmdHealth);
  EXPECT_EQ(kHaierAcCmdHealth, haier.getCommand());
  haier.setCommand(kHaierAcCmdSwing);
  EXPECT_EQ(kHaierAcCmdSwing, haier.getCommand());
  haier.setCommand(kHaierAcCmdOn);
  EXPECT_EQ(kHaierAcCmdOn, haier.getCommand());

  // Test unexpected values.
  haier.setCommand(0b00001110);
  EXPECT_EQ(kHaierAcCmdOff, haier.getCommand());
  haier.setCommand(0b00001111);
  EXPECT_EQ(kHaierAcCmdOff, haier.getCommand());
  haier.setCommand(0b00000100);
  EXPECT_EQ(kHaierAcCmdOff, haier.getCommand());
}

TEST(TestHaierACClass, OperatingMode) {
  IRHaierAC haier(0);
  haier.begin();

  haier.setMode(kHaierAcAuto);
  EXPECT_EQ(kHaierAcAuto, haier.getMode());

  haier.setMode(kHaierAcCool);
  EXPECT_EQ(kHaierAcCool, haier.getMode());

  haier.setMode(kHaierAcHeat);
  EXPECT_EQ(kHaierAcHeat, haier.getMode());

  haier.setMode(kHaierAcFan);
  EXPECT_EQ(kHaierAcFan, haier.getMode());

  haier.setMode(kHaierAcDry);
  EXPECT_EQ(kHaierAcDry, haier.getMode());

  haier.setMode(kHaierAcAuto - 1);
  EXPECT_EQ(kHaierAcAuto, haier.getMode());

  haier.setMode(kHaierAcCool);
  EXPECT_EQ(kHaierAcCool, haier.getMode());

  haier.setMode(kHaierAcFan + 1);
  EXPECT_EQ(kHaierAcAuto, haier.getMode());

  haier.setMode(255);
  EXPECT_EQ(kHaierAcAuto, haier.getMode());
}

TEST(TestHaierACClass, Temperature) {
  IRHaierAC haier(0);
  haier.begin();

  haier.setTemp(kHaierAcMinTemp);
  EXPECT_EQ(kHaierAcMinTemp, haier.getTemp());

  haier.setCommand(kHaierAcCmdOn);
  haier.setTemp(kHaierAcMinTemp + 1);
  EXPECT_EQ(kHaierAcMinTemp + 1, haier.getTemp());
  EXPECT_EQ(kHaierAcCmdTempUp, haier.getCommand());

  haier.setTemp(kHaierAcMaxTemp);
  EXPECT_EQ(kHaierAcMaxTemp, haier.getTemp());
  EXPECT_EQ(kHaierAcCmdTempUp, haier.getCommand());

  haier.setTemp(kHaierAcMinTemp - 1);
  EXPECT_EQ(kHaierAcMinTemp, haier.getTemp());
  EXPECT_EQ(kHaierAcCmdTempDown, haier.getCommand());

  haier.setTemp(kHaierAcMaxTemp + 1);
  EXPECT_EQ(kHaierAcMaxTemp, haier.getTemp());
  EXPECT_EQ(kHaierAcCmdTempUp, haier.getCommand());

  haier.setTemp(23);
  EXPECT_EQ(23, haier.getTemp());
  EXPECT_EQ(kHaierAcCmdTempDown, haier.getCommand());
  haier.setCommand(kHaierAcCmdOn);
  haier.setTemp(23);
  EXPECT_EQ(23, haier.getTemp());
  EXPECT_EQ(kHaierAcCmdOn, haier.getCommand());

  haier.setTemp(0);
  EXPECT_EQ(kHaierAcMinTemp, haier.getTemp());
  EXPECT_EQ(kHaierAcCmdTempDown, haier.getCommand());

  haier.setTemp(255);
  EXPECT_EQ(kHaierAcMaxTemp, haier.getTemp());
  EXPECT_EQ(kHaierAcCmdTempUp, haier.getCommand());
}

TEST(TestHaierACClass, FanSpeed) {
  IRHaierAC haier(0);
  haier.begin();
  haier.setFan(kHaierAcFanLow);
  haier.setCommand(kHaierAcCmdOn);

  haier.setFan(kHaierAcFanAuto);
  EXPECT_EQ(kHaierAcFanAuto, haier.getFan());
  EXPECT_EQ(kHaierAcCmdFan, haier.getCommand());

  haier.setFan(kHaierAcFanLow);
  EXPECT_EQ(kHaierAcFanLow, haier.getFan());
  haier.setFan(kHaierAcFanMed);
  EXPECT_EQ(kHaierAcFanMed, haier.getFan());
  haier.setFan(kHaierAcFanHigh);
  EXPECT_EQ(kHaierAcFanHigh, haier.getFan());

  haier.setCommand(kHaierAcCmdOn);
  haier.setFan(kHaierAcFanHigh);
  EXPECT_EQ(kHaierAcFanHigh, haier.getFan());
  EXPECT_EQ(kHaierAcCmdOn, haier.getCommand());
}

TEST(TestHaierACClass, Swing) {
  IRHaierAC haier(0);
  haier.begin();
  haier.setFan(kHaierAcFanLow);
  haier.setCommand(kHaierAcCmdOn);

  haier.setSwing(kHaierAcSwingOff);
  EXPECT_EQ(kHaierAcSwingOff, haier.getSwing());

  haier.setSwing(kHaierAcSwingUp);
  EXPECT_EQ(kHaierAcSwingUp, haier.getSwing());
  EXPECT_EQ(kHaierAcCmdSwing, haier.getCommand());

  haier.setSwing(kHaierAcSwingDown);
  EXPECT_EQ(kHaierAcSwingDown, haier.getSwing());
  EXPECT_EQ(kHaierAcCmdSwing, haier.getCommand());

  haier.setSwing(kHaierAcSwingChg);
  EXPECT_EQ(kHaierAcSwingChg, haier.getSwing());
  EXPECT_EQ(kHaierAcCmdSwing, haier.getCommand());
}

TEST(TestHaierACClass, CurrentTime) {
  IRHaierAC haier(0);
  haier.begin();
  EXPECT_EQ(0, haier.getCurrTime());

  haier.setCurrTime(1);
  EXPECT_EQ(1, haier.getCurrTime());

  haier.setCurrTime(60);
  EXPECT_EQ(60, haier.getCurrTime());

  haier.setCurrTime(61);
  EXPECT_EQ(61, haier.getCurrTime());

  haier.setCurrTime(18 * 60 + 34);  // 18:34
  EXPECT_EQ(1114, haier.getCurrTime());

  haier.setCurrTime(23 * 60 + 59);                  // 23:59
  EXPECT_EQ(kHaierAcMaxTime, haier.getCurrTime());  // 23:59

  haier.setCurrTime(23 * 60 + 59 + 1);              // 24:00
  EXPECT_EQ(kHaierAcMaxTime, haier.getCurrTime());  // 23:59

  haier.setCurrTime(UINT16_MAX);
  EXPECT_EQ(kHaierAcMaxTime, haier.getCurrTime());  // 23:59
}

TEST(TestHaierACClass, Timers) {
  IRHaierAC haier(0);
  haier.begin();

  haier.setCommand(kHaierAcCmdOn);

  // Off by default.
  EXPECT_GT(0, haier.getOnTimer());
  EXPECT_GT(0, haier.getOffTimer());
  EXPECT_EQ(kHaierAcCmdOn, haier.getCommand());

  // On Timer.
  haier.setOnTimer(6 * 60);               // 6am
  EXPECT_EQ(6 * 60, haier.getOnTimer());  // 6am
  EXPECT_GT(0, haier.getOffTimer());
  EXPECT_EQ(kHaierAcCmdTimerSet, haier.getCommand());

  haier.setCommand(kHaierAcCmdOn);
  EXPECT_EQ(6 * 60, haier.getOnTimer());  // 6am
  EXPECT_GT(0, haier.getOffTimer());
  EXPECT_EQ(kHaierAcCmdOn, haier.getCommand());

  haier.cancelTimers();
  EXPECT_GT(0, haier.getOnTimer());
  EXPECT_GT(0, haier.getOffTimer());
  EXPECT_EQ(kHaierAcCmdTimerCancel, haier.getCommand());

  // Off Timer.
  haier.setOffTimer(18 * 60 + 30);  // 6:30pm
  EXPECT_GT(0, haier.getOnTimer());
  EXPECT_EQ(18 * 60 + 30, haier.getOffTimer());  // 6:30pm
  EXPECT_EQ(kHaierAcCmdTimerSet, haier.getCommand());

  haier.setCommand(kHaierAcCmdOn);
  EXPECT_GT(0, haier.getOnTimer());
  EXPECT_EQ(18 * 60 + 30, haier.getOffTimer());  // 6:30pm
  EXPECT_EQ(kHaierAcCmdOn, haier.getCommand());

  haier.cancelTimers();
  EXPECT_GT(0, haier.getOnTimer());
  EXPECT_GT(0, haier.getOffTimer());
  EXPECT_EQ(kHaierAcCmdTimerCancel, haier.getCommand());

  // Both Timers.
  haier.setOnTimer(6 * 60);  // 6am
  EXPECT_EQ(kHaierAcCmdTimerSet, haier.getCommand());
  haier.setOffTimer(18 * 60 + 30);  // 6:30pm
  EXPECT_EQ(kHaierAcCmdTimerSet, haier.getCommand());
  EXPECT_EQ(6 * 60, haier.getOnTimer());         // 6am
  EXPECT_EQ(18 * 60 + 30, haier.getOffTimer());  // 6:30pm

  haier.cancelTimers();
  EXPECT_GT(0, haier.getOnTimer());
  EXPECT_GT(0, haier.getOffTimer());
  EXPECT_EQ(kHaierAcCmdTimerCancel, haier.getCommand());
}

TEST(TestHaierACClass, TimeToString) {
  EXPECT_EQ("00:00", IRHaierAC::timeToString(0));
  EXPECT_EQ("00:01", IRHaierAC::timeToString(1));
  EXPECT_EQ("00:10", IRHaierAC::timeToString(10));
  EXPECT_EQ("00:59", IRHaierAC::timeToString(59));

  EXPECT_EQ("01:00", IRHaierAC::timeToString(60));
  EXPECT_EQ("01:01", IRHaierAC::timeToString(61));
  EXPECT_EQ("01:59", IRHaierAC::timeToString(60 + 59));
  EXPECT_EQ("18:59", IRHaierAC::timeToString(18 * 60 + 59));
  EXPECT_EQ("23:59", IRHaierAC::timeToString(23 * 60 + 59));
}

TEST(TestHaierACClass, MessageConstuction) {
  IRHaierAC haier(0);

  EXPECT_EQ(
      "Command: 1 (On), Mode: 0 (AUTO), Temp: 25C, Fan: 0 (AUTO), "
      "Swing: 0 (Off), Sleep: Off, Health: Off, "
      "Current Time: 00:00, On Timer: Off, Off Timer: Off",
      haier.toString());
  haier.setMode(kHaierAcCool);
  haier.setTemp(21);
  haier.setFan(kHaierAcFanHigh);
  EXPECT_EQ(
      "Command: 3 (Fan), Mode: 1 (COOL), Temp: 21C, Fan: 3 (MAX), "
      "Swing: 0 (Off), Sleep: Off, Health: Off, "
      "Current Time: 00:00, On Timer: Off, Off Timer: Off",
      haier.toString());
  haier.setSwing(kHaierAcSwingChg);
  haier.setHealth(true);
  haier.setSleep(true);
  haier.setCurrTime(615);  // 10:15am
  EXPECT_EQ(
      "Command: 8 (Sleep), Mode: 3 (HEAT), Temp: 21C, Fan: 3 (MAX), "
      "Swing: 3 (Chg), Sleep: On, Health: On, "
      "Current Time: 10:15, On Timer: Off, Off Timer: Off",
      haier.toString());
  haier.setOnTimer(800);    // 1:20pm
  haier.setOffTimer(1125);  // 6:45pm
  haier.setCommand(kHaierAcCmdOn);

  EXPECT_EQ(
      "Command: 1 (On), Mode: 2 (DRY), Temp: 21C, Fan: 2, "
      "Swing: 3 (Chg), Sleep: On, Health: On, "
      "Current Time: 10:15, On Timer: 13:20, Off Timer: 18:45",
      haier.toString());

  // Now change a few already set things.
  haier.setMode(kHaierAcHeat);
  EXPECT_EQ(
      "Command: 2 (Mode), Mode: 3 (HEAT), Temp: 21C, Fan: 2, "
      "Swing: 3 (Chg), Sleep: On, Health: On, "
      "Current Time: 10:15, On Timer: 13:52, Off Timer: 18:45",
      haier.toString());

  haier.setTemp(25);
  EXPECT_EQ(
      "Command: 6 (Temp Up), Mode: 3 (HEAT), Temp: 25C, Fan: 2, "
      "Swing: 3 (Chg), Sleep: On, Health: On, "
      "Current Time: 10:15, On Timer: 13:52, Off Timer: 18:45",
      haier.toString());

  uint8_t expectedState[kHaierACStateLength] = {0xA5, 0x96, 0xEA, 0xCF, 0x32,
                                                0x2D, 0x0D, 0x74, 0xD4};
  EXPECT_STATE_EQ(expectedState, haier.getRaw(), kHaierACBits);

  // Check that the checksum is valid.
  EXPECT_TRUE(IRHaierAC::validChecksum(haier.getRaw()));

  // Now load up some random data.
  uint8_t randomState[kHaierACStateLength] = {0x52, 0x49, 0x50, 0x20, 0x54,
                                              0x61, 0x6C, 0x69, 0x61};
  EXPECT_FALSE(IRHaierAC::validChecksum(randomState));
  haier.setRaw(randomState);
  EXPECT_EQ(
      "Command: 9 (Timer Set), Mode: 3 (HEAT), Temp: 20C, Fan: 2, "
      "Swing: 1 (Up), Sleep: On, Health: Off, "
      "Current Time: 16:32, On Timer: Off, Off Timer: Off",
      haier.toString());
  // getRaw() should correct the checksum.
  EXPECT_TRUE(IRHaierAC::validChecksum(haier.getRaw()));
}

// Tests for the IRHaierACYRW02 class.

TEST(TestHaierACYRW02Class, Button) {
  IRHaierACYRW02 haier(0);
  haier.begin();

  haier.setButton(kHaierAcYrw02ButtonPower);
  EXPECT_EQ(kHaierAcYrw02ButtonPower, haier.getButton());
  haier.setButton(kHaierAcYrw02ButtonMode);
  EXPECT_EQ(kHaierAcYrw02ButtonMode, haier.getButton());
  haier.setButton(kHaierAcYrw02ButtonSleep);
  EXPECT_EQ(kHaierAcYrw02ButtonSleep, haier.getButton());
  haier.setButton(kHaierAcYrw02ButtonFan);

  // Test unexpected values.
  haier.setButton(0xFF);
  EXPECT_EQ(kHaierAcYrw02ButtonFan, haier.getButton());
  haier.setButton(0x10);
  EXPECT_EQ(kHaierAcYrw02ButtonFan, haier.getButton());
}

TEST(TestHaierACYRW02Class, OperatingMode) {
  IRHaierACYRW02 haier(0);
  haier.begin();

  haier.setButton(kHaierAcYrw02ButtonPower);
  haier.setMode(kHaierAcYrw02Auto);
  EXPECT_EQ(kHaierAcYrw02Auto, haier.getMode());
  EXPECT_EQ(kHaierAcYrw02ButtonMode, haier.getButton());

  haier.setMode(kHaierAcYrw02Cool);
  EXPECT_EQ(kHaierAcYrw02Cool, haier.getMode());

  haier.setMode(kHaierAcYrw02Heat);
  EXPECT_EQ(kHaierAcYrw02Heat, haier.getMode());

  haier.setMode(kHaierAcYrw02Fan);
  EXPECT_EQ(kHaierAcYrw02Fan, haier.getMode());

  haier.setMode(kHaierAcYrw02Dry);
  EXPECT_EQ(kHaierAcYrw02Dry, haier.getMode());

  haier.setMode(kHaierAcYrw02Auto - 1);
  EXPECT_EQ(kHaierAcYrw02Auto, haier.getMode());

  haier.setMode(kHaierAcYrw02Cool);
  EXPECT_EQ(kHaierAcYrw02Cool, haier.getMode());

  haier.setMode(kHaierAcYrw02Fan + 1);
  EXPECT_EQ(kHaierAcYrw02Auto, haier.getMode());

  haier.setMode(255);
  EXPECT_EQ(kHaierAcYrw02Auto, haier.getMode());
}

TEST(TestHaierACYRW02Class, Temperature) {
  IRHaierACYRW02 haier(0);
  haier.begin();

  haier.setTemp(kHaierAcMinTemp);
  EXPECT_EQ(kHaierAcMinTemp, haier.getTemp());

  haier.setButton(kHaierAcYrw02ButtonPower);
  haier.setTemp(kHaierAcMinTemp + 1);
  EXPECT_EQ(kHaierAcMinTemp + 1, haier.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, haier.getButton());

  haier.setTemp(kHaierAcMaxTemp);
  EXPECT_EQ(kHaierAcMaxTemp, haier.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, haier.getButton());

  haier.setTemp(kHaierAcMinTemp - 1);
  EXPECT_EQ(kHaierAcMinTemp, haier.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempDown, haier.getButton());

  haier.setTemp(kHaierAcMaxTemp + 1);
  EXPECT_EQ(kHaierAcMaxTemp, haier.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, haier.getButton());

  haier.setTemp(23);
  EXPECT_EQ(23, haier.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempDown, haier.getButton());
  haier.setButton(kHaierAcYrw02ButtonPower);
  haier.setTemp(23);
  EXPECT_EQ(23, haier.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonPower, haier.getButton());

  haier.setTemp(0);
  EXPECT_EQ(kHaierAcMinTemp, haier.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempDown, haier.getButton());

  haier.setTemp(255);
  EXPECT_EQ(kHaierAcMaxTemp, haier.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, haier.getButton());
}

TEST(TestHaierACYRW02Class, HealthMode) {
  IRHaierACYRW02 haier(0);
  haier.begin();

  haier.setHealth(true);
  EXPECT_TRUE(haier.getHealth());
  EXPECT_EQ(kHaierAcYrw02ButtonHealth, haier.getButton());

  haier.setButton(kHaierAcYrw02ButtonTempUp);
  haier.setHealth(false);
  EXPECT_FALSE(haier.getHealth());
  EXPECT_EQ(kHaierAcYrw02ButtonHealth, haier.getButton());

  haier.setHealth(true);
  EXPECT_TRUE(haier.getHealth());
  EXPECT_EQ(kHaierAcYrw02ButtonHealth, haier.getButton());
}

TEST(TestHaierACYRW02Class, Power) {
  IRHaierACYRW02 haier(0);
  haier.begin();

  haier.setPower(true);
  EXPECT_TRUE(haier.getPower());
  EXPECT_EQ(kHaierAcYrw02ButtonPower, haier.getButton());

  haier.setButton(kHaierAcYrw02ButtonTempUp);
  haier.setPower(false);
  EXPECT_FALSE(haier.getPower());
  EXPECT_EQ(kHaierAcYrw02ButtonPower, haier.getButton());

  haier.setPower(true);
  EXPECT_TRUE(haier.getPower());
  EXPECT_EQ(kHaierAcYrw02ButtonPower, haier.getButton());

  haier.off();
  EXPECT_FALSE(haier.getPower());
  haier.on();
  EXPECT_TRUE(haier.getPower());
}

TEST(TestHaierACYRW02Class, SleepMode) {
  IRHaierACYRW02 haier(0);
  haier.begin();

  haier.setSleep(true);
  EXPECT_TRUE(haier.getSleep());
  EXPECT_EQ(kHaierAcYrw02ButtonSleep, haier.getButton());

  haier.setButton(kHaierAcYrw02ButtonTempUp);
  haier.setSleep(false);
  EXPECT_FALSE(haier.getSleep());
  EXPECT_EQ(kHaierAcYrw02ButtonSleep, haier.getButton());

  haier.setSleep(true);
  EXPECT_TRUE(haier.getSleep());
  EXPECT_EQ(kHaierAcYrw02ButtonSleep, haier.getButton());
}

TEST(TestHaierACYRW02Class, TurboMode) {
  IRHaierACYRW02 haier(0);
  haier.begin();

  haier.setTurbo(kHaierAcYrw02TurboOff);
  EXPECT_EQ(kHaierAcYrw02TurboOff, haier.getTurbo());
  EXPECT_EQ(kHaierAcYrw02ButtonTurbo, haier.getButton());

  haier.setButton(kHaierAcYrw02ButtonTempUp);

  haier.setTurbo(kHaierAcYrw02TurboLow);
  EXPECT_EQ(kHaierAcYrw02TurboLow, haier.getTurbo());
  EXPECT_EQ(kHaierAcYrw02ButtonTurbo, haier.getButton());

  haier.setTurbo(kHaierAcYrw02TurboHigh);
  EXPECT_EQ(kHaierAcYrw02TurboHigh, haier.getTurbo());
  EXPECT_EQ(kHaierAcYrw02ButtonTurbo, haier.getButton());

  haier.setTurbo(kHaierAcYrw02TurboOff);
  EXPECT_EQ(kHaierAcYrw02TurboOff, haier.getTurbo());
  EXPECT_EQ(kHaierAcYrw02ButtonTurbo, haier.getButton());
}

TEST(TestHaierACYRW02Class, Fan) {
  IRHaierACYRW02 haier(0);
  haier.begin();

  haier.setFan(kHaierAcYrw02FanAuto);
  EXPECT_EQ(kHaierAcYrw02FanAuto, haier.getFan());
  EXPECT_EQ(kHaierAcYrw02ButtonFan, haier.getButton());

  haier.setButton(kHaierAcYrw02ButtonTempUp);

  haier.setFan(kHaierAcYrw02FanLow);
  EXPECT_EQ(kHaierAcYrw02FanLow, haier.getFan());
  EXPECT_EQ(kHaierAcYrw02ButtonFan, haier.getButton());

  haier.setFan(kHaierAcYrw02FanHigh);
  EXPECT_EQ(kHaierAcYrw02FanHigh, haier.getFan());
  EXPECT_EQ(kHaierAcYrw02ButtonFan, haier.getButton());

  haier.setFan(kHaierAcYrw02FanMed);
  EXPECT_EQ(kHaierAcYrw02FanMed, haier.getFan());
  EXPECT_EQ(kHaierAcYrw02ButtonFan, haier.getButton());

  // Test unexpected values.
  haier.setButton(kHaierAcYrw02ButtonTempUp);
  haier.setFan(0x00);
  EXPECT_EQ(kHaierAcYrw02FanMed, haier.getFan());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, haier.getButton());
}

TEST(TestHaierACYRW02Class, Swing) {
  IRHaierACYRW02 haier(0);
  haier.begin();

  haier.setSwing(kHaierAcYrw02SwingOff);
  EXPECT_EQ(kHaierAcYrw02SwingOff, haier.getSwing());
  EXPECT_EQ(kHaierAcYrw02ButtonSwing, haier.getButton());

  haier.setButton(kHaierAcYrw02ButtonTempUp);

  haier.setSwing(kHaierAcYrw02SwingAuto);
  EXPECT_EQ(kHaierAcYrw02SwingAuto, haier.getSwing());
  EXPECT_EQ(kHaierAcYrw02ButtonSwing, haier.getButton());

  haier.setSwing(kHaierAcYrw02SwingTop);
  EXPECT_EQ(kHaierAcYrw02SwingTop, haier.getSwing());
  EXPECT_EQ(kHaierAcYrw02ButtonSwing, haier.getButton());

  haier.setSwing(kHaierAcYrw02SwingDown);
  EXPECT_EQ(kHaierAcYrw02SwingDown, haier.getSwing());
  EXPECT_EQ(kHaierAcYrw02ButtonSwing, haier.getButton());

  // Test unexpected values.
  haier.setButton(kHaierAcYrw02ButtonTempUp);
  haier.setSwing(0xFF);
  EXPECT_EQ(kHaierAcYrw02SwingDown, haier.getSwing());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, haier.getButton());

  // Test the mode-dependant positions.
  haier.setMode(kHaierAcYrw02Auto);
  haier.setSwing(kHaierAcYrw02SwingMiddle);
  EXPECT_EQ(kHaierAcYrw02SwingMiddle, haier.getSwing());
  EXPECT_EQ(kHaierAcYrw02ButtonSwing, haier.getButton());
  haier.setMode(kHaierAcYrw02Heat);
  haier.setSwing(kHaierAcYrw02SwingMiddle);
  EXPECT_EQ(kHaierAcYrw02SwingBottom, haier.getSwing());
  haier.setSwing(kHaierAcYrw02SwingAuto);
  EXPECT_EQ(kHaierAcYrw02SwingAuto, haier.getSwing());
  haier.setSwing(kHaierAcYrw02SwingBottom);
  EXPECT_EQ(kHaierAcYrw02SwingBottom, haier.getSwing());
  haier.setMode(kHaierAcYrw02Cool);
  haier.setSwing(kHaierAcYrw02SwingBottom);
  EXPECT_EQ(kHaierAcYrw02SwingMiddle, haier.getSwing());
}

TEST(TestHaierACYRW02Class, MessageConstuction) {
  IRHaierACYRW02 haier(0);

  EXPECT_EQ(
      "Power: On, Button: 5 (Power), Mode: 0 (Auto), Temp: 25C,"
      " Fan: 10 (Auto), Turbo: 0 (Off), Swing: 0 (Off), Sleep: Off,"
      " Health: On",
      haier.toString());
  haier.setMode(kHaierAcYrw02Cool);
  haier.setTemp(21);
  haier.setFan(kHaierAcYrw02FanHigh);
  EXPECT_EQ(
      "Power: On, Button: 4 (Fan), Mode: 2 (Cool), Temp: 21C,"
      " Fan: 2 (High), Turbo: 0 (Off), Swing: 0 (Off), Sleep: Off,"
      " Health: On",
      haier.toString());

  haier.setSwing(kHaierAcYrw02SwingMiddle);
  haier.setHealth(false);
  haier.setSleep(true);
  haier.setTurbo(kHaierAcYrw02TurboHigh);
  EXPECT_EQ(
      "Power: On, Button: 8 (Turbo), Mode: 2 (Cool), Temp: 21C,"
      " Fan: 2 (High), Turbo: 1 (High), Swing: 2 (Middle),"
      " Sleep: On, Health: Off",
      haier.toString());
}

// Decode "real" state messages.
TEST(TestHaierACYRW02Class, RealStates) {
  uint8_t expectedState1[kHaierACYRW02StateLength] = {
      0xA6, 0xE1, 0x00, 0x00, 0x40, 0x20, 0x00,
      0x80, 0x00, 0x00, 0x00, 0x00, 0x07, 0x6E};

  IRHaierACYRW02 haier(0);
  haier.setRaw(expectedState1);
  EXPECT_EQ(
      "Power: On, Button: 7 (Health), Mode: 8 (Heat), Temp: 30C,"
      " Fan: 2 (High), Turbo: 0 (Off), Swing: 1 (Top), Sleep: Off,"
      " Health: Off",
      haier.toString());

  uint8_t expectedState2[kHaierACYRW02StateLength] = {
      0xA6, 0xE0, 0x00, 0x00, 0x00, 0x20, 0x00,
      0x80, 0x00, 0x00, 0x00, 0x00, 0x05, 0x75};
  haier.setRaw(expectedState2);
  EXPECT_EQ(
      "Power: Off, Button: 5 (Power), Mode: 8 (Heat), Temp: 30C,"
      " Fan: 2 (High), Turbo: 0 (Off), Swing: 0 (Off), Sleep: Off,"
      " Health: Off",
      haier.toString());

  uint8_t expectedState3[kHaierACYRW02StateLength] = {
      0xA6, 0x02, 0x00, 0x02, 0x40, 0x20, 0x00,
      0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x2B};
  haier.setRaw(expectedState3);
  EXPECT_EQ(
      "Power: On, Button: 1 (Temp Down), Mode: 2 (Cool), Temp: 16C,"
      " Fan: 2 (High), Turbo: 0 (Off), Swing: 2 (Middle), Sleep: Off,"
      " Health: On",
      haier.toString());

  // cool 25, health, fan auto, swing auto,  sleep on
  uint8_t expectedState4[kHaierACYRW02StateLength] = {
      0xA6, 0x9C, 0x00, 0x02, 0x40, 0xA8, 0x00,
      0x20, 0x80, 0x00, 0x00, 0x00, 0x0B, 0xD7};
  haier.setRaw(expectedState4);
  EXPECT_EQ(
      "Power: On, Button: 11 (Sleep), Mode: 2 (Cool), Temp: 25C,"
      " Fan: 10 (Auto), Turbo: 0 (Off), Swing: 12 (Auto), Sleep: On,"
      " Health: On",
      haier.toString());

  // cool 25, health, fan 3, swing auto,  sleep on
  uint8_t expectedState5[kHaierACYRW02StateLength] = {
      0xA6, 0x9C, 0x00, 0x02, 0x40, 0x27, 0x36,
      0x20, 0x80, 0x00, 0x00, 0x00, 0x04, 0x85};
  haier.setRaw(expectedState5);
  EXPECT_EQ(
      "Power: On, Button: 4 (Fan), Mode: 2 (Cool), Temp: 25C,"
      " Fan: 2 (High), Turbo: 0 (Off), Swing: 12 (Auto), Sleep: On,"
      " Health: On",
      haier.toString());
}

// Tests for decodeHaierAC().

// Decode normal "synthetic" messages.
TEST(TestDecodeHaierAC, NormalDecodeWithStrict) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  uint8_t expectedState[kHaierACStateLength] = {0xA5, 0x01, 0x20, 0x01, 0x00,
                                                0xC0, 0x20, 0x00, 0xA7};

  // With the specific decoder.
  irsend.reset();
  irsend.sendHaierAC(expectedState);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeHaierAC(&irsend.capture, kHaierACBits, true));
  EXPECT_EQ(HAIER_AC, irsend.capture.decode_type);
  EXPECT_EQ(kHaierACBits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);

  // With the all the decoders.
  irsend.reset();
  irsend.sendHaierAC(expectedState);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(HAIER_AC, irsend.capture.decode_type);
  EXPECT_EQ(kHaierACBits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
}

// Decode a "real" example message.
TEST(TestDecodeHaierAC, RealExample1) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  // Data from Issue #404 captured by kuzin2006
  uint16_t rawData[149] = {
      3030, 3044, 3030, 4304, 576,  1694, 550,  582,  552, 1704, 552, 714, 550,
      582,  550,  1706, 552,  582,  550,  1836, 552,  582, 578,  568, 550, 582,
      550,  714,  550,  582,  550,  582,  552,  582,  550, 1836, 552, 582, 552,
      580,  580,  1692, 550,  712,  552,  582,  550,  582, 552,  580, 550, 714,
      552,  582,  550,  582,  552,  582,  578,  698,  552, 580,  552, 582, 552,
      582,  552,  1836, 552,  580,  552,  582,  552,  582, 550,  714, 578, 568,
      550,  582,  550,  582,  552,  714,  550,  1706, 550, 1706, 550, 582, 550,
      714,  552,  582,  580,  566,  552,  582,  550,  714, 552,  580, 552, 580,
      552,  1706, 550,  714,  550,  582,  552,  582,  578, 568,  552, 712, 552,
      582,  550,  582,  550,  582,  550,  712,  552,  582, 550,  582, 552, 582,
      578,  722,  552,  1704, 550,  582,  550,  1706, 550, 736,  550, 582, 550,
      1706, 550,  1704, 552,  1704, 578};
  uint8_t expectedState[kHaierACStateLength] = {0xA5, 0x01, 0x20, 0x01, 0x00,
                                                0xC0, 0x20, 0x00, 0xA7};

  irsend.sendRaw(rawData, 149, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(HAIER_AC, irsend.capture.decode_type);
  EXPECT_EQ(kHaierACBits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);

  IRHaierAC haier(0);
  haier.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Command: 1 (On), Mode: 0 (AUTO), Temp: 16C, Fan: 0 (AUTO), "
      "Swing: 0 (Off), Sleep: Off, Health: Off, "
      "Current Time: 00:01, On Timer: Off, Off Timer: Off",
      haier.toString());
}

// Decode a "real" example message.
TEST(TestDecodeHaierAC, RealExample2) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  // Data from Issue #404 captured by kuzin2006
  uint16_t rawData[149] = {
      3028, 3046, 3028, 4304, 576, 1694, 552, 582,  550, 1704, 552, 714,
      550,  582,  552,  1704, 550, 582,  550, 1836, 552, 582,  578, 1690,
      552,  1704, 552,  712,  550, 582,  550, 1706, 550, 1706, 552, 712,
      550,  582,  552,  582,  578, 1690, 552, 714,  552, 580,  552, 582,
      552,  582,  550,  712,  552, 582,  550, 582,  550, 582,  578, 698,
      552,  582,  550,  584,  550, 582,  552, 1836, 550, 582,  550, 582,
      550,  582,  550,  712,  578, 568,  550, 582,  550, 582,  550, 714,
      552,  1706, 550,  1706, 552, 580,  550, 714,  550, 582,  580, 568,
      550,  582,  550,  714,  550, 582,  550, 582,  550, 1706, 552, 712,
      550,  582,  550,  582,  580, 568,  552, 712,  550, 584,  550, 582,
      550,  584,  550,  712,  550, 582,  550, 582,  550, 582,  578, 722,
      550,  582,  552,  580,  552, 582,  550, 738,  550, 1706, 550, 1704,
      552,  582,  550,  582,  578};
  uint8_t expectedState[kHaierACStateLength] = {0xA5, 0x66, 0x20, 0x01, 0x00,
                                                0xC0, 0x20, 0x00, 0x0C};

  irsend.sendRaw(rawData, 149, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(HAIER_AC, irsend.capture.decode_type);
  EXPECT_EQ(kHaierACBits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);

  IRHaierAC haier(0);
  haier.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Command: 6 (Temp Up), Mode: 0 (AUTO), Temp: 22C, Fan: 0 (AUTO), "
      "Swing: 0 (Off), Sleep: Off, Health: Off, "
      "Current Time: 00:01, On Timer: Off, Off Timer: Off",
      haier.toString());
}

// Decode a "real" example message.
TEST(TestDecodeHaierAC, RealExample3) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  // Data from Issue #404 captured by kuzin2006
  uint16_t rawData[149] = {
      3030, 3044, 3030, 4302, 578, 1692, 550, 582,  550, 1706, 550, 714,
      550,  582,  552,  1706, 550, 582,  550, 1836, 552, 1706, 578, 1690,
      552,  1704, 552,  714,  550, 1706, 552, 1706, 550, 582,  550, 714,
      552,  582,  550,  582,  578, 1690, 550, 714,  552, 582,  552, 582,
      550,  582,  550,  714,  550, 584,  550, 582,  550, 582,  578, 700,
      552,  1706, 550,  582,  550, 582,  552, 1836, 550, 582,  550, 582,
      552,  1706, 550,  714,  578, 568,  552, 582,  552, 582,  550, 714,
      550,  1706, 550,  1706, 550, 582,  552, 712,  552, 582,  580, 568,
      550,  582,  550,  714,  550, 582,  550, 582,  550, 1706, 550, 714,
      550,  582,  550,  582,  578, 568,  552, 712,  552, 582,  550, 582,
      550,  582,  550,  712,  550, 584,  550, 582,  552, 582,  578, 722,
      552,  1704, 550,  582,  550, 1706, 550, 1862, 550, 1706, 550, 582,
      550,  1704, 552,  582,  578};
  uint8_t expectedState[kHaierACStateLength] = {0xA5, 0xEC, 0x20, 0x09, 0x20,
                                                0xC0, 0x20, 0x00, 0xBA};

  irsend.sendRaw(rawData, 149, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(HAIER_AC, irsend.capture.decode_type);
  EXPECT_EQ(kHaierACBits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);

  IRHaierAC haier(0);
  haier.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Command: 12 (Health), Mode: 0 (AUTO), Temp: 30C, Fan: 0 (AUTO), "
      "Swing: 0 (Off), Sleep: Off, Health: On, "
      "Current Time: 00:09, On Timer: Off, Off Timer: Off",
      haier.toString());
}

// Decode normal "synthetic" messages.
TEST(TestDecodeHaierAC_YRW02, NormalDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  uint8_t expectedState[kHaierACYRW02StateLength] = {
      0xA6, 0x12, 0x00, 0x02, 0x40, 0x20, 0x00,
      0x20, 0x00, 0x00, 0x00, 0x00, 0x05, 0x3F};

  irsend.reset();
  irsend.sendHaierACYRW02(expectedState);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(HAIER_AC_YRW02, irsend.capture.decode_type);
  EXPECT_EQ(kHaierACYRW02Bits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
}

// Decode a "real" example message.
TEST(TestDecodeHaierAC_YRW02, RealExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  // Data from Issue #485 captured by non7top
  uint16_t rawData[229] = {
      2998, 3086, 2998, 4460, 568, 1640, 596, 492,  514, 1690, 590, 496,
      566,  532,  592,  1596, 570, 1618, 518, 584,  590, 538,  524, 536,
      568,  532,  590,  1596, 516, 612,  568, 538,  522, 1638, 586, 500,
      512,  614,  568,  538,  520, 538,  586, 538,  566, 540,  520, 538,
      586,  538,  522,  538,  588, 538,  568, 538,  520, 538,  586, 538,
      566,  538,  520,  540,  588, 1596, 590, 536,  568, 538,  520, 1592,
      640,  538,  520,  540,  588, 538,  568, 538,  516, 562,  566, 538,
      518,  542,  586,  540,  566, 1596, 590, 538,  566, 538,  516, 544,
      586,  538,  516,  542,  588, 540,  564, 540,  468, 590,  588, 538,
      566,  540,  466,  590,  588, 538,  514, 544,  588, 538,  566, 538,
      468,  1692, 606,  526,  466, 592,  588, 538,  568, 490,  588, 538,
      566,  540,  466,  592,  588, 538,  566, 538,  466, 592,  588, 538,
      568,  492,  586,  540,  566, 540,  468, 590,  588, 538,  568, 516,
      488,  590,  588,  538,  568, 492,  588, 538,  566, 518,  488, 590,
      588,  540,  564,  518,  490, 590,  588, 538,  562, 496,  588, 538,
      566,  518,  488,  590,  588, 538,  562, 522,  488, 588,  590, 538,
      560,  498,  588,  540,  564, 522,  486, 590,  590, 538,  560, 524,
      488,  588,  588,  1598, 514, 608,  564, 1600, 548, 536,  586, 538,
      568,  1594, 590,  1618, 578, 1606, 606, 1582, 590, 1596, 590, 1616,
      580};
  uint8_t expectedState[kHaierACYRW02StateLength] = {
      0xA6, 0x12, 0x00, 0x02, 0x40, 0x20, 0x00,
      0x20, 0x00, 0x00, 0x00, 0x00, 0x05, 0x3F};

  irsend.sendRaw(rawData, 229, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(HAIER_AC_YRW02, irsend.capture.decode_type);
  EXPECT_EQ(kHaierACYRW02Bits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);

  IRHaierACYRW02 haier(0);
  haier.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: On, Button: 5 (Power), Mode: 2 (Cool), Temp: 17C,"
      " Fan: 2 (High), Turbo: 0 (Off), Swing: 2 (Middle), Sleep: Off,"
      " Health: On",
      haier.toString());
}
