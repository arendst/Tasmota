// Copyright 2019 David Conran

#include "ir_Teco.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// General housekeeping
TEST(TestTeco, Housekeeping) {
  ASSERT_EQ("TECO", typeToString(TECO));
  ASSERT_FALSE(hasACState(TECO));  // Uses uint64_t, not uint8_t*.
}

// Tests for sendTeco()

// Test sending typical data only.
TEST(TestSendTeco, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();

  irsend.reset();
  irsend.sendTeco(0x250002BC9);
  EXPECT_EQ(
      "f38000d50"
      "m9000s4440"
      "m620s1650m620s580m620s580m620s1650m620s580m620s580m620s1650m620s1650"
      "m620s1650m620s1650m620s580m620s1650m620s580m620s1650m620s580m620s580"
      "m620s580m620s580m620s580m620s580m620s580m620s580m620s580m620s580"
      "m620s580m620s580m620s580m620s580m620s1650m620s580m620s1650m620s580"
      "m620s580m620s1650m620s580"
      "m620s100000",
      irsend.outputStr());
}

// Test sending typical data with repeats.
TEST(TestSendTeco, SendWithRepeats) {
  IRsendTest irsend(0);
  irsend.begin();

  irsend.reset();
  irsend.sendTeco(0x250002BC9, kTecoBits, 2);  // two repeats.
  EXPECT_EQ(
      "f38000d50"
      "m9000s4440"
      "m620s1650m620s580m620s580m620s1650m620s580m620s580m620s1650m620s1650"
      "m620s1650m620s1650m620s580m620s1650m620s580m620s1650m620s580m620s580"
      "m620s580m620s580m620s580m620s580m620s580m620s580m620s580m620s580"
      "m620s580m620s580m620s580m620s580m620s1650m620s580m620s1650m620s580"
      "m620s580m620s1650m620s580"
      "m620s100000"
      "m9000s4440"
      "m620s1650m620s580m620s580m620s1650m620s580m620s580m620s1650m620s1650"
      "m620s1650m620s1650m620s580m620s1650m620s580m620s1650m620s580m620s580"
      "m620s580m620s580m620s580m620s580m620s580m620s580m620s580m620s580"
      "m620s580m620s580m620s580m620s580m620s1650m620s580m620s1650m620s580"
      "m620s580m620s1650m620s580"
      "m620s100000"
      "m9000s4440"
      "m620s1650m620s580m620s580m620s1650m620s580m620s580m620s1650m620s1650"
      "m620s1650m620s1650m620s580m620s1650m620s580m620s1650m620s580m620s580"
      "m620s580m620s580m620s580m620s580m620s580m620s580m620s580m620s580"
      "m620s580m620s580m620s580m620s580m620s1650m620s580m620s1650m620s580"
      "m620s580m620s1650m620s580"
      "m620s100000",
      irsend.outputStr());
}


// Tests for IRTeco class.

TEST(TestTecoACClass, Power) {
  IRTecoAc ac(0);
  ac.begin();

  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());

  ac.setPower(false);
  EXPECT_EQ(false, ac.getPower());

  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());

  ac.off();
  EXPECT_EQ(false, ac.getPower());

  ac.on();
  EXPECT_TRUE(ac.getPower());
}

TEST(TestTecoACClass, OperatingMode) {
  IRTecoAc ac(0);
  ac.begin();

  ac.setMode(kTecoAuto);
  EXPECT_EQ(kTecoAuto, ac.getMode());

  ac.setMode(kTecoCool);
  EXPECT_EQ(kTecoCool, ac.getMode());

  ac.setMode(kTecoHeat);
  EXPECT_EQ(kTecoHeat, ac.getMode());

  ac.setMode(kTecoFan);
  EXPECT_EQ(kTecoFan, ac.getMode());

  ac.setMode(kTecoDry);
  EXPECT_EQ(kTecoDry, ac.getMode());

  ac.setMode(kTecoAuto - 1);
  EXPECT_EQ(kTecoAuto, ac.getMode());

  ac.setMode(kTecoCool);
  EXPECT_EQ(kTecoCool, ac.getMode());

  ac.setMode(kTecoHeat + 1);
  EXPECT_EQ(kTecoAuto, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kTecoAuto, ac.getMode());
}

