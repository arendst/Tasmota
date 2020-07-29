// Copyright 2017 David Conran

#include "IRrecv_test.h"
#include "IRrecv.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for the IRrecv object.
TEST(TestIRrecv, DefaultBufferSize) {
  IRrecv irrecv_default(1);
  EXPECT_EQ(kRawBuf, irrecv_default.getBufSize());
}

TEST(TestIRrecv, LargeBufferSize) {
  IRrecv irrecv_large(3, 1024);
  EXPECT_EQ(1024, irrecv_large.getBufSize());
}

TEST(TestIRrecv, SmallBufferSize) {
  IRrecv irrecv_small(4, 80);
  EXPECT_EQ(80, irrecv_small.getBufSize());
}

TEST(TestIRrecv, MediumBufferSize) {
  IRrecv irrecv_medium(4, 512);
  EXPECT_EQ(512, irrecv_medium.getBufSize());
}

TEST(TestIRrecv, IRrecvDestructor) {
  IRrecv *irrecv_ptr = new IRrecv(1);
  EXPECT_EQ(kRawBuf, irrecv_ptr->getBufSize());

  delete irrecv_ptr;
  irrecv_ptr = new IRrecv(1, 1234);
  EXPECT_EQ(1234, irrecv_ptr->getBufSize());
  delete irrecv_ptr;

  irrecv_ptr = new IRrecv(1, 123);
  EXPECT_EQ(123, irrecv_ptr->getBufSize());
  delete irrecv_ptr;
}

// Tests for copyIrParams()

TEST(TestCopyIrParams, CopyEmpty) {
  irparams_t src;
  irparams_t dst;
  uint16_t test_size = 1234;
  src.bufsize = test_size;
  src.rawlen = 0;
  src.rawbuf = new uint16_t[test_size];
  src.overflow = false;
  dst.bufsize = 4567;
  dst.rawlen = 123;
  dst.rawbuf = new uint16_t[test_size];
  dst.overflow = true;
  // Confirm we are looking at different memory for the buffers.
  ASSERT_NE(src.rawbuf, dst.rawbuf);

  IRrecv irrecv(4);
  irrecv.copyIrParams(&src, &dst);

  ASSERT_EQ(src.bufsize, dst.bufsize);
  ASSERT_EQ(src.rawlen, dst.rawlen);
  ASSERT_NE(src.rawbuf, dst.rawbuf);  // Pointers, not content.
  ASSERT_EQ(src.overflow, dst.overflow);
  // Contents of the buffers needs to match.
  EXPECT_EQ(0, memcmp(src.rawbuf, dst.rawbuf, src.bufsize * sizeof(uint16_t)));
}

TEST(TestCopyIrParams, CopyNonEmpty) {
  irparams_t src;
  irparams_t dst;
  uint16_t test_size = 1234;
  src.bufsize = test_size;
  src.rawlen = 67;
  src.rawbuf = new uint16_t[test_size];
  src.rawbuf[0] = 0xF00D;
  src.rawbuf[1] = 0xBEEF;
  src.rawbuf[test_size - 1] = 0xDEAD;
  src.overflow = true;
  dst.bufsize = 0;
  dst.rawlen = 0;
  dst.rawbuf = new uint16_t[test_size];
  dst.overflow = false;
  // Confirm we are looking at different memory for the buffers.
  ASSERT_NE(src.rawbuf, dst.rawbuf);
  // and that they differ before we test.
  EXPECT_NE(0, memcmp(src.rawbuf, dst.rawbuf, src.bufsize * sizeof(uint16_t)));

  IRrecv irrecv(4);
  irrecv.copyIrParams(&src, &dst);

  ASSERT_EQ(src.bufsize, dst.bufsize);
  EXPECT_EQ(test_size, dst.bufsize);
  ASSERT_EQ(src.rawlen, dst.rawlen);
  EXPECT_EQ(67, dst.rawlen);
  ASSERT_EQ(src.overflow, dst.overflow);
  EXPECT_TRUE(dst.overflow);
  ASSERT_NE(src.rawbuf, dst.rawbuf);  // Pointers, not content.
  // Contents of the buffers needs to match.
  EXPECT_EQ(0, memcmp(src.rawbuf, dst.rawbuf, src.bufsize * sizeof(uint16_t)));
  // Check the canary values.
  EXPECT_EQ(0xF00D, dst.rawbuf[0]);
  EXPECT_EQ(0xBEEF, dst.rawbuf[1]);
  EXPECT_EQ(0xDEAD, dst.rawbuf[test_size - 1]);
}

// Tests for decode().

// Test decode of a NEC message.
TEST(TestDecode, DecodeNEC) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();
  irsend.reset();
  irsend.sendNEC(0x807F40BF);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x807F40BF, irsend.capture.value);
}

// Test decode of a JVC message.
TEST(TestDecode, DecodeJVC) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();
  irsend.reset();
  irsend.sendJVC(0xC2B8);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(JVC, irsend.capture.decode_type);
  EXPECT_EQ(kJvcBits, irsend.capture.bits);
  EXPECT_EQ(0xC2B8, irsend.capture.value);
}

// Test decode of a LG message.
TEST(TestDecode, DecodeLG) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();
  irsend.reset();
  irsend.sendLG(0x4B4AE51);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(kLgBits, irsend.capture.bits);
  EXPECT_EQ(0x4B4AE51, irsend.capture.value);

  irsend.reset();
  irsend.sendLG(0xB4B4AE51, kLg32Bits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(kLg32Bits, irsend.capture.bits);
  EXPECT_EQ(0xB4B4AE51, irsend.capture.value);
}

// Test decode of a Panasonic message.
TEST(TestDecode, DecodePanasonic) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();
  irsend.reset();
  irsend.sendPanasonic64(0x40040190ED7C);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodePanasonic(&irsend.capture, kStartOffset,
                                     kPanasonicBits, true));
  EXPECT_EQ(PANASONIC, irsend.capture.decode_type);
  EXPECT_EQ(kPanasonicBits, irsend.capture.bits);
  EXPECT_EQ(0x40040190ED7C, irsend.capture.value);
}

// Test decode of a Samsun message.
TEST(TestDecode, DecodeSamsung) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();
  irsend.reset();
  irsend.sendSAMSUNG(0xE0E09966);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SAMSUNG, irsend.capture.decode_type);
  EXPECT_EQ(kSamsungBits, irsend.capture.bits);
  EXPECT_EQ(0xE0E09966, irsend.capture.value);
}

