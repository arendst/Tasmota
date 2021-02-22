/*
  xsns_78_ezoorp.ino - EZO ORP I2C ORP sensor support for Tasmota

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
#ifdef USE_EZOORP

#define EZO_ORP_READ_LATENCY   900

struct EZOORP : public EZOStruct {
  EZOORP(uint32_t addr) : EZOStruct(addr), ORP(NAN) {}

  virtual void ProcessMeasurement(void)
  {
    char data[D_EZO_MAX_BUF];

    EZOStruct::ProcessMeasurement(data, sizeof(data), EZO_ORP_READ_LATENCY);
    ORP = CharToFloat(data);
  }

  virtual void Show(bool json, const char *name)
  {
    char str[8];
    dtostrfd(ORP, 2, str);

    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_ORP "\":%s}" ), name, str);
#ifdef USE_WEBSERVER
    }else {
      WSContentSend_PD(HTTP_SNS_ORP, name, str);
#endif  // USE_WEBSERVER
    }
  }

  static const char id[] PROGMEM;

private:
  float     ORP;
};

const char EZOORP::id[] PROGMEM = "ORP";

#endif  // USE_EZOORP
#endif  // USE_I2C
