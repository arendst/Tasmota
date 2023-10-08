// Copyright 2019 crankyoldgit (David Conran)

#include "IRsend.h"
#include "IRsend_test.h"
#include "IRutils.h"
#include "gtest/gtest.h"


// General housekeeping
TEST(TestInax, Housekeeping) {
  ASSERT_EQ("INAX", typeToString(INAX));
  ASSERT_FALSE(hasACState(INAX));
}

// Tests for sendInax().
// Test sending typical data only.
TEST(TestSendInax, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();

  irsend.reset();
  irsend.sendInax(0x5C32CD);  // Small flush.
  EXPECT_EQ(
    "f38000d50"
    "m9000s4500"
    "m560s560m560s1675m560s560m560s1675m560s1675m560s1675m560s560m560s560"
    "m560s560m560s560m560s1675m560s1675m560s560m560s560m560s1675m560s560"
    "m560s1675m560s1675m560s560m560s560m560s1675m560s1675m560s560m560s1675"
    "m560s40000"
    "m9000s4500"
    "m560s560m560s1675m560s560m560s1675m560s1675m560s1675m560s560m560s560"
    "m560s560m560s560m560s1675m560s1675m560s560m560s560m560s1675m560s560"
    "m560s1675m560s1675m560s560m560s560m560s1675m560s1675m560s560m560s1675"
    "m560s40000",
      irsend.outputStr());

  irsend.reset();
}

// Test sending with different repeats.
TEST(TestSendInax, SendWithRepeats) {
  IRsendTest irsend(0);
  irsend.begin();

  irsend.reset();
  irsend.sendInax(0x5C32CD, kInaxBits, 0);  // 0 repeats.
  EXPECT_EQ(
      "f38000d50"
      "m9000s4500"
      "m560s560m560s1675m560s560m560s1675m560s1675m560s1675m560s560m560s560"
      "m560s560m560s560m560s1675m560s1675m560s560m560s560m560s1675m560s560"
      "m560s1675m560s1675m560s560m560s560m560s1675m560s1675m560s560m560s1675"
      "m560s40000",
      irsend.outputStr());
  irsend.sendInax(0x5C32CD, kInaxBits, 2);  // 2 repeats.
  EXPECT_EQ(
      "f38000d50"
      "m9000s4500"
      "m560s560m560s1675m560s560m560s1675m560s1675m560s1675m560s560m560s560"
      "m560s560m560s560m560s1675m560s1675m560s560m560s560m560s1675m560s560"
      "m560s1675m560s1675m560s560m560s560m560s1675m560s1675m560s560m560s1675"
      "m560s40000"
      "m9000s4500"
      "m560s560m560s1675m560s560m560s1675m560s1675m560s1675m560s560m560s560"
      "m560s560m560s560m560s1675m560s1675m560s560m560s560m560s1675m560s560"
      "m560s1675m560s1675m560s560m560s560m560s1675m560s1675m560s560m560s1675"
      "m560s40000"
      "m9000s4500"
      "m560s560m560s1675m560s560m560s1675m560s1675m560s1675m560s560m560s560"
      "m560s560m560s560m560s1675m560s1675m560s560m560s560m560s1675m560s560"
      "m560s1675m560s1675m560s560m560s560m560s1675m560s1675m560s560m560s1675"
      "m560s40000",
      irsend.outputStr());
}

// Tests for decodeInax().

// Decode normal Inax messages.
TEST(TestDecodeInax, SyntheticDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Normal Inax 24-bit message.
  irsend.reset();
  irsend.sendInax(0x5C32CD);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(INAX, irsend.capture.decode_type);
  EXPECT_EQ(kInaxBits, irsend.capture.bits);
  EXPECT_EQ(0x5C32CD, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);
}

// Decode real example via Issue #704
TEST(TestDecodeInax, DecodeExamples) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  // Inax Small Flush from Issue #309
  uint16_t smallFlushRawData[51] = {
      8996, 4474, 568, 556, 560, 1676, 568, 556, 562, 1676, 562, 1678, 566,
      1674, 566, 558, 560, 560, 566, 556, 566, 556, 560, 1678, 562, 1676, 566,
      556, 562, 560, 564, 1672, 566, 556, 562, 1676, 562, 1678, 562, 560, 564,
      558, 564, 1674, 560, 1678, 564, 560, 566, 1670, 562};

  irsend.sendRaw(smallFlushRawData, 51, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(INAX, irsend.capture.decode_type);
  EXPECT_EQ(kInaxBits, irsend.capture.bits);
  EXPECT_EQ(0x5C32CD, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);
}
