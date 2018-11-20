// Copyright 2018 David Conran

#include "ir_Hitachi.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendHitachiAC().

// Test sending typical data only.
TEST(TestSendHitachiAC, SendData) {
  IRsendTest irsend(0);
  irsend.begin();

  uint8_t hitachi_code[kHitachiAcStateLength] = {
      0x80, 0x08, 0x0C, 0x02, 0xFD, 0x80, 0x7F, 0x88, 0x48, 0x80,
      0x20, 0x04, 0x00, 0x80, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0xAC};
  irsend.reset();
  irsend.sendHitachiAC(hitachi_code);
  EXPECT_EQ(
      "m3300s1700"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s1250m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s1250m400s1250m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s1250m400s500"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s500m400s1250"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s1250m400s500m400s500m400s500m400s1250m400s500m400s500m400s500"
      "m400s500m400s1250m400s500m400s500m400s1250m400s500m400s500m400s500"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s1250m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s1250m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s1250"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s500m400s1250m400s500m400s1250m400s1250m400s500m400s500"
      "m400s100000",
      irsend.outputStr());
}

// Test sending with repeats.
TEST(TestSendHitachiAC, SendWithRepeats) {
  IRsendTest irsend(0);
  irsend.begin();

  uint8_t hitachi_code[kHitachiAcStateLength] = {
      0x80, 0x08, 0x0C, 0x02, 0xFD, 0x80, 0x7F, 0x88, 0x48, 0x80,
      0x20, 0x04, 0x00, 0x80, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0xAC};
  irsend.reset();

  irsend.sendHitachiAC(hitachi_code, kHitachiAcStateLength, 1);
  EXPECT_EQ(
      "m3300s1700"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s1250m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s1250m400s1250m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s1250m400s500"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s500m400s1250"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s1250m400s500m400s500m400s500m400s1250m400s500m400s500m400s500"
      "m400s500m400s1250m400s500m400s500m400s1250m400s500m400s500m400s500"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s1250m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s1250m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s1250"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s500m400s1250m400s500m400s1250m400s1250m400s500m400s500"
      "m400s100000"
      "m3300s1700"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s1250m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s1250m400s1250m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s1250m400s500"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s500m400s1250"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s1250m400s500m400s500m400s500m400s1250m400s500m400s500m400s500"
      "m400s500m400s1250m400s500m400s500m400s1250m400s500m400s500m400s500"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s1250m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s1250m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s1250"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s500m400s1250m400s500m400s1250m400s1250m400s500m400s500"
      "m400s100000",
      irsend.outputStr());
}

// Test sending atypical sizes.
TEST(TestSendHitachiAC, SendUnexpectedSizes) {
  IRsendTest irsend(0);
  irsend.begin();

  uint8_t hitachi_short_code[kHitachiAcStateLength - 1] = {
      0x80, 0x08, 0x0C, 0x02, 0xFD, 0x80, 0x7F, 0x88, 0x48,
      0x80, 0x20, 0x04, 0x00, 0x80, 0x00, 0x00, 0x00, 0x01,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00};
  uint8_t hitachi_long_code[kHitachiAcStateLength + 1] = {
      0x80, 0x08, 0x0C, 0x02, 0xFD, 0x80, 0x7F, 0x88, 0x48, 0x80,
      0x20, 0x04, 0x00, 0x80, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0xAC, 0xFF};
  irsend.reset();
  irsend.sendHitachiAC(hitachi_short_code, kHitachiAcStateLength - 1);
  ASSERT_EQ("", irsend.outputStr());

  irsend.reset();
  irsend.sendHitachiAC(hitachi_long_code, kHitachiAcStateLength + 1);
  ASSERT_EQ(
      "m3300s1700"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s1250m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s1250m400s1250m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s1250m400s500"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s500m400s1250"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s1250m400s500m400s500m400s500m400s1250m400s500m400s500m400s500"
      "m400s500m400s1250m400s500m400s500m400s1250m400s500m400s500m400s500"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s1250m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s1250m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s1250"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s500m400s1250m400s500m400s1250m400s1250m400s500m400s500"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s100000",
      irsend.outputStr());
}

// Tests for IRHitachiAc class.
TEST(TestIRHitachiAcClass, SetAndGetPower) {
  IRHitachiAc ac(0);
  ac.on();
  EXPECT_TRUE(ac.getPower());
  ac.off();
  EXPECT_FALSE(ac.getPower());
  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());
  ac.setPower(false);
  EXPECT_FALSE(ac.getPower());
}

