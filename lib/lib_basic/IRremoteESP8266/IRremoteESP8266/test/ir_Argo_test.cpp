// Copyright 2019 David Conran
// Copyright 2022 Mateusz Bronk (mbronk)
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include "ir_Argo.h"
#include "IRac.h"
#include "IRrecv.h"
#include "IRrecv_test.h"
#include "IRsend.h"
#include "IRsend_test.h"
#include "./ut_utils.h"

/******************************************************************************/
/* Tests for toCommon()                                                       */
/******************************************************************************/

TEST(TestArgoACClass, toCommon) {
  IRArgoAC ac(kGpioUnused);
  ac.setPower(true);
  ac.setMode(kArgoCool);
  ac.setTemp(20);
  ac.setFan(kArgoFan3);
  ac.setMax(true);
  ac.setNight(true);
  // Now test it.
  ASSERT_EQ(decode_type_t::ARGO, ac.toCommon().protocol);
  ASSERT_EQ(stdAc::ac_command_t::kControlCommand, ac.toCommon().command);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_EQ(20, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_EQ(0, ac.toCommon().sleep);
  ASSERT_TRUE(ac.toCommon().turbo);
  ASSERT_EQ(argo_ac_remote_model_t::SAC_WREM2, ac.toCommon().model);
  // Unsupported.
  ASSERT_EQ(stdAc::swingv_t::kOff, ac.toCommon().swingv);
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  ASSERT_FALSE(ac.toCommon().econo);
  ASSERT_FALSE(ac.toCommon().light);
  ASSERT_FALSE(ac.toCommon().filter);
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_FALSE(ac.toCommon().beep);
  ASSERT_FALSE(ac.toCommon().quiet);
  ASSERT_EQ(-1, ac.toCommon().clock);
  ASSERT_FALSE(ac.toCommon().iFeel);
  ASSERT_EQ(25, ac.toCommon().sensorTemperature);
}

TEST(TestArgoAC_WREM3Class, toCommon) {
  IRArgoAC_WREM3 ac(kGpioUnused);
  ac.setPower(true);
  ac.setMode(argoMode_t::COOL);
  ac.setTemp(21);
  ac.setFan(argoFan_t::FAN_HIGHEST);
  ac.setMax(true);
  ac.setNight(true);
  ac.setFlap(argoFlap_t::FLAP_4);
  ac.setEco(true);
  ac.setFilter(true);
  ac.setLight(true);
  ac.setiFeel(true);
  // Now test it.
  ASSERT_EQ(decode_type_t::ARGO, ac.toCommon().protocol);
  ASSERT_EQ(argo_ac_remote_model_t::SAC_WREM3, ac.toCommon().model);
  ASSERT_EQ(stdAc::ac_command_t::kControlCommand, ac.toCommon().command);
  ASSERT_TRUE(ac.toCommon().celsius);
  ASSERT_TRUE(ac.toCommon().beep);  // Always on (except for iFeel)
  ASSERT_FALSE(ac.toCommon().clean);
  ASSERT_EQ(-1, ac.toCommon().clock);
  ASSERT_EQ(0, ac.toCommon().sleep);
  ASSERT_EQ(stdAc::swingh_t::kOff, ac.toCommon().swingh);
  ASSERT_TRUE(ac.toCommon().power);
  ASSERT_EQ(stdAc::opmode_t::kCool, ac.toCommon().mode);
  ASSERT_EQ(21, ac.toCommon().degrees);
  ASSERT_EQ(stdAc::fanspeed_t::kMax, ac.toCommon().fanspeed);
  ASSERT_TRUE(ac.toCommon().turbo);
  ASSERT_TRUE(ac.toCommon().quiet);  // Night
  ASSERT_EQ(stdAc::swingv_t::kUpperMiddle, ac.toCommon().swingv);
  ASSERT_TRUE(ac.toCommon().econo);
  ASSERT_TRUE(ac.toCommon().light);
  ASSERT_TRUE(ac.toCommon().filter);
  ASSERT_TRUE(ac.toCommon().iFeel);
  ASSERT_EQ(25, ac.toCommon().sensorTemperature);
}

/******************************************************************************/
/* Tests of message construction                                              */
/******************************************************************************/

TEST(TestArgoACClass, MessageConstructon) {
  IRArgoAC ac(kGpioUnused);
  ac.setPower(true);
  ac.setTemp(20);
  ac.setMode(kArgoCool);
  ac.setFan(kArgoFanAuto);
  ac.setSensorTemp(21);
  ac.setiFeel(true);
  ac.setMax(true);
  ac.setNight(true);

  // Don't implicitly trust this. It's just a guess.
  auto expected = std::vector<uint8_t>({
      0xAC, 0xF5, 0x00, 0x24, 0x02, 0x00, 0x00, 0x00, 0x00, 0xAC, 0xD6, 0x01});
  auto actual = ac.getRaw();
  EXPECT_THAT(std::vector<uint8_t>(actual, actual + kArgoBits / 8),
              ::testing::ElementsAreArray(expected));
  EXPECT_EQ(
      "Model: 1 (WREM2), Power: On, Mode: 0 (Cool), Fan: 0 (Auto), Temp: 20C, "
      "Sensor Temp: 21C, Max: On, IFeel: On, Night: On",
      ac.toString());
}

TEST(TestArgoAC_WREM3Class, MessageConstructon_ACControl) {
  IRArgoAC_WREM3 ac(kGpioUnused);
  ac.setChannel(0);
  ac.setPower(true);
  ac.setMode(argoMode_t::COOL);
  ac.setTemp(22);
  ac.setSensorTemp(26);
  ac.setFan(argoFan_t::FAN_AUTO);
  ac.setFlap(argoFlap_t::FLAP_FULL);
  ac.setiFeel(false);
  ac.setNight(false);
  ac.setEco(false);
  ac.setMax(false);
  ac.setFilter(false);
  ac.setLight(true);
  auto expected = std::vector<uint8_t>({
      0x0B, 0x36, 0x12, 0x0F, 0xC2, 0x24});
  auto actual = ac.getRaw();
  ASSERT_EQ(ac.getRawByteLength(), kArgo3AcControlStateLength);
  EXPECT_THAT(std::vector<uint8_t>(actual, actual + ac.getRawByteLength()),
              ::testing::ElementsAreArray(expected));
  EXPECT_EQ(
      "Command[CH#0]: Model: 2 (WREM3), Power: On, Mode: 1 (Cool), Temp: 22C, "
      "Sensor Temp: 26C, Fan: 0 (Auto), Swing(V): 7 (Breeze), IFeel: Off, "
      "Night: Off, Econo: Off, Max: Off, Filter: Off, Light: On",
      ac.toString());
}

TEST(TestArgoAC_WREM3Class, MessageConstructon_ACControl_2) {
  IRArgoAC_WREM3 ac(kGpioUnused);
  ac.setChannel(2);
  ac.setPower(true);
  ac.setMode(argoMode_t::AUTO);
  ac.setTemp(23);
  ac.setSensorTemp(28);
  ac.setFan(argoFan_t::FAN_MEDIUM);
  ac.setMax(true);
  ac.setNight(true);
  ac.setFlap(argoFlap_t::FLAP_4);
  ac.setEco(true);
  ac.setFilter(true);
  ac.setLight(true);
  ac.setiFeel(true);
  auto expected = std::vector<uint8_t>({
      0x2B, 0xB8, 0x93, 0xFC, 0xC3, 0x35});
  auto actual = ac.getRaw();
  ASSERT_EQ(ac.getRawByteLength(), kArgo3AcControlStateLength);
  EXPECT_THAT(std::vector<uint8_t>(actual, actual + ac.getRawByteLength()),
              ::testing::ElementsAreArray(expected));
  EXPECT_EQ(
      "Command[CH#2]: Model: 2 (WREM3), Power: On, Mode: 5 (Auto), Temp: 23C, "
      "Sensor Temp: 28C, Fan: 4 (Med-High), Swing(V): 4 (Middle), IFeel: On, "
      "Night: On, Econo: On, Max: On, Filter: On, Light: On",
      ac.toString());
}

TEST(TestArgoAC_WREM3Class, MessageConstructon_iFeelReport) {
  IRArgoAC_WREM3 ac(kGpioUnused);
  ac.setMessageType(argoIrMessageType_t::IFEEL_TEMP_REPORT);
  ac.setSensorTemp(31);

  auto expected = std::vector<uint8_t>({0x4B, 0xDB});
  auto actual = ac.getRaw();
  ASSERT_EQ(ac.getRawByteLength(), kArgo3iFeelReportStateLength);
  EXPECT_THAT(std::vector<uint8_t>(actual, actual + ac.getRawByteLength()),
              ::testing::ElementsAreArray(expected));
  EXPECT_EQ(
      "IFeel Report[CH#0]: Model: 2 (WREM3), Sensor Temp: 31C",
      ac.toString());
}

TEST(TestArgoAC_WREM3Class, MessageConstructon_Config) {
  IRArgoAC_WREM3 ac(kGpioUnused);
  ac.setMessageType(argoIrMessageType_t::CONFIG_PARAM_SET);
  ac.setConfigEntry(6, 30);

  auto expected = std::vector<uint8_t>({0xCB, 0x06, 0x1E, 0xEF});
  auto actual = ac.getRaw();
  ASSERT_EQ(ac.getRawByteLength(), kArgo3ConfigStateLength);
  EXPECT_THAT(std::vector<uint8_t>(actual, actual + ac.getRawByteLength()),
              ::testing::ElementsAreArray(expected));
  EXPECT_EQ(
      "Config[CH#0]: Model: 2 (WREM3), Key: 6, Value: 30",
      ac.toString());
}

TEST(TestArgoAC_WREM3Class, MessageConstructon_NoTimer) {
  IRArgoAC_WREM3 ac(kGpioUnused);
  ac.setMessageType(argoIrMessageType_t::TIMER_COMMAND);
  ac.off();
  ac.setCurrentTimeMinutes(1*60+59);
  ac.setCurrentDayOfWeek(argoWeekday::MONDAY);
  ac.setTimerType(argoTimerType_t::NO_TIMER);

  auto expected = std::vector<uint8_t>({0x8B, 0x70, 0x87, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x34});
  auto actual = ac.getRaw();
  ASSERT_EQ(ac.getRawByteLength(), kArgo3TimerStateLength);
  EXPECT_THAT(std::vector<uint8_t>(actual, actual + ac.getRawByteLength()),
              ::testing::ElementsAreArray(expected));
  EXPECT_EQ(
      "Timer[CH#0]: Model: 2 (WREM3), Power: Off, Timer Mode: 0 (Off), "
      "Clock: 01:59, Day: 1 (Mon), Timer: Off",
      ac.toString());
}

TEST(TestArgoAC_WREM3Class, MessageConstructon_DelayTimer) {
  IRArgoAC_WREM3 ac(kGpioUnused);
  ac.setMessageType(argoIrMessageType_t::TIMER_COMMAND);
  ac.off();
  ac.setCurrentTimeMinutes(12*60+00);
  ac.setCurrentDayOfWeek(argoWeekday::SATURDAY);
  ac.setTimerType(argoTimerType_t::DELAY_TIMER);
  ac.setDelayTimerMinutes(9*60+40);

  auto expected = std::vector<uint8_t>({0x8B, 0x02, 0x2D, 0x13, 0x09, 0x00,
                                        0x00, 0x00, 0xD4});
  auto actual = ac.getRaw();
  ASSERT_EQ(ac.getRawByteLength(), kArgo3TimerStateLength);
  EXPECT_THAT(std::vector<uint8_t>(actual, actual + ac.getRawByteLength()),
              ::testing::ElementsAreArray(expected));
  EXPECT_EQ(
      "Timer[CH#0]: Model: 2 (WREM3), Power: Off, Timer Mode: 1 (Sleep Timer),"
      " Clock: 12:00, Day: 6 (Sat), Timer: 09:40",
      ac.toString());
}

TEST(TestArgoAC_WREM3Class, MessageConstructon_ScheduleTimer) {
  IRArgoAC_WREM3 ac(kGpioUnused);
  ac.setMessageType(argoIrMessageType_t::TIMER_COMMAND);
  ac.on();
  ac.setCurrentTimeMinutes(18*60+16);
  ac.setCurrentDayOfWeek(argoWeekday::SATURDAY);
  ac.setTimerType(argoTimerType_t::SCHEDULE_TIMER_3);
  ac.setScheduleTimerStartMinutes(8*60+40);
  ac.setScheduleTimerStopMinutes(19*60+50);
  ac.setScheduleTimerActiveDays({argoWeekday::MONDAY, argoWeekday::SATURDAY,
                                 argoWeekday::SUNDAY});

  auto expected = std::vector<uint8_t>({0x8B, 0x89, 0x44, 0x03, 0x00, 0x41,
                                        0xA6, 0x1C, 0x26});
  auto actual = ac.getRaw();
  ASSERT_EQ(ac.getRawByteLength(), kArgo3TimerStateLength);
  EXPECT_THAT(std::vector<uint8_t>(actual, actual + ac.getRawByteLength()),
              ::testing::ElementsAreArray(expected));
  EXPECT_EQ(
      "Timer[CH#0]: Model: 2 (WREM3), Power: On, Timer Mode: 4 (Schedule3),"
      " Clock: 18:16, Day: 6 (Sat), On Timer: 08:40, Off Timer: 19:50, "
      "TimerActiveDays: Sun|Mon|Sat",
      ac.toString());
}

/******************************************************************************/
/* Tests for sendArgo().                                                      */
/******************************************************************************/

// Test sending typical data only.
TEST(TestSendArgo, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();
  uint8_t data[kArgoStateLength] = {
      0xAC, 0xF5, 0x00, 0x24, 0x02, 0x00, 0x00, 0x00, 0x00, 0xAC, 0xD6, 0x01};

  irsend.sendArgo(data);
  EXPECT_EQ(
      "f38000d50"
      "m6400s3300"
      "m400s900m400s900m400s2200m400s2200m400s900m400s2200m400s900m400s2200"
      "m400s2200m400s900m400s2200m400s900m400s2200m400s2200m400s2200m400s2200"
      "m400s900m400s900m400s900m400s900m400s900m400s900m400s900m400s900"
      "m400s900m400s900m400s2200m400s900m400s900m400s2200m400s900m400s900"
      "m400s900m400s2200m400s900m400s900m400s900m400s900m400s900m400s900"
      "m400s900m400s900m400s900m400s900m400s900m400s900m400s900m400s900"
      "m400s900m400s900m400s900m400s900m400s900m400s900m400s900m400s900"
      "m400s900m400s900m400s900m400s900m400s900m400s900m400s900m400s900"
      "m400s900m400s900m400s900m400s900m400s900m400s900m400s900m400s900"
      "m400s900m400s900m400s2200m400s2200m400s900m400s2200m400s900m400s2200"
      "m400s900m400s2200m400s2200m400s900m400s2200m400s900m400s2200m400s2200"
      "m400s2200m400s900m400s900m400s900m400s900m400s900m400s900"
      "m400s900",
      irsend.outputStr());
}

TEST(TestSendArgoWrem3, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();
  uint8_t data[kArgoStateLength] = {
      0x0B, 0x31, 0x35, 0xFE, 0xC0, 0x2F};

  irsend.sendArgoWREM3(data);
  EXPECT_EQ(
      "f38000d50"
      "m6400s3300"
      "m400s2200m400s2200m400s900m400s2200m400s900m400s900m400s900m400s900"
      "m400s2200m400s900m400s900m400s900m400s2200m400s2200m400s900m400s900"
      "m400s2200m400s900m400s2200m400s900m400s2200m400s2200m400s900m400s900"
      "m400s900m400s2200m400s2200m400s2200m400s2200m400s2200m400s2200m400s2200"
      "m400s900m400s900m400s900m400s900m400s900m400s900m400s2200m400s2200"
      "m400s2200m400s2200m400s2200m400s2200m400s900m400s2200m400s900m400s900"
      "m400s900m400s900m400s900m400s900m400s900m400s900m400s900m400s900m400s900"
      "m400s900m400s900m400s900m400s900m400s900m400s900m400s900m400s900m400s900"
      "m400s900m400s900m400s900m400s900m400s900m400s900m400s900m400s900m400s900"
      "m400s900m400s900m400s900m400s900m400s900m400s900m400s900m400s900m400s900"
      "m400s900m400s900m400s900m400s900m400s900m400s900m400s900m400s900m400s900"
      "m400s900m400s900m400s900m400s100000",
      irsend.outputStr());
}

/******************************************************************************/
/* Tests for decodeArgo().                                                    */
/******************************************************************************/

// Decode normal Argo messages.
TEST(TestDecodeArgo, SyntheticDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Synthesised Normal Argo message.
  irsend.reset();
  uint8_t expectedState[kArgoStateLength] = {
      0xAC, 0xF5, 0x00, 0x24, 0x02, 0x00, 0x00, 0x00, 0x00, 0xAC, 0xD6, 0x01};
  irsend.sendArgo(expectedState);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::ARGO, irsend.capture.decode_type);
  EXPECT_EQ(kArgoBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Model: 1 (WREM2), Power: On, Mode: 0 (Cool), Fan: 0 (Auto), Temp: 20C, "
      "Sensor Temp: 21C, Max: On, IFeel: On, Night: On",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  ASSERT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
}

// Synthetic send and decode ***via common*** interface
TEST(TestIrAc, ArgoWrem2_SyntheticSendAndDecode_ACCommand) {
  IRac irac(kGpioUnused);
  auto capture = std::make_shared<IRrecv>(kGpioUnused);
  irac._utReceiver = capture;

  stdAc::state_t state = {};
  state.protocol = ARGO;
  state.model = 1;
  state.power = true;
  state.mode = stdAc::opmode_t::kCool;

  irac.sendAc(state, nullptr);
  ASSERT_NE(nullptr, irac._lastDecodeResults);
  EXPECT_EQ(ARGO, irac._lastDecodeResults->decode_type);
  EXPECT_EQ("Model: 1 (WREM2), Power: On, Mode: 0 (Cool), Fan: 0 (Auto), "
            "Temp: 25C, Sensor Temp: 25C, Max: Off, IFeel: Off, Night: Off",
    IRAcUtils::resultAcToString(irac._lastDecodeResults.get()));

  stdAc::state_t r = {};
  ASSERT_TRUE(IRAcUtils::decodeToState(irac._lastDecodeResults.get(), &r,
                                      nullptr));
  EXPECT_EQ(ARGO, r.protocol);
  EXPECT_EQ(1, r.model);
  EXPECT_TRUE(r.power);
  EXPECT_EQ(state.mode, r.mode);
}

TEST(TestIrAc, ArgoWrem3_SyntheticSendAndDecode_ACCommand) {
  IRac irac(kGpioUnused);
  auto capture = std::make_shared<IRrecv>(kGpioUnused);
  irac._utReceiver = capture;

  stdAc::state_t state = {};
  state.protocol = ARGO;
  state.model = argo_ac_remote_model_t::SAC_WREM3;
  state.power = true;
  state.mode = stdAc::opmode_t::kCool;

  irac.sendAc(state, nullptr);
  ASSERT_NE(nullptr, irac._lastDecodeResults);
  EXPECT_EQ(ARGO, irac._lastDecodeResults->decode_type);
  EXPECT_EQ("Command[CH#0]: Model: 2 (WREM3), Power: On, Mode: 1 (Cool),"
            " Temp: 25C, Sensor Temp: 25C, Fan: 0 (Auto), Swing(V): 7 (Breeze),"
            " IFeel: Off, Night: Off, Econo: Off, Max: Off, Filter: Off, "
            "Light: Off",
    IRAcUtils::resultAcToString(irac._lastDecodeResults.get()));

  stdAc::state_t r = {};
  ASSERT_TRUE(IRAcUtils::decodeToState(irac._lastDecodeResults.get(), &r,
                                       nullptr));
  EXPECT_EQ(ARGO, r.protocol);
  EXPECT_EQ(state.model, r.model);
  EXPECT_EQ(state.power, r.power);
}

TEST(TestIrAc, ArgoWrem3_SyntheticSendAndDecode_iFeelReport) {
  IRac irac(kGpioUnused);
  auto capture = std::make_shared<IRrecv>(kGpioUnused);
  irac._utReceiver = capture;

  stdAc::state_t state = {};
  state.protocol = ARGO;
  state.model = argo_ac_remote_model_t::SAC_WREM3;
  state.command = stdAc::ac_command_t::kSensorTempReport;
  state.sensorTemperature = 18.9;  // expected to be rounded up

  irac.sendAc(state, nullptr);

  ASSERT_NE(nullptr, irac._lastDecodeResults);
  EXPECT_EQ(ARGO, irac._lastDecodeResults->decode_type);
  EXPECT_EQ("IFeel Report[CH#0]: Model: 2 (WREM3), Sensor Temp: 19C",
            IRAcUtils::resultAcToString(irac._lastDecodeResults.get()));

  stdAc::state_t r = {};
  ASSERT_TRUE(IRAcUtils::decodeToState(irac._lastDecodeResults.get(), &r,
                                       nullptr));
  EXPECT_EQ(ARGO, r.protocol);
  EXPECT_EQ(state.model, r.model);
  EXPECT_EQ(state.command, r.command);
  EXPECT_EQ(19, r.sensorTemperature);
}

TEST(TestIrAc, ArgoWrem3_SyntheticSendAndDecode_Timer) {
  IRac irac(kGpioUnused);
  auto capture = std::make_shared<IRrecv>(kGpioUnused);
  irac._utReceiver = capture;

  stdAc::state_t state = {};
  state.protocol = ARGO;
  state.model = argo_ac_remote_model_t::SAC_WREM3;
  state.command = stdAc::ac_command_t::kTimerCommand;
  state.power = true;
  state.mode = stdAc::opmode_t::kAuto;  // Needs to be set for `state.power`
                                        // not to be ignored!
  state.clock = 13*60+21;
  state.sleep = 2*60+10;

  irac.sendAc(state, nullptr);

  ASSERT_NE(nullptr, irac._lastDecodeResults);
  EXPECT_EQ(ARGO, irac._lastDecodeResults->decode_type);
  EXPECT_EQ("Timer[CH#0]: Model: 2 (WREM3), Power: On, Timer Mode: 1 "
            "(Sleep Timer), Clock: 13:21, Day: 0 (Sun), Timer: 02:10",
    IRAcUtils::resultAcToString(irac._lastDecodeResults.get()));

  stdAc::state_t r = {};
  ASSERT_TRUE(IRAcUtils::decodeToState(irac._lastDecodeResults.get(), &r,
                                       nullptr));
  EXPECT_EQ(ARGO, r.protocol);
  EXPECT_EQ(state.model, r.model);
  EXPECT_EQ(state.command, r.command);
  EXPECT_EQ(state.power, r.power);
  EXPECT_EQ(state.clock, r.clock);
  EXPECT_EQ(state.sleep, r.sleep);
}

///
/// @brief Test fixture for Config messages sent via @c IRAc generic i-face
/// @note The commands are abusing generic intafrace and instead are
///       using: @c clock -> for settingID
///              @c sleep -> for setting Value
///
class TestArgoConfigViaIRAc :
    public ::testing::TestWithParam<std::tuple<uint16_t, uint16_t>> {
};

class TestArgoConfigViaIRAc_Positive : public TestArgoConfigViaIRAc {};
TEST_P(TestArgoConfigViaIRAc_Positive,
    ArgoWrem3_SyntheticSendAndDecode_Config_Positive) {
  int16_t settingId = std::get<0>(GetParam());
  int16_t settingValue = std::get<1>(GetParam());
  IRac irac(kGpioUnused);
  auto capture = std::make_shared<IRrecv>(kGpioUnused);
  irac._utReceiver = capture;

  stdAc::state_t state = {};
  state.protocol = ARGO;
  state.model = argo_ac_remote_model_t::SAC_WREM3;
  state.command = stdAc::ac_command_t::kConfigCommand;
  state.clock = settingId;
  state.sleep = settingValue;

  irac.sendAc(state, nullptr);

  ASSERT_NE(nullptr, irac._lastDecodeResults);
  EXPECT_EQ(ARGO, irac._lastDecodeResults->decode_type);
  std::ostringstream ossExpectedStr;
  ossExpectedStr << "Config[CH#0]: Model: 2 (WREM3), Key: " << settingId
                 << ", Value: " << settingValue;
  EXPECT_EQ(ossExpectedStr.str(),
    IRAcUtils::resultAcToString(irac._lastDecodeResults.get()));

  stdAc::state_t r = {};
  ASSERT_TRUE(IRAcUtils::decodeToState(irac._lastDecodeResults.get(), &r,
                                       nullptr));
  EXPECT_EQ(ARGO, r.protocol);
  EXPECT_EQ(state.model, r.model);
  EXPECT_EQ(state.command, r.command);
}

INSTANTIATE_TEST_CASE_P(
  TestIrAc,
  TestArgoConfigViaIRAc_Positive,
  ::testing::Values(
    std::make_tuple(5, 0),
    std::make_tuple(5, 1),
    std::make_tuple(6, 0),
    std::make_tuple(6, 1),
    std::make_tuple(6, 2),
    std::make_tuple(6, 3),
    std::make_tuple(12, 30),
    std::make_tuple(12, 75),
    std::make_tuple(12, 99)
));

class TestArgoConfigViaIRAc_Negative : public TestArgoConfigViaIRAc {};
TEST_P(TestArgoConfigViaIRAc_Negative,
    ArgoWrem3_SyntheticSendAndDecode_Config_Negative) {
  int16_t settingId = std::get<0>(GetParam());
  int16_t settingValue = std::get<1>(GetParam());
  IRac irac(kGpioUnused);
  auto capture = std::make_shared<IRrecv>(kGpioUnused);
  irac._utReceiver = capture;

  stdAc::state_t state = {};
  state.protocol = ARGO;
  state.model = argo_ac_remote_model_t::SAC_WREM3;
  state.command = stdAc::ac_command_t::kConfigCommand;
  state.clock = settingId;
  state.sleep = settingValue;

  irac.sendAc(state, nullptr);

  // The "safe" mode should have prevented the message from sending out
  ASSERT_EQ(nullptr, irac._lastDecodeResults);  // nothing got sent
}

INSTANTIATE_TEST_CASE_P(
  TestIrAc,
  TestArgoConfigViaIRAc_Negative,
  ::testing::Values(
    std::make_tuple(5, 2),
    std::make_tuple(6, 4),
    std::make_tuple(12, 29),
    std::make_tuple(12, 100),
    std::make_tuple(0, 0),
    std::make_tuple(80, 86)
));


/******************************************************************************/
/* Tests for IRArgoACBase (comon functionality across WREM2 and WREM3)        */
/******************************************************************************/

using IRArgoACBase_typelist = ::testing::Types<ArgoProtocol, ArgoProtocolWREM3>;
template<class> struct TestArgoACBaseClass : public testing::Test {};
TYPED_TEST_CASE(TestArgoACBaseClass, IRArgoACBase_typelist);


TYPED_TEST(TestArgoACBaseClass, SetAndGetTemp) {
  IRArgoACBase<TypeParam> ac(kGpioUnused);

  ac.setTemp(25);
  EXPECT_EQ(25, ac.getTemp());
  ac.setTemp(kArgoMinTemp);
  EXPECT_EQ(kArgoMinTemp, ac.getTemp());
  ac.setTemp(kArgoMaxTemp);
  EXPECT_EQ(kArgoMaxTemp, ac.getTemp());
  ac.setTemp(kArgoMinTemp - 1);
  EXPECT_EQ(kArgoMinTemp, ac.getTemp());
  ac.setTemp(kArgoMaxTemp + 1);
  EXPECT_EQ(kArgoMaxTemp, ac.getTemp());
}

TYPED_TEST(TestArgoACBaseClass, SetAndGetSensorTemp) {
  IRArgoACBase<TypeParam> ac(kGpioUnused);

  // Room Temp from AC command
  ac.setSensorTemp(25);
  EXPECT_EQ(25, ac.getSensorTemp());
  ac.setSensorTemp(kArgoTempDelta);
  EXPECT_EQ(kArgoTempDelta, ac.getSensorTemp());
  ac.setSensorTemp(kArgoMaxRoomTemp);
  EXPECT_EQ(kArgoMaxRoomTemp, ac.getSensorTemp());
  ac.setSensorTemp(kArgoTempDelta - 1);
  EXPECT_EQ(kArgoTempDelta, ac.getSensorTemp());
  ac.setSensorTemp(kArgoMaxRoomTemp + 1);
  EXPECT_EQ(kArgoMaxRoomTemp, ac.getSensorTemp());

  // Room temp from iFeel coommand
  ac.setMessageType(argoIrMessageType_t::IFEEL_TEMP_REPORT);  // reset
  EXPECT_EQ(kArgoTempDelta, ac.getSensorTemp());
  ac.setSensorTemp(19);
  EXPECT_EQ(19, ac.getSensorTemp());
  ac.setSensorTemp(kArgoTempDelta);
  EXPECT_EQ(kArgoTempDelta, ac.getSensorTemp());
  ac.setSensorTemp(kArgoMaxRoomTemp);
  EXPECT_EQ(kArgoMaxRoomTemp, ac.getSensorTemp());
  ac.setSensorTemp(kArgoTempDelta - 1);
  EXPECT_EQ(kArgoTempDelta, ac.getSensorTemp());
  ac.setSensorTemp(kArgoMaxRoomTemp + 1);
  EXPECT_EQ(kArgoMaxRoomTemp, ac.getSensorTemp());
}

TYPED_TEST(TestArgoACBaseClass, SetAndGetModeEx) {
  IRArgoACBase<TypeParam> ac(kGpioUnused);

  ac.setMode(argoMode_t::HEAT);
  EXPECT_EQ(argoMode_t::HEAT, ac.getModeEx());
  ac.setMode(argoMode_t::AUTO);
  EXPECT_EQ(argoMode_t::AUTO, ac.getModeEx());
  ac.setMode(argoMode_t::COOL);
  EXPECT_EQ(argoMode_t::COOL, ac.getModeEx());
  ac.setMode(argoMode_t::DRY);
  EXPECT_EQ(argoMode_t::DRY, ac.getModeEx());
  ac.setMode(argoMode_t::FAN);
  EXPECT_EQ(argoMode_t::FAN, ac.getModeEx());
  ac.setMode(static_cast<argoMode_t>(255));
  EXPECT_EQ(argoMode_t::AUTO, ac.getModeEx());
}

TYPED_TEST(TestArgoACBaseClass, SetAndGetFanEx) {
  IRArgoACBase<TypeParam> ac(kGpioUnused);

  ac.setFan(argoFan_t::FAN_AUTO);
  EXPECT_EQ(argoFan_t::FAN_AUTO, ac.getFanEx());
  ac.setFan(argoFan_t::FAN_HIGHEST);
  EXPECT_EQ(argoFan_t::FAN_HIGHEST, ac.getFanEx());
  if (std::is_same<TypeParam, ArgoProtocolWREM3>()) {
    // Only supported on WREM3
    ac.setFan(argoFan_t::FAN_HIGH);
    EXPECT_EQ(argoFan_t::FAN_HIGH, ac.getFanEx());
  }
  ac.setFan(argoFan_t::FAN_MEDIUM);
  EXPECT_EQ(argoFan_t::FAN_MEDIUM, ac.getFanEx());
  if (std::is_same<TypeParam, ArgoProtocolWREM3>()) {
    // Only supported on WREM3
    ac.setFan(argoFan_t::FAN_LOW);
    EXPECT_EQ(argoFan_t::FAN_LOW, ac.getFanEx());
    ac.setFan(argoFan_t::FAN_LOWER);
    EXPECT_EQ(argoFan_t::FAN_LOWER, ac.getFanEx());
  }
  ac.setFan(argoFan_t::FAN_LOWEST);
  EXPECT_EQ(argoFan_t::FAN_LOWEST, ac.getFanEx());
  ac.setFan(static_cast<argoFan_t>(255));
  EXPECT_EQ(argoFan_t::FAN_AUTO, ac.getFanEx());
}

TYPED_TEST(TestArgoACBaseClass, SetAndGetFlapEx) {
  IRArgoACBase<TypeParam> ac(kGpioUnused);

  ac.setFlap(argoFlap_t::FLAP_FULL);
  EXPECT_EQ(argoFlap_t::FLAP_FULL, ac.getFlapEx());
  ac.setFlap(argoFlap_t::FLAP_AUTO);
  EXPECT_EQ(argoFlap_t::FLAP_AUTO, ac.getFlapEx());
  ac.setFlap(argoFlap_t::FLAP_6);
  EXPECT_EQ(argoFlap_t::FLAP_6, ac.getFlapEx());
  ac.setFlap(argoFlap_t::FLAP_5);
  EXPECT_EQ(argoFlap_t::FLAP_5, ac.getFlapEx());
  ac.setFlap(argoFlap_t::FLAP_4);
  EXPECT_EQ(argoFlap_t::FLAP_4, ac.getFlapEx());
  ac.setFlap(argoFlap_t::FLAP_3);
  EXPECT_EQ(argoFlap_t::FLAP_3, ac.getFlapEx());
  ac.setFlap(argoFlap_t::FLAP_2);
  EXPECT_EQ(argoFlap_t::FLAP_2, ac.getFlapEx());
  ac.setFlap(argoFlap_t::FLAP_1);
  EXPECT_EQ(argoFlap_t::FLAP_1, ac.getFlapEx());
  ac.setFlap(argoFlap_t::FLAP_FULL);
  ac.setFlap(static_cast<argoFlap_t>(255));
  EXPECT_EQ(argoFlap_t::FLAP_AUTO, ac.getFlapEx());
}

TYPED_TEST(TestArgoACBaseClass, Night) {
  IRArgoACBase<TypeParam> ac(kGpioUnused);

  ac.setNight(false);
  ASSERT_FALSE(ac.getNight());
  ac.setNight(true);
  ASSERT_TRUE(ac.getNight());
  ac.setNight(false);
  ASSERT_FALSE(ac.getNight());
}

TYPED_TEST(TestArgoACBaseClass, iFeel) {
  IRArgoACBase<TypeParam> ac(kGpioUnused);

  ac.setiFeel(false);
  ASSERT_FALSE(ac.getiFeel());
  ac.setiFeel(true);
  ASSERT_TRUE(ac.getiFeel());
  ac.setiFeel(false);
  ASSERT_FALSE(ac.getiFeel());
}

TYPED_TEST(TestArgoACBaseClass, Power) {
  IRArgoACBase<TypeParam> ac(kGpioUnused);

  ac.setPower(false);
  ASSERT_FALSE(ac.getPower());
  ac.setPower(true);
  ASSERT_TRUE(ac.getPower());
  ac.setPower(false);
  ASSERT_FALSE(ac.getPower());
}

TYPED_TEST(TestArgoACBaseClass, OnOff) {
  IRArgoACBase<TypeParam> ac(kGpioUnused);

  ASSERT_FALSE(ac.getPower());
  ac.on();
  ASSERT_TRUE(ac.getPower());
  ac.off();
  ASSERT_FALSE(ac.getPower());
}

TYPED_TEST(TestArgoACBaseClass, Max) {
  IRArgoACBase<TypeParam> ac(kGpioUnused);
  ac.setMax(false);
  ASSERT_FALSE(ac.getMax());
  ac.setMax(true);
  ASSERT_TRUE(ac.getMax());
  ac.setMax(false);
  ASSERT_FALSE(ac.getMax());
}

TYPED_TEST(TestArgoACBaseClass, SetAndGetMessageType) {
  IRArgoACBase<TypeParam> ac(kGpioUnused);

  ac.setMessageType(argoIrMessageType_t::AC_CONTROL);
  EXPECT_EQ(argoIrMessageType_t::AC_CONTROL, ac.getMessageType());
  ac.setMessageType(argoIrMessageType_t::CONFIG_PARAM_SET);
  EXPECT_EQ(argoIrMessageType_t::CONFIG_PARAM_SET, ac.getMessageType());
  ac.setMessageType(argoIrMessageType_t::IFEEL_TEMP_REPORT);
  EXPECT_EQ(argoIrMessageType_t::IFEEL_TEMP_REPORT, ac.getMessageType());
  ac.setMessageType(argoIrMessageType_t::TIMER_COMMAND);
  EXPECT_EQ(argoIrMessageType_t::TIMER_COMMAND, ac.getMessageType());
}

TYPED_TEST(TestArgoACBaseClass, SetMessageTypeResetsState) {
  IRArgoACBase<TypeParam> ac(kGpioUnused);

  ac.on();
  ac.setTemp(30);
  ac.setSensorTemp(33);
  ac.setMode(argoMode_t::COOL);
  ac.setFan(argoFan_t::FAN_HIGHEST);

  ac.setMessageType(argoIrMessageType_t::AC_CONTROL);
  EXPECT_EQ(argoIrMessageType_t::AC_CONTROL, ac.getMessageType());
  EXPECT_FALSE(ac.getPower());
  EXPECT_EQ(20, ac.getTemp());
  EXPECT_EQ(25, ac.getSensorTemp());
  EXPECT_EQ(argoMode_t::AUTO, ac.getModeEx());
  EXPECT_EQ(argoFan_t::FAN_AUTO, ac.getFanEx());

  ac.setMessageType(argoIrMessageType_t::IFEEL_TEMP_REPORT);
  EXPECT_EQ(argoIrMessageType_t::IFEEL_TEMP_REPORT, ac.getMessageType());
  EXPECT_EQ(kArgoTempDelta, ac.getSensorTemp());
}

TYPED_TEST(TestArgoACBaseClass, staticGetMessageType) {
  IRArgoACBase<TypeParam> ac(kGpioUnused);

  ac.setMessageType(argoIrMessageType_t::AC_CONTROL);
  EXPECT_EQ(argoIrMessageType_t::AC_CONTROL,
            IRArgoACBase<TypeParam>::getMessageType(ac.getRaw(),
                                                    ac.getRawByteLength()));

  ac.setMessageType(argoIrMessageType_t::IFEEL_TEMP_REPORT);
  EXPECT_EQ(argoIrMessageType_t::IFEEL_TEMP_REPORT,
            IRArgoACBase<TypeParam>::getMessageType(ac.getRaw(),
                                                    ac.getRawByteLength()));

  if (std::is_same<TypeParam, ArgoProtocolWREM3>()) {
    // Only supported for WREM3
    ac.setMessageType(argoIrMessageType_t::CONFIG_PARAM_SET);
    EXPECT_EQ(argoIrMessageType_t::CONFIG_PARAM_SET,
              IRArgoACBase<TypeParam>::getMessageType(ac.getRaw(),
                                                      ac.getRawByteLength()));

    ac.setMessageType(argoIrMessageType_t::TIMER_COMMAND);
    EXPECT_EQ(argoIrMessageType_t::TIMER_COMMAND,
              IRArgoACBase<TypeParam>::getMessageType(ac.getRaw(),
                                                      ac.getRawByteLength()));
  }
}

TYPED_TEST(TestArgoACBaseClass, staticgetStateLengthForIrMsgType) {
  if (std::is_same<TypeParam, ArgoProtocol>()) {
    EXPECT_EQ(kArgoStateLength,
              IRArgoACBase<TypeParam>::getStateLengthForIrMsgType(
                argoIrMessageType_t::AC_CONTROL));
    EXPECT_EQ(kArgoStateLength,
              IRArgoACBase<TypeParam>::getStateLengthForIrMsgType(
                argoIrMessageType_t::TIMER_COMMAND));
    EXPECT_EQ(0,
              IRArgoACBase<TypeParam>::getStateLengthForIrMsgType(
                argoIrMessageType_t::CONFIG_PARAM_SET));
    EXPECT_EQ(kArgoShortStateLength,
              IRArgoACBase<TypeParam>::getStateLengthForIrMsgType(
                argoIrMessageType_t::IFEEL_TEMP_REPORT));
  } else {
    EXPECT_EQ(kArgo3AcControlStateLength,
              IRArgoACBase<TypeParam>::getStateLengthForIrMsgType(
                argoIrMessageType_t::AC_CONTROL));
    EXPECT_EQ(kArgo3TimerStateLength,
              IRArgoACBase<TypeParam>::getStateLengthForIrMsgType(
                argoIrMessageType_t::TIMER_COMMAND));
    EXPECT_EQ(kArgo3ConfigStateLength,
              IRArgoACBase<TypeParam>::getStateLengthForIrMsgType(
                argoIrMessageType_t::CONFIG_PARAM_SET));
    EXPECT_EQ(kArgo3iFeelReportStateLength,
              IRArgoACBase<TypeParam>::getStateLengthForIrMsgType(
                argoIrMessageType_t::IFEEL_TEMP_REPORT));
  }
}

TYPED_TEST(TestArgoACBaseClass, setRaw) {
  TypeParam rawStateAC = {};
  rawStateAC.RoomTemp = 30;

  TypeParam rawStateIFeel = {};
  rawStateIFeel.SensorT = 25;

  IRArgoACBase<TypeParam> ac(kGpioUnused);

  if (std::is_same<TypeParam, ArgoProtocol>()) {
    ac.setRaw(reinterpret_cast<uint8_t*>(&rawStateAC), std::min(
      static_cast<size_t>(kArgoStateLength), sizeof(TypeParam)));
    EXPECT_EQ(30 + kArgoTempDelta, ac.getSensorTemp());
    EXPECT_EQ(argoIrMessageType_t::AC_CONTROL, ac.getMessageType());

    ac.setRaw(reinterpret_cast<uint8_t*>(&rawStateIFeel), std::min(
      static_cast<size_t>(kArgoShortStateLength), sizeof(TypeParam)));
    EXPECT_EQ(25 + kArgoTempDelta, ac.getSensorTemp());
    EXPECT_EQ(argoIrMessageType_t::IFEEL_TEMP_REPORT, ac.getMessageType());
  } else {
    ac.setRaw(reinterpret_cast<uint8_t*>(&rawStateAC), std::min(
      static_cast<size_t>(kArgo3AcControlStateLength), sizeof(TypeParam)));
    EXPECT_EQ(30 + kArgoTempDelta, ac.getSensorTemp());
    EXPECT_EQ(argoIrMessageType_t::AC_CONTROL, ac.getMessageType());

    auto raw = reinterpret_cast<uint8_t*>(&rawStateIFeel);
    raw[0] = 0x4B;  // sets Byte0::IrCommandType to IFeel (0b01)
    ac.setRaw(raw, std::min(static_cast<size_t>(kArgo3iFeelReportStateLength),
                            sizeof(TypeParam)));
    EXPECT_EQ(25 + kArgoTempDelta, ac.getSensorTemp());
    EXPECT_EQ(argoIrMessageType_t::IFEEL_TEMP_REPORT, ac.getMessageType());
  }
}

/******************************************************************************/
/* Backward-compatibility tests of legacy IRArgoAc raw methods vs. base class */
/******************************************************************************/

/// @brief Tests interactions of raw setFan() method
///        with a base-class getFanEx()
TEST(TestArgoACClass, SetAndGetFanEx) {
  IRArgoAC ac(kGpioUnused);

  ac.setFan(kArgoFan3);
  EXPECT_EQ(kArgoFan3, ac.getFan());
  EXPECT_EQ(argoFan_t::FAN_HIGHEST, ac.getFanEx());
  ac.setFan(kArgoFan1);
  EXPECT_EQ(kArgoFan1, ac.getFan());
  EXPECT_EQ(argoFan_t::FAN_LOWEST, ac.getFanEx());
  ac.setFan(kArgoFanAuto);
  EXPECT_EQ(kArgoFanAuto, ac.getFan());
  EXPECT_EQ(argoFan_t::FAN_AUTO, ac.getFanEx());
  ac.setFan(kArgoFan2);
  EXPECT_EQ(kArgoFan2, ac.getFan());
  EXPECT_EQ(argoFan_t::FAN_MEDIUM, ac.getFanEx());

  ASSERT_NE(7, kArgoFan3);
  // Now try some unexpected value.
  ac.setFan(7);
  EXPECT_EQ(kArgoFan3, ac.getFan());
}

/// @brief Tests interactions of base-class setFan() method
///        with a raw getFan()
TEST(TestArgoACClass, SetFanExAndGetFan) {
  IRArgoAC ac(kGpioUnused);

  ac.setFan(argoFan_t::FAN_AUTO);
  EXPECT_EQ(kArgoFanAuto, ac.getFan());

  ac.setFan(argoFan_t::FAN_HIGHEST);
  EXPECT_EQ(kArgoFan3, ac.getFan());
  ac.setFan(argoFan_t::FAN_HIGH);
  EXPECT_EQ(kArgoFan3, ac.getFan());

  ac.setFan(argoFan_t::FAN_MEDIUM);
  EXPECT_EQ(kArgoFan2, ac.getFan());
  ac.setFan(argoFan_t::FAN_LOW);
  EXPECT_EQ(kArgoFan2, ac.getFan());

  ac.setFan(argoFan_t::FAN_LOWER);
  EXPECT_EQ(kArgoFan1, ac.getFan());
  ac.setFan(argoFan_t::FAN_LOWEST);
  EXPECT_EQ(kArgoFan1, ac.getFan());

  ac.setFan(static_cast<argoFan_t>(255));
  EXPECT_EQ(kArgoFanAuto, ac.getFan());
}

TEST(TestArgoACClass, SetFlapGetFlap) {
  IRArgoAC ac(kGpioUnused);

  ac.setFlap(kArgoFlapFull);
  EXPECT_EQ(kArgoFlapFull, ac.getFlap());
  ac.setFlap(kArgoFlapAuto);
  EXPECT_EQ(kArgoFlapAuto, ac.getFlap());
  ac.setFlap(kArgoFlap1);
  EXPECT_EQ(kArgoFlap1, ac.getFlap());
  ac.setFlap(kArgoFlap2);
  EXPECT_EQ(kArgoFlap2, ac.getFlap());
  ac.setFlap(kArgoFlap3);
  EXPECT_EQ(kArgoFlap3, ac.getFlap());
  ac.setFlap(kArgoFlap4);
  EXPECT_EQ(kArgoFlap4, ac.getFlap());
  ac.setFlap(kArgoFlap5);
  EXPECT_EQ(kArgoFlap5, ac.getFlap());
  ac.setFlap(kArgoFlap6);
  EXPECT_EQ(kArgoFlap6, ac.getFlap());
}

/// @brief Tests interactions of raw setMode() method
///        with a base-class getModeEx()
TEST(TestArgoACClass, SetModeAndGetModeEx) {
  IRArgoAC ac(kGpioUnused);

  ac.setMode(kArgoHeat);
  EXPECT_EQ(kArgoHeat, ac.getMode());
  EXPECT_EQ(argoMode_t::HEAT, ac.getModeEx());
  ac.setMode(kArgoCool);
  EXPECT_EQ(kArgoCool, ac.getMode());
  EXPECT_EQ(argoMode_t::COOL, ac.getModeEx());
  ac.setMode(kArgoDry);
  EXPECT_EQ(kArgoDry, ac.getMode());
  EXPECT_EQ(argoMode_t::DRY, ac.getModeEx());
  ac.setMode(kArgoAuto);
  EXPECT_EQ(kArgoAuto, ac.getMode());
  EXPECT_EQ(argoMode_t::AUTO, ac.getModeEx());
  ac.setMode(kArgoHeatAuto);
  EXPECT_EQ(kArgoHeatAuto, ac.getMode());
  EXPECT_EQ(argoMode_t::AUTO, ac.getModeEx());
  ac.setMode(kArgoOff);
  EXPECT_EQ(kArgoOff, ac.getMode());
  EXPECT_EQ(argoMode_t::FAN, ac.getModeEx());
  ac.setMode(255);
  EXPECT_EQ(kArgoAuto, ac.getMode());
  EXPECT_EQ(argoMode_t::AUTO, ac.getModeEx());
}

/// @brief Tests interactions of base-class setMode() method
///        with a raw getMode()
TEST(TestArgoACClass, SetModeExAndGetMode) {
  IRArgoAC ac(kGpioUnused);

  ac.setMode(argoMode_t::HEAT);
  EXPECT_EQ(kArgoHeat, ac.getMode());
  EXPECT_EQ(argoMode_t::HEAT, ac.getModeEx());
  ac.setMode(argoMode_t::AUTO);
  EXPECT_EQ(kArgoAuto, ac.getMode());
  EXPECT_EQ(argoMode_t::AUTO, ac.getModeEx());
  ac.setMode(argoMode_t::COOL);
  EXPECT_EQ(kArgoCool, ac.getMode());
  EXPECT_EQ(argoMode_t::COOL, ac.getModeEx());
  ac.setMode(argoMode_t::DRY);
  EXPECT_EQ(kArgoDry, ac.getMode());
  EXPECT_EQ(argoMode_t::DRY, ac.getModeEx());
  ac.setMode(argoMode_t::FAN);
  EXPECT_EQ(kArgoOff, ac.getMode());  // Fan is N/A (?) -> defaults to off
  EXPECT_EQ(argoMode_t::FAN, ac.getModeEx());
  ac.setMode(static_cast<argoMode_t>(kArgoHeatBlink));
  EXPECT_EQ(kArgoHeatBlink, ac.getMode());
  EXPECT_EQ(static_cast<argoMode_t>(kArgoHeatBlink), ac.getModeEx());
  ac.setMode(static_cast<argoMode_t>(255));
  EXPECT_EQ(kArgoAuto, ac.getMode());
  EXPECT_EQ(argoMode_t::AUTO, ac.getModeEx());
}

TEST(TestArgoACClass, SendSensorTemp) {
  IRrecv irrecv(kGpioUnused);

  // Method 1 (via sendSensorTemp())
  IRArgoAC ac(kGpioUnused);
  ac.sendSensorTemp(10);
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&ac._irsend.capture));
  EXPECT_EQ(decode_type_t::ARGO, ac._irsend.capture.decode_type);
  EXPECT_EQ("Model: 1 (WREM2), Sensor Temp: 10C",
      IRAcUtils::resultAcToString(&ac._irsend.capture));

  // Method 2 (via send())
  IRArgoAC ac2(kGpioUnused);
  ac2.setMessageType(argoIrMessageType_t::IFEEL_TEMP_REPORT);
  ac2.setSensorTemp(19);
  ac2.send();
  ac2._irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&ac2._irsend.capture));
  EXPECT_EQ(decode_type_t::ARGO, ac2._irsend.capture.decode_type);
  EXPECT_EQ("Model: 1 (WREM2), Sensor Temp: 19C",
      IRAcUtils::resultAcToString(&ac2._irsend.capture));
}

