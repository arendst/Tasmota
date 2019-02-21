// Copyright 2017 David Conran

#include "IRsend_test.h"
#include "IRsend.h"
#include "gtest/gtest.h"

// Tests sendData().

// Test sending zero bits.
TEST(TestSendData, SendZeroBits) {
  IRsendTest irsend(4);
  irsend.begin();
  irsend.sendData(1, 2, 3, 4, 0b1, 0, true);
  EXPECT_EQ("", irsend.outputStr());
}

// Test sending zero and one.
TEST(TestSendData, SendSingleBit) {
  IRsendTest irsend(4);
  irsend.begin();
  irsend.sendData(1, 2, 3, 4, 0b1, 1, true);
  EXPECT_EQ("m1s2", irsend.outputStr());
  irsend.sendData(1, 2, 3, 4, 0b0, 1, true);
  EXPECT_EQ("m3s4", irsend.outputStr());
}

// Test sending bit order.
TEST(TestSendData, TestingBitSendOrder) {
  IRsendTest irsend(4);
  irsend.begin();
  irsend.sendData(1, 2, 3, 4, 0b10, 2, true);
  EXPECT_EQ("m1s2m3s4", irsend.outputStr());
  irsend.sendData(1, 2, 3, 4, 0b10, 2, false);
  EXPECT_EQ("m3s4m1s2", irsend.outputStr());
  irsend.sendData(1, 2, 3, 4, 0b0001, 4, false);
  EXPECT_EQ("m1s2m3s4m3s4m3s4", irsend.outputStr());
}

// Test sending typical data.
TEST(TestSendData, SendTypicalData) {
  IRsendTest irsend(4);
  irsend.begin();
  irsend.sendData(1, 2, 3, 4, 0b1010110011110000, 16, true);
  EXPECT_EQ("m1s2m3s4m1s2m3s4m1s2m1s2m3s4m3s4m1s2m1s2m1s2m1s2m3s4m3s4m3s4m3s4",
            irsend.outputStr());
  irsend.sendData(1, 2, 3, 4, 0x1234567890ABCDEF, 64, true);
  EXPECT_EQ(
      "m3s4m3s4m3s4m1s2m3s4m3s4m1s2m3s4m3s4m3s4m1s2m1s2m3s4m1s2m3s4m3s4"
      "m3s4m1s2m3s4m1s2m3s4m1s2m1s2m3s4m3s4m1s2m1s2m1s2m1s2m3s4m3s4m3s4"
      "m1s2m3s4m3s4m1s2m3s4m3s4m3s4m3s4m1s2m3s4m1s2m3s4m1s2m3s4m1s2m1s2"
      "m1s2m1s2m3s4m3s4m1s2m1s2m3s4m1s2m1s2m1s2m1s2m3s4m1s2m1s2m1s2m1s2",
      irsend.outputStr());
}

// Test sending more than expected bits.
TEST(TestSendData, SendOverLargeData) {
  IRsendTest irsend(4);
  irsend.begin();
  irsend.sendData(1, 2, 3, 4, 0xFFFFFFFFFFFFFFFF, 70, true);
  EXPECT_EQ(
      "m3s4m3s4m3s4m3s4m3s4m3s4"
      "m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2"
      "m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2"
      "m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2"
      "m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2m1s2",
      irsend.outputStr());
}

// Test inverting the output.
TEST(TestIRSend, InvertedOutput) {
  IRsendTest irsend(4, true);
  irsend.begin();
  irsend.sendData(1, 2, 3, 4, 0b1, 1, true);
  EXPECT_EQ("s1m2", irsend.outputStr());
  irsend.sendData(1, 2, 3, 4, 0b0, 1, true);
  EXPECT_EQ("s3m4", irsend.outputStr());
}

