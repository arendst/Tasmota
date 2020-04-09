// Copyright 2019 David Conran

#include "IRac.h"
#include "ir_Amcor.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "IRutils.h"
#include "gtest/gtest.h"

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("AMCOR", typeToString(decode_type_t::AMCOR));
  ASSERT_EQ(decode_type_t::AMCOR, strToDecodeType("AMCOR"));
  ASSERT_TRUE(hasACState(decode_type_t::AMCOR));
  ASSERT_TRUE(IRac::isProtocolSupported(decode_type_t::AMCOR));
}

// Test sending typical data only.
TEST(TestSendAmcor, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();

  uint8_t expectedState[kAmcorStateLength] = {
      0x01, 0x41, 0x36, 0x00, 0x00, 0x30, 0x00, 0x12};

  irsend.reset();
  irsend.sendAmcor(expectedState);
  EXPECT_EQ(
      "f38000d50"
      "m8200s4200"
      "m1500s600m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500"
      "m1500s600m600s1500m600s1500m600s1500m600s1500m600s1500m1500s600m600s1500"
      "m600s1500m1500s600m1500s600m600s1500m1500s600m1500s600m600s1500m600s1500"
      "m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500"
      "m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500"
      "m600s1500m600s1500m600s1500m600s1500m1500s600m1500s600m600s1500m600s1500"
      "m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500"
      "m600s1500m1500s600m600s1500m600s1500m1500s600m600s1500m600s1500m600s1500"
      "m1900s34300"
      "m8200s4200"
      "m1500s600m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500"
      "m1500s600m600s1500m600s1500m600s1500m600s1500m600s1500m1500s600m600s1500"
      "m600s1500m1500s600m1500s600m600s1500m1500s600m1500s600m600s1500m600s1500"
      "m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500"
      "m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500"
      "m600s1500m600s1500m600s1500m600s1500m1500s600m1500s600m600s1500m600s1500"
      "m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500m600s1500"
      "m600s1500m1500s600m600s1500m600s1500m1500s600m600s1500m600s1500m600s1500"
      "m1900s34300",
      irsend.outputStr());
}

TEST(TestDecodeAmcor, SyntheticSelfDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  IRAmcorAc ac(0);

  uint8_t expectedState[kAmcorStateLength] = {
      0x01, 0x41, 0x30, 0x00, 0x00, 0x30, 0x00, 0x0C};

  irsend.begin();
  irsend.reset();
  irsend.sendAmcor(expectedState);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(AMCOR, irsend.capture.decode_type);
  EXPECT_EQ(kAmcorBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  ac.setRaw(irsend.capture.state);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Fan: 4 (Auto), Temp: 24C, Max: Off",
      ac.toString());
}

