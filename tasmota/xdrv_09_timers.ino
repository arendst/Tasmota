/*
  xdrv_09_timers.ino - timer support for Tasmota

  Copyright (C) 2021  Theo Arends

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
 * Window allows Time offset for +/- 15 minutes max as long as Time is not within Window from 00:00
\*********************************************************************************************/

#define XDRV_09             9

const char kTimerCommands[] PROGMEM = "|"  // No prefix
  D_CMND_TIMER "|" D_CMND_TIMERS
#ifdef USE_SUNRISE
  "|" D_CMND_LATITUDE "|" D_CMND_LONGITUDE
#endif
  ;

void (* const TimerCommand[])(void) PROGMEM = {
  &CmndTimer, &CmndTimers
#ifdef USE_SUNRISE
  , &CmndLatitude, &CmndLongitude
#endif
  };

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

const float pi2 = TWO_PI;
const float pi = PI;
const float RAD = DEG_TO_RAD;

// Compute the Julian date from the Calendar date, using only unsigned ints for code compactness
// Warning this formula works only from 2000 to 2099, after 2100 we get 1 day off per century. If ever Tasmota survives until then.
uint32_t JulianDate(const struct TIME_T &now) {
  // https://en.wikipedia.org/wiki/Julian_day

  uint32_t Year = now.year;             // Year ex:2020
  uint32_t Month = now.month;            // 1..12
  uint32_t Day = now.day_of_month;     // 1..31
  uint32_t Julian;                          // Julian day number

  if (Month <= 2) {
    Month += 12;
    Year -= 1;
  }
  // Warning, this formula works only for the 20th century, afterwards be are off by 1 day - which does not impact Sunrise much
  // Julian = (1461 * Year + 6884472) / 4 + (153 * Month - 457) / 5 + Day -1 -13;
  Julian = (1461 * Year + 6884416) / 4 + (153 * Month - 457) / 5 + Day;   // -1 -13 included in 6884472 - 14*4 = 6884416
  return Julian;
}

// Force value in the 0..pi2 range
float InPi(float x)
{
  return ModulusRangef(x, 0.0f, pi2);
}

// Time formula
// Tdays is the number of days since Jan 1 2000, and replaces T as the Tropical Century. T = Tdays / 36525.0
float TimeFormula(float *DK, uint32_t Tdays) {
  float RA_Mean = 18.71506921f + (2400.0513369f / 36525.0f) * Tdays;    // we keep only first order value as T is between 0.20 and 0.30
  float M = InPi( (pi2 * 0.993133f) + (pi2 * 99.997361f / 36525.0f) * Tdays);
  float L = InPi( (pi2 * 0.7859453f) + M + (6893.0f * sinf(M) + 72.0f * sinf(M+M) + (6191.2f / 36525.0f) * Tdays) * (pi2 / 1296.0e3f));

  float cos_eps = 0.91750f;     // precompute cos(eps)
  float sin_eps = 0.39773f;     // precompute sin(eps)

  float RA = atanf(tanf(L) * cos_eps);
  if (RA < 0.0f) RA += pi;
  if (L > pi) RA += pi;
  RA = RA * (24.0f/pi2);
  *DK = asinf(sin_eps * sinf(L));
  RA_Mean = ModulusRangef(RA_Mean, 0.0f, 24.0f);
  float dRA = ModulusRangef(RA_Mean - RA, -12.0f, 12.0f);
  dRA = dRA * 1.0027379f;
  return dRA;
}

void DuskTillDawn(uint8_t *hour_up,uint8_t *minute_up, uint8_t *hour_down, uint8_t *minute_down)
{
  const uint32_t JD2000 = 2451545;
  uint32_t JD = JulianDate(RtcTime);
  uint32_t Tdays = JD - JD2000;           // number of days since Jan 1 2000

  // ex 2458977 (2020 May 7) - 2451545 -> 7432 -> 0,2034
  float DK;
  /*
  h (D) = -0.8333 normaler SA & SU-Gang
  h (D) = -6.0 civile Dämmerung
  h (D) = -12.0 nautische Dämmerung
  h (D) = -18.0 astronomische Dämmerung
  */
  const float h = SUNRISE_DAWN_ANGLE * RAD;
  const float sin_h = sinf(h);    // let GCC pre-compute the sin() at compile time

  float B = Settings.latitude / (1000000.0f / RAD); // geographische Breite
  //float B = (((float)Settings.latitude)/1000000) * RAD; // geographische Breite
  float GeographischeLaenge = ((float)Settings.longitude)/1000000;
//  double Zeitzone = 0; //Weltzeit
//  double Zeitzone = 1; //Winterzeit
//  double Zeitzone = 2.0;   //Sommerzeit
  float Zeitzone = ((float)Rtc.time_timezone) / 60;
  float Zeitgleichung = TimeFormula(&DK, Tdays);
  float Zeitdifferenz = acosf((sin_h - sinf(B)*sinf(DK)) / (cosf(B)*cosf(DK))) * (12.0f / pi);
  float AufgangOrtszeit = 12.0f - Zeitdifferenz - Zeitgleichung;
  float UntergangOrtszeit = 12.0f + Zeitdifferenz - Zeitgleichung;
  float AufgangWeltzeit = AufgangOrtszeit - GeographischeLaenge / 15.0f;
  float UntergangWeltzeit = UntergangOrtszeit - GeographischeLaenge / 15.0f;
  float Aufgang = AufgangWeltzeit + Zeitzone + (1/120.0f);         // In Stunden, with rounding to nearest minute (1/60 * .5)

  Aufgang = ModulusRangef(Aufgang, 0.0f, 24.0f);        // force 0 <= x < 24.0
  int AufgangStunden = (int)Aufgang;
  int AufgangMinuten = (int)(60.0f * fmodf(Aufgang, 1.0f));
  float Untergang = UntergangWeltzeit + Zeitzone;

  Untergang = ModulusRangef(Untergang, 0.0f, 24.0f);
  int UntergangStunden = (int)Untergang;
  int UntergangMinuten = (int)(60.0f * fmodf(Untergang, 1.0f));

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

  if (hour[mode]==255) {
    // Permanent day/night sets the unreachable limit values
    if ((Settings.latitude > 0) != (RtcTime.month>=4 && RtcTime.month<=9)) {
      duskdawn->time=2046; // permanent night
    } else {
      duskdawn->time=2047; // permanent day
    }
    // So skip the offset/underflow/overflow/day-shift
    return;
  }

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
    if (timeBuffer >= 1440) {
      timeBuffer -= 1440;
      duskdawn->days = duskdawn->days << 1;
      duskdawn->days |= (stored.days >> 6);
    }
  }
  duskdawn->time = timeBuffer;
}

String GetSun(uint32_t dawn)
{
  char stime[6];

  uint8_t hour[2];
  uint8_t minute[2];

  DuskTillDawn(&hour[0], &minute[0], &hour[1], &minute[1]);
  dawn &= 1;
  snprintf_P(stime, sizeof(stime), PSTR("%02d:%02d"), hour[dawn], minute[dawn]);
  return String(stime);
}

uint16_t SunMinutes(uint32_t dawn)
{
  uint8_t hour[2];
  uint8_t minute[2];

  DuskTillDawn(&hour[0], &minute[0], &hour[1], &minute[1]);
  dawn &= 1;
  return (hour[dawn] *60) + minute[dawn];
}

