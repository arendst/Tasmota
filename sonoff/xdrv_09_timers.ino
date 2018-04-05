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
 * {"Arm":1,"Mode":0,"Time":"09:23","Days":"--TW--S","Repeat":1,"Device":1,"Power":1}
 *
 * Arm    0 = Off, 1 = On
 * Mode   0 = Schedule, 1 = Sunrise, 2 = Sunset
 * Time   hours:minutes
 * Days   7 day character mask starting with Sunday (SMTWTFS). 0 or - = Off, any other value = On
 * Repeat 0 = Execute once, 1 = Execute again
 * Device 1..16
 * Power  0 = Off, 1 = On, 2 = Toggle, 3 = Blink
 *
\*********************************************************************************************/

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

uint16_t timer_fired = 0;
uint16_t timer_last_minute = 60;

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

double JulianischesDatum()
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
  double h = -50/60.0*RAD;
  double B = ((double)Settings.latitude/1000000) * RAD; // geographische Breite
  double GeographischeLaenge = (double)Settings.longitude/1000000;
  double Zeitzone = (double)time_timezone / 10;
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

void TimerEverySecond()
{
  if (RtcTime.valid) {
    if (RtcTime.minute != timer_last_minute) {  // Execute every minute
      timer_last_minute = RtcTime.minute;
      uint16_t time = (RtcTime.hour *60) + RtcTime.minute;
      uint8_t days = 1 << (RtcTime.day_of_week -1);

      for (byte i = 0; i < MAX_TIMERS; i++) {
        if (Settings.timer[i].device >= devices_present) Settings.timer[i].data = 0;  // Reset timer due to change in devices present
        uint16_t set_time = Settings.timer[i].time;
#ifdef USE_SUNRISE
        if ((1 == Settings.timer[i].mode) || (2 == Settings.timer[i].mode)) {  // Sunrise or Sunset
          set_time = GetSunMinutes(Settings.timer[i].mode -1);
        }
#endif
        if (Settings.timer[i].arm) {
          if (time == set_time) {
            if (!bitRead(timer_fired, i) && (Settings.timer[i].days & days)) {
              bitSet(timer_fired, i);
              Settings.timer[i].arm = Settings.timer[i].repeat;
              ExecuteCommandPower(Settings.timer[i].device +1, Settings.timer[i].power);
            }
          } else {
            bitClear(timer_fired, i);
          }
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
#ifdef USE_SUNRISE
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s\"" D_CMND_TIMER "%d\":{\"" D_JSON_TIMER_ARM "\":%d,\"" D_JSON_TIMER_MODE "\":%d,\"" D_JSON_TIMER_TIME "\":\"%02d:%02d\",\"" D_JSON_TIMER_DAYS "\":\"%s\",\"" D_JSON_TIMER_REPEAT "\":%d,\"" D_JSON_TIMER_OUTPUT "\":%d,\"" D_JSON_TIMER_POWER "\":%d}"),
    mqtt_data, index +1, Settings.timer[index].arm, Settings.timer[index].mode, Settings.timer[index].time / 60, Settings.timer[index].time % 60, days, Settings.timer[index].repeat, Settings.timer[index].device +1, Settings.timer[index].power);
#else
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s\"" D_CMND_TIMER "%d\":{\"" D_JSON_TIMER_ARM "\":%d,\"" D_JSON_TIMER_TIME "\":\"%02d:%02d\",\"" D_JSON_TIMER_DAYS "\":\"%s\",\"" D_JSON_TIMER_REPEAT "\":%d,\"" D_JSON_TIMER_OUTPUT "\":%d,\"" D_JSON_TIMER_POWER "\":%d}"),
    mqtt_data, index +1, Settings.timer[index].arm, Settings.timer[index].time / 60, Settings.timer[index].time % 60, days, Settings.timer[index].repeat, Settings.timer[index].device +1, Settings.timer[index].power);
#endif  // USE_SUNRISE
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
        if (devices_present) {
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
#ifdef USE_SUNRISE
            if (root[UpperCase_P(parm_uc, PSTR(D_JSON_TIMER_MODE))].success()) {
              Settings.timer[index].mode = (uint8_t)root[parm_uc] & 0x03;
            }
#endif
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
            if (root[UpperCase_P(parm_uc, PSTR(D_JSON_TIMER_OUTPUT))].success()) {
              uint8_t device = ((uint8_t)root[parm_uc] -1) & 0x0F;
              Settings.timer[index].device = (device < devices_present) ? device : devices_present -1;
            }
            if (root[UpperCase_P(parm_uc, PSTR(D_JSON_TIMER_POWER))].success()) {
              Settings.timer[index].power = (uint8_t)root[parm_uc] & 0x03;
            }
            if (Settings.timer[index].arm) bitClear(timer_fired, index);

            index++;
          }
        } else {
          snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"" D_CMND_TIMER "%d\":\"" D_JSON_TIMER_NO_DEVICE "\"}"), index);  // No outputs defined so nothing to control
          error = 1;
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
      if ((strlen(mqtt_data) > (LOGSZ - TOPSZ - 20)) || (i == MAX_TIMERS -1)) {
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
      Settings.longitude = (int)(AtoD(XdrvMailbox.data) *1000000);
    }
    char lbuff[32];
    dtostrfd(((double)Settings.longitude) /1000000, 6, lbuff);
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, lbuff);
  }
  else if (CMND_LATITUDE == command_code) {
    if (XdrvMailbox.data_len) {
      Settings.latitude = (int)(AtoD(XdrvMailbox.data) *1000000);
    }
    char lbuff[32];
    dtostrfd(((double)Settings.latitude) /1000000, 6, lbuff);
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, lbuff);
  }
