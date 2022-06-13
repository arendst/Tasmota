/*
  xsns_78_ezoo2.ino - EZO O2 I2C O2 sensor support for Tasmota

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
#ifdef USE_EZOO2

#define EZO_O2_READ_LATENCY   900

struct EZOO2 : public EZOStruct {
  EZOO2(uint32_t addr) : EZOStruct(addr), O2(0) {}

  virtual void ProcessMeasurement(void)
  {
    char data[D_EZO_MAX_BUF];

    EZOStruct::ProcessMeasurement(data, sizeof(data), EZO_O2_READ_LATENCY);
    O2 = CharToFloat(data);
  }

  virtual void Show(bool json, const char *name)
  {
    char str[8];
    dtostrfd(O2, 2, str);

    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_O2 "\":%s}" ), name, str);
#ifdef USE_WEBSERVER
    }else {
      WSContentSend_PD(HTTP_SNS_O2, name, str);
#endif  // USE_WEBSERVER
    }
  }

  static const char id[] PROGMEM;

private:
  float O2;
};

const char EZOO2::id[] PROGMEM = "O2";

#endif  // USE_EZOO2
#endif  // USE_I2C