TEST(TestDecodeAmcor, RealExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Data from Issue #834 captured by ldellus
  // Turn on, cooling, 27 deg C.
  uint16_t rawData[263] = {
    8210, 4276, 1544, 480, 596, 1510, 596, 1510, 596, 1692, 388, 1534, 596,
    1510, 596, 1510, 596, 1684, 1450, 480, 596, 1510, 570, 1534, 570, 1718, 386,
    1536, 594, 1500, 1632, 482, 596, 1694, 362, 1550, 1632, 472, 1658, 456, 596,
    1684, 1474, 446, 1634, 480, 572, 1534, 572, 1718, 362, 1558, 572, 1534, 570,
    1534, 570, 1720, 360, 1558, 572, 1534, 570, 1534, 570, 1718, 360, 1560, 572,
    1534, 570, 1534, 570, 1718, 362, 1560, 572, 1532, 572, 1534, 570, 1718, 362,
    1558, 572, 1532, 572, 1534, 570, 1710, 1448, 472, 1634, 480, 572, 1534, 570,
    1718, 362, 1558, 572, 1534, 570, 1534, 572, 1716, 362, 1560, 572, 1534, 572,
    1534, 570, 1718, 362, 1550, 1634, 480, 570, 1536, 570, 1710, 1448, 482, 570,
    1534, 570, 1536, 570, 1508, 1856, 34298,
    // rawData[132] is here. (8218)
    8218, 4314, 1502, 522, 530, 1576, 504, 1602, 504, 1786, 392, 1528, 504,
    1600, 504, 1600, 504, 1770, 1414, 522, 528, 1578, 502, 1602, 504, 1784, 394,
    1528, 504, 1584, 1574, 548, 528, 1762, 392, 1512, 1572, 530, 1600, 524, 528,
    1744, 1390, 530, 1574, 546, 506, 1600, 504, 1784, 394, 1528, 504, 1600, 578,
    1528, 504, 1784, 394, 1526, 504, 1600, 504, 1600, 506, 1784, 394, 1528, 504,
    1602, 504, 1602, 504, 1784, 394, 1526, 506, 1600, 504, 1600, 506, 1784, 392,
    1526, 506, 1600, 504, 1602, 502, 1768, 1390, 530, 1574, 548, 504, 1600, 504,
    1786, 392, 1530, 504, 1600, 504, 1600, 504, 1786, 392, 1528, 504, 1600, 504,
    1600, 506, 1784, 394, 1512, 1574, 548, 504, 1602, 504, 1768, 1388, 548, 504,
    1602, 504, 1602, 502, 1574, 1792};  // UNKNOWN D510A6EF

  uint8_t expectedState[kAmcorStateLength] = {
      0x01, 0x41, 0x36, 0x00, 0x00, 0x30, 0x00, 0x12};

  irsend.reset();
  irsend.sendRaw(rawData, 263, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(AMCOR, irsend.capture.decode_type);
  EXPECT_EQ(kAmcorBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);

  // Verify the repeat is the same decode.
  irsend.reset();
  irsend.sendRaw(rawData + 132, 131, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(AMCOR, irsend.capture.decode_type);
  EXPECT_EQ(kAmcorBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);

  // https://github.com/crankyoldgit/IRremoteESP8266/issues/834#issuecomment-515700254
  uint16_t rawData2[263] = {8252, 4294, 1518, 508, 544, 1560, 546, 1560, 570,
    1718, 416, 1504, 546, 1560, 570, 1532, 572, 1718, 1414, 506, 544, 1560, 570,
    1534, 570, 1718, 416, 1506, 544, 1558, 1598, 508, 544, 1746, 416, 1504, 546,
    1560, 570, 1534, 1598, 690, 1414, 508, 544, 1560, 546, 1560, 544, 1746, 416,
    1504, 546, 1560, 546, 1560, 570, 1718, 416, 1504, 544, 1560, 570, 1536, 544,
    1744, 416, 1506, 570, 1534, 546, 1558, 546, 1744, 418, 1502, 572, 1534, 544,
    1560, 570, 1720, 416, 1506, 544, 1560, 546, 1560, 544, 1744, 1414, 506,
    1572, 534, 544, 1560, 570, 1720, 416, 1504, 570, 1536, 544, 1560, 572, 1718,
    416, 1504, 570, 1542, 592, 1504, 570, 1720, 416, 1502, 1572, 534, 544, 1560,
    572, 1718, 1414, 508, 544, 1560, 570, 1534, 570, 1508, 1840, 34174, 8230,
    4292, 1546, 480, 546, 1560, 572, 1534, 570, 1718, 416, 1502, 572, 1532, 572,
    1532, 572, 1718, 1440, 480, 570, 1534, 572, 1534, 572, 1716, 418, 1504, 572,
    1532, 1626, 480, 572, 1718, 418, 1502, 574, 1534, 572, 1530, 1626, 662,
    1442, 480, 572, 1534, 572, 1534, 572, 1716, 418, 1502, 574, 1542, 592, 1504,
    598, 1692, 418, 1504, 572, 1532, 574, 1530, 574, 1716, 418, 1502, 598, 1508,
    572, 1532, 598, 1692, 418, 1502, 598, 1508, 572, 1532, 574, 1716, 418, 1504,
    598, 1508, 572, 1532, 574, 1716, 1442, 478, 1626, 480, 572, 1534, 572, 1718,
    392, 1526, 574, 1532, 572, 1532, 572, 1716, 418, 1502, 598, 1508, 574, 1532,
    598, 1700, 408, 1504, 1624, 480, 572, 1532, 574, 1716, 1440, 480, 572, 1532,
    572, 1532, 572, 1506, 1814};  // UNKNOWN ADA838FB

    uint8_t expectedState2[kAmcorStateLength] = {
        0x01, 0x41, 0x18, 0x00, 0x00, 0x30, 0x00, 0x12};

    irsend.reset();
    irsend.sendRaw(rawData2, 263, 38000);
    irsend.makeDecodeResult();
    ASSERT_TRUE(irrecv.decode(&irsend.capture));
    EXPECT_EQ(AMCOR, irsend.capture.decode_type);
    EXPECT_EQ(kAmcorBits, irsend.capture.bits);
    EXPECT_STATE_EQ(expectedState2, irsend.capture.state, irsend.capture.bits);
}

// Tests for IRAmcorAc class.

TEST(TestAmcorAcClass, Power) {
  IRAmcorAc ac(0);
  ac.begin();

  ac.on();
  EXPECT_TRUE(ac.getPower());

  ac.off();
  EXPECT_FALSE(ac.getPower());

  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());

  ac.setPower(false);
  EXPECT_FALSE(ac.getPower());
}

TEST(TestAmcorAcClass, Temperature) {
  IRAmcorAc ac(0);
  ac.begin();

  ac.setTemp(0);
  EXPECT_EQ(kAmcorMinTemp, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kAmcorMaxTemp, ac.getTemp());

  ac.setTemp(kAmcorMinTemp);
  EXPECT_EQ(kAmcorMinTemp, ac.getTemp());

  ac.setTemp(kAmcorMaxTemp);
  EXPECT_EQ(kAmcorMaxTemp, ac.getTemp());

  ac.setTemp(kAmcorMinTemp - 1);
  EXPECT_EQ(kAmcorMinTemp, ac.getTemp());

  ac.setTemp(kAmcorMaxTemp + 1);
  EXPECT_EQ(kAmcorMaxTemp, ac.getTemp());

  ac.setTemp(17);
  EXPECT_EQ(17, ac.getTemp());

  ac.setTemp(21);
  EXPECT_EQ(21, ac.getTemp());

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());

  ac.setTemp(29);
  EXPECT_EQ(29, ac.getTemp());
}

