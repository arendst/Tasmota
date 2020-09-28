/*
  xdrv_29_deepsleep.ino - DeepSleep support for Tasmota

  Copyright (C) 2020  Stefan Bode

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_DEEPSLEEP
/*********************************************************************************************\
 * DeepSleep Support
 *
 * - For wakeup from DeepSleep needs GPIO16 to be connected to RST
 * - GPIO_DEEPSLEEP may be used to stop DeepSleep when connected to Gnd
 * - GPIO16 may be configured as GPIO_DEEPSLEEP
 *
 * See wiki https://github.com/arendst/Tasmota/wiki/DeepSleep
\*********************************************************************************************/

#define XDRV_29                29

#define D_PRFX_DEEPSLEEP "DeepSleep"
#define D_CMND_DEEPSLEEP_TIME "Time"

const uint32_t DEEPSLEEP_MAX = 10 * 366 * 24 * 60 * 60;  // Allow max 10 years sleep
const uint32_t DEEPSLEEP_MAX_CYCLE = 60 * 60;            // Maximum time for a deepsleep as defined by chip hardware
const uint32_t DEEPSLEEP_MIN_TIME = 5;                   // Allow 5 seconds skew
const uint32_t DEEPSLEEP_START_COUNTDOWN = 4;            // Allow 4 seconds to update web console before deepsleep

const char kDeepsleepCommands[] PROGMEM = D_PRFX_DEEPSLEEP "|"
  D_CMND_DEEPSLEEP_TIME ;

void (* const DeepsleepCommand[])(void) PROGMEM = {
  &CmndDeepsleepTime };

uint32_t deepsleep_sleeptime = 0;
uint8_t deepsleep_flag = 0;

bool DeepSleepEnabled(void)
{
  if ((Settings.deepsleep < 10) || (Settings.deepsleep > DEEPSLEEP_MAX)) {
    Settings.deepsleep = 0;     // Issue #6961
    return false;               // Disabled
  }

  if (PinUsed(GPIO_DEEPSLEEP)) {
    pinMode(Pin(GPIO_DEEPSLEEP), INPUT_PULLUP);
    return (digitalRead(Pin(GPIO_DEEPSLEEP)));  // Disable DeepSleep if user holds pin GPIO_DEEPSLEEP low
  }

  return true;                  // Enabled
}

void DeepSleepReInit(void)
{
  if ((ResetReason() == REASON_DEEP_SLEEP_AWAKE) && DeepSleepEnabled()) {
    if ((RtcSettings.ultradeepsleep > DEEPSLEEP_MAX_CYCLE) && (RtcSettings.ultradeepsleep < 1700000000)) {
      // Go back to sleep after 60 minutes if requested deepsleep has not been reached
      RtcSettings.ultradeepsleep = RtcSettings.ultradeepsleep - DEEPSLEEP_MAX_CYCLE;
      AddLog_P2(LOG_LEVEL_ERROR, PSTR("DSL: Remain DeepSleep %d"), RtcSettings.ultradeepsleep);
      RtcSettingsSave();
      RtcRebootReset();
#ifdef ESP8266
      ESP.deepSleep(100 * RtcSettings.deepsleep_slip * (DEEPSLEEP_MAX_CYCLE < RtcSettings.ultradeepsleep ? DEEPSLEEP_MAX_CYCLE : RtcSettings.ultradeepsleep), WAKE_RF_DEFAULT);
#else  // ESP32
      esp_sleep_enable_timer_wakeup(100 * RtcSettings.deepsleep_slip * (DEEPSLEEP_MAX_CYCLE < RtcSettings.ultradeepsleep ? DEEPSLEEP_MAX_CYCLE : RtcSettings.ultradeepsleep));
      esp_deep_sleep_start();
#endif  // ESP8266 or ESP32
      yield();
      // Sleeping
    }
  }
  // Stay awake
  RtcSettings.ultradeepsleep = 0;
}