#endif  // USE_SUNRISE

/*******************************************************************************************/

void TimerSetRandomWindow(uint32_t index)
{
  timer_window[index] = 0;
  if (Settings.timer[index].window) {
    timer_window[index] = (random(0, (Settings.timer[index].window << 1) +1)) - Settings.timer[index].window;  // -15 .. 15
  }
}

void TimerSetRandomWindows(void)
{
  for (uint32_t i = 0; i < MAX_TIMERS; i++) { TimerSetRandomWindow(i); }
}

void TimerEverySecond(void)
{
  if (RtcTime.valid) {
    if (!RtcTime.hour && !RtcTime.minute && !RtcTime.second) { TimerSetRandomWindows(); }  // Midnight
    if (Settings.flag3.timers_enable &&                            // CMND_TIMERS
        (TasmotaGlobal.uptime > 60) && (RtcTime.minute != timer_last_minute)) {  // Execute from one minute after restart every minute only once
      timer_last_minute = RtcTime.minute;
      int32_t time = (RtcTime.hour *60) + RtcTime.minute;
      uint8_t days = 1 << (RtcTime.day_of_week -1);

      for (uint32_t i = 0; i < MAX_TIMERS; i++) {
        Timer xtimer = Settings.timer[i];
        if (xtimer.arm) {
#ifdef USE_SUNRISE
          if ((1 == xtimer.mode) || (2 == xtimer.mode)) {      // Sunrise or Sunset
            ApplyTimerOffsets(&xtimer);
            if (xtimer.time>=2046) { continue; }
          }
#endif
          int32_t set_time = xtimer.time + timer_window[i];  // Add random time offset
          if (set_time < 0) {
            set_time = abs(timer_window[i]);                 // After midnight and within negative window so stay today but allow positive randomness;
          }
          if (set_time > 1439) {
            set_time = xtimer.time - abs(timer_window[i]);   // Before midnight and within positive window so stay today but allow negative randomness;
          }
          if (set_time > 1439) { set_time = 1439; }          // Stay today

          DEBUG_DRIVER_LOG(PSTR("TIM: Timer %d, Time %d, Window %d, SetTime %d"), i +1, xtimer.time, timer_window[i], set_time);

          if (time == set_time) {
            if (xtimer.days & days) {
              Settings.timer[i].arm = xtimer.repeat;
#if defined(USE_RULES) || defined(USE_SCRIPT)
              if (POWER_BLINK == xtimer.power) {             // Blink becomes Rule disregarding device and allowing use of Backlog commands
                Response_P(PSTR("{\"Clock\":{\"Timer\":%d}}"), i +1);
                XdrvRulesProcess();
              } else
#endif  // USE_RULES
                if (TasmotaGlobal.devices_present) { ExecuteCommandPower(xtimer.device +1, xtimer.power, SRC_TIMER); }
            }
          }
        }
      }
    }
  }
}

void PrepShowTimer(uint32_t index)
{
  Timer xtimer = Settings.timer[index -1];

  char days[8] = { 0 };
  for (uint32_t i = 0; i < 7; i++) {
    uint8_t mask = 1 << i;
    snprintf(days, sizeof(days), PSTR("%s%d"), days, ((xtimer.days & mask) > 0));
  }

  char soutput[80];
  soutput[0] = '\0';
  if (TasmotaGlobal.devices_present) {
    snprintf_P(soutput, sizeof(soutput), PSTR(",\"" D_JSON_TIMER_OUTPUT "\":%d"), xtimer.device +1);
  }
#ifdef USE_SUNRISE
  char sign[2] = { 0 };
  int16_t hour = xtimer.time / 60;
  if ((1 == xtimer.mode) || (2 == xtimer.mode)) {  // Sunrise or Sunset
    if (hour > 11) {
      hour -= 12;
      sign[0] = '-';
    }
  }
  ResponseAppend_P(PSTR("\"" D_CMND_TIMER "%d\":{\"" D_JSON_TIMER_ARM "\":%d,\"" D_JSON_TIMER_MODE "\":%d,\"" D_JSON_TIMER_TIME "\":\"%s%02d:%02d\",\"" D_JSON_TIMER_WINDOW "\":%d,\"" D_JSON_TIMER_DAYS "\":\"%s\",\"" D_JSON_TIMER_REPEAT "\":%d%s,\"" D_JSON_TIMER_ACTION "\":%d}"),
    index, xtimer.arm, xtimer.mode, sign, hour, xtimer.time % 60, xtimer.window, days, xtimer.repeat, soutput, xtimer.power);
#else
  ResponseAppend_P(PSTR("\"" D_CMND_TIMER "%d\":{\"" D_JSON_TIMER_ARM "\":%d,\"" D_JSON_TIMER_TIME "\":\"%02d:%02d\",\"" D_JSON_TIMER_WINDOW "\":%d,\"" D_JSON_TIMER_DAYS "\":\"%s\",\"" D_JSON_TIMER_REPEAT "\":%d%s,\"" D_JSON_TIMER_ACTION "\":%d}"),
    index, xtimer.arm, xtimer.time / 60, xtimer.time % 60, xtimer.window, days, xtimer.repeat, soutput, xtimer.power);
#endif  // USE_SUNRISE
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndTimer(void)
{
  uint32_t index = XdrvMailbox.index;
  if ((index > 0) && (index <= MAX_TIMERS)) {
    uint32_t error = 0;
    if (XdrvMailbox.data_len) {
      if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= MAX_TIMERS)) {
        if (XdrvMailbox.payload == 0) {
          Settings.timer[index -1].data = 0;  // Clear timer
        } else {
          Settings.timer[index -1].data = Settings.timer[XdrvMailbox.payload -1].data;  // Copy timer
        }
      } else {
//#ifndef USE_RULES
#if defined(USE_RULES)==0 && defined(USE_SCRIPT)==0
        if (TasmotaGlobal.devices_present) {
#endif
          JsonParser parser(XdrvMailbox.data);
          JsonParserObject root = parser.getRootObject();
          if (!root) {
            Response_P(PSTR("{\"" D_CMND_TIMER "%d\":\"" D_JSON_INVALID_JSON "\"}"), index); // JSON decode failed
            error = 1;
          }
          else {
            index--;
            JsonParserToken val = root[PSTR(D_JSON_TIMER_ARM)];
            if (val) {
              Settings.timer[index].arm = (val.getInt() != 0);
            }
#ifdef USE_SUNRISE
            val = root[PSTR(D_JSON_TIMER_MODE)];
            if (val) {
              Settings.timer[index].mode = val.getUInt() & 0x03;
            }
#endif
            val = root[PSTR(D_JSON_TIMER_TIME)];
            if (val) {
              uint16_t itime = 0;
              int8_t value = 0;
              uint8_t sign = 0;
              char time_str[10];

              strlcpy(time_str, val.getStr(), sizeof(time_str));
              const char *substr = strtok(time_str, ":");
              if (substr != nullptr) {
                if (strchr(substr, '-')) {
                  sign = 1;
                  substr++;
                }
                value = atoi(substr);
                if (sign) { value += 12; }  // Allow entering timer offset from -11:59 to -00:01 converted to 12:01 to 23:59
                if (value > 23) { value = 23; }
                itime = value * 60;
                substr = strtok(nullptr, ":");
                if (substr != nullptr) {
                  value = atoi(substr);
                  if (value < 0) { value = 0; }
                  if (value > 59) { value = 59; }
                  itime += value;
                }
              }
              Settings.timer[index].time = itime;
            }
            val = root[PSTR(D_JSON_TIMER_WINDOW)];
            if (val) {
              Settings.timer[index].window = val.getUInt() & 0x0F;
              TimerSetRandomWindow(index);
            }
            val = root[PSTR(D_JSON_TIMER_DAYS)];
            if (val) {
              // SMTWTFS = 1234567 = 0011001 = 00TW00S = --TW--S
              Settings.timer[index].days = 0;
              const char *tday = val.getStr();
              uint8_t i = 0;
              char ch = *tday++;
              while ((ch != '\0') && (i < 7)) {
                if (ch == '-') { ch = '0'; }
                uint8_t mask = 1 << i++;
                Settings.timer[index].days |= (ch == '0') ? 0 : mask;
                ch = *tday++;
              }
            }
            val = root[PSTR(D_JSON_TIMER_REPEAT)];
            if (val) {
              Settings.timer[index].repeat = (val.getUInt() != 0);
            }
            val = root[PSTR(D_JSON_TIMER_OUTPUT)];
            if (val) {
              uint8_t device = (val.getUInt() -1) & 0x0F;
              Settings.timer[index].device = (device < TasmotaGlobal.devices_present) ? device : 0;
            }
            val = root[PSTR(D_JSON_TIMER_ACTION)];
            if (val) {
              uint8_t action = val.getUInt() & 0x03;
              Settings.timer[index].power = (TasmotaGlobal.devices_present) ? action : 3;  // If no devices than only allow rules
            }

            index++;
          }
//#ifndef USE_RULES
#if defined(USE_RULES)==0 && defined(USE_SCRIPT)==0
        } else {
          Response_P(PSTR("{\"" D_CMND_TIMER "%d\":\"" D_JSON_TIMER_NO_DEVICE "\"}"), index);  // No outputs defined so nothing to control
          error = 1;
        }
#endif
      }
    }
    if (!error) {
      Response_P(PSTR("{"));
      PrepShowTimer(index);
      ResponseJsonEnd();
    }
  }
}

