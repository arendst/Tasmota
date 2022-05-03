// Copyright 2020-2021 David Conran

#include "ir_Mirage.h"
#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("MIRAGE", typeToString(decode_type_t::MIRAGE));
  ASSERT_EQ(decode_type_t::MIRAGE, strToDecodeType("MIRAGE"));
  ASSERT_TRUE(hasACState(decode_type_t::MIRAGE));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::MIRAGE));
  ASSERT_EQ(kMirageBits, IRsend::defaultBits(decode_type_t::MIRAGE));
  ASSERT_EQ(kMirageMinRepeat, IRsend::minRepeats(decode_type_t::MIRAGE));
}

// Tests for decodeMirage().
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1289
// Data from:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/1289#issuecomment-705826015
//   But it is corrected to adjust for poor capture.
TEST(TestDecodeMirage, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  const uint16_t rawData[243] = {
      8360, 4248, 582, 518, 556, 1582, 586, 1572, 528, 572, 556, 1590, 526, 572,
      554, 1586, 528, 578, 558, 1582, 556, 542, 558, 1598, 528, 572, 556, 1590,
      528, 1610, 556, 1600, 554, 546, 556, 544, 558, 542, 558, 542, 676, 400,
      606, 492, 582, 542, 556, 544, 556, 542, 558, 544, 556, 542, 556, 544, 558,
      542, 556, 544, 530, 570, 586, 516, 584, 514, 558, 542, 558, 542, 558, 542,
      554, 546, 558, 542, 558, 1582, 534, 542, 580, 552, 528, 1610, 556, 544,
      554, 546, 554, 544, 556, 544, 556, 544, 558, 542, 558, 552, 558, 542, 558,
      542, 558, 542, 556, 544, 558, 542, 558, 542, 554, 544, 584, 516, 558, 542,
      528, 572, 588, 512, 556, 544, 532, 568, 560, 542, 558, 542, 560, 540, 560,
      538, 530, 570, 558, 542, 558, 542, 560, 540, 558, 542, 558, 542, 530, 568,
      558, 542, 558, 542, 532, 570, 530, 570, 558, 542, 558, 542, 558, 542, 530,
      570, 530, 568, 560, 540, 560, 540, 532, 568, 558, 542, 558, 542, 532, 568,
      560, 542, 532, 568, 532, 568, 530, 570, 532, 570, 530, 570, 558, 540, 560,
      540, 558, 534, 558, 542, 556, 1600, 558, 1592, 558, 542, 560, 1590, 530,
      570, 530, 570, 556, 544, 560, 540, 556, 544, 558, 1582, 556, 544, 558,
      1600, 556, 542, 560, 542, 532, 568, 558, 542, 610, 1538, 504, 1646, 582,
      518, 528, 572, 528, 1612, 556, 544, 528, 580, 554};  // UNKNOWN 28DACDC4
  const uint8_t expected[kMirageStateLength] = {
      0x56, 0x75, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x16, 0x14, 0x26};
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 243, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::MIRAGE, irsend.capture.decode_type);
  ASSERT_EQ(kMirageBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Model: 1 (KKG9AC1), Power: On, Mode: 2 (Cool), Temp: 25C, "
      "Fan: 0 (Auto), Turbo: Off, Sleep: Off, Light: Off, "
      "Swing(V): 0 (Off), Clock: 14:16",
      IRAcUtils::resultAcToString(&irsend.capture));
}

TEST(TestDecodeMirage, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  const uint8_t expected[kMirageStateLength] = {
      0x56, 0x75, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x16, 0x14, 0x26};
  irsend.begin();
  irsend.reset();
  irsend.sendMirage(expected);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::MIRAGE, irsend.capture.decode_type);
  ASSERT_EQ(kMirageBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Model: 1 (KKG9AC1), Power: On, Mode: 2 (Cool), Temp: 25C, "
      "Fan: 0 (Auto), Turbo: Off, Sleep: Off, Light: Off, "
      "Swing(V): 0 (Off), Clock: 14:16",
      IRAcUtils::resultAcToString(&irsend.capture));
}

