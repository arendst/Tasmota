// Copyright 2019 David Conran

#include "ir_Argo.h"
#include "ir_Daikin.h"
#include "ir_Fujitsu.h"
#include "ir_Gree.h"
#include "ir_Haier.h"
#include "ir_Hitachi.h"
#include "ir_Kelvinator.h"
#include "ir_Midea.h"
#include "ir_Mitsubishi.h"
#include "ir_MitsubishiHeavy.h"
#include "ir_Panasonic.h"
#include "ir_Samsung.h"
#include "ir_Tcl.h"
#include "ir_Teco.h"
#include "ir_Toshiba.h"
#include "ir_Trotec.h"
#include "ir_Vestel.h"
#include "ir_Whirlpool.h"
#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for IRac class.

TEST(TestIRac, Argo) {
  IRArgoAC ac(0);
  IRac irac(0);

  ac.begin();
  irac.argo(&ac,
            true,                        // Power
            stdAc::opmode_t::kHeat,      // Mode
            21,                          // Celsius
            stdAc::fanspeed_t::kHigh,    // Fan speed
            stdAc::swingv_t::kOff,       // Veritcal swing
            false,                       // Turbo
            -1);                         // Sleep
  EXPECT_TRUE(ac.getPower());
  EXPECT_EQ(1, ac.getMode());
  EXPECT_EQ(21, ac.getTemp());
  EXPECT_EQ(kArgoFlapAuto, ac.getFlap());
  EXPECT_FALSE(ac.getMax());  // Turbo
  EXPECT_FALSE(ac.getNight());  // Sleep
}

TEST(TestIRac, Coolix) {
  IRCoolixAC ac(0);
  IRac irac(0);
  IRrecv capture(0);
  char expected[] =
      "Power: On, Mode: 3 (HEAT), Fan: 1 (MAX), Temp: 21C, Zone Follow: Off, "
      "Sensor Temp: Ignored";

  ac.begin();
  irac.coolix(&ac,
              true,                        // Power
              stdAc::opmode_t::kHeat,      // Mode
              21,                          // Celsius
              stdAc::fanspeed_t::kHigh,    // Fan speed
              stdAc::swingv_t::kOff,       // Veritcal swing
              stdAc::swingh_t::kOff,       // Horizontal swing
              false,                       // Turbo
              false,                       // Light
              false,                       // Clean
              -1);                         // Sleep
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(COOLIX, ac._irsend.capture.decode_type);
  ASSERT_EQ(kCoolixBits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.value);
  ASSERT_EQ(expected, ac.toString());
}

TEST(TestIRac, Daikin) {
  IRDaikinESP ac(0);
  IRac irac(0);
  char expected[] =
      "Power: On, Mode: 3 (COOL), Temp: 19C, Fan: 2, Powerful: Off, "
      "Quiet: Off, Sensor: Off, Eye: Off, Mold: On, Comfort: Off, "
      "Swing (Horizontal): Off, Swing (Vertical): Off, "
      "Current Time: 0:00, On Time: Off, Off Time: Off";

  ac.begin();
  irac.daikin(&ac,
              true,                        // Power
              stdAc::opmode_t::kCool,      // Mode
              19,                          // Celsius
              stdAc::fanspeed_t::kMedium,  // Fan speed
              stdAc::swingv_t::kOff,       // Veritcal swing
              stdAc::swingh_t::kOff,       // Horizontal swing
              false,                       // Quiet
              false,                       // Turbo
              true,                        // Filter
              true);                       // Clean
  ASSERT_EQ(expected, ac.toString());
}

TEST(TestIRac, Daikin2) {
  IRDaikin2 ac(0);
  IRac irac(0);
  IRrecv capture(0);
  char expected[] =
      "Power: On, Mode: 3 (COOL), Temp: 19C, Fan: 2, Swing (V): 14 (Auto), "
      "Swing (H): 0, Clock: 0:00, On Time: Off, Off Time: Off, "
      "Sleep Time: Off, Beep: 1 (Quiet), Light: 1 (Bright), Mold: On, "
      "Clean: Off, Fresh Air: Off, Eye: Off, Eye Auto: Off, Quiet: Off, "
      "Powerful: Off, Purify: On, Econo: Off";

  ac.begin();
  irac.daikin2(&ac,
               true,                        // Power
               stdAc::opmode_t::kCool,      // Mode
               19,                          // Celsius
               stdAc::fanspeed_t::kMedium,  // Fan speed
               stdAc::swingv_t::kOff,       // Veritcal swing
               stdAc::swingh_t::kOff,       // Horizontal swing
               false,                       // Quiet
               false,                       // Turbo
               true,                        // Light
               false,                       // Econo
               true,                        // Filter
               true,                        // Clean (aka Mold)
               -1,                          // Sleep time
               -1);                         // Current time
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(DAIKIN2, ac._irsend.capture.decode_type);
  ASSERT_EQ(kDaikin2Bits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state);
  ASSERT_EQ(expected, ac.toString());
}

