// Copyright 2021 crankyoldgit

#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for decodeAirton().

TEST(TestDecodeAirton, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  const uint16_t rawData[115] = {
      6632, 3352,
      404, 1266, 404, 1264, 406, 430, 406, 430, 400, 1264, 406, 430, 402, 1264,
      408, 1262, 406, 1264, 404, 430, 402, 434, 402, 432, 402, 1264, 406, 430,
      404, 432, 400, 456, 376, 432, 402, 430, 402, 1264, 404, 1264, 404, 432,
      402, 434, 398, 434, 402, 430, 404, 1264, 404, 432, 402, 430, 404, 1264,
      406, 430, 402, 432, 400, 434, 402, 430, 402, 430, 404, 432, 402, 430,
      402, 432, 402, 432, 402, 430, 402, 432, 402, 430, 402, 434, 400, 432,
      402, 1264, 404, 430, 404, 1264, 404, 432, 402, 454, 378, 432, 402, 430,
      404, 1264, 404, 1264, 404, 1264, 378, 1292, 404, 432, 402, 1264, 404, 432,
      402};
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 115, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::AIRTON, irsend.capture.decode_type);
  ASSERT_EQ(kAirtonBits, irsend.capture.bits);
  EXPECT_EQ(0x5E1400090C11D3, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

TEST(TestDecodeAirton, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  irsend.reset();
  irsend.sendAirton(0x5E1400090C11D3);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::AIRTON, irsend.capture.decode_type);
  ASSERT_EQ(kAirtonBits, irsend.capture.bits);
  EXPECT_EQ(0x5E1400090C11D3, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("AIRTON", typeToString(decode_type_t::AIRTON));
  ASSERT_EQ(decode_type_t::AIRTON, strToDecodeType("AIRTON"));
  ASSERT_FALSE(hasACState(decode_type_t::AIRTON));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::AIRTON));
  ASSERT_EQ(kAirtonBits, IRsend::defaultBits(decode_type_t::AIRTON));
  ASSERT_EQ(kAirtonDefaultRepeat, IRsend::minRepeats(decode_type_t::AIRTON));
}