// Test decode of a Sherwood message.
TEST(TestDecode, DecodeSherwood) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();
  irsend.reset();
  irsend.sendSherwood(0x807F40BF);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  // Sherwood codes are really NEC codes.
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x807F40BF, irsend.capture.value);
}

// Test decode of a Whynter message.
TEST(TestDecode, DecodeWhynter) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();
  irsend.reset();
  irsend.sendWhynter(0x87654321);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(WHYNTER, irsend.capture.decode_type);
  EXPECT_EQ(kWhynterBits, irsend.capture.bits);
  EXPECT_EQ(0x87654321, irsend.capture.value);
}

// Test decode of Sony messages.
TEST(TestDecode, DecodeSony) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  // Synthesised Normal Sony 20-bit message.
  irsend.reset();
  irsend.sendSony(irsend.encodeSony(kSony20Bits, 0x1, 0x1, 0x1));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(kSony20Bits, irsend.capture.bits);
  EXPECT_EQ(0x81080, irsend.capture.value);

  // Synthesised Normal Sony 15-bit message.
  irsend.reset();
  irsend.sendSony(irsend.encodeSony(kSony15Bits, 21, 1), kSony15Bits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(kSony15Bits, irsend.capture.bits);
  EXPECT_EQ(0x5480, irsend.capture.value);

  // Synthesised Normal Sony 12-bit message.
  irsend.reset();
  irsend.sendSony(irsend.encodeSony(kSony12Bits, 21, 1), kSony12Bits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SONY, irsend.capture.decode_type);
  EXPECT_EQ(kSony12Bits, irsend.capture.bits);
  EXPECT_EQ(0xA90, irsend.capture.value);
}

// Test decode of Sharp messages.
TEST(TestDecode, DecodeSharp) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();
  irsend.reset();
  irsend.sendSharpRaw(0x454A);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SHARP, irsend.capture.decode_type);
  EXPECT_EQ(kSharpBits, irsend.capture.bits);
  EXPECT_EQ(0x454A, irsend.capture.value);
}

// Test decode of Sanyo messages.
TEST(TestDecode, DecodeSanyo) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();
  irsend.reset();
  irsend.sendSanyoLC7461(0x2468DCB56A9);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(SANYO_LC7461, irsend.capture.decode_type);
  EXPECT_EQ(kSanyoLC7461Bits, irsend.capture.bits);
  EXPECT_EQ(0x2468DCB56A9, irsend.capture.value);
}

// Test decode of RC-MM messages.
TEST(TestDecode, DecodeRCMM) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  // Normal RCMM 24-bit message.
  irsend.reset();
  irsend.sendRCMM(0xe0a600);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(RCMM, irsend.capture.decode_type);
  EXPECT_EQ(kRCMMBits, irsend.capture.bits);
  EXPECT_EQ(0xe0a600, irsend.capture.value);

  // Normal RCMM 12-bit message.
  irsend.reset();
  irsend.sendRCMM(0x600, 12);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(RCMM, irsend.capture.decode_type);
  EXPECT_EQ(12, irsend.capture.bits);
  EXPECT_EQ(0x600, irsend.capture.value);

  // Normal RCMM 32-bit message.
  irsend.reset();
  irsend.sendRCMM(0x28e0a600, 32);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(RCMM, irsend.capture.decode_type);
  EXPECT_EQ(32, irsend.capture.bits);
  EXPECT_EQ(0x28e0a600, irsend.capture.value);
}

// Test decode of Mitsubishi messages.
TEST(TestDecode, DecodeMitsubishi) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();
  irsend.reset();
  irsend.sendMitsubishi(0xC2B8);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(MITSUBISHI, irsend.capture.decode_type);
  EXPECT_EQ(kMitsubishiBits, irsend.capture.bits);
  EXPECT_EQ(0xC2B8, irsend.capture.value);
}

// Test decode of RC-5/RC-5X messages.
TEST(TestDecode, DecodeRC5) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();
  // Normal RC-5 12-bit message.
  irsend.reset();
  irsend.sendRC5(0x175);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(RC5, irsend.capture.decode_type);
  EXPECT_EQ(kRC5Bits, irsend.capture.bits);
  EXPECT_EQ(0x175, irsend.capture.value);
  // Synthesised Normal RC-5X 13-bit message.
  irsend.reset();
  irsend.sendRC5(irsend.encodeRC5X(0x02, 0x41, true), kRC5XBits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(RC5X, irsend.capture.decode_type);
  EXPECT_EQ(kRC5XBits, irsend.capture.bits);
  EXPECT_EQ(0x1881, irsend.capture.value);
}

// Test decode of RC-6 messages.
TEST(TestDecode, DecodeRC6) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();
  // Normal RC-6 Mode 0 (20-bit) message.
  irsend.reset();
  irsend.sendRC6(0x175);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(RC6, irsend.capture.decode_type);
  EXPECT_EQ(kRC6Mode0Bits, irsend.capture.bits);
  EXPECT_EQ(0x175, irsend.capture.value);

  // Normal RC-6 36-bit message.
  irsend.reset();
  irsend.sendRC6(0xC800F742A, kRC6_36Bits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(RC6, irsend.capture.decode_type);
  EXPECT_EQ(kRC6_36Bits, irsend.capture.bits);
  EXPECT_EQ(0xC800F742A, irsend.capture.value);
}

// Test decode of Dish messages.
TEST(TestDecode, DecodeDish) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();
  irsend.reset();
  irsend.sendDISH(0x9C00);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(DISH, irsend.capture.decode_type);
  EXPECT_EQ(kDishBits, irsend.capture.bits);
  EXPECT_EQ(0x9C00, irsend.capture.value);
}

// Test decode of Denon messages.
TEST(TestDecode, DecodeDenon) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();
  // Normal Denon 15-bit message. (Sharp)
  irsend.reset();
  irsend.sendDenon(0x2278);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(DENON, irsend.capture.decode_type);
  EXPECT_EQ(kDenonBits, irsend.capture.bits);
  EXPECT_EQ(0x2278, irsend.capture.value);
  // Legacy Denon 14-bit message.
  irsend.reset();
  irsend.sendDenon(0x1278, kDenonLegacyBits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(DENON, irsend.capture.decode_type);
  EXPECT_EQ(kDenonBits, irsend.capture.bits);
  EXPECT_EQ(0x1278, irsend.capture.value);
  // Normal Denon 48-bit message. (Panasonic/Kaseikyo)
  irsend.reset();
  irsend.sendDenon(0x2A4C028D6CE3, kDenon48Bits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(DENON, irsend.capture.decode_type);
  EXPECT_EQ(kDenon48Bits, irsend.capture.bits);
  EXPECT_EQ(0x2A4C028D6CE3, irsend.capture.value);
}

// Test decode of Coolix messages.
TEST(TestDecode, DecodeCoolix) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();
  irsend.reset();
  irsend.sendCOOLIX(0x123456);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(COOLIX, irsend.capture.decode_type);
  EXPECT_EQ(kCoolixBits, irsend.capture.bits);
  EXPECT_EQ(0x123456, irsend.capture.value);
}

