// Copyright 2018 David Conran

#include "ir_Hitachi.h"
#include "IRac.h"
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
      "f38000d50"
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
      "f38000d50"
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
      "f38000d50"
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
      "Power: On, Mode: 3 (Heat), Temp: 32C, Fan: 5 (High), "
      "Swing(V): On, Swing(H): Off",
      ac.toString());
  ac.setMode(kHitachiAcCool);
  ac.setTemp(kHitachiAcMinTemp);
  ac.setFan(kHitachiAcFanLow);
  ac.setSwingVertical(false);
  ac.setSwingHorizontal(true);
  EXPECT_EQ(
      "Power: On, Mode: 4 (Cool), Temp: 16C, Fan: 2 (Low), "
      "Swing(V): Off, Swing(H): On",
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

  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/417
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
      "Power: On, Mode: 4 (Cool), Temp: 16C, Fan: 1 (Auto), "
      "Swing(V): Off, Swing(H): Off",
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

  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/417
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
      "Power: On, Mode: 3 (Heat), Temp: 32C, Fan: 5 (High), "
      "Swing(V): Off, Swing(H): Off",
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
      "f38000d50"
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

  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/453
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
  EXPECT_EQ(
      "Model: 2 (R-LT0541-HTA-B), Power: Off, Power Toggle: On, "
      "Mode: 6 (Cool), Temp: 23C, Fan: 1 (Auto), "
      "Swing(V) Toggle: Off, Swing(V): Off, Swing(H): Off, Sleep: Off, "
      "On Timer: Off, Off Timer: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
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
      "f38000d50"
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

  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/417
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

TEST(TestIRHitachiAcClass, toCommon) {
  IRHitachiAc ac(0);
  ac.setPower(true);
  ac.setMode(kHitachiAcCool);
  ac.setTemp(20);
  ac.setFan(kHitachiAcFanHigh);
  ac.setSwingVertical(true);
  ac.setSwingHorizontal(true);
  // Now test it.
  ASSERT_EQ(decode_type_t::HITACHI_AC, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kAuto, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kAuto, ac.toCommon().swingh);
  // Unsupported.
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("HITACHI_AC", typeToString(decode_type_t::HITACHI_AC));
  ASSERT_EQ(decode_type_t::HITACHI_AC, strToDecodeType("HITACHI_AC"));
  ASSERT_TRUE(hasACState(decode_type_t::HITACHI_AC));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::HITACHI_AC));
  ASSERT_EQ(kHitachiAcBits,
            IRsend::defaultBits(decode_type_t::HITACHI_AC));
  ASSERT_EQ(kNoRepeat,
            IRsend::minRepeats(decode_type_t::HITACHI_AC));

  ASSERT_EQ("HITACHI_AC1", typeToString(decode_type_t::HITACHI_AC1));
  ASSERT_EQ(decode_type_t::HITACHI_AC1, strToDecodeType("HITACHI_AC1"));
  ASSERT_TRUE(hasACState(decode_type_t::HITACHI_AC1));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::HITACHI_AC1));
  ASSERT_EQ(kHitachiAc1Bits,
            IRsend::defaultBits(decode_type_t::HITACHI_AC1));
  ASSERT_EQ(kNoRepeat,
            IRsend::minRepeats(decode_type_t::HITACHI_AC1));

  ASSERT_EQ("HITACHI_AC2", typeToString(decode_type_t::HITACHI_AC2));
  ASSERT_EQ(decode_type_t::HITACHI_AC2, strToDecodeType("HITACHI_AC2"));
  ASSERT_TRUE(hasACState(decode_type_t::HITACHI_AC2));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::HITACHI_AC2));
  ASSERT_EQ(kHitachiAc2Bits,
            IRsend::defaultBits(decode_type_t::HITACHI_AC2));
  ASSERT_EQ(kNoRepeat,
            IRsend::minRepeats(decode_type_t::HITACHI_AC2));

  ASSERT_EQ("HITACHI_AC3", typeToString(decode_type_t::HITACHI_AC3));
  ASSERT_EQ(decode_type_t::HITACHI_AC3, strToDecodeType("HITACHI_AC3"));
  ASSERT_TRUE(hasACState(decode_type_t::HITACHI_AC3));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::HITACHI_AC3));

  ASSERT_EQ("HITACHI_AC424", typeToString(decode_type_t::HITACHI_AC424));
  ASSERT_EQ(decode_type_t::HITACHI_AC424, strToDecodeType("HITACHI_AC424"));
  ASSERT_TRUE(hasACState(decode_type_t::HITACHI_AC424));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::HITACHI_AC424));
  ASSERT_EQ(kHitachiAc424Bits,
            IRsend::defaultBits(decode_type_t::HITACHI_AC424));
  ASSERT_EQ(kNoRepeat,
            IRsend::minRepeats(decode_type_t::HITACHI_AC424));

  ASSERT_EQ("HITACHI_AC344", typeToString(decode_type_t::HITACHI_AC344));
  ASSERT_EQ(decode_type_t::HITACHI_AC344, strToDecodeType("HITACHI_AC344"));
  ASSERT_TRUE(hasACState(decode_type_t::HITACHI_AC344));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::HITACHI_AC344));
  ASSERT_EQ(kHitachiAc344Bits,
            IRsend::defaultBits(decode_type_t::HITACHI_AC344));
  ASSERT_EQ(kNoRepeat,
            IRsend::minRepeats(decode_type_t::HITACHI_AC344));
}

