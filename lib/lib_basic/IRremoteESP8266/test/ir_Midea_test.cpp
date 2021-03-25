// Copyright 2017 David Conran

#include "ir_Midea.h"
#include "IRac.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendMidea().

// Test sending typical data only.
TEST(TestSendMidea, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendMidea(0x0);
  EXPECT_EQ(
      "f38000d50"
      "m4480s4480"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s5600"
      "m4480s4480"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s105600",
      irsend.outputStr());

  irsend.reset();
  irsend.sendMidea(0x55AA55AA55AA);
  EXPECT_EQ(
      "f38000d50"
      "m4480s4480"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s5600"
      "m4480s4480"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s105600",
      irsend.outputStr());

  irsend.reset();
  irsend.sendMidea(0xFFFFFFFFFFFF);
  EXPECT_EQ(
      "f38000d50"
      "m4480s4480"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s5600"
      "m4480s4480"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s105600",
      irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendMidea, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendMidea(0x55AA55AA55AA, kMideaBits, 1);  // 1 repeat.
  EXPECT_EQ(
      "f38000d50"
      "m4480s4480"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s5600"
      "m4480s4480"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s105600"
      "m4480s4480"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s5600"
      "m4480s4480"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s105600",
      irsend.outputStr());
  irsend.sendMidea(0x55AA55AA55AA, kMideaBits, 2);  // 2 repeats.
  EXPECT_EQ(
      "f38000d50"
      "m4480s4480"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s5600"
      "m4480s4480"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s105600"
      "m4480s4480"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s5600"
      "m4480s4480"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s105600"
      "m4480s4480"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s5600"
      "m4480s4480"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s105600",
      irsend.outputStr());
}

// Test sending an atypical data size.
TEST(TestSendMidea, SendUnusualSize) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendMidea(0x0, 8);
  EXPECT_EQ(
      "f38000d50"
      "m4480s4480"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s5600"
      "m4480s4480"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s105600",
      irsend.outputStr());

  irsend.reset();
  irsend.sendMidea(0x1234567890ABCDEF, 64);
  EXPECT_EQ(
      "f38000d50"
      "m4480s4480"
      "m560s560m560s560m560s560m560s1680m560s560m560s560m560s1680m560s560"
      "m560s560m560s560m560s1680m560s1680m560s560m560s1680m560s560m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s1680m560s560"
      "m560s560m560s1680m560s1680m560s1680m560s1680m560s560m560s560m560s560"
      "m560s1680m560s560m560s560m560s1680m560s560m560s560m560s560m560s560"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s1680"
      "m560s1680m560s1680m560s560m560s560m560s1680m560s1680m560s560m560s1680"
      "m560s1680m560s1680m560s1680m560s560m560s1680m560s1680m560s1680m560s1680"
      "m560s5600"
      "m4480s4480"
      "m560s1680m560s1680m560s1680m560s560m560s1680m560s1680m560s560m560s1680"
      "m560s1680m560s1680m560s560m560s560m560s1680m560s560m560s1680m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s560m560s1680"
      "m560s1680m560s560m560s560m560s560m560s560m560s1680m560s1680m560s1680"
      "m560s560m560s1680m560s1680m560s560m560s1680m560s1680m560s1680m560s1680"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s560"
      "m560s560m560s560m560s1680m560s1680m560s560m560s560m560s1680m560s560"
      "m560s560m560s560m560s560m560s1680m560s560m560s560m560s560m560s560"
      "m560s105600",
      irsend.outputStr());

  // Bit sizes must be a multiple of 8.
  irsend.reset();
  irsend.sendMidea(0x0, 17);
  EXPECT_EQ("", irsend.outputStr());
}

// Tests for IRMideaAC class.

// Tests for controlling the power state.
TEST(TestMideaACClass, Power) {
  IRMideaAC midea(0);
  midea.begin();

  midea.setRaw(0xA1026FFFFFE2);  // Power off.

  midea.on();
  EXPECT_TRUE(midea.getPower());

  EXPECT_EQ(0xA1826FFFFF62, midea.getRaw());

  midea.off();
  EXPECT_FALSE(midea.getPower());
  EXPECT_EQ(0xA1026FFFFFE2, midea.getRaw());

  midea.setPower(true);
  EXPECT_TRUE(midea.getPower());
  EXPECT_EQ(0xA1826FFFFF62, midea.getRaw());

  midea.setPower(false);
  EXPECT_FALSE(midea.getPower());
  EXPECT_EQ(0xA1026FFFFFE2, midea.getRaw());
}

// Tests for the various Checksum routines.
TEST(TestMideaACClass, Checksums) {
  IRMideaAC midea(0);
  midea.begin();

  // Known good states
  EXPECT_EQ(0x62, IRMideaAC::calcChecksum(0xA1826FFFFF62));
  EXPECT_EQ(0x70, IRMideaAC::calcChecksum(0xA18177FFFF70));
  // Now without the checksum part.
  EXPECT_EQ(0x62, IRMideaAC::calcChecksum(0xA1826FFFFF00));
  EXPECT_EQ(0x70, IRMideaAC::calcChecksum(0xA18177FFFF00));
  // Made up values.
  EXPECT_EQ(0x00, IRMideaAC::calcChecksum(0x000000000000));
  EXPECT_EQ(0xDF, IRMideaAC::calcChecksum(0x1234567890AB));
  EXPECT_EQ(0xA0, IRMideaAC::calcChecksum(0xFFFFFFFFFFFF));
  // Larger than expected value (full 64bit)
  EXPECT_EQ(0xDF, IRMideaAC::calcChecksum(0xFF1234567890AB));
  EXPECT_EQ(0xDF, IRMideaAC::calcChecksum(0x551234567890AB));

  // Validity tests.
  EXPECT_TRUE(IRMideaAC::validChecksum(0xA1826FFFFF62));
  EXPECT_TRUE(IRMideaAC::validChecksum(0xA18177FFFF70));
  EXPECT_FALSE(IRMideaAC::validChecksum(0x1234567890AB));

  // Doing a setRaw() with a bad state should make a valid checksum on getRaw().
  midea.setRaw(0xA1826FFFFF00);
  EXPECT_EQ(0xA1826FFFFF62, midea.getRaw());
}

