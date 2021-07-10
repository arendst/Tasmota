// Copyright 2021 David Conran (crankyoldgit)

#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for decodeTeknopoint().

TEST(TestDecodeTeknopoint, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  // "On"
  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1486#issue-904276382
  const uint16_t rawData[227] = {
      3614, 1610, 474, 1210, 390, 1294, 454, 550, 394, 606, 474, 530, 474, 1186,
      394, 634, 390, 638, 450, 1210, 366, 1318, 450, 554, 394, 1270, 470, 554,
      450, 530, 390, 1318, 450, 1214, 470, 530, 474, 1210, 370, 1294, 474, 550,
      450, 554, 470, 1214, 450, 550, 394, 614, 466, 1218, 450, 550, 370, 634,
      474, 530, 474, 550, 450, 550, 450, 554, 366, 634, 502, 554, 446, 554, 450,
      550, 450, 554, 390, 614, 474, 526, 478, 526, 474, 550, 450, 534, 406, 614,
      390, 1294, 394, 610, 390, 614, 470, 1214, 366, 638, 470, 554, 450, 1214,
      366, 1318, 450, 550, 454, 550, 474, 530, 470, 530, 474, 550, 454, 550,
      450, 1238, 450, 1210, 470, 1214, 454, 1210, 474, 550, 450, 554, 394, 582,
      394, 634, 474, 526, 478, 550, 450, 526, 474, 1214, 474, 550, 454, 550,
      394, 606, 370, 634, 474, 526, 478, 550, 450, 550, 394, 610, 366, 638, 414,
      586, 478, 546, 454, 550, 422, 606, 390, 610, 474, 554, 450, 550, 454, 550,
      450, 530, 410, 614, 478, 526, 474, 550, 450, 550, 454, 550, 390, 614, 474,
      526, 474, 550, 454, 550, 450, 554, 366, 634, 474, 526, 478, 550, 450, 550,
      454, 550, 366, 638, 474, 526, 474, 550, 454, 1210, 414, 1270, 394, 610,
      390, 614, 470, 1214, 366, 638, 470, 1190, 474, 554, 390};
  const uint8_t expectedState[kTeknopointStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03,
      0x0F, 0x08, 0x00, 0x00, 0x00, 0x00, 0x53};
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 227, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::TEKNOPOINT, irsend.capture.decode_type);
  ASSERT_EQ(kTeknopointBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "",
      IRAcUtils::resultAcToString(&irsend.capture));
}

TEST(TestDecodeTeknopoint, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  irsend.reset();

  //  "On"
  const uint8_t expectedState[kTeknopointStateLength] = {
      0x23, 0xCB, 0x26, 0x01, 0x00, 0x24, 0x03,
      0x0F, 0x08, 0x00, 0x00, 0x00, 0x00, 0x53};

  irsend.sendTeknopoint(expectedState);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::TEKNOPOINT, irsend.capture.decode_type);
  ASSERT_EQ(kTeknopointBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "",
      IRAcUtils::resultAcToString(&irsend.capture));

  EXPECT_EQ(
      "f38000d50"
      "m3600s1600"
      "m477s1200m477s1200m477s530m477s530m477s530m477s1200m477s530m477s530"
      "m477s1200m477s1200m477s530m477s1200m477s530m477s530m477s1200m477s1200"
      "m477s530m477s1200m477s1200m477s530m477s530m477s1200m477s530m477s530"
      "m477s1200m477s530m477s530m477s530m477s530m477s530m477s530m477s530"
      "m477s530m477s530m477s530m477s530m477s530m477s530m477s530m477s530"
      "m477s530m477s530m477s1200m477s530m477s530m477s1200m477s530m477s530"
      "m477s1200m477s1200m477s530m477s530m477s530m477s530m477s530m477s530"
      "m477s1200m477s1200m477s1200m477s1200m477s530m477s530m477s530m477s530"
      "m477s530m477s530m477s530m477s1200m477s530m477s530m477s530m477s530"
      "m477s530m477s530m477s530m477s530m477s530m477s530m477s530m477s530"
      "m477s530m477s530m477s530m477s530m477s530m477s530m477s530m477s530"
      "m477s530m477s530m477s530m477s530m477s530m477s530m477s530m477s530"
      "m477s530m477s530m477s530m477s530m477s530m477s530m477s530m477s530"
      "m477s1200m477s1200m477s530m477s530m477s1200m477s530m477s1200m477s530"
      "m477s100000",
    irsend.outputStr());
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("TEKNOPOINT", typeToString(decode_type_t::TEKNOPOINT));
  ASSERT_EQ(decode_type_t::TEKNOPOINT, strToDecodeType("TEKNOPOINT"));
  ASSERT_TRUE(hasACState(decode_type_t::TEKNOPOINT));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::TEKNOPOINT));
  ASSERT_EQ(kTeknopointBits, IRsend::defaultBits(decode_type_t::TEKNOPOINT));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::TEKNOPOINT));
}
