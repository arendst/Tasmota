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
 * {"Arm":1,"Mode":0,"Time":"09:23","Window":0,"Days":"--TW--S","Repeat":1,"Output":1,"Action":1}
 *
 * Arm     0 = Off, 1 = On
 * Mode    0 = Schedule, 1 = Sunrise, 2 = Sunset
 * Time    hours:minutes
 * Window  minutes (0..15)
 * Days    7 day character mask starting with Sunday (SMTWTFS). 0 or - = Off, any other value = On
 * Repeat  0 = Execute once, 1 = Execute again
 * Output  1..16
 * Action  0 = Off, 1 = On, 2 = Toggle, 3 = Blink or Rule if USE_RULES enabled
 *
\*********************************************************************************************/

#define XDRV_09             9

enum TimerCommands { CMND_TIMER, CMND_TIMERS
#ifdef USE_SUNRISE
, CMND_LATITUDE, CMND_LONGITUDE
#endif
 };
const char kTimerCommands[] PROGMEM = D_CMND_TIMER "|" D_CMND_TIMERS
#ifdef USE_SUNRISE
"|" D_CMND_LATITUDE "|" D_CMND_LONGITUDE
#endif
;

uint16_t timer_last_minute = 60;
int8_t timer_window[MAX_TIMERS] = { 0 };

#ifdef USE_SUNRISE
/*********************************************************************************************\
 * Sunrise and sunset (+13k code)
 *
 * https://forum.arduino.cc/index.php?topic=218280.0
 * Source: C-Programm von http://lexikon.astronomie.info/zeitgleichung/neu.html
 *         Rewrite for Arduino by 'jurs' for German Arduino forum
\*********************************************************************************************/

const double pi2 = TWO_PI;
const double pi = PI;
const double RAD = DEG_TO_RAD;

double JulianischesDatum(void)
{
  // Gregorianischer Kalender
  int Gregor;
  int Jahr = RtcTime.year;
  int Monat = RtcTime.month;
  int Tag = RtcTime.day_of_month;

  if (Monat <= 2) {
    Monat += 12;
    Jahr -= 1;
  }
  Gregor = (Jahr / 400) - (Jahr / 100) + (Jahr / 4);  // Gregorianischer Kalender
  return 2400000.5 + 365.0*Jahr - 679004.0 + Gregor + (int)(30.6001 * (Monat +1)) + Tag + 0.5;
}

double InPi(double x)
{
  int n = (int)(x / pi2);
  x = x - n*pi2;
  if (x < 0) x += pi2;
  return x;
}

double eps(double T)
{
  // Neigung der Erdachse
  return RAD * (23.43929111 + (-46.8150*T - 0.00059*T*T + 0.001813*T*T*T)/3600.0);
}

double BerechneZeitgleichung(double *DK,double T)
{
  double RA_Mittel = 18.71506921 + 2400.0513369*T +(2.5862e-5 - 1.72e-9*T)*T*T;
  double M = InPi(pi2 * (0.993133 + 99.997361*T));
  double L = InPi(pi2 * (0.7859453 + M/pi2 + (6893.0*sin(M)+72.0*sin(2.0*M)+6191.2*T) / 1296.0e3));
  double e = eps(T);
  double RA = atan(tan(L)*cos(e));
  if (RA < 0.0) RA += pi;
  if (L > pi) RA += pi;
  RA = 24.0*RA/pi2;
  *DK = asin(sin(e)*sin(L));
  // Damit 0<=RA_Mittel<24
  RA_Mittel = 24.0 * InPi(pi2*RA_Mittel/24.0)/pi2;
  double dRA = RA_Mittel - RA;
  if (dRA < -12.0) dRA += 24.0;
  if (dRA > 12.0) dRA -= 24.0;
  dRA = dRA * 1.0027379;
  return dRA;
}

