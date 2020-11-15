// Copyright 2020 crankyoldgit

#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for decodeMetz().

TEST(TestDecodeMetz, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  uint16_t rawbuf[41] = {
      // Header          1         0          1
      880, 2336, 462, 1656, 460, 952, 486, 1630,
      //      1          1         0         0
      462, 1654, 460, 1660, 462, 948, 488, 902,
      //      1         0          1         0
      486, 1656, 486, 926, 486, 1630, 486, 924,
      //      1         0         0          1
      488, 1630, 460, 954, 460, 950, 512, 1606,
      //     0          1         0          1
      460, 978, 436, 1656, 486, 926, 486, 1630,
      458};
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawbuf, 41, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::METZ, irsend.capture.decode_type);
  ASSERT_EQ(kMetzBits, irsend.capture.bits);
  EXPECT_EQ(0x5CA95, irsend.capture.value);
  EXPECT_EQ(0x2A, irsend.capture.command);
  EXPECT_EQ(0x3, irsend.capture.address);
}

TEST(TestDecodeMetz, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  irsend.reset();
  irsend.sendMetz(0x5CA95);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::METZ, irsend.capture.decode_type);
  EXPECT_EQ(kMetzBits, irsend.capture.bits);
  EXPECT_EQ(0x5CA95, irsend.capture.value);
  EXPECT_EQ(0x2A, irsend.capture.command);
  EXPECT_EQ(0x3, irsend.capture.address);

  EXPECT_EQ(
    "f38000d50"
  //  880 2336 462 1656 460 952 486 1630 462 1654 460 1660 462 948 488 902
    "m880s2336m473s1640m473s940m473s1640m473s1640m473s1640m473s940m473s940"
  //  486 1656 486 926 486 1630 486 924 488 1630 460 954 460 950 512 1606
    "m473s1640m473s940m473s1640m473s940m473s1640m473s940m473s940m473s1640"
  //  460 978 436 1656 486 926 486 1630
    "m473s940m473s1640m473s940m473s1640"
  //  458
    "m473s100000",
    irsend.outputStr());
}

TEST(TestEncodeMetz, BasicTests) {
  EXPECT_EQ(0x703F,  IRsend::encodeMetz(0, 0));
  EXPECT_EQ(0x703F,  IRsend::encodeMetz(0, 0, false));
  EXPECT_EQ(0x4703F, IRsend::encodeMetz(0, 0, true));

  EXPECT_EQ(0xE0BD, IRsend::encodeMetz(1, 2, false));
  EXPECT_EQ(0x4E0BD, IRsend::encodeMetz(1, 2, true));

  EXPECT_EQ(0x38FC0, IRsend::encodeMetz(0b111, 0b111111, false));
  EXPECT_EQ(0x78FC0, IRsend::encodeMetz(0b111, 0b111111, true));

  // Out of range.
  EXPECT_EQ(0x38FC0, IRsend::encodeMetz(255, 255, false));
  EXPECT_EQ(0x78FC0, IRsend::encodeMetz(255, 255, true));
}

TEST(TestEncodeMetz, RealExamples) {
  EXPECT_EQ(0x5CA95, IRsend::encodeMetz(0x3, 0x2A, true));
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("METZ", typeToString(decode_type_t::METZ));
  ASSERT_EQ(decode_type_t::METZ, strToDecodeType("METZ"));
  ASSERT_FALSE(hasACState(decode_type_t::METZ));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::METZ));
  ASSERT_EQ(kMetzBits, IRsend::defaultBits(decode_type_t::METZ));
  ASSERT_EQ(kMetzMinRepeat, IRsend::minRepeats(decode_type_t::METZ));
}
