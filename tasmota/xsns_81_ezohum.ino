/*
  xsns_81_ezortd.ino - EZO HUM I2C HUM sensor support for Tasmota

  Copyright (C) 2020  Christopher Tremblay

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

#ifdef USE_I2C
#ifdef USE_EZOHUM

#define XSNS_81 81

#define EZO_HUM_READ_LATENCY   300

struct EZOHUM : public EZOStruct {
  void ProcessMeasurement(void)
  {
    char data[D_EZO_MAX_BUF];
    // Hum, Temp, Dew

    EZOStruct::ProcessMeasurement(data, sizeof(data), EZO_HUM_READ_LATENCY);

    humidity = CharToFloat(data);

    char *next = strchr(data, ',');
    if (next) {
      temperature = CharToFloat(next + 1);
    }
  }

  void Show(bool json, uint32_t index)
  {
    if (valid) {
      char name[10];
      snprintf_P(name, sizeof(name), PSTR("%s%c%X"), EZOHUM::name, IndexSeparator(), index + 1);

      if (count == 1) {
        name[sizeof("EZOHUM") - 1] = 0;
      }

      if (isnan(temperature)) {
        char parameter[FLOATSZ];

        dtostrfd(humidity, Settings.flag2.humidity_resolution, parameter);
        WSContentSend_PD(HTTP_SNS_HUM, name, parameter);
      } else {
        TempHumDewShow(json, (0 == tele_period), name, temperature, humidity);
      }
    }
  }

  static int8_t     count;
  static EZOHUM    *list;

private:
  static const char name[];

  float     humidity    = NAN;
  float     temperature = NAN;
};

int8_t      EZOHUM::count           = -1;
EZOHUM     *EZOHUM::list            = NULL;
const char  EZOHUM::name[]  PROGMEM = "EZOHUM";


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
#define Xsns81 XsnsEZO<EZOHUM, EZO_HUM>

#endif  // USE_EZOHUM
#endif  // USE_I2C