TEST(TestAmcorAcClass, OperatingMode) {
  IRAmcorAc ac(0);
  ac.begin();

  ac.setMode(kAmcorAuto);
  EXPECT_EQ(kAmcorAuto, ac.getMode());

  ac.setMode(kAmcorCool);
  EXPECT_EQ(kAmcorCool, ac.getMode());

  ac.setMode(kAmcorHeat);
  EXPECT_EQ(kAmcorHeat, ac.getMode());

  ac.setMode(kAmcorDry);
  EXPECT_EQ(kAmcorDry, ac.getMode());

  ac.setMode(kAmcorFan);
  EXPECT_EQ(kAmcorFan, ac.getMode());

  ac.setMode(kAmcorAuto + 1);
  EXPECT_EQ(kAmcorAuto, ac.getMode());

  ac.setMode(255);
  EXPECT_EQ(kAmcorAuto, ac.getMode());
}

TEST(TestAmcorAcClass, FanSpeed) {
  IRAmcorAc ac(0);
  ac.begin();

  ac.setFan(0);
  EXPECT_EQ(kAmcorFanAuto, ac.getFan());

  ac.setFan(255);
  EXPECT_EQ(kAmcorFanAuto, ac.getFan());

  ac.setFan(kAmcorFanMax);
  EXPECT_EQ(kAmcorFanMax, ac.getFan());

  ac.setFan(kAmcorFanMax + 1);
  EXPECT_EQ(kAmcorFanAuto, ac.getFan());

  ac.setFan(kAmcorFanMax - 1);
  EXPECT_EQ(kAmcorFanMax - 1, ac.getFan());

  ac.setFan(1);
  EXPECT_EQ(1, ac.getFan());

  ac.setFan(1);
  EXPECT_EQ(1, ac.getFan());

  ac.setFan(3);
  EXPECT_EQ(3, ac.getFan());
}

