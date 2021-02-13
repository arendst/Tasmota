// Copyright 2021 David Conran

#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "IRutils.h"
#include "gtest/gtest.h"

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("ECOCLIM", typeToString(decode_type_t::ECOCLIM));
  ASSERT_EQ(decode_type_t::ECOCLIM, strToDecodeType("ECOCLIM"));
  ASSERT_FALSE(hasACState(decode_type_t::ECOCLIM));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::ECOCLIM));
  ASSERT_EQ(kEcoclimBits, IRsend::defaultBits(decode_type_t::ECOCLIM));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::ECOCLIM));
}

// Test sending typical data only.
TEST(TestSendEcoclim, SendDataOnly) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendEcoclim(0x110673AEFFFF72);
  EXPECT_EQ(
      "f38000d50"
      "m5730s1935"
      "m440s637m440s637m440s637m440s1739m440s637m440s637m440s637m440s1739"
      "m440s637m440s637m440s637m440s637m440s637m440s1739m440s1739m440s637"
      "m440s637m440s1739m440s1739m440s1739m440s637m440s637m440s1739m440s1739"
      "m440s1739m440s637m440s1739m440s637m440s1739m440s1739m440s1739m440s637"
      "m440s1739m440s1739m440s1739m440s1739m440s1739m440s1739m440s1739m440s1739"
      "m440s1739m440s1739m440s1739m440s1739m440s1739m440s1739m440s1739m440s1739"
      "m440s637m440s1739m440s1739m440s1739m440s637m440s637m440s1739m440s637"
      "m5730s1935"
      "m440s637m440s637m440s637m440s1739m440s637m440s637m440s637m440s1739"
      "m440s637m440s637m440s637m440s637m440s637m440s1739m440s1739m440s637"
      "m440s637m440s1739m440s1739m440s1739m440s637m440s637m440s1739m440s1739"
      "m440s1739m440s637m440s1739m440s637m440s1739m440s1739m440s1739m440s637"
      "m440s1739m440s1739m440s1739m440s1739m440s1739m440s1739m440s1739m440s1739"
      "m440s1739m440s1739m440s1739m440s1739m440s1739m440s1739m440s1739m440s1739"
      "m440s637m440s1739m440s1739m440s1739m440s637m440s637m440s1739m440s637"
      "m5730s1935"
      "m440s637m440s637m440s637m440s1739m440s637m440s637m440s637m440s1739"
      "m440s637m440s637m440s637m440s637m440s637m440s1739m440s1739m440s637"
      "m440s637m440s1739m440s1739m440s1739m440s637m440s637m440s1739m440s1739"
      "m440s1739m440s637m440s1739m440s637m440s1739m440s1739m440s1739m440s637"
      "m440s1739m440s1739m440s1739m440s1739m440s1739m440s1739m440s1739m440s1739"
      "m440s1739m440s1739m440s1739m440s1739m440s1739m440s1739m440s1739m440s1739"
      "m440s637m440s1739m440s1739m440s1739m440s637m440s637m440s1739m440s637"
      "m7820s100000",
      irsend.outputStr());

  irsend.reset();
  irsend.sendEcoclim(0x93B, kEcoclimShortBits);
  EXPECT_EQ(
      "f38000d50"
      "m5730s1935"
      "m440s637m440s637m440s637m440s1739m440s637m440s637m440s1739m440s637"
      "m440s637m440s1739m440s1739m440s1739m440s637m440s1739m440s1739"
      "m5730s1935"
      "m440s637m440s637m440s637m440s1739m440s637m440s637m440s1739m440s637"
      "m440s637m440s1739m440s1739m440s1739m440s637m440s1739m440s1739"
      "m5730s1935"
      "m440s637m440s637m440s637m440s1739m440s637m440s637m440s1739m440s637"
      "m440s637m440s1739m440s1739m440s1739m440s637m440s1739m440s1739"
      "m7820s100000",
      irsend.outputStr());
}

TEST(TestDecodeEcoclim, SyntheticSelfDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);

  // Long (normal) message. i.e. 56 bits.
  irsend.begin();
  irsend.reset();
  irsend.sendEcoclim(0x110673AEFFFF72);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(ECOCLIM, irsend.capture.decode_type);
  EXPECT_EQ(kEcoclimBits, irsend.capture.bits);
  EXPECT_EQ(0x110673AEFFFF72, irsend.capture.value);

  // Short (automated) message. i.e. 15 bits.
  irsend.reset();
  irsend.sendEcoclim(0x93B, kEcoclimShortBits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(ECOCLIM, irsend.capture.decode_type);
  EXPECT_EQ(kEcoclimShortBits, irsend.capture.bits);
  EXPECT_EQ(0x93B, irsend.capture.value);
}