// Data from:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/1289#issuecomment-705624234
TEST(TestDecodeMirage, RealExampleWithDodgyHardwareCapture) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  const uint16_t rawData[243] = {
      8360, 4248, 582, 518, 556, 1582, 586, 1572, 528, 572, 556, 1590, 526, 572,
      554, 1586, 528, 578, 558, 1582, 556, 542, 558, 1598, 528, 572, 556, 1590,
      528, 1610, 556, 1600, 554, 546, 556, 544, 558, 542, 558, 542, 676, 400,
      606, 492, 582, 542, 556, 544, 556, 542, 558, 544, 556, 542, 556, 544, 558,
      542, 556, 544, 530, 570, 586, 516, 584, 514, 558, 542, 558, 542, 558, 542,
      554, 546, 558, 542, 558, 1582,
      734, 342,  // Really poor data here.
      580, 552, 528, 1610, 556, 544, 554, 546, 554, 544, 556, 544, 556, 544,
      558, 542, 558, 552, 558, 542, 558, 542, 558, 542, 556, 544, 558, 542, 558,
      542, 554, 544, 584, 516, 558, 542, 528, 572, 588, 512, 556, 544, 532, 568,
      560, 542, 558, 542, 560, 540, 560, 538, 530, 570, 558, 542, 558, 542, 560,
      540, 558, 542, 558, 542, 530, 568, 558, 542, 558, 542, 532, 570, 530, 570,
      558, 542, 558, 542, 558, 542, 530, 570, 530, 568, 560, 540, 560, 540, 532,
      568, 558, 542, 558, 542, 532, 568, 560, 542, 532, 568, 532, 568, 530, 570,
      532, 570, 530, 570, 558, 540, 560, 540, 558, 534, 558, 542, 556, 1600,
      558, 1592, 558, 542, 560, 1590, 530, 570, 530, 570, 556, 544, 560, 540,
      556, 544, 558, 1582, 556, 544, 558, 1600, 556, 542, 560, 542, 532, 568,
      558, 542, 610, 1538, 504, 1646, 582, 518, 528, 572, 528, 1612, 556, 544,
      528, 580, 554};  // UNKNOWN 28DACDC4
  const uint8_t expected[kMirageStateLength] = {
      0x56, 0x75, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x16, 0x14, 0x26};
  irrecv.setTolerance(kTolerance + 10);  // Bump tolerance to match poor data.
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 243, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::MIRAGE, irsend.capture.decode_type);
  ASSERT_EQ(kMirageBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Model: 1 (KKG9AC1), Power: On, Mode: 2 (Cool), Temp: 25C, "
      "Fan: 0 (Auto), Turbo: Off, Sleep: Off, Light: Off, "
      "Swing(V): 0 (Off), Clock: 14:16",
      IRAcUtils::resultAcToString(&irsend.capture));
}

TEST(TestMirageAcClass, Power) {
  IRMirageAc ac(kGpioUnused);
  ac.begin();

  ac.setModel(mirage_ac_remote_model_t::KKG9AC1);
  ac.on();
  EXPECT_TRUE(ac.getPower());
  ac.on();
  EXPECT_TRUE(ac.getPower());

  ac.off();
  EXPECT_FALSE(ac.getPower());
  ac.off();
  EXPECT_FALSE(ac.getPower());

  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());

  ac.setPower(false);
  EXPECT_FALSE(ac.getPower());

  const uint8_t on[kMirageStateLength] = {
      0x56, 0x75, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x16, 0x14, 0x26};
  ac.setRaw(on);
  EXPECT_TRUE(ac.getPower());
  const uint8_t off[kMirageStateLength] = {
      0x56, 0x6C, 0x00, 0x00, 0x21, 0xD8, 0x00, 0x00,
      0x0C, 0x00, 0x0C, 0x2C, 0x23, 0x01, 0x61};
  ac.setRaw(off);
  EXPECT_FALSE(ac.getPower());

  ac.setModel(mirage_ac_remote_model_t::KKG29AC1);
  ac.on();
  EXPECT_TRUE(ac.getPower());
  ac.off();
  EXPECT_FALSE(ac.getPower());
  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());
  ac.setPower(false);
  EXPECT_FALSE(ac.getPower());
}

TEST(TestMirageAcClass, OperatingMode) {
  IRMirageAc ac(kGpioUnused);
  ac.begin();

  ac.setMode(kMirageAcCool);
  EXPECT_EQ(kMirageAcCool, ac.getMode());
  ac.setMode(kMirageAcHeat);
  EXPECT_EQ(kMirageAcHeat, ac.getMode());
  ac.setMode(kMirageAcDry);
  EXPECT_EQ(kMirageAcDry, ac.getMode());
  ac.setMode(kMirageAcFan);
  EXPECT_EQ(kMirageAcFan, ac.getMode());
  ac.setMode(kMirageAcRecycle);
  EXPECT_EQ(kMirageAcRecycle, ac.getMode());
  ac.setMode(255);
  EXPECT_EQ(kMirageAcCool, ac.getMode());
}

