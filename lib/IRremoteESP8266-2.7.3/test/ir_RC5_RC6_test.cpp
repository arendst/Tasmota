// Copyright 2017 David Conran

#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// RRRRRR   CCCCC          555555   RRRRRR   CCCCC          555555  XX    XX
// RR   RR CC    C         55       RR   RR CC    C         55       XX  XX
// RRRRRR  CC       _____  555555   RRRRRR  CC       _____  555555    XXXX
// RR  RR  CC    C            5555  RR  RR  CC    C            5555  XX  XX
// RR   RR  CCCCC          555555   RR   RR  CCCCC          555555  XX    XX

// Tests for encodeRC5().
TEST(TestEncodeRC5, NormalEncoding) {
  IRsendTest irsend(4);

  EXPECT_EQ(0x0, irsend.encodeRC5(0, 0));
  EXPECT_EQ(0x800, irsend.encodeRC5(0, 0, true));
  EXPECT_EQ(0x41, irsend.encodeRC5(1, 1));
  EXPECT_EQ(0x42, irsend.encodeRC5(1, 2));
  EXPECT_EQ(0x7FF, irsend.encodeRC5(0x1F, 0x3F));
  EXPECT_EQ(0xFFF, irsend.encodeRC5(0x1F, 0x3F, true));
  EXPECT_EQ(0x7FF, irsend.encodeRC5(0xFF, 0xFF));
  EXPECT_EQ(0xFFF, irsend.encodeRC5(0xFF, 0xFF, true));

  EXPECT_EQ(0x175, irsend.encodeRC5(0x05, 0x35));
}

// Tests for encodeRC5X().
TEST(TestEncodeRC5X, NormalEncoding) {
  IRsendTest irsend(4);

  EXPECT_EQ(0x0, irsend.encodeRC5X(0, 0));
  EXPECT_EQ(0x800, irsend.encodeRC5X(0, 0, true));
  EXPECT_EQ(0x41, irsend.encodeRC5X(1, 1));
  EXPECT_EQ(0x42, irsend.encodeRC5X(1, 2));
  EXPECT_EQ(0x3FF, irsend.encodeRC5X(0x0F, 0x3F));
  EXPECT_EQ(0x3FF, irsend.encodeRC5X(0x0F, 0x3F, false));
  EXPECT_EQ(0xBFF, irsend.encodeRC5X(0x0F, 0x3F, true));
  EXPECT_EQ(0x17FF, irsend.encodeRC5X(0x1F, 0x7F));
  EXPECT_EQ(0x1FFF, irsend.encodeRC5X(0x1F, 0x7F, true));
  EXPECT_EQ(0x17FF, irsend.encodeRC5X(0xFF, 0xFF));
  EXPECT_EQ(0x1FFF, irsend.encodeRC5X(0xFF, 0xFF, true));

  EXPECT_EQ(0x175, irsend.encodeRC5X(0x05, 0x35));

  // Values of command <= 6-bits. i.e (<= 63 (0x3F)) should be the same
  // as encodeRC5.
  EXPECT_EQ(irsend.encodeRC5X(0, 0), irsend.encodeRC5(0, 0));
  EXPECT_EQ(irsend.encodeRC5X(0, 0, true), irsend.encodeRC5(0, 0, true));
  EXPECT_EQ(irsend.encodeRC5X(0x5, 0x35, false),
            irsend.encodeRC5(0x5, 0x35, false));
  EXPECT_EQ(irsend.encodeRC5X(0x5, 0x35, true),
            irsend.encodeRC5(0x5, 0x35, true));
  EXPECT_EQ(irsend.encodeRC5X(0x1F, 0x3F, true),
            irsend.encodeRC5(0x1F, 0x3F, true));
  EXPECT_NE(irsend.encodeRC5X(0x1F, 0x7F, true),
            irsend.encodeRC5(0x1F, 0x7F, true));
}

// Tests for toggleRC5().
TEST(TestToggleRC5, GeneralUse) {
  IRsendTest irsend(4);

  EXPECT_EQ(0x800, irsend.toggleRC5(0x0));
  EXPECT_EQ(0x0, irsend.toggleRC5(0x800));
  EXPECT_EQ(0x0, irsend.toggleRC5(irsend.toggleRC5(0x0)));
  EXPECT_EQ(irsend.encodeRC5(0x5, 0x35, false),
            irsend.toggleRC5(irsend.encodeRC5(0x5, 0x35, true)));
}

// Tests for sendRC5().

