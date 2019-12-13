// Copyright 2019 David Conran

#include "ir_MitsubishiHeavy.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// General housekeeping
TEST(TestMitsubishiHeavy, Housekeeping) {
  ASSERT_EQ("MITSUBISHI_HEAVY_88", typeToString(MITSUBISHI_HEAVY_88));
  ASSERT_TRUE(hasACState(MITSUBISHI_HEAVY_88));
  ASSERT_EQ("MITSUBISHI_HEAVY_152", typeToString(MITSUBISHI_HEAVY_152));
  ASSERT_TRUE(hasACState(MITSUBISHI_HEAVY_152));
}

// Tests for IRMitsubishiHeavy152Ac class.

TEST(TestMitsubishiHeavy152AcClass, Power) {
  IRMitsubishiHeavy152Ac ac(0);
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

TEST(TestMitsubishiHeavy152AcClass, Temperature) {
  IRMitsubishiHeavy152Ac ac(0);
  ac.begin();

  ac.setMode(kMitsubishiHeavyCool);

  ac.setTemp(0);
  EXPECT_EQ(kMitsubishiHeavyMinTemp, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kMitsubishiHeavyMaxTemp, ac.getTemp());

  ac.setTemp(kMitsubishiHeavyMinTemp);
  EXPECT_EQ(kMitsubishiHeavyMinTemp, ac.getTemp());

  ac.setTemp(kMitsubishiHeavyMaxTemp);
  EXPECT_EQ(kMitsubishiHeavyMaxTemp, ac.getTemp());

  ac.setTemp(kMitsubishiHeavyMinTemp - 1);
  EXPECT_EQ(kMitsubishiHeavyMinTemp, ac.getTemp());

  ac.setTemp(kMitsubishiHeavyMaxTemp + 1);
  EXPECT_EQ(kMitsubishiHeavyMaxTemp, ac.getTemp());

  ac.setTemp(19);
  EXPECT_EQ(19, ac.getTemp());

  ac.setTemp(21);
  EXPECT_EQ(21, ac.getTemp());

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());

  ac.setTemp(29);
  EXPECT_EQ(29, ac.getTemp());
}

TEST(TestMitsubishiHeavy152AcClass, OperatingMode) {
  IRMitsubishiHeavy152Ac ac(0);
  ac.begin();

  ac.setMode(kMitsubishiHeavyAuto);
  EXPECT_EQ(kMitsubishiHeavyAuto, ac.getMode());

  ac.setMode(kMitsubishiHeavyCool);
  EXPECT_EQ(kMitsubishiHeavyCool, ac.getMode());

  ac.setMode(kMitsubishiHeavyHeat);
  EXPECT_EQ(kMitsubishiHeavyHeat, ac.getMode());

  ac.setMode(kMitsubishiHeavyDry);
  EXPECT_EQ(kMitsubishiHeavyDry, ac.getMode());

  ac.setMode(kMitsubishiHeavyFan);
  EXPECT_EQ(kMitsubishiHeavyFan, ac.getMode());

  ac.setMode(kMitsubishiHeavyHeat + 1);
  EXPECT_EQ(kMitsubishiHeavyAuto, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kMitsubishiHeavyAuto, ac.getMode());
}


TEST(TestMitsubishiHeavy152AcClass, Filter) {
  IRMitsubishiHeavy152Ac ac(0);
  ac.begin();

  ac.setFilter(true);
  EXPECT_TRUE(ac.getFilter());

  ac.setFilter(false);
  EXPECT_FALSE(ac.getFilter());

  ac.setFilter(true);
  EXPECT_TRUE(ac.getFilter());
}

TEST(TestMitsubishiHeavy152AcClass, Turbo) {
  IRMitsubishiHeavy152Ac ac(0);
  ac.begin();

  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());

  ac.setTurbo(false);
  EXPECT_FALSE(ac.getTurbo());

  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());
}

TEST(TestMitsubishiHeavy152AcClass, Econo) {
  IRMitsubishiHeavy152Ac ac(0);
  ac.begin();

  ac.setEcono(true);
  EXPECT_TRUE(ac.getEcono());

  ac.setEcono(false);
  EXPECT_FALSE(ac.getEcono());

  ac.setEcono(true);
  EXPECT_TRUE(ac.getEcono());
}

TEST(TestMitsubishiHeavy152AcClass, 3D) {
  IRMitsubishiHeavy152Ac ac(0);
  ac.begin();

  ac.set3D(true);
  EXPECT_TRUE(ac.get3D());

  ac.set3D(false);
  EXPECT_FALSE(ac.get3D());

  ac.set3D(true);
  EXPECT_TRUE(ac.get3D());
}

TEST(TestMitsubishiHeavy152AcClass, Night) {
  IRMitsubishiHeavy152Ac ac(0);
  ac.begin();

  ac.setNight(true);
  EXPECT_TRUE(ac.getNight());

  ac.setNight(false);
  EXPECT_FALSE(ac.getNight());

  ac.setNight(true);
  EXPECT_TRUE(ac.getNight());
}

