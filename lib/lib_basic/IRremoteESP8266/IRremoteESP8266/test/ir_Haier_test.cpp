// Copyright 2018 David Conran

#include "ir_Haier.h"
#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendHaierAC()

// Test sending typical data only.
TEST(TestSendHaierAC, SendDataOnly) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();
  uint8_t haier_zero[kHaierACStateLength] = {0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x00};

  irsend.reset();
  irsend.sendHaierAC(haier_zero);
  EXPECT_EQ(
      "f38000d50"
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
      "f38000d50"
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
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  irsend.reset();
  uint8_t haier_test[kHaierACStateLength] = {0xA5, 0x01, 0x20, 0x01, 0x00,
                                             0xC0, 0x20, 0x00, 0xA7};
  irsend.reset();
  irsend.sendHaierAC(haier_test, kHaierACStateLength, 2);  // two repeats.
  EXPECT_EQ(
      "f38000d50"
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
  IRHaierAC haier(kGpioUnused);
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
  haier.setCommand(kHaierAcCmdOff);
  EXPECT_EQ(kHaierAcCmdOff, haier.getCommand());

  // Test unexpected values.
  haier.setCommand(0b00001110);
  EXPECT_EQ(kHaierAcCmdOff, haier.getCommand());
  haier.setCommand(0b00001111);
  EXPECT_EQ(kHaierAcCmdOff, haier.getCommand());
  haier.setCommand(0b00000100);
  EXPECT_EQ(kHaierAcCmdOff, haier.getCommand());
}

TEST(TestHaierACClass, OperatingMode) {
  IRHaierAC haier(kGpioUnused);
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
  IRHaierAC haier(kGpioUnused);
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
  IRHaierAC haier(kGpioUnused);
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
  IRHaierAC haier(kGpioUnused);
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
  IRHaierAC haier(kGpioUnused);
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
  IRHaierAC haier(kGpioUnused);
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

TEST(TestHaierACClass, MessageConstuction) {
  IRHaierAC haier(kGpioUnused);

  EXPECT_EQ(
      "Command: 1 (On), Mode: 0 (Auto), Temp: 25C, Fan: 1 (Low), "
      "Swing: 0 (Off), Sleep: Off, Health: Off, "
      "Clock: 00:00, On Timer: Off, Off Timer: Off",
      haier.toString());
  haier.setMode(kHaierAcCool);
  haier.setTemp(21);
  haier.setFan(kHaierAcFanHigh);
  EXPECT_EQ(
      "Command: 3 (Fan), Mode: 1 (Cool), Temp: 21C, Fan: 3 (High), "
      "Swing: 0 (Off), Sleep: Off, Health: Off, "
      "Clock: 00:00, On Timer: Off, Off Timer: Off",
      haier.toString());
  haier.setSwing(kHaierAcSwingChg);
  haier.setHealth(true);
  haier.setSleep(true);
  haier.setCurrTime(615);  // 10:15am
  EXPECT_EQ(
      "Command: 8 (Sleep), Mode: 1 (Cool), Temp: 21C, Fan: 3 (High), "
      "Swing: 3 (Change), Sleep: On, Health: On, "
      "Clock: 10:15, On Timer: Off, Off Timer: Off",
      haier.toString());
  haier.setOnTimer(800);    // 1:20pm
  haier.setOffTimer(1125);  // 6:45pm
  haier.setCommand(kHaierAcCmdOn);

  EXPECT_EQ(
      "Command: 1 (On), Mode: 1 (Cool), Temp: 21C, Fan: 3 (High), "
      "Swing: 3 (Change), Sleep: On, Health: On, "
      "Clock: 10:15, On Timer: 13:20, Off Timer: 18:45",
      haier.toString());

  // Now change a few already set things.
  haier.setMode(kHaierAcHeat);
  EXPECT_EQ(
      "Command: 2 (Mode), Mode: 3 (Heat), Temp: 21C, Fan: 3 (High), "
      "Swing: 3 (Change), Sleep: On, Health: On, "
      "Clock: 10:15, On Timer: 13:20, Off Timer: 18:45",
      haier.toString());

  haier.setTemp(25);
  EXPECT_EQ(
      "Command: 6 (Temp Up), Mode: 3 (Heat), Temp: 25C, Fan: 3 (High), "
      "Swing: 3 (Change), Sleep: On, Health: On, "
      "Clock: 10:15, On Timer: 13:20, Off Timer: 18:45",
      haier.toString());

  uint8_t expectedState[kHaierACStateLength] = {0xA5, 0x96, 0xEA, 0xCF, 0x32,
                                                0x6D, 0x6D, 0x54, 0x54};
  EXPECT_STATE_EQ(expectedState, haier.getRaw(), kHaierACBits);

  // Check that the checksum is valid.
  EXPECT_TRUE(IRHaierAC::validChecksum(haier.getRaw()));

  // Now load up some random data.
  uint8_t randomState[kHaierACStateLength] = {0x52, 0x49, 0x50, 0x20, 0x54,
                                              0x61, 0x6C, 0x69, 0x61};
  EXPECT_FALSE(IRHaierAC::validChecksum(randomState));
  haier.setRaw(randomState);
  EXPECT_EQ(
      "Command: 9 (Timer Set), Mode: 3 (Heat), Temp: 20C, Fan: 3 (High), "
      "Swing: 1 (Up), Sleep: On, Health: Off, "
      "Clock: 16:32, On Timer: Off, Off Timer: Off",
      haier.toString());
  // getRaw() should correct the checksum.
  EXPECT_TRUE(IRHaierAC::validChecksum(haier.getRaw()));
}

// Tests for the IRHaierACYRW02 class.

TEST(TestHaierACYRW02Class, Button) {
  IRHaierACYRW02 haier(kGpioUnused);
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
  IRHaierACYRW02 haier(kGpioUnused);
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
  IRHaierACYRW02 haier(kGpioUnused);
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
  IRHaierACYRW02 haier(kGpioUnused);
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
  IRHaierACYRW02 haier(kGpioUnused);
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
  IRHaierACYRW02 haier(kGpioUnused);
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
  IRHaierACYRW02 haier(kGpioUnused);
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
  IRHaierACYRW02 haier(kGpioUnused);
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
  IRHaierACYRW02 haier(kGpioUnused);
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
  IRHaierACYRW02 haier(kGpioUnused);

  EXPECT_EQ(
      "Power: On, Button: 5 (Power), Mode: 0 (Auto), Temp: 25C,"
      " Fan: 5 (Auto), Turbo: 0 (Off), Swing: 0 (Off), Sleep: Off,"
      " Health: On, Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off",
      haier.toString());
  haier.setMode(kHaierAcYrw02Cool);
  haier.setTemp(21);
  haier.setFan(kHaierAcYrw02FanHigh);
  EXPECT_EQ(
      "Power: On, Button: 4 (Fan), Mode: 1 (Cool), Temp: 21C,"
      " Fan: 1 (High), Turbo: 0 (Off), Swing: 0 (Off), Sleep: Off,"
      " Health: On, Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off",
      haier.toString());

  haier.setSwing(kHaierAcYrw02SwingMiddle);
  haier.setHealth(false);
  haier.setSleep(true);
  haier.setTurbo(kHaierAcYrw02TurboHigh);
  EXPECT_EQ(
      "Power: On, Button: 8 (Turbo), Mode: 1 (Cool), Temp: 21C, "
      "Fan: 1 (High), Turbo: 1 (High), Swing: 2 (Middle), "
      "Sleep: On, Health: Off, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off",
      haier.toString());
}

// Decode "real" state messages.
TEST(TestHaierACYRW02Class, RealStates) {
  uint8_t expectedState1[kHaierACYRW02StateLength] = {
      0xA6, 0xE1, 0x00, 0x00, 0x40, 0x20, 0x00,
      0x80, 0x00, 0x00, 0x00, 0x00, 0x07, 0x6E};

  IRHaierACYRW02 haier(kGpioUnused);
  haier.setRaw(expectedState1);
  EXPECT_EQ(
      "Power: On, Button: 7 (Health), Mode: 4 (Heat), Temp: 30C, "
      "Fan: 1 (High), Turbo: 0 (Off), Swing: 1 (Highest), Sleep: Off, "
      "Health: Off, Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off",
      haier.toString());

  uint8_t expectedState2[kHaierACYRW02StateLength] = {
      0xA6, 0xE0, 0x00, 0x00, 0x00, 0x20, 0x00,
      0x80, 0x00, 0x00, 0x00, 0x00, 0x05, 0x75};
  haier.setRaw(expectedState2);
  EXPECT_EQ(
      "Power: Off, Button: 5 (Power), Mode: 4 (Heat), Temp: 30C, "
      "Fan: 1 (High), Turbo: 0 (Off), Swing: 0 (Off), Sleep: Off, "
      "Health: Off, Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off",
      haier.toString());

  uint8_t expectedState3[kHaierACYRW02StateLength] = {
      0xA6, 0x02, 0x00, 0x02, 0x40, 0x20, 0x00,
      0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x2B};
  haier.setRaw(expectedState3);
  EXPECT_EQ(
      "Power: On, Button: 1 (Temp Down), Mode: 1 (Cool), Temp: 16C, "
      "Fan: 1 (High), Turbo: 0 (Off), Swing: 2 (Middle), Sleep: Off, "
      "Health: On, Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off",
      haier.toString());

  // cool 25, health, fan auto, swing auto,  sleep on
  uint8_t expectedState4[kHaierACYRW02StateLength] = {
      0xA6, 0x9C, 0x00, 0x02, 0x40, 0xA8, 0x00,
      0x20, 0x80, 0x00, 0x00, 0x00, 0x0B, 0xD7};
  haier.setRaw(expectedState4);
  EXPECT_EQ(
      "Power: On, Button: 11 (Sleep), Mode: 1 (Cool), Temp: 25C, "
      "Fan: 5 (Auto), Turbo: 0 (Off), Swing: 12 (Auto), Sleep: On, "
      "Health: On, Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off",
      haier.toString());

  // cool 25, health, fan 3, swing auto,  sleep on
  uint8_t expectedState5[kHaierACYRW02StateLength] = {
      0xA6, 0x9C, 0x00, 0x02, 0x40, 0x27, 0x36,
      0x20, 0x80, 0x00, 0x00, 0x00, 0x04, 0x85};
  haier.setRaw(expectedState5);
  EXPECT_EQ(
      "Power: On, Button: 4 (Fan), Mode: 1 (Cool), Temp: 25C, "
      "Fan: 1 (High), Turbo: 0 (Off), Swing: 12 (Auto), Sleep: On, "
      "Health: On, Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off",
      haier.toString());
}

// Tests for decodeHaierAC().

// Decode normal "synthetic" messages.
TEST(TestDecodeHaierAC, NormalDecodeWithStrict) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  uint8_t expectedState[kHaierACStateLength] = {0xA5, 0x01, 0x20, 0x01, 0x00,
                                                0xC0, 0x20, 0x00, 0xA7};

  // With the specific decoder.
  irsend.reset();
  irsend.sendHaierAC(expectedState);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeHaierAC(&irsend.capture, kStartOffset, kHaierACBits,
                                   true));
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
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  // Data from Issue #404 captured by kuzin2006,
  // ON COOL 16C FAN LOW ALL OFF 12:00AM
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

  EXPECT_EQ(
      "Command: 1 (On), Mode: 1 (Cool), Temp: 16C, Fan: 1 (Low), "
      "Swing: 0 (Off), Sleep: Off, Health: Off, "
      "Clock: 00:01, On Timer: Off, Off Timer: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

// Decode a "real" example message.
TEST(TestDecodeHaierAC, RealExample2) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  // Data from Issue #404 captured by kuzin2006
  // TEMP 21C
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

  IRHaierAC haier(kGpioUnused);
  haier.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Command: 6 (Temp Up), Mode: 1 (Cool), Temp: 22C, Fan: 1 (Low), "
      "Swing: 0 (Off), Sleep: Off, Health: Off, "
      "Clock: 00:01, On Timer: Off, Off Timer: Off",
      haier.toString());
}

