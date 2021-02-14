// Copyright 2018 David Conran

#include "IRsend.h"
#include "IRsend_test.h"
#include "IRutils.h"
#include "gtest/gtest.h"

// Tests for sendPioneer().

// Test sending typical data only.
TEST(TestSendPioneer, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();
  irsend.sendPioneer(0);
  EXPECT_EQ(
      "f40000d33"
      "m8506s4191"
      "m568s487m568s487m568s487m568s487m568s487m568s487m568s487m568s487"
      "m568s487m568s487m568s487m568s487m568s487m568s487m568s487m568s487"
      "m568s487m568s487m568s487m568s487m568s487m568s487m568s487m568s487"
      "m568s487m568s487m568s487m568s487m568s487m568s487m568s487m568s487"
      "m568s37881"
      "m8506s4191"
      "m568s487m568s487m568s487m568s487m568s487m568s487m568s487m568s487"
      "m568s487m568s487m568s487m568s487m568s487m568s487m568s487m568s487"
      "m568s487m568s487m568s487m568s487m568s487m568s487m568s487m568s487"
      "m568s487m568s487m568s487m568s487m568s487m568s487m568s487m568s487"
      "m568s37881",
      irsend.outputStr());
  irsend.sendPioneer(0x55FF00AAAA00FF55);
  EXPECT_EQ(
      "f40000d33"
      "m8506s4191"
      "m568s487m568s1542m568s487m568s1542m568s487m568s1542m568s487m568s1542"
      "m568s1542m568s1542m568s1542m568s1542m568s1542m568s1542m568s1542m568s1542"
      "m568s487m568s487m568s487m568s487m568s487m568s487m568s487m568s487"
      "m568s1542m568s487m568s1542m568s487m568s1542m568s487m568s1542m568s487"
      "m568s25181"
      "m8506s4191"
      "m568s1542m568s487m568s1542m568s487m568s1542m568s487m568s1542m568s487"
      "m568s487m568s487m568s487m568s487m568s487m568s487m568s487m568s487"
      "m568s1542m568s1542m568s1542m568s1542m568s1542m568s1542m568s1542m568s1542"
      "m568s487m568s1542m568s487m568s1542m568s487m568s1542m568s487m568s1542"
      "m568s25181",
      irsend.outputStr());
}

// Tests for IRutils.

TEST(TestIRUtils, TypeToString) { EXPECT_EQ("PIONEER", typeToString(PIONEER)); }

// Tests for encodePioneer().

TEST(TestEncodePioneer, SimpleEncoding) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);

  // Spotify button (A556+AF20)
  // via
  // https://www.pioneerelectronics.com/StaticFiles/PUSA/Files/Home%20Custom%20Install/2015%20Pioneer%20&%20Elite%20AVR%20IR%20with%20Hex_1.xls
  EXPECT_EQ(0xA55A6A95F50A04FB, irsend.encodePioneer(0xA556, 0xAF20));

  // "Source" from
  // https://github.com/crankyoldgit/IRremoteESP8266/pull/547#issuecomment-429616582
  EXPECT_EQ(0x659A05FAF50AC53A, irsend.encodePioneer(0xA6A0, 0xAFA3));
}

// Tests for decodePioneer().

// Synthesised Normal Pioneer message.
TEST(TestDecodePioneer, SyntheticPioneerDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  irsend.sendPioneer(0x659A05FAF50AC53A);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(PIONEER, irsend.capture.decode_type);
  EXPECT_EQ(kPioneerBits, irsend.capture.bits);
  EXPECT_EQ(0x659A05FAF50AC53A, irsend.capture.value);
  EXPECT_EQ(0xA6A0, irsend.capture.address);
  EXPECT_EQ(0xAFA3, irsend.capture.command);
}