TEST(TestIRHitachiAcClass, SetAndGetSwing) {
  IRHitachiAc ac(0);
  ac.setSwingVertical(true);
  ac.setSwingHorizontal(true);
  EXPECT_TRUE(ac.getSwingVertical());
  EXPECT_TRUE(ac.getSwingHorizontal());
  ac.setSwingVertical(false);
  EXPECT_FALSE(ac.getSwingVertical());
  EXPECT_TRUE(ac.getSwingHorizontal());
  ac.setSwingVertical(true);
  EXPECT_TRUE(ac.getSwingVertical());
  EXPECT_TRUE(ac.getSwingHorizontal());
  ac.setSwingHorizontal(false);
  EXPECT_TRUE(ac.getSwingVertical());
  EXPECT_FALSE(ac.getSwingHorizontal());
  ac.setSwingHorizontal(true);
  EXPECT_TRUE(ac.getSwingHorizontal());
}

TEST(TestIRHitachiAcClass, SetAndGetTemp) {
  IRHitachiAc ac(0);
  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());
  ac.setTemp(kHitachiAcMinTemp);
  EXPECT_EQ(kHitachiAcMinTemp, ac.getTemp());
  ac.setTemp(kHitachiAcMinTemp - 1);
  EXPECT_EQ(kHitachiAcMinTemp, ac.getTemp());
  ac.setTemp(kHitachiAcMaxTemp);
  EXPECT_EQ(kHitachiAcMaxTemp, ac.getTemp());
  ac.setTemp(kHitachiAcMaxTemp + 1);
  EXPECT_EQ(kHitachiAcMaxTemp, ac.getTemp());
  ac.setTemp(64);
  EXPECT_EQ(64, ac.getTemp());
}

TEST(TestIRHitachiAcClass, SetAndGetMode) {
  IRHitachiAc ac(0);
  ac.setMode(kHitachiAcCool);
  ac.setFan(kHitachiAcFanAuto);
  EXPECT_EQ(kHitachiAcCool, ac.getMode());
  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());
  EXPECT_EQ(kHitachiAcFanAuto, ac.getFan());
  ac.setMode(kHitachiAcFan);
  EXPECT_EQ(kHitachiAcFan, ac.getMode());
  EXPECT_EQ(64, ac.getTemp());
  EXPECT_NE(kHitachiAcFanAuto, ac.getFan());
  ac.setMode(kHitachiAcHeat);
  EXPECT_EQ(25, ac.getTemp());
  EXPECT_EQ(kHitachiAcHeat, ac.getMode());
  ac.setMode(kHitachiAcAuto);
  ac.setFan(kHitachiAcFanAuto);
  EXPECT_EQ(kHitachiAcAuto, ac.getMode());
  ac.setMode(kHitachiAcDry);
  EXPECT_EQ(kHitachiAcDry, ac.getMode());
  EXPECT_NE(kHitachiAcFanAuto, ac.getFan());
}

TEST(TestIRHitachiAcClass, SetAndGetFan) {
  IRHitachiAc ac(0);
  ac.setMode(kHitachiAcCool);  // All fan options are available in this mode.
  ac.setFan(kHitachiAcFanAuto);
  EXPECT_EQ(kHitachiAcFanAuto, ac.getFan());
  ac.setFan(kHitachiAcFanLow);
  EXPECT_EQ(kHitachiAcFanLow, ac.getFan());
  ac.setFan(kHitachiAcFanHigh);
  EXPECT_EQ(kHitachiAcFanHigh, ac.getFan());
  ac.setFan(kHitachiAcFanHigh + 1);
  EXPECT_EQ(kHitachiAcFanHigh, ac.getFan());
  ac.setFan(0);
  EXPECT_EQ(kHitachiAcFanAuto, ac.getFan());

  ac.setMode(kHitachiAcFan);  // No auto-fan in Fan mode.
  EXPECT_EQ(kHitachiAcFanLow, ac.getFan());
  ac.setFan(kHitachiAcFanAuto);
  EXPECT_EQ(kHitachiAcFanLow, ac.getFan());
  ac.setFan(kHitachiAcFanHigh);
  EXPECT_EQ(kHitachiAcFanHigh, ac.getFan());

  // Only Low and one higher fan settin in Dry mode.
  ac.setMode(kHitachiAcDry);
  EXPECT_EQ(kHitachiAcFanLow + 1, ac.getFan());
  ac.setFan(kHitachiAcFanHigh);
  EXPECT_EQ(kHitachiAcFanLow + 1, ac.getFan());
  ac.setFan(kHitachiAcFanLow);
  EXPECT_EQ(kHitachiAcFanLow, ac.getFan());
  ac.setFan(kHitachiAcFanAuto);
  EXPECT_EQ(kHitachiAcFanLow, ac.getFan());
}