TEST(TestTecoACClass, Temperature) {
  IRTecoAc ac(0);
  ac.begin();

  ac.setTemp(kTecoMinTemp);
  EXPECT_EQ(kTecoMinTemp, ac.getTemp());

  ac.setTemp(kTecoMinTemp + 1);
  EXPECT_EQ(kTecoMinTemp + 1, ac.getTemp());

  ac.setTemp(kTecoMaxTemp);
  EXPECT_EQ(kTecoMaxTemp, ac.getTemp());

  ac.setTemp(kTecoMinTemp - 1);
  EXPECT_EQ(kTecoMinTemp, ac.getTemp());

  ac.setTemp(kTecoMaxTemp + 1);
  EXPECT_EQ(kTecoMaxTemp, ac.getTemp());

  ac.setTemp(23);
  EXPECT_EQ(23, ac.getTemp());

  ac.setTemp(0);
  EXPECT_EQ(kTecoMinTemp, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kTecoMaxTemp, ac.getTemp());
}

TEST(TestTecoACClass, FanSpeed) {
  IRTecoAc ac(0);
  ac.begin();
  ac.setFan(kTecoFanLow);

  ac.setFan(kTecoFanAuto);
  EXPECT_EQ(kTecoFanAuto, ac.getFan());

  ac.setFan(kTecoFanLow);
  EXPECT_EQ(kTecoFanLow, ac.getFan());
  ac.setFan(kTecoFanMed);
  EXPECT_EQ(kTecoFanMed, ac.getFan());
  ac.setFan(kTecoFanHigh);
  EXPECT_EQ(kTecoFanHigh, ac.getFan());

  ac.setFan(kTecoFanHigh);
  EXPECT_EQ(kTecoFanHigh, ac.getFan());
}

TEST(TestTecoACClass, Swing) {
  IRTecoAc ac(0);
  ac.begin();

  ac.setSwing(true);
  EXPECT_TRUE(ac.getSwing());

  ac.setSwing(false);
  EXPECT_EQ(false, ac.getSwing());

  ac.setSwing(true);
  EXPECT_TRUE(ac.getSwing());
}

TEST(TestTecoACClass, Sleep) {
  IRTecoAc ac(0);
  ac.begin();

  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());

  ac.setSleep(false);
  EXPECT_EQ(false, ac.getSleep());

  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
}

TEST(TestTecoACClass, Light) {
  IRTecoAc ac(0);
  ac.begin();

  ac.setLight(true);
  EXPECT_TRUE(ac.getLight());
  ac.setLight(false);
  EXPECT_EQ(false, ac.getLight());
  ac.setLight(true);
  EXPECT_TRUE(ac.getLight());
  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/870#issue-484797174
  ac.setRaw(0x250200A09);
  EXPECT_TRUE(ac.getLight());
}

TEST(TestTecoACClass, Humid) {
  IRTecoAc ac(0);
  ac.begin();

  ac.setHumid(true);
  EXPECT_TRUE(ac.getHumid());
  ac.setHumid(false);
  EXPECT_EQ(false, ac.getHumid());
  ac.setHumid(true);
  EXPECT_TRUE(ac.getHumid());
  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/870#issuecomment-524536810
  ac.setRaw(0x250100A09);
  EXPECT_TRUE(ac.getHumid());
}

TEST(TestTecoACClass, Save) {
  IRTecoAc ac(0);
  ac.begin();

  ac.setSave(true);
  EXPECT_TRUE(ac.getSave());
  ac.setSave(false);
  EXPECT_EQ(false, ac.getSave());
  ac.setSave(true);
  EXPECT_TRUE(ac.getSave());
  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/870#issuecomment-524536810
  ac.setRaw(0x250800A09);
  EXPECT_TRUE(ac.getSave());
}

// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/882
TEST(TestTecoACClass, Timer) {
  IRTecoAc ac(0);
  ac.begin();

  ac.setTimer(60);
  EXPECT_TRUE(ac.getTimerEnabled());
  EXPECT_EQ(60, ac.getTimer());
  ac.setTimer(0);
  EXPECT_EQ(false, ac.getTimerEnabled());
  EXPECT_EQ(0, ac.getTimer());
  ac.setTimer(17 * 60 + 59);
  EXPECT_TRUE(ac.getTimerEnabled());
  EXPECT_EQ(17 * 60 + 30, ac.getTimer());
  ac.setTimer(24 * 60 + 31);
  EXPECT_TRUE(ac.getTimerEnabled());
  EXPECT_EQ(24 * 60, ac.getTimer());

  // Data from: https://github.com/crankyoldgit/IRremoteESP8266/issues/882#issuecomment-527079339
  ac.setRaw(0x250218A49);  // Timer On 1hr
  EXPECT_TRUE(ac.getTimerEnabled());
  EXPECT_EQ(60, ac.getTimer());
  ac.setRaw(0x250219A49);  // Timer On 1.5hr
  EXPECT_TRUE(ac.getTimerEnabled());
  EXPECT_EQ(60 + 30, ac.getTimer());
  ac.setRaw(0x250200A49);  // Timer Off
  EXPECT_FALSE(ac.getTimerEnabled());
  EXPECT_EQ(0, ac.getTimer());
  ac.setRaw(0x25023DA41);  // Timer On 23.5hrs
  EXPECT_TRUE(ac.getTimerEnabled());
  EXPECT_EQ(23 * 60 + 30, ac.getTimer());
}

TEST(TestTecoACClass, MessageConstuction) {
  IRTecoAc ac(0);

  EXPECT_EQ(
      "Power: Off, Mode: 0 (Auto), Temp: 16C, Fan: 0 (Auto), Sleep: Off, "
      "Swing: Off, Light: Off, Humid: Off, Save: Off, Timer: Off",
      ac.toString());
  ac.setPower(true);
  ac.setMode(kTecoCool);
  ac.setTemp(21);
  ac.setFan(kTecoFanHigh);
  ac.setSwing(false);
  ac.setLight(false);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 21C, Fan: 3 (High), Sleep: Off, "
      "Swing: Off, Light: Off, Humid: Off, Save: Off, Timer: Off",
      ac.toString());
  ac.setSwing(true);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 21C, Fan: 3 (High), Sleep: Off, "
      "Swing: On, Light: Off, Humid: Off, Save: Off, Timer: Off",
      ac.toString());
  ac.setSwing(false);
  ac.setFan(kTecoFanLow);
  ac.setSleep(true);
  ac.setMode(kTecoHeat);
  EXPECT_EQ(
      "Power: On, Mode: 4 (Heat), Temp: 21C, Fan: 1 (Low), Sleep: On, "
      "Swing: Off, Light: Off, Humid: Off, Save: Off, Timer: Off",
      ac.toString());
  ac.setSleep(false);
  EXPECT_EQ(
      "Power: On, Mode: 4 (Heat), Temp: 21C, Fan: 1 (Low), Sleep: Off, "
      "Swing: Off, Light: Off, Humid: Off, Save: Off, Timer: Off",
      ac.toString());
  ac.setTemp(25);
  ac.setLight(true);
  ac.setSave(true);
  ac.setHumid(true);
  ac.setTimer(18 * 60 + 37);
  EXPECT_EQ(
      "Power: On, Mode: 4 (Heat), Temp: 25C, Fan: 1 (Low), Sleep: Off, "
      "Swing: Off, Light: On, Humid: On, Save: On, Timer: 18:30",
      ac.toString());
}

TEST(TestTecoACClass, ReconstructKnownMessage) {
  IRTecoAc ac(0);

  const uint64_t expected = 0x250002BC9;
  ASSERT_FALSE(ac.getRaw() == expected);
  ac.setPower(true);
  ac.setMode(kTecoCool);
  ac.setTemp(27);
  ac.setFan(kTecoFanAuto);
  ac.setSleep(true);
  ac.setSwing(true);
  EXPECT_EQ(expected, ac.getRaw());
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 27C, Fan: 0 (Auto), Sleep: On, "
      "Swing: On, Light: Off, Humid: Off, Save: Off, Timer: Off",
      ac.toString());
}

// Tests for decodeTeco().

