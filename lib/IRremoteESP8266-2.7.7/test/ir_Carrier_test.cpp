// Copyright 2018, 2020 David Conran

#include "IRac.h"
#include "IRrecv.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendCarrierAC()

// Test sending typical data only.
TEST(TestSendCarrierAC, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();

  irsend.reset();
  irsend.sendCarrierAC(0x0);
  EXPECT_EQ(
      "f38000d50"
      "m8532s4228"
      "m628s532m628s532m628s532m628s532m628s532m628s532m628s532m628s532"
      "m628s532m628s532m628s532m628s532m628s532m628s532m628s532m628s532"
      "m628s532m628s532m628s532m628s532m628s532m628s532m628s532m628s532"
      "m628s532m628s532m628s532m628s532m628s532m628s532m628s532m628s532"
      "m628s20000"
      "m8532s4228"
      "m628s1320m628s1320m628s1320m628s1320m628s1320m628s1320m628s1320m628s1320"
      "m628s1320m628s1320m628s1320m628s1320m628s1320m628s1320m628s1320m628s1320"
      "m628s1320m628s1320m628s1320m628s1320m628s1320m628s1320m628s1320m628s1320"
      "m628s1320m628s1320m628s1320m628s1320m628s1320m628s1320m628s1320m628s1320"
      "m628s20000"
      "m8532s4228"
      "m628s532m628s532m628s532m628s532m628s532m628s532m628s532m628s532"
      "m628s532m628s532m628s532m628s532m628s532m628s532m628s532m628s532"
      "m628s532m628s532m628s532m628s532m628s532m628s532m628s532m628s532"
      "m628s532m628s532m628s532m628s532m628s532m628s532m628s532m628s532"
      "m628s20000",
      irsend.outputStr());
  irsend.reset();
  irsend.sendCarrierAC(0x12345678);
  EXPECT_EQ(
      "f38000d50"
      "m8532s4228"
      "m628s532m628s532m628s532m628s1320m628s532m628s532m628s1320m628s532"
      "m628s532m628s532m628s1320m628s1320m628s532m628s1320m628s532m628s532"
      "m628s532m628s1320m628s532m628s1320m628s532m628s1320m628s1320m628s532"
      "m628s532m628s1320m628s1320m628s1320m628s1320m628s532m628s532m628s532"
      "m628s20000"
      "m8532s4228"
      "m628s1320m628s1320m628s1320m628s532m628s1320m628s1320m628s532m628s1320"
      "m628s1320m628s1320m628s532m628s532m628s1320m628s532m628s1320m628s1320"
      "m628s1320m628s532m628s1320m628s532m628s1320m628s532m628s532m628s1320"
      "m628s1320m628s532m628s532m628s532m628s532m628s1320m628s1320m628s1320"
      "m628s20000"
      "m8532s4228"
      "m628s532m628s532m628s532m628s1320m628s532m628s532m628s1320m628s532"
      "m628s532m628s532m628s1320m628s1320m628s532m628s1320m628s532m628s532"
      "m628s532m628s1320m628s532m628s1320m628s532m628s1320m628s1320m628s532"
      "m628s532m628s1320m628s1320m628s1320m628s1320m628s532m628s532m628s532"
      "m628s20000",
      irsend.outputStr());

  irsend.reset();
  irsend.sendCarrierAC(0x4CCA541D);
  EXPECT_EQ(
      "f38000d50"
      "m8532s4228"
      "m628s532m628s1320m628s532m628s532m628s1320m628s1320m628s532m628s532"
      "m628s1320m628s1320m628s532m628s532m628s1320m628s532m628s1320m628s532"
      "m628s532m628s1320m628s532m628s1320m628s532m628s1320m628s532m628s532"
      "m628s532m628s532m628s532m628s1320m628s1320m628s1320m628s532m628s1320"
      "m628s20000"
      "m8532s4228"
      "m628s1320m628s532m628s1320m628s1320m628s532m628s532m628s1320m628s1320"
      "m628s532m628s532m628s1320m628s1320m628s532m628s1320m628s532m628s1320"
      "m628s1320m628s532m628s1320m628s532m628s1320m628s532m628s1320m628s1320"
      "m628s1320m628s1320m628s1320m628s532m628s532m628s532m628s1320m628s532"
      "m628s20000"
      "m8532s4228"
      "m628s532m628s1320m628s532m628s532m628s1320m628s1320m628s532m628s532"
      "m628s1320m628s1320m628s532m628s532m628s1320m628s532m628s1320m628s532"
      "m628s532m628s1320m628s532m628s1320m628s532m628s1320m628s532m628s532"
      "m628s532m628s532m628s532m628s1320m628s1320m628s1320m628s532m628s1320"
      "m628s20000",
      irsend.outputStr());
}

