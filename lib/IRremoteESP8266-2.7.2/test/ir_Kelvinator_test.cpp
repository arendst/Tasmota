// Copyright 2017 David Conran

#include "ir_Kelvinator.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendKelvinator().

// Test sending typical data only.
TEST(TestSendKelvinator, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  uint8_t kelv_code[kKelvinatorStateLength] = {
      0x19, 0x0B, 0x80, 0x50, 0x00, 0x00, 0x00, 0xE0,
      0x19, 0x0B, 0x80, 0x70, 0x00, 0x00, 0x10, 0xf0};
  irsend.reset();
  irsend.sendKelvinator(kelv_code);
  EXPECT_EQ(
      "f38000d50"
      "m9010s4505"
      "m680s1530m680s510m680s510m680s1530m680s1530m680s510m680s510m680s510"
      "m680s1530m680s1530m680s510m680s1530m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s1530"
      "m680s510m680s510m680s510m680s510m680s1530m680s510m680s1530m680s510"
      "m680s510m680s1530m680s510"
      "m680s19975"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s1530m680s1530m680s1530"
      "m680s39950"
      "m9010s4505"
      "m680s1530m680s510m680s510m680s1530m680s1530m680s510m680s510m680s510"
      "m680s1530m680s1530m680s510m680s1530m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s1530"
      "m680s510m680s510m680s510m680s510m680s1530m680s1530m680s1530m680s510"
      "m680s510m680s1530m680s510"
      "m680s19975"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s1530m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s1530m680s1530m680s1530m680s1530"
      "m680s39950",
      irsend.outputStr());
}

// Test sending with repeats.
TEST(TestSendKelvinator, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  uint8_t kelv_code[kKelvinatorStateLength] = {
      0x19, 0x0B, 0x80, 0x50, 0x00, 0x00, 0x00, 0xE0,
      0x19, 0x0B, 0x80, 0x70, 0x00, 0x00, 0x10, 0xf0};
  irsend.reset();

  irsend.sendKelvinator(kelv_code, kKelvinatorStateLength, 1);
  EXPECT_EQ(
      "f38000d50"
      "m9010s4505"
      "m680s1530m680s510m680s510m680s1530m680s1530m680s510m680s510m680s510"
      "m680s1530m680s1530m680s510m680s1530m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s1530"
      "m680s510m680s510m680s510m680s510m680s1530m680s510m680s1530m680s510"
      "m680s510m680s1530m680s510"
      "m680s19975"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s1530m680s1530m680s1530"
      "m680s39950"
      "m9010s4505"
      "m680s1530m680s510m680s510m680s1530m680s1530m680s510m680s510m680s510"
      "m680s1530m680s1530m680s510m680s1530m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s1530"
      "m680s510m680s510m680s510m680s510m680s1530m680s1530m680s1530m680s510"
      "m680s510m680s1530m680s510"
      "m680s19975"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s1530m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s1530m680s1530m680s1530m680s1530"
      "m680s39950"
      "m9010s4505"
      "m680s1530m680s510m680s510m680s1530m680s1530m680s510m680s510m680s510"
      "m680s1530m680s1530m680s510m680s1530m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s1530"
      "m680s510m680s510m680s510m680s510m680s1530m680s510m680s1530m680s510"
      "m680s510m680s1530m680s510"
      "m680s19975"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s1530m680s1530m680s1530"
      "m680s39950"
      "m9010s4505"
      "m680s1530m680s510m680s510m680s1530m680s1530m680s510m680s510m680s510"
      "m680s1530m680s1530m680s510m680s1530m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s1530"
      "m680s510m680s510m680s510m680s510m680s1530m680s1530m680s1530m680s510"
      "m680s510m680s1530m680s510"
      "m680s19975"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s1530m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s1530m680s1530m680s1530m680s1530"
      "m680s39950",
      irsend.outputStr());
}

