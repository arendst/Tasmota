// Copyright 2017, 2019 David Conran

#include "ir_LG.h"
#include "IRac.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"


// Tests for sendLG().

// Test sending typical data only.
TEST(TestSendLG, SendDataOnly) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendLG(0x4B4AE51);
  EXPECT_EQ(
      "f38000d50"
      "m8500s4250"
      "m550s550m550s1600m550s550m550s550"
      "m550s1600m550s550m550s1600m550s1600m550s550m550s1600m550s550m550s550"
      "m550s1600m550s550m550s1600m550s550m550s1600m550s1600m550s1600m550s550"
      "m550s550m550s1600m550s550m550s1600m550s550m550s550m550s550m550s1600"
      "m550s50300",
      irsend.outputStr());

  irsend.reset();
  irsend.sendLG(0xB4B4AE51, kLg32Bits);
  EXPECT_EQ(
      "f38000d33"
      "m4480s4480"
      "m560s1680m560s560m560s1680m560s1680m560s560m560s1680m560s560m560s560"
      "m560s1680m560s560m560s1680m560s1680m560s560m560s1680m560s560m560s560"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s1680m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s560m560s560m560s1680"
      "m560s44800"
      "m8950s2250m550s96300",
      irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendLG, SendWithRepeats) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendLG(0x4B4AE51, kLgBits, 1);
  EXPECT_EQ(
      "f38000d50"
      "m8500s4250"
      "m550s550m550s1600m550s550m550s550"
      "m550s1600m550s550m550s1600m550s1600m550s550m550s1600m550s550m550s550"
      "m550s1600m550s550m550s1600m550s550m550s1600m550s1600m550s1600m550s550"
      "m550s550m550s1600m550s550m550s1600m550s550m550s550m550s550m550s1600"
      "m550s50300"
      "m8500s2250m550s96750",
      irsend.outputStr());

  irsend.reset();
  irsend.sendLG(0xB4B4AE51, kLg32Bits, 1);
  EXPECT_EQ(
      "f38000d33"
      "m4480s4480"
      "m560s1680m560s560m560s1680m560s1680m560s560m560s1680m560s560m560s560"
      "m560s1680m560s560m560s1680m560s1680m560s560m560s1680m560s560m560s560"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s1680m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s560m560s560m560s1680"
      "m560s44800"
      "m8950s2250m550s96300"
      "m8950s2250m550s96300",
      irsend.outputStr());
}

// Test sending an atypical data size.
TEST(TestSendLG, SendUnusualSize) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendLG(0x0, 31);
  EXPECT_EQ(
      "f38000d50"
      "m8500s4250"
      "m550s550m550s550m550s550m550s550m550s550m550s550m550s550m550s550"
      "m550s550m550s550m550s550m550s550m550s550m550s550m550s550m550s550"
      "m550s550m550s550m550s550m550s550m550s550m550s550m550s550m550s550"
      "m550s550m550s550m550s550m550s550m550s550m550s550m550s550"
      "m550s60650",
      irsend.outputStr());

  irsend.reset();
  irsend.sendLG(0x0, 64);
  EXPECT_EQ(
      "f38000d33"
      "m4480s4480"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s26880"
      "m8950s2250m550s96300",
      irsend.outputStr());
}

// Tests for encodeLG().

TEST(TestEncodeLG, NormalEncoding) {
  IRsendTest irsend(kGpioUnused);
  EXPECT_EQ(0x0, irsend.encodeLG(0, 0));
  EXPECT_EQ(0x100011, irsend.encodeLG(1, 1));
  EXPECT_EQ(0x100022, irsend.encodeLG(1, 2));
  EXPECT_EQ(0x43001DE, irsend.encodeLG(0x43, 0x1D));
  EXPECT_EQ(0xB4B4AE51, irsend.encodeLG(0xB4B, 0x4AE5));
  EXPECT_EQ(0xAA0055A, irsend.encodeLG(0xAA, 0x55));
  EXPECT_EQ(0xFFFFFFFC, irsend.encodeLG(0xFFFF, 0xFFFF));
}

// Tests for decodeLG().