TEST(TestIRac, Daikin216) {
  IRDaikin216 ac(0);
  IRac irac(0);
  IRrecv capture(0);
  char expected[] =
      "Power: On, Mode: 4 (HEAT), Temp: 31C, Fan: 11 (QUIET), "
      "Swing (Horizontal): On, Swing (Vertical): On, Quiet: On";

  ac.begin();
  irac.daikin216(&ac,
                 true,                        // Power
                 stdAc::opmode_t::kHeat,      // Mode
                 31,                          // Celsius
                 stdAc::fanspeed_t::kMedium,  // Fan speed
                 stdAc::swingv_t::kAuto,      // Veritcal swing
                 stdAc::swingh_t::kLeft,      // Horizontal swing
                 true);                       // Quiet
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(DAIKIN216, ac._irsend.capture.decode_type);
  ASSERT_EQ(kDaikin216Bits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state);
  ASSERT_EQ(expected, ac.toString());
}

TEST(TestIRac, Fujitsu) {
  IRFujitsuAC ac(0);
  IRac irac(0);
  IRrecv capture(0);
  char expected[] =
      "Power: On, Mode: 1 (COOL), Temp: 19C, Fan: 2 (MED), "
      "Swing: Off, Command: N/A";

  ac.begin();
  irac.fujitsu(&ac,
               ARDB1,                       // Model
               true,                        // Power
               stdAc::opmode_t::kCool,      // Mode
               19,                          // Celsius
               stdAc::fanspeed_t::kMedium,  // Fan speed
               stdAc::swingv_t::kOff,       // Veritcal swing
               stdAc::swingh_t::kOff,       // Horizontal swing
               false);                      // Quiet
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(FUJITSU_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kFujitsuAcBits - 8, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state, ac._irsend.capture.bits / 8);
  ASSERT_EQ(expected, ac.toString());

  ac._irsend.reset();
  irac.fujitsu(&ac,
               ARRAH2E,                     // Model
               true,                        // Power
               stdAc::opmode_t::kCool,      // Mode
               19,                          // Celsius
               stdAc::fanspeed_t::kMedium,  // Fan speed
               stdAc::swingv_t::kOff,       // Veritcal swing
               stdAc::swingh_t::kOff,       // Horizontal swing
               false);                      // Quiet
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(FUJITSU_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kFujitsuAcBits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state, ac._irsend.capture.bits / 8);
  ASSERT_EQ(expected, ac.toString());
}

TEST(TestIRac, Gree) {
  IRGreeAC ac(0);
  IRac irac(0);
  IRrecv capture(0);
  char expected[] =
      "Power: On, Mode: 1 (COOL), Temp: 22C, Fan: 2, Turbo: Off, XFan: On, "
      "Light: On, Sleep: On, Swing Vertical Mode: Manual, "
      "Swing Vertical Pos: 3";

  ac.begin();
  irac.gree(&ac,
            true,                        // Power
            stdAc::opmode_t::kCool,      // Mode
            22,                          // Celsius
            stdAc::fanspeed_t::kMedium,  // Fan speed
            stdAc::swingv_t::kHigh,      // Veritcal swing
            false,                       // Turbo
            true,                        // Light
            true,                        // Clean (aka Mold/XFan)
            8 * 60 + 0);                 // Sleep time
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(GREE, ac._irsend.capture.decode_type);
  ASSERT_EQ(kGreeBits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state);
  ASSERT_EQ(expected, ac.toString());
}

