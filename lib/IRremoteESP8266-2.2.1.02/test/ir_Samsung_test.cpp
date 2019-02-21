// Copyright 2017 David Conran

#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendSAMSUNG().

// Test sending typical data only.
TEST(TestSendSamsung, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSAMSUNG(0xE0E09966);  // Samsung TV Power On.
  EXPECT_EQ("m4480s4480"
            "m560s1680m560s1680m560s1680m560s560m560s560m560s560m560s560"
            "m560s560m560s1680m560s1680m560s1680m560s560m560s560m560s560"
            "m560s560m560s560m560s1680m560s560m560s560m560s1680m560s1680"
            "m560s560m560s560m560s1680m560s560m560s1680m560s1680m560s560"
            "m560s560m560s1680m560s1680m560s560m560s108080",
            irsend.outputStr());

  irsend.reset();
}

// Test sending with different repeats.
TEST(TestSendSamsung, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSAMSUNG(0xE0E09966, SAMSUNG_BITS, 1);  // 1 repeat.
  EXPECT_EQ("m4480s4480"
            "m560s1680m560s1680m560s1680m560s560m560s560m560s560m560s560"
            "m560s560m560s1680m560s1680m560s1680m560s560m560s560m560s560"
            "m560s560m560s560m560s1680m560s560m560s560m560s1680m560s1680"
            "m560s560m560s560m560s1680m560s560m560s1680m560s1680m560s560"
            "m560s560m560s1680m560s1680m560s560m560s108080"
            "m4480s4480"
            "m560s1680m560s1680m560s1680m560s560m560s560m560s560m560s560"
            "m560s560m560s1680m560s1680m560s1680m560s560m560s560m560s560"
            "m560s560m560s560m560s1680m560s560m560s560m560s1680m560s1680"
            "m560s560m560s560m560s1680m560s560m560s1680m560s1680m560s560"
            "m560s560m560s1680m560s1680m560s560m560s108080"
            , irsend.outputStr());
  irsend.sendSAMSUNG(0xE0E09966, SAMSUNG_BITS, 2);  // 2 repeats.
  EXPECT_EQ("m4480s4480"
            "m560s1680m560s1680m560s1680m560s560m560s560m560s560m560s560"
            "m560s560m560s1680m560s1680m560s1680m560s560m560s560m560s560"
            "m560s560m560s560m560s1680m560s560m560s560m560s1680m560s1680"
            "m560s560m560s560m560s1680m560s560m560s1680m560s1680m560s560"
            "m560s560m560s1680m560s1680m560s560m560s108080"
            "m4480s4480"
            "m560s1680m560s1680m560s1680m560s560m560s560m560s560m560s560"
            "m560s560m560s1680m560s1680m560s1680m560s560m560s560m560s560"
            "m560s560m560s560m560s1680m560s560m560s560m560s1680m560s1680"
            "m560s560m560s560m560s1680m560s560m560s1680m560s1680m560s560"
            "m560s560m560s1680m560s1680m560s560m560s108080"
            "m4480s4480"
            "m560s1680m560s1680m560s1680m560s560m560s560m560s560m560s560"
            "m560s560m560s1680m560s1680m560s1680m560s560m560s560m560s560"
            "m560s560m560s560m560s1680m560s560m560s560m560s1680m560s1680"
            "m560s560m560s560m560s1680m560s560m560s1680m560s1680m560s560"
            "m560s560m560s1680m560s1680m560s560m560s108080"
            , irsend.outputStr());
}

// Tests for encodeSAMSUNG().

TEST(TestEncodeSamsung, NormalEncoding) {
  IRsendTest irsend(4);
  EXPECT_EQ(0xFF, irsend.encodeSAMSUNG(0, 0));
  EXPECT_EQ(0x8080807F, irsend.encodeSAMSUNG(1, 1));
  EXPECT_EQ(0xF8F805FA, irsend.encodeSAMSUNG(0x1F, 0xA0));
  EXPECT_EQ(0xA0A0CC33, irsend.encodeSAMSUNG(0x05, 0x33));
  EXPECT_EQ(0xFFFFFF00, irsend.encodeSAMSUNG(0xFF, 0xFF));
  EXPECT_EQ(0xE0E09966, irsend.encodeSAMSUNG(0x07, 0x99));
}

// Tests for decodeSAMSUNG().

// Decode normal Samsung messages.
TEST(TestDecodeSamsung, NormalDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Samsung 32-bit message.
  irsend.reset();
  irsend.sendSAMSUNG(0xE0E09966);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSAMSUNG(&irsend.capture, SAMSUNG_BITS, true));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(SAMSUNG_BITS, irsend.capture.bits);
  EXPECT_EQ(0xE0E09966, irsend.capture.value);
  EXPECT_EQ(0x07, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);

  // Synthesised Normal Samsung 32-bit message.
  irsend.reset();
  irsend.sendSAMSUNG(irsend.encodeSAMSUNG(0x07, 0x99));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSAMSUNG(&irsend.capture, SAMSUNG_BITS, true));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(SAMSUNG_BITS, irsend.capture.bits);
  EXPECT_EQ(0xE0E09966, irsend.capture.value);
  EXPECT_EQ(0x07, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);

  // Synthesised Normal Samsung 32-bit message.
  irsend.reset();
  irsend.sendSAMSUNG(irsend.encodeSAMSUNG(0x1, 0x1));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSAMSUNG(&irsend.capture, SAMSUNG_BITS, true));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(SAMSUNG_BITS, irsend.capture.bits);
  EXPECT_EQ(0x8080807F, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x1, irsend.capture.command);
}

