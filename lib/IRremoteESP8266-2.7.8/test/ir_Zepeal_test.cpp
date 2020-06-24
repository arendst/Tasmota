// Copyright 2020 Christian Nilsson

#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for decodeZepeal().

TEST(TestDecodeZepeal, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  // fuuryou (speed) one of 5 repeats
  const uint16_t rawData_1[35] = {
      2328, 3412,
      424, 1314,  1302, 436,  1276, 454,  424, 1316,
      1274, 464,  1274, 458,  454, 1278,  450, 1288,
      1302, 432,  424, 1306,  424, 1306,  424, 1306,
      428, 1304,  450, 1290,  1298, 430,  426, 1308,
      426};  // UNKNOWN B5E66F84
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData_1, 35, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::ZEPEAL, irsend.capture.decode_type);
  ASSERT_EQ(kZepealBits, irsend.capture.bits);
  EXPECT_EQ(0x6C82, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  // kiri/iri (off/on) short press
  const uint16_t rawData_2[179] = {
      2338, 3384,
      426, 1314,  1300, 436,  1304, 428,  424, 1314,
      1302, 440,  1300, 428,  424, 1306,  424, 1314,
      1300, 432,  400, 1330,  400, 1332,  418, 1316,
      422, 1308,  424, 1308,  400, 1338,  1300, 432,
      426, 6728,
      2352, 3384,
      426, 1312,  1302, 436,  1276, 438,  442, 1316,
      1300, 434,  1302, 430,  424, 1308,  422, 1298,
      1318, 414,  468, 1280,  426, 1306,  452, 1280,
      450, 1282,  400, 1332,  400, 1338,  1302, 412,
      416, 6760,
      2350, 3384,
      400, 1340,  1300, 438,  1300, 412,  444, 1312,
      1302, 434,  1302, 412,  442, 1308,  426, 1312,
      1302, 430,  422, 1308,  424, 1304,  400, 1332,
      424, 1306,  402, 1332,  422, 1318,  1300, 430,
      398, 6754,
      2354, 3364,
      442, 1316,  1300, 438,  1300, 430,  422, 1316,
      1328, 410,  1302, 430,  400, 1332,  400, 1338,
      1302, 412,  418, 1334,  398, 1314,  444, 1306,
      400, 1330,  424, 1308,  424, 1314,  1300, 430,
      428, 6774,
      2334, 3384,
      400, 1338,  1302, 436,  1304, 430,  396, 1340,
      1300, 436,  1302, 432,  452, 1280,  398, 1338,
      1302, 428,  402, 1330,  426, 1310,  422, 1306,
      426, 1306,  426, 1306,  404, 1336,  1274, 458,
      424};  // UNKNOWN C2DCDDE5

  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData_2, 179, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::ZEPEAL, irsend.capture.decode_type);
  ASSERT_EQ(kZepealBits, irsend.capture.bits);
  EXPECT_EQ(0x6C81, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  EXPECT_EQ(
    "f38000d50"
    "m2338s3384"
    "m426s1314m1300s436m1304s428m424s1314m1302s440m1300s428m424s1306m424s1314"
    "m1300s432m400s1330m400s1332m418s1316m422s1308m424s1308m400s1338m1300s432"
    "m426s6728"
    "m2352s3384"
    "m426s1312m1302s436m1276s438m442s1316m1300s434m1302s430m424s1308m422s1298"
    "m1318s414m468s1280m426s1306m452s1280m450s1282m400s1332m400s1338m1302s412"
    "m416s6760"
    "m2350s3384"
    "m400s1340m1300s438m1300s412m444s1312m1302s434m1302s412m442s1308m426s1312"
    "m1302s430m422s1308m424s1304m400s1332m424s1306m402s1332m422s1318m1300s430"
    "m398s6754"
    "m2354s3364"
    "m442s1316m1300s438m1300s430m422s1316m1328s410m1302s430m400s1332m400s1338"
    "m1302s412m418s1334m398s1314m444s1306m400s1330m424s1308m424s1314m1300s430"
    "m428s6774"
    "m2334s3384"
    "m400s1338m1302s436m1304s430m396s1340m1300s436m1302s432m452s1280m398s1338"
    "m1302s428m402s1330m426s1310m422s1306m426s1306m426s1306m404s1336m1274s458"
    "m424",
    irsend.outputStr());

  // rizumu/oyasumi (rhythm/sleep - mode)
  const uint16_t rawData_3[143] = {
      2354, 3386,
      422, 1312,  1278, 462,  1300, 410,  442, 1316,
      1300, 436,  1302, 412,  448, 1304,  398, 1338,
      1300, 430,  426, 1306,  426, 1304,  400, 1332,
      400, 1340,  1300, 430,  400, 1334,  426, 1304,
      398, 6756,
      2354, 3382,
      424, 1294,  1346, 410,  1302, 410,  418, 1340,
      1300, 434,  1304, 430,  398, 1332,  400, 1338,
      1276, 456,  400, 1334,  400, 1330,  400, 1332,
      398, 1342,  1298, 430,  422, 1308,  400, 1332,
      398, 6754,
      2356, 3382,
      400, 1340,  1274, 464,  1298, 412,  444, 1314,
      1300, 438,  1300, 412,  442, 1308,  400, 1338,
      1300, 414,  418, 1330,  400, 1332,  400, 1330,
      426, 1312,  1332, 380,  446, 1306,  424, 1308,
      424, 6736,
      2352, 3380,
      402, 1338,  1302, 436,  1300, 412,  420, 1338,
      1300, 436,  1300, 412,  418, 1332,  402, 1336,
      1302, 408,  418, 1332,  424, 1308,  424, 1306,
      398, 1340,  1276, 454,  400, 1314,  418, 1332,
      426};  // UNKNOWN 712E7A7F
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData_3, 143, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::ZEPEAL, irsend.capture.decode_type);
  ASSERT_EQ(kZepealBits, irsend.capture.bits);
  EXPECT_EQ(0x6C84, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  // kiri taimaa (off timer)
  const uint16_t rawData_4[143] = {
      2308, 3392,
      444, 1314,  1302, 436,  1330, 382,  420, 1338,
      1302, 436,  1302, 408,  444, 1308,  450, 1288,
      1300, 410,  420, 1332,  424, 1308,  400, 1338,
      1302, 430,  428, 1286,  444, 1306,  452, 1282,
      424, 6728,
      2354, 3362,
      444, 1314,  1274, 464,  1302, 410,  420, 1338,
      1300, 438,  1328, 384,  444, 1308,  398, 1338,
      1274, 456,  424, 1306,  402, 1332,  424, 1312,
      1302, 412,  442, 1308,  424, 1308,  398, 1334,
      422, 6752,
      2336, 3382,
      422, 1316,  1302, 434,  1302, 430,  400, 1340,
      1302, 434,  1302, 410,  444, 1310,  422, 1314,
      1302, 410,  444, 1304,  428, 1304,  426, 1312,
      1302, 428,  424, 1308,  424, 1308,  426, 1304,
      426, 6736,
      2354, 3382,
      424, 1314,  1302, 438,  1300, 410,  418, 1338,
      1304, 436,  1304, 406,  418, 1336,  398, 1338,
      1300, 432,  424, 1306,  426, 1306,  450, 1286,
      1302, 412,  418, 1336,  396, 1334,  424, 1306,
      424};  // UNKNOWN AAD01FEF
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData_4, 143, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::ZEPEAL, irsend.capture.decode_type);
  ASSERT_EQ(kZepealBits, irsend.capture.bits);
  EXPECT_EQ(0x6C88, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  // iri taimaa (on timer)
  const uint16_t rawData_5[143] = {
      2364, 3358,
      424, 1316,  1298, 438,  1300, 432,  424, 1314,
      1298, 440,  1300, 430,  424, 1310,  424, 1312,
      1300, 436,  1302, 430,  424, 1308,  422, 1308,
      424, 1308,  424, 1314,  1302, 438,  1302, 430,
      422, 6730,
      2354, 3384,
      422, 1314,  1300, 438,  1302, 426,  426, 1312,
      1300, 438,  1302, 412,  418, 1330,  426, 1316,
      1302, 434,  1302, 430,  426, 1306,  424, 1306,
      424, 1308,  426, 1312,  1302, 436,  1302, 430,
      428, 6752,
      2364, 3354,
      428, 1308,  1304, 436,  1302, 428,  426, 1314,
      1330, 408,  1304, 428,  402, 1330,  398, 1338,
      1306, 432,  1280, 454,  426, 1302,  428, 1304,
      430, 1302,  428, 1310,  1306, 434,  1304, 428,
      428, 6788,
      2336, 3382,
      428, 1310,  1302, 434,  1304, 430,  426, 1310,
      1306, 434,  1304, 430,  424, 1306,  402, 1336,
      1302, 438,  1304, 426,  426, 1304,  402, 1330,
      426, 1304,  400, 1336,  1306, 434,  1304, 428,
      424};  // UNKNOWN F8FC587
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData_5, 143, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::ZEPEAL, irsend.capture.decode_type);
  ASSERT_EQ(kZepealBits, irsend.capture.bits);
  EXPECT_EQ(0x6CC3, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

TEST(TestDecodeZepeal, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  irsend.reset();
  // power
  irsend.sendZepeal(0x6C81);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::ZEPEAL, irsend.capture.decode_type);
  EXPECT_EQ(kZepealBits, irsend.capture.bits);
  EXPECT_EQ(0x6C81, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  EXPECT_EQ(
    "f38000d50"
    "m2330s3380"
    "m420s1300m1300s420m1300s420m420s1300m1300s420m1300s420m420s1300m420s1300"
    "m1300s420m420s1300m420s1300m420s1300m420s1300m420s1300m420s1300m1300s420"
    "m420s6750"
    "m2330s3380"
    "m420s1300m1300s420m1300s420m420s1300m1300s420m1300s420m420s1300m420s1300"
    "m1300s420m420s1300m420s1300m420s1300m420s1300m420s1300m420s1300m1300s420"
    "m420s6750"
    "m2330s3380"
    "m420s1300m1300s420m1300s420m420s1300m1300s420m1300s420m420s1300m420s1300"
    "m1300s420m420s1300m420s1300m420s1300m420s1300m420s1300m420s1300m1300s420"
    "m420s6750"
    "m2330s3380"
    "m420s1300m1300s420m1300s420m420s1300m1300s420m1300s420m420s1300m420s1300"
    "m1300s420m420s1300m420s1300m420s1300m420s1300m420s1300m420s1300m1300s420"
    "m420s6750"
    "m2330s3380"
    "m420s1300m1300s420m1300s420m420s1300m1300s420m1300s420m420s1300m420s1300"
    "m1300s420m420s1300m420s1300m420s1300m420s1300m420s1300m420s1300m1300s420"
    "m420s6750",
    irsend.outputStr());

  irsend.reset();
  irsend.sendZepeal(0x6Cff, kZepealBits, kNoRepeat);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::ZEPEAL, irsend.capture.decode_type);
  EXPECT_EQ(kZepealBits, irsend.capture.bits);
  EXPECT_EQ(0x6Cff, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_EQ(
    "f38000d50"
    "m2330s3380"
    "m420s1300m1300s420m1300s420m420s1300m1300s420m1300s420m420s1300m420s1300"
    "m1300s420m1300s420m1300s420m1300s420m1300s420m1300s420m1300s420m1300s420"
    "m420s6750",
    irsend.outputStr());

  irsend.reset();
  // testing a non valid value
  irsend.sendZepeal(0xffff, kZepealBits, kNoRepeat);
  irsend.makeDecodeResult();

  // strict check should fail
  ASSERT_FALSE(irrecv.decodeZepeal(&irsend.capture, kStartOffset,
                                   kZepealBits, true));
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(-1, irsend.capture.decode_type);

  // non strict check should be ok
  ASSERT_TRUE(irrecv.decodeZepeal(&irsend.capture, kStartOffset,
                                  kZepealBits, false));
  EXPECT_EQ(decode_type_t::ZEPEAL, irsend.capture.decode_type);
  EXPECT_EQ(kZepealBits, irsend.capture.bits);
  EXPECT_EQ(0xffff, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_EQ(
    "f38000d50"
    "m2330s3380"
    "m1300s420m1300s420m1300s420m1300s420m1300s420m1300s420m1300s420m1300s420"
    "m1300s420m1300s420m1300s420m1300s420m1300s420m1300s420m1300s420m1300s420"
    "m420s6750",
    irsend.outputStr());
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("ZEPEAL", typeToString(decode_type_t::ZEPEAL));
  ASSERT_EQ(decode_type_t::ZEPEAL, strToDecodeType("ZEPEAL"));
  ASSERT_FALSE(hasACState(decode_type_t::ZEPEAL));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::ZEPEAL));
  ASSERT_EQ(kZepealBits, IRsend::defaultBits(decode_type_t::ZEPEAL));
  ASSERT_EQ(kZepealMinRepeat, IRsend::minRepeats(decode_type_t::ZEPEAL));
}
