// Copyright 2018 David Conran

#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendGICable().

// Test sending typical data only.
TEST(TestSendGICable, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();
  irsend.sendGICable(0);
  EXPECT_EQ(
      "m9000s4400"
      "m550s2200m550s2200m550s2200m550s2200m550s2200m550s2200m550s2200m550s2200"
      "m550s2200m550s2200m550s2200m550s2200m550s2200m550s2200m550s2200m550s2200"
      "m550s41650"
      "m9000s2200m550s87850",
      irsend.outputStr());
  irsend.sendGICable(0x8807);
  EXPECT_EQ(
      "m9000s4400"
      "m550s4400m550s2200m550s2200m550s2200m550s4400m550s2200m550s2200m550s2200"
      "m550s2200m550s2200m550s2200m550s2200m550s2200m550s4400m550s4400m550s4400"
      "m550s30650"
      "m9000s2200m550s87850",
      irsend.outputStr());
  irsend.sendGICable(0xFFFF);
  EXPECT_EQ(
      "m9000s4400"
      "m550s4400m550s4400m550s4400m550s4400m550s4400m550s4400m550s4400m550s4400"
      "m550s4400m550s4400m550s4400m550s4400m550s4400m550s4400m550s4400m550s4400"
      "m550s6450"
      "m9000s2200m550s87850",
      irsend.outputStr());
}

// Test sending with repeats.
TEST(TestSendGICable, SendWithRepeats) {
  IRsendTest irsend(0);
  irsend.begin();
  // Send a command with 0 repeats.
  irsend.sendGICable(0x8807, kGicableBits, 0);
  EXPECT_EQ(
      "m9000s4400"
      "m550s4400m550s2200m550s2200m550s2200m550s4400m550s2200m550s2200m550s2200"
      "m550s2200m550s2200m550s2200m550s2200m550s2200m550s4400m550s4400m550s4400"
      "m550s30650",
      irsend.outputStr());
  // Send a command with 1 repeat.
  irsend.sendGICable(0x8807, kGicableBits, 1);
  EXPECT_EQ(
      "m9000s4400"
      "m550s4400m550s2200m550s2200m550s2200m550s4400m550s2200m550s2200m550s2200"
      "m550s2200m550s2200m550s2200m550s2200m550s2200m550s4400m550s4400m550s4400"
      "m550s30650"
      "m9000s2200m550s87850",
      irsend.outputStr());
  // Send a command with 3 repeats.
  irsend.sendGICable(0x8807, kGicableBits, 3);
  EXPECT_EQ(
      "m9000s4400"
      "m550s4400m550s2200m550s2200m550s2200m550s4400m550s2200m550s2200m550s2200"
      "m550s2200m550s2200m550s2200m550s2200m550s2200m550s4400m550s4400m550s4400"
      "m550s30650"
      "m9000s2200m550s87850"
      "m9000s2200m550s87850"
      "m9000s2200m550s87850",
      irsend.outputStr());
}

// Tests for decodeGICable().
// Decode normal GICable messages.
TEST(TestDecodeGICable, SyntheticDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Synthesised Normal GICable message.
  irsend.reset();
  irsend.sendGICable(0x8807);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(GICABLE, irsend.capture.decode_type);
  EXPECT_EQ(kGicableBits, irsend.capture.bits);
  EXPECT_EQ(0x8807, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

// Decode a recorded example
TEST(TestDecodeGICable, RealExampleDecodeOK) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Real GICable "OK/Select" message.
  // Ref: https://github.com/markszabo/IRremoteESP8266/issues/447
  uint16_t rawData[39] = {9064, 4408, 580, 4408,  580,  2152, 578, 2150,
                          580,  2150, 580, 4408,  580,  2150, 580, 2150,
                          580,  2150, 580, 2150,  580,  2150, 580, 2150,
                          580,  2150, 580, 2150,  580,  4408, 580, 4408,
                          580,  4408, 580, 30622, 9066, 2148, 580};
  irsend.reset();
  irsend.sendRaw(rawData, 39, 39);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(GICABLE, irsend.capture.decode_type);
  EXPECT_EQ(kGicableBits, irsend.capture.bits);
  EXPECT_EQ(0x8807, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

TEST(TestDecodeGICable, RealExampleDecodeLEFT) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Real GICable "LEFT" message.
  // Ref: https://github.com/markszabo/IRremoteESP8266/issues/447
  uint16_t rawData[39] = {9040, 4434, 554, 2176,  580,  4408, 554, 4434,
                          582,  2148, 554, 4434,  580,  4408, 556, 2174,
                          580,  2150, 580, 2150,  582,  2148, 556, 2176,
                          580,  2150, 580, 4408,  580,  4408, 580, 4408,
                          582,  2150, 580, 26078, 9066, 2148, 580};
  irsend.reset();
  irsend.sendRaw(rawData, 39, 39);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(GICABLE, irsend.capture.decode_type);
  EXPECT_EQ(kGicableBits, irsend.capture.bits);
  EXPECT_EQ(0x6C0E, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

TEST(TestDecodeGICable, RealExampleDecodeZEROKey) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Real GICable "Zero Key" message.
  // Note: Zero key looks similar to a JVC message, hence this test.
  // Ref: https://github.com/markszabo/IRremoteESP8266/issues/447
  uint16_t rawData[39] = {9036, 4434, 552, 2178,  552,  2178, 552, 2180,
                          550,  2178, 552, 2178,  550,  2180, 552, 2178,
                          552,  2178, 550, 2180,  552,  2178, 526, 2204,
                          552,  2178, 552, 2178,  526,  2204, 526, 2204,
                          526,  2204, 526, 41932, 9036, 2176, 552};
  irsend.reset();
  irsend.sendRaw(rawData, 39, 39);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(GICABLE, irsend.capture.decode_type);
  EXPECT_EQ(kGicableBits, irsend.capture.bits);
  EXPECT_EQ(0x0, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}
