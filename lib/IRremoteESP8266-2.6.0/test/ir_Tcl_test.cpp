// Copyright 2019 David Conran

#include "ir_Tcl.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// General housekeeping
TEST(TestTcl112Ac, Housekeeping) {
  ASSERT_EQ("TCL112AC", typeToString(TCL112AC));
  ASSERT_TRUE(hasACState(TCL112AC));
}

// Tests for decodeTcl112Ac().

// Decode a real Tcl112Ac A/C example from Issue #619
TEST(TestDecodeTcl112Ac, DecodeRealExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  // Tcl112Ac A/C example from Issue #619 On.txt
  uint16_t rawData[227] = {
      3030, 1658, 494,  1066, 494,  1068, 498,  320,  494,
      326,  498,  320,  494,  1068, 500,  320,  494,  332,
      494,  1068, 500,  1062, 496,  324,  492,  1044, 524,
      322,  492,  326,  498,  1062, 494,  1074, 494,  326,
      500,  1062, 496,  1066, 490,  328,  496,  322,  492,
      1070, 498,  322,  494,  332,  492,  1068, 498,  320,
      494,  326,  498,  320,  496,  324,  500,  320,  494,
      324,  490,  336,  500,  320,  496,  324,  490,  328,
      496,  322,  492,  328,  498,  322,  492,  326,  498,
      328,  496,  322,  492,  328,  498,  1064, 494,  326,
      498,  320,  494,  1066, 490,  330,  496,  330,  494,
      1066, 490,  1070, 498,  322,  492,  328,  498,  322,
      492,  326,  498,  322,  492,  332,  492,  1068, 498,
      1062, 494,  1066, 500,  318,  496,  324,  490,  328,
      496,  324,  492,  334,  490,  328,  496,  324,  492,
      328,  496,  322,  492,  328,  498,  320,  494,  1068,
      500,  326,  500,  320,  492,  326,  500,  320,  496,
      324,  500,  318,  496,  324,  490,  328,  496,  330,
      496,  324,  490,  328,  496,  324,  490,  328,  498,
      322,  492,  328,  498,  320,  492,  334,  492,  328,
      498,  322,  494,  326,  498,  320,  494,  324,  500,
      322,  492,  324,  490,  336,  498,  320,  494,  324,
      500,  320,  496,  324,  490,  328,  498,  322,  492,
      328,  496,  1070, 496,  1064, 492,  1070, 498,  322,
      494,  326,  500,  320,  494,  324,  500,  320,  494,
      324,  470};  // UNKNOWN CE60D6B9
  uint8_t expectedState[kTcl112AcStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03,
      0x07, 0x40, 0x00, 0x00, 0x00, 0x80, 0x03};

  irsend.sendRaw(rawData, 227, 38000);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(TCL112AC, irsend.capture.decode_type);
  EXPECT_EQ(kTcl112AcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);

  IRTcl112Ac ac(0);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: On, Mode: 3 (COOL), Temp: 24C, Fan: 0 (Auto), Econo: Off, "
      "Health: Off, Light: On, Turbo: Off, Swing (H): Off, Swing (V): Off",
      ac.toString());
}

