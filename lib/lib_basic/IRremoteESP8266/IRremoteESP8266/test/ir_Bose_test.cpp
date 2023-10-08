// Copyright 2021 parsnip42
// Copyright 2021 David Conran

#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"


TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("BOSE", typeToString(decode_type_t::BOSE));
  ASSERT_EQ(decode_type_t::BOSE, strToDecodeType("BOSE"));
  ASSERT_FALSE(hasACState(decode_type_t::BOSE));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::BOSE));
  ASSERT_EQ(kBoseBits, IRsend::defaultBits(decode_type_t::BOSE));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::BOSE));
}

// Tests for sendBose().

// Test sending typical data only.
TEST(TestSendBose, SendDataOnly) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();
  irsend.sendBose(0xCD32);
  EXPECT_EQ("f38000d50"
            "m1100s1350m555s500m555s1435m555s500m555s500m555s1435m555s1435"
            "m555s500m555s500m555s1435m555s500m555s1435m555s1435m555s500"
            "m555s500m555s1435m555s1435m555s100000",
            irsend.outputStr());
}

// Decode normal Bose messages.
TEST(TestDecodeBose, SyntheticSelfDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Synthesised 16-bit Bose message (TV Speaker Power On).
  irsend.reset();
  irsend.sendBose(0xCD32);
  irsend.makeDecodeResult();

  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(BOSE, irsend.capture.decode_type);
  EXPECT_EQ(kBoseBits, irsend.capture.bits);
  EXPECT_EQ(0xCD32, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

// Decode normal Bose messages.
TEST(TestDecodeBose, RealMessageDecode1) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);

  irsend.begin();

  // Real-life Bose code from an actual capture/decode (TV Speaker Power On).
  irsend.reset();

  const uint16_t rawData_0[35] = {
      942, 1558,
      442, 558, 442, 1502, 494, 534, 466, 560, 440, 1530, 468, 1532, 466,
      558, 440, 504, 496, 1558, 440, 534, 466, 1556, 442, 1558, 440, 558,
      440, 534, 466, 1556, 442, 1558, 440
  };

  irsend.sendRaw(rawData_0, 35, 38000);
  irsend.makeDecodeResult();

  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(BOSE, irsend.capture.decode_type);
  EXPECT_EQ(kBoseBits, irsend.capture.bits);
  EXPECT_EQ(0xCD32, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

// Decode normal Bose messages.
TEST(TestDecodeBose, RealMessageDecode2) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);

  irsend.begin();

  // Real-life Bose code from an actual capture/decode (TV Speaker Mute).
  irsend.reset();

  const uint16_t rawData_0[35] = {
      1024, 1504,
      496, 528, 472, 480, 520, 502, 496, 506, 494, 502, 496, 502, 498,
      502, 498, 1500, 498, 1502, 496, 1504, 496, 1502, 496, 1504, 494,
      1472, 524, 1504, 468, 1556, 442, 532, 468,
  };

  irsend.sendRaw(rawData_0, 35, 38000);
  irsend.makeDecodeResult();

  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(BOSE, irsend.capture.decode_type);
  EXPECT_EQ(kBoseBits, irsend.capture.bits);
  EXPECT_EQ(0x7F80, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}