// Decode normal LG messages.
TEST(TestDecodeLG, NormalDecodeWithStrict) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Normal LG 28-bit message.
  irsend.reset();
  irsend.sendLG(0x4B4AE51, kLgBits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(kLgBits, irsend.capture.bits);
  EXPECT_EQ(0x4B4AE51, irsend.capture.value);
  EXPECT_EQ(0x4B, irsend.capture.address);
  EXPECT_EQ(0x4AE5, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal LG 32-bit message.
  irsend.reset();
  irsend.sendLG(0xB4B4AE51, kLg32Bits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(kLg32Bits, irsend.capture.bits);
  EXPECT_EQ(0xB4B4AE51, irsend.capture.value);
  EXPECT_EQ(0xB4B, irsend.capture.address);
  EXPECT_EQ(0x4AE5, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Synthesised Normal LG 28-bit message.
  irsend.reset();
  irsend.sendLG(irsend.encodeLG(0x07, 0x99));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(kLgBits, irsend.capture.bits);
  EXPECT_EQ(0x700992, irsend.capture.value);
  EXPECT_EQ(0x07, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Synthesised Normal LG 32-bit message.
  irsend.reset();
  irsend.sendLG(irsend.encodeLG(0x800, 0x8000), kLg32Bits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(kLg32Bits, irsend.capture.bits);
  EXPECT_EQ(0x80080008, irsend.capture.value);
  EXPECT_EQ(0x800, irsend.capture.address);
  EXPECT_EQ(0x8000, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode normal repeated LG messages.
TEST(TestDecodeLG, NormalDecodeWithRepeatAndStrict) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Normal LG 28-bit message with 2 repeats.
  irsend.reset();
  irsend.sendLG(irsend.encodeLG(0x07, 0x99), kLgBits, 2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(kLgBits, irsend.capture.bits);
  EXPECT_EQ(0x700992, irsend.capture.value);
  EXPECT_EQ(0x07, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal LG 32-bit message with 2 repeats.
  irsend.reset();
  irsend.sendLG(irsend.encodeLG(0x07, 0x99), kLg32Bits, 2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(kLg32Bits, irsend.capture.bits);
  EXPECT_EQ(0x700992, irsend.capture.value);
  EXPECT_EQ(0x07, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode unsupported LG message values.
TEST(TestDecodeLG, DecodeWithNonStrictValues) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Illegal values should be rejected when strict is on.
  // Illegal LG 28-bit message value.
  irsend.reset();
  irsend.sendLG(0x1);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kStartOffset, kLgBits, true));
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kStartOffset, kLg32Bits, true));
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kStartOffset, kLg32Bits,
                               false));
  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture, kStartOffset, kLgBits, false));

  // Illegal LG 32-bit message value.
  irsend.reset();
  irsend.sendLG(0x1111111, kLg32Bits);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kStartOffset, kLg32Bits, true));
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kStartOffset, kLgBits, true));

  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture, kStartOffset, kLg32Bits, false));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(kLg32Bits, irsend.capture.bits);
  EXPECT_EQ(0x1111111, irsend.capture.value);
  EXPECT_EQ(0x11, irsend.capture.address);
  EXPECT_EQ(0x1111, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  irsend.reset();
  irsend.sendLG(0x1111111, kLg32Bits);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kStartOffset, kLgBits, false));
}

// Decode unsupported LG message sizes.
TEST(TestDecodeLG, DecodeWithNonStrictSizes) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Illegal sizes should be rejected when strict is on.
  // Illegal LG 16-bit message size.
  irsend.reset();
  irsend.sendLG(irsend.encodeLG(0x07, 0x99), 16);
  irsend.makeDecodeResult();
  // Should fail when unexpected against different bit sizes.
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kStartOffset, kLgBits, true));
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kStartOffset, kLg32Bits, true));
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kStartOffset, kLgBits, false));
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kStartOffset, kLg32Bits,
                               false));

  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture, kStartOffset, 16, false));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(16, irsend.capture.bits);
  EXPECT_EQ(0x992, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);

  // Illegal LG 36-bit message size.
  irsend.reset();
  irsend.sendLG(0x123456789, 36);  // Illegal value LG 36-bit message.
  irsend.makeDecodeResult();
  // Should fail when unexpected against different bit sizes.
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kStartOffset, kLgBits, true));
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kStartOffset, kLgBits, false));
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kStartOffset, kLg32Bits, true));
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kStartOffset, kLg32Bits,
                               false));

  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture, kStartOffset, 36, false));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(36, irsend.capture.bits);
  EXPECT_EQ(0x123456789, irsend.capture.value);
  EXPECT_EQ(0x1234, irsend.capture.address);
  EXPECT_EQ(0x5678, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode (non-standard) 64-bit messages.
TEST(TestDecodeLG, Decode64BitMessages) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  // Illegal value & size LG 64-bit message.
  irsend.sendLG(0xFFFFFFFFFFFFFFFF, 64);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kStartOffset, 64, true));
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture, kStartOffset, 64, false));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, irsend.capture.value);
  EXPECT_EQ(0xFFFFFFFF, irsend.capture.address);
  EXPECT_EQ(0xFFFF, irsend.capture.command);
}

