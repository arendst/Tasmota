// Copyright 2025 GottemHams

#include "ir_Eurom.h"
#include "gtest/gtest.h"
#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"

TEST(TestEurom, Housekeeping) {
  ASSERT_EQ("EUROM", typeToString(decode_type_t::EUROM));
  ASSERT_EQ(decode_type_t::EUROM, strToDecodeType("EUROM"));
  ASSERT_TRUE(hasACState(decode_type_t::EUROM));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::EUROM));
  ASSERT_EQ(kEuromBits, IRsend::defaultBits(decode_type_t::EUROM));
}

/// Tests for sendEurom().

/// Test sending typical data only.
TEST(TestSendEurom, SendDataOnly) {
  IRsendTest irsend(kGpioUnused);

  const uint8_t state[kEuromStateLength] = {
    0x18, 0x27,
    0x71,  // Cooling mode, 23 C
    0x80,  // Power on, swing off
    0x00,  // No Fahrenheit
    0x00,  // Sleep disabled, no "on timer"
    0x00,
    0x80,  // No "off timer"
    0x00,  // "Off timer" disabled
    0x80,
    0x10,  // Low fan
    0x21,  // Checksum
  };

  irsend.begin();
  irsend.reset();
  irsend.sendEurom(state);
  EXPECT_EQ(
    "f38000d50"
    "m3257s3187"
    "m454s355m454s355m454s355m454s1162m454s1162m454s355m454s355m454s355m454s355"
    "m454s355m454s1162m454s355m454s355m454s1162m454s1162m454s1162m454s355m454"
    "s1162m454s1162m454s1162m454s355m454s355m454s355m454s1162m454s1162m454s355"
    "m454s355m454s355m454s355m454s355m454s355m454s355m454s355m454s355m454s355"
    "m454s355m454s355m454s355m454s355m454s355m454s355m454s355m454s355m454s355"
    "m454s355m454s355m454s355m454s355m454s355m454s355m454s355m454s355m454s355"
    "m454s355m454s355m454s355m454s1162m454s355m454s355m454s355m454s355m454s355"
    "m454s355m454s355m454s355m454s355m454s355m454s355m454s355m454s355m454s355"
    "m454s355m454s1162m454s355m454s355m454s355m454s355m454s355m454s355m454s355"
    "m454s355m454s355m454s355m454s1162m454s355m454s355m454s355m454s355m454s355"
    "m454s355m454s1162m454s355m454s355m454s355m454s355m454s1162m454"
    "s50058",
      irsend.outputStr());

  irsend.reset();
}

/// Tests for decodeEurom().

/// Decode a normal Eurom message.
TEST(TestDecodeEurom, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);

  // This is the same state as used in SendDataOnly
  const uint8_t state[kEuromStateLength] = {
    0x18, 0x27,
    0x71,  // Cooling mode, 23 C
    0x80,  // Power on, swing off
    0x00,  // No Fahrenheit
    0x00,  // Sleep disabled, no "on timer"
    0x00,
    0x80,  // No "off timer"
    0x00,  // "Off timer" disabled
    0x80,
    0x10,  // Low fan
    0x21,  // Checksum
  };

  irsend.begin();
  irsend.reset();
  irsend.sendEurom(state);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::EUROM, irsend.capture.decode_type);
  EXPECT_EQ(kEuromBits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);

  EXPECT_STATE_EQ(state, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
    "Power: On, Mode: 1 (Cool), Temp: 23C, Fan: 16 (Low), Swing(V): Off"
    ", Sleep: Off, Off Timer: Off, On Timer: Off",
      IRAcUtils::resultAcToString(&irsend.capture));

  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));

  irsend.reset();
}

