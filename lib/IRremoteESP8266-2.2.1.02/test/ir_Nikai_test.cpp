// Copyright 2017 David Conran

#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendNikai().
// Test sending typical data only.
TEST(TestSendNikai, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendNikai(0xD5F2A);  // Nikai TV Power Off.
  EXPECT_EQ("m4000s4000"
            "m500s2000m500s2000m500s2000m500s2000m500s1000m500s1000m500s2000"
            "m500s1000m500s2000m500s1000m500s2000m500s1000m500s1000m500s1000"
            "m500s1000m500s1000m500s2000m500s2000m500s1000m500s2000m500s1000"
            "m500s2000m500s1000m500s2000m500s8500",
            irsend.outputStr());

  irsend.reset();
}

// Test sending with different repeats.
TEST(TestSendNikai, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendNikai(0xD5F2A, NIKAI_BITS, 1);  // 1 repeat.
  EXPECT_EQ("m4000s4000"
            "m500s2000m500s2000m500s2000m500s2000m500s1000m500s1000m500s2000"
            "m500s1000m500s2000m500s1000m500s2000m500s1000m500s1000m500s1000"
            "m500s1000m500s1000m500s2000m500s2000m500s1000m500s2000m500s1000"
            "m500s2000m500s1000m500s2000m500s8500"
            "m4000s4000"
            "m500s2000m500s2000m500s2000m500s2000m500s1000m500s1000m500s2000"
            "m500s1000m500s2000m500s1000m500s2000m500s1000m500s1000m500s1000"
            "m500s1000m500s1000m500s2000m500s2000m500s1000m500s2000m500s1000"
            "m500s2000m500s1000m500s2000m500s8500",
            irsend.outputStr());
  irsend.sendNikai(0xD5F2A, NIKAI_BITS, 2);  // 2 repeat.
  EXPECT_EQ("m4000s4000"
            "m500s2000m500s2000m500s2000m500s2000m500s1000m500s1000m500s2000"
            "m500s1000m500s2000m500s1000m500s2000m500s1000m500s1000m500s1000"
            "m500s1000m500s1000m500s2000m500s2000m500s1000m500s2000m500s1000"
            "m500s2000m500s1000m500s2000m500s8500"
            "m4000s4000"
            "m500s2000m500s2000m500s2000m500s2000m500s1000m500s1000m500s2000"
            "m500s1000m500s2000m500s1000m500s2000m500s1000m500s1000m500s1000"
            "m500s1000m500s1000m500s2000m500s2000m500s1000m500s2000m500s1000"
            "m500s2000m500s1000m500s2000m500s8500"
            "m4000s4000"
            "m500s2000m500s2000m500s2000m500s2000m500s1000m500s1000m500s2000"
            "m500s1000m500s2000m500s1000m500s2000m500s1000m500s1000m500s1000"
            "m500s1000m500s1000m500s2000m500s2000m500s1000m500s2000m500s1000"
            "m500s2000m500s1000m500s2000m500s8500",
            irsend.outputStr());
}

// Tests for decodeNikai().

// Decode normal Nikai messages.
TEST(TestDecodeNikai, NormalDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Nikai 24-bit message.
  irsend.reset();
  irsend.sendNikai(0x123456);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeNikai(&irsend.capture, NIKAI_BITS, true));
  EXPECT_EQ(NIKAI, irsend.capture.decode_type);
  EXPECT_EQ(NIKAI_BITS, irsend.capture.bits);
  EXPECT_EQ(0x123456, irsend.capture.value);

  irsend.reset();
  irsend.sendNikai(0x101);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeNikai(&irsend.capture, NIKAI_BITS, true));
  EXPECT_EQ(NIKAI, irsend.capture.decode_type);
  EXPECT_EQ(NIKAI_BITS, irsend.capture.bits);
  EXPECT_EQ(0x101, irsend.capture.value);
}

// Decode normal repeated Nikai messages.
TEST(TestDecodeNikai, NormalDecodeWithRepeatAndStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Nikai 24-bit message.
  irsend.reset();
  irsend.sendNikai(0xD5F2A, NIKAI_BITS, 2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeNikai(&irsend.capture, NIKAI_BITS, true));
  EXPECT_EQ(NIKAI, irsend.capture.decode_type);
  EXPECT_EQ(NIKAI_BITS, irsend.capture.bits);
  EXPECT_EQ(0xD5F2A, irsend.capture.value);
}

