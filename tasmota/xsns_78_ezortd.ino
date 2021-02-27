/*
  xsns_78_ezortd.ino - EZO RTD I2C RTD sensor support for Tasmota

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
#ifdef USE_EZORTD

#define EZO_RTD_READ_LATENCY   600

struct EZORTD : public EZOStruct {
  EZORTD(uint32_t addr) : EZOStruct(addr), temperature(NAN) {}

  virtual void ProcessMeasurement(void)
  {
    char data[D_EZO_MAX_BUF];

    EZOStruct::ProcessMeasurement(data, sizeof(data), EZO_RTD_READ_LATENCY);
    temperature = CharToFloat(data);
  }

  virtual void Show(bool json, const char *name)
  {
    float temp = ConvertTemp(temperature);

    if (json) {
      ResponseAppend_P(JSON_SNS_F_TEMP, name, Settings.flag2.temperature_resolution, &temp);
#ifdef USE_WEBSERVER
    }else {
      WSContentSend_Temp(name, temp);
#endif  // USE_WEBSERVER
    }
  }

  static const char id[] PROGMEM;

private:
  float     temperature;
};

const char EZORTD::id[] PROGMEM = "RTD";

#endif  // USE_EZORTD
#endif  // USE_I2C
