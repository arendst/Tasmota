// Copyright 2020 crankyoldgit

#include "ir_Voltas.h"
#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for decodeVoltas().

TEST(TestDecodeVoltas, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  const uint16_t rawData[161] = {
      1002, 584, 1000, 586, 1000, 2568, 1002, 2570, 1002, 586, 998, 588, 1000,
      2568, 1002, 2570, 1002, 2572, 1002, 584, 1002, 586, 1000, 584, 1000, 586,
      1002, 2568, 1004, 584, 1000, 586, 1002, 2568, 1002, 584, 1002, 584, 1004,
      584, 1000, 2568, 1002, 586, 1000, 586, 998, 590, 998, 584, 1002, 584,
      1000, 586, 1000, 2570, 1002, 2568, 1004, 584, 1000, 584, 1002, 584, 1002,
      582, 1004, 584, 1002, 2568, 1002, 2570, 1004, 2570, 1000, 586, 1002, 2568,
      1004, 2568, 1006, 584, 1000, 584, 1002, 2568, 1002, 2570, 1002, 2568,
      1002, 586, 1002, 2570, 1000, 2570, 1002, 588, 998, 586, 1000, 2568, 1004,
      2568, 1004, 2568, 1002, 588, 998, 2570, 1002, 2568, 1004, 586, 1002, 584,
      1000, 586, 1000, 2570, 1000, 586, 1000, 584, 1002, 586, 1000, 2568, 1004,
      584, 1000, 586, 1000, 586, 1002, 584, 1002, 586, 1000, 586, 1000, 586,
      1000, 586, 1000, 2568, 1002, 2568, 1002, 2568, 1004, 586, 1000, 584,
      1000, 2570, 1004, 2568, 1004, 584, 1002};
  const uint8_t expected[kVoltasStateLength] = {
      0x33, 0x84, 0x88, 0x18, 0x3B, 0x3B, 0x3B, 0x11, 0x00, 0xE6};

  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 161, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::VOLTAS, irsend.capture.decode_type);
  ASSERT_EQ(kVoltasBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Model: 1 (122LZF), Power: On, Mode: 4 (Dry), Temp: 24C, Fan: 4 (Low), "
      "Swing(V): Off, Swing(H): N/A, "
      "Turbo: Off, Econo: Off, WiFi: On, Light: Off, Sleep: Off, "
      "On Timer: Off, Off Timer: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

TEST(TestDecodeVoltas, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  irsend.reset();
  const uint8_t expected[kVoltasStateLength] = {
      0x33, 0x84, 0x88, 0x18, 0x3B, 0x3B, 0x3B, 0x11, 0x00, 0xE6};
  // power
  irsend.sendVoltas(expected);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::VOLTAS, irsend.capture.decode_type);
  EXPECT_EQ(kVoltasBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("VOLTAS", typeToString(decode_type_t::VOLTAS));
  ASSERT_EQ(decode_type_t::VOLTAS, strToDecodeType("VOLTAS"));
  ASSERT_TRUE(hasACState(decode_type_t::VOLTAS));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::VOLTAS));
  ASSERT_EQ(kVoltasBits, IRsend::defaultBits(decode_type_t::VOLTAS));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::VOLTAS));
}

TEST(TestIRVoltasClass, Checksums) {
  const uint8_t valid[kVoltasStateLength] = {
      0x33, 0x84, 0x88, 0x18, 0x3B, 0x3B, 0x3B, 0x11, 0x00, 0xE6};
  EXPECT_TRUE(IRVoltas::validChecksum(valid));
  EXPECT_FALSE(IRVoltas::validChecksum(valid, kVoltasStateLength - 1));
  EXPECT_EQ(0xE6, IRVoltas::calcChecksum(valid));
  const uint8_t badchecksum[kVoltasStateLength] = {
      0x33, 0x84, 0x88, 0x18, 0x3B, 0x3B, 0x3B, 0x11, 0x00, 0x00};
  EXPECT_FALSE(IRVoltas::validChecksum(badchecksum));
  EXPECT_EQ(0xE6, IRVoltas::calcChecksum(badchecksum));
  const uint8_t kReset[kVoltasStateLength] = {
      0x33, 0x28, 0x00, 0x17, 0x3B, 0x3B, 0x3B, 0x11, 0x00, 0xCB};
  EXPECT_TRUE(IRVoltas::validChecksum(kReset));
  EXPECT_EQ(0xCB, IRVoltas::calcChecksum(kReset));
}

