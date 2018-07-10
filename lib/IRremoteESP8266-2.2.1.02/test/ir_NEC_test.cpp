// Copyright 2017 David Conran

#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendNEC().

// Test sending typical data only.
TEST(TestSendNEC, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();
  irsend.sendNEC(0);
  EXPECT_EQ("m8960s4480m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
            "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
            "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
            "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
            "m560s560m560s108080", irsend.outputStr());
  irsend.sendNEC(0xAA00FF55);
  EXPECT_EQ("m8960s4480m560s1680m560s560m560s1680m560s560m560s1680m560s560"
            "m560s1680m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
            "m560s560m560s560m560s1680m560s1680m560s1680m560s1680m560s1680"
            "m560s1680m560s1680m560s1680m560s560m560s1680m560s560m560s1680"
            "m560s560m560s1680m560s560m560s1680m560s108080",
            irsend.outputStr());
}

// Test sending different bit lengths.
TEST(TestSendNEC, SendSmallData) {
  IRsendTest irsend(4);
  irsend.begin();
  irsend.sendNEC(0xA, 4);  // Send only 4 data bits.
  EXPECT_EQ("m8960s4480m560s1680m560s560m560s1680m560s560m560s108080",
            irsend.outputStr());
  irsend.sendNEC(0, 8);  // Send only 8 data bits.
  EXPECT_EQ("m8960s4480m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
            "m560s560m560s108080", irsend.outputStr());
  irsend.sendNEC(0x1234567890ABCDEF, 64);  // Send 64 data bits.
  EXPECT_EQ("m8960s4480m560s560m560s560m560s560m560s1680m560s560m560s560"
            "m560s1680m560s560m560s560m560s560m560s1680m560s1680m560s560"
            "m560s1680m560s560m560s560m560s560m560s1680m560s560m560s1680"
            "m560s560m560s1680m560s1680m560s560m560s560m560s1680m560s1680"
            "m560s1680m560s1680m560s560m560s560m560s560m560s1680m560s560"
            "m560s560m560s1680m560s560m560s560m560s560m560s560m560s1680m560s560"
            "m560s1680m560s560m560s1680m560s560m560s1680m560s1680m560s1680"
            "m560s1680m560s560m560s560m560s1680m560s1680m560s560m560s1680"
            "m560s1680m560s1680m560s1680m560s560m560s1680m560s1680m560s1680"
            "m560s1680m560s108080", irsend.outputStr());
}

// Test sending with repeats.
TEST(TestSendNEC, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();
  irsend.sendNEC(0, 8, 0);  // Send a command with 0 repeats.
  EXPECT_EQ("m8960s4480m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
            "m560s560m560s108080", irsend.outputStr());
  irsend.sendNEC(0xAA, 8, 1);  // Send a command with 1 repeat.
  EXPECT_EQ("m8960s4480m560s1680m560s560m560s1680m560s560m560s1680m560s560"
            "m560s1680m560s560m560s108080"
            "m8960s2240m560s108080",
            irsend.outputStr());
  irsend.sendNEC(0xAA, 8, 3);  // Send a command with 3 repeats.
  EXPECT_EQ("m8960s4480m560s1680m560s560m560s1680m560s560m560s1680m560s560"
            "m560s1680m560s560m560s108080"
            "m8960s2240m560s108080"
            "m8960s2240m560s108080"
            "m8960s2240m560s108080",
            irsend.outputStr());
}

// Tests for encodeNEC().

TEST(TestEncodeNEC, NormalNECEncoding) {
  IRsendTest irsend(4);
  EXPECT_EQ(0x807F40BF, irsend.encodeNEC(1, 2));
  EXPECT_EQ(0x9A656897, irsend.encodeNEC(0x59, 0x16));
}

TEST(TestEncodeNEC, ExtendedNECEncoding) {
  IRsendTest irsend(4);
  EXPECT_EQ(0x9A806897, irsend.encodeNEC(0x159, 0x16));
}

