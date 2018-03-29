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

uint16_t fired = 0;

void TimerEverySecond()
{
  if (RtcTime.valid) {
    uint16_t time = (RtcTime.hour * 60) + RtcTime.minute;
    uint8_t days = 1 << (RtcTime.day_of_week -1);

    for (byte i = 0; i < MAX_TIMERS; i++) {
      if (Settings.timer[i].device >= devices_present) Settings.timer[i].data = 0;  // Reset timer due to change in devices present
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
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= MAX_TIMERS)) {
        if (XdrvMailbox.payload == 0) {
          Settings.timer[index -1].data = 0;  // Clear timer
        } else {
          Settings.timer[index -1].data = Settings.timer[XdrvMailbox.payload -1].data;  // Copy timer
        }
      } else {
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
            uint8_t device = ((uint8_t)root[parm_uc] -1) & 0x0F;
            Settings.timer[index].device = (device < devices_present) ? device : devices_present -1;
          }
          if (root[UpperCase_P(parm_uc, PSTR(D_JSON_TIMER_POWER))].success()) {
            Settings.timer[index].power = (uint8_t)root[parm_uc] & 0x03;
          }
          if (Settings.timer[index].arm) bitClear(fired, index);

          index++;
        }
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
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
#ifdef USE_TIMERS_WEB
const char HTTP_TIMER_SCRIPT[] PROGMEM =
  "var pt=[],ct=99;"
  "function qs(s){"                                               // Save code space
    "return document.querySelector(s);"
  "}"
  "function ce(i,q){"                                             // Create select option
    "var o=document.createElement('option');"
    "o.textContent=i;"
    "q.appendChild(o);"
  "}"
  "function st(){"                                                // Save parameters to hidden area
    "var d,h,i,m,n,s,p;"
    "h=qs('#ho');"
    "m=qs('#mi');"
    "d=qs('#d1');"
    "s=0;"
    "n=1<<30;if(eb('a0').checked){s|=n;}"                         // Get arm
    "n=1<<29;if(eb('r0').checked){s|=n;}"                         // Get repeat
    "for(i=0;i<7;i++){n=1<<(16+i);if(eb('w'+i).checked){s|=n;}}"  // Get weekdays
    "s|=(eb('p1').value<<27);"                                    // Get power
    "s|=(d.selectedIndex<<23);"                                   // Get device
    "s|=((h.selectedIndex*60)+m.selectedIndex)&0x7FF;"            // Get time
    "pt[ct]=s;"
    "eb('t0').value=pt.join();"                                   // Save parameters from array to hidden area
  "}"
  "function ot(t,e){"
    "var d,h,i,m,n,s,tl,p,q;"
    "h=qs('#ho');"
    "m=qs('#mi');"
    "d=qs('#d1');"
    "if(ct==99){"                                                 // Do this once
      "pt=eb('t0').value.split(',').map(Number);"                 // Get parameters from hidden area to array
      "for(i=0;i<=23;i++){ce((i<10)?('0'+i):i,h);}"               // Create hours select options
      "for(i=0;i<=59;i++){ce((i<10)?('0'+i):i,m);}"               // Create minutes select options
      "for(i=0;i<}1;i++){ce(i+1,d);}"                             // Create devices
    "}else{"
      "st();"                                                     // Save changes
    "}"
    "tl=document.getElementsByClassName('tl');"                   // Remove the background color of all tablinks/buttons
    "for(i=0;i<tl.length;i++){tl[i].style.cssText=\"background-color:#ccc;color:#fff;font-weight:normal;\"}"
    // Add the specific color to the button used to open the tab content
    "e.style.cssText=\"background-color:#fff;color:#000;font-weight:bold;\";"
    "s=pt[t];"                                                    // Get parameters from array
    "p=s&0x7FF;"                                                  // Get time
    "q=Math.floor(p/60);if(q<10){q='0'+q;}h.value=q;"             // Set hours
    "q=p%60;if(q<10){q='0'+q;}m.value=q;"                         // Set minutes
    "for(i=0;i<7;i++){p=(s>>(16+i))&1;eb('w'+i).checked=p;}"      // Set weekdays
    "p=(s>>23)&0xF;d.value=p+1;"                                  // Set device
    "p=(s>>27)&3;eb('p1').value=p;"                               // Set power
    "p=(s>>29)&1;eb('r0').checked=p;"                             // Set repeat
    "p=(s>>30)&1;eb('a0').checked=p;"                             // Set arm
    "ct=t;"
  "}";