void DuskTillDawn(uint8_t *hour_up,uint8_t *minute_up, uint8_t *hour_down, uint8_t *minute_down)
{
  double JD2000 = 2451545.0;
  double JD = JulianischesDatum();
  double T = (JD - JD2000) / 36525.0;
  double DK;
  /*
  h (D) = -0.8333 normaler SA & SU-Gang
  h (D) = -6.0 civile Dämmerung
  h (D) = -12.0 nautische Dämmerung
  h (D) = -18.0 astronomische Dämmerung
  */
//  double h = -50/60.0*RAD;
  double h = SUNRISE_DAWN_ANGLE *RAD;
  double B = (((double)Settings.latitude)/1000000) * RAD; // geographische Breite
  double GeographischeLaenge = ((double)Settings.longitude)/1000000;
//  double Zeitzone = 0; //Weltzeit
//  double Zeitzone = 1; //Winterzeit
//  double Zeitzone = 2.0;   //Sommerzeit
  double Zeitzone = ((double)time_timezone) / 60;
  double Zeitgleichung = BerechneZeitgleichung(&DK, T);
  double Minuten = Zeitgleichung * 60.0;
  double Zeitdifferenz = 12.0*acos((sin(h) - sin(B)*sin(DK)) / (cos(B)*cos(DK)))/pi;
  double AufgangOrtszeit = 12.0 - Zeitdifferenz - Zeitgleichung;
  double UntergangOrtszeit = 12.0 + Zeitdifferenz - Zeitgleichung;
  double AufgangWeltzeit = AufgangOrtszeit - GeographischeLaenge / 15.0;
  double UntergangWeltzeit = UntergangOrtszeit - GeographischeLaenge / 15.0;
  double Aufgang = AufgangWeltzeit + Zeitzone;         // In Stunden
  if (Aufgang < 0.0) {
    Aufgang += 24.0;
  } else {
    if (Aufgang >= 24.0) Aufgang -= 24.0;
  }
  double Untergang = UntergangWeltzeit + Zeitzone;
  if (Untergang < 0.0) {
    Untergang += 24.0;
  } else {
    if (Untergang >= 24.0) Untergang -= 24.0;
  }
  int AufgangMinuten = (int)(60.0*(Aufgang - (int)Aufgang)+0.5);
  int AufgangStunden = (int)Aufgang;
  if (AufgangMinuten >= 60.0) {
    AufgangMinuten -= 60.0;
    AufgangStunden++;
  } else {
    if (AufgangMinuten < 0.0) {
      AufgangMinuten += 60.0;
      AufgangStunden--;
      if (AufgangStunden < 0.0) AufgangStunden += 24.0;
    }
  }
  int UntergangMinuten = (int)(60.0*(Untergang - (int)Untergang)+0.5);
  int UntergangStunden = (int)Untergang;
  if (UntergangMinuten >= 60.0) {
    UntergangMinuten -= 60.0;
    UntergangStunden++;
  } else {
    if (UntergangMinuten<0) {
      UntergangMinuten += 60.0;
      UntergangStunden--;
      if (UntergangStunden < 0.0) UntergangStunden += 24.0;
    }
  }
  *hour_up = AufgangStunden;
  *minute_up = AufgangMinuten;
  *hour_down = UntergangStunden;
  *minute_down = UntergangMinuten;
}

void ApplyTimerOffsets(Timer *duskdawn)
{
  uint8_t hour[2];
  uint8_t minute[2];
  Timer stored = (Timer)*duskdawn;

  // replace hours, minutes by sunrise
  DuskTillDawn(&hour[0], &minute[0], &hour[1], &minute[1]);
  uint8_t mode = (duskdawn->mode -1) &1;
  duskdawn->time = (hour[mode] *60) + minute[mode];

  // apply offsets, check for over- and underflows
  uint16_t timeBuffer;
  if ((uint16_t)stored.time > 719) {
    // negative offset, time after 12:00
    timeBuffer = (uint16_t)stored.time - 720;
    // check for underflow
    if (timeBuffer > (uint16_t)duskdawn->time) {
      timeBuffer = 1440 - (timeBuffer - (uint16_t)duskdawn->time);
      duskdawn->days = duskdawn->days >> 1;
      duskdawn->days |= (stored.days << 6);
    } else {
      timeBuffer = (uint16_t)duskdawn->time - timeBuffer;
    }
  } else {
    // positive offset
    timeBuffer = (uint16_t)duskdawn->time + (uint16_t)stored.time;
    // check for overflow
    if (timeBuffer > 1440) {
      timeBuffer -= 1440;
      duskdawn->days = duskdawn->days << 1;
      duskdawn->days |= (stored.days >> 6);
    }
  }
  duskdawn->time = timeBuffer;
}

