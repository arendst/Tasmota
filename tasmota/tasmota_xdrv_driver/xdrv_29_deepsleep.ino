/*
  xdrv_29_deepsleep.ino - DeepSleep support for Tasmota

  Copyright (C) 2023  Stefan Bode

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
 * See wiki https://tasmota.github.io/docs/DeepSleep/
\*********************************************************************************************/

#define XDRV_29                29

#define D_PRFX_DEEPSLEEP "DeepSleep"
#define D_CMND_DEEPSLEEP_TIME "Time"
#ifndef DEEPSLEEP_NETWORK_TIMEOUT
  #define DEEPSLEEP_NETWORK_TIMEOUT 15
#endif

const uint32_t DEEPSLEEP_MAX = 10 * 366 * 24 * 60 * 60;  // Allow max 10 years sleep
const uint32_t DEEPSLEEP_MAX_CYCLE = 60 * 60;            // Maximum time for a deepsleep as defined by chip hardware
const uint32_t DEEPSLEEP_MIN_TIME = 15;                  // Allow 15 seconds skew
const uint32_t DEEPSLEEP_START_COUNTDOWN = 4;            // Allow 4 seconds to update web console before deepsleep

const char kDeepsleepCommands[] PROGMEM = D_PRFX_DEEPSLEEP "|"
  D_CMND_DEEPSLEEP_TIME ;

void (* const DeepsleepCommand[])(void) PROGMEM = {
  &CmndDeepsleepTime };

uint32_t deepsleep_sleeptime = 0;
uint8_t deepsleep_flag = 0;

bool DeepSleepEnabled(void)
{
#if defined(USE_RULES) && defined(USE_TIMERS) 
  if (GetRule(0) == "Wakeup" && bitRead(Settings->rule_enabled | Settings->rule_once, 0)) {
    return true;
  }
#endif
  if ((Settings->deepsleep < 10) || (Settings->deepsleep > DEEPSLEEP_MAX)) {
    Settings->deepsleep = 0;    // Issue #6961
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
      AddLog(LOG_LEVEL_ERROR, PSTR("DSL: Remain DeepSleep %d"), RtcSettings.ultradeepsleep);
      RtcSettingsSave();
      RtcRebootReset();
#ifdef ESP8266
      ESP.deepSleep(100 * RtcSettings.deepsleep_slip * (DEEPSLEEP_MAX_CYCLE < RtcSettings.ultradeepsleep ? DEEPSLEEP_MAX_CYCLE : RtcSettings.ultradeepsleep), WAKE_RF_DEFAULT);
#endif  // ESP8266
#ifdef ESP32
      esp_sleep_enable_timer_wakeup(100 * RtcSettings.deepsleep_slip * (DEEPSLEEP_MAX_CYCLE < RtcSettings.ultradeepsleep ? DEEPSLEEP_MAX_CYCLE : RtcSettings.ultradeepsleep));
      esp_deep_sleep_start();
#endif  // ESP32
      yield();
      // Sleeping
    }
  }
  // Stay awake
  RtcSettings.ultradeepsleep = 0;
}

// Function to find the next relevant weekday
int8_t getNextWeekday(uint8_t currentDay, uint32_t day_bitarray, bool sameDayEventPassed) 
{
    for (uint8_t daysChecked = 0; daysChecked < 7; ++daysChecked) {
        uint8_t nextDay = (currentDay + daysChecked) % 7;
        if ((day_bitarray & (1 << nextDay)) && (daysChecked > 0 || !sameDayEventPassed)) {
            return nextDay;
        }
    }
    return 0;
}