// Test sending typical data only.
TEST(TestSendCarrierAC, SendWithRepeats) {
  IRsendTest irsend(0);
  irsend.begin();

  irsend.reset();
  irsend.sendCarrierAC(0x12345678, kCarrierAcBits, 2);  // two repeats.
  EXPECT_EQ(
      "f38000d50"
      "m8532s4228"
      "m628s532m628s532m628s532m628s1320m628s532m628s532m628s1320m628s532"
      "m628s532m628s532m628s1320m628s1320m628s532m628s1320m628s532m628s532"
      "m628s532m628s1320m628s532m628s1320m628s532m628s1320m628s1320m628s532"
      "m628s532m628s1320m628s1320m628s1320m628s1320m628s532m628s532m628s532"
      "m628s20000"
      "m8532s4228"
      "m628s1320m628s1320m628s1320m628s532m628s1320m628s1320m628s532m628s1320"
      "m628s1320m628s1320m628s532m628s532m628s1320m628s532m628s1320m628s1320"
      "m628s1320m628s532m628s1320m628s532m628s1320m628s532m628s532m628s1320"
      "m628s1320m628s532m628s532m628s532m628s532m628s1320m628s1320m628s1320"
      "m628s20000"
      "m8532s4228"
      "m628s532m628s532m628s532m628s1320m628s532m628s532m628s1320m628s532"
      "m628s532m628s532m628s1320m628s1320m628s532m628s1320m628s532m628s532"
      "m628s532m628s1320m628s532m628s1320m628s532m628s1320m628s1320m628s532"
      "m628s532m628s1320m628s1320m628s1320m628s1320m628s532m628s532m628s532"
      "m628s20000"
      "m8532s4228"
      "m628s532m628s532m628s532m628s1320m628s532m628s532m628s1320m628s532"
      "m628s532m628s532m628s1320m628s1320m628s532m628s1320m628s532m628s532"
      "m628s532m628s1320m628s532m628s1320m628s532m628s1320m628s1320m628s532"
      "m628s532m628s1320m628s1320m628s1320m628s1320m628s532m628s532m628s532"
      "m628s20000"
      "m8532s4228"
      "m628s1320m628s1320m628s1320m628s532m628s1320m628s1320m628s532m628s1320"
      "m628s1320m628s1320m628s532m628s532m628s1320m628s532m628s1320m628s1320"
      "m628s1320m628s532m628s1320m628s532m628s1320m628s532m628s532m628s1320"
      "m628s1320m628s532m628s532m628s532m628s532m628s1320m628s1320m628s1320"
      "m628s20000"
      "m8532s4228"
      "m628s532m628s532m628s532m628s1320m628s532m628s532m628s1320m628s532"
      "m628s532m628s532m628s1320m628s1320m628s532m628s1320m628s532m628s532"
      "m628s532m628s1320m628s532m628s1320m628s532m628s1320m628s1320m628s532"
      "m628s532m628s1320m628s1320m628s1320m628s1320m628s532m628s532m628s532"
      "m628s20000"
      "m8532s4228"
      "m628s532m628s532m628s532m628s1320m628s532m628s532m628s1320m628s532"
      "m628s532m628s532m628s1320m628s1320m628s532m628s1320m628s532m628s532"
      "m628s532m628s1320m628s532m628s1320m628s532m628s1320m628s1320m628s532"
      "m628s532m628s1320m628s1320m628s1320m628s1320m628s532m628s532m628s532"
      "m628s20000"
      "m8532s4228"
      "m628s1320m628s1320m628s1320m628s532m628s1320m628s1320m628s532m628s1320"
      "m628s1320m628s1320m628s532m628s532m628s1320m628s532m628s1320m628s1320"
      "m628s1320m628s532m628s1320m628s532m628s1320m628s532m628s532m628s1320"
      "m628s1320m628s532m628s532m628s532m628s532m628s1320m628s1320m628s1320"
      "m628s20000"
      "m8532s4228"
      "m628s532m628s532m628s532m628s1320m628s532m628s532m628s1320m628s532"
      "m628s532m628s532m628s1320m628s1320m628s532m628s1320m628s532m628s532"
      "m628s532m628s1320m628s532m628s1320m628s532m628s1320m628s1320m628s532"
      "m628s532m628s1320m628s1320m628s1320m628s1320m628s532m628s532m628s532"
      "m628s20000",
      irsend.outputStr());
}