/******************************************************************************/
/* IRArgoAC_WREM3-specific tests                                              */
/******************************************************************************/

TEST(TestArgoAC_WREM3Class, Eco) {
  IRArgoAC_WREM3 ac(kGpioUnused);

  ac.setEco(false);
  ASSERT_FALSE(ac.getEco());
  ac.setEco(true);
  ASSERT_TRUE(ac.getEco());
  ac.setEco(false);
  ASSERT_FALSE(ac.getEco());
}

TEST(TestArgoAC_WREM3Class, Filter) {
  IRArgoAC_WREM3 ac(kGpioUnused);

  ac.setFilter(false);
  ASSERT_FALSE(ac.getFilter());
  ac.setFilter(true);
  ASSERT_TRUE(ac.getFilter());
  ac.setFilter(false);
  ASSERT_FALSE(ac.getFilter());
}

TEST(TestArgoAC_WREM3Class, Light) {
  IRArgoAC_WREM3 ac(kGpioUnused);

  ac.setLight(false);
  ASSERT_FALSE(ac.getLight());
  ac.setLight(true);
  ASSERT_TRUE(ac.getLight());
  ac.setLight(false);
  ASSERT_FALSE(ac.getLight());
}

TEST(TestArgoAC_WREM3Class, Channel) {
  IRArgoAC_WREM3 ac(kGpioUnused);

  ac.setChannel(0);
  ASSERT_EQ(0, ac.getChannel());
  ac.setChannel(1);
  ASSERT_EQ(1, ac.getChannel());
  ac.setChannel(2);
  ASSERT_EQ(2, ac.getChannel());
  ac.setChannel(3);
  ASSERT_EQ(3, ac.getChannel());
  ac.setChannel(4);
  ASSERT_EQ(3, ac.getChannel());
}

