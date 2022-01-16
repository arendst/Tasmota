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
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  uint8_t toshiba_code[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                                 0x00, 0x00, 0x00, 0x00};
  irsend.reset();
  irsend.sendToshibaAC(toshiba_code);
  EXPECT_EQ(
      "f38000d50"
      "m4400s4300"
      "m580s1600m580s1600m580s1600m580s1600m580s490m580s490m580s1600m580s490"
      "m580s490m580s490m580s490m580s490m580s1600m580s1600m580s490m580s1600"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s1600m580s1600"
      "m580s1600m580s1600m580s1600m580s1600m580s1600m580s1600m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s1600"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s7400"
      "m4400s4300"
      "m580s1600m580s1600m580s1600m580s1600m580s490m580s490m580s1600m580s490"
      "m580s490m580s490m580s490m580s490m580s1600m580s1600m580s490m580s1600"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s1600m580s1600"
      "m580s1600m580s1600m580s1600m580s1600m580s1600m580s1600m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s1600"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s7400",
      irsend.outputStr());
}

// Test sending with repeats.
TEST(TestSendToshibaAC, SendWithRepeats) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  irsend.reset();
  uint8_t toshiba_code[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                                 0x00, 0x00, 0x00, 0x00};

  irsend.sendToshibaAC(toshiba_code, kToshibaACStateLength, 0);
  EXPECT_EQ(
      "f38000d50"
      "m4400s4300"
      "m580s1600m580s1600m580s1600m580s1600m580s490m580s490m580s1600m580s490"
      "m580s490m580s490m580s490m580s490m580s1600m580s1600m580s490m580s1600"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s1600m580s1600"
      "m580s1600m580s1600m580s1600m580s1600m580s1600m580s1600m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s1600"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s7400",
      irsend.outputStr());

  irsend.reset();
  irsend.sendToshibaAC(toshiba_code, kToshibaACStateLength, 2);
  EXPECT_EQ(
      "f38000d50"
      "m4400s4300"
      "m580s1600m580s1600m580s1600m580s1600m580s490m580s490m580s1600m580s490"
      "m580s490m580s490m580s490m580s490m580s1600m580s1600m580s490m580s1600"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s1600m580s1600"
      "m580s1600m580s1600m580s1600m580s1600m580s1600m580s1600m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s1600"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s7400"
      "m4400s4300"
      "m580s1600m580s1600m580s1600m580s1600m580s490m580s490m580s1600m580s490"
      "m580s490m580s490m580s490m580s490m580s1600m580s1600m580s490m580s1600"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s1600m580s1600"
      "m580s1600m580s1600m580s1600m580s1600m580s1600m580s1600m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s1600"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s7400"
      "m4400s4300"
      "m580s1600m580s1600m580s1600m580s1600m580s490m580s490m580s1600m580s490"
      "m580s490m580s490m580s490m580s490m580s1600m580s1600m580s490m580s1600"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s1600m580s1600"
      "m580s1600m580s1600m580s1600m580s1600m580s1600m580s1600m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s1600"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s7400",
      irsend.outputStr());
}

// Tests for IRToshibaAC class.

TEST(TestToshibaACClass, Power) {
  IRToshibaAC ac(kGpioUnused);
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

TEST(TestToshibaACClass, Temperature) {
  IRToshibaAC ac(kGpioUnused);
  ac.begin();

  ac.setTemp(0);
  EXPECT_EQ(kToshibaAcMinTemp, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kToshibaAcMaxTemp, ac.getTemp());

  ac.setTemp(kToshibaAcMinTemp);
  EXPECT_EQ(kToshibaAcMinTemp, ac.getTemp());

  ac.setTemp(kToshibaAcMaxTemp);
  EXPECT_EQ(kToshibaAcMaxTemp, ac.getTemp());

  ac.setTemp(kToshibaAcMinTemp - 1);
  EXPECT_EQ(kToshibaAcMinTemp, ac.getTemp());

  ac.setTemp(kToshibaAcMaxTemp + 1);
  EXPECT_EQ(kToshibaAcMaxTemp, ac.getTemp());

  ac.setTemp(17);
  EXPECT_EQ(17, ac.getTemp());

  ac.setTemp(21);
  EXPECT_EQ(21, ac.getTemp());

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());

  ac.setTemp(30);
  EXPECT_EQ(30, ac.getTemp());
}

