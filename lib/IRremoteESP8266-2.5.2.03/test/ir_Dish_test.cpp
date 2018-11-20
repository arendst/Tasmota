// Copyright 2017 David Conran

#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendDISH().

// Test sending typical data only.
TEST(TestSendDish, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendDISH(0x0);
  EXPECT_EQ(
      "m400s6100"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s6100"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s6100"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s6100"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s6100",
      irsend.outputStr());

  irsend.reset();
  irsend.sendDISH(0x9C00);  // Power on.
  EXPECT_EQ(
      "m400s6100"
      "m400s1700m400s2800m400s2800m400s1700m400s1700m400s1700m400s2800m400s2800"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s6100"
      "m400s1700m400s2800m400s2800m400s1700m400s1700m400s1700m400s2800m400s2800"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s6100"
      "m400s1700m400s2800m400s2800m400s1700m400s1700m400s1700m400s2800m400s2800"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s6100"
      "m400s1700m400s2800m400s2800m400s1700m400s1700m400s1700m400s2800m400s2800"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s6100",
      irsend.outputStr());

  irsend.reset();
  irsend.sendDISH(0xFFFF);
  EXPECT_EQ(
      "m400s6100"
      "m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700"
      "m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700"
      "m400s6100"
      "m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700"
      "m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700"
      "m400s6100"
      "m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700"
      "m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700"
      "m400s6100"
      "m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700"
      "m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700m400s1700"
      "m400s6100",
      irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendDish, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendDISH(0x9C00, kDishBits, 0);  // 0 repeats.
  EXPECT_EQ(
      "m400s6100"
      "m400s1700m400s2800m400s2800m400s1700m400s1700m400s1700m400s2800m400s2800"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s6100",
      irsend.outputStr());

  irsend.reset();
  irsend.sendDISH(0x9C00, kDishBits, 1);  // 1 repeat.
  EXPECT_EQ(
      "m400s6100"
      "m400s1700m400s2800m400s2800m400s1700m400s1700m400s1700m400s2800m400s2800"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s6100"
      "m400s1700m400s2800m400s2800m400s1700m400s1700m400s1700m400s2800m400s2800"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s6100",
      irsend.outputStr());

  irsend.sendDISH(0x9C00, kDishBits, 2);  // 2 repeats.
  EXPECT_EQ(
      "m400s6100"
      "m400s1700m400s2800m400s2800m400s1700m400s1700m400s1700m400s2800m400s2800"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s6100"
      "m400s1700m400s2800m400s2800m400s1700m400s1700m400s1700m400s2800m400s2800"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s6100"
      "m400s1700m400s2800m400s2800m400s1700m400s1700m400s1700m400s2800m400s2800"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s6100",
      irsend.outputStr());
}

// Test sending an atypical data size.
TEST(TestSendDish, SendUnusualSize) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendDISH(0x0, 8);
  EXPECT_EQ(
      "m400s6100"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s6100"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s6100"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s6100"
      "m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800m400s2800"
      "m400s6100",
      irsend.outputStr());

  irsend.reset();
  irsend.sendDISH(0x1234567890ABCDEF, 64);
  EXPECT_EQ(
      "m400s6100"
      "m400s2800m400s2800m400s2800m400s1700m400s2800m400s2800m400s1700m400s2800"
      "m400s2800m400s2800m400s1700m400s1700m400s2800m400s1700m400s2800m400s2800"
      "m400s2800m400s1700m400s2800m400s1700m400s2800m400s1700m400s1700m400s2800"
      "m400s2800m400s1700m400s1700m400s1700m400s1700m400s2800m400s2800m400s2800"
      "m400s1700m400s2800m400s2800m400s1700m400s2800m400s2800m400s2800m400s2800"
      "m400s1700m400s2800m400s1700m400s2800m400s1700m400s2800m400s1700m400s1700"
      "m400s1700m400s1700m400s2800m400s2800m400s1700m400s1700m400s2800m400s1700"
      "m400s1700m400s1700m400s1700m400s2800m400s1700m400s1700m400s1700m400s1700"
      "m400s6100"
      "m400s2800m400s2800m400s2800m400s1700m400s2800m400s2800m400s1700m400s2800"
      "m400s2800m400s2800m400s1700m400s1700m400s2800m400s1700m400s2800m400s2800"
      "m400s2800m400s1700m400s2800m400s1700m400s2800m400s1700m400s1700m400s2800"
      "m400s2800m400s1700m400s1700m400s1700m400s1700m400s2800m400s2800m400s2800"
      "m400s1700m400s2800m400s2800m400s1700m400s2800m400s2800m400s2800m400s2800"
      "m400s1700m400s2800m400s1700m400s2800m400s1700m400s2800m400s1700m400s1700"
      "m400s1700m400s1700m400s2800m400s2800m400s1700m400s1700m400s2800m400s1700"
      "m400s1700m400s1700m400s1700m400s2800m400s1700m400s1700m400s1700m400s1700"
      "m400s6100"
      "m400s2800m400s2800m400s2800m400s1700m400s2800m400s2800m400s1700m400s2800"
      "m400s2800m400s2800m400s1700m400s1700m400s2800m400s1700m400s2800m400s2800"
      "m400s2800m400s1700m400s2800m400s1700m400s2800m400s1700m400s1700m400s2800"
      "m400s2800m400s1700m400s1700m400s1700m400s1700m400s2800m400s2800m400s2800"
      "m400s1700m400s2800m400s2800m400s1700m400s2800m400s2800m400s2800m400s2800"
      "m400s1700m400s2800m400s1700m400s2800m400s1700m400s2800m400s1700m400s1700"
      "m400s1700m400s1700m400s2800m400s2800m400s1700m400s1700m400s2800m400s1700"
      "m400s1700m400s1700m400s1700m400s2800m400s1700m400s1700m400s1700m400s1700"
      "m400s6100"
      "m400s2800m400s2800m400s2800m400s1700m400s2800m400s2800m400s1700m400s2800"
      "m400s2800m400s2800m400s1700m400s1700m400s2800m400s1700m400s2800m400s2800"
      "m400s2800m400s1700m400s2800m400s1700m400s2800m400s1700m400s1700m400s2800"
      "m400s2800m400s1700m400s1700m400s1700m400s1700m400s2800m400s2800m400s2800"
      "m400s1700m400s2800m400s2800m400s1700m400s2800m400s2800m400s2800m400s2800"
      "m400s1700m400s2800m400s1700m400s2800m400s1700m400s2800m400s1700m400s1700"
      "m400s1700m400s1700m400s2800m400s2800m400s1700m400s1700m400s2800m400s1700"
      "m400s1700m400s1700m400s1700m400s2800m400s1700m400s1700m400s1700m400s1700"
      "m400s6100",
      irsend.outputStr());
}