// Decode a 'real' example via GlobalCache
TEST(TestDecodeLG, DecodeGlobalCacheExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // TODO(anyone): Find a Global Cache example of the LG 28-bit message.
  irsend.reset();
  // LG (32-bit) code from Global Cache.
  uint16_t gc_test[75] = {
      38000, 1,  69, 341, 170, 21, 64, 21, 21, 21, 64,   21,  64, 21, 21,
      21,    64, 21, 21,  21,  21, 21, 64, 21, 21, 21,   64,  21, 64, 21,
      21,    21, 64, 21,  21,  21, 21, 21, 64, 21, 21,   21,  64, 21, 21,
      21,    64, 21, 64,  21,  64, 21, 21, 21, 21, 21,   64,  21, 21, 21,
      64,    21, 21, 21,  21,  21, 21, 21, 64, 21, 1517, 341, 85, 21, 3655};
  irsend.sendGC(gc_test, 75);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture, kStartOffset, kLg32Bits, true));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(kLg32Bits, irsend.capture.bits);
  EXPECT_EQ(0xB4B4AE51, irsend.capture.value);
  EXPECT_EQ(0xB4B, irsend.capture.address);
  EXPECT_EQ(0x4AE5, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Fail to decode a non-LG example via GlobalCache
TEST(TestDecodeLG, FailToDecodeNonLGExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  // Modified a few entries to unexpected values, based on previous test case.
  uint16_t gc_test[39] = {38000, 1,  1,  322, 162, 20, 61,  20, 61, 20,
                          20,    20, 20, 20,  20,  20, 127, 20, 61, 9,
                          20,    20, 61, 20,  20,  20, 61,  20, 61, 20,
                          61,    20, 20, 20,  20,  20, 20,  20, 884};
  irsend.sendGC(gc_test, 39);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kStartOffset, kLgBits, false));
}

// Tests for sendLG2().

// Test sending typical data only.
TEST(TestSendLG2, SendDataOnly) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendLG2(0x880094D);
  EXPECT_EQ(
      "f38000d33"
      "m3200s9900"
      "m480s1600m480s550m480s550m480s550m480s1600m480s550m480s550m480s550"
      "m480s550m480s550m480s550m480s550m480s550m480s550m480s550m480s550"
      "m480s1600m480s550m480s550m480s1600m480s550m480s1600m480s550m480s550"
      "m480s1600m480s1600m480s550m480s1600"
      "m480s57230",
      irsend.outputStr());
}

TEST(TestDecodeLG2, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendLG2(0x880094D);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(LG2, irsend.capture.decode_type);
  EXPECT_EQ(kLgBits, irsend.capture.bits);
  EXPECT_EQ(0x880094D, irsend.capture.value);
}

// Verify decoding of LG variant 2 messages.
TEST(TestDecodeLG2, RealLG2Example) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  // From issue #548
  uint16_t rawData[59] = {
      3154, 9834, 520, 1634, 424, 606,  424, 568, 462, 570,  462, 1564,
      508,  568,  458, 544,  500, 546,  508, 530, 508, 532,  506, 566,
      464,  568,  460, 578,  464, 568,  464, 532, 506, 552,  474, 1592,
      506,  568,  460, 570,  462, 1564, 506, 606, 424, 1640, 424, 616,
      422,  570,  462, 1616, 460, 1584, 500, 544, 506, 1598, 490};  // UNKNOWN
                                                                    // F6D13AE8
  irsend.sendRaw(rawData, 59, 38000);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture));
  ASSERT_EQ(LG2, irsend.capture.decode_type);
  EXPECT_EQ(kLgBits, irsend.capture.bits);
  EXPECT_EQ(0x880094D, irsend.capture.value);
}

