// Copyright 2021 David Conran (crankyoldgit)

#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for decodeTruma().

TEST(TestDecodeTruma, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  //  16 / AUTO / -
  const uint16_t rawData[117] = {
      20194, 1028, 1798, 628, 558, 662, 1190, 640,
      1190, 638, 1190, 634, 1190, 638, 1188, 640,
      1164, 662, 558, 658, 1192, 636, 1166, 662,
      1190, 638, 1164, 658, 1190, 638, 582, 640,
      556, 662, 558, 660, 1188, 638, 558, 662,
      558, 662, 1190, 634, 1188, 638, 556, 664,
      558, 662, 556, 660, 556, 664, 558, 662,
      556, 664, 556, 662, 556, 664, 558, 662,
      558, 662, 558, 660, 556, 664, 556, 664,
      556, 664, 556, 660, 558, 664, 578, 640,
      580, 640, 556, 660, 556, 664, 556, 664,
      556, 664, 580, 636, 556, 664, 556, 664,
      556, 664, 556, 660, 556, 664, 1188, 638,
      1188, 640, 554, 662, 1188, 640, 1188, 638,
      554, 666, 1164, 654, 582};
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 117, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::TRUMA, irsend.capture.decode_type);
  ASSERT_EQ(kTrumaBits, irsend.capture.bits);
  EXPECT_EQ(0x49ffffffe6e081, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_EQ(
      "Power: On, Mode: 0 (Auto), Temp: 16C, Fan: 4 (High), Quiet: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

TEST(TestDecodeTruma, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  irsend.reset();

  //  16 / AUTO / -
  irsend.sendTruma(0x49ffffffe6e081);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::TRUMA, irsend.capture.decode_type);
  EXPECT_EQ(kTrumaBits, irsend.capture.bits);
  EXPECT_EQ(0x49ffffffe6e081, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  EXPECT_EQ(
    "f38000d50"
    "m20200s1000"
    "m1800s630"
    "m600s630m1200s630m1200s630m1200s630m1200s630m1200s630m1200s630m600s630"
    "m1200s630m1200s630m1200s630m1200s630m1200s630m600s630m600s630m600s630"
    "m1200s630m600s630m600s630m1200s630m1200s630m600s630m600s630m600s630"
    "m600s630m600s630m600s630m600s630m600s630m600s630m600s630m600s630"
    "m600s630m600s630m600s630m600s630m600s630m600s630m600s630m600s630"
    "m600s630m600s630m600s630m600s630m600s630m600s630m600s630m600s630"
    "m600s630m1200s630m1200s630m600s630m1200s630m1200s630m600s630m1200s630"
    "m600s100000",
    irsend.outputStr());
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("TRUMA", typeToString(decode_type_t::TRUMA));
  ASSERT_EQ(decode_type_t::TRUMA, strToDecodeType("TRUMA"));
  ASSERT_FALSE(hasACState(decode_type_t::TRUMA));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::TRUMA));
  ASSERT_EQ(kTrumaBits, IRsend::defaultBits(decode_type_t::TRUMA));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::TRUMA));
}

// Tests for IRTrumaAc class.