TEST(TestToshibaACClass, OperatingMode) {
  IRToshibaAC ac(kGpioUnused);
  ac.begin();

  ac.setMode(kToshibaAcAuto);
  EXPECT_EQ(kToshibaAcAuto, ac.getMode());

  ac.setMode(kToshibaAcCool);
  EXPECT_EQ(kToshibaAcCool, ac.getMode());

  ac.setMode(kToshibaAcHeat);
  EXPECT_EQ(kToshibaAcHeat, ac.getMode());

  ac.setMode(kToshibaAcDry);
  EXPECT_EQ(kToshibaAcDry, ac.getMode());

  ac.setMode(kToshibaAcFan);
  EXPECT_EQ(kToshibaAcFan, ac.getMode());

  ac.setMode(kToshibaAcFan + 1);
  EXPECT_EQ(kToshibaAcAuto, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kToshibaAcAuto, ac.getMode());

  // Setting the power off changes the underlying mode in the state to a special
  // off mode.
  ac.setPower(true);
  ac.setMode(kToshibaAcCool);
  EXPECT_EQ(kToshibaAcCool, ac.getMode());
  ac.setPower(false);
  EXPECT_EQ(kToshibaAcCool, ac.getMode());
}

TEST(TestToshibaACClass, FanSpeed) {
  IRToshibaAC ac(kGpioUnused);
  ac.begin();

  ac.setFan(kToshibaAcFanAuto);
  EXPECT_EQ(kToshibaAcFanAuto, ac.getFan());

  ac.setFan(255);
  EXPECT_EQ(kToshibaAcFanMax, ac.getFan());

  ac.setFan(kToshibaAcFanMax);
  EXPECT_EQ(kToshibaAcFanMax, ac.getFan());

  ac.setFan(kToshibaAcFanMax - 1);
  EXPECT_EQ(kToshibaAcFanMax - 1, ac.getFan());

  ac.setFan(1);
  EXPECT_EQ(1, ac.getFan());

  ac.setFan(2);
  EXPECT_EQ(2, ac.getFan());

  ac.setFan(3);
  EXPECT_EQ(3, ac.getFan());

  ac.setFan(4);
  EXPECT_EQ(4, ac.getFan());

  ac.setFan(kToshibaAcFanMax + 1);
  EXPECT_EQ(kToshibaAcFanMax, ac.getFan());
}

TEST(TestToshibaACClass, RawState) {
  IRToshibaAC ac(kGpioUnused);
  ac.begin();

  uint8_t initial_state[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                                  0x50, 0x00, 0x00, 0x51};
  uint8_t modified_state[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                                   0x00, 0xC1, 0x00, 0xC0};

  // Verify the starting state.
  EXPECT_STATE_EQ(initial_state, ac.getRaw(), kToshibaACBits);
  EXPECT_TRUE(ac.getPower());
  EXPECT_EQ(kToshibaAcAuto, ac.getMode());
  EXPECT_EQ(kToshibaAcFanAuto, ac.getFan());

  // Change some settings.
  ac.setMode(kToshibaAcCool);
  ac.setFan(kToshibaAcFanMax);
  ac.setTemp(kToshibaAcMinTemp);
  // Verify those were set.
  EXPECT_EQ(kToshibaAcCool, ac.getMode());
  EXPECT_EQ(kToshibaAcFanMax, ac.getFan());
  EXPECT_EQ(kToshibaAcMinTemp, ac.getTemp());
  // Retrieve the modified state.
  EXPECT_STATE_EQ(modified_state, ac.getRaw(), kToshibaACBits);

  // Set it back to the initial state.
  ac.setRaw(initial_state);

  // Check the new state was set correctly.
  EXPECT_TRUE(ac.getPower());
  EXPECT_EQ(kToshibaAcAuto, ac.getMode());
  EXPECT_EQ(kToshibaAcFanAuto, ac.getFan());
  EXPECT_STATE_EQ(initial_state, ac.getRaw(), kToshibaACBits);
}

