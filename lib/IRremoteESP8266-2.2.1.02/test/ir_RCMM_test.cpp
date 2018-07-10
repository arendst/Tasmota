// Copyright 2017 David Conran

#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for sendRCMM().

// Test sending typical data only.
TEST(TestSendRCMM, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendRCMM(0xe0a600);
  EXPECT_EQ("m416s277"
            "m166s777m166s611m166s277m166s277"
            "m166s611m166s611m166s444m166s611m166s277m166s277m166s277m166s277"
            "m166s27778", irsend.outputStr());
  irsend.reset();
  irsend.sendRCMM(0x28e0a600UL, 32);
  EXPECT_EQ("m416s277"
            "m166s277m166s611m166s611m166s277m166s777m166s611m166s277m166s277"
            "m166s611m166s611m166s444m166s611m166s277m166s277m166s277m166s277"
            "m166s27778", irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendRCMM, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendRCMM(0x28e0a600, 32, 2);  // 2 repeats.
  EXPECT_EQ("m416s277"
            "m166s277m166s611m166s611m166s277m166s777m166s611m166s277m166s277"
            "m166s611m166s611m166s444m166s611m166s277m166s277m166s277m166s277"
            "m166s27778"
            "m416s277"
            "m166s277m166s611m166s611m166s277m166s777m166s611m166s277m166s277"
            "m166s611m166s611m166s444m166s611m166s277m166s277m166s277m166s277"
            "m166s27778"
            "m416s277"
            "m166s277m166s611m166s611m166s277m166s777m166s611m166s277m166s277"
            "m166s611m166s611m166s444m166s611m166s277m166s277m166s277m166s277"
            "m166s27778", irsend.outputStr());
}

// Test sending an atypical data size.
TEST(TestSendRCMM, SendUsualSize) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendRCMM(0xE0, 8);
  EXPECT_EQ("m416s277"
            "m166s777m166s611m166s277m166s277"
            "m166s27778", irsend.outputStr());
  irsend.reset();
  irsend.sendRCMM(0x28e0a60000UL, 40);
  EXPECT_EQ("m416s277"
            "m166s277m166s611m166s611m166s277m166s777m166s611m166s277m166s277"
            "m166s611m166s611m166s444m166s611m166s277m166s277m166s277m166s277"
            "m166s277m166s277m166s277m166s277m166s27778", irsend.outputStr());
}

// Tests for decodeRCMM().

// Decode normal RCMM messages.
TEST(TestDecodeRCMM, NormalDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal RCMM 24-bit message.
  irsend.reset();
  irsend.sendRCMM(0xe0a600);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRCMM(&irsend.capture, RCMM_BITS, true));
  EXPECT_EQ(RCMM, irsend.capture.decode_type);
  EXPECT_EQ(RCMM_BITS, irsend.capture.bits);
  EXPECT_EQ(0xe0a600, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal RCMM 12-bit message.
  irsend.reset();
  irsend.sendRCMM(0x600, 12);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRCMM(&irsend.capture, 12, true));
  EXPECT_EQ(RCMM, irsend.capture.decode_type);
  EXPECT_EQ(12, irsend.capture.bits);
  EXPECT_EQ(0x600, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal RCMM 32-bit message.
  irsend.reset();
  irsend.sendRCMM(0x28e0a600, 32);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRCMM(&irsend.capture, 32, true));
  EXPECT_EQ(RCMM, irsend.capture.decode_type);
  EXPECT_EQ(32, irsend.capture.bits);
  EXPECT_EQ(0x28e0a600, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decodes should fail for illegal bit sizes when in strict mode.
TEST(TestDecodeRCMM, IllegalDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Illegal RCMM 8-bit message.
  irsend.reset();
  irsend.sendRCMM(0x0, 8);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeRCMM(&irsend.capture, 8, true));
  ASSERT_FALSE(irrecv.decodeRCMM(&irsend.capture, 12, true));

  // Illegal RCMM 36-bit message.
  irsend.reset();
  irsend.sendRCMM(0x0, 36);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeRCMM(&irsend.capture, 12, true));
  ASSERT_FALSE(irrecv.decodeRCMM(&irsend.capture, 36, true));
}

// Decodes without strict mode.
TEST(TestDecodeRCMM, DecodeWithoutStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Illegal RCMM 8-bit message.
  irsend.reset();
  irsend.sendRCMM(0x55, 8);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRCMM(&irsend.capture, 8, false));
  EXPECT_EQ(RCMM, irsend.capture.decode_type);
  EXPECT_EQ(8, irsend.capture.bits);
  EXPECT_EQ(0x55, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  ASSERT_TRUE(irrecv.decodeRCMM(&irsend.capture, 12, false));
  EXPECT_EQ(RCMM, irsend.capture.decode_type);
  EXPECT_EQ(8, irsend.capture.bits);
  EXPECT_EQ(0x55, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  // Illegal RCMM 36-bit message.
  irsend.reset();
  irsend.sendRCMM(0x123456789, 36);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeRCMM(&irsend.capture, 12, false));
  EXPECT_EQ(RCMM, irsend.capture.decode_type);
  EXPECT_EQ(36, irsend.capture.bits);
  EXPECT_EQ(0x123456789, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  ASSERT_TRUE(irrecv.decodeRCMM(&irsend.capture, 24, false));
  EXPECT_EQ(36, irsend.capture.bits);
  EXPECT_EQ(0x123456789, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  ASSERT_TRUE(irrecv.decodeRCMM(&irsend.capture, 36, false));
  EXPECT_EQ(RCMM, irsend.capture.decode_type);
  EXPECT_EQ(36, irsend.capture.bits);
  EXPECT_EQ(0x123456789, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

// Decode (non-standard) 64-bit messages.
TEST(TestDecodeRCMM, Decode64BitMessages) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Illegal value & size RCMM 64-bit message.
  irsend.sendRCMM(0xFEDCBA9876543210, 64);
  irsend.makeDecodeResult();
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeRCMM(&irsend.capture, 64, false));
  EXPECT_EQ(RCMM, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0xFEDCBA9876543210, irsend.capture.value);
  EXPECT_EQ(0, irsend.capture.address);
  EXPECT_EQ(0, irsend.capture.command);
}

// Fail to decode a non-RCMM example via GlobalCache
TEST(TestDecodeRCMM, FailToDecodeNonRCMMExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Modified a few entries to unexpected values, based on previous test case.
  uint16_t gc_test[39] = {38000, 1, 1, 322, 162, 20, 61, 20, 61, 20, 20, 20, 20,
                          20, 20, 20, 127, 20, 61, 9, 20, 20, 61, 20, 20, 20,
                          61, 20, 61, 20, 61, 20, 20, 20, 20, 20, 20, 20, 884};
  irsend.sendGC(gc_test, 39);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeRCMM(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeRCMM(&irsend.capture, RCMM_BITS, false));
}

// Issue 281 Debugging
TEST(TestDecodeRCMM, DebugIssue281) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Data from Issue #281 (shortened version)
  uint16_t rawData[36] = {448, 276, 150, 285, 164, 613, 163, 447, 162, 613,
                          164, 445, 164, 776, 167, 278, 163, 280, 163, 280,
                          162, 611, 168, 444, 163, 612, 164, 277, 168, 447,
                          157, 282, 165, 276,
                          165, 65535};  // Last value modified from 89729

  irsend.reset();
  irsend.sendRaw(rawData, 36, 36);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(RCMM, irsend.capture.decode_type);
  EXPECT_EQ(32, irsend.capture.bits);
  EXPECT_EQ(0x26702610, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}
