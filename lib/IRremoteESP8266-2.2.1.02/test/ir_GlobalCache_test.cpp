// Copyright 2017 David Conran

#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendGlobalCache().

// Test sending a typical command wihtout a repeat.
TEST(TestSendGlobalCache, NonRepeatingCode) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();
  irsend.reset();

    // Modified NEC TV "Power On" from Global Cache with no repeats
  uint16_t gc_test[71] = {38000, 1, 1, 342, 172, 21, 22, 21, 21, 21, 65, 21, 21,
                          21, 22, 21, 22, 21, 21, 21, 22, 21, 65, 21, 65, 21,
                          22, 21, 65, 21, 65, 21, 65, 21, 65, 21, 65, 21, 65,
                          21, 22, 21, 22, 21, 21, 21, 22, 21, 22, 21, 65, 21,
                          22, 21, 21, 21, 65, 21, 65, 21, 65, 21, 64, 22, 65,
                          21, 22, 21, 65, 21, 1519};
  irsend.sendGC(gc_test, 71);
  irsend.makeDecodeResult();
  EXPECT_EQ("m8892s4472m546s572m546s546m546s1690m546s546m546s572m546s572"
            "m546s546m546s572m546s1690m546s1690m546s572m546s1690m546s1690"
            "m546s1690m546s1690m546s1690m546s1690m546s572m546s572m546s546"
            "m546s572m546s572m546s1690m546s572m546s546m546s1690m546s1690"
            "m546s1690m546s1664m572s1690m546s572m546s1690m546s39494",
            irsend.outputStr());
  EXPECT_TRUE(irrecv.decodeNEC(&irsend.capture));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(NEC_BITS, irsend.capture.bits);
  EXPECT_EQ(0x20DF827D, irsend.capture.value);
  EXPECT_EQ(0x4, irsend.capture.address);
  EXPECT_EQ(0x41, irsend.capture.command);
}

// Test sending typical command with repeats.
TEST(TestSendGlobalCache, RepeatCode) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();
  irsend.reset();

    // Sherwood (NEC-like) "Power On" from Global Cache with 2 repeats
  uint16_t gc_test[75] = {38000, 2, 69, 341, 171, 21, 64, 21, 64, 21, 21, 21,
                          21, 21, 21, 21, 21, 21, 21, 21, 64, 21, 64, 21, 21,
                          21, 64, 21, 21, 21, 21, 21, 21, 21, 64, 21, 21, 21,
                          64, 21, 21, 21, 21, 21, 21, 21, 64, 21, 21, 21, 21,
                          21, 21, 21, 21, 21, 64, 21, 64, 21, 64, 21, 21, 21,
                          64, 21, 64, 21, 64, 21, 1600, 341, 85, 21, 3647};
  irsend.sendGC(gc_test, 75);
  irsend.makeDecodeResult();
  EXPECT_EQ("m8866s4446m546s1664m546s1664m546s546m546s546m546s546m546s546"
            "m546s546m546s1664m546s1664m546s546m546s1664m546s546m546s546"
            "m546s546m546s1664m546s546m546s1664m546s546m546s546m546s546"
            "m546s1664m546s546m546s546m546s546m546s546m546s1664m546s1664"
            "m546s1664m546s546m546s1664m546s1664m546s1664m546s41600"
            "m8866s2210m546s94822"
            "m8866s2210m546s94822", irsend.outputStr());
  EXPECT_TRUE(irrecv.decodeNEC(&irsend.capture));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(NEC_BITS, irsend.capture.bits);
  EXPECT_EQ(0xC1A28877, irsend.capture.value);
  EXPECT_EQ(0x4583, irsend.capture.address);
  EXPECT_EQ(0x11, irsend.capture.command);
}