TEST(TestToshibaACClass, Checksums) {
  IRToshibaAC ac(kGpioUnused);
  ac.begin();

  uint8_t initial_state[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                                  0x00, 0x00, 0x00, 0x01};
  uint8_t modified_state[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                                   0x00, 0xC1, 0x00, 0xC0};
  uint8_t invalid_state[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                                  0x00, 0x00, 0x00, 0x00};

  EXPECT_EQ(0x01, ac.calcChecksum(initial_state));
  EXPECT_EQ(0xC0, ac.calcChecksum(modified_state));
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
  IRToshibaAC ac(kGpioUnused);
  ac.begin();

  uint8_t initial_state[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                                  0x00, 0x00, 0x00, 0x01};
  uint8_t modified_state[kToshibaACStateLength] = {0xF2, 0x0D, 0x03, 0xFC, 0x01,
                                                   0x00, 0xC1, 0x00, 0xC0};

  ac.setRaw(initial_state);
  EXPECT_EQ("Temp: 17C, Power: On, Mode: 0 (Auto), Fan: 0 (Auto), "
            "Turbo: Off, Econo: Off, Filter: Off",
            ac.toString());
  ac.setRaw(modified_state);
  EXPECT_EQ("Temp: 17C, Power: On, Mode: 1 (Cool), Fan: 5 (High), "
            "Turbo: Off, Econo: Off, Filter: Off",
            ac.toString());
  ac.setTemp(25);
  ac.setFan(3);
  ac.setMode(kToshibaAcDry);
  EXPECT_EQ("Temp: 25C, Power: On, Mode: 2 (Dry), Fan: 3 (Medium), "
            "Turbo: Off, Econo: Off, Filter: Off",
            ac.toString());
  ac.off();
  EXPECT_EQ("Temp: 25C, Power: Off, Fan: 3 (Medium), Turbo: Off, Econo: Off, "
            "Filter: Off",
            ac.toString());
}

TEST(TestToshibaACClass, MessageConstuction) {
  IRToshibaAC ac(kGpioUnused);
  ac.begin();

  ac.on();
  ac.setFan(1);
  ac.setMode(kToshibaAcCool);
  ac.setTemp(27);

  // Check everything for kicks.
  EXPECT_EQ(1, ac.getFan());
  EXPECT_EQ(kToshibaAcCool, ac.getMode());
  EXPECT_EQ(27, ac.getTemp());
  EXPECT_TRUE(ac.getPower());

  // Turn off the power and re-check.
  ac.setPower(false);
  // Check everything for kicks.
  EXPECT_EQ(1, ac.getFan());
  EXPECT_EQ(kToshibaAcCool, ac.getMode());
  EXPECT_EQ(27, ac.getTemp());
  EXPECT_FALSE(ac.getPower());

  // Turn the power back on, and check nothing changed.
  ac.on();

  EXPECT_EQ(1, ac.getFan());
  EXPECT_EQ(kToshibaAcCool, ac.getMode());
  EXPECT_EQ(27, ac.getTemp());
  EXPECT_TRUE(ac.getPower());
}

