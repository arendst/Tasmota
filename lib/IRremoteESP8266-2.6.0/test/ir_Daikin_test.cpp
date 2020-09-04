// Copyright 2017 David Conran
#include "ir_Daikin.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendDaikin().

// Test sending typical data only.
TEST(TestSendDaikin, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();

  uint8_t daikin_code[kDaikinStateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0xC5, 0x00, 0x00, 0xD7,
      0x11, 0xDA, 0x27, 0xF0, 0x00, 0x00, 0x00, 0x20, 0x11,
      0xDA, 0x27, 0x00, 0x00, 0x41, 0x1E, 0x00, 0xB0, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0xE3};

  irsend.reset();
  irsend.sendDaikin(daikin_code);
  EXPECT_EQ(
      "f38000d50"
      "m428s428m428s428m428s428m428s428m428s428"
      "m428s29428"
      "m3650s1623"
      "m428s1280m428s428m428s428m428s428m428s1280m428s428m428s428m428s428"
      "m428s428m428s1280m428s428m428s1280m428s1280m428s428m428s1280m428s1280"
      "m428s1280m428s1280m428s1280m428s428m428s428m428s1280m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s1280m428s428m428s1280m428s428m428s428m428s428m428s1280m428s1280"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s1280m428s1280m428s1280m428s428m428s1280m428s428m428s1280m428s1280"
      "m428s29428"
      "m3650s1623"
      "m428s1280m428s428m428s428m428s428m428s1280m428s428m428s428m428s428"
      "m428s428m428s1280m428s428m428s1280m428s1280m428s428m428s1280m428s1280"
      "m428s1280m428s1280m428s1280m428s428m428s428m428s1280m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s1280m428s1280m428s1280m428s1280"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s1280m428s428m428s428"
      "m428s29428"
      "m3650s1623"
      "m428s1280m428s428m428s428m428s428m428s1280m428s428m428s428m428s428"
      "m428s428m428s1280m428s428m428s1280m428s1280m428s428m428s1280m428s1280"
      "m428s1280m428s1280m428s1280m428s428m428s428m428s1280m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s1280m428s428m428s428m428s428m428s428m428s428m428s1280m428s428"
      "m428s428m428s1280m428s1280m428s1280m428s1280m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s1280m428s1280m428s428m428s1280"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s1280m428s1280"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s1280m428s1280m428s428m428s428m428s428m428s1280m428s1280m428s1280"
      "m428s29428",
      irsend.outputStr());
}

// Test sending with repeats.
TEST(TestSendDaikin, SendWithRepeats) {
  IRsendTest irsend(0);
  irsend.begin();

  irsend.reset();
  uint8_t daikin_code[kDaikinStateLengthShort] = {
      0x11, 0xDA, 0x27, 0xF0, 0x00, 0x00, 0x00, 0x20, 0x11,
      0xDA, 0x27, 0x00, 0x00, 0x41, 0x1E, 0x00, 0xB0, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0xE3};
  irsend.reset();

  irsend.sendDaikin(daikin_code, kDaikinStateLengthShort, 1);
  EXPECT_EQ(
      "f38000d50"
      "m428s428m428s428m428s428m428s428m428s428"
      "m428s29428m3650s1623"
      "m428s1280m428s428m428s428m428s428m428s1280m428s428m428s428m428s428"
      "m428s428m428s1280m428s428m428s1280m428s1280m428s428m428s1280m428s1280"
      "m428s1280m428s1280m428s1280m428s428m428s428m428s1280m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s1280m428s428m428s1280m428s428m428s428m428s428m428s1280m428s1280"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s1280m428s1280m428s1280m428s428m428s1280m428s428m428s1280m428s1280"
      "m428s29428m3650s1623"
      "m428s1280m428s428m428s428m428s428m428s1280m428s428m428s428m428s428"
      "m428s428m428s1280m428s428m428s1280m428s1280m428s428m428s1280m428s1280"
      "m428s1280m428s1280m428s1280m428s428m428s428m428s1280m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s1280m428s1280m428s1280m428s1280"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s1280m428s428m428s428"
      "m428s29428m3650s1623"
      "m428s1280m428s428m428s428m428s428m428s1280m428s428m428s428m428s428"
      "m428s428m428s1280m428s428m428s1280m428s1280m428s428m428s1280m428s1280"
      "m428s1280m428s1280m428s1280m428s428m428s428m428s1280m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s1280m428s428m428s428m428s428m428s428m428s428m428s1280m428s428"
      "m428s428m428s1280m428s1280m428s1280m428s1280m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s1280m428s1280m428s428m428s1280"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s1280m428s1280"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s1280m428s1280m428s428m428s428m428s428m428s1280m428s1280m428s1280"
      "m428s29428"
      "m428s428m428s428m428s428m428s428m428s428"
      "m428s29428m3650s1623"
      "m428s1280m428s428m428s428m428s428m428s1280m428s428m428s428m428s428"
      "m428s428m428s1280m428s428m428s1280m428s1280m428s428m428s1280m428s1280"
      "m428s1280m428s1280m428s1280m428s428m428s428m428s1280m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s1280m428s428m428s1280m428s428m428s428m428s428m428s1280m428s1280"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s1280m428s1280m428s1280m428s428m428s1280m428s428m428s1280m428s1280"
      "m428s29428m3650s1623"
      "m428s1280m428s428m428s428m428s428m428s1280m428s428m428s428m428s428"
      "m428s428m428s1280m428s428m428s1280m428s1280m428s428m428s1280m428s1280"
      "m428s1280m428s1280m428s1280m428s428m428s428m428s1280m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s1280m428s1280m428s1280m428s1280"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s1280m428s428m428s428"
      "m428s29428m3650s1623"
      "m428s1280m428s428m428s428m428s428m428s1280m428s428m428s428m428s428"
      "m428s428m428s1280m428s428m428s1280m428s1280m428s428m428s1280m428s1280"
      "m428s1280m428s1280m428s1280m428s428m428s428m428s1280m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s1280m428s428m428s428m428s428m428s428m428s428m428s1280m428s428"
      "m428s428m428s1280m428s1280m428s1280m428s1280m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s1280m428s1280m428s428m428s1280"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s1280m428s1280"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s1280m428s1280m428s428m428s428m428s428m428s1280m428s1280m428s1280"
      "m428s29428",
      irsend.outputStr());
}

// Test sending atypical sizes.
TEST(TestSendDaikin, SendUnexpectedSizes) {
  IRsendTest irsend(4);
  irsend.begin();

  uint8_t daikin_short_code[kDaikinStateLengthShort - 1] = {
      0x11, 0xDA, 0x27, 0xF0, 0x00, 0x00, 0x00, 0x20, 0x11,
      0xDA, 0x27, 0x00, 0x00, 0x41, 0x1E, 0x00, 0xB0, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00};

  irsend.reset();
  irsend.sendDaikin(daikin_short_code, kDaikinStateLengthShort - 1);
  ASSERT_EQ("", irsend.outputStr());

  uint8_t daikin_long_code[kDaikinStateLengthShort + 1] = {
      0x11, 0xDA, 0x27, 0xF0, 0x00, 0x00, 0x00, 0x20, 0x11, 0xDA,
      0x27, 0x00, 0x00, 0x41, 0x1E, 0x00, 0xB0, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0xE3, 0x11};
  irsend.reset();
  irsend.sendDaikin(daikin_long_code, kDaikinStateLengthShort + 1);
  ASSERT_EQ(
      "f38000d50"
      "m428s428m428s428m428s428m428s428m428s428"
      "m428s29428m3650s1623"
      "m428s1280m428s428m428s428m428s428m428s1280m428s428m428s428m428s428"
      "m428s428m428s1280m428s428m428s1280m428s1280m428s428m428s1280m428s1280"
      "m428s1280m428s1280m428s1280m428s428m428s428m428s1280m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s1280m428s428m428s1280m428s428m428s428m428s428m428s1280m428s1280"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s1280m428s1280m428s1280m428s428m428s1280m428s428m428s1280m428s1280"
      "m428s29428m3650s1623"
      "m428s1280m428s428m428s428m428s428m428s1280m428s428m428s428m428s428"
      "m428s428m428s1280m428s428m428s1280m428s1280m428s428m428s1280m428s1280"
      "m428s1280m428s1280m428s1280m428s428m428s428m428s1280m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s1280m428s1280m428s1280m428s1280"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s1280m428s428m428s428"
      "m428s29428m3650s1623"
      "m428s1280m428s428m428s428m428s428m428s1280m428s428m428s428m428s428"
      "m428s428m428s1280m428s428m428s1280m428s1280m428s428m428s1280m428s1280"
      "m428s1280m428s1280m428s1280m428s428m428s428m428s1280m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s1280m428s428m428s428m428s428m428s428m428s428m428s1280m428s428"
      "m428s428m428s1280m428s1280m428s1280m428s1280m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s1280m428s1280m428s428m428s1280"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s1280m428s1280"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s1280m428s1280m428s428m428s428m428s428m428s1280m428s1280m428s1280"
      "m428s1280m428s428m428s428m428s428m428s1280m428s428m428s428m428s428"
      "m428s29428",
      irsend.outputStr());
}

// Tests for IRDaikinESP class.

TEST(TestDaikinClass, Power) {
  IRDaikinESP ac(0);
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

TEST(TestDaikinClass, Temperature) {
  IRDaikinESP ac(0);
  ac.begin();

  ac.setTemp(0);
  EXPECT_EQ(kDaikinMinTemp, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kDaikinMaxTemp, ac.getTemp());

  ac.setTemp(kDaikinMinTemp);
  EXPECT_EQ(kDaikinMinTemp, ac.getTemp());

  ac.setTemp(kDaikinMaxTemp);
  EXPECT_EQ(kDaikinMaxTemp, ac.getTemp());

  ac.setTemp(kDaikinMinTemp - 1);
  EXPECT_EQ(kDaikinMinTemp, ac.getTemp());

  ac.setTemp(kDaikinMaxTemp + 1);
  EXPECT_EQ(kDaikinMaxTemp, ac.getTemp());

  ac.setTemp(kDaikinMinTemp + 1);
  EXPECT_EQ(kDaikinMinTemp + 1, ac.getTemp());

  ac.setTemp(21);
  EXPECT_EQ(21, ac.getTemp());

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());

  ac.setTemp(29);
  EXPECT_EQ(29, ac.getTemp());
}

