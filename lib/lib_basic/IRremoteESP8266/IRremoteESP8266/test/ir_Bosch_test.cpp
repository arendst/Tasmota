// Copyright 2022 David Conran

#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"


TEST(TestUtils, Housekeeping) {
  // Bosch144
  ASSERT_EQ("BOSCH144", typeToString(decode_type_t::BOSCH144));
  ASSERT_EQ(decode_type_t::BOSCH144, strToDecodeType("BOSCH144"));
  ASSERT_TRUE(hasACState(decode_type_t::BOSCH144));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::BOSCH144));
  ASSERT_EQ(kBosch144Bits, IRsend::defaultBits(decode_type_t::BOSCH144));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::BOSCH144));
}

// Tests for decodeBosch144().

// Decode normal Bosch144 messages.
TEST(TestDecodeBosch144, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);

  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1787#issuecomment-1099993189
  // Mode: Cool; Fan: 100% ; Temp: 16°C
  const uint16_t rawData[299] = {
      4380, 4400,
      528, 1646, 504, 570, 504, 1646, 504, 1646, 504, 572, 502, 570, 504, 1646,
      504, 570, 504, 572, 502, 1646, 504, 570, 502, 570, 502, 1648, 502, 1646,
      502, 570, 502, 1646, 504, 572, 502, 572, 502, 1644, 504, 1646, 504, 1646,
      504, 1646, 502, 1648, 500, 1646, 504, 1646, 504, 1646, 504, 572, 502, 570,
      504, 570, 504, 570, 504, 570, 504, 570, 506, 570, 502, 572, 502, 570, 502,
      572, 502, 572, 502, 572, 502, 572, 502, 572, 500, 1648, 502, 1644, 502,
      1646, 504, 1646, 502, 1646, 504, 1646, 504, 1644, 504, 1646,
      504, 5234,
      4360, 4422,
      504, 1646, 502, 596, 478, 1670, 478, 1646, 504, 570, 504, 572, 500, 1646,
      502, 572, 502, 572, 502, 1644, 506, 570, 502, 570, 504, 1644, 506, 1644,
      502, 574, 502, 1644, 504, 570, 504, 570, 504, 1644, 504, 1646, 504, 1644,
      506, 1644, 504, 1646, 504, 1646, 504, 1644, 504, 1646, 502, 570, 504, 570,
      504, 570, 504, 570, 502, 570, 504, 570, 502, 572, 502, 570, 504, 570, 504,
      570, 504, 570, 502, 572, 502, 570, 506, 570, 504, 1646, 502, 1646, 504,
      1646, 504, 1646, 504, 1646, 502, 1644, 504, 1644, 504, 1646,
      502, 5236,
      4360, 4424,
      504, 1646, 504, 1646, 502, 572, 504, 1644, 504, 570, 504, 1646, 504, 570,
      502, 1644, 504, 570, 504, 1644, 506, 1646, 502, 572, 502, 572, 502, 1646,
      504, 570, 504, 570, 504, 570, 502, 572, 504, 570, 504, 570, 504, 570, 502,
      572, 502, 570, 504, 570, 502, 570, 504, 572, 502, 572, 502, 1646, 504,
      570, 504, 570, 504, 570, 502, 574, 502, 572, 502, 572, 502, 572, 502, 572,
      502, 572, 502, 570, 504, 572, 502, 572, 502, 572, 502, 1646, 504, 572,
      502, 570, 502, 1646, 504, 572, 504, 570, 504, 1644,
      504};  // COOLIX B23F00
  const uint8_t expectedState[kBosch144StateLength] = {
      0xB2, 0x4D, 0x3F, 0xC0, 0x00, 0xFF,
      0xB2, 0x4D, 0x3F, 0xC0, 0x00, 0xFF,
      0xD5, 0x64, 0x00, 0x10, 0x00, 0x49};
  irsend.begin();
  irsend.reset();

  irsend.sendRaw(rawData, 299, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::BOSCH144, irsend.capture.decode_type);
  EXPECT_EQ(kBosch144Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Power: On, Mode: 0 (Cool), Fan: 5 (High), Temp: 16C, Quiet: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t result, prev;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &result, &prev));
}

TEST(TestDecodeBosch144, SyntheticSelfDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  const uint8_t expectedState[kBosch144StateLength] = {
      0xB2, 0x4D, 0x3F, 0xC0, 0x00, 0xFF,
      0xB2, 0x4D, 0x3F, 0xC0, 0x00, 0xFF,
      0xD5, 0x64, 0x00, 0x10, 0x00, 0x49};
  irsend.sendBosch144(expectedState);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::BOSCH144, irsend.capture.decode_type);
  EXPECT_EQ(kBosch144Bits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Power: On, Mode: 0 (Cool), Fan: 5 (High), Temp: 16C, Quiet: Off",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t result, prev;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &result, &prev));
}