// Decode a 'real' HitachiAc424 message.
TEST(TestDecodeHitachiAc424, RealExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  uint8_t expected[kHitachiAc424StateLength] = {
      0x01, 0x10, 0x00, 0x40, 0xBF, 0xFF, 0x00, 0xCC, 0x33, 0x92,
      0x6D, 0x13, 0xEC, 0x5C, 0xA3, 0x00, 0xFF, 0x00, 0xFF, 0x00,
      0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x53, 0xAC, 0xF1, 0x0E, 0x00,
      0xFF, 0x00, 0xFF, 0x80, 0x7F, 0x03, 0xFC, 0x01, 0xFE, 0x88,
      0x77, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
      0x00, 0xFF, 0x00};

  // Ref: https://docs.google.com/spreadsheets/d/1TTRx7INyDlsJBn9UwebL2Q4Q0S0apq0DlKP6bfrVPQw/edit#gid=0&range=A2:B2
  uint16_t rawData[853] = {
      // On Auto Cool 23
      29784, 49290, 3416, 1604, 464, 1210, 468, 372, 460, 374, 462, 374, 466,
      368, 464, 372, 462, 374, 464, 374, 464, 368, 464, 370, 464, 370, 466, 370,
      464, 1208, 464, 374, 462, 372, 466, 374, 464, 370, 462, 372, 464, 370,
      466, 370, 464, 372, 462, 374, 462, 374, 462, 378, 460, 370, 460, 374, 464,
      372, 462, 372, 464, 374, 466, 368, 464, 1210, 464, 374, 466, 1202, 464,
      1206, 464, 1210, 466, 1206, 468, 1204, 464, 1210, 466, 370, 462, 1214,
      460, 1208, 464, 1206, 464, 1208, 466, 1208, 464, 1206, 466, 1208, 464,
      1206, 466, 1212, 464, 370, 464, 370, 462, 374, 462, 374, 462, 374, 462,
      374, 462, 372, 464, 376, 460, 372, 462, 374, 466, 1204, 464, 1210, 464,
      372, 460, 374, 462, 1208, 464, 1212, 464, 1202, 468, 1204, 464, 374, 460,
      374, 466, 1208, 462, 1210, 462, 374, 462, 376, 464, 368, 466, 1204, 462,
      374, 466, 372, 464, 1206, 462, 376, 460, 376, 464, 1210, 462, 1208, 462,
      372, 466, 1206, 464, 1208, 466, 372, 462, 1210, 462, 1210, 466, 374, 468,
      1202, 464, 1206, 466, 374, 462, 372, 464, 1208, 464, 374, 464, 372, 464,
      376, 462, 370, 466, 368, 464, 1208, 462, 1210, 460, 374, 464, 1208, 466,
      1206, 464, 1214, 464, 368, 462, 374, 462, 1212, 460, 1210, 466, 1206, 466,
      370, 462, 1210, 464, 416, 424, 1202, 466, 1220, 448, 376, 464, 372, 462,
      372, 462, 1212, 462, 374, 460, 1214, 468, 364, 468, 370, 462, 372, 462,
      376, 458, 374, 464, 372, 462, 376, 464, 376, 462, 1204, 464, 1210, 462,
      1210, 464, 1208, 466, 1208, 464, 1206, 462, 1210, 464, 1212, 464, 368,
      462, 372, 464, 372, 464, 372, 464, 372, 466, 370, 466, 370, 464, 376,
      464, 1202, 464, 1212, 464, 1204, 464, 1210, 462, 1208, 464, 1212, 462,
      1210, 464, 1212, 460, 372, 462, 374, 462, 374, 466, 370, 462, 374, 462,
      372, 464, 372, 462, 376, 462, 1206, 464, 1206, 466, 1210, 462, 1208, 464,
      1210, 466, 1204, 464, 1210, 462, 1214, 462, 368, 462, 374, 466, 370, 462,
      376, 466, 368, 466, 370, 462, 414, 424, 374, 464, 1206, 464, 1206, 464,
      1206, 468, 1206, 466, 1206, 466, 1210, 462, 1206, 464, 1214, 468, 364,
      466, 372, 466, 370, 462, 372, 462, 374, 464, 372, 462, 374, 460, 376, 466,
      1204, 464, 1208, 462, 1210, 464, 1206, 464, 1210, 464, 1208, 464, 1208,
      466, 1210, 462, 1206, 466, 1206, 466, 372, 462, 374, 466, 1206, 466, 370,
      464, 1206, 466, 376, 464, 368, 462, 372, 466, 1206, 464, 1206, 464, 374,
      466, 1204, 464, 374, 466, 1206, 466, 1204, 468, 368, 466, 370, 466, 370,
      462, 1212, 462, 1210, 462, 1210, 462, 1214, 464, 368, 464, 1206, 466,
      1206, 466, 1206, 464, 374, 464, 370, 466, 370, 462, 378, 466, 366, 464,
      372, 466, 368, 466, 370, 464, 370, 462, 372, 462, 374, 464, 374, 464,
      1202, 466, 1206, 462, 1208, 466, 1208, 466, 1208, 464, 1210, 462, 1206,
      464, 1212, 464, 368, 464, 372, 464, 370, 468, 368, 462, 376, 462, 372,
      466, 370, 464, 376, 462, 1206, 464, 1210, 462, 1212, 462, 1208, 464, 1208,
      462, 1212, 466, 1246, 424, 1212, 464, 368, 464, 372, 466, 370, 464, 372,
      462, 374, 464, 372, 464, 370, 462, 1212, 466, 1206, 462, 1206, 464, 1210,
      466, 1206, 462, 1208, 464, 1250, 422, 1208, 468, 372, 464, 1204, 466,
      1206, 466, 370, 462, 374, 462, 376, 460, 374, 466, 370, 462, 376, 464,
      368, 462, 376, 462, 1210, 462, 1208, 464, 1206, 466, 1206, 464, 1208, 468,
      1212, 460, 1206, 464, 372, 464, 372, 466, 370, 462, 374, 466, 370, 466,
      370, 466, 374, 464, 368, 462, 1210, 462, 1210, 464, 1210, 462, 1208, 462,
      1212, 464, 1206, 466, 1208, 466, 366, 464, 374, 460, 374, 462, 1208, 466,
      372, 462, 374, 462, 374, 464, 1212, 468, 1202, 464, 1208, 466, 1204, 464,
      376, 460, 1208, 468, 1208, 462, 1208, 464, 378, 460, 372, 460, 372, 462,
      376, 464, 372, 462, 374, 460, 374, 464, 370, 462, 378, 464, 1202, 468,
      1204, 468, 1204, 466, 1208, 466, 1208, 464, 1210, 460, 1212, 462, 1212,
      464, 366, 466, 370, 464, 372, 466, 370, 464, 372, 462, 414, 424, 372, 466,
      372, 460, 1206, 466, 1206, 466, 1206, 466, 1208, 466, 1206, 464, 1208,
      466, 1208, 462, 1212, 468, 1202, 466, 1204, 470, 1204, 468, 1204, 466,
      1206, 466, 1206, 464, 1210, 462, 1212, 468, 366, 464, 372, 462, 374, 460,
      374, 460, 374, 466, 410, 424, 372, 460, 378, 466, 1200, 464, 1212, 462,
      1210, 464, 1210, 466, 1206, 462, 1208, 464, 1210, 464, 1210, 464, 366,
      462, 376, 462, 374, 460, 376, 462, 372, 466, 374, 460, 372, 462, 378, 462,
      1202, 468, 1206, 464, 1208, 466, 1208, 462, 1208, 464, 1208, 468, 1204,
      464, 1212, 466, 368, 462, 374, 466, 372, 464, 370, 462, 374, 464, 370,
      462, 376, 464, 374, 462, 1206, 464, 1208, 462, 1210, 466, 1208, 460, 1210,
      468, 1206, 462, 1210, 464, 1212, 466, 366, 464, 374, 462, 372, 466, 370,
      462, 374, 464, 372, 464, 370, 464, 374, 462};

  irsend.reset();
  irsend.sendRaw(rawData, 853, kHitachiAcFreq);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(HITACHI_AC424, irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAc424Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, kHitachiAc424Bits);
  IRHitachiAc ac(0);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: On, Mode: 3 (Cool), Temp: 23C, Fan: 5 (Auto), "
      "Button: 19 (Power/Mode), Swing(V) Toggle: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

TEST(TestDecodeHitachiAc424, SyntheticExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  uint8_t expected[kHitachiAc424StateLength] = {
      0x01, 0x10, 0x00, 0x40, 0xBF, 0xFF, 0x00, 0xCC, 0x33, 0x92,
      0x6D, 0x13, 0xEC, 0x5C, 0xA3, 0x00, 0xFF, 0x00, 0xFF, 0x00,
      0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x53, 0xAC, 0xF1, 0x0E, 0x00,
      0xFF, 0x00, 0xFF, 0x80, 0x7F, 0x03, 0xFC, 0x01, 0xFE, 0x88,
      0x77, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
      0x00, 0xFF, 0x00};

  irsend.reset();
  irsend.sendHitachiAc424(expected);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(HITACHI_AC424, irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAc424Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
}

// Tests for IRHitachiAc424 class.
TEST(TestIRHitachiAc424Class, SetInvertedStates) {
  IRHitachiAc424 ac(0);

  uint8_t raw[kHitachiAc424StateLength] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00};
  uint8_t expected[kHitachiAc424StateLength] = {
    0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0x00, 0xFF};

  ac.setRaw(raw);
  EXPECT_STATE_EQ(expected, ac.getRaw(), kHitachiAc424Bits);
}

TEST(TestIRHitachiAc424Class, SetAndGetPower) {
  IRHitachiAc424 ac(0);
  ac.on();
  EXPECT_TRUE(ac.getPower());
  ac.off();
  EXPECT_FALSE(ac.getPower());
  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());
  EXPECT_EQ(ac.getButton(), kHitachiAc424ButtonPowerMode);
  ac.setPower(false);
  EXPECT_FALSE(ac.getPower());
  EXPECT_EQ(ac.getButton(), kHitachiAc424ButtonPowerMode);
}

TEST(TestIRHitachiAc424Class, SetAndGetTemp) {
  IRHitachiAc424 ac(0);
  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());
  ac.setTemp(kHitachiAc424MinTemp);
  EXPECT_EQ(kHitachiAc424MinTemp, ac.getTemp());
  ac.setTemp(kHitachiAc424MinTemp - 1);
  EXPECT_EQ(kHitachiAc424MinTemp, ac.getTemp());
  ac.setTemp(kHitachiAc424MaxTemp);
  EXPECT_EQ(kHitachiAc424MaxTemp, ac.getTemp());
  ac.setTemp(kHitachiAc424MaxTemp + 1);
  EXPECT_EQ(kHitachiAc424MaxTemp, ac.getTemp());
}

TEST(TestIRHitachiAc424Class, SetAndGetMode) {
  IRHitachiAc424 ac(0);
  ac.setMode(kHitachiAc424Cool);
  ac.setFan(kHitachiAc424FanAuto);
  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());
  EXPECT_EQ(kHitachiAc424Cool, ac.getMode());
  EXPECT_EQ(kHitachiAc424FanAuto, ac.getFan());
  ac.setMode(kHitachiAc424Fan);
  EXPECT_EQ(kHitachiAc424Fan, ac.getMode());
  EXPECT_EQ(27, ac.getTemp());
  EXPECT_NE(kHitachiAc424FanAuto, ac.getFan());
  ac.setMode(kHitachiAc424Heat);
  EXPECT_EQ(25, ac.getTemp());
  EXPECT_EQ(kHitachiAc424Heat, ac.getMode());
  ac.setMode(kHitachiAc424Dry);
  EXPECT_EQ(kHitachiAc424Dry, ac.getMode());
  EXPECT_NE(kHitachiAc424FanAuto, ac.getFan());
}

