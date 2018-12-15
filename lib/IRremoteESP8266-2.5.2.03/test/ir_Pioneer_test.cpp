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
      "m8960s4480"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s58240"
      "m8960s4480"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s58240",
      irsend.outputStr());
  irsend.sendPioneer(0x55FF00AAAA00FF55);
  EXPECT_EQ(
      "m8960s4480"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s40320"
      "m8960s4480"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680m560s1680"
      "m560s560m560s1680m560s560m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s40320",
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
  // https://github.com/markszabo/IRremoteESP8266/pull/547#issuecomment-429616582
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
  // https://github.com/markszabo/IRremoteESP8266/pull/547#issuecomment-429616582
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
// https://github.com/markszabo/IRremoteESP8266/pull/547#issuecomment-430800734
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
      "m8960s4480"
      "m560s560m560s1680m560s1680m560s560m560s560m560s1680m560s560m560s1680"
      "m560s1680m560s560m560s560m560s1680m560s1680m560s560m560s1680m560s560"
      "m560s1680m560s560m560s560m560s560m560s560m560s1680m560s560m560s1680"
      "m560s560m560s1680m560s1680m560s1680m560s1680m560s560m560s1680m560s560"
      "m560s40320"
      "m8960s4480"
      "m560s1680m560s1680m560s1680m560s1680m560s560m560s1680m560s560m560s1680"
      "m560s560m560s560m560s560m560s560m560s1680m560s560m560s1680m560s560"
      "m560s560m560s560m560s1680m560s1680m560s1680m560s1680m560s560m560s1680"
      "m560s1680m560s1680m560s560m560s560m560s560m560s560m560s1680m560s560"
      "m560s40320",
      irsend.outputStr());
}
