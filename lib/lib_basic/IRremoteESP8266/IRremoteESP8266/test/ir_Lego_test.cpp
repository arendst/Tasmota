// Copyright 2019 David Conran

#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// General housekeeping
TEST(TestLego, Housekeeping) {
  ASSERT_EQ("LEGOPF", typeToString(LEGOPF));
  ASSERT_FALSE(hasACState(LEGOPF));  // Uses uint64_t, not uint8_t*.
}

// Tests for sendLego().

// Test sending typical data only.
TEST(TestSendLegoPf, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();

  irsend.reset();
  irsend.sendLegoPf(0x1234);
  EXPECT_EQ(
      "f38000d50"
      "m158s1026"
      "m158s263m158s263m158s263m158s553m158s263m158s263m158s553m158s263"
      "m158s263m158s263m158s553m158s553m158s263m158s553m158s263m158s263"
      "m158s70472", irsend.outputStr());

  irsend.reset();
  irsend.send(LEGOPF, 0x1234, kLegoPfBits);
  EXPECT_EQ(
      "f38000d50"
      "m158s1026"
      "m158s263m158s263m158s263m158s553m158s263m158s263m158s553m158s263"
      "m158s263m158s263m158s553m158s553m158s263m158s553m158s263m158s263"
      "m158s70472", irsend.outputStr());
}

// Test sending typical repeat data.
TEST(TestSendLegoPf, SendDataWithRepeats) {
  IRsendTest irsend(0);
  irsend.begin();

  irsend.reset();
  irsend.sendLegoPf(0x1234, kLegoPfBits, 1);
  EXPECT_EQ(
      "f38000d50"
      "m0s32000"
      "m158s1026"
      "m158s263m158s263m158s263m158s553m158s263m158s263m158s553m158s263"
      "m158s263m158s263m158s553m158s553m158s263m158s553m158s263m158s263"
      "m158s70472"
      "m158s1026"
      "m158s263m158s263m158s263m158s553m158s263m158s263m158s553m158s263"
      "m158s263m158s263m158s553m158s553m158s263m158s553m158s263m158s263"
      "m158s70472"
      "m158s1026"
      "m158s263m158s263m158s263m158s553m158s263m158s263m158s553m158s263"
      "m158s263m158s263m158s553m158s553m158s263m158s553m158s263m158s263"
      "m158s150472"
      "m158s1026"
      "m158s263m158s263m158s263m158s553m158s263m158s263m158s553m158s263"
      "m158s263m158s263m158s553m158s553m158s263m158s553m158s263m158s263"
      "m158s150472"
      "m158s1026"
      "m158s263m158s263m158s263m158s553m158s263m158s263m158s553m158s263"
      "m158s263m158s263m158s553m158s553m158s263m158s553m158s263m158s263"
      "m158s150472", irsend.outputStr());

  irsend.reset();
  irsend.sendLegoPf(0x2345, kLegoPfBits, 2);
  EXPECT_EQ(
      "f38000d50"
      "m0s16000"
      "m158s1026"
      "m158s263m158s263m158s553m158s263m158s263m158s263m158s553m158s553"
      "m158s263m158s553m158s263m158s263m158s263m158s553m158s263m158s553"
      "m158s70182"
      "m158s1026"
      "m158s263m158s263m158s553m158s263m158s263m158s263m158s553m158s553"
      "m158s263m158s553m158s263m158s263m158s263m158s553m158s263m158s553"
      "m158s70182"
      "m158s1026"
      "m158s263m158s263m158s553m158s263m158s263m158s263m158s553m158s553"
      "m158s263m158s553m158s263m158s263m158s263m158s553m158s263m158s553"
      "m158s182182"
      "m158s1026"
      "m158s263m158s263m158s553m158s263m158s263m158s263m158s553m158s553"
      "m158s263m158s553m158s263m158s263m158s263m158s553m158s263m158s553"
      "m158s182182"
      "m158s1026"
      "m158s263m158s263m158s553m158s263m158s263m158s263m158s553m158s553"
      "m158s263m158s553m158s263m158s263m158s263m158s553m158s263m158s553"
      "m158s182182", irsend.outputStr());

  irsend.reset();
  irsend.sendLegoPf(0x3456, kLegoPfBits, 7);
  EXPECT_EQ(
      "f38000d50"
      "m158s1026"
      "m158s263m158s263m158s553m158s553m158s263m158s553m158s263m158s263"
      "m158s263m158s553m158s263m158s553m158s263m158s553m158s553m158s263"
      "m158s69892"
      "m158s1026"
      "m158s263m158s263m158s553m158s553m158s263m158s553m158s263m158s263"
      "m158s263m158s553m158s263m158s553m158s263m158s553m158s553m158s263"
      "m158s69892"
      "m158s1026"
      "m158s263m158s263m158s553m158s553m158s263m158s553m158s263m158s263"
      "m158s263m158s553m158s263m158s553m158s263m158s553m158s553m158s263"
      "m158s213892"
      "m158s1026"
      "m158s263m158s263m158s553m158s553m158s263m158s553m158s263m158s263"
      "m158s263m158s553m158s263m158s553m158s263m158s553m158s553m158s263"
      "m158s213892"
      "m158s1026"
      "m158s263m158s263m158s553m158s553m158s263m158s553m158s263m158s263"
      "m158s263m158s553m158s263m158s553m158s263m158s553m158s553m158s263"
      "m158s213892"
      "m158s1026"
      "m158s263m158s263m158s553m158s553m158s263m158s553m158s263m158s263"
      "m158s263m158s553m158s263m158s553m158s263m158s553m158s553m158s263"
      "m158s213892"
      "m158s1026"
      "m158s263m158s263m158s553m158s553m158s263m158s553m158s263m158s263"
      "m158s263m158s553m158s263m158s553m158s263m158s553m158s553m158s263"
      "m158s213892", irsend.outputStr());
}