String GetSun(byte dawn)
{
  char stime[6];

  uint8_t hour[2];
  uint8_t minute[2];

  DuskTillDawn(&hour[0], &minute[0], &hour[1], &minute[1]);
  dawn &= 1;
  snprintf_P(stime, sizeof(stime), PSTR("%02d:%02d"), hour[dawn], minute[dawn]);
  return String(stime);
}

uint16_t GetSunMinutes(byte dawn)
{
  uint8_t hour[2];
  uint8_t minute[2];

  DuskTillDawn(&hour[0], &minute[0], &hour[1], &minute[1]);
  dawn &= 1;
  return (hour[dawn] *60) + minute[dawn];
}

#endif  // USE_SUNRISE

/*******************************************************************************************/

void TimerSetRandomWindow(byte index)
{
  timer_window[index] = 0;
  if (Settings.timer[index].window) {
    timer_window[index] = (random(0, (Settings.timer[index].window << 1) +1)) - Settings.timer[index].window;  // -15 .. 15
  }
}

void TimerSetRandomWindows(void)
{
  for (byte i = 0; i < MAX_TIMERS; i++) { TimerSetRandomWindow(i); }
}

void TimerEverySecond(void)
{
  if (RtcTime.valid) {
    if (!RtcTime.hour && !RtcTime.minute && !RtcTime.second) { TimerSetRandomWindows(); }  // Midnight
    if (Settings.flag3.timers_enable && (uptime > 60) && (RtcTime.minute != timer_last_minute)) {  // Execute from one minute after restart every minute only once
      timer_last_minute = RtcTime.minute;
      int16_t time = (RtcTime.hour *60) + RtcTime.minute;
      uint8_t days = 1 << (RtcTime.day_of_week -1);

      for (byte i = 0; i < MAX_TIMERS; i++) {
//        if (Settings.timer[i].device >= devices_present) Settings.timer[i].data = 0;  // Reset timer due to change in devices present
        Timer xtimer = Settings.timer[i];
        uint16_t set_time = xtimer.time;
#ifdef USE_SUNRISE
        if ((1 == xtimer.mode) || (2 == xtimer.mode)) {  // Sunrise or Sunset
          ApplyTimerOffsets(&xtimer);
          set_time = xtimer.time;
        }
#endif
        if (xtimer.arm) {
          set_time += timer_window[i];                // Add random time offset
          if (set_time < 0) { set_time = 0; }         // Stay today;
          if (set_time > 1439) { set_time = 1439; }
          if (time == set_time) {
            if (xtimer.days & days) {
              Settings.timer[i].arm = xtimer.repeat;
#ifdef USE_RULES
              if (3 == xtimer.power) {  // Blink becomes Rule disregarding device and allowing use of Backlog commands
                snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"Clock\":{\"Timer\":%d}}"), i +1);
                XdrvRulesProcess();
              } else
#endif  // USE_RULES
                if (devices_present) { ExecuteCommandPower(xtimer.device +1, xtimer.power, SRC_TIMER); }
            }
          }
        }
      }
    }
  }
}

void PrepShowTimer(uint8_t index)
{
  char days[8] = { 0 };
  char sign[2] = { 0 };
  char soutput[80];

  Timer xtimer = Settings.timer[index -1];

  for (byte i = 0; i < 7; i++) {
    uint8_t mask = 1 << i;
    snprintf(days, sizeof(days), "%s%d", days, ((xtimer.days & mask) > 0));
  }

  soutput[0] = '\0';
  if (devices_present) {
    snprintf_P(soutput, sizeof(soutput), PSTR(",\"" D_JSON_TIMER_OUTPUT "\":%d"), xtimer.device +1);
  }
#ifdef USE_SUNRISE
  int16_t hour = xtimer.time / 60;
  if ((1 == xtimer.mode) || (2 == xtimer.mode)) {  // Sunrise or Sunset
    if (hour > 11) {
      hour -= 12;
      sign[0] = '-';
    }
  }
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s\"" D_CMND_TIMER "%d\":{\"" D_JSON_TIMER_ARM "\":%d,\"" D_JSON_TIMER_MODE "\":%d,\"" D_JSON_TIMER_TIME "\":\"%s%02d:%02d\",\"" D_JSON_TIMER_WINDOW "\":%d,\"" D_JSON_TIMER_DAYS "\":\"%s\",\"" D_JSON_TIMER_REPEAT "\":%d%s,\"" D_JSON_TIMER_ACTION "\":%d}"),
    mqtt_data, index, xtimer.arm, xtimer.mode, sign, hour, xtimer.time % 60, xtimer.window, days, xtimer.repeat, soutput, xtimer.power);