TEST(TestMirageAcClass, HumanReadable) {
  IRMirageAc ac(kGpioUnused);
  ac.begin();

  // Tests for the KKG9AC1 model.
  EXPECT_EQ(
      "Model: 1 (KKG9AC1), Power: On, Mode: 2 (Cool), Temp: 16C, "
      "Fan: 0 (Auto), Turbo: Off, Sleep: Off, Light: Off, "
      "Swing(V): 13 (Auto), Clock: 00:00",
      ac.toString());
  // Ref: https://docs.google.com/spreadsheets/d/1Ucu9mOOIIJoWQjUJq_VCvwgV3EwKaRk8K2AuZgccYEk/edit#gid=0&range=C7
  // 0x56710000201A00000C000C26010041
  const uint8_t cool_21c_auto[kMirageStateLength] = {
      0x56, 0x71, 0x00, 0x00, 0x20, 0x1A, 0x00, 0x00,
      0x0C, 0x00, 0x0C, 0x26, 0x01, 0x00, 0x41};
  ac.setRaw(cool_21c_auto);
  EXPECT_EQ(
      "Model: 1 (KKG9AC1), Power: On, Mode: 2 (Cool), Temp: 21C, "
      "Fan: 0 (Auto), Turbo: Off, Sleep: Off, Light: Off, "
      "Swing(V): 13 (Auto), Clock: 00:01",
      ac.toString());

  const uint8_t SyntheticExample[kMirageStateLength] = {
      0x56, 0x75, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x16, 0x14, 0x26};
  ac.setRaw(SyntheticExample);
  EXPECT_EQ(
      "Model: 1 (KKG9AC1), Power: On, Mode: 2 (Cool), Temp: 25C, "
      "Fan: 0 (Auto), Turbo: Off, Sleep: Off, Light: Off, "
      "Swing(V): 0 (Off), Clock: 14:16",
      ac.toString());

  // Tests for the KKG29AC1 model.
  ac.setModel(mirage_ac_remote_model_t::KKG29AC1);
  EXPECT_EQ(
      "Model: 2 (KKG29AC1), Power: On, Mode: 2 (Cool), Temp: 25C, "
      "Fan: 0 (Auto), Turbo: Off, Sleep: Off, Quiet: Off, Light: -, "
      "Swing(V): Off, Swing(H): Off, "
      "Filter: Off, Clean: -, On Timer: Off, Off Timer: Off, "
      "IFeel: Off",
      ac.toString());
}

TEST(TestMirageAcClass, Temperature) {
  IRMirageAc ac(kGpioUnused);
  ac.begin();

  ac.setTemp(0);
  EXPECT_EQ(kMirageAcMinTemp, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kMirageAcMaxTemp, ac.getTemp());

  ac.setTemp(kMirageAcMinTemp);
  EXPECT_EQ(kMirageAcMinTemp, ac.getTemp());

  ac.setTemp(kMirageAcMaxTemp);
  EXPECT_EQ(kMirageAcMaxTemp, ac.getTemp());

  ac.setTemp(kMirageAcMinTemp - 1);
  EXPECT_EQ(kMirageAcMinTemp, ac.getTemp());

  ac.setTemp(kMirageAcMaxTemp + 1);
  EXPECT_EQ(kMirageAcMaxTemp, ac.getTemp());

  ac.setTemp(17);
  EXPECT_EQ(17, ac.getTemp());

  ac.setTemp(21);
  EXPECT_EQ(21, ac.getTemp());

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());

  ac.setTemp(30);
  EXPECT_EQ(30, ac.getTemp());
}

TEST(TestMirageAcClass, FanSpeed) {
  IRMirageAc ac(kGpioUnused);
  ac.begin();

  ac.setFan(kMirageAcFanAuto);
  EXPECT_EQ(kMirageAcFanAuto, ac.getFan());
  ac.setFan(kMirageAcFanLow);
  EXPECT_EQ(kMirageAcFanLow, ac.getFan());
  ac.setFan(kMirageAcFanMed);
  EXPECT_EQ(kMirageAcFanMed, ac.getFan());
  ac.setFan(kMirageAcFanHigh);
  EXPECT_EQ(kMirageAcFanHigh, ac.getFan());

  ac.setFan(255);
  EXPECT_EQ(kMirageAcFanAuto, ac.getFan());
}

