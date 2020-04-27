// Copyright 2017 David Conran

#include "ir_Sharp.h"
#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for encodeSharp().

TEST(TestEncodeSharp, NormalEncoding) {
  IRsendTest irsend(4);
  EXPECT_EQ(0x2, irsend.encodeSharp(0, 0));
  EXPECT_EQ(0x4202, irsend.encodeSharp(1, 1));
  EXPECT_EQ(0x4102, irsend.encodeSharp(1, 2));
  EXPECT_EQ(0x62E2, irsend.encodeSharp(0x43, 0x1D));
  EXPECT_EQ(0x2AAA, irsend.encodeSharp(0xAA, 0x55));
  EXPECT_EQ(0x7FFE, irsend.encodeSharp(0x1F, 0xFF));
  EXPECT_EQ(0x454A, irsend.encodeSharp(0x11, 0x4A));
}

TEST(TestEncodeSharp, AdvancedEncoding) {
  IRsendTest irsend(4);
  EXPECT_EQ(0x0, irsend.encodeSharp(0, 0, 0, 0));
  EXPECT_EQ(0x1, irsend.encodeSharp(0, 0, 0, 1));
  EXPECT_EQ(0x2, irsend.encodeSharp(0, 0, 1, 0));
  EXPECT_EQ(0x3, irsend.encodeSharp(0, 0, 1, 1));
  EXPECT_EQ(0x4200, irsend.encodeSharp(1, 1, 0, 0));
  EXPECT_EQ(0x4203, irsend.encodeSharp(1, 1, 1, 1));

  EXPECT_EQ(0x4200, irsend.encodeSharp(1, 1, 0, 0, false));
  EXPECT_EQ(0x4201, irsend.encodeSharp(1, 1, 0, 1, false));
  EXPECT_EQ(0x4203, irsend.encodeSharp(1, 1, 1, 1, false));

  EXPECT_EQ(0x0404, irsend.encodeSharp(1, 1, 0, 0, true));
  EXPECT_EQ(0x0405, irsend.encodeSharp(1, 1, 0, 1, true));
  EXPECT_EQ(0x0407, irsend.encodeSharp(1, 1, 1, 1, true));

  EXPECT_EQ(0x454A, irsend.encodeSharp(0x11, 0x52, 1, 0, true));
}

// Tests for sendSharp().

// Test sending typical data only.
TEST(TestSendSharp, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSharp(0x11, 0x52);
  EXPECT_EQ(
      "f38000d33"
      "m260s1820m260s780m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s1820m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s43602"
      "m260s1820m260s780m260s780m260s780m260s1820m260s1820m260s780m260s1820"
      "m260s780m260s1820m260s1820m260s780m260s1820m260s780m260s1820"
      "m260s43602",
      irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendSharp, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSharp(0x11, 0x52, kSharpBits, 1);  // 1 repeat.
  EXPECT_EQ(
      "f38000d33"
      "m260s1820m260s780m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s1820m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s43602"
      "m260s1820m260s780m260s780m260s780m260s1820m260s1820m260s780m260s1820"
      "m260s780m260s1820m260s1820m260s780m260s1820m260s780m260s1820"
      "m260s43602"
      "m260s1820m260s780m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s1820m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s43602"
      "m260s1820m260s780m260s780m260s780m260s1820m260s1820m260s780m260s1820"
      "m260s780m260s1820m260s1820m260s780m260s1820m260s780m260s1820"
      "m260s43602",
      irsend.outputStr());
}

// Test sending an atypical data size.
TEST(TestSendSharp, SendUnusualSize) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSharp(0x0, 0x0, 8);
  EXPECT_EQ(
      "f38000d33"
      "m260s780m260s780m260s780m260s780m260s780m260s780m260s1820m260s780"
      "m260s43602"
      "m260s1820m260s1820m260s1820m260s1820m260s1820m260s1820m260s780m260s1820"
      "m260s43602",
      irsend.outputStr());

  irsend.reset();
  irsend.sendSharp(0x0, 0x0, 16);
  EXPECT_EQ(
      "f38000d33"
      "m260s780m260s780m260s780m260s780m260s780m260s780m260s780m260s780"
      "m260s780m260s780m260s780m260s780m260s780m260s780m260s1820m260s780"
      "m260s43602"
      "m260s780m260s780m260s780m260s780m260s780m260s780m260s1820m260s1820"
      "m260s1820m260s1820m260s1820m260s1820m260s1820m260s1820m260s780m260s1820"
      "m260s43602",
      irsend.outputStr());
}

