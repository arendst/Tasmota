// Copyright 2021 Davide Depau

#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendKelon().

// Test sending typical data only.
TEST(TestSendKelon, SendDataOnly) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  // Temp: 26C, Mode: 2 (Cool), Fan: 0 (Auto), Sleep: Off, Dry: 0, Timer: Off,
  // Turbo: Off
  irsend.reset();
  irsend.sendKelon(0x82000683);
  EXPECT_EQ(
      "f38000d50"
      "m9000s4600m560s1680m560s1680m560s600m560s600m560s600m560s600"
      "m560s600m560s1680m560s600m560s1680m560s1680m560s600m560s600m560s600"
      "m560s600m560s600m560s600m560s600m560s600m560s600m560s600m560s600"
      "m560s600m560s600m560s600m560s1680m560s600m560s600m560s600m560s600"
      "m560s600m560s1680m560s600m560s600m560s600m560s600m560s600m560s600"
      "m560s600m560s600m560s600m560s600m560s600m560s600m560s600m560s600"
      "m560s600m560s600m560s200000",
      irsend.outputStr());

  // Temp: 18C, Mode: 2 (Cool), Fan: 1 (Low), Sleep: Off, Dry: 0, Timer: Off,
  // Turbo: On
  irsend.reset();
  irsend.sendKelon(0x900002010683);
  EXPECT_EQ(
      "f38000d50"
      "m9000s4600m560s1680m560s1680m560s600m560s600m560s600m560s600m560s600"
      "m560s1680m560s600m560s1680m560s1680m560s600m560s600m560s600"
      "m560s600m560s600m560s1680m560s600m560s600m560s600m560s600"
      "m560s600m560s600m560s600m560s600m560s1680m560s600m560s600m560s600"
      "m560s600m560s600m560s600m560s600m560s600m560s600m560s600"
      "m560s600m560s600m560s600m560s600m560s600m560s600m560s600"
      "m560s600m560s1680m560s600m560s600m560s1680m560s200000",
      irsend.outputStr());

  // Temp: 23C, Mode: 0 (Heat), Fan: 0 (Auto), Sleep: Off, Dry: 0, Timer: Off,
  // Turbo: Off, Power Toggle: On
  irsend.reset();
  irsend.sendKelon(0x50040683);
  EXPECT_EQ(
      "f38000d50"
      "m9000s4600m560s1680m560s1680m560s600m560s600m560s600m560s600"
      "m560s600m560s1680m560s600m560s1680m560s1680m560s600m560s600"
      "m560s600m560s600m560s600m560s600m560s600m560s1680m560s600m560s600"
      "m560s600m560s600m560s600m560s600m560s600m560s600m560s600m560s1680"
      "m560s600m560s1680m560s600m560s600m560s600m560s600m560s600m560s600"
      "m560s600m560s600m560s600m560s600m560s600m560s600m560s600m560s600"
      "m560s600m560s600m560s600m560s200000",
      irsend.outputStr());

  // Temp: 26C, Mode: 2 (Cool), Fan: 0 (Auto), Sleep: Off, Dry: 0, Timer:
  // On (9.5h), Turbo:
  irsend.reset();
  irsend.sendKelon(0x138A000683);
  EXPECT_EQ(
      "f38000d50"
      "m9000s4600m560s1680m560s1680m560s600m560s600m560s600"
      "m560s600m560s600m560s1680m560s600m560s1680m560s1680m560s600"
      "m560s600m560s600m560s600m560s600m560s600m560s600m560s600"
      "m560s600m560s600m560s600m560s600m560s600m560s600m560s1680"
      "m560s600m560s1680m560s600m560s600m560s600m560s1680m560s1680"
      "m560s1680m560s600m560s600m560s1680m560s600m560s600m560s600"
      "m560s600m560s600m560s600m560s600m560s600m560s600m560s600"
      "m560s600m560s200000",
      irsend.outputStr());

  // Temp: 26C, Mode: 2 (Cool), Fan: 0 (Auto), Sleep: Off, Dry: 0, Timer:
  // On (15h), Turbo: Off:
  irsend.reset();
  irsend.sendKelon(0x198A000683);
  EXPECT_EQ(
      "f38000d50"
      "m9000s4600m560s1680m560s1680m560s600m560s600m560s600"
      "m560s600m560s600m560s1680m560s600m560s1680m560s1680"
      "m560s600m560s600m560s600m560s600m560s600m560s600m560s600"
      "m560s600m560s600m560s600m560s600m560s600m560s600m560s600"
      "m560s1680m560s600m560s1680m560s600m560s600m560s600m560s1680"
      "m560s1680m560s600m560s600m560s1680m560s1680m560s600"
      "m560s600m560s600m560s600m560s600m560s600m560s600m560s600"
      "m560s600m560s600m560s600m560s200000",
      irsend.outputStr());
}

