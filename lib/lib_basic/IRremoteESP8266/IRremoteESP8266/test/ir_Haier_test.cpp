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
  IRHaierAC ac(kGpioUnused);
  ac.begin();

  ac.setCommand(kHaierAcCmdOff);
  EXPECT_EQ(kHaierAcCmdOff, ac.getCommand());
  ac.setCommand(kHaierAcCmdOn);
  EXPECT_EQ(kHaierAcCmdOn, ac.getCommand());
  ac.setCommand(kHaierAcCmdMode);
  EXPECT_EQ(kHaierAcCmdMode, ac.getCommand());
  ac.setCommand(kHaierAcCmdFan);
  EXPECT_EQ(kHaierAcCmdFan, ac.getCommand());
  ac.setCommand(kHaierAcCmdTempUp);
  EXPECT_EQ(kHaierAcCmdTempUp, ac.getCommand());
  ac.setCommand(kHaierAcCmdTempDown);
  EXPECT_EQ(kHaierAcCmdTempDown, ac.getCommand());
  ac.setCommand(kHaierAcCmdSleep);
  EXPECT_EQ(kHaierAcCmdSleep, ac.getCommand());
  ac.setCommand(kHaierAcCmdTimerSet);
  EXPECT_EQ(kHaierAcCmdTimerSet, ac.getCommand());
  ac.setCommand(kHaierAcCmdTimerCancel);
  EXPECT_EQ(kHaierAcCmdTimerCancel, ac.getCommand());
  ac.setCommand(kHaierAcCmdHealth);
  EXPECT_EQ(kHaierAcCmdHealth, ac.getCommand());
  ac.setCommand(kHaierAcCmdSwing);
  EXPECT_EQ(kHaierAcCmdSwing, ac.getCommand());
  ac.setCommand(kHaierAcCmdOn);
  EXPECT_EQ(kHaierAcCmdOn, ac.getCommand());
  ac.setCommand(kHaierAcCmdOff);
  EXPECT_EQ(kHaierAcCmdOff, ac.getCommand());

  // Test unexpected values.
  ac.setCommand(0b00001110);
  EXPECT_EQ(kHaierAcCmdOff, ac.getCommand());
  ac.setCommand(0b00001111);
  EXPECT_EQ(kHaierAcCmdOff, ac.getCommand());
  ac.setCommand(0b00000100);
  EXPECT_EQ(kHaierAcCmdOff, ac.getCommand());
}

TEST(TestHaierACClass, OperatingMode) {
  IRHaierAC ac(kGpioUnused);
  ac.begin();

  ac.setMode(kHaierAcAuto);
  EXPECT_EQ(kHaierAcAuto, ac.getMode());

  ac.setMode(kHaierAcCool);
  EXPECT_EQ(kHaierAcCool, ac.getMode());

  ac.setMode(kHaierAcHeat);
  EXPECT_EQ(kHaierAcHeat, ac.getMode());

  ac.setMode(kHaierAcFan);
  EXPECT_EQ(kHaierAcFan, ac.getMode());

  ac.setMode(kHaierAcDry);
  EXPECT_EQ(kHaierAcDry, ac.getMode());

  ac.setMode(kHaierAcAuto - 1);
  EXPECT_EQ(kHaierAcAuto, ac.getMode());

  ac.setMode(kHaierAcCool);
  EXPECT_EQ(kHaierAcCool, ac.getMode());

  ac.setMode(kHaierAcFan + 1);
  EXPECT_EQ(kHaierAcAuto, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kHaierAcAuto, ac.getMode());
}

TEST(TestHaierACClass, Temperature) {
  IRHaierAC ac(kGpioUnused);
  ac.begin();

  ac.setTemp(kHaierAcMinTemp);
  EXPECT_EQ(kHaierAcMinTemp, ac.getTemp());

  ac.setCommand(kHaierAcCmdOn);
  ac.setTemp(kHaierAcMinTemp + 1);
  EXPECT_EQ(kHaierAcMinTemp + 1, ac.getTemp());
  EXPECT_EQ(kHaierAcCmdTempUp, ac.getCommand());

  ac.setTemp(kHaierAcMaxTemp);
  EXPECT_EQ(kHaierAcMaxTemp, ac.getTemp());
  EXPECT_EQ(kHaierAcCmdTempUp, ac.getCommand());

  ac.setTemp(kHaierAcMinTemp - 1);
  EXPECT_EQ(kHaierAcMinTemp, ac.getTemp());
  EXPECT_EQ(kHaierAcCmdTempDown, ac.getCommand());

  ac.setTemp(kHaierAcMaxTemp + 1);
  EXPECT_EQ(kHaierAcMaxTemp, ac.getTemp());
  EXPECT_EQ(kHaierAcCmdTempUp, ac.getCommand());

  ac.setTemp(23);
  EXPECT_EQ(23, ac.getTemp());
  EXPECT_EQ(kHaierAcCmdTempDown, ac.getCommand());
  ac.setCommand(kHaierAcCmdOn);
  ac.setTemp(23);
  EXPECT_EQ(23, ac.getTemp());
  EXPECT_EQ(kHaierAcCmdOn, ac.getCommand());

  ac.setTemp(0);
  EXPECT_EQ(kHaierAcMinTemp, ac.getTemp());
  EXPECT_EQ(kHaierAcCmdTempDown, ac.getCommand());

  ac.setTemp(255);
  EXPECT_EQ(kHaierAcMaxTemp, ac.getTemp());
  EXPECT_EQ(kHaierAcCmdTempUp, ac.getCommand());
}

TEST(TestHaierACClass, FanSpeed) {
  IRHaierAC ac(kGpioUnused);
  ac.begin();
  ac.setFan(kHaierAcFanLow);
  ac.setCommand(kHaierAcCmdOn);

  ac.setFan(kHaierAcFanAuto);
  EXPECT_EQ(kHaierAcFanAuto, ac.getFan());
  EXPECT_EQ(kHaierAcCmdFan, ac.getCommand());

  ac.setFan(kHaierAcFanLow);
  EXPECT_EQ(kHaierAcFanLow, ac.getFan());
  ac.setFan(kHaierAcFanMed);
  EXPECT_EQ(kHaierAcFanMed, ac.getFan());
  ac.setFan(kHaierAcFanHigh);
  EXPECT_EQ(kHaierAcFanHigh, ac.getFan());

  ac.setCommand(kHaierAcCmdOn);
  ac.setFan(kHaierAcFanHigh);
  EXPECT_EQ(kHaierAcFanHigh, ac.getFan());
  EXPECT_EQ(kHaierAcCmdOn, ac.getCommand());
}

TEST(TestHaierACClass, SwingV) {
  IRHaierAC ac(kGpioUnused);
  ac.begin();
  ac.setFan(kHaierAcFanLow);
  ac.setCommand(kHaierAcCmdOn);

  ac.setSwingV(kHaierAcSwingVOff);
  EXPECT_EQ(kHaierAcSwingVOff, ac.getSwingV());

  ac.setSwingV(kHaierAcSwingVUp);
  EXPECT_EQ(kHaierAcSwingVUp, ac.getSwingV());
  EXPECT_EQ(kHaierAcCmdSwing, ac.getCommand());

  ac.setSwingV(kHaierAcSwingVDown);
  EXPECT_EQ(kHaierAcSwingVDown, ac.getSwingV());
  EXPECT_EQ(kHaierAcCmdSwing, ac.getCommand());

  ac.setSwingV(kHaierAcSwingVChg);
  EXPECT_EQ(kHaierAcSwingVChg, ac.getSwingV());
  EXPECT_EQ(kHaierAcCmdSwing, ac.getCommand());
}

TEST(TestHaierACClass, CurrentTime) {
  IRHaierAC ac(kGpioUnused);
  ac.begin();
  EXPECT_EQ(0, ac.getCurrTime());

  ac.setCurrTime(1);
  EXPECT_EQ(1, ac.getCurrTime());

  ac.setCurrTime(60);
  EXPECT_EQ(60, ac.getCurrTime());

  ac.setCurrTime(61);
  EXPECT_EQ(61, ac.getCurrTime());

  ac.setCurrTime(18 * 60 + 34);  // 18:34
  EXPECT_EQ(1114, ac.getCurrTime());

  ac.setCurrTime(23 * 60 + 59);                  // 23:59
  EXPECT_EQ(kHaierAcMaxTime, ac.getCurrTime());  // 23:59

  ac.setCurrTime(23 * 60 + 59 + 1);              // 24:00
  EXPECT_EQ(kHaierAcMaxTime, ac.getCurrTime());  // 23:59

  ac.setCurrTime(UINT16_MAX);
  EXPECT_EQ(kHaierAcMaxTime, ac.getCurrTime());  // 23:59
}

