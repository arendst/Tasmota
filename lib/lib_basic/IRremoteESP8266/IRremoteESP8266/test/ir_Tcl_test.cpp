// Copyright 2019 David Conran

#include "ir_Tcl.h"
#include "IRac.h"
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
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
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
  EXPECT_EQ(
      "Type: 1, Power: On, Mode: 3 (Cool), Temp: 24C, Fan: 0 (Auto), "
      "Econo: Off, Health: Off, Turbo: Off, Swing(H): Off, Swing(V): Off, "
      "Light: On",
      IRAcUtils::resultAcToString(&irsend.capture));
}

// Decode a synthetic Tcl112Ac A/C example from Issue #619
TEST(TestDecodeTcl112Ac, DecodeSyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
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
  IRTcl112Ac ac(kGpioUnused);
  ac.setRaw(temp16C);
  EXPECT_EQ(
      "Type: 1, Power: On, Mode: 3 (Cool), Temp: 16C, Fan: 0 (Auto), "
      "Econo: Off, Health: Off, Turbo: Off, Swing(H): Off, Swing(V): Off, "
      "Light: On",
      ac.toString());
  ac.setRaw(temp16point5C);
  EXPECT_EQ(
      "Type: 1, Power: On, Mode: 3 (Cool), Temp: 16.5C, Fan: 0 (Auto), "
      "Econo: Off, Health: Off, Turbo: Off, Swing(H): Off, Swing(V): Off, "
      "Light: On",
      ac.toString());
  ac.setRaw(temp19point5C);
  EXPECT_EQ(
      "Type: 1, Power: On, Mode: 3 (Cool), Temp: 19.5C, Fan: 0 (Auto), "
      "Econo: Off, Health: Off, Turbo: Off, Swing(H): Off, Swing(V): Off, "
      "Light: On",
      ac.toString());
  ac.setRaw(temp31C);
  EXPECT_EQ(
      "Type: 1, Power: On, Mode: 3 (Cool), Temp: 31C, Fan: 0 (Auto), "
      "Econo: Off, Health: Off, Turbo: Off, Swing(H): Off, Swing(V): Off, "
      "Light: On",
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
  IRTcl112Ac ac(kGpioUnused);
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
      "Type: 1, Power: On, Mode: 8 (Auto), Temp: 24C, Fan: 0 (Auto), "
      "Econo: Off, Health: Off, Turbo: Off, Swing(H): Off, Swing(V): Off, "
      "Light: On",
      ac.toString());
}

TEST(TestTcl112AcClass, Power) {
  IRTcl112Ac ac(kGpioUnused);
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
      "Type: 1, Power: On, Mode: 3 (Cool), Temp: 16C, Fan: 0 (Auto), "
      "Econo: Off, Health: Off, Turbo: Off, Swing(H): Off, Swing(V): Off, "
      "Light: On",
      ac.toString());

  const uint8_t off[kTcl112AcStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x20, 0x03,
      0x07, 0x40, 0x00, 0x00, 0x00, 0x80, 0xCB};
  ac.setRaw(off);
  EXPECT_EQ(
      "Type: 1, Power: Off, Mode: 3 (Cool), Temp: 24C, Fan: 0 (Auto), "
      "Econo: Off, Health: Off, Turbo: Off, Swing(H): Off, Swing(V): Off, "
      "Light: On",
      ac.toString());
}