// Test decode of Aiwa messages.
TEST(TestDecode, DecodeAiwa) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();
  irsend.reset();
  irsend.sendAiwaRCT501(0x7F);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(AIWA_RC_T501, irsend.capture.decode_type);
  EXPECT_EQ(kAiwaRcT501Bits, irsend.capture.bits);
  EXPECT_EQ(0x7F, irsend.capture.value);
}

// Test matchData() on space encoded data.
TEST(TestMatchData, SpaceEncoded) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  uint16_t space_encoded_raw[11] = {500, 500,  500, 1500, 499, 499,
                                    501, 1501, 499, 1490, 500};
  match_result_t result;

  irsend.reset();
  irsend.sendRaw(space_encoded_raw, 11, 38000);
  irsend.makeDecodeResult();
  result = irrecv.matchData(irsend.capture.rawbuf + 1, 5, 500, 1500, 500, 500);
  ASSERT_TRUE(result.success);
  EXPECT_EQ(0b01011, result.data);
  EXPECT_EQ(10, result.used);

  irsend.reset();
  irsend.sendRaw(space_encoded_raw, 11, 38000);
  irsend.makeDecodeResult();
  result = irrecv.matchData(irsend.capture.rawbuf + 1, 5, 500, 1000, 500, 500);
  ASSERT_FALSE(result.success);
}

// Test matchData() on mark encoded data.
TEST(TestMatchData, MarkEncoded) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  uint16_t mark_encoded_raw[11] = {500,  500, 1500, 500, 499, 499,
                                   1501, 501, 1499, 490, 500};
  match_result_t result;

  irsend.reset();
  irsend.sendRaw(mark_encoded_raw, 11, 38000);
  irsend.makeDecodeResult();
  // MSBF order.
  result = irrecv.matchData(irsend.capture.rawbuf + 1, 5, 1500, 500, 500, 500);
  ASSERT_TRUE(result.success);
  EXPECT_EQ(0b01011, result.data);
  EXPECT_EQ(10, result.used);
  // LSBF order.
  result = irrecv.matchData(irsend.capture.rawbuf + 1, 5, 1500, 500, 500, 500,
                            kTolerance, kMarkExcess, false);
  ASSERT_TRUE(result.success);
  EXPECT_EQ(0b11010, result.data);  // Bits reversed of the previous test.
  EXPECT_EQ(10, result.used);

  irsend.reset();
  irsend.sendRaw(mark_encoded_raw, 11, 38000);
  irsend.makeDecodeResult();
  // MSBF order.
  result = irrecv.matchData(irsend.capture.rawbuf + 1, 5, 1000, 500, 500, 500);
  ASSERT_FALSE(result.success);
  // LSBF order.
  result = irrecv.matchData(irsend.capture.rawbuf + 1, 5, 1000, 500, 500, 500,
                            kTolerance, kMarkExcess, false);
  ASSERT_FALSE(result.success);
}

// Test matchData() on "equal total bit time" encoded data.
TEST(TestMatchData, EqualTotalBitTimeEncoded) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  uint16_t equal_encoded_raw[11] = {500,  1500, 1500, 500, 499, 1499,
                                    1501, 501,  1499, 490, 500};
  match_result_t result;

  irsend.reset();
  irsend.sendRaw(equal_encoded_raw, 11, 38000);
  irsend.makeDecodeResult();
  result = irrecv.matchData(irsend.capture.rawbuf + 1, 5, 1500, 500, 500, 1500);
  ASSERT_TRUE(result.success);
  EXPECT_EQ(0b01011, result.data);
  EXPECT_EQ(10, result.used);

  irsend.reset();
  irsend.sendRaw(equal_encoded_raw, 11, 38000);
  irsend.makeDecodeResult();
  result = irrecv.matchData(irsend.capture.rawbuf + 1, 5, 1000, 500, 500, 1000);
  ASSERT_FALSE(result.success);
}

// Test matchData() on arbitrary encoded data.
TEST(TestMatchData, ArbitraryEncoded) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  uint16_t arbitrary_encoded_raw[11] = {500,  1500, 3000, 1000, 499, 1499,
                                        3001, 1001, 2999, 990,  500};
  match_result_t result;

  irsend.reset();
  irsend.sendRaw(arbitrary_encoded_raw, 11, 38000);
  irsend.makeDecodeResult();
  result =
      irrecv.matchData(irsend.capture.rawbuf + 1, 5, 3000, 1000, 500, 1500);
  ASSERT_TRUE(result.success);
  EXPECT_EQ(0b01011, result.data);
  EXPECT_EQ(10, result.used);

  irsend.reset();
  irsend.sendRaw(arbitrary_encoded_raw, 11, 38000);
  irsend.makeDecodeResult();
  result = irrecv.matchData(irsend.capture.rawbuf + 1, 5, 1000, 500, 500, 1000);
  ASSERT_FALSE(result.success);
}

