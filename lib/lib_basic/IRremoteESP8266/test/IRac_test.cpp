// Copyright 2019-2021 David Conran

#include <string>
#include "ir_Airton.h"
#include "ir_Airwell.h"
#include "ir_Amcor.h"
#include "ir_Argo.h"
#include "ir_Carrier.h"
#include "ir_Coolix.h"
#include "ir_Corona.h"
#include "ir_Daikin.h"
#include "ir_Delonghi.h"
#include "ir_Ecoclim.h"
#include "ir_Electra.h"
#include "ir_Fujitsu.h"
#include "ir_Goodweather.h"
#include "ir_Gree.h"
#include "ir_Haier.h"
#include "ir_Hitachi.h"
#include "ir_Kelvinator.h"
#include "ir_LG.h"
#include "ir_Midea.h"
#include "ir_Mirage.h"
#include "ir_Mitsubishi.h"
#include "ir_MitsubishiHeavy.h"
#include "ir_Neoclima.h"
#include "ir_Panasonic.h"
#include "ir_Samsung.h"
#include "ir_Sharp.h"
#include "ir_Tcl.h"
#include "ir_Teco.h"
#include "ir_Toshiba.h"
#include "ir_Trotec.h"
#include "ir_Truma.h"
#include "ir_Vestel.h"
#include "ir_Voltas.h"
#include "ir_Whirlpool.h"
#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "gtest/gtest.h"

// Tests for IRac class.

