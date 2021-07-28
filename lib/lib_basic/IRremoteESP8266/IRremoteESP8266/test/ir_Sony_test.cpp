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
  EXPECT_EQ(
      "f40000d33"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m600s600m600s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m600s600m600s600m600s600m600s18600"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m600s600m600s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m600s600m600s600m600s600m600s18600"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m600s600m600s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m600s600m600s600m600s600m600s18600",
      irsend.outputStr());

  irsend.reset();
  irsend.sendSony(0x240C, kSony20Bits);
  // We expect three 20-bit commands to be sent.
  EXPECT_EQ(
      "f40000d33"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
      "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m1200s600m1200s600m600s600m600s16200"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
      "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m1200s600m1200s600m600s600m600s16200"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
      "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m1200s600m1200s600m600s600m600s16200",
      irsend.outputStr());

  irsend.reset();
  irsend.sendSony(0x240C, kSony15Bits);
  // We expect three 15-bit commands to be sent.
  EXPECT_EQ(
      "f40000d33"
      "m2400s600m600s600m1200s600m600s600m600s600m1200s600m600s600"
      "m600s600m600s600m600s600m600s600m600s600m1200s600m1200s600m600s600"
      "m600s22200"
      "m2400s600m600s600m1200s600m600s600m600s600m1200s600m600s600"
      "m600s600m600s600m600s600m600s600m600s600m1200s600m1200s600m600s600"
      "m600s22200"
      "m2400s600m600s600m1200s600m600s600m600s600m1200s600m600s600"
      "m600s600m600s600m600s600m600s600m600s600m1200s600m1200s600m600s600"
      "m600s22200",
      irsend.outputStr());

  irsend.reset();
  irsend.sendSony(0xA90, kSony12Bits);
  // We expect three 15-bit commands to be sent.
  EXPECT_EQ(
      "f40000d33"
      "m2400s600m1200s600m600s600m1200s600m600s600m1200s600m600s600"
      "m600s600m1200s600m600s600m600s600m600s600m600s25800"
      "m2400s600m1200s600m600s600m1200s600m600s600m1200s600m600s600"
      "m600s600m1200s600m600s600m600s600m600s600m600s25800"
      "m2400s600m1200s600m600s600m1200s600m600s600m1200s600m600s600"
      "m600s600m1200s600m600s600m600s600m600s600m600s25800",
      irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendSony, SendWithDiffRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSony(0x240C, kSony20Bits, 0);  // Send a command with 0 repeats.
  EXPECT_EQ(
      "f40000d33"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
      "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m1200s600m1200s600m600s600m600s16200",
      irsend.outputStr());
  irsend.sendSony(0x240C, kSony20Bits, 1);  // Send a command with 1 repeat.
  EXPECT_EQ(
      "f40000d33"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
      "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m1200s600m1200s600m600s600m600s16200"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
      "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m1200s600m1200s600m600s600m600s16200",
      irsend.outputStr());
  irsend.sendSony(0x240C, kSony20Bits, 3);  // Send a command with 3 repeats.
  EXPECT_EQ(
      "f40000d33"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
      "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m1200s600m1200s600m600s600m600s16200"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
      "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m1200s600m1200s600m600s600m600s16200"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
      "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m1200s600m1200s600m600s600m600s16200"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
      "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m1200s600m1200s600m600s600m600s16200",
      irsend.outputStr());
}

// Tests for encodeSony().

TEST(TestEncodeSony, NormalSonyEncoding) {
  IRsendTest irsend(4);
  EXPECT_EQ(0x0, irsend.encodeSony(kSony12Bits, 0, 0));
  EXPECT_EQ(0xA90, irsend.encodeSony(kSony12Bits, 21, 1));
  EXPECT_EQ(0xFFF, irsend.encodeSony(kSony12Bits, 0x7F, 0x1F));

  EXPECT_EQ(0x0, irsend.encodeSony(kSony15Bits, 0, 0));
  EXPECT_EQ(0x5480, irsend.encodeSony(kSony15Bits, 21, 1));
  EXPECT_EQ(0x5455, irsend.encodeSony(kSony15Bits, 21, 0xAA));
  EXPECT_EQ(0x7FFF, irsend.encodeSony(kSony15Bits, 0x7F, 0xFF));

  EXPECT_EQ(0x0, irsend.encodeSony(kSony20Bits, 0, 0, 0));
  EXPECT_EQ(0x81080, irsend.encodeSony(kSony20Bits, 1, 1, 1));
  EXPECT_EQ(0xFFFFF, irsend.encodeSony(kSony20Bits, 0x7F, 0x1F, 0xFF));
}