#endif
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
  "function qs(s){"                                               // Alias to save code space
    "return document.querySelector(s);"
  "}"
  "function ce(i,q){"                                             // Create select option
    "var o=document.createElement('option');"
    "o.textContent=i;"
    "q.appendChild(o);"
  "}"
#ifdef USE_SUNRISE
  "function gt(){"                                                // Set hours and minutas according to mode
    "var m,p,q;"
    "m=qs('input[name=\"rd\"]:checked').value;"                   // Get mode
    "if(m==0){p=pt[ct]&0x7FF;}"                                   // Schedule time
    "if(m==1){p=pt[" STR(MAX_TIMERS) "];}"                        // Sunrise
    "if(m==2){p=pt[" STR(MAX_TIMERS +1) "];}"                     // Sunset
    "q=Math.floor(p/60);if(q<10){q='0'+q;}qs('#ho').value=q;"     // Set hours
    "q=p%60;if(q<10){q='0'+q;}qs('#mi').value=q;"                 // Set minutes
  "}"
#endif
  "function st(){"                                                // Save parameters to hidden area
    "var i,n,p,s;"
    "s=0;"
    "n=1<<30;if(eb('a0').checked){s|=n;}"                         // Get arm
    "n=1<<29;if(eb('r0').checked){s|=n;}"                         // Get repeat
    "for(i=0;i<7;i++){n=1<<(16+i);if(eb('w'+i).checked){s|=n;}}"  // Get weekdays
#ifdef USE_SUNRISE
    "s|=(qs('input[name=\"rd\"]:checked').value<<11);"            // Get mode
#endif
    "s|=(eb('p1').value<<27);"                                    // Get power
    "s|=(qs('#d1').selectedIndex<<23);"                           // Get device
    "s|=((qs('#ho').selectedIndex*60)+qs('#mi').selectedIndex)&0x7FF;"  // Get time
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
    "p=(s>>11)&3;eb('b'+p).checked=1;"                            // Set mode
    "gt();"                                                       // Set hours and minutes according to mode
#else
    "p=s&0x7FF;"                                                  // Get time
    "q=Math.floor(p/60);if(q<10){q='0'+q;}qs('#ho').value=q;"     // Set hours
    "q=p%60;if(q<10){q='0'+q;}qs('#mi').value=q;"                 // Set minutes
#endif
    "for(i=0;i<7;i++){p=(s>>(16+i))&1;eb('w'+i).checked=p;}"      // Set weekdays
    "p=(s>>23)&0xF;qs('#d1').value=p+1;"                          // Set device
    "p=(s>>27)&3;eb('p1').value=p;"                               // Set power
    "p=(s>>29)&1;eb('r0').checked=p;"                             // Set repeat
    "p=(s>>30)&1;eb('a0').checked=p;"                             // Set arm
  "}"
  "function it(){"                                                // Initialize elements and select first tab
    "var b,i,o,s;"
    "pt=eb('t0').value.split(',').map(Number);"                   // Get parameters from hidden area to array
    "s='';for(i=0;i<" STR(MAX_TIMERS) ";i++){b='';if(0==i){b=\" id='dP'\";}s+=\"<button type='button' class='tl' onclick='ot(\"+i+\",this)'\"+b+\">\"+(i+1)+\"</button>\"}"
    "eb('bt').innerHTML=s;"                                       // Create tabs
    "o=qs('#ho');for(i=0;i<=23;i++){ce((i<10)?('0'+i):i,o);}"     // Create hours select options
    "o=qs('#mi');for(i=0;i<=59;i++){ce((i<10)?('0'+i):i,o);}"     // Create minutes select options
    "o=qs('#d1');for(i=0;i<}1;i++){ce(i+1,o);}"                   // Create devices
    "var a='" D_DAY3LIST "';"
    "s='';for(i=0;i<7;i++){s+=\"<input style='width:5%;' id='w\"+i+\"' name='w\"+i+\"' type='checkbox'><b>\"+a.substring(i*3,(i*3)+3)+\"</b>\"}"
    "eb('ds').innerHTML=s;"                                       // Create weekdays
    "eb('dP').click();"                                           // Get the element with id='dP' and click on it
  "}";
const char HTTP_TIMER_STYLE[] PROGMEM =
  ".tl{float:left;border-radius:0;border:1px solid #fff;padding:1px;width:6.25%;}"
