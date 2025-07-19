// Copyright 2022 David Conran

#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("WOWWEE", typeToString(decode_type_t::WOWWEE));
  ASSERT_EQ(decode_type_t::WOWWEE, strToDecodeType("WOWWEE"));
  ASSERT_FALSE(hasACState(decode_type_t::WOWWEE));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::WOWWEE));
  ASSERT_EQ(kWowweeBits, IRsend::defaultBits(decode_type_t::WOWWEE));
  ASSERT_EQ(kWowweeDefaultRepeat, IRsend::minRepeats(decode_type_t::WOWWEE));
}

// Tests for sendWowwee().
// Test sending typical data only.
TEST(TestSendWowwee, SendDataOnly) {
  IRsendTest irsend(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendWowwee(0x186);  // Nikai TV Power Off.
  EXPECT_EQ(
      "f38000d33"
      "m6684s723"
      "m912s723m912s723m912s3259m912s3259m912s723m912s723m912s723m912s723"
      "m912s3259m912s3259m912s723m912s100000",
      irsend.outputStr());

  irsend.reset();
}

// Tests for decodeWowwee().

// Decode normal Wowwee messages.
TEST(TestDecodeWowwee, RealDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Ref:  https://github.com/crankyoldgit/IRremoteESP8266/issues/1938#issue-1513240242
  const uint16_t rawForward[25] = {
      6684, 740, 918, 724, 942, 724, 918, 3250, 870, 3268, 872, 770, 940, 690,
      942, 688, 942, 738, 942, 3250, 868, 3268, 872, 732, 918
      };  // UNKNOWN 7469BF81
  irsend.reset();
  irsend.sendRaw(rawForward, 25, 38);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::WOWWEE, irsend.capture.decode_type);
  EXPECT_EQ(kWowweeBits, irsend.capture.bits);
  EXPECT_EQ(0x186, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_EQ(0x0, irsend.capture.address);

  // Ref:  https://github.com/crankyoldgit/IRremoteESP8266/issues/1938#issue-1513240242
  const uint16_t rawLeft[25] = {
      6630, 764, 868, 762, 892, 788, 866, 3324, 792, 3348, 818, 760, 866, 788,
      894, 772, 892, 750, 870, 786, 920, 750, 864, 776, 868
      };  // UNKNOWN 28A1120F
  irsend.reset();
  irsend.sendRaw(rawLeft, 25, 38);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::WOWWEE, irsend.capture.decode_type);
  EXPECT_EQ(kWowweeBits, irsend.capture.bits);
  EXPECT_EQ(0x180, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_EQ(0x0, irsend.capture.address);
}

// Decode normal repeated Wowwee messages.
TEST(TestDecodeWowwee, SyntheticDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Normal Wowwee 11-bit message.
  irsend.reset();
  irsend.sendWowwee(0x186);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::WOWWEE, irsend.capture.decode_type);
  EXPECT_EQ(kWowweeBits, irsend.capture.bits);
  EXPECT_EQ(0x186, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_EQ(0x0, irsend.capture.address);

  // Normal Wowwee 11-bit message.
  irsend.reset();
  irsend.sendWowwee(0x180);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::WOWWEE, irsend.capture.decode_type);
  EXPECT_EQ(kWowweeBits, irsend.capture.bits);
  EXPECT_EQ(0x180, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_EQ(0x0, irsend.capture.address);
}
