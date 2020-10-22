/*
  xsns_80_ezortd.ino - EZO RTD I2C RTD sensor support for Tasmota

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
#ifdef USE_EZORTD

#define XSNS_80 80

#define EZO_RTD_READ_LATENCY   600

struct EZORTD : public EZOStruct {
  void ProcessMeasurement(void)
  {
    char data[D_EZO_MAX_BUF];

    EZOStruct::ProcessMeasurement(data, sizeof(data), EZO_RTD_READ_LATENCY);
    RTD = CharToFloat(data);
  }

  void Show(bool json, uint32_t index)
  {
    if (valid) {
      char str[10];
      dtostrfd(ConvertTemp(RTD), 2, str);

      char name[10];
      snprintf_P(name, sizeof(name), PSTR("%s%c%X"), EZORTD::name, IndexSeparator(), index + 1);

      if (count == 1) {
        name[sizeof("EZORTD") - 1] = 0;
      }

      if (json) {
        ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_TEMPERATURE "\":%s}"), name, str);
      }
#ifdef USE_WEBSERVER  
      else {
        WSContentSend_PD(HTTP_SNS_TEMP, name, str, TempUnit());
#endif  // USE_WEBSERVER
      }
    }
  }

  static int8_t     count;
  static EZORTD    *list;
  static const char name[];

private:
  float     RTD = NAN;
};

int8_t      EZORTD::count           = -1;
EZORTD     *EZORTD::list            = NULL;
const char  EZORTD::name[]  PROGMEM = "EZORTD";


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
#define Xsns80 XsnsEZO<EZORTD, EZO_RTD>

#endif  // USE_EZORTD
#endif  // USE_I2C