// Tests for sendSharpRaw().

// Test sending typical data only.
TEST(TestSendSharpRaw, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSharpRaw(0x454A);
  EXPECT_EQ(
      "f38000d33"
      "m260s1820m260s780m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s1820m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s43602"
      "m260s1820m260s780m260s780m260s780m260s1820m260s1820m260s780m260s1820"
      "m260s780m260s1820m260s1820m260s780m260s1820m260s780m260s1820"
      "m260s43602",
      irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendSharpRaw, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSharpRaw(0x454A, kSharpBits, 1);  // 1 repeat.
  EXPECT_EQ(
      "f38000d33"
      "m260s1820m260s780m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s1820m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s43602"
      "m260s1820m260s780m260s780m260s780m260s1820m260s1820m260s780m260s1820"
      "m260s780m260s1820m260s1820m260s780m260s1820m260s780m260s1820"
      "m260s43602"
      "m260s1820m260s780m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s1820m260s780m260s780m260s1820m260s780m260s1820m260s780"
      "m260s43602"
      "m260s1820m260s780m260s780m260s780m260s1820m260s1820m260s780m260s1820"
      "m260s780m260s1820m260s1820m260s780m260s1820m260s780m260s1820"
      "m260s43602",
      irsend.outputStr());
}

// Test sending an atypical data size.
TEST(TestSendSharpRaw, SendUnusualSize) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSharpRaw(0x2, 8);
  EXPECT_EQ(
      "f38000d33"
      "m260s780m260s780m260s780m260s780m260s780m260s780m260s1820m260s780"
      "m260s43602"
      "m260s1820m260s1820m260s1820m260s1820m260s1820m260s1820m260s780m260s1820"
      "m260s43602",
      irsend.outputStr());

  irsend.reset();
  irsend.sendSharpRaw(0x2, 16);
  EXPECT_EQ(
      "f38000d33"
      "m260s780m260s780m260s780m260s780m260s780m260s780m260s780m260s780"
      "m260s780m260s780m260s780m260s780m260s780m260s780m260s1820m260s780"
      "m260s43602"
      "m260s780m260s780m260s780m260s780m260s780m260s780m260s1820m260s1820"
      "m260s1820m260s1820m260s1820m260s1820m260s1820m260s1820m260s780m260s1820"
      "m260s43602",
      irsend.outputStr());
}

// Tests for decodeSharp().

// Decode normal Sharp messages.
TEST(TestDecodeSharp, NormalDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Sharp 15-bit message.
  irsend.reset();
  irsend.sendSharpRaw(0x454A);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decodeSharp(&irsend.capture, kStartOffset, kSharpBits,
                                 true));
  EXPECT_EQ(SHARP, irsend.capture.decode_type);
  EXPECT_EQ(kSharpBits, irsend.capture.bits);
  EXPECT_EQ(0x454A, irsend.capture.value);
  EXPECT_EQ(0x11, irsend.capture.address);
  EXPECT_EQ(0x4A, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Synthesised Normal Sharp 15-bit message.
  irsend.reset();
  irsend.sendSharpRaw(irsend.encodeSharp(0x07, 0x99));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSharp(&irsend.capture, kStartOffset, kSharpBits,
                                 true));
  EXPECT_EQ(SHARP, irsend.capture.decode_type);
  EXPECT_EQ(kSharpBits, irsend.capture.bits);
  EXPECT_EQ(0x7266, irsend.capture.value);
  EXPECT_EQ(0x07, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Synthesised Normal Sharp 15-bit message.
  irsend.reset();
  irsend.sendSharpRaw(irsend.encodeSharp(0x1, 0x1));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSharp(&irsend.capture, kStartOffset, kSharpBits,
                                 true));
  EXPECT_EQ(SHARP, irsend.capture.decode_type);
  EXPECT_EQ(kSharpBits, irsend.capture.bits);
  EXPECT_EQ(0x4202, irsend.capture.value);
  EXPECT_EQ(0x1, irsend.capture.address);
  EXPECT_EQ(0x1, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode normal repeated Sharp messages.
TEST(TestDecodeSharp, NormalDecodeWithRepeatAndStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Sharp 15-bit message with 1 repeat.
  irsend.reset();
  irsend.sendSharpRaw(0x7266, kSharpBits, 1);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeSharp(&irsend.capture, kStartOffset, kSharpBits,
                                 true));
  EXPECT_EQ(SHARP, irsend.capture.decode_type);
  EXPECT_EQ(kSharpBits, irsend.capture.bits);
  EXPECT_EQ(0x7266, irsend.capture.value);
  EXPECT_EQ(0x07, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);

  irsend.makeDecodeResult(2 * (2 * kSharpBits + kFooter));
  ASSERT_TRUE(irrecv.decodeSharp(&irsend.capture, kStartOffset, kSharpBits,
                                 true));
  EXPECT_EQ(SHARP, irsend.capture.decode_type);
  EXPECT_EQ(kSharpBits, irsend.capture.bits);
  EXPECT_EQ(0x7266, irsend.capture.value);
}