TEST(TestIRHitachiAcClass, HumanReadable) {
  IRHitachiAc ac(0);

  ac.setMode(kHitachiAcHeat);
  ac.setTemp(kHitachiAcMaxTemp);
  ac.on();
  ac.setFan(kHitachiAcFanHigh);
  ac.setSwingVertical(true);
  EXPECT_EQ(
      "Power: On, Mode: 3 (HEAT), Temp: 32C, Fan: 5 (HIGH), "
      "Swing (Vertical): On, Swing (Horizontal): Off",
      ac.toString());
  ac.setMode(kHitachiAcCool);
  ac.setTemp(kHitachiAcMinTemp);
  ac.setFan(kHitachiAcFanLow);
  ac.setSwingVertical(false);
  ac.setSwingHorizontal(true);
  EXPECT_EQ(
      "Power: On, Mode: 4 (COOL), Temp: 16C, Fan: 2 (LOW), "
      "Swing (Vertical): Off, Swing (Horizontal): On",
      ac.toString());
}

TEST(TestIRHitachiAcClass, ChecksumCalculation) {
  IRHitachiAc ac(0);

  const uint8_t originalstate[kHitachiAcStateLength] = {
      0x80, 0x08, 0x0C, 0x02, 0xFD, 0x80, 0x7F, 0x88, 0x48, 0x80,
      0x20, 0x04, 0x00, 0x80, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0xAC};
  uint8_t examplestate[kHitachiAcStateLength] = {
      0x80, 0x08, 0x0C, 0x02, 0xFD, 0x80, 0x7F, 0x88, 0x48, 0x80,
      0x20, 0x04, 0x00, 0x80, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0xAC};

  EXPECT_TRUE(IRHitachiAc::validChecksum(examplestate));
  EXPECT_EQ(0xAC, IRHitachiAc::calcChecksum(examplestate));

  examplestate[kHitachiAcStateLength - 1] =
      0x12;  // Make the existing checksum invalid
  EXPECT_FALSE(IRHitachiAc::validChecksum(examplestate));
  EXPECT_EQ(0xAC, IRHitachiAc::calcChecksum(examplestate));
  ac.setRaw(examplestate);
  // Extracting the state from the object should have a correct checksum.
  EXPECT_TRUE(IRHitachiAc::validChecksum(ac.getRaw()));
  EXPECT_STATE_EQ(originalstate, ac.getRaw(), kHitachiAcBits);

  examplestate[8] = 0x12;  // Force a different checksum calc.
  EXPECT_FALSE(IRHitachiAc::validChecksum(examplestate));
  EXPECT_EQ(0xFF, IRHitachiAc::calcChecksum(examplestate));
}

// Tests for decodeHitachiAC().

// Decode a synthetic HitachiAC message.
TEST(TestDecodeHitachiAC, NormalSynthetic) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  uint8_t hitachi_code[kHitachiAcStateLength] = {
      0x80, 0x08, 0x0C, 0x02, 0xFD, 0x80, 0x7F, 0x88, 0x48, 0x80,
      0x20, 0x04, 0x00, 0x80, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0xAC};

  irsend.reset();
  irsend.sendHitachiAC(hitachi_code);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(HITACHI_AC, irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(hitachi_code, irsend.capture.state, kHitachiAcBits);
}

