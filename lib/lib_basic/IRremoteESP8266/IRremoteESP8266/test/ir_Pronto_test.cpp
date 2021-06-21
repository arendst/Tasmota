// Copyright 2017 David Conran
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendPronto().

TEST(TestSendPronto, CodeTooShort) {
  IRsendTest irsend(4);
  irsend.begin();
  irsend.reset();

  // Less entries than the smallest possible (practical) Pronto code.
  uint16_t pronto_test[5] = {0x0000, 0x0067, 0x0034, 0x0000, 0x0000};
  irsend.sendPronto(pronto_test, 5);
  EXPECT_EQ("", irsend.outputStr());  // Should do nothing.
}

TEST(TestSendPronto, NormalSequenceTooLong) {
  IRsendTest irsend(4);
  irsend.begin();
  irsend.reset();

  // The First 'Normal' sequence is declared to be longer than the data we have.
  uint16_t pronto_test[6] = {0x0000, 0x0067, 0x0010, 0x0000, 0x0000, 0x0000};
  irsend.sendPronto(pronto_test, 6);
  EXPECT_EQ("", irsend.outputStr());  // Should do nothing.
}

TEST(TestSendPronto, RepeatSequenceTooLong) {
  IRsendTest irsend(4);
  irsend.begin();
  irsend.reset();

  // The 2nd 'Repeat' sequence is declared to be longer than the data we have.
  uint16_t pronto_test[6] = {0x0000, 0x0067, 0x0000, 0x0010, 0x0000, 0x0000};
  irsend.sendPronto(pronto_test, 6);
  EXPECT_EQ("", irsend.outputStr());  // Should do nothing.
}

TEST(TestSendPronto, BothSequencesTooLong) {
  IRsendTest irsend(4);
  irsend.begin();
  irsend.reset();

  // All sequences are declared to be longer than the data we have.
  uint16_t pronto_test[6] = {0x0000, 0x0067, 0x0010, 0x0010, 0x0000, 0x0000};
  irsend.sendPronto(pronto_test, 6);
  EXPECT_EQ("", irsend.outputStr());  // Should do nothing.
}

TEST(TestSendPronto, MoreDataThanNeededInNormal) {
  IRsendTest irsend(4);
  irsend.begin();
  irsend.reset();

  // We should handle when we are given more data than needed. (normal seq.)
  uint16_t pronto_test[8] = {0x0000, 0x0067, 0x0001, 0x0000,
                             0x0001, 0x0002, 0x0003, 0x0004};
  irsend.sendPronto(pronto_test, 8);
  EXPECT_EQ("f40244d50m24s49",
            irsend.outputStr());  // Only send the data required.
}

TEST(TestSendPronto, MoreDataThanNeededInRepeat) {
  IRsendTest irsend(4);
  irsend.begin();
  irsend.reset();

  // We should handle when we are given more data than needed. (repeat seq.)
  uint16_t pronto_test[8] = {0x0000, 0x0067, 0x0000, 0x0001,
                             0x0001, 0x0002, 0x0003, 0x0004};
  irsend.sendPronto(pronto_test, 8);
  EXPECT_EQ("f40244d50m24s49",
            irsend.outputStr());  // Only send the data required.
}

TEST(TestSendPronto, MoreDataThanNeededInBoth) {
  IRsendTest irsend(4);
  irsend.begin();
  irsend.reset();

  // We should handle when we are given more data than needed. (repeat seq.)
  uint16_t pronto_test[10] = {0x0000, 0x0067, 0x0001, 0x0001, 0x0001,
                              0x0002, 0x0003, 0x0004, 0x5,    0x6};
  irsend.sendPronto(pronto_test, 10);
  EXPECT_EQ("f40244d50m24s49",
             irsend.outputStr());  // Only send the data required.
  irsend.sendPronto(pronto_test, 10, 1);
  EXPECT_EQ("f40244d50m24s49m74s99",
             irsend.outputStr());  // Only the data required.
}

TEST(TestSendPronto, ShortestValidCodeThatSendsNothing) {
  IRsendTest irsend(4);
  irsend.begin();
  irsend.reset();

  uint16_t pronto_test[6] = {0x0000, 0x0067, 0x0000, 0x0000, 0x0001, 0x0002};
  irsend.sendPronto(pronto_test, 6);
  EXPECT_EQ("", irsend.outputStr());  // Nothing Happens.
  irsend.sendPronto(pronto_test, 6, 1);
  EXPECT_EQ("", irsend.outputStr());  // Twice as much Nothing Happens. ;-)
}

