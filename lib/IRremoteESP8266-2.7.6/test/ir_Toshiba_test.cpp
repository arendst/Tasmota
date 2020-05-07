// Copyright 2017 David Conran
#include "ir_Toshiba.h"
#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for Toshiba A/C methods.

// Test sending typical data only.
TEST(TestSendToshibaAC, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  uint8_t toshiba_code[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                                 0x00, 0x00, 0x00, 0x00};
  irsend.reset();
  irsend.sendToshibaAC(toshiba_code);
  EXPECT_EQ(
      "f38000d50"
      "m4400s4300"
      "m543s1623m543s1623m543s1623m543s1623m543s472m543s472m543s1623m543s472"
      "m543s472m543s472m543s472m543s472m543s1623m543s1623m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s1623m543s1623"
      "m543s1623m543s1623m543s1623m543s1623m543s1623m543s1623m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s7048"
      "m4400s4300"
      "m543s1623m543s1623m543s1623m543s1623m543s472m543s472m543s1623m543s472"
      "m543s472m543s472m543s472m543s472m543s1623m543s1623m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s1623m543s1623"
      "m543s1623m543s1623m543s1623m543s1623m543s1623m543s1623m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s7048",
      irsend.outputStr());
}

// Test sending with repeats.
TEST(TestSendToshibaAC, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  uint8_t toshiba_code[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                                 0x00, 0x00, 0x00, 0x00};

  irsend.sendToshibaAC(toshiba_code, kToshibaACStateLength, 0);
  EXPECT_EQ(
      "f38000d50"
      "m4400s4300"
      "m543s1623m543s1623m543s1623m543s1623m543s472m543s472m543s1623m543s472"
      "m543s472m543s472m543s472m543s472m543s1623m543s1623m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s1623m543s1623"
      "m543s1623m543s1623m543s1623m543s1623m543s1623m543s1623m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s7048",
      irsend.outputStr());

  irsend.reset();
  irsend.sendToshibaAC(toshiba_code, kToshibaACStateLength, 2);
  EXPECT_EQ(
      "f38000d50"
      "m4400s4300"
      "m543s1623m543s1623m543s1623m543s1623m543s472m543s472m543s1623m543s472"
      "m543s472m543s472m543s472m543s472m543s1623m543s1623m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s1623m543s1623"
      "m543s1623m543s1623m543s1623m543s1623m543s1623m543s1623m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s7048"
      "m4400s4300"
      "m543s1623m543s1623m543s1623m543s1623m543s472m543s472m543s1623m543s472"
      "m543s472m543s472m543s472m543s472m543s1623m543s1623m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s1623m543s1623"
      "m543s1623m543s1623m543s1623m543s1623m543s1623m543s1623m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s7048"
      "m4400s4300"
      "m543s1623m543s1623m543s1623m543s1623m543s472m543s472m543s1623m543s472"
      "m543s472m543s472m543s472m543s472m543s1623m543s1623m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s1623m543s1623"
      "m543s1623m543s1623m543s1623m543s1623m543s1623m543s1623m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s7048",
      irsend.outputStr());
}

// Test sending atypical sizes.
TEST(TestSendToshibaAC, SendUnexpectedSizes) {
  IRsendTest irsend(4);
  irsend.begin();

  uint8_t toshiba_short_code[8] = {0x01, 0x02, 0x03, 0x04,
                                   0x05, 0x06, 0x07, 0x08};
  uint8_t toshiba_long_code[10] = {0x01, 0x02, 0x03, 0x04, 0x05,
                                   0x06, 0x07, 0x08, 0x09, 0x0A};
  irsend.reset();
  irsend.sendToshibaAC(toshiba_short_code, kToshibaACStateLength - 1);
  ASSERT_EQ("", irsend.outputStr());

  irsend.reset();
  irsend.sendToshibaAC(toshiba_long_code, kToshibaACStateLength + 1);
  ASSERT_EQ(
      "f38000d50"
      "m4400s4300"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s1623m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s1623m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s1623m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s1623m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s1623m543s1623m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s1623m543s1623m543s1623"
      "m543s472m543s472m543s472m543s472m543s1623m543s472m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s1623m543s472m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s1623m543s472m543s1623m543s472"
      "m543s7048"
      "m4400s4300"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s1623m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s1623m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s1623m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s1623m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s1623m543s1623m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s1623m543s1623m543s1623"
      "m543s472m543s472m543s472m543s472m543s1623m543s472m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s1623m543s472m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s1623m543s472m543s1623m543s472"
      "m543s7048",
      irsend.outputStr());
}

