// Copyright 2022 crankyoldgit

#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for decodeClimaButler().
TEST(TestDecodeClimaButler, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  const uint16_t rawData[109] = {
      554, 3512,
      558, 1488, 580, 496, 522, 494, 546, 558, 552, 494, 548, 496, 546, 500,
      548, 558, 544, 504, 544, 502, 540, 506, 538, 570, 514, 530, 514, 532, 512,
      534, 512, 592, 512, 536, 510, 532, 510, 536, 510, 598, 510, 536, 506, 536,
      514, 534, 510, 594, 514, 534, 510, 536, 534, 510, 510, 598, 514, 534, 510,
      536, 508, 536, 534, 572, 534, 500, 512, 1536, 512, 526, 510, 1588, 512,
      1536, 510, 538, 512, 532, 510, 588, 510, 1536, 514, 532, 510, 536, 534,
      570, 512, 534, 510, 536, 514, 528, 540, 568, 540, 506, 512, 524, 510,
      1534, 516, 532,
      536, 3396,
      544};  // UNKNOWN E6CA5369 POWER OFF
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 109, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::CLIMABUTLER, irsend.capture.decode_type);
  ASSERT_EQ(kClimaButlerBits, irsend.capture.bits);
  EXPECT_EQ(0x8000000058802, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_EQ(
      "",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_FALSE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

TEST(TestDecodeClimaButler, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  irsend.reset();
  irsend.sendClimaButler(0x8000000058802);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::CLIMABUTLER, irsend.capture.decode_type);
  ASSERT_EQ(kClimaButlerBits, irsend.capture.bits);
  EXPECT_EQ(0x8000000058802, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("CLIMABUTLER", typeToString(decode_type_t::CLIMABUTLER));
  ASSERT_EQ(decode_type_t::CLIMABUTLER, strToDecodeType("CLIMABUTLER"));
  ASSERT_FALSE(hasACState(decode_type_t::CLIMABUTLER));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::CLIMABUTLER));
  ASSERT_EQ(kClimaButlerBits, IRsend::defaultBits(decode_type_t::CLIMABUTLER));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::CLIMABUTLER));
}
