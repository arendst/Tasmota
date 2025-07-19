/*
  xsns_78_ezoco2.ino - EZO CO2 I2C CO2 sensor support for Tasmota

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
#ifdef USE_EZOCO2

#define EZO_CO2_READ_LATENCY   900

struct EZOCO2 : public EZOStruct {
  EZOCO2(uint32_t addr) : EZOStruct(addr), CO2(0) {}

  virtual void ProcessMeasurement(void)
  {
    char data[D_EZO_MAX_BUF];

    EZOStruct::ProcessMeasurement(data, sizeof(data), EZO_CO2_READ_LATENCY);

    // sensor has a 10s warmup period
    if (TasmotaGlobal.uptime >= 10) {
      CO2 = atoi(data);
#ifdef USE_LIGHT
      LightSetSignal(CO2_LOW, CO2_HIGH, CO2);
#endif  // USE_LIGHT
    }
  }

  virtual void Show(bool json, const char *name)
  {
    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_CO2 "\":%d}" ), name, CO2);
#ifdef USE_WEBSERVER
    }else {
      WSContentSend_PD(HTTP_SNS_CO2, name, CO2);
#endif  // USE_WEBSERVER
    }
  }

  static const char id[] PROGMEM;

private:
  uint16_t  CO2;
};

const char EZOCO2::id[] PROGMEM = "CO2";

#endif  // USE_EZOCO2
#endif  // USE_I2C