// Test sending typical RC-5 & RC-5X data only.
TEST(TestSendRC5, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendRC5(0x0, kRC5Bits);
  EXPECT_EQ(
      "f36000d25"
      "m889s889m1778s889m889s889m889s889m889s889m889s889m889"
      "s889m889s889m889s889m889s889m889s889m889s889m889s90664",
      irsend.outputStr());

  irsend.reset();
  irsend.sendRC5(0x1AAA, kRC5Bits);
  EXPECT_EQ(
      "f36000d25"
      "m889s889m889s889m1778s1778m1778s1778m1778s1778"
      "m1778s1778m1778s1778m1778s90664",
      irsend.outputStr());

  irsend.reset();
  irsend.sendRC5(0x175, kRC5Bits);
  EXPECT_EQ(
      "f36000d25"
      "m889s889m1778s889m889s889m889s1778m1778s1778"
      "m889s889m889s889m1778s1778m1778s1778m889s89775",
      irsend.outputStr());

  irsend.reset();
  irsend.sendRC5(0x3FFF, kRC5Bits);
  EXPECT_EQ(
      "f36000d25"
      "m889s889m889s889m889s889m889s889m889s889m889s889m889s889"
      "m889s889m889s889m889s889m889s889m889s889m889s889m889s89775",
      irsend.outputStr());

  irsend.reset();
  irsend.sendRC5(0x0, kRC5XBits);
  EXPECT_EQ(
      "f36000d25"
      "m889s889m1778s889m889s889m889s889m889s889m889s889m889"
      "s889m889s889m889s889m889s889m889s889m889s889m889s90664",
      irsend.outputStr());

  irsend.reset();
  irsend.sendRC5(0x1AAA, kRC5XBits);
  EXPECT_EQ(
      "f36000d25"
      "m1778s1778m1778s1778m1778s1778m1778"
      "s1778m1778s1778m1778s1778m1778s90664",
      irsend.outputStr());

  irsend.reset();
  irsend.sendRC5(0x175, kRC5XBits);
  EXPECT_EQ(
      "f36000d25"
      "m889s889m1778s889m889s889m889s1778m1778s1778"
      "m889s889m889s889m1778s1778m1778s1778m889s89775",
      irsend.outputStr());

  irsend.reset();
  irsend.sendRC5(0x3FFF, kRC5XBits);
  EXPECT_EQ(
      "f36000d25"
      "m1778s1778m889s889m889s889m889s889m889s889m889s889m889"
      "s889m889s889m889s889m889s889m889s889m889s889m889s89775",
      irsend.outputStr());
}

// Test sending RC-5 & RC-5X with different repeats.
TEST(TestSendRC5, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendRC5(0x175, kRC5Bits, 1);
  EXPECT_EQ(
      "f36000d25"
      "m889s889m1778s889m889s889m889s1778m1778s1778"
      "m889s889m889s889m1778s1778m1778s1778m889s90664"
      "m889s889m1778s889m889s889m889s1778m1778s1778"
      "m889s889m889s889m1778s1778m1778s1778m889s88886",
      irsend.outputStr());

  irsend.reset();
  irsend.sendRC5(0x175, kRC5Bits, 2);
  EXPECT_EQ(
      "f36000d25"
      "m889s889m1778s889m889s889m889s1778m1778s1778"
      "m889s889m889s889m1778s1778m1778s1778m889s90664"
      "m889s889m1778s889m889s889m889s1778m1778s1778"
      "m889s889m889s889m1778s1778m1778s1778m889s89775"
      "m889s889m1778s889m889s889m889s1778m1778s1778"
      "m889s889m889s889m1778s1778m1778s1778m889s88886",
      irsend.outputStr());

  irsend.reset();
  irsend.sendRC5(0x175, kRC5XBits, 1);
  EXPECT_EQ(
      "f36000d25"
      "m889s889m1778s889m889s889m889s1778m1778s1778"
      "m889s889m889s889m1778s1778m1778s1778m889s90664"
      "m889s889m1778s889m889s889m889s1778m1778s1778"
      "m889s889m889s889m1778s1778m1778s1778m889s88886",
      irsend.outputStr());

  irsend.reset();
  irsend.sendRC5(0x1175, kRC5XBits, 2);
  EXPECT_EQ(
      "f36000d25"
      "m1778s889m889s889m889s889m889s1778m1778s1778"
      "m889s889m889s889m1778s1778m1778s1778m889s90664"
      "m1778s889m889s889m889s889m889s1778m1778s1778"
      "m889s889m889s889m1778s1778m1778s1778m889s89775"
      "m1778s889m889s889m889s889m889s1778m1778s1778"
      "m889s889m889s889m1778s1778m1778s1778m889s88886",
      irsend.outputStr());
}
// Tests for decodeRC5().