TEST(TestIRac, Airton) {
  IRAirtonAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  const char expected[] =
      "Power: On, Mode: 1 (Cool), Fan: 5 (Maximum), Temp: 18C, "
      "Swing(V): On, Econo: On, Turbo: On, Light: On, Health: On, Sleep: On";

  ac.begin();
  irac.airton(&ac,
              true,                        // Power
              stdAc::opmode_t::kCool,      // Mode
              18,                          // Celsius
              stdAc::fanspeed_t::kMax,     // Fan speed
              stdAc::swingv_t::kAuto,      // Vertical Swing
              true,                        // Turbo
              true,                        // Light/Display/LED
              true,                        // Econo (Eco)
              true,                        // Filter (Health)
              9 * 60 + 12);                // Sleep (09:12)
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(AIRTON, ac._irsend.capture.decode_type);
  ASSERT_EQ(kAirtonBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Airwell) {
  IRAirwellAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power Toggle: On, Mode: 3 (Auto), Fan: 1 (Medium), Temp: 18C";

  ac.begin();
  irac.airwell(&ac,
               true,                        // Power
               stdAc::opmode_t::kAuto,      // Mode
               18,                          // Celsius
               stdAc::fanspeed_t::kMedium);   // Fan speed
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(AIRWELL, ac._irsend.capture.decode_type);
  ASSERT_EQ(kAirwellBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Amcor) {
  IRAmcorAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 5 (Auto), Fan: 3 (High), Temp: 19C, Max: Off";

  ac.begin();
  irac.amcor(&ac,
             true,                        // Power
             stdAc::opmode_t::kAuto,      // Mode
             19,                          // Celsius
             stdAc::fanspeed_t::kHigh);   // Fan speed
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(AMCOR, ac._irsend.capture.decode_type);
  ASSERT_EQ(kAmcorBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Argo) {
  IRArgoAC ac(kGpioUnused);
  IRac irac(kGpioUnused);

  ac.begin();
  irac.argo(&ac,
            true,                        // Power
            stdAc::opmode_t::kHeat,      // Mode
            21,                          // Celsius
            stdAc::fanspeed_t::kHigh,    // Fan speed
            stdAc::swingv_t::kOff,       // Vertical swing
            false,                       // Turbo
            -1);                         // Sleep
  EXPECT_TRUE(ac.getPower());
  EXPECT_EQ(kArgoHeat, ac.getMode());
  EXPECT_EQ(21, ac.getTemp());
  EXPECT_EQ(kArgoFlapAuto, ac.getFlap());
  EXPECT_FALSE(ac.getMax());  // Turbo
  EXPECT_FALSE(ac.getNight());  // Sleep
}

TEST(TestIRac, Carrier64) {
  IRCarrierAc64 ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);

  char expected[] =
      "Power: On, Mode: 1 (Heat), Temp: 21C, Fan: 3 (High), Swing(V): On, "
      "Sleep: On, On Timer: Off, Off Timer: Off";

  ac.begin();
  irac.carrier64(&ac,
            true,                        // Power
            stdAc::opmode_t::kHeat,      // Mode
            21,                          // Celsius
            stdAc::fanspeed_t::kHigh,    // Fan speed
            stdAc::swingv_t::kAuto,      // Vertical swing
            1);                          // Sleep
  EXPECT_TRUE(ac.getPower());  // Power.
  EXPECT_EQ(kCarrierAc64Heat, ac.getMode());  // Operating mode.
  EXPECT_EQ(21, ac.getTemp());  // Temperature.
  EXPECT_EQ(kCarrierAc64FanHigh, ac.getFan());  // Fan Speed
  EXPECT_TRUE(ac.getSwingV());  // SwingV
  EXPECT_TRUE(ac.getSleep());  // Sleep

  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(CARRIER_AC64, ac._irsend.capture.decode_type);
  ASSERT_EQ(kCarrierAc64Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Coolix) {
  IRCoolixAC ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 3 (Heat), Fan: 1 (Max), Temp: 21C, Zone Follow: Off, "
      "Sensor Temp: Off";

  ac.begin();
  irac.coolix(&ac,
              true,                        // Power
              stdAc::opmode_t::kHeat,      // Mode
              21,                          // Celsius
              stdAc::fanspeed_t::kHigh,    // Fan speed
              stdAc::swingv_t::kOff,       // Vertical swing
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
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
  // Confirm we are sending with a repeat of 1. i.e. two messages.
  EXPECT_EQ(
      "f38000d50"  // 38kHz Frequency and 50% duty-cycle.
      // Start of message #1 (i.e. Repeat '0')
      // Header
      "m4692s4416"
      // Data
      "m552s1656m552s552m552s1656m552s1656m552s552m552s552m552s1656m552s552"
      "m552s552m552s1656m552s552m552s552m552s1656m552s1656m552s552m552s1656"
      "m552s552m552s552m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s1656m552s1656m552s552m552s552m552s552m552s552m552s552m552s552"
      "m552s552m552s1656m552s1656m552s552m552s1656m552s1656m552s552m552s552"
      "m552s1656m552s552m552s552m552s1656m552s552m552s552m552s1656m552s1656"
      // Footer
      "m552s5244"
      // End of message #1 (i.e. Repeat '0')
      // Start of message #2 (i.e. Repeat '1')
      // Header
      "m4692s4416"
      // Data
      "m552s1656m552s552m552s1656m552s1656m552s552m552s552m552s1656m552s552"
      "m552s552m552s1656m552s552m552s552m552s1656m552s1656m552s552m552s1656"
      "m552s552m552s552m552s1656m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s1656m552s1656m552s552m552s552m552s552m552s552m552s552m552s552"
      "m552s552m552s1656m552s1656m552s552m552s1656m552s1656m552s552m552s552"
      "m552s1656m552s552m552s552m552s1656m552s552m552s552m552s1656m552s1656"
      // Footer
      "m552s105244",
      // End of message #2 (i.e. Repeat '1')
      // Note: the two messages (#1 & #2) are identical.
      ac._irsend.outputStr());
}

TEST(TestIRac, Corona) {
  IRCoronaAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);

  char expectedAfterSent[] =
      "Power: On, Power Button: On, Mode: 0 (Heat), Temp: 21C, "
      "Fan: 3 (High), Swing(V) Toggle: On, Econo: On, "
      "On Timer: Off, Off Timer: Off";

  char expectedCapture[] =
      "Power: On, Power Button: Off, Mode: 0 (Heat), Temp: 21C, "
      "Fan: 3 (High), Swing(V) Toggle: On, Econo: On, "
      "On Timer: Off, Off Timer: Off";

  ac.begin();
  // this sends as well
  irac.corona(&ac,
              true,                        // Power
              stdAc::opmode_t::kHeat,      // Mode
              21,                          // Celsius
              stdAc::fanspeed_t::kHigh,    // Fan speed
              stdAc::swingv_t::kAuto,      // Vertical swing
              true);                       // Econo (PowerSave)
  EXPECT_TRUE(ac.getPower());  // Power.
  EXPECT_TRUE(ac.getPowerButton());  // Power.button
  EXPECT_EQ(kCoronaAcModeHeat, ac.getMode());  // Operating mode.
  EXPECT_EQ(21, ac.getTemp());  // Temperature.
  EXPECT_EQ(kCoronaAcFanHigh, ac.getFan());  // Fan Speed
  EXPECT_TRUE(ac.getSwingVToggle());  // SwingV
  EXPECT_TRUE(ac.getEcono());  // Econo (PowerSave)

  ASSERT_EQ(expectedAfterSent, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(CORONA_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kCoronaAcBits, ac._irsend.capture.bits);
  ASSERT_EQ(expectedCapture, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Daikin) {
  IRDaikinESP ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 3 (Cool), Temp: 19C, Fan: 5 (High), Powerful: Off, "
      "Quiet: Off, Sensor: Off, Mould: On, Comfort: Off, "
      "Swing(H): Off, Swing(V): Off, "
      "Clock: 00:00, Day: 0 (UNKNOWN), On Timer: Off, "
      "Off Timer: Off, Weekly Timer: On";

  ac.begin();
  irac.daikin(&ac,
              true,                        // Power
              stdAc::opmode_t::kCool,      // Mode
              19,                          // Celsius
              stdAc::fanspeed_t::kMax,     // Fan speed
              stdAc::swingv_t::kOff,       // Vertical swing
              stdAc::swingh_t::kOff,       // Horizontal swing
              false,                       // Quiet
              false,                       // Turbo
              true,                        // Filter
              true);                       // Clean
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(DAIKIN, ac._irsend.capture.decode_type);
  ASSERT_EQ(kDaikinBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Daikin128) {
  IRDaikin128 ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power Toggle: On, Mode: 8 (Heat), Temp: 27C, Fan: 9 (Quiet), "
      "Powerful: Off, Quiet: On, Swing(V): On, Sleep: On, "
      "Econo: Off, Clock: 21:57, On Timer: Off, On Timer: 00:00, "
      "Off Timer: Off, Off Timer: 00:00, Light Toggle: 8 (Wall)";

  ac.begin();
  irac.daikin128(&ac,
                 true,                        // Power
                 stdAc::opmode_t::kHeat,      // Mode
                 27,                          // Celsius
                 stdAc::fanspeed_t::kMin,     // Fan speed
                 stdAc::swingv_t::kAuto,      // Vertical swing
                 true,                        // Quiet
                 false,                       // Turbo
                 true,                        // Light
                 false,                       // Econo
                 18 * 60,                     // Sleep
                 21 * 60 + 57);               // Clock
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(DAIKIN128, ac._irsend.capture.decode_type);
  ASSERT_EQ(kDaikin128Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Daikin152) {
  IRDaikin152 ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 3 (Cool), Temp: 27C, Fan: 3 (Medium), Swing(V): On, "
      "Powerful: Off, Quiet: Off, Econo: On, Sensor: Off, Comfort: Off";

  ac.begin();
  irac.daikin152(&ac,
                 true,                        // Power
                 stdAc::opmode_t::kCool,      // Mode
                 27,                          // Celsius
                 stdAc::fanspeed_t::kMedium,  // Fan speed
                 stdAc::swingv_t::kAuto,      // Vertical swing
                 false,                       // Quiet
                 false,                       // Turbo
                 true);                       // Econo
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(DAIKIN152, ac._irsend.capture.decode_type);
  ASSERT_EQ(kDaikin152Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Daikin160) {
  IRDaikin160 ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 2 (Dry), Temp: 23C, Fan: 1 (Low), "
      "Swing(V): 3 (Middle)";

  ac.begin();
  irac.daikin160(&ac,
                 true,                        // Power
                 stdAc::opmode_t::kDry,       // Mode
                 23,                          // Celsius
                 stdAc::fanspeed_t::kMin,     // Fan speed
                 stdAc::swingv_t::kMiddle);   // Vertical swing
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(DAIKIN160, ac._irsend.capture.decode_type);
  ASSERT_EQ(kDaikin160Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Daikin176) {
  IRDaikin176 ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 2 (Cool), Temp: 26C, Fan: 1 (Low), Swing(H): 5 (Auto), "
      "Id: 0";

  ac.begin();
  irac.daikin176(&ac,
                 true,                        // Power
                 stdAc::opmode_t::kCool,      // Mode
                 26,                          // Celsius
                 stdAc::fanspeed_t::kLow,     // Fan speed
                 stdAc::swingh_t::kAuto);     // Horizontal swing
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(DAIKIN176, ac._irsend.capture.decode_type);
  ASSERT_EQ(kDaikin176Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Daikin2) {
  IRDaikin2 ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 3 (Cool), Temp: 19C, Fan: 1 (Low), "
      "Swing(V): 14 (Off), Swing(H): 170 (Middle), Clock: 00:00, "
      "On Timer: Off, Off Timer: Off, Sleep Timer: Off, Beep: 2 (Loud), "
      "Light: 1 (High), Mould: On, Clean: On, Fresh: Off, Eye: Off, "
      "Eye Auto: Off, Quiet: Off, Powerful: Off, Purify: On, Econo: Off, "
      "Humid: 0 (Off)";

  ac.begin();
  irac.daikin2(&ac,
               true,                        // Power
               stdAc::opmode_t::kCool,      // Mode
               19,                          // Celsius
               stdAc::fanspeed_t::kLow,     // Fan speed
               stdAc::swingv_t::kOff,       // Vertical swing
               stdAc::swingh_t::kMiddle,    // Horizontal swing
               false,                       // Quiet
               false,                       // Turbo
               true,                        // Light
               false,                       // Econo
               true,                        // Filter (aka Purify)
               true,                        // Clean (aka Mold)
               true,                        // Beep (Loud)
               -1,                          // Sleep time
               -1);                         // Current time
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(DAIKIN2, ac._irsend.capture.decode_type);
  ASSERT_EQ(kDaikin2Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Daikin216) {
  IRDaikin216 ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 4 (Heat), Temp: 31C, Fan: 11 (Quiet), "
      "Swing(H): On, Swing(V): On, Quiet: On, Powerful: Off";

  ac.begin();
  irac.daikin216(&ac,
                 true,                        // Power
                 stdAc::opmode_t::kHeat,      // Mode
                 31,                          // Celsius
                 stdAc::fanspeed_t::kMedium,  // Fan speed
                 stdAc::swingv_t::kAuto,      // Vertical swing
                 stdAc::swingh_t::kLeft,      // Horizontal swing
                 true,                        // Quiet
                 false);                      // Turbo (Powerful)
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(DAIKIN216, ac._irsend.capture.decode_type);
  ASSERT_EQ(kDaikin216Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Daikin64) {
  IRDaikin64 ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power Toggle: On, Mode: 2 (Cool), Temp: 27C, Fan: 8 (Low), "
      "Turbo: Off, Quiet: Off, Swing(V): On, Sleep: On, "
      "Clock: 17:59, On Timer: Off, Off Timer: Off";

  ac.begin();
  irac.daikin64(&ac,
                true,                        // Power (Toggle)
                stdAc::opmode_t::kCool,      // Mode
                27,                          // Celsius
                stdAc::fanspeed_t::kLow,     // Fan Speed
                stdAc::swingv_t::kAuto,      // Vertical swing
                false,                       // Quiet
                false,                       // Turbo
                360,                         // Sleep
                17 * 60 + 59);               // Clock
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(DAIKIN64, ac._irsend.capture.decode_type);
  ASSERT_EQ(kDaikin64Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
}

TEST(TestIRac, DelonghiAc) {
  IRDelonghiAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 0 (Cool), Fan: 2 (Medium), Temp: 77F, "
      "Turbo: On, Sleep: On, On Timer: Off, Off Timer: Off";

  ac.begin();
  irac.delonghiac(&ac,
                true,                        // Power
                stdAc::opmode_t::kCool,      // Mode
                false,                       // Celsius (i.e. Fahrenheit)
                77,                          // Degrees (F)
                stdAc::fanspeed_t::kMedium,  // Fan Speed
                true,                        // Turbo
                360);                        // Sleep
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(decode_type_t::DELONGHI_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kDelonghiAcBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
}

TEST(TestIRac, Ecoclim) {
  IREcoclimAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 1 (Cool), Temp: 26C, SensorTemp: 26C, Fan: 2 (High), "
      "Clock: 12:34, On Timer: Off, Off Timer: Off, Type: 0";

  ac.begin();
  irac.ecoclim(&ac,
               true,                        // Power
               stdAc::opmode_t::kCool,      // Mode
               26,                          // Celsius
               stdAc::fanspeed_t::kHigh,    // Fan speed
               -1,                          // Sleep
               12 * 60 + 34);               // Clock
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(ECOCLIM, ac._irsend.capture.decode_type);
  ASSERT_EQ(kEcoclimBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));

  char expected_sleep[] =
      "Power: On, Mode: 7 (Sleep), Temp: 21C, SensorTemp: 21C, Fan: 0 (Low), "
      "Clock: 17:17, On Timer: Off, Off Timer: Off, Type: 0";

  ac._irsend.reset();
  irac.ecoclim(&ac,
               true,                        // Power
               stdAc::opmode_t::kCool,      // Mode
               21,                          // Celsius
               stdAc::fanspeed_t::kLow,     // Fan speed
               8 * 60,                      // Sleep
               17 * 60 + 17);               // Clock
  ASSERT_EQ(expected_sleep, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(ECOCLIM, ac._irsend.capture.decode_type);
  ASSERT_EQ(kEcoclimBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected_sleep, IRAcUtils::resultAcToString(&ac._irsend.capture));
}

TEST(TestIRac, Electra) {
  IRElectraAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 6 (Fan), Temp: 26C, Fan: 1 (High), "
      "Swing(V): On, Swing(H): On, Light: Toggle, Clean: On, Turbo: On, "
      "IFeel: Off";

  ac.begin();
  irac.electra(&ac,
               true,                        // Power
               stdAc::opmode_t::kFan,       // Mode
               26,                          // Celsius
               stdAc::fanspeed_t::kHigh,    // Fan speed
               stdAc::swingv_t::kAuto,      // Vertical swing
               stdAc::swingh_t::kLeft,      // Horizontal swing
               true,                        // Turbo
               true,                        // Light (toggle)
               true);                       // Clean
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(ELECTRA_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kElectraAcBits, ac._irsend.capture.bits);
  ac.setRaw(ac._irsend.capture.state);
  ASSERT_EQ(expected, ac.toString());
}

TEST(TestIRac, Fujitsu) {
  IRFujitsuAC ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  std::string ardb1_expected =
      "Model: 2 (ARDB1), Id: 0, Power: On, Mode: 1 (Cool), Temp: 19C, "
      "Fan: 2 (Medium), Command: N/A";
  std::string arrah2e_expected =
      "Model: 1 (ARRAH2E), Id: 0, Power: On, Mode: 1 (Cool), Temp: 19C, "
      "Fan: 2 (Medium), Clean: Off, Filter: Off, Swing: 0 (Off), Command: N/A, "
      "Sleep Timer: 03:00";
  std::string arry4_expected =
      "Model: 5 (ARRY4), Id: 0, Power: On, Mode: 1 (Cool), Temp: 19C, "
      "Fan: 2 (Medium), Clean: On, Filter: On, Swing: 0 (Off), Command: N/A";
  std::string arrew4e_expected =
      "Model: 6 (ARREW4E), Id: 0, Power: On, Mode: 1 (Cool), Temp: 73F, "
      "Fan: 1 (High), 10C Heat: Off, Swing: 0 (Off), Command: N/A, "
      "Outside Quiet: Off, Timer: Off";
  ac.begin();
  irac.fujitsu(&ac,
               ARDB1,                       // Model
               true,                        // Power
               stdAc::opmode_t::kCool,      // Mode
               true,                        // Celsius
               19,                          // Degrees
               stdAc::fanspeed_t::kMedium,  // Fan speed
               stdAc::swingv_t::kOff,       // Vertical swing
               stdAc::swingh_t::kOff,       // Horizontal swing
               false,                       // Quiet
               false,                       // Turbo (Powerful)
               false,                       // Econo
               true,                        // Filter
               true);                       // Clean
  ASSERT_EQ(ardb1_expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(FUJITSU_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kFujitsuAcBits - 8, ac._irsend.capture.bits);
  ASSERT_EQ(ardb1_expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));

  ac._irsend.reset();
  irac.fujitsu(&ac,
               ARRAH2E,                     // Model
               true,                        // Power
               stdAc::opmode_t::kCool,      // Mode
               true,                        // Celsius
               19,                          // Degrees
               stdAc::fanspeed_t::kMedium,  // Fan speed
               stdAc::swingv_t::kOff,       // Vertical swing
               stdAc::swingh_t::kOff,       // Horizontal swing
               false,                       // Quiet
               false,                       // Turbo (Powerful)
               false,                       // Econo
               true,                        // Filter
               true,                        // Clean
               3 * 60);                     // Sleep
  ASSERT_EQ(arrah2e_expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(FUJITSU_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kFujitsuAcBits, ac._irsend.capture.bits);
  ASSERT_EQ(arrah2e_expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
  ac._irsend.reset();
  irac.fujitsu(&ac,
               fujitsu_ac_remote_model_t::ARRY4,  // Model
               true,                        // Power
               stdAc::opmode_t::kCool,      // Mode
               true,                        // Celsius
               19,                          // Degrees
               stdAc::fanspeed_t::kMedium,  // Fan speed
               stdAc::swingv_t::kOff,       // Vertical swing
               stdAc::swingh_t::kOff,       // Horizontal swing
               false,                       // Quiet
               false,                       // Turbo (Powerful)
               false,                       // Econo
               true,                        // Filter
               true);                       // Clean
  ASSERT_EQ(arry4_expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(FUJITSU_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kFujitsuAcBits, ac._irsend.capture.bits);
  ASSERT_EQ(arry4_expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));

  ac._irsend.reset();
  irac.fujitsu(&ac,
               ARREW4E,                     // Model
               true,                        // Power
               stdAc::opmode_t::kCool,      // Mode
               false,                       // Fahrenheit
               73,                          // Degrees
               stdAc::fanspeed_t::kHigh,    // Fan speed
               stdAc::swingv_t::kOff,       // Vertical swing
               stdAc::swingh_t::kOff,       // Horizontal swing
               false,                       // Quiet
               false,                       // Turbo (Powerful)
               false,                       // Econo
               false,                        // Filter
               false);                       // Clean
  ASSERT_EQ(arrew4e_expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(FUJITSU_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kFujitsuAcBits, ac._irsend.capture.bits);
  ASSERT_EQ(arrew4e_expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
}

TEST(TestIRac, Goodweather) {
  IRGoodweatherAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 1 (Cool), Temp: 19C, Fan: 2 (Medium), Turbo: Toggle, "
      "Light: Toggle, Sleep: Toggle, Swing: 1 (Slow), Command: 0 (Power)";

  ac.begin();
  irac.goodweather(&ac,
                   true,                        // Power
                   stdAc::opmode_t::kCool,      // Mode
                   19,                          // Celsius
                   stdAc::fanspeed_t::kMedium,  // Fan speed
                   stdAc::swingv_t::kHigh,      // Vertical swing
                   true,                        // Turbo
                   true,                        // Light
                   8 * 60 + 0);                 // Sleep time
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(GOODWEATHER, ac._irsend.capture.decode_type);
  ASSERT_EQ(kGoodweatherBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Gree) {
  IRGreeAC ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Model: 1 (YAW1F), Power: On, Mode: 1 (Cool), Temp: 71F, "
      "Fan: 2 (Medium), Turbo: Off, Econo: Off, IFeel: Off, WiFi: Off, "
      "XFan: On, Light: On, Sleep: On, Swing(V) Mode: Manual, "
      "Swing(V): 3 (UNKNOWN), Swing(H): 5 (Right), Timer: Off, "
      "Display Temp: 0 (Off)";

  ac.begin();
  irac.gree(&ac,
            gree_ac_remote_model_t::YAW1F,  // Model
            true,                           // Power
            stdAc::opmode_t::kCool,         // Mode
            false,                          // Celsius
            71,                             // Degrees (F)
            stdAc::fanspeed_t::kMedium,     // Fan speed
            stdAc::swingv_t::kHigh,         // Vertical swing
            stdAc::swingh_t::kRight,        // Horizontal swing
            false,                          // Turbo
            false,                          // Econo
            true,                           // Light
            true,                           // Clean (aka Mold/XFan)
            8 * 60 + 0);                    // Sleep time
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(GREE, ac._irsend.capture.decode_type);
  ASSERT_EQ(kGreeBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Haier) {
  IRHaierAC ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Command: 1 (On), Mode: 1 (Cool), Temp: 24C, Fan: 2 (Medium), "
      "Swing(V): 1 (Up), Sleep: On, Health: On, Clock: 13:45, "
      "On Timer: Off, Off Timer: Off";

  ac.begin();
  irac.haier(&ac,
             true,                        // Power
             stdAc::opmode_t::kCool,      // Mode
             24,                          // Celsius
             stdAc::fanspeed_t::kMedium,  // Fan speed
             stdAc::swingv_t::kHigh,      // Vertical swing
             true,                        // Filter
             8 * 60 + 0,                  // Sleep time
             13 * 60 + 45);               // Clock
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(HAIER_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kHaierACBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Haier176) {
  IRHaierAC176 ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  const char expected[] =
      "Model: 1 (V9014557-A), Power: On, Button: 5 (Power), "
      "Mode: 1 (Cool), Temp: 23C, Fan: 2 (Medium), Turbo: On, Quiet: Off, "
      "Swing(V): 1 (Highest), Swing(H): 0 (Middle), Sleep: On, Health: On, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off, Lock: Off";
  ac.begin();
  irac.haier176(&ac,
                haier_ac176_remote_model_t::V9014557_A,  // Model
                true,                                    // Power
                stdAc::opmode_t::kCool,                  // Mode
                true,                                    // Celsius
                23,                                      // Degrees
                stdAc::fanspeed_t::kMedium,              // Fan speed
                stdAc::swingv_t::kHigh,                  // Vertical swing
                stdAc::swingh_t::kOff,                   // Horizontal swing
                true,                                    // Turbo
                false,                                   // Quiet
                true,                                    // Filter
                8 * 60 + 0);                             // Sleep time
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(HAIER_AC176, ac._irsend.capture.decode_type);
  ASSERT_EQ(kHaierAC176Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, HaierYrwo2) {
  IRHaierACYRW02 ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Model: 1 (V9014557-A), Power: On, Button: 5 (Power), "
      "Mode: 1 (Cool), Temp: 23C, Fan: 2 (Medium), Turbo: Off, Quiet: On, "
      "Swing(V): 1 (Highest), Swing(H): 7 (Auto), Sleep: On, Health: On, "
      "Timer Mode: 0 (N/A), On Timer: Off, Off Timer: Off, Lock: Off";

  ac.begin();
  irac.haierYrwo2(&ac,
                  true,                        // Power
                  stdAc::opmode_t::kCool,      // Mode
                  true,                        // Celsius
                  23,                          // Degrees
                  stdAc::fanspeed_t::kMedium,  // Fan speed
                  stdAc::swingv_t::kHigh,      // Vertical swing
                  stdAc::swingh_t::kAuto,      // Vertical swing
                  false,                       // Turbo
                  true,                        // Quiet
                  true,                        // Filter
                  8 * 60 + 0);                 // Sleep time
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(HAIER_AC_YRW02, ac._irsend.capture.decode_type);
  ASSERT_EQ(kHaierACYRW02Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Hitachi) {
  IRHitachiAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 2 (Auto), Temp: 22C, Fan: 3 (Medium), "
      "Swing(V): Off, Swing(H): On";

  ac.begin();
  irac.hitachi(&ac,
               true,                        // Power
               stdAc::opmode_t::kAuto,      // Mode
               22,                          // Celsius
               stdAc::fanspeed_t::kMedium,  // Fan speed
               stdAc::swingv_t::kOff,       // Vertical swing
               stdAc::swingh_t::kAuto);     // Horizontal swing

  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(HITACHI_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAcBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Hitachi1) {
  IRHitachiAc1 ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Model: 1 (R-LT0541-HTA-A), Power: On, Power Toggle: Off, "
      "Mode: 6 (Cool), Temp: 19C, Fan: 4 (Medium), "
      "Swing(V) Toggle: On, Swing(V): On, Swing(H): On, Sleep: 2, "
      "On Timer: Off, Off Timer: Off";

  ac.begin();
  irac.hitachi1(&ac,
                hitachi_ac1_remote_model_t::R_LT0541_HTA_A,  // Model
                true,                                        // Power
                false,                                       // Power Toggle
                stdAc::opmode_t::kCool,                      // Mode
                19,                                          // Celsius
                stdAc::fanspeed_t::kMedium,                  // Fan speed
                stdAc::swingv_t::kAuto,                      // Vertical swing
                stdAc::swingh_t::kLeft,                      // Horizontal swing
                true,                                        // Swing toggle
                5 * 60 + 37);                                // Sleep

  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(HITACHI_AC1, ac._irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAc1Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Hitachi264) {
  IRHitachiAc264 ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected_swingon[] =
      "Power: On, Mode: 6 (Heat), Temp: 25C, Fan: 4 (High), "
      "Button: 19 (Power/Mode)";

  ac.begin();
  irac.hitachi264(&ac,
                  true,                         // Power
                  stdAc::opmode_t::kHeat,       // Mode
                  25,                           // Celsius
                  stdAc::fanspeed_t::kMax);     // Fan speed

  ASSERT_EQ(expected_swingon, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(HITACHI_AC264, ac._irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAc264Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected_swingon, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
  EXPECT_EQ(decode_type_t::HITACHI_AC264, r.protocol);
  EXPECT_TRUE(r.power);
  EXPECT_EQ(stdAc::opmode_t::kHeat, r.mode);
  EXPECT_EQ(25, r.degrees);
}

TEST(TestIRac, Hitachi296) {
  IRHitachiAc296 ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 6 (Heat), Temp: 20C, Fan: 2 (Low)";

  ac.begin();
  irac.hitachi296(&ac,
                  true,                         // Power
                  stdAc::opmode_t::kHeat,       // Mode
                  20,                           // Celsius
                  stdAc::fanspeed_t::kLow);     // Fan speed

  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(HITACHI_AC296, ac._irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAc296Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
  EXPECT_EQ(decode_type_t::HITACHI_AC296, r.protocol);
  EXPECT_TRUE(r.power);
  EXPECT_EQ(stdAc::opmode_t::kHeat, r.mode);
  EXPECT_EQ(20, r.degrees);
}

TEST(TestIRac, Hitachi344) {
  IRHitachiAc344 ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected_swingon[] =
      "Power: On, Mode: 6 (Heat), Temp: 25C, Fan: 6 (Max), "
      "Button: 129 (Swing(V)), Swing(V): Off, Swing(H): 2 (Right)";

  ac.begin();
  irac.hitachi344(&ac,
                  true,                         // Power
                  stdAc::opmode_t::kHeat,       // Mode
                  25,                           // Celsius
                  stdAc::fanspeed_t::kMax,      // Fan speed
                  stdAc::swingv_t::kAuto,       // Swing(V)
                  stdAc::swingh_t::kRight);     // Swing(H)

  ASSERT_EQ(expected_swingon, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(HITACHI_AC344, ac._irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAc344Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected_swingon, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
  EXPECT_EQ(decode_type_t::HITACHI_AC344, r.protocol);
  EXPECT_TRUE(r.power);
  EXPECT_EQ(stdAc::opmode_t::kHeat, r.mode);
  EXPECT_EQ(25, r.degrees);

  char expected_swingoff[] =
      "Power: On, Mode: 6 (Heat), Temp: 25C, Fan: 6 (Max), "
      "Button: 19 (Power/Mode), Swing(V): Off, Swing(H): 2 (Right)";

  ac._irsend.reset();
  irac.hitachi344(&ac,
                  true,                         // Power
                  stdAc::opmode_t::kHeat,       // Mode
                  25,                           // Celsius
                  stdAc::fanspeed_t::kMax,      // Fan speed
                  stdAc::swingv_t::kOff,        // Swing(V)
                  stdAc::swingh_t::kRight);     // Swing(H)
  ASSERT_EQ(expected_swingoff, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(HITACHI_AC344, ac._irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAc344Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected_swingoff,
            IRAcUtils::resultAcToString(&ac._irsend.capture));
}

TEST(TestIRac, Hitachi424) {
  IRHitachiAc424 ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 6 (Heat), Temp: 25C, Fan: 6 (Max), "
      "Button: 19 (Power/Mode), Swing(V) Toggle: Off";
  char expected_swingv[] =
      "Power: On, Mode: 3 (Cool), Temp: 26C, Fan: 1 (Min), "
      "Button: 129 (Swing(V)), Swing(V) Toggle: On";

  ac.begin();
  irac.hitachi424(&ac,
                  true,                         // Power
                  stdAc::opmode_t::kHeat,       // Mode
                  25,                           // Celsius
                  stdAc::fanspeed_t::kMax,      // Fan speed
                  stdAc::swingv_t::kOff);       // Swing(V)

  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(HITACHI_AC424, ac._irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAc424Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));

  ac._irsend.reset();
  irac.hitachi424(&ac,
                  true,                         // Power
                  stdAc::opmode_t::kCool,       // Mode
                  26,                           // Celsius
                  stdAc::fanspeed_t::kMin,      // Fan speed
                  stdAc::swingv_t::kAuto);      // Swing(V)

  ASSERT_EQ(expected_swingv, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(HITACHI_AC424, ac._irsend.capture.decode_type);
  ASSERT_EQ(kHitachiAc424Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected_swingv, IRAcUtils::resultAcToString(&ac._irsend.capture));
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Kelvinator) {
  IRKelvinatorAC ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 1 (Cool), Temp: 19C, Fan: 3 (Medium), Turbo: Off, "
      "Quiet: Off, XFan: On, Ion: On, Light: On, "
      "Swing(H): Off, Swing(V): 0 (Off)";

  ac.begin();
  irac.kelvinator(&ac,
                  true,                        // Power
                  stdAc::opmode_t::kCool,      // Mode
                  19,                          // Celsius
                  stdAc::fanspeed_t::kMedium,  // Fan speed
                  stdAc::swingv_t::kOff,       // Vertical swing
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
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, LG) {
  IRLgAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Model: 1 (GE6711AR2853M), "
      "Power: On, Mode: 1 (Dry), Temp: 27C, Fan: 2 (Medium)";

  ac.begin();
  irac.lg(&ac,
          lg_ac_remote_model_t::GE6711AR2853M,  // Model
          true,                                 // Power
          stdAc::opmode_t::kDry,                // Mode
          27,                                   // Degrees C
          stdAc::fanspeed_t::kMedium,           // Fan speed
          stdAc::swingv_t::kLow,                // Vertical swing
          stdAc::swingv_t::kOff,                // Vertical swing (previous)
          stdAc::swingh_t::kOff,                // Horizontal swing
          true);                                // Light

  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(LG, ac._irsend.capture.decode_type);
  ASSERT_EQ(kLgBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  // There should only be a single message.
  ASSERT_EQ(61, ac._irsend.capture.rawlen);
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, LG2) {
  IRLgAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Model: 3 (AKB74955603), "
      "Power: On, Mode: 1 (Dry), Temp: 27C, Fan: 9 (Low)";

  ac.begin();
  irac.lg(&ac,
          lg_ac_remote_model_t::AKB74955603,    // Model
          true,                                 // Power
          stdAc::opmode_t::kDry,                // Mode
          27,                                   // Degrees C
          stdAc::fanspeed_t::kLow,              // Fan speed
          stdAc::swingv_t::kLow,                // Vertical swing
          stdAc::swingv_t::kOff,                // Vertical swing (previous)
          stdAc::swingh_t::kOff,                // Horizontal swing
          false);                               // Light

  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  ASSERT_EQ(181, ac._irsend.capture.rawlen);  // We expect three messages.
  // Message #1
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(LG2, ac._irsend.capture.decode_type);
  ASSERT_EQ(kLgBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
  // Message #2 - SwingV Low
  EXPECT_TRUE(capture.decodeLG(&ac._irsend.capture, 61));
  ASSERT_EQ(LG2, ac._irsend.capture.decode_type);
  ASSERT_EQ(kLgBits, ac._irsend.capture.bits);
  ASSERT_EQ(kLgAcSwingVLow, ac._irsend.capture.value);
  // Message #3 - Light Toggle
  EXPECT_TRUE(capture.decodeLG(&ac._irsend.capture, 121));
  ASSERT_EQ(LG2, ac._irsend.capture.decode_type);
  ASSERT_EQ(kLgBits, ac._irsend.capture.bits);
  ASSERT_EQ(kLgAcLightToggle, ac._irsend.capture.value);
}

// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/1513#issuecomment-877960010
//   https://docs.google.com/spreadsheets/d/1zF0FI2ENvbLdk4zaWBY9ZYVM3MB_4oxro9wCM7ETX4Y/edit#gid=348220307&range=A49:C49
TEST(TestIRac, Issue1513) {
  // Simulate sending a state with a SwingV auto, then followed by a SwingV Off.
  IRLgAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  ac.begin();
  // IRhvac {"Vendor":"LG2", "Model": 3, "Power": "On", "Mode": "Heat",
  //         "Temp": 26, "FanSpeed": "min", "SwingV": "Auto", "Light": "On"}
  ac._irsend.reset();
  irac.lg(&ac,
          lg_ac_remote_model_t::AKB74955603,    // Model
          true,                                 // Power
          stdAc::opmode_t::kHeat,               // Mode
          26,                                   // Degrees C
          stdAc::fanspeed_t::kMin,              // Fan speed
          stdAc::swingv_t::kAuto,               // Vertical swing
          stdAc::swingv_t::kHighest,            // Vertical swing (previous)
          stdAc::swingh_t::kOff,                // Horizontal swing
          true);                                // Light
  ac._irsend.makeDecodeResult();
  EXPECT_EQ(121, ac._irsend.capture.rawlen);
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(LG2, ac._irsend.capture.decode_type);  // Not "LG"
  ASSERT_EQ(kLgBits, ac._irsend.capture.bits);
  ASSERT_EQ(
      "Model: 2 (AKB75215403), Power: On, Mode: 4 (Heat), Temp: 26C, "
      "Fan: 0 (Quiet)",
      IRAcUtils::resultAcToString(&ac._irsend.capture));

  // The next should be a SwingV On.
  EXPECT_TRUE(capture.decodeLG(&ac._irsend.capture, 61));
  ASSERT_EQ(LG2, ac._irsend.capture.decode_type);  // Not "LG"
  ASSERT_EQ(kLgBits, ac._irsend.capture.bits);
  EXPECT_EQ(kLgAcSwingVSwing, ac._irsend.capture.value);
  ASSERT_EQ("Model: 3 (AKB74955603), Swing(V): 20 (Swing)",
            IRAcUtils::resultAcToString(&ac._irsend.capture));
  ac._irsend.reset();
  ac.stateReset();
  ac.send();
  // IRhvac {"Vendor":"LG2", "Model": 3, "Power": "On", "Mode": "Heat",
  //         "Temp": 26, "FanSpeed": "min", "SwingV": "Off", "Light": "On"}
  ac._irsend.makeDecodeResult();
  ac._irsend.reset();
  irac.lg(&ac,
          lg_ac_remote_model_t::AKB74955603,    // Model
          true,                                 // Power
          stdAc::opmode_t::kHeat,               // Mode
          26,                                   // Degrees C
          stdAc::fanspeed_t::kMin,              // Fan speed
          stdAc::swingv_t::kOff,                // Vertical swing
          stdAc::swingv_t::kAuto,               // Vertical swing (previous)
          stdAc::swingh_t::kOff,                // Horizontal swing
          true);                                // Light
  ac._irsend.makeDecodeResult();
  // There should only be two messages.
  EXPECT_EQ(121, ac._irsend.capture.rawlen);
  // First message should be normal.
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(LG2, ac._irsend.capture.decode_type);  // Not "LG"
  ASSERT_EQ(kLgBits, ac._irsend.capture.bits);
  ASSERT_EQ(
      "Model: 2 (AKB75215403), Power: On, Mode: 4 (Heat), Temp: 26C,"
      " Fan: 0 (Quiet)",
      IRAcUtils::resultAcToString(&ac._irsend.capture));
  // The next should be a SwingV Off.
  EXPECT_TRUE(capture.decodeLG(&ac._irsend.capture, 61));
  ASSERT_EQ(LG2, ac._irsend.capture.decode_type);  // Not "LG"
  ASSERT_EQ(kLgBits, ac._irsend.capture.bits);
  EXPECT_EQ(kLgAcSwingVOff, ac._irsend.capture.value);
  ASSERT_EQ("Model: 3 (AKB74955603), Swing(V): 21 (Off)",
            IRAcUtils::resultAcToString(&ac._irsend.capture));
}

// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/1651#issuecomment-952811720
TEST(TestIRac, Issue1651) {
  // Simulate sending a state with a SwingV off, then followed by a SwingV Auto.
  IRLgAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  ac.begin();
  // IRhvac {"Vendor":"LG2","Model":3,"Mode":"Auto","Power":"On","Celsius":"On",
  //         "Temp":15,"FanSpeed":"Auto","SwingV":"Off","SwingH":"Off",
  //         "Quiet":"Off","Turbo":"Off","Econo":"Off","Light":"On",
  //         "Filter":"Off","Clean":"Off","Beep":"Off","Sleep":-1}
  ac._irsend.reset();
  irac.lg(&ac,
          lg_ac_remote_model_t::AKB74955603,    // Model
          true,                                 // Power
          stdAc::opmode_t::kAuto,               // Mode
          15,                                   // Degrees C (Note: 16C is min)
          stdAc::fanspeed_t::kAuto,             // Fan speed
          stdAc::swingv_t::kOff,                // Vertical swing
          stdAc::swingv_t::kOff,                // Vertical swing (previous)
          stdAc::swingh_t::kOff,                // Horizontal swing
          true);                                // Light
  ac._irsend.makeDecodeResult();
  // As we are not making a change of the SwingV state, there should only be
  // one message. (i.e. 60 + 1)
  EXPECT_EQ(61, ac._irsend.capture.rawlen);
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(LG2, ac._irsend.capture.decode_type);  // Not "LG"
  ASSERT_EQ(kLgBits, ac._irsend.capture.bits);
  ASSERT_EQ(
      "Model: 2 (AKB75215403), Power: On, Mode: 3 (Auto), Temp: 16C, "
      "Fan: 5 (Auto)",
      IRAcUtils::resultAcToString(&ac._irsend.capture));
  ac._irsend.reset();
  ac.stateReset();
  ac.send();
  // IRhvac {"Vendor":"LG2","Model":3,"Mode":"Auto","Power":"On","Celsius":"On",
  //         "Temp":15,"FanSpeed":"Max","SwingV":"Auto","SwingH":"Off",
  //         "Quiet":"Off","Turbo":"Off","Econo":"Off","Light":"On",
  //         "Filter":"Off","Clean":"Off","Beep":"Off","Sleep":-1}
  ac._irsend.makeDecodeResult();
  ac._irsend.reset();
  irac.lg(&ac,
          lg_ac_remote_model_t::AKB74955603,    // Model
          true,                                 // Power
          stdAc::opmode_t::kAuto,               // Mode
          15,                                   // Degrees C (Note: 16C is min)
          stdAc::fanspeed_t::kMax,              // Fan speed
          stdAc::swingv_t::kAuto,               // Vertical swing
          stdAc::swingv_t::kOff,                // Vertical swing (previous)
          stdAc::swingh_t::kOff,                // Horizontal swing
          true);                                // Light
  ac._irsend.makeDecodeResult();
  // There should only be two messages.
  EXPECT_EQ(121, ac._irsend.capture.rawlen);
  // First message should be normal.
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(LG2, ac._irsend.capture.decode_type);  // Not "LG"
  ASSERT_EQ(kLgBits, ac._irsend.capture.bits);
  ASSERT_EQ(
      "Model: 2 (AKB75215403), Power: On, Mode: 3 (Auto), Temp: 16C, "
      "Fan: 4 (Maximum)",
      IRAcUtils::resultAcToString(&ac._irsend.capture));
  // The next should be a SwingV Off.
  EXPECT_TRUE(capture.decodeLG(&ac._irsend.capture, 61));
  ASSERT_EQ(LG2, ac._irsend.capture.decode_type);  // Not "LG"
  ASSERT_EQ(kLgBits, ac._irsend.capture.bits);
  EXPECT_EQ(kLgAcSwingVSwing, ac._irsend.capture.value);
  ASSERT_EQ("Model: 3 (AKB74955603), Swing(V): 20 (Swing)",
            IRAcUtils::resultAcToString(&ac._irsend.capture));
}

TEST(TestIRac, LG2_AKB73757604) {
  IRLgAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Model: 2 (AKB75215403), "
      "Power: On, Mode: 1 (Dry), Temp: 27C, Fan: 1 (Low)";

  ac.begin();
  irac.lg(&ac,
          lg_ac_remote_model_t::AKB73757604,    // Model
          true,                                 // Power
          stdAc::opmode_t::kDry,                // Mode
          27,                                   // Degrees C
          stdAc::fanspeed_t::kLow,              // Fan speed
          stdAc::swingv_t::kLow,                // Vertical swing
          stdAc::swingv_t::kOff,                // Vertical swing (previous)
          stdAc::swingh_t::kAuto,               // Horizontal swing
          true);                                // Light

  ac._irsend.makeDecodeResult();
  ASSERT_EQ(361, ac._irsend.capture.rawlen);  // We expect six messages.
  // Message #1
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(LG2, ac._irsend.capture.decode_type);
  ASSERT_EQ(kLgBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
  // Message #2 - Vane 0 SwingV Low
  EXPECT_TRUE(capture.decodeLG(&ac._irsend.capture, 61));
  ASSERT_EQ(LG2, ac._irsend.capture.decode_type);
  ASSERT_EQ(kLgBits, ac._irsend.capture.bits);
  ASSERT_EQ(0x881325B, ac._irsend.capture.value);
  // Message #3 - Vane 1 SwingV Low
  EXPECT_TRUE(capture.decodeLG(&ac._irsend.capture, 121));
  ASSERT_EQ(LG2, ac._irsend.capture.decode_type);
  ASSERT_EQ(kLgBits, ac._irsend.capture.bits);
  ASSERT_EQ(0x88132D3, ac._irsend.capture.value);
  // Message #4 - Vane 2 SwingV Low
  EXPECT_TRUE(capture.decodeLG(&ac._irsend.capture, 181));
  ASSERT_EQ(LG2, ac._irsend.capture.decode_type);
  ASSERT_EQ(kLgBits, ac._irsend.capture.bits);
  ASSERT_EQ(0x881335C, ac._irsend.capture.value);
  // Message #5 - Vane 3 SwingV Low
  EXPECT_TRUE(capture.decodeLG(&ac._irsend.capture, 241));
  ASSERT_EQ(LG2, ac._irsend.capture.decode_type);
  ASSERT_EQ(kLgBits, ac._irsend.capture.bits);
  ASSERT_EQ(0x88133D4, ac._irsend.capture.value);
  // Message #6 - Horizontal swing
  EXPECT_TRUE(capture.decodeLG(&ac._irsend.capture, 301));
  ASSERT_EQ(LG2, ac._irsend.capture.decode_type);
  ASSERT_EQ(kLgBits, ac._irsend.capture.bits);
  ASSERT_EQ(kLgAcSwingHAuto, ac._irsend.capture.value);
}

TEST(TestIRac, Midea) {
  IRMideaAC ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Type: 1 (Command), Power: On, Mode: 1 (Dry), Celsius: On, "
      "Temp: 27C/80F, On Timer: Off, Off Timer: Off, Fan: 2 (Medium), "
      "Sleep: On, Swing(V): -, Econo: -, "
      "Turbo: -, Quiet: Off, Light: -, Clean: -, 8C Heat: -";

  ac.begin();
  irac.midea(&ac,
             true,                        // Power
             stdAc::opmode_t::kDry,       // Mode
             true,                        // Celsius
             27,                          // Degrees
             stdAc::fanspeed_t::kMedium,  // Fan speed
             stdAc::swingv_t::kOff,       // Swing(V)
             false,                       // Silent/Quiet
             false,                       // Previous Silent/Quiet setting
             false,                       // Turbo
             false,                       // Econo
             false,                       // Light
             false,                       // Clean
             8 * 60 + 0);                 // Sleep time

  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(MIDEA, ac._irsend.capture.decode_type);
  ASSERT_EQ(kMideaBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Mirage) {
  IRMirageAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  stdAc::state_t state, r, p;
  const char expected_KKG9AC1[] =
      "Model: 1 (KKG9AC1), Power: On, Mode: 3 (Dry), Temp: 27C, "
      "Fan: 2 (Medium), Turbo: Off, Sleep: On, Light: Off, "
      "Swing(V): 9 (High), Clock: 17:31";

  ac.begin();

  state.model = mirage_ac_remote_model_t::KKG9AC1;
  state.power = true;
  state.mode = stdAc::opmode_t::kDry;
  state.celsius = true;
  state.degrees = 27;
  state.fanspeed = stdAc::fanspeed_t::kMedium;
  state.swingv = stdAc::swingv_t::kHigh;
  state.swingh = stdAc::swingh_t::kLeft;
  state.turbo = false;
  state.quiet = true;
  state.light = false;
  state.filter = true;
  state.clean = false;
  state.sleep = 8 * 60 + 0;
  state.clock = 17 * 60 + 31;
  state.beep = false;
  irac.mirage(&ac, state);

  ASSERT_EQ(expected_KKG9AC1, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(MIRAGE, ac._irsend.capture.decode_type);
  ASSERT_EQ(kMirageBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected_KKG9AC1, IRAcUtils::resultAcToString(&ac._irsend.capture));
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));

  const char expected_KKG29AC1[] =
      "Model: 2 (KKG29AC1), Power: On, Mode: 3 (Dry), Temp: 27C, "
      "Fan: 3 (Medium), Turbo: Off, Sleep: On, Quiet: On, Light: -, "
      "Swing(V): On, Swing(H): On, Filter: On, Clean: -, "
      "On Timer: Off, Off Timer: Off, IFeel: Off";
  ac._irsend.reset();
  state.model = mirage_ac_remote_model_t::KKG29AC1;
  irac.mirage(&ac, state);
  ASSERT_EQ(expected_KKG29AC1, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(MIRAGE, ac._irsend.capture.decode_type);
  ASSERT_EQ(kMirageBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected_KKG29AC1,
            IRAcUtils::resultAcToString(&ac._irsend.capture));
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Mitsubishi) {
  IRMitsubishiAC ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 3 (Cool), Temp: 20C, Fan: 2 (Medium), "
      "Swing(V): 0 (Auto), Swing(H): 3 (Middle), "
      "Clock: 14:30, On Timer: 00:00, Off Timer: 00:00, Timer: -, "
      "Weekly Timer: Off"
      ", 10C Heat: Off, ISee: Off, Econo: Off, Absense detect: Off, "
      "Direct / Indirect Mode: 0, Fresh: Off";

  ac.begin();
  irac.mitsubishi(&ac,
                  true,                        // Power
                  stdAc::opmode_t::kCool,      // Mode
                  20,                          // Celsius
                  stdAc::fanspeed_t::kMedium,  // Fan speed
                  stdAc::swingv_t::kOff,       // Vertical swing
                  stdAc::swingh_t::kOff,       // Horizontal swing
                  false,                       // Silent
                  14 * 60 + 35);               // Clock
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(MITSUBISHI_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kMitsubishiACBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Mitsubishi136) {
  IRMitsubishi136 ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 5 (Dry), Temp: 22C, Fan: 3 (High), "
      "Swing(V): 3 (Highest), Quiet: Off";

  ac.begin();
  irac.mitsubishi136(&ac,
                     true,                        // Power
                     stdAc::opmode_t::kDry,       // Mode
                     22,                          // Celsius
                     stdAc::fanspeed_t::kMax,     // Fan speed
                     stdAc::swingv_t::kHighest,   // Vertical swing
                     false);                      // Quiet
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(MITSUBISHI136, ac._irsend.capture.decode_type);
  ASSERT_EQ(kMitsubishi136Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, MitsubishiHeavy88) {
  IRMitsubishiHeavy88Ac ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 1 (Cool), Temp: 21C, Fan: 3 (Med), "
      "Swing(V): 4 (Auto), Swing(H): 0 (Off), Turbo: Off, Econo: Off, "
      "3D: Off, Clean: On";

  ac.begin();
  irac.mitsubishiHeavy88(&ac,
                         true,                        // Power
                         stdAc::opmode_t::kCool,      // Mode
                         21,                          // Celsius
                         stdAc::fanspeed_t::kMedium,  // Fan speed
                         stdAc::swingv_t::kAuto,      // Vertical swing
                         stdAc::swingh_t::kOff,       // Horizontal swing
                         false,                       // Turbo
                         false,                       // Econo
                         true);                       // Clean
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(MITSUBISHI_HEAVY_88, ac._irsend.capture.decode_type);
  ASSERT_EQ(kMitsubishiHeavy88Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, MitsubishiHeavy152) {
  IRMitsubishiHeavy152Ac ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 1 (Cool), Temp: 20C, Fan: 6 (Econo), "
      "Swing(V): 6 (Off), Swing(H): 0 (Auto), Silent: On, Turbo: Off, "
      "Econo: On, Night: On, Filter: On, 3D: Off, Clean: Off";

  ac.begin();
  irac.mitsubishiHeavy152(&ac,
                          true,                        // Power
                          stdAc::opmode_t::kCool,      // Mode
                          20,                          // Celsius
                          stdAc::fanspeed_t::kLow,     // Fan speed
                          stdAc::swingv_t::kOff,       // Vertical swing
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
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Neoclima) {
  IRNeoclimaAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 1 (Cool), Temp: 20C, Fan: 3 (Low), "
      "Swing(V): Off, Swing(H): On, Sleep: On, Turbo: Off, Econo: On, "
      "Hold: Off, Ion: On, Eye: Off, Light: On, Follow: Off, 8C Heat: Off, "
      "Fresh: Off, Button: 0 (Power)";

  ac.begin();
  irac.neoclima(&ac,
                true,                        // Power
                stdAc::opmode_t::kCool,      // Mode
                true,                        // Celsius
                20,                          // Degrees
                stdAc::fanspeed_t::kLow,     // Fan speed
                stdAc::swingv_t::kOff,       // Vertical swing
                stdAc::swingh_t::kAuto,      // Horizontal swing
                false,                       // Turbo
                true,                        // Econo
                true,                        // Light
                true,                        // Filter
                8 * 60);                     // Sleep
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(decode_type_t::NEOCLIMA, ac._irsend.capture.decode_type);
  ASSERT_EQ(kNeoclimaBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Panasonic) {
  IRPanasonicAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected_nke[] =
      "Model: 2 (NKE), Power: On, Mode: 4 (Heat), Temp: 28C, Fan: 2 (Medium), "
      "Swing(V): 15 (Auto), Swing(H): 6 (Middle), Quiet: On, "
      "Powerful: Off, Clock: 19:17, On Timer: Off, Off Timer: Off";

  ac.begin();
  irac.panasonic(&ac,
                 kPanasonicNke,               // Model
                 true,                        // Power
                 stdAc::opmode_t::kHeat,      // Mode
                 28,                          // Celsius
                 stdAc::fanspeed_t::kMedium,  // Fan speed
                 stdAc::swingv_t::kAuto,      // Vertical swing
                 stdAc::swingh_t::kLeft,      // Horizontal swing
                 true,                        // Quiet
                 false,                       // Turbo
                 false,                       // Filter
                 19 * 60 + 17);               // Clock
  ASSERT_EQ(expected_nke, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(PANASONIC_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kPanasonicAcBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected_nke, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));

  char expected_dke[] =
      "Model: 3 (DKE), Power: On, Mode: 3 (Cool), Temp: 18C, Fan: 4 (Maximum), "
      "Swing(V): 2 (High), Swing(H): 6 (Middle), "
      "Quiet: Off, Powerful: On, Ion: On, "
      "Clock: 19:17, On Timer: Off, Off Timer: Off";
  ac._irsend.reset();
  irac.panasonic(&ac,
               kPanasonicDke,               // Model
               true,                        // Power
               stdAc::opmode_t::kCool,      // Mode
               18,                          // Celsius
               stdAc::fanspeed_t::kMax,     // Fan speed
               stdAc::swingv_t::kHigh,      // Vertical swing
               stdAc::swingh_t::kMiddle,    // Horizontal swing
               false,                       // Quiet
               true,                        // Turbo
               true,                        // Filter
               19 * 60 + 17);               // Clock
  ASSERT_EQ(expected_dke, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(PANASONIC_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kPanasonicAcBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected_dke, IRAcUtils::resultAcToString(&ac._irsend.capture));
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Panasonic32) {
  IRPanasonicAc32 ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power Toggle: On, Mode: 4 (Heat), Temp: 28C, Fan: 4 (Medium), "
      "Swing(H): On, Swing(V): 7 (Auto)";

  ac.begin();
  irac.panasonic32(&ac,
                 true,                        // Power
                 stdAc::opmode_t::kHeat,      // Mode
                 28,                          // Celsius
                 stdAc::fanspeed_t::kMedium,  // Fan speed
                 stdAc::swingv_t::kAuto,      // Vertical swing
                 stdAc::swingh_t::kLeft);     // Horizontal swing
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(PANASONIC_AC32, ac._irsend.capture.decode_type);
  ASSERT_EQ(kPanasonicAc32Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Samsung) {
  IRSamsungAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  const char expected[] =
      "Power: On, Mode: 0 (Auto), Temp: 28C, Fan: 6 (Auto), "
      "Swing(V): On, Swing(H): On, Beep: Toggle, "
      "Clean: Toggle, Quiet: On, Powerful: Off, ""Econo: Off, Breeze: Off, "
      "Light: On, Ion: Off";

  ac.begin();
  irac.samsung(&ac,
               true,                        // Power
               stdAc::opmode_t::kAuto,      // Mode
               28,                          // Celsius
               stdAc::fanspeed_t::kMedium,  // Fan speed
               stdAc::swingv_t::kAuto,      // Vertical swing
               stdAc::swingh_t::kAuto,      // Horizontal swing
               true,                        // Quiet
               false,                       // Turbo
               false,                       // Econo
               true,                        // Light (Display)
               false,                       // Filter (Ion)
               true,                        // Clean (Toggle)
               true,                        // Beep
               -1,                          // Sleep
               true,                        // Previous power state
               -1,                          // Previous Sleep
               false);                      // Force Extended
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(SAMSUNG_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kSamsungAcBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));

  ac._irsend.reset();
  irac.samsung(&ac,
               true,                        // Power
               stdAc::opmode_t::kAuto,      // Mode
               28,                          // Celsius
               stdAc::fanspeed_t::kMedium,  // Fan speed
               stdAc::swingv_t::kAuto,      // Vertical swing
               stdAc::swingh_t::kAuto,      // Horizontal swing
               true,                        // Quiet
               false,                       // Turbo
               false,                       // Econo
               true,                        // Light (Display)
               false,                       // Filter (Ion)
               true,                        // Clean (Toggle)
               true,                        // Beep
               -1,                          // Sleep
               true,                        // Previous power state
               -1,                          // Previous Sleep
               true);                       // Force Extended
  ASSERT_EQ(expected, ac.toString());  // Class should be in the desired mode.
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(SAMSUNG_AC, ac._irsend.capture.decode_type);
  // We expect an extended state because of `Force Extended`.
  ASSERT_EQ(kSamsungAcExtendedBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));

  ac._irsend.reset();
  const char sleep[] =
      "Power: On, Mode: 0 (Auto), Temp: 28C, Fan: 6 (Auto), "
      "Swing(V): On, Swing(H): Off, Beep: -, Clean: Toggle, "
      "Quiet: On, Powerful: Off, Econo: Off, Breeze: Off, "
      "Light: On, Ion: Off, Sleep Timer: 08:00";
  irac.samsung(&ac,
               true,                        // Power
               stdAc::opmode_t::kAuto,      // Mode
               28,                          // Celsius
               stdAc::fanspeed_t::kMedium,  // Fan speed
               stdAc::swingv_t::kAuto,      // Vertical swing
               stdAc::swingh_t::kOff,       // Horizontal swing
               true,                        // Quiet
               false,                       // Turbo
               false,                       // Econo
               true,                        // Light (Display)
               false,                       // Filter (Ion)
               true,                        // Clean (Toggle)
               false,                       // Beep
               8 * 60,                      // Sleep
               true,                        // Previous power state
               -1,                          // Previous Sleep
               false);                      // Force Extended
  ASSERT_EQ(sleep, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(SAMSUNG_AC, ac._irsend.capture.decode_type);
  // We expect an extended state because of the change in `sleep`.
  ASSERT_EQ(kSamsungAcExtendedBits, ac._irsend.capture.bits);
  ASSERT_EQ(sleep, IRAcUtils::resultAcToString(&ac._irsend.capture));
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Sanyo) {
  IRSanyoAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 2 (Cool), Temp: 28C, Fan: 3 (Medium), "
      "Swing(V): 7 (Highest), Sleep: On, Beep: On, "
      "Sensor: Room, Sensor Temp: 28C, Off Timer: Off";

  ac.begin();
  irac.sanyo(&ac,
             true,                         // Power
             stdAc::opmode_t::kCool,       // Mode
             28,                           // Celsius
             stdAc::fanspeed_t::kMedium,   // Fan speed
             stdAc::swingv_t::kHighest,    // Vertical Swing
             true,                         // Beep
             17);                          // Sleep
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(SANYO_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kSanyoAcBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Sanyo88) {
  IRSanyoAc88 ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  const char expected[] =
      "Power: On, Mode: 2 (Cool), Temp: 28C, Fan: 2 (Medium), Swing(V): On, "
      "Turbo: On, Sleep: On, Clock: 11:40";

  ac.begin();
  irac.sanyo88(&ac,
               true,                         // Power
               stdAc::opmode_t::kCool,       // Mode
               28,                           // Celsius
               stdAc::fanspeed_t::kMedium,   // Fan speed
               stdAc::swingv_t::kAuto,       // Vertical Swing
               true,                         // Turbo
               true,                         // Filter
               17,                           // Sleep
               11 * 60 + 40);                // Clock
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(SANYO_AC88, ac._irsend.capture.decode_type);
  ASSERT_EQ(kSanyoAc88Bits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Sharp) {
  IRSharpAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Model: 1 (A907), Power: On, Mode: 2 (Cool), Temp: 28C, Fan: 3 (Medium), "
      "Swing(V): 7 (Swing), Turbo: Off, Ion: On, Econo: -, Clean: Off";

  ac.begin();
  irac.sharp(&ac,
             sharp_ac_remote_model_t::A907,  // Model
             true,                           // Power
             true,                           // Previous Power
             stdAc::opmode_t::kCool,         // Mode
             28,                             // Celsius
             stdAc::fanspeed_t::kMedium,     // Fan speed
             stdAc::swingv_t::kAuto,         // Vertical swing
             stdAc::swingv_t::kOff,          // Previous Vertical swing
             false,                          // Turbo
             false,                          // Light
             true,                           // Filter (Ion)
             false);                         // Clean
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(SHARP_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kSharpAcBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Tcl112) {
  IRTcl112Ac ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Model: 1 (TAC09CHSD), Type: 1, Power: On, Mode: 3 (Cool), Temp: 20C, "
      "Fan: 3 (Medium), Swing(V): 0 (Auto), Swing(H): On, "
      "Econo: On, Health: On, Turbo: Off, Light: On, "
      "On Timer: Off, Off Timer: Off";

  ac.begin();
  irac.tcl112(&ac,
              tcl_ac_remote_model_t::TAC09CHSD,  // Model
              true,                              // Power
              stdAc::opmode_t::kCool,            // Mode
              20,                                // Celsius
              stdAc::fanspeed_t::kMedium,        // Fan speed
              stdAc::swingv_t::kOff,             // Vertical swing
              stdAc::swingh_t::kAuto,            // Horizontal swing
              false,                             // Quiet (aka. Mute)
              false,                             // Turbo
              true,                              // Light
              true,                              // Econo
              true);                             // Filter (aka. Health)
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(TCL112AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kTcl112AcBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
  // Test the quiet mode, which should generate two messages.
  ac._irsend.reset();
  irac.tcl112(&ac,
              tcl_ac_remote_model_t::TAC09CHSD,  // Model
              true,                              // Power
              stdAc::opmode_t::kCool,            // Mode
              20,                                // Celsius
              stdAc::fanspeed_t::kMedium,        // Fan speed
              stdAc::swingv_t::kOff,             // Vertical swing
              stdAc::swingh_t::kAuto,            // Horizontal swing
              true,                              // Quiet (aka. Mute)
              false,                             // Turbo
              true,                              // Light
              true,                              // Econo
              true);                             // Filter (aka. Health)
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(TCL112AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kTcl112AcBits, ac._irsend.capture.bits);
  ASSERT_EQ(
      "Model: 1 (TAC09CHSD), Type: 2, Quiet: On",
      IRAcUtils::resultAcToString(&ac._irsend.capture));
  // TCL112 uses the Mitsubishi112 decoder.
  // Skip first message.
  EXPECT_TRUE(capture.decodeMitsubishi112(&ac._irsend.capture, 229));
  ASSERT_EQ(TCL112AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kTcl112AcBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
}

TEST(TestIRac, Technibel) {
  IRTechnibelAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 8 (Heat), Fan: 2 (Medium), Temp: 72F, Sleep: On, "
      "Swing(V): On, Timer: Off";

  ac.begin();
  irac.technibel(&ac,
                 true,                        // Power
                 stdAc::opmode_t::kHeat,      // Mode
                 false,                       // Celsius
                 72,                          // Degrees
                 stdAc::fanspeed_t::kMedium,  // Fan speed
                 stdAc::swingv_t::kAuto,      // Vertical swing
                 8 * 60 + 30);                // Sleep
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(decode_type_t::TECHNIBEL_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kTechnibelAcBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Teco) {
  IRTecoAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 0 (Auto), Temp: 21C, Fan: 2 (Medium), Sleep: On, "
      "Swing: On, Light: On, Humid: Off, Save: Off, Timer: Off";

  ac.begin();
  irac.teco(&ac,
            true,                        // Power
            stdAc::opmode_t::kAuto,      // Mode
            21,                          // Celsius
            stdAc::fanspeed_t::kMedium,  // Fan speed
            stdAc::swingv_t::kAuto,      // Vertical swing
            true,                        // Light
            8 * 60 + 30);                // Sleep
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(TECO, ac._irsend.capture.decode_type);
  ASSERT_EQ(kTecoBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Toshiba) {
  IRToshibaAC ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Temp: 29C, Power: On, Mode: 2 (Dry), Fan: 2 (UNKNOWN), "
      "Turbo: Off, Econo: On, Filter: Off";

  ac.begin();
  irac.toshiba(&ac,
               true,                      // Power
               stdAc::opmode_t::kDry,     // Mode
               29,                        // Celsius
               stdAc::fanspeed_t::kLow,   // Fan speed
               stdAc::swingv_t::kOff,     // Vertical Swing
               false,                     // Turbo
               true,                      // Econo
               false);                    // Filter
  ASSERT_EQ(expected, ac.toString());
  ASSERT_EQ(kToshibaACStateLengthLong, ac.getStateLength());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(TOSHIBA_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kToshibaACBitsLong, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
  EXPECT_EQ(
      "f38000d50"
      "m4400s4300"
      "m580s1600m580s1600m580s1600m580s1600m580s490m580s490m580s1600m580s490"
      "m580s490m580s490m580s490m580s490m580s1600m580s1600m580s490m580s1600"
      "m580s490m580s490m580s490m580s490m580s490m580s1600m580s490m580s490"
      "m580s1600m580s1600m580s1600m580s1600m580s1600m580s490m580s1600m580s1600"
      "m580s490m580s490m580s490m580s490m580s1600m580s490m580s490m580s1600"
      "m580s1600m580s1600m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s1600m580s1600m580s490m580s490m580s490m580s1600m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s1600m580s1600"
      "m580s1600m580s490m580s1600m580s490m580s1600m580s490m580s490m580s490"
      "m580s7400"
      "m4400s4300"
      "m580s1600m580s1600m580s1600m580s1600m580s490m580s490m580s1600m580s490"
      "m580s490m580s490m580s490m580s490m580s1600m580s1600m580s490m580s1600"
      "m580s490m580s490m580s490m580s490m580s490m580s1600m580s490m580s490"
      "m580s1600m580s1600m580s1600m580s1600m580s1600m580s490m580s1600m580s1600"
      "m580s490m580s490m580s490m580s490m580s1600m580s490m580s490m580s1600"
      "m580s1600m580s1600m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s1600m580s1600m580s490m580s490m580s490m580s1600m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s490"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s1600m580s1600"
      "m580s1600m580s490m580s1600m580s490m580s1600m580s490m580s490m580s490"
      "m580s7400"
      "m4400s4300"
      "m580s1600m580s1600m580s1600m580s1600m580s490m580s490m580s1600m580s490"
      "m580s490m580s490m580s490m580s490m580s1600m580s1600m580s490m580s1600"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s1600"
      "m580s1600m580s1600m580s1600m580s1600m580s1600m580s1600m580s1600m580s490"
      "m580s490m580s490m580s1600m580s490m580s490m580s490m580s490m580s1600"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s1600m580s490"
      "m580s490m580s490m580s1600m580s490m580s490m580s490m580s1600m580s1600"
      "m580s7400"
      "m4400s4300"
      "m580s1600m580s1600m580s1600m580s1600m580s490m580s490m580s1600m580s490"
      "m580s490m580s490m580s490m580s490m580s1600m580s1600m580s490m580s1600"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s490m580s1600"
      "m580s1600m580s1600m580s1600m580s1600m580s1600m580s1600m580s1600m580s490"
      "m580s490m580s490m580s1600m580s490m580s490m580s490m580s490m580s1600"
      "m580s490m580s490m580s490m580s490m580s490m580s490m580s1600m580s490"
      "m580s490m580s490m580s1600m580s490m580s490m580s490m580s1600m580s1600"
      "m580s7400",
      ac._irsend.outputStr());
}

TEST(TestIRac, Transcold) {
  IRTranscoldAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 6 (Cool), Fan: 11 (Max), Temp: 19C";

  ac.begin();
  irac.transcold(&ac,
              true,                        // Power
              stdAc::opmode_t::kCool,      // Mode
              19,                          // Celsius
              stdAc::fanspeed_t::kMax,     // Fan speed
              stdAc::swingv_t::kOff,       // Vertical swing
              stdAc::swingh_t::kOff);      // Horizontal swing
  EXPECT_TRUE(ac.getPower());
  EXPECT_EQ(kTranscoldCool, ac.getMode());
  EXPECT_EQ(19, ac.getTemp());
  EXPECT_EQ(kTranscoldFanMax, ac.getFan());
  EXPECT_FALSE(ac.getSwing());
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(decode_type_t::TRANSCOLD, ac._irsend.capture.decode_type);
  ASSERT_EQ(kTranscoldBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Trotec) {
  IRTrotecESP ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 1 (Cool), Temp: 18C, Fan: 3 (High), Sleep: On";

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
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(TROTEC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kTrotecBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Trotec3550) {
  IRTrotec3550 ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 1 (Cool), Temp: 18C, Fan: 3 (High), Swing(V): On, "
      "Timer: Off";
  ac.begin();
  irac.trotec3550(&ac,
                  true,                        // Power
                  stdAc::opmode_t::kCool,      // Mode
                  true,                        // Celsius
                  18,                          // Degrees
                  stdAc::fanspeed_t::kHigh,    // Fan speed
                  stdAc::swingv_t::kAuto);     // Vertical Swing
  EXPECT_TRUE(ac.getPower());
  EXPECT_EQ(kTrotecCool, ac.getMode());
  EXPECT_EQ(18, ac.getTemp());
  EXPECT_EQ(kTrotecFanHigh, ac.getFan());
  EXPECT_TRUE(ac.getSwingV());
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(TROTEC_3550, ac._irsend.capture.decode_type);
  ASSERT_EQ(kTrotecBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Truma) {
  IRTrumaAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 2 (Cool), Temp: 22C, Fan: 3 (Quiet), Quiet: On";

  ac.begin();
  irac.truma(&ac,
             true,                        // Power
             stdAc::opmode_t::kCool,      // Mode
             22,                          // Celsius
             stdAc::fanspeed_t::kHigh,    // Fan speed
             true);                       // Quiet (will override fan speed)
  EXPECT_TRUE(ac.getPower());
  EXPECT_EQ(kTrumaCool, ac.getMode());
  EXPECT_EQ(22, ac.getTemp());
  EXPECT_EQ(kTrumaFanQuiet, ac.getFan());
  EXPECT_TRUE(ac.getQuiet());
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(TRUMA, ac._irsend.capture.decode_type);
  ASSERT_EQ(kTrumaBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, Vestel) {
  IRVestelAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Power: On, Mode: 0 (Auto), Temp: 22C, Fan: 5 (Low), Sleep: On, "
      "Turbo: Off, Ion: On, Swing: On";

  ac.begin();
  irac.vestel(&ac,
              true,                      // Power
              stdAc::opmode_t::kAuto,    // Mode
              22,                        // Celsius
              stdAc::fanspeed_t::kLow,   // Fan speed
              stdAc::swingv_t::kHigh,    // Vertical swing
              false,                     // Turbo
              true,                      // Filter
              8 * 60 + 0);               // Sleep time
              // 13 * 60 + 45);             // Clock
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(VESTEL_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kVestelAcBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));

  ac._irsend.reset();
  char expected_clocks[] =
      "Clock: 13:45, Timer: Off, On Timer: Off, Off Timer: Off";

  ac.begin();
  irac.vestel(&ac,
              true,                      // Power
              stdAc::opmode_t::kAuto,    // Mode
              22,                        // Celsius
              stdAc::fanspeed_t::kLow,   // Fan speed
              stdAc::swingv_t::kHigh,    // Vertical swing
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
  ASSERT_EQ(expected_clocks, IRAcUtils::resultAcToString(&ac._irsend.capture));
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));

  // Now check it sends both messages during normal operation when the
  // clock is set.
  ac._irsend.reset();
  ac.begin();
  irac.vestel(&ac,
              true,                      // Power
              stdAc::opmode_t::kAuto,    // Mode
              22,                        // Celsius
              stdAc::fanspeed_t::kLow,   // Fan speed
              stdAc::swingv_t::kHigh,    // Vertical swing
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

TEST(TestIRac, Voltas) {
  IRVoltas ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  ac.begin();

  // Test the UNKNOWN model type
  char expected_unknown[] =
      "Model: 0 (UNKNOWN), Power: On, Mode: 8 (Cool), Temp: 18C, "
      "Fan: 1 (High), Swing(V): On, Swing(H): On, "
      "Turbo: Off, Econo: Off, WiFi: Off, Light: On, Sleep: On, "
      "On Timer: Off, Off Timer: Off";
  irac.voltas(&ac,
              voltas_ac_remote_model_t::kVoltasUnknown,  // Model
              true,                                      // Power
              stdAc::opmode_t::kCool,                    // Mode
              18,                                        // Celsius
              stdAc::fanspeed_t::kHigh,                  // Fan speed
              stdAc::swingv_t::kAuto,                    // Vertical Swing
              stdAc::swingh_t::kAuto,                    // Horizontal Swing
              false,                                     // Turbo
              false,                                     // Econo
              true,                                      // Light
              3 * 60);                                   // Sleep
  EXPECT_EQ(voltas_ac_remote_model_t::kVoltasUnknown, ac.getModel());
  EXPECT_TRUE(ac.getPower());
  EXPECT_EQ(kVoltasCool, ac.getMode());
  EXPECT_EQ(18, ac.getTemp());
  EXPECT_EQ(kVoltasFanHigh, ac.getFan());
  EXPECT_FALSE(ac.getTurbo());
  EXPECT_FALSE(ac.getEcono());
  EXPECT_TRUE(ac.getLight());
  EXPECT_TRUE(ac.getSleep());
  ASSERT_EQ(expected_unknown, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(VOLTAS, ac._irsend.capture.decode_type);
  ASSERT_EQ(kVoltasBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected_unknown, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));

  ac._irsend.reset();
  // Test the UNKNOWN model type
  char expected_122LZF[] =
      "Model: 1 (122LZF), Power: On, Mode: 8 (Cool), Temp: 18C, "
      "Fan: 1 (High), Swing(V): On, Swing(H): N/A, "
      "Turbo: Off, Econo: Off, WiFi: Off, Light: On, Sleep: On, "
      "On Timer: Off, Off Timer: Off";
  irac.voltas(&ac,
              voltas_ac_remote_model_t::kVoltas122LZF,   // Model
              true,                                      // Power
              stdAc::opmode_t::kCool,                    // Mode
              18,                                        // Celsius
              stdAc::fanspeed_t::kHigh,                  // Fan speed
              stdAc::swingv_t::kAuto,                    // Vertical Swing
              stdAc::swingh_t::kAuto,                    // Horizontal Swing
              false,                                     // Turbo
              false,                                     // Econo
              true,                                      // Light
              3 * 60);                                   // Sleep
  EXPECT_EQ(voltas_ac_remote_model_t::kVoltas122LZF, ac.getModel());
  EXPECT_TRUE(ac.getPower());
  EXPECT_EQ(kVoltasCool, ac.getMode());
  EXPECT_EQ(18, ac.getTemp());
  EXPECT_EQ(kVoltasFanHigh, ac.getFan());
  EXPECT_FALSE(ac.getTurbo());
  EXPECT_FALSE(ac.getEcono());
  EXPECT_TRUE(ac.getLight());
  EXPECT_TRUE(ac.getSleep());
  ASSERT_EQ(expected_122LZF, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(VOLTAS, ac._irsend.capture.decode_type);
  ASSERT_EQ(kVoltasBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected_122LZF, IRAcUtils::resultAcToString(&ac._irsend.capture));
}

TEST(TestIRac, Whirlpool) {
  IRWhirlpoolAc ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  char expected[] =
      "Model: 1 (DG11J13A), Power Toggle: On, Mode: 1 (Auto), Temp: 21C, "
      "Fan: 3 (Low), Swing: On, Light: On, Clock: 23:58, On Timer: Off, "
      "Off Timer: Off, Sleep: On, Super: Off, Command: 1 (Power)";

  ac.begin();
  irac.whirlpool(&ac,
                 DG11J13A,
                 true,                        // Power
                 stdAc::opmode_t::kAuto,      // Mode
                 21,                          // Celsius
                 stdAc::fanspeed_t::kMedium,  // Fan speed
                 stdAc::swingv_t::kAuto,      // Vertical swing
                 false,                       // Turbo
                 true,                        // Light
                 8 * 60 + 30,                 // Sleep
                 23 * 60 + 58);               // Clock
  ASSERT_EQ(expected, ac.toString());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(WHIRLPOOL_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kWhirlpoolAcBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected, IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

TEST(TestIRac, cmpStates) {
  stdAc::state_t a, b;
  a.protocol = decode_type_t::COOLIX;
  a.model = -1;
  a.power = true;
  a.celsius = true;
  a.degrees = 25;
  a.mode = stdAc::opmode_t::kAuto;
  a.fanspeed = stdAc::fanspeed_t::kAuto;
  a.swingh = stdAc::swingh_t::kOff;
  a.swingv = stdAc::swingv_t::kOff;
  a.quiet = false;
  a.turbo = false;
  a.light = false;
  a.econo = false;
  a.beep = false;
  a.filter = false;
  a.clean = false;
  a.quiet = false;
  a.sleep = -1;
  a.clock = -1;

  ASSERT_FALSE(IRac::cmpStates(a, a));
  ASSERT_TRUE(IRac::cmpStates(a, b));

  b = a;
  ASSERT_FALSE(IRac::cmpStates(a, b));

  // Check we don't compare the clock.
  b.clock = 1234;
  ASSERT_FALSE(IRac::cmpStates(a, b));

  // Now make them different.
  b.power = false;
  ASSERT_TRUE(IRac::cmpStates(a, b));
}

TEST(TestIRac, handleToggles) {
  stdAc::state_t desired, prev, result;
  desired.protocol = decode_type_t::COOLIX;
  desired.model = -1;
  desired.power = true;
  desired.celsius = true;
  desired.degrees = 25;
  desired.mode = stdAc::opmode_t::kAuto;
  desired.fanspeed = stdAc::fanspeed_t::kAuto;
  desired.swingh = stdAc::swingh_t::kOff;
  desired.swingv = stdAc::swingv_t::kOff;
  desired.quiet = false;
  desired.turbo = false;
  desired.light = false;
  desired.econo = false;
  desired.beep = false;
  desired.filter = false;
  desired.clean = false;
  desired.quiet = false;
  desired.sleep = -1;
  desired.clock = -1;

  // The states should be the same as we gave no previous state.
  EXPECT_FALSE(IRac::cmpStates(desired, IRac::handleToggles(desired)));
  // The states should be the same as we gave no settings that changed.
  prev = desired;
  EXPECT_FALSE(IRac::cmpStates(desired, IRac::handleToggles(desired, &prev)));
  // Change something that isn't a toggle.
  desired.degrees = 26;
  ASSERT_TRUE(IRac::cmpStates(desired, prev));
  // Still shouldn't change.
  EXPECT_FALSE(IRac::cmpStates(desired, IRac::handleToggles(desired, &prev)));
  prev.turbo = true;  // This requires a toggle.
  result = IRac::handleToggles(desired, &prev);
  EXPECT_TRUE(IRac::cmpStates(desired, result));
  EXPECT_TRUE(result.turbo);
  desired.turbo = true;  // As the desired setting hasn't changed from previous
                         // the result should not have turbo set, as it is
                         // a toggle setting.
  result = IRac::handleToggles(desired, &prev);
  EXPECT_TRUE(IRac::cmpStates(desired, result));
  EXPECT_FALSE(result.turbo);

  // Go back to the same states.
  prev = desired;
  ASSERT_FALSE(IRac::cmpStates(desired, prev));
  // Test swing, as it is more complicated.
  result = IRac::handleToggles(desired, &prev);
  EXPECT_EQ(stdAc::swingv_t::kOff, result.swingv);
  desired.swingv = stdAc::swingv_t::kAuto;
  result = IRac::handleToggles(desired, &prev);
  EXPECT_NE(stdAc::swingv_t::kOff, result.swingv);

  prev = desired;  // Pretend it was sent and time has passed.
  ASSERT_FALSE(IRac::cmpStates(desired, prev));
  ASSERT_NE(stdAc::swingv_t::kOff, desired.swingv);

  // User changes setting but it's still an "on" setting, as this device
  // only has a binary on/off for swingv. Nothing should change.
  desired.swingv = stdAc::swingv_t::kHigh;
  result = IRac::handleToggles(desired, &prev);
  ASSERT_EQ(stdAc::swingv_t::kOff, result.swingv);  // i.e No toggle.

  prev = desired;  // Pretend it was sent and time has passed.
  // User changes setting to off. i.e. It is no longer on, so it should toggle.
  desired.swingv = stdAc::swingv_t::kOff;
  result = IRac::handleToggles(desired, &prev);
  ASSERT_NE(stdAc::swingv_t::kOff, result.swingv);  // i.e A toggle.
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

TEST(TestIRac, boolToString) {
  EXPECT_EQ("On", IRac::boolToString(true));
  EXPECT_EQ("Off", IRac::boolToString(false));
}

TEST(TestIRac, opmodeToString) {
  EXPECT_EQ("Off", IRac::opmodeToString(stdAc::opmode_t::kOff));
  EXPECT_EQ("Auto", IRac::opmodeToString(stdAc::opmode_t::kAuto));
  EXPECT_EQ("Cool", IRac::opmodeToString(stdAc::opmode_t::kCool));
  EXPECT_EQ("UNKNOWN", IRac::opmodeToString((stdAc::opmode_t)500));
  // Home Assistant/Google Home differences.
  EXPECT_EQ("Fan", IRac::opmodeToString(stdAc::opmode_t::kFan, false));
  EXPECT_EQ("fan_only", IRac::opmodeToString(stdAc::opmode_t::kFan, true));
  EXPECT_EQ("Fan", IRac::opmodeToString(stdAc::opmode_t::kFan));  // Default
}

TEST(TestIRac, fanspeedToString) {
  EXPECT_EQ("Low", IRac::fanspeedToString(stdAc::fanspeed_t::kLow));
  EXPECT_EQ("Auto", IRac::fanspeedToString(stdAc::fanspeed_t::kAuto));
  EXPECT_EQ("UNKNOWN", IRac::fanspeedToString((stdAc::fanspeed_t)500));
}

TEST(TestIRac, swingvToString) {
  EXPECT_EQ("Off", IRac::swingvToString(stdAc::swingv_t::kOff));
  EXPECT_EQ("Low", IRac::swingvToString(stdAc::swingv_t::kLow));
  EXPECT_EQ("Auto", IRac::swingvToString(stdAc::swingv_t::kAuto));
  EXPECT_EQ("UNKNOWN", IRac::swingvToString((stdAc::swingv_t)500));
}

TEST(TestIRac, swinghToString) {
  EXPECT_EQ("Off", IRac::swinghToString(stdAc::swingh_t::kOff));
  EXPECT_EQ("Left", IRac::swinghToString(stdAc::swingh_t::kLeft));
  EXPECT_EQ("Auto", IRac::swinghToString(stdAc::swingh_t::kAuto));
  EXPECT_EQ("Wide", IRac::swinghToString(stdAc::swingh_t::kWide));
  EXPECT_EQ("UNKNOWN", IRac::swinghToString((stdAc::swingh_t)500));
}

// Check that we keep the previous state info if the message is a special
// state-less command.
TEST(TestIRac, CoolixDecodeToState) {
  stdAc::state_t prev;
  prev.mode = stdAc::opmode_t::kHeat;
  prev.power = true;
  prev.celsius = true;
  prev.degrees = 20;
  prev.fanspeed = stdAc::fanspeed_t::kLow;

  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();
  irsend.sendCOOLIX(kCoolixOff);  // Special state-less "off" message.
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  stdAc::state_t result;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &result, &prev));
  ASSERT_EQ(decode_type_t::COOLIX, result.protocol);
  ASSERT_FALSE(result.power);
  ASSERT_EQ(stdAc::opmode_t::kHeat, result.mode);
  ASSERT_TRUE(result.celsius);
  ASSERT_EQ(20, result.degrees);
  ASSERT_EQ(stdAc::fanspeed_t::kLow, result.fanspeed);
}

// Check light on/off functionality in Coolix common a/c handling.
TEST(TestIRac, Issue821) {
  stdAc::state_t prev;
  stdAc::state_t next;
  stdAc::state_t result;
  // state info from:
  // https://github.com/crankyoldgit/IRremoteESP8266/issues/821#issuecomment-513708970
  prev.protocol = decode_type_t::COOLIX;
  prev.model = -1;
  prev.power = true;
  prev.mode = stdAc::opmode_t::kAuto;
  prev.degrees = 24;
  prev.celsius = true;
  prev.fanspeed = stdAc::fanspeed_t::kAuto;
  prev.swingv = stdAc::swingv_t::kOff;
  prev.swingh = stdAc::swingh_t::kOff;
  prev.quiet = false;
  prev.turbo = false;
  prev.econo = false;
  prev.light = false;
  prev.filter = false;
  prev.clean = false;
  prev.beep = false;

  next = prev;
  next.light = true;

  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  IRCoolixAC ac(kGpioUnused);

  ac.begin();
  result = irac.handleToggles(next, &prev);
  ASSERT_TRUE(result.light);
  irac.sendAc(next, &prev);
  ASSERT_TRUE(next.light);
  irac.coolix(&ac,
              result.power,     // Power
              result.mode,      // Mode
              result.degrees,   // Celsius
              result.fanspeed,  // Fan speed
              result.swingv,    // Vertical swing
              result.swingh,    // Horizontal swing
              result.turbo,     // Turbo
              result.light,     // Light
              result.clean,     // Clean
              -1);              // Sleep
  ac._irsend.makeDecodeResult();
  // We expect a normal state message, followed by the special "light" message.
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(COOLIX, ac._irsend.capture.decode_type);
  ASSERT_EQ(kCoolixBits, ac._irsend.capture.bits);
  ASSERT_EQ("Power: On, Mode: 2 (Auto), Fan: 0 (Auto0), Temp: 24C, "
            "Zone Follow: Off, Sensor Temp: Off",
            IRAcUtils::resultAcToString(&ac._irsend.capture));
  EXPECT_TRUE(capture.decodeCOOLIX(&ac._irsend.capture, 201));
  ASSERT_EQ(COOLIX, ac._irsend.capture.decode_type);
  ASSERT_EQ(kCoolixBits, ac._irsend.capture.bits);
  ASSERT_EQ("Power: On, Light: Toggle",
            IRAcUtils::resultAcToString(&ac._irsend.capture));
  EXPECT_EQ(
      "f38000d50"
      "m4692s4416"
      "m552s1656m552s552m552s1656m552s1656m552s552m552s552m552s1656m552s552"
      "m552s552m552s1656m552s552m552s552m552s1656m552s1656m552s552m552s1656"
      "m552s552m552s552m552s552m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s1656m552s1656m552s1656m552s552m552s552m552s552m552s552m552s552"
      "m552s552m552s1656m552s552m552s552m552s1656m552s552m552s552m552s552"
      "m552s1656m552s552m552s1656m552s1656m552s552m552s1656m552s1656m552s1656"
      "m552s5244"
      "m4692s4416"
      "m552s1656m552s552m552s1656m552s1656m552s552m552s552m552s1656m552s552"
      "m552s552m552s1656m552s552m552s552m552s1656m552s1656m552s552m552s1656"
      "m552s552m552s552m552s552m552s1656m552s1656m552s1656m552s1656m552s1656"
      "m552s1656m552s1656m552s1656m552s552m552s552m552s552m552s552m552s552"
      "m552s552m552s1656m552s552m552s552m552s1656m552s552m552s552m552s552"
      "m552s1656m552s552m552s1656m552s1656m552s552m552s1656m552s1656m552s1656"
      "m552s105244"
      "m4692s4416"
      "m552s1656m552s552m552s1656m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s552m552s1656m552s552m552s552m552s1656m552s552m552s1656m552s552"
      "m552s1656m552s1656m552s1656m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s552m552s552m552s552m552s552m552s1656m552s552m552s1656m552s552"
      "m552s1656m552s552m552s1656m552s552m552s552m552s1656m552s552m552s1656"
      "m552s552m552s1656m552s552m552s1656m552s1656m552s552m552s1656m552s552"
      "m552s5244"
      "m4692s4416"
      "m552s1656m552s552m552s1656m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s552m552s1656m552s552m552s552m552s1656m552s552m552s1656m552s552"
      "m552s1656m552s1656m552s1656m552s1656m552s552m552s1656m552s552m552s1656"
      "m552s552m552s552m552s552m552s552m552s1656m552s552m552s1656m552s552"
      "m552s1656m552s552m552s1656m552s552m552s552m552s1656m552s552m552s1656"
      "m552s552m552s1656m552s552m552s1656m552s1656m552s552m552s1656m552s552"
      "m552s105244",
      ac._irsend.outputStr());
}

// Check power toggling in Whirlpool common a/c handling.
TEST(TestIRac, Issue1001) {
  stdAc::state_t desired;  // New desired state
  stdAc::state_t prev;     // Previously desired state
  stdAc::state_t result;   // State we need to send to get to `desired`
  prev.protocol = decode_type_t::WHIRLPOOL_AC;
  prev.model = 1;
  prev.power = true;
  prev.mode = stdAc::opmode_t::kAuto;
  prev.degrees = 24;
  prev.celsius = true;
  prev.fanspeed = stdAc::fanspeed_t::kAuto;
  prev.swingv = stdAc::swingv_t::kOff;
  prev.swingh = stdAc::swingh_t::kOff;
  prev.quiet = false;
  prev.turbo = false;
  prev.econo = false;
  prev.light = false;
  prev.filter = false;
  prev.clean = false;
  prev.beep = false;
  prev.sleep = -1;

  desired = prev;
  desired.power = false;

  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  IRWhirlpoolAc ac(kGpioUnused);

  ac.begin();
  ASSERT_TRUE(prev.power);
  ASSERT_FALSE(desired.power);
  result = irac.handleToggles(irac.cleanState(desired), &prev);
  ASSERT_TRUE(result.power);
  irac.sendAc(desired, &prev);
  ASSERT_FALSE(desired.power);
  irac.whirlpool(&ac,
                 (whirlpool_ac_remote_model_t)result.model,  // Model
                 result.power,     // Power
                 result.mode,      // Mode
                 result.degrees,   // Celsius
                 result.fanspeed,  // Fan speed
                 result.swingv,    // Vertical swing
                 result.turbo,     // Turbo
                 result.light,     // Light
                 result.sleep);    // Sleep
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(WHIRLPOOL_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kWhirlpoolAcBits, ac._irsend.capture.bits);
  ASSERT_EQ("Model: 1 (DG11J13A), Power Toggle: On, Mode: 1 (Auto), Temp: 24C, "
            "Fan: 0 (Auto), Swing: Off, Light: Off, Clock: 00:00, "
            "On Timer: Off, Off Timer: Off, Sleep: Off, Super: Off, "
            "Command: 1 (Power)",
            IRAcUtils::resultAcToString(&ac._irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));

  // Now check if the mode is set to "Off" instead of just change to power off.
  // i.e. How Home Assistant expects things to work.
  ac._irsend.reset();
  desired.power = true;
  desired.mode = stdAc::opmode_t::kOff;
  result = irac.handleToggles(irac.cleanState(desired), &prev);
  ASSERT_TRUE(result.power);
  irac.sendAc(desired, &prev);
  ASSERT_TRUE(desired.power);
  irac.whirlpool(&ac,
                 (whirlpool_ac_remote_model_t)result.model,  // Model
                 result.power,     // Power
                 result.mode,      // Mode
                 result.degrees,   // Celsius
                 result.fanspeed,  // Fan speed
                 result.swingv,    // Vertical swing
                 result.turbo,     // Turbo
                 result.light,     // Light
                 result.sleep);    // Sleep
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(WHIRLPOOL_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kWhirlpoolAcBits, ac._irsend.capture.bits);
  ASSERT_EQ("Model: 1 (DG11J13A), Power Toggle: On, Mode: 2 (Cool), Temp: 24C, "
            "Fan: 0 (Auto), Swing: Off, Light: Off, Clock: 00:00, "
            "On Timer: Off, Off Timer: Off, Sleep: Off, Super: Off, "
            "Command: 1 (Power)",
            IRAcUtils::resultAcToString(&ac._irsend.capture));
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &r, &p));
}

// Check power switching in Daikin2 common a/c handling when from an IR message.
TEST(TestIRac, Issue1035) {
  stdAc::state_t prev;     // Previously desired state
  stdAc::state_t result;   // State we need to send to get to `desired`
  prev.protocol = decode_type_t::DAIKIN2;
  prev.model = -1;
  prev.power = false;
  prev.mode = stdAc::opmode_t::kAuto;
  prev.degrees = 24;
  prev.celsius = true;
  prev.fanspeed = stdAc::fanspeed_t::kAuto;
  prev.swingv = stdAc::swingv_t::kOff;
  prev.swingh = stdAc::swingh_t::kOff;
  prev.quiet = false;
  prev.turbo = false;
  prev.econo = false;
  prev.light = false;
  prev.filter = false;
  prev.clean = false;
  prev.beep = false;
  prev.sleep = -1;

  // https://github.com/crankyoldgit/IRremoteESP8266/issues/1035#issuecomment-580963572
  const uint8_t on_code[kDaikin2StateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0x01, 0x15, 0x43, 0x90, 0x29, 0x0C, 0x80, 0x04,
      0xC0, 0x16, 0x24, 0x00, 0x00, 0xBE, 0xC1, 0x2D, 0x11, 0xDA, 0x27, 0x00,
      0x00, 0x09, 0x2A, 0x00, 0xB0, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0xC1,
      0x90, 0x60, 0x0C};
  const uint8_t off_code[kDaikin2StateLength] = {
      0x11, 0xDA, 0x27, 0x00, 0x01, 0x15, 0xC3, 0x90, 0x29, 0x0C, 0x80, 0x04,
      0xC0, 0x16, 0x24, 0x00, 0x00, 0xBE, 0xD1, 0xBD, 0x11, 0xDA, 0x27, 0x00,
      0x00, 0x08, 0x2A, 0x00, 0xB0, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0xC1,
      0x90, 0x60, 0x0B};

  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);
  IRDaikin2 ac(kGpioUnused);

  ac.begin();
  ac.setRaw(on_code);
  ac.send();
  ac._irsend.makeDecodeResult();
  ASSERT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(DAIKIN2, ac._irsend.capture.decode_type);
  ASSERT_FALSE(prev.power);
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &result, &prev));
  ASSERT_TRUE(result.power);

  prev = result;

  ac._irsend.reset();
  ac.setRaw(off_code);
  ac.send();
  ac._irsend.makeDecodeResult();
  ASSERT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(DAIKIN2, ac._irsend.capture.decode_type);
  ASSERT_TRUE(prev.power);
  ASSERT_TRUE(IRAcUtils::decodeToState(&ac._irsend.capture, &result, &prev));
  ASSERT_FALSE(result.power);
}

TEST(TestIRac, Issue1250) {
  IRToshibaAC ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);

  ac.begin();
  irac.next.protocol = decode_type_t::TOSHIBA_AC;  // Set a protocol to use.
  irac.next.model = 1;  // Some A/Cs have different models. Try just the first.
  irac.next.mode = stdAc::opmode_t::kFan;  // Run in Fan mode initially.
  irac.next.celsius = true;  // Use Celsius for temp units. False = Fahrenheit
  irac.next.degrees = 19;  // 19 degrees.
  irac.next.fanspeed = stdAc::fanspeed_t::kAuto;  // Start the fan at Auto.
  irac.next.swingv = stdAc::swingv_t::kOff;  // Don't swing the fan up or down.
  irac.next.swingh = stdAc::swingh_t::kOff;  // Don't swing the fan left/right.
  irac.next.light = true;  // Turn off any LED/Lights/Display that we can.
  irac.next.beep = false;  // Turn off any beep from the A/C if we can.
  irac.next.econo = false;  // Turn off any economy modes if we can.
  irac.next.filter = false;  // Turn off any Ion/Mold/Health filters if we can.
  irac.next.turbo = false;  // Don't use any turbo/powerful/etc modes.
  irac.next.quiet = false;  // Don't use any quiet/silent/etc modes.
  irac.next.sleep = -1;  // Don't set any sleep time or modes.
  irac.next.clean = false;  // Turn off any Cleaning options if we can.
  irac.next.clock = -1;  // Don't set any current time if we can avoid it.
  irac.next.power = true;  // Initially start with the unit on.

  stdAc::state_t copy_of_next_pre_send = irac.next;
  irac.sendAc();
  // Confirm nothing in the state changed with the send.
  ASSERT_FALSE(IRac::cmpStates(irac.next, copy_of_next_pre_send));

  // Now send the state so we can actually decode/capture what we sent.
  char expected_on[] =
      "Temp: 19C, Power: On, Mode: 4 (Fan), Fan: 0 (Auto), "
      "Turbo: Off, Econo: Off, Filter: Off";
  ac._irsend.reset();
  irac.toshiba(&ac,
               irac.next.power,     // Power
               irac.next.mode,      // Mode
               irac.next.degrees,   // Celsius
               irac.next.fanspeed,  // Fan speed
               irac.next.swingv,    // Vertical Swing
               irac.next.turbo,     // Turbo
               irac.next.econo,     // Econo
               irac.next.filter);   // Filter
  ASSERT_EQ(expected_on, ac.toString());
  ASSERT_EQ(kToshibaACStateLength, ac.getStateLength());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(TOSHIBA_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kToshibaACBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected_on, IRAcUtils::resultAcToString(&ac._irsend.capture));


  irac.next.power = false;
  ASSERT_TRUE(IRac::cmpStates(irac.next, copy_of_next_pre_send));
  copy_of_next_pre_send = irac.next;
  irac.sendAc();
  // Now send the state so we can actually decode/capture what we sent.
  char expected_off[] =
      "Temp: 19C, Power: Off, Fan: 0 (Auto), Turbo: Off, Econo: Off, "
      "Filter: Off";
  ac._irsend.reset();
  irac.toshiba(&ac,
               irac.next.power,     // Power
               irac.next.mode,      // Mode
               irac.next.degrees,   // Celsius
               irac.next.fanspeed,  // Fan speed
               irac.next.swingv,    // Vertical Swing
               irac.next.turbo,     // Turbo
               irac.next.econo,     // Econo
               irac.next.filter);   // Filter
  ASSERT_EQ(expected_off, ac.toString());
  ASSERT_EQ(kToshibaACStateLength, ac.getStateLength());
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(capture.decode(&ac._irsend.capture));
  ASSERT_EQ(TOSHIBA_AC, ac._irsend.capture.decode_type);
  ASSERT_EQ(kToshibaACBits, ac._irsend.capture.bits);
  ASSERT_EQ(expected_off, IRAcUtils::resultAcToString(&ac._irsend.capture));
  // Confirm nothing in the state changed with the send.
  ASSERT_FALSE(IRac::cmpStates(irac.next, copy_of_next_pre_send));
}

// Ensure Protocols that expect the IRac::sendAC() call to have a prev value set
// still works when it is NULL. i.e. It doesn't crash.
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1339
TEST(TestIRac, Issue1339) {
  IRac irac(kGpioUnused);
  stdAc::state_t to_send;

  to_send.protocol = decode_type_t::SAMSUNG_AC;
  ASSERT_TRUE(irac.sendAc(to_send, NULL));
  to_send.protocol = decode_type_t::SHARP_AC;
  ASSERT_TRUE(irac.sendAc(to_send, NULL));
  to_send.protocol = decode_type_t::HITACHI_AC1;
  ASSERT_TRUE(irac.sendAc(to_send, NULL));
}

// See if we handle an inbound Swing Toggle IR command correctly.
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1424#issuecomment-787998777
TEST(TestIRac, Issue1424) {
  IRToshibaAC ac(kGpioUnused);
  IRac irac(kGpioUnused);
  IRrecv capture(kGpioUnused);

  ac.begin();
  irac.next.protocol = decode_type_t::TOSHIBA_AC;  // Set a protocol to use.
  irac.next.model = 1;  // Some A/Cs have different models. Try just the first.
  irac.next.mode = stdAc::opmode_t::kFan;  // Run in Fan mode initially.
  irac.next.celsius = true;  // Use Celsius for temp units. False = Fahrenheit
  irac.next.degrees = 19;  // 19 degrees.
  irac.next.fanspeed = stdAc::fanspeed_t::kAuto;  // Start the fan at Auto.
  irac.next.swingv = stdAc::swingv_t::kOff;  // Don't swing the fan up or down.
  irac.next.swingh = stdAc::swingh_t::kOff;  // Don't swing the fan left/right.
  irac.next.light = true;  // Turn off any LED/Lights/Display that we can.
  irac.next.beep = false;  // Turn off any beep from the A/C if we can.
  irac.next.econo = false;  // Turn off any economy modes if we can.
  irac.next.filter = false;  // Turn off any Ion/Mold/Health filters if we can.
  irac.next.turbo = false;  // Don't use any turbo/powerful/etc modes.
  irac.next.quiet = false;  // Don't use any quiet/silent/etc modes.
  irac.next.sleep = -1;  // Don't set any sleep time or modes.
  irac.next.clean = false;  // Turn off any Cleaning options if we can.
  irac.next.clock = -1;  // Don't set any current time if we can avoid it.
  irac.next.power = true;  // Initially start with the unit on.

  // Start with the SwingV being off.
  stdAc::state_t copy_of_next_pre_send = irac.next;
  irac.sendAc();
  // Confirm nothing in the state changed with the send.
  ASSERT_FALSE(IRac::cmpStates(irac.next, copy_of_next_pre_send));


  irac.next.swingv = stdAc::swingv_t::kAuto;  // Turn on the swing.
  ASSERT_TRUE(IRac::cmpStates(irac.next, copy_of_next_pre_send));
  copy_of_next_pre_send = irac.next;
  irac.sendAc();
  // Confirm it is NOT Off. i.e. On.
  EXPECT_NE(stdAc::swingv_t::kOff, irac.next.swingv);
  // Confirm nothing in the state changed with the send.
  ASSERT_FALSE(IRac::cmpStates(irac.next, copy_of_next_pre_send));

  stdAc::state_t copy_of_next_pre_receive = irac.next;  // aka. Prev.
  // Simulate receiving a Swing Toggle message.
  const uint8_t swingToggleState[kToshibaACStateLengthShort] = {
        0xF2, 0x0D, 0x01, 0xFE, 0x21, 0x04, 0x25};
  ac.setRaw(swingToggleState, kToshibaACStateLengthShort);
  // Import the new "state" to IRac.
  irac.next = ac.toCommon(&copy_of_next_pre_receive);
  // The toggle should have turned the effective "On" state to "Off".
  EXPECT_EQ(irac.next.swingv, stdAc::swingv_t::kOff);
  // Confirm the state really did change.
  ASSERT_TRUE(IRac::cmpStates(irac.next, copy_of_next_pre_receive));
}

// Confirm/check that the default initialisation of a state_t is as expected.
TEST(TestIRac, initState) {
  IRac irac(kGpioUnused);
  stdAc::state_t builtin_init{};
  stdAc::state_t custom_init;
  stdAc::state_t no_init;
  IRac::initState(&custom_init);

  EXPECT_FALSE(IRac::cmpStates(builtin_init, custom_init));
  builtin_init.protocol = decode_type_t::SAMSUNG_AC;
  EXPECT_TRUE(IRac::cmpStates(builtin_init, custom_init));
  EXPECT_FALSE(IRac::cmpStates(no_init, custom_init));
  EXPECT_EQ(-1, builtin_init.model);
  EXPECT_EQ(stdAc::swingv_t::kOff, builtin_init.swingv);
  EXPECT_EQ(decode_type_t::UNKNOWN, no_init.protocol);
}