TEST(TestEncodeSony, SonyEncodingWithOversizedValues) {
  IRsendTest irsend(4);
  EXPECT_EQ(0xFFF, irsend.encodeSony(kSony12Bits, 0xFFFF, 0xFFFF));

  EXPECT_EQ(0x7FFF, irsend.encodeSony(kSony15Bits, 0xFFFF, 0xFFFF));

  EXPECT_EQ(0xFFFFF, irsend.encodeSony(kSony20Bits, 0xFFFF, 0xFFFF, 0xFFFF));
}

// Tests for decodeSony().

// Decode normal Sony messages.
TEST(TestDecodeSony, NormalSonyDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Synthesised Normal Sony 20-bit message.
  irsend.reset();
  irsend.sendSony(irsend.encodeSony(kSony20Bits, 0x1, 0x1, 0x1));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(kSony20Bits, irsend.capture.bits);
  EXPECT_EQ(0x81080, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x81, irsend.capture.command);

  // Synthesised Normal Sony 15-bit message.
  irsend.reset();
  irsend.sendSony(irsend.encodeSony(kSony15Bits, 21, 1), kSony15Bits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(kSony15Bits, irsend.capture.bits);
  EXPECT_EQ(0x5480, irsend.capture.value);
  EXPECT_EQ(1, irsend.capture.address);
  EXPECT_EQ(21, irsend.capture.command);

  // Synthesised Normal Sony 12-bit message.
  irsend.reset();
  irsend.sendSony(irsend.encodeSony(kSony12Bits, 21, 1), kSony12Bits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(kSony12Bits, irsend.capture.bits);
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
  irsend.sendSony(irsend.encodeSony(kSony20Bits, 0x1, 0x1, 0x1));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSonyMinBits));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(kSony20Bits, irsend.capture.bits);
  EXPECT_EQ(0x81080, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x81, irsend.capture.command);

  // Synthesised Normal Sony 12-bit message when expecting 20-bits.
  irsend.reset();
  irsend.sendSony(irsend.encodeSony(kSony12Bits, 21, 1), kSony12Bits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSony20Bits));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(kSony12Bits, irsend.capture.bits);
  EXPECT_EQ(0xA90, irsend.capture.value);
  EXPECT_EQ(1, irsend.capture.address);
  EXPECT_EQ(21, irsend.capture.command);

  // 12-bit message should be regected when using strict and a different size.
  irsend.reset();
  irsend.sendSony(irsend.encodeSony(kSony12Bits, 21, 1), kSony12Bits);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSony20Bits,
                                 true));
  ASSERT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSony15Bits,
                                 true));

  // 15-bit message should be regected when using strict and a different size.
  irsend.reset();
  irsend.sendSony(irsend.encodeSony(kSony15Bits, 21, 1), kSony15Bits);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSony12Bits,
                                 true));
  ASSERT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSony20Bits,
                                 true));

  // 20-bit message should be regected when using strict and a different size.
  irsend.reset();
  irsend.sendSony(irsend.encodeSony(kSony20Bits, 1, 1, 1), kSony20Bits);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSony12Bits,
                                 true));
  ASSERT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSony15Bits,
                                 true));
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
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSonyMinBits,
                                 true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSony12Bits,
                                 true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSony15Bits,
                                 true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSony20Bits,
                                 true));
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(8, irsend.capture.bits);
  EXPECT_EQ(0xFF, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  irsend.sendSony(0x1FFF, 13);  // Illegal 13-bit Sony-like message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSonyMinBits,
                                 true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSony12Bits,
                                 true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSony15Bits,
                                 true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSony20Bits,
                                 true));
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(13, irsend.capture.bits);
  EXPECT_EQ(0x1FFF, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  irsend.sendSony(0x1FFFF, 17);  // Illegal 17-bit Sony-like message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSonyMinBits,
                                 true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSony12Bits,
                                 true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSony15Bits,
                                 true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSony20Bits,
                                 true));
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(17, irsend.capture.bits);
  EXPECT_EQ(0x1FFFF, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  irsend.sendSony(0x1FFFFF, 21);  // Illegal 21-bit Sony-like message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSonyMinBits,
                                 true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSony12Bits,
                                 true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSony15Bits,
                                 true));
  EXPECT_FALSE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSony20Bits,
                                 true));
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
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
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