// Test typical use of sendRaw().
TEST(TestSendRaw, GeneralUse) {
  IRsendTest irsend(4);
  IRrecv irrecv(0);

  irsend.begin();
  // NEC C3E0E0E8 as measured in #204
  uint16_t rawData[67] = {
      8950, 4500, 550, 1650, 600, 1650, 550, 550,  600, 500,  600, 550,
      550,  550,  600, 1650, 550, 1650, 600, 1650, 600, 1650, 550, 1700,
      550,  550,  600, 550,  550, 550,  600, 500,  600, 550,  550, 1650,
      600,  1650, 600, 1650, 550, 550,  600, 500,  600, 500,  600, 550,
      550,  550,  600, 1650, 550, 1650, 600, 1650, 600, 500,  650, 1600,
      600,  500,  600, 550,  550, 550,  600};

  irsend.sendRaw(rawData, 67, 38);
  EXPECT_EQ(
      "m8950s4500"
      "m550s1650m600s1650m550s550m600s500m600s550m550s550m600s1650m550s1650"
      "m600s1650m600s1650m550s1700m550s550m600s550m550s550m600s500m600s550"
      "m550s1650m600s1650m600s1650m550s550m600s500m600s500m600s550m550s550"
      "m600s1650m550s1650m600s1650m600s500m650s1600m600s500m600s550m550s550"
      "m600",
      irsend.outputStr());

  irsend.reset();
  irsend.sendRaw(rawData, 67, 38);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeNEC(&irsend.capture, kNECBits, false));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(32, irsend.capture.bits);
  EXPECT_EQ(0xC3E0E0E8, irsend.capture.value);
  EXPECT_EQ(
      "m8950s4500"
      "m550s1650m600s1650m550s550m600s500m600s550m550s550m600s1650m550s1650"
      "m600s1650m600s1650m550s1700m550s550m600s550m550s550m600s500m600s550"
      "m550s1650m600s1650m600s1650m550s550m600s500m600s500m600s550m550s550"
      "m600s1650m550s1650m600s1650m600s500m650s1600m600s500m600s550m550s550"
      "m600",
      irsend.outputStr());
}

// Incorrect handling of decodes from Raw. i.e. There is no gap recorded at
// the end of a command when using the interrupt code. sendRaw() best emulates
// this for unit testing purposes. sendGC() and sendXXX() will add the trailing
// gap. Users won't see this in normal use.
TEST(TestSendRaw, NoTrailingGap) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  uint16_t rawData[67] = {
      9000, 4500, 650, 550,  650, 1650, 600, 550,  650, 550,  600, 1650,
      650,  550,  600, 1650, 650, 1650, 650, 1650, 600, 550,  650, 1650,
      650,  1650, 650, 550,  600, 1650, 650, 1650, 650, 550,  650, 550,
      650,  1650, 650, 550,  650, 550,  650, 550,  600, 550,  650, 550,
      650,  550,  650, 1650, 600, 550,  650, 1650, 650, 1650, 650, 1650,
      650,  1650, 650, 1650, 650, 1650, 600};
  irsend.sendRaw(rawData, 67, 38);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decodeNEC(&irsend.capture));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
}

TEST(TestLowLevelSend, MarkFrequencyModulationAt38kHz) {
  IRsendLowLevelTest irsend(0);

  irsend.begin();

  irsend.reset();
  irsend.enableIROut(38000, 50);
  EXPECT_EQ(5, irsend.mark(100));
  EXPECT_EQ(
      "[On]10usecs[Off]11usecs[On]10usecs[Off]11usecs[On]10usecs[Off]11usecs"
      "[On]10usecs[Off]11usecs[On]10usecs[Off]6usecs",
      irsend.low_level_sequence);

  irsend.reset();
  irsend.enableIROut(38000, 33);
  EXPECT_EQ(5, irsend.mark(100));
  EXPECT_EQ(
      "[On]6usecs[Off]15usecs[On]6usecs[Off]15usecs[On]6usecs[Off]15usecs"
      "[On]6usecs[Off]15usecs[On]6usecs[Off]10usecs",
      irsend.low_level_sequence);

  irsend.reset();
  irsend.enableIROut(38000, 100);
  EXPECT_EQ(1, irsend.mark(1000));
  EXPECT_EQ("[On]1000usecs[Off]", irsend.low_level_sequence);
}