void CmndTimers(void)
{
  if (XdrvMailbox.data_len) {
    if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
      Settings.flag3.timers_enable = XdrvMailbox.payload;            // CMND_TIMERS
    }
    if (XdrvMailbox.payload == 2) {
      Settings.flag3.timers_enable = !Settings.flag3.timers_enable;  // CMND_TIMERS
    }
  }

  ResponseCmndStateText(Settings.flag3.timers_enable);               // CMND_TIMERS
  MqttPublishPrefixTopic_P(RESULT_OR_STAT, XdrvMailbox.command);

  uint32_t jsflg = 0;
  uint32_t lines = 1;
  for (uint32_t i = 0; i < MAX_TIMERS; i++) {
    if (!jsflg) {
      Response_P(PSTR("{\"" D_CMND_TIMERS "%d\":{"), lines++);
    } else {
      ResponseAppend_P(PSTR(","));
    }
    jsflg++;
    PrepShowTimer(i +1);
    if (jsflg > 3) {
      ResponseJsonEndEnd();
      MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_CMND_TIMERS));
      jsflg = 0;
    }
  }
  ResponseClear();
}

#ifdef USE_SUNRISE
void CmndLongitude(void)
{
  if (XdrvMailbox.data_len) {
    Settings.longitude = (int)(CharToFloat(XdrvMailbox.data) *1000000);
  }
  ResponseCmndFloat((float)(Settings.longitude) /1000000, 6);
}

void CmndLatitude(void)
{
  if (XdrvMailbox.data_len) {
    Settings.latitude = (int)(CharToFloat(XdrvMailbox.data) *1000000);
  }
  ResponseCmndFloat((float)(Settings.latitude) /1000000, 6);
}
#endif  // USE_SUNRISE

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
#ifdef USE_TIMERS_WEB

#define WEB_HANDLE_TIMER "tm"

const char HTTP_BTN_MENU_TIMER[] PROGMEM =
  "<p><form action='" WEB_HANDLE_TIMER "' method='get'><button>" D_CONFIGURE_TIMER "</button></form></p>";

#ifdef USE_UNISHOX_COMPRESSION
const size_t HTTP_TIMER_SCRIPT1_SIZE = 106;
const char HTTP_TIMER_SCRIPT1_COMPRESSED[] PROGMEM = "\x33\xBF\xA1\x94\x7C\x3D\xE3\xDF\x3A\x83\xA3\xE1\xC4\x8F\x04\x60\x5F\x07\x5B\x9C"
                             "\x83\x67\x77\x4E\xA3\x51\xDE\x3D\xA6\x77\xF5\x87\xC1\x30\x31\x63\x5F\x51\xD0\x3F"
                             "\xBB\xA6\x4C\x26\x35\xF5\x1D\xD3\xEF\x06\x56\xE7\x1F\x67\x78\xF1\x87\x4A\x66\xCA"
                             "\x20\xF3\xA9\xF5\x1F\x34\xF0\x6A\x3A\x58\xC1\x8F\x84\x20\xC5\x68\x42\x1D\xDC\x3B"
                             "\xC7\x83\xDC";
#define  HTTP_TIMER_SCRIPT1       Decompress(HTTP_TIMER_SCRIPT1_COMPRESSED,HTTP_TIMER_SCRIPT1_SIZE).c_str()
#else
const char HTTP_TIMER_SCRIPT1[] PROGMEM =
  "var pt=[],ct=99;"
  "function ce(i,q){"                                             // Create select option
    "var o=document.createElement('option');"
    "o.textContent=i;"
    "q.appendChild(o);"
  "}";
#endif //USE_UNISHOX_COMPRESSION

