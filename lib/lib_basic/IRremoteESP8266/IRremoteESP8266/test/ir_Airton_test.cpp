// Copyright 2021 crankyoldgit

#include "ir_Airton.h"
#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for decodeAirton().

TEST(TestDecodeAirton, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  const uint16_t rawData[115] = {
      6632, 3352,
      404, 1266, 404, 1264, 406, 430, 406, 430, 400, 1264, 406, 430, 402, 1264,
      408, 1262, 406, 1264, 404, 430, 402, 434, 402, 432, 402, 1264, 406, 430,
      404, 432, 400, 456, 376, 432, 402, 430, 402, 1264, 404, 1264, 404, 432,
      402, 434, 398, 434, 402, 430, 404, 1264, 404, 432, 402, 430, 404, 1264,
      406, 430, 402, 432, 400, 434, 402, 430, 402, 430, 404, 432, 402, 430,
      402, 432, 402, 432, 402, 430, 402, 432, 402, 430, 402, 434, 400, 432,
      402, 1264, 404, 430, 404, 1264, 404, 432, 402, 454, 378, 432, 402, 430,
      404, 1264, 404, 1264, 404, 1264, 378, 1292, 404, 432, 402, 1264, 404, 432,
      402};
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 115, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::AIRTON, irsend.capture.decode_type);
  ASSERT_EQ(kAirtonBits, irsend.capture.bits);
  EXPECT_EQ(0x5E1400090C11D3, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_EQ(
      "Power: On, Mode: 4 (Heat), Fan: 0 (Auto), Temp: 25C, "
      "Swing(V): Off, Econo: Off, Turbo: Off, Light: Off, "
      "Health: Off, Sleep: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

TEST(TestDecodeAirton, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  irsend.reset();
  irsend.sendAirton(0x5E1400090C11D3);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::AIRTON, irsend.capture.decode_type);
  ASSERT_EQ(kAirtonBits, irsend.capture.bits);
  EXPECT_EQ(0x5E1400090C11D3, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("AIRTON", typeToString(decode_type_t::AIRTON));
  ASSERT_EQ(decode_type_t::AIRTON, strToDecodeType("AIRTON"));
  ASSERT_FALSE(hasACState(decode_type_t::AIRTON));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::AIRTON));
  ASSERT_EQ(kAirtonBits, IRsend::defaultBits(decode_type_t::AIRTON));
  ASSERT_EQ(kAirtonDefaultRepeat, IRsend::minRepeats(decode_type_t::AIRTON));
}

// Tests for IRAirtonAc class.