TEST(TestHaierACClass, Timers) {
  IRHaierAC ac(kGpioUnused);
  ac.begin();

  ac.setCommand(kHaierAcCmdOn);

  // Off by default.
  EXPECT_GT(0, ac.getOnTimer());
  EXPECT_GT(0, ac.getOffTimer());
  EXPECT_EQ(kHaierAcCmdOn, ac.getCommand());

  // On Timer.
  ac.setOnTimer(6 * 60);               // 6am
  EXPECT_EQ(6 * 60, ac.getOnTimer());  // 6am
  EXPECT_GT(0, ac.getOffTimer());
  EXPECT_EQ(kHaierAcCmdTimerSet, ac.getCommand());

  ac.setCommand(kHaierAcCmdOn);
  EXPECT_EQ(6 * 60, ac.getOnTimer());  // 6am
  EXPECT_GT(0, ac.getOffTimer());
  EXPECT_EQ(kHaierAcCmdOn, ac.getCommand());

  ac.cancelTimers();
  EXPECT_GT(0, ac.getOnTimer());
  EXPECT_GT(0, ac.getOffTimer());
  EXPECT_EQ(kHaierAcCmdTimerCancel, ac.getCommand());

  // Off Timer.
  ac.setOffTimer(18 * 60 + 30);  // 6:30pm
  EXPECT_GT(0, ac.getOnTimer());
  EXPECT_EQ(18 * 60 + 30, ac.getOffTimer());  // 6:30pm
  EXPECT_EQ(kHaierAcCmdTimerSet, ac.getCommand());

  ac.setCommand(kHaierAcCmdOn);
  EXPECT_GT(0, ac.getOnTimer());
  EXPECT_EQ(18 * 60 + 30, ac.getOffTimer());  // 6:30pm
  EXPECT_EQ(kHaierAcCmdOn, ac.getCommand());

  ac.cancelTimers();
  EXPECT_GT(0, ac.getOnTimer());
  EXPECT_GT(0, ac.getOffTimer());
  EXPECT_EQ(kHaierAcCmdTimerCancel, ac.getCommand());

  // Both Timers.
  ac.setOnTimer(6 * 60);  // 6am
  EXPECT_EQ(kHaierAcCmdTimerSet, ac.getCommand());
  ac.setOffTimer(18 * 60 + 30);  // 6:30pm
  EXPECT_EQ(kHaierAcCmdTimerSet, ac.getCommand());
  EXPECT_EQ(6 * 60, ac.getOnTimer());         // 6am
  EXPECT_EQ(18 * 60 + 30, ac.getOffTimer());  // 6:30pm

  ac.cancelTimers();
  EXPECT_GT(0, ac.getOnTimer());
  EXPECT_GT(0, ac.getOffTimer());
  EXPECT_EQ(kHaierAcCmdTimerCancel, ac.getCommand());
}

TEST(TestHaierACClass, MessageConstuction) {
  IRHaierAC ac(kGpioUnused);

  EXPECT_EQ(
      "Command: 1 (On), Mode: 0 (Auto), Temp: 25C, Fan: 1 (Low), "
      "Swing(V): 0 (Off), Sleep: Off, Health: Off, "
      "Clock: 00:00, On Timer: Off, Off Timer: Off",
      ac.toString());
  ac.setMode(kHaierAcCool);
  ac.setTemp(21);
  ac.setFan(kHaierAcFanHigh);
  EXPECT_EQ(
      "Command: 3 (Fan), Mode: 1 (Cool), Temp: 21C, Fan: 3 (High), "
      "Swing(V): 0 (Off), Sleep: Off, Health: Off, "
      "Clock: 00:00, On Timer: Off, Off Timer: Off",
      ac.toString());
  ac.setSwingV(kHaierAcSwingVChg);
  ac.setHealth(true);
  ac.setSleep(true);
  ac.setCurrTime(615);  // 10:15am
  EXPECT_EQ(
      "Command: 8 (Sleep), Mode: 1 (Cool), Temp: 21C, Fan: 3 (High), "
      "Swing(V): 3 (Change), Sleep: On, Health: On, "
      "Clock: 10:15, On Timer: Off, Off Timer: Off",
      ac.toString());
  ac.setOnTimer(800);    // 1:20pm
  ac.setOffTimer(1125);  // 6:45pm
  ac.setCommand(kHaierAcCmdOn);

  EXPECT_EQ(
      "Command: 1 (On), Mode: 1 (Cool), Temp: 21C, Fan: 3 (High), "
      "Swing(V): 3 (Change), Sleep: On, Health: On, "
      "Clock: 10:15, On Timer: 13:20, Off Timer: 18:45",
      ac.toString());

  // Now change a few already set things.
  ac.setMode(kHaierAcHeat);
  EXPECT_EQ(
      "Command: 2 (Mode), Mode: 3 (Heat), Temp: 21C, Fan: 3 (High), "
      "Swing(V): 3 (Change), Sleep: On, Health: On, "
      "Clock: 10:15, On Timer: 13:20, Off Timer: 18:45",
      ac.toString());

  ac.setTemp(25);
  EXPECT_EQ(
      "Command: 6 (Temp Up), Mode: 3 (Heat), Temp: 25C, Fan: 3 (High), "
      "Swing(V): 3 (Change), Sleep: On, Health: On, "
      "Clock: 10:15, On Timer: 13:20, Off Timer: 18:45",
      ac.toString());

  uint8_t expectedState[kHaierACStateLength] = {0xA5, 0x96, 0xEA, 0xCF, 0x32,
                                                0x6D, 0x6D, 0x54, 0x54};
  EXPECT_STATE_EQ(expectedState, ac.getRaw(), kHaierACBits);

  // Check that the checksum is valid.
  EXPECT_TRUE(IRHaierAC::validChecksum(ac.getRaw()));

  // Now load up some random data.
  uint8_t randomState[kHaierACStateLength] = {0x52, 0x49, 0x50, 0x20, 0x54,
                                              0x61, 0x6C, 0x69, 0x61};
  EXPECT_FALSE(IRHaierAC::validChecksum(randomState));
  ac.setRaw(randomState);
  EXPECT_EQ(
      "Command: 9 (Timer Set), Mode: 3 (Heat), Temp: 20C, Fan: 3 (High), "
      "Swing(V): 1 (Up), Sleep: On, Health: Off, "
      "Clock: 16:32, On Timer: Off, Off Timer: Off",
      ac.toString());
  // getRaw() should correct the checksum.
  EXPECT_TRUE(IRHaierAC::validChecksum(ac.getRaw()));
}

// Tests for the IRHaierACYRW02 class.

TEST(TestHaierACYRW02Class, Button) {
  IRHaierACYRW02 ac(kGpioUnused);
  ac.begin();

  ac.setButton(kHaierAcYrw02ButtonPower);
  EXPECT_EQ(kHaierAcYrw02ButtonPower, ac.getButton());
  ac.setButton(kHaierAcYrw02ButtonMode);
  EXPECT_EQ(kHaierAcYrw02ButtonMode, ac.getButton());
  ac.setButton(kHaierAcYrw02ButtonSleep);
  EXPECT_EQ(kHaierAcYrw02ButtonSleep, ac.getButton());
  ac.setButton(kHaierAcYrw02ButtonFan);

  // Test unexpected values.
  ac.setButton(0xFF);
  EXPECT_EQ(kHaierAcYrw02ButtonFan, ac.getButton());
  ac.setButton(0x10);
  EXPECT_EQ(kHaierAcYrw02ButtonFan, ac.getButton());
}

TEST(TestHaierACYRW02Class, OperatingMode) {
  IRHaierACYRW02 ac(kGpioUnused);
  ac.begin();

  ac.setButton(kHaierAcYrw02ButtonPower);
  ac.setMode(kHaierAcYrw02Auto);
  EXPECT_EQ(kHaierAcYrw02Auto, ac.getMode());
  EXPECT_EQ(kHaierAcYrw02ButtonMode, ac.getButton());

  ac.setMode(kHaierAcYrw02Cool);
  EXPECT_EQ(kHaierAcYrw02Cool, ac.getMode());

  ac.setMode(kHaierAcYrw02Heat);
  EXPECT_EQ(kHaierAcYrw02Heat, ac.getMode());

  ac.setMode(kHaierAcYrw02Fan);
  EXPECT_EQ(kHaierAcYrw02Fan, ac.getMode());

  ac.setMode(kHaierAcYrw02Dry);
  EXPECT_EQ(kHaierAcYrw02Dry, ac.getMode());

  ac.setMode(kHaierAcYrw02Auto - 1);
  EXPECT_EQ(kHaierAcYrw02Auto, ac.getMode());

  ac.setMode(kHaierAcYrw02Cool);
  EXPECT_EQ(kHaierAcYrw02Cool, ac.getMode());

  ac.setMode(kHaierAcYrw02Fan + 1);
  EXPECT_EQ(kHaierAcYrw02Auto, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kHaierAcYrw02Auto, ac.getMode());
}

