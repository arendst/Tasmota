/*
  xdrv_29_deepsleep.ino - DeepSleep support for Sonoff-Tasmota

  Copyright (C) 2019  Stefan Bode

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
\*********************************************************************************************/

#define XDRV_29                29

#define MAX_DEEPSLEEP_CYCLE  3600         // Maximum time for a deepsleep
#define MIN_DEEPSLEEP_TIME      5

#define D_PRFX_DEEPSLEEP "DeepSleep"
#define D_CMND_DEEPSLEEP_TIME "Time"

const char kDeepsleepCommands[] PROGMEM = D_PRFX_DEEPSLEEP "|"
  D_CMND_DEEPSLEEP_TIME ;

void (* const DeepsleepCommand[])(void) PROGMEM = {
  &CmndDeepsleepTime };

const char JSON_DEEPSLEEP[] PROGMEM = "\"" D_PRFX_DEEPSLEEP "%d\":{\"Time\":%d}";

void DeepSleepInit(void)
{
  if (pin[GPIO_DEEPSLEEP] < 99) {
    if (!digitalRead(pin[GPIO_DEEPSLEEP])) {
      RtcSettings.ultradeepsleep = 0;
    }
  }
  if ((RtcSettings.ultradeepsleep > MAX_DEEPSLEEP_CYCLE) && (RtcSettings.ultradeepsleep < 1700000000)) {
     RtcSettings.ultradeepsleep = RtcSettings.ultradeepsleep - MAX_DEEPSLEEP_CYCLE;
     RtcReboot.fast_reboot_count = 0;
     RtcRebootSave();
     AddLog_P2(LOG_LEVEL_ERROR, PSTR("DSL: Remain DeepSleep %d"), RtcSettings.ultradeepsleep);
     RtcSettingsSave();
     ESP.deepSleep(100 * RtcSettings.deepsleep_slip * (MAX_DEEPSLEEP_CYCLE < RtcSettings.ultradeepsleep ? MAX_DEEPSLEEP_CYCLE : RtcSettings.ultradeepsleep), WAKE_RF_DEFAULT);
     yield();
  }
  RtcSettings.ultradeepsleep = 0;
}

