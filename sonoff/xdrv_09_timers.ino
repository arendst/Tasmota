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
//#define USE_SUNRISE
#define D_TIMER_MODE "Mode"
#define D_GIVENTIME "given time"
#define D_SUNRISE "sunrise"
#define D_SUNSET "sunset"
#define D_CMND_LONGITUDE "Longitude"
#define D_CMND_LATITUDE "Latitude"
#define D_JSON_TIMER_MODE "Mode"

enum TimerCommands { CMND_TIMER, CMND_TIMERS,CMND_LONGITUDE,CMND_LATITUDE };

const char kTimerCommands[] PROGMEM = D_CMND_TIMER "|" D_CMND_TIMERS
#ifdef USE_SUNRISE
"|" D_CMND_LONGITUDE "|" D_CMND_LATITUDE
#endif
;

uint16_t fired = 0;

uint8_t lastminute;

void TimerEverySecond()
{
  if (RtcTime.valid) {
      if (RtcTime.minute!=lastminute) {
        // execute every minute
        lastminute=RtcTime.minute;
        uint16_t time = (RtcTime.hour * 60) + RtcTime.minute;
        uint8_t days = 1 << (RtcTime.day_of_week -1);

        for (byte i = 0; i < MAX_TIMERS; i++) {
          uint16_t xtime;
#ifdef USE_SUNRISE
          if (Settings.timer[i].mday>0) {
            // sunrise,sunset mode
            uint8_t hour_up,minute_up,hour_down,minute_down;
            dusktilldawn(&hour_up,&minute_up,&hour_down,&minute_down);
            if (Settings.timer[i].mday==1) {
              // sunrise
              xtime=(hour_up*60)+minute_up;
            } else {
              // sunset
              xtime=(hour_down*60)+minute_down;
            }
          } else {
             xtime = Settings.timer[i].time;
          }
#else
          xtime = Settings.timer[i].time;
#endif
          if (Settings.timer[i].device >= devices_present) Settings.timer[i].data = 0;  // Reset timer due to change in devices present
          if (Settings.timer[i].arm) {
            if (time == xtime) {
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
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s\"" D_CMND_TIMER "%d\":{\"" D_JSON_TIMER_ARM "\":%d,\"" D_JSON_TIMER_TIME "\":\"%02d:%02d\",\"" D_JSON_TIMER_DAYS "\":\"%s\",\"" D_JSON_TIMER_REPEAT "\":%d,\"" D_JSON_TIMER_OUTPUT "\":%d,\"" D_JSON_TIMER_POWER "\":%d,\"" D_JSON_TIMER_MODE "\":%d}"),
  mqtt_data, index +1, Settings.timer[index].arm, Settings.timer[index].time / 60, Settings.timer[index].time % 60, days, Settings.timer[index].repeat, Settings.timer[index].device +1, Settings.timer[index].power,Settings.timer[index].mday);
#else
  snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s\"" D_CMND_TIMER "%d\":{\"" D_JSON_TIMER_ARM "\":%d,\"" D_JSON_TIMER_TIME "\":\"%02d:%02d\",\"" D_JSON_TIMER_DAYS "\":\"%s\",\"" D_JSON_TIMER_REPEAT "\":%d,\"" D_JSON_TIMER_OUTPUT "\":%d,\"" D_JSON_TIMER_POWER "\":%d}"),
    mqtt_data, index +1, Settings.timer[index].arm, Settings.timer[index].time / 60, Settings.timer[index].time % 60, days, Settings.timer[index].repeat, Settings.timer[index].device +1, Settings.timer[index].power);
#endif
}


#ifdef USE_SUNRISE
// simple asci to double, because atof is too large
double myatof(char *str) {
    // strstr
    char strbuf[24];
    strcpy(strbuf,str);
    char *pt;
    double left = atoi(strbuf);
    double right = 0;
    short len=0;
    pt = strtok (strbuf,".");
    if (pt) {
        pt=strtok (NULL,".");
        if (pt) {
            right = atoi(pt);
            len=strlen(pt);
            double fac=1;
            while (len) {
              fac/=10.0;
              len--;
            }
            // pow is also very large
            //double fac=pow(10,-len);
            right*=fac;
        }
    }
    return left+right;
}


// https://forum.arduino.cc/index.php?topic=218280.0
// C-Programm von http://lexikon.astronomie.info/zeitgleichung/neu.html
// umgeschrieben auf Arduino by 'jurs' for German Arduino forum
const double pi2=6.283185307179586476925286766559;
const double pi=3.1415926535897932384626433832795;
const double RAD = 0.017453292519943295769236907684886;

double JulianischesDatum ( int Jahr, int Monat, int Tag, int Stunde, int Minuten, double Sekunden ) {
// Gregorianischer Kalender
    int   Gregor;
    if (Monat<=2) {
        Monat=Monat +12;
        Jahr = Jahr -1;
    }
    Gregor = (Jahr/400)-(Jahr/100)+(Jahr/4);  // Gregorianischer Kalender
    return 2400000.5+365.0*Jahr - 679004.0 + Gregor + (int)(30.6001*(Monat+1)) + Tag + Stunde/24.0 + Minuten/1440.0 + Sekunden/86400.0;
}

double InPi(double x) {
    int n = (int)(x/pi2);
    x = x - n*pi2;
    if (x<0) x +=pi2;
    return x;
}

double eps(double T) { // Neigung der Erdachse
    return RAD*(23.43929111 + (-46.8150*T - 0.00059*T*T + 0.001813*T*T*T)/3600.0);
}

double BerechneZeitgleichung(double *DK,double T) {

    double RA_Mittel = 18.71506921 + 2400.0513369*T +(2.5862e-5 - 1.72e-9*T)*T*T;

    double M  = InPi(pi2 * (0.993133 + 99.997361*T));
    double L  = InPi(pi2 * (  0.7859453 + M/pi2 + (6893.0*sin(M)+72.0*sin(2.0*M)+6191.2*T) / 1296.0e3));
    double e = eps(T);
    double RA = atan(tan(L)*cos(e));
    if (RA<0.0) RA+=pi;
    if (L>pi) RA+=pi;

    RA = 24.0*RA/pi2;

    *DK = asin(sin(e)*sin(L));

    // Damit 0<=RA_Mittel<24
    RA_Mittel = 24.0*InPi(pi2*RA_Mittel/24.0)/pi2;

    double dRA = RA_Mittel - RA;
    if (dRA < -12.0) dRA+=24.0;
    if (dRA > 12.0) dRA-=24.0;

    dRA = dRA* 1.0027379;

    return dRA ;
}


void dusktilldawn(uint8_t *hour_up,uint8_t *minute_up, uint8_t *hour_down, uint8_t *minute_down) {

    double JD2000 = 2451545.0;
    double JD;

    JD = JulianischesDatum(RtcTime.year,RtcTime.month,RtcTime.day_of_month,12,0,0);

    /*
     // manuelle Dateneingabe
     int year, month, day;
     cout << "Berechnung Sonnenufgang und -untergang" << endl;
     cout << "Jahr (YYYY): "; cin >> year;
     cout << "Monat (MM): ";  cin >> month;
     cout << "Tag (DD): ";    cin >> day;
     JD = JulianischesDatum(year,month,day,12,0,0);
     */

    /*
    h (D) = -0.8333 normaler SA & SU-Gang
    h (D) = -6.0 civile Dämmerung
    h (D) = -12.0 nautische Dämmerung
    h (D) = -18.0 astronomische Dämmerung

    double T = (JD - JD2000)/36525.0;
    double DK;
    double h = D*RAD;
    double B = xx.xx*RAD; // geographische Breite
    double GeographischeLaenge = x.xx;
    */

    double T = (JD - JD2000)/36525.0;
    double DK;
    double h = -50/60.0*RAD;
    double B = Settings.latitude*RAD; // geographische Breite
    double GeographischeLaenge = Settings.longitude;

    //double Zeitzone = 0; //Weltzeit
    double Zeitzone = 2;

    double Zeitgleichung = BerechneZeitgleichung(&DK,T);
    double Minuten = Zeitgleichung*60.0;
    double Zeitdifferenz = 12.0*acos((sin(h) - sin(B)*sin(DK)) / (cos(B)*cos(DK)))/pi;
    double AufgangOrtszeit = 12.0 - Zeitdifferenz - Zeitgleichung;
    double UntergangOrtszeit = 12.0 + Zeitdifferenz - Zeitgleichung;
    double AufgangWeltzeit = AufgangOrtszeit - GeographischeLaenge /15.0;
    double UntergangWeltzeit = UntergangOrtszeit - GeographischeLaenge /15.0;

    double Aufgang = AufgangWeltzeit + Zeitzone;         // In Stunden
    if (Aufgang<0.0) Aufgang +=24.0;
    else if (Aufgang>=24.0) Aufgang -=24.0;

    double Untergang = UntergangWeltzeit + Zeitzone;
    if (Untergang<0.0) Untergang +=24.0;
    else if (Untergang>=24.0) Untergang -=24.0;

    int AufgangMinuten   = (int)(60.0*(Aufgang - (int)Aufgang)+0.5);
    int AufgangStunden   = (int)Aufgang;
    if (AufgangMinuten>=60.0) { AufgangMinuten-=60.0; AufgangStunden++; }
    else if (AufgangMinuten<0.0) {
        AufgangMinuten+=60.0; AufgangStunden--;
        if (AufgangStunden<0.0) AufgangStunden+=24.0;
    }

    int UntergangMinuten = (int)(60.0*(Untergang - (int)Untergang)+0.5);
    int UntergangStunden = (int)Untergang;
    if (UntergangMinuten>=60.0) { UntergangMinuten-=60.0; UntergangStunden++; }
    else if (UntergangMinuten<0) {
        UntergangMinuten+=60.0; UntergangStunden--;
        if (UntergangStunden<0.0) UntergangStunden+=24.0;
    }

    *hour_up=AufgangStunden;
    *minute_up=AufgangMinuten;

    *hour_down=UntergangStunden;
    *minute_down=UntergangMinuten;

}
#endif
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
#ifdef USE_SUNRISE
          if (root[UpperCase_P(parm_uc, PSTR(D_JSON_TIMER_MODE))].success()) {
            Settings.timer[index].mday = (uint8_t)root[parm_uc] & 0x03;
          }
#endif
          if (Settings.timer[index].arm) bitClear(fired, index);
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
            if (Settings.timer[index].arm) bitClear(fired, index);

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
      if ((strlen(mqtt_data) > (LOGSZ - TOPSZ)) || (i == MAX_TIMERS -1)) {
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}}"), mqtt_data);
        MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_TIMERS));
        jsflg = 0;
      }
    }
    mqtt_data[0] = '\0';
