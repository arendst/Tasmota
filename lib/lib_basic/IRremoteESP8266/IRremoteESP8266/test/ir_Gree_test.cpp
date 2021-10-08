// Copyright 2017 David Conran

#include "ir_Gree.h"
#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendGree().

// Test sending typical data only.
TEST(TestSendGreeChars, SendData) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  uint8_t gree_code[kGreeStateLength] = {0x12, 0x34, 0x56, 0x78,
                                         0x90, 0xAB, 0xCD, 0xEF};
  irsend.reset();
  irsend.sendGree(gree_code);
  EXPECT_EQ(
      "f38000d50"
      "m9000s4500"
      "m620s540m620s1600m620s540m620s540m620s1600m620s540m620s540m620s540"
      "m620s540m620s540m620s1600m620s540m620s1600m620s1600m620s540m620s540"
      "m620s540m620s1600m620s1600m620s540m620s1600m620s540m620s1600m620s540"
      "m620s540m620s540m620s540m620s1600m620s1600m620s1600m620s1600m620s540"
      "m620s540m620s1600m620s540"
      "m620s19980"
      "m620s540m620s540m620s540m620s540m620s1600m620s540m620s540m620s1600"
      "m620s1600m620s1600m620s540m620s1600m620s540m620s1600m620s540m620s1600"
      "m620s1600m620s540m620s1600m620s1600m620s540m620s540m620s1600m620s1600"
      "m620s1600m620s1600m620s1600m620s1600m620s540m620s1600m620s1600m620s1600"
      "m620s19980",
      irsend.outputStr());
}

TEST(TestSendGreeUint64, SendData) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendGree(0x1234567890ABCDEF);
  EXPECT_EQ(
      "f38000d50"
      "m9000s4500"
      "m620s540m620s1600m620s540m620s540m620s1600m620s540m620s540m620s540"
      "m620s540m620s540m620s1600m620s540m620s1600m620s1600m620s540m620s540"
      "m620s540m620s1600m620s1600m620s540m620s1600m620s540m620s1600m620s540"
      "m620s540m620s540m620s540m620s1600m620s1600m620s1600m620s1600m620s540"
      "m620s540m620s1600m620s540"
      "m620s19980"
      "m620s540m620s540m620s540m620s540m620s1600m620s540m620s540m620s1600"
      "m620s1600m620s1600m620s540m620s1600m620s540m620s1600m620s540m620s1600"
      "m620s1600m620s540m620s1600m620s1600m620s540m620s540m620s1600m620s1600"
      "m620s1600m620s1600m620s1600m620s1600m620s540m620s1600m620s1600m620s1600"
      "m620s19980",
      irsend.outputStr());
}

// Test sending with repeats.
TEST(TestSendGreeChars, SendWithRepeats) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  irsend.reset();
  uint8_t gree_code[kGreeStateLength] = {0x12, 0x34, 0x56, 0x78,
                                         0x90, 0xAB, 0xCD, 0xEF};
  irsend.reset();

  irsend.sendGree(gree_code, kGreeStateLength, 1);
  EXPECT_EQ(
      "f38000d50"
      "m9000s4500"
      "m620s540m620s1600m620s540m620s540m620s1600m620s540m620s540m620s540"
      "m620s540m620s540m620s1600m620s540m620s1600m620s1600m620s540m620s540"
      "m620s540m620s1600m620s1600m620s540m620s1600m620s540m620s1600m620s540"
      "m620s540m620s540m620s540m620s1600m620s1600m620s1600m620s1600m620s540"
      "m620s540m620s1600m620s540"
      "m620s19980"
      "m620s540m620s540m620s540m620s540m620s1600m620s540m620s540m620s1600"
      "m620s1600m620s1600m620s540m620s1600m620s540m620s1600m620s540m620s1600"
      "m620s1600m620s540m620s1600m620s1600m620s540m620s540m620s1600m620s1600"
      "m620s1600m620s1600m620s1600m620s1600m620s540m620s1600m620s1600m620s1600"
      "m620s19980"
      "m9000s4500"
      "m620s540m620s1600m620s540m620s540m620s1600m620s540m620s540m620s540"
      "m620s540m620s540m620s1600m620s540m620s1600m620s1600m620s540m620s540"
      "m620s540m620s1600m620s1600m620s540m620s1600m620s540m620s1600m620s540"
      "m620s540m620s540m620s540m620s1600m620s1600m620s1600m620s1600m620s540"
      "m620s540m620s1600m620s540"
      "m620s19980"
      "m620s540m620s540m620s540m620s540m620s1600m620s540m620s540m620s1600"
      "m620s1600m620s1600m620s540m620s1600m620s540m620s1600m620s540m620s1600"
      "m620s1600m620s540m620s1600m620s1600m620s540m620s540m620s1600m620s1600"
      "m620s1600m620s1600m620s1600m620s1600m620s540m620s1600m620s1600m620s1600"
      "m620s19980",
      irsend.outputStr());
}