TEST(TestMitsubishiHeavy152AcClass, Clean) {
  IRMitsubishiHeavy152Ac ac(0);
  ac.begin();

  ac.setClean(true);
  EXPECT_TRUE(ac.getClean());

  ac.setClean(false);
  EXPECT_FALSE(ac.getClean());

  ac.setClean(true);
  EXPECT_TRUE(ac.getClean());
}

TEST(TestMitsubishiHeavy152AcClass, FanSpeed) {
  IRMitsubishiHeavy152Ac ac(0);
  ac.begin();

  ac.setFan(kMitsubishiHeavy152FanLow);
  EXPECT_EQ(kMitsubishiHeavy152FanLow, ac.getFan());

  ac.setFan(kMitsubishiHeavy152FanAuto);
  EXPECT_EQ(kMitsubishiHeavy152FanAuto, ac.getFan());


  ac.setFan(255);
  EXPECT_EQ(kMitsubishiHeavy152FanAuto, ac.getFan());

  ac.setFan(kMitsubishiHeavy152FanMax);
  EXPECT_EQ(kMitsubishiHeavy152FanMax, ac.getFan());

  ac.setFan(kMitsubishiHeavy152FanMax + 1);
  EXPECT_EQ(kMitsubishiHeavy152FanAuto, ac.getFan());

  ac.setFan(kMitsubishiHeavy152FanMax - 1);
  EXPECT_EQ(kMitsubishiHeavy152FanMax - 1, ac.getFan());

  ac.setFan(kMitsubishiHeavy152FanLow + 1);
  EXPECT_EQ(kMitsubishiHeavy152FanLow + 1, ac.getFan());

  ac.setFan(kMitsubishiHeavy152FanEcono);
  EXPECT_EQ(kMitsubishiHeavy152FanEcono, ac.getFan());

  ac.setFan(kMitsubishiHeavy152FanTurbo);
  EXPECT_EQ(kMitsubishiHeavy152FanTurbo, ac.getFan());
}

TEST(TestMitsubishiHeavy152AcClass, VerticalSwing) {
  IRMitsubishiHeavy152Ac ac(0);
  ac.begin();
  ac.setSwingVertical(kMitsubishiHeavy152SwingVAuto);
  EXPECT_EQ(kMitsubishiHeavy152SwingVAuto, ac.getSwingVertical());

  ac.setSwingVertical(kMitsubishiHeavy152SwingVHighest);
  EXPECT_EQ(kMitsubishiHeavy152SwingVHighest, ac.getSwingVertical());

  ac.setSwingVertical(kMitsubishiHeavy152SwingVHighest + 1);
  EXPECT_EQ(kMitsubishiHeavy152SwingVHighest + 1, ac.getSwingVertical());

  ac.setSwingVertical(kMitsubishiHeavy152SwingVOff);
  EXPECT_EQ(kMitsubishiHeavy152SwingVOff, ac.getSwingVertical());

  ac.setSwingVertical(kMitsubishiHeavy152SwingVOff + 1);
  EXPECT_EQ(kMitsubishiHeavy152SwingVOff, ac.getSwingVertical());

  // Out of bounds.
  ac.setSwingVertical(255);
  EXPECT_EQ(kMitsubishiHeavy152SwingVOff, ac.getSwingVertical());
}

TEST(TestMitsubishiHeavy152AcClass, HorizontalSwing) {
  IRMitsubishiHeavy152Ac ac(0);
  ac.begin();
  ac.setSwingHorizontal(kMitsubishiHeavy152SwingHAuto);
  EXPECT_EQ(kMitsubishiHeavy152SwingHAuto, ac.getSwingHorizontal());

  ac.setSwingHorizontal(kMitsubishiHeavy152SwingHLeftMax);
  EXPECT_EQ(kMitsubishiHeavy152SwingHLeftMax, ac.getSwingHorizontal());

  ac.setSwingHorizontal(kMitsubishiHeavy152SwingHLeftMax + 1);
  EXPECT_EQ(kMitsubishiHeavy152SwingHLeftMax + 1, ac.getSwingHorizontal());

  ac.setSwingHorizontal(kMitsubishiHeavy152SwingHRightMax);
  EXPECT_EQ(kMitsubishiHeavy152SwingHRightMax, ac.getSwingHorizontal());

  ac.setSwingHorizontal(kMitsubishiHeavy152SwingHRightMax - 1);
  EXPECT_EQ(kMitsubishiHeavy152SwingHRightMax - 1, ac.getSwingHorizontal());

  ac.setSwingHorizontal(kMitsubishiHeavy152SwingHOff);
  EXPECT_EQ(kMitsubishiHeavy152SwingHOff, ac.getSwingHorizontal());

  ac.setSwingHorizontal(kMitsubishiHeavy152SwingHOff + 1);
  EXPECT_EQ(kMitsubishiHeavy152SwingHOff, ac.getSwingHorizontal());

  // Out of bounds.
  ac.setSwingHorizontal(255);
  EXPECT_EQ(kMitsubishiHeavy152SwingHOff, ac.getSwingHorizontal());
}