TEST(TestTcl112AcClass, Checksum) {
  uint8_t temp16C[kTcl112AcStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03,
      0x0F, 0x00, 0x00, 0x00, 0x00, 0x80, 0xCB};
  uint8_t temp31C[kTcl112AcStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xBC};
  IRTcl112Ac ac(kGpioUnused);
  EXPECT_EQ(0xCB, ac.calcChecksum(temp16C));
  ac.setRaw(temp16C);
  EXPECT_EQ(
      "Type: 1, Power: On, Mode: 3 (Cool), Temp: 16C, Fan: 0 (Auto), "
      "Econo: Off, Health: Off, Turbo: Off, Swing(H): Off, Swing(V): Off, "
      "Light: On",
      ac.toString());
  ac.setRaw(temp31C);
  EXPECT_EQ(
      "Type: 1, Power: On, Mode: 3 (Cool), Temp: 31C, Fan: 0 (Auto), "
      "Econo: Off, Health: Off, Turbo: Off, Swing(H): Off, Swing(V): Off, "
      "Light: On",
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
  IRTcl112Ac ac(kGpioUnused);
  ac.begin();

  ac.setEcono(true);
  EXPECT_TRUE(ac.getEcono());
  ac.setEcono(false);
  EXPECT_EQ(false, ac.getEcono());
  ac.setEcono(true);
  EXPECT_TRUE(ac.getEcono());
}

TEST(TestTcl112AcClass, Health) {
  IRTcl112Ac ac(kGpioUnused);
  ac.begin();

  ac.setHealth(true);
  EXPECT_TRUE(ac.getHealth());
  ac.setHealth(false);
  EXPECT_EQ(false, ac.getHealth());
  ac.setHealth(true);
  EXPECT_TRUE(ac.getHealth());
}

TEST(TestTcl112AcClass, Light) {
  IRTcl112Ac ac(kGpioUnused);
  ac.begin();

  ac.setLight(true);
  EXPECT_TRUE(ac.getLight());
  ac.setLight(false);
  EXPECT_EQ(false, ac.getLight());
  ac.setLight(true);
  EXPECT_TRUE(ac.getLight());
}

TEST(TestTcl112AcClass, SwingHorizontal) {
  IRTcl112Ac ac(kGpioUnused);
  ac.begin();

  ac.setSwingHorizontal(true);
  EXPECT_TRUE(ac.getSwingHorizontal());
  ac.setSwingHorizontal(false);
  EXPECT_EQ(false, ac.getSwingHorizontal());
  ac.setSwingHorizontal(true);
  EXPECT_TRUE(ac.getSwingHorizontal());
}

TEST(TestTcl112AcClass, SwingVertical) {
  IRTcl112Ac ac(kGpioUnused);
  ac.begin();

  ac.setSwingVertical(true);
  EXPECT_TRUE(ac.getSwingVertical());
  ac.setSwingVertical(false);
  EXPECT_EQ(false, ac.getSwingVertical());
  ac.setSwingVertical(true);
  EXPECT_TRUE(ac.getSwingVertical());
}

TEST(TestTcl112AcClass, Turbo) {
  IRTcl112Ac ac(kGpioUnused);
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
  IRTcl112Ac ac(kGpioUnused);
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

TEST(TestTcl112AcClass, Quiet_Mute) {
  IRTcl112Ac ac(kGpioUnused);
  ac.begin();

  EXPECT_FALSE(ac.getQuiet());
  EXPECT_FALSE(ac.getQuiet(false));
  EXPECT_TRUE(ac.getQuiet(true));
  ac.setQuiet(true);
  EXPECT_TRUE(ac.getQuiet());
  ac.setQuiet(false);
  EXPECT_FALSE(ac.getQuiet());
}

TEST(TestTcl112AcClass, toCommon) {
  IRTcl112Ac ac(kGpioUnused);
  ac.setPower(true);
  ac.setMode(kTcl112AcCool);
  ac.setTemp(20);
  ac.setFan(kTcl112AcFanHigh);
  ac.setSwingVertical(true);
  ac.setSwingHorizontal(true);
  ac.setTurbo(true);
  ac.setHealth(true);
  ac.setEcono(true);
  ac.setLight(true);
  ac.setQuiet(false);
  // Now test it.
  ASSERT_EQ(decode_type_t::TCL112AC, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kAuto, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kAuto, ac.toCommon().swingh);
  ASSERT_TRUE(ac.toCommon().turbo);
  ASSERT_TRUE(ac.toCommon().econo);
  ASSERT_TRUE(ac.toCommon().light);
  ASSERT_TRUE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().quiet);
  // Unsupported.
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);

  // Tests for the quiet setting.
  stdAc::state_t prev = ac.toCommon();
  prev.quiet = true;
  ASSERT_FALSE(ac.toCommon(&prev).quiet);
  ac.stateReset();
  // If the current quiet setting hasn't been explicitly set, use the previous.
  ASSERT_TRUE(ac.toCommon(&prev).quiet);
}

TEST(TestDecodeTcl112Ac, Issue744) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  uint16_t rawData[227] = {
      3164, 1532, 584, 1082, 472, 1068, 580, 244, 602, 264, 542, 328, 530, 1034,
      586, 262, 540, 326, 508, 1064, 582, 1082, 490, 328, 532, 1032, 586, 262,
      544, 352, 478, 1060, 584, 1082, 486, 328, 502, 1058, 588, 1084, 472, 344,
      530, 250, 600, 1086, 492, 322, 530, 258, 594, 1082, 494, 318, 510, 344,
      530, 248, 600, 262, 544, 326, 504, 296, 578, 252, 598, 260, 550, 318, 506,
      344, 530, 250, 600, 258, 546, 318, 508, 342, 532, 254, 596, 236, 606, 266,
      524, 1066, 580, 242, 602, 266, 542, 1054, 574, 246, 604, 262, 550, 1088,
      530, 1034, 588, 262, 542, 328, 504, 296, 582, 238, 606, 262, 546, 322,
      508, 342, 530, 250, 602, 260, 544, 1052, 572, 252, 600, 260, 546, 320,
      506, 344, 530, 254, 596, 264, 578, 268, 552, 316, 528, 256, 598, 260, 578,
      272, 520, 372, 476, 294, 582, 240, 604, 266, 542, 328, 502, 294, 582, 238,
      604, 268, 540, 322, 506, 346, 530, 244, 604, 260, 542, 354, 478, 298, 580,
      240, 604, 262, 542, 326, 506, 342, 530, 250, 600, 260, 548, 318, 506, 344,
      530, 250, 600, 260, 546, 320, 528, 322, 530, 254, 598, 262, 548, 316, 468,
      380, 532, 250, 600, 260, 546, 1092, 500, 300, 578, 246, 602, 1082, 474,
      346, 530, 248, 602, 260, 542, 1054, 570, 1090, 524};  // UNKNOWN 3338FACE

  uint8_t expectedState[kTcl112AcStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03,
      0x08, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC4};

  irsend.sendRaw(rawData, 227, 38000);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(TCL112AC, irsend.capture.decode_type);
  EXPECT_EQ(kTcl112AcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);

  IRTcl112Ac ac(kGpioUnused);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Type: 1, Power: On, Mode: 3 (Cool), Temp: 23C, Fan: 0 (Auto), "
      "Econo: Off, Health: Off, Turbo: Off, Swing(H): Off, Swing(V): Off, "
      "Light: On",
      ac.toString());
}