TEST(TestArgoAC_WREM3Class, ConfigEntry) {
  IRArgoAC_WREM3 ac(kGpioUnused);

  ac.setConfigEntry(0, 0);
  ASSERT_EQ(std::make_pair(static_cast<uint8_t>(0), static_cast<uint8_t>(0)),
            ac.getConfigEntry());
  ac.setConfigEntry(80, 86);
  ASSERT_EQ(std::make_pair(static_cast<uint8_t>(80), static_cast<uint8_t>(86)),
            ac.getConfigEntry());
  ac.setConfigEntry(255, 255);
  ASSERT_EQ(std::make_pair(static_cast<uint8_t>(255),
                           static_cast<uint8_t>(255)),
            ac.getConfigEntry());
}

TEST(TestArgoAC_WREM3Class, CurrentTimeMinutes) {
  IRArgoAC_WREM3 ac(kGpioUnused);

  ac.setCurrentTimeMinutes(0);
  ASSERT_EQ(0, ac.getCurrentTimeMinutes());
  ac.setCurrentTimeMinutes(16*60+50);
  ASSERT_EQ(16*60+50, ac.getCurrentTimeMinutes());
  ac.setCurrentTimeMinutes(23*60+59);
  ASSERT_EQ(23*60+59, ac.getCurrentTimeMinutes());
  ac.setCurrentTimeMinutes(23*60+59+1);
  ASSERT_EQ(23*60+59, ac.getCurrentTimeMinutes());
}