#else
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s\"" D_CMND_TIMER "%d\":{\"" D_JSON_TIMER_ARM "\":%d,\"" D_JSON_TIMER_TIME "\":\"%02d:%02d\",\"" D_JSON_TIMER_WINDOW "\":%d,\"" D_JSON_TIMER_DAYS "\":\"%s\",\"" D_JSON_TIMER_REPEAT "\":%d%s,\"" D_JSON_TIMER_ACTION "\":%d}"),
    mqtt_data, index, xtimer.arm, xtimer.time / 60, xtimer.time % 60, xtimer.window, days, xtimer.repeat, soutput, xtimer.power);
#endif  // USE_SUNRISE
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

boolean TimerCommand(void)
{
  char command[CMDSZ];
  char dataBufUc[XdrvMailbox.data_len];
  boolean serviced = true;
  uint8_t index = XdrvMailbox.index;

  UpperCase(dataBufUc, XdrvMailbox.data);
  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kTimerCommands);
  if (-1 == command_code) {
    serviced = false;  // Unknown command
  }
  else if ((CMND_TIMER == command_code) && (index > 0) && (index <= MAX_TIMERS)) {
    uint8_t error = 0;
    if (XdrvMailbox.data_len) {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= MAX_TIMERS)) {
        if (XdrvMailbox.payload == 0) {
          Settings.timer[index -1].data = 0;  // Clear timer
        } else {
          Settings.timer[index -1].data = Settings.timer[XdrvMailbox.payload -1].data;  // Copy timer
        }
      } else {
#ifndef USE_RULES
        if (devices_present) {
#endif
          StaticJsonBuffer<256> jsonBuffer;
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
#ifdef USE_SUNRISE
            if (root[UpperCase_P(parm_uc, PSTR(D_JSON_TIMER_MODE))].success()) {
              Settings.timer[index].mode = (uint8_t)root[parm_uc] & 0x03;
            }
#endif
            if (root[UpperCase_P(parm_uc, PSTR(D_JSON_TIMER_TIME))].success()) {
              uint16_t itime = 0;
              int8_t value = 0;
              uint8_t sign = 0;
              char time_str[10];

              snprintf(time_str, sizeof(time_str), root[parm_uc]);
              const char *substr = strtok(time_str, ":");
              if (substr != NULL) {
                if (strchr(substr, '-')) {
                  sign = 1;
                  substr++;
                }
                value = atoi(substr);
                if (sign) { value += 12; }  // Allow entering timer offset from -11:59 to -00:01 converted to 12:01 to 23:59
                if (value > 23) { value = 23; }
                itime = value * 60;
                substr = strtok(NULL, ":");
                if (substr != NULL) {
                  value = atoi(substr);
                  if (value < 0) { value = 0; }
                  if (value > 59) { value = 59; }
                  itime += value;
                }
              }
              Settings.timer[index].time = itime;
            }
            if (root[UpperCase_P(parm_uc, PSTR(D_JSON_TIMER_WINDOW))].success()) {
              Settings.timer[index].window = (uint8_t)root[parm_uc] & 0x0F;
              TimerSetRandomWindow(index);
            }
            if (root[UpperCase_P(parm_uc, PSTR(D_JSON_TIMER_DAYS))].success()) {
              // SMTWTFS = 1234567 = 0011001 = 00TW00S = --TW--S
              Settings.timer[index].days = 0;
              const char *tday = root[parm_uc];
              uint8_t i = 0;
              char ch = *tday++;
              while ((ch != '\0') && (i < 7)) {
                if (ch == '-') { ch = '0'; }
                uint8_t mask = 1 << i++;
                Settings.timer[index].days |= (ch == '0') ? 0 : mask;
                ch = *tday++;
              }
            }
            if (root[UpperCase_P(parm_uc, PSTR(D_JSON_TIMER_REPEAT))].success()) {
              Settings.timer[index].repeat = (root[parm_uc] != 0);
            }
            if (root[UpperCase_P(parm_uc, PSTR(D_JSON_TIMER_OUTPUT))].success()) {
              uint8_t device = ((uint8_t)root[parm_uc] -1) & 0x0F;
              Settings.timer[index].device = (device < devices_present) ? device : 0;
            }
            if (root[UpperCase_P(parm_uc, PSTR(D_JSON_TIMER_ACTION))].success()) {
              uint8_t action = (uint8_t)root[parm_uc] & 0x03;
              Settings.timer[index].power = (devices_present) ? action : 3;  // If no devices than only allow rules
            }

            index++;
          }
#ifndef USE_RULES
        } else {
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_TIMER "%d\":\"" D_JSON_TIMER_NO_DEVICE "\"}"), index);  // No outputs defined so nothing to control
          error = 1;
        }