// Tests for decodeDISH().

// Decode normal Dish messages.
TEST(TestDecodeDish, NormalDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal Dish 16-bit message.
  irsend.reset();
  irsend.sendDISH(0x9C00);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeDISH(&irsend.capture, kDishBits, true));
  EXPECT_EQ(DISH, irsend.capture.decode_type);
  EXPECT_EQ(kDishBits, irsend.capture.bits);
  EXPECT_EQ(0x9C00, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode Dish messages with unsupported size/lengths.
TEST(TestDecodeDish, DecodeWithNonStrictSize) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  irsend.sendDISH(0x12, 8);  // Illegal size Dish message. (smaller)
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeDISH(&irsend.capture, kDishBits, true));

  irsend.makeDecodeResult();
  // Should fail with strict when we ask for the wrong bit size.
  ASSERT_FALSE(irrecv.decodeDISH(&irsend.capture, 8, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeDISH(&irsend.capture, 8, false));
  EXPECT_EQ(DISH, irsend.capture.decode_type);
  EXPECT_EQ(8, irsend.capture.bits);
  EXPECT_EQ(0x12, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  irsend.reset();
  irsend.sendDISH(0x12345678, 32);  // Illegal size Dish message. (larger)
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeDISH(&irsend.capture, kDishBits, true));

  irsend.makeDecodeResult();
  // Should fail with strict when we ask for the wrong bit size.
  ASSERT_FALSE(irrecv.decodeDISH(&irsend.capture, 32, true));
  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeDISH(&irsend.capture, 32, false));
  EXPECT_EQ(DISH, irsend.capture.decode_type);
  EXPECT_EQ(32, irsend.capture.bits);
  EXPECT_EQ(0x12345678, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

// Decode (non-standard) 64-bit messages.
TEST(TestDecodeDish, Decode64BitMessages) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Illegal value & size Dish 64-bit message.
  irsend.sendDISH(0xFFFFFFFFFFFFFFFF, 64);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeDISH(&irsend.capture, 64, true));
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeDISH(&irsend.capture, 64, false));
  EXPECT_EQ(DISH, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, irsend.capture.value);
}