TEST(TestDecodeSony, DecodeGlobalCacheExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Sony "Power On" from Global Cache.
  uint16_t gc_test[29] = {40000, 1,  1,  96, 24, 24, 24, 48, 24,  48,
                          24,    48, 24, 24, 24, 48, 24, 24, 24,  48,
                          24,    24, 24, 24, 24, 24, 24, 24, 1013};
  irsend.sendGC(gc_test, 29);
  irsend.makeDecodeResult();

  // Without strict.
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(12, irsend.capture.bits);
  EXPECT_EQ(0x750, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x2E, irsend.capture.command);
  // With strict and correct size.
  ASSERT_TRUE(irrecv.decodeSony(&irsend.capture, kStartOffset, kSony12Bits,
                                true));
}

// Encoding & Decode 20 bit Sony messages. Issue #476
TEST(TestEncodeSony, Issue476) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  irsend.sendSony(0x6AB47, 20);
  irsend.makeDecodeResult();

  // Without strict.
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(20, irsend.capture.bits);
  EXPECT_EQ(0x6AB47, irsend.capture.value);   // 20 bits
  EXPECT_EQ(0x1A, irsend.capture.address);    // 5 bits
  EXPECT_EQ(0x7156, irsend.capture.command);  // 15 bits
  EXPECT_EQ(0x56, 0x7156 & 0x7F);             // command (lower 7 bits)
  EXPECT_EQ(0xE2, (0x7156 >> 7) & 0xFF);      // extended (top 8 of 15 bits)
  EXPECT_EQ(0x6AB47, irsend.encodeSony(20, 0x56, 0x1A, 0xE2));
}