// Funktion zur Berechnung der Sekunden bis zum nächsten ausgewählten Wochentag und Zeit
uint32_t calculateSecondsToTarget(uint32_t targetMinutesAfterMidnight, uint32_t day_bitarray) 
{
  int32_t  currentMinutesAfterMidnight = (RtcTime.hour *60) + RtcTime.minute;

  // samedayevent is a bit tricky. is wakeup is 10% before deepsleeptime this is tagged as "wakeup at time".
  // e.g. deepsleep 3600 -> 360s (min) before the hour is fine and no other wakeup e.g in 6min. In this case the next 
  // wakeup is in 60+6min. A wakeup after the proposed time is no problem at all.
  bool     sameDayEventPassed = currentMinutesAfterMidnight + 1 + (RtcSettings.nextwakeup < LocalTime()?0:(Settings->deepsleep / 600)) >= targetMinutesAfterMidnight;
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DSL: passed: %d, currMin: %d, tagetMin: %d + error: %d->%d"), sameDayEventPassed, currentMinutesAfterMidnight,  targetMinutesAfterMidnight, Settings->deepsleep, (RtcSettings.nextwakeup < LocalTime()?0:(Settings->deepsleep / 600)));
  int8_t   nextWeekday = getNextWeekday(RtcTime.day_of_week - 1, day_bitarray, sameDayEventPassed);
  uint32_t secondsUntilTarget = 0;
  int8_t   daysUntilNextWeekday = (nextWeekday + 7 - (RtcTime.day_of_week - 1)) % 7;

  // Wenn das Zielereignis am selben Tag und bereits vorbei ist, wird der nächste mögliche Tag in der nächsten Woche berechnet
  if (daysUntilNextWeekday == 0 && sameDayEventPassed) {
      daysUntilNextWeekday = 7;
  }
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DSL: nextDay %d, Duration %d"), 
                                      nextWeekday, daysUntilNextWeekday);

  secondsUntilTarget = daysUntilNextWeekday * 24 * 60 * 60 + (targetMinutesAfterMidnight - currentMinutesAfterMidnight) * 60;
  return secondsUntilTarget;
}

void DeepSleepCalculate()
{
  uint32_t secondsToTarget = 604800; // one week is the maximum
  // Settings->deepsleep will be calculated by timers if timer is set to rule and rule is "Wakeup"
  
  // default add to the next wakeup. May be overwritten by "Wakeup" rule
  RtcSettings.nextwakeup += Settings->deepsleep;
#if defined(USE_RULES) && defined(USE_TIMERS) 
  if (RtcTime.valid) {
    if (!RtcTime.hour && !RtcTime.minute && !RtcTime.second) { TimerSetRandomWindows(); }  // Midnight
    if (Settings->flag3.timers_enable) { 
      int32_t time = (RtcTime.hour *60) + RtcTime.minute;
      for (uint32_t i = 0; i < MAX_TIMERS; i++) {
        Timer    xtimer = Settings->timer[i];
        uint32_t day_bitarray = xtimer.days;
        // day_bitarray>0 otherwhise no weekday selected
        // rule keyword "Wakeup"
        // Timer action: rule
        if (xtimer.arm && day_bitarray && GetRule(0) == "Wakeup" && (TasmotaGlobal.restart_deepsleep || bitRead(Settings->rule_enabled | Settings->rule_once, 0)) && POWER_BLINK == xtimer.power) {
#ifdef USE_SUNRISE
          if ((1 == xtimer.mode) || (2 == xtimer.mode)) {      // Sunrise or Sunset
            ApplyTimerOffsets(&xtimer);
            if (xtimer.time>=2046) { continue; }
          }
#endif
          Settings->timer[i].arm = xtimer.repeat;
          uint32_t secondsToTargetTemp = calculateSecondsToTarget(xtimer.time,  day_bitarray);
          secondsToTarget = tmin(secondsToTarget, secondsToTargetTemp);
        }
      } // loop over timers
      // we found a timer 604800 == seconds in one week
      if (secondsToTarget < 604800) {
        Settings->deepsleep = secondsToTarget;
        // check if rule ist just once
        bitWrite(Settings->rule_once, 0, 0);
        SettingsSaveAll();
        AddLog(LOG_LEVEL_DEBUG, PSTR("DSL: Wakeup: %ld [s]"), secondsToTarget);
        RtcSettings.nextwakeup = LocalTime() + secondsToTarget - (LocalTime() % 60);
      }
    }
  } //if (RtcTime.valid)
#endif  // USE_RULES && TIMERS
}