// Decoding a message we entirely constructed based solely on a given state.
TEST(TestDecodeToshibaAC, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
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
      "Temp: 17C, Power: On, Mode: 0 (Auto), Fan: 0 (Auto), Turbo: Off, "
      "Econo: Off, Filter: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

// Test decoding against captures from a real Toshiba A/C remote.
// Recorded by @mwildbolz
TEST(TestDecodeToshibaAC, RealExamples) {
  IRToshibaAC ac(kGpioUnused);
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
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
  ac.setRaw(irsend.capture.state, irsend.capture.bits / 8);
  EXPECT_TRUE(ac.getPower());
  EXPECT_EQ(23, ac.getTemp());
  EXPECT_EQ(kToshibaAcFanAuto, ac.getFan());
  EXPECT_EQ(kToshibaAcAuto, ac.getMode());

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
  ac.setRaw(irsend.capture.state, irsend.capture.bits / 8);
  EXPECT_TRUE(ac.getPower());
  EXPECT_EQ(17, ac.getTemp());
  EXPECT_EQ(3, ac.getFan());
  EXPECT_EQ(kToshibaAcCool, ac.getMode());

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
  ac.setRaw(irsend.capture.state, irsend.capture.bits / 8);
  EXPECT_TRUE(ac.getPower());
  EXPECT_EQ(24, ac.getTemp());
  EXPECT_EQ(kToshibaAcFanMax, ac.getFan());
  EXPECT_EQ(kToshibaAcHeat, ac.getMode());

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
  ac.setRaw(irsend.capture.state, irsend.capture.bits / 8);
  EXPECT_FALSE(ac.getPower());
  EXPECT_EQ(22, ac.getTemp());
  EXPECT_EQ(4, ac.getFan());
}

TEST(TestToshibaACClass, toCommon) {
  IRToshibaAC ac(kGpioUnused);
  ac.setPower(true);
  ac.setMode(kToshibaAcCool);
  ac.setTemp(20);
  ac.setFan(kToshibaAcFanMax);
  ac.setFilter(true);
  // Now test it.
  ASSERT_EQ(decode_type_t::TOSHIBA_AC, ac.toCommon().protocol);
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_TRUE(ac.toCommon().filter);
  // Unsupported.
  ASSERT_EQ(stdAc::swingv_t::kOff, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

// Tests for CarrierAc2
/// Decode a "real" long example message.
TEST(TestDecodeToshibaAC, RealLongExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  // Data from:
  //  https://github.com/crankyoldgit/IRremoteESP8266/issues/1205#issuecomment-650475434
  const uint16_t high_power_on[327] = {
    4424, 4320,
    582, 1574, 588, 1578, 582, 1574, 586, 1578, 586, 496, 582, 492, 586, 1576,
    586, 492, 586, 492, 588, 496, 584, 496, 584, 496, 584, 1626, 534, 1626,
    534, 494, 586, 1578, 582, 494, 586, 494, 586, 494, 588, 492, 586, 492,
    586, 1576, 586, 494, 588, 492, 588, 1574, 588, 1576, 584, 1578, 584, 1574,
    588, 1574, 588, 492, 588, 1572, 590, 1570, 590, 492, 588, 492, 590, 488,
    590, 494, 584, 1570, 592, 492, 586, 490, 590, 1572, 590, 490, 590, 1570,
    590, 490, 590, 1570, 590, 492, 588, 490, 588, 492, 588, 492, 590, 490,
    590, 494, 586, 490, 590, 490, 588, 490, 590, 490, 588, 492, 590, 490,
    588, 492, 590, 490, 590, 490, 590, 494, 584, 490, 590, 490, 590, 490,
    590, 490, 588, 490, 588, 492, 588, 492, 586, 492, 588, 490, 588, 492,
    588, 490, 590, 1572, 588, 494, 586, 1574, 588, 492, 588, 1572, 590, 1572,
    588, 492, 588, 492, 586, 494, 588,
    7422,
    4424, 4320,
    586, 1572, 588, 1572, 588, 1576, 584, 1574, 588, 494, 586, 492, 588, 1572,
    588, 492, 588, 492, 588, 492, 588, 494, 586, 496, 584, 1574, 586, 1578,
    582, 494, 586, 1578, 584, 494, 586, 492, 588, 492, 586, 496, 584, 494,
    586, 1578, 584, 494, 586, 494, 584, 1574, 588, 1572, 586, 1574, 588, 1574,
    588, 1572, 588, 494, 590, 1572, 588, 1574, 588, 492, 588, 492, 588, 492,
    586, 492, 588, 1572, 588, 498, 582, 492, 588, 1576, 586, 492, 588, 1572,
    588, 494, 588, 1572, 588, 492, 586, 492, 588, 492, 590, 490, 588, 492,
    586, 492, 588, 492, 590, 490, 588, 490, 588, 492, 590, 490, 588, 492,
    590, 490, 588, 490, 590, 490, 590, 490, 592, 488, 592, 494, 584, 494,
    586, 490, 590, 494, 586, 494, 588, 488, 592, 490, 588, 492, 586, 490,
    592, 490, 588, 1576, 584, 494, 586, 1570, 590, 494, 586, 1576, 582, 1572,
    590, 490, 590, 490, 588, 490, 590};  // UNKNOWN 54926187

  const uint8_t expectedState[kToshibaACStateLengthLong] = {
        0xF2, 0x0D, 0x04, 0xFB, 0x09, 0x50, 0x00, 0x00, 0x01, 0x58};
  irsend.sendRaw(high_power_on, 327, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::TOSHIBA_AC, irsend.capture.decode_type);
  EXPECT_EQ(kToshibaACBitsLong, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Temp: 22C, Power: On, Mode: 0 (Auto), Fan: 0 (Auto), Turbo: On, "
      "Econo: Off, Filter: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
}


/// Decode a synthetic long message.
TEST(TestDecodeToshibaAC, SyntheticLongExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);

  const uint8_t expectedState[kToshibaACStateLengthLong] = {
        0xF2, 0x0D, 0x04, 0xFB, 0x09, 0x50, 0x00, 0x00, 0x01, 0x58};
  irsend.begin();
  irsend.reset();
  irsend.sendToshibaAC(expectedState, kToshibaACStateLengthLong);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::TOSHIBA_AC, irsend.capture.decode_type);
  EXPECT_EQ(kToshibaACBitsLong, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "f38000d50"
    //  4424 4320
      "m4400s4300"
    //  582 1574 588 1578 582 1574 586 1578 586 496 582 492 586 1576 586 492
      "m580s1600m580s1600m580s1600m580s1600m580s490m580s490m580s1600m580s490"
    //  586 492 588 496 584 496 584 496 584 1626 534 1626 534 494 586 1578
      "m580s490m580s490m580s490m580s490m580s1600m580s1600m580s490m580s1600"
    //  582 494 586 494 586 494 588 492 586 492 586 1576 586 494 588 492
      "m580s490m580s490m580s490m580s490m580s490m580s1600m580s490m580s490"
    //  588 1574 588 1576 584 1578 584 1574 588 1574 588 492 588 1572 590 1570
      "m580s1600m580s1600m580s1600m580s1600m580s1600m580s490m580s1600m580s1600"
    //  590 492 588 492 590 488 590 494 584 1570 592 492 586 490 590 1572
      "m580s490m580s490m580s490m580s490m580s1600m580s490m580s490m580s1600"
    //  590 490 590 1570 590 490 590 1570 590 492 588 490 588 492 588 492
      "m580s490m580s1600m580s490m580s1600m580s490m580s490m580s490m580s490"
    //  590 490 590 494 586 490 590 490 588 490 590 490 588 492 590 490
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
    //  588 492 590 490 590 490 590 494 584 490 590 490 590 490 590 490
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
    //  588 490 588 492 588 492 586 492 588 490 588 492 588 490 590 1572
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s1600"
    //  588 494 586 1574 588 492 588 1572 590 1572 588 492 588 492 586 494
      "m580s490m580s1600m580s490m580s1600m580s1600m580s490m580s490m580s490"
    //  588 7422
      "m580s7400"
      "m4400s4300"
      "m580s1600m580s1600m580s1600m580s1600m580s490m580s490m580s1600m580s490"
      "m580s490m580s490m580s490m580s490m580s1600m580s1600m580s490m580s1600"
      "m580s490m580s490m580s490m580s490m580s490m580s1600m580s490m580s490"
      "m580s1600m580s1600m580s1600m580s1600m580s1600m580s490m580s1600m580s1600"
      "m580s490m580s490m580s490m580s490m580s1600m580s490m580s490m580s1600"
      "m580s490m580s1600m580s490m580s1600m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s1600"
      "m580s490m580s1600m580s490m580s1600m580s1600m580s490m580s490m580s490"
      "m580s7400",
      irsend.outputStr());
}

/// Decode a "real" short example message.
TEST(TestDecodeToshibaAC, RealShortExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  // Data from:
  //  https://github.com/crankyoldgit/IRremoteESP8266/issues/1205#issuecomment-650475096
  const uint16_t air_direction[115] = {
      4424, 4318,
      588, 1574, 588, 1572, 588, 1574, 586, 1572, 590, 490, 586, 494, 586, 1574,
      588, 492, 586, 494, 586, 494, 586, 496, 584, 492, 588, 1572, 588, 1572,
      590, 492, 588, 1572, 590, 490, 588, 492, 586, 494, 588, 492, 588, 492,
      588, 494, 584, 492, 588, 1572, 588, 1574, 588, 1574, 588, 1572, 588, 1572,
      588, 1572, 588, 1574, 590, 1570, 588, 494, 586, 496, 584, 494, 588, 1572,
      588, 492, 588, 492, 588, 490, 588, 492, 590, 1572, 588, 492, 588, 496,
      586, 492, 588, 492, 586, 492, 588, 492, 588, 490, 590, 490, 588, 492,
      588, 490, 588, 1572, 588, 492, 588, 492, 588, 490, 588, 492, 588, 1572,
      586};  // UNKNOWN DEB8845C
  const uint8_t expectedState[kToshibaACStateLengthShort] = {
        0xF2, 0x0D, 0x01, 0xFE, 0x21, 0x00, 0x21};
  irsend.sendRaw(air_direction, 115, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::TOSHIBA_AC, irsend.capture.decode_type);
  EXPECT_EQ(kToshibaACBitsShort, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Temp: 17C, Swing(V): 0 (Step)",
      IRAcUtils::resultAcToString(&irsend.capture));
}

TEST(TestToshibaACClass, ConstructLongState) {
  IRToshibaAC ac(kGpioUnused);
  ac.setPower(true);
  ac.setMode(kToshibaAcDry);
  ac.setTemp(29);
  ac.setFan(2);
  ac.setSwing(false);
  ac.setTurbo(false);
  ac.setEcono(true);
  EXPECT_EQ(
      "Temp: 29C, Power: On, Mode: 2 (Dry), Fan: 2 (UNKNOWN), "
      "Turbo: Off, Econo: On, Filter: Off",
      ac.toString());
  EXPECT_EQ(kToshibaACStateLengthLong, ac.getStateLength());
  const uint8_t expectedState[kToshibaACStateLengthLong] = {
        0xF2, 0x0D, 0x04, 0xFB, 0x09, 0xC0, 0x62, 0x00, 0x03, 0xA8};
  EXPECT_STATE_EQ(expectedState, ac.getRaw(), kToshibaACBitsLong);
  EXPECT_EQ(kToshibaACStateLengthLong, ac.getStateLength());
}

// Decode a real WH-UB03NJ message.
TEST(TestDecodeToshibaAC, RealExample_WHUB03NJ) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  // Data from:
  //  https://github.com/crankyoldgit/IRremoteESP8266/issues/1420#issue-817430242
  const uint16_t rawData[295] = {4438, 4364, 558, 1604, 558, 1580, 580, 1604,
      558, 1580, 582, 522, 556, 500, 580, 1582, 580, 500, 580, 498, 582, 498,
      582, 498, 580, 500, 582, 1582, 580, 1582, 582, 500, 582, 1580, 582, 520,
      558, 522, 558, 496, 582, 524, 560, 520, 556, 500, 582, 1604, 558, 1584,
      580, 1582, 580, 1582, 582, 1580, 582, 1580, 582, 1580, 582, 1582, 580,
      522, 556, 524, 556, 500, 580, 522, 560, 498, 582, 522, 558, 500, 554, 524,
      582, 498, 580, 1582, 580, 500, 582, 496, 582, 1582, 582, 1604, 558, 524,
      558, 522, 560, 498, 580, 500, 582, 498, 582, 522, 558, 498, 582, 520, 558,
      500, 528, 1634, 580, 1584, 578, 1582, 582, 496, 584, 496, 582, 498, 582,
      498, 582, 498, 582, 500, 582, 522, 558, 498, 584, 498, 582, 496, 582,
      1580, 582, 1604, 558, 500, 582, 1580, 554, 1608, 580, 500, 580,
      4630, 4416, 4316, 606, 1604, 558, 1580, 580, 1606, 556, 1604, 558, 500,
      580, 498, 554, 1606, 582, 498, 582, 498, 584, 496, 584, 498, 582, 522,
      558, 1586, 556, 1608, 580, 498, 584, 1580, 580, 500, 582, 498, 582, 500,
      582, 498, 580, 498, 582, 522, 558, 1580, 554, 1610, 580, 1604, 532, 1606,
      582, 1604, 558, 1604, 560, 1582, 580, 1582, 554, 548, 560, 498, 582, 500,
      580, 500, 580, 500, 582, 520, 558, 500, 580, 524, 558, 498, 582, 1582,
      582, 498, 580, 522, 532, 1608, 582, 1604, 558, 498, 582, 522, 556, 500,
      580, 500, 582, 498, 582, 522, 560, 496, 582, 500, 582, 498, 582, 1604,
      558, 1604, 558, 1604, 560, 496, 582, 522, 558, 500, 580, 500, 582, 500,
      554, 524, 582, 500, 580, 500, 582, 498, 554, 524, 582, 1604, 558, 1582,
      582, 496, 582, 1582, 580, 1604, 558, 500, 580};  // TOSHIBA_AC

  const uint8_t expectedState[kToshibaACStateLength] = {
      0xF2, 0x0D, 0x03, 0xFC, 0x01, 0x30, 0x07, 0x00, 0x36};
  irsend.sendRaw(rawData, 295, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::TOSHIBA_AC, irsend.capture.decode_type);
  EXPECT_EQ(kToshibaACBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Temp: 20C, Power: Off, Fan: 0 (Auto), Turbo: Off, Econo: Off, "
      "Filter: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("TOSHIBA_AC", typeToString(decode_type_t::TOSHIBA_AC));
  ASSERT_EQ(decode_type_t::TOSHIBA_AC, strToDecodeType("TOSHIBA_AC"));
  ASSERT_TRUE(hasACState(decode_type_t::TOSHIBA_AC));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::TOSHIBA_AC));
  ASSERT_EQ(kToshibaACBits, IRsend::defaultBits(decode_type_t::TOSHIBA_AC));
  ASSERT_EQ(kSingleRepeat, IRsend::minRepeats(decode_type_t::TOSHIBA_AC));
}