// Decode a 'real' HitachiAC message.
TEST(TestDecodeHitachiAC, NormalRealExample1) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  uint8_t hitachi_code[kHitachiAcStateLength] = {
      0x80, 0x08, 0x0C, 0x02, 0xFD, 0x80, 0x7F, 0x88, 0x48, 0x80,
      0x20, 0x04, 0x00, 0x80, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0xAC};

  // Ref: https://github.com/markszabo/IRremoteESP8266/issues/417
  // 'On' '16c' 'auto fan' 'cooling mode'
  uint16_t rawData[451] = {
      3318, 1720, 400, 1276, 400, 432,  398, 434,  398, 434,  400, 432,
      398,  432,  398, 432,  398, 434,  398, 432,  398, 434,  400, 432,
      398,  434,  398, 1278, 398, 434,  398, 434,  396, 434,  398, 434,
      398,  432,  398, 434,  398, 432,  398, 1276, 426, 1252, 424, 408,
      424,  406,  424, 408,  426, 406,  398, 432,  398, 434,  398, 432,
      400,  432,  398, 1276, 426, 408,  424, 1252, 426, 1252, 424, 1250,
      426,  1252, 428, 1250, 426, 1252, 424, 406,  426, 1248, 428, 1252,
      426,  406,  426, 406,  424, 408,  400, 432,  400, 430,  400, 432,
      400,  430,  400, 432,  400, 1276, 400, 1276, 402, 1276, 400, 1276,
      400,  1276, 400, 1278, 400, 1276, 402, 1276, 402, 428,  402, 430,
      400,  430,  402, 1276, 400, 430,  402, 430,  400, 432,  402, 428,
      402,  1278, 400, 430,  402, 430,  402, 1276, 402, 428,  402, 430,
      402,  430,  400, 1276, 402, 430,  402, 430,  402, 430,  402, 430,
      402,  428,  402, 430,  404, 430,  402, 428,  402, 430,  402, 1276,
      402,  430,  402, 428,  402, 430,  400, 428,  402, 430,  402, 430,
      402,  430,  402, 430,  402, 428,  402, 430,  402, 1274, 402, 428,
      402,  430,  402, 430,  402, 430,  402, 430,  402, 428,  402, 428,
      402,  428,  404, 428,  404, 428,  402, 1276, 400, 430,  402, 430,
      400,  432,  400, 456,  374, 432,  400, 456,  404, 428,  404, 426,
      404,  428,  402, 428,  402, 430,  402, 430,  400, 432,  398, 434,
      376,  454,  378, 454,  380, 452,  378, 452,  404, 428,  406, 424,
      432,  398,  406, 426,  430, 402,  404, 428,  428, 402,  400, 430,
      400,  432,  398, 434,  398, 432,  398, 434,  372, 460,  374, 434,
      398,  432,  398, 434,  396, 434,  376, 456,  376, 456,  376, 456,
      376,  1300, 378, 454,  378, 452,  378, 454,  378, 454,  378, 454,
      378,  452,  378, 454,  400, 432,  402, 430,  402, 430,  402, 430,
      402,  428,  402, 430,  402, 430,  400, 430,  402, 430,  400, 432,
      400,  430,  400, 432,  400, 430,  402, 430,  400, 432,  398, 432,
      400,  430,  400, 432,  398, 432,  398, 434,  398, 432,  398, 432,
      400,  434,  398, 432,  398, 432,  398, 434,  398, 434,  396, 434,
      398,  434,  398, 432,  398, 434,  398, 432,  398, 456,  376, 454,
      376,  436,  396, 454,  376, 454,  378, 454,  376, 454,  376, 456,
      374,  458,  374, 1302, 374, 456,  374, 458,  374, 458,  376, 456,
      374,  456,  374, 456,  374, 456,  376, 456,  376, 456,  374, 458,
      374,  458,  374, 458,  372, 458,  372, 460,  374, 458,  372, 460,
      372,  460,  372, 460,  370, 462,  372, 460,  372, 460,  370, 462,
      370,  462,  356, 1320, 368, 464,  346, 1332, 356, 476,  368, 1310,
      366,  1312, 366, 464,  366, 466,  366};

  irsend.reset();
  irsend.sendRaw(rawData, 451, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decodeHitachiAC(&irsend.capture));
  EXPECT_EQ(HITACHI_AC, irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(hitachi_code, irsend.capture.state, kHitachiAcBits);
  IRHitachiAc ac(0);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: On, Mode: 4 (COOL), Temp: 16C, Fan: 1 (AUTO), "
      "Swing (Vertical): Off, Swing (Horizontal): Off",
      ac.toString());
}

