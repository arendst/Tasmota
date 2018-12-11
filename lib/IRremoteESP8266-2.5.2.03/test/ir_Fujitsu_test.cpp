// Copyright 2017 Jonny Graham, David Conran

#include "IRrecv_test.h"
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
// Tests for Fujitsu A/C methods.

// Test sending typical data only.
TEST(TestIRFujitsuACClass, GetRawDefault) {
  IRFujitsuAC fujitsu = IRFujitsuAC(4);  // AR-RAH2E
  fujitsu.setCmd(kFujitsuAcCmdTurnOn);
  fujitsu.setSwing(kFujitsuAcSwingBoth);
  fujitsu.setMode(kFujitsuAcModeCool);
  fujitsu.setFanSpeed(kFujitsuAcFanHigh);
  fujitsu.setTemp(24);
  uint8_t expected_arrah2e[16] = {
      0x14, 0x63, 0x00, 0x10, 0x10, 0xFE, 0x09, 0x30,
      0x81, 0x01, 0x31, 0x00, 0x00, 0x00, 0x20, 0xFD};
  EXPECT_TRUE(ArraysMatch(expected_arrah2e, fujitsu.getRaw()));
  EXPECT_EQ(kFujitsuAcStateLength, fujitsu.getStateLength());
  EXPECT_EQ("Power: On, Mode: 1 (COOL), Temp: 24C, Fan: 1 (HIGH), "
            "Swing: Vert + Horiz, Command: N/A", fujitsu.toString());

  uint8_t expected_ardb1[15] = {
      0x14, 0x63, 0x00, 0x10, 0x10, 0xFC, 0x08, 0x30,
      0x81, 0x01, 0x31, 0x00, 0x00, 0x00, 0x1D};
  fujitsu.setModel(ARDB1);
  EXPECT_TRUE(ArraysMatch(expected_ardb1, fujitsu.getRaw()));
  EXPECT_EQ(kFujitsuAcStateLength - 1, fujitsu.getStateLength());
  EXPECT_EQ("Power: On, Mode: 1 (COOL), Temp: 24C, Fan: 1 (HIGH), "
            "Swing: Vert + Horiz, Command: N/A", fujitsu.toString());
}

TEST(TestIRFujitsuACClass, GetRawTurnOff) {
  IRFujitsuAC fujitsu = IRFujitsuAC(4);
  fujitsu.setModel(ARRAH2E);
  fujitsu.off();
  uint8_t expected_arrah2e[7] = {0x14, 0x63, 0x0, 0x10, 0x10, 0x02, 0xFD};
  EXPECT_TRUE(ArraysMatch(expected_arrah2e, fujitsu.getRaw()));
  EXPECT_EQ(kFujitsuAcStateLengthShort, fujitsu.getStateLength());
  EXPECT_EQ("Power: Off, Mode: 1 (COOL), Temp: 24C, Fan: 1 (HIGH), "
            "Swing: Vert + Horiz, Command: N/A", fujitsu.toString());

  fujitsu.setModel(ARDB1);
  uint8_t expected_ardb1[6] = {0x14, 0x63, 0x0, 0x10, 0x10, 0x02};
  EXPECT_TRUE(ArraysMatch(expected_ardb1, fujitsu.getRaw()));
  EXPECT_EQ(kFujitsuAcStateLengthShort - 1, fujitsu.getStateLength());
  EXPECT_EQ("Power: Off, Mode: 1 (COOL), Temp: 24C, Fan: 1 (HIGH), "
            "Swing: Vert + Horiz, Command: N/A", fujitsu.toString());
}

TEST(TestIRFujitsuACClass, GetRawStepHoriz) {
  IRFujitsuAC fujitsu = IRFujitsuAC(4);
  fujitsu.stepHoriz();
  uint8_t expected[7] = {0x14, 0x63, 0x0, 0x10, 0x10, 0x79, 0x86};
  EXPECT_TRUE(ArraysMatch(expected, fujitsu.getRaw()));
  EXPECT_EQ(kFujitsuAcStateLengthShort, fujitsu.getStateLength());
  EXPECT_EQ("Power: On, Mode: 1 (COOL), Temp: 24C, Fan: 1 (HIGH), "
            "Swing: Vert + Horiz, Command: Step vane horizontally",
            fujitsu.toString());
}