/// Decode a real example.
TEST(TestDecodeEurom, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);

  // UNKNOWN 1C60B17
  const uint16_t raw_data[391] = {
    3318, 3136,
    490, 318, 490, 322, 486, 324, 488, 1126, 490, 1118, 490, 322, 490, 318, 490,
    326, 490, 318, 488, 322, 486, 1124, 490, 328, 488, 318, 490, 1124, 490,
    1122, 486, 1130, 484, 324, 490, 1122, 486, 1122, 490, 1128, 490, 322, 486,
    322, 490, 322, 486, 1132, 484, 1124, 488, 1124, 486, 322, 490, 328, 488,
    318, 490, 322, 486, 322, 490, 328, 486, 322, 490, 322, 486, 322, 486, 330,
    486, 322, 486, 322, 490, 322, 486, 332, 486, 322, 486, 322, 490, 322, 484,
    328, 490, 322, 486, 322, 486, 322, 490, 326, 484, 324, 488, 324, 484, 324,
    484, 332, 486, 322, 486, 326, 486, 322, 486, 330, 486, 1122, 490, 324, 486,
    322, 484, 332, 486, 322, 486, 328, 484, 322, 486, 332, 484, 322, 490, 324,
    484, 322, 486, 332, 486, 326, 486, 322, 486, 322, 484, 336, 486, 1124, 486,
    348, 458, 328, 486, 330, 486, 350, 458, 326, 486, 324, 484, 332, 484, 326,
    486, 322, 486, 322, 486, 1136, 486, 322, 484, 348, 438, 348, 484, 332, 484,
    348, 460, 326, 486, 1122, 486, 336, 486, 344, 464, 1148, 438, 348, 486,
    1130, 486,
    50010,
    3308, 3140,
    490, 322, 486, 322, 486, 322, 486, 1132, 490, 1122, 486, 324, 484, 328, 484,
    328, 490, 322, 486, 322, 486, 1126, 486, 328, 488, 324, 484, 1124, 490,
    1122, 486, 1132, 486, 322, 490, 1122, 486, 1128, 484, 1132, 486, 322, 486,
    322, 490, 322, 486, 1132, 486, 1122, 490, 1122, 486, 322, 490, 326, 486,
    328, 484, 324, 486, 322, 484, 332, 486, 322, 486, 328, 484, 322, 486, 330,
    486, 324, 484, 322, 486, 326, 486, 328, 484, 328, 486, 324, 484,  322, 486,
    330, 486, 322, 486, 326, 486, 322, 486, 332, 486, 322, 484, 348,  460, 328,
    486, 330, 482, 326, 486, 322, 486, 322, 486, 330, 486, 1128, 484, 324, 484,
    322, 486, 332, 484, 348, 460, 328, 484, 324, 484, 358, 460, 326,  486, 322,
    486, 348, 460, 336, 480, 328, 484, 348, 460, 328, 480, 336, 486,  1148, 460,
    328, 484, 324, 484, 358, 460, 326, 482, 326, 486, 348, 458, 336, 480, 348,
    464, 348, 460, 328, 480, 1136, 484, 348, 460, 328, 480, 328, 484, 358, 460,
    326, 482, 348, 460, 1152, 460, 356, 460, 354, 458, 1148, 460, 328, 484,
    1162, 460,
  };

  // Note that this is a different state than before
  const uint8_t expected_state[kEuromStateLength] = {
    0x18, 0x27, 0x71, 0xC0, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x10, 0x25,
  };

  irsend.begin();
  irsend.reset();
  irsend.sendRaw(raw_data, 391, kEuromFreq);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::EUROM, irsend.capture.decode_type);
  EXPECT_EQ(kEuromBits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);

  EXPECT_STATE_EQ(expected_state, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
    "Power: On, Mode: 1 (Cool), Temp: 23C, Fan: 16 (Low), Swing(V): On"
    ", Sleep: Off, Off Timer: Off, On Timer: Off",
      IRAcUtils::resultAcToString(&irsend.capture));

  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));

  irsend.reset();
}