TEST(TestArgoAC_WREM3Class, CurrentDayOfWeek) {
  IRArgoAC_WREM3 ac(kGpioUnused);

  ac.setCurrentDayOfWeek(argoWeekday::SUNDAY);
  ASSERT_EQ(argoWeekday::SUNDAY, ac.getCurrentDayOfWeek());
  ac.setCurrentDayOfWeek(argoWeekday::MONDAY);
  ASSERT_EQ(argoWeekday::MONDAY, ac.getCurrentDayOfWeek());
  ac.setCurrentDayOfWeek(argoWeekday::TUESDAY);
  ASSERT_EQ(argoWeekday::TUESDAY, ac.getCurrentDayOfWeek());
  ac.setCurrentDayOfWeek(argoWeekday::WEDNESDAY);
  ASSERT_EQ(argoWeekday::WEDNESDAY, ac.getCurrentDayOfWeek());
  ac.setCurrentDayOfWeek(argoWeekday::THURSDAY);
  ASSERT_EQ(argoWeekday::THURSDAY, ac.getCurrentDayOfWeek());
  ac.setCurrentDayOfWeek(argoWeekday::FRIDAY);
  ASSERT_EQ(argoWeekday::FRIDAY, ac.getCurrentDayOfWeek());
  ac.setCurrentDayOfWeek(argoWeekday::SATURDAY);
  ASSERT_EQ(argoWeekday::SATURDAY, ac.getCurrentDayOfWeek());
  ac.setCurrentDayOfWeek(static_cast<argoWeekday>(200));
  ASSERT_EQ(argoWeekday::SATURDAY, ac.getCurrentDayOfWeek());
}