TEST(TestIRFujitsuACClass, GetRawStepVert) {
  IRFujitsuAC fujitsu = IRFujitsuAC(4);
  fujitsu.setModel(ARRAH2E);
  fujitsu.stepVert();
  uint8_t expected_arrah2e[7] = {0x14, 0x63, 0x0, 0x10, 0x10, 0x6C, 0x93};
  EXPECT_TRUE(ArraysMatch(expected_arrah2e, fujitsu.getRaw()));
  EXPECT_EQ(kFujitsuAcStateLengthShort, fujitsu.getStateLength());
  EXPECT_EQ("Power: On, Mode: 1 (COOL), Temp: 24C, Fan: 1 (HIGH), "
            "Swing: Vert + Horiz, Command: Step vane vertically",
            fujitsu.toString());

  fujitsu.setModel(ARDB1);
  fujitsu.stepVert();
  uint8_t expected_ardb1[6] = {0x14, 0x63, 0x0, 0x10, 0x10, 0x6C};
  EXPECT_TRUE(ArraysMatch(expected_ardb1, fujitsu.getRaw()));
  EXPECT_EQ(kFujitsuAcStateLengthShort - 1,
            fujitsu.getStateLength());
  EXPECT_EQ("Power: On, Mode: 1 (COOL), Temp: 24C, Fan: 1 (HIGH), "
            "Swing: Vert + Horiz, Command: Step vane vertically",
            fujitsu.toString());
}

TEST(TestIRFujitsuACClass, GetRawWithSwingHoriz) {
  IRFujitsuAC fujitsu = IRFujitsuAC(4);
  fujitsu.setCmd(kFujitsuAcCmdStayOn);
  fujitsu.setSwing(kFujitsuAcSwingHoriz);
  fujitsu.setMode(kFujitsuAcModeCool);
  fujitsu.setFanSpeed(kFujitsuAcFanQuiet);
  fujitsu.setTemp(25);
  uint8_t expected[16] = {0x14, 0x63, 0x0, 0x10, 0x10, 0xFE, 0x9, 0x30,
                          0x90, 0x1, 0x24, 0x0, 0x0, 0x0, 0x20, 0xFB};
  EXPECT_TRUE(ArraysMatch(expected, fujitsu.getRaw()));
  EXPECT_EQ("Power: On, Mode: 1 (COOL), Temp: 25C, Fan: 4 (QUIET), "
            "Swing: Horiz, Command: N/A",
            fujitsu.toString());
}

TEST(TestIRFujitsuACClass, GetRawWithFan) {
  IRFujitsuAC fujitsu = IRFujitsuAC(4);
  fujitsu.setCmd(kFujitsuAcCmdStayOn);
  fujitsu.setSwing(kFujitsuAcSwingHoriz);
  fujitsu.setMode(kFujitsuAcModeFan);
  fujitsu.setFanSpeed(kFujitsuAcFanMed);
  fujitsu.setTemp(20);  // temp doesn't matter for fan
                        // but it is sent by the RC anyway
  fujitsu.setModel(ARRAH2E);
  uint8_t expected_arrah2e[16] = {
      0x14, 0x63, 0x0, 0x10, 0x10, 0xFE, 0x9, 0x30,
      0x40, 0x3, 0x22, 0x0, 0x0, 0x0, 0x20, 0x4B};
  EXPECT_TRUE(ArraysMatch(expected_arrah2e, fujitsu.getRaw()));
  EXPECT_EQ(kFujitsuAcStateLength, fujitsu.getStateLength());
  EXPECT_EQ("Power: On, Mode: 3 (FAN), Temp: 20C, Fan: 2 (MED), Swing: Horiz, "
            "Command: N/A", fujitsu.toString());

  fujitsu.setModel(ARDB1);
  uint8_t expected_ardb1[15] = {
      0x14, 0x63, 0x0, 0x10, 0x10, 0xFC, 0x8, 0x30,
      0x40, 0x3, 0x22, 0x0, 0x0, 0x0, 0x6B};
  EXPECT_TRUE(ArraysMatch(expected_ardb1, fujitsu.getRaw()));
  EXPECT_EQ(kFujitsuAcStateLength - 1, fujitsu.getStateLength());
  EXPECT_EQ("Power: On, Mode: 3 (FAN), Temp: 20C, Fan: 2 (MED), Swing: Horiz, "
            "Command: N/A", fujitsu.toString());
}