TEST(TestMirageAcClass, Turbo) {
  IRMirageAc ac(kGpioUnused);
  ac.begin();

  ac.setModel(mirage_ac_remote_model_t::KKG9AC1);
  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());
  ac.setTurbo(false);
  EXPECT_FALSE(ac.getTurbo());
  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());

  ac.setModel(mirage_ac_remote_model_t::KKG29AC1);
  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());
  ac.setTurbo(false);
  EXPECT_FALSE(ac.getTurbo());
  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());
}

TEST(TestMirageAcClass, Light) {
  IRMirageAc ac(kGpioUnused);
  ac.begin();

  ac.setModel(mirage_ac_remote_model_t::KKG9AC1);
  ac.setLight(true);
  EXPECT_TRUE(ac.getLight());
  ac.setLight(false);
  EXPECT_FALSE(ac.getLight());
  ac.setLight(true);
  EXPECT_TRUE(ac.getLight());

  ac.setModel(mirage_ac_remote_model_t::KKG29AC1);
  ac.setLight(true);
  EXPECT_TRUE(ac.getLight());
  ac.setLight(false);
  EXPECT_FALSE(ac.getLight());
  ac.setLight(true);
  EXPECT_TRUE(ac.getLight());
}

TEST(TestMirageAcClass, Sleep) {
  IRMirageAc ac(kGpioUnused);
  ac.begin();

  ac.setModel(mirage_ac_remote_model_t::KKG9AC1);
  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
  ac.setSleep(false);
  EXPECT_FALSE(ac.getSleep());
  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());

  ac.setModel(mirage_ac_remote_model_t::KKG29AC1);
  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
  ac.setSleep(false);
  EXPECT_FALSE(ac.getSleep());
  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
}

TEST(TestMirageAcClass, Clock) {
  IRMirageAc ac(kGpioUnused);
  ac.begin();

  ac.setModel(mirage_ac_remote_model_t::KKG9AC1);  // This model supports time.
  ac.setClock(0);
  EXPECT_EQ(0, ac.getClock());
  ac.setClock(12 * 60 * 60 + 30 * 60 + 59);  // aka. 12:30:59
  EXPECT_EQ(12 * 60 * 60 + 30 * 60 + 59, ac.getClock());
  ac.setClock(23 * 60 * 60 + 59 * 60 + 59);  // aka. 23:59:59
  EXPECT_EQ(23 * 60 * 60 + 59 * 60 + 59, ac.getClock());
  ac.setClock(24 * 60 * 60);  // aka. 24:00:00
  EXPECT_EQ(23 * 60 * 60 + 59 * 60 + 59, ac.getClock());  // aka. 23:59:59

  ac.setModel(mirage_ac_remote_model_t::KKG29AC1);  // This model has no clock.
  EXPECT_EQ(0, ac.getClock());
  ac.setClock(12 * 60 * 60 + 30 * 60 + 59);  // aka. 12:30:59
  EXPECT_EQ(0, ac.getClock());
}

TEST(TestMirageAcClass, Checksums) {
  IRMirageAc ac(kGpioUnused);
  ac.begin();

  const uint8_t SyntheticExample[kMirageStateLength] = {
      0x56, 0x75, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x16, 0x14, 0x26};
  EXPECT_TRUE(IRMirageAc::validChecksum(SyntheticExample));
  EXPECT_EQ(0x26, IRMirageAc::calculateChecksum(SyntheticExample));
}

TEST(TestMirageAcClass, SwingV) {
  IRMirageAc ac(kGpioUnused);
  ac.begin();

  // Set the model to one with full swingv support.
  ac.setModel(mirage_ac_remote_model_t::KKG9AC1);

  ac.setSwingV(kMirageAcSwingVAuto);
  EXPECT_EQ(kMirageAcSwingVAuto, ac.getSwingV());

  ac.setSwingV(kMirageAcSwingVHigh);
  EXPECT_EQ(kMirageAcSwingVHigh, ac.getSwingV());

  ac.setSwingV(0xFF);
  EXPECT_EQ(kMirageAcSwingVAuto, ac.getSwingV());

  ac.setSwingV(kMirageAcSwingVLowest);
  EXPECT_EQ(kMirageAcSwingVLowest, ac.getSwingV());

  ac.setSwingV(kMirageAcSwingVLowest - 1);
  EXPECT_EQ(kMirageAcSwingVAuto, ac.getSwingV());

  // Set the model to one with limited swingv support.
  ac.setModel(mirage_ac_remote_model_t::KKG29AC1);

  ac.setSwingV(kMirageAcSwingVAuto);
  EXPECT_EQ(kMirageAcSwingVAuto, ac.getSwingV());
  ac.setSwingV(kMirageAcSwingVOff);
  EXPECT_EQ(kMirageAcSwingVOff, ac.getSwingV());
  ac.setSwingV(kMirageAcSwingVHigh);
  EXPECT_EQ(kMirageAcSwingVAuto, ac.getSwingV());
  ac.setSwingV(0xFF);
  EXPECT_EQ(kMirageAcSwingVAuto, ac.getSwingV());
  ac.setSwingV(kMirageAcSwingVOff);
  EXPECT_EQ(kMirageAcSwingVOff, ac.getSwingV());
}