// Decode another 'real' HitachiAC message.
TEST(TestDecodeHitachiAC, NormalRealExample2) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  uint8_t hitachi_code[kHitachiAcStateLength] = {
      0x80, 0x08, 0x0C, 0x02, 0xFD, 0x80, 0x7F, 0x88, 0x48, 0x10,
      0xC0, 0x02, 0x00, 0xA0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0xD0};

  // Ref: https://github.com/markszabo/IRremoteESP8266/issues/417
  // 'On' '32c' 'auto fan' 'heating mode'
  uint16_t rawData[451] = {
      3322, 1718, 400, 1278, 398, 432,  402, 430,  400, 430,  402, 430,
      402,  430,  400, 432,  400, 432,  402, 430,  400, 430,  402, 430,
      400,  430,  402, 1276, 402, 430,  400, 430,  402, 430,  402, 430,
      402,  428,  402, 430,  400, 430,  402, 1278, 400, 1274, 402, 430,
      400,  432,  400, 430,  402, 430,  402, 430,  400, 430,  400, 432,
      402,  428,  402, 1276, 404, 428,  402, 1276, 402, 1274, 402, 1274,
      404,  1272, 402, 1274, 402, 1274, 404, 430,  402, 1274, 402, 1274,
      402,  430,  402, 430,  402, 430,  400, 430,  402, 430,  404, 428,
      402,  430,  400, 430,  402, 1274, 402, 1276, 402, 1276, 402, 1276,
      400,  1276, 402, 1272, 404, 1274, 402, 1272, 404, 430,  402, 430,
      402,  430,  402, 1274, 404, 428,  402, 428,  404, 430,  402, 430,
      402,  1274, 402, 430,  402, 430,  402, 1274, 402, 430,  402, 430,
      402,  428,  404, 428,  402, 430,  402, 430,  402, 1272, 406, 430,
      402,  428,  402, 430,  402, 430,  400, 1276, 400, 1276, 402, 430,
      402,  430,  402, 430,  402, 430,  402, 428,  402, 430,  402, 430,
      402,  430,  402, 430,  402, 430,  402, 430,  400, 430,  402, 1272,
      404,  430,  400, 430,  402, 430,  402, 428,  404, 430,  400, 430,
      402,  430,  402, 430,  402, 428,  402, 1276, 402, 428,  402, 1276,
      402,  430,  402, 428,  402, 430,  402, 430,  402, 430,  402, 428,
      402,  430,  402, 430,  402, 430,  402, 430,  402, 428,  402, 428,
      404,  430,  402, 428,  402, 430,  402, 428,  402, 430,  402, 432,
      402,  428,  402, 430,  402, 430,  402, 430,  400, 430,  400, 430,
      402,  430,  402, 430,  402, 426,  404, 430,  402, 430,  400, 428,
      404,  428,  404, 432,  398, 432,  400, 456,  376, 456,  376, 456,
      376,  1276, 402, 454,  378, 456,  376, 454,  378, 454,  402, 430,
      376,  454,  376, 456,  400, 430,  378, 454,  400, 432,  402, 430,
      376,  454,  376, 456,  376, 454,  400, 432,  374, 458,  400, 430,
      398,  434,  374, 456,  374, 456,  374, 458,  374, 458,  374, 456,
      352,  482,  372, 460,  374, 458,  364, 468,  352, 480,  372, 460,
      352,  480,  352, 480,  352, 480,  352, 480,  352, 454,  400, 432,
      376,  480,  374, 456,  374, 456,  378, 430,  404, 452,  376, 430,
      404,  428,  406, 426,  406, 426,  404, 426,  406, 428,  406, 426,
      430,  400,  430, 1244, 408, 424,  434, 398,  432, 400,  406, 424,
      432,  400,  406, 426,  430, 400,  432, 400,  432, 400,  430, 402,
      428,  402,  404, 428,  404, 426,  406, 426,  406, 426,  404, 428,
      428,  402,  404, 428,  404, 428,  402, 428,  404, 428,  402, 428,
      404,  428,  404, 1274, 402, 1274, 404, 428,  400, 1276, 402, 430,
      402,  430,  402, 430,  402, 430,  400};

  irsend.reset();
  irsend.sendRaw(rawData, 451, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(HITACHI_AC, irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(hitachi_code, irsend.capture.state, kHitachiAcBits);
  IRHitachiAc ac(0);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: On, Mode: 3 (HEAT), Temp: 32C, Fan: 5 (HIGH), "
      "Swing (Vertical): Off, Swing (Horizontal): Off",
      ac.toString());
}

// Tests for sendHitachiAC1().

// Test sending typical data only.
TEST(TestSendHitachiAC1, SendData) {
  IRsendTest irsend(0);
  irsend.begin();

  uint8_t hitachi_code[kHitachiAc1StateLength] = {0xB2, 0xAE, 0x4D, 0x51, 0xF0,
                                                  0x61, 0x84, 0x00, 0x00, 0x00,
                                                  0x00, 0x30, 0xB8};
  irsend.reset();
  irsend.sendHitachiAC1(hitachi_code);
  EXPECT_EQ(
      "m3400s3400"
      "m400s1250m400s500m400s1250m400s1250m400s500m400s500m400s1250m400s500"
      "m400s1250m400s500m400s1250m400s500m400s1250m400s1250m400s1250m400s500"
      "m400s500m400s1250m400s500m400s500m400s1250m400s1250m400s500m400s1250"
      "m400s500m400s1250m400s500m400s1250m400s500m400s500m400s500m400s1250"
      "m400s1250m400s1250m400s1250m400s1250m400s500m400s500m400s500m400s500"
      "m400s500m400s1250m400s1250m400s500m400s500m400s500m400s500m400s1250"
      "m400s1250m400s500m400s500m400s500m400s500m400s1250m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s1250m400s1250m400s500m400s500m400s500m400s500"
      "m400s1250m400s500m400s1250m400s1250m400s1250m400s500m400s500m400s500"
      "m400s100000",
      irsend.outputStr());
}