TEST(TestMatchGeneric, NormalWithNoAtleast) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  const uint16_t kgood_entries_trailing_space = 12;
  uint16_t good_trailing_space_data[kgood_entries_trailing_space] = {
      8000,  // Header mark
      4000,  // Header space
      500, 2000,  // Bit #0 (1)
      500, 1000,  // Bit #1 (0)
      500, 2000,  // Bit #2 (1)
      500, 1000,  // Bit #3 (0)
      3000,  // Footer mark
      15000};  // Footer space

  const uint16_t kgood_entries_no_trailing_space = 11;
  uint16_t good_no_trailing_space_data[kgood_entries_no_trailing_space] = {
      8000,  // Header mark
      4000,  // Header space
      500, 2000,  // Bit #0 (1)
      500, 1000,  // Bit #1 (0)
      500, 2000,  // Bit #2 (1)
      500, 1000,  // Bit #3 (0)
      3000};  // Footer mark  (No Footer space.)

  uint16_t offset = kStartOffset;
  irsend.reset();
  irsend.sendRaw(good_trailing_space_data, kgood_entries_trailing_space, 38000);
  irsend.makeDecodeResult();
  uint64_t result_data = 0;
  uint16_t entries_used = 0;
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      4,  // nbits
      8000, 4000,  // Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      3000, 15000,  // Footer
      false,  // No atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_NE(0, entries_used);
  EXPECT_EQ(0b1010, result_data);
  EXPECT_EQ(irsend.capture.rawlen - kStartOffset, entries_used);
  EXPECT_EQ(kgood_entries_trailing_space, entries_used);

  // Same again but with a footer space mis-match, which should fail.
  result_data = 0;
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      4,  // nbits
      8000, 4000,  // Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      3000, 30000,  // Footer
      false,  // no atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_EQ(0, entries_used);

  // Same again as first part but with no footer space data as the last entry.
  irsend.reset();
  result_data = 0;
  irsend.sendRaw(good_no_trailing_space_data, kgood_entries_no_trailing_space,
                 38000);
  irsend.makeDecodeResult();
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      4,  // nbits
      8000, 4000,  // Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      3000, 15000,  // Footer
      false,  // No atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_NE(0, entries_used);
  EXPECT_EQ(0b1010, result_data);
  EXPECT_EQ(irsend.capture.rawlen - kStartOffset, entries_used);
  EXPECT_EQ(kgood_entries_no_trailing_space, entries_used);
}


TEST(TestMatchGeneric, NormalWithAtleast) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  const uint16_t kgood_entries_trailing_space = 12;
  uint16_t good_trailing_space_data[kgood_entries_trailing_space] = {
      8000,  // Header mark
      4000,  // Header space
      500, 2000,  // Bit #0 (1)
      500, 1000,  // Bit #1 (0)
      500, 2000,  // Bit #2 (1)
      500, 1000,  // Bit #3 (0)
      3000,  // Footer mark
      15000};  // Footer space

  const uint16_t kgood_entries_no_trailing_space = 11;
  uint16_t good_no_trailing_space_data[kgood_entries_no_trailing_space] = {
      8000,  // Header mark
      4000,  // Header space
      500, 2000,  // Bit #0 (1)
      500, 1000,  // Bit #1 (0)
      500, 2000,  // Bit #2 (1)
      500, 1000,  // Bit #3 (0)
      3000};  // Footer mark  (No Footer space.)

  uint16_t offset = kStartOffset;
  irsend.reset();
  irsend.sendRaw(good_trailing_space_data, kgood_entries_trailing_space, 38000);
  irsend.makeDecodeResult();
  uint64_t result_data = 0;
  uint16_t entries_used = 0;
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      4,  // nbits
      8000, 4000,  // Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      3000, 15000,  // Footer
      true,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_NE(0, entries_used);
  EXPECT_EQ(0b1010, result_data);
  EXPECT_EQ(irsend.capture.rawlen - kStartOffset, entries_used);
  EXPECT_EQ(kgood_entries_trailing_space, entries_used);

  // Same again but with a footer space under-match.
  result_data = 0;
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      4,  // nbits
      8000, 4000,  // Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      3000, 1500,  // Footer
      true,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_NE(0, entries_used);
  EXPECT_EQ(0b1010, result_data);
  EXPECT_EQ(irsend.capture.rawlen - kStartOffset, entries_used);
  EXPECT_EQ(kgood_entries_trailing_space, entries_used);

  // Same again but with a footer space under-match using less bits so the
  // atleast footer isn't the last entry in the buffer.
  result_data = 0;
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      3,  // nbits (1 less than normal)
      8000, 4000,  // Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      500, 500,  // Footer
      true,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_NE(0, entries_used);
  EXPECT_EQ(0b101, result_data);
  // -2 because we reduced nbits by 1.
  EXPECT_EQ(irsend.capture.rawlen - kStartOffset - 2, entries_used);
  EXPECT_EQ(kgood_entries_trailing_space - 2, entries_used);

  // Same again but with a footer space over-match, which should fail.
  result_data = 0;
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      3,  // nbits
      8000, 4000,  // Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      500, 10000,  // Footer
      true,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_EQ(0, entries_used);

  // Same as first part but with no footer space data as the last entry.
  irsend.reset();
  result_data = 0;
  irsend.sendRaw(good_no_trailing_space_data, kgood_entries_no_trailing_space,
                 38000);
  irsend.makeDecodeResult();
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      4,  // nbits
      8000, 4000,  // Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      3000, 15000,  // Footer
      true,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_NE(0, entries_used);
  EXPECT_EQ(0b1010, result_data);
  EXPECT_EQ(irsend.capture.rawlen - kStartOffset, entries_used);
  EXPECT_EQ(kgood_entries_no_trailing_space, entries_used);
}

TEST(TestMatchGeneric, FailureCases) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  const uint16_t kentries = 11;
  uint16_t data[kentries] = {
      8000,  // Header mark
      4000,  // Header space
      500, 2000,  // Bit #0 (1)
      500, 1000,  // Bit #1 (0)
      500, 2000,  // Bit #2 (1)
      500, 1000,  // Bit #3 (0)
      3000};  // Footer mark  (No Footer space.)

  uint16_t offset = kStartOffset;
  irsend.reset();
  irsend.sendRaw(data, kentries, 38000);
  irsend.makeDecodeResult();
  uint16_t entries_used = 0;

  // Wanting too many bits should fail.
  uint64_t result_data = 0;
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      5,  // nbits
      8000, 4000,  // Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      3000, 15000,  // Footer
      true,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_EQ(0, entries_used);

  // Bad header mark.
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      4,  // nbits
      2000, 4000,  // Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      3000, 15000,  // Footer
      true,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_EQ(0, entries_used);

  // Bad header space.
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      4,  // nbits
      8000, 2000,  // Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      3000, 15000,  // Footer
      true,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_EQ(0, entries_used);

  // Bad one mark.
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      4,  // nbits
      8000, 4000,  // Header
      600, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      3000, 15000,  // Footer
      true,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_EQ(0, entries_used);

  // Bad one space.
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      4,  // nbits
      8000, 4000,  // Header
      500, 2500,  // one mark & space
      500, 1000,  // zero mark & space
      3000, 15000,  // Footer
      true,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_EQ(0, entries_used);

  // Bad zero space.
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      4,  // nbits
      8000, 4000,  // Header
      500, 2000,  // one mark & space
      500, 1500,  // zero mark & space
      3000, 15000,  // Footer
      true,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_EQ(0, entries_used);

  // Bad zero mark.
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      4,  // nbits
      8000, 4000,  // Header
      500, 2000,  // one mark & space
      900, 1000,  // zero mark & space
      3000, 15000,  // Footer
      true,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_EQ(0, entries_used);

  // Bad Footer mark.
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      4,  // nbits
      8000, 4000,  // Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      1000, 15000,  // Footer
      true,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_EQ(0, entries_used);

  // Confirm it really does match as expected..
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      4,  // nbits
      8000, 4000,  // Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      3000, 15000,  // Footer
      true,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_NE(0, entries_used);
}