// Test sending atypical sizes.
TEST(TestSendKelvinator, SendUnexpectedSizes) {
  IRsendTest irsend(4);
  irsend.begin();

  uint8_t kelv_short_code[15] = {0x19, 0x0B, 0x80, 0x50, 0x00, 0x00, 0x00, 0xE0,
                                 0x19, 0x0B, 0x80, 0x70, 0x00, 0x00, 0x10};
  uint8_t kelv_long_code[17] = {0x19, 0x0B, 0x80, 0x50, 0x00, 0x00,
                                0x00, 0xE0, 0x19, 0x0B, 0x80, 0x70,
                                0x00, 0x00, 0x10, 0xf0, 0x00};
  irsend.reset();
  irsend.sendKelvinator(kelv_short_code, 15);
  ASSERT_EQ("", irsend.outputStr());

  irsend.reset();
  // Shouldn't be different from the SendDataOnly. We just don't send the
  // extra data.
  irsend.sendKelvinator(kelv_long_code, 17);
  ASSERT_EQ(
      "f38000d50"
      "m9010s4505"
      "m680s1530m680s510m680s510m680s1530m680s1530m680s510m680s510m680s510"
      "m680s1530m680s1530m680s510m680s1530m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s1530"
      "m680s510m680s510m680s510m680s510m680s1530m680s510m680s1530m680s510"
      "m680s510m680s1530m680s510"
      "m680s19975"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s1530m680s1530m680s1530"
      "m680s39950"
      "m9010s4505"
      "m680s1530m680s510m680s510m680s1530m680s1530m680s510m680s510m680s510"
      "m680s1530m680s1530m680s510m680s1530m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s1530"
      "m680s510m680s510m680s510m680s510m680s1530m680s1530m680s1530m680s510"
      "m680s510m680s1530m680s510"
      "m680s19975"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s1530m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s1530m680s1530m680s1530m680s1530"
      "m680s39950",
      irsend.outputStr());
}

// Tests for IRKelvinatorAC class.

TEST(TestKelvinatorClass, Power) {
  IRKelvinatorAC irkelv(0);
  irkelv.begin();

  irkelv.on();
  EXPECT_TRUE(irkelv.getPower());

  irkelv.off();
  EXPECT_FALSE(irkelv.getPower());

  irkelv.setPower(true);
  EXPECT_TRUE(irkelv.getPower());

  irkelv.setPower(false);
  EXPECT_FALSE(irkelv.getPower());
}

TEST(TestKelvinatorClass, Temperature) {
  IRKelvinatorAC irkelv(0);
  irkelv.begin();

  irkelv.setTemp(0);
  EXPECT_EQ(kKelvinatorMinTemp, irkelv.getTemp());

  irkelv.setTemp(255);
  EXPECT_EQ(kKelvinatorMaxTemp, irkelv.getTemp());

  irkelv.setTemp(kKelvinatorMinTemp);
  EXPECT_EQ(kKelvinatorMinTemp, irkelv.getTemp());

  irkelv.setTemp(kKelvinatorMaxTemp);
  EXPECT_EQ(kKelvinatorMaxTemp, irkelv.getTemp());

  irkelv.setTemp(kKelvinatorMinTemp - 1);
  EXPECT_EQ(kKelvinatorMinTemp, irkelv.getTemp());

  irkelv.setTemp(kKelvinatorMaxTemp + 1);
  EXPECT_EQ(kKelvinatorMaxTemp, irkelv.getTemp());

  irkelv.setTemp(17);
  EXPECT_EQ(17, irkelv.getTemp());

  irkelv.setTemp(21);
  EXPECT_EQ(21, irkelv.getTemp());

  irkelv.setTemp(25);
  EXPECT_EQ(25, irkelv.getTemp());

  irkelv.setTemp(29);
  EXPECT_EQ(29, irkelv.getTemp());
}