/// Decode a real example without repeat.
TEST(TestDecodeEurom, RealExampleNoRepeat) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);

  // UNKNOWN 14601C1D
  const uint16_t raw_data[195] = {
    3260, 3186,
    468, 344, 438, 374, 438, 370, 438, 1178, 438, 1174, 438, 374, 438, 370, 438,
    378, 438, 370, 438, 374, 438, 1174, 438, 378, 438, 370, 438, 1174, 438,
    1170, 438, 1184, 438, 370, 438, 1174, 438, 370, 438, 1182, 434, 374, 438,
    374, 434, 374, 438, 1178, 438, 374, 438, 370, 438, 374, 434, 382, 434, 374,
    438, 374, 434, 374, 438, 378, 438, 370, 438, 374, 434, 374, 438, 378, 438,
    370, 438, 374, 434, 374, 438, 378, 438, 370, 438, 374, 438, 370, 438, 378,
    438, 374, 434, 374, 438, 370, 438, 378, 438, 374, 434, 374, 438, 374, 434,
    378, 438, 374, 438, 370, 438, 374, 434, 382, 434, 1174, 438, 374, 438, 370,
    438, 378, 438, 370, 438, 374, 438, 370, 438, 382, 434, 374, 438, 370, 438,
    374, 438, 378, 438, 374, 434, 374, 438, 370, 438, 382, 434, 1174, 464, 348,
    434, 374, 438, 382, 434, 374, 438, 370, 438, 374, 438, 378, 438, 374, 434,
    374, 438, 370, 438, 1182, 438, 374, 434, 374, 438, 370, 438, 382, 434, 374,
    438, 374, 434, 374, 438, 1182, 434, 374, 438, 1174, 434, 1174, 438, 1182,
    434,
  };

  // This is also another state than all the earlier tests
  const uint8_t expected_state[kEuromStateLength] = {
    0x18, 0x27, 0x51, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x10, 0x17,
  };

  irsend.begin();
  irsend.reset();
  irsend.sendRaw(raw_data, 195, kEuromFreq);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::EUROM, irsend.capture.decode_type);
  EXPECT_EQ(kEuromBits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);

  EXPECT_STATE_EQ(expected_state, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
    "Power: Off, Mode: 1 (Cool), Temp: 23C, Fan: 16 (Low), Swing(V): Off"
    ", Sleep: Off, Off Timer: Off, On Timer: Off",
      IRAcUtils::resultAcToString(&irsend.capture));

  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));

  irsend.reset();
}

/// Tests for the IREuromAc class.

/// Test power setting and getting.
TEST(TestEuromAc, SetAndGetPower) {
  IREuromAc ac(kGpioUnused);

  // The initial state is powered off
  ASSERT_FALSE(ac.getPower());

  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());
}

/// Test operation mode setting and getting.
TEST(TestEuromAc, SetAndGetMode) {
  IREuromAc ac(kGpioUnused);

  // The initial state is cooling mode
  ASSERT_EQ(kEuromCool, ac.getMode());

  // Temperature is not used in dehumidification/ventilation modes
  ac.setMode(kEuromDehumidify);
  EXPECT_EQ(kEuromDehumidify, ac.getMode());

  ac.setMode(kEuromVentilate);
  EXPECT_EQ(kEuromVentilate, ac.getMode());

  ac.setMode(kEuromHeat);
  EXPECT_EQ(kEuromHeat, ac.getMode());
}

/// Test temperature setting and getting.
TEST(TestEuromAc, SetAndGetTemperature) {
  IREuromAc ac(kGpioUnused);

  // The initial state is 23 C
  ASSERT_FALSE(ac.getTempIsFahrenheit());
  ASSERT_EQ(23, ac.getTemp());

  ac.setTemp(22);
  ASSERT_FALSE(ac.getTempIsFahrenheit());
  EXPECT_EQ(22, ac.getTemp());
}