TEST(TestMatchGeneric, MissingHeaderFooter) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  const uint16_t kentries = 11;
  uint16_t data[kentries] = {
      8000,  // Header mark
      4000,  // Header space
      500, 2000,  // Bit #0 (1)
      500, 1000,  // Bit #1 (0)
      500, 2000,  // Bit #2 (1)
      500, 1000,  // Bit #3 (0)
      3000};  // Footer mark  (No Footer space.)

  uint16_t offset = kStartOffset;
  irsend.reset();
  irsend.sendRaw(data, kentries, 38000);
  irsend.makeDecodeResult();
  uint16_t entries_used = 0;

  uint64_t result_data = 0;

  // No footer match
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      4,  // nbits
      8000, 4000,  // Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      0, 0,  // NO Footer
      true,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_NE(0, entries_used);
  EXPECT_EQ(0b1010, result_data);
  EXPECT_EQ(irsend.capture.rawlen - kStartOffset - 1, entries_used);
  EXPECT_EQ(kentries - 1, entries_used);

  // No header match (should fail)
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      4,  // nbits
      0, 0,  // NO Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      0, 0,  // NO Footer
      true,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_EQ(0, entries_used);

  // No header match but starting after header
  offset += 2;
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      4,  // nbits
      0, 0,  // NO Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      3000, 15000,  // Footer
      true,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_NE(0, entries_used);
  EXPECT_EQ(0b1010, result_data);
  EXPECT_EQ(irsend.capture.rawlen - offset, entries_used);
  EXPECT_EQ(kentries - 2, entries_used);
}

TEST(TestMatchGeneric, BitOrdering) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  const uint16_t kentries = 11;
  uint16_t data[kentries] = {
      8000,  // Header mark
      4000,  // Header space
      500, 2000,  // Bit #0 (1)
      500, 1000,  // Bit #1 (0)
      500, 2000,  // Bit #2 (1)
      500, 1000,  // Bit #3 (0)
      3000};  // Footer mark  (No Footer space.)

  uint16_t offset = kStartOffset;
  irsend.reset();
  irsend.sendRaw(data, kentries, 38000);
  irsend.makeDecodeResult();
  uint16_t entries_used = 0;

  uint64_t result_data = 0;

  // MSB order
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      4,  // nbits
      8000, 4000,  // Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      3000, 15000,  // Footer
      true,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_NE(0, entries_used);
  EXPECT_EQ(0b1010, result_data);
  EXPECT_EQ(irsend.capture.rawlen - kStartOffset, entries_used);
  EXPECT_EQ(kentries, entries_used);

  // LSB order
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, &result_data,
      irsend.capture.rawlen - offset,
      4,  // nbits
      8000, 4000,  // Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      3000, 15000,  // Footer
      true,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      false);  // LSB first.
  ASSERT_NE(0, entries_used);
  EXPECT_EQ(0b0101, result_data);
  EXPECT_EQ(irsend.capture.rawlen - kStartOffset, entries_used);
  EXPECT_EQ(kentries, entries_used);
}

TEST(TestMatchGeneric, UsingBytes) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  const uint16_t kentries = 32;
  uint16_t data[kentries] = {
      // No header
      500, 2000,   // Byte #0 Bit #0 (1)
      500, 1000,   // Byte #0 Bit #1 (0)
      500, 2000,   // Byte #0 Bit #2 (1)
      500, 1000,   // Byte #0 Bit #3 (0)
      500, 2000,   // Byte #0 Bit #4 (1)
      500, 1000,   // Byte #0 Bit #5 (0)
      500, 2000,   // Byte #0 Bit #6 (1)
      500, 1000,   // Byte #0 Bit #7 (0)
      500, 2000,   // Byte #1 Bit #0 (1)
      500, 2000,   // Byte #1 Bit #1 (1)
      500, 2000,   // Byte #1 Bit #2 (1)
      500, 2000,   // Byte #1 Bit #3 (1)
      500, 1000,   // Byte #1 Bit #4 (0)
      500, 1000,   // Byte #1 Bit #5 (0)
      500, 1000,   // Byte #1 Bit #6 (0)
      500, 1000};  // Byte #1 Bit #7 (0) & No footer

  uint16_t offset = kStartOffset;
  irsend.reset();
  irsend.sendRaw(data, kentries, 38000);
  irsend.makeDecodeResult();
  uint16_t entries_used = 0;

  uint8_t result_data[4] = {};  // Bigger than we need.

  // MSB order
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, result_data,
      irsend.capture.rawlen - offset,
      2 * 8,  // nbits
      0, 0,  // No Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      0, 0,  // No Footer
      false,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_NE(0, entries_used);
  EXPECT_EQ(0b10101010, result_data[0]);
  EXPECT_EQ(0b11110000, result_data[1]);
  EXPECT_EQ(irsend.capture.rawlen - kStartOffset, entries_used);
  EXPECT_EQ(kentries, entries_used);

  // LSB order
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, result_data,
      irsend.capture.rawlen - offset,
      2 * 8,  // nbits
      0, 0,  // No Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      0, 0,  // No Footer
      false,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      false);  // MSB first.
  ASSERT_NE(0, entries_used);
  EXPECT_EQ(0b01010101, result_data[0]);
  EXPECT_EQ(0b00001111, result_data[1]);
  EXPECT_EQ(irsend.capture.rawlen - kStartOffset, entries_used);
  EXPECT_EQ(kentries, entries_used);

  // Asking for too much.
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, result_data,
      irsend.capture.rawlen - offset,
      3 * 8,  // nbits
      0, 0,  // No Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      0, 0,  // No Footer
      false,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_EQ(0, entries_used);

  // Asking for less than what is there.
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, result_data,
      irsend.capture.rawlen - offset,
      1 * 8,  // nbits
      0, 0,  // No Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      0, 0,  // No Footer
      false,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_NE(0, entries_used);
  EXPECT_EQ(0b10101010, result_data[0]);
  EXPECT_GT(irsend.capture.rawlen - kStartOffset, entries_used);
  EXPECT_EQ(16, entries_used);

  // Asking for non mod-8 size should fail.
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, result_data,
      irsend.capture.rawlen - offset,
      9,  // nbits
      0, 0,  // No Header
      500, 2000,  // one mark & space
      500, 1000,  // zero mark & space
      0, 0,  // No Footer
      false,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_EQ(0, entries_used);

  // Expecting different timings should fail.
  entries_used = irrecv.matchGeneric(
      irsend.capture.rawbuf + offset, result_data,
      irsend.capture.rawlen - offset,
      8,  // nbits
      0, 0,  // No Header
      500, 900,  // one mark & space
      500, 1000,  // zero mark & space
      0, 0,  // No Footer
      false,  // atleast on the footer space.
      1,  // 1% Tolerance
      0,  // No excess margin
      true);  // MSB first.
  ASSERT_EQ(0, entries_used);
}