#endif
      }
    }
    if (!error) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{"));
      PrepShowTimer(index);
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
    }
  }
  else if (CMND_TIMERS == command_code) {
    if (XdrvMailbox.data_len) {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
        Settings.flag3.timers_enable = XdrvMailbox.payload;
      }
      if (XdrvMailbox.payload == 2) {
        Settings.flag3.timers_enable = !Settings.flag3.timers_enable;
      }
    }

    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, GetStateText(Settings.flag3.timers_enable));
    MqttPublishPrefixTopic_P(RESULT_OR_STAT, command);

    byte jsflg = 0;
    byte lines = 1;
    for (byte i = 0; i < MAX_TIMERS; i++) {
      if (!jsflg) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_TIMERS "%d\":{"), lines++);
      } else {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,"), mqtt_data);
      }
      jsflg++;
      PrepShowTimer(i +1);
      if (jsflg > 3) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}}"), mqtt_data);
        MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_TIMERS));
        jsflg = 0;
      }
    }
    mqtt_data[0] = '\0';
  }
#ifdef USE_SUNRISE
  else if (CMND_LONGITUDE == command_code) {
    if (XdrvMailbox.data_len) {
      Settings.longitude = (int)(CharToDouble(XdrvMailbox.data) *1000000);
    }
    char lbuff[33];
    dtostrfd(((double)Settings.longitude) /1000000, 6, lbuff);
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, lbuff);
  }
  else if (CMND_LATITUDE == command_code) {
    if (XdrvMailbox.data_len) {
      Settings.latitude = (int)(CharToDouble(XdrvMailbox.data) *1000000);
    }
    char lbuff[33];
    dtostrfd(((double)Settings.latitude) /1000000, 6, lbuff);
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, lbuff);
  }
#endif
  else serviced = false;  // Unknown command

  return serviced;
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
#ifdef USE_TIMERS_WEB

#define WEB_HANDLE_TIMER "tm"

const char S_CONFIGURE_TIMER[] PROGMEM = D_CONFIGURE_TIMER;

const char HTTP_BTN_MENU_TIMER[] PROGMEM =
  "<br/><form action='" WEB_HANDLE_TIMER "' method='get'><button>" D_CONFIGURE_TIMER "</button></form>";

const char HTTP_TIMER_SCRIPT[] PROGMEM =
  "var pt=[],ct=99;"
  "function qs(s){"                                               // Alias to save code space
    "return document.querySelector(s);"
  "}"
  "function ce(i,q){"                                             // Create select option
    "var o=document.createElement('option');"
    "o.textContent=i;"
    "q.appendChild(o);"
  "}"
#ifdef USE_SUNRISE
  "function gt(){"                                                // Set hours and minutes according to mode
    "var m,p,q;"
    "m=qs('input[name=\"rd\"]:checked').value;"                   // Get mode
    "p=pt[ct]&0x7FF;"                                             // Get time
    "if(m==0){"                                                   // Time is set
      "so(0);"                                                    // Hide offset span and allow Hour 00..23
      "q=Math.floor(p/60);if(q<10){q='0'+q;}qs('#ho').value=q;"   // Set hours
      "q=p%60;if(q<10){q='0'+q;}qs('#mi').value=q;"               // Set minutes
    "}"
    "if((m==1)||(m==2)){"                                         // Sunrise or sunset is set
      "so(1);"                                                    // Show offset span and allow Hour 00..11
      "q=Math.floor(p/60);"                                       // Parse hours
      "if(q>=12){q-=12;qs('#dr').selectedIndex=1;}"               // Negative offset
        "else{qs('#dr').selectedIndex=0;}"
      "if(q<10){q='0'+q;}qs('#ho').value=q;"                      // Set offset hours
      "q=p%60;if(q<10){q='0'+q;}qs('#mi').value=q;"               // Set offset minutes
    "}"
  "}"
  "function so(b){"                                               // Hide or show offset items
    "o=qs('#ho');"
    "e=o.childElementCount;"
    "if(b==1){"
      "qs('#dr').disabled='';"
      "if(e>12){for(i=12;i<=23;i++){o.removeChild(o.lastElementChild);}}"  // Create offset hours select options
    "}else{"
      "qs('#dr').disabled='disabled';"
      "if(e<23){for(i=12;i<=23;i++){ce(i,o);}}"                   // Create hours select options
    "}"
  "}"