#ifdef USE_SUNRISE
#ifdef USE_UNISHOX_COMPRESSION
const size_t HTTP_TIMER_SCRIPT2_SIZE = 630;
const char HTTP_TIMER_SCRIPT2_COMPRESSED[] PROGMEM = "\x30\x2F\x83\xAD\xCE\x43\xD4\x77\x4E\xF1\xED\x33\xBF\xA1\xA7\x50\xC3\xA8\xD4\x78"
                             "\x1A\x7C\x35\x78\xEE\x9F\x7B\xC3\x05\xD1\xEF\x75\x8D\x67\xC3\xD9\xF1\x0F\x61\xEF"
                             "\x9E\x61\x8A\x61\x9A\x31\x0F\xB3\xBC\x74\x33\xB0\x85\xB3\xC0\xC3\xE0\xCA\x3D\xE0"
                             "\xE8\xF7\xCF\xD1\xC6\x46\xC3\x9E\x22\x30\x46\x0F\x1A\x60\xEE\x8D\x3E\x1F\x0E\x33"
                             "\xBC\x7B\x4B\xD8\x77\x4E\x33\xBC\x78\x23\x51\xF0\x86\xDD\x0A\x3A\x18\x0B\x33\xE7"
                             "\x74\x61\xD8\x73\x99\xDE\x3C\x16\x98\x3B\xA6\xA3\xD0\xE4\x67\x78\xF6\x91\xA8\xF8"
                             "\x7D\x9C\x67\xD9\xDB\x23\x51\xE0\xF7\x1A\xBC\x77\x4F\xB3\xC8\x56\x02\x1E\x5E\x7C"
                             "\x35\x1E\x0D\x47\xC1\x87\xD1\xF4\x73\x99\x02\x9E\x10\x37\x41\x1B\x08\x3D\xDA\x60"
                             "\xEE\x9D\xD1\xA7\xC3\xE1\xC8\x77\x8F\xF1\xFE\x3B\xA4\x34\xF8\x7C\x39\x47\x78\xEF"
                             "\x1E\xD2\xF6\x1D\xD3\x90\x81\x53\x59\x3F\x0F\x87\x25\x1D\xE3\xDA\x46\xA3\xAC\xF8"
                             "\x72\x51\xE0\x8D\x5E\x3B\xA7\xD9\xE4\x27\xCF\xB3\xBC\x74\xF3\x09\x87\x4C\x42\xDE"
                             "\x11\x9B\x0F\x87\x21\xE0\xF7\x13\x0B\xCC\xF6\x82\x9D\xC3\x8C\xF0\x7B\x88\x19\x67"
                             "\x04\x87\xB8\x11\x38\xE6\xF6\x1D\xD1\xC7\x78\xF6\xE1\xF0\x11\x32\xD3\xC3\x3E\x61"
                             "\xD0\x31\x5A\x10\x84\xC2\x63\x5F\x51\x07\x82\xFA\x8F\x1A\x60\xEE\x8E\x3E\x1F\x0E"
                             "\x43\xBC\x40\x8F\xC0\x1D\x19\x04\xCE\x86\x7B\xED\x1D\xA1\x6D\x19\x1F\x0F\xB3\xEC"
                             "\xF1\xA6\x0E\xEB\x3F\x0E\x4A\x3B\xC7\xB4\x8C\x67\xCE\xEE\x9F\x0E\x4A\x3C\x16\x9E"
                             "\x87\xC3\x95\x67\x82\xD3\xB6\x76\xCE\xF1\xED\xC3\xA7\xD8\xDC\x33\x64\x18\xAD\x08"
                             "\x43\xBB\x87\x40\xAF\xD4\x08\x7A\x08\xAD\x08\x43\xBC\x78\x3D\xC7\xB8\x13\x38\x68"
                             "\x04\xCD\x04\x56\x88\x23\xE0\x41\xD1\xCF\x43\x95\x64\x0A\x3A\x38\x6C\xEE\xE9\xD5"
                             "\x87\x78\xF0\x7B\x8F\x71\xEE\x3D\xC6";
#define  HTTP_TIMER_SCRIPT2       Decompress(HTTP_TIMER_SCRIPT2_COMPRESSED,HTTP_TIMER_SCRIPT2_SIZE).c_str()
#else
const char HTTP_TIMER_SCRIPT2[] PROGMEM =
  "function gt(){"                                                // Set hours and minutes according to mode
    "var m,p,q;"
    "m=qs('input[name=\"rd\"]:checked').value;"                   // Get mode
    "p=pt[ct]&0x7FF;"                                             // Get time
    "if(m==0){"                                                   // Time is set
      "so(0);"                                                    // Hide offset span and allow Hour 00..23
      "q=Math.floor(p/60);if(q<10){q='0'+q;}qs('#ho').value=q;"   // Set hours
      "q=p%%60;if(q<10){q='0'+q;}qs('#mi').value=q;"               // Set minutes
    "}"
    "if((m==1)||(m==2)){"                                         // Sunrise or sunset is set
      "so(1);"                                                    // Show offset span and allow Hour 00..11
      "q=Math.floor(p/60);"                                       // Parse hours
      "if(q>=12){q-=12;qs('#dr').selectedIndex=1;}"               // Negative offset
        "else{qs('#dr').selectedIndex=0;}"
      "if(q<10){q='0'+q;}qs('#ho').value=q;"                      // Set offset hours
      "q=p%%60;if(q<10){q='0'+q;}qs('#mi').value=q;"               // Set offset minutes
    "}"
  "}"
  "function so(b){"                                               // Hide or show offset items
    "o=qs('#ho');"
    "e=o.childElementCount;"
    "if(b==1){"
      "qs('#dr').style.visibility='';"
      "if(e>12){for(i=12;i<=23;i++){o.removeChild(o.lastElementChild);}}"  // Create offset hours select options
    "}else{"
      "qs('#dr').style.visibility='hidden';"
      "if(e<23){for(i=12;i<=23;i++){ce(i,o);}}"                   // Create hours select options
    "}"
  "}";
#endif //USE_UNISHOX_COMPRESSION
#endif //USE_SUNRISE

#ifdef USE_UNISHOX_COMPRESSION
#ifdef USE_SUNRISE
const size_t HTTP_TIMER_SCRIPT3_SIZE = 587;
const char HTTP_TIMER_SCRIPT3_COMPRESSED[] PROGMEM = "\x30\x2F\x83\xAD\xCE\x5E\xA3\xBA\x77\x8F\x69\x9D\xFD\x69\xD4\x11\xD4\x34\xEA\xE3"
                             "\xA8\x61\xD5\xE3\xC0\xD3\xE1\xC6\x78\x2F\x1F\x0E\x33\xC1\x71\xF0\xE4\x3D\x0F\x4B"
                             "\x87\x82\xD3\x07\x75\x8E\x3B\xA7\xDD\x9C\x67\xD9\xDE\x3A\x10\x62\x98\x66\x8C\x43"
                             "\xBC\x7B\x7C\x7F\x8F\x9C\x78\x3D\xDC\x7C\x39\x0F\x43\xD2\x69\x02\x1D\xFF\x82\x75"
                             "\xF3\x19\xF3\xBB\xA7\xC3\x8C\xF0\x5A\x7A\x1C\xF1\xE0\xB4\xED\x9D\xB3\xBC\x7B\x78"
                             "\xF8\x72\x1E\x87\xA1\xDD\x9C\x76\xCB\x4E\xF0\x21\xE2\x83\xE7\xD9\xDB\xD0\x4C\xC5"
                             "\x4F\x70\xD3\xE1\xAB\xC7\x74\xFB\xDE\x18\x2E\x8F\x7B\xAC\x6B\x3E\x1E\xCF\x88\x7B"
                             "\x0F\x7C\xF3\x04\x2C\x0C\xFB\x3B\xC7\x43\x3B\x08\x5B\x3C\x78\xFF\x1F\x0E\xE8\x2F"
                             "\xE0\xA7\xA1\xE8\x72\x91\xDE\x3C\x16\x98\x3B\xA7\xD0\x87\xE1\xC6\x77\x8F\x69\x69"
                             "\xF0\xD5\xE3\xBA\x7D\x9E\x42\x1C\x87\xD9\xDE\x3A\x17\x98\x4C\x3A\x62\x16\xF0\x8C"
                             "\xD8\x78\xD3\x07\x77\x4F\xC3\xE1\xC6\x77\x8F\x69\x78\xFF\x1F\x0E\xEE\x9E\x87\xA1"
                             "\xCA\xB3\xBC\x78\x3D\xC4\x08\x7A\x11\xE4\x30\x13\x30\xD3\xD0\xF4\x39\x5E\x3B\xC7"
                             "\x83\xDC\x4C\x2F\x33\xDB\xE3\xFC\x7C\x39\x67\xA1\xE9\x5E\x3C\x1E\xE2\x08\xF8\x77"
                             "\x41\x07\x0D\x15\x80\x97\x86\x9E\xB3\x9C\xCE\xF1\xDB\x23\x57\x8E\xE9\xF6\x79\x0D"
                             "\xD0\x4B\xB0\x77\x8F\xD1\xC6\x46\xC3\x9E\x22\x30\x46\x0F\x1A\x60\xEE\x8D\x3E\x02"
                             "\x16\xC2\x11\xE0\xF7\x69\x83\xBA\x77\x46\x9F\x0F\x87\x21\xDE\x3F\xC7\xF8\xEE\x90"
                             "\xD3\xE1\xF0\xE5\x1D\xE3\xBC\x7B\x4B\x4C\x02\x0E\x78\x27\xC1\x2F\x20\x3F\x0E\x33"
                             "\xBC\x7B\x4B\x4C\x1D\xD0\x8F\xC3\x8C\xEF\x1E\xD2\x08\xED\x9F\x0E\x7A\x99\xE0\xF7"
                             "\x1E\xE2\xF1\xFE\x3E\x04\x08\x59\xC1\xEE\xF1\xFE\x04\x3D\xE4\x68\xF8\x27\xEB\xA7"
                             "\x19\x11\x83\xBC\x7A\x1E\x87\x24\x3C\x10\xCA\x3D\xE0\xE8\xF7\xCF\x9E\x3C\x31\xC7"
                             "\x74\xFB\xA3\x8C\x81\x0F\x8A\x63\xE0\xCA\x3A\x1A\xF3\x78\xEE\x9D\xE3\xC1\xEE";
