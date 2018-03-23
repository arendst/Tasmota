/*
  xdrv_09_timers.ino - timer support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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

#ifdef USE_TIMERS
/*********************************************************************************************\
 * Timers
 *
 * Arm a timer using one or all of the following JSON values:
 * {"Arm":1,"Time":"09:23","Days":"--TW--S","Repeat":1,"Device":1,"Power":1}
 *
 * Arm    0 = Off, 1 = On
 * Time   hours:minutes
 * Days   7 day character mask starting with Sunday (SMTWTFS). 0 or - = Off, any other value = On
 * Repeat 0 = Execute once, 1 = Execute again
 * Device 1..16
 * Power  0 = Off, 1 = On, 2 = Toggle, 3 = Blink
 *
\*********************************************************************************************/

enum TimerCommands { CMND_TIMER, CMND_TIMERS };
const char kTimerCommands[] PROGMEM = D_CMND_TIMER "|" D_CMND_TIMERS ;

power_t fired = 0;

void TimerEverySecond()
{
  if (RtcTime.valid) {
    uint16_t time = (RtcTime.hour * 60) + RtcTime.minute;
    uint8_t days = 1 << (RtcTime.day_of_week -1);

    for (byte i = 0; i < MAX_TIMERS; i++) {
      if (Settings.timer[i].arm) {
        if (time == Settings.timer[i].time) {
          if (!bitRead(fired, i) && (Settings.timer[i].days & days)) {
            bitSet(fired, i);
            Settings.timer[i].arm = Settings.timer[i].repeat;
            ExecuteCommandPower(Settings.timer[i].device +1, Settings.timer[i].power);
          }
        } else {
          bitClear(fired, i);
        }
      }
    }
  }
}

void PrepShowTimer(uint8_t index)
{
  char days[8] = { 0 };

  index--;
  for (byte i = 0; i < 7; i++) {
    uint8_t mask = 1 << i;
    snprintf(days, sizeof(days), "%s%d", days, ((Settings.timer[index].days & mask) > 0));
  }
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s\"" D_CMND_TIMER "%d\":{\"" D_JSON_TIMER_ARM "\":%d,\"" D_JSON_TIMER_TIME "\":\"%02d:%02d\",\"" D_JSON_TIMER_DAYS "\":\"%s\",\"" D_JSON_TIMER_REPEAT "\":%d,\"" D_JSON_TIMER_DEVICE "\":%d,\"" D_JSON_TIMER_POWER "\":%d}"),
    mqtt_data, index +1, Settings.timer[index].arm, Settings.timer[index].time / 60, Settings.timer[index].time % 60, days, Settings.timer[index].repeat, Settings.timer[index].device +1, Settings.timer[index].power);
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean TimerCommand()
{
  char command [CMDSZ];
  char dataBufUc[XdrvMailbox.data_len];
  boolean serviced = true;
  uint8_t index = XdrvMailbox.index;

  UpperCase(dataBufUc, XdrvMailbox.data);
  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kTimerCommands);
  if ((CMND_TIMER == command_code) && (index > 0) && (index <= MAX_TIMERS)) {
    uint8_t error = 0;

    if (XdrvMailbox.data_len) {
      StaticJsonBuffer<128> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(dataBufUc);
      if (!root.success()) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_TIMER "%d\":\"" D_JSON_INVALID_JSON "\"}"), index); // JSON decode failed
        error = 1;
      }
      else {
        char parm_uc[10];

        index--;
        if (root[UpperCase_P(parm_uc, PSTR(D_JSON_TIMER_ARM))].success()) {
          Settings.timer[index].arm = (root[parm_uc] != 0);
        }
        if (root[UpperCase_P(parm_uc, PSTR(D_JSON_TIMER_TIME))].success()) {
          uint16_t itime = 0;
          uint8_t value = 0;
          char time_str[10];

          snprintf(time_str, sizeof(time_str), root[parm_uc]);
          const char *substr = strtok(time_str, ":");
          if (substr != NULL) {
            value = atoi(substr);
            if (value > 23) value = 23;
            itime = value * 60;
            substr = strtok(NULL, ":");
            if (substr != NULL) {
              value = atoi(substr);
              if (value > 59) value = 59;
              itime += value;
            }
          }
          Settings.timer[index].time = itime;
        }
        if (root[UpperCase_P(parm_uc, PSTR(D_JSON_TIMER_DAYS))].success()) {
          // SMTWTFS = 1234567 = 0011001 = 00TW00S = --TW--S
          Settings.timer[index].days = 0;
          const char *tday = root[parm_uc];
          char ch = '.';

          uint8_t i = 0;
          while ((ch != '\0') && (i < 7)) {
            ch = *tday++;
            if (ch == '-') ch = '0';
            uint8_t mask = 1 << i++;
            Settings.timer[index].days |= (ch == '0') ? 0 : mask;
          }
        }
        if (root[UpperCase_P(parm_uc, PSTR(D_JSON_TIMER_REPEAT))].success()) {
          Settings.timer[index].repeat = (root[parm_uc] != 0);
        }
        if (root[UpperCase_P(parm_uc, PSTR(D_JSON_TIMER_DEVICE))].success()) {
          Settings.timer[index].device = ((uint8_t)root[parm_uc] -1) & 0x0F;
        }
        if (root[UpperCase_P(parm_uc, PSTR(D_JSON_TIMER_POWER))].success()) {
          Settings.timer[index].power = (uint8_t)root[parm_uc] & 0x03;
        }
        if (Settings.timer[index].arm) bitClear(fired, index);

        index++;
      }
    }
    if (!error) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{"));
      PrepShowTimer(index);
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
    }
  }
  else if (CMND_TIMERS == command_code) {
    byte jsflg = 0;
    byte lines = 1;
    for (byte i = 0; i < MAX_TIMERS; i++) {
      if (!jsflg) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_TIMERS "%d\":{"), lines++);
      } else {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,"), mqtt_data);
      }
      jsflg = 1;
      PrepShowTimer(i +1);
      if ((strlen(mqtt_data) > (LOGSZ - TOPSZ)) || (i == MAX_TIMERS -1)) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}}"), mqtt_data);
        MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_TIMERS));
        jsflg = 0;
      }
    }
    mqtt_data[0] = '\0';
  }
  else serviced = false;

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_09

boolean Xdrv09(byte function)
{
  boolean result = false;

  switch (function) {
    case FUNC_EVERY_SECOND:
      TimerEverySecond();
      break;
    case FUNC_COMMAND:
      result = TimerCommand();
      break;
  }
  return result;
}

#endif  // USE_TIMERS