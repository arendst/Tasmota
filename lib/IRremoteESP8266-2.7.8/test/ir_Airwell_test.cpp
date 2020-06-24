// Copyright 2020 David Conran

#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for decodeAirwell().

// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1069
// Data from:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/1069#issuecomment-604293514
TEST(TestDecodeAirwell, RealExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  // ON / OFF / 25 degrees heat
  const uint16_t rawData_1[163] = {
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
      4800};  // UNKNOWN 565E2BB3
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData_1, 163, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::AIRWELL, irsend.capture.decode_type);
  ASSERT_EQ(kAirwellBits, irsend.capture.bits);
  EXPECT_EQ(0x2B0D0181B, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  const uint16_t rawData_2[175] = {
      2862, 3892,
      1894, 1060, 890, 1944, 952, 1014, 922, 1016, 1892, 1062, 886,
      1020, 928, 1042, 896, 1938, 960, 1008, 920, 1020, 928, 1010, 928, 1012,
      1896, 1056, 892, 1016, 922, 1020, 918, 1018, 920, 1018, 920, 1946, 950,
      1014, 1894, 1062, 896, 1010, 928, 1010, 928, 1014, 924, 1014, 922, 1938,
      960, 1012, 1896, 1984, 922, 1014,
      3862, 3894,
      1892, 1062, 896, 1936, 950, 1020, 928, 1008, 1898, 1056, 892, 1016, 952,
      984, 954, 1910, 956, 1012, 924, 1014, 924, 1014, 922, 1014, 1892, 1060,
      916, 992, 926, 1010, 928, 1014, 954, 982, 944, 1918, 958, 1010, 1896,
      1058, 922, 986, 952, 988, 920, 1018, 950, 986, 952, 1914, 954, 1014,
      1892, 1986, 922, 1016,
      3860, 3896,
      1898, 1054, 924, 1908, 958, 1012, 926, 1012, 1896, 1056, 942, 966, 952,
      988, 950, 1910, 956, 1016, 922, 1016, 952, 988, 950, 986, 1922, 1032,
      916, 990, 948, 960, 988, 984, 944, 960, 978, 1918, 978, 990, 1896, 1054,
      924, 982, 946, 994, 954, 984, 954, 988, 950, 1910, 956, 1012, 1896, 1984,
      922, 1018, 4768};  // UNKNOWN 1002DCC8

  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData_2, 175, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::AIRWELL, irsend.capture.decode_type);
  ASSERT_EQ(kAirwellBits, irsend.capture.bits);
  EXPECT_EQ(0x270F8181B, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

TEST(TestDecodeAirwell, SyntheticExample) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();
  irsend.reset();
  irsend.sendAirwell(0x2B0D0181B);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::AIRWELL, irsend.capture.decode_type);
  EXPECT_EQ(kAirwellBits, irsend.capture.bits);
  EXPECT_EQ(0x2B0D0181B, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  EXPECT_EQ(
      "f38000d50"
      "m2850s3800"
      "m1900s1900m1900s1900m950s950m1900s950m950s950m950s950m950"
      "s1900m950s950m1900s1900m1900s950m950s950m950s950m950s950"
      "m950s950m950s950m950s1900m950s950m1900s950m950s950m950"
      "s950m950s950m950s950m950s1900m950s950m1900s1900m950s950"
      "m3800s3800"
      "m1900s1900m1900s1900m950s950m1900s950m950s950m950s950m950"
      "s1900m950s950m1900s1900m1900s950m950s950m950s950m950s950"
      "m950s950m950s950m950s1900m950s950m1900s950m950s950m950"
      "s950m950s950m950s950m950s1900m950s950m1900s1900m950s950"
      "m3800s3800"
      "m1900s1900m1900s1900m950s950m1900s950m950s950m950s950m950"
      "s1900m950s950m1900s1900m1900s950m950s950m950s950m950s950"
      "m950s950m950s950m950s1900m950s950m1900s950m950s950m950"
      "s950m950s950m950s950m950s1900m950s950m1900s1900m950s950"
      "m4750s100000",
      irsend.outputStr());

  irsend.reset();
  irsend.sendAirwell(0x60080002);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::AIRWELL, irsend.capture.decode_type);
  EXPECT_EQ(kAirwellBits, irsend.capture.bits);
  EXPECT_EQ(0x60080002, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
  EXPECT_EQ(
      "f38000d50"
      "m2850s2850"
      "m950s950m950s950m950s1900m950s950m1900s950m950s950m950s950m950s950"
      "m950s950m950s950m950s950m950s950m950s1900m1900s950m950s950m950s950"
      "m950s950m950s950m950s950m950s950m950s950m950s950m950s950m950s950"
      "m950s950m950s950m950s950m950s950m950s950m950s1900m1900s950"
      "m2850s2850"
      "m950s950m950s950m950s1900m950s950m1900s950m950s950m950s950m950s950"
      "m950s950m950s950m950s950m950s950m950s1900m1900s950m950s950m950s950"
      "m950s950m950s950m950s950m950s950m950s950m950s950m950s950m950s950"
      "m950s950m950s950m950s950m950s950m950s950m950s1900m1900s950"
      "m2850s2850"
      "m950s950m950s950m950s1900m950s950m1900s950m950s950m950s950m950s950"
      "m950s950m950s950m950s950m950s950m950s1900m1900s950m950s950m950s950"
      "m950s950m950s950m950s950m950s950m950s950m950s950m950s950m950s950"
      "m950s950m950s950m950s950m950s950m950s950m950s1900m1900s950"
      "m3800s100000",
      irsend.outputStr());

  irsend.reset();
  irsend.sendAirwell(0x70F8181B);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::AIRWELL, irsend.capture.decode_type);
  EXPECT_EQ(kAirwellBits, irsend.capture.bits);
  EXPECT_EQ(0x70F8181B, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1069
// Data from:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/1069#issuecomment-607659677
TEST(TestDecodeAirwell, RealExample2) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  // "When I change from 22° to 23° :"
  const uint16_t rawData[175] = {
      2890, 2918,
      924, 1006, 946, 1886, 1986, 1922, 978, 958, 1952, 1000, 952, 956, 974,
      964, 976, 1884, 1016, 952, 1948, 1006, 944, 962, 980, 958, 982, 958, 952,
      982, 978, 960, 980, 956, 986, 956, 974, 1888, 1014, 954, 1948, 1002, 948,
      960, 980, 960, 950, 986, 1006, 962, 980, 1854, 1036, 932, 1948, 1928, 984,
      954,
      3918, 2920,
      922, 1008, 942, 1888, 1984, 1926, 984, 950, 1952, 1000, 952, 956, 984,
      956, 954, 1908, 1014, 952, 1948, 1002, 948, 960, 950, 986, 984, 956, 954,
      984, 956, 984, 946, 992, 948, 992, 948, 1910, 1012, 958, 1954, 996, 922,
      984, 956, 986, 956, 980, 950, 988, 954, 1910, 1010, 956, 1946, 1932, 978,
      962,
      3922, 2914,
      918, 1010, 920, 1912, 1980, 1928, 982, 956, 1944, 1006, 924, 982, 958,
      982, 948, 1912, 1008, 958, 1952, 1000, 920, 988, 952, 982, 958, 984, 958,
      978, 952, 986, 986, 952, 958, 984, 946, 1912, 1010, 962, 1948, 1004, 926,
      980, 950, 988, 952, 984, 956, 982, 948, 1916, 1016, 952, 1950, 1926, 984,
      952, 4830};  // UNKNOWN 8E34167B
  irsend.begin();
  irsend.reset();
  irsend.sendRaw(rawData, 175, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::AIRWELL, irsend.capture.decode_type);
  ASSERT_EQ(kAirwellBits, irsend.capture.bits);
  EXPECT_EQ(0xB0C0181B, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);

  // Resend it as a synthetic to see if it decodes to the same value.
  irsend.reset();
  irsend.sendAirwell(0xB0C0181B);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  ASSERT_EQ(decode_type_t::AIRWELL, irsend.capture.decode_type);
  ASSERT_EQ(kAirwellBits, irsend.capture.bits);
  EXPECT_EQ(0xB0C0181B, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("AIRWELL", typeToString(decode_type_t::AIRWELL));
  ASSERT_EQ(decode_type_t::AIRWELL, strToDecodeType("AIRWELL"));
  ASSERT_FALSE(hasACState(decode_type_t::AIRWELL));
  ASSERT_FALSE(IRac::isProtocolSupported(decode_type_t::AIRWELL));
  ASSERT_EQ(kAirwellBits, IRsend::defaultBits(decode_type_t::AIRWELL));
  ASSERT_EQ(kAirwellMinRepeats, IRsend::minRepeats(decode_type_t::AIRWELL));
}

// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1069
// Data from:
//   https://docs.google.com/spreadsheets/d/1MeqVsgXQAMSiAx7zNunG8qbFcIT8tqZtcjLmMa0GEng/edit#gid=1053299474&range=M2:GW2
TEST(TestDecodeAirwell, RealExample3) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  const uint16_t rawData[193] = {
      3010, 2852,
      904, 1042, 872, 1052, 872, 1932, 904, 956, 1878, 1050, 874, 1046, 872,
      956, 876, 1048, 872, 1044, 876, 1046, 874, 958, 872, 1050, 872, 1966,
      1880, 956, 872, 1048, 874, 1048, 872, 1050, 872, 958, 872, 1050, 872,
      1048, 872, 1050, 872, 958, 872, 1048, 872, 1050, 872, 1048, 872, 958, 872,
      1050, 872, 1048, 872, 1050, 872, 1872, 1880, 1048,
      2978, 2880,
      872, 1048, 872, 1050, 872, 1964, 872, 958, 1880, 1050, 872, 1050, 872,
      958, 872, 1050, 872, 1050, 872, 1050, 872, 958, 872, 1050, 872, 1964,
      1882, 958, 870, 1050, 872, 1048, 872, 1050, 872, 958, 872, 1048, 872,
      1050, 872, 1050, 872, 958, 872, 1050, 872, 1050, 872, 1050, 872, 958, 872,
      1050, 872, 1050, 872, 1050, 872, 1874, 1880, 1050,
      2978, 2880,
      872, 1050, 872, 1050, 872, 1964, 872, 958, 1880, 1050, 872, 1050, 872,
      958, 872, 1050, 872, 1050, 872, 1050, 872, 958, 872, 1050, 872, 1964,
      1880, 958, 872, 1050, 872, 1050, 872, 1050, 872, 958, 872, 1052, 870,
      1050, 872, 1050, 872, 958, 872, 1050, 872, 1050, 872, 1050, 872, 958, 872,
      1050, 872, 1050, 872, 1050, 872, 1874, 1880, 1050, 3894};
  irsend.reset();
  irsend.sendRaw(rawData, 193, 38);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodeAirwell(&irsend.capture));
  ASSERT_EQ(decode_type_t::AIRWELL, irsend.capture.decode_type);
  ASSERT_EQ(kAirwellBits, irsend.capture.bits);
  EXPECT_EQ(0x60080002, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x0, irsend.capture.command);
}