// Encoding & Decode 15 bit Sony messages. Issue #1018
TEST(TestEncodeSony, Issue1018) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  irsend.sendSony(0x240C, 15);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(15, irsend.capture.bits);
  EXPECT_EQ(0x240C, irsend.capture.value);   // 15 bits
  EXPECT_EQ(0x30, irsend.capture.address);
  EXPECT_EQ(0x12, irsend.capture.command);
  EXPECT_EQ(
      "f40000d33"
      "m2400s600"  // Message
      "m600s600m1200s600m600s600m600s600m1200s600m600s600m600s600"
      "m600s600m600s600m600s600m600s600m1200s600m1200s600m600s600"
      "m600s22200"
      "m2400s600"  // Repeat #1
      "m600s600m1200s600m600s600m600s600m1200s600m600s600m600s600"
      "m600s600m600s600m600s600m600s600m1200s600m1200s600m600s600"
      "m600s22200"
      "m2400s600"  // Repeat #2
      "m600s600m1200s600m600s600m600s600m1200s600m600s600m600s600"
      "m600s600m600s600m600s600m600s600m1200s600m1200s600m600s600"
      "m600s22200",
      irsend.outputStr());

  irsend.reset();
  uint16_t rawData[127] = {
      2448, 550,
      648, 544, 1250, 546, 648, 548, 648, 550, 1272, 524, 648, 550, 644, 550,
      674, 524, 648, 550, 648, 544, 674, 524, 1270, 524, 1246, 550, 674, 524,
      648, 22404,
      2474, 524,
      674, 520, 1250, 548, 648, 544, 674, 524, 1270, 524, 648, 550, 648, 546,
      674, 524, 648, 546, 652, 546, 674, 524, 1270, 524, 1272, 522, 674, 520,
      648, 22404,
      2452, 544,
      674, 524, 1270, 524, 674, 518, 674, 522, 1246, 550, 674, 524, 648, 544,
      674, 524, 648, 546, 674, 524, 674, 518, 1276, 518, 1276, 524, 648, 546,
      674, 22380,
      2474, 520,
      674, 524, 1250, 544, 674, 524, 674, 518, 1276, 520, 674, 522, 674, 524,
      674, 520, 674, 524, 674, 524, 674, 518, 1276, 518, 1276, 524, 672, 524,
      648};  // SONY 240C

  irsend.sendRaw(rawData, 127, 40);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(15, irsend.capture.bits);
  EXPECT_EQ(0x240C, irsend.capture.value);   // 15 bits
  EXPECT_EQ(0x30, irsend.capture.address);
  EXPECT_EQ(0x12, irsend.capture.command);
  EXPECT_EQ(
      "f40000d50"
      "m2448s550"  // Message
      "m648s544m1250s546m648s548m648s550m1272s524m648s550m644s550"
      "m674s524m648s550m648s544m674s524m1270s524m1246s550m674s524"
      "m648s22404"
      "m2474s524"  // Repeat #1
      "m674s520m1250s548m648s544m674s524m1270s524m648s550m648s546"
      "m674s524m648s546m652s546m674s524m1270s524m1272s522m674s520"
      "m648s22404"
      "m2452s544"  // Repeat #2
      "m674s524m1270s524m674s518m674s522m1246s550m674s524m648s544"
      "m674s524m648s546m674s524m674s518m1276s518m1276s524m648s546"
      "m674s22380"
      "m2474s520"  // Repeat #3
      "m674s524m1250s544m674s524m674s518m1276s520m674s522m674s524"
      "m674s520m674s524m674s524m674s518m1276s518m1276s524m672s524"
      "m648",
      irsend.outputStr());

  // Now see if we can reproduce it with `sendSony`
  irsend.reset();
  irsend.sendSony(0x240C, 15, 3);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(15, irsend.capture.bits);
  EXPECT_EQ(0x240C, irsend.capture.value);   // 15 bits
  EXPECT_EQ(0x30, irsend.capture.address);
  EXPECT_EQ(0x12, irsend.capture.command);

  // Compare expected result with real `rawData` result.
  // Comparison notes:
  //   * Seems visually the same. i.e. '1' where '1's should be etc.
  //   * Timings are *roughly* the same. They should be within device
  //     tollerance.
  // TL;DR: Looks fine/the same/as expected.
  EXPECT_EQ(
      "f40000d33"
      "m2400s600"  // Message
  //  "m2448s550"  (Commented out data is from `rawData` sample above.)
      "m600s600m1200s600m600s600m600s600m1200s600m600s600m600s600"
  //  "m648s544m1250s546m648s548m648s550m1272s524m648s550m644s550"
      "m600s600m600s600m600s600m600s600m1200s600m1200s600m600s600"
  //  "m674s524m648s550m648s544m674s524m1270s524m1246s550m674s524"
      "m600s22200"
  //  "m648s22404"
      "m2400s600"  // Repeat #1
  //  "m2474s524"
      "m600s600m1200s600m600s600m600s600m1200s600m600s600m600s600"
  //  "m674s520m1250s548m648s544m674s524m1270s524m648s550m648s546"
      "m600s600m600s600m600s600m600s600m1200s600m1200s600m600s600"
  //  "m674s524m648s546m652s546m674s524m1270s524m1272s522m674s520"
      "m600s22200"
  //  "m648s22404"
      "m2400s600"  // Repeat #2
  //  "m2452s544"
      "m600s600m1200s600m600s600m600s600m1200s600m600s600m600s600"
  //  "m674s524m1270s524m674s518m674s522m1246s550m674s524m648s544"
      "m600s600m600s600m600s600m600s600m1200s600m1200s600m600s600"
  //  "m674s524m648s546m674s524m674s518m1276s518m1276s524m648s546"
      "m600s22200"
  //  "m674s22380"
      "m2400s600"  // Repeat #3
  //  "m2474s520"
      "m600s600m1200s600m600s600m600s600m1200s600m600s600m600s600"
  //  "m674s524m1250s544m674s524m674s518m1276s520m674s522m674s524"
      "m600s600m600s600m600s600m600s600m1200s600m1200s600m600s600"
  //  "m674s520m674s524m674s524m674s518m1276s518m1276s524m672s524"
      "m600s22200",
  //  "m648"  // (Trailing space is ignored in real captures.)
      irsend.outputStr());

  // Now see if we can reproduce it with `sendSony38`
  irsend.reset();
  irsend.sendSony38(0x240C, 15);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(15, irsend.capture.bits);
  EXPECT_EQ(0x240C, irsend.capture.value);   // 15 bits
  EXPECT_EQ(0x30, irsend.capture.address);
  EXPECT_EQ(0x12, irsend.capture.command);
}

