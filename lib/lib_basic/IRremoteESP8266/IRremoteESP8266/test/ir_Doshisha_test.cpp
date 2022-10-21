// Copyright 2020 Christian Nilsson

#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for decodeDoshisha().

TEST(TestDecodeDoshisha, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  // CH2 Light Level 1
  const uint16_t rawData_1[83] = {
      3404, 1718,
      416, 1306, 416, 460,  438, 432,  412, 460, 464, 396,  488, 394,  488, 394,
      488, 398,  460, 430,  468, 398,  484, 416, 468, 396,  488, 1264, 414, 460,
      436, 1286, 524, 1210, 470, 390,  434, 456, 414, 1288, 458, 1290, 438, 434,
      412, 458,  440, 434,  438, 418,  456, 432, 466, 1270, 438, 442,  438, 430,
      414, 1316, 412, 460,  440, 432,  412, 460, 440, 1264, 458, 434, 436, 1268,
      458, 436,  438, 412,  456, 1290, 438, 442, 436, 1290,
      464};  // DOSHISHA 800B3048A5
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData_1, 83, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::DOSHISHA, irsend.capture.decode_type);
  ASSERT_EQ(kDoshishaBits, irsend.capture.bits);
  EXPECT_EQ(0x800B3048A5, irsend.capture.value);
  EXPECT_EQ(0b1, irsend.capture.address);
  EXPECT_EQ(0xA4, irsend.capture.command);

  // CH2 OFF
  const uint16_t rawData_2[83] = {
      3434, 1700,
      446, 1284, 442, 440,  442, 428, 444, 432, 440, 430,  442, 432,  472, 438,
      444, 446,  416, 430,  470, 416, 470, 438, 444, 442,  470, 1242, 440, 430,
      442, 1314, 390, 1344, 444, 440, 414, 432, 442, 1310, 412, 1318, 416, 458,
      416, 434,  438, 432,  418, 484, 416, 458, 442, 1298, 416, 466,  420, 428,
      442, 1316, 418, 432,  442, 430, 442, 434, 444, 1286, 440, 430,  444, 1288,
      444, 430,  444, 430,  442, 432, 498, 386, 498, 1242,
      474};  // DOSHISHA 800B3048A1

  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData_2, 175, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::DOSHISHA, irsend.capture.decode_type);
  ASSERT_EQ(kDoshishaBits, irsend.capture.bits);
  EXPECT_EQ(0x800B3048A1, irsend.capture.value);
  EXPECT_EQ(0b1, irsend.capture.address);
  EXPECT_EQ(0xA0, irsend.capture.command);

  // CH1 OFF
  const uint16_t rawData_4[83] = {
      3470, 1670,
      444, 1294, 470, 408,  442, 430, 444, 412, 464, 440,  498, 386,  498, 388,
      474, 418,  440, 430,  472, 414, 474, 412, 472, 414,  470, 1268, 444, 428,
      448, 1284, 444, 1294, 472, 412, 446, 428, 442, 1284, 444, 1290, 444, 430,
      446, 430,  442, 410,  490, 406, 446, 428, 472, 1270, 472, 414,  442, 410,
      460, 1292, 444, 430,  442, 430, 444, 434, 444, 1286, 442, 432,  442, 1288,
      446, 430,  442, 414,  486, 386, 516, 388, 446, 438,
      446};  // DOSHISHA 800B3048A0
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData_4, 175, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::DOSHISHA, irsend.capture.decode_type);
  ASSERT_EQ(kDoshishaBits, irsend.capture.bits);
  EXPECT_EQ(0x800B3048A0, irsend.capture.value);
  EXPECT_EQ(0b0, irsend.capture.address);
  EXPECT_EQ(0xA0, irsend.capture.command);
}

TEST(TestDecodeDoshisha, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  irsend.reset();
  irsend.sendDoshisha(0x800B3048A5);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::DOSHISHA, irsend.capture.decode_type);
  EXPECT_EQ(kDoshishaBits, irsend.capture.bits);
  EXPECT_EQ(0x800B3048A5, irsend.capture.value);
  EXPECT_EQ(0b1, irsend.capture.address);
  EXPECT_EQ(0xA4, irsend.capture.command);

  EXPECT_EQ(
      "f38000d50"
      "m3412s1722"
      "m420s1310m420s452m420s452m420s452m420s452m420s452m420s452"
      "m420s452m420s452m420s452m420s452m420s452m420s1310m420s452"
      "m420s1310m420s1310m420s452m420s452m420s1310m420s1310m420s452"
      "m420s452m420s452m420s452m420s452m420s1310m420s452m420s452"
      "m420s1310m420s452m420s452m420s452m420s1310m420s452m420s1310"
      "m420s452m420s452m420s1310m420s452m420s1310"
      "m420s100000",
      irsend.outputStr());

  irsend.reset();
  irsend.sendDoshisha(0x800B3048D0);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::DOSHISHA, irsend.capture.decode_type);
  EXPECT_EQ(kDoshishaBits, irsend.capture.bits);
  EXPECT_EQ(0x800B3048D0, irsend.capture.value);
  EXPECT_EQ(irsend.encodeDoshisha(0xD0, 0b0), irsend.capture.value);
  EXPECT_EQ(0b0, irsend.capture.address);
  EXPECT_EQ(0xD0, irsend.capture.command);
  EXPECT_EQ(
      "f38000d50"
      "m3412s1722"
      "m420s1310m420s452m420s452m420s452m420s452m420s452m420s452"
      "m420s452m420s452m420s452m420s452m420s452m420s1310m420s452"
      "m420s1310m420s1310m420s452m420s452m420s1310m420s1310m420s452"
      "m420s452m420s452m420s452m420s452m420s1310m420s452m420s452m420"
      "s1310m420s452m420s452m420s452m420s1310m420s1310m420s452m420s1310"
      "m420s452m420s452m420s452m420s452"
      "m420s100000",
      irsend.outputStr());
}

TEST(TestEncodeDoshisha, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  //  kRcz01CheckExpected
  EXPECT_EQ(0x800B304800, irsend.encodeDoshisha(0x00, 0b0));
  //  kRcz01CommandTimmer30
  EXPECT_EQ(0x800B304892, irsend.encodeDoshisha(0x92, 0b0));
  //  kRcz01CommandLevel1
  EXPECT_EQ(0x800B3048A5, irsend.encodeDoshisha(0xA4, 0b1));
  EXPECT_EQ(0x800B3048A4, irsend.encodeDoshisha(0xA4, 0b0));
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("DOSHISHA", typeToString(decode_type_t::DOSHISHA));
  ASSERT_EQ(decode_type_t::DOSHISHA, strToDecodeType("DOSHISHA"));
  ASSERT_FALSE(hasACState(decode_type_t::DOSHISHA));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::DOSHISHA));
  ASSERT_EQ(kDoshishaBits, IRsend::defaultBits(decode_type_t::DOSHISHA));
  ASSERT_EQ(kNoRepeat, IRsend::minRepeats(decode_type_t::DOSHISHA));
}