// Tests for decodeKelon().
// Decode normal Kelon messages.
TEST(TestDecodeKelon, Timer12HSmartMode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendKelon(0x1679030683);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(KELON, irsend.capture.decode_type);
  EXPECT_EQ(kKelonBits, irsend.capture.bits);
  EXPECT_EQ(0x1679030683, irsend.capture.value);
  EXPECT_EQ(
      "Temp: 25C, Mode: 1 (Auto), Fan: 3 (High), Sleep: Off, Dry: 0, "
      "Timer: 12:00, Turbo: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t result, prev;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &result, &prev));
}

TEST(TestDecodeKelon, Timer5_5hSuperCoolMode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendKelon(0x100B0A010683);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(KELON, irsend.capture.decode_type);
  EXPECT_EQ(kKelonBits, irsend.capture.bits);
  EXPECT_EQ(0x100B0A010683, irsend.capture.value);
  EXPECT_EQ(
      "Temp: 18C, Mode: 2 (Cool), Fan: 1 (Low), Sleep: Off, Dry: 0, "
      "Timer: 05:30, Turbo: On",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t result, prev;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &result, &prev));
}

TEST(TestDecodeKelon, ChangeSettingsWithTimerSetHeatMode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendKelon(0x58000683);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(KELON, irsend.capture.decode_type);
  EXPECT_EQ(kKelonBits, irsend.capture.bits);
  EXPECT_EQ(0x58000683, irsend.capture.value);
  EXPECT_EQ(
      "Temp: 23C, Mode: 0 (Heat), Fan: 0 (Auto), Sleep: Off, Dry: 0, "
      "Timer: On, Turbo: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t result, prev;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &result, &prev));
}

TEST(TestDecodeKelon, TestPowerToggleDryMode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendKelon(0x83040683);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(KELON, irsend.capture.decode_type);
  EXPECT_EQ(kKelonBits, irsend.capture.bits);
  EXPECT_EQ(0x83040683, irsend.capture.value);
  EXPECT_EQ(
      "Temp: 26C, Mode: 3 (Dry), Fan: 0 (Auto), Sleep: Off, Dry: 0, Timer:"
      " Off, Turbo: Off, Power Toggle: On",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t result, prev;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &result, &prev));
}

TEST(TestDecodeKelon, TestSwingToggleDryMode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendKelon(0x83800683);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(KELON, irsend.capture.decode_type);
  EXPECT_EQ(kKelonBits, irsend.capture.bits);
  EXPECT_EQ(0x83800683, irsend.capture.value);
  EXPECT_EQ(
      "Temp: 26C, Mode: 3 (Dry), Fan: 0 (Auto), Sleep: Off, Dry: 0, Timer:"
      " Off, Turbo: Off, Swing(V) Toggle: On",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t result, prev;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &result, &prev));
}

TEST(TestDecodeKelon, TestDryGradeNegativeValue) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendKelon(0x83600683);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(KELON, irsend.capture.decode_type);
  EXPECT_EQ(kKelonBits, irsend.capture.bits);
  EXPECT_EQ(0x83600683, irsend.capture.value);
  EXPECT_EQ(
      "Temp: 26C, Mode: 3 (Dry), Fan: 0 (Auto), Sleep: Off, Dry: -2,"
      " Timer: Off, Turbo: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t result, prev;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &result, &prev));
}

TEST(TestIRKelonClass, SetAndGetRaw) {
  uint64_t rawData = 0x100B0A010683;
  IRKelonAc ac(kGpioUnused);
  ac.setRaw(rawData);
  EXPECT_EQ(rawData, ac.getRaw());
}