TEST(TestIRVoltasClass, SetandGetRaw) {
  const uint8_t valid[kVoltasStateLength] = {
      0x33, 0x84, 0x88, 0x18, 0x3B, 0x3B, 0x3B, 0x11, 0x00, 0xE6};
  const uint8_t badchecksum[kVoltasStateLength] = {
      0x33, 0x84, 0x88, 0x18, 0x3B, 0x3B, 0x3B, 0x11, 0x00, 0x00};
  IRVoltas ac(kGpioUnused);

  ac.setRaw(valid);
  EXPECT_STATE_EQ(valid, ac.getRaw(), kVoltasBits);
  ac.setRaw(badchecksum);
  EXPECT_STATE_EQ(valid, ac.getRaw(), kVoltasBits);
}

TEST(TestIRVoltasClass, Power) {
  IRVoltas ac(kGpioUnused);
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

TEST(TestIRVoltasClass, Wifi) {
  IRVoltas ac(kGpioUnused);
  ac.begin();

  ac.setWifi(false);
  EXPECT_FALSE(ac.getWifi());
  ac.setWifi(true);
  EXPECT_TRUE(ac.getWifi());
  ac.setWifi(false);
  EXPECT_FALSE(ac.getWifi());
}

TEST(TestIRVoltasClass, Turbo) {
  IRVoltas ac(kGpioUnused);
  ac.begin();

  ac.setTurbo(false);
  EXPECT_FALSE(ac.getTurbo());
  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());
  ac.setTurbo(false);
  EXPECT_FALSE(ac.getTurbo());
}

TEST(TestIRVoltasClass, Sleep) {
  IRVoltas ac(kGpioUnused);
  ac.begin();

  ac.setSleep(false);
  EXPECT_FALSE(ac.getSleep());
  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
  ac.setSleep(false);
  EXPECT_FALSE(ac.getSleep());
}

TEST(TestIRVoltasClass, Econo) {
  IRVoltas ac(kGpioUnused);
  ac.begin();
  // Control of econo mode is only available in cool.
  ac.setMode(kVoltasCool);
  ac.setEcono(false);
  EXPECT_FALSE(ac.getEcono());
  ac.setEcono(true);
  EXPECT_TRUE(ac.getEcono());
  ac.setEcono(false);
  EXPECT_FALSE(ac.getEcono());
  ac.setEcono(true);
  ac.setMode(kVoltasHeat);  // Control of econo mode should now be disabled.
  EXPECT_FALSE(ac.getEcono());
  ac.setEcono(true);
  EXPECT_FALSE(ac.getEcono());
}

TEST(TestIRVoltasClass, Light) {
  IRVoltas ac(kGpioUnused);
  ac.begin();

  ac.setLight(false);
  EXPECT_FALSE(ac.getLight());
  ac.setLight(true);
  EXPECT_TRUE(ac.getLight());
  ac.setLight(false);
  EXPECT_FALSE(ac.getLight());

  const uint8_t light_off[kVoltasStateLength] = {
      0x33, 0x84, 0x88, 0x18, 0x3B, 0x3B, 0x3B, 0x11, 0x00, 0xE6};
  ac.setRaw(light_off);
  EXPECT_FALSE(ac.getLight());
  const uint8_t light_on[kVoltasStateLength] = {
      0x33, 0x84, 0x88, 0x18, 0x3B, 0x3B, 0x3B, 0x11, 0x20, 0xC6};
  ac.setRaw(light_on);
  EXPECT_TRUE(ac.getLight());
  ac.setLight(false);
  EXPECT_STATE_EQ(light_off, ac.getRaw(), kVoltasBits);
  ac.setLight(true);
  EXPECT_STATE_EQ(light_on, ac.getRaw(), kVoltasBits);
}

TEST(TestVoltasClass, OperatingMode) {
  IRVoltas ac(kGpioUnused);
  ac.begin();

  ac.setMode(kVoltasCool);
  EXPECT_EQ(kVoltasCool, ac.getMode());
  ac.setMode(kVoltasFan);
  EXPECT_EQ(kVoltasFan, ac.getMode());
  ac.setMode(kVoltasDry);
  EXPECT_EQ(kVoltasDry, ac.getMode());
  ac.setMode(kVoltasHeat);
  EXPECT_EQ(kVoltasHeat, ac.getMode());

  ac.setMode(kVoltasCool - 1);
  EXPECT_EQ(kVoltasCool, ac.getMode());

  ac.setMode(kVoltasCool + 1);
  EXPECT_EQ(kVoltasCool, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kVoltasCool, ac.getMode());
}

TEST(TestVoltasClass, Temperature) {
  IRVoltas ac(kGpioUnused);
  ac.begin();

  ac.setTemp(kVoltasMinTemp);
  EXPECT_EQ(kVoltasMinTemp, ac.getTemp());

  ac.setTemp(kVoltasMinTemp + 1);
  EXPECT_EQ(kVoltasMinTemp + 1, ac.getTemp());

  ac.setTemp(kVoltasMaxTemp);
  EXPECT_EQ(kVoltasMaxTemp, ac.getTemp());

  ac.setTemp(kVoltasMinTemp - 1);
  EXPECT_EQ(kVoltasMinTemp, ac.getTemp());

  ac.setTemp(kVoltasMaxTemp + 1);
  EXPECT_EQ(kVoltasMaxTemp, ac.getTemp());

  ac.setTemp(23);
  EXPECT_EQ(23, ac.getTemp());

  ac.setTemp(0);
  EXPECT_EQ(kVoltasMinTemp, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kVoltasMaxTemp, ac.getTemp());
}

