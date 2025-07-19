// Copyright 2021 David Conran

#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for decodeArris().

// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1595
// Data from:
//   https://github.com/crankyoldgit/IRremoteESP8266/files/7100289/raw_data.txt
TEST(TestDecodeArris, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);

  const uint16_t rawData_1[59] = {
      2584, 1896, 666, 306, 338, 300, 336, 304, 668, 610, 332, 306, 338, 300,
      334, 304, 332, 312, 332, 306, 340, 300, 334, 304, 330, 308, 338, 302, 334,
      304, 330, 308, 336, 308, 336, 302, 332, 306, 330, 310, 674, 606, 336, 302,
      332, 306, 338, 306, 668, 612, 668, 306, 338, 304, 332, 308, 336, 608,
      334};  // UNKNOWN EDF1C0D0

  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData_1, 59, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::ARRIS, irsend.capture.decode_type);
  EXPECT_EQ(kArrisBits, irsend.capture.bits);
  EXPECT_EQ(0x1000085E, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x85, irsend.capture.command);

  irsend.reset();
  const uint16_t rawData_2[115] = {
      2584, 1898, 664, 308, 338, 302, 332, 306, 668, 614, 330, 308, 336, 302,
      332, 306, 340, 304, 330, 310, 336, 304, 332, 306, 338, 300, 334, 304, 330,
      308, 336, 302, 332, 312, 334, 306, 330, 308, 336, 302, 670, 610, 332, 306,
      330, 310, 336, 308, 674, 606, 664, 312, 334, 306, 338, 302, 334, 612, 330,
      5930,
      2584, 1898, 664, 308, 336, 302, 332, 306, 666, 614, 338, 300, 336, 304,
      332, 310, 674, 610, 332, 334, 312, 328, 308, 332, 304, 336, 310, 330, 306,
      332, 302, 314, 330, 336, 308, 330, 306, 334, 640, 612, 330, 308, 336, 302,
      332, 312, 672, 608, 672, 608, 672, 304, 330, 614, 330
  };  // UNKNOWN E6A77D83
  irsend.sendRaw(rawData_2, 115, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::ARRIS, irsend.capture.decode_type);
  EXPECT_EQ(kArrisBits, irsend.capture.bits);
  EXPECT_EQ(0x1000085E, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x85, irsend.capture.command);

  const uint16_t rawData_3[51] = {
      2584, 1896, 666, 308, 338, 328, 306, 332, 640, 612, 332, 336, 310, 300,
      334, 304, 678, 606, 336, 330, 306, 334, 310, 300, 334, 304, 332, 308, 338,
      302, 334, 310, 672, 304, 332, 614, 668, 612, 330, 336, 638, 620, 670, 610,
      670, 304, 330, 310, 336, 610, 672};  // UNKNOWN 4CA048A1
  irsend.reset();
  irsend.sendRaw(rawData_3, 51, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::ARRIS, irsend.capture.decode_type);
  EXPECT_EQ(kArrisBits, irsend.capture.bits);
  EXPECT_EQ(0x1080695D, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x695, irsend.capture.command);
}

TEST(TestDecodeArris, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  irsend.reset();
  irsend.sendArris(0x1000085E);
  irsend.makeDecodeResult();

  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::ARRIS, irsend.capture.decode_type);
  EXPECT_EQ(kArrisBits, irsend.capture.bits);
  EXPECT_EQ(0x1000085E, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x85, irsend.capture.command);

  EXPECT_EQ(
      "f38000d50"
// const uint16_t rawData_1[59] = {
//     2584, 1896,
      "m2560s1920"
//     666, 306,
       "m640s320"
//     338, 300,
      "m320s320"
//     336, 304,
      "m320s320"
//     668, 610,
      "m640s640"
//     332, 306,
      "m320s320"
//     338, 300,
      "m320s320"
//     334, 304,
      "m320s320"
//     332, 312,
      "m320s320"
//     332, 306,
      "m320s320"
//     340, 300,
      "m320s320"
//     334, 304,
      "m320s320"
//     330, 308,
      "m320s320"
//     338, 302,
      "m320s320"
//     334, 304,
      "m320s320"
//     330, 308,
      "m320s320"
//     336, 308,
      "m320s320"
//     336, 302,
      "m320s320"
//     332, 306,
      "m320s320"
//     330, 310,
      "m320s320"
//     674, 606,
      "m640s640"
//     336, 302,
      "m320s320"
//     332, 306,
      "m320s320"
//     338, 306,
      "m320s320"
//     668, 612,
      "m640s640"
//     668, 306,
      "m640s320"
//     338, 304,
      "m320s320"
//     332, 308,
      "m320s320"
//     336, 608,
      "m320s640"
//     334};  // UNKNOWN EDF1C0D0
      "m320s77184", irsend.outputStr());

  irsend.reset();
  irsend.sendArris(0x1080695D);
  irsend.makeDecodeResult();

  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::ARRIS, irsend.capture.decode_type);
  EXPECT_EQ(kArrisBits, irsend.capture.bits);
  EXPECT_EQ(0x1080695D, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x695, irsend.capture.command);
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("ARRIS", typeToString(decode_type_t::ARRIS));
  ASSERT_EQ(decode_type_t::ARRIS, strToDecodeType("ARRIS"));
  ASSERT_FALSE(hasACState(decode_type_t::ARRIS));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::ARRIS));
  ASSERT_EQ(kArrisBits, IRsend::defaultBits(decode_type_t::ARRIS));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::ARRIS));
}

TEST(TestSendArris, ReleaseToggle) {
  EXPECT_EQ(0x10800F5D, IRsend::toggleArrisRelease(0x10000F55));
  EXPECT_EQ(0x10000F55, IRsend::toggleArrisRelease(0x10800F5D));
  EXPECT_EQ(
      0x10800F5D,
      IRsend::toggleArrisRelease(IRsend::toggleArrisRelease(0x10800F5D)));
}

TEST(TestSendArris, encodeArris) {
  EXPECT_EQ(0x10800F5D, IRsend::encodeArris(0xF5, true));
  EXPECT_EQ(0x10000F55, IRsend::encodeArris(0xF5, false));
  EXPECT_EQ(0x1080695D, IRsend::encodeArris(0x695, true));
}