TEST(TestMideaACClass, OperatingMode) {
  IRMideaAC midea(0);
  midea.begin();

  midea.setRaw(0xA1826FFFFF62);  // Auto mode already set.
  midea.setMode(kMideaACAuto);
  EXPECT_EQ(kMideaACAuto, midea.getMode());
  EXPECT_EQ(0xA1826FFFFF62, midea.getRaw());  // State shouldn't have changed.

  midea.setMode(kMideaACCool);
  EXPECT_EQ(kMideaACCool, midea.getMode());
  EXPECT_EQ(0xA1806FFFFF61, midea.getRaw());

  midea.setMode(kMideaACAuto);
  EXPECT_EQ(kMideaACAuto, midea.getMode());
  EXPECT_EQ(0xA1826FFFFF62, midea.getRaw());

  midea.setMode(kMideaACHeat);
  EXPECT_EQ(kMideaACHeat, midea.getMode());
  EXPECT_EQ(0xA1836FFFFF63, midea.getRaw());

  midea.setMode(kMideaACDry);
  EXPECT_EQ(kMideaACDry, midea.getMode());
  EXPECT_EQ(0xA1816FFFFF60, midea.getRaw());

  midea.setMode(kMideaACFan);
  EXPECT_EQ(kMideaACFan, midea.getMode());
  EXPECT_EQ(0xA1846FFFFF66, midea.getRaw());

  midea.setMode(255);
  EXPECT_EQ(kMideaACAuto, midea.getMode());
  EXPECT_EQ(0xA1826FFFFF62, midea.getRaw());
}

TEST(TestMideaACClass, FanSpeed) {
  IRMideaAC midea(0);
  midea.begin();

  midea.setRaw(0xA1826FFFFF62);  // Auto mode already set.
  EXPECT_EQ(kMideaACFanAuto, midea.getFan());

  midea.setFan(kMideaACFanLow);
  EXPECT_EQ(kMideaACFanLow, midea.getFan());
  EXPECT_EQ(0xA18A6FFFFF6C, midea.getRaw());

  midea.setFan(255);  // Setting an unexpected value defaults to auto.
  EXPECT_EQ(kMideaACFanAuto, midea.getFan());
  EXPECT_EQ(0xA1826FFFFF62, midea.getRaw());

  midea.setFan(kMideaACFanMed);
  EXPECT_EQ(kMideaACFanMed, midea.getFan());
  EXPECT_EQ(0xA1926FFFFF7C, midea.getRaw());

  midea.setFan(kMideaACFanHigh);
  EXPECT_EQ(kMideaACFanHigh, midea.getFan());
  EXPECT_EQ(0xA19A6FFFFF74, midea.getRaw());

  midea.setFan(kMideaACFanAuto);
  EXPECT_EQ(kMideaACFanAuto, midea.getFan());
  EXPECT_EQ(0xA1826FFFFF62, midea.getRaw());
}

TEST(TestMideaACClass, Temperature) {
  IRMideaAC midea(0);
  midea.begin();

  midea.setRaw(0xA1826FFFFF62);         // 77F / 25C
  EXPECT_EQ(77, midea.getTemp());       // F
  EXPECT_EQ(77, midea.getTemp(false));  // F
  EXPECT_EQ(25, midea.getTemp(true));   // F

  midea.setTemp(0);
  EXPECT_EQ(kMideaACMinTempF, midea.getTemp());
  EXPECT_EQ(0xA18260FFFF6C, midea.getRaw());

  midea.setTemp(255);
  EXPECT_EQ(kMideaACMaxTempF, midea.getTemp());
  EXPECT_EQ(0xA18278FFFF78, midea.getRaw());

  midea.setTemp(0, true);
  EXPECT_EQ(kMideaACMinTempF, midea.getTemp());
  EXPECT_EQ(0xA18260FFFF6C, midea.getRaw());

  midea.setTemp(255, true);
  EXPECT_EQ(kMideaACMaxTempF, midea.getTemp());
  EXPECT_EQ(0xA18278FFFF78, midea.getRaw());

  // fahrenheit min/max etc.
  midea.setTemp(kMideaACMinTempF);
  EXPECT_EQ(kMideaACMinTempF, midea.getTemp());

  midea.setTemp(kMideaACMaxTempF);
  EXPECT_EQ(kMideaACMaxTempF, midea.getTemp());

  midea.setTemp(kMideaACMinTempF - 1);
  EXPECT_EQ(kMideaACMinTempF, midea.getTemp());

  midea.setTemp(kMideaACMaxTempF + 1);
  EXPECT_EQ(kMideaACMaxTempF, midea.getTemp());

  // celsius min/max etc.
  midea.setTemp(kMideaACMinTempC, true);
  EXPECT_EQ(kMideaACMinTempC, midea.getTemp(true));
  EXPECT_EQ(kMideaACMinTempF, midea.getTemp(false));

  midea.setTemp(kMideaACMaxTempC, true);
  EXPECT_EQ(kMideaACMaxTempC, midea.getTemp(true));
  EXPECT_EQ(kMideaACMaxTempF, midea.getTemp(false));

  midea.setTemp(kMideaACMinTempC - 1, true);
  EXPECT_EQ(kMideaACMinTempC, midea.getTemp(true));

  midea.setTemp(kMideaACMaxTempC + 1, true);
  EXPECT_EQ(kMideaACMaxTempC, midea.getTemp(true));
  EXPECT_EQ(kMideaACMaxTempF, midea.getTemp(false));

  // General changes.
  midea.setTemp(18, true);              // C
  EXPECT_EQ(18, midea.getTemp(true));   // C
  EXPECT_EQ(64, midea.getTemp(false));  // F

  midea.setTemp(21, true);              // C
  EXPECT_EQ(21, midea.getTemp(true));   // C
  EXPECT_EQ(69, midea.getTemp(false));  // F

  midea.setTemp(25, true);              // C
  EXPECT_EQ(25, midea.getTemp(true));   // C
  EXPECT_EQ(77, midea.getTemp(false));  // F

  midea.setTemp(30, true);              // C
  EXPECT_EQ(30, midea.getTemp(true));   // C
  EXPECT_EQ(86, midea.getTemp(false));  // F

  midea.setTemp(80, false);             // F
  EXPECT_EQ(27, midea.getTemp(true));   // C
  EXPECT_EQ(80, midea.getTemp(false));  // F

  midea.setTemp(70);                    // F
  EXPECT_EQ(21, midea.getTemp(true));   // C
  EXPECT_EQ(70, midea.getTemp(false));  // F
  EXPECT_EQ(70, midea.getTemp());       // F
}

// Tests for controlling the sleep state.
TEST(TestMideaACClass, Sleep) {
  IRMideaAC midea(0);
  midea.begin();

  midea.setRaw(0xA1826FFFFF62);  // Sleep off.

  EXPECT_FALSE(midea.getSleep());
  midea.setSleep(true);
  EXPECT_TRUE(midea.getSleep());
  EXPECT_EQ(0xA1C26FFFFF22, midea.getRaw());
  midea.setSleep(false);
  EXPECT_FALSE(midea.getSleep());
  EXPECT_EQ(0xA1826FFFFF62, midea.getRaw());
}