TEST(TestDaikinClass, OperatingMode) {
  IRDaikinESP ac(0);
  ac.begin();

  ac.setMode(kDaikinAuto);
  EXPECT_EQ(kDaikinAuto, ac.getMode());

  ac.setMode(kDaikinCool);
  EXPECT_EQ(kDaikinCool, ac.getMode());

  ac.setMode(kDaikinHeat);
  EXPECT_EQ(kDaikinHeat, ac.getMode());

  ac.setMode(kDaikinDry);
  EXPECT_EQ(kDaikinDry, ac.getMode());

  ac.setMode(kDaikinFan);
  EXPECT_EQ(kDaikinFan, ac.getMode());

  ac.setMode(kDaikinFan + 1);
  EXPECT_EQ(kDaikinAuto, ac.getMode());

  ac.setMode(kDaikinAuto + 1);
  EXPECT_EQ(kDaikinAuto, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kDaikinAuto, ac.getMode());
}

TEST(TestDaikinClass, VaneSwing) {
  IRDaikinESP ac(0);
  ac.begin();

  ac.setSwingHorizontal(true);
  ac.setSwingVertical(false);

  ac.setSwingHorizontal(true);
  EXPECT_TRUE(ac.getSwingHorizontal());
  EXPECT_FALSE(ac.getSwingVertical());

  ac.setSwingVertical(true);
  EXPECT_TRUE(ac.getSwingHorizontal());
  EXPECT_TRUE(ac.getSwingVertical());

  ac.setSwingHorizontal(false);
  EXPECT_FALSE(ac.getSwingHorizontal());
  EXPECT_TRUE(ac.getSwingVertical());

  ac.setSwingVertical(false);
  EXPECT_FALSE(ac.getSwingHorizontal());
  EXPECT_FALSE(ac.getSwingVertical());
}

TEST(TestDaikinClass, QuietMode) {
  IRDaikinESP ac(0);
  ac.begin();

  ac.setQuiet(true);
  EXPECT_TRUE(ac.getQuiet());

  ac.setQuiet(false);
  EXPECT_FALSE(ac.getQuiet());

  ac.setQuiet(true);
  EXPECT_TRUE(ac.getQuiet());

  // Setting Econo mode should NOT change out of quiet mode.
  ac.setEcono(true);
  EXPECT_TRUE(ac.getQuiet());
  ac.setEcono(false);
  EXPECT_TRUE(ac.getQuiet());

  // But setting Powerful mode should exit out of quiet mode.
  ac.setPowerful(true);
  EXPECT_FALSE(ac.getQuiet());
}

TEST(TestDaikinClass, PowerfulMode) {
  IRDaikinESP ac(0);
  ac.begin();

  ac.setPowerful(true);
  EXPECT_TRUE(ac.getPowerful());

  ac.setPowerful(false);
  EXPECT_FALSE(ac.getPowerful());

  ac.setPowerful(true);
  EXPECT_TRUE(ac.getPowerful());

  ac.setQuiet(true);
  EXPECT_FALSE(ac.getPowerful());

  ac.setPowerful(true);
  ac.setEcono(true);
  EXPECT_FALSE(ac.getPowerful());
}

TEST(TestDaikinClass, EconoMode) {
  IRDaikinESP ac(0);
  ac.begin();

  ac.setEcono(true);
  EXPECT_TRUE(ac.getEcono());

  ac.setEcono(false);
  EXPECT_FALSE(ac.getEcono());

  ac.setEcono(true);
  EXPECT_TRUE(ac.getEcono());

  // Setting Quiet mode should NOT change out of Econo mode.
  ac.setQuiet(true);
  EXPECT_TRUE(ac.getEcono());
  ac.setQuiet(false);
  EXPECT_TRUE(ac.getEcono());

  // But setting Powerful mode should exit out of Econo mode.
  ac.setPowerful(true);
  EXPECT_FALSE(ac.getEcono());
}

TEST(TestDaikinClass, FanSpeed) {
  IRDaikinESP ac(0);
  ac.begin();

  // Unexpected value should default to Auto.
  ac.setFan(0);
  EXPECT_EQ(kDaikinFanAuto, ac.getFan());

  // Unexpected value should default to Auto.
  ac.setFan(255);
  EXPECT_EQ(kDaikinFanAuto, ac.getFan());

  ac.setFan(kDaikinFanMax);
  EXPECT_EQ(kDaikinFanMax, ac.getFan());

  // Beyond Max should default to Auto.
  ac.setFan(kDaikinFanMax + 1);
  EXPECT_EQ(kDaikinFanAuto, ac.getFan());

  ac.setFan(kDaikinFanMax - 1);
  EXPECT_EQ(kDaikinFanMax - 1, ac.getFan());

  ac.setFan(kDaikinFanMin);
  EXPECT_EQ(kDaikinFanMin, ac.getFan());

  ac.setFan(kDaikinFanMin + 1);
  EXPECT_EQ(kDaikinFanMin + 1, ac.getFan());

  // Beyond Min should default to Auto.
  ac.setFan(kDaikinFanMin - 1);
  EXPECT_EQ(kDaikinFanAuto, ac.getFan());

  ac.setFan(3);
  EXPECT_EQ(3, ac.getFan());

  ac.setFan(kDaikinFanAuto);
  EXPECT_EQ(kDaikinFanAuto, ac.getFan());

  ac.setFan(kDaikinFanQuiet);
  EXPECT_EQ(kDaikinFanQuiet, ac.getFan());
}

TEST(TestDaikinClass, CurrentTime) {
  IRDaikinESP ac(0);
  ac.begin();

  ac.setCurrentTime(0);  // 00:00
  EXPECT_EQ(0, ac.getCurrentTime());

  ac.setCurrentTime(754);  // 12:34
  EXPECT_EQ(754, ac.getCurrentTime());

  ac.setCurrentTime(1439);  // 23:59
  EXPECT_EQ(1439, ac.getCurrentTime());
}

TEST(TestDaikinClass, OnOffTimers) {
  IRDaikinESP ac(0);
  ac.begin();

  // Both timers turned off.
  ac.disableOnTimer();
  ac.disableOffTimer();
  EXPECT_FALSE(ac.getOnTimerEnabled());
  EXPECT_EQ(kDaikinUnusedTime, ac.getOnTime());
  EXPECT_FALSE(ac.getOffTimerEnabled());
  EXPECT_EQ(kDaikinUnusedTime, ac.getOffTime());

  // Turn on just the On Timer.
  ac.enableOnTimer(123);
  EXPECT_TRUE(ac.getOnTimerEnabled());
  EXPECT_EQ(123, ac.getOnTime());
  EXPECT_FALSE(ac.getOffTimerEnabled());
  EXPECT_EQ(kDaikinUnusedTime, ac.getOffTime());

  // Now turn on the Off Timer.
  ac.enableOffTimer(754);
  EXPECT_TRUE(ac.getOffTimerEnabled());
  EXPECT_EQ(754, ac.getOffTime());
  EXPECT_TRUE(ac.getOnTimerEnabled());
  EXPECT_EQ(123, ac.getOnTime());

  // Turn off the just the On Timer.
  ac.disableOnTimer();
  EXPECT_FALSE(ac.getOnTimerEnabled());
  EXPECT_EQ(kDaikinUnusedTime, ac.getOnTime());
  EXPECT_TRUE(ac.getOffTimerEnabled());
  EXPECT_EQ(754, ac.getOffTime());

  // Now turn off the Off Timer.
  ac.disableOffTimer();
  EXPECT_FALSE(ac.getOffTimerEnabled());
  EXPECT_EQ(kDaikinUnusedTime, ac.getOffTime());
  EXPECT_FALSE(ac.getOnTimerEnabled());
  EXPECT_EQ(kDaikinUnusedTime, ac.getOnTime());

  // Use some canary values around the timers to ensure no accidental
  // bit flips happen. i.e. Neighbouring bytes in the state.
  // (Found some during testing on systems with different endian-ness)
  // Tests here to make sure it never happens again.
  ac.setSwingHorizontal(true);
  ac.setPowerful(true);
  ac.disableOffTimer();
  ac.disableOnTimer();
  ASSERT_TRUE(ac.getSwingHorizontal());
  ASSERT_TRUE(ac.getPowerful());
  ac.enableOnTimer(123);
  ac.enableOffTimer(456);
  ASSERT_TRUE(ac.getSwingHorizontal());
  ASSERT_TRUE(ac.getPowerful());
  ac.disableOffTimer();
  ac.disableOnTimer();
  ASSERT_TRUE(ac.getSwingHorizontal());
  ASSERT_TRUE(ac.getPowerful());

  ac.setSwingHorizontal(false);
  ac.setPowerful(false);
  ac.disableOffTimer();
  ac.disableOnTimer();
  ASSERT_FALSE(ac.getSwingHorizontal());
  ASSERT_FALSE(ac.getPowerful());
  ac.enableOnTimer(123);
  ac.enableOffTimer(456);
  ASSERT_FALSE(ac.getSwingHorizontal());
  ASSERT_FALSE(ac.getPowerful());
  ac.disableOffTimer();
  ac.disableOnTimer();
  ASSERT_FALSE(ac.getSwingHorizontal());
  ASSERT_FALSE(ac.getPowerful());
}

// Test Eye mode.
TEST(TestDaikinClass, EyeSetting) {
  IRDaikinESP ac(0);
  ac.begin();

  // The Eye setting is stored in the same byte as Econo mode.
  // Econo mode tests are there to make sure it isn't harmed and vice-versa.
  ac.setEcono(false);
  ac.setEye(false);
  ASSERT_FALSE(ac.getEye());
  EXPECT_FALSE(ac.getEcono());

  ac.setEye(true);
  ASSERT_TRUE(ac.getEye());
  EXPECT_FALSE(ac.getEcono());

  ac.setEcono(false);
  ASSERT_TRUE(ac.getEye());
  EXPECT_FALSE(ac.getEcono());

  ac.setEcono(true);
  ASSERT_TRUE(ac.getEye());
  EXPECT_TRUE(ac.getEcono());

  ac.setEye(false);
  ASSERT_FALSE(ac.getEye());
  EXPECT_TRUE(ac.getEcono());
}

// Test Mold mode.
TEST(TestDaikinClass, MoldSetting) {
  IRDaikinESP ac(0);
  ac.begin();

  ac.setMold(false);
  ASSERT_FALSE(ac.getMold());

  ac.setMold(true);
  ASSERT_TRUE(ac.getMold());

  ac.setMold(false);
  ASSERT_FALSE(ac.getMold());
}

// Test Comfort mode.
TEST(TestDaikinClass, ComfortSetting) {
  IRDaikinESP ac(0);
  ac.begin();

  ac.setComfort(false);
  ASSERT_FALSE(ac.getComfort());

  ac.setComfort(true);
  ASSERT_TRUE(ac.getComfort());

  ac.setComfort(false);
  ASSERT_FALSE(ac.getComfort());
}