// Decode a synthetic Tcl112Ac A/C example from Issue #619
TEST(TestDecodeTcl112Ac, DecodeSyntheticExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();

  uint8_t expectedState[kTcl112AcStateLength] = {0x23, 0xCB, 0x26, 0x01, 0x00,
                                                 0x24, 0x03, 0x07, 0x40, 0x00,
                                                 0x00, 0x00, 0x80, 0x03};

  irsend.sendTcl112Ac(expectedState);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(TCL112AC, irsend.capture.decode_type);
  EXPECT_EQ(kTcl112AcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
}

TEST(TestTcl112AcClass, Temperature) {
  const uint8_t temp16C[kTcl112AcStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03,
      0x0F, 0x00, 0x00, 0x00, 0x00, 0x80, 0xCB};
  const uint8_t temp16point5C[kTcl112AcStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03,
      0x0F, 0x00, 0x00, 0x00, 0x00, 0xA0, 0xEB};
  const uint8_t temp19point5C[kTcl112AcStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03,
      0x0C, 0x00, 0x00, 0x00, 0x00, 0xA0, 0xE8};
  const uint8_t temp31C[kTcl112AcStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xBC};
  IRTcl112Ac ac(0);
  ac.setRaw(temp16C);
  EXPECT_EQ(
      "Power: On, Mode: 3 (COOL), Temp: 16C, Fan: 0 (Auto), Econo: Off, "
      "Health: Off, Light: On, Turbo: Off, Swing (H): Off, Swing (V): Off",
      ac.toString());
  ac.setRaw(temp16point5C);
  EXPECT_EQ(
      "Power: On, Mode: 3 (COOL), Temp: 16.5C, Fan: 0 (Auto), Econo: Off, "
      "Health: Off, Light: On, Turbo: Off, Swing (H): Off, Swing (V): Off",
      ac.toString());
  ac.setRaw(temp19point5C);
  EXPECT_EQ(
      "Power: On, Mode: 3 (COOL), Temp: 19.5C, Fan: 0 (Auto), Econo: Off, "
      "Health: Off, Light: On, Turbo: Off, Swing (H): Off, Swing (V): Off",
      ac.toString());
  ac.setRaw(temp31C);
  EXPECT_EQ(
      "Power: On, Mode: 3 (COOL), Temp: 31C, Fan: 0 (Auto), Econo: Off, "
      "Health: Off, Light: On, Turbo: Off, Swing (H): Off, Swing (V): Off",
      ac.toString());

  ac.setTemp(kTcl112AcTempMin);
  EXPECT_EQ(kTcl112AcTempMin, ac.getTemp());

  ac.setTemp(kTcl112AcTempMin + 1);
  EXPECT_EQ(kTcl112AcTempMin + 1, ac.getTemp());

  ac.setTemp(kTcl112AcTempMax);
  EXPECT_EQ(kTcl112AcTempMax, ac.getTemp());

  ac.setTemp(kTcl112AcTempMin - 1);
  EXPECT_EQ(kTcl112AcTempMin, ac.getTemp());

  ac.setTemp(kTcl112AcTempMax + 0.5);
  EXPECT_EQ(kTcl112AcTempMax, ac.getTemp());

  ac.setTemp(23);
  EXPECT_EQ(23, ac.getTemp());

  ac.setTemp(27.4);
  EXPECT_EQ(27, ac.getTemp());

  ac.setTemp(22.5);
  EXPECT_EQ(22.5, ac.getTemp());

  ac.setTemp(25.6);
  EXPECT_EQ(25.5, ac.getTemp());

  ac.setTemp(0);
  EXPECT_EQ(kTcl112AcTempMin, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kTcl112AcTempMax, ac.getTemp());
}

TEST(TestTcl112AcClass, OperatingMode) {
  IRTcl112Ac ac(0);
  ac.begin();

  ac.setMode(kTcl112AcAuto);
  EXPECT_EQ(kTcl112AcAuto, ac.getMode());

  ac.setMode(kTcl112AcCool);
  EXPECT_EQ(kTcl112AcCool, ac.getMode());

  ac.setMode(kTcl112AcHeat);
  EXPECT_EQ(kTcl112AcHeat, ac.getMode());

  ac.setFan(kTcl112AcFanAuto);
  ac.setMode(kTcl112AcFan);  // Should set fan speed to High.
  EXPECT_EQ(kTcl112AcFan, ac.getMode());
  EXPECT_EQ(kTcl112AcFanHigh, ac.getFan());

  ac.setMode(kTcl112AcDry);
  EXPECT_EQ(kTcl112AcDry, ac.getMode());

  ac.setMode(kTcl112AcHeat - 1);
  EXPECT_EQ(kTcl112AcAuto, ac.getMode());

  ac.setMode(kTcl112AcCool);
  EXPECT_EQ(kTcl112AcCool, ac.getMode());

  ac.setMode(kTcl112AcAuto + 1);
  EXPECT_EQ(kTcl112AcAuto, ac.getMode());

  ac.setMode(kTcl112AcCool);
  ac.setMode(255);
  EXPECT_EQ(kTcl112AcAuto, ac.getMode());

  ac.setMode(kTcl112AcCool);
  ac.setMode(0);
  EXPECT_EQ(kTcl112AcAuto, ac.getMode());

  const uint8_t automode[] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x08,
      0x07, 0x00, 0x00, 0x00, 0x00, 0x80, 0x48};
  ac.setRaw(automode);
  EXPECT_EQ(
      "Power: On, Mode: 8 (AUTO), Temp: 24C, Fan: 0 (Auto), Econo: Off, "
      "Health: Off, Light: On, Turbo: Off, Swing (H): Off, Swing (V): Off",
      ac.toString());
}

TEST(TestTcl112AcClass, Power) {
  IRTcl112Ac ac(0);
  ac.begin();

  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());

  ac.setPower(false);
  EXPECT_EQ(false, ac.getPower());

  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());

  ac.off();
  EXPECT_EQ(false, ac.getPower());

  ac.on();
  EXPECT_TRUE(ac.getPower());

  const uint8_t on[kTcl112AcStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03,
      0x0F, 0x00, 0x00, 0x00, 0x00, 0x80, 0xCB};
  ac.setRaw(on);
  EXPECT_EQ(
      "Power: On, Mode: 3 (COOL), Temp: 16C, Fan: 0 (Auto), Econo: Off, "
      "Health: Off, Light: On, Turbo: Off, Swing (H): Off, Swing (V): Off",
      ac.toString());

  const uint8_t off[kTcl112AcStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x20, 0x03,
      0x07, 0x40, 0x00, 0x00, 0x00, 0x80, 0xCB};
  ac.setRaw(off);
  EXPECT_EQ(
      "Power: Off, Mode: 3 (COOL), Temp: 24C, Fan: 0 (Auto), Econo: Off, "
      "Health: Off, Light: On, Turbo: Off, Swing (H): Off, Swing (V): Off",
      ac.toString());
}