TEST(TestIRAirtonAcClass, Power) {
  IRAirtonAc ac(kGpioUnused);
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

TEST(TestIRAirtonAcClass, Checksums) {
  ASSERT_TRUE(IRAirtonAc::validChecksum(0x5E1400090C11D3));
  ASSERT_EQ(0x5E, IRAirtonAc::calcChecksum(0x5E1400090C11D3));
  ASSERT_FALSE(IRAirtonAc::validChecksum(0x551400090C11D3));
  ASSERT_TRUE(IRAirtonAc::validChecksum(0x2F8801060911D3));
  ASSERT_TRUE(IRAirtonAc::validChecksum(0xDB8800021A11D3));
}

TEST(TestIRAirtonAcClass, Temperature) {
  IRAirtonAc ac(kGpioUnused);
  ac.begin();
  ac.setMode(kAirtonCool);  // Cool mode allows the entire temp range.
  ac.setTemp(0);
  EXPECT_EQ(kAirtonMinTemp, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kAirtonMaxTemp, ac.getTemp());

  ac.setTemp(kAirtonMinTemp);
  EXPECT_EQ(kAirtonMinTemp, ac.getTemp());

  ac.setTemp(kAirtonMaxTemp);
  EXPECT_EQ(kAirtonMaxTemp, ac.getTemp());

  ac.setTemp(kAirtonMinTemp - 1);
  EXPECT_EQ(kAirtonMinTemp, ac.getTemp());

  ac.setTemp(kAirtonMaxTemp + 1);
  EXPECT_EQ(kAirtonMaxTemp, ac.getTemp());

  ac.setTemp(17);
  EXPECT_EQ(17, ac.getTemp());

  ac.setTemp(21);
  EXPECT_EQ(21, ac.getTemp());

  ac.setTemp(20);
  EXPECT_EQ(20, ac.getTemp());
}

TEST(TestIRAirtonAcClass, OperatingMode) {
  IRAirtonAc ac(kGpioUnused);
  ac.begin();

  ac.setMode(kAirtonCool);
  EXPECT_EQ(kAirtonCool, ac.getMode());
  ac.setMode(kAirtonDry);
  EXPECT_EQ(kAirtonDry, ac.getMode());
  ac.setMode(kAirtonFan);
  EXPECT_EQ(kAirtonFan, ac.getMode());
  EXPECT_NE(kAirtonMaxTemp, ac.getTemp());
  ac.setMode(kAirtonAuto);
  EXPECT_EQ(kAirtonAuto, ac.getMode());
  EXPECT_EQ(kAirtonMaxTemp, ac.getTemp());
  ac.setMode(kAirtonHeat);
  EXPECT_EQ(kAirtonHeat, ac.getMode());

  ac.setMode(kAirtonHeat + 1);
  EXPECT_EQ(kAirtonAuto, ac.getMode());
  ac.setMode(255);
  EXPECT_EQ(kAirtonAuto, ac.getMode());
}

TEST(TestIRAirtonAcClass, FanSpeed) {
  IRAirtonAc ac(kGpioUnused);
  ac.begin();
  ac.setMode(kAirtonCool);  // All fan speeds available in this mode.

  ac.setFan(0);
  EXPECT_EQ(kAirtonFanAuto, ac.getFan());

  ac.setFan(255);
  EXPECT_EQ(kAirtonFanAuto, ac.getFan());

  ac.setFan(kAirtonFanHigh);
  EXPECT_EQ(kAirtonFanHigh, ac.getFan());

  ac.setFan(kAirtonFanLow);
  EXPECT_EQ(kAirtonFanLow, ac.getFan());

  ac.setFan(kAirtonFanMax);
  EXPECT_EQ(kAirtonFanMax, ac.getFan());

  ac.setFan(kAirtonFanMin);
  EXPECT_EQ(kAirtonFanMin, ac.getFan());

  ac.setFan(kAirtonFanMed);
  EXPECT_EQ(kAirtonFanMed, ac.getFan());

  ac.setFan(kAirtonFanMax + 1);
  EXPECT_EQ(kAirtonFanAuto, ac.getFan());
}

TEST(TestIRAirtonAcClass, SwingV) {
  IRAirtonAc ac(kGpioUnused);
  ac.begin();

  ac.setSwingV(false);
  EXPECT_FALSE(ac.getSwingV());
  ac.setSwingV(true);
  EXPECT_TRUE(ac.getSwingV());
  ac.setSwingV(false);
  EXPECT_FALSE(ac.getSwingV());

  // Known swingv on state
  ac.setRaw(0xBC0401050111D3);
  EXPECT_TRUE(ac.getSwingV());
}

TEST(TestIRAirtonAcClass, Light) {
  IRAirtonAc ac(kGpioUnused);
  ac.begin();

  ac.setLight(false);
  EXPECT_FALSE(ac.getLight());
  ac.setLight(true);
  EXPECT_TRUE(ac.getLight());
  ac.setLight(false);
  EXPECT_FALSE(ac.getLight());

  // Known light on state
  ac.setRaw(0x298801040911D3);
  EXPECT_TRUE(ac.getLight());
}

TEST(TestIRAirtonAcClass, ConstructKnownExamples) {
  IRAirtonAc ac(kGpioUnused);
  ac.begin();
  ac.stateReset();
  ac.on();
  ac.setMode(kAirtonHeat);
  ac.setFan(kAirtonFanAuto);
  ac.setTemp(25);
  ac.setSwingV(false);
  ac.setLight(false);
  ac.setTurbo(false);
  ac.setSleep(false);
  ac.setEcono(false);
  ac.setHealth(false);
  EXPECT_EQ(
      "Power: On, Mode: 4 (Heat), Fan: 0 (Auto), Temp: 25C, "
      "Swing(V): Off, Econo: Off, Turbo: Off, Light: Off, "
      "Health: Off, Sleep: Off",
      ac.toString());
  EXPECT_EQ(0x5E1400090C11D3, ac.getRaw());
}

TEST(TestIRAirtonAcClass, Turbo) {
  IRAirtonAc ac(kGpioUnused);
  ac.begin();

  ac.setTurbo(false);
  EXPECT_FALSE(ac.getTurbo());
  EXPECT_NE(kAirtonFanMax, ac.getFan());
  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());
  EXPECT_EQ(kAirtonFanMax, ac.getFan());
  ac.setTurbo(false);
  EXPECT_FALSE(ac.getTurbo());

  // Known Turbo on state
  ac.setRaw(0x92040000D911D3);
  EXPECT_TRUE(ac.getTurbo());
}