// Decode a 'real' HitachiAC1 message.
TEST(TestDecodeHitachiAC1, NormalRealExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  uint8_t hitachi_code[kHitachiAc1StateLength] = {0xB2, 0xAE, 0x4D, 0x51, 0xF0,
                                                  0x61, 0x84, 0x00, 0x00, 0x00,
                                                  0x00, 0x10, 0x98};

  // Ref: https://github.com/markszabo/IRremoteESP8266/issues/453
  uint16_t rawData[211] = {
      3400, 3350, 450, 1250, 450, 400,  400, 1300, 400, 1300, 400, 400,
      450,  400,  400, 1300, 400, 400,  400, 1300, 400, 400,  450, 1250,
      400,  450,  400, 1300, 400, 1250, 450, 1250, 450, 400,  400, 450,
      400,  1250, 450, 400,  400, 400,  400, 1300, 400, 1300, 400, 400,
      450,  1250, 450, 400,  400, 1300, 400, 400,  450, 1250, 400, 400,
      450,  400,  400, 400,  450, 1250, 400, 1300, 450, 1250, 450, 1250,
      400,  1300, 400, 400,  450, 400,  400, 450,  350, 450,  400, 400,
      400,  1300, 400, 1300, 400, 400,  450, 400,  400, 400,  450, 400,
      400,  1300, 400, 1250, 450, 400,  400, 400,  450, 400,  400, 400,
      450,  1250, 450, 400,  400, 400,  450, 400,  400, 400,  450, 400,
      400,  400,  450, 400,  400, 400,  450, 400,  400, 400,  400, 450,
      400,  400,  400, 400,  450, 400,  400, 400,  450, 400,  400, 450,
      400,  400,  400, 400,  450, 400,  400, 400,  450, 400,  400, 450,
      400,  400,  400, 400,  400, 450,  400, 400,  400, 400,  450, 400,
      400,  400,  450, 400,  400, 400,  450, 400,  400, 400,  450, 400,
      400,  400,  450, 400,  400, 1300, 400, 400,  450, 400,  400, 400,
      400,  400,  450, 1250, 450, 400,  400, 400,  450, 1250, 450, 1250,
      450,  400,  400, 400,  450, 400,  400};  // UNKNOWN 828A89E1

  irsend.reset();
  irsend.sendRaw(rawData, 211, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(HITACHI_AC1, irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAc1Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(hitachi_code, irsend.capture.state, kHitachiAc1Bits);
}

// Tests for sendHitachiAC2().

// Test sending typical data only.
TEST(TestSendHitachiAC2, SendData) {
  IRsendTest irsend(0);
  irsend.begin();

  uint8_t hitachi_code[kHitachiAc2StateLength] = {
      0x80, 0x08, 0x00, 0x02, 0xFD, 0xFF, 0x00, 0x33, 0xCC, 0x49, 0xB6,
      0x22, 0xDD, 0x01, 0xFE, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
      0xFF, 0x00, 0xFF, 0xCA, 0x35, 0x8F, 0x70, 0x00, 0xFF, 0x00, 0xFF,
      0x01, 0xFE, 0xC0, 0x3F, 0x80, 0x7F, 0x11, 0xEE, 0x00, 0xFF, 0x00,
      0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00};
  irsend.reset();
  irsend.sendHitachiAC2(hitachi_code);
  EXPECT_EQ(
      "m3300s1700"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s1250m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s1250m400s500"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s500m400s1250"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s1250m400s1250m400s500m400s500m400s1250m400s1250"
      "m400s1250m400s1250m400s500m400s500m400s1250m400s1250m400s500m400s500"
      "m400s500m400s1250m400s500m400s500m400s1250m400s500m400s500m400s1250"
      "m400s1250m400s500m400s1250m400s1250m400s500m400s1250m400s1250m400s500"
      "m400s500m400s500m400s1250m400s500m400s500m400s500m400s1250m400s500"
      "m400s1250m400s1250m400s500m400s1250m400s1250m400s1250m400s500m400s1250"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s1250"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s1250m400s1250m400s500m400s500m400s1250m400s500m400s1250m400s500"
      "m400s500m400s500m400s1250m400s1250m400s500m400s1250m400s500m400s1250"
      "m400s1250m400s500m400s500m400s500m400s1250m400s1250m400s1250m400s1250"
      "m400s500m400s1250m400s1250m400s1250m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s1250"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s500"
      "m400s1250m400s1250m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s500m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s1250m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s500m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s500m400s500m400s500m400s1250m400s500m400s500m400s500m400s1250"
      "m400s1250m400s1250m400s1250m400s500m400s1250m400s1250m400s1250m400s500"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250m400s1250"
      "m400s500m400s500m400s500m400s500m400s500m400s500m400s500m400s500"
      "m400s100000",
      irsend.outputStr());
}

// Decode a synthetic HitachiAC2 message.

TEST(TestDecodeHitachiAC2, NormalSyntheticExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // ON - 32c cool (fan auto)
  uint8_t hitachi_code[kHitachiAc2StateLength] = {
      0x80, 0x08, 0x00, 0x02, 0xFD, 0xFF, 0x00, 0x33, 0xCC, 0x49, 0xB6,
      0x22, 0xDD, 0x01, 0xFE, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
      0xFF, 0x00, 0xFF, 0xCA, 0x35, 0x8F, 0x70, 0x00, 0xFF, 0x00, 0xFF,
      0x01, 0xFE, 0xC0, 0x3F, 0x80, 0x7F, 0x11, 0xEE, 0x00, 0xFF, 0x00,
      0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00};

  irsend.reset();
  irsend.sendHitachiAC2(hitachi_code);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(HITACHI_AC2, irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAc2Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(hitachi_code, irsend.capture.state, kHitachiAc2Bits);
}

// Decode a 'real' HitachiAC2 message.
TEST(TestDecodeHitachiAC2, NormalRealExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  uint8_t hitachi_code[kHitachiAc2StateLength] = {
      0x80, 0x08, 0x00, 0x02, 0xFD, 0xFF, 0x00, 0x33, 0xCC, 0x49, 0xB6,
      0x22, 0xDD, 0x01, 0xFE, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
      0xFF, 0x00, 0xFF, 0xCA, 0x35, 0x8F, 0x70, 0x00, 0xFF, 0x00, 0xFF,
      0x01, 0xFE, 0xC0, 0x3F, 0x80, 0x7F, 0x11, 0xEE, 0x00, 0xFF, 0x00,
      0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00};

  // Ref: https://github.com/markszabo/IRremoteESP8266/issues/417
  uint16_t rawData[851] = {
      // ON - 32c cool (fan auto)
      3432, 1654, 492, 1180, 488, 360,  486, 360,  486, 360,  486, 362,
      486,  362,  460, 360,  486, 362,  486, 362,  486, 360,  486, 362,
      486,  362,  486, 1184, 488, 362,  486, 360,  486, 362,  486, 362,
      486,  362,  460, 362,  486, 362,  486, 362,  484, 362,  486, 362,
      486,  362,  486, 362,  458, 362,  484, 362,  486, 362,  484, 364,
      484,  362,  486, 1184, 486, 362,  486, 1210, 488, 1210, 488, 1184,
      486,  1210, 488, 1210, 488, 1182, 488, 360,  488, 1208, 488, 1208,
      488,  1182, 488, 1208, 488, 1208, 488, 1208, 462, 1208, 490, 1208,
      488,  1208, 490, 332,  488, 360,  488, 360,  488, 360,  486, 360,
      486,  360,  486, 362,  460, 360,  486, 362,  486, 362,  486, 1210,
      488,  1210, 460, 362,  486, 362,  486, 1210, 488, 1210, 488, 1182,
      488,  1210, 488, 362,  486, 360,  488, 1182, 488, 1210, 488, 362,
      486,  362,  486, 360,  486, 1182, 488, 362,  486, 362,  486, 1210,
      488,  360,  486, 336,  486, 1210, 488, 1210, 488, 360,  486, 1182,
      488,  1210, 488, 360,  488, 1208, 488, 1182, 488, 362,  486, 362,
      486,  360,  486, 1210, 460, 362,  486, 360,  486, 362,  486, 1210,
      486,  362,  486, 1184, 488, 1208, 488, 360,  486, 1210, 488, 1184,
      488,  1208, 488, 362,  486, 1208, 462, 360,  486, 360,  486, 362,
      486,  360,  486, 362,  486, 360,  486, 334,  486, 1210, 488, 1208,
      488,  1210, 488, 1182, 488, 1208, 488, 1210, 488, 1182, 488, 1208,
      488,  360,  488, 360,  486, 360,  486, 334,  486, 362,  486, 362,
      486,  362,  486, 362,  486, 362,  486, 1182, 488, 1210, 486, 1210,
      488,  1210, 460, 1210, 488, 1210, 488, 1210, 488, 1184, 488, 360,
      486,  360,  486, 362,  486, 362,  486, 360,  460, 360,  486, 362,
      486,  362,  484, 1210, 488, 1182, 488, 1210, 488, 1210, 488, 1210,
      488,  1182, 488, 1208, 488, 1208, 488, 360,  460, 360,  486, 360,
      486,  360,  488, 360,  486, 362,  486, 362,  486, 334,  486, 1210,
      488,  1208, 488, 1210, 460, 1210, 488, 1208, 488, 1210, 488, 1182,
      488,  1208, 488, 360,  486, 362,  486, 360,  488, 334,  486, 360,
      486,  362,  486, 362,  486, 362,  486, 1210, 460, 1210, 488, 1210,
      488,  1210, 488, 1182, 488, 1210, 488, 1208, 488, 1182, 488, 360,
      486,  360,  486, 362,  486, 362,  486, 360,  486, 362,  460, 360,
      486,  362,  486, 1210, 486, 1212, 486, 1184, 488, 1210, 488, 1208,
      488,  1182, 488, 1208, 488, 1208, 488, 1208, 462, 1208, 488, 360,
      486,  362,  486, 1208, 488, 334,  486, 1208, 488, 360,  486, 360,
      486,  362,  486, 1182, 488, 1210, 488, 360,  486, 1210, 488, 334,
      486,  1208, 488, 1208, 488, 360,  486, 360,  486, 334,  486, 1210,
      488,  1210, 488, 1210, 460, 1210, 488, 360,  486, 1208, 488, 1182,
      490,  1208, 488, 360,  486, 360,  486, 360,  486, 360,  460, 360,
      486,  362,  486, 362,  486, 362,  486, 362,  486, 362,  486, 362,
      458,  362,  486, 1210, 488, 1210, 488, 1182, 488, 1210, 488, 1208,
      488,  1208, 462, 1208, 488, 1208, 488, 360,  486, 360,  486, 334,
      486,  360,  486, 362,  486, 362,  486, 362,  486, 362,  486, 1184,
      488,  1210, 488, 1210, 488, 1208, 462, 1210, 488, 1208, 488, 1208,
      488,  1182, 488, 360,  486, 360,  486, 360,  486, 362,  486, 360,
      460,  362,  486, 360,  488, 1208, 488, 1210, 488, 1184, 488, 1208,
      488,  1210, 488, 1182, 488, 1208, 488, 1208, 488, 360,  488, 1182,
      488,  1208, 488, 360,  488, 360,  488, 360,  486, 334,  486, 360,
      486,  362,  486, 362,  486, 360,  486, 1210, 488, 1182, 488, 1210,
      488,  1210, 488, 1182, 488, 1210, 488, 1210, 488, 360,  488, 334,
      486,  360,  488, 360,  486, 362,  486, 362,  486, 362,  486, 362,
      460,  1210, 488, 1210, 488, 1210, 488, 1184, 488, 1208, 488, 1208,
      490,  1182, 488, 360,  488, 360,  486, 362,  486, 1208, 488, 360,
      460,  362,  486, 362,  486, 1210, 488, 1210, 488, 1182, 488, 1208,
      488,  360,  488, 1208, 488, 1184, 486, 1210, 488, 360,  488, 360,
      486,  360,  460, 362,  486, 362,  486, 362,  486, 362,  484, 362,
      486,  362,  486, 1184, 488, 1210, 488, 1210, 488, 1182, 488, 1210,
      488,  1208, 488, 1208, 462, 1210, 488, 360,  486, 362,  486, 360,
      486,  362,  486, 334,  486, 362,  486, 360,  486, 362,  486, 1208,
      488,  1184, 488, 1208, 488, 1210, 488, 1182, 488, 1208, 488, 1208,
      488,  1208, 488, 1182, 488, 1208, 488, 1208, 488, 1182, 488, 1208,
      488,  1208, 488, 1182, 488, 1208, 488, 360,  488, 360,  486, 360,
      486,  362,  460, 362,  486, 362,  486, 362,  486, 360,  486, 1208,
      488,  1182, 488, 1208, 488, 1210, 488, 1182, 488, 1208, 490, 1208,
      488,  1208, 462, 360,  488, 360,  486, 360,  488, 360,  486, 360,
      486,  360,  486, 336,  486, 362,  486, 1210, 488, 1208, 488, 1182,
      488,  1208, 488, 1208, 488, 1208, 462, 1208, 490, 1206, 490, 360,
      488,  360,  488, 332,  486, 360,  488, 360,  486, 360,  488, 360,
      486,  360,  486, 1182, 488, 1208, 488, 1206, 492, 1208, 462, 1208,
      488,  1208, 488, 1208, 490, 1182, 490, 360,  486, 360,  486, 360,
      488,  360,  488, 360,  460, 360,  486, 362,  486, 360,  488};

  irsend.reset();
  irsend.sendRaw(rawData, 851, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(HITACHI_AC2, irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAc2Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(hitachi_code, irsend.capture.state, kHitachiAc2Bits);
}