TEST(TestMideaACClass, HumanReadableOutput) {
  IRMideaAC ac(kGpioUnused);
  ac.begin();

  ac.setRaw(0xA1826FFFFF62);
  EXPECT_EQ(
      "Type: 1 (Command), Power: On, Mode: 2 (Auto), Celsius: Off, "
      "Temp: 25C/77F, On Timer: Off, Off Timer: Off, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
  ac.off();
  ac.setTemp(25, true);
  ac.setFan(kMideaACFanHigh);
  ac.setMode(kMideaACDry);
  ac.setSleep(true);
  EXPECT_EQ(
      "Type: 1 (Command), Power: Off, Mode: 1 (Dry), Celsius: Off, "
      "Temp: 25C/77F, On Timer: Off, Off Timer: Off, Fan: 3 (High), "
      "Sleep: On, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
  ac.setUseCelsius(true);
  EXPECT_EQ(
      "Type: 1 (Command), Power: Off, Mode: 1 (Dry), Celsius: On, "
      "Temp: 25C/77F, On Timer: Off, Off Timer: Off, Fan: 3 (High), "
      "Sleep: On, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());

  ac.setRaw(0xA19867FFFF7E);
  EXPECT_EQ(
      "Type: 1 (Command), Power: On, Mode: 0 (Cool), Celsius: Off, "
      "Temp: 21C/69F, On Timer: Off, Off Timer: Off, Fan: 3 (High), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
}

// Tests for decodeMidea().

// Decode normal Midea messages with strict set.
TEST(TestDecodeMidea, NormalDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Midea 48-bit message.
  irsend.reset();
  irsend.sendMidea(0x1234567890DF);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeMidea(&irsend.capture, kStartOffset, kMideaBits,
                                 true));
  EXPECT_EQ(MIDEA, irsend.capture.decode_type);
  EXPECT_EQ(kMideaBits, irsend.capture.bits);
  EXPECT_EQ(0x1234567890DF, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal Midea 48-bit message.
  irsend.reset();
  irsend.sendMidea(0x0);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeMidea(&irsend.capture, kStartOffset, kMideaBits,
                                 true));
  EXPECT_EQ(MIDEA, irsend.capture.decode_type);
  EXPECT_EQ(kMideaBits, irsend.capture.bits);
  EXPECT_EQ(0x0, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal Midea 48-bit message.
  irsend.reset();
  irsend.sendMidea(0xFFFFFFFFFFA0);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeMidea(&irsend.capture, kStartOffset, kMideaBits,
                                 true));
  EXPECT_EQ(MIDEA, irsend.capture.decode_type);
  EXPECT_EQ(kMideaBits, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFFFFFFA0, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Real Midea 48-bit message via just decode().
  // i.e. No conficts with other decoders.
  irsend.reset();
  irsend.sendMidea(0xA18263FFFF6E);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MIDEA, irsend.capture.decode_type);
  EXPECT_EQ(kMideaBits, irsend.capture.bits);
  EXPECT_EQ(0xA18263FFFF6E, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode normal repeated Midea messages.
TEST(TestDecodeMidea, NormalDecodeWithRepeatAndStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Midea 48-bit message with 2 repeats.
  irsend.reset();
  irsend.sendMidea(0xA18263FFFF6E, kMideaBits, 2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeMidea(&irsend.capture, kStartOffset, kMideaBits,
                                 true));
  EXPECT_EQ(MIDEA, irsend.capture.decode_type);
  EXPECT_EQ(kMideaBits, irsend.capture.bits);
  EXPECT_EQ(0xA18263FFFF6E, irsend.capture.value);
  EXPECT_FALSE(irsend.capture.repeat);

  irsend.makeDecodeResult(2 * (2 * kMideaBits + 4));
  ASSERT_TRUE(irrecv.decodeMidea(&irsend.capture, kStartOffset, kMideaBits,
                                 true));
  EXPECT_EQ(MIDEA, irsend.capture.decode_type);
  EXPECT_EQ(kMideaBits, irsend.capture.bits);
  EXPECT_EQ(0xA18263FFFF6E, irsend.capture.value);

  irsend.makeDecodeResult(4 * (2 * kMideaBits + 4));
  ASSERT_TRUE(irrecv.decodeMidea(&irsend.capture, kStartOffset, kMideaBits,
                                 true));
  EXPECT_EQ(MIDEA, irsend.capture.decode_type);
  EXPECT_EQ(kMideaBits, irsend.capture.bits);
  EXPECT_EQ(0xA18263FFFF6E, irsend.capture.value);
}

// Decode unsupported Midea messages.
TEST(TestDecodeMidea, DecodeWithNonStrictSizes) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  irsend.sendMidea(0x12, 8);  // Illegal value Midea 8-bit message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeMidea(&irsend.capture, kStartOffset, kMideaBits,
                                  true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeMidea(&irsend.capture, kStartOffset, 8, false));
  EXPECT_EQ(MIDEA, irsend.capture.decode_type);
  EXPECT_EQ(8, irsend.capture.bits);
  EXPECT_EQ(0x12, irsend.capture.value);

  irsend.reset();
  irsend.sendMidea(0x12345678, 32);  // Illegal value Midea 32-bit message.
  irsend.makeDecodeResult();
  // Shouldn't pass with strict when we ask for less bits than we got.
  ASSERT_FALSE(irrecv.decodeMidea(&irsend.capture, kStartOffset, kMideaBits,
                                  true));

  irsend.makeDecodeResult();
  // Should fail with strict when we ask for the wrong bit size.
  ASSERT_FALSE(irrecv.decodeMidea(&irsend.capture, kStartOffset, 32, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeMidea(&irsend.capture, kStartOffset, 32, false));
  EXPECT_EQ(MIDEA, irsend.capture.decode_type);
  EXPECT_EQ(32, irsend.capture.bits);
  EXPECT_EQ(0x12345678, irsend.capture.value);

  // Decode should fail if asked to decode non-multiples of 8 bits.
  irsend.reset();
  irsend.sendMidea(0x123456, kMideaBits, 2);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeMidea(&irsend.capture, kStartOffset, 9, false));
}

// Decode (non-standard) 64-bit messages.
TEST(TestDecodeMidea, Decode64BitMessages) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Illegal size Midea 64-bit message.
  irsend.sendMidea(0xFFFFFFFFFFFFFFFF, 64);
  irsend.makeDecodeResult();
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeMidea(&irsend.capture, kStartOffset, 64, false));
  EXPECT_EQ(MIDEA, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, irsend.capture.value);
}

// Fail to decode a non-Midea example via GlobalCache
TEST(TestDecodeMidea, FailToDecodeNonMideaExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Modified a few entries to unexpected values, based on previous test case.
  uint16_t gc_test[39] = {38000, 1,  1,  322, 162, 20, 61,  20, 61, 20,
                          20,    20, 20, 20,  20,  20, 127, 20, 61, 9,
                          20,    20, 61, 20,  20,  20, 61,  20, 61, 20,
                          61,    20, 20, 20,  20,  20, 20,  20, 884};
  irsend.sendGC(gc_test, 39);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeMidea(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeMidea(&irsend.capture, kStartOffset, kMideaBits,
                                  false));
}

// Decode against a real capture reported by a user. See issue #354
TEST(TestDecodeMidea, DecodeRealExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();
  irsend.reset();

  uint16_t rawData[199] = {
      4366, 4470, 498, 1658, 522,  554,  498, 1658, 496, 580,  498, 580,
      498,  578,  498, 580,  498,  1658, 498, 1658, 498, 578,  498, 578,
      498,  580,  496, 582,  496,  578,  498, 1658, 498, 580,  498, 580,
      498,  1656, 498, 1656, 500,  580,  498, 578,  502, 576,  500, 1656,
      498,  1656, 500, 1654, 500,  1656, 500, 1656, 498, 1658, 498, 1656,
      500,  1658, 498, 1656, 498,  1656, 500, 1656, 500, 1654, 500, 1578,
      578,  1658, 498, 1656, 500,  1658, 498, 1656, 498, 1656, 500, 578,
      498,  1638, 516, 1656, 500,  578,  500, 1656, 500, 1656, 498, 1658,
      522,  554,  500, 5258, 4366, 4472, 498, 580,  498, 1658, 498, 580,
      498,  1656, 500, 1600, 556,  1658, 500, 1656, 500, 578,  498, 578,
      522,  1634, 498, 1588, 568,  1658, 498, 1656, 500, 1654, 498, 580,
      498,  1658, 498, 1658, 498,  580,  496, 578,  500, 1654, 500, 1636,
      518,  1656, 500, 578,  520,  558,  498, 578,  498, 580,  498, 576,
      500,  578,  498, 580,  498,  578,  498, 578,  498, 580,  498, 578,
      498,  580,  498, 580,  520,  556,  498, 580,  496, 580,  498, 578,
      500,  578,  498, 1658, 498,  580,  498, 578,  498, 1656, 500, 578,
      498,  580,  498, 580,  498,  1656, 522};
  irsend.sendRaw(rawData, 199, 38000);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MIDEA, irsend.capture.decode_type);
  EXPECT_EQ(kMideaBits, irsend.capture.bits);
  EXPECT_EQ(0xA18263FFFF6E, irsend.capture.value);
  EXPECT_EQ(
      "Type: 1 (Command), Power: On, Mode: 2 (Auto), Celsius: Off, "
      "Temp: 18C/65F, On Timer: Off, Off Timer: Off, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

TEST(TestMideaACClass, toCommon) {
  IRMideaAC ac(kGpioUnused);
  ac.setPower(true);
  ac.setMode(kMideaACCool);
  ac.setUseCelsius(true);
  ac.setTemp(20, true);
  ac.setFan(kMideaACFanHigh);
  // Now test it.
  ASSERT_EQ(decode_type_t::MIDEA, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kOff, ac.toCommon().swingv);
  // Unsupported.
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

// https://github.com/crankyoldgit/IRremoteESP8266/issues/819
TEST(TestMideaACClass, CelsiusRemoteTemp) {
  IRMideaAC ac(kGpioUnused);
  uint64_t on_cool_low_17c = 0xA18840FFFF56;
  uint64_t on_cool_low_30c = 0xA1884DFFFF5D;
  ac.on();
  ac.setMode(kMideaACCool);
  ac.setFan(kMideaACFanLow);
  ac.setTemp(17, true);
  EXPECT_FALSE(ac.getUseCelsius());
  ac.setUseCelsius(true);
  EXPECT_TRUE(ac.getUseCelsius());
  EXPECT_EQ(on_cool_low_17c, ac.getRaw());
  EXPECT_EQ(
      "Type: 1 (Command), Power: On, Mode: 0 (Cool), Celsius: On, "
      "Temp: 17C/62F, On Timer: Off, Off Timer: Off, Fan: 1 (Low), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
  ac.setRaw(on_cool_low_17c);
  EXPECT_EQ(17, ac.getTemp(true));
  EXPECT_EQ(62, ac.getTemp(false));
  EXPECT_EQ(
      "Type: 1 (Command), Power: On, Mode: 0 (Cool), Celsius: On, "
      "Temp: 17C/62F, On Timer: Off, Off Timer: Off, Fan: 1 (Low), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
  ac.setTemp(17, true);
  EXPECT_EQ(17, ac.getTemp(true));
  EXPECT_EQ(62, ac.getTemp(false));
  EXPECT_EQ(on_cool_low_17c, ac.getRaw());

  ac.setRaw(on_cool_low_30c);
  EXPECT_EQ(
      "Type: 1 (Command), Power: On, Mode: 0 (Cool), Celsius: On, "
      "Temp: 30C/86F, On Timer: Off, Off Timer: Off, Fan: 1 (Low), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
}

// https://github.com/crankyoldgit/IRremoteESP8266/issues/819
TEST(TestMideaACClass, SwingV) {
  IRMideaAC ac(kGpioUnused);
  ac.setSwingVToggle(false);
  ASSERT_FALSE(ac.getSwingVToggle());
  ac.setSwingVToggle(true);
  ASSERT_TRUE(ac.getSwingVToggle());
  EXPECT_EQ(
      "Type: 1 (Command), Power: On, Mode: 2 (Auto), Celsius: Off, "
      "Temp: 25C/77F, On Timer: Off, Off Timer: Off, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: On, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
  ac.setSwingVToggle(false);
  ASSERT_FALSE(ac.getSwingVToggle());
  EXPECT_EQ(
      "Type: 1 (Command), Power: On, Mode: 2 (Auto), Celsius: Off, "
      "Temp: 25C/77F, On Timer: Off, Off Timer: Off, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
  ac.setRaw(kMideaACToggleSwingV);
  EXPECT_EQ("Type: 2 (Special), Swing(V) Toggle: On, Econo Toggle: Off, "
            "Turbo Toggle: Off, Light Toggle: Off",
            ac.toString());
}

// https://github.com/crankyoldgit/IRremoteESP8266/pull/1213
TEST(TestMideaACClass, Econo) {
  IRMideaAC ac(kGpioUnused);
  ac.setEconoToggle(false);
  ASSERT_FALSE(ac.getEconoToggle());
  ac.setEconoToggle(true);
  ASSERT_TRUE(ac.getEconoToggle());
  EXPECT_EQ(
      "Type: 1 (Command), Power: On, Mode: 2 (Auto), Celsius: Off, "
      "Temp: 25C/77F, On Timer: Off, Off Timer: Off, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: On, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
  ac.setEconoToggle(false);
  ASSERT_FALSE(ac.getEconoToggle());
  EXPECT_EQ(
      "Type: 1 (Command), Power: On, Mode: 2 (Auto), Celsius: Off, "
      "Temp: 25C/77F, On Timer: Off, Off Timer: Off, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
  ac.setRaw(kMideaACToggleEcono);
  EXPECT_EQ("Type: 2 (Special), Swing(V) Toggle: Off, Econo Toggle: On, "
            "Turbo Toggle: Off, Light Toggle: Off",
            ac.toString());
}

TEST(TestMideaACClass, Turbo) {
  IRMideaAC ac(kGpioUnused);
  ac.setTurboToggle(false);
  ASSERT_FALSE(ac.getTurboToggle());
  ac.setTurboToggle(true);
  ASSERT_TRUE(ac.getTurboToggle());
  EXPECT_EQ(
      "Type: 1 (Command), Power: On, Mode: 2 (Auto), Celsius: Off, "
      "Temp: 25C/77F, On Timer: Off, Off Timer: Off, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: On, Light Toggle: Off", ac.toString());
  ac.setTurboToggle(false);
  ASSERT_FALSE(ac.getTurboToggle());
  EXPECT_EQ(
      "Type: 1 (Command), Power: On, Mode: 2 (Auto), Celsius: Off, "
      "Temp: 25C/77F, On Timer: Off, Off Timer: Off, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
  ac.setRaw(kMideaACToggleTurbo);
  EXPECT_EQ("Type: 2 (Special), Swing(V) Toggle: Off, Econo Toggle: Off, "
            "Turbo Toggle: On, Light Toggle: Off",
            ac.toString());
}

TEST(TestMideaACClass, Light) {
  IRMideaAC ac(kGpioUnused);
  ac.setLightToggle(false);
  ASSERT_FALSE(ac.getLightToggle());
  ac.setLightToggle(true);
  ASSERT_TRUE(ac.getLightToggle());
  EXPECT_EQ(
      "Type: 1 (Command), Power: On, Mode: 2 (Auto), Celsius: Off, "
      "Temp: 25C/77F, On Timer: Off, Off Timer: Off, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: On", ac.toString());
  ac.setLightToggle(false);
  ASSERT_FALSE(ac.getLightToggle());
  EXPECT_EQ(
      "Type: 1 (Command), Power: On, Mode: 2 (Auto), Celsius: Off, "
      "Temp: 25C/77F, On Timer: Off, Off Timer: Off, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
  ac.setRaw(kMideaACToggleLight);
  EXPECT_EQ("Type: 2 (Special), Swing(V) Toggle: Off, Econo Toggle: Off, "
            "Turbo Toggle: Off, Light Toggle: On",
            ac.toString());
}

// Test abusing the protocol for sending 6 arbitrary bytes.
// See https://github.com/crankyoldgit/IRremoteESP8266/issues/887
TEST(TestDecodeMidea, Issue887) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();
  irsend.reset();

  uint64_t hwaddr = 0x1234567890AB;  // 48bits doesn't conform to Midea checksum

  irsend.sendMidea(hwaddr);
  irsend.makeDecodeResult();

  // Test normal operation, it shouldn't match.
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_NE(MIDEA, irsend.capture.decode_type);
  irsend.reset();
  irsend.sendMidea(hwaddr);
  irsend.makeDecodeResult();
  EXPECT_FALSE(irrecv.decodeMidea(&irsend.capture));

  // Now test it with Midea's strict processing turned off!
  irsend.reset();
  irsend.sendMidea(hwaddr);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decodeMidea(&irsend.capture, kStartOffset, kMideaBits,
                                 false));
  EXPECT_EQ(MIDEA, irsend.capture.decode_type);
  EXPECT_EQ(kMideaBits, irsend.capture.bits);
  EXPECT_EQ(hwaddr, irsend.capture.value);
}

// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1170
TEST(TestDecodeMidea24, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  irsend.reset();

  const uint16_t rawData[103] = {
      8928, 4412, 590, 1630, 588, 538, 538, 544, 610, 516, 592, 516, 590, 538,
      568, 536, 538, 568, 592, 518, 588, 1630, 588, 1630, 560, 1680, 592, 1628,
      594, 1628, 592, 1650, 568, 1630, 558, 1680, 594, 1652, 568, 514, 592, 536,
      538, 568, 594, 516, 588, 538, 566, 518, 560, 566, 588, 514, 594, 1630,
      588, 1630, 590, 1630, 560, 1680, 594, 1630, 588, 1652, 568, 1650, 540,
      1680, 590, 512, 594, 518, 586, 538, 566, 538, 536, 568, 592, 540, 564,
      540, 566, 540, 538, 1680, 590, 1626, 592, 1630, 588, 1628, 538, 1702, 590,
      1630, 590, 13318, 8916, 2166, 638};  // UNKNOWN 774B249A

  irsend.sendRaw(rawData, 103, 38);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MIDEA24, irsend.capture.decode_type);
  EXPECT_EQ(kMidea24Bits, irsend.capture.bits);
  EXPECT_EQ(0x80C0C0, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);
}

TEST(TestDecodeMidea24, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  irsend.reset();

  irsend.sendMidea24(0x80C0C0);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MIDEA24, irsend.capture.decode_type);
  EXPECT_EQ(kMidea24Bits, irsend.capture.bits);
  EXPECT_EQ(0x80C0C0, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);
  EXPECT_EQ(
      "f38000d33"
      "m8960s4480"
      "m560s1680m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s1680m560s1680m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s1680m560s1680m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s22400"
      "m8960s2240m560s96320",
      irsend.outputStr());
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("MIDEA", typeToString(decode_type_t::MIDEA));
  ASSERT_EQ(decode_type_t::MIDEA, strToDecodeType("MIDEA"));
  ASSERT_FALSE(hasACState(decode_type_t::MIDEA));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::MIDEA));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::MIDEA));
  ASSERT_EQ(kMideaBits, IRsend::defaultBits(decode_type_t::MIDEA));

  ASSERT_EQ("MIDEA24", typeToString(decode_type_t::MIDEA24));
  ASSERT_EQ(decode_type_t::MIDEA24, strToDecodeType("MIDEA24"));
  ASSERT_FALSE(hasACState(decode_type_t::MIDEA24));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::MIDEA24));
  ASSERT_EQ(kSingleRepeat, IRsend::minRepeats(decode_type_t::MIDEA24));
  ASSERT_EQ(kMidea24Bits, IRsend::defaultBits(decode_type_t::MIDEA24));
}

TEST(TestDecodeMidea24, RealExample2) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  irsend.reset();

  // https://github.com/crankyoldgit/IRremoteESP8266/issues/1170#issuecomment-639271003
  const uint16_t rawData[103] = {
      8926, 4412, 590, 1630, 536, 570, 608, 518, 594, 516, 588, 518, 588, 538,
      538, 568, 592, 514, 590, 518, 588, 1630, 536, 1684, 610, 1628, 594, 1630,
      588, 1630, 590, 1630, 560, 1680, 592, 1630, 588, 1650, 568, 538, 538, 568,
      590, 514, 594, 538, 566, 518, 536, 594, 584, 516, 594, 518, 586, 1630,
      588, 1650, 538, 1682, 592, 1630, 588, 1628, 588, 1630, 560, 1680, 590,
      1626, 594, 538, 566, 538, 568, 536, 538, 570, 590, 538, 566, 538, 568,
      538, 538, 568, 590, 1626, 594, 1650, 568, 1628, 558, 1662, 558, 1680, 592,
      1650, 568, 13312, 8924, 2186, 588};  // UNKNOWN 774B249A

  irsend.sendRaw(rawData, 103, 38);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MIDEA24, irsend.capture.decode_type);
  EXPECT_EQ(kMidea24Bits, irsend.capture.bits);
  EXPECT_EQ(0x80C0C0, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);

  // ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1170#issuecomment-639349316
  const uint16_t rawData639349316[105] = {
      8924, 4410, 596, 1628, 590, 538, 538, 568, 588, 520, 592, 516, 588, 518,
      588, 516, 558, 568, 594, 538, 566, 1652, 568, 1650, 540, 1680, 590, 1628,
      592, 1628, 590, 1628, 590, 1650, 540, 1680, 592, 1630, 590, 518, 586, 538,
      538, 568, 590, 514, 616, 494, 588, 516, 588, 516, 560, 566, 590, 1630,
      586, 1634, 564, 1652, 538, 1704, 566, 948, 254, 450, 564, 1654, 566,
      1652, 540, 1682, 610, 518, 588, 516, 590, 514, 590, 514, 582, 526, 614,
      436, 670, 512, 592, 514, 538, 1578, 718, 1602, 614, 1542, 676, 1626, 538,
      1678, 586, 1634, 616, 13308, 8920, 2158, 626};  // UNKNOWN CBEC0079
  irsend.reset();
  irsend.sendRaw(rawData639349316, 105, 38);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_NE(MIDEA24, irsend.capture.decode_type);

  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1170#issuecomment-639358566
  const uint16_t rawData639358566[103] = {
      8920, 4412, 620, 1606, 614, 512, 592, 512, 536, 568, 618, 512, 590, 492,
      614, 514, 514, 590, 594, 510, 620, 1624, 592, 1604, 590, 1632, 608, 1626,
      606, 1616, 618, 1602, 618, 1602, 560, 1682, 622, 1602, 618, 512, 592, 510,
      538, 568, 590, 512, 622, 488, 616, 490, 614, 488, 560, 568, 620, 1624,
      594, 1604, 616, 1602, 536, 1684, 640, 1600, 618, 1598, 618, 1602, 590,
      1630, 612, 512, 622, 510, 594, 514, 592, 512, 534, 568, 620, 512, 594,
      488, 616, 488, 536, 1702, 594, 1622, 622, 1604, 614, 1624, 594, 1602, 610,
      1630, 620, 13294, 8914, 2184, 596};  // UNKNOWN 774B249A
  irsend.reset();
  irsend.sendRaw(rawData639358566, 103, 38);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MIDEA24, irsend.capture.decode_type);
  EXPECT_EQ(kMidea24Bits, irsend.capture.bits);
  EXPECT_EQ(0x80C0C0, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);
}