TEST(TestIRAirtonAcClass, Sleep) {
  IRAirtonAc ac(kGpioUnused);
  ac.begin();

  ac.setMode(kAirtonCool);  // Sleep is available in Cool mode.
  ac.setSleep(false);
  EXPECT_FALSE(ac.getSleep());
  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
  ac.setSleep(false);
  EXPECT_FALSE(ac.getSleep());

  ac.setSleep(true);
  // Sleep is available in Heat mode, but changing modes resets it.
  ac.setMode(kAirtonHeat);
  EXPECT_FALSE(ac.getSleep());
  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());

  ac.setMode(kAirtonAuto);  // Sleep is NOT available in Auto mode.
  EXPECT_FALSE(ac.getSleep());
  ac.setSleep(true);
  EXPECT_FALSE(ac.getSleep());

  ac.setMode(kAirtonFan);  // Sleep is NOT available in Fan mode.
  EXPECT_FALSE(ac.getSleep());
  ac.setSleep(true);
  EXPECT_FALSE(ac.getSleep());

  // Known Sleep on state
  ac.setRaw(0xA00600000911D3);
  EXPECT_TRUE(ac.getSleep());
  EXPECT_NE(kAirtonAuto, ac.getMode());
  EXPECT_NE(kAirtonFan, ac.getMode());
}

TEST(TestIRAirtonAcClass, Health) {
  IRAirtonAc ac(kGpioUnused);
  ac.begin();

  ac.setHealth(false);
  EXPECT_FALSE(ac.getHealth());
  ac.setHealth(true);
  EXPECT_TRUE(ac.getHealth());
  ac.setHealth(false);
  EXPECT_FALSE(ac.getHealth());

  // Known Health on state
  ac.setRaw(0xE5C900000911D3);
  EXPECT_TRUE(ac.getHealth());
}

TEST(TestIRAirtonAcClass, Econo) {
  IRAirtonAc ac(kGpioUnused);
  ac.begin();
  ac.setMode(kAirtonCool);  // Econo is only available in Cool.
  ac.setEcono(false);
  EXPECT_FALSE(ac.getEcono());
  ac.setEcono(true);
  EXPECT_TRUE(ac.getEcono());
  ac.setEcono(false);
  EXPECT_FALSE(ac.getEcono());

  ac.setEcono(true);
  ac.setMode(kAirtonHeat);  // Econo is only available in Cool, not Heat!
  EXPECT_FALSE(ac.getEcono());
  ac.setEcono(true);
  EXPECT_FALSE(ac.getEcono());

  // Known Econo on state
  ac.setRaw(0xE5C900000911D3);
  EXPECT_TRUE(ac.getEcono());
}