TEST(TestHaierACYRW02Class, Temperature) {
  IRHaierACYRW02 ac(kGpioUnused);
  ac.begin();

  ac.setTemp(kHaierAcYrw02MinTempC);
  EXPECT_EQ(kHaierAcYrw02MinTempC, ac.getTemp());

  ac.setButton(kHaierAcYrw02ButtonPower);
  ac.setTemp(kHaierAcYrw02MinTempC + 1);
  EXPECT_EQ(kHaierAcYrw02MinTempC + 1, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, ac.getButton());

  ac.setTemp(kHaierAcYrw02MaxTempC);
  EXPECT_EQ(kHaierAcYrw02MaxTempC, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, ac.getButton());

  ac.setTemp(kHaierAcYrw02MinTempC - 1);
  EXPECT_EQ(kHaierAcYrw02MinTempC, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempDown, ac.getButton());

  ac.setTemp(kHaierAcYrw02MaxTempC + 1);
  EXPECT_EQ(kHaierAcYrw02MaxTempC, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, ac.getButton());

  ac.setTemp(23);
  EXPECT_EQ(23, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempDown, ac.getButton());
  ac.setButton(kHaierAcYrw02ButtonPower);
  ac.setTemp(23);
  EXPECT_EQ(23, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonPower, ac.getButton());

  ac.setTemp(kHaierAcYrw02MinTempF, true);
  EXPECT_EQ(kHaierAcYrw02MinTempF, ac.getTemp());

  ac.setButton(kHaierAcYrw02ButtonPower);
  ac.setTemp(kHaierAcYrw02MinTempF + 1, true);
  EXPECT_EQ(kHaierAcYrw02MinTempF + 1, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, ac.getButton());

  ac.setTemp(kHaierAcYrw02MaxTempF, true);
  EXPECT_EQ(kHaierAcYrw02MaxTempF, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, ac.getButton());

  ac.setTemp(kHaierAcYrw02MinTempF - 1, true);
  EXPECT_EQ(kHaierAcYrw02MinTempF, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempDown, ac.getButton());

  ac.setTemp(kHaierAcYrw02MaxTempF + 1, true);
  EXPECT_EQ(kHaierAcYrw02MaxTempF, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, ac.getButton());

  ac.setTemp(66, true);
  EXPECT_EQ(66, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempDown, ac.getButton());
  ac.setButton(kHaierAcYrw02ButtonPower);
  ac.setTemp(66, true);
  EXPECT_EQ(66, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonPower, ac.getButton());

  // Test specific cases for converting to Fahrenheit
  ac.setTemp(76, true);
  EXPECT_EQ(76, ac.getTemp());
  ac.setTemp(77, true);
  EXPECT_EQ(77, ac.getTemp());
  ac.setTemp(78, true);
  EXPECT_EQ(78, ac.getTemp());

  ac.setTemp(24);
  EXPECT_EQ(kHaierAcYrw02ButtonCFAB, ac.getButton());

  ac.setTemp(0);
  EXPECT_EQ(kHaierAcYrw02MinTempC, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempDown, ac.getButton());

  ac.setTemp(255);
  EXPECT_EQ(kHaierAcMaxTemp, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, ac.getButton());
}

TEST(TestHaierACYRW02Class, HealthMode) {
  IRHaierACYRW02 ac(kGpioUnused);
  ac.begin();

  ac.setHealth(true);
  EXPECT_TRUE(ac.getHealth());
  EXPECT_EQ(kHaierAcYrw02ButtonHealth, ac.getButton());

  ac.setButton(kHaierAcYrw02ButtonTempUp);
  ac.setHealth(false);
  EXPECT_FALSE(ac.getHealth());
  EXPECT_EQ(kHaierAcYrw02ButtonHealth, ac.getButton());

  ac.setHealth(true);
  EXPECT_TRUE(ac.getHealth());
  EXPECT_EQ(kHaierAcYrw02ButtonHealth, ac.getButton());
}

TEST(TestHaierACYRW02Class, Power) {
  IRHaierACYRW02 ac(kGpioUnused);
  ac.begin();

  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());
  EXPECT_EQ(kHaierAcYrw02ButtonPower, ac.getButton());

  ac.setButton(kHaierAcYrw02ButtonTempUp);
  ac.setPower(false);
  EXPECT_FALSE(ac.getPower());
  EXPECT_EQ(kHaierAcYrw02ButtonPower, ac.getButton());

  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());
  EXPECT_EQ(kHaierAcYrw02ButtonPower, ac.getButton());

  ac.off();
  EXPECT_FALSE(ac.getPower());
  ac.on();
  EXPECT_TRUE(ac.getPower());
}

TEST(TestHaierACYRW02Class, SleepMode) {
  IRHaierACYRW02 ac(kGpioUnused);
  ac.begin();

  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
  EXPECT_EQ(kHaierAcYrw02ButtonSleep, ac.getButton());

  ac.setButton(kHaierAcYrw02ButtonTempUp);
  ac.setSleep(false);
  EXPECT_FALSE(ac.getSleep());
  EXPECT_EQ(kHaierAcYrw02ButtonSleep, ac.getButton());

  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
  EXPECT_EQ(kHaierAcYrw02ButtonSleep, ac.getButton());
}

TEST(TestHaierACYRW02Class, TurboAndQuiet) {
  IRHaierACYRW02 ac(kGpioUnused);
  ac.begin();

  ac.setMode(kHaierAcYrw02Cool);  // Turbo & Quiet is allowed in this mode.
  ac.setTurbo(false);
  ac.setQuiet(false);
  EXPECT_FALSE(ac.getTurbo());
  EXPECT_FALSE(ac.getQuiet());
  EXPECT_EQ(kHaierAcYrw02ButtonTurbo, ac.getButton());

  ac.setButton(kHaierAcYrw02ButtonTempUp);

  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());
  EXPECT_FALSE(ac.getQuiet());
  EXPECT_EQ(kHaierAcYrw02ButtonTurbo, ac.getButton());

  ac.setQuiet(true);
  EXPECT_FALSE(ac.getTurbo());
  EXPECT_TRUE(ac.getQuiet());
  EXPECT_EQ(kHaierAcYrw02ButtonTurbo, ac.getButton());

  ac.setTurbo(false);
  ac.setQuiet(false);
  EXPECT_FALSE(ac.getTurbo());
  EXPECT_FALSE(ac.getQuiet());
  EXPECT_EQ(kHaierAcYrw02ButtonTurbo, ac.getButton());

  ac.setMode(kHaierAcYrw02Auto);  // Turbo & Quiet is not allowed in this mode.
  EXPECT_FALSE(ac.getTurbo());
  EXPECT_FALSE(ac.getQuiet());
  ac.setTurbo(true);
  EXPECT_FALSE(ac.getTurbo());
  EXPECT_NE(kHaierAcYrw02ButtonTurbo, ac.getButton());
  ac.setQuiet(true);
  EXPECT_FALSE(ac.getQuiet());
  EXPECT_NE(kHaierAcYrw02ButtonTurbo, ac.getButton());
}

TEST(TestHaierACYRW02Class, Fan) {
  IRHaierACYRW02 ac(kGpioUnused);
  ac.begin();

  ac.setFan(kHaierAcYrw02FanAuto);
  EXPECT_EQ(kHaierAcYrw02FanAuto, ac.getFan());
  EXPECT_EQ(kHaierAcYrw02ButtonFan, ac.getButton());

  ac.setButton(kHaierAcYrw02ButtonTempUp);

  ac.setFan(kHaierAcYrw02FanLow);
  EXPECT_EQ(kHaierAcYrw02FanLow, ac.getFan());
  EXPECT_EQ(kHaierAcYrw02ButtonFan, ac.getButton());

  ac.setFan(kHaierAcYrw02FanHigh);
  EXPECT_EQ(kHaierAcYrw02FanHigh, ac.getFan());
  EXPECT_EQ(kHaierAcYrw02ButtonFan, ac.getButton());

  ac.setFan(kHaierAcYrw02FanMed);
  EXPECT_EQ(kHaierAcYrw02FanMed, ac.getFan());
  EXPECT_EQ(kHaierAcYrw02ButtonFan, ac.getButton());

  // Test unexpected values.
  ac.setButton(kHaierAcYrw02ButtonTempUp);
  ac.setFan(0x00);
  EXPECT_EQ(kHaierAcYrw02FanMed, ac.getFan());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, ac.getButton());
}