// Decode normal RC-5/RC5X messages.
TEST(TestDecodeRC5, NormalDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal RC-5 12-bit message.
  irsend.reset();
  irsend.sendRC5(0x175);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC5(&irsend.capture, kRC5Bits, true));
  EXPECT_EQ(RC5, irsend.capture.decode_type);
  EXPECT_EQ(kRC5Bits, irsend.capture.bits);
  EXPECT_EQ(0x175, irsend.capture.value);
  EXPECT_EQ(0x05, irsend.capture.address);
  EXPECT_EQ(0x35, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal RC-5 12-bit message decoded as RC5-X.
  irsend.reset();
  irsend.sendRC5(0x175);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC5(&irsend.capture, kRC5XBits, true));
  EXPECT_EQ(RC5, irsend.capture.decode_type);
  EXPECT_EQ(kRC5Bits, irsend.capture.bits);
  EXPECT_EQ(0x175, irsend.capture.value);
  EXPECT_EQ(0x05, irsend.capture.address);
  EXPECT_EQ(0x35, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // A RC-5X 13-bit message but with a value that is valid for RC-5 decoded
  // as RC5-X.
  irsend.reset();
  irsend.sendRC5(0x175, kRC5XBits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC5(&irsend.capture, kRC5XBits, true));
  EXPECT_EQ(RC5, irsend.capture.decode_type);
  EXPECT_EQ(kRC5Bits, irsend.capture.bits);
  EXPECT_EQ(0x175, irsend.capture.value);
  EXPECT_EQ(0x05, irsend.capture.address);
  EXPECT_EQ(0x35, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Synthesised Normal RC-5 12-bit message.
  irsend.reset();
  irsend.sendRC5(irsend.encodeRC5(0x00, 0x0B, true));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC5(&irsend.capture, kRC5Bits, true));
  EXPECT_EQ(RC5, irsend.capture.decode_type);
  EXPECT_EQ(kRC5Bits, irsend.capture.bits);
  EXPECT_EQ(0x80B, irsend.capture.value);
  EXPECT_EQ(0x00, irsend.capture.address);
  EXPECT_EQ(0x0B, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Synthesised Normal RC-5X 13-bit message.
  irsend.reset();
  irsend.sendRC5(irsend.encodeRC5X(0x02, 0x41, true), kRC5XBits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC5(&irsend.capture, kRC5XBits, true));
  EXPECT_EQ(RC5X, irsend.capture.decode_type);
  EXPECT_EQ(kRC5XBits, irsend.capture.bits);
  EXPECT_EQ(0x1881, irsend.capture.value);
  EXPECT_EQ(0x02, irsend.capture.address);
  EXPECT_EQ(0x41, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Synthesised Normal RC-5X 13-bit message should fail at being decoded
  // as a normal RC-5 (12 bit) message.
  irsend.reset();
  irsend.sendRC5(irsend.encodeRC5X(0x02, 0x41, true), kRC5XBits);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeRC5(&irsend.capture, kRC5Bits, true));
}

// Decode normal repeated RC5 messages.
TEST(TestDecodeRC5, NormalDecodeWithRepeatAndStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal RC-5 12-bit (even) message with one repeat.
  irsend.reset();
  irsend.sendRC5(0x174, kRC5Bits, 1);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC5(&irsend.capture, kRC5Bits, true));
  EXPECT_EQ(RC5, irsend.capture.decode_type);
  EXPECT_EQ(kRC5Bits, irsend.capture.bits);
  EXPECT_EQ(0x174, irsend.capture.value);
  EXPECT_EQ(0x05, irsend.capture.address);
  EXPECT_EQ(0x34, irsend.capture.command);

  // Normal RC-5 12-bit (odd) message with one repeat.
  irsend.reset();
  irsend.sendRC5(0x175, kRC5Bits, 1);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC5(&irsend.capture, kRC5Bits, true));
  EXPECT_EQ(RC5, irsend.capture.decode_type);
  EXPECT_EQ(kRC5Bits, irsend.capture.bits);
  EXPECT_EQ(0x175, irsend.capture.value);
  EXPECT_EQ(0x05, irsend.capture.address);
  EXPECT_EQ(0x35, irsend.capture.command);

  // Synthesised Normal RC-5X 13-bit message with 2 repeats.
  irsend.reset();
  irsend.sendRC5(irsend.encodeRC5X(0x02, 0x41, true), kRC5XBits, 2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC5(&irsend.capture, kRC5XBits, true));
  EXPECT_EQ(RC5X, irsend.capture.decode_type);
  EXPECT_EQ(kRC5XBits, irsend.capture.bits);
  EXPECT_EQ(0x1881, irsend.capture.value);
  EXPECT_EQ(0x02, irsend.capture.address);
  EXPECT_EQ(0x41, irsend.capture.command);
}