TEST(TestSendGreeUint64, SendWithRepeats) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendGree(0x1234567890ABCDEF, kGreeBits, 1);
  EXPECT_EQ(
      "f38000d50"
      "m9000s4500"
      "m620s540m620s1600m620s540m620s540m620s1600m620s540m620s540m620s540"
      "m620s540m620s540m620s1600m620s540m620s1600m620s1600m620s540m620s540"
      "m620s540m620s1600m620s1600m620s540m620s1600m620s540m620s1600m620s540"
      "m620s540m620s540m620s540m620s1600m620s1600m620s1600m620s1600m620s540"
      "m620s540m620s1600m620s540"
      "m620s19980"
      "m620s540m620s540m620s540m620s540m620s1600m620s540m620s540m620s1600"
      "m620s1600m620s1600m620s540m620s1600m620s540m620s1600m620s540m620s1600"
      "m620s1600m620s540m620s1600m620s1600m620s540m620s540m620s1600m620s1600"
      "m620s1600m620s1600m620s1600m620s1600m620s540m620s1600m620s1600m620s1600"
      "m620s19980"
      "m9000s4500"
      "m620s540m620s1600m620s540m620s540m620s1600m620s540m620s540m620s540"
      "m620s540m620s540m620s1600m620s540m620s1600m620s1600m620s540m620s540"
      "m620s540m620s1600m620s1600m620s540m620s1600m620s540m620s1600m620s540"
      "m620s540m620s540m620s540m620s1600m620s1600m620s1600m620s1600m620s540"
      "m620s540m620s1600m620s540"
      "m620s19980"
      "m620s540m620s540m620s540m620s540m620s1600m620s540m620s540m620s1600"
      "m620s1600m620s1600m620s540m620s1600m620s540m620s1600m620s540m620s1600"
      "m620s1600m620s540m620s1600m620s1600m620s540m620s540m620s1600m620s1600"
      "m620s1600m620s1600m620s1600m620s1600m620s540m620s1600m620s1600m620s1600"
      "m620s19980",
      irsend.outputStr());
}

// Test sending atypical sizes.
TEST(TestSendGreeChars, SendUnexpectedSizes) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  uint8_t gree_short_code[kGreeStateLength - 1] = {0x12, 0x34, 0x56, 0x78,
                                                   0x90, 0xAB, 0xCD};
  uint8_t gree_long_code[kGreeStateLength + 1] = {0x12, 0x34, 0x56, 0x78, 0x90,
                                                  0xAB, 0xCD, 0xEF, 0x12};
  irsend.reset();
  irsend.sendGree(gree_short_code, kGreeStateLength - 1);
  ASSERT_EQ("", irsend.outputStr());

  irsend.reset();
  irsend.sendGree(gree_long_code, kGreeStateLength + 1);
  ASSERT_EQ(
      "f38000d50"
      "m9000s4500"
      "m620s540m620s1600m620s540m620s540m620s1600m620s540m620s540m620s540"
      "m620s540m620s540m620s1600m620s540m620s1600m620s1600m620s540m620s540"
      "m620s540m620s1600m620s1600m620s540m620s1600m620s540m620s1600m620s540"
      "m620s540m620s540m620s540m620s1600m620s1600m620s1600m620s1600m620s540"
      "m620s540m620s1600m620s540"
      "m620s19980"
      "m620s540m620s540m620s540m620s540m620s1600m620s540m620s540m620s1600"
      "m620s1600m620s1600m620s540m620s1600m620s540m620s1600m620s540m620s1600"
      "m620s1600m620s540m620s1600m620s1600m620s540m620s540m620s1600m620s1600"
      "m620s1600m620s1600m620s1600m620s1600m620s540m620s1600m620s1600m620s1600"
      "m620s540m620s1600m620s540m620s540m620s1600m620s540m620s540m620s540"
      "m620s19980",
      irsend.outputStr());
}