TEST(TestHaierACYRW02Class, SwingV) {
  IRHaierACYRW02 ac(kGpioUnused);
  ac.begin();

  ac.setSwingV(kHaierAcYrw02SwingVOff);
  EXPECT_EQ(kHaierAcYrw02SwingVOff, ac.getSwingV());
  EXPECT_EQ(kHaierAcYrw02ButtonSwingV, ac.getButton());

  ac.setButton(kHaierAcYrw02ButtonTempUp);

  ac.setSwingV(kHaierAcYrw02SwingVAuto);
  EXPECT_EQ(kHaierAcYrw02SwingVAuto, ac.getSwingV());
  EXPECT_EQ(kHaierAcYrw02ButtonSwingV, ac.getButton());

  ac.setSwingV(kHaierAcYrw02SwingVTop);
  EXPECT_EQ(kHaierAcYrw02SwingVTop, ac.getSwingV());
  EXPECT_EQ(kHaierAcYrw02ButtonSwingV, ac.getButton());

  ac.setSwingV(kHaierAcYrw02SwingVDown);
  EXPECT_EQ(kHaierAcYrw02SwingVDown, ac.getSwingV());
  EXPECT_EQ(kHaierAcYrw02ButtonSwingV, ac.getButton());

  // Test unexpected values.
  ac.setButton(kHaierAcYrw02ButtonTempUp);
  ac.setSwingV(0xFF);
  EXPECT_EQ(kHaierAcYrw02SwingVDown, ac.getSwingV());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, ac.getButton());

  // Test the mode-dependant positions.
  ac.setMode(kHaierAcYrw02Auto);
  ac.setSwingV(kHaierAcYrw02SwingVMiddle);
  EXPECT_EQ(kHaierAcYrw02SwingVMiddle, ac.getSwingV());
  EXPECT_EQ(kHaierAcYrw02ButtonSwingV, ac.getButton());
  ac.setMode(kHaierAcYrw02Heat);
  ac.setSwingV(kHaierAcYrw02SwingVMiddle);
  EXPECT_EQ(kHaierAcYrw02SwingVBottom, ac.getSwingV());
  ac.setSwingV(kHaierAcYrw02SwingVAuto);
  EXPECT_EQ(kHaierAcYrw02SwingVAuto, ac.getSwingV());
  ac.setSwingV(kHaierAcYrw02SwingVBottom);
  EXPECT_EQ(kHaierAcYrw02SwingVBottom, ac.getSwingV());
  ac.setMode(kHaierAcYrw02Cool);
  ac.setSwingV(kHaierAcYrw02SwingVBottom);
  EXPECT_EQ(kHaierAcYrw02SwingVMiddle, ac.getSwingV());
}

TEST(TestHaierACYRW02Class, SwingH) {
  IRHaierACYRW02 ac(kGpioUnused);
  ac.begin();

  ac.setSwingH(kHaierAcYrw02SwingVOff);
  EXPECT_EQ(kHaierAcYrw02SwingHMiddle, ac.getSwingH());
  EXPECT_EQ(kHaierAcYrw02ButtonSwingH, ac.getButton());

  ac.setButton(kHaierAcYrw02ButtonTempUp);

  ac.setSwingH(kHaierAcYrw02SwingHLeftMax);
  EXPECT_EQ(kHaierAcYrw02SwingHLeftMax, ac.getSwingH());
  EXPECT_EQ(kHaierAcYrw02ButtonSwingH, ac.getButton());

  ac.setSwingH(kHaierAcYrw02SwingHLeft);
  EXPECT_EQ(kHaierAcYrw02SwingHLeft, ac.getSwingH());
  EXPECT_EQ(kHaierAcYrw02ButtonSwingH, ac.getButton());

  ac.setSwingH(kHaierAcYrw02SwingHRight);
  EXPECT_EQ(kHaierAcYrw02SwingHRight, ac.getSwingH());
  EXPECT_EQ(kHaierAcYrw02ButtonSwingH, ac.getButton());

  ac.setSwingH(kHaierAcYrw02SwingHRightMax);
  EXPECT_EQ(kHaierAcYrw02SwingHRightMax, ac.getSwingH());
  EXPECT_EQ(kHaierAcYrw02ButtonSwingH, ac.getButton());

  ac.setSwingH(kHaierAcYrw02SwingHAuto);
  EXPECT_EQ(kHaierAcYrw02SwingHAuto, ac.getSwingH());
  EXPECT_EQ(kHaierAcYrw02ButtonSwingH, ac.getButton());

  // Test unexpected values.
  ac.setButton(kHaierAcYrw02ButtonTempUp);
  ac.setSwingH(0xFF);
  EXPECT_EQ(kHaierAcYrw02SwingHAuto, ac.getSwingH());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, ac.getButton());
}

TEST(TestHaierACYRW02Class, Lock) {
  IRHaierACYRW02 ac(kGpioUnused);
  ac.begin();

  ac.setLock(true);
  EXPECT_TRUE(ac.getLock());
  EXPECT_EQ(kHaierAcYrw02ButtonLock, ac.getButton());

  ac.setButton(kHaierAcYrw02ButtonTempUp);
  ac.setLock(false);
  EXPECT_FALSE(ac.getLock());
  EXPECT_EQ(kHaierAcYrw02ButtonLock, ac.getButton());

  ac.setLock(true);
  EXPECT_TRUE(ac.getLock());
  EXPECT_EQ(kHaierAcYrw02ButtonLock, ac.getButton());
}

TEST(TestHaierACYRW02Class, MessageConstuction) {
  IRHaierACYRW02 ac(kGpioUnused);

  EXPECT_EQ(
      "Model: 1 (V9014557-A), Power: On, Button: 5 (Power), "
      "Mode: 0 (Auto), Temp: 25C, Fan: 5 (Auto), Turbo: Off, Quiet: Off, "
      "Swing(V): 0 (Off), Swing(H): 0 (Middle), Sleep: Off, Health: On, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off, Lock: Off",
      ac.toString());
  ac.setMode(kHaierAcYrw02Cool);
  ac.setTemp(21);
  ac.setFan(kHaierAcYrw02FanHigh);
  EXPECT_EQ(
      "Model: 1 (V9014557-A), Power: On, Button: 4 (Fan), "
      "Mode: 1 (Cool), Temp: 21C, Fan: 1 (High), Turbo: Off, Quiet: Off, "
      "Swing(V): 0 (Off), Swing(H): 0 (Middle), Sleep: Off, Health: On, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off, Lock: Off",
      ac.toString());

  ac.setTemp(75, true);
  ac.setSwingV(kHaierAcYrw02SwingVMiddle);
  ac.setHealth(false);
  ac.setSleep(true);
  ac.setTurbo(true);
  EXPECT_EQ(
      "Model: 1 (V9014557-A), Power: On, Button: 8 (Turbo), "
      "Mode: 1 (Cool), Temp: 75F, Fan: 1 (High), Turbo: On, Quiet: Off, "
      "Swing(V): 2 (Middle), Swing(H): 0 (Middle), Sleep: On, Health: Off, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off, Lock: Off",
      ac.toString());
}