// Test sending typical data only.
TEST(TestSendSony38, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();

  irsend.reset();
  irsend.sendSony38(0);
  // We expect three 20-bit commands to be sent.
  EXPECT_EQ(
      "f38000d33"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m600s600m600s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m600s600m600s600m600s600m600s18600"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m600s600m600s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m600s600m600s600m600s600m600s18600"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m600s600m600s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m600s600m600s600m600s600m600s18600"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m600s600m600s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m600s600m600s600m600s600m600s18600",
      irsend.outputStr());

  irsend.reset();
  irsend.sendSony38(0x240C, kSony20Bits);
  // We expect three 20-bit commands to be sent.
  EXPECT_EQ(
      "f38000d33"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
      "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m1200s600m1200s600m600s600m600s16200"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
      "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m1200s600m1200s600m600s600m600s16200"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
      "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m1200s600m1200s600m600s600m600s16200"
      "m2400s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
      "m600s600m600s600m1200s600m600s600m600s600m600s600m600s600m600s600"
      "m600s600m1200s600m1200s600m600s600m600s16200",
      irsend.outputStr());

  irsend.reset();
  irsend.sendSony38(0x240C, kSony15Bits);
  // We expect three 15-bit commands to be sent.
  EXPECT_EQ(
      "f38000d33"
      "m2400s600m600s600m1200s600m600s600m600s600m1200s600m600s600"
      "m600s600m600s600m600s600m600s600m600s600m1200s600m1200s600m600s600"
      "m600s22200"
      "m2400s600m600s600m1200s600m600s600m600s600m1200s600m600s600"
      "m600s600m600s600m600s600m600s600m600s600m1200s600m1200s600m600s600"
      "m600s22200"
      "m2400s600m600s600m1200s600m600s600m600s600m1200s600m600s600"
      "m600s600m600s600m600s600m600s600m600s600m1200s600m1200s600m600s600"
      "m600s22200"
      "m2400s600m600s600m1200s600m600s600m600s600m1200s600m600s600"
      "m600s600m600s600m600s600m600s600m600s600m1200s600m1200s600m600s600"
      "m600s22200",
      irsend.outputStr());

  irsend.reset();
  irsend.sendSony38(0xA90, kSony12Bits);
  // We expect three 15-bit commands to be sent.
  EXPECT_EQ(
      "f38000d33"
      "m2400s600m1200s600m600s600m1200s600m600s600m1200s600m600s600"
      "m600s600m1200s600m600s600m600s600m600s600m600s25800"
      "m2400s600m1200s600m600s600m1200s600m600s600m1200s600m600s600"
      "m600s600m1200s600m600s600m600s600m600s600m600s25800"
      "m2400s600m1200s600m600s600m1200s600m600s600m1200s600m600s600"
      "m600s600m1200s600m600s600m600s600m600s600m600s25800"
      "m2400s600m1200s600m600s600m1200s600m600s600m1200s600m600s600"
      "m600s600m1200s600m600s600m600s600m600s600m600s25800",
      irsend.outputStr());
}