#define  HTTP_TIMER_SCRIPT3       Decompress(HTTP_TIMER_SCRIPT3_COMPRESSED,HTTP_TIMER_SCRIPT3_SIZE).c_str()
#else
const size_t HTTP_TIMER_SCRIPT3_SIZE = 424;
const char HTTP_TIMER_SCRIPT3_COMPRESSED[] PROGMEM = "\x30\x2F\x83\xAD\xCE\x5E\xA3\xBA\x77\x8F\x69\x9D\xFD\x69\xD4\x11\xD4\x34\xEA\xE3"
                             "\xA8\x61\xD5\xE3\xC0\xD3\xE1\xC6\x78\x2F\x1F\x0E\x33\xC1\x71\xF0\xE4\x3D\x0F\x4B"
                             "\x87\x82\xD3\x07\x75\x8E\x3B\xA7\xDD\x9C\x67\xD9\xDE\x3A\x10\x62\x98\x66\x8C\x43"
                             "\xBC\x7B\x7C\x7F\x8F\x9C\x78\x3D\xDC\x7C\x39\x0F\x43\xD2\x69\x02\x1D\xFF\x82\x75"
                             "\xF3\x19\xF3\xBB\xA7\xC3\x8C\xF0\x5A\x7A\x1C\xF1\xE0\xB4\xED\x9D\xB3\xBC\x7B\x78"
                             "\xF8\x72\x1E\x87\xA1\xDD\x9C\x76\xCB\x4E\xF0\x21\xE2\x83\xE7\xD9\xDB\xD0\x4C\xC5"
                             "\x4F\x76\x98\x3B\xA7\xD0\x87\xE1\xC6\x77\x8F\x69\x69\xF0\xD5\xE3\xBA\x7D\x9E\x42"
                             "\x1C\x87\xD9\xDE\x3A\x17\x98\x4C\x3A\x62\x16\xF0\x8C\xD8\x78\xD3\x07\x77\x4F\xC3"
                             "\xE1\xC6\x77\x8F\x69\x78\xFF\x1F\x0E\xEE\x9E\x87\xA1\xCA\xB3\xBC\x78\x3D\xC5\xE3"
                             "\xFC\x7C\x3B\xA6\xAF\x1D\xD3\xEC\xF2\x18\x09\x98\x69\xE8\x7A\x1C\xAF\x1D\xE3\xC1"
                             "\xEE\x26\x17\x99\xED\xF1\xFE\x3E\x1C\xB3\xD0\xF4\xAF\x1E\x0F\x71\x04\x7C\x3B\xA0"
                             "\x83\x86\x8A\xC0\x4B\xC3\x4F\x59\xCE\x67\x78\xED\x91\xAB\xC7\x74\xFB\x3C\x86\xE8"
                             "\x25\xD8\x3B\xC7\xE8\xE3\x23\x61\xCF\x11\x18\x23\x07\x8D\x30\x77\x46\x9F\x01\x0B"
                             "\x61\x08\x10\x75\xB0\x41\xCA\xC6\x8F\x82\x7E\x1E\x71\x91\x18\x3B\xC7\xA1\xE8\x72"
                             "\x43\xC1\x0C\xA3\xDE\x0E\x8F\x7C\xF9\xE3\xC3\x1C\x77\x4F\xBA\x38\xCF\xB3\xBC\x74"
                             "\x23\x3B\x08\x5B\x3E\x0C\xA3\xA1\xAF\x37\x8E\xE9\xDE\x3C\x1E\xE3";
#define  HTTP_TIMER_SCRIPT3       Decompress(HTTP_TIMER_SCRIPT3_COMPRESSED,HTTP_TIMER_SCRIPT3_SIZE).c_str()
#endif //USE_SUNRISE
#else
const char HTTP_TIMER_SCRIPT3[] PROGMEM =
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
    "if(%d>0){"
      "i=qs('#d1').selectedIndex;if(i>=0){s|=(i<<23);}"           // Get output
      "s|=(qs('#p1').selectedIndex<<27);"                         // Get action
    "}else{"
      "s|=3<<27;"                                                 // Get action (rule)
    "}"
    "l=((qs('#ho').selectedIndex*60)+qs('#mi').selectedIndex)&0x7FF;"
    "if(m==0){s|=l;}"                                             // Get time
#ifdef USE_SUNRISE
    "if((m==1)||(m==2)){"
      "if(qs('#dr').selectedIndex>0){if(l>0){l+=720;}}"           // If negative offset and delta-time > 0, add 12h to given offset time
      "s|=l&0x7FF;"                                               // Save offset instead of time
    "}"
#endif
    "s|=((qs('#mw').selectedIndex)&0x0F)<<11;"                    // Get window minutes
    "pt[ct]=s;"
    "eb('t0').value=pt.join();"                                   // Save parameters from array to hidden area
  "}";
#endif //USE_UNISHOX_COMPRESSION