TEST(TestArgoAC_WREM3Class, TimerType) {
  IRArgoAC_WREM3 ac(kGpioUnused);

  ac.setTimerType(argoTimerType_t::NO_TIMER);
  ASSERT_EQ(argoTimerType_t::NO_TIMER, ac.getTimerType());
  ac.setTimerType(argoTimerType_t::DELAY_TIMER);
  ASSERT_EQ(argoTimerType_t::DELAY_TIMER, ac.getTimerType());
  ac.setTimerType(argoTimerType_t::SCHEDULE_TIMER_1);
  ASSERT_EQ(argoTimerType_t::SCHEDULE_TIMER_1, ac.getTimerType());
  ac.setTimerType(argoTimerType_t::SCHEDULE_TIMER_2);
  ASSERT_EQ(argoTimerType_t::SCHEDULE_TIMER_2, ac.getTimerType());
  ac.setTimerType(argoTimerType_t::SCHEDULE_TIMER_3);
  ASSERT_EQ(argoTimerType_t::SCHEDULE_TIMER_3, ac.getTimerType());
  ac.setTimerType(static_cast<argoTimerType_t>(201));
  ASSERT_EQ(argoTimerType_t::NO_TIMER, ac.getTimerType());
}

TEST(TestArgoAC_WREM3Class, DelayTimerMinutes) {
  IRArgoAC_WREM3 ac(kGpioUnused);

  ac.setDelayTimerMinutes(0);
  ASSERT_EQ(0, ac.getDelayTimerMinutes());
  ac.setDelayTimerMinutes(16*60+50);
  ASSERT_EQ(16*60+50, ac.getDelayTimerMinutes());
  ac.setDelayTimerMinutes(16*60+54);
  ASSERT_EQ(16*60+50, ac.getDelayTimerMinutes());
  ac.setDelayTimerMinutes(16*60+55);
  ASSERT_EQ(16*60+60, ac.getDelayTimerMinutes());
  ac.setDelayTimerMinutes(19*60+44);
  ASSERT_EQ(19*60+40, ac.getDelayTimerMinutes());
  ac.setDelayTimerMinutes(19*60+50);
  ASSERT_EQ(19*60+50, ac.getDelayTimerMinutes());
  ac.setDelayTimerMinutes(19*60+60);
  ASSERT_EQ(19*60+50, ac.getDelayTimerMinutes());
  ac.setDelayTimerMinutes(23*60+59);  // Above max (19h50m)
  ASSERT_EQ(19*60+50, ac.getDelayTimerMinutes());
  ac.setDelayTimerMinutes(23*60+59+1);
  ASSERT_EQ(19*60+50, ac.getDelayTimerMinutes());
}