TEST(TestDecodeEcoclim, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  stdAc::state_t r, p;
  irsend.begin();

  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1397#issuecomment-770376241
  const uint16_t long_rawData[343] = {
      5834, 1950, 482, 580, 506, 614, 480, 612, 456, 1738, 482, 562, 532, 582,
      508, 608, 430, 1760, 456, 642, 456, 608, 484, 638, 458, 634, 456, 634,
      456, 1734, 482, 1704, 402, 690, 456, 638, 404, 1786, 458, 1730, 430, 1762,
      454, 638, 456, 636, 456, 1732, 426, 1764, 426, 1788, 400, 692, 402, 1764,
      452, 638, 430, 1760, 424, 1786, 374, 1818, 402, 690, 374, 1786, 424, 1796,
      402, 1758, 426, 1790, 376, 1782, 426, 1766, 400, 1810, 398, 1796, 400,
      1788, 428, 1734, 398, 1814, 400, 1762, 470, 1742, 400, 1786, 398, 1794,
      400, 1762, 398, 718, 400, 1792, 400, 1788, 400, 1788, 400, 694, 400, 694,
      402, 1788, 398, 664, 5720, 1944, 426, 642, 450, 696, 442, 650, 396, 1794,
      468, 602, 422, 642, 448, 696, 442, 1744, 392, 702, 392, 678, 420, 700,
      394, 700, 464, 628, 466, 1720, 464, 1726, 462, 628, 464, 630, 464, 1728,
      438, 1752, 462, 1722, 464, 636, 438, 626, 464, 1722, 490, 1698, 488, 1722,
      464, 628, 466, 1724, 466, 626, 464, 1724, 464, 1724, 462, 1732, 462, 626,
      464, 1726, 464, 1724, 464, 1722, 464, 1732, 464, 1690, 490, 1724, 464,
      1726, 464, 1728, 464, 1728, 462, 1690, 492, 1728, 462, 1724, 464, 1726,
      464, 1728, 464, 1720, 462, 1736, 460, 604, 490, 1718, 464, 1730, 462,
      1720, 462, 630, 464, 628, 462, 1734, 462, 600, 5632, 2028, 490, 630, 464,
      632, 464, 630, 462, 1724, 462, 630, 462, 636, 462, 602, 488, 1728, 464,
      630, 462, 632, 464, 630, 462, 628, 462, 630, 464, 1730, 460, 1724, 464,
      630, 464, 630, 462, 1728, 464, 1724, 462, 1728, 462, 630, 464, 630, 462,
      1724, 464, 1728, 460, 1730, 462, 628, 460, 1732, 464, 602, 492, 1722, 464,
      1726, 460, 1726, 464, 632, 464, 1696, 488, 1728, 460, 1732, 462, 1728,
      462, 1694, 488, 1728, 462, 1724, 464, 1732, 460, 1700, 490, 1728, 462,
      1694, 488, 1730, 462, 1720, 462, 1728, 464, 1726, 462, 1726, 460, 632,
      464, 1724, 462, 1726, 460, 1730, 464, 630, 464, 632, 464, 1728, 462, 596,
      7862};  // UNKNOWN 842242BF

  const uint16_t short_rawData[97] = {
      5778, 1940, 458, 662, 432, 664, 428, 666, 430, 1756, 430, 664, 430, 664,
      430, 1756, 430, 664, 428, 668, 428, 1758, 430, 1760, 428, 1758, 430, 668,
      430, 1756, 430, 1768, 5672, 1988, 456, 610, 456, 658, 430, 670, 456, 1732,
      430, 610, 506, 636, 456, 1738, 458, 636, 482, 584, 506, 1708, 430, 1758,
      482, 1708, 456, 638, 430, 1754, 456, 1738, 5674, 1982, 430, 668, 458, 632,
      456, 638, 456, 1736, 450, 646, 480, 588, 482, 1728, 426, 666, 428, 666,
      426, 1764, 426, 1760, 454, 1744, 452, 608, 452, 1762, 424, 1766,
      7770};  // UNKNOWN C0071146

  const uint16_t long_rawData2[343] = {  // UNKNOWN 15923F6
      5832, 1928, 482, 614, 482, 610, 482, 588, 506, 1704, 482, 586, 508, 1714,
      454, 606, 508, 1682, 508, 618, 480, 1646, 534, 594, 506, 1708, 454, 1708,
      506, 582, 508, 586, 508, 1706, 456, 612, 508, 1704, 482, 614, 480, 590,
      508, 586, 506, 1678, 508, 592, 506, 1682, 506, 590, 506, 578, 508, 612,
      480, 590, 478, 646, 454, 1676, 534, 1708, 480, 1712, 482, 1706, 480, 1710,
      480, 1704, 482, 1672, 430, 1744, 534, 1712, 482, 1698, 482, 1708, 482,
      1702, 484, 1704, 484, 1712, 484, 1704, 484, 1700, 484, 1706, 484, 1710,
      484, 1698, 484, 612, 484, 610, 482, 612, 484, 610, 482, 1732, 458, 608,
      484, 1704, 482, 614, 5726, 1906, 510, 610, 458, 634, 484, 614, 480, 1706,
      484, 612, 484, 1732, 458, 610, 484, 1700, 484, 616, 482, 1708, 456, 608,
      510, 1710, 484, 1700, 482, 612, 484, 614, 480, 1696, 482, 622, 482, 1704,
      482, 620, 484, 602, 482, 610, 486, 1706, 482, 612, 484, 1706, 482, 612,
      482, 608, 484, 610, 482, 610, 482, 614, 482, 1710, 482, 1708, 482, 1700,
      480, 1714, 478, 1706, 482, 1706, 482, 1712, 480, 1702, 482, 1710, 478,
      1710, 480, 1706, 480, 1732, 456, 1716, 478, 1676, 504, 1736, 456, 1708,
      478, 1708, 482, 1714, 450, 1706, 504, 640, 454, 616, 480, 638, 454, 642,
      452, 1710, 480, 612, 478, 1710, 480, 602, 5726, 1950, 450, 642, 482, 614,
      452, 642, 478, 1730, 428, 644, 450, 1746, 478, 630, 456, 1710, 478, 644,
      452, 1734, 456, 614, 452, 1738, 480, 1706, 450, 666, 428, 642, 452, 1760,
      456, 614, 452, 1736, 454, 646, 452, 662, 454, 640, 398, 1788, 426, 670,
      456, 1712, 446, 670, 454, 638, 454, 646, 430, 664, 426, 662, 396, 1774,
      416, 1792, 392, 1798, 464, 1696, 416, 1798, 464, 1724, 392, 1800, 392,
      1790, 390, 1788, 464, 1734, 392, 1798, 462, 1734, 438, 1724, 490, 1724,
      390, 1766, 464, 1752, 390, 1802, 390, 1792, 366, 1802, 414, 702, 390, 702,
      390, 704, 394, 700, 392, 1792, 440, 654, 390, 1800, 366, 730, 7800};

  irsend.reset();
  irsend.sendRaw(long_rawData, 343, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(ECOCLIM, irsend.capture.decode_type);
  EXPECT_EQ(kEcoclimBits, irsend.capture.bits);
  EXPECT_EQ(0x110673AEFFFF72, irsend.capture.value);
  EXPECT_EQ(
      "",
      IRAcUtils::resultAcToString(&irsend.capture));
  ASSERT_FALSE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));

  irsend.reset();
  irsend.sendRaw(short_rawData, 97, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(ECOCLIM, irsend.capture.decode_type);
  EXPECT_EQ(kEcoclimShortBits, irsend.capture.bits);
  EXPECT_EQ(0x93B, irsend.capture.value);
  EXPECT_EQ(
      "",
      IRAcUtils::resultAcToString(&irsend.capture));
  ASSERT_FALSE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));

  irsend.reset();
  irsend.sendRaw(long_rawData2, 343, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(ECOCLIM, irsend.capture.decode_type);
  EXPECT_EQ(kEcoclimBits, irsend.capture.bits);
  EXPECT_EQ(0x15594507FFFF0A, irsend.capture.value);
  EXPECT_EQ(
      "",
      IRAcUtils::resultAcToString(&irsend.capture));
  ASSERT_FALSE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}