TEST(TestSendGreeUint64, SendUnexpectedSizes) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendGree(0x1234567890ABCDEF, kGreeBits - 1);
  ASSERT_EQ("", irsend.outputStr());

  irsend.reset();
  irsend.sendGree(0x1234567890ABCDEF, kGreeBits + 1);
  ASSERT_EQ("", irsend.outputStr());
}

TEST(TestSendGree, CompareUint64ToCharResults) {
  IRsendTest irsend_chars(4);
  IRsendTest irsend_uint64(0);

  uint8_t gree_code[kGreeStateLength] = {0x12, 0x34, 0x56, 0x78,
                                         0x90, 0xAB, 0xCD, 0xEF};

  irsend_chars.begin();
  irsend_uint64.begin();

  irsend_chars.reset();
  irsend_uint64.reset();
  irsend_chars.sendGree(gree_code);
  irsend_uint64.sendGree(0x1234567890ABCDEF);
  ASSERT_EQ(irsend_chars.outputStr(), irsend_uint64.outputStr());

  uint8_t gree_zero_code[kGreeStateLength] = {0x00, 0x00, 0x00, 0x00,
                                              0x00, 0x00, 0x00, 0x00};
  irsend_chars.reset();
  irsend_uint64.reset();
  irsend_chars.sendGree(gree_zero_code);
  irsend_uint64.sendGree((uint64_t)0x0);
  ASSERT_EQ(irsend_chars.outputStr(), irsend_uint64.outputStr());
}

// Tests for IRGreeAC class.