// See https://github.com/crankyoldgit/IRremoteESP8266/issues/1170#issuecomment-639468620
// for why this test exists.
TEST(TestDecodeMidea24, LargeTimeout) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused, 1000, 90);  // Test with a large timeout value
  irsend.begin();
  irsend.reset();
  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1170#issuecomment-639358566
  const uint16_t rawData639358566[103] = {
      8920, 4412, 620, 1606, 614, 512, 592, 512, 536, 568, 618, 512, 590, 492,
      614, 514, 514, 590, 594, 510, 620, 1624, 592, 1604, 590, 1632, 608, 1626,
      606, 1616, 618, 1602, 618, 1602, 560, 1682, 622, 1602, 618, 512, 592, 510,
      538, 568, 590, 512, 622, 488, 616, 490, 614, 488, 560, 568, 620, 1624,
      594, 1604, 616, 1602, 536, 1684, 640, 1600, 618, 1598, 618, 1602, 590,
      1630, 612, 512, 622, 510, 594, 514, 592, 512, 534, 568, 620, 512, 594,
      488, 616, 488, 536, 1702, 594, 1622, 622, 1604, 614, 1624, 594, 1602, 610,
      1630, 620, 13294, 8914, 2184, 596};  // UNKNOWN 774B249A
  irsend.reset();
  irsend.sendRaw(rawData639358566, 103, 38);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MIDEA24, irsend.capture.decode_type);
  EXPECT_EQ(kMidea24Bits, irsend.capture.bits);
  EXPECT_EQ(0x80C0C0, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);
}