// Tests for decodeCarrierAC().

// Decode normal "synthetic" messages.
TEST(TestDecodeCarrierAC, NormalDecodeWithStrict) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  irsend.sendCarrierAC(0x0);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeCarrierAC(&irsend.capture, kStartOffset,
                                     kCarrierAcBits, true));
  EXPECT_EQ(CARRIER_AC, irsend.capture.decode_type);
  EXPECT_EQ(kCarrierAcBits, irsend.capture.bits);
  EXPECT_EQ(0x0, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  irsend.reset();
  irsend.sendCarrierAC(0xB335ABE2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeCarrierAC(&irsend.capture, kStartOffset,
                                     kCarrierAcBits, true));
  EXPECT_EQ(CARRIER_AC, irsend.capture.decode_type);
  EXPECT_EQ(kCarrierAcBits, irsend.capture.bits);
  EXPECT_EQ(0xB335ABE2, irsend.capture.value);
  EXPECT_EQ(0xB335, irsend.capture.address);
  EXPECT_EQ(0xABE2, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Do the last one again, & use the full decoder, not just protocol specific.
  irsend.reset();
  irsend.sendCarrierAC(0xB335ABE2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(CARRIER_AC, irsend.capture.decode_type);
  EXPECT_EQ(kCarrierAcBits, irsend.capture.bits);
  EXPECT_EQ(0xB335ABE2, irsend.capture.value);
}

// Decode a "real" example message.
TEST(TestDecodeCarrierAC, RealExamples) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  // Data from Issue #385 captured by gnkarn
  uint16_t rawData[203] = {
      8532, 4216, 628, 1312,  628,  528,  628, 1312,  628,  1312, 628, 528,
      628,  524,  628, 1316,  624,  1316, 628, 524,   628,  528,  628, 1312,
      628,  1316, 624, 528,   628,  1312, 628, 528,   628,  1312, 628, 1312,
      628,  528,  628, 1316,  624,  528,  628, 1312,  628,  528,  628, 1312,
      628,  1316, 624, 1316,  628,  1312, 628, 1316,  628,  524,  628, 528,
      628,  528,  624, 1316,  628,  528,  628, 20064, 8504, 4228, 628, 528,
      628,  1312, 628, 528,   628,  528,  628, 1312,  628,  1316, 624, 532,
      624,  528,  628, 1316,  628,  1312, 628, 528,   628,  528,  628, 1312,
      628,  528,  628, 1316,  628,  528,  624, 528,   628,  1316, 628, 528,
      628,  1316, 624, 528,   628,  1316, 628, 528,   624,  532,  624, 528,
      628,  528,  628, 528,   628,  1316, 624, 1316,  628,  1316, 628, 528,
      624,  1316, 628, 20076, 8528, 4212, 624, 1316,  628,  528,  628, 1316,
      628,  1316, 624, 528,   628,  528,  628, 1316,  628,  1316, 628, 528,
      624,  532,  624, 1316,  628,  1316, 628, 528,   628,  1316, 624, 528,
      628,  1316, 628, 1316,  628,  528,  628, 1316,  624,  532,  624, 1316,
      628,  532,  624, 1316,  628,  1316, 624, 1320,  624,  1316, 628, 1316,
      628,  528,  628, 528,   628,  528,  628, 1316,  624,  532,  624};

  irsend.sendRaw(rawData, 203, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(CARRIER_AC, irsend.capture.decode_type);
  EXPECT_EQ(kCarrierAcBits, irsend.capture.bits);
  EXPECT_EQ(0xB335ABE2, irsend.capture.value);
  EXPECT_EQ(0xB335, irsend.capture.address);
  EXPECT_EQ(0xABE2, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

TEST(TestUtils, Housekeeping) {
  // CARRIER_AC
  ASSERT_EQ("CARRIER_AC", typeToString(decode_type_t::CARRIER_AC));
  ASSERT_EQ(decode_type_t::CARRIER_AC, strToDecodeType("CARRIER_AC"));
  ASSERT_FALSE(hasACState(decode_type_t::CARRIER_AC));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::CARRIER_AC));
  ASSERT_EQ(kCarrierAcBits,
            IRsend::defaultBits(decode_type_t::CARRIER_AC));
  ASSERT_EQ(kCarrierAcMinRepeat,
            IRsend::minRepeats(decode_type_t::CARRIER_AC));

  // CARRIER_AC40
  ASSERT_EQ("CARRIER_AC40", typeToString(decode_type_t::CARRIER_AC40));
  ASSERT_EQ(decode_type_t::CARRIER_AC40, strToDecodeType("CARRIER_AC40"));
  ASSERT_FALSE(hasACState(decode_type_t::CARRIER_AC40));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::CARRIER_AC40));
  ASSERT_EQ(kCarrierAc40Bits,
            IRsend::defaultBits(decode_type_t::CARRIER_AC40));
  ASSERT_EQ(kCarrierAc40MinRepeat,
            IRsend::minRepeats(decode_type_t::CARRIER_AC40));

  // CARRIER_AC64
  ASSERT_EQ("CARRIER_AC64", typeToString(decode_type_t::CARRIER_AC64));
  ASSERT_EQ(decode_type_t::CARRIER_AC64, strToDecodeType("CARRIER_AC64"));
  ASSERT_FALSE(hasACState(decode_type_t::CARRIER_AC64));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::CARRIER_AC64));
  ASSERT_EQ(kCarrierAc64Bits,
            IRsend::defaultBits(decode_type_t::CARRIER_AC64));
  ASSERT_EQ(kCarrierAc64MinRepeat,
            IRsend::minRepeats(decode_type_t::CARRIER_AC64));
}