#ifdef USE_UNISHOX_COMPRESSION
#ifdef USE_SUNRISE
const size_t HTTP_TIMER_SCRIPT4_SIZE = 548;
const char HTTP_TIMER_SCRIPT4_COMPRESSED[] PROGMEM = "\x30\x2F\x83\xAD\xCE\x59\x47\x76\x8E\xA6\x77\x8F\x69\x9D\xFD\x69\xD5\xC7\x56\x1D"
                             "\x43\x0E\xA3\x51\xD5\xE3\xC6\x98\x3B\xA1\xD1\xE8\x71\x23\xBC\x7B\x4B\xD4\x77\x4E"
                             "\xF1\xE0\xF7\x07\x47\xCA\x3C\x61\xF0\x4C\x0C\x58\xD7\xD4\x74\x1E\x74\x4C\x26\x35"
                             "\xF5\x78\x87\x19\x10\x61\x5F\xBC\x5D\x63\x59\xDD\x3E\xE8\x23\xEC\xEF\x1E\x0C\x67"
                             "\xCE\xEE\x9F\x0E\x33\xC1\x69\xE9\x87\x40\x9F\x0F\x50\xA3\xC6\x9D\xB3\xB6\x77\x8F"
                             "\x6E\x1E\xF6\x9E\xF9\xD3\xD4\x64\x13\x3A\x07\xEF\x15\x33\x65\x1F\x0F\x60\xEB\x0C"
                             "\xD0\x7B\xF8\x2F\x84\x3C\xCF\x23\xE8\xE3\xE2\x36\x1E\x03\xC0\xB3\xE0\x85\x20\xC6"
                             "\x75\x1D\x63\xEF\x47\x85\x51\xE7\xD9\xF1\xB6\x11\xE0\xF6\x1E\xE6\x0C\x53\x1F\x1D"
                             "\x81\x08\x78\x3D\x87\x8F\x1F\x06\x51\xEF\x07\x47\xBE\x78\x18\x7C\x3B\xBE\x3F\x0F"
                             "\xC3\x94\x8E\xF1\xFA\xB3\xC1\x31\xC7\x74\xFB\x1C\x7D\x9D\xB1\x87\x78\xE8\x18\xA6"
                             "\x19\xA3\x10\xF8\x72\x1E\x08\x7A\x8E\xE9\xDE\x3C\x1A\x8F\x87\x77\xC7\xE1\xF8\x72"
                             "\x43\xBC\x7E\x99\x1B\x08\xC1\xE3\x4C\x1D\xD3\x51\xE8\x72\x33\xBC\x7B\x48\xD4\x7C"
                             "\x3E\xCE\x33\xEC\xED\x91\xA8\xF0\x7B\x8D\x5E\x3B\xA7\xD9\xE4\x34\x7C\xFB\x3B\xC7"
                             "\x43\x3B\x08\x5B\x3E\x1A\x81\x1B\x85\xB3\x9E\x20\x41\xE1\x50\x10\x74\x43\xBA\x72"
                             "\x71\xDB\x2D\x3B\xC7\x78\xFD\x1C\x87\x82\x63\x8E\xE9\xF6\x3E\x7D\x9D\xBD\x04\x5D"
                             "\x20\x61\xE0\xF7\x69\x83\xBA\x7D\x08\x7E\x1C\x64\x08\x78\x51\xCA\xB2\x04\x1D\x34"
                             "\xD5\xE3\xBA\x7D\x9E\x42\x1C\x84\x08\x99\xD8\xC3\xB6\x72\x10\x21\xF0\x28\x73\xC7"
                             "\x78\xFD\x59\x02\x0D\xC1\x87\x21\xF6\x77\x8E\x85\xE6\x13\x0E\x98\x85\xBC\x23\x36"
                             "\x1F\x06\x1E\x0F\x70\x20\xE0\x67\x26\x90\x21\xE9\xFF\x38\xCF\xB2\x04\x7D\x38\x10"
                             "\x6D\x9C\xB8\x40\x87\x6E\xC1\x26\xD9\xEE";
#define  HTTP_TIMER_SCRIPT4       Decompress(HTTP_TIMER_SCRIPT4_COMPRESSED,HTTP_TIMER_SCRIPT4_SIZE).c_str()
#else
const size_t HTTP_TIMER_SCRIPT4_SIZE = 620;
const char HTTP_TIMER_SCRIPT4_COMPRESSED[] PROGMEM = "\x30\x2F\x83\xAD\xCE\x59\x47\x76\x8E\xA6\x77\x8F\x69\x9D\xFD\x69\xD5\xC7\x56\x1D"
                             "\x43\x0E\xA3\x51\xD5\xE3\xC6\x98\x3B\xA1\xD1\xE8\x71\x23\xBC\x7B\x4B\xD4\x77\x4E"
                             "\xF1\xE0\xF7\x07\x47\xCA\x3C\x61\xF0\x4C\x0C\x58\xD7\xD4\x74\x1E\x74\x4C\x26\x35"
                             "\xF5\x78\x87\x19\x10\x61\x5F\xBC\x5D\x63\x59\xDD\x3E\xE8\x23\xEC\xEF\x1E\x0C\x67"
                             "\xCE\xEE\x9F\x0E\x33\xC1\x69\xE9\x87\x40\x9F\x0F\x50\xA3\xC6\x9D\xB3\xB6\x77\x8F"
                             "\x6E\x1E\xF6\x9E\xF9\xD3\xD4\x64\x13\x3A\x07\xEF\x15\x33\x65\x1F\x0F\x60\xEB\x0C"
                             "\xD0\x7B\xF8\x2F\x84\x3C\xCF\x23\xE8\xE3\xE2\x36\x1E\x03\xC0\xB3\xE0\x85\x20\xC6"
                             "\x75\x1D\x63\xEF\x47\x85\x51\xE7\xD9\xF1\xB6\x11\xE0\xF6\x1E\xE6\x0C\x53\x1F\x1D"
                             "\x81\x08\x78\x3D\x87\x8F\x1F\x06\x51\xEF\x07\x47\xBE\x78\x18\x7C\xF1\xFA\x38\xC8"
                             "\xD8\x73\xC4\x46\x08\xC1\xE0\xD4\x7C\x21\xB7\x42\x8E\x86\x02\xCC\xF9\xDD\x18\x76"
                             "\x1C\xE6\x77\x8F\x05\xA6\x0E\xE9\xA8\xF4\x39\x19\xDE\x3D\xA4\x6A\x3E\x1F\x67\x19"
                             "\xF6\x76\xC8\xD4\x78\x3D\xC6\xAF\x1D\xD3\xEC\xF2\x15\x87\xD9\xDE\x3A\x19\xD8\x42"
                             "\xD9\xF0\xD4\x78\x35\x1F\x06\x1F\x47\xD1\xCE\x64\x0A\x78\x40\xDD\x04\x8C\x20\xEE"
                             "\xF8\xFC\x3F\x0E\x48\x77\x8F\xD3\x23\x61\x18\x05\x4C\x38\x7C\x11\xB0\xE0\x45\xE2"
                             "\x8C\xE7\x88\x10\x78\x9C\x18\x7C\x3B\xBE\x3F\x0F\xC3\xBA\x72\x71\xDB\x2D\x3B\xC7"
                             "\x78\xFD\x1C\x87\x82\x63\x8E\xE9\xF6\x3E\x7D\x9D\xBD\x3B\xC7\x40\xC5\x30\xCD\x18"
                             "\x87\xC1\x87\x83\xDD\xA6\x0E\xE9\xF4\x21\xF8\x71\x90\x21\xE1\x47\x2A\x2B\xC8\x10"
                             "\x74\xD3\x57\x8E\xE9\xF6\x79\x08\x72\x10\x22\x67\x63\x0E\xD9\xC8\x78\x20\x42\xBC"
                             "\x73\xC7\x78\xFD\x59\x02\x0D\xC1\x87\x21\xF6\x77\x8E\x85\xE6\x13\x0E\x98\x85\xBC"
                             "\x23\x36\x1F\x06\x1E\x0F\x70\x20\xE0\x67\x26\x90\x21\xE9\xFF\x38\xCF\xB2\x04\x7D"
                             "\x38\x10\x6D\x9C\xB8\x40\x87\x6E\xC1\x26\xD9\xEE";
