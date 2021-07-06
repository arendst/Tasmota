// Copyright 2020 Quentin BRIOLLANT

#include "IRac.h"
#include "ir_Technibel.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "IRutils.h"
#include "gtest/gtest.h"

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("TECHNIBEL_AC", typeToString(decode_type_t::TECHNIBEL_AC));
  ASSERT_EQ(decode_type_t::TECHNIBEL_AC, strToDecodeType("TECHNIBEL_AC"));
  ASSERT_FALSE(hasACState(decode_type_t::TECHNIBEL_AC));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::TECHNIBEL_AC));
  ASSERT_EQ(kTechnibelAcBits, IRsend::defaultBits(decode_type_t::TECHNIBEL_AC));
  ASSERT_EQ(kTechnibelAcDefaultRepeat,
            IRsend::minRepeats(decode_type_t::TECHNIBEL_AC));
}

TEST(TestDecodeTechnibelAc, SyntheticSelfDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);

  irsend.begin();
  irsend.reset();
  irsend.sendTechnibelAc(0x1881221200004B);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(TECHNIBEL_AC, irsend.capture.decode_type);
  EXPECT_EQ(kTechnibelAcBits, irsend.capture.bits);
  EXPECT_EQ(0x1881221200004B, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.command);
  EXPECT_EQ(0, irsend.capture.address);
}