TEST(TestArgoAC_WREM3Class, ScheduleTimerStartMinutes) {
  IRArgoAC_WREM3 ac(kGpioUnused);

  ac.setScheduleTimerStartMinutes(0);
  ASSERT_EQ(0, ac.getScheduleTimerStartMinutes());
  ac.setScheduleTimerStartMinutes(16*60+50);
  ASSERT_EQ(16*60+50, ac.getScheduleTimerStartMinutes());
  ac.setScheduleTimerStartMinutes(16*60+54);
  ASSERT_EQ(16*60+50, ac.getScheduleTimerStartMinutes());
  ac.setScheduleTimerStartMinutes(16*60+55);
  ASSERT_EQ(16*60+60, ac.getScheduleTimerStartMinutes());
  ac.setScheduleTimerStartMinutes(23*60+50);
  ASSERT_EQ(23*60+50, ac.getScheduleTimerStartMinutes());
  ac.setScheduleTimerStartMinutes(23*60+59);  // Above max (23h50m)
  ASSERT_EQ(23*60+50, ac.getScheduleTimerStartMinutes());
  ac.setScheduleTimerStartMinutes(23*60+59+1);
  ASSERT_EQ(23*60+50, ac.getScheduleTimerStartMinutes());
}

TEST(TestArgoAC_WREM3Class, ScheduleTimerStopMinutes) {
  IRArgoAC_WREM3 ac(kGpioUnused);

  ac.setScheduleTimerStopMinutes(0);
  ASSERT_EQ(0, ac.getScheduleTimerStopMinutes());
  ac.setScheduleTimerStopMinutes(16*60+50);
  ASSERT_EQ(16*60+50, ac.getScheduleTimerStopMinutes());
  ac.setScheduleTimerStopMinutes(16*60+54);
  ASSERT_EQ(16*60+50, ac.getScheduleTimerStopMinutes());
  ac.setScheduleTimerStopMinutes(16*60+55);
  ASSERT_EQ(16*60+60, ac.getScheduleTimerStopMinutes());
  ac.setScheduleTimerStopMinutes(23*60+50);
  ASSERT_EQ(23*60+50, ac.getScheduleTimerStopMinutes());
  ac.setScheduleTimerStopMinutes(23*60+59);  // Above max (23h50m)
  ASSERT_EQ(23*60+50, ac.getScheduleTimerStopMinutes());
  ac.setScheduleTimerStopMinutes(23*60+59+1);
  ASSERT_EQ(23*60+50, ac.getScheduleTimerStopMinutes());
}

TEST(TestArgoAC_WREM3Class, ScheduleTimerActiveDays) {
  IRArgoAC_WREM3 ac(kGpioUnused);

  ac.setScheduleTimerActiveDays(std::set<argoWeekday>({}));
  EXPECT_THAT(ac.getScheduleTimerActiveDays(),
              ::testing::IsEmpty());
  EXPECT_EQ(0b0000000, ac.getTimerActiveDaysBitmap());

  ac.setScheduleTimerActiveDays(std::set<argoWeekday>({argoWeekday::SUNDAY}));
  EXPECT_THAT(ac.getScheduleTimerActiveDays(),
              ::testing::ElementsAre(argoWeekday::SUNDAY));
  EXPECT_EQ(0b0000001, ac.getTimerActiveDaysBitmap());

  ac.setScheduleTimerActiveDays(std::set<argoWeekday>({argoWeekday::MONDAY}));
  EXPECT_THAT(ac.getScheduleTimerActiveDays(),
              ::testing::ElementsAre(argoWeekday::MONDAY));
  EXPECT_EQ(0b0000010, ac.getTimerActiveDaysBitmap());

  ac.setScheduleTimerActiveDays(std::set<argoWeekday>({argoWeekday::TUESDAY}));
  EXPECT_THAT(ac.getScheduleTimerActiveDays(),
              ::testing::ElementsAre(argoWeekday::TUESDAY));
  EXPECT_EQ(0b0000100, ac.getTimerActiveDaysBitmap());

  ac.setScheduleTimerActiveDays(std::set<argoWeekday>({
    argoWeekday::WEDNESDAY}));
  EXPECT_THAT(ac.getScheduleTimerActiveDays(),
              ::testing::ElementsAre(argoWeekday::WEDNESDAY));
  EXPECT_EQ(0b0001000, ac.getTimerActiveDaysBitmap());

  ac.setScheduleTimerActiveDays(std::set<argoWeekday>({argoWeekday::THURSDAY}));
  EXPECT_THAT(ac.getScheduleTimerActiveDays(),
              ::testing::ElementsAre(argoWeekday::THURSDAY));
  EXPECT_EQ(0b0010000, ac.getTimerActiveDaysBitmap());

  ac.setScheduleTimerActiveDays(std::set<argoWeekday>({argoWeekday::FRIDAY}));
  EXPECT_THAT(ac.getScheduleTimerActiveDays(),
              ::testing::ElementsAre(argoWeekday::FRIDAY));
  EXPECT_EQ(0b0100000, ac.getTimerActiveDaysBitmap());

  ac.setScheduleTimerActiveDays(std::set<argoWeekday>({argoWeekday::SATURDAY}));
  EXPECT_THAT(ac.getScheduleTimerActiveDays(),
              ::testing::ElementsAre(argoWeekday::SATURDAY));
  EXPECT_EQ(0b1000000, ac.getTimerActiveDaysBitmap());

  ac.setScheduleTimerActiveDays(std::set<argoWeekday>({
    argoWeekday::MONDAY, argoWeekday::TUESDAY, argoWeekday::WEDNESDAY,
    argoWeekday::THURSDAY, argoWeekday::FRIDAY, argoWeekday::SATURDAY,
    argoWeekday::SUNDAY}));
  EXPECT_THAT(ac.getScheduleTimerActiveDays(), ::testing::ElementsAre(
    argoWeekday::SUNDAY, argoWeekday::MONDAY, argoWeekday::TUESDAY,
    argoWeekday::WEDNESDAY, argoWeekday::THURSDAY, argoWeekday::FRIDAY,
    argoWeekday::SATURDAY));
  EXPECT_EQ(0b1111111, ac.getTimerActiveDaysBitmap());

  ac.setScheduleTimerActiveDays(std::set<argoWeekday>({
    argoWeekday::MONDAY, argoWeekday::TUESDAY, argoWeekday::WEDNESDAY,
    argoWeekday::THURSDAY, argoWeekday::FRIDAY}));
  EXPECT_THAT(ac.getScheduleTimerActiveDays(), ::testing::ElementsAre(
    argoWeekday::MONDAY, argoWeekday::TUESDAY, argoWeekday::WEDNESDAY,
    argoWeekday::THURSDAY, argoWeekday::FRIDAY));
  EXPECT_EQ(0b0111110, ac.getTimerActiveDaysBitmap());

  ac.setScheduleTimerActiveDays(std::set<argoWeekday>({ argoWeekday::TUESDAY,
    argoWeekday::THURSDAY, argoWeekday::SATURDAY, argoWeekday::SUNDAY}));
  EXPECT_THAT(ac.getScheduleTimerActiveDays(), ::testing::ElementsAre(
    argoWeekday::SUNDAY, argoWeekday::TUESDAY, argoWeekday::THURSDAY,
    argoWeekday::SATURDAY));
  EXPECT_EQ(0b1010101, ac.getTimerActiveDaysBitmap());
}