// Decode unsupported Sharp messages.
TEST(TestDecodeSharp, DecodeWithNonStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  irsend.sendSharpRaw(0x0, 8);  // Illegal length Sharp 8-bit message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeSharp(&irsend.capture, kStartOffset, kSharpBits,
                                  true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeSharp(&irsend.capture, kStartOffset, 8, false));
  EXPECT_EQ(SHARP, irsend.capture.decode_type);
  EXPECT_EQ(8, irsend.capture.bits);
  EXPECT_EQ(0x0, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  irsend.sendSharpRaw(0x12345678, 32);  // Illegal length Sharp 32-bit message.
  irsend.makeDecodeResult();
  // Should fail with strict on.
  ASSERT_FALSE(irrecv.decodeSharp(&irsend.capture, kStartOffset, kSharpBits,
                                  true));

  // Should fail with strict when we ask for the wrong bit size.
  ASSERT_FALSE(irrecv.decodeSharp(&irsend.capture, kStartOffset, 32, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeSharp(&irsend.capture, kStartOffset, 32, false));
  EXPECT_EQ(SHARP, irsend.capture.decode_type);
  EXPECT_EQ(32, irsend.capture.bits);
  EXPECT_EQ(0x12345678, irsend.capture.value);
  EXPECT_EQ(0x8, irsend.capture.address);
  EXPECT_EQ(0x79, irsend.capture.command);
}

// Decode (non-standard) 64-bit messages.
TEST(TestDecodeSharp, Decode64BitMessages) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Illegal value & size Sharp 64-bit message.
  irsend.sendSharpRaw(0xFFFFFFFFFFFFFFFF, 64);
  irsend.makeDecodeResult();
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeSharp(&irsend.capture, kStartOffset, 64, false));
  EXPECT_EQ(SHARP, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, irsend.capture.value);
  EXPECT_EQ(0x1F, irsend.capture.address);
  EXPECT_EQ(0xFF, irsend.capture.command);
}

