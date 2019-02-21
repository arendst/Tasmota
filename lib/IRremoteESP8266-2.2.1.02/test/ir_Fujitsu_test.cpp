// Copyright 2017 Jonny Graham

#include "IRsend.h"
#include "IRsend_test.h"
#include "ir_Fujitsu.h"
#include "gtest/gtest.h"

template<typename T, size_t size>
::testing::AssertionResult ArraysMatch(const T (&expected)[size],
                                       const T* actual) {
  for (size_t i(0); i < size; ++i) {
    if (expected[i] != actual[i]) {
      int e = expected[i];
      int a = actual[i];
      return ::testing::AssertionFailure() << "array[" << i
        << "] (" << std::hex << a << std::dec << ") != expected[" << i
        << "] (" << std::hex << e << std::dec << ")";
    }
  }

  return ::testing::AssertionSuccess();
}
// Tests for Mitsubishi A/C methods.

// Test sending typical data only.
TEST(TestSendFujitsuAC, GetRawDefault) {
  IRFujitsuAC fujitsuACSender = IRFujitsuAC(4);
  fujitsuACSender.setCmd(FUJITSU_AC_CMD_TURN_ON);
  fujitsuACSender.setSwing(FUJITSU_AC_SWING_BOTH);
  fujitsuACSender.setMode(FUJITSU_AC_MODE_COOL);
  fujitsuACSender.setFanSpeed(FUJITSU_AC_FAN_HIGH);
  fujitsuACSender.setTemp(24);
  uint8_t expected[16] = {0x14, 0x63, 0x0, 0x10, 0x10, 0xFE, 0x9, 0x30,
                          0x81, 0x1, 0x31, 0x0, 0x0, 0x0, 0x20, 0xFD};
  EXPECT_TRUE(ArraysMatch(expected, fujitsuACSender.getRaw()));
}

TEST(TestSendFujitsuAC, GetRawTurnOff) {
  IRFujitsuAC fujitsuACSender = IRFujitsuAC(4);
  fujitsuACSender.off();
  uint8_t expected[7] = {0x14, 0x63, 0x0, 0x10, 0x10, 0x02, 0xFD};
  EXPECT_TRUE(ArraysMatch(expected, fujitsuACSender.getRaw()));
}
TEST(TestSendFujitsuAC, GetRawStepHoriz) {
  IRFujitsuAC fujitsuACSender = IRFujitsuAC(4);
  fujitsuACSender.stepHoriz();
  uint8_t expected[7] = {0x14, 0x63, 0x0, 0x10, 0x10, 0x79, 0x86};
  EXPECT_TRUE(ArraysMatch(expected, fujitsuACSender.getRaw()));
}
TEST(TestSendFujitsuAC, GetRawStepVert) {
  IRFujitsuAC fujitsuACSender = IRFujitsuAC(4);
  fujitsuACSender.stepVert();
  uint8_t expected[7] = {0x14, 0x63, 0x0, 0x10, 0x10, 0x6C, 0x93};
  EXPECT_TRUE(ArraysMatch(expected, fujitsuACSender.getRaw()));
}

TEST(TestSendFujitsuAC, GetRawWithSwingHoriz) {
  IRFujitsuAC fujitsuACSender = IRFujitsuAC(4);
  fujitsuACSender.setCmd(FUJITSU_AC_CMD_STAY_ON);
  fujitsuACSender.setSwing(FUJITSU_AC_SWING_HORIZ);
  fujitsuACSender.setMode(FUJITSU_AC_MODE_COOL);
  fujitsuACSender.setFanSpeed(FUJITSU_AC_FAN_QUIET);
  fujitsuACSender.setTemp(25);
  uint8_t expected[16] = {0x14, 0x63, 0x0, 0x10, 0x10, 0xFE, 0x9, 0x30,
                          0x90, 0x1, 0x24, 0x0, 0x0, 0x0, 0x20, 0xFB};
  EXPECT_TRUE(ArraysMatch(expected, fujitsuACSender.getRaw()));
}