TEST(TestEncodeNEC, CommandTrimmedTo8Bits) {
  IRsendTest irsend(4);
  EXPECT_EQ(irsend.encodeNEC(0x1, 0x2), irsend.encodeNEC(0x1, 0xF02));
  EXPECT_EQ(irsend.encodeNEC(0xFFF0, 0x2), irsend.encodeNEC(0xFFF0, 0xF02));
}

// Tests for decodeNEC().
// Decode normal NEC messages.
TEST(TestDecodeNEC, NormalNECDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Synthesised Normal NEC message.
  irsend.reset();
  irsend.sendNEC(irsend.encodeNEC(0x1, 0x2));
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decodeNEC(&irsend.capture));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(NEC_BITS, irsend.capture.bits);
  EXPECT_EQ(0x807F40BF, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x2, irsend.capture.command);

  // Real-life Extended NEC code from an actual capture/decode.
  irsend.reset();
  irsend.sendNEC(0xC1A28877);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decodeNEC(&irsend.capture));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(NEC_BITS, irsend.capture.bits);
  EXPECT_EQ(0xC1A28877, irsend.capture.value);
  EXPECT_EQ(0x4583, irsend.capture.address);
  EXPECT_EQ(0x11, irsend.capture.command);

  // Test strict decoding rejects a NEC-like message.
  irsend.reset();
  irsend.sendNEC(0x0);
  irsend.makeDecodeResult();
  EXPECT_FALSE(irrecv.decodeNEC(&irsend.capture));

  // Synthesised Normal NEC message with a repeat.
  irsend.reset();
  irsend.sendNEC(irsend.encodeNEC(0x1, 0x2), 32, 1);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decodeNEC(&irsend.capture));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(NEC_BITS, irsend.capture.bits);
  EXPECT_EQ(0x807F40BF, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x2, irsend.capture.command);
}

// NEC-like messages without strict mode.
TEST(TestDecodeNEC, NormalNECDecodeWithoutStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  irsend.sendNEC(0x0);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decodeNEC(&irsend.capture, 32, false));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(NEC_BITS, irsend.capture.bits);
  EXPECT_EQ(0, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);

  irsend.reset();
  irsend.sendNEC(0x12345678);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decodeNEC(&irsend.capture, 32, false));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(NEC_BITS, irsend.capture.bits);
  EXPECT_EQ(0x12345678, irsend.capture.value);
  EXPECT_EQ(0x2C48, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);
}

// Short NEC-like messages (without strict naturally)
TEST(TestDecodeNEC, ShortNECDecodeWithoutStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  irsend.sendNEC(0x0, 16);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decodeNEC(&irsend.capture, 16, false));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(16, irsend.capture.bits);
  EXPECT_EQ(0, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);

  // Expecting less than what was sent is not valid.
  irsend.reset();
  irsend.sendNEC(0x0, 32);
  irsend.makeDecodeResult();
  EXPECT_FALSE(irrecv.decodeNEC(&irsend.capture, 16, false));

  // Send 16 bits of data, but fail because we are expecting 17.
  irsend.reset();
  irsend.sendNEC(0x0, 16);
  irsend.makeDecodeResult();
  EXPECT_FALSE(irrecv.decodeNEC(&irsend.capture, 17, false));
}

// Longer NEC-like messages (without strict naturally)
TEST(TestDecodeNEC, LongerNECDecodeWithoutStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  irsend.sendNEC(0x1234567890ABCDEF, 64);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decodeNEC(&irsend.capture, 64, false));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0x1234567890ABCDEF, irsend.capture.value);
  EXPECT_EQ(0xD509, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);

  // Send 63 bits of data, but fail because we are expecting 64.
  irsend.reset();
  irsend.sendNEC(0x0, 63);
  irsend.makeDecodeResult();
  EXPECT_FALSE(irrecv.decodeNEC(&irsend.capture, 64, false));
}