// Test sending a Pronto code that only has a normal (first) sequence.
// i.e. No Pronto repeat sequence.
TEST(TestSendPronto, NonRepeatingCode) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // A random Pronto code I found on the Internet that has no repeat sequence.
  // It was an example of a poor Pronto code.
  // It turned out to be a 4 copies of a Sony 12-bit code. Who knew!?!
  uint16_t pronto_test[108] = {
      0x0000, 0x0067, 0x0034, 0x0000, 0x0060, 0x0018, 0x0018, 0x0018, 0x0018,
      0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018,
      0x0018, 0x0018, 0x0030, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018,
      0x0018, 0x0018, 0x0452, 0x0060, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018,
      0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018,
      0x0018, 0x0030, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018,
      0x0018, 0x0452, 0x0060, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018,
      0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018,
      0x0030, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018,
      0x0452, 0x0060, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018,
      0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0030,
      0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018};

  // Send the Pronto code without any repeats set.
  irsend.reset();
  irsend.sendPronto(pronto_test, 108);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(kSony12Bits, irsend.capture.bits);
  EXPECT_EQ(0x10, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_EQ(
      "f40244d50"
      "m2390s597"
      "m597s597m597s597m597s597m597s597m597s597m597s597m597s597m1195s597"
      "m597s597m597s597m597s597m597s27539"
      "m2390s597"
      "m597s597m597s597m597s597m597s597m597s597m597s597m597s597m1195s597"
      "m597s597m597s597m597s597m597s27539"
      "m2390s597"
      "m597s597m597s597m597s597m597s597m597s597m597s597m597s597m1195s597"
      "m597s597m597s597m597s597m597s27539"
      "m2390s597"
      "m597s597m597s597m597s597m597s597m597s597m597s597m597s597m1195s597"
      "m597s597m597s597m597s597m597s597",
      irsend.outputStr());

  // Now try repeating it.
  // As it has no repeat sequence, we shouldn't repeat it. (I think)
  irsend.reset();
  irsend.sendPronto(pronto_test, 108, 3);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(kSony12Bits, irsend.capture.bits);
  EXPECT_EQ(0x10, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_EQ(
      "f40244d50"
      "m2390s597"
      "m597s597m597s597m597s597m597s597m597s597m597s597m597s597m1195s597"
      "m597s597m597s597m597s597m597s27539"
      "m2390s597"
      "m597s597m597s597m597s597m597s597m597s597m597s597m597s597m1195s597"
      "m597s597m597s597m597s597m597s27539"
      "m2390s597"
      "m597s597m597s597m597s597m597s597m597s597m597s597m597s597m1195s597"
      "m597s597m597s597m597s597m597s27539"
      "m2390s597"
      "m597s597m597s597m597s597m597s597m597s597m597s597m597s597m1195s597"
      "m597s597m597s597m597s597m597s597",
      irsend.outputStr());
}

// Test sending a Pronto code that only has a repeat sequence (Sony).
TEST(TestSendPronto, RepeatSequenceOnlyForSony) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Sony 20-bit command.
  uint16_t pronto_test[46] = {
      0x0000, 0x0067, 0x0000, 0x0015, 0x0060, 0x0018, 0x0018, 0x0018,
      0x0030, 0x0018, 0x0030, 0x0018, 0x0030, 0x0018, 0x0018, 0x0018,
      0x0030, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0030, 0x0018,
      0x0018, 0x0018, 0x0030, 0x0018, 0x0030, 0x0018, 0x0030, 0x0018,
      0x0018, 0x0018, 0x0018, 0x0018, 0x0030, 0x0018, 0x0018, 0x0018,
      0x0018, 0x0018, 0x0030, 0x0018, 0x0018, 0x03f6};

  // Send the Pronto code without any repeats set.
  irsend.reset();
  irsend.sendPronto(pronto_test, 46);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(kSony20Bits, irsend.capture.bits);
  EXPECT_EQ(0x74B92, irsend.capture.value);
  EXPECT_EQ(0x1A, irsend.capture.address);
  EXPECT_EQ(0x24AE, irsend.capture.command);
  EXPECT_EQ(
      "f40244d50"
      "m2390s597"
      "m597s597m1195s597m1195s597m1195s597m597s597m1195s597m597s597m597s597"
      "m1195s597m597s597m1195s597m1195s597m1195s597m597s597m597s597m1195s597"
      "m597s597m597s597m1195s597m597s25248",
      irsend.outputStr());

  // Send the Pronto code with 2 repeats.
  irsend.reset();
  irsend.sendPronto(pronto_test, 46, kSonyMinRepeat);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(kSony20Bits, irsend.capture.bits);
  EXPECT_EQ(0x74B92, irsend.capture.value);
  EXPECT_EQ(0x1A, irsend.capture.address);
  EXPECT_EQ(0x24AE, irsend.capture.command);
  EXPECT_EQ(
      "f40244d50"
      "m2390s597"
      "m597s597m1195s597m1195s597m1195s597m597s597m1195s597m597s597m597s597"
      "m1195s597m597s597m1195s597m1195s597m1195s597m597s597m597s597m1195s597"
      "m597s597m597s597m1195s597m597s25248"
      "m2390s597"
      "m597s597m1195s597m1195s597m1195s597m597s597m1195s597m597s597m597s597"
      "m1195s597m597s597m1195s597m1195s597m1195s597m597s597m597s597m1195s597"
      "m597s597m597s597m1195s597m597s25248"
      "m2390s597"
      "m597s597m1195s597m1195s597m1195s597m597s597m1195s597m597s597m597s597"
      "m1195s597m597s597m1195s597m1195s597m1195s597m597s597m597s597m1195s597"
      "m597s597m597s597m1195s597m597s25248",
      irsend.outputStr());
}