// Decode a 'real' example via GlobalCache
TEST(TestDecodeDish, DecodeGlobalCacheExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  /*
    irsend.reset();
    // Dish DTV Pal code from Global Cache.
    uint16_t gc_test_dtv[27] = {58000, 1, 3, 22, 538, 22, 252, 22, 156, 22, 156,
                                22, 156, 22, 156, 22, 252, 22, 252, 22, 252, 22,
                                252, 22, 252, 22, 538};
    irsend.sendGC(gc_test_dtv, 27);
    irsend.makeDecodeResult();

    ASSERT_TRUE(irrecv.decodeDISH(&irsend.capture, kDishBits, true));
    EXPECT_EQ(DISH, irsend.capture.decode_type);
    EXPECT_EQ(kDishBits, irsend.capture.bits);
    EXPECT_EQ(0x0, irsend.capture.value);
    EXPECT_EQ(0x0, irsend.capture.address);
    EXPECT_EQ(0x0, irsend.capture.command);
    EXPECT_FALSE(irsend.capture.repeat);

    ASSERT_TRUE(irrecv.decodeDISH(&irsend.capture));
    EXPECT_EQ(DISH, irsend.capture.decode_type);
    EXPECT_EQ(kDishBits, irsend.capture.bits);
    EXPECT_EQ(0x0, irsend.capture.value);
    EXPECT_EQ(0x0, irsend.capture.address);
    EXPECT_EQ(0x0, irsend.capture.command);
    EXPECT_FALSE(irsend.capture.repeat);
  */
  // Dish Hopper 3 code from Global Cache.
  uint16_t gc_test_hopper[73] = {
      58000, 1,  37,  23, 351, 23, 94,  23, 164, 23, 164, 23, 94,  23, 94,  23,
      94,    23, 164, 23, 164, 23, 164, 23, 164, 23, 164, 23, 164, 23, 164, 23,
      164,   23, 164, 23, 164, 23, 351, 23, 94,  23, 164, 23, 164, 23, 94,  23,
      94,    23, 94,  23, 164, 23, 164, 23, 164, 23, 164, 23, 164, 23, 164, 23,
      164,   23, 164, 23, 164, 23, 164, 23, 351};
  irsend.reset();
  irsend.sendGC(gc_test_hopper, 73);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodeDISH(&irsend.capture, kDishBits, true));
  EXPECT_EQ(DISH, irsend.capture.decode_type);
  EXPECT_EQ(kDishBits, irsend.capture.bits);
  EXPECT_EQ(0x9C00, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  ASSERT_TRUE(irrecv.decodeDISH(&irsend.capture));
  EXPECT_EQ(DISH, irsend.capture.decode_type);
  EXPECT_EQ(kDishBits, irsend.capture.bits);
  EXPECT_EQ(0x9C00, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Fail to decode a non-Dish example via GlobalCache
TEST(TestDecodeDish, FailToDecodeNonDishExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Modified a few entries to unexpected values, based on previous test case.
  uint16_t gc_test[39] = {38000, 1,  1,  322, 162, 20, 61,  20, 61, 20,
                          20,    20, 20, 20,  20,  20, 127, 20, 61, 9,
                          20,    20, 61, 20,  20,  20, 61,  20, 61, 20,
                          61,    20, 20, 20,  20,  20, 20,  20, 884};
  irsend.sendGC(gc_test, 39);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeDISH(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeDISH(&irsend.capture, kDishBits, false));
}