TEST(TestSendFujitsuAC, GetRawWithFan) {
  IRFujitsuAC fujitsuACSender = IRFujitsuAC(4);
  fujitsuACSender.setCmd(FUJITSU_AC_CMD_STAY_ON);
  fujitsuACSender.setSwing(FUJITSU_AC_SWING_HORIZ);
  fujitsuACSender.setMode(FUJITSU_AC_MODE_FAN);
  fujitsuACSender.setFanSpeed(FUJITSU_AC_FAN_MED);
  fujitsuACSender.setTemp(20);  // temp doesn't matter for fan
                                // but it is sent by the RC anyway
  uint8_t expected[16] = {0x14, 0x63, 0x0, 0x10, 0x10, 0xFE, 0x9, 0x30,
                          0x40, 0x3, 0x22, 0x0, 0x0, 0x0, 0x20, 0x4B};
  EXPECT_TRUE(ArraysMatch(expected, fujitsuACSender.getRaw()));
}

TEST(TestSendFujitsuAC, GenerateMessage) {
  IRFujitsuAC fujitsuACSender = IRFujitsuAC(4);
  IRsendTest irsend(4);
  fujitsuACSender.begin();
  irsend.begin();

  fujitsuACSender.setCmd(FUJITSU_AC_CMD_STAY_ON);
  fujitsuACSender.setSwing(FUJITSU_AC_SWING_BOTH);
  fujitsuACSender.setMode(FUJITSU_AC_MODE_COOL);
  fujitsuACSender.setFanSpeed(FUJITSU_AC_FAN_HIGH);
  fujitsuACSender.setTemp(24);

  EXPECT_EQ(FUJITSU_AC_FAN_HIGH, fujitsuACSender.getFanSpeed());
  EXPECT_EQ(FUJITSU_AC_MODE_COOL, fujitsuACSender.getMode());
  EXPECT_EQ(24, fujitsuACSender.getTemp());
  EXPECT_EQ(FUJITSU_AC_SWING_BOTH, fujitsuACSender.getSwing());
  EXPECT_EQ(FUJITSU_AC_CMD_STAY_ON, fujitsuACSender.getCmd());

  irsend.reset();
  irsend.sendFujitsuAC(fujitsuACSender.getRaw(), FUJITSU_AC_STATE_LENGTH);
  EXPECT_EQ(
  "m3224s1574m448s367m448s367m448s1182m448s367m448s1182m448s367m448s367m448"
  "s367m448s1182m448s1182m448s367m448s367m448s367m448s1182m448s1182m448s367"
  "m448s367m448s367m448s367m448s367m448s367m448s367m448s367m448s367m448"
  "s367m448s367m448s367m448s367m448s1182m448s367m448s367m448s367m448s367m448"
  "s367m448s367m448s367m448s1182m448s367m448s367m448s367m448s367m448s1182"
  "m448s1182m448s1182m448s1182m448s1182m448s1182m448s1182m448s1182m448s367"
  "m448s367m448s1182m448s367m448s367m448s367m448s367m448s367m448s367m448s367"
  "m448s367m448s1182m448s1182m448s367m448s367m448s367m448s367m448s367m448s367"
  "m448s367m448s367m448s367m448s1182m448s1182m448s367m448s367m448s367m448"
  "s367m448s367m448s367m448s367m448s1182m448s367m448s367m448s367m448s1182m448"
  "s1182m448s367m448s367m448s367m448s367m448s367m448s367m448s367m448s367m448"
  "s367m448s367m448s367m448s367m448s367m448s367m448s367m448s367m448s367m448"
  "s367m448s367m448s367m448s367m448s367m448s367m448s367m448s367m448s367m448"
  "s367m448s367m448s367m448s367m448s367m448s1182m448s367m448s367m448s367m448"
  "s1182m448s1182m448s1182m448s1182m448s1182m448s1182m448s1182m448s8100",
  irsend.outputStr());
}
TEST(TestSendFujitsuAC, GenerateShortMessage) {
  IRFujitsuAC fujitsuACSender = IRFujitsuAC(4);
  IRsendTest irsend(4);
  fujitsuACSender.begin();
  irsend.begin();

  fujitsuACSender.off();

  EXPECT_EQ(FUJITSU_AC_CMD_TURN_OFF, fujitsuACSender.getCmd());

  irsend.reset();
  irsend.sendFujitsuAC(fujitsuACSender.getRaw(), FUJITSU_AC_STATE_LENGTH_SHORT);
  EXPECT_EQ(
  "m3224s1574m448s367m448s367m448s1182m448s367m448s1182m448s367m448s367m448"
  "s367m448s1182m448s1182m448s367m448s367m448s367m448s1182m448s1182m448s367"
  "m448s367m448s367m448s367m448s367m448s367m448s367m448s367m448s367m448s367"
  "m448s367m448s367m448s367m448s1182m448s367m448s367m448s367m448s367m448s367"
  "m448s367m448s367m448s1182m448s367m448s367m448s367m448s367m448s1182m448s367"
  "m448s367m448s367m448s367m448s367m448s367m448s1182m448s367m448s1182m448"
  "s1182m448s1182m448s1182m448s1182m448s1182m448s8100",
  irsend.outputStr());
}