// Decode normal repeated Samsung messages.
TEST(TestDecodeSamsung, NormalDecodeWithRepeatAndStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Samsung 32-bit message.
  irsend.reset();
  irsend.sendSAMSUNG(0xE0E09966, SAMSUNG_BITS, 2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSAMSUNG(&irsend.capture, SAMSUNG_BITS, true));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(SAMSUNG_BITS, irsend.capture.bits);
  EXPECT_EQ(0xE0E09966, irsend.capture.value);
  EXPECT_EQ(0x07, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);
}

// Decode unsupported Samsung messages.
TEST(TestDecodeSamsung, DecodeWithNonStrictValues) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSAMSUNG(0x0);  // Illegal value Samsung 32-bit message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture, SAMSUNG_BITS, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeSAMSUNG(&irsend.capture, SAMSUNG_BITS, false));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(SAMSUNG_BITS, irsend.capture.bits);
  EXPECT_EQ(0x0, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  irsend.sendSAMSUNG(0x12345678);  // Illegal value Samsung 32-bit message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture, SAMSUNG_BITS, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeSAMSUNG(&irsend.capture, SAMSUNG_BITS, false));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(SAMSUNG_BITS, irsend.capture.bits);
  EXPECT_EQ(0x12345678, irsend.capture.value);
  EXPECT_EQ(0x48, irsend.capture.address);
  EXPECT_EQ(0x6A, irsend.capture.command);

  // Illegal over length (36-bit) message.
  irsend.reset();
  irsend.sendSAMSUNG(irsend.encodeSAMSUNG(0, 0), 36);
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture, SAMSUNG_BITS, true));
  // Shouldn't pass if strict off and wrong expected bit size.
  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture, SAMSUNG_BITS, false));
  // Re-decode with correct bit size.
  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture, 36, true));
  ASSERT_TRUE(irrecv.decodeSAMSUNG(&irsend.capture, 36, false));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(36, irsend.capture.bits);
  EXPECT_EQ(0xFF, irsend.capture.value);  // We told it to expect 8 bits less.
  EXPECT_EQ(0x00, irsend.capture.address);
  EXPECT_EQ(0x00, irsend.capture.command);

  // Illegal under length (16-bit) message
  irsend.reset();
  irsend.sendSAMSUNG(irsend.encodeSAMSUNG(0x0, 0x0), 16);
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture, SAMSUNG_BITS, true));
  // And it should fail when we expect more bits.
  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture, SAMSUNG_BITS, false));

  // Should pass if strict off if we ask for correct nr. of bits sent.
  ASSERT_TRUE(irrecv.decodeSAMSUNG(&irsend.capture, 16, false));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(16, irsend.capture.bits);
  EXPECT_EQ(0xFF, irsend.capture.value);  // We told it to expect 4 bits less.
  EXPECT_EQ(0x00, irsend.capture.address);
  EXPECT_EQ(0x00, irsend.capture.command);

  // Should fail as we are expecting less bits than there are.
  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture, 12, false));
}

// Decode (non-standard) 64-bit messages.
// Decode unsupported Samsung messages.
TEST(TestDecodeSamsung, Decode64BitMessages) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Illegal value & size Samsung 64-bit message.
  irsend.sendSAMSUNG(0xFFFFFFFFFFFFFFFF, 64);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture, SAMSUNG_BITS, true));
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeSAMSUNG(&irsend.capture, 64, false));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, irsend.capture.value);
  EXPECT_EQ(0xFF, irsend.capture.address);
  EXPECT_EQ(0xFF, irsend.capture.command);
}

// Decode a 'real' example via GlobalCache
TEST(TestDecodeSamsung, DecodeGlobalCacheExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Samsung TV Power On from Global Cache.
  uint16_t gc_test[71] = {38000, 1, 1, 172, 172, 22, 64, 22, 64, 22, 64, 22, 21,
                          22, 21, 22, 21, 22, 21, 22, 21, 22, 64, 22, 64, 22,
                          64, 22, 21, 22, 21, 22, 21, 22, 21, 22, 21, 22, 64,
                          22, 21, 22, 21, 22, 64, 22, 64, 22, 21, 22, 21, 22,
                          64, 22, 21, 22, 64, 22, 64, 22, 21, 22, 21, 22, 64,
                          22, 64, 22, 21, 22, 1820};
  irsend.sendGC(gc_test, 71);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodeSAMSUNG(&irsend.capture));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(SAMSUNG_BITS, irsend.capture.bits);
  EXPECT_EQ(0xE0E09966, irsend.capture.value);
  EXPECT_EQ(0x07, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);
}

// Fail to decode a non-Samsung example via GlobalCache
TEST(TestDecodeSamsung, FailToDecodeNonSamsungExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Modified a few entries to unexpected values, based on previous test case.
  uint16_t gc_test[71] = {38000, 1, 1, 172, 172, 22, 64, 22, 64, 22, 64, 22, 21,
                          22, 21, 22, 21, 22, 11, 22, 21, 22, 128, 22, 64, 22,
                          64, 22, 21, 22, 21, 22, 21, 22, 21, 22, 21, 22, 64,
                          22, 21, 22, 21, 22, 64, 22, 64, 22, 21, 22, 21, 22,
                          64, 22, 21, 22, 64, 22, 64, 22, 21, 22, 21, 22, 64,
                          22, 64, 22, 21, 22, 1820};
  irsend.sendGC(gc_test, 71);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeSAMSUNG(&irsend.capture, SAMSUNG_BITS, false));
}