// Decode a 'real' example via GlobalCache
TEST(TestDecodeSharp, DecodeGlobalCacheExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Sharp Power On from Global Cache.
  uint16_t gc_test[67] = {
      38000, 1,  1,  10, 70, 10, 30, 10, 30, 10, 30, 10, 70, 10, 30, 10,  70,
      10,    30, 10, 70, 10, 30, 10, 30, 10, 70, 10, 30, 10, 70, 10, 30,  10,
      1657,  10, 70, 10, 30, 10, 30, 10, 30, 10, 70, 10, 70, 10, 30, 10,  70,
      10,    30, 10, 70, 10, 70, 10, 30, 10, 70, 10, 30, 10, 70, 10, 1657};
  irsend.sendGC(gc_test, 67);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodeSharp(&irsend.capture));
  EXPECT_EQ(SHARP, irsend.capture.decode_type);
  EXPECT_EQ(kSharpBits, irsend.capture.bits);
  EXPECT_EQ(0x454A, irsend.capture.value);
  EXPECT_EQ(0x11, irsend.capture.address);
  EXPECT_EQ(0x4A, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Fail to decode a non-Sharp example via GlobalCache
TEST(TestDecodeSharp, FailToDecodeNonSharpExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Modified a few entries to unexpected values, based on previous test case.
  uint16_t gc_test[67] = {
      38000, 1,  1,  10, 70, 30, 30, 10, 30, 10, 30, 10, 70, 10, 30, 10,  70,
      10,    30, 10, 70, 10, 30, 10, 30, 10, 70, 10, 30, 10, 70, 10, 30,  10,
      1657,  10, 70, 10, 30, 10, 30, 10, 30, 10, 70, 10, 70, 10, 30, 10,  60,
      10,    30, 10, 70, 10, 70, 10, 30, 10, 10, 70, 30, 10, 70, 10, 1657};
  irsend.sendGC(gc_test, 67);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeSharp(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeSharp(&irsend.capture, kStartOffset, kSharpBits,
                                  false));

  // Test only half of a good message, as it is sent (sort of) twice.
  uint16_t gc_half[35] = {38000, 1,  1,  10, 70, 10, 30, 10, 30, 10, 30,  10,
                          70,    10, 30, 10, 70, 10, 30, 10, 70, 10, 30,  10,
                          30,    10, 70, 10, 30, 10, 70, 10, 30, 10, 1657};

  irsend.sendGC(gc_half, 35);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeSharp(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeSharp(&irsend.capture, kStartOffset, kSharpBits,
                                  false));
}

// https://github.com/crankyoldgit/IRremoteESP8266/issues/638#issue-421064165
TEST(TestDecodeSharpAc, RealExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  // cool-auto-27.txt
  uint16_t rawData[211] = {
      3804, 1892, 466, 486, 466, 1388, 466, 486, 466, 1386, 468, 486, 468, 1388,
      466, 486, 466, 1386, 468, 488, 466, 1388, 466, 488, 466, 1386, 468, 1388,
      466, 486, 466, 1388, 466, 486, 468, 1384, 468, 1388, 468, 1388, 466, 1388,
      466, 486, 468, 484, 468, 1386, 468, 1386, 468, 486, 466, 486, 468, 486,
      466, 488, 466, 1388, 466, 486, 466, 486, 468, 486, 466, 488, 466, 488,
      466, 1386, 468, 1388, 466, 486, 468, 486, 466, 1388, 464, 1388, 466, 1386,
      468, 486, 466, 486, 468, 486, 466, 1388, 468, 1384, 470, 486, 466, 486,
      468, 486, 468, 1386, 468, 486, 468, 486, 468, 486, 468, 1388, 466, 486,
      466, 486, 466, 486, 466, 488, 466, 486, 468, 486, 468, 486, 468, 486, 466,
      486, 466, 486, 466, 488, 466, 486, 466, 486, 466, 1388, 466, 486, 468,
      486, 466, 486, 468, 486, 468, 486, 466, 486, 466, 488, 466, 486, 466, 486,
      466, 488, 466, 486, 468, 1386, 468, 486, 466, 486, 466, 1390, 464, 488,
      466, 486, 468, 486, 468, 486, 466, 486, 466, 486, 466, 486, 468, 486, 468,
      486, 466, 486, 466, 1386, 468, 1390, 466, 1388, 466, 1388, 468, 486, 466,
      486, 468, 486, 466, 486, 466, 486, 466, 1390, 464, 486, 414};
      // UNKNOWN F2B82C78
  uint8_t expectedState[kSharpAcStateLength] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xCC, 0x31, 0x22, 0x00, 0x08, 0x80, 0x04, 0xE0,
      0x41};

  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 211, 38000);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(SHARP_AC, irsend.capture.decode_type);
  ASSERT_EQ(kSharpAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ("Power: On, Previous Power: On, Mode: 2 (Cool), Temp: 27C, "
            "Fan: 2 (Auto)",
            IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

// https://github.com/crankyoldgit/IRremoteESP8266/issues/638#issue-421064165
TEST(TestDecodeSharpAc, SyntheticExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  // cool-auto-27.txt
  uint8_t expectedState[kSharpAcStateLength] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xCC, 0x31, 0x22, 0x00, 0x08, 0x80, 0x04, 0xE0,
      0x41};

  irsend.begin();
  irsend.reset();
  irsend.sendSharpAc(expectedState);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(SHARP_AC, irsend.capture.decode_type);
  ASSERT_EQ(kSharpAcBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
}

TEST(TestIRUtils, Sharp) {
  ASSERT_EQ("SHARP", typeToString(decode_type_t::SHARP));
  ASSERT_EQ(decode_type_t::SHARP, strToDecodeType("SHARP"));
  ASSERT_FALSE(hasACState(decode_type_t::SHARP));
}

TEST(TestIRUtils, SharpAc) {
  ASSERT_EQ("SHARP_AC", typeToString(decode_type_t::SHARP_AC));
  ASSERT_EQ(decode_type_t::SHARP_AC, strToDecodeType("SHARP_AC"));
  ASSERT_TRUE(hasACState(decode_type_t::SHARP_AC));
}

// Tests for IRSharpAc class.

TEST(TestSharpAcClass, Power) {
  IRSharpAc ac(0);
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

TEST(TestSharpAcClass, Checksum) {
  uint8_t state[kSharpAcStateLength] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xCC, 0x31, 0x22, 0x00, 0x08, 0x80, 0x04, 0xE0,
      0x41};
  EXPECT_EQ(0x4, IRSharpAc::calcChecksum(state));
  EXPECT_TRUE(IRSharpAc::validChecksum(state));
  // Change the state so it is not valid.
  state[3] = 0;
  EXPECT_FALSE(IRSharpAc::validChecksum(state));
}

TEST(TestSharpAcClass, Temperature) {
  IRSharpAc ac(0);
  ac.begin();
  ac.setMode(kSharpAcCool);  // Cool mode doesn't have temp restrictions.

  ac.setTemp(0);
  EXPECT_EQ(kSharpAcMinTemp, ac.getTemp());

  ac.setTemp(255);
  EXPECT_EQ(kSharpAcMaxTemp, ac.getTemp());

  ac.setTemp(kSharpAcMinTemp);
  EXPECT_EQ(kSharpAcMinTemp, ac.getTemp());

  ac.setTemp(kSharpAcMaxTemp);
  EXPECT_EQ(kSharpAcMaxTemp, ac.getTemp());

  ac.setTemp(kSharpAcMinTemp - 1);
  EXPECT_EQ(kSharpAcMinTemp, ac.getTemp());

  ac.setTemp(kSharpAcMaxTemp + 1);
  EXPECT_EQ(kSharpAcMaxTemp, ac.getTemp());

  ac.setTemp(kSharpAcMinTemp + 1);
  EXPECT_EQ(kSharpAcMinTemp + 1, ac.getTemp());

  ac.setTemp(21);
  EXPECT_EQ(21, ac.getTemp());

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());

  ac.setTemp(29);
  EXPECT_EQ(29, ac.getTemp());
}

TEST(TestSharpAcClass, OperatingMode) {
  IRSharpAc ac(0);
  ac.begin();

  ac.setTemp(25);
  ac.setMode(kSharpAcAuto);
  EXPECT_EQ(kSharpAcAuto, ac.getMode());

  ac.setMode(kSharpAcCool);
  EXPECT_EQ(kSharpAcCool, ac.getMode());

  ac.setMode(kSharpAcHeat);
  EXPECT_EQ(kSharpAcHeat, ac.getMode());

  ac.setMode(kSharpAcDry);
  EXPECT_EQ(kSharpAcDry, ac.getMode());
  ASSERT_EQ(kSharpAcMinTemp, ac.getTemp());  // Dry mode restricts the temp.
  ac.setTemp(25);
  ASSERT_EQ(kSharpAcMinTemp, ac.getTemp());

  ac.setMode(kSharpAcDry + 1);
  EXPECT_EQ(kSharpAcAuto, ac.getMode());

  ac.setMode(kSharpAcCool);
  EXPECT_EQ(kSharpAcCool, ac.getMode());
  // We are no longer restricted.
  ac.setTemp(25);
  ASSERT_EQ(25, ac.getTemp());

  ac.setMode(255);
  EXPECT_EQ(kSharpAcAuto, ac.getMode());
}


TEST(TestSharpAcClass, FanSpeed) {
  IRSharpAc ac(0);
  ac.begin();

  // Unexpected value should default to Auto.
  ac.setFan(0);
  EXPECT_EQ(kSharpAcFanAuto, ac.getFan());

  // Unexpected value should default to Auto.
  ac.setFan(255);
  EXPECT_EQ(kSharpAcFanAuto, ac.getFan());

  ac.setFan(kSharpAcFanMax);
  EXPECT_EQ(kSharpAcFanMax, ac.getFan());

  // Beyond Max should default to Auto.
  ac.setFan(kSharpAcFanMax + 1);
  EXPECT_EQ(kSharpAcFanAuto, ac.getFan());

  ac.setFan(kSharpAcFanMed);
  EXPECT_EQ(kSharpAcFanMed, ac.getFan());

  ac.setFan(kSharpAcFanMin);
  EXPECT_EQ(kSharpAcFanMin, ac.getFan());

  ac.setFan(kSharpAcFanAuto - 1);
  EXPECT_EQ(kSharpAcFanAuto, ac.getFan());

  ac.setFan(kSharpAcFanMax + 1);
  EXPECT_EQ(kSharpAcFanAuto, ac.getFan());

  ac.setFan(kSharpAcFanAuto);
  EXPECT_EQ(kSharpAcFanAuto, ac.getFan());
}

TEST(TestSharpAcClass, ReconstructKnownState) {
  IRSharpAc ac(0);
  ac.begin();

  uint8_t on_auto_auto[kSharpAcStateLength] = {
      0xAA, 0x5A, 0xCF, 0x10, 0x00, 0x11, 0x20, 0x00, 0x08, 0x80, 0x00, 0xE0,
      0x01};
  ac.setPower(true, false);
  ac.setTemp(kSharpAcMinTemp);
  ac.setFan(kSharpAcFanAuto);
  ac.setMode(kSharpAcAuto);
  EXPECT_STATE_EQ(on_auto_auto, ac.getRaw(), kSharpAcBits);
  EXPECT_EQ("Power: On, Previous Power: Off, Mode: 0 (Auto), Temp: 15C, "
            "Fan: 2 (Auto)",
            ac.toString());

  uint8_t cool_auto_28[kSharpAcStateLength] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xCD, 0x31, 0x22, 0x00, 0x08, 0x80, 0x04, 0xE0,
      0x51};
  ac.stateReset();
  ac.setPower(true, true);
  ac.setMode(kSharpAcCool);
  ac.setFan(kSharpAcFanAuto);
  ac.setTemp(28);
  EXPECT_EQ("Power: On, Previous Power: On, Mode: 2 (Cool), Temp: 28C, "
            "Fan: 2 (Auto)",
            ac.toString());
  EXPECT_STATE_EQ(cool_auto_28, ac.getRaw(), kSharpAcBits);
}