// Real long Pioneer message.
TEST(TestDecodePioneer, RealExampleLongDecodeSourceButton) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  // "Source" button.
  // https://github.com/crankyoldgit/IRremoteESP8266/pull/547#issuecomment-429616582
  uint16_t rawData[135] = {
      8552, 4184, 596, 472,  592, 1524, 594, 1524,  594,  472,  592, 472,
      598,  1520, 596, 472,  594, 1524, 592, 1524,  592,  472,  592, 472,
      596,  1520, 598, 1520, 596, 472,  592, 1524,  592,  472,  592, 476,
      592,  472,  592, 472,  592, 476,  592, 472,   592,  1524, 592, 472,
      598,  1518, 598, 1520, 596, 1520, 596, 1520,  596,  1520, 596, 1520,
      596,  472,  592, 1524, 592, 472,  598, 25282, 8552, 4182, 596, 1520,
      598,  1518, 598, 1520, 596, 1520, 596, 472,   592,  1524, 592, 472,
      598,  1520, 596, 472,  594, 472,  592, 472,   596,  472,  592, 1524,
      592,  472,  592, 1524, 596, 472,  594, 1520,  596,  1520, 598, 472,
      592,  472,  598, 472,  594, 1522, 594, 472,   592,  1524, 594, 472,
      596,  472,  594, 1524, 592, 1524, 592, 1524,  592,  472,  594, 1524,
      598,  472,  592};

  irsend.sendRaw(rawData, 135, 38000);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(PIONEER, irsend.capture.decode_type);
  EXPECT_EQ(kPioneerBits, irsend.capture.bits);
  EXPECT_EQ(0x659A05FAF50AC53A, irsend.capture.value);
  EXPECT_EQ(0xA6A0, irsend.capture.address);
  EXPECT_EQ(0xAFA3, irsend.capture.command);
}

// Synthetic Pioneer message.
// For:
// https://github.com/crankyoldgit/IRremoteESP8266/pull/547#issuecomment-430800734
TEST(TestDecodePioneer, SyntheticPioneerMessage) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  irsend.sendPioneer(0x659A857AF50A3DC2, 64, 0);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(PIONEER, irsend.capture.decode_type);
  EXPECT_EQ(kPioneerBits, irsend.capture.bits);
  EXPECT_EQ(0x659A857AF50A3DC2, irsend.capture.value);
  EXPECT_EQ(0xA6A1, irsend.capture.address);
  EXPECT_EQ(0xAFBC, irsend.capture.command);

  irsend.reset();
  irsend.sendPioneer(0x659A857AF50A3DC2, 64, 0);
  EXPECT_EQ(
      "f40000d33"
      "m8506s4191"
      "m568s487m568s1542m568s1542m568s487m568s487m568s1542m568s487m568s1542"
      "m568s1542m568s487m568s487m568s1542m568s1542m568s487m568s1542m568s487"
      "m568s1542m568s487m568s487m568s487m568s487m568s1542m568s487m568s1542"
      "m568s487m568s1542m568s1542m568s1542m568s1542m568s487m568s1542m568s487"
      "m568s25181"
      "m8506s4191"
      "m568s1542m568s1542m568s1542m568s1542m568s487m568s1542m568s487m568s1542"
      "m568s487m568s487m568s487m568s487m568s1542m568s487m568s1542m568s487"
      "m568s487m568s487m568s1542m568s1542m568s1542m568s1542m568s487m568s1542"
      "m568s1542m568s1542m568s487m568s487m568s487m568s487m568s1542m568s487"
      "m568s25181",
      irsend.outputStr());
}

// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1220#issuecomment-661598412
TEST(TestDecodePioneer, Issue1220) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  // Pwr Toggle {"IrReceived":{"Protocol":"PIONEER","Bits":64,
  //             "Data":"0xA55A38C7A55A38C7"}}
  const uint16_t rawPowerToggle[203] = {
      8510, 4188, 580, 1544, 556, 470, 582, 1566, 556, 494, 560, 488, 556, 1542,
      580, 470, 584, 1540, 582, 492, 552, 1522, 578, 496, 558, 1564, 558, 1542,
      580, 470, 586, 1564, 536, 512, 532, 494, 560, 464, 582, 1542, 580, 1544,
      578, 1544, 578, 498, 558, 492, 562, 488, 556, 1518, 582, 1542, 582, 492,
      552, 498, 556, 494, 550, 1572, 560, 1538, 562, 1536, 586, 25188,
      8512, 4186, 584, 1540, 584, 468, 576, 1572, 528, 522, 532, 492, 552, 1546,
      584, 492, 550, 1548, 582, 494, 560, 1538, 580, 468, 586, 1514, 586, 1538,
      584, 490, 554, 1546, 586, 488, 554, 470, 584, 490, 556, 1542, 556, 1542,
      580, 1544, 578, 496, 560, 466, 578, 496, 560, 1538, 584, 1516, 584, 490,
      554, 472, 582, 492, 552, 1546, 586, 1536, 586, 1538, 586, 25162,
      8514, 4184, 582, 1542, 580, 496, 562, 1536, 584, 490, 554, 496, 560, 1538,
      562, 514, 530, 1542, 580, 496, 560, 1538, 584, 466, 578, 1546, 576, 1524,
      578, 496, 558, 1542, 586, 488, 554, 496, 558, 466, 576, 1546, 576, 1548,
      586, 1512, 578, 498, 558, 492, 552, 496, 558, 1540, 580, 1542, 578, 498,
      556, 494, 550, 498, 556, 1516, 584, 1540, 584, 1540, 580};
  irsend.sendRaw(rawPowerToggle, 203, 40);  // Pioneer uses 40kHz
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(PIONEER, irsend.capture.decode_type);
  EXPECT_EQ(kPioneerBits, irsend.capture.bits);
  EXPECT_EQ(0xA55A38C7A55A38C7, irsend.capture.value);
  EXPECT_EQ(0xA51C, irsend.capture.address);
  EXPECT_EQ(0xA51C, irsend.capture.command);

  irsend.reset();
  irsend.sendPioneer(0xA55A38C7A55A38C7, 64, 1);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(PIONEER, irsend.capture.decode_type);
  EXPECT_EQ(kPioneerBits, irsend.capture.bits);
  EXPECT_EQ(0xA55A38C7A55A38C7, irsend.capture.value);
  EXPECT_EQ(0xA51C, irsend.capture.address);
  EXPECT_EQ(0xA51C, irsend.capture.command);
  EXPECT_EQ(
      "f40000d33"
      "m8506s4191"
      "m568s1542m568s487m568s1542m568s487m568s487m568s1542m568s487m568s1542"
      "m568s487m568s1542m568s487m568s1542m568s1542m568s487m568s1542m568s487"
      "m568s487m568s487m568s1542m568s1542m568s1542m568s487m568s487m568s487"
      "m568s1542m568s1542m568s487m568s487m568s487m568s1542m568s1542m568s1542"
      "m568s25181"
      "m8506s4191"
      "m568s1542m568s487m568s1542m568s487m568s487m568s1542m568s487m568s1542"
      "m568s487m568s1542m568s487m568s1542m568s1542m568s487m568s1542m568s487"
      "m568s487m568s487m568s1542m568s1542m568s1542m568s487m568s487m568s487"
      "m568s1542m568s1542m568s487m568s487m568s487m568s1542m568s1542m568s1542"
      "m568s25181"
      "m8506s4191"
      "m568s1542m568s487m568s1542m568s487m568s487m568s1542m568s487m568s1542"
      "m568s487m568s1542m568s487m568s1542m568s1542m568s487m568s1542m568s487"
      "m568s487m568s487m568s1542m568s1542m568s1542m568s487m568s487m568s487"
      "m568s1542m568s1542m568s487m568s487m568s487m568s1542m568s1542m568s1542"
      "m568s25181"
      "m8506s4191"
      "m568s1542m568s487m568s1542m568s487m568s487m568s1542m568s487m568s1542"
      "m568s487m568s1542m568s487m568s1542m568s1542m568s487m568s1542m568s487"
      "m568s487m568s487m568s1542m568s1542m568s1542m568s487m568s487m568s487"
      "m568s1542m568s1542m568s487m568s487m568s487m568s1542m568s1542m568s1542"
      "m568s25181",
      irsend.outputStr());

  EXPECT_EQ(0xA55A38C7A55A38C7, irsend.encodePioneer(0xA51C, 0xA51C));
}