TEST(TestTcl112AcClass, Checksum) {
  uint8_t temp16C[kTcl112AcStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03,
      0x0F, 0x00, 0x00, 0x00, 0x00, 0x80, 0xCB};
  uint8_t temp31C[kTcl112AcStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xBC};
  IRTcl112Ac ac(0);
  EXPECT_EQ(0xCB, ac.calcChecksum(temp16C));
  ac.setRaw(temp16C);
  EXPECT_EQ(
      "Power: On, Mode: 3 (COOL), Temp: 16C, Fan: 0 (Auto), Econo: Off, "
      "Health: Off, Light: On, Turbo: Off, Swing (H): Off, Swing (V): Off",
      ac.toString());
  ac.setRaw(temp31C);
  EXPECT_EQ(
      "Power: On, Mode: 3 (COOL), Temp: 31C, Fan: 0 (Auto), Econo: Off, "
      "Health: Off, Light: On, Turbo: Off, Swing (H): Off, Swing (V): Off",
      ac.toString());
  EXPECT_EQ(0xBC, ac.calcChecksum(temp31C));

  EXPECT_TRUE(IRTcl112Ac::validChecksum(temp16C));
  EXPECT_TRUE(IRTcl112Ac::validChecksum(temp31C));
  EXPECT_TRUE(ac.validChecksum(temp31C));
  ac.setRaw(temp16C);
  EXPECT_TRUE(ac.validChecksum(ac.getRaw()));
  ac.setTemp(31);
  EXPECT_TRUE(ac.validChecksum(ac.getRaw()));
  EXPECT_EQ(0xBC, ac.calcChecksum(ac.getRaw()));
}

TEST(TestTcl112AcClass, Econo) {
  IRTcl112Ac ac(0);
  ac.begin();

  ac.setEcono(true);
  EXPECT_TRUE(ac.getEcono());
  ac.setEcono(false);
  EXPECT_EQ(false, ac.getEcono());
  ac.setEcono(true);
  EXPECT_TRUE(ac.getEcono());
}

TEST(TestTcl112AcClass, Health) {
  IRTcl112Ac ac(0);
  ac.begin();

  ac.setHealth(true);
  EXPECT_TRUE(ac.getHealth());
  ac.setHealth(false);
  EXPECT_EQ(false, ac.getHealth());
  ac.setHealth(true);
  EXPECT_TRUE(ac.getHealth());
}

TEST(TestTcl112AcClass, Light) {
  IRTcl112Ac ac(0);
  ac.begin();

  ac.setLight(true);
  EXPECT_TRUE(ac.getLight());
  ac.setLight(false);
  EXPECT_EQ(false, ac.getLight());
  ac.setLight(true);
  EXPECT_TRUE(ac.getLight());
}

TEST(TestTcl112AcClass, SwingHorizontal) {
  IRTcl112Ac ac(0);
  ac.begin();

  ac.setSwingHorizontal(true);
  EXPECT_TRUE(ac.getSwingHorizontal());
  ac.setSwingHorizontal(false);
  EXPECT_EQ(false, ac.getSwingHorizontal());
  ac.setSwingHorizontal(true);
  EXPECT_TRUE(ac.getSwingHorizontal());
}

TEST(TestTcl112AcClass, SwingVertical) {
  IRTcl112Ac ac(0);
  ac.begin();

  ac.setSwingVertical(true);
  EXPECT_TRUE(ac.getSwingVertical());
  ac.setSwingVertical(false);
  EXPECT_EQ(false, ac.getSwingVertical());
  ac.setSwingVertical(true);
  EXPECT_TRUE(ac.getSwingVertical());
}

TEST(TestTcl112AcClass, Turbo) {
  IRTcl112Ac ac(0);
  ac.begin();

  ac.setFan(kTcl112AcFanLow);
  ac.setSwingHorizontal(false);

  ac.setTurbo(false);
  EXPECT_FALSE(ac.getTurbo());
  EXPECT_FALSE(ac.getSwingVertical());
  EXPECT_EQ(kTcl112AcFanLow, ac.getFan());

  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());
  EXPECT_TRUE(ac.getSwingVertical());
  EXPECT_EQ(kTcl112AcFanHigh, ac.getFan());

  ac.setTurbo(false);
  EXPECT_FALSE(ac.getTurbo());
  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());
}

TEST(TestTcl112AcClass, FanSpeed) {
  IRTcl112Ac ac(0);
  ac.begin();

  // Unexpected value should default to Auto.
  ac.setFan(255);
  EXPECT_EQ(kTcl112AcFanAuto, ac.getFan());

  ac.setFan(kTcl112AcFanLow);
  EXPECT_EQ(kTcl112AcFanLow, ac.getFan());
  ac.setFan(kTcl112AcFanMed);
  EXPECT_EQ(kTcl112AcFanMed, ac.getFan());
  ac.setFan(kTcl112AcFanHigh);
  EXPECT_EQ(kTcl112AcFanHigh, ac.getFan());
  ac.setFan(kTcl112AcFanAuto);
  EXPECT_EQ(kTcl112AcFanAuto, ac.getFan());

  // Beyond High should default to Auto.
  ac.setFan(kTcl112AcFanHigh + 1);
  EXPECT_EQ(kTcl112AcFanAuto, ac.getFan());
}