// Tests for IRToshibaAC class.

TEST(TestToshibaACClass, Power) {
  IRToshibaAC toshiba(0);
  toshiba.begin();

  toshiba.on();
  EXPECT_TRUE(toshiba.getPower());

  toshiba.off();
  EXPECT_FALSE(toshiba.getPower());

  toshiba.setPower(true);
  EXPECT_TRUE(toshiba.getPower());

  toshiba.setPower(false);
  EXPECT_FALSE(toshiba.getPower());
}

TEST(TestToshibaACClass, Temperature) {
  IRToshibaAC toshiba(0);
  toshiba.begin();

  toshiba.setTemp(0);
  EXPECT_EQ(kToshibaAcMinTemp, toshiba.getTemp());

  toshiba.setTemp(255);
  EXPECT_EQ(kToshibaAcMaxTemp, toshiba.getTemp());

  toshiba.setTemp(kToshibaAcMinTemp);
  EXPECT_EQ(kToshibaAcMinTemp, toshiba.getTemp());

  toshiba.setTemp(kToshibaAcMaxTemp);
  EXPECT_EQ(kToshibaAcMaxTemp, toshiba.getTemp());

  toshiba.setTemp(kToshibaAcMinTemp - 1);
  EXPECT_EQ(kToshibaAcMinTemp, toshiba.getTemp());

  toshiba.setTemp(kToshibaAcMaxTemp + 1);
  EXPECT_EQ(kToshibaAcMaxTemp, toshiba.getTemp());

  toshiba.setTemp(17);
  EXPECT_EQ(17, toshiba.getTemp());

  toshiba.setTemp(21);
  EXPECT_EQ(21, toshiba.getTemp());

  toshiba.setTemp(25);
  EXPECT_EQ(25, toshiba.getTemp());

  toshiba.setTemp(30);
  EXPECT_EQ(30, toshiba.getTemp());
}

TEST(TestToshibaACClass, OperatingMode) {
  IRToshibaAC toshiba(0);
  toshiba.begin();

  toshiba.setMode(kToshibaAcAuto);
  EXPECT_EQ(kToshibaAcAuto, toshiba.getMode());

  toshiba.setMode(kToshibaAcCool);
  EXPECT_EQ(kToshibaAcCool, toshiba.getMode());

  toshiba.setMode(kToshibaAcHeat);
  EXPECT_EQ(kToshibaAcHeat, toshiba.getMode());

  toshiba.setMode(kToshibaAcDry);
  EXPECT_EQ(kToshibaAcDry, toshiba.getMode());

  toshiba.setMode(kToshibaAcHeat + 1);
  EXPECT_EQ(kToshibaAcAuto, toshiba.getMode());

  toshiba.setMode(255);
  EXPECT_EQ(kToshibaAcAuto, toshiba.getMode());

  // Setting the power off changes the underlying mode in the state to heat.
  toshiba.setPower(true);
  toshiba.setMode(kToshibaAcCool);
  EXPECT_EQ(kToshibaAcCool, toshiba.getMode());
  EXPECT_EQ(kToshibaAcCool, toshiba.getMode(true));
  toshiba.setPower(false);
  EXPECT_EQ(kToshibaAcCool, toshiba.getMode());
  EXPECT_EQ(kToshibaAcHeat, toshiba.getMode(true));
}

