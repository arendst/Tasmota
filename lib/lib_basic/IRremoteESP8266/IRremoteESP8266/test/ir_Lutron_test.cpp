// Copyright 2018 David Conran

#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendLutron().

// Test sending typical data only.
TEST(TestSendLutron, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();
  irsend.sendLutron(0);
  EXPECT_EQ("f40000d40m2288s230080", irsend.outputStr());
  irsend.sendLutron(0xAAAAAAAAA);  // Longest possible sequence. (I think)
  EXPECT_EQ(
      "f40000d40"
      "m2288s2288m2288s2288m2288s2288m2288s2288m2288s2288m2288s2288m2288s2288"
      "m2288s2288m2288s2288m2288s2288m2288s2288m2288s2288m2288s2288m2288s2288"
      "m2288s2288m2288s2288m2288s2288m2288s152288",
      irsend.outputStr());
  irsend.sendLutron(0x7FFFFFFFF);
  EXPECT_EQ("f40000d40m82368s150000", irsend.outputStr());
  irsend.sendLutron(0x7F88BD120);
  EXPECT_EQ(
      "f40000d40"
      "m20592s6864m2288s6864m2288s2288m9152s2288m2288s6864m2288s4576m2288"
      "s161440",
      irsend.outputStr());
}

// Test sending with repeats.
TEST(TestSendLutron, SendWithRepeats) {
  IRsendTest irsend(0);
  irsend.begin();
  // Send a command with 0 repeats.
  irsend.sendLutron(0x7F88BD120, kLutronBits, 0);
  EXPECT_EQ(
      "f40000d40"
      "m20592s6864m2288s6864m2288s2288m9152s2288m2288s6864m2288s4576m2288"
      "s161440",
      irsend.outputStr());
  // Send a command with 1 repeat.
  irsend.sendLutron(0x7F88BD120, kLutronBits, 1);
  EXPECT_EQ(
      "f40000d40"
      "m20592s6864m2288s6864m2288s2288m9152s2288m2288s6864m2288s4576m2288"
      "s161440"
      "m20592s6864m2288s6864m2288s2288m9152s2288m2288s6864m2288s4576m2288"
      "s161440",
      irsend.outputStr());
  // Send a command with 3 repeats.
  irsend.sendLutron(0x7F88BD120, kLutronBits, 3);
  EXPECT_EQ(
      "f40000d40"
      "m20592s6864m2288s6864m2288s2288m9152s2288m2288s6864m2288s4576m2288"
      "s161440"
      "m20592s6864m2288s6864m2288s2288m9152s2288m2288s6864m2288s4576m2288"
      "s161440"
      "m20592s6864m2288s6864m2288s2288m9152s2288m2288s6864m2288s4576m2288"
      "s161440"
      "m20592s6864m2288s6864m2288s2288m9152s2288m2288s6864m2288s4576m2288"
      "s161440",
      irsend.outputStr());
}

// Tests for decodeLutron().
// Decode normal Lutron messages.
TEST(TestDecodeLutron, SyntheticDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Synthesised Normal Lutron messages.
  irsend.reset();
  irsend.sendLutron(0x7F88BD120);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LUTRON, irsend.capture.decode_type);
  EXPECT_EQ(kLutronBits, irsend.capture.bits);
  EXPECT_EQ(0x7F88BD120, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  irsend.sendLutron(0x0DEADBEEF);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LUTRON, irsend.capture.decode_type);
  EXPECT_EQ(kLutronBits, irsend.capture.bits);
  EXPECT_EQ(0x0DEADBEEF, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  irsend.sendLutron(0x0);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LUTRON, irsend.capture.decode_type);
  EXPECT_EQ(kLutronBits, irsend.capture.bits);
  EXPECT_EQ(0x0, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  irsend.sendLutron(0x7FFFFFFFF);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LUTRON, irsend.capture.decode_type);
  EXPECT_EQ(kLutronBits, irsend.capture.bits);
  EXPECT_EQ(0x7FFFFFFFF, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

// Decode a documented example
TEST(TestDecodeLutron, DocumentedExampleFullOff) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Full Off code.
  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/515
  uint16_t rawData[14] = {20518, 6839, 2280, 6839, 2280, 2280, 9119,
                          2280,  2280, 6839, 2280, 4560, 2280, 11399};
  irsend.reset();
  irsend.sendRaw(rawData, 14, 40);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LUTRON, irsend.capture.decode_type);
  EXPECT_EQ(kLutronBits, irsend.capture.bits);
  EXPECT_EQ(0x7F88BD120, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  uint16_t pronto[18] = {0x0000, 0x0069, 0x0007, 0x0000, 0x032a, 0x010e,
                         0x005a, 0x010e, 0x005a, 0x005a, 0x0168, 0x005a,
                         0x005a, 0x010e, 0x005a, 0x00b4, 0x005a, 0x01c2};
  irsend.reset();
  irsend.sendPronto(pronto, 18);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LUTRON, irsend.capture.decode_type);
  EXPECT_EQ(kLutronBits, irsend.capture.bits);
  EXPECT_EQ(0x7F88BD120, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}
