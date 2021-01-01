/*
  xsns_78_ezoph.ino - EZO pH I2C pH sensor support for Tasmota

  Copyright (C) 2021  Christopher Tremblay

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
#ifdef USE_EZOPH

#define EZO_PH_READ_LATENCY   900

struct EZOPH : public EZOStruct {
  EZOPH(uint32_t addr) : EZOStruct(addr), pH(NAN) {}

  virtual void ProcessMeasurement(void)
  {
    char data[D_EZO_MAX_BUF];

    EZOStruct::ProcessMeasurement(data, sizeof(data), EZO_PH_READ_LATENCY);
    pH    = CharToFloat(data);
  }

  virtual void Show(bool json, const char *name)
  {
    char str[6];
    dtostrfd(pH, 2, str);

    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_PH "\":%s}" ), name, str);
    }
#ifdef USE_WEBSERVER
    else {
      WSContentSend_PD(HTTP_SNS_PH, name, str);
#endif  // USE_WEBSERVER
    }
  }

  static const char id[] PROGMEM;

private:
  float     pH;
};

const char EZOPH::id[]  PROGMEM = "pH";

#endif  // USE_EZOPH
#endif  // USE_I2C