// Tests for decodeLego().

// Decode normal "synthetic" messages.
TEST(TestDecodeLegoPf, SyntheticDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  irsend.sendLegoPf(0x000F);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LEGOPF, irsend.capture.decode_type);
  EXPECT_EQ(kLegoPfBits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_EQ(0x000F, irsend.capture.value);
  EXPECT_EQ(1, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);

  irsend.reset();
  irsend.sendLegoPf(0x100E);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LEGOPF, irsend.capture.decode_type);
  EXPECT_EQ(kLegoPfBits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_EQ(0x100E, irsend.capture.value);
  EXPECT_EQ(2, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);

  irsend.reset();
  irsend.sendLegoPf(0x221E);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LEGOPF, irsend.capture.decode_type);
  EXPECT_EQ(kLegoPfBits, irsend.capture.bits);
  EXPECT_FALSE(irsend.capture.repeat);
  EXPECT_EQ(0x221E, irsend.capture.value);
  EXPECT_EQ(3, irsend.capture.address);
  EXPECT_EQ(0x21, irsend.capture.command);

  // Test a bad LRC is not matched.
  irsend.reset();
  irsend.sendLegoPf(0x001F);  // LRC should be 0xE, not 0xF.
  irsend.makeDecodeResult();
  irrecv.decode(&irsend.capture);
  EXPECT_NE(LEGOPF, irsend.capture.decode_type);
}

// Decode normal "synthetic" message with releats.
TEST(TestDecodeLegoPf, SyntheticDecodeWithRepeat) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  irsend.sendLegoPf(0x330F, kLegoPfBits, 1);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LEGOPF, irsend.capture.decode_type);
  EXPECT_EQ(kLegoPfBits, irsend.capture.bits);
  EXPECT_EQ(0x330F, irsend.capture.value);
  EXPECT_EQ(4, irsend.capture.address);
  EXPECT_EQ(0x30, irsend.capture.command);
}
