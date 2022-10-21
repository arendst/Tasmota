/*
  xsns_78_ezoprs.ino - EZO PRS I2C PRS sensor support for Tasmota

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
#ifdef USE_EZOPRS

#define EZO_PRS_READ_LATENCY   900

struct EZOPRS : public EZOStruct {
  EZOPRS(uint32_t addr) : EZOStruct(addr), pressure(NAN) {}

  virtual void ProcessMeasurement(void)
  {
    char data[D_EZO_MAX_BUF];

    EZOStruct::ProcessMeasurement(data, sizeof(data), EZO_PRS_READ_LATENCY);
    pressure = CharToFloat(data) * 68.9476f; // Convert to hPa
  }

  virtual void Show(bool json, const char *name)
  {
    char str[33];
    char sealevelstr[33];
    dtostrfd(ConvertPressure(pressure), Settings->flag2.pressure_resolution, str);
    dtostrfd(ConvertPressureForSeaLevel(pressure), Settings->flag2.pressure_resolution, sealevelstr);

    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_PRESSURE "\":%s"), name, str);
      if (Settings->altitude != 0) {
        ResponseAppend_P(PSTR(",\"" D_JSON_PRESSUREATSEALEVEL "\":%s"), sealevelstr);
      }
      ResponseJsonEnd();
#ifdef USE_WEBSERVER
    }else {
      WSContentSend_PD(HTTP_SNS_PRESSURE, name, str, PressureUnit().c_str());
      if (Settings->altitude != 0) {
        WSContentSend_PD(HTTP_SNS_SEAPRESSURE, name, sealevelstr, PressureUnit().c_str());
      }
#endif  // USE_WEBSERVER
    }
  }

  static const char id[] PROGMEM;

private:
  float     pressure;
};

const char EZOPRS::id[]  PROGMEM = "PRS";

#endif  // USE_EZOPRS
#endif  // USE_I2C
