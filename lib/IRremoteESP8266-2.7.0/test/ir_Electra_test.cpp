// Copyright 2018, 2019 David Conran

#include "ir_Electra.h"
#include <algorithm>
#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendElectraAC().

// Test sending typical data only.
TEST(TestSendElectraAC, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();
  uint8_t data[kElectraAcStateLength] = {0xC3, 0x87, 0xF6, 0x28, 0x60,
                                         0x00, 0x20, 0x00, 0x00, 0x20,
                                         0x00, 0x05, 0x0D};

  irsend.sendElectraAC(data);
  EXPECT_EQ(
      "f38000d50"
      "m9166s4470"
      "m646s1647m646s1647m646s547m646s547m646s547m646s547m646s1647m646s1647"
      "m646s1647m646s1647m646s1647m646s547m646s547m646s547m646s547m646s1647"
      "m646s547m646s1647m646s1647m646s547m646s1647m646s1647m646s1647m646s1647"
      "m646s547m646s547m646s547m646s1647m646s547m646s1647m646s547m646s547"
      "m646s547m646s547m646s547m646s547m646s547m646s1647m646s1647m646s547"
      "m646s547m646s547m646s547m646s547m646s547m646s547m646s547m646s547"
      "m646s547m646s547m646s547m646s547m646s547m646s1647m646s547m646s547"
      "m646s547m646s547m646s547m646s547m646s547m646s547m646s547m646s547"
      "m646s547m646s547m646s547m646s547m646s547m646s547m646s547m646s547"
      "m646s547m646s547m646s547m646s547m646s547m646s1647m646s547m646s547"
      "m646s547m646s547m646s547m646s547m646s547m646s547m646s547m646s547"
      "m646s1647m646s547m646s1647m646s547m646s547m646s547m646s547m646s547"
      "m646s1647m646s547m646s1647m646s1647m646s547m646s547m646s547m646s547"
      "m646s100000",
      irsend.outputStr());
}

// Tests for decodeElectraAC().
// Decode normal ElectraAC messages.

