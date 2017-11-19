// Copyright 2017 David Conran

#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendSherwood().

// Test sending typical data only.
TEST(TestSendSherwood, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSherwood(0xC1A28877);
  EXPECT_EQ("m8960s4480m560s1680m560s1680m560s560m560s560m560s560m560s560"
            "m560s560m560s1680m560s1680m560s560m560s1680m560s560m560s560"
            "m560s560m560s1680m560s560m560s1680m560s560m560s560m560s560"
            "m560s1680m560s560m560s560m560s560m560s560m560s1680m560s1680"
            "m560s1680m560s560m560s1680m560s1680m560s1680m560s108080"
            "m8960s2240m560s108080", irsend.outputStr());
}

// Test sending typical data with extra repeats.
TEST(TestSendSherwood, SendDataWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSherwood(0xC1A28877, 32, 2);
  EXPECT_EQ("m8960s4480m560s1680m560s1680m560s560m560s560m560s560m560s560"
            "m560s560m560s1680m560s1680m560s560m560s1680m560s560m560s560"
            "m560s560m560s1680m560s560m560s1680m560s560m560s560m560s560"
            "m560s1680m560s560m560s560m560s560m560s560m560s1680m560s1680"
            "m560s1680m560s560m560s1680m560s1680m560s1680m560s108080"
            "m8960s2240m560s108080"
            "m8960s2240m560s108080", irsend.outputStr());
}

// Test sending typical data with explicit no repeats.
TEST(TestSendSherwood, SendDataWithZeroRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSherwood(0xC1A28877, 32, 0);
  // Should have a single NEC repeat, as we always send one.
  EXPECT_EQ("m8960s4480m560s1680m560s1680m560s560m560s560m560s560m560s560"
            "m560s560m560s1680m560s1680m560s560m560s1680m560s560m560s560"
            "m560s560m560s1680m560s560m560s1680m560s560m560s560m560s560"
            "m560s1680m560s560m560s560m560s560m560s560m560s1680m560s1680"
            "m560s1680m560s560m560s1680m560s1680m560s1680m560s108080"
            "m8960s2240m560s108080", irsend.outputStr());
}

// Test that a typical Sherwood send decodes as the appropriate NEC value.
TEST(TestSendSherwood, DecodesAsNEC) {
  IRsendTest irsend(4);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  irsend.sendSherwood(0xC1A28877);
  irsend.makeDecodeResult();

  EXPECT_TRUE(irrecv.decodeNEC(&irsend.capture));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(NEC_BITS, irsend.capture.bits);
  EXPECT_EQ(0xC1A28877, irsend.capture.value);
  EXPECT_EQ(0x4583, irsend.capture.address);
  EXPECT_EQ(0x11, irsend.capture.command);
}