// Decode "real" state messages.
TEST(TestHaierACYRW02Class, RealStates) {
  uint8_t expectedState1[kHaierACYRW02StateLength] = {
      0xA6, 0xE1, 0x00, 0x00, 0x40, 0x20, 0x00,
      0x80, 0x00, 0x00, 0x00, 0x00, 0x07, 0x6E};

  IRHaierACYRW02 ac(kGpioUnused);
  ac.setRaw(expectedState1);
  EXPECT_EQ(
      "Model: 1 (V9014557-A), Power: On, Button: 7 (Health), "
      "Mode: 4 (Heat), Temp: 30C, Fan: 1 (High), Turbo: Off, Quiet: Off, "
      "Swing(V): 1 (Highest), Swing(H): 0 (Middle), Sleep: Off, Health: Off, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off, Lock: Off",
      ac.toString());

  uint8_t expectedState2[kHaierACYRW02StateLength] = {
      0xA6, 0xE0, 0x00, 0x00, 0x00, 0x20, 0x00,
      0x80, 0x00, 0x00, 0x00, 0x00, 0x05, 0x75};
  ac.setRaw(expectedState2);
  EXPECT_EQ(
      "Model: 1 (V9014557-A), Power: Off, Button: 5 (Power), "
      "Mode: 4 (Heat), Temp: 30C, Fan: 1 (High), Turbo: Off, Quiet: Off, "
      "Swing(V): 0 (Off), Swing(H): 0 (Middle), Sleep: Off, Health: Off, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off, Lock: Off",
      ac.toString());

  uint8_t expectedState3[kHaierACYRW02StateLength] = {
      0xA6, 0x02, 0x00, 0x02, 0x40, 0x20, 0x00,
      0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x2B};
  ac.setRaw(expectedState3);
  EXPECT_EQ(
      "Model: 1 (V9014557-A), Power: On, Button: 1 (Temp Down), "
      "Mode: 1 (Cool), Temp: 16C, Fan: 1 (High), Turbo: Off, Quiet: Off, "
      "Swing(V): 2 (Middle), Swing(H): 0 (Middle), Sleep: Off, Health: On, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off, Lock: Off",
      ac.toString());

  // cool 25, health, fan auto, vertical swing auto,  sleep on
  uint8_t expectedState4[kHaierACYRW02StateLength] = {
      0xA6, 0x9C, 0x00, 0x02, 0x40, 0xA8, 0x00,
      0x20, 0x80, 0x00, 0x00, 0x00, 0x0B, 0xD7};
  ac.setRaw(expectedState4);
  EXPECT_EQ(
      "Model: 1 (V9014557-A), Power: On, Button: 11 (Sleep), "
      "Mode: 1 (Cool), Temp: 25C, Fan: 5 (Auto), Turbo: Off, Quiet: Off, "
      "Swing(V): 12 (Auto), Swing(H): 0 (Middle), Sleep: On, Health: On, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off, Lock: Off",
      ac.toString());

  // cool 25, health, fan 3, vertical swing auto,  sleep on
  uint8_t expectedState5[kHaierACYRW02StateLength] = {
      0xA6, 0x9C, 0x00, 0x02, 0x40, 0x27, 0x36,
      0x20, 0x80, 0x00, 0x00, 0x00, 0x04, 0x85};
  ac.setRaw(expectedState5);
  EXPECT_EQ(
      "Model: 1 (V9014557-A), Power: On, Button: 4 (Fan), "
      "Mode: 1 (Cool), Temp: 25C, Fan: 1 (High), Turbo: Off, Quiet: Off, "
      "Swing(V): 12 (Auto), Swing(H): 0 (Middle), Sleep: On, Health: On, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off, Lock: Off",
      ac.toString());
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
      "Swing(V): 0 (Off), Sleep: Off, Health: Off, "
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

  IRHaierAC ac(kGpioUnused);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Command: 6 (Temp Up), Mode: 1 (Cool), Temp: 22C, Fan: 1 (Low), "
      "Swing(V): 0 (Off), Sleep: Off, Health: Off, "
      "Clock: 00:01, On Timer: Off, Off Timer: Off",
      ac.toString());
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

  IRHaierAC ac(kGpioUnused);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Command: 12 (Health), Mode: 1 (Cool), Temp: 30C, Fan: 1 (Low), "
      "Swing(V): 0 (Off), Sleep: Off, Health: On, "
      "Clock: 00:09, On Timer: Off, Off Timer: Off",
      ac.toString());
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

  IRHaierACYRW02 ac(kGpioUnused);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Model: 1 (V9014557-A), Power: On, Button: 5 (Power), "
      "Mode: 1 (Cool), Temp: 17C, Fan: 1 (High), Turbo: Off, Quiet: Off, "
      "Swing(V): 2 (Middle), Swing(H): 0 (Middle), Sleep: Off, Health: On, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off, Lock: Off",
      ac.toString());
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
      "Swing(V): 0 (Off), Sleep: Off, Health: Off, Clock: 00:00, "
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
      "Swing(V): 0 (Off), Sleep: Off, Health: Off, Clock: 00:00, "
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
      "Swing(V): 0 (Off), Sleep: Off, Health: Off, Clock: 00:00, "
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
  ac.setSwingV(kHaierAcSwingVChg);
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
  ASSERT_TRUE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

TEST(TestHaierACYRW02Class, toCommon) {
  IRHaierACYRW02 ac(kGpioUnused);
  ac.setPower(true);
  ac.setMode(kHaierAcYrw02Cool);
  ac.setTemp(20);
  ac.setFan(kHaierAcYrw02FanHigh);
  ac.setSwingV(kHaierAcYrw02SwingVTop);
  ac.setSwingH(kHaierAcYrw02SwingHRightMax);
  ac.setHealth(true);
  ac.setSleep(true);
  ac.setTurbo(true);
  // Now test it.
  ASSERT_EQ(decode_type_t::HAIER_AC_YRW02, ac.toCommon().protocol);
  ASSERT_EQ(1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_TRUE(ac.toCommon().filter);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kHighest, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kRightMax, ac.toCommon().swingh);
  ASSERT_EQ(0, ac.toCommon().sleep);
  ASSERT_TRUE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().quiet);
  // Unsupported.
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_TRUE(ac.toCommon().beep);
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
      "Model: 1 (V9014557-A), Power: On, Button: 5 (Power), "
      "Mode: 1 (Cool), Temp: 24C, Fan: 5 (Auto), Turbo: Off, Quiet: Off, "
      "Swing(V): 6 (UNKNOWN), Swing(H): 0 (Middle), Sleep: Off, Health: Off, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off, Lock: Off",
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

  ASSERT_EQ("HAIER_AC160", typeToString(decode_type_t::HAIER_AC160));
  ASSERT_EQ(decode_type_t::HAIER_AC160, strToDecodeType("HAIER_AC160"));
  ASSERT_TRUE(hasACState(decode_type_t::HAIER_AC160));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::HAIER_AC160));
  ASSERT_EQ(kHaierAC160Bits, IRsend::defaultBits(decode_type_t::HAIER_AC160));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::HAIER_AC160));
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
      "Model: 1 (V9014557-A), Power: On, Button: 4 (Fan), "
      "Mode: 4 (Heat), Temp: 24C, Fan: 1 (High), Turbo: Off, Quiet: Off, "
      "Swing(V): 0 (Off), Swing(H): 0 (Middle), Sleep: Off, Health: On, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off, Lock: Off",
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
  const uint8_t timer30m[kHaierAC176StateLength] = {
      0xA6, 0x82, 0x00, 0x40, 0x00, 0xA0, 0x00, 0x00, 0x1E, 0x00, 0x00,
      0x00, 0x10, 0x36, 0xB7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB7};
  const uint8_t timeroff[kHaierAC176StateLength] = {
      0xA6, 0x82, 0x00, 0x00, 0x40, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x10, 0x18, 0xB7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB7};
  // https://docs.google.com/spreadsheets/d/1wdOVS08wgK2pEP7hTZLYMmrQ9FZVmLpZF2HjNQaVxlU/edit#gid=0&range=A65
  const uint8_t timeroffthenon[kHaierAC176StateLength] = {
      0xA6, 0x82, 0x00, 0xA0, 0x40, 0xA0, 0x1E, 0x08, 0x00, 0x00, 0x00,
      0x00, 0x10, 0xDE, 0xB7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB7};
  ac.setRaw(timer30m);
  EXPECT_EQ(kHaierAcYrw02OnTimer, ac.getTimerMode());
  EXPECT_EQ(
      "Model: 1 (V9014557-A), Power: Off, Button: 16 (Timer), "
      "Mode: 0 (Auto), Temp: 24C, Fan: 5 (Auto), Turbo: Off, Quiet: Off, "
      "Swing(V): 2 (Middle), Swing(H): 0 (Middle), Sleep: Off, Health: Off, "
      "Timer Mode: 2 (On), On Timer: 00:30, Off Timer: Off, Lock: Off",
      ac.toString());
  ac.setRaw(timeroff);
  EXPECT_EQ(kHaierAcYrw02NoTimers, ac.getTimerMode());
  EXPECT_EQ(
      "Model: 1 (V9014557-A), Power: On, Button: 16 (Timer), "
      "Mode: 0 (Auto), Temp: 24C, Fan: 5 (Auto), Turbo: Off, Quiet: Off, "
      "Swing(V): 2 (Middle), Swing(H): 0 (Middle), Sleep: Off, Health: Off, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off, Lock: Off",
      ac.toString());
  ac.setRaw(timeroffthenon);
  EXPECT_EQ(
      "Model: 1 (V9014557-A), Power: On, Button: 16 (Timer), "
      "Mode: 0 (Auto), Temp: 24C, Fan: 5 (Auto), Turbo: Off, Quiet: Off, "
      "Swing(V): 2 (Middle), Swing(H): 0 (Middle), Sleep: Off, Health: Off, "
      "Timer Mode: 5 (Off-On), On Timer: 08:00, Off Timer: 00:30, Lock: Off",
      ac.toString());
  ac.setTimerMode(kHaierAcYrw02OnThenOffTimer);
  EXPECT_EQ(
      "Model: 1 (V9014557-A), Power: On, Button: 16 (Timer), "
      "Mode: 0 (Auto), Temp: 24C, Fan: 5 (Auto), Turbo: Off, Quiet: Off, "
      "Swing(V): 2 (Middle), Swing(H): 0 (Middle), Sleep: Off, Health: Off, "
      "Timer Mode: 4 (On-Off), On Timer: 08:00, Off Timer: 00:30, Lock: Off",
      ac.toString());
  ac.setTimerMode(kHaierAcYrw02OffTimer);
  EXPECT_EQ(0, ac.getOnTimer());
  EXPECT_EQ(30, ac.getOffTimer());
  EXPECT_EQ(
      "Model: 1 (V9014557-A), Power: On, Button: 16 (Timer), "
      "Mode: 0 (Auto), Temp: 24C, Fan: 5 (Auto), Turbo: Off, Quiet: Off, "
      "Swing(V): 2 (Middle), Swing(H): 0 (Middle), Sleep: Off, Health: Off, "
      "Timer Mode: 1 (Off), On Timer: Off, Off Timer: 00:30, Lock: Off",
      ac.toString());
  ac.setTimerMode(kHaierAcYrw02NoTimers);
  EXPECT_EQ(0, ac.getOnTimer());
  EXPECT_EQ(0, ac.getOffTimer());
  EXPECT_EQ(
      "Model: 1 (V9014557-A), Power: On, Button: 16 (Timer), "
      "Mode: 0 (Auto), Temp: 24C, Fan: 5 (Auto), Turbo: Off, Quiet: Off, "
      "Swing(V): 2 (Middle), Swing(H): 0 (Middle), Sleep: Off, Health: Off, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off, Lock: Off",
      ac.toString());
}