TEST(TestArgoAC_WREM3Class, staticGetMessageTypeFromRaw) {
  ArgoProtocolWREM3 raw = {};

  raw.IrCommandType = static_cast<uint8_t>(
    argoIrMessageType_t::AC_CONTROL);
  EXPECT_EQ(argoIrMessageType_t::AC_CONTROL,
            IRArgoAC_WREM3::getMessageType(raw));

  raw.IrCommandType = static_cast<uint8_t>(
    argoIrMessageType_t::CONFIG_PARAM_SET);
  EXPECT_EQ(argoIrMessageType_t::CONFIG_PARAM_SET,
            IRArgoAC_WREM3::getMessageType(raw));

  raw.IrCommandType = static_cast<uint8_t>(
    argoIrMessageType_t::IFEEL_TEMP_REPORT);
  EXPECT_EQ(argoIrMessageType_t::IFEEL_TEMP_REPORT,
            IRArgoAC_WREM3::getMessageType(raw));

  raw.IrCommandType = static_cast<uint8_t>(
    argoIrMessageType_t::TIMER_COMMAND);
  EXPECT_EQ(argoIrMessageType_t::TIMER_COMMAND,
            IRArgoAC_WREM3::getMessageType(raw));
}

TEST(TestArgoAC_WREM3Class, HasValidPreamble) {
  uint8_t preamble[] = { 0x4B, 0x57 };
  ASSERT_TRUE(IRArgoAC_WREM3::hasValidPreamble(preamble,
               sizeof(preamble) / sizeof(preamble[0])));
  ASSERT_FALSE(IRArgoAC_WREM3::hasValidPreamble(preamble, 0));
  ASSERT_TRUE(IRArgoAC_WREM3::hasValidPreamble(preamble, 1));

  preamble[0] = 0b00001011;
  ASSERT_TRUE(IRArgoAC_WREM3::hasValidPreamble(preamble,
               sizeof(preamble) / sizeof(preamble[0])));
  preamble[0] = 0b11111011;
  ASSERT_TRUE(IRArgoAC_WREM3::hasValidPreamble(preamble,
               sizeof(preamble) / sizeof(preamble[0])));
  preamble[0] = 0b00001010;
  ASSERT_FALSE(IRArgoAC_WREM3::hasValidPreamble(preamble,
               sizeof(preamble) / sizeof(preamble[0])));
  preamble[0] = 0b00000011;
  ASSERT_FALSE(IRArgoAC_WREM3::hasValidPreamble(preamble,
               sizeof(preamble) / sizeof(preamble[0])));
}

TEST(TestArgoAC_WREM3Class, IsValidWrem3Message) {
  uint8_t wrem3AC[] = { 0x0B, 0x36, 0x12, 0x0F, 0xC2, 0x24 };
  uint8_t wrem3IFeel[] = { 0x4B, 0x78 };
  uint8_t wrem3Config[] = { 0xCB, 0x0C, 0x4A, 0x21 };
  uint8_t wrem3Tmr[] = { 0x8B, 0x05, 0x4D, 0x98, 0xD2, 0x44, 0x2E, 0x34, 0xA7 };
  uint8_t wrem2IFeel[] = { 0xAC, 0xF5, 0xC2, 0x63 };

  ASSERT_TRUE(IRArgoAC_WREM3::isValidWrem3Message(wrem3AC,
               sizeof(wrem3AC) / sizeof(wrem3AC[0]) * 8, true));
  ASSERT_TRUE(IRArgoAC_WREM3::isValidWrem3Message(wrem3IFeel,
               sizeof(wrem3IFeel) / sizeof(wrem3IFeel[0]) * 8, true));
  ASSERT_TRUE(IRArgoAC_WREM3::isValidWrem3Message(wrem3Config,
               sizeof(wrem3Config) / sizeof(wrem3Config[0]) * 8, true));
  ASSERT_TRUE(IRArgoAC_WREM3::isValidWrem3Message(wrem3Tmr,
               sizeof(wrem3Tmr) / sizeof(wrem3Tmr[0]) * 8, true));
  ASSERT_FALSE(IRArgoAC_WREM3::isValidWrem3Message(wrem2IFeel,
               sizeof(wrem2IFeel) / sizeof(wrem2IFeel[0]) * 8, true));

  // 1 bit too short
  ASSERT_FALSE(IRArgoAC_WREM3::isValidWrem3Message(wrem3AC,
               sizeof(wrem3AC) / sizeof(wrem3AC[0]) * 8 - 1, true));

  // wrong checksum
  wrem3AC[5] ^= wrem3AC[5];
  ASSERT_FALSE(IRArgoAC_WREM3::isValidWrem3Message(wrem3AC,
               sizeof(wrem3AC) / sizeof(wrem3AC[0]) * 8, true));  // strict
  ASSERT_TRUE(IRArgoAC_WREM3::isValidWrem3Message(wrem3AC,
               sizeof(wrem3AC) / sizeof(wrem3AC[0]) * 8, false));  // lax
  wrem3AC[5] ^= wrem3AC[5];  // restore

  wrem3IFeel[1] ^= wrem3IFeel[1];
  ASSERT_FALSE(IRArgoAC_WREM3::isValidWrem3Message(wrem3IFeel,
               sizeof(wrem3IFeel) / sizeof(wrem3IFeel[0]) * 8, true));
  ASSERT_TRUE(IRArgoAC_WREM3::isValidWrem3Message(wrem3IFeel,
               sizeof(wrem3IFeel) / sizeof(wrem3IFeel[0]) * 8, false));
  wrem3IFeel[1] ^= wrem3IFeel[1];  // restore

  wrem3Config[3] ^= wrem3Config[3];
  ASSERT_FALSE(IRArgoAC_WREM3::isValidWrem3Message(wrem3Config,
               sizeof(wrem3Config) / sizeof(wrem3Config[0]) * 8, true));
  ASSERT_TRUE(IRArgoAC_WREM3::isValidWrem3Message(wrem3Config,
               sizeof(wrem3Config) / sizeof(wrem3Config[0]) * 8, false));
  wrem3Config[3] ^= wrem3Config[3];  // restore

  wrem3Tmr[8] ^= wrem3Tmr[8];
  ASSERT_FALSE(IRArgoAC_WREM3::isValidWrem3Message(wrem3Tmr,
               sizeof(wrem3Tmr) / sizeof(wrem3Tmr[0]) * 8, true));
  ASSERT_TRUE(IRArgoAC_WREM3::isValidWrem3Message(wrem3Tmr,
               sizeof(wrem3Tmr) / sizeof(wrem3Tmr[0]) * 8, false));
  wrem3Tmr[8] ^= wrem3Tmr[8];  // restore

  // wrong preamble
  wrem3IFeel[0] += 1;
  ASSERT_FALSE(IRArgoAC_WREM3::isValidWrem3Message(wrem3IFeel,
               sizeof(wrem3IFeel) / sizeof(wrem3IFeel[0]) * 8, true));
  ASSERT_FALSE(IRArgoAC_WREM3::isValidWrem3Message(wrem3IFeel,
               sizeof(wrem3IFeel) / sizeof(wrem3IFeel[0]) * 8, false));
  wrem3IFeel[0] -= 1;  // restore
}

TEST(TestArgoAC_WREM3Class, SendSensorTemp) {
  IRrecv irrecv(kGpioUnused);

  // Method 1 (via sendSensorTemp())
  IRArgoAC_WREM3 ac(kGpioUnused);
  ac.sendSensorTemp(10);
  ac._irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&ac._irsend.capture));
  EXPECT_EQ(decode_type_t::ARGO, ac._irsend.capture.decode_type);
  EXPECT_EQ("IFeel Report[CH#0]: Model: 2 (WREM3), Sensor Temp: 10C",
      IRAcUtils::resultAcToString(&ac._irsend.capture));

  // Method 2 (via send())
  IRArgoAC_WREM3 ac2(kGpioUnused);
  ac2.setMessageType(argoIrMessageType_t::IFEEL_TEMP_REPORT);
  ac2.setSensorTemp(19);
  ac2.send();
  ac2._irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&ac2._irsend.capture));
  EXPECT_EQ(decode_type_t::ARGO, ac2._irsend.capture.decode_type);
  EXPECT_EQ("IFeel Report[CH#0]: Model: 2 (WREM3), Sensor Temp: 19C",
      IRAcUtils::resultAcToString(&ac2._irsend.capture));
}

TEST(TestArgoAC_WREM3Class, NonExModeFlapFan) {
  IRArgoAC_WREM3 ac(kGpioUnused);

  ac.setFan(argoFan_t::FAN_HIGH);
  EXPECT_EQ(argoFan_t::FAN_HIGH, ac.getFan());
  EXPECT_EQ(argoFan_t::FAN_HIGH, ac.getFanEx());

  ac.setMode(argoMode_t::FAN);
  EXPECT_EQ(argoMode_t::FAN, ac.getMode());
  EXPECT_EQ(argoMode_t::FAN, ac.getModeEx());

  ac.setFlap(argoFlap_t::FLAP_4);
  EXPECT_EQ(argoFlap_t::FLAP_4, ac.getFlap());
  EXPECT_EQ(argoFlap_t::FLAP_4, ac.getFlapEx());
}


