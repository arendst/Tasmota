// Copyright 2017 David Conran
// Copyright 2018 Brett T. Warden

#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

//   MM   MM WW   WW MM   MM
//   MMM MMM WW   WW MMM MMM
//   MM M MM WW W WW MM M MM
//   MM   MM WWW WWW MM   MM
//   MM   MM WW   WW MM   MM

// Tests for sendMWM().

// Test sending simplest case data only.
TEST(TestSendMWM, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  unsigned char test1[] = {0x96, 0x19, 0x10, 0x24, 0x0A,
                           0x6B, 0x20, 0x03, 0x82};
  /*
  ++--+-++--
  +-++--+++-
  +++++-+++-
  +++-++-++-
  ++-+-++++-
  +--+-+--+-
  ++++++-++-
  +--++++++-
  ++-+++++--
  */
  irsend.sendMWM(test1, sizeof(test1), 0);
  EXPECT_EQ(
      "f38000d25"
      "m834s834m417s417m834s834"
      "m417s417m834s834m1251s417"
      "m2085s417m1251s417"
      "m1251s417m834s417m834s417"
      "m834s417m417s417m1668s417"
      "m417s834m417s417m417s834m417s417"
      "m2502s417m834s417"
      "m417s834m2502s417"
      "m834s417m2085s30834", irsend.outputStr());

  irsend.reset();
  unsigned char test2[] = {
      0x99, 0x26, 0x66, 0x6E, 0xD1, 0x42, 0x06,
      0x20, 0xD0, 0x32, 0xF0, 0x0B
      // +-++--++--
      // ++--++-++-
      // ++--++--+-
      // ++---+--+-
      // +-+++-+---
      // ++-++++-+-
      // ++--+++++-
      // ++++++-++-
      // +++++-+---
      // ++-++--++-
      // +++++-----
      // +--+-++++-
  };
  irsend.sendMWM(test2, sizeof(test2), 0);
  EXPECT_EQ(
      "f38000d25"
      "m417s417m834s834m834s834"
      "m834s834m834s417m834s417"
      "m834s834m834s834m417s417"
      "m834s1251m417s834m417s417"
      "m417s417m1251s417m417s1251"
      "m834s417m1668s417m417s417"
      "m834s834m2085s417"
      "m2502s417m834s417"
      "m2085s417m417s1251"
      "m834s417m834s834m834s417"
      "m2085s2085"
      "m417s834m417s417m1668s30417", irsend.outputStr());
}

// Tests for decodeMWM().

// Example data
TEST(TestDecodeMWM, RealExamples) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  uint16_t short_code[] = {
      915,  793, 488,  366, 915,  793, 427, 366,  915, 793,  1281, 427,
      2136, 366, 1281, 366, 915,  793, 427, 854,  854, 366,  1281, 854,
      1708, 366, 488,  793, 854,  427, 427, 427,  427, 366,  854,  427,
      2563, 366, 488,  793, 2563, 366, 488, 2075, 427, 34057};
  unsigned char short_expected[] = {0x96, 0x19, 0x10, 0x36, 0x0C,
                                    0x53, 0x02, 0x03, 0xDF};
  irsend.sendRaw(short_code, sizeof(short_code) / sizeof(short_code[0]), 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MWM, irsend.capture.decode_type);
  EXPECT_EQ(8 * sizeof(short_expected) / sizeof(short_expected[0]),
            irsend.capture.bits);
  EXPECT_STATE_EQ(short_expected, irsend.capture.state, irsend.capture.bits);

  irsend.reset();
  uint16_t long_code[] = {
      427,  427,  854,  854,  854, 793,  915,  793, 854,  366,  915, 366, 854,
      854,  854,  793,  488,  366, 915,  1220, 427, 793,  488,  366, 488, 366,
      1281, 427,  427,  1220, 915, 366,  1708, 366, 488,  366,  854, 854, 2136,
      366,  2563, 366,  854,  427, 2136, 366,  488, 1220, 854,  427, 854, 793,
      915,  366,  2136, 2075, 427, 793,  488,  366, 1708, 30517};
  unsigned char long_expected[] = {0x99, 0x26, 0x66, 0x6E, 0xD1, 0x42,
                                   0x06, 0x20, 0xD0, 0x32, 0xF0, 0x0B};
  irsend.sendRaw(long_code, sizeof(long_code) / sizeof(long_code[0]), 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MWM, irsend.capture.decode_type);
  EXPECT_EQ(8 * sizeof(long_expected) / sizeof(long_expected[0]),
            irsend.capture.bits);
  EXPECT_STATE_EQ(long_expected, irsend.capture.state, irsend.capture.bits);
}

// vim: et:ts=2:sw=2