#ifdef USE_SUNRISE
  } else if (CMND_LONGITUDE == command_code) {
    if (XdrvMailbox.data_len) {
      Settings.longitude = myatof(XdrvMailbox.data);
    }
    char lbuff[32];
    dtostrfd(Settings.longitude,6,lbuff);
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, lbuff);

  }  else if (CMND_LATITUDE == command_code) {
    if (XdrvMailbox.data_len) {
      Settings.latitude = myatof(XdrvMailbox.data);
    }
    char lbuff[32];
    dtostrfd(Settings.latitude,6,lbuff);
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_SVALUE, command, lbuff);
#endif
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

  "function qs(s){"                                               // Alias to save code space
    "return document.querySelector(s);"
  "}"

  "function ce(i,q){"                                             // Create select option
    "var o=document.createElement('option');"
    "o.textContent=i;"
    "q.appendChild(o);"
  "}"

  "function st(){"                                                // Save parameters to hidden area
    "var i,n,p,s;"
    "s=0;"
    "n=1<<30;if(eb('a0').checked){s|=n;}"                         // Get arm
    "n=1<<29;if(eb('r0').checked){s|=n;}"                         // Get repeat
    "for(i=0;i<7;i++){n=1<<(16+i);if(eb('w'+i).checked){s|=n;}}"  // Get weekdays
    "s|=(eb('p1').value<<27);"                                    // Get power
    "s|=(qs('#d1').selectedIndex<<23);"                           // Get device

