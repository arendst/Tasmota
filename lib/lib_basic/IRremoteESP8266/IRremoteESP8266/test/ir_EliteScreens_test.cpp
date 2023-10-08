// Copyright 2017 David Conran

#include "IRac.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Housekeeping tests

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("ELITESCREENS", typeToString(decode_type_t::ELITESCREENS));
  ASSERT_EQ(decode_type_t::ELITESCREENS, strToDecodeType("ELITESCREENS"));
  ASSERT_FALSE(hasACState(decode_type_t::ELITESCREENS));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::ELITESCREENS));
  ASSERT_EQ(kEliteScreensBits,
            IRsendTest::defaultBits(decode_type_t::ELITESCREENS));
  ASSERT_EQ(kEliteScreensDefaultRepeat,
            IRsendTest::minRepeats(decode_type_t::ELITESCREENS));
}

// Tests for sendElitescreens().

// Test sending typical data only.
TEST(TestSendElitescreens, SendDataOnly) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendElitescreens(0xFEA3387);  // STOP command
  EXPECT_EQ(
      "f38000d50"
      "m1214s470m1214s470m1214s470m1214s470"
      "m470s1214m470s1214m470s1214m470s1214"
      "m470s1214m470s1214m470s1214m1214s470"
      "m470s1214m1214s470m470s1214m1214s470"
      "m1214s470m1214s470m470s1214m470s1214"
      "m1214s470m1214s470m470s1214m470s1214"
      "m470s1214m1214s470m1214s470m1214s470"
      "m1214s470m470s1214m470s1214m470"
      "s30414"
      "m1214s470m1214s470m1214s470m1214s470"
      "m470s1214m470s1214m470s1214m470s1214"
      "m470s1214m470s1214m470s1214m1214s470"
      "m470s1214m1214s470m470s1214m1214s470"
      "m1214s470m1214s470m470s1214m470s1214"
      "m1214s470m1214s470m470s1214m470s1214"
      "m470s1214m1214s470m1214s470m1214s470"
      "m1214s470m470s1214m470s1214m470"
      "s30414",
      irsend.outputStr());
}

// Tests for decodeElitescreens().

// Decode a 'real' example
TEST(TestDecodeElitescreens, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // From https://github.com/crankyoldgit/IRremoteESP8266/issues/1306#issuecomment-715913084
  // STOP (111100000010 0xF02)
  const uint16_t rawData[127] = {
      1278, 456, 1248, 486, 1226, 452, 1252, 454,  // 0 0 0 0 0x0
      474, 1220, 474, 1222, 480, 1214, 476, 1222,  // 1 1 1 1 0xF
      472, 1224, 478, 1216, 476, 1220, 1256, 480,  // 1 1 1 0 0xE
      450, 1216, 1248, 488, 442, 1222, 1252, 486,  // 1 0 1 0 0xA
      1226, 478, 1224, 482, 448, 1220, 472, 1222,  // 0 0 1 1 0x3
      1254, 482, 1220, 486, 444, 1222, 480, 1218,  // 0 0 1 1 0x3
      474, 1220, 1254, 482, 1222, 484, 1218, 488,  // 1 0 0 0 0x8
      1224, 482, 450, 1218, 474, 1220, 470,        // 0 1 1 1 0x7
      30482,
      1246, 460, 1274, 460, 1220, 456, 1246, 488,
      452, 1214, 478, 1218, 474, 1220, 470, 1228,
      476, 1220, 472, 1222, 480, 1214, 1248, 486,
      476, 1190, 1274, 460, 480, 1186, 1278, 460,
      1252, 426, 1276, 456, 474, 1194, 478, 1216,
      1280, 456, 1246, 460, 482, 1186, 474, 1224,
      478, 1216, 1282, 454, 1248, 458, 1244, 462,
      1252, 456, 474, 1192, 480, 1216, 476};  // UNKNOWN 148A4DFF

  irsend.reset();
  irsend.sendRaw(rawData, 127, 38000);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(ELITESCREENS, irsend.capture.decode_type);
  EXPECT_EQ(kEliteScreensBits, irsend.capture.bits);
  EXPECT_EQ(0xFEA3387, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode a Synthetic example
TEST(TestDecodeElitescreens, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendElitescreens(0xFEA3387);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(ELITESCREENS, irsend.capture.decode_type);
  EXPECT_EQ(kEliteScreensBits, irsend.capture.bits);
  EXPECT_EQ(0xFEA3387, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode a 'real' example with no repeat.
TEST(TestDecodeElitescreens, RealExampleNoRepeat) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // From https://github.com/crankyoldgit/IRremoteESP8266/issues/1306#issuecomment-715901468
  // UP
  const uint16_t rawData[63] = {
    1250, 450, 1250, 450, 1250, 500, 1200, 500, 450, 1250, 450, 1200, 500, 1200,
    500, 1200, 500, 1200, 500, 1200, 1250, 450, 500, 1200, 450, 1250, 1250, 450,
    450, 1250, 1250, 450, 1250, 450, 1250, 450, 500, 1200, 1250, 450, 1250, 450,
    1250, 450, 500, 1200, 1250, 450, 1250, 450, 500, 1200, 1250, 450, 500, 1200,
    1250, 450, 500, 1200, 500, 1200, 1250};  // Protocol=UNKNOWN Data=0x2D8CB141

  irsend.reset();
  irsend.sendRaw(rawData, 63, 38000);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(ELITESCREENS, irsend.capture.decode_type);
  EXPECT_EQ(kEliteScreensBits, irsend.capture.bits);
  EXPECT_EQ(0xFDA2256, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}
