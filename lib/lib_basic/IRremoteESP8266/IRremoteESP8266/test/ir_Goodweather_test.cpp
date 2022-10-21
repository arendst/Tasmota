// Copyright 2019 David Conran

#include "ir_Goodweather.h"
#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

TEST(TestIRUtils, Goodweather) {
  ASSERT_EQ("GOODWEATHER", typeToString(decode_type_t::GOODWEATHER));
  ASSERT_EQ(decode_type_t::GOODWEATHER, strToDecodeType("GOODWEATHER"));
  ASSERT_FALSE(hasACState(decode_type_t::GOODWEATHER));
}

// Tests for sendGoodweather().

// Test sending typical data only.
TEST(TestSendGoodweather, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();

  irsend.reset();
  irsend.sendGoodweather(0x0);
  EXPECT_EQ(
      "f38000d50"
      "m6820s6820"
      "m580s1860m580s1860m580s1860m580s1860m580s1860m580s1860m580s1860m580s1860"
      "m580s580m580s580m580s580m580s580m580s580m580s580m580s580m580s580"
      "m580s1860m580s1860m580s1860m580s1860m580s1860m580s1860m580s1860m580s1860"
      "m580s580m580s580m580s580m580s580m580s580m580s580m580s580m580s580"
      "m580s1860m580s1860m580s1860m580s1860m580s1860m580s1860m580s1860m580s1860"
      "m580s580m580s580m580s580m580s580m580s580m580s580m580s580m580s580"
      "m580s1860m580s1860m580s1860m580s1860m580s1860m580s1860m580s1860m580s1860"
      "m580s580m580s580m580s580m580s580m580s580m580s580m580s580m580s580"
      "m580s1860m580s1860m580s1860m580s1860m580s1860m580s1860m580s1860m580s1860"
      "m580s580m580s580m580s580m580s580m580s580m580s580m580s580m580s580"
      "m580s1860m580s1860m580s1860m580s1860m580s1860m580s1860m580s1860m580s1860"
      "m580s580m580s580m580s580m580s580m580s580m580s580m580s580m580s580"
      "m580s6820m580s100000",
      irsend.outputStr());

  irsend.reset();
}

// Tests for decodeGoodweather().