TEST(TestIRrecv, Tolerance) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  uint16_t equal_encoded_raw[11] = {500,  1500, 1500, 500, 499, 1499,
                                    1501, 501,  1499, 490, 500};
  match_result_t result;

  ASSERT_EQ(kTolerance, irrecv.getTolerance());
  irrecv.setTolerance();
  ASSERT_EQ(kTolerance, irrecv.getTolerance());
  irrecv.setTolerance(kTolerance + 1);
  ASSERT_EQ(kTolerance + 1, irrecv.getTolerance());
  irrecv.setTolerance(kTolerance - 1);
  ASSERT_EQ(kTolerance - 1, irrecv.getTolerance());

  irrecv.setTolerance();
  ASSERT_EQ(kTolerance, irrecv.getTolerance());

  irsend.reset();
  irsend.sendRaw(equal_encoded_raw, 11, 38000);
  irsend.makeDecodeResult();
  result = irrecv.matchData(irsend.capture.rawbuf + 1, 5, 1500, 500, 500, 1500);
  ASSERT_TRUE(result.success);
  EXPECT_EQ(0b01011, result.data);
  EXPECT_EQ(10, result.used);

  irrecv.setTolerance(0);
  ASSERT_EQ(0, irrecv.getTolerance());
  irsend.reset();
  irsend.sendRaw(equal_encoded_raw, 11, 38000);
  irsend.makeDecodeResult();
  result = irrecv.matchData(irsend.capture.rawbuf + 1, 5, 1500, 500, 500, 1500);
  ASSERT_FALSE(result.success);
}

TEST(TestDecode, SkippingInDecode) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  // Normal (default) usage.
  irsend.reset();
  irsend.sendNEC(0x4BB640BF);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);

  // Explicitly use the default (no skipping)
  irsend.reset();
  irsend.sendNEC(0x4BB640BF);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 0));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);
  // Enable the least amount of actual skipping.
  // Should still decode normal data.
  irsend.reset();
  irsend.sendNEC(0x4BB640BF);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 1));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);

  // Crank up the amount of actual skipping.
  // Should still decode normal data.
  irsend.reset();
  irsend.sendNEC(0x4BB640BF);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 10));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);

  // Introduce some noise before the actual message.

  // Should not match as NEC anymore.
  irsend.reset();
  irsend.mark(60);
  irsend.space(60);
  irsend.sendNEC(0x4BB640BF);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_NE(NEC, irsend.capture.decode_type);
  // Now with explicit default options set.
  irsend.reset();
  irsend.mark(60);
  irsend.space(60);
  irsend.sendNEC(0x4BB640BF);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 0));
  EXPECT_NE(NEC, irsend.capture.decode_type);

  // Now with 1 skip, which it should match as NEC as there is only one
  // mark + space pair of noise.
  irsend.reset();
  irsend.mark(60);
  irsend.space(60);
  irsend.sendNEC(0x4BB640BF);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 1));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);

  // Now with more than 1 skip, which it should match as NEC as there is only
  // one mark + space pair of noise.
  irsend.reset();
  irsend.mark(60);
  irsend.space(60);
  irsend.sendNEC(0x4BB640BF);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 5));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);

  // Test multiple noise pairs.

  irsend.reset();
  irsend.mark(160);
  irsend.space(60);
  irsend.mark(60);
  irsend.space(160);
  irsend.sendNEC(0x4BB640BF);
  irsend.makeDecodeResult();
  // Default decode parameters should fail to detect as NEC.
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_NE(NEC, irsend.capture.decode_type);
  // A single skip should fail to detect as NEC, as there are two pairs of noise
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 1));
  EXPECT_NE(NEC, irsend.capture.decode_type);
  // However, two skips should match it correctly.
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 2));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);
  // And so should more than 2 skips.
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 10));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);
}