TEST(TestIRKelonClass, SetAndGetTemp) {
  IRKelonAc ac(kGpioUnused);

  for (uint8_t temp = kKelonMinTemp; temp <= kKelonMaxTemp; temp++) {
    ac.setTemp(temp);
    EXPECT_EQ(temp, ac.getTemp());
  }

  // Check bounds
  ac.setTemp(kKelonMinTemp - 1);
  EXPECT_EQ(kKelonMinTemp, ac.getTemp());
  ac.setTemp(kKelonMaxTemp + 1);
  EXPECT_EQ(kKelonMaxTemp, ac.getTemp());
}

TEST(TestIRKelonClass, SetAndGetTimer) {
  IRKelonAc ac(kGpioUnused);

  // 0.5h to 10h timers have a granularity of 30 minutes
  for (uint16_t minutes = 0; minutes <= 60 * 10; minutes += 30) {
    ac.setTimer(minutes);
    EXPECT_EQ(minutes, ac.getTimer());
  }
  // 10h to 24h timers have a granularity of 1h
  for (uint16_t minutes = 600; minutes <= 60 * 24; minutes += 60) {
    ac.setTimer(minutes);
    EXPECT_EQ(minutes, ac.getTimer());
  }
  // For 10h to 24h timers, we expect the minutes to be floored down to the hour
  for (uint16_t minutes = 600 + 30; minutes <= 60 * 24; minutes += 60) {
    ac.setTimer(minutes);
    EXPECT_EQ(minutes - 30, ac.getTimer());
  }
}

TEST(TestIRKelonClass, CheckToggles) {
  IRKelonAc ac(kGpioUnused);

  ac.setTogglePower(true);
  EXPECT_TRUE(ac.getTogglePower());
  ac.send();
  EXPECT_FALSE(ac.getTogglePower());

  ac.setToggleSwingVertical(true);
  EXPECT_TRUE(ac.getToggleSwingVertical());
  ac.send();
  EXPECT_FALSE(ac.getToggleSwingVertical());

  // Known state with a power toggle
  ac.setRaw(0x62040683);
  EXPECT_TRUE(ac.getTogglePower());

  // Known state with a swing toggle
  ac.setRaw(0x62800683);
  EXPECT_TRUE(ac.getToggleSwingVertical());
}

TEST(TestIRKelonClass, SetAndGetMode) {
  IRKelonAc ac(kGpioUnused);

  uint8_t initial_temp = 20;
  ac.setMode(kKelonModeHeat);
  ac.setTemp(initial_temp);

  ac.setMode(kKelonModeCool);
  EXPECT_EQ(kKelonModeCool, ac.getMode());
  EXPECT_EQ(initial_temp, ac.getTemp());

  ac.setMode(kKelonModeHeat);
  EXPECT_EQ(kKelonModeHeat, ac.getMode());
  EXPECT_EQ(initial_temp, ac.getTemp());

  ac.setMode(kKelonModeDry);
  EXPECT_EQ(kKelonModeDry, ac.getMode());
  EXPECT_EQ(25, ac.getTemp());

  // Should revert back to previous temp
  ac.setMode(kKelonModeHeat);
  EXPECT_EQ(kKelonModeHeat, ac.getMode());
  EXPECT_EQ(initial_temp, ac.getTemp());

  ac.setMode(kKelonModeFan);
  EXPECT_EQ(kKelonModeFan, ac.getMode());
  EXPECT_EQ(25, ac.getTemp());

  // Should revert back to previous temp
  ac.setMode(kKelonModeHeat);
  EXPECT_EQ(kKelonModeHeat, ac.getMode());
  EXPECT_EQ(initial_temp, ac.getTemp());

  ac.setMode(kKelonModeSmart);
  EXPECT_EQ(kKelonModeSmart, ac.getMode());
  EXPECT_EQ(26, ac.getTemp());

  // Should revert back to previous temp
  ac.setMode(kKelonModeHeat);
  EXPECT_EQ(kKelonModeHeat, ac.getMode());
  EXPECT_EQ(initial_temp, ac.getTemp());
}