// Decode normal Goodweather messages.
TEST(TestDecodeGoodweather, SyntheticDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Normal (made-up value) Goodweather 48-bit message.
  irsend.reset();
  irsend.sendGoodweather(0x1234567890AB);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::GOODWEATHER, irsend.capture.decode_type);
  EXPECT_EQ(kGoodweatherBits, irsend.capture.bits);
  EXPECT_EQ(0x1234567890AB, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
  // Normal (Real) Goodweather 48-bit message.
  irsend.reset();
  irsend.sendGoodweather(0xD5276A030000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::GOODWEATHER, irsend.capture.decode_type);
  EXPECT_EQ(kGoodweatherBits, irsend.capture.bits);
  EXPECT_EQ(0xD5276A030000, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode a real example of a Goodweather message.
// https://github.com/crankyoldgit/IRremoteESP8266/issues/697#issuecomment-490209819
TEST(TestDecodeGoodweather, RealExampleDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  IRGoodweatherAc ac(0);
  irsend.begin();
  ac.begin();

  irsend.reset();
  // Raw Goodweather 48-bit message.
  uint16_t rawData_4624AB[197] = {
      6828, 6828, 732, 1780, 652, 1830, 652, 1806, 678, 1830, 652, 1806, 678,
      1830, 652, 1830, 652, 1834, 706, 518, 734, 508, 734, 514, 734, 510, 732,
      510, 732, 510, 732, 510, 732, 514, 732, 1776, 706, 1780, 628, 1854, 628,
      1832, 654, 1832, 654, 1856, 628, 1832, 634, 1876, 680, 536, 708, 536, 708,
      536, 706, 538, 706, 538, 706, 538, 706, 536, 680, 564, 680, 1828, 708,
      1758, 680, 1804, 680, 1828, 708, 1778, 732, 1754, 732, 1754, 732, 1756,
      732, 490, 658, 586, 658, 586, 658, 586, 658, 586, 658, 584, 658, 586, 658,
      586, 660, 1850, 704, 520, 658, 1828, 658, 1826, 658, 1826, 658, 586, 660,
      584, 684, 1826, 730, 490, 686, 1824, 660, 560, 710, 532, 710, 534, 712,
      1776, 712, 1774, 686, 560, 712, 1774, 712, 1798, 730, 492, 712, 1798, 684,
      1798, 678, 568, 730, 1756, 686, 1796, 686, 532, 712, 532, 712, 1796, 728,
      494, 712, 532, 738, 1772, 730, 492, 712, 532, 738, 506, 738, 1772, 660,
      582, 728, 1736, 712, 558, 710, 1750, 710, 558, 710, 510, 738, 1748, 738,
      508, 736, 1772, 684, 534, 736, 1772, 704, 518, 738, 1772, 660, 1824, 678,
      6770, 684};  // COOLIX 4624AB
  irsend.sendRaw(rawData_4624AB, 197, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::GOODWEATHER, irsend.capture.decode_type);
  EXPECT_EQ(kGoodweatherBits, irsend.capture.bits);
  EXPECT_EQ(0xD52462000000, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
  ac.setRaw(irsend.capture.value);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 20C, Fan: 3 (Low), "
      "Turbo: -, Light: -, Sleep: -, Swing: 0 (Fast), Command: 0 (Power)",
      ac.toString());

uint16_t rawData_FAD2BE31[197] = {
    6142, 7348, 570, 1612, 638, 1562, 620, 1584, 670, 1538, 566, 1638, 564,
    1610, 618, 1582, 642, 1542, 638, 498, 622, 518, 618, 496, 622, 518, 596,
    522, 596, 542, 618, 498, 618, 520, 594, 1590, 614, 1586, 618, 1588, 640,
    1592, 538, 1614, 612, 1584, 620, 1584, 616, 1592, 564, 546, 596, 540, 596,
    520, 620, 520, 594, 524, 618, 522, 650, 466, 616, 522, 670, 1532, 618, 1568,
    590, 1610, 618, 1612, 640, 1530, 594, 1586, 618, 1616, 590, 1586, 640, 472,
    618, 520, 672, 446, 618, 520, 646, 474, 616, 520, 622, 500, 614, 518, 624,
    1612, 560, 1616, 590, 1584, 620, 520, 646, 1540, 612, 518, 622, 516, 596,
    1586, 618, 518, 622, 498, 616, 520, 622, 1582, 616, 498, 620, 1582, 622,
    1586, 586, 528, 616, 1582, 622, 498, 616, 518, 624, 1582, 614, 1592, 568,
    544, 620, 1580, 648, 1542, 610, 520, 622, 1586, 666, 1536, 592, 518, 600,
    542, 594, 1592, 590, 544, 620, 498, 616, 518, 622, 1580, 620, 496, 620,
    1586, 618, 502, 610, 1584, 620, 518, 672, 446, 620, 1612, 592, 504, 608,
    1586, 618, 518, 646, 1540, 612, 520, 600, 1604, 622, 1582, 596, 7382, 566};
    // UNKNOWN FAD2BE31

  irsend.reset();
  irsend.sendRaw(rawData_FAD2BE31, 197, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::GOODWEATHER, irsend.capture.decode_type);
  EXPECT_EQ(kGoodweatherBits, irsend.capture.bits);
  EXPECT_EQ(0xD52668000000, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
  ac.setRaw(irsend.capture.value);
  EXPECT_EQ(
      "Power: Off, Mode: 1 (Cool), Temp: 22C, Fan: 3 (Low), Turbo: -, "
      "Light: -, Sleep: -, Swing: 2 (Off), Command: 0 (Power)",
      ac.toString());

  uint16_t rawData_5453D3AD[197] = {
      6190, 7298, 668, 1542, 614, 1590, 590, 1582, 620, 1584, 566, 1624, 632,
      1592, 616, 1588, 638, 1538, 594, 520, 620, 520, 594, 522, 620, 520, 586,
      530, 618, 520, 640, 480, 616, 520, 642, 1544, 612, 1588, 622, 1576, 668,
      1540, 564, 1640, 592, 1582, 646, 1558, 670, 1536, 594, 518, 622, 520, 594,
      522, 620, 520, 566, 552, 618, 520, 614, 504, 618, 518, 666, 1520, 610,
      1586, 618, 1612, 636, 1568, 564, 1590, 614, 1584, 620, 1582, 666, 1542,
      614, 526, 590, 520, 596, 520, 622, 520, 566, 550, 620, 520, 588, 530, 618,
      520, 668, 1536, 594, 520, 646, 1558, 668, 452, 616, 1584, 642, 498, 566,
      550, 618, 1582, 668, 454, 612, 1582, 646, 496, 594, 1614, 666, 450, 662,
      1536, 584, 1600, 612, 520, 642, 1590, 588, 502, 616, 520, 588, 1600, 612,
      1586, 616, 520, 612, 1574, 610, 1584, 644, 496, 564, 1620, 636, 1562, 640,
      524, 560, 530, 616, 1582, 644, 498, 620, 494, 670, 472, 622, 1558, 616,
      520, 642, 1564, 594, 518, 646, 1558, 668, 454, 638, 494, 668, 1538, 616,
      498, 642, 1558, 670, 454, 636, 1560, 642, 496, 614, 1592, 616, 1584, 620,
      7350, 668};  // UNKNOWN 5453D3AD

  irsend.reset();
  irsend.sendRaw(rawData_5453D3AD, 197, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::GOODWEATHER, irsend.capture.decode_type);
  EXPECT_EQ(kGoodweatherBits, irsend.capture.bits);
  EXPECT_EQ(0xD5266A000000, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
  ac.setRaw(irsend.capture.value);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 22C, Fan: 3 (Low), Turbo: -, "
      "Light: -, Sleep: -, Swing: 2 (Off), Command: 0 (Power)",
      ac.toString());

  uint16_t rawData_B2354FBB[197] = {
      6192, 7298, 592, 1616, 618, 1584, 620, 1558, 668, 1520, 636, 1562, 642,
      1584, 590, 1614, 542, 1634, 622, 494, 668, 454, 638, 494, 670, 454, 638,
      492, 646, 480, 636, 494, 672, 470, 622, 1560, 642, 1556, 672, 1534, 614,
      1572, 636, 1584, 622, 1582, 644, 1534, 596, 1586, 642, 494, 666, 454, 640,
      494, 668, 452, 640, 494, 668, 454, 638, 494, 670, 470, 620, 1562, 666,
      470, 644, 1546, 634, 1584, 618, 1584, 644, 1534, 640, 1548, 636, 1560,
      644, 520, 542, 1618, 638, 494, 670, 454, 636, 496, 670, 454, 634, 494,
      672, 470, 620, 1564, 640, 496, 642, 1562, 616, 520, 622, 1558, 668, 450,
      640, 494, 694, 1536, 566, 524, 644, 1558, 666, 456, 638, 1558, 644, 520,
      572, 1588, 638, 1558, 644, 494, 590, 1596, 638, 1584, 620, 1584, 644, 454,
      638, 1556, 672, 472, 620, 1562, 640, 1558, 646, 494, 644, 470, 646, 496,
      566, 1618, 638, 494, 668, 1534, 646, 468, 674, 468, 568, 550, 670, 1530,
      670, 454, 638, 1560, 644, 494, 622, 1582, 620, 494, 646, 496, 620, 1560,
      644, 494, 668, 1522, 610, 518, 674, 1532, 614, 504, 640, 1584, 642, 1562,
      616, 7332, 594};  // UNKNOWN B2354FBB

  irsend.reset();
  irsend.sendRaw(rawData_B2354FBB, 197, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::GOODWEATHER, irsend.capture.decode_type);
  EXPECT_EQ(kGoodweatherBits, irsend.capture.bits);
  EXPECT_EQ(0xD5286A020000, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
  ac.setRaw(irsend.capture.value);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 24C, Fan: 3 (Low), Turbo: -, "
      "Light: -, Sleep: -, Swing: 2 (Off), Command: 2 (Temp Up)",
      ac.toString());

  uint16_t rawData_71DD9105[197] = {
      6190, 7296, 696, 1496, 634, 1562, 642, 1582, 640, 1564, 564, 1598, 638,
      1558, 646, 1560, 588, 1616, 618, 520, 620, 494, 622, 494, 646, 494, 620,
      496, 644, 494, 590, 528, 642, 494, 642, 1544, 638, 1584, 618, 1564, 804,
      1394, 620, 1564, 640, 1558, 644, 1586, 562, 1616, 620, 492, 672, 470, 622,
      494, 646, 494, 622, 494, 646, 494, 620, 498, 644, 492, 596, 520, 644, 494,
      592, 1596, 612, 1584, 642, 1560, 614, 1612, 594, 1584, 620, 1558, 646,
      1556, 644, 1562, 618, 520, 620, 494, 620, 494, 646, 494, 568, 548, 644,
      494, 616, 1570, 638, 494, 670, 1534, 568, 550, 646, 1556, 616, 526, 618,
      492, 672, 1532, 568, 550, 646, 1558, 640, 500, 618, 1560, 668, 470, 642,
      1548, 658, 1536, 642, 520, 588, 504, 644, 492, 644, 478, 642, 1582, 618,
      1586, 590, 506, 640, 1556, 646, 1584, 562, 1616, 620, 1558, 646, 1556,
      670, 454, 638, 492, 648, 1558, 642, 478, 644, 492, 590, 530, 858, 1342,
      642, 496, 618, 1564, 642, 492, 642, 1548, 636, 492, 648, 494, 622, 1562,
      642, 492, 644, 1562, 618, 520, 620, 1558, 644, 476, 640, 1558, 646, 1558,
      612, 7382, 594};  // UNKNOWN 71DD9105

  irsend.reset();
  irsend.sendRaw(rawData_71DD9105, 197, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::GOODWEATHER, irsend.capture.decode_type);
  EXPECT_EQ(kGoodweatherBits, irsend.capture.bits);
  EXPECT_EQ(0xD5276A030000, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
  ac.setRaw(irsend.capture.value);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 23C, Fan: 3 (Low), "
      "Turbo: -, Light: -, Sleep: -, Swing: 2 (Off), Command: 3 (Temp Down)",
      ac.toString());

  uint16_t rawData_C4F9E573[199] = {
      6186, 7296, 648, 1558, 670, 1542, 612, 1584, 618, 1560, 668, 1534, 622,
      1566, 638, 1558, 646, 1584, 590, 506, 640, 492, 642, 480, 640, 494, 644,
      478, 640, 494, 668, 454, 614, 516, 648, 1560, 566, 1638, 618, 1584, 620,
      1556, 672, 1534, 620, 1564, 640, 1584, 618, 1586, 564, 528, 670, 468, 640,
      478, 642, 494, 644, 478, 640, 492, 670, 454, 638, 494, 670, 1560, 542,
      1636, 644, 468, 670, 1534, 620, 1586, 618, 1558, 646, 1556, 670, 1534,
      622, 492, 648, 494, 620, 1562, 642, 496, 642, 476, 642, 494, 696, 426,
      642, 492, 646, 1558, 568, 548, 644, 494, 642, 1564, 618, 1584, 620, 494,
      568, 548, 644, 1558, 644, 480, 636, 1584, 620, 1584, 644, 456, 634, 494,
      672, 1560, 540, 1638, 618, 494, 728, 1476, 592, 524, 646, 492, 616, 1572,
      638, 1560, 644, 492, 668, 1520, 638, 1562, 642, 494, 588, 1598, 638, 1560,
      642, 494, 622, 498, 642, 1556, 646, 478, 638, 492, 646, 494, 620, 1584,
      618, 522, 616, 1546, 612, 516, 648, 1556, 644, 476, 668, 468, 670, 1534,
      620, 494, 648, 1556, 670, 452, 640, 1558, 644, 496, 646, 1536, 616, 1582,
      646, 7326, 616, 41598, 230};  // UNKNOWN C4F9E573

  irsend.reset();
  irsend.sendRaw(rawData_C4F9E573, 199, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::GOODWEATHER, irsend.capture.decode_type);
  EXPECT_EQ(kGoodweatherBits, irsend.capture.bits);
  EXPECT_EQ(0xD52666040000, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_EQ(
      "Power: On, Mode: 1 (Cool), Temp: 22C, Fan: 3 (Low), Turbo: -, Light: -, "
      "Sleep: -, Swing: 1 (Slow), Command: 4 (Swing)",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}


TEST(TestGoodweatherAcClass, toCommon) {
  IRGoodweatherAc ac(0);
  ac.setPower(true);
  ac.setMode(kGoodweatherCool);
  ac.setTemp(20);
  ac.setFan(kGoodweatherFanHigh);
  ac.setSwing(kGoodweatherSwingFast);
  ac.setTurbo(true);
  ac.setLight(true);
  ac.setSleep(true);
  // Now test it.
  ASSERT_EQ(decode_type_t::GOODWEATHER, ac.toCommon().protocol);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(stdAc::swingv_t::kAuto, ac.toCommon().swingv);
  ASSERT_TRUE(ac.toCommon().turbo);
  ASSERT_TRUE(ac.toCommon().light);
  ASSERT_EQ(0, ac.toCommon().sleep);
  // Unsupported.
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}


TEST(TestGoodweatherAcClass, Temperature) {
  IRGoodweatherAc ac(0);

  ac.setTemp(kGoodweatherTempMin);
  EXPECT_EQ(kGoodweatherTempMin, ac.getTemp());

  ac.setTemp(kGoodweatherTempMin + 1);
  EXPECT_EQ(kGoodweatherTempMin + 1, ac.getTemp());

  ac.setTemp(kGoodweatherTempMax);
  EXPECT_EQ(kGoodweatherTempMax, ac.getTemp());

  ac.setTemp(kGoodweatherTempMin - 1);
  EXPECT_EQ(kGoodweatherTempMin, ac.getTemp());

  ac.setTemp(kGoodweatherTempMax + 1);
  EXPECT_EQ(kGoodweatherTempMax, ac.getTemp());

  ac.setTemp(23);
  EXPECT_EQ(23, ac.getTemp());

  ac.setTemp(27);
  EXPECT_EQ(27, ac.getTemp());

  ac.setTemp(22);
  EXPECT_EQ(22, ac.getTemp());

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());

  ac.setTemp(0);
  EXPECT_EQ(kGoodweatherTempMin, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kGoodweatherTempMax, ac.getTemp());
}

TEST(TestGoodweatherAcClass, OperatingMode) {
  IRGoodweatherAc ac(0);
  ac.begin();

  ac.setMode(kGoodweatherAuto);
  EXPECT_EQ(kGoodweatherAuto, ac.getMode());

  ac.setMode(kGoodweatherCool);
  EXPECT_EQ(kGoodweatherCool, ac.getMode());

  ac.setMode(kGoodweatherHeat);
  EXPECT_EQ(kGoodweatherHeat, ac.getMode());

  ac.setMode(kGoodweatherFan);  // Should set fan speed to High.
  EXPECT_EQ(kGoodweatherFan, ac.getMode());

  ac.setMode(kGoodweatherDry);
  EXPECT_EQ(kGoodweatherDry, ac.getMode());

  ac.setMode(kGoodweatherHeat + 1);
  EXPECT_EQ(kGoodweatherAuto, ac.getMode());

  ac.setMode(kGoodweatherCool);
  EXPECT_EQ(kGoodweatherCool, ac.getMode());

  ac.setMode(kGoodweatherAuto - 1);
  EXPECT_EQ(kGoodweatherAuto, ac.getMode());

  ac.setMode(kGoodweatherCool);
  ac.setMode(255);
  EXPECT_EQ(kGoodweatherAuto, ac.getMode());

  ac.setMode(kGoodweatherCool);
  ac.setMode(0);
  EXPECT_EQ(kGoodweatherAuto, ac.getMode());
}

TEST(TestGoodweatherAcClass, Power) {
  IRGoodweatherAc ac(0);
  ac.begin();

  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());

  ac.setPower(false);
  EXPECT_EQ(false, ac.getPower());

  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());

  ac.off();
  EXPECT_EQ(false, ac.getPower());

  ac.on();
  EXPECT_TRUE(ac.getPower());
}

TEST(TestGoodweatherAcClass, Light) {
  IRGoodweatherAc ac(0);
  ac.begin();

  ac.setLight(true);
  EXPECT_TRUE(ac.getLight());
  ac.setLight(false);
  EXPECT_EQ(false, ac.getLight());
  ac.setLight(true);
  EXPECT_TRUE(ac.getLight());
}

TEST(TestGoodweatherAcClass, Turbo) {
  IRGoodweatherAc ac(0);
  ac.begin();

  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());
  ac.setTurbo(false);
  EXPECT_EQ(false, ac.getTurbo());
  ac.setTurbo(true);
  EXPECT_TRUE(ac.getTurbo());
}

TEST(TestGoodweatherAcClass, Sleep) {
  IRGoodweatherAc ac(0);
  ac.begin();

  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
  ac.setSleep(false);
  EXPECT_EQ(false, ac.getSleep());
  ac.setSleep(true);
  EXPECT_TRUE(ac.getSleep());
}

TEST(TestGoodweatherAcClass, FanSpeed) {
  IRGoodweatherAc ac(0);
  ac.begin();

  // Unexpected value should default to Auto.
  ac.setFan(255);
  EXPECT_EQ(kGoodweatherFanAuto, ac.getFan());

  ac.setFan(kGoodweatherFanLow);
  EXPECT_EQ(kGoodweatherFanLow, ac.getFan());
  ac.setFan(kGoodweatherFanMed);
  EXPECT_EQ(kGoodweatherFanMed, ac.getFan());
  ac.setFan(kGoodweatherFanHigh);
  EXPECT_EQ(kGoodweatherFanHigh, ac.getFan());
  ac.setFan(kGoodweatherFanAuto);
  EXPECT_EQ(kGoodweatherFanAuto, ac.getFan());

  // Beyond Low should default to Auto.
  ac.setFan(kGoodweatherFanLow + 1);
  EXPECT_EQ(kGoodweatherFanAuto, ac.getFan());
}


TEST(TestGoodweatherAcClass, SwingSpeed) {
  IRGoodweatherAc ac(0);
  ac.begin();

  // Unexpected value should default to Off.
  ac.setSwing(255);
  EXPECT_EQ(kGoodweatherSwingOff, ac.getSwing());

  ac.setSwing(kGoodweatherSwingSlow);
  EXPECT_EQ(kGoodweatherSwingSlow, ac.getSwing());
  ac.setSwing(kGoodweatherSwingOff);
  EXPECT_EQ(kGoodweatherSwingOff, ac.getSwing());
  ac.setSwing(kGoodweatherSwingFast);
  EXPECT_EQ(kGoodweatherSwingFast, ac.getSwing());

  // Beyond Off should default to Off.
  ac.setSwing(kGoodweatherSwingOff + 1);
  EXPECT_EQ(kGoodweatherSwingOff, ac.getSwing());
}

TEST(TestGoodweatherAcClass, Command) {
  IRGoodweatherAc ac(0);
  ac.begin();

  ac.setCommand(kGoodweatherCmdMode);
  EXPECT_EQ(kGoodweatherCmdMode, ac.getCommand());
  // Unexpected value should not change anything.
  ac.setCommand(255);
  EXPECT_EQ(kGoodweatherCmdMode, ac.getCommand());

  ac.setCommand(kGoodweatherCmdPower);
  EXPECT_EQ(kGoodweatherCmdPower, ac.getCommand());
  ac.setCommand(kGoodweatherCmdLight);
  EXPECT_EQ(kGoodweatherCmdLight, ac.getCommand());

  // Beyond Light should be ignored.
  ac.setCommand(kGoodweatherCmdLight + 1);
  EXPECT_EQ(kGoodweatherCmdLight, ac.getCommand());
}