// Tests for issue reported in
// https://github.com/crankyoldgit/IRremoteESP8266/issues/620
TEST(TestDecodeLG, Issue620) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Raw data as reported in initial comment of Issue #620
  uint16_t rawData[59] = {
      8886, 4152,
      560, 1538, 532, 502, 532, 504,  530, 484,  558, 1536,
      508, 516,  558, 502, 532, 484,  558, 502,  532, 500,
      534, 508,  532, 502, 532, 1518, 558, 510,  532, 484,
      556, 486,  556, 510, 532, 1518, 558, 1560, 532, 1528,
      556, 504,  530, 506, 530, 1520, 558, 508,  534, 500,
      532, 512,  530, 484, 556, 1536, 532};  // LG 8808721
  irsend.sendRaw(rawData, 59, 38000);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(28, irsend.capture.bits);
  EXPECT_EQ(0x8808721, irsend.capture.value);
  EXPECT_EQ(0x88, irsend.capture.address);
  EXPECT_EQ(0x872, irsend.capture.command);

  irsend.reset();

  // Resend the same code as the report is a sent code doesn't decode
  // to the same message code.
  IRLgAc ac(kGpioUnused);
  irsend.sendLG(0x8808721);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(28, irsend.capture.bits);
  EXPECT_EQ(0x8808721, irsend.capture.value);
  EXPECT_EQ(0x88, irsend.capture.address);
  EXPECT_EQ(0x872, irsend.capture.command);
  ac.setRaw(irsend.capture.value);
  ASSERT_TRUE(ac.isValidLgAc());
  EXPECT_EQ("Model: 1 (GE6711AR2853M), "
            "Power: On, Mode: 0 (Cool), Temp: 22C, Fan: 2 (Medium)",
            ac.toString());
  // The following seems to match the rawData above.
  EXPECT_EQ(
      "f38000d50"
      "m8500s4250"
      "m550s1600m550s550m550s550m550s550m550s1600"
      "m550s550m550s550m550s550m550s550m550s550"
      "m550s550m550s550m550s1600m550s550m550s550"
      "m550s550m550s550m550s1600m550s1600m550s1600"
      "m550s550m550s550m550s1600m550s550m550s550"
      "m550s550m550s550m550s1600m550"
      "s55550",
      irsend.outputStr());
}

TEST(TestIRLgAcClass, SetAndGetPower) {
  IRLgAc ac(kGpioUnused);
  ac.on();
  EXPECT_TRUE(ac.getPower());
  ac.off();
  EXPECT_FALSE(ac.getPower());
  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());
  ac.setPower(false);
  EXPECT_FALSE(ac.getPower());
}

TEST(TestIRLgAcClass, SetAndGetTemp) {
  IRLgAc ac(kGpioUnused);
  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());
  ac.setTemp(kLgAcMinTemp);
  EXPECT_EQ(kLgAcMinTemp, ac.getTemp());
  ac.setTemp(kLgAcMinTemp - 1);
  EXPECT_EQ(kLgAcMinTemp, ac.getTemp());
  ac.setTemp(kLgAcMaxTemp);
  EXPECT_EQ(kLgAcMaxTemp, ac.getTemp());
  ac.setTemp(kLgAcMaxTemp + 1);
  EXPECT_EQ(kLgAcMaxTemp, ac.getTemp());
}

TEST(TestIRLgAcClass, SetAndGetMode) {
  IRLgAc ac(kGpioUnused);
  ac.setMode(kLgAcCool);
  ac.setFan(kLgAcFanAuto);
  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());
  EXPECT_EQ(kLgAcCool, ac.getMode());
  EXPECT_EQ(kLgAcFanAuto, ac.getFan());
  ac.setMode(kLgAcHeat);
  EXPECT_EQ(kLgAcHeat, ac.getMode());
  ac.setMode(kLgAcDry);
  EXPECT_EQ(kLgAcDry, ac.getMode());
}