// Decode unsupported RC5 messages.
TEST(TestDecodeRC5, DecodeWithNonStrictValues) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  irsend.sendRC5(0xFA, 8);  // Illegal value RC5 8-bit message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeRC5(&irsend.capture, kRC5Bits, true));
  ASSERT_FALSE(irrecv.decodeRC5(&irsend.capture, kRC5XBits, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeRC5(&irsend.capture, 8, false));
  EXPECT_EQ(RC5, irsend.capture.decode_type);
  EXPECT_EQ(8, irsend.capture.bits);
  EXPECT_EQ(0xFA, irsend.capture.value);
  EXPECT_EQ(0x3, irsend.capture.address);
  EXPECT_EQ(0x3A, irsend.capture.command);

  irsend.reset();
  irsend.sendRC5(0x12345678, 32);  // Illegal size RC5 32-bit message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeRC5(&irsend.capture, kRC5Bits, true));
  ASSERT_FALSE(irrecv.decodeRC5(&irsend.capture, kRC5XBits, true));

  irsend.makeDecodeResult();
  // Should fail with strict when we ask for the wrong bit size.
  ASSERT_FALSE(irrecv.decodeRC5(&irsend.capture, 32, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeRC5(&irsend.capture, 32, false));
  EXPECT_EQ(RC5, irsend.capture.decode_type);
  EXPECT_EQ(31, irsend.capture.bits);
  EXPECT_EQ(0x12345678, irsend.capture.value);

  irsend.reset();
  irsend.sendRC5(0x87654321, 32);  // Illegal size RC5 32-bit message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeRC5(&irsend.capture, kRC5Bits, true));
  ASSERT_FALSE(irrecv.decodeRC5(&irsend.capture, kRC5XBits, true));

  irsend.makeDecodeResult();
  // Should fail with strict when we ask for the wrong bit size.
  ASSERT_FALSE(irrecv.decodeRC5(&irsend.capture, 32, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeRC5(&irsend.capture, 32, false));
  EXPECT_EQ(RC5X, irsend.capture.decode_type);
  EXPECT_EQ(32, irsend.capture.bits);
  EXPECT_EQ(0x87654321, irsend.capture.value);
}

// Decode (non-standard) 64-bit messages.
TEST(TestDecodeRC5, Decode64BitMessages) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Illegal size RC-5 64-bit message.
  irsend.sendRC5(0xFFFFFFFFFFFFFFFF, 64);
  irsend.makeDecodeResult();
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeRC5(&irsend.capture, 64, false));
  EXPECT_EQ(RC5X, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, irsend.capture.value);
}

// Fail to decode a non-RC-5 example via GlobalCache
TEST(TestDecodeRC5, FailToDecodeNonRC5Example) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  uint16_t gc_test[39] = {38000, 1,  1,  322, 162, 20, 61,  20, 61, 20,
                          20,    20, 20, 20,  20,  20, 127, 20, 61, 9,
                          20,    20, 61, 20,  20,  20, 61,  20, 61, 20,
                          61,    20, 20, 20,  20,  20, 20,  20, 884};
  irsend.sendGC(gc_test, 39);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeRC5(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeRC5(&irsend.capture, kRC5Bits, false));
}

//                      RRRRRR   CCCCC            666
//                      RR   RR CC    C          66
//                      RRRRRR  CC       _____  666666
//                      RR  RR  CC    C         66   66
//                      RR   RR  CCCCC           66666

// Tests for encodeRC6().
TEST(TestEncodeRC6, Mode0Encoding) {
  IRsendTest irsend(4);
  EXPECT_EQ(0x0, irsend.encodeRC6(0, 0, kRC6Mode0Bits));
  EXPECT_EQ(0x1234, irsend.encodeRC6(0x12, 0x34, kRC6Mode0Bits));
  EXPECT_EQ(0x12345, irsend.encodeRC6(0x123, 0x45, kRC6Mode0Bits));
  EXPECT_EQ(0xFFFFF, irsend.encodeRC6(0xFFF, 0xFF, kRC6Mode0Bits));
  EXPECT_EQ(0xFFF00, irsend.encodeRC6(0xFFFF, 0x00, kRC6Mode0Bits));
  EXPECT_EQ(0xFF, irsend.encodeRC6(0x00, 0xFF, kRC6Mode0Bits));
}