TEST(TestDecodeNikai, NormalDecodeWithNonStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Illegal under length (16-bit) message
  irsend.reset();
  irsend.sendNikai(0x0, 16);
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeNikai(&irsend.capture, NIKAI_BITS, true));
  // And it should fail when we expect more bits.
  ASSERT_FALSE(irrecv.decodeNikai(&irsend.capture, NIKAI_BITS, false));

  // Should pass if strict off if we ask for correct nr. of bits sent.
  ASSERT_TRUE(irrecv.decodeNikai(&irsend.capture, 16, false));
  EXPECT_EQ(NIKAI, irsend.capture.decode_type);
  EXPECT_EQ(16, irsend.capture.bits);
  EXPECT_EQ(0x0, irsend.capture.value);

  // Should fail as we are expecting less bits than there are.
  ASSERT_FALSE(irrecv.decodeNikai(&irsend.capture, 12, false));
}

// Decode (non-standard) 64-bit messages.
// Decode unsupported Nikai messages.
TEST(TestDecodeNikai, Decode64BitMessages) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Illegal size Nikai 64-bit message.
  irsend.sendNikai(0xFFFFFFFFFFFFFFFF, 64);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeNikai(&irsend.capture, NIKAI_BITS, true));
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeNikai(&irsend.capture, 64, false));
  EXPECT_EQ(NIKAI, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, irsend.capture.value);
}

// Decode real example via Issue #309
TEST(TestDecodeNikai, DecodeExamples) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Nikai TV Power Off from Issue #309
  uint16_t rawdata_off[100] = {4060, 3918,
      508, 2004, 508, 2002, 510, 2002, 508, 2004, 506, 1050, 508, 1048,
      510, 2004, 508, 1048, 508, 2002, 510, 1050, 508, 2004, 510, 1048,
      508, 1050, 508, 1048, 508, 1050, 508, 1050, 508, 2004, 508, 2002,
      510, 1048, 508, 2004, 508, 1050, 506, 2004, 508, 1048, 510, 2002,
      456, 8446,
      3956, 3998,
      508, 2004, 508, 2002, 508, 2004, 508, 1978, 532, 1050, 508, 1050,
      508, 2002, 508, 1050, 508, 2004, 508, 1050, 508, 2002, 510, 1050,
      508, 1050, 508, 1048, 508, 1050, 508, 1050, 508, 2002, 510, 2002,
      508, 1050, 508, 2002, 510, 1050, 508, 2002, 508};
  irsend.sendRaw(rawdata_off, 100, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(NIKAI, irsend.capture.decode_type);
  EXPECT_EQ(NIKAI_BITS, irsend.capture.bits);
  EXPECT_EQ(0xD5F2A, irsend.capture.value);

  // Nikai TV Volume Up from Issue #309
  uint16_t rawdata_volup[52] = {3972, 4002,
      504, 1982, 526, 2010, 502, 2010, 502, 2010, 500, 1056, 502, 1056,
      502, 2010, 500, 1056, 502, 2010, 502, 2010, 500, 2010, 502, 2010,
      502, 1056, 502, 1056, 502, 1056, 500, 1056, 502, 2010, 502, 2010,
      500, 1056, 502, 2008, 502, 1054, 504, 1054, 504, 1054, 500, 1056,
      450};

  irsend.reset();
  irsend.sendRaw(rawdata_volup, 52, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(NIKAI, irsend.capture.decode_type);
  EXPECT_EQ(NIKAI_BITS, irsend.capture.bits);
  EXPECT_EQ(0xD0F2F, irsend.capture.value);
}

// Fail to decode a non-Nikai example via GlobalCache
TEST(TestDecodeNikai, FailToDecodeNonNikaiExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  uint16_t gc_test[71] = {38000, 1, 1, 172, 172, 22, 64, 22, 64, 22, 64, 22, 21,
                          22, 21, 22, 21, 22, 11, 22, 21, 22, 128, 22, 64, 22,
                          64, 22, 21, 22, 21, 22, 21, 22, 21, 22, 21, 22, 64,
                          22, 21, 22, 21, 22, 64, 22, 64, 22, 21, 22, 21, 22,
                          64, 22, 21, 22, 64, 22, 64, 22, 21, 22, 21, 22, 64,
                          22, 64, 22, 21, 22, 1820};
  irsend.sendGC(gc_test, 71);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeNikai(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeNikai(&irsend.capture, NIKAI_BITS, false));
}