TEST(TestCrudeNoiseFilter, General) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  // Normal (default) usage.
  irsend.reset();
  irsend.sendNEC(0x4BB640BF);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);
  EXPECT_EQ(69, irsend.capture.rawlen);

  // Explicitly no filtering.
  irsend.reset();
  irsend.sendNEC(0x4BB640BF);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 0, 0));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);
  EXPECT_EQ(69, irsend.capture.rawlen);

  // Filter out anything less than 100usecs of which there should be none.
  irsend.reset();
  irsend.sendNEC(0x4BB640BF);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 0, 100));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);
  EXPECT_EQ(69, irsend.capture.rawlen);

  // Add small noise at the start, but don't filter it
  irsend.reset();
  irsend.mark(60);
  irsend.space(60);
  irsend.sendNEC(0x4BB640BF);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 0, 0));
  EXPECT_NE(NEC, irsend.capture.decode_type);
  EXPECT_EQ(69 + 2, irsend.capture.rawlen);

  // Add small noise at the start, but filter it this time
  irsend.reset();
  irsend.mark(60);
  irsend.space(60);
  irsend.sendNEC(0x4BB640BF);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 0, 100));
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);
  EXPECT_EQ(69, irsend.capture.rawlen);

  irsend.reset();
  irsend.mark(60);  // Less than the filter floor.
  irsend.space(560);
  irsend.sendNEC(0x4BB640BF);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 0, 100));
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);
  EXPECT_EQ(69, irsend.capture.rawlen);

  irsend.reset();
  irsend.mark(100);  // Higher than the filter floor.
  irsend.space(200);  // Higher than the filter floor.
  irsend.sendNEC(0x4BB640BF);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 0, 100));
  EXPECT_NE(kNECBits, irsend.capture.bits);
  EXPECT_EQ(69 + 2, irsend.capture.rawlen);

  irsend.reset();
  irsend.mark(160);  // Higher than the filter floor.
  irsend.space(60);  // Lower than the filter floor.
  irsend.sendNEC(0x4BB640BF);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 0, 100));
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);
  EXPECT_EQ(69, irsend.capture.rawlen);

  // Multiple noise at the start
  irsend.reset();
  irsend.mark(60);  // Lower than the filter floor.
  irsend.space(60);  // Lower than the filter floor.
  irsend.mark(60);  // Lower than the filter floor.
  irsend.space(60);  // Lower than the filter floor.
  irsend.mark(60);  // Lower than the filter floor.
  irsend.space(60);  // Lower than the filter floor.
  irsend.sendNEC(0x4BB640BF);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 0, 100));
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);
  EXPECT_EQ(69, irsend.capture.rawlen);

  // A mix of noise that should be removed.
  irsend.reset();
  irsend.mark(60);    // Lower than the filter floor.
  irsend.space(160);  // Higher than the filter floor.
  irsend.mark(160);   // Higher than the filter floor.
  irsend.space(60);   // Lower than the filter floor.
  irsend.mark(160);   // Higher than the filter floor.
  irsend.space(60);   // Lower than the filter floor.
  irsend.sendNEC(0x4BB640BF);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 0, 100));
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);
  EXPECT_EQ(69, irsend.capture.rawlen);

  // Add noise that should be removed to the end of a message.
  irsend.reset();
  irsend.sendNEC(0x4BB640BF);
  irsend.mark(60);    // Lower than the filter floor.
  irsend.space(160);  // Higher than the filter floor.
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 0, 100));
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x4BB640BF, irsend.capture.value);
  EXPECT_EQ(69, irsend.capture.rawlen);
}

TEST(TestCrudeNoiseFilter, NoiseMidSample) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1042#issuecomment-583895303
  uint16_t rawData[71] = {
      482, 1370, 9082, 1558, 342, 2514, 662, 470, 660, 468, 658, 1588, 662, 466,
      662, 466, 662, 466, 662, 466, 662, 466, 662, 1586, 660, 1588, 662, 466,
      662, 1588, 662, 1586, 662, 1586, 660, 1588, 662, 1586, 662, 468, 660,
      1588, 662, 468, 662, 466, 660, 466, 662, 464, 662, 466, 662, 466, 662,
      1588, 660, 466, 662, 1586, 662, 1588, 660, 1586, 662, 1586, 662, 1586,
      664, 1594, 662};  // UNKNOWN B0784C9E

  irsend.reset();
  irsend.sendRaw(rawData, 71, 38);
  irsend.makeDecodeResult();
  // Try with no filter first.
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 1, 0));
  EXPECT_NE(NEC, irsend.capture.decode_type);
  const uint16_t prev_length = irsend.capture.rawlen;
  // Now with the filter set to 350, to remove the 342 mark at rawData[4].
  // Note: a mark of 350us is huge.
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 1, 350));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x20DF40BF, irsend.capture.value);
  EXPECT_EQ(prev_length - 2, irsend.capture.rawlen);
  EXPECT_EQ(
      "uint16_t rawData[69] = {482, 1370,  9082, 4414,  662, 470,  660, 468,  "
      "658, 1588,  662, 466,  662, 466,  662, 466,  662, 466,  662, 466,  662, "
      "1586,  660, 1588,  662, 466,  662, 1588,  662, 1586,  662, 1586,  660, "
      "1588,  662, 1586,  662, 468,  660, 1588,  662, 468,  662, 466,  660, "
      "466,  662, 464,  662, 466,  662, 466,  662, 1588,  660, 466,  662, "
      "1586,  662, 1588,  660, 1586,  662, 1586,  662, 1586,  664, 1594,  "
      "662};  // NEC 20DF40BF\n"
      "uint32_t address = 0x4;\n"
      "uint32_t command = 0x2;\n"
      "uint64_t data = 0x20DF40BF;\n",
      resultToSourceCode(&irsend.capture));

  // Data modified as rawData2[3] which was 882 is too far out of spec. for a
  // NEC bit mark.
  uint16_t rawData2[7] = {
      9066, 2026, 600, 13906, 222, 992, 734};  // UNKNOWN 8FDE36A7
  irsend.reset();
  irsend.sendRaw(rawData2, 7, 38);
  irsend.makeDecodeResult();
  // Try with no filter first.
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 0, 0));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(0, irsend.capture.bits);
  EXPECT_TRUE(irsend.capture.repeat);
  // Now with the filter set to 250, to remove the 222 mark at rawData2[4].
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 0, 350));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(0, irsend.capture.bits);
  EXPECT_TRUE(irsend.capture.repeat);
  EXPECT_EQ(
      "uint16_t rawData[5] = {9066, 2026,  600, 15120,  734};  "
      "// NEC (Repeat) FFFFFFFFFFFFFFFF\n"
      "uint64_t data = 0xFFFFFFFFFFFFFFFF;\n",
      resultToSourceCode(&irsend.capture));
}

TEST(TestCrudeNoiseFilter, NoiseAtEndOfSample) {
  IRsendTest irsend(0);
  IRrecv irrecv(1);
  irsend.begin();

  // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1042#issuecomment-583895303
  uint16_t rawData[69] = {  // UNKNOWN B52869E1
      9078, 4386, 662, 468, 660, 466, 662, 1588, 660, 468, 660, 468, 662, 466,
      662, 466, 662, 466, 662, 1588, 660, 1588, 660, 466, 662, 1590, 660, 1586,
      662, 1586, 662, 1588, 662, 1584, 662, 1588, 662, 1588, 660, 466, 686, 442,
      662, 466, 662, 466, 662, 466, 662, 468, 662, 466, 662, 466, 662, 1586,
      662, 1588, 662, 1586, 644, 1600, 662, 1586, 688, 1566, 684, 2638, 146};
  irsend.reset();
  irsend.sendRaw(rawData, 69, 38);
  irsend.makeDecodeResult();
  // Try with no filter first.
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 0, 0));
  EXPECT_NE(NEC, irsend.capture.decode_type);
  const uint16_t prev_length = irsend.capture.rawlen;

  // Now with the filter set to 200, to remove the 146 mark at at the end.
  EXPECT_TRUE(irrecv.decode(&irsend.capture, NULL, 0, 200));
  EXPECT_EQ(NEC, irsend.capture.decode_type);
  EXPECT_EQ(kNECBits, irsend.capture.bits);
  EXPECT_EQ(0x20DFC03F, irsend.capture.value);
  EXPECT_EQ(prev_length - 2, irsend.capture.rawlen);
  EXPECT_EQ(
      "uint16_t rawData[67] = {9078, 4386,  662, 468,  660, 466,  662, 1588,  "
      "660, 468,  660, 468,  662, 466,  662, 466,  662, 466,  662, 1588,  660, "
      "1588,  660, 466,  662, 1590,  660, 1586,  662, 1586,  662, 1588,  662, "
      "1584,  662, 1588,  662, 1588,  660, 466,  686, 442,  662, 466,  662, "
      "466,  662, 466,  662, 468,  662, 466,  662, 466,  662, 1586,  662, 1588,"
      "  662, 1586,  644, 1600,  662, 1586,  688, 1566,  684};  "
      "// NEC 20DFC03F\n"
      "uint32_t address = 0x4;\n"
      "uint32_t command = 0x3;\n"
      "uint64_t data = 0x20DFC03F;\n",
      resultToSourceCode(&irsend.capture));
}