void DeepSleepStart(void)
{
  char stopic[TOPSZ];
  // Deepsleep_slip is ideally 10.000 == 100%
  // Typically the device has up to 4% slip. Anything else is a wrong setting in the deepsleep_slip
  // Therefore all values >110% or <90% will be resetted to 100% to avoid crazy sleep times.
  // This should normally never executed, but can happen an manual wakeup and problems during wakeup
  if ((RtcSettings.nextwakeup == 0) ||
      (RtcSettings.deepsleep_slip < 9000) ||
      (RtcSettings.deepsleep_slip > 11000) ||
      (RtcSettings.nextwakeup > (LocalTime() + Settings->deepsleep))) {
    AddLog(LOG_LEVEL_ERROR, PSTR("DSL: Reset wrong settings wakeup: %ld, slip %ld"), RtcSettings.nextwakeup, RtcSettings.deepsleep_slip );
    RtcSettings.nextwakeup = 0;
    RtcSettings.deepsleep_slip = 10000;
  }
  
  // Timeslip in 0.1 seconds between the real wakeup and the calculated wakeup
  // Because deepsleep is in second and timeslip in 0.1 sec the compare always check if the slip is in the 10% range
  int32_t timeslip = (int32_t)(RtcSettings.nextwakeup + millis() / 1000 - LocalTime()) * 10;

  // Allow 10% of deepsleep error to count as valid deepsleep; expecting 3-4%
  // if more then 10% timeslip = 0 == non valid wakeup; maybe manual
  timeslip = (timeslip < -(int32_t)Settings->deepsleep) ? 0 : (timeslip > (int32_t)Settings->deepsleep) ? 0 : 1;
  DeepSleepCalculate();
  if (timeslip) {
    RtcSettings.deepsleep_slip = (RtcSettings.nextwakeup - LocalTime()) * RtcSettings.deepsleep_slip / tmax((Settings->deepsleep - (millis() / 1000)),5);
    // Avoid crazy numbers. Again maximum 10% deviation.
    RtcSettings.deepsleep_slip = tmin(tmax(RtcSettings.deepsleep_slip, 9000), 11000);
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("DSL: Time %ld, next %ld, slip %ld"), timeslip, RtcSettings.nextwakeup, RtcSettings.deepsleep_slip );
  // It may happen that wakeup in just <5 seconds in future
  // In this case also add deepsleep to nextwakeup
  if (RtcSettings.nextwakeup <= (LocalTime() + DEEPSLEEP_MIN_TIME) && Settings->deepsleep > 0 ) {
    // ensure nextwakeup is at least in the future, and add 5%
    RtcSettings.nextwakeup += (((LocalTime() + DEEPSLEEP_MIN_TIME - RtcSettings.nextwakeup) / Settings->deepsleep) + 1) * Settings->deepsleep;
    //RtcSettings.nextwakeup += Settings->deepsleep * 0.05;
    //AddLog(LOG_LEVEL_DEBUG, PSTR("DSL: Time too short: time %ld, next %ld, slip %ld"), timeslip, RtcSettings.nextwakeup, RtcSettings.deepsleep_slip);
  }

  String dt = GetDT(RtcSettings.nextwakeup);  // 2017-03-07T11:08:02
  if (Settings->flag3.time_append_timezone) {  // SetOption52 - Append timezone to JSON time
    dt += GetTimeZone();    // 2017-03-07T11:08:02-07:00
  }
  // Limit sleeptime to DEEPSLEEP_MAX_CYCLE
  // uint32_t deepsleep_sleeptime = DEEPSLEEP_MAX_CYCLE < (RtcSettings.nextwakeup - LocalTime()) ? (uint32_t)DEEPSLEEP_MAX_CYCLE : RtcSettings.nextwakeup - LocalTime();
  deepsleep_sleeptime = tmin((uint32_t)DEEPSLEEP_MAX_CYCLE ,RtcSettings.nextwakeup>0?RtcSettings.nextwakeup - LocalTime():0);

  

  // Sending Deepsleep parameters to automation for react
  Response_P(PSTR("{\"" D_PRFX_DEEPSLEEP "\":{\"" D_JSON_TIME "\":\"%s\",\"" D_PRFX_DEEPSLEEP "\":%d,\"Wakeup\":%d}}"), (char*)dt.c_str(), LocalTime(), RtcSettings.nextwakeup);
  MqttPublishPrefixTopicRulesProcess_P(TELE, PSTR(D_PRFX_DEEPSLEEP), true);

    WifiShutdown();
    RtcSettings.ultradeepsleep = RtcSettings.nextwakeup - LocalTime();
    RtcSettingsSave();
    RtcRebootReset();
#ifdef ESP8266
    ESP.deepSleep(100 * RtcSettings.deepsleep_slip * deepsleep_sleeptime);
#endif  // ESP8266
#ifdef ESP32
  esp_sleep_enable_timer_wakeup(100 * RtcSettings.deepsleep_slip * deepsleep_sleeptime);
  esp_deep_sleep_start();
#endif  // ESP32
  yield();

}