TEST(TestHaierAC176Class, Models) {
  IRHaierAC176 ac(kGpioUnused);
  ac.begin();
  EXPECT_EQ(haier_ac176_remote_model_t::V9014557_A, ac.getModel());

  ac.setButton(kHaierAcYrw02ButtonTempUp);
  ac.setModel(haier_ac176_remote_model_t::V9014557_B);
  EXPECT_EQ(haier_ac176_remote_model_t::V9014557_B, ac.getModel());
  EXPECT_EQ(kHaierAcYrw02ButtonCFAB, ac.getButton());

  ac.setButton(kHaierAcYrw02ButtonTempDown);
  ac.setModel(haier_ac176_remote_model_t::V9014557_A);
  EXPECT_EQ(haier_ac176_remote_model_t::V9014557_A, ac.getModel());
  EXPECT_EQ(kHaierAcYrw02ButtonCFAB, ac.getButton());

  // Real data.
  const uint8_t setmodelb[kHaierAC176StateLength] = {
      0x59, 0x82, 0x00, 0x00, 0x40, 0x60, 0x00, 0xC0, 0x00, 0x00, 0x00,
      0x00, 0x1A, 0x55, 0xB7, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x77};
  ac.setRaw(setmodelb);
  EXPECT_EQ(haier_ac176_remote_model_t::V9014557_B, ac.getModel());
  EXPECT_EQ(
      "Model: 2 (V9014557-B), Power: On, Button: 26 (Celsius/Fahrenheit), "
      "Mode: 6 (Fan), Temp: 24C, Fan: 3 (Low), Turbo: Off, Quiet: Off, "
      "Swing(V): 2 (Middle), Swing(H): 0 (Middle), Sleep: Off, Health: Off, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off, Lock: Off",
      ac.toString());
}