TEST(TestManchesterCode, matchManchester) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);

  const uint16_t rawData[163] = {
      2860, 3862,
      1924, 1952, 1926, 1952, 956, 984, 1924, 1028, 952, 958, 980, 956, 982,
      1882, 1016, 950, 1958, 1920, 1948, 1004, 954, 956, 984, 956, 952, 984,
      974, 966, 974, 964, 974, 1888, 1010, 960, 1948, 1002, 946, 962, 978,
      962, 976, 960, 948, 992, 978, 1886, 982, 984, 1924, 1954, 952, 986,
      3892, 3862,
      1924, 1954, 1924, 1954, 984, 952, 1956, 996, 954, 990, 948, 958, 980,
      1882, 1016, 952, 1958, 1920, 1956, 994, 944, 962, 986, 956, 972, 962,
      976, 962, 978, 964, 952, 1908, 1010, 960, 1948, 1002, 946, 960, 978,
      962, 946, 990, 978, 960, 978, 1888, 1008, 954, 1952, 1928, 982, 956,
      3920, 3830,
      1946, 1934, 1954, 1922, 976, 962, 1946, 1006, 922, 990, 948, 988, 950,
      1910, 1008, 964, 1922, 1952, 1924, 1030, 920, 984, 954, 986, 952, 986,
      952, 984, 954, 986, 952, 1910, 1010, 960, 1928, 1024, 944, 996, 924,
      984, 954, 988, 950, 984, 954, 1910, 1008, 960, 1920, 1958, 980, 958,
      4800};
  irsend.sendRaw(rawData, 163, 38);
  irsend.makeDecodeResult();

  uint16_t offset = 1;
  uint64_t result = 0;
  uint16_t nbits = 32;
  EXPECT_EQ(56, irrecv.matchManchester(irsend.capture.rawbuf + offset, &result,
                                       irsend.capture.rawlen - offset, nbits,
                                       2860, 3800, 1000, 0, 3800));
  EXPECT_EQ(0x4F2FE7E4, result);

  irsend.reset();
  irsend.sendRaw(rawData, 55, 38);  // Send just the bare minimum.
  irsend.makeDecodeResult();

  EXPECT_EQ(55, irrecv.matchManchester(irsend.capture.rawbuf + offset, &result,
                                       irsend.capture.rawlen - offset, nbits,
                                       2860, 3800, 1000, 0, 3800));
  EXPECT_EQ(0x4F2FE7E4, result);

  irsend.reset();
  irsend.sendRaw(rawData, 52, 38);  // Now, just too short.
  irsend.makeDecodeResult();

  EXPECT_EQ(0, irrecv.matchManchester(irsend.capture.rawbuf + offset, &result,
                                      irsend.capture.rawlen - offset, nbits,
                                      2860, 3800, 1000, 0, 0));
}

TEST(TestManchesterCode, ManchesterLoopBackGEThomasTest) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  uint16_t offset = 1;
  uint64_t result = 0;
  uint16_t nbits = 32;
  irsend.begin();
  irsend.reset();
  irsend.sendManchester(5000, 7000, 1000, 0, 10000, 0x12345678, nbits);
  irsend.makeDecodeResult();
  EXPECT_EQ(52, irrecv.matchManchester(irsend.capture.rawbuf + offset, &result,
                                       irsend.capture.rawlen - offset, nbits,
                                       5000, 7000, 1000, 0, 10000, true,
                                       kUseDefTol, kMarkExcess, true, true));
  EXPECT_EQ(0x12345678, result);

  irsend.reset();
  irsend.sendManchester(5000, 7000, 1000, 0, 10000, 0x87654321, nbits);
  irsend.makeDecodeResult();
  EXPECT_EQ(52, irrecv.matchManchester(irsend.capture.rawbuf + offset, &result,
                                       irsend.capture.rawlen - offset, nbits,
                                       5000, 7000, 1000, 0, 10000, true,
                                       kUseDefTol, kMarkExcess, true, true));
  EXPECT_EQ(0x87654321, result);
}

TEST(TestManchesterCode, ManchesterLoopBackIEEE802_3Test) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  uint16_t offset = 1;
  uint64_t result = 0;
  uint16_t nbits = 32;
  irsend.begin();
  irsend.reset();
  irsend.sendManchester(5000, 7000, 1000, 0, 10000, 0x12345678, nbits, 38000,
                        true, kNoRepeat, kDutyDefault, false);
  irsend.makeDecodeResult();
  EXPECT_EQ(52, irrecv.matchManchester(irsend.capture.rawbuf + offset, &result,
                                       irsend.capture.rawlen - offset, nbits,
                                       5000, 7000, 1000, 0, 10000, true,
                                       kUseDefTol, kMarkExcess, true, false));
  EXPECT_EQ(0x12345678, result);

  irsend.reset();
  irsend.sendManchester(5000, 7000, 1000, 0, 10000, 0x87654321, nbits, 38000,
                        true, kNoRepeat, kDutyDefault, false);
  irsend.makeDecodeResult();
  EXPECT_EQ(54, irrecv.matchManchester(irsend.capture.rawbuf + offset, &result,
                                       irsend.capture.rawlen - offset, nbits,
                                       5000, 7000, 1000, 0, 10000, true,
                                       kUseDefTol, kMarkExcess, true, false));
  EXPECT_EQ(0x87654321, result);
}