/// Test temperature setting and getting with Fahrenheit.
TEST(TestEuromAc, SetAndGetTemperatureFahrenheit) {
  IREuromAc ac(kGpioUnused);

  // The initial state is not using Fahrenheit
  ASSERT_FALSE(ac.getTempIsFahrenheit());

  // This corresponds to 16 C
  ac.setTemp(70, true);
  ASSERT_TRUE(ac.getTempIsFahrenheit());
  EXPECT_EQ(70, ac.getTemp());
}

/// Test fan speed setting and getting.
TEST(TestEuromAc, SetAndGetFan) {
  IREuromAc ac(kGpioUnused);

  // The initial state is low fan
  ASSERT_EQ(kEuromFanLow, ac.getFan());

  ac.setFan(kEuromFanHigh);
  EXPECT_EQ(kEuromFanHigh, ac.getFan());
}

/// Test swing setting and getting.
TEST(TestEuromAc, SetAndGetSwing) {
  IREuromAc ac(kGpioUnused);

  // The initial state is swing disabled
  ASSERT_FALSE(ac.getSwing());

  ac.setSwing(true);
  EXPECT_TRUE(ac.getSwing());
}

/// Test sleep mode setting and getting.
TEST(TestEuromAc, SetAndGetSleep) {
  IREuromAc ac(kGpioUnused);

  // The initial state is sleep disabled
  ASSERT_FALSE(ac.getSleep());

  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
}

/// Test "off timer" setting and getting.
TEST(TestEuromAc, SetAndGetOffTimer) {
  IREuromAc ac(kGpioUnused);

  // The initial state is no timer
  ASSERT_EQ(kEuromTimerMin, ac.getOffTimer());

  ac.setOffTimer(kEuromTimerMax);
  EXPECT_EQ(kEuromTimerMax, ac.getOffTimer());
}

/// Test "on timer" setting and getting.
TEST(TestEuromAc, SetAndGetOnTimer) {
  IREuromAc ac(kGpioUnused);

  // The initial state is no timer
  ASSERT_EQ(kEuromTimerMin, ac.getOnTimer());

  ac.setOnTimer(kEuromTimerMax);
  EXPECT_EQ(kEuromTimerMax, ac.getOnTimer());
}

/// Test checksumming for the initial state.
TEST(TestEuromAc, ChecksumInitial) {
  IREuromAc ac(kGpioUnused);

  // The initial state is powered off, cooling mode, 23 C, low fan, swing and
  // sleep disabled, no timers
  const uint8_t *raw_state = ac.getRaw();
  ASSERT_EQ(0x19, raw_state[kEuromStateLength - 1]);
}

/// Test checksumming with every "feature" set to the "highest" value, which
/// also includes the special case of using the max temperature.
TEST(TestEuromAc, ChecksumHigh) {
  IREuromAc ac(kGpioUnused);

  // The initial state is powered off, cooling mode, 23 C, low fan, swing and
  // sleep disabled, no timers
  ac.setPower(true);
  ac.setMode(kEuromHeat);
  ac.setTemp(kEuromMaxTempF, true);
  ac.setFan(kEuromFanHigh);
  ac.setSwing(true);
  ac.setSleep(true);
  ac.setOffTimer(kEuromTimerMax);
  ac.setOnTimer(kEuromTimerMax);

  const uint8_t expected_state[kEuromStateLength] = {
    0x18, 0x27,
    0x0C,  // Heating mode, 32 C (changing Fahrenheit updates this too)
    0xC0,  // Power on, swing on
    0x5E,  // 90 F
    0x64,  // Sleep enabled, "on timer" set to 24 hours
    0x00,
    0xA4,  // "Off timer" set to 24 hours
    0x80,  // "Off timer" enabled
    0x80,
    0x40,  // High fan
    0x57,  // Checksum
  };

  EXPECT_STATE_EQ(expected_state, ac.getRaw(), kEuromBits);
}