TEST(TestLowLevelSend, MarkFrequencyModulationAt36_7kHz) {
  IRsendLowLevelTest irsend(0);

  irsend.begin();

  irsend.reset();
  irsend.enableIROut(36700, 50);
  EXPECT_EQ(5, irsend.mark(100));
  EXPECT_EQ(
      "[On]11usecs[Off]11usecs[On]11usecs[Off]11usecs[On]11usecs[Off]11usecs"
      "[On]11usecs[Off]11usecs[On]11usecs[Off]1usecs",
      irsend.low_level_sequence);

  irsend.reset();
  irsend.enableIROut(36700, 33);
  EXPECT_EQ(5, irsend.mark(100));
  EXPECT_EQ(
      "[On]7usecs[Off]15usecs[On]7usecs[Off]15usecs[On]7usecs[Off]15usecs"
      "[On]7usecs[Off]15usecs[On]7usecs[Off]5usecs",
      irsend.low_level_sequence);

  irsend.reset();
  irsend.enableIROut(36700, 100);
  EXPECT_EQ(1, irsend.mark(1000));
  EXPECT_EQ("[On]1000usecs[Off]", irsend.low_level_sequence);
}

TEST(TestLowLevelSend, MarkFrequencyModulationAt40kHz) {
  IRsendLowLevelTest irsend(0);

  irsend.begin();

  irsend.reset();
  irsend.enableIROut(40000, 50);
  EXPECT_EQ(5, irsend.mark(100));
  EXPECT_EQ(
      "[On]10usecs[Off]10usecs[On]10usecs[Off]10usecs[On]10usecs[Off]10usecs"
      "[On]10usecs[Off]10usecs[On]10usecs[Off]10usecs",
      irsend.low_level_sequence);

  irsend.reset();
  irsend.enableIROut(40000, 33);
  EXPECT_EQ(5, irsend.mark(100));
  EXPECT_EQ(
      "[On]6usecs[Off]14usecs[On]6usecs[Off]14usecs[On]6usecs[Off]14usecs"
      "[On]6usecs[Off]14usecs[On]6usecs[Off]14usecs",
      irsend.low_level_sequence);

  irsend.reset();
  irsend.enableIROut(40000, 100);
  EXPECT_EQ(1, irsend.mark(1000));
  EXPECT_EQ("[On]1000usecs[Off]", irsend.low_level_sequence);
}

TEST(TestLowLevelSend, MarkNoModulation) {
  IRsendLowLevelTest irsend(0, false, false);

  irsend.begin();

  irsend.reset();
  irsend.enableIROut(38000, 50);
  EXPECT_EQ(1, irsend.mark(1000));
  EXPECT_EQ("[On]1000usecs[Off]", irsend.low_level_sequence);

  irsend.reset();
  irsend.enableIROut(36700, 25);
  EXPECT_EQ(1, irsend.mark(1000));
  EXPECT_EQ("[On]1000usecs[Off]", irsend.low_level_sequence);

  irsend.reset();
  irsend.enableIROut(40000, 75);
  EXPECT_EQ(1, irsend.mark(1000));
  EXPECT_EQ("[On]1000usecs[Off]", irsend.low_level_sequence);
}

TEST(TestLowLevelSend, SpaceFrequencyModulation) {
  IRsendLowLevelTest irsend(0);

  irsend.reset();
  irsend.enableIROut(38000);
  irsend.space(1000);
  EXPECT_EQ("[Off]1000usecs", irsend.low_level_sequence);

  irsend.reset();
  irsend.enableIROut(40000, 75);
  irsend.space(1000);
  EXPECT_EQ("[Off]1000usecs", irsend.low_level_sequence);

  irsend.reset();
  irsend.enableIROut(38000, 100);
  irsend.space(1000);
  EXPECT_EQ("[Off]1000usecs", irsend.low_level_sequence);

  irsend.reset();
  irsend.enableIROut(38000, 33);
  irsend.space(1000);
  EXPECT_EQ("[Off]1000usecs", irsend.low_level_sequence);
}

TEST(TestLowLevelSend, SpaceNoModulation) {
  IRsendLowLevelTest irsend(0, false, false);

  irsend.begin();

  irsend.reset();
  irsend.enableIROut(38000, 50);
  irsend.space(1000);
  EXPECT_EQ("[Off]1000usecs", irsend.low_level_sequence);

  irsend.reset();
  irsend.enableIROut(36700, 25);
  irsend.space(1000);
  EXPECT_EQ("[Off]1000usecs", irsend.low_level_sequence);

  irsend.reset();
  irsend.enableIROut(40000, 75);
  irsend.space(1000);
  EXPECT_EQ("[Off]1000usecs", irsend.low_level_sequence);
}