TEST(TestIRFujitsuACClass, SetRaw) {
  IRFujitsuAC fujitsu = IRFujitsuAC(0);
  EXPECT_EQ(kFujitsuAcStateLength, fujitsu.getStateLength());
  uint8_t expected_default_arrah2e[kFujitsuAcStateLength] = {
      0x14, 0x63, 0x00, 0x10, 0x10, 0xFE, 0x09, 0x30,
      0x81, 0x01, 0x31, 0x00, 0x00, 0x00, 0x20, 0xFD};
  EXPECT_TRUE(ArraysMatch(expected_default_arrah2e, fujitsu.getRaw()));
  EXPECT_EQ("Power: On, Mode: 1 (COOL), Temp: 24C, Fan: 1 (HIGH), "
            "Swing: Vert + Horiz, Command: N/A", fujitsu.toString());
  // Now set a new state via setRaw();
  // This state is a real state from an AR-DB1 remote.
  uint8_t new_state1[kFujitsuAcStateLength - 1] = {
    0x14, 0x63, 0x00, 0x10, 0x10, 0xFC, 0x08, 0x30,
    0x30, 0x01, 0x00, 0x00, 0x00, 0x00, 0x9F};
  fujitsu.setRaw(new_state1, kFujitsuAcStateLength - 1);
  EXPECT_EQ(kFujitsuAcStateLength - 1, fujitsu.getStateLength());
  EXPECT_TRUE(ArraysMatch(new_state1, fujitsu.getRaw()));
  EXPECT_EQ("Power: On, Mode: 1 (COOL), Temp: 19C, Fan: 0 (AUTO), "
            "Swing: Off, Command: N/A", fujitsu.toString());
}

TEST(TestSendFujitsuAC, GenerateMessage) {
  IRFujitsuAC fujitsu = IRFujitsuAC(4);
  IRsendTest irsend(4);
  fujitsu.begin();
  irsend.begin();

  fujitsu.setCmd(kFujitsuAcCmdStayOn);
  fujitsu.setSwing(kFujitsuAcSwingBoth);
  fujitsu.setMode(kFujitsuAcModeCool);
  fujitsu.setFanSpeed(kFujitsuAcFanHigh);
  fujitsu.setTemp(24);

  EXPECT_EQ(kFujitsuAcFanHigh, fujitsu.getFanSpeed());
  EXPECT_EQ(kFujitsuAcModeCool, fujitsu.getMode());
  EXPECT_EQ(24, fujitsu.getTemp());
  EXPECT_EQ(kFujitsuAcSwingBoth, fujitsu.getSwing());
  EXPECT_EQ(kFujitsuAcCmdStayOn, fujitsu.getCmd());

  irsend.reset();
  irsend.sendFujitsuAC(fujitsu.getRaw(), kFujitsuAcStateLength);
  EXPECT_EQ(
  "m3324s1574"
  "m448s390m448s390m448s1182m448s390m448s1182m448s390m448s390m448s390"
  "m448s1182m448s1182m448s390m448s390m448s390m448s1182m448s1182m448s390"
  "m448s390m448s390m448s390m448s390m448s390m448s390m448s390m448s390"
  "m448s390m448s390m448s390m448s390m448s1182m448s390m448s390m448s390"
  "m448s390m448s390m448s390m448s390m448s1182m448s390m448s390m448s390"
  "m448s390m448s1182m448s1182m448s1182m448s1182m448s1182m448s1182m448s1182"
  "m448s1182m448s390m448s390m448s1182m448s390m448s390m448s390m448s390"
  "m448s390m448s390m448s390m448s390m448s1182m448s1182m448s390m448s390"
  "m448s390m448s390m448s390m448s390m448s390m448s390m448s390m448s1182"
  "m448s1182m448s390m448s390m448s390m448s390m448s390m448s390m448s390"
  "m448s1182m448s390m448s390m448s390m448s1182m448s1182m448s390m448s390"
  "m448s390m448s390m448s390m448s390m448s390m448s390m448s390m448s390"
  "m448s390m448s390m448s390m448s390m448s390m448s390m448s390m448s390"
  "m448s390m448s390m448s390m448s390m448s390m448s390m448s390m448s390"
  "m448s390m448s390m448s390m448s390m448s390m448s1182m448s390m448s390"
  "m448s390m448s1182m448s1182m448s1182m448s1182m448s1182m448s1182m448s1182"
  "m448s8100",
  irsend.outputStr());
}

