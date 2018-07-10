// Copyright 2017 David Conran

#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendSony().

// Test sending typical data only.
TEST(TestSendSony, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSony(0);
  // We expect three 20-bit commands to be sent.
  EXPECT_EQ("m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m600s600"
            "m600s600m600s600m600s600m600s600m600s600m600s600m600s600m600s600"
            "m600s600m600s600m600s600m600s600m600s45600"
            "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m600s600"
            "m600s600m600s600m600s600m600s600m600s600m600s600m600s600m600s600"
            "m600s600m600s600m600s600m600s600m600s45600"
            "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m600s600"
            "m600s600m600s600m600s600m600s600m600s600m600s600m600s600m600s600"
            "m600s600m600s600m600s600m600s600m600s45600", irsend.outputStr());

  irsend.reset();
  irsend.sendSony(0x240C, SONY_20_BITS);
  // We expect three 20-bit commands to be sent.
  EXPECT_EQ("m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
            "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
            "m600s600m1200s600m1200s600m600s600m600s45600"
            "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
            "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
            "m600s600m1200s600m1200s600m600s600m600s45600"
            "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
            "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
            "m600s600m1200s600m1200s600m600s600m600s45600", irsend.outputStr());

  irsend.reset();
  irsend.sendSony(0x240C, SONY_15_BITS);
  // We expect three 15-bit commands to be sent.
  EXPECT_EQ("m2400s600m600s600m1200s600m600s600m600s600m1200s600m600s600"
            "m600s600m600s600m600s600m600s600m600s600m1200s600m1200s600m600s600"
            "m600s45600"
            "m2400s600m600s600m1200s600m600s600m600s600m1200s600m600s600"
            "m600s600m600s600m600s600m600s600m600s600m1200s600m1200s600m600s600"
            "m600s45600"
            "m2400s600m600s600m1200s600m600s600m600s600m1200s600m600s600"
            "m600s600m600s600m600s600m600s600m600s600m1200s600m1200s600m600s600"
            "m600s45600", irsend.outputStr());

  irsend.reset();
  irsend.sendSony(0xA90, SONY_12_BITS);
  // We expect three 15-bit commands to be sent.
  EXPECT_EQ("m2400s600m1200s600m600s600m1200s600m600s600m1200s600m600s600"
            "m600s600m1200s600m600s600m600s600m600s600m600s45600"
            "m2400s600m1200s600m600s600m1200s600m600s600m1200s600m600s600"
            "m600s600m1200s600m600s600m600s600m600s600m600s45600"
            "m2400s600m1200s600m600s600m1200s600m600s600m1200s600m600s600"
            "m600s600m1200s600m600s600m600s600m600s600m600s45600",
            irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendSony, SendWithDiffRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSony(0x240C, SONY_20_BITS, 0);  // Send a command with 0 repeats.
  EXPECT_EQ("m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
            "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
            "m600s600m1200s600m1200s600m600s600m600s45600", irsend.outputStr());
  irsend.sendSony(0x240C, SONY_20_BITS, 1);  // Send a command with 1 repeat.
  EXPECT_EQ("m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
            "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
            "m600s600m1200s600m1200s600m600s600m600s45600"
            "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
            "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
            "m600s600m1200s600m1200s600m600s600m600s45600", irsend.outputStr());
  irsend.sendSony(0x240C, SONY_20_BITS, 3);  // Send a command with 3 repeats.
  EXPECT_EQ("m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
            "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
            "m600s600m1200s600m1200s600m600s600m600s45600"
            "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
            "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
            "m600s600m1200s600m1200s600m600s600m600s45600"
            "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
            "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
            "m600s600m1200s600m1200s600m600s600m600s45600"
            "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
            "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
            "m600s600m1200s600m1200s600m600s600m600s45600", irsend.outputStr());
}

// Tests for encodeSony().

TEST(TestEncodeSony, NormalSonyEncoding) {
  IRsendTest irsend(4);
  EXPECT_EQ(0x0, irsend.encodeSony(SONY_12_BITS, 0, 0));
  EXPECT_EQ(0xA90, irsend.encodeSony(SONY_12_BITS, 21, 1));
  EXPECT_EQ(0xFFF, irsend.encodeSony(SONY_12_BITS, 0x7F, 0x1F));

  EXPECT_EQ(0x0, irsend.encodeSony(SONY_15_BITS, 0, 0));
  EXPECT_EQ(0x5480, irsend.encodeSony(SONY_15_BITS, 21, 1));
  EXPECT_EQ(0x5455, irsend.encodeSony(SONY_15_BITS, 21, 0xAA));
  EXPECT_EQ(0x7FFF, irsend.encodeSony(SONY_15_BITS, 0x7F, 0xFF));

  EXPECT_EQ(0x0, irsend.encodeSony(SONY_20_BITS, 0, 0, 0));
  EXPECT_EQ(0x81080, irsend.encodeSony(SONY_20_BITS, 1, 1, 1));
  EXPECT_EQ(0xFFFFF, irsend.encodeSony(SONY_20_BITS, 0x7F, 0x1F, 0xFF));
}

TEST(TestEncodeSony, SonyEncodingWithOversizedValues) {
  IRsendTest irsend(4);
  EXPECT_EQ(0xFFF, irsend.encodeSony(SONY_12_BITS, 0xFFFF, 0xFFFF));

  EXPECT_EQ(0x7FFF, irsend.encodeSony(SONY_15_BITS, 0xFFFF, 0xFFFF));

  EXPECT_EQ(0xFFFFF, irsend.encodeSony(SONY_20_BITS, 0xFFFF, 0xFFFF, 0xFFFF));
}

// Tests for decodeSony().

// Decode normal Sony messages.
TEST(TestDecodeSony, NormalSonyDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Synthesised Normal Sony 20-bit message.
  irsend.reset();
  irsend.sendSony(irsend.encodeSony(SONY_20_BITS, 0x1, 0x1, 0x1));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSony(&irsend.capture, SONY_20_BITS, true));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(SONY_20_BITS, irsend.capture.bits);
  EXPECT_EQ(0x81080, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x81, irsend.capture.command);

  // Synthesised Normal Sony 15-bit message.
  irsend.reset();
  irsend.sendSony(irsend.encodeSony(SONY_15_BITS, 21, 1), SONY_15_BITS);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSony(&irsend.capture, SONY_15_BITS, true));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(SONY_15_BITS, irsend.capture.bits);
  EXPECT_EQ(0x5480, irsend.capture.value);
  EXPECT_EQ(1, irsend.capture.address);
  EXPECT_EQ(21, irsend.capture.command);

  // Synthesised Normal Sony 12-bit message.
  irsend.reset();
  irsend.sendSony(irsend.encodeSony(SONY_12_BITS, 21, 1), SONY_12_BITS);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSony(&irsend.capture, SONY_12_BITS, true));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(SONY_12_BITS, irsend.capture.bits);
  EXPECT_EQ(0xA90, irsend.capture.value);
  EXPECT_EQ(1, irsend.capture.address);
  EXPECT_EQ(21, irsend.capture.command);
}