TEST(TestIRLgAcClass, SetAndGetFan) {
  IRLgAc ac(kGpioUnused);
  ac.setMode(kLgAcCool);
  ac.setFan(kLgAcFanAuto);
  EXPECT_EQ(kLgAcFanAuto, ac.getFan());
  ac.setFan(kLgAcFanLowest);
  EXPECT_EQ(kLgAcFanLowest, ac.getFan());
  ac.setFan(kLgAcFanHigh);
  EXPECT_EQ(kLgAcFanHigh, ac.getFan());
  ac.setFan(kLgAcFanAuto + 1);
  EXPECT_EQ(kLgAcFanAuto, ac.getFan());
  ac.setFan(kLgAcFanLowest - 1);
  EXPECT_EQ(kLgAcFanAuto, ac.getFan());
}

TEST(TestIRLgAcClass, toCommon) {
  IRLgAc ac(kGpioUnused);
  ac.setPower(true);
  ac.setMode(kLgAcCool);
  ac.setTemp(20);
  ac.setFan(kLgAcFanHigh);
  // Now test it.
  ASSERT_EQ(decode_type_t::LG, ac.toCommon().protocol);
  ASSERT_EQ(lg_ac_remote_model_t::GE6711AR2853M, ac.toCommon().model);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  // Unsupported.
  ASSERT_EQ(stdAc::swingv_t::kOff, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);

  // Change models
  ac.setModel(AKB75215403);
  ASSERT_EQ(lg_ac_remote_model_t::AKB75215403, ac.toCommon().model);
}

TEST(TestIRLgAcClass, HumanReadable) {
  IRLgAc ac(kGpioUnused);

  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: Off",
      ac.toString());
  ac.setMode(kLgAcHeat);
  ac.setTemp(kLgAcMaxTemp);
  ac.on();
  ac.setFan(kLgAcFanHigh);
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 4 (Heat), Temp: 30C, Fan: 4 (High)",
      ac.toString());
  ac.setMode(kLgAcCool);
  ac.setFan(kLgAcFanLow);
  ac.setTemp(kLgAcMinTemp);
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 0 (Cool), Temp: 16C, Fan: 1 (Low)",
      ac.toString());
  ac.setTemp(ac.getTemp() + 1);
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 0 (Cool), Temp: 17C, Fan: 1 (Low)",
      ac.toString());
  ac.setTemp(ac.getTemp() - 1);
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 0 (Cool), Temp: 16C, Fan: 1 (Low)",
      ac.toString());
  ac.setPower(false);
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: Off",
      ac.toString());
}

TEST(TestIRLgAcClass, SetAndGetRaw) {
  IRLgAc ac(kGpioUnused);

  ac.setRaw(0x8800A4E);
  ASSERT_EQ(0x8800A4E, ac.getRaw());
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 0 (Cool), Temp: 25C, Fan: 4 (High)",
      ac.toString());

  ac.setRaw(0x88C0051);
  ASSERT_EQ(0x88C0051, ac.getRaw());
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: Off",
      ac.toString());
}

TEST(TestIRLgAcClass, MessageConstruction) {
  IRLgAc ac(kGpioUnused);

  ac.on();
  ac.setMode(kLgAcCool);
  ac.setTemp(25);
  ac.setFan(kLgAcFanHigh);
  ASSERT_EQ(0x8800A4E, ac.getRaw());
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 0 (Cool), Temp: 25C, Fan: 4 (High)",
      ac.toString());
}

TEST(TestIRLgAcClass, isValidLgAc) {
  IRLgAc ac(kGpioUnused);

  ac.setRaw(0x8800A4E);
  ASSERT_TRUE(ac.isValidLgAc());

  // Make the checksum wrong.
  ac.setRaw(0x8800A4F);
  ASSERT_FALSE(ac.isValidLgAc());

  ac.setRaw(0x88C0051);
  ASSERT_TRUE(ac.isValidLgAc());

  // Use a wrong signature.
  ac.setRaw(0x8000A4E);
  ASSERT_FALSE(ac.isValidLgAc());
}