TEST(TestKelvinatorClass, OperatingMode) {
  IRKelvinatorAC irkelv(0);
  irkelv.begin();

  irkelv.setTemp(24);
  irkelv.setMode(kKelvinatorAuto);
  EXPECT_EQ(kKelvinatorAuto, irkelv.getMode());
  EXPECT_EQ(kKelvinatorAutoTemp, irkelv.getTemp());

  irkelv.setMode(kKelvinatorCool);
  EXPECT_EQ(kKelvinatorCool, irkelv.getMode());

  irkelv.setMode(kKelvinatorHeat);
  EXPECT_EQ(kKelvinatorHeat, irkelv.getMode());

  irkelv.setTemp(24);
  irkelv.setMode(kKelvinatorDry);
  EXPECT_EQ(kKelvinatorDry, irkelv.getMode());
  EXPECT_EQ(kKelvinatorAutoTemp, irkelv.getTemp());

  irkelv.setMode(kKelvinatorFan);
  EXPECT_EQ(kKelvinatorFan, irkelv.getMode());

  irkelv.setMode(kKelvinatorHeat + 1);
  EXPECT_EQ(kKelvinatorAuto, irkelv.getMode());

  irkelv.setMode(255);
  EXPECT_EQ(kKelvinatorAuto, irkelv.getMode());
}

TEST(TestKelvinatorClass, VaneSwing) {
  IRKelvinatorAC irkelv(0);
  irkelv.begin();

  irkelv.setSwingHorizontal(true);
  irkelv.setSwingVertical(false);

  irkelv.setSwingHorizontal(true);
  EXPECT_TRUE(irkelv.getSwingHorizontal());
  EXPECT_FALSE(irkelv.getSwingVertical());

  irkelv.setSwingVertical(true);
  EXPECT_TRUE(irkelv.getSwingHorizontal());
  EXPECT_TRUE(irkelv.getSwingVertical());

  irkelv.setSwingHorizontal(false);
  EXPECT_FALSE(irkelv.getSwingHorizontal());
  EXPECT_TRUE(irkelv.getSwingVertical());

  irkelv.setSwingVertical(false);
  EXPECT_FALSE(irkelv.getSwingHorizontal());
  EXPECT_FALSE(irkelv.getSwingVertical());
}

TEST(TestKelvinatorClass, QuietMode) {
  IRKelvinatorAC irkelv(0);
  irkelv.begin();

  irkelv.setQuiet(true);
  EXPECT_TRUE(irkelv.getQuiet());

  irkelv.setQuiet(false);
  EXPECT_FALSE(irkelv.getQuiet());

  irkelv.setQuiet(true);
  EXPECT_TRUE(irkelv.getQuiet());
}

TEST(TestKelvinatorClass, IonFilter) {
  IRKelvinatorAC irkelv(0);
  irkelv.begin();

  irkelv.setIonFilter(true);
  EXPECT_TRUE(irkelv.getIonFilter());

  irkelv.setIonFilter(false);
  EXPECT_FALSE(irkelv.getIonFilter());

  irkelv.setIonFilter(true);
  EXPECT_TRUE(irkelv.getIonFilter());
}

TEST(TestKelvinatorClass, Light) {
  IRKelvinatorAC irkelv(0);
  irkelv.begin();

  irkelv.setLight(true);
  EXPECT_TRUE(irkelv.getLight());

  irkelv.setLight(false);
  EXPECT_FALSE(irkelv.getLight());

  irkelv.setLight(true);
  EXPECT_TRUE(irkelv.getLight());
}

TEST(TestKelvinatorClass, XFan) {
  IRKelvinatorAC irkelv(0);
  irkelv.begin();

  irkelv.setXFan(true);
  EXPECT_TRUE(irkelv.getXFan());

  irkelv.setXFan(false);
  EXPECT_FALSE(irkelv.getXFan());

  irkelv.setXFan(true);
  EXPECT_TRUE(irkelv.getXFan());
}

TEST(TestKelvinatorClass, TurboFan) {
  IRKelvinatorAC irkelv(0);
  irkelv.begin();

  irkelv.setTurbo(true);
  EXPECT_TRUE(irkelv.getTurbo());

  irkelv.setTurbo(false);
  EXPECT_FALSE(irkelv.getTurbo());

  irkelv.setFan(2);
  irkelv.setTurbo(true);
  EXPECT_TRUE(irkelv.getTurbo());

  // Turbo mode is turned off if the temperature is changed.
  irkelv.setFan(3);
  EXPECT_FALSE(irkelv.getTurbo());

  // But only when it is changed, not set to the same value again.
  irkelv.setTurbo(true);
  irkelv.setFan(3);
  EXPECT_TRUE(irkelv.getTurbo());
}