TEST(TestVoltasClass, FanSpeed) {
  IRVoltas ac(kGpioUnused);
  ac.begin();
  ac.setMode(kVoltasCool);  // All fan speeds are allowed in cool mode.
  ac.setFan(kVoltasFanLow);

  ac.setFan(kVoltasFanAuto);
  EXPECT_EQ(kVoltasFanAuto, ac.getFan());

  ac.setFan(kVoltasFanLow);
  EXPECT_EQ(kVoltasFanLow, ac.getFan());
  ac.setFan(kVoltasFanMed);
  EXPECT_EQ(kVoltasFanMed, ac.getFan());
  ac.setFan(kVoltasFanHigh);
  EXPECT_EQ(kVoltasFanHigh, ac.getFan());

  ac.setFan(0);
  EXPECT_EQ(kVoltasFanAuto, ac.getFan());

  ac.setFan(255);
  EXPECT_EQ(kVoltasFanAuto, ac.getFan());

  // Confirm auto speed isn't operable in Fan mode.
  ac.setMode(kVoltasFan);
  EXPECT_NE(kVoltasFanAuto, ac.getFan());
  ac.setFan(kVoltasFanLow);
  EXPECT_EQ(kVoltasFanLow, ac.getFan());
  ac.setFan(kVoltasFanMed);
  EXPECT_EQ(kVoltasFanMed, ac.getFan());
  ac.setFan(kVoltasFanHigh);
  EXPECT_EQ(kVoltasFanHigh, ac.getFan());
  ac.setFan(kVoltasFanAuto);
  EXPECT_NE(kVoltasFanAuto, ac.getFan());
}

TEST(TestVoltasClass, SwingV) {
  IRVoltas ac(kGpioUnused);
  ac.begin();

  ac.setSwingV(true);
  EXPECT_TRUE(ac.getSwingV());

  ac.setSwingV(false);
  EXPECT_EQ(false, ac.getSwingV());

  ac.setSwingV(true);
  EXPECT_TRUE(ac.getSwingV());
}

TEST(TestVoltasClass, SwingH) {
  IRVoltas ac(kGpioUnused);
  ac.begin();
  // This model allows full control.
  ac.setModel(voltas_ac_remote_model_t::kVoltasUnknown);

  ac.setSwingHChange(false);
  EXPECT_FALSE(ac.getSwingHChange());

  ac.setSwingH(true);
  EXPECT_TRUE(ac.getSwingH());
  EXPECT_TRUE(ac.getSwingHChange());

  ac.setSwingHChange(false);
  ac.setSwingH(false);
  EXPECT_FALSE(ac.getSwingH());
  EXPECT_TRUE(ac.getSwingHChange());

  ac.setSwingH(true);
  EXPECT_TRUE(ac.getSwingH());
  EXPECT_TRUE(ac.getSwingHChange());

  // Switch to a model that does not allow SwingH control.
  ac.setModel(voltas_ac_remote_model_t::kVoltas122LZF);
  EXPECT_FALSE(ac.getSwingHChange());
  EXPECT_FALSE(ac.getSwingH());
  ac.setSwingH(true);
  EXPECT_FALSE(ac.getSwingHChange());
  EXPECT_FALSE(ac.getSwingH());
  ac.setSwingH(false);
  EXPECT_FALSE(ac.getSwingHChange());
  EXPECT_FALSE(ac.getSwingH());
}