void CheckForDeepsleep(void)
{
  uint8_t disable_deepsleep_switch = 0;
  if (pin[GPIO_DEEPSLEEP] < 99) {
    disable_deepsleep_switch = !digitalRead(pin[GPIO_DEEPSLEEP]);
  }
  // new function AFTER_TELEPERIOD can take some time therefore <2
  if ((Settings.deepsleep > 10) && (Settings.deepsleep < 4294967295) && !disable_deepsleep_switch && (tele_period < 2) && prep_called) {
    SettingsSaveAll();
    // deepsleep_slip is ideally 10.000 == 100%
    // typically the device has up to 4% slip. Anything else is a wrong setting in the deepsleep_slip
    // therefore all values >110% or <90% will be resetted to 100% to avoid crazy sleep times.
    // This should normally never executed, but can happen an manual wakeup and problems during wakeup
    if ((RtcSettings.nextwakeup == 0) || (RtcSettings.deepsleep_slip < 9000) || (RtcSettings.deepsleep_slip > 11000) || (RtcSettings.nextwakeup > (UtcTime() + Settings.deepsleep))) {
      AddLog_P2(LOG_LEVEL_ERROR, PSTR("DSL: Reset wrong settings wakeup: %ld, slip %ld"),  RtcSettings.nextwakeup, RtcSettings.deepsleep_slip );
      RtcSettings.nextwakeup = 0;
      RtcSettings.deepsleep_slip = 10000;
//    AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("new settings wakeup: %ld, slip %ld"), RtcSettings.nextwakeup, RtcSettings.deepsleep_slip );
    }
    // timeslip in 0.1 seconds between the real wakeup and the calculated wakeup
    // because deepsleep is in second and timeslip in 0.1 sec the compare always check if the slip is in the 10% range
    int16_t timeslip = (int16_t)(RtcSettings.nextwakeup+millis()/1000-UtcTime())*10;
//    AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("DSL: Timeslip 0.1 sec:? %d < %d < %ld"), -Settings.deepsleep, timeslip, Settings.deepsleep );
    //allow 10% of deepsleep error to count as valid deepsleep; expecting 3-4%
    // if more then 10% timeslip = 0 == non valid wakeup; maybe manual
    timeslip = (timeslip < -(int32_t)Settings.deepsleep) ? 0 : (timeslip > (int32_t)Settings.deepsleep) ? 0 : 1;
//    AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("DSL: Normal deepsleep? %d"), timeslip );
    if (timeslip) {
      RtcSettings.deepsleep_slip = (Settings.deepsleep + RtcSettings.nextwakeup-UtcTime()) * RtcSettings.deepsleep_slip / (Settings.deepsleep - (millis() / 1000));
      //Avoid crazy numbers. Again maximum 10% deviation.
//      AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("DSL: %% calculate drift %ld"), RtcSettings.deepsleep_slip );
      RtcSettings.deepsleep_slip = tmin(tmax(RtcSettings.deepsleep_slip, 9000),11000);

//      AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("DSL: %% new drift %ld"), RtcSettings.deepsleep_slip );
      RtcSettings.nextwakeup += Settings.deepsleep;
    }
    // it may happen that wakeup in just <5 seconds in future
    // in this case also add deepsleep to nextwakeup
    if (RtcSettings.nextwakeup <= (UtcTime() - MIN_DEEPSLEEP_TIME)) {
      // ensure nextwakeup is at least in the future
      RtcSettings.nextwakeup += (((UtcTime() + MIN_DEEPSLEEP_TIME - RtcSettings.nextwakeup) / Settings.deepsleep) + 1) * Settings.deepsleep;
    }
    String dt = GetDT(RtcSettings.nextwakeup + LocalTime() - UtcTime());  // 2017-03-07T11:08:02
//    AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("DSL: Next wakeup %s"), (char*)dt.c_str());
    //limit sleeptime to MAX_DEEPSLEEP_CYCLE
    //uint32_t sleeptime = MAX_DEEPSLEEP_CYCLE < (RtcSettings.nextwakeup - UtcTime()) ? (uint32_t)MAX_DEEPSLEEP_CYCLE : RtcSettings.nextwakeup - UtcTime();
    uint32_t sleeptime = tmin((uint32_t)MAX_DEEPSLEEP_CYCLE , RtcSettings.nextwakeup - UtcTime());
    Response_P(PSTR("{\"" D_PRFX_DEEPSLEEP "\":{\"" D_JSON_TIME "\":\"%s\",\"Epoch\":%d}}"), (char*)dt.c_str(), RtcSettings.nextwakeup);
    MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_STATUS "1"), false);
    Response_P(S_OFFLINE);
    MqttPublishPrefixTopic_P(TELE, PSTR(D_LWT), true);  // Offline or remove previous retained topic
    yield();
    MqttDisconnect();
    RtcSettings.ultradeepsleep =  RtcSettings.nextwakeup - UtcTime();
//    AddLog_P2(LOG_LEVEL_DEBUG_MORE, PSTR("DSL: Sleeptime %d sec, deepsleep_slip %ld"), sleeptime, RtcSettings.deepsleep_slip);
    RtcSettingsSave();
    ESP.deepSleep(100 * RtcSettings.deepsleep_slip * sleeptime);
    yield();
  }
  prep_called = false;
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndDeepsleepTime(void)
{
//  if ((XdrvMailbox.payload == 0) || ((XdrvMailbox.payload > 10) && (XdrvMailbox.payload < 4294967295))) {
  if ((XdrvMailbox.payload == 0) || ((XdrvMailbox.payload > 10) && (XdrvMailbox.payload < (24 * 60 * 60)))) {  // Allow max 24 hours sleep
    Settings.deepsleep = XdrvMailbox.payload;
    RtcSettings.nextwakeup = 0;
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
    case FUNC_AFTER_TELEPERIOD:
      CheckForDeepsleep();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kDeepsleepCommands, DeepsleepCommand);
      break;
    case FUNC_PRE_INIT:
      DeepSleepInit();
      break;
  }
  return result;
}

#endif //USE_DEEPSLEEP