TEST(TestGreeClass, Power) {
  IRGreeAC ac(kGpioUnused);
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

TEST(TestGreeClass, Temperature) {
  IRGreeAC ac(kGpioUnused);
  ac.begin();

  ac.setMode(kGreeCool);

  ac.setTemp(0);
  EXPECT_EQ(kGreeMinTempC, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kGreeMaxTempC, ac.getTemp());

  ac.setTemp(kGreeMinTempC);
  EXPECT_EQ(kGreeMinTempC, ac.getTemp());
  EXPECT_FALSE(ac.getUseFahrenheit());

  ac.setTemp(kGreeMaxTempC);
  EXPECT_EQ(kGreeMaxTempC, ac.getTemp());

  ac.setTemp(kGreeMinTempC - 1);
  EXPECT_EQ(kGreeMinTempC, ac.getTemp());

  ac.setTemp(kGreeMaxTempC + 1);
  EXPECT_EQ(kGreeMaxTempC, ac.getTemp());

  ac.setTemp(17);
  EXPECT_EQ(17, ac.getTemp());

  ac.setTemp(21);
  EXPECT_EQ(21, ac.getTemp());

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());

  ac.setTemp(29);
  EXPECT_EQ(29, ac.getTemp());

  // Fahrenheit tests.
  ac.setTemp(kGreeMinTempF, true);
  ASSERT_TRUE(ac.getUseFahrenheit());
  EXPECT_EQ(kGreeMinTempF, ac.getTemp());

  ac.setTemp(kGreeMaxTempF, true);
  ASSERT_TRUE(ac.getUseFahrenheit());
  EXPECT_EQ(kGreeMaxTempF, ac.getTemp());

  ac.setTemp(kGreeMaxTempF + 1, true);
  ASSERT_TRUE(ac.getUseFahrenheit());
  EXPECT_EQ(kGreeMaxTempF, ac.getTemp());

  ac.setTemp(kGreeMaxTempF - 1, true);
  ASSERT_TRUE(ac.getUseFahrenheit());
  EXPECT_EQ(kGreeMaxTempF - 1, ac.getTemp());

  ac.setTemp(kGreeMaxTempF - 2, true);
  ASSERT_TRUE(ac.getUseFahrenheit());
  EXPECT_EQ(kGreeMaxTempF - 2, ac.getTemp());

  ac.setTemp(kGreeMinTempF - 1, true);
  ASSERT_TRUE(ac.getUseFahrenheit());
  EXPECT_EQ(kGreeMinTempF, ac.getTemp());

  ac.setTemp(kGreeMinTempF + 1, true);
  ASSERT_TRUE(ac.getUseFahrenheit());
  EXPECT_EQ(kGreeMinTempF + 1, ac.getTemp());

  ac.setTemp(kGreeMinTempF + 2, true);
  ASSERT_TRUE(ac.getUseFahrenheit());
  EXPECT_EQ(kGreeMinTempF + 2, ac.getTemp());

  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1121#issuecomment-628946040
  ac.setUseFahrenheit(false);
  const uint8_t state[] = {0x09, 0x01, 0x20, 0x5C, 0x00, 0x20, 0x00, 0x20};
  ac.setRaw(state);
  EXPECT_TRUE(ac.getUseFahrenheit());
  EXPECT_EQ(63, ac.getTemp());
  EXPECT_EQ(
      "Model: 2 (YBOFB), Power: On, Mode: 1 (Cool), Temp: 63F, Fan: 0 (Auto), "
      "Turbo: Off, IFeel: Off, WiFi: Off, XFan: Off, Light: On, Sleep: Off, "
      "Swing(V) Mode: Manual, Swing(V): 0 (Last), Timer: Off, "
      "Display Temp: 0 (Off)", ac.toString());
}

TEST(TestGreeClass, OperatingMode) {
  IRGreeAC ac(kGpioUnused);
  ac.begin();

  ac.setTemp(17);
  ac.setMode(kGreeAuto);  // Auto should lock the temp to 25C.
  EXPECT_EQ(kGreeAuto, ac.getMode());
  EXPECT_EQ(25, ac.getTemp());
  ac.setTemp(17);
  EXPECT_EQ(25, ac.getTemp());

  ac.setMode(kGreeCool);
  EXPECT_EQ(kGreeCool, ac.getMode());

  ac.setMode(kGreeHeat);
  EXPECT_EQ(kGreeHeat, ac.getMode());

  ASSERT_NE(kGreeFanMax, 1);
  ac.setFan(kGreeFanMax);
  ac.setMode(kGreeDry);  // Dry should lock the fan to speed 1.
  EXPECT_EQ(kGreeDry, ac.getMode());
  EXPECT_EQ(1, ac.getFan());
  ac.setFan(kGreeFanMax);
  EXPECT_EQ(1, ac.getFan());

  ac.setMode(kGreeFan);
  EXPECT_EQ(kGreeFan, ac.getMode());

  ac.setMode(kGreeHeat + 1);
  EXPECT_EQ(kGreeAuto, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kGreeAuto, ac.getMode());
}

TEST(TestGreeClass, Light) {
  IRGreeAC ac(kGpioUnused);
  ac.begin();

  ac.setLight(true);
  EXPECT_TRUE(ac.getLight());

  ac.setLight(false);
  EXPECT_FALSE(ac.getLight());

  ac.setLight(true);
  EXPECT_TRUE(ac.getLight());
}

TEST(TestGreeClass, XFan) {
  IRGreeAC ac(kGpioUnused);
  ac.begin();

  ac.setXFan(true);
  EXPECT_TRUE(ac.getXFan());

  ac.setXFan(false);
  EXPECT_FALSE(ac.getXFan());

  ac.setXFan(true);
  EXPECT_TRUE(ac.getXFan());
}