#endif
  "function st(){"                                                // Save parameters to hidden area
    "var i,l,m,n,p,s;"
    "m=0;s=0;"
    "n=1<<31;if(eb('a0').checked){s|=n;}"                         // Get arm
    "n=1<<15;if(eb('r0').checked){s|=n;}"                         // Get repeat
    "for(i=0;i<7;i++){n=1<<(16+i);if(eb('w'+i).checked){s|=n;}}"  // Get weekdays
#ifdef USE_SUNRISE
    "m=qs('input[name=\"rd\"]:checked').value;"                   // Check mode
    "s|=(qs('input[name=\"rd\"]:checked').value<<29);"            // Get mode
#endif
    "if(}1>0){"
      "i=qs('#d1').selectedIndex;if(i>=0){s|=(i<<23);}"           // Get output
      "s|=(qs('#p1').selectedIndex<<27);"                         // Get action
    "}else{"
      "s|=3<<27;"                                                 // Get action (rule)
    "}"
    "l=((qs('#ho').selectedIndex*60)+qs('#mi').selectedIndex)&0x7FF;"
    "if(m==0){s|=l;}"                                             // Get time
#ifdef USE_SUNRISE
    "if((m==1)||(m==2)){"
      "if(qs('#dr').selectedIndex>0){l+=720;}"                    // If negative offset, add 12h to given offset time
      "s|=l&0x7FF;"                                               // Save offset instead of time
    "}"
#endif
    "s|=((qs('#mw').selectedIndex)&0x0F)<<11;"                    // Get window minutes
    "pt[ct]=s;"
    "eb('t0').value=pt.join();"                                   // Save parameters from array to hidden area
  "}"
  "function ot(t,e){"                                             // Select tab and update elements
    "var i,n,o,p,q,s;"
    "if(ct<99){st();}"                                            // Save changes
    "ct=t;"
    "o=document.getElementsByClassName('tl');"                    // Restore style to all tabs/buttons
    "for(i=0;i<o.length;i++){o[i].style.cssText=\"background-color:#ccc;color:#fff;font-weight:normal;\"}"
    "e.style.cssText=\"background-color:#fff;color:#000;font-weight:bold;\";"  // Change style to tab/button used to open content
    "s=pt[ct];"                                                   // Get parameters from array
#ifdef USE_SUNRISE
    "p=(s>>29)&3;eb('b'+p).checked=1;"                            // Set mode
    "gt();"                                                       // Set hours and minutes according to mode
#else
    "p=s&0x7FF;"                                                  // Get time
    "q=Math.floor(p/60);if(q<10){q='0'+q;}qs('#ho').value=q;"     // Set hours
    "q=p%60;if(q<10){q='0'+q;}qs('#mi').value=q;"                 // Set minutes
#endif
    "q=(s>>11)&0xF;if(q<10){q='0'+q;}qs('#mw').value=q;"          // Set window minutes
    "for(i=0;i<7;i++){p=(s>>(16+i))&1;eb('w'+i).checked=p;}"      // Set weekdays
    "if(}1>0){"
      "p=(s>>23)&0xF;qs('#d1').value=p+1;"                        // Set output
      "p=(s>>27)&3;qs('#p1').selectedIndex=p;"                    // Set action
    "}"
    "p=(s>>15)&1;eb('r0').checked=p;"                             // Set repeat
    "p=(s>>31)&1;eb('a0').checked=p;"                             // Set arm
  "}"
  "function it(){"                                                // Initialize elements and select first tab
    "var b,i,o,s;"
    "pt=eb('t0').value.split(',').map(Number);"                   // Get parameters from hidden area to array
    "s='';for(i=0;i<" STR(MAX_TIMERS) ";i++){b='';if(0==i){b=\" id='dP'\";}s+=\"<button type='button' class='tl' onclick='ot(\"+i+\",this)'\"+b+\">\"+(i+1)+\"</button>\"}"
    "eb('bt').innerHTML=s;"                                       // Create tabs
    "if(}1>0){"                                                   // Create Output and Action drop down boxes
      "eb('oa').innerHTML=\"<b>" D_TIMER_OUTPUT "</b>&nbsp;<span><select style='width:60px;' id='d1' name='d1'></select></span>&emsp;<b>" D_TIMER_ACTION "</b>&nbsp;<select style='width:99px;' id='p1' name='p1'></select>\";"
      "o=qs('#p1');ce('" D_OFF "',o);ce('" D_ON "',o);ce('" D_TOGGLE "',o);"  // Create offset direction select options