// Test sending a Pronto code that only has a repeat sequence (Panasonic).
TEST(TestSendPronto, RepeatSequenceOnlyForPanasonic) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Panasonic Plasma TV Descrete code (Power On).
  uint16_t pronto_test[104] = {
      0x0000, 0x0071, 0x0000, 0x0032, 0x0080, 0x003F, 0x0010, 0x0010, 0x0010,
      0x0030, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010,
      0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010,
      0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0030, 0x0010, 0x0010,
      0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010,
      0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0030,
      0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010,
      0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010,
      0x0010, 0x0030, 0x0010, 0x0030, 0x0010, 0x0030, 0x0010, 0x0030, 0x0010,
      0x0030, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0030,
      0x0010, 0x0030, 0x0010, 0x0030, 0x0010, 0x0030, 0x0010, 0x0030, 0x0010,
      0x0010, 0x0010, 0x0030, 0x0010, 0x0A98};

  // Send the Pronto code without any repeats set.
  irsend.reset();
  irsend.sendPronto(pronto_test, 104);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicBits, irsend.capture.bits);
  EXPECT_EQ(0x400401007C7D, irsend.capture.value);
  EXPECT_EQ(0x4004, irsend.capture.address);
  EXPECT_EQ(0x1007C7D, irsend.capture.command);
  EXPECT_EQ(
      "f36682d50"
      "m3494s1719"
      "m436s436m436s1310m436s436m436s436m436s436m436s436m436s436m436s436"
      "m436s436m436s436m436s436m436s436m436s436m436s1310m436s436m436s436"
      "m436s436m436s436m436s436m436s436m436s436m436s436m436s436m436s1310"
      "m436s436m436s436m436s436m436s436m436s436m436s436m436s436m436s436"
      "m436s436m436s1310m436s1310m436s1310m436s1310m436s1310m436s436m436s436"
      "m436s436m436s1310m436s1310m436s1310m436s1310m436s1310m436s436m436s1310"
      "m436s74037",
      irsend.outputStr());
}

// Test sending a Pronto code that has a normal & arepeat sequence (NEC).
TEST(TestSendPronto, NormalPlusRepeatSequence) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // NEC 32 bit power on command.
  uint16_t pronto_test[76] = {
      0x0000, 0x006D, 0x0022, 0x0002, 0x0156, 0x00AB, 0x0015, 0x0015, 0x0015,
      0x0015, 0x0015, 0x0015, 0x0015, 0x0040, 0x0015, 0x0040, 0x0015, 0x0015,
      0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0040, 0x0015, 0x0040, 0x0015,
      0x0040, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0040, 0x0015, 0x0040,
      0x0015, 0x0040, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015,
      0x0040, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015,
      0x0015, 0x0040, 0x0015, 0x0040, 0x0015, 0x0040, 0x0015, 0x0015, 0x0015,
      0x0040, 0x0015, 0x0040, 0x0015, 0x0040, 0x0015, 0x0040, 0x0015, 0x05FD,
      0x0156, 0x0055, 0x0015, 0x0E4E};

  // Send the Pronto code without any repeats set.
  irsend.reset();
  irsend.sendPronto(pronto_test, 76);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x18E710EF, irsend.capture.value);
  EXPECT_EQ(0x18, irsend.capture.address);
  EXPECT_EQ(0x8, irsend.capture.command);
  EXPECT_EQ(
      "f38028d50"
      "m8994s4497"
      "m552s552m552s552m552s552m552s1683m552s1683m552s552m552s552m552s552"
      "m552s1683m552s1683m552s1683m552s552m552s552m552s1683m552s1683m552s1683"
      "m552s552m552s552m552s552m552s1683m552s552m552s552m552s552m552s552"
      "m552s1683m552s1683m552s1683m552s552m552s1683m552s1683m552s1683m552s1683"
      "m552s40317",
      irsend.outputStr());

  // Send it again with a single repeat.
  irsend.reset();
  irsend.sendPronto(pronto_test, 76, 1);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x18E710EF, irsend.capture.value);
  EXPECT_EQ(0x18, irsend.capture.address);
  EXPECT_EQ(0x8, irsend.capture.command);
  EXPECT_EQ(
      "f38028d50"
      "m8994s4497"
      "m552s552m552s552m552s552m552s1683m552s1683m552s552m552s552m552s552"
      "m552s1683m552s1683m552s1683m552s552m552s552m552s1683m552s1683m552s1683"
      "m552s552m552s552m552s552m552s1683m552s552m552s552m552s552m552s552"
      "m552s1683m552s1683m552s1683m552s552m552s1683m552s1683m552s1683m552s1683"
      "m552s40317"
      "m8994s2235m552s96310",
      irsend.outputStr());

  // Send it again with a two repeats.
  irsend.reset();
  irsend.sendPronto(pronto_test, 76, 2);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x18E710EF, irsend.capture.value);
  EXPECT_EQ(0x18, irsend.capture.address);
  EXPECT_EQ(0x8, irsend.capture.command);
  EXPECT_EQ(
      "f38028d50"
      "m8994s4497"
      "m552s552m552s552m552s552m552s1683m552s1683m552s552m552s552m552s552"
      "m552s1683m552s1683m552s1683m552s552m552s552m552s1683m552s1683m552s1683"
      "m552s552m552s552m552s552m552s1683m552s552m552s552m552s552m552s552"
      "m552s1683m552s1683m552s1683m552s552m552s1683m552s1683m552s1683m552s1683"
      "m552s40317"
      "m8994s2235m552s96310"
      "m8994s2235m552s96310",
      irsend.outputStr());
}