TEST(TestMitsubishiHeavy152AcClass, Checksums) {
  IRMitsubishiHeavy152Ac ac(0);
  ac.begin();

  EXPECT_TRUE(ac.validChecksum(ac.getRaw()));

  uint8_t expected[kMitsubishiHeavy152StateLength] = {
      0xAD, 0x51, 0x3C, 0xE5, 0x1A, 0x0C, 0xF3, 0x07,
      0xF8, 0x04, 0xFB, 0x00, 0xFF, 0x00, 0xFF, 0x00,
      0xFF, 0x80, 0x7F};
  EXPECT_TRUE(IRMitsubishiHeavy152Ac::validChecksum(expected));

  // Screw up the "checksum" to test it fails.
  expected[kMitsubishiHeavy152StateLength - 1] = 0x55;
  EXPECT_FALSE(IRMitsubishiHeavy152Ac::validChecksum(expected));
  // getting the after getRaw() should repair it.
  ac.setRaw(expected);
  EXPECT_TRUE(ac.validChecksum(ac.getRaw()));
  EXPECT_TRUE(IRMitsubishiHeavy152Ac::validChecksum(ac.getRaw()));
}

TEST(TestMitsubishiHeavy152AcClass, HumanReadable) {
  IRMitsubishiHeavy152Ac ac(0);

  EXPECT_EQ(
      "Power: Off, Mode: 0 (Auto), Temp: 17C, Fan: 0 (Auto), "
      "Swing(V): 0 (Auto), Swing(H): 0 (Auto), Silent: Off, Turbo: Off, "
      "Econo: Off, Night: Off, Filter: Off, 3D: Off, Clean: Off",
      ac.toString());
  ac.on();
  ac.setMode(kMitsubishiHeavyCool);
  ac.setTemp(kMitsubishiHeavyMinTemp);
  ac.setFan(kMitsubishiHeavy152FanMax);
  ac.setFilter(true);
  ac.setNight(true);
  ac.setTurbo(false);
  ac.setSilent(true);
  ac.setEcono(false);
  ac.set3D(true);
  ac.setSwingVertical(kMitsubishiHeavy152SwingVAuto);
  ac.setSwingHorizontal(kMitsubishiHeavy152SwingHAuto);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 17C, Fan: 4 (Max), "
      "Swing(V): 0 (Auto), Swing(H): 0 (Auto), Silent: On, Turbo: Off, "
      "Econo: Off, Night: On, Filter: On, 3D: On, Clean: Off",
      ac.toString());

  ac.setMode(kMitsubishiHeavyHeat);
  ac.setTemp(kMitsubishiHeavyMaxTemp);
  ac.setFilter(true);
  ac.setNight(false);
  ac.setTurbo(true);
  ac.setEcono(false);
  ac.setSilent(false);
  ac.set3D(false);
  ac.setSwingVertical(kMitsubishiHeavy152SwingVLowest);
  ac.setSwingHorizontal(kMitsubishiHeavy152SwingHLeftMax);

  EXPECT_EQ(
      "Power: On, Mode: 4 (Heat), Temp: 31C, Fan: 8 (Turbo), "
      "Swing(V): 5 (Lowest), Swing(H): 1 (Max Left), Silent: Off, Turbo: On, "
      "Econo: Off, Night: Off, Filter: On, 3D: Off, Clean: Off",
      ac.toString());

  ac.setClean(true);
  ac.setEcono(true);
  ac.setMode(kMitsubishiHeavyAuto);
  ac.setSwingVertical(kMitsubishiHeavy152SwingVOff);

  EXPECT_EQ(
      "Power: On, Mode: 0 (Auto), Temp: 31C, Fan: 6 (Econo), "
      "Swing(V): 6 (Off), Swing(H): 1 (Max Left), Silent: Off, "
      "Turbo: Off, Econo: On, Night: Off, Filter: On, 3D: Off, Clean: On",
      ac.toString());

  ac.setClean(false);
  ac.setTemp(25);
  ac.setEcono(false);
  ac.setMode(kMitsubishiHeavyDry);
  ac.setSwingHorizontal(kMitsubishiHeavy152SwingHLeftRight);
  EXPECT_EQ(
      "Power: On, Mode: 2 (Dry), Temp: 25C, Fan: 0 (Auto), "
      "Swing(V): 6 (Off), Swing(H): 7 (Left Right), Silent: Off, "
      "Turbo: Off, Econo: Off, Night: Off, Filter: Off, 3D: Off, Clean: Off",
      ac.toString());
}

