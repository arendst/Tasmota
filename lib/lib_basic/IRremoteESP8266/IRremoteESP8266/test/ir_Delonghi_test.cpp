// Copyright 2020 David Conran

#include "IRac.h"
#include "ir_Delonghi.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "IRutils.h"
#include "gtest/gtest.h"

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("DELONGHI_AC", typeToString(decode_type_t::DELONGHI_AC));
  ASSERT_EQ(decode_type_t::DELONGHI_AC, strToDecodeType("DELONGHI_AC"));
  ASSERT_FALSE(hasACState(decode_type_t::DELONGHI_AC));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::DELONGHI_AC));
  ASSERT_EQ(kDelonghiAcBits, IRsend::defaultBits(decode_type_t::DELONGHI_AC));
  ASSERT_EQ(kDelonghiAcDefaultRepeat,
            IRsend::minRepeats(decode_type_t::DELONGHI_AC));
}

TEST(TestDecodeDelonghiAc, SyntheticSelfDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);

  irsend.begin();
  irsend.reset();
  irsend.sendDelonghiAc(0x6900000D0D01FB53);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(DELONGHI_AC, irsend.capture.decode_type);
  EXPECT_EQ(kDelonghiAcBits, irsend.capture.bits);
  EXPECT_EQ(0x6900000D0D01FB53, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.command);
  EXPECT_EQ(0, irsend.capture.address);
}