TEST(TestToshibaACClass, FanSpeed) {
  IRToshibaAC toshiba(0);
  toshiba.begin();

  toshiba.setFan(kToshibaAcFanAuto);
  EXPECT_EQ(kToshibaAcFanAuto, toshiba.getFan());

  toshiba.setFan(255);
  EXPECT_EQ(kToshibaAcFanMax, toshiba.getFan());

  toshiba.setFan(kToshibaAcFanMax);
  EXPECT_EQ(kToshibaAcFanMax, toshiba.getFan());

  toshiba.setFan(kToshibaAcFanMax - 1);
  EXPECT_EQ(kToshibaAcFanMax - 1, toshiba.getFan());

  toshiba.setFan(1);
  EXPECT_EQ(1, toshiba.getFan());

  toshiba.setFan(2);
  EXPECT_EQ(2, toshiba.getFan());

  toshiba.setFan(3);
  EXPECT_EQ(3, toshiba.getFan());

  toshiba.setFan(4);
  EXPECT_EQ(4, toshiba.getFan());

  toshiba.setFan(kToshibaAcFanMax + 1);
  EXPECT_EQ(kToshibaAcFanMax, toshiba.getFan());
}

TEST(TestToshibaACClass, RawState) {
  IRToshibaAC toshiba(0);
  toshiba.begin();

  uint8_t initial_state[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                                  0x00, 0x00, 0x00, 0x01};
  uint8_t modified_state[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                                   0x00, 0xC1, 0x00, 0xC0};

  // Verify the starting state.
  EXPECT_STATE_EQ(initial_state, toshiba.getRaw(), kToshibaACBits);
  EXPECT_TRUE(toshiba.getPower());
  EXPECT_EQ(kToshibaAcAuto, toshiba.getMode());
  EXPECT_EQ(kToshibaAcFanAuto, toshiba.getFan());

  // Change some settings.
  toshiba.setMode(kToshibaAcCool);
  toshiba.setFan(kToshibaAcFanMax);
  toshiba.setTemp(kToshibaAcMinTemp);
  // Verify those were set.
  EXPECT_EQ(kToshibaAcCool, toshiba.getMode());
  EXPECT_EQ(kToshibaAcFanMax, toshiba.getFan());
  EXPECT_EQ(kToshibaAcMinTemp, toshiba.getTemp());
  // Retrieve the modified state.
  EXPECT_STATE_EQ(modified_state, toshiba.getRaw(), kToshibaACBits);

  // Set it back to the initial state.
  toshiba.setRaw(initial_state);

  // Check the new state was set correctly.
  EXPECT_TRUE(toshiba.getPower());
  EXPECT_EQ(kToshibaAcAuto, toshiba.getMode());
  EXPECT_EQ(kToshibaAcFanAuto, toshiba.getFan());
  EXPECT_STATE_EQ(initial_state, toshiba.getRaw(), kToshibaACBits);
}

TEST(TestToshibaACClass, Checksums) {
  IRToshibaAC toshiba(0);
  toshiba.begin();

  uint8_t initial_state[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                                  0x00, 0x00, 0x00, 0x01};
  uint8_t modified_state[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                                   0x00, 0xC1, 0x00, 0xC0};
  uint8_t invalid_state[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                                  0x00, 0x00, 0x00, 0x00};

  EXPECT_EQ(0x01, toshiba.calcChecksum(initial_state));
  EXPECT_EQ(0xC0, toshiba.calcChecksum(modified_state));
  // Check we can call it without instantiating the object.
  EXPECT_EQ(0x01, IRToshibaAC::calcChecksum(initial_state));
  // Use different lengths.
  EXPECT_EQ(0x01, IRToshibaAC::calcChecksum(initial_state,
                                            kToshibaACStateLength - 1));
  EXPECT_EQ(0xFF, IRToshibaAC::calcChecksum(initial_state, 3));
  // Minimum length that actually means anything.
  EXPECT_EQ(0xF2, IRToshibaAC::calcChecksum(initial_state, 2));
  // Technically, there is no such thing as a checksum for a length of < 2
  // But test it anyway
  EXPECT_EQ(0x00, IRToshibaAC::calcChecksum(initial_state, 1));
  EXPECT_EQ(0x00, IRToshibaAC::calcChecksum(initial_state, 0));

  // Validity tests.
  EXPECT_TRUE(IRToshibaAC::validChecksum(initial_state));
  EXPECT_TRUE(IRToshibaAC::validChecksum(modified_state));
  EXPECT_FALSE(IRToshibaAC::validChecksum(invalid_state));
  EXPECT_FALSE(IRToshibaAC::validChecksum(initial_state, 0));
  EXPECT_FALSE(IRToshibaAC::validChecksum(initial_state, 1));
  EXPECT_FALSE(IRToshibaAC::validChecksum(initial_state, 2));
}