TEST(TestIRKelonClass, CheckSuperCoolMode) {
  IRKelonAc ac(kGpioUnused);

  uint8_t initial_temp = 20;
  uint8_t initial_fan = kKelonFanMin;
  ac.setMode(kKelonModeHeat);
  ac.setTemp(initial_temp);
  ac.setFan(initial_fan);

  ac.setSupercool(true);
  EXPECT_TRUE(ac.getSupercool());
  EXPECT_EQ(kKelonModeCool, ac.getMode());
  EXPECT_EQ(kKelonMinTemp, ac.getTemp());
  EXPECT_EQ(kKelonFanMax, ac.getFan());

  // Should revert back to previous temp and mode
  ac.setSupercool(false);
  EXPECT_EQ(kKelonModeHeat, ac.getMode());
  EXPECT_EQ(initial_temp, ac.getTemp());
  EXPECT_EQ(initial_fan, ac.getFan());

  ac.setSupercool(true);
  EXPECT_TRUE(ac.getSupercool());
  EXPECT_EQ(kKelonModeCool, ac.getMode());
  EXPECT_EQ(kKelonMinTemp, ac.getTemp());
  EXPECT_EQ(kKelonFanMax, ac.getFan());

  // Setting any mode should cancel supercool
  ac.setMode(kKelonModeHeat);
  EXPECT_EQ(kKelonModeHeat, ac.getMode());
  EXPECT_EQ(initial_temp, ac.getTemp());
  EXPECT_EQ(initial_fan, ac.getFan());
}

TEST(TestIRKelonClass, SetAndGetDryGrade) {
  IRKelonAc ac(kGpioUnused);

  for (int i = -2; i <= 2; i++) {
    ac.setDryGrade(i);
    EXPECT_EQ(i, ac.getDryGrade());
  }
  // Check bounds
  ac.setDryGrade(3);
  EXPECT_EQ(2, ac.getDryGrade());
  ac.setDryGrade(-3);
  EXPECT_EQ(-2, ac.getDryGrade());
}

TEST(TestIRKelonClass, toCommon) {
  IRKelonAc ac(kGpioUnused);

  ac.setSleep(false);
  ac.setTemp(23);
  ac.setMode(kKelonModeHeat);
  ac.setFan(kAmcorFanMed);
  auto common = ac.toCommon();

  EXPECT_EQ(decode_type_t::KELON, common.protocol);
  EXPECT_EQ(23, common.degrees);
  EXPECT_TRUE(common.celsius);
  EXPECT_EQ(stdAc::fanspeed_t::kMedium, common.fanspeed);
  EXPECT_FALSE(common.turbo);
  EXPECT_EQ(-1, common.sleep);

  ac.setSleep(true);
  ac.setSupercool(true);

  common = ac.toCommon();
  EXPECT_EQ(decode_type_t::KELON, common.protocol);
  EXPECT_EQ(kKelonMinTemp, common.degrees);
  EXPECT_TRUE(common.celsius);
  EXPECT_EQ(stdAc::fanspeed_t::kHigh, common.fanspeed);
  EXPECT_TRUE(common.turbo);
  EXPECT_EQ(0, common.sleep);
}

TEST(TestIRKelonClass, toCommonToggles) {
  IRKelonAc ac(kGpioUnused);

  stdAc::state_t common = ac.toCommon();
  stdAc::state_t prev = common;

  EXPECT_TRUE(common.power);
  EXPECT_EQ(stdAc::swingv_t::kAuto, common.swingv);

  ac.setTogglePower(true);
  ac.setToggleSwingVertical(true);

  common = ac.toCommon(&prev);
  prev = common;

  EXPECT_FALSE(common.power);
  EXPECT_EQ(stdAc::swingv_t::kOff, common.swingv);

  ac.setTogglePower(true);
  ac.setToggleSwingVertical(true);

  common = ac.toCommon(&prev);

  EXPECT_TRUE(common.power);
  EXPECT_EQ(stdAc::swingv_t::kAuto, common.swingv);
}

TEST(TestUtils, Housekeeping) {
  // KELON
  ASSERT_EQ("KELON", typeToString(decode_type_t::KELON));
  ASSERT_EQ(decode_type_t::KELON, strToDecodeType("KELON"));
  ASSERT_FALSE(hasACState(decode_type_t::KELON));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::KELON));
  ASSERT_EQ(kKelonBits, IRsend::defaultBits(decode_type_t::KELON));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::KELON));

  // KELON168
  ASSERT_EQ("KELON168", typeToString(decode_type_t::KELON168));
  ASSERT_EQ(decode_type_t::KELON168, strToDecodeType("KELON168"));
  ASSERT_TRUE(hasACState(decode_type_t::KELON168));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::KELON168));
  ASSERT_EQ(kKelon168Bits, IRsend::defaultBits(decode_type_t::KELON168));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::KELON168));
}

