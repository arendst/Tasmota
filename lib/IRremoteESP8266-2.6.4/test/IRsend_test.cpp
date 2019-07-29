// Copyright 2017,2019 David Conran

#include "IRsend_test.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRutils.h"
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
  EXPECT_EQ("d50m1s2", irsend.outputStr());
  irsend.sendData(1, 2, 3, 4, 0b0, 1, true);
  EXPECT_EQ("d50m3s4", irsend.outputStr());
}

// Test sending bit order.
TEST(TestSendData, TestingBitSendOrder) {
  IRsendTest irsend(4);
  irsend.begin();
  irsend.sendData(1, 2, 3, 4, 0b10, 2, true);
  EXPECT_EQ("d50m1s2m3s4", irsend.outputStr());
  irsend.sendData(1, 2, 3, 4, 0b10, 2, false);
  EXPECT_EQ("d50m3s4m1s2", irsend.outputStr());
  irsend.sendData(1, 2, 3, 4, 0b0001, 4, false);
  EXPECT_EQ("d50m1s2m3s4m3s4m3s4", irsend.outputStr());
}

// Test sending typical data.
TEST(TestSendData, SendTypicalData) {
  IRsendTest irsend(4);
  irsend.begin();
  irsend.sendData(1, 2, 3, 4, 0b1010110011110000, 16, true);
  EXPECT_EQ(
      "d50m1s2m3s4m1s2m3s4m1s2m1s2m3s4m3s4m1s2m1s2m1s2m1s2m3s4m3s4m3s4m3s4",
      irsend.outputStr());
  irsend.sendData(1, 2, 3, 4, 0x1234567890ABCDEF, 64, true);
  EXPECT_EQ(
      "d50"
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
      "d50"
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
  EXPECT_EQ("d50s1m2", irsend.outputStr());
  irsend.sendData(1, 2, 3, 4, 0b0, 1, true);
  EXPECT_EQ("d50s3m4", irsend.outputStr());
}

// Test we correctly pick up frequency changes.
TEST(TestIRSend, DetectFreqChanges) {
  IRsendTest irsend(0);

  irsend.begin();
  irsend.enableIROut(40);  // 40kHz
  irsend.sendData(1, 2, 3, 4, 0b1, 1, true);
  irsend.enableIROut(38);  // 40kHz
  irsend.sendData(1, 2, 3, 4, 0b1, 1, true);
  irsend.enableIROut(40);  // 40kHz
  irsend.sendData(1, 2, 3, 4, 0b1, 1, true);
  irsend.enableIROut(38);  // 40kHz
  irsend.sendData(1, 2, 3, 4, 0b1, 1, true);
  EXPECT_EQ(
      "f40000d50"
      "m1s2"
      "f38000"
      "m1s2"
      "f40000"
      "m1s2"
      "f38000"
      "m1s2",
      irsend.outputStr());
  irsend.reset();
  irsend.enableIROut(40);  // 40kHz
  irsend.sendData(1, 2, 3, 4, 0b1, 1, true);
  irsend.enableIROut(40);  // 40kHz
  irsend.sendData(1, 2, 3, 4, 0b1, 1, true);
  irsend.enableIROut(38);  // 40kHz
  irsend.sendData(1, 2, 3, 4, 0b1, 1, true);
  irsend.enableIROut(38);  // 40kHz
  irsend.sendData(1, 2, 3, 4, 0b1, 1, true);
  EXPECT_EQ(
      "f40000d50"
      "m1s2m1s2"
      "f38000m1s2m1s2",
      irsend.outputStr());
}

// Test we correctly pick up duty cycle changes.
TEST(TestIRSend, DetectDutyChanges) {
  IRsendTest irsend(0);

  irsend.begin();
  irsend.sendGeneric(1, 2, 3, 4, 5, 6, 7, 8, 0b1, 1, 38000, true, 0, 33);
  EXPECT_EQ(
      "f38000d33"
      "m1s2m3s4m7s8",
      irsend.outputStr());

  irsend.reset();
  irsend.sendGeneric(1, 2, 3, 4, 5, 6, 7, 8, 0b1, 1, 38000, true, 0, 50);
  irsend.sendGeneric(1, 2, 3, 4, 5, 6, 7, 8, 0b1, 1, 38000, true, 0, 33);
  irsend.sendGeneric(1, 2, 3, 4, 5, 6, 7, 8, 0b1, 1, 38000, true, 0, 25);
  EXPECT_EQ(
      "f38000d50"
      "m1s2m3s4m7s8"
      "d33"
      "m1s2m3s4m7s8"
      "d25"
      "m1s2m3s4m7s8",
      irsend.outputStr());
}


// Test we correctly pick up frequency AND duty changes.
TEST(TestIRSend, DetectFreqAndDutyChanges) {
  IRsendTest irsend(0);

  irsend.begin();
  irsend.sendGeneric(1, 2, 3, 4, 5, 6, 7, 8, 0b1, 1, 38000, true, 0, 50);
  irsend.sendGeneric(1, 2, 3, 4, 5, 6, 7, 8, 0b1, 1, 38000, true, 0, 33);
  irsend.sendGeneric(1, 2, 3, 4, 5, 6, 7, 8, 0b1, 1, 40000, true, 0, 25);
  EXPECT_EQ(
      "f38000d50"
      "m1s2m3s4m7s8"
      "d33"
      "m1s2m3s4m7s8"
      "f40000d25"
      "m1s2m3s4m7s8",
      irsend.outputStr());
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
      "f38000d50"
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
      "f38000d50"
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

// Test expected to work/produce a message for simple irsend:send()
TEST(TestSend, GenericSimpleSendMethod) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  ASSERT_TRUE(irsend.send(AIWA_RC_T501, 0x1234, kAiwaRcT501Bits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(AIWA_RC_T501, irsend.capture.decode_type);
  EXPECT_EQ(kAiwaRcT501Bits, irsend.capture.bits);
  EXPECT_EQ(0x1234, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(CARRIER_AC, 0x1234, kCarrierAcBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(CARRIER_AC, irsend.capture.decode_type);
  EXPECT_EQ(kCarrierAcBits, irsend.capture.bits);
  EXPECT_EQ(0x1234, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(COOLIX, 0x1234, kCoolixBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(kCoolixBits, irsend.capture.bits);
  EXPECT_EQ(0x1234, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(DENON, 0x1234, kDenonBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(DENON, irsend.capture.decode_type);
  EXPECT_EQ(kDenonBits, irsend.capture.bits);
  EXPECT_EQ(0x1234, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(DISH, 0x1234, kDishBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(DISH, irsend.capture.decode_type);
  EXPECT_EQ(kDishBits, irsend.capture.bits);
  EXPECT_EQ(0x1234, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(GICABLE, 0x1234, kGicableBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(GICABLE, irsend.capture.decode_type);
  EXPECT_EQ(kGicableBits, irsend.capture.bits);
  EXPECT_EQ(0x1234, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(GREE, 0x0009205000200050, kGreeBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(GREE, irsend.capture.decode_type);
  EXPECT_EQ(kGreeBits, irsend.capture.bits);
  // No simple value test for gree as it decodes to an Array.

  irsend.reset();
  ASSERT_TRUE(irsend.send(JVC, 0x1234, kJvcBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(JVC, irsend.capture.decode_type);
  EXPECT_EQ(kJvcBits, irsend.capture.bits);
  EXPECT_EQ(0x1234, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(LASERTAG, 0x123, kLasertagBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LASERTAG, irsend.capture.decode_type);
  EXPECT_EQ(kLasertagBits, irsend.capture.bits);
  EXPECT_EQ(0x123, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(LG, 0x700992, kLgBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(kLgBits, irsend.capture.bits);
  EXPECT_EQ(0x700992, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(LG, 0x700992, kLg32Bits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(kLg32Bits, irsend.capture.bits);
  EXPECT_EQ(0x700992, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(LG2, 0x880094D, kLgBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LG2, irsend.capture.decode_type);
  EXPECT_EQ(kLgBits, irsend.capture.bits);
  EXPECT_EQ(0x880094D, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(LUTRON, 0x1234, kLutronBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LUTRON, irsend.capture.decode_type);
  EXPECT_EQ(kLutronBits, irsend.capture.bits);
  EXPECT_EQ(0x1234, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(MAGIQUEST, 0x560F40020455, kMagiquestBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MAGIQUEST, irsend.capture.decode_type);
  EXPECT_EQ(kMagiquestBits, irsend.capture.bits);
  EXPECT_EQ(0x560F40020455, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(MIDEA, 0xA18263FFFF6E, kMideaBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MIDEA, irsend.capture.decode_type);
  EXPECT_EQ(kMideaBits, irsend.capture.bits);
  EXPECT_EQ(0xA18263FFFF6E, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(MITSUBISHI, 0x1234, kMitsubishiBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MITSUBISHI, irsend.capture.decode_type);
  EXPECT_EQ(kMitsubishiBits, irsend.capture.bits);
  EXPECT_EQ(0x1234, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(MITSUBISHI2, 0x1234, kMitsubishiBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MITSUBISHI2, irsend.capture.decode_type);
  EXPECT_EQ(kMitsubishiBits, irsend.capture.bits);
  EXPECT_EQ(0x1234, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(NIKAI, 0x1234, kNikaiBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(NIKAI, irsend.capture.decode_type);
  EXPECT_EQ(kNikaiBits, irsend.capture.bits);
  EXPECT_EQ(0x1234, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(NEC, 0x4BB640BF, kNECBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(NEC_LIKE, 0x12345678, kNECBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(NEC_LIKE, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x12345678, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(PANASONIC, 0x40040190ED7C, kPanasonicBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicBits, irsend.capture.bits);
  EXPECT_EQ(0x40040190ED7C, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(PIONEER, 0x659A05FAF50AC53A, kPioneerBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(PIONEER, irsend.capture.decode_type);
  EXPECT_EQ(kPioneerBits, irsend.capture.bits);
  EXPECT_EQ(0x659A05FAF50AC53A, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(RC5, 0x175, kRC5Bits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(RC5, irsend.capture.decode_type);
  EXPECT_EQ(kRC5Bits, irsend.capture.bits);
  EXPECT_EQ(0x175, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(RC6, 0xC800F740C, kRC6_36Bits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(RC6, irsend.capture.decode_type);
  EXPECT_EQ(kRC6_36Bits, irsend.capture.bits);
  EXPECT_EQ(0xC800F740C, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(RCMM, 0x1234, kRCMMBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(RCMM, irsend.capture.decode_type);
  EXPECT_EQ(kRCMMBits, irsend.capture.bits);
  EXPECT_EQ(0x1234, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(SAMSUNG, 0xE0E09966, kSamsungBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(kSamsungBits, irsend.capture.bits);
  EXPECT_EQ(0xE0E09966, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(SAMSUNG36, 0x1234, kSamsung36Bits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SAMSUNG36, irsend.capture.decode_type);
  EXPECT_EQ(kSamsung36Bits, irsend.capture.bits);
  EXPECT_EQ(0x1234, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(SANYO_LC7461, 0x2468DCB56A9, kSanyoLC7461Bits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SANYO_LC7461, irsend.capture.decode_type);
  EXPECT_EQ(kSanyoLC7461Bits, irsend.capture.bits);
  EXPECT_EQ(0x2468DCB56A9, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(SHARP, 0x7266, kSharpBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SHARP, irsend.capture.decode_type);
  EXPECT_EQ(kSharpBits, irsend.capture.bits);
  EXPECT_EQ(0x7266, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(SHERWOOD, 0x4BB640BF, kSherwoodBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kSherwoodBits, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(SONY, 0x1234, kSony20Bits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(kSony20Bits, irsend.capture.bits);
  EXPECT_EQ(0x1234, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(TECO, 0x1234, kTecoBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(TECO, irsend.capture.decode_type);
  EXPECT_EQ(kTecoBits, irsend.capture.bits);
  EXPECT_EQ(0x1234, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(VESTEL_AC, 0xF4410001FF1201, kVestelAcBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(VESTEL_AC, irsend.capture.decode_type);
  EXPECT_EQ(kVestelAcBits, irsend.capture.bits);
  EXPECT_EQ(0xF4410001FF1201, irsend.capture.value);

  irsend.reset();
  ASSERT_TRUE(irsend.send(WHYNTER, 0x1234, kWhynterBits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(WHYNTER, irsend.capture.decode_type);
  EXPECT_EQ(kWhynterBits, irsend.capture.bits);
  EXPECT_EQ(0x1234, irsend.capture.value);
}

// Test some expected types to NOT work/produce a message via irsend:send()
TEST(TestSend, GenericSimpleSendMethodFailure) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Check nothing is sent for unexpected protocols
  irsend.reset();
  ASSERT_FALSE(irsend.send(KELVINATOR, 0x1234, kKelvinatorBits));
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decode(&irsend.capture));

  // For every A/C protocol which decodes to having a state[].
  for (int i = 0; i <= kLastDecodeType; i++) {
    if (hasACState((decode_type_t)i) && i != GREE) {  // Gree is an exception.
      // Check it fails.
      ASSERT_FALSE(irsend.send((decode_type_t)i, (uint64_t)0, 0));
    }
  }

  // Test some other special cases.
  ASSERT_FALSE(irsend.send(UNKNOWN, (uint64_t)0, 0));
  ASSERT_FALSE(irsend.send(UNUSED, (uint64_t)0, 0));
  ASSERT_FALSE(irsend.send(RAW, (uint64_t)0, 0));
  ASSERT_FALSE(irsend.send(PRONTO, (uint64_t)0, 0));
  ASSERT_FALSE(irsend.send(GLOBALCACHE, (uint64_t)0, 0));
}

// Test expected to work/produce a message for complex irsend:send()
TEST(TestSend, GenericComplexSendMethod) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  uint8_t kelvinator[kKelvinatorStateLength] = {
      0x19, 0x0B, 0x80, 0x50, 0x00, 0x00, 0x00, 0xE0,
      0x19, 0x0B, 0x80, 0x70, 0x00, 0x00, 0x10, 0xF0};
  ASSERT_TRUE(irsend.send(KELVINATOR, kelvinator, kKelvinatorStateLength));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(KELVINATOR, irsend.capture.decode_type);
  EXPECT_EQ(kKelvinatorStateLength * 8, irsend.capture.bits);
  EXPECT_STATE_EQ(kelvinator, irsend.capture.state, irsend.capture.bits / 8);

  irsend.reset();
  uint8_t panasonic[kPanasonicAcStateLength] = {
      0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02,
      0x20, 0xE0, 0x04, 0x00, 0x4E, 0x2E, 0x80, 0xAF, 0x00,
      0x00, 0x0E, 0xE0, 0x11, 0x00, 0x01, 0x00, 0x06, 0xB7};
  ASSERT_TRUE(irsend.send(PANASONIC_AC, panasonic, kPanasonicAcStateLength));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(PANASONIC_AC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicAcStateLength * 8, irsend.capture.bits);
  EXPECT_STATE_EQ(panasonic, irsend.capture.state, irsend.capture.bits / 8);
}

// Test some expected types to NOT work/produce a complex message via
// irsend:send()
TEST(TestSend, GenericComplexSendMethodFailure) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Check nothing is sent for unexpected protocols
  uint8_t state[kStateSizeMax] = {};
  irsend.reset();
  ASSERT_FALSE(irsend.send(NEC, state, kNECBits));
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decode(&irsend.capture));

  // For every A/C protocol which DOESN'T decode to having a state[].
  for (int i = -1; i <= kLastDecodeType; i++) {
    if (!hasACState((decode_type_t)i))
      // Check it fails.
      ASSERT_FALSE(irsend.send((decode_type_t)i, state, 0));
    else  // Or if it is okay.
      ASSERT_TRUE(irsend.send((decode_type_t)i, state, 0));
  }
}

TEST(TestSend, GenericSendExistsForEveryRealProtocol) {
  IRsendTest irsend(0);
  irsend.begin();

  uint8_t state[kStateSizeMax] = {};
  uint64_t value = 0;
  for (int i = 1; i <= kLastDecodeType; i++) {
    switch (i) {
      // Protocols that don't have a generic send equiv.
      case PRONTO:
      case RAW:
      case GLOBALCACHE:
      // Protocols that are disabled because they don't work.
      case SANYO:
        break;
      default:
        EXPECT_TRUE(irsend.send((decode_type_t)i, state, 0) ||
                    irsend.send((decode_type_t)i, value, 0)) <<
            "Protocol " << typeToString((decode_type_t)i) << "(" << i <<
            ") doesn't have a generic send option for it.";
    }
  }
}

TEST(TestSend, defaultBits) {
  for (int i = 1; i <= kLastDecodeType; i++) {
    switch (i) {
      // Protocols that don't have have a default bit size.
      case PRONTO:
      case RAW:
      case GLOBALCACHE:
      case SANYO:  // Not implemented / disabled.
      // Deliberate no default size.
      case FUJITSU_AC:
      case MWM:
        EXPECT_EQ(IRsend::defaultBits((decode_type_t)i), 0) <<
            "Protocol " << typeToString((decode_type_t)i) << "(" << i <<
            ") doesn't have a correct value for it.";
        break;
      default:
        EXPECT_GT(IRsend::defaultBits((decode_type_t)i), 0) <<
            "Protocol " << typeToString((decode_type_t)i) << "(" << i <<
            ") doesn't have a correct value for it.";
    }
  }
}