/// Decode a "real" example message.
TEST(TestDecodeCarrierAC40, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  // Data from:
  //  https://github.com/crankyoldgit/IRremoteESP8266/issues/1112#issuecomment-627961192
  const uint16_t rawData[83] = {
      8402, 4166,
      562, 1550, 538, 1526, 562, 1552, 538, 1524, 566, 504, 538, 504, 540, 480,
      564, 506, 538, 506, 538, 506, 538, 1550, 538, 1550, 540, 506, 538, 506,
      538, 1550, 538, 506, 540, 478, 564, 480, 564, 506, 540, 1550, 538, 506,
      540, 506, 538, 1524, 564, 506, 538, 1550, 538, 1550, 538, 482, 562, 482,
      562, 506, 540, 504, 540, 482, 562, 506, 538, 1550, 538, 1550, 540, 1524,
      564, 1526, 564, 480, 564, 1528, 562, 504, 540, 480,
      564};  // UNKNOWN BCF4730D

  irsend.sendRaw(rawData, 83, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(CARRIER_AC40, irsend.capture.decode_type);
  EXPECT_EQ(kCarrierAc40Bits, irsend.capture.bits);
  EXPECT_EQ(0xF03212C0F4, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);
}

/// Send & Decode a synthetic message.
TEST(TestDecodeCarrierAC40, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();

  irsend.sendCarrierAC40(0xF03212C0F4);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(CARRIER_AC40, irsend.capture.decode_type);
  EXPECT_EQ(kCarrierAc40Bits, irsend.capture.bits);
  EXPECT_EQ(0xF03212C0F4, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);
  // Payload is sent a total of three times.
  EXPECT_EQ(
      "f38000d50"
      // Initial
      "m8402s4166"
      "m547s1540m547s1540m547s1540m547s1540m547s497m547s497m547s497m547s497"
      "m547s497m547s497m547s1540m547s1540m547s497m547s497m547s1540m547s497"
      "m547s497m547s497m547s497m547s1540m547s497m547s497m547s1540m547s497"
      "m547s1540m547s1540m547s497m547s497m547s497m547s497m547s497m547s497"
      "m547s1540m547s1540m547s1540m547s1540m547s497m547s1540m547s497m547s497"
      "m547s20000"
      // Repeat #1
      "m8402s4166"
      "m547s1540m547s1540m547s1540m547s1540m547s497m547s497m547s497m547s497"
      "m547s497m547s497m547s1540m547s1540m547s497m547s497m547s1540m547s497"
      "m547s497m547s497m547s497m547s1540m547s497m547s497m547s1540m547s497"
      "m547s1540m547s1540m547s497m547s497m547s497m547s497m547s497m547s497"
      "m547s1540m547s1540m547s1540m547s1540m547s497m547s1540m547s497m547s497"
      "m547s20000"
      // Repeat #2
      "m8402s4166"
      "m547s1540m547s1540m547s1540m547s1540m547s497m547s497m547s497m547s497"
      "m547s497m547s497m547s1540m547s1540m547s497m547s497m547s1540m547s497"
      "m547s497m547s497m547s497m547s1540m547s497m547s497m547s1540m547s497"
      "m547s1540m547s1540m547s497m547s497m547s497m547s497m547s497m547s497"
      "m547s1540m547s1540m547s1540m547s1540m547s497m547s1540m547s497m547s497"
      "m547s20000",
      irsend.outputStr());
}

/// Decode a "real" example message.
TEST(TestDecodeCarrierAC64, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  // Data from:
  //  https://github.com/crankyoldgit/IRremoteESP8266/issues/1127#issuecomment-629713855
  const uint16_t rawData[131] = {
      8940, 4556,
      504, 616, 504, 616, 502, 1736, 504, 616, 504, 616, 504, 616, 502, 616,
      502, 1736, 504, 1736, 502, 616, 504, 1736, 504, 616, 502, 1736, 504, 616,
      502, 1736, 502, 616, 504, 616, 504, 1736, 502, 1736, 504, 1736, 502, 1736,
      504, 616, 502, 1736, 502, 616, 504, 616, 504, 1736, 504, 1736, 504, 1736,
      504, 616, 504, 1736, 502, 616, 502, 616, 504, 616, 504, 616, 502, 616,
      502, 616, 504, 1736, 504, 616, 504, 616, 502, 616, 504, 616, 504, 616,
      502, 616, 502, 616, 504, 616, 504, 616, 504, 616, 502, 616, 504, 616,
      504, 616, 502, 616, 502, 616, 504, 616, 504, 616, 504, 1736, 504, 616,
      504, 616, 504, 616, 504, 616, 504, 616, 504, 616, 504, 616, 502, 1736,
      504, 586, 502};

  irsend.sendRaw(rawData, 131, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(CARRIER_AC64, irsend.capture.decode_type);
  EXPECT_EQ(kCarrierAc64Bits, irsend.capture.bits);
  EXPECT_EQ(0x404000102E5E5584, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);
}

/// Send & Decode a synthetic message.
TEST(TestDecodeCarrierAC64, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();

  irsend.sendCarrierAC64(0x404000102E5E5584);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(CARRIER_AC64, irsend.capture.decode_type);
  EXPECT_EQ(kCarrierAc64Bits, irsend.capture.bits);
  EXPECT_EQ(0x404000102E5E5584, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);
  EXPECT_EQ(
      "f38000d50m"
      // Header
      "8940s4556"
      // Data
      "m503s615m503s615m503s1736m503s615m503s615m503s615m503s615m503s1736"
      "m503s1736m503s615m503s1736m503s615m503s1736m503s615m503s1736m503s615"
      "m503s615m503s1736m503s1736m503s1736m503s1736m503s615m503s1736m503s615"
      "m503s615m503s1736m503s1736m503s1736m503s615m503s1736m503s615m503s615"
      "m503s615m503s615m503s615m503s615m503s1736m503s615m503s615m503s615"
      "m503s615m503s615m503s615m503s615m503s615m503s615m503s615m503s615"
      "m503s615m503s615m503s615m503s615m503s615m503s615m503s1736m503s615"
      "m503s615m503s615m503s615m503s615m503s615m503s615m503s1736m503s615"
      // Footer
      "m503s100000",
      irsend.outputStr());
}