void DeepSleepEverySecond(void)
{
  //AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("Wifi Info: up %d, wifidown %d, wifistatus %d, flag %d"),TasmotaGlobal.uptime, TasmotaGlobal.global_state.wifi_down, Wifi.status , deepsleep_flag);
  if (DEEPSLEEP_NETWORK_TIMEOUT && TasmotaGlobal.uptime > DEEPSLEEP_NETWORK_TIMEOUT && Wifi.status != WL_CONNECTED && !deepsleep_flag && DeepSleepEnabled()) {
    AddLog(LOG_LEVEL_ERROR, PSTR("Error Wifi no connect %d [s]. Deepsleep"), DEEPSLEEP_NETWORK_TIMEOUT);
    deepsleep_flag = DEEPSLEEP_START_COUNTDOWN;  // Start deepsleep in 4 seconds
  }
#if defined(USE_RULES) && defined(USE_TIMERS)
  if (!deepsleep_flag) { 
    //AddLog(LOG_LEVEL_ERROR, PSTR("DSL: timers %d, rule: %d, once %d, comb %d, bitr %d"),Settings->flag3.timers_enable, Settings->rule_enabled, Settings->rule_once, Settings->rule_enabled | Settings->rule_once, bitRead(Settings->rule_enabled | Settings->rule_once, 0));
    if (Settings->flag3.timers_enable && GetRule(0) == "Wakeup" 
        && bitRead(Settings->rule_enabled | Settings->rule_once, 0) ) { 
      //AddLog(LOG_LEVEL_ERROR, PSTR("DSL: Start timer"));    
      deepsleep_flag = 60; 
    }
    return; 
  }
  //AddLog(LOG_LEVEL_ERROR, PSTR("DSL: wakeup: %d, timer %d, rules %d"),GetRule(0) == "Wakeup", Settings->flag3.timers_enable, bitRead(Settings->rule_enabled | Settings->rule_once, 0));
  if (GetRule(0) == "Wakeup" && ( !Settings->flag3.timers_enable || !bitRead(Settings->rule_enabled | Settings->rule_once, 0) )) { 
    deepsleep_flag = 0;
    Settings->deepsleep = 0;
    //AddLog(LOG_LEVEL_ERROR, PSTR("DSL: Stop timer")); 
    return; 
  }
#else
  if (!deepsleep_flag) { return; }
#endif

  deepsleep_flag--;
  AddLog(LOG_LEVEL_ERROR, PSTR("DSL: Countdown: %d"),deepsleep_flag);
  if (DeepSleepEnabled()) {
    if (1 == deepsleep_flag) {  
      SettingsSaveAll();
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
    Settings->deepsleep = XdrvMailbox.payload;
    RtcSettings.nextwakeup = 0;
    deepsleep_flag = (0 == XdrvMailbox.payload) ? 0 : DEEPSLEEP_START_COUNTDOWN + (ResetReason() != REASON_DEEP_SLEEP_AWAKE?60:0);
    if (deepsleep_flag) {
      if (!Settings->tele_period) {
        Settings->tele_period = TELE_PERIOD;  // Need teleperiod to go back to sleep
      }
    }
    TasmotaGlobal.discovery_counter = 1; // force TasDiscovery()
  }
  ResponseCmndNumber(Settings->deepsleep);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv29(uint32_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_EVERY_SECOND:
      DeepSleepEverySecond();
      break;
    case FUNC_AFTER_TELEPERIOD:
        if (DeepSleepEnabled() && !deepsleep_flag && (Settings->tele_period == 10 || Settings->tele_period == 300 || millis() > 20000)) {
          // on initial start the device will be 40 seconds awake. This allows to make changes
          deepsleep_flag = DEEPSLEEP_START_COUNTDOWN + (ResetReason() != REASON_DEEP_SLEEP_AWAKE?60:0);  // Start deepsleep in 4 seconds
        }
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kDeepsleepCommands, DeepsleepCommand);
      break;
    case FUNC_PRE_INIT:
      DeepSleepReInit();
      break;
    case FUNC_ACTIVE:
      result = true;
      break;
  }
  return result;
}

#endif //USE_DEEPSLEEP