// Incorrect decoding reported in Issue #243
// Incorrect handling of decodes when there is no gap recorded at
// the end of a command when using the interrupt code. sendRaw() best emulates
// this for unit testing purposes. sendGC() and sendXXX() will add the trailing
// gap. Users won't see this in normal use.
TEST(TestDecodeNEC, NoTrailingGap_Issue243) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  uint16_t rawData[67] = {9000, 4500, 650, 550, 650, 1650, 600, 550, 650, 550,
                          600, 1650, 650, 550, 600, 1650, 650, 1650, 650, 1650,
                          600, 550, 650, 1650, 650, 1650, 650, 550, 600, 1650,
                          650, 1650, 650, 550, 650, 550, 650, 1650, 650, 550,
                          650, 550, 650, 550, 600, 550, 650, 550, 650, 550,
                          650, 1650, 600, 550, 650, 1650, 650, 1650, 650, 1650,
                          650, 1650, 650, 1650, 650, 1650, 600};
  irsend.sendRaw(rawData, 67, 38);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decodeNEC(&irsend.capture));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(NEC_BITS, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);
  EXPECT_EQ(0x6DD2, irsend.capture.address);
  EXPECT_EQ(0x2, irsend.capture.command);

  irsend.reset();
  irsend.sendRaw(rawData, 67, 38);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(NEC_BITS, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);

  // Add a zero length space to the message to test how it handles that as
  // a end of command gap.
  irsend.addGap(0);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(NEC_BITS, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);
}

// Inconsistent decoding for unknown in Issue #264
// Reported issues decoding an Apple Remote. Apple doesn't appear to respect
// or use the command structure/checks in the NEC protocol.
TEST(TestDecodeNEC, NonStrictNECDecode_Issue264) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Slightly modified example than reported due to poor timings that are too
  // far out of spec.
  uint16_t rawData[67] = {9150, 4650, 550, 600, 550, 1800, 600, 1750, 600, 1800,
                          550, 600, 550, 1800, 550, 1750, 600, 1750, 600, 1750,
                          600, 1750, 600, 1700, 600, 600, 600, 600, 550, 600,
                          600, 600, 600, 1750, 600, 1750, 600, 600, 550, 1800,
                          600, 600, 600, 600, 600, 600, 500, 600, 600, 600,
                          600, 600, 600, 1750, 600, 600, 600, 550, 600, 600,
                          600, 600, 600, 600, 600, 550, 600};

  irsend.sendRaw(rawData, 67, 38);
  irsend.makeDecodeResult();
  EXPECT_FALSE(irrecv.decodeNEC(&irsend.capture));  // Not strictly NEC
  EXPECT_TRUE(irrecv.decodeNEC(&irsend.capture, NEC_BITS, false));
  EXPECT_EQ(0x77E1A040, irsend.capture.value);

  // Do it all again, but with a normal decode.
  irsend.reset();
  irsend.sendRaw(rawData, 67, 38);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(NEC_LIKE, irsend.capture.decode_type);
  EXPECT_EQ(NEC_BITS, irsend.capture.bits);
  EXPECT_EQ(0x77E1A040, irsend.capture.value);
}

TEST(TestDecodeNEC, AutoReceiveCalibration) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Data from Issue #264, for a remote that is slightly off spec.
  // Should be matched as a NEC-like message however without self-calibrating
  // the timings of the short spaces is out.
  uint16_t rawData[67] = {9150, 4600, 650, 600, 650, 1650, 650, 1700, 650, 1750,
                          650, 600, 650, 1700, 650, 1750, 650, 1750, 650, 1700,
                          650, 1700, 650, 1700, 650, 600, 650, 600, 650, 600,
                          600, 600, 650, 1750, 650, 1750, 650, 600, 650, 1700,
                          600, 600, 700, 550, 650, 550, 650, 600, 650, 600,
                          650, 600, 650, 1750, 600, 600, 650, 600, 650, 550,
                          650, 600, 650, 600, 650, 600, 600};

  irsend.sendRaw(rawData, 67, 38);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decodeNEC(&irsend.capture, NEC_BITS, false));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(NEC_BITS, irsend.capture.bits);
  EXPECT_EQ(0x77E1A040, irsend.capture.value);
}