TEST(TestIRLgAcClass, calcChecksum) {
  EXPECT_EQ(0x1, IRLgAc::calcChecksum(0x88C0051));
  EXPECT_EQ(0x4, IRLgAc::calcChecksum(0x88C0354));
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("LG", typeToString(decode_type_t::LG));
  ASSERT_EQ(decode_type_t::LG, strToDecodeType("LG"));
  ASSERT_FALSE(hasACState(decode_type_t::LG));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::LG));
  ASSERT_EQ(kLgBits, IRsendTest::defaultBits(decode_type_t::LG));
  ASSERT_EQ(kLgDefaultRepeat, IRsendTest::minRepeats(decode_type_t::LG));

  ASSERT_EQ("LG2", typeToString(decode_type_t::LG2));
  ASSERT_EQ(decode_type_t::LG2, strToDecodeType("LG2"));
  ASSERT_FALSE(hasACState(decode_type_t::LG2));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::LG2));
  ASSERT_EQ(kLgBits, IRsendTest::defaultBits(decode_type_t::LG2));
  ASSERT_EQ(kLgDefaultRepeat, IRsendTest::minRepeats(decode_type_t::LG2));
}

TEST(TestIRLgAcClass, KnownExamples) {
  IRLgAc ac(kGpioUnused);
  // Ref:
  // https://github.com/crankyoldgit/IRremoteESP8266/issues/1008#issuecomment-570646648

  // Temp
  ac.setRaw(0x880C152);
  ASSERT_TRUE(ac.isValidLgAc());
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 4 (Heat), Temp: 16C, Fan: 5 (Auto)",
      ac.toString());

  ac.setRaw(0x880CF50);
  ASSERT_TRUE(ac.isValidLgAc());
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 4 (Heat), Temp: 30C, Fan: 5 (Auto)",
      ac.toString());

  // Modes
  ac.setRaw(0x880960F);
  ASSERT_TRUE(ac.isValidLgAc());
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 1 (Dry), Temp: 21C, Fan: 0 (Quiet)",
      ac.toString());

  ac.setRaw(0x880C758);
  ASSERT_TRUE(ac.isValidLgAc());
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 4 (Heat), Temp: 22C, Fan: 5 (Auto)",
      ac.toString());

  ac.setRaw(0x8808855);
  ASSERT_TRUE(ac.isValidLgAc());
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 0 (Cool), Temp: 23C, Fan: 5 (Auto)",
      ac.toString());

  // Fan speeds
  ac.setRaw(0x880870F);
  ASSERT_TRUE(ac.isValidLgAc());
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 0 (Cool), Temp: 22C, Fan: 0 (Quiet)",
      ac.toString());

  ac.setRaw(0x8808721);
  ASSERT_TRUE(ac.isValidLgAc());
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 0 (Cool), Temp: 22C, Fan: 2 (Medium)",
      ac.toString());

  ac.setRaw(0x8808743);
  ASSERT_TRUE(ac.isValidLgAc());
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 0 (Cool), Temp: 22C, Fan: 4 (High)",
      ac.toString());

  ac.setRaw(0x8808754);
  ASSERT_TRUE(ac.isValidLgAc());
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 0 (Cool), Temp: 22C, Fan: 5 (Auto)",
      ac.toString());

  ac.setRaw(0x880A745);
  ASSERT_TRUE(ac.isValidLgAc());
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 2 (Fan), Temp: 22C, Fan: 4 (High)",
      ac.toString());

  // https://github.com/crankyoldgit/IRremoteESP8266/issues/1008#issuecomment-570794029
  ac.setRaw(0x8800347);
  ASSERT_TRUE(ac.isValidLgAc());
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 0 (Cool), Temp: 18C, Fan: 4 (High)",
      ac.toString());
  ac.setRaw(0x8808440);
  ASSERT_TRUE(ac.isValidLgAc());
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 0 (Cool), Temp: 19C, Fan: 4 (High)",
      ac.toString());
  ac.setRaw(0x8800459);
  ASSERT_TRUE(ac.isValidLgAc());
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 0 (Cool), Temp: 19C, Fan: 5 (Auto)",
      ac.toString());
  ac.setRaw(0x8809946);
  ASSERT_TRUE(ac.isValidLgAc());
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 1 (Dry), Temp: 24C, Fan: 4 (High)",
      ac.toString());
  ac.setRaw(0x880A341);
  ASSERT_TRUE(ac.isValidLgAc());
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 2 (Fan), Temp: 18C, Fan: 4 (High)",
      ac.toString());
  ac.setRaw(0x8810045);
  ASSERT_TRUE(ac.isValidLgAc());
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 0 (Cool), Temp: 15C, Fan: 4 (High)",
      ac.toString());
  ac.setRaw(0x8810056);
  ASSERT_TRUE(ac.isValidLgAc());
  EXPECT_EQ(
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 0 (Cool), Temp: 15C, Fan: 5 (Auto)",
      ac.toString());
}