// Test Sensor mode.
TEST(TestDaikinClass, SensorSetting) {
  IRDaikinESP ac(0);
  ac.begin();

  ac.setSensor(false);
  ASSERT_FALSE(ac.getSensor());

  ac.setSensor(true);
  ASSERT_TRUE(ac.getSensor());

  ac.setSensor(false);
  ASSERT_FALSE(ac.getSensor());
}

TEST(TestDaikinClass, RenderTime) {
  EXPECT_EQ("0:00", IRDaikinESP::renderTime(0));
  EXPECT_EQ("0:10", IRDaikinESP::renderTime(10));
  EXPECT_EQ("1:00", IRDaikinESP::renderTime(1 * 60 + 0));
  EXPECT_EQ("23:59", IRDaikinESP::renderTime(23 * 60 + 59));
}

TEST(TestDaikinClass, SetAndGetRaw) {
  IRDaikinESP ac(0);
  uint8_t shortState[kDaikinStateLengthShort] = {
      0x11, 0xDA, 0x27, 0x00, 0x42, 0x00, 0x00, 0x54, 0x11,
      0xDA, 0x27, 0x00, 0x00, 0x49, 0x1E, 0x00, 0xB0, 0x00,
      0x00, 0x06, 0x60, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x4F};
  uint8_t longState[kDaikinStateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0xC5, 0x00, 0x00, 0xD7,
      0x11, 0xDA, 0x27, 0x00, 0x42, 0x00, 0x00, 0x54, 0x11,
      0xDA, 0x27, 0x00, 0x00, 0x49, 0x1E, 0x00, 0xB0, 0x00,
      0x00, 0x06, 0x60, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x4F};
  uint8_t expectedState[kDaikinStateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0xC5, 0x00, 0x00, 0xD7,
      0x11, 0xDA, 0x27, 0x00, 0x42, 0x00, 0x00, 0x54, 0x11,
      0xDA, 0x27, 0x00, 0x00, 0x48, 0x2A, 0x00, 0xB0, 0x00,
      0x00, 0x06, 0x60, 0x00, 0x00, 0xC0, 0x00, 0x02, 0x5C};

  EXPECT_STATE_EQ(longState, ac.getRaw(), kDaikinBits);
  // toggle the power state.
  ac.setPower(!ac.getPower());
  ac.setTemp(21);
  ac.setMold(true);
  EXPECT_STATE_EQ(expectedState, ac.getRaw(), kDaikinBits);
  ac.setRaw(longState);
  EXPECT_STATE_EQ(longState, ac.getRaw(), kDaikinBits);
  ac.setRaw(shortState, kDaikinStateLengthShort);
  EXPECT_STATE_EQ(longState, ac.getRaw(), kDaikinBits);
}

TEST(TestDaikinClass, ChecksumValidation) {
  uint8_t daikin_code[kDaikinStateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0xC5, 0x00, 0x00, 0xD7,
      0x11, 0xDA, 0x27, 0xF0, 0x00, 0x00, 0x00, 0x02, 0x11,
      0xDA, 0x27, 0x00, 0x00, 0x41, 0x1E, 0x00, 0xB0, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0xE1};

  EXPECT_TRUE(IRDaikinESP::validChecksum(daikin_code));
  // Change the array so the checksum is invalid.
  daikin_code[0] ^= 0xFF;
  EXPECT_FALSE(IRDaikinESP::validChecksum(daikin_code));
  // Restore the previous change, and change another byte.
  daikin_code[0] ^= 0xFF;
  daikin_code[4] ^= 0xFF;
  EXPECT_FALSE(IRDaikinESP::validChecksum(daikin_code));
  daikin_code[4] ^= 0xFF;
  // Change something in the 2nd block.
  daikin_code[10] ^= 0xFF;
  EXPECT_FALSE(IRDaikinESP::validChecksum(daikin_code));
  daikin_code[10] ^= 0xFF;
  EXPECT_TRUE(IRDaikinESP::validChecksum(daikin_code));
  // Change something in the 3rd block.
  daikin_code[20] ^= 0xFF;
  EXPECT_FALSE(IRDaikinESP::validChecksum(daikin_code));
  daikin_code[20] ^= 0xFF;
  EXPECT_TRUE(IRDaikinESP::validChecksum(daikin_code));
}

// Test human readable output.
TEST(TestDaikinClass, HumanReadable) {
  IRDaikinESP ac(0);

  EXPECT_EQ(
      "Power: On, Mode: 4 (HEAT), Temp: 15C, Fan: 11 (QUIET), "
      "Powerful: Off, Quiet: Off, Sensor: Off, Eye: Off, Mold: Off, "
      "Comfort: Off, Swing (Horizontal): Off, Swing (Vertical): Off, "
      "Current Time: 0:00, On Time: Off, Off Time: Off",
      ac.toString());
  ac.setMode(kDaikinAuto);
  ac.setTemp(25);
  ac.setFan(kDaikinFanAuto);
  ac.setQuiet(true);
  ac.setSensor(true);
  ac.setEye(true);
  ac.setMold(true);
  ac.setSwingVertical(true);
  ac.setSwingHorizontal(true);
  ac.setCurrentTime(9 * 60 + 15);
  ac.enableOnTimer(8 * 60 + 0);
  ac.enableOffTimer(17 * 60 + 30);
  ac.setComfort(true);
  ac.off();
  EXPECT_EQ(
      "Power: Off, Mode: 0 (AUTO), Temp: 25C, Fan: 10 (AUTO), "
      "Powerful: Off, Quiet: On, Sensor: On, Eye: On, Mold: On, Comfort: On, "
      "Swing (Horizontal): On, Swing (Vertical): On, "
      "Current Time: 9:15, On Time: 8:00, Off Time: 17:30",
      ac.toString());
}

// Test general message construction after tweaking some settings.
TEST(TestDaikinClass, MessageConstuction) {
  IRDaikinESP ac(0);
  IRsendTest irsend(4);
  ac.begin();
  irsend.begin();

  ac.setFan(kDaikinFanMin);
  ac.setMode(kDaikinCool);
  ac.setTemp(27);
  ac.setSwingVertical(false);
  ac.setSwingHorizontal(true);
  ac.setQuiet(false);
  ac.setPower(true);

  // Check everything for kicks.
  EXPECT_EQ(kDaikinFanMin, ac.getFan());
  EXPECT_EQ(kDaikinCool, ac.getMode());
  EXPECT_EQ(27, ac.getTemp());
  EXPECT_FALSE(ac.getSwingVertical());
  EXPECT_TRUE(ac.getSwingHorizontal());
  EXPECT_FALSE(ac.getQuiet());
  EXPECT_TRUE(ac.getPower());

  irsend.reset();
  irsend.sendDaikin(ac.getRaw());
  EXPECT_EQ(
      "f38000d50"
      "m428s428m428s428m428s428m428s428m428s428"
      "m428s29428m3650s1623"
      "m428s1280m428s428m428s428m428s428m428s1280m428s428m428s428m428s428"
      "m428s428m428s1280m428s428m428s1280m428s1280m428s428m428s1280m428s1280"
      "m428s1280m428s1280m428s1280m428s428m428s428m428s1280m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s1280m428s428m428s1280m428s428m428s428m428s428m428s1280m428s1280"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s1280m428s1280m428s1280m428s428m428s1280m428s428m428s1280m428s1280"
      "m428s29428m3650s1623"
      "m428s1280m428s428m428s428m428s428m428s1280m428s428m428s428m428s428"
      "m428s428m428s1280m428s428m428s1280m428s1280m428s428m428s1280m428s1280"
      "m428s1280m428s1280m428s1280m428s428m428s428m428s1280m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s1280m428s428m428s428m428s428m428s428m428s1280m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s1280m428s428m428s1280m428s428m428s1280m428s428"
      "m428s29428m3650s1623"
      "m428s1280m428s428m428s428m428s428m428s1280m428s428m428s428m428s428"
      "m428s428m428s1280m428s428m428s1280m428s1280m428s428m428s1280m428s1280"
      "m428s1280m428s1280m428s1280m428s428m428s428m428s1280m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s1280m428s428m428s428m428s1280m428s1280m428s1280m428s428m428s428"
      "m428s428m428s1280m428s1280m428s428m428s1280m428s1280m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s1280m428s1280m428s428m428s428"
      "m428s1280m428s1280m428s1280m428s1280m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s1280m428s1280m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s1280m428s1280m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s1280m428s1280"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s428m428s428m428s428m428s428m428s428m428s428m428s428"
      "m428s428m428s1280m428s1280m428s428m428s428m428s1280m428s1280m428s1280"
      "m428s29428",
      irsend.outputStr());
}

// Tests for decodeDaikin().