// Verify the library can self-decode `sendMidea(0xA4A3477F1979)` to the correct
// value.
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1318#issuecomment-721829757
TEST(TestDecodeMidea, Issue1318_self_decode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  irsend.reset();
  irsend.sendMidea(0xA4A3477F1979);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MIDEA, irsend.capture.decode_type);
  EXPECT_EQ(kMideaBits, irsend.capture.bits);
  EXPECT_EQ(0xA4A3477F1979, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);
  EXPECT_EQ(
      "Type: 4 (Follow), Power: On, Mode: 3 (Heat), Celsius: On, "
      "Temp: 24C/75F, SensorTemp: 24C/75F, Off Timer: Off, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  EXPECT_EQ(
      "f38000d50"
      "m4480s4480m560s1680m560s560m560s1680m560s560m560s560m560s1680"
      "m560s560m560s560m560s1680m560s560m560s1680m560s560m560s560"
      "m560s560m560s1680m560s1680m560s560m560s1680m560s560m560s560"
      "m560s560m560s1680m560s1680m560s1680m560s560m560s1680m560s1680"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s560m560s560"
      "m560s560m560s1680m560s1680m560s560m560s560m560s1680m560s560"
      "m560s1680m560s1680m560s1680m560s1680m560s560m560s560m560s1680"
      "m560s5600"
      "m4480s4480m560s560m560s1680m560s560m560s1680m560s1680m560s560"
      "m560s1680m560s1680m560s560m560s1680m560s560m560s1680m560s1680"
      "m560s1680m560s560m560s560m560s1680m560s560m560s1680m560s1680"
      "m560s1680m560s560m560s560m560s560m560s1680m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s1680m560s1680"
      "m560s1680m560s560m560s560m560s1680m560s1680m560s560m560s1680"
      "m560s560m560s560m560s560m560s560m560s1680m560s1680m560s560"
      "m560s105600",
      irsend.outputStr());
}