// Verify decoding of LG2 message.
TEST(TestDecodeLG2, Issue1008) {
  IRsendTest irsend(kGpioUnused);
  IRrecv capture(kGpioUnused);
  irsend.begin();

  irsend.reset();
  // From https://github.com/crankyoldgit/IRremoteESP8266/issues/1008#issuecomment-570794029
  // First entry.
  uint16_t rawData[59] = {
      3272, 9844, 506, 1588, 536, 498, 534, 498, 536, 498, 534, 1540, 534, 506,
      534, 498, 534, 500, 532, 500, 534, 498, 534, 498, 534, 506, 534, 500, 534,
      498, 534, 498, 534, 498, 534, 500, 534, 498, 534, 1566, 508, 1566, 508,
      500, 534, 1540, 534, 506, 534, 500, 534, 500, 534, 1560, 508, 1540, 534,
      1558, 508};  // UNKNOWN AFC3034C
  irsend.sendRaw(rawData, 59, 38000);
  irsend.makeDecodeResult();

  ASSERT_TRUE(capture.decode(&irsend.capture));
  ASSERT_EQ(LG2, irsend.capture.decode_type);
  EXPECT_EQ(kLgBits, irsend.capture.bits);
  EXPECT_EQ(0x8800347, irsend.capture.value);

  irsend.reset();
  IRLgAc ac(kGpioUnused);
  ac.setRaw(0x8800347);
  ac.setModel(lg_ac_remote_model_t::AKB75215403);  // aka. 2
  ac.send();

  char expected[] =
      "Model: 2 (AKB75215403), "
      "Power: On, Mode: 0 (Cool), Temp: 18C, Fan: 4 (High)";
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(LG2, ac._irsend.capture.decode_type);
  ASSERT_EQ(kLgBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRLgAcClass, DifferentModels) {
  IRLgAc ac(kGpioUnused);
  IRrecv capture(kGpioUnused);

  ac.setRaw(0x8800347);

  ac.setModel(lg_ac_remote_model_t::GE6711AR2853M);  // aka. 1
  ac._irsend.reset();
  ac.send();

  char expected1[] =
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 0 (Cool), Temp: 18C, Fan: 4 (High)";
  ASSERT_EQ(expected1, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(LG, ac._irsend.capture.decode_type);  // Not "LG2"
  ASSERT_EQ(kLgBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected1, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));

  ac.setModel(lg_ac_remote_model_t::AKB75215403);  // aka. 2
  ac._irsend.reset();
  ac.send();

  char expected2[] =
      "Model: 2 (AKB75215403), "
      "Power: On, Mode: 0 (Cool), Temp: 18C, Fan: 4 (High)";
  ASSERT_EQ(expected2, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(LG2, ac._irsend.capture.decode_type);  // Not "LG"
  ASSERT_EQ(kLgBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected2, IRAcUtils::resultAcToString(&ac._irsend.capture));
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRLgAcClass, FanSpeedIssue1214) {
  EXPECT_EQ(kLgAcFanLowest, IRLgAc::convertFan(stdAc::fanspeed_t::kMin));
  EXPECT_EQ(kLgAcFanLow, IRLgAc::convertFan(stdAc::fanspeed_t::kLow));
  EXPECT_EQ(kLgAcFanMedium, IRLgAc::convertFan(stdAc::fanspeed_t::kMedium));
  EXPECT_EQ(kLgAcFanHigh, IRLgAc::convertFan(stdAc::fanspeed_t::kHigh));
  EXPECT_EQ(kLgAcFanHigh, IRLgAc::convertFan(stdAc::fanspeed_t::kMax));
  EXPECT_EQ(kLgAcFanAuto, IRLgAc::convertFan(stdAc::fanspeed_t::kAuto));
}