// Test decoding a message captured from a real IR remote.
TEST(TestDecodeDaikin, RealExample) {
  IRDaikinESP ac(0);
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  uint8_t expectedState[kDaikinStateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0xC5, 0x00, 0x00, 0xD7,
      0x11, 0xDA, 0x27, 0x00, 0x42, 0x3A, 0x05, 0x93, 0x11,
      0xDA, 0x27, 0x00, 0x00, 0x3F, 0x3A, 0x00, 0xA0, 0x00,
      0x0A, 0x25, 0x17, 0x01, 0x00, 0xC0, 0x00, 0x00, 0x32};
  uint16_t rawData[583] = {
      416,  446,  416, 446,  416, 446,  418, 446,  416, 446,  416, 25434,
      3436, 1768, 390, 1336, 390, 446,  416, 446,  416, 446,  416, 1336,
      390,  446,  416, 446,  416, 446,  416, 446,  416, 1336, 390, 448,
      416,  1336, 390, 1336, 390, 448,  416, 1336, 390, 1336, 390, 1338,
      388,  1338, 390, 1336, 390, 446,  416, 446,  416, 1336, 390, 446,
      416,  446,  416, 446,  416, 446,  416, 446,  416, 446,  416, 448,
      416,  446,  416, 446,  416, 446,  416, 1336, 390, 446,  416, 1336,
      390,  448,  416, 446,  416, 446,  416, 1336, 390, 1336, 390, 446,
      416,  446,  416, 446,  416, 446,  416, 446,  416, 446,  416, 446,
      416,  446,  416, 446,  416, 448,  416, 446,  416, 446,  416, 446,
      416,  448,  414, 448,  416, 448,  416, 1336, 390, 1336, 390, 1336,
      390,  446,  414, 1336, 390, 448,  414, 1336, 390, 1336, 390, 34878,
      3436, 1768, 390, 1336, 390, 446,  416, 448,  416, 446,  416, 1336,
      390,  446,  416, 448,  416, 446,  416, 446,  416, 1336, 390, 446,
      416,  1336, 390, 1336, 390, 446,  416, 1336, 390, 1336, 390, 1336,
      390,  1336, 390, 1336, 392, 446,  414, 448,  416, 1336, 390, 446,
      416,  446,  416, 446,  416, 446,  414, 448,  416, 446,  416, 448,
      414,  448,  416, 446,  416, 446,  416, 446,  414, 1336, 390, 448,
      416,  446,  416, 446,  416, 448,  416, 1336, 390, 446,  416, 446,
      416,  1336, 390, 446,  416, 1336, 390, 1336, 390, 1336, 390, 446,
      416,  446,  414, 1338, 390, 446,  416, 1336, 390, 446,  416, 446,
      416,  446,  416, 446,  416, 446,  416, 1336, 390, 1336, 390, 446,
      416,  446,  416, 1336, 390, 446,  416, 446,  416, 1336, 390, 34876,
      3436, 1768, 388, 1336, 390, 446,  416, 446,  416, 448,  416, 1336,
      390,  446,  416, 446,  416, 446,  416, 448,  416, 1336, 390, 448,
      414,  1336, 390, 1336, 390, 446,  416, 1336, 388, 1338, 388, 1336,
      390,  1336, 390, 1336, 390, 446,  416, 446,  416, 1336, 390, 446,
      420,  442,  416, 446,  416, 446,  416, 446,  416, 446,  416, 446,
      416,  446,  416, 446,  416, 446,  416, 446,  416, 446,  416, 448,
      416,  446,  416, 448,  416, 446,  416, 448,  416, 446,  416, 1336,
      390,  1336, 390, 1336, 388, 1338, 390, 1336, 390, 1336, 392, 446,
      416,  446,  416, 448,  416, 1334, 390, 446,  416, 1338, 388, 1336,
      390,  1336, 390, 446,  416, 446,  416, 448,  414, 446,  416, 446,
      416,  446,  416, 448,  416, 446,  416, 446,  416, 446,  416, 446,
      416,  446,  416, 446,  416, 446,  416, 446,  416, 1336, 390, 446,
      416,  1336, 390, 446,  414, 448,  416, 446,  416, 446,  416, 446,
      416,  448,  416, 446,  416, 446,  416, 446,  416, 1336, 390, 446,
      416,  1336, 390, 446,  416, 446,  416, 446,  416, 448,  416, 1338,
      390,  444,  418, 1336, 390, 448,  416, 446,  416, 1336, 390, 446,
      416,  446,  416, 1336, 390, 1336, 388, 1336, 390, 446,  416, 1336,
      390,  448,  414, 448,  414, 448,  416, 1334, 390, 446,  416, 446,
      416,  446,  416, 448,  416, 446,  416, 446,  416, 448,  416, 446,
      416,  446,  416, 446,  416, 446,  416, 446,  416, 446,  416, 446,
      416,  446,  416, 446,  416, 446,  416, 446,  416, 446,  416, 446,
      416,  448,  416, 1336, 390, 1336, 390, 446,  416, 446,  416, 446,
      416,  446,  414, 446,  416, 448,  416, 446,  416, 448,  414, 446,
      418,  446,  416, 446,  416, 448,  416, 446,  416, 448,  416, 446,
      416,  448,  416, 446,  416, 1336, 390, 446,  416, 446,  416, 1338,
      390,  1336, 390, 446,  416, 446,  416};  // Captured by @sillyfrog

  irsend.reset();
  irsend.sendRaw(rawData, 583, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(DAIKIN, irsend.capture.decode_type);
  ASSERT_EQ(kDaikinBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: On, Mode: 3 (COOL), Temp: 29C, Fan: 10 (AUTO), Powerful: On, "
      "Quiet: Off, Sensor: Off, Eye: Off, Mold: Off, Comfort: Off, "
      "Swing (Horizontal): Off, Swing (Vertical): Off, "
      "Current Time: 22:18, On Time: 21:30, Off Time: 6:10", ac.toString());
}

// Decoding a message we entirely constructed based solely on a given state.
TEST(TestDecodeDaikin, ShortSyntheticExample) {
  IRDaikinESP ac(0);
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  uint8_t shortState[kDaikinStateLengthShort] = {
      0x11, 0xDA, 0x27, 0x00, 0x42, 0x3A, 0x05, 0x93, 0x11,
      0xDA, 0x27, 0x00, 0x00, 0x3F, 0x3A, 0x00, 0xA0, 0x00,
      0x0A, 0x25, 0x17, 0x01, 0x00, 0xC0, 0x00, 0x00, 0x32};

  uint8_t longState[kDaikinStateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0xC5, 0x00, 0x00, 0xD7,
      0x11, 0xDA, 0x27, 0x00, 0x42, 0x3A, 0x05, 0x93, 0x11,
      0xDA, 0x27, 0x00, 0x00, 0x3F, 0x3A, 0x00, 0xA0, 0x00,
      0x0A, 0x25, 0x17, 0x01, 0x00, 0xC0, 0x00, 0x00, 0x32};
  irsend.reset();
  irsend.sendDaikin(shortState, kDaikinStateLengthShort);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(DAIKIN, irsend.capture.decode_type);
  ASSERT_EQ(kDaikinBits, irsend.capture.bits);
  EXPECT_STATE_EQ(longState, irsend.capture.state, irsend.capture.bits);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: On, Mode: 3 (COOL), Temp: 29C, Fan: 10 (AUTO), Powerful: On, "
      "Quiet: Off, Sensor: Off, Eye: Off, Mold: Off, Comfort: Off, "
      "Swing (Horizontal): Off, Swing (Vertical): Off, "
      "Current Time: 22:18, On Time: 21:30, Off Time: 6:10", ac.toString());
}

// Decoding a message we entirely constructed based solely on a given state.
TEST(TestDecodeDaikin, LongSyntheticExample) {
  IRDaikinESP ac(0);
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  uint8_t expectedState[kDaikinStateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0xC5, 0x00, 0x00, 0xD7,
      0x11, 0xDA, 0x27, 0x00, 0x42, 0x3A, 0x05, 0x93,
      0x11, 0xDA, 0x27, 0x00, 0x00, 0x3F, 0x3A, 0x00, 0xA0, 0x00,
      0x0A, 0x25, 0x17, 0x01, 0x00, 0xC0, 0x00, 0x00, 0x32};

  irsend.reset();
  irsend.sendDaikin(expectedState);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decodeDaikin(&irsend.capture));
  ASSERT_EQ(DAIKIN, irsend.capture.decode_type);
  ASSERT_EQ(kDaikinBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: On, Mode: 3 (COOL), Temp: 29C, Fan: 10 (AUTO), Powerful: On, "
      "Quiet: Off, Sensor: Off, Eye: Off, Mold: Off, Comfort: Off, "
      "Swing (Horizontal): Off, Swing (Vertical): Off, "
      "Current Time: 22:18, On Time: 21:30, Off Time: 6:10", ac.toString());
}