#ifdef USE_RULES
      "ce('" D_RULE "',o);"
#else
      "ce('" D_BLINK "',o);"
#endif
    "}else{"
      "eb('oa').innerHTML=\"<b>" D_TIMER_ACTION "</b> " D_RULE "\";"  // No outputs but rule is allowed
    "}"
#ifdef USE_SUNRISE
    "o=qs('#dr');ce('+',o);ce('-',o);"                            // Create offset direction select options
#endif
    "o=qs('#ho');for(i=0;i<=23;i++){ce((i<10)?('0'+i):i,o);}"     // Create hours select options
    "o=qs('#mi');for(i=0;i<=59;i++){ce((i<10)?('0'+i):i,o);}"     // Create minutes select options
    "o=qs('#mw');for(i=0;i<=15;i++){ce((i<10)?('0'+i):i,o);}"     // Create window minutes select options
    "o=qs('#d1');for(i=0;i<}1;i++){ce(i+1,o);}"                   // Create outputs
    "var a='" D_DAY3LIST "';"
    "s='';for(i=0;i<7;i++){s+=\"<input style='width:5%;' id='w\"+i+\"' name='w\"+i+\"' type='checkbox'><b>\"+a.substring(i*3,(i*3)+3)+\"</b>\"}"
    "eb('ds').innerHTML=s;"                                       // Create weekdays
    "eb('dP').click();"                                           // Get the element with id='dP' and click on it
  "}";
const char HTTP_TIMER_STYLE[] PROGMEM =
  ".tl{float:left;border-radius:0;border:1px solid #fff;padding:1px;width:6.25%;}"
#ifdef USE_SUNRISE
  "input[type='radio']{width:13px;height:24px;margin-top:-1px;margin-right:8px;vertical-align:middle;}"
#endif
  "</style>";
const char HTTP_FORM_TIMER[] PROGMEM =
  "<fieldset style='min-width:470px;text-align:center;'>"
  "<legend style='text-align:left;'><b>&nbsp;" D_TIMER_PARAMETERS "&nbsp;</b></legend>"
  "<form method='post' action='" WEB_HANDLE_TIMER "' onsubmit='return st();'>"
  "<br/><input style='width:5%;' id='e0' name='e0' type='checkbox'{e0><b>" D_TIMER_ENABLE "</b><br/><br/><hr/>"
  "<input id='t0' name='t0' value='";
const char HTTP_FORM_TIMER1[] PROGMEM =
  "' hidden><div id='bt' name='bt'></div><br/><br/><br/>"
  "<div id='oa' name='oa'></div><br/>"
  "<div>"
  "<input style='width:5%;' id='a0' name='a0' type='checkbox'><b>" D_TIMER_ARM "</b>&emsp;"
  "<input style='width:5%;' id='r0' name='r0' type='checkbox'><b>" D_TIMER_REPEAT "</b>"
  "</div><br/>"
  "<div>"
#ifdef USE_SUNRISE
  "<fieldset style='width:299px;margin:auto;text-align:left;border:0;'>"
  "<input id='b0' name='rd' type='radio' value='0' onclick='gt();'><b>" D_TIMER_TIME "</b><br/>"
  "<input id='b1' name='rd' type='radio' value='1' onclick='gt();'><b>" D_SUNRISE "</b> (}8)<br/>"
  "<input id='b2' name='rd' type='radio' value='2' onclick='gt();'><b>" D_SUNSET "</b> (}9)<br/>"
  "</fieldset>"
  "<span><select style='width:46px;' id='dr' name='dr'></select></span>"
  "&nbsp;"