// Decode a "real" example message.
TEST(TestDecodeHaierAC, RealExample3) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  // Data from Issue #404 captured by kuzin2006
  // HEALTH ON
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

  IRHaierAC haier(kGpioUnused);
  haier.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Command: 12 (Health), Mode: 1 (Cool), Temp: 30C, Fan: 1 (Low), "
      "Swing: 0 (Off), Sleep: Off, Health: On, "
      "Clock: 00:09, On Timer: Off, Off Timer: Off",
      haier.toString());
}

// Decode normal "synthetic" messages.
TEST(TestDecodeHaierAC_YRW02, NormalDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
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
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
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

  IRHaierACYRW02 haier(kGpioUnused);
  haier.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: On, Button: 5 (Power), Mode: 1 (Cool), Temp: 17C, "
      "Fan: 1 (High), Turbo: 0 (Off), Swing: 2 (Middle), Sleep: Off, "
      "Health: On, Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off",
      haier.toString());
}

// Default state of the remote needed to include hidden data.
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/668
TEST(TestHaierAcIssues, Issue668) {
  IRHaierAC ac(kGpioUnused);
  IRHaierAC acText(1);
  IRrecv irrecv(kGpioUnused);
  ac.begin();

  // Turn on the AC.
  ac._irsend.reset();
  char expected_on[] =
      "Command: 1 (On), Mode: 1 (Cool), Temp: 25C, Fan: 1 (Low), "
      "Swing: 0 (Off), Sleep: Off, Health: Off, Clock: 00:00, "
      "On Timer: Off, Off Timer: Off";
  // State taken from real capture:
  //   https://github.com/crankyoldgit/IRremoteESP8266/issues/668#issuecomment-483531895
  uint8_t expected_on_state[9] = {
      0xA5, 0x91, 0x20, 0x00, 0x0C, 0xC0, 0x20, 0x00, 0x42};
  ac.setMode(kHaierAcCool);
  ac.setCommand(kHaierAcCmdOn);
  EXPECT_EQ(expected_on, ac.toString());
  ac.send();
  ac._irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&ac._irsend.capture));
  ASSERT_EQ(HAIER_AC, ac._irsend.capture.decode_type);
  EXPECT_EQ(kHaierACBits, ac._irsend.capture.bits);
  EXPECT_STATE_EQ(expected_on_state,
                  ac._irsend.capture.state, ac._irsend.capture.bits);
  acText.setRaw(ac._irsend.capture.state);
  EXPECT_EQ(expected_on, acText.toString());

  // Set the temp to 25C
  ac._irsend.reset();
  ac.setTemp(25);
  EXPECT_EQ(expected_on, ac.toString());
  ASSERT_EQ(25, ac.getTemp());
  ac.send();
  ac._irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&ac._irsend.capture));
  ASSERT_EQ(HAIER_AC, ac._irsend.capture.decode_type);
  EXPECT_EQ(kHaierACBits, ac._irsend.capture.bits);
  EXPECT_STATE_EQ(expected_on_state,
                  ac._irsend.capture.state, ac._irsend.capture.bits);
  acText.setRaw(ac._irsend.capture.state);
  EXPECT_EQ(expected_on, acText.toString());

  // Increase the temp by 1.
  ac._irsend.reset();
  char expected_temp_plus_one[] =
      "Command: 6 (Temp Up), Mode: 1 (Cool), Temp: 26C, Fan: 1 (Low), "
      "Swing: 0 (Off), Sleep: Off, Health: Off, Clock: 00:00, "
      "On Timer: Off, Off Timer: Off";
  // State taken from real capture:
  //   https://github.com/crankyoldgit/IRremoteESP8266/issues/668#issuecomment-483531895
  uint8_t expected_temp_plus_one_state[9] = {
      0xA5, 0xA6, 0x20, 0x00, 0x0C, 0xC0, 0x20, 0x00, 0x57};
  ASSERT_EQ(25, ac.getTemp());
  ac.setTemp(ac.getTemp() + 1);
  ASSERT_EQ(26, ac.getTemp());
  EXPECT_EQ(expected_temp_plus_one, ac.toString());
  ac.send();
  ac._irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&ac._irsend.capture));
  ASSERT_EQ(HAIER_AC, ac._irsend.capture.decode_type);
  EXPECT_EQ(kHaierACBits, ac._irsend.capture.bits);
  EXPECT_STATE_EQ(expected_temp_plus_one_state,
                  ac._irsend.capture.state, ac._irsend.capture.bits);
  acText.setRaw(ac._irsend.capture.state);
  EXPECT_EQ(expected_temp_plus_one, acText.toString());

  // Decrease the temp by 1.
  ac._irsend.reset();
  char expected_temp_minus_one[] =
      "Command: 7 (Temp Down), Mode: 1 (Cool), Temp: 25C, Fan: 1 (Low), "
      "Swing: 0 (Off), Sleep: Off, Health: Off, Clock: 00:00, "
      "On Timer: Off, Off Timer: Off";
  ASSERT_EQ(26, ac.getTemp());
  ac.setTemp(ac.getTemp() - 1);
  ASSERT_EQ(25, ac.getTemp());
  EXPECT_EQ(expected_temp_minus_one, ac.toString());
  ac.send();
  ac._irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&ac._irsend.capture));
  ASSERT_EQ(HAIER_AC, ac._irsend.capture.decode_type);
  EXPECT_EQ(kHaierACBits, ac._irsend.capture.bits);
  acText.setRaw(ac._irsend.capture.state);
  EXPECT_EQ(expected_temp_minus_one, acText.toString());
}