TEST(TestMitsubishiHeavy152AcClass, ReconstructKnownExample) {
  IRMitsubishiHeavy152Ac ac(0);

  EXPECT_EQ(
      "Power: Off, Mode: 0 (Auto), Temp: 17C, Fan: 0 (Auto), "
      "Swing(V): 0 (Auto), Swing(H): 0 (Auto), Silent: Off, Turbo: Off, "
      "Econo: Off, Night: Off, Filter: Off, 3D: Off, Clean: Off",
      ac.toString());
  ac.on();
  ac.setMode(kMitsubishiHeavyHeat);
  ac.setTemp(24);
  ac.setFan(kMitsubishiHeavy152FanMax);
  ac.setFilter(true);
  ac.setNight(false);
  ac.setTurbo(false);
  ac.setSilent(false);
  ac.setEcono(false);
  ac.set3D(false);
  ac.setClean(false);
  ac.setSwingVertical(kMitsubishiHeavy152SwingVAuto);
  ac.setSwingHorizontal(kMitsubishiHeavy152SwingHAuto);
  EXPECT_EQ(
      "Power: On, Mode: 4 (Heat), Temp: 24C, Fan: 4 (Max), "
      "Swing(V): 0 (Auto), Swing(H): 0 (Auto), Silent: Off, Turbo: Off, "
      "Econo: Off, Night: Off, Filter: Off, 3D: Off, Clean: Off",
      ac.toString());

  uint8_t expected[kMitsubishiHeavy152StateLength] = {
      0xAD, 0x51, 0x3C, 0xE5, 0x1A, 0x0C, 0xF3, 0x07,
      0xF8, 0x04, 0xFB, 0x00, 0xFF, 0x00, 0xFF, 0x00,
      0xFF, 0x80, 0x7F};
  EXPECT_STATE_EQ(expected, ac.getRaw(), kMitsubishiHeavy152Bits);
}

// Tests for IRMitsubishiHeavy88Ac class.

TEST(TestMitsubishiHeavy88AcClass, Power) {
  IRMitsubishiHeavy88Ac ac(0);
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

TEST(TestMitsubishiHeavy88AcClass, Temperature) {
  IRMitsubishiHeavy88Ac ac(0);
  ac.begin();

  ac.setMode(kMitsubishiHeavyCool);

  ac.setTemp(0);
  EXPECT_EQ(kMitsubishiHeavyMinTemp, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kMitsubishiHeavyMaxTemp, ac.getTemp());

  ac.setTemp(kMitsubishiHeavyMinTemp);
  EXPECT_EQ(kMitsubishiHeavyMinTemp, ac.getTemp());

  ac.setTemp(kMitsubishiHeavyMaxTemp);
  EXPECT_EQ(kMitsubishiHeavyMaxTemp, ac.getTemp());

  ac.setTemp(kMitsubishiHeavyMinTemp - 1);
  EXPECT_EQ(kMitsubishiHeavyMinTemp, ac.getTemp());

  ac.setTemp(kMitsubishiHeavyMaxTemp + 1);
  EXPECT_EQ(kMitsubishiHeavyMaxTemp, ac.getTemp());

  ac.setTemp(19);
  EXPECT_EQ(19, ac.getTemp());

  ac.setTemp(21);
  EXPECT_EQ(21, ac.getTemp());

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());

  ac.setTemp(29);
  EXPECT_EQ(29, ac.getTemp());
}

TEST(TestMitsubishiHeavy88AcClass, OperatingMode) {
  IRMitsubishiHeavy88Ac ac(0);
  ac.begin();

  ac.setMode(kMitsubishiHeavyAuto);
  EXPECT_EQ(kMitsubishiHeavyAuto, ac.getMode());

  ac.setMode(kMitsubishiHeavyCool);
  EXPECT_EQ(kMitsubishiHeavyCool, ac.getMode());

  ac.setMode(kMitsubishiHeavyHeat);
  EXPECT_EQ(kMitsubishiHeavyHeat, ac.getMode());

  ac.setMode(kMitsubishiHeavyDry);
  EXPECT_EQ(kMitsubishiHeavyDry, ac.getMode());

  ac.setMode(kMitsubishiHeavyFan);
  EXPECT_EQ(kMitsubishiHeavyFan, ac.getMode());

  ac.setMode(kMitsubishiHeavyHeat + 1);
  EXPECT_EQ(kMitsubishiHeavyAuto, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kMitsubishiHeavyAuto, ac.getMode());
}

TEST(TestMitsubishiHeavy88AcClass, Turbo) {
  IRMitsubishiHeavy88Ac ac(0);
  ac.begin();

  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());

  ac.setTurbo(false);
  EXPECT_FALSE(ac.getTurbo());

  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());
}

TEST(TestMitsubishiHeavy88AcClass, Econo) {
  IRMitsubishiHeavy88Ac ac(0);
  ac.begin();

  ac.setEcono(true);
  EXPECT_TRUE(ac.getEcono());

  ac.setEcono(false);
  EXPECT_FALSE(ac.getEcono());

  ac.setEcono(true);
  EXPECT_TRUE(ac.getEcono());
}

TEST(TestMitsubishiHeavy88AcClass, 3D) {
  IRMitsubishiHeavy88Ac ac(0);
  ac.begin();

  ac.set3D(true);
  EXPECT_TRUE(ac.get3D());

  ac.set3D(false);
  EXPECT_FALSE(ac.get3D());

  ac.set3D(true);
  EXPECT_TRUE(ac.get3D());
}

TEST(TestMitsubishiHeavy88AcClass, Clean) {
  IRMitsubishiHeavy88Ac ac(0);
  ac.begin();

  ac.setClean(true);
  EXPECT_TRUE(ac.getClean());

  ac.setClean(false);
  EXPECT_FALSE(ac.getClean());

  ac.setClean(true);
  EXPECT_TRUE(ac.getClean());
}

