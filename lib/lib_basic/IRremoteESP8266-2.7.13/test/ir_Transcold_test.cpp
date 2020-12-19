// Copyright 2020 crankyoldgit

#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for decodeTranscold().

TEST(TestDecodeTranscold, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1256#issuecomment-683608224
  const uint16_t rawData[101] = {
      5944, 7612,
      558, 3556, 556, 3556, 556, 3556, 556, 1526, 554, 3556, 556, 1528,
      554, 1526, 556, 3558, 554, 1524, 556, 1528, 556, 1526, 556, 3556,
      554, 1528, 556, 3556, 554, 3556, 556, 1528, 554, 1526, 556, 3556,
      556, 3556, 554, 1528, 554, 1526, 554, 3558, 554, 1528, 554, 3556,
      556, 3556, 556, 1526, 554, 1526, 556, 3556, 554, 3556, 554, 1526,
      554, 3556, 556, 1526, 556, 1526, 554, 3558, 554, 1526, 556, 3556,
      556, 1526, 556, 3554, 556, 1524, 556, 1526, 556, 3556, 556, 1526,
      554, 3556, 556, 1524, 558, 3556, 554, 1526, 556, 3556, 554, 3556,
      556, 7514,
      556};  // UNKNOWN C38A8243

  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 101, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::TRANSCOLD, irsend.capture.decode_type);
  ASSERT_EQ(kTranscoldBits, irsend.capture.bits);
  EXPECT_EQ(0xE96554, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(
      "Power: On, Mode: 6 (Cool), Fan: 9 (Min), Temp: 22C",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

TEST(TestDecodeTranscold, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  irsend.reset();
  irsend.sendTranscold(0xE96554);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::TRANSCOLD, irsend.capture.decode_type);
  EXPECT_EQ(kTranscoldBits, irsend.capture.bits);
  EXPECT_EQ(0xE96554, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_EQ(0x0, irsend.capture.address);

  EXPECT_EQ(
    "f38000d50"
    "m5944s7563"
    "m555s3556m555s3556m555s3556m555s1526m555s3556m555s1526m555s1526m555s3556"
    "m555s1526m555s1526m555s1526m555s3556m555s1526m555s3556m555s3556m555s1526"
    "m555s1526m555s3556m555s3556m555s1526m555s1526m555s3556m555s1526m555s3556"
    "m555s3556m555s1526m555s1526m555s3556m555s3556m555s1526m555s3556m555s1526"
    "m555s1526m555s3556m555s1526m555s3556m555s1526m555s3556m555s1526m555s1526"
    "m555s3556m555s1526m555s3556m555s1526m555s3556m555s1526m555s3556m555s3556"
    "m555s7563"
    "m555s100000",
    irsend.outputStr());
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("TRANSCOLD", typeToString(decode_type_t::TRANSCOLD));
  ASSERT_EQ(decode_type_t::TRANSCOLD, strToDecodeType("TRANSCOLD"));
  ASSERT_FALSE(hasACState(decode_type_t::TRANSCOLD));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::TRANSCOLD));
  ASSERT_EQ(kTranscoldBits, IRsend::defaultBits(decode_type_t::TRANSCOLD));
  ASSERT_EQ(kTranscoldDefaultRepeat,
            IRsend::minRepeats(decode_type_t::TRANSCOLD));
}

// Tests for the IRTranscoldAc class.
TEST(TestTranscoldAcClass, SetAndGetRaw) {
  IRTranscoldAc ac(kGpioUnused);

  ac.setRaw(0xB21F28);
  EXPECT_EQ(0xB21F28, ac.getRaw());
  ac.setRaw(kTranscoldKnownGoodState);
  EXPECT_EQ(kTranscoldKnownGoodState, ac.getRaw());
}

TEST(TestTranscoldAcClass, SetAndGetTemp) {
  IRTranscoldAc ac(kGpioUnused);

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());
  ac.setTemp(kTranscoldTempMin);
  EXPECT_EQ(kTranscoldTempMin, ac.getTemp());
  ac.setTemp(kTranscoldTempMax);
  EXPECT_EQ(kTranscoldTempMax, ac.getTemp());
  ac.setTemp(kTranscoldTempMin - 1);
  EXPECT_EQ(kTranscoldTempMin, ac.getTemp());
  ac.setTemp(kTranscoldTempMax + 1);
  EXPECT_EQ(kTranscoldTempMax, ac.getTemp());
}

