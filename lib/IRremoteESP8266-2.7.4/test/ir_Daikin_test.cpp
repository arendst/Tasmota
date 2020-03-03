// Copyright 2017-2019 David Conran
#include "ir_Daikin.h"
#include "IRac.h"
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

TEST(TestDaikinClass, WeeklyTimerEnable) {
  IRDaikinESP ac(0);
  ac.begin();

  // The Weekly Timer Enabled flag is stored in the same byte as Econo mode.
  // Econo mode tests are there to make sure it isn't harmed and vice-versa.
  ac.setEcono(false);
  ac.setWeeklyTimerEnable(false);
  ASSERT_FALSE(ac.getWeeklyTimerEnable());
  EXPECT_FALSE(ac.getEcono());

  ac.setWeeklyTimerEnable(true);
  ASSERT_TRUE(ac.getWeeklyTimerEnable());
  EXPECT_FALSE(ac.getEcono());

  ac.setEcono(false);
  ASSERT_TRUE(ac.getWeeklyTimerEnable());
  EXPECT_FALSE(ac.getEcono());

  ac.setEcono(true);
  ASSERT_TRUE(ac.getWeeklyTimerEnable());
  EXPECT_TRUE(ac.getEcono());

  ac.setWeeklyTimerEnable(false);
  ASSERT_FALSE(ac.getWeeklyTimerEnable());
  EXPECT_TRUE(ac.getEcono());

  // Tests with real data from:
  // https://github.com/crankyoldgit/IRremoteESP8266/issues/704#issuecomment-493731421
  uint8_t on[kDaikinStateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0xC5, 0x00, 0x00, 0xD7, 0x11, 0xDA, 0x27, 0x00,
      0x42, 0xE3, 0x0B, 0x42, 0x11, 0xDA, 0x27, 0x00, 0x00, 0x68, 0x32, 0x00,
      0x30, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0xC1, 0x00, 0x00, 0x03};
  uint8_t off[kDaikinStateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0xC5, 0x00, 0x00, 0xD7, 0x11, 0xDA, 0x27, 0x00,
      0x42, 0xE3, 0x0B, 0x42, 0x11, 0xDA, 0x27, 0x00, 0x00, 0x68, 0x32, 0x00,
      0x30, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0xC1, 0x80, 0x00, 0x83};
  ac.setRaw(on);
  EXPECT_TRUE(ac.getWeeklyTimerEnable());
  ac.setRaw(off);
  EXPECT_FALSE(ac.getWeeklyTimerEnable());
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
      "Power: On, Mode: 4 (Heat), Temp: 15C, Fan: 11 (Quiet), "
      "Powerful: Off, Quiet: Off, Sensor: Off, Mould: Off, "
      "Comfort: Off, Swing(H): Off, Swing(V): Off, "
      "Clock: 00:00, Day: 0 (UNKNOWN), On Timer: Off, "
      "Off Timer: Off, Weekly Timer: On",
      ac.toString());
  ac.setMode(kDaikinAuto);
  ac.setTemp(25);
  ac.setFan(kDaikinFanAuto);
  ac.setQuiet(true);
  ac.setSensor(true);
  ac.setMold(true);
  ac.setSwingVertical(true);
  ac.setSwingHorizontal(true);
  ac.setCurrentTime(9 * 60 + 15);
  ac.setCurrentDay(4);
  ac.enableOnTimer(8 * 60 + 0);
  ac.enableOffTimer(17 * 60 + 30);
  ac.setComfort(true);
  ac.setWeeklyTimerEnable(false);
  ac.off();
  EXPECT_EQ(
      "Power: Off, Mode: 0 (Auto), Temp: 25C, Fan: 10 (Auto), "
      "Powerful: Off, Quiet: On, Sensor: On, Mould: On, Comfort: On, "
      "Swing(H): On, Swing(V): On, "
      "Clock: 09:15, Day: 4 (Wed), On Timer: 08:00, Off Timer: 17:30, "
      "Weekly Timer: Off",
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
      "Power: On, Mode: 3 (Cool), Temp: 29C, Fan: 10 (Auto), Powerful: On, "
      "Quiet: Off, Sensor: Off, Mould: Off, Comfort: Off, "
      "Swing(H): Off, Swing(V): Off, "
      "Clock: 22:18, Day: 0 (UNKNOWN), "
      "On Timer: 21:30, Off Timer: 06:10, Weekly Timer: On", ac.toString());
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
      "Power: On, Mode: 3 (Cool), Temp: 29C, Fan: 10 (Auto), Powerful: On, "
      "Quiet: Off, Sensor: Off, Mould: Off, Comfort: Off, "
      "Swing(H): Off, Swing(V): Off, "
      "Clock: 22:18, Day: 0 (UNKNOWN), "
      "On Timer: 21:30, Off Timer: 06:10, Weekly Timer: On", ac.toString());
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
      "Power: On, Mode: 3 (Cool), Temp: 29C, Fan: 10 (Auto), Powerful: On, "
      "Quiet: Off, Sensor: Off, Mould: Off, Comfort: Off, "
      "Swing(H): Off, Swing(V): Off, "
      "Clock: 22:18, Day: 0 (UNKNOWN), "
      "On Timer: 21:30, Off Timer: 06:10, Weekly Timer: On", ac.toString());
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
  // "Off" Data from https://github.com/crankyoldgit/IRremoteESP8266/issues/582
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
      "Power: Off, Mode: 0 (Auto), Temp: 19C, Fan: 10 (Auto), "
      "Swing(V): 5 (Low), Swing(H): 190 (Auto), "
      "Clock: 14:50, On Timer: Off, Off Timer: Off, Sleep Timer: Off, "
      "Beep: 1 (Quiet), Light: 3 (Off), Mould: On, Clean: On, Fresh: Off, "
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
  ac.setSwingVertical(kDaikin2SwingVSwing);
  ASSERT_EQ(kDaikin2SwingVSwing, ac.getSwingVertical());
  ac.setSwingVertical(kDaikin2SwingVAuto);
  ASSERT_EQ(kDaikin2SwingVAuto, ac.getSwingVertical());
  ac.setSwingVertical(0);
  ASSERT_EQ(kDaikin2SwingVAuto, ac.getSwingVertical());
  ac.setSwingVertical(20);
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
      "Power: On, Mode: 3 (Cool), Temp: 21C, Fan: 5 (High), "
      "Swing(V): 14 (Auto), Swing(H): 191 (Swing), Clock: 12:34, "
      "On Timer: Off, Off Timer: 20:00, Sleep Timer: 04:00, Beep: 2 (Loud), "
      "Light: 2 (Low), Mould: On, Clean: Off, Fresh: On, Eye: On, "
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
      "Power: On, Mode: 4 (Heat), Temp: 32C, Fan: 1 (Low), "
      "Swing(V): 14 (Auto), Swing(H): 191 (Swing), Clock: 23:45, "
      "On Timer: 09:11, Off Timer: 20:00, Sleep Timer: Off, Beep: 1 (Quiet), "
      "Light: 1 (High), Mould: On, Clean: Off, Fresh: On, Eye: On, "
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
      "Power: Off, Mode: 0 (Auto), Temp: 19C, Fan: 10 (Auto), "
      "Swing(V): 5 (Low), Swing(H): 190 (Auto), "
      "Clock: 14:50, On Timer: Off, Off Timer: Off, Sleep Timer: Off, "
      "Beep: 1 (Quiet), Light: 3 (Off), Mould: On, Clean: On, Fresh: Off, "
      "Eye: Off, Eye Auto: Off, Quiet: Off, Powerful: Off, Purify: Off, "
      "Econo: Off",
      ac.toString());
  EXPECT_STATE_EQ(expectedState, ac.getRaw(), kDaikin2Bits);
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("DAIKIN", typeToString(decode_type_t::DAIKIN));
  ASSERT_EQ(decode_type_t::DAIKIN, strToDecodeType("DAIKIN"));
  ASSERT_TRUE(hasACState(decode_type_t::DAIKIN));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::DAIKIN));

  ASSERT_EQ("DAIKIN128", typeToString(decode_type_t::DAIKIN128));
  ASSERT_EQ(decode_type_t::DAIKIN128, strToDecodeType("DAIKIN128"));
  ASSERT_TRUE(hasACState(decode_type_t::DAIKIN128));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::DAIKIN128));

  ASSERT_EQ("DAIKIN152", typeToString(decode_type_t::DAIKIN152));
  ASSERT_EQ(decode_type_t::DAIKIN152, strToDecodeType("DAIKIN152"));
  ASSERT_TRUE(hasACState(decode_type_t::DAIKIN152));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::DAIKIN152));

  ASSERT_EQ("DAIKIN160", typeToString(decode_type_t::DAIKIN160));
  ASSERT_EQ(decode_type_t::DAIKIN160, strToDecodeType("DAIKIN160"));
  ASSERT_TRUE(hasACState(decode_type_t::DAIKIN160));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::DAIKIN160));

  ASSERT_EQ("DAIKIN176", typeToString(decode_type_t::DAIKIN176));
  ASSERT_EQ(decode_type_t::DAIKIN176, strToDecodeType("DAIKIN176"));
  ASSERT_TRUE(hasACState(decode_type_t::DAIKIN176));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::DAIKIN176));

  ASSERT_EQ("DAIKIN2", typeToString(decode_type_t::DAIKIN2));
  ASSERT_EQ(decode_type_t::DAIKIN2, strToDecodeType("DAIKIN2"));
  ASSERT_TRUE(hasACState(decode_type_t::DAIKIN2));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::DAIKIN2));

  ASSERT_EQ("DAIKIN216", typeToString(decode_type_t::DAIKIN216));
  ASSERT_EQ(decode_type_t::DAIKIN216, strToDecodeType("DAIKIN216"));
  ASSERT_TRUE(hasACState(decode_type_t::DAIKIN216));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::DAIKIN216));
}

