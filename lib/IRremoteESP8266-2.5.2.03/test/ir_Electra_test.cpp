// Copyright 2018 David Conran

#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendElectraAC().

// Test sending typical data only.
TEST(TestSendElectraAC, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();
  uint8_t data[kElectraAcStateLength] = {0xC3, 0xE1, 0x6F, 0x14, 0x06,
                                         0x00, 0x04, 0x00, 0x00, 0x04,
                                         0x00, 0xA0, 0xB0};

  irsend.sendElectraAC(data);
  EXPECT_EQ(
      "m9166s4470"
      "m646s1647m646s1647m646s547m646s547m646s547m646s547m646s1647m646s1647"
      "m646s1647m646s1647m646s1647m646s547m646s547m646s547m646s547m646s1647"
      "m646s547m646s1647m646s1647m646s547m646s1647m646s1647m646s1647m646s1647"
      "m646s547m646s547m646s547m646s1647m646s547m646s1647m646s547m646s547"
      "m646s547m646s547m646s547m646s547m646s547m646s1647m646s1647m646s547"
      "m646s547m646s547m646s547m646s547m646s547m646s547m646s547m646s547"
      "m646s547m646s547m646s547m646s547m646s547m646s1647m646s547m646s547"
      "m646s547m646s547m646s547m646s547m646s547m646s547m646s547m646s547"
      "m646s547m646s547m646s547m646s547m646s547m646s547m646s547m646s547"
      "m646s547m646s547m646s547m646s547m646s547m646s1647m646s547m646s547"
      "m646s547m646s547m646s547m646s547m646s547m646s547m646s547m646s547"
      "m646s1647m646s547m646s1647m646s547m646s547m646s547m646s547m646s547"
      "m646s1647m646s547m646s1647m646s1647m646s547m646s547m646s547m646s547"
      "m646s100000",
      irsend.outputStr());
}

// Tests for decodeElectraAC().
// Decode normal ElectraAC messages.

TEST(TestDecodeElectraAC, SyntheticDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Synthesised Normal ElectraAC message.
  irsend.reset();
  uint8_t expectedState[kElectraAcStateLength] = {0xC3, 0xE1, 0x6F, 0x14, 0x06,
                                                  0x00, 0x04, 0x00, 0x00, 0x04,
                                                  0x00, 0xA0, 0xB0};
  irsend.sendElectraAC(expectedState);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(ELECTRA_AC, irsend.capture.decode_type);
  EXPECT_EQ(kElectraAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
}

// Decode a recorded example
TEST(TestDecodeElectraAC, RealExampleDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // Real ElectraAC message.
  // Ref: https://github.com/markszabo/IRremoteESP8266/issues/527
  uint16_t rawData[211] = {
      9166, 4470, 642, 1632, 642, 1632, 668, 534,  666, 534,  668, 534,
      614,  536,  640, 1636, 640, 1646, 694, 1662, 612, 1628, 642, 1666,
      664,  532,  668, 534,  666, 534,  666, 532,  666, 1644, 642, 532,
      640,  1634, 668, 1632, 642, 538,  666, 1660, 610, 1666, 664, 1632,
      642,  1672, 610, 536,  666, 534,  694, 532,  666, 1636, 614, 538,
      666,  1632, 642, 536,  666, 544,  692, 534,  640, 558,  640, 534,
      640,  540,  666, 534,  638, 1666, 638, 1636, 640, 550,  666, 534,
      640,  540,  666, 534,  640, 540,  666, 536,  638, 540,  666, 536,
      638,  550,  664, 536,  638, 540,  664, 536,  638, 540,  666, 534,
      638,  1640, 664, 536,  692, 546,  664, 536,  664, 536,  664, 536,
      664,  546,  612, 532,  636, 538,  664, 536,  664, 546,  612, 538,
      638,  538,  638, 538,  664, 536,  690, 538,  662, 538,  664, 538,
      662,  548,  664, 536,  662, 538,  662, 562,  638, 564,  636, 564,
      636,  1668, 582, 556,  652, 572,  612, 568,  636, 564,  610, 570,
      636,  556,  616, 550,  656, 566,  610, 570,  632, 578,  608, 1640,
      662,  562,  642, 1686, 582, 570,  634, 566,  604, 576,  636, 566,
      610,  578,  634, 1664, 584, 590,  660, 1636, 610, 1642, 664, 590,
      610,  590,  636, 566,  634, 568,  686};  // UNKNOWN 9AD8CDB5
  uint8_t expectedState[kElectraAcStateLength] = {0xC3, 0xE1, 0x6F, 0x14, 0x06,
                                                  0x00, 0x04, 0x00, 0x00, 0x04,
                                                  0x00, 0xA0, 0xB0};

  irsend.reset();
  irsend.sendRaw(rawData, 211, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(ELECTRA_AC, irsend.capture.decode_type);
  EXPECT_EQ(kElectraAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
}