TEST(TestDecodeElectraAC, SyntheticDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Synthesised Normal ElectraAC message.
  irsend.reset();
  uint8_t expectedState[kElectraAcStateLength] = {0xC3, 0x87, 0xF6, 0x28, 0x60,
                                                  0x00, 0x20, 0x00, 0x00, 0x20,
                                                  0x00, 0x05, 0x0D};
  irsend.sendElectraAC(expectedState);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(ELECTRA_AC, irsend.capture.decode_type);
  EXPECT_EQ(kElectraAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
}

// Decode a recorded example
TEST(TestDecodeElectraAC, RealExampleDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Real ElectraAC message.
  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/527
  uint16_t rawData[211] = {
      9166, 4470, 642, 1632, 642, 1632, 668, 534,  666, 534,  668, 534,
      614,  536,  640, 1636, 640, 1646, 694, 1662, 612, 1628, 642, 1666,
      664,  532,  668, 534,  666, 534,  666, 532,  666, 1644, 642, 532,
      640,  1634, 668, 1632, 642, 538,  666, 1660, 610, 1666, 664, 1632,
      642,  1672, 610, 536,  666, 534,  694, 532,  666, 1636, 614, 538,
      666,  1632, 642, 536,  666, 544,  692, 534,  640, 558,  640, 534,
      640,  540,  666, 534,  638, 1666, 638, 1636, 640, 550,  666, 534,
      640,  540,  666, 534,  640, 540,  666, 536,  638, 540,  666, 536,
      638,  550,  664, 536,  638, 540,  664, 536,  638, 540,  666, 534,
      638,  1640, 664, 536,  692, 546,  664, 536,  664, 536,  664, 536,
      664,  546,  612, 532,  636, 538,  664, 536,  664, 546,  612, 538,
      638,  538,  638, 538,  664, 536,  690, 538,  662, 538,  664, 538,
      662,  548,  664, 536,  662, 538,  662, 562,  638, 564,  636, 564,
      636,  1668, 582, 556,  652, 572,  612, 568,  636, 564,  610, 570,
      636,  556,  616, 550,  656, 566,  610, 570,  632, 578,  608, 1640,
      662,  562,  642, 1686, 582, 570,  634, 566,  604, 576,  636, 566,
      610,  578,  634, 1664, 584, 590,  660, 1636, 610, 1642, 664, 590,
      610,  590,  636, 566,  634, 568,  686};  // UNKNOWN 9AD8CDB5
  uint8_t expectedState[kElectraAcStateLength] = {0xC3, 0x87, 0xF6, 0x28, 0x60,
                                                  0x00, 0x20, 0x00, 0x00, 0x20,
                                                  0x00, 0x05, 0x0D};

  irsend.reset();
  irsend.sendRaw(rawData, 211, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(ELECTRA_AC, irsend.capture.decode_type);
  ASSERT_EQ(kElectraAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 24C, Fan: 3 (Low), "
      "Swing(V): Off, Swing(H): Off",
      IRAcUtils::resultAcToString(&irsend.capture));
}

TEST(TestIRElectraAcClass, Power) {
  IRElectraAc ac(0);
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

TEST(TestIRElectraAcClass, OperatingMode) {
  IRElectraAc ac(0);
  ac.begin();

  ac.setMode(kElectraAcAuto);
  EXPECT_EQ(kElectraAcAuto, ac.getMode());

  ac.setMode(kElectraAcCool);
  EXPECT_EQ(kElectraAcCool, ac.getMode());

  ac.setMode(kElectraAcHeat);
  EXPECT_EQ(kElectraAcHeat, ac.getMode());

  ac.setMode(kElectraAcDry);
  EXPECT_EQ(kElectraAcDry, ac.getMode());

  ac.setMode(kElectraAcFan);
  EXPECT_EQ(kElectraAcFan, ac.getMode());

  ac.setMode(kElectraAcHeat + 1);
  EXPECT_EQ(kElectraAcAuto, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kElectraAcAuto, ac.getMode());
}

TEST(TestIRElectraAcClass, SetAndGetTemp) {
  IRElectraAc ac(0);
  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());
  ac.setTemp(kElectraAcMinTemp);
  EXPECT_EQ(kElectraAcMinTemp, ac.getTemp());
  ac.setTemp(kElectraAcMinTemp - 1);
  EXPECT_EQ(kElectraAcMinTemp, ac.getTemp());
  ac.setTemp(kElectraAcMaxTemp);
  EXPECT_EQ(kElectraAcMaxTemp, ac.getTemp());
  ac.setTemp(kElectraAcMaxTemp + 1);
  EXPECT_EQ(kElectraAcMaxTemp, ac.getTemp());
}

TEST(TestIRElectraAcClass, FanSpeed) {
  IRElectraAc ac(0);
  ac.begin();

  ac.setFan(0);
  EXPECT_EQ(kElectraAcFanAuto, ac.getFan());

  ac.setFan(255);
  EXPECT_EQ(kElectraAcFanAuto, ac.getFan());

  ac.setFan(kElectraAcFanHigh);
  EXPECT_EQ(kElectraAcFanHigh, ac.getFan());

  ac.setFan(std::max(kElectraAcFanHigh, kElectraAcFanLow) + 1);
  EXPECT_EQ(kElectraAcFanAuto, ac.getFan());

  ac.setFan(kElectraAcFanHigh - 1);
  EXPECT_EQ(kElectraAcFanAuto, ac.getFan());

  ac.setFan(1);
  EXPECT_EQ(1, ac.getFan());

  ac.setFan(1);
  EXPECT_EQ(1, ac.getFan());

  ac.setFan(3);
  EXPECT_EQ(3, ac.getFan());
}

TEST(TestIRElectraAcClass, toCommon) {
  IRElectraAc ac(0);
  ac.setPower(true);
  ac.setMode(kElectraAcCool);
  ac.setTemp(20);
  ac.setFan(kElectraAcFanHigh);
  ac.setSwingV(true);
  ac.setSwingH(true);
  // Now test it.
  ASSERT_EQ(decode_type_t::ELECTRA_AC, ac.toCommon().protocol);
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
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

TEST(TestIRElectraAcClass, HumanReadable) {
  IRElectraAc ac(0);
  // Data from:
  //   https://github.com/crankyoldgit/IRremoteESP8266/issues/778#issue-460052080
  uint8_t on_cool_32C_auto_voff[13] = {
      0xC3, 0xC7, 0xE0, 0x00, 0xA0, 0x00, 0x20,
      0x00, 0x00, 0x20, 0x00, 0x40, 0x8A};
  ac.setRaw(on_cool_32C_auto_voff);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 32C, Fan: 5 (Auto), "
      "Swing(V): Off, Swing(H): Off", ac.toString());
  uint8_t on_cool_16C_auto_voff[13] = {
      0xC3, 0x47, 0xE0, 0x00, 0xA0, 0x00, 0x20,
      0x00, 0x00, 0x20, 0x00, 0x41, 0x0B};
  ac.setRaw(on_cool_16C_auto_voff);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 16C, Fan: 5 (Auto), "
      "Swing(V): Off, Swing(H): Off", ac.toString());
  uint8_t on_cool_16C_low_voff[13] = {
      0xC3, 0x47, 0xE0, 0x00, 0x60, 0x00, 0x20,
      0x00, 0x00, 0x20, 0x00, 0x41, 0xCB};
  ac.setRaw(on_cool_16C_low_voff);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 16C, Fan: 3 (Low), "
      "Swing(V): Off, Swing(H): Off", ac.toString());
}