// https://github.com/crankyoldgit/IRremoteESP8266/issues/582#issuecomment-453863879
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
      "Power: On, Mode: 0 (Auto), Temp: 19C, Fan: 10 (Auto), "
      "Swing(V): 14 (Auto), Swing(H): 190 (Auto), Clock: 09:20, "
      "On Timer: Off, Off Timer: Off, Sleep Timer: Off, Beep: 3 (Off), "
      "Light: 3 (Off), Mould: On, Clean: On, Fresh: Off, Eye: On, "
      "Eye Auto: Off, Quiet: Off, Powerful: Off, Purify: On, Econo: Off",
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
      "Power: On, Mode: 3 (Cool), Temp: 20C, Fan: 10 (Auto), "
      "Swing(V): 14 (Auto), Swing(H): 190 (Auto), Clock: 13:46, "
      "On Timer: Off, Off Timer: Off, Sleep Timer: Off, Beep: 3 (Off), "
      "Light: 3 (Off), Mould: On, Clean: On, Fresh: Off, Eye: Off, "
      "Eye Auto: Off, Quiet: Off, Powerful: On, Purify: On, Econo: Off",
      ac.toString());
  ac.setRaw(PowerfulOff);
  ASSERT_FALSE(ac.getPowerful());
  EXPECT_EQ(
      "Power: On, Mode: 3 (Cool), Temp: 20C, Fan: 10 (Auto), "
      "Swing(V): 14 (Auto), Swing(H): 190 (Auto), Clock: 13:46, "
      "On Timer: Off, Off Timer: Off, Sleep Timer: Off, Beep: 3 (Off), "
      "Light: 3 (Off), Mould: On, Clean: On, Fresh: Off, Eye: Off, "
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
      "Power: On, Mode: 3 (Cool), Temp: 20C, Fan: 10 (Auto), "
      "Swing(V): 14 (Auto), Swing(H): 190 (Auto), Clock: 13:47, "
      "On Timer: Off, Off Timer: Off, Sleep Timer: Off, Beep: 3 (Off), "
      "Light: 3 (Off), Mould: On, Clean: On, Fresh: Off, Eye: Off, "
      "Eye Auto: Off, Quiet: Off, Powerful: Off, Purify: On, Econo: On",
      ac.toString());
  ac.setRaw(EconoOff);
  ASSERT_FALSE(ac.getEcono());
  EXPECT_EQ(
      "Power: On, Mode: 3 (Cool), Temp: 20C, Fan: 10 (Auto), "
      "Swing(V): 14 (Auto), Swing(H): 190 (Auto), Clock: 13:47, "
      "On Timer: Off, Off Timer: Off, Sleep Timer: Off, Beep: 3 (Off), "
      "Light: 3 (Off), Mould: On, Clean: On, Fresh: Off, Eye: Off, "
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

TEST(TestDaikin216Class, QuietAndPowerful) {
  IRDaikin216 ac(0);
  ac.begin();

  ac.setQuiet(false);
  ac.setPowerful(false);
  EXPECT_FALSE(ac.getQuiet());
  EXPECT_FALSE(ac.getPowerful());

  ac.setQuiet(true);
  EXPECT_TRUE(ac.getQuiet());
  EXPECT_FALSE(ac.getPowerful());

  ac.setPowerful(true);
  EXPECT_FALSE(ac.getQuiet());
  EXPECT_TRUE(ac.getPowerful());

  ac.setQuiet(true);
  EXPECT_TRUE(ac.getQuiet());
  EXPECT_FALSE(ac.getPowerful());

  ac.setQuiet(false);
  EXPECT_FALSE(ac.getQuiet());
  EXPECT_FALSE(ac.getPowerful());

  ac.setPowerful(true);
  EXPECT_FALSE(ac.getQuiet());
  EXPECT_TRUE(ac.getPowerful());

  ac.setQuiet(true);
  EXPECT_TRUE(ac.getQuiet());
}

TEST(TestDaikin216Class, ExampleStates) {
  IRDaikin216 ac(0);
  ac.begin();
  // https://github.com/crankyoldgit/IRremoteESP8266/pull/690#issuecomment-487770194
  uint8_t state[kDaikin216StateLength] = {
      0x11, 0xDA, 0x27, 0xF0, 0x00, 0x00, 0x00, 0x02,
      0x11, 0xDA, 0x27, 0x00, 0x00, 0x21, 0xC0, 0x00, 0xA0, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x53};
  ac.setRaw(state);
  EXPECT_EQ(
      "Power: On, Mode: 2 (Dry), Temp: 32C, Fan: 10 (Auto), "
      "Swing(H): Off, Swing(V): Off, "
      "Quiet: Off, Powerful: Off",
      ac.toString());
}

TEST(TestDaikin216Class, ReconstructKnownState) {
  IRDaikin216 ac(0);
  ac.begin();
  // https://github.com/crankyoldgit/IRremoteESP8266/issues/689#issue-438086949
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
      "Power: Off, Mode: 0 (Auto), Temp: 19C, Fan: 10 (Auto), "
      "Swing(H): Off, Swing(V): Off, "
      "Quiet: Off, Powerful: Off",
      ac.toString());

  EXPECT_STATE_EQ(expectedState, ac.getRaw(), kDaikin216Bits);
}

// https://github.com/crankyoldgit/IRremoteESP8266/issues/689
TEST(TestDecodeDaikin216, RealExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  // https://github.com/crankyoldgit/IRremoteESP8266/issues/689#issue-438086949
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
      "Power: Off, Mode: 0 (Auto), Temp: 19C, Fan: 10 (Auto), "
      "Swing(H): Off, Swing(V): Off, "
      "Quiet: Off, Powerful: Off",
      ac.toString());
}