TEST(TestAmcorAcClass, Checksums) {
  uint8_t state[kAmcorStateLength] = {
      0x01, 0x41, 0x30, 0x00, 0x00, 0x30, 0x00, 0x0C};

  ASSERT_EQ(0x0C, IRAmcorAc::calcChecksum(state));
  EXPECT_TRUE(IRAmcorAc::validChecksum(state));
  // Change the array so the checksum is invalid.
  state[0] ^= 0xFF;
  EXPECT_FALSE(IRAmcorAc::validChecksum(state));
  // Restore the previous change, and change another byte.
  state[0] ^= 0xFF;
  state[4] ^= 0xFF;
  EXPECT_FALSE(IRAmcorAc::validChecksum(state));
  state[4] ^= 0xFF;
  EXPECT_TRUE(IRAmcorAc::validChecksum(state));

  // Additional known good states.
  uint8_t knownGood1[kAmcorStateLength] = {
      0x01, 0x11, 0x3E, 0x00, 0x00, 0x30, 0x00, 0x17};
  EXPECT_TRUE(IRAmcorAc::validChecksum(knownGood1));
  ASSERT_EQ(0x17, IRAmcorAc::calcChecksum(knownGood1));
  uint8_t knownGood2[kAmcorStateLength] = {
      0x01, 0x22, 0x26, 0x00, 0x00, 0x30, 0x00, 0x10};
  EXPECT_TRUE(IRAmcorAc::validChecksum(knownGood2));
  ASSERT_EQ(0x10, IRAmcorAc::calcChecksum(knownGood2));
  uint8_t knownGood3[kAmcorStateLength] = {
      0x01, 0x41, 0x24, 0x00, 0x00, 0xC0, 0x00, 0x18};
  EXPECT_TRUE(IRAmcorAc::validChecksum(knownGood3));
  ASSERT_EQ(0x18, IRAmcorAc::calcChecksum(knownGood3));

  // For a recalculation.
  uint8_t knownBad[kAmcorStateLength] = {
      // Same as knownGood3 except for the checksum.
      0x01, 0x41, 0x24, 0x00, 0x00, 0xC0, 0x00, 0x00};
  EXPECT_FALSE(IRAmcorAc::validChecksum(knownBad));
  IRAmcorAc ac(0);
  ac.setRaw(knownBad);
  EXPECT_STATE_EQ(knownGood3, ac.getRaw(), kAmcorBits);
}

TEST(TestAmcorAcClass, Max) {
  IRAmcorAc ac(0);
  ac.begin();

  ac.setMode(kAmcorCool);
  ac.setMax(true);
  EXPECT_EQ(kAmcorCool, ac.getMode());
  EXPECT_EQ(kAmcorMinTemp, ac.getTemp());
  EXPECT_TRUE(ac.getMax());
  ac.setMax(false);
  EXPECT_EQ(kAmcorCool, ac.getMode());
  EXPECT_EQ(kAmcorMinTemp, ac.getTemp());
  EXPECT_FALSE(ac.getMax());

  ac.setMode(kAmcorHeat);
  ac.setMax(true);
  EXPECT_EQ(kAmcorHeat, ac.getMode());
  EXPECT_EQ(kAmcorMaxTemp, ac.getTemp());
  EXPECT_TRUE(ac.getMax());
  ac.setMax(false);
  EXPECT_EQ(kAmcorHeat, ac.getMode());
  EXPECT_EQ(kAmcorMaxTemp, ac.getTemp());
  EXPECT_FALSE(ac.getMax());

  ac.setMode(kAmcorAuto);
  ac.setTemp(25);
  ac.setMax(true);
  EXPECT_EQ(kAmcorAuto, ac.getMode());
  EXPECT_EQ(25, ac.getTemp());
  EXPECT_FALSE(ac.getMax());

  // Test known real data.
  uint8_t lo[kAmcorStateLength] = {
      0x01, 0x41, 0x18, 0x00, 0x00, 0x30, 0x03, 0x15};
  uint8_t hi[kAmcorStateLength] = {
      0x01, 0x12, 0x40, 0x00, 0x00, 0x30, 0x03, 0x0E};
  ac.setRaw(lo);
  EXPECT_EQ("Power: On, Mode: 1 (Cool), Fan: 4 (Auto), Temp: 12C, Max: On",
            ac.toString());
  ac.setRaw(hi);
  EXPECT_EQ("Power: On, Mode: 2 (Heat), Fan: 1 (Low), Temp: 32C, Max: On",
            ac.toString());
}