#define  HTTP_TIMER_SCRIPT4       Decompress(HTTP_TIMER_SCRIPT4_COMPRESSED,HTTP_TIMER_SCRIPT4_SIZE).c_str()
#endif //USE_SUNRISE
#else
const char HTTP_TIMER_SCRIPT4[] PROGMEM =
  "function ot(t,e){"                                             // Select tab and update elements
    "var i,n,o,p,q,s;"
    "if(ct<99){st();}"                                            // Save changes
    "ct=t;"
    "o=document.getElementsByClassName('tl');"                    // Restore style to all tabs/buttons
    "for(i=0;i<o.length;i++){o[i].style.cssText=\"background:#%06x;color:#%06x;font-weight:normal;\"}"  // COLOR_TIMER_TAB_BACKGROUND, COLOR_TIMER_TAB_TEXT
    "e.style.cssText=\"background:#%06x;color:#%06x;font-weight:bold;\";"  // COLOR_FORM, COLOR_TEXT, Change style to tab/button used to open content
    "s=pt[ct];"                                                   // Get parameters from array
#ifdef USE_SUNRISE
    "p=(s>>29)&3;eb('b'+p).checked=1;"                            // Set mode
    "gt();"                                                       // Set hours and minutes according to mode
#else
    "p=s&0x7FF;"                                                  // Get time
    "q=Math.floor(p/60);if(q<10){q='0'+q;}qs('#ho').value=q;"     // Set hours
    "q=p%%60;if(q<10){q='0'+q;}qs('#mi').value=q;"                // Set minutes
#endif
    "q=(s>>11)&0xF;if(q<10){q='0'+q;}qs('#mw').value=q;"          // Set window minutes
    "for(i=0;i<7;i++){p=(s>>(16+i))&1;eb('w'+i).checked=p;}"      // Set weekdays
    "if(%d>0){"
      "p=(s>>23)&0xF;qs('#d1').value=p+1;"                        // Set output
      "p=(s>>27)&3;qs('#p1').selectedIndex=p;"                    // Set action
    "}"
    "p=(s>>15)&1;eb('r0').checked=p;"                             // Set repeat
    "p=(s>>31)&1;eb('a0').checked=p;"                             // Set arm
  "}";
#endif //USE_UNISHOX_COMPRESSION

const char HTTP_TIMER_SCRIPT5[] PROGMEM =
  "function it(){"                                                // Initialize elements and select first tab
    "var b,i,o,s;"
    "pt=eb('t0').value.split(',').map(Number);"                   // Get parameters from hidden area to array
    "s='';"
    "for(i=0;i<%d;i++){"
      "b='';"
      "if(0==i){b=\" id='dP'\";}"
      "s+=\"<button type='button' class='tl' onclick='ot(\"+i+\",this)'\"+b+\">\"+(i+1)+\"</button>\""
    "}"
    "eb('bt').innerHTML=s;"                                       // Create tabs
    "if(%d>0){"                                                   // Create Output and Action drop down boxes
      "eb('oa').innerHTML=\"<b>" D_TIMER_OUTPUT "</b>&nbsp;<span><select style='width:60px;' id='d1'></select></span>&emsp;<b>" D_TIMER_ACTION "</b>&nbsp;<select style='width:99px;' id='p1'></select>\";"
      "o=qs('#p1');ce('" D_OFF "',o);ce('" D_ON "',o);ce('" D_TOGGLE "',o);"  // Create offset direction select options
#if defined(USE_RULES) || defined(USE_SCRIPT)
      "ce('" D_RULE "',o);"
#else
      "ce('" D_BLINK "',o);"
#endif
    "}else{"
      "eb('oa').innerHTML=\"<b>" D_TIMER_ACTION "</b> " D_RULE "\";"  // No outputs but rule is allowed
    "}";
const char HTTP_TIMER_SCRIPT6[] PROGMEM =
#ifdef USE_SUNRISE
    "o=qs('#dr');ce('+',o);ce('-',o);"                            // Create offset direction select options
#endif
    "o=qs('#ho');for(i=0;i<=23;i++){ce((i<10)?('0'+i):i,o);}"     // Create hours select options
    "o=qs('#mi');for(i=0;i<=59;i++){ce((i<10)?('0'+i):i,o);}"     // Create minutes select options
    "o=qs('#mw');for(i=0;i<=15;i++){ce((i<10)?('0'+i):i,o);}"     // Create window minutes select options
    "o=qs('#d1');for(i=0;i<%d;i++){ce(i+1,o);}"                   // Create outputs
    "var a='" D_DAY3LIST "';"

//    "s='';for(i=0;i<7;i++){s+=\"<input id='w\"+i+\"' type='checkbox'><b>\"+a.substring(i*3,(i*3)+3)+\"</b> \"}"
//    "s='';for(i=0;i<7;i++){s+=\"<input id='w\"+i+\"' type='checkbox'><label for='w\"+i+\"'>\"+a.substring(i*3,(i*3)+3)+\"</label> \"}"
    "s='';for(i=0;i<7;i++){s+=\"<label><input id='w\"+i+\"' type='checkbox'><b>\"+a.substring(i*3,(i*3)+3)+\"</b></label> \"}"

    "eb('ds').innerHTML=s;"                                       // Create weekdays
    "eb('dP').click();"                                           // Get the element with id='dP' and click on it
  "}"
  "wl(it);";
const char HTTP_TIMER_STYLE[] PROGMEM =
  ".tl{float:left;border-radius:0;border:1px solid #%06x;padding:1px;width:6.25%%;}";  // COLOR_FORM, Border color needs to be the same as Fieldset background color from HTTP_HEAD_STYLE1 (transparent won't work)
const char HTTP_FORM_TIMER1[] PROGMEM =
  "<fieldset style='min-width:470px;text-align:center;'>"
  "<legend style='text-align:left;'><b>&nbsp;" D_TIMER_PARAMETERS "&nbsp;</b></legend>"
  "<form method='post' action='" WEB_HANDLE_TIMER "' onsubmit='return st();'>"
  "<br><label><input id='e0' type='checkbox'%s><b>" D_TIMER_ENABLE "</b></label><br><br><hr>"
  "<input id='t0' value='";
const char HTTP_FORM_TIMER2[] PROGMEM =
  "' hidden><div id='bt'></div><br><br><br>"
  "<div id='oa' name='oa'></div><br>"
  "<div>"
  "<label><input id='a0' type='checkbox'><b>" D_TIMER_ARM "</b></label>&emsp;"
  "<label><input id='r0' type='checkbox'><b>" D_TIMER_REPEAT "</b></label>"
  "</div><br>"
  "<div>";