TEST(TestIRHitachiAc424Class, SetAndGetFan) {
  IRHitachiAc424 ac(0);
  ac.setMode(kHitachiAc424Cool);  // All fan options are available in this mode.
  ac.setFan(kHitachiAc424FanAuto);
  EXPECT_EQ(kHitachiAc424FanAuto, ac.getFan());
  ac.setFan(kHitachiAc424FanLow);
  EXPECT_EQ(kHitachiAc424FanLow, ac.getFan());
  EXPECT_EQ(kHitachiAc424ButtonFan, ac.getButton());
  ac.setFan(kHitachiAc424FanHigh);
  EXPECT_EQ(kHitachiAc424FanHigh, ac.getFan());
  ac.setFan(kHitachiAc424FanMax + 1);
  EXPECT_EQ(kHitachiAc424FanMax, ac.getFan());
  ac.setFan(kHitachiAc424FanMin - 1);
  EXPECT_EQ(kHitachiAc424FanMin, ac.getFan());

  ac.setFan(kHitachiAc424FanAuto);
  ac.setMode(kHitachiAc424Fan);  // No auto-fan in Fan mode.
  EXPECT_EQ(kHitachiAc424FanMin, ac.getFan());
  ac.setFan(kHitachiAc424FanMax);
  EXPECT_EQ(kHitachiAc424FanMax, ac.getFan());

  // Only min, low and auto fan settings in Dry mode.
  ac.setMode(kHitachiAc424Dry);
  EXPECT_EQ(kHitachiAc424FanLow, ac.getFan());
  ac.setFan(kHitachiAc424FanHigh);
  EXPECT_EQ(kHitachiAc424FanLow, ac.getFan());
  ac.setFan(kHitachiAc424FanMin);
  EXPECT_EQ(kHitachiAc424FanMin, ac.getFan());
  ac.setFan(kHitachiAc424FanAuto);
  EXPECT_EQ(kHitachiAc424FanAuto, ac.getFan());

  // Check additional bytes set by min & max fan
  ac.setMode(kHitachiAc424Cool);
  ac.setFan(kHitachiAc424FanMax);
  EXPECT_EQ(ac.getRaw()[9], 0xA9);
  EXPECT_EQ(ac.getRaw()[29], 0x30);
  ac.setFan(kHitachiAc424FanMin);
  EXPECT_EQ(ac.getRaw()[9], 0x98);
  EXPECT_EQ(ac.getRaw()[29], 0x00);
  ac.setFan(kHitachiAc424FanLow);
  EXPECT_EQ(ac.getRaw()[9], 0x92);
  EXPECT_EQ(ac.getRaw()[29], 0x00);
}


TEST(TestIRHitachiAc424Class, SetAndGetButton) {
  IRHitachiAc424 ac(0);
  ac.on();
  EXPECT_EQ(ac.getButton(), kHitachiAc424ButtonPowerMode);
  ac.setButton(kHitachiAc424ButtonTempUp);
  EXPECT_EQ(ac.getButton(), kHitachiAc424ButtonTempUp);
  ac.setButton(kHitachiAc424ButtonSwingV);
  EXPECT_EQ(ac.getButton(), kHitachiAc424ButtonSwingV);
}

TEST(TestIRHitachiAc424Class, ToggleSwingVertical) {
  IRHitachiAc424 ac(0);
  ac.on();
  EXPECT_EQ(ac.getButton(), kHitachiAc424ButtonPowerMode);
  ac.setSwingVToggle(true);
  EXPECT_TRUE(ac.getSwingVToggle());
  EXPECT_EQ(ac.getButton(), kHitachiAc424ButtonSwingV);
  ac.setSwingVToggle(false);
  EXPECT_FALSE(ac.getSwingVToggle());
  EXPECT_EQ(ac.getButton(), kHitachiAc424ButtonPowerMode);
}

TEST(TestIRHitachiAc424Class, HumanReadable) {
  IRHitachiAc424 ac(0);

  ac.setMode(kHitachiAc424Heat);
  ac.setTemp(kHitachiAc424MaxTemp);
  ac.on();
  ac.setFan(kHitachiAc424FanHigh);
  EXPECT_EQ(
      "Power: On, Mode: 6 (Heat), Temp: 32C, Fan: 4 (High), "
      "Button: 66 (Fan), Swing(V) Toggle: Off",
      ac.toString());
  ac.setMode(kHitachiAc424Cool);
  ac.setFan(kHitachiAc424FanMin);
  ac.setTemp(kHitachiAc424MinTemp);
  EXPECT_EQ(
      "Power: On, Mode: 3 (Cool), Temp: 16C, Fan: 1 (Min), "
      "Button: 67 (Temp Down), Swing(V) Toggle: Off",
      ac.toString());
  ac.setSwingVToggle(true);
  EXPECT_EQ(
      "Power: On, Mode: 3 (Cool), Temp: 16C, Fan: 1 (Min), "
      "Button: 129 (Swing(V)), Swing(V) Toggle: On",
      ac.toString());
  ac.setTemp(ac.getTemp() + 1);
  EXPECT_EQ(
      "Power: On, Mode: 3 (Cool), Temp: 17C, Fan: 1 (Min), "
      "Button: 68 (Temp Up), Swing(V) Toggle: Off",
      ac.toString());
  ac.setTemp(ac.getTemp() - 1);
  EXPECT_EQ(
      "Power: On, Mode: 3 (Cool), Temp: 16C, Fan: 1 (Min), "
      "Button: 67 (Temp Down), Swing(V) Toggle: Off",
      ac.toString());
}

