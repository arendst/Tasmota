// Copyright 2023 Daniele Gobbetti

#include "ir_York.h"
#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"


// General housekeeping
TEST(TestYork, Housekeeping) {
  ASSERT_EQ("YORK", typeToString(decode_type_t::YORK));
  ASSERT_EQ(typeToString(decode_type_t::YORK), "YORK");
  ASSERT_TRUE(hasACState(YORK));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::YORK));
  ASSERT_EQ(kYorkBits, IRsend::defaultBits(decode_type_t::YORK));
}

// Tests for sendYork().
// Test sending typical data only.
TEST(TestSendYork, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();
  uint8_t data[kYorkStateLength] = {
    0x08, 0x10, 0x07, 0x02, 0x40, 0x08,
    0x03, 0x18, 0x01, 0x60, 0x00, 0x00, 0x00, 0x00,
    0xEC, 0xF5, 0xF2};
  irsend.reset();
  irsend.sendYork(data);
  EXPECT_EQ(
    "f38000d50"
    "m4887s2267m612s579m612s579m612s579m612s1778m612s579m612s579m612s579"
    "m612s579m612s579m612s579m612s579m612s579m612s1778m612s579m612s579"
    "m612s579m612s1778m612s1778m612s1778m612s579m612s579m612s579m612s579"
    "m612s579m612s579m612s1778m612s579m612s579m612s579m612s579m612s579m612s579"
    "m612s579m612s579m612s579m612s579m612s579m612s579m612s1778m612s579m612s579"
    "m612s579m612s579m612s1778m612s579m612s579m612s579m612s579m612s1778"
    "m612s1778m612s579m612s579m612s579m612s579m612s579m612s579m612s579m612s579"
    "m612s579m612s1778m612s1778m612s579m612s579m612s579m612s1778m612s579"
    "m612s579m612s579m612s579m612s579m612s579m612s579m612s579m612s579m612s579"
    "m612s579m612s579m612s1778m612s1778m612s579m612s579m612s579m612s579"
    "m612s579m612s579m612s579m612s579m612s579m612s579m612s579m612s579m612s579"
    "m612s579m612s579m612s579m612s579m612s579m612s579m612s579m612s579m612s579"
    "m612s579m612s579m612s579m612s579m612s579m612s579m612s579m612s579m612s579"
    "m612s579m612s579m612s579m612s579m612s1778m612s1778m612s579m612s1778"
    "m612s1778m612s1778m612s1778m612s579m612s1778m612s579m612s1778m612s1778"
    "m612s1778m612s1778m612s579m612s1778m612s579m612s579m612s1778m612s1778"
    "m612s1778m612s1778m612s100000",
      irsend.outputStr());

  irsend.reset();
}

// Decode normal York messages.
TEST(TestDecodeYork, SyntheticDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Synthesised Normal York message.
  irsend.reset();
  uint8_t kYorkKnownGoodState[kYorkStateLength] = {
    0x08, 0x10, 0x07, 0x02, 0x40, 0x08,
    0x03, 0x18, 0x01, 0x60, 0x00, 0x00, 0x00, 0x00,
    0xEC, 0xF5, 0xF2};
  irsend.sendYork(kYorkKnownGoodState);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(YORK, irsend.capture.decode_type);
  EXPECT_EQ(kYorkBits, irsend.capture.bits);
  EXPECT_STATE_EQ(kYorkKnownGoodState, irsend.capture.state,
                  irsend.capture.bits);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Heat), Fan: 8 (Auto), Temp: 24C, Swing(V): Off"
      ", On Timer: 00:00, Off Timer: 00:00",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

// test checksum calculation
TEST(TestYorkClass, SetAndGetTemp) {
  IRYorkAc ac(kGpioUnused);
  EXPECT_NE(23, ac.getTemp());  // default state is 24 deegrees Celsius
  ac.setTemp(23);
  EXPECT_EQ(23, ac.getTemp());

  uint8_t expectedState[kYorkStateLength] = {
    0x08, 0x10, 0x07, 0x02, 0x40, 0x08,
    0x03, 0x18, 0x01, 0x5C, 0x00, 0x00, 0x00, 0x00,
    0xEC, 0xA5, 0xF7};

  EXPECT_STATE_EQ(expectedState, ac.getRaw(), kYorkBits);
}