#ifdef USE_SUNRISE
const char HTTP_FORM_TIMER3[] PROGMEM =
  "<fieldset style='width:%dpx;margin:auto;text-align:left;border:0;'>"
  "<label><input id='b0' name='rd' type='radio' value='0' onclick='gt();'><b>" D_TIMER_TIME "</b></label><br>"
  "<label><input id='b1' name='rd' type='radio' value='1' onclick='gt();'><b>" D_SUNRISE "</b> (%s)</label><br>"
  "<label><input id='b2' name='rd' type='radio' value='2' onclick='gt();'><b>" D_SUNSET "</b> (%s)</label><br>"
  "</fieldset>"
  "<p></p>"
  "<span><select style='width:46px;' id='dr'></select></span>"
  "&nbsp;";
#else
const char HTTP_FORM_TIMER3[] PROGMEM =
  "<b>" D_TIMER_TIME "</b>&nbsp;";
#endif  // USE_SUNRISE

#ifdef USE_UNISHOX_COMPRESSION
const size_t HTTP_FORM_TIMER4_SIZE = 249;
const char HTTP_FORM_TIMER4_COMPRESSED[] PROGMEM = "\x3D\x3C\x32\xF8\xFC\x3D\x3C\xC2\x61\xD2\xF5\x19\x04\xCF\x87\xD8\xFE\x89\x42\x8F"
                             "\x33\x9C\xC8\x61\xB0\xF0\x7D\xAD\x10\xF8\x7D\x8A\xC3\xEC\xFC\x3D\x0E\xC0\x41\xC0"
                             "\x4F\xC3\xD0\xEC\xF0\xCB\xE3\xF0\xFD\x70\xEF\x0C\x3C\x1F\x5E\x04\x18\x80\xC0\x72"
                             "\x41\xBA\x09\xD9\x23\x1B\xE1\x87\x83\xD0\x71\xF8\x76\xCE\xC3\xAC\xF4\x3B\x07\x02"
                             "\x16\x68\x0C\x0B\x2C\x1F\x04\xDC\xB0\xF4\x3B\x04\xD3\x33\xF0\xF4\x1D\xF3\xF0\xF4"
                             "\x13\x4C\xD6\x88\x7C\x3E\xC4\xF1\xF6\xBA\xC6\xB3\xE1\xF6\x27\x8F\xB0\x42\xBA";
#define  HTTP_FORM_TIMER4       Decompress(HTTP_FORM_TIMER4_COMPRESSED,HTTP_FORM_TIMER4_SIZE).c_str()
#else
const char HTTP_FORM_TIMER4[] PROGMEM =
  "<span><select style='width:60px;' id='ho'></select></span>"
  "&nbsp;" D_HOUR_MINUTE_SEPARATOR "&nbsp;"
  "<span><select style='width:60px;' id='mi'></select></span>"
  "&emsp;<b>+/-</b>&nbsp;"
  "<span><select style='width:60px;' id='mw'></select></span>"
  "</div><br>"
  "<div id='ds' name='ds'></div>";
#endif //USE_UNISHOX_COMPRESSION

void HandleTimerConfiguration(void)
{
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_CONFIGURE_TIMER));

  if (Webserver->hasArg(F("save"))) {
    TimerSaveSettings();
    HandleConfiguration();
    return;
  }

  WSContentStart_P(PSTR(D_CONFIGURE_TIMER));
  WSContentSend_P(HTTP_TIMER_SCRIPT1);
#ifdef USE_SUNRISE
  WSContentSend_P(HTTP_TIMER_SCRIPT2);
#endif  // USE_SUNRISE
  WSContentSend_P(HTTP_TIMER_SCRIPT3, TasmotaGlobal.devices_present);
  WSContentSend_P(HTTP_TIMER_SCRIPT4, WebColor(COL_TIMER_TAB_BACKGROUND), WebColor(COL_TIMER_TAB_TEXT), WebColor(COL_FORM), WebColor(COL_TEXT), TasmotaGlobal.devices_present);
  WSContentSend_P(HTTP_TIMER_SCRIPT5, MAX_TIMERS, TasmotaGlobal.devices_present);
  WSContentSend_P(HTTP_TIMER_SCRIPT6, TasmotaGlobal.devices_present);
  WSContentSendStyle_P(HTTP_TIMER_STYLE, WebColor(COL_FORM));
  WSContentSend_P(HTTP_FORM_TIMER1, (Settings.flag3.timers_enable) ? PSTR(" checked") : "");  // CMND_TIMERS
  for (uint32_t i = 0; i < MAX_TIMERS; i++) {
    WSContentSend_P(PSTR("%s%u"), (i > 0) ? "," : "", Settings.timer[i].data);
  }
  WSContentSend_P(HTTP_FORM_TIMER2);
#ifdef USE_SUNRISE
  WSContentSend_P(HTTP_FORM_TIMER3, 100 + (strlen(D_SUNSET) *12), GetSun(0).c_str(), GetSun(1).c_str());
#else
  WSContentSend_P(HTTP_FORM_TIMER3);
#endif  // USE_SUNRISE
#ifdef USE_UNISHOX_COMPRESSION
  WSContentSend_P(HTTP_FORM_TIMER4,D_HOUR_MINUTE_SEPARATOR);
#else
  WSContentSend_P(HTTP_FORM_TIMER4);
#endif //USE_UNISHOX_COMPRESSION
  WSContentSend_P(HTTP_FORM_END);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();
}

void TimerSaveSettings(void)
{
  char tmp[MAX_TIMERS *12];  // Need space for MAX_TIMERS x 10 digit numbers separated by a comma
  char message[32 + (MAX_TIMERS *11)];  // MQT: Timers 0,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
  Timer timer;

  Settings.flag3.timers_enable = Webserver->hasArg(F("e0"));  // CMND_TIMERS
  WebGetArg(PSTR("t0"), tmp, sizeof(tmp));
  char *p = tmp;
  snprintf_P(message, sizeof(message), PSTR(D_LOG_MQTT D_CMND_TIMERS " %d"), Settings.flag3.timers_enable);  // CMND_TIMERS
  for (uint32_t i = 0; i < MAX_TIMERS; i++) {
    timer.data = strtol(p, &p, 10);
    p++;  // Skip comma
    if (timer.time < 1440) {
      bool flag = (timer.window != Settings.timer[i].window);
      Settings.timer[i].data = timer.data;
      if (flag) TimerSetRandomWindow(i);
    }
    snprintf_P(message, sizeof(message), PSTR("%s,0x%08X"), message, Settings.timer[i].data);
  }
  AddLogData(LOG_LEVEL_DEBUG, message);
}
#endif  // USE_TIMERS_WEB
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv09(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_PRE_INIT:
      TimerSetRandomWindows();
      break;
#ifdef USE_WEBSERVER
#ifdef USE_TIMERS_WEB
    case FUNC_WEB_ADD_BUTTON:
#if defined(USE_RULES) || defined(USE_SCRIPT)
      WSContentSend_P(HTTP_BTN_MENU_TIMER);
#else
      if (TasmotaGlobal.devices_present) { WSContentSend_P(HTTP_BTN_MENU_TIMER); }
#endif  // USE_RULES
      break;
    case FUNC_WEB_ADD_HANDLER:
      WebServer_on(PSTR("/" WEB_HANDLE_TIMER), HandleTimerConfiguration);
      break;
#endif  // USE_TIMERS_WEB
#endif  // USE_WEBSERVER
    case FUNC_EVERY_SECOND:
      TimerEverySecond();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kTimerCommands, TimerCommand);
      break;
  }
  return result;
}

#endif  // USE_TIMERS