TEST(TestEncodeRC6, 36BitEncoding) {
  IRsendTest irsend(4);
  EXPECT_EQ(0x0, irsend.encodeRC6(0, 0, kRC6_36Bits));
  EXPECT_EQ(0x1234, irsend.encodeRC6(0x12, 0x34, kRC6_36Bits));
  EXPECT_EQ(0x123456789, irsend.encodeRC6(0x1234567, 0x89, kRC6_36Bits));
  EXPECT_EQ(0xFFFFFFFFF, irsend.encodeRC6(0xFFFFFFF, 0xFF, kRC6_36Bits));
  EXPECT_EQ(0xFFFFFFFFF, irsend.encodeRC6(0xFFFFFFFF, 0xFF, kRC6_36Bits));
  EXPECT_EQ(0xFFFFFFF00, irsend.encodeRC6(0xFFFFFFF, 0x00, kRC6_36Bits));
  EXPECT_EQ(0xFF, irsend.encodeRC6(0x0, 0xFF, kRC6_36Bits));
  EXPECT_EQ(0xFFFFFFFFF, irsend.encodeRC6(0xFFFFFFFF, 0xFF, kRC6_36Bits));
}

// Tests for toggleRC6().

// Normal use (RC-6 Mode 0)
TEST(TestToggleRC6, Mode0) {
  IRsendTest irsend(4);

  EXPECT_EQ(0x10000, irsend.toggleRC6(0x0));
  EXPECT_EQ(irsend.toggleRC6(0x0), irsend.toggleRC6(0x0, kRC6Mode0Bits));
  EXPECT_EQ(0x0, irsend.toggleRC6(0x10000));
  EXPECT_EQ(0x0, irsend.toggleRC6(irsend.toggleRC6(0x0)));
}

// RC-6 36-bit use (Xbox 360)
TEST(TestToggleRC6, 36BitUse) {
  IRsendTest irsend(4);

  EXPECT_EQ(0x8000, irsend.toggleRC6(0x0, kRC6_36Bits));
  EXPECT_EQ(0x0, irsend.toggleRC6(0x8000, kRC6_36Bits));
  EXPECT_EQ(0x0,
            irsend.toggleRC6(irsend.toggleRC6(0x0, kRC6_36Bits), kRC6_36Bits));
}

// Tests for sendRC6().

// Test sending typical RC-6 Mode-0 data only.
TEST(TestSendRC6, SendMode0DataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendRC6(0x0);
  EXPECT_EQ(
      "f36000d33"
      "m2664s888"
      "m444s888m444s444m444s444m444s888m888s444m444s444m444s444"
      "m444s444m444s444m444s444m444s444m444s444m444s444m444s444"
      "m444s444m444s444m444s444m444s444m444s444m444s444m444"
      "s83028",
      irsend.outputStr());

  irsend.reset();
  irsend.sendRC6(0x1FFFF);
  EXPECT_EQ(
      "f36000d33"
      "m2664s888"
      "m444s888m444s444m444s444m1332s888m444s444m444s444m444s444"
      "m444s444m444s444m444s444m444s444m444s444m444s444m444s444"
      "m444s444m444s444m444s444m444s444m444s444m444"
      "s83472",
      irsend.outputStr());

  irsend.reset();
  irsend.sendRC6(0x15555);
  EXPECT_EQ(
      "f36000d33"
      "m2664s888"
      "m444s888m444s444m444s444m1332s1332m888s888m888s888"
      "m888s888m888s888m888s888m888s888m888s888m888"
      "s83472",
      irsend.outputStr());
}

// Test sending typical RC-6 36-bit data only.
TEST(TestSendRC6, Send36BitDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendRC6(0x0, kRC6_36Bits);
  EXPECT_EQ(
      "f36000d33"
      "m2664s888"
      "m444s888m444s444m444s444m444"
      "s888m888"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444"
      "s83028",
      irsend.outputStr());

  irsend.reset();
  irsend.sendRC6(0xFFFFFFFFF, kRC6_36Bits);
  EXPECT_EQ(
      "f36000d33"
      "m2664s888"
      "m444s444m444s444m444s444m444s444"
      "m888s888"
      "m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444s444"
      "m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444s444"
      "m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444s444"
      "m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444"
      "s83472",
      irsend.outputStr());

  irsend.reset();
  irsend.sendRC6(0xAAAAAAAAAA, kRC6_36Bits);
  EXPECT_EQ(
      "f36000d33"
      "m2664s888m444s444m444s888m888"
      "s1332m1332"
      "s888m888s888m888s888m888s888m888s888m888s888m888s888m888s888m888s888m888"
      "s888m888s888m888s888m888s888m888s888m888s888m888s888m444s83028",
      irsend.outputStr());

  irsend.reset();
  irsend.sendRC6(0xC800F740C, kRC6_36Bits);  // Xbox 360 OnOff code
  EXPECT_EQ(
      "f36000d33"
      "m2664s888"
      "m444s444m444s444m444s888m444"
      "s888m1332"
      "s888m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444"
      "s444m444s444m444s444m888s444m444s444m444s444m444s888m888s444m444"
      "s444m444s888m888s888m444s444m444s444m444s444m444s444m444s444m888"
      "s444m444s888m444s444m444s83028",
      irsend.outputStr());
  irsend.reset();
  irsend.sendRC6(irsend.toggleRC6(0xC800F740C, kRC6_36Bits),
                 kRC6_36Bits);  // Xbox 360 OnOff code (toggled)
  EXPECT_EQ(
      "f36000d33"
      "m2664s888"
      "m444s444m444s444m444s888m444"
      "s888m1332"
      "s888m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444"
      "s444m444s444m444s444m888s444m444s444m444s444m444s444m444s444m444"
      "s444m444s444m444s888m888s888m444s444m444s444m444s444m444s444m444"
      "s444m888s444m444s888m444s444m444s83028",
      irsend.outputStr());
}