TEST(TestDecodeKelon, Discussion1744) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/discussions/1744#discussioncomment-2061968
  // Note: This message is NOT at 48-bit Kelon protocol message.
  //       It is actually 48 + 64 + 56 = 168 bit (21 byte) message.
  //       i.e. A different protocol all together.
  const uint16_t rawData[343] = {
      8922, 4494,  // Header
      548, 1714, 524, 1718, 524, 572, 524, 550,  // Byte 0
      550, 556, 550, 560, 548, 586, 524, 1724,
      526, 564, 524, 1692, 550, 1696, 550, 552,  // Byte 1
      550, 554, 550, 560, 548, 586, 524, 550,
      548, 1688, 550, 1694, 548, 548, 550, 552,  // Byte 2
      548, 556, 548, 582, 524, 586, 524, 576,
      524, 540, 548, 568, 526, 572, 526, 548,    // Byte 3
      550, 1702, 550, 1706, 550, 1734, 526, 574,
      526, 564, 526, 544, 550, 548, 548, 550,    // Byte 4
      550, 554, 550, 582, 526, 586, 524, 550,
      548, 540, 550, 544, 550, 570, 526, 550,    // Byte 5
      550, 558, 546, 582, 524, 586, 524, 562,
      524, 7978,  // Section Footer
      548, 1690, 548, 568, 524, 1696, 548, 1700,  // Byte 6
      548, 554, 550, 560, 548, 560, 550, 1722,
      526, 1686, 550, 1692, 548, 1696, 550, 576,  // Byte 7
      524, 1704, 548, 558, 550, 562, 548, 550,
      550, 564, 524, 570, 524, 548, 548, 576,     // Byte 8
      524, 578, 526, 558, 550, 586, 524, 574,
      524, 566, 524, 568, 526, 548, 548, 576,     // Byte 9
      526, 556, 548, 558, 550, 560, 550, 550,
      550, 564, 526, 544, 548, 548, 548, 552,     // Byte 10
      548, 556, 548, 582, 526, 564, 548, 550,
      550, 540, 548, 568, 526, 546, 550, 550,     // Byte 11
      550, 580, 524, 558, 550, 560, 550, 1698,
      550, 542, 548, 542, 550, 546, 550, 1722,    // Byte 12
      524, 1706, 548, 582, 526, 586, 524, 574,
      526, 1690, 548, 544, 550, 546, 550, 552,    // Byte 13
      548, 1726, 526, 1730, 524, 1734, 526, 562,
      524, 7976,  // Section footer
      550, 566, 524, 544, 550, 570, 526, 550,     // Byte 14
      548, 554, 550, 1706, 550, 562, 548, 574,
      526, 542, 548, 544, 550, 572, 526, 548,     // Byte 15
      552, 554, 550, 582, 526, 584, 526, 574,
      524, 542, 548, 544, 548, 572, 524, 552,     // Byte 16
      548, 578, 524, 560, 548, 562, 548, 550,
      550, 540, 550, 546, 548, 572, 524, 576,     // Byte 17
      526, 556, 548, 560, 546, 564, 548, 574,
      526, 540, 550, 546, 546, 546, 550, 1698,    // Byte 18
      550, 1728, 524, 1706, 548, 564, 548, 574,
      524, 544, 548, 568, 526, 548, 548, 574,     // Byte 19
      524, 554, 550, 558, 550, 562, 548, 550,
      550, 566, 524, 544, 548, 546, 552, 1698,    // Byte 20
      548, 1702, 550, 560, 546, 586, 524, 538,
      546  // Footer
  };  // KELON 178D000070030683

  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 343, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_NE(KELON, irsend.capture.decode_type);  // Not a KELON message
  EXPECT_NE(kKelonBits, irsend.capture.bits);    // Not a 48 bit message.
}