TEST(TestVoltasClass, HumanReadable) {
  IRVoltas ac(kGpioUnused);
  EXPECT_EQ(
      "Model: 1 (122LZF), Power: Off, Mode: 8 (Cool), Temp: 23C, "
      "Fan: 1 (High), Swing(V): Off, Swing(H): N/A, Turbo: Off, Econo: Off, "
      "WiFi: Off, Light: Off, Sleep: Off, On Timer: Off, Off Timer: Off",
      ac.toString());
  ac.on();
  ac.setTemp(21);
  ac.setFan(kVoltasFanAuto);
  ac.setSwingV(true);
  ac.setWifi(true);
  ac.setLight(true);
  ac.setTurbo(true);
  ac.setSleep(true);
  ac.setEcono(true);
  ac.setOnTime(2 * 60 + 17);
  ac.setMode(kVoltasHeat);  // Heat mode should cancel Sleep, Turbo, & Econo.
  EXPECT_EQ(
      "Model: 1 (122LZF), Power: On, Mode: 2 (Heat), Temp: 21C, "
      "Fan: 7 (Auto), Swing(V): On, Swing(H): N/A, Turbo: Off, Econo: Off, "
      "WiFi: On, Light: On, Sleep: Off, On Timer: 02:17, Off Timer: Off",
      ac.toString());
  ac.setOffTime(13 * 60 + 37);
  ac.setMode(kVoltasCool);
  ac.setTurbo(true);
  ac.setSleep(true);
  ac.setEcono(true);
  EXPECT_EQ(
      "Model: 1 (122LZF), Power: On, Mode: 8 (Cool), Temp: 21C, "
      "Fan: 7 (Auto), Swing(V): On, Swing(H): N/A, Turbo: On, Econo: On, "
      "WiFi: On, Light: On, Sleep: On, On Timer: 02:17, Off Timer: 13:37",
      ac.toString());
  ac.setModel(voltas_ac_remote_model_t::kVoltasUnknown);
  ac.setSwingH(true);
  EXPECT_EQ(
      "Model: 0 (UNKNOWN), Power: On, Mode: 8 (Cool), Temp: 21C, "
      "Fan: 7 (Auto), Swing(V): On, Swing(H): On, Turbo: On, Econo: On, "
      "WiFi: On, Light: On, Sleep: On, On Timer: 02:17, Off Timer: 13:37",
      ac.toString());
  ac.setModel(voltas_ac_remote_model_t::kVoltas122LZF);
  ac.setOnTime(0);
  EXPECT_EQ(
      "Model: 1 (122LZF), Power: On, Mode: 8 (Cool), Temp: 21C, "
      "Fan: 7 (Auto), Swing(V): On, Swing(H): N/A, Turbo: On, Econo: On, "
      "WiFi: On, Light: On, Sleep: On, On Timer: Off, Off Timer: 13:37",
      ac.toString());
  ac.setOffTime(0);
  EXPECT_EQ(
      "Model: 1 (122LZF), Power: On, Mode: 8 (Cool), Temp: 21C, "
      "Fan: 7 (Auto), Swing(V): On, Swing(H): N/A, Turbo: On, Econo: On, "
      "WiFi: On, Light: On, Sleep: On, On Timer: Off, Off Timer: Off",
      ac.toString());
}

TEST(TestVoltasClass, Timers) {
  IRVoltas ac(kGpioUnused);
  const uint8_t off_7hrs[10] = {  // Real Data
      0x33, 0x28, 0x80, 0x1B, 0x3B, 0x3B, 0x3B, 0x71, 0x40, 0xA7};
  ac.setRaw(off_7hrs);
  EXPECT_EQ(
      "Model: 1 (122LZF), Power: On, Mode: 8 (Cool), Temp: 27C, Fan: 1 (High), "
      "Swing(V): Off, Swing(H): N/A, Turbo: Off, Econo: Off, WiFi: Off, "
      "Light: Off, Sleep: Off, On Timer: Off, Off Timer: 06:59",
      ac.toString());
  const uint8_t off_16hrs[10] = {  // Real Data
      0x33, 0x28, 0x80, 0x1B, 0x3B, 0xBB, 0x3B, 0x41, 0x40, 0x57};
  ac.setRaw(off_16hrs);
  EXPECT_EQ(
    "Model: 1 (122LZF), Power: On, Mode: 8 (Cool), Temp: 27C, Fan: 1 (High), "
    "Swing(V): Off, Swing(H): N/A, Turbo: Off, Econo: Off, WiFi: Off, "
    "Light: Off, Sleep: Off, On Timer: Off, Off Timer: 15:59",
    ac.toString());
  ac.setOffTime(23 * 60 + 59);
  EXPECT_EQ(
    "Model: 1 (122LZF), Power: On, Mode: 8 (Cool), Temp: 27C, Fan: 1 (High), "
    "Swing(V): Off, Swing(H): N/A, Turbo: Off, Econo: Off, WiFi: Off, "
    "Light: Off, Sleep: Off, On Timer: Off, Off Timer: 23:59",
    ac.toString());
  const uint8_t off_24hrs[10] = {  // Real Data
      0x33, 0x28, 0x80, 0x1B, 0x3A, 0x3A, 0x3B, 0x01, 0x40, 0x19};
  ac.setRaw(off_24hrs);
  EXPECT_EQ(
    "Model: 1 (122LZF), Power: On, Mode: 8 (Cool), Temp: 27C, Fan: 1 (High), "
    "Swing(V): Off, Swing(H): N/A, Turbo: Off, Econo: Off, WiFi: Off, "
    "Light: Off, Sleep: Off, On Timer: Off, Off Timer: 23:58",
    ac.toString());
}
