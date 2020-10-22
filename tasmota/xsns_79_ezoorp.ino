/*
  xsns_79_ezoorp.ino - EZO ORP I2C ORP sensor support for Tasmota

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
#ifdef USE_EZOORP

#define XSNS_79 79

#define EZO_ORP_READ_LATENCY   900

struct EZOORP : public EZOStruct {
  void ProcessMeasurement(void)
  {
    char data[D_EZO_MAX_BUF];

    EZOStruct::ProcessMeasurement(data, sizeof(data), EZO_ORP_READ_LATENCY);
    ORP = CharToFloat(data);
  }

  void Show(bool json, uint32_t index)
  {
    if (valid) {
      char str[6];
      dtostrfd(ORP, 2, str);

      char name[10];
      snprintf_P(name, sizeof(name), PSTR("%s%c%X"), EZOORP::name, IndexSeparator(), index + 1);

      if (count == 1) {
        name[sizeof("EZOORP") - 1] = 0;
      }

      if (json) {
        ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_ORP "\":%s}" ), name, str);
      }
#ifdef USE_WEBSERVER  
      else {
        WSContentSend_PD(HTTP_SNS_ORP, name, str);
#endif  // USE_WEBSERVER
      }
    }
  }

  static int8_t     count;
  static EZOORP    *list;
  static const char name[];

private:
  float     ORP = NAN;
};

int8_t      EZOORP::count           = -1;
EZOORP     *EZOORP::list            = NULL;
const char  EZOORP::name[]  PROGMEM = "EZOORP";


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
#define Xsns79 XsnsEZO<EZOORP, EZO_ORP>

#endif  // USE_EZOORP
#endif  // USE_I2C
