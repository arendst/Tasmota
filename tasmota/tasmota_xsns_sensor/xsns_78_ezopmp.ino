/*
  xsns_78_ezopmp.ino - EZO PMP I2C PMP sensor support for Tasmota

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
#ifdef USE_EZOPMP

#define EZO_PMP_READ_LATENCY   300

struct EZOPMP : public EZOStruct {
  EZOPMP(uint32_t addr) : EZOStruct(addr), volume(NAN) {}

  virtual void ProcessMeasurement(void)
  {
    char data[D_EZO_MAX_BUF];

    EZOStruct::ProcessMeasurement(data, sizeof(data), EZO_PMP_READ_LATENCY);

    volume = CharToFloat(data);
  }

  virtual void Show(bool json, const char *name)
  {
    char str[FLOATSZ];

    dtostrfd(volume, 1, str);

    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_PUMP "\":%s"), name, str);
      ResponseJsonEnd();

#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_MILLILITERS, name, str);
#endif
    }
  }

  static const char id[] PROGMEM;

private:
  float     volume;
};

const char EZOPMP::id[] PROGMEM = "PMP";

#endif  // USE_EZOPMP
#endif  // USE_I2C