TEST(TestKelvinatorClass, FanSpeed) {
  IRKelvinatorAC irkelv(0);
  irkelv.begin();

  irkelv.setFan(0);
  EXPECT_EQ(0, irkelv.getFan());

  irkelv.setFan(255);
  EXPECT_EQ(kKelvinatorFanMax, irkelv.getFan());

  irkelv.setFan(kKelvinatorFanMax);
  EXPECT_EQ(kKelvinatorFanMax, irkelv.getFan());

  irkelv.setFan(kKelvinatorFanMax + 1);
  EXPECT_EQ(kKelvinatorFanMax, irkelv.getFan());

  irkelv.setFan(kKelvinatorFanMax - 1);
  EXPECT_EQ(kKelvinatorFanMax - 1, irkelv.getFan());

  irkelv.setFan(1);
  EXPECT_EQ(1, irkelv.getFan());

  irkelv.setFan(1);
  EXPECT_EQ(1, irkelv.getFan());

  irkelv.setFan(3);
  EXPECT_EQ(3, irkelv.getFan());
}

TEST(TestKelvinatorClass, Checksums) {
  uint8_t kelv_code[kKelvinatorStateLength] = {
      0x19, 0x0B, 0x80, 0x50, 0x00, 0x00, 0x00, 0xE0,
      0x19, 0x0B, 0x80, 0x70, 0x00, 0x00, 0x10, 0xf0};

  EXPECT_TRUE(IRKelvinatorAC::validChecksum(kelv_code));
  // Change the array so the checksum is invalid.
  kelv_code[0] ^= 0xFF;
  EXPECT_FALSE(IRKelvinatorAC::validChecksum(kelv_code));
  // Restore the previous change, and change another byte.
  kelv_code[0] ^= 0xFF;
  kelv_code[4] ^= 0xFF;
  EXPECT_FALSE(IRKelvinatorAC::validChecksum(kelv_code));
  kelv_code[4] ^= 0xFF;
  // Change something in the 2nd block.
  kelv_code[10] ^= 0xFF;
  EXPECT_FALSE(IRKelvinatorAC::validChecksum(kelv_code));
  kelv_code[10] ^= 0xFF;
  EXPECT_TRUE(IRKelvinatorAC::validChecksum(kelv_code));
}

TEST(TestKelvinatorClass, SetAndGetRaw) {
  IRKelvinatorAC irkelv(0);
  uint8_t initialState[kKelvinatorStateLength] = {
      0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0xA0,
      0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0xA0};
  uint8_t expectedState[kKelvinatorStateLength] = {
      0x08, 0x05, 0x20, 0x50, 0x00, 0x00, 0x00, 0x70,
      0x08, 0x05, 0x20, 0x70, 0x00, 0x00, 0x00, 0x70};

  EXPECT_STATE_EQ(initialState, irkelv.getRaw(), kKelvinatorBits);
  // toggle the power state.
  irkelv.setPower(!irkelv.getPower());
  irkelv.setTemp(21);
  irkelv.setLight(true);
  EXPECT_STATE_EQ(expectedState, irkelv.getRaw(), kKelvinatorBits);
  irkelv.setRaw(initialState);
  EXPECT_STATE_EQ(initialState, irkelv.getRaw(), kKelvinatorBits);
}

TEST(TestKelvinatorClass, HumanReadable) {
  IRKelvinatorAC irkelv(0);

  EXPECT_EQ(
      "Power: Off, Mode: 0 (Auto), Temp: 16C, Fan: 0 (Auto), Turbo: Off, "
      "Quiet: Off, XFan: Off, Ion: Off, Light: Off, "
      "Swing(H): Off, Swing(V): Off",
      irkelv.toString());
  irkelv.on();
  irkelv.setMode(kKelvinatorCool);
  irkelv.setTemp(25);
  irkelv.setFan(kKelvinatorFanMax);
  irkelv.setXFan(true);
  irkelv.setIonFilter(true);
  irkelv.setLight(true);
  irkelv.setSwingHorizontal(true);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 25C, Fan: 5 (High), Turbo: Off, "
      "Quiet: Off, XFan: On, Ion: On, Light: On, "
      "Swing(H): On, Swing(V): Off",
      irkelv.toString());
}

