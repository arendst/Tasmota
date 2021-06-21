// Copyright 2021 David Conran

#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "IRutils.h"
#include "gtest/gtest.h"

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("MILESTAG2", typeToString(decode_type_t::MILESTAG2));
  ASSERT_EQ(decode_type_t::MILESTAG2, strToDecodeType("MILESTAG2"));
  ASSERT_FALSE(hasACState(decode_type_t::MILESTAG2));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::MILESTAG2));
  ASSERT_EQ(kMilesTag2ShotBits, IRsend::defaultBits(decode_type_t::MILESTAG2));
}

// Test sending typical data only.
TEST(TestSendMilestag2, SendDataOnly) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  // Shot packet
  irsend.reset();
  irsend.sendMilestag2(0x379);
  EXPECT_EQ(
      "f38000d25"
      "m2400s600"
      "m600s600m600s600m600s600m600s600m1200s600m1200s600m600s600m1200s600"
      "m1200s600m1200s600m1200s600m600s600m600s600m1200s32600",
      irsend.outputStr());

  irsend.reset();
  irsend.sendMilestag2(0x379, kMilesTag2ShotBits);
  EXPECT_EQ(
      "f38000d25"
      "m2400s600"
      "m600s600m600s600m600s600m600s600m1200s600m1200s600m600s600m1200s600"
      "m1200s600m1200s600m1200s600m600s600m600s600m1200s32600",
      irsend.outputStr());

  // Msg packet
  irsend.reset();
  irsend.sendMilestag2(0x8123E8, kMilesTag2MsgBits);
  EXPECT_EQ(
      "f38000d25"
      "m2400s600"
      "m1200s600m600s600m600s600m600s600m600s600m600s600m600s600m1200s600"
      "m600s600m600s600m1200s600m600s600m600s600m600s600m1200s600m1200s600"
      "m1200s600m1200s600m1200s600m600s600m1200s600m600s600m600s600m600s32600",
      irsend.outputStr());
}

TEST(TestDecodeMilestag2, SyntheticSelfDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);

  irsend.begin();
  // Shot packet
  irsend.reset();
  irsend.sendMilestag2(0x379);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MILESTAG2, irsend.capture.decode_type);
  EXPECT_EQ(kMilesTag2ShotBits, irsend.capture.bits);
  EXPECT_EQ(0x379, irsend.capture.value);
  EXPECT_EQ(0xD, irsend.capture.address);
  EXPECT_EQ(0x39, irsend.capture.command);

  // Msg packet
  irsend.reset();
  irsend.sendMilestag2(0x8123E8, kMilesTag2MsgBits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MILESTAG2, irsend.capture.decode_type);
  EXPECT_EQ(kMilesTag2MsgBits, irsend.capture.bits);
  EXPECT_EQ(0x8123E8, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x23, irsend.capture.command);
}

TEST(TestDecodeMilestag2, FailToDecodeBadData) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);

  irsend.begin();
  // Shot packet with the Shot packet bit incorrectly set.
  irsend.reset();
  irsend.sendMilestag2(0xFF79);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_NE(MILESTAG2, irsend.capture.decode_type);

  // Msg packet with the Msg packet bit cleared.
  irsend.reset();
  irsend.sendMilestag2(0x0123E8, kMilesTag2MsgBits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_NE(MILESTAG2, irsend.capture.decode_type);

  // Msg packet with a bad Msg terminator.
  irsend.reset();
  irsend.sendMilestag2(0x8123E9, kMilesTag2MsgBits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_NE(MILESTAG2, irsend.capture.decode_type);
}

TEST(TestDecodeMilestag2, RealSelfDecodeExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/pull/1380#issuecomment-761159985
  const uint16_t rawData[29] = {
      2440, 602,
      608, 600, 606, 600, 606, 600, 602, 606, 1208, 602, 1216, 596, 604, 600,
      1214, 598, 1212, 600, 1208, 604, 1208, 602, 606, 600, 610, 596, 1210};

  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 29, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MILESTAG2, irsend.capture.decode_type);
  EXPECT_EQ(kMilesTag2ShotBits, irsend.capture.bits);
  EXPECT_EQ(0x379, irsend.capture.value);
  EXPECT_EQ(0xD, irsend.capture.address);
  EXPECT_EQ(0x39, irsend.capture.command);
}