#ifdef USE_SUNRISE
  "input[type='radio']{width:13px;height:34px;margin-top:-1px;margin-right:8px;vertical-align:middle;}"
#endif
  "</style>";
const char HTTP_FORM_TIMER[] PROGMEM =
  "<fieldset style='min-width:470px;text-align:center;'><legend style='text-align:left;'><b>&nbsp;" D_TIMER_PARAMETERS "&nbsp;</b></legend><form method='post' action='sv'>"
  "<input id='w' name='w' value='7,0' hidden><input id='t0' name='t0' value='";
const char HTTP_FORM_TIMER1[] PROGMEM =
  "' hidden><div id='bt' name='bt'></div><br/><br/><br/>"
  "<div>"
  "<b>" D_TIMER_OUTPUT "</b>&nbsp;<span><select style='width:60px;' id='d1' name='d1'></select></span>&emsp;"
  "<b>" D_TIMER_POWER "</b>&nbsp;<select style='width:99px;' id='p1' name='p1'>"
    "<option value='0'>" D_OFF "</option>"
    "<option value='1'>" D_ON "</option>"
    "<option value='2'>" D_TOGGLE "</option>"
    "<option value='3'>" D_BLINK "</option>"
  "</select>"
  "</div><br/>"
  "<div>"
  "<input style='width:5%;' id='a0' name='a0' type='checkbox'><b>" D_TIMER_ARM "</b>&emsp;"
  "<input style='width:5%;' id='r0' name='r0' type='checkbox'><b>" D_TIMER_REPEAT "</b>"
  "</div><br/>"
  "<div>"
//  "<b>Time</b>&nbsp;<input type='time' style='width:25%;' id='s1' name='s1' value='00:00' pattern='[0-9]{2}:[0-9]{2}'>"
#ifdef USE_SUNRISE
  "<fieldset style='width:299px;margin:auto;text-align:left;border:0;'>"
  "<input id='b0' name='rd' type='radio' value='0' onclick='gt();'><b>" D_TIMER_TIME "</b>&nbsp;"
                                                  "<span><select style='width:60px;' id='ho' name='ho' onclick='eb(\"b0\").checked=1;'></select></span>"
                                                  "&nbsp;" D_HOUR_MINUTE_SEPARATOR "&nbsp;"
                                                  "<span><select style='width:60px;' id='mi' name='mi' onclick='eb(\"b0\").checked=1;'></select></span><br/>"
  "<input id='b1' name='rd' type='radio' value='1' onclick='gt();'><b>" D_SUNRISE "</b><br/>"
  "<input id='b2' name='rd' type='radio' value='2' onclick='gt();'><b>" D_SUNSET "</b><br/>"
  "</fieldset>"
#else
  "<b>" D_TIMER_TIME "</b>&nbsp;"
  "<span><select style='width:60px;' id='ho' name='ho'></select></span>"
  "&nbsp;" D_HOUR_MINUTE_SEPARATOR "&nbsp;"
  "<span><select style='width:60px;' id='mi' name='mi'></select></span>"
#endif  // USE_SUNRISE
  "</div><br/>"
  "<div id='ds' name='ds'></div>";
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
#ifdef USE_SUNRISE
  page += F(","); page += String(GetSunMinutes(0));  // Add Sunrise
  page += F(","); page += String(GetSunMinutes(1));  // Add Sunset
#endif  // USE_SUNRISE
  page += FPSTR(HTTP_FORM_TIMER1);
  page.replace(F("}1"), String(devices_present));
#ifdef USE_SUNRISE
  page.replace(F("299"), String(180 + (strlen(D_TIMER_TIME) *10)));  // Fix string length to keep radios centered
#endif  // USE_SUNRISE
  page += FPSTR(HTTP_FORM_END);
  page.replace(F("type='submit'"), FPSTR(HTTP_FORM_TIMER2));
  page += F("<script>it();</script>");  // Init elements and select first tab/button
  page += FPSTR(HTTP_BTN_CONF);
  ShowPage(page);
}

void TimerSaveSettings()
{
  char tmp[MAX_TIMERS *12];  // Need space for MAX_TIMERS x 10 digit numbers separated by a comma
  Timer timer;

  WebGetArg("t0", tmp, sizeof(tmp));
  char *p = tmp;
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MQTT D_CMND_TIMERS " "));
  for (byte i = 0; i < MAX_TIMERS; i++) {
    timer.data = strtol(p, &p, 10);
    p++;  // Skip comma
    if (timer.time < 1440) {
#ifdef USE_SUNRISE
      if ((1 == timer.mode) || (2 == timer.mode)) timer.time = Settings.timer[i].time;  // Do not save time on Sunrise or Sunset
#endif
      Settings.timer[i].data = timer.data;
    }
    snprintf_P(log_data, sizeof(log_data), PSTR("%s%s0x%08X"), log_data, (i > 0)?",":"", Settings.timer[i].data);
  }
  AddLog(LOG_LEVEL_DEBUG);
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