// Decode unexpected Sony messages. i.e longer than minimum etc.
TEST(TestDecodeSony, SonyDecodeWithUnexpectedLegalSize) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Synthesised Normal Sony 20-bit message decoded when looking for 12-bits
  irsend.reset();
  irsend.sendSony(irsend.encodeSony(SONY_20_BITS, 0x1, 0x1, 0x1));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSony(&irsend.capture, SONY_MIN_BITS));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(SONY_20_BITS, irsend.capture.bits);
  EXPECT_EQ(0x81080, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x81, irsend.capture.command);

  // Synthesised Normal Sony 12-bit message when expecting 20-bits.
  irsend.reset();
  irsend.sendSony(irsend.encodeSony(SONY_12_BITS, 21, 1), SONY_12_BITS);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSony(&irsend.capture, SONY_20_BITS));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(SONY_12_BITS, irsend.capture.bits);
  EXPECT_EQ(0xA90, irsend.capture.value);
  EXPECT_EQ(1, irsend.capture.address);
  EXPECT_EQ(21, irsend.capture.command);

  // 12-bit message should be regected when using strict and a different size.
  irsend.reset();
  irsend.sendSony(irsend.encodeSony(SONY_12_BITS, 21, 1), SONY_12_BITS);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_20_BITS, true));
  ASSERT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_15_BITS, true));

  // 15-bit message should be regected when using strict and a different size.
  irsend.reset();
  irsend.sendSony(irsend.encodeSony(SONY_15_BITS, 21, 1), SONY_15_BITS);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_12_BITS, true));
  ASSERT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_20_BITS, true));

  // 20-bit message should be regected when using strict and a different size.
  irsend.reset();
  irsend.sendSony(irsend.encodeSony(SONY_20_BITS, 1, 1, 1), SONY_20_BITS);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_12_BITS, true));
  ASSERT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_15_BITS, true));
}

// Decode unsupported Sony messages. i.e non-standard sizes.
TEST(TestDecodeSony, SonyDecodeWithIllegalSize) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSony(0xFF, 8);  // Illegal 8-bit Sony-like message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_MIN_BITS, true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_12_BITS, true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_15_BITS, true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_20_BITS, true));
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeSony(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(8, irsend.capture.bits);
  EXPECT_EQ(0xFF, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  irsend.sendSony(0x1FFF, 13);  // Illegal 13-bit Sony-like message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_MIN_BITS, true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_12_BITS, true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_15_BITS, true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_20_BITS, true));
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeSony(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(13, irsend.capture.bits);
  EXPECT_EQ(0x1FFF, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  irsend.sendSony(0x1FFFF, 17);  // Illegal 17-bit Sony-like message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_MIN_BITS, true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_12_BITS, true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_15_BITS, true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_20_BITS, true));
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeSony(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(17, irsend.capture.bits);
  EXPECT_EQ(0x1FFFF, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  irsend.sendSony(0x1FFFFF, 21);  // Illegal 21-bit Sony-like message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_MIN_BITS, true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_12_BITS, true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_15_BITS, true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, SONY_20_BITS, true));
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeSony(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(21, irsend.capture.bits);
  EXPECT_EQ(0x1FFFFF, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  // Illegal 64-bit (max) Sony-like message.
  irsend.sendSony(0xFFFFFFFFFFFFFFFF, 64, 0);
  irsend.makeDecodeResult();
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeSony(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}


// Decode unsupported Sony messages. i.e non-standard sizes.
TEST(TestDecodeSony, DecodeGlobalCacheExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Sony "Power On" from Global Cache.
  uint16_t gc_test[29] = {40000, 1, 1, 96, 24, 24, 24, 48, 24, 48, 24, 48, 24,
                          24, 24, 48, 24, 24, 24, 48, 24, 24, 24, 24, 24, 24,
                          24, 24, 1013};
  irsend.sendGC(gc_test, 29);
  irsend.makeDecodeResult();

  // Without strict.
  ASSERT_TRUE(irrecv.decodeSony(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(12, irsend.capture.bits);
  EXPECT_EQ(0x750, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x2E, irsend.capture.command);
  // With strict and correct size.
  ASSERT_TRUE(irrecv.decodeSony(&irsend.capture, SONY_12_BITS, true));
}