TEST(TestDecodeHaierAC160, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1804#issue-1236115063
  const uint16_t rawData[325] = {
      3078, 3002,
      3058, 4338,
      590, 1612, 588, 516, 584, 1588, 586, 542, 560, 540, 560, 1590, 584, 1618,
      584, 542, 558, 1592, 584, 542, 582, 1594, 584, 542, 582, 1592, 560, 1642,
      560, 516, 558, 544, 558, 542, 558, 542, 582, 518, 558, 542, 558, 516, 558,
      542, 558, 542, 582, 520, 558, 542, 558, 542, 564, 510, 608, 492, 558, 542,
      582, 520, 558, 542, 558, 544, 582, 492, 608, 1594, 558, 544, 558, 542,
      582, 494, 608, 494, 558, 544, 558, 542, 558, 544, 558, 1616, 582, 1620,
      560, 542, 558, 542, 582, 494, 558, 542, 558, 542, 558, 544, 556, 542, 582,
      520, 580, 494, 582, 520, 582, 520, 558, 542, 558, 542, 582, 520, 582, 492,
      608, 1592, 582, 520, 558, 544, 558, 516, 584, 518, 558, 542, 582, 520,
      580, 520, 582, 520, 580, 492, 584, 516, 556, 544, 582, 518, 580, 520, 580,
      520, 580, 494, 606, 494, 580, 520, 580, 520, 582, 520, 580, 520, 556, 518,
      608, 492, 580, 520, 556, 544, 580, 520, 580, 520, 580, 494, 606, 494, 580,
      520, 580, 520, 580, 520, 580, 520, 580, 494, 606, 496, 578, 522, 580, 520,
      580, 520, 580, 520, 580, 494, 606, 494, 580, 520, 580, 520, 580, 1594,
      608, 494, 580, 1620, 580, 522, 580, 520, 580, 494, 578, 1620, 582, 520,
      580, 1596, 582, 1620, 582, 1594, 608, 1594, 582, 522, 580, 1594, 582,
      1620, 582, 520, 580, 1596, 582, 520, 580, 1620, 582, 494, 606, 496, 580,
      522, 580, 522, 580, 520, 580, 520, 580, 496, 604, 496, 580, 520, 580,
      1620, 582, 1594, 582, 522, 578, 522, 580, 522, 578, 522, 580, 496, 580,
      522, 578, 522, 578, 522, 578, 524, 578, 522, 578, 496, 578, 522, 578, 522,
      578, 546, 554, 522, 578, 522, 578, 520, 556, 546, 554, 546, 554, 546, 556,
      544, 556, 546, 554, 520, 554, 544, 556, 1620, 582, 544, 556, 1594, 580,
      546, 556, 1594, 580};  // UNKNOWN B6B57D85
  const uint8_t expectedState[kHaierAC160StateLength] = {
      0xA6, 0xAC, 0x00, 0x00, 0x40, 0x60, 0x00, 0x20, 0x00, 0x00,
      0x00, 0x00, 0x05, 0x17, 0xB5, 0x00, 0x60, 0x00, 0x00, 0x15};

  irsend.sendRaw(rawData, 325, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(HAIER_AC160, irsend.capture.decode_type);
  ASSERT_EQ(kHaierAC160Bits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Power: On, Button: 5 (Power), Mode: 1 (Cool), Temp: 26C, Fan: 3 (Low), "
      "Turbo: Off, Quiet: Off, Health: Off, Swing(V): 12 (Auto), Sleep: Off, "
      "Clean: Off, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off, Lock: Off, "
      "Heating: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t result, prev;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &result, &prev));
}

// Decoding a message we entirely constructed based solely on a given state.
TEST(TestDecodeHaierAC160, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  const uint8_t expectedState[kHaierAC160StateLength] = {
        0xA6, 0xAC, 0x00, 0x00, 0x40, 0x60, 0x00, 0x20, 0x00, 0x00,
        0x00, 0x00, 0x05, 0x17, 0xB5, 0x00, 0x60, 0x00, 0x00, 0x15};

  irsend.reset();
  irsend.sendHaierAC160(expectedState);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(HAIER_AC160, irsend.capture.decode_type);
  ASSERT_EQ(kHaierAC160Bits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Power: On, Button: 5 (Power), Mode: 1 (Cool), Temp: 26C, Fan: 3 (Low), "
      "Turbo: Off, Quiet: Off, Health: Off, Swing(V): 12 (Auto), Sleep: Off, "
      "Clean: Off, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off, Lock: Off, "
      "Heating: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t result, prev;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &result, &prev));
}

// Tests for the IRHaierAC160 class.

TEST(TestHaierAC160Class, Button) {
  IRHaierAC160 ac(kGpioUnused);
  ac.begin();

  ac.setButton(kHaierAcYrw02ButtonPower);
  EXPECT_EQ(kHaierAcYrw02ButtonPower, ac.getButton());
  ac.setButton(kHaierAcYrw02ButtonMode);
  EXPECT_EQ(kHaierAcYrw02ButtonMode, ac.getButton());
  ac.setButton(kHaierAcYrw02ButtonSleep);
  EXPECT_EQ(kHaierAcYrw02ButtonSleep, ac.getButton());
  ac.setButton(kHaierAcYrw02ButtonFan);

  // Test unexpected values.
  ac.setButton(0xFF);
  EXPECT_EQ(kHaierAcYrw02ButtonFan, ac.getButton());
  ac.setButton(0x10);
  EXPECT_EQ(kHaierAcYrw02ButtonFan, ac.getButton());
}

TEST(TestHaierAC160Class, OperatingMode) {
  IRHaierAC160 ac(kGpioUnused);
  ac.begin();

  ac.setButton(kHaierAcYrw02ButtonPower);
  ac.setMode(kHaierAcYrw02Auto);
  EXPECT_EQ(kHaierAcYrw02Auto, ac.getMode());
  EXPECT_EQ(kHaierAcYrw02ButtonMode, ac.getButton());

  ac.setMode(kHaierAcYrw02Cool);
  EXPECT_EQ(kHaierAcYrw02Cool, ac.getMode());
  EXPECT_FALSE(ac.getAuxHeating());

  ac.setMode(kHaierAcYrw02Heat);
  EXPECT_EQ(kHaierAcYrw02Heat, ac.getMode());
  EXPECT_TRUE(ac.getAuxHeating());

  ac.setMode(kHaierAcYrw02Fan);
  EXPECT_EQ(kHaierAcYrw02Fan, ac.getMode());
  EXPECT_FALSE(ac.getAuxHeating());

  ac.setMode(kHaierAcYrw02Dry);
  EXPECT_EQ(kHaierAcYrw02Dry, ac.getMode());

  ac.setMode(kHaierAcYrw02Auto - 1);
  EXPECT_EQ(kHaierAcYrw02Auto, ac.getMode());

  ac.setMode(kHaierAcYrw02Cool);
  EXPECT_EQ(kHaierAcYrw02Cool, ac.getMode());

  ac.setMode(kHaierAcYrw02Fan + 1);
  EXPECT_EQ(kHaierAcYrw02Auto, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kHaierAcYrw02Auto, ac.getMode());
}

TEST(TestHaierAC160Class, Temperature) {
  IRHaierAC160 ac(kGpioUnused);
  ac.begin();

  ac.setTemp(kHaierAcYrw02MinTempC);
  EXPECT_EQ(kHaierAcYrw02MinTempC, ac.getTemp());

  ac.setButton(kHaierAcYrw02ButtonPower);
  ac.setTemp(kHaierAcYrw02MinTempC + 1);
  EXPECT_EQ(kHaierAcYrw02MinTempC + 1, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, ac.getButton());

  ac.setTemp(kHaierAcYrw02MaxTempC);
  EXPECT_EQ(kHaierAcYrw02MaxTempC, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, ac.getButton());

  ac.setTemp(kHaierAcYrw02MinTempC - 1);
  EXPECT_EQ(kHaierAcYrw02MinTempC, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempDown, ac.getButton());

  ac.setTemp(kHaierAcYrw02MaxTempC + 1);
  EXPECT_EQ(kHaierAcYrw02MaxTempC, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, ac.getButton());

  ac.setTemp(23);
  EXPECT_EQ(23, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempDown, ac.getButton());
  ac.setButton(kHaierAcYrw02ButtonPower);
  ac.setTemp(23);
  EXPECT_EQ(23, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonPower, ac.getButton());

  ac.setTemp(kHaierAcYrw02MinTempF, true);
  EXPECT_EQ(kHaierAcYrw02MinTempF, ac.getTemp());

  ac.setButton(kHaierAcYrw02ButtonPower);
  ac.setTemp(kHaierAcYrw02MinTempF + 1, true);
  EXPECT_EQ(kHaierAcYrw02MinTempF + 1, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, ac.getButton());

  ac.setTemp(kHaierAcYrw02MaxTempF, true);
  EXPECT_EQ(kHaierAcYrw02MaxTempF, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, ac.getButton());

  ac.setTemp(kHaierAcYrw02MinTempF - 1, true);
  EXPECT_EQ(kHaierAcYrw02MinTempF, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempDown, ac.getButton());

  ac.setTemp(kHaierAcYrw02MaxTempF + 1, true);
  EXPECT_EQ(kHaierAcYrw02MaxTempF, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, ac.getButton());

  ac.setTemp(66, true);
  EXPECT_EQ(66, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempDown, ac.getButton());
  ac.setButton(kHaierAcYrw02ButtonPower);
  ac.setTemp(66, true);
  EXPECT_EQ(66, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonPower, ac.getButton());

  // Test specific cases for converting to Fahrenheit
  ac.setTemp(76, true);
  EXPECT_EQ(76, ac.getTemp());
  ac.setTemp(77, true);
  EXPECT_EQ(77, ac.getTemp());
  ac.setTemp(78, true);
  EXPECT_EQ(78, ac.getTemp());

  ac.setTemp(24);
  EXPECT_EQ(kHaierAcYrw02ButtonCFAB, ac.getButton());

  ac.setTemp(0);
  EXPECT_EQ(kHaierAcYrw02MinTempC, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempDown, ac.getButton());

  ac.setTemp(255);
  EXPECT_EQ(kHaierAcMaxTemp, ac.getTemp());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, ac.getButton());
}

TEST(TestHaierAC160Class, CleanMode) {
  IRHaierAC160 ac(kGpioUnused);
  ac.begin();

  ac.setClean(true);
  EXPECT_TRUE(ac.getClean());
  EXPECT_EQ(kHaierAc160ButtonClean, ac.getButton());

  ac.setButton(kHaierAcYrw02ButtonTempUp);
  ac.setClean(false);
  EXPECT_FALSE(ac.getClean());
  EXPECT_EQ(kHaierAc160ButtonClean, ac.getButton());

  ac.setClean(true);
  EXPECT_TRUE(ac.getClean());
  EXPECT_EQ(kHaierAc160ButtonClean, ac.getButton());

  ac.stateReset();
  EXPECT_FALSE(ac.getClean());
  // clean button pressed.
  // https://docs.google.com/spreadsheets/d/1RNJ7esbArS5fy1lmiM-i1PekXSNojCMad4WuuyunsC8/edit#gid=2048081808&range=FR22
  const uint8_t clean_on[kHaierAC160StateLength] = {
      0xA6, 0xAC, 0x00, 0x00, 0x40, 0x60, 0x00, 0x20, 0x00, 0x00,
      0x10, 0x00, 0x19, 0x3B, 0xB5, 0x40, 0x60, 0x00, 0x00, 0x55};
  ac.setRaw(clean_on);
  EXPECT_TRUE(ac.getClean());
  EXPECT_EQ(
      "Power: On, Button: 25 (Clean), Mode: 1 (Cool), Temp: 26C, "
      "Fan: 3 (Low), Turbo: Off, Quiet: Off, Health: Off, Swing(V): 12 (Auto), "
      "Sleep: Off, Clean: On, Timer Mode: 0 (N/A), "
      "On Timer: Off, Off Timer: Off, Lock: Off, Heating: Off",
      ac.toString());
  // No clean set.
  // https://docs.google.com/spreadsheets/d/1RNJ7esbArS5fy1lmiM-i1PekXSNojCMad4WuuyunsC8/edit#gid=2048081808&range=FR4
  const uint8_t clean_off[kHaierAC160StateLength] = {
      0xA6, 0xAC, 0x00, 0x00, 0x40, 0x60, 0x00, 0x20, 0x00, 0x00,
      0x00, 0x00, 0x05, 0x17, 0xB5, 0x00, 0x60, 0x00, 0x00, 0x15};
  ac.setRaw(clean_off);
  EXPECT_FALSE(ac.getClean());
  EXPECT_EQ(
      "Power: On, Button: 5 (Power), Mode: 1 (Cool), Temp: 26C, "
      "Fan: 3 (Low), Turbo: Off, Quiet: Off, Health: Off, Swing(V): 12 (Auto), "
      "Sleep: Off, Clean: Off, Timer Mode: 0 (N/A), "
      "On Timer: Off, Off Timer: Off, Lock: Off, Heating: Off",
      ac.toString());
}

TEST(TestHaierAC160Class, Power) {
  IRHaierAC160 ac(kGpioUnused);
  ac.begin();

  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());
  EXPECT_EQ(kHaierAcYrw02ButtonPower, ac.getButton());

  ac.setButton(kHaierAcYrw02ButtonTempUp);
  ac.setPower(false);
  EXPECT_FALSE(ac.getPower());
  EXPECT_EQ(kHaierAcYrw02ButtonPower, ac.getButton());

  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());
  EXPECT_EQ(kHaierAcYrw02ButtonPower, ac.getButton());

  ac.off();
  EXPECT_FALSE(ac.getPower());
  ac.on();
  EXPECT_TRUE(ac.getPower());
}

TEST(TestHaierAC160Class, SleepMode) {
  IRHaierAC160 ac(kGpioUnused);
  ac.begin();

  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
  EXPECT_EQ(kHaierAcYrw02ButtonSleep, ac.getButton());

  ac.setButton(kHaierAcYrw02ButtonTempUp);
  ac.setSleep(false);
  EXPECT_FALSE(ac.getSleep());
  EXPECT_EQ(kHaierAcYrw02ButtonSleep, ac.getButton());

  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
  EXPECT_EQ(kHaierAcYrw02ButtonSleep, ac.getButton());
}