TEST(TestIRac, Haier) {
  IRHaierAC ac(0);
  IRac irac(0);
  IRrecv capture(0);
  char expected[] =
      "Command: 1 (On), Mode: 3 (HEAT), Temp: 24C, Fan: 2, Swing: 1 (Up), "
      "Sleep: On, Health: On, Current Time: 13:45, On Timer: Off, "
      "Off Timer: Off";

  ac.begin();
  irac.haier(&ac,
             true,                        // Power
             stdAc::opmode_t::kCool,      // Mode
             24,                          // Celsius
             stdAc::fanspeed_t::kMedium,  // Fan speed
             stdAc::swingv_t::kHigh,      // Veritcal swing
             true,                        // Filter
             8 * 60 + 0,                  // Sleep time
             13 * 60 + 45);               // Clock
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(HAIER_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kHaierACBits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state);
  ASSERT_EQ(expected, ac.toString());
}


TEST(TestIRac, HaierYrwo2) {
  IRHaierACYRW02 ac(0);
  IRac irac(0);
  IRrecv capture(0);
  char expected[] =
      "Power: On, Button: 5 (Power), Mode: 2 (Cool), Temp: 23C, Fan: 4 (Med), "
      "Turbo: 1 (High), Swing: 1 (Top), Sleep: On, Health: On";

  ac.begin();
  irac.haierYrwo2(&ac,
             true,                        // Power
             stdAc::opmode_t::kCool,      // Mode
             23,                          // Celsius
             stdAc::fanspeed_t::kMedium,  // Fan speed
             stdAc::swingv_t::kHigh,      // Veritcal swing
             true,                        // Turbo
             true,                        // Filter
             8 * 60 + 0);                 // Sleep time
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(HAIER_AC_YRW02, ac._irsend.capture.decode_type);
  ASSERT_EQ(kHaierACYRW02Bits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state);
  ASSERT_EQ(expected, ac.toString());
}

TEST(TestIRac, Hitachi) {
  IRHitachiAc ac(0);
  IRac irac(0);
  IRrecv capture(0);
  char expected[] =
      "Power: On, Mode: 2 (AUTO), Temp: 22C, Fan: 3 (UNKNOWN), "
      "Swing (Vertical): Off, Swing (Horizontal): On";

  ac.begin();
  irac.hitachi(&ac,
               true,                        // Power
               stdAc::opmode_t::kAuto,      // Mode
               22,                          // Celsius
               stdAc::fanspeed_t::kMedium,  // Fan speed
               stdAc::swingv_t::kOff,       // Veritcal swing
               stdAc::swingh_t::kAuto);     // Horizontal swing

  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(HITACHI_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAcBits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state);
  ASSERT_EQ(expected, ac.toString());
}

TEST(TestIRac, Kelvinator) {
  IRKelvinatorAC ac(0);
  IRac irac(0);
  IRrecv capture(0);
  char expected[] =
      "Power: On, Mode: 1 (COOL), Temp: 19C, Fan: 3, Turbo: Off, Quiet: Off, "
      "XFan: On, IonFilter: On, Light: On, Swing (Horizontal): Off, "
      "Swing (Vertical): Off";

  ac.begin();
  irac.kelvinator(&ac,
                  true,                        // Power
                  stdAc::opmode_t::kCool,      // Mode
                  19,                          // Celsius
                  stdAc::fanspeed_t::kMedium,  // Fan speed
                  stdAc::swingv_t::kOff,       // Veritcal swing
                  stdAc::swingh_t::kOff,       // Horizontal swing
                  false,                       // Quiet
                  false,                       // Turbo
                  true,                        // Light
                  true,                        // Filter
                  true);                       // Clean

  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(KELVINATOR, ac._irsend.capture.decode_type);
  ASSERT_EQ(kKelvinatorBits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state);
  ASSERT_EQ(expected, ac.toString());
}

TEST(TestIRac, Midea) {
  IRMideaAC ac(0);
  IRac irac(0);
  IRrecv capture(0);
  char expected[] =
      "Power: On, Mode: 1 (DRY), Temp: 27C/81F, Fan: 2 (MED), Sleep: On";

  ac.begin();
  irac.midea(&ac,
             true,                        // Power
             stdAc::opmode_t::kDry,       // Mode
             27,                          // Celsius
             stdAc::fanspeed_t::kMedium,  // Fan speed
             8 * 60 + 0);                 // Sleep time

  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(MIDEA, ac._irsend.capture.decode_type);
  ASSERT_EQ(kMideaBits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.value);
  ASSERT_EQ(expected, ac.toString());
}