// https://github.com/crankyoldgit/IRremoteESP8266/issues/638#issue-421064165
TEST(TestSharpAcClass, KnownStates) {
  IRSharpAc ac(0);
  ac.begin();

  uint8_t off_auto_auto[kSharpAcStateLength] = {
      0xAA, 0x5A, 0xCF, 0x10, 0x00, 0x21, 0x20, 0x00, 0x08, 0x80, 0x00, 0xE0,
      0x31};
  ASSERT_TRUE(ac.validChecksum(off_auto_auto));
  ac.setRaw(off_auto_auto);
  EXPECT_EQ("Power: Off, Previous Power: On, Mode: 0 (Auto), Temp: 15C, "
            "Fan: 2 (Auto)",
            ac.toString());
  uint8_t on_auto_auto[kSharpAcStateLength] = {
      0xAA, 0x5A, 0xCF, 0x10, 0x00, 0x11, 0x20, 0x00, 0x08, 0x80, 0x00, 0xE0,
      0x01};
  ASSERT_TRUE(ac.validChecksum(on_auto_auto));
  ac.setRaw(on_auto_auto);
  EXPECT_EQ("Power: On, Previous Power: Off, Mode: 0 (Auto), Temp: 15C, "
            "Fan: 2 (Auto)",
            ac.toString());
  uint8_t cool_auto_28[kSharpAcStateLength] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xCD, 0x31, 0x22, 0x00, 0x08, 0x80, 0x04, 0xE0,
      0x51};
  ASSERT_TRUE(ac.validChecksum(cool_auto_28));
  ac.setRaw(cool_auto_28);
  EXPECT_EQ("Power: On, Previous Power: On, Mode: 2 (Cool), Temp: 28C, "
            "Fan: 2 (Auto)",
            ac.toString());
  uint8_t cool_fan1_28[kSharpAcStateLength] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xCD, 0x31, 0x42, 0x00, 0x08, 0x80, 0x05, 0xE0,
      0x21};
  ASSERT_TRUE(ac.validChecksum(cool_fan1_28));
  ac.setRaw(cool_fan1_28);
  EXPECT_EQ("Power: On, Previous Power: On, Mode: 2 (Cool), Temp: 28C, "
            "Fan: 4 (Low)",
            ac.toString());
  uint8_t cool_fan2_28[kSharpAcStateLength] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xCD, 0x31, 0x32, 0x00, 0x08, 0x80, 0x05, 0xE0,
      0x51};
  ASSERT_TRUE(ac.validChecksum(cool_fan2_28));
  ac.setRaw(cool_fan2_28);
  EXPECT_EQ("Power: On, Previous Power: On, Mode: 2 (Cool), Temp: 28C, "
            "Fan: 3 (Medium)",
            ac.toString());
  uint8_t cool_fan3_28[kSharpAcStateLength] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xCD, 0x31, 0x52, 0x00, 0x08, 0x80, 0x05, 0xE0,
      0x31};
  ASSERT_TRUE(ac.validChecksum(cool_fan3_28));
  ac.setRaw(cool_fan3_28);
  EXPECT_EQ("Power: On, Previous Power: On, Mode: 2 (Cool), Temp: 28C, "
            "Fan: 5 (UNKNOWN)",
            ac.toString());
  uint8_t cool_fan4_28[kSharpAcStateLength] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xCD, 0x31, 0x72, 0x00, 0x08, 0x80, 0x05, 0xE0,
      0x11};
  ASSERT_TRUE(ac.validChecksum(cool_fan4_28));
  ac.setRaw(cool_fan4_28);
  EXPECT_EQ("Power: On, Previous Power: On, Mode: 2 (Cool), Temp: 28C, "
            "Fan: 7 (High)",
            ac.toString());
  /* Unsupported / Not yet reverse engineered.
  uint8_t cool_fan4_28_ion_on[kSharpAcStateLength] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xCD, 0x61, 0x72, 0x08, 0x08, 0x80, 0x00, 0xE4,
      0xD1};
  ASSERT_TRUE(ac.validChecksum(cool_fan4_28_ion_on));
  ac.setRaw(cool_fan4_28_ion_on);
  EXPECT_EQ("Power: On, Mode: 2 (Cool), Temp: 28C, Fan: 7 (Max)",
            ac.toString());
  uint8_t cool_fan4_28_eco1[kSharpAcStateLength] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xCD, 0x61, 0x72, 0x18, 0x08, 0x80, 0x00, 0xE8,
      0x01};
  ASSERT_TRUE(ac.validChecksum(cool_fan4_28_eco1));
  ac.setRaw(cool_fan4_28_eco1);
  EXPECT_EQ("Power: On, Mode: 2 (Cool), Temp: 28C, Fan: 7 (Max)",
            ac.toString()); */
  uint8_t dry_auto[kSharpAcStateLength] = {
      0xAA, 0x5A, 0xCF, 0x10, 0x00, 0x31, 0x23, 0x00, 0x08, 0x80, 0x00, 0xE0,
      0x11};
  ASSERT_TRUE(ac.validChecksum(dry_auto));
  ac.setRaw(dry_auto);
  EXPECT_EQ("Power: On, Previous Power: On, Mode: 3 (Dry), Temp: 15C, "
            "Fan: 2 (Auto)",
            ac.toString());
}