#ifdef USE_SUNRISE
    "s|=(eb('o1').value<<11);"                                    // Get mode
#endif

    "s|=((qs('#ho').selectedIndex*60)+qs('#mi').selectedIndex)&0x7FF;"  // Get time
    "pt[ct]=s;"
    "eb('t0').value=pt.join();"                                   // Save parameters from array to hidden area
  "}"

  "function ot(t,e){"                                             // Select tab and update elements
    "var i,n,o,p,q,s;"
    "if(ct<99){"
      "st();"                                                     // Save changes
    "}"
    "o=document.getElementsByClassName('tl');"                    // Restore style to all tabs/buttons
    "for(i=0;i<o.length;i++){o[i].style.cssText=\"background-color:#ccc;color:#fff;font-weight:normal;\"}"
    "e.style.cssText=\"background-color:#fff;color:#000;font-weight:bold;\";"  // Change style to tab/button used to open content
    "s=pt[t];"                                                    // Get parameters from array
    "p=s&0x7FF;"                                                  // Get time
    "q=Math.floor(p/60);if(q<10){q='0'+q;}qs('#ho').value=q;"     // Set hours
    "q=p%60;if(q<10){q='0'+q;}qs('#mi').value=q;"                 // Set minutes
    "for(i=0;i<7;i++){p=(s>>(16+i))&1;eb('w'+i).checked=p;}"      // Set weekdays
    "p=(s>>23)&0xF;qs('#d1').value=p+1;"                          // Set device
    "p=(s>>27)&3;eb('p1').value=p;"                               // Set power
#ifdef USE_SUNRISE
    "p=(s>>11)&3;eb('o1').value=p;"                               // Set mode
#endif
    "p=(s>>29)&1;eb('r0').checked=p;"                             // Set repeat
    "p=(s>>30)&1;eb('a0').checked=p;"                             // Set arm
    "ct=t;"
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
  "</style>";
const char HTTP_FORM_TIMER[] PROGMEM =