TEST(TestSendFujitsuAC, GenerateShortMessage) {
  IRFujitsuAC fujitsu = IRFujitsuAC(4);
  IRsendTest irsend(4);
  fujitsu.begin();
  irsend.begin();

  fujitsu.off();

  EXPECT_EQ(kFujitsuAcCmdTurnOff, fujitsu.getCmd());

  irsend.reset();
  irsend.sendFujitsuAC(fujitsu.getRaw(), kFujitsuAcStateLengthShort);
  EXPECT_EQ(
  "m3324s1574m448s390m448s390m448s1182m448s390m448s1182m448s390m448s390m448"
  "s390m448s1182m448s1182m448s390m448s390m448s390m448s1182m448s1182m448s390"
  "m448s390m448s390m448s390m448s390m448s390m448s390m448s390m448s390m448s390"
  "m448s390m448s390m448s390m448s1182m448s390m448s390m448s390m448s390m448s390"
  "m448s390m448s390m448s1182m448s390m448s390m448s390m448s390m448s1182m448s390"
  "m448s390m448s390m448s390m448s390m448s390m448s1182m448s390m448s1182m448"
  "s1182m448s1182m448s1182m448s1182m448s1182m448s8100",
  irsend.outputStr());
}

// Issue #275
TEST(TestSendFujitsuAC, Issue275) {
  IRFujitsuAC fujitsu = IRFujitsuAC(4);
  IRsendTest irsend(4);
  fujitsu.begin();
  irsend.begin();
  irsend.reset();

  fujitsu.setCmd(kFujitsuAcCmdTurnOff);
  irsend.sendFujitsuAC(fujitsu.getRaw(), kFujitsuAcStateLengthShort);
  EXPECT_EQ(
      // Header
      "m3324s1574"
      //  0       0       1        0       1        0       0       0     (0x28)
      "m448s390m448s390m448s1182m448s390m448s1182m448s390m448s390m448s390"
      //  1        1        0       0       0       1        1        0   (0xC6)
      "m448s1182m448s1182m448s390m448s390m448s390m448s1182m448s1182m448s390"
      //  0       0       0       0       0       0       0       0       (0x00)
      "m448s390m448s390m448s390m448s390m448s390m448s390m448s390m448s390"
      //  0       0       0       0       1        0       0       0      (0x08)
      "m448s390m448s390m448s390m448s390m448s1182m448s390m448s390m448s390"
      //  0       0       0       0       1        0       0       0      (0x08)
      "m448s390m448s390m448s390m448s390m448s1182m448s390m448s390m448s390"
      //  0       1        0       0       0       0       0       0      (0x40)
      "m448s390m448s1182m448s390m448s390m448s390m448s390m448s390m448s390"
      //  1        0       1        1        1        1        1        1 (0xBF)
      "m448s1182m448s390m448s1182m448s1182m448s1182m448s1182m448s1182m448s1182"
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

TEST(TestDecodeFujitsuAC, SyntheticShortMessages) {
  IRsendTest irsend(0);
  IRFujitsuAC fujitsu = IRFujitsuAC(0);
  IRrecv irrecv(0);

  irsend.begin();
  irsend.reset();

  fujitsu.setModel(ARRAH2E);
  fujitsu.setCmd(kFujitsuAcCmdTurnOff);
  irsend.sendFujitsuAC(fujitsu.getRaw(), fujitsu.getStateLength());
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(FUJITSU_AC, irsend.capture.decode_type);
  ASSERT_EQ(kFujitsuAcMinBits + 8, irsend.capture.bits);
  uint8_t expected_arrah2e[7] = {0x14, 0x63, 0x0, 0x10, 0x10, 0x02, 0xFD};
  EXPECT_TRUE(ArraysMatch(expected_arrah2e, irsend.capture.state));

  irsend.reset();

  fujitsu.setModel(ARDB1);
  fujitsu.setCmd(kFujitsuAcCmdTurnOff);
  irsend.sendFujitsuAC(fujitsu.getRaw(), fujitsu.getStateLength());
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(FUJITSU_AC, irsend.capture.decode_type);
  ASSERT_EQ(kFujitsuAcMinBits, irsend.capture.bits);
  uint8_t expected_ardb1[6] = {0x14, 0x63, 0x0, 0x10, 0x10, 0x02};
  EXPECT_TRUE(ArraysMatch(expected_ardb1, irsend.capture.state));
}

TEST(TestDecodeFujitsuAC, SyntheticLongMessages) {
  IRsendTest irsend(0);
  IRFujitsuAC fujitsu = IRFujitsuAC(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();

  fujitsu.setModel(ARRAH2E);
  fujitsu.setCmd(kFujitsuAcCmdStayOn);
  fujitsu.setSwing(kFujitsuAcSwingVert);
  fujitsu.setMode(kFujitsuAcModeCool);
  fujitsu.setFanSpeed(kFujitsuAcFanQuiet);
  fujitsu.setTemp(18);
  irsend.sendFujitsuAC(fujitsu.getRaw(), fujitsu.getStateLength());
  ASSERT_EQ(kFujitsuAcStateLength, fujitsu.getStateLength());
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decodeFujitsuAC(&irsend.capture));
  ASSERT_EQ(FUJITSU_AC, irsend.capture.decode_type);
  ASSERT_EQ(kFujitsuAcBits, irsend.capture.bits);
  uint8_t expected_arrah2e[kFujitsuAcStateLength] = {
    0x14, 0x63, 0x00, 0x10, 0x10, 0xFE, 0x09, 0x30,
    0x20, 0x01, 0x14, 0x00, 0x00, 0x00, 0x20, 0x7B};
  EXPECT_TRUE(ArraysMatch(expected_arrah2e, irsend.capture.state));
  fujitsu.setRaw(irsend.capture.state, irsend.capture.bits / 8);
  EXPECT_EQ(kFujitsuAcStateLength, fujitsu.getStateLength());
  EXPECT_EQ("Power: On, Mode: 1 (COOL), Temp: 18C, Fan: 4 (QUIET), "
            "Swing: Vert, Command: N/A", fujitsu.toString());

  irsend.reset();

  fujitsu.setModel(ARDB1);
  irsend.sendFujitsuAC(fujitsu.getRaw(), fujitsu.getStateLength());
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(FUJITSU_AC, irsend.capture.decode_type);
  ASSERT_EQ(kFujitsuAcBits - 8, irsend.capture.bits);
  uint8_t expected_ardb1[kFujitsuAcStateLength - 1] = {
    0x14, 0x63, 0x00, 0x10, 0x10, 0xFC, 0x08, 0x30,
    0x20, 0x01, 0x14, 0x00, 0x00, 0x00, 0x9B};
  EXPECT_TRUE(ArraysMatch(expected_ardb1, irsend.capture.state));
  fujitsu.setRaw(irsend.capture.state, irsend.capture.bits / 8);
  EXPECT_EQ(kFujitsuAcStateLength - 1, fujitsu.getStateLength());
  EXPECT_EQ("Power: On, Mode: 1 (COOL), Temp: 18C, Fan: 4 (QUIET), "
            "Swing: Vert, Command: N/A", fujitsu.toString());
}

TEST(TestDecodeFujitsuAC, RealShortARDB1OffExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  IRFujitsuAC fujitsu = IRFujitsuAC(0);

  irsend.begin();

  irsend.reset();
  // "Off" Message recorded from an AR-DB1 remote.
  uint16_t rawData[99] = {
      3310, 1636,  440, 386,  440, 394,  442, 1210,  442, 390,  414, 1220,
      444, 390,  446, 380,  446, 380,  436, 1216,  438, 1214,  438, 388,
      438, 386,  438, 396,  410, 1222,  440, 1220,  442, 384,  442, 384,
      442, 384,  442, 382,  444, 382,  442, 382,  444, 380,  446, 380,
      446, 380,  444, 380,  436, 390,  436, 388,  436, 388,  438, 1214,
      438, 386,  438, 388,  438, 386,  440, 386,  440, 384,  442, 384,
      442, 384,  442, 1210,  444, 382,  444, 382,  444, 382,  444, 380,
      446, 1206,  436, 390,  436, 388,  436, 388,  438, 388,  438, 396,
      420, 388,  436};
  irsend.sendRaw(rawData, 99, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(FUJITSU_AC, irsend.capture.decode_type);
  ASSERT_EQ(kFujitsuAcMinBits, irsend.capture.bits);
  uint8_t expected[6] = {0x14, 0x63, 0x0, 0x10, 0x10, 0x02};
  EXPECT_TRUE(ArraysMatch(expected, irsend.capture.state));
  fujitsu.setRaw(irsend.capture.state, irsend.capture.bits / 8);
  EXPECT_EQ(kFujitsuAcStateLengthShort - 1, fujitsu.getStateLength());
  EXPECT_EQ("Power: Off, Mode: 0 (AUTO), Temp: 16C, Fan: 0 (AUTO), "
            "Swing: Off, Command: N/A", fujitsu.toString());
}

TEST(TestDecodeFujitsuAC, RealLongARDB1Example) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  IRFujitsuAC fujitsu = IRFujitsuAC(0);

  irsend.begin();
  irsend.reset();
  uint16_t rawData1[243] = {
      3316, 1632,  444, 390,  438, 388,  436, 1216,  438, 388,  438, 1214,
      438, 388,  438, 386,  440, 386,  440, 1212,  440, 1210,  442, 392,
      412, 396,  442, 392,  444, 1208,  444, 1208,  444, 380,  444, 380,
      446, 380,  436, 390,  436, 390,  436, 390,  436, 388,  438, 388,
      438, 388,  438, 388,  438, 386,  438, 386,  440, 384,  440, 1210,
      442, 384,  442, 382,  442, 384,  442, 384,  442, 382,  442, 382,
      444, 382,  444, 1208,  444, 382,  444, 380,  446, 380,  436, 390,
      436, 390,  436, 1214,  438, 1214,  438, 1212,  440, 1212,  440, 1220,
      412, 1222,  440, 394,  442, 382,  442, 382,  444, 1208,  444, 382,
      444, 380,  446, 380,  446, 380,  434, 390,  436, 388,  438, 388,
      438, 388,  438, 1214,  438, 1212,  440, 386,  440, 394,  412, 1222,
      440, 394,  442, 384,  442, 384,  442, 382,  442, 1208,  444, 390,
      414, 394,  442, 1216,  446, 380,  436, 390,  436, 390,  436, 388,
      436, 390,  436, 388,  438, 386,  440, 386,  440, 386,  438, 1212,
      440, 386,  440, 384,  440, 384,  442, 392,  412, 396,  440, 394,
      442, 382,  444, 382,  444, 382,  444, 380,  444, 380,  444, 382,
      444, 380,  446, 380,  436, 388,  436, 390,  436, 388,  438, 388,
      438, 388,  438, 388,  438, 386,  440, 386,  440, 386,  442, 384,
      440, 386,  442, 384,  440, 384,  442, 384,  442, 382,  442, 382,
      444, 1208,  444, 382,  444, 1208,  444, 380,  446, 1206,  436, 390,
      436, 1216,  436};
  irsend.sendRaw(rawData1, 243, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(FUJITSU_AC, irsend.capture.decode_type);
  ASSERT_EQ(kFujitsuAcBits - 8, irsend.capture.bits);
  uint8_t expected1[kFujitsuAcStateLength - 1] = {
      0x14, 0x63, 0x00, 0x10, 0x10, 0xFC, 0x08, 0x30,
      0x21, 0x01, 0x04, 0x00, 0x00, 0x00, 0xAA};
  EXPECT_TRUE(ArraysMatch(expected1, irsend.capture.state));
  fujitsu.setRaw(irsend.capture.state, irsend.capture.bits / 8);
  EXPECT_EQ(kFujitsuAcStateLength - 1, fujitsu.getStateLength());
  EXPECT_EQ("Power: On, Mode: 1 (COOL), Temp: 18C, Fan: 4 (QUIET), "
            "Swing: Off, Command: N/A", fujitsu.toString());

  irsend.reset();
  uint16_t rawData2[243] = {
      3316, 1630,  436, 398,  438, 386,  438, 1212,  440, 384,  440, 1212,
      442, 384,  442, 392,  414, 394,  442, 1218,  446, 1206,  436, 390,
      436, 388,  438, 388,  438, 1214,  440, 1212,  440, 384,  442, 384,
      442, 384,  442, 382,  444, 382,  444, 382,  444, 380,  446, 380,
      444, 380,  436, 390,  436, 388,  438, 396,  418, 388,  438, 1232,
      410, 396,  440, 394,  442, 384,  442, 384,  442, 382,  442, 392,
      414, 392,  444, 1216,  446, 380,  436, 390,  436, 396,  418, 390,
      436, 398,  438, 1214,  440, 1212,  440, 1210,  442, 1208,  444, 1216,
      416, 1218,  444, 388,  436, 390,  436, 388,  438, 1214,  440, 386,
      438, 386,  440, 386,  440, 384,  442, 384,  442, 384,  442, 382,
      444, 382,  444, 1206,  446, 1206,  436, 390,  436, 388,  438, 388,
      438, 386,  440, 394,  410, 396,  440, 1220,  442, 1210,  442, 392,
      414, 394,  442, 1218,  446, 406,  410, 388,  436, 390,  436, 390,
      436, 388,  438, 386,  440, 386,  440, 386,  440, 386,  440, 384,
      442, 384,  442, 384,  442, 382,  444, 382,  444, 380,  446, 380,
      446, 380,  436, 390,  436, 390,  436, 388,  438, 386,  438, 388,
      438, 386,  440, 386,  440, 384,  442, 384,  442, 384,  442, 384,
      442, 382,  444, 382,  444, 380,  446, 380,  446, 380,  436, 390,
      436, 388,  436, 388,  438, 386,  438, 386,  440, 386,  440, 1212,
      440, 1210,  442, 1210,  442, 1208,  444, 1208,  436, 390,  436, 388,
      436, 1214,  440};
  irsend.sendRaw(rawData2, 243, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(FUJITSU_AC, irsend.capture.decode_type);
  ASSERT_EQ(kFujitsuAcBits - 8, irsend.capture.bits);
  uint8_t expected2[kFujitsuAcStateLength - 1] = {
      0x14, 0x63, 0x00, 0x10, 0x10, 0xFC, 0x08, 0x30,
      0x30, 0x01, 0x00, 0x00, 0x00, 0x00, 0x9F};
  EXPECT_TRUE(ArraysMatch(expected2, irsend.capture.state));
  fujitsu.setRaw(irsend.capture.state, irsend.capture.bits / 8);
  EXPECT_EQ(kFujitsuAcStateLength - 1, fujitsu.getStateLength());
  EXPECT_EQ("Power: On, Mode: 1 (COOL), Temp: 19C, Fan: 0 (AUTO), "
            "Swing: Off, Command: N/A", fujitsu.toString());
}

TEST(TestDecodeFujitsuAC, Issue414) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  IRFujitsuAC fujitsu = IRFujitsuAC(0);

  // Capture as supplied by arpmota
  uint16_t rawData[259] = {3352, 1574, 480, 350, 480, 346, 480, 1190, 458, 346,
      508, 1140, 480, 346, 506, 346, 458, 346, 480, 1168, 480, 1192, 452, 374,
      458, 346, 480, 346, 508, 1168, 480, 1140, 480, 346, 506, 346, 458, 346,
      480, 346, 480, 346, 480, 346, 484, 372, 454, 374, 456, 346, 508, 318,
      480, 374, 458, 374, 480, 318, 480, 1196, 452, 346, 480, 346, 484, 342,
      484, 346, 480, 374, 458, 346, 506, 318, 508, 1170, 452, 346, 480, 374,
      458, 346, 506, 318, 480, 1196, 452, 1190, 458, 1162, 480, 1196, 452,
      1170, 480, 1190, 458, 1164, 480, 1196, 480, 318, 508, 346, 456, 1192,
      480, 346, 456, 374, 452, 346, 480, 374, 458, 342, 484, 346, 508, 346,
      456, 342, 512, 1164, 458, 1164, 508, 346, 456, 346, 480, 1190, 456, 342,
      484, 346, 506, 346, 456, 374, 452, 346, 508, 346, 458, 1164, 508, 346,
      458, 374, 452, 1168, 480, 374, 480, 318, 480, 374, 456, 346, 508, 318,
      480, 346, 484, 374, 480, 318, 484, 342, 484, 374, 480, 318, 484, 342,
      484, 346, 508, 318, 508, 346, 458, 346, 506, 318, 480, 374, 458, 346,
      506, 318, 480, 346, 484, 374, 480, 318, 482, 372, 456, 346, 508, 318,
      506, 348, 456, 342, 484, 346, 508, 318, 484, 374, 480, 318, 508, 318,
      484, 346, 508, 318, 480, 374, 456, 346, 508, 346, 480, 318, 480, 346,
      484, 374, 480, 320, 484, 1164, 508, 346, 458, 342, 512, 1164, 458, 1190,
      454, 346, 484, 1164, 508, 346, 458, 1164, 480, 350, 480, 374, 480};
  uint8_t state[16] = {
      0x14, 0x63, 0x00, 0x10, 0x10, 0xFE, 0x09, 0x30, 0x81, 0x04, 0x00, 0x00,
      0x00, 0x00, 0x20, 0x2B};
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 259, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(FUJITSU_AC, irsend.capture.decode_type);
  ASSERT_EQ(kFujitsuAcBits, irsend.capture.bits);
  EXPECT_TRUE(ArraysMatch(state, irsend.capture.state));
  fujitsu.setRaw(irsend.capture.state, irsend.capture.bits / 8);
  EXPECT_EQ(kFujitsuAcStateLength, fujitsu.getStateLength());
  EXPECT_EQ("Power: On, Mode: 4 (HEAT), Temp: 24C, Fan: 0 (AUTO), "
            "Swing: Off, Command: N/A", fujitsu.toString());

  // Resend it using the state this time.
  irsend.reset();
  irsend.sendFujitsuAC(state, 16);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(FUJITSU_AC, irsend.capture.decode_type);
  ASSERT_EQ(kFujitsuAcBits, irsend.capture.bits);
  EXPECT_TRUE(ArraysMatch(state, irsend.capture.state));
  EXPECT_EQ(
      "m3324s1574"
      "m448s390m448s390m448s1182m448s390m448s1182m448s390m448s390m448s390"
      "m448s1182m448s1182m448s390m448s390m448s390m448s1182m448s1182m448s390"
      "m448s390m448s390m448s390m448s390m448s390m448s390m448s390m448s390"
      "m448s390m448s390m448s390m448s390m448s1182m448s390m448s390m448s390"
      "m448s390m448s390m448s390m448s390m448s1182m448s390m448s390m448s390"
      "m448s390m448s1182m448s1182m448s1182m448s1182m448s1182m448s1182m448s1182"
      "m448s1182m448s390m448s390m448s1182m448s390m448s390m448s390m448s390"
      "m448s390m448s390m448s390m448s390m448s1182m448s1182m448s390m448s390"
      "m448s1182m448s390m448s390m448s390m448s390m448s390m448s390m448s1182"
      "m448s390m448s390m448s1182m448s390m448s390m448s390m448s390m448s390"
      "m448s390m448s390m448s390m448s390m448s390m448s390m448s390m448s390"
      "m448s390m448s390m448s390m448s390m448s390m448s390m448s390m448s390"
      "m448s390m448s390m448s390m448s390m448s390m448s390m448s390m448s390"
      "m448s390m448s390m448s390m448s390m448s390m448s390m448s390m448s390"
      "m448s390m448s390m448s390m448s390m448s390m448s1182m448s390m448s390"
      "m448s1182m448s1182m448s390m448s1182m448s390m448s1182m448s390m448s390"
      "m448s8100", irsend.outputStr());
}