TEST(TestMitsubishiHeavy88AcClass, FanSpeed) {
  IRMitsubishiHeavy88Ac ac(0);
  ac.begin();

  ac.setFan(kMitsubishiHeavy88FanLow);
  EXPECT_EQ(kMitsubishiHeavy88FanLow, ac.getFan());

  ac.setFan(kMitsubishiHeavy88FanAuto);
  EXPECT_EQ(kMitsubishiHeavy88FanAuto, ac.getFan());


  ac.setFan(255);
  EXPECT_EQ(kMitsubishiHeavy88FanAuto, ac.getFan());

  ac.setFan(kMitsubishiHeavy88FanHigh);
  EXPECT_EQ(kMitsubishiHeavy88FanHigh, ac.getFan());

  ac.setFan(kMitsubishiHeavy88FanHigh + 1);
  EXPECT_EQ(kMitsubishiHeavy88FanAuto, ac.getFan());

  ac.setFan(kMitsubishiHeavy88FanHigh - 1);
  EXPECT_EQ(kMitsubishiHeavy88FanHigh - 1, ac.getFan());

  ac.setFan(kMitsubishiHeavy88FanLow + 1);
  EXPECT_EQ(kMitsubishiHeavy88FanLow + 1, ac.getFan());

  ac.setFan(kMitsubishiHeavy88FanEcono);
  EXPECT_EQ(kMitsubishiHeavy88FanEcono, ac.getFan());

  ac.setFan(kMitsubishiHeavy88FanTurbo);
  EXPECT_EQ(kMitsubishiHeavy88FanTurbo, ac.getFan());
}

TEST(TestMitsubishiHeavy88AcClass, VerticalSwing) {
  IRMitsubishiHeavy88Ac ac(0);
  ac.begin();
  ac.setSwingVertical(kMitsubishiHeavy88SwingVAuto);
  EXPECT_EQ(kMitsubishiHeavy88SwingVAuto, ac.getSwingVertical());

  ac.setSwingVertical(kMitsubishiHeavy88SwingVHighest);
  EXPECT_EQ(kMitsubishiHeavy88SwingVHighest, ac.getSwingVertical());

  ac.setSwingVertical(kMitsubishiHeavy88SwingVOff);
  EXPECT_EQ(kMitsubishiHeavy88SwingVOff, ac.getSwingVertical());

  ac.setSwingVertical(kMitsubishiHeavy88SwingVLowest + 1);
  EXPECT_EQ(kMitsubishiHeavy88SwingVOff, ac.getSwingVertical());

  ac.setSwingVertical(kMitsubishiHeavy88SwingVHigh + 1);
  EXPECT_EQ(kMitsubishiHeavy88SwingVOff, ac.getSwingVertical());

  // Out of bounds.
  ac.setSwingVertical(255);
  EXPECT_EQ(kMitsubishiHeavy88SwingVOff, ac.getSwingVertical());
}

TEST(TestMitsubishiHeavy88AcClass, HorizontalSwing) {
  IRMitsubishiHeavy88Ac ac(0);
  ac.begin();
  ac.setSwingHorizontal(kMitsubishiHeavy88SwingHAuto);
  EXPECT_EQ(kMitsubishiHeavy88SwingHAuto, ac.getSwingHorizontal());

  ac.setSwingHorizontal(kMitsubishiHeavy88SwingHLeftMax);
  EXPECT_EQ(kMitsubishiHeavy88SwingHLeftMax, ac.getSwingHorizontal());

  ac.setSwingHorizontal(kMitsubishiHeavy88SwingHRightMax + 1);
  EXPECT_EQ(kMitsubishiHeavy88SwingHOff, ac.getSwingHorizontal());

  ac.setSwingHorizontal(kMitsubishiHeavy88SwingHRightMax);
  EXPECT_EQ(kMitsubishiHeavy88SwingHRightMax, ac.getSwingHorizontal());

  ac.setSwingHorizontal(kMitsubishiHeavy88SwingHRightMax + 1);
  EXPECT_EQ(kMitsubishiHeavy88SwingHOff, ac.getSwingHorizontal());

  ac.setSwingHorizontal(kMitsubishiHeavy88SwingHOff);
  EXPECT_EQ(kMitsubishiHeavy88SwingHOff, ac.getSwingHorizontal());

  ac.setSwingHorizontal(kMitsubishiHeavy88SwingH3D + 1);
  EXPECT_EQ(kMitsubishiHeavy88SwingHOff, ac.getSwingHorizontal());

  // Out of bounds.
  ac.setSwingHorizontal(255);
  EXPECT_EQ(kMitsubishiHeavy88SwingHOff, ac.getSwingHorizontal());
}