#else
  "<b>" D_TIMER_TIME "</b>&nbsp;"
#endif  // USE_SUNRISE
  "<span><select style='width:60px;' id='ho' name='ho'></select></span>"
  "&nbsp;" D_HOUR_MINUTE_SEPARATOR "&nbsp;"
  "<span><select style='width:60px;' id='mi' name='mi'></select></span>"
  "&emsp;<b>+/-</b>&nbsp;"
  "<span><select style='width:60px;' id='mw' name='mw'></select></span>"
  "</div><br/>"
  "<div id='ds' name='ds'></div>";

void HandleTimerConfiguration(void)
{
  if (HttpUser()) { return; }
  if (!WebAuthenticate()) { return WebServer->requestAuthentication(); }
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_TIMER);

  if (WebServer->hasArg("save")) {
    TimerSaveSettings();
    HandleConfiguration();
    return;
  }

  String page = FPSTR(HTTP_HEAD);
  page.replace(F("{v}"), FPSTR(S_CONFIGURE_TIMER));
  page += FPSTR(HTTP_TIMER_SCRIPT);
  page += FPSTR(HTTP_HEAD_STYLE);
  page.replace(F("</style>"), FPSTR(HTTP_TIMER_STYLE));
  page += FPSTR(HTTP_FORM_TIMER);
  page.replace(F("{e0"), (Settings.flag3.timers_enable) ? F(" checked") : F(""));
  for (byte i = 0; i < MAX_TIMERS; i++) {
    if (i > 0) { page += F(","); }
    page += String(Settings.timer[i].data);
  }
  page += FPSTR(HTTP_FORM_TIMER1);
  page.replace(F("}1"), String(devices_present));
#ifdef USE_SUNRISE
  page.replace(F("}8"), GetSun(0));  // Add Sunrise
  page.replace(F("}9"), GetSun(1));  // Add Sunset
  page.replace(F("299"), String(100 + (strlen(D_SUNSET) *12)));  // Fix string length to keep radios centered
#endif  // USE_SUNRISE
  page += FPSTR(HTTP_FORM_END);
  page += F("<script>it();</script>");  // Init elements and select first tab/button
  page += FPSTR(HTTP_BTN_CONF);
  ShowPage(page);
}

void TimerSaveSettings(void)
{
  char tmp[MAX_TIMERS *12];  // Need space for MAX_TIMERS x 10 digit numbers separated by a comma
  Timer timer;

  Settings.flag3.timers_enable = WebServer->hasArg("e0");
  WebGetArg("t0", tmp, sizeof(tmp));
  char *p = tmp;
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MQTT D_CMND_TIMERS " %d"), Settings.flag3.timers_enable);
  for (byte i = 0; i < MAX_TIMERS; i++) {
    timer.data = strtol(p, &p, 10);
    p++;  // Skip comma
    if (timer.time < 1440) {
      bool flag = (timer.window != Settings.timer[i].window);
      Settings.timer[i].data = timer.data;
      if (flag) TimerSetRandomWindow(i);
    }
    snprintf_P(log_data, sizeof(log_data), PSTR("%s,0x%08X"), log_data, Settings.timer[i].data);
  }
  AddLog(LOG_LEVEL_DEBUG);
}
#endif  // USE_TIMERS_WEB
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xdrv09(byte function)
{
  boolean result = false;

  switch (function) {
    case FUNC_PRE_INIT:
      TimerSetRandomWindows();
      break;
#ifdef USE_WEBSERVER
#ifdef USE_TIMERS_WEB
    case FUNC_WEB_ADD_BUTTON:
#ifdef USE_RULES
      strncat_P(mqtt_data, HTTP_BTN_MENU_TIMER, sizeof(mqtt_data) - strlen(mqtt_data) -1);
#else
      if (devices_present) { strncat_P(mqtt_data, HTTP_BTN_MENU_TIMER, sizeof(mqtt_data) - strlen(mqtt_data) -1); }
#endif  // USE_RULES
      break;
    case FUNC_WEB_ADD_HANDLER:
      WebServer->on("/" WEB_HANDLE_TIMER, HandleTimerConfiguration);
      break;
#endif  // USE_TIMERS_WEB
#endif  // USE_WEBSERVER
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