// Decode normal "synthetic" messages.
TEST(TestDecodeTeco, NormalDecodeWithStrict) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // With the specific decoder.
  uint64_t expectedState = kTecoReset;
  irsend.reset();
  irsend.sendTeco(expectedState);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeTeco(&irsend.capture, kStartOffset, kTecoBits,
                                true));
  EXPECT_EQ(TECO, irsend.capture.decode_type);
  EXPECT_EQ(kTecoBits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_EQ(expectedState, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);

  // With the all the decoders.
  irsend.reset();
  irsend.sendTeco(expectedState);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(TECO, irsend.capture.decode_type);
  EXPECT_EQ(kTecoBits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_EQ(expectedState, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);

  IRTecoAc ac(0);
  ac.begin();
  ac.setRaw(irsend.capture.value);
  EXPECT_EQ(
      "Power: Off, Mode: 0 (Auto), Temp: 16C, Fan: 0 (Auto), Sleep: Off, "
      "Swing: Off, Light: Off, Humid: Off, Save: Off, Timer: Off",
      ac.toString());
}

// Decode a real message from Raw Data.
TEST(TestDecodeTeco, RealNormalExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  IRTecoAc ac(0);
  irsend.begin();

  uint16_t rawData1[73] = {
      9076, 4442,  670, 1620,  670, 516,  670, 516,  666, 1626,  670, 516,
      664, 520,  666, 1626,  666, 1626,  664, 1626,  666, 1626,  666, 520,
      666, 1626,  666, 520,  666, 1626,  666, 520,  666, 516,  670, 514,
      670, 516,  666, 520,  670, 516,  666, 520,  666, 516,  672, 514,  670,
      516,  666, 520,  666, 516,  672, 514,  670, 516,  666, 1624,  666, 520,
      666, 1626,  666, 520,  666, 516,  672, 1620,  670, 516,  670};
  uint64_t expected1 = 0b01001010000000000000010101111001001;  // 0x250002BC9
  irsend.reset();
  irsend.sendRaw(rawData1, 73, 38);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(TECO, irsend.capture.decode_type);
  EXPECT_EQ(kTecoBits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_EQ(expected1, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);
  ac.begin();
  ac.setRaw(irsend.capture.value);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 27C, Fan: 0 (Auto), Sleep: On, "
      "Swing: On, Light: Off, Humid: Off, Save: Off, Timer: Off",
      ac.toString());

  uint16_t rawData2[73] = {
    9048, 4472, 636, 548, 636, 1654, 638, 546, 642, 1650, 642, 546, 638,
    1654, 638, 1654, 638, 546, 638, 1654, 636, 546, 642, 1650, 640, 548,
    636, 548, 638, 546, 636, 546, 642, 542, 642, 546, 638, 546, 638, 546,
    636, 548, 642, 542, 642, 546, 636, 548, 636, 546, 642, 542, 642, 546,
    638, 546, 638, 546, 636, 1654, 642, 542, 642, 1650, 642, 546, 638, 546,
    638, 1654, 638, 546, 642};  // TECO 25000056A
  uint64_t expected2 = 0b01001010000000000000000010101101010;  // 0x25000056A
  irsend.reset();
  irsend.sendRaw(rawData2, 73, 38);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(TECO, irsend.capture.decode_type);
  EXPECT_EQ(kTecoBits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_EQ(expected2, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);
  ac.begin();
  ac.setRaw(irsend.capture.value);
  EXPECT_EQ(
      "Power: On, Mode: 2 (Dry), Temp: 21C, Fan: 2 (Medium), Sleep: Off, "
      "Swing: On, Light: Off, Humid: Off, Save: Off, Timer: Off",
      ac.toString());
}


TEST(TestTecoACClass, toCommon) {
  IRTecoAc ac(0);
  ac.setPower(true);
  ac.setMode(kTecoCool);
  ac.setTemp(20);
  ac.setFan(kTecoFanHigh);
  ac.setSwing(true);
  ac.setSleep(true);
  // Now test it.
  ASSERT_EQ(decode_type_t::TECO, ac.toCommon().protocol);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kAuto, ac.toCommon().swingv);
  ASSERT_EQ(0, ac.toCommon().sleep);
  // Unsupported.
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_EQ(-1, ac.toCommon().clock);
}