TEST(TestMitsubishiHeavy88AcClass, Checksums) {
  IRMitsubishiHeavy88Ac ac(0);
  ac.begin();

  EXPECT_TRUE(ac.validChecksum(ac.getRaw()));

  uint8_t expected[kMitsubishiHeavy88StateLength] = {
      0xAD, 0x51, 0x3C, 0xD9, 0x26, 0x48, 0xB7, 0x00, 0xFF, 0x8A, 0x75};
  EXPECT_TRUE(IRMitsubishiHeavy88Ac::validChecksum(expected));

  // Screw up the "checksum" to test it fails.
  expected[kMitsubishiHeavy88StateLength - 1] = 0x55;
  EXPECT_FALSE(IRMitsubishiHeavy88Ac::validChecksum(expected));
  // getting the after getRaw() should repair it.
  ac.setRaw(expected);
  EXPECT_TRUE(ac.validChecksum(ac.getRaw()));
  EXPECT_TRUE(IRMitsubishiHeavy88Ac::validChecksum(ac.getRaw()));
}

TEST(TestMitsubishiHeavy88AcClass, HumanReadable) {
  IRMitsubishiHeavy88Ac ac(0);

  EXPECT_EQ(
      "Power: Off, Mode: 0 (Auto), Temp: 17C, Fan: 0 (Auto), "
      "Swing(V): 0 (Off), Swing(H): 0 (Off), "
      "Turbo: Off, Econo: Off, 3D: Off, Clean: Off",
      ac.toString());
  ac.on();
  ac.setMode(kMitsubishiHeavyCool);
  ac.setTemp(kMitsubishiHeavyMinTemp);
  ac.setFan(kMitsubishiHeavy88FanHigh);
  ac.setTurbo(false);
  ac.setEcono(false);
  ac.set3D(true);
  ac.setSwingVertical(kMitsubishiHeavy88SwingVAuto);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 17C, Fan: 4 (High), "
      "Swing(V): 4 (Auto), Swing(H): 14 (3D), "
      "Turbo: Off, Econo: Off, 3D: On, Clean: Off",
      ac.toString());

  ac.setMode(kMitsubishiHeavyHeat);
  ac.setTemp(kMitsubishiHeavyMaxTemp);
  ac.setTurbo(true);
  ac.setEcono(false);
  ac.set3D(false);
  ac.setSwingVertical(kMitsubishiHeavy88SwingVLowest);
  ac.setSwingHorizontal(kMitsubishiHeavy88SwingHLeftMax);

  EXPECT_EQ(
      "Power: On, Mode: 4 (Heat), Temp: 31C, Fan: 6 (Turbo), "
      "Swing(V): 7 (Lowest), Swing(H): 1 (Max Left), Turbo: On, Econo: Off, "
      "3D: Off, Clean: Off",
      ac.toString());

  ac.setClean(true);
  ac.setEcono(true);
  ac.setMode(kMitsubishiHeavyAuto);
  ac.setSwingVertical(kMitsubishiHeavy88SwingVOff);

  EXPECT_EQ(
      "Power: On, Mode: 0 (Auto), Temp: 31C, Fan: 7 (Econo), "
      "Swing(V): 0 (Off), Swing(H): 1 (Max Left), Turbo: Off, Econo: On, "
      "3D: Off, Clean: On",
      ac.toString());

  ac.setClean(false);
  ac.setTemp(25);
  ac.setEcono(false);
  ac.setMode(kMitsubishiHeavyDry);
  ac.setSwingHorizontal(kMitsubishiHeavy88SwingHLeftRight);
  EXPECT_EQ(
      "Power: On, Mode: 2 (Dry), Temp: 25C, Fan: 0 (Auto), "
      "Swing(V): 0 (Off), Swing(H): 6 (Left Right), Turbo: Off, Econo: Off, "
      "3D: Off, Clean: Off",
      ac.toString());
}

// Tests for decodeMitsubishiHeavy().