// Tests for #1034
TEST(TestSendPronto, Issue1034) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  // JVC code allegedly.
  uint16_t pronto_test[40] = {
      0x0000, 0x006c, 0x0001, 0x0011, 0x0140, 0x00a0, 0x0014, 0x003c, 0x0014,
      0x003c, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x003c,
      0x0014, 0x0014, 0x0014, 0x003c, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014,
      0x0014, 0x0014, 0x0014, 0x0014, 0x003c, 0x0014, 0x0014, 0x0014, 0x0014,
      0x0014, 0x0014, 0x0014, 0x0384};

  irsend.reset();
  irsend.sendPronto(pronto_test, 40, 1);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(JVC, irsend.capture.decode_type);
  EXPECT_EQ(kJvcBits, irsend.capture.bits);
  EXPECT_EQ(0xc508, irsend.capture.value);
  EXPECT_EQ(0xa3, irsend.capture.address);
  EXPECT_EQ(0x10, irsend.capture.command);
}

// Tests for #1103
TEST(TestSendPronto, Issue1103) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();
  // Based on raw data:
  //   uint16_t rawData[7] = {20100, 20472, 15092, 30704, 20102, 20472, 15086};
  // and output from `raw_to_pronto_code.py --hz 38000`:
  // Pronto code = '0000 006D 0004 0000 02FB 0309 023D 048E 02FB 0309 023D 0ED8'
  uint16_t pronto_test[12] = {
      0x0000, 0x006D, 0x0004, 0x0000, 0x02FB, 0x0309, 0x023D, 0x048E,
      0x02FB, 0x0309, 0x023D, 0x0ED8};
  irsend.reset();
  irsend.sendPronto(pronto_test, 12);
  EXPECT_EQ(
      "f38028d50m20066s20435m15069s30665m20066s20435m15069s99940",
      irsend.outputStr());
  // Which pretty much matches the `rawData` above.

  // Shorter test.
  // uint16_t rawData[4] = {20100, 20472, 15092, 30704};
  // and output from `raw_to_pronto_code.py --hz 38000`:
  //   Pronto code = '0000 006D 0002 0000 02FB 0309 023D 048E'
  uint16_t pronto_test2[8] = {
      0x0000, 0x006D, 0x0002, 0x0000, 0x02FB, 0x0309, 0x023D, 0x048E};
  irsend.reset();
  irsend.sendPronto(pronto_test2, 8);
  EXPECT_EQ(
      "f38028d50m20066s20435m15069s30665",
      irsend.outputStr());
  // Which pretty much matches the `rawData` above.

  // Ref:
  //  https://github.com/crankyoldgit/IRremoteESP8266/issues/1103#issuecomment-628946514
  uint16_t pronto_test_using_repeat[12] = {
      0x0000, 0x006D, 0x0000, 0x0004, 0x02fb, 0x0309, 0x023d, 0x048e, 0x02fb,
      0x0309, 0x023d, 0x0474};
  irsend.reset();
  irsend.sendPronto(pronto_test_using_repeat, 12);
  EXPECT_EQ(
      "f38028d50m20066s20435m15069s30665m20066s20435m15069s29982",
      irsend.outputStr());
}