TEST(TestGreeClass, Turbo) {
  IRGreeAC ac(kGpioUnused);
  ac.begin();

  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());

  ac.setTurbo(false);
  EXPECT_FALSE(ac.getTurbo());

  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());
}

TEST(TestGreeClass, IFeel) {
  IRGreeAC ac(kGpioUnused);
  ac.begin();

  ac.setIFeel(true);
  EXPECT_TRUE(ac.getIFeel());

  ac.setIFeel(false);
  EXPECT_FALSE(ac.getIFeel());

  ac.setIFeel(true);
  EXPECT_TRUE(ac.getIFeel());

  // https://github.com/crankyoldgit/IRremoteESP8266/pull/770#issuecomment-504992209
  uint8_t on[8] = {0x08, 0x09, 0x60, 0x50, 0x00, 0x44, 0x00, 0xF0};
  uint8_t off[8] = {0x08, 0x09, 0x60, 0x50, 0x00, 0x40, 0x00, 0xF0};
  ac.setRaw(off);
  EXPECT_FALSE(ac.getIFeel());
  ac.setRaw(on);
  EXPECT_TRUE(ac.getIFeel());
}

TEST(TestGreeClass, WiFi) {
  IRGreeAC ac(kGpioUnused);
  ac.begin();

  ac.setWiFi(true);
  EXPECT_TRUE(ac.getWiFi());

  ac.setWiFi(false);
  EXPECT_FALSE(ac.getWiFi());

  ac.setWiFi(true);
  EXPECT_TRUE(ac.getWiFi());

  // https://github.com/crankyoldgit/IRremoteESP8266/pull/770#issuecomment-504992209
  uint8_t on[8] = {0x09, 0x09, 0x60, 0x50, 0x00, 0x40, 0x00, 0x00};
  uint8_t off[8] = {0x09, 0x09, 0x60, 0x50, 0x00, 0x00, 0x00, 0xC0};
  ac.setRaw(off);
  EXPECT_FALSE(ac.getWiFi());
  ac.setRaw(on);
  EXPECT_TRUE(ac.getWiFi());
}

TEST(TestGreeClass, Sleep) {
  IRGreeAC ac(kGpioUnused);
  ac.begin();

  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());

  ac.setSleep(false);
  EXPECT_FALSE(ac.getSleep());

  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
}

TEST(TestGreeClass, FanSpeed) {
  IRGreeAC ac(kGpioUnused);
  ac.begin();

  ac.setFan(0);
  EXPECT_EQ(0, ac.getFan());

  ac.setFan(255);
  EXPECT_EQ(kGreeFanMax, ac.getFan());

  ac.setFan(kGreeFanMax);
  EXPECT_EQ(kGreeFanMax, ac.getFan());

  ac.setFan(kGreeFanMax + 1);
  EXPECT_EQ(kGreeFanMax, ac.getFan());

  ac.setFan(kGreeFanMax - 1);
  EXPECT_EQ(kGreeFanMax - 1, ac.getFan());

  ac.setFan(1);
  EXPECT_EQ(1, ac.getFan());

  ac.setFan(1);
  EXPECT_EQ(1, ac.getFan());

  ac.setFan(3);
  EXPECT_EQ(3, ac.getFan());
}