// Issue #275
TEST(TestSendFujitsuAC, Issue275) {
  IRFujitsuAC fujitsuACSender = IRFujitsuAC(4);
  IRsendTest irsend(4);
  fujitsuACSender.begin();
  irsend.begin();
  irsend.reset();

  fujitsuACSender.setCmd(FUJITSU_AC_CMD_TURN_OFF);
  irsend.sendFujitsuAC(fujitsuACSender.getRaw(), FUJITSU_AC_STATE_LENGTH_SHORT);
  EXPECT_EQ(
      // Header
      "m3224s1574"
      //  0       0       1        0       1        0       0       0     (0x28)
      "m448s367m448s367m448s1182m448s367m448s1182m448s367m448s367m448s367"
      //  1        1        0       0       0       1        1        0   (0xC6)
      "m448s1182m448s1182m448s367m448s367m448s367m448s1182m448s1182m448s367"
      //  0       0       0       0       0       0       0       0       (0x00)
      "m448s367m448s367m448s367m448s367m448s367m448s367m448s367m448s367"
      //  0       0       0       0       1        0       0       0      (0x08)
      "m448s367m448s367m448s367m448s367m448s1182m448s367m448s367m448s367"
      //  0       0       0       0       1        0       0       0      (0x08)
      "m448s367m448s367m448s367m448s367m448s1182m448s367m448s367m448s367"
      //  0       1        0       0       0       0       0       0      (0x40)
      "m448s367m448s1182m448s367m448s367m448s367m448s367m448s367m448s367"
      //  1        0       1        1        1        1        1        1 (0xBF)
      "m448s1182m448s367m448s1182m448s1182m448s1182m448s1182m448s1182m448s1182"
      // Footer
      "m448s8100", irsend.outputStr());

  irsend.reset();
  // Per report in Issue #275
  uint16_t off[115] = {
      3350, 1650,
      450, 400, 450, 450, 450, 1250, 450, 400, 450, 1250, 450, 400, 450, 400,
      450, 400, 450, 1250, 450, 1250, 450, 400, 450, 400, 450, 400, 450, 1250,
      450, 1250, 450, 400, 450, 400, 450, 400, 450, 400, 450, 400, 450, 400,
      450, 400, 450, 400, 450, 400, 450, 400, 450, 400, 450, 400, 450, 400,
      450, 1250, 450, 400, 450, 400, 450, 400, 450, 400, 450, 400, 450, 400,
      450, 400, 450, 1250, 450, 400, 450, 400, 450, 400, 450, 400, 450, 1250,
      450, 400, 450, 400, 450, 400, 450, 400, 450, 400, 450, 400, 450, 1250,
      450, 400, 450, 1250, 450, 1250, 450, 1250, 450, 1250, 450, 1250,
      450, 1250, 450};
  irsend.sendRaw(off, 115, 38);
  EXPECT_EQ(
      // Header
      "m3350s1650"
      //  0       0       1        0       1        0       0       0     (0x28)
      "m450s400m450s450m450s1250m450s400m450s1250m450s400m450s400m450s400"
      //  1        1        0       0       0       1        1        0   (0xC6)
      "m450s1250m450s1250m450s400m450s400m450s400m450s1250m450s1250m450s400"
      //  0       0       0       0       0       0       0       0       (0x00)
      "m450s400m450s400m450s400m450s400m450s400m450s400m450s400m450s400"
      //  0       0       0       0       1        0       0       0      (0x08)
      "m450s400m450s400m450s400m450s400m450s1250m450s400m450s400m450s400"
      //  0       0       0       0       1        0       0       0      (0x08)
      "m450s400m450s400m450s400m450s400m450s1250m450s400m450s400m450s400"
      //  0       1        0       0       0       0       0       0      (0x40)
      "m450s400m450s1250m450s400m450s400m450s400m450s400m450s400m450s400"
      //  1        0       1        1        1        1        1        1 (0xBF)
      "m450s1250m450s400m450s1250m450s1250m450s1250m450s1250m450s1250m450s1250"
      // Footer
      "m450",
      irsend.outputStr());
}
