// Copyright 2020 David Conran

#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendEpson().

// Test sending typical data only.
TEST(TestSendEpson, SendDataOnly) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();
  irsend.sendEpson(0xC1AA09F6);
  EXPECT_EQ(
      "f38000d33"
      "m8960s4480"
      "m560s1680m560s1680m560s560m560s560m560s560m560s560m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s560m560s560m560s560m560s1680m560s560m560s560m560s1680"
      "m560s1680m560s1680m560s1680m560s1680m560s560m560s1680m560s1680m560s560"
      "m560s41440"
      "m8960s4480"
      "m560s1680m560s1680m560s560m560s560m560s560m560s560m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s560m560s560m560s560m560s1680m560s560m560s560m560s1680"
      "m560s1680m560s1680m560s1680m560s1680m560s560m560s1680m560s1680m560s560"
      "m560s41440"
      "m8960s4480"
      "m560s1680m560s1680m560s560m560s560m560s560m560s560m560s560m560s1680"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s560m560s560m560s560m560s1680m560s560m560s560m560s1680"
      "m560s1680m560s1680m560s1680m560s1680m560s560m560s1680m560s1680m560s560"
      "m560s41440",
      irsend.outputStr());
}

// Tests for decodeEpson().

// Decode normal Epson messages.
TEST(TestDecodeEpson, SyntheticSelfDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Real-life Epson code from an actual capture/decode.
  irsend.reset();
  irsend.sendEpson(0xC1AA09F6);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(EPSON, irsend.capture.decode_type);
  EXPECT_EQ(kEpsonBits, irsend.capture.bits);
  EXPECT_EQ(0xC1AA09F6, irsend.capture.value);
  EXPECT_EQ(0x5583, irsend.capture.address);
  EXPECT_EQ(0x90, irsend.capture.command);
  EXPECT_TRUE(irsend.capture.repeat);
}

// Decode a real Epson message.
TEST(TestDecodeEpson, RealMessageDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Real-life Epson code from an actual capture/decode.
  irsend.reset();
  const uint16_t rawData_0[203] = {
      8976, 4496, 542, 1690, 542, 1688, 542, 588, 518, 612, 542, 586, 518, 610,
      518, 610, 542, 1688, 516, 1714, 542, 614, 516, 1688, 542, 612, 516, 1690,
      540, 614, 514, 1690, 520, 638, 516, 584, 518, 612, 542, 586, 540, 588,
      542, 1688, 520, 610, 540, 586, 520, 1712, 540, 1692, 542, 1688, 542, 1690,
      520, 1710, 518, 610, 518, 1712, 518, 1716, 544, 584, 544, 40870,
      8978, 4496, 542, 1690, 542, 1688, 520, 610, 542, 584, 516, 638, 514, 588,
      540, 588, 540, 1690, 516, 1716, 516, 614, 542, 1688, 518, 612, 542, 1688,
      540, 614, 514, 1692, 542, 588, 538, 590, 542, 586, 542, 612, 516, 612,
      514, 1692, 516, 612, 540, 586, 520, 1712, 518, 1714, 542, 1688, 518, 1716,
      542, 1688, 516, 614, 542, 1686, 518, 1714, 516, 612, 532, 40882,
      8976, 4498, 516, 1716, 542, 1690, 540, 614, 516, 614, 516, 614, 514, 588,
      540, 588, 544, 1688, 522, 1710, 518, 638, 516, 1688, 520, 636, 492, 1714,
      542, 586, 542, 1690, 542, 588, 542, 584, 542, 612, 516, 612, 514, 614,
      516, 1690, 542, 586, 542, 586, 540, 1690, 518, 1712, 518, 1716, 518, 1714,
      542, 1690, 542, 586, 540, 1690, 542, 1690, 542, 588, 540};
  irsend.sendRaw(rawData_0, 203, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(EPSON, irsend.capture.decode_type);
  EXPECT_EQ(kEpsonBits, irsend.capture.bits);
  EXPECT_EQ(0xC1AA09F6, irsend.capture.value);
  EXPECT_EQ(0x5583, irsend.capture.address);
  EXPECT_EQ(0x90, irsend.capture.command);
  EXPECT_TRUE(irsend.capture.repeat);
}

TEST(TestDecodeEpson, RequiresARepeat) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Simulate sending only two messages (mesg + repeat)
  irsend.reset();
  irsend.sendEpson(0xC1AA09F6, kEpsonBits, 1);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(EPSON, irsend.capture.decode_type);
  EXPECT_EQ(kEpsonBits, irsend.capture.bits);
  EXPECT_EQ(0xC1AA09F6, irsend.capture.value);
  EXPECT_EQ(0x5583, irsend.capture.address);
  EXPECT_EQ(0x90, irsend.capture.command);
  EXPECT_TRUE(irsend.capture.repeat);

  // Simulate sending no repeats (just 1 message) which should fail to be
  // detected as Epson.
  irsend.reset();
  irsend.sendEpson(0xC1AA09F6, kEpsonBits, 0);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_NE(EPSON, irsend.capture.decode_type);
}

TEST(TestDecodeEpson, DecodeOnlyIfRepeatIsTheSame) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Simulate sending only two identical messages.
  // This should succeed.
  irsend.reset();
  irsend.sendEpson(0xC1AA09F6, kEpsonBits, 0);
  irsend.sendEpson(0xC1AA09F6, kEpsonBits, 0);

  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(EPSON, irsend.capture.decode_type);
  EXPECT_EQ(kEpsonBits, irsend.capture.bits);
  EXPECT_EQ(0xC1AA09F6, irsend.capture.value);
  EXPECT_EQ(0x5583, irsend.capture.address);
  EXPECT_EQ(0x90, irsend.capture.command);
  EXPECT_TRUE(irsend.capture.repeat);

  // Simulate sending a repeat but with a different code.
  // This should fail.
  irsend.reset();
  irsend.sendEpson(0xC1AA09F6, kEpsonBits, 0);  // Valid code.
  irsend.sendEpson(0x4BB640BF, kEpsonBits, 0);  // Valid code but different.
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_NE(EPSON, irsend.capture.decode_type);
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("EPSON", typeToString(decode_type_t::EPSON));
  ASSERT_EQ(decode_type_t::EPSON, strToDecodeType("EPSON"));
  ASSERT_FALSE(hasACState(decode_type_t::EPSON));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::EPSON));
  ASSERT_EQ(kEpsonBits, IRsendTest::defaultBits(decode_type_t::EPSON));
  ASSERT_EQ(kEpsonMinRepeat, IRsendTest::minRepeats(decode_type_t::EPSON));
}