TEST(TestDecodeDelonghiAc, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Data from:
  // https://github.com/crankyoldgit/IRremoteESP8266/issues/1096#issue-610665633
  uint16_t rawData[131] = {
      8984, 4200,
      608, 1516, 608, 1516, 612, 472, 556, 528, 560, 1564, 556, 528, 560, 1564,
      564, 528, 552, 1572, 556, 1568, 556, 528, 552, 1572, 556, 1568, 560, 1564,
      552, 1572, 556, 1576, 552, 1568, 560, 528, 560, 524, 556, 528, 552, 532,
      560, 528, 552, 532, 556, 532, 560, 1564, 560, 528, 552, 1568, 560, 1564,
      564, 524, 556, 528, 560, 524, 556, 536, 556, 1568, 560, 524, 556, 1568,
      560, 1564, 584, 500, 588, 496, 584, 500, 592, 500, 588, 496, 584, 500,
      592, 496, 584, 500, 588, 496, 584, 500, 592, 492, 584, 508, 584, 500,
      588, 496, 584, 500, 592, 496, 584, 500, 580, 504, 584, 500, 580, 508,
      584, 1544, 584, 500, 588, 496, 584, 1540, 588, 500, 580, 1540, 588, 1536,
      588, 500,
      592};

  irsend.reset();
  irsend.sendRaw(rawData, 263, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(DELONGHI_AC, irsend.capture.decode_type);
  EXPECT_EQ(kDelonghiAcBits, irsend.capture.bits);
  EXPECT_EQ(0x6900000D0D01FB53, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.command);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(
      "Power: On, Mode: 0 (Cool), Fan: 3 (Low), Temp: 90F, "
      "Turbo: Off, Sleep: Off, On Timer: 06:13, Off Timer: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

// Tests for IRDelonghiAc class.

TEST(TestIRDelonghiAcClass, Power) {
  IRDelonghiAc ac(kGpioUnused);
  ac.begin();

  ac.on();
  EXPECT_TRUE(ac.getPower());

  ac.off();
  EXPECT_FALSE(ac.getPower());

  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());

  ac.setPower(false);
  EXPECT_FALSE(ac.getPower());

  // Ref:
  //   https://github.com/crankyoldgit/IRremoteESP8266/issues/1096#issuecomment-622521726
  ac.setRaw(0x5500000000010153);  // Power on
  EXPECT_TRUE(ac.getPower());
  ac.setRaw(0x5400000000000153);  // Power off
  EXPECT_FALSE(ac.getPower());
}

TEST(TestIRDelonghiAcClass, Temperature) {
  IRDelonghiAc ac(kGpioUnused);
  ac.begin();

  // Celsius
  ac.setTemp(0);
  EXPECT_EQ(kDelonghiAcTempMinC, ac.getTemp());
  EXPECT_FALSE(ac.getTempUnit());

  ac.setTemp(255);
  EXPECT_EQ(kDelonghiAcTempMaxC, ac.getTemp());
  EXPECT_FALSE(ac.getTempUnit());

  ac.setTemp(kDelonghiAcTempMinC);
  EXPECT_EQ(kDelonghiAcTempMinC, ac.getTemp());
  EXPECT_FALSE(ac.getTempUnit());

  ac.setTemp(kDelonghiAcTempMaxC);
  EXPECT_EQ(kDelonghiAcTempMaxC, ac.getTemp());
  EXPECT_FALSE(ac.getTempUnit());

  ac.setTemp(kDelonghiAcTempMinC - 1);
  EXPECT_EQ(kDelonghiAcTempMinC, ac.getTemp());
  EXPECT_FALSE(ac.getTempUnit());

  ac.setTemp(kDelonghiAcTempMaxC + 1);
  EXPECT_EQ(kDelonghiAcTempMaxC, ac.getTemp());
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
  EXPECT_EQ(kDelonghiAcTempMinF, ac.getTemp());
  EXPECT_TRUE(ac.getTempUnit());

  ac.setTemp(255, true);
  EXPECT_EQ(kDelonghiAcTempMaxF, ac.getTemp());
  EXPECT_TRUE(ac.getTempUnit());

  ac.setTemp(kDelonghiAcTempMinF, true);
  EXPECT_EQ(kDelonghiAcTempMinF, ac.getTemp());
  EXPECT_TRUE(ac.getTempUnit());

  ac.setTemp(kDelonghiAcTempMaxF, true);
  EXPECT_EQ(kDelonghiAcTempMaxF, ac.getTemp());
  EXPECT_TRUE(ac.getTempUnit());

  ac.setTemp(kDelonghiAcTempMinF - 1, true);
  EXPECT_EQ(kDelonghiAcTempMinF, ac.getTemp());
  EXPECT_TRUE(ac.getTempUnit());

  ac.setTemp(kDelonghiAcTempMaxF + 1, true);
  EXPECT_EQ(kDelonghiAcTempMaxF, ac.getTemp());
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

TEST(TestIRDelonghiAcClass, OperatingMode) {
  IRDelonghiAc ac(kGpioUnused);
  ac.begin();

  ac.setMode(kDelonghiAcAuto);
  EXPECT_EQ(kDelonghiAcAuto, ac.getMode());
  EXPECT_EQ(17, ac.getTemp());  // Check for special temp
  EXPECT_EQ(kDelonghiAcFanAuto, ac.getFan());  // Look for fan speed enforcement

  ac.setMode(kDelonghiAcCool);
  EXPECT_EQ(kDelonghiAcCool, ac.getMode());
  // Check changing to another mode that has a fixed temp and back keeps the
  // existing temp. Only for Cool mode.
  ac.setTemp(22);
  EXPECT_EQ(22, ac.getTemp());
  ac.setMode(kDelonghiAcAuto);
  EXPECT_NE(22, ac.getTemp());
  ac.setMode(kDelonghiAcCool);
  EXPECT_EQ(22, ac.getTemp());

  ac.setMode(kDelonghiAcDry);
  EXPECT_EQ(kDelonghiAcDry, ac.getMode());
  EXPECT_EQ(17, ac.getTemp());  // Check for special temp
  EXPECT_EQ(kDelonghiAcFanAuto, ac.getFan());  // Look for fan speed enforcement

  ac.setMode(kDelonghiAcFan);
  EXPECT_EQ(kDelonghiAcFan, ac.getMode());
  EXPECT_EQ(23, ac.getTemp());  // Check for special temp
  EXPECT_NE(kDelonghiAcFanAuto, ac.getFan());  // Look for fan speed enforcement

  ac.setMode(kDelonghiAcAuto + 1);
  EXPECT_EQ(kDelonghiAcAuto, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kDelonghiAcAuto, ac.getMode());
}

TEST(TestIRDelonghiAcClass, FanSpeed) {
  IRDelonghiAc ac(kGpioUnused);
  ac.begin();
  ac.setMode(kDelonghiAcCool);  // All fan speeds available in this mode.

  ac.setFan(0);
  EXPECT_EQ(kDelonghiAcFanAuto, ac.getFan());

  ac.setFan(255);
  EXPECT_EQ(kDelonghiAcFanAuto, ac.getFan());

  ac.setFan(kDelonghiAcFanHigh);
  EXPECT_EQ(kDelonghiAcFanHigh, ac.getFan());

  ac.setFan(kDelonghiAcFanLow + 1);
  EXPECT_EQ(kDelonghiAcFanAuto, ac.getFan());

  ac.setFan(1);
  EXPECT_EQ(1, ac.getFan());

  ac.setFan(2);
  EXPECT_EQ(2, ac.getFan());

  ac.setFan(3);
  EXPECT_EQ(3, ac.getFan());

  // Confirm changing to fan mode handles speed behaviour correctly.
  ac.setFan(kDelonghiAcFanLow);
  ac.setMode(kDelonghiAcFan);
  EXPECT_EQ(kDelonghiAcFanLow, ac.getFan());
  ac.setMode(kDelonghiAcAuto);
  EXPECT_EQ(kDelonghiAcFanAuto, ac.getFan());
  ac.setMode(kDelonghiAcFan);
  EXPECT_NE(kDelonghiAcFanAuto, ac.getFan());
}

TEST(TestIRDelonghiAcClass, Boost) {
  IRDelonghiAc ac(kGpioUnused);
  ac.begin();

  ac.setBoost(false);
  EXPECT_FALSE(ac.getBoost());
  ac.setBoost(true);
  EXPECT_TRUE(ac.getBoost());
  ac.setBoost(false);
  EXPECT_FALSE(ac.getBoost());
}

TEST(TestIRDelonghiAcClass, Sleep) {
  IRDelonghiAc ac(kGpioUnused);
  ac.begin();

  ac.setSleep(false);
  EXPECT_FALSE(ac.getSleep());
  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
  ac.setSleep(false);
  EXPECT_FALSE(ac.getSleep());
}

TEST(TestIRDelonghiAcClass, OnTimer) {
  IRDelonghiAc ac(kGpioUnused);
  ac.begin();

  ac.setOnTimerEnabled(false);
  EXPECT_FALSE(ac.getOnTimerEnabled());
  ac.setOnTimerEnabled(true);
  EXPECT_TRUE(ac.getOnTimerEnabled());
  ac.setOnTimerEnabled(false);
  EXPECT_FALSE(ac.getOnTimerEnabled());

  ac.setOnTimer(0);
  EXPECT_FALSE(ac.getOnTimerEnabled());
  EXPECT_EQ(0, ac.getOnTimer());

  ac.setOnTimer(1);
  EXPECT_TRUE(ac.getOnTimerEnabled());
  EXPECT_EQ(1, ac.getOnTimer());

  ac.setOnTimer(61);
  EXPECT_TRUE(ac.getOnTimerEnabled());
  EXPECT_EQ(61, ac.getOnTimer());

  ac.setOnTimerEnabled(false);
  ac.setOnTimer(23 * 60 + 59);
  EXPECT_TRUE(ac.getOnTimerEnabled());
  EXPECT_EQ(23 * 60 + 59, ac.getOnTimer());

  ac.setOnTimerEnabled(false);
  ac.setOnTimer(24 * 60);
  EXPECT_TRUE(ac.getOnTimerEnabled());
  EXPECT_EQ(23 * 60 + 59, ac.getOnTimer());
}

TEST(TestIRDelonghiAcClass, OffTimer) {
  IRDelonghiAc ac(kGpioUnused);
  ac.begin();

  ac.setOffTimerEnabled(false);
  EXPECT_FALSE(ac.getOffTimerEnabled());
  ac.setOffTimerEnabled(true);
  EXPECT_TRUE(ac.getOffTimerEnabled());
  ac.setOffTimerEnabled(false);
  EXPECT_FALSE(ac.getOffTimerEnabled());

  ac.setOffTimer(0);
  EXPECT_FALSE(ac.getOffTimerEnabled());
  EXPECT_EQ(0, ac.getOffTimer());

  ac.setOffTimer(1);
  EXPECT_TRUE(ac.getOffTimerEnabled());
  EXPECT_EQ(1, ac.getOffTimer());

  ac.setOffTimer(61);
  EXPECT_TRUE(ac.getOffTimerEnabled());
  EXPECT_EQ(61, ac.getOffTimer());

  ac.setOffTimerEnabled(false);
  ac.setOffTimer(23 * 60 + 59);
  EXPECT_TRUE(ac.getOffTimerEnabled());
  EXPECT_EQ(23 * 60 + 59, ac.getOffTimer());

  ac.setOffTimerEnabled(false);
  ac.setOffTimer(24 * 60);
  EXPECT_TRUE(ac.getOffTimerEnabled());
  EXPECT_EQ(23 * 60 + 59, ac.getOffTimer());

  // Real Data
  // From: https://github.com/crankyoldgit/IRremoteESP8266/issues/1096#issuecomment-623115619
  // Setting off timer to 8:51 when the time on the remote displayed 16:05.
  // (8:51 + 24:00 - 16:05 == 32:51 - 16:05 == 16:46) i.e. Turn off in 16h46m.
  ac.setRaw(0xB12E210000000F53);
  EXPECT_TRUE(ac.getOffTimerEnabled());
  EXPECT_EQ(16 * 60 + 46, ac.getOffTimer());
  EXPECT_FALSE(ac.getOnTimerEnabled());
  EXPECT_EQ(0, ac.getOnTimer());
}