TEST(TestDecodeKelon168, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/discussions/1744#discussioncomment-2061968
  const uint16_t rawData[343] = {
      8922, 4494,  // Header
      548, 1714, 524, 1718, 524, 572, 524, 550,  // Byte 0
      550, 556, 550, 560, 548, 586, 524, 1724,
      526, 564, 524, 1692, 550, 1696, 550, 552,  // Byte 1
      550, 554, 550, 560, 548, 586, 524, 550,
      548, 1688, 550, 1694, 548, 548, 550, 552,  // Byte 2
      548, 556, 548, 582, 524, 586, 524, 576,
      524, 540, 548, 568, 526, 572, 526, 548,    // Byte 3
      550, 1702, 550, 1706, 550, 1734, 526, 574,
      526, 564, 526, 544, 550, 548, 548, 550,    // Byte 4
      550, 554, 550, 582, 526, 586, 524, 550,
      548, 540, 550, 544, 550, 570, 526, 550,    // Byte 5
      550, 558, 546, 582, 524, 586, 524, 562,
      524, 7978,  // Section Footer
      548, 1690, 548, 568, 524, 1696, 548, 1700,  // Byte 6
      548, 554, 550, 560, 548, 560, 550, 1722,
      526, 1686, 550, 1692, 548, 1696, 550, 576,  // Byte 7
      524, 1704, 548, 558, 550, 562, 548, 550,
      550, 564, 524, 570, 524, 548, 548, 576,     // Byte 8
      524, 578, 526, 558, 550, 586, 524, 574,
      524, 566, 524, 568, 526, 548, 548, 576,     // Byte 9
      526, 556, 548, 558, 550, 560, 550, 550,
      550, 564, 526, 544, 548, 548, 548, 552,     // Byte 10
      548, 556, 548, 582, 526, 564, 548, 550,
      550, 540, 548, 568, 526, 546, 550, 550,     // Byte 11
      550, 580, 524, 558, 550, 560, 550, 1698,
      550, 542, 548, 542, 550, 546, 550, 1722,    // Byte 12
      524, 1706, 548, 582, 526, 586, 524, 574,
      526, 1690, 548, 544, 550, 546, 550, 552,    // Byte 13
      548, 1726, 526, 1730, 524, 1734, 526, 562,
      524, 7976,  // Section footer
      550, 566, 524, 544, 550, 570, 526, 550,     // Byte 14
      548, 554, 550, 1706, 550, 562, 548, 574,
      526, 542, 548, 544, 550, 572, 526, 548,     // Byte 15
      552, 554, 550, 582, 526, 584, 526, 574,
      524, 542, 548, 544, 548, 572, 524, 552,     // Byte 16
      548, 578, 524, 560, 548, 562, 548, 550,
      550, 540, 550, 546, 548, 572, 524, 576,     // Byte 17
      526, 556, 548, 560, 546, 564, 548, 574,
      526, 540, 550, 546, 546, 546, 550, 1698,    // Byte 18
      550, 1728, 524, 1706, 548, 564, 548, 574,
      524, 544, 548, 568, 526, 548, 548, 574,     // Byte 19
      524, 554, 550, 558, 550, 562, 548, 550,
      550, 566, 524, 544, 548, 546, 552, 1698,    // Byte 20
      548, 1702, 550, 560, 546, 586, 524, 538,
      546  // Footer
  };  // KELON 178D000070030683

  const uint8_t expected[kKelon168StateLength] = {
      0x83, 0x06, 0x03, 0x70, 0x00, 0x00, 0x8D,
      0x17, 0x00, 0x00, 0x00, 0x80, 0x18, 0x71,
      0x20, 0x00, 0x00, 0x00, 0x38, 0x00, 0x18};

  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 343, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::KELON168, irsend.capture.decode_type);
  EXPECT_EQ(kKelon168Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ("", IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_FALSE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

TEST(TestDecodeKelon168, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);

  const uint8_t expected[kKelon168StateLength] = {
      0x83, 0x06, 0x03, 0x70, 0x00, 0x00, 0x8D,
      0x17, 0x00, 0x00, 0x00, 0x80, 0x18, 0x71,
      0x20, 0x00, 0x00, 0x00, 0x38, 0x00, 0x18};

  irsend.begin();
  irsend.reset();
  irsend.sendKelon168(expected);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::KELON168, irsend.capture.decode_type);
  EXPECT_EQ(kKelon168Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ("", IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_FALSE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}
