// Copyright 2020 David Conran

#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("MIRAGE", typeToString(decode_type_t::MIRAGE));
  ASSERT_EQ(decode_type_t::MIRAGE, strToDecodeType("MIRAGE"));
  ASSERT_TRUE(hasACState(decode_type_t::MIRAGE));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::MIRAGE));
  ASSERT_EQ(kMirageBits, IRsend::defaultBits(decode_type_t::MIRAGE));
  ASSERT_EQ(kMirageMinRepeat, IRsend::minRepeats(decode_type_t::MIRAGE));
}

// Tests for decodeMirage().
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1289
// Data from:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/1289#issuecomment-705826015
//   But it is corrected to adjust for poor capture.
TEST(TestDecodeMirage, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  const uint16_t rawData[243] = {
      8360, 4248, 582, 518, 556, 1582, 586, 1572, 528, 572, 556, 1590, 526, 572,
      554, 1586, 528, 578, 558, 1582, 556, 542, 558, 1598, 528, 572, 556, 1590,
      528, 1610, 556, 1600, 554, 546, 556, 544, 558, 542, 558, 542, 676, 400,
      606, 492, 582, 542, 556, 544, 556, 542, 558, 544, 556, 542, 556, 544, 558,
      542, 556, 544, 530, 570, 586, 516, 584, 514, 558, 542, 558, 542, 558, 542,
      554, 546, 558, 542, 558, 1582, 534, 542, 580, 552, 528, 1610, 556, 544,
      554, 546, 554, 544, 556, 544, 556, 544, 558, 542, 558, 552, 558, 542, 558,
      542, 558, 542, 556, 544, 558, 542, 558, 542, 554, 544, 584, 516, 558, 542,
      528, 572, 588, 512, 556, 544, 532, 568, 560, 542, 558, 542, 560, 540, 560,
      538, 530, 570, 558, 542, 558, 542, 560, 540, 558, 542, 558, 542, 530, 568,
      558, 542, 558, 542, 532, 570, 530, 570, 558, 542, 558, 542, 558, 542, 530,
      570, 530, 568, 560, 540, 560, 540, 532, 568, 558, 542, 558, 542, 532, 568,
      560, 542, 532, 568, 532, 568, 530, 570, 532, 570, 530, 570, 558, 540, 560,
      540, 558, 534, 558, 542, 556, 1600, 558, 1592, 558, 542, 560, 1590, 530,
      570, 530, 570, 556, 544, 560, 540, 556, 544, 558, 1582, 556, 544, 558,
      1600, 556, 542, 560, 542, 532, 568, 558, 542, 610, 1538, 504, 1646, 582,
      518, 528, 572, 528, 1612, 556, 544, 528, 580, 554};  // UNKNOWN 28DACDC4
  const uint8_t expected[kMirageStateLength] = {
      0x56, 0x75, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x16, 0x14, 0x26};
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 243, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::MIRAGE, irsend.capture.decode_type);
  ASSERT_EQ(kMirageBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "",
      IRAcUtils::resultAcToString(&irsend.capture));
}

TEST(TestDecodeMirage, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  const uint8_t expected[kMirageStateLength] = {
      0x56, 0x75, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x16, 0x14, 0x26};
  irsend.begin();
  irsend.reset();
  irsend.sendMirage(expected);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::MIRAGE, irsend.capture.decode_type);
  ASSERT_EQ(kMirageBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "",
      IRAcUtils::resultAcToString(&irsend.capture));
}

// Data from:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/1289#issuecomment-705624234
TEST(TestDecodeMirage, RealExampleWithDodgyHardwareCapture) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  const uint16_t rawData[243] = {
      8360, 4248, 582, 518, 556, 1582, 586, 1572, 528, 572, 556, 1590, 526, 572,
      554, 1586, 528, 578, 558, 1582, 556, 542, 558, 1598, 528, 572, 556, 1590,
      528, 1610, 556, 1600, 554, 546, 556, 544, 558, 542, 558, 542, 676, 400,
      606, 492, 582, 542, 556, 544, 556, 542, 558, 544, 556, 542, 556, 544, 558,
      542, 556, 544, 530, 570, 586, 516, 584, 514, 558, 542, 558, 542, 558, 542,
      554, 546, 558, 542, 558, 1582,
      734, 342,  // Really poor data here.
      580, 552, 528, 1610, 556, 544, 554, 546, 554, 544, 556, 544, 556, 544,
      558, 542, 558, 552, 558, 542, 558, 542, 558, 542, 556, 544, 558, 542, 558,
      542, 554, 544, 584, 516, 558, 542, 528, 572, 588, 512, 556, 544, 532, 568,
      560, 542, 558, 542, 560, 540, 560, 538, 530, 570, 558, 542, 558, 542, 560,
      540, 558, 542, 558, 542, 530, 568, 558, 542, 558, 542, 532, 570, 530, 570,
      558, 542, 558, 542, 558, 542, 530, 570, 530, 568, 560, 540, 560, 540, 532,
      568, 558, 542, 558, 542, 532, 568, 560, 542, 532, 568, 532, 568, 530, 570,
      532, 570, 530, 570, 558, 540, 560, 540, 558, 534, 558, 542, 556, 1600,
      558, 1592, 558, 542, 560, 1590, 530, 570, 530, 570, 556, 544, 560, 540,
      556, 544, 558, 1582, 556, 544, 558, 1600, 556, 542, 560, 542, 532, 568,
      558, 542, 610, 1538, 504, 1646, 582, 518, 528, 572, 528, 1612, 556, 544,
      528, 580, 554};  // UNKNOWN 28DACDC4
  const uint8_t expected[kMirageStateLength] = {
      0x56, 0x75, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x16, 0x14, 0x26};
  irrecv.setTolerance(kTolerance + 10);  // Bump tolerance to match poor data.
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 243, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::MIRAGE, irsend.capture.decode_type);
  ASSERT_EQ(kMirageBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expected, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "",
      IRAcUtils::resultAcToString(&irsend.capture));
}