TEST(TestDecodeTcl112Ac, Issue1528) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  const uint16_t rawData[227] = {
      3040, 1632, 500, 1084, 502, 1084, 500, 318, 474, 344, 474, 344, 472, 1110,
      474, 344, 474, 344, 472, 1110, 474, 1110, 474, 344, 472, 1112, 474, 344,
      472, 346, 470, 1112, 472, 1112, 472, 346, 470, 1114, 470, 1114, 470, 348,
      468, 348, 468, 1116, 470, 348, 468, 350, 464, 354, 424, 1158, 426, 392,
      424, 394, 424, 394, 424, 392, 424, 392, 424, 394, 424, 392, 424, 392, 424,
      394, 424, 392, 424, 392, 424, 394, 424, 392, 460, 358, 460, 358, 458, 358,
      458, 358, 460, 358, 460, 358, 460, 1124, 460, 1124, 460, 358, 460, 358,
      458, 358, 458, 360, 482, 334, 484, 334, 484, 334, 484, 334, 484, 334, 484,
      334, 484, 334, 486, 332, 484, 334, 484, 332, 484, 334, 484, 332, 484, 334,
      484, 332, 484, 332, 484, 332, 486, 332, 484, 332, 484, 334, 484, 334, 484,
      332, 484, 334, 484, 334, 484, 332, 484, 332, 486, 332, 484, 334, 484, 334,
      484, 334, 484, 334, 484, 334, 484, 334, 484, 334, 484, 334, 482, 334, 482,
      334, 482, 336, 482, 336, 482, 336, 482, 336, 482, 336, 482, 336, 482, 336,
      482, 336, 480, 336, 480, 338, 480, 338, 480, 338, 480, 336, 480, 338, 480,
      338, 480, 338, 480, 338, 478, 1104, 478, 340, 478, 1104, 480, 338, 478,
      340, 478, 340, 476, 340, 476, 1106, 478};  // UNKNOWN 2757C6B2

  uint8_t expectedState[kTcl112AcStateLength] = {
      0x23, 0xCB, 0x26, 0x02, 0x00, 0x60, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85};

  irsend.sendRaw(rawData, 227, 38000);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(TCL112AC, irsend.capture.decode_type);
  EXPECT_EQ(kTcl112AcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Type: 2, Quiet: On",
      IRAcUtils::resultAcToString(&irsend.capture));
}

// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1528#issuecomment-877640837
TEST(TestTcl112AcClass, SendingQuiet) {
  IRTcl112Ac ac(kGpioUnused);
  IRrecv capture(kGpioUnused);


  ac.begin();
  ac.on();
  ac.setTemp(24);
  ac.setLight(false);
  ac.setSwingHorizontal(true);
  ac.send();
  ac.setSwingHorizontal(false);
  EXPECT_FALSE(ac.getQuiet());
  ac.send();
  ac._irsend.reset();
  ac.setQuiet(true);
  EXPECT_TRUE(ac.getQuiet());
  ac.send();
  EXPECT_TRUE(ac.getQuiet());
  ac._irsend.makeDecodeResult();
  // We are expecting two messages, confirm we got at least that much data.
  EXPECT_EQ(1 + 228 + 228, ac._irsend.capture.rawlen);
  // First message.
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(TCL112AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kTcl112AcBits, ac._irsend.capture.bits);
  ASSERT_EQ(
      "Type: 2, Quiet: On",
      IRAcUtils::resultAcToString(&ac._irsend.capture));
  // Second message.
  // TCL112 uses the Mitsubishi112 decoder.
  // Skip first message.
  EXPECT_TRUE(capture.decodeMitsubishi112(&ac._irsend.capture, 229));
  ASSERT_EQ(TCL112AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kTcl112AcBits, ac._irsend.capture.bits);
  ASSERT_EQ(
      "Type: 1, Power: On, Mode: 3 (Cool), Temp: 24C, Fan: 0 (Auto), "
      "Econo: Off, Health: Off, Turbo: Off, Swing(H): Off, Swing(V): Off, "
      "Light: Off", IRAcUtils::resultAcToString(&ac._irsend.capture));
}