// https://github.com/crankyoldgit/IRremoteESP8266/issues/1318#issuecomment-722655214
TEST(TestMideaACClass, SensorTemp) {
  IRMideaAC ac(kGpioUnused);
  ASSERT_FALSE(ac.getEnableSensorTemp());  // Off by default.
  ASSERT_NE(kMideaACTypeFollow, ac.getType());
  ac.setSensorTemp(21, true);
  EXPECT_EQ(21, ac.getSensorTemp(true));
  EXPECT_TRUE(ac.getEnableSensorTemp());  // Should be enabled when set.
  EXPECT_EQ(kMideaACTypeFollow, ac.getType());
  ac.setEnableSensorTemp(true);  // Values shouldn't change when already on.
  EXPECT_EQ(21, ac.getSensorTemp(true));
  EXPECT_TRUE(ac.getEnableSensorTemp());  // Should be abled when set.
  ac.setEnableSensorTemp(false);  // Old value should be cleared when disabled.
  EXPECT_NE(21, ac.getSensorTemp(true));
  EXPECT_FALSE(ac.getEnableSensorTemp());
  ASSERT_NE(kMideaACTypeFollow, ac.getType());
  EXPECT_EQ(kMideaACTypeCommand, ac.getType());

  // Fahrenheit
  ac.setSensorTemp(74, false);
  EXPECT_EQ(74, ac.getSensorTemp(false));
  EXPECT_TRUE(ac.getEnableSensorTemp());  // Should be abled when set.

  // Bounds checks
  // Celsius
  ac.setSensorTemp(kMideaACMinSensorTempC, true);
  EXPECT_EQ(kMideaACMinSensorTempC, ac.getSensorTemp(true));
  EXPECT_TRUE(ac.getEnableSensorTemp());  // Should be enabled when set.
  ac.setSensorTemp(kMideaACMaxSensorTempC, true);
  EXPECT_EQ(kMideaACMaxSensorTempC, ac.getSensorTemp(true));
  EXPECT_TRUE(ac.getEnableSensorTemp());  // Should be enabled when set.
  ac.setSensorTemp(kMideaACMaxSensorTempC + 1, true);
  EXPECT_EQ(kMideaACMaxSensorTempC, ac.getSensorTemp(true));
  EXPECT_TRUE(ac.getEnableSensorTemp());  // Should be enabled when set.
  // Fahrenheit
  ac.setSensorTemp(kMideaACMinSensorTempF, false);
  EXPECT_EQ(kMideaACMinSensorTempF, ac.getSensorTemp(false));
  EXPECT_TRUE(ac.getEnableSensorTemp());  // Should be enabled when set.
  ac.setSensorTemp(kMideaACMinSensorTempF - 1, false);
  EXPECT_EQ(kMideaACMinSensorTempF, ac.getSensorTemp(false));
  EXPECT_TRUE(ac.getEnableSensorTemp());  // Should be enabled when set.
  ac.setSensorTemp(kMideaACMaxSensorTempF, false);
  EXPECT_EQ(kMideaACMaxSensorTempF, ac.getSensorTemp(false));
  EXPECT_TRUE(ac.getEnableSensorTemp());  // Should be enabled when set.
  ac.setSensorTemp(kMideaACMaxSensorTempF + 1, false);
  EXPECT_EQ(kMideaACMaxSensorTempF, ac.getSensorTemp(false));
  EXPECT_TRUE(ac.getEnableSensorTemp());  // Should be enabled when set.

  // Real examples
  ac.setRaw(0xA482467F015D);  // 0C
  EXPECT_EQ(0, ac.getSensorTemp(true));
  EXPECT_TRUE(ac.getEnableSensorTemp());
  EXPECT_EQ(
      "Type: 4 (Follow), Power: On, Mode: 2 (Auto), Celsius: On, "
      "Temp: 23C/73F, SensorTemp: 0C/32F, Off Timer: Off, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
  ac.setRaw(0xA482467F134E);  // 18C
  EXPECT_EQ(18, ac.getSensorTemp(true));
  EXPECT_TRUE(ac.getEnableSensorTemp());
  EXPECT_EQ(
      "Type: 4 (Follow), Power: On, Mode: 2 (Auto), Celsius: On, "
      "Temp: 23C/73F, SensorTemp: 18C/64F, Off Timer: Off, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
  ac.setRaw(0xA482467F266B);  // 37C
  EXPECT_EQ(37, ac.getSensorTemp(true));
  EXPECT_TRUE(ac.getEnableSensorTemp());
  EXPECT_EQ(
      "Type: 4 (Follow), Power: On, Mode: 2 (Auto), Celsius: On, "
      "Temp: 23C/73F, SensorTemp: 37C/98F, Off Timer: Off, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
  ac.setRaw(0xA482607F2B43);  // 74F
  EXPECT_EQ(74, ac.getSensorTemp(false));
  EXPECT_TRUE(ac.getEnableSensorTemp());
  EXPECT_EQ(
      "Type: 4 (Follow), Power: On, Mode: 2 (Auto), Celsius: Off, "
      "Temp: 17C/62F, SensorTemp: 23C/74F, Off Timer: Off, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
}

TEST(TestMideaACClass, MessageType) {
  IRMideaAC ac(kGpioUnused);
  EXPECT_EQ(kMideaACTypeCommand, ac.getType());
  ac.setType(kMideaACTypeFollow);
  EXPECT_EQ(kMideaACTypeFollow, ac.getType());
  ac.setType(kMideaACTypeCommand);
  EXPECT_EQ(kMideaACTypeCommand, ac.getType());
  ac.setType(kMideaACTypeSpecial);
  EXPECT_EQ(kMideaACTypeSpecial, ac.getType());
  ac.setType(0);
  EXPECT_EQ(kMideaACTypeCommand, ac.getType());
  ac.setType(255);
  EXPECT_EQ(kMideaACTypeCommand, ac.getType());
}

TEST(TestMideaACClass, ConstructKnownMessage) {
  IRMideaAC ac(kGpioUnused);
  const uint64_t followme_25C = 0xA482467F1A47;
  ac.stateReset();
  ac.on();
  ac.setMode(kMideaACAuto);
  ac.setUseCelsius(true);
  ac.setTemp(23, true);
  ac.setSensorTemp(25, true);
  ac.setFan(kMideaACFanAuto);
  EXPECT_EQ(
      "Type: 4 (Follow), Power: On, Mode: 2 (Auto), Celsius: On, "
      "Temp: 23C/73F, SensorTemp: 25C/77F, Off Timer: Off, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
  EXPECT_EQ(followme_25C, ac.getRaw());
}

TEST(TestMideaACClass, OnTimer) {
  IRMideaAC ac(kGpioUnused);
  EXPECT_FALSE(ac.isOnTimerEnabled());
  // Data from: https://github.com/crankyoldgit/IRremoteESP8266/issues/1318#issuecomment-723171666
  ac.setRaw(0xA1A34DFF9B34);  // 7 hours.
  EXPECT_TRUE(ac.isOnTimerEnabled());
  EXPECT_EQ(7 * 60, ac.getOnTimer());
  EXPECT_EQ(
      "Type: 1 (Command), Power: On, Mode: 3 (Heat), Celsius: On, "
      "Temp: 30C/86F, On Timer: 07:00, Off Timer: Off, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
  ac.setOnTimer(0);  // Disable / Timer off.
  EXPECT_FALSE(ac.isOnTimerEnabled());
  EXPECT_EQ(kMideaACTypeCommand, ac.getType());
  ac.setOnTimer(3 * 60 + 48);  // 3h48m
  EXPECT_TRUE(ac.isOnTimerEnabled());
  EXPECT_EQ(kMideaACTypeCommand, ac.getType());
  EXPECT_EQ(3 * 60 + 30, ac.getOnTimer());
  EXPECT_EQ(
      "Type: 1 (Command), Power: On, Mode: 3 (Heat), Celsius: On, "
      "Temp: 30C/86F, On Timer: 03:30, Off Timer: Off, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
  // Limit to max time.
  ac.setOnTimer(25 * 60);  // 25h
  EXPECT_TRUE(ac.isOnTimerEnabled());
  EXPECT_EQ(kMideaACTypeCommand, ac.getType());
  EXPECT_EQ(24 * 60, ac.getOnTimer());
  EXPECT_EQ(
      "Type: 1 (Command), Power: On, Mode: 3 (Heat), Celsius: On, "
      "Temp: 30C/86F, On Timer: 24:00, Off Timer: Off, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
}


TEST(TestMideaACClass, OffTimer) {
  IRMideaAC ac(kGpioUnused);
  EXPECT_FALSE(ac.isOffTimerEnabled());
  // Data from: https://github.com/crankyoldgit/IRremoteESP8266/issues/1318#issuecomment-723171666
  ac.setRaw(0xA1A34D81FF21);  // 0.5 hours.
  EXPECT_TRUE(ac.isOffTimerEnabled());
  EXPECT_EQ(30, ac.getOffTimer());
  EXPECT_EQ(
      "Type: 1 (Command), Power: On, Mode: 3 (Heat), Celsius: On, "
      "Temp: 30C/86F, On Timer: Off, Off Timer: 00:30, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
  ac.setOffTimer(0);  // Disable / Timer off.
  EXPECT_FALSE(ac.isOffTimerEnabled());
  EXPECT_EQ(kMideaACTypeCommand, ac.getType());
  ac.setOffTimer(3 * 60 + 48);  // 3h48m
  EXPECT_TRUE(ac.isOffTimerEnabled());
  EXPECT_EQ(kMideaACTypeCommand, ac.getType());
  EXPECT_EQ(3 * 60 + 30, ac.getOffTimer());
  EXPECT_EQ(
      "Type: 1 (Command), Power: On, Mode: 3 (Heat), Celsius: On, "
      "Temp: 30C/86F, On Timer: Off, Off Timer: 03:30, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
  // Limit to max time.
  ac.setOffTimer(25 * 60);  // 25h
  EXPECT_TRUE(ac.isOffTimerEnabled());
  EXPECT_EQ(kMideaACTypeCommand, ac.getType());
  EXPECT_EQ(24 * 60, ac.getOffTimer());
  EXPECT_EQ(
      "Type: 1 (Command), Power: On, Mode: 3 (Heat), Celsius: On, "
      "Temp: 30C/86F, On Timer: Off, Off Timer: 24:00, Fan: 0 (Auto), "
      "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
      "Turbo Toggle: Off, Light Toggle: Off", ac.toString());
}

// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1318#issuecomment-725406813
TEST(TestMideaACClass, SendingSwingV) {
  IRMideaAC ac(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  ac.begin();
  ac.setSwingVToggle(true);
  ac.send();  // This should generate two messages back to back.
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&ac._irsend.capture));
  EXPECT_EQ(MIDEA, ac._irsend.capture.decode_type);
  EXPECT_EQ(kMideaBits, ac._irsend.capture.bits);
  EXPECT_EQ(
    "Type: 1 (Command), Power: On, Mode: 2 (Auto), Celsius: Off, "
    "Temp: 25C/77F, On Timer: Off, Off Timer: Off, Fan: 0 (Auto), "
    "Sleep: Off, Swing(V) Toggle: Off, Econo Toggle: Off, "
    "Turbo Toggle: Off, Light Toggle: Off",
    IRAcUtils::resultAcToString(&ac._irsend.capture));
  // Decode the second/following message which should be the swing toggle.
  EXPECT_TRUE(irrecv.decodeMidea(&ac._irsend.capture, 201));
  EXPECT_EQ(MIDEA, ac._irsend.capture.decode_type);
  EXPECT_EQ(kMideaBits, ac._irsend.capture.bits);
  EXPECT_EQ(
    "Type: 2 (Special), Swing(V) Toggle: On, Econo Toggle: Off, "
    "Turbo Toggle: Off, Light Toggle: Off",
    IRAcUtils::resultAcToString(&ac._irsend.capture));
}

// Reconstruct a Midea known raw code for an odd Pioneer System A/C.
TEST(TestMideaACClass, Issue1342) {
  IRMideaAC ac(kGpioUnused);

  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1342#issuecomment-733721085
  ASSERT_TRUE((1ULL<<37) & 0xA1A368FFFF45);  // Confirm the special bit is set.

  ac.setRaw(0xA1A368FFFF45);
  EXPECT_EQ(
    "Type: 1 (Command), Power: On, Mode: 3 (Heat), Celsius: Off, "
    "Temp: 21C/70F, On Timer: Off, Off Timer: Off, Fan: 0 (Auto), Sleep: Off, "
    "Swing(V) Toggle: Off, Econo Toggle: Off, Turbo Toggle: Off, "
    "Light Toggle: Off",
    ac.toString());
  ac.stateReset();  // Remove any existing info so we can build from scratch.
  ASSERT_NE(0xA1A368FFFF45, ac.getRaw());
  // Some Pioneer Systems have required a special bit to be set in order
  // for the A/C unit to accept the message. We don't currently understand what
  // this bit does. See the link for details of how to set this if needed.
  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1342#issuecomment-733721085
  ac.setRaw((1ULL<<37) | ac.getRaw());
  ASSERT_TRUE((1ULL<<37) & ac.getRaw());  // Confirm the special bit is set.

  ac.setPower(true);
  ac.setMode(kMideaACHeat);
  ac.setTemp(70);
  ac.setFan(kMideaACFanAuto);
  EXPECT_EQ(
    "Type: 1 (Command), Power: On, Mode: 3 (Heat), Celsius: Off, "
    "Temp: 21C/70F, On Timer: Off, Off Timer: Off, Fan: 0 (Auto), Sleep: Off, "
    "Swing(V) Toggle: Off, Econo Toggle: Off, Turbo Toggle: Off, "
    "Light Toggle: Off",
    ac.toString());
  EXPECT_EQ(0xA1A368FFFF45, ac.getRaw());
}