TEST(TestIRHitachiAc424Class, toCommon) {
  IRHitachiAc424 ac(kGpioUnused);
  ac.setPower(true);
  ac.setMode(kHitachiAc424Cool);
  ac.setTemp(20);
  ac.setFan(kHitachiAc424FanMax);
  // Now test it.
  ASSERT_EQ(decode_type_t::HITACHI_AC424, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  // Unsupported.
  ASSERT_EQ(stdAc::swingv_t::kOff, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

TEST(TestDecodeHitachiAc3, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  uint8_t expected[kHitachiAc3StateLength - 4] = {
      0x01, 0x10, 0x00, 0x40, 0xBF, 0xFF, 0x00, 0xE6, 0x19, 0x89, 0x76, 0x01,
      0xFE, 0x3F, 0xC0, 0x2F, 0xD0, 0x18, 0xE7, 0x00, 0xFF, 0xA0, 0x5F};

  irsend.reset();
  irsend.sendHitachiAc3(expected, kHitachiAc3StateLength - 4);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(HITACHI_AC3, irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAc3Bits - 32, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
}

// Decode a 'real' HitachiAc3 message.
TEST(TestDecodeHitachiAc3, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  uint8_t expected[kHitachiAc3StateLength - 4] = {
      0x01, 0x10, 0x00, 0x40, 0xBF, 0xFF, 0x00, 0xE6, 0x19, 0x89, 0x76, 0x01,
      0xFE, 0x3F, 0xC0, 0x2F, 0xD0, 0x18, 0xE7, 0x00, 0xFF, 0xA0, 0x5F};

  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1060#issuecomment-597519432
  uint16_t rawData[371] = {
      // Power Off
      3422, 1660, 464, 1264, 438, 426, 438, 402, 486, 426, 440, 402, 462, 402,
      488, 428, 438, 402, 460, 404, 488, 426, 440, 424, 436, 428, 462, 1240,
      466, 398, 462, 404, 486, 426, 438, 402, 464, 400, 490, 400, 464, 426, 438,
      402, 488, 428, 436, 428, 462, 376, 486, 404, 474, 416, 438, 400, 488, 402,
      462, 426, 438, 426, 458, 1246, 464, 426, 436, 1244, 488, 1266, 434, 1268,
      436, 1242, 486, 1242, 462, 1240, 464, 426, 462, 1266, 438, 1242, 462,
      1240, 514, 1214, 466, 1236, 462, 1266, 464, 1264, 438, 1266, 438, 1240,
      488, 400, 466, 424, 440, 402, 486, 404, 462, 402, 464, 402, 488, 426, 438,
      402, 462, 402, 488, 1264, 438, 1242, 460, 428, 462, 428, 436, 1264, 440,
      1240, 488, 1240, 464, 1240, 460, 402, 490, 424, 440, 1264, 438, 1242, 512,
      402, 438, 426, 466, 398, 464, 1266, 440, 400, 462, 402, 488, 1264, 438,
      402, 462, 402, 482, 432, 438, 1264, 436, 404, 488, 1240, 462, 1264, 438,
      402, 486, 1246, 456, 1266, 438, 1238, 488, 402, 462, 1240, 462, 402, 512,
      402, 438, 428, 438, 426, 462, 430, 434, 428, 438, 402, 512, 376, 462,
      1240, 464, 1264, 458, 1270, 436, 1242, 462, 1240, 486, 1242, 460, 1242,
      462, 1242, 486, 1240, 464, 1240, 464, 1238, 492, 1264, 436, 1266, 440,
      400, 488, 426, 436, 430, 434, 430, 460, 404, 462, 426, 438, 402, 488, 426,
      436, 1264, 466, 1238, 462, 1242, 462, 1266, 438, 1238, 490, 1264, 438,
      426, 438, 1240, 486, 406, 462, 402, 462, 400, 488, 428, 436, 426, 438,
      402, 484, 1268, 438, 426, 436, 1242, 488, 1266, 436, 402, 462, 402, 502,
      386, 464, 1240, 464, 1240, 488, 426, 438, 426, 438, 402, 488, 1240, 462,
      1266, 438, 1242, 486, 428, 440, 424, 438, 1266, 460, 1268, 438, 1264, 438,
      404, 486, 428, 438, 426, 438, 402, 490, 400, 462, 426, 436, 402, 490, 426,
      438, 1240, 462, 1268, 460, 1268, 434, 1266, 436, 1240, 514, 1238, 438,
      1240, 488, 1240, 460, 406, 464, 426, 436, 402, 488, 402, 462, 402, 462,
      1264, 462, 404, 462, 1266, 434, 1268, 464, 1264, 438, 1242, 464, 1238,
      488, 1266, 438, 402, 462, 1268, 458, 430, 410};

  irsend.reset();
  irsend.sendRaw(rawData, 371, kHitachiAcFreq);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(HITACHI_AC3, irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAc3Bits - 32, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
}

TEST(TestDecodeHitachiAc3, SyntheticTempChangeExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  const uint16_t expectedLength = kHitachiAc3MinStateLength + 2;
  uint8_t expected[expectedLength] = {
      0x01, 0x10, 0x00, 0x40, 0xBF, 0xFF, 0x00, 0xE3, 0x1C, 0x89, 0x76, 0x08,
      0xF7, 0x3F, 0xC0, 0x15, 0xEA};

  irsend.reset();
  irsend.sendHitachiAc3(expected, expectedLength);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(HITACHI_AC3, irsend.capture.decode_type);
  ASSERT_EQ(expectedLength * 8, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
}

// Decode a 'real' Temp Change HitachiAc3 message.
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1060#issuecomment-597592537
TEST(TestDecodeHitachiAc3, RealTempChangeExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  const uint8_t expected[kHitachiAc3MinStateLength + 2] = {
      0x01, 0x10, 0x00, 0x40, 0xBF, 0xFF, 0x00, 0xE3, 0x1C, 0x89, 0x76, 0x08,
      0xF7, 0x3F, 0xC0, 0x15, 0xEA};
  const uint16_t expectedBits = kHitachiAc3MinBits + 2 * 8;

  const uint16_t rawData[275] = {
      // Change Temp
      3422, 1636, 490, 1242, 484, 380, 488, 400, 466, 426, 432, 406, 490, 400,
      464, 426, 462, 398, 466, 376, 492, 400, 458, 404, 490, 400, 466, 1262,
      434, 430, 464, 400, 490, 400, 458, 380, 490, 374, 490, 426, 432, 430, 464,
      400, 464, 426, 460, 402, 468, 372, 490, 400, 460, 404, 490, 374, 490, 400,
      484, 378, 490, 398, 464, 1240, 490, 398, 466, 1238, 464, 1266, 434, 1268,
      466, 1238, 466, 1240, 484, 1218, 490, 400, 460, 1246, 484, 1242, 466,
      1214, 488, 1240, 486, 1218, 490, 1214, 488, 1266, 434, 1242, 490, 1214,
      490, 424, 460, 404, 464, 374, 492, 424, 434, 430, 466, 400, 464, 426, 464,
      376, 492, 1236, 464, 1240, 488, 402, 466, 374, 490, 400, 458, 1244, 490,
      1238, 496, 1234, 436, 404, 488, 400, 464, 1240, 484, 1244, 464, 1238, 466,
      426, 434, 430, 466, 400, 464, 1264, 434, 406, 490, 374, 490, 1264, 458,
      404, 466, 400, 466, 424, 466, 1238, 464, 400, 466, 1238, 486, 1216, 490,
      400, 464, 1240, 486, 1240, 466, 1238, 460, 432, 432, 430, 468, 374, 488,
      426, 462, 1242, 464, 400, 466, 426, 434, 404, 490, 374, 490, 1240, 484,
      1244, 466, 1240, 462, 428, 436, 1242, 490, 1212, 490, 1264, 466, 1236,
      466, 1214, 490, 1240, 488, 1240, 466, 1236, 466, 1264, 434, 1268, 464,
      400, 494, 400, 430, 406, 488, 376, 488, 428, 432, 432, 462, 402, 462, 426,
      458, 1220, 488, 1214, 490, 1240, 484, 406, 466, 1212, 490, 426, 462, 1216,
      488, 400, 464, 402, 488, 402, 462, 374, 492, 1262, 460, 380, 488, 1240,
      464, 428, 462, 1214, 492, 1236, 462, 1216, 490};

  irsend.reset();
  irsend.sendRaw(rawData, 275, kHitachiAcFreq);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(HITACHI_AC3, irsend.capture.decode_type);
  ASSERT_EQ(expectedBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, expectedBits);
}

// Decode a 'real' Cancel Timer HitachiAc3 message.
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1060#issuecomment-598631576
TEST(TestDecodeHitachiAc3, RealCancelTimerExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  const uint8_t expected[kHitachiAc3MinStateLength] = {
      0x01, 0x10, 0x00, 0x40, 0xBF, 0xFF, 0x00, 0xE2, 0x1D, 0x89, 0x76, 0x0D,
      0xF2, 0x3F, 0xC0};
  const uint16_t expectedBits = kHitachiAc3MinBits;

  const uint16_t rawData[243] = {
      // Cancel Timer
      3368, 1688, 460, 1244, 456, 434, 458, 406, 456, 408, 480, 410, 458, 406,
      458, 406, 484, 406, 458, 406, 458, 432, 428, 464, 456, 380, 458, 1242,
      458, 432, 456, 406, 486, 378, 430, 458, 456, 408, 458, 406, 456, 434, 456,
      410, 454, 408, 454, 434, 460, 404, 458, 406, 454, 436, 456, 408, 458, 406,
      458, 432, 456, 434, 430, 1246, 484, 404, 484, 1218, 458, 1244, 484, 1244,
      460, 1244, 456, 1270, 430, 1274, 460, 404, 484, 1218, 486, 1244, 456,
      1244, 456, 1248, 454, 1272, 460, 1268, 432, 1246, 484, 1244, 458, 1244,
      462, 404, 456, 434, 458, 432, 432, 408, 456, 434, 460, 404, 460, 404, 484,
      406, 432, 432, 458, 1298, 402, 432, 460, 404, 460, 404, 482, 1244, 460,
      1270, 462, 1214, 456, 1272, 460, 404, 456, 1246, 486, 1244, 458, 1244,
      458, 406, 454, 460, 434, 404, 460, 1268, 430, 460, 432, 406, 458, 1244,
      456, 434, 458, 406, 456, 432, 460, 1268, 432, 406, 458, 1244, 484, 1272,
      432, 430, 432, 1298, 378, 1298, 458, 1244, 484, 406, 460, 1242, 458, 406,
      458, 1246, 456, 1272, 458, 406, 458, 432, 458, 404, 460, 404, 456, 408,
      484, 1272, 432, 432, 432, 406, 454, 1272, 458, 1246, 458, 1244, 484, 1268,
      436, 1242, 458, 1298, 402, 1274, 456, 1244, 458, 1244, 486, 1242, 458,
      408, 486, 378, 454, 460, 434, 406, 458, 406, 484, 406, 458, 406, 458, 408,
      452, 1302, 434, 1244, 430};

  irsend.reset();
  irsend.sendRaw(rawData, 243, kHitachiAcFreq);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(HITACHI_AC3, irsend.capture.decode_type);
  ASSERT_EQ(expectedBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, expectedBits);
}

TEST(TestDecodeHitachiAc3, SyntheticCancelTimerExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  const uint8_t expected[kHitachiAc3MinStateLength] = {
      0x01, 0x10, 0x00, 0x40, 0xBF, 0xFF, 0x00, 0xE2, 0x1D, 0x89, 0x76, 0x0D,
      0xF2, 0x3F, 0xC0};

  irsend.reset();
  irsend.sendHitachiAc3(expected, kHitachiAc3MinStateLength);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(HITACHI_AC3, irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAc3MinBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
}

// Decode a 'real' Set Timer HitachiAc3 message.
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1060#issuecomment-598631576
TEST(TestDecodeHitachiAc3, RealSetTimerExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  const uint8_t expected[kHitachiAc3StateLength] = {
      0x01, 0x10, 0x00, 0x40, 0xBF, 0xFF, 0x00, 0xE8, 0x17, 0x89, 0x76, 0x0B,
      0xF4, 0x3F, 0xC0, 0x15, 0xEA, 0x00, 0xFF, 0x00, 0xFF, 0x4B, 0xB4, 0x18,
      0xE7, 0x00, 0xFF};
  const uint16_t expectedBits = kHitachiAc3Bits;

  const uint16_t rawData[435] = {
      // Set Timer
      3364, 1668, 486, 1244, 458, 406, 456, 436, 430, 434, 430, 434, 456, 408,
      484, 432, 428, 432, 414, 450, 458, 432, 432, 432, 430, 436, 458, 1270,
      430, 408, 456, 408, 454, 462, 404, 460, 428, 408, 486, 404, 456, 408, 458,
      406, 484, 406, 456, 434, 432, 408, 484, 406, 454, 436, 404, 432, 486, 430,
      430, 408, 456, 432, 458, 1270, 406, 434, 456, 1246, 484, 1244, 456, 1274,
      432, 1244, 486, 1268, 432, 1244, 454, 412, 454, 1272, 430, 1272, 456,
      1274, 456, 1246, 456, 1244, 458, 1270, 458, 1246, 456, 1254, 450, 1246,
      484, 408, 456, 434, 430, 408, 486, 430, 404, 460, 430, 408, 486, 404, 458,
      406, 458, 406, 486, 404, 458, 432, 430, 1272, 456, 408, 458, 1244, 458,
      1244, 456, 1274, 456, 1246, 456, 1270, 460, 1250, 426, 460, 404, 1270,
      484, 432, 404, 462, 402, 460, 458, 1244, 456, 386, 452, 460, 458, 1244,
      458, 406, 458, 406, 486, 430, 416, 1260, 458, 406, 484, 1270, 406, 1296,
      430, 410, 482, 1246, 456, 1246, 456, 1246, 456, 434, 458, 1244, 456, 1272,
      458, 408, 458, 1244, 456, 410, 484, 404, 454, 410, 456, 408, 484, 406,
      458, 432, 430, 1246, 484, 406, 454, 1248, 458, 1244, 486, 1248, 428, 1270,
      456, 1266, 464, 1244, 458, 1246, 456, 1246, 484, 1270, 430, 1246, 456,
      434, 430, 460, 406, 432, 430, 432, 484, 432, 406, 432, 458, 406, 484, 432,
      432, 1266, 434, 1274, 430, 1298, 428, 408, 456, 1246, 486, 430, 432, 1270,
      404, 434, 484, 432, 430, 408, 456, 406, 460, 1268, 458, 408, 456, 1246,
      486, 406, 456, 1244, 456, 1248, 456, 1300, 430, 408, 456, 408, 484, 432,
      430, 434, 428, 434, 460, 430, 404, 434, 458, 406, 486, 1242, 458, 1270,
      430, 1246, 484, 1244, 456, 1270, 432, 1248, 484, 1244, 456, 1246, 456,
      408, 486, 404, 456, 408, 456, 432, 462, 430, 406, 458, 432, 432, 428,
      436, 456, 1246, 456, 1246, 484, 1244, 454, 1268, 438, 1244, 488, 1266,
      430, 1246, 458, 1244, 486, 1244, 430, 1298, 430, 434, 460, 1242, 458, 430,
      430, 408, 486, 1242, 456, 434, 404, 462, 456, 430, 432, 1246, 456, 408,
      486, 1270, 430, 1246, 460, 404, 482, 1246, 458, 406, 456, 408, 484, 404,
      458, 1270, 430, 1274, 458, 432, 430, 408, 456, 406, 486, 1244, 458, 1242,
      458, 1246, 484, 406, 458, 432, 430, 1246, 486, 1242, 456, 1272, 430, 434,
      458, 406, 458, 406, 458, 406, 486, 404, 458, 406, 458, 430, 430, 460, 430,
      1270, 430, 1248, 484, 1244, 456, 1244, 458, 1246, 484, 1244, 456, 1246,
      460, 1244, 458};  // UNKNOWN D3A5A0BA

  irsend.reset();
  irsend.sendRaw(rawData, 435, kHitachiAcFreq);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(HITACHI_AC3, irsend.capture.decode_type);
  ASSERT_EQ(expectedBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, expectedBits);
}

TEST(TestDecodeHitachiAc3, SyntheticSetTimerExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  const uint8_t expected[kHitachiAc3StateLength] = {
      0x01, 0x10, 0x00, 0x40, 0xBF, 0xFF, 0x00, 0xE8, 0x17, 0x89, 0x76, 0x0B,
      0xF4, 0x3F, 0xC0, 0x15, 0xEA, 0x00, 0xFF, 0x00, 0xFF, 0x4B, 0xB4, 0x18,
      0xE7, 0x00, 0xFF};

  irsend.reset();
  irsend.sendHitachiAc3(expected, kHitachiAc3StateLength);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(HITACHI_AC3, irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAc3Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
}

// Decode a 'real' Change Mode HitachiAc3 message.
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1060#issuecomment-598631576
TEST(TestDecodeHitachiAc3, RealChangeModeExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  const uint8_t expected[kHitachiAc3StateLength - 6] = {
      0x01, 0x10, 0x00, 0x40, 0xBF, 0xFF, 0x00, 0xE5, 0x1A, 0x89, 0x76, 0x04,
      0xFB, 0x3F, 0xC0, 0x1B, 0xE4, 0x14, 0xEB, 0x02, 0xFD};
  const uint16_t expectedBits = kHitachiAc3Bits - 6 * 8;

  const uint16_t rawData[339] = {
      // Change Mode
      3364, 1666, 488, 1268, 404, 434, 454, 434, 460, 404, 458, 406, 458, 406,
      484, 432, 432, 430, 432, 408, 484, 406, 458, 432, 432, 406, 486, 1242,
      458, 430, 430, 408, 454, 436, 456, 408, 458, 406, 484, 432, 428, 434, 430,
      408, 484, 406, 458, 430, 432, 408, 484, 430, 430, 432, 432, 408, 482, 408,
      456, 408, 458, 432, 458, 1246, 458, 430, 430, 1246, 486, 1242, 458, 1270,
      432, 1246, 486, 1242, 458, 1268, 432, 434, 430, 1272, 456, 1246, 458,
      1270, 460, 1244, 458, 1246, 456, 1272, 460, 1242, 456, 1270, 434, 1246,
      486, 430, 430, 434, 432, 406, 484, 430, 432, 432, 430, 408, 484, 432, 432,
      432, 430, 1246, 484, 406, 458, 1246, 456, 408, 486, 404, 458, 1268, 434,
      1270, 430, 1274, 456, 408, 458, 1270, 460, 404, 458, 1244, 458, 1246, 486,
      430, 430, 432, 460, 378, 488, 1240, 460, 404, 458, 408, 486, 1242, 458,
      432, 430, 434, 460, 430, 432, 1246, 458, 406, 488, 1240, 430, 1272, 458,
      406, 486, 1242, 430, 1298, 430, 1274, 428, 432, 430, 436, 456, 408, 482,
      1248, 458, 406, 430, 462, 456, 404, 458, 432, 430, 408, 486, 1266, 436,
      1242, 458, 406, 486, 1242, 456, 1246, 458, 1244, 488, 1240, 458, 1244,
      458, 1272, 460, 1242, 456, 1246, 458, 1246, 486, 1242, 458, 1270, 432,
      406, 458, 458, 434, 430, 432, 406, 486, 406, 456, 408, 458, 432, 484, 406,
      430, 1272, 460, 1216, 486, 1242, 456, 1246, 458, 406, 486, 1268, 432,
      1244, 458, 406, 486, 404, 460, 432, 430, 406, 488, 402, 458, 1272, 428,
      434, 460, 404, 460, 1242, 458, 1246, 480, 1244, 462, 428, 432, 432, 460,
      1244, 458, 432, 430, 1246, 488, 402, 456, 408, 458, 406, 486, 1268, 432,
      1246, 460, 430, 460, 1244, 458, 406, 458, 1244, 486, 1242, 458, 1244, 458,
      432, 462, 1242, 456, 408, 456, 406, 486, 428, 434, 406, 458, 406, 456,
      434, 458, 1244, 460, 430, 462, 1240, 458, 1244, 460, 1244, 486, 1244, 458,
      1242, 488, 1214, 460};  // UNKNOWN C1EA1036

  irsend.reset();
  irsend.sendRaw(rawData, 339, kHitachiAcFreq);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(HITACHI_AC3, irsend.capture.decode_type);
  ASSERT_EQ(expectedBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, expectedBits);
}

TEST(TestDecodeHitachiAc3, SyntheticChangeModeExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  const uint8_t expected[kHitachiAc3StateLength - 6] = {
      0x01, 0x10, 0x00, 0x40, 0xBF, 0xFF, 0x00, 0xE5, 0x1A, 0x89, 0x76, 0x04,
      0xFB, 0x3F, 0xC0, 0x1B, 0xE4, 0x14, 0xEB, 0x02, 0xFD};
  const uint16_t expectedBits = kHitachiAc3Bits - 6 * 8;

  irsend.reset();
  irsend.sendHitachiAc3(expected, kHitachiAc3StateLength - 6);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(HITACHI_AC3, irsend.capture.decode_type);
  ASSERT_EQ(expectedBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
}

TEST(TestHitachiAc3Class, hasInvertedStates) {
  const uint8_t good_state[kHitachiAc3StateLength] = {
      0x01, 0x10, 0x00, 0x40, 0xBF, 0xFF, 0x00, 0xE8, 0x17, 0x89, 0x76, 0x0B,
      0xF4, 0x3F, 0xC0, 0x15, 0xEA, 0x00, 0xFF, 0x00, 0xFF, 0x4B, 0xB4, 0x18,
      0xE7, 0x00, 0xFF};
  // bad_state[kHitachiAc3MinStateLength + 1] has been modified to be different.
  // i.e. Anything larger than kHitachiAc3MinStateLength should fail.
  //      kHitachiAc3MinStateLength or shorter should pass.
  const uint8_t bad_state[kHitachiAc3StateLength] = {
      0x01, 0x10, 0x00, 0x40, 0xBF, 0xFF, 0x00, 0xE8, 0x17, 0x89, 0x76, 0x0B,
      0xF4, 0x3F, 0xC0, 0x15, 0xE0, 0x00, 0xFF, 0x00, 0xFF, 0x4B, 0xB4, 0x18,
      0xE7, 0x00, 0xFF};

  EXPECT_TRUE(IRHitachiAc3::hasInvertedStates(good_state,
                                              kHitachiAc3StateLength));

  for (uint8_t len = kHitachiAc3StateLength;
       len > kHitachiAc3MinStateLength;
       len -= 2) {
    EXPECT_FALSE(IRHitachiAc3::hasInvertedStates(bad_state, len));
  }
  EXPECT_TRUE(IRHitachiAc3::hasInvertedStates(bad_state,
                                              kHitachiAc3MinStateLength));
  EXPECT_TRUE(IRHitachiAc3::hasInvertedStates(bad_state,
                                              kHitachiAc3MinStateLength - 2));
}

// HitachiAc1 Class tests

TEST(TestIRHitachiAc1Class, SetAndGetPower) {
  IRHitachiAc1 ac(kGpioUnused);
  ac.on();
  ac.setPowerToggle(false);
  EXPECT_TRUE(ac.getPower());
  EXPECT_FALSE(ac.getPowerToggle());
  ac.off();
  EXPECT_FALSE(ac.getPower());
  EXPECT_TRUE(ac.getPowerToggle());
  ac.setPowerToggle(false);
  EXPECT_FALSE(ac.getPowerToggle());
  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());
  EXPECT_TRUE(ac.getPowerToggle());
  ac.setPower(false);
  EXPECT_FALSE(ac.getPower());
  EXPECT_TRUE(ac.getPowerToggle());
}

TEST(TestIRHitachiAc1Class, SetAndGetTemp) {
  IRHitachiAc1 ac(kGpioUnused);
  ac.setMode(kHitachiAc1Cool);  // All temps possible in Cool mode.
  ac.setTemp(26);
  EXPECT_EQ(26, ac.getTemp());
  ac.setTemp(kHitachiAcMinTemp);
  EXPECT_EQ(kHitachiAcMinTemp, ac.getTemp());
  ac.setTemp(kHitachiAcMinTemp - 1);
  EXPECT_EQ(kHitachiAcMinTemp, ac.getTemp());
  ac.setTemp(kHitachiAcMaxTemp);
  EXPECT_EQ(kHitachiAcMaxTemp, ac.getTemp());
  ac.setTemp(kHitachiAcMaxTemp + 1);
  EXPECT_EQ(kHitachiAcMaxTemp, ac.getTemp());

  // Can't change temp in Auto mode.
  ac.setMode(kHitachiAc1Auto);  // All temps possible in Cool mode.
  EXPECT_EQ(kHitachiAc1TempAuto, ac.getTemp());
  ac.setTemp(kHitachiAcMinTemp);
  EXPECT_EQ(kHitachiAc1TempAuto, ac.getTemp());

  // Ref: https://docs.google.com/spreadsheets/d/10eKpJEWJppUYktPRLCcAIwzfFXjtkOZNyn1reh5MFfU/edit#gid=0&range=B46
  const uint8_t cool_31_auto[kHitachiAc1StateLength] = {
      0xB2, 0xAE, 0x4D, 0x91, 0xF0, 0x61, 0x8C, 0x00, 0x00, 0x00, 0x00, 0x20,
      0x68};
  ac.setRaw(cool_31_auto);
  EXPECT_EQ(31, ac.getTemp());
}

TEST(TestIRHitachiAc1Class, SetAndGetMode) {
  IRHitachiAc1 ac(kGpioUnused);
  ac.setMode(kHitachiAc1Auto);
  EXPECT_EQ(kHitachiAc1Auto, ac.getMode());
  ac.setMode(kHitachiAc1Cool);
  EXPECT_EQ(kHitachiAc1Cool, ac.getMode());
  ac.setMode(kHitachiAc1Fan);
  EXPECT_EQ(kHitachiAc1Fan, ac.getMode());
  ac.setMode(kHitachiAc1Heat);
  EXPECT_EQ(kHitachiAc1Heat, ac.getMode());
  ac.setMode(kHitachiAc1Dry);
  EXPECT_EQ(kHitachiAc1Dry, ac.getMode());
  ac.setMode(0);
  EXPECT_EQ(kHitachiAc1Auto, ac.getMode());
  ac.setMode(255);
  EXPECT_EQ(kHitachiAc1Auto, ac.getMode());
}

TEST(TestIRHitachiAc1Class, SetAndGetFan) {
  IRHitachiAc1 ac(kGpioUnused);
  ac.setMode(kHitachiAc1Cool);  // All speeds possible in Cool mode.
  ac.setFan(kHitachiAc1FanAuto);
  EXPECT_EQ(kHitachiAc1FanAuto, ac.getFan());
  ac.setFan(kHitachiAc1FanLow);
  EXPECT_EQ(kHitachiAc1FanLow, ac.getFan());
  ac.setFan(kHitachiAc1FanHigh);
  EXPECT_EQ(kHitachiAc1FanHigh, ac.getFan());
  ac.setFan(kHitachiAc1FanMed);
  EXPECT_EQ(kHitachiAc1FanMed, ac.getFan());

  ac.setFan(255);
  EXPECT_EQ(kHitachiAc1FanAuto, ac.getFan());
  ac.setFan(0);
  EXPECT_EQ(kHitachiAc1FanAuto, ac.getFan());

  // Can't change speed in Auto mode. Locked to auto speed.
  ac.setMode(kHitachiAc1Auto);
  EXPECT_EQ(kHitachiAc1FanAuto, ac.getFan());
  ac.setFan(kHitachiAc1FanLow);
  EXPECT_EQ(kHitachiAc1FanAuto, ac.getFan());
  // Can't change speed in Dry mode. Locked to low speed.
  ac.setMode(kHitachiAc1Dry);
  EXPECT_EQ(kHitachiAc1FanLow, ac.getFan());
  ac.setFan(kHitachiAc1FanHigh);
  EXPECT_EQ(kHitachiAc1FanLow, ac.getFan());
}

TEST(TestIRHitachiAc1Class, HumanReadable) {
  IRHitachiAc1 ac(kGpioUnused);

  // Ref: https://docs.google.com/spreadsheets/d/10eKpJEWJppUYktPRLCcAIwzfFXjtkOZNyn1reh5MFfU/edit#gid=0&range=A47:B47
  const uint8_t cool_32_auto[kHitachiAc1StateLength] = {
      0xB2, 0xAE, 0x4D, 0x91, 0xF0, 0x61, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x30,
      0x04};
  ac.setRaw(cool_32_auto);
  EXPECT_EQ(
      "Model: 1 (R-LT0541-HTA-A), Power: On, Power Toggle: On, Mode: 6 (Cool), "
      "Temp: 32C, Fan: 1 (Auto), Swing(V) Toggle: Off, "
      "Swing(V): Off, Swing(H): Off, "
      "Sleep: Off, On Timer: Off, Off Timer: Off",
      ac.toString());
  ac.setModel(hitachi_ac1_remote_model_t::R_LT0541_HTA_B);
  ac.setSwingV(true);
  ac.setSwingToggle(true);
  ac.setSleep(kHitachiAc1Sleep2);
  ac.setPowerToggle(false);
  ac.setOnTimer(2 * 60 + 39);
  ac.setOffTimer(10 * 60 + 17);
  EXPECT_EQ(
      "Model: 2 (R-LT0541-HTA-B), Power: On, Power Toggle: Off, "
      "Mode: 6 (Cool), Temp: 32C, Fan: 1 (Auto), "
      "Swing(V) Toggle: On, Swing(V): On, Swing(H): Off, Sleep: 2, "
      "On Timer: 02:39, Off Timer: 10:17",
      ac.toString());
}

TEST(TestIRHitachiAc1Class, Checksum) {
  // Reverse Table: 2=4, B=D, A=5, E=7, 1=8, 3=C, 0=0, 6=6, 9=9, F=F
  IRHitachiAc1 ac(kGpioUnused);
  // Ref: https://docs.google.com/spreadsheets/d/10eKpJEWJppUYktPRLCcAIwzfFXjtkOZNyn1reh5MFfU/edit#gid=0&range=A47:B47
  const uint8_t cool_32_auto[kHitachiAc1StateLength] = {
      0xB2, 0xAE, 0x4D, 0x91, 0xF0, 0x61, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x30,
      0x04};
  // Reversed: 4D, 75, B2, 89, 0F, 86, 33, 00, 00, 00, 00, 0C, 20
  EXPECT_TRUE(ac.validChecksum(cool_32_auto));
  EXPECT_EQ(0x04, ac.calcChecksum(cool_32_auto));

  // Ref: https://docs.google.com/spreadsheets/d/10eKpJEWJppUYktPRLCcAIwzfFXjtkOZNyn1reh5MFfU/edit#gid=0&range=B46
  const uint8_t cool_31_auto[kHitachiAc1StateLength] = {
      0xB2, 0xAE, 0x4D, 0x91, 0xF0, 0x61, 0x8C, 0x00, 0x00, 0x00, 0x00, 0x20,
      0x68};
  // Reversed: 4D, 75, B2, 89, 0F, 86, 31, 00, 00, 00, 00, 04, 16
  EXPECT_TRUE(ac.validChecksum(cool_31_auto));
  EXPECT_EQ(0x68, ac.calcChecksum(cool_31_auto));

  // Ref: https://docs.google.com/spreadsheets/d/10eKpJEWJppUYktPRLCcAIwzfFXjtkOZNyn1reh5MFfU/edit#gid=0&range=B13
  const uint8_t auto_25_auto_swing_on[kHitachiAc1StateLength] = {
      0xB2, 0xAE, 0x4D, 0x91, 0xF0, 0xE1, 0xA4, 0x00, 0x00, 0x00, 0x00, 0x61,
      0x24};
  // Reversed: 4D, 75, B2, 89, 0F, 87, 25, 00, 00, 00, 00, 86, 42
  EXPECT_TRUE(ac.validChecksum(auto_25_auto_swing_on));
  EXPECT_EQ(0x24, ac.calcChecksum(auto_25_auto_swing_on));
  // Ref: https://docs.google.com/spreadsheets/d/10eKpJEWJppUYktPRLCcAIwzfFXjtkOZNyn1reh5MFfU/edit#gid=0&range=B45
  const uint8_t cool_30_auto[kHitachiAc1StateLength] = {
      0xB2, 0xAE, 0x4D, 0x91, 0xF0, 0x61, 0xF4, 0x00, 0x00, 0x00, 0x00, 0x20,
      0xC4};
  EXPECT_TRUE(ac.validChecksum(cool_30_auto));
  EXPECT_EQ(0xC4, ac.calcChecksum(cool_30_auto));
}

TEST(TestIRHitachiAc1Class, toCommon) {
  IRHitachiAc1 ac(kGpioUnused);
  ac.setPower(true);
  ac.setMode(kHitachiAc1Cool);
  ac.setTemp(20);
  ac.setFan(kHitachiAc1FanHigh);
  ac.setSwingV(false);
  ac.setSwingH(true);
  ac.setModel(hitachi_ac1_remote_model_t::R_LT0541_HTA_B);
  // Now test it.
  ASSERT_EQ(decode_type_t::HITACHI_AC1, ac.toCommon().protocol);
  ASSERT_EQ(2, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kOff, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kAuto, ac.toCommon().swingh);
  // Unsupported.
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

TEST(TestIRHitachiAc1Class, ReconstructKnownGood) {
  IRHitachiAc1 ac(kGpioUnused);
  const uint8_t known_good[kHitachiAc1StateLength] = {
      0xB2, 0xAE, 0x4D, 0x51, 0xF0, 0x61, 0x84,
      0x00, 0x00, 0x00, 0x00, 0x10, 0x98};
  ac.stateReset();
  ac.setPower(false);
  ac.setPowerToggle(true);
  ac.setMode(kHitachiAc1Cool);
  ac.setTemp(23);
  ac.setFan(kHitachiAc1FanAuto);
  ac.setSwingV(false);
  ac.setSwingH(false);
  ac.setSwingToggle(false);
  ac.setModel(hitachi_ac1_remote_model_t::R_LT0541_HTA_B);

  EXPECT_STATE_EQ(known_good, ac.getRaw(), kHitachiAc1Bits);
  EXPECT_EQ(
      "Model: 2 (R-LT0541-HTA-B), Power: Off, Power Toggle: On, "
      "Mode: 6 (Cool), Temp: 23C, Fan: 1 (Auto), "
      "Swing(V) Toggle: Off, Swing(V): Off, Swing(H): Off, Sleep: Off, "
      "On Timer: Off, Off Timer: Off",
      ac.toString());
}

TEST(TestIRHitachiAc1Class, Swing) {
  IRHitachiAc1 ac(kGpioUnused);
  ac.setSwingV(false);
  EXPECT_FALSE(ac.getSwingV());
  ac.setSwingToggle(false);
  EXPECT_FALSE(ac.getSwingToggle());

  ac.setSwingV(true);
  EXPECT_TRUE(ac.getSwingV());
  EXPECT_FALSE(ac.getSwingToggle());
  ac.setSwingToggle(true);
  EXPECT_TRUE(ac.getSwingV());
  EXPECT_TRUE(ac.getSwingToggle());

  ac.setSwingV(false);
  EXPECT_FALSE(ac.getSwingV());
  ac.setSwingToggle(false);
  EXPECT_FALSE(ac.getSwingToggle());

  const uint8_t swing_on_with_toggle[kHitachiAc1StateLength] = {
    0xB2, 0xAE, 0x4D, 0x91, 0xF0, 0xE1, 0xA4,
    0x00, 0x00, 0x00, 0x00, 0x61, 0x24};
  ac.setRaw(swing_on_with_toggle);
  EXPECT_TRUE(ac.getSwingV());
  EXPECT_FALSE(ac.getSwingH());
  EXPECT_TRUE(ac.getSwingToggle());
  const uint8_t swing_off_with_toggle[kHitachiAc1StateLength] = {
    0xB2, 0xAE, 0x4D, 0x91, 0xF0, 0xE1, 0xA4,
    0x00, 0x00, 0x00, 0x00, 0x21, 0x44};
  ac.setRaw(swing_off_with_toggle);
  EXPECT_FALSE(ac.getSwingV());
  EXPECT_FALSE(ac.getSwingH());
  EXPECT_TRUE(ac.getSwingToggle());
  ac.setSwingToggle(true);
  EXPECT_STATE_EQ(swing_off_with_toggle, ac.getRaw(), kHitachiAc1Bits);
}

// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1056#issuecomment-609374682
TEST(TestIRHitachiAc1Class, FanSpeedInDryMode) {
  IRHitachiAc1 ac(kGpioUnused);
  // IRhvac {"Vendor":"HITACHI_AC1", "Power":"On","Mode":"dry","FanSpeed":"low"
  //         "Temp":22.5}
  const uint8_t state[kHitachiAc1StateLength] = {  // Code generated by Tasmota.
      0xB2, 0xAE, 0x4D, 0x91, 0xF0, 0x21, 0xF8,
      0x00, 0x00, 0x00, 0x00, 0x31, 0x0C};
  ac.setRaw(state);
  EXPECT_EQ(
      "Model: 1 (R-LT0541-HTA-A), Power: On, Power Toggle: On, Mode: 2 (Dry), "
      "Temp: 22C, Fan: 1 (Auto), "
      "Swing(V) Toggle: On, Swing(V): Off, Swing(H): Off, Sleep: Off, "
      "On Timer: Off, Off Timer: Off",
      ac.toString());
  ac.stateReset();
  // Build it via the build order in IRac::hitachi1()
  ac.setModel(hitachi_ac1_remote_model_t::R_LT0541_HTA_A);
  ac.setPower(true);
  ac.setPowerToggle(true);
  ac.setMode(kHitachiAc1Dry);
  ac.setTemp(22.5);
  ac.setFan(kHitachiAc1FanLow);
  ac.setSwingV(false);
  ac.setSwingH(false);
  ac.setSwingToggle(true);
  ac.setSleep(0);
  EXPECT_EQ(
      "Model: 1 (R-LT0541-HTA-A), Power: On, Power Toggle: On, Mode: 2 (Dry), "
      "Temp: 22C, Fan: 8 (Low), "
      "Swing(V) Toggle: On, Swing(V): Off, Swing(H): Off, Sleep: Off, "
      "On Timer: Off, Off Timer: Off",
      ac.toString());
}

// Decode a 'real' HitachiAc344 message.
TEST(TestDecodeHitachiAc344, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  uint8_t expected[kHitachiAc344StateLength] = {
      0x01, 0x10, 0x00, 0x40, 0xBF, 0xFF, 0x00, 0xCC, 0x33, 0x98, 0x67, 0x13,
      0xEC, 0x68, 0x97, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
      0xFF, 0x13, 0xEC, 0xF1, 0x0E, 0x00, 0xFF, 0x00, 0xFF, 0x80, 0x7F, 0x03,
      0xFC, 0x20, 0xDF, 0x00, 0xFF, 0x00, 0xFF};

  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1134#issue-622516158
  const uint16_t rawData[691] = {3410, 1624, 482, 1226, 458, 464, 458, 462, 456,
      464, 460, 460, 462, 460, 460, 462, 460, 460, 460, 462, 460, 462, 456, 466,
      458, 462, 456, 1228, 462, 460, 460, 462, 460, 462, 458, 462, 456, 466,
      456, 464, 458, 464, 456, 466, 460, 460, 460, 462, 460, 460, 458, 464, 456,
      464, 458, 464, 456, 464, 460, 462, 454, 466, 458, 1226, 460, 462, 456,
      1226, 456, 1230, 456, 1228, 456, 1230, 458, 1228, 456, 1228, 456, 464,
      456, 1230, 454, 1230, 458, 1228, 458, 1226, 454, 1230, 454, 1230, 454,
      1230, 458, 1228, 458, 1230, 452, 468, 456, 464, 454, 468, 456, 464, 418,
      502, 456, 466, 458, 464, 458, 462, 458, 464, 454, 468, 456, 1230, 454,
      1230, 454, 466, 456, 464, 456, 1228, 456, 1230, 456, 1230, 456, 1230, 456,
      464, 458, 464, 458, 1228, 456, 1230, 456, 464, 454, 466, 456, 466, 456,
      464, 458, 462, 458, 1228, 458, 1228, 458, 464, 454, 468, 456, 1228, 458,
      1228, 458, 1228, 456, 1228, 456, 464, 454, 468, 456, 1228, 458, 1226, 458,
      464, 458, 1226, 456, 1230, 454, 468, 458, 462, 452, 1232, 460, 462, 458,
      460, 460, 462, 456, 466, 458, 462, 456, 1230, 456, 1228, 460, 462, 460,
      1226, 458, 1228, 458, 1226, 460, 460, 456, 468, 458, 464, 456, 1226, 460,
      462, 458, 1228, 456, 1228, 458, 462, 462, 1224, 460, 1226, 458, 1226, 460,
      464, 458, 1228, 456, 462, 460, 462, 460, 1226, 460, 460, 462, 460, 458,
      462, 458, 462, 456, 464, 460, 460, 462, 460, 462, 460, 460, 1224, 462,
      1224, 462, 1224, 458, 1226, 462, 1224, 460, 1224, 400, 1284, 446, 1240,
      446, 476, 458, 462, 464, 458, 462, 460, 460, 460, 458, 462, 462, 462, 456,
      464, 460, 1226, 460, 1226, 460, 1224, 462, 1224, 460, 1224, 460, 1224,
      460, 1224, 462, 1224, 462, 460, 460, 462, 460, 460, 458, 464, 458, 462,
      458, 464, 458, 462, 460, 460, 462, 1224, 460, 1226, 458, 1228, 456, 1226,
      462, 1222, 460, 1228, 458, 1226, 460, 1226, 460, 460, 458, 462, 460, 462,
      460, 460, 460, 462, 460, 462, 458, 464, 460, 458, 460, 1226, 456, 1228,
      462, 1224, 460, 1224, 458, 1228, 458, 1226, 458, 1228, 462, 1224, 460,
      462, 460, 462, 458, 464, 460, 460, 456, 466, 458, 462, 460, 460, 462,
      458, 460, 1224, 460, 1224, 458, 1226, 460, 1224, 460, 1226, 460, 1226,
      458, 1228, 456, 1230, 456, 1228, 462, 1224, 460, 460, 458, 462, 458,
      1228, 456, 466, 458, 462, 454, 468, 458, 462, 458, 462, 460, 1226, 456,
      1228, 458, 464, 420, 1264, 458, 1228, 458, 1228, 456, 1228, 454, 468, 456,
      464, 456, 466, 456, 1228, 460, 1226, 456, 1230, 456, 1228, 456, 464, 456,
      1230, 458, 1226, 458, 1226, 452, 468, 456, 466, 376, 546, 456, 466, 456,
      464, 456, 466, 458, 464, 458, 464, 456, 466, 424, 496, 456, 464, 416, 504,
      454, 1230, 454, 1232, 456, 1228, 456, 1228, 456, 1230, 456, 1230, 454,
      1230, 460, 1226, 426, 496, 424, 496, 456, 466, 374, 546, 454, 468, 374,
      544, 458, 464, 456, 464, 458, 1228, 424, 1262, 454, 1232, 426, 1258, 458,
      1228, 426, 1260, 454, 1230, 456, 1228, 426, 496, 374, 546, 426, 494, 426,
      496, 424, 496, 426, 496, 424, 496, 456, 1230, 456, 1230, 456, 1228, 458,
      1228, 456, 1230, 456, 1230, 456, 1230, 424, 1260, 458, 464, 426, 1258,
      456, 1230, 422, 500, 456, 466, 418, 504, 424, 496, 426, 496, 456, 464,
      420, 500, 454, 468, 456, 1230, 424, 1260, 420, 1264, 422, 1264, 418,
      1266, 420, 1264, 420, 500, 456, 466, 426, 494, 454, 468, 456, 464, 420,
      1266, 452, 470, 454, 466, 374, 1310, 456, 1228, 456, 1230, 452, 1232, 420,
      1266, 424, 498, 456, 1230, 456, 1230, 456, 466, 450, 470, 456, 464, 376,
      546, 456, 466, 422, 498, 426, 496, 458, 464, 424, 1260, 454, 1232, 454,
      1232, 454, 1232, 458, 1228, 422, 1262, 456, 1228, 454, 1230, 456, 468,
      416, 504, 422, 498, 424, 498, 454, 466, 456, 466, 422, 500, 456, 466, 374,
      1310, 418, 1266, 424, 1262, 422, 1262, 376, 1310, 376, 1308, 422, 1264,
      424, 1262, 350};  // UNKNOWN 919B8582

  irsend.reset();
  irsend.sendRaw(rawData, 691, kHitachiAcFreq);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(HITACHI_AC344, irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAc344Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
}

// Decode a synthetic HitachiAc344 message.
TEST(TestDecodeHitachiAc344, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  uint8_t expected[kHitachiAc344StateLength] = {
      0x01, 0x10, 0x00, 0x40, 0xBF, 0xFF, 0x00, 0xCC, 0x33, 0x98, 0x67, 0x13,
      0xEC, 0x68, 0x97, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
      0xFF, 0x13, 0xEC, 0xF1, 0x0E, 0x00, 0xFF, 0x00, 0xFF, 0x80, 0x7F, 0x03,
      0xFC, 0x20, 0xDF, 0x00, 0xFF, 0x00, 0xFF};

  irsend.reset();
  irsend.sendHitachiAc344(expected);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(HITACHI_AC344, irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAc344Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Power: On, Mode: 3 (Cool), Temp: 26C, Fan: 1 (Min), "
      "Button: 19 (Power/Mode), Swing(V): On, Swing(H): 3 (Middle)",
      IRAcUtils::resultAcToString(&irsend.capture));
}

TEST(TestDecodeIRHitachiAc344, ExampleMessages) {
  IRHitachiAc344 ac(kGpioUnused);

  // On, 17, Hot, Auto
  // Ref: https://docs.google.com/spreadsheets/d/1LPd8K9V437oyEMZT6JDv5LlPXh61RPmgeoVcHLWWr7k/edit#gid=1093727366&range=E21
  // but bit order revered.
  const uint8_t state[kHitachiAc344StateLength] = {
      0x01, 0x10, 0x00, 0x40, 0xBF, 0xFF, 0x00, 0xCC, 0x33, 0x92, 0x6D, 0x13,
      0xEC, 0x44, 0xBB, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
      0xFF, 0x56, 0xA9, 0xF1, 0x0E, 0x00, 0xFF, 0x00, 0xFF, 0x80, 0x7F, 0x03,
      0xFC, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF};
  ac.setRaw(state);
  EXPECT_EQ(
      "Power: On, Mode: 6 (Heat), Temp: 17C, Fan: 5 (Auto), "
      "Button: 19 (Power/Mode), Swing(V): Off, Swing(H): 3 (Middle)",
      ac.toString());
}

TEST(TestIRHitachiAc344Class, ReconstructKnownState) {
  IRHitachiAc344 ac(kGpioUnused);

  // On, 17, Hot, Auto
  const uint8_t expected[kHitachiAc344StateLength] = {
      0x01, 0x10, 0x00, 0x40, 0xBF, 0xFF, 0x00, 0xCC, 0x33, 0x92, 0x6D, 0x13,
      0xEC, 0x44, 0xBB, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
      0xFF, 0x56, 0xA9, 0xF1, 0x0E, 0x00, 0xFF, 0x00, 0xFF, 0x80, 0x7F, 0x03,
      0xFC, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF};
  ac.setMode(kHitachiAc344Heat);
  ac.setTemp(17);
  ac.setFan(kHitachiAc344FanAuto);
  ac.setButton(kHitachiAc344ButtonPowerMode);
  EXPECT_STATE_EQ(expected, ac.getRaw(), kHitachiAc344Bits);
}

TEST(TestIRHitachiAc344Class, SwingV) {
  IRHitachiAc344 ac(kGpioUnused);
  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1134#issuecomment-635760537

  // https://docs.google.com/spreadsheets/d/1LPd8K9V437oyEMZT6JDv5LlPXh61RPmgeoVcHLWWr7k/edit#gid=874235844&range=G4
  // aka. On 17 Cool Auto SwingV off SwingH off
  const uint8_t start[43] = {
      0x01, 0x10, 0x00, 0x40, 0xBF, 0xFF, 0x00, 0xCC, 0x33, 0x92, 0x6D, 0x44,
      0xBB, 0x44, 0xBB, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
      0xFF, 0x53, 0xAC, 0xF1, 0x0E, 0x00, 0xFF, 0x00, 0xFF, 0x80, 0x7F, 0x03,
      0xFC, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF};
  ac.setRaw(start);
  EXPECT_FALSE(ac.getSwingV());
  const uint8_t turn_on_swingv[43] = {
      0x01, 0x10, 0x00, 0x40, 0xBF, 0xFF, 0x00, 0xCC, 0x33, 0x92, 0x6D, 0x81,
      0x7E, 0x44, 0xBB, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
      0xFF, 0x53, 0xAC, 0xF1, 0x0E, 0x00, 0xFF, 0x00, 0xFF, 0x80, 0x7F, 0x03,
      0xFC, 0x20, 0xDF, 0x00, 0xFF, 0x00, 0xFF};
  ac.setSwingV(true);  // Turn it on.
  EXPECT_TRUE(ac.getSwingV());
  EXPECT_STATE_EQ(turn_on_swingv, ac.getRaw(), kHitachiAc344Bits);
  ac.setSwingV(false);
  EXPECT_FALSE(ac.getSwingV());
}