TEST(TestTranscoldAcClass, SetAndGetMode) {
  IRTranscoldAc ac(kGpioUnused);

  ac.setMode(kTranscoldHeat);
  EXPECT_EQ(kTranscoldHeat, ac.getMode());
  ac.setMode(kTranscoldCool);
  EXPECT_EQ(kTranscoldCool, ac.getMode());
  ac.setMode(kTranscoldDry);
  EXPECT_EQ(kTranscoldDry, ac.getMode());
  ac.setMode(kTranscoldAuto);
  EXPECT_EQ(kTranscoldAuto, ac.getMode());
  ac.setMode(kTranscoldFan);
  EXPECT_EQ(kTranscoldFan, ac.getMode());
}

TEST(TestTranscoldAcClass, SetAndGetFan) {
  IRTranscoldAc ac(kGpioUnused);

  // This mode allows pretty much everything except Auto0 speed.
  ac.setMode(kTranscoldCool);
  ac.setFan(kTranscoldFanMax);
  EXPECT_EQ(kTranscoldFanMax, ac.getFan());
  ac.setFan(kTranscoldFanMin);
  EXPECT_EQ(kTranscoldFanMin, ac.getFan());
  ac.setFan(kTranscoldFanZoneFollow);
  EXPECT_EQ(kTranscoldFanZoneFollow, ac.getFan());
  ac.setFan(kTranscoldFanAuto);
  EXPECT_EQ(kTranscoldFanAuto, ac.getFan());
  ac.setFan(kTranscoldFanAuto0);
  EXPECT_EQ(kTranscoldFanAuto, ac.getFan());
  ac.setFan(kTranscoldFanMax);
  EXPECT_EQ(kTranscoldFanMax, ac.getFan());
  ASSERT_NE(3, kTranscoldFanAuto);
  // Now try some unexpected value.
  ac.setFan(3);
  EXPECT_EQ(kTranscoldFanAuto, ac.getFan());

  // These modes allows pretty much everything except Auto speed.
  ac.setMode(kTranscoldDry);
  EXPECT_EQ(kTranscoldFanAuto0, ac.getFan());
  ac.setFan(kTranscoldFanMax);
  EXPECT_EQ(kTranscoldFanMax, ac.getFan());
  ac.setFan(kTranscoldFanAuto);
  EXPECT_EQ(kTranscoldFanAuto0, ac.getFan());

  ac.setMode(kTranscoldAuto);
  EXPECT_EQ(kTranscoldFanAuto0, ac.getFan());
  ac.setFan(kTranscoldFanMax);
  EXPECT_EQ(kTranscoldFanMax, ac.getFan());
  ac.setFan(kTranscoldFanAuto0);
  EXPECT_EQ(kTranscoldFanAuto0, ac.getFan());
}

TEST(TestTranscoldAcClass, SpecialModesAndReset) {
  IRTranscoldAc ac(kGpioUnused);
  ASSERT_NE(kTranscoldSwing, ac.getRaw());
  ac.setSwing();
  ASSERT_EQ(kTranscoldSwing, ac.getRaw());
  ac.stateReset();
  ASSERT_NE(kTranscoldSwing, ac.getRaw());
}

TEST(TestTranscoldAcClass, HumanReadable) {
  IRTranscoldAc ac(kGpioUnused);
  ac.begin();

  // Initial starting point.
  EXPECT_EQ(
      "Power: Off", ac.toString());
  ac.setPower(true);
  EXPECT_EQ(
      "Power: On, Mode: 6 (Cool), Fan: 9 (Min), Temp: 22C",
      ac.toString());
  ac.setMode(kTranscoldHeat);
  ac.setFan(kTranscoldFanMin);
  ac.setTemp(22);
  EXPECT_EQ(
      "Power: On, Mode: 10 (Heat), Fan: 9 (Min), Temp: 22C",
      ac.toString());
  ac.setSwing();
  EXPECT_EQ("Power: On, Swing: Toggle", ac.toString());
  ac.setPower(false);
  EXPECT_EQ("Power: Off", ac.toString());
}

TEST(TestTranscoldAcClass, BuildKnownState) {
  IRTranscoldAc ac(kGpioUnused);
  // "temp down, 19, Auto, cool, close (right)"
  // Data from:
  //   https://docs.google.com/spreadsheets/d/1qdoyB0FyJm85HPP9oXcfui0n4ztXBFlik6kiNlkO2IM/edit#gid=694351627&range=A25:F25
  const uint32_t state = 0xEF6B54;
  ac.stateReset();
  ac.on();
  ac.setMode(kTranscoldCool);
  ac.setFan(kTranscoldFanAuto);
  ac.setTemp(19);
  EXPECT_EQ("Power: On, Mode: 6 (Cool), Fan: 15 (Auto), Temp: 19C",
            ac.toString());
  ASSERT_EQ(state, ac.getRaw());
}