TEST(TestToshibaACClass, HumanReadableOutput) {
  IRToshibaAC toshiba(0);
  toshiba.begin();

  uint8_t initial_state[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                                  0x00, 0x00, 0x00, 0x01};
  uint8_t modified_state[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                                   0x00, 0xC1, 0x00, 0xC0};

  toshiba.setRaw(initial_state);
  EXPECT_EQ("Power: On, Mode: 0 (Auto), Temp: 17C, Fan: 0 (Auto)",
            toshiba.toString());
  toshiba.setRaw(modified_state);
  EXPECT_EQ("Power: On, Mode: 1 (Cool), Temp: 17C, Fan: 5 (High)",
            toshiba.toString());
  toshiba.off();
  toshiba.setTemp(25);
  toshiba.setFan(3);
  toshiba.setMode(kToshibaAcDry);
  EXPECT_EQ("Power: Off, Mode: 2 (Dry), Temp: 25C, Fan: 3 (Medium)",
            toshiba.toString());
}

TEST(TestToshibaACClass, MessageConstuction) {
  IRToshibaAC toshiba(0);
  IRsendTest irsend(4);
  toshiba.begin();
  irsend.begin();

  toshiba.setFan(1);
  toshiba.setMode(kToshibaAcCool);
  toshiba.setTemp(27);
  toshiba.on();

  // Check everything for kicks.
  EXPECT_EQ(1, toshiba.getFan());
  EXPECT_EQ(kToshibaAcCool, toshiba.getMode());
  EXPECT_EQ(27, toshiba.getTemp());
  EXPECT_TRUE(toshiba.getPower());

  irsend.reset();
  irsend.sendToshibaAC(toshiba.getRaw());
  EXPECT_EQ(
      "f38000d50"
      "m4400s4300"
      "m543s1623m543s1623m543s1623m543s1623m543s472m543s472m543s1623m543s472"
      "m543s472m543s472m543s472m543s472m543s1623m543s1623m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s1623m543s1623"
      "m543s1623m543s1623m543s1623m543s1623m543s1623m543s1623m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s1623"
      "m543s1623m543s472m543s1623m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s1623m543s472m543s472m543s472m543s472m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s1623m543s1623m543s1623m543s472m543s472m543s472m543s472m543s472"
      "m543s7048"
      "m4400s4300"
      "m543s1623m543s1623m543s1623m543s1623m543s472m543s472m543s1623m543s472"
      "m543s472m543s472m543s472m543s472m543s1623m543s1623m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s1623m543s1623"
      "m543s1623m543s1623m543s1623m543s1623m543s1623m543s1623m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s1623"
      "m543s1623m543s472m543s1623m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s1623m543s472m543s472m543s472m543s472m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s1623m543s1623m543s1623m543s472m543s472m543s472m543s472m543s472"
      "m543s7048",
      irsend.outputStr());

  // Turn off the power and re-check.
  toshiba.setPower(false);
  // Check everything for kicks.
  EXPECT_EQ(1, toshiba.getFan());
  EXPECT_EQ(kToshibaAcCool, toshiba.getMode());
  EXPECT_EQ(27, toshiba.getTemp());
  EXPECT_FALSE(toshiba.getPower());

  irsend.reset();
  irsend.sendToshibaAC(toshiba.getRaw());
  EXPECT_EQ(
      "f38000d50"
      "m4400s4300"
      "m543s1623m543s1623m543s1623m543s1623m543s472m543s472m543s1623m543s472"
      "m543s472m543s472m543s472m543s472m543s1623m543s1623m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s1623m543s1623"
      "m543s1623m543s1623m543s1623m543s1623m543s1623m543s1623m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s1623"
      "m543s1623m543s472m543s1623m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s1623m543s472m543s472m543s472m543s1623m543s1623m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s1623m543s1623m543s1623m543s472m543s472m543s1623m543s1623m543s472"
      "m543s7048"
      "m4400s4300"
      "m543s1623m543s1623m543s1623m543s1623m543s472m543s472m543s1623m543s472"
      "m543s472m543s472m543s472m543s472m543s1623m543s1623m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s1623m543s1623"
      "m543s1623m543s1623m543s1623m543s1623m543s1623m543s1623m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s1623"
      "m543s1623m543s472m543s1623m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s1623m543s472m543s472m543s472m543s1623m543s1623m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s1623m543s1623m543s1623m543s472m543s472m543s1623m543s1623m543s472"
      "m543s7048",
      irsend.outputStr());

  // Turn the power back on, and check nothing changed.
  toshiba.on();

  EXPECT_EQ(1, toshiba.getFan());
  EXPECT_EQ(kToshibaAcCool, toshiba.getMode());
  EXPECT_EQ(27, toshiba.getTemp());
  EXPECT_TRUE(toshiba.getPower());

  irsend.reset();
  irsend.sendToshibaAC(toshiba.getRaw());
  EXPECT_EQ(
      "f38000d50"
      "m4400s4300"
      "m543s1623m543s1623m543s1623m543s1623m543s472m543s472m543s1623m543s472"
      "m543s472m543s472m543s472m543s472m543s1623m543s1623m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s1623m543s1623"
      "m543s1623m543s1623m543s1623m543s1623m543s1623m543s1623m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s1623"
      "m543s1623m543s472m543s1623m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s1623m543s472m543s472m543s472m543s472m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s1623m543s1623m543s1623m543s472m543s472m543s472m543s472m543s472"
      "m543s7048"
      "m4400s4300"
      "m543s1623m543s1623m543s1623m543s1623m543s472m543s472m543s1623m543s472"
      "m543s472m543s472m543s472m543s472m543s1623m543s1623m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s1623m543s1623"
      "m543s1623m543s1623m543s1623m543s1623m543s1623m543s1623m543s472m543s472"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s1623"
      "m543s1623m543s472m543s1623m543s472m543s472m543s472m543s472m543s472"
      "m543s472m543s1623m543s472m543s472m543s472m543s472m543s472m543s1623"
      "m543s472m543s472m543s472m543s472m543s472m543s472m543s472m543s472"
      "m543s1623m543s1623m543s1623m543s472m543s472m543s472m543s472m543s472"
      "m543s7048",
      irsend.outputStr());
}