TEST(TestHaierACClass, toCommon) {
  IRHaierAC ac(kGpioUnused);
  ac.setCommand(kHaierAcCmdOn);
  ac.setMode(kHaierAcCool);
  ac.setTemp(20);
  ac.setFan(kHaierAcFanHigh);
  ac.setSwing(kHaierAcSwingChg);
  ac.setHealth(true);
  ac.setSleep(true);
  // Now test it.
  ASSERT_EQ(decode_type_t::HAIER_AC, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_TRUE(ac.toCommon().filter);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kAuto, ac.toCommon().swingv);
  ASSERT_EQ(0, ac.toCommon().sleep);
  // Unsupported.
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

TEST(TestHaierACYRW02Class, toCommon) {
  IRHaierACYRW02 ac(kGpioUnused);
  ac.setPower(true);
  ac.setMode(kHaierAcYrw02Cool);
  ac.setTemp(20);
  ac.setFan(kHaierAcYrw02FanHigh);
  ac.setSwing(kHaierAcYrw02SwingTop);
  ac.setHealth(true);
  ac.setSleep(true);
  // Now test it.
  ASSERT_EQ(decode_type_t::HAIER_AC_YRW02, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_TRUE(ac.toCommon().filter);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kHighest, ac.toCommon().swingv);
  ASSERT_EQ(0, ac.toCommon().sleep);
  // Unsupported.
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

TEST(TestDecodeHaierAC176, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1480
  const uint16_t rawData[357] = {
      3096, 2948, 3048, 4388, 588, 1610, 614, 498, 586, 1612, 612, 500, 612,
      500, 586, 1610, 588, 1612, 612, 502, 586, 1612, 612, 500, 612, 500, 614,
      500, 612, 498, 586, 1610, 586, 1612, 612, 502, 612, 500, 612, 500, 612,
      500, 612, 500, 612, 500, 612, 500, 612, 500, 612, 504, 612, 500, 612, 500,
      612, 500, 612, 500, 612, 500, 612, 500, 612, 500, 612, 502, 614, 498, 586,
      1612, 612, 500, 612, 500, 612, 500, 612, 500, 612, 500, 612, 502, 586,
      1612, 612, 500, 586, 1610, 612, 500, 612, 498, 612, 500, 614, 478, 634,
      502, 612, 500, 612, 500, 612, 500, 612, 500, 612, 500, 612, 500, 612, 498,
      614, 504, 612, 500, 614, 500, 586, 1612, 612, 500, 612, 500, 612, 500,
      612, 500, 612, 502, 612, 500, 612, 500, 612, 500, 612, 500, 612, 500, 612,
      500, 612, 500, 612, 504, 614, 500, 612, 500, 612, 498, 614, 500, 612, 500,
      612, 500, 612, 500, 612, 482, 632, 500, 612, 502, 610, 500, 614, 500, 612,
      500, 612, 500, 612, 480, 632, 504, 612, 480, 632, 500, 612, 500, 612, 480,
      632, 500, 612, 500, 612, 500, 612, 502, 612, 500, 612, 500, 612, 500, 612,
      500, 612, 500, 586, 1612, 612, 500, 586, 1616, 612, 500, 612, 500, 586,
      1610, 588, 1612, 612, 502, 612, 500, 614, 498, 586, 1614, 586, 1612, 612,
      500, 586, 1610, 586, 1592, 632, 498, 586, 1610, 588, 1610, 586, 1614, 614,
      500, 612, 480, 632, 500, 612, 500, 612, 500, 612, 500, 614, 498, 612, 500,
      614, 500, 614, 500, 612, 500, 612, 500, 614, 498, 614, 498, 614, 500, 612,
      504, 612, 500, 612, 500, 612, 500, 612, 498, 612, 502, 612, 500, 614, 498,
      612, 502, 612, 500, 612, 498, 614, 500, 612, 500, 612, 500, 612, 500, 612,
      500, 614, 502, 612, 500, 614, 478, 634, 498, 614, 500, 612, 500, 612, 500,
      612, 500, 612, 482, 634, 500, 612, 500, 612, 500, 612, 500, 614, 498, 614,
      500, 612, 480, 632, 502, 586, 1610, 614, 478, 608, 1610, 588, 1610, 612,
      498, 586, 1610, 588, 1610, 586, 1606, 612};  // UNKNOWN ABFAD961

  irsend.sendRaw(rawData, 357, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(HAIER_AC176, irsend.capture.decode_type);
  EXPECT_EQ(kHaierAC176Bits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  const uint8_t expectedState[kHaierAC176StateLength] = {
      0xA6, 0x86, 0x00, 0x00, 0x40, 0xA0, 0x00,
      0x20, 0x00, 0x00, 0x00, 0x00, 0x05, 0x31,
      0xB7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB7};
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
}

// Decode normal "synthetic" messages.
TEST(TestDecodeHaierAC176, SyntheticDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  const uint8_t expectedState[kHaierAC176StateLength] = {
      0xA6, 0x86, 0x00, 0x00, 0x40, 0xA0, 0x00,
      0x20, 0x00, 0x00, 0x00, 0x00, 0x05, 0x31,
      0xB7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB7};

  irsend.reset();
  irsend.sendHaierAC176(expectedState);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(HAIER_AC176, irsend.capture.decode_type);
  EXPECT_EQ(kHaierAC176Bits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Power: On, Button: 5 (Power), Mode: 1 (Cool), Temp: 24C, Fan: 5 (Auto), "
      "Turbo: 0 (Off), Swing: 6 (UNKNOWN), Sleep: Off, Health: Off, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t result, prev;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &result, &prev));
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("HAIER_AC", typeToString(decode_type_t::HAIER_AC));
  ASSERT_EQ(decode_type_t::HAIER_AC, strToDecodeType("HAIER_AC"));
  ASSERT_TRUE(hasACState(decode_type_t::HAIER_AC));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::HAIER_AC));
  ASSERT_EQ(kHaierACBits, IRsend::defaultBits(decode_type_t::HAIER_AC));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::HAIER_AC));

  ASSERT_EQ("HAIER_AC_YRW02", typeToString(decode_type_t::HAIER_AC_YRW02));
  ASSERT_EQ(decode_type_t::HAIER_AC_YRW02, strToDecodeType("HAIER_AC_YRW02"));
  ASSERT_TRUE(hasACState(decode_type_t::HAIER_AC_YRW02));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::HAIER_AC_YRW02));
  ASSERT_EQ(kHaierACYRW02Bits,
            IRsend::defaultBits(decode_type_t::HAIER_AC_YRW02));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::HAIER_AC_YRW02));

  ASSERT_EQ("HAIER_AC176", typeToString(decode_type_t::HAIER_AC176));
  ASSERT_EQ(decode_type_t::HAIER_AC176, strToDecodeType("HAIER_AC176"));
  ASSERT_TRUE(hasACState(decode_type_t::HAIER_AC176));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::HAIER_AC176));
  ASSERT_EQ(kHaierAC176Bits, IRsend::defaultBits(decode_type_t::HAIER_AC176));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::HAIER_AC176));
}