TEST(TestIRac, Mitsubishi) {
  IRMitsubishiAC ac(0);
  IRac irac(0);
  IRrecv capture(0);
  char expected[] =
      "Power: On (COOL), Temp: 20C, FAN: 2, VANE: AUTO, Time: 14:30, "
      "On timer: 00:00, Off timer: 00:00, Timer: -";

  ac.begin();
  irac.mitsubishi(&ac,
                  true,                        // Power
                  stdAc::opmode_t::kCool,      // Mode
                  20,                          // Celsius
                  stdAc::fanspeed_t::kMedium,  // Fan speed
                  stdAc::swingv_t::kOff,       // Veritcal swing
                  false,                       // Silent
                  14 * 60 + 35);               // Clock
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(MITSUBISHI_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kMitsubishiACBits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state);
  ASSERT_EQ(expected, ac.toString());
}

TEST(TestIRac, MitsubishiHeavy88) {
  IRMitsubishiHeavy88Ac ac(0);
  IRac irac(0);
  IRrecv capture(0);
  char expected[] =
      "Power: On, Mode: 1 (Cool), Temp: 21C, Fan: 3 (Med), "
      "Swing (V): 16 (Auto), Swing (H): 0 (Off), Turbo: Off, Econo: Off, "
      "3D: Off, Clean: On";

  ac.begin();
  irac.mitsubishiHeavy88(&ac,
                         true,                        // Power
                         stdAc::opmode_t::kCool,      // Mode
                         21,                          // Celsius
                         stdAc::fanspeed_t::kMedium,  // Fan speed
                         stdAc::swingv_t::kAuto,      // Veritcal swing
                         stdAc::swingh_t::kOff,       // Horizontal swing
                         false,                       // Turbo
                         false,                       // Econo
                         true);                       // Clean
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(MITSUBISHI_HEAVY_88, ac._irsend.capture.decode_type);
  ASSERT_EQ(kMitsubishiHeavy88Bits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state);
  ASSERT_EQ(expected, ac.toString());
}

TEST(TestIRac, MitsubishiHeavy152) {
  IRMitsubishiHeavy152Ac ac(0);
  IRac irac(0);
  IRrecv capture(0);
  char expected[] =
      "Power: On, Mode: 1 (Cool), Temp: 20C, Fan: 6 (Econo), "
      "Swing (V): 6 (Off), Swing (H): 0 (Auto), Silent: On, Turbo: Off, "
      "Econo: On, Night: On, Filter: On, 3D: Off, Clean: Off";

  ac.begin();
  irac.mitsubishiHeavy152(&ac,
                          true,                        // Power
                          stdAc::opmode_t::kCool,      // Mode
                          20,                          // Celsius
                          stdAc::fanspeed_t::kLow,     // Fan speed
                          stdAc::swingv_t::kOff,       // Veritcal swing
                          stdAc::swingh_t::kAuto,      // Horizontal swing
                          true,                        // Silent
                          false,                       // Turbo
                          true,                        // Econo
                          true,                        // Filter
                          false,                       // Clean
                          8 * 60);                     // Sleep
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(MITSUBISHI_HEAVY_152, ac._irsend.capture.decode_type);
  ASSERT_EQ(kMitsubishiHeavy152Bits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state);
  ASSERT_EQ(expected, ac.toString());
}