// For https://github.com/crankyoldgit/IRremoteESP8266/issues/1423
TEST(TestToshibaACClass, SwingCodes) {
  IRToshibaAC ac(kGpioUnused);
  ac.setStateLength(kToshibaACStateLengthShort);
  ac.setTemp(kToshibaAcMinTemp);
  ac.setSwing(kToshibaAcSwingOn);

  EXPECT_EQ(
      "Temp: 17C, Swing(V): 1 (On)",
      ac.toString());
  EXPECT_EQ(kToshibaACStateLengthShort, ac.getStateLength());
  const uint8_t swingOnState[kToshibaACStateLengthShort] = {
        0xF2, 0x0D, 0x01, 0xFE, 0x21, 0x01, 0x20};
  EXPECT_STATE_EQ(swingOnState, ac.getRaw(), kToshibaACBitsShort);
  EXPECT_EQ(kToshibaACStateLengthShort, ac.getStateLength());

  ac.setSwing(kToshibaAcSwingOff);
  EXPECT_EQ(
      "Temp: 17C, Swing(V): 2 (Off)",
      ac.toString());
  EXPECT_EQ(kToshibaACStateLengthShort, ac.getStateLength());
  const uint8_t swingOffState[kToshibaACStateLengthShort] = {
        0xF2, 0x0D, 0x01, 0xFE, 0x21, 0x02, 0x23};
  EXPECT_STATE_EQ(swingOffState, ac.getRaw(), kToshibaACBitsShort);
  EXPECT_EQ(kToshibaACStateLengthShort, ac.getStateLength());

  const uint8_t swingToggleState[kToshibaACStateLengthShort] = {
        0xF2, 0x0D, 0x01, 0xFE, 0x21, 0x04, 0x25};
  ac.setRaw(swingToggleState, kToshibaACStateLengthShort);
  EXPECT_EQ(kToshibaAcSwingToggle, ac.getSwing());
  EXPECT_EQ(
      "Temp: 17C, Swing(V): 4 (Toggle)",
      ac.toString());
}

// For https://github.com/crankyoldgit/IRremoteESP8266/issues/1692
TEST(TestToshibaACClass, Filter) {
  IRToshibaAC ac(kGpioUnused);
  ac.begin();
  EXPECT_FALSE(ac.getFilter());

  ac.setFilter(true);
  EXPECT_TRUE(ac.getFilter());


  ac.setFilter(false);
  EXPECT_FALSE(ac.getFilter());

  ac.setFilter(true);
  EXPECT_TRUE(ac.getFilter());

  const uint8_t pure_off[kToshibaACStateLength] = {
      0xF2, 0x0D, 0x03, 0xFC, 0x01, 0x40, 0x03, 0x00, 0x42};
  ac.setRaw(pure_off);
  EXPECT_FALSE(ac.getFilter());

  const uint8_t pure_on[kToshibaACStateLength] = {
      0xF2, 0x0D, 0x03, 0xFC, 0x01, 0x40, 0x03, 0x10, 0x52};
  ac.setRaw(pure_on);
  EXPECT_TRUE(ac.getFilter());

  // Convert a known filter/pure on state to a known off filter/pure state.
  ac.setFilter(false);
  EXPECT_STATE_EQ(pure_off, ac.getRaw(), ac.getStateLength() * 8);
}