TEST(TestKelvinatorClass, MessageConstuction) {
  IRKelvinatorAC irkelv(0);
  IRsendTest irsend(4);
  irkelv.begin();
  irsend.begin();

  irkelv.setFan(1);
  irkelv.setMode(kKelvinatorCool);
  irkelv.setTemp(27);
  irkelv.setSwingVertical(false);
  irkelv.setSwingHorizontal(true);
  irkelv.setIonFilter(true);
  irkelv.setQuiet(false);
  irkelv.setLight(false);
  irkelv.setPower(true);
  irkelv.setTurbo(false);
  irkelv.setXFan(true);

  // Check everything for kicks.
  EXPECT_EQ(1, irkelv.getFan());
  EXPECT_EQ(kKelvinatorCool, irkelv.getMode());
  EXPECT_EQ(27, irkelv.getTemp());
  EXPECT_FALSE(irkelv.getSwingVertical());
  EXPECT_TRUE(irkelv.getSwingHorizontal());
  EXPECT_TRUE(irkelv.getIonFilter());
  EXPECT_FALSE(irkelv.getQuiet());
  EXPECT_FALSE(irkelv.getLight());
  EXPECT_TRUE(irkelv.getPower());
  EXPECT_FALSE(irkelv.getTurbo());
  EXPECT_TRUE(irkelv.getXFan());

  irsend.reset();
  irsend.sendKelvinator(irkelv.getRaw());
  EXPECT_EQ(
      "f38000d50"
      "m9010s4505"
      "m680s1530m680s510m680s510m680s1530m680s1530m680s510m680s1530m680s510"
      "m680s1530m680s1530m680s510m680s1530m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s1530m680s1530"
      "m680s510m680s510m680s510m680s510m680s1530m680s510m680s1530m680s510"
      "m680s510m680s1530m680s510"
      "m680s19975"
      "m680s510m680s510m680s510m680s510m680s1530m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s1530m680s1530m680s1530m680s1530"
      "m680s39950"
      "m9010s4505"
      "m680s1530m680s510m680s510m680s1530m680s1530m680s510m680s1530m680s510"
      "m680s1530m680s1530m680s510m680s1530m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s1530m680s1530"
      "m680s510m680s510m680s510m680s510m680s1530m680s1530m680s1530m680s510"
      "m680s510m680s1530m680s510"
      "m680s19975"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s510m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s1530m680s510m680s510m680s510"
      "m680s510m680s510m680s510m680s510m680s1530m680s1530m680s1530m680s1530"
      "m680s39950",
      irsend.outputStr());
}

// Decode a synthetic Kelvinator message.
TEST(TestDecodeKelvinator, NormalSynthetic) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  uint8_t kelv_code[kKelvinatorStateLength] = {
      0x19, 0x0B, 0x80, 0x50, 0x00, 0x00, 0x00, 0xE0,
      0x19, 0x0B, 0x80, 0x70, 0x00, 0x00, 0x10, 0xf0};
  irsend.reset();
  irsend.sendKelvinator(kelv_code);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(KELVINATOR, irsend.capture.decode_type);
  ASSERT_EQ(kKelvinatorBits, irsend.capture.bits);
  EXPECT_STATE_EQ(kelv_code, irsend.capture.state, kKelvinatorBits);
}

TEST(TestKelvinatorClass, toCommon) {
  IRKelvinatorAC ac(0);
  ac.setPower(true);
  ac.setMode(kKelvinatorCool);
  ac.setTemp(20);
  ac.setFan(kKelvinatorFanMax);
  ac.setIonFilter(true);
  ac.setXFan(true);
  ac.setQuiet(false);
  ac.setTurbo(true);
  ac.setLight(true);
  ac.setSwingHorizontal(false);
  ac.setSwingVertical(true);

  // Now test it.
  ASSERT_EQ(decode_type_t::KELVINATOR, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_TRUE(ac.toCommon().filter);
  ASSERT_TRUE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_TRUE(ac.toCommon().turbo);
  ASSERT_TRUE(ac.toCommon().light);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kAuto, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  // Unsupported.
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}