/******************************************************************************/
/* Housekeeping                                                               */
/******************************************************************************/

TEST(TestUtils, Housekeeping) {
  ASSERT_EQ("ARGO", typeToString(decode_type_t::ARGO));
  ASSERT_EQ(decode_type_t::ARGO, strToDecodeType("ARGO"));
  ASSERT_TRUE(hasACState(decode_type_t::ARGO));
  ASSERT_EQ(irutils::modelToStr(decode_type_t::ARGO,
                                argo_ac_remote_model_t::SAC_WREM2), "WREM2");
  ASSERT_EQ(irutils::modelToStr(decode_type_t::ARGO,
                                argo_ac_remote_model_t::SAC_WREM3), "WREM3");
}


/******************************************************************************/
/* Decode tests based on real (recorded) IR messages                          */
/******************************************************************************/
TEST(TestDecodeArgo, RealShortDecode) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  // Real short Argo message. (i.e. 32 bits)
  const uint16_t sensor_28C[67] = {
      6418, 3168, 444, 834, 444, 834, 442, 2112, 444, 2112, 444, 834, 442, 2114,
      442, 834, 442, 2112, 444, 2112, 444, 832, 442, 2114, 442, 834, 442, 2112,
      442, 2114, 442, 2112, 444, 2112, 442, 834, 442, 2112, 444, 834, 442, 834,
      442, 834, 442, 834, 442, 2114, 442, 2114, 442, 2112, 442, 2112, 442, 834,
      444, 834, 442, 834, 442, 2112, 442, 2112, 442, 836,
      442};  // UNKNOWN 6149090
  irsend.reset();
  uint8_t expectedState[kArgoShortStateLength] = {
      0xAC, 0xF5, 0xC2, 0x63};
  irsend.sendRaw(sensor_28C, 67, 38);
  irsend.makeDecodeResult();
  EXPECT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::ARGO, irsend.capture.decode_type);
  EXPECT_EQ(kArgoShortBits, irsend.capture.bits);
  EXPECT_STATE_EQ(expectedState, irsend.capture.state, irsend.capture.bits);
  EXPECT_EQ(
      "Model: 1 (WREM2), Sensor Temp: 28C",
      IRAcUtils::resultAcToString(&irsend.capture));
  stdAc::state_t r, p;
  // These short messages do result in a valid state (w/ room temperature only)
  EXPECT_TRUE(IRAcUtils::decodeToState(&irsend.capture, &r, &p));
  EXPECT_EQ(stdAc::ac_command_t::kSensorTempReport, r.command);
  EXPECT_EQ(28, r.sensorTemperature);
}

///
/// @brief Test Fixture for recorded tests
///
struct ArgoE2ETestParam {
  const std::vector<uint16_t> rawDataInput;
  const uint8_t expectedEncodedSizeBytes;
  const std::vector<uint8_t> expectedEncodedValue;
  const std::string expectedString;

  ArgoE2ETestParam(std::vector<uint16_t> _raw, uint8_t _encSize,
                   std::vector<uint8_t> _encValue, std::string _str)
    : rawDataInput(_raw), expectedEncodedSizeBytes(_encSize),
      expectedEncodedValue(_encValue), expectedString(_str) {}

  friend std::ostream& operator<<(std::ostream& os, const ArgoE2ETestParam& v) {
    return os << "rawDataInput: " << ::testing::PrintToString(v.rawDataInput)
      << "\n\texpectedEncodedSize: "
      << static_cast<int>(v.expectedEncodedSizeBytes)
      << "[B]" << "\n\texpectedEncodedValue: 0x"
      <<  bytesToHexString(v.expectedEncodedValue) << "\n\texpectedString: "
      << v.expectedString;
  }
};

///
/// @brief Test fixture for real-world recorded messages for WREM3
///
class TestArgoE2E : public ::testing::TestWithParam<ArgoE2ETestParam> {};

// Test code
TEST_P(TestArgoE2E, RealExampleCommands) {
  IRsendTest irsend(kGpioUnused);
  IRrecv irrecv(kGpioUnused);
  irsend.begin();

  irsend.reset();
  irsend.sendRaw(&GetParam().rawDataInput[0],
                 GetParam().rawDataInput.size(), kArgoFrequency);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decode(&irsend.capture));
  EXPECT_EQ(decode_type_t::ARGO, irsend.capture.decode_type);
  ASSERT_TRUE(IRArgoAC_WREM3::isValidWrem3Message(irsend.capture.state,
    irsend.capture.bits, true));

  EXPECT_EQ(GetParam().expectedEncodedSizeBytes * 8, irsend.capture.bits);

  std::vector<uint8_t> stateActual(irsend.capture.state, irsend.capture.state
    + GetParam().expectedEncodedSizeBytes);
  EXPECT_THAT(stateActual, ::testing::ElementsAreArray(
    GetParam().expectedEncodedValue));

  EXPECT_FALSE(irsend.capture.repeat);

  EXPECT_EQ(GetParam().expectedString,
            IRAcUtils::resultAcToString(&irsend.capture));
}

// Test cases
INSTANTIATE_TEST_CASE_P(
  TestDecodeArgo,
  TestArgoE2E,
  ::testing::Values(
    ArgoE2ETestParam(
      std::vector<uint16_t> {
        6468, 3150,  456, 2154,  428, 2152,  462, 874,  422, 2158,  424, 882,
        424, 880,  428, 876,  430, 874,  454, 850,  424, 2154,  460, 2150,
        430, 874,  422, 2156,  458, 2152,  430, 874,  420, 884,  454, 850,
        424, 2152,  462, 874,  422, 882,  424, 2154,  428, 876,  430, 874,
        476, 828,  478, 2098,  462, 2148,  424, 2156,  458, 2150,  430, 874,
        432, 872,  422, 882,  424, 880,  482, 822,  430, 2148,  454, 852,
        488, 816,  480, 826,  482, 852,  454, 2126,  424, 2154,  458, 876,
        454, 852,  454, 2124,  426, 880,  426, 878,  428, 2150,  486, 848,
        426, 878,  428
      },
      kArgo3AcControlStateLength,
      std::vector<uint8_t> { 0x0B, 0x36, 0x12, 0x0F, 0xC2, 0x24 },
      "Command[CH#0]: Model: 2 (WREM3), Power: On, Mode: 1 (Cool), Temp: 22C, "
      "Sensor Temp: 26C, Fan: 0 (Auto), Swing(V): 7 (Breeze), IFeel: Off, "
      "Night: Off, Econo: Off, Max: Off, Filter: Off, Light: On"),


    ArgoE2ETestParam(
      std::vector<uint16_t> {
        6460, 3150,  454, 2154,  426, 2152,  462, 842,  452, 2156,  424, 878,
        428, 874,  432, 2144,  456, 878,  428, 2148,  432, 870,  424, 2152,
        460, 842,  452, 2154,  426, 878,  428, 876,  430, 872,  422
      },
      kArgo3iFeelReportStateLength,
      std::vector<uint8_t> { 0x4B, 0x15 },
      "IFeel Report[CH#0]: Model: 2 (WREM3), Sensor Temp: 25C"),


    ArgoE2ETestParam(
      std::vector<uint16_t> {
        6434, 3222,  424, 2186,  424, 2156,  422, 882,  422, 2158,  430, 874,
        430, 876,  430, 2212,  400, 874,  430, 2150,  450, 2160,  428, 2152,
        428, 908,  396, 2184,  426, 878,  426, 2152,  426, 880,  448
      },
      kArgo3iFeelReportStateLength,
      std::vector<uint8_t> {0x4B, 0x57},
      "IFeel Report[CH#0]: Model: 2 (WREM3), Sensor Temp: 27C"),


    ArgoE2ETestParam(
      std::vector<uint16_t> {
        6468, 3154,  482, 2128,  452, 2128,  484, 820,  486, 2124,  456, 848,
        458, 848,  424, 880,  448, 2130,  482, 2126,  452, 852,  422, 2158,
        488, 816,  490, 814,  490, 814,  492, 812,  482, 822,  484, 2124,
        422, 882,  456, 2124,  488, 2122,  456, 848,  458, 846,  460, 2120,
        482, 822,  484, 822,  484, 820,  486, 818,  488, 2122,  458, 2122,
        490, 814,  492, 814,  492, 2118,  450, 854,  452, 2126,  486, 820,
        486, 818,  488, 2122,  458, 846,  458, 2120,  492, 2118,  430, 874,
        430, 876,  454, 2126,  484, 820,  486, 818,  478, 828,  488, 2120,
        426, 878,  460, 844,  430, 2148,  486, 2094,  454, 2154,  480, 826,
        480, 2128,  430, 876,  430, 872,  422, 882,  424, 882,  424, 2156,
        478, 826,  480, 2100,  458, 2152,  482, 822,  484, 822,  452, 2156,
        424, 2156,  478, 2104,  452, 852,  454, 852,  452, 2158,  478, 828,
        478, 2132,  426
      },
      kArgo3TimerStateLength,
      std::vector<uint8_t> {
        0x8B, 0x05, 0x4D, 0x98, 0xD2, 0x44, 0x2E, 0x34, 0xA7
      },
      "Timer[CH#0]: Model: 2 (WREM3), Power: On, Timer Mode: 2 (Schedule1), "
      "Clock: 20:32, Day: 0 (Sun), On Timer: 09:10, Off Timer: 17:50, "
      "TimerActiveDays: Mon|Tue|Fri|Sat"),


    ArgoE2ETestParam(
      std::vector<uint16_t> {
        6464, 3156,  492, 2118,  472, 2108,  484, 820,  486, 2124,  454, 850,
        456, 848,  426, 2154,  492, 2120,  482, 822,  450, 854,  452, 2128,
        484, 2126,  454, 850,  454, 850,  456, 848,  424, 880,  480, 822,
        428, 2152,  482, 822,  484, 2126,  454, 850,  454, 850,  456, 2122,
        490, 814,  492, 2118,  452, 852,  454, 850,  454, 848,  458, 846,
        460, 2118,  482, 822,  484, 820,  486
      },
      kArgo3ConfigStateLength,
      std::vector<uint8_t> { 0xCB, 0x0C, 0x4A, 0x21 },
      "Config[CH#0]: Model: 2 (WREM3), Key: 12, Value: 74")),
  [](const testing::TestParamInfo<ArgoE2ETestParam>& info) {
      return bytesToHexString(info.param.expectedEncodedValue);
  }
);
