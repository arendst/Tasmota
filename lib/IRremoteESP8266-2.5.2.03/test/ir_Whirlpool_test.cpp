// Copyright 2018 David Conran

#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendWhirlpoolAC().

// Test sending typical data only.
TEST(TestSendWhirlpoolAC, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();
  uint8_t data[kWhirlpoolAcStateLength] = {
      0x83, 0x06, 0x10, 0x71, 0x00, 0x00, 0x91, 0x1F, 0x00, 0x00, 0x00,
      0x00, 0x00, 0xEF, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02};

  irsend.sendWhirlpoolAC(data);
  EXPECT_EQ(
      "m8950s4484"
      "m597s1649m597s1649m597s533m597s533m597s533m597s533m597s533m597s1649"
      "m597s533m597s1649m597s1649m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s1649m597s533m597s533m597s533"
      "m597s1649m597s533m597s533m597s533m597s1649m597s1649m597s1649m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s7920"
      "m597s1649m597s533m597s533m597s533m597s1649m597s533m597s533m597s1649"
      "m597s1649m597s1649m597s1649m597s1649m597s1649m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s1649m597s1649m597s1649m597s1649m597s533m597s1649m597s1649m597s1649"
      "m597s7920"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s1649m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s533m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s533m597s1649m597s533m597s533m597s533m597s533m597s533m597s533"
      "m597s100000",
      irsend.outputStr());
}

// Tests for decodeWhirlpoolAC().
// Decode normal WhirlpoolAC messages.
TEST(TestDecodeWhirlpoolAC, SyntheticDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Synthesised Normal WhirlpoolAC message.
  irsend.reset();
  uint8_t expectedState[kWhirlpoolAcStateLength] = {
      0x83, 0x06, 0x10, 0x71, 0x00, 0x00, 0x91, 0x1F, 0x00, 0x00, 0x00,
      0x00, 0x00, 0xEF, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02};
  irsend.sendWhirlpoolAC(expectedState);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(WHIRLPOOL_AC, irsend.capture.decode_type);
  EXPECT_EQ(kWhirlpoolAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
}

// Decode a recorded example
TEST(TestDecodeWhirlpoolAC, RealExampleDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Real WhirlpoolAC message.
  // Ref: https://github.com/markszabo/IRremoteESP8266/issues/509
  uint16_t rawData[343] = {
      8950, 4484, 598, 1642, 598, 1646, 594, 534,  594, 538,  602, 532,
      598,  540,  600, 542,  598, 1650, 600, 522,  598, 1644, 596, 1650,
      600,  532,  598, 538,  602, 536,  594, 548,  592, 538,  602, 518,
      600,  524,  596, 532,  598, 532,  598, 1654, 596, 544,  596, 544,
      596,  536,  594, 1644, 596, 528,  600, 528,  592, 538,  602, 1648,
      602,  1654, 596, 1664, 598, 534,  594, 526,  594, 530,  598, 528,
      602,  530,  600, 534,  596, 542,  598, 542,  598, 534,  596, 526,
      594,  530,  600, 528,  602, 530,  600, 534,  596, 542,  598, 544,
      596,  518,  602, 7916, 598, 1642, 598, 528,  600, 528,  602, 530,
      600,  1652, 598, 542,  598, 544,  596, 1654, 596, 1644, 596, 1648,
      602,  1644, 596, 1654, 596, 1656, 604, 536,  594, 548,  602, 528,
      600,  520,  600, 524,  596, 532,  598, 532,  596, 538,  602, 536,
      594,  546,  594, 538,  602, 518,  600, 524,  596, 532,  598, 532,
      598,  536,  594, 544,  596, 544,  596, 536,  594, 526,  592, 530,
      600,  528,  600, 530,  602, 532,  596, 542,  598, 542,  598, 534,
      596,  524,  596, 528,  600, 526,  592, 538,  592, 542,  598, 540,
      600,  540,  600, 530,  598, 522,  598, 526,  594, 534,  596, 534,
      594,  540,  602, 536,  592, 548,  592, 538,  600, 1636, 594, 1648,
      602,  1642, 598, 1652, 598, 538,  602, 1680, 570, 1662, 598, 1634,
      596,  7924, 600, 520,  598, 526,  592, 534,  596, 534,  596, 540,
      600,  536,  604, 538,  602, 530,  600, 520,  598, 1640, 600, 528,
      600,  530,  600, 534,  594, 544,  596, 544,  596, 534,  596, 526,
      594,  528,  600, 526,  594, 536,  592, 542,  598, 538,  602, 538,
      602,  528,  600, 520,  600, 524,  596, 530,  600, 532,  598, 534,
      596,  542,  598, 542,  598, 532,  598, 524,  596, 528,  602, 526,
      594,  536,  594, 540,  600, 536,  594, 548,  592, 538,  602, 518,
      602,  522,  596, 530,  600, 530,  600, 534,  596, 542,  598, 544,
      596,  534,  596, 524,  594, 1644, 596, 532,  596, 534,  596, 538,
      602,  536,  594, 546,  594, 520,  600};
  uint8_t expectedState[kWhirlpoolAcStateLength] = {
      0x83, 0x06, 0x10, 0x71, 0x00, 0x00, 0x91, 0x1F, 0x00, 0x00, 0x00,
      0x00, 0x00, 0xEF, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02};

  irsend.reset();
  irsend.sendRaw(rawData, 343, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(WHIRLPOOL_AC, irsend.capture.decode_type);
  EXPECT_EQ(kWhirlpoolAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
}