TEST(TestMirageAcClass, SwingH) {
  IRMirageAc ac(kGpioUnused);
  ac.begin();

  ac.setModel(mirage_ac_remote_model_t::KKG9AC1);
  ac.setSwingH(true);
  EXPECT_FALSE(ac.getSwingH());
  ac.setSwingH(false);
  EXPECT_FALSE(ac.getSwingH());
  ac.setSwingH(true);
  EXPECT_FALSE(ac.getSwingH());

  ac.setModel(mirage_ac_remote_model_t::KKG29AC1);
  ac.setSwingH(true);
  EXPECT_TRUE(ac.getSwingH());
  ac.setSwingH(false);
  EXPECT_FALSE(ac.getSwingH());
  ac.setSwingH(true);
  EXPECT_TRUE(ac.getSwingH());
}

TEST(TestMirageAcClass, Filter) {
  IRMirageAc ac(kGpioUnused);
  ac.begin();

  ac.setModel(mirage_ac_remote_model_t::KKG9AC1);  // No Support
  ac.setFilter(true);
  EXPECT_FALSE(ac.getFilter());
  ac.setFilter(false);
  EXPECT_FALSE(ac.getFilter());
  ac.setFilter(true);
  EXPECT_FALSE(ac.getFilter());

  ac.setModel(mirage_ac_remote_model_t::KKG29AC1);  // Supported
  ac.setFilter(true);
  EXPECT_TRUE(ac.getFilter());
  ac.setFilter(false);
  EXPECT_FALSE(ac.getFilter());
  ac.setFilter(true);
  EXPECT_TRUE(ac.getFilter());
}

TEST(TestMirageAcClass, Quiet) {
  IRMirageAc ac(kGpioUnused);
  ac.begin();

  ac.setModel(mirage_ac_remote_model_t::KKG9AC1);  // No Support
  ac.setQuiet(true);
  EXPECT_FALSE(ac.getQuiet());
  ac.setQuiet(false);
  EXPECT_FALSE(ac.getQuiet());
  ac.setQuiet(true);
  EXPECT_FALSE(ac.getQuiet());

  ac.setModel(mirage_ac_remote_model_t::KKG29AC1);  // Supported
  ac.setQuiet(true);
  EXPECT_TRUE(ac.getQuiet());
  ac.setQuiet(false);
  EXPECT_FALSE(ac.getQuiet());
  ac.setQuiet(true);
  EXPECT_TRUE(ac.getQuiet());
}

TEST(TestMirageAcClass, CleanToggle) {
  IRMirageAc ac(kGpioUnused);
  ac.begin();

  ac.setModel(mirage_ac_remote_model_t::KKG9AC1);
  ac.setCleanToggle(true);
  EXPECT_FALSE(ac.getCleanToggle());
  ac.setCleanToggle(false);
  EXPECT_FALSE(ac.getCleanToggle());
  ac.setCleanToggle(true);
  EXPECT_FALSE(ac.getCleanToggle());

  ac.setModel(mirage_ac_remote_model_t::KKG29AC1);
  ac.setCleanToggle(true);
  EXPECT_TRUE(ac.getCleanToggle());
  ac.setCleanToggle(false);
  EXPECT_FALSE(ac.getCleanToggle());
  ac.setCleanToggle(true);
  EXPECT_TRUE(ac.getCleanToggle());
  ac.send();  // Should be reset when sent.
  EXPECT_FALSE(ac.getCleanToggle());
}