TEST(TestDecodeTechnibelAc, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  uint16_t rawData[115] = {
      8812, 4376,
      534, 560, 530, 562, 530, 562, 532, 1668, 532, 1634, 560, 560, 530, 562,
      528, 566, 530, 1668, 532, 560, 504, 588, 526, 568, 530, 562, 530, 562,
      528, 564, 530, 1670, 530, 562, 530, 564, 528, 1670, 530, 564, 532, 560,
      530, 562, 532, 1636, 560, 564, 530, 562, 528, 564, 528, 564, 526, 1672,
      528, 538, 554, 564, 528, 1640, 558, 564, 530, 562, 526, 564, 504, 586,
      504, 592, 526, 564, 504, 588, 526, 564, 504, 592, 528, 562, 526, 566,
      526, 564, 504, 592, 526, 566, 530, 562, 530, 562, 528, 568, 528, 564,
      504, 1662, 558, 564, 528, 566, 528, 1668, 528, 562, 528, 1666, 532, 1664,
      582};

  irsend.reset();
  irsend.sendRaw(rawData, 115, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(TECHNIBEL_AC, irsend.capture.decode_type);
  EXPECT_EQ(kTechnibelAcBits, irsend.capture.bits);
  EXPECT_EQ(0x1881221200004B, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.command);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Fan: 2 (Medium), Temp: 18C, "
      "Sleep: Off, Swing(V): On, Timer: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

// Tests for IRTechnibelAc class.

TEST(TestIRTechnibelAcClass, Power) {
  IRTechnibelAc ac(kGpioUnused);
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

TEST(TestIRTechnibelAcClass, Temperature) {
  IRTechnibelAc ac(kGpioUnused);
  ac.begin();

  // Celsius
  ac.setTemp(0);
  EXPECT_EQ(kTechnibelAcTempMinC, ac.getTemp());
  EXPECT_FALSE(ac.getTempUnit());

  ac.setTemp(255);
  EXPECT_EQ(kTechnibelAcTempMaxC, ac.getTemp());
  EXPECT_FALSE(ac.getTempUnit());

  ac.setTemp(kTechnibelAcTempMinC);
  EXPECT_EQ(kTechnibelAcTempMinC, ac.getTemp());
  EXPECT_FALSE(ac.getTempUnit());

  ac.setTemp(kTechnibelAcTempMaxC);
  EXPECT_EQ(kTechnibelAcTempMaxC, ac.getTemp());
  EXPECT_FALSE(ac.getTempUnit());

  ac.setTemp(kTechnibelAcTempMinC - 1);
  EXPECT_EQ(kTechnibelAcTempMinC, ac.getTemp());
  EXPECT_FALSE(ac.getTempUnit());

  ac.setTemp(kTechnibelAcTempMaxC + 1);
  EXPECT_EQ(kTechnibelAcTempMaxC, ac.getTemp());
  EXPECT_FALSE(ac.getTempUnit());

  ac.setTemp(19);
  EXPECT_EQ(19, ac.getTemp());
  EXPECT_FALSE(ac.getTempUnit());

  ac.setTemp(21);
  EXPECT_EQ(21, ac.getTemp());
  EXPECT_FALSE(ac.getTempUnit());

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());
  EXPECT_FALSE(ac.getTempUnit());

  ac.setTemp(29, false);
  EXPECT_EQ(29, ac.getTemp());
  EXPECT_FALSE(ac.getTempUnit());

  // Fahrenheit
  ac.setTemp(0, true);
  EXPECT_EQ(kTechnibelAcTempMinF, ac.getTemp());
  EXPECT_TRUE(ac.getTempUnit());

  ac.setTemp(255, true);
  EXPECT_EQ(kTechnibelAcTempMaxF, ac.getTemp());
  EXPECT_TRUE(ac.getTempUnit());

  ac.setTemp(kTechnibelAcTempMinF, true);
  EXPECT_EQ(kTechnibelAcTempMinF, ac.getTemp());
  EXPECT_TRUE(ac.getTempUnit());

  ac.setTemp(kTechnibelAcTempMaxF, true);
  EXPECT_EQ(kTechnibelAcTempMaxF, ac.getTemp());
  EXPECT_TRUE(ac.getTempUnit());

  ac.setTemp(kTechnibelAcTempMinF - 1, true);
  EXPECT_EQ(kTechnibelAcTempMinF, ac.getTemp());
  EXPECT_TRUE(ac.getTempUnit());

  ac.setTemp(kTechnibelAcTempMaxF + 1, true);
  EXPECT_EQ(kTechnibelAcTempMaxF, ac.getTemp());
  EXPECT_TRUE(ac.getTempUnit());

  ac.setTemp(66, true);
  EXPECT_EQ(66, ac.getTemp());
  EXPECT_TRUE(ac.getTempUnit());

  ac.setTemp(75, true);
  EXPECT_EQ(75, ac.getTemp());
  EXPECT_TRUE(ac.getTempUnit());

  ac.setTemp(80, true);
  EXPECT_EQ(80, ac.getTemp());
  EXPECT_TRUE(ac.getTempUnit());

  ac.setTemp(88, true);
  EXPECT_EQ(88, ac.getTemp());
  EXPECT_TRUE(ac.getTempUnit());
}

TEST(TestIRTechnibelAcClass, OperatingMode) {
  IRTechnibelAc ac(kGpioUnused);
  ac.begin();

  ac.setMode(kTechnibelAcCool);
  EXPECT_EQ(kTechnibelAcCool, ac.getMode());

  ac.setMode(kTechnibelAcDry);
  EXPECT_EQ(kTechnibelAcDry, ac.getMode());
  EXPECT_EQ(kTechnibelAcFanLow, ac.getFan());  // Look for fan speed enforcement

  ac.setMode(kTechnibelAcFan);
  EXPECT_EQ(kTechnibelAcFan, ac.getMode());

  ac.setMode(kTechnibelAcHeat);
  EXPECT_EQ(kTechnibelAcHeat, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kTechnibelAcCool, ac.getMode());
}

TEST(TestIRTechnibelAcClass, FanSpeed) {
  IRTechnibelAc ac(kGpioUnused);
  ac.begin();
  ac.setMode(kTechnibelAcCool);  // All fan speeds available in this mode.

  ac.setFan(kTechnibelAcFanLow);
  EXPECT_EQ(kTechnibelAcFanLow, ac.getFan());

  ac.setFan(kTechnibelAcFanMedium);
  EXPECT_EQ(kTechnibelAcFanMedium, ac.getFan());

  ac.setFan(kTechnibelAcFanHigh);
  EXPECT_EQ(kTechnibelAcFanHigh, ac.getFan());

  ac.setFan(0);
  EXPECT_EQ(kTechnibelAcFanLow, ac.getFan());

  ac.setFan(kTechnibelAcFanMedium);
  ac.setFan(255);
  EXPECT_EQ(kTechnibelAcFanLow, ac.getFan());

  // Check fan speed enforcement for Dry mode.
  ac.setFan(kTechnibelAcFanMedium);
  ac.setMode(kTechnibelAcDry);
  EXPECT_EQ(kTechnibelAcFanLow, ac.getFan());
  EXPECT_EQ(kTechnibelAcDry, ac.getMode());
  ac.setFan(kTechnibelAcFanHigh);
  EXPECT_EQ(kTechnibelAcFanLow, ac.getFan());
}

TEST(TestIRTechnibelAcClass, Swing) {
  IRTechnibelAc ac(kGpioUnused);
  ac.begin();

  ac.setSwing(false);
  EXPECT_FALSE(ac.getSwing());
  ac.setSwing(true);
  EXPECT_TRUE(ac.getSwing());
  ac.setSwing(false);
  EXPECT_FALSE(ac.getSwing());
}

TEST(TestIRTechnibelAcClass, Sleep) {
  IRTechnibelAc ac(kGpioUnused);
  ac.begin();

  ac.setSleep(false);
  EXPECT_FALSE(ac.getSleep());
  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
  ac.setSleep(false);
  EXPECT_FALSE(ac.getSleep());
}

TEST(TestIRTechnibelAcClass, Timer) {
  IRTechnibelAc ac(kGpioUnused);
  ac.begin();

  ac.setTimerEnabled(false);
  EXPECT_FALSE(ac.getTimerEnabled());
  ac.setTimerEnabled(true);
  EXPECT_TRUE(ac.getTimerEnabled());
  ac.setTimerEnabled(false);
  EXPECT_FALSE(ac.getTimerEnabled());

  ac.setTimer(0);
  EXPECT_FALSE(ac.getTimerEnabled());
  EXPECT_EQ(0, ac.getTimer());

  ac.setTimer(60);
  EXPECT_TRUE(ac.getTimerEnabled());
  EXPECT_EQ(60, ac.getTimer());

  ac.setTimer(1200);
  EXPECT_TRUE(ac.getTimerEnabled());
  EXPECT_EQ(1200, ac.getTimer());

  ac.setTimerEnabled(false);
  ac.setTimer(1440);
  EXPECT_TRUE(ac.getTimerEnabled());
  EXPECT_EQ(1440, ac.getTimer());

  ac.setTimerEnabled(false);
  ac.setTimer(1500);
  EXPECT_TRUE(ac.getTimerEnabled());
  EXPECT_EQ(1440, ac.getTimer());
}

TEST(TestIRTechnibelAcClass, ConstructKnownState) {
  IRTechnibelAc ac(kGpioUnused);
  EXPECT_EQ(kTechnibelAcResetState, ac.getRaw());
  ac.on();
  ac.setMode(kTechnibelAcCool);
  ac.setFan(kTechnibelAcFanMedium);
  ac.setTemp(18);  // 18C
  ac.setSleep(false);  // Off
  ac.setSwing(true);  // On
  EXPECT_EQ(0x1881221200004B, ac.getRaw());
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Fan: 2 (Medium), Temp: 18C, "
      "Sleep: Off, Swing(V): On, Timer: Off",
      ac.toString());
}

TEST(TestIRTechnibelAcClass, HumanReadable) {
  IRTechnibelAc ac(kGpioUnused);
  ac.begin();
  EXPECT_EQ(
      "Power: Off, Mode: 1 (Cool), Fan: 1 (Low), Temp: 20C, Sleep: Off, "
      "Swing(V): Off, Timer: Off", ac.toString());
  ac.setPower(true);
  ac.setTemp(72, true);  // 72F
  ac.setFan(kTechnibelAcFanMedium);
  ac.setSleep(true);
  ac.setSwing(true);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Fan: 2 (Medium), Temp: 72F, Sleep: On, "
      "Swing(V): On, Timer: Off", ac.toString());
  ac.setMode(kTechnibelAcHeat);
  ac.setTimer(23 * 60);
  EXPECT_EQ(
      "Power: On, Mode: 8 (Heat), Fan: 2 (Medium), Temp: 72F, Sleep: On, "
      "Swing(V): On, Timer: 23:00", ac.toString());
}