TEST(TestIRac, Panasonic) {
  IRPanasonicAc ac(0);
  IRac irac(0);
  IRrecv capture(0);
  char expected_nke[] =
      "Model: 2 (NKE), Power: On, Mode: 4 (HEAT), Temp: 28C, Fan: 2 (UNKNOWN), "
      "Swing (Vertical): 15 (AUTO), Swing (Horizontal): 6 (Middle), Quiet: On, "
      "Powerful: Off, Clock: 19:17, On Timer: Off, Off Timer: Off";

  ac.begin();
  irac.panasonic(&ac,
                 kPanasonicNke,               // Model
                 true,                        // Power
                 stdAc::opmode_t::kHeat,      // Mode
                 28,                          // Celsius
                 stdAc::fanspeed_t::kMedium,  // Fan speed
                 stdAc::swingv_t::kAuto,      // Veritcal swing
                 stdAc::swingh_t::kLeft,      // Horizontal swing
                 true,                        // Quiet
                 false,                       // Turbo
                 19 * 60 + 17);               // Clock
  ASSERT_EQ(expected_nke, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(PANASONIC_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kPanasonicAcBits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state);
  ASSERT_EQ(expected_nke, ac.toString());

  char expected_dke[] =
      "Model: 3 (DKE), Power: On, Mode: 3 (COOL), Temp: 18C, Fan: 4 (MAX), "
      "Swing (Vertical): 1 (Full Up), Swing (Horizontal): 6 (Middle), "
      "Quiet: Off, Powerful: On, Clock: 19:17, On Timer: Off, Off Timer: Off";
  ac._irsend.reset();
  irac.panasonic(&ac,
               kPanasonicDke,               // Model
               true,                        // Power
               stdAc::opmode_t::kCool,      // Mode
               18,                          // Celsius
               stdAc::fanspeed_t::kMax,     // Fan speed
               stdAc::swingv_t::kHigh,      // Veritcal swing
               stdAc::swingh_t::kMiddle,    // Horizontal swing
               false,                       // Quiet
               true,                        // Turbo
               19 * 60 + 17);               // Clock
  ASSERT_EQ(expected_dke, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(PANASONIC_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kPanasonicAcBits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state);
  ASSERT_EQ(expected_dke, ac.toString());
}

TEST(TestIRac, Samsung) {
  IRSamsungAc ac(0);
  IRac irac(0);
  IRrecv capture(0);
  char expected[] =
      "Power: On, Mode: 0 (AUTO), Temp: 28C, Fan: 6 (AUTO), Swing: On, "
      "Beep: On, Clean: On, Quiet: On";

  ac.begin();
  irac.samsung(&ac,
               true,                        // Power
               stdAc::opmode_t::kAuto,      // Mode
               28,                          // Celsius
               stdAc::fanspeed_t::kMedium,  // Fan speed
               stdAc::swingv_t::kAuto,      // Veritcal swing
               true,                        // Quiet
               false,                       // Turbo
               true,                        // Clean
               true,                        // Beep
               false);                      // with the Hack Off
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(SAMSUNG_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kSamsungAcBits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state);
  ASSERT_EQ(expected, ac.toString());

  ac._irsend.reset();
  irac.samsung(&ac,
               true,                        // Power
               stdAc::opmode_t::kAuto,      // Mode
               28,                          // Celsius
               stdAc::fanspeed_t::kMedium,  // Fan speed
               stdAc::swingv_t::kAuto,      // Veritcal swing
               true,                        // Quiet
               false,                       // Turbo
               true,                        // Clean
               true,                        // Beep
               true);                       // with the Hack On
  ASSERT_EQ(expected, ac.toString());  // Class should be in the desired mode.
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(SAMSUNG_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kSamsungAcExtendedBits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state);
  // However, we expect a plain "on" state as it should be sent before the
  // desired state.
  char expected_on[] =
      "Power: On, Mode: 0 (AUTO), Temp: 16C, Fan: 0 (AUTO), Swing: Off, "
      "Beep: Off, Clean: Off, Quiet: Off";
  ASSERT_EQ(expected_on, ac.toString());
}

TEST(TestIRac, Tcl112) {
  IRTcl112Ac ac(0);
  IRac irac(0);
  IRrecv capture(0);
  char expected[] =
      "Power: On, Mode: 3 (COOL), Temp: 20C, Fan: 3 (Med), Econo: On, "
      "Health: On, Light: On, Turbo: Off, Swing (H): On, Swing (V): Off";

  ac.begin();
  irac.tcl112(&ac,
              true,                        // Power
              stdAc::opmode_t::kCool,      // Mode
              20,                          // Celsius
              stdAc::fanspeed_t::kMedium,  // Fan speed
              stdAc::swingv_t::kOff,       // Veritcal swing
              stdAc::swingh_t::kAuto,      // Horizontal swing
              false,                       // Turbo
              true,                        // Light
              true,                        // Econo
              true);                       // Filter (aka. Health)
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(TCL112AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kTcl112AcBits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state);
  ASSERT_EQ(expected, ac.toString());
}

TEST(TestIRac, Teco) {
  IRTecoAc ac(0);
  IRac irac(0);
  IRrecv capture(0);
  char expected[] =
      "Power: On, Mode: 0 (AUTO), Temp: 21C, Fan: 2 (Med), Sleep: On, "
      "Swing: On";

  ac.begin();
  irac.teco(&ac,
            true,                        // Power
            stdAc::opmode_t::kAuto,      // Mode
            21,                          // Celsius
            stdAc::fanspeed_t::kMedium,  // Fan speed
            stdAc::swingv_t::kAuto,      // Veritcal swing
            8 * 60 + 30);                // Sleep
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(TECO, ac._irsend.capture.decode_type);
  ASSERT_EQ(kTecoBits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.value);
  ASSERT_EQ(expected, ac.toString());
}

TEST(TestIRac, Toshiba) {
  IRToshibaAC ac(0);
  IRac irac(0);
  IRrecv capture(0);
  char expected[] = "Power: On, Mode: 2 (DRY), Temp: 29C, Fan: 2";

  ac.begin();
  irac.toshiba(&ac,
               true,                      // Power
               stdAc::opmode_t::kDry,     // Mode
               29,                        // Celsius
               stdAc::fanspeed_t::kLow);  // Fan speed
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(TOSHIBA_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kToshibaACBits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state);
  ASSERT_EQ(expected, ac.toString());
}

TEST(TestIRac, Trotec) {
  IRTrotecESP ac(0);
  IRac irac(0);

  ac.begin();
  irac.trotec(&ac,
              true,                        // Power
              stdAc::opmode_t::kCool,      // Mode
              18,                          // Celsius
              stdAc::fanspeed_t::kHigh,    // Fan speed
              8 * 60 + 17);                // Sleep
  EXPECT_TRUE(ac.getPower());
  EXPECT_EQ(kTrotecCool, ac.getMode());
  EXPECT_EQ(18, ac.getTemp());
  EXPECT_EQ(kTrotecFanHigh, ac.getSpeed());
  EXPECT_TRUE(ac.getSleep());
}

TEST(TestIRac, Vestel) {
  IRVestelAc ac(0);
  IRac irac(0);
  IRrecv capture(0);
  char expected[] =
      "Power: On, Mode: 0 (AUTO), Temp: 22C, Fan: 5 (LOW), Sleep: On, "
      "Turbo: Off, Ion: On, Swing: On";

  ac.begin();
  irac.vestel(&ac,
              true,                      // Power
              stdAc::opmode_t::kAuto,    // Mode
              22,                        // Celsius
              stdAc::fanspeed_t::kLow,   // Fan speed
              stdAc::swingv_t::kHigh,    // Veritcal swing
              false,                     // Turbo
              true,                      // Filter
              8 * 60 + 0);               // Sleep time
              // 13 * 60 + 45);             // Clock
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(VESTEL_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kVestelAcBits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state);
  ASSERT_EQ(expected, ac.toString());

  ac._irsend.reset();
  char expected_clocks[] =
      "Time: 13:45, Timer: Off, On Timer: Off, Off Timer: Off";

  ac.begin();
  irac.vestel(&ac,
              true,                      // Power
              stdAc::opmode_t::kAuto,    // Mode
              22,                        // Celsius
              stdAc::fanspeed_t::kLow,   // Fan speed
              stdAc::swingv_t::kHigh,    // Veritcal swing
              false,                     // Turbo
              true,                      // Filter
              8 * 60 + 0,                // Sleep time
              13 * 60 + 45,              // Clock
              false);                    // Don't send the normal message.
                                         // Just for testing purposes.
  ASSERT_EQ(expected_clocks, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(VESTEL_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kVestelAcBits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state);
  ASSERT_EQ(expected_clocks, ac.toString());

  // Now check it sends both messages during normal operation when the
  // clock is set.
  ac._irsend.reset();
  ac.begin();
  irac.vestel(&ac,
              true,                      // Power
              stdAc::opmode_t::kAuto,    // Mode
              22,                        // Celsius
              stdAc::fanspeed_t::kLow,   // Fan speed
              stdAc::swingv_t::kHigh,    // Veritcal swing
              false,                     // Turbo
              true,                      // Filter
              8 * 60 + 0,                // Sleep time
              13 * 60 + 45);             // Clock

  EXPECT_EQ(
      "f38000d50"
      "m3110s9066"
      "m520s1535m520s480m520s480m520s480m520s480m520s480m520s480m520s480"
      "m520s480m520s1535m520s480m520s480m520s480m520s480m520s480m520s480"
      "m520s1535m520s1535m520s1535m520s1535m520s480m520s1535m520s480m520s1535"
      "m520s1535m520s1535m520s480m520s480m520s480m520s480m520s480m520s480"
      "m520s480m520s480m520s480m520s480m520s480m520s1535m520s1535m520s480"
      "m520s1535m520s480m520s1535m520s480m520s480m520s480m520s480m520s480"
      "m520s480m520s480m520s1535m520s480m520s1535m520s1535m520s1535m520s1535"
      "m520s100000"
      "m3110s9066"
      "m520s1535m520s480m520s480m520s480m520s480m520s480m520s480m520s480"
      "m520s480m520s1535m520s480m520s480m520s480m520s1535m520s1535m520s480"
      "m520s1535m520s1535m520s1535m520s1535m520s480m520s480m520s480m520s480"
      "m520s480m520s480m520s480m520s480m520s480m520s480m520s480m520s480"
      "m520s480m520s480m520s480m520s480m520s1535m520s480m520s1535m520s1535"
      "m520s480m520s480m520s480m520s480m520s1535m520s480m520s1535m520s1535"
      "m520s480m520s1535m520s480m520s480m520s480m520s480m520s480m520s480"
      "m520s100000", ac._irsend.outputStr());
}


TEST(TestIRac, Whirlpool) {
  IRWhirlpoolAc ac(0);
  IRac irac(0);
  IRrecv capture(0);
  char expected[] =
      "Model: 1 (DG11J13A), Power toggle: On, Mode: 1 (AUTO), Temp: 21C, "
      "Fan: 3 (LOW), Swing: On, Light: On, Clock: 23:58, On Timer: Off, "
      "Off Timer: Off, Sleep: On, Super: Off, Command: 1 (POWER)";

  ac.begin();
  irac.whirlpool(&ac,
                 DG11J13A,
                 true,                        // Power
                 stdAc::opmode_t::kAuto,      // Mode
                 21,                          // Celsius
                 stdAc::fanspeed_t::kMedium,  // Fan speed
                 stdAc::swingv_t::kAuto,      // Veritcal swing
                 false,                       // Turbo
                 true,                        // Light
                 8 * 60 + 30,                 // Sleep
                 23 * 60 + 58);               // Clock
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(WHIRLPOOL_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kWhirlpoolAcBits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state);
  ASSERT_EQ(expected, ac.toString());
}

TEST(TestIRac, strToBool) {
  EXPECT_TRUE(IRac::strToBool("ON"));
  EXPECT_TRUE(IRac::strToBool("1"));
  EXPECT_TRUE(IRac::strToBool("TRUE"));
  EXPECT_TRUE(IRac::strToBool("YES"));
  EXPECT_FALSE(IRac::strToBool("OFF"));
  EXPECT_FALSE(IRac::strToBool("0"));
  EXPECT_FALSE(IRac::strToBool("FALSE"));
  EXPECT_FALSE(IRac::strToBool("NO"));
  EXPECT_FALSE(IRac::strToBool("FOOBAR"));
  EXPECT_TRUE(IRac::strToBool("FOOBAR", true));
}

TEST(TestIRac, strToOpmode) {
  EXPECT_EQ(stdAc::opmode_t::kAuto, IRac::strToOpmode("AUTO"));
  EXPECT_EQ(stdAc::opmode_t::kCool, IRac::strToOpmode("COOL"));
  EXPECT_EQ(stdAc::opmode_t::kHeat, IRac::strToOpmode("HEAT"));
  EXPECT_EQ(stdAc::opmode_t::kDry, IRac::strToOpmode("DRY"));
  EXPECT_EQ(stdAc::opmode_t::kFan, IRac::strToOpmode("FAN"));
  EXPECT_EQ(stdAc::opmode_t::kFan, IRac::strToOpmode("FAN_ONLY"));
  EXPECT_EQ(stdAc::opmode_t::kAuto, IRac::strToOpmode("FOOBAR"));
  EXPECT_EQ(stdAc::opmode_t::kOff, IRac::strToOpmode("OFF"));
  EXPECT_EQ(stdAc::opmode_t::kOff, IRac::strToOpmode("FOOBAR",
                                                     stdAc::opmode_t::kOff));
}

TEST(TestIRac, strToFanspeed) {
  EXPECT_EQ(stdAc::fanspeed_t::kAuto, IRac::strToFanspeed("AUTO"));
  EXPECT_EQ(stdAc::fanspeed_t::kMin, IRac::strToFanspeed("MIN"));
  EXPECT_EQ(stdAc::fanspeed_t::kLow, IRac::strToFanspeed("LOW"));
  EXPECT_EQ(stdAc::fanspeed_t::kMedium, IRac::strToFanspeed("MEDIUM"));
  EXPECT_EQ(stdAc::fanspeed_t::kHigh, IRac::strToFanspeed("HIGH"));
  EXPECT_EQ(stdAc::fanspeed_t::kMax, IRac::strToFanspeed("MAX"));
  EXPECT_EQ(stdAc::fanspeed_t::kAuto, IRac::strToFanspeed("FOOBAR"));
  EXPECT_EQ(stdAc::fanspeed_t::kMin,
            IRac::strToFanspeed("FOOBAR", stdAc::fanspeed_t::kMin));
}

TEST(TestIRac, strToSwingV) {
  EXPECT_EQ(stdAc::swingv_t::kAuto, IRac::strToSwingV("AUTO"));
  EXPECT_EQ(stdAc::swingv_t::kLowest, IRac::strToSwingV("LOWEST"));
  EXPECT_EQ(stdAc::swingv_t::kLow, IRac::strToSwingV("LOW"));
  EXPECT_EQ(stdAc::swingv_t::kMiddle, IRac::strToSwingV("MIDDLE"));
  EXPECT_EQ(stdAc::swingv_t::kHigh, IRac::strToSwingV("HIGH"));
  EXPECT_EQ(stdAc::swingv_t::kHighest, IRac::strToSwingV("HIGHEST"));
  EXPECT_EQ(stdAc::swingv_t::kOff, IRac::strToSwingV("OFF"));
  EXPECT_EQ(stdAc::swingv_t::kOff, IRac::strToSwingV("FOOBAR"));
  EXPECT_EQ(stdAc::swingv_t::kAuto,
            IRac::strToSwingV("FOOBAR", stdAc::swingv_t::kAuto));
}

TEST(TestIRac, strToSwingH) {
  EXPECT_EQ(stdAc::swingh_t::kAuto, IRac::strToSwingH("AUTO"));
  EXPECT_EQ(stdAc::swingh_t::kLeftMax, IRac::strToSwingH("MAX LEFT"));
  EXPECT_EQ(stdAc::swingh_t::kLeft, IRac::strToSwingH("LEFT"));
  EXPECT_EQ(stdAc::swingh_t::kMiddle, IRac::strToSwingH("CENTRE"));
  EXPECT_EQ(stdAc::swingh_t::kRight, IRac::strToSwingH("RIGHT"));
  EXPECT_EQ(stdAc::swingh_t::kRightMax, IRac::strToSwingH("RIGHTMAX"));
  EXPECT_EQ(stdAc::swingh_t::kOff, IRac::strToSwingH("OFF"));
  EXPECT_EQ(stdAc::swingh_t::kOff, IRac::strToSwingH("FOOBAR"));
  EXPECT_EQ(stdAc::swingh_t::kAuto,
            IRac::strToSwingH("FOOBAR", stdAc::swingh_t::kAuto));
}

TEST(TestIRac, strToModel) {
  EXPECT_EQ(panasonic_ac_remote_model_t::kPanasonicLke,
            IRac::strToModel("LKE"));
  EXPECT_EQ(panasonic_ac_remote_model_t::kPanasonicLke,
            IRac::strToModel("PANASONICLKE"));
  EXPECT_EQ(fujitsu_ac_remote_model_t::ARRAH2E,
            IRac::strToModel("ARRAH2E"));
  EXPECT_EQ(whirlpool_ac_remote_model_t::DG11J13A,
            IRac::strToModel("DG11J13A"));
  EXPECT_EQ(1, IRac::strToModel("1"));
  EXPECT_EQ(10, IRac::strToModel("10"));
  EXPECT_EQ(-1, IRac::strToModel("0"));
  EXPECT_EQ(-1, IRac::strToModel("FOOBAR"));
  EXPECT_EQ(0, IRac::strToModel("FOOBAR", 0));
}