// Decoding a message we entirely constructed based solely on a given state.
TEST(TestDecodeToshibaAC, SyntheticExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  uint8_t expectedState[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                                  0x00, 0x00, 0x00, 0x01};

  irsend.reset();
  irsend.sendToshibaAC(expectedState);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(TOSHIBA_AC, irsend.capture.decode_type);
  ASSERT_EQ(kToshibaACBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Power: On, Mode: 0 (Auto), Temp: 17C, Fan: 0 (Auto)",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

// Test decoding against captures from a real Toshiba A/C remote.
// Recorded by @mwildbolz
TEST(TestDecodeToshibaAC, RealExamples) {
  IRToshibaAC toshiba(0);
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  uint16_t rawData1[295] = {
      4442, 4292, 612, 1544, 616,  1544, 616, 1544, 612, 1548, 610, 468,
      612,  468,  662, 1494, 640,  438,  616, 464,  614, 464,  616, 464,
      612,  468,  610, 1544, 616,  1544, 616, 468,  612, 1544, 616, 464,
      694,  386,  616, 464,  612,  468,  612, 468,  636, 444,  610, 1546,
      616,  1544, 612, 1546, 614,  1546, 616, 1546, 740, 1420, 612, 1544,
      616,  1546, 616, 464,  610,  468,  610, 470,  612, 468,  610, 468,
      610,  470,  636, 438,  616,  464,  616, 464,  616, 1546, 636, 442,
      612,  1546, 614, 1544, 616,  464,  614, 464,  610, 468,  612, 468,
      612,  468,  612, 468,  636,  440,  614, 464,  616, 464,  616, 464,
      612,  468,  636, 442,  638,  442,  662, 418,  610, 464,  616, 464,
      616,  464,  610, 468,  612,  468,  636, 444,  610, 468,  638, 438,
      614,  1546, 612, 1548, 612,  470,  610, 468,  636, 442,  612, 468,
      612,  1544, 612, 7396, 4442, 4292, 610, 1546, 616, 1544, 612, 1548,
      612,  1546, 616, 464,  616,  464,  616, 1544, 612, 468,  662, 418,
      610,  468,  638, 442,  638,  438,  616, 1546, 616, 1544, 612, 468,
      610,  1546, 616, 464,  616,  464,  642, 438,  616, 464,  612, 468,
      610,  470,  610, 1546, 616,  1544, 612, 1546, 616, 1546, 614, 1546,
      612,  1550, 610, 1544, 616,  1546, 614, 464,  642, 438,  610, 468,
      612,  468,  612, 468,  612,  468,  610, 468,  638, 438,  614, 464,
      616,  1544, 636, 444,  636,  1520, 616, 1544, 616, 464,  616, 464,
      612,  468,  612, 468,  612,  468,  612, 468,  612, 464,  612, 470,
      636,  442,  638, 442,  612,  470,  692, 384,  614, 464,  616, 464,
      612,  468,  610, 468,  612,  468,  610, 470,  610, 464,  616, 464,
      616,  464,  616, 464,  610,  1550, 610, 1546, 640, 444,  688, 386,
      616,  464,  612, 468,  612,  1544, 642};

  irsend.reset();
  irsend.sendRaw(rawData1, 295, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(TOSHIBA_AC, irsend.capture.decode_type);
  ASSERT_EQ(kToshibaACBits, irsend.capture.bits);
  toshiba.setRaw(irsend.capture.state);
  EXPECT_TRUE(toshiba.getPower());
  EXPECT_EQ(23, toshiba.getTemp());
  EXPECT_EQ(kToshibaAcFanAuto, toshiba.getFan());
  EXPECT_EQ(kToshibaAcAuto, toshiba.getMode());

  uint16_t rawData2[295] = {
      4500, 4236, 636, 1520, 642,  1520, 640, 1520, 664, 1492, 642, 440,
      668,  412,  642, 1518, 638,  438,  666, 414,  640, 438,  642, 438,
      638,  442,  642, 1516, 640,  1520, 642, 438,  642, 1520, 636, 438,
      668,  412,  640, 440,  666,  412,  642, 438,  668, 412,  640, 1516,
      668,  1492, 642, 1520, 666,  1494, 638, 1520, 642, 1520, 668, 1490,
      666,  1494, 642, 438,  638,  438,  668, 412,  668, 412,  642, 438,
      642,  438,  664, 412,  642,  438,  642, 438,  642, 1518, 642, 434,
      668,  412,  642, 438,  668,  412,  692, 388,  666, 412,  642, 434,
      642,  438,  642, 1518, 668,  412,  668, 412,  640, 438,  638, 438,
      642,  438,  640, 438,  668,  1492, 642, 440,  666, 412,  640, 438,
      642,  438,  642, 434,  668,  412,  668, 412,  666, 414,  666, 1494,
      640,  438,  642, 434,  668,  412,  642, 438,  642, 438,  668, 412,
      668,  414,  640, 7362, 4474, 4262, 642, 1518, 638, 1520, 640, 1520,
      668,  1494, 640, 434,  642,  438,  640, 1520, 642, 438,  642, 438,
      642,  438,  642, 438,  642,  434,  668, 1494, 642, 1518, 638, 442,
      638,  1520, 642, 438,  642,  438,  668, 414,  664, 408,  642, 438,
      668,  412,  642, 1520, 666,  1494, 642, 1514, 642, 1518, 642, 1520,
      636,  1520, 668, 1494, 666,  1494, 638, 438,  666, 414,  640, 440,
      666,  412,  668, 412,  668,  412,  642, 434,  668, 412,  668, 412,
      668,  1494, 642, 438,  642,  434,  642, 438,  642, 438,  642, 438,
      642,  438,  642, 434,  646,  434,  642, 1518, 668, 412,  642, 438,
      642,  434,  666, 414,  640,  438,  642, 438,  642, 1518, 642, 438,
      642,  434,  668, 412,  642,  438,  642, 438,  642, 438,  642, 438,
      642,  438,  640, 1520, 636,  438,  642, 438,  642, 438,  666, 414,
      668,  412,  642, 440,  640,  438,  640};

  irsend.reset();
  irsend.sendRaw(rawData2, 295, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(TOSHIBA_AC, irsend.capture.decode_type);
  ASSERT_EQ(kToshibaACBits, irsend.capture.bits);
  toshiba.setRaw(irsend.capture.state);
  EXPECT_TRUE(toshiba.getPower());
  EXPECT_EQ(17, toshiba.getTemp());
  EXPECT_EQ(3, toshiba.getFan());
  EXPECT_EQ(kToshibaAcCool, toshiba.getMode());

  uint16_t rawData3[295] = {
      4474, 4262, 642, 1514, 642,  1520, 642, 1520, 642, 1514, 642, 438,
      642,  438,  642, 1520, 636,  438,  642, 438,  642, 438,  642, 438,
      642,  438,  640, 1520, 638,  1520, 642, 438,  640, 1520, 642, 438,
      642,  434,  642, 438,  642,  438,  642, 438,  668, 414,  636, 1520,
      642,  1520, 642, 1514, 642,  1520, 642, 1520, 640, 1518, 638, 1520,
      666,  1494, 642, 438,  642,  434,  642, 438,  640, 438,  642, 438,
      642,  440,  640, 434,  642,  438,  642, 438,  642, 1520, 642, 438,
      642,  1514, 642, 1520, 640,  1520, 636, 438,  642, 438,  642, 438,
      666,  414,  642, 1520, 636,  1520, 642, 438,  642, 438,  640, 438,
      642,  434,  642, 1518, 642,  1520, 642, 438,  642, 434,  640, 438,
      642,  438,  642, 438,  642,  440,  642, 438,  668, 408,  642, 1520,
      642,  438,  642, 1520, 638,  1518, 642, 438,  642, 438,  640, 1520,
      640,  438,  642, 7362, 4474, 4262, 642, 1518, 638, 1520, 640, 1520,
      642,  1520, 638, 438,  642,  438,  642, 1518, 642, 438,  638, 438,
      642,  438,  642, 438,  642,  438,  642, 1514, 642, 1520, 642, 438,
      666,  1490, 642, 438,  642,  438,  642, 440,  640, 438,  642, 434,
      640,  438,  642, 1520, 642,  1520, 636, 1520, 642, 1520, 642, 1514,
      642,  1518, 642, 1518, 640,  1516, 642, 438,  642, 438,  642, 438,
      640,  438,  638, 442,  642,  434,  642, 440,  640, 438,  642, 438,
      642,  1518, 642, 438,  642,  1514, 642, 1520, 642, 1518, 642, 438,
      642,  432,  642, 438,  642,  438,  642, 1520, 642, 1514, 642, 438,
      642,  438,  642, 438,  642,  438,  642, 1514, 642, 1520, 642, 438,
      642,  438,  638, 438,  642,  438,  642, 438,  640, 440,  642, 438,
      640,  434,  642, 1520, 642,  438,  640, 1520, 668, 1490, 666, 414,
      640,  438,  642, 1520, 642,  438,  636};

  irsend.reset();
  irsend.sendRaw(rawData3, 295, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(TOSHIBA_AC, irsend.capture.decode_type);
  ASSERT_EQ(kToshibaACBits, irsend.capture.bits);
  toshiba.setRaw(irsend.capture.state);
  EXPECT_TRUE(toshiba.getPower());
  EXPECT_EQ(24, toshiba.getTemp());
  EXPECT_EQ(kToshibaAcFanMax, toshiba.getFan());
  EXPECT_EQ(kToshibaAcHeat, toshiba.getMode());

  uint16_t rawData4[295] = {
      4474, 4262, 636, 1520, 640,  1520, 640, 1520, 638, 1518, 642, 438,
      642,  438,  642, 1520, 636,  438,  642, 438,  642, 438,  642, 438,
      636,  444,  636, 1520, 640,  1520, 642, 438,  638, 1524, 638, 438,
      640,  438,  642, 438,  640,  438,  642, 438,  638, 438,  642, 1518,
      642,  1520, 666, 1494, 636,  1520, 640, 1520, 640, 1520, 636, 1524,
      638,  1520, 640, 440,  640,  438,  642, 438,  636, 444,  636, 438,
      642,  438,  640, 440,  640,  438,  642, 438,  642, 1518, 638, 438,
      642,  1518, 642, 438,  640,  1520, 636, 444,  636, 438,  640, 438,
      642,  438,  668, 1494, 640,  438,  642, 1518, 636, 444,  636, 438,
      640,  1520, 642, 1518, 642,  1520, 636, 444,  636, 438,  642, 438,
      642,  438,  640, 440,  640,  438,  640, 440,  640, 438,  640, 1518,
      642,  1520, 636, 1524, 636,  1518, 642, 438,  642, 1518, 642, 1518,
      640,  438,  642, 7364, 4472, 4262, 642, 1518, 642, 1518, 638, 1518,
      642,  1520, 642, 438,  642,  438,  640, 1520, 636, 440,  640, 438,
      642,  438,  640, 438,  642,  438,  642, 1518, 636, 1524, 636, 438,
      640,  1520, 642, 438,  642,  438,  640, 438,  636, 444,  636, 438,
      668,  412,  642, 1518, 642,  1520, 642, 1520, 636, 1518, 642, 1518,
      642,  1520, 636, 1520, 668,  1494, 642, 438,  636, 444,  664, 412,
      642,  438,  668, 412,  642,  438,  636, 442,  638, 442,  638, 438,
      642,  1518, 642, 438,  642,  1518, 638, 438,  642, 1518, 642, 440,
      640,  438,  636, 444,  636,  444,  636, 1520, 642, 438,  642, 1520,
      636,  444,  636, 438,  642,  1520, 640, 1520, 636, 1520, 668, 412,
      642,  438,  642, 438,  642,  438,  638, 442,  636, 438,  642, 438,
      668,  412,  640, 1520, 638,  1524, 636, 1520, 642, 1520, 636, 444,
      638,  1522, 638, 1518, 640,  438,  642};

  irsend.reset();
  irsend.sendRaw(rawData4, 295, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(TOSHIBA_AC, irsend.capture.decode_type);
  ASSERT_EQ(kToshibaACBits, irsend.capture.bits);
  toshiba.setRaw(irsend.capture.state);
  EXPECT_FALSE(toshiba.getPower());
  EXPECT_EQ(22, toshiba.getTemp());
  EXPECT_EQ(4, toshiba.getFan());

  // Confirming the quirky behaviour that the 'Power OFF' signal
  // sets the mode to heat.
  // The previous state the remote was in was 'AUTO' just prior to
  // sending the power off message.
  EXPECT_EQ(kToshibaAcHeat, toshiba.getMode());
}

TEST(TestToshibaACClass, toCommon) {
  IRToshibaAC ac(0);
  ac.setPower(true);
  ac.setMode(kToshibaAcCool);
  ac.setTemp(20);
  ac.setFan(kToshibaAcFanMax);
  // Now test it.
  ASSERT_EQ(decode_type_t::TOSHIBA_AC, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  // Unsupported.
  ASSERT_EQ(stdAc::swingv_t::kOff, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}