// Decode a real MitsubishiHeavy 152Bit message.
TEST(TestDecodeMitsubishiHeavy, ZmsRealExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  IRMitsubishiHeavy152Ac ac(0);
  irsend.begin();

  uint8_t expected[kMitsubishiHeavy152StateLength] = {
      0xAD, 0x51, 0x3C, 0xE5, 0x1A, 0x0C, 0xF3, 0x07,
      0xF8, 0x04, 0xFB, 0x00, 0xFF, 0x00, 0xFF, 0x00,
      0xFF, 0x80, 0x7F};

  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/660#issuecomment-480571466
  uint16_t rawData[307] = {
      3136, 1638, 364, 428, 366, 1224, 362, 432, 364, 430, 364, 1226, 362, 432,
      364, 1224, 366, 428, 366, 430, 366, 1224, 362, 1228, 362, 1228, 362, 432,
      364, 1224, 364, 432, 364, 1226, 364, 1224, 366, 1226, 364, 428, 364, 430,
      364, 430, 364, 432, 366, 1226, 364, 1224, 364, 430, 364, 1226, 364, 428,
      364, 1224, 368, 1224, 364, 428, 364, 430, 366, 430, 364, 1158, 430, 432,
      366, 1222, 366, 430, 366, 430, 364, 1226, 364, 1224, 364, 1224, 364, 1224,
      366, 1224, 364, 430, 364, 430, 364, 1228, 362, 1226, 364, 1226, 366, 1222,
      366, 430, 364, 430, 364, 1224, 366, 1224, 364, 430, 364, 430, 364, 432,
      364, 430, 364, 428, 364, 430, 364, 430, 366, 1226, 362, 1154, 434, 1228,
      364, 1226, 362, 1226, 364, 1226, 364, 1228, 362, 1226, 362, 432, 364, 430,
      364, 428, 364, 430, 364, 430, 364, 1228, 362, 1228, 362, 432, 364, 1224,
      368, 1224, 364, 1226, 362, 1226, 364, 1226, 366, 428, 366, 430, 364, 1224,
      364, 430, 366, 430, 366, 430, 364, 430, 364, 430, 364, 1226, 364, 1226,
      366, 1224, 366, 1224, 366, 1226, 364, 1224, 366, 1224, 366, 1224, 366,
      428, 364, 430, 366, 428, 364, 430, 364, 430, 366, 428, 364, 430, 364, 432,
      364, 1226, 364, 1226, 364, 1226, 364, 1228, 364, 1222, 370, 1222, 362,
      1228, 362, 1226, 362, 430, 364, 430, 364, 430, 364, 432, 364, 428, 364,
      432, 364, 428, 364, 430, 366, 1226, 362, 1224, 364, 1226, 364, 1226, 364,
      1226, 362, 1226, 366, 1224, 366, 1224, 364, 430, 364, 432, 364, 428, 364,
      432, 364, 428, 364, 430, 366, 430, 364, 430, 364, 1226, 362, 1226, 364,
      1224, 366, 1226, 362, 1228, 364, 1224, 366, 1224, 364, 430, 364, 432, 364,
      428, 364, 430, 364, 430, 364, 430, 366, 430, 364, 430, 338, 1252, 362
    };  // UNKNOWN 5138D49D

  irsend.reset();
  irsend.sendRaw(rawData, 307, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(MITSUBISHI_HEAVY_152, irsend.capture.decode_type);
  ASSERT_EQ(kMitsubishiHeavy152Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: On, Mode: 4 (Heat), Temp: 24C, Fan: 4 (Max), "
      "Swing(V): 0 (Auto), Swing(H): 0 (Auto), Silent: Off, Turbo: Off, "
      "Econo: Off, Night: Off, Filter: Off, 3D: Off, Clean: Off",
      ac.toString());
}

// Decode a Synthetic MitsubishiHeavy 152Bit message.
TEST(TestDecodeMitsubishiHeavy, ZmsSyntheticExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  IRMitsubishiHeavy152Ac ac(0);
  irsend.begin();

  uint8_t expected[kMitsubishiHeavy152StateLength] = {
      0xAD, 0x51, 0x3C, 0xE5, 0x1A, 0x0C, 0xF3, 0x07,
      0xF8, 0x04, 0xFB, 0x00, 0xFF, 0x00, 0xFF, 0x00,
      0xFF, 0x80, 0x7F};

  irsend.reset();
  irsend.sendMitsubishiHeavy152(expected);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(MITSUBISHI_HEAVY_152, irsend.capture.decode_type);
  ASSERT_EQ(kMitsubishiHeavy152Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: On, Mode: 4 (Heat), Temp: 24C, Fan: 4 (Max), "
      "Swing(V): 0 (Auto), Swing(H): 0 (Auto), Silent: Off, Turbo: Off, "
      "Econo: Off, Night: Off, Filter: Off, 3D: Off, Clean: Off",
      ac.toString());
}

// Decode a real MitsubishiHeavy 152Bit message.
TEST(TestDecodeMitsubishiHeavy, ZmsRealExample2) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  IRMitsubishiHeavy152Ac ac(0);
  irsend.begin();

  uint8_t expected[kMitsubishiHeavy152StateLength] = {
      0xAD, 0x51, 0x3C, 0xE5, 0x1A, 0x04, 0xFB, 0x07,
      0xF8, 0x04, 0xFB, 0x00, 0xFF, 0x00, 0xFF, 0x00,
      0xFF, 0x80, 0x7F};

  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/660#issuecomment-480571466
  uint16_t rawData[307] = {
      3196, 1580, 398, 390, 404, 1190, 400, 390, 402, 390, 402, 1192, 402, 388,
      402, 1192, 400, 390, 402, 392, 402, 1192, 400, 1188, 400, 1188, 400, 390,
      404, 1192, 398, 392, 400, 1192, 402, 1188, 400, 1190, 402, 388, 402, 392,
      404, 392, 402, 392, 404, 1188, 400, 1190, 398, 392, 404, 1188, 398, 392,
      402, 1192, 398, 1190, 400, 390, 404, 390, 402, 392, 404, 1188, 398, 392,
      404, 1190, 400, 392, 400, 394, 402, 1192, 398, 1190, 398, 1192, 398, 1190,
      400, 1190, 398, 392, 402, 1192, 398, 1190, 398, 1190, 398, 1192, 396,
      1192, 398, 396, 400, 394, 398, 1194, 396, 394, 400, 394, 398, 396, 398,
      396, 400, 402, 390, 394, 402, 392, 398, 396, 398, 1194, 396, 1194, 398,
      1192, 398, 1192, 396, 1194, 396, 1192, 396, 1196, 398, 1190, 398, 392,
      402, 392, 402, 394, 398, 394, 400, 394, 400, 1192, 398, 1192, 400, 390,
      402, 1190, 398, 1190, 398, 1192, 402, 1188, 398, 1190, 400, 390, 402, 392,
      402, 1190, 400, 390, 404, 390, 402, 394, 402, 392, 402, 390, 404, 1190,
      400, 1188, 400, 1190, 400, 1190, 402, 1188, 402, 1188, 400, 1188, 402,
      1190, 400, 388, 402, 394, 404, 392, 404, 388, 404, 390, 404, 392, 402,
      394, 402, 390, 402, 1190, 402, 1186, 402, 1190, 400, 1190, 398, 1190, 402,
      1186, 402, 1190, 400, 1188, 400, 390, 404, 392, 404, 390, 402, 392, 402,
      392, 400, 394, 402, 392, 402, 394, 400, 1192, 400, 1190, 400, 1188, 400,
      1192, 400, 1186, 402, 1190, 400, 1190, 400, 1188, 402, 388, 402, 390, 404,
      392, 402, 392, 402, 392, 402, 392, 404, 392, 402, 392, 404, 1190, 400,
      1190, 398, 1190, 400, 1190, 400, 1190, 400, 1188, 400, 1188, 400, 392,
      402, 392, 404, 390, 402, 392, 402, 392, 402, 392, 402, 390, 402, 392, 402,
      1192, 398};  // UNKNOWN A650F2C1

  irsend.reset();
  irsend.sendRaw(rawData, 307, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(MITSUBISHI_HEAVY_152, irsend.capture.decode_type);
  ASSERT_EQ(kMitsubishiHeavy152Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: Off, Mode: 4 (Heat), Temp: 24C, Fan: 4 (Max), "
      "Swing(V): 0 (Auto), Swing(H): 0 (Auto), Silent: Off, Turbo: Off, "
      "Econo: Off, Night: Off, Filter: Off, 3D: Off, Clean: Off",
      ac.toString());
}

// Decode a Synthetic MitsubishiHeavy 88 Bit message.
TEST(TestDecodeMitsubishiHeavy, ZjsSyntheticExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  IRMitsubishiHeavy88Ac ac(0);
  irsend.begin();

  uint8_t expected[kMitsubishiHeavy88StateLength] = {
      0xAD, 0x51, 0x3C, 0xD9, 0x26, 0x48, 0xB7, 0x00, 0xFF, 0x8A, 0x75};

  irsend.reset();
  irsend.sendMitsubishiHeavy88(expected);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(MITSUBISHI_HEAVY_88, irsend.capture.decode_type);
  ASSERT_EQ(kMitsubishiHeavy88Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: On, Mode: 2 (Dry), Temp: 25C, Fan: 0 (Auto), "
      "Swing(V): 0 (Off), Swing(H): 6 (Left Right), Turbo: Off, Econo: Off, "
      "3D: Off, Clean: Off",
      ac.toString());
}

TEST(TestMitsubishiHeavy152AcClass, toCommon) {
  IRMitsubishiHeavy152Ac ac(0);
  ac.setPower(true);
  ac.setMode(kMitsubishiHeavyCool);
  ac.setTemp(20);
  ac.setFan(kMitsubishiHeavy152FanLow);
  ac.setSwingVertical(kMitsubishiHeavy152SwingVHighest);
  ac.setSwingHorizontal(kMitsubishiHeavy152SwingHRightMax);
  ac.setTurbo(false);
  ac.setEcono(true);
  ac.setClean(true);
  ac.setFilter(true);
  ac.setSilent(true);
  ac.setNight(true);
  // Now test it.
  ASSERT_EQ(decode_type_t::MITSUBISHI_HEAVY_152, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMin, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kHighest, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kRightMax, ac.toCommon().swingh);
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_TRUE(ac.toCommon().econo);
  ASSERT_TRUE(ac.toCommon().clean);
  ASSERT_TRUE(ac.toCommon().quiet);
  ASSERT_TRUE(ac.toCommon().filter);
  ASSERT_EQ(0, ac.toCommon().sleep);
  // Unsupported.
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

TEST(TestMitsubishiHeavy88AcClass, toCommon) {
  IRMitsubishiHeavy88Ac ac(0);
  ac.setPower(true);
  ac.setMode(kMitsubishiHeavyCool);
  ac.setTemp(20);
  ac.setFan(kMitsubishiHeavy88FanLow);
  ac.setSwingVertical(kMitsubishiHeavy88SwingVHighest);
  ac.setSwingHorizontal(kMitsubishiHeavy88SwingHRightMax);
  ac.setTurbo(false);
  ac.setEcono(true);
  ac.setClean(true);
  // Now test it.
  ASSERT_EQ(decode_type_t::MITSUBISHI_HEAVY_88, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMin, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kHighest, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kRightMax, ac.toCommon().swingh);
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_TRUE(ac.toCommon().econo);
  ASSERT_TRUE(ac.toCommon().clean);
  // Unsupported.
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}