TEST(TestHaierAC176Class, BuildKnownState) {
  IRHaierAC176 ac(kGpioUnused);
  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1480#issuecomment-884920033
  // heat, 24 C, fan max
  ac.setTemp(24);
  ac.setMode(kHaierAcYrw02Heat);
  ac.setFan(kHaierAcYrw02FanHigh);
  EXPECT_TRUE(ac.validChecksum(ac.getRaw()));
  EXPECT_EQ(
      "Power: On, Button: 4 (Fan), Mode: 4 (Heat), Temp: 24C, Fan: 1 (High), "
      "Turbo: 0 (Off), Swing: 0 (Off), Sleep: Off, Health: On, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off",
      ac.toString());
  /* Disabled pending:
     https://github.com/crankyoldgit/IRremoteESP8266/issues/1480#issuecomment-885636790
  const uint8_t expectedState[kHaierAC176StateLength] = {
      0xA6, 0x0A, 0x00, 0x00, 0x40, 0x20, 0x00,
      0x80, 0x00, 0x00, 0x00, 0x00, 0x04, 0x94,
      0xB7, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0xF7};
  EXPECT_STATE_EQ(expectedState, ac.getRaw(), kHaierAC176StateLength);
  */
}

TEST(TestHaierAC176Class, Timers) {
  IRHaierAC176 ac(kGpioUnused);
  EXPECT_EQ(kHaierAcYrw02NoTimers, ac.getTimerMode());
  EXPECT_EQ(0, ac.getOnTimer());
  EXPECT_EQ(0, ac.getOffTimer());

  // On Timer
  ac.setOnTimer(1);
  EXPECT_EQ(kHaierAcYrw02OnTimer, ac.getTimerMode());
  EXPECT_EQ(1, ac.getOnTimer());

  ac.setOnTimer(0);
  EXPECT_EQ(kHaierAcYrw02NoTimers, ac.getTimerMode());
  EXPECT_EQ(0, ac.getOnTimer());

  ac.setOnTimer(1 * 60);
  EXPECT_EQ(kHaierAcYrw02OnTimer, ac.getTimerMode());
  EXPECT_EQ(1 * 60, ac.getOnTimer());

  ac.setOnTimer(2 * 60 + 37);
  EXPECT_EQ(kHaierAcYrw02OnTimer, ac.getTimerMode());
  EXPECT_EQ(2 * 60 + 37, ac.getOnTimer());

  ac.setOnTimer(23 * 60 + 59);  // Max
  EXPECT_EQ(kHaierAcYrw02OnTimer, ac.getTimerMode());
  EXPECT_EQ(23 * 60 + 59, ac.getOnTimer());

  ac.setOnTimer(24 * 60);  // Beyond max
  EXPECT_EQ(kHaierAcYrw02OnTimer, ac.getTimerMode());
  EXPECT_EQ(23 * 60 + 59, ac.getOnTimer());  // Max

  ac.setOnTimer(0);
  EXPECT_EQ(kHaierAcYrw02NoTimers, ac.getTimerMode());
  EXPECT_EQ(0, ac.getOnTimer());

  // Off Timer
  ac.setOffTimer(1);
  EXPECT_EQ(kHaierAcYrw02OffTimer, ac.getTimerMode());
  EXPECT_EQ(1, ac.getOffTimer());

  ac.setOffTimer(0);
  EXPECT_EQ(kHaierAcYrw02NoTimers, ac.getTimerMode());
  EXPECT_EQ(0, ac.getOffTimer());

  ac.setOffTimer(1 * 60);
  EXPECT_EQ(kHaierAcYrw02OffTimer, ac.getTimerMode());
  EXPECT_EQ(1 * 60, ac.getOffTimer());

  ac.setOffTimer(2 * 60 + 37);
  EXPECT_EQ(kHaierAcYrw02OffTimer, ac.getTimerMode());
  ac.setOffTimer(23 * 60 + 59);  // Max
  EXPECT_EQ(kHaierAcYrw02OffTimer, ac.getTimerMode());
  EXPECT_EQ(23 * 60 + 59, ac.getOffTimer());

  ac.setOffTimer(24 * 60);  // Beyond max
  EXPECT_EQ(kHaierAcYrw02OffTimer, ac.getTimerMode());
  EXPECT_EQ(23 * 60 + 59, ac.getOffTimer());  // Max

  ac.setOffTimer(0);
  EXPECT_EQ(kHaierAcYrw02NoTimers, ac.getTimerMode());
  EXPECT_EQ(0, ac.getOffTimer());

  // Real data.
  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1480#issuecomment-894804106
  const uint8_t timer30m[22] = {
      0xA6, 0x82, 0x00, 0x40, 0x00, 0xA0, 0x00, 0x00, 0x1E, 0x00, 0x00,
      0x00, 0x10, 0x36, 0xB7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB7};
  const uint8_t timeroff[22] = {
      0xA6, 0x82, 0x00, 0x00, 0x40, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x10, 0x18, 0xB7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB7};
  // https://docs.google.com/spreadsheets/d/1wdOVS08wgK2pEP7hTZLYMmrQ9FZVmLpZF2HjNQaVxlU/edit#gid=0&range=A65
  const uint8_t timeroffthenon[22] = {
      0xA6, 0x82, 0x00, 0xA0, 0x40, 0xA0, 0x1E, 0x08, 0x00, 0x00, 0x00,
      0x00, 0x10, 0xDE, 0xB7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB7};
  ac.setRaw(timer30m);
  EXPECT_EQ(kHaierAcYrw02OnTimer, ac.getTimerMode());
  EXPECT_EQ(
      "Power: Off, Button: 0 (Temp Up), Mode: 0 (Auto), Temp: 24C, "
      "Fan: 5 (Auto), Turbo: 0 (Off), Swing: 2 (Middle), Sleep: Off, "
      "Health: Off, Timer Mode: 2 (On), On Timer: 00:30, Off Timer: Off",
      ac.toString());
  ac.setRaw(timeroff);
  EXPECT_EQ(kHaierAcYrw02NoTimers, ac.getTimerMode());
  EXPECT_EQ(
      "Power: On, Button: 0 (Temp Up), Mode: 0 (Auto), Temp: 24C, "
      "Fan: 5 (Auto), Turbo: 0 (Off), Swing: 2 (Middle), Sleep: Off, "
      "Health: Off, Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off",
      ac.toString());
  ac.setRaw(timeroffthenon);
  EXPECT_EQ(
      "Power: On, Button: 0 (Temp Up), Mode: 0 (Auto), Temp: 24C, "
      "Fan: 5 (Auto), Turbo: 0 (Off), Swing: 2 (Middle), Sleep: Off, "
      "Health: Off, Timer Mode: 5 (Off-On), On Timer: 08:00, Off Timer: 00:30",
      ac.toString());
  ac.setTimerMode(kHaierAcYrw02OnThenOffTimer);
  EXPECT_EQ(
      "Power: On, Button: 0 (Temp Up), Mode: 0 (Auto), Temp: 24C, "
      "Fan: 5 (Auto), Turbo: 0 (Off), Swing: 2 (Middle), Sleep: Off, "
      "Health: Off, Timer Mode: 4 (On-Off), On Timer: 08:00, Off Timer: 00:30",
      ac.toString());
  ac.setTimerMode(kHaierAcYrw02OffTimer);
  EXPECT_EQ(0, ac.getOnTimer());
  EXPECT_EQ(30, ac.getOffTimer());
  EXPECT_EQ(
      "Power: On, Button: 0 (Temp Up), Mode: 0 (Auto), Temp: 24C, "
      "Fan: 5 (Auto), Turbo: 0 (Off), Swing: 2 (Middle), Sleep: Off, "
      "Health: Off, Timer Mode: 1 (Off), On Timer: Off, Off Timer: 00:30",
      ac.toString());
  ac.setTimerMode(kHaierAcYrw02NoTimers);
  EXPECT_EQ(0, ac.getOnTimer());
  EXPECT_EQ(0, ac.getOffTimer());
  EXPECT_EQ(
      "Power: On, Button: 0 (Temp Up), Mode: 0 (Auto), Temp: 24C, "
      "Fan: 5 (Auto), Turbo: 0 (Off), Swing: 2 (Middle), Sleep: Off, "
      "Health: Off, Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off",
      ac.toString());
}
