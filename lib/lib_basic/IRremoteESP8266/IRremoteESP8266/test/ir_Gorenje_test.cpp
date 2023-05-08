// Copyright 2022 Mateusz Bronk (mbronk)

#include "IRac.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

class TestDecodeGorenjeSyntheticSendTestFixture
  : public ::testing::TestWithParam<uint64_t> {};
class TestDecodeGorenjeReceiveTestFixture
  : public ::testing::TestWithParam<std::tuple<std::vector<uint16_t>,
                                               uint64_t>> {};

TEST(TestGorenje, Settings) {
  ASSERT_EQ("GORENJE", typeToString(decode_type_t::GORENJE));
  ASSERT_EQ(decode_type_t::GORENJE, strToDecodeType("GORENJE"));
  ASSERT_FALSE(hasACState(decode_type_t::GORENJE));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::GORENJE));
  ASSERT_EQ(kGorenjeBits,
            IRsendTest::defaultBits(decode_type_t::GORENJE));
}

// Test sending typical data (cooker hood light toggle)
TEST(TestSendGorenje, SendLightToggle) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendGorenje(0x10);  // Light toggle
  EXPECT_EQ(
    "f38000d50"
    "m1300s1700m1300s1700m1300s1700m1300s5700m1300s1700m1300s1700m1300s1700"
    "m1300s1700"
    "m1300s100000",
    irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendGorenje, SendWithRepeats) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendGorenje(0x8, kGorenjeBits, 0);  // 0 repeats.
  EXPECT_EQ(
      "f38000d50"
      "m1300s1700m1300s1700m1300s1700m1300s1700m1300s5700m1300s1700m1300s1700"
      "m1300s1700"
      "m1300s100000",
      irsend.outputStr());
  irsend.sendGorenje(0x8, kGorenjeBits, 2);  // 2 repeats.
  EXPECT_EQ(
      "f38000d50"
      "m1300s1700m1300s1700m1300s1700m1300s1700m1300s5700m1300s1700m1300s1700"
      "m1300s1700"
      "m1300s100000"
      "m1300s1700m1300s1700m1300s1700m1300s1700m1300s5700m1300s1700m1300s1700"
      "m1300s1700"
      "m1300s100000"
      "m1300s1700m1300s1700m1300s1700m1300s1700m1300s5700m1300s1700m1300s1700"
      "m1300s1700"
      "m1300s100000",
      irsend.outputStr());
}


// Decode a Synthetic example
TEST_P(TestDecodeGorenjeSyntheticSendTestFixture, SyntheticExample) {
  uint64_t commandToTest = GetParam();
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendGorenje(commandToTest);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(GORENJE, irsend.capture.decode_type);
  EXPECT_EQ(kGorenjeBits, irsend.capture.bits);
  EXPECT_EQ(commandToTest, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

INSTANTIATE_TEST_CASE_P(
  TestDecodeGorenje,
  TestDecodeGorenjeSyntheticSendTestFixture,
  ::testing::Values(0x2, 0x8, 0x4, 0x10, 0x20, 0x1));


// Decode a real example (codes captured from original remote)
TEST_P(TestDecodeGorenjeReceiveTestFixture, RealExample) {
  const std::vector<uint16_t> rawDataInput = std::get<0>(GetParam());
  const uint64_t expectedValue = std::get<1>(GetParam());

  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendRaw(&rawDataInput[0], rawDataInput.size(), 38000);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::GORENJE, irsend.capture.decode_type);
  EXPECT_EQ(kGorenjeBits, irsend.capture.bits);
  EXPECT_EQ(expectedValue, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

INSTANTIATE_TEST_CASE_P(
  TestDecodeGorenje,
  TestDecodeGorenjeReceiveTestFixture,
  ::testing::Values(
    // POWER
    std::make_tuple(std::vector<uint16_t> {
        1292, 1716,  1300, 1708,  1296, 1712,
        1294, 1714,  1302, 1708,  1298, 1710,
        1294, 5752,  1294, 1714,  1302
      },
      0x2),
    std::make_tuple(std::vector<uint16_t> {
        1302, 1706,  1298, 1710,  1294, 1714,
        1292, 1716,  1300, 1708,  1296, 1712,
        1294, 5754,  1292, 1716,  1300
      },
      0x2),

    // FAN-
    std::make_tuple(std::vector<uint16_t> {
        1328, 1680,  1324, 1684,  1320, 1688,
        1328, 1680,  1324, 5724,  1322, 1684,
        1330, 1678,  1326, 1682,  1324
      },
      0x8),
    std::make_tuple(std::vector<uint16_t> {
      1296, 1712,  1292, 1718,  1298, 1710,
      1294, 1714,  1292, 5756,  1300, 1708,
      1296, 1712,  1294, 1714,  1300
      },
      0x8),

    // FAN+
    std::make_tuple(std::vector<uint16_t> {
        1324, 1684,  1320, 1688,  1328, 1680,
        1324, 1684,  1322, 1688,  1328, 5718,
        1326, 1682,  1322, 1686,  1352
      },
      0x4),
    std::make_tuple(std::vector<uint16_t> {
        1296, 1714,  1292, 1716,  1298, 1710,
        1296, 1714,  1292, 1716,  1300, 5748,
        1296, 1712,  1292, 1716,  1300
      },
      0x4),

    // Light toggle
    std::make_tuple(std::vector<uint16_t> {
        1326, 1682,  1322, 1686,  1328, 1680,
        1324, 5722,  1322, 1686,  1330, 1680,
        1326, 1682,  1322, 1686,  1328
      },
      0x10),
    std::make_tuple(std::vector<uint16_t> {
        1328, 1680,  1324, 1684,  1330, 1678,
        1326, 5722,  1324, 1684,  1330, 1678,
        1326, 1682,  1322, 1686,  1330
      },
      0x10),

    // Light-
    std::make_tuple(std::vector<uint16_t> {
        1328, 1680,  1324, 1686,  1330, 5716,
        1328, 1680,  1324, 1684,  1330, 1678,
        1326, 1682,  1354, 1654,  1328
      },
      0x20),
    std::make_tuple(std::vector<uint16_t> {
        1322, 1686,  1318, 1690,  1326, 5722,
        1322, 1686,  1330, 1678,  1326, 1682,
        1322, 1686,  1328, 1680,  1324
      },
      0x20),

    // Light+
    std::make_tuple(std::vector<uint16_t> {
        1328, 1680,  1326, 1682,  1322, 1688,
        1328, 1680,  1324, 1686,  1330, 1678,
        1326, 1682,  1322, 5724,  1330
      },
      0x1),
    std::make_tuple(std::vector<uint16_t> {
        1298, 1710,  1294, 1714,  1302, 1708,
        1298, 1710,  1294, 1716,  1300, 1708,
        1296, 1712,  1292, 5756,  1300
      },
      0x1)
  )
);