TEST(TestTrumaAcClass, Power) {
  IRTrumaAc ac(kGpioUnused);
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

TEST(TestTrumaAcClass, toCommon) {
  IRTrumaAc ac(kGpioUnused);
  ac.setPower(true);
  ac.setMode(kTrumaCool);
  ac.setTemp(20);
  ac.setFan(kTrumaFanHigh);
  ac.setQuiet(false);
  // Now test it.
  ASSERT_EQ(decode_type_t::TRUMA, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kHigh, ac.toCommon().fanspeed);
  ASSERT_FALSE(ac.toCommon().quiet);
  // Unsupported.
  ASSERT_EQ(stdAc::swingv_t::kOff, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

TEST(TestTrumaAcClass, HumanReadableOutput) {
  IRTrumaAc ac(kGpioUnused);
  ac.begin();

  // Data from https://docs.google.com/spreadsheets/d/1k-RHu0vSIB6IweiTZSa3Rxy3Z_qPUtqwcqot8uXVO6I/edit#gid=0
  const uint64_t on_25_auto = 0x52FFFFFFEFE081;
  const uint64_t on_17_cool_med = 0x54FFFFFFE7EA81;

  ac.setRaw(on_25_auto);
  EXPECT_EQ("Power: On, Mode: 0 (Auto), Temp: 25C, Fan: 4 (High), Quiet: Off",
            ac.toString());
  ac.setRaw(on_17_cool_med);
  EXPECT_EQ("Power: On, Mode: 2 (Cool), Temp: 17C, Fan: 5 (Medium), Quiet: Off",
            ac.toString());
  ac.setTemp(25);
  ac.setFan(kTrumaFanLow);
  ac.setMode(kTrumaFan);
  EXPECT_EQ("Power: On, Mode: 3 (Fan), Temp: 25C, Fan: 6 (Low), Quiet: Off",
            ac.toString());
  ac.off();
  EXPECT_EQ("Power: Off, Temp: 25C, Fan: 6 (Low), Quiet: Off",
            ac.toString());
}

TEST(TestTrumaAcClass, FanSpeed) {
  IRTrumaAc ac(kGpioUnused);
  ac.begin();
  ac.on();
  ac.setMode(kTrumaCool);  // Cool allows all options.
  ac.setFan(kTrumaFanLow);
  EXPECT_EQ(kTrumaFanLow, ac.getFan());

  ac.setFan(0);
  EXPECT_EQ(kTrumaFanHigh, ac.getFan());
  ac.setFan(255);
  EXPECT_EQ(kTrumaFanHigh, ac.getFan());

  ac.setFan(kTrumaFanMed);
  EXPECT_EQ(kTrumaFanMed, ac.getFan());

  ac.setFan(kTrumaFanHigh);
  EXPECT_EQ(kTrumaFanHigh, ac.getFan());

  ac.setFan(kTrumaFanQuiet);
  EXPECT_EQ(kTrumaFanQuiet, ac.getFan());

  ac.setMode(kTrumaAuto);  // Quiet should not be available/an option now.
  EXPECT_NE(kTrumaFanQuiet, ac.getFan());
  ac.setFan(kTrumaFanQuiet);
  EXPECT_NE(kTrumaFanQuiet, ac.getFan());
}

TEST(TestTrumaAcClass, OperatingMode) {
  IRTrumaAc ac(kGpioUnused);
  ac.begin();

  ac.on();  // Power on state allows all modes.
  ac.setMode(kTrumaAuto);
  EXPECT_EQ(kTrumaAuto, ac.getMode());

  ac.setMode(kTrumaCool);
  EXPECT_EQ(kTrumaCool, ac.getMode());

  ac.setMode(kTrumaFan);
  EXPECT_EQ(kTrumaFan, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kTrumaAuto, ac.getMode());

  ac.off();  // Off, only allows the fan mode.
  EXPECT_EQ(kTrumaFan, ac.getMode());
  ac.setMode(kTrumaCool);
  EXPECT_EQ(kTrumaFan, ac.getMode());
}

TEST(TestTrumaAcClass, Temperature) {
  IRTrumaAc ac(kGpioUnused);
  ac.begin();

  ac.setTemp(0);
  EXPECT_EQ(kTrumaMinTemp, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kTrumaMaxTemp, ac.getTemp());

  ac.setTemp(kTrumaMinTemp);
  EXPECT_EQ(kTrumaMinTemp, ac.getTemp());

  ac.setTemp(kTrumaMaxTemp);
  EXPECT_EQ(kTrumaMaxTemp, ac.getTemp());

  ac.setTemp(kTrumaMinTemp - 1);
  EXPECT_EQ(kTrumaMinTemp, ac.getTemp());

  ac.setTemp(kTrumaMaxTemp + 1);
  EXPECT_EQ(kTrumaMaxTemp, ac.getTemp());

  ac.setTemp(17);
  EXPECT_EQ(17, ac.getTemp());

  ac.setTemp(21);
  EXPECT_EQ(21, ac.getTemp());

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());

  ac.setTemp(30);
  EXPECT_EQ(30, ac.getTemp());
}

TEST(TestTrumaAcClass, Checksums) {
  IRTrumaAc ac(kGpioUnused);
  ac.begin();

  uint64_t valid = 0x52FFFFFFEFE081;
  uint64_t invalid = 0x51FFFFFFEFE081;

  EXPECT_EQ(0x52, ac.calcChecksum(valid));
  EXPECT_EQ(0x52, ac.calcChecksum(invalid));
  // Check we can call it without instantiating the object.
  EXPECT_EQ(0x52, IRTrumaAc::calcChecksum(valid));

  EXPECT_TRUE(IRTrumaAc::validChecksum(valid));
  EXPECT_FALSE(IRTrumaAc::validChecksum(invalid));

  ac.setRaw(invalid);
  EXPECT_EQ(valid, ac.getRaw());
}

TEST(TestTrumaAcClass, KnownMessageConstuction) {
  IRTrumaAc ac(kGpioUnused);
  ac.begin();

  const uint64_t on_17_cool_med = 0x54FFFFFFE7EA81;
  ac.on();
  ac.setMode(kTrumaCool);
  ac.setFan(kTrumaFanMed);
  ac.setTemp(17);
  ac.setQuiet(false);
  EXPECT_EQ(on_17_cool_med, ac.getRaw());

  const uint64_t off_16_auto = 0x50FFFFFFE6E781;  // 4DFFFFFFE6E481
  ac.setMode(kTrumaAuto);
  ac.setTemp(16);
  ac.setFan(kTrumaFanHigh);
  ac.off();
  EXPECT_EQ(off_16_auto, ac.getRaw());
  EXPECT_EQ("Power: Off, Temp: 16C, Fan: 4 (High), Quiet: Off", ac.toString());
}