TEST(TestGreeClass, VerticalSwing) {
  IRGreeAC ac(kGpioUnused);
  ac.begin();
  EXPECT_FALSE(ac.getSwingVerticalAuto());
  EXPECT_EQ(kGreeSwingLastPos, ac.getSwingVerticalPosition());

  ac.setSwingVertical(true, kGreeSwingAuto);
  EXPECT_TRUE(ac.getSwingVerticalAuto());
  EXPECT_EQ(kGreeSwingAuto, ac.getSwingVerticalPosition());

  ac.setSwingVertical(false, kGreeSwingMiddle);
  EXPECT_FALSE(ac.getSwingVerticalAuto());
  EXPECT_EQ(kGreeSwingMiddle, ac.getSwingVerticalPosition());

  ac.setSwingVertical(true, kGreeSwingDownAuto);
  EXPECT_TRUE(ac.getSwingVerticalAuto());
  EXPECT_EQ(kGreeSwingDownAuto, ac.getSwingVerticalPosition());

  // Out of bounds.
  ac.setSwingVertical(false, 255);
  EXPECT_FALSE(ac.getSwingVerticalAuto());
  EXPECT_EQ(kGreeSwingLastPos, ac.getSwingVerticalPosition());
  ac.setSwingVertical(false, kGreeSwingAuto);
  EXPECT_FALSE(ac.getSwingVerticalAuto());
  EXPECT_EQ(kGreeSwingLastPos, ac.getSwingVerticalPosition());

  ac.setSwingVertical(true, 255);
  EXPECT_TRUE(ac.getSwingVerticalAuto());
  EXPECT_EQ(kGreeSwingAuto, ac.getSwingVerticalPosition());
  ac.setSwingVertical(true, kGreeSwingDown);
  EXPECT_TRUE(ac.getSwingVerticalAuto());
  EXPECT_EQ(kGreeSwingAuto, ac.getSwingVerticalPosition());
}

TEST(TestGreeClass, SetAndGetRaw) {
  IRGreeAC ac(kGpioUnused);
  uint8_t initialState[kGreeStateLength] = {0x00, 0x09, 0x20, 0x50,
                                            0x00, 0x20, 0x00, 0x50};
  uint8_t expectedState[kGreeStateLength] = {0xA9, 0x05, 0xD0, 0x50,
                                             0x00, 0x20, 0x00, 0xA0};

  EXPECT_STATE_EQ(initialState, ac.getRaw(), kGreeBits);

  // toggle the power state.
  ac.setPower(!ac.getPower());
  ac.setMode(kGreeCool);
  ac.setTemp(21);
  ac.setFan(2);
  ac.setLight(false);
  ac.setTurbo(true);
  ac.setSleep(true);
  ac.setXFan(true);

  EXPECT_EQ(kGreeCool, ac.getMode());
  EXPECT_EQ(21, ac.getTemp());
  EXPECT_EQ(2, ac.getFan());
  EXPECT_FALSE(ac.getLight());
  EXPECT_TRUE(ac.getTurbo());
  EXPECT_TRUE(ac.getSleep());
  EXPECT_TRUE(ac.getXFan());

  EXPECT_STATE_EQ(expectedState, ac.getRaw(), kGreeBits);
  ac.setRaw(initialState);
  EXPECT_STATE_EQ(initialState, ac.getRaw(), kGreeBits);
}

TEST(TestGreeClass, HumanReadable) {
  IRGreeAC ac(kGpioUnused);

  EXPECT_EQ(
      "Model: 1 (YAW1F), Power: Off, Mode: 0 (Auto), Temp: 25C, Fan: 0 (Auto), "
      "Turbo: Off, IFeel: Off, WiFi: Off, XFan: Off, Light: On, Sleep: Off, "
      "Swing(V) Mode: Manual, Swing(V): 0 (Last), "
      "Timer: Off, Display Temp: 0 (Off)",
      ac.toString());
  ac.on();
  ac.setMode(kGreeCool);
  ac.setTemp(kGreeMinTempC);
  ac.setFan(kGreeFanMax);
  ac.setXFan(true);
  ac.setSleep(true);
  ac.setLight(false);
  ac.setTurbo(true);
  ac.setIFeel(true);
  ac.setWiFi(true);
  ac.setSwingVertical(true, kGreeSwingAuto);
  ac.setTimer(12 * 60 + 30);
  ac.setDisplayTempSource(3);
  EXPECT_EQ(
      "Model: 1 (YAW1F), Power: On, Mode: 1 (Cool), Temp: 16C, Fan: 3 (High), "
      "Turbo: On, IFeel: On, WiFi: On, XFan: On, Light: Off, Sleep: On, "
      "Swing(V) Mode: Auto, Swing(V): 1 (Auto), Timer: 12:30, "
      "Display Temp: 3 (Outside)",
      ac.toString());
}

// Tests for decodeGree().