TEST(TestHaierAC160Class, Health) {
  IRHaierAC160 ac(kGpioUnused);
  ac.begin();

  ac.setHealth(true);
  EXPECT_TRUE(ac.getHealth());
  EXPECT_EQ(kHaierAcYrw02ButtonHealth, ac.getButton());

  ac.setButton(kHaierAcYrw02ButtonTempUp);
  ac.setHealth(false);
  EXPECT_FALSE(ac.getHealth());
  EXPECT_EQ(kHaierAcYrw02ButtonHealth, ac.getButton());

  ac.setHealth(true);
  EXPECT_TRUE(ac.getHealth());
  EXPECT_EQ(kHaierAcYrw02ButtonHealth, ac.getButton());
}

TEST(TestHaierAC160Class, TurboAndQuiet) {
  IRHaierAC160 ac(kGpioUnused);
  ac.begin();

  ac.setMode(kHaierAcYrw02Cool);  // Turbo & Quiet is allowed in this mode.
  ac.setTurbo(false);
  ac.setQuiet(false);
  EXPECT_FALSE(ac.getTurbo());
  EXPECT_FALSE(ac.getQuiet());
  EXPECT_EQ(kHaierAcYrw02ButtonTurbo, ac.getButton());

  ac.setButton(kHaierAcYrw02ButtonTempUp);

  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());
  EXPECT_FALSE(ac.getQuiet());
  EXPECT_EQ(kHaierAcYrw02ButtonTurbo, ac.getButton());

  ac.setQuiet(true);
  EXPECT_FALSE(ac.getTurbo());
  EXPECT_TRUE(ac.getQuiet());
  EXPECT_EQ(kHaierAcYrw02ButtonTurbo, ac.getButton());

  ac.setTurbo(false);
  ac.setQuiet(false);
  EXPECT_FALSE(ac.getTurbo());
  EXPECT_FALSE(ac.getQuiet());
  EXPECT_EQ(kHaierAcYrw02ButtonTurbo, ac.getButton());

  ac.setMode(kHaierAcYrw02Auto);  // Turbo & Quiet is not allowed in this mode.
  EXPECT_FALSE(ac.getTurbo());
  EXPECT_FALSE(ac.getQuiet());
  ac.setTurbo(true);
  EXPECT_FALSE(ac.getTurbo());
  EXPECT_NE(kHaierAcYrw02ButtonTurbo, ac.getButton());
  ac.setQuiet(true);
  EXPECT_FALSE(ac.getQuiet());
  EXPECT_NE(kHaierAcYrw02ButtonTurbo, ac.getButton());
}

TEST(TestHaierAC160Class, Fan) {
  IRHaierAC160 ac(kGpioUnused);
  ac.begin();

  ac.setFan(kHaierAcYrw02FanAuto);
  EXPECT_EQ(kHaierAcYrw02FanAuto, ac.getFan());
  EXPECT_EQ(kHaierAcYrw02ButtonFan, ac.getButton());

  ac.setButton(kHaierAcYrw02ButtonTempUp);

  ac.setFan(kHaierAcYrw02FanLow);
  EXPECT_EQ(kHaierAcYrw02FanLow, ac.getFan());
  EXPECT_EQ(kHaierAcYrw02ButtonFan, ac.getButton());

  ac.setFan(kHaierAcYrw02FanHigh);
  EXPECT_EQ(kHaierAcYrw02FanHigh, ac.getFan());
  EXPECT_EQ(kHaierAcYrw02ButtonFan, ac.getButton());

  ac.setFan(kHaierAcYrw02FanMed);
  EXPECT_EQ(kHaierAcYrw02FanMed, ac.getFan());
  EXPECT_EQ(kHaierAcYrw02ButtonFan, ac.getButton());

  // Test unexpected values.
  ac.setButton(kHaierAcYrw02ButtonTempUp);
  ac.setFan(0x00);
  EXPECT_EQ(kHaierAcYrw02FanMed, ac.getFan());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, ac.getButton());
}

TEST(TestHaierAC160Class, SwingV) {
  IRHaierAC160 ac(kGpioUnused);
  ac.begin();

  ac.setSwingV(kHaierAc160SwingVOff);
  EXPECT_EQ(kHaierAc160SwingVOff, ac.getSwingV());
  EXPECT_EQ(kHaierAcYrw02ButtonSwingV, ac.getButton());

  ac.setButton(kHaierAcYrw02ButtonTempUp);

  ac.setSwingV(kHaierAc160SwingVAuto);
  EXPECT_EQ(kHaierAc160SwingVAuto, ac.getSwingV());
  EXPECT_EQ(kHaierAcYrw02ButtonSwingV, ac.getButton());

  ac.setSwingV(kHaierAc160SwingVTop);
  EXPECT_EQ(kHaierAc160SwingVTop, ac.getSwingV());
  EXPECT_EQ(kHaierAcYrw02ButtonSwingV, ac.getButton());

  ac.setSwingV(kHaierAc160SwingVLow);
  EXPECT_EQ(kHaierAc160SwingVLow, ac.getSwingV());
  EXPECT_EQ(kHaierAcYrw02ButtonSwingV, ac.getButton());

  // Test unexpected values.
  ac.setButton(kHaierAcYrw02ButtonTempUp);
  ac.setSwingV(0xFF);
  EXPECT_EQ(kHaierAc160SwingVLow, ac.getSwingV());
  EXPECT_EQ(kHaierAcYrw02ButtonTempUp, ac.getButton());
}

TEST(TestHaierAC160Class, Light) {
  IRHaierAC160 ac(kGpioUnused);
  ac.begin();

  ac.setLightToggle(true);
  EXPECT_TRUE(ac.getLightToggle());
  EXPECT_EQ(kHaierAc160ButtonLight, ac.getButton());

  ac.setButton(kHaierAcYrw02ButtonTempUp);
  ac.setLightToggle(false);
  EXPECT_FALSE(ac.getLightToggle());
  EXPECT_NE(kHaierAc160ButtonLight, ac.getButton());

  ac.setLightToggle(true);
  EXPECT_TRUE(ac.getLightToggle());
  EXPECT_EQ(kHaierAc160ButtonLight, ac.getButton());

  const uint8_t light_press[kHaierAC160StateLength] = {
      0xA6, 0xAC, 0x00, 0x00, 0x40, 0x60, 0x00, 0x20, 0x00, 0x00,
      0x00, 0x00, 0x15, 0x27, 0xB5, 0x00, 0x60, 0x00, 0x00, 0x15};
  ac.setRaw(light_press);
  EXPECT_TRUE(ac.getLightToggle());
  EXPECT_EQ(kHaierAc160ButtonLight, ac.getButton());
  EXPECT_EQ(
      "Power: On, Button: 21 (Light), Mode: 1 (Cool), Temp: 26C, "
      "Fan: 3 (Low), Turbo: Off, Quiet: Off, Health: Off, Swing(V): 12 (Auto), "
      "Sleep: Off, Clean: Off, Timer Mode: 0 (N/A), "
      "On Timer: Off, Off Timer: Off, Lock: Off, Heating: Off",
      ac.toString());
}

TEST(TestHaierAC160Class, AuxHeating) {
  IRHaierAC160 ac(kGpioUnused);
  ac.begin();

  ac.setAuxHeating(true);
  EXPECT_TRUE(ac.getAuxHeating());
  EXPECT_EQ(kHaierAc160ButtonAuxHeating, ac.getButton());

  ac.setButton(kHaierAcYrw02ButtonTempUp);
  ac.setAuxHeating(false);
  EXPECT_FALSE(ac.getAuxHeating());
  EXPECT_EQ(kHaierAc160ButtonAuxHeating, ac.getButton());

  ac.setAuxHeating(true);
  EXPECT_TRUE(ac.getAuxHeating());
  EXPECT_EQ(kHaierAc160ButtonAuxHeating, ac.getButton());

  // https://docs.google.com/spreadsheets/d/1RNJ7esbArS5fy1lmiM-i1PekXSNojCMad4WuuyunsC8/edit#gid=2048081808&range=A124:W143
  const uint8_t aux_button_off[kHaierAC160StateLength] = {
      0xA6, 0xAC, 0x00, 0x00, 0x40, 0x60, 0x00, 0x80, 0x00, 0x00,
      0x00, 0x00, 0x16, 0x88, 0xB5, 0x00, 0x60, 0x00, 0x00, 0x15};
  ac.setRaw(aux_button_off);
  EXPECT_FALSE(ac.getAuxHeating());
  EXPECT_EQ(kHaierAc160ButtonAuxHeating, ac.getButton());
  EXPECT_EQ(
      "Power: On, Button: 22 (Heating), Mode: 4 (Heat), Temp: 26C, "
      "Fan: 3 (Low), Turbo: Off, Quiet: Off, Health: Off, Swing(V): 12 (Auto), "
      "Sleep: Off, Clean: Off, Timer Mode: 0 (N/A), "
      "On Timer: Off, Off Timer: Off, Lock: Off, Heating: Off",
      ac.toString());
}