const char HTTP_TIMER_STYLE[] PROGMEM =
  ".tl{float:left;border-radius:0;border:1px solid #fff;padding:1px;width:6.25%;}"
  "</style>";
const char HTTP_FORM_TIMER[] PROGMEM =
  "<fieldset style='min-width:470px;text-align:center;'><legend style='text-align:left;'><b>&nbsp;" D_TIMER_PARAMETERS "&nbsp;</b></legend><form method='post' action='sv'>"
  "<input id='w' name='w' value='7,0' hidden><input id='t0' name='t0' value='";
const char HTTP_FORM_TIMER1[] PROGMEM =
  "</div><br/><br/><br/>"
  "<div>"
  "<b>" D_TIMER_DEVICE "</b>&nbsp;<span><select style='width:12%;' id='d1' name='d1'></select></span>&emsp;"
  "<b>" D_TIMER_POWER "</b>&nbsp;<select style='width:25%;' id='p1' name='p1'>"
    "<option value='0'>" D_OFF "</option>"
    "<option value='1'>" D_ON "</option>"
    "<option value='2'>" D_TOGGLE "</option>"
    "<option value='3'>" D_BLINK "</option>"
  "</select>"
  "</div><br/>"
  "<div>"
//  "<b>Time</b>&nbsp;<input type='time' style='width:25%;' id='s1' name='s1' value='00:00' pattern='[0-9]{2}:[0-9]{2}'>&emsp;"
  "<b>" D_TIMER_TIME "</b>&nbsp;<span><select style='width:12%;' id='ho' name='ho'></select></span>&nbsp;:&nbsp;<span><select style='width:12%;' id='mi' name='mi'></select></span>&emsp;"
  "<input style='width:5%;' id='a0' name='a0' type='checkbox'><b>" D_TIMER_ARM "</b>&emsp;"
  "<input style='width:5%;' id='r0' name='r0' type='checkbox'><b>" D_TIMER_REPEAT "</b>"
  "</div><br/>"
  "<div>";
const char HTTP_FORM_TIMER2[] PROGMEM =
  "type='submit' onclick='st();this.form.submit();'";

const char S_CONFIGURE_TIMER[] PROGMEM = D_CONFIGURE_TIMER;

void HandleTimerConfiguration()
{
  if (HTTP_USER == webserver_state) {
    HandleRoot();
    return;
  }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_TIMER);

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_TIMER));
  page += FPSTR(HTTP_TIMER_SCRIPT);
  page += FPSTR(HTTP_HEAD_STYLE);
  page.replace(F("</style>"), FPSTR(HTTP_TIMER_STYLE));
  page += FPSTR(HTTP_FORM_TIMER);
  for (byte i = 0; i < MAX_TIMERS; i++) {
    if (i > 0) page += F(",");
    page += String(Settings.timer[i].data);
  }
  page += F("' hidden><div>");
  for (byte i = 0; i < MAX_TIMERS; i++) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("<button type='button' class='tl' onclick=\"ot(%d,this)\"%s>%d</button>"),
      i, (0 == i) ? " id='dP'" : "", i +1);
    page += mqtt_data;
  }
  page += FPSTR(HTTP_FORM_TIMER1);
  page.replace(F("}1"), String(devices_present));
  char day[4] = { 0 };
  for (byte i = 0; i < 7; i++) {
    strncpy_P(day, PSTR(D_DAY3LIST) + (i *3), 3);
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("<input style='width:5%%;' id='w%d' name='w%d' type='checkbox'><b>%s</b>"), i, i, day);
    page += mqtt_data;
  }
  page += F("</div>");

  page += FPSTR(HTTP_FORM_END);
  page.replace(F("type='submit'"), FPSTR(HTTP_FORM_TIMER2));
  page += F("<script>eb('dP').click();</script>");  // Get the element with id='defaultOpen' and click on it
  page += FPSTR(HTTP_BTN_CONF);
  ShowPage(page);
}

void TimerSaveSettings()
{
  char tmp[MAX_TIMERS *12];  // Need space for MAX_TIMERS x 10 digit numbers separated by a comma

  WebGetArg("t0", tmp, sizeof(tmp));
  char *p = tmp;
  for (byte i = 0; i < MAX_TIMERS; i++) {
    uint32_t data = strtol(p, &p, 10);
    p++;  // Skip comma
    if ((data & 0x7FF) < 1440) Settings.timer[i].data = data;
  }
}
#endif  // USE_TIMERS_WEB
#endif  // USE_WEBSERVER

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