// Decode a synthetic Gree message.
TEST(TestDecodeGree, NormalSynthetic) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  uint8_t gree_code[kGreeStateLength] = {0x00, 0x09, 0x20, 0x50,
                                         0x00, 0x20, 0x00, 0x50};

  irsend.reset();
  irsend.sendGree(gree_code);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(GREE, irsend.capture.decode_type);
  ASSERT_EQ(kGreeBits, irsend.capture.bits);
  EXPECT_STATE_EQ(gree_code, irsend.capture.state, kGreeBits);
}

// Decode a real Gree message.
TEST(TestDecodeGree, NormalRealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  IRGreeAC ac(kGpioUnused);
  irsend.begin();

  uint8_t gree_code[kGreeStateLength] = {0x19, 0x0A, 0x60, 0x50,
                                         0x02, 0x23, 0x00, 0xF0};

  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/386
  uint16_t rawData[139] = {
      9008, 4496,  644, 1660, 676, 530,  648, 558,  672, 1636, 646, 1660,
      644,  556,   650, 584,  626, 560,  644, 580,  628, 1680, 624, 560,
      648,  1662,  644, 582,  648, 536,  674, 530,  646, 580,  628, 560,
      670,  532,   646, 562,  644, 556,  672, 536,  648, 1662, 646, 1660,
      652,  554,   644, 558,  672, 538,  644, 560,  668, 560,  648, 1638,
      668,  536,   644, 1660, 668, 532,  648, 560,  648, 1660, 674, 554,
      622,  19990, 646, 580,  624, 1660, 648, 556,  648, 558,  674, 556,
      622,  560,   644, 564,  668, 536,  646, 1662, 646, 1658, 672, 534,
      648,  558,   644, 562,  648, 1662, 644, 584,  622, 558,  648, 562,
      668,  534,   670, 536,  670, 532,  672, 536,  646, 560,  646, 558,
      648,  558,   670, 534,  650, 558,  646, 560,  646, 560,  668, 1638,
      646,  1662,  646, 1660, 646, 1660, 648};  // Issue #386

  irsend.reset();
  irsend.sendRaw(rawData, 139, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(GREE, irsend.capture.decode_type);
  ASSERT_EQ(kGreeBits, irsend.capture.bits);
  EXPECT_STATE_EQ(gree_code, irsend.capture.state, kGreeBits);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Model: 1 (YAW1F), Power: On, Mode: 1 (Cool), Temp: 26C, Fan: 1 (Low), "
      "Turbo: Off, IFeel: Off, WiFi: Off, XFan: Off, Light: On, Sleep: Off, "
      "Swing(V) Mode: Manual, Swing(V): 2 (UNKNOWN), Timer: Off, "
      "Display Temp: 3 (Outside)",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

TEST(TestGreeClass, toCommon) {
  IRGreeAC ac(kGpioUnused);
  ac.setPower(true);
  ac.setMode(kGreeCool);
  ac.setTemp(20);
  ac.setFan(kGreeFanMax);
  ac.setSwingVertical(false, kGreeSwingUp);
  ac.setTurbo(true);
  ac.setXFan(true);
  ac.setLight(true);
  ac.setSleep(true);
  // Now test it.
  ASSERT_EQ(decode_type_t::GREE, ac.toCommon().protocol);
  ASSERT_EQ(gree_ac_remote_model_t::YAW1F, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_TRUE(ac.toCommon().turbo);
  ASSERT_TRUE(ac.toCommon().clean);
  ASSERT_TRUE(ac.toCommon().light);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kHighest, ac.toCommon().swingv);
  ASSERT_EQ(0, ac.toCommon().sleep);
  // Unsupported.
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

TEST(TestGreeClass, Issue814Power) {
  IRGreeAC ac(kGpioUnused);
  ac.begin();

  // https://github.com/crankyoldgit/IRremoteESP8266/issues/814#issuecomment-511263921
  uint8_t YBOFB_on[8] = {0x59, 0x07, 0x20, 0x50, 0x01, 0x20, 0x00, 0xC0};
  uint8_t off[8] = {0x51, 0x07, 0x20, 0x50, 0x01, 0x20, 0x00, 0x40};

  ac.on();
  EXPECT_EQ(gree_ac_remote_model_t::YAW1F, ac.getModel());
  ac.setRaw(off);
  EXPECT_FALSE(ac.getPower());
  ac.setRaw(YBOFB_on);
  EXPECT_TRUE(ac.getPower());
  EXPECT_EQ(gree_ac_remote_model_t::YBOFB, ac.getModel());
  EXPECT_EQ(
      "Model: 2 (YBOFB), Power: On, Mode: 1 (Cool), Temp: 23C, Fan: 1 (Low), "
      "Turbo: Off, IFeel: Off, WiFi: Off, XFan: Off, Light: On, Sleep: Off, "
      "Swing(V) Mode: Auto, Swing(V): 1 (Auto), Timer: Off, "
      "Display Temp: 0 (Off)",
      ac.toString());
  ac.off();
  EXPECT_STATE_EQ(off, ac.getRaw(), kGreeBits);
  ac.on();
  EXPECT_STATE_EQ(YBOFB_on, ac.getRaw(), kGreeBits);
  uint8_t YAW1F_on[8] = {0x59, 0x07, 0x60, 0x50, 0x01, 0x20, 0x00, 0xC0};
  ac.setModel(gree_ac_remote_model_t::YAW1F);
  EXPECT_STATE_EQ(YAW1F_on, ac.getRaw(), kGreeBits);
  ac.off();
  EXPECT_STATE_EQ(off, ac.getRaw(), kGreeBits);
  ac.setModel(gree_ac_remote_model_t::YBOFB);
  ac.on();
  EXPECT_STATE_EQ(YBOFB_on, ac.getRaw(), kGreeBits);
}

TEST(TestGreeClass, Timer) {
  IRGreeAC ac(kGpioUnused);
  ac.begin();

  ac.setTimer(0);
  EXPECT_FALSE(ac.getTimerEnabled());
  EXPECT_EQ(0, ac.getTimer());

  ac.setTimer(29);
  EXPECT_FALSE(ac.getTimerEnabled());
  EXPECT_EQ(0, ac.getTimer());

  ac.setTimer(30);
  EXPECT_TRUE(ac.getTimerEnabled());
  EXPECT_EQ(30, ac.getTimer());

  ac.setTimer(60);
  EXPECT_TRUE(ac.getTimerEnabled());
  EXPECT_EQ(60, ac.getTimer());

  ac.setTimer(90);
  EXPECT_TRUE(ac.getTimerEnabled());
  EXPECT_EQ(90, ac.getTimer());

  ac.setTimer(10 * 60);
  EXPECT_TRUE(ac.getTimerEnabled());
  EXPECT_EQ(10 * 60, ac.getTimer());

  ac.setTimer(23 * 60 + 59);
  EXPECT_TRUE(ac.getTimerEnabled());
  EXPECT_EQ(23 * 60 + 30, ac.getTimer());

  ac.setTimer(24 * 60 + 1);
  EXPECT_TRUE(ac.getTimerEnabled());
  EXPECT_EQ(24 * 60, ac.getTimer());

  ac.setTimer(24 * 60 + 30);
  EXPECT_TRUE(ac.getTimerEnabled());
  EXPECT_EQ(24 * 60, ac.getTimer());
}

TEST(TestGreeClass, DisplayTempSource) {
  IRGreeAC ac(kGpioUnused);
  ac.begin();

  ac.setDisplayTempSource(1);
  EXPECT_EQ(1, ac.getDisplayTempSource());

  ac.setDisplayTempSource(2);
  EXPECT_EQ(2, ac.getDisplayTempSource());

  ac.setDisplayTempSource(3);
  EXPECT_EQ(3, ac.getDisplayTempSource());

  ac.setDisplayTempSource(1);
  EXPECT_EQ(1, ac.getDisplayTempSource());

  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1118#issuecomment-627674014
  const uint8_t state[8] = {0x4C, 0x04, 0x60, 0x50, 0x01, 0x02, 0x00, 0xA0};
  ac.setRaw(state);
  EXPECT_EQ(2, ac.getDisplayTempSource());
}