// Test decoding a message captured from a real IR remote.
TEST(TestDecodeDaikin2, RealExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  uint8_t expectedState[kDaikin2StateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0x01, 0x7A, 0xC3, 0x70, 0x28, 0x0C,
      0x80, 0x04, 0xB0, 0x16, 0x24, 0x00, 0x00, 0xBE, 0xD5, 0xF5,
      0x11, 0xDA, 0x27, 0x00, 0x00, 0x08, 0x26, 0x00, 0xA0, 0x00,
      0x00, 0x06, 0x60, 0x00, 0x00, 0xC1, 0x80, 0x60, 0xE7};
  // "Off" Data from https://github.com/markszabo/IRremoteESP8266/issues/582
  uint16_t rawData[633] = {  // Data supplied by @sheppy99
      10024, 25180, 3494, 1732, 436, 1300, 436, 436,  432, 438,  430, 438,
      426,   1306,  430,  442,  430, 438,  428, 440,  430, 440,  430, 1304,
      432,   442,   428,  1308, 424, 1312, 428, 442,  428, 1306, 424, 1314,
      426,   1308,  434,  1306, 426, 1308, 428, 444,  426, 442,  428, 1310,
      428,   442,   424,  444,  426, 442,  426, 444,  424, 444,  426, 444,
      424,   446,   422,  446,  422, 446,  422, 446,  418, 1318, 418, 450,
      420,   448,   420,  448,  422, 448,  420, 450,  420, 448,  420, 450,
      420,   452,   418,  1318, 420, 450,  420, 1318, 420, 1314, 418, 1318,
      424,   1314,  424,  448,  422, 1316, 424, 1312, 426, 446,  422, 448,
      420,   448,   422,  448,  422, 1314, 418, 1320, 416, 452,  420, 448,
      420,   448,   422,  448,  422, 1314, 416, 1320, 422, 1316, 422, 450,
      418,   450,   420,  448,  420, 448,  416, 1320, 418, 452,  418, 1316,
      422,   448,   420,  450,  420, 450,  420, 448,  422, 1314, 418, 1320,
      418,   450,   420,  448,  420, 448,  420, 450,  420, 450,  418, 450,
      418,   450,   420,  450,  418, 452,  416, 452,  420, 450,  418, 1318,
      420,   452,   418,  452,  418, 1322, 416, 452,  416, 452,  418, 452,
      418,   452,   416,  454,  418, 452,  416, 456,  414, 452,  418, 454,
      416,   1320,  410,  1324, 418, 452,  418, 1320, 416, 452,  418, 1320,
      418,   1318,  420,  448,  420, 1316, 420, 450,  420, 450,  418, 450,
      420,   450,   418,  452,  418, 1320, 418, 450,  418, 450,  416, 1322,
      412,   458,   420,  450,  416, 452,  418, 452,  416, 452,  418, 452,
      416,   454,   416,  452,  418, 452,  416, 454,  414, 454,  416, 454,
      416,   454,   414,  456,  414, 454,  414, 456,  412, 454,  416, 456,
      414,   456,   412,  1326, 412, 1320, 412, 1322, 414, 1322, 418, 1320,
      420,   452,   418,  1318, 420, 1316, 422, 450,  420, 1314, 424, 448,
      422,   1314,  422,  448,  422, 1314, 418, 1318, 424, 1316, 422, 448,
      422,   1312,  424,  446,  422, 1314, 420, 1318, 422, 1316, 426, 1310,
      426,   35166, 3500, 1724, 446, 1296, 444, 432,  436, 432,  438, 432,
      436,   1296,  440,  434,  434, 436,  432, 436,  434, 436,  434, 1298,
      438,   438,   432,  1304, 428, 1304, 432, 442,  430, 1302, 430, 1308,
      430,   1306,  434,  1302, 432, 1306, 430, 440,  430, 438,  430, 1308,
      434,   438,   430,  440,  428, 440,  430, 440,  428, 442,  426, 444,
      428,   442,   426,  444,  426, 442,  426, 444,  424, 446,  422, 446,
      424,   446,   424,  446,  422, 446,  424, 448,  420, 448,  422, 446,
      422,   448,   422,  450,  420, 450,  414, 1320, 420, 450,  418, 450,
      418,   448,   420,  450,  418, 452,  418, 1320, 418, 1316, 422, 450,
      418,   452,   418,  1320, 420, 448,  418, 450,  420, 450,  418, 452,
      416,   452,   418,  450,  418, 452,  416, 452,  418, 452,  416, 454,
      416,   452,   416,  454,  416, 454,  414, 456,  416, 454,  414, 1322,
      416,   454,   416,  1320, 418, 452,  416, 454,  414, 454,  416, 454,
      414,   454,   414,  454,  414, 456,  414, 456,  412, 456,  414, 456,
      414,   456,   412,  456,  414, 458,  406, 464,  410, 458,  412, 458,
      410,   460,   410,  1326, 412, 1324, 414, 456,  412, 458,  412, 456,
      414,   456,   412,  458,  410, 458,  414, 458,  410, 458,  408, 460,
      410,   470,   400,  1324, 408, 1328, 410, 458,  410, 460,  414, 456,
      410,   456,   414,  458,  412, 460,  410, 458,  412, 458,  412, 460,
      408,   460,   410,  460,  408, 472,  396, 462,  408, 470,  402, 470,
      396,   472,   400,  470,  398, 1326, 412, 460,  408, 472,  396, 472,
      400,   470,   400,  472,  396, 1328, 410, 1324, 414, 458,  410, 458,
      410,   458,   412,  458,  412, 460,  408, 460,  410, 460,  410, 1324,
      414,   458,   410,  460,  408, 460,  410, 458,  410, 460,  410, 1326,
      412,   1322,  416,  456,  412, 1322, 412, 1326, 416, 1322, 418, 452,
      416,   454,   412,  1324, 418, 1320, 420, 1316, 420};
  irsend.reset();
  irsend.sendRaw(rawData, 633, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(DAIKIN2, irsend.capture.decode_type);
  ASSERT_EQ(kDaikin2Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
}

// Decoding a message we entirely constructed based solely on a given state.
TEST(TestDecodeDaikin2, SyntheticExample) {
  IRDaikin2 ac(0);
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  uint8_t expectedState[kDaikin2StateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0x01, 0x7A, 0xC3, 0x70, 0x28, 0x0C,
      0x80, 0x04, 0xB0, 0x16, 0x24, 0x00, 0x00, 0xBE, 0xD5, 0xF5,
      0x11, 0xDA, 0x27, 0x00, 0x00, 0x08, 0x26, 0x00, 0xA0, 0x00,
      0x00, 0x06, 0x60, 0x00, 0x00, 0xC1, 0x80, 0x60, 0xE7};

  irsend.reset();
  irsend.sendDaikin2(expectedState);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(DAIKIN2, irsend.capture.decode_type);
  ASSERT_EQ(kDaikin2Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: Off, Mode: 0 (AUTO), Temp: 19C, Fan: 10 (Auto), "
      "Swing (V): 5, Swing (H): 190 (Auto), "
      "Clock: 14:50, On Time: Off, Off Time: Off, Sleep Time: Off, "
      "Beep: 1 (Quiet), Light: 3 (Off), Mold: On, Clean: On, Fresh Air: Off, "
      "Eye: Off, Eye Auto: Off, Quiet: Off, Powerful: Off, Purify: Off, "
      "Econo: Off",
      ac.toString());
}

TEST(TestDaikin2Class, CurrentTime) {
  IRDaikin2 ac(0);
  ac.begin();

  ac.setCurrentTime(0);  // 00:00
  EXPECT_EQ(0, ac.getCurrentTime());

  ac.setCurrentTime(754);  // 12:34
  EXPECT_EQ(754, ac.getCurrentTime());

  ac.setCurrentTime(1439);  // 23:59
  EXPECT_EQ(1439, ac.getCurrentTime());
}

TEST(TestDaikin2Class, OnOffTimers) {
  IRDaikin2 ac(0);
  ac.begin();

  // Both timers turned off.
  ac.disableOnTimer();
  ac.disableOffTimer();
  EXPECT_FALSE(ac.getOnTimerEnabled());
  EXPECT_EQ(kDaikinUnusedTime, ac.getOnTime());
  EXPECT_FALSE(ac.getOffTimerEnabled());
  EXPECT_EQ(kDaikinUnusedTime, ac.getOffTime());

  // Turn on just the On Timer.
  ac.enableOnTimer(123);
  EXPECT_TRUE(ac.getOnTimerEnabled());
  EXPECT_EQ(123, ac.getOnTime());
  EXPECT_FALSE(ac.getOffTimerEnabled());
  EXPECT_EQ(kDaikinUnusedTime, ac.getOffTime());

  // Now turn on the Off Timer.
  ac.enableOffTimer(754);
  EXPECT_TRUE(ac.getOffTimerEnabled());
  EXPECT_EQ(754, ac.getOffTime());
  EXPECT_TRUE(ac.getOnTimerEnabled());
  EXPECT_EQ(123, ac.getOnTime());

  // Turn off the just the On Timer.
  ac.disableOnTimer();
  EXPECT_FALSE(ac.getOnTimerEnabled());
  EXPECT_EQ(kDaikinUnusedTime, ac.getOnTime());
  EXPECT_TRUE(ac.getOffTimerEnabled());
  EXPECT_EQ(754, ac.getOffTime());

  // Now turn off the Off Timer.
  ac.disableOffTimer();
  EXPECT_FALSE(ac.getOffTimerEnabled());
  EXPECT_EQ(kDaikinUnusedTime, ac.getOffTime());
  EXPECT_FALSE(ac.getOnTimerEnabled());
  EXPECT_EQ(kDaikinUnusedTime, ac.getOnTime());
}

TEST(TestDaikin2Class, LightAndBeep) {
  IRDaikin2 ac(0);
  ac.begin();

  ac.setLight(kDaikinLightOff);
  EXPECT_EQ(kDaikinLightOff, ac.getLight());
  ac.setBeep(kDaikinBeepOff);
  EXPECT_EQ(kDaikinBeepOff, ac.getBeep());
  ac.setLight(kDaikinLightBright);
  EXPECT_EQ(kDaikinLightBright, ac.getLight());
  EXPECT_EQ(kDaikinBeepOff, ac.getBeep());
  ac.setBeep(kDaikinBeepLoud);
  EXPECT_EQ(kDaikinBeepLoud, ac.getBeep());
  EXPECT_EQ(kDaikinLightBright, ac.getLight());
}

TEST(TestDaikin2Class, FanSpeed) {
  IRDaikin2 ac(0);
  ac.begin();

  // Unexpected value should default to Auto.
  ac.setFan(0);
  EXPECT_EQ(kDaikinFanAuto, ac.getFan());

  // Unexpected value should default to Auto.
  ac.setFan(255);
  EXPECT_EQ(kDaikinFanAuto, ac.getFan());

  ac.setFan(kDaikinFanMax);
  EXPECT_EQ(kDaikinFanMax, ac.getFan());

  // Beyond Max should default to Auto.
  ac.setFan(kDaikinFanMax + 1);
  EXPECT_EQ(kDaikinFanAuto, ac.getFan());

  ac.setFan(kDaikinFanMax - 1);
  EXPECT_EQ(kDaikinFanMax - 1, ac.getFan());

  ac.setFan(kDaikinFanMin);
  EXPECT_EQ(kDaikinFanMin, ac.getFan());

  ac.setFan(kDaikinFanMin + 1);
  EXPECT_EQ(kDaikinFanMin + 1, ac.getFan());

  // Beyond Min should default to Auto.
  ac.setFan(kDaikinFanMin - 1);
  EXPECT_EQ(kDaikinFanAuto, ac.getFan());

  ac.setFan(3);
  EXPECT_EQ(3, ac.getFan());

  ac.setFan(kDaikinFanAuto);
  EXPECT_EQ(kDaikinFanAuto, ac.getFan());

  ac.setFan(kDaikinFanQuiet);
  EXPECT_EQ(kDaikinFanQuiet, ac.getFan());
}

// Test Mold mode.
TEST(TestDaikin2Class, MoldSetting) {
  IRDaikin2 ac(0);
  ac.begin();

  ac.setMold(false);
  ASSERT_FALSE(ac.getMold());

  ac.setMold(true);
  ASSERT_TRUE(ac.getMold());

  ac.setMold(false);
  ASSERT_FALSE(ac.getMold());
}

// Test Auto Clean setting.
TEST(TestDaikin2Class, CleanSetting) {
  IRDaikin2 ac(0);
  ac.begin();

  ac.setClean(false);
  ASSERT_FALSE(ac.getClean());

  ac.setClean(true);
  ASSERT_TRUE(ac.getClean());

  ac.setClean(false);
  ASSERT_FALSE(ac.getClean());
}


TEST(TestDaikin2Class, Temperature) {
  IRDaikin2 ac(0);
  ac.begin();

  ac.setMode(kDaikinAuto);
  ac.setTemp(0);
  EXPECT_EQ(kDaikinMinTemp, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kDaikinMaxTemp, ac.getTemp());

  ac.setTemp(kDaikinMinTemp);
  EXPECT_EQ(kDaikinMinTemp, ac.getTemp());

  ac.setTemp(kDaikinMaxTemp);
  EXPECT_EQ(kDaikinMaxTemp, ac.getTemp());

  ac.setTemp(kDaikinMinTemp - 1);
  EXPECT_EQ(kDaikinMinTemp, ac.getTemp());

  ac.setTemp(kDaikinMaxTemp + 1);
  EXPECT_EQ(kDaikinMaxTemp, ac.getTemp());

  ac.setTemp(kDaikinMinTemp + 1);
  EXPECT_EQ(kDaikinMinTemp + 1, ac.getTemp());

  // Now try it with Cool mode, which should set the temp to kDaikin2MinCoolTemp
  ASSERT_TRUE(kDaikinMinTemp + 1 < kDaikin2MinCoolTemp);
  ac.setMode(kDaikinCool);
  EXPECT_EQ(kDaikin2MinCoolTemp, ac.getTemp());
  ac.setTemp(kDaikin2MinCoolTemp - 1);
  EXPECT_EQ(kDaikin2MinCoolTemp, ac.getTemp());
  ac.setTemp(kDaikin2MinCoolTemp + 1);
  EXPECT_EQ(kDaikin2MinCoolTemp + 1, ac.getTemp());
  // Should be released from that requirement in other modes.
  ac.setMode(kDaikinAuto);
  ac.setTemp(kDaikin2MinCoolTemp - 1);
  EXPECT_EQ(kDaikin2MinCoolTemp - 1, ac.getTemp());

  ac.setTemp(21);
  EXPECT_EQ(21, ac.getTemp());

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());

  ac.setTemp(29);
  EXPECT_EQ(29, ac.getTemp());
}

// Test Fresh Air settings.
TEST(TestDaikin2Class, FreshAirSettings) {
  IRDaikin2 ac(0);
  ac.begin();

  ac.setFreshAir(false);
  ac.setFreshAirHigh(false);
  ASSERT_FALSE(ac.getFreshAir());
  ASSERT_FALSE(ac.getFreshAirHigh());

  ac.setFreshAir(true);
  ASSERT_TRUE(ac.getFreshAir());
  ASSERT_FALSE(ac.getFreshAirHigh());

  ac.setFreshAirHigh(true);
  ASSERT_TRUE(ac.getFreshAir());
  ASSERT_TRUE(ac.getFreshAirHigh());

  ac.setFreshAir(false);
  ASSERT_FALSE(ac.getFreshAir());
  ASSERT_TRUE(ac.getFreshAirHigh());

  ac.setFreshAirHigh(false);
  ASSERT_FALSE(ac.getFreshAir());
  ASSERT_FALSE(ac.getFreshAirHigh());
}

// Test Eye mode.
TEST(TestDaikin2Class, EyeSetting) {
  IRDaikin2 ac(0);
  ac.begin();

  ac.setEye(false);
  ASSERT_FALSE(ac.getEye());
  ac.setEye(true);
  ASSERT_TRUE(ac.getEye());
  ac.setEye(false);
  ASSERT_FALSE(ac.getEye());
}

// Test Econo setting.
TEST(TestDaikin2Class, EconoSetting) {
  IRDaikin2 ac(0);
  ac.begin();

  ac.setEcono(false);
  ASSERT_FALSE(ac.getEcono());
  ac.setEcono(true);
  ASSERT_TRUE(ac.getEcono());
  ac.setEcono(false);
  ASSERT_FALSE(ac.getEcono());
}

TEST(TestDaikin2Class, SleepTimer) {
  IRDaikin2 ac(0);
  ac.begin();

  // NOTE: On & Sleep timer share the same time location.

  // Both timers turned off.
  ac.disableOnTimer();
  ac.disableSleepTimer();
  EXPECT_FALSE(ac.getOnTimerEnabled());
  EXPECT_EQ(kDaikinUnusedTime, ac.getOnTime());
  EXPECT_FALSE(ac.getSleepTimerEnabled());
  EXPECT_EQ(kDaikinUnusedTime, ac.getSleepTime());

  // Turn on just the On Timer.
  ac.enableOnTimer(123);
  EXPECT_TRUE(ac.getOnTimerEnabled());
  EXPECT_EQ(123, ac.getOnTime());
  EXPECT_FALSE(ac.getSleepTimerEnabled());
  EXPECT_EQ(123, ac.getSleepTime());

  // Now turn on the Sleep Timer. This shoud disable the On Timer.
  ac.enableSleepTimer(754);
  EXPECT_TRUE(ac.getSleepTimerEnabled());
  EXPECT_EQ(754, ac.getSleepTime());
  EXPECT_FALSE(ac.getOnTimerEnabled());
  EXPECT_EQ(754, ac.getOnTime());

  // Turn off the just the On Timer.
  ac.disableOnTimer();
  EXPECT_FALSE(ac.getOnTimerEnabled());
  EXPECT_EQ(kDaikinUnusedTime, ac.getOnTime());
  EXPECT_FALSE(ac.getSleepTimerEnabled());
  EXPECT_EQ(kDaikinUnusedTime, ac.getSleepTime());

  // Now turn on the On Timer and turn off the Sleep Timer.
  // Both should be off afterwards.
  ac.enableOnTimer(123);
  ac.disableSleepTimer();
  EXPECT_FALSE(ac.getSleepTimerEnabled());
  EXPECT_EQ(kDaikinUnusedTime, ac.getSleepTime());
  EXPECT_FALSE(ac.getOnTimerEnabled());
  EXPECT_EQ(kDaikinUnusedTime, ac.getOnTime());
}

// Test Vertical Swing.
TEST(TestDaikin2Class, Swing) {
  IRDaikin2 ac(0);
  ac.begin();

  // Vertical
  ac.setSwingVertical(1);
  ASSERT_EQ(1, ac.getSwingVertical());
  ac.setSwingVertical(3);
  ASSERT_EQ(3, ac.getSwingVertical());
  ac.setSwingVertical(6);
  ASSERT_EQ(6, ac.getSwingVertical());
  ac.setSwingVertical(kDaikin2SwingVBreeze);
  ASSERT_EQ(kDaikin2SwingVBreeze, ac.getSwingVertical());
  ac.setSwingVertical(kDaikin2SwingVCirculate);
  ASSERT_EQ(kDaikin2SwingVCirculate, ac.getSwingVertical());
  ac.setSwingVertical(kDaikin2SwingVAuto);
  ASSERT_EQ(kDaikin2SwingVAuto, ac.getSwingVertical());
  ac.setSwingVertical(0);
  ASSERT_EQ(kDaikin2SwingVAuto, ac.getSwingVertical());
  ac.setSwingVertical(7);
  ASSERT_EQ(kDaikin2SwingVAuto, ac.getSwingVertical());
  ac.setSwingVertical(255);
  ASSERT_EQ(kDaikin2SwingVAuto, ac.getSwingVertical());

  // Horizontal
  ac.setSwingHorizontal(kDaikin2SwingHAuto);
  ASSERT_EQ(kDaikin2SwingHAuto, ac.getSwingHorizontal());
  ac.setSwingHorizontal(kDaikin2SwingHSwing);
  ASSERT_EQ(kDaikin2SwingHSwing, ac.getSwingHorizontal());

  ac.setSwingHorizontal(0);
  ASSERT_EQ(0, ac.getSwingHorizontal());
  ac.setSwingHorizontal(255);
  ASSERT_EQ(255, ac.getSwingHorizontal());
}

TEST(TestDaikin2Class, QuietMode) {
  IRDaikin2 ac(0);
  ac.begin();

  ac.setQuiet(true);
  EXPECT_TRUE(ac.getQuiet());

  ac.setQuiet(false);
  EXPECT_FALSE(ac.getQuiet());

  ac.setQuiet(true);
  EXPECT_TRUE(ac.getQuiet());

  // But setting Powerful mode should exit out of quiet mode.
  ac.setPowerful(true);
  EXPECT_FALSE(ac.getQuiet());
}

TEST(TestDaikin2Class, PowerfulMode) {
  IRDaikin2 ac(0);
  ac.begin();

  ac.setPowerful(true);
  EXPECT_TRUE(ac.getPowerful());

  ac.setPowerful(false);
  EXPECT_FALSE(ac.getPowerful());

  ac.setPowerful(true);
  EXPECT_TRUE(ac.getPowerful());

  ac.setQuiet(true);
  EXPECT_FALSE(ac.getPowerful());
}

// Test Purify mode.
TEST(TestDaikin2Class, PurifySetting) {
  IRDaikin2 ac(0);
  ac.begin();

  ac.setPurify(false);
  ASSERT_FALSE(ac.getPurify());
  ac.setPurify(true);
  ASSERT_TRUE(ac.getPurify());
  ac.setPurify(false);
  ASSERT_FALSE(ac.getPurify());
}

TEST(TestDaikin2Class, HumanReadable) {
  IRDaikin2 ac(0);
  ac.begin();
  ac.setPower(true);
  ac.setMode(kDaikinCool);
  ac.setTemp(21);
  ac.setFan(kDaikinFanMax);
  ac.setSwingVertical(kDaikin2SwingVAuto);
  ac.setSwingHorizontal(kDaikin2SwingHSwing);
  ac.setCurrentTime(12 * 60 + 34);  // 12:34
  ac.disableOnTimer();
  ac.enableOffTimer(20 * 60);  // 20:00
  ac.enableSleepTimer(4 * 60);  // 4:00
  ac.setBeep(kDaikinBeepLoud);
  ac.setLight(kDaikinLightDim);
  ac.setMold(true);
  ac.setClean(false);
  ac.setFreshAir(true);
  ac.setEye(true);
  ac.setEyeAuto(true);
  ac.setQuiet(false);
  ac.setPowerful(true);
  ac.setPurify(true);
  ac.setEcono(false);
  EXPECT_EQ(
      "Power: On, Mode: 3 (COOL), Temp: 21C, Fan: 5 (Max), "
      "Swing (V): 14 (Auto), Swing (H): 191 (Swing), Clock: 12:34, "
      "On Time: Off, Off Time: 20:00, Sleep Time: 4:00, Beep: 2 (Loud), "
      "Light: 2 (Dim), Mold: On, Clean: Off, Fresh Air: On, Eye: On, "
      "Eye Auto: On, Quiet: Off, Powerful: On, Purify: On, Econo: Off",
      ac.toString());
  ac.setQuiet(true);
  ac.setMode(kDaikinHeat);
  ac.setBeep(kDaikinBeepQuiet);
  ac.setLight(kDaikinLightBright);
  ac.setTemp(32);
  ac.setFan(kDaikinFanMin);
  ac.setCurrentTime(23 * 60 + 45);  // 23:45
  ac.enableOnTimer(9 * 60 + 11);  // 9:11
  EXPECT_EQ(
      "Power: On, Mode: 4 (HEAT), Temp: 32C, Fan: 1 (Min), "
      "Swing (V): 14 (Auto), Swing (H): 191 (Swing), Clock: 23:45, "
      "On Time: 9:11, Off Time: 20:00, Sleep Time: Off, Beep: 1 (Quiet), "
      "Light: 1 (Bright), Mold: On, Clean: Off, Fresh Air: On, Eye: On, "
      "Eye Auto: On, Quiet: On, Powerful: Off, Purify: On, Econo: Off",
      ac.toString());
}

// See if we can construct a known state.
TEST(TestDaikin2Class, KnownConstruction) {
  IRDaikin2 ac(0);

  uint8_t expectedState[kDaikin2StateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0x01, 0x7A, 0xC3, 0x70, 0x28, 0x0C,
      0x80, 0x04, 0xB0, 0x16, 0x24, 0x00, 0x00, 0xBE, 0xD5, 0xF5,
      0x11, 0xDA, 0x27, 0x00, 0x00, 0x08, 0x26, 0x00, 0xA0, 0x00,
      0x00, 0x06, 0x60, 0x00, 0x00, 0xC1, 0x80, 0x60, 0xE7};

  ac.begin();
  ac.setPower(false);
  ac.setMode(kDaikinAuto);
  ac.setTemp(19);
  ac.setFan(kDaikinFanAuto);
  ac.setSwingVertical(5);
  ac.setSwingHorizontal(kDaikin2SwingHAuto);
  ac.setCurrentTime(14 * 60 + 50);  // 14:50
  ac.disableOnTimer();
  ac.disableOffTimer();
  ac.disableSleepTimer();
  ac.setBeep(kDaikinBeepQuiet);
  ac.setLight(kDaikinLightOff);
  ac.setMold(true);
  ac.setClean(true);
  ac.setFreshAir(false);
  ac.setEye(false);
  ac.setEyeAuto(false);
  ac.setQuiet(false);
  ac.setPowerful(false);
  ac.setPurify(false);
  ac.setEcono(false);
  EXPECT_EQ(
      "Power: Off, Mode: 0 (AUTO), Temp: 19C, Fan: 10 (Auto), "
      "Swing (V): 5, Swing (H): 190 (Auto), "
      "Clock: 14:50, On Time: Off, Off Time: Off, Sleep Time: Off, "
      "Beep: 1 (Quiet), Light: 3 (Off), Mold: On, Clean: On, Fresh Air: Off, "
      "Eye: Off, Eye Auto: Off, Quiet: Off, Powerful: Off, Purify: Off, "
      "Econo: Off",
      ac.toString());
  EXPECT_STATE_EQ(expectedState, ac.getRaw(), kDaikin2Bits);
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("DAIKIN", typeToString(decode_type_t::DAIKIN));
  ASSERT_EQ(decode_type_t::DAIKIN, strToDecodeType("DAIKIN"));
  ASSERT_TRUE(hasACState(decode_type_t::DAIKIN));

  ASSERT_EQ("DAIKIN2", typeToString(decode_type_t::DAIKIN2));
  ASSERT_EQ(decode_type_t::DAIKIN2, strToDecodeType("DAIKIN2"));
  ASSERT_TRUE(hasACState(decode_type_t::DAIKIN2));

  ASSERT_EQ("DAIKIN216", typeToString(decode_type_t::DAIKIN216));
  ASSERT_EQ(decode_type_t::DAIKIN216, strToDecodeType("DAIKIN216"));
  ASSERT_TRUE(hasACState(decode_type_t::DAIKIN216));
}

// https://github.com/markszabo/IRremoteESP8266/issues/582#issuecomment-453863879
TEST(TestDecodeDaikin2, Issue582DeepDecodeExample) {
  IRDaikin2 ac(0);

  const uint8_t state[kDaikin2StateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0x01, 0x30, 0x42, 0xF0, 0x28, 0x0C,
      0x80, 0x04, 0xB0, 0x16, 0x24, 0x00, 0x00, 0xBE, 0xCE, 0xA3,
      0x11, 0xDA, 0x27, 0x00, 0x00, 0x09, 0x26, 0x00, 0xA0, 0x00,
      0x00, 0x06, 0x60, 0x00, 0x00, 0xC1, 0x92, 0x60, 0xFA};

  ac.setRaw(state);
  ASSERT_TRUE(ac.getMold());
  ASSERT_TRUE(ac.getEye());
  ASSERT_TRUE(ac.getPurify());
  EXPECT_EQ(
      "Power: On, Mode: 0 (AUTO), Temp: 19C, Fan: 10 (Auto), "
      "Swing (V): 14 (Auto), Swing (H): 190 (Auto), Clock: 9:20, On Time: Off, "
      "Off Time: Off, Sleep Time: Off, Beep: 3 (Off), Light: 3 (Off), "
      "Mold: On, Clean: On, Fresh Air: Off, Eye: On, Eye Auto: Off, "
      "Quiet: Off, Powerful: Off, Purify: On, Econo: Off",
      ac.toString());
}

// https://docs.google.com/spreadsheets/d/1f8EGfIbBUo2B-CzUFdrgKQprWakoYNKM80IKZN4KXQE/edit?ts=5c317775#gid=1023395743
TEST(TestDecodeDaikin2, Issue582PowerfulEconoFix) {
  IRDaikin2 ac(0);

  const uint8_t PowerfulOn[39] = {
      0x11, 0xDA, 0x27, 0x00, 0x01, 0x3A, 0x43, 0xF0, 0x28, 0x0C,
      0x80, 0x04, 0xB0, 0x16, 0x24, 0x00, 0x00, 0xBE, 0xCE, 0xAE,
      0x11, 0xDA, 0x27, 0x00, 0x00, 0x39, 0x28, 0x00, 0xA0, 0x00,
      0x00, 0x06, 0x60, 0x01, 0x00, 0xC1, 0x90, 0x60, 0x2B};
  const uint8_t PowerfulOff[39] = {
      0x11, 0xDA, 0x27, 0x00, 0x01, 0x3A, 0x43, 0xF0, 0x28, 0x0C,
      0x80, 0x04, 0xB0, 0x16, 0x24, 0x00, 0x00, 0xBE, 0xCE, 0xAE,
      0x11, 0xDA, 0x27, 0x00, 0x00, 0x39, 0x28, 0x00, 0xA0, 0x00,
      0x00, 0x06, 0x60, 0x00, 0x00, 0xC1, 0x90, 0x60, 0x2A};
  ac.setRaw(PowerfulOn);
  ASSERT_TRUE(ac.getPowerful());
  EXPECT_EQ(
      "Power: On, Mode: 3 (COOL), Temp: 20C, Fan: 10 (Auto), "
      "Swing (V): 14 (Auto), Swing (H): 190 (Auto), Clock: 13:46, "
      "On Time: Off, Off Time: Off, Sleep Time: Off, Beep: 3 (Off), "
      "Light: 3 (Off), Mold: On, Clean: On, Fresh Air: Off, Eye: Off, "
      "Eye Auto: Off, Quiet: Off, Powerful: On, Purify: On, Econo: Off",
      ac.toString());
  ac.setRaw(PowerfulOff);
  ASSERT_FALSE(ac.getPowerful());
  EXPECT_EQ(
      "Power: On, Mode: 3 (COOL), Temp: 20C, Fan: 10 (Auto), "
      "Swing (V): 14 (Auto), Swing (H): 190 (Auto), Clock: 13:46, "
      "On Time: Off, Off Time: Off, Sleep Time: Off, Beep: 3 (Off), "
      "Light: 3 (Off), Mold: On, Clean: On, Fresh Air: Off, Eye: Off, "
      "Eye Auto: Off, Quiet: Off, Powerful: Off, Purify: On, Econo: Off",
      ac.toString());

  const uint8_t EconoOn[39] = {
      0x11, 0xDA, 0x27, 0x00, 0x01, 0x3B, 0x43, 0xF0, 0x28, 0x0C,
      0x80, 0x04, 0xB0, 0x16, 0x24, 0x00, 0x00, 0xBE, 0xCE, 0xAF,
      0x11, 0xDA, 0x27, 0x00, 0x00, 0x39, 0x28, 0x00, 0xA0, 0x00,
      0x00, 0x06, 0x60, 0x00, 0x00, 0xC1, 0x94, 0x60, 0x2E};
  const uint8_t EconoOff[39] = {
      0x11, 0xDA, 0x27, 0x00, 0x01, 0x3B, 0x43, 0xF0, 0x28, 0x0C,
      0x80, 0x04, 0xB0, 0x16, 0x24, 0x00, 0x00, 0xBE, 0xCE, 0xAF,
      0x11, 0xDA, 0x27, 0x00, 0x00, 0x39, 0x28, 0x00, 0xA0, 0x00,
      0x00, 0x06, 0x60, 0x00, 0x00, 0xC1, 0x90, 0x60, 0x2A};
  ac.setRaw(EconoOn);
  ASSERT_TRUE(ac.getEcono());
  EXPECT_EQ(
      "Power: On, Mode: 3 (COOL), Temp: 20C, Fan: 10 (Auto), "
      "Swing (V): 14 (Auto), Swing (H): 190 (Auto), Clock: 13:47, "
      "On Time: Off, Off Time: Off, Sleep Time: Off, Beep: 3 (Off), "
      "Light: 3 (Off), Mold: On, Clean: On, Fresh Air: Off, Eye: Off, "
      "Eye Auto: Off, Quiet: Off, Powerful: Off, Purify: On, Econo: On",
      ac.toString());
  ac.setRaw(EconoOff);
  ASSERT_FALSE(ac.getEcono());
  EXPECT_EQ(
      "Power: On, Mode: 3 (COOL), Temp: 20C, Fan: 10 (Auto), "
      "Swing (V): 14 (Auto), Swing (H): 190 (Auto), Clock: 13:47, "
      "On Time: Off, Off Time: Off, Sleep Time: Off, Beep: 3 (Off), "
      "Light: 3 (Off), Mold: On, Clean: On, Fresh Air: Off, Eye: Off, "
      "Eye Auto: Off, Quiet: Off, Powerful: Off, Purify: On, Econo: Off",
      ac.toString());
}

// Tests for IRDaikin216 class.

TEST(TestDaikin216Class, Power) {
  IRDaikin216 ac(0);
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

TEST(TestDaikin216Class, Temperature) {
  IRDaikin216 ac(0);
  ac.begin();

  ac.setTemp(0);
  EXPECT_EQ(kDaikinMinTemp, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kDaikinMaxTemp, ac.getTemp());

  ac.setTemp(kDaikinMinTemp);
  EXPECT_EQ(kDaikinMinTemp, ac.getTemp());

  ac.setTemp(kDaikinMaxTemp);
  EXPECT_EQ(kDaikinMaxTemp, ac.getTemp());

  ac.setTemp(kDaikinMinTemp - 1);
  EXPECT_EQ(kDaikinMinTemp, ac.getTemp());

  ac.setTemp(kDaikinMaxTemp + 1);
  EXPECT_EQ(kDaikinMaxTemp, ac.getTemp());

  ac.setTemp(kDaikinMinTemp + 1);
  EXPECT_EQ(kDaikinMinTemp + 1, ac.getTemp());

  ac.setTemp(21);
  EXPECT_EQ(21, ac.getTemp());

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());

  ac.setTemp(29);
  EXPECT_EQ(29, ac.getTemp());
}

TEST(TestDaikin216Class, OperatingMode) {
  IRDaikin216 ac(0);
  ac.begin();

  ac.setMode(kDaikinAuto);
  EXPECT_EQ(kDaikinAuto, ac.getMode());

  ac.setMode(kDaikinCool);
  EXPECT_EQ(kDaikinCool, ac.getMode());

  ac.setMode(kDaikinHeat);
  EXPECT_EQ(kDaikinHeat, ac.getMode());

  ac.setMode(kDaikinDry);
  EXPECT_EQ(kDaikinDry, ac.getMode());

  ac.setMode(kDaikinFan);
  EXPECT_EQ(kDaikinFan, ac.getMode());

  ac.setMode(kDaikinFan + 1);
  EXPECT_EQ(kDaikinAuto, ac.getMode());

  ac.setMode(kDaikinAuto + 1);
  EXPECT_EQ(kDaikinAuto, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kDaikinAuto, ac.getMode());
}


TEST(TestDaikin216Class, VaneSwing) {
  IRDaikin216 ac(0);
  ac.begin();

  ac.setSwingHorizontal(true);
  ac.setSwingVertical(false);

  ac.setSwingHorizontal(true);
  EXPECT_TRUE(ac.getSwingHorizontal());
  EXPECT_FALSE(ac.getSwingVertical());

  ac.setSwingVertical(true);
  EXPECT_TRUE(ac.getSwingHorizontal());
  EXPECT_TRUE(ac.getSwingVertical());

  ac.setSwingHorizontal(false);
  EXPECT_FALSE(ac.getSwingHorizontal());
  EXPECT_TRUE(ac.getSwingVertical());

  ac.setSwingVertical(false);
  EXPECT_FALSE(ac.getSwingHorizontal());
  EXPECT_FALSE(ac.getSwingVertical());
}

TEST(TestDaikin216Class, FanSpeed) {
  IRDaikin216 ac(0);
  ac.begin();

  // Unexpected value should default to Auto.
  ac.setFan(0);
  EXPECT_EQ(kDaikinFanAuto, ac.getFan());

  // Unexpected value should default to Auto.
  ac.setFan(255);
  EXPECT_EQ(kDaikinFanAuto, ac.getFan());

  ac.setFan(kDaikinFanMax);
  EXPECT_EQ(kDaikinFanMax, ac.getFan());

  // Beyond Max should default to Auto.
  ac.setFan(kDaikinFanMax + 1);
  EXPECT_EQ(kDaikinFanAuto, ac.getFan());

  ac.setFan(kDaikinFanMax - 1);
  EXPECT_EQ(kDaikinFanMax - 1, ac.getFan());

  ac.setFan(kDaikinFanMin);
  EXPECT_EQ(kDaikinFanMin, ac.getFan());

  ac.setFan(kDaikinFanMin + 1);
  EXPECT_EQ(kDaikinFanMin + 1, ac.getFan());

  // Beyond Min should default to Auto.
  ac.setFan(kDaikinFanMin - 1);
  EXPECT_EQ(kDaikinFanAuto, ac.getFan());

  ac.setFan(3);
  EXPECT_EQ(3, ac.getFan());

  ac.setFan(kDaikinFanAuto);
  EXPECT_EQ(kDaikinFanAuto, ac.getFan());

  ac.setFan(kDaikinFanQuiet);
  EXPECT_EQ(kDaikinFanQuiet, ac.getFan());
}

TEST(TestDaikin216Class, Quiet) {
  IRDaikin216 ac(0);
  ac.begin();

  ac.setQuiet(true);
  EXPECT_TRUE(ac.getQuiet());

  ac.setQuiet(false);
  EXPECT_FALSE(ac.getQuiet());

  ac.setQuiet(true);
  EXPECT_TRUE(ac.getQuiet());
}

TEST(TestDaikin216Class, ExampleStates) {
  IRDaikin216 ac(0);
  ac.begin();
  // https://github.com/markszabo/IRremoteESP8266/pull/690#issuecomment-487770194
  uint8_t state[kDaikin216StateLength] = {
      0x11, 0xDA, 0x27, 0xF0, 0x00, 0x00, 0x00, 0x02,
      0x11, 0xDA, 0x27, 0x00, 0x00, 0x21, 0xC0, 0x00, 0xA0, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x53};
  ac.setRaw(state);
  EXPECT_EQ(
      "Power: On, Mode: 2 (DRY), Temp: 32C, Fan: 10 (AUTO), "
      "Swing (Horizontal): Off, Swing (Vertical): Off, Quiet: Off",
      ac.toString());
}

TEST(TestDaikin216Class, ReconstructKnownState) {
  IRDaikin216 ac(0);
  ac.begin();
  // https://github.com/markszabo/IRremoteESP8266/issues/689#issue-438086949
  uint8_t expectedState[kDaikin216StateLength] = {
      0x11, 0xDA, 0x27, 0xF0, 0x00, 0x00, 0x00, 0x02,
      0x11, 0xDA, 0x27, 0x00, 0x00, 0x00, 0x26, 0x00, 0xA0, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x98};
  ac.setPower(false);
  ac.setMode(kDaikinAuto);
  ac.setTemp(19);
  ac.setFan(kDaikinFanAuto);
  ac.setSwingHorizontal(false);
  ac.setSwingVertical(false);
  ac.setQuiet(false);
  EXPECT_EQ(
      "Power: Off, Mode: 0 (AUTO), Temp: 19C, Fan: 10 (AUTO), "
      "Swing (Horizontal): Off, Swing (Vertical): Off, Quiet: Off",
      ac.toString());

  EXPECT_STATE_EQ(expectedState, ac.getRaw(), kDaikin216Bits);
}

// https://github.com/markszabo/IRremoteESP8266/issues/689
TEST(TestDecodeDaikin216, RealExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  // https://github.com/markszabo/IRremoteESP8266/issues/689#issue-438086949
  uint16_t rawData[439] = {
      3402, 1770, 382, 1340, 382, 480, 382, 478, 382, 480, 380, 1342, 382, 478,
      356, 504, 382, 480, 380, 478, 384, 1342, 380, 480, 380, 1342, 382, 1342,
      382, 478, 382, 1340, 382, 1340, 384, 1340, 382, 1342, 382, 1340, 380, 480,
      382, 480, 382, 1296, 426, 480, 380, 480, 382, 480, 380, 480, 382, 480,
      382, 478, 382, 1342, 382, 1342, 382, 1340, 356, 1368, 382, 478, 382, 480,
      382, 478, 380, 480, 382, 480, 382, 480, 382, 478, 382, 480, 382, 478, 358,
      504, 382, 480, 380, 480, 382, 480, 382, 480, 380, 480, 382, 478, 382, 480,
      382, 478, 382, 480, 354, 506, 354, 506, 380, 480, 382, 480, 382, 480, 382,
      480, 380, 1342, 382, 480, 382, 480, 382, 478, 382, 478, 382, 478, 384,
      478, 382, 29652, 3426, 1772, 382, 1340, 382, 480, 380, 478, 382, 480, 382,
      1342, 382, 480, 382, 480, 382, 478, 356, 506, 382, 1342, 380, 480, 382,
      1340, 382, 1340, 382, 478, 356, 1366, 382, 1340, 384, 1340, 382, 1340,
      382, 1342, 382, 478, 382, 478, 382, 1340, 382, 478, 382, 478, 382, 478,
      382, 480, 382, 480, 384, 478, 358, 504, 382, 478, 382, 480, 382, 478, 382,
      480, 382, 480, 382, 478, 382, 480, 382, 478, 382, 478, 382, 478, 382, 478,
      384, 478, 382, 478, 360, 500, 358, 504, 382, 478, 382, 480, 382, 480, 382,
      478, 382, 478, 382, 1340, 382, 1342, 382, 480, 380, 480, 382, 1342, 382,
      478, 382, 480, 356, 506, 382, 478, 382, 480, 382, 480, 356, 506, 382, 478,
      382, 480, 382, 478, 382, 480, 382, 478, 382, 480, 380, 480, 380, 480, 382,
      1342, 382, 478, 382, 1342, 382, 480, 382, 480, 382, 478, 382, 478, 382,
      480, 382, 478, 382, 480, 356, 504, 384, 478, 382, 480, 382, 480, 380, 480,
      382, 478, 382, 480, 382, 480, 382, 478, 356, 504, 384, 478, 380, 480, 382,
      480, 382, 480, 382, 478, 356, 506, 382, 478, 382, 480, 380, 480, 382, 478,
      382, 480, 382, 478, 382, 480, 358, 504, 382, 478, 382, 478, 356, 504, 382,
      478, 382, 480, 382, 478, 382, 478, 382, 478, 382, 480, 380, 480, 382, 480,
      380, 480, 356, 506, 356, 504, 382, 480, 382, 478, 382, 478, 382, 478, 382,
      478, 382, 480, 382, 478, 382, 480, 382, 480, 382, 1340, 382, 1342, 382,
      478, 384, 478, 382, 478, 382, 480, 380, 480, 382, 478, 382, 480, 356, 506,
      382, 478, 382, 480, 382, 478, 356, 506, 380, 480, 382, 478, 382, 478, 382,
      478, 382, 480, 382, 480, 380, 480, 382, 1342, 382, 1340, 382, 480, 356,
      504, 382, 1342, 382};  // UNKNOWN E0E32232
  uint8_t expectedState[kDaikin216StateLength] = {
      // 8 bytes
      0x11, 0xDA, 0x27, 0xF0, 0x00, 0x00, 0x00, 0x02,
      // 19 bytes
      0x11, 0xDA, 0x27, 0x00, 0x00, 0x00, 0x26, 0x00, 0xA0, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x98};

  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 439, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(DAIKIN216, irsend.capture.decode_type);
  ASSERT_EQ(kDaikin216Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);

  IRDaikin216 ac(0);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: Off, Mode: 0 (AUTO), Temp: 19C, Fan: 10 (AUTO), "
      "Swing (Horizontal): Off, Swing (Vertical): Off, Quiet: Off",
      ac.toString());
}

// https://github.com/markszabo/IRremoteESP8266/issues/689
TEST(TestDecodeDaikin216, SyntheticExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  // https://github.com/markszabo/IRremoteESP8266/issues/689#issue-438086949
  uint8_t expectedState[kDaikin216StateLength] = {
      // 8 bytes
      0x11, 0xDA, 0x27, 0xF0, 0x00, 0x00, 0x00, 0x02,
      // 19 bytes
      0x11, 0xDA, 0x27, 0x00, 0x00, 0x00, 0x26, 0x00, 0xA0, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x98};

  irsend.begin();
  irsend.reset();
  irsend.sendDaikin216(expectedState);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(DAIKIN216, irsend.capture.decode_type);
  ASSERT_EQ(kDaikin216Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
}