// Test sending RC-6 Mode 0 with different repeats.
TEST(TestSendRC6, SendMode0WithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendRC6(0x175, kRC6Mode0Bits, 0);
  EXPECT_EQ(
      "f36000d33"
      "m2664s888"
      "m444s888m444s444m444s444m444"
      "s888m888"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m888s888m888"
      "s444m444s444m444s888m888s888m888s83472",
      irsend.outputStr());

  irsend.reset();
  irsend.sendRC6(0x175, kRC6Mode0Bits, 1);
  EXPECT_EQ(
      "f36000d33"
      "m2664s888"
      "m444s888m444s444m444s444m444"
      "s888m888"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m888s888m888"
      "s444m444s444m444s888m888s888m888s83472"
      "m2664s888"
      "m444s888m444s444m444s444m444"
      "s888m888"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m888s888m888"
      "s444m444s444m444s888m888s888m888s83472",
      irsend.outputStr());

  irsend.reset();
  irsend.sendRC6(0x175, kRC6Mode0Bits, 2);
  EXPECT_EQ(
      "f36000d33"
      "m2664s888"
      "m444s888m444s444m444s444m444"
      "s888m888"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m888s888m888"
      "s444m444s444m444s888m888s888m888s83472"
      "m2664s888"
      "m444s888m444s444m444s444m444"
      "s888m888"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m888s888m888"
      "s444m444s444m444s888m888s888m888s83472"
      "m2664s888"
      "m444s888m444s444m444s444m444"
      "s888m888"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m888s888m888"
      "s444m444s444m444s888m888s888m888s83472",
      irsend.outputStr());
}

// Test sending RC-6 36-bit with different repeats.
TEST(TestSendRC6, Send36BitWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendRC6(0x175, kRC6_36Bits, 0);
  EXPECT_EQ(
      "f36000d33"
      "m2664s888"
      "m444s888m444s444m444s444m444"
      "s888m888"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m888s888m888"
      "s444m444s444m444s888m888s888m888s83472",
      irsend.outputStr());

  irsend.reset();
  irsend.sendRC6(0x175, kRC6_36Bits, 1);
  EXPECT_EQ(
      "f36000d33"
      "m2664s888"
      "m444s888m444s444m444s444m444"
      "s888m888"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m888s888m888"
      "s444m444s444m444s888m888s888m888s83472"
      "m2664s888"
      "m444s888m444s444m444s444m444"
      "s888m888"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m888s888m888"
      "s444m444s444m444s888m888s888m888s83472",
      irsend.outputStr());

  irsend.reset();
  irsend.sendRC6(0x175, kRC6_36Bits, 2);
  EXPECT_EQ(
      "f36000d33"
      "m2664s888"
      "m444s888m444s444m444s444m444"
      "s888m888"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m888s888m888"
      "s444m444s444m444s888m888s888m888s83472"
      "m2664s888"
      "m444s888m444s444m444s444m444"
      "s888m888"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m888s888m888"
      "s444m444s444m444s888m888s888m888s83472"
      "m2664s888"
      "m444s888m444s444m444s444m444"
      "s888m888"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m444s444m444"
      "s444m444s444m444s444m444s444m444s444m444s444m444s444m888s888m888"
      "s444m444s444m444s888m888s888m888s83472",
      irsend.outputStr());
}

// Tests for decodeRC6().

