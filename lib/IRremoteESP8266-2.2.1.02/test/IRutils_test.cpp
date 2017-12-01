// Copyright 2017 David Conran

#include "IRutils.h"
#include <stdint.h>
#include "gtest/gtest.h"

// Tests reverseBits().

// Tests reverseBits for typical use.
TEST(ReverseBitsTest, TypicalUse) {
  EXPECT_EQ(0xF, reverseBits(0xF0, 8));
  EXPECT_EQ(0xFFFF, reverseBits(0xFFFF0000, 32));
  EXPECT_EQ(0x555500005555FFFF, reverseBits(0xFFFFAAAA0000AAAA, 64));
  EXPECT_EQ(0, reverseBits(0, 64));
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, reverseBits(0xFFFFFFFFFFFFFFFF, 64));
}

// Tests reverseBits for bit size values <= 1
TEST(ReverseBitsTest, LessThanTwoBitsReversed) {
  EXPECT_EQ(0x12345678, reverseBits(0x12345678, 1));
  EXPECT_EQ(1234, reverseBits(1234, 0));
}

// Tests reverseBits for bit size larger than a uint64_t.
TEST(ReverseBitsTest, LargerThan64BitsReversed) {
  EXPECT_EQ(0, reverseBits(0, 65));
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, reverseBits(0xFFFFFFFFFFFFFFFF, 100));
  EXPECT_EQ(0x555500005555FFFF, reverseBits(0xFFFFAAAA0000AAAA, 3000));
}

// Tests reverseBits for bit sizes less than all the data stored.
TEST(ReverseBitsTest, LessBitsReversedThanInputHasSet) {
  EXPECT_EQ(0xF8, reverseBits(0xF1, 4));
  EXPECT_EQ(0xF5, reverseBits(0xFA, 4));
  EXPECT_EQ(0x12345678FFFF0000, reverseBits(0x123456780000FFFF, 32));
}

// Tests for uint64ToString()

TEST(TestUint64ToString, TrivialCases) {
  EXPECT_EQ("0", uint64ToString(0));  // Default base (10)
  EXPECT_EQ("0", uint64ToString(0, 2));  // Base-2
  EXPECT_EQ("0", uint64ToString(0, 8));  // Base-8
  EXPECT_EQ("0", uint64ToString(0, 10));  // Base-10
  EXPECT_EQ("0", uint64ToString(0, 16));  // Base-16

  EXPECT_EQ("1", uint64ToString(1, 2));  // Base-2
  EXPECT_EQ("2", uint64ToString(2, 8));  // Base-8
  EXPECT_EQ("3", uint64ToString(3, 10));  // Base-10
  EXPECT_EQ("4", uint64ToString(4, 16));  // Base-16
}

TEST(TestUint64ToString, NormalUse) {
  EXPECT_EQ("12345", uint64ToString(12345));
  EXPECT_EQ("100", uint64ToString(4, 2));
  EXPECT_EQ("3039", uint64ToString(12345, 16));
  EXPECT_EQ("123456", uint64ToString(123456));
  EXPECT_EQ("1E240", uint64ToString(123456, 16));
  EXPECT_EQ("FEEDDEADBEEF", uint64ToString(0xfeeddeadbeef, 16));
}

TEST(TestUint64ToString, Max64Bit) {
  EXPECT_EQ("18446744073709551615", uint64ToString(UINT64_MAX));  // Default
  EXPECT_EQ("1111111111111111111111111111111111111111111111111111111111111111",
            uint64ToString(UINT64_MAX, 2));  // Base-2
  EXPECT_EQ("1777777777777777777777", uint64ToString(UINT64_MAX, 8));  // Base-8
  EXPECT_EQ("18446744073709551615", uint64ToString(UINT64_MAX, 10));  // Base-10
  EXPECT_EQ("FFFFFFFFFFFFFFFF", uint64ToString(UINT64_MAX, 16));  // Base-16
}

TEST(TestUint64ToString, Max32Bit) {
  EXPECT_EQ("4294967295", uint64ToString(UINT32_MAX));  // Default
  EXPECT_EQ("37777777777", uint64ToString(UINT32_MAX, 8));  // Base-8
  EXPECT_EQ("4294967295", uint64ToString(UINT32_MAX, 10));  // Base-10
  EXPECT_EQ("FFFFFFFF", uint64ToString(UINT32_MAX, 16));  // Base-16
}

TEST(TestUint64ToString, InterestingCases) {
  // Previous hacky-code didn't handle leading zeros in the lower 32 bits.
  EXPECT_EQ("100000000", uint64ToString(0x100000000, 16));
  EXPECT_EQ("100000001", uint64ToString(0x100000001, 16));
}

TEST(TestUint64ToString, SillyBases) {
  // If we are given a silly base, we should defer to Base-10.
  EXPECT_EQ("12345", uint64ToString(12345, 0));  // Super silly, makes no sense.
  EXPECT_EQ("12345", uint64ToString(12345, 1));  // We don't do unary.
  EXPECT_EQ("12345", uint64ToString(12345, 100));  // We can't print base-100.
  EXPECT_EQ("12345", uint64ToString(12345, 37));  // Base-37 is one to far.
  EXPECT_EQ("9IX", uint64ToString(12345, 36));  // But we *can* do base-36.
}