#ifdef USE_SUNRISE
  "<fieldset style='min-width:600px;text-align:center;'><legend style='text-align:left;'><b>&nbsp;" D_TIMER_PARAMETERS "&nbsp;</b></legend><form method='post' action='sv'>"
#else
  "<fieldset style='min-width:470px;text-align:center;'><legend style='text-align:left;'><b>&nbsp;" D_TIMER_PARAMETERS "&nbsp;</b></legend><form method='post' action='sv'>"
#endif
  "<input id='w' name='w' value='7,0' hidden><input id='t0' name='t0' value='";
const char HTTP_FORM_TIMER1[] PROGMEM =
  "' hidden><div id='bt' name='bt'></div><br/><br/><br/>"
  "<div>"
  "<b>" D_TIMER_OUTPUT "</b>&nbsp;<span><select style='width:12%;' id='d1' name='d1'></select></span>&emsp;"
  "<b>" D_TIMER_POWER "</b>&nbsp;<select style='width:25%;' id='p1' name='p1'>"
    "<option value='0'>" D_OFF "</option>"
    "<option value='1'>" D_ON "</option>"
    "<option value='2'>" D_TOGGLE "</option>"
    "<option value='3'>" D_BLINK "</option>"
    "</select>"

#ifdef USE_SUNRISE
  "<b>" D_TIMER_MODE "</b>&nbsp;<select style='width:25%;' id='o1' name='o1' onchange='changedMode(this.value)' >"
      "<option value='0'>" D_GIVENTIME "</option>"
      "<option value='1'>" D_SUNRISE "</option>"
      "<option value='2'>" D_SUNSET "</option>"
      "</select>"
#endif

  "</div><br/>"
  "<div>"
//  "<b>Time</b>&nbsp;<input type='time' style='width:25%;' id='s1' name='s1' value='00:00' pattern='[0-9]{2}:[0-9]{2}'>&emsp;"
  "<b>" D_TIMER_TIME "</b>&nbsp;<span><select style='width:12%;' id='ho' name='ho'></select></span>&nbsp;" D_HOUR_MINUTE_SEPARATOR "&nbsp;<span><select style='width:12%;' id='mi' name='mi'></select></span>&emsp;"
  "<input style='width:5%;' id='a0' name='a0' type='checkbox'><b>" D_TIMER_ARM "</b>&emsp;"
  "<input style='width:5%;' id='r0' name='r0' type='checkbox'><b>" D_TIMER_REPEAT "</b>"
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
#ifdef USE_SUNRISE
    Timer xtimer=Settings.timer[i];
    if (xtimer.mday>0) {
      // replace hours, minutes by sunrise
      uint8_t hour_up,minute_up,hour_down,minute_down;
      dusktilldawn(&hour_up,&minute_up,&hour_down,&minute_down);
      if (xtimer.mday==1) {
        // sunrise
        xtimer.time=(hour_up*60)+minute_up;
      } else {
        // sunset
        xtimer.time=(hour_down*60)+minute_down;
      }
    }
    page += String(xtimer.data);
#else
    page += String(Settings.timer[i].data);
#endif

  }
  page += FPSTR(HTTP_FORM_TIMER1);
  page.replace(F("}1"), String(devices_present));
  page += FPSTR(HTTP_FORM_END);
  page.replace(F("type='submit'"), FPSTR(HTTP_FORM_TIMER2));
  page += F("<script>it();</script>");  // Init elements and select first tab/button
  page += FPSTR(HTTP_BTN_CONF);
  ShowPage(page);
}

void TimerSaveSettings()
{
  char tmp[MAX_TIMERS *12];  // Need space for MAX_TIMERS x 10 digit numbers separated by a comma

  WebGetArg("t0", tmp, sizeof(tmp));
  char *p = tmp;
  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MQTT D_CMND_TIMERS " "));
  for (byte i = 0; i < MAX_TIMERS; i++) {
    uint32_t data = strtol(p, &p, 10);
    p++;  // Skip comma
#ifdef USE_SUNRISE
    Timer xtimer;
    xtimer.data=data;
    if (xtimer.mday!=Settings.timer[i].mday) {
      // status has changed
      if (xtimer.mday>0) {
        // now on, do not save time
        // copy everything but time
        Settings.timer[i].mday=xtimer.mday;
        Settings.timer[i].days=xtimer.days;
        Settings.timer[i].device=xtimer.device;
        Settings.timer[i].power=xtimer.power;
        Settings.timer[i].repeat=xtimer.repeat;
        Settings.timer[i].arm=xtimer.arm;
      } else {
          // now off, copy all
          if ((data & 0x7FF) < 1440) Settings.timer[i].data = data;
      }
    }
#else
    if ((data & 0x7FF) < 1440) Settings.timer[i].data = data;
#endif
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