TEST(TestSharpAcClass, toCommon) {
  IRSharpAc ac(0);
  ac.setPower(true);
  ac.setMode(kSharpAcCool);
  ac.setTemp(20);
  ac.setFan(kSharpAcFanMax);
  // Now test it.
  ASSERT_EQ(decode_type_t::SHARP_AC, ac.toCommon().protocol);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  // Unsupported.
  ASSERT_EQ(-1, ac.toCommon().model);
  ASSERT_EQ(stdAc::swingv_t::kOff, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  ASSERT_FALSE(ac.toCommon().turbo);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_EQ(-1, ac.toCommon().sleep);
  ASSERT_EQ(-1, ac.toCommon().clock);
}

TEST(TestSharpAcClass, PreviousPower) {
  IRSharpAc ac(kGpioUnused);
  ac.setPower(false, false);
  EXPECT_FALSE(ac.getPower());
  EXPECT_FALSE(ac.getPreviousPower());
  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());
  EXPECT_FALSE(ac.getPreviousPower());
  ac.setPower(false);
  EXPECT_FALSE(ac.getPower());
  EXPECT_TRUE(ac.getPreviousPower());
  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());
  EXPECT_FALSE(ac.getPreviousPower());
  ac.setPower(true);
  EXPECT_TRUE(ac.getPower());
  EXPECT_TRUE(ac.getPreviousPower());
  ac.setPreviousPower(false);
  EXPECT_TRUE(ac.getPower());
  EXPECT_FALSE(ac.getPreviousPower());
  ac.setPreviousPower(true);
  EXPECT_TRUE(ac.getPower());
  EXPECT_TRUE(ac.getPreviousPower());
  ac.setPower(true, false);
  EXPECT_TRUE(ac.getPower());
  EXPECT_FALSE(ac.getPreviousPower());

  // Data from: https://github.com/crankyoldgit/IRremoteESP8266/pull/1074#discussion_r403407146
  // Command ON (previously OFF) -> 0xAA 5A CF 10 CB 11 22 00 08 80 00 E0 51
  const uint8_t on_prev_off[] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xCB, 0x11, 0x22,
      0x00, 0x08, 0x80, 0x00, 0xE0, 0x51};
  ac.setRaw(on_prev_off);
  EXPECT_TRUE(ac.getPower());
  EXPECT_FALSE(ac.getPreviousPower());
  // Command ON (previously ON)  -> 0xAA 5A CF 10 CB 31 22 00 08 80 04 E0 31
  const uint8_t on_prev_on[] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xCB, 0x31, 0x22,
      0x00, 0x08, 0x80, 0x04, 0xE0, 0x31};
  ac.setRaw(on_prev_on);
  EXPECT_TRUE(ac.getPower());
  EXPECT_TRUE(ac.getPreviousPower());
  // Command OFF (previously ON) -> 0xAA 5A CF 10 CB 21 22 00 08 80 00 E0 61
  const uint8_t off_prev_on[] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xCB, 0x21, 0x22,
      0x00, 0x08, 0x80, 0x00, 0xE0, 0x61};
  ac.setRaw(off_prev_on);
  EXPECT_FALSE(ac.getPower());
  EXPECT_TRUE(ac.getPreviousPower());
  /* Extra test data if needed.
  // Power:OFF<Previously ON> Mode:2(Cool) Fan:2(Auto) Temp:22
  const uint8_t collect1[13] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xC7, 0x21, 0x22,
      0x00, 0x08, 0x80, 0x00, 0xE0, 0xA1};
  // Power:ON<Previously OFF> Mode:2(Cool) Fan:2(Auto) Temp:22
  const uint8_t collect2[13] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xC7, 0x11, 0x22,
      0x00, 0x08, 0x80, 0x00, 0xE0, 0x91};
  // Power:ON Mode:2(Cool) Fan:2(Auto) Temp:23<Previously 22>
  const uint8_t collect3[13] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xC8, 0x31, 0x22,
      0x00, 0x08, 0x80, 0x04, 0xE0, 0x01};
  // Power:ON Mode:2(Cool) Fan:2(Auto) Temp:22<Previously 23>
  const uint8_t collect4[13] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xC7, 0x31, 0x22,
      0x00, 0x08, 0x80, 0x04, 0xE0, 0xF1};
  // Power:ON Mode:3(Dry)<Previously 2(Cool)> Fan:Automaticly 2(Auto)
  // Temp:Automaticly 15
  const uint8_t collect5[13] = {
      0xAA, 0x5A, 0xCF, 0x10, 0x00, 0x31, 0x23,
      0x00, 0x08, 0x80, 0x00, 0xE0, 0x11};
  // Power:ON Mode:2(Cool)<Previously 0(Auto)> Fan:2(Auto) Temp:22
  const uint8_t collect6[13] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xC7, 0x31, 0x22,
      0x00, 0x08, 0x80, 0x00, 0xE0, 0xB1};
  // Power:ON Mode:2(Cool) Fan:3(Medium)<Previously 2(Auto)> Temp:22
  const uint8_t collect7[13] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xC7, 0x31, 0x32,
      0x00, 0x08, 0x80, 0x05, 0xE0, 0xF1};
  // Power:OFF<Previously ON> Mode:2(Cool) Fan:3(Medium) Temp:22
  const uint8_t collect8[13] = {
      0xAA, 0x5A, 0xCF, 0x10, 0xC7, 0x21, 0x32,
      0x00, 0x08, 0x80, 0x00, 0xE0, 0xB1};
  */
}
