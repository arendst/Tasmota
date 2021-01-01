/*
  xsns_78_ezoflo.ino - EZO FLO I2C FLO sensor support for Tasmota

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
#ifdef USE_EZOFLO

#define EZO_FLO_READ_LATENCY   300

struct EZOFLO : public EZOStruct {
  EZOFLO(uint32_t addr) : EZOStruct(addr), totalVolume(NAN), flowRate(NAN) {}

  virtual void ProcessMeasurement(void)
  {
    char data[D_EZO_MAX_BUF];
    // Total Volume, Flow Rate

    EZOStruct::ProcessMeasurement(data, sizeof(data), EZO_FLO_READ_LATENCY);

    totalVolume = CharToFloat(data);

    char *next = strchr(data, ',');
    if (next) {
      flowRate = CharToFloat(next + 1);
    }
  }

  virtual void Show(bool json, const char *name)
  {
    char volume[FLOATSZ];
    char rate[FLOATSZ];
    bool hasFlow = !isnan(flowRate);

    dtostrfd(totalVolume, 2, volume);

    dtostrfd(flowRate, 2, rate);

    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_VOLUME "\":%s"), name, volume);
      if (hasFlow) {
        ResponseAppend_P(PSTR(",\"" D_JSON_FLOWRATE "\":%s"), rate);
      }
      ResponseJsonEnd();

#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_LITERS, name, volume);

      if (hasFlow) {
        WSContentSend_PD(HTTP_SNS_LPM, name, rate);
      }
#endif
    }
  }

  static const char id[] PROGMEM;

private:
  float     totalVolume;
  float     flowRate;
};

const char EZOFLO::id[] PROGMEM = "FLO";

#endif  // USE_EZOFLO
#endif  // USE_I2C