TEST(TestMirageAcClass, Timers) {
  IRMirageAc ac(kGpioUnused);
  ac.begin();

  ac.setModel(mirage_ac_remote_model_t::KKG9AC1);  // No timer support
  EXPECT_EQ(0, ac.getOnTimer());
  EXPECT_EQ(0, ac.getOffTimer());
  ac.setOnTimer(12 * 60 + 37);  // 12:37
  EXPECT_EQ(0, ac.getOnTimer());
  EXPECT_EQ(0, ac.getOffTimer());
  ac.setOffTimer(17 * 60 + 5);  // 17:05
  EXPECT_EQ(0, ac.getOnTimer());
  EXPECT_EQ(0, ac.getOffTimer());

  ac.setModel(mirage_ac_remote_model_t::KKG29AC1);  // Timer supported
  EXPECT_EQ(0, ac.getOnTimer());
  EXPECT_EQ(0, ac.getOffTimer());

  ac.setOnTimer(12 * 60 + 37);  // 12:37
  EXPECT_EQ(12 * 60 + 37, ac.getOnTimer());
  EXPECT_EQ(0, ac.getOffTimer());

  ac.setOffTimer(17 * 60 + 5);  // 17:05
  EXPECT_EQ(17 * 60 + 5, ac.getOffTimer());
  EXPECT_EQ(12 * 60 + 37, ac.getOnTimer());
  ac.setOnTimer(0);  // Off/Disabled
  EXPECT_EQ(0, ac.getOnTimer());
  EXPECT_EQ(17 * 60 + 5, ac.getOffTimer());
  ac.setOffTimer(0);  // Off/Disabled
  EXPECT_EQ(0, ac.getOffTimer());
  EXPECT_EQ(0, ac.getOnTimer());

  ac.setOnTimer(12 * 60 + 37);  // 12:37
  ac.setOffTimer(17 * 60 + 5);  // 17:05
  ac.setModel(mirage_ac_remote_model_t::KKG9AC1);  // No timer support
  EXPECT_EQ(0, ac.getOffTimer());
  EXPECT_EQ(0, ac.getOnTimer());
}

TEST(TestMirageAcClass, IFeelAndSensorTemp) {
  IRMirageAc ac(kGpioUnused);
  ac.begin();

  ac.setModel(mirage_ac_remote_model_t::KKG9AC1);  // No support
  EXPECT_FALSE(ac.getIFeel());
  EXPECT_EQ(0, ac.getSensorTemp());
  ac.setIFeel(true);
  EXPECT_FALSE(ac.getIFeel());
  EXPECT_EQ(0, ac.getSensorTemp());
  ac.setSensorTemp(20);  // 20C
  EXPECT_FALSE(ac.getIFeel());
  EXPECT_EQ(0, ac.getSensorTemp());

  ac.setModel(mirage_ac_remote_model_t::KKG29AC1);  // Supported
  EXPECT_FALSE(ac.getIFeel());
  EXPECT_EQ(0, ac.getSensorTemp());
  ac.setIFeel(true);
  EXPECT_TRUE(ac.getIFeel());
  EXPECT_EQ(0, ac.getSensorTemp());
  ac.setSensorTemp(25);  // 25C
  EXPECT_TRUE(ac.getIFeel());
  EXPECT_EQ(25, ac.getSensorTemp());
  ac.setIFeel(false);
  EXPECT_FALSE(ac.getIFeel());
}

TEST(TestMirageAcClass, getModel) {
  IRMirageAc ac(kGpioUnused);
  ac.begin();
  const uint8_t KKG9AC1[kMirageStateLength] = {
      0x56, 0x6C, 0x00, 0x00, 0x20, 0xD8, 0x00, 0x00,
      0x0C, 0x32, 0x0B, 0x00, 0x32, 0x0F, 0x64};
  EXPECT_EQ(mirage_ac_remote_model_t::KKG9AC1, IRMirageAc::getModel(KKG9AC1));

  // https://github.com/crankyoldgit/IRremoteESP8266/issues/1573#issuecomment-955722044
  const uint8_t KKG29AC1[kMirageStateLength] = {
      0x56, 0x74, 0x00, 0x00, 0x12, 0x00, 0x40, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1D};
  EXPECT_EQ(mirage_ac_remote_model_t::KKG29AC1, IRMirageAc::getModel(KKG29AC1));

  // https://github.com/crankyoldgit/IRremoteESP8266/issues/1573#issuecomment-962362540
  const uint8_t KKG29AC1_2[kMirageStateLength] = {
      0x56, 0x72, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19};
  EXPECT_EQ(mirage_ac_remote_model_t::KKG29AC1,
            IRMirageAc::getModel(KKG29AC1_2));
}