// Decode normal RC-6 Mode 0 messages.
TEST(TestDecodeRC6, NormalMode0DecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal RC-6 Mode 0 (20-bit) message.
  irsend.reset();
  irsend.sendRC6(0x175);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC6(&irsend.capture, kRC6Mode0Bits, true));
  EXPECT_EQ(RC6, irsend.capture.decode_type);
  EXPECT_EQ(kRC6Mode0Bits, irsend.capture.bits);
  EXPECT_EQ(0x175, irsend.capture.value);
  EXPECT_EQ(0x01, irsend.capture.address);
  EXPECT_EQ(0x75, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal Synthesised RC-6 Mode 0 (20-bit) message.
  irsend.reset();
  irsend.sendRC6(irsend.encodeRC6(0x1234567, 0x89, kRC6Mode0Bits));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC6(&irsend.capture, kRC6Mode0Bits, true));
  EXPECT_EQ(RC6, irsend.capture.decode_type);
  EXPECT_EQ(kRC6Mode0Bits, irsend.capture.bits);
  EXPECT_EQ(0x56789, irsend.capture.value);
  EXPECT_EQ(0x567, irsend.capture.address);
  EXPECT_EQ(0x89, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Same again, but encoded manually.
  irsend.reset();
  irsend.sendRC6(0x123456789, kRC6Mode0Bits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC6(&irsend.capture, kRC6Mode0Bits, true));
  EXPECT_EQ(RC6, irsend.capture.decode_type);
  EXPECT_EQ(kRC6Mode0Bits, irsend.capture.bits);
  EXPECT_EQ(0x56789, irsend.capture.value);
  EXPECT_EQ(0x567, irsend.capture.address);
  EXPECT_EQ(0x89, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode normal RC-6 36-bit messages.
TEST(TestDecodeRC6, Normal36BitDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal RC-6 36-bit message.
  irsend.reset();
  irsend.sendRC6(0x175, kRC6_36Bits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC6(&irsend.capture, kRC6_36Bits, true));
  EXPECT_EQ(RC6, irsend.capture.decode_type);
  EXPECT_EQ(kRC6_36Bits, irsend.capture.bits);
  EXPECT_EQ(0x175, irsend.capture.value);
  EXPECT_EQ(0x01, irsend.capture.address);
  EXPECT_EQ(0x75, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal Synthesised RC-6 36-bit message.
  irsend.reset();
  irsend.sendRC6(irsend.encodeRC6(0x1234567, 0x89, kRC6_36Bits), kRC6_36Bits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC6(&irsend.capture, kRC6_36Bits, true));
  EXPECT_EQ(RC6, irsend.capture.decode_type);
  EXPECT_EQ(kRC6_36Bits, irsend.capture.bits);
  EXPECT_EQ(0x123456789, irsend.capture.value);
  EXPECT_EQ(0x1234567, irsend.capture.address);
  EXPECT_EQ(0x89, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode normal repeated RC6 Mode 0 messages.
TEST(TestDecodeRC6, NormalMode0DecodeWithRepeatAndStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal RC-6 Mode 0 (20-bit) even message with one repeat.
  irsend.reset();
  irsend.sendRC6(0x174, kRC6Mode0Bits, 1);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC6(&irsend.capture, kRC6Mode0Bits, true));
  EXPECT_EQ(RC6, irsend.capture.decode_type);
  EXPECT_EQ(kRC6Mode0Bits, irsend.capture.bits);
  EXPECT_EQ(0x174, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x74, irsend.capture.command);

  // Normal RC-6 Mode 0 (20-bit) odd message with one repeat.
  irsend.reset();
  irsend.sendRC6(0x175, kRC6Mode0Bits, 1);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC6(&irsend.capture, kRC6Mode0Bits, true));
  EXPECT_EQ(RC6, irsend.capture.decode_type);
  EXPECT_EQ(kRC6Mode0Bits, irsend.capture.bits);
  EXPECT_EQ(0x175, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x75, irsend.capture.command);
}

// Decode normal repeated RC6 36-bit messages.
TEST(TestDecodeRC6, Normal36BitDecodeWithRepeatAndStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal RC-6 36-bit odd message with one repeat.
  irsend.reset();
  irsend.sendRC6(0x175, kRC6_36Bits, 1);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC6(&irsend.capture, kRC6_36Bits, true));
  EXPECT_EQ(RC6, irsend.capture.decode_type);
  EXPECT_EQ(kRC6_36Bits, irsend.capture.bits);
  EXPECT_EQ(0x175, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x75, irsend.capture.command);

  // Normal RC-6 36-bit even message with one repeat.
  irsend.reset();
  irsend.sendRC6(0x174, kRC6_36Bits, 1);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC6(&irsend.capture, kRC6_36Bits, true));
  EXPECT_EQ(RC6, irsend.capture.decode_type);
  EXPECT_EQ(kRC6_36Bits, irsend.capture.bits);
  EXPECT_EQ(0x174, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x74, irsend.capture.command);
}

// Decode RC-6 messages without strict.
TEST(TestDecodeRC6, NormalDecodeWithoutStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal RC-6 Mode 0 (20-bit) message.
  irsend.reset();
  irsend.sendRC6(0x175, kRC6Mode0Bits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC6(&irsend.capture));
  EXPECT_EQ(RC6, irsend.capture.decode_type);
  EXPECT_EQ(kRC6Mode0Bits, irsend.capture.bits);
  EXPECT_EQ(0x175, irsend.capture.value);
  EXPECT_EQ(0x01, irsend.capture.address);
  EXPECT_EQ(0x75, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal RC-6 Mode 0 (20-bit) message.
  irsend.reset();
  irsend.sendRC6(0x174, kRC6Mode0Bits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC6(&irsend.capture));
  EXPECT_EQ(RC6, irsend.capture.decode_type);
  EXPECT_EQ(kRC6Mode0Bits, irsend.capture.bits);
  EXPECT_EQ(0x174, irsend.capture.value);
  EXPECT_EQ(0x01, irsend.capture.address);
  EXPECT_EQ(0x74, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal RC-6 36-bit message.
  irsend.reset();
  irsend.sendRC6(0x174, kRC6_36Bits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC6(&irsend.capture));
  EXPECT_EQ(RC6, irsend.capture.decode_type);
  EXPECT_EQ(kRC6_36Bits, irsend.capture.bits);
  EXPECT_EQ(0x174, irsend.capture.value);
  EXPECT_EQ(0x01, irsend.capture.address);
  EXPECT_EQ(0x74, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal Synthesised RC-6 36-bit message.
  irsend.reset();
  irsend.sendRC6(irsend.encodeRC6(0x1234567, 0x89, kRC6_36Bits), kRC6_36Bits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRC6(&irsend.capture));
  EXPECT_EQ(RC6, irsend.capture.decode_type);
  EXPECT_EQ(kRC6_36Bits, irsend.capture.bits);
  EXPECT_EQ(0x123456789, irsend.capture.value);
  EXPECT_EQ(0x1234567, irsend.capture.address);
  EXPECT_EQ(0x89, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode a 'real' example via GlobalCache
TEST(TestDecodeRC6, Decode36BitGlobalCacheExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Xbox-360 Power On from Global Cache.
  uint16_t gc_test[65] = {
      36000, 1,  1,  96, 32, 16, 16, 16, 16, 16, 32, 16, 32, 48,  32, 16, 16,
      16,    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,  16, 16, 16,
      16,    32, 16, 16, 16, 16, 16, 16, 32, 32, 16, 16, 16, 16,  32, 32, 32,
      16,    16, 16, 16, 16, 16, 32, 32, 32, 32, 32, 32, 16, 2476};
  irsend.sendGC(gc_test, 65);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodeRC6(&irsend.capture, kRC6_36Bits));
  EXPECT_EQ(RC6, irsend.capture.decode_type);
  EXPECT_EQ(kRC6_36Bits, irsend.capture.bits);
  EXPECT_EQ(0xC800F742A, irsend.capture.value);
  EXPECT_EQ(0xC800F74, irsend.capture.address);
  EXPECT_EQ(0x2A, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Fail to decode a non-RC-6 example via GlobalCache
TEST(TestDecodeRC5, FailToDecodeNonRC6Example) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  uint16_t gc_test[39] = {38000, 1,  1,  322, 162, 20, 61,  20, 61, 20,
                          20,    20, 20, 20,  20,  20, 127, 20, 61, 9,
                          20,    20, 61, 20,  20,  20, 61,  20, 61, 20,
                          61,    20, 20, 20,  20,  20, 20,  20, 884};
  irsend.sendGC(gc_test, 39);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeRC6(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeRC6(&irsend.capture, kRC6Mode0Bits, true));
  ASSERT_FALSE(irrecv.decodeRC6(&irsend.capture, kRC6Mode0Bits, false));
  ASSERT_FALSE(irrecv.decodeRC6(&irsend.capture, kRC6_36Bits, true));
  ASSERT_FALSE(irrecv.decodeRC6(&irsend.capture, kRC6_36Bits, false));

  irsend.reset();
  irsend.sendRC5(0x0);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeRC6(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeRC6(&irsend.capture, kRC6Mode0Bits, true));
  ASSERT_FALSE(irrecv.decodeRC6(&irsend.capture, kRC6Mode0Bits, false));
  ASSERT_FALSE(irrecv.decodeRC6(&irsend.capture, kRC6_36Bits, true));
  ASSERT_FALSE(irrecv.decodeRC6(&irsend.capture, kRC6_36Bits, false));
}