// https://github.com/crankyoldgit/IRremoteESP8266/issues/689
TEST(TestDecodeDaikin216, SyntheticExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  // https://github.com/crankyoldgit/IRremoteESP8266/issues/689#issue-438086949
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

TEST(TestDaikinClass, toCommon) {
  IRDaikinESP ac(0);
  ac.setPower(true);
  ac.setMode(kDaikinCool);
  ac.setTemp(20);
  ac.setFan(kDaikinFanMax);
  ac.setSwingVertical(true);
  ac.setSwingHorizontal(true);
  ac.setQuiet(false);
  ac.setPowerful(true);
  ac.setEcono(false);
  ac.setMold(true);
  // Now test it.
  ASSERT_EQ(decode_type_t::DAIKIN, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_TRUE(ac.toCommon().turbo);
  ASSERT_TRUE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kAuto, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kAuto, ac.toCommon().swingh);
  // Unsupported.
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

TEST(TestDaikin2Class, toCommon) {
  IRDaikin2 ac(0);
  ac.setPower(true);
  ac.setMode(kDaikinCool);
  ac.setTemp(20);
  ac.setFan(kDaikinFanMax);
  ac.setSwingVertical(kDaikin2SwingVHigh + 3);
  ac.setSwingHorizontal(kDaikin2SwingHAuto);
  ac.setQuiet(false);
  ac.setPowerful(true);
  ac.setEcono(false);
  ac.setMold(true);
  ac.setLight(true);
  ac.setPurify(true);
  ac.setBeep(true);
  ac.enableSleepTimer(6 * 60);
  // Now test it.
  ASSERT_EQ(decode_type_t::DAIKIN2, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_TRUE(ac.toCommon().turbo);
  ASSERT_TRUE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_TRUE(ac.toCommon().light);
  ASSERT_TRUE(ac.toCommon().filter);
  ASSERT_TRUE(ac.toCommon().beep);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kMiddle, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  ASSERT_EQ(6 * 60, ac.toCommon().sleep);
  // Unsupported.
  ASSERT_EQ(-1, ac.toCommon().clock);
}

TEST(TestDaikin216Class, toCommon) {
  IRDaikin216 ac(0);
  ac.setPower(true);
  ac.setMode(kDaikinCool);
  ac.setTemp(20);
  ac.setFan(kDaikinFanMax);
  ac.setSwingVertical(true);
  ac.setSwingHorizontal(true);
  ac.setQuiet(false);
  ac.setPowerful(true);
  // Now test it.
  ASSERT_EQ(decode_type_t::DAIKIN216, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_TRUE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kAuto, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kAuto, ac.toCommon().swingh);
  // Unsupported.
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

// https://github.com/crankyoldgit/IRremoteESP8266/issues/731
TEST(TestDecodeDaikin160, RealExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  uint16_t rawData[327] = {
      5024, 2144, 342, 1786, 344, 706, 342, 706, 344, 706, 342, 1786, 342, 706,
      342, 708, 342, 708, 342, 708, 342, 1786, 342, 708, 342, 1786, 342, 1788,
      342, 708, 342, 1786, 344, 1786, 342, 1786, 342, 1786, 342, 1786, 342, 708,
      342, 708, 340, 1786, 344, 706, 342, 708, 342, 706, 342, 708, 342, 708,
      342, 706, 342, 1786, 342, 1788, 342, 1786, 342, 1786, 342, 1788, 342, 706,
      342, 1788, 342, 1786, 342, 706, 342, 706, 342, 708, 342, 708, 342, 708,
      342, 708, 342, 706, 342, 708, 342, 708, 342, 706, 342, 706, 342, 708, 342,
      1786, 342, 1786, 342, 1786, 342, 1788, 342, 706, 342, 706, 342, 706, 342,
      708, 342, 29442, 5022, 2146, 342, 1786, 342, 706, 342, 708, 342, 708, 342,
      1788, 342, 706, 342, 706, 342, 706, 342, 706, 342, 1788, 342, 708, 342,
      1786, 342, 1786, 342, 708, 342, 1786, 342, 1788, 342, 1786, 342, 1786,
      342, 1786, 342, 706, 342, 706, 342, 1786, 344, 706, 342, 706, 344, 706,
      342, 706, 342, 706, 342, 708, 342, 706, 342, 706, 342, 706, 342, 706, 342,
      1786, 342, 1786, 342, 706, 342, 706, 342, 1788, 342, 708, 342, 1786, 342,
      1786, 342, 706, 342, 706, 342, 706, 342, 708, 342, 1786, 342, 1786, 342,
      708, 342, 708, 342, 1786, 342, 706, 342, 706, 344, 706, 342, 1786, 342,
      708, 342, 706, 342, 706, 342, 708, 342, 706, 342, 708, 342, 706, 342, 708,
      342, 706, 342, 704, 344, 706, 342, 706, 344, 706, 342, 706, 342, 708, 342,
      706, 342, 706, 344, 706, 342, 706, 342, 706, 344, 1786, 342, 1786, 342,
      1786, 342, 1786, 342, 706, 342, 706, 344, 706, 342, 706, 342, 1786, 344,
      706, 342, 1786, 342, 706, 342, 708, 342, 706, 342, 706, 344, 706, 342,
      706, 342, 706, 342, 1786, 342, 706, 344, 706, 342, 706, 342, 708, 342,
      706, 342, 1788, 342, 1786, 342, 706, 344, 1786, 344, 706, 344, 1786, 342,
      708, 342};  // UNKNOWN 99CC993

  uint8_t expectedState[kDaikin160StateLength] = {
      // 7 bytes
      0x11, 0xDA, 0x27, 0xF0, 0x0D, 0x00, 0x0F,
      // 13 bytes
      0x11, 0xDA, 0x27, 0x00, 0xD3, 0x30, 0x11,
      0x00, 0x00, 0x1E, 0x0A, 0x08, 0x56};

  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 327, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(DAIKIN160, irsend.capture.decode_type);
  ASSERT_EQ(kDaikin160Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  IRDaikin160 ac(0);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ("Power: Off, Mode: 3 (Cool), Temp: 25C, Fan: 10 (Auto), "
            "Swing(V): 1 (Lowest)", ac.toString());
}

TEST(TestDecodeDaikin160, SyntheticExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);

  uint8_t expectedState[kDaikin160StateLength] = {
      // 7 bytes
      0x11, 0xDA, 0x27, 0xF0, 0x0D, 0x00, 0x0F,
      // 13 bytes
      0x11, 0xDA, 0x27, 0x00, 0xD3, 0x30, 0x11,
      0x00, 0x00, 0x1E, 0x0A, 0x08, 0x56};

  irsend.begin();
  irsend.reset();
  irsend.sendDaikin160(expectedState);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(DAIKIN160, irsend.capture.decode_type);
  ASSERT_EQ(kDaikin160Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
}

TEST(TestDaikin160Class, toCommon) {
  IRDaikin160 ac(0);
  ac.setPower(true);
  ac.setMode(kDaikinCool);
  ac.setTemp(20);
  ac.setFan(kDaikinFanMax);
  ac.setSwingVertical(kDaikin160SwingVAuto);
  // Now test it.
  ASSERT_EQ(decode_type_t::DAIKIN160, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kAuto, ac.toCommon().swingv);
  // Unsupported.
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

TEST(TestDaikin160Class, FanSpeed) {
  IRDaikin160 ac(0);
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

TEST(TestDaikin160Class, VaneSwing) {
  IRDaikin160 ac(0);
  ac.begin();

  ac.setSwingVertical(kDaikin160SwingVAuto);
  EXPECT_EQ(kDaikin160SwingVAuto, ac.getSwingVertical());

  ac.setSwingVertical(kDaikin160SwingVHigh);
  EXPECT_EQ(kDaikin160SwingVHigh, ac.getSwingVertical());

  ac.setSwingVertical(255);
  EXPECT_EQ(kDaikin160SwingVAuto, ac.getSwingVertical());

  EXPECT_EQ(kDaikin160SwingVHighest,
            IRDaikin160::convertSwingV(stdAc::swingv_t::kHighest));
  EXPECT_EQ(kDaikin160SwingVLowest,
            IRDaikin160::convertSwingV(stdAc::swingv_t::kLowest));
  EXPECT_EQ(kDaikin160SwingVMiddle,
            IRDaikin160::convertSwingV(stdAc::swingv_t::kMiddle));
}

TEST(TestDaikin160Class, Power) {
  IRDaikin160 ac(0);
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

TEST(TestDaikin160Class, Temperature) {
  IRDaikin160 ac(0);
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

TEST(TestDaikin160Class, OperatingMode) {
  IRDaikin160 ac(0);
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

TEST(TestDaikin160Class, HumanReadable) {
  IRDaikin160 ac(0);

  EXPECT_EQ(
      "Power: Off, Mode: 3 (Cool), Temp: 25C, Fan: 10 (Auto), "
      "Swing(V): 1 (Lowest)",
      ac.toString());
  ac.setMode(kDaikinAuto);
  ac.setTemp(19);
  ac.setFan(kDaikinFanMin);
  ac.setSwingVertical(kDaikin160SwingVAuto);
  ac.setPower(true);
  EXPECT_EQ(
      "Power: On, Mode: 0 (Auto), Temp: 19C, Fan: 1 (Low), "
      "Swing(V): 15 (Auto)",
      ac.toString());
}

TEST(TestDaikin176Class, FanControl) {
  IRDaikin176 ac(0);

  EXPECT_EQ(
      "Power: Off, Mode: 7 (Cool), Temp: 9C, Fan: 1 (Low), Swing(H): 6 (Off)",
      ac.toString());
  ac.setFan(kDaikinFanMin);
  ac.setPower(true);
  EXPECT_EQ(
      "Power: On, Mode: 7 (Cool), Temp: 9C, Fan: 1 (Low), Swing(H): 6 (Off)",
      ac.toString());
  ac.setFan(kDaikinFanMin + 1);
  EXPECT_EQ(
      "Power: On, Mode: 7 (Cool), Temp: 9C, Fan: 3 (High), Swing(H): 6 (Off)",
      ac.toString());
  ac.setFan(kDaikin176FanMax);
  EXPECT_EQ(
      "Power: On, Mode: 7 (Cool), Temp: 9C, Fan: 3 (High), Swing(H): 6 (Off)",
      ac.toString());

  // Real state from remote
  // https://github.com/crankyoldgit/IRremoteESP8266/pull/826#issuecomment-513168270
  uint8_t state[kDaikin176StateLength] = {
      0x11, 0xDA, 0x17, 0x18, 0x04, 0x00, 0x1E,
      0x11, 0xDA, 0x17, 0x18, 0x00, 0x73, 0x00, 0x21, 0x00, 0x00, 0x22, 0x35,
      0x00, 0x20, 0x25};
  ac.setRaw(state);
  EXPECT_EQ(
      "Power: On, Mode: 7 (Cool), Temp: 26C, Fan: 3 (High), "
      "Swing(H): 5 (Auto)",
      ac.toString());
}

TEST(TestDaikin176Class, convertFan) {
  EXPECT_EQ(kDaikinFanMin, IRDaikin176::convertFan(stdAc::fanspeed_t::kMin));
  EXPECT_EQ(kDaikinFanMin, IRDaikin176::convertFan(stdAc::fanspeed_t::kLow));
  EXPECT_EQ(kDaikin176FanMax,
            IRDaikin176::convertFan(stdAc::fanspeed_t::kMedium));
  EXPECT_EQ(kDaikin176FanMax,
            IRDaikin176::convertFan(stdAc::fanspeed_t::kHigh));
  EXPECT_EQ(kDaikin176FanMax, IRDaikin176::convertFan(stdAc::fanspeed_t::kMax));
  EXPECT_EQ(kDaikin176FanMax,
            IRDaikin176::convertFan(stdAc::fanspeed_t::kAuto));
}

TEST(TestDaikin176Class, SimulateIRacDaikin176) {
  IRDaikin176 ac(0);

  ac.setPower(true);
  ac.setMode(ac.convertMode(stdAc::opmode_t::kCool));
  ac.setTemp(26);
  ac.setFan(ac.convertFan(stdAc::fanspeed_t::kMax));
  ac.setSwingHorizontal(kDaikin176SwingHOff);
  EXPECT_EQ(
      "Power: On, Mode: 7 (Cool), Temp: 26C, Fan: 3 (High), Swing(H): 6 (Off)",
      ac.toString());
  ac.setSwingHorizontal(ac.convertSwingH(stdAc::swingh_t::kAuto));
  EXPECT_EQ(
      "Power: On, Mode: 7 (Cool), Temp: 26C, Fan: 3 (High), "
      "Swing(H): 5 (Auto)",
      ac.toString());
}

TEST(TestDaikin176Class, OperatingMode) {
  IRDaikin176 ac(0);
  ac.begin();

  ac.setMode(kDaikinAuto);
  EXPECT_EQ(kDaikin176Cool, ac.getMode());

  ac.setMode(kDaikin176Cool);
  EXPECT_EQ(kDaikin176Cool, ac.getMode());

  ac.setMode(kDaikinDry);
  EXPECT_EQ(kDaikinDry, ac.getMode());

  ac.setMode(kDaikinHeat);
  EXPECT_EQ(kDaikin176Cool, ac.getMode());

  ac.setMode(kDaikinFan);
  EXPECT_EQ(kDaikinFan, ac.getMode());

  ac.setMode(kDaikin176Cool + 1);
  EXPECT_EQ(kDaikin176Cool, ac.getMode());

  ac.setMode(kDaikinAuto + 1);
  EXPECT_EQ(kDaikin176Cool, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kDaikin176Cool, ac.getMode());
}

TEST(TestDaikin176Class, Temperature) {
  IRDaikin176 ac(0);
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

  ac.setTemp(21);
  EXPECT_EQ(21, ac.getTemp());

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());

  ac.setTemp(29);
  EXPECT_EQ(29, ac.getTemp());

  // Temp should be locked to kDaikin176DryFanTemp when in Dry or Fan Mode.
  ac.setMode(kDaikinFan);
  EXPECT_EQ(kDaikin176DryFanTemp, ac.getTemp());
  ac.setMode(kDaikin176Cool);
  EXPECT_EQ(29, ac.getTemp());
  ac.setMode(kDaikinDry);
  EXPECT_EQ(kDaikinDry, ac.getMode());
  EXPECT_EQ(kDaikin176DryFanTemp, ac.getTemp());
  ac.setMode(kDaikin176Cool);
  EXPECT_EQ(29, ac.getTemp());
  ac.setMode(kDaikinFan);
  ac.setTemp(25);
  EXPECT_EQ(kDaikin176DryFanTemp, ac.getTemp());
  ac.setMode(kDaikinHeat);
  EXPECT_EQ(25, ac.getTemp());
}

TEST(TestDaikin176Class, Power) {
  IRDaikin176 ac(0);
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

TEST(TestDaikin176Class, VaneSwing) {
  IRDaikin176 ac(0);
  ac.begin();

  ac.setSwingHorizontal(kDaikin176SwingHAuto);
  EXPECT_EQ(kDaikin176SwingHAuto, ac.getSwingHorizontal());

  ac.setSwingHorizontal(0);
  EXPECT_EQ(kDaikin176SwingHAuto, ac.getSwingHorizontal());

  ac.setSwingHorizontal(kDaikin176SwingHOff);
  EXPECT_EQ(kDaikin176SwingHOff, ac.getSwingHorizontal());

  ac.setSwingHorizontal(255);
  EXPECT_EQ(kDaikin176SwingHAuto, ac.getSwingHorizontal());

  EXPECT_EQ(kDaikin176SwingHAuto,
            IRDaikin176::convertSwingH(stdAc::swingh_t::kAuto));
  EXPECT_EQ(kDaikin176SwingHOff,
            IRDaikin176::convertSwingH(stdAc::swingh_t::kOff));
  EXPECT_EQ(kDaikin176SwingHAuto,
            IRDaikin176::convertSwingH(stdAc::swingh_t::kLeft));
}

TEST(TestDaikin176Class, ReconstructKnownStates) {
  IRDaikin176 ac(0);
  ac.begin();
  // Data from:
  //   https://github.com/crankyoldgit/IRremoteESP8266/pull/826#issuecomment-513531138

  // Power: On, Mode: 7 (Cool), Temp: 25C, Fan: 3 (Max), Swing(H): 5 (Auto)
  // 11DA171804001E11DA17180073002100002035002023
  uint8_t on_cool_25_max_auto[22] = {
      0x11, 0xDA, 0x17, 0x18, 0x04, 0x00, 0x1E,
      0x11, 0xDA, 0x17, 0x18, 0x00, 0x73, 0x00, 0x21, 0x00, 0x00, 0x20, 0x35,
      0x00, 0x20, 0x23};
  // Power: On, Mode: 6 (Fan), Temp: 17C, Fan: 3 (Max), Swing(H): 5 (Auto)
  // 11DA171804001E11DA171800630401000010350020E7
  uint8_t on_fan_17_max_auto[22] = {
      0x11, 0xDA, 0x17, 0x18, 0x04, 0x00, 0x1E,
      0x11, 0xDA, 0x17, 0x18, 0x00, 0x63, 0x04, 0x01, 0x00, 0x00, 0x10, 0x35,
      0x00, 0x20, 0xE7};
  // Power: On, Mode: 2 (Dry), Temp: 17C, Fan: 3 (Max), Swing(H): 5 (Auto)
  // 11DA171804001E11DA17180023047100001035002017
  uint8_t on_dry_17_max_auto[22] = {
      0x11, 0xDA, 0x17, 0x18, 0x04, 0x00, 0x1E,
      0x11, 0xDA, 0x17, 0x18, 0x00, 0x23, 0x04, 0x71, 0x00, 0x00, 0x10, 0x35,
      0x00, 0x20, 0x17};
  // Power: On, Mode: 7 (Cool), Temp: 25C, Fan: 3 (Max), Swing(H): 5 (Auto)
  // 11DA171804001E11DA17180073042100002035002027
  uint8_t on_cool_25_max_auto_v2[22] = {
      0x11, 0xDA, 0x17, 0x18, 0x04, 0x00, 0x1E,
      0x11, 0xDA, 0x17, 0x18, 0x00, 0x73, 0x04, 0x21, 0x00, 0x00, 0x20, 0x35,
      0x00, 0x20, 0x27};
  ac.setMode(kDaikin176Cool);
  ac.setPower(true);
  ac.setTemp(25);
  ac.setFan(kDaikin176FanMax);
  ac.setSwingHorizontal(true);
  EXPECT_STATE_EQ(on_cool_25_max_auto, ac.getRaw(), kDaikin176Bits);
  ac.setMode(kDaikinFan);
  EXPECT_STATE_EQ(on_fan_17_max_auto, ac.getRaw(), kDaikin176Bits);
  ac.setMode(kDaikinDry);
  EXPECT_STATE_EQ(on_dry_17_max_auto, ac.getRaw(), kDaikin176Bits);
  ac.setMode(kDaikin176Cool);
  EXPECT_STATE_EQ(on_cool_25_max_auto_v2, ac.getRaw(), kDaikin176Bits);
}

// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/827
// Data from:
//   https://docs.google.com/spreadsheets/d/1-YJnHyzy6bId5QmjTEZuw8_wSufESoIl-L_VEF-o8lM/edit?usp=sharing
TEST(TestDecodeDaikin128, RealExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  uint16_t rawData[265] = {
      9846, 9794, 9848, 9796, 4638, 2512, 348, 382, 352, 954, 352, 956, 352,
      382, 352, 956, 352, 384, 352, 382, 352, 386, 352, 382, 352, 954, 352, 384,
      352, 382, 352, 954, 352, 384, 352, 382, 352, 386, 352, 382, 352, 382, 354,
      382, 354, 382, 352, 382, 352, 954, 352, 382, 352, 384, 352, 954, 352, 382,
      352, 382, 352, 954, 352, 954, 354, 382, 352, 382, 352, 386, 352, 954, 354,
      954, 352, 954, 352, 384, 352, 382, 352, 382, 352, 954, 352, 384, 354, 382,
      352, 954, 352, 382, 352, 382, 352, 382, 352, 956, 352, 382, 354, 384, 354,
      382, 354, 954, 352, 954, 352, 382, 352, 382, 352, 954, 352, 382, 352, 384,
      354, 954, 352, 382, 352, 954, 352, 954, 352, 382, 352, 954, 352, 382, 352,
      956, 352, 20306, 376, 954, 352, 384, 352, 382, 352, 382, 354, 382, 352,
      954, 352, 382, 352, 958, 352, 384, 352, 382, 352, 382, 352, 382, 352, 382,
      354, 382, 352, 382, 352, 386, 352, 382, 352, 382, 354, 382, 352, 384, 352,
      382, 352, 382, 352, 382, 354, 384, 354, 382, 354, 382, 354, 382, 352, 382,
      352, 382, 352, 382, 352, 382, 352, 386, 354, 382, 352, 382, 352, 382, 352,
      382, 352, 382, 352, 382, 354, 382, 352, 384, 352, 382, 354, 382, 354, 382,
      354, 382, 352, 382, 354, 382, 354, 382, 354, 384, 354, 382, 354, 382, 352,
      382, 352, 382, 354, 382, 352, 382, 352, 382, 352, 386, 354, 952, 354, 954,
      352, 382, 352, 954, 354, 382, 352, 382, 354, 382, 354, 382, 4618
      };  // UNKNOWN DBA1F5E3
  uint8_t expectedState[kDaikin128StateLength] = {
      // 8 bytes
      0x16, 0x12, 0x20, 0x19, 0x47, 0x22, 0x26, 0xAD,
      // 8 bytes
      0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B};

  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 265, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(DAIKIN128, irsend.capture.decode_type);
  ASSERT_EQ(kDaikin128Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Power Toggle: On, Mode: 2 (Cool), Temp: 26C, Fan: 1 (Auto), "
      "Powerful: Off, Quiet: Off, Swing(V): On, Sleep: Off, "
      "Econo: Off, Clock: 19:20, "
      "On Timer: Off, On Timer: 07:30, Off Timer: Off, Off Timer: 22:00, "
      "Light Toggle: 0 (Off)",
      IRAcUtils::resultAcToString(&irsend.capture));
}

// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/827
// Data from:
//   https://docs.google.com/spreadsheets/d/1-YJnHyzy6bId5QmjTEZuw8_wSufESoIl-L_VEF-o8lM/edit?usp=sharing
TEST(TestDecodeDaikin128, SyntheticSelfDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  uint8_t expectedState[kDaikin128StateLength] = {
      // 8 bytes
      0x16, 0x12, 0x20, 0x19, 0x47, 0x22, 0x26, 0xAD,
      // 8 bytes
      0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B};

  irsend.begin();
  irsend.reset();
  irsend.sendDaikin128(expectedState);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(DAIKIN128, irsend.capture.decode_type);
  ASSERT_EQ(kDaikin128Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
}

TEST(TestDaikin128Class, Checksums) {
  IRDaikin128 ac(0);

  uint8_t knownGood[kDaikin128StateLength] = {
      0x16, 0x12, 0x20, 0x19, 0x47, 0x22, 0x26, 0xAD,
      0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B};
  uint8_t knownBad[kDaikin128StateLength] = {
      0x16, 0x12, 0x20, 0x19, 0x47, 0x22, 0x26, 0x0D,
      0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  EXPECT_EQ(0xA, ac.calcFirstChecksum(knownGood));
  EXPECT_EQ(0x0B, ac.calcSecondChecksum(knownGood));
  EXPECT_TRUE(ac.validChecksum(knownGood));
  ac.setRaw(knownBad);
  EXPECT_STATE_EQ(knownGood, ac.getRaw(), kDaikin128Bits);
}

TEST(TestDaikin128Class, PowerToggle) {
  IRDaikin128 ac(0);
  ac.begin();

  ac.setPowerToggle(true);
  EXPECT_TRUE(ac.getPowerToggle());
  ac.setPowerToggle(false);
  EXPECT_FALSE(ac.getPowerToggle());
  ac.setPowerToggle(true);
  EXPECT_TRUE(ac.getPowerToggle());
}

TEST(TestDaikin128Class, SwingVertical) {
  IRDaikin128 ac(0);
  ac.begin();

  ac.setSwingVertical(true);
  EXPECT_TRUE(ac.getSwingVertical());
  ac.setSwingVertical(false);
  EXPECT_FALSE(ac.getSwingVertical());
  ac.setSwingVertical(true);
  EXPECT_TRUE(ac.getSwingVertical());
}

TEST(TestDaikin128Class, Sleep) {
  IRDaikin128 ac(0);
  ac.begin();

  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
  ac.setSleep(false);
  EXPECT_FALSE(ac.getSleep());
  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
}

TEST(TestDaikin128Class, Econo) {
  IRDaikin128 ac(0);
  ac.begin();

  // Econo works in some modes
  ac.setMode(kDaikin128Heat);
  ac.setEcono(true);
  EXPECT_TRUE(ac.getEcono());
  ac.setEcono(false);
  EXPECT_FALSE(ac.getEcono());
  ac.setEcono(true);
  EXPECT_TRUE(ac.getEcono());
  // But not some some modes
  ac.setMode(kDaikin128Auto);
  EXPECT_FALSE(ac.getEcono());
  ac.setEcono(true);
  EXPECT_FALSE(ac.getEcono());
}

TEST(TestDaikin128Class, FanSpeed) {
  IRDaikin128 ac(0);
  ac.begin();

  ac.setMode(kDaikin128Cool);
  // Unexpected value should default to Auto.
  ac.setFan(0);
  EXPECT_EQ(kDaikin128FanAuto, ac.getFan());
  ac.setFan(255);
  EXPECT_EQ(kDaikin128FanAuto, ac.getFan());
  ac.setFan(5);
  EXPECT_EQ(kDaikin128FanAuto, ac.getFan());

  ac.setFan(kDaikin128FanHigh);
  EXPECT_EQ(kDaikin128FanHigh, ac.getFan());

  // Beyond Quiet should default to Auto.
  ac.setFan(kDaikin128FanQuiet + 1);
  EXPECT_EQ(kDaikin128FanAuto, ac.getFan());

  ac.setFan(kDaikin128FanMed);
  EXPECT_EQ(kDaikin128FanMed, ac.getFan());

  ac.setFan(kDaikin128FanLow);
  EXPECT_EQ(kDaikin128FanLow, ac.getFan());

  ac.setFan(kDaikin128FanPowerful);
  EXPECT_EQ(kDaikin128FanPowerful, ac.getFan());

  ac.setFan(kDaikin128FanAuto);
  EXPECT_EQ(kDaikin128FanAuto, ac.getFan());

  ac.setFan(kDaikin128FanQuiet);
  EXPECT_EQ(kDaikin128FanQuiet, ac.getFan());
}

TEST(TestDaikin128Class, OperatingMode) {
  IRDaikin128 ac(0);
  ac.begin();

  ac.setMode(0);
  EXPECT_EQ(kDaikin128Auto, ac.getMode());
  ac.setMode(kDaikin128Cool);
  EXPECT_EQ(kDaikin128Cool, ac.getMode());
  ac.setMode(kDaikin128Auto);
  EXPECT_EQ(kDaikin128Auto, ac.getMode());
  ac.setMode(kDaikin128Heat);
  EXPECT_EQ(kDaikin128Heat, ac.getMode());
  ac.setMode(kDaikin128Dry);
  EXPECT_EQ(kDaikin128Dry, ac.getMode());
  ac.setMode(kDaikin128Fan);
  EXPECT_EQ(kDaikin128Fan, ac.getMode());
  ac.setMode(3);
  EXPECT_EQ(kDaikin128Auto, ac.getMode());
  ac.setMode(kDaikin128Auto + 1);
  EXPECT_EQ(kDaikin128Auto, ac.getMode());
  ac.setMode(255);
  EXPECT_EQ(kDaikin128Auto, ac.getMode());
}

TEST(TestDaikin128Class, Quiet) {
  IRDaikin128 ac(0);
  ac.begin();

  // Quiet works in some modes
  ac.setMode(kDaikin128Cool);
  ac.setQuiet(true);
  EXPECT_TRUE(ac.getQuiet());
  ac.setQuiet(false);
  EXPECT_FALSE(ac.getQuiet());
  ac.setQuiet(true);
  EXPECT_TRUE(ac.getQuiet());
  // But not some some modes
  ac.setMode(kDaikin128Auto);
  EXPECT_FALSE(ac.getQuiet());
  ac.setQuiet(true);
  EXPECT_FALSE(ac.getQuiet());
}

TEST(TestDaikin128Class, Powerful) {
  IRDaikin128 ac(0);
  ac.begin();

  // Powerful works in some modes
  ac.setMode(kDaikin128Cool);
  ac.setPowerful(true);
  EXPECT_TRUE(ac.getPowerful());
  ac.setPowerful(false);
  EXPECT_FALSE(ac.getPowerful());
  ac.setPowerful(true);
  EXPECT_TRUE(ac.getPowerful());
  // But not some some modes
  ac.setMode(kDaikin128Auto);
  EXPECT_FALSE(ac.getPowerful());
  ac.setPowerful(true);
  EXPECT_FALSE(ac.getPowerful());
}

TEST(TestDaikin128Class, Temperature) {
  IRDaikin128 ac(0);
  ac.begin();

  ac.setTemp(0);
  EXPECT_EQ(kDaikin128MinTemp, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kDaikin128MaxTemp, ac.getTemp());

  ac.setTemp(kDaikin128MinTemp);
  EXPECT_EQ(kDaikin128MinTemp, ac.getTemp());

  ac.setTemp(kDaikin128MaxTemp);
  EXPECT_EQ(kDaikin128MaxTemp, ac.getTemp());

  ac.setTemp(kDaikin128MinTemp - 1);
  EXPECT_EQ(kDaikin128MinTemp, ac.getTemp());

  ac.setTemp(kDaikin128MaxTemp + 1);
  EXPECT_EQ(kDaikin128MaxTemp, ac.getTemp());

  ac.setTemp(kDaikin128MinTemp + 1);
  EXPECT_EQ(kDaikin128MinTemp + 1, ac.getTemp());

  ac.setTemp(21);
  EXPECT_EQ(21, ac.getTemp());

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());

  ac.setTemp(29);
  EXPECT_EQ(29, ac.getTemp());
}

// Test human readable output.
TEST(TestDaikin128Class, HumanReadable) {
  IRDaikin128 ac(0);

  ac.setPowerToggle(false);
  ac.setMode(kDaikin128Auto);
  ac.setTemp(25);
  ac.setFan(kDaikin128FanAuto);
  ac.setQuiet(false);
  ac.setPowerful(false);
  ac.setSleep(false);
  ac.setEcono(false);
  ac.setSwingVertical(true);
  EXPECT_EQ(
      "Power Toggle: Off, Mode: 10 (Auto), Temp: 25C, Fan: 1 (Auto), "
      "Powerful: Off, Quiet: Off, Swing(V): On, "
      "Sleep: Off, Econo: Off, Clock: 00:00, "
      "On Timer: Off, On Timer: 00:00, Off Timer: Off, Off Timer: 00:00, "
      "Light Toggle: 0 (Off)",
      ac.toString());
  ac.setMode(kDaikin128Cool);
  ac.setTemp(16);
  ac.setQuiet(true);
  ac.setSwingVertical(false);
  ac.setPowerToggle(true);
  ac.setSleep(true);
  ac.setEcono(true);
  ac.setClock(18 * 60 + 33);  // 18:33
  ac.setOnTimer(10 * 60);  // 10am
  ac.setOnTimerEnabled(true);
  ac.setOffTimer(21 * 60 + 30);  // 9:30pm
  ac.setOffTimerEnabled(true);
  ac.setLightToggle(kDaikin128BitWall);
  EXPECT_EQ(
      "Power Toggle: On, Mode: 2 (Cool), Temp: 16C, Fan: 9 (Quiet), "
      "Powerful: Off, Quiet: On, Swing(V): Off, "
      "Sleep: On, Econo: On, Clock: 18:33, "
      "On Timer: On, On Timer: 10:00, Off Timer: On, Off Timer: 21:30, "
      "Light Toggle: 8 (Wall)",
      ac.toString());
}

TEST(TestDaikin128Class, Clock) {
  IRDaikin128 ac(0);
  ac.begin();

  ac.setClock(0);
  EXPECT_EQ(0, ac.getClock());
  ac.setClock(23 * 60 + 59);
  EXPECT_EQ(23 * 60 + 59, ac.getClock());
  ac.setClock(23 * 60 + 59 + 1);
  EXPECT_EQ(0, ac.getClock());
  ac.setClock(24 * 60 + 99);
  EXPECT_EQ(0, ac.getClock());
}

TEST(TestDaikin128Class, Timers) {
  IRDaikin128 ac(0);
  ac.begin();

  ac.setOnTimerEnabled(false);
  EXPECT_FALSE(ac.getOnTimerEnabled());
  ac.setOnTimerEnabled(true);
  EXPECT_TRUE(ac.getOnTimerEnabled());
  ac.setOnTimer(13 * 60 + 30);
  EXPECT_EQ("13:30", irutils::minsToString(ac.getOnTimer()));
  ac.setOnTimer(13 * 60 + 31);
  EXPECT_EQ("13:30", irutils::minsToString(ac.getOnTimer()));
  ac.setOnTimer(13 * 60 + 29);
  EXPECT_EQ("13:00", irutils::minsToString(ac.getOnTimer()));
  EXPECT_TRUE(ac.getOnTimerEnabled());
  ac.setOnTimerEnabled(false);
  EXPECT_FALSE(ac.getOnTimerEnabled());

  ac.setOffTimerEnabled(false);
  EXPECT_FALSE(ac.getOffTimerEnabled());
  ac.setOffTimerEnabled(true);
  EXPECT_TRUE(ac.getOffTimerEnabled());
  ac.setOffTimer(1 * 60 + 30);
  EXPECT_EQ("01:30", irutils::minsToString(ac.getOffTimer()));
  ac.setOffTimer(23 * 60 + 31);
  EXPECT_EQ("23:30", irutils::minsToString(ac.getOffTimer()));
  ac.setOffTimer(24 * 60 + 29);
  EXPECT_EQ("00:00", irutils::minsToString(ac.getOffTimer()));
  EXPECT_TRUE(ac.getOffTimerEnabled());
  ac.setOffTimerEnabled(false);
  EXPECT_FALSE(ac.getOffTimerEnabled());
}

TEST(TestDaikin128Class, ReconstructKnownState) {
  IRDaikin128 ac(0);

  uint8_t expectedState[kDaikin128StateLength] = {
      0x16, 0x12, 0x20, 0x19, 0x47, 0x22, 0x26, 0xAD,
      0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B};

  ac.begin();
  ac.setPowerToggle(true);
  ac.setMode(kDaikin128Cool);
  ac.setTemp(26);
  ac.setFan(kDaikin128FanAuto);
  ac.setPowerful(false);
  ac.setQuiet(false);
  ac.setSwingVertical(true);
  ac.setSleep(false);
  ac.setEcono(false);
  ac.setClock(19 * 60 + 20);
  ac.setOnTimerEnabled(false);
  ac.setOnTimer(7 * 60 + 30);
  ac.setOffTimerEnabled(false);
  ac.setOffTimer(22 * 60 + 0);
  ac.setLightToggle(0);

  EXPECT_STATE_EQ(expectedState, ac.getRaw(), kDaikin128Bits);
}

// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/873
// Data from:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/873#issue-485088080
TEST(TestDecodeDaikin152, RealExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  uint16_t rawData[319] = {
      450, 420, 448, 446, 422, 444, 422, 446, 422, 446, 422, 25182, 3492, 1718,
      450, 1288, 448, 422, 446, 448, 420, 446, 422, 1290, 448, 422, 446, 446,
      422, 446, 424, 420, 448, 1290, 448, 446, 422, 1288, 448, 1288, 450, 420,
      448, 1288, 448, 1288, 450, 1288, 448, 1288, 448, 1290, 448, 446, 422, 446,
      422, 1288, 450, 446, 422, 420, 446, 446, 422, 422, 446, 446, 422, 420,
      448, 422, 446, 446, 422, 446, 422, 446, 422, 420, 446, 446, 422, 446, 422,
      422, 446, 446, 422, 422, 446, 446, 422, 446, 422, 446, 422, 446, 422, 446,
      424, 444, 424, 446, 420, 446, 422, 446, 422, 424, 444, 444, 422, 424, 444,
      1288, 450, 444, 422, 1288, 450, 1288, 450, 444, 422, 422, 446, 446, 422,
      446, 422, 446, 422, 446, 422, 422, 446, 420, 448, 444, 422, 446, 422, 446,
      422, 420, 448, 446, 422, 446, 422, 446, 422, 422, 446, 1286, 450, 422,
      448, 446, 422, 446, 422, 422, 446, 420, 446, 422, 446, 446, 422, 422, 446,
      446, 422, 422, 446, 446, 424, 444, 422, 420, 448, 446, 422, 420, 446, 446,
      422, 446, 422, 420, 448, 444, 422, 422, 448, 444, 424, 420, 446, 446, 422,
      446, 422, 422, 446, 444, 422, 446, 422, 444, 422, 446, 422, 420, 448, 446,
      422, 420, 448, 446, 422, 446, 422, 446, 422, 446, 422, 446, 422, 444, 422,
      1288, 450, 420, 448, 446, 420, 446, 422, 446, 422, 446, 424, 420, 448,
      444, 422, 422, 446, 446, 424, 420, 448, 1312, 424, 420, 448, 1288, 448,
      446, 422, 446, 424, 420, 446, 1288, 450, 1288, 450, 444, 422, 446, 422,
      422, 448, 444, 422, 420, 448, 446, 422, 1288, 448, 446, 422, 446, 422,
      444, 424, 444, 422, 446, 422, 446, 422, 420, 448, 446, 422, 420, 446,
      1290, 448, 1288, 448, 420, 446, 1288, 448, 420, 446, 1288, 450, 444, 424,
      1286, 450};  // UNKNOWN 2B9504D3
  uint8_t expectedState[kDaikin152StateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0x00, 0x00, 0x34, 0x00, 0x40, 0x00,
      0x00, 0x00, 0x00, 0x20, 0x00, 0xC5, 0x40, 0x00, 0xAB};

  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 319, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(DAIKIN152, irsend.capture.decode_type);
  ASSERT_EQ(kDaikin152Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Power: Off, Mode: 0 (Auto), Temp: 26C, Fan: 2 (UNKNOWN), Swing(V): Off, "
      "Powerful: Off, Quiet: On, Econo: Off, Sensor: Off, Comfort: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
}

// https://github.com/crankyoldgit/IRremoteESP8266/issues/873
TEST(TestDecodeDaikin152, SyntheticExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  uint8_t expectedState[kDaikin152StateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0x00, 0x00, 0x34, 0x00, 0x40, 0x00,
      0x00, 0x00, 0x00, 0x20, 0x00, 0xC5, 0x40, 0x00, 0xAB};

  irsend.begin();
  irsend.reset();
  irsend.sendDaikin152(expectedState);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(DAIKIN152, irsend.capture.decode_type);
  ASSERT_EQ(kDaikin152Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);

  irsend.reset();
  // Data from:
  //   https://github.com/crankyoldgit/IRremoteESP8266/issues/873#issuecomment-525166905
  uint8_t expectedState2[kDaikin152StateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0x00, 0x31, 0x28, 0x00, 0x3F, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0xC5, 0x00, 0x00, 0x6F};
  irsend.sendDaikin152(expectedState2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(DAIKIN152, irsend.capture.decode_type);
  ASSERT_EQ(kDaikin152Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState2, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Power: On, Mode: 3 (Cool), Temp: 20C, Fan: 1 (Low), Swing(V): On, "
      "Powerful: Off, Quiet: Off, Econo: Off, Sensor: Off, Comfort: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
}

TEST(TestDaikin2ClassNew, Issue908) {
  IRDaikin2 ac(0);
  // https://docs.google.com/spreadsheets/d/1f8EGfIbBUo2B-CzUFdrgKQprWakoYNKM80IKZN4KXQE/edit#gid=236366525&range=I8
  uint8_t fanMedium[kDaikin2StateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0x01, 0x4A, 0x42, 0xB0, 0x28, 0x0C, 0x80, 0x04,
      0xB0, 0x16, 0x24, 0x00, 0x00, 0xAA, 0xC3, 0x5E, 0x11, 0xDA, 0x27, 0x00,
      0x00, 0x09, 0x3C, 0x00, 0x50, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0xC1,
      0x90, 0x60, 0xBE};
  ac.setRaw(fanMedium);
  EXPECT_EQ(
      "Power: On, Mode: 0 (Auto), Temp: 30C, Fan: 3 (Medium), "
      "Swing(V): 3 (UpperMiddle), Swing(H): 170 (UNKNOWN), Clock: 09:46, "
      "On Timer: Off, Off Timer: Off, Sleep Timer: Off, Beep: 2 (Loud), "
      "Light: 3 (Off), Mould: On, Clean: On, Fresh: Off, Eye: Off, "
      "Eye Auto: Off, Quiet: Off, Powerful: Off, Purify: On, Econo: Off",
      ac.toString());
  ASSERT_EQ(kDaikinFanMed, ac.getFan());
  ASSERT_EQ(stdAc::fanspeed_t::kMedium, ac.toCommon().fanspeed);
  ASSERT_EQ(kDaikinFanMed, ac.convertFan(stdAc::fanspeed_t::kMedium));

  // https://docs.google.com/spreadsheets/d/1f8EGfIbBUo2B-CzUFdrgKQprWakoYNKM80IKZN4KXQE/edit#gid=236366525&range=I17
  uint8_t swingvMiddle[kDaikin2StateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0x01, 0x55, 0x42, 0xB0, 0x28, 0x0C, 0x80, 0x04,
      0xB0, 0x16, 0x24, 0x00, 0x00, 0xAA, 0xC3, 0x69, 0x11, 0xDA, 0x27, 0x00,
      0x00, 0x09, 0x3C, 0x00, 0xB0, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0xC1,
      0x90, 0x60, 0x1E};
  ac.setRaw(swingvMiddle);
  EXPECT_EQ(
      "Power: On, Mode: 0 (Auto), Temp: 30C, Fan: 11 (Quiet), "
      "Swing(V): 3 (UpperMiddle), Swing(H): 170 (UNKNOWN), Clock: 09:57, "
      "On Timer: Off, Off Timer: Off, Sleep Timer: Off, Beep: 2 (Loud), "
      "Light: 3 (Off), Mould: On, Clean: On, Fresh: Off, Eye: Off, "
      "Eye Auto: Off, Quiet: Off, Powerful: Off, Purify: On, Econo: Off",
      ac.toString());
  ASSERT_EQ(3, ac.getSwingVertical());
  ASSERT_EQ(stdAc::swingv_t::kMiddle, ac.toCommon().swingv);
  ac.setSwingVertical(4);
  ASSERT_EQ(4, ac.getSwingVertical());
  ASSERT_EQ(stdAc::swingv_t::kMiddle, ac.toCommon().swingv);
  // Either 3 or 4 is fine as they both map to stdAc::swingv_t::kMiddle.
  ASSERT_EQ(4, ac.convertSwingV(stdAc::swingv_t::kMiddle));
  // Native "swing" should convert to common's "auto".
  ac.setSwingVertical(kDaikin2SwingVSwing);
  ASSERT_EQ(kDaikin2SwingVSwing, ac.getSwingVertical());
  ASSERT_EQ(stdAc::swingv_t::kAuto, ac.toCommon().swingv);
  ASSERT_EQ(kDaikin2SwingVSwing, ac.convertSwingV(stdAc::swingv_t::kAuto));
  // Native "auto" should convert to common's "off".
  ac.setSwingVertical(kDaikin2SwingVAuto);
  ASSERT_EQ(kDaikin2SwingVAuto, ac.getSwingVertical());
  ASSERT_EQ(stdAc::swingv_t::kOff, ac.toCommon().swingv);
  ASSERT_EQ(kDaikin2SwingVAuto, ac.convertSwingV(stdAc::swingv_t::kOff));
}

// Tests for IRDaikin152 class.

TEST(TestDaikin152Class, Power) {
  IRDaikin152 ac(0);
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

TEST(TestDaikin152Class, Temperature) {
  IRDaikin152 ac(0);
  ac.begin();
  ac.setMode(kDaikinHeat);  // Heat has the widest temp range.
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

  // Special temps.
  ac.setTemp(kDaikin152FanTemp);
  EXPECT_EQ(kDaikin152FanTemp, ac.getTemp());
}

TEST(TestDaikin152Class, OperatingMode) {
  IRDaikin152 ac(0);
  ac.begin();

  ac.setMode(kDaikinAuto);
  EXPECT_EQ(kDaikinAuto, ac.getMode());

  ac.setTemp(25);

  ac.setMode(kDaikinCool);
  EXPECT_EQ(kDaikinCool, ac.getMode());

  ac.setMode(kDaikinHeat);
  EXPECT_EQ(kDaikinHeat, ac.getMode());

  ac.setMode(kDaikinFan);
  EXPECT_EQ(kDaikinFan, ac.getMode());
  EXPECT_EQ(kDaikin152FanTemp, ac.getTemp());

  ac.setMode(kDaikinDry);
  EXPECT_EQ(kDaikinDry, ac.getMode());
  EXPECT_EQ(kDaikin152DryTemp, ac.getTemp());

  ac.setMode(kDaikinFan + 1);
  EXPECT_EQ(kDaikinAuto, ac.getMode());

  ac.setMode(kDaikinAuto + 1);
  EXPECT_EQ(kDaikinAuto, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kDaikinAuto, ac.getMode());
}

TEST(TestDaikin152Class, Swing) {
  IRDaikin152 ac(0);
  ac.begin();

  ac.setSwingV(false);
  EXPECT_FALSE(ac.getSwingV());

  ac.setSwingV(true);
  EXPECT_TRUE(ac.getSwingV());

  ac.setSwingV(false);
  EXPECT_FALSE(ac.getSwingV());
}

TEST(TestDaikin152Class, QuietMode) {
  IRDaikin152 ac(0);
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

TEST(TestDaikin152Class, PowerfulMode) {
  IRDaikin152 ac(0);
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

TEST(TestDaikin152Class, EconoMode) {
  IRDaikin152 ac(0);
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

TEST(TestDaikin152Class, FanSpeed) {
  IRDaikin152 ac(0);
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

TEST(TestDaikin152Class, Comfort) {
  IRDaikin152 ac(0);
  ac.begin();

  // Comfort mode should change these settings.
  ac.setSwingV(true);
  ac.setFan(kDaikinFanMax);
  ac.setPowerful(true);

  ac.setComfort(false);
  ASSERT_FALSE(ac.getComfort());

  ac.setComfort(true);
  ASSERT_TRUE(ac.getComfort());
  ASSERT_FALSE(ac.getSwingV());
  ASSERT_FALSE(ac.getPowerful());
  ASSERT_EQ(kDaikinFanAuto, ac.getFan());

  ac.setComfort(false);
  ASSERT_FALSE(ac.getComfort());
}

TEST(TestDaikin152Class, toCommon) {
  IRDaikin152 ac(0);
  ac.setPower(true);
  ac.setMode(kDaikinCool);
  ac.setTemp(20);
  ac.setFan(kDaikinFanMax);
  ac.setSwingV(true);
  ac.setQuiet(false);
  ac.setPowerful(true);
  ac.setEcono(false);
  // Now test it.
  ASSERT_EQ(decode_type_t::DAIKIN152, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_TRUE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kAuto, ac.toCommon().swingv);
  // Unsupported.
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

TEST(TestDaikin152Class, convertFan) {
  EXPECT_EQ(kDaikinFanQuiet, IRDaikin152::convertFan(stdAc::fanspeed_t::kMin));
  EXPECT_EQ(kDaikinFanMin, IRDaikin152::convertFan(stdAc::fanspeed_t::kLow));
  EXPECT_EQ(kDaikinFanMed,
            IRDaikin152::convertFan(stdAc::fanspeed_t::kMedium));
  EXPECT_EQ(kDaikinFanMax - 1,
            IRDaikin152::convertFan(stdAc::fanspeed_t::kHigh));
  EXPECT_EQ(kDaikinFanMax, IRDaikin152::convertFan(stdAc::fanspeed_t::kMax));
  EXPECT_EQ(kDaikinFanAuto,
            IRDaikin152::convertFan(stdAc::fanspeed_t::kAuto));
}

TEST(TestDaikin152Class, BuildKnownState) {
  IRDaikin152 ac(0);
  ac.setPower(true);
  ac.setMode(kDaikinCool);
  ac.setTemp(20);
  ac.setFan(kDaikinFanMin);
  ac.setSwingV(true);
  ac.setQuiet(false);
  ac.setPowerful(false);
  ac.setEcono(false);
  ac.setSensor(false);
  ac.setComfort(false);
  EXPECT_EQ(
      "Power: On, Mode: 3 (Cool), Temp: 20C, Fan: 1 (Low), Swing(V): On, "
      "Powerful: Off, Quiet: Off, Econo: Off, Sensor: Off, Comfort: Off",
      ac.toString());
  uint8_t expectedState[kDaikin152StateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0x00, 0x31, 0x28, 0x00, 0x3F, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0xC5, 0x00, 0x00, 0x6F};
  EXPECT_STATE_EQ(expectedState, ac.getRaw(), kDaikin152Bits);
}

TEST(TestDaikin2Class, Issue1035) {
  IRDaikin2 ac(kGpioUnused);
  // https://github.com/crankyoldgit/IRremoteESP8266/issues/1035#issuecomment-580963572
  const uint8_t on_code[kDaikin2StateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0x01, 0x15, 0x43, 0x90, 0x29, 0x0C, 0x80, 0x04,
      0xC0, 0x16, 0x24, 0x00, 0x00, 0xBE, 0xC1, 0x2D, 0x11, 0xDA, 0x27, 0x00,
      0x00, 0x09, 0x2A, 0x00, 0xB0, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0xC1,
      0x90, 0x60, 0x0C};
  const uint8_t off_code[kDaikin2StateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0x01, 0x15, 0xC3, 0x90, 0x29, 0x0C, 0x80, 0x04,
      0xC0, 0x16, 0x24, 0x00, 0x00, 0xBE, 0xD1, 0xBD, 0x11, 0xDA, 0x27, 0x00,
      0x00, 0x08, 0x2A, 0x00, 0xB0, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0xC1,
      0x90, 0x60, 0x0B};

  ac.setRaw(on_code);
  EXPECT_EQ(
      "Power: On, Mode: 0 (Auto), Temp: 21C, Fan: 11 (Quiet), "
      "Swing(V): 1 (Highest), Swing(H): 190 (Auto), Clock: 13:09, "
      "On Timer: Off, Off Timer: Off, Sleep Timer: Off, Beep: 2 (Loud), "
      "Light: 1 (High), Mould: On, Clean: On, Fresh: On, Eye: Off, "
      "Eye Auto: Off, Quiet: Off, Powerful: Off, Purify: On, Econo: Off",
      ac.toString());
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_NE(ac.toCommon().mode, stdAc::opmode_t::kOff);

  ac.setRaw(off_code);
  EXPECT_EQ(
      "Power: Off, Mode: 0 (Auto), Temp: 21C, Fan: 11 (Quiet), "
      "Swing(V): 1 (Highest), Swing(H): 190 (Auto), Clock: 13:09, "
      "On Timer: Off, Off Timer: Off, Sleep Timer: Off, Beep: 2 (Loud), "
      "Light: 1 (High), Mould: On, Clean: On, Fresh: On, Eye: Off, "
      "Eye Auto: Off, Quiet: Off, Powerful: Off, Purify: On, Econo: Off",
      ac.toString());
  ASSERT_FALSE(ac.toCommon().power);
}