void DeepSleepPrepare(void)
{
  // Deepsleep_slip is ideally 10.000 == 100%
  // Typically the device has up to 4% slip. Anything else is a wrong setting in the deepsleep_slip
  // Therefore all values >110% or <90% will be resetted to 100% to avoid crazy sleep times.
  // This should normally never executed, but can happen an manual wakeup and problems during wakeup
  if ((RtcSettings.nextwakeup == 0) ||
      (RtcSettings.deepsleep_slip < 9000) ||
      (RtcSettings.deepsleep_slip > 11000) ||
      (RtcSettings.nextwakeup > (UtcTime() + Settings.deepsleep))) {
    AddLog_P2(LOG_LEVEL_ERROR, PSTR("DSL: Reset wrong settings wakeup: %ld, slip %ld"), RtcSettings.nextwakeup, RtcSettings.deepsleep_slip );
    RtcSettings.nextwakeup = 0;
    RtcSettings.deepsleep_slip = 10000;
  }

  // Timeslip in 0.1 seconds between the real wakeup and the calculated wakeup
  // Because deepsleep is in second and timeslip in 0.1 sec the compare always check if the slip is in the 10% range
  int16_t timeslip = (int16_t)(RtcSettings.nextwakeup + millis() / 1000 - UtcTime()) * 10;

  // Allow 10% of deepsleep error to count as valid deepsleep; expecting 3-4%
  // if more then 10% timeslip = 0 == non valid wakeup; maybe manual
  timeslip = (timeslip < -(int32_t)Settings.deepsleep) ? 0 : (timeslip > (int32_t)Settings.deepsleep) ? 0 : 1;
  if (timeslip) {
    RtcSettings.deepsleep_slip = (Settings.deepsleep + RtcSettings.nextwakeup - UtcTime()) * RtcSettings.deepsleep_slip / tmax((Settings.deepsleep - (millis() / 1000)),5);
    // Avoid crazy numbers. Again maximum 10% deviation.
    RtcSettings.deepsleep_slip = tmin(tmax(RtcSettings.deepsleep_slip, 9000), 11000);
    RtcSettings.nextwakeup += Settings.deepsleep;
  }

  // It may happen that wakeup in just <5 seconds in future
  // In this case also add deepsleep to nextwakeup
  if (RtcSettings.nextwakeup <= (UtcTime() - DEEPSLEEP_MIN_TIME)) {
    // ensure nextwakeup is at least in the future
    RtcSettings.nextwakeup += (((UtcTime() + DEEPSLEEP_MIN_TIME - RtcSettings.nextwakeup) / Settings.deepsleep) + 1) * Settings.deepsleep;
  }

  String dt = GetDT(RtcSettings.nextwakeup + LocalTime() - UtcTime());  // 2017-03-07T11:08:02
  // Limit sleeptime to DEEPSLEEP_MAX_CYCLE
  // uint32_t deepsleep_sleeptime = DEEPSLEEP_MAX_CYCLE < (RtcSettings.nextwakeup - UtcTime()) ? (uint32_t)DEEPSLEEP_MAX_CYCLE : RtcSettings.nextwakeup - UtcTime();
  deepsleep_sleeptime = tmin((uint32_t)DEEPSLEEP_MAX_CYCLE ,RtcSettings.nextwakeup - UtcTime());

  // stat/tasmota/STATUS = {"DeepSleep":{"Time":"2019-11-12T21:33:45","Epoch":1573590825}}
  Response_P(PSTR("{\"" D_PRFX_DEEPSLEEP "\":{\"" D_JSON_TIME "\":\"%s\",\"Epoch\":%d}}"), (char*)dt.c_str(), RtcSettings.nextwakeup);
  MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_STATUS));

//  Response_P(S_LWT_OFFLINE);
//  MqttPublishPrefixTopic_P(TELE, PSTR(D_LWT), true);  // Offline or remove previous retained topic
}

void DeepSleepStart(void)
{
  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION "Sleeping"));  // Won't show in GUI

  WifiShutdown();
  RtcSettings.ultradeepsleep = RtcSettings.nextwakeup - UtcTime();
  RtcSettingsSave();
#ifdef ESP8266
  ESP.deepSleep(100 * RtcSettings.deepsleep_slip * deepsleep_sleeptime);
#else  // ESP32
  esp_sleep_enable_timer_wakeup(100 * RtcSettings.deepsleep_slip * deepsleep_sleeptime);
  esp_deep_sleep_start();
#endif  // ESP8266 or ESP32
  yield();
}

void DeepSleepEverySecond(void)
{
  if (!deepsleep_flag) { return; }

  if (DeepSleepEnabled()) {
    if (DEEPSLEEP_START_COUNTDOWN == deepsleep_flag) {  // Allow 4 seconds to update web console before deepsleep
      SettingsSaveAll();
      DeepSleepPrepare();
    }
    deepsleep_flag--;
    if (deepsleep_flag <= 0) {
      DeepSleepStart();
    }
  } else {
    deepsleep_flag = 0;
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndDeepsleepTime(void)
{
  if ((0 == XdrvMailbox.payload) ||
     ((XdrvMailbox.payload > 10) && (XdrvMailbox.payload < DEEPSLEEP_MAX))) {
    Settings.deepsleep = XdrvMailbox.payload;
    RtcSettings.nextwakeup = 0;
    deepsleep_flag = (0 == XdrvMailbox.payload) ? 0 : DEEPSLEEP_START_COUNTDOWN;
    if (deepsleep_flag) {
      if (!Settings.tele_period) {
        Settings.tele_period = TELE_PERIOD;  // Need teleperiod to go back to sleep
      }
    }
  }
  Response_P(S_JSON_COMMAND_NVALUE, XdrvMailbox.command, Settings.deepsleep);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv29(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_EVERY_SECOND:
      DeepSleepEverySecond();
      break;
    case FUNC_AFTER_TELEPERIOD:
        if (DeepSleepEnabled() && !deepsleep_flag && (Settings.tele_period == 10 || Settings.tele_period == 300 || UpTime() > Settings.tele_period)) {
        deepsleep_flag = DEEPSLEEP_START_COUNTDOWN;  // Start deepsleep in 4 seconds
      }
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kDeepsleepCommands, DeepsleepCommand);
      break;
    case FUNC_PRE_INIT:
      DeepSleepReInit();
      break;
  }
  return result;
}

#endif //USE_DEEPSLEEP
